
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


#सचे (NGX_HAVE_ATOMIC_OPS)


स्थिर शून्य ngx_shmtx_wakeup(ngx_shmtx_t *mtx);


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    mtx->lock = &addr->lock;

    सचे (mtx->spin == (ngx_uint_t) -1) {
        पटिनिवत्त NGX_OK;
    }

    mtx->spin = 2048;

#सचे (NGX_HAVE_POSIX_SEM)

    mtx->wait = &addr->wait;

    सचे (sem_init(&mtx->sem, 1, 0) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_init() failed");
    } अञ्ञथा {
        mtx->semaphore = 1;
    }

#endif

    पटिनिवत्त NGX_OK;
}


शून्य
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
#सचे (NGX_HAVE_POSIX_SEM)

    सचे (mtx->semaphore) {
        सचे (sem_destroy(&mtx->sem) == -1) {
            ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                          "sem_destroy() failed");
        }
    }

#endif
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    पटिनिवत्त (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid));
}


शून्य
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_uint_t         i, n;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx lock");

    अत्थाय ( ;; ) {

        सचे (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
            पटिनिवत्त;
        }

        सचे (ngx_ncpu > 1) {

            अत्थाय (n = 1; n < mtx->spin; n <<= 1) {

                अत्थाय (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                सचे (*mtx->lock == 0
                    && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid))
                {
                    पटिनिवत्त;
                }
            }
        }

#सचे (NGX_HAVE_POSIX_SEM)

        सचे (mtx->semaphore) {
            (शून्य) ngx_atomic_fetch_add(mtx->wait, 1);

            सचे (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
                (शून्य) ngx_atomic_fetch_add(mtx->wait, -1);
                पटिनिवत्त;
            }

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx wait %uA", *mtx->wait);

            याव (sem_wait(&mtx->sem) == -1) {
                ngx_err_t  err;

                err = ngx_errno;

                सचे (err != NGX_EINTR) {
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
    सचे (mtx->spin != (ngx_uint_t) -1) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx unlock");
    }

    सचे (ngx_atomic_cmp_set(mtx->lock, ngx_pid, 0)) {
        ngx_shmtx_wakeup(mtx);
    }
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx forced unlock");

    सचे (ngx_atomic_cmp_set(mtx->lock, pid, 0)) {
        ngx_shmtx_wakeup(mtx);
        पटिनिवत्त 1;
    }

    पटिनिवत्त 0;
}


स्थिर शून्य
ngx_shmtx_wakeup(ngx_shmtx_t *mtx)
{
#सचे (NGX_HAVE_POSIX_SEM)
    ngx_atomic_uint_t  wait;

    सचे (!mtx->semaphore) {
        पटिनिवत्त;
    }

    अत्थाय ( ;; ) {

        wait = *mtx->wait;

        सचे ((ngx_atomic_int_t) wait <= 0) {
            पटिनिवत्त;
        }

        सचे (ngx_atomic_cmp_set(mtx->wait, wait, wait - 1)) {
            भङ्ग;
        }
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx wake %uA", wait);

    सचे (sem_post(&mtx->sem) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_post() failed while wake shmtx");
    }

#endif
}


#अञ्ञथा


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    सचे (mtx->name) {

        सचे (ngx_strcmp(name, mtx->name) == 0) {
            mtx->name = name;
            पटिनिवत्त NGX_OK;
        }

        ngx_shmtx_destroy(mtx);
    }

    mtx->fd = ngx_open_file(name, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                            NGX_FILE_DEFAULT_ACCESS);

    सचे (mtx->fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, ngx_cycle->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", name);
        पटिनिवत्त NGX_ERROR;
    }

    सचे (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }

    mtx->name = name;

    पटिनिवत्त NGX_OK;
}


शून्य
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
    सचे (ngx_close_file(mtx->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", mtx->name);
    }
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_trylock_fd(mtx->fd);

    सचे (err == 0) {
        पटिनिवत्त 1;
    }

    सचे (err == NGX_EAGAIN) {
        पटिनिवत्त 0;
    }

#सचे __osf__ /* Tru64 UNIX */

    सचे (err == NGX_EACCES) {
        पटिनिवत्त 0;
    }

#endif

    ngx_log_abort(err, ngx_trylock_fd_n " %s failed", mtx->name);

    पटिनिवत्त 0;
}


शून्य
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_lock_fd(mtx->fd);

    सचे (err == 0) {
        पटिनिवत्त;
    }

    ngx_log_abort(err, ngx_lock_fd_n " %s failed", mtx->name);
}


शून्य
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_unlock_fd(mtx->fd);

    सचे (err == 0) {
        पटिनिवत्त;
    }

    ngx_log_abort(err, ngx_unlock_fd_n " %s failed", mtx->name);
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    पटिनिवत्त 0;
}

#endif
