
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


#अगर (NGX_HAVE_ATOMIC_OPS)


अटल व्योम ngx_shmtx_wakeup(ngx_shmtx_t *mtx);


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    mtx->lock = &addr->lock;

    अगर (mtx->spin == (ngx_uint_t) -1) {
        वापस NGX_OK;
    }

    mtx->spin = 2048;

#अगर (NGX_HAVE_POSIX_SEM)

    mtx->रुको = &addr->रुको;

    अगर (sem_init(&mtx->sem, 1, 0) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_init() failed");
    } अन्यथा {
        mtx->semaphore = 1;
    }

#पूर्ण_अगर

    वापस NGX_OK;
}


व्योम
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
#अगर (NGX_HAVE_POSIX_SEM)

    अगर (mtx->semaphore) {
        अगर (sem_destroy(&mtx->sem) == -1) {
            ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                          "sem_destroy() failed");
        }
    }

#पूर्ण_अगर
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    वापस (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid));
}


व्योम
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_uint_t         i, n;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx lock");

    क्रम ( ;; ) {

        अगर (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
            वापस;
        }

        अगर (ngx_ncpu > 1) {

            क्रम (n = 1; n < mtx->spin; n <<= 1) {

                क्रम (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                अगर (*mtx->lock == 0
                    && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid))
                {
                    वापस;
                }
            }
        }

#अगर (NGX_HAVE_POSIX_SEM)

        अगर (mtx->semaphore) {
            (व्योम) ngx_atomic_fetch_add(mtx->रुको, 1);

            अगर (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
                (व्योम) ngx_atomic_fetch_add(mtx->रुको, -1);
                वापस;
            }

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx wait %uA", *mtx->रुको);

            जबतक (sem_wait(&mtx->sem) == -1) {
                ngx_err_t  err;

                err = ngx_errno;

                अगर (err != NGX_EINTR) {
                    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, err,
                                  "sem_wait() failed while waiting on shmtx");
                    अवरोध;
                }
            }

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx awoke");

            जारी;
        }

#पूर्ण_अगर

        ngx_sched_yield();
    }
}


व्योम
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    अगर (mtx->spin != (ngx_uint_t) -1) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx unlock");
    }

    अगर (ngx_atomic_cmp_set(mtx->lock, ngx_pid, 0)) {
        ngx_shmtx_wakeup(mtx);
    }
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx forced unlock");

    अगर (ngx_atomic_cmp_set(mtx->lock, pid, 0)) {
        ngx_shmtx_wakeup(mtx);
        वापस 1;
    }

    वापस 0;
}


अटल व्योम
ngx_shmtx_wakeup(ngx_shmtx_t *mtx)
{
#अगर (NGX_HAVE_POSIX_SEM)
    ngx_atomic_uint_t  रुको;

    अगर (!mtx->semaphore) {
        वापस;
    }

    क्रम ( ;; ) {

        रुको = *mtx->रुको;

        अगर ((ngx_atomic_int_t) रुको <= 0) {
            वापस;
        }

        अगर (ngx_atomic_cmp_set(mtx->रुको, रुको, रुको - 1)) {
            अवरोध;
        }
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx wake %uA", रुको);

    अगर (sem_post(&mtx->sem) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_post() failed while wake shmtx");
    }

#पूर्ण_अगर
}


#अन्यथा


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    अगर (mtx->name) {

        अगर (ngx_strcmp(name, mtx->name) == 0) {
            mtx->name = name;
            वापस NGX_OK;
        }

        ngx_shmtx_destroy(mtx);
    }

    mtx->fd = ngx_open_file(name, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                            NGX_FILE_DEFAULT_ACCESS);

    अगर (mtx->fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, ngx_cycle->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", name);
        वापस NGX_ERROR;
    }

    अगर (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }

    mtx->name = name;

    वापस NGX_OK;
}


व्योम
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
    अगर (ngx_close_file(mtx->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", mtx->name);
    }
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_trylock_fd(mtx->fd);

    अगर (err == 0) {
        वापस 1;
    }

    अगर (err == NGX_EAGAIN) {
        वापस 0;
    }

#अगर __osf__ /* Tru64 UNIX */

    अगर (err == NGX_EACCES) {
        वापस 0;
    }

#पूर्ण_अगर

    ngx_log_abort(err, ngx_trylock_fd_n " %s failed", mtx->name);

    वापस 0;
}


व्योम
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_lock_fd(mtx->fd);

    अगर (err == 0) {
        वापस;
    }

    ngx_log_abort(err, ngx_lock_fd_n " %s failed", mtx->name);
}


व्योम
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_unlock_fd(mtx->fd);

    अगर (err == 0) {
        वापस;
    }

    ngx_log_abort(err, ngx_unlock_fd_n " %s failed", mtx->name);
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    वापस 0;
}

#पूर्ण_अगर
