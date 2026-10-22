
/*
 * Copyright (C) Nginx, Inc.
 * Copyright (C) Valentin V. Bartenev
 */


#నిర్వచితం_కాకపోతే _NGX_THREAD_POOL_H_INCLUDED_
#నిర్వచించు _NGX_THREAD_POOL_H_INCLUDED_


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>
#అన్తర్భావ <ngx_event.h>


నిర్మాణం ngx_thread_task_s {
    ngx_thread_task_t   *తదుపరి;
    ngx_uint_t           id;
    శూన్య                *ctx;
    శూన్య               (*handler)(శూన్య *దత్తాంశం, ngx_log_t *సంవర్గమానం);
    ngx_event_t          event;
};


రకనిర్వచనం నిర్మాణం ngx_thread_pool_s  ngx_thread_pool_t;


ngx_thread_pool_t *ngx_thread_pool_కూడు(ngx_conf_t *cf, ngx_str_t *name);
ngx_thread_pool_t *ngx_thread_pool_get(ngx_చక్రం_t *చక్రం, ngx_str_t *name);

ngx_thread_task_t *ngx_thread_task_alloc(ngx_pool_t *pool, కొలత_t కొలత);
ngx_int_t ngx_thread_task_post(ngx_thread_pool_t *tp, ngx_thread_task_t *task);


#అయితే_ముగింపు /* _NGX_THREAD_POOL_H_INCLUDED_ */
