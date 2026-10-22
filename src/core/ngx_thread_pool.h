
/*
 * Copyright (C) Nginx, Inc.
 * Copyright (C) Valentin V. Bartenev
 */


#अगर_अघोषित _NGX_THREAD_POOL_H_INCLUDED_
#घोषणा _NGX_THREAD_POOL_H_INCLUDED_


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>
#समावेश <ngx_event.h>


काष्ठा ngx_thread_task_s {
    ngx_thread_task_t   *अग्र;
    ngx_uint_t           id;
    व्योम                *ctx;
    व्योम               (*handler)(व्योम *तथ्य, ngx_log_t *log);
    ngx_event_t          event;
};


प्रकार काष्ठा ngx_thread_pool_s  ngx_thread_pool_t;


ngx_thread_pool_t *ngx_thread_pool_add(ngx_conf_t *cf, ngx_str_t *name);
ngx_thread_pool_t *ngx_thread_pool_get(ngx_cycle_t *cycle, ngx_str_t *name);

ngx_thread_task_t *ngx_thread_task_alloc(ngx_pool_t *pool, माप_प्रकार आकार);
ngx_int_t ngx_thread_task_post(ngx_thread_pool_t *tp, ngx_thread_task_t *task);


#पूर्ण_अगर /* _NGX_THREAD_POOL_H_INCLUDED_ */
