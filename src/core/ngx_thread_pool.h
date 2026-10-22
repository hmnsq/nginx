
/*
 * Copyright (C) Nginx, Inc.
 * Copyright (C) Valentin V. Bartenev
 */


#اگر_غیر_تعریف_شدہ _NGX_THREAD_POOL_H_INCLUDED_
#تعریف_کرو _NGX_THREAD_POOL_H_INCLUDED_


#شامل <ngx_config.h>
#شامل <ngx_core.h>
#شامل <ngx_event.h>


ساخت ngx_thread_task_s {
    ngx_thread_task_t   *اگلا;
    ngx_uint_t           id;
    خالی                *ctx;
    خالی               (*handler)(خالی *ڈیٹا, ngx_log_t *لوگارتھم);
    ngx_event_t          event;
};


قسمتعریف ساخت ngx_thread_pool_s  ngx_thread_pool_t;


ngx_thread_pool_t *ngx_thread_pool_جوڑو(ngx_conf_t *cf, ngx_str_t *name);
ngx_thread_pool_t *ngx_thread_pool_get(ngx_چکر_t *چکر, ngx_str_t *name);

ngx_thread_task_t *ngx_thread_task_alloc(ngx_pool_t *pool, سائز_t سائز);
ngx_int_t ngx_thread_task_post(ngx_thread_pool_t *tp, ngx_thread_task_t *task);


#اگر_ختم /* _NGX_THREAD_POOL_H_INCLUDED_ */
