
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अगर_अघोषित _NGX_SHMTX_H_INCLUDED_
#घोषणा _NGX_SHMTX_H_INCLUDED_


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


प्रकार काष्ठा {
    ngx_atomic_t   lock;
#अगर (NGX_HAVE_POSIX_SEM)
    ngx_atomic_t   रुको;
#पूर्ण_अगर
} ngx_shmtx_sh_t;


प्रकार काष्ठा {
#अगर (NGX_HAVE_ATOMIC_OPS)
    ngx_atomic_t  *lock;
#अगर (NGX_HAVE_POSIX_SEM)
    ngx_atomic_t  *रुको;
    ngx_uint_t     semaphore;
    sem_t          sem;
#पूर्ण_अगर
#अन्यथा
    ngx_fd_t       fd;
    u_char        *name;
#पूर्ण_अगर
    ngx_uint_t     spin;
} ngx_shmtx_t;


ngx_int_t ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr,
    u_char *name);
व्योम ngx_shmtx_destroy(ngx_shmtx_t *mtx);
ngx_uint_t ngx_shmtx_trylock(ngx_shmtx_t *mtx);
व्योम ngx_shmtx_lock(ngx_shmtx_t *mtx);
व्योम ngx_shmtx_unlock(ngx_shmtx_t *mtx);
ngx_uint_t ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid);


#पूर्ण_अगर /* _NGX_SHMTX_H_INCLUDED_ */
