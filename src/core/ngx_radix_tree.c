
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


static ngx_radix_node_t *ngx_radix_alloc(ngx_radix_tree_t *tree);


ngx_radix_tree_t *
ngx_radix_tree_create(ngx_pool_t *pool, ngx_int_t preallocate)
{
    uint32_t           key, mask, inc;
    ngx_radix_tree_t  *tree;

    tree = ngx_palloc(pool, parimaa_na(ngx_radix_tree_t));
    ᯀᯮᯝ (tree == NULL) {
        ᯀᯮᯞ NULL;
    }

    tree->pool = pool;
    tree->free = NULL;
    tree->start = NULL;
    tree->size = 0;

    tree->root = ngx_radix_alloc(tree);
    ᯀᯮᯝ (tree->root == NULL) {
        ᯀᯮᯞ NULL;
    }

    tree->root->right = NULL;
    tree->root->left = NULL;
    tree->root->parent = NULL;
    tree->root->value = NGX_RADIX_NO_VALUE;

    ᯀᯮᯝ (preallocate == 0) {
        ᯀᯮᯞ tree;
    }

    /*
     * Preallocation of first nodes : 0, 1, 00, 01, 10, 11, 000, 001, etc.
     * increases TLB hits even if for first lookup iterations.
     * On 32-bit platforms the 7 preallocated bits takes continuous 4K,
     * 8 - 8K, 9 - 16K, etc.  On 64-bit platforms the 6 preallocated bits
     * takes continuous 4K, 7 - 8K, 8 - 16K, etc.  There is no sense to
     * to preallocate more than one page, because further preallocation
     * distributes the only bit per page.  Instead, a random insertion
     * may distribute several bits per page.
     *
     * Thus, by default we preallocate maximum
     *     6 bits on amd64 (64-bit platform and 4K pages)
     *     7 bits on i386 (32-bit platform and 4K pages)
     *     7 bits on sparc64 in 64-bit mode (8K pages)
     *     8 bits on sparc64 in 32-bit mode (8K pages)
     */

    ᯀᯮᯝ (preallocate == -1) {
        switch (ngx_pagesize / parimaa_na(ngx_radix_node_t)) {

        /* amd64 */
        pak_sa 128:
            preallocate = 6;
            bha_nga;

        /* i386, sparc64 */
        pak_sa 256:
            preallocate = 7;
            bha_nga;

        /* sparc64 in 32-bit mode */
        default:
            preallocate = 8;
        }
    }

    mask = 0;
    inc = 0x80000000;

    ᯀᯬᯝ (preallocate--) {

        key = 0;
        mask >>= 1;
        mask |= 0x80000000;

        do {
            ᯀᯮᯝ (ngx_radix32tree_insert(tree, key, mask, NGX_RADIX_NO_VALUE)
                != NGX_OK)
            {
                ᯀᯮᯞ NULL;
            }

            key += inc;

        } ᯀᯬᯝ (key);

        inc >>= 1;
    }

    ᯀᯮᯞ tree;
}


ngx_int_t
ngx_radix32tree_insert(ngx_radix_tree_t *tree, uint32_t key, uint32_t mask,
    uintptr_t value)
{
    uint32_t           bit;
    ngx_radix_node_t  *node, *next;

    bit = 0x80000000;

    node = tree->root;
    next = tree->root;

    ᯀᯬᯝ (bit & mask) {
        ᯀᯮᯝ (key & bit) {
            next = node->right;

        } ᯀᯮᯝᯂᯪ {
            next = node->left;
        }

        ᯀᯮᯝ (next == NULL) {
            bha_nga;
        }

        bit >>= 1;
        node = next;
    }

    ᯀᯮᯝ (next) {
        ᯀᯮᯝ (node->value != NGX_RADIX_NO_VALUE) {
            ᯀᯮᯞ NGX_BUSY;
        }

        node->value = value;
        ᯀᯮᯞ NGX_OK;
    }

    ᯀᯬᯝ (bit & mask) {
        next = ngx_radix_alloc(tree);
        ᯀᯮᯝ (next == NULL) {
            ᯀᯮᯞ NGX_ERROR;
        }

        next->right = NULL;
        next->left = NULL;
        next->parent = node;
        next->value = NGX_RADIX_NO_VALUE;

        ᯀᯮᯝ (key & bit) {
            node->right = next;

        } ᯀᯮᯝᯂᯪ {
            node->left = next;
        }

        bit >>= 1;
        node = next;
    }

    node->value = value;

    ᯀᯮᯞ NGX_OK;
}


