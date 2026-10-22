
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>
#شامل <ngx_event.h>


#تعریف_کرو NGX_RESOLVER_UDP_سائز   4096

#تعریف_کرو NGX_RESOLVER_TCP_RSIZE  (2 + 65535)
#تعریف_کرو NGX_RESOLVER_TCP_WSIZE  8192


قسمتعریف ساخت {
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


قسمتعریف ساخت {
    u_char  type_hi;
    u_char  type_lo;
    u_char  class_hi;
    u_char  class_lo;
} ngx_resolver_qs_t;


قسمتعریف ساخت {
    u_char  type_hi;
    u_char  type_lo;
    u_char  class_hi;
    u_char  class_lo;
    u_char  ttl[4];
    u_char  len_hi;
    u_char  len_lo;
} ngx_resolver_an_t;


#تعریف_کرو ngx_resolver_گرہ(n)  ngx_rbtree_ڈیٹا(n, ngx_resolver_گرہ_t, گرہ)


ساکن ngx_int_t ngx_udp_connect(ngx_resolver_connection_t *rec);
ساکن ngx_int_t ngx_tcp_connect(ngx_resolver_connection_t *rec);


ساکن خالی ngx_resolver_cleanup(خالی *ڈیٹا);
ساکن خالی ngx_resolver_cleanup_درخت(ngx_resolver_t *r, ngx_rbtree_t *درخت);
ساکن ngx_int_t ngx_resolve_name_locked(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx, ngx_str_t *name);
ساکن خالی ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *درخت,
    ngx_قطار_t *قطار);
ساکن ngx_int_t ngx_resolver_send_query(ngx_resolver_t *r,
    ngx_resolver_گرہ_t *rn);
ساکن ngx_int_t ngx_resolver_send_udp_query(ngx_resolver_t *r,
    ngx_resolver_connection_t *rec, u_char *query, u_short qlen);
ساکن ngx_int_t ngx_resolver_send_tcp_query(ngx_resolver_t *r,
    ngx_resolver_connection_t *rec, u_char *query, u_short qlen);
ساکن ngx_int_t ngx_resolver_create_name_query(ngx_resolver_t *r,
    ngx_resolver_گرہ_t *rn, ngx_str_t *name);
ساکن ngx_int_t ngx_resolver_create_srv_query(ngx_resolver_t *r,
    ngx_resolver_گرہ_t *rn, ngx_str_t *name);
ساکن ngx_int_t ngx_resolver_create_addr_query(ngx_resolver_t *r,
    ngx_resolver_گرہ_t *rn, ngx_resolver_addr_t *addr);
ساکن خالی ngx_resolver_resend_handler(ngx_event_t *ev);
ساکن وقت_t ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *درخت,
    ngx_قطار_t *قطار);
ساکن ngx_uint_t ngx_resolver_resend_خالی_خانے(ngx_resolver_t *r);
ساکن خالی ngx_resolver_udp_read(ngx_event_t *rev);
ساکن خالی ngx_resolver_tcp_write(ngx_event_t *wev);
ساکن خالی ngx_resolver_tcp_read(ngx_event_t *rev);
ساکن خالی ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf,
    سائز_t n, ngx_uint_t tcp);
ساکن خالی ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, سائز_t n,
    ngx_uint_t ident, ngx_uint_t رمز, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans);
ساکن خالی ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, سائز_t n,
    ngx_uint_t ident, ngx_uint_t رمز, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans);
ساکن خالی ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, سائز_t n,
    ngx_uint_t ident, ngx_uint_t رمز, ngx_uint_t nan);
ساکن ngx_resolver_گرہ_t *ngx_resolver_lookup_name(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
ساکن ngx_resolver_گرہ_t *ngx_resolver_lookup_srv(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
ساکن ngx_resolver_گرہ_t *ngx_resolver_lookup_addr(ngx_resolver_t *r,
    اندر_addr_t addr);
ساکن خالی ngx_resolver_rbtree_داخل_کرو_قدر(ngx_rbtree_گرہ_t *عارضی,
    ngx_rbtree_گرہ_t *گرہ, ngx_rbtree_گرہ_t *sentinel);
ساکن ngx_int_t ngx_resolver_copy(ngx_resolver_t *r, ngx_str_t *name,
    u_char *buf, u_char *src, u_char *آخری);
ساکن ngx_int_t ngx_resolver_سیٹ_timeout(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx);
ساکن خالی ngx_resolver_timeout_handler(ngx_event_t *ev);
ساکن خالی ngx_resolver_free_گرہ(ngx_resolver_t *r, ngx_resolver_گرہ_t *rn);
ساکن خالی *ngx_resolver_alloc(ngx_resolver_t *r, سائز_t سائز);
ساکن خالی *ngx_resolver_calloc(ngx_resolver_t *r, سائز_t سائز);
ساکن خالی ngx_resolver_free(ngx_resolver_t *r, خالی *p);
ساکن خالی ngx_resolver_free_locked(ngx_resolver_t *r, خالی *p);
ساکن خالی *ngx_resolver_dup(ngx_resolver_t *r, خالی *src, سائز_t سائز);
ساکن ngx_resolver_addr_t *ngx_resolver_export(ngx_resolver_t *r,
    ngx_resolver_گرہ_t *rn, ngx_uint_t rotate);
ساکن خالی ngx_resolver_report_srv(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx);
ساکن u_char *ngx_resolver_log_error(ngx_log_t *لوگارتھم, u_char *buf, سائز_t len);
ساکن خالی ngx_resolver_resolve_srv_نام(ngx_resolver_ctx_t *ctx,
    ngx_resolver_گرہ_t *rn);
ساکن خالی ngx_resolver_srv_نام_handler(ngx_resolver_ctx_t *ctx);
ساکن ngx_int_t ngx_resolver_cmp_srvs(ثابت خالی *one, ثابت خالی *two);

#اگر (NGX_HAVE_INET6)
ساکن خالی ngx_resolver_rbtree_داخل_کرو_addr6_قدر(ngx_rbtree_گرہ_t *عارضی,
    ngx_rbtree_گرہ_t *گرہ, ngx_rbtree_گرہ_t *sentinel);
ساکن ngx_resolver_گرہ_t *ngx_resolver_lookup_addr6(ngx_resolver_t *r,
    ساخت in6_addr *addr, uint32_t hash);
#اگر_ختم


ngx_resolver_t *
ngx_resolver_create(ngx_conf_t *cf, ngx_str_t *نام, ngx_uint_t n)
{
    ngx_str_t                   s;
    ngx_url_t                   u;
    ngx_uint_t                  i, j;
    ngx_resolver_t             *r;
    ngx_pool_cleanup_t         *cln;
    ngx_resolver_connection_t  *rec;

    r = ngx_pcalloc(cf->pool, حجم(ngx_resolver_t));
    اگر (r == تہی) {
        واپس تہی;
    }

    r->event = ngx_pcalloc(cf->pool, حجم(ngx_event_t));
    اگر (r->event == تہی) {
        واپس تہی;
    }

    cln = ngx_pool_cleanup_جوڑو(cf->pool, 0);
    اگر (cln == تہی) {
        واپس تہی;
    }

    cln->handler = ngx_resolver_cleanup;
    cln->ڈیٹا = r;

    r->ipv4 = 1;

    ngx_rbtree_init(&r->name_rbtree, &r->name_sentinel,
                    ngx_resolver_rbtree_داخل_کرو_قدر);

    ngx_rbtree_init(&r->srv_rbtree, &r->srv_sentinel,
                    ngx_resolver_rbtree_داخل_کرو_قدر);

    ngx_rbtree_init(&r->addr_rbtree, &r->addr_sentinel,
                    ngx_rbtree_داخل_کرو_قدر);

    ngx_قطار_init(&r->name_resend_قطار);
    ngx_قطار_init(&r->srv_resend_قطار);
    ngx_قطار_init(&r->addr_resend_قطار);

    ngx_قطار_init(&r->name_expire_قطار);
    ngx_قطار_init(&r->srv_expire_قطار);
    ngx_قطار_init(&r->addr_expire_قطار);

#اگر (NGX_HAVE_INET6)
    r->ipv6 = 1;

    ngx_rbtree_init(&r->addr6_rbtree, &r->addr6_sentinel,
                    ngx_resolver_rbtree_داخل_کرو_addr6_قدر);

    ngx_قطار_init(&r->addr6_resend_قطار);

    ngx_قطار_init(&r->addr6_expire_قطار);
#اگر_ختم

    r->event->handler = ngx_resolver_resend_handler;
    r->event->ڈیٹا = r;
    r->event->لوگارتھم = &cf->چکر->new_log;
    r->event->cancelable = 1;
    r->ident = -1;

    r->resend_timeout = 5;
    r->tcp_timeout = 5;
    r->expire = 30;
    r->valid = 0;

    r->لوگارتھم = &cf->چکر->new_log;
    r->log_level = NGX_LOG_ERR;

    اگر (n) {
        اگر (ngx_array_init(&r->connections, cf->pool, n,
                           حجم(ngx_resolver_connection_t))
            != NGX_OK)
        {
            واپس تہی;
        }
    }

    برائے (i = 0; i < n; i++) {
        اگر (ngx_strncmp(نام[i].ڈیٹا, "valid=", 6) == 0) {
            s.len = نام[i].len - 6;
            s.ڈیٹا = نام[i].ڈیٹا + 6;

            r->valid = ngx_parse_وقت(&s, 1);

            اگر (r->valid == (وقت_t) NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &نام[i]);
                واپس تہی;
            }

            جاری;
        }

#اگر (NGX_HAVE_INET6)
        اگر (ngx_strncmp(نام[i].ڈیٹا, "ipv4=", 5) == 0) {

            اگر (ngx_strcmp(&نام[i].ڈیٹا[5], "on") == 0) {
                r->ipv4 = 1;

            } ورنہ اگر (ngx_strcmp(&نام[i].ڈیٹا[5], "off") == 0) {
                r->ipv4 = 0;

            } ورنہ {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &نام[i]);
                واپس تہی;
            }

            جاری;
        }

        اگر (ngx_strncmp(نام[i].ڈیٹا, "ipv6=", 5) == 0) {

            اگر (ngx_strcmp(&نام[i].ڈیٹا[5], "on") == 0) {
                r->ipv6 = 1;

            } ورنہ اگر (ngx_strcmp(&نام[i].ڈیٹا[5], "off") == 0) {
                r->ipv6 = 0;

            } ورنہ {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &نام[i]);
                واپس تہی;
            }

            جاری;
        }
#اگر_ختم

        ngx_memzero(&u, حجم(ngx_url_t));

        u.url = نام[i];
        u.default_پورٹ = 53;

        اگر (ngx_parse_url(cf->pool, &u) != NGX_OK) {
            اگر (u.err) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "%s in resolver \"%V\"",
                                   u.err, &u.url);
            }

            واپس تہی;
        }

        rec = ngx_array_ڈالو_n(&r->connections, u.naddrs);
        اگر (rec == تہی) {
            واپس تہی;
        }

        ngx_memzero(rec, u.naddrs * حجم(ngx_resolver_connection_t));

        برائے (j = 0; j < u.naddrs; j++) {
            rec[j].sockaddr = u.addrs[j].sockaddr;
            rec[j].socklen = u.addrs[j].socklen;
            rec[j].server = u.addrs[j].name;
            rec[j].resolver = r;
        }
    }

#اگر (NGX_HAVE_INET6)
    اگر (r->ipv4 + r->ipv6 == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"ipv4\" and \"ipv6\" cannot both be \"off\"");
        واپس تہی;
    }
#اگر_ختم

    اگر (n && r->connections.nelts == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "no name servers defined");
        واپس تہی;
    }

    واپس r;
}


ساکن خالی
ngx_resolver_cleanup(خالی *ڈیٹا)
{
    ngx_resolver_t  *r = ڈیٹا;

    ngx_uint_t                  i;
    ngx_resolver_connection_t  *rec;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_چکر->لوگارتھم, 0, "cleanup resolver");

    ngx_resolver_cleanup_درخت(r, &r->name_rbtree);

    ngx_resolver_cleanup_درخت(r, &r->srv_rbtree);

    ngx_resolver_cleanup_درخت(r, &r->addr_rbtree);

#اگر (NGX_HAVE_INET6)
    ngx_resolver_cleanup_درخت(r, &r->addr6_rbtree);
#اگر_ختم

    اگر (r->event->timer_سیٹ) {
        ngx_del_timer(r->event);
    }

    rec = r->connections.elts;

    برائے (i = 0; i < r->connections.nelts; i++) {
        اگر (rec[i].udp) {
            ngx_close_connection(rec[i].udp);
        }

        اگر (rec[i].tcp) {
            ngx_close_connection(rec[i].tcp);
        }

        اگر (rec[i].read_buf) {
            ngx_resolver_free(r, rec[i].read_buf->آغاز);
            ngx_resolver_free(r, rec[i].read_buf);
        }

        اگر (rec[i].write_buf) {
            ngx_resolver_free(r, rec[i].write_buf->آغاز);
            ngx_resolver_free(r, rec[i].write_buf);
        }
    }
}


ساکن خالی
ngx_resolver_cleanup_درخت(ngx_resolver_t *r, ngx_rbtree_t *درخت)
{
    ngx_resolver_ctx_t   *ctx, *اگلا;
    ngx_resolver_گرہ_t  *rn;

    جب (درخت->جڑ != درخت->sentinel) {

        rn = ngx_resolver_گرہ(ngx_rbtree_min(درخت->جڑ, درخت->sentinel));

        ngx_قطار_remove(&rn->قطار);

        برائے (ctx = rn->انتظار; ctx; ctx = اگلا) {
            اگلا = ctx->اگلا;

            اگر (ctx->event) {
                اگر (ctx->event->timer_سیٹ) {
                    ngx_del_timer(ctx->event);
                }

                ngx_resolver_free(r, ctx->event);
            }

            ngx_resolver_free(r, ctx);
        }

        ngx_rbtree_delete(درخت, &rn->گرہ);

        ngx_resolver_free_گرہ(r, rn);
    }
}


ngx_resolver_ctx_t *
ngx_resolve_آغاز(ngx_resolver_t *r, ngx_resolver_ctx_t *عارضی)
{
    اندر_addr_t            addr;
    ngx_resolver_ctx_t  *ctx;

    اگر (عارضی) {
        addr = ngx_inet_addr(عارضی->name.ڈیٹا, عارضی->name.len);

        اگر (addr != INADDR_NONE) {
            عارضی->resolver = r;
            عارضی->state = NGX_OK;
            عارضی->naddrs = 1;
            عارضی->addrs = &عارضی->addr;
            عارضی->addr.sockaddr = (ساخت sockaddr *) &عارضی->جیب;
            عارضی->addr.socklen = حجم(ساخت sockaddr_اندر);
            ngx_memzero(&عارضی->جیب, حجم(ساخت sockaddr_اندر));
            عارضی->جیب.sin_family = AF_INET;
            عارضی->جیب.sin_addr.s_addr = addr;
            عارضی->تیز = 1;

            واپس عارضی;
        }
    }

    اگر (r->connections.nelts == 0) {
        واپس NGX_NO_RESOLVER;
    }

    ctx = ngx_resolver_calloc(r, حجم(ngx_resolver_ctx_t));

    اگر (ctx) {
        ctx->resolver = r;
    }

    واپس ctx;
}


