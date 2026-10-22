
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <ngx_event.h>


#وصف_ڪريو NGX_RESOLVER_UDP_سائيز   4096

#وصف_ڪريو NGX_RESOLVER_TCP_RSIZE  (2 + 65535)
#وصف_ڪريو NGX_RESOLVER_TCP_WSIZE  8192


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


#وصف_ڪريو ngx_resolver_ڳنڍ(n)  ngx_rbtree_معلومات(n, ngx_resolver_ڳنڍ_t, ڳنڍ)


ساكن ngx_int_t ngx_udp_connect(ngx_resolver_connection_t *rec);
ساكن ngx_int_t ngx_tcp_connect(ngx_resolver_connection_t *rec);


ساكن فراغ ngx_resolver_cleanup(فراغ *معلومات);
ساكن فراغ ngx_resolver_cleanup_وڻ(ngx_resolver_t *r, ngx_rbtree_t *وڻ);
ساكن ngx_int_t ngx_resolve_name_locked(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx, ngx_str_t *name);
ساكن فراغ ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *وڻ,
    ngx_قطار_t *قطار);
ساكن ngx_int_t ngx_resolver_send_query(ngx_resolver_t *r,
    ngx_resolver_ڳنڍ_t *rn);
ساكن ngx_int_t ngx_resolver_send_udp_query(ngx_resolver_t *r,
    ngx_resolver_connection_t *rec, u_char *query, u_short qlen);
ساكن ngx_int_t ngx_resolver_send_tcp_query(ngx_resolver_t *r,
    ngx_resolver_connection_t *rec, u_char *query, u_short qlen);
ساكن ngx_int_t ngx_resolver_create_name_query(ngx_resolver_t *r,
    ngx_resolver_ڳنڍ_t *rn, ngx_str_t *name);
ساكن ngx_int_t ngx_resolver_create_srv_query(ngx_resolver_t *r,
    ngx_resolver_ڳنڍ_t *rn, ngx_str_t *name);
ساكن ngx_int_t ngx_resolver_create_addr_query(ngx_resolver_t *r,
    ngx_resolver_ڳنڍ_t *rn, ngx_resolver_addr_t *addr);
ساكن فراغ ngx_resolver_resend_handler(ngx_event_t *ev);
ساكن وقت_t ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *وڻ,
    ngx_قطار_t *قطار);
ساكن ngx_uint_t ngx_resolver_resend_خالي_جايون(ngx_resolver_t *r);
ساكن فراغ ngx_resolver_udp_read(ngx_event_t *rev);
ساكن فراغ ngx_resolver_tcp_write(ngx_event_t *wev);
ساكن فراغ ngx_resolver_tcp_read(ngx_event_t *rev);
ساكن فراغ ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf,
    سائيز_t n, ngx_uint_t tcp);
ساكن فراغ ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, سائيز_t n,
    ngx_uint_t ident, ngx_uint_t ڪوڊ, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans);
ساكن فراغ ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, سائيز_t n,
    ngx_uint_t ident, ngx_uint_t ڪوڊ, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans);
ساكن فراغ ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, سائيز_t n,
    ngx_uint_t ident, ngx_uint_t ڪوڊ, ngx_uint_t nan);
ساكن ngx_resolver_ڳنڍ_t *ngx_resolver_lookup_name(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
ساكن ngx_resolver_ڳنڍ_t *ngx_resolver_lookup_srv(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
ساكن ngx_resolver_ڳنڍ_t *ngx_resolver_lookup_addr(ngx_resolver_t *r,
    اندريون_addr_t addr);
ساكن فراغ ngx_resolver_rbtree_وجهو_قدر(ngx_rbtree_ڳنڍ_t *عارضي,
    ngx_rbtree_ڳنڍ_t *ڳنڍ, ngx_rbtree_ڳنڍ_t *sentinel);
ساكن ngx_int_t ngx_resolver_copy(ngx_resolver_t *r, ngx_str_t *name,
    u_char *buf, u_char *src, u_char *آخري);
ساكن ngx_int_t ngx_resolver_سيٽ_timeout(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx);
ساكن فراغ ngx_resolver_timeout_handler(ngx_event_t *ev);
ساكن فراغ ngx_resolver_free_ڳنڍ(ngx_resolver_t *r, ngx_resolver_ڳنڍ_t *rn);
ساكن فراغ *ngx_resolver_alloc(ngx_resolver_t *r, سائيز_t سائيز);
ساكن فراغ *ngx_resolver_calloc(ngx_resolver_t *r, سائيز_t سائيز);
ساكن فراغ ngx_resolver_free(ngx_resolver_t *r, فراغ *p);
ساكن فراغ ngx_resolver_free_locked(ngx_resolver_t *r, فراغ *p);
ساكن فراغ *ngx_resolver_dup(ngx_resolver_t *r, فراغ *src, سائيز_t سائيز);
ساكن ngx_resolver_addr_t *ngx_resolver_export(ngx_resolver_t *r,
    ngx_resolver_ڳنڍ_t *rn, ngx_uint_t rotate);
ساكن فراغ ngx_resolver_report_srv(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx);
ساكن u_char *ngx_resolver_log_error(ngx_log_t *لاگرٿم, u_char *buf, سائيز_t len);
ساكن فراغ ngx_resolver_resolve_srv_نالا(ngx_resolver_ctx_t *ctx,
    ngx_resolver_ڳنڍ_t *rn);
ساكن فراغ ngx_resolver_srv_نالا_handler(ngx_resolver_ctx_t *ctx);
ساكن ngx_int_t ngx_resolver_cmp_srvs(ثابت فراغ *one, ثابت فراغ *two);

#إذا (NGX_HAVE_INET6)
ساكن فراغ ngx_resolver_rbtree_وجهو_addr6_قدر(ngx_rbtree_ڳنڍ_t *عارضي,
    ngx_rbtree_ڳنڍ_t *ڳنڍ, ngx_rbtree_ڳنڍ_t *sentinel);
ساكن ngx_resolver_ڳنڍ_t *ngx_resolver_lookup_addr6(ngx_resolver_t *r,
    بنية in6_addr *addr, uint32_t hash);
#جيڪڏهن_پورو


ngx_resolver_t *
ngx_resolver_create(ngx_conf_t *cf, ngx_str_t *نالا, ngx_uint_t n)
{
    ngx_str_t                   s;
    ngx_url_t                   u;
    ngx_uint_t                  i, j;
    ngx_resolver_t             *r;
    ngx_pool_cleanup_t         *cln;
    ngx_resolver_connection_t  *rec;

    r = ngx_pcalloc(cf->pool, حجم(ngx_resolver_t));
    إذا (r == عدم) {
        أرجع عدم;
    }

    r->event = ngx_pcalloc(cf->pool, حجم(ngx_event_t));
    إذا (r->event == عدم) {
        أرجع عدم;
    }

    cln = ngx_pool_cleanup_جوڙيو(cf->pool, 0);
    إذا (cln == عدم) {
        أرجع عدم;
    }

    cln->handler = ngx_resolver_cleanup;
    cln->معلومات = r;

    r->ipv4 = 1;

    ngx_rbtree_init(&r->name_rbtree, &r->name_sentinel,
                    ngx_resolver_rbtree_وجهو_قدر);

    ngx_rbtree_init(&r->srv_rbtree, &r->srv_sentinel,
                    ngx_resolver_rbtree_وجهو_قدر);

    ngx_rbtree_init(&r->addr_rbtree, &r->addr_sentinel,
                    ngx_rbtree_وجهو_قدر);

    ngx_قطار_init(&r->name_resend_قطار);
    ngx_قطار_init(&r->srv_resend_قطار);
    ngx_قطار_init(&r->addr_resend_قطار);

    ngx_قطار_init(&r->name_expire_قطار);
    ngx_قطار_init(&r->srv_expire_قطار);
    ngx_قطار_init(&r->addr_expire_قطار);

#إذا (NGX_HAVE_INET6)
    r->ipv6 = 1;

    ngx_rbtree_init(&r->addr6_rbtree, &r->addr6_sentinel,
                    ngx_resolver_rbtree_وجهو_addr6_قدر);

    ngx_قطار_init(&r->addr6_resend_قطار);

    ngx_قطار_init(&r->addr6_expire_قطار);
#جيڪڏهن_پورو

    r->event->handler = ngx_resolver_resend_handler;
    r->event->معلومات = r;
    r->event->لاگرٿم = &cf->چڪر->new_log;
    r->event->cancelable = 1;
    r->ident = -1;

    r->resend_timeout = 5;
    r->tcp_timeout = 5;
    r->expire = 30;
    r->valid = 0;

    r->لاگرٿم = &cf->چڪر->new_log;
    r->log_level = NGX_LOG_ERR;

    إذا (n) {
        إذا (ngx_array_init(&r->connections, cf->pool, n,
                           حجم(ngx_resolver_connection_t))
            != NGX_OK)
        {
            أرجع عدم;
        }
    }

    لكل (i = 0; i < n; i++) {
        إذا (ngx_strncmp(نالا[i].معلومات, "valid=", 6) == 0) {
            s.len = نالا[i].len - 6;
            s.معلومات = نالا[i].معلومات + 6;

            r->valid = ngx_parse_وقت(&s, 1);

            إذا (r->valid == (وقت_t) NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &نالا[i]);
                أرجع عدم;
            }

            استمر;
        }

#إذا (NGX_HAVE_INET6)
        إذا (ngx_strncmp(نالا[i].معلومات, "ipv4=", 5) == 0) {

            إذا (ngx_strcmp(&نالا[i].معلومات[5], "on") == 0) {
                r->ipv4 = 1;

            } وإلا إذا (ngx_strcmp(&نالا[i].معلومات[5], "off") == 0) {
                r->ipv4 = 0;

            } وإلا {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &نالا[i]);
                أرجع عدم;
            }

            استمر;
        }

        إذا (ngx_strncmp(نالا[i].معلومات, "ipv6=", 5) == 0) {

            إذا (ngx_strcmp(&نالا[i].معلومات[5], "on") == 0) {
                r->ipv6 = 1;

            } وإلا إذا (ngx_strcmp(&نالا[i].معلومات[5], "off") == 0) {
                r->ipv6 = 0;

            } وإلا {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &نالا[i]);
                أرجع عدم;
            }

            استمر;
        }
#جيڪڏهن_پورو

        ngx_memzero(&u, حجم(ngx_url_t));

        u.url = نالا[i];
        u.default_پورٽ = 53;

        إذا (ngx_parse_url(cf->pool, &u) != NGX_OK) {
            إذا (u.err) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "%s in resolver \"%V\"",
                                   u.err, &u.url);
            }

            أرجع عدم;
        }

        rec = ngx_array_ڌڪو_n(&r->connections, u.naddrs);
        إذا (rec == عدم) {
            أرجع عدم;
        }

        ngx_memzero(rec, u.naddrs * حجم(ngx_resolver_connection_t));

        لكل (j = 0; j < u.naddrs; j++) {
            rec[j].sockaddr = u.addrs[j].sockaddr;
            rec[j].socklen = u.addrs[j].socklen;
            rec[j].server = u.addrs[j].name;
            rec[j].resolver = r;
        }
    }

#إذا (NGX_HAVE_INET6)
    إذا (r->ipv4 + r->ipv6 == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"ipv4\" and \"ipv6\" cannot both be \"off\"");
        أرجع عدم;
    }
#جيڪڏهن_پورو

    إذا (n && r->connections.nelts == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "no name servers defined");
        أرجع عدم;
    }

    أرجع r;
}


ساكن فراغ
ngx_resolver_cleanup(فراغ *معلومات)
{
    ngx_resolver_t  *r = معلومات;

    ngx_uint_t                  i;
    ngx_resolver_connection_t  *rec;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_چڪر->لاگرٿم, 0, "cleanup resolver");

    ngx_resolver_cleanup_وڻ(r, &r->name_rbtree);

    ngx_resolver_cleanup_وڻ(r, &r->srv_rbtree);

    ngx_resolver_cleanup_وڻ(r, &r->addr_rbtree);

#إذا (NGX_HAVE_INET6)
    ngx_resolver_cleanup_وڻ(r, &r->addr6_rbtree);
#جيڪڏهن_پورو

    إذا (r->event->timer_سيٽ) {
        ngx_del_timer(r->event);
    }

    rec = r->connections.elts;

    لكل (i = 0; i < r->connections.nelts; i++) {
        إذا (rec[i].udp) {
            ngx_close_connection(rec[i].udp);
        }

        إذا (rec[i].tcp) {
            ngx_close_connection(rec[i].tcp);
        }

        إذا (rec[i].read_buf) {
            ngx_resolver_free(r, rec[i].read_buf->شروعات);
            ngx_resolver_free(r, rec[i].read_buf);
        }

        إذا (rec[i].write_buf) {
            ngx_resolver_free(r, rec[i].write_buf->شروعات);
            ngx_resolver_free(r, rec[i].write_buf);
        }
    }
}


ساكن فراغ
ngx_resolver_cleanup_وڻ(ngx_resolver_t *r, ngx_rbtree_t *وڻ)
{
    ngx_resolver_ctx_t   *ctx, *ايندڙ;
    ngx_resolver_ڳنڍ_t  *rn;

    طالما (وڻ->پاڙ != وڻ->sentinel) {

        rn = ngx_resolver_ڳنڍ(ngx_rbtree_min(وڻ->پاڙ, وڻ->sentinel));

        ngx_قطار_remove(&rn->قطار);

        لكل (ctx = rn->انتظار; ctx; ctx = ايندڙ) {
            ايندڙ = ctx->ايندڙ;

            إذا (ctx->event) {
                إذا (ctx->event->timer_سيٽ) {
                    ngx_del_timer(ctx->event);
                }

                ngx_resolver_free(r, ctx->event);
            }

            ngx_resolver_free(r, ctx);
        }

        ngx_rbtree_delete(وڻ, &rn->ڳنڍ);

        ngx_resolver_free_ڳنڍ(r, rn);
    }
}


ngx_resolver_ctx_t *
ngx_resolve_شروعات(ngx_resolver_t *r, ngx_resolver_ctx_t *عارضي)
{
    اندريون_addr_t            addr;
    ngx_resolver_ctx_t  *ctx;

    إذا (عارضي) {
        addr = ngx_inet_addr(عارضي->name.معلومات, عارضي->name.len);

        إذا (addr != INADDR_NONE) {
            عارضي->resolver = r;
            عارضي->state = NGX_OK;
            عارضي->naddrs = 1;
            عارضي->addrs = &عارضي->addr;
            عارضي->addr.sockaddr = (بنية sockaddr *) &عارضي->جيب;
            عارضي->addr.socklen = حجم(بنية sockaddr_اندريون);
            ngx_memzero(&عارضي->جيب, حجم(بنية sockaddr_اندريون));
            عارضي->جيب.sin_family = AF_INET;
            عارضي->جيب.sin_addr.s_addr = addr;
            عارضي->تيز = 1;

            أرجع عارضي;
        }
    }

    إذا (r->connections.nelts == 0) {
        أرجع NGX_NO_RESOLVER;
    }

    ctx = ngx_resolver_calloc(r, حجم(ngx_resolver_ctx_t));

    إذا (ctx) {
        ctx->resolver = r;
    }

    أرجع ctx;
}


