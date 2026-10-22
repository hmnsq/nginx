
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_event.h>


#define NGX_RESOLVER_UDP_SIZE   4096

#define NGX_RESOLVER_TCP_RSIZE  (2 + 65535)
#define NGX_RESOLVER_TCP_WSIZE  8192


typedef struct {
    u_char  ident_hi;
    u_char  ident_lo;
    u_char  flags_hi;
    u_char  flags_lo;
    u_char  nqs_hi;
    u_char  nqs_lo;
    u_char  nan_hi;
    u_char  nan_lo;
    u_char  nns_hi;
    u_char  nns_lo;
    u_char  nar_hi;
    u_char  nar_lo;
} ngx_resolver_hdr_t;


typedef struct {
    u_char  type_hi;
    u_char  type_lo;
    u_char  class_hi;
    u_char  class_lo;
} ngx_resolver_qs_t;


typedef struct {
    u_char  type_hi;
    u_char  type_lo;
    u_char  class_hi;
    u_char  class_lo;
    u_char  ttl[4];
    u_char  len_hi;
    u_char  len_lo;
} ngx_resolver_an_t;


#define ngx_resolver_node(n)  ngx_rbtree_data(n, ngx_resolver_node_t, node)


static ngx_int_t ngx_udp_connect(ngx_resolver_connection_t *rec);
static ngx_int_t ngx_tcp_connect(ngx_resolver_connection_t *rec);


static void ngx_resolver_cleanup(void *data);
static void ngx_resolver_cleanup_tree(ngx_resolver_t *r, ngx_rbtree_t *tree);
static ngx_int_t ngx_resolve_name_locked(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx, ngx_str_t *name);
static void ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *tree,
    ngx_queue_t *queue);
static ngx_int_t ngx_resolver_send_query(ngx_resolver_t *r,
    ngx_resolver_node_t *rn);
static ngx_int_t ngx_resolver_send_udp_query(ngx_resolver_t *r,
    ngx_resolver_connection_t *rec, u_char *query, u_short qlen);
static ngx_int_t ngx_resolver_send_tcp_query(ngx_resolver_t *r,
    ngx_resolver_connection_t *rec, u_char *query, u_short qlen);
static ngx_int_t ngx_resolver_create_name_query(ngx_resolver_t *r,
    ngx_resolver_node_t *rn, ngx_str_t *name);
static ngx_int_t ngx_resolver_create_srv_query(ngx_resolver_t *r,
    ngx_resolver_node_t *rn, ngx_str_t *name);
static ngx_int_t ngx_resolver_create_addr_query(ngx_resolver_t *r,
    ngx_resolver_node_t *rn, ngx_resolver_addr_t *addr);
static void ngx_resolver_resend_handler(ngx_event_t *ev);
static time_t ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *tree,
    ngx_queue_t *queue);
static ngx_uint_t ngx_resolver_resend_empty(ngx_resolver_t *r);
static void ngx_resolver_udp_read(ngx_event_t *rev);
static void ngx_resolver_tcp_write(ngx_event_t *wev);
static void ngx_resolver_tcp_read(ngx_event_t *rev);
static void ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf,
    size_t n, ngx_uint_t tcp);
static void ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans);
static void ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans);
static void ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan);
static ngx_resolver_node_t *ngx_resolver_lookup_name(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
static ngx_resolver_node_t *ngx_resolver_lookup_srv(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
static ngx_resolver_node_t *ngx_resolver_lookup_addr(ngx_resolver_t *r,
    in_addr_t addr);
static void ngx_resolver_rbtree_insert_value(ngx_rbtree_node_t *temp,
    ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel);
static ngx_int_t ngx_resolver_copy(ngx_resolver_t *r, ngx_str_t *name,
    u_char *buf, u_char *src, u_char *last);
static ngx_int_t ngx_resolver_set_timeout(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx);
static void ngx_resolver_timeout_handler(ngx_event_t *ev);
static void ngx_resolver_free_node(ngx_resolver_t *r, ngx_resolver_node_t *rn);
static void *ngx_resolver_alloc(ngx_resolver_t *r, size_t size);
static void *ngx_resolver_calloc(ngx_resolver_t *r, size_t size);
static void ngx_resolver_free(ngx_resolver_t *r, void *p);
static void ngx_resolver_free_locked(ngx_resolver_t *r, void *p);
static void *ngx_resolver_dup(ngx_resolver_t *r, void *src, size_t size);
static ngx_resolver_addr_t *ngx_resolver_export(ngx_resolver_t *r,
    ngx_resolver_node_t *rn, ngx_uint_t rotate);
static void ngx_resolver_report_srv(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx);
static u_char *ngx_resolver_log_error(ngx_log_t *log, u_char *buf, size_t len);
static void ngx_resolver_resolve_srv_names(ngx_resolver_ctx_t *ctx,
    ngx_resolver_node_t *rn);
static void ngx_resolver_srv_names_handler(ngx_resolver_ctx_t *ctx);
static ngx_int_t ngx_resolver_cmp_srvs(const void *one, const void *two);

#如果 (NGX_HAVE_INET6)
static void ngx_resolver_rbtree_insert_addr6_value(ngx_rbtree_node_t *temp,
    ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel);
static ngx_resolver_node_t *ngx_resolver_lookup_addr6(ngx_resolver_t *r,
    struct in6_addr *addr, uint32_t hash);
#endif


ngx_resolver_t *
ngx_resolver_create(ngx_conf_t *cf, ngx_str_t *names, ngx_uint_t n)
{
    ngx_str_t                   s;
    ngx_url_t                   u;
    ngx_uint_t                  i, j;
    ngx_resolver_t             *r;
    ngx_pool_cleanup_t         *cln;
    ngx_resolver_connection_t  *rec;

    r = ngx_pcalloc(cf->pool, sizeof(ngx_resolver_t));
    如果 (r == NULL) {
        返回 NULL;
    }

    r->event = ngx_pcalloc(cf->pool, sizeof(ngx_event_t));
    如果 (r->event == NULL) {
        返回 NULL;
    }

    cln = ngx_pool_cleanup_add(cf->pool, 0);
    如果 (cln == NULL) {
        返回 NULL;
    }

    cln->handler = ngx_resolver_cleanup;
    cln->data = r;

    r->ipv4 = 1;

    ngx_rbtree_init(&r->name_rbtree, &r->name_sentinel,
                    ngx_resolver_rbtree_insert_value);

    ngx_rbtree_init(&r->srv_rbtree, &r->srv_sentinel,
                    ngx_resolver_rbtree_insert_value);

    ngx_rbtree_init(&r->addr_rbtree, &r->addr_sentinel,
                    ngx_rbtree_insert_value);

    ngx_queue_init(&r->name_resend_queue);
    ngx_queue_init(&r->srv_resend_queue);
    ngx_queue_init(&r->addr_resend_queue);

    ngx_queue_init(&r->name_expire_queue);
    ngx_queue_init(&r->srv_expire_queue);
    ngx_queue_init(&r->addr_expire_queue);

#如果 (NGX_HAVE_INET6)
    r->ipv6 = 1;

    ngx_rbtree_init(&r->addr6_rbtree, &r->addr6_sentinel,
                    ngx_resolver_rbtree_insert_addr6_value);

    ngx_queue_init(&r->addr6_resend_queue);

    ngx_queue_init(&r->addr6_expire_queue);
#endif

    r->event->handler = ngx_resolver_resend_handler;
    r->event->data = r;
    r->event->log = &cf->cycle->new_log;
    r->event->cancelable = 1;
    r->ident = -1;

    r->resend_timeout = 5;
    r->tcp_timeout = 5;
    r->expire = 30;
    r->valid = 0;

    r->log = &cf->cycle->new_log;
    r->log_level = NGX_LOG_ERR;

    如果 (n) {
        如果 (ngx_array_init(&r->connections, cf->pool, n,
                           sizeof(ngx_resolver_connection_t))
            != NGX_OK)
        {
            返回 NULL;
        }
    }

    为 (i = 0; i < n; i++) {
        如果 (ngx_strncmp(names[i].data, "valid=", 6) == 0) {
            s.len = names[i].len - 6;
            s.data = names[i].data + 6;

            r->valid = ngx_parse_time(&s, 1);

            如果 (r->valid == (time_t) NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &names[i]);
                返回 NULL;
            }

            continue;
        }

#如果 (NGX_HAVE_INET6)
        如果 (ngx_strncmp(names[i].data, "ipv4=", 5) == 0) {

            如果 (ngx_strcmp(&names[i].data[5], "on") == 0) {
                r->ipv4 = 1;

            } 否则 如果 (ngx_strcmp(&names[i].data[5], "off") == 0) {
                r->ipv4 = 0;

            } 否则 {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &names[i]);
                返回 NULL;
            }

            continue;
        }

        如果 (ngx_strncmp(names[i].data, "ipv6=", 5) == 0) {

            如果 (ngx_strcmp(&names[i].data[5], "on") == 0) {
                r->ipv6 = 1;

            } 否则 如果 (ngx_strcmp(&names[i].data[5], "off") == 0) {
                r->ipv6 = 0;

            } 否则 {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &names[i]);
                返回 NULL;
            }

            continue;
        }
#endif

        ngx_memzero(&u, sizeof(ngx_url_t));

        u.url = names[i];
        u.default_port = 53;

        如果 (ngx_parse_url(cf->pool, &u) != NGX_OK) {
            如果 (u.err) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "%s in resolver \"%V\"",
                                   u.err, &u.url);
            }

            返回 NULL;
        }

        rec = ngx_array_push_n(&r->connections, u.naddrs);
        如果 (rec == NULL) {
            返回 NULL;
        }

        ngx_memzero(rec, u.naddrs * sizeof(ngx_resolver_connection_t));

        为 (j = 0; j < u.naddrs; j++) {
            rec[j].sockaddr = u.addrs[j].sockaddr;
            rec[j].socklen = u.addrs[j].socklen;
            rec[j].server = u.addrs[j].name;
            rec[j].resolver = r;
        }
    }

#如果 (NGX_HAVE_INET6)
    如果 (r->ipv4 + r->ipv6 == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"ipv4\" and \"ipv6\" cannot both be \"off\"");
        返回 NULL;
    }
#endif

    如果 (n && r->connections.nelts == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "no name servers defined");
        返回 NULL;
    }

    返回 r;
}


static void
ngx_resolver_cleanup(void *data)
{
    ngx_resolver_t  *r = data;

    ngx_uint_t                  i;
    ngx_resolver_connection_t  *rec;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "cleanup resolver");

    ngx_resolver_cleanup_tree(r, &r->name_rbtree);

    ngx_resolver_cleanup_tree(r, &r->srv_rbtree);

    ngx_resolver_cleanup_tree(r, &r->addr_rbtree);

#如果 (NGX_HAVE_INET6)
    ngx_resolver_cleanup_tree(r, &r->addr6_rbtree);
#endif

    如果 (r->event->timer_set) {
        ngx_del_timer(r->event);
    }

    rec = r->connections.elts;

    为 (i = 0; i < r->connections.nelts; i++) {
        如果 (rec[i].udp) {
            ngx_close_connection(rec[i].udp);
        }

        如果 (rec[i].tcp) {
            ngx_close_connection(rec[i].tcp);
        }

        如果 (rec[i].read_buf) {
            ngx_resolver_free(r, rec[i].read_buf->start);
            ngx_resolver_free(r, rec[i].read_buf);
        }

        如果 (rec[i].write_buf) {
            ngx_resolver_free(r, rec[i].write_buf->start);
            ngx_resolver_free(r, rec[i].write_buf);
        }
    }
}


static void
ngx_resolver_cleanup_tree(ngx_resolver_t *r, ngx_rbtree_t *tree)
{
    ngx_resolver_ctx_t   *ctx, *next;
    ngx_resolver_node_t  *rn;

    当 (tree->root != tree->sentinel) {

        rn = ngx_resolver_node(ngx_rbtree_min(tree->root, tree->sentinel));

        ngx_queue_remove(&rn->queue);

        为 (ctx = rn->waiting; ctx; ctx = next) {
            next = ctx->next;

            如果 (ctx->event) {
                如果 (ctx->event->timer_set) {
                    ngx_del_timer(ctx->event);
                }

                ngx_resolver_free(r, ctx->event);
            }

            ngx_resolver_free(r, ctx);
        }

        ngx_rbtree_delete(tree, &rn->node);

        ngx_resolver_free_node(r, rn);
    }
}


ngx_resolver_ctx_t *
ngx_resolve_start(ngx_resolver_t *r, ngx_resolver_ctx_t *temp)
{
    in_addr_t            addr;
    ngx_resolver_ctx_t  *ctx;

    如果 (temp) {
        addr = ngx_inet_addr(temp->name.data, temp->name.len);

        如果 (addr != INADDR_NONE) {
            temp->resolver = r;
            temp->state = NGX_OK;
            temp->naddrs = 1;
            temp->addrs = &temp->addr;
            temp->addr.sockaddr = (struct sockaddr *) &temp->sin;
            temp->addr.socklen = sizeof(struct sockaddr_in);
            ngx_memzero(&temp->sin, sizeof(struct sockaddr_in));
            temp->sin.sin_family = AF_INET;
            temp->sin.sin_addr.s_addr = addr;
            temp->quick = 1;

            返回 temp;
        }
    }

    如果 (r->connections.nelts == 0) {
        返回 NGX_NO_RESOLVER;
    }

    ctx = ngx_resolver_calloc(r, sizeof(ngx_resolver_ctx_t));

    如果 (ctx) {
        ctx->resolver = r;
    }

    返回 ctx;
}


