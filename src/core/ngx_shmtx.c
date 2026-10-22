
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


#ਜੇ (NGX_HAVE_ATOMIC_OPS)


ਸਥਿਰ ਖਾਲੀ ngx_shmtx_wakeup(ngx_shmtx_t *mtx);


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    mtx->ਤਾਲਾ = &addr->ਤਾਲਾ;

    ਜੇ (mtx->spin == (ngx_uint_t) -1) {
        ਵਾਪਸ NGX_OK;
    }

    mtx->spin = 2048;

#ਜੇ (NGX_HAVE_POSIX_SEM)

    mtx->wait = &addr->wait;

    ਜੇ (sem_init(&mtx->sem, 1, 0) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_errno,
                      "sem_init() failed");
    } ਨਹੀਂਤਾਂ {
        mtx->semaphore = 1;
    }

#ਜੇ_ਅੰਤ

    ਵਾਪਸ NGX_OK;
}


ਖਾਲੀ
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
#ਜੇ (NGX_HAVE_POSIX_SEM)

    ਜੇ (mtx->semaphore) {
        ਜੇ (sem_destroy(&mtx->sem) == -1) {
            ngx_log_error(NGX_LOG_ALERT, ngx_ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_errno,
                          "sem_destroy() failed");
        }
    }

#ਜੇ_ਅੰਤ
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ਵਾਪਸ (*mtx->ਤਾਲਾ == 0 && ngx_atomic_cmp_ਸੈੱਟ(mtx->ਤਾਲਾ, 0, ngx_pid));
}


ਖਾਲੀ
ngx_shmtx_ਤਾਲਾ(ngx_shmtx_t *mtx)
{
    ngx_uint_t         i, n;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_ਚੱਕਰ->ਲਘੂਗਣਕ, 0, "shmtx lock");

    ਲਈ ( ;; ) {

        ਜੇ (*mtx->ਤਾਲਾ == 0 && ngx_atomic_cmp_ਸੈੱਟ(mtx->ਤਾਲਾ, 0, ngx_pid)) {
            ਵਾਪਸ;
        }

        ਜੇ (ngx_ncpu > 1) {

            ਲਈ (n = 1; n < mtx->spin; n <<= 1) {

                ਲਈ (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                ਜੇ (*mtx->ਤਾਲਾ == 0
                    && ngx_atomic_cmp_ਸੈੱਟ(mtx->ਤਾਲਾ, 0, ngx_pid))
                {
                    ਵਾਪਸ;
                }
            }
        }

#ਜੇ (NGX_HAVE_POSIX_SEM)

        ਜੇ (mtx->semaphore) {
            (ਖਾਲੀ) ngx_atomic_fetch_ਜੋੜੋ(mtx->wait, 1);

            ਜੇ (*mtx->ਤਾਲਾ == 0 && ngx_atomic_cmp_ਸੈੱਟ(mtx->ਤਾਲਾ, 0, ngx_pid)) {
                (ਖਾਲੀ) ngx_atomic_fetch_ਜੋੜੋ(mtx->wait, -1);
                ਵਾਪਸ;
            }

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_ਚੱਕਰ->ਲਘੂਗਣਕ, 0,
                           "shmtx wait %uA", *mtx->wait);

            ਜਦੋਂ (sem_wait(&mtx->sem) == -1) {
                ngx_err_t  err;

                err = ngx_errno;

                ਜੇ (err != NGX_EINTR) {
                    ngx_log_error(NGX_LOG_ALERT, ngx_ਚੱਕਰ->ਲਘੂਗਣਕ, err,
                                  "sem_wait() failed while waiting on shmtx");
                    ਰੋਕੋ;
                }
            }

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_ਚੱਕਰ->ਲਘੂਗਣਕ, 0,
                           "shmtx awoke");

            ਜਾਰੀ;
        }

#ਜੇ_ਅੰਤ

        ngx_sched_yield();
    }
}


ਖਾਲੀ
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    ਜੇ (mtx->spin != (ngx_uint_t) -1) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_ਚੱਕਰ->ਲਘੂਗਣਕ, 0, "shmtx unlock");
    }

    ਜੇ (ngx_atomic_cmp_ਸੈੱਟ(mtx->ਤਾਲਾ, ngx_pid, 0)) {
        ngx_shmtx_wakeup(mtx);
    }
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_ਚੱਕਰ->ਲਘੂਗਣਕ, 0,
                   "shmtx forced unlock");

    ਜੇ (ngx_atomic_cmp_ਸੈੱਟ(mtx->ਤਾਲਾ, pid, 0)) {
        ngx_shmtx_wakeup(mtx);
        ਵਾਪਸ 1;
    }

    ਵਾਪਸ 0;
}


ਸਥਿਰ ਖਾਲੀ
ngx_shmtx_wakeup(ngx_shmtx_t *mtx)
{
#ਜੇ (NGX_HAVE_POSIX_SEM)
    ngx_atomic_uint_t  wait;

    ਜੇ (!mtx->semaphore) {
        ਵਾਪਸ;
    }

    ਲਈ ( ;; ) {

        wait = *mtx->wait;

        ਜੇ ((ngx_atomic_int_t) wait <= 0) {
            ਵਾਪਸ;
        }

        ਜੇ (ngx_atomic_cmp_ਸੈੱਟ(mtx->wait, wait, wait - 1)) {
            ਰੋਕੋ;
        }
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_ਚੱਕਰ->ਲਘੂਗਣਕ, 0,
                   "shmtx wake %uA", wait);

    ਜੇ (sem_post(&mtx->sem) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_errno,
                      "sem_post() failed while wake shmtx");
    }

#ਜੇ_ਅੰਤ
}


#ਨਹੀਂਤਾਂ


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    ਜੇ (mtx->name) {

        ਜੇ (ngx_strcmp(name, mtx->name) == 0) {
            mtx->name = name;
            ਵਾਪਸ NGX_OK;
        }

        ngx_shmtx_destroy(mtx);
    }

    mtx->fd = ngx_open_file(name, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                            NGX_FILE_DEFAULT_ACCESS);

    ਜੇ (mtx->fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, ngx_ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", name);
        ਵਾਪਸ NGX_ERROR;
    }

    ਜੇ (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }

    mtx->name = name;

    ਵਾਪਸ NGX_OK;
}


ਖਾਲੀ
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
    ਜੇ (ngx_close_file(mtx->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", mtx->name);
    }
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_trylock_fd(mtx->fd);

    ਜੇ (err == 0) {
        ਵਾਪਸ 1;
    }

    ਜੇ (err == NGX_EAGAIN) {
        ਵਾਪਸ 0;
    }

#ਜੇ __osf__ /* Tru64 UNIX */

    ਜੇ (err == NGX_EACCES) {
        ਵਾਪਸ 0;
    }

#ਜੇ_ਅੰਤ

    ngx_log_abort(err, ngx_trylock_fd_n " %s failed", mtx->name);

    ਵਾਪਸ 0;
}


ਖਾਲੀ
ngx_shmtx_ਤਾਲਾ(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_ਤਾਲਾ_fd(mtx->fd);

    ਜੇ (err == 0) {
        ਵਾਪਸ;
    }

    ngx_log_abort(err, ngx_ਤਾਲਾ_fd_n " %s failed", mtx->name);
}


ਖਾਲੀ
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_unlock_fd(mtx->fd);

    ਜੇ (err == 0) {
        ਵਾਪਸ;
    }

    ngx_log_abort(err, ngx_unlock_fd_n " %s failed", mtx->name);
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ਵਾਪਸ 0;
}

#ਜੇ_ਅੰਤ
