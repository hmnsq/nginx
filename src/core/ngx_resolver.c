
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


typedef sa_nracanaa {
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


typedef sa_nracanaa {
    u_char  type_hi;
    u_char  type_lo;
    u_char  class_hi;
    u_char  class_lo;
} ngx_resolver_qs_t;


typedef sa_nracanaa {
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

#အကယ်၍ (NGX_HAVE_INET6)
static void ngx_resolver_rbtree_insert_addr6_value(ngx_rbtree_node_t *temp,
    ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel);
static ngx_resolver_node_t *ngx_resolver_lookup_addr6(ngx_resolver_t *r,
    sa_nracanaa in6_addr *addr, uint32_t hash);
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

    r = ngx_pcalloc(cf->pool, parimaa_na(ngx_resolver_t));
    အကယ်၍ (r == NULL) {
        ပြန်ပေး NULL;
    }

    r->event = ngx_pcalloc(cf->pool, parimaa_na(ngx_event_t));
    အကယ်၍ (r->event == NULL) {
        ပြန်ပေး NULL;
    }

    cln = ngx_pool_cleanup_add(cf->pool, 0);
    အကယ်၍ (cln == NULL) {
        ပြန်ပေး NULL;
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

#အကယ်၍ (NGX_HAVE_INET6)
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

    အကယ်၍ (n) {
        အကယ်၍ (ngx_array_init(&r->connections, cf->pool, n,
                           parimaa_na(ngx_resolver_connection_t))
            != NGX_OK)
        {
            ပြန်ပေး NULL;
        }
    }

    အတွက် (i = 0; i < n; i++) {
        အကယ်၍ (ngx_strncmp(names[i].data, "valid=", 6) == 0) {
            s.len = names[i].len - 6;
            s.data = names[i].data + 6;

            r->valid = ngx_parse_time(&s, 1);

            အကယ်၍ (r->valid == (time_t) NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &names[i]);
                ပြန်ပေး NULL;
            }

            continue;
        }

#အကယ်၍ (NGX_HAVE_INET6)
        အကယ်၍ (ngx_strncmp(names[i].data, "ipv4=", 5) == 0) {

            အကယ်၍ (ngx_strcmp(&names[i].data[5], "on") == 0) {
                r->ipv4 = 1;

            } မဟုတ်ပါက အကယ်၍ (ngx_strcmp(&names[i].data[5], "off") == 0) {
                r->ipv4 = 0;

            } မဟုတ်ပါက {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &names[i]);
                ပြန်ပေး NULL;
            }

            continue;
        }

        အကယ်၍ (ngx_strncmp(names[i].data, "ipv6=", 5) == 0) {

            အကယ်၍ (ngx_strcmp(&names[i].data[5], "on") == 0) {
                r->ipv6 = 1;

            } မဟုတ်ပါက အကယ်၍ (ngx_strcmp(&names[i].data[5], "off") == 0) {
                r->ipv6 = 0;

            } မဟုတ်ပါက {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &names[i]);
                ပြန်ပေး NULL;
            }

            continue;
        }
#endif

        ngx_memzero(&u, parimaa_na(ngx_url_t));

        u.url = names[i];
        u.default_port = 53;

        အကယ်၍ (ngx_parse_url(cf->pool, &u) != NGX_OK) {
            အကယ်၍ (u.err) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "%s in resolver \"%V\"",
                                   u.err, &u.url);
            }

            ပြန်ပေး NULL;
        }

        rec = ngx_array_push_n(&r->connections, u.naddrs);
        အကယ်၍ (rec == NULL) {
            ပြန်ပေး NULL;
        }

        ngx_memzero(rec, u.naddrs * parimaa_na(ngx_resolver_connection_t));

        အတွက် (j = 0; j < u.naddrs; j++) {
            rec[j].sockaddr = u.addrs[j].sockaddr;
            rec[j].socklen = u.addrs[j].socklen;
            rec[j].server = u.addrs[j].name;
            rec[j].resolver = r;
        }
    }

#အကယ်၍ (NGX_HAVE_INET6)
    အကယ်၍ (r->ipv4 + r->ipv6 == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"ipv4\" and \"ipv6\" cannot both be \"off\"");
        ပြန်ပေး NULL;
    }
#endif

    အကယ်၍ (n && r->connections.nelts == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "no name servers defined");
        ပြန်ပေး NULL;
    }

    ပြန်ပေး r;
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

#အကယ်၍ (NGX_HAVE_INET6)
    ngx_resolver_cleanup_tree(r, &r->addr6_rbtree);
