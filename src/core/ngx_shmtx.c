
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


#ꦪꦼꦤ꧀ (NGX_HAVE_ATOMIC_OPS)


static void ngx_shmtx_wakeup(ngx_shmtx_t *mtx);


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    mtx->lock = &addr->lock;

    ꦪꦼꦤ꧀ (mtx->spin == (ngx_uint_t) -1) {
        ꦮꦭꦶ NGX_OK;
    }

    mtx->spin = 2048;

#ꦪꦼꦤ꧀ (NGX_HAVE_POSIX_SEM)

    mtx->wait = &addr->wait;

    ꦪꦼꦤ꧀ (sem_init(&mtx->sem, 1, 0) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_init() failed");
    } ꦭꦶꦪ {
        mtx->semaphore = 1;
    }

#endif

    ꦮꦭꦶ NGX_OK;
}


void
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
#ꦪꦼꦤ꧀ (NGX_HAVE_POSIX_SEM)

    ꦪꦼꦤ꧀ (mtx->semaphore) {
        ꦪꦼꦤ꧀ (sem_destroy(&mtx->sem) == -1) {
            ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                          "sem_destroy() failed");
        }
    }

#endif
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ꦮꦭꦶ (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid));
}


void
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_uint_t         i, n;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx lock");

    ꦏꦁꦒꦺꦴ ( ;; ) {

        ꦪꦼꦤ꧀ (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
            ꦮꦭꦶ;
        }

        ꦪꦼꦤ꧀ (ngx_ncpu > 1) {

            ꦏꦁꦒꦺꦴ (n = 1; n < mtx->spin; n <<= 1) {

                ꦏꦁꦒꦺꦴ (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                ꦪꦼꦤ꧀ (*mtx->lock == 0
                    && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid))
                {
                    ꦮꦭꦶ;
                }
            }
        }

#ꦪꦼꦤ꧀ (NGX_HAVE_POSIX_SEM)

        ꦪꦼꦤ꧀ (mtx->semaphore) {
            (void) ngx_atomic_fetch_add(mtx->wait, 1);

            ꦪꦼꦤ꧀ (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
                (void) ngx_atomic_fetch_add(mtx->wait, -1);
                ꦮꦭꦶ;
            }

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx wait %uA", *mtx->wait);

            ꦱꦼꦢꦼꦭ (sem_wait(&mtx->sem) == -1) {
                ngx_err_t  err;

                err = ngx_errno;

                ꦪꦼꦤ꧀ (err != NGX_EINTR) {
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
    ꦪꦼꦤ꧀ (mtx->spin != (ngx_uint_t) -1) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx unlock");
    }

    ꦪꦼꦤ꧀ (ngx_atomic_cmp_set(mtx->lock, ngx_pid, 0)) {
        ngx_shmtx_wakeup(mtx);
    }
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx forced unlock");

    ꦪꦼꦤ꧀ (ngx_atomic_cmp_set(mtx->lock, pid, 0)) {
        ngx_shmtx_wakeup(mtx);
        ꦮꦭꦶ 1;
    }

    ꦮꦭꦶ 0;
}


static void
ngx_shmtx_wakeup(ngx_shmtx_t *mtx)
{
#ꦪꦼꦤ꧀ (NGX_HAVE_POSIX_SEM)
    ngx_atomic_uint_t  wait;

    ꦪꦼꦤ꧀ (!mtx->semaphore) {
        ꦮꦭꦶ;
    }

    ꦏꦁꦒꦺꦴ ( ;; ) {

        wait = *mtx->wait;

        ꦪꦼꦤ꧀ ((ngx_atomic_int_t) wait <= 0) {
            ꦮꦭꦶ;
        }

        ꦪꦼꦤ꧀ (ngx_atomic_cmp_set(mtx->wait, wait, wait - 1)) {
            bha_nga;
        }
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx wake %uA", wait);

    ꦪꦼꦤ꧀ (sem_post(&mtx->sem) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_post() failed while wake shmtx");
    }

#endif
}


#ꦭꦶꦪ


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    ꦪꦼꦤ꧀ (mtx->name) {

        ꦪꦼꦤ꧀ (ngx_strcmp(name, mtx->name) == 0) {
            mtx->name = name;
            ꦮꦭꦶ NGX_OK;
        }

        ngx_shmtx_destroy(mtx);
    }

    mtx->fd = ngx_open_file(name, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                            NGX_FILE_DEFAULT_ACCESS);

    ꦪꦼꦤ꧀ (mtx->fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, ngx_cycle->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", name);
        ꦮꦭꦶ NGX_ERROR;
    }

    ꦪꦼꦤ꧀ (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }

    mtx->name = name;

    ꦮꦭꦶ NGX_OK;
}


void
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
    ꦪꦼꦤ꧀ (ngx_close_file(mtx->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", mtx->name);
    }
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_trylock_fd(mtx->fd);

    ꦪꦼꦤ꧀ (err == 0) {
        ꦮꦭꦶ 1;
    }

    ꦪꦼꦤ꧀ (err == NGX_EAGAIN) {
        ꦮꦭꦶ 0;
    }

#ꦪꦼꦤ꧀ __osf__ /* Tru64 UNIX */

    ꦪꦼꦤ꧀ (err == NGX_EACCES) {
        ꦮꦭꦶ 0;
    }

#endif

    ngx_log_abort(err, ngx_trylock_fd_n " %s failed", mtx->name);

    ꦮꦭꦶ 0;
}


void
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_lock_fd(mtx->fd);

    ꦪꦼꦤ꧀ (err == 0) {
        ꦮꦭꦶ;
    }

    ngx_log_abort(err, ngx_lock_fd_n " %s failed", mtx->name);
}


void
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_unlock_fd(mtx->fd);

    ꦪꦼꦤ꧀ (err == 0) {
        ꦮꦭꦶ;
    }

    ngx_log_abort(err, ngx_unlock_fd_n " %s failed", mtx->name);
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ꦮꦭꦶ 0;
}

#endif