ngx_int_t
ngx_resolve_name(ngx_resolver_ctx_t *ctx)
{
    سائز_t           slen;
    ngx_int_t        rc;
    ngx_str_t        name;
    ngx_resolver_t  *r;

    r = ctx->resolver;

    اگر (ctx->name.len > 0 && ctx->name.ڈیٹا[ctx->name.len - 1] == '.') {
        ctx->name.len--;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0,
                   "resolve: \"%V\"", &ctx->name);

    اگر (ctx->تیز) {
        ctx->handler(ctx);
        واپس NGX_OK;
    }

    اگر (ctx->service.len) {
        slen = ctx->service.len;

        اگر (ngx_strlchr(ctx->service.ڈیٹا,
                        ctx->service.ڈیٹا + ctx->service.len, '.')
            == تہی)
        {
            slen += حجم("_._tcp") - 1;
        }

        name.len = slen + 1 + ctx->name.len;

        name.ڈیٹا = ngx_resolver_alloc(r, name.len);
        اگر (name.ڈیٹا == تہی) {
            جاؤ failed;
        }

        اگر (slen == ctx->service.len) {
            ngx_sprintf(name.ڈیٹا, "%V.%V", &ctx->service, &ctx->name);

        } ورنہ {
            ngx_sprintf(name.ڈیٹا, "_%V._tcp.%V", &ctx->service, &ctx->name);
        }

        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &name);

        ngx_resolver_free(r, name.ڈیٹا);

    } ورنہ {
        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &ctx->name);
    }

    اگر (rc == NGX_OK) {
        واپس NGX_OK;
    }

    /* unlock name mutex */

    اگر (rc == NGX_AGAIN) {
        واپس NGX_OK;
    }

    /* NGX_ERROR */

    اگر (ctx->event) {
        ngx_resolver_free(r, ctx->event);
    }

failed:

    ngx_resolver_free(r, ctx);

    واپس NGX_ERROR;
}


خالی
ngx_resolve_name_مکمل(ngx_resolver_ctx_t *ctx)
{
    ngx_uint_t            i;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_گرہ_t  *rn;

    r = ctx->resolver;

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0,
                   "resolve name done: %i", ctx->state);

    اگر (ctx->تیز) {
        واپس;
    }

    اگر (ctx->event && ctx->event->timer_سیٹ) {
        ngx_del_timer(ctx->event);
    }

    /* lock name mutex */

    اگر (ctx->nsrvs) {
        برائے (i = 0; i < ctx->nsrvs; i++) {
            اگر (ctx->srvs[i].ctx) {
                ngx_resolve_name_مکمل(ctx->srvs[i].ctx);
            }

            اگر (ctx->srvs[i].addrs) {
                ngx_resolver_free(r, ctx->srvs[i].addrs->sockaddr);
                ngx_resolver_free(r, ctx->srvs[i].addrs);
            }

            ngx_resolver_free(r, ctx->srvs[i].name.ڈیٹا);
        }

        ngx_resolver_free(r, ctx->srvs);
    }

    اگر (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->گرہ;

        اگر (rn) {
            p = &rn->انتظار;
            w = rn->انتظار;

            جب (w) {
                اگر (w == ctx) {
                    *p = w->اگلا;

                    جاؤ مکمل;
                }

                p = &w->اگلا;
                w = w->اگلا;
            }

            ngx_log_error(NGX_LOG_ALERT, r->لوگارتھم, 0,
                          "could not cancel %V resolving", &ctx->name);
        }
    }

مکمل:

    اگر (ctx->service.len) {
        ngx_resolver_expire(r, &r->srv_rbtree, &r->srv_expire_قطار);

    } ورنہ {
        ngx_resolver_expire(r, &r->name_rbtree, &r->name_expire_قطار);
    }

    /* unlock name mutex */

    /* lock alloc mutex */

    اگر (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    اگر (r->event->timer_سیٹ && ngx_resolver_resend_خالی_خانے(r)) {
        ngx_del_timer(r->event);
    }
}


ساکن ngx_int_t
ngx_resolve_name_locked(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx,
    ngx_str_t *name)
{
    uint32_t              hash;
    ngx_int_t             rc;
    ngx_str_t             cname;
    ngx_uint_t            i, naddrs;
    ngx_قطار_t          *resend_قطار, *expire_قطار;
    ngx_rbtree_t         *درخت;
    ngx_resolver_ctx_t   *اگلا, *آخری;
    ngx_resolver_addr_t  *addrs;
    ngx_resolver_گرہ_t  *rn;

    ngx_strlow(name->ڈیٹا, name->ڈیٹا, name->len);

    hash = ngx_چکری_جانچ_32_short(name->ڈیٹا, name->len);

    اگر (ctx->service.len) {
        rn = ngx_resolver_lookup_srv(r, name, hash);

        درخت = &r->srv_rbtree;
        resend_قطار = &r->srv_resend_قطار;
        expire_قطار = &r->srv_expire_قطار;

    } ورنہ {
        rn = ngx_resolver_lookup_name(r, name, hash);

        درخت = &r->name_rbtree;
        resend_قطار = &r->name_resend_قطار;
        expire_قطار = &r->name_expire_قطار;
    }

    اگر (rn) {

        /* ctx can be a list after NGX_RESOLVE_CNAME */
        برائے (آخری = ctx; آخری->اگلا; آخری = آخری->اگلا);

        اگر (rn->valid >= ngx_وقت()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0, "resolve cached");

            ngx_قطار_remove(&rn->قطار);

            rn->expire = ngx_وقت() + r->expire;

            ngx_قطار_داخل_کرو_سر(expire_قطار, &rn->قطار);

            naddrs = (rn->naddrs == (u_short) -1) ? 0 : rn->naddrs;
#اگر (NGX_HAVE_INET6)
            naddrs += (rn->naddrs6 == (u_short) -1) ? 0 : rn->naddrs6;
#اگر_ختم

            اگر (naddrs) {

                اگر (naddrs == 1 && rn->naddrs == 1) {
                    addrs = تہی;

                } ورنہ {
                    addrs = ngx_resolver_export(r, rn, 1);
                    اگر (addrs == تہی) {
                        واپس NGX_ERROR;
                    }
                }

                آخری->اگلا = rn->انتظار;
                rn->انتظار = تہی;

                /* unlock name mutex */

                کرو {
                    ctx->state = NGX_OK;
                    ctx->valid = rn->valid;
                    ctx->naddrs = naddrs;

                    اگر (addrs == تہی) {
                        ctx->addrs = &ctx->addr;
                        ctx->addr.sockaddr = (ساخت sockaddr *) &ctx->جیب;
                        ctx->addr.socklen = حجم(ساخت sockaddr_اندر);
                        ngx_memzero(&ctx->جیب, حجم(ساخت sockaddr_اندر));
                        ctx->جیب.sin_family = AF_INET;
                        ctx->جیب.sin_addr.s_addr = rn->u.addr;

                    } ورنہ {
                        ctx->addrs = addrs;
                    }

                    اگلا = ctx->اگلا;

                    ctx->handler(ctx);

                    ctx = اگلا;
                } جب (ctx);

                اگر (addrs != تہی) {
                    ngx_resolver_free(r, addrs->sockaddr);
                    ngx_resolver_free(r, addrs);
                }

                واپس NGX_OK;
            }

            اگر (rn->nsrvs) {
                آخری->اگلا = rn->انتظار;
                rn->انتظار = تہی;

                /* unlock name mutex */

                کرو {
                    اگلا = ctx->اگلا;

                    ngx_resolver_resolve_srv_نام(ctx, rn);

                    ctx = اگلا;
                } جب (ctx);

                واپس NGX_OK;
            }

            /* NGX_RESOLVE_CNAME */

            اگر (ctx->recursion++ < NGX_RESOLVER_MAX_RECURSION) {

                cname.len = rn->cnlen;
                cname.ڈیٹا = rn->u.cname;

                واپس ngx_resolve_name_locked(r, ctx, &cname);
            }

            آخری->اگلا = rn->انتظار;
            rn->انتظار = تہی;

            /* unlock name mutex */

            کرو {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                ctx->valid = ngx_وقت() + (r->valid ? r->valid : 10);
                اگلا = ctx->اگلا;

                ctx->handler(ctx);

                ctx = اگلا;
            } جب (ctx);

            واپس NGX_OK;
        }

        اگر (rn->انتظار) {
            اگر (ngx_resolver_سیٹ_timeout(r, ctx) != NGX_OK) {
                واپس NGX_ERROR;
            }

            آخری->اگلا = rn->انتظار;
            rn->انتظار = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;

            کرو {
                ctx->گرہ = rn;
                ctx = ctx->اگلا;
            } جب (ctx);

            واپس NGX_AGAIN;
        }

        ngx_قطار_remove(&rn->قطار);

        /* lock alloc mutex */

        اگر (rn->query) {
            ngx_resolver_free_locked(r, rn->query);
            rn->query = تہی;
#اگر (NGX_HAVE_INET6)
            rn->query6 = تہی;
#اگر_ختم
        }

        اگر (rn->cnlen) {
            ngx_resolver_free_locked(r, rn->u.cname);
        }

        اگر (rn->naddrs > 1 && rn->naddrs != (u_short) -1) {
            ngx_resolver_free_locked(r, rn->u.addrs);
        }

#اگر (NGX_HAVE_INET6)
        اگر (rn->naddrs6 > 1 && rn->naddrs6 != (u_short) -1) {
            ngx_resolver_free_locked(r, rn->u6.addrs6);
        }
#اگر_ختم

        اگر (rn->nsrvs) {
            برائے (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
                اگر (rn->u.srvs[i].name.ڈیٹا) {
                    ngx_resolver_free_locked(r, rn->u.srvs[i].name.ڈیٹا);
                }
            }

            ngx_resolver_free_locked(r, rn->u.srvs);
        }

        /* unlock alloc mutex */

    } ورنہ {

        rn = ngx_resolver_alloc(r, حجم(ngx_resolver_گرہ_t));
        اگر (rn == تہی) {
            واپس NGX_ERROR;
        }

        rn->name = ngx_resolver_dup(r, name->ڈیٹا, name->len);
        اگر (rn->name == تہی) {
            ngx_resolver_free(r, rn);
            واپس NGX_ERROR;
        }

        rn->گرہ.کلید = hash;
        rn->nlen = (u_short) name->len;
        rn->query = تہی;
#اگر (NGX_HAVE_INET6)
        rn->query6 = تہی;
#اگر_ختم

        ngx_rbtree_داخل_کرو(درخت, &rn->گرہ);
    }

    اگر (ctx->service.len) {
        rc = ngx_resolver_create_srv_query(r, rn, name);

    } ورنہ {
        rc = ngx_resolver_create_name_query(r, rn, name);
    }

    اگر (rc == NGX_ERROR) {
        جاؤ failed;
    }

    اگر (rc == NGX_DECLINED) {
        ngx_rbtree_delete(درخت, &rn->گرہ);

        ngx_resolver_free(r, rn->query);
        ngx_resolver_free(r, rn->name);
        ngx_resolver_free(r, rn);

        کرو {
            ctx->state = NGX_RESOLVE_NXDOMAIN;
            اگلا = ctx->اگلا;

            ctx->handler(ctx);

            ctx = اگلا;
        } جب (ctx);

        واپس NGX_OK;
    }

    rn->آخری_connection = r->آخری_connection++;
    اگر (r->آخری_connection == r->connections.nelts) {
        r->آخری_connection = 0;
    }

    rn->naddrs = r->ipv4 ? (u_short) -1 : 0;
    rn->tcp = 0;
#اگر (NGX_HAVE_INET6)
    rn->naddrs6 = r->ipv6 ? (u_short) -1 : 0;
    rn->tcp6 = 0;
#اگر_ختم
    rn->nsrvs = 0;

    اگر (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->آخری_connection++;
        اگر (rn->آخری_connection == r->connections.nelts) {
            rn->آخری_connection = 0;
        }

        (خالی) ngx_resolver_send_query(r, rn);
    }

    اگر (ngx_resolver_سیٹ_timeout(r, ctx) != NGX_OK) {
        جاؤ failed;
    }

    اگر (ngx_resolver_resend_خالی_خانے(r)) {
        ngx_جوڑو_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_وقت() + r->resend_timeout;

    ngx_قطار_داخل_کرو_سر(resend_قطار, &rn->قطار);

    rn->رمز = 0;
    rn->cnlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_قدر;
    rn->انتظار = ctx;

    ctx->state = NGX_AGAIN;
    ctx->async = 1;

    کرو {
        ctx->گرہ = rn;
        ctx = ctx->اگلا;
    } جب (ctx);

    واپس NGX_AGAIN;

failed:

    ngx_rbtree_delete(درخت, &rn->گرہ);

    اگر (rn->query) {
        ngx_resolver_free(r, rn->query);
    }

    ngx_resolver_free(r, rn->name);

    ngx_resolver_free(r, rn);

    واپس NGX_ERROR;
}


