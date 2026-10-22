
/*
 * Copyright (C) Nginx, Inc.
 * Copyright (C) Valentin V. Bartenev
 */


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _NGX_THREAD_POOL_H_INCLUDED_
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _NGX_THREAD_POOL_H_INCLUDED_


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_event.h>


ਬਣਤਰ ngx_thread_task_s {
    ngx_thread_task_t   *ਅਗਲਾ;
    ngx_uint_t           id;
    ਖਾਲੀ                *ctx;
    ਖਾਲੀ               (*handler)(ਖਾਲੀ *ਅੰਕੜੇ, ngx_log_t *ਲਘੂਗਣਕ);
    ngx_event_t          event;
};


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_thread_pool_s  ngx_thread_pool_t;


ngx_thread_pool_t *ngx_thread_pool_ਜੋੜੋ(ngx_conf_t *cf, ngx_str_t *name);
ngx_thread_pool_t *ngx_thread_pool_get(ngx_ਚੱਕਰ_t *ਚੱਕਰ, ngx_str_t *name);

ngx_thread_task_t *ngx_thread_task_alloc(ngx_pool_t *pool, ਮਾਪ_t ਮਾਪ);
ngx_int_t ngx_thread_task_post(ngx_thread_pool_t *tp, ngx_thread_task_t *task);


#ਜੇ_ਅੰਤ /* _NGX_THREAD_POOL_H_INCLUDED_ */
