
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#నిర్వచితం_కాకపోతే NGX_RADIX_వృక్షం_H_INCLUDED
#నిర్వచించు NGX_RADIX_వృక్షం_H_INCLUDED


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


#నిర్వచించు NGX_RADIX_NO_విలువ   (uintptr_t) -1

రకనిర్వచనం నిర్మాణం ngx_radix_కణుపు_s  ngx_radix_కణుపు_t;

నిర్మాణం ngx_radix_కణుపు_s {
    ngx_radix_కణుపు_t  *కుడి;
    ngx_radix_కణుపు_t  *ఎడమ;
    ngx_radix_కణుపు_t  *మాతృ;
    uintptr_t          విలువ;
};


రకనిర్వచనం నిర్మాణం {
    ngx_radix_కణుపు_t  *మూలం;
    ngx_pool_t        *pool;
    ngx_radix_కణుపు_t  *విడుదల;
    అక్షరం              *ప్రారంభం;
    కొలత_t             కొలత;
} ngx_radix_వృక్షం_t;


ngx_radix_వృక్షం_t *ngx_radix_వృక్షం_create(ngx_pool_t *pool,
    ngx_int_t preallocate);

ngx_int_t ngx_radix32tree_చొప్పించు(ngx_radix_వృక్షం_t *వృక్షం,
    uint32_t కీలకం, uint32_t ముసుగు, uintptr_t విలువ);
ngx_int_t ngx_radix32tree_delete(ngx_radix_వృక్షం_t *వృక్షం,
    uint32_t కీలకం, uint32_t ముసుగు);
uintptr_t ngx_radix32tree_కనుగొను(ngx_radix_వృక్షం_t *వృక్షం, uint32_t కీలకం);

#ఒకవేళ (NGX_HAVE_INET6)
ngx_int_t ngx_radix128tree_చొప్పించు(ngx_radix_వృక్షం_t *వృక్షం,
    u_char *కీలకం, u_char *ముసుగు, uintptr_t విలువ);
ngx_int_t ngx_radix128tree_delete(ngx_radix_వృక్షం_t *వృక్షం,
    u_char *కీలకం, u_char *ముసుగు);
uintptr_t ngx_radix128tree_కనుగొను(ngx_radix_వృక్షం_t *వృక్షం, u_char *కీలకం);
#అయితే_ముగింపు


#అయితే_ముగింపు /* _NGX_RADIX_TREE_H_INCLUDED_ */
