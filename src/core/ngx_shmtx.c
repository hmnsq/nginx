
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


#ಯದಿ (NGX_HAVE_ATOMIC_OPS)


ಸ್ಥಿರ ಶೂನ್ಯ ngx_shmtx_wakeup(ngx_shmtx_t *mtx);


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    mtx->lock = &addr->lock;

    ಯದಿ (mtx->spin == (ngx_uint_t) -1) {
        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    mtx->spin = 2048;

#ಯದಿ (NGX_HAVE_POSIX_SEM)

    mtx->wait = &addr->wait;

    ಯದಿ (sem_init(&mtx->sem, 1, 0) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_init() failed");
    } ಅನ್ಯಥಾ {
        mtx->semaphore = 1;
    }

#endif

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ಶೂನ್ಯ
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
#ಯದಿ (NGX_HAVE_POSIX_SEM)

    ಯದಿ (mtx->semaphore) {
        ಯದಿ (sem_destroy(&mtx->sem) == -1) {
            ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                          "sem_destroy() failed");
        }
    }

#endif
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ಪ್ರತ್ಯಾಗಮ (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid));
}


ಶೂನ್ಯ
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_uint_t         i, n;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx lock");

    ಪ್ರತಿ ( ;; ) {

        ಯದಿ (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
            ಪ್ರತ್ಯಾಗಮ;
        }

        ಯದಿ (ngx_ncpu > 1) {

            ಪ್ರತಿ (n = 1; n < mtx->spin; n <<= 1) {

                ಪ್ರತಿ (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                ಯದಿ (*mtx->lock == 0
                    && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid))
                {
                    ಪ್ರತ್ಯಾಗಮ;
                }
            }
        }

#ಯದಿ (NGX_HAVE_POSIX_SEM)

        ಯದಿ (mtx->semaphore) {
            (ಶೂನ್ಯ) ngx_atomic_fetch_add(mtx->wait, 1);

            ಯದಿ (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
                (ಶೂನ್ಯ) ngx_atomic_fetch_add(mtx->wait, -1);
                ಪ್ರತ್ಯಾಗಮ;
            }

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx wait %uA", *mtx->wait);

            ಯಾವತ್ (sem_wait(&mtx->sem) == -1) {
                ngx_err_t  err;

                err = ngx_errno;

                ಯದಿ (err != NGX_EINTR) {
                    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, err,
                                  "sem_wait() failed while waiting on shmtx");
                    ಭಙ್ಗ;
                }
            }

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx awoke");

            ಅನುವರ್ತ;
        }

#endif

        ngx_sched_yield();
    }
}


ಶೂನ್ಯ
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    ಯದಿ (mtx->spin != (ngx_uint_t) -1) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx unlock");
    }

    ಯದಿ (ngx_atomic_cmp_set(mtx->lock, ngx_pid, 0)) {
        ngx_shmtx_wakeup(mtx);
    }
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx forced unlock");

    ಯದಿ (ngx_atomic_cmp_set(mtx->lock, pid, 0)) {
        ngx_shmtx_wakeup(mtx);
        ಪ್ರತ್ಯಾಗಮ 1;
    }

    ಪ್ರತ್ಯಾಗಮ 0;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_shmtx_wakeup(ngx_shmtx_t *mtx)
{
#ಯದಿ (NGX_HAVE_POSIX_SEM)
    ngx_atomic_uint_t  wait;

    ಯದಿ (!mtx->semaphore) {
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಪ್ರತಿ ( ;; ) {

        wait = *mtx->wait;

        ಯದಿ ((ngx_atomic_int_t) wait <= 0) {
            ಪ್ರತ್ಯಾಗಮ;
        }

        ಯದಿ (ngx_atomic_cmp_set(mtx->wait, wait, wait - 1)) {
            ಭಙ್ಗ;
        }
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx wake %uA", wait);

    ಯದಿ (sem_post(&mtx->sem) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_post() failed while wake shmtx");
    }

#endif
}


#ಅನ್ಯಥಾ


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    ಯದಿ (mtx->name) {

        ಯದಿ (ngx_strcmp(name, mtx->name) == 0) {
            mtx->name = name;
            ಪ್ರತ್ಯಾಗಮ NGX_OK;
        }

        ngx_shmtx_destroy(mtx);
    }

    mtx->fd = ngx_open_file(name, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                            NGX_FILE_DEFAULT_ACCESS);

    ಯದಿ (mtx->fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, ngx_cycle->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", name);
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಯದಿ (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }

    mtx->name = name;

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ಶೂನ್ಯ
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
    ಯದಿ (ngx_close_file(mtx->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", mtx->name);
    }
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_trylock_fd(mtx->fd);

    ಯದಿ (err == 0) {
        ಪ್ರತ್ಯಾಗಮ 1;
    }

    ಯದಿ (err == NGX_EAGAIN) {
        ಪ್ರತ್ಯಾಗಮ 0;
    }

#ಯದಿ __osf__ /* Tru64 UNIX */

    ಯದಿ (err == NGX_EACCES) {
        ಪ್ರತ್ಯಾಗಮ 0;
    }

#endif

    ngx_log_abort(err, ngx_trylock_fd_n " %s failed", mtx->name);

    ಪ್ರತ್ಯಾಗಮ 0;
}


ಶೂನ್ಯ
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_lock_fd(mtx->fd);

    ಯದಿ (err == 0) {
        ಪ್ರತ್ಯಾಗಮ;
    }

    ngx_log_abort(err, ngx_lock_fd_n " %s failed", mtx->name);
}


ಶೂನ್ಯ
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_unlock_fd(mtx->fd);

    ಯದಿ (err == 0) {
        ಪ್ರತ್ಯಾಗಮ;
    }

    ngx_log_abort(err, ngx_unlock_fd_n " %s failed", mtx->name);
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ಪ್ರತ್ಯಾಗಮ 0;
}

#endif
