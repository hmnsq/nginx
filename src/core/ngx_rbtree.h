
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_RBTREE_H_INCLUDED_
#define _NGX_RBTREE_H_INCLUDED_


#অন্তর্ভাৱ <ngx_config.h>
#অন্তর্ভাৱ <ngx_core.h>


প্রকারনির্দেশ ngx_uint_t  ngx_rbtree_চাবি_t;
প্রকারনির্দেশ ngx_int_t   ngx_rbtree_চাবি_int_t;


প্রকারনির্দেশ সংরচনা ngx_rbtree_গ্ৰন্থি_s  ngx_rbtree_গ্ৰন্থি_t;

সংরচনা ngx_rbtree_গ্ৰন্থি_s {
    ngx_rbtree_চাবি_t       চাবি;
    ngx_rbtree_গ্ৰন্থি_t     *বাম;
    ngx_rbtree_গ্ৰন্থি_t     *ডান;
    ngx_rbtree_গ্ৰন্থি_t     *parent;
    u_char                 color;
    u_char                 data;
};


প্রকারনির্দেশ সংরচনা ngx_rbtree_s  ngx_rbtree_t;

প্রকারনির্দেশ শূন্য (*ngx_rbtree_ঢোকাও_pt) (ngx_rbtree_গ্ৰন্থি_t *মূল,
    ngx_rbtree_গ্ৰন্থি_t *গ্ৰন্থি, ngx_rbtree_গ্ৰন্থি_t *sentinel);

সংরচনা ngx_rbtree_s {
    ngx_rbtree_গ্ৰন্থি_t     *মূল;
    ngx_rbtree_গ্ৰন্থি_t     *sentinel;
    ngx_rbtree_ঢোকাও_pt   ঢোকাও;
};


#define ngx_rbtree_init(বৃক্ষ, s, i)                                           \
    ngx_rbtree_sentinel_init(s);                                              \
    (বৃক্ষ)->মূল = s;                                                         \
    (বৃক্ষ)->sentinel = s;                                                     \
    (বৃক্ষ)->ঢোকাও = i

#define ngx_rbtree_data(গ্ৰন্থি, type, link)                                     \
    (type *) ((u_char *) (গ্ৰন্থি) - offsetof(type, link))


শূন্য ngx_rbtree_ঢোকাও(ngx_rbtree_t *বৃক্ষ, ngx_rbtree_গ্ৰন্থি_t *গ্ৰন্থি);
শূন্য ngx_rbtree_delete(ngx_rbtree_t *বৃক্ষ, ngx_rbtree_গ্ৰন্থি_t *গ্ৰন্থি);
শূন্য ngx_rbtree_ঢোকাও_মান(ngx_rbtree_গ্ৰন্থি_t *মূল, ngx_rbtree_গ্ৰন্থি_t *গ্ৰন্থি,
    ngx_rbtree_গ্ৰন্থি_t *sentinel);
শূন্য ngx_rbtree_ঢোকাও_timer_মান(ngx_rbtree_গ্ৰন্থি_t *মূল,
    ngx_rbtree_গ্ৰন্থি_t *গ্ৰন্থি, ngx_rbtree_গ্ৰন্থি_t *sentinel);
ngx_rbtree_গ্ৰন্থি_t *ngx_rbtree_next(ngx_rbtree_t *বৃক্ষ,
    ngx_rbtree_গ্ৰন্থি_t *গ্ৰন্থি);


#define ngx_rbt_red(গ্ৰন্থি)               ((গ্ৰন্থি)->color = 1)
#define ngx_rbt_black(গ্ৰন্থি)             ((গ্ৰন্থি)->color = 0)
#define ngx_rbt_is_red(গ্ৰন্থি)            ((গ্ৰন্থি)->color)
#define ngx_rbt_is_black(গ্ৰন্থি)          (!ngx_rbt_is_red(গ্ৰন্থি))
#define ngx_rbt_copy_color(n1, n2)      (n1->color = n2->color)


/* a sentinel must be black */

#define ngx_rbtree_sentinel_init(গ্ৰন্থি)  ngx_rbt_black(গ্ৰন্থি)


স্থির ngx_inline ngx_rbtree_গ্ৰন্থি_t *
ngx_rbtree_min(ngx_rbtree_গ্ৰন্থি_t *গ্ৰন্থি, ngx_rbtree_গ্ৰন্থি_t *sentinel)
{
    যেতিয়ালৈ (গ্ৰন্থি->বাম != sentinel) {
        গ্ৰন্থি = গ্ৰন্থি->বাম;
    }

    ঘূৰাই গ্ৰন্থি;
}


#endif /* _NGX_RBTREE_H_INCLUDED_ */
