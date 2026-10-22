
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _NGX_RBTREE_H_INCLUDED_
#ವ್ಯಾಖ್ಯಾನಿಸು _NGX_RBTREE_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ngx_uint_t  ngx_rbtree_ಕೀಲಿ_t;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ngx_int_t   ngx_rbtree_ಕೀಲಿ_int_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ngx_rbtree_ಗಂಟು_s  ngx_rbtree_ಗಂಟು_t;

ರಚನೆ ngx_rbtree_ಗಂಟು_s {
    ngx_rbtree_ಕೀಲಿ_t       ಕೀಲಿ;
    ngx_rbtree_ಗಂಟು_t     *ಎಡ;
    ngx_rbtree_ಗಂಟು_t     *ಬಲ;
    ngx_rbtree_ಗಂಟು_t     *ಪೋಷಕ;
    u_char                 color;
    u_char                 ದತ್ತಾಂಶ;
};


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ngx_rbtree_s  ngx_rbtree_t;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*ngx_rbtree_ಸೇರಿಸು_pt) (ngx_rbtree_ಗಂಟು_t *ಬೇರು,
    ngx_rbtree_ಗಂಟು_t *ಗಂಟು, ngx_rbtree_ಗಂಟು_t *sentinel);

ರಚನೆ ngx_rbtree_s {
    ngx_rbtree_ಗಂಟು_t     *ಬೇರು;
    ngx_rbtree_ಗಂಟು_t     *sentinel;
    ngx_rbtree_ಸೇರಿಸು_pt   ಸೇರಿಸು;
};


#ವ್ಯಾಖ್ಯಾನಿಸು ngx_rbtree_init(ವೃಕ್ಷ, s, i)                                           \
    ngx_rbtree_sentinel_init(s);                                              \
    (ವೃಕ್ಷ)->ಬೇರು = s;                                                         \
    (ವೃಕ್ಷ)->sentinel = s;                                                     \
    (ವೃಕ್ಷ)->ಸೇರಿಸು = i

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_rbtree_ದತ್ತಾಂಶ(ಗಂಟು, type, link)                                     \
    (type *) ((u_char *) (ಗಂಟು) - offsetof(type, link))


ಶೂನ್ಯ ngx_rbtree_ಸೇರಿಸು(ngx_rbtree_t *ವೃಕ್ಷ, ngx_rbtree_ಗಂಟು_t *ಗಂಟು);
ಶೂನ್ಯ ngx_rbtree_delete(ngx_rbtree_t *ವೃಕ್ಷ, ngx_rbtree_ಗಂಟು_t *ಗಂಟು);
ಶೂನ್ಯ ngx_rbtree_ಸೇರಿಸು_ಮೌಲ್ಯ(ngx_rbtree_ಗಂಟು_t *ಬೇರು, ngx_rbtree_ಗಂಟು_t *ಗಂಟು,
    ngx_rbtree_ಗಂಟು_t *sentinel);
ಶೂನ್ಯ ngx_rbtree_ಸೇರಿಸು_timer_ಮೌಲ್ಯ(ngx_rbtree_ಗಂಟು_t *ಬೇರು,
    ngx_rbtree_ಗಂಟು_t *ಗಂಟು, ngx_rbtree_ಗಂಟು_t *sentinel);
ngx_rbtree_ಗಂಟು_t *ngx_rbtree_ಮುಂದಿನ(ngx_rbtree_t *ವೃಕ್ಷ,
    ngx_rbtree_ಗಂಟು_t *ಗಂಟು);


#ವ್ಯಾಖ್ಯಾನಿಸು ngx_rbt_red(ಗಂಟು)               ((ಗಂಟು)->color = 1)
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_rbt_black(ಗಂಟು)             ((ಗಂಟು)->color = 0)
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_rbt_is_red(ಗಂಟು)            ((ಗಂಟು)->color)
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_rbt_is_black(ಗಂಟು)          (!ngx_rbt_is_red(ಗಂಟು))
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_rbt_copy_color(n1, n2)      (n1->color = n2->color)


/* a sentinel must be black */

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_rbtree_sentinel_init(ಗಂಟು)  ngx_rbt_black(ಗಂಟು)


ಸ್ಥಿರ ngx_inline ngx_rbtree_ಗಂಟು_t *
ngx_rbtree_min(ngx_rbtree_ಗಂಟು_t *ಗಂಟು, ngx_rbtree_ಗಂಟು_t *sentinel)
{
    ಆಗ (ಗಂಟು->ಎಡ != sentinel) {
        ಗಂಟು = ಗಂಟು->ಎಡ;
    }

    ಹಿಂತಿರುಗಿ ಗಂಟು;
}


#ಆದರೆ_ಅಂತ್ಯ /* _NGX_RBTREE_H_INCLUDED_ */
