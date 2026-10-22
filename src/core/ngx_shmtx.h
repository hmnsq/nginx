
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جيڪڏهن_وصف_نه_ٿيل _NGX_SHMTX_H_INCLUDED_
#وصف_ڪريو _NGX_SHMTX_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية {
    ngx_atomic_t   تالو;
#إذا (NGX_HAVE_POSIX_SEM)
    ngx_atomic_t   wait;
#جيڪڏهن_پورو
} ngx_shmtx_sh_t;


تعريفنوع بنية {
#إذا (NGX_HAVE_ATOMIC_OPS)
    ngx_atomic_t  *تالو;
#إذا (NGX_HAVE_POSIX_SEM)
    ngx_atomic_t  *wait;
    ngx_uint_t     semaphore;
    sem_t          sem;
#جيڪڏهن_پورو
#وإلا
    ngx_fd_t       fd;
    u_char        *name;
#جيڪڏهن_پورو
    ngx_uint_t     spin;
} ngx_shmtx_t;


ngx_int_t ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr,
    u_char *name);
فراغ ngx_shmtx_destroy(ngx_shmtx_t *mtx);
ngx_uint_t ngx_shmtx_trylock(ngx_shmtx_t *mtx);
فراغ ngx_shmtx_تالو(ngx_shmtx_t *mtx);
فراغ ngx_shmtx_unlock(ngx_shmtx_t *mtx);
ngx_uint_t ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid);


#جيڪڏهن_پورو /* _NGX_SHMTX_H_INCLUDED_ */
