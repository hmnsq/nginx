
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাৱ <ngx_config.h>
#অন্তর্ভাৱ <ngx_core.h>


#যদি (NGX_HAVE_ATOMIC_OPS)


স্থির শূন্য ngx_shmtx_wakeup(ngx_shmtx_t *mtx);


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    mtx->lock = &addr->lock;

    যদি (mtx->spin == (ngx_uint_t) -1) {
        ঘূৰাই NGX_OK;
    }

    mtx->spin = 2048;

#যদি (NGX_HAVE_POSIX_SEM)

    mtx->wait = &addr->wait;

    যদি (sem_init(&mtx->sem, 1, 0) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_init() failed");
    } অন্যথা {
        mtx->semaphore = 1;
    }

#endif

    ঘূৰাই NGX_OK;
}


শূন্য
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
#যদি (NGX_HAVE_POSIX_SEM)

    যদি (mtx->semaphore) {
        যদি (sem_destroy(&mtx->sem) == -1) {
            ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                          "sem_destroy() failed");
        }
    }

#endif
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ঘূৰাই (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid));
}


শূন্য
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_uint_t         i, n;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx lock");

    বাবে ( ;; ) {

        যদি (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
            ঘূৰাই;
        }

        যদি (ngx_ncpu > 1) {

            বাবে (n = 1; n < mtx->spin; n <<= 1) {

                বাবে (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                যদি (*mtx->lock == 0
                    && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid))
                {
                    ঘূৰাই;
                }
            }
        }

#যদি (NGX_HAVE_POSIX_SEM)

        যদি (mtx->semaphore) {
            (শূন্য) ngx_atomic_fetch_add(mtx->wait, 1);

            যদি (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
                (শূন্য) ngx_atomic_fetch_add(mtx->wait, -1);
                ঘূৰাই;
            }

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx wait %uA", *mtx->wait);

            যেতিয়ালৈ (sem_wait(&mtx->sem) == -1) {
                ngx_err_t  err;

                err = ngx_errno;

                যদি (err != NGX_EINTR) {
                    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, err,
                                  "sem_wait() failed while waiting on shmtx");
                    ভঙ্গ;
                }
            }

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx awoke");

            অনুৱর্ত;
        }

#endif

        ngx_sched_yield();
    }
}


শূন্য
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    যদি (mtx->spin != (ngx_uint_t) -1) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx unlock");
    }

    যদি (ngx_atomic_cmp_set(mtx->lock, ngx_pid, 0)) {
        ngx_shmtx_wakeup(mtx);
    }
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx forced unlock");

    যদি (ngx_atomic_cmp_set(mtx->lock, pid, 0)) {
        ngx_shmtx_wakeup(mtx);
        ঘূৰাই 1;
    }

    ঘূৰাই 0;
}


স্থির শূন্য
ngx_shmtx_wakeup(ngx_shmtx_t *mtx)
{
#যদি (NGX_HAVE_POSIX_SEM)
    ngx_atomic_uint_t  wait;

    যদি (!mtx->semaphore) {
        ঘূৰাই;
    }

    বাবে ( ;; ) {

        wait = *mtx->wait;

        যদি ((ngx_atomic_int_t) wait <= 0) {
            ঘূৰাই;
        }

        যদি (ngx_atomic_cmp_set(mtx->wait, wait, wait - 1)) {
            ভঙ্গ;
        }
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx wake %uA", wait);

    যদি (sem_post(&mtx->sem) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_post() failed while wake shmtx");
    }

#endif
}


#অন্যথা


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    যদি (mtx->name) {

        যদি (ngx_strcmp(name, mtx->name) == 0) {
            mtx->name = name;
            ঘূৰাই NGX_OK;
        }

        ngx_shmtx_destroy(mtx);
    }

    mtx->fd = ngx_open_file(name, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                            NGX_FILE_DEFAULT_ACCESS);

    যদি (mtx->fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, ngx_cycle->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", name);
        ঘূৰাই NGX_ERROR;
    }

    যদি (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }

    mtx->name = name;

    ঘূৰাই NGX_OK;
}


শূন্য
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
    যদি (ngx_close_file(mtx->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", mtx->name);
    }
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_trylock_fd(mtx->fd);

    যদি (err == 0) {
        ঘূৰাই 1;
    }

    যদি (err == NGX_EAGAIN) {
        ঘূৰাই 0;
    }

#যদি __osf__ /* Tru64 UNIX */

    যদি (err == NGX_EACCES) {
        ঘূৰাই 0;
    }

#endif

    ngx_log_abort(err, ngx_trylock_fd_n " %s failed", mtx->name);

    ঘূৰাই 0;
}


শূন্য
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_lock_fd(mtx->fd);

    যদি (err == 0) {
        ঘূৰাই;
    }

    ngx_log_abort(err, ngx_lock_fd_n " %s failed", mtx->name);
}


শূন্য
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_unlock_fd(mtx->fd);

    যদি (err == 0) {
        ঘূৰাই;
    }

    ngx_log_abort(err, ngx_unlock_fd_n " %s failed", mtx->name);
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ঘূৰাই 0;
}

#endif