ngx_int_t
ngx_resolve_name(ngx_resolver_ctx_t *ctx)
{
    سائيز_t           slen;
    ngx_int_t        rc;
    ngx_str_t        name;
    ngx_resolver_t  *r;

    r = ctx->resolver;

    إذا (ctx->name.len > 0 && ctx->name.معلومات[ctx->name.len - 1] == '.') {
        ctx->name.len--;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لاگرٿم, 0,
                   "resolve: \"%V\"", &ctx->name);

    إذا (ctx->تيز) {
        ctx->handler(ctx);
        أرجع NGX_OK;
    }

    إذا (ctx->service.len) {
        slen = ctx->service.len;

        إذا (ngx_strlchr(ctx->service.معلومات,
                        ctx->service.معلومات + ctx->service.len, '.')
            == عدم)
        {
            slen += حجم("_._tcp") - 1;
        }

        name.len = slen + 1 + ctx->name.len;

        name.معلومات = ngx_resolver_alloc(r, name.len);
        إذا (name.معلومات == عدم) {
            اذهب failed;
        }

        إذا (slen == ctx->service.len) {
            ngx_sprintf(name.معلومات, "%V.%V", &ctx->service, &ctx->name);

        } وإلا {
            ngx_sprintf(name.معلومات, "_%V._tcp.%V", &ctx->service, &ctx->name);
        }

        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &name);

        ngx_resolver_free(r, name.معلومات);

    } وإلا {
        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &ctx->name);
    }

    إذا (rc == NGX_OK) {
        أرجع NGX_OK;
    }

    /* unlock name mutex */

    إذا (rc == NGX_AGAIN) {
        أرجع NGX_OK;
    }

    /* NGX_ERROR */

    إذا (ctx->event) {
        ngx_resolver_free(r, ctx->event);
    }

failed:

    ngx_resolver_free(r, ctx);

    أرجع NGX_ERROR;
}


فراغ
ngx_resolve_name_پورو(ngx_resolver_ctx_t *ctx)
{
    ngx_uint_t            i;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_ڳنڍ_t  *rn;

    r = ctx->resolver;

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لاگرٿم, 0,
                   "resolve name done: %i", ctx->state);

    إذا (ctx->تيز) {
        أرجع;
    }

    إذا (ctx->event && ctx->event->timer_سيٽ) {
        ngx_del_timer(ctx->event);
    }

    /* lock name mutex */

    إذا (ctx->nsrvs) {
        لكل (i = 0; i < ctx->nsrvs; i++) {
            إذا (ctx->srvs[i].ctx) {
                ngx_resolve_name_پورو(ctx->srvs[i].ctx);
            }

            إذا (ctx->srvs[i].addrs) {
                ngx_resolver_free(r, ctx->srvs[i].addrs->sockaddr);
                ngx_resolver_free(r, ctx->srvs[i].addrs);
            }

            ngx_resolver_free(r, ctx->srvs[i].name.معلومات);
        }

        ngx_resolver_free(r, ctx->srvs);
    }

    إذا (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->ڳنڍ;

        إذا (rn) {
            p = &rn->انتظار;
            w = rn->انتظار;

            طالما (w) {
                إذا (w == ctx) {
                    *p = w->ايندڙ;

                    اذهب پورو;
                }

                p = &w->ايندڙ;
                w = w->ايندڙ;
            }

            ngx_log_error(NGX_LOG_ALERT, r->لاگرٿم, 0,
                          "could not cancel %V resolving", &ctx->name);
        }
    }

پورو:

    إذا (ctx->service.len) {
        ngx_resolver_expire(r, &r->srv_rbtree, &r->srv_expire_قطار);

    } وإلا {
        ngx_resolver_expire(r, &r->name_rbtree, &r->name_expire_قطار);
    }

    /* unlock name mutex */

    /* lock alloc mutex */

    إذا (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    إذا (r->event->timer_سيٽ && ngx_resolver_resend_خالي_جايون(r)) {
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
    ngx_قطار_t          *resend_قطار, *expire_قطار;
    ngx_rbtree_t         *وڻ;
    ngx_resolver_ctx_t   *ايندڙ, *آخري;
    ngx_resolver_addr_t  *addrs;
    ngx_resolver_ڳنڍ_t  *rn;

    ngx_strlow(name->معلومات, name->معلومات, name->len);

    hash = ngx_چڪري_جاچ_32_short(name->معلومات, name->len);

    إذا (ctx->service.len) {
        rn = ngx_resolver_lookup_srv(r, name, hash);

        وڻ = &r->srv_rbtree;
        resend_قطار = &r->srv_resend_قطار;
        expire_قطار = &r->srv_expire_قطار;

    } وإلا {
        rn = ngx_resolver_lookup_name(r, name, hash);

        وڻ = &r->name_rbtree;
        resend_قطار = &r->name_resend_قطار;
        expire_قطار = &r->name_expire_قطار;
    }

    إذا (rn) {

        /* ctx can be a list after NGX_RESOLVE_CNAME */
        لكل (آخري = ctx; آخري->ايندڙ; آخري = آخري->ايندڙ);

        إذا (rn->valid >= ngx_وقت()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->لاگرٿم, 0, "resolve cached");

            ngx_قطار_remove(&rn->قطار);

            rn->expire = ngx_وقت() + r->expire;

            ngx_قطار_وجهو_سر(expire_قطار, &rn->قطار);

            naddrs = (rn->naddrs == (u_short) -1) ? 0 : rn->naddrs;
#إذا (NGX_HAVE_INET6)
            naddrs += (rn->naddrs6 == (u_short) -1) ? 0 : rn->naddrs6;
#جيڪڏهن_پورو

            إذا (naddrs) {

                إذا (naddrs == 1 && rn->naddrs == 1) {
                    addrs = عدم;

                } وإلا {
                    addrs = ngx_resolver_export(r, rn, 1);
                    إذا (addrs == عدم) {
                        أرجع NGX_ERROR;
                    }
                }

                آخري->ايندڙ = rn->انتظار;
                rn->انتظار = عدم;

                /* unlock name mutex */

                افعل {
                    ctx->state = NGX_OK;
                    ctx->valid = rn->valid;
                    ctx->naddrs = naddrs;

                    إذا (addrs == عدم) {
                        ctx->addrs = &ctx->addr;
                        ctx->addr.sockaddr = (بنية sockaddr *) &ctx->جيب;
                        ctx->addr.socklen = حجم(بنية sockaddr_اندريون);
                        ngx_memzero(&ctx->جيب, حجم(بنية sockaddr_اندريون));
                        ctx->جيب.sin_family = AF_INET;
                        ctx->جيب.sin_addr.s_addr = rn->u.addr;

                    } وإلا {
                        ctx->addrs = addrs;
                    }

                    ايندڙ = ctx->ايندڙ;

                    ctx->handler(ctx);

                    ctx = ايندڙ;
                } طالما (ctx);

                إذا (addrs != عدم) {
                    ngx_resolver_free(r, addrs->sockaddr);
                    ngx_resolver_free(r, addrs);
                }

                أرجع NGX_OK;
            }

            إذا (rn->nsrvs) {
                آخري->ايندڙ = rn->انتظار;
                rn->انتظار = عدم;

                /* unlock name mutex */

                افعل {
                    ايندڙ = ctx->ايندڙ;

                    ngx_resolver_resolve_srv_نالا(ctx, rn);

                    ctx = ايندڙ;
                } طالما (ctx);

                أرجع NGX_OK;
            }

            /* NGX_RESOLVE_CNAME */

            إذا (ctx->recursion++ < NGX_RESOLVER_MAX_RECURSION) {

                cname.len = rn->cnlen;
                cname.معلومات = rn->u.cname;

                أرجع ngx_resolve_name_locked(r, ctx, &cname);
            }

            آخري->ايندڙ = rn->انتظار;
            rn->انتظار = عدم;

            /* unlock name mutex */

            افعل {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                ctx->valid = ngx_وقت() + (r->valid ? r->valid : 10);
                ايندڙ = ctx->ايندڙ;

                ctx->handler(ctx);

                ctx = ايندڙ;
            } طالما (ctx);

            أرجع NGX_OK;
        }

        إذا (rn->انتظار) {
            إذا (ngx_resolver_سيٽ_timeout(r, ctx) != NGX_OK) {
                أرجع NGX_ERROR;
            }

            آخري->ايندڙ = rn->انتظار;
            rn->انتظار = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;

            افعل {
                ctx->ڳنڍ = rn;
                ctx = ctx->ايندڙ;
            } طالما (ctx);

            أرجع NGX_AGAIN;
        }

        ngx_قطار_remove(&rn->قطار);

        /* lock alloc mutex */

        إذا (rn->query) {
            ngx_resolver_free_locked(r, rn->query);
            rn->query = عدم;
#إذا (NGX_HAVE_INET6)
            rn->query6 = عدم;
#جيڪڏهن_پورو
        }

        إذا (rn->cnlen) {
            ngx_resolver_free_locked(r, rn->u.cname);
        }

        إذا (rn->naddrs > 1 && rn->naddrs != (u_short) -1) {
            ngx_resolver_free_locked(r, rn->u.addrs);
        }

#إذا (NGX_HAVE_INET6)
        إذا (rn->naddrs6 > 1 && rn->naddrs6 != (u_short) -1) {
            ngx_resolver_free_locked(r, rn->u6.addrs6);
        }
#جيڪڏهن_پورو

        إذا (rn->nsrvs) {
            لكل (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
                إذا (rn->u.srvs[i].name.معلومات) {
                    ngx_resolver_free_locked(r, rn->u.srvs[i].name.معلومات);
                }
            }

            ngx_resolver_free_locked(r, rn->u.srvs);
        }

        /* unlock alloc mutex */

    } وإلا {

        rn = ngx_resolver_alloc(r, حجم(ngx_resolver_ڳنڍ_t));
        إذا (rn == عدم) {
            أرجع NGX_ERROR;
        }

        rn->name = ngx_resolver_dup(r, name->معلومات, name->len);
        إذا (rn->name == عدم) {
            ngx_resolver_free(r, rn);
            أرجع NGX_ERROR;
        }

        rn->ڳنڍ.ڪنجي = hash;
        rn->nlen = (u_short) name->len;
        rn->query = عدم;
#إذا (NGX_HAVE_INET6)
        rn->query6 = عدم;
#جيڪڏهن_پورو

        ngx_rbtree_وجهو(وڻ, &rn->ڳنڍ);
    }

    إذا (ctx->service.len) {
        rc = ngx_resolver_create_srv_query(r, rn, name);

    } وإلا {
        rc = ngx_resolver_create_name_query(r, rn, name);
    }

    إذا (rc == NGX_ERROR) {
        اذهب failed;
    }

    إذا (rc == NGX_DECLINED) {
        ngx_rbtree_delete(وڻ, &rn->ڳنڍ);

        ngx_resolver_free(r, rn->query);
        ngx_resolver_free(r, rn->name);
        ngx_resolver_free(r, rn);

        افعل {
            ctx->state = NGX_RESOLVE_NXDOMAIN;
            ايندڙ = ctx->ايندڙ;

            ctx->handler(ctx);

            ctx = ايندڙ;
        } طالما (ctx);

        أرجع NGX_OK;
    }

    rn->آخري_connection = r->آخري_connection++;
    إذا (r->آخري_connection == r->connections.nelts) {
        r->آخري_connection = 0;
    }

    rn->naddrs = r->ipv4 ? (u_short) -1 : 0;
    rn->tcp = 0;
#إذا (NGX_HAVE_INET6)
    rn->naddrs6 = r->ipv6 ? (u_short) -1 : 0;
    rn->tcp6 = 0;
#جيڪڏهن_پورو
    rn->nsrvs = 0;

    إذا (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->آخري_connection++;
        إذا (rn->آخري_connection == r->connections.nelts) {
            rn->آخري_connection = 0;
        }

        (فراغ) ngx_resolver_send_query(r, rn);
    }

    إذا (ngx_resolver_سيٽ_timeout(r, ctx) != NGX_OK) {
        اذهب failed;
    }

    إذا (ngx_resolver_resend_خالي_جايون(r)) {
        ngx_جوڙيو_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_وقت() + r->resend_timeout;

    ngx_قطار_وجهو_سر(resend_قطار, &rn->قطار);

    rn->ڪوڊ = 0;
    rn->cnlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_قدر;
    rn->انتظار = ctx;

    ctx->state = NGX_AGAIN;
    ctx->async = 1;

    افعل {
        ctx->ڳنڍ = rn;
        ctx = ctx->ايندڙ;
    } طالما (ctx);

    أرجع NGX_AGAIN;

failed:

    ngx_rbtree_delete(وڻ, &rn->ڳنڍ);

    إذا (rn->query) {
        ngx_resolver_free(r, rn->query);
    }

    ngx_resolver_free(r, rn->name);

    ngx_resolver_free(r, rn);

    أرجع NGX_ERROR;
}


ngx_int_t
ngx_resolve_addr(ngx_resolver_ctx_t *ctx)
{
    u_char               *name;
    اندريون_addr_t             addr;
    ngx_قطار_t          *resend_قطار, *expire_قطار;
    ngx_rbtree_t         *وڻ;
    ngx_resolver_t       *r;
    بنية sockaddr_اندريون   *جيب;
    ngx_resolver_ڳنڍ_t  *rn;
#إذا (NGX_HAVE_INET6)
    uint32_t              hash;
    بنية sockaddr_in6  *sin6;
#جيڪڏهن_پورو

#إذا (NGX_SUPPRESS_WARN)
    addr = 0;
#إذا (NGX_HAVE_INET6)
    hash = 0;
    sin6 = عدم;
#جيڪڏهن_پورو
#جيڪڏهن_پورو

    r = ctx->resolver;

    بدل (ctx->addr.sockaddr->sa_family) {

#إذا (NGX_HAVE_INET6)
    حالة AF_INET6:
        sin6 = (بنية sockaddr_in6 *) ctx->addr.sockaddr;
        hash = ngx_چڪري_جاچ_32_short(sin6->sin6_addr.s6_addr, 16);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr6(r, &sin6->sin6_addr, hash);

        وڻ = &r->addr6_rbtree;
        resend_قطار = &r->addr6_resend_قطار;
        expire_قطار = &r->addr6_expire_قطار;

        اكسر;
#جيڪڏهن_پورو

    افتراضي: /* AF_INET */
        جيب = (بنية sockaddr_اندريون *) ctx->addr.sockaddr;
        addr = ntohl(جيب->sin_addr.s_addr);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        وڻ = &r->addr_rbtree;
        resend_قطار = &r->addr_resend_قطار;
        expire_قطار = &r->addr_expire_قطار;
    }

    إذا (rn) {

        إذا (rn->valid >= ngx_وقت()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->لاگرٿم, 0, "resolve cached");

            ngx_قطار_remove(&rn->قطار);

            rn->expire = ngx_وقت() + r->expire;

            ngx_قطار_وجهو_سر(expire_قطار, &rn->قطار);

            name = ngx_resolver_dup(r, rn->name, rn->nlen);
            إذا (name == عدم) {
                ngx_resolver_free(r, ctx);
                أرجع NGX_ERROR;
            }

            ctx->name.len = rn->nlen;
            ctx->name.معلومات = name;

            /* unlock addr mutex */

            ctx->state = NGX_OK;
            ctx->valid = rn->valid;

            ctx->handler(ctx);

            ngx_resolver_free(r, name);

            أرجع NGX_OK;
        }

        إذا (rn->انتظار) {
            إذا (ngx_resolver_سيٽ_timeout(r, ctx) != NGX_OK) {
                أرجع NGX_ERROR;
            }

            ctx->ايندڙ = rn->انتظار;
            rn->انتظار = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;
            ctx->ڳنڍ = rn;

            /* unlock addr mutex */

            أرجع NGX_OK;
        }

        ngx_قطار_remove(&rn->قطار);

        ngx_resolver_free(r, rn->query);
        rn->query = عدم;
#إذا (NGX_HAVE_INET6)
        rn->query6 = عدم;
#جيڪڏهن_پورو

    } وإلا {
        rn = ngx_resolver_alloc(r, حجم(ngx_resolver_ڳنڍ_t));
        إذا (rn == عدم) {
            اذهب failed;
        }

        بدل (ctx->addr.sockaddr->sa_family) {

#إذا (NGX_HAVE_INET6)
        حالة AF_INET6:
            rn->addr6 = sin6->sin6_addr;
            rn->ڳنڍ.ڪنجي = hash;
            اكسر;
#جيڪڏهن_پورو

        افتراضي: /* AF_INET */
            rn->ڳنڍ.ڪنجي = addr;
        }

        rn->query = عدم;
#إذا (NGX_HAVE_INET6)
        rn->query6 = عدم;
#جيڪڏهن_پورو

        ngx_rbtree_وجهو(وڻ, &rn->ڳنڍ);
    }

    إذا (ngx_resolver_create_addr_query(r, rn, &ctx->addr) != NGX_OK) {
        اذهب failed;
    }

    rn->آخري_connection = r->آخري_connection++;
    إذا (r->آخري_connection == r->connections.nelts) {
        r->آخري_connection = 0;
    }

    rn->naddrs = (u_short) -1;
    rn->tcp = 0;
#إذا (NGX_HAVE_INET6)
    rn->naddrs6 = (u_short) -1;
    rn->tcp6 = 0;
#جيڪڏهن_پورو
    rn->nsrvs = 0;

    إذا (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->آخري_connection++;
        إذا (rn->آخري_connection == r->connections.nelts) {
            rn->آخري_connection = 0;
        }

        (فراغ) ngx_resolver_send_query(r, rn);
    }

    إذا (ngx_resolver_سيٽ_timeout(r, ctx) != NGX_OK) {
        اذهب failed;
    }

    إذا (ngx_resolver_resend_خالي_جايون(r)) {
        ngx_جوڙيو_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_وقت() + r->resend_timeout;

    ngx_قطار_وجهو_سر(resend_قطار, &rn->قطار);

    rn->ڪوڊ = 0;
    rn->cnlen = 0;
    rn->name = عدم;
    rn->nlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_قدر;
    rn->انتظار = ctx;

    /* unlock addr mutex */

    ctx->state = NGX_AGAIN;
    ctx->async = 1;
    ctx->ڳنڍ = rn;

    أرجع NGX_OK;

failed:

    إذا (rn) {
        ngx_rbtree_delete(وڻ, &rn->ڳنڍ);

        إذا (rn->query) {
            ngx_resolver_free(r, rn->query);
        }

        ngx_resolver_free(r, rn);
    }

    /* unlock addr mutex */

    إذا (ctx->event) {
        ngx_resolver_free(r, ctx->event);
    }

    ngx_resolver_free(r, ctx);

    أرجع NGX_ERROR;
}


فراغ
ngx_resolve_addr_پورو(ngx_resolver_ctx_t *ctx)
{
    ngx_قطار_t          *expire_قطار;
    ngx_rbtree_t         *وڻ;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_ڳنڍ_t  *rn;

    r = ctx->resolver;

    بدل (ctx->addr.sockaddr->sa_family) {

#إذا (NGX_HAVE_INET6)
    حالة AF_INET6:
        وڻ = &r->addr6_rbtree;
        expire_قطار = &r->addr6_expire_قطار;
        اكسر;
#جيڪڏهن_پورو

    افتراضي: /* AF_INET */
        وڻ = &r->addr_rbtree;
        expire_قطار = &r->addr_expire_قطار;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لاگرٿم, 0,
                   "resolve addr done: %i", ctx->state);

    إذا (ctx->event && ctx->event->timer_سيٽ) {
        ngx_del_timer(ctx->event);
    }

    /* lock addr mutex */

    إذا (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->ڳنڍ;

        إذا (rn) {
            p = &rn->انتظار;
            w = rn->انتظار;

            طالما (w) {
                إذا (w == ctx) {
                    *p = w->ايندڙ;

                    اذهب پورو;
                }

                p = &w->ايندڙ;
                w = w->ايندڙ;
            }
        }

        {
            u_char     متن[NGX_SOCKADDR_STRLEN];
            ngx_str_t  addrtext;

            addrtext.معلومات = متن;
            addrtext.len = ngx_sock_ntop(ctx->addr.sockaddr, ctx->addr.socklen,
                                         متن, NGX_SOCKADDR_STRLEN, 0);

            ngx_log_error(NGX_LOG_ALERT, r->لاگرٿم, 0,
                          "could not cancel %V resolving", &addrtext);
        }
    }

پورو:

    ngx_resolver_expire(r, وڻ, expire_قطار);

    /* unlock addr mutex */

    /* lock alloc mutex */

    إذا (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    إذا (r->event->timer_سيٽ && ngx_resolver_resend_خالي_جايون(r)) {
        ngx_del_timer(r->event);
    }
}


ساكن فراغ
ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *وڻ, ngx_قطار_t *قطار)
{
    وقت_t                now;
    ngx_uint_t            i;
    ngx_قطار_t          *q;
    ngx_resolver_ڳنڍ_t  *rn;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->لاگرٿم, 0, "resolver expire");

    now = ngx_وقت();

    لكل (i = 0; i < 2; i++) {
        إذا (ngx_قطار_خالي_جايون(قطار)) {
            أرجع;
        }

        q = ngx_قطار_آخري(قطار);

        rn = ngx_قطار_معلومات(q, ngx_resolver_ڳنڍ_t, قطار);

        إذا (now <= rn->expire) {
            أرجع;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->لاگرٿم, 0,
                       "resolver expire \"%*s\"", (سائيز_t) rn->nlen, rn->name);

        ngx_قطار_remove(q);

        ngx_rbtree_delete(وڻ, &rn->ڳنڍ);

        ngx_resolver_free_ڳنڍ(r, rn);
    }
}


