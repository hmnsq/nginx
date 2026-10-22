
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>
#அந்தர்பாவ <ngx_event.h>


#define NGX_RESOLVER_UDP_SIZE   4096

#define NGX_RESOLVER_TCP_RSIZE  (2 + 65535)
#define NGX_RESOLVER_TCP_WSIZE  8192


வகைவரை கட்டமைப்பு {
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


வகைவரை கட்டமைப்பு {
    u_char  type_hi;
    u_char  type_lo;
    u_char  class_hi;
    u_char  class_lo;
} ngx_resolver_qs_t;


வகைவரை கட்டமைப்பு {
    u_char  type_hi;
    u_char  type_lo;
    u_char  class_hi;
    u_char  class_lo;
    u_char  ttl[4];
    u_char  len_hi;
    u_char  len_lo;
} ngx_resolver_an_t;


#define ngx_resolver_முனை(n)  ngx_rbtree_data(n, ngx_resolver_முனை_t, முனை)


நிலையான ngx_int_t ngx_udp_connect(ngx_resolver_connection_t *rec);
நிலையான ngx_int_t ngx_tcp_connect(ngx_resolver_connection_t *rec);


நிலையான வெறுமை ngx_resolver_cleanup(வெறுமை *data);
நிலையான வெறுமை ngx_resolver_cleanup_மரம்(ngx_resolver_t *r, ngx_rbtree_t *மரம்);
நிலையான ngx_int_t ngx_resolve_name_locked(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx, ngx_str_t *name);
நிலையான வெறுமை ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *மரம்,
    ngx_queue_t *queue);
நிலையான ngx_int_t ngx_resolver_send_query(ngx_resolver_t *r,
    ngx_resolver_முனை_t *rn);
நிலையான ngx_int_t ngx_resolver_send_udp_query(ngx_resolver_t *r,
    ngx_resolver_connection_t *rec, u_char *query, u_short qlen);
நிலையான ngx_int_t ngx_resolver_send_tcp_query(ngx_resolver_t *r,
    ngx_resolver_connection_t *rec, u_char *query, u_short qlen);
நிலையான ngx_int_t ngx_resolver_create_name_query(ngx_resolver_t *r,
    ngx_resolver_முனை_t *rn, ngx_str_t *name);
நிலையான ngx_int_t ngx_resolver_create_srv_query(ngx_resolver_t *r,
    ngx_resolver_முனை_t *rn, ngx_str_t *name);
நிலையான ngx_int_t ngx_resolver_create_addr_query(ngx_resolver_t *r,
    ngx_resolver_முனை_t *rn, ngx_resolver_addr_t *addr);
நிலையான வெறுமை ngx_resolver_resend_handler(ngx_event_t *ev);
நிலையான time_t ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *மரம்,
    ngx_queue_t *queue);
நிலையான ngx_uint_t ngx_resolver_resend_empty(ngx_resolver_t *r);
நிலையான வெறுமை ngx_resolver_udp_read(ngx_event_t *rev);
நிலையான வெறுமை ngx_resolver_tcp_write(ngx_event_t *wev);
நிலையான வெறுமை ngx_resolver_tcp_read(ngx_event_t *rev);
நிலையான வெறுமை ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf,
    size_t n, ngx_uint_t tcp);
நிலையான வெறுமை ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans);
நிலையான வெறுமை ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans);
நிலையான வெறுமை ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan);
நிலையான ngx_resolver_முனை_t *ngx_resolver_lookup_name(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
நிலையான ngx_resolver_முனை_t *ngx_resolver_lookup_srv(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
நிலையான ngx_resolver_முனை_t *ngx_resolver_lookup_addr(ngx_resolver_t *r,
    in_addr_t addr);
நிலையான வெறுமை ngx_resolver_rbtree_செருகு_மதிப்பு(ngx_rbtree_முனை_t *temp,
    ngx_rbtree_முனை_t *முனை, ngx_rbtree_முனை_t *sentinel);
நிலையான ngx_int_t ngx_resolver_copy(ngx_resolver_t *r, ngx_str_t *name,
    u_char *buf, u_char *src, u_char *last);
நிலையான ngx_int_t ngx_resolver_set_timeout(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx);
நிலையான வெறுமை ngx_resolver_timeout_handler(ngx_event_t *ev);
நிலையான வெறுமை ngx_resolver_free_முனை(ngx_resolver_t *r, ngx_resolver_முனை_t *rn);
நிலையான வெறுமை *ngx_resolver_alloc(ngx_resolver_t *r, size_t size);
நிலையான வெறுமை *ngx_resolver_calloc(ngx_resolver_t *r, size_t size);
நிலையான வெறுமை ngx_resolver_free(ngx_resolver_t *r, வெறுமை *p);
நிலையான வெறுமை ngx_resolver_free_locked(ngx_resolver_t *r, வெறுமை *p);
நிலையான வெறுமை *ngx_resolver_dup(ngx_resolver_t *r, வெறுமை *src, size_t size);
நிலையான ngx_resolver_addr_t *ngx_resolver_export(ngx_resolver_t *r,
    ngx_resolver_முனை_t *rn, ngx_uint_t rotate);
நிலையான வெறுமை ngx_resolver_report_srv(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx);
நிலையான u_char *ngx_resolver_log_error(ngx_log_t *log, u_char *buf, size_t len);
நிலையான வெறுமை ngx_resolver_resolve_srv_names(ngx_resolver_ctx_t *ctx,
    ngx_resolver_முனை_t *rn);
நிலையான வெறுமை ngx_resolver_srv_names_handler(ngx_resolver_ctx_t *ctx);
நிலையான ngx_int_t ngx_resolver_cmp_srvs(மாறிலி வெறுமை *one, மாறிலி வெறுமை *two);

#எனில் (NGX_HAVE_INET6)
நிலையான வெறுமை ngx_resolver_rbtree_செருகு_addr6_மதிப்பு(ngx_rbtree_முனை_t *temp,
    ngx_rbtree_முனை_t *முனை, ngx_rbtree_முனை_t *sentinel);
நிலையான ngx_resolver_முனை_t *ngx_resolver_lookup_addr6(ngx_resolver_t *r,
    கட்டமைப்பு in6_addr *addr, uint32_t hash);
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

    r = ngx_pcalloc(cf->pool, அளவு(ngx_resolver_t));
    எனில் (r == வெற்று) {
        திரும்பு வெற்று;
    }

    r->event = ngx_pcalloc(cf->pool, அளவு(ngx_event_t));
    எனில் (r->event == வெற்று) {
        திரும்பு வெற்று;
    }

    cln = ngx_pool_cleanup_add(cf->pool, 0);
    எனில் (cln == வெற்று) {
        திரும்பு வெற்று;
    }

    cln->handler = ngx_resolver_cleanup;
    cln->data = r;

    r->ipv4 = 1;

    ngx_rbtree_init(&r->name_rbtree, &r->name_sentinel,
                    ngx_resolver_rbtree_செருகு_மதிப்பு);

    ngx_rbtree_init(&r->srv_rbtree, &r->srv_sentinel,
                    ngx_resolver_rbtree_செருகு_மதிப்பு);

    ngx_rbtree_init(&r->addr_rbtree, &r->addr_sentinel,
                    ngx_rbtree_செருகு_மதிப்பு);

    ngx_queue_init(&r->name_resend_queue);
    ngx_queue_init(&r->srv_resend_queue);
    ngx_queue_init(&r->addr_resend_queue);

    ngx_queue_init(&r->name_expire_queue);
    ngx_queue_init(&r->srv_expire_queue);
    ngx_queue_init(&r->addr_expire_queue);

#எனில் (NGX_HAVE_INET6)
    r->ipv6 = 1;

    ngx_rbtree_init(&r->addr6_rbtree, &r->addr6_sentinel,
                    ngx_resolver_rbtree_செருகு_addr6_மதிப்பு);

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

    எனில் (n) {
        எனில் (ngx_array_init(&r->connections, cf->pool, n,
                           அளவு(ngx_resolver_connection_t))
            != NGX_OK)
        {
            திரும்பு வெற்று;
        }
    }

    க்கு (i = 0; i < n; i++) {
        எனில் (ngx_strncmp(names[i].data, "valid=", 6) == 0) {
            s.len = names[i].len - 6;
            s.data = names[i].data + 6;

            r->valid = ngx_parse_time(&s, 1);

            எனில் (r->valid == (time_t) NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &names[i]);
                திரும்பு வெற்று;
            }

            தொடர்;
        }

#எனில் (NGX_HAVE_INET6)
        எனில் (ngx_strncmp(names[i].data, "ipv4=", 5) == 0) {

            எனில் (ngx_strcmp(&names[i].data[5], "on") == 0) {
                r->ipv4 = 1;

            } இல்லையெனில் எனில் (ngx_strcmp(&names[i].data[5], "off") == 0) {
                r->ipv4 = 0;

            } இல்லையெனில் {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &names[i]);
                திரும்பு வெற்று;
            }

            தொடர்;
        }

        எனில் (ngx_strncmp(names[i].data, "ipv6=", 5) == 0) {

            எனில் (ngx_strcmp(&names[i].data[5], "on") == 0) {
                r->ipv6 = 1;

            } இல்லையெனில் எனில் (ngx_strcmp(&names[i].data[5], "off") == 0) {
                r->ipv6 = 0;

            } இல்லையெனில் {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &names[i]);
                திரும்பு வெற்று;
            }

            தொடர்;
        }
#endif

        ngx_memzero(&u, அளவு(ngx_url_t));

        u.url = names[i];
        u.default_port = 53;

        எனில் (ngx_parse_url(cf->pool, &u) != NGX_OK) {
            எனில் (u.err) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "%s in resolver \"%V\"",
                                   u.err, &u.url);
            }

            திரும்பு வெற்று;
        }

        rec = ngx_array_தள்ளு_n(&r->connections, u.naddrs);
        எனில் (rec == வெற்று) {
            திரும்பு வெற்று;
        }

        ngx_memzero(rec, u.naddrs * அளவு(ngx_resolver_connection_t));

        க்கு (j = 0; j < u.naddrs; j++) {
            rec[j].sockaddr = u.addrs[j].sockaddr;
            rec[j].socklen = u.addrs[j].socklen;
            rec[j].server = u.addrs[j].name;
            rec[j].resolver = r;
        }
    }

#எனில் (NGX_HAVE_INET6)
    எனில் (r->ipv4 + r->ipv6 == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"ipv4\" and \"ipv6\" cannot both be \"off\"");
        திரும்பு வெற்று;
    }
#endif

    எனில் (n && r->connections.nelts == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "no name servers defined");
        திரும்பு வெற்று;
    }

    திரும்பு r;
}


நிலையான வெறுமை
ngx_resolver_cleanup(வெறுமை *data)
{
    ngx_resolver_t  *r = data;

    ngx_uint_t                  i;
    ngx_resolver_connection_t  *rec;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "cleanup resolver");

    ngx_resolver_cleanup_மரம்(r, &r->name_rbtree);

    ngx_resolver_cleanup_மரம்(r, &r->srv_rbtree);

    ngx_resolver_cleanup_மரம்(r, &r->addr_rbtree);

#எனில் (NGX_HAVE_INET6)
    ngx_resolver_cleanup_மரம்(r, &r->addr6_rbtree);
#endif

    எனில் (r->event->timer_set) {
        ngx_del_timer(r->event);
    }

    rec = r->connections.elts;

    க்கு (i = 0; i < r->connections.nelts; i++) {
        எனில் (rec[i].udp) {
            ngx_close_connection(rec[i].udp);
        }

        எனில் (rec[i].tcp) {
            ngx_close_connection(rec[i].tcp);
        }

        எனில் (rec[i].read_buf) {
            ngx_resolver_free(r, rec[i].read_buf->start);
            ngx_resolver_free(r, rec[i].read_buf);
        }

        எனில் (rec[i].write_buf) {
            ngx_resolver_free(r, rec[i].write_buf->start);
            ngx_resolver_free(r, rec[i].write_buf);
        }
    }
}


நிலையான வெறுமை
ngx_resolver_cleanup_மரம்(ngx_resolver_t *r, ngx_rbtree_t *மரம்)
{
    ngx_resolver_ctx_t   *ctx, *next;
    ngx_resolver_முனை_t  *rn;

    போது (மரம்->வேர் != மரம்->sentinel) {

        rn = ngx_resolver_முனை(ngx_rbtree_min(மரம்->வேர், மரம்->sentinel));

        ngx_queue_remove(&rn->queue);

        க்கு (ctx = rn->waiting; ctx; ctx = next) {
            next = ctx->next;

            எனில் (ctx->event) {
                எனில் (ctx->event->timer_set) {
                    ngx_del_timer(ctx->event);
                }

                ngx_resolver_free(r, ctx->event);
            }

            ngx_resolver_free(r, ctx);
        }

        ngx_rbtree_delete(மரம், &rn->முனை);

        ngx_resolver_free_முனை(r, rn);
    }
}


