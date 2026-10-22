
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

    tree = ngx_palloc(pool, sizeof(ngx_radix_tree_t));
    nếu (tree == NULL) {
        trả_về NULL;
    }

    tree->pool = pool;
    tree->free = NULL;
    tree->start = NULL;
    tree->size = 0;

    tree->root = ngx_radix_alloc(tree);
    nếu (tree->root == NULL) {
        trả_về NULL;
    }

    tree->root->right = NULL;
    tree->root->left = NULL;
    tree->root->parent = NULL;
    tree->root->value = NGX_RADIX_NO_VALUE;

    nếu (preallocate == 0) {
        trả_về tree;
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

    nếu (preallocate == -1) {
        switch (ngx_pagesize / sizeof(ngx_radix_node_t)) {

        /* amd64 */
        case 128:
            preallocate = 6;
            break;

        /* i386, sparc64 */
        case 256:
            preallocate = 7;
            break;

        /* sparc64 in 32-bit mode */
        default:
            preallocate = 8;
        }
    }

    mask = 0;
    inc = 0x80000000;

    trong_khi (preallocate--) {

        key = 0;
        mask >>= 1;
        mask |= 0x80000000;

        do {
            nếu (ngx_radix32tree_insert(tree, key, mask, NGX_RADIX_NO_VALUE)
                != NGX_OK)
            {
                trả_về NULL;
            }

            key += inc;

        } trong_khi (key);

        inc >>= 1;
    }

    trả_về tree;
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

    trong_khi (bit & mask) {
        nếu (key & bit) {
            next = node->right;

        } không_thì {
            next = node->left;
        }

        nếu (next == NULL) {
            break;
        }

        bit >>= 1;
        node = next;
    }

    nếu (next) {
        nếu (node->value != NGX_RADIX_NO_VALUE) {
            trả_về NGX_BUSY;
        }

        node->value = value;
        trả_về NGX_OK;
    }

    trong_khi (bit & mask) {
        next = ngx_radix_alloc(tree);
        nếu (next == NULL) {
            trả_về NGX_ERROR;
        }

        next->right = NULL;
        next->left = NULL;
        next->parent = node;
        next->value = NGX_RADIX_NO_VALUE;

        nếu (key & bit) {
            node->right = next;

        } không_thì {
            node->left = next;
        }

        bit >>= 1;
        node = next;
    }

    node->value = value;

    trả_về NGX_OK;
}


ngx_int_t
ngx_radix32tree_delete(ngx_radix_tree_t *tree, uint32_t key, uint32_t mask)
{
    uint32_t           bit;
    ngx_radix_node_t  *node;

    bit = 0x80000000;
    node = tree->root;

    trong_khi (node && (bit & mask)) {
        nếu (key & bit) {
            node = node->right;

        } không_thì {
            node = node->left;
        }

        bit >>= 1;
    }

    nếu (node == NULL) {
        trả_về NGX_ERROR;
    }

    nếu (node->right || node->left) {
        nếu (node->value != NGX_RADIX_NO_VALUE) {
            node->value = NGX_RADIX_NO_VALUE;
            trả_về NGX_OK;
        }

        trả_về NGX_ERROR;
    }

    cho ( ;; ) {
        nếu (node->parent->right == node) {
            node->parent->right = NULL;

        } không_thì {
            node->parent->left = NULL;
        }

        node->right = tree->free;
        tree->free = node;

        node = node->parent;

        nếu (node->right || node->left) {
            break;
        }

        nếu (node->value != NGX_RADIX_NO_VALUE) {
            break;
        }

        nếu (node->parent == NULL) {
            break;
        }
    }

    trả_về NGX_OK;
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

    trong_khi (node) {
        nếu (node->value != NGX_RADIX_NO_VALUE) {
            value = node->value;
        }

        nếu (key & bit) {
            node = node->right;

        } không_thì {
            node = node->left;
        }

        bit >>= 1;
    }

    trả_về value;
}


#nếu (NGX_HAVE_INET6)

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

    trong_khi (bit & mask[i]) {
        nếu (key[i] & bit) {
            next = node->right;

        } không_thì {
            next = node->left;
        }

        nếu (next == NULL) {
            break;
        }

        bit >>= 1;
        node = next;

        nếu (bit == 0) {
            nếu (++i == 16) {
                break;
            }

            bit = 0x80;
        }
    }

    nếu (next) {
        nếu (node->value != NGX_RADIX_NO_VALUE) {
            trả_về NGX_BUSY;
        }

        node->value = value;
        trả_về NGX_OK;
    }

    trong_khi (bit & mask[i]) {
        next = ngx_radix_alloc(tree);
        nếu (next == NULL) {
            trả_về NGX_ERROR;
        }

        next->right = NULL;
        next->left = NULL;
        next->parent = node;
        next->value = NGX_RADIX_NO_VALUE;

        nếu (key[i] & bit) {
            node->right = next;

        } không_thì {
            node->left = next;
        }

        bit >>= 1;
        node = next;

        nếu (bit == 0) {
            nếu (++i == 16) {
                break;
            }

            bit = 0x80;
        }
    }

    node->value = value;

    trả_về NGX_OK;
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

    trong_khi (node && (bit & mask[i])) {
        nếu (key[i] & bit) {
            node = node->right;

        } không_thì {
            node = node->left;
        }

        bit >>= 1;

        nếu (bit == 0) {
            nếu (++i == 16) {
                break;
            }

            bit = 0x80;
        }
    }

    nếu (node == NULL) {
        trả_về NGX_ERROR;
    }

    nếu (node->right || node->left) {
        nếu (node->value != NGX_RADIX_NO_VALUE) {
            node->value = NGX_RADIX_NO_VALUE;
            trả_về NGX_OK;
        }

        trả_về NGX_ERROR;
    }

    cho ( ;; ) {
        nếu (node->parent->right == node) {
            node->parent->right = NULL;

        } không_thì {
            node->parent->left = NULL;
        }

        node->right = tree->free;
        tree->free = node;

        node = node->parent;

        nếu (node->right || node->left) {
            break;
        }

        nếu (node->value != NGX_RADIX_NO_VALUE) {
            break;
        }

        nếu (node->parent == NULL) {
            break;
        }
    }

    trả_về NGX_OK;
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

    trong_khi (node) {
        nếu (node->value != NGX_RADIX_NO_VALUE) {
            value = node->value;
        }

        nếu (key[i] & bit) {
            node = node->right;

        } không_thì {
            node = node->left;
        }

        bit >>= 1;

        nếu (bit == 0) {
            i++;
            bit = 0x80;
        }
    }

    trả_về value;
}

#endif


static ngx_radix_node_t *
ngx_radix_alloc(ngx_radix_tree_t *tree)
{
    ngx_radix_node_t  *p;

    nếu (tree->free) {
        p = tree->free;
        tree->free = tree->free->right;
        trả_về p;
    }

    nếu (tree->size < sizeof(ngx_radix_node_t)) {
        tree->start = ngx_pmemalign(tree->pool, ngx_pagesize, ngx_pagesize);
        nếu (tree->start == NULL) {
            trả_về NULL;
        }

        tree->size = ngx_pagesize;
    }

    p = (ngx_radix_node_t *) tree->start;
    tree->start += sizeof(ngx_radix_node_t);
    tree->size -= sizeof(ngx_radix_node_t);

    trả_về p;
}
