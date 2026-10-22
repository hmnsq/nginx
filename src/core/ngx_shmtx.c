
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


#ᱡᱩᱫᱤ (NGX_HAVE_ATOMIC_OPS)


static void ngx_shmtx_wakeup(ngx_shmtx_t *mtx);


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    mtx->lock = &addr->lock;

    ᱡᱩᱫᱤ (mtx->spin == (ngx_uint_t) -1) {
        ᱨᱩᱣᱟᱹᱲ NGX_OK;
    }

    mtx->spin = 2048;

#ᱡᱩᱫᱤ (NGX_HAVE_POSIX_SEM)

    mtx->wait = &addr->wait;

    ᱡᱩᱫᱤ (sem_init(&mtx->sem, 1, 0) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_init() failed");
    } ᱵᱟᱝ {
        mtx->semaphore = 1;
    }

#endif

    ᱨᱩᱣᱟᱹᱲ NGX_OK;
}


void
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
#ᱡᱩᱫᱤ (NGX_HAVE_POSIX_SEM)

    ᱡᱩᱫᱤ (mtx->semaphore) {
        ᱡᱩᱫᱤ (sem_destroy(&mtx->sem) == -1) {
            ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                          "sem_destroy() failed");
        }
    }

#endif
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ᱨᱩᱣᱟᱹᱲ (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid));
}


void
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_uint_t         i, n;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx lock");

    for ( ;; ) {

        ᱡᱩᱫᱤ (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
            ᱨᱩᱣᱟᱹᱲ;
        }

        ᱡᱩᱫᱤ (ngx_ncpu > 1) {

            for (n = 1; n < mtx->spin; n <<= 1) {

                for (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                ᱡᱩᱫᱤ (*mtx->lock == 0
                    && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid))
                {
                    ᱨᱩᱣᱟᱹᱲ;
                }
            }
        }

#ᱡᱩᱫᱤ (NGX_HAVE_POSIX_SEM)

        ᱡᱩᱫᱤ (mtx->semaphore) {
            (void) ngx_atomic_fetch_add(mtx->wait, 1);

            ᱡᱩᱫᱤ (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
                (void) ngx_atomic_fetch_add(mtx->wait, -1);
                ᱨᱩᱣᱟᱹᱲ;
            }

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx wait %uA", *mtx->wait);

            ᱡᱚᱠᱷᱚᱱ (sem_wait(&mtx->sem) == -1) {
                ngx_err_t  err;

                err = ngx_errno;

                ᱡᱩᱫᱤ (err != NGX_EINTR) {
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
    ᱡᱩᱫᱤ (mtx->spin != (ngx_uint_t) -1) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx unlock");
    }

    ᱡᱩᱫᱤ (ngx_atomic_cmp_set(mtx->lock, ngx_pid, 0)) {
        ngx_shmtx_wakeup(mtx);
    }
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx forced unlock");

    ᱡᱩᱫᱤ (ngx_atomic_cmp_set(mtx->lock, pid, 0)) {
        ngx_shmtx_wakeup(mtx);
        ᱨᱩᱣᱟᱹᱲ 1;
    }

    ᱨᱩᱣᱟᱹᱲ 0;
}


static void
ngx_shmtx_wakeup(ngx_shmtx_t *mtx)
{
#ᱡᱩᱫᱤ (NGX_HAVE_POSIX_SEM)
    ngx_atomic_uint_t  wait;

    ᱡᱩᱫᱤ (!mtx->semaphore) {
        ᱨᱩᱣᱟᱹᱲ;
    }

    for ( ;; ) {

        wait = *mtx->wait;

        ᱡᱩᱫᱤ ((ngx_atomic_int_t) wait <= 0) {
            ᱨᱩᱣᱟᱹᱲ;
        }

        ᱡᱩᱫᱤ (ngx_atomic_cmp_set(mtx->wait, wait, wait - 1)) {
            bha_nga;
        }
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx wake %uA", wait);

    ᱡᱩᱫᱤ (sem_post(&mtx->sem) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_post() failed while wake shmtx");
    }

#endif
}


#ᱵᱟᱝ


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    ᱡᱩᱫᱤ (mtx->name) {

        ᱡᱩᱫᱤ (ngx_strcmp(name, mtx->name) == 0) {
            mtx->name = name;
            ᱨᱩᱣᱟᱹᱲ NGX_OK;
        }

        ngx_shmtx_destroy(mtx);
    }

    mtx->fd = ngx_open_file(name, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                            NGX_FILE_DEFAULT_ACCESS);

    ᱡᱩᱫᱤ (mtx->fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, ngx_cycle->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", name);
        ᱨᱩᱣᱟᱹᱲ NGX_ERROR;
    }

    ᱡᱩᱫᱤ (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }

    mtx->name = name;

    ᱨᱩᱣᱟᱹᱲ NGX_OK;
}


void
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
    ᱡᱩᱫᱤ (ngx_close_file(mtx->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", mtx->name);
    }
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_trylock_fd(mtx->fd);

    ᱡᱩᱫᱤ (err == 0) {
        ᱨᱩᱣᱟᱹᱲ 1;
    }

    ᱡᱩᱫᱤ (err == NGX_EAGAIN) {
        ᱨᱩᱣᱟᱹᱲ 0;
    }

#ᱡᱩᱫᱤ __osf__ /* Tru64 UNIX */

    ᱡᱩᱫᱤ (err == NGX_EACCES) {
        ᱨᱩᱣᱟᱹᱲ 0;
    }

#endif

    ngx_log_abort(err, ngx_trylock_fd_n " %s failed", mtx->name);

    ᱨᱩᱣᱟᱹᱲ 0;
}


void
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_lock_fd(mtx->fd);

    ᱡᱩᱫᱤ (err == 0) {
        ᱨᱩᱣᱟᱹᱲ;
    }

    ngx_log_abort(err, ngx_lock_fd_n " %s failed", mtx->name);
}


void
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_unlock_fd(mtx->fd);

    ᱡᱩᱫᱤ (err == 0) {
        ᱨᱩᱣᱟᱹᱲ;
    }

    ngx_log_abort(err, ngx_unlock_fd_n " %s failed", mtx->name);
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ᱨᱩᱣᱟᱹᱲ 0;
}

#endif