ngx_resolver_ctx_t *
ngx_resolve_start(ngx_resolver_t *r, ngx_resolver_ctx_t *temp)
{
    in_addr_t            addr;
    ngx_resolver_ctx_t  *ctx;

    எனில் (temp) {
        addr = ngx_inet_addr(temp->name.data, temp->name.len);

        எனில் (addr != INADDR_NONE) {
            temp->resolver = r;
            temp->state = NGX_OK;
            temp->naddrs = 1;
            temp->addrs = &temp->addr;
            temp->addr.sockaddr = (கட்டமைப்பு sockaddr *) &temp->sin;
            temp->addr.socklen = அளவு(கட்டமைப்பு sockaddr_in);
            ngx_memzero(&temp->sin, அளவு(கட்டமைப்பு sockaddr_in));
            temp->sin.sin_family = AF_INET;
            temp->sin.sin_addr.s_addr = addr;
            temp->quick = 1;

            திரும்பு temp;
        }
    }

    எனில் (r->connections.nelts == 0) {
        திரும்பு NGX_NO_RESOLVER;
    }

    ctx = ngx_resolver_calloc(r, அளவு(ngx_resolver_ctx_t));

    எனில் (ctx) {
        ctx->resolver = r;
    }

    திரும்பு ctx;
}


ngx_int_t
ngx_resolve_name(ngx_resolver_ctx_t *ctx)
{
    size_t           slen;
    ngx_int_t        rc;
    ngx_str_t        name;
    ngx_resolver_t  *r;

    r = ctx->resolver;

    எனில் (ctx->name.len > 0 && ctx->name.data[ctx->name.len - 1] == '.') {
        ctx->name.len--;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve: \"%V\"", &ctx->name);

    எனில் (ctx->quick) {
        ctx->handler(ctx);
        திரும்பு NGX_OK;
    }

    எனில் (ctx->service.len) {
        slen = ctx->service.len;

        எனில் (ngx_strlchr(ctx->service.data,
                        ctx->service.data + ctx->service.len, '.')
            == வெற்று)
        {
            slen += அளவு("_._tcp") - 1;
        }

        name.len = slen + 1 + ctx->name.len;

        name.data = ngx_resolver_alloc(r, name.len);
        எனில் (name.data == வெற்று) {
            செல் failed;
        }

        எனில் (slen == ctx->service.len) {
            ngx_sprintf(name.data, "%V.%V", &ctx->service, &ctx->name);

        } இல்லையெனில் {
            ngx_sprintf(name.data, "_%V._tcp.%V", &ctx->service, &ctx->name);
        }

        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &name);

        ngx_resolver_free(r, name.data);

    } இல்லையெனில் {
        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &ctx->name);
    }

    எனில் (rc == NGX_OK) {
        திரும்பு NGX_OK;
    }

    /* unlock name mutex */

    எனில் (rc == NGX_AGAIN) {
        திரும்பு NGX_OK;
    }

    /* NGX_ERROR */

    எனில் (ctx->event) {
        ngx_resolver_free(r, ctx->event);
    }

failed:

    ngx_resolver_free(r, ctx);

    திரும்பு NGX_ERROR;
}


