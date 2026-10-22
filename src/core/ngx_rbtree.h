
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_RBTREE_H_INCLUDED_
#define _NGX_RBTREE_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


typedef ngx_uint_t  ngx_rbtree_सुसि_t;
typedef ngx_int_t   ngx_rbtree_सुसि_int_t;


typedef struct ngx_rbtree_बुहोल_s  ngx_rbtree_बुहोल_t;

struct ngx_rbtree_बुहोल_s {
    ngx_rbtree_सुसि_t       सुसि;
    ngx_rbtree_बुहोल_t     *कालिवा;
    ngx_rbtree_बुहोल_t     *कानान;
    ngx_rbtree_बुहोल_t     *parent;
    u_char                 color;
    u_char                 data;
};


typedef struct ngx_rbtree_s  ngx_rbtree_t;

typedef void (*ngx_rbtree_इसिंगित_pt) (ngx_rbtree_बुहोल_t *उगात,
    ngx_rbtree_बुहोल_t *बुहोल, ngx_rbtree_बुहोल_t *sentinel);

struct ngx_rbtree_s {
    ngx_rbtree_बुहोल_t     *उगात;
    ngx_rbtree_बुहोल_t     *sentinel;
    ngx_rbtree_इसिंगित_pt   इसिंगित;
};


#define ngx_rbtree_init(पुनो, s, i)                                           \
    ngx_rbtree_sentinel_init(s);                                              \
    (पुनो)->उगात = s;                                                         \
    (पुनो)->sentinel = s;                                                     \
    (पुनो)->इसिंगित = i

#define ngx_rbtree_data(बुहोल, type, link)                                     \
    (type *) ((u_char *) (बुहोल) - offsetof(type, link))


void ngx_rbtree_इसिंगित(ngx_rbtree_t *पुनो, ngx_rbtree_बुहोल_t *बुहोल);
void ngx_rbtree_delete(ngx_rbtree_t *पुनो, ngx_rbtree_बुहोल_t *बुहोल);
void ngx_rbtree_इसिंगित_हालागा(ngx_rbtree_बुहोल_t *उगात, ngx_rbtree_बुहोल_t *बुहोल,
    ngx_rbtree_बुहोल_t *sentinel);
void ngx_rbtree_इसिंगित_timer_हालागा(ngx_rbtree_बुहोल_t *उगात,
    ngx_rbtree_बुहोल_t *बुहोल, ngx_rbtree_बुहोल_t *sentinel);
ngx_rbtree_बुहोल_t *ngx_rbtree_next(ngx_rbtree_t *पुनो,
    ngx_rbtree_बुहोल_t *बुहोल);


#define ngx_rbt_red(बुहोल)               ((बुहोल)->color = 1)
#define ngx_rbt_black(बुहोल)             ((बुहोल)->color = 0)
#define ngx_rbt_is_red(बुहोल)            ((बुहोल)->color)
#define ngx_rbt_is_black(बुहोल)          (!ngx_rbt_is_red(बुहोल))
#define ngx_rbt_copy_color(n1, n2)      (n1->color = n2->color)


/* a sentinel must be black */

#define ngx_rbtree_sentinel_init(बुहोल)  ngx_rbt_black(बुहोल)


static ngx_inline ngx_rbtree_बुहोल_t *
ngx_rbtree_min(ngx_rbtree_बुहोल_t *बुहोल, ngx_rbtree_बुहोल_t *sentinel)
{
    while (बुहोल->कालिवा != sentinel) {
        बुहोल = बुहोल->कालिवा;
    }

    return बुहोल;
}


#endif /* _NGX_RBTREE_H_INCLUDED_ */
