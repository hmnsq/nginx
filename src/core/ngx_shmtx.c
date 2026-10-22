
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


#ಆದರೆ (NGX_HAVE_ATOMIC_OPS)


ಸ್ಥಿರ ಶೂನ್ಯ ngx_shmtx_wakeup(ngx_shmtx_t *mtx);


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    mtx->ಬೀಗ = &addr->ಬೀಗ;

    ಆದರೆ (mtx->spin == (ngx_uint_t) -1) {
        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    mtx->spin = 2048;

#ಆದರೆ (NGX_HAVE_POSIX_SEM)

    mtx->wait = &addr->wait;

    ಆದರೆ (sem_init(&mtx->sem, 1, 0) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_ಚಕ್ರ->ಲಘುಗಣಕ, ngx_errno,
                      "sem_init() failed");
    } ಇಲ್ಲದಿದ್ದರೆ {
        mtx->semaphore = 1;
    }

#ಆದರೆ_ಅಂತ್ಯ

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ಶೂನ್ಯ
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
#ಆದರೆ (NGX_HAVE_POSIX_SEM)

    ಆದರೆ (mtx->semaphore) {
        ಆದರೆ (sem_destroy(&mtx->sem) == -1) {
            ngx_log_error(NGX_LOG_ALERT, ngx_ಚಕ್ರ->ಲಘುಗಣಕ, ngx_errno,
                          "sem_destroy() failed");
        }
    }

#ಆದರೆ_ಅಂತ್ಯ
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ಹಿಂತಿರುಗಿ (*mtx->ಬೀಗ == 0 && ngx_atomic_cmp_ಗಣ(mtx->ಬೀಗ, 0, ngx_pid));
}


ಶೂನ್ಯ
ngx_shmtx_ಬೀಗ(ngx_shmtx_t *mtx)
{
    ngx_uint_t         i, n;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_ಚಕ್ರ->ಲಘುಗಣಕ, 0, "shmtx lock");

    ಗಾಗಿ ( ;; ) {

        ಆದರೆ (*mtx->ಬೀಗ == 0 && ngx_atomic_cmp_ಗಣ(mtx->ಬೀಗ, 0, ngx_pid)) {
            ಹಿಂತಿರುಗಿ;
        }

        ಆದರೆ (ngx_ncpu > 1) {

            ಗಾಗಿ (n = 1; n < mtx->spin; n <<= 1) {

                ಗಾಗಿ (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                ಆದರೆ (*mtx->ಬೀಗ == 0
                    && ngx_atomic_cmp_ಗಣ(mtx->ಬೀಗ, 0, ngx_pid))
                {
                    ಹಿಂತಿರುಗಿ;
                }
            }
        }

#ಆದರೆ (NGX_HAVE_POSIX_SEM)

        ಆದರೆ (mtx->semaphore) {
            (ಶೂನ್ಯ) ngx_atomic_fetch_ಕೂಡಿಸು(mtx->wait, 1);

            ಆದರೆ (*mtx->ಬೀಗ == 0 && ngx_atomic_cmp_ಗಣ(mtx->ಬೀಗ, 0, ngx_pid)) {
                (ಶೂನ್ಯ) ngx_atomic_fetch_ಕೂಡಿಸು(mtx->wait, -1);
                ಹಿಂತಿರುಗಿ;
            }

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_ಚಕ್ರ->ಲಘುಗಣಕ, 0,
                           "shmtx wait %uA", *mtx->wait);

            ಆಗ (sem_wait(&mtx->sem) == -1) {
                ngx_err_t  err;

                err = ngx_errno;

                ಆದರೆ (err != NGX_EINTR) {
                    ngx_log_error(NGX_LOG_ALERT, ngx_ಚಕ್ರ->ಲಘುಗಣಕ, err,
                                  "sem_wait() failed while waiting on shmtx");
                    ನಿಲ್ಲಿಸು;
                }
            }

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_ಚಕ್ರ->ಲಘುಗಣಕ, 0,
                           "shmtx awoke");

            ಮುಂದುವರಿ;
        }

#ಆದರೆ_ಅಂತ್ಯ

        ngx_sched_yield();
    }
}


