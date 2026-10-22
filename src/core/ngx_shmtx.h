
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#నిర్వచితం_కాకపోతే _NGX_SHMTX_H_INCLUDED_
#నిర్వచించు _NGX_SHMTX_H_INCLUDED_


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


రకనిర్వచనం నిర్మాణం {
    ngx_atomic_t   తాళం;
#ఒకవేళ (NGX_HAVE_POSIX_SEM)
    ngx_atomic_t   wait;
#అయితే_ముగింపు
} ngx_shmtx_sh_t;


రకనిర్వచనం నిర్మాణం {
#ఒకవేళ (NGX_HAVE_ATOMIC_OPS)
    ngx_atomic_t  *తాళం;
#ఒకవేళ (NGX_HAVE_POSIX_SEM)
    ngx_atomic_t  *wait;
    ngx_uint_t     semaphore;
    sem_t          sem;
#అయితే_ముగింపు
#లేకపోతే
    ngx_fd_t       fd;
    u_char        *name;
#అయితే_ముగింపు
    ngx_uint_t     spin;
} ngx_shmtx_t;


ngx_int_t ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr,
    u_char *name);
శూన్య ngx_shmtx_destroy(ngx_shmtx_t *mtx);
ngx_uint_t ngx_shmtx_trylock(ngx_shmtx_t *mtx);
శూన్య ngx_shmtx_తాళం(ngx_shmtx_t *mtx);
శూన్య ngx_shmtx_unlock(ngx_shmtx_t *mtx);
ngx_uint_t ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid);


#అయితే_ముగింపు /* _NGX_SHMTX_H_INCLUDED_ */
