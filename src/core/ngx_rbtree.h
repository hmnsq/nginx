
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_RBTREE_H_INCLUDED_
#define _NGX_RBTREE_H_INCLUDED_


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>


തരനിർവചനം ngx_uint_t  ngx_rbtree_താക്കോൽ_t;
തരനിർവചനം ngx_int_t   ngx_rbtree_താക്കോൽ_int_t;


തരനിർവചനം ഘടന ngx_rbtree_കണ്ണി_s  ngx_rbtree_കണ്ണി_t;

ഘടന ngx_rbtree_കണ്ണി_s {
    ngx_rbtree_താക്കോൽ_t       താക്കോൽ;
    ngx_rbtree_കണ്ണി_t     *ഇടത്;
    ngx_rbtree_കണ്ണി_t     *വലത്;
    ngx_rbtree_കണ്ണി_t     *parent;
    u_char                 color;
    u_char                 data;
};


തരനിർവചനം ഘടന ngx_rbtree_s  ngx_rbtree_t;

തരനിർവചനം ശൂന്യം (*ngx_rbtree_തിരുകുക_pt) (ngx_rbtree_കണ്ണി_t *വേര്,
    ngx_rbtree_കണ്ണി_t *കണ്ണി, ngx_rbtree_കണ്ണി_t *sentinel);

ഘടന ngx_rbtree_s {
    ngx_rbtree_കണ്ണി_t     *വേര്;
    ngx_rbtree_കണ്ണി_t     *sentinel;
    ngx_rbtree_തിരുകുക_pt   തിരുകുക;
};


#define ngx_rbtree_init(വൃക്ഷം, s, i)                                           \
    ngx_rbtree_sentinel_init(s);                                              \
    (വൃക്ഷം)->വേര് = s;                                                         \
    (വൃക്ഷം)->sentinel = s;                                                     \
    (വൃക്ഷം)->തിരുകുക = i

#define ngx_rbtree_data(കണ്ണി, type, link)                                     \
    (type *) ((u_char *) (കണ്ണി) - offsetof(type, link))


ശൂന്യം ngx_rbtree_തിരുകുക(ngx_rbtree_t *വൃക്ഷം, ngx_rbtree_കണ്ണി_t *കണ്ണി);
ശൂന്യം ngx_rbtree_delete(ngx_rbtree_t *വൃക്ഷം, ngx_rbtree_കണ്ണി_t *കണ്ണി);
ശൂന്യം ngx_rbtree_തിരുകുക_മൂല്യം(ngx_rbtree_കണ്ണി_t *വേര്, ngx_rbtree_കണ്ണി_t *കണ്ണി,
    ngx_rbtree_കണ്ണി_t *sentinel);
ശൂന്യം ngx_rbtree_തിരുകുക_timer_മൂല്യം(ngx_rbtree_കണ്ണി_t *വേര്,
    ngx_rbtree_കണ്ണി_t *കണ്ണി, ngx_rbtree_കണ്ണി_t *sentinel);
ngx_rbtree_കണ്ണി_t *ngx_rbtree_next(ngx_rbtree_t *വൃക്ഷം,
    ngx_rbtree_കണ്ണി_t *കണ്ണി);


#define ngx_rbt_red(കണ്ണി)               ((കണ്ണി)->color = 1)
#define ngx_rbt_black(കണ്ണി)             ((കണ്ണി)->color = 0)
#define ngx_rbt_is_red(കണ്ണി)            ((കണ്ണി)->color)
#define ngx_rbt_is_black(കണ്ണി)          (!ngx_rbt_is_red(കണ്ണി))
#define ngx_rbt_copy_color(n1, n2)      (n1->color = n2->color)


/* a sentinel must be black */

#define ngx_rbtree_sentinel_init(കണ്ണി)  ngx_rbt_black(കണ്ണി)


സ്ഥിതം ngx_inline ngx_rbtree_കണ്ണി_t *
ngx_rbtree_min(ngx_rbtree_കണ്ണി_t *കണ്ണി, ngx_rbtree_കണ്ണി_t *sentinel)
{
    ആകുമ്പോൾ (കണ്ണി->ഇടത് != sentinel) {
        കണ്ണി = കണ്ണി->ഇടത്;
    }

    തിരികെ കണ്ണി;
}


#endif /* _NGX_RBTREE_H_INCLUDED_ */
