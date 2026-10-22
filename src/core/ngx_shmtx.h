
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#यदि_अपरिभाषितम् _NGX_SHMTX_H_INCLUDED_
#परिभाषय _NGX_SHMTX_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


प्रकारनिर्देश संरचना {
    ngx_atomic_t   तालकम्;
#यदि (NGX_HAVE_POSIX_SEM)
    ngx_atomic_t   प्रतीक्षस्व;
#यदि_अन्तः
} ngx_shmtx_sh_t;


प्रकारनिर्देश संरचना {
#यदि (NGX_HAVE_ATOMIC_OPS)
    ngx_atomic_t  *तालकम्;
#यदि (NGX_HAVE_POSIX_SEM)
    ngx_atomic_t  *प्रतीक्षस्व;
    ngx_uint_t     semaphore;
    sem_t          sem;
#यदि_अन्तः
#अन्यथा
    ngx_fd_t       fd;
    u_char        *name;
#यदि_अन्तः
    ngx_uint_t     spin;
} ngx_shmtx_t;


ngx_int_t ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr,
    u_char *name);
शून्य ngx_shmtx_destroy(ngx_shmtx_t *mtx);
ngx_uint_t ngx_shmtx_trylock(ngx_shmtx_t *mtx);
शून्य ngx_shmtx_तालकम्(ngx_shmtx_t *mtx);
शून्य ngx_shmtx_unlock(ngx_shmtx_t *mtx);
ngx_uint_t ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid);


#यदि_अन्तः /* _NGX_SHMTX_H_INCLUDED_ */