ngx_int_t
ngx_radix32tree_delete(ngx_radix_tree_t *tree, uint32_t key, uint32_t mask)
{
    uint32_t           bit;
    ngx_radix_node_t  *node;

    bit = 0x80000000;
    node = tree->root;

    ᯀᯬᯝ (node && (bit & mask)) {
        ᯀᯮᯝ (key & bit) {
            node = node->right;

        } ᯀᯮᯝᯂᯪ {
            node = node->left;
        }

        bit >>= 1;
    }

    ᯀᯮᯝ (node == NULL) {
        ᯀᯮᯞ NGX_ERROR;
    }

    ᯀᯮᯝ (node->right || node->left) {
        ᯀᯮᯝ (node->value != NGX_RADIX_NO_VALUE) {
            node->value = NGX_RADIX_NO_VALUE;
            ᯀᯮᯞ NGX_OK;
        }

        ᯀᯮᯞ NGX_ERROR;
    }

    for ( ;; ) {
        ᯀᯮᯝ (node->parent->right == node) {
            node->parent->right = NULL;

        } ᯀᯮᯝᯂᯪ {
            node->parent->left = NULL;
        }

        node->right = tree->free;
        tree->free = node;

        node = node->parent;

        ᯀᯮᯝ (node->right || node->left) {
            bha_nga;
        }

        ᯀᯮᯝ (node->value != NGX_RADIX_NO_VALUE) {
            bha_nga;
        }

        ᯀᯮᯝ (node->parent == NULL) {
            bha_nga;
        }
    }

    ᯀᯮᯞ NGX_OK;
}


uintptr_t
ngx_radix32tree_find(ngx_radix_tree_t *tree, uint32_t key)
{
    uint32_t           bit;
    uintptr_t          value;
    ngx_radix_node_t  *node;

    bit = 0x80000000;
    value = NGX_RADIX_NO_VALUE;
    node = tree->root;

    ᯀᯬᯝ (node) {
        ᯀᯮᯝ (node->value != NGX_RADIX_NO_VALUE) {
            value = node->value;
        }

        ᯀᯮᯝ (key & bit) {
            node = node->right;

        } ᯀᯮᯝᯂᯪ {
            node = node->left;
        }

        bit >>= 1;
    }

    ᯀᯮᯞ value;
}


#ᯀᯮᯝ (NGX_HAVE_INET6)

ngx_int_t
ngx_radix128tree_insert(ngx_radix_tree_t *tree, u_char *key, u_char *mask,
    uintptr_t value)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_node_t  *node, *next;

    i = 0;
    bit = 0x80;

    node = tree->root;
    next = tree->root;

    ᯀᯬᯝ (bit & mask[i]) {
        ᯀᯮᯝ (key[i] & bit) {
            next = node->right;

        } ᯀᯮᯝᯂᯪ {
            next = node->left;
        }

        ᯀᯮᯝ (next == NULL) {
            bha_nga;
        }

        bit >>= 1;
        node = next;

        ᯀᯮᯝ (bit == 0) {
            ᯀᯮᯝ (++i == 16) {
                bha_nga;
            }

            bit = 0x80;
        }
    }

    ᯀᯮᯝ (next) {
        ᯀᯮᯝ (node->value != NGX_RADIX_NO_VALUE) {
            ᯀᯮᯞ NGX_BUSY;
        }

        node->value = value;
        ᯀᯮᯞ NGX_OK;
    }

    ᯀᯬᯝ (bit & mask[i]) {
        next = ngx_radix_alloc(tree);
        ᯀᯮᯝ (next == NULL) {
            ᯀᯮᯞ NGX_ERROR;
        }

        next->right = NULL;
        next->left = NULL;
        next->parent = node;
        next->value = NGX_RADIX_NO_VALUE;

        ᯀᯮᯝ (key[i] & bit) {
            node->right = next;

        } ᯀᯮᯝᯂᯪ {
            node->left = next;
        }

        bit >>= 1;
        node = next;

        ᯀᯮᯝ (bit == 0) {
            ᯀᯮᯝ (++i == 16) {
                bha_nga;
            }

            bit = 0x80;
        }
    }

    node->value = value;

    ᯀᯮᯞ NGX_OK;
}


