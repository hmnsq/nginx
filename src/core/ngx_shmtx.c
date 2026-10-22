
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#إذا (NGX_HAVE_ATOMIC_OPS)


ساكن فراغ ngx_shmtx_wakeup(ngx_shmtx_t *mtx);


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    mtx->قفل = &addr->قفل;

    إذا (mtx->spin == (ngx_uint_t) -1) {
        أرجع NGX_OK;
    }

    mtx->spin = 2048;

#إذا (NGX_HAVE_POSIX_SEM)

    mtx->wait = &addr->wait;

    إذا (sem_init(&mtx->sem, 1, 0) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_چرخه->لگاریتم, ngx_errno,
                      "sem_init() failed");
    } وإلا {
        mtx->semaphore = 1;
    }

#پایان_اگر

    أرجع NGX_OK;
}


فراغ
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
#إذا (NGX_HAVE_POSIX_SEM)

    إذا (mtx->semaphore) {
        إذا (sem_destroy(&mtx->sem) == -1) {
            ngx_log_error(NGX_LOG_ALERT, ngx_چرخه->لگاریتم, ngx_errno,
                          "sem_destroy() failed");
        }
    }

#پایان_اگر
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    أرجع (*mtx->قفل == 0 && ngx_atomic_cmp_مجموعه(mtx->قفل, 0, ngx_pid));
}


فراغ
ngx_shmtx_قفل(ngx_shmtx_t *mtx)
{
    ngx_uint_t         i, n;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_چرخه->لگاریتم, 0, "shmtx lock");

    لكل ( ;; ) {

        إذا (*mtx->قفل == 0 && ngx_atomic_cmp_مجموعه(mtx->قفل, 0, ngx_pid)) {
            أرجع;
        }

        إذا (ngx_ncpu > 1) {

            لكل (n = 1; n < mtx->spin; n <<= 1) {

                لكل (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                إذا (*mtx->قفل == 0
                    && ngx_atomic_cmp_مجموعه(mtx->قفل, 0, ngx_pid))
                {
                    أرجع;
                }
            }
        }

#إذا (NGX_HAVE_POSIX_SEM)

        إذا (mtx->semaphore) {
            (فراغ) ngx_atomic_fetch_جمع_کن(mtx->wait, 1);

            إذا (*mtx->قفل == 0 && ngx_atomic_cmp_مجموعه(mtx->قفل, 0, ngx_pid)) {
                (فراغ) ngx_atomic_fetch_جمع_کن(mtx->wait, -1);
                أرجع;
            }

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_چرخه->لگاریتم, 0,
                           "shmtx wait %uA", *mtx->wait);

            طالما (sem_wait(&mtx->sem) == -1) {
                ngx_err_t  err;

                err = ngx_errno;

                إذا (err != NGX_EINTR) {
                    ngx_log_error(NGX_LOG_ALERT, ngx_چرخه->لگاریتم, err,
                                  "sem_wait() failed while waiting on shmtx");
                    اكسر;
                }
            }

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_چرخه->لگاریتم, 0,
                           "shmtx awoke");

            استمر;
        }

#پایان_اگر

        ngx_sched_yield();
    }
}


فراغ
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    إذا (mtx->spin != (ngx_uint_t) -1) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_چرخه->لگاریتم, 0, "shmtx unlock");
    }

    إذا (ngx_atomic_cmp_مجموعه(mtx->قفل, ngx_pid, 0)) {
        ngx_shmtx_wakeup(mtx);
    }
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_چرخه->لگاریتم, 0,
                   "shmtx forced unlock");

    إذا (ngx_atomic_cmp_مجموعه(mtx->قفل, pid, 0)) {
        ngx_shmtx_wakeup(mtx);
        أرجع 1;
    }

    أرجع 0;
}


ساكن فراغ
ngx_shmtx_wakeup(ngx_shmtx_t *mtx)
{
#إذا (NGX_HAVE_POSIX_SEM)
    ngx_atomic_uint_t  wait;

    إذا (!mtx->semaphore) {
        أرجع;
    }

    لكل ( ;; ) {

        wait = *mtx->wait;

        إذا ((ngx_atomic_int_t) wait <= 0) {
            أرجع;
        }

        إذا (ngx_atomic_cmp_مجموعه(mtx->wait, wait, wait - 1)) {
            اكسر;
        }
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_چرخه->لگاریتم, 0,
                   "shmtx wake %uA", wait);

    إذا (sem_post(&mtx->sem) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_چرخه->لگاریتم, ngx_errno,
                      "sem_post() failed while wake shmtx");
    }

#پایان_اگر
}


#وإلا


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    إذا (mtx->name) {

        إذا (ngx_strcmp(name, mtx->name) == 0) {
            mtx->name = name;
            أرجع NGX_OK;
        }

        ngx_shmtx_destroy(mtx);
    }

    mtx->fd = ngx_open_file(name, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                            NGX_FILE_DEFAULT_ACCESS);

    إذا (mtx->fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, ngx_چرخه->لگاریتم, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", name);
        أرجع NGX_ERROR;
    }

    إذا (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_چرخه->لگاریتم, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }

    mtx->name = name;

    أرجع NGX_OK;
}


فراغ
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
    إذا (ngx_close_file(mtx->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_چرخه->لگاریتم, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", mtx->name);
    }
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_trylock_fd(mtx->fd);

    إذا (err == 0) {
        أرجع 1;
    }

    إذا (err == NGX_EAGAIN) {
        أرجع 0;
    }

#إذا __osf__ /* Tru64 UNIX */

    إذا (err == NGX_EACCES) {
        أرجع 0;
    }

#پایان_اگر

    ngx_log_abort(err, ngx_trylock_fd_n " %s failed", mtx->name);

    أرجع 0;
}


فراغ
ngx_shmtx_قفل(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_قفل_fd(mtx->fd);

    إذا (err == 0) {
        أرجع;
    }

    ngx_log_abort(err, ngx_قفل_fd_n " %s failed", mtx->name);
}


فراغ
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_unlock_fd(mtx->fd);

    إذا (err == 0) {
        أرجع;
    }

    ngx_log_abort(err, ngx_unlock_fd_n " %s failed", mtx->name);
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    أرجع 0;
}

#پایان_اگر