ಶೂನ್ಯ
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    ಆದರೆ (mtx->spin != (ngx_uint_t) -1) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_ಚಕ್ರ->ಲಘುಗಣಕ, 0, "shmtx unlock");
    }

    ಆದರೆ (ngx_atomic_cmp_ಗಣ(mtx->ಬೀಗ, ngx_pid, 0)) {
        ngx_shmtx_wakeup(mtx);
    }
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_ಚಕ್ರ->ಲಘುಗಣಕ, 0,
                   "shmtx forced unlock");

    ಆದರೆ (ngx_atomic_cmp_ಗಣ(mtx->ಬೀಗ, pid, 0)) {
        ngx_shmtx_wakeup(mtx);
        ಹಿಂತಿರುಗಿ 1;
    }

    ಹಿಂತಿರುಗಿ 0;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_shmtx_wakeup(ngx_shmtx_t *mtx)
{
#ಆದರೆ (NGX_HAVE_POSIX_SEM)
    ngx_atomic_uint_t  wait;

    ಆದರೆ (!mtx->semaphore) {
        ಹಿಂತಿರುಗಿ;
    }

    ಗಾಗಿ ( ;; ) {

        wait = *mtx->wait;

        ಆದರೆ ((ngx_atomic_int_t) wait <= 0) {
            ಹಿಂತಿರುಗಿ;
        }

        ಆದರೆ (ngx_atomic_cmp_ಗಣ(mtx->wait, wait, wait - 1)) {
            ನಿಲ್ಲಿಸು;
        }
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_ಚಕ್ರ->ಲಘುಗಣಕ, 0,
                   "shmtx wake %uA", wait);

    ಆದರೆ (sem_post(&mtx->sem) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_ಚಕ್ರ->ಲಘುಗಣಕ, ngx_errno,
                      "sem_post() failed while wake shmtx");
    }

#ಆದರೆ_ಅಂತ್ಯ
}


#ಇಲ್ಲದಿದ್ದರೆ


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    ಆದರೆ (mtx->name) {

        ಆದರೆ (ngx_strcmp(name, mtx->name) == 0) {
            mtx->name = name;
            ಹಿಂತಿರುಗಿ NGX_OK;
        }

        ngx_shmtx_destroy(mtx);
    }

    mtx->fd = ngx_open_file(name, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                            NGX_FILE_DEFAULT_ACCESS);

    ಆದರೆ (mtx->fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, ngx_ಚಕ್ರ->ಲಘುಗಣಕ, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", name);
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಆದರೆ (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_ಚಕ್ರ->ಲಘುಗಣಕ, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }

    mtx->name = name;

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ಶೂನ್ಯ
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
    ಆದರೆ (ngx_close_file(mtx->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_ಚಕ್ರ->ಲಘುಗಣಕ, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", mtx->name);
    }
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_trylock_fd(mtx->fd);

    ಆದರೆ (err == 0) {
        ಹಿಂತಿರುಗಿ 1;
    }

    ಆದರೆ (err == NGX_EAGAIN) {
        ಹಿಂತಿರುಗಿ 0;
    }

#ಆದರೆ __osf__ /* Tru64 UNIX */

    ಆದರೆ (err == NGX_EACCES) {
        ಹಿಂತಿರುಗಿ 0;
    }

#ಆದರೆ_ಅಂತ್ಯ

    ngx_log_abort(err, ngx_trylock_fd_n " %s failed", mtx->name);

    ಹಿಂತಿರುಗಿ 0;
}


ಶೂನ್ಯ
ngx_shmtx_ಬೀಗ(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_ಬೀಗ_fd(mtx->fd);

    ಆದರೆ (err == 0) {
        ಹಿಂತಿರುಗಿ;
    }

    ngx_log_abort(err, ngx_ಬೀಗ_fd_n " %s failed", mtx->name);
}


ಶೂನ್ಯ
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_unlock_fd(mtx->fd);

    ಆದರೆ (err == 0) {
        ಹಿಂತಿರುಗಿ;
    }

    ngx_log_abort(err, ngx_unlock_fd_n " %s failed", mtx->name);
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ಹಿಂತಿರುಗಿ 0;
}

#ಆದರೆ_ಅಂತ್ಯ
