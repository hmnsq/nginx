
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_غیر_تعریف_شدہ _NGX_RBTREE_H_INCLUDED_
#تعریف_کرو _NGX_RBTREE_H_INCLUDED_


#شامل <ngx_config.h>
#شامل <ngx_core.h>


قسمتعریف ngx_uint_t  ngx_rbtree_کلید_t;
قسمتعریف ngx_int_t   ngx_rbtree_کلید_int_t;


قسمتعریف ساخت ngx_rbtree_گرہ_s  ngx_rbtree_گرہ_t;

ساخت ngx_rbtree_گرہ_s {
    ngx_rbtree_کلید_t       کلید;
    ngx_rbtree_گرہ_t     *بایاں;
    ngx_rbtree_گرہ_t     *دایاں;
    ngx_rbtree_گرہ_t     *والد;
    u_char                 color;
    u_char                 ڈیٹا;
};


قسمتعریف ساخت ngx_rbtree_s  ngx_rbtree_t;

قسمتعریف خالی (*ngx_rbtree_داخل_کرو_pt) (ngx_rbtree_گرہ_t *جڑ,
    ngx_rbtree_گرہ_t *گرہ, ngx_rbtree_گرہ_t *sentinel);

ساخت ngx_rbtree_s {
    ngx_rbtree_گرہ_t     *جڑ;
    ngx_rbtree_گرہ_t     *sentinel;
    ngx_rbtree_داخل_کرو_pt   داخل_کرو;
};


#تعریف_کرو ngx_rbtree_init(درخت, s, i)                                           \
    ngx_rbtree_sentinel_init(s);                                              \
    (درخت)->جڑ = s;                                                         \
    (درخت)->sentinel = s;                                                     \
    (درخت)->داخل_کرو = i

#تعریف_کرو ngx_rbtree_ڈیٹا(گرہ, type, link)                                     \
    (type *) ((u_char *) (گرہ) - offsetof(type, link))


خالی ngx_rbtree_داخل_کرو(ngx_rbtree_t *درخت, ngx_rbtree_گرہ_t *گرہ);
خالی ngx_rbtree_delete(ngx_rbtree_t *درخت, ngx_rbtree_گرہ_t *گرہ);
خالی ngx_rbtree_داخل_کرو_قدر(ngx_rbtree_گرہ_t *جڑ, ngx_rbtree_گرہ_t *گرہ,
    ngx_rbtree_گرہ_t *sentinel);
خالی ngx_rbtree_داخل_کرو_timer_قدر(ngx_rbtree_گرہ_t *جڑ,
    ngx_rbtree_گرہ_t *گرہ, ngx_rbtree_گرہ_t *sentinel);
ngx_rbtree_گرہ_t *ngx_rbtree_اگلا(ngx_rbtree_t *درخت,
    ngx_rbtree_گرہ_t *گرہ);


#تعریف_کرو ngx_rbt_red(گرہ)               ((گرہ)->color = 1)
#تعریف_کرو ngx_rbt_black(گرہ)             ((گرہ)->color = 0)
#تعریف_کرو ngx_rbt_is_red(گرہ)            ((گرہ)->color)
#تعریف_کرو ngx_rbt_is_black(گرہ)          (!ngx_rbt_is_red(گرہ))
#تعریف_کرو ngx_rbt_copy_color(n1, n2)      (n1->color = n2->color)


/* a sentinel must be black */

#تعریف_کرو ngx_rbtree_sentinel_init(گرہ)  ngx_rbt_black(گرہ)


ساکن ngx_inline ngx_rbtree_گرہ_t *
ngx_rbtree_min(ngx_rbtree_گرہ_t *گرہ, ngx_rbtree_گرہ_t *sentinel)
{
    جب (گرہ->بایاں != sentinel) {
        گرہ = گرہ->بایاں;
    }

    واپس گرہ;
}


#اگر_ختم /* _NGX_RBTREE_H_INCLUDED_ */
