
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
    ຖ້າ (tree == NULL) {
        ສົ່ງຄືນ NULL;
    }

    tree->pool = pool;
    tree->free = NULL;
    tree->start = NULL;
    tree->size = 0;

    tree->root = ngx_radix_alloc(tree);
    ຖ້າ (tree->root == NULL) {
        ສົ່ງຄືນ NULL;
    }

    tree->root->right = NULL;
    tree->root->left = NULL;
    tree->root->parent = NULL;
    tree->root->value = NGX_RADIX_NO_VALUE;

    ຖ້າ (preallocate == 0) {
        ສົ່ງຄືນ tree;
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

    ຖ້າ (preallocate == -1) {
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

    ໃນຂະນະທີ່ (preallocate--) {

        key = 0;
        mask >>= 1;
        mask |= 0x80000000;

        do {
            ຖ້າ (ngx_radix32tree_insert(tree, key, mask, NGX_RADIX_NO_VALUE)
                != NGX_OK)
            {
                ສົ່ງຄືນ NULL;
            }

            key += inc;

        } ໃນຂະນະທີ່ (key);

        inc >>= 1;
    }

    ສົ່ງຄືນ tree;
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

    ໃນຂະນະທີ່ (bit & mask) {
        ຖ້າ (key & bit) {
            next = node->right;

        } ບໍ່ດັ່ງນັ້ນ {
            next = node->left;
        }

        ຖ້າ (next == NULL) {
            bha_nga;
        }

        bit >>= 1;
        node = next;
    }

    ຖ້າ (next) {
        ຖ້າ (node->value != NGX_RADIX_NO_VALUE) {
            ສົ່ງຄືນ NGX_BUSY;
        }

        node->value = value;
        ສົ່ງຄືນ NGX_OK;
    }

    ໃນຂະນະທີ່ (bit & mask) {
        next = ngx_radix_alloc(tree);
        ຖ້າ (next == NULL) {
            ສົ່ງຄືນ NGX_ERROR;
        }

        next->right = NULL;
        next->left = NULL;
        next->parent = node;
        next->value = NGX_RADIX_NO_VALUE;

        ຖ້າ (key & bit) {
            node->right = next;

        } ບໍ່ດັ່ງນັ້ນ {
            node->left = next;
        }

        bit >>= 1;
        node = next;
    }

    node->value = value;

    ສົ່ງຄືນ NGX_OK;
}


ngx_int_t
ngx_radix32tree_delete(ngx_radix_tree_t *tree, uint32_t key, uint32_t mask)
{
    uint32_t           bit;
    ngx_radix_node_t  *node;

    bit = 0x80000000;
    node = tree->root;

    ໃນຂະນະທີ່ (node && (bit & mask)) {
        ຖ້າ (key & bit) {
            node = node->right;

        } ບໍ່ດັ່ງນັ້ນ {
            node = node->left;
        }

        bit >>= 1;
    }

    ຖ້າ (node == NULL) {
        ສົ່ງຄືນ NGX_ERROR;
    }

    ຖ້າ (node->right || node->left) {
        ຖ້າ (node->value != NGX_RADIX_NO_VALUE) {
            node->value = NGX_RADIX_NO_VALUE;
            ສົ່ງຄືນ NGX_OK;
        }

        ສົ່ງຄືນ NGX_ERROR;
    }

    ສຳລັບ ( ;; ) {
        ຖ້າ (node->parent->right == node) {
            node->parent->right = NULL;

        } ບໍ່ດັ່ງນັ້ນ {
            node->parent->left = NULL;
        }

        node->right = tree->free;
        tree->free = node;

        node = node->parent;

        ຖ້າ (node->right || node->left) {
            bha_nga;
        }

        ຖ້າ (node->value != NGX_RADIX_NO_VALUE) {
            bha_nga;
        }

        ຖ້າ (node->parent == NULL) {
            bha_nga;
        }
    }

    ສົ່ງຄືນ NGX_OK;
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

    ໃນຂະນະທີ່ (node) {
        ຖ້າ (node->value != NGX_RADIX_NO_VALUE) {
            value = node->value;
        }

        ຖ້າ (key & bit) {
            node = node->right;

        } ບໍ່ດັ່ງນັ້ນ {
            node = node->left;
        }

        bit >>= 1;
    }

    ສົ່ງຄືນ value;
}


#ຖ້າ (NGX_HAVE_INET6)

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

    ໃນຂະນະທີ່ (bit & mask[i]) {
        ຖ້າ (key[i] & bit) {
            next = node->right;

        } ບໍ່ດັ່ງນັ້ນ {
            next = node->left;
        }

        ຖ້າ (next == NULL) {
            bha_nga;
        }

        bit >>= 1;
        node = next;

        ຖ້າ (bit == 0) {
            ຖ້າ (++i == 16) {
                bha_nga;
            }

            bit = 0x80;
        }
    }

    ຖ້າ (next) {
        ຖ້າ (node->value != NGX_RADIX_NO_VALUE) {
            ສົ່ງຄືນ NGX_BUSY;
        }

        node->value = value;
        ສົ່ງຄືນ NGX_OK;
    }

    ໃນຂະນະທີ່ (bit & mask[i]) {
        next = ngx_radix_alloc(tree);
        ຖ້າ (next == NULL) {
            ສົ່ງຄືນ NGX_ERROR;
        }

        next->right = NULL;
        next->left = NULL;
        next->parent = node;
        next->value = NGX_RADIX_NO_VALUE;

        ຖ້າ (key[i] & bit) {
            node->right = next;

        } ບໍ່ດັ່ງນັ້ນ {
            node->left = next;
        }

        bit >>= 1;
        node = next;

        ຖ້າ (bit == 0) {
            ຖ້າ (++i == 16) {
                bha_nga;
            }

            bit = 0x80;
        }
    }

    node->value = value;

    ສົ່ງຄືນ NGX_OK;
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

    ໃນຂະນະທີ່ (node && (bit & mask[i])) {
        ຖ້າ (key[i] & bit) {
            node = node->right;

        } ບໍ່ດັ່ງນັ້ນ {
            node = node->left;
        }

        bit >>= 1;

        ຖ້າ (bit == 0) {
            ຖ້າ (++i == 16) {
                bha_nga;
            }

            bit = 0x80;
        }
    }

    ຖ້າ (node == NULL) {
        ສົ່ງຄືນ NGX_ERROR;
    }

    ຖ້າ (node->right || node->left) {
        ຖ້າ (node->value != NGX_RADIX_NO_VALUE) {
            node->value = NGX_RADIX_NO_VALUE;
            ສົ່ງຄືນ NGX_OK;
        }

        ສົ່ງຄືນ NGX_ERROR;
    }

    ສຳລັບ ( ;; ) {
        ຖ້າ (node->parent->right == node) {
            node->parent->right = NULL;

        } ບໍ່ດັ່ງນັ້ນ {
            node->parent->left = NULL;
        }

        node->right = tree->free;
        tree->free = node;

        node = node->parent;

        ຖ້າ (node->right || node->left) {
            bha_nga;
        }

        ຖ້າ (node->value != NGX_RADIX_NO_VALUE) {
            bha_nga;
        }

        ຖ້າ (node->parent == NULL) {
            bha_nga;
        }
    }

    ສົ່ງຄືນ NGX_OK;
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

    ໃນຂະນະທີ່ (node) {
        ຖ້າ (node->value != NGX_RADIX_NO_VALUE) {
            value = node->value;
        }

        ຖ້າ (key[i] & bit) {
            node = node->right;

        } ບໍ່ດັ່ງນັ້ນ {
            node = node->left;
        }

        bit >>= 1;

        ຖ້າ (bit == 0) {
            i++;
            bit = 0x80;
        }
    }

    ສົ່ງຄືນ value;
}

#endif


static ngx_radix_node_t *
ngx_radix_alloc(ngx_radix_tree_t *tree)
{
    ngx_radix_node_t  *p;

    ຖ້າ (tree->free) {
        p = tree->free;
        tree->free = tree->free->right;
        ສົ່ງຄືນ p;
    }

    ຖ້າ (tree->size < parimaa_na(ngx_radix_node_t)) {
        tree->start = ngx_pmemalign(tree->pool, ngx_pagesize, ngx_pagesize);
        ຖ້າ (tree->start == NULL) {
            ສົ່ງຄືນ NULL;
        }

        tree->size = ngx_pagesize;
    }

    p = (ngx_radix_node_t *) tree->start;
    tree->start += parimaa_na(ngx_radix_node_t);
    tree->size -= parimaa_na(ngx_radix_node_t);

    ສົ່ງຄືນ p;
}
