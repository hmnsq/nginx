
/*
 * Copyright (C) Nginx, Inc.
 * Copyright (C) Valentin V. Bartenev
 */


#جے_بنا_تعریف _NGX_THREAD_POOL_H_INCLUDED_
#تعریف_کرو _NGX_THREAD_POOL_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <ngx_event.h>


بنية ngx_thread_task_s {
    ngx_thread_task_t   *اگلا;
    ngx_uint_t           id;
    فراغ                *ctx;
    فراغ               (*handler)(فراغ *انکڑے, ngx_log_t *لوگارتھم);
    ngx_event_t          event;
};


تعريفنوع بنية ngx_thread_pool_s  ngx_thread_pool_t;


ngx_thread_pool_t *ngx_thread_pool_جوڑو(ngx_conf_t *cf, ngx_str_t *name);
ngx_thread_pool_t *ngx_thread_pool_get(ngx_چکر_t *چکر, ngx_str_t *name);

ngx_thread_task_t *ngx_thread_task_alloc(ngx_pool_t *pool, ناپ_t ناپ);
ngx_int_t ngx_thread_task_post(ngx_thread_pool_t *tp, ngx_thread_task_t *task);


#جے_انت /* _NGX_THREAD_POOL_H_INCLUDED_ */
