
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <ngx_event.h>


#تعریف_کن NGX_RESOLVER_UDP_اندازه   4096

#تعریف_کن NGX_RESOLVER_TCP_RSIZE  (2 + 65535)
#تعریف_کن NGX_RESOLVER_TCP_WSIZE  8192


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


#تعریف_کن ngx_resolver_گره(n)  ngx_rbtree_داده(n, ngx_resolver_گره_t, گره)


ساكن ngx_int_t ngx_udp_connect(ngx_resolver_connection_t *rec);
ساكن ngx_int_t ngx_tcp_connect(ngx_resolver_connection_t *rec);


ساكن فراغ ngx_resolver_cleanup(فراغ *داده);
ساكن فراغ ngx_resolver_cleanup_درخت(ngx_resolver_t *r, ngx_rbtree_t *درخت);
ساكن ngx_int_t ngx_resolve_name_locked(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx, ngx_str_t *name);
ساكن فراغ ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *درخت,
    ngx_صف_t *صف);
ساكن ngx_int_t ngx_resolver_send_query(ngx_resolver_t *r,
    ngx_resolver_گره_t *rn);
ساكن ngx_int_t ngx_resolver_send_udp_query(ngx_resolver_t *r,
    ngx_resolver_connection_t *rec, u_char *query, u_short qlen);
ساكن ngx_int_t ngx_resolver_send_tcp_query(ngx_resolver_t *r,
    ngx_resolver_connection_t *rec, u_char *query, u_short qlen);
ساكن ngx_int_t ngx_resolver_create_name_query(ngx_resolver_t *r,
    ngx_resolver_گره_t *rn, ngx_str_t *name);
ساكن ngx_int_t ngx_resolver_create_srv_query(ngx_resolver_t *r,
    ngx_resolver_گره_t *rn, ngx_str_t *name);
ساكن ngx_int_t ngx_resolver_create_addr_query(ngx_resolver_t *r,
    ngx_resolver_گره_t *rn, ngx_resolver_addr_t *addr);
ساكن فراغ ngx_resolver_resend_handler(ngx_event_t *ev);
ساكن زمان_t ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *درخت,
    ngx_صف_t *صف);
ساكن ngx_uint_t ngx_resolver_resend_خالی(ngx_resolver_t *r);
ساكن فراغ ngx_resolver_udp_read(ngx_event_t *rev);
ساكن فراغ ngx_resolver_tcp_write(ngx_event_t *wev);
ساكن فراغ ngx_resolver_tcp_read(ngx_event_t *rev);
ساكن فراغ ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf,
    اندازه_t n, ngx_uint_t tcp);
ساكن فراغ ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, اندازه_t n,
    ngx_uint_t ident, ngx_uint_t کد, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans);
ساكن فراغ ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, اندازه_t n,
    ngx_uint_t ident, ngx_uint_t کد, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans);
ساكن فراغ ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, اندازه_t n,
    ngx_uint_t ident, ngx_uint_t کد, ngx_uint_t nan);
ساكن ngx_resolver_گره_t *ngx_resolver_lookup_name(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
ساكن ngx_resolver_گره_t *ngx_resolver_lookup_srv(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
ساكن ngx_resolver_گره_t *ngx_resolver_lookup_addr(ngx_resolver_t *r,
    درون_addr_t addr);
ساكن فراغ ngx_resolver_rbtree_درج_مقدار(ngx_rbtree_گره_t *موقت,
    ngx_rbtree_گره_t *گره, ngx_rbtree_گره_t *sentinel);
ساكن ngx_int_t ngx_resolver_copy(ngx_resolver_t *r, ngx_str_t *name,
    u_char *buf, u_char *src, u_char *آخرین);
ساكن ngx_int_t ngx_resolver_مجموعه_timeout(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx);
ساكن فراغ ngx_resolver_timeout_handler(ngx_event_t *ev);
ساكن فراغ ngx_resolver_free_گره(ngx_resolver_t *r, ngx_resolver_گره_t *rn);
ساكن فراغ *ngx_resolver_alloc(ngx_resolver_t *r, اندازه_t اندازه);
ساكن فراغ *ngx_resolver_calloc(ngx_resolver_t *r, اندازه_t اندازه);
ساكن فراغ ngx_resolver_free(ngx_resolver_t *r, فراغ *p);
ساكن فراغ ngx_resolver_free_locked(ngx_resolver_t *r, فراغ *p);
ساكن فراغ *ngx_resolver_dup(ngx_resolver_t *r, فراغ *src, اندازه_t اندازه);
ساكن ngx_resolver_addr_t *ngx_resolver_export(ngx_resolver_t *r,
    ngx_resolver_گره_t *rn, ngx_uint_t rotate);
ساكن فراغ ngx_resolver_report_srv(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx);
ساكن u_char *ngx_resolver_log_error(ngx_log_t *لگاریتم, u_char *buf, اندازه_t len);
ساكن فراغ ngx_resolver_resolve_srv_نامها(ngx_resolver_ctx_t *ctx,
    ngx_resolver_گره_t *rn);
ساكن فراغ ngx_resolver_srv_نامها_handler(ngx_resolver_ctx_t *ctx);
ساكن ngx_int_t ngx_resolver_cmp_srvs(ثابت فراغ *one, ثابت فراغ *two);

#إذا (NGX_HAVE_INET6)
ساكن فراغ ngx_resolver_rbtree_درج_addr6_مقدار(ngx_rbtree_گره_t *موقت,
    ngx_rbtree_گره_t *گره, ngx_rbtree_گره_t *sentinel);
ساكن ngx_resolver_گره_t *ngx_resolver_lookup_addr6(ngx_resolver_t *r,
    بنية in6_addr *addr, uint32_t hash);
#پایان_اگر


ngx_resolver_t *
ngx_resolver_create(ngx_conf_t *cf, ngx_str_t *نامها, ngx_uint_t n)
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

    cln = ngx_pool_cleanup_جمع_کن(cf->pool, 0);
    إذا (cln == عدم) {
        أرجع عدم;
    }

    cln->handler = ngx_resolver_cleanup;
    cln->داده = r;

    r->ipv4 = 1;

    ngx_rbtree_init(&r->name_rbtree, &r->name_sentinel,
                    ngx_resolver_rbtree_درج_مقدار);

    ngx_rbtree_init(&r->srv_rbtree, &r->srv_sentinel,
                    ngx_resolver_rbtree_درج_مقدار);

    ngx_rbtree_init(&r->addr_rbtree, &r->addr_sentinel,
                    ngx_rbtree_درج_مقدار);

    ngx_صف_init(&r->name_resend_صف);
    ngx_صف_init(&r->srv_resend_صف);
    ngx_صف_init(&r->addr_resend_صف);

    ngx_صف_init(&r->name_expire_صف);
    ngx_صف_init(&r->srv_expire_صف);
    ngx_صف_init(&r->addr_expire_صف);

#إذا (NGX_HAVE_INET6)
    r->ipv6 = 1;

    ngx_rbtree_init(&r->addr6_rbtree, &r->addr6_sentinel,
                    ngx_resolver_rbtree_درج_addr6_مقدار);

    ngx_صف_init(&r->addr6_resend_صف);

    ngx_صف_init(&r->addr6_expire_صف);
#پایان_اگر

    r->event->handler = ngx_resolver_resend_handler;
    r->event->داده = r;
    r->event->لگاریتم = &cf->چرخه->new_log;
    r->event->cancelable = 1;
    r->ident = -1;

    r->resend_timeout = 5;
    r->tcp_timeout = 5;
    r->expire = 30;
    r->valid = 0;

    r->لگاریتم = &cf->چرخه->new_log;
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
        إذا (ngx_strncmp(نامها[i].داده, "valid=", 6) == 0) {
            s.len = نامها[i].len - 6;
            s.داده = نامها[i].داده + 6;

            r->valid = ngx_parse_زمان(&s, 1);

            إذا (r->valid == (زمان_t) NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &نامها[i]);
                أرجع عدم;
            }

            استمر;
        }

#إذا (NGX_HAVE_INET6)
        إذا (ngx_strncmp(نامها[i].داده, "ipv4=", 5) == 0) {

            إذا (ngx_strcmp(&نامها[i].داده[5], "on") == 0) {
                r->ipv4 = 1;

            } وإلا إذا (ngx_strcmp(&نامها[i].داده[5], "off") == 0) {
                r->ipv4 = 0;

            } وإلا {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &نامها[i]);
                أرجع عدم;
            }

            استمر;
        }

        إذا (ngx_strncmp(نامها[i].داده, "ipv6=", 5) == 0) {

            إذا (ngx_strcmp(&نامها[i].داده[5], "on") == 0) {
                r->ipv6 = 1;

            } وإلا إذا (ngx_strcmp(&نامها[i].داده[5], "off") == 0) {
                r->ipv6 = 0;

            } وإلا {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &نامها[i]);
                أرجع عدم;
            }

            استمر;
        }
#پایان_اگر

        ngx_memzero(&u, حجم(ngx_url_t));

        u.url = نامها[i];
        u.default_پورت = 53;

        إذا (ngx_parse_url(cf->pool, &u) != NGX_OK) {
            إذا (u.err) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "%s in resolver \"%V\"",
                                   u.err, &u.url);
            }

            أرجع عدم;
        }

        rec = ngx_array_افزودن_n(&r->connections, u.naddrs);
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
#پایان_اگر

    إذا (n && r->connections.nelts == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "no name servers defined");
        أرجع عدم;
    }

    أرجع r;
}


ساكن فراغ
ngx_resolver_cleanup(فراغ *داده)
{
    ngx_resolver_t  *r = داده;

    ngx_uint_t                  i;
    ngx_resolver_connection_t  *rec;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_چرخه->لگاریتم, 0, "cleanup resolver");

    ngx_resolver_cleanup_درخت(r, &r->name_rbtree);

    ngx_resolver_cleanup_درخت(r, &r->srv_rbtree);

    ngx_resolver_cleanup_درخت(r, &r->addr_rbtree);

#إذا (NGX_HAVE_INET6)
    ngx_resolver_cleanup_درخت(r, &r->addr6_rbtree);
#پایان_اگر

    إذا (r->event->timer_مجموعه) {
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
            ngx_resolver_free(r, rec[i].read_buf->شروع);
            ngx_resolver_free(r, rec[i].read_buf);
        }

        إذا (rec[i].write_buf) {
            ngx_resolver_free(r, rec[i].write_buf->شروع);
            ngx_resolver_free(r, rec[i].write_buf);
        }
    }
}


ساكن فراغ
ngx_resolver_cleanup_درخت(ngx_resolver_t *r, ngx_rbtree_t *درخت)
{
    ngx_resolver_ctx_t   *ctx, *بعدی;
    ngx_resolver_گره_t  *rn;

    طالما (درخت->ریشه != درخت->sentinel) {

        rn = ngx_resolver_گره(ngx_rbtree_min(درخت->ریشه, درخت->sentinel));

        ngx_صف_remove(&rn->صف);

        لكل (ctx = rn->انتظار; ctx; ctx = بعدی) {
            بعدی = ctx->بعدی;

            إذا (ctx->event) {
                إذا (ctx->event->timer_مجموعه) {
                    ngx_del_timer(ctx->event);
                }

                ngx_resolver_free(r, ctx->event);
            }

            ngx_resolver_free(r, ctx);
        }

        ngx_rbtree_delete(درخت, &rn->گره);

        ngx_resolver_free_گره(r, rn);
    }
}