வெறுமை
ngx_resolve_name_done(ngx_resolver_ctx_t *ctx)
{
    ngx_uint_t            i;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_முனை_t  *rn;

    r = ctx->resolver;

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve name done: %i", ctx->state);

    எனில் (ctx->quick) {
        திரும்பு;
    }

    எனில் (ctx->event && ctx->event->timer_set) {
        ngx_del_timer(ctx->event);
    }

    /* lock name mutex */

    எனில் (ctx->nsrvs) {
        க்கு (i = 0; i < ctx->nsrvs; i++) {
            எனில் (ctx->srvs[i].ctx) {
                ngx_resolve_name_done(ctx->srvs[i].ctx);
            }

            எனில் (ctx->srvs[i].addrs) {
                ngx_resolver_free(r, ctx->srvs[i].addrs->sockaddr);
                ngx_resolver_free(r, ctx->srvs[i].addrs);
            }

            ngx_resolver_free(r, ctx->srvs[i].name.data);
        }

        ngx_resolver_free(r, ctx->srvs);
    }

    எனில் (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->முனை;

        எனில் (rn) {
            p = &rn->waiting;
            w = rn->waiting;

            போது (w) {
                எனில் (w == ctx) {
                    *p = w->next;

                    செல் done;
                }

                p = &w->next;
                w = w->next;
            }

            ngx_log_error(NGX_LOG_ALERT, r->log, 0,
                          "could not cancel %V resolving", &ctx->name);
        }
    }

done:

    எனில் (ctx->service.len) {
        ngx_resolver_expire(r, &r->srv_rbtree, &r->srv_expire_queue);

    } இல்லையெனில் {
        ngx_resolver_expire(r, &r->name_rbtree, &r->name_expire_queue);
    }

    /* unlock name mutex */

    /* lock alloc mutex */

    எனில் (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    எனில் (r->event->timer_set && ngx_resolver_resend_empty(r)) {
        ngx_del_timer(r->event);
    }
}


நிலையான ngx_int_t
ngx_resolve_name_locked(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx,
    ngx_str_t *name)
{
    uint32_t              hash;
    ngx_int_t             rc;
    ngx_str_t             cname;
    ngx_uint_t            i, naddrs;
    ngx_queue_t          *resend_queue, *expire_queue;
    ngx_rbtree_t         *மரம்;
    ngx_resolver_ctx_t   *next, *last;
    ngx_resolver_addr_t  *addrs;
    ngx_resolver_முனை_t  *rn;

    ngx_strlow(name->data, name->data, name->len);

    hash = ngx_crc32_short(name->data, name->len);

    எனில் (ctx->service.len) {
        rn = ngx_resolver_lookup_srv(r, name, hash);

        மரம் = &r->srv_rbtree;
        resend_queue = &r->srv_resend_queue;
        expire_queue = &r->srv_expire_queue;

    } இல்லையெனில் {
        rn = ngx_resolver_lookup_name(r, name, hash);

        மரம் = &r->name_rbtree;
        resend_queue = &r->name_resend_queue;
        expire_queue = &r->name_expire_queue;
    }

    எனில் (rn) {

        /* ctx can be a list after NGX_RESOLVE_CNAME */
        க்கு (last = ctx; last->next; last = last->next);

        எனில் (rn->valid >= ngx_time()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0, "resolve cached");

            ngx_queue_remove(&rn->queue);

            rn->expire = ngx_time() + r->expire;

            ngx_queue_செருகு_head(expire_queue, &rn->queue);

            naddrs = (rn->naddrs == (u_short) -1) ? 0 : rn->naddrs;
#எனில் (NGX_HAVE_INET6)
            naddrs += (rn->naddrs6 == (u_short) -1) ? 0 : rn->naddrs6;
#endif

            எனில் (naddrs) {

                எனில் (naddrs == 1 && rn->naddrs == 1) {
                    addrs = வெற்று;

                } இல்லையெனில் {
                    addrs = ngx_resolver_export(r, rn, 1);
                    எனில் (addrs == வெற்று) {
                        திரும்பு NGX_ERROR;
                    }
                }

                last->next = rn->waiting;
                rn->waiting = வெற்று;

                /* unlock name mutex */

                செய் {
                    ctx->state = NGX_OK;
                    ctx->valid = rn->valid;
                    ctx->naddrs = naddrs;

                    எனில் (addrs == வெற்று) {
                        ctx->addrs = &ctx->addr;
                        ctx->addr.sockaddr = (கட்டமைப்பு sockaddr *) &ctx->sin;
                        ctx->addr.socklen = அளவு(கட்டமைப்பு sockaddr_in);
                        ngx_memzero(&ctx->sin, அளவு(கட்டமைப்பு sockaddr_in));
                        ctx->sin.sin_family = AF_INET;
                        ctx->sin.sin_addr.s_addr = rn->u.addr;

                    } இல்லையெனில் {
                        ctx->addrs = addrs;
                    }

                    next = ctx->next;

                    ctx->handler(ctx);

                    ctx = next;
                } போது (ctx);

                எனில் (addrs != வெற்று) {
                    ngx_resolver_free(r, addrs->sockaddr);
                    ngx_resolver_free(r, addrs);
                }

                திரும்பு NGX_OK;
            }

            எனில் (rn->nsrvs) {
                last->next = rn->waiting;
                rn->waiting = வெற்று;

                /* unlock name mutex */

                செய் {
                    next = ctx->next;

                    ngx_resolver_resolve_srv_names(ctx, rn);

                    ctx = next;
                } போது (ctx);

                திரும்பு NGX_OK;
            }

            /* NGX_RESOLVE_CNAME */

            எனில் (ctx->recursion++ < NGX_RESOLVER_MAX_RECURSION) {

                cname.len = rn->cnlen;
                cname.data = rn->u.cname;

                திரும்பு ngx_resolve_name_locked(r, ctx, &cname);
            }

            last->next = rn->waiting;
            rn->waiting = வெற்று;

            /* unlock name mutex */

            செய் {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
                next = ctx->next;

                ctx->handler(ctx);

                ctx = next;
            } போது (ctx);

            திரும்பு NGX_OK;
        }

        எனில் (rn->waiting) {
            எனில் (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
                திரும்பு NGX_ERROR;
            }

            last->next = rn->waiting;
            rn->waiting = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;

            செய் {
                ctx->முனை = rn;
                ctx = ctx->next;
            } போது (ctx);

            திரும்பு NGX_AGAIN;
        }

        ngx_queue_remove(&rn->queue);

        /* lock alloc mutex */

        எனில் (rn->query) {
            ngx_resolver_free_locked(r, rn->query);
            rn->query = வெற்று;
#எனில் (NGX_HAVE_INET6)
            rn->query6 = வெற்று;
#endif
        }

        எனில் (rn->cnlen) {
            ngx_resolver_free_locked(r, rn->u.cname);
        }

        எனில் (rn->naddrs > 1 && rn->naddrs != (u_short) -1) {
            ngx_resolver_free_locked(r, rn->u.addrs);
        }

#எனில் (NGX_HAVE_INET6)
        எனில் (rn->naddrs6 > 1 && rn->naddrs6 != (u_short) -1) {
            ngx_resolver_free_locked(r, rn->u6.addrs6);
        }
#endif

        எனில் (rn->nsrvs) {
            க்கு (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
                எனில் (rn->u.srvs[i].name.data) {
                    ngx_resolver_free_locked(r, rn->u.srvs[i].name.data);
                }
            }

            ngx_resolver_free_locked(r, rn->u.srvs);
        }

        /* unlock alloc mutex */

    } இல்லையெனில் {

        rn = ngx_resolver_alloc(r, அளவு(ngx_resolver_முனை_t));
        எனில் (rn == வெற்று) {
            திரும்பு NGX_ERROR;
        }

        rn->name = ngx_resolver_dup(r, name->data, name->len);
        எனில் (rn->name == வெற்று) {
            ngx_resolver_free(r, rn);
            திரும்பு NGX_ERROR;
        }

        rn->முனை.சாவி = hash;
        rn->nlen = (u_short) name->len;
        rn->query = வெற்று;
#எனில் (NGX_HAVE_INET6)
        rn->query6 = வெற்று;
#endif

        ngx_rbtree_செருகு(மரம், &rn->முனை);
    }

    எனில் (ctx->service.len) {
        rc = ngx_resolver_create_srv_query(r, rn, name);

    } இல்லையெனில் {
        rc = ngx_resolver_create_name_query(r, rn, name);
    }

    எனில் (rc == NGX_ERROR) {
        செல் failed;
    }

    எனில் (rc == NGX_DECLINED) {
        ngx_rbtree_delete(மரம், &rn->முனை);

        ngx_resolver_free(r, rn->query);
        ngx_resolver_free(r, rn->name);
        ngx_resolver_free(r, rn);

        செய் {
            ctx->state = NGX_RESOLVE_NXDOMAIN;
            next = ctx->next;

            ctx->handler(ctx);

            ctx = next;
        } போது (ctx);

        திரும்பு NGX_OK;
    }

    rn->last_connection = r->last_connection++;
    எனில் (r->last_connection == r->connections.nelts) {
        r->last_connection = 0;
    }

    rn->naddrs = r->ipv4 ? (u_short) -1 : 0;
    rn->tcp = 0;
#எனில் (NGX_HAVE_INET6)
    rn->naddrs6 = r->ipv6 ? (u_short) -1 : 0;
    rn->tcp6 = 0;
#endif
    rn->nsrvs = 0;

    எனில் (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->last_connection++;
        எனில் (rn->last_connection == r->connections.nelts) {
            rn->last_connection = 0;
        }

        (வெறுமை) ngx_resolver_send_query(r, rn);
    }

    எனில் (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
        செல் failed;
    }

    எனில் (ngx_resolver_resend_empty(r)) {
        ngx_add_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_time() + r->resend_timeout;

    ngx_queue_செருகு_head(resend_queue, &rn->queue);

    rn->code = 0;
    rn->cnlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_மதிப்பு;
    rn->waiting = ctx;

    ctx->state = NGX_AGAIN;
    ctx->async = 1;

    செய் {
        ctx->முனை = rn;
        ctx = ctx->next;
    } போது (ctx);

    திரும்பு NGX_AGAIN;

failed:

    ngx_rbtree_delete(மரம், &rn->முனை);

    எனில் (rn->query) {
        ngx_resolver_free(r, rn->query);
    }

    ngx_resolver_free(r, rn->name);

    ngx_resolver_free(r, rn);

    திரும்பு NGX_ERROR;
}


ngx_int_t
ngx_resolve_addr(ngx_resolver_ctx_t *ctx)
{
    u_char               *name;
    in_addr_t             addr;
    ngx_queue_t          *resend_queue, *expire_queue;
    ngx_rbtree_t         *மரம்;
    ngx_resolver_t       *r;
    கட்டமைப்பு sockaddr_in   *sin;
    ngx_resolver_முனை_t  *rn;
#எனில் (NGX_HAVE_INET6)
    uint32_t              hash;
    கட்டமைப்பு sockaddr_in6  *sin6;
#endif

#எனில் (NGX_SUPPRESS_WARN)
    addr = 0;
#எனில் (NGX_HAVE_INET6)
    hash = 0;
    sin6 = வெற்று;
#endif
#endif

    r = ctx->resolver;

    தேர்வு (ctx->addr.sockaddr->sa_family) {

#எனில் (NGX_HAVE_INET6)
    நிலை AF_INET6:
        sin6 = (கட்டமைப்பு sockaddr_in6 *) ctx->addr.sockaddr;
        hash = ngx_crc32_short(sin6->sin6_addr.s6_addr, 16);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr6(r, &sin6->sin6_addr, hash);

        மரம் = &r->addr6_rbtree;
        resend_queue = &r->addr6_resend_queue;
        expire_queue = &r->addr6_expire_queue;

        நிறுத்து;
#endif

    இயல்பு: /* AF_INET */
        sin = (கட்டமைப்பு sockaddr_in *) ctx->addr.sockaddr;
        addr = ntohl(sin->sin_addr.s_addr);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        மரம் = &r->addr_rbtree;
        resend_queue = &r->addr_resend_queue;
        expire_queue = &r->addr_expire_queue;
    }

    எனில் (rn) {

        எனில் (rn->valid >= ngx_time()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0, "resolve cached");

            ngx_queue_remove(&rn->queue);

            rn->expire = ngx_time() + r->expire;

            ngx_queue_செருகு_head(expire_queue, &rn->queue);

            name = ngx_resolver_dup(r, rn->name, rn->nlen);
            எனில் (name == வெற்று) {
                ngx_resolver_free(r, ctx);
                திரும்பு NGX_ERROR;
            }

            ctx->name.len = rn->nlen;
            ctx->name.data = name;

            /* unlock addr mutex */

            ctx->state = NGX_OK;
            ctx->valid = rn->valid;

            ctx->handler(ctx);

            ngx_resolver_free(r, name);

            திரும்பு NGX_OK;
        }

        எனில் (rn->waiting) {
            எனில் (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
                திரும்பு NGX_ERROR;
            }

            ctx->next = rn->waiting;
            rn->waiting = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;
            ctx->முனை = rn;

            /* unlock addr mutex */

            திரும்பு NGX_OK;
        }

        ngx_queue_remove(&rn->queue);

        ngx_resolver_free(r, rn->query);
        rn->query = வெற்று;
#எனில் (NGX_HAVE_INET6)
        rn->query6 = வெற்று;
#endif

    } இல்லையெனில் {
        rn = ngx_resolver_alloc(r, அளவு(ngx_resolver_முனை_t));
        எனில் (rn == வெற்று) {
            செல் failed;
        }

        தேர்வு (ctx->addr.sockaddr->sa_family) {

#எனில் (NGX_HAVE_INET6)
        நிலை AF_INET6:
            rn->addr6 = sin6->sin6_addr;
            rn->முனை.சாவி = hash;
            நிறுத்து;
#endif

        இயல்பு: /* AF_INET */
            rn->முனை.சாவி = addr;
        }

        rn->query = வெற்று;
#எனில் (NGX_HAVE_INET6)
        rn->query6 = வெற்று;
#endif

        ngx_rbtree_செருகு(மரம், &rn->முனை);
    }

    எனில் (ngx_resolver_create_addr_query(r, rn, &ctx->addr) != NGX_OK) {
        செல் failed;
    }

    rn->last_connection = r->last_connection++;
    எனில் (r->last_connection == r->connections.nelts) {
        r->last_connection = 0;
    }

    rn->naddrs = (u_short) -1;
    rn->tcp = 0;
#எனில் (NGX_HAVE_INET6)
    rn->naddrs6 = (u_short) -1;
    rn->tcp6 = 0;
#endif
    rn->nsrvs = 0;

    எனில் (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->last_connection++;
        எனில் (rn->last_connection == r->connections.nelts) {
            rn->last_connection = 0;
        }

        (வெறுமை) ngx_resolver_send_query(r, rn);
    }

    எனில் (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
        செல் failed;
    }

    எனில் (ngx_resolver_resend_empty(r)) {
        ngx_add_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_time() + r->resend_timeout;

    ngx_queue_செருகு_head(resend_queue, &rn->queue);

    rn->code = 0;
    rn->cnlen = 0;
    rn->name = வெற்று;
    rn->nlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_மதிப்பு;
    rn->waiting = ctx;

    /* unlock addr mutex */

    ctx->state = NGX_AGAIN;
    ctx->async = 1;
    ctx->முனை = rn;

    திரும்பு NGX_OK;

failed:

    எனில் (rn) {
        ngx_rbtree_delete(மரம், &rn->முனை);

        எனில் (rn->query) {
            ngx_resolver_free(r, rn->query);
        }

        ngx_resolver_free(r, rn);
    }

    /* unlock addr mutex */

    எனில் (ctx->event) {
        ngx_resolver_free(r, ctx->event);
    }

    ngx_resolver_free(r, ctx);

    திரும்பு NGX_ERROR;
}


வெறுமை
ngx_resolve_addr_done(ngx_resolver_ctx_t *ctx)
{
    ngx_queue_t          *expire_queue;
    ngx_rbtree_t         *மரம்;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_முனை_t  *rn;

    r = ctx->resolver;

    தேர்வு (ctx->addr.sockaddr->sa_family) {

#எனில் (NGX_HAVE_INET6)
    நிலை AF_INET6:
        மரம் = &r->addr6_rbtree;
        expire_queue = &r->addr6_expire_queue;
        நிறுத்து;
#endif

    இயல்பு: /* AF_INET */
        மரம் = &r->addr_rbtree;
        expire_queue = &r->addr_expire_queue;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve addr done: %i", ctx->state);

    எனில் (ctx->event && ctx->event->timer_set) {
        ngx_del_timer(ctx->event);
    }

    /* lock addr mutex */

    எனில் (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->முனை;

        எனில் (rn) {
            p = &rn->waiting;
            w = rn->waiting;

            போது (w) {
                எனில் (w == ctx) {
                    *p = w->next;

                    செல் done;
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

    ngx_resolver_expire(r, மரம், expire_queue);

    /* unlock addr mutex */

    /* lock alloc mutex */

    எனில் (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    எனில் (r->event->timer_set && ngx_resolver_resend_empty(r)) {
        ngx_del_timer(r->event);
    }
}


நிலையான வெறுமை
ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *மரம், ngx_queue_t *queue)
{
    time_t                now;
    ngx_uint_t            i;
    ngx_queue_t          *q;
    ngx_resolver_முனை_t  *rn;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver expire");

    now = ngx_time();

    க்கு (i = 0; i < 2; i++) {
        எனில் (ngx_queue_empty(queue)) {
            திரும்பு;
        }

        q = ngx_queue_last(queue);

        rn = ngx_queue_data(q, ngx_resolver_முனை_t, queue);

        எனில் (now <= rn->expire) {
            திரும்பு;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver expire \"%*s\"", (size_t) rn->nlen, rn->name);

        ngx_queue_remove(q);

        ngx_rbtree_delete(மரம், &rn->முனை);

        ngx_resolver_free_முனை(r, rn);
    }
}


நிலையான ngx_int_t
ngx_resolver_send_query(ngx_resolver_t *r, ngx_resolver_முனை_t *rn)
{
    ngx_int_t                   rc;
    ngx_resolver_connection_t  *rec;

    rec = r->connections.elts;
    rec = &rec[rn->last_connection];

    எனில் (rec->log.handler == வெற்று) {
        rec->log = *r->log;
        rec->log.handler = ngx_resolver_log_error;
        rec->log.data = rec;
        rec->log.action = "resolving";
    }

    எனில் (rn->query && rn->naddrs == (u_short) -1) {
        rc = rn->tcp ? ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen)
                     : ngx_resolver_send_udp_query(r, rec, rn->query, rn->qlen);

        எனில் (rc != NGX_OK) {
            திரும்பு rc;
        }
    }

#எனில் (NGX_HAVE_INET6)

    எனில் (rn->query6 && rn->naddrs6 == (u_short) -1) {
        rc = rn->tcp6
                    ? ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen)
                    : ngx_resolver_send_udp_query(r, rec, rn->query6, rn->qlen);

        எனில் (rc != NGX_OK) {
            திரும்பு rc;
        }
    }

#endif

    திரும்பு NGX_OK;
}


நிலையான ngx_int_t
ngx_resolver_send_udp_query(ngx_resolver_t *r, ngx_resolver_connection_t  *rec,
    u_char *query, u_short qlen)
{
    ssize_t  n;

    எனில் (rec->udp == வெற்று) {
        எனில் (ngx_udp_connect(rec) != NGX_OK) {
            திரும்பு NGX_ERROR;
        }

        rec->udp->data = rec;
        rec->udp->பட->handler = ngx_resolver_udp_read;
        rec->udp->பட->resolver = 1;
    }

    n = ngx_send(rec->udp, query, qlen);

    எனில் (n == NGX_ERROR) {
        செல் failed;
    }

    எனில் ((size_t) n != (size_t) qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->log, 0, "send() incomplete");
        செல் failed;
    }

    திரும்பு NGX_OK;

failed:

    ngx_close_connection(rec->udp);
    rec->udp = வெற்று;

    திரும்பு NGX_ERROR;
}


நிலையான ngx_int_t
ngx_resolver_send_tcp_query(ngx_resolver_t *r, ngx_resolver_connection_t *rec,
    u_char *query, u_short qlen)
{
    ngx_buf_t  *b;
    ngx_int_t   rc;

    rc = NGX_OK;

    எனில் (rec->tcp == வெற்று) {
        b = rec->read_buf;

        எனில் (b == வெற்று) {
            b = ngx_resolver_calloc(r, அளவு(ngx_buf_t));
            எனில் (b == வெற்று) {
                திரும்பு NGX_ERROR;
            }

            b->start = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_RSIZE);
            எனில் (b->start == வெற்று) {
                ngx_resolver_free(r, b);
                திரும்பு NGX_ERROR;
            }

            b->end = b->start + NGX_RESOLVER_TCP_RSIZE;

            rec->read_buf = b;
        }

        b->pos = b->start;
        b->last = b->start;

        b = rec->write_buf;

        எனில் (b == வெற்று) {
            b = ngx_resolver_calloc(r, அளவு(ngx_buf_t));
            எனில் (b == வெற்று) {
                திரும்பு NGX_ERROR;
            }

            b->start = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_WSIZE);
            எனில் (b->start == வெற்று) {
                ngx_resolver_free(r, b);
                திரும்பு NGX_ERROR;
            }

            b->end = b->start + NGX_RESOLVER_TCP_WSIZE;

            rec->write_buf = b;
        }

        b->pos = b->start;
        b->last = b->start;

        rc = ngx_tcp_connect(rec);
        எனில் (rc == NGX_ERROR) {
            திரும்பு NGX_ERROR;
        }

        rec->tcp->data = rec;
        rec->tcp->லிக->handler = ngx_resolver_tcp_write;
        rec->tcp->லிக->cancelable = 1;
        rec->tcp->பட->handler = ngx_resolver_tcp_read;
        rec->tcp->பட->resolver = 1;

        ngx_add_timer(rec->tcp->லிக, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    b = rec->write_buf;

    எனில் (b->end - b->last <  2 + qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->log, 0, "buffer overflow");
        திரும்பு NGX_ERROR;
    }

    *b->last++ = (u_char) (qlen >> 8);
    *b->last++ = (u_char) qlen;
    b->last = ngx_cpymem(b->last, query, qlen);

    எனில் (rc == NGX_OK) {
        ngx_resolver_tcp_write(rec->tcp->லிக);
    }

    திரும்பு NGX_OK;
}


நிலையான வெறுமை
ngx_resolver_resend_handler(ngx_event_t *ev)
{
    time_t           timer, atimer, stimer, ntimer;
#எனில் (NGX_HAVE_INET6)
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

#எனில் (NGX_HAVE_INET6)

    /* lock addr6 mutex */

    a6timer = ngx_resolver_resend(r, &r->addr6_rbtree, &r->addr6_resend_queue);

    /* unlock addr6 mutex */

#endif

    timer = ntimer;

    எனில் (timer == 0) {
        timer = atimer;

    } இல்லையெனில் எனில் (atimer) {
        timer = ngx_min(timer, atimer);
    }

    எனில் (timer == 0) {
        timer = stimer;

    } இல்லையெனில் எனில் (stimer) {
        timer = ngx_min(timer, stimer);
    }

#எனில் (NGX_HAVE_INET6)

    எனில் (timer == 0) {
        timer = a6timer;

    } இல்லையெனில் எனில் (a6timer) {
        timer = ngx_min(timer, a6timer);
    }

#endif

    எனில் (timer) {
        ngx_add_timer(r->event, (ngx_msec_t) (timer * 1000));
    }
}


நிலையான time_t
ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *மரம், ngx_queue_t *queue)
{
    time_t                now;
    ngx_queue_t          *q;
    ngx_resolver_முனை_t  *rn;

    now = ngx_time();

    க்கு ( ;; ) {
        எனில் (ngx_queue_empty(queue)) {
            திரும்பு 0;
        }

        q = ngx_queue_last(queue);

        rn = ngx_queue_data(q, ngx_resolver_முனை_t, queue);

        எனில் (now < rn->expire) {
            திரும்பு rn->expire - now;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver resend \"%*s\" %p",
                       (size_t) rn->nlen, rn->name, rn->waiting);

        ngx_queue_remove(q);

        எனில் (rn->waiting) {

            எனில் (++rn->last_connection == r->connections.nelts) {
                rn->last_connection = 0;
            }

            (வெறுமை) ngx_resolver_send_query(r, rn);

            rn->expire = now + r->resend_timeout;

            ngx_queue_செருகு_head(queue, q);

            தொடர்;
        }

        ngx_rbtree_delete(மரம், &rn->முனை);

        ngx_resolver_free_முனை(r, rn);
    }
}


நிலையான ngx_uint_t
ngx_resolver_resend_empty(ngx_resolver_t *r)
{
    திரும்பு ngx_queue_empty(&r->name_resend_queue)
           && ngx_queue_empty(&r->srv_resend_queue)
#எனில் (NGX_HAVE_INET6)
           && ngx_queue_empty(&r->addr6_resend_queue)
#endif
           && ngx_queue_empty(&r->addr_resend_queue);
}


நிலையான வெறுமை
ngx_resolver_udp_read(ngx_event_t *rev)
{
    ssize_t                     n;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;
    u_char                      buf[NGX_RESOLVER_UDP_SIZE];

    c = rev->data;
    rec = c->data;

    செய் {
        n = ngx_udp_recv(c, buf, NGX_RESOLVER_UDP_SIZE);

        எனில் (n == NGX_AGAIN) {
            நிறுத்து;
        }

        எனில் (n == NGX_ERROR) {
            செல் failed;
        }

        ngx_resolver_process_response(rec->resolver, buf, n, 0);

    } போது (rev->ready);

    எனில் (ngx_handle_read_event(rev, 0) != NGX_OK) {
        செல் failed;
    }

    திரும்பு;

failed:

    ngx_close_connection(rec->udp);
    rec->udp = வெற்று;
}


நிலையான வெறுமை
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

    எனில் (wev->timedout) {
        செல் failed;
    }

    sent = c->sent;

    போது (wev->ready && b->pos < b->last) {
        n = ngx_send(c, b->pos, b->last - b->pos);

        எனில் (n == NGX_AGAIN) {
            நிறுத்து;
        }

        எனில் (n == NGX_ERROR) {
            செல் failed;
        }

        b->pos += n;
    }

    எனில் (b->pos != b->start) {
        b->last = ngx_movemem(b->start, b->pos, b->last - b->pos);
        b->pos = b->start;
    }

    எனில் (c->sent != sent) {
        ngx_add_timer(wev, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    எனில் (ngx_handle_write_event(wev, 0) != NGX_OK) {
        செல் failed;
    }

    திரும்பு;

failed:

    ngx_close_connection(c);
    rec->tcp = வெற்று;
}


நிலையான வெறுமை
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

    போது (rev->ready) {
        n = ngx_recv(c, b->last, b->end - b->last);

        எனில் (n == NGX_AGAIN) {
            நிறுத்து;
        }

        எனில் (n == NGX_ERROR || n == 0) {
            செல் failed;
        }

        b->last += n;

        க்கு ( ;; ) {
            p = b->pos;
            size = b->last - p;

            எனில் (size < 2) {
                நிறுத்து;
            }

            qlen = (u_short) *p++ << 8;
            qlen += *p++;

            எனில் (size < (size_t) (2 + qlen)) {
                நிறுத்து;
            }

            ngx_resolver_process_response(r, p, qlen, 1);

            b->pos += 2 + qlen;
        }

        எனில் (b->pos != b->start) {
            b->last = ngx_movemem(b->start, b->pos, b->last - b->pos);
            b->pos = b->start;
        }
    }

    எனில் (ngx_handle_read_event(rev, 0) != NGX_OK) {
        செல் failed;
    }

    திரும்பு;

failed:

    ngx_close_connection(c);
    rec->tcp = வெற்று;
}


நிலையான வெறுமை
ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t tcp)
{
    எழுத்து                 *err;
    ngx_uint_t            i, times, ident, qident, flags, code, nqs, nan, trunc,
                          qtype, qclass;
#எனில் (NGX_HAVE_INET6)
    ngx_uint_t            qident6;
#endif
    ngx_queue_t          *q;
    ngx_resolver_qs_t    *qs;
    ngx_resolver_hdr_t   *response;
    ngx_resolver_முனை_t  *rn;

    எனில் (n < அளவு(ngx_resolver_hdr_t)) {
        செல் short_response;
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
    எனில் ((flags & 0xf870) != 0x8000 || (trunc && tcp)) {
        ngx_log_error(r->log_level, r->log, 0,
                      "invalid %s DNS response %ui fl:%04Xi",
                      tcp ? "TCP" : "UDP", ident, flags);
        திரும்பு;
    }

    code = flags & 0xf;

    எனில் (code == NGX_RESOLVE_FORMERR) {

        times = 0;

        க்கு (q = ngx_queue_head(&r->name_resend_queue);
             q != ngx_queue_sentinel(&r->name_resend_queue) && times++ < 100;
             q = ngx_queue_next(q))
        {
            rn = ngx_queue_data(q, ngx_resolver_முனை_t, queue);

            எனில் (rn->query) {
                qident = (rn->query[0] << 8) + rn->query[1];

                எனில் (qident == ident) {
                    செல் dns_error_name;
                }
            }

#எனில் (NGX_HAVE_INET6)
            எனில் (rn->query6) {
                qident6 = (rn->query6[0] << 8) + rn->query6[1];

                எனில் (qident6 == ident) {
                    செல் dns_error_name;
                }
            }
#endif
        }

        செல் dns_error;
    }

    எனில் (code > NGX_RESOLVE_REFUSED) {
        செல் dns_error;
    }

    எனில் (nqs != 1) {
        err = "invalid number of questions in DNS response";
        செல் done;
    }

    i = அளவு(ngx_resolver_hdr_t);

    போது (i < (ngx_uint_t) n) {

        எனில் (buf[i] & 0xc0) {
            err = "unexpected compression pointer in DNS response";
            செல் done;
        }

        எனில் (buf[i] == '\0') {
            செல் found;
        }

        i += 1 + buf[i];
    }

    செல் short_response;

found:

    எனில் (i++ == அளவு(ngx_resolver_hdr_t)) {
        err = "zero-length domain name in DNS response";
        செல் done;
    }

    எனில் (i + அளவு(ngx_resolver_qs_t) + nan * (2 + அளவு(ngx_resolver_an_t))
        > (ngx_uint_t) n)
    {
        செல் short_response;
    }

    qs = (ngx_resolver_qs_t *) &buf[i];

    qtype = (qs->type_hi << 8) + qs->type_lo;
    qclass = (qs->class_hi << 8) + qs->class_lo;

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver DNS response qt:%ui cl:%ui", qtype, qclass);

    எனில் (qclass != 1) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unknown query class %ui in DNS response", qclass);
        திரும்பு;
    }

    தேர்வு (qtype) {

    நிலை NGX_RESOLVE_A:
#எனில் (NGX_HAVE_INET6)
    நிலை NGX_RESOLVE_AAAA:
#endif

        ngx_resolver_process_a(r, buf, n, ident, code, qtype, nan, trunc,
                               i + அளவு(ngx_resolver_qs_t));

        நிறுத்து;

    நிலை NGX_RESOLVE_SRV:

        ngx_resolver_process_srv(r, buf, n, ident, code, nan, trunc,
                                 i + அளவு(ngx_resolver_qs_t));

        நிறுத்து;

    நிலை NGX_RESOLVE_PTR:

        ngx_resolver_process_ptr(r, buf, n, ident, code, nan);

        நிறுத்து;

    இயல்பு:
        ngx_log_error(r->log_level, r->log, 0,
                      "unknown query type %ui in DNS response", qtype);
        திரும்பு;
    }

    திரும்பு;

short_response:

    err = "short DNS response";

done:

    ngx_log_error(r->log_level, r->log, 0, err);

    திரும்பு;

dns_error_name:

    ngx_log_error(r->log_level, r->log, 0,
                  "DNS error (%ui: %s), query id:%ui, name:\"%*s\"",
                  code, ngx_resolver_strerror(code), ident,
                  (size_t) rn->nlen, rn->name);
    திரும்பு;

dns_error:

    ngx_log_error(r->log_level, r->log, 0,
                  "DNS error (%ui: %s), query id:%ui",
                  code, ngx_resolver_strerror(code), ident);
    திரும்பு;
}


