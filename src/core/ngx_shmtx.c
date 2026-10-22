
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


#ᤀᤡ᤺ (NGX_HAVE_ATOMIC_OPS)


static void ngx_shmtx_wakeup(ngx_shmtx_t *mtx);


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    mtx->lock = &addr->lock;

    ᤀᤡ᤺ (mtx->spin == (ngx_uint_t) -1) {
        ᤀᤡᤴ NGX_OK;
    }

    mtx->spin = 2048;

#ᤀᤡ᤺ (NGX_HAVE_POSIX_SEM)

    mtx->wait = &addr->wait;

    ᤀᤡ᤺ (sem_init(&mtx->sem, 1, 0) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_init() failed");
    } ᤀᤡ᤺ᤆᤡ {
        mtx->semaphore = 1;
    }

#endif

    ᤀᤡᤴ NGX_OK;
}


void
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
#ᤀᤡ᤺ (NGX_HAVE_POSIX_SEM)

    ᤀᤡ᤺ (mtx->semaphore) {
        ᤀᤡ᤺ (sem_destroy(&mtx->sem) == -1) {
            ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                          "sem_destroy() failed");
        }
    }

#endif
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ᤀᤡᤴ (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid));
}


void
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_uint_t         i, n;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx lock");

    for ( ;; ) {

        ᤀᤡ᤺ (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
            ᤀᤡᤴ;
        }

        ᤀᤡ᤺ (ngx_ncpu > 1) {

            for (n = 1; n < mtx->spin; n <<= 1) {

                for (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                ᤀᤡ᤺ (*mtx->lock == 0
                    && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid))
                {
                    ᤀᤡᤴ;
                }
            }
        }

#ᤀᤡ᤺ (NGX_HAVE_POSIX_SEM)

        ᤀᤡ᤺ (mtx->semaphore) {
            (void) ngx_atomic_fetch_add(mtx->wait, 1);

            ᤀᤡ᤺ (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
                (void) ngx_atomic_fetch_add(mtx->wait, -1);
                ᤀᤡᤴ;
            }

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx wait %uA", *mtx->wait);

            ᤀᤠ᤺ (sem_wait(&mtx->sem) == -1) {
                ngx_err_t  err;

                err = ngx_errno;

                ᤀᤡ᤺ (err != NGX_EINTR) {
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
    ᤀᤡ᤺ (mtx->spin != (ngx_uint_t) -1) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx unlock");
    }

    ᤀᤡ᤺ (ngx_atomic_cmp_set(mtx->lock, ngx_pid, 0)) {
        ngx_shmtx_wakeup(mtx);
    }
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx forced unlock");

    ᤀᤡ᤺ (ngx_atomic_cmp_set(mtx->lock, pid, 0)) {
        ngx_shmtx_wakeup(mtx);
        ᤀᤡᤴ 1;
    }

    ᤀᤡᤴ 0;
}


static void
ngx_shmtx_wakeup(ngx_shmtx_t *mtx)
{
#ᤀᤡ᤺ (NGX_HAVE_POSIX_SEM)
    ngx_atomic_uint_t  wait;

    ᤀᤡ᤺ (!mtx->semaphore) {
        ᤀᤡᤴ;
    }

    for ( ;; ) {

        wait = *mtx->wait;

        ᤀᤡ᤺ ((ngx_atomic_int_t) wait <= 0) {
            ᤀᤡᤴ;
        }

        ᤀᤡ᤺ (ngx_atomic_cmp_set(mtx->wait, wait, wait - 1)) {
            bha_nga;
        }
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx wake %uA", wait);

    ᤀᤡ᤺ (sem_post(&mtx->sem) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_post() failed while wake shmtx");
    }

#endif
}


#ᤀᤡ᤺ᤆᤡ


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    ᤀᤡ᤺ (mtx->name) {

        ᤀᤡ᤺ (ngx_strcmp(name, mtx->name) == 0) {
            mtx->name = name;
            ᤀᤡᤴ NGX_OK;
        }

        ngx_shmtx_destroy(mtx);
    }

    mtx->fd = ngx_open_file(name, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                            NGX_FILE_DEFAULT_ACCESS);

    ᤀᤡ᤺ (mtx->fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, ngx_cycle->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", name);
        ᤀᤡᤴ NGX_ERROR;
    }

    ᤀᤡ᤺ (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }

    mtx->name = name;

    ᤀᤡᤴ NGX_OK;
}


void
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
    ᤀᤡ᤺ (ngx_close_file(mtx->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", mtx->name);
    }
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_trylock_fd(mtx->fd);

    ᤀᤡ᤺ (err == 0) {
        ᤀᤡᤴ 1;
    }

    ᤀᤡ᤺ (err == NGX_EAGAIN) {
        ᤀᤡᤴ 0;
    }

#ᤀᤡ᤺ __osf__ /* Tru64 UNIX */

    ᤀᤡ᤺ (err == NGX_EACCES) {
        ᤀᤡᤴ 0;
    }

#endif

    ngx_log_abort(err, ngx_trylock_fd_n " %s failed", mtx->name);

    ᤀᤡᤴ 0;
}


void
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_lock_fd(mtx->fd);

    ᤀᤡ᤺ (err == 0) {
        ᤀᤡᤴ;
    }

    ngx_log_abort(err, ngx_lock_fd_n " %s failed", mtx->name);
}


void
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_unlock_fd(mtx->fd);

    ᤀᤡ᤺ (err == 0) {
        ᤀᤡᤴ;
    }

    ngx_log_abort(err, ngx_unlock_fd_n " %s failed", mtx->name);
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ᤀᤡᤴ 0;
}

#endif