ngx_resolver_ctx_t *
ngx_resolve_شروع(ngx_resolver_t *r, ngx_resolver_ctx_t *موقت)
{
    درون_addr_t            addr;
    ngx_resolver_ctx_t  *ctx;

    إذا (موقت) {
        addr = ngx_inet_addr(موقت->name.داده, موقت->name.len);

        إذا (addr != INADDR_NONE) {
            موقت->resolver = r;
            موقت->state = NGX_OK;
            موقت->naddrs = 1;
            موقت->addrs = &موقت->addr;
            موقت->addr.sockaddr = (بنية sockaddr *) &موقت->سینوس;
            موقت->addr.socklen = حجم(بنية sockaddr_درون);
            ngx_memzero(&موقت->سینوس, حجم(بنية sockaddr_درون));
            موقت->سینوس.sin_family = AF_INET;
            موقت->سینوس.sin_addr.s_addr = addr;
            موقت->سریع = 1;

            أرجع موقت;
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
    اندازه_t           slen;
    ngx_int_t        rc;
    ngx_str_t        name;
    ngx_resolver_t  *r;

    r = ctx->resolver;

    إذا (ctx->name.len > 0 && ctx->name.داده[ctx->name.len - 1] == '.') {
        ctx->name.len--;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لگاریتم, 0,
                   "resolve: \"%V\"", &ctx->name);

    إذا (ctx->سریع) {
        ctx->handler(ctx);
        أرجع NGX_OK;
    }

    إذا (ctx->service.len) {
        slen = ctx->service.len;

        إذا (ngx_strlchr(ctx->service.داده,
                        ctx->service.داده + ctx->service.len, '.')
            == عدم)
        {
            slen += حجم("_._tcp") - 1;
        }

        name.len = slen + 1 + ctx->name.len;

        name.داده = ngx_resolver_alloc(r, name.len);
        إذا (name.داده == عدم) {
            اذهب failed;
        }

        إذا (slen == ctx->service.len) {
            ngx_sprintf(name.داده, "%V.%V", &ctx->service, &ctx->name);

        } وإلا {
            ngx_sprintf(name.داده, "_%V._tcp.%V", &ctx->service, &ctx->name);
        }

        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &name);

        ngx_resolver_free(r, name.داده);

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
ngx_resolve_name_انجامشده(ngx_resolver_ctx_t *ctx)
{
    ngx_uint_t            i;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_گره_t  *rn;

    r = ctx->resolver;

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لگاریتم, 0,
                   "resolve name done: %i", ctx->state);

    إذا (ctx->سریع) {
        أرجع;
    }

    إذا (ctx->event && ctx->event->timer_مجموعه) {
        ngx_del_timer(ctx->event);
    }

    /* lock name mutex */

    إذا (ctx->nsrvs) {
        لكل (i = 0; i < ctx->nsrvs; i++) {
            إذا (ctx->srvs[i].ctx) {
                ngx_resolve_name_انجامشده(ctx->srvs[i].ctx);
            }

            إذا (ctx->srvs[i].addrs) {
                ngx_resolver_free(r, ctx->srvs[i].addrs->sockaddr);
                ngx_resolver_free(r, ctx->srvs[i].addrs);
            }

            ngx_resolver_free(r, ctx->srvs[i].name.داده);
        }

        ngx_resolver_free(r, ctx->srvs);
    }

    إذا (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->گره;

        إذا (rn) {
            p = &rn->انتظار;
            w = rn->انتظار;

            طالما (w) {
                إذا (w == ctx) {
                    *p = w->بعدی;

                    اذهب انجامشده;
                }

                p = &w->بعدی;
                w = w->بعدی;
            }

            ngx_log_error(NGX_LOG_ALERT, r->لگاریتم, 0,
                          "could not cancel %V resolving", &ctx->name);
        }
    }

انجامشده:

    إذا (ctx->service.len) {
        ngx_resolver_expire(r, &r->srv_rbtree, &r->srv_expire_صف);

    } وإلا {
        ngx_resolver_expire(r, &r->name_rbtree, &r->name_expire_صف);
    }

    /* unlock name mutex */

    /* lock alloc mutex */

    إذا (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    إذا (r->event->timer_مجموعه && ngx_resolver_resend_خالی(r)) {
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
    ngx_صف_t          *resend_صف, *expire_صف;
    ngx_rbtree_t         *درخت;
    ngx_resolver_ctx_t   *بعدی, *آخرین;
    ngx_resolver_addr_t  *addrs;
    ngx_resolver_گره_t  *rn;

    ngx_strlow(name->داده, name->داده, name->len);

    hash = ngx_بررسی_چرخهای_32_short(name->داده, name->len);

    إذا (ctx->service.len) {
        rn = ngx_resolver_lookup_srv(r, name, hash);

        درخت = &r->srv_rbtree;
        resend_صف = &r->srv_resend_صف;
        expire_صف = &r->srv_expire_صف;

    } وإلا {
        rn = ngx_resolver_lookup_name(r, name, hash);

        درخت = &r->name_rbtree;
        resend_صف = &r->name_resend_صف;
        expire_صف = &r->name_expire_صف;
    }

    إذا (rn) {

        /* ctx can be a list after NGX_RESOLVE_CNAME */
        لكل (آخرین = ctx; آخرین->بعدی; آخرین = آخرین->بعدی);

        إذا (rn->valid >= ngx_زمان()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->لگاریتم, 0, "resolve cached");

            ngx_صف_remove(&rn->صف);

            rn->expire = ngx_زمان() + r->expire;

            ngx_صف_درج_سر(expire_صف, &rn->صف);

            naddrs = (rn->naddrs == (u_short) -1) ? 0 : rn->naddrs;
#إذا (NGX_HAVE_INET6)
            naddrs += (rn->naddrs6 == (u_short) -1) ? 0 : rn->naddrs6;
#پایان_اگر

            إذا (naddrs) {

                إذا (naddrs == 1 && rn->naddrs == 1) {
                    addrs = عدم;

                } وإلا {
                    addrs = ngx_resolver_export(r, rn, 1);
                    إذا (addrs == عدم) {
                        أرجع NGX_ERROR;
                    }
                }

                آخرین->بعدی = rn->انتظار;
                rn->انتظار = عدم;

                /* unlock name mutex */

                افعل {
                    ctx->state = NGX_OK;
                    ctx->valid = rn->valid;
                    ctx->naddrs = naddrs;

                    إذا (addrs == عدم) {
                        ctx->addrs = &ctx->addr;
                        ctx->addr.sockaddr = (بنية sockaddr *) &ctx->سینوس;
                        ctx->addr.socklen = حجم(بنية sockaddr_درون);
                        ngx_memzero(&ctx->سینوس, حجم(بنية sockaddr_درون));
                        ctx->سینوس.sin_family = AF_INET;
                        ctx->سینوس.sin_addr.s_addr = rn->u.addr;

                    } وإلا {
                        ctx->addrs = addrs;
                    }

                    بعدی = ctx->بعدی;

                    ctx->handler(ctx);

                    ctx = بعدی;
                } طالما (ctx);

                إذا (addrs != عدم) {
                    ngx_resolver_free(r, addrs->sockaddr);
                    ngx_resolver_free(r, addrs);
                }

                أرجع NGX_OK;
            }

            إذا (rn->nsrvs) {
                آخرین->بعدی = rn->انتظار;
                rn->انتظار = عدم;

                /* unlock name mutex */

                افعل {
                    بعدی = ctx->بعدی;

                    ngx_resolver_resolve_srv_نامها(ctx, rn);

                    ctx = بعدی;
                } طالما (ctx);

                أرجع NGX_OK;
            }

            /* NGX_RESOLVE_CNAME */

            إذا (ctx->recursion++ < NGX_RESOLVER_MAX_RECURSION) {

                cname.len = rn->cnlen;
                cname.داده = rn->u.cname;

                أرجع ngx_resolve_name_locked(r, ctx, &cname);
            }

            آخرین->بعدی = rn->انتظار;
            rn->انتظار = عدم;

            /* unlock name mutex */

            افعل {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                ctx->valid = ngx_زمان() + (r->valid ? r->valid : 10);
                بعدی = ctx->بعدی;

                ctx->handler(ctx);

                ctx = بعدی;
            } طالما (ctx);

            أرجع NGX_OK;
        }

        إذا (rn->انتظار) {
            إذا (ngx_resolver_مجموعه_timeout(r, ctx) != NGX_OK) {
                أرجع NGX_ERROR;
            }

            آخرین->بعدی = rn->انتظار;
            rn->انتظار = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;

            افعل {
                ctx->گره = rn;
                ctx = ctx->بعدی;
            } طالما (ctx);

            أرجع NGX_AGAIN;
        }

        ngx_صف_remove(&rn->صف);

        /* lock alloc mutex */

        إذا (rn->query) {
            ngx_resolver_free_locked(r, rn->query);
            rn->query = عدم;
#إذا (NGX_HAVE_INET6)
            rn->query6 = عدم;
#پایان_اگر
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
#پایان_اگر

        إذا (rn->nsrvs) {
            لكل (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
                إذا (rn->u.srvs[i].name.داده) {
                    ngx_resolver_free_locked(r, rn->u.srvs[i].name.داده);
                }
            }

            ngx_resolver_free_locked(r, rn->u.srvs);
        }

        /* unlock alloc mutex */

    } وإلا {

        rn = ngx_resolver_alloc(r, حجم(ngx_resolver_گره_t));
        إذا (rn == عدم) {
            أرجع NGX_ERROR;
        }

        rn->name = ngx_resolver_dup(r, name->داده, name->len);
        إذا (rn->name == عدم) {
            ngx_resolver_free(r, rn);
            أرجع NGX_ERROR;
        }

        rn->گره.کلید = hash;
        rn->nlen = (u_short) name->len;
        rn->query = عدم;
#إذا (NGX_HAVE_INET6)
        rn->query6 = عدم;
#پایان_اگر

        ngx_rbtree_درج(درخت, &rn->گره);
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
        ngx_rbtree_delete(درخت, &rn->گره);

        ngx_resolver_free(r, rn->query);
        ngx_resolver_free(r, rn->name);
        ngx_resolver_free(r, rn);

        افعل {
            ctx->state = NGX_RESOLVE_NXDOMAIN;
            بعدی = ctx->بعدی;

            ctx->handler(ctx);

            ctx = بعدی;
        } طالما (ctx);

        أرجع NGX_OK;
    }

    rn->آخرین_connection = r->آخرین_connection++;
    إذا (r->آخرین_connection == r->connections.nelts) {
        r->آخرین_connection = 0;
    }

    rn->naddrs = r->ipv4 ? (u_short) -1 : 0;
    rn->tcp = 0;
#إذا (NGX_HAVE_INET6)
    rn->naddrs6 = r->ipv6 ? (u_short) -1 : 0;
    rn->tcp6 = 0;
#پایان_اگر
    rn->nsrvs = 0;

    إذا (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->آخرین_connection++;
        إذا (rn->آخرین_connection == r->connections.nelts) {
            rn->آخرین_connection = 0;
        }

        (فراغ) ngx_resolver_send_query(r, rn);
    }

    إذا (ngx_resolver_مجموعه_timeout(r, ctx) != NGX_OK) {
        اذهب failed;
    }

    إذا (ngx_resolver_resend_خالی(r)) {
        ngx_جمع_کن_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_زمان() + r->resend_timeout;

    ngx_صف_درج_سر(resend_صف, &rn->صف);

    rn->کد = 0;
    rn->cnlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_مقدار;
    rn->انتظار = ctx;

    ctx->state = NGX_AGAIN;
    ctx->async = 1;

    افعل {
        ctx->گره = rn;
        ctx = ctx->بعدی;
    } طالما (ctx);

    أرجع NGX_AGAIN;

failed:

    ngx_rbtree_delete(درخت, &rn->گره);

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
    درون_addr_t             addr;
    ngx_صف_t          *resend_صف, *expire_صف;
    ngx_rbtree_t         *درخت;
    ngx_resolver_t       *r;
    بنية sockaddr_درون   *سینوس;
    ngx_resolver_گره_t  *rn;
#إذا (NGX_HAVE_INET6)
    uint32_t              hash;
    بنية sockaddr_in6  *sin6;
#پایان_اگر

#إذا (NGX_SUPPRESS_WARN)
    addr = 0;
#إذا (NGX_HAVE_INET6)
    hash = 0;
    sin6 = عدم;
#پایان_اگر
#پایان_اگر

    r = ctx->resolver;

    بدل (ctx->addr.sockaddr->sa_family) {

#إذا (NGX_HAVE_INET6)
    حالة AF_INET6:
        sin6 = (بنية sockaddr_in6 *) ctx->addr.sockaddr;
        hash = ngx_بررسی_چرخهای_32_short(sin6->sin6_addr.s6_addr, 16);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr6(r, &sin6->sin6_addr, hash);

        درخت = &r->addr6_rbtree;
        resend_صف = &r->addr6_resend_صف;
        expire_صف = &r->addr6_expire_صف;

        اكسر;
#پایان_اگر

    افتراضي: /* AF_INET */
        سینوس = (بنية sockaddr_درون *) ctx->addr.sockaddr;
        addr = ntohl(سینوس->sin_addr.s_addr);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        درخت = &r->addr_rbtree;
        resend_صف = &r->addr_resend_صف;
        expire_صف = &r->addr_expire_صف;
    }

    إذا (rn) {

        إذا (rn->valid >= ngx_زمان()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->لگاریتم, 0, "resolve cached");

            ngx_صف_remove(&rn->صف);

            rn->expire = ngx_زمان() + r->expire;

            ngx_صف_درج_سر(expire_صف, &rn->صف);

            name = ngx_resolver_dup(r, rn->name, rn->nlen);
            إذا (name == عدم) {
                ngx_resolver_free(r, ctx);
                أرجع NGX_ERROR;
            }

            ctx->name.len = rn->nlen;
            ctx->name.داده = name;

            /* unlock addr mutex */

            ctx->state = NGX_OK;
            ctx->valid = rn->valid;

            ctx->handler(ctx);

            ngx_resolver_free(r, name);

            أرجع NGX_OK;
        }

        إذا (rn->انتظار) {
            إذا (ngx_resolver_مجموعه_timeout(r, ctx) != NGX_OK) {
                أرجع NGX_ERROR;
            }

            ctx->بعدی = rn->انتظار;
            rn->انتظار = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;
            ctx->گره = rn;

            /* unlock addr mutex */

            أرجع NGX_OK;
        }

        ngx_صف_remove(&rn->صف);

        ngx_resolver_free(r, rn->query);
        rn->query = عدم;
#إذا (NGX_HAVE_INET6)
        rn->query6 = عدم;
#پایان_اگر

    } وإلا {
        rn = ngx_resolver_alloc(r, حجم(ngx_resolver_گره_t));
        إذا (rn == عدم) {
            اذهب failed;
        }

        بدل (ctx->addr.sockaddr->sa_family) {

#إذا (NGX_HAVE_INET6)
        حالة AF_INET6:
            rn->addr6 = sin6->sin6_addr;
            rn->گره.کلید = hash;
            اكسر;
#پایان_اگر

        افتراضي: /* AF_INET */
            rn->گره.کلید = addr;
        }

        rn->query = عدم;
#إذا (NGX_HAVE_INET6)
        rn->query6 = عدم;
#پایان_اگر

        ngx_rbtree_درج(درخت, &rn->گره);
    }

    إذا (ngx_resolver_create_addr_query(r, rn, &ctx->addr) != NGX_OK) {
        اذهب failed;
    }

    rn->آخرین_connection = r->آخرین_connection++;
    إذا (r->آخرین_connection == r->connections.nelts) {
        r->آخرین_connection = 0;
    }

    rn->naddrs = (u_short) -1;
    rn->tcp = 0;
#إذا (NGX_HAVE_INET6)
    rn->naddrs6 = (u_short) -1;
    rn->tcp6 = 0;
#پایان_اگر
    rn->nsrvs = 0;

    إذا (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->آخرین_connection++;
        إذا (rn->آخرین_connection == r->connections.nelts) {
            rn->آخرین_connection = 0;
        }

        (فراغ) ngx_resolver_send_query(r, rn);
    }

    إذا (ngx_resolver_مجموعه_timeout(r, ctx) != NGX_OK) {
        اذهب failed;
    }

    إذا (ngx_resolver_resend_خالی(r)) {
        ngx_جمع_کن_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_زمان() + r->resend_timeout;

    ngx_صف_درج_سر(resend_صف, &rn->صف);

    rn->کد = 0;
    rn->cnlen = 0;
    rn->name = عدم;
    rn->nlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_مقدار;
    rn->انتظار = ctx;

    /* unlock addr mutex */

    ctx->state = NGX_AGAIN;
    ctx->async = 1;
    ctx->گره = rn;

    أرجع NGX_OK;

failed:

    إذا (rn) {
        ngx_rbtree_delete(درخت, &rn->گره);

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
ngx_resolve_addr_انجامشده(ngx_resolver_ctx_t *ctx)
{
    ngx_صف_t          *expire_صف;
    ngx_rbtree_t         *درخت;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_گره_t  *rn;

    r = ctx->resolver;

    بدل (ctx->addr.sockaddr->sa_family) {

#إذا (NGX_HAVE_INET6)
    حالة AF_INET6:
        درخت = &r->addr6_rbtree;
        expire_صف = &r->addr6_expire_صف;
        اكسر;
#پایان_اگر

    افتراضي: /* AF_INET */
        درخت = &r->addr_rbtree;
        expire_صف = &r->addr_expire_صف;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لگاریتم, 0,
                   "resolve addr done: %i", ctx->state);

    إذا (ctx->event && ctx->event->timer_مجموعه) {
        ngx_del_timer(ctx->event);
    }

    /* lock addr mutex */

    إذا (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->گره;

        إذا (rn) {
            p = &rn->انتظار;
            w = rn->انتظار;

            طالما (w) {
                إذا (w == ctx) {
                    *p = w->بعدی;

                    اذهب انجامشده;
                }

                p = &w->بعدی;
                w = w->بعدی;
            }
        }

        {
            u_char     متن[NGX_SOCKADDR_STRLEN];
            ngx_str_t  addrtext;

            addrtext.داده = متن;
            addrtext.len = ngx_sock_ntop(ctx->addr.sockaddr, ctx->addr.socklen,
                                         متن, NGX_SOCKADDR_STRLEN, 0);

            ngx_log_error(NGX_LOG_ALERT, r->لگاریتم, 0,
                          "could not cancel %V resolving", &addrtext);
        }
    }

انجامشده:

    ngx_resolver_expire(r, درخت, expire_صف);

    /* unlock addr mutex */

    /* lock alloc mutex */

    إذا (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    إذا (r->event->timer_مجموعه && ngx_resolver_resend_خالی(r)) {
        ngx_del_timer(r->event);
    }
}


ساكن فراغ
ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *درخت, ngx_صف_t *صف)
{
    زمان_t                now;
    ngx_uint_t            i;
    ngx_صف_t          *q;
    ngx_resolver_گره_t  *rn;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->لگاریتم, 0, "resolver expire");

    now = ngx_زمان();

    لكل (i = 0; i < 2; i++) {
        إذا (ngx_صف_خالی(صف)) {
            أرجع;
        }

        q = ngx_صف_آخرین(صف);

        rn = ngx_صف_داده(q, ngx_resolver_گره_t, صف);

        إذا (now <= rn->expire) {
            أرجع;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->لگاریتم, 0,
                       "resolver expire \"%*s\"", (اندازه_t) rn->nlen, rn->name);

        ngx_صف_remove(q);

        ngx_rbtree_delete(درخت, &rn->گره);

        ngx_resolver_free_گره(r, rn);
    }
}


ساكن ngx_int_t
ngx_resolver_send_query(ngx_resolver_t *r, ngx_resolver_گره_t *rn)
{
    ngx_int_t                   rc;
    ngx_resolver_connection_t  *rec;

    rec = r->connections.elts;
    rec = &rec[rn->آخرین_connection];

    إذا (rec->لگاریتم.handler == عدم) {
        rec->لگاریتم = *r->لگاریتم;
        rec->لگاریتم.handler = ngx_resolver_log_error;
        rec->لگاریتم.داده = rec;
        rec->لگاریتم.action = "resolving";
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

#پایان_اگر

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

        rec->udp->داده = rec;
        rec->udp->اقرأ->handler = ngx_resolver_udp_read;
        rec->udp->اقرأ->resolver = 1;
    }

    n = ngx_send(rec->udp, query, qlen);

    إذا (n == NGX_ERROR) {
        اذهب failed;
    }

    إذا ((اندازه_t) n != (اندازه_t) qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->لگاریتم, 0, "send() incomplete");
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

            b->شروع = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_RSIZE);
            إذا (b->شروع == عدم) {
                ngx_resolver_free(r, b);
                أرجع NGX_ERROR;
            }

            b->انتها = b->شروع + NGX_RESOLVER_TCP_RSIZE;

            rec->read_buf = b;
        }

        b->pos = b->شروع;
        b->آخرین = b->شروع;

        b = rec->write_buf;

        إذا (b == عدم) {
            b = ngx_resolver_calloc(r, حجم(ngx_buf_t));
            إذا (b == عدم) {
                أرجع NGX_ERROR;
            }

            b->شروع = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_WSIZE);
            إذا (b->شروع == عدم) {
                ngx_resolver_free(r, b);
                أرجع NGX_ERROR;
            }

            b->انتها = b->شروع + NGX_RESOLVER_TCP_WSIZE;

            rec->write_buf = b;
        }

        b->pos = b->شروع;
        b->آخرین = b->شروع;

        rc = ngx_tcp_connect(rec);
        إذا (rc == NGX_ERROR) {
            أرجع NGX_ERROR;
        }

        rec->tcp->داده = rec;
        rec->tcp->اكتب->handler = ngx_resolver_tcp_write;
        rec->tcp->اكتب->cancelable = 1;
        rec->tcp->اقرأ->handler = ngx_resolver_tcp_read;
        rec->tcp->اقرأ->resolver = 1;

        ngx_جمع_کن_timer(rec->tcp->اكتب, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    b = rec->write_buf;

    إذا (b->انتها - b->آخرین <  2 + qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->لگاریتم, 0, "buffer overflow");
        أرجع NGX_ERROR;
    }

    *b->آخرین++ = (u_char) (qlen >> 8);
    *b->آخرین++ = (u_char) qlen;
    b->آخرین = ngx_cpymem(b->آخرین, query, qlen);

    إذا (rc == NGX_OK) {
        ngx_resolver_tcp_write(rec->tcp->اكتب);
    }

    أرجع NGX_OK;
}


ساكن فراغ
ngx_resolver_resend_handler(ngx_event_t *ev)
{
    زمان_t           timer, atimer, stimer, ntimer;
#إذا (NGX_HAVE_INET6)
    زمان_t           a6timer;
#پایان_اگر
    ngx_resolver_t  *r;

    r = ev->داده;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->لگاریتم, 0,
                   "resolver resend handler");

    /* lock name mutex */

    ntimer = ngx_resolver_resend(r, &r->name_rbtree, &r->name_resend_صف);

    stimer = ngx_resolver_resend(r, &r->srv_rbtree, &r->srv_resend_صف);

    /* unlock name mutex */

    /* lock addr mutex */

    atimer = ngx_resolver_resend(r, &r->addr_rbtree, &r->addr_resend_صف);

    /* unlock addr mutex */

#إذا (NGX_HAVE_INET6)

    /* lock addr6 mutex */

    a6timer = ngx_resolver_resend(r, &r->addr6_rbtree, &r->addr6_resend_صف);

    /* unlock addr6 mutex */

#پایان_اگر

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

#پایان_اگر

    إذا (timer) {
        ngx_جمع_کن_timer(r->event, (ngx_msec_t) (timer * 1000));
    }
}


ساكن زمان_t
ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *درخت, ngx_صف_t *صف)
{
    زمان_t                now;
    ngx_صف_t          *q;
    ngx_resolver_گره_t  *rn;

    now = ngx_زمان();

    لكل ( ;; ) {
        إذا (ngx_صف_خالی(صف)) {
            أرجع 0;
        }

        q = ngx_صف_آخرین(صف);

        rn = ngx_صف_داده(q, ngx_resolver_گره_t, صف);

        إذا (now < rn->expire) {
            أرجع rn->expire - now;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->لگاریتم, 0,
                       "resolver resend \"%*s\" %p",
                       (اندازه_t) rn->nlen, rn->name, rn->انتظار);

        ngx_صف_remove(q);

        إذا (rn->انتظار) {

            إذا (++rn->آخرین_connection == r->connections.nelts) {
                rn->آخرین_connection = 0;
            }

            (فراغ) ngx_resolver_send_query(r, rn);

            rn->expire = now + r->resend_timeout;

            ngx_صف_درج_سر(صف, q);

            استمر;
        }

        ngx_rbtree_delete(درخت, &rn->گره);

        ngx_resolver_free_گره(r, rn);
    }
}


ساكن ngx_uint_t
ngx_resolver_resend_خالی(ngx_resolver_t *r)
{
    أرجع ngx_صف_خالی(&r->name_resend_صف)
           && ngx_صف_خالی(&r->srv_resend_صف)
#إذا (NGX_HAVE_INET6)
           && ngx_صف_خالی(&r->addr6_resend_صف)
#پایان_اگر
           && ngx_صف_خالی(&r->addr_resend_صف);
}


ساكن فراغ
ngx_resolver_udp_read(ngx_event_t *rev)
{
    ssize_t                     n;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;
    u_char                      buf[NGX_RESOLVER_UDP_اندازه];

    c = rev->داده;
    rec = c->داده;

    افعل {
        n = ngx_udp_recv(c, buf, NGX_RESOLVER_UDP_اندازه);

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
    off_t                       فرستاده;
    ssize_t                     n;
    ngx_buf_t                  *b;
    ngx_resolver_t             *r;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;

    c = wev->داده;
    rec = c->داده;
    b = rec->write_buf;
    r = rec->resolver;

    إذا (wev->timedout) {
        اذهب failed;
    }

    فرستاده = c->فرستاده;

    طالما (wev->ready && b->pos < b->آخرین) {
        n = ngx_send(c, b->pos, b->آخرین - b->pos);

        إذا (n == NGX_AGAIN) {
            اكسر;
        }

        إذا (n == NGX_ERROR) {
            اذهب failed;
        }

        b->pos += n;
    }

    إذا (b->pos != b->شروع) {
        b->آخرین = ngx_movemem(b->شروع, b->pos, b->آخرین - b->pos);
        b->pos = b->شروع;
    }

    إذا (c->فرستاده != فرستاده) {
        ngx_جمع_کن_timer(wev, (ngx_msec_t) (r->tcp_timeout * 1000));
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
    اندازه_t                      اندازه;
    ssize_t                     n;
    u_short                     qlen;
    ngx_buf_t                  *b;
    ngx_resolver_t             *r;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;

    c = rev->داده;
    rec = c->داده;
    b = rec->read_buf;
    r = rec->resolver;

    طالما (rev->ready) {
        n = ngx_recv(c, b->آخرین, b->انتها - b->آخرین);

        إذا (n == NGX_AGAIN) {
            اكسر;
        }

        إذا (n == NGX_ERROR || n == 0) {
            اذهب failed;
        }

        b->آخرین += n;

        لكل ( ;; ) {
            p = b->pos;
            اندازه = b->آخرین - p;

            إذا (اندازه < 2) {
                اكسر;
            }

            qlen = (u_short) *p++ << 8;
            qlen += *p++;

            إذا (اندازه < (اندازه_t) (2 + qlen)) {
                اكسر;
            }

            ngx_resolver_process_response(r, p, qlen, 1);

            b->pos += 2 + qlen;
        }

        إذا (b->pos != b->شروع) {
            b->آخرین = ngx_movemem(b->شروع, b->pos, b->آخرین - b->pos);
            b->pos = b->شروع;
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
ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf, اندازه_t n,
    ngx_uint_t tcp)
{
    حرف                 *err;
    ngx_uint_t            i, times, ident, qident, flags, کد, nqs, nan, trunc,
                          qtype, qclass;
#إذا (NGX_HAVE_INET6)
    ngx_uint_t            qident6;
#پایان_اگر
    ngx_صف_t          *q;
    ngx_resolver_qs_t    *qs;
    ngx_resolver_hdr_t   *response;
    ngx_resolver_گره_t  *rn;

    إذا (n < حجم(ngx_resolver_hdr_t)) {
        اذهب short_response;
    }

    response = (ngx_resolver_hdr_t *) buf;

    ident = (response->ident_hi << 8) + response->ident_lo;
    flags = (response->flags_hi << 8) + response->flags_lo;
    nqs = (response->nqs_hi << 8) + response->nqs_lo;
    nan = (response->nan_hi << 8) + response->nan_lo;
    trunc = flags & 0x0200;

    ngx_log_debug6(NGX_LOG_DEBUG_CORE, r->لگاریتم, 0,
                   "resolver DNS response %ui fl:%04Xi %ui/%ui/%ud/%ud",
                   ident, flags, nqs, nan,
                   (response->nns_hi << 8) + response->nns_lo,
                   (response->nar_hi << 8) + response->nar_lo);

    /* response to a standard query */
    إذا ((flags & 0xf870) != 0x8000 || (trunc && tcp)) {
        ngx_log_error(r->log_level, r->لگاریتم, 0,
                      "invalid %s DNS response %ui fl:%04Xi",
                      tcp ? "TCP" : "UDP", ident, flags);
        أرجع;
    }

    کد = flags & 0xf;

    إذا (کد == NGX_RESOLVE_FORMERR) {

        times = 0;

        لكل (q = ngx_صف_سر(&r->name_resend_صف);
             q != ngx_صف_sentinel(&r->name_resend_صف) && times++ < 100;
             q = ngx_صف_بعدی(q))
        {
            rn = ngx_صف_داده(q, ngx_resolver_گره_t, صف);

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
#پایان_اگر
        }

        اذهب dns_error;
    }

    إذا (کد > NGX_RESOLVE_REFUSED) {
        اذهب dns_error;
    }

    إذا (nqs != 1) {
        err = "invalid number of questions in DNS response";
        اذهب انجامشده;
    }

    i = حجم(ngx_resolver_hdr_t);

    طالما (i < (ngx_uint_t) n) {

        إذا (buf[i] & 0xc0) {
            err = "unexpected compression pointer in DNS response";
            اذهب انجامشده;
        }

        إذا (buf[i] == '\0') {
            اذهب یافت;
        }

        i += 1 + buf[i];
    }

    اذهب short_response;

یافت:

    إذا (i++ == حجم(ngx_resolver_hdr_t)) {
        err = "zero-length domain name in DNS response";
        اذهب انجامشده;
    }

    إذا (i + حجم(ngx_resolver_qs_t) + nan * (2 + حجم(ngx_resolver_an_t))
        > (ngx_uint_t) n)
    {
        اذهب short_response;
    }

    qs = (ngx_resolver_qs_t *) &buf[i];

    qtype = (qs->type_hi << 8) + qs->type_lo;
    qclass = (qs->class_hi << 8) + qs->class_lo;

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->لگاریتم, 0,
                   "resolver DNS response qt:%ui cl:%ui", qtype, qclass);

    إذا (qclass != 1) {
        ngx_log_error(r->log_level, r->لگاریتم, 0,
                      "unknown query class %ui in DNS response", qclass);
        أرجع;
    }

    بدل (qtype) {

    حالة NGX_RESOLVE_A:
#إذا (NGX_HAVE_INET6)
    حالة NGX_RESOLVE_AAAA:
#پایان_اگر

        ngx_resolver_process_a(r, buf, n, ident, کد, qtype, nan, trunc,
                               i + حجم(ngx_resolver_qs_t));

        اكسر;

    حالة NGX_RESOLVE_SRV:

        ngx_resolver_process_srv(r, buf, n, ident, کد, nan, trunc,
                                 i + حجم(ngx_resolver_qs_t));

        اكسر;

    حالة NGX_RESOLVE_PTR:

        ngx_resolver_process_ptr(r, buf, n, ident, کد, nan);

        اكسر;

    افتراضي:
        ngx_log_error(r->log_level, r->لگاریتم, 0,
                      "unknown query type %ui in DNS response", qtype);
        أرجع;
    }

    أرجع;

short_response:

    err = "short DNS response";

انجامشده:

    ngx_log_error(r->log_level, r->لگاریتم, 0, err);

    أرجع;

dns_error_name:

    ngx_log_error(r->log_level, r->لگاریتم, 0,
                  "DNS error (%ui: %s), query id:%ui, name:\"%*s\"",
                  کد, ngx_resolver_strerror(کد), ident,
                  (اندازه_t) rn->nlen, rn->name);
    أرجع;

dns_error:

    ngx_log_error(r->log_level, r->لگاریتم, 0,
                  "DNS error (%ui: %s), query id:%ui",
                  کد, ngx_resolver_strerror(کد), ident);
    أرجع;
}


ساكن فراغ
ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, اندازه_t n,
    ngx_uint_t ident, ngx_uint_t کد, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans)
{
    حرف                       *err;
    u_char                     *cname;
    اندازه_t                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    درون_addr_t                  *addr;
    ngx_str_t                   name;
    ngx_uint_t                  type, class, qident, naddrs, a, i, j, شروع;
#إذا (NGX_HAVE_INET6)
    بنية in6_addr            *addr6;
#پایان_اگر
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *بعدی;
    ngx_resolver_گره_t        *rn;
    ngx_resolver_addr_t        *addrs;
    ngx_resolver_connection_t  *rec;

    إذا (ngx_resolver_copy(r, &name, buf,
                          buf + حجم(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        أرجع;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لگاریتم, 0, "resolver qs:%V", &name);

    hash = ngx_بررسی_چرخهای_32_short(name.داده, name.len);

    /* lock name mutex */

    rn = ngx_resolver_lookup_name(r, &name, hash);

    إذا (rn == عدم) {
        ngx_log_error(r->log_level, r->لگاریتم, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.داده);
        اذهب failed;
    }

    بدل (qtype) {

#إذا (NGX_HAVE_INET6)
    حالة NGX_RESOLVE_AAAA:

        إذا (rn->query6 == عدم || rn->naddrs6 != (u_short) -1) {
            ngx_log_error(r->log_level, r->لگاریتم, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.داده);
            اذهب failed;
        }

        إذا (trunc && rn->tcp6) {
            ngx_resolver_free(r, name.داده);
            اذهب failed;
        }

        qident = (rn->query6[0] << 8) + rn->query6[1];

        اكسر;
#پایان_اگر

    افتراضي: /* NGX_RESOLVE_A */

        إذا (rn->query == عدم || rn->naddrs != (u_short) -1) {
            ngx_log_error(r->log_level, r->لگاریتم, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.داده);
            اذهب failed;
        }

        إذا (trunc && rn->tcp) {
            ngx_resolver_free(r, name.داده);
            اذهب failed;
        }

        qident = (rn->query[0] << 8) + rn->query[1];
    }

    إذا (ident != qident) {
        ngx_log_error(r->log_level, r->لگاریتم, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.داده);
        اذهب failed;
    }

    ngx_resolver_free(r, name.داده);

    إذا (trunc) {

        ngx_صف_remove(&rn->صف);

        إذا (rn->انتظار == عدم) {
            ngx_rbtree_delete(&r->name_rbtree, &rn->گره);
            ngx_resolver_free_گره(r, rn);
            اذهب بعدی;
        }

        rec = r->connections.elts;
        rec = &rec[rn->آخرین_connection];

        بدل (qtype) {

#إذا (NGX_HAVE_INET6)
        حالة NGX_RESOLVE_AAAA:

            rn->tcp6 = 1;

            (فراغ) ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen);

            اكسر;
#پایان_اگر

        افتراضي: /* NGX_RESOLVE_A */

            rn->tcp = 1;

            (فراغ) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);
        }

        rn->expire = ngx_زمان() + r->resend_timeout;

        ngx_صف_درج_سر(&r->name_resend_صف, &rn->صف);

        اذهب بعدی;
    }

    إذا (کد == 0 && rn->کد) {
        کد = rn->کد;
    }

    إذا (کد == 0 && nan == 0) {

#إذا (NGX_HAVE_INET6)
        بدل (qtype) {

        حالة NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            إذا (rn->naddrs == (u_short) -1) {
                اذهب بعدی;
            }

            إذا (rn->naddrs) {
                اذهب export;
            }

            اكسر;

        افتراضي: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            إذا (rn->naddrs6 == (u_short) -1) {
                اذهب بعدی;
            }

            إذا (rn->naddrs6) {
                اذهب export;
            }
        }
#پایان_اگر

        کد = NGX_RESOLVE_NXDOMAIN;
    }

    إذا (کد) {

#إذا (NGX_HAVE_INET6)
        بدل (qtype) {

        حالة NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            إذا (rn->naddrs == (u_short) -1) {
                rn->کد = (u_char) کد;
                اذهب بعدی;
            }

            اكسر;

        افتراضي: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            إذا (rn->naddrs6 == (u_short) -1) {
                rn->کد = (u_char) کد;
                اذهب بعدی;
            }
        }
#پایان_اگر

        بعدی = rn->انتظار;
        rn->انتظار = عدم;

        ngx_صف_remove(&rn->صف);

        ngx_rbtree_delete(&r->name_rbtree, &rn->گره);

        /* unlock name mutex */

        طالما (بعدی) {
            ctx = بعدی;
            ctx->state = کد;
            ctx->valid = ngx_زمان() + (r->valid ? r->valid : 10);
            بعدی = ctx->بعدی;

            ctx->handler(ctx);
        }

        ngx_resolver_free_گره(r, rn);

        أرجع;
    }

    i = ans;
    naddrs = 0;
    cname = عدم;

    لكل (a = 0; a < nan; a++) {

        شروع = i;

        طالما (i < n) {

            إذا (buf[i] & 0xc0) {
                i += 2;
                اذهب یافت;
            }

            إذا (buf[i] == 0) {
                i++;
                اذهب test_درازی;
            }

            i += 1 + buf[i];
        }

        اذهب short_response;

    test_درازی:

        إذا (i - شروع < 2) {
            err = "invalid name in DNS response";
            اذهب invalid;
        }

    یافت:

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
            ngx_log_error(r->log_level, r->لگاریتم, 0,
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
#پایان_اگر

        حالة NGX_RESOLVE_CNAME:

            cname = &buf[i];

            اكسر;

        حالة NGX_RESOLVE_DNAME:

            اكسر;

        افتراضي:

            ngx_log_error(r->log_level, r->لگاریتم, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->لگاریتم, 0,
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
#پایان_اگر

            اكسر;
#پایان_اگر

        افتراضي: /* NGX_RESOLVE_A */

            إذا (naddrs == 1) {
                addr = &rn->u.addr;
                rn->naddrs = 1;

            } وإلا {
                addr = ngx_resolver_alloc(r, naddrs * حجم(درون_addr_t));
                إذا (addr == عدم) {
                    اذهب failed;
                }

                rn->u.addrs = addr;
                rn->naddrs = (u_short) naddrs;
            }

#إذا (NGX_HAVE_INET6 && NGX_SUPPRESS_WARN)
            addr6 = عدم;
#پایان_اگر
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
                        اذهب بعدی;
                    }
#پایان_اگر

                    اكسر;
                }
            }

#إذا (NGX_HAVE_INET6)
            وإلا إذا (type == NGX_RESOLVE_AAAA) {

                ngx_memcpy(addr6[j].s6_addr, &buf[i], 16);

                إذا (++j == naddrs) {

                    إذا (rn->naddrs == (u_short) -1) {
                        اذهب بعدی;
                    }

                    اكسر;
                }
            }
#پایان_اگر

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
#پایان_اگر

    افتراضي: /* NGX_RESOLVE_A */

        إذا (rn->naddrs == (u_short) -1) {
            rn->naddrs = 0;
        }
    }

    إذا (rn->naddrs != (u_short) -1
#إذا (NGX_HAVE_INET6)
        && rn->naddrs6 != (u_short) -1
#پایان_اگر
        && rn->naddrs
#إذا (NGX_HAVE_INET6)
           + rn->naddrs6
#پایان_اگر
           > 0)
    {

#إذا (NGX_HAVE_INET6)
    export:
#پایان_اگر

        naddrs = rn->naddrs;
#إذا (NGX_HAVE_INET6)
        naddrs += rn->naddrs6;
#پایان_اگر

        إذا (naddrs == 1 && rn->naddrs == 1) {
            addrs = عدم;

        } وإلا {
            addrs = ngx_resolver_export(r, rn, 0);
            إذا (addrs == عدم) {
                اذهب failed;
            }
        }

        ngx_صف_remove(&rn->صف);

        rn->valid = ngx_زمان() + (r->valid ? r->valid : (زمان_t) rn->ttl);
        rn->expire = ngx_زمان() + r->expire;

        ngx_صف_درج_سر(&r->name_expire_صف, &rn->صف);

        بعدی = rn->انتظار;
        rn->انتظار = عدم;

        /* unlock name mutex */

        طالما (بعدی) {
            ctx = بعدی;
            ctx->state = NGX_OK;
            ctx->valid = rn->valid;
            ctx->naddrs = naddrs;

            إذا (addrs == عدم) {
                ctx->addrs = &ctx->addr;
                ctx->addr.sockaddr = (بنية sockaddr *) &ctx->سینوس;
                ctx->addr.socklen = حجم(بنية sockaddr_درون);
                ngx_memzero(&ctx->سینوس, حجم(بنية sockaddr_درون));
                ctx->سینوس.sin_family = AF_INET;
                ctx->سینوس.sin_addr.s_addr = rn->u.addr;

            } وإلا {
                ctx->addrs = addrs;
            }

            بعدی = ctx->بعدی;

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
#پایان_اگر

        أرجع;
    }

    إذا (cname) {

        /* CNAME only */

        إذا (rn->naddrs == (u_short) -1
#إذا (NGX_HAVE_INET6)
            || rn->naddrs6 == (u_short) -1
#پایان_اگر
            )
        {
            اذهب بعدی;
        }

        إذا (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            اذهب failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لگاریتم, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_صف_remove(&rn->صف);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.داده;

        rn->valid = ngx_زمان() + (r->valid ? r->valid : (زمان_t) rn->ttl);
        rn->expire = ngx_زمان() + r->expire;

        ngx_صف_درج_سر(&r->name_expire_صف, &rn->صف);

        ngx_resolver_free(r, rn->query);
        rn->query = عدم;
#إذا (NGX_HAVE_INET6)
        rn->query6 = عدم;
#پایان_اگر

        ctx = rn->انتظار;
        rn->انتظار = عدم;

        إذا (ctx) {

            إذا (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                افعل {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    بعدی = ctx->بعدی;

                    ctx->handler(ctx);

                    ctx = بعدی;
                } طالما (ctx);

                أرجع;
            }

            لكل (بعدی = ctx; بعدی; بعدی = بعدی->بعدی) {
                بعدی->گره = عدم;
            }

            (فراغ) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        أرجع;
    }

    ngx_log_error(r->log_level, r->لگاریتم, 0,
                  "no A or CNAME types in DNS response");
    أرجع;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->لگاریتم, 0, err);

    أرجع;

failed:

بعدی:

    /* unlock name mutex */

    أرجع;
}


ساكن فراغ
ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, اندازه_t n,
    ngx_uint_t ident, ngx_uint_t کد, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans)
{
    حرف                       *err;
    u_char                     *cname;
    اندازه_t                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    ngx_str_t                   name;
    ngx_uint_t                  type, qident, class, شروع, nsrvs, a, i, j;
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *بعدی;
    ngx_resolver_srv_t         *srvs;
    ngx_resolver_گره_t        *rn;
    ngx_resolver_connection_t  *rec;

    إذا (ngx_resolver_copy(r, &name, buf,
                          buf + حجم(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        أرجع;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لگاریتم, 0, "resolver qs:%V", &name);

    hash = ngx_بررسی_چرخهای_32_short(name.داده, name.len);

    rn = ngx_resolver_lookup_srv(r, &name, hash);

    إذا (rn == عدم || rn->query == عدم) {
        ngx_log_error(r->log_level, r->لگاریتم, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.داده);
        اذهب failed;
    }

    إذا (trunc && rn->tcp) {
        ngx_resolver_free(r, name.داده);
        اذهب failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    إذا (ident != qident) {
        ngx_log_error(r->log_level, r->لگاریتم, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.داده);
        اذهب failed;
    }

    ngx_resolver_free(r, name.داده);

    إذا (trunc) {

        ngx_صف_remove(&rn->صف);

        إذا (rn->انتظار == عدم) {
            ngx_rbtree_delete(&r->srv_rbtree, &rn->گره);
            ngx_resolver_free_گره(r, rn);
            أرجع;
        }

        rec = r->connections.elts;
        rec = &rec[rn->آخرین_connection];

        rn->tcp = 1;

        (فراغ) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);

        rn->expire = ngx_زمان() + r->resend_timeout;

        ngx_صف_درج_سر(&r->srv_resend_صف, &rn->صف);

        أرجع;
    }

    إذا (کد == 0 && rn->کد) {
        کد = rn->کد;
    }

    إذا (کد == 0 && nan == 0) {
        کد = NGX_RESOLVE_NXDOMAIN;
    }

    إذا (کد) {
        بعدی = rn->انتظار;
        rn->انتظار = عدم;

        ngx_صف_remove(&rn->صف);

        ngx_rbtree_delete(&r->srv_rbtree, &rn->گره);

        طالما (بعدی) {
            ctx = بعدی;
            ctx->state = کد;
            ctx->valid = ngx_زمان() + (r->valid ? r->valid : 10);
            بعدی = ctx->بعدی;

            ctx->handler(ctx);
        }

        ngx_resolver_free_گره(r, rn);

        أرجع;
    }

    i = ans;
    nsrvs = 0;
    cname = عدم;

    لكل (a = 0; a < nan; a++) {

        شروع = i;

        طالما (i < n) {

            إذا (buf[i] & 0xc0) {
                i += 2;
                اذهب یافت;
            }

            إذا (buf[i] == 0) {
                i++;
                اذهب test_درازی;
            }

            i += 1 + buf[i];
        }

        اذهب short_response;

    test_درازی:

        إذا (i - شروع < 2) {
            err = "invalid name DNS response";
            اذهب invalid;
        }

    یافت:

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
            ngx_log_error(r->log_level, r->لگاریتم, 0,
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

            ngx_log_error(r->log_level, r->لگاریتم, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->لگاریتم, 0,
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

                srvs[j].اولویت = (buf[i] << 8) + buf[i + 1];
                srvs[j].وزن = (buf[i + 2] << 8) + buf[i + 3];

                إذا (srvs[j].وزن == 0) {
                    srvs[j].وزن = 1;
                }

                srvs[j].پورت = (buf[i + 4] << 8) + buf[i + 5];

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

        ngx_مرتب_سازی(srvs, nsrvs, حجم(ngx_resolver_srv_t),
                 ngx_resolver_cmp_srvs);

        ngx_resolver_free(r, rn->query);
        rn->query = عدم;

        ngx_صف_remove(&rn->صف);

        rn->valid = ngx_زمان() + (r->valid ? r->valid : (زمان_t) rn->ttl);
        rn->expire = ngx_زمان() + r->expire;

        ngx_صف_درج_سر(&r->srv_expire_صف, &rn->صف);

        بعدی = rn->انتظار;
        rn->انتظار = عدم;

        طالما (بعدی) {
            ctx = بعدی;
            بعدی = ctx->بعدی;

            ngx_resolver_resolve_srv_نامها(ctx, rn);
        }

        أرجع;
    }

    rn->nsrvs = 0;

    إذا (cname) {

        /* CNAME only */

        إذا (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            اذهب failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لگاریتم, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_صف_remove(&rn->صف);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.داده;

        rn->valid = ngx_زمان() + (r->valid ? r->valid : (زمان_t) rn->ttl);
        rn->expire = ngx_زمان() + r->expire;

        ngx_صف_درج_سر(&r->srv_expire_صف, &rn->صف);

        ngx_resolver_free(r, rn->query);
        rn->query = عدم;
#إذا (NGX_HAVE_INET6)
        rn->query6 = عدم;
#پایان_اگر

        ctx = rn->انتظار;
        rn->انتظار = عدم;

        إذا (ctx) {

            إذا (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                افعل {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    بعدی = ctx->بعدی;

                    ctx->handler(ctx);

                    ctx = بعدی;
                } طالما (ctx);

                أرجع;
            }

            لكل (بعدی = ctx; بعدی; بعدی = بعدی->بعدی) {
                بعدی->گره = عدم;
            }

            (فراغ) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        أرجع;
    }

    ngx_log_error(r->log_level, r->لگاریتم, 0, "no SRV type in DNS response");

    أرجع;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->لگاریتم, 0, err);

    أرجع;

failed:

    /* unlock name mutex */

    أرجع;
}


ساكن فراغ
ngx_resolver_resolve_srv_نامها(ngx_resolver_ctx_t *ctx, ngx_resolver_گره_t *rn)
{
    ngx_uint_t                i;
    ngx_resolver_t           *r;
    ngx_resolver_ctx_t       *cctx;
    ngx_resolver_srv_name_t  *srvs;

    r = ctx->resolver;

    ctx->گره = عدم;
    ctx->state = NGX_OK;
    ctx->valid = rn->valid;
    ctx->شمار = rn->nsrvs;

    srvs = ngx_resolver_calloc(r, rn->nsrvs * حجم(ngx_resolver_srv_name_t));
    إذا (srvs == عدم) {
        اذهب failed;
    }

    ctx->srvs = srvs;
    ctx->nsrvs = rn->nsrvs;

    إذا (ctx->event && ctx->event->timer_مجموعه) {
        ngx_del_timer(ctx->event);
    }

    لكل (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
        srvs[i].name.داده = ngx_resolver_alloc(r, rn->u.srvs[i].name.len);
        إذا (srvs[i].name.داده == عدم) {
            اذهب failed;
        }

        srvs[i].name.len = rn->u.srvs[i].name.len;
        ngx_memcpy(srvs[i].name.داده, rn->u.srvs[i].name.داده,
                   srvs[i].name.len);

        cctx = ngx_resolve_شروع(r, عدم);
        إذا (cctx == عدم) {
            اذهب failed;
        }

        cctx->name = srvs[i].name;
        cctx->handler = ngx_resolver_srv_نامها_handler;
        cctx->داده = ctx;
        cctx->srvs = &srvs[i];
        cctx->timeout = ctx->timeout;

        srvs[i].اولویت = rn->u.srvs[i].اولویت;
        srvs[i].وزن = rn->u.srvs[i].وزن;
        srvs[i].پورت = rn->u.srvs[i].پورت;
        srvs[i].ctx = cctx;

        إذا (ngx_resolve_name(cctx) == NGX_ERROR) {
            srvs[i].ctx = عدم;
            اذهب failed;
        }
    }

    أرجع;

failed:

    ctx->state = NGX_ERROR;
    ctx->valid = ngx_زمان() + (r->valid ? r->valid : 10);

    ctx->handler(ctx);
}


ساكن فراغ
ngx_resolver_srv_نامها_handler(ngx_resolver_ctx_t *cctx)
{
    ngx_uint_t                i;
    ngx_addr_t               *addrs;
    ngx_resolver_t           *r;
    ngx_sockaddr_t           *sockaddr;
    ngx_resolver_ctx_t       *ctx;
    ngx_resolver_srv_name_t  *srv;

    r = cctx->resolver;
    ctx = cctx->داده;
    srv = cctx->srvs;

    ctx->شمار--;
    ctx->async |= cctx->async;

    srv->ctx = عدم;
    srv->state = cctx->state;

    إذا (cctx->naddrs) {

        ctx->valid = ngx_min(ctx->valid, cctx->valid);

        addrs = ngx_resolver_calloc(r, cctx->naddrs * حجم(ngx_addr_t));
        إذا (addrs == عدم) {
            srv->state = NGX_ERROR;
            اذهب انجامشده;
        }

        sockaddr = ngx_resolver_alloc(r, cctx->naddrs * حجم(ngx_sockaddr_t));
        إذا (sockaddr == عدم) {
            ngx_resolver_free(r, addrs);
            srv->state = NGX_ERROR;
            اذهب انجامشده;
        }

        لكل (i = 0; i < cctx->naddrs; i++) {
            addrs[i].sockaddr = &sockaddr[i].sockaddr;
            addrs[i].socklen = cctx->addrs[i].socklen;

            ngx_memcpy(&sockaddr[i], cctx->addrs[i].sockaddr,
                       addrs[i].socklen);

            ngx_inet_مجموعه_پورت(addrs[i].sockaddr, srv->پورت);
        }

        srv->addrs = addrs;
        srv->naddrs = cctx->naddrs;
    }

انجامشده:

    ngx_resolve_name_انجامشده(cctx);

    إذا (ctx->شمار == 0) {
        ngx_resolver_report_srv(r, ctx);
    }
}


ساكن فراغ
ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, اندازه_t n,
    ngx_uint_t ident, ngx_uint_t کد, ngx_uint_t nan)
{
    حرف                 *err;
    اندازه_t                len;
    درون_addr_t             addr;
    int32_t               ttl;
    ngx_int_t             octet;
    ngx_str_t             name;
    ngx_uint_t            ماسک, type, class, qident, a, i, شروع;
    ngx_صف_t          *expire_صف;
    ngx_rbtree_t         *درخت;
    ngx_resolver_an_t    *an;
    ngx_resolver_ctx_t   *ctx, *بعدی;
    ngx_resolver_گره_t  *rn;
#إذا (NGX_HAVE_INET6)
    uint32_t              hash;
    ngx_int_t             digit;
    بنية in6_addr       addr6;
#پایان_اگر

    إذا (ngx_resolver_copy(r, &name, buf,
                          buf + حجم(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        أرجع;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لگاریتم, 0, "resolver qs:%V", &name);

    /* AF_INET */

    addr = 0;
    i = حجم(ngx_resolver_hdr_t);

    لكل (ماسک = 0; ماسک < 32; ماسک += 8) {
        len = buf[i++];

        octet = ngx_atoi(&buf[i], len);
        إذا (octet == NGX_ERROR || octet > 255) {
            اذهب invalid_درون_addr_arpa;
        }

        addr += octet << ماسک;
        i += len;
    }

    إذا (ngx_strcasecmp(&buf[i], (u_char *) "\7in-addr\4arpa") == 0) {
        i += حجم("\7in-addr\4arpa");

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        درخت = &r->addr_rbtree;
        expire_صف = &r->addr_expire_صف;

        اذهب valid;
    }

invalid_درون_addr_arpa:

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

        hash = ngx_بررسی_چرخهای_32_short(addr6.s6_addr, 16);
        rn = ngx_resolver_lookup_addr6(r, &addr6, hash);

        درخت = &r->addr6_rbtree;
        expire_صف = &r->addr6_expire_صف;

        اذهب valid;
    }

invalid_ip6_arpa:
#پایان_اگر

    ngx_log_error(r->log_level, r->لگاریتم, 0,
                  "invalid in-addr.arpa or ip6.arpa name in DNS response");
    ngx_resolver_free(r, name.داده);
    أرجع;

valid:

    إذا (rn == عدم || rn->query == عدم) {
        ngx_log_error(r->log_level, r->لگاریتم, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.داده);
        اذهب failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    إذا (ident != qident) {
        ngx_log_error(r->log_level, r->لگاریتم, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.داده);
        اذهب failed;
    }

    ngx_resolver_free(r, name.داده);

    إذا (کد == 0 && nan == 0) {
        کد = NGX_RESOLVE_NXDOMAIN;
    }

    إذا (کد) {
        بعدی = rn->انتظار;
        rn->انتظار = عدم;

        ngx_صف_remove(&rn->صف);

        ngx_rbtree_delete(درخت, &rn->گره);

        /* unlock addr mutex */

        طالما (بعدی) {
            ctx = بعدی;
            ctx->state = کد;
            ctx->valid = ngx_زمان() + (r->valid ? r->valid : 10);
            بعدی = ctx->بعدی;

            ctx->handler(ctx);
        }

        ngx_resolver_free_گره(r, rn);

        أرجع;
    }

    i += حجم(ngx_resolver_qs_t);

    لكل (a = 0; a < nan; a++) {

        شروع = i;

        طالما (i < n) {

            إذا (buf[i] & 0xc0) {
                i += 2;
                اذهب یافت;
            }

            إذا (buf[i] == 0) {
                i++;
                اذهب test_درازی;
            }

            i += 1 + buf[i];
        }

        اذهب short_response;

    test_درازی:

        إذا (i - شروع < 2) {
            err = "invalid name in DNS response";
            اذهب invalid;
        }

    یافت:

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
            ngx_log_error(r->log_level, r->لگاریتم, 0,
                          "unexpected RR class %ui in DNS response", class);
            اذهب failed;
        }

        إذا (ttl < 0) {
            ttl = 0;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->لگاریتم, 0,
                      "resolver qt:%ui cl:%ui len:%uz",
                      type, class, len);

        i += حجم(ngx_resolver_an_t);

        بدل (type) {

        حالة NGX_RESOLVE_PTR:

            اذهب ptr;

        حالة NGX_RESOLVE_CNAME:

            اكسر;

        افتراضي:

            ngx_log_error(r->log_level, r->لگاریتم, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->لگاریتم, 0,
                  "no PTR type in DNS response");
    أرجع;

ptr:

    إذا (ngx_resolver_copy(r, &name, buf, buf + i, buf + n) != NGX_OK) {
        اذهب failed;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لگاریتم, 0, "resolver an:%V", &name);

    إذا (name.len != (اندازه_t) rn->nlen
        || ngx_strncmp(name.داده, rn->name, name.len) != 0)
    {
        إذا (rn->nlen) {
            ngx_resolver_free(r, rn->name);
        }

        rn->nlen = (u_short) name.len;
        rn->name = name.داده;

        name.داده = ngx_resolver_dup(r, rn->name, name.len);
        إذا (name.داده == عدم) {
            اذهب failed;
        }
    }

    ngx_صف_remove(&rn->صف);

    rn->valid = ngx_زمان() + (r->valid ? r->valid : ttl);
    rn->expire = ngx_زمان() + r->expire;

    ngx_صف_درج_سر(expire_صف, &rn->صف);

    بعدی = rn->انتظار;
    rn->انتظار = عدم;

    /* unlock addr mutex */

    طالما (بعدی) {
        ctx = بعدی;
        ctx->state = NGX_OK;
        ctx->valid = rn->valid;
        ctx->name = name;
        بعدی = ctx->بعدی;

        ctx->handler(ctx);
    }

    ngx_resolver_free(r, name.داده);

    أرجع;

short_response:

    err = "short DNS response";

invalid:

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->لگاریتم, 0, err);

    أرجع;

failed:

    /* unlock addr mutex */

    أرجع;
}


ساكن ngx_resolver_گره_t *
ngx_resolver_lookup_name(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_گره_t    *گره, *sentinel;
    ngx_resolver_گره_t  *rn;

    گره = r->name_rbtree.ریشه;
    sentinel = r->name_rbtree.sentinel;

    طالما (گره != sentinel) {

        إذا (hash < گره->کلید) {
            گره = گره->چپ;
            استمر;
        }

        إذا (hash > گره->کلید) {
            گره = گره->راست;
            استمر;
        }

        /* hash == node->key */

        rn = ngx_resolver_گره(گره);

        rc = ngx_memn2cmp(name->داده, rn->name, name->len, rn->nlen);

        إذا (rc == 0) {
            أرجع rn;
        }

        گره = (rc < 0) ? گره->چپ : گره->راست;
    }

    /* not found */

    أرجع عدم;
}


ساكن ngx_resolver_گره_t *
ngx_resolver_lookup_srv(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_گره_t    *گره, *sentinel;
    ngx_resolver_گره_t  *rn;

    گره = r->srv_rbtree.ریشه;
    sentinel = r->srv_rbtree.sentinel;

    طالما (گره != sentinel) {

        إذا (hash < گره->کلید) {
            گره = گره->چپ;
            استمر;
        }

        إذا (hash > گره->کلید) {
            گره = گره->راست;
            استمر;
        }

        /* hash == node->key */

        rn = ngx_resolver_گره(گره);

        rc = ngx_memn2cmp(name->داده, rn->name, name->len, rn->nlen);

        إذا (rc == 0) {
            أرجع rn;
        }

        گره = (rc < 0) ? گره->چپ : گره->راست;
    }

    /* not found */

    أرجع عدم;
}


ساكن ngx_resolver_گره_t *
ngx_resolver_lookup_addr(ngx_resolver_t *r, درون_addr_t addr)
{
    ngx_rbtree_گره_t  *گره, *sentinel;

    گره = r->addr_rbtree.ریشه;
    sentinel = r->addr_rbtree.sentinel;

    طالما (گره != sentinel) {

        إذا (addr < گره->کلید) {
            گره = گره->چپ;
            استمر;
        }

        إذا (addr > گره->کلید) {
            گره = گره->راست;
            استمر;
        }

        /* addr == node->key */

        أرجع ngx_resolver_گره(گره);
    }

    /* not found */

    أرجع عدم;
}


#إذا (NGX_HAVE_INET6)

ساكن ngx_resolver_گره_t *
ngx_resolver_lookup_addr6(ngx_resolver_t *r, بنية in6_addr *addr,
    uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_گره_t    *گره, *sentinel;
    ngx_resolver_گره_t  *rn;

    گره = r->addr6_rbtree.ریشه;
    sentinel = r->addr6_rbtree.sentinel;

    طالما (گره != sentinel) {

        إذا (hash < گره->کلید) {
            گره = گره->چپ;
            استمر;
        }

        إذا (hash > گره->کلید) {
            گره = گره->راست;
            استمر;
        }

        /* hash == node->key */

        rn = ngx_resolver_گره(گره);

        rc = ngx_memcmp(addr, &rn->addr6, 16);

        إذا (rc == 0) {
            أرجع rn;
        }

        گره = (rc < 0) ? گره->چپ : گره->راست;
    }

    /* not found */

    أرجع عدم;
}

#پایان_اگر


ساكن فراغ
ngx_resolver_rbtree_درج_مقدار(ngx_rbtree_گره_t *موقت,
    ngx_rbtree_گره_t *گره, ngx_rbtree_گره_t *sentinel)
{
    ngx_rbtree_گره_t    **p;
    ngx_resolver_گره_t   *rn, *rn_موقت;

    لكل ( ;; ) {

        إذا (گره->کلید < موقت->کلید) {

            p = &موقت->چپ;

        } وإلا إذا (گره->کلید > موقت->کلید) {

            p = &موقت->راست;

        } وإلا { /* node->key == temp->key */

            rn = ngx_resolver_گره(گره);
            rn_موقت = ngx_resolver_گره(موقت);

            p = (ngx_memn2cmp(rn->name, rn_موقت->name, rn->nlen, rn_موقت->nlen)
                 < 0) ? &موقت->چپ : &موقت->راست;
        }

        إذا (*p == sentinel) {
            اكسر;
        }

        موقت = *p;
    }

    *p = گره;
    گره->والد = موقت;
    گره->چپ = sentinel;
    گره->راست = sentinel;
    ngx_rbt_red(گره);
}


#إذا (NGX_HAVE_INET6)

ساكن فراغ
ngx_resolver_rbtree_درج_addr6_مقدار(ngx_rbtree_گره_t *موقت,
    ngx_rbtree_گره_t *گره, ngx_rbtree_گره_t *sentinel)
{
    ngx_rbtree_گره_t    **p;
    ngx_resolver_گره_t   *rn, *rn_موقت;

    لكل ( ;; ) {

        إذا (گره->کلید < موقت->کلید) {

            p = &موقت->چپ;

        } وإلا إذا (گره->کلید > موقت->کلید) {

            p = &موقت->راست;

        } وإلا { /* node->key == temp->key */

            rn = ngx_resolver_گره(گره);
            rn_موقت = ngx_resolver_گره(موقت);

            p = (ngx_memcmp(&rn->addr6, &rn_موقت->addr6, 16)
                 < 0) ? &موقت->چپ : &موقت->راست;
        }

        إذا (*p == sentinel) {
            اكسر;
        }

        موقت = *p;
    }

    *p = گره;
    گره->والد = موقت;
    گره->چپ = sentinel;
    گره->راست = sentinel;
    ngx_rbt_red(گره);
}

#پایان_اگر


ساكن ngx_int_t
ngx_resolver_create_name_query(ngx_resolver_t *r, ngx_resolver_گره_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    اندازه_t               len, nlen;
    ngx_uint_t           ident;
    ngx_resolver_qs_t   *qs;
    ngx_resolver_hdr_t  *query;

    nlen = name->len ? (1 + name->len + 1) : 1;

    len = حجم(ngx_resolver_hdr_t) + nlen + حجم(ngx_resolver_qs_t);

#إذا (NGX_HAVE_INET6)
    p = ngx_resolver_alloc(r, len * (r->ipv4 + r->ipv6));
#وإلا
    p = ngx_resolver_alloc(r, len);
#پایان_اگر
    إذا (p == عدم) {
        أرجع NGX_ERROR;
    }

    rn->qlen = (u_short) len;
    rn->query = p;

#إذا (NGX_HAVE_INET6)
    إذا (r->ipv6) {
        rn->query6 = r->ipv4 ? (p + len) : p;
    }
#پایان_اگر

    query = (ngx_resolver_hdr_t *) p;

    إذا (r->ipv4) {
        ident = ngx_random();

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->لگاریتم, 0,
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

    لكل (s = name->داده + name->len - 1; s >= name->داده; s--) {
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

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->لگاریتم, 0,
                   "resolve: \"%V\" AAAA %i", name, ident & 0xffff);

    query->ident_hi = (u_char) ((ident >> 8) & 0xff);
    query->ident_lo = (u_char) (ident & 0xff);

    p += حجم(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    qs->type_lo = NGX_RESOLVE_AAAA;
#پایان_اگر

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_resolver_create_srv_query(ngx_resolver_t *r, ngx_resolver_گره_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    اندازه_t               len, nlen;
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

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->لگاریتم, 0,
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

    لكل (s = name->داده + name->len - 1; s >= name->داده; s--) {
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
ngx_resolver_create_addr_query(ngx_resolver_t *r, ngx_resolver_گره_t *rn,
    ngx_resolver_addr_t *addr)
{
    u_char               *p, *d;
    اندازه_t                len;
    درون_addr_t             inaddr;
    ngx_int_t             n;
    ngx_uint_t            ident;
    ngx_resolver_hdr_t   *query;
    بنية sockaddr_درون   *سینوس;
#إذا (NGX_HAVE_INET6)
    بنية sockaddr_in6  *sin6;
#پایان_اگر

    بدل (addr->sockaddr->sa_family) {

#إذا (NGX_HAVE_INET6)
    حالة AF_INET6:
        len = حجم(ngx_resolver_hdr_t)
              + 64 + حجم(".ip6.arpa.") - 1
              + حجم(ngx_resolver_qs_t);

        اكسر;
#پایان_اگر

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
#پایان_اگر

    افتراضي: /* AF_INET */

        سینوس = (بنية sockaddr_درون *) addr->sockaddr;
        inaddr = ntohl(سینوس->sin_addr.s_addr);

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
    u_char *آخرین)
{
    حرف        *err;
    u_char      *p, *dst;
    اندازه_t       len;
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
            اذهب انجامشده;
        }

        إذا (n & 0xc0) {
            إذا ((n & 0xc0) != 0xc0) {
                err = "invalid label type in DNS response";
                اذهب invalid;
            }

            إذا (p >= آخرین) {
                err = "name is out of DNS response";
                اذهب invalid;
            }

            n = ((n & 0x3f) << 8) + *p;
            p = &buf[n];

        } وإلا {
            len += 1 + n;
            p = &p[n];
        }

        إذا (p >= آخرین) {
            err = "name is out of DNS response";
            اذهب invalid;
        }
    }

    err = "compression pointers loop in DNS response";

invalid:

    ngx_log_error(r->log_level, r->لگاریتم, 0, err);

    أرجع NGX_ERROR;

انجامشده:

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

    name->داده = dst;

    لكل ( ;; ) {
        n = *src++;

        إذا (n == 0) {
            name->len = dst - name->داده - 1;
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
ngx_resolver_مجموعه_timeout(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx)
{
    إذا (ctx->event || ctx->timeout == 0) {
        أرجع NGX_OK;
    }

    ctx->event = ngx_resolver_calloc(r, حجم(ngx_event_t));
    إذا (ctx->event == عدم) {
        أرجع NGX_ERROR;
    }

    ctx->event->handler = ngx_resolver_timeout_handler;
    ctx->event->داده = ctx;
    ctx->event->لگاریتم = r->لگاریتم;
    ctx->event->cancelable = ctx->cancelable;
    ctx->ident = -1;

    ngx_جمع_کن_timer(ctx->event, ctx->timeout);

    أرجع NGX_OK;
}


ساكن فراغ
ngx_resolver_timeout_handler(ngx_event_t *ev)
{
    ngx_resolver_ctx_t  *ctx;

    ctx = ev->داده;

    ctx->state = NGX_RESOLVE_TIMEDOUT;

    ctx->handler(ctx);
}


ساكن فراغ
ngx_resolver_free_گره(ngx_resolver_t *r, ngx_resolver_گره_t *rn)
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
#پایان_اگر

    إذا (rn->nsrvs) {
        لكل (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
            إذا (rn->u.srvs[i].name.داده) {
                ngx_resolver_free_locked(r, rn->u.srvs[i].name.داده);
            }
        }

        ngx_resolver_free_locked(r, rn->u.srvs);
    }

    ngx_resolver_free_locked(r, rn);

    /* unlock alloc mutex */
}


ساكن فراغ *
ngx_resolver_alloc(ngx_resolver_t *r, اندازه_t اندازه)
{
    u_char  *p;

    /* lock alloc mutex */

    p = ngx_alloc(اندازه, r->لگاریتم);

    /* unlock alloc mutex */

    أرجع p;
}


ساكن فراغ *
ngx_resolver_calloc(ngx_resolver_t *r, اندازه_t اندازه)
{
    u_char  *p;

    p = ngx_resolver_alloc(r, اندازه);

    إذا (p) {
        ngx_memzero(p, اندازه);
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
ngx_resolver_dup(ngx_resolver_t *r, فراغ *src, اندازه_t اندازه)
{
    فراغ  *dst;

    dst = ngx_resolver_alloc(r, اندازه);

    إذا (dst == عدم) {
        أرجع dst;
    }

    ngx_memcpy(dst, src, اندازه);

    أرجع dst;
}


ساكن ngx_resolver_addr_t *
ngx_resolver_export(ngx_resolver_t *r, ngx_resolver_گره_t *rn,
    ngx_uint_t rotate)
{
    ngx_uint_t            d, i, j, n;
    درون_addr_t            *addr;
    ngx_sockaddr_t       *sockaddr;
    بنية sockaddr_درون   *سینوس;
    ngx_resolver_addr_t  *dst;
#إذا (NGX_HAVE_INET6)
    بنية in6_addr      *addr6;
    بنية sockaddr_in6  *sin6;
#پایان_اگر

    n = rn->naddrs;
#إذا (NGX_HAVE_INET6)
    n += rn->naddrs6;
#پایان_اگر

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
            سینوس = &sockaddr[d].sockaddr_درون;
            سینوس->sin_family = AF_INET;
            سینوس->sin_addr.s_addr = addr[j++];
            dst[d].sockaddr = (بنية sockaddr *) سینوس;
            dst[d++].socklen = حجم(بنية sockaddr_درون);

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
#پایان_اگر

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
            ctx->valid = ngx_زمان() + (r->valid ? r->valid : 10);

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

        ctx->valid = ngx_زمان() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        أرجع;
    }

    addrs = ngx_resolver_calloc(r, naddrs * حجم(ngx_resolver_addr_t));
    إذا (addrs == عدم) {
        ctx->state = NGX_ERROR;
        ctx->valid = ngx_زمان() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        أرجع;
    }

    i = 0;
    n = 0;

    افعل {
        nw = 0;

        لكل (j = i; j < nsrvs; j++) {
            إذا (srvs[j].اولویت != srvs[i].اولویت) {
                اكسر;
            }

            nw += srvs[j].naddrs * srvs[j].وزن;
        }

        إذا (nw == 0) {
            اذهب بعدی_srv;
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
                addrs[n].اولویت = srvs[k].اولویت;
                addrs[n].وزن = srvs[k].وزن;
                n++;
            }

            إذا (++k == j) {
                k = i;
            }
        }

بعدی_srv:

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
ngx_resolver_log_error(ngx_log_t *لگاریتم, u_char *buf, اندازه_t len)
{
    u_char                     *p;
    ngx_resolver_connection_t  *rec;

    p = buf;

    إذا (لگاریتم->action) {
        p = ngx_snprintf(buf, len, " while %s", لگاریتم->action);
        len -= p - buf;
    }

    rec = لگاریتم->داده;

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

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->لگاریتم, 0, "UDP socket %d", s);

    إذا (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->لگاریتم, ngx_socket_errno,
                      ngx_socket_n " failed");
        أرجع NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->لگاریتم);

    إذا (c == عدم) {
        إذا (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->لگاریتم, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        أرجع NGX_ERROR;
    }

    إذا (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->لگاریتم, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        اذهب failed;
    }

    rev = c->اقرأ;
    wev = c->اكتب;

    rev->لگاریتم = &rec->لگاریتم;
    wev->لگاریتم = &rec->لگاریتم;

    rec->udp = c;

    c->عدد = ngx_atomic_fetch_جمع_کن(ngx_connection_counter, 1);

    c->شروع_زمان = ngx_current_msec;

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->لگاریتم, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->عدد);

    rc = connect(s, rec->sockaddr, rec->socklen);

    إذا (rc == -1) {
        ngx_log_error(NGX_LOG_CRIT, &rec->لگاریتم, ngx_socket_errno,
                      "connect() failed");

        اذهب failed;
    }

    /* UDP sockets are always ready to write */
    wev->ready = 1;

    event = (ngx_event_flags & NGX_USE_CLEAR_EVENT) ?
                /* kqueue, epoll */                 NGX_CLEAR_EVENT:
                /* select, poll, /dev/poll */       NGX_LEVEL_EVENT;
                /* eventport event type has no meaning: oneshot only */

    إذا (ngx_جمع_کن_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
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

    s = ngx_socket(rec->sockaddr->sa_family, SOCK_جریان, 0);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->لگاریتم, 0, "TCP socket %d", s);

    إذا (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->لگاریتم, ngx_socket_errno,
                      ngx_socket_n " failed");
        أرجع NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->لگاریتم);

    إذا (c == عدم) {
        إذا (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->لگاریتم, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        أرجع NGX_ERROR;
    }

    إذا (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->لگاریتم, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        اذهب failed;
    }

    rev = c->اقرأ;
    wev = c->اكتب;

    rev->لگاریتم = &rec->لگاریتم;
    wev->لگاریتم = &rec->لگاریتم;

    rec->tcp = c;

    c->عدد = ngx_atomic_fetch_جمع_کن(ngx_connection_counter, 1);

    c->شروع_زمان = ngx_current_msec;

    إذا (ngx_جمع_کن_conn) {
        إذا (ngx_جمع_کن_conn(c) == NGX_ERROR) {
            اذهب failed;
        }
    }

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->لگاریتم, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->عدد);

    rc = connect(s, rec->sockaddr, rec->socklen);

    إذا (rc == -1) {
        err = ngx_socket_errno;


        إذا (err != NGX_EINPROGRESS
#إذا (NGX_WIN32)
            /* Winsock returns WSAEWOULDBLOCK (NGX_EAGAIN) */
            && err != NGX_EAGAIN
#پایان_اگر
            )
        {
            إذا (err == NGX_ECONNREFUSED
#إذا (NGX_LINUX)
                /*
                 * Linux returns EAGAIN instead of ECONNREFUSED
                 * for unix sockets if listen queue is full
                 */
                || err == NGX_EAGAIN
#پایان_اگر
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

            ngx_log_error(level, &rec->لگاریتم, err, "connect() to %V failed",
                          &rec->server);

            ngx_close_connection(c);
            rec->tcp = عدم;

            أرجع NGX_ERROR;
        }
    }

    إذا (ngx_جمع_کن_conn) {
        إذا (rc == -1) {

            /* NGX_EINPROGRESS */

            أرجع NGX_AGAIN;
        }

        ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->لگاریتم, 0, "connected");

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

    إذا (ngx_جمع_کن_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        اذهب failed;
    }

    إذا (rc == -1) {

        /* NGX_EINPROGRESS */

        إذا (ngx_جمع_کن_event(wev, NGX_WRITE_EVENT, event) != NGX_OK) {
            اذهب failed;
        }

        أرجع NGX_AGAIN;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->لگاریتم, 0, "connected");

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
    ngx_resolver_srv_t  *اولی, *دومی;

    اولی = (ngx_resolver_srv_t *) one;
    دومی = (ngx_resolver_srv_t *) two;

    p1 = اولی->اولویت;
    p2 = دومی->اولویت;

    أرجع p1 - p2;
}
