
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#که_نه_تعریف_شوی _NGX_SHMTX_H_INCLUDED_
#تعریف_کړه _NGX_SHMTX_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية {
    ngx_atomic_t   قلف;
#إذا (NGX_HAVE_POSIX_SEM)
    ngx_atomic_t   wait;
#د_که_پای
} ngx_shmtx_sh_t;


تعريفنوع بنية {
#إذا (NGX_HAVE_ATOMIC_OPS)
    ngx_atomic_t  *قلف;
#إذا (NGX_HAVE_POSIX_SEM)
    ngx_atomic_t  *wait;
    ngx_uint_t     semaphore;
    sem_t          sem;
#د_که_پای
#وإلا
    ngx_fd_t       fd;
    u_char        *name;
#د_که_پای
    ngx_uint_t     spin;
} ngx_shmtx_t;


ngx_int_t ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr,
    u_char *name);
فراغ ngx_shmtx_destroy(ngx_shmtx_t *mtx);
ngx_uint_t ngx_shmtx_trylock(ngx_shmtx_t *mtx);
فراغ ngx_shmtx_قلف(ngx_shmtx_t *mtx);
فراغ ngx_shmtx_unlock(ngx_shmtx_t *mtx);
ngx_uint_t ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid);


#د_که_پای /* _NGX_SHMTX_H_INCLUDED_ */
