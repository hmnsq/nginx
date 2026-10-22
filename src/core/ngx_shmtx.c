
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


#만약 (NGX_HAVE_ATOMIC_OPS)


static void ngx_shmtx_wakeup(ngx_shmtx_t *mtx);


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    mtx->lock = &addr->lock;

    만약 (mtx->spin == (ngx_uint_t) -1) {
        반환 NGX_OK;
    }

    mtx->spin = 2048;

#만약 (NGX_HAVE_POSIX_SEM)

    mtx->wait = &addr->wait;

    만약 (sem_init(&mtx->sem, 1, 0) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_init() failed");
    } 아니면 {
        mtx->semaphore = 1;
    }

#endif

    반환 NGX_OK;
}


void
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
#만약 (NGX_HAVE_POSIX_SEM)

    만약 (mtx->semaphore) {
        만약 (sem_destroy(&mtx->sem) == -1) {
            ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                          "sem_destroy() failed");
        }
    }

#endif
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    반환 (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid));
}


void
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_uint_t         i, n;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx lock");

    위해 ( ;; ) {

        만약 (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
            반환;
        }

        만약 (ngx_ncpu > 1) {

            위해 (n = 1; n < mtx->spin; n <<= 1) {

                위해 (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                만약 (*mtx->lock == 0
                    && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid))
                {
                    반환;
                }
            }
        }

#만약 (NGX_HAVE_POSIX_SEM)

        만약 (mtx->semaphore) {
            (void) ngx_atomic_fetch_add(mtx->wait, 1);

            만약 (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
                (void) ngx_atomic_fetch_add(mtx->wait, -1);
                반환;
            }

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx wait %uA", *mtx->wait);

            동안 (sem_wait(&mtx->sem) == -1) {
                ngx_err_t  err;

                err = ngx_errno;

                만약 (err != NGX_EINTR) {
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
    만약 (mtx->spin != (ngx_uint_t) -1) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx unlock");
    }

    만약 (ngx_atomic_cmp_set(mtx->lock, ngx_pid, 0)) {
        ngx_shmtx_wakeup(mtx);
    }
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx forced unlock");

    만약 (ngx_atomic_cmp_set(mtx->lock, pid, 0)) {
        ngx_shmtx_wakeup(mtx);
        반환 1;
    }

    반환 0;
}


static void
ngx_shmtx_wakeup(ngx_shmtx_t *mtx)
{
#만약 (NGX_HAVE_POSIX_SEM)
    ngx_atomic_uint_t  wait;

    만약 (!mtx->semaphore) {
        반환;
    }

    위해 ( ;; ) {

        wait = *mtx->wait;

        만약 ((ngx_atomic_int_t) wait <= 0) {
            반환;
        }

        만약 (ngx_atomic_cmp_set(mtx->wait, wait, wait - 1)) {
            break;
        }
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx wake %uA", wait);

    만약 (sem_post(&mtx->sem) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_post() failed while wake shmtx");
    }

#endif
}


#아니면


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    만약 (mtx->name) {

        만약 (ngx_strcmp(name, mtx->name) == 0) {
            mtx->name = name;
            반환 NGX_OK;
        }

        ngx_shmtx_destroy(mtx);
    }

    mtx->fd = ngx_open_file(name, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                            NGX_FILE_DEFAULT_ACCESS);

    만약 (mtx->fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, ngx_cycle->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", name);
        반환 NGX_ERROR;
    }

    만약 (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }

    mtx->name = name;

    반환 NGX_OK;
}


void
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
    만약 (ngx_close_file(mtx->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", mtx->name);
    }
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_trylock_fd(mtx->fd);

    만약 (err == 0) {
        반환 1;
    }

    만약 (err == NGX_EAGAIN) {
        반환 0;
    }

#만약 __osf__ /* Tru64 UNIX */

    만약 (err == NGX_EACCES) {
        반환 0;
    }

#endif

    ngx_log_abort(err, ngx_trylock_fd_n " %s failed", mtx->name);

    반환 0;
}


void
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_lock_fd(mtx->fd);

    만약 (err == 0) {
        반환;
    }

    ngx_log_abort(err, ngx_lock_fd_n " %s failed", mtx->name);
}


void
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_unlock_fd(mtx->fd);

    만약 (err == 0) {
        반환;
    }

    ngx_log_abort(err, ngx_unlock_fd_n " %s failed", mtx->name);
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    반환 0;
}

#endif