ساكن ngx_int_t
ngx_resolver_send_query(ngx_resolver_t *r, ngx_resolver_ڳنڍ_t *rn)
{
    ngx_int_t                   rc;
    ngx_resolver_connection_t  *rec;

    rec = r->connections.elts;
    rec = &rec[rn->آخري_connection];

    إذا (rec->لاگرٿم.handler == عدم) {
        rec->لاگرٿم = *r->لاگرٿم;
        rec->لاگرٿم.handler = ngx_resolver_log_error;
        rec->لاگرٿم.معلومات = rec;
        rec->لاگرٿم.action = "resolving";
    }

    إذا (rn->query && rn->naddrs == (u_short) -1) {
        rc = rn->tcp ? ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen)
                     : ngx_resolver_send_udp_query(r, rec, rn->query, rn->qlen);

        إذا (rc != NGX_OK) {
            أرجع rc;
        }
    }

#إذا (NGX_HAVE_INET6)

    إذا (rn->query6 && rn->naddrs6 == (u_short) -1) {
        rc = rn->tcp6
                    ? ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen)
                    : ngx_resolver_send_udp_query(r, rec, rn->query6, rn->qlen);

        إذا (rc != NGX_OK) {
            أرجع rc;
        }
    }

#جيڪڏهن_پورو

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_resolver_send_udp_query(ngx_resolver_t *r, ngx_resolver_connection_t  *rec,
    u_char *query, u_short qlen)
{
    ssize_t  n;

    إذا (rec->udp == عدم) {
        إذا (ngx_udp_connect(rec) != NGX_OK) {
            أرجع NGX_ERROR;
        }

        rec->udp->معلومات = rec;
        rec->udp->اقرأ->handler = ngx_resolver_udp_read;
        rec->udp->اقرأ->resolver = 1;
    }

    n = ngx_send(rec->udp, query, qlen);

    إذا (n == NGX_ERROR) {
        اذهب failed;
    }

    إذا ((سائيز_t) n != (سائيز_t) qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->لاگرٿم, 0, "send() incomplete");
        اذهب failed;
    }

    أرجع NGX_OK;

failed:

    ngx_close_connection(rec->udp);
    rec->udp = عدم;

    أرجع NGX_ERROR;
}


ساكن ngx_int_t
ngx_resolver_send_tcp_query(ngx_resolver_t *r, ngx_resolver_connection_t *rec,
    u_char *query, u_short qlen)
{
    ngx_buf_t  *b;
    ngx_int_t   rc;

    rc = NGX_OK;

    إذا (rec->tcp == عدم) {
        b = rec->read_buf;

        إذا (b == عدم) {
            b = ngx_resolver_calloc(r, حجم(ngx_buf_t));
            إذا (b == عدم) {
                أرجع NGX_ERROR;
            }

            b->شروعات = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_RSIZE);
            إذا (b->شروعات == عدم) {
                ngx_resolver_free(r, b);
                أرجع NGX_ERROR;
            }

            b->پڇاڙي = b->شروعات + NGX_RESOLVER_TCP_RSIZE;

            rec->read_buf = b;
        }

        b->pos = b->شروعات;
        b->آخري = b->شروعات;

        b = rec->write_buf;

        إذا (b == عدم) {
            b = ngx_resolver_calloc(r, حجم(ngx_buf_t));
            إذا (b == عدم) {
                أرجع NGX_ERROR;
            }

            b->شروعات = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_WSIZE);
            إذا (b->شروعات == عدم) {
                ngx_resolver_free(r, b);
                أرجع NGX_ERROR;
            }

            b->پڇاڙي = b->شروعات + NGX_RESOLVER_TCP_WSIZE;

            rec->write_buf = b;
        }

        b->pos = b->شروعات;
        b->آخري = b->شروعات;

        rc = ngx_tcp_connect(rec);
        إذا (rc == NGX_ERROR) {
            أرجع NGX_ERROR;
        }

        rec->tcp->معلومات = rec;
        rec->tcp->اكتب->handler = ngx_resolver_tcp_write;
        rec->tcp->اكتب->cancelable = 1;
        rec->tcp->اقرأ->handler = ngx_resolver_tcp_read;
        rec->tcp->اقرأ->resolver = 1;

        ngx_جوڙيو_timer(rec->tcp->اكتب, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    b = rec->write_buf;

    إذا (b->پڇاڙي - b->آخري <  2 + qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->لاگرٿم, 0, "buffer overflow");
        أرجع NGX_ERROR;
    }

    *b->آخري++ = (u_char) (qlen >> 8);
    *b->آخري++ = (u_char) qlen;
    b->آخري = ngx_cpymem(b->آخري, query, qlen);

    إذا (rc == NGX_OK) {
        ngx_resolver_tcp_write(rec->tcp->اكتب);
    }

    أرجع NGX_OK;
}


ساكن فراغ
ngx_resolver_resend_handler(ngx_event_t *ev)
{
    وقت_t           timer, atimer, stimer, ntimer;
#إذا (NGX_HAVE_INET6)
    وقت_t           a6timer;
#جيڪڏهن_پورو
    ngx_resolver_t  *r;

    r = ev->معلومات;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->لاگرٿم, 0,
                   "resolver resend handler");

    /* lock name mutex */

    ntimer = ngx_resolver_resend(r, &r->name_rbtree, &r->name_resend_قطار);

    stimer = ngx_resolver_resend(r, &r->srv_rbtree, &r->srv_resend_قطار);

    /* unlock name mutex */

    /* lock addr mutex */

    atimer = ngx_resolver_resend(r, &r->addr_rbtree, &r->addr_resend_قطار);

    /* unlock addr mutex */

#إذا (NGX_HAVE_INET6)

    /* lock addr6 mutex */

    a6timer = ngx_resolver_resend(r, &r->addr6_rbtree, &r->addr6_resend_قطار);

    /* unlock addr6 mutex */

#جيڪڏهن_پورو

    timer = ntimer;

    إذا (timer == 0) {
        timer = atimer;

    } وإلا إذا (atimer) {
        timer = ngx_min(timer, atimer);
    }

    إذا (timer == 0) {
        timer = stimer;

    } وإلا إذا (stimer) {
        timer = ngx_min(timer, stimer);
    }

#إذا (NGX_HAVE_INET6)

    إذا (timer == 0) {
        timer = a6timer;

    } وإلا إذا (a6timer) {
        timer = ngx_min(timer, a6timer);
    }

#جيڪڏهن_پورو

    إذا (timer) {
        ngx_جوڙيو_timer(r->event, (ngx_msec_t) (timer * 1000));
    }
}


ساكن وقت_t
ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *وڻ, ngx_قطار_t *قطار)
{
    وقت_t                now;
    ngx_قطار_t          *q;
    ngx_resolver_ڳنڍ_t  *rn;

    now = ngx_وقت();

    لكل ( ;; ) {
        إذا (ngx_قطار_خالي_جايون(قطار)) {
            أرجع 0;
        }

        q = ngx_قطار_آخري(قطار);

        rn = ngx_قطار_معلومات(q, ngx_resolver_ڳنڍ_t, قطار);

        إذا (now < rn->expire) {
            أرجع rn->expire - now;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->لاگرٿم, 0,
                       "resolver resend \"%*s\" %p",
                       (سائيز_t) rn->nlen, rn->name, rn->انتظار);

        ngx_قطار_remove(q);

        إذا (rn->انتظار) {

            إذا (++rn->آخري_connection == r->connections.nelts) {
                rn->آخري_connection = 0;
            }

            (فراغ) ngx_resolver_send_query(r, rn);

            rn->expire = now + r->resend_timeout;

            ngx_قطار_وجهو_سر(قطار, q);

            استمر;
        }

        ngx_rbtree_delete(وڻ, &rn->ڳنڍ);

        ngx_resolver_free_ڳنڍ(r, rn);
    }
}


