
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
        फर्काऊ NGX_OK;
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

    फर्काऊ NGX_OK;
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
    फर्काऊ (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid));
}


शून्य
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_uint_t         i, n;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx lock");

    लागि ( ;; ) {

        यदि (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
            फर्काऊ;
        }

        यदि (ngx_ncpu > 1) {

            लागि (n = 1; n < mtx->spin; n <<= 1) {

                लागि (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                यदि (*mtx->lock == 0
                    && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid))
                {
                    फर्काऊ;
                }
            }
        }

#यदि (NGX_HAVE_POSIX_SEM)

        यदि (mtx->semaphore) {
            (शून्य) ngx_atomic_fetch_add(mtx->रुको, 1);

            यदि (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
                (शून्य) ngx_atomic_fetch_add(mtx->रुको, -1);
                फर्काऊ;
            }

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx wait %uA", *mtx->रुको);

            जबसम्म (sem_wait(&mtx->sem) == -1) {
                ngx_err_t  err;

                err = ngx_errno;

                यदि (err != NGX_EINTR) {
                    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, err,
                                  "sem_wait() failed while waiting on shmtx");
                    रोक;
                }
            }

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx awoke");

            जारी;
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
        फर्काऊ 1;
    }

    फर्काऊ 0;
}


स्थिर शून्य
ngx_shmtx_wakeup(ngx_shmtx_t *mtx)
{
#यदि (NGX_HAVE_POSIX_SEM)
    ngx_atomic_uint_t  रुको;

    यदि (!mtx->semaphore) {
        फर्काऊ;
    }

    लागि ( ;; ) {

        रुको = *mtx->रुको;

        यदि ((ngx_atomic_int_t) रुको <= 0) {
            फर्काऊ;
        }

        यदि (ngx_atomic_cmp_set(mtx->रुको, रुको, रुको - 1)) {
            रोक;
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
            फर्काऊ NGX_OK;
        }

        ngx_shmtx_destroy(mtx);
    }

    mtx->fd = ngx_open_file(name, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                            NGX_FILE_DEFAULT_ACCESS);

    यदि (mtx->fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, ngx_cycle->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", name);
        फर्काऊ NGX_ERROR;
    }

    यदि (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }

    mtx->name = name;

    फर्काऊ NGX_OK;
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
        फर्काऊ 1;
    }

    यदि (err == NGX_EAGAIN) {
        फर्काऊ 0;
    }

#यदि __osf__ /* Tru64 UNIX */

    यदि (err == NGX_EACCES) {
        फर्काऊ 0;
    }

#endif

    ngx_log_abort(err, ngx_trylock_fd_n " %s failed", mtx->name);

    फर्काऊ 0;
}


शून्य
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_lock_fd(mtx->fd);

    यदि (err == 0) {
        फर्काऊ;
    }

    ngx_log_abort(err, ngx_lock_fd_n " %s failed", mtx->name);
}


शून्य
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_unlock_fd(mtx->fd);

    यदि (err == 0) {
        फर्काऊ;
    }

    ngx_log_abort(err, ngx_unlock_fd_n " %s failed", mtx->name);
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    फर्काऊ 0;
}

#endif
