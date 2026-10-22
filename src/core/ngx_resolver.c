
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <ngx_event.h>


#define NGX_RESOLVER_UDP_SIZE   4096

#define NGX_RESOLVER_TCP_RSIZE  (2 + 65535)
#define NGX_RESOLVER_TCP_WSIZE  8192


تعريفنوع بنية {
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


تعريفنوع بنية {
    u_char  type_hi;
    u_char  type_lo;
    u_char  class_hi;
    u_char  class_lo;
} ngx_resolver_qs_t;


تعريفنوع بنية {
    u_char  type_hi;
    u_char  type_lo;
    u_char  class_hi;
    u_char  class_lo;
    u_char  ttl[4];
    u_char  len_hi;
    u_char  len_lo;
} ngx_resolver_an_t;


#define ngx_resolver_node(n)  ngx_rbtree_data(n, ngx_resolver_node_t, node)


ساكن ngx_int_t ngx_udp_connect(ngx_resolver_connection_t *rec);
ساكن ngx_int_t ngx_tcp_connect(ngx_resolver_connection_t *rec);


ساكن فراغ ngx_resolver_cleanup(فراغ *data);
ساكن فراغ ngx_resolver_cleanup_tree(ngx_resolver_t *r, ngx_rbtree_t *tree);
ساكن ngx_int_t ngx_resolve_name_locked(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx, ngx_str_t *name);
ساكن فراغ ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *tree,
    ngx_queue_t *queue);
ساكن ngx_int_t ngx_resolver_send_query(ngx_resolver_t *r,
    ngx_resolver_node_t *rn);
ساكن ngx_int_t ngx_resolver_send_udp_query(ngx_resolver_t *r,
    ngx_resolver_connection_t *rec, u_char *query, u_short qlen);
ساكن ngx_int_t ngx_resolver_send_tcp_query(ngx_resolver_t *r,
    ngx_resolver_connection_t *rec, u_char *query, u_short qlen);
ساكن ngx_int_t ngx_resolver_create_name_query(ngx_resolver_t *r,
    ngx_resolver_node_t *rn, ngx_str_t *name);
ساكن ngx_int_t ngx_resolver_create_srv_query(ngx_resolver_t *r,
    ngx_resolver_node_t *rn, ngx_str_t *name);
ساكن ngx_int_t ngx_resolver_create_addr_query(ngx_resolver_t *r,
    ngx_resolver_node_t *rn, ngx_resolver_addr_t *addr);
ساكن فراغ ngx_resolver_resend_handler(ngx_event_t *ev);
ساكن time_t ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *tree,
    ngx_queue_t *queue);
ساكن ngx_uint_t ngx_resolver_resend_empty(ngx_resolver_t *r);
ساكن فراغ ngx_resolver_udp_read(ngx_event_t *rev);
ساكن فراغ ngx_resolver_tcp_write(ngx_event_t *wev);
ساكن فراغ ngx_resolver_tcp_read(ngx_event_t *rev);
ساكن فراغ ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf,
    size_t n, ngx_uint_t tcp);
ساكن فراغ ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans);
ساكن فراغ ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans);
ساكن فراغ ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan);
ساكن ngx_resolver_node_t *ngx_resolver_lookup_name(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
ساكن ngx_resolver_node_t *ngx_resolver_lookup_srv(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
ساكن ngx_resolver_node_t *ngx_resolver_lookup_addr(ngx_resolver_t *r,
    in_addr_t addr);
ساكن فراغ ngx_resolver_rbtree_insert_value(ngx_rbtree_node_t *temp,
    ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel);
ساكن ngx_int_t ngx_resolver_copy(ngx_resolver_t *r, ngx_str_t *name,
    u_char *buf, u_char *src, u_char *last);
ساكن ngx_int_t ngx_resolver_set_timeout(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx);
ساكن فراغ ngx_resolver_timeout_handler(ngx_event_t *ev);
ساكن فراغ ngx_resolver_free_node(ngx_resolver_t *r, ngx_resolver_node_t *rn);
ساكن فراغ *ngx_resolver_alloc(ngx_resolver_t *r, size_t size);
ساكن فراغ *ngx_resolver_calloc(ngx_resolver_t *r, size_t size);
ساكن فراغ ngx_resolver_free(ngx_resolver_t *r, فراغ *p);
ساكن فراغ ngx_resolver_free_locked(ngx_resolver_t *r, فراغ *p);
ساكن فراغ *ngx_resolver_dup(ngx_resolver_t *r, فراغ *src, size_t size);
ساكن ngx_resolver_addr_t *ngx_resolver_export(ngx_resolver_t *r,
    ngx_resolver_node_t *rn, ngx_uint_t rotate);
ساكن فراغ ngx_resolver_report_srv(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx);
ساكن u_char *ngx_resolver_log_error(ngx_log_t *log, u_char *buf, size_t len);
ساكن فراغ ngx_resolver_resolve_srv_names(ngx_resolver_ctx_t *ctx,
    ngx_resolver_node_t *rn);
ساكن فراغ ngx_resolver_srv_names_handler(ngx_resolver_ctx_t *ctx);
ساكن ngx_int_t ngx_resolver_cmp_srvs(ثابت فراغ *one, ثابت فراغ *two);

#אם (NGX_HAVE_INET6)
ساكن فراغ ngx_resolver_rbtree_insert_addr6_value(ngx_rbtree_node_t *temp,
    ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel);
ساكن ngx_resolver_node_t *ngx_resolver_lookup_addr6(ngx_resolver_t *r,
    بنية in6_addr *addr, uint32_t hash);
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

    r = ngx_pcalloc(cf->pool, حجم(ngx_resolver_t));
    אם (r == عدم) {
        החזר عدم;
    }

    r->event = ngx_pcalloc(cf->pool, حجم(ngx_event_t));
    אם (r->event == عدم) {
        החזר عدم;
    }

    cln = ngx_pool_cleanup_add(cf->pool, 0);
    אם (cln == عدم) {
        החזר عدم;
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

#אם (NGX_HAVE_INET6)
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

    אם (n) {
        אם (ngx_array_init(&r->connections, cf->pool, n,
                           حجم(ngx_resolver_connection_t))
            != NGX_OK)
        {
            החזר عدم;
        }
    }

    עבור (i = 0; i < n; i++) {
        אם (ngx_strncmp(names[i].data, "valid=", 6) == 0) {
            s.len = names[i].len - 6;
            s.data = names[i].data + 6;

            r->valid = ngx_parse_time(&s, 1);

            אם (r->valid == (time_t) NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &names[i]);
                החזר عدم;
            }

            استمر;
        }

#אם (NGX_HAVE_INET6)
        אם (ngx_strncmp(names[i].data, "ipv4=", 5) == 0) {

            אם (ngx_strcmp(&names[i].data[5], "on") == 0) {
                r->ipv4 = 1;

            } אחרת אם (ngx_strcmp(&names[i].data[5], "off") == 0) {
                r->ipv4 = 0;

            } אחרת {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &names[i]);
                החזר عدم;
            }

            استمر;
        }

        אם (ngx_strncmp(names[i].data, "ipv6=", 5) == 0) {

            אם (ngx_strcmp(&names[i].data[5], "on") == 0) {
                r->ipv6 = 1;

            } אחרת אם (ngx_strcmp(&names[i].data[5], "off") == 0) {
                r->ipv6 = 0;

            } אחרת {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &names[i]);
                החזר عدم;
            }

            استمر;
        }
#endif

        ngx_memzero(&u, حجم(ngx_url_t));

        u.url = names[i];
        u.default_port = 53;

        אם (ngx_parse_url(cf->pool, &u) != NGX_OK) {
            אם (u.err) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "%s in resolver \"%V\"",
                                   u.err, &u.url);
            }

            החזר عدم;
        }

        rec = ngx_array_push_n(&r->connections, u.naddrs);
        אם (rec == عدم) {
            החזר عدم;
        }

        ngx_memzero(rec, u.naddrs * حجم(ngx_resolver_connection_t));

        עבור (j = 0; j < u.naddrs; j++) {
            rec[j].sockaddr = u.addrs[j].sockaddr;
            rec[j].socklen = u.addrs[j].socklen;
            rec[j].server = u.addrs[j].name;
            rec[j].resolver = r;
        }
    }

#אם (NGX_HAVE_INET6)
    אם (r->ipv4 + r->ipv6 == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"ipv4\" and \"ipv6\" cannot both be \"off\"");
        החזר عدم;
    }
#endif

    אם (n && r->connections.nelts == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "no name servers defined");
        החזר عدم;
    }

    החזר r;
}


ساكن فراغ
ngx_resolver_cleanup(فراغ *data)
{
    ngx_resolver_t  *r = data;

    ngx_uint_t                  i;
    ngx_resolver_connection_t  *rec;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "cleanup resolver");

    ngx_resolver_cleanup_tree(r, &r->name_rbtree);

    ngx_resolver_cleanup_tree(r, &r->srv_rbtree);

    ngx_resolver_cleanup_tree(r, &r->addr_rbtree);

#אם (NGX_HAVE_INET6)
    ngx_resolver_cleanup_tree(r, &r->addr6_rbtree);
#endif

    אם (r->event->timer_set) {
        ngx_del_timer(r->event);
    }

    rec = r->connections.elts;

    עבור (i = 0; i < r->connections.nelts; i++) {
        אם (rec[i].udp) {
            ngx_close_connection(rec[i].udp);
        }

        אם (rec[i].tcp) {
            ngx_close_connection(rec[i].tcp);
        }

        אם (rec[i].read_buf) {
            ngx_resolver_free(r, rec[i].read_buf->start);
            ngx_resolver_free(r, rec[i].read_buf);
        }

        אם (rec[i].write_buf) {
            ngx_resolver_free(r, rec[i].write_buf->start);
            ngx_resolver_free(r, rec[i].write_buf);
        }
    }
}


