
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
    အကယ်၍ (tree == NULL) {
        ပြန်ပေး NULL;
    }

    tree->pool = pool;
    tree->free = NULL;
    tree->start = NULL;
    tree->size = 0;

    tree->root = ngx_radix_alloc(tree);
    အကယ်၍ (tree->root == NULL) {
        ပြန်ပေး NULL;
    }

    tree->root->right = NULL;
    tree->root->left = NULL;
    tree->root->parent = NULL;
    tree->root->value = NGX_RADIX_NO_VALUE;

    အကယ်၍ (preallocate == 0) {
        ပြန်ပေး tree;
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

    အကယ်၍ (preallocate == -1) {
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

    နေစဉ် (preallocate--) {

        key = 0;
        mask >>= 1;
        mask |= 0x80000000;

        do {
            အကယ်၍ (ngx_radix32tree_insert(tree, key, mask, NGX_RADIX_NO_VALUE)
                != NGX_OK)
            {
                ပြန်ပေး NULL;
            }

            key += inc;

        } နေစဉ် (key);

        inc >>= 1;
    }

    ပြန်ပေး tree;
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

    နေစဉ် (bit & mask) {
        အကယ်၍ (key & bit) {
            next = node->right;

        } မဟုတ်ပါက {
            next = node->left;
        }

        အကယ်၍ (next == NULL) {
            bha_nga;
        }

        bit >>= 1;
        node = next;
    }

    အကယ်၍ (next) {
        အကယ်၍ (node->value != NGX_RADIX_NO_VALUE) {
            ပြန်ပေး NGX_BUSY;
        }

        node->value = value;
        ပြန်ပေး NGX_OK;
    }

    နေစဉ် (bit & mask) {
        next = ngx_radix_alloc(tree);
        အကယ်၍ (next == NULL) {
            ပြန်ပေး NGX_ERROR;
        }

        next->right = NULL;
        next->left = NULL;
        next->parent = node;
        next->value = NGX_RADIX_NO_VALUE;

        အကယ်၍ (key & bit) {
            node->right = next;

        } မဟုတ်ပါက {
            node->left = next;
        }

        bit >>= 1;
        node = next;
    }

    node->value = value;

    ပြန်ပေး NGX_OK;
}


ngx_int_t
ngx_radix32tree_delete(ngx_radix_tree_t *tree, uint32_t key, uint32_t mask)
{
    uint32_t           bit;
    ngx_radix_node_t  *node;

    bit = 0x80000000;
    node = tree->root;

    နေစဉ် (node && (bit & mask)) {
        အကယ်၍ (key & bit) {
            node = node->right;

        } မဟုတ်ပါက {
            node = node->left;
        }

        bit >>= 1;
    }

    အကယ်၍ (node == NULL) {
        ပြန်ပေး NGX_ERROR;
    }

    အကယ်၍ (node->right || node->left) {
        အကယ်၍ (node->value != NGX_RADIX_NO_VALUE) {
            node->value = NGX_RADIX_NO_VALUE;
            ပြန်ပေး NGX_OK;
        }

        ပြန်ပေး NGX_ERROR;
    }

    အတွက် ( ;; ) {
        အကယ်၍ (node->parent->right == node) {
            node->parent->right = NULL;

        } မဟုတ်ပါက {
            node->parent->left = NULL;
        }

        node->right = tree->free;
        tree->free = node;

        node = node->parent;

        အကယ်၍ (node->right || node->left) {
            bha_nga;
        }

        အကယ်၍ (node->value != NGX_RADIX_NO_VALUE) {
            bha_nga;
        }

        အကယ်၍ (node->parent == NULL) {
            bha_nga;
        }
    }

    ပြန်ပေး NGX_OK;
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

    နေစဉ် (node) {
        အကယ်၍ (node->value != NGX_RADIX_NO_VALUE) {
            value = node->value;
        }

        အကယ်၍ (key & bit) {
            node = node->right;

        } မဟုတ်ပါက {
            node = node->left;
        }

        bit >>= 1;
    }

    ပြန်ပေး value;
}


#အကယ်၍ (NGX_HAVE_INET6)

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

    နေစဉ် (bit & mask[i]) {
        အကယ်၍ (key[i] & bit) {
            next = node->right;

        } မဟုတ်ပါက {
            next = node->left;
        }

        အကယ်၍ (next == NULL) {
            bha_nga;
        }

        bit >>= 1;
        node = next;

        အကယ်၍ (bit == 0) {
            အကယ်၍ (++i == 16) {
                bha_nga;
            }

            bit = 0x80;
        }
    }

    အကယ်၍ (next) {
        အကယ်၍ (node->value != NGX_RADIX_NO_VALUE) {
            ပြန်ပေး NGX_BUSY;
        }

        node->value = value;
        ပြန်ပေး NGX_OK;
    }

    နေစဉ် (bit & mask[i]) {
        next = ngx_radix_alloc(tree);
        အကယ်၍ (next == NULL) {
            ပြန်ပေး NGX_ERROR;
        }

        next->right = NULL;
        next->left = NULL;
        next->parent = node;
        next->value = NGX_RADIX_NO_VALUE;

        အကယ်၍ (key[i] & bit) {
            node->right = next;

        } မဟုတ်ပါက {
            node->left = next;
        }

        bit >>= 1;
        node = next;

        အကယ်၍ (bit == 0) {
            အကယ်၍ (++i == 16) {
                bha_nga;
            }

            bit = 0x80;
        }
    }

    node->value = value;

    ပြန်ပေး NGX_OK;
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

    နေစဉ် (node && (bit & mask[i])) {
        အကယ်၍ (key[i] & bit) {
            node = node->right;

        } မဟုတ်ပါက {
            node = node->left;
        }

        bit >>= 1;

        အကယ်၍ (bit == 0) {
            အကယ်၍ (++i == 16) {
                bha_nga;
            }

            bit = 0x80;
        }
    }

    အကယ်၍ (node == NULL) {
        ပြန်ပေး NGX_ERROR;
    }

    အကယ်၍ (node->right || node->left) {
        အကယ်၍ (node->value != NGX_RADIX_NO_VALUE) {
            node->value = NGX_RADIX_NO_VALUE;
            ပြန်ပေး NGX_OK;
        }

        ပြန်ပေး NGX_ERROR;
    }

    အတွက် ( ;; ) {
        အကယ်၍ (node->parent->right == node) {
            node->parent->right = NULL;

        } မဟုတ်ပါက {
            node->parent->left = NULL;
        }

        node->right = tree->free;
        tree->free = node;

        node = node->parent;

        အကယ်၍ (node->right || node->left) {
            bha_nga;
        }

        အကယ်၍ (node->value != NGX_RADIX_NO_VALUE) {
            bha_nga;
        }

        အကယ်၍ (node->parent == NULL) {
            bha_nga;
        }
    }

    ပြန်ပေး NGX_OK;
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

    နေစဉ် (node) {
        အကယ်၍ (node->value != NGX_RADIX_NO_VALUE) {
            value = node->value;
        }

        အကယ်၍ (key[i] & bit) {
            node = node->right;

        } မဟုတ်ပါက {
            node = node->left;
        }

        bit >>= 1;

        အကယ်၍ (bit == 0) {
            i++;
            bit = 0x80;
        }
    }

    ပြန်ပေး value;
}

#endif


static ngx_radix_node_t *
ngx_radix_alloc(ngx_radix_tree_t *tree)
{
    ngx_radix_node_t  *p;

    အကယ်၍ (tree->free) {
        p = tree->free;
        tree->free = tree->free->right;
        ပြန်ပေး p;
    }

    အကယ်၍ (tree->size < parimaa_na(ngx_radix_node_t)) {
        tree->start = ngx_pmemalign(tree->pool, ngx_pagesize, ngx_pagesize);
        အကယ်၍ (tree->start == NULL) {
            ပြန်ပေး NULL;
        }

        tree->size = ngx_pagesize;
    }

    p = (ngx_radix_node_t *) tree->start;
    tree->start += parimaa_na(ngx_radix_node_t);
    tree->size -= parimaa_na(ngx_radix_node_t);

    ပြန်ပေး p;
}
