
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>


#எனில் (NGX_HAVE_ATOMIC_OPS)


நிலையான வெறுமை ngx_shmtx_wakeup(ngx_shmtx_t *mtx);


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    mtx->lock = &addr->lock;

    எனில் (mtx->spin == (ngx_uint_t) -1) {
        திரும்பு NGX_OK;
    }

    mtx->spin = 2048;

#எனில் (NGX_HAVE_POSIX_SEM)

    mtx->wait = &addr->wait;

    எனில் (sem_init(&mtx->sem, 1, 0) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_init() failed");
    } இல்லையெனில் {
        mtx->semaphore = 1;
    }

#endif

    திரும்பு NGX_OK;
}


வெறுமை
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
#எனில் (NGX_HAVE_POSIX_SEM)

    எனில் (mtx->semaphore) {
        எனில் (sem_destroy(&mtx->sem) == -1) {
            ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                          "sem_destroy() failed");
        }
    }

#endif
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    திரும்பு (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid));
}


வெறுமை
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_uint_t         i, n;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx lock");

    க்கு ( ;; ) {

        எனில் (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
            திரும்பு;
        }

        எனில் (ngx_ncpu > 1) {

            க்கு (n = 1; n < mtx->spin; n <<= 1) {

                க்கு (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                எனில் (*mtx->lock == 0
                    && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid))
                {
                    திரும்பு;
                }
            }
        }

#எனில் (NGX_HAVE_POSIX_SEM)

        எனில் (mtx->semaphore) {
            (வெறுமை) ngx_atomic_fetch_add(mtx->wait, 1);

            எனில் (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
                (வெறுமை) ngx_atomic_fetch_add(mtx->wait, -1);
                திரும்பு;
            }

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx wait %uA", *mtx->wait);

            போது (sem_wait(&mtx->sem) == -1) {
                ngx_err_t  err;

                err = ngx_errno;

                எனில் (err != NGX_EINTR) {
                    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, err,
                                  "sem_wait() failed while waiting on shmtx");
                    நிறுத்து;
                }
            }

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx awoke");

            தொடர்;
        }

#endif

        ngx_sched_yield();
    }
}


வெறுமை
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    எனில் (mtx->spin != (ngx_uint_t) -1) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx unlock");
    }

    எனில் (ngx_atomic_cmp_set(mtx->lock, ngx_pid, 0)) {
        ngx_shmtx_wakeup(mtx);
    }
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx forced unlock");

    எனில் (ngx_atomic_cmp_set(mtx->lock, pid, 0)) {
        ngx_shmtx_wakeup(mtx);
        திரும்பு 1;
    }

    திரும்பு 0;
}


நிலையான வெறுமை
ngx_shmtx_wakeup(ngx_shmtx_t *mtx)
{
#எனில் (NGX_HAVE_POSIX_SEM)
    ngx_atomic_uint_t  wait;

    எனில் (!mtx->semaphore) {
        திரும்பு;
    }

    க்கு ( ;; ) {

        wait = *mtx->wait;

        எனில் ((ngx_atomic_int_t) wait <= 0) {
            திரும்பு;
        }

        எனில் (ngx_atomic_cmp_set(mtx->wait, wait, wait - 1)) {
            நிறுத்து;
        }
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx wake %uA", wait);

    எனில் (sem_post(&mtx->sem) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_post() failed while wake shmtx");
    }

#endif
}


#இல்லையெனில்


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    எனில் (mtx->name) {

        எனில் (ngx_strcmp(name, mtx->name) == 0) {
            mtx->name = name;
            திரும்பு NGX_OK;
        }

        ngx_shmtx_destroy(mtx);
    }

    mtx->fd = ngx_open_file(name, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                            NGX_FILE_DEFAULT_ACCESS);

    எனில் (mtx->fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, ngx_cycle->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", name);
        திரும்பு NGX_ERROR;
    }

    எனில் (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }

    mtx->name = name;

    திரும்பு NGX_OK;
}


வெறுமை
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
    எனில் (ngx_close_file(mtx->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", mtx->name);
    }
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_trylock_fd(mtx->fd);

    எனில் (err == 0) {
        திரும்பு 1;
    }

    எனில் (err == NGX_EAGAIN) {
        திரும்பு 0;
    }

#எனில் __osf__ /* Tru64 UNIX */

    எனில் (err == NGX_EACCES) {
        திரும்பு 0;
    }

#endif

    ngx_log_abort(err, ngx_trylock_fd_n " %s failed", mtx->name);

    திரும்பு 0;
}


வெறுமை
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_lock_fd(mtx->fd);

    எனில் (err == 0) {
        திரும்பு;
    }

    ngx_log_abort(err, ngx_lock_fd_n " %s failed", mtx->name);
}


வெறுமை
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_unlock_fd(mtx->fd);

    எனில் (err == 0) {
        திரும்பு;
    }

    ngx_log_abort(err, ngx_unlock_fd_n " %s failed", mtx->name);
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    திரும்பு 0;
}

#endif
