
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


#nếu (NGX_HAVE_ATOMIC_OPS)


static void ngx_shmtx_wakeup(ngx_shmtx_t *mtx);


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    mtx->lock = &addr->lock;

    nếu (mtx->spin == (ngx_uint_t) -1) {
        trả_về NGX_OK;
    }

    mtx->spin = 2048;

#nếu (NGX_HAVE_POSIX_SEM)

    mtx->wait = &addr->wait;

    nếu (sem_init(&mtx->sem, 1, 0) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_init() failed");
    } không_thì {
        mtx->semaphore = 1;
    }

#endif

    trả_về NGX_OK;
}


void
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
#nếu (NGX_HAVE_POSIX_SEM)

    nếu (mtx->semaphore) {
        nếu (sem_destroy(&mtx->sem) == -1) {
            ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                          "sem_destroy() failed");
        }
    }

#endif
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    trả_về (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid));
}


void
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_uint_t         i, n;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx lock");

    cho ( ;; ) {

        nếu (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
            trả_về;
        }

        nếu (ngx_ncpu > 1) {

            cho (n = 1; n < mtx->spin; n <<= 1) {

                cho (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                nếu (*mtx->lock == 0
                    && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid))
                {
                    trả_về;
                }
            }
        }

#nếu (NGX_HAVE_POSIX_SEM)

        nếu (mtx->semaphore) {
            (void) ngx_atomic_fetch_add(mtx->wait, 1);

            nếu (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
                (void) ngx_atomic_fetch_add(mtx->wait, -1);
                trả_về;
            }

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx wait %uA", *mtx->wait);

            trong_khi (sem_wait(&mtx->sem) == -1) {
                ngx_err_t  err;

                err = ngx_errno;

                nếu (err != NGX_EINTR) {
                    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, err,
                                  "sem_wait() failed while waiting on shmtx");
                    break;
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
    nếu (mtx->spin != (ngx_uint_t) -1) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx unlock");
    }

    nếu (ngx_atomic_cmp_set(mtx->lock, ngx_pid, 0)) {
        ngx_shmtx_wakeup(mtx);
    }
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx forced unlock");

    nếu (ngx_atomic_cmp_set(mtx->lock, pid, 0)) {
        ngx_shmtx_wakeup(mtx);
        trả_về 1;
    }

    trả_về 0;
}


static void
ngx_shmtx_wakeup(ngx_shmtx_t *mtx)
{
#nếu (NGX_HAVE_POSIX_SEM)
    ngx_atomic_uint_t  wait;

    nếu (!mtx->semaphore) {
        trả_về;
    }

    cho ( ;; ) {

        wait = *mtx->wait;

        nếu ((ngx_atomic_int_t) wait <= 0) {
            trả_về;
        }

        nếu (ngx_atomic_cmp_set(mtx->wait, wait, wait - 1)) {
            break;
        }
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx wake %uA", wait);

    nếu (sem_post(&mtx->sem) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_post() failed while wake shmtx");
    }

#endif
}


#không_thì


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    nếu (mtx->name) {

        nếu (ngx_strcmp(name, mtx->name) == 0) {
            mtx->name = name;
            trả_về NGX_OK;
        }

        ngx_shmtx_destroy(mtx);
    }

    mtx->fd = ngx_open_file(name, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                            NGX_FILE_DEFAULT_ACCESS);

    nếu (mtx->fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, ngx_cycle->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", name);
        trả_về NGX_ERROR;
    }

    nếu (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }

    mtx->name = name;

    trả_về NGX_OK;
}


void
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
    nếu (ngx_close_file(mtx->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", mtx->name);
    }
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_trylock_fd(mtx->fd);

    nếu (err == 0) {
        trả_về 1;
    }

    nếu (err == NGX_EAGAIN) {
        trả_về 0;
    }

#nếu __osf__ /* Tru64 UNIX */

    nếu (err == NGX_EACCES) {
        trả_về 0;
    }

#endif

    ngx_log_abort(err, ngx_trylock_fd_n " %s failed", mtx->name);

    trả_về 0;
}


void
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_lock_fd(mtx->fd);

    nếu (err == 0) {
        trả_về;
    }

    ngx_log_abort(err, ngx_lock_fd_n " %s failed", mtx->name);
}


void
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_unlock_fd(mtx->fd);

    nếu (err == 0) {
        trả_về;
    }

    ngx_log_abort(err, ngx_unlock_fd_n " %s failed", mtx->name);
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    trả_về 0;
}

#endif
