
/*
 * Copyright (C) Nginx, Inc.
 * Copyright (C) Valentin V. Bartenev
 */


#که_نه_تعریف_شوی _NGX_THREAD_POOL_H_INCLUDED_
#تعریف_کړه _NGX_THREAD_POOL_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <ngx_event.h>


بنية ngx_thread_task_s {
    ngx_thread_task_t   *بل;
    ngx_uint_t           id;
    فراغ                *ctx;
    فراغ               (*handler)(فراغ *معلومات, ngx_log_t *لوګاریتم);
    ngx_event_t          event;
};


تعريفنوع بنية ngx_thread_pool_s  ngx_thread_pool_t;


ngx_thread_pool_t *ngx_thread_pool_جمع_کړه(ngx_conf_t *cf, ngx_str_t *name);
ngx_thread_pool_t *ngx_thread_pool_get(ngx_دوره_t *دوره, ngx_str_t *name);

ngx_thread_task_t *ngx_thread_task_alloc(ngx_pool_t *pool, اندازه_t اندازه);
ngx_int_t ngx_thread_task_post(ngx_thread_pool_t *tp, ngx_thread_task_t *task);


#د_که_پای /* _NGX_THREAD_POOL_H_INCLUDED_ */
