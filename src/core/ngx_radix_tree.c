
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
    𐡄𐡍 (tree == عدم) {
        أرجع عدم;
    }

    tree->pool = pool;
    tree->حرر = عدم;
    tree->start = عدم;
    tree->size = 0;

    tree->root = ngx_radix_alloc(tree);
    𐡄𐡍 (tree->root == عدم) {
        أرجع عدم;
    }

    tree->root->right = عدم;
    tree->root->left = عدم;
    tree->root->parent = عدم;
    tree->root->value = NGX_RADIX_NO_VALUE;

    𐡄𐡍 (preallocate == 0) {
        أرجع tree;
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

    𐡄𐡍 (preallocate == -1) {
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

    طالما (preallocate--) {

        key = 0;
        mask >>= 1;
        mask |= 0x80000000;

        𐡏𐡁𐡃 {
            𐡄𐡍 (ngx_radix32tree_insert(tree, key, mask, NGX_RADIX_NO_VALUE)
                != NGX_OK)
            {
                أرجع عدم;
            }

            key += inc;

        } طالما (key);

        inc >>= 1;
    }

    أرجع tree;
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

    طالما (bit & mask) {
        𐡄𐡍 (key & bit) {
            next = node->right;

        } وإلا {
            next = node->left;
        }

        𐡄𐡍 (next == عدم) {
            اكسر;
        }

        bit >>= 1;
        node = next;
    }

    𐡄𐡍 (next) {
        𐡄𐡍 (node->value != NGX_RADIX_NO_VALUE) {
            أرجع NGX_BUSY;
        }

        node->value = value;
        أرجع NGX_OK;
    }

    طالما (bit & mask) {
        next = ngx_radix_alloc(tree);
        𐡄𐡍 (next == عدم) {
            أرجع NGX_ERROR;
        }

        next->right = عدم;
        next->left = عدم;
        next->parent = node;
        next->value = NGX_RADIX_NO_VALUE;

        𐡄𐡍 (key & bit) {
            node->right = next;

        } وإلا {
            node->left = next;
        }

        bit >>= 1;
        node = next;
    }

    node->value = value;

    أرجع NGX_OK;
}


ngx_int_t
ngx_radix32tree_delete(ngx_radix_tree_t *tree, uint32_t key, uint32_t mask)
{
    uint32_t           bit;
    ngx_radix_node_t  *node;

    bit = 0x80000000;
    node = tree->root;

    طالما (node && (bit & mask)) {
        𐡄𐡍 (key & bit) {
            node = node->right;

        } وإلا {
            node = node->left;
        }

        bit >>= 1;
    }

    𐡄𐡍 (node == عدم) {
        أرجع NGX_ERROR;
    }

    𐡄𐡍 (node->right || node->left) {
        𐡄𐡍 (node->value != NGX_RADIX_NO_VALUE) {
            node->value = NGX_RADIX_NO_VALUE;
            أرجع NGX_OK;
        }

        أرجع NGX_ERROR;
    }

    لكل ( ;; ) {
        𐡄𐡍 (node->parent->right == node) {
            node->parent->right = عدم;

        } وإلا {
            node->parent->left = عدم;
        }

        node->right = tree->حرر;
        tree->حرر = node;

        node = node->parent;

        𐡄𐡍 (node->right || node->left) {
            اكسر;
        }

        𐡄𐡍 (node->value != NGX_RADIX_NO_VALUE) {
            اكسر;
        }

        𐡄𐡍 (node->parent == عدم) {
            اكسر;
        }
    }

    أرجع NGX_OK;
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

    طالما (node) {
        𐡄𐡍 (node->value != NGX_RADIX_NO_VALUE) {
            value = node->value;
        }

        𐡄𐡍 (key & bit) {
            node = node->right;

        } وإلا {
            node = node->left;
        }

        bit >>= 1;
    }

    أرجع value;
}


#𐡄𐡍 (NGX_HAVE_INET6)

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

    طالما (bit & mask[i]) {
        𐡄𐡍 (key[i] & bit) {
            next = node->right;

        } وإلا {
            next = node->left;
        }

        𐡄𐡍 (next == عدم) {
            اكسر;
        }

        bit >>= 1;
        node = next;

        𐡄𐡍 (bit == 0) {
            𐡄𐡍 (++i == 16) {
                اكسر;
            }

            bit = 0x80;
        }
    }

    𐡄𐡍 (next) {
        𐡄𐡍 (node->value != NGX_RADIX_NO_VALUE) {
            أرجع NGX_BUSY;
        }

        node->value = value;
        أرجع NGX_OK;
    }

    طالما (bit & mask[i]) {
        next = ngx_radix_alloc(tree);
        𐡄𐡍 (next == عدم) {
            أرجع NGX_ERROR;
        }

        next->right = عدم;
        next->left = عدم;
        next->parent = node;
        next->value = NGX_RADIX_NO_VALUE;

        𐡄𐡍 (key[i] & bit) {
            node->right = next;

        } وإلا {
            node->left = next;
        }

        bit >>= 1;
        node = next;

        𐡄𐡍 (bit == 0) {
            𐡄𐡍 (++i == 16) {
                اكسر;
            }

            bit = 0x80;
        }
    }

    node->value = value;

    أرجع NGX_OK;
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

    طالما (node && (bit & mask[i])) {
        𐡄𐡍 (key[i] & bit) {
            node = node->right;

        } وإلا {
            node = node->left;
        }

        bit >>= 1;

        𐡄𐡍 (bit == 0) {
            𐡄𐡍 (++i == 16) {
                اكسر;
            }

            bit = 0x80;
        }
    }

    𐡄𐡍 (node == عدم) {
        أرجع NGX_ERROR;
    }

    𐡄𐡍 (node->right || node->left) {
        𐡄𐡍 (node->value != NGX_RADIX_NO_VALUE) {
            node->value = NGX_RADIX_NO_VALUE;
            أرجع NGX_OK;
        }

        أرجع NGX_ERROR;
    }

    لكل ( ;; ) {
        𐡄𐡍 (node->parent->right == node) {
            node->parent->right = عدم;

        } وإلا {
            node->parent->left = عدم;
        }

        node->right = tree->حرر;
        tree->حرر = node;

        node = node->parent;

        𐡄𐡍 (node->right || node->left) {
            اكسر;
        }

        𐡄𐡍 (node->value != NGX_RADIX_NO_VALUE) {
            اكسر;
        }

        𐡄𐡍 (node->parent == عدم) {
            اكسر;
        }
    }

    أرجع NGX_OK;
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

    طالما (node) {
        𐡄𐡍 (node->value != NGX_RADIX_NO_VALUE) {
            value = node->value;
        }

        𐡄𐡍 (key[i] & bit) {
            node = node->right;

        } وإلا {
            node = node->left;
        }

        bit >>= 1;

        𐡄𐡍 (bit == 0) {
            i++;
            bit = 0x80;
        }
    }

    أرجع value;
}

#endif


ساكن ngx_radix_node_t *
ngx_radix_alloc(ngx_radix_tree_t *tree)
{
    ngx_radix_node_t  *p;

    𐡄𐡍 (tree->حرر) {
        p = tree->حرر;
        tree->حرر = tree->حرر->right;
        أرجع p;
    }

    𐡄𐡍 (tree->size < حجم(ngx_radix_node_t)) {
        tree->start = ngx_pmemalign(tree->pool, ngx_pagesize, ngx_pagesize);
        𐡄𐡍 (tree->start == عدم) {
            أرجع عدم;
        }

        tree->size = ngx_pagesize;
    }

    p = (ngx_radix_node_t *) tree->start;
    tree->start += حجم(ngx_radix_node_t);
    tree->size -= حجم(ngx_radix_node_t);

    أرجع p;
}
