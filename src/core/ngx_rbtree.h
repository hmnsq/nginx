
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#که_نه_تعریف_شوی _NGX_RBTREE_H_INCLUDED_
#تعریف_کړه _NGX_RBTREE_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع ngx_uint_t  ngx_rbtree_کلی_t;
تعريفنوع ngx_int_t   ngx_rbtree_کلی_int_t;


تعريفنوع بنية ngx_rbtree_غوټه_s  ngx_rbtree_غوټه_t;

بنية ngx_rbtree_غوټه_s {
    ngx_rbtree_کلی_t       کلی;
    ngx_rbtree_غوټه_t     *کیڼ;
    ngx_rbtree_غوټه_t     *ښی;
    ngx_rbtree_غوټه_t     *پلار;
    u_char                 color;
    u_char                 معلومات;
};


تعريفنوع بنية ngx_rbtree_s  ngx_rbtree_t;

تعريفنوع فراغ (*ngx_rbtree_ورننباسه_pt) (ngx_rbtree_غوټه_t *ریښه,
    ngx_rbtree_غوټه_t *غوټه, ngx_rbtree_غوټه_t *sentinel);

بنية ngx_rbtree_s {
    ngx_rbtree_غوټه_t     *ریښه;
    ngx_rbtree_غوټه_t     *sentinel;
    ngx_rbtree_ورننباسه_pt   ورننباسه;
};


#تعریف_کړه ngx_rbtree_init(ونه, s, i)                                           \
    ngx_rbtree_sentinel_init(s);                                              \
    (ونه)->ریښه = s;                                                         \
    (ونه)->sentinel = s;                                                     \
    (ونه)->ورننباسه = i

#تعریف_کړه ngx_rbtree_معلومات(غوټه, type, link)                                     \
    (type *) ((u_char *) (غوټه) - offsetof(type, link))


فراغ ngx_rbtree_ورننباسه(ngx_rbtree_t *ونه, ngx_rbtree_غوټه_t *غوټه);
فراغ ngx_rbtree_delete(ngx_rbtree_t *ونه, ngx_rbtree_غوټه_t *غوټه);
فراغ ngx_rbtree_ورننباسه_ارزښت(ngx_rbtree_غوټه_t *ریښه, ngx_rbtree_غوټه_t *غوټه,
    ngx_rbtree_غوټه_t *sentinel);
فراغ ngx_rbtree_ورننباسه_timer_ارزښت(ngx_rbtree_غوټه_t *ریښه,
    ngx_rbtree_غوټه_t *غوټه, ngx_rbtree_غوټه_t *sentinel);
ngx_rbtree_غوټه_t *ngx_rbtree_بل(ngx_rbtree_t *ونه,
    ngx_rbtree_غوټه_t *غوټه);


#تعریف_کړه ngx_rbt_red(غوټه)               ((غوټه)->color = 1)
#تعریف_کړه ngx_rbt_black(غوټه)             ((غوټه)->color = 0)
#تعریف_کړه ngx_rbt_is_red(غوټه)            ((غوټه)->color)
#تعریف_کړه ngx_rbt_is_black(غوټه)          (!ngx_rbt_is_red(غوټه))
#تعریف_کړه ngx_rbt_copy_color(n1, n2)      (n1->color = n2->color)


/* a sentinel must be black */

#تعریف_کړه ngx_rbtree_sentinel_init(غوټه)  ngx_rbt_black(غوټه)


ساكن ngx_inline ngx_rbtree_غوټه_t *
ngx_rbtree_min(ngx_rbtree_غوټه_t *غوټه, ngx_rbtree_غوټه_t *sentinel)
{
    طالما (غوټه->کیڼ != sentinel) {
        غوټه = غوټه->کیڼ;
    }

    أرجع غوټه;
}


#د_که_پای /* _NGX_RBTREE_H_INCLUDED_ */
