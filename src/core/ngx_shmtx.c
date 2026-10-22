
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


#གལ་སྲིད (NGX_HAVE_ATOMIC_OPS)


static void ngx_shmtx_wakeup(ngx_shmtx_t *mtx);


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    mtx->lock = &addr->lock;

    གལ་སྲིད (mtx->spin == (ngx_uint_t) -1) {
        ལོག NGX_OK;
    }

    mtx->spin = 2048;

#གལ་སྲིད (NGX_HAVE_POSIX_SEM)

    mtx->wait = &addr->wait;

    གལ་སྲིད (sem_init(&mtx->sem, 1, 0) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_init() failed");
    } དེ་མིན {
        mtx->semaphore = 1;
    }

#endif

    ལོག NGX_OK;
}


void
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
#གལ་སྲིད (NGX_HAVE_POSIX_SEM)

    གལ་སྲིད (mtx->semaphore) {
        གལ་སྲིད (sem_destroy(&mtx->sem) == -1) {
            ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                          "sem_destroy() failed");
        }
    }

#endif
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ལོག (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid));
}


void
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_uint_t         i, n;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx lock");

    དོན་ལུ ( ;; ) {

        གལ་སྲིད (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
            ལོག;
        }

        གལ་སྲིད (ngx_ncpu > 1) {

            དོན་ལུ (n = 1; n < mtx->spin; n <<= 1) {

                དོན་ལུ (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                གལ་སྲིད (*mtx->lock == 0
                    && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid))
                {
                    ལོག;
                }
            }
        }

#གལ་སྲིད (NGX_HAVE_POSIX_SEM)

        གལ་སྲིད (mtx->semaphore) {
            (void) ngx_atomic_fetch_add(mtx->wait, 1);

            གལ་སྲིད (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
                (void) ngx_atomic_fetch_add(mtx->wait, -1);
                ལོག;
            }

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx wait %uA", *mtx->wait);

            སྐབས (sem_wait(&mtx->sem) == -1) {
                ngx_err_t  err;

                err = ngx_errno;

                གལ་སྲིད (err != NGX_EINTR) {
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
    གལ་སྲིད (mtx->spin != (ngx_uint_t) -1) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx unlock");
    }

    གལ་སྲིད (ngx_atomic_cmp_set(mtx->lock, ngx_pid, 0)) {
        ngx_shmtx_wakeup(mtx);
    }
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx forced unlock");

    གལ་སྲིད (ngx_atomic_cmp_set(mtx->lock, pid, 0)) {
        ngx_shmtx_wakeup(mtx);
        ལོག 1;
    }

    ལོག 0;
}


static void
ngx_shmtx_wakeup(ngx_shmtx_t *mtx)
{
#གལ་སྲིད (NGX_HAVE_POSIX_SEM)
    ngx_atomic_uint_t  wait;

    གལ་སྲིད (!mtx->semaphore) {
        ལོག;
    }

    དོན་ལུ ( ;; ) {

        wait = *mtx->wait;

        གལ་སྲིད ((ngx_atomic_int_t) wait <= 0) {
            ལོག;
        }

        གལ་སྲིད (ngx_atomic_cmp_set(mtx->wait, wait, wait - 1)) {
            bha_nga;
        }
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx wake %uA", wait);

    གལ་སྲིད (sem_post(&mtx->sem) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_post() failed while wake shmtx");
    }

#endif
}


#དེ་མིན


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    གལ་སྲིད (mtx->name) {

        གལ་སྲིད (ngx_strcmp(name, mtx->name) == 0) {
            mtx->name = name;
            ལོག NGX_OK;
        }

        ngx_shmtx_destroy(mtx);
    }

    mtx->fd = ngx_open_file(name, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                            NGX_FILE_DEFAULT_ACCESS);

    གལ་སྲིད (mtx->fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, ngx_cycle->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", name);
        ལོག NGX_ERROR;
    }

    གལ་སྲིད (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }

    mtx->name = name;

    ལོག NGX_OK;
}


void
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
    གལ་སྲིད (ngx_close_file(mtx->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", mtx->name);
    }
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_trylock_fd(mtx->fd);

    གལ་སྲིད (err == 0) {
        ལོག 1;
    }

    གལ་སྲིད (err == NGX_EAGAIN) {
        ལོག 0;
    }

#གལ་སྲིད __osf__ /* Tru64 UNIX */

    གལ་སྲིད (err == NGX_EACCES) {
        ལོག 0;
    }

#endif

    ngx_log_abort(err, ngx_trylock_fd_n " %s failed", mtx->name);

    ལོག 0;
}


void
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_lock_fd(mtx->fd);

    གལ་སྲིད (err == 0) {
        ལོག;
    }

    ngx_log_abort(err, ngx_lock_fd_n " %s failed", mtx->name);
}


void
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_unlock_fd(mtx->fd);

    གལ་སྲིད (err == 0) {
        ལོག;
    }

    ngx_log_abort(err, ngx_unlock_fd_n " %s failed", mtx->name);
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ལོག 0;
}

#endif
