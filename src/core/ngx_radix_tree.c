
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
    если (tree == NULL) {
        вернуть NULL;
    }

    tree->pool = pool;
    tree->free = NULL;
    tree->start = NULL;
    tree->size = 0;

    tree->root = ngx_radix_alloc(tree);
    если (tree->root == NULL) {
        вернуть NULL;
    }

    tree->root->right = NULL;
    tree->root->left = NULL;
    tree->root->parent = NULL;
    tree->root->value = NGX_RADIX_NO_VALUE;

    если (preallocate == 0) {
        вернуть tree;
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

    если (preallocate == -1) {
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

    пока (preallocate--) {

        key = 0;
        mask >>= 1;
        mask |= 0x80000000;

        do {
            если (ngx_radix32tree_insert(tree, key, mask, NGX_RADIX_NO_VALUE)
                != NGX_OK)
            {
                вернуть NULL;
            }

            key += inc;

        } пока (key);

        inc >>= 1;
    }

    вернуть tree;
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

    пока (bit & mask) {
        если (key & bit) {
            next = node->right;

        } иначе {
            next = node->left;
        }

        если (next == NULL) {
            break;
        }

        bit >>= 1;
        node = next;
    }

    если (next) {
        если (node->value != NGX_RADIX_NO_VALUE) {
            вернуть NGX_BUSY;
        }

        node->value = value;
        вернуть NGX_OK;
    }

    пока (bit & mask) {
        next = ngx_radix_alloc(tree);
        если (next == NULL) {
            вернуть NGX_ERROR;
        }

        next->right = NULL;
        next->left = NULL;
        next->parent = node;
        next->value = NGX_RADIX_NO_VALUE;

        если (key & bit) {
            node->right = next;

        } иначе {
            node->left = next;
        }

        bit >>= 1;
        node = next;
    }

    node->value = value;

    вернуть NGX_OK;
}


ngx_int_t
ngx_radix32tree_delete(ngx_radix_tree_t *tree, uint32_t key, uint32_t mask)
{
    uint32_t           bit;
    ngx_radix_node_t  *node;

    bit = 0x80000000;
    node = tree->root;

    пока (node && (bit & mask)) {
        если (key & bit) {
            node = node->right;

        } иначе {
            node = node->left;
        }

        bit >>= 1;
    }

    если (node == NULL) {
        вернуть NGX_ERROR;
    }

    если (node->right || node->left) {
        если (node->value != NGX_RADIX_NO_VALUE) {
            node->value = NGX_RADIX_NO_VALUE;
            вернуть NGX_OK;
        }

        вернуть NGX_ERROR;
    }

    для ( ;; ) {
        если (node->parent->right == node) {
            node->parent->right = NULL;

        } иначе {
            node->parent->left = NULL;
        }

        node->right = tree->free;
        tree->free = node;

        node = node->parent;

        если (node->right || node->left) {
            break;
        }

        если (node->value != NGX_RADIX_NO_VALUE) {
            break;
        }

        если (node->parent == NULL) {
            break;
        }
    }

    вернуть NGX_OK;
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

    пока (node) {
        если (node->value != NGX_RADIX_NO_VALUE) {
            value = node->value;
        }

        если (key & bit) {
            node = node->right;

        } иначе {
            node = node->left;
        }

        bit >>= 1;
    }

    вернуть value;
}


#если (NGX_HAVE_INET6)

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

    пока (bit & mask[i]) {
        если (key[i] & bit) {
            next = node->right;

        } иначе {
            next = node->left;
        }

        если (next == NULL) {
            break;
        }

        bit >>= 1;
        node = next;

        если (bit == 0) {
            если (++i == 16) {
                break;
            }

            bit = 0x80;
        }
    }

    если (next) {
        если (node->value != NGX_RADIX_NO_VALUE) {
            вернуть NGX_BUSY;
        }

        node->value = value;
        вернуть NGX_OK;
    }

    пока (bit & mask[i]) {
        next = ngx_radix_alloc(tree);
        если (next == NULL) {
            вернуть NGX_ERROR;
        }

        next->right = NULL;
        next->left = NULL;
        next->parent = node;
        next->value = NGX_RADIX_NO_VALUE;

        если (key[i] & bit) {
            node->right = next;

        } иначе {
            node->left = next;
        }

        bit >>= 1;
        node = next;

        если (bit == 0) {
            если (++i == 16) {
                break;
            }

            bit = 0x80;
        }
    }

    node->value = value;

    вернуть NGX_OK;
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

    пока (node && (bit & mask[i])) {
        если (key[i] & bit) {
            node = node->right;

        } иначе {
            node = node->left;
        }

        bit >>= 1;

        если (bit == 0) {
            если (++i == 16) {
                break;
            }

            bit = 0x80;
        }
    }

    если (node == NULL) {
        вернуть NGX_ERROR;
    }

    если (node->right || node->left) {
        если (node->value != NGX_RADIX_NO_VALUE) {
            node->value = NGX_RADIX_NO_VALUE;
            вернуть NGX_OK;
        }

        вернуть NGX_ERROR;
    }

    для ( ;; ) {
        если (node->parent->right == node) {
            node->parent->right = NULL;

        } иначе {
            node->parent->left = NULL;
        }

        node->right = tree->free;
        tree->free = node;

        node = node->parent;

        если (node->right || node->left) {
            break;
        }

        если (node->value != NGX_RADIX_NO_VALUE) {
            break;
        }

        если (node->parent == NULL) {
            break;
        }
    }

    вернуть NGX_OK;
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

    пока (node) {
        если (node->value != NGX_RADIX_NO_VALUE) {
            value = node->value;
        }

        если (key[i] & bit) {
            node = node->right;

        } иначе {
            node = node->left;
        }

        bit >>= 1;

        если (bit == 0) {
            i++;
            bit = 0x80;
        }
    }

    вернуть value;
}

#endif


static ngx_radix_node_t *
ngx_radix_alloc(ngx_radix_tree_t *tree)
{
    ngx_radix_node_t  *p;

    если (tree->free) {
        p = tree->free;
        tree->free = tree->free->right;
        вернуть p;
    }

    если (tree->size < sizeof(ngx_radix_node_t)) {
        tree->start = ngx_pmemalign(tree->pool, ngx_pagesize, ngx_pagesize);
        если (tree->start == NULL) {
            вернуть NULL;
        }

        tree->size = ngx_pagesize;
    }

    p = (ngx_radix_node_t *) tree->start;
    tree->start += sizeof(ngx_radix_node_t);
    tree->size -= sizeof(ngx_radix_node_t);

    вернуть p;
}
