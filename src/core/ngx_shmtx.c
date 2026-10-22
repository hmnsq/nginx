
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


#ఒకవేళ (NGX_HAVE_ATOMIC_OPS)


స్థిర శూన్య ngx_shmtx_wakeup(ngx_shmtx_t *mtx);


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    mtx->తాళం = &addr->తాళం;

    ఒకవేళ (mtx->spin == (ngx_uint_t) -1) {
        తిరిగి NGX_OK;
    }

    mtx->spin = 2048;

#ఒకవేళ (NGX_HAVE_POSIX_SEM)

    mtx->wait = &addr->wait;

    ఒకవేళ (sem_init(&mtx->sem, 1, 0) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_చక్రం->సంవర్గమానం, ngx_errno,
                      "sem_init() failed");
    } లేకపోతే {
        mtx->semaphore = 1;
    }

#అయితే_ముగింపు

    తిరిగి NGX_OK;
}


శూన్య
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
#ఒకవేళ (NGX_HAVE_POSIX_SEM)

    ఒకవేళ (mtx->semaphore) {
        ఒకవేళ (sem_destroy(&mtx->sem) == -1) {
            ngx_log_error(NGX_LOG_ALERT, ngx_చక్రం->సంవర్గమానం, ngx_errno,
                          "sem_destroy() failed");
        }
    }

#అయితే_ముగింపు
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    తిరిగి (*mtx->తాళం == 0 && ngx_atomic_cmp_సమితి(mtx->తాళం, 0, ngx_pid));
}


శూన్య
ngx_shmtx_తాళం(ngx_shmtx_t *mtx)
{
    ngx_uint_t         i, n;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_చక్రం->సంవర్గమానం, 0, "shmtx lock");

    కొరకు ( ;; ) {

        ఒకవేళ (*mtx->తాళం == 0 && ngx_atomic_cmp_సమితి(mtx->తాళం, 0, ngx_pid)) {
            తిరిగి;
        }

        ఒకవేళ (ngx_ncpu > 1) {

            కొరకు (n = 1; n < mtx->spin; n <<= 1) {

                కొరకు (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                ఒకవేళ (*mtx->తాళం == 0
                    && ngx_atomic_cmp_సమితి(mtx->తాళం, 0, ngx_pid))
                {
                    తిరిగి;
                }
            }
        }

#ఒకవేళ (NGX_HAVE_POSIX_SEM)

        ఒకవేళ (mtx->semaphore) {
            (శూన్య) ngx_atomic_fetch_కూడు(mtx->wait, 1);

            ఒకవేళ (*mtx->తాళం == 0 && ngx_atomic_cmp_సమితి(mtx->తాళం, 0, ngx_pid)) {
                (శూన్య) ngx_atomic_fetch_కూడు(mtx->wait, -1);
                తిరిగి;
            }

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_చక్రం->సంవర్గమానం, 0,
                           "shmtx wait %uA", *mtx->wait);

            అయితే (sem_wait(&mtx->sem) == -1) {
                ngx_err_t  err;

                err = ngx_errno;

                ఒకవేళ (err != NGX_EINTR) {
                    ngx_log_error(NGX_LOG_ALERT, ngx_చక్రం->సంవర్గమానం, err,
                                  "sem_wait() failed while waiting on shmtx");
                    ఆపు;
                }
            }

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_చక్రం->సంవర్గమానం, 0,
                           "shmtx awoke");

            కొనసాగు;
        }

#అయితే_ముగింపు

        ngx_sched_yield();
    }
}


శూన్య
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    ఒకవేళ (mtx->spin != (ngx_uint_t) -1) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_చక్రం->సంవర్గమానం, 0, "shmtx unlock");
    }

    ఒకవేళ (ngx_atomic_cmp_సమితి(mtx->తాళం, ngx_pid, 0)) {
        ngx_shmtx_wakeup(mtx);
    }
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_చక్రం->సంవర్గమానం, 0,
                   "shmtx forced unlock");

    ఒకవేళ (ngx_atomic_cmp_సమితి(mtx->తాళం, pid, 0)) {
        ngx_shmtx_wakeup(mtx);
        తిరిగి 1;
    }

    తిరిగి 0;
}


స్థిర శూన్య
ngx_shmtx_wakeup(ngx_shmtx_t *mtx)
{
#ఒకవేళ (NGX_HAVE_POSIX_SEM)
    ngx_atomic_uint_t  wait;

    ఒకవేళ (!mtx->semaphore) {
        తిరిగి;
    }

    కొరకు ( ;; ) {

        wait = *mtx->wait;

        ఒకవేళ ((ngx_atomic_int_t) wait <= 0) {
            తిరిగి;
        }

        ఒకవేళ (ngx_atomic_cmp_సమితి(mtx->wait, wait, wait - 1)) {
            ఆపు;
        }
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_చక్రం->సంవర్గమానం, 0,
                   "shmtx wake %uA", wait);

    ఒకవేళ (sem_post(&mtx->sem) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ngx_చక్రం->సంవర్గమానం, ngx_errno,
                      "sem_post() failed while wake shmtx");
    }

#అయితే_ముగింపు
}


#లేకపోతే


ngx_int_t
ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr, u_char *name)
{
    ఒకవేళ (mtx->name) {

        ఒకవేళ (ngx_strcmp(name, mtx->name) == 0) {
            mtx->name = name;
            తిరిగి NGX_OK;
        }

        ngx_shmtx_destroy(mtx);
    }

    mtx->fd = ngx_open_file(name, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                            NGX_FILE_DEFAULT_ACCESS);

    ఒకవేళ (mtx->fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, ngx_చక్రం->సంవర్గమానం, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", name);
        తిరిగి NGX_ERROR;
    }

    ఒకవేళ (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_చక్రం->సంవర్గమానం, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }

    mtx->name = name;

    తిరిగి NGX_OK;
}


శూన్య
ngx_shmtx_destroy(ngx_shmtx_t *mtx)
{
    ఒకవేళ (ngx_close_file(mtx->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ngx_చక్రం->సంవర్గమానం, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", mtx->name);
    }
}


ngx_uint_t
ngx_shmtx_trylock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_trylock_fd(mtx->fd);

    ఒకవేళ (err == 0) {
        తిరిగి 1;
    }

    ఒకవేళ (err == NGX_EAGAIN) {
        తిరిగి 0;
    }

#ఒకవేళ __osf__ /* Tru64 UNIX */

    ఒకవేళ (err == NGX_EACCES) {
        తిరిగి 0;
    }

#అయితే_ముగింపు

    ngx_log_abort(err, ngx_trylock_fd_n " %s failed", mtx->name);

    తిరిగి 0;
}


శూన్య
ngx_shmtx_తాళం(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_తాళం_fd(mtx->fd);

    ఒకవేళ (err == 0) {
        తిరిగి;
    }

    ngx_log_abort(err, ngx_తాళం_fd_n " %s failed", mtx->name);
}


శూన్య
ngx_shmtx_unlock(ngx_shmtx_t *mtx)
{
    ngx_err_t  err;

    err = ngx_unlock_fd(mtx->fd);

    ఒకవేళ (err == 0) {
        తిరిగి;
    }

    ngx_log_abort(err, ngx_unlock_fd_n " %s failed", mtx->name);
}


ngx_uint_t
ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid)
{
    తిరిగి 0;
}

#అయితే_ముగింపు
