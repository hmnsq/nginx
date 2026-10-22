
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _NGX_SHMTX_H_INCLUDED_
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _NGX_SHMTX_H_INCLUDED_


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_atomic_t   ਤਾਲਾ;
#ਜੇ (NGX_HAVE_POSIX_SEM)
    ngx_atomic_t   wait;
#ਜੇ_ਅੰਤ
} ngx_shmtx_sh_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
#ਜੇ (NGX_HAVE_ATOMIC_OPS)
    ngx_atomic_t  *ਤਾਲਾ;
#ਜੇ (NGX_HAVE_POSIX_SEM)
    ngx_atomic_t  *wait;
    ngx_uint_t     semaphore;
    sem_t          sem;
#ਜੇ_ਅੰਤ
#ਨਹੀਂਤਾਂ
    ngx_fd_t       fd;
    u_char        *name;
#ਜੇ_ਅੰਤ
    ngx_uint_t     spin;
} ngx_shmtx_t;


ngx_int_t ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr,
    u_char *name);
ਖਾਲੀ ngx_shmtx_destroy(ngx_shmtx_t *mtx);
ngx_uint_t ngx_shmtx_trylock(ngx_shmtx_t *mtx);
ਖਾਲੀ ngx_shmtx_ਤਾਲਾ(ngx_shmtx_t *mtx);
ਖਾਲੀ ngx_shmtx_unlock(ngx_shmtx_t *mtx);
ngx_uint_t ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid);


#ਜੇ_ਅੰਤ /* _NGX_SHMTX_H_INCLUDED_ */
