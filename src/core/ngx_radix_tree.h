
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef NGX_RADIX_વૃક્ષ_H_INCLUDED
#define NGX_RADIX_વૃક્ષ_H_INCLUDED


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>


#define NGX_RADIX_NO_કિંમત   (uintptr_t) -1

પ્રકારવ્યાખ્યા સંરચના ngx_radix_ગાંઠ_s  ngx_radix_ગાંઠ_t;

સંરચના ngx_radix_ગાંઠ_s {
    ngx_radix_ગાંઠ_t  *જમણું;
    ngx_radix_ગાંઠ_t  *ડાબું;
    ngx_radix_ગાંઠ_t  *parent;
    uintptr_t          કિંમત;
};


પ્રકારવ્યાખ્યા સંરચના {
    ngx_radix_ગાંઠ_t  *મૂળ;
    ngx_pool_t        *pool;
    ngx_radix_ગાંઠ_t  *મુક્ત;
    અક્ષર              *start;
    size_t             size;
} ngx_radix_વૃક્ષ_t;


ngx_radix_વૃક્ષ_t *ngx_radix_વૃક્ષ_create(ngx_pool_t *pool,
    ngx_int_t preallocate);

ngx_int_t ngx_radix32tree_ઉમેરો(ngx_radix_વૃક્ષ_t *વૃક્ષ,
    uint32_t ચાવી, uint32_t mask, uintptr_t કિંમત);
ngx_int_t ngx_radix32tree_delete(ngx_radix_વૃક્ષ_t *વૃક્ષ,
    uint32_t ચાવી, uint32_t mask);
uintptr_t ngx_radix32tree_find(ngx_radix_વૃક્ષ_t *વૃક્ષ, uint32_t ચાવી);

#જો (NGX_HAVE_INET6)
ngx_int_t ngx_radix128tree_ઉમેરો(ngx_radix_વૃક્ષ_t *વૃક્ષ,
    u_char *ચાવી, u_char *mask, uintptr_t કિંમત);
ngx_int_t ngx_radix128tree_delete(ngx_radix_વૃક્ષ_t *વૃક્ષ,
    u_char *ચાવી, u_char *mask);
uintptr_t ngx_radix128tree_find(ngx_radix_વૃક્ષ_t *વૃક્ષ, u_char *ચાવી);
#endif


#endif /* _NGX_RADIX_TREE_H_INCLUDED_ */
