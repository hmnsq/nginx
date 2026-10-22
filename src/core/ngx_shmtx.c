
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


#اگر (NGX_HAVE_ATOMIC_OPS)


ساکن خالی ngx_shmtx_wakeup(ngx_shmtx_t *mtx);


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    mtx->تالا = &addr->تالا;

    اگر (mtx->spin == (ngx_uint_t) -1) {
        واپس NGX_OK;
    }

    mtx->spin = 2048;

#اگر (NGX_HAVE_POSIX_SEM)

    mtx->wait = &addr->wait;

    اگر (sem_init(&mtx->sem, 1, 0) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_چکر->لوگارتھم, ngx_errno,
                      "sem_init() failed");
    } ورنہ {
        mtx->semaphore = 1;
    }

#اگر_ختم

    واپس NGX_OK;
}


خالی
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
#اگر (NGX_HAVE_POSIX_SEM)

    اگر (mtx->semaphore) {
        اگر (sem_destroy(&mtx->sem) == -1) {
            ngx_log_error(NGX_LOG_ALERT, ngx_چکر->لوگارتھم, ngx_errno,
                          "sem_destroy() failed");
        }
    }

#اگر_ختم
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    واپس (*mtx->تالا == 0 && ngx_atomic_cmp_سیٹ(mtx->تالا, 0, ngx_pid));
}


خالی
ngx_shmtx_تالا(ngx_shmtx_t *mtx)
{
    ngx_uint_t         i, n;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_چکر->لوگارتھم, 0, "shmtx lock");

    برائے ( ;; ) {

        اگر (*mtx->تالا == 0 && ngx_atomic_cmp_سیٹ(mtx->تالا, 0, ngx_pid)) {
            واپس;
        }

        اگر (ngx_ncpu > 1) {

            برائے (n = 1; n < mtx->spin; n <<= 1) {

                برائے (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                اگر (*mtx->تالا == 0
                    && ngx_atomic_cmp_سیٹ(mtx->تالا, 0, ngx_pid))
                {
                    واپس;
                }
            }
        }

#اگر (NGX_HAVE_POSIX_SEM)

        اگر (mtx->semaphore) {
            (خالی) ngx_atomic_fetch_جوڑو(mtx->wait, 1);

            اگر (*mtx->تالا == 0 && ngx_atomic_cmp_سیٹ(mtx->تالا, 0, ngx_pid)) {
                (خالی) ngx_atomic_fetch_جوڑو(mtx->wait, -1);
                واپس;
            }

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_چکر->لوگارتھم, 0,
                           "shmtx wait %uA", *mtx->wait);

            جب (sem_wait(&mtx->sem) == -1) {
                ngx_err_t  err;

                err = ngx_errno;

                اگر (err != NGX_EINTR) {
                    ngx_log_error(NGX_LOG_ALERT, ngx_چکر->لوگارتھم, err,
                                  "sem_wait() failed while waiting on shmtx");
                    توڑو;
                }
            }

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_چکر->لوگارتھم, 0,
                           "shmtx awoke");

            جاری;
        }

#اگر_ختم

        ngx_sched_yield();
    }
}


خالی
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    اگر (mtx->spin != (ngx_uint_t) -1) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_چکر->لوگارتھم, 0, "shmtx unlock");
    }

    اگر (ngx_atomic_cmp_سیٹ(mtx->تالا, ngx_pid, 0)) {
        ngx_shmtx_wakeup(mtx);
    }
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_چکر->لوگارتھم, 0,
                   "shmtx forced unlock");

    اگر (ngx_atomic_cmp_سیٹ(mtx->تالا, pid, 0)) {
        ngx_shmtx_wakeup(mtx);
        واپس 1;
    }

    واپس 0;
}


ساکن خالی
ngx_shmtx_wakeup(ngx_shmtx_t *mtx)
{
#اگر (NGX_HAVE_POSIX_SEM)
    ngx_atomic_uint_t  wait;

    اگر (!mtx->semaphore) {
        واپس;
    }

    برائے ( ;; ) {

        wait = *mtx->wait;

        اگر ((ngx_atomic_int_t) wait <= 0) {
            واپس;
        }

        اگر (ngx_atomic_cmp_سیٹ(mtx->wait, wait, wait - 1)) {
            توڑو;
        }
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_چکر->لوگارتھم, 0,
                   "shmtx wake %uA", wait);

    اگر (sem_post(&mtx->sem) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_چکر->لوگارتھم, ngx_errno,
                      "sem_post() failed while wake shmtx");
    }

#اگر_ختم
}


#ورنہ


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    اگر (mtx->name) {

        اگر (ngx_strcmp(name, mtx->name) == 0) {
            mtx->name = name;
            واپس NGX_OK;
        }

        ngx_shmtx_destroy(mtx);
    }

    mtx->fd = ngx_open_file(name, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                            NGX_FILE_DEFAULT_ACCESS);

    اگر (mtx->fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, ngx_چکر->لوگارتھم, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", name);
        واپس NGX_ERROR;
    }

    اگر (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_چکر->لوگارتھم, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }

    mtx->name = name;

    واپس NGX_OK;
}


خالی
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
    اگر (ngx_close_file(mtx->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_چکر->لوگارتھم, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", mtx->name);
    }
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_trylock_fd(mtx->fd);

    اگر (err == 0) {
        واپس 1;
    }

    اگر (err == NGX_EAGAIN) {
        واپس 0;
    }

#اگر __osf__ /* Tru64 UNIX */

    اگر (err == NGX_EACCES) {
        واپس 0;
    }

#اگر_ختم

    ngx_log_abort(err, ngx_trylock_fd_n " %s failed", mtx->name);

    واپس 0;
}


خالی
ngx_shmtx_تالا(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_تالا_fd(mtx->fd);

    اگر (err == 0) {
        واپس;
    }

    ngx_log_abort(err, ngx_تالا_fd_n " %s failed", mtx->name);
}


خالی
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_unlock_fd(mtx->fd);

    اگر (err == 0) {
        واپس;
    }

    ngx_log_abort(err, ngx_unlock_fd_n " %s failed", mtx->name);
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    واپس 0;
}

#اگر_ختم