ngx_int_t
ngx_resolve_name(ngx_resolver_ctx_t *ctx)
{
    size_t           slen;
    ngx_int_t        rc;
    ngx_str_t        name;
    ngx_resolver_t  *r;

    r = ctx->resolver;

    如果 (ctx->name.len > 0 && ctx->name.data[ctx->name.len - 1] == '.') {
        ctx->name.len--;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve: \"%V\"", &ctx->name);

    如果 (ctx->quick) {
        ctx->handler(ctx);
        返回 NGX_OK;
    }

    如果 (ctx->service.len) {
        slen = ctx->service.len;

        如果 (ngx_strlchr(ctx->service.data,
                        ctx->service.data + ctx->service.len, '.')
            == NULL)
        {
            slen += sizeof("_._tcp") - 1;
        }

        name.len = slen + 1 + ctx->name.len;

        name.data = ngx_resolver_alloc(r, name.len);
        如果 (name.data == NULL) {
            goto failed;
        }

        如果 (slen == ctx->service.len) {
            ngx_sprintf(name.data, "%V.%V", &ctx->service, &ctx->name);

        } 否则 {
            ngx_sprintf(name.data, "_%V._tcp.%V", &ctx->service, &ctx->name);
        }

        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &name);

        ngx_resolver_free(r, name.data);

    } 否则 {
        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &ctx->name);
    }

    如果 (rc == NGX_OK) {
        返回 NGX_OK;
    }

    /* unlock name mutex */

    如果 (rc == NGX_AGAIN) {
        返回 NGX_OK;
    }

    /* NGX_ERROR */

    如果 (ctx->event) {
        ngx_resolver_free(r, ctx->event);
    }

failed:

    ngx_resolver_free(r, ctx);

    返回 NGX_ERROR;
}


void
ngx_resolve_name_done(ngx_resolver_ctx_t *ctx)
{
    ngx_uint_t            i;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_node_t  *rn;

    r = ctx->resolver;

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve name done: %i", ctx->state);

    如果 (ctx->quick) {
        返回;
    }

    如果 (ctx->event && ctx->event->timer_set) {
        ngx_del_timer(ctx->event);
    }

    /* lock name mutex */

    如果 (ctx->nsrvs) {
        为 (i = 0; i < ctx->nsrvs; i++) {
            如果 (ctx->srvs[i].ctx) {
                ngx_resolve_name_done(ctx->srvs[i].ctx);
            }

            如果 (ctx->srvs[i].addrs) {
                ngx_resolver_free(r, ctx->srvs[i].addrs->sockaddr);
                ngx_resolver_free(r, ctx->srvs[i].addrs);
            }

            ngx_resolver_free(r, ctx->srvs[i].name.data);
        }

        ngx_resolver_free(r, ctx->srvs);
    }

    如果 (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->node;

        如果 (rn) {
            p = &rn->waiting;
            w = rn->waiting;

            当 (w) {
                如果 (w == ctx) {
                    *p = w->next;

                    goto done;
                }

                p = &w->next;
                w = w->next;
            }

            ngx_log_error(NGX_LOG_ALERT, r->log, 0,
                          "could not cancel %V resolving", &ctx->name);
        }
    }

done:

    如果 (ctx->service.len) {
        ngx_resolver_expire(r, &r->srv_rbtree, &r->srv_expire_queue);

    } 否则 {
        ngx_resolver_expire(r, &r->name_rbtree, &r->name_expire_queue);
    }

    /* unlock name mutex */

    /* lock alloc mutex */

    如果 (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    如果 (r->event->timer_set && ngx_resolver_resend_empty(r)) {
        ngx_del_timer(r->event);
    }
}


static ngx_int_t
ngx_resolve_name_locked(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx,
    ngx_str_t *name)
{
    uint32_t              hash;
    ngx_int_t             rc;
    ngx_str_t             cname;
    ngx_uint_t            i, naddrs;
    ngx_queue_t          *resend_queue, *expire_queue;
    ngx_rbtree_t         *tree;
    ngx_resolver_ctx_t   *next, *last;
    ngx_resolver_addr_t  *addrs;
    ngx_resolver_node_t  *rn;

    ngx_strlow(name->data, name->data, name->len);

    hash = ngx_crc32_short(name->data, name->len);

    如果 (ctx->service.len) {
        rn = ngx_resolver_lookup_srv(r, name, hash);

        tree = &r->srv_rbtree;
        resend_queue = &r->srv_resend_queue;
        expire_queue = &r->srv_expire_queue;

    } 否则 {
        rn = ngx_resolver_lookup_name(r, name, hash);

        tree = &r->name_rbtree;
        resend_queue = &r->name_resend_queue;
        expire_queue = &r->name_expire_queue;
    }

    如果 (rn) {

        /* ctx can be a list after NGX_RESOLVE_CNAME */
        为 (last = ctx; last->next; last = last->next);

        如果 (rn->valid >= ngx_time()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0, "resolve cached");

            ngx_queue_remove(&rn->queue);

            rn->expire = ngx_time() + r->expire;

            ngx_queue_insert_head(expire_queue, &rn->queue);

            naddrs = (rn->naddrs == (u_short) -1) ? 0 : rn->naddrs;
#如果 (NGX_HAVE_INET6)
            naddrs += (rn->naddrs6 == (u_short) -1) ? 0 : rn->naddrs6;
#endif

            如果 (naddrs) {

                如果 (naddrs == 1 && rn->naddrs == 1) {
                    addrs = NULL;

                } 否则 {
                    addrs = ngx_resolver_export(r, rn, 1);
                    如果 (addrs == NULL) {
                        返回 NGX_ERROR;
                    }
                }

                last->next = rn->waiting;
                rn->waiting = NULL;

                /* unlock name mutex */

                do {
                    ctx->state = NGX_OK;
                    ctx->valid = rn->valid;
                    ctx->naddrs = naddrs;

                    如果 (addrs == NULL) {
                        ctx->addrs = &ctx->addr;
                        ctx->addr.sockaddr = (struct sockaddr *) &ctx->sin;
                        ctx->addr.socklen = sizeof(struct sockaddr_in);
                        ngx_memzero(&ctx->sin, sizeof(struct sockaddr_in));
                        ctx->sin.sin_family = AF_INET;
                        ctx->sin.sin_addr.s_addr = rn->u.addr;

                    } 否则 {
                        ctx->addrs = addrs;
                    }

                    next = ctx->next;

                    ctx->handler(ctx);

                    ctx = next;
                } 当 (ctx);

                如果 (addrs != NULL) {
                    ngx_resolver_free(r, addrs->sockaddr);
                    ngx_resolver_free(r, addrs);
                }

                返回 NGX_OK;
            }

            如果 (rn->nsrvs) {
                last->next = rn->waiting;
                rn->waiting = NULL;

                /* unlock name mutex */

                do {
                    next = ctx->next;

                    ngx_resolver_resolve_srv_names(ctx, rn);

                    ctx = next;
                } 当 (ctx);

                返回 NGX_OK;
            }

            /* NGX_RESOLVE_CNAME */

            如果 (ctx->recursion++ < NGX_RESOLVER_MAX_RECURSION) {

                cname.len = rn->cnlen;
                cname.data = rn->u.cname;

                返回 ngx_resolve_name_locked(r, ctx, &cname);
            }

            last->next = rn->waiting;
            rn->waiting = NULL;

            /* unlock name mutex */

            do {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
                next = ctx->next;

                ctx->handler(ctx);

                ctx = next;
            } 当 (ctx);

            返回 NGX_OK;
        }

        如果 (rn->waiting) {
            如果 (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
                返回 NGX_ERROR;
            }

            last->next = rn->waiting;
            rn->waiting = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;

            do {
                ctx->node = rn;
                ctx = ctx->next;
            } 当 (ctx);

            返回 NGX_AGAIN;
        }

        ngx_queue_remove(&rn->queue);

        /* lock alloc mutex */

        如果 (rn->query) {
            ngx_resolver_free_locked(r, rn->query);
            rn->query = NULL;
#如果 (NGX_HAVE_INET6)
            rn->query6 = NULL;
#endif
        }

        如果 (rn->cnlen) {
            ngx_resolver_free_locked(r, rn->u.cname);
        }

        如果 (rn->naddrs > 1 && rn->naddrs != (u_short) -1) {
            ngx_resolver_free_locked(r, rn->u.addrs);
        }

#如果 (NGX_HAVE_INET6)
        如果 (rn->naddrs6 > 1 && rn->naddrs6 != (u_short) -1) {
            ngx_resolver_free_locked(r, rn->u6.addrs6);
        }
#endif

        如果 (rn->nsrvs) {
            为 (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
                如果 (rn->u.srvs[i].name.data) {
                    ngx_resolver_free_locked(r, rn->u.srvs[i].name.data);
                }
            }

            ngx_resolver_free_locked(r, rn->u.srvs);
        }

        /* unlock alloc mutex */

    } 否则 {

        rn = ngx_resolver_alloc(r, sizeof(ngx_resolver_node_t));
        如果 (rn == NULL) {
            返回 NGX_ERROR;
        }

        rn->name = ngx_resolver_dup(r, name->data, name->len);
        如果 (rn->name == NULL) {
            ngx_resolver_free(r, rn);
            返回 NGX_ERROR;
        }

        rn->node.key = hash;
        rn->nlen = (u_short) name->len;
        rn->query = NULL;
#如果 (NGX_HAVE_INET6)
        rn->query6 = NULL;
#endif

        ngx_rbtree_insert(tree, &rn->node);
    }

    如果 (ctx->service.len) {
        rc = ngx_resolver_create_srv_query(r, rn, name);

    } 否则 {
        rc = ngx_resolver_create_name_query(r, rn, name);
    }

    如果 (rc == NGX_ERROR) {
        goto failed;
    }

    如果 (rc == NGX_DECLINED) {
        ngx_rbtree_delete(tree, &rn->node);

        ngx_resolver_free(r, rn->query);
        ngx_resolver_free(r, rn->name);
        ngx_resolver_free(r, rn);

        do {
            ctx->state = NGX_RESOLVE_NXDOMAIN;
            next = ctx->next;

            ctx->handler(ctx);

            ctx = next;
        } 当 (ctx);

        返回 NGX_OK;
    }

    rn->last_connection = r->last_connection++;
    如果 (r->last_connection == r->connections.nelts) {
        r->last_connection = 0;
    }

    rn->naddrs = r->ipv4 ? (u_short) -1 : 0;
    rn->tcp = 0;
#如果 (NGX_HAVE_INET6)
    rn->naddrs6 = r->ipv6 ? (u_short) -1 : 0;
    rn->tcp6 = 0;
#endif
    rn->nsrvs = 0;

    如果 (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->last_connection++;
        如果 (rn->last_connection == r->connections.nelts) {
            rn->last_connection = 0;
        }

        (void) ngx_resolver_send_query(r, rn);
    }

    如果 (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
        goto failed;
    }

    如果 (ngx_resolver_resend_empty(r)) {
        ngx_add_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_time() + r->resend_timeout;

    ngx_queue_insert_head(resend_queue, &rn->queue);

    rn->code = 0;
    rn->cnlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_VALUE;
    rn->waiting = ctx;

    ctx->state = NGX_AGAIN;
    ctx->async = 1;

    do {
        ctx->node = rn;
        ctx = ctx->next;
    } 当 (ctx);

    返回 NGX_AGAIN;

failed:

    ngx_rbtree_delete(tree, &rn->node);

    如果 (rn->query) {
        ngx_resolver_free(r, rn->query);
    }

    ngx_resolver_free(r, rn->name);

    ngx_resolver_free(r, rn);

    返回 NGX_ERROR;
}


ngx_int_t
ngx_resolve_addr(ngx_resolver_ctx_t *ctx)
{
    u_char               *name;
    in_addr_t             addr;
    ngx_queue_t          *resend_queue, *expire_queue;
    ngx_rbtree_t         *tree;
    ngx_resolver_t       *r;
    struct sockaddr_in   *sin;
    ngx_resolver_node_t  *rn;
#如果 (NGX_HAVE_INET6)
    uint32_t              hash;
    struct sockaddr_in6  *sin6;
#endif

#如果 (NGX_SUPPRESS_WARN)
    addr = 0;
#如果 (NGX_HAVE_INET6)
    hash = 0;
    sin6 = NULL;
#endif
#endif

    r = ctx->resolver;

    switch (ctx->addr.sockaddr->sa_family) {

#如果 (NGX_HAVE_INET6)
    case AF_INET6:
        sin6 = (struct sockaddr_in6 *) ctx->addr.sockaddr;
        hash = ngx_crc32_short(sin6->sin6_addr.s6_addr, 16);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr6(r, &sin6->sin6_addr, hash);

        tree = &r->addr6_rbtree;
        resend_queue = &r->addr6_resend_queue;
        expire_queue = &r->addr6_expire_queue;

        break;
#endif

    default: /* AF_INET */
        sin = (struct sockaddr_in *) ctx->addr.sockaddr;
        addr = ntohl(sin->sin_addr.s_addr);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        tree = &r->addr_rbtree;
        resend_queue = &r->addr_resend_queue;
        expire_queue = &r->addr_expire_queue;
    }

    如果 (rn) {

        如果 (rn->valid >= ngx_time()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0, "resolve cached");

            ngx_queue_remove(&rn->queue);

            rn->expire = ngx_time() + r->expire;

            ngx_queue_insert_head(expire_queue, &rn->queue);

            name = ngx_resolver_dup(r, rn->name, rn->nlen);
            如果 (name == NULL) {
                ngx_resolver_free(r, ctx);
                返回 NGX_ERROR;
            }

            ctx->name.len = rn->nlen;
            ctx->name.data = name;

            /* unlock addr mutex */

            ctx->state = NGX_OK;
            ctx->valid = rn->valid;

            ctx->handler(ctx);

            ngx_resolver_free(r, name);

            返回 NGX_OK;
        }

        如果 (rn->waiting) {
            如果 (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
                返回 NGX_ERROR;
            }

            ctx->next = rn->waiting;
            rn->waiting = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;
            ctx->node = rn;

            /* unlock addr mutex */

            返回 NGX_OK;
        }

        ngx_queue_remove(&rn->queue);

        ngx_resolver_free(r, rn->query);
        rn->query = NULL;
#如果 (NGX_HAVE_INET6)
        rn->query6 = NULL;
#endif

    } 否则 {
        rn = ngx_resolver_alloc(r, sizeof(ngx_resolver_node_t));
        如果 (rn == NULL) {
            goto failed;
        }

        switch (ctx->addr.sockaddr->sa_family) {

#如果 (NGX_HAVE_INET6)
        case AF_INET6:
            rn->addr6 = sin6->sin6_addr;
            rn->node.key = hash;
            break;
#endif

        default: /* AF_INET */
            rn->node.key = addr;
        }

        rn->query = NULL;
#如果 (NGX_HAVE_INET6)
        rn->query6 = NULL;
#endif

        ngx_rbtree_insert(tree, &rn->node);
    }

    如果 (ngx_resolver_create_addr_query(r, rn, &ctx->addr) != NGX_OK) {
        goto failed;
    }

    rn->last_connection = r->last_connection++;
    如果 (r->last_connection == r->connections.nelts) {
        r->last_connection = 0;
    }

    rn->naddrs = (u_short) -1;
    rn->tcp = 0;
#如果 (NGX_HAVE_INET6)
    rn->naddrs6 = (u_short) -1;
    rn->tcp6 = 0;
#endif
    rn->nsrvs = 0;

    如果 (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->last_connection++;
        如果 (rn->last_connection == r->connections.nelts) {
            rn->last_connection = 0;
        }

        (void) ngx_resolver_send_query(r, rn);
    }

    如果 (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
        goto failed;
    }

    如果 (ngx_resolver_resend_empty(r)) {
        ngx_add_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_time() + r->resend_timeout;

    ngx_queue_insert_head(resend_queue, &rn->queue);

    rn->code = 0;
    rn->cnlen = 0;
    rn->name = NULL;
    rn->nlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_VALUE;
    rn->waiting = ctx;

    /* unlock addr mutex */

    ctx->state = NGX_AGAIN;
    ctx->async = 1;
    ctx->node = rn;

    返回 NGX_OK;

failed:

    如果 (rn) {
        ngx_rbtree_delete(tree, &rn->node);

        如果 (rn->query) {
            ngx_resolver_free(r, rn->query);
        }

        ngx_resolver_free(r, rn);
    }

    /* unlock addr mutex */

    如果 (ctx->event) {
        ngx_resolver_free(r, ctx->event);
    }

    ngx_resolver_free(r, ctx);

    返回 NGX_ERROR;
}


void
ngx_resolve_addr_done(ngx_resolver_ctx_t *ctx)
{
    ngx_queue_t          *expire_queue;
    ngx_rbtree_t         *tree;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_node_t  *rn;

    r = ctx->resolver;

    switch (ctx->addr.sockaddr->sa_family) {

#如果 (NGX_HAVE_INET6)
    case AF_INET6:
        tree = &r->addr6_rbtree;
        expire_queue = &r->addr6_expire_queue;
        break;
#endif

    default: /* AF_INET */
        tree = &r->addr_rbtree;
        expire_queue = &r->addr_expire_queue;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve addr done: %i", ctx->state);

    如果 (ctx->event && ctx->event->timer_set) {
        ngx_del_timer(ctx->event);
    }

    /* lock addr mutex */

    如果 (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->node;

        如果 (rn) {
            p = &rn->waiting;
            w = rn->waiting;

            当 (w) {
                如果 (w == ctx) {
                    *p = w->next;

                    goto done;
                }

                p = &w->next;
                w = w->next;
            }
        }

        {
            u_char     text[NGX_SOCKADDR_STRLEN];
            ngx_str_t  addrtext;

            addrtext.data = text;
            addrtext.len = ngx_sock_ntop(ctx->addr.sockaddr, ctx->addr.socklen,
                                         text, NGX_SOCKADDR_STRLEN, 0);

            ngx_log_error(NGX_LOG_ALERT, r->log, 0,
                          "could not cancel %V resolving", &addrtext);
        }
    }

done:

    ngx_resolver_expire(r, tree, expire_queue);

    /* unlock addr mutex */

    /* lock alloc mutex */

    如果 (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    如果 (r->event->timer_set && ngx_resolver_resend_empty(r)) {
        ngx_del_timer(r->event);
    }
}


static void
ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *tree, ngx_queue_t *queue)
{
    time_t                now;
    ngx_uint_t            i;
    ngx_queue_t          *q;
    ngx_resolver_node_t  *rn;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver expire");

    now = ngx_time();

    为 (i = 0; i < 2; i++) {
        如果 (ngx_queue_empty(queue)) {
            返回;
        }

        q = ngx_queue_last(queue);

        rn = ngx_queue_data(q, ngx_resolver_node_t, queue);

        如果 (now <= rn->expire) {
            返回;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver expire \"%*s\"", (size_t) rn->nlen, rn->name);

        ngx_queue_remove(q);

        ngx_rbtree_delete(tree, &rn->node);

        ngx_resolver_free_node(r, rn);
    }
}


static ngx_int_t
ngx_resolver_send_query(ngx_resolver_t *r, ngx_resolver_node_t *rn)
{
    ngx_int_t                   rc;
    ngx_resolver_connection_t  *rec;

    rec = r->connections.elts;
    rec = &rec[rn->last_connection];

    如果 (rec->log.handler == NULL) {
        rec->log = *r->log;
        rec->log.handler = ngx_resolver_log_error;
        rec->log.data = rec;
        rec->log.action = "resolving";
    }

    如果 (rn->query && rn->naddrs == (u_short) -1) {
        rc = rn->tcp ? ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen)
                     : ngx_resolver_send_udp_query(r, rec, rn->query, rn->qlen);

        如果 (rc != NGX_OK) {
            返回 rc;
        }
    }

#如果 (NGX_HAVE_INET6)

    如果 (rn->query6 && rn->naddrs6 == (u_short) -1) {
        rc = rn->tcp6
                    ? ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen)
                    : ngx_resolver_send_udp_query(r, rec, rn->query6, rn->qlen);

        如果 (rc != NGX_OK) {
            返回 rc;
        }
    }

#endif

    返回 NGX_OK;
}


