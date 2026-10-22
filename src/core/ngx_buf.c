
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


ngx_buf_t *
ngx_create_عارضی_buf(ngx_pool_t *pool, سائز_t سائز)
{
    ngx_buf_t *b;

    b = ngx_calloc_buf(pool);
    اگر (b == تہی) {
        واپس تہی;
    }

    b->آغاز = ngx_palloc(pool, سائز);
    اگر (b->آغاز == تہی) {
        واپس تہی;
    }

    /*
     * set by ngx_calloc_buf():
     *
     *     b->file_pos = 0;
     *     b->file_last = 0;
     *     b->file = NULL;
     *     b->shadow = NULL;
     *     b->tag = 0;
     *     and flags
     */

    b->pos = b->آغاز;
    b->آخری = b->آغاز;
    b->انتہا = b->آخری + سائز;
    b->temporary = 1;

    واپس b;
}


ngx_chain_t *
ngx_alloc_chain_link(ngx_pool_t *pool)
{
    ngx_chain_t  *cl;

    cl = pool->chain;

    اگر (cl) {
        pool->chain = cl->اگلا;
        واپس cl;
    }

    cl = ngx_palloc(pool, حجم(ngx_chain_t));
    اگر (cl == تہی) {
        واپس تہی;
    }

    واپس cl;
}


ngx_chain_t *
ngx_create_chain_کا_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs)
{
    u_char       *p;
    ngx_int_t     i;
    ngx_buf_t    *b;
    ngx_chain_t  *chain, *cl, **ll;

    p = ngx_palloc(pool, bufs->عدد * bufs->سائز);
    اگر (p == تہی) {
        واپس تہی;
    }

    ll = &chain;

    برائے (i = 0; i < bufs->عدد; i++) {

        b = ngx_calloc_buf(pool);
        اگر (b == تہی) {
            واپس تہی;
        }

        /*
         * set by ngx_calloc_buf():
         *
         *     b->file_pos = 0;
         *     b->file_last = 0;
         *     b->file = NULL;
         *     b->shadow = NULL;
         *     b->tag = 0;
         *     and flags
         *
         */

        b->pos = p;
        b->آخری = p;
        b->temporary = 1;

        b->آغاز = p;
        p += bufs->سائز;
        b->انتہا = p;

        cl = ngx_alloc_chain_link(pool);
        اگر (cl == تہی) {
            واپس تہی;
        }

        cl->buf = b;
        *ll = cl;
        ll = &cl->اگلا;
    }

    *ll = تہی;

    واپس chain;
}


ngx_int_t
ngx_chain_جوڑو_copy(ngx_pool_t *pool, ngx_chain_t **chain, ngx_chain_t *اندر)
{
    ngx_chain_t  *cl, **ll;

    ll = chain;

    برائے (cl = *chain; cl; cl = cl->اگلا) {
        ll = &cl->اگلا;
    }

    جب (اندر) {
        cl = ngx_alloc_chain_link(pool);
        اگر (cl == تہی) {
            *ll = تہی;
            واپس NGX_ERROR;
        }

        cl->buf = اندر->buf;
        *ll = cl;
        ll = &cl->اگلا;
        اندر = اندر->اگلا;
    }

    *ll = تہی;

    واپس NGX_OK;
}


ngx_chain_t *
ngx_chain_get_free_buf(ngx_pool_t *p, ngx_chain_t **آزاد)
{
    ngx_chain_t  *cl;

    اگر (*آزاد) {
        cl = *آزاد;
        *آزاد = cl->اگلا;
        cl->اگلا = تہی;
        واپس cl;
    }

    cl = ngx_alloc_chain_link(p);
    اگر (cl == تہی) {
        واپس تہی;
    }

    cl->buf = ngx_calloc_buf(p);
    اگر (cl->buf == تہی) {
        واپس تہی;
    }

    cl->اگلا = تہی;

    واپس cl;
}


خالی
ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **آزاد, ngx_chain_t **busy,
    ngx_chain_t **برآمد, ngx_buf_لیبل_t لیبل)
{
    ngx_chain_t  *cl;

    اگر (*برآمد) {
        اگر (*busy == تہی) {
            *busy = *برآمد;

        } ورنہ {
            برائے (cl = *busy; cl->اگلا; cl = cl->اگلا) { /* void */ }

            cl->اگلا = *برآمد;
        }

        *برآمد = تہی;
    }

    جب (*busy) {
        cl = *busy;

        اگر (cl->buf->لیبل != لیبل) {
            *busy = cl->اگلا;
            ngx_free_chain(p, cl);
            جاری;
        }

        اگر (ngx_buf_سائز(cl->buf) != 0) {
            توڑو;
        }

        cl->buf->pos = cl->buf->آغاز;
        cl->buf->آخری = cl->buf->آغاز;

        *busy = cl->اگلا;
        cl->اگلا = *آزاد;
        *آزاد = cl;
    }
}


off_t
ngx_chain_coalesce_file(ngx_chain_t **اندر, off_t حد_بندی)
{
    off_t         کل, سائز, aligned, fprev;
    ngx_fd_t      fd;
    ngx_chain_t  *cl;

    کل = 0;

    cl = *اندر;
    fd = cl->buf->file->fd;

    کرو {
        سائز = cl->buf->file_آخری - cl->buf->file_pos;

        اگر (سائز > حد_بندی - کل) {
            سائز = حد_بندی - کل;

            aligned = (cl->buf->file_pos + سائز + ngx_pagesize - 1)
                       & ~((off_t) ngx_pagesize - 1);

            اگر (aligned <= cl->buf->file_آخری) {
                سائز = aligned - cl->buf->file_pos;
            }

            کل += سائز;
            توڑو;
        }

        کل += سائز;
        fprev = cl->buf->file_pos + سائز;
        cl = cl->اگلا;

    } جب (cl
             && cl->buf->اندر_file
             && کل < حد_بندی
             && fd == cl->buf->file->fd
             && fprev == cl->buf->file_pos);

    *اندر = cl;

    واپس کل;
}


ngx_chain_t *
ngx_chain_update_بھیجا(ngx_chain_t *اندر, off_t بھیجا)
{
    off_t  سائز;

    برائے ( /* void */ ; اندر; اندر = اندر->اگلا) {

        اگر (ngx_buf_special(اندر->buf)) {
            جاری;
        }

        اگر (بھیجا == 0) {
            توڑو;
        }

        سائز = ngx_buf_سائز(اندر->buf);

        اگر (بھیجا >= سائز) {
            بھیجا -= سائز;

            اگر (ngx_buf_اندر_حافظہ(اندر->buf)) {
                اندر->buf->pos = اندر->buf->آخری;
            }

            اگر (اندر->buf->اندر_file) {
                اندر->buf->file_pos = اندر->buf->file_آخری;
            }

            جاری;
        }

        اگر (ngx_buf_اندر_حافظہ(اندر->buf)) {
            اندر->buf->pos += (سائز_t) بھیجا;
        }

        اگر (اندر->buf->اندر_file) {
            اندر->buf->file_pos += بھیجا;
        }

        توڑو;
    }

    واپس اندر;
}