ngx_int_t
ngx_resolve_addr(ngx_resolver_ctx_t *ctx)
{
    u_char               *name;
    اندر_addr_t             addr;
    ngx_قطار_t          *resend_قطار, *expire_قطار;
    ngx_rbtree_t         *درخت;
    ngx_resolver_t       *r;
    ساخت sockaddr_اندر   *جیب;
    ngx_resolver_گرہ_t  *rn;
#اگر (NGX_HAVE_INET6)
    uint32_t              hash;
    ساخت sockaddr_in6  *sin6;
#اگر_ختم

#اگر (NGX_SUPPRESS_WARN)
    addr = 0;
#اگر (NGX_HAVE_INET6)
    hash = 0;
    sin6 = تہی;
#اگر_ختم
#اگر_ختم

    r = ctx->resolver;

    انتخاب (ctx->addr.sockaddr->sa_family) {

#اگر (NGX_HAVE_INET6)
    حالت AF_INET6:
        sin6 = (ساخت sockaddr_in6 *) ctx->addr.sockaddr;
        hash = ngx_چکری_جانچ_32_short(sin6->sin6_addr.s6_addr, 16);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr6(r, &sin6->sin6_addr, hash);

        درخت = &r->addr6_rbtree;
        resend_قطار = &r->addr6_resend_قطار;
        expire_قطار = &r->addr6_expire_قطار;

        توڑو;
#اگر_ختم

    طےشدہ: /* AF_INET */
        جیب = (ساخت sockaddr_اندر *) ctx->addr.sockaddr;
        addr = ntohl(جیب->sin_addr.s_addr);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        درخت = &r->addr_rbtree;
        resend_قطار = &r->addr_resend_قطار;
        expire_قطار = &r->addr_expire_قطار;
    }

    اگر (rn) {

        اگر (rn->valid >= ngx_وقت()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0, "resolve cached");

            ngx_قطار_remove(&rn->قطار);

            rn->expire = ngx_وقت() + r->expire;

            ngx_قطار_داخل_کرو_سر(expire_قطار, &rn->قطار);

            name = ngx_resolver_dup(r, rn->name, rn->nlen);
            اگر (name == تہی) {
                ngx_resolver_free(r, ctx);
                واپس NGX_ERROR;
            }

            ctx->name.len = rn->nlen;
            ctx->name.ڈیٹا = name;

            /* unlock addr mutex */

            ctx->state = NGX_OK;
            ctx->valid = rn->valid;

            ctx->handler(ctx);

            ngx_resolver_free(r, name);

            واپس NGX_OK;
        }

        اگر (rn->انتظار) {
            اگر (ngx_resolver_سیٹ_timeout(r, ctx) != NGX_OK) {
                واپس NGX_ERROR;
            }

            ctx->اگلا = rn->انتظار;
            rn->انتظار = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;
            ctx->گرہ = rn;

            /* unlock addr mutex */

            واپس NGX_OK;
        }

        ngx_قطار_remove(&rn->قطار);

        ngx_resolver_free(r, rn->query);
        rn->query = تہی;
#اگر (NGX_HAVE_INET6)
        rn->query6 = تہی;
#اگر_ختم

    } ورنہ {
        rn = ngx_resolver_alloc(r, حجم(ngx_resolver_گرہ_t));
        اگر (rn == تہی) {
            جاؤ failed;
        }

        انتخاب (ctx->addr.sockaddr->sa_family) {

#اگر (NGX_HAVE_INET6)
        حالت AF_INET6:
            rn->addr6 = sin6->sin6_addr;
            rn->گرہ.کلید = hash;
            توڑو;
#اگر_ختم

        طےشدہ: /* AF_INET */
            rn->گرہ.کلید = addr;
        }

        rn->query = تہی;
#اگر (NGX_HAVE_INET6)
        rn->query6 = تہی;
#اگر_ختم

        ngx_rbtree_داخل_کرو(درخت, &rn->گرہ);
    }

    اگر (ngx_resolver_create_addr_query(r, rn, &ctx->addr) != NGX_OK) {
        جاؤ failed;
    }

    rn->آخری_connection = r->آخری_connection++;
    اگر (r->آخری_connection == r->connections.nelts) {
        r->آخری_connection = 0;
    }

    rn->naddrs = (u_short) -1;
    rn->tcp = 0;
#اگر (NGX_HAVE_INET6)
    rn->naddrs6 = (u_short) -1;
    rn->tcp6 = 0;
#اگر_ختم
    rn->nsrvs = 0;

    اگر (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->آخری_connection++;
        اگر (rn->آخری_connection == r->connections.nelts) {
            rn->آخری_connection = 0;
        }

        (خالی) ngx_resolver_send_query(r, rn);
    }

    اگر (ngx_resolver_سیٹ_timeout(r, ctx) != NGX_OK) {
        جاؤ failed;
    }

    اگر (ngx_resolver_resend_خالی_خانے(r)) {
        ngx_جوڑو_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_وقت() + r->resend_timeout;

    ngx_قطار_داخل_کرو_سر(resend_قطار, &rn->قطار);

    rn->رمز = 0;
    rn->cnlen = 0;
    rn->name = تہی;
    rn->nlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_قدر;
    rn->انتظار = ctx;

    /* unlock addr mutex */

    ctx->state = NGX_AGAIN;
    ctx->async = 1;
    ctx->گرہ = rn;

    واپس NGX_OK;

failed:

    اگر (rn) {
        ngx_rbtree_delete(درخت, &rn->گرہ);

        اگر (rn->query) {
            ngx_resolver_free(r, rn->query);
        }

        ngx_resolver_free(r, rn);
    }

    /* unlock addr mutex */

    اگر (ctx->event) {
        ngx_resolver_free(r, ctx->event);
    }

    ngx_resolver_free(r, ctx);

    واپس NGX_ERROR;
}


خالی
ngx_resolve_addr_مکمل(ngx_resolver_ctx_t *ctx)
{
    ngx_قطار_t          *expire_قطار;
    ngx_rbtree_t         *درخت;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_گرہ_t  *rn;

    r = ctx->resolver;

    انتخاب (ctx->addr.sockaddr->sa_family) {

#اگر (NGX_HAVE_INET6)
    حالت AF_INET6:
        درخت = &r->addr6_rbtree;
        expire_قطار = &r->addr6_expire_قطار;
        توڑو;
#اگر_ختم

    طےشدہ: /* AF_INET */
        درخت = &r->addr_rbtree;
        expire_قطار = &r->addr_expire_قطار;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0,
                   "resolve addr done: %i", ctx->state);

    اگر (ctx->event && ctx->event->timer_سیٹ) {
        ngx_del_timer(ctx->event);
    }

    /* lock addr mutex */

    اگر (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->گرہ;

        اگر (rn) {
            p = &rn->انتظار;
            w = rn->انتظار;

            جب (w) {
                اگر (w == ctx) {
                    *p = w->اگلا;

                    جاؤ مکمل;
                }

                p = &w->اگلا;
                w = w->اگلا;
            }
        }

        {
            u_char     متن[NGX_SOCKADDR_STRLEN];
            ngx_str_t  addrtext;

            addrtext.ڈیٹا = متن;
            addrtext.len = ngx_sock_ntop(ctx->addr.sockaddr, ctx->addr.socklen,
                                         متن, NGX_SOCKADDR_STRLEN, 0);

            ngx_log_error(NGX_LOG_ALERT, r->لوگارتھم, 0,
                          "could not cancel %V resolving", &addrtext);
        }
    }

مکمل:

    ngx_resolver_expire(r, درخت, expire_قطار);

    /* unlock addr mutex */

    /* lock alloc mutex */

    اگر (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    اگر (r->event->timer_سیٹ && ngx_resolver_resend_خالی_خانے(r)) {
        ngx_del_timer(r->event);
    }
}


ساکن خالی
ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *درخت, ngx_قطار_t *قطار)
{
    وقت_t                now;
    ngx_uint_t            i;
    ngx_قطار_t          *q;
    ngx_resolver_گرہ_t  *rn;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0, "resolver expire");

    now = ngx_وقت();

    برائے (i = 0; i < 2; i++) {
        اگر (ngx_قطار_خالی_خانے(قطار)) {
            واپس;
        }

        q = ngx_قطار_آخری(قطار);

        rn = ngx_قطار_ڈیٹا(q, ngx_resolver_گرہ_t, قطار);

        اگر (now <= rn->expire) {
            واپس;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0,
                       "resolver expire \"%*s\"", (سائز_t) rn->nlen, rn->name);

        ngx_قطار_remove(q);

        ngx_rbtree_delete(درخت, &rn->گرہ);

        ngx_resolver_free_گرہ(r, rn);
    }
}


ساکن ngx_int_t
ngx_resolver_send_query(ngx_resolver_t *r, ngx_resolver_گرہ_t *rn)
{
    ngx_int_t                   rc;
    ngx_resolver_connection_t  *rec;

    rec = r->connections.elts;
    rec = &rec[rn->آخری_connection];

    اگر (rec->لوگارتھم.handler == تہی) {
        rec->لوگارتھم = *r->لوگارتھم;
        rec->لوگارتھم.handler = ngx_resolver_log_error;
        rec->لوگارتھم.ڈیٹا = rec;
        rec->لوگارتھم.action = "resolving";
    }

    اگر (rn->query && rn->naddrs == (u_short) -1) {
        rc = rn->tcp ? ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen)
                     : ngx_resolver_send_udp_query(r, rec, rn->query, rn->qlen);

        اگر (rc != NGX_OK) {
            واپس rc;
        }
    }

#اگر (NGX_HAVE_INET6)

    اگر (rn->query6 && rn->naddrs6 == (u_short) -1) {
        rc = rn->tcp6
                    ? ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen)
                    : ngx_resolver_send_udp_query(r, rec, rn->query6, rn->qlen);

        اگر (rc != NGX_OK) {
            واپس rc;
        }
    }

#اگر_ختم

    واپس NGX_OK;
}


ساکن ngx_int_t
ngx_resolver_send_udp_query(ngx_resolver_t *r, ngx_resolver_connection_t  *rec,
    u_char *query, u_short qlen)
{
    ssize_t  n;

    اگر (rec->udp == تہی) {
        اگر (ngx_udp_connect(rec) != NGX_OK) {
            واپس NGX_ERROR;
        }

        rec->udp->ڈیٹا = rec;
        rec->udp->اقرأ->handler = ngx_resolver_udp_read;
        rec->udp->اقرأ->resolver = 1;
    }

    n = ngx_send(rec->udp, query, qlen);

    اگر (n == NGX_ERROR) {
        جاؤ failed;
    }

    اگر ((سائز_t) n != (سائز_t) qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->لوگارتھم, 0, "send() incomplete");
        جاؤ failed;
    }

    واپس NGX_OK;

failed:

    ngx_close_connection(rec->udp);
    rec->udp = تہی;

    واپس NGX_ERROR;
}


ساکن ngx_int_t
ngx_resolver_send_tcp_query(ngx_resolver_t *r, ngx_resolver_connection_t *rec,
    u_char *query, u_short qlen)
{
    ngx_buf_t  *b;
    ngx_int_t   rc;

    rc = NGX_OK;

    اگر (rec->tcp == تہی) {
        b = rec->read_buf;

        اگر (b == تہی) {
            b = ngx_resolver_calloc(r, حجم(ngx_buf_t));
            اگر (b == تہی) {
                واپس NGX_ERROR;
            }

            b->آغاز = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_RSIZE);
            اگر (b->آغاز == تہی) {
                ngx_resolver_free(r, b);
                واپس NGX_ERROR;
            }

            b->انتہا = b->آغاز + NGX_RESOLVER_TCP_RSIZE;

            rec->read_buf = b;
        }

        b->pos = b->آغاز;
        b->آخری = b->آغاز;

        b = rec->write_buf;

        اگر (b == تہی) {
            b = ngx_resolver_calloc(r, حجم(ngx_buf_t));
            اگر (b == تہی) {
                واپس NGX_ERROR;
            }

            b->آغاز = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_WSIZE);
            اگر (b->آغاز == تہی) {
                ngx_resolver_free(r, b);
                واپس NGX_ERROR;
            }

            b->انتہا = b->آغاز + NGX_RESOLVER_TCP_WSIZE;

            rec->write_buf = b;
        }

        b->pos = b->آغاز;
        b->آخری = b->آغاز;

        rc = ngx_tcp_connect(rec);
        اگر (rc == NGX_ERROR) {
            واپس NGX_ERROR;
        }

        rec->tcp->ڈیٹا = rec;
        rec->tcp->اكتب->handler = ngx_resolver_tcp_write;
        rec->tcp->اكتب->cancelable = 1;
        rec->tcp->اقرأ->handler = ngx_resolver_tcp_read;
        rec->tcp->اقرأ->resolver = 1;

        ngx_جوڑو_timer(rec->tcp->اكتب, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    b = rec->write_buf;

    اگر (b->انتہا - b->آخری <  2 + qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->لوگارتھم, 0, "buffer overflow");
        واپس NGX_ERROR;
    }

    *b->آخری++ = (u_char) (qlen >> 8);
    *b->آخری++ = (u_char) qlen;
    b->آخری = ngx_cpymem(b->آخری, query, qlen);

    اگر (rc == NGX_OK) {
        ngx_resolver_tcp_write(rec->tcp->اكتب);
    }

    واپس NGX_OK;
}


ساکن خالی
ngx_resolver_resend_handler(ngx_event_t *ev)
{
    وقت_t           timer, atimer, stimer, ntimer;
#اگر (NGX_HAVE_INET6)
    وقت_t           a6timer;
#اگر_ختم
    ngx_resolver_t  *r;

    r = ev->ڈیٹا;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0,
                   "resolver resend handler");

    /* lock name mutex */

    ntimer = ngx_resolver_resend(r, &r->name_rbtree, &r->name_resend_قطار);

    stimer = ngx_resolver_resend(r, &r->srv_rbtree, &r->srv_resend_قطار);

    /* unlock name mutex */

    /* lock addr mutex */

    atimer = ngx_resolver_resend(r, &r->addr_rbtree, &r->addr_resend_قطار);

    /* unlock addr mutex */

#اگر (NGX_HAVE_INET6)

    /* lock addr6 mutex */

    a6timer = ngx_resolver_resend(r, &r->addr6_rbtree, &r->addr6_resend_قطار);

    /* unlock addr6 mutex */

#اگر_ختم

    timer = ntimer;

    اگر (timer == 0) {
        timer = atimer;

    } ورنہ اگر (atimer) {
        timer = ngx_min(timer, atimer);
    }

    اگر (timer == 0) {
        timer = stimer;

    } ورنہ اگر (stimer) {
        timer = ngx_min(timer, stimer);
    }

#اگر (NGX_HAVE_INET6)

    اگر (timer == 0) {
        timer = a6timer;

    } ورنہ اگر (a6timer) {
        timer = ngx_min(timer, a6timer);
    }

#اگر_ختم

    اگر (timer) {
        ngx_جوڑو_timer(r->event, (ngx_msec_t) (timer * 1000));
    }
}


ساکن وقت_t
ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *درخت, ngx_قطار_t *قطار)
{
    وقت_t                now;
    ngx_قطار_t          *q;
    ngx_resolver_گرہ_t  *rn;

    now = ngx_وقت();

    برائے ( ;; ) {
        اگر (ngx_قطار_خالی_خانے(قطار)) {
            واپس 0;
        }

        q = ngx_قطار_آخری(قطار);

        rn = ngx_قطار_ڈیٹا(q, ngx_resolver_گرہ_t, قطار);

        اگر (now < rn->expire) {
            واپس rn->expire - now;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0,
                       "resolver resend \"%*s\" %p",
                       (سائز_t) rn->nlen, rn->name, rn->انتظار);

        ngx_قطار_remove(q);

        اگر (rn->انتظار) {

            اگر (++rn->آخری_connection == r->connections.nelts) {
                rn->آخری_connection = 0;
            }

            (خالی) ngx_resolver_send_query(r, rn);

            rn->expire = now + r->resend_timeout;

            ngx_قطار_داخل_کرو_سر(قطار, q);

            جاری;
        }

        ngx_rbtree_delete(درخت, &rn->گرہ);

        ngx_resolver_free_گرہ(r, rn);
    }
}