#endif

    အကယ်၍ (r->event->timer_set) {
        ngx_del_timer(r->event);
    }

    rec = r->connections.elts;

    အတွက် (i = 0; i < r->connections.nelts; i++) {
        အကယ်၍ (rec[i].udp) {
            ngx_close_connection(rec[i].udp);
        }

        အကယ်၍ (rec[i].tcp) {
            ngx_close_connection(rec[i].tcp);
        }

        အကယ်၍ (rec[i].read_buf) {
            ngx_resolver_free(r, rec[i].read_buf->start);
            ngx_resolver_free(r, rec[i].read_buf);
        }

        အကယ်၍ (rec[i].write_buf) {
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

    နေစဉ် (tree->root != tree->sentinel) {

        rn = ngx_resolver_node(ngx_rbtree_min(tree->root, tree->sentinel));

        ngx_queue_remove(&rn->queue);

        အတွက် (ctx = rn->waiting; ctx; ctx = next) {
            next = ctx->next;

            အကယ်၍ (ctx->event) {
                အကယ်၍ (ctx->event->timer_set) {
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

    အကယ်၍ (temp) {
        addr = ngx_inet_addr(temp->name.data, temp->name.len);

        အကယ်၍ (addr != INADDR_NONE) {
            temp->resolver = r;
            temp->state = NGX_OK;
            temp->naddrs = 1;
            temp->addrs = &temp->addr;
            temp->addr.sockaddr = (sa_nracanaa sockaddr *) &temp->sin;
            temp->addr.socklen = parimaa_na(sa_nracanaa sockaddr_in);
            ngx_memzero(&temp->sin, parimaa_na(sa_nracanaa sockaddr_in));
            temp->sin.sin_family = AF_INET;
            temp->sin.sin_addr.s_addr = addr;
            temp->quick = 1;

            ပြန်ပေး temp;
        }
    }

    အကယ်၍ (r->connections.nelts == 0) {
        ပြန်ပေး NGX_NO_RESOLVER;
    }

    ctx = ngx_resolver_calloc(r, parimaa_na(ngx_resolver_ctx_t));

    အကယ်၍ (ctx) {
        ctx->resolver = r;
    }

    ပြန်ပေး ctx;
}


ngx_int_t
ngx_resolve_name(ngx_resolver_ctx_t *ctx)
{
    size_t           slen;
    ngx_int_t        rc;
    ngx_str_t        name;
    ngx_resolver_t  *r;

    r = ctx->resolver;

    အကယ်၍ (ctx->name.len > 0 && ctx->name.data[ctx->name.len - 1] == '.') {
        ctx->name.len--;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve: \"%V\"", &ctx->name);

    အကယ်၍ (ctx->quick) {
        ctx->handler(ctx);
        ပြန်ပေး NGX_OK;
    }

    အကယ်၍ (ctx->service.len) {
        slen = ctx->service.len;

        အကယ်၍ (ngx_strlchr(ctx->service.data,
                        ctx->service.data + ctx->service.len, '.')
            == NULL)
        {
            slen += parimaa_na("_._tcp") - 1;
        }

        name.len = slen + 1 + ctx->name.len;

        name.data = ngx_resolver_alloc(r, name.len);
        အကယ်၍ (name.data == NULL) {
            goto failed;
        }

        အကယ်၍ (slen == ctx->service.len) {
            ngx_sprintf(name.data, "%V.%V", &ctx->service, &ctx->name);

        } မဟုတ်ပါက {
            ngx_sprintf(name.data, "_%V._tcp.%V", &ctx->service, &ctx->name);
        }

        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &name);

        ngx_resolver_free(r, name.data);

    } မဟုတ်ပါက {
        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &ctx->name);
    }

    အကယ်၍ (rc == NGX_OK) {
        ပြန်ပေး NGX_OK;
    }

    /* unlock name mutex */

    အကယ်၍ (rc == NGX_AGAIN) {
        ပြန်ပေး NGX_OK;
    }

    /* NGX_ERROR */

    အကယ်၍ (ctx->event) {
        ngx_resolver_free(r, ctx->event);
    }

failed:

    ngx_resolver_free(r, ctx);

    ပြန်ပေး NGX_ERROR;
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

    အကယ်၍ (ctx->quick) {
        ပြန်ပေး;
    }

    အကယ်၍ (ctx->event && ctx->event->timer_set) {
        ngx_del_timer(ctx->event);
    }

    /* lock name mutex */

    အကယ်၍ (ctx->nsrvs) {
        အတွက် (i = 0; i < ctx->nsrvs; i++) {
            အကယ်၍ (ctx->srvs[i].ctx) {
                ngx_resolve_name_done(ctx->srvs[i].ctx);
            }

            အကယ်၍ (ctx->srvs[i].addrs) {
                ngx_resolver_free(r, ctx->srvs[i].addrs->sockaddr);
                ngx_resolver_free(r, ctx->srvs[i].addrs);
            }

            ngx_resolver_free(r, ctx->srvs[i].name.data);
        }

        ngx_resolver_free(r, ctx->srvs);
    }

    အကယ်၍ (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->node;

        အကယ်၍ (rn) {
            p = &rn->waiting;
            w = rn->waiting;

            နေစဉ် (w) {
                အကယ်၍ (w == ctx) {
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

    အကယ်၍ (ctx->service.len) {
        ngx_resolver_expire(r, &r->srv_rbtree, &r->srv_expire_queue);

    } မဟုတ်ပါက {
        ngx_resolver_expire(r, &r->name_rbtree, &r->name_expire_queue);
    }

    /* unlock name mutex */

    /* lock alloc mutex */

    အကယ်၍ (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    အကယ်၍ (r->event->timer_set && ngx_resolver_resend_empty(r)) {
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

    အကယ်၍ (ctx->service.len) {
        rn = ngx_resolver_lookup_srv(r, name, hash);

        tree = &r->srv_rbtree;
        resend_queue = &r->srv_resend_queue;
        expire_queue = &r->srv_expire_queue;

    } မဟုတ်ပါက {
        rn = ngx_resolver_lookup_name(r, name, hash);

        tree = &r->name_rbtree;
        resend_queue = &r->name_resend_queue;
        expire_queue = &r->name_expire_queue;
    }

    အကယ်၍ (rn) {

        /* ctx can be a list after NGX_RESOLVE_CNAME */
        အတွက် (last = ctx; last->next; last = last->next);

        အကယ်၍ (rn->valid >= ngx_time()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0, "resolve cached");

            ngx_queue_remove(&rn->queue);

            rn->expire = ngx_time() + r->expire;

            ngx_queue_insert_head(expire_queue, &rn->queue);

            naddrs = (rn->naddrs == (u_short) -1) ? 0 : rn->naddrs;
#အကယ်၍ (NGX_HAVE_INET6)
            naddrs += (rn->naddrs6 == (u_short) -1) ? 0 : rn->naddrs6;
#endif

            အကယ်၍ (naddrs) {

                အကယ်၍ (naddrs == 1 && rn->naddrs == 1) {
                    addrs = NULL;

                } မဟုတ်ပါက {
                    addrs = ngx_resolver_export(r, rn, 1);
                    အကယ်၍ (addrs == NULL) {
                        ပြန်ပေး NGX_ERROR;
                    }
                }

                last->next = rn->waiting;
                rn->waiting = NULL;

                /* unlock name mutex */

                do {
                    ctx->state = NGX_OK;
                    ctx->valid = rn->valid;
                    ctx->naddrs = naddrs;

                    အကယ်၍ (addrs == NULL) {
                        ctx->addrs = &ctx->addr;
                        ctx->addr.sockaddr = (sa_nracanaa sockaddr *) &ctx->sin;
                        ctx->addr.socklen = parimaa_na(sa_nracanaa sockaddr_in);
                        ngx_memzero(&ctx->sin, parimaa_na(sa_nracanaa sockaddr_in));
                        ctx->sin.sin_family = AF_INET;
                        ctx->sin.sin_addr.s_addr = rn->u.addr;

                    } မဟုတ်ပါက {
                        ctx->addrs = addrs;
                    }

                    next = ctx->next;

                    ctx->handler(ctx);

                    ctx = next;
                } နေစဉ် (ctx);

                အကယ်၍ (addrs != NULL) {
                    ngx_resolver_free(r, addrs->sockaddr);
                    ngx_resolver_free(r, addrs);
                }

                ပြန်ပေး NGX_OK;
            }

            အကယ်၍ (rn->nsrvs) {
                last->next = rn->waiting;
                rn->waiting = NULL;

                /* unlock name mutex */

                do {
                    next = ctx->next;

                    ngx_resolver_resolve_srv_names(ctx, rn);

                    ctx = next;
                } နေစဉ် (ctx);

                ပြန်ပေး NGX_OK;
            }

            /* NGX_RESOLVE_CNAME */

            အကယ်၍ (ctx->recursion++ < NGX_RESOLVER_MAX_RECURSION) {

                cname.len = rn->cnlen;
                cname.data = rn->u.cname;

                ပြန်ပေး ngx_resolve_name_locked(r, ctx, &cname);
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
            } နေစဉ် (ctx);

            ပြန်ပေး NGX_OK;
        }

        အကယ်၍ (rn->waiting) {
            အကယ်၍ (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
                ပြန်ပေး NGX_ERROR;
            }

            last->next = rn->waiting;
            rn->waiting = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;

            do {
                ctx->node = rn;
                ctx = ctx->next;
            } နေစဉ် (ctx);

            ပြန်ပေး NGX_AGAIN;
        }

        ngx_queue_remove(&rn->queue);

        /* lock alloc mutex */

        အကယ်၍ (rn->query) {
            ngx_resolver_free_locked(r, rn->query);
            rn->query = NULL;
#အကယ်၍ (NGX_HAVE_INET6)
            rn->query6 = NULL;
#endif
        }

        အကယ်၍ (rn->cnlen) {
            ngx_resolver_free_locked(r, rn->u.cname);
        }

        အကယ်၍ (rn->naddrs > 1 && rn->naddrs != (u_short) -1) {
            ngx_resolver_free_locked(r, rn->u.addrs);
        }

#အကယ်၍ (NGX_HAVE_INET6)
        အကယ်၍ (rn->naddrs6 > 1 && rn->naddrs6 != (u_short) -1) {
            ngx_resolver_free_locked(r, rn->u6.addrs6);
        }
#endif

        အကယ်၍ (rn->nsrvs) {
            အတွက် (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
                အကယ်၍ (rn->u.srvs[i].name.data) {
                    ngx_resolver_free_locked(r, rn->u.srvs[i].name.data);
                }
            }

            ngx_resolver_free_locked(r, rn->u.srvs);
        }

        /* unlock alloc mutex */

    } မဟုတ်ပါက {

        rn = ngx_resolver_alloc(r, parimaa_na(ngx_resolver_node_t));
        အကယ်၍ (rn == NULL) {
            ပြန်ပေး NGX_ERROR;
        }

        rn->name = ngx_resolver_dup(r, name->data, name->len);
        အကယ်၍ (rn->name == NULL) {
            ngx_resolver_free(r, rn);
            ပြန်ပေး NGX_ERROR;
        }

        rn->node.key = hash;
        rn->nlen = (u_short) name->len;
        rn->query = NULL;
#အကယ်၍ (NGX_HAVE_INET6)
        rn->query6 = NULL;
#endif

        ngx_rbtree_insert(tree, &rn->node);
    }

    အကယ်၍ (ctx->service.len) {
        rc = ngx_resolver_create_srv_query(r, rn, name);

    } မဟုတ်ပါက {
        rc = ngx_resolver_create_name_query(r, rn, name);
    }

    အကယ်၍ (rc == NGX_ERROR) {
        goto failed;
    }

    အကယ်၍ (rc == NGX_DECLINED) {
        ngx_rbtree_delete(tree, &rn->node);

        ngx_resolver_free(r, rn->query);
        ngx_resolver_free(r, rn->name);
        ngx_resolver_free(r, rn);

        do {
            ctx->state = NGX_RESOLVE_NXDOMAIN;
            next = ctx->next;

            ctx->handler(ctx);

            ctx = next;
        } နေစဉ် (ctx);

        ပြန်ပေး NGX_OK;
    }

    rn->last_connection = r->last_connection++;
    အကယ်၍ (r->last_connection == r->connections.nelts) {
        r->last_connection = 0;
    }

    rn->naddrs = r->ipv4 ? (u_short) -1 : 0;
    rn->tcp = 0;
#အကယ်၍ (NGX_HAVE_INET6)
    rn->naddrs6 = r->ipv6 ? (u_short) -1 : 0;
    rn->tcp6 = 0;
#endif
    rn->nsrvs = 0;

    အကယ်၍ (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->last_connection++;
        အကယ်၍ (rn->last_connection == r->connections.nelts) {
            rn->last_connection = 0;
        }

        (void) ngx_resolver_send_query(r, rn);
    }

    အကယ်၍ (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
        goto failed;
    }

    အကယ်၍ (ngx_resolver_resend_empty(r)) {
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
    } နေစဉ် (ctx);

    ပြန်ပေး NGX_AGAIN;

failed:

    ngx_rbtree_delete(tree, &rn->node);

    အကယ်၍ (rn->query) {
        ngx_resolver_free(r, rn->query);
    }

    ngx_resolver_free(r, rn->name);

    ngx_resolver_free(r, rn);

    ပြန်ပေး NGX_ERROR;
}


ngx_int_t
ngx_resolve_addr(ngx_resolver_ctx_t *ctx)
{
    u_char               *name;
    in_addr_t             addr;
    ngx_queue_t          *resend_queue, *expire_queue;
    ngx_rbtree_t         *tree;
    ngx_resolver_t       *r;
    sa_nracanaa sockaddr_in   *sin;
    ngx_resolver_node_t  *rn;
#အကယ်၍ (NGX_HAVE_INET6)
    uint32_t              hash;
    sa_nracanaa sockaddr_in6  *sin6;
#endif

#အကယ်၍ (NGX_SUPPRESS_WARN)
    addr = 0;
#အကယ်၍ (NGX_HAVE_INET6)
    hash = 0;
    sin6 = NULL;
#endif
#endif

    r = ctx->resolver;

    switch (ctx->addr.sockaddr->sa_family) {

#အကယ်၍ (NGX_HAVE_INET6)
    pak_sa AF_INET6:
        sin6 = (sa_nracanaa sockaddr_in6 *) ctx->addr.sockaddr;
        hash = ngx_crc32_short(sin6->sin6_addr.s6_addr, 16);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr6(r, &sin6->sin6_addr, hash);

        tree = &r->addr6_rbtree;
        resend_queue = &r->addr6_resend_queue;
        expire_queue = &r->addr6_expire_queue;

        bha_nga;
#endif

    default: /* AF_INET */
        sin = (sa_nracanaa sockaddr_in *) ctx->addr.sockaddr;
        addr = ntohl(sin->sin_addr.s_addr);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        tree = &r->addr_rbtree;
        resend_queue = &r->addr_resend_queue;
        expire_queue = &r->addr_expire_queue;
    }

    အကယ်၍ (rn) {

        အကယ်၍ (rn->valid >= ngx_time()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0, "resolve cached");

            ngx_queue_remove(&rn->queue);

            rn->expire = ngx_time() + r->expire;

            ngx_queue_insert_head(expire_queue, &rn->queue);

            name = ngx_resolver_dup(r, rn->name, rn->nlen);
            အကယ်၍ (name == NULL) {
                ngx_resolver_free(r, ctx);
                ပြန်ပေး NGX_ERROR;
            }

            ctx->name.len = rn->nlen;
            ctx->name.data = name;

            /* unlock addr mutex */

            ctx->state = NGX_OK;
            ctx->valid = rn->valid;

            ctx->handler(ctx);

            ngx_resolver_free(r, name);

            ပြန်ပေး NGX_OK;
        }

        အကယ်၍ (rn->waiting) {
            အကယ်၍ (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
                ပြန်ပေး NGX_ERROR;
            }

            ctx->next = rn->waiting;
            rn->waiting = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;
            ctx->node = rn;

            /* unlock addr mutex */

            ပြန်ပေး NGX_OK;
        }

        ngx_queue_remove(&rn->queue);

        ngx_resolver_free(r, rn->query);
        rn->query = NULL;
#အကယ်၍ (NGX_HAVE_INET6)
        rn->query6 = NULL;
#endif

    } မဟုတ်ပါက {
        rn = ngx_resolver_alloc(r, parimaa_na(ngx_resolver_node_t));
        အကယ်၍ (rn == NULL) {
            goto failed;
        }

        switch (ctx->addr.sockaddr->sa_family) {

#အကယ်၍ (NGX_HAVE_INET6)
        pak_sa AF_INET6:
            rn->addr6 = sin6->sin6_addr;
            rn->node.key = hash;
            bha_nga;
#endif

        default: /* AF_INET */
            rn->node.key = addr;
        }

        rn->query = NULL;
#အကယ်၍ (NGX_HAVE_INET6)
        rn->query6 = NULL;
#endif

        ngx_rbtree_insert(tree, &rn->node);
    }

    အကယ်၍ (ngx_resolver_create_addr_query(r, rn, &ctx->addr) != NGX_OK) {
        goto failed;
    }

    rn->last_connection = r->last_connection++;
    အကယ်၍ (r->last_connection == r->connections.nelts) {
        r->last_connection = 0;
    }

    rn->naddrs = (u_short) -1;
    rn->tcp = 0;
#အကယ်၍ (NGX_HAVE_INET6)
    rn->naddrs6 = (u_short) -1;
    rn->tcp6 = 0;
#endif
    rn->nsrvs = 0;

    အကယ်၍ (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->last_connection++;
        အကယ်၍ (rn->last_connection == r->connections.nelts) {
            rn->last_connection = 0;
        }

        (void) ngx_resolver_send_query(r, rn);
    }

    အကယ်၍ (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
        goto failed;
    }

    အကယ်၍ (ngx_resolver_resend_empty(r)) {
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

    ပြန်ပေး NGX_OK;

failed:

    အကယ်၍ (rn) {
        ngx_rbtree_delete(tree, &rn->node);

        အကယ်၍ (rn->query) {
            ngx_resolver_free(r, rn->query);
        }

        ngx_resolver_free(r, rn);
    }

    /* unlock addr mutex */

    အကယ်၍ (ctx->event) {
        ngx_resolver_free(r, ctx->event);
    }

    ngx_resolver_free(r, ctx);

    ပြန်ပေး NGX_ERROR;
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

#အကယ်၍ (NGX_HAVE_INET6)
    pak_sa AF_INET6:
        tree = &r->addr6_rbtree;
        expire_queue = &r->addr6_expire_queue;
        bha_nga;
#endif

    default: /* AF_INET */
        tree = &r->addr_rbtree;
        expire_queue = &r->addr_expire_queue;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve addr done: %i", ctx->state);

    အကယ်၍ (ctx->event && ctx->event->timer_set) {
        ngx_del_timer(ctx->event);
    }

    /* lock addr mutex */

    အကယ်၍ (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->node;

        အကယ်၍ (rn) {
            p = &rn->waiting;
            w = rn->waiting;

            နေစဉ် (w) {
                အကယ်၍ (w == ctx) {
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

    အကယ်၍ (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    အကယ်၍ (r->event->timer_set && ngx_resolver_resend_empty(r)) {
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

    အတွက် (i = 0; i < 2; i++) {
        အကယ်၍ (ngx_queue_empty(queue)) {
            ပြန်ပေး;
        }

        q = ngx_queue_last(queue);

        rn = ngx_queue_data(q, ngx_resolver_node_t, queue);

        အကယ်၍ (now <= rn->expire) {
            ပြန်ပေး;
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

    အကယ်၍ (rec->log.handler == NULL) {
        rec->log = *r->log;
        rec->log.handler = ngx_resolver_log_error;
        rec->log.data = rec;
        rec->log.action = "resolving";
    }

    အကယ်၍ (rn->query && rn->naddrs == (u_short) -1) {
        rc = rn->tcp ? ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen)
                     : ngx_resolver_send_udp_query(r, rec, rn->query, rn->qlen);

        အကယ်၍ (rc != NGX_OK) {
            ပြန်ပေး rc;
        }
    }

#အကယ်၍ (NGX_HAVE_INET6)

    အကယ်၍ (rn->query6 && rn->naddrs6 == (u_short) -1) {
        rc = rn->tcp6
                    ? ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen)
                    : ngx_resolver_send_udp_query(r, rec, rn->query6, rn->qlen);

        အကယ်၍ (rc != NGX_OK) {
            ပြန်ပေး rc;
        }
    }

#endif

    ပြန်ပေး NGX_OK;
}


static ngx_int_t
ngx_resolver_send_udp_query(ngx_resolver_t *r, ngx_resolver_connection_t  *rec,
    u_char *query, u_short qlen)
{
    ssize_t  n;

    အကယ်၍ (rec->udp == NULL) {
        အကယ်၍ (ngx_udp_connect(rec) != NGX_OK) {
            ပြန်ပေး NGX_ERROR;
        }

        rec->udp->data = rec;
        rec->udp->read->handler = ngx_resolver_udp_read;
        rec->udp->read->resolver = 1;
    }

    n = ngx_send(rec->udp, query, qlen);

    အကယ်၍ (n == NGX_ERROR) {
        goto failed;
    }

    အကယ်၍ ((size_t) n != (size_t) qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->log, 0, "send() incomplete");
        goto failed;
    }

    ပြန်ပေး NGX_OK;

failed:

    ngx_close_connection(rec->udp);
    rec->udp = NULL;

    ပြန်ပေး NGX_ERROR;
}


static ngx_int_t
ngx_resolver_send_tcp_query(ngx_resolver_t *r, ngx_resolver_connection_t *rec,
    u_char *query, u_short qlen)
{
    ngx_buf_t  *b;
    ngx_int_t   rc;

    rc = NGX_OK;

    အကယ်၍ (rec->tcp == NULL) {
        b = rec->read_buf;

        အကယ်၍ (b == NULL) {
            b = ngx_resolver_calloc(r, parimaa_na(ngx_buf_t));
            အကယ်၍ (b == NULL) {
                ပြန်ပေး NGX_ERROR;
            }

            b->start = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_RSIZE);
            အကယ်၍ (b->start == NULL) {
                ngx_resolver_free(r, b);
                ပြန်ပေး NGX_ERROR;
            }

            b->end = b->start + NGX_RESOLVER_TCP_RSIZE;

            rec->read_buf = b;
        }

        b->pos = b->start;
        b->last = b->start;

        b = rec->write_buf;

        အကယ်၍ (b == NULL) {
            b = ngx_resolver_calloc(r, parimaa_na(ngx_buf_t));
            အကယ်၍ (b == NULL) {
                ပြန်ပေး NGX_ERROR;
            }

            b->start = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_WSIZE);
            အကယ်၍ (b->start == NULL) {
                ngx_resolver_free(r, b);
                ပြန်ပေး NGX_ERROR;
            }

            b->end = b->start + NGX_RESOLVER_TCP_WSIZE;

            rec->write_buf = b;
        }

        b->pos = b->start;
        b->last = b->start;

        rc = ngx_tcp_connect(rec);
        အကယ်၍ (rc == NGX_ERROR) {
            ပြန်ပေး NGX_ERROR;
        }

        rec->tcp->data = rec;
        rec->tcp->write->handler = ngx_resolver_tcp_write;
        rec->tcp->write->cancelable = 1;
        rec->tcp->read->handler = ngx_resolver_tcp_read;
        rec->tcp->read->resolver = 1;

        ngx_add_timer(rec->tcp->write, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    b = rec->write_buf;

    အကယ်၍ (b->end - b->last <  2 + qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->log, 0, "buffer overflow");
        ပြန်ပေး NGX_ERROR;
    }

    *b->last++ = (u_char) (qlen >> 8);
    *b->last++ = (u_char) qlen;
    b->last = ngx_cpymem(b->last, query, qlen);

    အကယ်၍ (rc == NGX_OK) {
        ngx_resolver_tcp_write(rec->tcp->write);
    }

    ပြန်ပေး NGX_OK;
}


static void
ngx_resolver_resend_handler(ngx_event_t *ev)
{
    time_t           timer, atimer, stimer, ntimer;
#အကယ်၍ (NGX_HAVE_INET6)
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

#အကယ်၍ (NGX_HAVE_INET6)

    /* lock addr6 mutex */

    a6timer = ngx_resolver_resend(r, &r->addr6_rbtree, &r->addr6_resend_queue);

    /* unlock addr6 mutex */

#endif

    timer = ntimer;

    အကယ်၍ (timer == 0) {
        timer = atimer;

    } မဟုတ်ပါက အကယ်၍ (atimer) {
        timer = ngx_min(timer, atimer);
    }

    အကယ်၍ (timer == 0) {
        timer = stimer;

    } မဟုတ်ပါက အကယ်၍ (stimer) {
        timer = ngx_min(timer, stimer);
    }

#အကယ်၍ (NGX_HAVE_INET6)

    အကယ်၍ (timer == 0) {
        timer = a6timer;

    } မဟုတ်ပါက အကယ်၍ (a6timer) {
        timer = ngx_min(timer, a6timer);
    }

#endif

    အကယ်၍ (timer) {
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

    အတွက် ( ;; ) {
        အကယ်၍ (ngx_queue_empty(queue)) {
            ပြန်ပေး 0;
        }

        q = ngx_queue_last(queue);

        rn = ngx_queue_data(q, ngx_resolver_node_t, queue);

        အကယ်၍ (now < rn->expire) {
            ပြန်ပေး rn->expire - now;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver resend \"%*s\" %p",
                       (size_t) rn->nlen, rn->name, rn->waiting);

        ngx_queue_remove(q);

        အကယ်၍ (rn->waiting) {

            အကယ်၍ (++rn->last_connection == r->connections.nelts) {
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
    ပြန်ပေး ngx_queue_empty(&r->name_resend_queue)
           && ngx_queue_empty(&r->srv_resend_queue)
#အကယ်၍ (NGX_HAVE_INET6)
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

        အကယ်၍ (n == NGX_AGAIN) {
            bha_nga;
        }

        အကယ်၍ (n == NGX_ERROR) {
            goto failed;
        }

        ngx_resolver_process_response(rec->resolver, buf, n, 0);

    } နေစဉ် (rev->ready);

    အကယ်၍ (ngx_handle_read_event(rev, 0) != NGX_OK) {
        goto failed;
    }

    ပြန်ပေး;

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

    အကယ်၍ (wev->timedout) {
        goto failed;
    }

    sent = c->sent;

    နေစဉ် (wev->ready && b->pos < b->last) {
        n = ngx_send(c, b->pos, b->last - b->pos);

        အကယ်၍ (n == NGX_AGAIN) {
            bha_nga;
        }

        အကယ်၍ (n == NGX_ERROR) {
            goto failed;
        }

        b->pos += n;
    }

    အကယ်၍ (b->pos != b->start) {
        b->last = ngx_movemem(b->start, b->pos, b->last - b->pos);
        b->pos = b->start;
    }

    အကယ်၍ (c->sent != sent) {
        ngx_add_timer(wev, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    အကယ်၍ (ngx_handle_write_event(wev, 0) != NGX_OK) {
        goto failed;
    }

    ပြန်ပေး;

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

    နေစဉ် (rev->ready) {
        n = ngx_recv(c, b->last, b->end - b->last);

        အကယ်၍ (n == NGX_AGAIN) {
            bha_nga;
        }

        အကယ်၍ (n == NGX_ERROR || n == 0) {
            goto failed;
        }

        b->last += n;

        အတွက် ( ;; ) {
            p = b->pos;
            size = b->last - p;

            အကယ်၍ (size < 2) {
                bha_nga;
            }

            qlen = (u_short) *p++ << 8;
            qlen += *p++;

            အကယ်၍ (size < (size_t) (2 + qlen)) {
                bha_nga;
            }

            ngx_resolver_process_response(r, p, qlen, 1);

            b->pos += 2 + qlen;
        }

        အကယ်၍ (b->pos != b->start) {
            b->last = ngx_movemem(b->start, b->pos, b->last - b->pos);
            b->pos = b->start;
        }
    }

    အကယ်၍ (ngx_handle_read_event(rev, 0) != NGX_OK) {
        goto failed;
    }

    ပြန်ပေး;

failed:

    ngx_close_connection(c);
    rec->tcp = NULL;
}


static void
ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t tcp)
{
    var_na                 *err;
    ngx_uint_t            i, times, ident, qident, flags, code, nqs, nan, trunc,
                          qtype, qclass;
#အကယ်၍ (NGX_HAVE_INET6)
    ngx_uint_t            qident6;
#endif
    ngx_queue_t          *q;
    ngx_resolver_qs_t    *qs;
    ngx_resolver_hdr_t   *response;
    ngx_resolver_node_t  *rn;

    အကယ်၍ (n < parimaa_na(ngx_resolver_hdr_t)) {
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
    အကယ်၍ ((flags & 0xf870) != 0x8000 || (trunc && tcp)) {
        ngx_log_error(r->log_level, r->log, 0,
                      "invalid %s DNS response %ui fl:%04Xi",
                      tcp ? "TCP" : "UDP", ident, flags);
        ပြန်ပေး;
    }

    code = flags & 0xf;

    အကယ်၍ (code == NGX_RESOLVE_FORMERR) {

        times = 0;

        အတွက် (q = ngx_queue_head(&r->name_resend_queue);
             q != ngx_queue_sentinel(&r->name_resend_queue) && times++ < 100;
             q = ngx_queue_next(q))
        {
            rn = ngx_queue_data(q, ngx_resolver_node_t, queue);

            အကယ်၍ (rn->query) {
                qident = (rn->query[0] << 8) + rn->query[1];

                အကယ်၍ (qident == ident) {
                    goto dns_error_name;
                }
            }

#အကယ်၍ (NGX_HAVE_INET6)
            အကယ်၍ (rn->query6) {
                qident6 = (rn->query6[0] << 8) + rn->query6[1];

                အကယ်၍ (qident6 == ident) {
                    goto dns_error_name;
                }
            }
#endif
        }

        goto dns_error;
    }

    အကယ်၍ (code > NGX_RESOLVE_REFUSED) {
        goto dns_error;
    }

    အကယ်၍ (nqs != 1) {
        err = "invalid number of questions in DNS response";
        goto done;
    }

    i = parimaa_na(ngx_resolver_hdr_t);

    နေစဉ် (i < (ngx_uint_t) n) {

        အကယ်၍ (buf[i] & 0xc0) {
            err = "unexpected compression pointer in DNS response";
            goto done;
        }

        အကယ်၍ (buf[i] == '\0') {
            goto found;
        }

        i += 1 + buf[i];
    }

    goto short_response;

found:

    အကယ်၍ (i++ == parimaa_na(ngx_resolver_hdr_t)) {
        err = "zero-length domain name in DNS response";
        goto done;
    }

    အကယ်၍ (i + parimaa_na(ngx_resolver_qs_t) + nan * (2 + parimaa_na(ngx_resolver_an_t))
        > (ngx_uint_t) n)
    {
        goto short_response;
    }

    qs = (ngx_resolver_qs_t *) &buf[i];

    qtype = (qs->type_hi << 8) + qs->type_lo;
    qclass = (qs->class_hi << 8) + qs->class_lo;

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver DNS response qt:%ui cl:%ui", qtype, qclass);

    အကယ်၍ (qclass != 1) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unknown query class %ui in DNS response", qclass);
        ပြန်ပေး;
    }

    switch (qtype) {

    pak_sa NGX_RESOLVE_A:
#အကယ်၍ (NGX_HAVE_INET6)
    pak_sa NGX_RESOLVE_AAAA:
#endif

        ngx_resolver_process_a(r, buf, n, ident, code, qtype, nan, trunc,
                               i + parimaa_na(ngx_resolver_qs_t));

        bha_nga;

    pak_sa NGX_RESOLVE_SRV:

        ngx_resolver_process_srv(r, buf, n, ident, code, nan, trunc,
                                 i + parimaa_na(ngx_resolver_qs_t));

        bha_nga;

    pak_sa NGX_RESOLVE_PTR:

        ngx_resolver_process_ptr(r, buf, n, ident, code, nan);

        bha_nga;

    default:
        ngx_log_error(r->log_level, r->log, 0,
                      "unknown query type %ui in DNS response", qtype);
        ပြန်ပေး;
    }

    ပြန်ပေး;

short_response:

    err = "short DNS response";

done:

    ngx_log_error(r->log_level, r->log, 0, err);

    ပြန်ပေး;

dns_error_name:

    ngx_log_error(r->log_level, r->log, 0,
                  "DNS error (%ui: %s), query id:%ui, name:\"%*s\"",
                  code, ngx_resolver_strerror(code), ident,
                  (size_t) rn->nlen, rn->name);
    ပြန်ပေး;

dns_error:

    ngx_log_error(r->log_level, r->log, 0,
                  "DNS error (%ui: %s), query id:%ui",
                  code, ngx_resolver_strerror(code), ident);
    ပြန်ပေး;
}


static void
ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans)
{
    var_na                       *err;
    u_char                     *cname;
    size_t                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    in_addr_t                  *addr;
    ngx_str_t                   name;
    ngx_uint_t                  type, class, qident, naddrs, a, i, j, start;
#အကယ်၍ (NGX_HAVE_INET6)
    sa_nracanaa in6_addr            *addr6;
#endif
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *next;
    ngx_resolver_node_t        *rn;
    ngx_resolver_addr_t        *addrs;
    ngx_resolver_connection_t  *rec;

    အကယ်၍ (ngx_resolver_copy(r, &name, buf,
                          buf + parimaa_na(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        ပြန်ပေး;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver qs:%V", &name);

    hash = ngx_crc32_short(name.data, name.len);

    /* lock name mutex */

    rn = ngx_resolver_lookup_name(r, &name, hash);

    အကယ်၍ (rn == NULL) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.data);
        goto failed;
    }

    switch (qtype) {

#အကယ်၍ (NGX_HAVE_INET6)
    pak_sa NGX_RESOLVE_AAAA:

        အကယ်၍ (rn->query6 == NULL || rn->naddrs6 != (u_short) -1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.data);
            goto failed;
        }

        အကယ်၍ (trunc && rn->tcp6) {
            ngx_resolver_free(r, name.data);
            goto failed;
        }

        qident = (rn->query6[0] << 8) + rn->query6[1];

        bha_nga;
#endif

    default: /* NGX_RESOLVE_A */

        အကယ်၍ (rn->query == NULL || rn->naddrs != (u_short) -1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.data);
            goto failed;
        }

        အကယ်၍ (trunc && rn->tcp) {
            ngx_resolver_free(r, name.data);
            goto failed;
        }

        qident = (rn->query[0] << 8) + rn->query[1];
    }

    အကယ်၍ (ident != qident) {
        ngx_log_error(r->log_level, r->log, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.data);
        goto failed;
    }

    ngx_resolver_free(r, name.data);

    အကယ်၍ (trunc) {

        ngx_queue_remove(&rn->queue);

        အကယ်၍ (rn->waiting == NULL) {
            ngx_rbtree_delete(&r->name_rbtree, &rn->node);
            ngx_resolver_free_node(r, rn);
            goto next;
        }

        rec = r->connections.elts;
        rec = &rec[rn->last_connection];

        switch (qtype) {

#အကယ်၍ (NGX_HAVE_INET6)
        pak_sa NGX_RESOLVE_AAAA:

            rn->tcp6 = 1;

            (void) ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen);

            bha_nga;
#endif

        default: /* NGX_RESOLVE_A */

            rn->tcp = 1;

            (void) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);
        }

        rn->expire = ngx_time() + r->resend_timeout;

        ngx_queue_insert_head(&r->name_resend_queue, &rn->queue);

        goto next;
    }

    အကယ်၍ (code == 0 && rn->code) {
        code = rn->code;
    }

    အကယ်၍ (code == 0 && nan == 0) {

#အကယ်၍ (NGX_HAVE_INET6)
        switch (qtype) {

        pak_sa NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            အကယ်၍ (rn->naddrs == (u_short) -1) {
                goto next;
            }

            အကယ်၍ (rn->naddrs) {
                goto export;
            }

            bha_nga;

        default: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            အကယ်၍ (rn->naddrs6 == (u_short) -1) {
                goto next;
            }

            အကယ်၍ (rn->naddrs6) {
                goto export;
            }
        }
#endif

        code = NGX_RESOLVE_NXDOMAIN;
    }

    အကယ်၍ (code) {

#အကယ်၍ (NGX_HAVE_INET6)
        switch (qtype) {

        pak_sa NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            အကယ်၍ (rn->naddrs == (u_short) -1) {
                rn->code = (u_char) code;
                goto next;
            }

            bha_nga;

        default: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            အကယ်၍ (rn->naddrs6 == (u_short) -1) {
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

        နေစဉ် (next) {
            ctx = next;
            ctx->state = code;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            next = ctx->next;

            ctx->handler(ctx);
        }

        ngx_resolver_free_node(r, rn);

        ပြန်ပေး;
    }

    i = ans;
    naddrs = 0;
    cname = NULL;

    အတွက် (a = 0; a < nan; a++) {

        start = i;

        နေစဉ် (i < n) {

            အကယ်၍ (buf[i] & 0xc0) {
                i += 2;
                goto found;
            }

            အကယ်၍ (buf[i] == 0) {
                i++;
                goto test_length;
            }

            i += 1 + buf[i];
        }

        goto short_response;

    test_length:

        အကယ်၍ (i - start < 2) {
            err = "invalid name in DNS response";
            goto invalid;
        }

    found:

        အကယ်၍ (i + parimaa_na(ngx_resolver_an_t) >= n) {
            goto short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        အကယ်၍ (class != 1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR class %ui in DNS response", class);
            goto failed;
        }

        အကယ်၍ (ttl < 0) {
            ttl = 0;
        }

        rn->ttl = ngx_min(rn->ttl, (uint32_t) ttl);

        i += parimaa_na(ngx_resolver_an_t);

        switch (type) {

        pak_sa NGX_RESOLVE_A:

            အကယ်၍ (qtype != NGX_RESOLVE_A) {
                err = "unexpected A record in DNS response";
                goto invalid;
            }

            အကယ်၍ (len != 4) {
                err = "invalid A record in DNS response";
                goto invalid;
            }

            အကယ်၍ (i + 4 > n) {
                goto short_response;
            }

            naddrs++;

            bha_nga;

#အကယ်၍ (NGX_HAVE_INET6)
        pak_sa NGX_RESOLVE_AAAA:

            အကယ်၍ (qtype != NGX_RESOLVE_AAAA) {
                err = "unexpected AAAA record in DNS response";
                goto invalid;
            }

            အကယ်၍ (len != 16) {
                err = "invalid AAAA record in DNS response";
                goto invalid;
            }

            အကယ်၍ (i + 16 > n) {
                goto short_response;
            }

            naddrs++;

            bha_nga;
#endif

        pak_sa NGX_RESOLVE_CNAME:

            cname = &buf[i];

            bha_nga;

        pak_sa NGX_RESOLVE_DNAME:

            bha_nga;

        default:

            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver naddrs:%ui cname:%p ttl:%uD",
                   naddrs, cname, rn->ttl);

    အကယ်၍ (naddrs) {

        switch (qtype) {

#အကယ်၍ (NGX_HAVE_INET6)
        pak_sa NGX_RESOLVE_AAAA:

            အကယ်၍ (naddrs == 1) {
                addr6 = &rn->u6.addr6;
                rn->naddrs6 = 1;

            } မဟုတ်ပါက {
                addr6 = ngx_resolver_alloc(r, naddrs * parimaa_na(sa_nracanaa in6_addr));
                အကယ်၍ (addr6 == NULL) {
                    goto failed;
                }

                rn->u6.addrs6 = addr6;
                rn->naddrs6 = (u_short) naddrs;
            }

#အကယ်၍ (NGX_SUPPRESS_WARN)
            addr = NULL;
#endif

            bha_nga;
#endif

        default: /* NGX_RESOLVE_A */

            အကယ်၍ (naddrs == 1) {
                addr = &rn->u.addr;
                rn->naddrs = 1;

            } မဟုတ်ပါက {
                addr = ngx_resolver_alloc(r, naddrs * parimaa_na(in_addr_t));
                အကယ်၍ (addr == NULL) {
                    goto failed;
                }

                rn->u.addrs = addr;
                rn->naddrs = (u_short) naddrs;
            }

#အကယ်၍ (NGX_HAVE_INET6 && NGX_SUPPRESS_WARN)
            addr6 = NULL;
#endif
        }

        j = 0;
        i = ans;

        အတွက် (a = 0; a < nan; a++) {

            အတွက် ( ;; ) {

                အကယ်၍ (buf[i] & 0xc0) {
                    i += 2;
                    bha_nga;
                }

                အကယ်၍ (buf[i] == 0) {
                    i++;
                    bha_nga;
                }

                i += 1 + buf[i];
            }

            an = (ngx_resolver_an_t *) &buf[i];

            type = (an->type_hi << 8) + an->type_lo;
            len = (an->len_hi << 8) + an->len_lo;

            i += parimaa_na(ngx_resolver_an_t);

            အကယ်၍ (type == NGX_RESOLVE_A) {

                addr[j] = htonl((buf[i] << 24) + (buf[i + 1] << 16)
                                + (buf[i + 2] << 8) + (buf[i + 3]));

                အကယ်၍ (++j == naddrs) {

#အကယ်၍ (NGX_HAVE_INET6)
                    အကယ်၍ (rn->naddrs6 == (u_short) -1) {
                        goto next;
                    }
#endif

                    bha_nga;
                }
            }

#အကယ်၍ (NGX_HAVE_INET6)
            မဟုတ်ပါက အကယ်၍ (type == NGX_RESOLVE_AAAA) {

                ngx_memcpy(addr6[j].s6_addr, &buf[i], 16);

                အကယ်၍ (++j == naddrs) {

                    အကယ်၍ (rn->naddrs == (u_short) -1) {
                        goto next;
                    }

                    bha_nga;
                }
            }
#endif

            i += len;
        }
    }

    switch (qtype) {

#အကယ်၍ (NGX_HAVE_INET6)
    pak_sa NGX_RESOLVE_AAAA:

        အကယ်၍ (rn->naddrs6 == (u_short) -1) {
            rn->naddrs6 = 0;
        }

        bha_nga;
#endif

    default: /* NGX_RESOLVE_A */

        အကယ်၍ (rn->naddrs == (u_short) -1) {
            rn->naddrs = 0;
        }
    }

    အကယ်၍ (rn->naddrs != (u_short) -1
#အကယ်၍ (NGX_HAVE_INET6)
        && rn->naddrs6 != (u_short) -1
#endif
        && rn->naddrs
#အကယ်၍ (NGX_HAVE_INET6)
           + rn->naddrs6
#endif
           > 0)
    {

#အကယ်၍ (NGX_HAVE_INET6)
    export:
#endif

        naddrs = rn->naddrs;
#အကယ်၍ (NGX_HAVE_INET6)
        naddrs += rn->naddrs6;
#endif

        အကယ်၍ (naddrs == 1 && rn->naddrs == 1) {
            addrs = NULL;

        } မဟုတ်ပါက {
            addrs = ngx_resolver_export(r, rn, 0);
            အကယ်၍ (addrs == NULL) {
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

        နေစဉ် (next) {
            ctx = next;
            ctx->state = NGX_OK;
            ctx->valid = rn->valid;
            ctx->naddrs = naddrs;

            အကယ်၍ (addrs == NULL) {
                ctx->addrs = &ctx->addr;
                ctx->addr.sockaddr = (sa_nracanaa sockaddr *) &ctx->sin;
                ctx->addr.socklen = parimaa_na(sa_nracanaa sockaddr_in);
                ngx_memzero(&ctx->sin, parimaa_na(sa_nracanaa sockaddr_in));
                ctx->sin.sin_family = AF_INET;
                ctx->sin.sin_addr.s_addr = rn->u.addr;

            } မဟုတ်ပါက {
                ctx->addrs = addrs;
            }

            next = ctx->next;

            ctx->handler(ctx);
        }

        အကယ်၍ (addrs != NULL) {
            ngx_resolver_free(r, addrs->sockaddr);
            ngx_resolver_free(r, addrs);
        }

        ngx_resolver_free(r, rn->query);
        rn->query = NULL;
#အကယ်၍ (NGX_HAVE_INET6)
        rn->query6 = NULL;
#endif

        ပြန်ပေး;
    }

    အကယ်၍ (cname) {

        /* CNAME only */

        အကယ်၍ (rn->naddrs == (u_short) -1
#အကယ်၍ (NGX_HAVE_INET6)
            || rn->naddrs6 == (u_short) -1
#endif
            )
        {
            goto next;
        }

        အကယ်၍ (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
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
#အကယ်၍ (NGX_HAVE_INET6)
        rn->query6 = NULL;
#endif

        ctx = rn->waiting;
        rn->waiting = NULL;

        အကယ်၍ (ctx) {

            အကယ်၍ (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                do {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    next = ctx->next;

                    ctx->handler(ctx);

                    ctx = next;
                } နေစဉ် (ctx);

                ပြန်ပေး;
            }

            အတွက် (next = ctx; next; next = next->next) {
                next->node = NULL;
            }

            (void) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        ပြန်ပေး;
    }

    ngx_log_error(r->log_level, r->log, 0,
                  "no A or CNAME types in DNS response");
    ပြန်ပေး;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->log, 0, err);

    ပြန်ပေး;

failed:

next:

    /* unlock name mutex */

    ပြန်ပေး;
}


static void
ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans)
{
    var_na                       *err;
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

    အကယ်၍ (ngx_resolver_copy(r, &name, buf,
                          buf + parimaa_na(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        ပြန်ပေး;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver qs:%V", &name);

    hash = ngx_crc32_short(name.data, name.len);

    rn = ngx_resolver_lookup_srv(r, &name, hash);

    အကယ်၍ (rn == NULL || rn->query == NULL) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.data);
        goto failed;
    }

    အကယ်၍ (trunc && rn->tcp) {
        ngx_resolver_free(r, name.data);
        goto failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    အကယ်၍ (ident != qident) {
        ngx_log_error(r->log_level, r->log, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.data);
        goto failed;
    }

    ngx_resolver_free(r, name.data);

    အကယ်၍ (trunc) {

        ngx_queue_remove(&rn->queue);

        အကယ်၍ (rn->waiting == NULL) {
            ngx_rbtree_delete(&r->srv_rbtree, &rn->node);
            ngx_resolver_free_node(r, rn);
            ပြန်ပေး;
        }

        rec = r->connections.elts;
        rec = &rec[rn->last_connection];

        rn->tcp = 1;

        (void) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);

        rn->expire = ngx_time() + r->resend_timeout;

        ngx_queue_insert_head(&r->srv_resend_queue, &rn->queue);

        ပြန်ပေး;
    }

    အကယ်၍ (code == 0 && rn->code) {
        code = rn->code;
    }

    အကယ်၍ (code == 0 && nan == 0) {
        code = NGX_RESOLVE_NXDOMAIN;
    }

    အကယ်၍ (code) {
        next = rn->waiting;
        rn->waiting = NULL;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(&r->srv_rbtree, &rn->node);

        နေစဉ် (next) {
            ctx = next;
            ctx->state = code;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            next = ctx->next;

            ctx->handler(ctx);
        }

        ngx_resolver_free_node(r, rn);

        ပြန်ပေး;
    }

    i = ans;
    nsrvs = 0;
    cname = NULL;

    အတွက် (a = 0; a < nan; a++) {

        start = i;

        နေစဉ် (i < n) {

            အကယ်၍ (buf[i] & 0xc0) {
                i += 2;
                goto found;
            }

            အကယ်၍ (buf[i] == 0) {
                i++;
                goto test_length;
            }

            i += 1 + buf[i];
        }

        goto short_response;

    test_length:

        အကယ်၍ (i - start < 2) {
            err = "invalid name DNS response";
            goto invalid;
        }

    found:

        အကယ်၍ (i + parimaa_na(ngx_resolver_an_t) >= n) {
            goto short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        အကယ်၍ (class != 1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR class %ui in DNS response", class);
            goto failed;
        }

        အကယ်၍ (ttl < 0) {
            ttl = 0;
        }

        rn->ttl = ngx_min(rn->ttl, (uint32_t) ttl);

        i += parimaa_na(ngx_resolver_an_t);

        switch (type) {

        pak_sa NGX_RESOLVE_SRV:

            အကယ်၍ (i + 6 >= n) {
                goto short_response;
            }

            အကယ်၍ (ngx_resolver_copy(r, NULL, buf, &buf[i + 6], buf + n)
                != NGX_OK)
            {
                goto failed;
            }

            nsrvs++;

            bha_nga;

        pak_sa NGX_RESOLVE_CNAME:

            cname = &buf[i];

            bha_nga;

        pak_sa NGX_RESOLVE_DNAME:

            bha_nga;

        default:

            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver nsrvs:%ui cname:%p ttl:%uD",
                   nsrvs, cname, rn->ttl);

    အကယ်၍ (nsrvs) {

        srvs = ngx_resolver_calloc(r, nsrvs * parimaa_na(ngx_resolver_srv_t));
        အကယ်၍ (srvs == NULL) {
            goto failed;
        }

        rn->u.srvs = srvs;
        rn->nsrvs = (u_short) nsrvs;

        j = 0;
        i = ans;

        အတွက် (a = 0; a < nan; a++) {

            အတွက် ( ;; ) {

                အကယ်၍ (buf[i] & 0xc0) {
                    i += 2;
                    bha_nga;
                }

                အကယ်၍ (buf[i] == 0) {
                    i++;
                    bha_nga;
                }

                i += 1 + buf[i];
            }

            an = (ngx_resolver_an_t *) &buf[i];

            type = (an->type_hi << 8) + an->type_lo;
            len = (an->len_hi << 8) + an->len_lo;

            i += parimaa_na(ngx_resolver_an_t);

            အကယ်၍ (type == NGX_RESOLVE_SRV) {

                srvs[j].priority = (buf[i] << 8) + buf[i + 1];
                srvs[j].weight = (buf[i + 2] << 8) + buf[i + 3];

                အကယ်၍ (srvs[j].weight == 0) {
                    srvs[j].weight = 1;
                }

                srvs[j].port = (buf[i + 4] << 8) + buf[i + 5];

                အကယ်၍ (ngx_resolver_copy(r, &srvs[j].name, buf, &buf[i + 6],
                                      buf + n)
                    != NGX_OK)
                {
                    goto failed;
                }

                j++;
            }

            i += len;
        }

        ngx_sort(srvs, nsrvs, parimaa_na(ngx_resolver_srv_t),
                 ngx_resolver_cmp_srvs);

        ngx_resolver_free(r, rn->query);
        rn->query = NULL;

        ngx_queue_remove(&rn->queue);

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_insert_head(&r->srv_expire_queue, &rn->queue);

        next = rn->waiting;
        rn->waiting = NULL;

        နေစဉ် (next) {
            ctx = next;
            next = ctx->next;

            ngx_resolver_resolve_srv_names(ctx, rn);
        }

        ပြန်ပေး;
    }

    rn->nsrvs = 0;

    အကယ်၍ (cname) {

        /* CNAME only */

        အကယ်၍ (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
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
#အကယ်၍ (NGX_HAVE_INET6)
        rn->query6 = NULL;
#endif

        ctx = rn->waiting;
        rn->waiting = NULL;

        အကယ်၍ (ctx) {

            အကယ်၍ (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                do {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    next = ctx->next;

                    ctx->handler(ctx);

                    ctx = next;
                } နေစဉ် (ctx);

                ပြန်ပေး;
            }

            အတွက် (next = ctx; next; next = next->next) {
                next->node = NULL;
            }

            (void) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        ပြန်ပေး;
    }

    ngx_log_error(r->log_level, r->log, 0, "no SRV type in DNS response");

    ပြန်ပေး;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->log, 0, err);

    ပြန်ပေး;

failed:

    /* unlock name mutex */

    ပြန်ပေး;
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

    srvs = ngx_resolver_calloc(r, rn->nsrvs * parimaa_na(ngx_resolver_srv_name_t));
    အကယ်၍ (srvs == NULL) {
        goto failed;
    }

    ctx->srvs = srvs;
    ctx->nsrvs = rn->nsrvs;

    အကယ်၍ (ctx->event && ctx->event->timer_set) {
        ngx_del_timer(ctx->event);
    }

    အတွက် (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
        srvs[i].name.data = ngx_resolver_alloc(r, rn->u.srvs[i].name.len);
        အကယ်၍ (srvs[i].name.data == NULL) {
            goto failed;
        }

        srvs[i].name.len = rn->u.srvs[i].name.len;
        ngx_memcpy(srvs[i].name.data, rn->u.srvs[i].name.data,
                   srvs[i].name.len);

        cctx = ngx_resolve_start(r, NULL);
        အကယ်၍ (cctx == NULL) {
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

        အကယ်၍ (ngx_resolve_name(cctx) == NGX_ERROR) {
            srvs[i].ctx = NULL;
            goto failed;
        }
    }

    ပြန်ပေး;

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

    အကယ်၍ (cctx->naddrs) {

        ctx->valid = ngx_min(ctx->valid, cctx->valid);

        addrs = ngx_resolver_calloc(r, cctx->naddrs * parimaa_na(ngx_addr_t));
        အကယ်၍ (addrs == NULL) {
            srv->state = NGX_ERROR;
            goto done;
        }

        sockaddr = ngx_resolver_alloc(r, cctx->naddrs * parimaa_na(ngx_sockaddr_t));
        အကယ်၍ (sockaddr == NULL) {
            ngx_resolver_free(r, addrs);
            srv->state = NGX_ERROR;
            goto done;
        }

        အတွက် (i = 0; i < cctx->naddrs; i++) {
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

    အကယ်၍ (ctx->count == 0) {
        ngx_resolver_report_srv(r, ctx);
    }
}


static void
ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan)
{
    var_na                 *err;
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
#အကယ်၍ (NGX_HAVE_INET6)
    uint32_t              hash;
    ngx_int_t             digit;
    sa_nracanaa in6_addr       addr6;
#endif

    အကယ်၍ (ngx_resolver_copy(r, &name, buf,
                          buf + parimaa_na(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        ပြန်ပေး;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver qs:%V", &name);

    /* AF_INET */

    addr = 0;
    i = parimaa_na(ngx_resolver_hdr_t);

    အတွက် (mask = 0; mask < 32; mask += 8) {
        len = buf[i++];

        octet = ngx_atoi(&buf[i], len);
        အကယ်၍ (octet == NGX_ERROR || octet > 255) {
            goto invalid_in_addr_arpa;
        }

        addr += octet << mask;
        i += len;
    }

    အကယ်၍ (ngx_strcasecmp(&buf[i], (u_char *) "\7in-addr\4arpa") == 0) {
        i += parimaa_na("\7in-addr\4arpa");

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        tree = &r->addr_rbtree;
        expire_queue = &r->addr_expire_queue;

        goto valid;
    }

invalid_in_addr_arpa:

#အကယ်၍ (NGX_HAVE_INET6)

    i = parimaa_na(ngx_resolver_hdr_t);

    အတွက် (octet = 15; octet >= 0; octet--) {
        အကယ်၍ (buf[i++] != '\1') {
            goto invalid_ip6_arpa;
        }

        digit = ngx_hextoi(&buf[i++], 1);
        အကယ်၍ (digit == NGX_ERROR) {
            goto invalid_ip6_arpa;
        }

        addr6.s6_addr[octet] = (u_char) digit;

        အကယ်၍ (buf[i++] != '\1') {
            goto invalid_ip6_arpa;
        }

        digit = ngx_hextoi(&buf[i++], 1);
        အကယ်၍ (digit == NGX_ERROR) {
            goto invalid_ip6_arpa;
        }

        addr6.s6_addr[octet] += (u_char) (digit * 16);
    }

    အကယ်၍ (ngx_strcasecmp(&buf[i], (u_char *) "\3ip6\4arpa") == 0) {
        i += parimaa_na("\3ip6\4arpa");

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
    ပြန်ပေး;

valid:

    အကယ်၍ (rn == NULL || rn->query == NULL) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.data);
        goto failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    အကယ်၍ (ident != qident) {
        ngx_log_error(r->log_level, r->log, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.data);
        goto failed;
    }

    ngx_resolver_free(r, name.data);

    အကယ်၍ (code == 0 && nan == 0) {
        code = NGX_RESOLVE_NXDOMAIN;
    }

    အကယ်၍ (code) {
        next = rn->waiting;
        rn->waiting = NULL;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(tree, &rn->node);

        /* unlock addr mutex */

        နေစဉ် (next) {
            ctx = next;
            ctx->state = code;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            next = ctx->next;

            ctx->handler(ctx);
        }

        ngx_resolver_free_node(r, rn);

        ပြန်ပေး;
    }

    i += parimaa_na(ngx_resolver_qs_t);

    အတွက် (a = 0; a < nan; a++) {

        start = i;

        နေစဉ် (i < n) {

            အကယ်၍ (buf[i] & 0xc0) {
                i += 2;
                goto found;
            }

            အကယ်၍ (buf[i] == 0) {
                i++;
                goto test_length;
            }

            i += 1 + buf[i];
        }

        goto short_response;

    test_length:

        အကယ်၍ (i - start < 2) {
            err = "invalid name in DNS response";
            goto invalid;
        }

    found:

        အကယ်၍ (i + parimaa_na(ngx_resolver_an_t) >= n) {
            goto short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        အကယ်၍ (class != 1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR class %ui in DNS response", class);
            goto failed;
        }

        အကယ်၍ (ttl < 0) {
            ttl = 0;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                      "resolver qt:%ui cl:%ui len:%uz",
                      type, class, len);

        i += parimaa_na(ngx_resolver_an_t);

        switch (type) {

        pak_sa NGX_RESOLVE_PTR:

            goto ptr;

        pak_sa NGX_RESOLVE_CNAME:

            bha_nga;

        default:

            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->log, 0,
                  "no PTR type in DNS response");
    ပြန်ပေး;

ptr:

    အကယ်၍ (ngx_resolver_copy(r, &name, buf, buf + i, buf + n) != NGX_OK) {
        goto failed;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver an:%V", &name);

    အကယ်၍ (name.len != (size_t) rn->nlen
        || ngx_strncmp(name.data, rn->name, name.len) != 0)
    {
        အကယ်၍ (rn->nlen) {
            ngx_resolver_free(r, rn->name);
        }

        rn->nlen = (u_short) name.len;
        rn->name = name.data;

        name.data = ngx_resolver_dup(r, rn->name, name.len);
        အကယ်၍ (name.data == NULL) {
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

    နေစဉ် (next) {
        ctx = next;
        ctx->state = NGX_OK;
        ctx->valid = rn->valid;
        ctx->name = name;
        next = ctx->next;

        ctx->handler(ctx);
    }

    ngx_resolver_free(r, name.data);

    ပြန်ပေး;

short_response:

    err = "short DNS response";

invalid:

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->log, 0, err);

    ပြန်ပေး;

failed:

    /* unlock addr mutex */

    ပြန်ပေး;
}


static ngx_resolver_node_t *
ngx_resolver_lookup_name(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_node_t    *node, *sentinel;
    ngx_resolver_node_t  *rn;

    node = r->name_rbtree.root;
    sentinel = r->name_rbtree.sentinel;

    နေစဉ် (node != sentinel) {

        အကယ်၍ (hash < node->key) {
            node = node->left;
            continue;
        }

        အကယ်၍ (hash > node->key) {
            node = node->right;
            continue;
        }

        /* hash == node->key */

        rn = ngx_resolver_node(node);

        rc = ngx_memn2cmp(name->data, rn->name, name->len, rn->nlen);

        အကယ်၍ (rc == 0) {
            ပြန်ပေး rn;
        }

        node = (rc < 0) ? node->left : node->right;
    }

    /* not found */

    ပြန်ပေး NULL;
}


static ngx_resolver_node_t *
ngx_resolver_lookup_srv(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_node_t    *node, *sentinel;
    ngx_resolver_node_t  *rn;

    node = r->srv_rbtree.root;
    sentinel = r->srv_rbtree.sentinel;

    နေစဉ် (node != sentinel) {

        အကယ်၍ (hash < node->key) {
            node = node->left;
            continue;
        }

        အကယ်၍ (hash > node->key) {
            node = node->right;
            continue;
        }

        /* hash == node->key */

        rn = ngx_resolver_node(node);

        rc = ngx_memn2cmp(name->data, rn->name, name->len, rn->nlen);

        အကယ်၍ (rc == 0) {
            ပြန်ပေး rn;
        }

        node = (rc < 0) ? node->left : node->right;
    }

    /* not found */

    ပြန်ပေး NULL;
}


static ngx_resolver_node_t *
ngx_resolver_lookup_addr(ngx_resolver_t *r, in_addr_t addr)
{
    ngx_rbtree_node_t  *node, *sentinel;

    node = r->addr_rbtree.root;
    sentinel = r->addr_rbtree.sentinel;

    နေစဉ် (node != sentinel) {

        အကယ်၍ (addr < node->key) {
            node = node->left;
            continue;
        }

        အကယ်၍ (addr > node->key) {
            node = node->right;
            continue;
        }

        /* addr == node->key */

        ပြန်ပေး ngx_resolver_node(node);
    }

    /* not found */

    ပြန်ပေး NULL;
}


#အကယ်၍ (NGX_HAVE_INET6)

static ngx_resolver_node_t *
ngx_resolver_lookup_addr6(ngx_resolver_t *r, sa_nracanaa in6_addr *addr,
    uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_node_t    *node, *sentinel;
    ngx_resolver_node_t  *rn;

    node = r->addr6_rbtree.root;
    sentinel = r->addr6_rbtree.sentinel;

    နေစဉ် (node != sentinel) {

        အကယ်၍ (hash < node->key) {
            node = node->left;
            continue;
        }

        အကယ်၍ (hash > node->key) {
            node = node->right;
            continue;
        }

        /* hash == node->key */

        rn = ngx_resolver_node(node);

        rc = ngx_memcmp(addr, &rn->addr6, 16);

        အကယ်၍ (rc == 0) {
            ပြန်ပေး rn;
        }

        node = (rc < 0) ? node->left : node->right;
    }

    /* not found */

    ပြန်ပေး NULL;
}

#endif


static void
ngx_resolver_rbtree_insert_value(ngx_rbtree_node_t *temp,
    ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel)
{
    ngx_rbtree_node_t    **p;
    ngx_resolver_node_t   *rn, *rn_temp;

    အတွက် ( ;; ) {

        အကယ်၍ (node->key < temp->key) {

            p = &temp->left;

        } မဟုတ်ပါက အကယ်၍ (node->key > temp->key) {

            p = &temp->right;

        } မဟုတ်ပါက { /* node->key == temp->key */

            rn = ngx_resolver_node(node);
            rn_temp = ngx_resolver_node(temp);

            p = (ngx_memn2cmp(rn->name, rn_temp->name, rn->nlen, rn_temp->nlen)
                 < 0) ? &temp->left : &temp->right;
        }

        အကယ်၍ (*p == sentinel) {
            bha_nga;
        }

        temp = *p;
    }

    *p = node;
    node->parent = temp;
    node->left = sentinel;
    node->right = sentinel;
    ngx_rbt_red(node);
}


#အကယ်၍ (NGX_HAVE_INET6)

static void
ngx_resolver_rbtree_insert_addr6_value(ngx_rbtree_node_t *temp,
    ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel)
{
    ngx_rbtree_node_t    **p;
    ngx_resolver_node_t   *rn, *rn_temp;

    အတွက် ( ;; ) {

        အကယ်၍ (node->key < temp->key) {

            p = &temp->left;

        } မဟုတ်ပါက အကယ်၍ (node->key > temp->key) {

            p = &temp->right;

        } မဟုတ်ပါက { /* node->key == temp->key */

            rn = ngx_resolver_node(node);
            rn_temp = ngx_resolver_node(temp);

            p = (ngx_memcmp(&rn->addr6, &rn_temp->addr6, 16)
                 < 0) ? &temp->left : &temp->right;
        }

        အကယ်၍ (*p == sentinel) {
            bha_nga;
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

    len = parimaa_na(ngx_resolver_hdr_t) + nlen + parimaa_na(ngx_resolver_qs_t);

#အကယ်၍ (NGX_HAVE_INET6)
    p = ngx_resolver_alloc(r, len * (r->ipv4 + r->ipv6));
#မဟုတ်ပါက
    p = ngx_resolver_alloc(r, len);
#endif
    အကယ်၍ (p == NULL) {
        ပြန်ပေး NGX_ERROR;
    }

    rn->qlen = (u_short) len;
    rn->query = p;

#အကယ်၍ (NGX_HAVE_INET6)
    အကယ်၍ (r->ipv6) {
        rn->query6 = r->ipv4 ? (p + len) : p;
    }
#endif

    query = (ngx_resolver_hdr_t *) p;

    အကယ်၍ (r->ipv4) {
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

    p += parimaa_na(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    /* query type */
    qs->type_hi = 0; qs->type_lo = NGX_RESOLVE_A;

    /* IN query class */
    qs->class_hi = 0; qs->class_lo = 1;

    /* convert "www.example.com" to "\3www\7example\3com\0" */

    len = 0;
    p--;
    *p-- = '\0';

    အကယ်၍ (name->len == 0)  {
        ပြန်ပေး NGX_DECLINED;
    }

    အတွက် (s = name->data + name->len - 1; s >= name->data; s--) {
        အကယ်၍ (*s != '.') {
            *p = *s;
            len++;

        } မဟုတ်ပါက {
            အကယ်၍ (len == 0 || len > 255) {
                ပြန်ပေး NGX_DECLINED;
            }

            *p = (u_char) len;
            len = 0;
        }

        p--;
    }

    အကယ်၍ (len == 0 || len > 255) {
        ပြန်ပေး NGX_DECLINED;
    }

    *p = (u_char) len;

#အကယ်၍ (NGX_HAVE_INET6)
    အကယ်၍ (!r->ipv6) {
        ပြန်ပေး NGX_OK;
    }

    p = rn->query6;

    အကယ်၍ (r->ipv4) {
        ngx_memcpy(p, rn->query, rn->qlen);
    }

    query = (ngx_resolver_hdr_t *) p;

    ident = ngx_random();

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve: \"%V\" AAAA %i", name, ident & 0xffff);

    query->ident_hi = (u_char) ((ident >> 8) & 0xff);
    query->ident_lo = (u_char) (ident & 0xff);

    p += parimaa_na(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    qs->type_lo = NGX_RESOLVE_AAAA;
#endif

    ပြန်ပေး NGX_OK;
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

    len = parimaa_na(ngx_resolver_hdr_t) + nlen + parimaa_na(ngx_resolver_qs_t);

    p = ngx_resolver_alloc(r, len);
    အကယ်၍ (p == NULL) {
        ပြန်ပေး NGX_ERROR;
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

    p += parimaa_na(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    /* query type */
    qs->type_hi = 0; qs->type_lo = NGX_RESOLVE_SRV;

    /* IN query class */
    qs->class_hi = 0; qs->class_lo = 1;

    /* converts "www.example.com" to "\3www\7example\3com\0" */

    len = 0;
    p--;
    *p-- = '\0';

    အကယ်၍ (name->len == 0)  {
        ပြန်ပေး NGX_DECLINED;
    }

    အတွက် (s = name->data + name->len - 1; s >= name->data; s--) {
        အကယ်၍ (*s != '.') {
            *p = *s;
            len++;

        } မဟုတ်ပါက {
            အကယ်၍ (len == 0 || len > 255) {
                ပြန်ပေး NGX_DECLINED;
            }

            *p = (u_char) len;
            len = 0;
        }

        p--;
    }

    အကယ်၍ (len == 0 || len > 255) {
        ပြန်ပေး NGX_DECLINED;
    }

    *p = (u_char) len;

    ပြန်ပေး NGX_OK;
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
    sa_nracanaa sockaddr_in   *sin;
#အကယ်၍ (NGX_HAVE_INET6)
    sa_nracanaa sockaddr_in6  *sin6;
#endif

    switch (addr->sockaddr->sa_family) {

#အကယ်၍ (NGX_HAVE_INET6)
    pak_sa AF_INET6:
        len = parimaa_na(ngx_resolver_hdr_t)
              + 64 + parimaa_na(".ip6.arpa.") - 1
              + parimaa_na(ngx_resolver_qs_t);

        bha_nga;
#endif

    default: /* AF_INET */
        len = parimaa_na(ngx_resolver_hdr_t)
              + parimaa_na(".255.255.255.255.in-addr.arpa.") - 1
              + parimaa_na(ngx_resolver_qs_t);
    }

    p = ngx_resolver_alloc(r, len);
    အကယ်၍ (p == NULL) {
        ပြန်ပေး NGX_ERROR;
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

    p += parimaa_na(ngx_resolver_hdr_t);

    switch (addr->sockaddr->sa_family) {

#အကယ်၍ (NGX_HAVE_INET6)
    pak_sa AF_INET6:
        sin6 = (sa_nracanaa sockaddr_in6 *) addr->sockaddr;

        အတွက် (n = 15; n >= 0; n--) {
            p = ngx_sprintf(p, "\1%xd\1%xd",
                            sin6->sin6_addr.s6_addr[n] & 0xf,
                            (sin6->sin6_addr.s6_addr[n] >> 4) & 0xf);
        }

        p = ngx_cpymem(p, "\3ip6\4arpa\0", 10);

        bha_nga;
#endif

    default: /* AF_INET */

        sin = (sa_nracanaa sockaddr_in *) addr->sockaddr;
        inaddr = ntohl(sin->sin_addr.s_addr);

        အတွက် (n = 0; n < 32; n += 8) {
            d = ngx_sprintf(&p[1], "%ud", (inaddr >> n) & 0xff);
            *p = (u_char) (d - &p[1]);
            p = d;
        }

        p = ngx_cpymem(p, "\7in-addr\4arpa\0", 14);
    }

    /* query type "PTR", IN query class */
    p = ngx_cpymem(p, "\0\14\0\1", 4);

    rn->qlen = (u_short) (p - rn->query);

    ပြန်ပေး NGX_OK;
}


static ngx_int_t
ngx_resolver_copy(ngx_resolver_t *r, ngx_str_t *name, u_char *buf, u_char *src,
    u_char *last)
{
    var_na        *err;
    u_char      *p, *dst;
    size_t       len;
    ngx_uint_t   i, n;

    p = src;
    len = 0;

    /*
     * compression pointers allow to create endless loop, so we set limit;
     * 128 pointers should be enough to store 255-byte name
     */

    အတွက် (i = 0; i < 128; i++) {
        n = *p++;

        အကယ်၍ (n == 0) {
            goto done;
        }

        အကယ်၍ (n & 0xc0) {
            အကယ်၍ ((n & 0xc0) != 0xc0) {
                err = "invalid label type in DNS response";
                goto invalid;
            }

            အကယ်၍ (p >= last) {
                err = "name is out of DNS response";
                goto invalid;
            }

            n = ((n & 0x3f) << 8) + *p;
            p = &buf[n];

        } မဟုတ်ပါက {
            len += 1 + n;
            p = &p[n];
        }

        အကယ်၍ (p >= last) {
            err = "name is out of DNS response";
            goto invalid;
        }
    }

    err = "compression pointers loop in DNS response";

invalid:

    ngx_log_error(r->log_level, r->log, 0, err);

    ပြန်ပေး NGX_ERROR;

done:

    အကယ်၍ (name == NULL) {
        ပြန်ပေး NGX_OK;
    }

    အကယ်၍ (len == 0) {
        ngx_str_null(name);
        ပြန်ပေး NGX_OK;
    }

    dst = ngx_resolver_alloc(r, len);
    အကယ်၍ (dst == NULL) {
        ပြန်ပေး NGX_ERROR;
    }

    name->data = dst;

    အတွက် ( ;; ) {
        n = *src++;

        အကယ်၍ (n == 0) {
            name->len = dst - name->data - 1;
            ပြန်ပေး NGX_OK;
        }

        အကယ်၍ (n & 0xc0) {
            n = ((n & 0x3f) << 8) + *src;
            src = &buf[n];

        } မဟုတ်ပါက {
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
    အကယ်၍ (ctx->event || ctx->timeout == 0) {
        ပြန်ပေး NGX_OK;
    }

    ctx->event = ngx_resolver_calloc(r, parimaa_na(ngx_event_t));
    အကယ်၍ (ctx->event == NULL) {
        ပြန်ပေး NGX_ERROR;
    }

    ctx->event->handler = ngx_resolver_timeout_handler;
    ctx->event->data = ctx;
    ctx->event->log = r->log;
    ctx->event->cancelable = ctx->cancelable;
    ctx->ident = -1;

    ngx_add_timer(ctx->event, ctx->timeout);

    ပြန်ပေး NGX_OK;
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

    အကယ်၍ (rn->query) {
        ngx_resolver_free_locked(r, rn->query);
    }

    အကယ်၍ (rn->name) {
        ngx_resolver_free_locked(r, rn->name);
    }

    အကယ်၍ (rn->cnlen) {
        ngx_resolver_free_locked(r, rn->u.cname);
    }

    အကယ်၍ (rn->naddrs > 1 && rn->naddrs != (u_short) -1) {
        ngx_resolver_free_locked(r, rn->u.addrs);
    }

#အကယ်၍ (NGX_HAVE_INET6)
    အကယ်၍ (rn->naddrs6 > 1 && rn->naddrs6 != (u_short) -1) {
        ngx_resolver_free_locked(r, rn->u6.addrs6);
    }
#endif

    အကယ်၍ (rn->nsrvs) {
        အတွက် (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
            အကယ်၍ (rn->u.srvs[i].name.data) {
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

    ပြန်ပေး p;
}


static void *
ngx_resolver_calloc(ngx_resolver_t *r, size_t size)
{
    u_char  *p;

    p = ngx_resolver_alloc(r, size);

    အကယ်၍ (p) {
        ngx_memzero(p, size);
    }

    ပြန်ပေး p;
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

    အကယ်၍ (dst == NULL) {
        ပြန်ပေး dst;
    }

    ngx_memcpy(dst, src, size);

    ပြန်ပေး dst;
}


static ngx_resolver_addr_t *
ngx_resolver_export(ngx_resolver_t *r, ngx_resolver_node_t *rn,
    ngx_uint_t rotate)
{
    ngx_uint_t            d, i, j, n;
    in_addr_t            *addr;
    ngx_sockaddr_t       *sockaddr;
    sa_nracanaa sockaddr_in   *sin;
    ngx_resolver_addr_t  *dst;
#အကယ်၍ (NGX_HAVE_INET6)
    sa_nracanaa in6_addr      *addr6;
    sa_nracanaa sockaddr_in6  *sin6;
#endif

    n = rn->naddrs;
#အကယ်၍ (NGX_HAVE_INET6)
    n += rn->naddrs6;
#endif

    dst = ngx_resolver_calloc(r, n * parimaa_na(ngx_resolver_addr_t));
    အကယ်၍ (dst == NULL) {
        ပြန်ပေး NULL;
    }

    sockaddr = ngx_resolver_calloc(r, n * parimaa_na(ngx_sockaddr_t));
    အကယ်၍ (sockaddr == NULL) {
        ngx_resolver_free(r, dst);
        ပြန်ပေး NULL;
    }

    i = 0;
    d = rotate ? ngx_random() % n : 0;

    အကယ်၍ (rn->naddrs) {
        j = rotate ? ngx_random() % rn->naddrs : 0;

        addr = (rn->naddrs == 1) ? &rn->u.addr : rn->u.addrs;

        do {
            sin = &sockaddr[d].sockaddr_in;
            sin->sin_family = AF_INET;
            sin->sin_addr.s_addr = addr[j++];
            dst[d].sockaddr = (sa_nracanaa sockaddr *) sin;
            dst[d++].socklen = parimaa_na(sa_nracanaa sockaddr_in);

            အကယ်၍ (d == n) {
                d = 0;
            }

            အကယ်၍ (j == (ngx_uint_t) rn->naddrs) {
                j = 0;
            }
        } နေစဉ် (++i < (ngx_uint_t) rn->naddrs);
    }

#အကယ်၍ (NGX_HAVE_INET6)
    အကယ်၍ (rn->naddrs6) {
        j = rotate ? ngx_random() % rn->naddrs6 : 0;

        addr6 = (rn->naddrs6 == 1) ? &rn->u6.addr6 : rn->u6.addrs6;

        do {
            sin6 = &sockaddr[d].sockaddr_in6;
            sin6->sin6_family = AF_INET6;
            ngx_memcpy(sin6->sin6_addr.s6_addr, addr6[j++].s6_addr, 16);
            dst[d].sockaddr = (sa_nracanaa sockaddr *) sin6;
            dst[d++].socklen = parimaa_na(sa_nracanaa sockaddr_in6);

            အကယ်၍ (d == n) {
                d = 0;
            }

            အကယ်၍ (j == rn->naddrs6) {
                j = 0;
            }
        } နေစဉ် (++i < n);
    }
#endif

    ပြန်ပေး dst;
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

    အတွက် (i = 0; i < nsrvs; i++) {
        အကယ်၍ (srvs[i].state == NGX_ERROR) {
            ctx->state = NGX_ERROR;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

            ctx->handler(ctx);
            ပြန်ပေး;
        }

        naddrs += srvs[i].naddrs;
    }

    အကယ်၍ (naddrs == 0) {
        ctx->state = srvs[0].state;

        အတွက် (i = 0; i < nsrvs; i++) {
            အကယ်၍ (srvs[i].state == NGX_RESOLVE_NXDOMAIN) {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                bha_nga;
            }
        }

        ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        ပြန်ပေး;
    }

    addrs = ngx_resolver_calloc(r, naddrs * parimaa_na(ngx_resolver_addr_t));
    အကယ်၍ (addrs == NULL) {
        ctx->state = NGX_ERROR;
        ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        ပြန်ပေး;
    }

    i = 0;
    n = 0;

    do {
        nw = 0;

        အတွက် (j = i; j < nsrvs; j++) {
            အကယ်၍ (srvs[j].priority != srvs[i].priority) {
                bha_nga;
            }

            nw += srvs[j].naddrs * srvs[j].weight;
        }

        အကယ်၍ (nw == 0) {
            goto next_srv;
        }

        w = ngx_random() % nw;

        အတွက် (k = i; k < j; k++) {
            အကယ်၍ (w < srvs[k].naddrs * srvs[k].weight) {
                bha_nga;
            }

            w -= srvs[k].naddrs * srvs[k].weight;
        }

        အတွက် (l = i; l < j; l++) {

            အတွက် (m = 0; m < srvs[k].naddrs; m++) {
                addrs[n].socklen = srvs[k].addrs[m].socklen;
                addrs[n].sockaddr = srvs[k].addrs[m].sockaddr;
                addrs[n].name = srvs[k].name;
                addrs[n].priority = srvs[k].priority;
                addrs[n].weight = srvs[k].weight;
                n++;
            }

            အကယ်၍ (++k == j) {
                k = i;
            }
        }

next_srv:

        i = j;

    } နေစဉ် (i < ctx->nsrvs);

    ctx->state = NGX_OK;
    ctx->addrs = addrs;
    ctx->naddrs = naddrs;

    ctx->handler(ctx);

    ngx_resolver_free(r, addrs);
}


var_na *
ngx_resolver_strerror(ngx_int_t err)
{
    static var_na *errors[] = {
        "Format error",     /* FORMERR */
        "Server failure",   /* SERVFAIL */
        "Host not found",   /* NXDOMAIN */
        "Unimplemented",    /* NOTIMP */
        "Operation refused" /* REFUSED */
    };

    အကယ်၍ (err > 0 && err < 6) {
        ပြန်ပေး errors[err - 1];
    }

    အကယ်၍ (err == NGX_RESOLVE_TIMEDOUT) {
        ပြန်ပေး "Operation timed out";
    }

    ပြန်ပေး "Unknown error";
}


static u_char *
ngx_resolver_log_error(ngx_log_t *log, u_char *buf, size_t len)
{
    u_char                     *p;
    ngx_resolver_connection_t  *rec;

    p = buf;

    အကယ်၍ (log->action) {
        p = ngx_snprintf(buf, len, " while %s", log->action);
        len -= p - buf;
    }

    rec = log->data;

    အကယ်၍ (rec) {
        p = ngx_snprintf(p, len, ", resolver: %V", &rec->server);
    }

    ပြန်ပေး p;
}


static ngx_int_t
ngx_udp_connect(ngx_resolver_connection_t *rec)
{
    ကိန်းပြည့်                rc;
    ngx_int_t          event;
    ngx_event_t       *rev, *wev;
    ngx_socket_t       s;
    ngx_connection_t  *c;

    s = ngx_socket(rec->sockaddr->sa_family, SOCK_DGRAM, 0);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "UDP socket %d", s);

    အကယ်၍ (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_socket_n " failed");
        ပြန်ပေး NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->log);

    အကယ်၍ (c == NULL) {
        အကယ်၍ (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        ပြန်ပေး NGX_ERROR;
    }

    အကယ်၍ (ngx_nonblocking(s) == -1) {
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

    အကယ်၍ (rc == -1) {
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

    အကယ်၍ (ngx_add_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        goto failed;
    }

    ပြန်ပေး NGX_OK;

failed:

    ngx_close_connection(c);
    rec->udp = NULL;

    ပြန်ပေး NGX_ERROR;
}


static ngx_int_t
ngx_tcp_connect(ngx_resolver_connection_t *rec)
{
    ကိန်းပြည့်                rc;
    ngx_int_t          event;
    ngx_err_t          err;
    ngx_uint_t         level;
    ngx_socket_t       s;
    ngx_event_t       *rev, *wev;
    ngx_connection_t  *c;

    s = ngx_socket(rec->sockaddr->sa_family, SOCK_STREAM, 0);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "TCP socket %d", s);

    အကယ်၍ (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_socket_n " failed");
        ပြန်ပေး NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->log);

    အကယ်၍ (c == NULL) {
        အကယ်၍ (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        ပြန်ပေး NGX_ERROR;
    }

    အကယ်၍ (ngx_nonblocking(s) == -1) {
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

    အကယ်၍ (ngx_add_conn) {
        အကယ်၍ (ngx_add_conn(c) == NGX_ERROR) {
            goto failed;
        }
    }

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->log, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->number);

    rc = connect(s, rec->sockaddr, rec->socklen);

    အကယ်၍ (rc == -1) {
        err = ngx_socket_errno;


        အကယ်၍ (err != NGX_EINPROGRESS
#အကယ်၍ (NGX_WIN32)
            /* Winsock returns WSAEWOULDBLOCK (NGX_EAGAIN) */
            && err != NGX_EAGAIN
#endif
            )
        {
            အကယ်၍ (err == NGX_ECONNREFUSED
#အကယ်၍ (NGX_LINUX)
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

            } မဟုတ်ပါက {
                level = NGX_LOG_CRIT;
            }

            ngx_log_error(level, &rec->log, err, "connect() to %V failed",
                          &rec->server);

            ngx_close_connection(c);
            rec->tcp = NULL;

            ပြန်ပေး NGX_ERROR;
        }
    }

    အကယ်၍ (ngx_add_conn) {
        အကယ်၍ (rc == -1) {

            /* NGX_EINPROGRESS */

            ပြန်ပေး NGX_AGAIN;
        }

        ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "connected");

        wev->ready = 1;

        ပြန်ပေး NGX_OK;
    }

    အကယ်၍ (ngx_event_flags & NGX_USE_CLEAR_EVENT) {

        /* kqueue */

        event = NGX_CLEAR_EVENT;

    } မဟုတ်ပါက {

        /* select, poll, /dev/poll */

        event = NGX_LEVEL_EVENT;
    }

    အကယ်၍ (ngx_add_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        goto failed;
    }

    အကယ်၍ (rc == -1) {

        /* NGX_EINPROGRESS */

        အကယ်၍ (ngx_add_event(wev, NGX_WRITE_EVENT, event) != NGX_OK) {
            goto failed;
        }

        ပြန်ပေး NGX_AGAIN;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "connected");

    wev->ready = 1;

    ပြန်ပေး NGX_OK;

failed:

    ngx_close_connection(c);
    rec->tcp = NULL;

    ပြန်ပေး NGX_ERROR;
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

    ပြန်ပေး p1 - p2;
}
