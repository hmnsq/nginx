
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


#ᰀᰧ (NGX_HAVE_ATOMIC_OPS)


static void ngx_shmtx_wakeup(ngx_shmtx_t *mtx);


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    mtx->lock = &addr->lock;

    ᰀᰧ (mtx->spin == (ngx_uint_t) -1) {
        ᰀᰧᰌ NGX_OK;
    }

    mtx->spin = 2048;

#ᰀᰧ (NGX_HAVE_POSIX_SEM)

    mtx->wait = &addr->wait;

    ᰀᰧ (sem_init(&mtx->sem, 1, 0) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_init() failed");
    } ᰀᰧᰀ {
        mtx->semaphore = 1;
    }

#endif

    ᰀᰧᰌ NGX_OK;
}


void
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
#ᰀᰧ (NGX_HAVE_POSIX_SEM)

    ᰀᰧ (mtx->semaphore) {
        ᰀᰧ (sem_destroy(&mtx->sem) == -1) {
            ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                          "sem_destroy() failed");
        }
    }

#endif
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ᰀᰧᰌ (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid));
}


void
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_uint_t         i, n;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx lock");

    for ( ;; ) {

        ᰀᰧ (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
            ᰀᰧᰌ;
        }

        ᰀᰧ (ngx_ncpu > 1) {

            for (n = 1; n < mtx->spin; n <<= 1) {

                for (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                ᰀᰧ (*mtx->lock == 0
                    && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid))
                {
                    ᰀᰧᰌ;
                }
            }
        }

#ᰀᰧ (NGX_HAVE_POSIX_SEM)

        ᰀᰧ (mtx->semaphore) {
            (void) ngx_atomic_fetch_add(mtx->wait, 1);

            ᰀᰧ (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
                (void) ngx_atomic_fetch_add(mtx->wait, -1);
                ᰀᰧᰌ;
            }

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx wait %uA", *mtx->wait);

            ᰀᰬ (sem_wait(&mtx->sem) == -1) {
                ngx_err_t  err;

                err = ngx_errno;

                ᰀᰧ (err != NGX_EINTR) {
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
    ᰀᰧ (mtx->spin != (ngx_uint_t) -1) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx unlock");
    }

    ᰀᰧ (ngx_atomic_cmp_set(mtx->lock, ngx_pid, 0)) {
        ngx_shmtx_wakeup(mtx);
    }
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx forced unlock");

    ᰀᰧ (ngx_atomic_cmp_set(mtx->lock, pid, 0)) {
        ngx_shmtx_wakeup(mtx);
        ᰀᰧᰌ 1;
    }

    ᰀᰧᰌ 0;
}


static void
ngx_shmtx_wakeup(ngx_shmtx_t *mtx)
{
#ᰀᰧ (NGX_HAVE_POSIX_SEM)
    ngx_atomic_uint_t  wait;

    ᰀᰧ (!mtx->semaphore) {
        ᰀᰧᰌ;
    }

    for ( ;; ) {

        wait = *mtx->wait;

        ᰀᰧ ((ngx_atomic_int_t) wait <= 0) {
            ᰀᰧᰌ;
        }

        ᰀᰧ (ngx_atomic_cmp_set(mtx->wait, wait, wait - 1)) {
            bha_nga;
        }
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx wake %uA", wait);

    ᰀᰧ (sem_post(&mtx->sem) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_post() failed while wake shmtx");
    }

#endif
}


#ᰀᰧᰀ


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    ᰀᰧ (mtx->name) {

        ᰀᰧ (ngx_strcmp(name, mtx->name) == 0) {
            mtx->name = name;
            ᰀᰧᰌ NGX_OK;
        }

        ngx_shmtx_destroy(mtx);
    }

    mtx->fd = ngx_open_file(name, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                            NGX_FILE_DEFAULT_ACCESS);

    ᰀᰧ (mtx->fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, ngx_cycle->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", name);
        ᰀᰧᰌ NGX_ERROR;
    }

    ᰀᰧ (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }

    mtx->name = name;

    ᰀᰧᰌ NGX_OK;
}


void
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
    ᰀᰧ (ngx_close_file(mtx->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", mtx->name);
    }
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_trylock_fd(mtx->fd);

    ᰀᰧ (err == 0) {
        ᰀᰧᰌ 1;
    }

    ᰀᰧ (err == NGX_EAGAIN) {
        ᰀᰧᰌ 0;
    }

#ᰀᰧ __osf__ /* Tru64 UNIX */

    ᰀᰧ (err == NGX_EACCES) {
        ᰀᰧᰌ 0;
    }

#endif

    ngx_log_abort(err, ngx_trylock_fd_n " %s failed", mtx->name);

    ᰀᰧᰌ 0;
}


void
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_lock_fd(mtx->fd);

    ᰀᰧ (err == 0) {
        ᰀᰧᰌ;
    }

    ngx_log_abort(err, ngx_lock_fd_n " %s failed", mtx->name);
}


void
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_unlock_fd(mtx->fd);

    ᰀᰧ (err == 0) {
        ᰀᰧᰌ;
    }

    ngx_log_abort(err, ngx_unlock_fd_n " %s failed", mtx->name);
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ᰀᰧᰌ 0;
}

#endif
