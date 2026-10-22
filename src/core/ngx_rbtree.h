
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_RBTREE_H_INCLUDED_
#define _NGX_RBTREE_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع ngx_uint_t  ngx_rbtree_ئاچقۇچ_t;
تعريفنوع ngx_int_t   ngx_rbtree_ئاچقۇچ_int_t;


تعريفنوع بنية ngx_rbtree_تۈگۈن_s  ngx_rbtree_تۈگۈن_t;

بنية ngx_rbtree_تۈگۈن_s {
    ngx_rbtree_ئاچقۇچ_t       ئاچقۇچ;
    ngx_rbtree_تۈگۈن_t     *سول;
    ngx_rbtree_تۈگۈن_t     *ئوڭ;
    ngx_rbtree_تۈگۈن_t     *parent;
    u_char                 color;
    u_char                 data;
};


تعريفنوع بنية ngx_rbtree_s  ngx_rbtree_t;

تعريفنوع فراغ (*ngx_rbtree_قىستۇرۇش_pt) (ngx_rbtree_تۈگۈن_t *يىلتىز,
    ngx_rbtree_تۈگۈن_t *تۈگۈن, ngx_rbtree_تۈگۈن_t *sentinel);

بنية ngx_rbtree_s {
    ngx_rbtree_تۈگۈن_t     *يىلتىز;
    ngx_rbtree_تۈگۈن_t     *sentinel;
    ngx_rbtree_قىستۇرۇش_pt   قىستۇرۇش;
};


#define ngx_rbtree_init(دەرەخ, s, i)                                           \
    ngx_rbtree_sentinel_init(s);                                              \
    (دەرەخ)->يىلتىز = s;                                                         \
    (دەرەخ)->sentinel = s;                                                     \
    (دەرەخ)->قىستۇرۇش = i

#define ngx_rbtree_data(تۈگۈن, type, link)                                     \
    (type *) ((u_char *) (تۈگۈن) - offsetof(type, link))


فراغ ngx_rbtree_قىستۇرۇش(ngx_rbtree_t *دەرەخ, ngx_rbtree_تۈگۈن_t *تۈگۈن);
فراغ ngx_rbtree_delete(ngx_rbtree_t *دەرەخ, ngx_rbtree_تۈگۈن_t *تۈگۈن);
فراغ ngx_rbtree_قىستۇرۇش_قىممەت(ngx_rbtree_تۈگۈن_t *يىلتىز, ngx_rbtree_تۈگۈن_t *تۈگۈن,
    ngx_rbtree_تۈگۈن_t *sentinel);
فراغ ngx_rbtree_قىستۇرۇش_timer_قىممەت(ngx_rbtree_تۈگۈن_t *يىلتىز,
    ngx_rbtree_تۈگۈن_t *تۈگۈن, ngx_rbtree_تۈگۈن_t *sentinel);
ngx_rbtree_تۈگۈن_t *ngx_rbtree_next(ngx_rbtree_t *دەرەخ,
    ngx_rbtree_تۈگۈن_t *تۈگۈن);


#define ngx_rbt_red(تۈگۈن)               ((تۈگۈن)->color = 1)
#define ngx_rbt_black(تۈگۈن)             ((تۈگۈن)->color = 0)
#define ngx_rbt_is_red(تۈگۈن)            ((تۈگۈن)->color)
#define ngx_rbt_is_black(تۈگۈن)          (!ngx_rbt_is_red(تۈگۈن))
#define ngx_rbt_copy_color(n1, n2)      (n1->color = n2->color)


/* a sentinel must be black */

#define ngx_rbtree_sentinel_init(تۈگۈن)  ngx_rbt_black(تۈگۈن)


ساكن ngx_inline ngx_rbtree_تۈگۈن_t *
ngx_rbtree_min(ngx_rbtree_تۈگۈن_t *تۈگۈن, ngx_rbtree_تۈگۈن_t *sentinel)
{
    طالما (تۈگۈن->سول != sentinel) {
        تۈگۈن = تۈگۈن->سول;
    }

    أرجع تۈگۈن;
}


#endif /* _NGX_RBTREE_H_INCLUDED_ */
