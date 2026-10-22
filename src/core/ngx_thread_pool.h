
/*
 * Copyright (C) Nginx, Inc.
 * Copyright (C) Valentin V. Bartenev
 */


#ifndef _NGX_THREAD_POOL_H_INCLUDED_
#define _NGX_THREAD_POOL_H_INCLUDED_


#অন্তর্ভাৱ <ngx_config.h>
#অন্তর্ভাৱ <ngx_core.h>
#অন্তর্ভাৱ <ngx_event.h>


সংরচনা ngx_thread_task_s {
    ngx_thread_task_t   *next;
    ngx_uint_t           id;
    শূন্য                *ctx;
    শূন্য               (*handler)(শূন্য *data, ngx_log_t *log);
    ngx_event_t          event;
};


প্রকারনির্দেশ সংরচনা ngx_thread_pool_s  ngx_thread_pool_t;


ngx_thread_pool_t *ngx_thread_pool_add(ngx_conf_t *cf, ngx_str_t *name);
ngx_thread_pool_t *ngx_thread_pool_get(ngx_cycle_t *cycle, ngx_str_t *name);

ngx_thread_task_t *ngx_thread_task_alloc(ngx_pool_t *pool, size_t size);
ngx_int_t ngx_thread_task_post(ngx_thread_pool_t *tp, ngx_thread_task_t *task);


#endif /* _NGX_THREAD_POOL_H_INCLUDED_ */
