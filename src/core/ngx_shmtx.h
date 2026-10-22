
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_تعریف_نشده _NGX_SHMTX_H_INCLUDED_
#تعریف_کن _NGX_SHMTX_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية {
    ngx_atomic_t   قفل;
#إذا (NGX_HAVE_POSIX_SEM)
    ngx_atomic_t   wait;
#پایان_اگر
} ngx_shmtx_sh_t;


تعريفنوع بنية {
#إذا (NGX_HAVE_ATOMIC_OPS)
    ngx_atomic_t  *قفل;
#إذا (NGX_HAVE_POSIX_SEM)
    ngx_atomic_t  *wait;
    ngx_uint_t     semaphore;
    sem_t          sem;
#پایان_اگر
#وإلا
    ngx_fd_t       fd;
    u_char        *name;
#پایان_اگر
    ngx_uint_t     spin;
} ngx_shmtx_t;


ngx_int_t ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr,
    u_char *name);
فراغ ngx_shmtx_destroy(ngx_shmtx_t *mtx);
ngx_uint_t ngx_shmtx_trylock(ngx_shmtx_t *mtx);
فراغ ngx_shmtx_قفل(ngx_shmtx_t *mtx);
فراغ ngx_shmtx_unlock(ngx_shmtx_t *mtx);
ngx_uint_t ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid);


#پایان_اگر /* _NGX_SHMTX_H_INCLUDED_ */
