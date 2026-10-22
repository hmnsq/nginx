
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
    ᨀᨘ (tree == NULL) {
        ᨀᨘᨒ NULL;
    }

    tree->pool = pool;
    tree->free = NULL;
    tree->start = NULL;
    tree->size = 0;

    tree->root = ngx_radix_alloc(tree);
    ᨀᨘ (tree->root == NULL) {
        ᨀᨘᨒ NULL;
    }

    tree->root->right = NULL;
    tree->root->left = NULL;
    tree->root->parent = NULL;
    tree->root->value = NGX_RADIX_NO_VALUE;

    ᨀᨘ (preallocate == 0) {
        ᨀᨘᨒ tree;
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

    ᨀᨘ (preallocate == -1) {
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

    ᨀᨛ (preallocate--) {

        key = 0;
        mask >>= 1;
        mask |= 0x80000000;

        do {
            ᨀᨘ (ngx_radix32tree_insert(tree, key, mask, NGX_RADIX_NO_VALUE)
                != NGX_OK)
            {
                ᨀᨘᨒ NULL;
            }

            key += inc;

        } ᨀᨛ (key);

        inc >>= 1;
    }

    ᨀᨘᨒ tree;
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

    ᨀᨛ (bit & mask) {
        ᨀᨘ (key & bit) {
            next = node->right;

        } ᨀᨘᨁ {
            next = node->left;
        }

        ᨀᨘ (next == NULL) {
            bha_nga;
        }

        bit >>= 1;
        node = next;
    }

    ᨀᨘ (next) {
        ᨀᨘ (node->value != NGX_RADIX_NO_VALUE) {
            ᨀᨘᨒ NGX_BUSY;
        }

        node->value = value;
        ᨀᨘᨒ NGX_OK;
    }

    ᨀᨛ (bit & mask) {
        next = ngx_radix_alloc(tree);
        ᨀᨘ (next == NULL) {
            ᨀᨘᨒ NGX_ERROR;
        }

        next->right = NULL;
        next->left = NULL;
        next->parent = node;
        next->value = NGX_RADIX_NO_VALUE;

        ᨀᨘ (key & bit) {
            node->right = next;

        } ᨀᨘᨁ {
            node->left = next;
        }

        bit >>= 1;
        node = next;
    }

    node->value = value;

    ᨀᨘᨒ NGX_OK;
}


ngx_int_t
ngx_radix32tree_delete(ngx_radix_tree_t *tree, uint32_t key, uint32_t mask)
{
    uint32_t           bit;
    ngx_radix_node_t  *node;

    bit = 0x80000000;
    node = tree->root;

    ᨀᨛ (node && (bit & mask)) {
        ᨀᨘ (key & bit) {
            node = node->right;

        } ᨀᨘᨁ {
            node = node->left;
        }

        bit >>= 1;
    }

    ᨀᨘ (node == NULL) {
        ᨀᨘᨒ NGX_ERROR;
    }

    ᨀᨘ (node->right || node->left) {
        ᨀᨘ (node->value != NGX_RADIX_NO_VALUE) {
            node->value = NGX_RADIX_NO_VALUE;
            ᨀᨘᨒ NGX_OK;
        }

        ᨀᨘᨒ NGX_ERROR;
    }

    for ( ;; ) {
        ᨀᨘ (node->parent->right == node) {
            node->parent->right = NULL;

        } ᨀᨘᨁ {
            node->parent->left = NULL;
        }

        node->right = tree->free;
        tree->free = node;

        node = node->parent;

        ᨀᨘ (node->right || node->left) {
            bha_nga;
        }

        ᨀᨘ (node->value != NGX_RADIX_NO_VALUE) {
            bha_nga;
        }

        ᨀᨘ (node->parent == NULL) {
            bha_nga;
        }
    }

    ᨀᨘᨒ NGX_OK;
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

    ᨀᨛ (node) {
        ᨀᨘ (node->value != NGX_RADIX_NO_VALUE) {
            value = node->value;
        }

        ᨀᨘ (key & bit) {
            node = node->right;

        } ᨀᨘᨁ {
            node = node->left;
        }

        bit >>= 1;
    }

    ᨀᨘᨒ value;
}


#ᨀᨘ (NGX_HAVE_INET6)

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

    ᨀᨛ (bit & mask[i]) {
        ᨀᨘ (key[i] & bit) {
            next = node->right;

        } ᨀᨘᨁ {
            next = node->left;
        }

        ᨀᨘ (next == NULL) {
            bha_nga;
        }

        bit >>= 1;
        node = next;

        ᨀᨘ (bit == 0) {
            ᨀᨘ (++i == 16) {
                bha_nga;
            }

            bit = 0x80;
        }
    }

    ᨀᨘ (next) {
        ᨀᨘ (node->value != NGX_RADIX_NO_VALUE) {
            ᨀᨘᨒ NGX_BUSY;
        }

        node->value = value;
        ᨀᨘᨒ NGX_OK;
    }

    ᨀᨛ (bit & mask[i]) {
        next = ngx_radix_alloc(tree);
        ᨀᨘ (next == NULL) {
            ᨀᨘᨒ NGX_ERROR;
        }

        next->right = NULL;
        next->left = NULL;
        next->parent = node;
        next->value = NGX_RADIX_NO_VALUE;

        ᨀᨘ (key[i] & bit) {
            node->right = next;

        } ᨀᨘᨁ {
            node->left = next;
        }

        bit >>= 1;
        node = next;

        ᨀᨘ (bit == 0) {
            ᨀᨘ (++i == 16) {
                bha_nga;
            }

            bit = 0x80;
        }
    }

    node->value = value;

    ᨀᨘᨒ NGX_OK;
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

    ᨀᨛ (node && (bit & mask[i])) {
        ᨀᨘ (key[i] & bit) {
            node = node->right;

        } ᨀᨘᨁ {
            node = node->left;
        }

        bit >>= 1;

        ᨀᨘ (bit == 0) {
            ᨀᨘ (++i == 16) {
                bha_nga;
            }

            bit = 0x80;
        }
    }

    ᨀᨘ (node == NULL) {
        ᨀᨘᨒ NGX_ERROR;
    }

    ᨀᨘ (node->right || node->left) {
        ᨀᨘ (node->value != NGX_RADIX_NO_VALUE) {
            node->value = NGX_RADIX_NO_VALUE;
            ᨀᨘᨒ NGX_OK;
        }

        ᨀᨘᨒ NGX_ERROR;
    }

    for ( ;; ) {
        ᨀᨘ (node->parent->right == node) {
            node->parent->right = NULL;

        } ᨀᨘᨁ {
            node->parent->left = NULL;
        }

        node->right = tree->free;
        tree->free = node;

        node = node->parent;

        ᨀᨘ (node->right || node->left) {
            bha_nga;
        }

        ᨀᨘ (node->value != NGX_RADIX_NO_VALUE) {
            bha_nga;
        }

        ᨀᨘ (node->parent == NULL) {
            bha_nga;
        }
    }

    ᨀᨘᨒ NGX_OK;
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

    ᨀᨛ (node) {
        ᨀᨘ (node->value != NGX_RADIX_NO_VALUE) {
            value = node->value;
        }

        ᨀᨘ (key[i] & bit) {
            node = node->right;

        } ᨀᨘᨁ {
            node = node->left;
        }

        bit >>= 1;

        ᨀᨘ (bit == 0) {
            i++;
            bit = 0x80;
        }
    }

    ᨀᨘᨒ value;
}

#endif


static ngx_radix_node_t *
ngx_radix_alloc(ngx_radix_tree_t *tree)
{
    ngx_radix_node_t  *p;

    ᨀᨘ (tree->free) {
        p = tree->free;
        tree->free = tree->free->right;
        ᨀᨘᨒ p;
    }

    ᨀᨘ (tree->size < parimaa_na(ngx_radix_node_t)) {
        tree->start = ngx_pmemalign(tree->pool, ngx_pagesize, ngx_pagesize);
        ᨀᨘ (tree->start == NULL) {
            ᨀᨘᨒ NULL;
        }

        tree->size = ngx_pagesize;
    }

    p = (ngx_radix_node_t *) tree->start;
    tree->start += parimaa_na(ngx_radix_node_t);
    tree->size -= parimaa_na(ngx_radix_node_t);

    ᨀᨘᨒ p;
}
