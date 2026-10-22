
/*
 * Copyright (C) Nginx, Inc.
 * Copyright (C) Valentin V. Bartenev
 */


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _NGX_THREAD_POOL_H_INCLUDED_
#ವ್ಯಾಖ್ಯಾನಿಸು _NGX_THREAD_POOL_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>
#ಅನ್ತರ್ಭಾವ <ngx_event.h>


ರಚನೆ ngx_thread_task_s {
    ngx_thread_task_t   *ಮುಂದಿನ;
    ngx_uint_t           id;
    ಶೂನ್ಯ                *ctx;
    ಶೂನ್ಯ               (*handler)(ಶೂನ್ಯ *ದತ್ತಾಂಶ, ngx_log_t *ಲಘುಗಣಕ);
    ngx_event_t          event;
};


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ngx_thread_pool_s  ngx_thread_pool_t;


ngx_thread_pool_t *ngx_thread_pool_ಕೂಡಿಸು(ngx_conf_t *cf, ngx_str_t *name);
ngx_thread_pool_t *ngx_thread_pool_get(ngx_ಚಕ್ರ_t *ಚಕ್ರ, ngx_str_t *name);

ngx_thread_task_t *ngx_thread_task_alloc(ngx_pool_t *pool, ಗಾತ್ರ_t ಗಾತ್ರ_2);
ngx_int_t ngx_thread_task_post(ngx_thread_pool_t *tp, ngx_thread_task_t *task);


#ಆದರೆ_ಅಂತ್ಯ /* _NGX_THREAD_POOL_H_INCLUDED_ */