static ngx_int_t
ngx_resolver_send_udp_query(ngx_resolver_t *r, ngx_resolver_connection_t  *rec,
    u_char *query, u_short qlen)
{
    ssize_t  n;

    如果 (rec->udp == NULL) {
        如果 (ngx_udp_connect(rec) != NGX_OK) {
            返回 NGX_ERROR;
        }

        rec->udp->data = rec;
        rec->udp->read->handler = ngx_resolver_udp_read;
        rec->udp->read->resolver = 1;
    }

    n = ngx_send(rec->udp, query, qlen);

    如果 (n == NGX_ERROR) {
        goto failed;
    }

    如果 ((size_t) n != (size_t) qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->log, 0, "send() incomplete");
        goto failed;
    }

    返回 NGX_OK;

failed:

    ngx_close_connection(rec->udp);
    rec->udp = NULL;

    返回 NGX_ERROR;
}


static ngx_int_t
ngx_resolver_send_tcp_query(ngx_resolver_t *r, ngx_resolver_connection_t *rec,
    u_char *query, u_short qlen)
{
    ngx_buf_t  *b;
    ngx_int_t   rc;

    rc = NGX_OK;

    如果 (rec->tcp == NULL) {
        b = rec->read_buf;

        如果 (b == NULL) {
            b = ngx_resolver_calloc(r, sizeof(ngx_buf_t));
            如果 (b == NULL) {
                返回 NGX_ERROR;
            }

            b->start = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_RSIZE);
            如果 (b->start == NULL) {
                ngx_resolver_free(r, b);
                返回 NGX_ERROR;
            }

            b->end = b->start + NGX_RESOLVER_TCP_RSIZE;

            rec->read_buf = b;
        }

        b->pos = b->start;
        b->last = b->start;

        b = rec->write_buf;

        如果 (b == NULL) {
            b = ngx_resolver_calloc(r, sizeof(ngx_buf_t));
            如果 (b == NULL) {
                返回 NGX_ERROR;
            }

            b->start = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_WSIZE);
            如果 (b->start == NULL) {
                ngx_resolver_free(r, b);
                返回 NGX_ERROR;
            }

            b->end = b->start + NGX_RESOLVER_TCP_WSIZE;

            rec->write_buf = b;
        }

        b->pos = b->start;
        b->last = b->start;

        rc = ngx_tcp_connect(rec);
        如果 (rc == NGX_ERROR) {
            返回 NGX_ERROR;
        }

        rec->tcp->data = rec;
        rec->tcp->write->handler = ngx_resolver_tcp_write;
        rec->tcp->write->cancelable = 1;
        rec->tcp->read->handler = ngx_resolver_tcp_read;
        rec->tcp->read->resolver = 1;

        ngx_add_timer(rec->tcp->write, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    b = rec->write_buf;

    如果 (b->end - b->last <  2 + qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->log, 0, "buffer overflow");
        返回 NGX_ERROR;
    }

    *b->last++ = (u_char) (qlen >> 8);
    *b->last++ = (u_char) qlen;
    b->last = ngx_cpymem(b->last, query, qlen);

    如果 (rc == NGX_OK) {
        ngx_resolver_tcp_write(rec->tcp->write);
    }

    返回 NGX_OK;
}


static void
ngx_resolver_resend_handler(ngx_event_t *ev)
{
    time_t           timer, atimer, stimer, ntimer;
#如果 (NGX_HAVE_INET6)
    time_t           a6timer;
#endif
    ngx_resolver_t  *r;

    r = ev->data;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver resend handler");

    /* lock name mutex */

    ntimer = ngx_resolver_resend(r, &r->name_rbtree, &r->name_resend_queue);

    stimer = ngx_resolver_resend(r, &r->srv_rbtree, &r->srv_resend_queue);

    /* unlock name mutex */

    /* lock addr mutex */

    atimer = ngx_resolver_resend(r, &r->addr_rbtree, &r->addr_resend_queue);

    /* unlock addr mutex */

#如果 (NGX_HAVE_INET6)

    /* lock addr6 mutex */

    a6timer = ngx_resolver_resend(r, &r->addr6_rbtree, &r->addr6_resend_queue);

    /* unlock addr6 mutex */

#endif

    timer = ntimer;

    如果 (timer == 0) {
        timer = atimer;

    } 否则 如果 (atimer) {
        timer = ngx_min(timer, atimer);
    }

    如果 (timer == 0) {
        timer = stimer;

    } 否则 如果 (stimer) {
        timer = ngx_min(timer, stimer);
    }

#如果 (NGX_HAVE_INET6)

    如果 (timer == 0) {
        timer = a6timer;

    } 否则 如果 (a6timer) {
        timer = ngx_min(timer, a6timer);
    }

#endif

    如果 (timer) {
        ngx_add_timer(r->event, (ngx_msec_t) (timer * 1000));
    }
}


static time_t
ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *tree, ngx_queue_t *queue)
{
    time_t                now;
    ngx_queue_t          *q;
    ngx_resolver_node_t  *rn;

    now = ngx_time();

    为 ( ;; ) {
        如果 (ngx_queue_empty(queue)) {
            返回 0;
        }

        q = ngx_queue_last(queue);

        rn = ngx_queue_data(q, ngx_resolver_node_t, queue);

        如果 (now < rn->expire) {
            返回 rn->expire - now;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver resend \"%*s\" %p",
                       (size_t) rn->nlen, rn->name, rn->waiting);

        ngx_queue_remove(q);

        如果 (rn->waiting) {

            如果 (++rn->last_connection == r->connections.nelts) {
                rn->last_connection = 0;
            }

            (void) ngx_resolver_send_query(r, rn);

            rn->expire = now + r->resend_timeout;

            ngx_queue_insert_head(queue, q);

            continue;
        }

        ngx_rbtree_delete(tree, &rn->node);

        ngx_resolver_free_node(r, rn);
    }
}


