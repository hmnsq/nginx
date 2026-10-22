
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


#ຖ້າ (NGX_HAVE_ATOMIC_OPS)


static void ngx_shmtx_wakeup(ngx_shmtx_t *mtx);


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    mtx->lock = &addr->lock;

    ຖ້າ (mtx->spin == (ngx_uint_t) -1) {
        ສົ່ງຄືນ NGX_OK;
    }

    mtx->spin = 2048;

#ຖ້າ (NGX_HAVE_POSIX_SEM)

    mtx->wait = &addr->wait;

    ຖ້າ (sem_init(&mtx->sem, 1, 0) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_init() failed");
    } ບໍ່ດັ່ງນັ້ນ {
        mtx->semaphore = 1;
    }

#endif

    ສົ່ງຄືນ NGX_OK;
}


void
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
#ຖ້າ (NGX_HAVE_POSIX_SEM)

    ຖ້າ (mtx->semaphore) {
        ຖ້າ (sem_destroy(&mtx->sem) == -1) {
            ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                          "sem_destroy() failed");
        }
    }

#endif
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ສົ່ງຄືນ (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid));
}


void
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_uint_t         i, n;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx lock");

    ສຳລັບ ( ;; ) {

        ຖ້າ (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
            ສົ່ງຄືນ;
        }

        ຖ້າ (ngx_ncpu > 1) {

            ສຳລັບ (n = 1; n < mtx->spin; n <<= 1) {

                ສຳລັບ (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                ຖ້າ (*mtx->lock == 0
                    && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid))
                {
                    ສົ່ງຄືນ;
                }
            }
        }

#ຖ້າ (NGX_HAVE_POSIX_SEM)

        ຖ້າ (mtx->semaphore) {
            (void) ngx_atomic_fetch_add(mtx->wait, 1);

            ຖ້າ (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
                (void) ngx_atomic_fetch_add(mtx->wait, -1);
                ສົ່ງຄືນ;
            }

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx wait %uA", *mtx->wait);

            ໃນຂະນະທີ່ (sem_wait(&mtx->sem) == -1) {
                ngx_err_t  err;

                err = ngx_errno;

                ຖ້າ (err != NGX_EINTR) {
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
    ຖ້າ (mtx->spin != (ngx_uint_t) -1) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx unlock");
    }

    ຖ້າ (ngx_atomic_cmp_set(mtx->lock, ngx_pid, 0)) {
        ngx_shmtx_wakeup(mtx);
    }
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx forced unlock");

    ຖ້າ (ngx_atomic_cmp_set(mtx->lock, pid, 0)) {
        ngx_shmtx_wakeup(mtx);
        ສົ່ງຄືນ 1;
    }

    ສົ່ງຄືນ 0;
}


static void
ngx_shmtx_wakeup(ngx_shmtx_t *mtx)
{
#ຖ້າ (NGX_HAVE_POSIX_SEM)
    ngx_atomic_uint_t  wait;

    ຖ້າ (!mtx->semaphore) {
        ສົ່ງຄືນ;
    }

    ສຳລັບ ( ;; ) {

        wait = *mtx->wait;

        ຖ້າ ((ngx_atomic_int_t) wait <= 0) {
            ສົ່ງຄືນ;
        }

        ຖ້າ (ngx_atomic_cmp_set(mtx->wait, wait, wait - 1)) {
            bha_nga;
        }
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx wake %uA", wait);

    ຖ້າ (sem_post(&mtx->sem) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_post() failed while wake shmtx");
    }

#endif
}


#ບໍ່ດັ່ງນັ້ນ


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    ຖ້າ (mtx->name) {

        ຖ້າ (ngx_strcmp(name, mtx->name) == 0) {
            mtx->name = name;
            ສົ່ງຄືນ NGX_OK;
        }

        ngx_shmtx_destroy(mtx);
    }

    mtx->fd = ngx_open_file(name, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                            NGX_FILE_DEFAULT_ACCESS);

    ຖ້າ (mtx->fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, ngx_cycle->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", name);
        ສົ່ງຄືນ NGX_ERROR;
    }

    ຖ້າ (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }

    mtx->name = name;

    ສົ່ງຄືນ NGX_OK;
}


void
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
    ຖ້າ (ngx_close_file(mtx->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", mtx->name);
    }
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_trylock_fd(mtx->fd);

    ຖ້າ (err == 0) {
        ສົ່ງຄືນ 1;
    }

    ຖ້າ (err == NGX_EAGAIN) {
        ສົ່ງຄືນ 0;
    }

#ຖ້າ __osf__ /* Tru64 UNIX */

    ຖ້າ (err == NGX_EACCES) {
        ສົ່ງຄືນ 0;
    }

#endif

    ngx_log_abort(err, ngx_trylock_fd_n " %s failed", mtx->name);

    ສົ່ງຄືນ 0;
}


void
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_lock_fd(mtx->fd);

    ຖ້າ (err == 0) {
        ສົ່ງຄືນ;
    }

    ngx_log_abort(err, ngx_lock_fd_n " %s failed", mtx->name);
}


void
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_unlock_fd(mtx->fd);

    ຖ້າ (err == 0) {
        ສົ່ງຄືນ;
    }

    ngx_log_abort(err, ngx_unlock_fd_n " %s failed", mtx->name);
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ສົ່ງຄືນ 0;
}

#endif
