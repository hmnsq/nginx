
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


#གལ་ཏེ (NGX_HAVE_ATOMIC_OPS)


static void ngx_shmtx_wakeup(ngx_shmtx_t *mtx);


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    mtx->lock = &addr->lock;

    གལ་ཏེ (mtx->spin == (ngx_uint_t) -1) {
        ཕྱིར NGX_OK;
    }

    mtx->spin = 2048;

#གལ་ཏེ (NGX_HAVE_POSIX_SEM)

    mtx->wait = &addr->wait;

    གལ་ཏེ (sem_init(&mtx->sem, 1, 0) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_init() failed");
    } དེ་མིན {
        mtx->semaphore = 1;
    }

#endif

    ཕྱིར NGX_OK;
}


void
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
#གལ་ཏེ (NGX_HAVE_POSIX_SEM)

    གལ་ཏེ (mtx->semaphore) {
        གལ་ཏེ (sem_destroy(&mtx->sem) == -1) {
            ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                          "sem_destroy() failed");
        }
    }

#endif
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ཕྱིར (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid));
}


void
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_uint_t         i, n;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx lock");

    ཆེད་དུ ( ;; ) {

        གལ་ཏེ (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
            ཕྱིར;
        }

        གལ་ཏེ (ngx_ncpu > 1) {

            ཆེད་དུ (n = 1; n < mtx->spin; n <<= 1) {

                ཆེད་དུ (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                གལ་ཏེ (*mtx->lock == 0
                    && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid))
                {
                    ཕྱིར;
                }
            }
        }

#གལ་ཏེ (NGX_HAVE_POSIX_SEM)

        གལ་ཏེ (mtx->semaphore) {
            (void) ngx_atomic_fetch_add(mtx->wait, 1);

            གལ་ཏེ (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
                (void) ngx_atomic_fetch_add(mtx->wait, -1);
                ཕྱིར;
            }

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx wait %uA", *mtx->wait);

            སྐབས (sem_wait(&mtx->sem) == -1) {
                ngx_err_t  err;

                err = ngx_errno;

                གལ་ཏེ (err != NGX_EINTR) {
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
    གལ་ཏེ (mtx->spin != (ngx_uint_t) -1) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx unlock");
    }

    གལ་ཏེ (ngx_atomic_cmp_set(mtx->lock, ngx_pid, 0)) {
        ngx_shmtx_wakeup(mtx);
    }
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx forced unlock");

    གལ་ཏེ (ngx_atomic_cmp_set(mtx->lock, pid, 0)) {
        ngx_shmtx_wakeup(mtx);
        ཕྱིར 1;
    }

    ཕྱིར 0;
}


static void
ngx_shmtx_wakeup(ngx_shmtx_t *mtx)
{
#གལ་ཏེ (NGX_HAVE_POSIX_SEM)
    ngx_atomic_uint_t  wait;

    གལ་ཏེ (!mtx->semaphore) {
        ཕྱིར;
    }

    ཆེད་དུ ( ;; ) {

        wait = *mtx->wait;

        གལ་ཏེ ((ngx_atomic_int_t) wait <= 0) {
            ཕྱིར;
        }

        གལ་ཏེ (ngx_atomic_cmp_set(mtx->wait, wait, wait - 1)) {
            bha_nga;
        }
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx wake %uA", wait);

    གལ་ཏེ (sem_post(&mtx->sem) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_post() failed while wake shmtx");
    }

#endif
}


#དེ་མིན


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    གལ་ཏེ (mtx->name) {

        གལ་ཏེ (ngx_strcmp(name, mtx->name) == 0) {
            mtx->name = name;
            ཕྱིར NGX_OK;
        }

        ngx_shmtx_destroy(mtx);
    }

    mtx->fd = ngx_open_file(name, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                            NGX_FILE_DEFAULT_ACCESS);

    གལ་ཏེ (mtx->fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, ngx_cycle->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", name);
        ཕྱིར NGX_ERROR;
    }

    གལ་ཏེ (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }

    mtx->name = name;

    ཕྱིར NGX_OK;
}


void
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
    གལ་ཏེ (ngx_close_file(mtx->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", mtx->name);
    }
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_trylock_fd(mtx->fd);

    གལ་ཏེ (err == 0) {
        ཕྱིར 1;
    }

    གལ་ཏེ (err == NGX_EAGAIN) {
        ཕྱིར 0;
    }

#གལ་ཏེ __osf__ /* Tru64 UNIX */

    གལ་ཏེ (err == NGX_EACCES) {
        ཕྱིར 0;
    }

#endif

    ngx_log_abort(err, ngx_trylock_fd_n " %s failed", mtx->name);

    ཕྱིར 0;
}


void
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_lock_fd(mtx->fd);

    གལ་ཏེ (err == 0) {
        ཕྱིར;
    }

    ngx_log_abort(err, ngx_lock_fd_n " %s failed", mtx->name);
}


void
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_unlock_fd(mtx->fd);

    གལ་ཏེ (err == 0) {
        ཕྱིར;
    }

    ngx_log_abort(err, ngx_unlock_fd_n " %s failed", mtx->name);
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ཕྱིར 0;
}

#endif