நிலையான வெறுமை
ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans)
{
    எழுத்து                       *err;
    u_char                     *cname;
    size_t                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    in_addr_t                  *addr;
    ngx_str_t                   name;
    ngx_uint_t                  type, class, qident, naddrs, a, i, j, start;
#எனில் (NGX_HAVE_INET6)
    கட்டமைப்பு in6_addr            *addr6;
#endif
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *next;
    ngx_resolver_முனை_t        *rn;
    ngx_resolver_addr_t        *addrs;
    ngx_resolver_connection_t  *rec;

    எனில் (ngx_resolver_copy(r, &name, buf,
                          buf + அளவு(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        திரும்பு;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver qs:%V", &name);

    hash = ngx_crc32_short(name.data, name.len);

    /* lock name mutex */

    rn = ngx_resolver_lookup_name(r, &name, hash);

    எனில் (rn == வெற்று) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.data);
        செல் failed;
    }

    தேர்வு (qtype) {

#எனில் (NGX_HAVE_INET6)
    நிலை NGX_RESOLVE_AAAA:

        எனில் (rn->query6 == வெற்று || rn->naddrs6 != (u_short) -1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.data);
            செல் failed;
        }

        எனில் (trunc && rn->tcp6) {
            ngx_resolver_free(r, name.data);
            செல் failed;
        }

        qident = (rn->query6[0] << 8) + rn->query6[1];

        நிறுத்து;
#endif

    இயல்பு: /* NGX_RESOLVE_A */

        எனில் (rn->query == வெற்று || rn->naddrs != (u_short) -1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.data);
            செல் failed;
        }

        எனில் (trunc && rn->tcp) {
            ngx_resolver_free(r, name.data);
            செல் failed;
        }

        qident = (rn->query[0] << 8) + rn->query[1];
    }

    எனில் (ident != qident) {
        ngx_log_error(r->log_level, r->log, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.data);
        செல் failed;
    }

    ngx_resolver_free(r, name.data);

    எனில் (trunc) {

        ngx_queue_remove(&rn->queue);

        எனில் (rn->waiting == வெற்று) {
            ngx_rbtree_delete(&r->name_rbtree, &rn->முனை);
            ngx_resolver_free_முனை(r, rn);
            செல் next;
        }

        rec = r->connections.elts;
        rec = &rec[rn->last_connection];

        தேர்வு (qtype) {

#எனில் (NGX_HAVE_INET6)
        நிலை NGX_RESOLVE_AAAA:

            rn->tcp6 = 1;

            (வெறுமை) ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen);

            நிறுத்து;
#endif

        இயல்பு: /* NGX_RESOLVE_A */

            rn->tcp = 1;

            (வெறுமை) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);
        }

        rn->expire = ngx_time() + r->resend_timeout;

        ngx_queue_செருகு_head(&r->name_resend_queue, &rn->queue);

        செல் next;
    }

    எனில் (code == 0 && rn->code) {
        code = rn->code;
    }

    எனில் (code == 0 && nan == 0) {

#எனில் (NGX_HAVE_INET6)
        தேர்வு (qtype) {

        நிலை NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            எனில் (rn->naddrs == (u_short) -1) {
                செல் next;
            }

            எனில் (rn->naddrs) {
                செல் export;
            }

            நிறுத்து;

        இயல்பு: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            எனில் (rn->naddrs6 == (u_short) -1) {
                செல் next;
            }

            எனில் (rn->naddrs6) {
                செல் export;
            }
        }
#endif

        code = NGX_RESOLVE_NXDOMAIN;
    }

    எனில் (code) {

#எனில் (NGX_HAVE_INET6)
        தேர்வு (qtype) {

        நிலை NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            எனில் (rn->naddrs == (u_short) -1) {
                rn->code = (u_char) code;
                செல் next;
            }

            நிறுத்து;

        இயல்பு: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            எனில் (rn->naddrs6 == (u_short) -1) {
                rn->code = (u_char) code;
                செல் next;
            }
        }
#endif

        next = rn->waiting;
        rn->waiting = வெற்று;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(&r->name_rbtree, &rn->முனை);

        /* unlock name mutex */

        போது (next) {
            ctx = next;
            ctx->state = code;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            next = ctx->next;

            ctx->handler(ctx);
        }

        ngx_resolver_free_முனை(r, rn);

        திரும்பு;
    }

    i = ans;
    naddrs = 0;
    cname = வெற்று;

    க்கு (a = 0; a < nan; a++) {

        start = i;

        போது (i < n) {

            எனில் (buf[i] & 0xc0) {
                i += 2;
                செல் found;
            }

            எனில் (buf[i] == 0) {
                i++;
                செல் test_length;
            }

            i += 1 + buf[i];
        }

        செல் short_response;

    test_length:

        எனில் (i - start < 2) {
            err = "invalid name in DNS response";
            செல் invalid;
        }

    found:

        எனில் (i + அளவு(ngx_resolver_an_t) >= n) {
            செல் short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        எனில் (class != 1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR class %ui in DNS response", class);
            செல் failed;
        }

        எனில் (ttl < 0) {
            ttl = 0;
        }

        rn->ttl = ngx_min(rn->ttl, (uint32_t) ttl);

        i += அளவு(ngx_resolver_an_t);

        தேர்வு (type) {

        நிலை NGX_RESOLVE_A:

            எனில் (qtype != NGX_RESOLVE_A) {
                err = "unexpected A record in DNS response";
                செல் invalid;
            }

            எனில் (len != 4) {
                err = "invalid A record in DNS response";
                செல் invalid;
            }

            எனில் (i + 4 > n) {
                செல் short_response;
            }

            naddrs++;

            நிறுத்து;

#எனில் (NGX_HAVE_INET6)
        நிலை NGX_RESOLVE_AAAA:

            எனில் (qtype != NGX_RESOLVE_AAAA) {
                err = "unexpected AAAA record in DNS response";
                செல் invalid;
            }

            எனில் (len != 16) {
                err = "invalid AAAA record in DNS response";
                செல் invalid;
            }

            எனில் (i + 16 > n) {
                செல் short_response;
            }

            naddrs++;

            நிறுத்து;
#endif

        நிலை NGX_RESOLVE_CNAME:

            cname = &buf[i];

            நிறுத்து;

        நிலை NGX_RESOLVE_DNAME:

            நிறுத்து;

        இயல்பு:

            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver naddrs:%ui cname:%p ttl:%uD",
                   naddrs, cname, rn->ttl);

    எனில் (naddrs) {

        தேர்வு (qtype) {

#எனில் (NGX_HAVE_INET6)
        நிலை NGX_RESOLVE_AAAA:

            எனில் (naddrs == 1) {
                addr6 = &rn->u6.addr6;
                rn->naddrs6 = 1;

            } இல்லையெனில் {
                addr6 = ngx_resolver_alloc(r, naddrs * அளவு(கட்டமைப்பு in6_addr));
                எனில் (addr6 == வெற்று) {
                    செல் failed;
                }

                rn->u6.addrs6 = addr6;
                rn->naddrs6 = (u_short) naddrs;
            }

#எனில் (NGX_SUPPRESS_WARN)
            addr = வெற்று;
#endif

            நிறுத்து;
#endif

        இயல்பு: /* NGX_RESOLVE_A */

            எனில் (naddrs == 1) {
                addr = &rn->u.addr;
                rn->naddrs = 1;

            } இல்லையெனில் {
                addr = ngx_resolver_alloc(r, naddrs * அளவு(in_addr_t));
                எனில் (addr == வெற்று) {
                    செல் failed;
                }

                rn->u.addrs = addr;
                rn->naddrs = (u_short) naddrs;
            }

#எனில் (NGX_HAVE_INET6 && NGX_SUPPRESS_WARN)
            addr6 = வெற்று;
#endif
        }

        j = 0;
        i = ans;

        க்கு (a = 0; a < nan; a++) {

            க்கு ( ;; ) {

                எனில் (buf[i] & 0xc0) {
                    i += 2;
                    நிறுத்து;
                }

                எனில் (buf[i] == 0) {
                    i++;
                    நிறுத்து;
                }

                i += 1 + buf[i];
            }

            an = (ngx_resolver_an_t *) &buf[i];

            type = (an->type_hi << 8) + an->type_lo;
            len = (an->len_hi << 8) + an->len_lo;

            i += அளவு(ngx_resolver_an_t);

            எனில் (type == NGX_RESOLVE_A) {

                addr[j] = htonl((buf[i] << 24) + (buf[i + 1] << 16)
                                + (buf[i + 2] << 8) + (buf[i + 3]));

                எனில் (++j == naddrs) {

#எனில் (NGX_HAVE_INET6)
                    எனில் (rn->naddrs6 == (u_short) -1) {
                        செல் next;
                    }
#endif

                    நிறுத்து;
                }
            }

#எனில் (NGX_HAVE_INET6)
            இல்லையெனில் எனில் (type == NGX_RESOLVE_AAAA) {

                ngx_memcpy(addr6[j].s6_addr, &buf[i], 16);

                எனில் (++j == naddrs) {

                    எனில் (rn->naddrs == (u_short) -1) {
                        செல் next;
                    }

                    நிறுத்து;
                }
            }
#endif

            i += len;
        }
    }

    தேர்வு (qtype) {

#எனில் (NGX_HAVE_INET6)
    நிலை NGX_RESOLVE_AAAA:

        எனில் (rn->naddrs6 == (u_short) -1) {
            rn->naddrs6 = 0;
        }

        நிறுத்து;
#endif

    இயல்பு: /* NGX_RESOLVE_A */

        எனில் (rn->naddrs == (u_short) -1) {
            rn->naddrs = 0;
        }
    }

    எனில் (rn->naddrs != (u_short) -1
#எனில் (NGX_HAVE_INET6)
        && rn->naddrs6 != (u_short) -1
#endif
        && rn->naddrs
#எனில் (NGX_HAVE_INET6)
           + rn->naddrs6
#endif
           > 0)
    {

#எனில் (NGX_HAVE_INET6)
    export:
#endif

        naddrs = rn->naddrs;
#எனில் (NGX_HAVE_INET6)
        naddrs += rn->naddrs6;
#endif

        எனில் (naddrs == 1 && rn->naddrs == 1) {
            addrs = வெற்று;

        } இல்லையெனில் {
            addrs = ngx_resolver_export(r, rn, 0);
            எனில் (addrs == வெற்று) {
                செல் failed;
            }
        }

        ngx_queue_remove(&rn->queue);

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_செருகு_head(&r->name_expire_queue, &rn->queue);

        next = rn->waiting;
        rn->waiting = வெற்று;

        /* unlock name mutex */

        போது (next) {
            ctx = next;
            ctx->state = NGX_OK;
            ctx->valid = rn->valid;
            ctx->naddrs = naddrs;

            எனில் (addrs == வெற்று) {
                ctx->addrs = &ctx->addr;
                ctx->addr.sockaddr = (கட்டமைப்பு sockaddr *) &ctx->sin;
                ctx->addr.socklen = அளவு(கட்டமைப்பு sockaddr_in);
                ngx_memzero(&ctx->sin, அளவு(கட்டமைப்பு sockaddr_in));
                ctx->sin.sin_family = AF_INET;
                ctx->sin.sin_addr.s_addr = rn->u.addr;

            } இல்லையெனில் {
                ctx->addrs = addrs;
            }

            next = ctx->next;

            ctx->handler(ctx);
        }

        எனில் (addrs != வெற்று) {
            ngx_resolver_free(r, addrs->sockaddr);
            ngx_resolver_free(r, addrs);
        }

        ngx_resolver_free(r, rn->query);
        rn->query = வெற்று;
#எனில் (NGX_HAVE_INET6)
        rn->query6 = வெற்று;
#endif

        திரும்பு;
    }

    எனில் (cname) {

        /* CNAME only */

        எனில் (rn->naddrs == (u_short) -1
#எனில் (NGX_HAVE_INET6)
            || rn->naddrs6 == (u_short) -1
#endif
            )
        {
            செல் next;
        }

        எனில் (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            செல் failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_queue_remove(&rn->queue);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.data;

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_செருகு_head(&r->name_expire_queue, &rn->queue);

        ngx_resolver_free(r, rn->query);
        rn->query = வெற்று;
#எனில் (NGX_HAVE_INET6)
        rn->query6 = வெற்று;
#endif

        ctx = rn->waiting;
        rn->waiting = வெற்று;

        எனில் (ctx) {

            எனில் (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                செய் {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    next = ctx->next;

                    ctx->handler(ctx);

                    ctx = next;
                } போது (ctx);

                திரும்பு;
            }

            க்கு (next = ctx; next; next = next->next) {
                next->முனை = வெற்று;
            }

            (வெறுமை) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        திரும்பு;
    }

    ngx_log_error(r->log_level, r->log, 0,
                  "no A or CNAME types in DNS response");
    திரும்பு;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->log, 0, err);

    திரும்பு;

failed:

next:

    /* unlock name mutex */

    திரும்பு;
}


நிலையான வெறுமை
ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans)
{
    எழுத்து                       *err;
    u_char                     *cname;
    size_t                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    ngx_str_t                   name;
    ngx_uint_t                  type, qident, class, start, nsrvs, a, i, j;
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *next;
    ngx_resolver_srv_t         *srvs;
    ngx_resolver_முனை_t        *rn;
    ngx_resolver_connection_t  *rec;

    எனில் (ngx_resolver_copy(r, &name, buf,
                          buf + அளவு(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        திரும்பு;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver qs:%V", &name);

    hash = ngx_crc32_short(name.data, name.len);

    rn = ngx_resolver_lookup_srv(r, &name, hash);

    எனில் (rn == வெற்று || rn->query == வெற்று) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.data);
        செல் failed;
    }

    எனில் (trunc && rn->tcp) {
        ngx_resolver_free(r, name.data);
        செல் failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    எனில் (ident != qident) {
        ngx_log_error(r->log_level, r->log, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.data);
        செல் failed;
    }

    ngx_resolver_free(r, name.data);

    எனில் (trunc) {

        ngx_queue_remove(&rn->queue);

        எனில் (rn->waiting == வெற்று) {
            ngx_rbtree_delete(&r->srv_rbtree, &rn->முனை);
            ngx_resolver_free_முனை(r, rn);
            திரும்பு;
        }

        rec = r->connections.elts;
        rec = &rec[rn->last_connection];

        rn->tcp = 1;

        (வெறுமை) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);

        rn->expire = ngx_time() + r->resend_timeout;

        ngx_queue_செருகு_head(&r->srv_resend_queue, &rn->queue);

        திரும்பு;
    }

    எனில் (code == 0 && rn->code) {
        code = rn->code;
    }

    எனில் (code == 0 && nan == 0) {
        code = NGX_RESOLVE_NXDOMAIN;
    }

    எனில் (code) {
        next = rn->waiting;
        rn->waiting = வெற்று;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(&r->srv_rbtree, &rn->முனை);

        போது (next) {
            ctx = next;
            ctx->state = code;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            next = ctx->next;

            ctx->handler(ctx);
        }

        ngx_resolver_free_முனை(r, rn);

        திரும்பு;
    }

    i = ans;
    nsrvs = 0;
    cname = வெற்று;

    க்கு (a = 0; a < nan; a++) {

        start = i;

        போது (i < n) {

            எனில் (buf[i] & 0xc0) {
                i += 2;
                செல் found;
            }

            எனில் (buf[i] == 0) {
                i++;
                செல் test_length;
            }

            i += 1 + buf[i];
        }

        செல் short_response;

    test_length:

        எனில் (i - start < 2) {
            err = "invalid name DNS response";
            செல் invalid;
        }

    found:

        எனில் (i + அளவு(ngx_resolver_an_t) >= n) {
            செல் short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        எனில் (class != 1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR class %ui in DNS response", class);
            செல் failed;
        }

        எனில் (ttl < 0) {
            ttl = 0;
        }

        rn->ttl = ngx_min(rn->ttl, (uint32_t) ttl);

        i += அளவு(ngx_resolver_an_t);

        தேர்வு (type) {

        நிலை NGX_RESOLVE_SRV:

            எனில் (i + 6 >= n) {
                செல் short_response;
            }

            எனில் (ngx_resolver_copy(r, வெற்று, buf, &buf[i + 6], buf + n)
                != NGX_OK)
            {
                செல் failed;
            }

            nsrvs++;

            நிறுத்து;

        நிலை NGX_RESOLVE_CNAME:

            cname = &buf[i];

            நிறுத்து;

        நிலை NGX_RESOLVE_DNAME:

            நிறுத்து;

        இயல்பு:

            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver nsrvs:%ui cname:%p ttl:%uD",
                   nsrvs, cname, rn->ttl);

    எனில் (nsrvs) {

        srvs = ngx_resolver_calloc(r, nsrvs * அளவு(ngx_resolver_srv_t));
        எனில் (srvs == வெற்று) {
            செல் failed;
        }

        rn->u.srvs = srvs;
        rn->nsrvs = (u_short) nsrvs;

        j = 0;
        i = ans;

        க்கு (a = 0; a < nan; a++) {

            க்கு ( ;; ) {

                எனில் (buf[i] & 0xc0) {
                    i += 2;
                    நிறுத்து;
                }

                எனில் (buf[i] == 0) {
                    i++;
                    நிறுத்து;
                }

                i += 1 + buf[i];
            }

            an = (ngx_resolver_an_t *) &buf[i];

            type = (an->type_hi << 8) + an->type_lo;
            len = (an->len_hi << 8) + an->len_lo;

            i += அளவு(ngx_resolver_an_t);

            எனில் (type == NGX_RESOLVE_SRV) {

                srvs[j].priority = (buf[i] << 8) + buf[i + 1];
                srvs[j].weight = (buf[i + 2] << 8) + buf[i + 3];

                எனில் (srvs[j].weight == 0) {
                    srvs[j].weight = 1;
                }

                srvs[j].port = (buf[i + 4] << 8) + buf[i + 5];

                எனில் (ngx_resolver_copy(r, &srvs[j].name, buf, &buf[i + 6],
                                      buf + n)
                    != NGX_OK)
                {
                    செல் failed;
                }

                j++;
            }

            i += len;
        }

        ngx_sort(srvs, nsrvs, அளவு(ngx_resolver_srv_t),
                 ngx_resolver_cmp_srvs);

        ngx_resolver_free(r, rn->query);
        rn->query = வெற்று;

        ngx_queue_remove(&rn->queue);

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_செருகு_head(&r->srv_expire_queue, &rn->queue);

        next = rn->waiting;
        rn->waiting = வெற்று;

        போது (next) {
            ctx = next;
            next = ctx->next;

            ngx_resolver_resolve_srv_names(ctx, rn);
        }

        திரும்பு;
    }

    rn->nsrvs = 0;

    எனில் (cname) {

        /* CNAME only */

        எனில் (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            செல் failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_queue_remove(&rn->queue);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.data;

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_செருகு_head(&r->srv_expire_queue, &rn->queue);

        ngx_resolver_free(r, rn->query);
        rn->query = வெற்று;
#எனில் (NGX_HAVE_INET6)
        rn->query6 = வெற்று;
#endif

        ctx = rn->waiting;
        rn->waiting = வெற்று;

        எனில் (ctx) {

            எனில் (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                செய் {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    next = ctx->next;

                    ctx->handler(ctx);

                    ctx = next;
                } போது (ctx);

                திரும்பு;
            }

            க்கு (next = ctx; next; next = next->next) {
                next->முனை = வெற்று;
            }

            (வெறுமை) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        திரும்பு;
    }

    ngx_log_error(r->log_level, r->log, 0, "no SRV type in DNS response");

    திரும்பு;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->log, 0, err);

    திரும்பு;

failed:

    /* unlock name mutex */

    திரும்பு;
}


நிலையான வெறுமை
ngx_resolver_resolve_srv_names(ngx_resolver_ctx_t *ctx, ngx_resolver_முனை_t *rn)
{
    ngx_uint_t                i;
    ngx_resolver_t           *r;
    ngx_resolver_ctx_t       *cctx;
    ngx_resolver_srv_name_t  *srvs;

    r = ctx->resolver;

    ctx->முனை = வெற்று;
    ctx->state = NGX_OK;
    ctx->valid = rn->valid;
    ctx->மொத்தம் = rn->nsrvs;

    srvs = ngx_resolver_calloc(r, rn->nsrvs * அளவு(ngx_resolver_srv_name_t));
    எனில் (srvs == வெற்று) {
        செல் failed;
    }

    ctx->srvs = srvs;
    ctx->nsrvs = rn->nsrvs;

    எனில் (ctx->event && ctx->event->timer_set) {
        ngx_del_timer(ctx->event);
    }

    க்கு (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
        srvs[i].name.data = ngx_resolver_alloc(r, rn->u.srvs[i].name.len);
        எனில் (srvs[i].name.data == வெற்று) {
            செல் failed;
        }

        srvs[i].name.len = rn->u.srvs[i].name.len;
        ngx_memcpy(srvs[i].name.data, rn->u.srvs[i].name.data,
                   srvs[i].name.len);

        cctx = ngx_resolve_start(r, வெற்று);
        எனில் (cctx == வெற்று) {
            செல் failed;
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

        எனில் (ngx_resolve_name(cctx) == NGX_ERROR) {
            srvs[i].ctx = வெற்று;
            செல் failed;
        }
    }

    திரும்பு;

failed:

    ctx->state = NGX_ERROR;
    ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

    ctx->handler(ctx);
}


நிலையான வெறுமை
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

    ctx->மொத்தம்--;
    ctx->async |= cctx->async;

    srv->ctx = வெற்று;
    srv->state = cctx->state;

    எனில் (cctx->naddrs) {

        ctx->valid = ngx_min(ctx->valid, cctx->valid);

        addrs = ngx_resolver_calloc(r, cctx->naddrs * அளவு(ngx_addr_t));
        எனில் (addrs == வெற்று) {
            srv->state = NGX_ERROR;
            செல் done;
        }

        sockaddr = ngx_resolver_alloc(r, cctx->naddrs * அளவு(ngx_sockaddr_t));
        எனில் (sockaddr == வெற்று) {
            ngx_resolver_free(r, addrs);
            srv->state = NGX_ERROR;
            செல் done;
        }

        க்கு (i = 0; i < cctx->naddrs; i++) {
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

    எனில் (ctx->மொத்தம் == 0) {
        ngx_resolver_report_srv(r, ctx);
    }
}


நிலையான வெறுமை
ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan)
{
    எழுத்து                 *err;
    size_t                len;
    in_addr_t             addr;
    int32_t               ttl;
    ngx_int_t             octet;
    ngx_str_t             name;
    ngx_uint_t            mask, type, class, qident, a, i, start;
    ngx_queue_t          *expire_queue;
    ngx_rbtree_t         *மரம்;
    ngx_resolver_an_t    *an;
    ngx_resolver_ctx_t   *ctx, *next;
    ngx_resolver_முனை_t  *rn;
#எனில் (NGX_HAVE_INET6)
    uint32_t              hash;
    ngx_int_t             digit;
    கட்டமைப்பு in6_addr       addr6;
#endif

    எனில் (ngx_resolver_copy(r, &name, buf,
                          buf + அளவு(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        திரும்பு;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver qs:%V", &name);

    /* AF_INET */

    addr = 0;
    i = அளவு(ngx_resolver_hdr_t);

    க்கு (mask = 0; mask < 32; mask += 8) {
        len = buf[i++];

        octet = ngx_atoi(&buf[i], len);
        எனில் (octet == NGX_ERROR || octet > 255) {
            செல் invalid_in_addr_arpa;
        }

        addr += octet << mask;
        i += len;
    }

    எனில் (ngx_strcasecmp(&buf[i], (u_char *) "\7in-addr\4arpa") == 0) {
        i += அளவு("\7in-addr\4arpa");

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        மரம் = &r->addr_rbtree;
        expire_queue = &r->addr_expire_queue;

        செல் valid;
    }

invalid_in_addr_arpa:

#எனில் (NGX_HAVE_INET6)

    i = அளவு(ngx_resolver_hdr_t);

    க்கு (octet = 15; octet >= 0; octet--) {
        எனில் (buf[i++] != '\1') {
            செல் invalid_ip6_arpa;
        }

        digit = ngx_hextoi(&buf[i++], 1);
        எனில் (digit == NGX_ERROR) {
            செல் invalid_ip6_arpa;
        }

        addr6.s6_addr[octet] = (u_char) digit;

        எனில் (buf[i++] != '\1') {
            செல் invalid_ip6_arpa;
        }

        digit = ngx_hextoi(&buf[i++], 1);
        எனில் (digit == NGX_ERROR) {
            செல் invalid_ip6_arpa;
        }

        addr6.s6_addr[octet] += (u_char) (digit * 16);
    }

    எனில் (ngx_strcasecmp(&buf[i], (u_char *) "\3ip6\4arpa") == 0) {
        i += அளவு("\3ip6\4arpa");

        /* lock addr mutex */

        hash = ngx_crc32_short(addr6.s6_addr, 16);
        rn = ngx_resolver_lookup_addr6(r, &addr6, hash);

        மரம் = &r->addr6_rbtree;
        expire_queue = &r->addr6_expire_queue;

        செல் valid;
    }

invalid_ip6_arpa:
#endif

    ngx_log_error(r->log_level, r->log, 0,
                  "invalid in-addr.arpa or ip6.arpa name in DNS response");
    ngx_resolver_free(r, name.data);
    திரும்பு;

valid:

    எனில் (rn == வெற்று || rn->query == வெற்று) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.data);
        செல் failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    எனில் (ident != qident) {
        ngx_log_error(r->log_level, r->log, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.data);
        செல் failed;
    }

    ngx_resolver_free(r, name.data);

    எனில் (code == 0 && nan == 0) {
        code = NGX_RESOLVE_NXDOMAIN;
    }

    எனில் (code) {
        next = rn->waiting;
        rn->waiting = வெற்று;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(மரம், &rn->முனை);

        /* unlock addr mutex */

        போது (next) {
            ctx = next;
            ctx->state = code;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            next = ctx->next;

            ctx->handler(ctx);
        }

        ngx_resolver_free_முனை(r, rn);

        திரும்பு;
    }

    i += அளவு(ngx_resolver_qs_t);

    க்கு (a = 0; a < nan; a++) {

        start = i;

        போது (i < n) {

            எனில் (buf[i] & 0xc0) {
                i += 2;
                செல் found;
            }

            எனில் (buf[i] == 0) {
                i++;
                செல் test_length;
            }

            i += 1 + buf[i];
        }

        செல் short_response;

    test_length:

        எனில் (i - start < 2) {
            err = "invalid name in DNS response";
            செல் invalid;
        }

    found:

        எனில் (i + அளவு(ngx_resolver_an_t) >= n) {
            செல் short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        எனில் (class != 1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR class %ui in DNS response", class);
            செல் failed;
        }

        எனில் (ttl < 0) {
            ttl = 0;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                      "resolver qt:%ui cl:%ui len:%uz",
                      type, class, len);

        i += அளவு(ngx_resolver_an_t);

        தேர்வு (type) {

        நிலை NGX_RESOLVE_PTR:

            செல் ptr;

        நிலை NGX_RESOLVE_CNAME:

            நிறுத்து;

        இயல்பு:

            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->log, 0,
                  "no PTR type in DNS response");
    திரும்பு;

ptr:

    எனில் (ngx_resolver_copy(r, &name, buf, buf + i, buf + n) != NGX_OK) {
        செல் failed;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver an:%V", &name);

    எனில் (name.len != (size_t) rn->nlen
        || ngx_strncmp(name.data, rn->name, name.len) != 0)
    {
        எனில் (rn->nlen) {
            ngx_resolver_free(r, rn->name);
        }

        rn->nlen = (u_short) name.len;
        rn->name = name.data;

        name.data = ngx_resolver_dup(r, rn->name, name.len);
        எனில் (name.data == வெற்று) {
            செல் failed;
        }
    }

    ngx_queue_remove(&rn->queue);

    rn->valid = ngx_time() + (r->valid ? r->valid : ttl);
    rn->expire = ngx_time() + r->expire;

    ngx_queue_செருகு_head(expire_queue, &rn->queue);

    next = rn->waiting;
    rn->waiting = வெற்று;

    /* unlock addr mutex */

    போது (next) {
        ctx = next;
        ctx->state = NGX_OK;
        ctx->valid = rn->valid;
        ctx->name = name;
        next = ctx->next;

        ctx->handler(ctx);
    }

    ngx_resolver_free(r, name.data);

    திரும்பு;

short_response:

    err = "short DNS response";

invalid:

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->log, 0, err);

    திரும்பு;

failed:

    /* unlock addr mutex */

    திரும்பு;
}


நிலையான ngx_resolver_முனை_t *
ngx_resolver_lookup_name(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_முனை_t    *முனை, *sentinel;
    ngx_resolver_முனை_t  *rn;

    முனை = r->name_rbtree.வேர்;
    sentinel = r->name_rbtree.sentinel;

    போது (முனை != sentinel) {

        எனில் (hash < முனை->சாவி) {
            முனை = முனை->இடது;
            தொடர்;
        }

        எனில் (hash > முனை->சாவி) {
            முனை = முனை->வலது;
            தொடர்;
        }

        /* hash == node->key */

        rn = ngx_resolver_முனை(முனை);

        rc = ngx_memn2cmp(name->data, rn->name, name->len, rn->nlen);

        எனில் (rc == 0) {
            திரும்பு rn;
        }

        முனை = (rc < 0) ? முனை->இடது : முனை->வலது;
    }

    /* not found */

    திரும்பு வெற்று;
}


நிலையான ngx_resolver_முனை_t *
ngx_resolver_lookup_srv(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_முனை_t    *முனை, *sentinel;
    ngx_resolver_முனை_t  *rn;

    முனை = r->srv_rbtree.வேர்;
    sentinel = r->srv_rbtree.sentinel;

    போது (முனை != sentinel) {

        எனில் (hash < முனை->சாவி) {
            முனை = முனை->இடது;
            தொடர்;
        }

        எனில் (hash > முனை->சாவி) {
            முனை = முனை->வலது;
            தொடர்;
        }

        /* hash == node->key */

        rn = ngx_resolver_முனை(முனை);

        rc = ngx_memn2cmp(name->data, rn->name, name->len, rn->nlen);

        எனில் (rc == 0) {
            திரும்பு rn;
        }

        முனை = (rc < 0) ? முனை->இடது : முனை->வலது;
    }

    /* not found */

    திரும்பு வெற்று;
}


நிலையான ngx_resolver_முனை_t *
ngx_resolver_lookup_addr(ngx_resolver_t *r, in_addr_t addr)
{
    ngx_rbtree_முனை_t  *முனை, *sentinel;

    முனை = r->addr_rbtree.வேர்;
    sentinel = r->addr_rbtree.sentinel;

    போது (முனை != sentinel) {

        எனில் (addr < முனை->சாவி) {
            முனை = முனை->இடது;
            தொடர்;
        }

        எனில் (addr > முனை->சாவி) {
            முனை = முனை->வலது;
            தொடர்;
        }

        /* addr == node->key */

        திரும்பு ngx_resolver_முனை(முனை);
    }

    /* not found */

    திரும்பு வெற்று;
}


#எனில் (NGX_HAVE_INET6)

நிலையான ngx_resolver_முனை_t *
ngx_resolver_lookup_addr6(ngx_resolver_t *r, கட்டமைப்பு in6_addr *addr,
    uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_முனை_t    *முனை, *sentinel;
    ngx_resolver_முனை_t  *rn;

    முனை = r->addr6_rbtree.வேர்;
    sentinel = r->addr6_rbtree.sentinel;

    போது (முனை != sentinel) {

        எனில் (hash < முனை->சாவி) {
            முனை = முனை->இடது;
            தொடர்;
        }

        எனில் (hash > முனை->சாவி) {
            முனை = முனை->வலது;
            தொடர்;
        }

        /* hash == node->key */

        rn = ngx_resolver_முனை(முனை);

        rc = ngx_memcmp(addr, &rn->addr6, 16);

        எனில் (rc == 0) {
            திரும்பு rn;
        }

        முனை = (rc < 0) ? முனை->இடது : முனை->வலது;
    }

    /* not found */

    திரும்பு வெற்று;
}

#endif


நிலையான வெறுமை
ngx_resolver_rbtree_செருகு_மதிப்பு(ngx_rbtree_முனை_t *temp,
    ngx_rbtree_முனை_t *முனை, ngx_rbtree_முனை_t *sentinel)
{
    ngx_rbtree_முனை_t    **p;
    ngx_resolver_முனை_t   *rn, *rn_temp;

    க்கு ( ;; ) {

        எனில் (முனை->சாவி < temp->சாவி) {

            p = &temp->இடது;

        } இல்லையெனில் எனில் (முனை->சாவி > temp->சாவி) {

            p = &temp->வலது;

        } இல்லையெனில் { /* node->key == temp->key */

            rn = ngx_resolver_முனை(முனை);
            rn_temp = ngx_resolver_முனை(temp);

            p = (ngx_memn2cmp(rn->name, rn_temp->name, rn->nlen, rn_temp->nlen)
                 < 0) ? &temp->இடது : &temp->வலது;
        }

        எனில் (*p == sentinel) {
            நிறுத்து;
        }

        temp = *p;
    }

    *p = முனை;
    முனை->parent = temp;
    முனை->இடது = sentinel;
    முனை->வலது = sentinel;
    ngx_rbt_red(முனை);
}


#எனில் (NGX_HAVE_INET6)

நிலையான வெறுமை
ngx_resolver_rbtree_செருகு_addr6_மதிப்பு(ngx_rbtree_முனை_t *temp,
    ngx_rbtree_முனை_t *முனை, ngx_rbtree_முனை_t *sentinel)
{
    ngx_rbtree_முனை_t    **p;
    ngx_resolver_முனை_t   *rn, *rn_temp;

    க்கு ( ;; ) {

        எனில் (முனை->சாவி < temp->சாவி) {

            p = &temp->இடது;

        } இல்லையெனில் எனில் (முனை->சாவி > temp->சாவி) {

            p = &temp->வலது;

        } இல்லையெனில் { /* node->key == temp->key */

            rn = ngx_resolver_முனை(முனை);
            rn_temp = ngx_resolver_முனை(temp);

            p = (ngx_memcmp(&rn->addr6, &rn_temp->addr6, 16)
                 < 0) ? &temp->இடது : &temp->வலது;
        }

        எனில் (*p == sentinel) {
            நிறுத்து;
        }

        temp = *p;
    }

    *p = முனை;
    முனை->parent = temp;
    முனை->இடது = sentinel;
    முனை->வலது = sentinel;
    ngx_rbt_red(முனை);
}

#endif


நிலையான ngx_int_t
ngx_resolver_create_name_query(ngx_resolver_t *r, ngx_resolver_முனை_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    size_t               len, nlen;
    ngx_uint_t           ident;
    ngx_resolver_qs_t   *qs;
    ngx_resolver_hdr_t  *query;

    nlen = name->len ? (1 + name->len + 1) : 1;

    len = அளவு(ngx_resolver_hdr_t) + nlen + அளவு(ngx_resolver_qs_t);

#எனில் (NGX_HAVE_INET6)
    p = ngx_resolver_alloc(r, len * (r->ipv4 + r->ipv6));
#இல்லையெனில்
    p = ngx_resolver_alloc(r, len);
#endif
    எனில் (p == வெற்று) {
        திரும்பு NGX_ERROR;
    }

    rn->qlen = (u_short) len;
    rn->query = p;

#எனில் (NGX_HAVE_INET6)
    எனில் (r->ipv6) {
        rn->query6 = r->ipv4 ? (p + len) : p;
    }
#endif

    query = (ngx_resolver_hdr_t *) p;

    எனில் (r->ipv4) {
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

    p += அளவு(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    /* query type */
    qs->type_hi = 0; qs->type_lo = NGX_RESOLVE_A;

    /* IN query class */
    qs->class_hi = 0; qs->class_lo = 1;

    /* convert "www.example.com" to "\3www\7example\3com\0" */

    len = 0;
    p--;
    *p-- = '\0';

    எனில் (name->len == 0)  {
        திரும்பு NGX_DECLINED;
    }

    க்கு (s = name->data + name->len - 1; s >= name->data; s--) {
        எனில் (*s != '.') {
            *p = *s;
            len++;

        } இல்லையெனில் {
            எனில் (len == 0 || len > 255) {
                திரும்பு NGX_DECLINED;
            }

            *p = (u_char) len;
            len = 0;
        }

        p--;
    }

    எனில் (len == 0 || len > 255) {
        திரும்பு NGX_DECLINED;
    }

    *p = (u_char) len;

#எனில் (NGX_HAVE_INET6)
    எனில் (!r->ipv6) {
        திரும்பு NGX_OK;
    }

    p = rn->query6;

    எனில் (r->ipv4) {
        ngx_memcpy(p, rn->query, rn->qlen);
    }

    query = (ngx_resolver_hdr_t *) p;

    ident = ngx_random();

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve: \"%V\" AAAA %i", name, ident & 0xffff);

    query->ident_hi = (u_char) ((ident >> 8) & 0xff);
    query->ident_lo = (u_char) (ident & 0xff);

    p += அளவு(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    qs->type_lo = NGX_RESOLVE_AAAA;
#endif

    திரும்பு NGX_OK;
}


நிலையான ngx_int_t
ngx_resolver_create_srv_query(ngx_resolver_t *r, ngx_resolver_முனை_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    size_t               len, nlen;
    ngx_uint_t           ident;
    ngx_resolver_qs_t   *qs;
    ngx_resolver_hdr_t  *query;

    nlen = name->len ? (1 + name->len + 1) : 1;

    len = அளவு(ngx_resolver_hdr_t) + nlen + அளவு(ngx_resolver_qs_t);

    p = ngx_resolver_alloc(r, len);
    எனில் (p == வெற்று) {
        திரும்பு NGX_ERROR;
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

    p += அளவு(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    /* query type */
    qs->type_hi = 0; qs->type_lo = NGX_RESOLVE_SRV;

    /* IN query class */
    qs->class_hi = 0; qs->class_lo = 1;

    /* converts "www.example.com" to "\3www\7example\3com\0" */

    len = 0;
    p--;
    *p-- = '\0';

    எனில் (name->len == 0)  {
        திரும்பு NGX_DECLINED;
    }

    க்கு (s = name->data + name->len - 1; s >= name->data; s--) {
        எனில் (*s != '.') {
            *p = *s;
            len++;

        } இல்லையெனில் {
            எனில் (len == 0 || len > 255) {
                திரும்பு NGX_DECLINED;
            }

            *p = (u_char) len;
            len = 0;
        }

        p--;
    }

    எனில் (len == 0 || len > 255) {
        திரும்பு NGX_DECLINED;
    }

    *p = (u_char) len;

    திரும்பு NGX_OK;
}


நிலையான ngx_int_t
ngx_resolver_create_addr_query(ngx_resolver_t *r, ngx_resolver_முனை_t *rn,
    ngx_resolver_addr_t *addr)
{
    u_char               *p, *d;
    size_t                len;
    in_addr_t             inaddr;
    ngx_int_t             n;
    ngx_uint_t            ident;
    ngx_resolver_hdr_t   *query;
    கட்டமைப்பு sockaddr_in   *sin;
#எனில் (NGX_HAVE_INET6)
    கட்டமைப்பு sockaddr_in6  *sin6;
#endif

    தேர்வு (addr->sockaddr->sa_family) {

#எனில் (NGX_HAVE_INET6)
    நிலை AF_INET6:
        len = அளவு(ngx_resolver_hdr_t)
              + 64 + அளவு(".ip6.arpa.") - 1
              + அளவு(ngx_resolver_qs_t);

        நிறுத்து;
#endif

    இயல்பு: /* AF_INET */
        len = அளவு(ngx_resolver_hdr_t)
              + அளவு(".255.255.255.255.in-addr.arpa.") - 1
              + அளவு(ngx_resolver_qs_t);
    }

    p = ngx_resolver_alloc(r, len);
    எனில் (p == வெற்று) {
        திரும்பு NGX_ERROR;
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

    p += அளவு(ngx_resolver_hdr_t);

    தேர்வு (addr->sockaddr->sa_family) {

#எனில் (NGX_HAVE_INET6)
    நிலை AF_INET6:
        sin6 = (கட்டமைப்பு sockaddr_in6 *) addr->sockaddr;

        க்கு (n = 15; n >= 0; n--) {
            p = ngx_sprintf(p, "\1%xd\1%xd",
                            sin6->sin6_addr.s6_addr[n] & 0xf,
                            (sin6->sin6_addr.s6_addr[n] >> 4) & 0xf);
        }

        p = ngx_cpymem(p, "\3ip6\4arpa\0", 10);

        நிறுத்து;
#endif

    இயல்பு: /* AF_INET */

        sin = (கட்டமைப்பு sockaddr_in *) addr->sockaddr;
        inaddr = ntohl(sin->sin_addr.s_addr);

        க்கு (n = 0; n < 32; n += 8) {
            d = ngx_sprintf(&p[1], "%ud", (inaddr >> n) & 0xff);
            *p = (u_char) (d - &p[1]);
            p = d;
        }

        p = ngx_cpymem(p, "\7in-addr\4arpa\0", 14);
    }

    /* query type "PTR", IN query class */
    p = ngx_cpymem(p, "\0\14\0\1", 4);

    rn->qlen = (u_short) (p - rn->query);

    திரும்பு NGX_OK;
}


நிலையான ngx_int_t
ngx_resolver_copy(ngx_resolver_t *r, ngx_str_t *name, u_char *buf, u_char *src,
    u_char *last)
{
    எழுத்து        *err;
    u_char      *p, *dst;
    size_t       len;
    ngx_uint_t   i, n;

    p = src;
    len = 0;

    /*
     * compression pointers allow to create endless loop, so we set limit;
     * 128 pointers should be enough to store 255-byte name
     */

    க்கு (i = 0; i < 128; i++) {
        n = *p++;

        எனில் (n == 0) {
            செல் done;
        }

        எனில் (n & 0xc0) {
            எனில் ((n & 0xc0) != 0xc0) {
                err = "invalid label type in DNS response";
                செல் invalid;
            }

            எனில் (p >= last) {
                err = "name is out of DNS response";
                செல் invalid;
            }

            n = ((n & 0x3f) << 8) + *p;
            p = &buf[n];

        } இல்லையெனில் {
            len += 1 + n;
            p = &p[n];
        }

        எனில் (p >= last) {
            err = "name is out of DNS response";
            செல் invalid;
        }
    }

    err = "compression pointers loop in DNS response";

invalid:

    ngx_log_error(r->log_level, r->log, 0, err);

    திரும்பு NGX_ERROR;

done:

    எனில் (name == வெற்று) {
        திரும்பு NGX_OK;
    }

    எனில் (len == 0) {
        ngx_str_null(name);
        திரும்பு NGX_OK;
    }

    dst = ngx_resolver_alloc(r, len);
    எனில் (dst == வெற்று) {
        திரும்பு NGX_ERROR;
    }

    name->data = dst;

    க்கு ( ;; ) {
        n = *src++;

        எனில் (n == 0) {
            name->len = dst - name->data - 1;
            திரும்பு NGX_OK;
        }

        எனில் (n & 0xc0) {
            n = ((n & 0x3f) << 8) + *src;
            src = &buf[n];

        } இல்லையெனில் {
            ngx_strlow(dst, src, n);
            dst += n;
            src += n;
            *dst++ = '.';
        }
    }
}


நிலையான ngx_int_t
ngx_resolver_set_timeout(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx)
{
    எனில் (ctx->event || ctx->timeout == 0) {
        திரும்பு NGX_OK;
    }

    ctx->event = ngx_resolver_calloc(r, அளவு(ngx_event_t));
    எனில் (ctx->event == வெற்று) {
        திரும்பு NGX_ERROR;
    }

    ctx->event->handler = ngx_resolver_timeout_handler;
    ctx->event->data = ctx;
    ctx->event->log = r->log;
    ctx->event->cancelable = ctx->cancelable;
    ctx->ident = -1;

    ngx_add_timer(ctx->event, ctx->timeout);

    திரும்பு NGX_OK;
}


நிலையான வெறுமை
ngx_resolver_timeout_handler(ngx_event_t *ev)
{
    ngx_resolver_ctx_t  *ctx;

    ctx = ev->data;

    ctx->state = NGX_RESOLVE_TIMEDOUT;

    ctx->handler(ctx);
}


நிலையான வெறுமை
ngx_resolver_free_முனை(ngx_resolver_t *r, ngx_resolver_முனை_t *rn)
{
    ngx_uint_t  i;

    /* lock alloc mutex */

    எனில் (rn->query) {
        ngx_resolver_free_locked(r, rn->query);
    }

    எனில் (rn->name) {
        ngx_resolver_free_locked(r, rn->name);
    }

    எனில் (rn->cnlen) {
        ngx_resolver_free_locked(r, rn->u.cname);
    }

    எனில் (rn->naddrs > 1 && rn->naddrs != (u_short) -1) {
        ngx_resolver_free_locked(r, rn->u.addrs);
    }

#எனில் (NGX_HAVE_INET6)
    எனில் (rn->naddrs6 > 1 && rn->naddrs6 != (u_short) -1) {
        ngx_resolver_free_locked(r, rn->u6.addrs6);
    }
#endif

    எனில் (rn->nsrvs) {
        க்கு (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
            எனில் (rn->u.srvs[i].name.data) {
                ngx_resolver_free_locked(r, rn->u.srvs[i].name.data);
            }
        }

        ngx_resolver_free_locked(r, rn->u.srvs);
    }

    ngx_resolver_free_locked(r, rn);

    /* unlock alloc mutex */
}


நிலையான வெறுமை *
ngx_resolver_alloc(ngx_resolver_t *r, size_t size)
{
    u_char  *p;

    /* lock alloc mutex */

    p = ngx_alloc(size, r->log);

    /* unlock alloc mutex */

    திரும்பு p;
}


நிலையான வெறுமை *
ngx_resolver_calloc(ngx_resolver_t *r, size_t size)
{
    u_char  *p;

    p = ngx_resolver_alloc(r, size);

    எனில் (p) {
        ngx_memzero(p, size);
    }

    திரும்பு p;
}


நிலையான வெறுமை
ngx_resolver_free(ngx_resolver_t *r, வெறுமை *p)
{
    /* lock alloc mutex */

    ngx_free(p);

    /* unlock alloc mutex */
}


நிலையான வெறுமை
ngx_resolver_free_locked(ngx_resolver_t *r, வெறுமை *p)
{
    ngx_free(p);
}


நிலையான வெறுமை *
ngx_resolver_dup(ngx_resolver_t *r, வெறுமை *src, size_t size)
{
    வெறுமை  *dst;

    dst = ngx_resolver_alloc(r, size);

    எனில் (dst == வெற்று) {
        திரும்பு dst;
    }

    ngx_memcpy(dst, src, size);

    திரும்பு dst;
}


நிலையான ngx_resolver_addr_t *
ngx_resolver_export(ngx_resolver_t *r, ngx_resolver_முனை_t *rn,
    ngx_uint_t rotate)
{
    ngx_uint_t            d, i, j, n;
    in_addr_t            *addr;
    ngx_sockaddr_t       *sockaddr;
    கட்டமைப்பு sockaddr_in   *sin;
    ngx_resolver_addr_t  *dst;
#எனில் (NGX_HAVE_INET6)
    கட்டமைப்பு in6_addr      *addr6;
    கட்டமைப்பு sockaddr_in6  *sin6;
#endif

    n = rn->naddrs;
#எனில் (NGX_HAVE_INET6)
    n += rn->naddrs6;
#endif

    dst = ngx_resolver_calloc(r, n * அளவு(ngx_resolver_addr_t));
    எனில் (dst == வெற்று) {
        திரும்பு வெற்று;
    }

    sockaddr = ngx_resolver_calloc(r, n * அளவு(ngx_sockaddr_t));
    எனில் (sockaddr == வெற்று) {
        ngx_resolver_free(r, dst);
        திரும்பு வெற்று;
    }

    i = 0;
    d = rotate ? ngx_random() % n : 0;

    எனில் (rn->naddrs) {
        j = rotate ? ngx_random() % rn->naddrs : 0;

        addr = (rn->naddrs == 1) ? &rn->u.addr : rn->u.addrs;

        செய் {
            sin = &sockaddr[d].sockaddr_in;
            sin->sin_family = AF_INET;
            sin->sin_addr.s_addr = addr[j++];
            dst[d].sockaddr = (கட்டமைப்பு sockaddr *) sin;
            dst[d++].socklen = அளவு(கட்டமைப்பு sockaddr_in);

            எனில் (d == n) {
                d = 0;
            }

            எனில் (j == (ngx_uint_t) rn->naddrs) {
                j = 0;
            }
        } போது (++i < (ngx_uint_t) rn->naddrs);
    }

#எனில் (NGX_HAVE_INET6)
    எனில் (rn->naddrs6) {
        j = rotate ? ngx_random() % rn->naddrs6 : 0;

        addr6 = (rn->naddrs6 == 1) ? &rn->u6.addr6 : rn->u6.addrs6;

        செய் {
            sin6 = &sockaddr[d].sockaddr_in6;
            sin6->sin6_family = AF_INET6;
            ngx_memcpy(sin6->sin6_addr.s6_addr, addr6[j++].s6_addr, 16);
            dst[d].sockaddr = (கட்டமைப்பு sockaddr *) sin6;
            dst[d++].socklen = அளவு(கட்டமைப்பு sockaddr_in6);

            எனில் (d == n) {
                d = 0;
            }

            எனில் (j == rn->naddrs6) {
                j = 0;
            }
        } போது (++i < n);
    }
#endif

    திரும்பு dst;
}


நிலையான வெறுமை
ngx_resolver_report_srv(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx)
{
    ngx_uint_t                naddrs, nsrvs, nw, i, j, k, l, m, n, w;
    ngx_resolver_addr_t      *addrs;
    ngx_resolver_srv_name_t  *srvs;

    srvs = ctx->srvs;
    nsrvs = ctx->nsrvs;

    naddrs = 0;

    க்கு (i = 0; i < nsrvs; i++) {
        எனில் (srvs[i].state == NGX_ERROR) {
            ctx->state = NGX_ERROR;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

            ctx->handler(ctx);
            திரும்பு;
        }

        naddrs += srvs[i].naddrs;
    }

    எனில் (naddrs == 0) {
        ctx->state = srvs[0].state;

        க்கு (i = 0; i < nsrvs; i++) {
            எனில் (srvs[i].state == NGX_RESOLVE_NXDOMAIN) {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                நிறுத்து;
            }
        }

        ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        திரும்பு;
    }

    addrs = ngx_resolver_calloc(r, naddrs * அளவு(ngx_resolver_addr_t));
    எனில் (addrs == வெற்று) {
        ctx->state = NGX_ERROR;
        ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        திரும்பு;
    }

    i = 0;
    n = 0;

    செய் {
        nw = 0;

        க்கு (j = i; j < nsrvs; j++) {
            எனில் (srvs[j].priority != srvs[i].priority) {
                நிறுத்து;
            }

            nw += srvs[j].naddrs * srvs[j].weight;
        }

        எனில் (nw == 0) {
            செல் next_srv;
        }

        w = ngx_random() % nw;

        க்கு (k = i; k < j; k++) {
            எனில் (w < srvs[k].naddrs * srvs[k].weight) {
                நிறுத்து;
            }

            w -= srvs[k].naddrs * srvs[k].weight;
        }

        க்கு (l = i; l < j; l++) {

            க்கு (m = 0; m < srvs[k].naddrs; m++) {
                addrs[n].socklen = srvs[k].addrs[m].socklen;
                addrs[n].sockaddr = srvs[k].addrs[m].sockaddr;
                addrs[n].name = srvs[k].name;
                addrs[n].priority = srvs[k].priority;
                addrs[n].weight = srvs[k].weight;
                n++;
            }

            எனில் (++k == j) {
                k = i;
            }
        }

next_srv:

        i = j;

    } போது (i < ctx->nsrvs);

    ctx->state = NGX_OK;
    ctx->addrs = addrs;
    ctx->naddrs = naddrs;

    ctx->handler(ctx);

    ngx_resolver_free(r, addrs);
}


எழுத்து *
ngx_resolver_strerror(ngx_int_t err)
{
    நிலையான எழுத்து *errors[] = {
        "Format error",     /* FORMERR */
        "Server failure",   /* SERVFAIL */
        "Host not found",   /* NXDOMAIN */
        "Unimplemented",    /* NOTIMP */
        "Operation refused" /* REFUSED */
    };

    எனில் (err > 0 && err < 6) {
        திரும்பு errors[err - 1];
    }

    எனில் (err == NGX_RESOLVE_TIMEDOUT) {
        திரும்பு "Operation timed out";
    }

    திரும்பு "Unknown error";
}


நிலையான u_char *
ngx_resolver_log_error(ngx_log_t *log, u_char *buf, size_t len)
{
    u_char                     *p;
    ngx_resolver_connection_t  *rec;

    p = buf;

    எனில் (log->action) {
        p = ngx_snprintf(buf, len, " while %s", log->action);
        len -= p - buf;
    }

    rec = log->data;

    எனில் (rec) {
        p = ngx_snprintf(p, len, ", resolver: %V", &rec->server);
    }

    திரும்பு p;
}


நிலையான ngx_int_t
ngx_udp_connect(ngx_resolver_connection_t *rec)
{
    முழு                rc;
    ngx_int_t          event;
    ngx_event_t       *rev, *wev;
    ngx_socket_t       s;
    ngx_connection_t  *c;

    s = ngx_socket(rec->sockaddr->sa_family, SOCK_DGRAM, 0);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "UDP socket %d", s);

    எனில் (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_socket_n " failed");
        திரும்பு NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->log);

    எனில் (c == வெற்று) {
        எனில் (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        திரும்பு NGX_ERROR;
    }

    எனில் (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        செல் failed;
    }

    rev = c->பட;
    wev = c->லிக;

    rev->log = &rec->log;
    wev->log = &rec->log;

    rec->udp = c;

    c->எண் = ngx_atomic_fetch_add(ngx_connection_counter, 1);

    c->start_time = ngx_current_msec;

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->log, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->எண்);

    rc = connect(s, rec->sockaddr, rec->socklen);

    எனில் (rc == -1) {
        ngx_log_error(NGX_LOG_CRIT, &rec->log, ngx_socket_errno,
                      "connect() failed");

        செல் failed;
    }

    /* UDP sockets are always ready to write */
    wev->ready = 1;

    event = (ngx_event_flags & NGX_USE_CLEAR_EVENT) ?
                /* kqueue, epoll */                 NGX_CLEAR_EVENT:
                /* select, poll, /dev/poll */       NGX_LEVEL_EVENT;
                /* eventport event type has no meaning: oneshot only */

    எனில் (ngx_add_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        செல் failed;
    }

    திரும்பு NGX_OK;

failed:

    ngx_close_connection(c);
    rec->udp = வெற்று;

    திரும்பு NGX_ERROR;
}


நிலையான ngx_int_t
ngx_tcp_connect(ngx_resolver_connection_t *rec)
{
    முழு                rc;
    ngx_int_t          event;
    ngx_err_t          err;
    ngx_uint_t         level;
    ngx_socket_t       s;
    ngx_event_t       *rev, *wev;
    ngx_connection_t  *c;

    s = ngx_socket(rec->sockaddr->sa_family, SOCK_STREAM, 0);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "TCP socket %d", s);

    எனில் (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_socket_n " failed");
        திரும்பு NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->log);

    எனில் (c == வெற்று) {
        எனில் (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        திரும்பு NGX_ERROR;
    }

    எனில் (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        செல் failed;
    }

    rev = c->பட;
    wev = c->லிக;

    rev->log = &rec->log;
    wev->log = &rec->log;

    rec->tcp = c;

    c->எண் = ngx_atomic_fetch_add(ngx_connection_counter, 1);

    c->start_time = ngx_current_msec;

    எனில் (ngx_add_conn) {
        எனில் (ngx_add_conn(c) == NGX_ERROR) {
            செல் failed;
        }
    }

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->log, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->எண்);

    rc = connect(s, rec->sockaddr, rec->socklen);

    எனில் (rc == -1) {
        err = ngx_socket_errno;


        எனில் (err != NGX_EINPROGRESS
#எனில் (NGX_WIN32)
            /* Winsock returns WSAEWOULDBLOCK (NGX_EAGAIN) */
            && err != NGX_EAGAIN
#endif
            )
        {
            எனில் (err == NGX_ECONNREFUSED
#எனில் (NGX_LINUX)
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

            } இல்லையெனில் {
                level = NGX_LOG_CRIT;
            }

            ngx_log_error(level, &rec->log, err, "connect() to %V failed",
                          &rec->server);

            ngx_close_connection(c);
            rec->tcp = வெற்று;

            திரும்பு NGX_ERROR;
        }
    }

    எனில் (ngx_add_conn) {
        எனில் (rc == -1) {

            /* NGX_EINPROGRESS */

            திரும்பு NGX_AGAIN;
        }

        ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "connected");

        wev->ready = 1;

        திரும்பு NGX_OK;
    }

    எனில் (ngx_event_flags & NGX_USE_CLEAR_EVENT) {

        /* kqueue */

        event = NGX_CLEAR_EVENT;

    } இல்லையெனில் {

        /* select, poll, /dev/poll */

        event = NGX_LEVEL_EVENT;
    }

    எனில் (ngx_add_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        செல் failed;
    }

    எனில் (rc == -1) {

        /* NGX_EINPROGRESS */

        எனில் (ngx_add_event(wev, NGX_WRITE_EVENT, event) != NGX_OK) {
            செல் failed;
        }

        திரும்பு NGX_AGAIN;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "connected");

    wev->ready = 1;

    திரும்பு NGX_OK;

failed:

    ngx_close_connection(c);
    rec->tcp = வெற்று;

    திரும்பு NGX_ERROR;
}


நிலையான ngx_int_t
ngx_resolver_cmp_srvs(மாறிலி வெறுமை *one, மாறிலி வெறுமை *two)
{
    ngx_int_t            p1, p2;
    ngx_resolver_srv_t  *first, *second;

    first = (ngx_resolver_srv_t *) one;
    second = (ngx_resolver_srv_t *) two;

    p1 = first->priority;
    p2 = second->priority;

    திரும்பு p1 - p2;
}