static ngx_uint_t
ngx_resolver_resend_empty(ngx_resolver_t *r)
{
    返回 ngx_queue_empty(&r->name_resend_queue)
           && ngx_queue_empty(&r->srv_resend_queue)
#如果 (NGX_HAVE_INET6)
           && ngx_queue_empty(&r->addr6_resend_queue)
#endif
           && ngx_queue_empty(&r->addr_resend_queue);
}


static void
ngx_resolver_udp_read(ngx_event_t *rev)
{
    ssize_t                     n;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;
    u_char                      buf[NGX_RESOLVER_UDP_SIZE];

    c = rev->data;
    rec = c->data;

    do {
        n = ngx_udp_recv(c, buf, NGX_RESOLVER_UDP_SIZE);

        如果 (n == NGX_AGAIN) {
            break;
        }

        如果 (n == NGX_ERROR) {
            goto failed;
        }

        ngx_resolver_process_response(rec->resolver, buf, n, 0);

    } 当 (rev->ready);

    如果 (ngx_handle_read_event(rev, 0) != NGX_OK) {
        goto failed;
    }

    返回;

failed:

    ngx_close_connection(rec->udp);
    rec->udp = NULL;
}


static void
ngx_resolver_tcp_write(ngx_event_t *wev)
{
    off_t                       sent;
    ssize_t                     n;
    ngx_buf_t                  *b;
    ngx_resolver_t             *r;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;

    c = wev->data;
    rec = c->data;
    b = rec->write_buf;
    r = rec->resolver;

    如果 (wev->timedout) {
        goto failed;
    }

    sent = c->sent;

    当 (wev->ready && b->pos < b->last) {
        n = ngx_send(c, b->pos, b->last - b->pos);

        如果 (n == NGX_AGAIN) {
            break;
        }

        如果 (n == NGX_ERROR) {
            goto failed;
        }

        b->pos += n;
    }

    如果 (b->pos != b->start) {
        b->last = ngx_movemem(b->start, b->pos, b->last - b->pos);
        b->pos = b->start;
    }

    如果 (c->sent != sent) {
        ngx_add_timer(wev, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    如果 (ngx_handle_write_event(wev, 0) != NGX_OK) {
        goto failed;
    }

    返回;

failed:

    ngx_close_connection(c);
    rec->tcp = NULL;
}


static void
ngx_resolver_tcp_read(ngx_event_t *rev)
{
    u_char                     *p;
    size_t                      size;
    ssize_t                     n;
    u_short                     qlen;
    ngx_buf_t                  *b;
    ngx_resolver_t             *r;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;

    c = rev->data;
    rec = c->data;
    b = rec->read_buf;
    r = rec->resolver;

    当 (rev->ready) {
        n = ngx_recv(c, b->last, b->end - b->last);

        如果 (n == NGX_AGAIN) {
            break;
        }

        如果 (n == NGX_ERROR || n == 0) {
            goto failed;
        }

        b->last += n;

        为 ( ;; ) {
            p = b->pos;
            size = b->last - p;

            如果 (size < 2) {
                break;
            }

            qlen = (u_short) *p++ << 8;
            qlen += *p++;

            如果 (size < (size_t) (2 + qlen)) {
                break;
            }

            ngx_resolver_process_response(r, p, qlen, 1);

            b->pos += 2 + qlen;
        }

        如果 (b->pos != b->start) {
            b->last = ngx_movemem(b->start, b->pos, b->last - b->pos);
            b->pos = b->start;
        }
    }

    如果 (ngx_handle_read_event(rev, 0) != NGX_OK) {
        goto failed;
    }

    返回;

failed:

    ngx_close_connection(c);
    rec->tcp = NULL;
}


static void
ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t tcp)
{
    char                 *err;
    ngx_uint_t            i, times, ident, qident, flags, code, nqs, nan, trunc,
                          qtype, qclass;
#如果 (NGX_HAVE_INET6)
    ngx_uint_t            qident6;
#endif
    ngx_queue_t          *q;
    ngx_resolver_qs_t    *qs;
    ngx_resolver_hdr_t   *response;
    ngx_resolver_node_t  *rn;

    如果 (n < sizeof(ngx_resolver_hdr_t)) {
        goto short_response;
    }

    response = (ngx_resolver_hdr_t *) buf;

    ident = (response->ident_hi << 8) + response->ident_lo;
    flags = (response->flags_hi << 8) + response->flags_lo;
    nqs = (response->nqs_hi << 8) + response->nqs_lo;
    nan = (response->nan_hi << 8) + response->nan_lo;
    trunc = flags & 0x0200;

    ngx_log_debug6(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver DNS response %ui fl:%04Xi %ui/%ui/%ud/%ud",
                   ident, flags, nqs, nan,
                   (response->nns_hi << 8) + response->nns_lo,
                   (response->nar_hi << 8) + response->nar_lo);

    /* response to a standard query */
    如果 ((flags & 0xf870) != 0x8000 || (trunc && tcp)) {
        ngx_log_error(r->log_level, r->log, 0,
                      "invalid %s DNS response %ui fl:%04Xi",
                      tcp ? "TCP" : "UDP", ident, flags);
        返回;
    }

    code = flags & 0xf;

    如果 (code == NGX_RESOLVE_FORMERR) {

        times = 0;

        为 (q = ngx_queue_head(&r->name_resend_queue);
             q != ngx_queue_sentinel(&r->name_resend_queue) && times++ < 100;
             q = ngx_queue_next(q))
        {
            rn = ngx_queue_data(q, ngx_resolver_node_t, queue);

            如果 (rn->query) {
                qident = (rn->query[0] << 8) + rn->query[1];

                如果 (qident == ident) {
                    goto dns_error_name;
                }
            }

#如果 (NGX_HAVE_INET6)
            如果 (rn->query6) {
                qident6 = (rn->query6[0] << 8) + rn->query6[1];

                如果 (qident6 == ident) {
                    goto dns_error_name;
                }
            }
#endif
        }

        goto dns_error;
    }

    如果 (code > NGX_RESOLVE_REFUSED) {
        goto dns_error;
    }

    如果 (nqs != 1) {
        err = "invalid number of questions in DNS response";
        goto done;
    }

    i = sizeof(ngx_resolver_hdr_t);

    当 (i < (ngx_uint_t) n) {

        如果 (buf[i] & 0xc0) {
            err = "unexpected compression pointer in DNS response";
            goto done;
        }

        如果 (buf[i] == '\0') {
            goto found;
        }

        i += 1 + buf[i];
    }

    goto short_response;

found:

    如果 (i++ == sizeof(ngx_resolver_hdr_t)) {
        err = "zero-length domain name in DNS response";
        goto done;
    }

    如果 (i + sizeof(ngx_resolver_qs_t) + nan * (2 + sizeof(ngx_resolver_an_t))
        > (ngx_uint_t) n)
    {
        goto short_response;
    }

    qs = (ngx_resolver_qs_t *) &buf[i];

    qtype = (qs->type_hi << 8) + qs->type_lo;
    qclass = (qs->class_hi << 8) + qs->class_lo;

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver DNS response qt:%ui cl:%ui", qtype, qclass);

    如果 (qclass != 1) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unknown query class %ui in DNS response", qclass);
        返回;
    }

    switch (qtype) {

    case NGX_RESOLVE_A:
#如果 (NGX_HAVE_INET6)
    case NGX_RESOLVE_AAAA:
#endif

        ngx_resolver_process_a(r, buf, n, ident, code, qtype, nan, trunc,
                               i + sizeof(ngx_resolver_qs_t));

        break;

    case NGX_RESOLVE_SRV:

        ngx_resolver_process_srv(r, buf, n, ident, code, nan, trunc,
                                 i + sizeof(ngx_resolver_qs_t));

        break;

    case NGX_RESOLVE_PTR:

        ngx_resolver_process_ptr(r, buf, n, ident, code, nan);

        break;

    default:
        ngx_log_error(r->log_level, r->log, 0,
                      "unknown query type %ui in DNS response", qtype);
        返回;
    }

    返回;

short_response:

    err = "short DNS response";

done:

    ngx_log_error(r->log_level, r->log, 0, err);

    返回;

dns_error_name:

    ngx_log_error(r->log_level, r->log, 0,
                  "DNS error (%ui: %s), query id:%ui, name:\"%*s\"",
                  code, ngx_resolver_strerror(code), ident,
                  (size_t) rn->nlen, rn->name);
    返回;

dns_error:

    ngx_log_error(r->log_level, r->log, 0,
                  "DNS error (%ui: %s), query id:%ui",
                  code, ngx_resolver_strerror(code), ident);
    返回;
}


