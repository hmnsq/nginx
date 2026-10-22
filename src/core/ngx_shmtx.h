
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_SHMTX_H_INCLUDED_
#define _NGX_SHMTX_H_INCLUDED_


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ngx_atomic_t   lock;
#ଯଦି (NGX_HAVE_POSIX_SEM)
    ngx_atomic_t   wait;
#endif
} ngx_shmtx_sh_t;


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
#ଯଦି (NGX_HAVE_ATOMIC_OPS)
    ngx_atomic_t  *lock;
#ଯଦି (NGX_HAVE_POSIX_SEM)
    ngx_atomic_t  *wait;
    ngx_uint_t     semaphore;
    sem_t          sem;
#endif
#ଅନ୍ୟଥା
    ngx_fd_t       fd;
    u_char        *name;
#endif
    ngx_uint_t     spin;
} ngx_shmtx_t;


ngx_int_t ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr,
    u_char *name);
ଶୂନ୍ୟ ngx_shmtx_destroy(ngx_shmtx_t *mtx);
ngx_uint_t ngx_shmtx_trylock(ngx_shmtx_t *mtx);
ଶୂନ୍ୟ ngx_shmtx_lock(ngx_shmtx_t *mtx);
ଶୂନ୍ୟ ngx_shmtx_unlock(ngx_shmtx_t *mtx);
ngx_uint_t ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid);


#endif /* _NGX_SHMTX_H_INCLUDED_ */