ساكن ngx_uint_t
ngx_resolver_resend_خالي_جايون(ngx_resolver_t *r)
{
    أرجع ngx_قطار_خالي_جايون(&r->name_resend_قطار)
           && ngx_قطار_خالي_جايون(&r->srv_resend_قطار)
#إذا (NGX_HAVE_INET6)
           && ngx_قطار_خالي_جايون(&r->addr6_resend_قطار)
#جيڪڏهن_پورو
           && ngx_قطار_خالي_جايون(&r->addr_resend_قطار);
}


ساكن فراغ
ngx_resolver_udp_read(ngx_event_t *rev)
{
    ssize_t                     n;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;
    u_char                      buf[NGX_RESOLVER_UDP_سائيز];

    c = rev->معلومات;
    rec = c->معلومات;

    افعل {
        n = ngx_udp_recv(c, buf, NGX_RESOLVER_UDP_سائيز);

        إذا (n == NGX_AGAIN) {
            اكسر;
        }

        إذا (n == NGX_ERROR) {
            اذهب failed;
        }

        ngx_resolver_process_response(rec->resolver, buf, n, 0);

    } طالما (rev->ready);

    إذا (ngx_handle_read_event(rev, 0) != NGX_OK) {
        اذهب failed;
    }

    أرجع;

failed:

    ngx_close_connection(rec->udp);
    rec->udp = عدم;
}


ساكن فراغ
ngx_resolver_tcp_write(ngx_event_t *wev)
{
    off_t                       موڪليل;
    ssize_t                     n;
    ngx_buf_t                  *b;
    ngx_resolver_t             *r;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;

    c = wev->معلومات;
    rec = c->معلومات;
    b = rec->write_buf;
    r = rec->resolver;

    إذا (wev->timedout) {
        اذهب failed;
    }

    موڪليل = c->موڪليل;

    طالما (wev->ready && b->pos < b->آخري) {
        n = ngx_send(c, b->pos, b->آخري - b->pos);

        إذا (n == NGX_AGAIN) {
            اكسر;
        }

        إذا (n == NGX_ERROR) {
            اذهب failed;
        }

        b->pos += n;
    }

    إذا (b->pos != b->شروعات) {
        b->آخري = ngx_movemem(b->شروعات, b->pos, b->آخري - b->pos);
        b->pos = b->شروعات;
    }

    إذا (c->موڪليل != موڪليل) {
        ngx_جوڙيو_timer(wev, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    إذا (ngx_handle_write_event(wev, 0) != NGX_OK) {
        اذهب failed;
    }

    أرجع;

failed:

    ngx_close_connection(c);
    rec->tcp = عدم;
}


ساكن فراغ
ngx_resolver_tcp_read(ngx_event_t *rev)
{
    u_char                     *p;
    سائيز_t                      سائيز;
    ssize_t                     n;
    u_short                     qlen;
    ngx_buf_t                  *b;
    ngx_resolver_t             *r;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;

    c = rev->معلومات;
    rec = c->معلومات;
    b = rec->read_buf;
    r = rec->resolver;

    طالما (rev->ready) {
        n = ngx_recv(c, b->آخري, b->پڇاڙي - b->آخري);

        إذا (n == NGX_AGAIN) {
            اكسر;
        }

        إذا (n == NGX_ERROR || n == 0) {
            اذهب failed;
        }

        b->آخري += n;

        لكل ( ;; ) {
            p = b->pos;
            سائيز = b->آخري - p;

            إذا (سائيز < 2) {
                اكسر;
            }

            qlen = (u_short) *p++ << 8;
            qlen += *p++;

            إذا (سائيز < (سائيز_t) (2 + qlen)) {
                اكسر;
            }

            ngx_resolver_process_response(r, p, qlen, 1);

            b->pos += 2 + qlen;
        }

        إذا (b->pos != b->شروعات) {
            b->آخري = ngx_movemem(b->شروعات, b->pos, b->آخري - b->pos);
            b->pos = b->شروعات;
        }
    }

    إذا (ngx_handle_read_event(rev, 0) != NGX_OK) {
        اذهب failed;
    }

    أرجع;

failed:

    ngx_close_connection(c);
    rec->tcp = عدم;
}


ساكن فراغ
ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf, سائيز_t n,
    ngx_uint_t tcp)
{
    حرف                 *err;
    ngx_uint_t            i, times, ident, qident, flags, ڪوڊ, nqs, nan, trunc,
                          qtype, qclass;
#إذا (NGX_HAVE_INET6)
    ngx_uint_t            qident6;
#جيڪڏهن_پورو
    ngx_قطار_t          *q;
    ngx_resolver_qs_t    *qs;
    ngx_resolver_hdr_t   *response;
    ngx_resolver_ڳنڍ_t  *rn;

    إذا (n < حجم(ngx_resolver_hdr_t)) {
        اذهب short_response;
    }

    response = (ngx_resolver_hdr_t *) buf;

    ident = (response->ident_hi << 8) + response->ident_lo;
    flags = (response->flags_hi << 8) + response->flags_lo;
    nqs = (response->nqs_hi << 8) + response->nqs_lo;
    nan = (response->nan_hi << 8) + response->nan_lo;
    trunc = flags & 0x0200;

    ngx_log_debug6(NGX_LOG_DEBUG_CORE, r->لاگرٿم, 0,
                   "resolver DNS response %ui fl:%04Xi %ui/%ui/%ud/%ud",
                   ident, flags, nqs, nan,
                   (response->nns_hi << 8) + response->nns_lo,
                   (response->nar_hi << 8) + response->nar_lo);

    /* response to a standard query */
    إذا ((flags & 0xf870) != 0x8000 || (trunc && tcp)) {
        ngx_log_error(r->log_level, r->لاگرٿم, 0,
                      "invalid %s DNS response %ui fl:%04Xi",
                      tcp ? "TCP" : "UDP", ident, flags);
        أرجع;
    }

    ڪوڊ = flags & 0xf;

    إذا (ڪوڊ == NGX_RESOLVE_FORMERR) {

        times = 0;

        لكل (q = ngx_قطار_سر(&r->name_resend_قطار);
             q != ngx_قطار_sentinel(&r->name_resend_قطار) && times++ < 100;
             q = ngx_قطار_ايندڙ(q))
        {
            rn = ngx_قطار_معلومات(q, ngx_resolver_ڳنڍ_t, قطار);

            إذا (rn->query) {
                qident = (rn->query[0] << 8) + rn->query[1];

                إذا (qident == ident) {
                    اذهب dns_error_name;
                }
            }

#إذا (NGX_HAVE_INET6)
            إذا (rn->query6) {
                qident6 = (rn->query6[0] << 8) + rn->query6[1];

                إذا (qident6 == ident) {
                    اذهب dns_error_name;
                }
            }
#جيڪڏهن_پورو
        }

        اذهب dns_error;
    }

    إذا (ڪوڊ > NGX_RESOLVE_REFUSED) {
        اذهب dns_error;
    }

    إذا (nqs != 1) {
        err = "invalid number of questions in DNS response";
        اذهب پورو;
    }

    i = حجم(ngx_resolver_hdr_t);

    طالما (i < (ngx_uint_t) n) {

        إذا (buf[i] & 0xc0) {
            err = "unexpected compression pointer in DNS response";
            اذهب پورو;
        }

        إذا (buf[i] == '\0') {
            اذهب مليو;
        }

        i += 1 + buf[i];
    }

    اذهب short_response;

مليو:

    إذا (i++ == حجم(ngx_resolver_hdr_t)) {
        err = "zero-length domain name in DNS response";
        اذهب پورو;
    }

    إذا (i + حجم(ngx_resolver_qs_t) + nan * (2 + حجم(ngx_resolver_an_t))
        > (ngx_uint_t) n)
    {
        اذهب short_response;
    }

    qs = (ngx_resolver_qs_t *) &buf[i];

    qtype = (qs->type_hi << 8) + qs->type_lo;
    qclass = (qs->class_hi << 8) + qs->class_lo;

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->لاگرٿم, 0,
                   "resolver DNS response qt:%ui cl:%ui", qtype, qclass);

    إذا (qclass != 1) {
        ngx_log_error(r->log_level, r->لاگرٿم, 0,
                      "unknown query class %ui in DNS response", qclass);
        أرجع;
    }

    بدل (qtype) {

    حالة NGX_RESOLVE_A:
#إذا (NGX_HAVE_INET6)
    حالة NGX_RESOLVE_AAAA:
#جيڪڏهن_پورو

        ngx_resolver_process_a(r, buf, n, ident, ڪوڊ, qtype, nan, trunc,
                               i + حجم(ngx_resolver_qs_t));

        اكسر;

    حالة NGX_RESOLVE_SRV:

        ngx_resolver_process_srv(r, buf, n, ident, ڪوڊ, nan, trunc,
                                 i + حجم(ngx_resolver_qs_t));

        اكسر;

    حالة NGX_RESOLVE_PTR:

        ngx_resolver_process_ptr(r, buf, n, ident, ڪوڊ, nan);

        اكسر;

    افتراضي:
        ngx_log_error(r->log_level, r->لاگرٿم, 0,
                      "unknown query type %ui in DNS response", qtype);
        أرجع;
    }

    أرجع;

short_response:

    err = "short DNS response";

پورو:

    ngx_log_error(r->log_level, r->لاگرٿم, 0, err);

    أرجع;

dns_error_name:

    ngx_log_error(r->log_level, r->لاگرٿم, 0,
                  "DNS error (%ui: %s), query id:%ui, name:\"%*s\"",
                  ڪوڊ, ngx_resolver_strerror(ڪوڊ), ident,
                  (سائيز_t) rn->nlen, rn->name);
    أرجع;

dns_error:

    ngx_log_error(r->log_level, r->لاگرٿم, 0,
                  "DNS error (%ui: %s), query id:%ui",
                  ڪوڊ, ngx_resolver_strerror(ڪوڊ), ident);
    أرجع;
}


