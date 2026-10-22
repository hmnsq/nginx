
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
    如果 (tree == NULL) {
        返回 NULL;
    }

    tree->pool = pool;
    tree->free = NULL;
    tree->start = NULL;
    tree->size = 0;

    tree->root = ngx_radix_alloc(tree);
    如果 (tree->root == NULL) {
        返回 NULL;
    }

    tree->root->right = NULL;
    tree->root->left = NULL;
    tree->root->parent = NULL;
    tree->root->value = NGX_RADIX_NO_VALUE;

    如果 (preallocate == 0) {
        返回 tree;
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

    如果 (preallocate == -1) {
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

    当 (preallocate--) {

        key = 0;
        mask >>= 1;
        mask |= 0x80000000;

        do {
            如果 (ngx_radix32tree_insert(tree, key, mask, NGX_RADIX_NO_VALUE)
                != NGX_OK)
            {
                返回 NULL;
            }

            key += inc;

        } 当 (key);

        inc >>= 1;
    }

    返回 tree;
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

    当 (bit & mask) {
        如果 (key & bit) {
            next = node->right;

        } 否则 {
            next = node->left;
        }

        如果 (next == NULL) {
            break;
        }

        bit >>= 1;
        node = next;
    }

    如果 (next) {
        如果 (node->value != NGX_RADIX_NO_VALUE) {
            返回 NGX_BUSY;
        }

        node->value = value;
        返回 NGX_OK;
    }

    当 (bit & mask) {
        next = ngx_radix_alloc(tree);
        如果 (next == NULL) {
            返回 NGX_ERROR;
        }

        next->right = NULL;
        next->left = NULL;
        next->parent = node;
        next->value = NGX_RADIX_NO_VALUE;

        如果 (key & bit) {
            node->right = next;

        } 否则 {
            node->left = next;
        }

        bit >>= 1;
        node = next;
    }

    node->value = value;

    返回 NGX_OK;
}


ngx_int_t
ngx_radix32tree_delete(ngx_radix_tree_t *tree, uint32_t key, uint32_t mask)
{
    uint32_t           bit;
    ngx_radix_node_t  *node;

    bit = 0x80000000;
    node = tree->root;

    当 (node && (bit & mask)) {
        如果 (key & bit) {
            node = node->right;

        } 否则 {
            node = node->left;
        }

        bit >>= 1;
    }

    如果 (node == NULL) {
        返回 NGX_ERROR;
    }

    如果 (node->right || node->left) {
        如果 (node->value != NGX_RADIX_NO_VALUE) {
            node->value = NGX_RADIX_NO_VALUE;
            返回 NGX_OK;
        }

        返回 NGX_ERROR;
    }

    为 ( ;; ) {
        如果 (node->parent->right == node) {
            node->parent->right = NULL;

        } 否则 {
            node->parent->left = NULL;
        }

        node->right = tree->free;
        tree->free = node;

        node = node->parent;

        如果 (node->right || node->left) {
            break;
        }

        如果 (node->value != NGX_RADIX_NO_VALUE) {
            break;
        }

        如果 (node->parent == NULL) {
            break;
        }
    }

    返回 NGX_OK;
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

    当 (node) {
        如果 (node->value != NGX_RADIX_NO_VALUE) {
            value = node->value;
        }

        如果 (key & bit) {
            node = node->right;

        } 否则 {
            node = node->left;
        }

        bit >>= 1;
    }

    返回 value;
}


#如果 (NGX_HAVE_INET6)

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

    当 (bit & mask[i]) {
        如果 (key[i] & bit) {
            next = node->right;

        } 否则 {
            next = node->left;
        }

        如果 (next == NULL) {
            break;
        }

        bit >>= 1;
        node = next;

        如果 (bit == 0) {
            如果 (++i == 16) {
                break;
            }

            bit = 0x80;
        }
    }

    如果 (next) {
        如果 (node->value != NGX_RADIX_NO_VALUE) {
            返回 NGX_BUSY;
        }

        node->value = value;
        返回 NGX_OK;
    }

    当 (bit & mask[i]) {
        next = ngx_radix_alloc(tree);
        如果 (next == NULL) {
            返回 NGX_ERROR;
        }

        next->right = NULL;
        next->left = NULL;
        next->parent = node;
        next->value = NGX_RADIX_NO_VALUE;

        如果 (key[i] & bit) {
            node->right = next;

        } 否则 {
            node->left = next;
        }

        bit >>= 1;
        node = next;

        如果 (bit == 0) {
            如果 (++i == 16) {
                break;
            }

            bit = 0x80;
        }
    }

    node->value = value;

    返回 NGX_OK;
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

    当 (node && (bit & mask[i])) {
        如果 (key[i] & bit) {
            node = node->right;

        } 否则 {
            node = node->left;
        }

        bit >>= 1;

        如果 (bit == 0) {
            如果 (++i == 16) {
                break;
            }

            bit = 0x80;
        }
    }

    如果 (node == NULL) {
        返回 NGX_ERROR;
    }

    如果 (node->right || node->left) {
        如果 (node->value != NGX_RADIX_NO_VALUE) {
            node->value = NGX_RADIX_NO_VALUE;
            返回 NGX_OK;
        }

        返回 NGX_ERROR;
    }

    为 ( ;; ) {
        如果 (node->parent->right == node) {
            node->parent->right = NULL;

        } 否则 {
            node->parent->left = NULL;
        }

        node->right = tree->free;
        tree->free = node;

        node = node->parent;

        如果 (node->right || node->left) {
            break;
        }

        如果 (node->value != NGX_RADIX_NO_VALUE) {
            break;
        }

        如果 (node->parent == NULL) {
            break;
        }
    }

    返回 NGX_OK;
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

    当 (node) {
        如果 (node->value != NGX_RADIX_NO_VALUE) {
            value = node->value;
        }

        如果 (key[i] & bit) {
            node = node->right;

        } 否则 {
            node = node->left;
        }

        bit >>= 1;

        如果 (bit == 0) {
            i++;
            bit = 0x80;
        }
    }

    返回 value;
}

#endif


static ngx_radix_node_t *
ngx_radix_alloc(ngx_radix_tree_t *tree)
{
    ngx_radix_node_t  *p;

    如果 (tree->free) {
        p = tree->free;
        tree->free = tree->free->right;
        返回 p;
    }

    如果 (tree->size < sizeof(ngx_radix_node_t)) {
        tree->start = ngx_pmemalign(tree->pool, ngx_pagesize, ngx_pagesize);
        如果 (tree->start == NULL) {
            返回 NULL;
        }

        tree->size = ngx_pagesize;
    }

    p = (ngx_radix_node_t *) tree->start;
    tree->start += sizeof(ngx_radix_node_t);
    tree->size -= sizeof(ngx_radix_node_t);

    返回 p;
}
