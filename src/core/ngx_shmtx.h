
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_غیر_تعریف_شدہ _NGX_SHMTX_H_INCLUDED_
#تعریف_کرو _NGX_SHMTX_H_INCLUDED_


#شامل <ngx_config.h>
#شامل <ngx_core.h>


قسمتعریف ساخت {
    ngx_atomic_t   تالا;
#اگر (NGX_HAVE_POSIX_SEM)
    ngx_atomic_t   wait;
#اگر_ختم
} ngx_shmtx_sh_t;


قسمتعریف ساخت {
#اگر (NGX_HAVE_ATOMIC_OPS)
    ngx_atomic_t  *تالا;
#اگر (NGX_HAVE_POSIX_SEM)
    ngx_atomic_t  *wait;
    ngx_uint_t     semaphore;
    sem_t          sem;
#اگر_ختم
#ورنہ
    ngx_fd_t       fd;
    u_char        *name;
#اگر_ختم
    ngx_uint_t     spin;
} ngx_shmtx_t;


ngx_int_t ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr,
    u_char *name);
خالی ngx_shmtx_destroy(ngx_shmtx_t *mtx);
ngx_uint_t ngx_shmtx_trylock(ngx_shmtx_t *mtx);
خالی ngx_shmtx_تالا(ngx_shmtx_t *mtx);
خالی ngx_shmtx_unlock(ngx_shmtx_t *mtx);
ngx_uint_t ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid);


#اگر_ختم /* _NGX_SHMTX_H_INCLUDED_ */
