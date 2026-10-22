
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#ܐܢ (NGX_HAVE_ATOMIC_OPS)


ساكن فراغ ngx_shmtx_wakeup(ngx_shmtx_t *mtx);


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    mtx->lock = &addr->lock;

    ܐܢ (mtx->spin == (ngx_uint_t) -1) {
        ܦܢܐ NGX_OK;
    }

    mtx->spin = 2048;

#ܐܢ (NGX_HAVE_POSIX_SEM)

    mtx->wait = &addr->wait;

    ܐܢ (sem_init(&mtx->sem, 1, 0) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_init() failed");
    } ܐܚܪܢܐ {
        mtx->semaphore = 1;
    }

#endif

    ܦܢܐ NGX_OK;
}


فراغ
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
#ܐܢ (NGX_HAVE_POSIX_SEM)

    ܐܢ (mtx->semaphore) {
        ܐܢ (sem_destroy(&mtx->sem) == -1) {
            ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                          "sem_destroy() failed");
        }
    }

#endif
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ܦܢܐ (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid));
}


فراغ
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_uint_t         i, n;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx lock");

    لكل ( ;; ) {

        ܐܢ (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
            ܦܢܐ;
        }

        ܐܢ (ngx_ncpu > 1) {

            لكل (n = 1; n < mtx->spin; n <<= 1) {

                لكل (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                ܐܢ (*mtx->lock == 0
                    && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid))
                {
                    ܦܢܐ;
                }
            }
        }

#ܐܢ (NGX_HAVE_POSIX_SEM)

        ܐܢ (mtx->semaphore) {
            (فراغ) ngx_atomic_fetch_add(mtx->wait, 1);

            ܐܢ (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
                (فراغ) ngx_atomic_fetch_add(mtx->wait, -1);
                ܦܢܐ;
            }

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx wait %uA", *mtx->wait);

            ܟܡܐ (sem_wait(&mtx->sem) == -1) {
                ngx_err_t  err;

                err = ngx_errno;

                ܐܢ (err != NGX_EINTR) {
                    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, err,
                                  "sem_wait() failed while waiting on shmtx");
                    اكسر;
                }
            }

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx awoke");

            استمر;
        }

#endif

        ngx_sched_yield();
    }
}


فراغ
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    ܐܢ (mtx->spin != (ngx_uint_t) -1) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx unlock");
    }

    ܐܢ (ngx_atomic_cmp_set(mtx->lock, ngx_pid, 0)) {
        ngx_shmtx_wakeup(mtx);
    }
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx forced unlock");

    ܐܢ (ngx_atomic_cmp_set(mtx->lock, pid, 0)) {
        ngx_shmtx_wakeup(mtx);
        ܦܢܐ 1;
    }

    ܦܢܐ 0;
}


ساكن فراغ
ngx_shmtx_wakeup(ngx_shmtx_t *mtx)
{
#ܐܢ (NGX_HAVE_POSIX_SEM)
    ngx_atomic_uint_t  wait;

    ܐܢ (!mtx->semaphore) {
        ܦܢܐ;
    }

    لكل ( ;; ) {

        wait = *mtx->wait;

        ܐܢ ((ngx_atomic_int_t) wait <= 0) {
            ܦܢܐ;
        }

        ܐܢ (ngx_atomic_cmp_set(mtx->wait, wait, wait - 1)) {
            اكسر;
        }
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx wake %uA", wait);

    ܐܢ (sem_post(&mtx->sem) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_post() failed while wake shmtx");
    }

#endif
}


#ܐܚܪܢܐ


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    ܐܢ (mtx->name) {

        ܐܢ (ngx_strcmp(name, mtx->name) == 0) {
            mtx->name = name;
            ܦܢܐ NGX_OK;
        }

        ngx_shmtx_destroy(mtx);
    }

    mtx->fd = ngx_open_file(name, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                            NGX_FILE_DEFAULT_ACCESS);

    ܐܢ (mtx->fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, ngx_cycle->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", name);
        ܦܢܐ NGX_ERROR;
    }

    ܐܢ (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }

    mtx->name = name;

    ܦܢܐ NGX_OK;
}


فراغ
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
    ܐܢ (ngx_close_file(mtx->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", mtx->name);
    }
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_trylock_fd(mtx->fd);

    ܐܢ (err == 0) {
        ܦܢܐ 1;
    }

    ܐܢ (err == NGX_EAGAIN) {
        ܦܢܐ 0;
    }

#ܐܢ __osf__ /* Tru64 UNIX */

    ܐܢ (err == NGX_EACCES) {
        ܦܢܐ 0;
    }

#endif

    ngx_log_abort(err, ngx_trylock_fd_n " %s failed", mtx->name);

    ܦܢܐ 0;
}


فراغ
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_lock_fd(mtx->fd);

    ܐܢ (err == 0) {
        ܦܢܐ;
    }

    ngx_log_abort(err, ngx_lock_fd_n " %s failed", mtx->name);
}


فراغ
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_unlock_fd(mtx->fd);

    ܐܢ (err == 0) {
        ܦܢܐ;
    }

    ngx_log_abort(err, ngx_unlock_fd_n " %s failed", mtx->name);
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ܦܢܐ 0;
}

#endif
