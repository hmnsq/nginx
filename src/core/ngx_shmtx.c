
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


#اگر (NGX_HAVE_ATOMIC_OPS)


ایستا تهی ngx_shmtx_wakeup(ngx_shmtx_t *mtx);


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    mtx->lock = &addr->lock;

    اگر (mtx->spin == (ngx_uint_t) -1) {
        بازگشت NGX_OK;
    }

    mtx->spin = 2048;

#اگر (NGX_HAVE_POSIX_SEM)

    mtx->wait = &addr->wait;

    اگر (sem_init(&mtx->sem, 1, 0) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_init() failed");
    } وگرنه {
        mtx->semaphore = 1;
    }

#endif

    بازگشت NGX_OK;
}


تهی
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
#اگر (NGX_HAVE_POSIX_SEM)

    اگر (mtx->semaphore) {
        اگر (sem_destroy(&mtx->sem) == -1) {
            ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                          "sem_destroy() failed");
        }
    }

#endif
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    بازگشت (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid));
}


تهی
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_uint_t         i, n;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx lock");

    برای ( ;; ) {

        اگر (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
            بازگشت;
        }

        اگر (ngx_ncpu > 1) {

            برای (n = 1; n < mtx->spin; n <<= 1) {

                برای (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                اگر (*mtx->lock == 0
                    && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid))
                {
                    بازگشت;
                }
            }
        }

#اگر (NGX_HAVE_POSIX_SEM)

        اگر (mtx->semaphore) {
            (تهی) ngx_atomic_fetch_add(mtx->wait, 1);

            اگر (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
                (تهی) ngx_atomic_fetch_add(mtx->wait, -1);
                بازگشت;
            }

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx wait %uA", *mtx->wait);

            وقتی (sem_wait(&mtx->sem) == -1) {
                ngx_err_t  err;

                err = ngx_errno;

                اگر (err != NGX_EINTR) {
                    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, err,
                                  "sem_wait() failed while waiting on shmtx");
                    بشکن;
                }
            }

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx awoke");

            ادامه;
        }

#endif

        ngx_sched_yield();
    }
}


تهی
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    اگر (mtx->spin != (ngx_uint_t) -1) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx unlock");
    }

    اگر (ngx_atomic_cmp_set(mtx->lock, ngx_pid, 0)) {
        ngx_shmtx_wakeup(mtx);
    }
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx forced unlock");

    اگر (ngx_atomic_cmp_set(mtx->lock, pid, 0)) {
        ngx_shmtx_wakeup(mtx);
        بازگشت 1;
    }

    بازگشت 0;
}


ایستا تهی
ngx_shmtx_wakeup(ngx_shmtx_t *mtx)
{
#اگر (NGX_HAVE_POSIX_SEM)
    ngx_atomic_uint_t  wait;

    اگر (!mtx->semaphore) {
        بازگشت;
    }

    برای ( ;; ) {

        wait = *mtx->wait;

        اگر ((ngx_atomic_int_t) wait <= 0) {
            بازگشت;
        }

        اگر (ngx_atomic_cmp_set(mtx->wait, wait, wait - 1)) {
            بشکن;
        }
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx wake %uA", wait);

    اگر (sem_post(&mtx->sem) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_post() failed while wake shmtx");
    }

#endif
}


#وگرنه


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    اگر (mtx->name) {

        اگر (ngx_strcmp(name, mtx->name) == 0) {
            mtx->name = name;
            بازگشت NGX_OK;
        }

        ngx_shmtx_destroy(mtx);
    }

    mtx->fd = ngx_open_file(name, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                            NGX_FILE_DEFAULT_ACCESS);

    اگر (mtx->fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, ngx_cycle->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", name);
        بازگشت NGX_ERROR;
    }

    اگر (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }

    mtx->name = name;

    بازگشت NGX_OK;
}


تهی
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
    اگر (ngx_close_file(mtx->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", mtx->name);
    }
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_trylock_fd(mtx->fd);

    اگر (err == 0) {
        بازگشت 1;
    }

    اگر (err == NGX_EAGAIN) {
        بازگشت 0;
    }

#اگر __osf__ /* Tru64 UNIX */

    اگر (err == NGX_EACCES) {
        بازگشت 0;
    }

#endif

    ngx_log_abort(err, ngx_trylock_fd_n " %s failed", mtx->name);

    بازگشت 0;
}


تهی
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_lock_fd(mtx->fd);

    اگر (err == 0) {
        بازگشت;
    }

    ngx_log_abort(err, ngx_lock_fd_n " %s failed", mtx->name);
}


تهی
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_unlock_fd(mtx->fd);

    اگر (err == 0) {
        بازگشت;
    }

    ngx_log_abort(err, ngx_unlock_fd_n " %s failed", mtx->name);
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    بازگشت 0;
}

#endif
