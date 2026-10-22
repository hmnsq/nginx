
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>


#എങ്കിൽ (NGX_HAVE_ATOMIC_OPS)


സ്ഥിതം ശൂന്യം ngx_shmtx_wakeup(ngx_shmtx_t *mtx);


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    mtx->lock = &addr->lock;

    എങ്കിൽ (mtx->spin == (ngx_uint_t) -1) {
        തിരികെ NGX_OK;
    }

    mtx->spin = 2048;

#എങ്കിൽ (NGX_HAVE_POSIX_SEM)

    mtx->wait = &addr->wait;

    എങ്കിൽ (sem_init(&mtx->sem, 1, 0) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_init() failed");
    } അല്ലെങ്കിൽ {
        mtx->semaphore = 1;
    }

#endif

    തിരികെ NGX_OK;
}


ശൂന്യം
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
#എങ്കിൽ (NGX_HAVE_POSIX_SEM)

    എങ്കിൽ (mtx->semaphore) {
        എങ്കിൽ (sem_destroy(&mtx->sem) == -1) {
            ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                          "sem_destroy() failed");
        }
    }

#endif
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    തിരികെ (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid));
}


ശൂന്യം
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_uint_t         i, n;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx lock");

    വേണ്ടി ( ;; ) {

        എങ്കിൽ (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
            തിരികെ;
        }

        എങ്കിൽ (ngx_ncpu > 1) {

            വേണ്ടി (n = 1; n < mtx->spin; n <<= 1) {

                വേണ്ടി (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                എങ്കിൽ (*mtx->lock == 0
                    && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid))
                {
                    തിരികെ;
                }
            }
        }

#എങ്കിൽ (NGX_HAVE_POSIX_SEM)

        എങ്കിൽ (mtx->semaphore) {
            (ശൂന്യം) ngx_atomic_fetch_add(mtx->wait, 1);

            എങ്കിൽ (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
                (ശൂന്യം) ngx_atomic_fetch_add(mtx->wait, -1);
                തിരികെ;
            }

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx wait %uA", *mtx->wait);

            ആകുമ്പോൾ (sem_wait(&mtx->sem) == -1) {
                ngx_err_t  err;

                err = ngx_errno;

                എങ്കിൽ (err != NGX_EINTR) {
                    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, err,
                                  "sem_wait() failed while waiting on shmtx");
                    നിർത്തുക;
                }
            }

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx awoke");

            തുടരുക;
        }

#endif

        ngx_sched_yield();
    }
}


ശൂന്യം
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    എങ്കിൽ (mtx->spin != (ngx_uint_t) -1) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx unlock");
    }

    എങ്കിൽ (ngx_atomic_cmp_set(mtx->lock, ngx_pid, 0)) {
        ngx_shmtx_wakeup(mtx);
    }
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx forced unlock");

    എങ്കിൽ (ngx_atomic_cmp_set(mtx->lock, pid, 0)) {
        ngx_shmtx_wakeup(mtx);
        തിരികെ 1;
    }

    തിരികെ 0;
}


സ്ഥിതം ശൂന്യം
ngx_shmtx_wakeup(ngx_shmtx_t *mtx)
{
#എങ്കിൽ (NGX_HAVE_POSIX_SEM)
    ngx_atomic_uint_t  wait;

    എങ്കിൽ (!mtx->semaphore) {
        തിരികെ;
    }

    വേണ്ടി ( ;; ) {

        wait = *mtx->wait;

        എങ്കിൽ ((ngx_atomic_int_t) wait <= 0) {
            തിരികെ;
        }

        എങ്കിൽ (ngx_atomic_cmp_set(mtx->wait, wait, wait - 1)) {
            നിർത്തുക;
        }
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx wake %uA", wait);

    എങ്കിൽ (sem_post(&mtx->sem) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_post() failed while wake shmtx");
    }

#endif
}


#അല്ലെങ്കിൽ


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    എങ്കിൽ (mtx->name) {

        എങ്കിൽ (ngx_strcmp(name, mtx->name) == 0) {
            mtx->name = name;
            തിരികെ NGX_OK;
        }

        ngx_shmtx_destroy(mtx);
    }

    mtx->fd = ngx_open_file(name, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                            NGX_FILE_DEFAULT_ACCESS);

    എങ്കിൽ (mtx->fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, ngx_cycle->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", name);
        തിരികെ NGX_ERROR;
    }

    എങ്കിൽ (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }

    mtx->name = name;

    തിരികെ NGX_OK;
}


ശൂന്യം
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
    എങ്കിൽ (ngx_close_file(mtx->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", mtx->name);
    }
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_trylock_fd(mtx->fd);

    എങ്കിൽ (err == 0) {
        തിരികെ 1;
    }

    എങ്കിൽ (err == NGX_EAGAIN) {
        തിരികെ 0;
    }

#എങ്കിൽ __osf__ /* Tru64 UNIX */

    എങ്കിൽ (err == NGX_EACCES) {
        തിരികെ 0;
    }

#endif

    ngx_log_abort(err, ngx_trylock_fd_n " %s failed", mtx->name);

    തിരികെ 0;
}


ശൂന്യം
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_lock_fd(mtx->fd);

    എങ്കിൽ (err == 0) {
        തിരികെ;
    }

    ngx_log_abort(err, ngx_lock_fd_n " %s failed", mtx->name);
}


ശൂന്യം
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_unlock_fd(mtx->fd);

    എങ്കിൽ (err == 0) {
        തിരികെ;
    }

    ngx_log_abort(err, ngx_unlock_fd_n " %s failed", mtx->name);
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    തിരികെ 0;
}

#endif
