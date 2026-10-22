
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_RBTREE_H_INCLUDED_
#define _NGX_RBTREE_H_INCLUDED_


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>


வகைவரை ngx_uint_t  ngx_rbtree_சாவி_t;
வகைவரை ngx_int_t   ngx_rbtree_சாவி_int_t;


வகைவரை கட்டமைப்பு ngx_rbtree_முனை_s  ngx_rbtree_முனை_t;

கட்டமைப்பு ngx_rbtree_முனை_s {
    ngx_rbtree_சாவி_t       சாவி;
    ngx_rbtree_முனை_t     *இடது;
    ngx_rbtree_முனை_t     *வலது;
    ngx_rbtree_முனை_t     *parent;
    u_char                 color;
    u_char                 data;
};


வகைவரை கட்டமைப்பு ngx_rbtree_s  ngx_rbtree_t;

வகைவரை வெறுமை (*ngx_rbtree_செருகு_pt) (ngx_rbtree_முனை_t *வேர்,
    ngx_rbtree_முனை_t *முனை, ngx_rbtree_முனை_t *sentinel);

கட்டமைப்பு ngx_rbtree_s {
    ngx_rbtree_முனை_t     *வேர்;
    ngx_rbtree_முனை_t     *sentinel;
    ngx_rbtree_செருகு_pt   செருகு;
};


#define ngx_rbtree_init(மரம், s, i)                                           \
    ngx_rbtree_sentinel_init(s);                                              \
    (மரம்)->வேர் = s;                                                         \
    (மரம்)->sentinel = s;                                                     \
    (மரம்)->செருகு = i

#define ngx_rbtree_data(முனை, type, link)                                     \
    (type *) ((u_char *) (முனை) - offsetof(type, link))


வெறுமை ngx_rbtree_செருகு(ngx_rbtree_t *மரம், ngx_rbtree_முனை_t *முனை);
வெறுமை ngx_rbtree_delete(ngx_rbtree_t *மரம், ngx_rbtree_முனை_t *முனை);
வெறுமை ngx_rbtree_செருகு_மதிப்பு(ngx_rbtree_முனை_t *வேர், ngx_rbtree_முனை_t *முனை,
    ngx_rbtree_முனை_t *sentinel);
வெறுமை ngx_rbtree_செருகு_timer_மதிப்பு(ngx_rbtree_முனை_t *வேர்,
    ngx_rbtree_முனை_t *முனை, ngx_rbtree_முனை_t *sentinel);
ngx_rbtree_முனை_t *ngx_rbtree_next(ngx_rbtree_t *மரம்,
    ngx_rbtree_முனை_t *முனை);


#define ngx_rbt_red(முனை)               ((முனை)->color = 1)
#define ngx_rbt_black(முனை)             ((முனை)->color = 0)
#define ngx_rbt_is_red(முனை)            ((முனை)->color)
#define ngx_rbt_is_black(முனை)          (!ngx_rbt_is_red(முனை))
#define ngx_rbt_copy_color(n1, n2)      (n1->color = n2->color)


/* a sentinel must be black */

#define ngx_rbtree_sentinel_init(முனை)  ngx_rbt_black(முனை)


நிலையான ngx_inline ngx_rbtree_முனை_t *
ngx_rbtree_min(ngx_rbtree_முனை_t *முனை, ngx_rbtree_முனை_t *sentinel)
{
    போது (முனை->இடது != sentinel) {
        முனை = முனை->இடது;
    }

    திரும்பு முனை;
}


#endif /* _NGX_RBTREE_H_INCLUDED_ */
