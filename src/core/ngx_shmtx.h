
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _NGX_SHMTX_H_INCLUDED_
#ವ್ಯಾಖ್ಯಾನಿಸು _NGX_SHMTX_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_atomic_t   ಬೀಗ;
#ಆದರೆ (NGX_HAVE_POSIX_SEM)
    ngx_atomic_t   wait;
#ಆದರೆ_ಅಂತ್ಯ
} ngx_shmtx_sh_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
#ಆದರೆ (NGX_HAVE_ATOMIC_OPS)
    ngx_atomic_t  *ಬೀಗ;
#ಆದರೆ (NGX_HAVE_POSIX_SEM)
    ngx_atomic_t  *wait;
    ngx_uint_t     semaphore;
    sem_t          sem;
#ಆದರೆ_ಅಂತ್ಯ
#ಇಲ್ಲದಿದ್ದರೆ
    ngx_fd_t       fd;
    u_char        *name;
#ಆದರೆ_ಅಂತ್ಯ
    ngx_uint_t     spin;
} ngx_shmtx_t;


ngx_int_t ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr,
    u_char *name);
ಶೂನ್ಯ ngx_shmtx_destroy(ngx_shmtx_t *mtx);
ngx_uint_t ngx_shmtx_trylock(ngx_shmtx_t *mtx);
ಶೂನ್ಯ ngx_shmtx_ಬೀಗ(ngx_shmtx_t *mtx);
ಶೂನ್ಯ ngx_shmtx_unlock(ngx_shmtx_t *mtx);
ngx_uint_t ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid);


#ಆದರೆ_ಅಂತ್ಯ /* _NGX_SHMTX_H_INCLUDED_ */
