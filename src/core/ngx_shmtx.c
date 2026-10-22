
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


#အကယ်၍ (NGX_HAVE_ATOMIC_OPS)


static void ngx_shmtx_wakeup(ngx_shmtx_t *mtx);


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    mtx->lock = &addr->lock;

    အကယ်၍ (mtx->spin == (ngx_uint_t) -1) {
        ပြန်ပေး NGX_OK;
    }

    mtx->spin = 2048;

#အကယ်၍ (NGX_HAVE_POSIX_SEM)

    mtx->wait = &addr->wait;

    အကယ်၍ (sem_init(&mtx->sem, 1, 0) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_init() failed");
    } မဟုတ်ပါက {
        mtx->semaphore = 1;
    }

#endif

    ပြန်ပေး NGX_OK;
}


void
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
#အကယ်၍ (NGX_HAVE_POSIX_SEM)

    အကယ်၍ (mtx->semaphore) {
        အကယ်၍ (sem_destroy(&mtx->sem) == -1) {
            ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                          "sem_destroy() failed");
        }
    }

#endif
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ပြန်ပေး (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid));
}


void
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_uint_t         i, n;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx lock");

    အတွက် ( ;; ) {

        အကယ်၍ (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
            ပြန်ပေး;
        }

        အကယ်၍ (ngx_ncpu > 1) {

            အတွက် (n = 1; n < mtx->spin; n <<= 1) {

                အတွက် (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                အကယ်၍ (*mtx->lock == 0
                    && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid))
                {
                    ပြန်ပေး;
                }
            }
        }

#အကယ်၍ (NGX_HAVE_POSIX_SEM)

        အကယ်၍ (mtx->semaphore) {
            (void) ngx_atomic_fetch_add(mtx->wait, 1);

            အကယ်၍ (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
                (void) ngx_atomic_fetch_add(mtx->wait, -1);
                ပြန်ပေး;
            }

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx wait %uA", *mtx->wait);

            နေစဉ် (sem_wait(&mtx->sem) == -1) {
                ngx_err_t  err;

                err = ngx_errno;

                အကယ်၍ (err != NGX_EINTR) {
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
    အကယ်၍ (mtx->spin != (ngx_uint_t) -1) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx unlock");
    }

    အကယ်၍ (ngx_atomic_cmp_set(mtx->lock, ngx_pid, 0)) {
        ngx_shmtx_wakeup(mtx);
    }
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx forced unlock");

    အကယ်၍ (ngx_atomic_cmp_set(mtx->lock, pid, 0)) {
        ngx_shmtx_wakeup(mtx);
        ပြန်ပေး 1;
    }

    ပြန်ပေး 0;
}


static void
ngx_shmtx_wakeup(ngx_shmtx_t *mtx)
{
#အကယ်၍ (NGX_HAVE_POSIX_SEM)
    ngx_atomic_uint_t  wait;

    အကယ်၍ (!mtx->semaphore) {
        ပြန်ပေး;
    }

    အတွက် ( ;; ) {

        wait = *mtx->wait;

        အကယ်၍ ((ngx_atomic_int_t) wait <= 0) {
            ပြန်ပေး;
        }

        အကယ်၍ (ngx_atomic_cmp_set(mtx->wait, wait, wait - 1)) {
            bha_nga;
        }
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx wake %uA", wait);

    အကယ်၍ (sem_post(&mtx->sem) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_post() failed while wake shmtx");
    }

#endif
}


#မဟုတ်ပါက


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    အကယ်၍ (mtx->name) {

        အကယ်၍ (ngx_strcmp(name, mtx->name) == 0) {
            mtx->name = name;
            ပြန်ပေး NGX_OK;
        }

        ngx_shmtx_destroy(mtx);
    }

    mtx->fd = ngx_open_file(name, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                            NGX_FILE_DEFAULT_ACCESS);

    အကယ်၍ (mtx->fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, ngx_cycle->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", name);
        ပြန်ပေး NGX_ERROR;
    }

    အကယ်၍ (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }

    mtx->name = name;

    ပြန်ပေး NGX_OK;
}


void
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
    အကယ်၍ (ngx_close_file(mtx->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", mtx->name);
    }
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_trylock_fd(mtx->fd);

    အကယ်၍ (err == 0) {
        ပြန်ပေး 1;
    }

    အကယ်၍ (err == NGX_EAGAIN) {
        ပြန်ပေး 0;
    }

#အကယ်၍ __osf__ /* Tru64 UNIX */

    အကယ်၍ (err == NGX_EACCES) {
        ပြန်ပေး 0;
    }

#endif

    ngx_log_abort(err, ngx_trylock_fd_n " %s failed", mtx->name);

    ပြန်ပေး 0;
}


void
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_lock_fd(mtx->fd);

    အကယ်၍ (err == 0) {
        ပြန်ပေး;
    }

    ngx_log_abort(err, ngx_lock_fd_n " %s failed", mtx->name);
}


void
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_unlock_fd(mtx->fd);

    အကယ်၍ (err == 0) {
        ပြန်ပေး;
    }

    ngx_log_abort(err, ngx_unlock_fd_n " %s failed", mtx->name);
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ပြန်ပေး 0;
}

#endif
