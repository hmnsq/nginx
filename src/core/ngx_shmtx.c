
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


#ᬬᬾᬦ᭄ (NGX_HAVE_ATOMIC_OPS)


static void ngx_shmtx_wakeup(ngx_shmtx_t *mtx);


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    mtx->lock = &addr->lock;

    ᬬᬾᬦ᭄ (mtx->spin == (ngx_uint_t) -1) {
        ᬯᬮᬶ NGX_OK;
    }

    mtx->spin = 2048;

#ᬬᬾᬦ᭄ (NGX_HAVE_POSIX_SEM)

    mtx->wait = &addr->wait;

    ᬬᬾᬦ᭄ (sem_init(&mtx->sem, 1, 0) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_init() failed");
    } ᬮᬶᬬ {
        mtx->semaphore = 1;
    }

#endif

    ᬯᬮᬶ NGX_OK;
}


void
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
#ᬬᬾᬦ᭄ (NGX_HAVE_POSIX_SEM)

    ᬬᬾᬦ᭄ (mtx->semaphore) {
        ᬬᬾᬦ᭄ (sem_destroy(&mtx->sem) == -1) {
            ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                          "sem_destroy() failed");
        }
    }

#endif
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ᬯᬮᬶ (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid));
}


void
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_uint_t         i, n;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx lock");

    for ( ;; ) {

        ᬬᬾᬦ᭄ (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
            ᬯᬮᬶ;
        }

        ᬬᬾᬦ᭄ (ngx_ncpu > 1) {

            for (n = 1; n < mtx->spin; n <<= 1) {

                for (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                ᬬᬾᬦ᭄ (*mtx->lock == 0
                    && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid))
                {
                    ᬯᬮᬶ;
                }
            }
        }

#ᬬᬾᬦ᭄ (NGX_HAVE_POSIX_SEM)

        ᬬᬾᬦ᭄ (mtx->semaphore) {
            (void) ngx_atomic_fetch_add(mtx->wait, 1);

            ᬬᬾᬦ᭄ (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
                (void) ngx_atomic_fetch_add(mtx->wait, -1);
                ᬯᬮᬶ;
            }

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx wait %uA", *mtx->wait);

            ᬲᭂᬤᭂᬮ (sem_wait(&mtx->sem) == -1) {
                ngx_err_t  err;

                err = ngx_errno;

                ᬬᬾᬦ᭄ (err != NGX_EINTR) {
                    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, err,
                                  "sem_wait() failed while waiting on shmtx");
                    bha_nga;
                }
            }

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx awoke");

            continue;
        }

#endif

        ngx_sched_yield();
    }
}


void
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    ᬬᬾᬦ᭄ (mtx->spin != (ngx_uint_t) -1) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx unlock");
    }

    ᬬᬾᬦ᭄ (ngx_atomic_cmp_set(mtx->lock, ngx_pid, 0)) {
        ngx_shmtx_wakeup(mtx);
    }
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx forced unlock");

    ᬬᬾᬦ᭄ (ngx_atomic_cmp_set(mtx->lock, pid, 0)) {
        ngx_shmtx_wakeup(mtx);
        ᬯᬮᬶ 1;
    }

    ᬯᬮᬶ 0;
}


static void
ngx_shmtx_wakeup(ngx_shmtx_t *mtx)
{
#ᬬᬾᬦ᭄ (NGX_HAVE_POSIX_SEM)
    ngx_atomic_uint_t  wait;

    ᬬᬾᬦ᭄ (!mtx->semaphore) {
        ᬯᬮᬶ;
    }

    for ( ;; ) {

        wait = *mtx->wait;

        ᬬᬾᬦ᭄ ((ngx_atomic_int_t) wait <= 0) {
            ᬯᬮᬶ;
        }

        ᬬᬾᬦ᭄ (ngx_atomic_cmp_set(mtx->wait, wait, wait - 1)) {
            bha_nga;
        }
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx wake %uA", wait);

    ᬬᬾᬦ᭄ (sem_post(&mtx->sem) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_post() failed while wake shmtx");
    }

#endif
}


#ᬮᬶᬬ


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    ᬬᬾᬦ᭄ (mtx->name) {

        ᬬᬾᬦ᭄ (ngx_strcmp(name, mtx->name) == 0) {
            mtx->name = name;
            ᬯᬮᬶ NGX_OK;
        }

        ngx_shmtx_destroy(mtx);
    }

    mtx->fd = ngx_open_file(name, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                            NGX_FILE_DEFAULT_ACCESS);

    ᬬᬾᬦ᭄ (mtx->fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, ngx_cycle->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", name);
        ᬯᬮᬶ NGX_ERROR;
    }

    ᬬᬾᬦ᭄ (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }

    mtx->name = name;

    ᬯᬮᬶ NGX_OK;
}


void
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
    ᬬᬾᬦ᭄ (ngx_close_file(mtx->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", mtx->name);
    }
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_trylock_fd(mtx->fd);

    ᬬᬾᬦ᭄ (err == 0) {
        ᬯᬮᬶ 1;
    }

    ᬬᬾᬦ᭄ (err == NGX_EAGAIN) {
        ᬯᬮᬶ 0;
    }

#ᬬᬾᬦ᭄ __osf__ /* Tru64 UNIX */

    ᬬᬾᬦ᭄ (err == NGX_EACCES) {
        ᬯᬮᬶ 0;
    }

#endif

    ngx_log_abort(err, ngx_trylock_fd_n " %s failed", mtx->name);

    ᬯᬮᬶ 0;
}


void
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_lock_fd(mtx->fd);

    ᬬᬾᬦ᭄ (err == 0) {
        ᬯᬮᬶ;
    }

    ngx_log_abort(err, ngx_lock_fd_n " %s failed", mtx->name);
}


void
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_unlock_fd(mtx->fd);

    ᬬᬾᬦ᭄ (err == 0) {
        ᬯᬮᬶ;
    }

    ngx_log_abort(err, ngx_unlock_fd_n " %s failed", mtx->name);
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ᬯᬮᬶ 0;
}

#endif
