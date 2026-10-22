
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#నిర్వచితం_కాకపోతే _NGX_RBTREE_H_INCLUDED_
#నిర్వచించు _NGX_RBTREE_H_INCLUDED_


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


రకనిర్వచనం ngx_uint_t  ngx_rbtree_కీలకం_t;
రకనిర్వచనం ngx_int_t   ngx_rbtree_కీలకం_int_t;


రకనిర్వచనం నిర్మాణం ngx_rbtree_కణుపు_s  ngx_rbtree_కణుపు_t;

నిర్మాణం ngx_rbtree_కణుపు_s {
    ngx_rbtree_కీలకం_t       కీలకం;
    ngx_rbtree_కణుపు_t     *ఎడమ;
    ngx_rbtree_కణుపు_t     *కుడి;
    ngx_rbtree_కణుపు_t     *మాతృ;
    u_char                 color;
    u_char                 దత్తాంశం;
};


రకనిర్వచనం నిర్మాణం ngx_rbtree_s  ngx_rbtree_t;

రకనిర్వచనం శూన్య (*ngx_rbtree_చొప్పించు_pt) (ngx_rbtree_కణుపు_t *మూలం,
    ngx_rbtree_కణుపు_t *కణుపు, ngx_rbtree_కణుపు_t *sentinel);

నిర్మాణం ngx_rbtree_s {
    ngx_rbtree_కణుపు_t     *మూలం;
    ngx_rbtree_కణుపు_t     *sentinel;
    ngx_rbtree_చొప్పించు_pt   చొప్పించు;
};


#నిర్వచించు ngx_rbtree_init(వృక్షం, s, i)                                           \
    ngx_rbtree_sentinel_init(s);                                              \
    (వృక్షం)->మూలం = s;                                                         \
    (వృక్షం)->sentinel = s;                                                     \
    (వృక్షం)->చొప్పించు = i

#నిర్వచించు ngx_rbtree_దత్తాంశం(కణుపు, type, link)                                     \
    (type *) ((u_char *) (కణుపు) - offsetof(type, link))


శూన్య ngx_rbtree_చొప్పించు(ngx_rbtree_t *వృక్షం, ngx_rbtree_కణుపు_t *కణుపు);
శూన్య ngx_rbtree_delete(ngx_rbtree_t *వృక్షం, ngx_rbtree_కణుపు_t *కణుపు);
శూన్య ngx_rbtree_చొప్పించు_విలువ(ngx_rbtree_కణుపు_t *మూలం, ngx_rbtree_కణుపు_t *కణుపు,
    ngx_rbtree_కణుపు_t *sentinel);
శూన్య ngx_rbtree_చొప్పించు_timer_విలువ(ngx_rbtree_కణుపు_t *మూలం,
    ngx_rbtree_కణుపు_t *కణుపు, ngx_rbtree_కణుపు_t *sentinel);
ngx_rbtree_కణుపు_t *ngx_rbtree_తదుపరి(ngx_rbtree_t *వృక్షం,
    ngx_rbtree_కణుపు_t *కణుపు);


#నిర్వచించు ngx_rbt_red(కణుపు)               ((కణుపు)->color = 1)
#నిర్వచించు ngx_rbt_black(కణుపు)             ((కణుపు)->color = 0)
#నిర్వచించు ngx_rbt_is_red(కణుపు)            ((కణుపు)->color)
#నిర్వచించు ngx_rbt_is_black(కణుపు)          (!ngx_rbt_is_red(కణుపు))
#నిర్వచించు ngx_rbt_copy_color(n1, n2)      (n1->color = n2->color)


/* a sentinel must be black */

#నిర్వచించు ngx_rbtree_sentinel_init(కణుపు)  ngx_rbt_black(కణుపు)


స్థిర ngx_inline ngx_rbtree_కణుపు_t *
ngx_rbtree_min(ngx_rbtree_కణుపు_t *కణుపు, ngx_rbtree_కణుపు_t *sentinel)
{
    అయితే (కణుపు->ఎడమ != sentinel) {
        కణుపు = కణుపు->ఎడమ;
    }

    తిరిగి కణుపు;
}


#అయితే_ముగింపు /* _NGX_RBTREE_H_INCLUDED_ */
