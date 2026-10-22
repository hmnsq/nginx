
/*
 * Copyright (C) Nginx, Inc.
 * Copyright (C) Valentin V. Bartenev
 */


#جيڪڏهن_وصف_نه_ٿيل _NGX_THREAD_POOL_H_INCLUDED_
#وصف_ڪريو _NGX_THREAD_POOL_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <ngx_event.h>


بنية ngx_thread_task_s {
    ngx_thread_task_t   *ايندڙ;
    ngx_uint_t           id;
    فراغ                *ctx;
    فراغ               (*handler)(فراغ *معلومات, ngx_log_t *لاگرٿم);
    ngx_event_t          event;
};


تعريفنوع بنية ngx_thread_pool_s  ngx_thread_pool_t;


ngx_thread_pool_t *ngx_thread_pool_جوڙيو(ngx_conf_t *cf, ngx_str_t *name);
ngx_thread_pool_t *ngx_thread_pool_get(ngx_چڪر_t *چڪر, ngx_str_t *name);

ngx_thread_task_t *ngx_thread_task_alloc(ngx_pool_t *pool, سائيز_t سائيز);
ngx_int_t ngx_thread_task_post(ngx_thread_pool_t *tp, ngx_thread_task_t *task);


#جيڪڏهن_پورو /* _NGX_THREAD_POOL_H_INCLUDED_ */