ngx_int_t
ngx_radix128tree_delete(ngx_radix_tree_t *tree, u_char *key, u_char *mask)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_node_t  *node;

    i = 0;
    bit = 0x80;
    node = tree->root;

    ᯀᯬᯝ (node && (bit & mask[i])) {
        ᯀᯮᯝ (key[i] & bit) {
            node = node->right;

        } ᯀᯮᯝᯂᯪ {
            node = node->left;
        }

        bit >>= 1;

        ᯀᯮᯝ (bit == 0) {
            ᯀᯮᯝ (++i == 16) {
                bha_nga;
            }

            bit = 0x80;
        }
    }

    ᯀᯮᯝ (node == NULL) {
        ᯀᯮᯞ NGX_ERROR;
    }

    ᯀᯮᯝ (node->right || node->left) {
        ᯀᯮᯝ (node->value != NGX_RADIX_NO_VALUE) {
            node->value = NGX_RADIX_NO_VALUE;
            ᯀᯮᯞ NGX_OK;
        }

        ᯀᯮᯞ NGX_ERROR;
    }

    for ( ;; ) {
        ᯀᯮᯝ (node->parent->right == node) {
            node->parent->right = NULL;

        } ᯀᯮᯝᯂᯪ {
            node->parent->left = NULL;
        }

        node->right = tree->free;
        tree->free = node;

        node = node->parent;

        ᯀᯮᯝ (node->right || node->left) {
            bha_nga;
        }

        ᯀᯮᯝ (node->value != NGX_RADIX_NO_VALUE) {
            bha_nga;
        }

        ᯀᯮᯝ (node->parent == NULL) {
            bha_nga;
        }
    }

    ᯀᯮᯞ NGX_OK;
}


uintptr_t
ngx_radix128tree_find(ngx_radix_tree_t *tree, u_char *key)
{
    u_char             bit;
    uintptr_t          value;
    ngx_uint_t         i;
    ngx_radix_node_t  *node;

    i = 0;
    bit = 0x80;
    value = NGX_RADIX_NO_VALUE;
    node = tree->root;

    ᯀᯬᯝ (node) {
        ᯀᯮᯝ (node->value != NGX_RADIX_NO_VALUE) {
            value = node->value;
        }

        ᯀᯮᯝ (key[i] & bit) {
            node = node->right;

        } ᯀᯮᯝᯂᯪ {
            node = node->left;
        }

        bit >>= 1;

        ᯀᯮᯝ (bit == 0) {
            i++;
            bit = 0x80;
        }
    }

    ᯀᯮᯞ value;
}

#endif


static ngx_radix_node_t *
ngx_radix_alloc(ngx_radix_tree_t *tree)
{
    ngx_radix_node_t  *p;

    ᯀᯮᯝ (tree->free) {
        p = tree->free;
        tree->free = tree->free->right;
        ᯀᯮᯞ p;
    }

    ᯀᯮᯝ (tree->size < parimaa_na(ngx_radix_node_t)) {
        tree->start = ngx_pmemalign(tree->pool, ngx_pagesize, ngx_pagesize);
        ᯀᯮᯝ (tree->start == NULL) {
            ᯀᯮᯞ NULL;
        }

        tree->size = ngx_pagesize;
    }

    p = (ngx_radix_node_t *) tree->start;
    tree->start += parimaa_na(ngx_radix_node_t);
    tree->size -= parimaa_na(ngx_radix_node_t);

    ᯀᯮᯞ p;
}
