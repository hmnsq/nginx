
/*
 * Copyright (C) Nginx, Inc.
 * Copyright (C) Valentin V. Bartenev
 */


#यदि_अपरिभाषितम् _NGX_THREAD_POOL_H_INCLUDED_
#परिभाषय _NGX_THREAD_POOL_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>
#अन्तर्भाव <ngx_event.h>


संरचना ngx_thread_task_s {
    ngx_thread_task_t   *अग्र;
    ngx_uint_t           id;
    शून्य                *ctx;
    शून्य               (*handler)(शून्य *दत्त, ngx_log_t *लघुगणकः);
    ngx_event_t          event;
};


प्रकारनिर्देश संरचना ngx_thread_pool_s  ngx_thread_pool_t;


ngx_thread_pool_t *ngx_thread_pool_सङ्कलय(ngx_conf_t *cf, ngx_str_t *name);
ngx_thread_pool_t *ngx_thread_pool_get(ngx_आवृत्तिः_t *आवृत्तिः, ngx_str_t *name);

ngx_thread_task_t *ngx_thread_task_alloc(ngx_pool_t *pool, आकार_t आकार);
ngx_int_t ngx_thread_task_post(ngx_thread_pool_t *tp, ngx_thread_task_t *task);


#यदि_अन्तः /* _NGX_THREAD_POOL_H_INCLUDED_ */
