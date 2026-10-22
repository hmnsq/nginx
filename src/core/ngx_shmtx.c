
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


#ប្រសិនបើ (NGX_HAVE_ATOMIC_OPS)


static void ngx_shmtx_wakeup(ngx_shmtx_t *mtx);


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    mtx->lock = &addr->lock;

    ប្រសិនបើ (mtx->spin == (ngx_uint_t) -1) {
        ត្រឡប់ NGX_OK;
    }

    mtx->spin = 2048;

#ប្រសិនបើ (NGX_HAVE_POSIX_SEM)

    mtx->wait = &addr->wait;

    ប្រសិនបើ (sem_init(&mtx->sem, 1, 0) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_init() failed");
    } បើមិនដូច្នេះ {
        mtx->semaphore = 1;
    }

#endif

    ត្រឡប់ NGX_OK;
}


void
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
#ប្រសិនបើ (NGX_HAVE_POSIX_SEM)

    ប្រសិនបើ (mtx->semaphore) {
        ប្រសិនបើ (sem_destroy(&mtx->sem) == -1) {
            ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                          "sem_destroy() failed");
        }
    }

#endif
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ត្រឡប់ (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid));
}


void
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_uint_t         i, n;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx lock");

    សម្រាប់ ( ;; ) {

        ប្រសិនបើ (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
            ត្រឡប់;
        }

        ប្រសិនបើ (ngx_ncpu > 1) {

            សម្រាប់ (n = 1; n < mtx->spin; n <<= 1) {

                សម្រាប់ (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                ប្រសិនបើ (*mtx->lock == 0
                    && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid))
                {
                    ត្រឡប់;
                }
            }
        }

#ប្រសិនបើ (NGX_HAVE_POSIX_SEM)

        ប្រសិនបើ (mtx->semaphore) {
            (void) ngx_atomic_fetch_add(mtx->wait, 1);

            ប្រសិនបើ (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
                (void) ngx_atomic_fetch_add(mtx->wait, -1);
                ត្រឡប់;
            }

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx wait %uA", *mtx->wait);

            ខណៈពេល (sem_wait(&mtx->sem) == -1) {
                ngx_err_t  err;

                err = ngx_errno;

                ប្រសិនបើ (err != NGX_EINTR) {
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
    ប្រសិនបើ (mtx->spin != (ngx_uint_t) -1) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx unlock");
    }

    ប្រសិនបើ (ngx_atomic_cmp_set(mtx->lock, ngx_pid, 0)) {
        ngx_shmtx_wakeup(mtx);
    }
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx forced unlock");

    ប្រសិនបើ (ngx_atomic_cmp_set(mtx->lock, pid, 0)) {
        ngx_shmtx_wakeup(mtx);
        ត្រឡប់ 1;
    }

    ត្រឡប់ 0;
}


static void
ngx_shmtx_wakeup(ngx_shmtx_t *mtx)
{
#ប្រសិនបើ (NGX_HAVE_POSIX_SEM)
    ngx_atomic_uint_t  wait;

    ប្រសិនបើ (!mtx->semaphore) {
        ត្រឡប់;
    }

    សម្រាប់ ( ;; ) {

        wait = *mtx->wait;

        ប្រសិនបើ ((ngx_atomic_int_t) wait <= 0) {
            ត្រឡប់;
        }

        ប្រសិនបើ (ngx_atomic_cmp_set(mtx->wait, wait, wait - 1)) {
            bha_nga;
        }
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx wake %uA", wait);

    ប្រសិនបើ (sem_post(&mtx->sem) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_post() failed while wake shmtx");
    }

#endif
}


#បើមិនដូច្នេះ


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    ប្រសិនបើ (mtx->name) {

        ប្រសិនបើ (ngx_strcmp(name, mtx->name) == 0) {
            mtx->name = name;
            ត្រឡប់ NGX_OK;
        }

        ngx_shmtx_destroy(mtx);
    }

    mtx->fd = ngx_open_file(name, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                            NGX_FILE_DEFAULT_ACCESS);

    ប្រសិនបើ (mtx->fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, ngx_cycle->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", name);
        ត្រឡប់ NGX_ERROR;
    }

    ប្រសិនបើ (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }

    mtx->name = name;

    ត្រឡប់ NGX_OK;
}


void
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
    ប្រសិនបើ (ngx_close_file(mtx->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", mtx->name);
    }
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_trylock_fd(mtx->fd);

    ប្រសិនបើ (err == 0) {
        ត្រឡប់ 1;
    }

    ប្រសិនបើ (err == NGX_EAGAIN) {
        ត្រឡប់ 0;
    }

#ប្រសិនបើ __osf__ /* Tru64 UNIX */

    ប្រសិនបើ (err == NGX_EACCES) {
        ត្រឡប់ 0;
    }

#endif

    ngx_log_abort(err, ngx_trylock_fd_n " %s failed", mtx->name);

    ត្រឡប់ 0;
}


void
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_lock_fd(mtx->fd);

    ប្រសិនបើ (err == 0) {
        ត្រឡប់;
    }

    ngx_log_abort(err, ngx_lock_fd_n " %s failed", mtx->name);
}


void
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_unlock_fd(mtx->fd);

    ប្រសិនបើ (err == 0) {
        ត្រឡប់;
    }

    ngx_log_abort(err, ngx_unlock_fd_n " %s failed", mtx->name);
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ត្រឡប់ 0;
}

#endif