ساكن فراغ
ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, سائيز_t n,
    ngx_uint_t ident, ngx_uint_t ڪوڊ, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans)
{
    حرف                       *err;
    u_char                     *cname;
    سائيز_t                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    اندريون_addr_t                  *addr;
    ngx_str_t                   name;
    ngx_uint_t                  type, class, qident, naddrs, a, i, j, شروعات;
#إذا (NGX_HAVE_INET6)
    بنية in6_addr            *addr6;
#جيڪڏهن_پورو
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *ايندڙ;
    ngx_resolver_ڳنڍ_t        *rn;
    ngx_resolver_addr_t        *addrs;
    ngx_resolver_connection_t  *rec;

    إذا (ngx_resolver_copy(r, &name, buf,
                          buf + حجم(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        أرجع;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لاگرٿم, 0, "resolver qs:%V", &name);

    hash = ngx_چڪري_جاچ_32_short(name.معلومات, name.len);

    /* lock name mutex */

    rn = ngx_resolver_lookup_name(r, &name, hash);

    إذا (rn == عدم) {
        ngx_log_error(r->log_level, r->لاگرٿم, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.معلومات);
        اذهب failed;
    }

    بدل (qtype) {

#إذا (NGX_HAVE_INET6)
    حالة NGX_RESOLVE_AAAA:

        إذا (rn->query6 == عدم || rn->naddrs6 != (u_short) -1) {
            ngx_log_error(r->log_level, r->لاگرٿم, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.معلومات);
            اذهب failed;
        }

        إذا (trunc && rn->tcp6) {
            ngx_resolver_free(r, name.معلومات);
            اذهب failed;
        }

        qident = (rn->query6[0] << 8) + rn->query6[1];

        اكسر;
#جيڪڏهن_پورو

    افتراضي: /* NGX_RESOLVE_A */

        إذا (rn->query == عدم || rn->naddrs != (u_short) -1) {
            ngx_log_error(r->log_level, r->لاگرٿم, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.معلومات);
            اذهب failed;
        }

        إذا (trunc && rn->tcp) {
            ngx_resolver_free(r, name.معلومات);
            اذهب failed;
        }

        qident = (rn->query[0] << 8) + rn->query[1];
    }

    إذا (ident != qident) {
        ngx_log_error(r->log_level, r->لاگرٿم, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.معلومات);
        اذهب failed;
    }

    ngx_resolver_free(r, name.معلومات);

    إذا (trunc) {

        ngx_قطار_remove(&rn->قطار);

        إذا (rn->انتظار == عدم) {
            ngx_rbtree_delete(&r->name_rbtree, &rn->ڳنڍ);
            ngx_resolver_free_ڳنڍ(r, rn);
            اذهب ايندڙ;
        }

        rec = r->connections.elts;
        rec = &rec[rn->آخري_connection];

        بدل (qtype) {

#إذا (NGX_HAVE_INET6)
        حالة NGX_RESOLVE_AAAA:

            rn->tcp6 = 1;

            (فراغ) ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen);

            اكسر;
#جيڪڏهن_پورو

        افتراضي: /* NGX_RESOLVE_A */

            rn->tcp = 1;

            (فراغ) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);
        }

        rn->expire = ngx_وقت() + r->resend_timeout;

        ngx_قطار_وجهو_سر(&r->name_resend_قطار, &rn->قطار);

        اذهب ايندڙ;
    }

    إذا (ڪوڊ == 0 && rn->ڪوڊ) {
        ڪوڊ = rn->ڪوڊ;
    }

    إذا (ڪوڊ == 0 && nan == 0) {

#إذا (NGX_HAVE_INET6)
        بدل (qtype) {

        حالة NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            إذا (rn->naddrs == (u_short) -1) {
                اذهب ايندڙ;
            }

            إذا (rn->naddrs) {
                اذهب export;
            }

            اكسر;

        افتراضي: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            إذا (rn->naddrs6 == (u_short) -1) {
                اذهب ايندڙ;
            }

            إذا (rn->naddrs6) {
                اذهب export;
            }
        }
#جيڪڏهن_پورو

        ڪوڊ = NGX_RESOLVE_NXDOMAIN;
    }

    إذا (ڪوڊ) {

#إذا (NGX_HAVE_INET6)
        بدل (qtype) {

        حالة NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            إذا (rn->naddrs == (u_short) -1) {
                rn->ڪوڊ = (u_char) ڪوڊ;
                اذهب ايندڙ;
            }

            اكسر;

        افتراضي: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            إذا (rn->naddrs6 == (u_short) -1) {
                rn->ڪوڊ = (u_char) ڪوڊ;
                اذهب ايندڙ;
            }
        }
#جيڪڏهن_پورو

        ايندڙ = rn->انتظار;
        rn->انتظار = عدم;

        ngx_قطار_remove(&rn->قطار);

        ngx_rbtree_delete(&r->name_rbtree, &rn->ڳنڍ);

        /* unlock name mutex */

        طالما (ايندڙ) {
            ctx = ايندڙ;
            ctx->state = ڪوڊ;
            ctx->valid = ngx_وقت() + (r->valid ? r->valid : 10);
            ايندڙ = ctx->ايندڙ;

            ctx->handler(ctx);
        }

        ngx_resolver_free_ڳنڍ(r, rn);

        أرجع;
    }

    i = ans;
    naddrs = 0;
    cname = عدم;

    لكل (a = 0; a < nan; a++) {

        شروعات = i;

        طالما (i < n) {

            إذا (buf[i] & 0xc0) {
                i += 2;
                اذهب مليو;
            }

            إذا (buf[i] == 0) {
                i++;
                اذهب test_ڊيگهائي;
            }

            i += 1 + buf[i];
        }

        اذهب short_response;

    test_ڊيگهائي:

        إذا (i - شروعات < 2) {
            err = "invalid name in DNS response";
            اذهب invalid;
        }

    مليو:

        إذا (i + حجم(ngx_resolver_an_t) >= n) {
            اذهب short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        إذا (class != 1) {
            ngx_log_error(r->log_level, r->لاگرٿم, 0,
                          "unexpected RR class %ui in DNS response", class);
            اذهب failed;
        }

        إذا (ttl < 0) {
            ttl = 0;
        }

        rn->ttl = ngx_min(rn->ttl, (uint32_t) ttl);

        i += حجم(ngx_resolver_an_t);

        بدل (type) {

        حالة NGX_RESOLVE_A:

            إذا (qtype != NGX_RESOLVE_A) {
                err = "unexpected A record in DNS response";
                اذهب invalid;
            }

            إذا (len != 4) {
                err = "invalid A record in DNS response";
                اذهب invalid;
            }

            إذا (i + 4 > n) {
                اذهب short_response;
            }

            naddrs++;

            اكسر;

#إذا (NGX_HAVE_INET6)
        حالة NGX_RESOLVE_AAAA:

            إذا (qtype != NGX_RESOLVE_AAAA) {
                err = "unexpected AAAA record in DNS response";
                اذهب invalid;
            }

            إذا (len != 16) {
                err = "invalid AAAA record in DNS response";
                اذهب invalid;
            }

            إذا (i + 16 > n) {
                اذهب short_response;
            }

            naddrs++;

            اكسر;
#جيڪڏهن_پورو

        حالة NGX_RESOLVE_CNAME:

            cname = &buf[i];

            اكسر;

        حالة NGX_RESOLVE_DNAME:

            اكسر;

        افتراضي:

            ngx_log_error(r->log_level, r->لاگرٿم, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->لاگرٿم, 0,
                   "resolver naddrs:%ui cname:%p ttl:%uD",
                   naddrs, cname, rn->ttl);

    إذا (naddrs) {

        بدل (qtype) {

#إذا (NGX_HAVE_INET6)
        حالة NGX_RESOLVE_AAAA:

            إذا (naddrs == 1) {
                addr6 = &rn->u6.addr6;
                rn->naddrs6 = 1;

            } وإلا {
                addr6 = ngx_resolver_alloc(r, naddrs * حجم(بنية in6_addr));
                إذا (addr6 == عدم) {
                    اذهب failed;
                }

                rn->u6.addrs6 = addr6;
                rn->naddrs6 = (u_short) naddrs;
            }

#إذا (NGX_SUPPRESS_WARN)
            addr = عدم;
#جيڪڏهن_پورو

            اكسر;
#جيڪڏهن_پورو

        افتراضي: /* NGX_RESOLVE_A */

            إذا (naddrs == 1) {
                addr = &rn->u.addr;
                rn->naddrs = 1;

            } وإلا {
                addr = ngx_resolver_alloc(r, naddrs * حجم(اندريون_addr_t));
                إذا (addr == عدم) {
                    اذهب failed;
                }

                rn->u.addrs = addr;
                rn->naddrs = (u_short) naddrs;
            }

#إذا (NGX_HAVE_INET6 && NGX_SUPPRESS_WARN)
            addr6 = عدم;
#جيڪڏهن_پورو
        }

        j = 0;
        i = ans;

        لكل (a = 0; a < nan; a++) {

            لكل ( ;; ) {

                إذا (buf[i] & 0xc0) {
                    i += 2;
                    اكسر;
                }

                إذا (buf[i] == 0) {
                    i++;
                    اكسر;
                }

                i += 1 + buf[i];
            }

            an = (ngx_resolver_an_t *) &buf[i];

            type = (an->type_hi << 8) + an->type_lo;
            len = (an->len_hi << 8) + an->len_lo;

            i += حجم(ngx_resolver_an_t);

            إذا (type == NGX_RESOLVE_A) {

                addr[j] = htonl((buf[i] << 24) + (buf[i + 1] << 16)
                                + (buf[i + 2] << 8) + (buf[i + 3]));

                إذا (++j == naddrs) {

#إذا (NGX_HAVE_INET6)
                    إذا (rn->naddrs6 == (u_short) -1) {
                        اذهب ايندڙ;
                    }
#جيڪڏهن_پورو

                    اكسر;
                }
            }

#إذا (NGX_HAVE_INET6)
            وإلا إذا (type == NGX_RESOLVE_AAAA) {

                ngx_memcpy(addr6[j].s6_addr, &buf[i], 16);

                إذا (++j == naddrs) {

                    إذا (rn->naddrs == (u_short) -1) {
                        اذهب ايندڙ;
                    }

                    اكسر;
                }
            }
#جيڪڏهن_پورو

            i += len;
        }
    }

    بدل (qtype) {

#إذا (NGX_HAVE_INET6)
    حالة NGX_RESOLVE_AAAA:

        إذا (rn->naddrs6 == (u_short) -1) {
            rn->naddrs6 = 0;
        }

        اكسر;
#جيڪڏهن_پورو

    افتراضي: /* NGX_RESOLVE_A */

        إذا (rn->naddrs == (u_short) -1) {
            rn->naddrs = 0;
        }
    }

    إذا (rn->naddrs != (u_short) -1
#إذا (NGX_HAVE_INET6)
        && rn->naddrs6 != (u_short) -1
#جيڪڏهن_پورو
        && rn->naddrs
#إذا (NGX_HAVE_INET6)
           + rn->naddrs6
#جيڪڏهن_پورو
           > 0)
    {

#إذا (NGX_HAVE_INET6)
    export:
#جيڪڏهن_پورو

        naddrs = rn->naddrs;
#إذا (NGX_HAVE_INET6)
        naddrs += rn->naddrs6;
#جيڪڏهن_پورو

        إذا (naddrs == 1 && rn->naddrs == 1) {
            addrs = عدم;

        } وإلا {
            addrs = ngx_resolver_export(r, rn, 0);
            إذا (addrs == عدم) {
                اذهب failed;
            }
        }

        ngx_قطار_remove(&rn->قطار);

        rn->valid = ngx_وقت() + (r->valid ? r->valid : (وقت_t) rn->ttl);
        rn->expire = ngx_وقت() + r->expire;

        ngx_قطار_وجهو_سر(&r->name_expire_قطار, &rn->قطار);

        ايندڙ = rn->انتظار;
        rn->انتظار = عدم;

        /* unlock name mutex */

        طالما (ايندڙ) {
            ctx = ايندڙ;
            ctx->state = NGX_OK;
            ctx->valid = rn->valid;
            ctx->naddrs = naddrs;

            إذا (addrs == عدم) {
                ctx->addrs = &ctx->addr;
                ctx->addr.sockaddr = (بنية sockaddr *) &ctx->جيب;
                ctx->addr.socklen = حجم(بنية sockaddr_اندريون);
                ngx_memzero(&ctx->جيب, حجم(بنية sockaddr_اندريون));
                ctx->جيب.sin_family = AF_INET;
                ctx->جيب.sin_addr.s_addr = rn->u.addr;

            } وإلا {
                ctx->addrs = addrs;
            }

            ايندڙ = ctx->ايندڙ;

            ctx->handler(ctx);
        }

        إذا (addrs != عدم) {
            ngx_resolver_free(r, addrs->sockaddr);
            ngx_resolver_free(r, addrs);
        }

        ngx_resolver_free(r, rn->query);
        rn->query = عدم;
#إذا (NGX_HAVE_INET6)
        rn->query6 = عدم;
#جيڪڏهن_پورو

        أرجع;
    }

    إذا (cname) {

        /* CNAME only */

        إذا (rn->naddrs == (u_short) -1
#إذا (NGX_HAVE_INET6)
            || rn->naddrs6 == (u_short) -1
#جيڪڏهن_پورو
            )
        {
            اذهب ايندڙ;
        }

        إذا (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            اذهب failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لاگرٿم, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_قطار_remove(&rn->قطار);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.معلومات;

        rn->valid = ngx_وقت() + (r->valid ? r->valid : (وقت_t) rn->ttl);
        rn->expire = ngx_وقت() + r->expire;

        ngx_قطار_وجهو_سر(&r->name_expire_قطار, &rn->قطار);

        ngx_resolver_free(r, rn->query);
        rn->query = عدم;
#إذا (NGX_HAVE_INET6)
        rn->query6 = عدم;
#جيڪڏهن_پورو

        ctx = rn->انتظار;
        rn->انتظار = عدم;

        إذا (ctx) {

            إذا (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                افعل {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    ايندڙ = ctx->ايندڙ;

                    ctx->handler(ctx);

                    ctx = ايندڙ;
                } طالما (ctx);

                أرجع;
            }

            لكل (ايندڙ = ctx; ايندڙ; ايندڙ = ايندڙ->ايندڙ) {
                ايندڙ->ڳنڍ = عدم;
            }

            (فراغ) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        أرجع;
    }

    ngx_log_error(r->log_level, r->لاگرٿم, 0,
                  "no A or CNAME types in DNS response");
    أرجع;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->لاگرٿم, 0, err);

    أرجع;

failed:

ايندڙ:

    /* unlock name mutex */

    أرجع;
}


ساكن فراغ
ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, سائيز_t n,
    ngx_uint_t ident, ngx_uint_t ڪوڊ, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans)
{
    حرف                       *err;
    u_char                     *cname;
    سائيز_t                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    ngx_str_t                   name;
    ngx_uint_t                  type, qident, class, شروعات, nsrvs, a, i, j;
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *ايندڙ;
    ngx_resolver_srv_t         *srvs;
    ngx_resolver_ڳنڍ_t        *rn;
    ngx_resolver_connection_t  *rec;

    إذا (ngx_resolver_copy(r, &name, buf,
                          buf + حجم(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        أرجع;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لاگرٿم, 0, "resolver qs:%V", &name);

    hash = ngx_چڪري_جاچ_32_short(name.معلومات, name.len);

    rn = ngx_resolver_lookup_srv(r, &name, hash);

    إذا (rn == عدم || rn->query == عدم) {
        ngx_log_error(r->log_level, r->لاگرٿم, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.معلومات);
        اذهب failed;
    }

    إذا (trunc && rn->tcp) {
        ngx_resolver_free(r, name.معلومات);
        اذهب failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    إذا (ident != qident) {
        ngx_log_error(r->log_level, r->لاگرٿم, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.معلومات);
        اذهب failed;
    }

    ngx_resolver_free(r, name.معلومات);

    إذا (trunc) {

        ngx_قطار_remove(&rn->قطار);

        إذا (rn->انتظار == عدم) {
            ngx_rbtree_delete(&r->srv_rbtree, &rn->ڳنڍ);
            ngx_resolver_free_ڳنڍ(r, rn);
            أرجع;
        }

        rec = r->connections.elts;
        rec = &rec[rn->آخري_connection];

        rn->tcp = 1;

        (فراغ) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);

        rn->expire = ngx_وقت() + r->resend_timeout;

        ngx_قطار_وجهو_سر(&r->srv_resend_قطار, &rn->قطار);

        أرجع;
    }

    إذا (ڪوڊ == 0 && rn->ڪوڊ) {
        ڪوڊ = rn->ڪوڊ;
    }

    إذا (ڪوڊ == 0 && nan == 0) {
        ڪوڊ = NGX_RESOLVE_NXDOMAIN;
    }

    إذا (ڪوڊ) {
        ايندڙ = rn->انتظار;
        rn->انتظار = عدم;

        ngx_قطار_remove(&rn->قطار);

        ngx_rbtree_delete(&r->srv_rbtree, &rn->ڳنڍ);

        طالما (ايندڙ) {
            ctx = ايندڙ;
            ctx->state = ڪوڊ;
            ctx->valid = ngx_وقت() + (r->valid ? r->valid : 10);
            ايندڙ = ctx->ايندڙ;

            ctx->handler(ctx);
        }

        ngx_resolver_free_ڳنڍ(r, rn);

        أرجع;
    }

    i = ans;
    nsrvs = 0;
    cname = عدم;

    لكل (a = 0; a < nan; a++) {

        شروعات = i;

        طالما (i < n) {

            إذا (buf[i] & 0xc0) {
                i += 2;
                اذهب مليو;
            }

            إذا (buf[i] == 0) {
                i++;
                اذهب test_ڊيگهائي;
            }

            i += 1 + buf[i];
        }

        اذهب short_response;

    test_ڊيگهائي:

        إذا (i - شروعات < 2) {
            err = "invalid name DNS response";
            اذهب invalid;
        }

    مليو:

        إذا (i + حجم(ngx_resolver_an_t) >= n) {
            اذهب short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        إذا (class != 1) {
            ngx_log_error(r->log_level, r->لاگرٿم, 0,
                          "unexpected RR class %ui in DNS response", class);
            اذهب failed;
        }

        إذا (ttl < 0) {
            ttl = 0;
        }

        rn->ttl = ngx_min(rn->ttl, (uint32_t) ttl);

        i += حجم(ngx_resolver_an_t);

        بدل (type) {

        حالة NGX_RESOLVE_SRV:

            إذا (i + 6 >= n) {
                اذهب short_response;
            }

            إذا (ngx_resolver_copy(r, عدم, buf, &buf[i + 6], buf + n)
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

            ngx_log_error(r->log_level, r->لاگرٿم, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->لاگرٿم, 0,
                   "resolver nsrvs:%ui cname:%p ttl:%uD",
                   nsrvs, cname, rn->ttl);

    إذا (nsrvs) {

        srvs = ngx_resolver_calloc(r, nsrvs * حجم(ngx_resolver_srv_t));
        إذا (srvs == عدم) {
            اذهب failed;
        }

        rn->u.srvs = srvs;
        rn->nsrvs = (u_short) nsrvs;

        j = 0;
        i = ans;

        لكل (a = 0; a < nan; a++) {

            لكل ( ;; ) {

                إذا (buf[i] & 0xc0) {
                    i += 2;
                    اكسر;
                }

                إذا (buf[i] == 0) {
                    i++;
                    اكسر;
                }

                i += 1 + buf[i];
            }

            an = (ngx_resolver_an_t *) &buf[i];

            type = (an->type_hi << 8) + an->type_lo;
            len = (an->len_hi << 8) + an->len_lo;

            i += حجم(ngx_resolver_an_t);

            إذا (type == NGX_RESOLVE_SRV) {

                srvs[j].ترجيح = (buf[i] << 8) + buf[i + 1];
                srvs[j].وزن = (buf[i + 2] << 8) + buf[i + 3];

                إذا (srvs[j].وزن == 0) {
                    srvs[j].وزن = 1;
                }

                srvs[j].پورٽ = (buf[i + 4] << 8) + buf[i + 5];

                إذا (ngx_resolver_copy(r, &srvs[j].name, buf, &buf[i + 6],
                                      buf + n)
                    != NGX_OK)
                {
                    اذهب failed;
                }

                j++;
            }

            i += len;
        }

        ngx_ترتيب_ڏيو(srvs, nsrvs, حجم(ngx_resolver_srv_t),
                 ngx_resolver_cmp_srvs);

        ngx_resolver_free(r, rn->query);
        rn->query = عدم;

        ngx_قطار_remove(&rn->قطار);

        rn->valid = ngx_وقت() + (r->valid ? r->valid : (وقت_t) rn->ttl);
        rn->expire = ngx_وقت() + r->expire;

        ngx_قطار_وجهو_سر(&r->srv_expire_قطار, &rn->قطار);

        ايندڙ = rn->انتظار;
        rn->انتظار = عدم;

        طالما (ايندڙ) {
            ctx = ايندڙ;
            ايندڙ = ctx->ايندڙ;

            ngx_resolver_resolve_srv_نالا(ctx, rn);
        }

        أرجع;
    }

    rn->nsrvs = 0;

    إذا (cname) {

        /* CNAME only */

        إذا (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            اذهب failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لاگرٿم, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_قطار_remove(&rn->قطار);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.معلومات;

        rn->valid = ngx_وقت() + (r->valid ? r->valid : (وقت_t) rn->ttl);
        rn->expire = ngx_وقت() + r->expire;

        ngx_قطار_وجهو_سر(&r->srv_expire_قطار, &rn->قطار);

        ngx_resolver_free(r, rn->query);
        rn->query = عدم;
#إذا (NGX_HAVE_INET6)
        rn->query6 = عدم;
#جيڪڏهن_پورو

        ctx = rn->انتظار;
        rn->انتظار = عدم;

        إذا (ctx) {

            إذا (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                افعل {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    ايندڙ = ctx->ايندڙ;

                    ctx->handler(ctx);

                    ctx = ايندڙ;
                } طالما (ctx);

                أرجع;
            }

            لكل (ايندڙ = ctx; ايندڙ; ايندڙ = ايندڙ->ايندڙ) {
                ايندڙ->ڳنڍ = عدم;
            }

            (فراغ) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        أرجع;
    }

    ngx_log_error(r->log_level, r->لاگرٿم, 0, "no SRV type in DNS response");

    أرجع;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->لاگرٿم, 0, err);

    أرجع;

failed:

    /* unlock name mutex */

    أرجع;
}


ساكن فراغ
ngx_resolver_resolve_srv_نالا(ngx_resolver_ctx_t *ctx, ngx_resolver_ڳنڍ_t *rn)
{
    ngx_uint_t                i;
    ngx_resolver_t           *r;
    ngx_resolver_ctx_t       *cctx;
    ngx_resolver_srv_name_t  *srvs;

    r = ctx->resolver;

    ctx->ڳنڍ = عدم;
    ctx->state = NGX_OK;
    ctx->valid = rn->valid;
    ctx->ڳڻپ = rn->nsrvs;

    srvs = ngx_resolver_calloc(r, rn->nsrvs * حجم(ngx_resolver_srv_name_t));
    إذا (srvs == عدم) {
        اذهب failed;
    }

    ctx->srvs = srvs;
    ctx->nsrvs = rn->nsrvs;

    إذا (ctx->event && ctx->event->timer_سيٽ) {
        ngx_del_timer(ctx->event);
    }

    لكل (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
        srvs[i].name.معلومات = ngx_resolver_alloc(r, rn->u.srvs[i].name.len);
        إذا (srvs[i].name.معلومات == عدم) {
            اذهب failed;
        }

        srvs[i].name.len = rn->u.srvs[i].name.len;
        ngx_memcpy(srvs[i].name.معلومات, rn->u.srvs[i].name.معلومات,
                   srvs[i].name.len);

        cctx = ngx_resolve_شروعات(r, عدم);
        إذا (cctx == عدم) {
            اذهب failed;
        }

        cctx->name = srvs[i].name;
        cctx->handler = ngx_resolver_srv_نالا_handler;
        cctx->معلومات = ctx;
        cctx->srvs = &srvs[i];
        cctx->timeout = ctx->timeout;

        srvs[i].ترجيح = rn->u.srvs[i].ترجيح;
        srvs[i].وزن = rn->u.srvs[i].وزن;
        srvs[i].پورٽ = rn->u.srvs[i].پورٽ;
        srvs[i].ctx = cctx;

        إذا (ngx_resolve_name(cctx) == NGX_ERROR) {
            srvs[i].ctx = عدم;
            اذهب failed;
        }
    }

    أرجع;

failed:

    ctx->state = NGX_ERROR;
    ctx->valid = ngx_وقت() + (r->valid ? r->valid : 10);

    ctx->handler(ctx);
}


ساكن فراغ
ngx_resolver_srv_نالا_handler(ngx_resolver_ctx_t *cctx)
{
    ngx_uint_t                i;
    ngx_addr_t               *addrs;
    ngx_resolver_t           *r;
    ngx_sockaddr_t           *sockaddr;
    ngx_resolver_ctx_t       *ctx;
    ngx_resolver_srv_name_t  *srv;

    r = cctx->resolver;
    ctx = cctx->معلومات;
    srv = cctx->srvs;

    ctx->ڳڻپ--;
    ctx->async |= cctx->async;

    srv->ctx = عدم;
    srv->state = cctx->state;

    إذا (cctx->naddrs) {

        ctx->valid = ngx_min(ctx->valid, cctx->valid);

        addrs = ngx_resolver_calloc(r, cctx->naddrs * حجم(ngx_addr_t));
        إذا (addrs == عدم) {
            srv->state = NGX_ERROR;
            اذهب پورو;
        }

        sockaddr = ngx_resolver_alloc(r, cctx->naddrs * حجم(ngx_sockaddr_t));
        إذا (sockaddr == عدم) {
            ngx_resolver_free(r, addrs);
            srv->state = NGX_ERROR;
            اذهب پورو;
        }

        لكل (i = 0; i < cctx->naddrs; i++) {
            addrs[i].sockaddr = &sockaddr[i].sockaddr;
            addrs[i].socklen = cctx->addrs[i].socklen;

            ngx_memcpy(&sockaddr[i], cctx->addrs[i].sockaddr,
                       addrs[i].socklen);

            ngx_inet_سيٽ_پورٽ(addrs[i].sockaddr, srv->پورٽ);
        }

        srv->addrs = addrs;
        srv->naddrs = cctx->naddrs;
    }

پورو:

    ngx_resolve_name_پورو(cctx);

    إذا (ctx->ڳڻپ == 0) {
        ngx_resolver_report_srv(r, ctx);
    }
}


ساكن فراغ
ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, سائيز_t n,
    ngx_uint_t ident, ngx_uint_t ڪوڊ, ngx_uint_t nan)
{
    حرف                 *err;
    سائيز_t                len;
    اندريون_addr_t             addr;
    int32_t               ttl;
    ngx_int_t             octet;
    ngx_str_t             name;
    ngx_uint_t            نقاب, type, class, qident, a, i, شروعات;
    ngx_قطار_t          *expire_قطار;
    ngx_rbtree_t         *وڻ;
    ngx_resolver_an_t    *an;
    ngx_resolver_ctx_t   *ctx, *ايندڙ;
    ngx_resolver_ڳنڍ_t  *rn;
#إذا (NGX_HAVE_INET6)
    uint32_t              hash;
    ngx_int_t             digit;
    بنية in6_addr       addr6;
#جيڪڏهن_پورو

    إذا (ngx_resolver_copy(r, &name, buf,
                          buf + حجم(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        أرجع;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لاگرٿم, 0, "resolver qs:%V", &name);

    /* AF_INET */

    addr = 0;
    i = حجم(ngx_resolver_hdr_t);

    لكل (نقاب = 0; نقاب < 32; نقاب += 8) {
        len = buf[i++];

        octet = ngx_atoi(&buf[i], len);
        إذا (octet == NGX_ERROR || octet > 255) {
            اذهب invalid_اندريون_addr_arpa;
        }

        addr += octet << نقاب;
        i += len;
    }

    إذا (ngx_strcasecmp(&buf[i], (u_char *) "\7in-addr\4arpa") == 0) {
        i += حجم("\7in-addr\4arpa");

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        وڻ = &r->addr_rbtree;
        expire_قطار = &r->addr_expire_قطار;

        اذهب valid;
    }

invalid_اندريون_addr_arpa:

#إذا (NGX_HAVE_INET6)

    i = حجم(ngx_resolver_hdr_t);

    لكل (octet = 15; octet >= 0; octet--) {
        إذا (buf[i++] != '\1') {
            اذهب invalid_ip6_arpa;
        }

        digit = ngx_hextoi(&buf[i++], 1);
        إذا (digit == NGX_ERROR) {
            اذهب invalid_ip6_arpa;
        }

        addr6.s6_addr[octet] = (u_char) digit;

        إذا (buf[i++] != '\1') {
            اذهب invalid_ip6_arpa;
        }

        digit = ngx_hextoi(&buf[i++], 1);
        إذا (digit == NGX_ERROR) {
            اذهب invalid_ip6_arpa;
        }

        addr6.s6_addr[octet] += (u_char) (digit * 16);
    }

    إذا (ngx_strcasecmp(&buf[i], (u_char *) "\3ip6\4arpa") == 0) {
        i += حجم("\3ip6\4arpa");

        /* lock addr mutex */

        hash = ngx_چڪري_جاچ_32_short(addr6.s6_addr, 16);
        rn = ngx_resolver_lookup_addr6(r, &addr6, hash);

        وڻ = &r->addr6_rbtree;
        expire_قطار = &r->addr6_expire_قطار;

        اذهب valid;
    }

invalid_ip6_arpa:
#جيڪڏهن_پورو

    ngx_log_error(r->log_level, r->لاگرٿم, 0,
                  "invalid in-addr.arpa or ip6.arpa name in DNS response");
    ngx_resolver_free(r, name.معلومات);
    أرجع;

valid:

    إذا (rn == عدم || rn->query == عدم) {
        ngx_log_error(r->log_level, r->لاگرٿم, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.معلومات);
        اذهب failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    إذا (ident != qident) {
        ngx_log_error(r->log_level, r->لاگرٿم, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.معلومات);
        اذهب failed;
    }

    ngx_resolver_free(r, name.معلومات);

    إذا (ڪوڊ == 0 && nan == 0) {
        ڪوڊ = NGX_RESOLVE_NXDOMAIN;
    }

    إذا (ڪوڊ) {
        ايندڙ = rn->انتظار;
        rn->انتظار = عدم;

        ngx_قطار_remove(&rn->قطار);

        ngx_rbtree_delete(وڻ, &rn->ڳنڍ);

        /* unlock addr mutex */

        طالما (ايندڙ) {
            ctx = ايندڙ;
            ctx->state = ڪوڊ;
            ctx->valid = ngx_وقت() + (r->valid ? r->valid : 10);
            ايندڙ = ctx->ايندڙ;

            ctx->handler(ctx);
        }

        ngx_resolver_free_ڳنڍ(r, rn);

        أرجع;
    }

    i += حجم(ngx_resolver_qs_t);

    لكل (a = 0; a < nan; a++) {

        شروعات = i;

        طالما (i < n) {

            إذا (buf[i] & 0xc0) {
                i += 2;
                اذهب مليو;
            }

            إذا (buf[i] == 0) {
                i++;
                اذهب test_ڊيگهائي;
            }

            i += 1 + buf[i];
        }

        اذهب short_response;

    test_ڊيگهائي:

        إذا (i - شروعات < 2) {
            err = "invalid name in DNS response";
            اذهب invalid;
        }

    مليو:

        إذا (i + حجم(ngx_resolver_an_t) >= n) {
            اذهب short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        إذا (class != 1) {
            ngx_log_error(r->log_level, r->لاگرٿم, 0,
                          "unexpected RR class %ui in DNS response", class);
            اذهب failed;
        }

        إذا (ttl < 0) {
            ttl = 0;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->لاگرٿم, 0,
                      "resolver qt:%ui cl:%ui len:%uz",
                      type, class, len);

        i += حجم(ngx_resolver_an_t);

        بدل (type) {

        حالة NGX_RESOLVE_PTR:

            اذهب ptr;

        حالة NGX_RESOLVE_CNAME:

            اكسر;

        افتراضي:

            ngx_log_error(r->log_level, r->لاگرٿم, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->لاگرٿم, 0,
                  "no PTR type in DNS response");
    أرجع;

ptr:

    إذا (ngx_resolver_copy(r, &name, buf, buf + i, buf + n) != NGX_OK) {
        اذهب failed;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لاگرٿم, 0, "resolver an:%V", &name);

    إذا (name.len != (سائيز_t) rn->nlen
        || ngx_strncmp(name.معلومات, rn->name, name.len) != 0)
    {
        إذا (rn->nlen) {
            ngx_resolver_free(r, rn->name);
        }

        rn->nlen = (u_short) name.len;
        rn->name = name.معلومات;

        name.معلومات = ngx_resolver_dup(r, rn->name, name.len);
        إذا (name.معلومات == عدم) {
            اذهب failed;
        }
    }

    ngx_قطار_remove(&rn->قطار);

    rn->valid = ngx_وقت() + (r->valid ? r->valid : ttl);
    rn->expire = ngx_وقت() + r->expire;

    ngx_قطار_وجهو_سر(expire_قطار, &rn->قطار);

    ايندڙ = rn->انتظار;
    rn->انتظار = عدم;

    /* unlock addr mutex */

    طالما (ايندڙ) {
        ctx = ايندڙ;
        ctx->state = NGX_OK;
        ctx->valid = rn->valid;
        ctx->name = name;
        ايندڙ = ctx->ايندڙ;

        ctx->handler(ctx);
    }

    ngx_resolver_free(r, name.معلومات);

    أرجع;

short_response:

    err = "short DNS response";

invalid:

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->لاگرٿم, 0, err);

    أرجع;

failed:

    /* unlock addr mutex */

    أرجع;
}


ساكن ngx_resolver_ڳنڍ_t *
ngx_resolver_lookup_name(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_ڳنڍ_t    *ڳنڍ, *sentinel;
    ngx_resolver_ڳنڍ_t  *rn;

    ڳنڍ = r->name_rbtree.پاڙ;
    sentinel = r->name_rbtree.sentinel;

    طالما (ڳنڍ != sentinel) {

        إذا (hash < ڳنڍ->ڪنجي) {
            ڳنڍ = ڳنڍ->کاٻو;
            استمر;
        }

        إذا (hash > ڳنڍ->ڪنجي) {
            ڳنڍ = ڳنڍ->ساڄو;
            استمر;
        }

        /* hash == node->key */

        rn = ngx_resolver_ڳنڍ(ڳنڍ);

        rc = ngx_memn2cmp(name->معلومات, rn->name, name->len, rn->nlen);

        إذا (rc == 0) {
            أرجع rn;
        }

        ڳنڍ = (rc < 0) ? ڳنڍ->کاٻو : ڳنڍ->ساڄو;
    }

    /* not found */

    أرجع عدم;
}


ساكن ngx_resolver_ڳنڍ_t *
ngx_resolver_lookup_srv(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_ڳنڍ_t    *ڳنڍ, *sentinel;
    ngx_resolver_ڳنڍ_t  *rn;

    ڳنڍ = r->srv_rbtree.پاڙ;
    sentinel = r->srv_rbtree.sentinel;

    طالما (ڳنڍ != sentinel) {

        إذا (hash < ڳنڍ->ڪنجي) {
            ڳنڍ = ڳنڍ->کاٻو;
            استمر;
        }

        إذا (hash > ڳنڍ->ڪنجي) {
            ڳنڍ = ڳنڍ->ساڄو;
            استمر;
        }

        /* hash == node->key */

        rn = ngx_resolver_ڳنڍ(ڳنڍ);

        rc = ngx_memn2cmp(name->معلومات, rn->name, name->len, rn->nlen);

        إذا (rc == 0) {
            أرجع rn;
        }

        ڳنڍ = (rc < 0) ? ڳنڍ->کاٻو : ڳنڍ->ساڄو;
    }

    /* not found */

    أرجع عدم;
}


ساكن ngx_resolver_ڳنڍ_t *
ngx_resolver_lookup_addr(ngx_resolver_t *r, اندريون_addr_t addr)
{
    ngx_rbtree_ڳنڍ_t  *ڳنڍ, *sentinel;

    ڳنڍ = r->addr_rbtree.پاڙ;
    sentinel = r->addr_rbtree.sentinel;

    طالما (ڳنڍ != sentinel) {

        إذا (addr < ڳنڍ->ڪنجي) {
            ڳنڍ = ڳنڍ->کاٻو;
            استمر;
        }

        إذا (addr > ڳنڍ->ڪنجي) {
            ڳنڍ = ڳنڍ->ساڄو;
            استمر;
        }

        /* addr == node->key */

        أرجع ngx_resolver_ڳنڍ(ڳنڍ);
    }

    /* not found */

    أرجع عدم;
}


#إذا (NGX_HAVE_INET6)

ساكن ngx_resolver_ڳنڍ_t *
ngx_resolver_lookup_addr6(ngx_resolver_t *r, بنية in6_addr *addr,
    uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_ڳنڍ_t    *ڳنڍ, *sentinel;
    ngx_resolver_ڳنڍ_t  *rn;

    ڳنڍ = r->addr6_rbtree.پاڙ;
    sentinel = r->addr6_rbtree.sentinel;

    طالما (ڳنڍ != sentinel) {

        إذا (hash < ڳنڍ->ڪنجي) {
            ڳنڍ = ڳنڍ->کاٻو;
            استمر;
        }

        إذا (hash > ڳنڍ->ڪنجي) {
            ڳنڍ = ڳنڍ->ساڄو;
            استمر;
        }

        /* hash == node->key */

        rn = ngx_resolver_ڳنڍ(ڳنڍ);

        rc = ngx_memcmp(addr, &rn->addr6, 16);

        إذا (rc == 0) {
            أرجع rn;
        }

        ڳنڍ = (rc < 0) ? ڳنڍ->کاٻو : ڳنڍ->ساڄو;
    }

    /* not found */

    أرجع عدم;
}

#جيڪڏهن_پورو


ساكن فراغ
ngx_resolver_rbtree_وجهو_قدر(ngx_rbtree_ڳنڍ_t *عارضي,
    ngx_rbtree_ڳنڍ_t *ڳنڍ, ngx_rbtree_ڳنڍ_t *sentinel)
{
    ngx_rbtree_ڳنڍ_t    **p;
    ngx_resolver_ڳنڍ_t   *rn, *rn_عارضي;

    لكل ( ;; ) {

        إذا (ڳنڍ->ڪنجي < عارضي->ڪنجي) {

            p = &عارضي->کاٻو;

        } وإلا إذا (ڳنڍ->ڪنجي > عارضي->ڪنجي) {

            p = &عارضي->ساڄو;

        } وإلا { /* node->key == temp->key */

            rn = ngx_resolver_ڳنڍ(ڳنڍ);
            rn_عارضي = ngx_resolver_ڳنڍ(عارضي);

            p = (ngx_memn2cmp(rn->name, rn_عارضي->name, rn->nlen, rn_عارضي->nlen)
                 < 0) ? &عارضي->کاٻو : &عارضي->ساڄو;
        }

        إذا (*p == sentinel) {
            اكسر;
        }

        عارضي = *p;
    }

    *p = ڳنڍ;
    ڳنڍ->والد = عارضي;
    ڳنڍ->کاٻو = sentinel;
    ڳنڍ->ساڄو = sentinel;
    ngx_rbt_red(ڳنڍ);
}


#إذا (NGX_HAVE_INET6)

ساكن فراغ
ngx_resolver_rbtree_وجهو_addr6_قدر(ngx_rbtree_ڳنڍ_t *عارضي,
    ngx_rbtree_ڳنڍ_t *ڳنڍ, ngx_rbtree_ڳنڍ_t *sentinel)
{
    ngx_rbtree_ڳنڍ_t    **p;
    ngx_resolver_ڳنڍ_t   *rn, *rn_عارضي;

    لكل ( ;; ) {

        إذا (ڳنڍ->ڪنجي < عارضي->ڪنجي) {

            p = &عارضي->کاٻو;

        } وإلا إذا (ڳنڍ->ڪنجي > عارضي->ڪنجي) {

            p = &عارضي->ساڄو;

        } وإلا { /* node->key == temp->key */

            rn = ngx_resolver_ڳنڍ(ڳنڍ);
            rn_عارضي = ngx_resolver_ڳنڍ(عارضي);

            p = (ngx_memcmp(&rn->addr6, &rn_عارضي->addr6, 16)
                 < 0) ? &عارضي->کاٻو : &عارضي->ساڄو;
        }

        إذا (*p == sentinel) {
            اكسر;
        }

        عارضي = *p;
    }

    *p = ڳنڍ;
    ڳنڍ->والد = عارضي;
    ڳنڍ->کاٻو = sentinel;
    ڳنڍ->ساڄو = sentinel;
    ngx_rbt_red(ڳنڍ);
}

#جيڪڏهن_پورو


ساكن ngx_int_t
ngx_resolver_create_name_query(ngx_resolver_t *r, ngx_resolver_ڳنڍ_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    سائيز_t               len, nlen;
    ngx_uint_t           ident;
    ngx_resolver_qs_t   *qs;
    ngx_resolver_hdr_t  *query;

    nlen = name->len ? (1 + name->len + 1) : 1;

    len = حجم(ngx_resolver_hdr_t) + nlen + حجم(ngx_resolver_qs_t);

#إذا (NGX_HAVE_INET6)
    p = ngx_resolver_alloc(r, len * (r->ipv4 + r->ipv6));
#وإلا
    p = ngx_resolver_alloc(r, len);
#جيڪڏهن_پورو
    إذا (p == عدم) {
        أرجع NGX_ERROR;
    }

    rn->qlen = (u_short) len;
    rn->query = p;

#إذا (NGX_HAVE_INET6)
    إذا (r->ipv6) {
        rn->query6 = r->ipv4 ? (p + len) : p;
    }
#جيڪڏهن_پورو

    query = (ngx_resolver_hdr_t *) p;

    إذا (r->ipv4) {
        ident = ngx_random();

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->لاگرٿم, 0,
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

    إذا (name->len == 0)  {
        أرجع NGX_DECLINED;
    }

    لكل (s = name->معلومات + name->len - 1; s >= name->معلومات; s--) {
        إذا (*s != '.') {
            *p = *s;
            len++;

        } وإلا {
            إذا (len == 0 || len > 255) {
                أرجع NGX_DECLINED;
            }

            *p = (u_char) len;
            len = 0;
        }

        p--;
    }

    إذا (len == 0 || len > 255) {
        أرجع NGX_DECLINED;
    }

    *p = (u_char) len;

#إذا (NGX_HAVE_INET6)
    إذا (!r->ipv6) {
        أرجع NGX_OK;
    }

    p = rn->query6;

    إذا (r->ipv4) {
        ngx_memcpy(p, rn->query, rn->qlen);
    }

    query = (ngx_resolver_hdr_t *) p;

    ident = ngx_random();

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->لاگرٿم, 0,
                   "resolve: \"%V\" AAAA %i", name, ident & 0xffff);

    query->ident_hi = (u_char) ((ident >> 8) & 0xff);
    query->ident_lo = (u_char) (ident & 0xff);

    p += حجم(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    qs->type_lo = NGX_RESOLVE_AAAA;
#جيڪڏهن_پورو

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_resolver_create_srv_query(ngx_resolver_t *r, ngx_resolver_ڳنڍ_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    سائيز_t               len, nlen;
    ngx_uint_t           ident;
    ngx_resolver_qs_t   *qs;
    ngx_resolver_hdr_t  *query;

    nlen = name->len ? (1 + name->len + 1) : 1;

    len = حجم(ngx_resolver_hdr_t) + nlen + حجم(ngx_resolver_qs_t);

    p = ngx_resolver_alloc(r, len);
    إذا (p == عدم) {
        أرجع NGX_ERROR;
    }

    rn->qlen = (u_short) len;
    rn->query = p;

    query = (ngx_resolver_hdr_t *) p;

    ident = ngx_random();

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->لاگرٿم, 0,
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

    إذا (name->len == 0)  {
        أرجع NGX_DECLINED;
    }

    لكل (s = name->معلومات + name->len - 1; s >= name->معلومات; s--) {
        إذا (*s != '.') {
            *p = *s;
            len++;

        } وإلا {
            إذا (len == 0 || len > 255) {
                أرجع NGX_DECLINED;
            }

            *p = (u_char) len;
            len = 0;
        }

        p--;
    }

    إذا (len == 0 || len > 255) {
        أرجع NGX_DECLINED;
    }

    *p = (u_char) len;

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_resolver_create_addr_query(ngx_resolver_t *r, ngx_resolver_ڳنڍ_t *rn,
    ngx_resolver_addr_t *addr)
{
    u_char               *p, *d;
    سائيز_t                len;
    اندريون_addr_t             inaddr;
    ngx_int_t             n;
    ngx_uint_t            ident;
    ngx_resolver_hdr_t   *query;
    بنية sockaddr_اندريون   *جيب;
#إذا (NGX_HAVE_INET6)
    بنية sockaddr_in6  *sin6;
#جيڪڏهن_پورو

    بدل (addr->sockaddr->sa_family) {

#إذا (NGX_HAVE_INET6)
    حالة AF_INET6:
        len = حجم(ngx_resolver_hdr_t)
              + 64 + حجم(".ip6.arpa.") - 1
              + حجم(ngx_resolver_qs_t);

        اكسر;
#جيڪڏهن_پورو

    افتراضي: /* AF_INET */
        len = حجم(ngx_resolver_hdr_t)
              + حجم(".255.255.255.255.in-addr.arpa.") - 1
              + حجم(ngx_resolver_qs_t);
    }

    p = ngx_resolver_alloc(r, len);
    إذا (p == عدم) {
        أرجع NGX_ERROR;
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

#إذا (NGX_HAVE_INET6)
    حالة AF_INET6:
        sin6 = (بنية sockaddr_in6 *) addr->sockaddr;

        لكل (n = 15; n >= 0; n--) {
            p = ngx_sprintf(p, "\1%xd\1%xd",
                            sin6->sin6_addr.s6_addr[n] & 0xf,
                            (sin6->sin6_addr.s6_addr[n] >> 4) & 0xf);
        }

        p = ngx_cpymem(p, "\3ip6\4arpa\0", 10);

        اكسر;
#جيڪڏهن_پورو

    افتراضي: /* AF_INET */

        جيب = (بنية sockaddr_اندريون *) addr->sockaddr;
        inaddr = ntohl(جيب->sin_addr.s_addr);

        لكل (n = 0; n < 32; n += 8) {
            d = ngx_sprintf(&p[1], "%ud", (inaddr >> n) & 0xff);
            *p = (u_char) (d - &p[1]);
            p = d;
        }

        p = ngx_cpymem(p, "\7in-addr\4arpa\0", 14);
    }

    /* query type "PTR", IN query class */
    p = ngx_cpymem(p, "\0\14\0\1", 4);

    rn->qlen = (u_short) (p - rn->query);

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_resolver_copy(ngx_resolver_t *r, ngx_str_t *name, u_char *buf, u_char *src,
    u_char *آخري)
{
    حرف        *err;
    u_char      *p, *dst;
    سائيز_t       len;
    ngx_uint_t   i, n;

    p = src;
    len = 0;

    /*
     * compression pointers allow to create endless loop, so we set limit;
     * 128 pointers should be enough to store 255-byte name
     */

    لكل (i = 0; i < 128; i++) {
        n = *p++;

        إذا (n == 0) {
            اذهب پورو;
        }

        إذا (n & 0xc0) {
            إذا ((n & 0xc0) != 0xc0) {
                err = "invalid label type in DNS response";
                اذهب invalid;
            }

            إذا (p >= آخري) {
                err = "name is out of DNS response";
                اذهب invalid;
            }

            n = ((n & 0x3f) << 8) + *p;
            p = &buf[n];

        } وإلا {
            len += 1 + n;
            p = &p[n];
        }

        إذا (p >= آخري) {
            err = "name is out of DNS response";
            اذهب invalid;
        }
    }

    err = "compression pointers loop in DNS response";

invalid:

    ngx_log_error(r->log_level, r->لاگرٿم, 0, err);

    أرجع NGX_ERROR;

پورو:

    إذا (name == عدم) {
        أرجع NGX_OK;
    }

    إذا (len == 0) {
        ngx_str_null(name);
        أرجع NGX_OK;
    }

    dst = ngx_resolver_alloc(r, len);
    إذا (dst == عدم) {
        أرجع NGX_ERROR;
    }

    name->معلومات = dst;

    لكل ( ;; ) {
        n = *src++;

        إذا (n == 0) {
            name->len = dst - name->معلومات - 1;
            أرجع NGX_OK;
        }

        إذا (n & 0xc0) {
            n = ((n & 0x3f) << 8) + *src;
            src = &buf[n];

        } وإلا {
            ngx_strlow(dst, src, n);
            dst += n;
            src += n;
            *dst++ = '.';
        }
    }
}


ساكن ngx_int_t
ngx_resolver_سيٽ_timeout(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx)
{
    إذا (ctx->event || ctx->timeout == 0) {
        أرجع NGX_OK;
    }

    ctx->event = ngx_resolver_calloc(r, حجم(ngx_event_t));
    إذا (ctx->event == عدم) {
        أرجع NGX_ERROR;
    }

    ctx->event->handler = ngx_resolver_timeout_handler;
    ctx->event->معلومات = ctx;
    ctx->event->لاگرٿم = r->لاگرٿم;
    ctx->event->cancelable = ctx->cancelable;
    ctx->ident = -1;

    ngx_جوڙيو_timer(ctx->event, ctx->timeout);

    أرجع NGX_OK;
}


ساكن فراغ
ngx_resolver_timeout_handler(ngx_event_t *ev)
{
    ngx_resolver_ctx_t  *ctx;

    ctx = ev->معلومات;

    ctx->state = NGX_RESOLVE_TIMEDOUT;

    ctx->handler(ctx);
}


ساكن فراغ
ngx_resolver_free_ڳنڍ(ngx_resolver_t *r, ngx_resolver_ڳنڍ_t *rn)
{
    ngx_uint_t  i;

    /* lock alloc mutex */

    إذا (rn->query) {
        ngx_resolver_free_locked(r, rn->query);
    }

    إذا (rn->name) {
        ngx_resolver_free_locked(r, rn->name);
    }

    إذا (rn->cnlen) {
        ngx_resolver_free_locked(r, rn->u.cname);
    }

    إذا (rn->naddrs > 1 && rn->naddrs != (u_short) -1) {
        ngx_resolver_free_locked(r, rn->u.addrs);
    }

#إذا (NGX_HAVE_INET6)
    إذا (rn->naddrs6 > 1 && rn->naddrs6 != (u_short) -1) {
        ngx_resolver_free_locked(r, rn->u6.addrs6);
    }
#جيڪڏهن_پورو

    إذا (rn->nsrvs) {
        لكل (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
            إذا (rn->u.srvs[i].name.معلومات) {
                ngx_resolver_free_locked(r, rn->u.srvs[i].name.معلومات);
            }
        }

        ngx_resolver_free_locked(r, rn->u.srvs);
    }

    ngx_resolver_free_locked(r, rn);

    /* unlock alloc mutex */
}


ساكن فراغ *
ngx_resolver_alloc(ngx_resolver_t *r, سائيز_t سائيز)
{
    u_char  *p;

    /* lock alloc mutex */

    p = ngx_alloc(سائيز, r->لاگرٿم);

    /* unlock alloc mutex */

    أرجع p;
}


ساكن فراغ *
ngx_resolver_calloc(ngx_resolver_t *r, سائيز_t سائيز)
{
    u_char  *p;

    p = ngx_resolver_alloc(r, سائيز);

    إذا (p) {
        ngx_memzero(p, سائيز);
    }

    أرجع p;
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
ngx_resolver_dup(ngx_resolver_t *r, فراغ *src, سائيز_t سائيز)
{
    فراغ  *dst;

    dst = ngx_resolver_alloc(r, سائيز);

    إذا (dst == عدم) {
        أرجع dst;
    }

    ngx_memcpy(dst, src, سائيز);

    أرجع dst;
}


ساكن ngx_resolver_addr_t *
ngx_resolver_export(ngx_resolver_t *r, ngx_resolver_ڳنڍ_t *rn,
    ngx_uint_t rotate)
{
    ngx_uint_t            d, i, j, n;
    اندريون_addr_t            *addr;
    ngx_sockaddr_t       *sockaddr;
    بنية sockaddr_اندريون   *جيب;
    ngx_resolver_addr_t  *dst;
#إذا (NGX_HAVE_INET6)
    بنية in6_addr      *addr6;
    بنية sockaddr_in6  *sin6;
#جيڪڏهن_پورو

    n = rn->naddrs;
#إذا (NGX_HAVE_INET6)
    n += rn->naddrs6;
#جيڪڏهن_پورو

    dst = ngx_resolver_calloc(r, n * حجم(ngx_resolver_addr_t));
    إذا (dst == عدم) {
        أرجع عدم;
    }

    sockaddr = ngx_resolver_calloc(r, n * حجم(ngx_sockaddr_t));
    إذا (sockaddr == عدم) {
        ngx_resolver_free(r, dst);
        أرجع عدم;
    }

    i = 0;
    d = rotate ? ngx_random() % n : 0;

    إذا (rn->naddrs) {
        j = rotate ? ngx_random() % rn->naddrs : 0;

        addr = (rn->naddrs == 1) ? &rn->u.addr : rn->u.addrs;

        افعل {
            جيب = &sockaddr[d].sockaddr_اندريون;
            جيب->sin_family = AF_INET;
            جيب->sin_addr.s_addr = addr[j++];
            dst[d].sockaddr = (بنية sockaddr *) جيب;
            dst[d++].socklen = حجم(بنية sockaddr_اندريون);

            إذا (d == n) {
                d = 0;
            }

            إذا (j == (ngx_uint_t) rn->naddrs) {
                j = 0;
            }
        } طالما (++i < (ngx_uint_t) rn->naddrs);
    }

#إذا (NGX_HAVE_INET6)
    إذا (rn->naddrs6) {
        j = rotate ? ngx_random() % rn->naddrs6 : 0;

        addr6 = (rn->naddrs6 == 1) ? &rn->u6.addr6 : rn->u6.addrs6;

        افعل {
            sin6 = &sockaddr[d].sockaddr_in6;
            sin6->sin6_family = AF_INET6;
            ngx_memcpy(sin6->sin6_addr.s6_addr, addr6[j++].s6_addr, 16);
            dst[d].sockaddr = (بنية sockaddr *) sin6;
            dst[d++].socklen = حجم(بنية sockaddr_in6);

            إذا (d == n) {
                d = 0;
            }

            إذا (j == rn->naddrs6) {
                j = 0;
            }
        } طالما (++i < n);
    }
#جيڪڏهن_پورو

    أرجع dst;
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

    لكل (i = 0; i < nsrvs; i++) {
        إذا (srvs[i].state == NGX_ERROR) {
            ctx->state = NGX_ERROR;
            ctx->valid = ngx_وقت() + (r->valid ? r->valid : 10);

            ctx->handler(ctx);
            أرجع;
        }

        naddrs += srvs[i].naddrs;
    }

    إذا (naddrs == 0) {
        ctx->state = srvs[0].state;

        لكل (i = 0; i < nsrvs; i++) {
            إذا (srvs[i].state == NGX_RESOLVE_NXDOMAIN) {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                اكسر;
            }
        }

        ctx->valid = ngx_وقت() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        أرجع;
    }

    addrs = ngx_resolver_calloc(r, naddrs * حجم(ngx_resolver_addr_t));
    إذا (addrs == عدم) {
        ctx->state = NGX_ERROR;
        ctx->valid = ngx_وقت() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        أرجع;
    }

    i = 0;
    n = 0;

    افعل {
        nw = 0;

        لكل (j = i; j < nsrvs; j++) {
            إذا (srvs[j].ترجيح != srvs[i].ترجيح) {
                اكسر;
            }

            nw += srvs[j].naddrs * srvs[j].وزن;
        }

        إذا (nw == 0) {
            اذهب ايندڙ_srv;
        }

        w = ngx_random() % nw;

        لكل (k = i; k < j; k++) {
            إذا (w < srvs[k].naddrs * srvs[k].وزن) {
                اكسر;
            }

            w -= srvs[k].naddrs * srvs[k].وزن;
        }

        لكل (l = i; l < j; l++) {

            لكل (m = 0; m < srvs[k].naddrs; m++) {
                addrs[n].socklen = srvs[k].addrs[m].socklen;
                addrs[n].sockaddr = srvs[k].addrs[m].sockaddr;
                addrs[n].name = srvs[k].name;
                addrs[n].ترجيح = srvs[k].ترجيح;
                addrs[n].وزن = srvs[k].وزن;
                n++;
            }

            إذا (++k == j) {
                k = i;
            }
        }

ايندڙ_srv:

        i = j;

    } طالما (i < ctx->nsrvs);

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

    إذا (err > 0 && err < 6) {
        أرجع errors[err - 1];
    }

    إذا (err == NGX_RESOLVE_TIMEDOUT) {
        أرجع "Operation timed out";
    }

    أرجع "Unknown error";
}


ساكن u_char *
ngx_resolver_log_error(ngx_log_t *لاگرٿم, u_char *buf, سائيز_t len)
{
    u_char                     *p;
    ngx_resolver_connection_t  *rec;

    p = buf;

    إذا (لاگرٿم->action) {
        p = ngx_snprintf(buf, len, " while %s", لاگرٿم->action);
        len -= p - buf;
    }

    rec = لاگرٿم->معلومات;

    إذا (rec) {
        p = ngx_snprintf(p, len, ", resolver: %V", &rec->server);
    }

    أرجع p;
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

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->لاگرٿم, 0, "UDP socket %d", s);

    إذا (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->لاگرٿم, ngx_socket_errno,
                      ngx_socket_n " failed");
        أرجع NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->لاگرٿم);

    إذا (c == عدم) {
        إذا (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->لاگرٿم, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        أرجع NGX_ERROR;
    }

    إذا (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->لاگرٿم, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        اذهب failed;
    }

    rev = c->اقرأ;
    wev = c->اكتب;

    rev->لاگرٿم = &rec->لاگرٿم;
    wev->لاگرٿم = &rec->لاگرٿم;

    rec->udp = c;

    c->عدد = ngx_atomic_fetch_جوڙيو(ngx_connection_counter, 1);

    c->شروعات_وقت = ngx_current_msec;

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->لاگرٿم, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->عدد);

    rc = connect(s, rec->sockaddr, rec->socklen);

    إذا (rc == -1) {
        ngx_log_error(NGX_LOG_CRIT, &rec->لاگرٿم, ngx_socket_errno,
                      "connect() failed");

        اذهب failed;
    }

    /* UDP sockets are always ready to write */
    wev->ready = 1;

    event = (ngx_event_flags & NGX_USE_CLEAR_EVENT) ?
                /* kqueue, epoll */                 NGX_CLEAR_EVENT:
                /* select, poll, /dev/poll */       NGX_LEVEL_EVENT;
                /* eventport event type has no meaning: oneshot only */

    إذا (ngx_جوڙيو_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        اذهب failed;
    }

    أرجع NGX_OK;

failed:

    ngx_close_connection(c);
    rec->udp = عدم;

    أرجع NGX_ERROR;
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

    s = ngx_socket(rec->sockaddr->sa_family, SOCK_وهڪرو, 0);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->لاگرٿم, 0, "TCP socket %d", s);

    إذا (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->لاگرٿم, ngx_socket_errno,
                      ngx_socket_n " failed");
        أرجع NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->لاگرٿم);

    إذا (c == عدم) {
        إذا (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->لاگرٿم, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        أرجع NGX_ERROR;
    }

    إذا (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->لاگرٿم, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        اذهب failed;
    }

    rev = c->اقرأ;
    wev = c->اكتب;

    rev->لاگرٿم = &rec->لاگرٿم;
    wev->لاگرٿم = &rec->لاگرٿم;

    rec->tcp = c;

    c->عدد = ngx_atomic_fetch_جوڙيو(ngx_connection_counter, 1);

    c->شروعات_وقت = ngx_current_msec;

    إذا (ngx_جوڙيو_conn) {
        إذا (ngx_جوڙيو_conn(c) == NGX_ERROR) {
            اذهب failed;
        }
    }

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->لاگرٿم, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->عدد);

    rc = connect(s, rec->sockaddr, rec->socklen);

    إذا (rc == -1) {
        err = ngx_socket_errno;


        إذا (err != NGX_EINPROGRESS
#إذا (NGX_WIN32)
            /* Winsock returns WSAEWOULDBLOCK (NGX_EAGAIN) */
            && err != NGX_EAGAIN
#جيڪڏهن_پورو
            )
        {
            إذا (err == NGX_ECONNREFUSED
#إذا (NGX_LINUX)
                /*
                 * Linux returns EAGAIN instead of ECONNREFUSED
                 * for unix sockets if listen queue is full
                 */
                || err == NGX_EAGAIN
#جيڪڏهن_پورو
                || err == NGX_ECONNRESET
                || err == NGX_ENETDOWN
                || err == NGX_ENETUNREACH
                || err == NGX_EHOSTDOWN
                || err == NGX_EHOSTUNREACH)
            {
                level = NGX_LOG_ERR;

            } وإلا {
                level = NGX_LOG_CRIT;
            }

            ngx_log_error(level, &rec->لاگرٿم, err, "connect() to %V failed",
                          &rec->server);

            ngx_close_connection(c);
            rec->tcp = عدم;

            أرجع NGX_ERROR;
        }
    }

    إذا (ngx_جوڙيو_conn) {
        إذا (rc == -1) {

            /* NGX_EINPROGRESS */

            أرجع NGX_AGAIN;
        }

        ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->لاگرٿم, 0, "connected");

        wev->ready = 1;

        أرجع NGX_OK;
    }

    إذا (ngx_event_flags & NGX_USE_CLEAR_EVENT) {

        /* kqueue */

        event = NGX_CLEAR_EVENT;

    } وإلا {

        /* select, poll, /dev/poll */

        event = NGX_LEVEL_EVENT;
    }

    إذا (ngx_جوڙيو_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        اذهب failed;
    }

    إذا (rc == -1) {

        /* NGX_EINPROGRESS */

        إذا (ngx_جوڙيو_event(wev, NGX_WRITE_EVENT, event) != NGX_OK) {
            اذهب failed;
        }

        أرجع NGX_AGAIN;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->لاگرٿم, 0, "connected");

    wev->ready = 1;

    أرجع NGX_OK;

failed:

    ngx_close_connection(c);
    rec->tcp = عدم;

    أرجع NGX_ERROR;
}


ساكن ngx_int_t
ngx_resolver_cmp_srvs(ثابت فراغ *one, ثابت فراغ *two)
{
    ngx_int_t            p1, p2;
    ngx_resolver_srv_t  *پهريون, *ٻيو;

    پهريون = (ngx_resolver_srv_t *) one;
    ٻيو = (ngx_resolver_srv_t *) two;

    p1 = پهريون->ترجيح;
    p2 = ٻيو->ترجيح;

    أرجع p1 - p2;
}
