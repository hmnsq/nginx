
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


#यदि (NGX_HAVE_ATOMIC_OPS)


स्थिर शून्य ngx_shmtx_wakeup(ngx_shmtx_t *mtx);


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    mtx->lock = &addr->lock;

    यदि (mtx->spin == (ngx_uint_t) -1) {
        प्रत्यागम NGX_OK;
    }

    mtx->spin = 2048;

#यदि (NGX_HAVE_POSIX_SEM)

    mtx->रुको = &addr->रुको;

    यदि (sem_init(&mtx->sem, 1, 0) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_init() failed");
    } अन्यथा {
        mtx->semaphore = 1;
    }

#endif

    प्रत्यागम NGX_OK;
}


शून्य
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
#यदि (NGX_HAVE_POSIX_SEM)

    यदि (mtx->semaphore) {
        यदि (sem_destroy(&mtx->sem) == -1) {
            ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                          "sem_destroy() failed");
        }
    }

#endif
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    प्रत्यागम (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid));
}


शून्य
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_uint_t         i, n;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx lock");

    प्रति ( ;; ) {

        यदि (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
            प्रत्यागम;
        }

        यदि (ngx_ncpu > 1) {

            प्रति (n = 1; n < mtx->spin; n <<= 1) {

                प्रति (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                यदि (*mtx->lock == 0
                    && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid))
                {
                    प्रत्यागम;
                }
            }
        }

#यदि (NGX_HAVE_POSIX_SEM)

        यदि (mtx->semaphore) {
            (शून्य) ngx_atomic_fetch_add(mtx->रुको, 1);

            यदि (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
                (शून्य) ngx_atomic_fetch_add(mtx->रुको, -1);
                प्रत्यागम;
            }

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx wait %uA", *mtx->रुको);

            यावत् (sem_wait(&mtx->sem) == -1) {
                ngx_err_t  err;

                err = ngx_errno;

                यदि (err != NGX_EINTR) {
                    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, err,
                                  "sem_wait() failed while waiting on shmtx");
                    भङ्ग;
                }
            }

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx awoke");

            अनुवर्त;
        }

#endif

        ngx_sched_yield();
    }
}


शून्य
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    यदि (mtx->spin != (ngx_uint_t) -1) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx unlock");
    }

    यदि (ngx_atomic_cmp_set(mtx->lock, ngx_pid, 0)) {
        ngx_shmtx_wakeup(mtx);
    }
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx forced unlock");

    यदि (ngx_atomic_cmp_set(mtx->lock, pid, 0)) {
        ngx_shmtx_wakeup(mtx);
        प्रत्यागम 1;
    }

    प्रत्यागम 0;
}


स्थिर शून्य
ngx_shmtx_wakeup(ngx_shmtx_t *mtx)
{
#यदि (NGX_HAVE_POSIX_SEM)
    ngx_atomic_uint_t  रुको;

    यदि (!mtx->semaphore) {
        प्रत्यागम;
    }

    प्रति ( ;; ) {

        रुको = *mtx->रुको;

        यदि ((ngx_atomic_int_t) रुको <= 0) {
            प्रत्यागम;
        }

        यदि (ngx_atomic_cmp_set(mtx->रुको, रुको, रुको - 1)) {
            भङ्ग;
        }
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx wake %uA", रुको);

    यदि (sem_post(&mtx->sem) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_post() failed while wake shmtx");
    }

#endif
}


#अन्यथा


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    यदि (mtx->name) {

        यदि (ngx_strcmp(name, mtx->name) == 0) {
            mtx->name = name;
            प्रत्यागम NGX_OK;
        }

        ngx_shmtx_destroy(mtx);
    }

    mtx->fd = ngx_open_file(name, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                            NGX_FILE_DEFAULT_ACCESS);

    यदि (mtx->fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, ngx_cycle->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", name);
        प्रत्यागम NGX_ERROR;
    }

    यदि (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }

    mtx->name = name;

    प्रत्यागम NGX_OK;
}


शून्य
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
    यदि (ngx_close_file(mtx->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", mtx->name);
    }
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_trylock_fd(mtx->fd);

    यदि (err == 0) {
        प्रत्यागम 1;
    }

    यदि (err == NGX_EAGAIN) {
        प्रत्यागम 0;
    }

#यदि __osf__ /* Tru64 UNIX */

    यदि (err == NGX_EACCES) {
        प्रत्यागम 0;
    }

#endif

    ngx_log_abort(err, ngx_trylock_fd_n " %s failed", mtx->name);

    प्रत्यागम 0;
}


शून्य
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_lock_fd(mtx->fd);

    यदि (err == 0) {
        प्रत्यागम;
    }

    ngx_log_abort(err, ngx_lock_fd_n " %s failed", mtx->name);
}


शून्य
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_unlock_fd(mtx->fd);

    यदि (err == 0) {
        प्रत्यागम;
    }

    ngx_log_abort(err, ngx_unlock_fd_n " %s failed", mtx->name);
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    प्रत्यागम 0;
}

#endif
