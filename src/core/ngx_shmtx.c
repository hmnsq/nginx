
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


#जर (NGX_HAVE_ATOMIC_OPS)


स्थिर शून्य ngx_shmtx_wakeup(ngx_shmtx_t *mtx);


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    mtx->lock = &addr->lock;

    जर (mtx->spin == (ngx_uint_t) -1) {
        परत NGX_OK;
    }

    mtx->spin = 2048;

#जर (NGX_HAVE_POSIX_SEM)

    mtx->रुको = &addr->रुको;

    जर (sem_init(&mtx->sem, 1, 0) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_init() failed");
    } अन्यथा {
        mtx->semaphore = 1;
    }

#endif

    परत NGX_OK;
}


शून्य
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
#जर (NGX_HAVE_POSIX_SEM)

    जर (mtx->semaphore) {
        जर (sem_destroy(&mtx->sem) == -1) {
            ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                          "sem_destroy() failed");
        }
    }

#endif
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    परत (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid));
}


शून्य
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_uint_t         i, n;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx lock");

    साठी ( ;; ) {

        जर (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
            परत;
        }

        जर (ngx_ncpu > 1) {

            साठी (n = 1; n < mtx->spin; n <<= 1) {

                साठी (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                जर (*mtx->lock == 0
                    && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid))
                {
                    परत;
                }
            }
        }

#जर (NGX_HAVE_POSIX_SEM)

        जर (mtx->semaphore) {
            (शून्य) ngx_atomic_fetch_add(mtx->रुको, 1);

            जर (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid)) {
                (शून्य) ngx_atomic_fetch_add(mtx->रुको, -1);
                परत;
            }

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx wait %uA", *mtx->रुको);

            पर्यंत (sem_wait(&mtx->sem) == -1) {
                ngx_err_t  err;

                err = ngx_errno;

                जर (err != NGX_EINTR) {
                    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, err,
                                  "sem_wait() failed while waiting on shmtx");
                    थांबा;
                }
            }

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                           "shmtx awoke");

            सुरूठेवा;
        }

#endif

        ngx_sched_yield();
    }
}


शून्य
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    जर (mtx->spin != (ngx_uint_t) -1) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "shmtx unlock");
    }

    जर (ngx_atomic_cmp_set(mtx->lock, ngx_pid, 0)) {
        ngx_shmtx_wakeup(mtx);
    }
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx forced unlock");

    जर (ngx_atomic_cmp_set(mtx->lock, pid, 0)) {
        ngx_shmtx_wakeup(mtx);
        परत 1;
    }

    परत 0;
}


स्थिर शून्य
ngx_shmtx_wakeup(ngx_shmtx_t *mtx)
{
#जर (NGX_HAVE_POSIX_SEM)
    ngx_atomic_uint_t  रुको;

    जर (!mtx->semaphore) {
        परत;
    }

    साठी ( ;; ) {

        रुको = *mtx->रुको;

        जर ((ngx_atomic_int_t) रुको <= 0) {
            परत;
        }

        जर (ngx_atomic_cmp_set(mtx->रुको, रुको, रुको - 1)) {
            थांबा;
        }
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "shmtx wake %uA", रुको);

    जर (sem_post(&mtx->sem) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      "sem_post() failed while wake shmtx");
    }

#endif
}


#अन्यथा


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    जर (mtx->name) {

        जर (ngx_strcmp(name, mtx->name) == 0) {
            mtx->name = name;
            परत NGX_OK;
        }

        ngx_shmtx_destroy(mtx);
    }

    mtx->fd = ngx_open_file(name, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                            NGX_FILE_DEFAULT_ACCESS);

    जर (mtx->fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, ngx_cycle->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", name);
        परत NGX_ERROR;
    }

    जर (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }

    mtx->name = name;

    परत NGX_OK;
}


शून्य
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
    जर (ngx_close_file(mtx->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", mtx->name);
    }
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_trylock_fd(mtx->fd);

    जर (err == 0) {
        परत 1;
    }

    जर (err == NGX_EAGAIN) {
        परत 0;
    }

#जर __osf__ /* Tru64 UNIX */

    जर (err == NGX_EACCES) {
        परत 0;
    }

#endif

    ngx_log_abort(err, ngx_trylock_fd_n " %s failed", mtx->name);

    परत 0;
}


शून्य
ngx_shmtx_lock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_lock_fd(mtx->fd);

    जर (err == 0) {
        परत;
    }

    ngx_log_abort(err, ngx_lock_fd_n " %s failed", mtx->name);
}


शून्य
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_unlock_fd(mtx->fd);

    जर (err == 0) {
        परत;
    }

    ngx_log_abort(err, ngx_unlock_fd_n " %s failed", mtx->name);
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    परत 0;
}

#endif
