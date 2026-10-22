
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>


#જો (NGX_HAVE_ATOMIC_OPS)


સ્થિર શૂન્ય ngx_shmtx_wakeup(ngx_shmtx_t *mtx);


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    mtx->lock = &addr->lock;

    જો (mtx->spin == (ngx_uint_t) -1) {
        પાછા NGX_OK;
    }

    mtx->spin = 2048;

#જો (NGX_HAVE_POSIX_SEM)

    mtx->wait = &addr->wait;

    જો (sem_init(&mtx->sem, 1, 0) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_init() failed");
    } નહીંતર {
        mtx->semaphore = 1;
    }

#endif

    પાછા NGX_OK;
}


શૂન્ય
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
#જો (NGX_HAVE_POSIX_SEM)

    જો (mtx->semaphore) {
        જો (sem_destroy(&mtx->sem) == -1) {
            ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                          "sem_destroy() failed");
        }
    }

#endif
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    પાછા (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid));
}


શૂન્ય
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_uint_t         i, n;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx lock");

    માટે ( ;; ) {

        જો (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
            પાછા;
        }

        જો (ngx_ncpu > 1) {

            માટે (n = 1; n < mtx->spin; n <<= 1) {

                માટે (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                જો (*mtx->lock == 0
                    && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid))
                {
                    પાછા;
                }
            }
        }

#જો (NGX_HAVE_POSIX_SEM)

        જો (mtx->semaphore) {
            (શૂન્ય) ngx_atomic_fetch_add(mtx->wait, 1);

            જો (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
                (શૂન્ય) ngx_atomic_fetch_add(mtx->wait, -1);
                પાછા;
            }

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx wait %uA", *mtx->wait);

            જ્યારે (sem_wait(&mtx->sem) == -1) {
                ngx_err_t  err;

                err = ngx_errno;

                જો (err != NGX_EINTR) {
                    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, err,
                                  "sem_wait() failed while waiting on shmtx");
                    તોડો;
                }
            }

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx awoke");

            ચાલુ;
        }

#endif

        ngx_sched_yield();
    }
}


શૂન્ય
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    જો (mtx->spin != (ngx_uint_t) -1) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx unlock");
    }

    જો (ngx_atomic_cmp_set(mtx->lock, ngx_pid, 0)) {
        ngx_shmtx_wakeup(mtx);
    }
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx forced unlock");

    જો (ngx_atomic_cmp_set(mtx->lock, pid, 0)) {
        ngx_shmtx_wakeup(mtx);
        પાછા 1;
    }

    પાછા 0;
}


સ્થિર શૂન્ય
ngx_shmtx_wakeup(ngx_shmtx_t *mtx)
{
#જો (NGX_HAVE_POSIX_SEM)
    ngx_atomic_uint_t  wait;

    જો (!mtx->semaphore) {
        પાછા;
    }

    માટે ( ;; ) {

        wait = *mtx->wait;

        જો ((ngx_atomic_int_t) wait <= 0) {
            પાછા;
        }

        જો (ngx_atomic_cmp_set(mtx->wait, wait, wait - 1)) {
            તોડો;
        }
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx wake %uA", wait);

    જો (sem_post(&mtx->sem) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_post() failed while wake shmtx");
    }

#endif
}


#નહીંતર


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    જો (mtx->name) {

        જો (ngx_strcmp(name, mtx->name) == 0) {
            mtx->name = name;
            પાછા NGX_OK;
        }

        ngx_shmtx_destroy(mtx);
    }

    mtx->fd = ngx_open_file(name, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                            NGX_FILE_DEFAULT_ACCESS);

    જો (mtx->fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, ngx_cycle->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", name);
        પાછા NGX_ERROR;
    }

    જો (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }

    mtx->name = name;

    પાછા NGX_OK;
}


શૂન્ય
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
    જો (ngx_close_file(mtx->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", mtx->name);
    }
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_trylock_fd(mtx->fd);

    જો (err == 0) {
        પાછા 1;
    }

    જો (err == NGX_EAGAIN) {
        પાછા 0;
    }

#જો __osf__ /* Tru64 UNIX */

    જો (err == NGX_EACCES) {
        પાછા 0;
    }

#endif

    ngx_log_abort(err, ngx_trylock_fd_n " %s failed", mtx->name);

    પાછા 0;
}


શૂન્ય
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_lock_fd(mtx->fd);

    જો (err == 0) {
        પાછા;
    }

    ngx_log_abort(err, ngx_lock_fd_n " %s failed", mtx->name);
}


શૂન્ય
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_unlock_fd(mtx->fd);

    જો (err == 0) {
        પાછા;
    }

    ngx_log_abort(err, ngx_unlock_fd_n " %s failed", mtx->name);
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    પાછા 0;
}

#endif
