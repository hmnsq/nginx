
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن ngx_radix_node_t *ngx_radix_alloc(ngx_radix_tree_t *tree);


ngx_radix_tree_t *
ngx_radix_tree_create(ngx_pool_t *pool, ngx_int_t preallocate)
{
    uint32_t           key, mask, inc;
    ngx_radix_tree_t  *tree;

    tree = ngx_palloc(pool, حجم(ngx_radix_tree_t));
    אם (tree == عدم) {
        החזר عدم;
    }

    tree->pool = pool;
    tree->حرر = عدم;
    tree->start = عدم;
    tree->size = 0;

    tree->root = ngx_radix_alloc(tree);
    אם (tree->root == عدم) {
        החזר عدم;
    }

    tree->root->right = عدم;
    tree->root->left = عدم;
    tree->root->parent = عدم;
    tree->root->value = NGX_RADIX_NO_VALUE;

    אם (preallocate == 0) {
        החזר tree;
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

    אם (preallocate == -1) {
        بدل (ngx_pagesize / حجم(ngx_radix_node_t)) {

        /* amd64 */
        حالة 128:
            preallocate = 6;
            اكسر;

        /* i386, sparc64 */
        حالة 256:
            preallocate = 7;
            اكسر;

        /* sparc64 in 32-bit mode */
        افتراضي:
            preallocate = 8;
        }
    }

    mask = 0;
    inc = 0x80000000;

    כלעוד (preallocate--) {

        key = 0;
        mask >>= 1;
        mask |= 0x80000000;

        עשה {
            אם (ngx_radix32tree_insert(tree, key, mask, NGX_RADIX_NO_VALUE)
                != NGX_OK)
            {
                החזר عدم;
            }

            key += inc;

        } כלעוד (key);

        inc >>= 1;
    }

    החזר tree;
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

    כלעוד (bit & mask) {
        אם (key & bit) {
            next = node->right;

        } אחרת {
            next = node->left;
        }

        אם (next == عدم) {
            اكسر;
        }

        bit >>= 1;
        node = next;
    }

    אם (next) {
        אם (node->value != NGX_RADIX_NO_VALUE) {
            החזר NGX_BUSY;
        }

        node->value = value;
        החזר NGX_OK;
    }

    כלעוד (bit & mask) {
        next = ngx_radix_alloc(tree);
        אם (next == عدم) {
            החזר NGX_ERROR;
        }

        next->right = عدم;
        next->left = عدم;
        next->parent = node;
        next->value = NGX_RADIX_NO_VALUE;

        אם (key & bit) {
            node->right = next;

        } אחרת {
            node->left = next;
        }

        bit >>= 1;
        node = next;
    }

    node->value = value;

    החזר NGX_OK;
}


ngx_int_t
ngx_radix32tree_delete(ngx_radix_tree_t *tree, uint32_t key, uint32_t mask)
{
    uint32_t           bit;
    ngx_radix_node_t  *node;

    bit = 0x80000000;
    node = tree->root;

    כלעוד (node && (bit & mask)) {
        אם (key & bit) {
            node = node->right;

        } אחרת {
            node = node->left;
        }

        bit >>= 1;
    }

    אם (node == عدم) {
        החזר NGX_ERROR;
    }

    אם (node->right || node->left) {
        אם (node->value != NGX_RADIX_NO_VALUE) {
            node->value = NGX_RADIX_NO_VALUE;
            החזר NGX_OK;
        }

        החזר NGX_ERROR;
    }

    עבור ( ;; ) {
        אם (node->parent->right == node) {
            node->parent->right = عدم;

        } אחרת {
            node->parent->left = عدم;
        }

        node->right = tree->حرر;
        tree->حرر = node;

        node = node->parent;

        אם (node->right || node->left) {
            اكسر;
        }

        אם (node->value != NGX_RADIX_NO_VALUE) {
            اكسر;
        }

        אם (node->parent == عدم) {
            اكسر;
        }
    }

    החזר NGX_OK;
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

    כלעוד (node) {
        אם (node->value != NGX_RADIX_NO_VALUE) {
            value = node->value;
        }

        אם (key & bit) {
            node = node->right;

        } אחרת {
            node = node->left;
        }

        bit >>= 1;
    }

    החזר value;
}


#אם (NGX_HAVE_INET6)

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

    כלעוד (bit & mask[i]) {
        אם (key[i] & bit) {
            next = node->right;

        } אחרת {
            next = node->left;
        }

        אם (next == عدم) {
            اكسر;
        }

        bit >>= 1;
        node = next;

        אם (bit == 0) {
            אם (++i == 16) {
                اكسر;
            }

            bit = 0x80;
        }
    }

    אם (next) {
        אם (node->value != NGX_RADIX_NO_VALUE) {
            החזר NGX_BUSY;
        }

        node->value = value;
        החזר NGX_OK;
    }

    כלעוד (bit & mask[i]) {
        next = ngx_radix_alloc(tree);
        אם (next == عدم) {
            החזר NGX_ERROR;
        }

        next->right = عدم;
        next->left = عدم;
        next->parent = node;
        next->value = NGX_RADIX_NO_VALUE;

        אם (key[i] & bit) {
            node->right = next;

        } אחרת {
            node->left = next;
        }

        bit >>= 1;
        node = next;

        אם (bit == 0) {
            אם (++i == 16) {
                اكسر;
            }

            bit = 0x80;
        }
    }

    node->value = value;

    החזר NGX_OK;
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

    כלעוד (node && (bit & mask[i])) {
        אם (key[i] & bit) {
            node = node->right;

        } אחרת {
            node = node->left;
        }

        bit >>= 1;

        אם (bit == 0) {
            אם (++i == 16) {
                اكسر;
            }

            bit = 0x80;
        }
    }

    אם (node == عدم) {
        החזר NGX_ERROR;
    }

    אם (node->right || node->left) {
        אם (node->value != NGX_RADIX_NO_VALUE) {
            node->value = NGX_RADIX_NO_VALUE;
            החזר NGX_OK;
        }

        החזר NGX_ERROR;
    }

    עבור ( ;; ) {
        אם (node->parent->right == node) {
            node->parent->right = عدم;

        } אחרת {
            node->parent->left = عدم;
        }

        node->right = tree->حرر;
        tree->حرر = node;

        node = node->parent;

        אם (node->right || node->left) {
            اكسر;
        }

        אם (node->value != NGX_RADIX_NO_VALUE) {
            اكسر;
        }

        אם (node->parent == عدم) {
            اكسر;
        }
    }

    החזר NGX_OK;
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

    כלעוד (node) {
        אם (node->value != NGX_RADIX_NO_VALUE) {
            value = node->value;
        }

        אם (key[i] & bit) {
            node = node->right;

        } אחרת {
            node = node->left;
        }

        bit >>= 1;

        אם (bit == 0) {
            i++;
            bit = 0x80;
        }
    }

    החזר value;
}

#endif


ساكن ngx_radix_node_t *
ngx_radix_alloc(ngx_radix_tree_t *tree)
{
    ngx_radix_node_t  *p;

    אם (tree->حرر) {
        p = tree->حرر;
        tree->حرر = tree->حرر->right;
        החזר p;
    }

    אם (tree->size < حجم(ngx_radix_node_t)) {
        tree->start = ngx_pmemalign(tree->pool, ngx_pagesize, ngx_pagesize);
        אם (tree->start == عدم) {
            החזר عدم;
        }

        tree->size = ngx_pagesize;
    }

    p = (ngx_radix_node_t *) tree->start;
    tree->start += حجم(ngx_radix_node_t);
    tree->size -= حجم(ngx_radix_node_t);

    החזר p;
}