ساكن فراغ
ngx_resolver_cleanup_tree(ngx_resolver_t *r, ngx_rbtree_t *tree)
{
    ngx_resolver_ctx_t   *ctx, *next;
    ngx_resolver_node_t  *rn;

    כלעוד (tree->root != tree->sentinel) {

        rn = ngx_resolver_node(ngx_rbtree_min(tree->root, tree->sentinel));

        ngx_queue_remove(&rn->queue);

        עבור (ctx = rn->waiting; ctx; ctx = next) {
            next = ctx->next;

            אם (ctx->event) {
                אם (ctx->event->timer_set) {
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

    אם (temp) {
        addr = ngx_inet_addr(temp->name.data, temp->name.len);

        אם (addr != INADDR_NONE) {
            temp->resolver = r;
            temp->state = NGX_OK;
            temp->naddrs = 1;
            temp->addrs = &temp->addr;
            temp->addr.sockaddr = (بنية sockaddr *) &temp->sin;
            temp->addr.socklen = حجم(بنية sockaddr_in);
            ngx_memzero(&temp->sin, حجم(بنية sockaddr_in));
            temp->sin.sin_family = AF_INET;
            temp->sin.sin_addr.s_addr = addr;
            temp->quick = 1;

            החזר temp;
        }
    }

    אם (r->connections.nelts == 0) {
        החזר NGX_NO_RESOLVER;
    }

    ctx = ngx_resolver_calloc(r, حجم(ngx_resolver_ctx_t));

    אם (ctx) {
        ctx->resolver = r;
    }

    החזר ctx;
}


ngx_int_t
ngx_resolve_name(ngx_resolver_ctx_t *ctx)
{
    size_t           slen;
    ngx_int_t        rc;
    ngx_str_t        name;
    ngx_resolver_t  *r;

    r = ctx->resolver;

    אם (ctx->name.len > 0 && ctx->name.data[ctx->name.len - 1] == '.') {
        ctx->name.len--;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve: \"%V\"", &ctx->name);

    אם (ctx->quick) {
        ctx->handler(ctx);
        החזר NGX_OK;
    }

    אם (ctx->service.len) {
        slen = ctx->service.len;

        אם (ngx_strlchr(ctx->service.data,
                        ctx->service.data + ctx->service.len, '.')
            == عدم)
        {
            slen += حجم("_._tcp") - 1;
        }

        name.len = slen + 1 + ctx->name.len;

        name.data = ngx_resolver_alloc(r, name.len);
        אם (name.data == عدم) {
            اذهب failed;
        }

        אם (slen == ctx->service.len) {
            ngx_sprintf(name.data, "%V.%V", &ctx->service, &ctx->name);

        } אחרת {
            ngx_sprintf(name.data, "_%V._tcp.%V", &ctx->service, &ctx->name);
        }

        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &name);

        ngx_resolver_free(r, name.data);

    } אחרת {
        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &ctx->name);
    }

    אם (rc == NGX_OK) {
        החזר NGX_OK;
    }

    /* unlock name mutex */

    אם (rc == NGX_AGAIN) {
        החזר NGX_OK;
    }

    /* NGX_ERROR */

    אם (ctx->event) {
        ngx_resolver_free(r, ctx->event);
    }

failed:

    ngx_resolver_free(r, ctx);

    החזר NGX_ERROR;
}


فراغ
ngx_resolve_name_done(ngx_resolver_ctx_t *ctx)
{
    ngx_uint_t            i;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_node_t  *rn;

    r = ctx->resolver;

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve name done: %i", ctx->state);

    אם (ctx->quick) {
        החזר;
    }

    אם (ctx->event && ctx->event->timer_set) {
        ngx_del_timer(ctx->event);
    }

    /* lock name mutex */

    אם (ctx->nsrvs) {
        עבור (i = 0; i < ctx->nsrvs; i++) {
            אם (ctx->srvs[i].ctx) {
                ngx_resolve_name_done(ctx->srvs[i].ctx);
            }

            אם (ctx->srvs[i].addrs) {
                ngx_resolver_free(r, ctx->srvs[i].addrs->sockaddr);
                ngx_resolver_free(r, ctx->srvs[i].addrs);
            }

            ngx_resolver_free(r, ctx->srvs[i].name.data);
        }

        ngx_resolver_free(r, ctx->srvs);
    }

    אם (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->node;

        אם (rn) {
            p = &rn->waiting;
            w = rn->waiting;

            כלעוד (w) {
                אם (w == ctx) {
                    *p = w->next;

                    اذهب done;
                }

                p = &w->next;
                w = w->next;
            }

            ngx_log_error(NGX_LOG_ALERT, r->log, 0,
                          "could not cancel %V resolving", &ctx->name);
        }
    }

done:

    אם (ctx->service.len) {
        ngx_resolver_expire(r, &r->srv_rbtree, &r->srv_expire_queue);

    } אחרת {
        ngx_resolver_expire(r, &r->name_rbtree, &r->name_expire_queue);
    }

    /* unlock name mutex */

    /* lock alloc mutex */

    אם (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    אם (r->event->timer_set && ngx_resolver_resend_empty(r)) {
        ngx_del_timer(r->event);
    }
}


ساكن ngx_int_t
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

    אם (ctx->service.len) {
        rn = ngx_resolver_lookup_srv(r, name, hash);

        tree = &r->srv_rbtree;
        resend_queue = &r->srv_resend_queue;
        expire_queue = &r->srv_expire_queue;

    } אחרת {
        rn = ngx_resolver_lookup_name(r, name, hash);

        tree = &r->name_rbtree;
        resend_queue = &r->name_resend_queue;
        expire_queue = &r->name_expire_queue;
    }

    אם (rn) {

        /* ctx can be a list after NGX_RESOLVE_CNAME */
        עבור (last = ctx; last->next; last = last->next);

        אם (rn->valid >= ngx_time()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0, "resolve cached");

            ngx_queue_remove(&rn->queue);

            rn->expire = ngx_time() + r->expire;

            ngx_queue_insert_head(expire_queue, &rn->queue);

            naddrs = (rn->naddrs == (u_short) -1) ? 0 : rn->naddrs;
#אם (NGX_HAVE_INET6)
            naddrs += (rn->naddrs6 == (u_short) -1) ? 0 : rn->naddrs6;
#endif

            אם (naddrs) {

                אם (naddrs == 1 && rn->naddrs == 1) {
                    addrs = عدم;

                } אחרת {
                    addrs = ngx_resolver_export(r, rn, 1);
                    אם (addrs == عدم) {
                        החזר NGX_ERROR;
                    }
                }

                last->next = rn->waiting;
                rn->waiting = عدم;

                /* unlock name mutex */

                עשה {
                    ctx->state = NGX_OK;
                    ctx->valid = rn->valid;
                    ctx->naddrs = naddrs;

                    אם (addrs == عدم) {
                        ctx->addrs = &ctx->addr;
                        ctx->addr.sockaddr = (بنية sockaddr *) &ctx->sin;
                        ctx->addr.socklen = حجم(بنية sockaddr_in);
                        ngx_memzero(&ctx->sin, حجم(بنية sockaddr_in));
                        ctx->sin.sin_family = AF_INET;
                        ctx->sin.sin_addr.s_addr = rn->u.addr;

                    } אחרת {
                        ctx->addrs = addrs;
                    }

                    next = ctx->next;

                    ctx->handler(ctx);

                    ctx = next;
                } כלעוד (ctx);

                אם (addrs != عدم) {
                    ngx_resolver_free(r, addrs->sockaddr);
                    ngx_resolver_free(r, addrs);
                }

                החזר NGX_OK;
            }

            אם (rn->nsrvs) {
                last->next = rn->waiting;
                rn->waiting = عدم;

                /* unlock name mutex */

                עשה {
                    next = ctx->next;

                    ngx_resolver_resolve_srv_names(ctx, rn);

                    ctx = next;
                } כלעוד (ctx);

                החזר NGX_OK;
            }

            /* NGX_RESOLVE_CNAME */

            אם (ctx->recursion++ < NGX_RESOLVER_MAX_RECURSION) {

                cname.len = rn->cnlen;
                cname.data = rn->u.cname;

                החזר ngx_resolve_name_locked(r, ctx, &cname);
            }

            last->next = rn->waiting;
            rn->waiting = عدم;

            /* unlock name mutex */

            עשה {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
                next = ctx->next;

                ctx->handler(ctx);

                ctx = next;
            } כלעוד (ctx);

            החזר NGX_OK;
        }

        אם (rn->waiting) {
            אם (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
                החזר NGX_ERROR;
            }

            last->next = rn->waiting;
            rn->waiting = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;

            עשה {
                ctx->node = rn;
                ctx = ctx->next;
            } כלעוד (ctx);

            החזר NGX_AGAIN;
        }

        ngx_queue_remove(&rn->queue);

        /* lock alloc mutex */

        אם (rn->query) {
            ngx_resolver_free_locked(r, rn->query);
            rn->query = عدم;
#אם (NGX_HAVE_INET6)
            rn->query6 = عدم;
#endif
        }

        אם (rn->cnlen) {
            ngx_resolver_free_locked(r, rn->u.cname);
        }

        אם (rn->naddrs > 1 && rn->naddrs != (u_short) -1) {
            ngx_resolver_free_locked(r, rn->u.addrs);
        }

#אם (NGX_HAVE_INET6)
        אם (rn->naddrs6 > 1 && rn->naddrs6 != (u_short) -1) {
            ngx_resolver_free_locked(r, rn->u6.addrs6);
        }
#endif

        אם (rn->nsrvs) {
            עבור (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
                אם (rn->u.srvs[i].name.data) {
                    ngx_resolver_free_locked(r, rn->u.srvs[i].name.data);
                }
            }

            ngx_resolver_free_locked(r, rn->u.srvs);
        }

        /* unlock alloc mutex */

    } אחרת {

        rn = ngx_resolver_alloc(r, حجم(ngx_resolver_node_t));
        אם (rn == عدم) {
            החזר NGX_ERROR;
        }

        rn->name = ngx_resolver_dup(r, name->data, name->len);
        אם (rn->name == عدم) {
            ngx_resolver_free(r, rn);
            החזר NGX_ERROR;
        }

        rn->node.key = hash;
        rn->nlen = (u_short) name->len;
        rn->query = عدم;
#אם (NGX_HAVE_INET6)
        rn->query6 = عدم;
#endif

        ngx_rbtree_insert(tree, &rn->node);
    }

    אם (ctx->service.len) {
        rc = ngx_resolver_create_srv_query(r, rn, name);

    } אחרת {
        rc = ngx_resolver_create_name_query(r, rn, name);
    }

    אם (rc == NGX_ERROR) {
        اذهب failed;
    }

    אם (rc == NGX_DECLINED) {
        ngx_rbtree_delete(tree, &rn->node);

        ngx_resolver_free(r, rn->query);
        ngx_resolver_free(r, rn->name);
        ngx_resolver_free(r, rn);

        עשה {
            ctx->state = NGX_RESOLVE_NXDOMAIN;
            next = ctx->next;

            ctx->handler(ctx);

            ctx = next;
        } כלעוד (ctx);

        החזר NGX_OK;
    }

    rn->last_connection = r->last_connection++;
    אם (r->last_connection == r->connections.nelts) {
        r->last_connection = 0;
    }

    rn->naddrs = r->ipv4 ? (u_short) -1 : 0;
    rn->tcp = 0;
#אם (NGX_HAVE_INET6)
    rn->naddrs6 = r->ipv6 ? (u_short) -1 : 0;
    rn->tcp6 = 0;
#endif
    rn->nsrvs = 0;

    אם (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->last_connection++;
        אם (rn->last_connection == r->connections.nelts) {
            rn->last_connection = 0;
        }

        (فراغ) ngx_resolver_send_query(r, rn);
    }

    אם (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
        اذهب failed;
    }

    אם (ngx_resolver_resend_empty(r)) {
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

    עשה {
        ctx->node = rn;
        ctx = ctx->next;
    } כלעוד (ctx);

    החזר NGX_AGAIN;

failed:

    ngx_rbtree_delete(tree, &rn->node);

    אם (rn->query) {
        ngx_resolver_free(r, rn->query);
    }

    ngx_resolver_free(r, rn->name);

    ngx_resolver_free(r, rn);

    החזר NGX_ERROR;
}


ngx_int_t
ngx_resolve_addr(ngx_resolver_ctx_t *ctx)
{
    u_char               *name;
    in_addr_t             addr;
    ngx_queue_t          *resend_queue, *expire_queue;
    ngx_rbtree_t         *tree;
    ngx_resolver_t       *r;
    بنية sockaddr_in   *sin;
    ngx_resolver_node_t  *rn;
#אם (NGX_HAVE_INET6)
    uint32_t              hash;
    بنية sockaddr_in6  *sin6;
#endif

#אם (NGX_SUPPRESS_WARN)
    addr = 0;
#אם (NGX_HAVE_INET6)
    hash = 0;
    sin6 = عدم;
#endif
#endif

    r = ctx->resolver;

    بدل (ctx->addr.sockaddr->sa_family) {

#אם (NGX_HAVE_INET6)
    حالة AF_INET6:
        sin6 = (بنية sockaddr_in6 *) ctx->addr.sockaddr;
        hash = ngx_crc32_short(sin6->sin6_addr.s6_addr, 16);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr6(r, &sin6->sin6_addr, hash);

        tree = &r->addr6_rbtree;
        resend_queue = &r->addr6_resend_queue;
        expire_queue = &r->addr6_expire_queue;

        اكسر;
#endif

    افتراضي: /* AF_INET */
        sin = (بنية sockaddr_in *) ctx->addr.sockaddr;
        addr = ntohl(sin->sin_addr.s_addr);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        tree = &r->addr_rbtree;
        resend_queue = &r->addr_resend_queue;
        expire_queue = &r->addr_expire_queue;
    }

    אם (rn) {

        אם (rn->valid >= ngx_time()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0, "resolve cached");

            ngx_queue_remove(&rn->queue);

            rn->expire = ngx_time() + r->expire;

            ngx_queue_insert_head(expire_queue, &rn->queue);

            name = ngx_resolver_dup(r, rn->name, rn->nlen);
            אם (name == عدم) {
                ngx_resolver_free(r, ctx);
                החזר NGX_ERROR;
            }

            ctx->name.len = rn->nlen;
            ctx->name.data = name;

            /* unlock addr mutex */

            ctx->state = NGX_OK;
            ctx->valid = rn->valid;

            ctx->handler(ctx);

            ngx_resolver_free(r, name);

            החזר NGX_OK;
        }

        אם (rn->waiting) {
            אם (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
                החזר NGX_ERROR;
            }

            ctx->next = rn->waiting;
            rn->waiting = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;
            ctx->node = rn;

            /* unlock addr mutex */

            החזר NGX_OK;
        }

        ngx_queue_remove(&rn->queue);

        ngx_resolver_free(r, rn->query);
        rn->query = عدم;
#אם (NGX_HAVE_INET6)
        rn->query6 = عدم;
#endif

    } אחרת {
        rn = ngx_resolver_alloc(r, حجم(ngx_resolver_node_t));
        אם (rn == عدم) {
            اذهب failed;
        }

        بدل (ctx->addr.sockaddr->sa_family) {

#אם (NGX_HAVE_INET6)
        حالة AF_INET6:
            rn->addr6 = sin6->sin6_addr;
            rn->node.key = hash;
            اكسر;
#endif

        افتراضي: /* AF_INET */
            rn->node.key = addr;
        }

        rn->query = عدم;
#אם (NGX_HAVE_INET6)
        rn->query6 = عدم;
#endif

        ngx_rbtree_insert(tree, &rn->node);
    }

    אם (ngx_resolver_create_addr_query(r, rn, &ctx->addr) != NGX_OK) {
        اذهب failed;
    }

    rn->last_connection = r->last_connection++;
    אם (r->last_connection == r->connections.nelts) {
        r->last_connection = 0;
    }

    rn->naddrs = (u_short) -1;
    rn->tcp = 0;
#אם (NGX_HAVE_INET6)
    rn->naddrs6 = (u_short) -1;
    rn->tcp6 = 0;
#endif
    rn->nsrvs = 0;

    אם (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->last_connection++;
        אם (rn->last_connection == r->connections.nelts) {
            rn->last_connection = 0;
        }

        (فراغ) ngx_resolver_send_query(r, rn);
    }

    אם (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
        اذهب failed;
    }

    אם (ngx_resolver_resend_empty(r)) {
        ngx_add_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_time() + r->resend_timeout;

    ngx_queue_insert_head(resend_queue, &rn->queue);

    rn->code = 0;
    rn->cnlen = 0;
    rn->name = عدم;
    rn->nlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_VALUE;
    rn->waiting = ctx;

    /* unlock addr mutex */

    ctx->state = NGX_AGAIN;
    ctx->async = 1;
    ctx->node = rn;

    החזר NGX_OK;

failed:

    אם (rn) {
        ngx_rbtree_delete(tree, &rn->node);

        אם (rn->query) {
            ngx_resolver_free(r, rn->query);
        }

        ngx_resolver_free(r, rn);
    }

    /* unlock addr mutex */

    אם (ctx->event) {
        ngx_resolver_free(r, ctx->event);
    }

    ngx_resolver_free(r, ctx);

    החזר NGX_ERROR;
}


فراغ
ngx_resolve_addr_done(ngx_resolver_ctx_t *ctx)
{
    ngx_queue_t          *expire_queue;
    ngx_rbtree_t         *tree;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_node_t  *rn;

    r = ctx->resolver;

    بدل (ctx->addr.sockaddr->sa_family) {

#אם (NGX_HAVE_INET6)
    حالة AF_INET6:
        tree = &r->addr6_rbtree;
        expire_queue = &r->addr6_expire_queue;
        اكسر;
#endif

    افتراضي: /* AF_INET */
        tree = &r->addr_rbtree;
        expire_queue = &r->addr_expire_queue;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve addr done: %i", ctx->state);

    אם (ctx->event && ctx->event->timer_set) {
        ngx_del_timer(ctx->event);
    }

    /* lock addr mutex */

    אם (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->node;

        אם (rn) {
            p = &rn->waiting;
            w = rn->waiting;

            כלעוד (w) {
                אם (w == ctx) {
                    *p = w->next;

                    اذهب done;
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

    אם (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    אם (r->event->timer_set && ngx_resolver_resend_empty(r)) {
        ngx_del_timer(r->event);
    }
}


ساكن فراغ
ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *tree, ngx_queue_t *queue)
{
    time_t                now;
    ngx_uint_t            i;
    ngx_queue_t          *q;
    ngx_resolver_node_t  *rn;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver expire");

    now = ngx_time();

    עבור (i = 0; i < 2; i++) {
        אם (ngx_queue_empty(queue)) {
            החזר;
        }

        q = ngx_queue_last(queue);

        rn = ngx_queue_data(q, ngx_resolver_node_t, queue);

        אם (now <= rn->expire) {
            החזר;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver expire \"%*s\"", (size_t) rn->nlen, rn->name);

        ngx_queue_remove(q);

        ngx_rbtree_delete(tree, &rn->node);

        ngx_resolver_free_node(r, rn);
    }
}


ساكن ngx_int_t
ngx_resolver_send_query(ngx_resolver_t *r, ngx_resolver_node_t *rn)
{
    ngx_int_t                   rc;
    ngx_resolver_connection_t  *rec;

    rec = r->connections.elts;
    rec = &rec[rn->last_connection];

    אם (rec->log.handler == عدم) {
        rec->log = *r->log;
        rec->log.handler = ngx_resolver_log_error;
        rec->log.data = rec;
        rec->log.action = "resolving";
    }

    אם (rn->query && rn->naddrs == (u_short) -1) {
        rc = rn->tcp ? ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen)
                     : ngx_resolver_send_udp_query(r, rec, rn->query, rn->qlen);

        אם (rc != NGX_OK) {
            החזר rc;
        }
    }

#אם (NGX_HAVE_INET6)

    אם (rn->query6 && rn->naddrs6 == (u_short) -1) {
        rc = rn->tcp6
                    ? ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen)
                    : ngx_resolver_send_udp_query(r, rec, rn->query6, rn->qlen);

        אם (rc != NGX_OK) {
            החזר rc;
        }
    }

#endif

    החזר NGX_OK;
}


ساكن ngx_int_t
ngx_resolver_send_udp_query(ngx_resolver_t *r, ngx_resolver_connection_t  *rec,
    u_char *query, u_short qlen)
{
    ssize_t  n;

    אם (rec->udp == عدم) {
        אם (ngx_udp_connect(rec) != NGX_OK) {
            החזר NGX_ERROR;
        }

        rec->udp->data = rec;
        rec->udp->اقرأ->handler = ngx_resolver_udp_read;
        rec->udp->اقرأ->resolver = 1;
    }

    n = ngx_send(rec->udp, query, qlen);

    אם (n == NGX_ERROR) {
        اذهب failed;
    }

    אם ((size_t) n != (size_t) qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->log, 0, "send() incomplete");
        اذهب failed;
    }

    החזר NGX_OK;

failed:

    ngx_close_connection(rec->udp);
    rec->udp = عدم;

    החזר NGX_ERROR;
}


ساكن ngx_int_t
ngx_resolver_send_tcp_query(ngx_resolver_t *r, ngx_resolver_connection_t *rec,
    u_char *query, u_short qlen)
{
    ngx_buf_t  *b;
    ngx_int_t   rc;

    rc = NGX_OK;

    אם (rec->tcp == عدم) {
        b = rec->read_buf;

        אם (b == عدم) {
            b = ngx_resolver_calloc(r, حجم(ngx_buf_t));
            אם (b == عدم) {
                החזר NGX_ERROR;
            }

            b->start = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_RSIZE);
            אם (b->start == عدم) {
                ngx_resolver_free(r, b);
                החזר NGX_ERROR;
            }

            b->end = b->start + NGX_RESOLVER_TCP_RSIZE;

            rec->read_buf = b;
        }

        b->pos = b->start;
        b->last = b->start;

        b = rec->write_buf;

        אם (b == عدم) {
            b = ngx_resolver_calloc(r, حجم(ngx_buf_t));
            אם (b == عدم) {
                החזר NGX_ERROR;
            }

            b->start = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_WSIZE);
            אם (b->start == عدم) {
                ngx_resolver_free(r, b);
                החזר NGX_ERROR;
            }

            b->end = b->start + NGX_RESOLVER_TCP_WSIZE;

            rec->write_buf = b;
        }

        b->pos = b->start;
        b->last = b->start;

        rc = ngx_tcp_connect(rec);
        אם (rc == NGX_ERROR) {
            החזר NGX_ERROR;
        }

        rec->tcp->data = rec;
        rec->tcp->اكتب->handler = ngx_resolver_tcp_write;
        rec->tcp->اكتب->cancelable = 1;
        rec->tcp->اقرأ->handler = ngx_resolver_tcp_read;
        rec->tcp->اقرأ->resolver = 1;

        ngx_add_timer(rec->tcp->اكتب, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    b = rec->write_buf;

    אם (b->end - b->last <  2 + qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->log, 0, "buffer overflow");
        החזר NGX_ERROR;
    }

    *b->last++ = (u_char) (qlen >> 8);
    *b->last++ = (u_char) qlen;
    b->last = ngx_cpymem(b->last, query, qlen);

    אם (rc == NGX_OK) {
        ngx_resolver_tcp_write(rec->tcp->اكتب);
    }

    החזר NGX_OK;
}


ساكن فراغ
ngx_resolver_resend_handler(ngx_event_t *ev)
{
    time_t           timer, atimer, stimer, ntimer;
#אם (NGX_HAVE_INET6)
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

#אם (NGX_HAVE_INET6)

    /* lock addr6 mutex */

    a6timer = ngx_resolver_resend(r, &r->addr6_rbtree, &r->addr6_resend_queue);

    /* unlock addr6 mutex */

#endif

    timer = ntimer;

    אם (timer == 0) {
        timer = atimer;

    } אחרת אם (atimer) {
        timer = ngx_min(timer, atimer);
    }

    אם (timer == 0) {
        timer = stimer;

    } אחרת אם (stimer) {
        timer = ngx_min(timer, stimer);
    }

#אם (NGX_HAVE_INET6)

    אם (timer == 0) {
        timer = a6timer;

    } אחרת אם (a6timer) {
        timer = ngx_min(timer, a6timer);
    }

#endif

    אם (timer) {
        ngx_add_timer(r->event, (ngx_msec_t) (timer * 1000));
    }
}


ساكن time_t
ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *tree, ngx_queue_t *queue)
{
    time_t                now;
    ngx_queue_t          *q;
    ngx_resolver_node_t  *rn;

    now = ngx_time();

    עבור ( ;; ) {
        אם (ngx_queue_empty(queue)) {
            החזר 0;
        }

        q = ngx_queue_last(queue);

        rn = ngx_queue_data(q, ngx_resolver_node_t, queue);

        אם (now < rn->expire) {
            החזר rn->expire - now;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver resend \"%*s\" %p",
                       (size_t) rn->nlen, rn->name, rn->waiting);

        ngx_queue_remove(q);

        אם (rn->waiting) {

            אם (++rn->last_connection == r->connections.nelts) {
                rn->last_connection = 0;
            }

            (فراغ) ngx_resolver_send_query(r, rn);

            rn->expire = now + r->resend_timeout;

            ngx_queue_insert_head(queue, q);

            استمر;
        }

        ngx_rbtree_delete(tree, &rn->node);

        ngx_resolver_free_node(r, rn);
    }
}


ساكن ngx_uint_t
ngx_resolver_resend_empty(ngx_resolver_t *r)
{
    החזר ngx_queue_empty(&r->name_resend_queue)
           && ngx_queue_empty(&r->srv_resend_queue)
#אם (NGX_HAVE_INET6)
           && ngx_queue_empty(&r->addr6_resend_queue)
#endif
           && ngx_queue_empty(&r->addr_resend_queue);
}


ساكن فراغ
ngx_resolver_udp_read(ngx_event_t *rev)
{
    ssize_t                     n;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;
    u_char                      buf[NGX_RESOLVER_UDP_SIZE];

    c = rev->data;
    rec = c->data;

    עשה {
        n = ngx_udp_recv(c, buf, NGX_RESOLVER_UDP_SIZE);

        אם (n == NGX_AGAIN) {
            اكسر;
        }

        אם (n == NGX_ERROR) {
            اذهب failed;
        }

        ngx_resolver_process_response(rec->resolver, buf, n, 0);

    } כלעוד (rev->ready);

    אם (ngx_handle_read_event(rev, 0) != NGX_OK) {
        اذهب failed;
    }

    החזר;

failed:

    ngx_close_connection(rec->udp);
    rec->udp = عدم;
}


ساكن فراغ
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

    אם (wev->timedout) {
        اذهب failed;
    }

    sent = c->sent;

    כלעוד (wev->ready && b->pos < b->last) {
        n = ngx_send(c, b->pos, b->last - b->pos);

        אם (n == NGX_AGAIN) {
            اكسر;
        }

        אם (n == NGX_ERROR) {
            اذهب failed;
        }

        b->pos += n;
    }

    אם (b->pos != b->start) {
        b->last = ngx_movemem(b->start, b->pos, b->last - b->pos);
        b->pos = b->start;
    }

    אם (c->sent != sent) {
        ngx_add_timer(wev, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    אם (ngx_handle_write_event(wev, 0) != NGX_OK) {
        اذهب failed;
    }

    החזר;

failed:

    ngx_close_connection(c);
    rec->tcp = عدم;
}


ساكن فراغ
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

    כלעוד (rev->ready) {
        n = ngx_recv(c, b->last, b->end - b->last);

        אם (n == NGX_AGAIN) {
            اكسر;
        }

        אם (n == NGX_ERROR || n == 0) {
            اذهب failed;
        }

        b->last += n;

        עבור ( ;; ) {
            p = b->pos;
            size = b->last - p;

            אם (size < 2) {
                اكسر;
            }

            qlen = (u_short) *p++ << 8;
            qlen += *p++;

            אם (size < (size_t) (2 + qlen)) {
                اكسر;
            }

            ngx_resolver_process_response(r, p, qlen, 1);

            b->pos += 2 + qlen;
        }

        אם (b->pos != b->start) {
            b->last = ngx_movemem(b->start, b->pos, b->last - b->pos);
            b->pos = b->start;
        }
    }

    אם (ngx_handle_read_event(rev, 0) != NGX_OK) {
        اذهب failed;
    }

    החזר;

failed:

    ngx_close_connection(c);
    rec->tcp = عدم;
}


ساكن فراغ
ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t tcp)
{
    حرف                 *err;
    ngx_uint_t            i, times, ident, qident, flags, code, nqs, nan, trunc,
                          qtype, qclass;
#אם (NGX_HAVE_INET6)
    ngx_uint_t            qident6;
#endif
    ngx_queue_t          *q;
    ngx_resolver_qs_t    *qs;
    ngx_resolver_hdr_t   *response;
    ngx_resolver_node_t  *rn;

    אם (n < حجم(ngx_resolver_hdr_t)) {
        اذهب short_response;
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
    אם ((flags & 0xf870) != 0x8000 || (trunc && tcp)) {
        ngx_log_error(r->log_level, r->log, 0,
                      "invalid %s DNS response %ui fl:%04Xi",
                      tcp ? "TCP" : "UDP", ident, flags);
        החזר;
    }

    code = flags & 0xf;

    אם (code == NGX_RESOLVE_FORMERR) {

        times = 0;

        עבור (q = ngx_queue_head(&r->name_resend_queue);
             q != ngx_queue_sentinel(&r->name_resend_queue) && times++ < 100;
             q = ngx_queue_next(q))
        {
            rn = ngx_queue_data(q, ngx_resolver_node_t, queue);

            אם (rn->query) {
                qident = (rn->query[0] << 8) + rn->query[1];

                אם (qident == ident) {
                    اذهب dns_error_name;
                }
            }

#אם (NGX_HAVE_INET6)
            אם (rn->query6) {
                qident6 = (rn->query6[0] << 8) + rn->query6[1];

                אם (qident6 == ident) {
                    اذهب dns_error_name;
                }
            }
#endif
        }

        اذهب dns_error;
    }

    אם (code > NGX_RESOLVE_REFUSED) {
        اذهب dns_error;
    }

    אם (nqs != 1) {
        err = "invalid number of questions in DNS response";
        اذهب done;
    }

    i = حجم(ngx_resolver_hdr_t);

    כלעוד (i < (ngx_uint_t) n) {

        אם (buf[i] & 0xc0) {
            err = "unexpected compression pointer in DNS response";
            اذهب done;
        }

        אם (buf[i] == '\0') {
            اذهب found;
        }

        i += 1 + buf[i];
    }

    اذهب short_response;

found:

    אם (i++ == حجم(ngx_resolver_hdr_t)) {
        err = "zero-length domain name in DNS response";
        اذهب done;
    }

    אם (i + حجم(ngx_resolver_qs_t) + nan * (2 + حجم(ngx_resolver_an_t))
        > (ngx_uint_t) n)
    {
        اذهب short_response;
    }

    qs = (ngx_resolver_qs_t *) &buf[i];

    qtype = (qs->type_hi << 8) + qs->type_lo;
    qclass = (qs->class_hi << 8) + qs->class_lo;

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver DNS response qt:%ui cl:%ui", qtype, qclass);

    אם (qclass != 1) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unknown query class %ui in DNS response", qclass);
        החזר;
    }

    بدل (qtype) {

    حالة NGX_RESOLVE_A:
#אם (NGX_HAVE_INET6)
    حالة NGX_RESOLVE_AAAA:
#endif

        ngx_resolver_process_a(r, buf, n, ident, code, qtype, nan, trunc,
                               i + حجم(ngx_resolver_qs_t));

        اكسر;

    حالة NGX_RESOLVE_SRV:

        ngx_resolver_process_srv(r, buf, n, ident, code, nan, trunc,
                                 i + حجم(ngx_resolver_qs_t));

        اكسر;

    حالة NGX_RESOLVE_PTR:

        ngx_resolver_process_ptr(r, buf, n, ident, code, nan);

        اكسر;

    افتراضي:
        ngx_log_error(r->log_level, r->log, 0,
                      "unknown query type %ui in DNS response", qtype);
        החזר;
    }

    החזר;

short_response:

    err = "short DNS response";

done:

    ngx_log_error(r->log_level, r->log, 0, err);

    החזר;

dns_error_name:

    ngx_log_error(r->log_level, r->log, 0,
                  "DNS error (%ui: %s), query id:%ui, name:\"%*s\"",
                  code, ngx_resolver_strerror(code), ident,
                  (size_t) rn->nlen, rn->name);
    החזר;

dns_error:

    ngx_log_error(r->log_level, r->log, 0,
                  "DNS error (%ui: %s), query id:%ui",
                  code, ngx_resolver_strerror(code), ident);
    החזר;
}


ساكن فراغ
ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans)
{
    حرف                       *err;
    u_char                     *cname;
    size_t                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    in_addr_t                  *addr;
    ngx_str_t                   name;
    ngx_uint_t                  type, class, qident, naddrs, a, i, j, start;
#אם (NGX_HAVE_INET6)
    بنية in6_addr            *addr6;
#endif
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *next;
    ngx_resolver_node_t        *rn;
    ngx_resolver_addr_t        *addrs;
    ngx_resolver_connection_t  *rec;

    אם (ngx_resolver_copy(r, &name, buf,
                          buf + حجم(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        החזר;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver qs:%V", &name);

    hash = ngx_crc32_short(name.data, name.len);

    /* lock name mutex */

    rn = ngx_resolver_lookup_name(r, &name, hash);

    אם (rn == عدم) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.data);
        اذهب failed;
    }

    بدل (qtype) {

#אם (NGX_HAVE_INET6)
    حالة NGX_RESOLVE_AAAA:

        אם (rn->query6 == عدم || rn->naddrs6 != (u_short) -1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.data);
            اذهب failed;
        }

        אם (trunc && rn->tcp6) {
            ngx_resolver_free(r, name.data);
            اذهب failed;
        }

        qident = (rn->query6[0] << 8) + rn->query6[1];

        اكسر;
#endif

    افتراضي: /* NGX_RESOLVE_A */

        אם (rn->query == عدم || rn->naddrs != (u_short) -1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.data);
            اذهب failed;
        }

        אם (trunc && rn->tcp) {
            ngx_resolver_free(r, name.data);
            اذهب failed;
        }

        qident = (rn->query[0] << 8) + rn->query[1];
    }

    אם (ident != qident) {
        ngx_log_error(r->log_level, r->log, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.data);
        اذهب failed;
    }

    ngx_resolver_free(r, name.data);

    אם (trunc) {

        ngx_queue_remove(&rn->queue);

        אם (rn->waiting == عدم) {
            ngx_rbtree_delete(&r->name_rbtree, &rn->node);
            ngx_resolver_free_node(r, rn);
            اذهب next;
        }

        rec = r->connections.elts;
        rec = &rec[rn->last_connection];

        بدل (qtype) {

#אם (NGX_HAVE_INET6)
        حالة NGX_RESOLVE_AAAA:

            rn->tcp6 = 1;

            (فراغ) ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen);

            اكسر;
#endif

        افتراضي: /* NGX_RESOLVE_A */

            rn->tcp = 1;

            (فراغ) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);
        }

        rn->expire = ngx_time() + r->resend_timeout;

        ngx_queue_insert_head(&r->name_resend_queue, &rn->queue);

        اذهب next;
    }

    אם (code == 0 && rn->code) {
        code = rn->code;
    }

    אם (code == 0 && nan == 0) {

#אם (NGX_HAVE_INET6)
        بدل (qtype) {

        حالة NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            אם (rn->naddrs == (u_short) -1) {
                اذهب next;
            }

            אם (rn->naddrs) {
                اذهب export;
            }

            اكسر;

        افتراضي: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            אם (rn->naddrs6 == (u_short) -1) {
                اذهب next;
            }

            אם (rn->naddrs6) {
                اذهب export;
            }
        }
#endif

        code = NGX_RESOLVE_NXDOMAIN;
    }

    אם (code) {

#אם (NGX_HAVE_INET6)
        بدل (qtype) {

        حالة NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            אם (rn->naddrs == (u_short) -1) {
                rn->code = (u_char) code;
                اذهب next;
            }

            اكسر;

        افتراضي: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            אם (rn->naddrs6 == (u_short) -1) {
                rn->code = (u_char) code;
                اذهب next;
            }
        }
#endif

        next = rn->waiting;
        rn->waiting = عدم;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(&r->name_rbtree, &rn->node);

        /* unlock name mutex */

        כלעוד (next) {
            ctx = next;
            ctx->state = code;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            next = ctx->next;

            ctx->handler(ctx);
        }

        ngx_resolver_free_node(r, rn);

        החזר;
    }

    i = ans;
    naddrs = 0;
    cname = عدم;

    עבור (a = 0; a < nan; a++) {

        start = i;

        כלעוד (i < n) {

            אם (buf[i] & 0xc0) {
                i += 2;
                اذهب found;
            }

            אם (buf[i] == 0) {
                i++;
                اذهب test_length;
            }

            i += 1 + buf[i];
        }

        اذهب short_response;

    test_length:

        אם (i - start < 2) {
            err = "invalid name in DNS response";
            اذهب invalid;
        }

    found:

        אם (i + حجم(ngx_resolver_an_t) >= n) {
            اذهب short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        אם (class != 1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR class %ui in DNS response", class);
            اذهب failed;
        }

        אם (ttl < 0) {
            ttl = 0;
        }

        rn->ttl = ngx_min(rn->ttl, (uint32_t) ttl);

        i += حجم(ngx_resolver_an_t);

        بدل (type) {

        حالة NGX_RESOLVE_A:

            אם (qtype != NGX_RESOLVE_A) {
                err = "unexpected A record in DNS response";
                اذهب invalid;
            }

            אם (len != 4) {
                err = "invalid A record in DNS response";
                اذهب invalid;
            }

            אם (i + 4 > n) {
                اذهب short_response;
            }

            naddrs++;

            اكسر;

#אם (NGX_HAVE_INET6)
        حالة NGX_RESOLVE_AAAA:

            אם (qtype != NGX_RESOLVE_AAAA) {
                err = "unexpected AAAA record in DNS response";
                اذهب invalid;
            }

            אם (len != 16) {
                err = "invalid AAAA record in DNS response";
                اذهب invalid;
            }

            אם (i + 16 > n) {
                اذهب short_response;
            }

            naddrs++;

            اكسر;
#endif

        حالة NGX_RESOLVE_CNAME:

            cname = &buf[i];

            اكسر;

        حالة NGX_RESOLVE_DNAME:

            اكسر;

        افتراضي:

            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver naddrs:%ui cname:%p ttl:%uD",
                   naddrs, cname, rn->ttl);

    אם (naddrs) {

        بدل (qtype) {

#אם (NGX_HAVE_INET6)
        حالة NGX_RESOLVE_AAAA:

            אם (naddrs == 1) {
                addr6 = &rn->u6.addr6;
                rn->naddrs6 = 1;

            } אחרת {
                addr6 = ngx_resolver_alloc(r, naddrs * حجم(بنية in6_addr));
                אם (addr6 == عدم) {
                    اذهب failed;
                }

                rn->u6.addrs6 = addr6;
                rn->naddrs6 = (u_short) naddrs;
            }

#אם (NGX_SUPPRESS_WARN)
            addr = عدم;
#endif

            اكسر;
#endif

        افتراضي: /* NGX_RESOLVE_A */

            אם (naddrs == 1) {
                addr = &rn->u.addr;
                rn->naddrs = 1;

            } אחרת {
                addr = ngx_resolver_alloc(r, naddrs * حجم(in_addr_t));
                אם (addr == عدم) {
                    اذهب failed;
                }

                rn->u.addrs = addr;
                rn->naddrs = (u_short) naddrs;
            }

#אם (NGX_HAVE_INET6 && NGX_SUPPRESS_WARN)
            addr6 = عدم;
#endif
        }

        j = 0;
        i = ans;

        עבור (a = 0; a < nan; a++) {

            עבור ( ;; ) {

                אם (buf[i] & 0xc0) {
                    i += 2;
                    اكسر;
                }

                אם (buf[i] == 0) {
                    i++;
                    اكسر;
                }

                i += 1 + buf[i];
            }

            an = (ngx_resolver_an_t *) &buf[i];

            type = (an->type_hi << 8) + an->type_lo;
            len = (an->len_hi << 8) + an->len_lo;

            i += حجم(ngx_resolver_an_t);

            אם (type == NGX_RESOLVE_A) {

                addr[j] = htonl((buf[i] << 24) + (buf[i + 1] << 16)
                                + (buf[i + 2] << 8) + (buf[i + 3]));

                אם (++j == naddrs) {

#אם (NGX_HAVE_INET6)
                    אם (rn->naddrs6 == (u_short) -1) {
                        اذهب next;
                    }
#endif

                    اكسر;
                }
            }

#אם (NGX_HAVE_INET6)
            אחרת אם (type == NGX_RESOLVE_AAAA) {

                ngx_memcpy(addr6[j].s6_addr, &buf[i], 16);

                אם (++j == naddrs) {

                    אם (rn->naddrs == (u_short) -1) {
                        اذهب next;
                    }

                    اكسر;
                }
            }
#endif

            i += len;
        }
    }

    بدل (qtype) {

#אם (NGX_HAVE_INET6)
    حالة NGX_RESOLVE_AAAA:

        אם (rn->naddrs6 == (u_short) -1) {
            rn->naddrs6 = 0;
        }

        اكسر;
#endif

    افتراضي: /* NGX_RESOLVE_A */

        אם (rn->naddrs == (u_short) -1) {
            rn->naddrs = 0;
        }
    }

    אם (rn->naddrs != (u_short) -1
#אם (NGX_HAVE_INET6)
        && rn->naddrs6 != (u_short) -1
#endif
        && rn->naddrs
#אם (NGX_HAVE_INET6)
           + rn->naddrs6
#endif
           > 0)
    {

#אם (NGX_HAVE_INET6)
    export:
#endif

        naddrs = rn->naddrs;
#אם (NGX_HAVE_INET6)
        naddrs += rn->naddrs6;
#endif

        אם (naddrs == 1 && rn->naddrs == 1) {
            addrs = عدم;

        } אחרת {
            addrs = ngx_resolver_export(r, rn, 0);
            אם (addrs == عدم) {
                اذهب failed;
            }
        }

        ngx_queue_remove(&rn->queue);

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_insert_head(&r->name_expire_queue, &rn->queue);

        next = rn->waiting;
        rn->waiting = عدم;

        /* unlock name mutex */

        כלעוד (next) {
            ctx = next;
            ctx->state = NGX_OK;
            ctx->valid = rn->valid;
            ctx->naddrs = naddrs;

            אם (addrs == عدم) {
                ctx->addrs = &ctx->addr;
                ctx->addr.sockaddr = (بنية sockaddr *) &ctx->sin;
                ctx->addr.socklen = حجم(بنية sockaddr_in);
                ngx_memzero(&ctx->sin, حجم(بنية sockaddr_in));
                ctx->sin.sin_family = AF_INET;
                ctx->sin.sin_addr.s_addr = rn->u.addr;

            } אחרת {
                ctx->addrs = addrs;
            }

            next = ctx->next;

            ctx->handler(ctx);
        }

        אם (addrs != عدم) {
            ngx_resolver_free(r, addrs->sockaddr);
            ngx_resolver_free(r, addrs);
        }

        ngx_resolver_free(r, rn->query);
        rn->query = عدم;
#אם (NGX_HAVE_INET6)
        rn->query6 = عدم;
#endif

        החזר;
    }

    אם (cname) {

        /* CNAME only */

        אם (rn->naddrs == (u_short) -1
#אם (NGX_HAVE_INET6)
            || rn->naddrs6 == (u_short) -1
#endif
            )
        {
            اذهب next;
        }

        אם (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            اذهب failed;
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
        rn->query = عدم;
#אם (NGX_HAVE_INET6)
        rn->query6 = عدم;
#endif

        ctx = rn->waiting;
        rn->waiting = عدم;

        אם (ctx) {

            אם (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                עשה {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    next = ctx->next;

                    ctx->handler(ctx);

                    ctx = next;
                } כלעוד (ctx);

                החזר;
            }

            עבור (next = ctx; next; next = next->next) {
                next->node = عدم;
            }

            (فراغ) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        החזר;
    }

    ngx_log_error(r->log_level, r->log, 0,
                  "no A or CNAME types in DNS response");
    החזר;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->log, 0, err);

    החזר;

failed:

next:

    /* unlock name mutex */

    החזר;
}


ساكن فراغ
ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans)
{
    حرف                       *err;
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

    אם (ngx_resolver_copy(r, &name, buf,
                          buf + حجم(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        החזר;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver qs:%V", &name);

    hash = ngx_crc32_short(name.data, name.len);

    rn = ngx_resolver_lookup_srv(r, &name, hash);

    אם (rn == عدم || rn->query == عدم) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.data);
        اذهب failed;
    }

    אם (trunc && rn->tcp) {
        ngx_resolver_free(r, name.data);
        اذهب failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    אם (ident != qident) {
        ngx_log_error(r->log_level, r->log, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.data);
        اذهب failed;
    }

    ngx_resolver_free(r, name.data);

    אם (trunc) {

        ngx_queue_remove(&rn->queue);

        אם (rn->waiting == عدم) {
            ngx_rbtree_delete(&r->srv_rbtree, &rn->node);
            ngx_resolver_free_node(r, rn);
            החזר;
        }

        rec = r->connections.elts;
        rec = &rec[rn->last_connection];

        rn->tcp = 1;

        (فراغ) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);

        rn->expire = ngx_time() + r->resend_timeout;

        ngx_queue_insert_head(&r->srv_resend_queue, &rn->queue);

        החזר;
    }

    אם (code == 0 && rn->code) {
        code = rn->code;
    }

    אם (code == 0 && nan == 0) {
        code = NGX_RESOLVE_NXDOMAIN;
    }

    אם (code) {
        next = rn->waiting;
        rn->waiting = عدم;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(&r->srv_rbtree, &rn->node);

        כלעוד (next) {
            ctx = next;
            ctx->state = code;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            next = ctx->next;

            ctx->handler(ctx);
        }

        ngx_resolver_free_node(r, rn);

        החזר;
    }

    i = ans;
    nsrvs = 0;
    cname = عدم;

    עבור (a = 0; a < nan; a++) {

        start = i;

        כלעוד (i < n) {

            אם (buf[i] & 0xc0) {
                i += 2;
                اذهب found;
            }

            אם (buf[i] == 0) {
                i++;
                اذهب test_length;
            }

            i += 1 + buf[i];
        }

        اذهب short_response;

    test_length:

        אם (i - start < 2) {
            err = "invalid name DNS response";
            اذهب invalid;
        }

    found:

        אם (i + حجم(ngx_resolver_an_t) >= n) {
            اذهب short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        אם (class != 1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR class %ui in DNS response", class);
            اذهب failed;
        }

        אם (ttl < 0) {
            ttl = 0;
        }

        rn->ttl = ngx_min(rn->ttl, (uint32_t) ttl);

        i += حجم(ngx_resolver_an_t);

        بدل (type) {

        حالة NGX_RESOLVE_SRV:

            אם (i + 6 >= n) {
                اذهب short_response;
            }

            אם (ngx_resolver_copy(r, عدم, buf, &buf[i + 6], buf + n)
                != NGX_OK)
            {
                اذهب failed;
            }

            nsrvs++;

            اكسر;

        حالة NGX_RESOLVE_CNAME:

            cname = &buf[i];

            اكسر;

        حالة NGX_RESOLVE_DNAME:

            اكسر;

        افتراضي:

            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver nsrvs:%ui cname:%p ttl:%uD",
                   nsrvs, cname, rn->ttl);

    אם (nsrvs) {

        srvs = ngx_resolver_calloc(r, nsrvs * حجم(ngx_resolver_srv_t));
        אם (srvs == عدم) {
            اذهب failed;
        }

        rn->u.srvs = srvs;
        rn->nsrvs = (u_short) nsrvs;

        j = 0;
        i = ans;

        עבור (a = 0; a < nan; a++) {

            עבור ( ;; ) {

                אם (buf[i] & 0xc0) {
                    i += 2;
                    اكسر;
                }

                אם (buf[i] == 0) {
                    i++;
                    اكسر;
                }

                i += 1 + buf[i];
            }

            an = (ngx_resolver_an_t *) &buf[i];

            type = (an->type_hi << 8) + an->type_lo;
            len = (an->len_hi << 8) + an->len_lo;

            i += حجم(ngx_resolver_an_t);

            אם (type == NGX_RESOLVE_SRV) {

                srvs[j].priority = (buf[i] << 8) + buf[i + 1];
                srvs[j].weight = (buf[i + 2] << 8) + buf[i + 3];

                אם (srvs[j].weight == 0) {
                    srvs[j].weight = 1;
                }

                srvs[j].port = (buf[i + 4] << 8) + buf[i + 5];

                אם (ngx_resolver_copy(r, &srvs[j].name, buf, &buf[i + 6],
                                      buf + n)
                    != NGX_OK)
                {
                    اذهب failed;
                }

                j++;
            }

            i += len;
        }

        ngx_sort(srvs, nsrvs, حجم(ngx_resolver_srv_t),
                 ngx_resolver_cmp_srvs);

        ngx_resolver_free(r, rn->query);
        rn->query = عدم;

        ngx_queue_remove(&rn->queue);

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_insert_head(&r->srv_expire_queue, &rn->queue);

        next = rn->waiting;
        rn->waiting = عدم;

        כלעוד (next) {
            ctx = next;
            next = ctx->next;

            ngx_resolver_resolve_srv_names(ctx, rn);
        }

        החזר;
    }

    rn->nsrvs = 0;

    אם (cname) {

        /* CNAME only */

        אם (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            اذهب failed;
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
        rn->query = عدم;
#אם (NGX_HAVE_INET6)
        rn->query6 = عدم;
#endif

        ctx = rn->waiting;
        rn->waiting = عدم;

        אם (ctx) {

            אם (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                עשה {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    next = ctx->next;

                    ctx->handler(ctx);

                    ctx = next;
                } כלעוד (ctx);

                החזר;
            }

            עבור (next = ctx; next; next = next->next) {
                next->node = عدم;
            }

            (فراغ) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        החזר;
    }

    ngx_log_error(r->log_level, r->log, 0, "no SRV type in DNS response");

    החזר;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->log, 0, err);

    החזר;

failed:

    /* unlock name mutex */

    החזר;
}


ساكن فراغ
ngx_resolver_resolve_srv_names(ngx_resolver_ctx_t *ctx, ngx_resolver_node_t *rn)
{
    ngx_uint_t                i;
    ngx_resolver_t           *r;
    ngx_resolver_ctx_t       *cctx;
    ngx_resolver_srv_name_t  *srvs;

    r = ctx->resolver;

    ctx->node = عدم;
    ctx->state = NGX_OK;
    ctx->valid = rn->valid;
    ctx->count = rn->nsrvs;

    srvs = ngx_resolver_calloc(r, rn->nsrvs * حجم(ngx_resolver_srv_name_t));
    אם (srvs == عدم) {
        اذهب failed;
    }

    ctx->srvs = srvs;
    ctx->nsrvs = rn->nsrvs;

    אם (ctx->event && ctx->event->timer_set) {
        ngx_del_timer(ctx->event);
    }

    עבור (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
        srvs[i].name.data = ngx_resolver_alloc(r, rn->u.srvs[i].name.len);
        אם (srvs[i].name.data == عدم) {
            اذهب failed;
        }

        srvs[i].name.len = rn->u.srvs[i].name.len;
        ngx_memcpy(srvs[i].name.data, rn->u.srvs[i].name.data,
                   srvs[i].name.len);

        cctx = ngx_resolve_start(r, عدم);
        אם (cctx == عدم) {
            اذهب failed;
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

        אם (ngx_resolve_name(cctx) == NGX_ERROR) {
            srvs[i].ctx = عدم;
            اذهب failed;
        }
    }

    החזר;

failed:

    ctx->state = NGX_ERROR;
    ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

    ctx->handler(ctx);
}


ساكن فراغ
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

    srv->ctx = عدم;
    srv->state = cctx->state;

    אם (cctx->naddrs) {

        ctx->valid = ngx_min(ctx->valid, cctx->valid);

        addrs = ngx_resolver_calloc(r, cctx->naddrs * حجم(ngx_addr_t));
        אם (addrs == عدم) {
            srv->state = NGX_ERROR;
            اذهب done;
        }

        sockaddr = ngx_resolver_alloc(r, cctx->naddrs * حجم(ngx_sockaddr_t));
        אם (sockaddr == عدم) {
            ngx_resolver_free(r, addrs);
            srv->state = NGX_ERROR;
            اذهب done;
        }

        עבור (i = 0; i < cctx->naddrs; i++) {
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

    אם (ctx->count == 0) {
        ngx_resolver_report_srv(r, ctx);
    }
}


ساكن فراغ
ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan)
{
    حرف                 *err;
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
#אם (NGX_HAVE_INET6)
    uint32_t              hash;
    ngx_int_t             digit;
    بنية in6_addr       addr6;
#endif

    אם (ngx_resolver_copy(r, &name, buf,
                          buf + حجم(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        החזר;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver qs:%V", &name);

    /* AF_INET */

    addr = 0;
    i = حجم(ngx_resolver_hdr_t);

    עבור (mask = 0; mask < 32; mask += 8) {
        len = buf[i++];

        octet = ngx_atoi(&buf[i], len);
        אם (octet == NGX_ERROR || octet > 255) {
            اذهب invalid_in_addr_arpa;
        }

        addr += octet << mask;
        i += len;
    }

    אם (ngx_strcasecmp(&buf[i], (u_char *) "\7in-addr\4arpa") == 0) {
        i += حجم("\7in-addr\4arpa");

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        tree = &r->addr_rbtree;
        expire_queue = &r->addr_expire_queue;

        اذهب valid;
    }

invalid_in_addr_arpa:

#אם (NGX_HAVE_INET6)

    i = حجم(ngx_resolver_hdr_t);

    עבור (octet = 15; octet >= 0; octet--) {
        אם (buf[i++] != '\1') {
            اذهب invalid_ip6_arpa;
        }

        digit = ngx_hextoi(&buf[i++], 1);
        אם (digit == NGX_ERROR) {
            اذهب invalid_ip6_arpa;
        }

        addr6.s6_addr[octet] = (u_char) digit;

        אם (buf[i++] != '\1') {
            اذهب invalid_ip6_arpa;
        }

        digit = ngx_hextoi(&buf[i++], 1);
        אם (digit == NGX_ERROR) {
            اذهب invalid_ip6_arpa;
        }

        addr6.s6_addr[octet] += (u_char) (digit * 16);
    }

    אם (ngx_strcasecmp(&buf[i], (u_char *) "\3ip6\4arpa") == 0) {
        i += حجم("\3ip6\4arpa");

        /* lock addr mutex */

        hash = ngx_crc32_short(addr6.s6_addr, 16);
        rn = ngx_resolver_lookup_addr6(r, &addr6, hash);

        tree = &r->addr6_rbtree;
        expire_queue = &r->addr6_expire_queue;

        اذهب valid;
    }

invalid_ip6_arpa:
#endif

    ngx_log_error(r->log_level, r->log, 0,
                  "invalid in-addr.arpa or ip6.arpa name in DNS response");
    ngx_resolver_free(r, name.data);
    החזר;

valid:

    אם (rn == عدم || rn->query == عدم) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.data);
        اذهب failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    אם (ident != qident) {
        ngx_log_error(r->log_level, r->log, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.data);
        اذهب failed;
    }

    ngx_resolver_free(r, name.data);

    אם (code == 0 && nan == 0) {
        code = NGX_RESOLVE_NXDOMAIN;
    }

    אם (code) {
        next = rn->waiting;
        rn->waiting = عدم;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(tree, &rn->node);

        /* unlock addr mutex */

        כלעוד (next) {
            ctx = next;
            ctx->state = code;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            next = ctx->next;

            ctx->handler(ctx);
        }

        ngx_resolver_free_node(r, rn);

        החזר;
    }

    i += حجم(ngx_resolver_qs_t);

    עבור (a = 0; a < nan; a++) {

        start = i;

        כלעוד (i < n) {

            אם (buf[i] & 0xc0) {
                i += 2;
                اذهب found;
            }

            אם (buf[i] == 0) {
                i++;
                اذهب test_length;
            }

            i += 1 + buf[i];
        }

        اذهب short_response;

    test_length:

        אם (i - start < 2) {
            err = "invalid name in DNS response";
            اذهب invalid;
        }

    found:

        אם (i + حجم(ngx_resolver_an_t) >= n) {
            اذهب short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        אם (class != 1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR class %ui in DNS response", class);
            اذهب failed;
        }

        אם (ttl < 0) {
            ttl = 0;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                      "resolver qt:%ui cl:%ui len:%uz",
                      type, class, len);

        i += حجم(ngx_resolver_an_t);

        بدل (type) {

        حالة NGX_RESOLVE_PTR:

            اذهب ptr;

        حالة NGX_RESOLVE_CNAME:

            اكسر;

        افتراضي:

            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->log, 0,
                  "no PTR type in DNS response");
    החזר;

ptr:

    אם (ngx_resolver_copy(r, &name, buf, buf + i, buf + n) != NGX_OK) {
        اذهب failed;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver an:%V", &name);

    אם (name.len != (size_t) rn->nlen
        || ngx_strncmp(name.data, rn->name, name.len) != 0)
    {
        אם (rn->nlen) {
            ngx_resolver_free(r, rn->name);
        }

        rn->nlen = (u_short) name.len;
        rn->name = name.data;

        name.data = ngx_resolver_dup(r, rn->name, name.len);
        אם (name.data == عدم) {
            اذهب failed;
        }
    }

    ngx_queue_remove(&rn->queue);

    rn->valid = ngx_time() + (r->valid ? r->valid : ttl);
    rn->expire = ngx_time() + r->expire;

    ngx_queue_insert_head(expire_queue, &rn->queue);

    next = rn->waiting;
    rn->waiting = عدم;

    /* unlock addr mutex */

    כלעוד (next) {
        ctx = next;
        ctx->state = NGX_OK;
        ctx->valid = rn->valid;
        ctx->name = name;
        next = ctx->next;

        ctx->handler(ctx);
    }

    ngx_resolver_free(r, name.data);

    החזר;

short_response:

    err = "short DNS response";

invalid:

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->log, 0, err);

    החזר;

failed:

    /* unlock addr mutex */

    החזר;
}


ساكن ngx_resolver_node_t *
ngx_resolver_lookup_name(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_node_t    *node, *sentinel;
    ngx_resolver_node_t  *rn;

    node = r->name_rbtree.root;
    sentinel = r->name_rbtree.sentinel;

    כלעוד (node != sentinel) {

        אם (hash < node->key) {
            node = node->left;
            استمر;
        }

        אם (hash > node->key) {
            node = node->right;
            استمر;
        }

        /* hash == node->key */

        rn = ngx_resolver_node(node);

        rc = ngx_memn2cmp(name->data, rn->name, name->len, rn->nlen);

        אם (rc == 0) {
            החזר rn;
        }

        node = (rc < 0) ? node->left : node->right;
    }

    /* not found */

    החזר عدم;
}


ساكن ngx_resolver_node_t *
ngx_resolver_lookup_srv(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_node_t    *node, *sentinel;
    ngx_resolver_node_t  *rn;

    node = r->srv_rbtree.root;
    sentinel = r->srv_rbtree.sentinel;

    כלעוד (node != sentinel) {

        אם (hash < node->key) {
            node = node->left;
            استمر;
        }

        אם (hash > node->key) {
            node = node->right;
            استمر;
        }

        /* hash == node->key */

        rn = ngx_resolver_node(node);

        rc = ngx_memn2cmp(name->data, rn->name, name->len, rn->nlen);

        אם (rc == 0) {
            החזר rn;
        }

        node = (rc < 0) ? node->left : node->right;
    }

    /* not found */

    החזר عدم;
}


ساكن ngx_resolver_node_t *
ngx_resolver_lookup_addr(ngx_resolver_t *r, in_addr_t addr)
{
    ngx_rbtree_node_t  *node, *sentinel;

    node = r->addr_rbtree.root;
    sentinel = r->addr_rbtree.sentinel;

    כלעוד (node != sentinel) {

        אם (addr < node->key) {
            node = node->left;
            استمر;
        }

        אם (addr > node->key) {
            node = node->right;
            استمر;
        }

        /* addr == node->key */

        החזר ngx_resolver_node(node);
    }

    /* not found */

    החזר عدم;
}


#אם (NGX_HAVE_INET6)

ساكن ngx_resolver_node_t *
ngx_resolver_lookup_addr6(ngx_resolver_t *r, بنية in6_addr *addr,
    uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_node_t    *node, *sentinel;
    ngx_resolver_node_t  *rn;

    node = r->addr6_rbtree.root;
    sentinel = r->addr6_rbtree.sentinel;

    כלעוד (node != sentinel) {

        אם (hash < node->key) {
            node = node->left;
            استمر;
        }

        אם (hash > node->key) {
            node = node->right;
            استمر;
        }

        /* hash == node->key */

        rn = ngx_resolver_node(node);

        rc = ngx_memcmp(addr, &rn->addr6, 16);

        אם (rc == 0) {
            החזר rn;
        }

        node = (rc < 0) ? node->left : node->right;
    }

    /* not found */

    החזר عدم;
}

#endif


ساكن فراغ
ngx_resolver_rbtree_insert_value(ngx_rbtree_node_t *temp,
    ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel)
{
    ngx_rbtree_node_t    **p;
    ngx_resolver_node_t   *rn, *rn_temp;

    עבור ( ;; ) {

        אם (node->key < temp->key) {

            p = &temp->left;

        } אחרת אם (node->key > temp->key) {

            p = &temp->right;

        } אחרת { /* node->key == temp->key */

            rn = ngx_resolver_node(node);
            rn_temp = ngx_resolver_node(temp);

            p = (ngx_memn2cmp(rn->name, rn_temp->name, rn->nlen, rn_temp->nlen)
                 < 0) ? &temp->left : &temp->right;
        }

        אם (*p == sentinel) {
            اكسر;
        }

        temp = *p;
    }

    *p = node;
    node->parent = temp;
    node->left = sentinel;
    node->right = sentinel;
    ngx_rbt_red(node);
}


#אם (NGX_HAVE_INET6)

ساكن فراغ
ngx_resolver_rbtree_insert_addr6_value(ngx_rbtree_node_t *temp,
    ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel)
{
    ngx_rbtree_node_t    **p;
    ngx_resolver_node_t   *rn, *rn_temp;

    עבור ( ;; ) {

        אם (node->key < temp->key) {

            p = &temp->left;

        } אחרת אם (node->key > temp->key) {

            p = &temp->right;

        } אחרת { /* node->key == temp->key */

            rn = ngx_resolver_node(node);
            rn_temp = ngx_resolver_node(temp);

            p = (ngx_memcmp(&rn->addr6, &rn_temp->addr6, 16)
                 < 0) ? &temp->left : &temp->right;
        }

        אם (*p == sentinel) {
            اكسر;
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


ساكن ngx_int_t
ngx_resolver_create_name_query(ngx_resolver_t *r, ngx_resolver_node_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    size_t               len, nlen;
    ngx_uint_t           ident;
    ngx_resolver_qs_t   *qs;
    ngx_resolver_hdr_t  *query;

    nlen = name->len ? (1 + name->len + 1) : 1;

    len = حجم(ngx_resolver_hdr_t) + nlen + حجم(ngx_resolver_qs_t);

#אם (NGX_HAVE_INET6)
    p = ngx_resolver_alloc(r, len * (r->ipv4 + r->ipv6));
#אחרת
    p = ngx_resolver_alloc(r, len);
#endif
    אם (p == عدم) {
        החזר NGX_ERROR;
    }

    rn->qlen = (u_short) len;
    rn->query = p;

#אם (NGX_HAVE_INET6)
    אם (r->ipv6) {
        rn->query6 = r->ipv4 ? (p + len) : p;
    }
#endif

    query = (ngx_resolver_hdr_t *) p;

    אם (r->ipv4) {
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

    p += حجم(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    /* query type */
    qs->type_hi = 0; qs->type_lo = NGX_RESOLVE_A;

    /* IN query class */
    qs->class_hi = 0; qs->class_lo = 1;

    /* convert "www.example.com" to "\3www\7example\3com\0" */

    len = 0;
    p--;
    *p-- = '\0';

    אם (name->len == 0)  {
        החזר NGX_DECLINED;
    }

    עבור (s = name->data + name->len - 1; s >= name->data; s--) {
        אם (*s != '.') {
            *p = *s;
            len++;

        } אחרת {
            אם (len == 0 || len > 255) {
                החזר NGX_DECLINED;
            }

            *p = (u_char) len;
            len = 0;
        }

        p--;
    }

    אם (len == 0 || len > 255) {
        החזר NGX_DECLINED;
    }

    *p = (u_char) len;

#אם (NGX_HAVE_INET6)
    אם (!r->ipv6) {
        החזר NGX_OK;
    }

    p = rn->query6;

    אם (r->ipv4) {
        ngx_memcpy(p, rn->query, rn->qlen);
    }

    query = (ngx_resolver_hdr_t *) p;

    ident = ngx_random();

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve: \"%V\" AAAA %i", name, ident & 0xffff);

    query->ident_hi = (u_char) ((ident >> 8) & 0xff);
    query->ident_lo = (u_char) (ident & 0xff);

    p += حجم(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    qs->type_lo = NGX_RESOLVE_AAAA;
#endif

    החזר NGX_OK;
}


ساكن ngx_int_t
ngx_resolver_create_srv_query(ngx_resolver_t *r, ngx_resolver_node_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    size_t               len, nlen;
    ngx_uint_t           ident;
    ngx_resolver_qs_t   *qs;
    ngx_resolver_hdr_t  *query;

    nlen = name->len ? (1 + name->len + 1) : 1;

    len = حجم(ngx_resolver_hdr_t) + nlen + حجم(ngx_resolver_qs_t);

    p = ngx_resolver_alloc(r, len);
    אם (p == عدم) {
        החזר NGX_ERROR;
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

    p += حجم(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    /* query type */
    qs->type_hi = 0; qs->type_lo = NGX_RESOLVE_SRV;

    /* IN query class */
    qs->class_hi = 0; qs->class_lo = 1;

    /* converts "www.example.com" to "\3www\7example\3com\0" */

    len = 0;
    p--;
    *p-- = '\0';

    אם (name->len == 0)  {
        החזר NGX_DECLINED;
    }

    עבור (s = name->data + name->len - 1; s >= name->data; s--) {
        אם (*s != '.') {
            *p = *s;
            len++;

        } אחרת {
            אם (len == 0 || len > 255) {
                החזר NGX_DECLINED;
            }

            *p = (u_char) len;
            len = 0;
        }

        p--;
    }

    אם (len == 0 || len > 255) {
        החזר NGX_DECLINED;
    }

    *p = (u_char) len;

    החזר NGX_OK;
}


ساكن ngx_int_t
ngx_resolver_create_addr_query(ngx_resolver_t *r, ngx_resolver_node_t *rn,
    ngx_resolver_addr_t *addr)
{
    u_char               *p, *d;
    size_t                len;
    in_addr_t             inaddr;
    ngx_int_t             n;
    ngx_uint_t            ident;
    ngx_resolver_hdr_t   *query;
    بنية sockaddr_in   *sin;
#אם (NGX_HAVE_INET6)
    بنية sockaddr_in6  *sin6;
#endif

    بدل (addr->sockaddr->sa_family) {

#אם (NGX_HAVE_INET6)
    حالة AF_INET6:
        len = حجم(ngx_resolver_hdr_t)
              + 64 + حجم(".ip6.arpa.") - 1
              + حجم(ngx_resolver_qs_t);

        اكسر;
#endif

    افتراضي: /* AF_INET */
        len = حجم(ngx_resolver_hdr_t)
              + حجم(".255.255.255.255.in-addr.arpa.") - 1
              + حجم(ngx_resolver_qs_t);
    }

    p = ngx_resolver_alloc(r, len);
    אם (p == عدم) {
        החזר NGX_ERROR;
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

    p += حجم(ngx_resolver_hdr_t);

    بدل (addr->sockaddr->sa_family) {

#אם (NGX_HAVE_INET6)
    حالة AF_INET6:
        sin6 = (بنية sockaddr_in6 *) addr->sockaddr;

        עבור (n = 15; n >= 0; n--) {
            p = ngx_sprintf(p, "\1%xd\1%xd",
                            sin6->sin6_addr.s6_addr[n] & 0xf,
                            (sin6->sin6_addr.s6_addr[n] >> 4) & 0xf);
        }

        p = ngx_cpymem(p, "\3ip6\4arpa\0", 10);

        اكسر;
#endif

    افتراضي: /* AF_INET */

        sin = (بنية sockaddr_in *) addr->sockaddr;
        inaddr = ntohl(sin->sin_addr.s_addr);

        עבור (n = 0; n < 32; n += 8) {
            d = ngx_sprintf(&p[1], "%ud", (inaddr >> n) & 0xff);
            *p = (u_char) (d - &p[1]);
            p = d;
        }

        p = ngx_cpymem(p, "\7in-addr\4arpa\0", 14);
    }

    /* query type "PTR", IN query class */
    p = ngx_cpymem(p, "\0\14\0\1", 4);

    rn->qlen = (u_short) (p - rn->query);

    החזר NGX_OK;
}


ساكن ngx_int_t
ngx_resolver_copy(ngx_resolver_t *r, ngx_str_t *name, u_char *buf, u_char *src,
    u_char *last)
{
    حرف        *err;
    u_char      *p, *dst;
    size_t       len;
    ngx_uint_t   i, n;

    p = src;
    len = 0;

    /*
     * compression pointers allow to create endless loop, so we set limit;
     * 128 pointers should be enough to store 255-byte name
     */

    עבור (i = 0; i < 128; i++) {
        n = *p++;

        אם (n == 0) {
            اذهب done;
        }

        אם (n & 0xc0) {
            אם ((n & 0xc0) != 0xc0) {
                err = "invalid label type in DNS response";
                اذهب invalid;
            }

            אם (p >= last) {
                err = "name is out of DNS response";
                اذهب invalid;
            }

            n = ((n & 0x3f) << 8) + *p;
            p = &buf[n];

        } אחרת {
            len += 1 + n;
            p = &p[n];
        }

        אם (p >= last) {
            err = "name is out of DNS response";
            اذهب invalid;
        }
    }

    err = "compression pointers loop in DNS response";

invalid:

    ngx_log_error(r->log_level, r->log, 0, err);

    החזר NGX_ERROR;

done:

    אם (name == عدم) {
        החזר NGX_OK;
    }

    אם (len == 0) {
        ngx_str_null(name);
        החזר NGX_OK;
    }

    dst = ngx_resolver_alloc(r, len);
    אם (dst == عدم) {
        החזר NGX_ERROR;
    }

    name->data = dst;

    עבור ( ;; ) {
        n = *src++;

        אם (n == 0) {
            name->len = dst - name->data - 1;
            החזר NGX_OK;
        }

        אם (n & 0xc0) {
            n = ((n & 0x3f) << 8) + *src;
            src = &buf[n];

        } אחרת {
            ngx_strlow(dst, src, n);
            dst += n;
            src += n;
            *dst++ = '.';
        }
    }
}


ساكن ngx_int_t
ngx_resolver_set_timeout(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx)
{
    אם (ctx->event || ctx->timeout == 0) {
        החזר NGX_OK;
    }

    ctx->event = ngx_resolver_calloc(r, حجم(ngx_event_t));
    אם (ctx->event == عدم) {
        החזר NGX_ERROR;
    }

    ctx->event->handler = ngx_resolver_timeout_handler;
    ctx->event->data = ctx;
    ctx->event->log = r->log;
    ctx->event->cancelable = ctx->cancelable;
    ctx->ident = -1;

    ngx_add_timer(ctx->event, ctx->timeout);

    החזר NGX_OK;
}


ساكن فراغ
ngx_resolver_timeout_handler(ngx_event_t *ev)
{
    ngx_resolver_ctx_t  *ctx;

    ctx = ev->data;

    ctx->state = NGX_RESOLVE_TIMEDOUT;

    ctx->handler(ctx);
}


ساكن فراغ
ngx_resolver_free_node(ngx_resolver_t *r, ngx_resolver_node_t *rn)
{
    ngx_uint_t  i;

    /* lock alloc mutex */

    אם (rn->query) {
        ngx_resolver_free_locked(r, rn->query);
    }

    אם (rn->name) {
        ngx_resolver_free_locked(r, rn->name);
    }

    אם (rn->cnlen) {
        ngx_resolver_free_locked(r, rn->u.cname);
    }

    אם (rn->naddrs > 1 && rn->naddrs != (u_short) -1) {
        ngx_resolver_free_locked(r, rn->u.addrs);
    }

#אם (NGX_HAVE_INET6)
    אם (rn->naddrs6 > 1 && rn->naddrs6 != (u_short) -1) {
        ngx_resolver_free_locked(r, rn->u6.addrs6);
    }
#endif

    אם (rn->nsrvs) {
        עבור (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
            אם (rn->u.srvs[i].name.data) {
                ngx_resolver_free_locked(r, rn->u.srvs[i].name.data);
            }
        }

        ngx_resolver_free_locked(r, rn->u.srvs);
    }

    ngx_resolver_free_locked(r, rn);

    /* unlock alloc mutex */
}


ساكن فراغ *
ngx_resolver_alloc(ngx_resolver_t *r, size_t size)
{
    u_char  *p;

    /* lock alloc mutex */

    p = ngx_alloc(size, r->log);

    /* unlock alloc mutex */

    החזר p;
}


ساكن فراغ *
ngx_resolver_calloc(ngx_resolver_t *r, size_t size)
{
    u_char  *p;

    p = ngx_resolver_alloc(r, size);

    אם (p) {
        ngx_memzero(p, size);
    }

    החזר p;
}


ساكن فراغ
ngx_resolver_free(ngx_resolver_t *r, فراغ *p)
{
    /* lock alloc mutex */

    ngx_free(p);

    /* unlock alloc mutex */
}


ساكن فراغ
ngx_resolver_free_locked(ngx_resolver_t *r, فراغ *p)
{
    ngx_free(p);
}


ساكن فراغ *
ngx_resolver_dup(ngx_resolver_t *r, فراغ *src, size_t size)
{
    فراغ  *dst;

    dst = ngx_resolver_alloc(r, size);

    אם (dst == عدم) {
        החזר dst;
    }

    ngx_memcpy(dst, src, size);

    החזר dst;
}


ساكن ngx_resolver_addr_t *
ngx_resolver_export(ngx_resolver_t *r, ngx_resolver_node_t *rn,
    ngx_uint_t rotate)
{
    ngx_uint_t            d, i, j, n;
    in_addr_t            *addr;
    ngx_sockaddr_t       *sockaddr;
    بنية sockaddr_in   *sin;
    ngx_resolver_addr_t  *dst;
#אם (NGX_HAVE_INET6)
    بنية in6_addr      *addr6;
    بنية sockaddr_in6  *sin6;
#endif

    n = rn->naddrs;
#אם (NGX_HAVE_INET6)
    n += rn->naddrs6;
#endif

    dst = ngx_resolver_calloc(r, n * حجم(ngx_resolver_addr_t));
    אם (dst == عدم) {
        החזר عدم;
    }

    sockaddr = ngx_resolver_calloc(r, n * حجم(ngx_sockaddr_t));
    אם (sockaddr == عدم) {
        ngx_resolver_free(r, dst);
        החזר عدم;
    }

    i = 0;
    d = rotate ? ngx_random() % n : 0;

    אם (rn->naddrs) {
        j = rotate ? ngx_random() % rn->naddrs : 0;

        addr = (rn->naddrs == 1) ? &rn->u.addr : rn->u.addrs;

        עשה {
            sin = &sockaddr[d].sockaddr_in;
            sin->sin_family = AF_INET;
            sin->sin_addr.s_addr = addr[j++];
            dst[d].sockaddr = (بنية sockaddr *) sin;
            dst[d++].socklen = حجم(بنية sockaddr_in);

            אם (d == n) {
                d = 0;
            }

            אם (j == (ngx_uint_t) rn->naddrs) {
                j = 0;
            }
        } כלעוד (++i < (ngx_uint_t) rn->naddrs);
    }

#אם (NGX_HAVE_INET6)
    אם (rn->naddrs6) {
        j = rotate ? ngx_random() % rn->naddrs6 : 0;

        addr6 = (rn->naddrs6 == 1) ? &rn->u6.addr6 : rn->u6.addrs6;

        עשה {
            sin6 = &sockaddr[d].sockaddr_in6;
            sin6->sin6_family = AF_INET6;
            ngx_memcpy(sin6->sin6_addr.s6_addr, addr6[j++].s6_addr, 16);
            dst[d].sockaddr = (بنية sockaddr *) sin6;
            dst[d++].socklen = حجم(بنية sockaddr_in6);

            אם (d == n) {
                d = 0;
            }

            אם (j == rn->naddrs6) {
                j = 0;
            }
        } כלעוד (++i < n);
    }
#endif

    החזר dst;
}


ساكن فراغ
ngx_resolver_report_srv(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx)
{
    ngx_uint_t                naddrs, nsrvs, nw, i, j, k, l, m, n, w;
    ngx_resolver_addr_t      *addrs;
    ngx_resolver_srv_name_t  *srvs;

    srvs = ctx->srvs;
    nsrvs = ctx->nsrvs;

    naddrs = 0;

    עבור (i = 0; i < nsrvs; i++) {
        אם (srvs[i].state == NGX_ERROR) {
            ctx->state = NGX_ERROR;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

            ctx->handler(ctx);
            החזר;
        }

        naddrs += srvs[i].naddrs;
    }

    אם (naddrs == 0) {
        ctx->state = srvs[0].state;

        עבור (i = 0; i < nsrvs; i++) {
            אם (srvs[i].state == NGX_RESOLVE_NXDOMAIN) {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                اكسر;
            }
        }

        ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        החזר;
    }

    addrs = ngx_resolver_calloc(r, naddrs * حجم(ngx_resolver_addr_t));
    אם (addrs == عدم) {
        ctx->state = NGX_ERROR;
        ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        החזר;
    }

    i = 0;
    n = 0;

    עשה {
        nw = 0;

        עבור (j = i; j < nsrvs; j++) {
            אם (srvs[j].priority != srvs[i].priority) {
                اكسر;
            }

            nw += srvs[j].naddrs * srvs[j].weight;
        }

        אם (nw == 0) {
            اذهب next_srv;
        }

        w = ngx_random() % nw;

        עבור (k = i; k < j; k++) {
            אם (w < srvs[k].naddrs * srvs[k].weight) {
                اكسر;
            }

            w -= srvs[k].naddrs * srvs[k].weight;
        }

        עבור (l = i; l < j; l++) {

            עבור (m = 0; m < srvs[k].naddrs; m++) {
                addrs[n].socklen = srvs[k].addrs[m].socklen;
                addrs[n].sockaddr = srvs[k].addrs[m].sockaddr;
                addrs[n].name = srvs[k].name;
                addrs[n].priority = srvs[k].priority;
                addrs[n].weight = srvs[k].weight;
                n++;
            }

            אם (++k == j) {
                k = i;
            }
        }

next_srv:

        i = j;

    } כלעוד (i < ctx->nsrvs);

    ctx->state = NGX_OK;
    ctx->addrs = addrs;
    ctx->naddrs = naddrs;

    ctx->handler(ctx);

    ngx_resolver_free(r, addrs);
}


حرف *
ngx_resolver_strerror(ngx_int_t err)
{
    ساكن حرف *errors[] = {
        "Format error",     /* FORMERR */
        "Server failure",   /* SERVFAIL */
        "Host not found",   /* NXDOMAIN */
        "Unimplemented",    /* NOTIMP */
        "Operation refused" /* REFUSED */
    };

    אם (err > 0 && err < 6) {
        החזר errors[err - 1];
    }

    אם (err == NGX_RESOLVE_TIMEDOUT) {
        החזר "Operation timed out";
    }

    החזר "Unknown error";
}


ساكن u_char *
ngx_resolver_log_error(ngx_log_t *log, u_char *buf, size_t len)
{
    u_char                     *p;
    ngx_resolver_connection_t  *rec;

    p = buf;

    אם (log->action) {
        p = ngx_snprintf(buf, len, " while %s", log->action);
        len -= p - buf;
    }

    rec = log->data;

    אם (rec) {
        p = ngx_snprintf(p, len, ", resolver: %V", &rec->server);
    }

    החזר p;
}


ساكن ngx_int_t
ngx_udp_connect(ngx_resolver_connection_t *rec)
{
    صحيح                rc;
    ngx_int_t          event;
    ngx_event_t       *rev, *wev;
    ngx_socket_t       s;
    ngx_connection_t  *c;

    s = ngx_socket(rec->sockaddr->sa_family, SOCK_DGRAM, 0);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "UDP socket %d", s);

    אם (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_socket_n " failed");
        החזר NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->log);

    אם (c == عدم) {
        אם (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        החזר NGX_ERROR;
    }

    אם (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        اذهب failed;
    }

    rev = c->اقرأ;
    wev = c->اكتب;

    rev->log = &rec->log;
    wev->log = &rec->log;

    rec->udp = c;

    c->number = ngx_atomic_fetch_add(ngx_connection_counter, 1);

    c->start_time = ngx_current_msec;

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->log, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->number);

    rc = connect(s, rec->sockaddr, rec->socklen);

    אם (rc == -1) {
        ngx_log_error(NGX_LOG_CRIT, &rec->log, ngx_socket_errno,
                      "connect() failed");

        اذهب failed;
    }

    /* UDP sockets are always ready to write */
    wev->ready = 1;

    event = (ngx_event_flags & NGX_USE_CLEAR_EVENT) ?
                /* kqueue, epoll */                 NGX_CLEAR_EVENT:
                /* select, poll, /dev/poll */       NGX_LEVEL_EVENT;
                /* eventport event type has no meaning: oneshot only */

    אם (ngx_add_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        اذهب failed;
    }

    החזר NGX_OK;

failed:

    ngx_close_connection(c);
    rec->udp = عدم;

    החזר NGX_ERROR;
}


ساكن ngx_int_t
ngx_tcp_connect(ngx_resolver_connection_t *rec)
{
    صحيح                rc;
    ngx_int_t          event;
    ngx_err_t          err;
    ngx_uint_t         level;
    ngx_socket_t       s;
    ngx_event_t       *rev, *wev;
    ngx_connection_t  *c;

    s = ngx_socket(rec->sockaddr->sa_family, SOCK_STREAM, 0);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "TCP socket %d", s);

    אם (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_socket_n " failed");
        החזר NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->log);

    אם (c == عدم) {
        אם (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        החזר NGX_ERROR;
    }

    אם (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        اذهب failed;
    }

    rev = c->اقرأ;
    wev = c->اكتب;

    rev->log = &rec->log;
    wev->log = &rec->log;

    rec->tcp = c;

    c->number = ngx_atomic_fetch_add(ngx_connection_counter, 1);

    c->start_time = ngx_current_msec;

    אם (ngx_add_conn) {
        אם (ngx_add_conn(c) == NGX_ERROR) {
            اذهب failed;
        }
    }

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->log, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->number);

    rc = connect(s, rec->sockaddr, rec->socklen);

    אם (rc == -1) {
        err = ngx_socket_errno;


        אם (err != NGX_EINPROGRESS
#אם (NGX_WIN32)
            /* Winsock returns WSAEWOULDBLOCK (NGX_EAGAIN) */
            && err != NGX_EAGAIN
#endif
            )
        {
            אם (err == NGX_ECONNREFUSED
#אם (NGX_LINUX)
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

            } אחרת {
                level = NGX_LOG_CRIT;
            }

            ngx_log_error(level, &rec->log, err, "connect() to %V failed",
                          &rec->server);

            ngx_close_connection(c);
            rec->tcp = عدم;

            החזר NGX_ERROR;
        }
    }

    אם (ngx_add_conn) {
        אם (rc == -1) {

            /* NGX_EINPROGRESS */

            החזר NGX_AGAIN;
        }

        ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "connected");

        wev->ready = 1;

        החזר NGX_OK;
    }

    אם (ngx_event_flags & NGX_USE_CLEAR_EVENT) {

        /* kqueue */

        event = NGX_CLEAR_EVENT;

    } אחרת {

        /* select, poll, /dev/poll */

        event = NGX_LEVEL_EVENT;
    }

    אם (ngx_add_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        اذهب failed;
    }

    אם (rc == -1) {

        /* NGX_EINPROGRESS */

        אם (ngx_add_event(wev, NGX_WRITE_EVENT, event) != NGX_OK) {
            اذهب failed;
        }

        החזר NGX_AGAIN;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "connected");

    wev->ready = 1;

    החזר NGX_OK;

failed:

    ngx_close_connection(c);
    rec->tcp = عدم;

    החזר NGX_ERROR;
}


ساكن ngx_int_t
ngx_resolver_cmp_srvs(ثابت فراغ *one, ثابت فراغ *two)
{
    ngx_int_t            p1, p2;
    ngx_resolver_srv_t  *first, *second;

    first = (ngx_resolver_srv_t *) one;
    second = (ngx_resolver_srv_t *) two;

    p1 = first->priority;
    p2 = second->priority;

    החזר p1 - p2;
}
