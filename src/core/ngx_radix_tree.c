
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
    якщо (tree == NULL) {
        повернути NULL;
    }

    tree->pool = pool;
    tree->free = NULL;
    tree->start = NULL;
    tree->size = 0;

    tree->root = ngx_radix_alloc(tree);
    якщо (tree->root == NULL) {
        повернути NULL;
    }

    tree->root->right = NULL;
    tree->root->left = NULL;
    tree->root->parent = NULL;
    tree->root->value = NGX_RADIX_NO_VALUE;

    якщо (preallocate == 0) {
        повернути tree;
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

    якщо (preallocate == -1) {
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

    поки (preallocate--) {

        key = 0;
        mask >>= 1;
        mask |= 0x80000000;

        do {
            якщо (ngx_radix32tree_insert(tree, key, mask, NGX_RADIX_NO_VALUE)
                != NGX_OK)
            {
                повернути NULL;
            }

            key += inc;

        } поки (key);

        inc >>= 1;
    }

    повернути tree;
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

    поки (bit & mask) {
        якщо (key & bit) {
            next = node->right;

        } інакше {
            next = node->left;
        }

        якщо (next == NULL) {
            break;
        }

        bit >>= 1;
        node = next;
    }

    якщо (next) {
        якщо (node->value != NGX_RADIX_NO_VALUE) {
            повернути NGX_BUSY;
        }

        node->value = value;
        повернути NGX_OK;
    }

    поки (bit & mask) {
        next = ngx_radix_alloc(tree);
        якщо (next == NULL) {
            повернути NGX_ERROR;
        }

        next->right = NULL;
        next->left = NULL;
        next->parent = node;
        next->value = NGX_RADIX_NO_VALUE;

        якщо (key & bit) {
            node->right = next;

        } інакше {
            node->left = next;
        }

        bit >>= 1;
        node = next;
    }

    node->value = value;

    повернути NGX_OK;
}


ngx_int_t
ngx_radix32tree_delete(ngx_radix_tree_t *tree, uint32_t key, uint32_t mask)
{
    uint32_t           bit;
    ngx_radix_node_t  *node;

    bit = 0x80000000;
    node = tree->root;

    поки (node && (bit & mask)) {
        якщо (key & bit) {
            node = node->right;

        } інакше {
            node = node->left;
        }

        bit >>= 1;
    }

    якщо (node == NULL) {
        повернути NGX_ERROR;
    }

    якщо (node->right || node->left) {
        якщо (node->value != NGX_RADIX_NO_VALUE) {
            node->value = NGX_RADIX_NO_VALUE;
            повернути NGX_OK;
        }

        повернути NGX_ERROR;
    }

    для ( ;; ) {
        якщо (node->parent->right == node) {
            node->parent->right = NULL;

        } інакше {
            node->parent->left = NULL;
        }

        node->right = tree->free;
        tree->free = node;

        node = node->parent;

        якщо (node->right || node->left) {
            break;
        }

        якщо (node->value != NGX_RADIX_NO_VALUE) {
            break;
        }

        якщо (node->parent == NULL) {
            break;
        }
    }

    повернути NGX_OK;
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

    поки (node) {
        якщо (node->value != NGX_RADIX_NO_VALUE) {
            value = node->value;
        }

        якщо (key & bit) {
            node = node->right;

        } інакше {
            node = node->left;
        }

        bit >>= 1;
    }

    повернути value;
}


#якщо (NGX_HAVE_INET6)

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

    поки (bit & mask[i]) {
        якщо (key[i] & bit) {
            next = node->right;

        } інакше {
            next = node->left;
        }

        якщо (next == NULL) {
            break;
        }

        bit >>= 1;
        node = next;

        якщо (bit == 0) {
            якщо (++i == 16) {
                break;
            }

            bit = 0x80;
        }
    }

    якщо (next) {
        якщо (node->value != NGX_RADIX_NO_VALUE) {
            повернути NGX_BUSY;
        }

        node->value = value;
        повернути NGX_OK;
    }

    поки (bit & mask[i]) {
        next = ngx_radix_alloc(tree);
        якщо (next == NULL) {
            повернути NGX_ERROR;
        }

        next->right = NULL;
        next->left = NULL;
        next->parent = node;
        next->value = NGX_RADIX_NO_VALUE;

        якщо (key[i] & bit) {
            node->right = next;

        } інакше {
            node->left = next;
        }

        bit >>= 1;
        node = next;

        якщо (bit == 0) {
            якщо (++i == 16) {
                break;
            }

            bit = 0x80;
        }
    }

    node->value = value;

    повернути NGX_OK;
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

    поки (node && (bit & mask[i])) {
        якщо (key[i] & bit) {
            node = node->right;

        } інакше {
            node = node->left;
        }

        bit >>= 1;

        якщо (bit == 0) {
            якщо (++i == 16) {
                break;
            }

            bit = 0x80;
        }
    }

    якщо (node == NULL) {
        повернути NGX_ERROR;
    }

    якщо (node->right || node->left) {
        якщо (node->value != NGX_RADIX_NO_VALUE) {
            node->value = NGX_RADIX_NO_VALUE;
            повернути NGX_OK;
        }

        повернути NGX_ERROR;
    }

    для ( ;; ) {
        якщо (node->parent->right == node) {
            node->parent->right = NULL;

        } інакше {
            node->parent->left = NULL;
        }

        node->right = tree->free;
        tree->free = node;

        node = node->parent;

        якщо (node->right || node->left) {
            break;
        }

        якщо (node->value != NGX_RADIX_NO_VALUE) {
            break;
        }

        якщо (node->parent == NULL) {
            break;
        }
    }

    повернути NGX_OK;
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

    поки (node) {
        якщо (node->value != NGX_RADIX_NO_VALUE) {
            value = node->value;
        }

        якщо (key[i] & bit) {
            node = node->right;

        } інакше {
            node = node->left;
        }

        bit >>= 1;

        якщо (bit == 0) {
            i++;
            bit = 0x80;
        }
    }

    повернути value;
}

#endif


static ngx_radix_node_t *
ngx_radix_alloc(ngx_radix_tree_t *tree)
{
    ngx_radix_node_t  *p;

    якщо (tree->free) {
        p = tree->free;
        tree->free = tree->free->right;
        повернути p;
    }

    якщо (tree->size < sizeof(ngx_radix_node_t)) {
        tree->start = ngx_pmemalign(tree->pool, ngx_pagesize, ngx_pagesize);
        якщо (tree->start == NULL) {
            повернути NULL;
        }

        tree->size = ngx_pagesize;
    }

    p = (ngx_radix_node_t *) tree->start;
    tree->start += sizeof(ngx_radix_node_t);
    tree->size -= sizeof(ngx_radix_node_t);

    повернути p;
}