ساکن ngx_uint_t
ngx_resolver_resend_خالی_خانے(ngx_resolver_t *r)
{
    واپس ngx_قطار_خالی_خانے(&r->name_resend_قطار)
           && ngx_قطار_خالی_خانے(&r->srv_resend_قطار)
#اگر (NGX_HAVE_INET6)
           && ngx_قطار_خالی_خانے(&r->addr6_resend_قطار)
#اگر_ختم
           && ngx_قطار_خالی_خانے(&r->addr_resend_قطار);
}


ساکن خالی
ngx_resolver_udp_read(ngx_event_t *rev)
{
    ssize_t                     n;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;
    u_char                      buf[NGX_RESOLVER_UDP_سائز];

    c = rev->ڈیٹا;
    rec = c->ڈیٹا;

    کرو {
        n = ngx_udp_recv(c, buf, NGX_RESOLVER_UDP_سائز);

        اگر (n == NGX_AGAIN) {
            توڑو;
        }

        اگر (n == NGX_ERROR) {
            جاؤ failed;
        }

        ngx_resolver_process_response(rec->resolver, buf, n, 0);

    } جب (rev->ready);

    اگر (ngx_handle_read_event(rev, 0) != NGX_OK) {
        جاؤ failed;
    }

    واپس;

failed:

    ngx_close_connection(rec->udp);
    rec->udp = تہی;
}


ساکن خالی
ngx_resolver_tcp_write(ngx_event_t *wev)
{
    off_t                       بھیجا;
    ssize_t                     n;
    ngx_buf_t                  *b;
    ngx_resolver_t             *r;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;

    c = wev->ڈیٹا;
    rec = c->ڈیٹا;
    b = rec->write_buf;
    r = rec->resolver;

    اگر (wev->timedout) {
        جاؤ failed;
    }

    بھیجا = c->بھیجا;

    جب (wev->ready && b->pos < b->آخری) {
        n = ngx_send(c, b->pos, b->آخری - b->pos);

        اگر (n == NGX_AGAIN) {
            توڑو;
        }

        اگر (n == NGX_ERROR) {
            جاؤ failed;
        }

        b->pos += n;
    }

    اگر (b->pos != b->آغاز) {
        b->آخری = ngx_movemem(b->آغاز, b->pos, b->آخری - b->pos);
        b->pos = b->آغاز;
    }

    اگر (c->بھیجا != بھیجا) {
        ngx_جوڑو_timer(wev, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    اگر (ngx_handle_write_event(wev, 0) != NGX_OK) {
        جاؤ failed;
    }

    واپس;

failed:

    ngx_close_connection(c);
    rec->tcp = تہی;
}


ساکن خالی
ngx_resolver_tcp_read(ngx_event_t *rev)
{
    u_char                     *p;
    سائز_t                      سائز;
    ssize_t                     n;
    u_short                     qlen;
    ngx_buf_t                  *b;
    ngx_resolver_t             *r;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;

    c = rev->ڈیٹا;
    rec = c->ڈیٹا;
    b = rec->read_buf;
    r = rec->resolver;

    جب (rev->ready) {
        n = ngx_recv(c, b->آخری, b->انتہا - b->آخری);

        اگر (n == NGX_AGAIN) {
            توڑو;
        }

        اگر (n == NGX_ERROR || n == 0) {
            جاؤ failed;
        }

        b->آخری += n;

        برائے ( ;; ) {
            p = b->pos;
            سائز = b->آخری - p;

            اگر (سائز < 2) {
                توڑو;
            }

            qlen = (u_short) *p++ << 8;
            qlen += *p++;

            اگر (سائز < (سائز_t) (2 + qlen)) {
                توڑو;
            }

            ngx_resolver_process_response(r, p, qlen, 1);

            b->pos += 2 + qlen;
        }

        اگر (b->pos != b->آغاز) {
            b->آخری = ngx_movemem(b->آغاز, b->pos, b->آخری - b->pos);
            b->pos = b->آغاز;
        }
    }

    اگر (ngx_handle_read_event(rev, 0) != NGX_OK) {
        جاؤ failed;
    }

    واپس;

failed:

    ngx_close_connection(c);
    rec->tcp = تہی;
}


ساکن خالی
ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf, سائز_t n,
    ngx_uint_t tcp)
{
    حرف                 *err;
    ngx_uint_t            i, times, ident, qident, flags, رمز, nqs, nan, trunc,
                          qtype, qclass;
#اگر (NGX_HAVE_INET6)
    ngx_uint_t            qident6;
#اگر_ختم
    ngx_قطار_t          *q;
    ngx_resolver_qs_t    *qs;
    ngx_resolver_hdr_t   *response;
    ngx_resolver_گرہ_t  *rn;

    اگر (n < حجم(ngx_resolver_hdr_t)) {
        جاؤ short_response;
    }

    response = (ngx_resolver_hdr_t *) buf;

    ident = (response->ident_hi << 8) + response->ident_lo;
    flags = (response->flags_hi << 8) + response->flags_lo;
    nqs = (response->nqs_hi << 8) + response->nqs_lo;
    nan = (response->nan_hi << 8) + response->nan_lo;
    trunc = flags & 0x0200;

    ngx_log_debug6(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0,
                   "resolver DNS response %ui fl:%04Xi %ui/%ui/%ud/%ud",
                   ident, flags, nqs, nan,
                   (response->nns_hi << 8) + response->nns_lo,
                   (response->nar_hi << 8) + response->nar_lo);

    /* response to a standard query */
    اگر ((flags & 0xf870) != 0x8000 || (trunc && tcp)) {
        ngx_log_error(r->log_level, r->لوگارتھم, 0,
                      "invalid %s DNS response %ui fl:%04Xi",
                      tcp ? "TCP" : "UDP", ident, flags);
        واپس;
    }

    رمز = flags & 0xf;

    اگر (رمز == NGX_RESOLVE_FORMERR) {

        times = 0;

        برائے (q = ngx_قطار_سر(&r->name_resend_قطار);
             q != ngx_قطار_sentinel(&r->name_resend_قطار) && times++ < 100;
             q = ngx_قطار_اگلا(q))
        {
            rn = ngx_قطار_ڈیٹا(q, ngx_resolver_گرہ_t, قطار);

            اگر (rn->query) {
                qident = (rn->query[0] << 8) + rn->query[1];

                اگر (qident == ident) {
                    جاؤ dns_error_name;
                }
            }

#اگر (NGX_HAVE_INET6)
            اگر (rn->query6) {
                qident6 = (rn->query6[0] << 8) + rn->query6[1];

                اگر (qident6 == ident) {
                    جاؤ dns_error_name;
                }
            }
#اگر_ختم
        }

        جاؤ dns_error;
    }

    اگر (رمز > NGX_RESOLVE_REFUSED) {
        جاؤ dns_error;
    }

    اگر (nqs != 1) {
        err = "invalid number of questions in DNS response";
        جاؤ مکمل;
    }

    i = حجم(ngx_resolver_hdr_t);

    جب (i < (ngx_uint_t) n) {

        اگر (buf[i] & 0xc0) {
            err = "unexpected compression pointer in DNS response";
            جاؤ مکمل;
        }

        اگر (buf[i] == '\0') {
            جاؤ ملا;
        }

        i += 1 + buf[i];
    }

    جاؤ short_response;

ملا:

    اگر (i++ == حجم(ngx_resolver_hdr_t)) {
        err = "zero-length domain name in DNS response";
        جاؤ مکمل;
    }

    اگر (i + حجم(ngx_resolver_qs_t) + nan * (2 + حجم(ngx_resolver_an_t))
        > (ngx_uint_t) n)
    {
        جاؤ short_response;
    }

    qs = (ngx_resolver_qs_t *) &buf[i];

    qtype = (qs->type_hi << 8) + qs->type_lo;
    qclass = (qs->class_hi << 8) + qs->class_lo;

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0,
                   "resolver DNS response qt:%ui cl:%ui", qtype, qclass);

    اگر (qclass != 1) {
        ngx_log_error(r->log_level, r->لوگارتھم, 0,
                      "unknown query class %ui in DNS response", qclass);
        واپس;
    }

    انتخاب (qtype) {

    حالت NGX_RESOLVE_A:
#اگر (NGX_HAVE_INET6)
    حالت NGX_RESOLVE_AAAA:
#اگر_ختم

        ngx_resolver_process_a(r, buf, n, ident, رمز, qtype, nan, trunc,
                               i + حجم(ngx_resolver_qs_t));

        توڑو;

    حالت NGX_RESOLVE_SRV:

        ngx_resolver_process_srv(r, buf, n, ident, رمز, nan, trunc,
                                 i + حجم(ngx_resolver_qs_t));

        توڑو;

    حالت NGX_RESOLVE_PTR:

        ngx_resolver_process_ptr(r, buf, n, ident, رمز, nan);

        توڑو;

    طےشدہ:
        ngx_log_error(r->log_level, r->لوگارتھم, 0,
                      "unknown query type %ui in DNS response", qtype);
        واپس;
    }

    واپس;

short_response:

    err = "short DNS response";

مکمل:

    ngx_log_error(r->log_level, r->لوگارتھم, 0, err);

    واپس;

dns_error_name:

    ngx_log_error(r->log_level, r->لوگارتھم, 0,
                  "DNS error (%ui: %s), query id:%ui, name:\"%*s\"",
                  رمز, ngx_resolver_strerror(رمز), ident,
                  (سائز_t) rn->nlen, rn->name);
    واپس;

dns_error:

    ngx_log_error(r->log_level, r->لوگارتھم, 0,
                  "DNS error (%ui: %s), query id:%ui",
                  رمز, ngx_resolver_strerror(رمز), ident);
    واپس;
}