static void
ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans)
{
    char                       *err;
    u_char                     *cname;
    size_t                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    in_addr_t                  *addr;
    ngx_str_t                   name;
    ngx_uint_t                  type, class, qident, naddrs, a, i, j, start;
#如果 (NGX_HAVE_INET6)
    struct in6_addr            *addr6;
#endif
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *next;
    ngx_resolver_node_t        *rn;
    ngx_resolver_addr_t        *addrs;
    ngx_resolver_connection_t  *rec;

    如果 (ngx_resolver_copy(r, &name, buf,
                          buf + sizeof(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        返回;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver qs:%V", &name);

    hash = ngx_crc32_short(name.data, name.len);

    /* lock name mutex */

    rn = ngx_resolver_lookup_name(r, &name, hash);

    如果 (rn == NULL) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.data);
        goto failed;
    }

    switch (qtype) {

#如果 (NGX_HAVE_INET6)
    case NGX_RESOLVE_AAAA:

        如果 (rn->query6 == NULL || rn->naddrs6 != (u_short) -1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.data);
            goto failed;
        }

        如果 (trunc && rn->tcp6) {
            ngx_resolver_free(r, name.data);
            goto failed;
        }

        qident = (rn->query6[0] << 8) + rn->query6[1];

        break;
#endif

    default: /* NGX_RESOLVE_A */

        如果 (rn->query == NULL || rn->naddrs != (u_short) -1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.data);
            goto failed;
        }

        如果 (trunc && rn->tcp) {
            ngx_resolver_free(r, name.data);
            goto failed;
        }

        qident = (rn->query[0] << 8) + rn->query[1];
    }

    如果 (ident != qident) {
        ngx_log_error(r->log_level, r->log, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.data);
        goto failed;
    }

    ngx_resolver_free(r, name.data);

    如果 (trunc) {

        ngx_queue_remove(&rn->queue);

        如果 (rn->waiting == NULL) {
            ngx_rbtree_delete(&r->name_rbtree, &rn->node);
            ngx_resolver_free_node(r, rn);
            goto next;
        }

        rec = r->connections.elts;
        rec = &rec[rn->last_connection];

        switch (qtype) {

#如果 (NGX_HAVE_INET6)
        case NGX_RESOLVE_AAAA:

            rn->tcp6 = 1;

            (void) ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen);

            break;
#endif

        default: /* NGX_RESOLVE_A */

            rn->tcp = 1;

            (void) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);
        }

        rn->expire = ngx_time() + r->resend_timeout;

        ngx_queue_insert_head(&r->name_resend_queue, &rn->queue);

        goto next;
    }

    如果 (code == 0 && rn->code) {
        code = rn->code;
    }

    如果 (code == 0 && nan == 0) {

#如果 (NGX_HAVE_INET6)
        switch (qtype) {

        case NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            如果 (rn->naddrs == (u_short) -1) {
                goto next;
            }

            如果 (rn->naddrs) {
                goto export;
            }

            break;

        default: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            如果 (rn->naddrs6 == (u_short) -1) {
                goto next;
            }

            如果 (rn->naddrs6) {
                goto export;
            }
        }
#endif

        code = NGX_RESOLVE_NXDOMAIN;
    }

    如果 (code) {

#如果 (NGX_HAVE_INET6)
        switch (qtype) {

        case NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            如果 (rn->naddrs == (u_short) -1) {
                rn->code = (u_char) code;
                goto next;
            }

            break;

        default: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            如果 (rn->naddrs6 == (u_short) -1) {
                rn->code = (u_char) code;
                goto next;
            }
        }
#endif

        next = rn->waiting;
        rn->waiting = NULL;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(&r->name_rbtree, &rn->node);

        /* unlock name mutex */

        当 (next) {
            ctx = next;
            ctx->state = code;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            next = ctx->next;

            ctx->handler(ctx);
        }

        ngx_resolver_free_node(r, rn);

        返回;
    }

    i = ans;
    naddrs = 0;
    cname = NULL;

    为 (a = 0; a < nan; a++) {

        start = i;

        当 (i < n) {

            如果 (buf[i] & 0xc0) {
                i += 2;
                goto found;
            }

            如果 (buf[i] == 0) {
                i++;
                goto test_length;
            }

            i += 1 + buf[i];
        }

        goto short_response;

    test_length:

        如果 (i - start < 2) {
            err = "invalid name in DNS response";
            goto invalid;
        }

    found:

        如果 (i + sizeof(ngx_resolver_an_t) >= n) {
            goto short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        如果 (class != 1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR class %ui in DNS response", class);
            goto failed;
        }

        如果 (ttl < 0) {
            ttl = 0;
        }

        rn->ttl = ngx_min(rn->ttl, (uint32_t) ttl);

        i += sizeof(ngx_resolver_an_t);

        switch (type) {

        case NGX_RESOLVE_A:

            如果 (qtype != NGX_RESOLVE_A) {
                err = "unexpected A record in DNS response";
                goto invalid;
            }

            如果 (len != 4) {
                err = "invalid A record in DNS response";
                goto invalid;
            }

            如果 (i + 4 > n) {
                goto short_response;
            }

            naddrs++;

            break;

#如果 (NGX_HAVE_INET6)
        case NGX_RESOLVE_AAAA:

            如果 (qtype != NGX_RESOLVE_AAAA) {
                err = "unexpected AAAA record in DNS response";
                goto invalid;
            }

            如果 (len != 16) {
                err = "invalid AAAA record in DNS response";
                goto invalid;
            }

            如果 (i + 16 > n) {
                goto short_response;
            }

            naddrs++;

            break;
#endif

        case NGX_RESOLVE_CNAME:

            cname = &buf[i];

            break;

        case NGX_RESOLVE_DNAME:

            break;

        default:

            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver naddrs:%ui cname:%p ttl:%uD",
                   naddrs, cname, rn->ttl);

    如果 (naddrs) {

        switch (qtype) {

#如果 (NGX_HAVE_INET6)
        case NGX_RESOLVE_AAAA:

            如果 (naddrs == 1) {
                addr6 = &rn->u6.addr6;
                rn->naddrs6 = 1;

            } 否则 {
                addr6 = ngx_resolver_alloc(r, naddrs * sizeof(struct in6_addr));
                如果 (addr6 == NULL) {
                    goto failed;
                }

                rn->u6.addrs6 = addr6;
                rn->naddrs6 = (u_short) naddrs;
            }

#如果 (NGX_SUPPRESS_WARN)
            addr = NULL;
#endif

            break;
#endif

        default: /* NGX_RESOLVE_A */

            如果 (naddrs == 1) {
                addr = &rn->u.addr;
                rn->naddrs = 1;

            } 否则 {
                addr = ngx_resolver_alloc(r, naddrs * sizeof(in_addr_t));
                如果 (addr == NULL) {
                    goto failed;
                }

                rn->u.addrs = addr;
                rn->naddrs = (u_short) naddrs;
            }

#如果 (NGX_HAVE_INET6 && NGX_SUPPRESS_WARN)
            addr6 = NULL;
#endif
        }

        j = 0;
        i = ans;

        为 (a = 0; a < nan; a++) {

            为 ( ;; ) {

                如果 (buf[i] & 0xc0) {
                    i += 2;
                    break;
                }

                如果 (buf[i] == 0) {
                    i++;
                    break;
                }

                i += 1 + buf[i];
            }

            an = (ngx_resolver_an_t *) &buf[i];

            type = (an->type_hi << 8) + an->type_lo;
            len = (an->len_hi << 8) + an->len_lo;

            i += sizeof(ngx_resolver_an_t);

            如果 (type == NGX_RESOLVE_A) {

                addr[j] = htonl((buf[i] << 24) + (buf[i + 1] << 16)
                                + (buf[i + 2] << 8) + (buf[i + 3]));

                如果 (++j == naddrs) {

#如果 (NGX_HAVE_INET6)
                    如果 (rn->naddrs6 == (u_short) -1) {
                        goto next;
                    }
#endif

                    break;
                }
            }

#如果 (NGX_HAVE_INET6)
            否则 如果 (type == NGX_RESOLVE_AAAA) {

                ngx_memcpy(addr6[j].s6_addr, &buf[i], 16);

                如果 (++j == naddrs) {

                    如果 (rn->naddrs == (u_short) -1) {
                        goto next;
                    }

                    break;
                }
            }
#endif

            i += len;
        }
    }

    switch (qtype) {

#如果 (NGX_HAVE_INET6)
    case NGX_RESOLVE_AAAA:

        如果 (rn->naddrs6 == (u_short) -1) {
            rn->naddrs6 = 0;
        }

        break;
#endif

    default: /* NGX_RESOLVE_A */

        如果 (rn->naddrs == (u_short) -1) {
            rn->naddrs = 0;
        }
    }

    如果 (rn->naddrs != (u_short) -1
#如果 (NGX_HAVE_INET6)
        && rn->naddrs6 != (u_short) -1
#endif
        && rn->naddrs
#如果 (NGX_HAVE_INET6)
           + rn->naddrs6
#endif
           > 0)
    {

#如果 (NGX_HAVE_INET6)
    export:
#endif

        naddrs = rn->naddrs;
#如果 (NGX_HAVE_INET6)
        naddrs += rn->naddrs6;
#endif

        如果 (naddrs == 1 && rn->naddrs == 1) {
            addrs = NULL;

        } 否则 {
            addrs = ngx_resolver_export(r, rn, 0);
            如果 (addrs == NULL) {
                goto failed;
            }
        }

        ngx_queue_remove(&rn->queue);

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_insert_head(&r->name_expire_queue, &rn->queue);

        next = rn->waiting;
        rn->waiting = NULL;

        /* unlock name mutex */

        当 (next) {
            ctx = next;
            ctx->state = NGX_OK;
            ctx->valid = rn->valid;
            ctx->naddrs = naddrs;

            如果 (addrs == NULL) {
                ctx->addrs = &ctx->addr;
                ctx->addr.sockaddr = (struct sockaddr *) &ctx->sin;
                ctx->addr.socklen = sizeof(struct sockaddr_in);
                ngx_memzero(&ctx->sin, sizeof(struct sockaddr_in));
                ctx->sin.sin_family = AF_INET;
                ctx->sin.sin_addr.s_addr = rn->u.addr;

            } 否则 {
                ctx->addrs = addrs;
            }

            next = ctx->next;

            ctx->handler(ctx);
        }

        如果 (addrs != NULL) {
            ngx_resolver_free(r, addrs->sockaddr);
            ngx_resolver_free(r, addrs);
        }

        ngx_resolver_free(r, rn->query);
        rn->query = NULL;
#如果 (NGX_HAVE_INET6)
        rn->query6 = NULL;
#endif

        返回;
    }

    如果 (cname) {

        /* CNAME only */

        如果 (rn->naddrs == (u_short) -1
#如果 (NGX_HAVE_INET6)
            || rn->naddrs6 == (u_short) -1
#endif
            )
        {
            goto next;
        }

        如果 (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            goto failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_queue_remove(&rn->queue);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.data;

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_insert_head(&r->name_expire_queue, &rn->queue);

        ngx_resolver_free(r, rn->query);
        rn->query = NULL;
#如果 (NGX_HAVE_INET6)
        rn->query6 = NULL;
#endif

        ctx = rn->waiting;
        rn->waiting = NULL;

        如果 (ctx) {

            如果 (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                do {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    next = ctx->next;

                    ctx->handler(ctx);

                    ctx = next;
                } 当 (ctx);

                返回;
            }

            为 (next = ctx; next; next = next->next) {
                next->node = NULL;
            }

            (void) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        返回;
    }

    ngx_log_error(r->log_level, r->log, 0,
                  "no A or CNAME types in DNS response");
    返回;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->log, 0, err);

    返回;

failed:

next:

    /* unlock name mutex */

    返回;
}


static void
ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans)
{
    char                       *err;
    u_char                     *cname;
    size_t                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    ngx_str_t                   name;
    ngx_uint_t                  type, qident, class, start, nsrvs, a, i, j;
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *next;
    ngx_resolver_srv_t         *srvs;
    ngx_resolver_node_t        *rn;
    ngx_resolver_connection_t  *rec;

    如果 (ngx_resolver_copy(r, &name, buf,
                          buf + sizeof(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        返回;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver qs:%V", &name);

    hash = ngx_crc32_short(name.data, name.len);

    rn = ngx_resolver_lookup_srv(r, &name, hash);

    如果 (rn == NULL || rn->query == NULL) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.data);
        goto failed;
    }

    如果 (trunc && rn->tcp) {
        ngx_resolver_free(r, name.data);
        goto failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    如果 (ident != qident) {
        ngx_log_error(r->log_level, r->log, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.data);
        goto failed;
    }

    ngx_resolver_free(r, name.data);

    如果 (trunc) {

        ngx_queue_remove(&rn->queue);

        如果 (rn->waiting == NULL) {
            ngx_rbtree_delete(&r->srv_rbtree, &rn->node);
            ngx_resolver_free_node(r, rn);
            返回;
        }

        rec = r->connections.elts;
        rec = &rec[rn->last_connection];

        rn->tcp = 1;

        (void) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);

        rn->expire = ngx_time() + r->resend_timeout;

        ngx_queue_insert_head(&r->srv_resend_queue, &rn->queue);

        返回;
    }

    如果 (code == 0 && rn->code) {
        code = rn->code;
    }

    如果 (code == 0 && nan == 0) {
        code = NGX_RESOLVE_NXDOMAIN;
    }

    如果 (code) {
        next = rn->waiting;
        rn->waiting = NULL;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(&r->srv_rbtree, &rn->node);

        当 (next) {
            ctx = next;
            ctx->state = code;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            next = ctx->next;

            ctx->handler(ctx);
        }

        ngx_resolver_free_node(r, rn);

        返回;
    }

    i = ans;
    nsrvs = 0;
    cname = NULL;

    为 (a = 0; a < nan; a++) {

        start = i;

        当 (i < n) {

            如果 (buf[i] & 0xc0) {
                i += 2;
                goto found;
            }

            如果 (buf[i] == 0) {
                i++;
                goto test_length;
            }

            i += 1 + buf[i];
        }

        goto short_response;

    test_length:

        如果 (i - start < 2) {
            err = "invalid name DNS response";
            goto invalid;
        }

    found:

        如果 (i + sizeof(ngx_resolver_an_t) >= n) {
            goto short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        如果 (class != 1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR class %ui in DNS response", class);
            goto failed;
        }

        如果 (ttl < 0) {
            ttl = 0;
        }

        rn->ttl = ngx_min(rn->ttl, (uint32_t) ttl);

        i += sizeof(ngx_resolver_an_t);

        switch (type) {

        case NGX_RESOLVE_SRV:

            如果 (i + 6 >= n) {
                goto short_response;
            }

            如果 (ngx_resolver_copy(r, NULL, buf, &buf[i + 6], buf + n)
                != NGX_OK)
            {
                goto failed;
            }

            nsrvs++;

            break;

        case NGX_RESOLVE_CNAME:

            cname = &buf[i];

            break;

        case NGX_RESOLVE_DNAME:

            break;

        default:

            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver nsrvs:%ui cname:%p ttl:%uD",
                   nsrvs, cname, rn->ttl);

    如果 (nsrvs) {

        srvs = ngx_resolver_calloc(r, nsrvs * sizeof(ngx_resolver_srv_t));
        如果 (srvs == NULL) {
            goto failed;
        }

        rn->u.srvs = srvs;
        rn->nsrvs = (u_short) nsrvs;

        j = 0;
        i = ans;

        为 (a = 0; a < nan; a++) {

            为 ( ;; ) {

                如果 (buf[i] & 0xc0) {
                    i += 2;
                    break;
                }

                如果 (buf[i] == 0) {
                    i++;
                    break;
                }

                i += 1 + buf[i];
            }

            an = (ngx_resolver_an_t *) &buf[i];

            type = (an->type_hi << 8) + an->type_lo;
            len = (an->len_hi << 8) + an->len_lo;

            i += sizeof(ngx_resolver_an_t);

            如果 (type == NGX_RESOLVE_SRV) {

                srvs[j].priority = (buf[i] << 8) + buf[i + 1];
                srvs[j].weight = (buf[i + 2] << 8) + buf[i + 3];

                如果 (srvs[j].weight == 0) {
                    srvs[j].weight = 1;
                }

                srvs[j].port = (buf[i + 4] << 8) + buf[i + 5];

                如果 (ngx_resolver_copy(r, &srvs[j].name, buf, &buf[i + 6],
                                      buf + n)
                    != NGX_OK)
                {
                    goto failed;
                }

                j++;
            }

            i += len;
        }

        ngx_sort(srvs, nsrvs, sizeof(ngx_resolver_srv_t),
                 ngx_resolver_cmp_srvs);

        ngx_resolver_free(r, rn->query);
        rn->query = NULL;

        ngx_queue_remove(&rn->queue);

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_insert_head(&r->srv_expire_queue, &rn->queue);

        next = rn->waiting;
        rn->waiting = NULL;

        当 (next) {
            ctx = next;
            next = ctx->next;

            ngx_resolver_resolve_srv_names(ctx, rn);
        }

        返回;
    }

    rn->nsrvs = 0;

    如果 (cname) {

        /* CNAME only */

        如果 (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            goto failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_queue_remove(&rn->queue);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.data;

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_insert_head(&r->srv_expire_queue, &rn->queue);

        ngx_resolver_free(r, rn->query);
        rn->query = NULL;
#如果 (NGX_HAVE_INET6)
        rn->query6 = NULL;
#endif

        ctx = rn->waiting;
        rn->waiting = NULL;

        如果 (ctx) {

            如果 (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                do {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    next = ctx->next;

                    ctx->handler(ctx);

                    ctx = next;
                } 当 (ctx);

                返回;
            }

            为 (next = ctx; next; next = next->next) {
                next->node = NULL;
            }

            (void) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        返回;
    }

    ngx_log_error(r->log_level, r->log, 0, "no SRV type in DNS response");

    返回;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->log, 0, err);

    返回;

failed:

    /* unlock name mutex */

    返回;
}


static void
ngx_resolver_resolve_srv_names(ngx_resolver_ctx_t *ctx, ngx_resolver_node_t *rn)
{
    ngx_uint_t                i;
    ngx_resolver_t           *r;
    ngx_resolver_ctx_t       *cctx;
    ngx_resolver_srv_name_t  *srvs;

    r = ctx->resolver;

    ctx->node = NULL;
    ctx->state = NGX_OK;
    ctx->valid = rn->valid;
    ctx->count = rn->nsrvs;

    srvs = ngx_resolver_calloc(r, rn->nsrvs * sizeof(ngx_resolver_srv_name_t));
    如果 (srvs == NULL) {
        goto failed;
    }

    ctx->srvs = srvs;
    ctx->nsrvs = rn->nsrvs;

    如果 (ctx->event && ctx->event->timer_set) {
        ngx_del_timer(ctx->event);
    }

    为 (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
        srvs[i].name.data = ngx_resolver_alloc(r, rn->u.srvs[i].name.len);
        如果 (srvs[i].name.data == NULL) {
            goto failed;
        }

        srvs[i].name.len = rn->u.srvs[i].name.len;
        ngx_memcpy(srvs[i].name.data, rn->u.srvs[i].name.data,
                   srvs[i].name.len);

        cctx = ngx_resolve_start(r, NULL);
        如果 (cctx == NULL) {
            goto failed;
        }

        cctx->name = srvs[i].name;
        cctx->handler = ngx_resolver_srv_names_handler;
        cctx->data = ctx;
        cctx->srvs = &srvs[i];
        cctx->timeout = ctx->timeout;

        srvs[i].priority = rn->u.srvs[i].priority;
        srvs[i].weight = rn->u.srvs[i].weight;
        srvs[i].port = rn->u.srvs[i].port;
        srvs[i].ctx = cctx;

        如果 (ngx_resolve_name(cctx) == NGX_ERROR) {
            srvs[i].ctx = NULL;
            goto failed;
        }
    }

    返回;

failed:

    ctx->state = NGX_ERROR;
    ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

    ctx->handler(ctx);
}


static void
ngx_resolver_srv_names_handler(ngx_resolver_ctx_t *cctx)
{
    ngx_uint_t                i;
    ngx_addr_t               *addrs;
    ngx_resolver_t           *r;
    ngx_sockaddr_t           *sockaddr;
    ngx_resolver_ctx_t       *ctx;
    ngx_resolver_srv_name_t  *srv;

    r = cctx->resolver;
    ctx = cctx->data;
    srv = cctx->srvs;

    ctx->count--;
    ctx->async |= cctx->async;

    srv->ctx = NULL;
    srv->state = cctx->state;

    如果 (cctx->naddrs) {

        ctx->valid = ngx_min(ctx->valid, cctx->valid);

        addrs = ngx_resolver_calloc(r, cctx->naddrs * sizeof(ngx_addr_t));
        如果 (addrs == NULL) {
            srv->state = NGX_ERROR;
            goto done;
        }

        sockaddr = ngx_resolver_alloc(r, cctx->naddrs * sizeof(ngx_sockaddr_t));
        如果 (sockaddr == NULL) {
            ngx_resolver_free(r, addrs);
            srv->state = NGX_ERROR;
            goto done;
        }

        为 (i = 0; i < cctx->naddrs; i++) {
            addrs[i].sockaddr = &sockaddr[i].sockaddr;
            addrs[i].socklen = cctx->addrs[i].socklen;

            ngx_memcpy(&sockaddr[i], cctx->addrs[i].sockaddr,
                       addrs[i].socklen);

            ngx_inet_set_port(addrs[i].sockaddr, srv->port);
        }

        srv->addrs = addrs;
        srv->naddrs = cctx->naddrs;
    }

done:

    ngx_resolve_name_done(cctx);

    如果 (ctx->count == 0) {
        ngx_resolver_report_srv(r, ctx);
    }
}


static void
ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan)
{
    char                 *err;
    size_t                len;
    in_addr_t             addr;
    int32_t               ttl;
    ngx_int_t             octet;
    ngx_str_t             name;
    ngx_uint_t            mask, type, class, qident, a, i, start;
    ngx_queue_t          *expire_queue;
    ngx_rbtree_t         *tree;
    ngx_resolver_an_t    *an;
    ngx_resolver_ctx_t   *ctx, *next;
    ngx_resolver_node_t  *rn;
#如果 (NGX_HAVE_INET6)
    uint32_t              hash;
    ngx_int_t             digit;
    struct in6_addr       addr6;
#endif

    如果 (ngx_resolver_copy(r, &name, buf,
                          buf + sizeof(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        返回;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver qs:%V", &name);

    /* AF_INET */

    addr = 0;
    i = sizeof(ngx_resolver_hdr_t);

    为 (mask = 0; mask < 32; mask += 8) {
        len = buf[i++];

        octet = ngx_atoi(&buf[i], len);
        如果 (octet == NGX_ERROR || octet > 255) {
            goto invalid_in_addr_arpa;
        }

        addr += octet << mask;
        i += len;
    }

    如果 (ngx_strcasecmp(&buf[i], (u_char *) "\7in-addr\4arpa") == 0) {
        i += sizeof("\7in-addr\4arpa");

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        tree = &r->addr_rbtree;
        expire_queue = &r->addr_expire_queue;

        goto valid;
    }

invalid_in_addr_arpa:

#如果 (NGX_HAVE_INET6)

    i = sizeof(ngx_resolver_hdr_t);

    为 (octet = 15; octet >= 0; octet--) {
        如果 (buf[i++] != '\1') {
            goto invalid_ip6_arpa;
        }

        digit = ngx_hextoi(&buf[i++], 1);
        如果 (digit == NGX_ERROR) {
            goto invalid_ip6_arpa;
        }

        addr6.s6_addr[octet] = (u_char) digit;

        如果 (buf[i++] != '\1') {
            goto invalid_ip6_arpa;
        }

        digit = ngx_hextoi(&buf[i++], 1);
        如果 (digit == NGX_ERROR) {
            goto invalid_ip6_arpa;
        }

        addr6.s6_addr[octet] += (u_char) (digit * 16);
    }

    如果 (ngx_strcasecmp(&buf[i], (u_char *) "\3ip6\4arpa") == 0) {
        i += sizeof("\3ip6\4arpa");

        /* lock addr mutex */

        hash = ngx_crc32_short(addr6.s6_addr, 16);
        rn = ngx_resolver_lookup_addr6(r, &addr6, hash);

        tree = &r->addr6_rbtree;
        expire_queue = &r->addr6_expire_queue;

        goto valid;
    }

invalid_ip6_arpa:
#endif

    ngx_log_error(r->log_level, r->log, 0,
                  "invalid in-addr.arpa or ip6.arpa name in DNS response");
    ngx_resolver_free(r, name.data);
    返回;

valid:

    如果 (rn == NULL || rn->query == NULL) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.data);
        goto failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    如果 (ident != qident) {
        ngx_log_error(r->log_level, r->log, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.data);
        goto failed;
    }

    ngx_resolver_free(r, name.data);

    如果 (code == 0 && nan == 0) {
        code = NGX_RESOLVE_NXDOMAIN;
    }

    如果 (code) {
        next = rn->waiting;
        rn->waiting = NULL;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(tree, &rn->node);

        /* unlock addr mutex */

        当 (next) {
            ctx = next;
            ctx->state = code;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            next = ctx->next;

            ctx->handler(ctx);
        }

        ngx_resolver_free_node(r, rn);

        返回;
    }

    i += sizeof(ngx_resolver_qs_t);

    为 (a = 0; a < nan; a++) {

        start = i;

        当 (i < n) {

            如果 (buf[i] & 0xc0) {
                i += 2;
                goto found;
            }

            如果 (buf[i] == 0) {
                i++;
                goto test_length;
            }

            i += 1 + buf[i];
        }

        goto short_response;

    test_length:

        如果 (i - start < 2) {
            err = "invalid name in DNS response";
            goto invalid;
        }

    found:

        如果 (i + sizeof(ngx_resolver_an_t) >= n) {
            goto short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        如果 (class != 1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR class %ui in DNS response", class);
            goto failed;
        }

        如果 (ttl < 0) {
            ttl = 0;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                      "resolver qt:%ui cl:%ui len:%uz",
                      type, class, len);

        i += sizeof(ngx_resolver_an_t);

        switch (type) {

        case NGX_RESOLVE_PTR:

            goto ptr;

        case NGX_RESOLVE_CNAME:

            break;

        default:

            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->log, 0,
                  "no PTR type in DNS response");
    返回;

ptr:

    如果 (ngx_resolver_copy(r, &name, buf, buf + i, buf + n) != NGX_OK) {
        goto failed;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver an:%V", &name);

    如果 (name.len != (size_t) rn->nlen
        || ngx_strncmp(name.data, rn->name, name.len) != 0)
    {
        如果 (rn->nlen) {
            ngx_resolver_free(r, rn->name);
        }

        rn->nlen = (u_short) name.len;
        rn->name = name.data;

        name.data = ngx_resolver_dup(r, rn->name, name.len);
        如果 (name.data == NULL) {
            goto failed;
        }
    }

    ngx_queue_remove(&rn->queue);

    rn->valid = ngx_time() + (r->valid ? r->valid : ttl);
    rn->expire = ngx_time() + r->expire;

    ngx_queue_insert_head(expire_queue, &rn->queue);

    next = rn->waiting;
    rn->waiting = NULL;

    /* unlock addr mutex */

    当 (next) {
        ctx = next;
        ctx->state = NGX_OK;
        ctx->valid = rn->valid;
        ctx->name = name;
        next = ctx->next;

        ctx->handler(ctx);
    }

    ngx_resolver_free(r, name.data);

    返回;

short_response:

    err = "short DNS response";

invalid:

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->log, 0, err);

    返回;

failed:

    /* unlock addr mutex */

    返回;
}


static ngx_resolver_node_t *
ngx_resolver_lookup_name(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_node_t    *node, *sentinel;
    ngx_resolver_node_t  *rn;

    node = r->name_rbtree.root;
    sentinel = r->name_rbtree.sentinel;

    当 (node != sentinel) {

        如果 (hash < node->key) {
            node = node->left;
            continue;
        }

        如果 (hash > node->key) {
            node = node->right;
            continue;
        }

        /* hash == node->key */

        rn = ngx_resolver_node(node);

        rc = ngx_memn2cmp(name->data, rn->name, name->len, rn->nlen);

        如果 (rc == 0) {
            返回 rn;
        }

        node = (rc < 0) ? node->left : node->right;
    }

    /* not found */

    返回 NULL;
}


static ngx_resolver_node_t *
ngx_resolver_lookup_srv(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_node_t    *node, *sentinel;
    ngx_resolver_node_t  *rn;

    node = r->srv_rbtree.root;
    sentinel = r->srv_rbtree.sentinel;

    当 (node != sentinel) {

        如果 (hash < node->key) {
            node = node->left;
            continue;
        }

        如果 (hash > node->key) {
            node = node->right;
            continue;
        }

        /* hash == node->key */

        rn = ngx_resolver_node(node);

        rc = ngx_memn2cmp(name->data, rn->name, name->len, rn->nlen);

        如果 (rc == 0) {
            返回 rn;
        }

        node = (rc < 0) ? node->left : node->right;
    }

    /* not found */

    返回 NULL;
}


static ngx_resolver_node_t *
ngx_resolver_lookup_addr(ngx_resolver_t *r, in_addr_t addr)
{
    ngx_rbtree_node_t  *node, *sentinel;

    node = r->addr_rbtree.root;
    sentinel = r->addr_rbtree.sentinel;

    当 (node != sentinel) {

        如果 (addr < node->key) {
            node = node->left;
            continue;
        }

        如果 (addr > node->key) {
            node = node->right;
            continue;
        }

        /* addr == node->key */

        返回 ngx_resolver_node(node);
    }

    /* not found */

    返回 NULL;
}


#如果 (NGX_HAVE_INET6)

static ngx_resolver_node_t *
ngx_resolver_lookup_addr6(ngx_resolver_t *r, struct in6_addr *addr,
    uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_node_t    *node, *sentinel;
    ngx_resolver_node_t  *rn;

    node = r->addr6_rbtree.root;
    sentinel = r->addr6_rbtree.sentinel;

    当 (node != sentinel) {

        如果 (hash < node->key) {
            node = node->left;
            continue;
        }

        如果 (hash > node->key) {
            node = node->right;
            continue;
        }

        /* hash == node->key */

        rn = ngx_resolver_node(node);

        rc = ngx_memcmp(addr, &rn->addr6, 16);

        如果 (rc == 0) {
            返回 rn;
        }

        node = (rc < 0) ? node->left : node->right;
    }

    /* not found */

    返回 NULL;
}

#endif


static void
ngx_resolver_rbtree_insert_value(ngx_rbtree_node_t *temp,
    ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel)
{
    ngx_rbtree_node_t    **p;
    ngx_resolver_node_t   *rn, *rn_temp;

    为 ( ;; ) {

        如果 (node->key < temp->key) {

            p = &temp->left;

        } 否则 如果 (node->key > temp->key) {

            p = &temp->right;

        } 否则 { /* node->key == temp->key */

            rn = ngx_resolver_node(node);
            rn_temp = ngx_resolver_node(temp);

            p = (ngx_memn2cmp(rn->name, rn_temp->name, rn->nlen, rn_temp->nlen)
                 < 0) ? &temp->left : &temp->right;
        }

        如果 (*p == sentinel) {
            break;
        }

        temp = *p;
    }

    *p = node;
    node->parent = temp;
    node->left = sentinel;
    node->right = sentinel;
    ngx_rbt_red(node);
}


#如果 (NGX_HAVE_INET6)

static void
ngx_resolver_rbtree_insert_addr6_value(ngx_rbtree_node_t *temp,
    ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel)
{
    ngx_rbtree_node_t    **p;
    ngx_resolver_node_t   *rn, *rn_temp;

    为 ( ;; ) {

        如果 (node->key < temp->key) {

            p = &temp->left;

        } 否则 如果 (node->key > temp->key) {

            p = &temp->right;

        } 否则 { /* node->key == temp->key */

            rn = ngx_resolver_node(node);
            rn_temp = ngx_resolver_node(temp);

            p = (ngx_memcmp(&rn->addr6, &rn_temp->addr6, 16)
                 < 0) ? &temp->left : &temp->right;
        }

        如果 (*p == sentinel) {
            break;
        }

        temp = *p;
    }

    *p = node;
    node->parent = temp;
    node->left = sentinel;
    node->right = sentinel;
    ngx_rbt_red(node);
}

#endif


static ngx_int_t
ngx_resolver_create_name_query(ngx_resolver_t *r, ngx_resolver_node_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    size_t               len, nlen;
    ngx_uint_t           ident;
    ngx_resolver_qs_t   *qs;
    ngx_resolver_hdr_t  *query;

    nlen = name->len ? (1 + name->len + 1) : 1;

    len = sizeof(ngx_resolver_hdr_t) + nlen + sizeof(ngx_resolver_qs_t);

#如果 (NGX_HAVE_INET6)
    p = ngx_resolver_alloc(r, len * (r->ipv4 + r->ipv6));
#否则
    p = ngx_resolver_alloc(r, len);
#endif
    如果 (p == NULL) {
        返回 NGX_ERROR;
    }

    rn->qlen = (u_short) len;
    rn->query = p;

#如果 (NGX_HAVE_INET6)
    如果 (r->ipv6) {
        rn->query6 = r->ipv4 ? (p + len) : p;
    }
#endif

    query = (ngx_resolver_hdr_t *) p;

    如果 (r->ipv4) {
        ident = ngx_random();

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolve: \"%V\" A %i", name, ident & 0xffff);

        query->ident_hi = (u_char) ((ident >> 8) & 0xff);
        query->ident_lo = (u_char) (ident & 0xff);
    }

    /* recursion query */
    query->flags_hi = 1; query->flags_lo = 0;

    /* one question */
    query->nqs_hi = 0; query->nqs_lo = 1;
    query->nan_hi = 0; query->nan_lo = 0;
    query->nns_hi = 0; query->nns_lo = 0;
    query->nar_hi = 0; query->nar_lo = 0;

    p += sizeof(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    /* query type */
    qs->type_hi = 0; qs->type_lo = NGX_RESOLVE_A;

    /* IN query class */
    qs->class_hi = 0; qs->class_lo = 1;

    /* convert "www.example.com" to "\3www\7example\3com\0" */

    len = 0;
    p--;
    *p-- = '\0';

    如果 (name->len == 0)  {
        返回 NGX_DECLINED;
    }

    为 (s = name->data + name->len - 1; s >= name->data; s--) {
        如果 (*s != '.') {
            *p = *s;
            len++;

        } 否则 {
            如果 (len == 0 || len > 255) {
                返回 NGX_DECLINED;
            }

            *p = (u_char) len;
            len = 0;
        }

        p--;
    }

    如果 (len == 0 || len > 255) {
        返回 NGX_DECLINED;
    }

    *p = (u_char) len;

#如果 (NGX_HAVE_INET6)
    如果 (!r->ipv6) {
        返回 NGX_OK;
    }

    p = rn->query6;

    如果 (r->ipv4) {
        ngx_memcpy(p, rn->query, rn->qlen);
    }

    query = (ngx_resolver_hdr_t *) p;

    ident = ngx_random();

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve: \"%V\" AAAA %i", name, ident & 0xffff);

    query->ident_hi = (u_char) ((ident >> 8) & 0xff);
    query->ident_lo = (u_char) (ident & 0xff);

    p += sizeof(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    qs->type_lo = NGX_RESOLVE_AAAA;
#endif

    返回 NGX_OK;
}


static ngx_int_t
ngx_resolver_create_srv_query(ngx_resolver_t *r, ngx_resolver_node_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    size_t               len, nlen;
    ngx_uint_t           ident;
    ngx_resolver_qs_t   *qs;
    ngx_resolver_hdr_t  *query;

    nlen = name->len ? (1 + name->len + 1) : 1;

    len = sizeof(ngx_resolver_hdr_t) + nlen + sizeof(ngx_resolver_qs_t);

    p = ngx_resolver_alloc(r, len);
    如果 (p == NULL) {
        返回 NGX_ERROR;
    }

    rn->qlen = (u_short) len;
    rn->query = p;

    query = (ngx_resolver_hdr_t *) p;

    ident = ngx_random();

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve: \"%V\" SRV %i", name, ident & 0xffff);

    query->ident_hi = (u_char) ((ident >> 8) & 0xff);
    query->ident_lo = (u_char) (ident & 0xff);

    /* recursion query */
    query->flags_hi = 1; query->flags_lo = 0;

    /* one question */
    query->nqs_hi = 0; query->nqs_lo = 1;
    query->nan_hi = 0; query->nan_lo = 0;
    query->nns_hi = 0; query->nns_lo = 0;
    query->nar_hi = 0; query->nar_lo = 0;

    p += sizeof(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    /* query type */
    qs->type_hi = 0; qs->type_lo = NGX_RESOLVE_SRV;

    /* IN query class */
    qs->class_hi = 0; qs->class_lo = 1;

    /* converts "www.example.com" to "\3www\7example\3com\0" */

    len = 0;
    p--;
    *p-- = '\0';

    如果 (name->len == 0)  {
        返回 NGX_DECLINED;
    }

    为 (s = name->data + name->len - 1; s >= name->data; s--) {
        如果 (*s != '.') {
            *p = *s;
            len++;

        } 否则 {
            如果 (len == 0 || len > 255) {
                返回 NGX_DECLINED;
            }

            *p = (u_char) len;
            len = 0;
        }

        p--;
    }

    如果 (len == 0 || len > 255) {
        返回 NGX_DECLINED;
    }

    *p = (u_char) len;

    返回 NGX_OK;
}


static ngx_int_t
ngx_resolver_create_addr_query(ngx_resolver_t *r, ngx_resolver_node_t *rn,
    ngx_resolver_addr_t *addr)
{
    u_char               *p, *d;
    size_t                len;
    in_addr_t             inaddr;
    ngx_int_t             n;
    ngx_uint_t            ident;
    ngx_resolver_hdr_t   *query;
    struct sockaddr_in   *sin;
#如果 (NGX_HAVE_INET6)
    struct sockaddr_in6  *sin6;
#endif

    switch (addr->sockaddr->sa_family) {

#如果 (NGX_HAVE_INET6)
    case AF_INET6:
        len = sizeof(ngx_resolver_hdr_t)
              + 64 + sizeof(".ip6.arpa.") - 1
              + sizeof(ngx_resolver_qs_t);

        break;
#endif

    default: /* AF_INET */
        len = sizeof(ngx_resolver_hdr_t)
              + sizeof(".255.255.255.255.in-addr.arpa.") - 1
              + sizeof(ngx_resolver_qs_t);
    }

    p = ngx_resolver_alloc(r, len);
    如果 (p == NULL) {
        返回 NGX_ERROR;
    }

    rn->query = p;
    query = (ngx_resolver_hdr_t *) p;

    ident = ngx_random();

    query->ident_hi = (u_char) ((ident >> 8) & 0xff);
    query->ident_lo = (u_char) (ident & 0xff);

    /* recursion query */
    query->flags_hi = 1; query->flags_lo = 0;

    /* one question */
    query->nqs_hi = 0; query->nqs_lo = 1;
    query->nan_hi = 0; query->nan_lo = 0;
    query->nns_hi = 0; query->nns_lo = 0;
    query->nar_hi = 0; query->nar_lo = 0;

    p += sizeof(ngx_resolver_hdr_t);

    switch (addr->sockaddr->sa_family) {

#如果 (NGX_HAVE_INET6)
    case AF_INET6:
        sin6 = (struct sockaddr_in6 *) addr->sockaddr;

        为 (n = 15; n >= 0; n--) {
            p = ngx_sprintf(p, "\1%xd\1%xd",
                            sin6->sin6_addr.s6_addr[n] & 0xf,
                            (sin6->sin6_addr.s6_addr[n] >> 4) & 0xf);
        }

        p = ngx_cpymem(p, "\3ip6\4arpa\0", 10);

        break;
#endif

    default: /* AF_INET */

        sin = (struct sockaddr_in *) addr->sockaddr;
        inaddr = ntohl(sin->sin_addr.s_addr);

        为 (n = 0; n < 32; n += 8) {
            d = ngx_sprintf(&p[1], "%ud", (inaddr >> n) & 0xff);
            *p = (u_char) (d - &p[1]);
            p = d;
        }

        p = ngx_cpymem(p, "\7in-addr\4arpa\0", 14);
    }

    /* query type "PTR", IN query class */
    p = ngx_cpymem(p, "\0\14\0\1", 4);

    rn->qlen = (u_short) (p - rn->query);

    返回 NGX_OK;
}


static ngx_int_t
ngx_resolver_copy(ngx_resolver_t *r, ngx_str_t *name, u_char *buf, u_char *src,
    u_char *last)
{
    char        *err;
    u_char      *p, *dst;
    size_t       len;
    ngx_uint_t   i, n;

    p = src;
    len = 0;

    /*
     * compression pointers allow to create endless loop, so we set limit;
     * 128 pointers should be enough to store 255-byte name
     */

    为 (i = 0; i < 128; i++) {
        n = *p++;

        如果 (n == 0) {
            goto done;
        }

        如果 (n & 0xc0) {
            如果 ((n & 0xc0) != 0xc0) {
                err = "invalid label type in DNS response";
                goto invalid;
            }

            如果 (p >= last) {
                err = "name is out of DNS response";
                goto invalid;
            }

            n = ((n & 0x3f) << 8) + *p;
            p = &buf[n];

        } 否则 {
            len += 1 + n;
            p = &p[n];
        }

        如果 (p >= last) {
            err = "name is out of DNS response";
            goto invalid;
        }
    }

    err = "compression pointers loop in DNS response";

invalid:

    ngx_log_error(r->log_level, r->log, 0, err);

    返回 NGX_ERROR;

done:

    如果 (name == NULL) {
        返回 NGX_OK;
    }

    如果 (len == 0) {
        ngx_str_null(name);
        返回 NGX_OK;
    }

    dst = ngx_resolver_alloc(r, len);
    如果 (dst == NULL) {
        返回 NGX_ERROR;
    }

    name->data = dst;

    为 ( ;; ) {
        n = *src++;

        如果 (n == 0) {
            name->len = dst - name->data - 1;
            返回 NGX_OK;
        }

        如果 (n & 0xc0) {
            n = ((n & 0x3f) << 8) + *src;
            src = &buf[n];

        } 否则 {
            ngx_strlow(dst, src, n);
            dst += n;
            src += n;
            *dst++ = '.';
        }
    }
}


static ngx_int_t
ngx_resolver_set_timeout(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx)
{
    如果 (ctx->event || ctx->timeout == 0) {
        返回 NGX_OK;
    }

    ctx->event = ngx_resolver_calloc(r, sizeof(ngx_event_t));
    如果 (ctx->event == NULL) {
        返回 NGX_ERROR;
    }

    ctx->event->handler = ngx_resolver_timeout_handler;
    ctx->event->data = ctx;
    ctx->event->log = r->log;
    ctx->event->cancelable = ctx->cancelable;
    ctx->ident = -1;

    ngx_add_timer(ctx->event, ctx->timeout);

    返回 NGX_OK;
}


static void
ngx_resolver_timeout_handler(ngx_event_t *ev)
{
    ngx_resolver_ctx_t  *ctx;

    ctx = ev->data;

    ctx->state = NGX_RESOLVE_TIMEDOUT;

    ctx->handler(ctx);
}


static void
ngx_resolver_free_node(ngx_resolver_t *r, ngx_resolver_node_t *rn)
{
    ngx_uint_t  i;

    /* lock alloc mutex */

    如果 (rn->query) {
        ngx_resolver_free_locked(r, rn->query);
    }

    如果 (rn->name) {
        ngx_resolver_free_locked(r, rn->name);
    }

    如果 (rn->cnlen) {
        ngx_resolver_free_locked(r, rn->u.cname);
    }

    如果 (rn->naddrs > 1 && rn->naddrs != (u_short) -1) {
        ngx_resolver_free_locked(r, rn->u.addrs);
    }

#如果 (NGX_HAVE_INET6)
    如果 (rn->naddrs6 > 1 && rn->naddrs6 != (u_short) -1) {
        ngx_resolver_free_locked(r, rn->u6.addrs6);
    }
#endif

    如果 (rn->nsrvs) {
        为 (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
            如果 (rn->u.srvs[i].name.data) {
                ngx_resolver_free_locked(r, rn->u.srvs[i].name.data);
            }
        }

        ngx_resolver_free_locked(r, rn->u.srvs);
    }

    ngx_resolver_free_locked(r, rn);

    /* unlock alloc mutex */
}


static void *
ngx_resolver_alloc(ngx_resolver_t *r, size_t size)
{
    u_char  *p;

    /* lock alloc mutex */

    p = ngx_alloc(size, r->log);

    /* unlock alloc mutex */

    返回 p;
}


static void *
ngx_resolver_calloc(ngx_resolver_t *r, size_t size)
{
    u_char  *p;

    p = ngx_resolver_alloc(r, size);

    如果 (p) {
        ngx_memzero(p, size);
    }

    返回 p;
}


static void
ngx_resolver_free(ngx_resolver_t *r, void *p)
{
    /* lock alloc mutex */

    ngx_free(p);

    /* unlock alloc mutex */
}


static void
ngx_resolver_free_locked(ngx_resolver_t *r, void *p)
{
    ngx_free(p);
}


static void *
ngx_resolver_dup(ngx_resolver_t *r, void *src, size_t size)
{
    void  *dst;

    dst = ngx_resolver_alloc(r, size);

    如果 (dst == NULL) {
        返回 dst;
    }

    ngx_memcpy(dst, src, size);

    返回 dst;
}


static ngx_resolver_addr_t *
ngx_resolver_export(ngx_resolver_t *r, ngx_resolver_node_t *rn,
    ngx_uint_t rotate)
{
    ngx_uint_t            d, i, j, n;
    in_addr_t            *addr;
    ngx_sockaddr_t       *sockaddr;
    struct sockaddr_in   *sin;
    ngx_resolver_addr_t  *dst;
#如果 (NGX_HAVE_INET6)
    struct in6_addr      *addr6;
    struct sockaddr_in6  *sin6;
#endif

    n = rn->naddrs;
#如果 (NGX_HAVE_INET6)
    n += rn->naddrs6;
#endif

    dst = ngx_resolver_calloc(r, n * sizeof(ngx_resolver_addr_t));
    如果 (dst == NULL) {
        返回 NULL;
    }

    sockaddr = ngx_resolver_calloc(r, n * sizeof(ngx_sockaddr_t));
    如果 (sockaddr == NULL) {
        ngx_resolver_free(r, dst);
        返回 NULL;
    }

    i = 0;
    d = rotate ? ngx_random() % n : 0;

    如果 (rn->naddrs) {
        j = rotate ? ngx_random() % rn->naddrs : 0;

        addr = (rn->naddrs == 1) ? &rn->u.addr : rn->u.addrs;

        do {
            sin = &sockaddr[d].sockaddr_in;
            sin->sin_family = AF_INET;
            sin->sin_addr.s_addr = addr[j++];
            dst[d].sockaddr = (struct sockaddr *) sin;
            dst[d++].socklen = sizeof(struct sockaddr_in);

            如果 (d == n) {
                d = 0;
            }

            如果 (j == (ngx_uint_t) rn->naddrs) {
                j = 0;
            }
        } 当 (++i < (ngx_uint_t) rn->naddrs);
    }

#如果 (NGX_HAVE_INET6)
    如果 (rn->naddrs6) {
        j = rotate ? ngx_random() % rn->naddrs6 : 0;

        addr6 = (rn->naddrs6 == 1) ? &rn->u6.addr6 : rn->u6.addrs6;

        do {
            sin6 = &sockaddr[d].sockaddr_in6;
            sin6->sin6_family = AF_INET6;
            ngx_memcpy(sin6->sin6_addr.s6_addr, addr6[j++].s6_addr, 16);
            dst[d].sockaddr = (struct sockaddr *) sin6;
            dst[d++].socklen = sizeof(struct sockaddr_in6);

            如果 (d == n) {
                d = 0;
            }

            如果 (j == rn->naddrs6) {
                j = 0;
            }
        } 当 (++i < n);
    }
#endif

    返回 dst;
}


static void
ngx_resolver_report_srv(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx)
{
    ngx_uint_t                naddrs, nsrvs, nw, i, j, k, l, m, n, w;
    ngx_resolver_addr_t      *addrs;
    ngx_resolver_srv_name_t  *srvs;

    srvs = ctx->srvs;
    nsrvs = ctx->nsrvs;

    naddrs = 0;

    为 (i = 0; i < nsrvs; i++) {
        如果 (srvs[i].state == NGX_ERROR) {
            ctx->state = NGX_ERROR;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

            ctx->handler(ctx);
            返回;
        }

        naddrs += srvs[i].naddrs;
    }

    如果 (naddrs == 0) {
        ctx->state = srvs[0].state;

        为 (i = 0; i < nsrvs; i++) {
            如果 (srvs[i].state == NGX_RESOLVE_NXDOMAIN) {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                break;
            }
        }

        ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        返回;
    }

    addrs = ngx_resolver_calloc(r, naddrs * sizeof(ngx_resolver_addr_t));
    如果 (addrs == NULL) {
        ctx->state = NGX_ERROR;
        ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        返回;
    }

    i = 0;
    n = 0;

    do {
        nw = 0;

        为 (j = i; j < nsrvs; j++) {
            如果 (srvs[j].priority != srvs[i].priority) {
                break;
            }

            nw += srvs[j].naddrs * srvs[j].weight;
        }

        如果 (nw == 0) {
            goto next_srv;
        }

        w = ngx_random() % nw;

        为 (k = i; k < j; k++) {
            如果 (w < srvs[k].naddrs * srvs[k].weight) {
                break;
            }

            w -= srvs[k].naddrs * srvs[k].weight;
        }

        为 (l = i; l < j; l++) {

            为 (m = 0; m < srvs[k].naddrs; m++) {
                addrs[n].socklen = srvs[k].addrs[m].socklen;
                addrs[n].sockaddr = srvs[k].addrs[m].sockaddr;
                addrs[n].name = srvs[k].name;
                addrs[n].priority = srvs[k].priority;
                addrs[n].weight = srvs[k].weight;
                n++;
            }

            如果 (++k == j) {
                k = i;
            }
        }

next_srv:

        i = j;

    } 当 (i < ctx->nsrvs);

    ctx->state = NGX_OK;
    ctx->addrs = addrs;
    ctx->naddrs = naddrs;

    ctx->handler(ctx);

    ngx_resolver_free(r, addrs);
}


char *
ngx_resolver_strerror(ngx_int_t err)
{
    static char *errors[] = {
        "Format error",     /* FORMERR */
        "Server failure",   /* SERVFAIL */
        "Host not found",   /* NXDOMAIN */
        "Unimplemented",    /* NOTIMP */
        "Operation refused" /* REFUSED */
    };

    如果 (err > 0 && err < 6) {
        返回 errors[err - 1];
    }

    如果 (err == NGX_RESOLVE_TIMEDOUT) {
        返回 "Operation timed out";
    }

    返回 "Unknown error";
}


static u_char *
ngx_resolver_log_error(ngx_log_t *log, u_char *buf, size_t len)
{
    u_char                     *p;
    ngx_resolver_connection_t  *rec;

    p = buf;

    如果 (log->action) {
        p = ngx_snprintf(buf, len, " while %s", log->action);
        len -= p - buf;
    }

    rec = log->data;

    如果 (rec) {
        p = ngx_snprintf(p, len, ", resolver: %V", &rec->server);
    }

    返回 p;
}


static ngx_int_t
ngx_udp_connect(ngx_resolver_connection_t *rec)
{
    整数                rc;
    ngx_int_t          event;
    ngx_event_t       *rev, *wev;
    ngx_socket_t       s;
    ngx_connection_t  *c;

    s = ngx_socket(rec->sockaddr->sa_family, SOCK_DGRAM, 0);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "UDP socket %d", s);

    如果 (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_socket_n " failed");
        返回 NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->log);

    如果 (c == NULL) {
        如果 (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        返回 NGX_ERROR;
    }

    如果 (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        goto failed;
    }

    rev = c->read;
    wev = c->write;

    rev->log = &rec->log;
    wev->log = &rec->log;

    rec->udp = c;

    c->number = ngx_atomic_fetch_add(ngx_connection_counter, 1);

    c->start_time = ngx_current_msec;

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->log, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->number);

    rc = connect(s, rec->sockaddr, rec->socklen);

    如果 (rc == -1) {
        ngx_log_error(NGX_LOG_CRIT, &rec->log, ngx_socket_errno,
                      "connect() failed");

        goto failed;
    }

    /* UDP sockets are always ready to write */
    wev->ready = 1;

    event = (ngx_event_flags & NGX_USE_CLEAR_EVENT) ?
                /* kqueue, epoll */                 NGX_CLEAR_EVENT:
                /* select, poll, /dev/poll */       NGX_LEVEL_EVENT;
                /* eventport event type has no meaning: oneshot only */

    如果 (ngx_add_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        goto failed;
    }

    返回 NGX_OK;

failed:

    ngx_close_connection(c);
    rec->udp = NULL;

    返回 NGX_ERROR;
}


static ngx_int_t
ngx_tcp_connect(ngx_resolver_connection_t *rec)
{
    整数                rc;
    ngx_int_t          event;
    ngx_err_t          err;
    ngx_uint_t         level;
    ngx_socket_t       s;
    ngx_event_t       *rev, *wev;
    ngx_connection_t  *c;

    s = ngx_socket(rec->sockaddr->sa_family, SOCK_STREAM, 0);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "TCP socket %d", s);

    如果 (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_socket_n " failed");
        返回 NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->log);

    如果 (c == NULL) {
        如果 (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        返回 NGX_ERROR;
    }

    如果 (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        goto failed;
    }

    rev = c->read;
    wev = c->write;

    rev->log = &rec->log;
    wev->log = &rec->log;

    rec->tcp = c;

    c->number = ngx_atomic_fetch_add(ngx_connection_counter, 1);

    c->start_time = ngx_current_msec;

    如果 (ngx_add_conn) {
        如果 (ngx_add_conn(c) == NGX_ERROR) {
            goto failed;
        }
    }

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->log, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->number);

    rc = connect(s, rec->sockaddr, rec->socklen);

    如果 (rc == -1) {
        err = ngx_socket_errno;


        如果 (err != NGX_EINPROGRESS
#如果 (NGX_WIN32)
            /* Winsock returns WSAEWOULDBLOCK (NGX_EAGAIN) */
            && err != NGX_EAGAIN
#endif
            )
        {
            如果 (err == NGX_ECONNREFUSED
#如果 (NGX_LINUX)
                /*
                 * Linux returns EAGAIN instead of ECONNREFUSED
                 * for unix sockets if listen queue is full
                 */
                || err == NGX_EAGAIN
#endif
                || err == NGX_ECONNRESET
                || err == NGX_ENETDOWN
                || err == NGX_ENETUNREACH
                || err == NGX_EHOSTDOWN
                || err == NGX_EHOSTUNREACH)
            {
                level = NGX_LOG_ERR;

            } 否则 {
                level = NGX_LOG_CRIT;
            }

            ngx_log_error(level, &rec->log, err, "connect() to %V failed",
                          &rec->server);

            ngx_close_connection(c);
            rec->tcp = NULL;

            返回 NGX_ERROR;
        }
    }

    如果 (ngx_add_conn) {
        如果 (rc == -1) {

            /* NGX_EINPROGRESS */

            返回 NGX_AGAIN;
        }

        ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "connected");

        wev->ready = 1;

        返回 NGX_OK;
    }

    如果 (ngx_event_flags & NGX_USE_CLEAR_EVENT) {

        /* kqueue */

        event = NGX_CLEAR_EVENT;

    } 否则 {

        /* select, poll, /dev/poll */

        event = NGX_LEVEL_EVENT;
    }

    如果 (ngx_add_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        goto failed;
    }

    如果 (rc == -1) {

        /* NGX_EINPROGRESS */

        如果 (ngx_add_event(wev, NGX_WRITE_EVENT, event) != NGX_OK) {
            goto failed;
        }

        返回 NGX_AGAIN;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "connected");

    wev->ready = 1;

    返回 NGX_OK;

failed:

    ngx_close_connection(c);
    rec->tcp = NULL;

    返回 NGX_ERROR;
}


static ngx_int_t
ngx_resolver_cmp_srvs(const void *one, const void *two)
{
    ngx_int_t            p1, p2;
    ngx_resolver_srv_t  *first, *second;

    first = (ngx_resolver_srv_t *) one;
    second = (ngx_resolver_srv_t *) two;

    p1 = first->priority;
    p2 = second->priority;

    返回 p1 - p2;
}
