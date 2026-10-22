
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_RBTREE_H_INCLUDED_
#define _NGX_RBTREE_H_INCLUDED_


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


ପ୍ରକାରବ୍ୟାଖ୍ୟା ngx_uint_t  ngx_rbtree_ଚାବି_t;
ପ୍ରକାରବ୍ୟାଖ୍ୟା ngx_int_t   ngx_rbtree_ଚାବି_int_t;


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା ngx_rbtree_ଗଣ୍ଠି_s  ngx_rbtree_ଗଣ୍ଠି_t;

ସଂରଚନା ngx_rbtree_ଗଣ୍ଠି_s {
    ngx_rbtree_ଚାବି_t       ଚାବି;
    ngx_rbtree_ଗଣ୍ଠି_t     *ବାମ;
    ngx_rbtree_ଗଣ୍ଠି_t     *ଡାହାଣ;
    ngx_rbtree_ଗଣ୍ଠି_t     *parent;
    u_char                 color;
    u_char                 data;
};


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା ngx_rbtree_s  ngx_rbtree_t;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*ngx_rbtree_ଭର୍ତ୍ତି_pt) (ngx_rbtree_ଗଣ୍ଠି_t *ମୂଳ,
    ngx_rbtree_ଗଣ୍ଠି_t *ଗଣ୍ଠି, ngx_rbtree_ଗଣ୍ଠି_t *sentinel);

ସଂରଚନା ngx_rbtree_s {
    ngx_rbtree_ଗଣ୍ଠି_t     *ମୂଳ;
    ngx_rbtree_ଗଣ୍ଠି_t     *sentinel;
    ngx_rbtree_ଭର୍ତ୍ତି_pt   ଭର୍ତ୍ତି;
};


#define ngx_rbtree_init(ବୃକ୍ଷ, s, i)                                           \
    ngx_rbtree_sentinel_init(s);                                              \
    (ବୃକ୍ଷ)->ମୂଳ = s;                                                         \
    (ବୃକ୍ଷ)->sentinel = s;                                                     \
    (ବୃକ୍ଷ)->ଭର୍ତ୍ତି = i

#define ngx_rbtree_data(ଗଣ୍ଠି, type, link)                                     \
    (type *) ((u_char *) (ଗଣ୍ଠି) - offsetof(type, link))


ଶୂନ୍ୟ ngx_rbtree_ଭର୍ତ୍ତି(ngx_rbtree_t *ବୃକ୍ଷ, ngx_rbtree_ଗଣ୍ଠି_t *ଗଣ୍ଠି);
ଶୂନ୍ୟ ngx_rbtree_delete(ngx_rbtree_t *ବୃକ୍ଷ, ngx_rbtree_ଗଣ୍ଠି_t *ଗଣ୍ଠି);
ଶୂନ୍ୟ ngx_rbtree_ଭର୍ତ୍ତି_ମୂଲ୍ୟ(ngx_rbtree_ଗଣ୍ଠି_t *ମୂଳ, ngx_rbtree_ଗଣ୍ଠି_t *ଗଣ୍ଠି,
    ngx_rbtree_ଗଣ୍ଠି_t *sentinel);
ଶୂନ୍ୟ ngx_rbtree_ଭର୍ତ୍ତି_timer_ମୂଲ୍ୟ(ngx_rbtree_ଗଣ୍ଠି_t *ମୂଳ,
    ngx_rbtree_ଗଣ୍ଠି_t *ଗଣ୍ଠି, ngx_rbtree_ଗଣ୍ଠି_t *sentinel);
ngx_rbtree_ଗଣ୍ଠି_t *ngx_rbtree_next(ngx_rbtree_t *ବୃକ୍ଷ,
    ngx_rbtree_ଗଣ୍ଠି_t *ଗଣ୍ଠି);


#define ngx_rbt_red(ଗଣ୍ଠି)               ((ଗଣ୍ଠି)->color = 1)
#define ngx_rbt_black(ଗଣ୍ଠି)             ((ଗଣ୍ଠି)->color = 0)
#define ngx_rbt_is_red(ଗଣ୍ଠି)            ((ଗଣ୍ଠି)->color)
#define ngx_rbt_is_black(ଗଣ୍ଠି)          (!ngx_rbt_is_red(ଗଣ୍ଠି))
#define ngx_rbt_copy_color(n1, n2)      (n1->color = n2->color)


/* a sentinel must be black */

#define ngx_rbtree_sentinel_init(ଗଣ୍ଠି)  ngx_rbt_black(ଗଣ୍ଠି)


ସ୍ଥିର ngx_inline ngx_rbtree_ଗଣ୍ଠି_t *
ngx_rbtree_min(ngx_rbtree_ଗଣ୍ଠି_t *ଗଣ୍ଠି, ngx_rbtree_ଗଣ୍ଠି_t *sentinel)
{
    ଯେତେବେଳେ (ଗଣ୍ଠି->ବାମ != sentinel) {
        ଗଣ୍ଠି = ଗଣ୍ଠି->ବାମ;
    }

    ଫେରନ୍ତୁ ଗଣ୍ଠି;
}


#endif /* _NGX_RBTREE_H_INCLUDED_ */