ساکن خالی
ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, سائز_t n,
    ngx_uint_t ident, ngx_uint_t رمز, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans)
{
    حرف                       *err;
    u_char                     *cname;
    سائز_t                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    اندر_addr_t                  *addr;
    ngx_str_t                   name;
    ngx_uint_t                  type, class, qident, naddrs, a, i, j, آغاز;
#اگر (NGX_HAVE_INET6)
    ساخت in6_addr            *addr6;
#اگر_ختم
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *اگلا;
    ngx_resolver_گرہ_t        *rn;
    ngx_resolver_addr_t        *addrs;
    ngx_resolver_connection_t  *rec;

    اگر (ngx_resolver_copy(r, &name, buf,
                          buf + حجم(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        واپس;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0, "resolver qs:%V", &name);

    hash = ngx_چکری_جانچ_32_short(name.ڈیٹا, name.len);

    /* lock name mutex */

    rn = ngx_resolver_lookup_name(r, &name, hash);

    اگر (rn == تہی) {
        ngx_log_error(r->log_level, r->لوگارتھم, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.ڈیٹا);
        جاؤ failed;
    }

    انتخاب (qtype) {

#اگر (NGX_HAVE_INET6)
    حالت NGX_RESOLVE_AAAA:

        اگر (rn->query6 == تہی || rn->naddrs6 != (u_short) -1) {
            ngx_log_error(r->log_level, r->لوگارتھم, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.ڈیٹا);
            جاؤ failed;
        }

        اگر (trunc && rn->tcp6) {
            ngx_resolver_free(r, name.ڈیٹا);
            جاؤ failed;
        }

        qident = (rn->query6[0] << 8) + rn->query6[1];

        توڑو;
#اگر_ختم

    طےشدہ: /* NGX_RESOLVE_A */

        اگر (rn->query == تہی || rn->naddrs != (u_short) -1) {
            ngx_log_error(r->log_level, r->لوگارتھم, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.ڈیٹا);
            جاؤ failed;
        }

        اگر (trunc && rn->tcp) {
            ngx_resolver_free(r, name.ڈیٹا);
            جاؤ failed;
        }

        qident = (rn->query[0] << 8) + rn->query[1];
    }

    اگر (ident != qident) {
        ngx_log_error(r->log_level, r->لوگارتھم, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.ڈیٹا);
        جاؤ failed;
    }

    ngx_resolver_free(r, name.ڈیٹا);

    اگر (trunc) {

        ngx_قطار_remove(&rn->قطار);

        اگر (rn->انتظار == تہی) {
            ngx_rbtree_delete(&r->name_rbtree, &rn->گرہ);
            ngx_resolver_free_گرہ(r, rn);
            جاؤ اگلا;
        }

        rec = r->connections.elts;
        rec = &rec[rn->آخری_connection];

        انتخاب (qtype) {

#اگر (NGX_HAVE_INET6)
        حالت NGX_RESOLVE_AAAA:

            rn->tcp6 = 1;

            (خالی) ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen);

            توڑو;
#اگر_ختم

        طےشدہ: /* NGX_RESOLVE_A */

            rn->tcp = 1;

            (خالی) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);
        }

        rn->expire = ngx_وقت() + r->resend_timeout;

        ngx_قطار_داخل_کرو_سر(&r->name_resend_قطار, &rn->قطار);

        جاؤ اگلا;
    }

    اگر (رمز == 0 && rn->رمز) {
        رمز = rn->رمز;
    }

    اگر (رمز == 0 && nan == 0) {

#اگر (NGX_HAVE_INET6)
        انتخاب (qtype) {

        حالت NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            اگر (rn->naddrs == (u_short) -1) {
                جاؤ اگلا;
            }

            اگر (rn->naddrs) {
                جاؤ export;
            }

            توڑو;

        طےشدہ: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            اگر (rn->naddrs6 == (u_short) -1) {
                جاؤ اگلا;
            }

            اگر (rn->naddrs6) {
                جاؤ export;
            }
        }
#اگر_ختم

        رمز = NGX_RESOLVE_NXDOMAIN;
    }

    اگر (رمز) {

#اگر (NGX_HAVE_INET6)
        انتخاب (qtype) {

        حالت NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            اگر (rn->naddrs == (u_short) -1) {
                rn->رمز = (u_char) رمز;
                جاؤ اگلا;
            }

            توڑو;

        طےشدہ: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            اگر (rn->naddrs6 == (u_short) -1) {
                rn->رمز = (u_char) رمز;
                جاؤ اگلا;
            }
        }
#اگر_ختم

        اگلا = rn->انتظار;
        rn->انتظار = تہی;

        ngx_قطار_remove(&rn->قطار);

        ngx_rbtree_delete(&r->name_rbtree, &rn->گرہ);

        /* unlock name mutex */

        جب (اگلا) {
            ctx = اگلا;
            ctx->state = رمز;
            ctx->valid = ngx_وقت() + (r->valid ? r->valid : 10);
            اگلا = ctx->اگلا;

            ctx->handler(ctx);
        }

        ngx_resolver_free_گرہ(r, rn);

        واپس;
    }

    i = ans;
    naddrs = 0;
    cname = تہی;

    برائے (a = 0; a < nan; a++) {

        آغاز = i;

        جب (i < n) {

            اگر (buf[i] & 0xc0) {
                i += 2;
                جاؤ ملا;
            }

            اگر (buf[i] == 0) {
                i++;
                جاؤ test_لمبائی;
            }

            i += 1 + buf[i];
        }

        جاؤ short_response;

    test_لمبائی:

        اگر (i - آغاز < 2) {
            err = "invalid name in DNS response";
            جاؤ invalid;
        }

    ملا:

        اگر (i + حجم(ngx_resolver_an_t) >= n) {
            جاؤ short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        اگر (class != 1) {
            ngx_log_error(r->log_level, r->لوگارتھم, 0,
                          "unexpected RR class %ui in DNS response", class);
            جاؤ failed;
        }

        اگر (ttl < 0) {
            ttl = 0;
        }

        rn->ttl = ngx_min(rn->ttl, (uint32_t) ttl);

        i += حجم(ngx_resolver_an_t);

        انتخاب (type) {

        حالت NGX_RESOLVE_A:

            اگر (qtype != NGX_RESOLVE_A) {
                err = "unexpected A record in DNS response";
                جاؤ invalid;
            }

            اگر (len != 4) {
                err = "invalid A record in DNS response";
                جاؤ invalid;
            }

            اگر (i + 4 > n) {
                جاؤ short_response;
            }

            naddrs++;

            توڑو;

#اگر (NGX_HAVE_INET6)
        حالت NGX_RESOLVE_AAAA:

            اگر (qtype != NGX_RESOLVE_AAAA) {
                err = "unexpected AAAA record in DNS response";
                جاؤ invalid;
            }

            اگر (len != 16) {
                err = "invalid AAAA record in DNS response";
                جاؤ invalid;
            }

            اگر (i + 16 > n) {
                جاؤ short_response;
            }

            naddrs++;

            توڑو;
#اگر_ختم

        حالت NGX_RESOLVE_CNAME:

            cname = &buf[i];

            توڑو;

        حالت NGX_RESOLVE_DNAME:

            توڑو;

        طےشدہ:

            ngx_log_error(r->log_level, r->لوگارتھم, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0,
                   "resolver naddrs:%ui cname:%p ttl:%uD",
                   naddrs, cname, rn->ttl);

    اگر (naddrs) {

        انتخاب (qtype) {

#اگر (NGX_HAVE_INET6)
        حالت NGX_RESOLVE_AAAA:

            اگر (naddrs == 1) {
                addr6 = &rn->u6.addr6;
                rn->naddrs6 = 1;

            } ورنہ {
                addr6 = ngx_resolver_alloc(r, naddrs * حجم(ساخت in6_addr));
                اگر (addr6 == تہی) {
                    جاؤ failed;
                }

                rn->u6.addrs6 = addr6;
                rn->naddrs6 = (u_short) naddrs;
            }

#اگر (NGX_SUPPRESS_WARN)
            addr = تہی;
#اگر_ختم

            توڑو;
#اگر_ختم

        طےشدہ: /* NGX_RESOLVE_A */

            اگر (naddrs == 1) {
                addr = &rn->u.addr;
                rn->naddrs = 1;

            } ورنہ {
                addr = ngx_resolver_alloc(r, naddrs * حجم(اندر_addr_t));
                اگر (addr == تہی) {
                    جاؤ failed;
                }

                rn->u.addrs = addr;
                rn->naddrs = (u_short) naddrs;
            }

#اگر (NGX_HAVE_INET6 && NGX_SUPPRESS_WARN)
            addr6 = تہی;
#اگر_ختم
        }

        j = 0;
        i = ans;

        برائے (a = 0; a < nan; a++) {

            برائے ( ;; ) {

                اگر (buf[i] & 0xc0) {
                    i += 2;
                    توڑو;
                }

                اگر (buf[i] == 0) {
                    i++;
                    توڑو;
                }

                i += 1 + buf[i];
            }

            an = (ngx_resolver_an_t *) &buf[i];

            type = (an->type_hi << 8) + an->type_lo;
            len = (an->len_hi << 8) + an->len_lo;

            i += حجم(ngx_resolver_an_t);

            اگر (type == NGX_RESOLVE_A) {

                addr[j] = htonl((buf[i] << 24) + (buf[i + 1] << 16)
                                + (buf[i + 2] << 8) + (buf[i + 3]));

                اگر (++j == naddrs) {

#اگر (NGX_HAVE_INET6)
                    اگر (rn->naddrs6 == (u_short) -1) {
                        جاؤ اگلا;
                    }
#اگر_ختم

                    توڑو;
                }
            }

#اگر (NGX_HAVE_INET6)
            ورنہ اگر (type == NGX_RESOLVE_AAAA) {

                ngx_memcpy(addr6[j].s6_addr, &buf[i], 16);

                اگر (++j == naddrs) {

                    اگر (rn->naddrs == (u_short) -1) {
                        جاؤ اگلا;
                    }

                    توڑو;
                }
            }
#اگر_ختم

            i += len;
        }
    }

    انتخاب (qtype) {

#اگر (NGX_HAVE_INET6)
    حالت NGX_RESOLVE_AAAA:

        اگر (rn->naddrs6 == (u_short) -1) {
            rn->naddrs6 = 0;
        }

        توڑو;
#اگر_ختم

    طےشدہ: /* NGX_RESOLVE_A */

        اگر (rn->naddrs == (u_short) -1) {
            rn->naddrs = 0;
        }
    }

    اگر (rn->naddrs != (u_short) -1
#اگر (NGX_HAVE_INET6)
        && rn->naddrs6 != (u_short) -1
#اگر_ختم
        && rn->naddrs
#اگر (NGX_HAVE_INET6)
           + rn->naddrs6
#اگر_ختم
           > 0)
    {

#اگر (NGX_HAVE_INET6)
    export:
#اگر_ختم

        naddrs = rn->naddrs;
#اگر (NGX_HAVE_INET6)
        naddrs += rn->naddrs6;
#اگر_ختم

        اگر (naddrs == 1 && rn->naddrs == 1) {
            addrs = تہی;

        } ورنہ {
            addrs = ngx_resolver_export(r, rn, 0);
            اگر (addrs == تہی) {
                جاؤ failed;
            }
        }

        ngx_قطار_remove(&rn->قطار);

        rn->valid = ngx_وقت() + (r->valid ? r->valid : (وقت_t) rn->ttl);
        rn->expire = ngx_وقت() + r->expire;

        ngx_قطار_داخل_کرو_سر(&r->name_expire_قطار, &rn->قطار);

        اگلا = rn->انتظار;
        rn->انتظار = تہی;

        /* unlock name mutex */

        جب (اگلا) {
            ctx = اگلا;
            ctx->state = NGX_OK;
            ctx->valid = rn->valid;
            ctx->naddrs = naddrs;

            اگر (addrs == تہی) {
                ctx->addrs = &ctx->addr;
                ctx->addr.sockaddr = (ساخت sockaddr *) &ctx->جیب;
                ctx->addr.socklen = حجم(ساخت sockaddr_اندر);
                ngx_memzero(&ctx->جیب, حجم(ساخت sockaddr_اندر));
                ctx->جیب.sin_family = AF_INET;
                ctx->جیب.sin_addr.s_addr = rn->u.addr;

            } ورنہ {
                ctx->addrs = addrs;
            }

            اگلا = ctx->اگلا;

            ctx->handler(ctx);
        }

        اگر (addrs != تہی) {
            ngx_resolver_free(r, addrs->sockaddr);
            ngx_resolver_free(r, addrs);
        }

        ngx_resolver_free(r, rn->query);
        rn->query = تہی;
#اگر (NGX_HAVE_INET6)
        rn->query6 = تہی;
#اگر_ختم

        واپس;
    }

    اگر (cname) {

        /* CNAME only */

        اگر (rn->naddrs == (u_short) -1
#اگر (NGX_HAVE_INET6)
            || rn->naddrs6 == (u_short) -1
#اگر_ختم
            )
        {
            جاؤ اگلا;
        }

        اگر (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            جاؤ failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_قطار_remove(&rn->قطار);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.ڈیٹا;

        rn->valid = ngx_وقت() + (r->valid ? r->valid : (وقت_t) rn->ttl);
        rn->expire = ngx_وقت() + r->expire;

        ngx_قطار_داخل_کرو_سر(&r->name_expire_قطار, &rn->قطار);

        ngx_resolver_free(r, rn->query);
        rn->query = تہی;
#اگر (NGX_HAVE_INET6)
        rn->query6 = تہی;
#اگر_ختم

        ctx = rn->انتظار;
        rn->انتظار = تہی;

        اگر (ctx) {

            اگر (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                کرو {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    اگلا = ctx->اگلا;

                    ctx->handler(ctx);

                    ctx = اگلا;
                } جب (ctx);

                واپس;
            }

            برائے (اگلا = ctx; اگلا; اگلا = اگلا->اگلا) {
                اگلا->گرہ = تہی;
            }

            (خالی) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        واپس;
    }

    ngx_log_error(r->log_level, r->لوگارتھم, 0,
                  "no A or CNAME types in DNS response");
    واپس;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->لوگارتھم, 0, err);

    واپس;

failed:

اگلا:

    /* unlock name mutex */

    واپس;
}


ساکن خالی
ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, سائز_t n,
    ngx_uint_t ident, ngx_uint_t رمز, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans)
{
    حرف                       *err;
    u_char                     *cname;
    سائز_t                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    ngx_str_t                   name;
    ngx_uint_t                  type, qident, class, آغاز, nsrvs, a, i, j;
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *اگلا;
    ngx_resolver_srv_t         *srvs;
    ngx_resolver_گرہ_t        *rn;
    ngx_resolver_connection_t  *rec;

    اگر (ngx_resolver_copy(r, &name, buf,
                          buf + حجم(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        واپس;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0, "resolver qs:%V", &name);

    hash = ngx_چکری_جانچ_32_short(name.ڈیٹا, name.len);

    rn = ngx_resolver_lookup_srv(r, &name, hash);

    اگر (rn == تہی || rn->query == تہی) {
        ngx_log_error(r->log_level, r->لوگارتھم, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.ڈیٹا);
        جاؤ failed;
    }

    اگر (trunc && rn->tcp) {
        ngx_resolver_free(r, name.ڈیٹا);
        جاؤ failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    اگر (ident != qident) {
        ngx_log_error(r->log_level, r->لوگارتھم, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.ڈیٹا);
        جاؤ failed;
    }

    ngx_resolver_free(r, name.ڈیٹا);

    اگر (trunc) {

        ngx_قطار_remove(&rn->قطار);

        اگر (rn->انتظار == تہی) {
            ngx_rbtree_delete(&r->srv_rbtree, &rn->گرہ);
            ngx_resolver_free_گرہ(r, rn);
            واپس;
        }

        rec = r->connections.elts;
        rec = &rec[rn->آخری_connection];

        rn->tcp = 1;

        (خالی) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);

        rn->expire = ngx_وقت() + r->resend_timeout;

        ngx_قطار_داخل_کرو_سر(&r->srv_resend_قطار, &rn->قطار);

        واپس;
    }

    اگر (رمز == 0 && rn->رمز) {
        رمز = rn->رمز;
    }

    اگر (رمز == 0 && nan == 0) {
        رمز = NGX_RESOLVE_NXDOMAIN;
    }

    اگر (رمز) {
        اگلا = rn->انتظار;
        rn->انتظار = تہی;

        ngx_قطار_remove(&rn->قطار);

        ngx_rbtree_delete(&r->srv_rbtree, &rn->گرہ);

        جب (اگلا) {
            ctx = اگلا;
            ctx->state = رمز;
            ctx->valid = ngx_وقت() + (r->valid ? r->valid : 10);
            اگلا = ctx->اگلا;

            ctx->handler(ctx);
        }

        ngx_resolver_free_گرہ(r, rn);

        واپس;
    }

    i = ans;
    nsrvs = 0;
    cname = تہی;

    برائے (a = 0; a < nan; a++) {

        آغاز = i;

        جب (i < n) {

            اگر (buf[i] & 0xc0) {
                i += 2;
                جاؤ ملا;
            }

            اگر (buf[i] == 0) {
                i++;
                جاؤ test_لمبائی;
            }

            i += 1 + buf[i];
        }

        جاؤ short_response;

    test_لمبائی:

        اگر (i - آغاز < 2) {
            err = "invalid name DNS response";
            جاؤ invalid;
        }

    ملا:

        اگر (i + حجم(ngx_resolver_an_t) >= n) {
            جاؤ short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        اگر (class != 1) {
            ngx_log_error(r->log_level, r->لوگارتھم, 0,
                          "unexpected RR class %ui in DNS response", class);
            جاؤ failed;
        }

        اگر (ttl < 0) {
            ttl = 0;
        }

        rn->ttl = ngx_min(rn->ttl, (uint32_t) ttl);

        i += حجم(ngx_resolver_an_t);

        انتخاب (type) {

        حالت NGX_RESOLVE_SRV:

            اگر (i + 6 >= n) {
                جاؤ short_response;
            }

            اگر (ngx_resolver_copy(r, تہی, buf, &buf[i + 6], buf + n)
                != NGX_OK)
            {
                جاؤ failed;
            }

            nsrvs++;

            توڑو;

        حالت NGX_RESOLVE_CNAME:

            cname = &buf[i];

            توڑو;

        حالت NGX_RESOLVE_DNAME:

            توڑو;

        طےشدہ:

            ngx_log_error(r->log_level, r->لوگارتھم, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0,
                   "resolver nsrvs:%ui cname:%p ttl:%uD",
                   nsrvs, cname, rn->ttl);

    اگر (nsrvs) {

        srvs = ngx_resolver_calloc(r, nsrvs * حجم(ngx_resolver_srv_t));
        اگر (srvs == تہی) {
            جاؤ failed;
        }

        rn->u.srvs = srvs;
        rn->nsrvs = (u_short) nsrvs;

        j = 0;
        i = ans;

        برائے (a = 0; a < nan; a++) {

            برائے ( ;; ) {

                اگر (buf[i] & 0xc0) {
                    i += 2;
                    توڑو;
                }

                اگر (buf[i] == 0) {
                    i++;
                    توڑو;
                }

                i += 1 + buf[i];
            }

            an = (ngx_resolver_an_t *) &buf[i];

            type = (an->type_hi << 8) + an->type_lo;
            len = (an->len_hi << 8) + an->len_lo;

            i += حجم(ngx_resolver_an_t);

            اگر (type == NGX_RESOLVE_SRV) {

                srvs[j].ترجیح = (buf[i] << 8) + buf[i + 1];
                srvs[j].وزن = (buf[i + 2] << 8) + buf[i + 3];

                اگر (srvs[j].وزن == 0) {
                    srvs[j].وزن = 1;
                }

                srvs[j].پورٹ = (buf[i + 4] << 8) + buf[i + 5];

                اگر (ngx_resolver_copy(r, &srvs[j].name, buf, &buf[i + 6],
                                      buf + n)
                    != NGX_OK)
                {
                    جاؤ failed;
                }

                j++;
            }

            i += len;
        }

        ngx_ترتیب_دو(srvs, nsrvs, حجم(ngx_resolver_srv_t),
                 ngx_resolver_cmp_srvs);

        ngx_resolver_free(r, rn->query);
        rn->query = تہی;

        ngx_قطار_remove(&rn->قطار);

        rn->valid = ngx_وقت() + (r->valid ? r->valid : (وقت_t) rn->ttl);
        rn->expire = ngx_وقت() + r->expire;

        ngx_قطار_داخل_کرو_سر(&r->srv_expire_قطار, &rn->قطار);

        اگلا = rn->انتظار;
        rn->انتظار = تہی;

        جب (اگلا) {
            ctx = اگلا;
            اگلا = ctx->اگلا;

            ngx_resolver_resolve_srv_نام(ctx, rn);
        }

        واپس;
    }

    rn->nsrvs = 0;

    اگر (cname) {

        /* CNAME only */

        اگر (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            جاؤ failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_قطار_remove(&rn->قطار);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.ڈیٹا;

        rn->valid = ngx_وقت() + (r->valid ? r->valid : (وقت_t) rn->ttl);
        rn->expire = ngx_وقت() + r->expire;

        ngx_قطار_داخل_کرو_سر(&r->srv_expire_قطار, &rn->قطار);

        ngx_resolver_free(r, rn->query);
        rn->query = تہی;
#اگر (NGX_HAVE_INET6)
        rn->query6 = تہی;
#اگر_ختم

        ctx = rn->انتظار;
        rn->انتظار = تہی;

        اگر (ctx) {

            اگر (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                کرو {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    اگلا = ctx->اگلا;

                    ctx->handler(ctx);

                    ctx = اگلا;
                } جب (ctx);

                واپس;
            }

            برائے (اگلا = ctx; اگلا; اگلا = اگلا->اگلا) {
                اگلا->گرہ = تہی;
            }

            (خالی) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        واپس;
    }

    ngx_log_error(r->log_level, r->لوگارتھم, 0, "no SRV type in DNS response");

    واپس;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->لوگارتھم, 0, err);

    واپس;

failed:

    /* unlock name mutex */

    واپس;
}


ساکن خالی
ngx_resolver_resolve_srv_نام(ngx_resolver_ctx_t *ctx, ngx_resolver_گرہ_t *rn)
{
    ngx_uint_t                i;
    ngx_resolver_t           *r;
    ngx_resolver_ctx_t       *cctx;
    ngx_resolver_srv_name_t  *srvs;

    r = ctx->resolver;

    ctx->گرہ = تہی;
    ctx->state = NGX_OK;
    ctx->valid = rn->valid;
    ctx->گنتی = rn->nsrvs;

    srvs = ngx_resolver_calloc(r, rn->nsrvs * حجم(ngx_resolver_srv_name_t));
    اگر (srvs == تہی) {
        جاؤ failed;
    }

    ctx->srvs = srvs;
    ctx->nsrvs = rn->nsrvs;

    اگر (ctx->event && ctx->event->timer_سیٹ) {
        ngx_del_timer(ctx->event);
    }

    برائے (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
        srvs[i].name.ڈیٹا = ngx_resolver_alloc(r, rn->u.srvs[i].name.len);
        اگر (srvs[i].name.ڈیٹا == تہی) {
            جاؤ failed;
        }

        srvs[i].name.len = rn->u.srvs[i].name.len;
        ngx_memcpy(srvs[i].name.ڈیٹا, rn->u.srvs[i].name.ڈیٹا,
                   srvs[i].name.len);

        cctx = ngx_resolve_آغاز(r, تہی);
        اگر (cctx == تہی) {
            جاؤ failed;
        }

        cctx->name = srvs[i].name;
        cctx->handler = ngx_resolver_srv_نام_handler;
        cctx->ڈیٹا = ctx;
        cctx->srvs = &srvs[i];
        cctx->timeout = ctx->timeout;

        srvs[i].ترجیح = rn->u.srvs[i].ترجیح;
        srvs[i].وزن = rn->u.srvs[i].وزن;
        srvs[i].پورٹ = rn->u.srvs[i].پورٹ;
        srvs[i].ctx = cctx;

        اگر (ngx_resolve_name(cctx) == NGX_ERROR) {
            srvs[i].ctx = تہی;
            جاؤ failed;
        }
    }

    واپس;

failed:

    ctx->state = NGX_ERROR;
    ctx->valid = ngx_وقت() + (r->valid ? r->valid : 10);

    ctx->handler(ctx);
}


ساکن خالی
ngx_resolver_srv_نام_handler(ngx_resolver_ctx_t *cctx)
{
    ngx_uint_t                i;
    ngx_addr_t               *addrs;
    ngx_resolver_t           *r;
    ngx_sockaddr_t           *sockaddr;
    ngx_resolver_ctx_t       *ctx;
    ngx_resolver_srv_name_t  *srv;

    r = cctx->resolver;
    ctx = cctx->ڈیٹا;
    srv = cctx->srvs;

    ctx->گنتی--;
    ctx->async |= cctx->async;

    srv->ctx = تہی;
    srv->state = cctx->state;

    اگر (cctx->naddrs) {

        ctx->valid = ngx_min(ctx->valid, cctx->valid);

        addrs = ngx_resolver_calloc(r, cctx->naddrs * حجم(ngx_addr_t));
        اگر (addrs == تہی) {
            srv->state = NGX_ERROR;
            جاؤ مکمل;
        }

        sockaddr = ngx_resolver_alloc(r, cctx->naddrs * حجم(ngx_sockaddr_t));
        اگر (sockaddr == تہی) {
            ngx_resolver_free(r, addrs);
            srv->state = NGX_ERROR;
            جاؤ مکمل;
        }

        برائے (i = 0; i < cctx->naddrs; i++) {
            addrs[i].sockaddr = &sockaddr[i].sockaddr;
            addrs[i].socklen = cctx->addrs[i].socklen;

            ngx_memcpy(&sockaddr[i], cctx->addrs[i].sockaddr,
                       addrs[i].socklen);

            ngx_inet_سیٹ_پورٹ(addrs[i].sockaddr, srv->پورٹ);
        }

        srv->addrs = addrs;
        srv->naddrs = cctx->naddrs;
    }

مکمل:

    ngx_resolve_name_مکمل(cctx);

    اگر (ctx->گنتی == 0) {
        ngx_resolver_report_srv(r, ctx);
    }
}


ساکن خالی
ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, سائز_t n,
    ngx_uint_t ident, ngx_uint_t رمز, ngx_uint_t nan)
{
    حرف                 *err;
    سائز_t                len;
    اندر_addr_t             addr;
    int32_t               ttl;
    ngx_int_t             octet;
    ngx_str_t             name;
    ngx_uint_t            نقاب, type, class, qident, a, i, آغاز;
    ngx_قطار_t          *expire_قطار;
    ngx_rbtree_t         *درخت;
    ngx_resolver_an_t    *an;
    ngx_resolver_ctx_t   *ctx, *اگلا;
    ngx_resolver_گرہ_t  *rn;
#اگر (NGX_HAVE_INET6)
    uint32_t              hash;
    ngx_int_t             digit;
    ساخت in6_addr       addr6;
#اگر_ختم

    اگر (ngx_resolver_copy(r, &name, buf,
                          buf + حجم(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        واپس;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0, "resolver qs:%V", &name);

    /* AF_INET */

    addr = 0;
    i = حجم(ngx_resolver_hdr_t);

    برائے (نقاب = 0; نقاب < 32; نقاب += 8) {
        len = buf[i++];

        octet = ngx_atoi(&buf[i], len);
        اگر (octet == NGX_ERROR || octet > 255) {
            جاؤ invalid_اندر_addr_arpa;
        }

        addr += octet << نقاب;
        i += len;
    }

    اگر (ngx_strcasecmp(&buf[i], (u_char *) "\7in-addr\4arpa") == 0) {
        i += حجم("\7in-addr\4arpa");

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        درخت = &r->addr_rbtree;
        expire_قطار = &r->addr_expire_قطار;

        جاؤ valid;
    }

invalid_اندر_addr_arpa:

#اگر (NGX_HAVE_INET6)

    i = حجم(ngx_resolver_hdr_t);

    برائے (octet = 15; octet >= 0; octet--) {
        اگر (buf[i++] != '\1') {
            جاؤ invalid_ip6_arpa;
        }

        digit = ngx_hextoi(&buf[i++], 1);
        اگر (digit == NGX_ERROR) {
            جاؤ invalid_ip6_arpa;
        }

        addr6.s6_addr[octet] = (u_char) digit;

        اگر (buf[i++] != '\1') {
            جاؤ invalid_ip6_arpa;
        }

        digit = ngx_hextoi(&buf[i++], 1);
        اگر (digit == NGX_ERROR) {
            جاؤ invalid_ip6_arpa;
        }

        addr6.s6_addr[octet] += (u_char) (digit * 16);
    }

    اگر (ngx_strcasecmp(&buf[i], (u_char *) "\3ip6\4arpa") == 0) {
        i += حجم("\3ip6\4arpa");

        /* lock addr mutex */

        hash = ngx_چکری_جانچ_32_short(addr6.s6_addr, 16);
        rn = ngx_resolver_lookup_addr6(r, &addr6, hash);

        درخت = &r->addr6_rbtree;
        expire_قطار = &r->addr6_expire_قطار;

        جاؤ valid;
    }

invalid_ip6_arpa:
#اگر_ختم

    ngx_log_error(r->log_level, r->لوگارتھم, 0,
                  "invalid in-addr.arpa or ip6.arpa name in DNS response");
    ngx_resolver_free(r, name.ڈیٹا);
    واپس;

valid:

    اگر (rn == تہی || rn->query == تہی) {
        ngx_log_error(r->log_level, r->لوگارتھم, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.ڈیٹا);
        جاؤ failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    اگر (ident != qident) {
        ngx_log_error(r->log_level, r->لوگارتھم, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.ڈیٹا);
        جاؤ failed;
    }

    ngx_resolver_free(r, name.ڈیٹا);

    اگر (رمز == 0 && nan == 0) {
        رمز = NGX_RESOLVE_NXDOMAIN;
    }

    اگر (رمز) {
        اگلا = rn->انتظار;
        rn->انتظار = تہی;

        ngx_قطار_remove(&rn->قطار);

        ngx_rbtree_delete(درخت, &rn->گرہ);

        /* unlock addr mutex */

        جب (اگلا) {
            ctx = اگلا;
            ctx->state = رمز;
            ctx->valid = ngx_وقت() + (r->valid ? r->valid : 10);
            اگلا = ctx->اگلا;

            ctx->handler(ctx);
        }

        ngx_resolver_free_گرہ(r, rn);

        واپس;
    }

    i += حجم(ngx_resolver_qs_t);

    برائے (a = 0; a < nan; a++) {

        آغاز = i;

        جب (i < n) {

            اگر (buf[i] & 0xc0) {
                i += 2;
                جاؤ ملا;
            }

            اگر (buf[i] == 0) {
                i++;
                جاؤ test_لمبائی;
            }

            i += 1 + buf[i];
        }

        جاؤ short_response;

    test_لمبائی:

        اگر (i - آغاز < 2) {
            err = "invalid name in DNS response";
            جاؤ invalid;
        }

    ملا:

        اگر (i + حجم(ngx_resolver_an_t) >= n) {
            جاؤ short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        اگر (class != 1) {
            ngx_log_error(r->log_level, r->لوگارتھم, 0,
                          "unexpected RR class %ui in DNS response", class);
            جاؤ failed;
        }

        اگر (ttl < 0) {
            ttl = 0;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0,
                      "resolver qt:%ui cl:%ui len:%uz",
                      type, class, len);

        i += حجم(ngx_resolver_an_t);

        انتخاب (type) {

        حالت NGX_RESOLVE_PTR:

            جاؤ ptr;

        حالت NGX_RESOLVE_CNAME:

            توڑو;

        طےشدہ:

            ngx_log_error(r->log_level, r->لوگارتھم, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->لوگارتھم, 0,
                  "no PTR type in DNS response");
    واپس;

ptr:

    اگر (ngx_resolver_copy(r, &name, buf, buf + i, buf + n) != NGX_OK) {
        جاؤ failed;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0, "resolver an:%V", &name);

    اگر (name.len != (سائز_t) rn->nlen
        || ngx_strncmp(name.ڈیٹا, rn->name, name.len) != 0)
    {
        اگر (rn->nlen) {
            ngx_resolver_free(r, rn->name);
        }

        rn->nlen = (u_short) name.len;
        rn->name = name.ڈیٹا;

        name.ڈیٹا = ngx_resolver_dup(r, rn->name, name.len);
        اگر (name.ڈیٹا == تہی) {
            جاؤ failed;
        }
    }

    ngx_قطار_remove(&rn->قطار);

    rn->valid = ngx_وقت() + (r->valid ? r->valid : ttl);
    rn->expire = ngx_وقت() + r->expire;

    ngx_قطار_داخل_کرو_سر(expire_قطار, &rn->قطار);

    اگلا = rn->انتظار;
    rn->انتظار = تہی;

    /* unlock addr mutex */

    جب (اگلا) {
        ctx = اگلا;
        ctx->state = NGX_OK;
        ctx->valid = rn->valid;
        ctx->name = name;
        اگلا = ctx->اگلا;

        ctx->handler(ctx);
    }

    ngx_resolver_free(r, name.ڈیٹا);

    واپس;

short_response:

    err = "short DNS response";

invalid:

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->لوگارتھم, 0, err);

    واپس;

failed:

    /* unlock addr mutex */

    واپس;
}


ساکن ngx_resolver_گرہ_t *
ngx_resolver_lookup_name(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_گرہ_t    *گرہ, *sentinel;
    ngx_resolver_گرہ_t  *rn;

    گرہ = r->name_rbtree.جڑ;
    sentinel = r->name_rbtree.sentinel;

    جب (گرہ != sentinel) {

        اگر (hash < گرہ->کلید) {
            گرہ = گرہ->بایاں;
            جاری;
        }

        اگر (hash > گرہ->کلید) {
            گرہ = گرہ->دایاں;
            جاری;
        }

        /* hash == node->key */

        rn = ngx_resolver_گرہ(گرہ);

        rc = ngx_memn2cmp(name->ڈیٹا, rn->name, name->len, rn->nlen);

        اگر (rc == 0) {
            واپس rn;
        }

        گرہ = (rc < 0) ? گرہ->بایاں : گرہ->دایاں;
    }

    /* not found */

    واپس تہی;
}


ساکن ngx_resolver_گرہ_t *
ngx_resolver_lookup_srv(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_گرہ_t    *گرہ, *sentinel;
    ngx_resolver_گرہ_t  *rn;

    گرہ = r->srv_rbtree.جڑ;
    sentinel = r->srv_rbtree.sentinel;

    جب (گرہ != sentinel) {

        اگر (hash < گرہ->کلید) {
            گرہ = گرہ->بایاں;
            جاری;
        }

        اگر (hash > گرہ->کلید) {
            گرہ = گرہ->دایاں;
            جاری;
        }

        /* hash == node->key */

        rn = ngx_resolver_گرہ(گرہ);

        rc = ngx_memn2cmp(name->ڈیٹا, rn->name, name->len, rn->nlen);

        اگر (rc == 0) {
            واپس rn;
        }

        گرہ = (rc < 0) ? گرہ->بایاں : گرہ->دایاں;
    }

    /* not found */

    واپس تہی;
}


ساکن ngx_resolver_گرہ_t *
ngx_resolver_lookup_addr(ngx_resolver_t *r, اندر_addr_t addr)
{
    ngx_rbtree_گرہ_t  *گرہ, *sentinel;

    گرہ = r->addr_rbtree.جڑ;
    sentinel = r->addr_rbtree.sentinel;

    جب (گرہ != sentinel) {

        اگر (addr < گرہ->کلید) {
            گرہ = گرہ->بایاں;
            جاری;
        }

        اگر (addr > گرہ->کلید) {
            گرہ = گرہ->دایاں;
            جاری;
        }

        /* addr == node->key */

        واپس ngx_resolver_گرہ(گرہ);
    }

    /* not found */

    واپس تہی;
}


#اگر (NGX_HAVE_INET6)

ساکن ngx_resolver_گرہ_t *
ngx_resolver_lookup_addr6(ngx_resolver_t *r, ساخت in6_addr *addr,
    uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_گرہ_t    *گرہ, *sentinel;
    ngx_resolver_گرہ_t  *rn;

    گرہ = r->addr6_rbtree.جڑ;
    sentinel = r->addr6_rbtree.sentinel;

    جب (گرہ != sentinel) {

        اگر (hash < گرہ->کلید) {
            گرہ = گرہ->بایاں;
            جاری;
        }

        اگر (hash > گرہ->کلید) {
            گرہ = گرہ->دایاں;
            جاری;
        }

        /* hash == node->key */

        rn = ngx_resolver_گرہ(گرہ);

        rc = ngx_memcmp(addr, &rn->addr6, 16);

        اگر (rc == 0) {
            واپس rn;
        }

        گرہ = (rc < 0) ? گرہ->بایاں : گرہ->دایاں;
    }

    /* not found */

    واپس تہی;
}

#اگر_ختم


ساکن خالی
ngx_resolver_rbtree_داخل_کرو_قدر(ngx_rbtree_گرہ_t *عارضی,
    ngx_rbtree_گرہ_t *گرہ, ngx_rbtree_گرہ_t *sentinel)
{
    ngx_rbtree_گرہ_t    **p;
    ngx_resolver_گرہ_t   *rn, *rn_عارضی;

    برائے ( ;; ) {

        اگر (گرہ->کلید < عارضی->کلید) {

            p = &عارضی->بایاں;

        } ورنہ اگر (گرہ->کلید > عارضی->کلید) {

            p = &عارضی->دایاں;

        } ورنہ { /* node->key == temp->key */

            rn = ngx_resolver_گرہ(گرہ);
            rn_عارضی = ngx_resolver_گرہ(عارضی);

            p = (ngx_memn2cmp(rn->name, rn_عارضی->name, rn->nlen, rn_عارضی->nlen)
                 < 0) ? &عارضی->بایاں : &عارضی->دایاں;
        }

        اگر (*p == sentinel) {
            توڑو;
        }

        عارضی = *p;
    }

    *p = گرہ;
    گرہ->والد = عارضی;
    گرہ->بایاں = sentinel;
    گرہ->دایاں = sentinel;
    ngx_rbt_red(گرہ);
}


#اگر (NGX_HAVE_INET6)

ساکن خالی
ngx_resolver_rbtree_داخل_کرو_addr6_قدر(ngx_rbtree_گرہ_t *عارضی,
    ngx_rbtree_گرہ_t *گرہ, ngx_rbtree_گرہ_t *sentinel)
{
    ngx_rbtree_گرہ_t    **p;
    ngx_resolver_گرہ_t   *rn, *rn_عارضی;

    برائے ( ;; ) {

        اگر (گرہ->کلید < عارضی->کلید) {

            p = &عارضی->بایاں;

        } ورنہ اگر (گرہ->کلید > عارضی->کلید) {

            p = &عارضی->دایاں;

        } ورنہ { /* node->key == temp->key */

            rn = ngx_resolver_گرہ(گرہ);
            rn_عارضی = ngx_resolver_گرہ(عارضی);

            p = (ngx_memcmp(&rn->addr6, &rn_عارضی->addr6, 16)
                 < 0) ? &عارضی->بایاں : &عارضی->دایاں;
        }

        اگر (*p == sentinel) {
            توڑو;
        }

        عارضی = *p;
    }

    *p = گرہ;
    گرہ->والد = عارضی;
    گرہ->بایاں = sentinel;
    گرہ->دایاں = sentinel;
    ngx_rbt_red(گرہ);
}

#اگر_ختم


ساکن ngx_int_t
ngx_resolver_create_name_query(ngx_resolver_t *r, ngx_resolver_گرہ_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    سائز_t               len, nlen;
    ngx_uint_t           ident;
    ngx_resolver_qs_t   *qs;
    ngx_resolver_hdr_t  *query;

    nlen = name->len ? (1 + name->len + 1) : 1;

    len = حجم(ngx_resolver_hdr_t) + nlen + حجم(ngx_resolver_qs_t);

#اگر (NGX_HAVE_INET6)
    p = ngx_resolver_alloc(r, len * (r->ipv4 + r->ipv6));
#ورنہ
    p = ngx_resolver_alloc(r, len);
#اگر_ختم
    اگر (p == تہی) {
        واپس NGX_ERROR;
    }

    rn->qlen = (u_short) len;
    rn->query = p;

#اگر (NGX_HAVE_INET6)
    اگر (r->ipv6) {
        rn->query6 = r->ipv4 ? (p + len) : p;
    }
#اگر_ختم

    query = (ngx_resolver_hdr_t *) p;

    اگر (r->ipv4) {
        ident = ngx_random();

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0,
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

    اگر (name->len == 0)  {
        واپس NGX_DECLINED;
    }

    برائے (s = name->ڈیٹا + name->len - 1; s >= name->ڈیٹا; s--) {
        اگر (*s != '.') {
            *p = *s;
            len++;

        } ورنہ {
            اگر (len == 0 || len > 255) {
                واپس NGX_DECLINED;
            }

            *p = (u_char) len;
            len = 0;
        }

        p--;
    }

    اگر (len == 0 || len > 255) {
        واپس NGX_DECLINED;
    }

    *p = (u_char) len;

#اگر (NGX_HAVE_INET6)
    اگر (!r->ipv6) {
        واپس NGX_OK;
    }

    p = rn->query6;

    اگر (r->ipv4) {
        ngx_memcpy(p, rn->query, rn->qlen);
    }

    query = (ngx_resolver_hdr_t *) p;

    ident = ngx_random();

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0,
                   "resolve: \"%V\" AAAA %i", name, ident & 0xffff);

    query->ident_hi = (u_char) ((ident >> 8) & 0xff);
    query->ident_lo = (u_char) (ident & 0xff);

    p += حجم(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    qs->type_lo = NGX_RESOLVE_AAAA;
#اگر_ختم

    واپس NGX_OK;
}


ساکن ngx_int_t
ngx_resolver_create_srv_query(ngx_resolver_t *r, ngx_resolver_گرہ_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    سائز_t               len, nlen;
    ngx_uint_t           ident;
    ngx_resolver_qs_t   *qs;
    ngx_resolver_hdr_t  *query;

    nlen = name->len ? (1 + name->len + 1) : 1;

    len = حجم(ngx_resolver_hdr_t) + nlen + حجم(ngx_resolver_qs_t);

    p = ngx_resolver_alloc(r, len);
    اگر (p == تہی) {
        واپس NGX_ERROR;
    }

    rn->qlen = (u_short) len;
    rn->query = p;

    query = (ngx_resolver_hdr_t *) p;

    ident = ngx_random();

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0,
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

    اگر (name->len == 0)  {
        واپس NGX_DECLINED;
    }

    برائے (s = name->ڈیٹا + name->len - 1; s >= name->ڈیٹا; s--) {
        اگر (*s != '.') {
            *p = *s;
            len++;

        } ورنہ {
            اگر (len == 0 || len > 255) {
                واپس NGX_DECLINED;
            }

            *p = (u_char) len;
            len = 0;
        }

        p--;
    }

    اگر (len == 0 || len > 255) {
        واپس NGX_DECLINED;
    }

    *p = (u_char) len;

    واپس NGX_OK;
}


ساکن ngx_int_t
ngx_resolver_create_addr_query(ngx_resolver_t *r, ngx_resolver_گرہ_t *rn,
    ngx_resolver_addr_t *addr)
{
    u_char               *p, *d;
    سائز_t                len;
    اندر_addr_t             inaddr;
    ngx_int_t             n;
    ngx_uint_t            ident;
    ngx_resolver_hdr_t   *query;
    ساخت sockaddr_اندر   *جیب;
#اگر (NGX_HAVE_INET6)
    ساخت sockaddr_in6  *sin6;
#اگر_ختم

    انتخاب (addr->sockaddr->sa_family) {

#اگر (NGX_HAVE_INET6)
    حالت AF_INET6:
        len = حجم(ngx_resolver_hdr_t)
              + 64 + حجم(".ip6.arpa.") - 1
              + حجم(ngx_resolver_qs_t);

        توڑو;
#اگر_ختم

    طےشدہ: /* AF_INET */
        len = حجم(ngx_resolver_hdr_t)
              + حجم(".255.255.255.255.in-addr.arpa.") - 1
              + حجم(ngx_resolver_qs_t);
    }

    p = ngx_resolver_alloc(r, len);
    اگر (p == تہی) {
        واپس NGX_ERROR;
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

    انتخاب (addr->sockaddr->sa_family) {

#اگر (NGX_HAVE_INET6)
    حالت AF_INET6:
        sin6 = (ساخت sockaddr_in6 *) addr->sockaddr;

        برائے (n = 15; n >= 0; n--) {
            p = ngx_sprintf(p, "\1%xd\1%xd",
                            sin6->sin6_addr.s6_addr[n] & 0xf,
                            (sin6->sin6_addr.s6_addr[n] >> 4) & 0xf);
        }

        p = ngx_cpymem(p, "\3ip6\4arpa\0", 10);

        توڑو;
#اگر_ختم

    طےشدہ: /* AF_INET */

        جیب = (ساخت sockaddr_اندر *) addr->sockaddr;
        inaddr = ntohl(جیب->sin_addr.s_addr);

        برائے (n = 0; n < 32; n += 8) {
            d = ngx_sprintf(&p[1], "%ud", (inaddr >> n) & 0xff);
            *p = (u_char) (d - &p[1]);
            p = d;
        }

        p = ngx_cpymem(p, "\7in-addr\4arpa\0", 14);
    }

    /* query type "PTR", IN query class */
    p = ngx_cpymem(p, "\0\14\0\1", 4);

    rn->qlen = (u_short) (p - rn->query);

    واپس NGX_OK;
}


ساکن ngx_int_t
ngx_resolver_copy(ngx_resolver_t *r, ngx_str_t *name, u_char *buf, u_char *src,
    u_char *آخری)
{
    حرف        *err;
    u_char      *p, *dst;
    سائز_t       len;
    ngx_uint_t   i, n;

    p = src;
    len = 0;

    /*
     * compression pointers allow to create endless loop, so we set limit;
     * 128 pointers should be enough to store 255-byte name
     */

    برائے (i = 0; i < 128; i++) {
        n = *p++;

        اگر (n == 0) {
            جاؤ مکمل;
        }

        اگر (n & 0xc0) {
            اگر ((n & 0xc0) != 0xc0) {
                err = "invalid label type in DNS response";
                جاؤ invalid;
            }

            اگر (p >= آخری) {
                err = "name is out of DNS response";
                جاؤ invalid;
            }

            n = ((n & 0x3f) << 8) + *p;
            p = &buf[n];

        } ورنہ {
            len += 1 + n;
            p = &p[n];
        }

        اگر (p >= آخری) {
            err = "name is out of DNS response";
            جاؤ invalid;
        }
    }

    err = "compression pointers loop in DNS response";

invalid:

    ngx_log_error(r->log_level, r->لوگارتھم, 0, err);

    واپس NGX_ERROR;

مکمل:

    اگر (name == تہی) {
        واپس NGX_OK;
    }

    اگر (len == 0) {
        ngx_str_null(name);
        واپس NGX_OK;
    }

    dst = ngx_resolver_alloc(r, len);
    اگر (dst == تہی) {
        واپس NGX_ERROR;
    }

    name->ڈیٹا = dst;

    برائے ( ;; ) {
        n = *src++;

        اگر (n == 0) {
            name->len = dst - name->ڈیٹا - 1;
            واپس NGX_OK;
        }

        اگر (n & 0xc0) {
            n = ((n & 0x3f) << 8) + *src;
            src = &buf[n];

        } ورنہ {
            ngx_strlow(dst, src, n);
            dst += n;
            src += n;
            *dst++ = '.';
        }
    }
}


ساکن ngx_int_t
ngx_resolver_سیٹ_timeout(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx)
{
    اگر (ctx->event || ctx->timeout == 0) {
        واپس NGX_OK;
    }

    ctx->event = ngx_resolver_calloc(r, حجم(ngx_event_t));
    اگر (ctx->event == تہی) {
        واپس NGX_ERROR;
    }

    ctx->event->handler = ngx_resolver_timeout_handler;
    ctx->event->ڈیٹا = ctx;
    ctx->event->لوگارتھم = r->لوگارتھم;
    ctx->event->cancelable = ctx->cancelable;
    ctx->ident = -1;

    ngx_جوڑو_timer(ctx->event, ctx->timeout);

    واپس NGX_OK;
}


ساکن خالی
ngx_resolver_timeout_handler(ngx_event_t *ev)
{
    ngx_resolver_ctx_t  *ctx;

    ctx = ev->ڈیٹا;

    ctx->state = NGX_RESOLVE_TIMEDOUT;

    ctx->handler(ctx);
}


ساکن خالی
ngx_resolver_free_گرہ(ngx_resolver_t *r, ngx_resolver_گرہ_t *rn)
{
    ngx_uint_t  i;

    /* lock alloc mutex */

    اگر (rn->query) {
        ngx_resolver_free_locked(r, rn->query);
    }

    اگر (rn->name) {
        ngx_resolver_free_locked(r, rn->name);
    }

    اگر (rn->cnlen) {
        ngx_resolver_free_locked(r, rn->u.cname);
    }

    اگر (rn->naddrs > 1 && rn->naddrs != (u_short) -1) {
        ngx_resolver_free_locked(r, rn->u.addrs);
    }

#اگر (NGX_HAVE_INET6)
    اگر (rn->naddrs6 > 1 && rn->naddrs6 != (u_short) -1) {
        ngx_resolver_free_locked(r, rn->u6.addrs6);
    }
#اگر_ختم

    اگر (rn->nsrvs) {
        برائے (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
            اگر (rn->u.srvs[i].name.ڈیٹا) {
                ngx_resolver_free_locked(r, rn->u.srvs[i].name.ڈیٹا);
            }
        }

        ngx_resolver_free_locked(r, rn->u.srvs);
    }

    ngx_resolver_free_locked(r, rn);

    /* unlock alloc mutex */
}


ساکن خالی *
ngx_resolver_alloc(ngx_resolver_t *r, سائز_t سائز)
{
    u_char  *p;

    /* lock alloc mutex */

    p = ngx_alloc(سائز, r->لوگارتھم);

    /* unlock alloc mutex */

    واپس p;
}


ساکن خالی *
ngx_resolver_calloc(ngx_resolver_t *r, سائز_t سائز)
{
    u_char  *p;

    p = ngx_resolver_alloc(r, سائز);

    اگر (p) {
        ngx_memzero(p, سائز);
    }

    واپس p;
}


ساکن خالی
ngx_resolver_free(ngx_resolver_t *r, خالی *p)
{
    /* lock alloc mutex */

    ngx_free(p);

    /* unlock alloc mutex */
}


ساکن خالی
ngx_resolver_free_locked(ngx_resolver_t *r, خالی *p)
{
    ngx_free(p);
}


ساکن خالی *
ngx_resolver_dup(ngx_resolver_t *r, خالی *src, سائز_t سائز)
{
    خالی  *dst;

    dst = ngx_resolver_alloc(r, سائز);

    اگر (dst == تہی) {
        واپس dst;
    }

    ngx_memcpy(dst, src, سائز);

    واپس dst;
}


ساکن ngx_resolver_addr_t *
ngx_resolver_export(ngx_resolver_t *r, ngx_resolver_گرہ_t *rn,
    ngx_uint_t rotate)
{
    ngx_uint_t            d, i, j, n;
    اندر_addr_t            *addr;
    ngx_sockaddr_t       *sockaddr;
    ساخت sockaddr_اندر   *جیب;
    ngx_resolver_addr_t  *dst;
#اگر (NGX_HAVE_INET6)
    ساخت in6_addr      *addr6;
    ساخت sockaddr_in6  *sin6;
#اگر_ختم

    n = rn->naddrs;
#اگر (NGX_HAVE_INET6)
    n += rn->naddrs6;
#اگر_ختم

    dst = ngx_resolver_calloc(r, n * حجم(ngx_resolver_addr_t));
    اگر (dst == تہی) {
        واپس تہی;
    }

    sockaddr = ngx_resolver_calloc(r, n * حجم(ngx_sockaddr_t));
    اگر (sockaddr == تہی) {
        ngx_resolver_free(r, dst);
        واپس تہی;
    }

    i = 0;
    d = rotate ? ngx_random() % n : 0;

    اگر (rn->naddrs) {
        j = rotate ? ngx_random() % rn->naddrs : 0;

        addr = (rn->naddrs == 1) ? &rn->u.addr : rn->u.addrs;

        کرو {
            جیب = &sockaddr[d].sockaddr_اندر;
            جیب->sin_family = AF_INET;
            جیب->sin_addr.s_addr = addr[j++];
            dst[d].sockaddr = (ساخت sockaddr *) جیب;
            dst[d++].socklen = حجم(ساخت sockaddr_اندر);

            اگر (d == n) {
                d = 0;
            }

            اگر (j == (ngx_uint_t) rn->naddrs) {
                j = 0;
            }
        } جب (++i < (ngx_uint_t) rn->naddrs);
    }

#اگر (NGX_HAVE_INET6)
    اگر (rn->naddrs6) {
        j = rotate ? ngx_random() % rn->naddrs6 : 0;

        addr6 = (rn->naddrs6 == 1) ? &rn->u6.addr6 : rn->u6.addrs6;

        کرو {
            sin6 = &sockaddr[d].sockaddr_in6;
            sin6->sin6_family = AF_INET6;
            ngx_memcpy(sin6->sin6_addr.s6_addr, addr6[j++].s6_addr, 16);
            dst[d].sockaddr = (ساخت sockaddr *) sin6;
            dst[d++].socklen = حجم(ساخت sockaddr_in6);

            اگر (d == n) {
                d = 0;
            }

            اگر (j == rn->naddrs6) {
                j = 0;
            }
        } جب (++i < n);
    }
#اگر_ختم

    واپس dst;
}


ساکن خالی
ngx_resolver_report_srv(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx)
{
    ngx_uint_t                naddrs, nsrvs, nw, i, j, k, l, m, n, w;
    ngx_resolver_addr_t      *addrs;
    ngx_resolver_srv_name_t  *srvs;

    srvs = ctx->srvs;
    nsrvs = ctx->nsrvs;

    naddrs = 0;

    برائے (i = 0; i < nsrvs; i++) {
        اگر (srvs[i].state == NGX_ERROR) {
            ctx->state = NGX_ERROR;
            ctx->valid = ngx_وقت() + (r->valid ? r->valid : 10);

            ctx->handler(ctx);
            واپس;
        }

        naddrs += srvs[i].naddrs;
    }

    اگر (naddrs == 0) {
        ctx->state = srvs[0].state;

        برائے (i = 0; i < nsrvs; i++) {
            اگر (srvs[i].state == NGX_RESOLVE_NXDOMAIN) {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                توڑو;
            }
        }

        ctx->valid = ngx_وقت() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        واپس;
    }

    addrs = ngx_resolver_calloc(r, naddrs * حجم(ngx_resolver_addr_t));
    اگر (addrs == تہی) {
        ctx->state = NGX_ERROR;
        ctx->valid = ngx_وقت() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        واپس;
    }

    i = 0;
    n = 0;

    کرو {
        nw = 0;

        برائے (j = i; j < nsrvs; j++) {
            اگر (srvs[j].ترجیح != srvs[i].ترجیح) {
                توڑو;
            }

            nw += srvs[j].naddrs * srvs[j].وزن;
        }

        اگر (nw == 0) {
            جاؤ اگلا_srv;
        }

        w = ngx_random() % nw;

        برائے (k = i; k < j; k++) {
            اگر (w < srvs[k].naddrs * srvs[k].وزن) {
                توڑو;
            }

            w -= srvs[k].naddrs * srvs[k].وزن;
        }

        برائے (l = i; l < j; l++) {

            برائے (m = 0; m < srvs[k].naddrs; m++) {
                addrs[n].socklen = srvs[k].addrs[m].socklen;
                addrs[n].sockaddr = srvs[k].addrs[m].sockaddr;
                addrs[n].name = srvs[k].name;
                addrs[n].ترجیح = srvs[k].ترجیح;
                addrs[n].وزن = srvs[k].وزن;
                n++;
            }

            اگر (++k == j) {
                k = i;
            }
        }

اگلا_srv:

        i = j;

    } جب (i < ctx->nsrvs);

    ctx->state = NGX_OK;
    ctx->addrs = addrs;
    ctx->naddrs = naddrs;

    ctx->handler(ctx);

    ngx_resolver_free(r, addrs);
}


حرف *
ngx_resolver_strerror(ngx_int_t err)
{
    ساکن حرف *errors[] = {
        "Format error",     /* FORMERR */
        "Server failure",   /* SERVFAIL */
        "Host not found",   /* NXDOMAIN */
        "Unimplemented",    /* NOTIMP */
        "Operation refused" /* REFUSED */
    };

    اگر (err > 0 && err < 6) {
        واپس errors[err - 1];
    }

    اگر (err == NGX_RESOLVE_TIMEDOUT) {
        واپس "Operation timed out";
    }

    واپس "Unknown error";
}


ساکن u_char *
ngx_resolver_log_error(ngx_log_t *لوگارتھم, u_char *buf, سائز_t len)
{
    u_char                     *p;
    ngx_resolver_connection_t  *rec;

    p = buf;

    اگر (لوگارتھم->action) {
        p = ngx_snprintf(buf, len, " while %s", لوگارتھم->action);
        len -= p - buf;
    }

    rec = لوگارتھم->ڈیٹا;

    اگر (rec) {
        p = ngx_snprintf(p, len, ", resolver: %V", &rec->server);
    }

    واپس p;
}


ساکن ngx_int_t
ngx_udp_connect(ngx_resolver_connection_t *rec)
{
    صحیح                rc;
    ngx_int_t          event;
    ngx_event_t       *rev, *wev;
    ngx_socket_t       s;
    ngx_connection_t  *c;

    s = ngx_socket(rec->sockaddr->sa_family, SOCK_DGRAM, 0);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->لوگارتھم, 0, "UDP socket %d", s);

    اگر (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->لوگارتھم, ngx_socket_errno,
                      ngx_socket_n " failed");
        واپس NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->لوگارتھم);

    اگر (c == تہی) {
        اگر (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->لوگارتھم, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        واپس NGX_ERROR;
    }

    اگر (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->لوگارتھم, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        جاؤ failed;
    }

    rev = c->اقرأ;
    wev = c->اكتب;

    rev->لوگارتھم = &rec->لوگارتھم;
    wev->لوگارتھم = &rec->لوگارتھم;

    rec->udp = c;

    c->نمبر = ngx_atomic_fetch_جوڑو(ngx_connection_شمار, 1);

    c->آغاز_وقت = ngx_current_msec;

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->لوگارتھم, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->نمبر);

    rc = connect(s, rec->sockaddr, rec->socklen);

    اگر (rc == -1) {
        ngx_log_error(NGX_LOG_CRIT, &rec->لوگارتھم, ngx_socket_errno,
                      "connect() failed");

        جاؤ failed;
    }

    /* UDP sockets are always ready to write */
    wev->ready = 1;

    event = (ngx_event_flags & NGX_USE_CLEAR_EVENT) ?
                /* kqueue, epoll */                 NGX_CLEAR_EVENT:
                /* select, poll, /dev/poll */       NGX_LEVEL_EVENT;
                /* eventport event type has no meaning: oneshot only */

    اگر (ngx_جوڑو_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        جاؤ failed;
    }

    واپس NGX_OK;

failed:

    ngx_close_connection(c);
    rec->udp = تہی;

    واپس NGX_ERROR;
}


ساکن ngx_int_t
ngx_tcp_connect(ngx_resolver_connection_t *rec)
{
    صحیح                rc;
    ngx_int_t          event;
    ngx_err_t          err;
    ngx_uint_t         level;
    ngx_socket_t       s;
    ngx_event_t       *rev, *wev;
    ngx_connection_t  *c;

    s = ngx_socket(rec->sockaddr->sa_family, SOCK_دھارا, 0);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->لوگارتھم, 0, "TCP socket %d", s);

    اگر (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->لوگارتھم, ngx_socket_errno,
                      ngx_socket_n " failed");
        واپس NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->لوگارتھم);

    اگر (c == تہی) {
        اگر (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->لوگارتھم, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        واپس NGX_ERROR;
    }

    اگر (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->لوگارتھم, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        جاؤ failed;
    }

    rev = c->اقرأ;
    wev = c->اكتب;

    rev->لوگارتھم = &rec->لوگارتھم;
    wev->لوگارتھم = &rec->لوگارتھم;

    rec->tcp = c;

    c->نمبر = ngx_atomic_fetch_جوڑو(ngx_connection_شمار, 1);

    c->آغاز_وقت = ngx_current_msec;

    اگر (ngx_جوڑو_conn) {
        اگر (ngx_جوڑو_conn(c) == NGX_ERROR) {
            جاؤ failed;
        }
    }

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->لوگارتھم, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->نمبر);

    rc = connect(s, rec->sockaddr, rec->socklen);

    اگر (rc == -1) {
        err = ngx_socket_errno;


        اگر (err != NGX_EINPROGRESS
#اگر (NGX_WIN32)
            /* Winsock returns WSAEWOULDBLOCK (NGX_EAGAIN) */
            && err != NGX_EAGAIN
#اگر_ختم
            )
        {
            اگر (err == NGX_ECONNREFUSED
#اگر (NGX_LINUX)
                /*
                 * Linux returns EAGAIN instead of ECONNREFUSED
                 * for unix sockets if listen queue is full
                 */
                || err == NGX_EAGAIN
#اگر_ختم
                || err == NGX_ECONNRESET
                || err == NGX_ENETDOWN
                || err == NGX_ENETUNREACH
                || err == NGX_EHOSTDOWN
                || err == NGX_EHOSTUNREACH)
            {
                level = NGX_LOG_ERR;

            } ورنہ {
                level = NGX_LOG_CRIT;
            }

            ngx_log_error(level, &rec->لوگارتھم, err, "connect() to %V failed",
                          &rec->server);

            ngx_close_connection(c);
            rec->tcp = تہی;

            واپس NGX_ERROR;
        }
    }

    اگر (ngx_جوڑو_conn) {
        اگر (rc == -1) {

            /* NGX_EINPROGRESS */

            واپس NGX_AGAIN;
        }

        ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->لوگارتھم, 0, "connected");

        wev->ready = 1;

        واپس NGX_OK;
    }

    اگر (ngx_event_flags & NGX_USE_CLEAR_EVENT) {

        /* kqueue */

        event = NGX_CLEAR_EVENT;

    } ورنہ {

        /* select, poll, /dev/poll */

        event = NGX_LEVEL_EVENT;
    }

    اگر (ngx_جوڑو_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        جاؤ failed;
    }

    اگر (rc == -1) {

        /* NGX_EINPROGRESS */

        اگر (ngx_جوڑو_event(wev, NGX_WRITE_EVENT, event) != NGX_OK) {
            جاؤ failed;
        }

        واپس NGX_AGAIN;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->لوگارتھم, 0, "connected");

    wev->ready = 1;

    واپس NGX_OK;

failed:

    ngx_close_connection(c);
    rec->tcp = تہی;

    واپس NGX_ERROR;
}


ساکن ngx_int_t
ngx_resolver_cmp_srvs(ثابت خالی *one, ثابت خالی *two)
{
    ngx_int_t            p1, p2;
    ngx_resolver_srv_t  *پہلا, *دوسرا;

    پہلا = (ngx_resolver_srv_t *) one;
    دوسرا = (ngx_resolver_srv_t *) two;

    p1 = پہلا->ترجیح;
    p2 = دوسرا->ترجیح;

    واپس p1 - p2;
}
