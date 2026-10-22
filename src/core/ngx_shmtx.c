
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


#ଯଦି (NGX_HAVE_ATOMIC_OPS)


ସ୍ଥିର ଶୂନ୍ୟ ngx_shmtx_wakeup(ngx_shmtx_t *mtx);


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    mtx->lock = &addr->lock;

    ଯଦି (mtx->spin == (ngx_uint_t) -1) {
        ଫେରନ୍ତୁ NGX_OK;
    }

    mtx->spin = 2048;

#ଯଦି (NGX_HAVE_POSIX_SEM)

    mtx->wait = &addr->wait;

    ଯଦି (sem_init(&mtx->sem, 1, 0) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_init() failed");
    } ଅନ୍ୟଥା {
        mtx->semaphore = 1;
    }

#endif

    ଫେରନ୍ତୁ NGX_OK;
}


ଶୂନ୍ୟ
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
#ଯଦି (NGX_HAVE_POSIX_SEM)

    ଯଦି (mtx->semaphore) {
        ଯଦି (sem_destroy(&mtx->sem) == -1) {
            ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                          "sem_destroy() failed");
        }
    }

#endif
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ଫେରନ୍ତୁ (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid));
}


ଶୂନ୍ୟ
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_uint_t         i, n;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx lock");

    ପାଇଁ ( ;; ) {

        ଯଦି (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
            ଫେରନ୍ତୁ;
        }

        ଯଦି (ngx_ncpu > 1) {

            ପାଇଁ (n = 1; n < mtx->spin; n <<= 1) {

                ପାଇଁ (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                ଯଦି (*mtx->lock == 0
                    && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid))
                {
                    ଫେରନ୍ତୁ;
                }
            }
        }

#ଯଦି (NGX_HAVE_POSIX_SEM)

        ଯଦି (mtx->semaphore) {
            (ଶୂନ୍ୟ) ngx_atomic_fetch_add(mtx->wait, 1);

            ଯଦି (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
                (ଶୂନ୍ୟ) ngx_atomic_fetch_add(mtx->wait, -1);
                ଫେରନ୍ତୁ;
            }

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx wait %uA", *mtx->wait);

            ଯେତେବେଳେ (sem_wait(&mtx->sem) == -1) {
                ngx_err_t  err;

                err = ngx_errno;

                ଯଦି (err != NGX_EINTR) {
                    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, err,
                                  "sem_wait() failed while waiting on shmtx");
                    ଭାଙ୍ଗ;
                }
            }

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx awoke");

            ଜାରି;
        }

#endif

        ngx_sched_yield();
    }
}


ଶୂନ୍ୟ
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    ଯଦି (mtx->spin != (ngx_uint_t) -1) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx unlock");
    }

    ଯଦି (ngx_atomic_cmp_set(mtx->lock, ngx_pid, 0)) {
        ngx_shmtx_wakeup(mtx);
    }
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx forced unlock");

    ଯଦି (ngx_atomic_cmp_set(mtx->lock, pid, 0)) {
        ngx_shmtx_wakeup(mtx);
        ଫେରନ୍ତୁ 1;
    }

    ଫେରନ୍ତୁ 0;
}


ସ୍ଥିର ଶୂନ୍ୟ
ngx_shmtx_wakeup(ngx_shmtx_t *mtx)
{
#ଯଦି (NGX_HAVE_POSIX_SEM)
    ngx_atomic_uint_t  wait;

    ଯଦି (!mtx->semaphore) {
        ଫେରନ୍ତୁ;
    }

    ପାଇଁ ( ;; ) {

        wait = *mtx->wait;

        ଯଦି ((ngx_atomic_int_t) wait <= 0) {
            ଫେରନ୍ତୁ;
        }

        ଯଦି (ngx_atomic_cmp_set(mtx->wait, wait, wait - 1)) {
            ଭାଙ୍ଗ;
        }
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx wake %uA", wait);

    ଯଦି (sem_post(&mtx->sem) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_post() failed while wake shmtx");
    }

#endif
}


#ଅନ୍ୟଥା


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    ଯଦି (mtx->name) {

        ଯଦି (ngx_strcmp(name, mtx->name) == 0) {
            mtx->name = name;
            ଫେରନ୍ତୁ NGX_OK;
        }

        ngx_shmtx_destroy(mtx);
    }

    mtx->fd = ngx_open_file(name, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                            NGX_FILE_DEFAULT_ACCESS);

    ଯଦି (mtx->fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, ngx_cycle->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", name);
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ଯଦି (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }

    mtx->name = name;

    ଫେରନ୍ତୁ NGX_OK;
}


ଶୂନ୍ୟ
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
    ଯଦି (ngx_close_file(mtx->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", mtx->name);
    }
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_trylock_fd(mtx->fd);

    ଯଦି (err == 0) {
        ଫେରନ୍ତୁ 1;
    }

    ଯଦି (err == NGX_EAGAIN) {
        ଫେରନ୍ତୁ 0;
    }

#ଯଦି __osf__ /* Tru64 UNIX */

    ଯଦି (err == NGX_EACCES) {
        ଫେରନ୍ତୁ 0;
    }

#endif

    ngx_log_abort(err, ngx_trylock_fd_n " %s failed", mtx->name);

    ଫେରନ୍ତୁ 0;
}


ଶୂନ୍ୟ
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_lock_fd(mtx->fd);

    ଯଦି (err == 0) {
        ଫେରନ୍ତୁ;
    }

    ngx_log_abort(err, ngx_lock_fd_n " %s failed", mtx->name);
}


ଶୂନ୍ୟ
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_unlock_fd(mtx->fd);

    ଯଦି (err == 0) {
        ଫେରନ୍ତୁ;
    }

    ngx_log_abort(err, ngx_unlock_fd_n " %s failed", mtx->name);
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ଫେରନ୍ତୁ 0;
}

#endif
