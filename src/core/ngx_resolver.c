
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <ngx_event.h>


#تعریف_کړه NGX_RESOLVER_UDP_اندازه   4096

#تعریف_کړه NGX_RESOLVER_TCP_RSIZE  (2 + 65535)
#تعریف_کړه NGX_RESOLVER_TCP_WSIZE  8192


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


#تعریف_کړه ngx_resolver_غوټه(n)  ngx_rbtree_معلومات(n, ngx_resolver_غوټه_t, غوټه)


ساكن ngx_int_t ngx_udp_connect(ngx_resolver_connection_t *rec);
ساكن ngx_int_t ngx_tcp_connect(ngx_resolver_connection_t *rec);


ساكن فراغ ngx_resolver_cleanup(فراغ *معلومات);
ساكن فراغ ngx_resolver_cleanup_ونه(ngx_resolver_t *r, ngx_rbtree_t *ونه);
ساكن ngx_int_t ngx_resolve_name_locked(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx, ngx_str_t *name);
ساكن فراغ ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *ونه,
    ngx_کتار_t *کتار);
ساكن ngx_int_t ngx_resolver_send_query(ngx_resolver_t *r,
    ngx_resolver_غوټه_t *rn);
ساكن ngx_int_t ngx_resolver_send_udp_query(ngx_resolver_t *r,
    ngx_resolver_connection_t *rec, u_char *query, u_short qlen);
ساكن ngx_int_t ngx_resolver_send_tcp_query(ngx_resolver_t *r,
    ngx_resolver_connection_t *rec, u_char *query, u_short qlen);
ساكن ngx_int_t ngx_resolver_create_name_query(ngx_resolver_t *r,
    ngx_resolver_غوټه_t *rn, ngx_str_t *name);
ساكن ngx_int_t ngx_resolver_create_srv_query(ngx_resolver_t *r,
    ngx_resolver_غوټه_t *rn, ngx_str_t *name);
ساكن ngx_int_t ngx_resolver_create_addr_query(ngx_resolver_t *r,
    ngx_resolver_غوټه_t *rn, ngx_resolver_addr_t *addr);
ساكن فراغ ngx_resolver_resend_handler(ngx_event_t *ev);
ساكن وخت_t ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *ونه,
    ngx_کتار_t *کتار);
ساكن ngx_uint_t ngx_resolver_resend_تشه(ngx_resolver_t *r);
ساكن فراغ ngx_resolver_udp_read(ngx_event_t *rev);
ساكن فراغ ngx_resolver_tcp_write(ngx_event_t *wev);
ساكن فراغ ngx_resolver_tcp_read(ngx_event_t *rev);
ساكن فراغ ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf,
    اندازه_t n, ngx_uint_t tcp);
ساكن فراغ ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, اندازه_t n,
    ngx_uint_t ident, ngx_uint_t کوډ, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans);
ساكن فراغ ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, اندازه_t n,
    ngx_uint_t ident, ngx_uint_t کوډ, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans);
ساكن فراغ ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, اندازه_t n,
    ngx_uint_t ident, ngx_uint_t کوډ, ngx_uint_t nan);
ساكن ngx_resolver_غوټه_t *ngx_resolver_lookup_name(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
ساكن ngx_resolver_غوټه_t *ngx_resolver_lookup_srv(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
ساكن ngx_resolver_غوټه_t *ngx_resolver_lookup_addr(ngx_resolver_t *r,
    دننه_addr_t addr);
ساكن فراغ ngx_resolver_rbtree_ورننباسه_ارزښت(ngx_rbtree_غوټه_t *لنډمهاله,
    ngx_rbtree_غوټه_t *غوټه, ngx_rbtree_غوټه_t *sentinel);
ساكن ngx_int_t ngx_resolver_copy(ngx_resolver_t *r, ngx_str_t *name,
    u_char *buf, u_char *src, u_char *وروستی);
ساكن ngx_int_t ngx_resolver_سېټ_timeout(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx);
ساكن فراغ ngx_resolver_timeout_handler(ngx_event_t *ev);
ساكن فراغ ngx_resolver_free_غوټه(ngx_resolver_t *r, ngx_resolver_غوټه_t *rn);
ساكن فراغ *ngx_resolver_alloc(ngx_resolver_t *r, اندازه_t اندازه);
ساكن فراغ *ngx_resolver_calloc(ngx_resolver_t *r, اندازه_t اندازه);
ساكن فراغ ngx_resolver_free(ngx_resolver_t *r, فراغ *p);
ساكن فراغ ngx_resolver_free_locked(ngx_resolver_t *r, فراغ *p);
ساكن فراغ *ngx_resolver_dup(ngx_resolver_t *r, فراغ *src, اندازه_t اندازه);
ساكن ngx_resolver_addr_t *ngx_resolver_export(ngx_resolver_t *r,
    ngx_resolver_غوټه_t *rn, ngx_uint_t rotate);
ساكن فراغ ngx_resolver_report_srv(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx);
ساكن u_char *ngx_resolver_log_error(ngx_log_t *لوګاریتم, u_char *buf, اندازه_t len);
ساكن فراغ ngx_resolver_resolve_srv_نومونه(ngx_resolver_ctx_t *ctx,
    ngx_resolver_غوټه_t *rn);
ساكن فراغ ngx_resolver_srv_نومونه_handler(ngx_resolver_ctx_t *ctx);
ساكن ngx_int_t ngx_resolver_cmp_srvs(ثابت فراغ *one, ثابت فراغ *two);

#إذا (NGX_HAVE_INET6)
ساكن فراغ ngx_resolver_rbtree_ورننباسه_addr6_ارزښت(ngx_rbtree_غوټه_t *لنډمهاله,
    ngx_rbtree_غوټه_t *غوټه, ngx_rbtree_غوټه_t *sentinel);
ساكن ngx_resolver_غوټه_t *ngx_resolver_lookup_addr6(ngx_resolver_t *r,
    بنية in6_addr *addr, uint32_t hash);
#د_که_پای


ngx_resolver_t *
ngx_resolver_create(ngx_conf_t *cf, ngx_str_t *نومونه, ngx_uint_t n)
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

    cln = ngx_pool_cleanup_جمع_کړه(cf->pool, 0);
    إذا (cln == عدم) {
        أرجع عدم;
    }

    cln->handler = ngx_resolver_cleanup;
    cln->معلومات = r;

    r->ipv4 = 1;

    ngx_rbtree_init(&r->name_rbtree, &r->name_sentinel,
                    ngx_resolver_rbtree_ورننباسه_ارزښت);

    ngx_rbtree_init(&r->srv_rbtree, &r->srv_sentinel,
                    ngx_resolver_rbtree_ورننباسه_ارزښت);

    ngx_rbtree_init(&r->addr_rbtree, &r->addr_sentinel,
                    ngx_rbtree_ورننباسه_ارزښت);

    ngx_کتار_init(&r->name_resend_کتار);
    ngx_کتار_init(&r->srv_resend_کتار);
    ngx_کتار_init(&r->addr_resend_کتار);

    ngx_کتار_init(&r->name_expire_کتار);
    ngx_کتار_init(&r->srv_expire_کتار);
    ngx_کتار_init(&r->addr_expire_کتار);

#إذا (NGX_HAVE_INET6)
    r->ipv6 = 1;

    ngx_rbtree_init(&r->addr6_rbtree, &r->addr6_sentinel,
                    ngx_resolver_rbtree_ورننباسه_addr6_ارزښت);

    ngx_کتار_init(&r->addr6_resend_کتار);

    ngx_کتار_init(&r->addr6_expire_کتار);
#د_که_پای

    r->event->handler = ngx_resolver_resend_handler;
    r->event->معلومات = r;
    r->event->لوګاریتم = &cf->دوره->new_log;
    r->event->cancelable = 1;
    r->ident = -1;

    r->resend_timeout = 5;
    r->tcp_timeout = 5;
    r->expire = 30;
    r->valid = 0;

    r->لوګاریتم = &cf->دوره->new_log;
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
        إذا (ngx_strncmp(نومونه[i].معلومات, "valid=", 6) == 0) {
            s.len = نومونه[i].len - 6;
            s.معلومات = نومونه[i].معلومات + 6;

            r->valid = ngx_parse_وخت(&s, 1);

            إذا (r->valid == (وخت_t) NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &نومونه[i]);
                أرجع عدم;
            }

            استمر;
        }

#إذا (NGX_HAVE_INET6)
        إذا (ngx_strncmp(نومونه[i].معلومات, "ipv4=", 5) == 0) {

            إذا (ngx_strcmp(&نومونه[i].معلومات[5], "on") == 0) {
                r->ipv4 = 1;

            } وإلا إذا (ngx_strcmp(&نومونه[i].معلومات[5], "off") == 0) {
                r->ipv4 = 0;

            } وإلا {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &نومونه[i]);
                أرجع عدم;
            }

            استمر;
        }

        إذا (ngx_strncmp(نومونه[i].معلومات, "ipv6=", 5) == 0) {

            إذا (ngx_strcmp(&نومونه[i].معلومات[5], "on") == 0) {
                r->ipv6 = 1;

            } وإلا إذا (ngx_strcmp(&نومونه[i].معلومات[5], "off") == 0) {
                r->ipv6 = 0;

            } وإلا {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &نومونه[i]);
                أرجع عدم;
            }

            استمر;
        }
#د_که_پای

        ngx_memzero(&u, حجم(ngx_url_t));

        u.url = نومونه[i];
        u.default_پورټ = 53;

        إذا (ngx_parse_url(cf->pool, &u) != NGX_OK) {
            إذا (u.err) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "%s in resolver \"%V\"",
                                   u.err, &u.url);
            }

            أرجع عدم;
        }

        rec = ngx_array_ټېل_n(&r->connections, u.naddrs);
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
#د_که_پای

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

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_دوره->لوګاریتم, 0, "cleanup resolver");

    ngx_resolver_cleanup_ونه(r, &r->name_rbtree);

    ngx_resolver_cleanup_ونه(r, &r->srv_rbtree);

    ngx_resolver_cleanup_ونه(r, &r->addr_rbtree);

#إذا (NGX_HAVE_INET6)
    ngx_resolver_cleanup_ونه(r, &r->addr6_rbtree);
#د_که_پای

    إذا (r->event->timer_سېټ) {
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
            ngx_resolver_free(r, rec[i].read_buf->پیل);
            ngx_resolver_free(r, rec[i].read_buf);
        }

        إذا (rec[i].write_buf) {
            ngx_resolver_free(r, rec[i].write_buf->پیل);
            ngx_resolver_free(r, rec[i].write_buf);
        }
    }
}


ساكن فراغ
ngx_resolver_cleanup_ونه(ngx_resolver_t *r, ngx_rbtree_t *ونه)
{
    ngx_resolver_ctx_t   *ctx, *بل;
    ngx_resolver_غوټه_t  *rn;

    طالما (ونه->ریښه != ونه->sentinel) {

        rn = ngx_resolver_غوټه(ngx_rbtree_min(ونه->ریښه, ونه->sentinel));

        ngx_کتار_remove(&rn->کتار);

        لكل (ctx = rn->انتظار; ctx; ctx = بل) {
            بل = ctx->بل;

            إذا (ctx->event) {
                إذا (ctx->event->timer_سېټ) {
                    ngx_del_timer(ctx->event);
                }

                ngx_resolver_free(r, ctx->event);
            }

            ngx_resolver_free(r, ctx);
        }

        ngx_rbtree_delete(ونه, &rn->غوټه);

        ngx_resolver_free_غوټه(r, rn);
    }
}


ngx_resolver_ctx_t *
ngx_resolve_پیل(ngx_resolver_t *r, ngx_resolver_ctx_t *لنډمهاله)
{
    دننه_addr_t            addr;
    ngx_resolver_ctx_t  *ctx;

    إذا (لنډمهاله) {
        addr = ngx_inet_addr(لنډمهاله->name.معلومات, لنډمهاله->name.len);

        إذا (addr != INADDR_NONE) {
            لنډمهاله->resolver = r;
            لنډمهاله->state = NGX_OK;
            لنډمهاله->naddrs = 1;
            لنډمهاله->addrs = &لنډمهاله->addr;
            لنډمهاله->addr.sockaddr = (بنية sockaddr *) &لنډمهاله->ساین;
            لنډمهاله->addr.socklen = حجم(بنية sockaddr_دننه);
            ngx_memzero(&لنډمهاله->ساین, حجم(بنية sockaddr_دننه));
            لنډمهاله->ساین.sin_family = AF_INET;
            لنډمهاله->ساین.sin_addr.s_addr = addr;
            لنډمهاله->چټک = 1;

            أرجع لنډمهاله;
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

    إذا (ctx->name.len > 0 && ctx->name.معلومات[ctx->name.len - 1] == '.') {
        ctx->name.len--;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لوګاریتم, 0,
                   "resolve: \"%V\"", &ctx->name);

    إذا (ctx->چټک) {
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
ngx_resolve_name_بشپړ(ngx_resolver_ctx_t *ctx)
{
    ngx_uint_t            i;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_غوټه_t  *rn;

    r = ctx->resolver;

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لوګاریتم, 0,
                   "resolve name done: %i", ctx->state);

    إذا (ctx->چټک) {
        أرجع;
    }

    إذا (ctx->event && ctx->event->timer_سېټ) {
        ngx_del_timer(ctx->event);
    }

    /* lock name mutex */

    إذا (ctx->nsrvs) {
        لكل (i = 0; i < ctx->nsrvs; i++) {
            إذا (ctx->srvs[i].ctx) {
                ngx_resolve_name_بشپړ(ctx->srvs[i].ctx);
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

        rn = ctx->غوټه;

        إذا (rn) {
            p = &rn->انتظار;
            w = rn->انتظار;

            طالما (w) {
                إذا (w == ctx) {
                    *p = w->بل;

                    اذهب بشپړ;
                }

                p = &w->بل;
                w = w->بل;
            }

            ngx_log_error(NGX_LOG_ALERT, r->لوګاریتم, 0,
                          "could not cancel %V resolving", &ctx->name);
        }
    }

بشپړ:

    إذا (ctx->service.len) {
        ngx_resolver_expire(r, &r->srv_rbtree, &r->srv_expire_کتار);

    } وإلا {
        ngx_resolver_expire(r, &r->name_rbtree, &r->name_expire_کتار);
    }

    /* unlock name mutex */

    /* lock alloc mutex */

    إذا (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    إذا (r->event->timer_سېټ && ngx_resolver_resend_تشه(r)) {
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
    ngx_کتار_t          *resend_کتار, *expire_کتار;
    ngx_rbtree_t         *ونه;
    ngx_resolver_ctx_t   *بل, *وروستی;
    ngx_resolver_addr_t  *addrs;
    ngx_resolver_غوټه_t  *rn;

    ngx_strlow(name->معلومات, name->معلومات, name->len);

    hash = ngx_دوراني_ازموینه_32_short(name->معلومات, name->len);

    إذا (ctx->service.len) {
        rn = ngx_resolver_lookup_srv(r, name, hash);

        ونه = &r->srv_rbtree;
        resend_کتار = &r->srv_resend_کتار;
        expire_کتار = &r->srv_expire_کتار;

    } وإلا {
        rn = ngx_resolver_lookup_name(r, name, hash);

        ونه = &r->name_rbtree;
        resend_کتار = &r->name_resend_کتار;
        expire_کتار = &r->name_expire_کتار;
    }

    إذا (rn) {

        /* ctx can be a list after NGX_RESOLVE_CNAME */
        لكل (وروستی = ctx; وروستی->بل; وروستی = وروستی->بل);

        إذا (rn->valid >= ngx_وخت()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->لوګاریتم, 0, "resolve cached");

            ngx_کتار_remove(&rn->کتار);

            rn->expire = ngx_وخت() + r->expire;

            ngx_کتار_ورننباسه_سر(expire_کتار, &rn->کتار);

            naddrs = (rn->naddrs == (u_short) -1) ? 0 : rn->naddrs;
#إذا (NGX_HAVE_INET6)
            naddrs += (rn->naddrs6 == (u_short) -1) ? 0 : rn->naddrs6;
#د_که_پای

            إذا (naddrs) {

                إذا (naddrs == 1 && rn->naddrs == 1) {
                    addrs = عدم;

                } وإلا {
                    addrs = ngx_resolver_export(r, rn, 1);
                    إذا (addrs == عدم) {
                        أرجع NGX_ERROR;
                    }
                }

                وروستی->بل = rn->انتظار;
                rn->انتظار = عدم;

                /* unlock name mutex */

                افعل {
                    ctx->state = NGX_OK;
                    ctx->valid = rn->valid;
                    ctx->naddrs = naddrs;

                    إذا (addrs == عدم) {
                        ctx->addrs = &ctx->addr;
                        ctx->addr.sockaddr = (بنية sockaddr *) &ctx->ساین;
                        ctx->addr.socklen = حجم(بنية sockaddr_دننه);
                        ngx_memzero(&ctx->ساین, حجم(بنية sockaddr_دننه));
                        ctx->ساین.sin_family = AF_INET;
                        ctx->ساین.sin_addr.s_addr = rn->u.addr;

                    } وإلا {
                        ctx->addrs = addrs;
                    }

                    بل = ctx->بل;

                    ctx->handler(ctx);

                    ctx = بل;
                } طالما (ctx);

                إذا (addrs != عدم) {
                    ngx_resolver_free(r, addrs->sockaddr);
                    ngx_resolver_free(r, addrs);
                }

                أرجع NGX_OK;
            }

            إذا (rn->nsrvs) {
                وروستی->بل = rn->انتظار;
                rn->انتظار = عدم;

                /* unlock name mutex */

                افعل {
                    بل = ctx->بل;

                    ngx_resolver_resolve_srv_نومونه(ctx, rn);

                    ctx = بل;
                } طالما (ctx);

                أرجع NGX_OK;
            }

            /* NGX_RESOLVE_CNAME */

            إذا (ctx->recursion++ < NGX_RESOLVER_MAX_RECURSION) {

                cname.len = rn->cnlen;
                cname.معلومات = rn->u.cname;

                أرجع ngx_resolve_name_locked(r, ctx, &cname);
            }

            وروستی->بل = rn->انتظار;
            rn->انتظار = عدم;

            /* unlock name mutex */

            افعل {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                ctx->valid = ngx_وخت() + (r->valid ? r->valid : 10);
                بل = ctx->بل;

                ctx->handler(ctx);

                ctx = بل;
            } طالما (ctx);

            أرجع NGX_OK;
        }

        إذا (rn->انتظار) {
            إذا (ngx_resolver_سېټ_timeout(r, ctx) != NGX_OK) {
                أرجع NGX_ERROR;
            }

            وروستی->بل = rn->انتظار;
            rn->انتظار = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;

            افعل {
                ctx->غوټه = rn;
                ctx = ctx->بل;
            } طالما (ctx);

            أرجع NGX_AGAIN;
        }

        ngx_کتار_remove(&rn->کتار);

        /* lock alloc mutex */

        إذا (rn->query) {
            ngx_resolver_free_locked(r, rn->query);
            rn->query = عدم;
#إذا (NGX_HAVE_INET6)
            rn->query6 = عدم;
#د_که_پای
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
#د_که_پای

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

        rn = ngx_resolver_alloc(r, حجم(ngx_resolver_غوټه_t));
        إذا (rn == عدم) {
            أرجع NGX_ERROR;
        }

        rn->name = ngx_resolver_dup(r, name->معلومات, name->len);
        إذا (rn->name == عدم) {
            ngx_resolver_free(r, rn);
            أرجع NGX_ERROR;
        }

        rn->غوټه.کلی = hash;
        rn->nlen = (u_short) name->len;
        rn->query = عدم;
#إذا (NGX_HAVE_INET6)
        rn->query6 = عدم;
#د_که_پای

        ngx_rbtree_ورننباسه(ونه, &rn->غوټه);
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
        ngx_rbtree_delete(ونه, &rn->غوټه);

        ngx_resolver_free(r, rn->query);
        ngx_resolver_free(r, rn->name);
        ngx_resolver_free(r, rn);

        افعل {
            ctx->state = NGX_RESOLVE_NXDOMAIN;
            بل = ctx->بل;

            ctx->handler(ctx);

            ctx = بل;
        } طالما (ctx);

        أرجع NGX_OK;
    }

    rn->وروستی_connection = r->وروستی_connection++;
    إذا (r->وروستی_connection == r->connections.nelts) {
        r->وروستی_connection = 0;
    }

    rn->naddrs = r->ipv4 ? (u_short) -1 : 0;
    rn->tcp = 0;
#إذا (NGX_HAVE_INET6)
    rn->naddrs6 = r->ipv6 ? (u_short) -1 : 0;
    rn->tcp6 = 0;
#د_که_پای
    rn->nsrvs = 0;

    إذا (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->وروستی_connection++;
        إذا (rn->وروستی_connection == r->connections.nelts) {
            rn->وروستی_connection = 0;
        }

        (فراغ) ngx_resolver_send_query(r, rn);
    }

    إذا (ngx_resolver_سېټ_timeout(r, ctx) != NGX_OK) {
        اذهب failed;
    }

    إذا (ngx_resolver_resend_تشه(r)) {
        ngx_جمع_کړه_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_وخت() + r->resend_timeout;

    ngx_کتار_ورننباسه_سر(resend_کتار, &rn->کتار);

    rn->کوډ = 0;
    rn->cnlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_ارزښت;
    rn->انتظار = ctx;

    ctx->state = NGX_AGAIN;
    ctx->async = 1;

    افعل {
        ctx->غوټه = rn;
        ctx = ctx->بل;
    } طالما (ctx);

    أرجع NGX_AGAIN;

failed:

    ngx_rbtree_delete(ونه, &rn->غوټه);

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
    دننه_addr_t             addr;
    ngx_کتار_t          *resend_کتار, *expire_کتار;
    ngx_rbtree_t         *ونه;
    ngx_resolver_t       *r;
    بنية sockaddr_دننه   *ساین;
    ngx_resolver_غوټه_t  *rn;
#إذا (NGX_HAVE_INET6)
    uint32_t              hash;
    بنية sockaddr_in6  *sin6;
#د_که_پای

#إذا (NGX_SUPPRESS_WARN)
    addr = 0;
#إذا (NGX_HAVE_INET6)
    hash = 0;
    sin6 = عدم;
#د_که_پای
#د_که_پای

    r = ctx->resolver;

    بدل (ctx->addr.sockaddr->sa_family) {

#إذا (NGX_HAVE_INET6)
    حالة AF_INET6:
        sin6 = (بنية sockaddr_in6 *) ctx->addr.sockaddr;
        hash = ngx_دوراني_ازموینه_32_short(sin6->sin6_addr.s6_addr, 16);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr6(r, &sin6->sin6_addr, hash);

        ونه = &r->addr6_rbtree;
        resend_کتار = &r->addr6_resend_کتار;
        expire_کتار = &r->addr6_expire_کتار;

        اكسر;
#د_که_پای

    افتراضي: /* AF_INET */
        ساین = (بنية sockaddr_دننه *) ctx->addr.sockaddr;
        addr = ntohl(ساین->sin_addr.s_addr);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        ونه = &r->addr_rbtree;
        resend_کتار = &r->addr_resend_کتار;
        expire_کتار = &r->addr_expire_کتار;
    }

    إذا (rn) {

        إذا (rn->valid >= ngx_وخت()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->لوګاریتم, 0, "resolve cached");

            ngx_کتار_remove(&rn->کتار);

            rn->expire = ngx_وخت() + r->expire;

            ngx_کتار_ورننباسه_سر(expire_کتار, &rn->کتار);

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
            إذا (ngx_resolver_سېټ_timeout(r, ctx) != NGX_OK) {
                أرجع NGX_ERROR;
            }

            ctx->بل = rn->انتظار;
            rn->انتظار = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;
            ctx->غوټه = rn;

            /* unlock addr mutex */

            أرجع NGX_OK;
        }

        ngx_کتار_remove(&rn->کتار);

        ngx_resolver_free(r, rn->query);
        rn->query = عدم;
#إذا (NGX_HAVE_INET6)
        rn->query6 = عدم;
#د_که_پای

    } وإلا {
        rn = ngx_resolver_alloc(r, حجم(ngx_resolver_غوټه_t));
        إذا (rn == عدم) {
            اذهب failed;
        }

        بدل (ctx->addr.sockaddr->sa_family) {

#إذا (NGX_HAVE_INET6)
        حالة AF_INET6:
            rn->addr6 = sin6->sin6_addr;
            rn->غوټه.کلی = hash;
            اكسر;
#د_که_پای

        افتراضي: /* AF_INET */
            rn->غوټه.کلی = addr;
        }

        rn->query = عدم;
#إذا (NGX_HAVE_INET6)
        rn->query6 = عدم;
#د_که_پای

        ngx_rbtree_ورننباسه(ونه, &rn->غوټه);
    }

    إذا (ngx_resolver_create_addr_query(r, rn, &ctx->addr) != NGX_OK) {
        اذهب failed;
    }

    rn->وروستی_connection = r->وروستی_connection++;
    إذا (r->وروستی_connection == r->connections.nelts) {
        r->وروستی_connection = 0;
    }

    rn->naddrs = (u_short) -1;
    rn->tcp = 0;
#إذا (NGX_HAVE_INET6)
    rn->naddrs6 = (u_short) -1;
    rn->tcp6 = 0;
#د_که_پای
    rn->nsrvs = 0;

    إذا (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->وروستی_connection++;
        إذا (rn->وروستی_connection == r->connections.nelts) {
            rn->وروستی_connection = 0;
        }

        (فراغ) ngx_resolver_send_query(r, rn);
    }

    إذا (ngx_resolver_سېټ_timeout(r, ctx) != NGX_OK) {
        اذهب failed;
    }

    إذا (ngx_resolver_resend_تشه(r)) {
        ngx_جمع_کړه_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_وخت() + r->resend_timeout;

    ngx_کتار_ورننباسه_سر(resend_کتار, &rn->کتار);

    rn->کوډ = 0;
    rn->cnlen = 0;
    rn->name = عدم;
    rn->nlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_ارزښت;
    rn->انتظار = ctx;

    /* unlock addr mutex */

    ctx->state = NGX_AGAIN;
    ctx->async = 1;
    ctx->غوټه = rn;

    أرجع NGX_OK;

failed:

    إذا (rn) {
        ngx_rbtree_delete(ونه, &rn->غوټه);

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
ngx_resolve_addr_بشپړ(ngx_resolver_ctx_t *ctx)
{
    ngx_کتار_t          *expire_کتار;
    ngx_rbtree_t         *ونه;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_غوټه_t  *rn;

    r = ctx->resolver;

    بدل (ctx->addr.sockaddr->sa_family) {

#إذا (NGX_HAVE_INET6)
    حالة AF_INET6:
        ونه = &r->addr6_rbtree;
        expire_کتار = &r->addr6_expire_کتار;
        اكسر;
#د_که_پای

    افتراضي: /* AF_INET */
        ونه = &r->addr_rbtree;
        expire_کتار = &r->addr_expire_کتار;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لوګاریتم, 0,
                   "resolve addr done: %i", ctx->state);

    إذا (ctx->event && ctx->event->timer_سېټ) {
        ngx_del_timer(ctx->event);
    }

    /* lock addr mutex */

    إذا (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->غوټه;

        إذا (rn) {
            p = &rn->انتظار;
            w = rn->انتظار;

            طالما (w) {
                إذا (w == ctx) {
                    *p = w->بل;

                    اذهب بشپړ;
                }

                p = &w->بل;
                w = w->بل;
            }
        }

        {
            u_char     متن[NGX_SOCKADDR_STRLEN];
            ngx_str_t  addrtext;

            addrtext.معلومات = متن;
            addrtext.len = ngx_sock_ntop(ctx->addr.sockaddr, ctx->addr.socklen,
                                         متن, NGX_SOCKADDR_STRLEN, 0);

            ngx_log_error(NGX_LOG_ALERT, r->لوګاریتم, 0,
                          "could not cancel %V resolving", &addrtext);
        }
    }

بشپړ:

    ngx_resolver_expire(r, ونه, expire_کتار);

    /* unlock addr mutex */

    /* lock alloc mutex */

    إذا (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    إذا (r->event->timer_سېټ && ngx_resolver_resend_تشه(r)) {
        ngx_del_timer(r->event);
    }
}


ساكن فراغ
ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *ونه, ngx_کتار_t *کتار)
{
    وخت_t                now;
    ngx_uint_t            i;
    ngx_کتار_t          *q;
    ngx_resolver_غوټه_t  *rn;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->لوګاریتم, 0, "resolver expire");

    now = ngx_وخت();

    لكل (i = 0; i < 2; i++) {
        إذا (ngx_کتار_تشه(کتار)) {
            أرجع;
        }

        q = ngx_کتار_وروستی(کتار);

        rn = ngx_کتار_معلومات(q, ngx_resolver_غوټه_t, کتار);

        إذا (now <= rn->expire) {
            أرجع;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->لوګاریتم, 0,
                       "resolver expire \"%*s\"", (اندازه_t) rn->nlen, rn->name);

        ngx_کتار_remove(q);

        ngx_rbtree_delete(ونه, &rn->غوټه);

        ngx_resolver_free_غوټه(r, rn);
    }
}


ساكن ngx_int_t
ngx_resolver_send_query(ngx_resolver_t *r, ngx_resolver_غوټه_t *rn)
{
    ngx_int_t                   rc;
    ngx_resolver_connection_t  *rec;

    rec = r->connections.elts;
    rec = &rec[rn->وروستی_connection];

    إذا (rec->لوګاریتم.handler == عدم) {
        rec->لوګاریتم = *r->لوګاریتم;
        rec->لوګاریتم.handler = ngx_resolver_log_error;
        rec->لوګاریتم.معلومات = rec;
        rec->لوګاریتم.action = "resolving";
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

#د_که_پای

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

    إذا ((اندازه_t) n != (اندازه_t) qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->لوګاریتم, 0, "send() incomplete");
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

            b->پیل = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_RSIZE);
            إذا (b->پیل == عدم) {
                ngx_resolver_free(r, b);
                أرجع NGX_ERROR;
            }

            b->پای_ټکی = b->پیل + NGX_RESOLVER_TCP_RSIZE;

            rec->read_buf = b;
        }

        b->pos = b->پیل;
        b->وروستی = b->پیل;

        b = rec->write_buf;

        إذا (b == عدم) {
            b = ngx_resolver_calloc(r, حجم(ngx_buf_t));
            إذا (b == عدم) {
                أرجع NGX_ERROR;
            }

            b->پیل = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_WSIZE);
            إذا (b->پیل == عدم) {
                ngx_resolver_free(r, b);
                أرجع NGX_ERROR;
            }

            b->پای_ټکی = b->پیل + NGX_RESOLVER_TCP_WSIZE;

            rec->write_buf = b;
        }

        b->pos = b->پیل;
        b->وروستی = b->پیل;

        rc = ngx_tcp_connect(rec);
        إذا (rc == NGX_ERROR) {
            أرجع NGX_ERROR;
        }

        rec->tcp->معلومات = rec;
        rec->tcp->اكتب->handler = ngx_resolver_tcp_write;
        rec->tcp->اكتب->cancelable = 1;
        rec->tcp->اقرأ->handler = ngx_resolver_tcp_read;
        rec->tcp->اقرأ->resolver = 1;

        ngx_جمع_کړه_timer(rec->tcp->اكتب, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    b = rec->write_buf;

    إذا (b->پای_ټکی - b->وروستی <  2 + qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->لوګاریتم, 0, "buffer overflow");
        أرجع NGX_ERROR;
    }

    *b->وروستی++ = (u_char) (qlen >> 8);
    *b->وروستی++ = (u_char) qlen;
    b->وروستی = ngx_cpymem(b->وروستی, query, qlen);

    إذا (rc == NGX_OK) {
        ngx_resolver_tcp_write(rec->tcp->اكتب);
    }

    أرجع NGX_OK;
}


ساكن فراغ
ngx_resolver_resend_handler(ngx_event_t *ev)
{
    وخت_t           timer, atimer, stimer, ntimer;
#إذا (NGX_HAVE_INET6)
    وخت_t           a6timer;
#د_که_پای
    ngx_resolver_t  *r;

    r = ev->معلومات;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->لوګاریتم, 0,
                   "resolver resend handler");

    /* lock name mutex */

    ntimer = ngx_resolver_resend(r, &r->name_rbtree, &r->name_resend_کتار);

    stimer = ngx_resolver_resend(r, &r->srv_rbtree, &r->srv_resend_کتار);

    /* unlock name mutex */

    /* lock addr mutex */

    atimer = ngx_resolver_resend(r, &r->addr_rbtree, &r->addr_resend_کتار);

    /* unlock addr mutex */

#إذا (NGX_HAVE_INET6)

    /* lock addr6 mutex */

    a6timer = ngx_resolver_resend(r, &r->addr6_rbtree, &r->addr6_resend_کتار);

    /* unlock addr6 mutex */

#د_که_پای

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

#د_که_پای

    إذا (timer) {
        ngx_جمع_کړه_timer(r->event, (ngx_msec_t) (timer * 1000));
    }
}


ساكن وخت_t
ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *ونه, ngx_کتار_t *کتار)
{
    وخت_t                now;
    ngx_کتار_t          *q;
    ngx_resolver_غوټه_t  *rn;

    now = ngx_وخت();

    لكل ( ;; ) {
        إذا (ngx_کتار_تشه(کتار)) {
            أرجع 0;
        }

        q = ngx_کتار_وروستی(کتار);

        rn = ngx_کتار_معلومات(q, ngx_resolver_غوټه_t, کتار);

        إذا (now < rn->expire) {
            أرجع rn->expire - now;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->لوګاریتم, 0,
                       "resolver resend \"%*s\" %p",
                       (اندازه_t) rn->nlen, rn->name, rn->انتظار);

        ngx_کتار_remove(q);

        إذا (rn->انتظار) {

            إذا (++rn->وروستی_connection == r->connections.nelts) {
                rn->وروستی_connection = 0;
            }

            (فراغ) ngx_resolver_send_query(r, rn);

            rn->expire = now + r->resend_timeout;

            ngx_کتار_ورننباسه_سر(کتار, q);

            استمر;
        }

        ngx_rbtree_delete(ونه, &rn->غوټه);

        ngx_resolver_free_غوټه(r, rn);
    }
}


ساكن ngx_uint_t
ngx_resolver_resend_تشه(ngx_resolver_t *r)
{
    أرجع ngx_کتار_تشه(&r->name_resend_کتار)
           && ngx_کتار_تشه(&r->srv_resend_کتار)
#إذا (NGX_HAVE_INET6)
           && ngx_کتار_تشه(&r->addr6_resend_کتار)
#د_که_پای
           && ngx_کتار_تشه(&r->addr_resend_کتار);
}


ساكن فراغ
ngx_resolver_udp_read(ngx_event_t *rev)
{
    ssize_t                     n;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;
    u_char                      buf[NGX_RESOLVER_UDP_اندازه];

    c = rev->معلومات;
    rec = c->معلومات;

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
    off_t                       لېږل_شوی;
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

    لېږل_شوی = c->لېږل_شوی;

    طالما (wev->ready && b->pos < b->وروستی) {
        n = ngx_send(c, b->pos, b->وروستی - b->pos);

        إذا (n == NGX_AGAIN) {
            اكسر;
        }

        إذا (n == NGX_ERROR) {
            اذهب failed;
        }

        b->pos += n;
    }

    إذا (b->pos != b->پیل) {
        b->وروستی = ngx_movemem(b->پیل, b->pos, b->وروستی - b->pos);
        b->pos = b->پیل;
    }

    إذا (c->لېږل_شوی != لېږل_شوی) {
        ngx_جمع_کړه_timer(wev, (ngx_msec_t) (r->tcp_timeout * 1000));
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

    c = rev->معلومات;
    rec = c->معلومات;
    b = rec->read_buf;
    r = rec->resolver;

    طالما (rev->ready) {
        n = ngx_recv(c, b->وروستی, b->پای_ټکی - b->وروستی);

        إذا (n == NGX_AGAIN) {
            اكسر;
        }

        إذا (n == NGX_ERROR || n == 0) {
            اذهب failed;
        }

        b->وروستی += n;

        لكل ( ;; ) {
            p = b->pos;
            اندازه = b->وروستی - p;

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

        إذا (b->pos != b->پیل) {
            b->وروستی = ngx_movemem(b->پیل, b->pos, b->وروستی - b->pos);
            b->pos = b->پیل;
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
    ngx_uint_t            i, times, ident, qident, flags, کوډ, nqs, nan, trunc,
                          qtype, qclass;
#إذا (NGX_HAVE_INET6)
    ngx_uint_t            qident6;
#د_که_پای
    ngx_کتار_t          *q;
    ngx_resolver_qs_t    *qs;
    ngx_resolver_hdr_t   *response;
    ngx_resolver_غوټه_t  *rn;

    إذا (n < حجم(ngx_resolver_hdr_t)) {
        اذهب short_response;
    }

    response = (ngx_resolver_hdr_t *) buf;

    ident = (response->ident_hi << 8) + response->ident_lo;
    flags = (response->flags_hi << 8) + response->flags_lo;
    nqs = (response->nqs_hi << 8) + response->nqs_lo;
    nan = (response->nan_hi << 8) + response->nan_lo;
    trunc = flags & 0x0200;

    ngx_log_debug6(NGX_LOG_DEBUG_CORE, r->لوګاریتم, 0,
                   "resolver DNS response %ui fl:%04Xi %ui/%ui/%ud/%ud",
                   ident, flags, nqs, nan,
                   (response->nns_hi << 8) + response->nns_lo,
                   (response->nar_hi << 8) + response->nar_lo);

    /* response to a standard query */
    إذا ((flags & 0xf870) != 0x8000 || (trunc && tcp)) {
        ngx_log_error(r->log_level, r->لوګاریتم, 0,
                      "invalid %s DNS response %ui fl:%04Xi",
                      tcp ? "TCP" : "UDP", ident, flags);
        أرجع;
    }

    کوډ = flags & 0xf;

    إذا (کوډ == NGX_RESOLVE_FORMERR) {

        times = 0;

        لكل (q = ngx_کتار_سر(&r->name_resend_کتار);
             q != ngx_کتار_sentinel(&r->name_resend_کتار) && times++ < 100;
             q = ngx_کتار_بل(q))
        {
            rn = ngx_کتار_معلومات(q, ngx_resolver_غوټه_t, کتار);

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
#د_که_پای
        }

        اذهب dns_error;
    }

    إذا (کوډ > NGX_RESOLVE_REFUSED) {
        اذهب dns_error;
    }

    إذا (nqs != 1) {
        err = "invalid number of questions in DNS response";
        اذهب بشپړ;
    }

    i = حجم(ngx_resolver_hdr_t);

    طالما (i < (ngx_uint_t) n) {

        إذا (buf[i] & 0xc0) {
            err = "unexpected compression pointer in DNS response";
            اذهب بشپړ;
        }

        إذا (buf[i] == '\0') {
            اذهب وموندل;
        }

        i += 1 + buf[i];
    }

    اذهب short_response;

وموندل:

    إذا (i++ == حجم(ngx_resolver_hdr_t)) {
        err = "zero-length domain name in DNS response";
        اذهب بشپړ;
    }

    إذا (i + حجم(ngx_resolver_qs_t) + nan * (2 + حجم(ngx_resolver_an_t))
        > (ngx_uint_t) n)
    {
        اذهب short_response;
    }

    qs = (ngx_resolver_qs_t *) &buf[i];

    qtype = (qs->type_hi << 8) + qs->type_lo;
    qclass = (qs->class_hi << 8) + qs->class_lo;

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->لوګاریتم, 0,
                   "resolver DNS response qt:%ui cl:%ui", qtype, qclass);

    إذا (qclass != 1) {
        ngx_log_error(r->log_level, r->لوګاریتم, 0,
                      "unknown query class %ui in DNS response", qclass);
        أرجع;
    }

    بدل (qtype) {

    حالة NGX_RESOLVE_A:
#إذا (NGX_HAVE_INET6)
    حالة NGX_RESOLVE_AAAA:
#د_که_پای

        ngx_resolver_process_a(r, buf, n, ident, کوډ, qtype, nan, trunc,
                               i + حجم(ngx_resolver_qs_t));

        اكسر;

    حالة NGX_RESOLVE_SRV:

        ngx_resolver_process_srv(r, buf, n, ident, کوډ, nan, trunc,
                                 i + حجم(ngx_resolver_qs_t));

        اكسر;

    حالة NGX_RESOLVE_PTR:

        ngx_resolver_process_ptr(r, buf, n, ident, کوډ, nan);

        اكسر;

    افتراضي:
        ngx_log_error(r->log_level, r->لوګاریتم, 0,
                      "unknown query type %ui in DNS response", qtype);
        أرجع;
    }

    أرجع;

short_response:

    err = "short DNS response";

بشپړ:

    ngx_log_error(r->log_level, r->لوګاریتم, 0, err);

    أرجع;

dns_error_name:

    ngx_log_error(r->log_level, r->لوګاریتم, 0,
                  "DNS error (%ui: %s), query id:%ui, name:\"%*s\"",
                  کوډ, ngx_resolver_strerror(کوډ), ident,
                  (اندازه_t) rn->nlen, rn->name);
    أرجع;

dns_error:

    ngx_log_error(r->log_level, r->لوګاریتم, 0,
                  "DNS error (%ui: %s), query id:%ui",
                  کوډ, ngx_resolver_strerror(کوډ), ident);
    أرجع;
}


ساكن فراغ
ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, اندازه_t n,
    ngx_uint_t ident, ngx_uint_t کوډ, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans)
{
    حرف                       *err;
    u_char                     *cname;
    اندازه_t                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    دننه_addr_t                  *addr;
    ngx_str_t                   name;
    ngx_uint_t                  type, class, qident, naddrs, a, i, j, پیل;
#إذا (NGX_HAVE_INET6)
    بنية in6_addr            *addr6;
#د_که_پای
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *بل;
    ngx_resolver_غوټه_t        *rn;
    ngx_resolver_addr_t        *addrs;
    ngx_resolver_connection_t  *rec;

    إذا (ngx_resolver_copy(r, &name, buf,
                          buf + حجم(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        أرجع;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لوګاریتم, 0, "resolver qs:%V", &name);

    hash = ngx_دوراني_ازموینه_32_short(name.معلومات, name.len);

    /* lock name mutex */

    rn = ngx_resolver_lookup_name(r, &name, hash);

    إذا (rn == عدم) {
        ngx_log_error(r->log_level, r->لوګاریتم, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.معلومات);
        اذهب failed;
    }

    بدل (qtype) {

#إذا (NGX_HAVE_INET6)
    حالة NGX_RESOLVE_AAAA:

        إذا (rn->query6 == عدم || rn->naddrs6 != (u_short) -1) {
            ngx_log_error(r->log_level, r->لوګاریتم, 0,
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
#د_که_پای

    افتراضي: /* NGX_RESOLVE_A */

        إذا (rn->query == عدم || rn->naddrs != (u_short) -1) {
            ngx_log_error(r->log_level, r->لوګاریتم, 0,
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
        ngx_log_error(r->log_level, r->لوګاریتم, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.معلومات);
        اذهب failed;
    }

    ngx_resolver_free(r, name.معلومات);

    إذا (trunc) {

        ngx_کتار_remove(&rn->کتار);

        إذا (rn->انتظار == عدم) {
            ngx_rbtree_delete(&r->name_rbtree, &rn->غوټه);
            ngx_resolver_free_غوټه(r, rn);
            اذهب بل;
        }

        rec = r->connections.elts;
        rec = &rec[rn->وروستی_connection];

        بدل (qtype) {

#إذا (NGX_HAVE_INET6)
        حالة NGX_RESOLVE_AAAA:

            rn->tcp6 = 1;

            (فراغ) ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen);

            اكسر;
#د_که_پای

        افتراضي: /* NGX_RESOLVE_A */

            rn->tcp = 1;

            (فراغ) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);
        }

        rn->expire = ngx_وخت() + r->resend_timeout;

        ngx_کتار_ورننباسه_سر(&r->name_resend_کتار, &rn->کتار);

        اذهب بل;
    }

    إذا (کوډ == 0 && rn->کوډ) {
        کوډ = rn->کوډ;
    }

    إذا (کوډ == 0 && nan == 0) {

#إذا (NGX_HAVE_INET6)
        بدل (qtype) {

        حالة NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            إذا (rn->naddrs == (u_short) -1) {
                اذهب بل;
            }

            إذا (rn->naddrs) {
                اذهب export;
            }

            اكسر;

        افتراضي: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            إذا (rn->naddrs6 == (u_short) -1) {
                اذهب بل;
            }

            إذا (rn->naddrs6) {
                اذهب export;
            }
        }
#د_که_پای

        کوډ = NGX_RESOLVE_NXDOMAIN;
    }

    إذا (کوډ) {

#إذا (NGX_HAVE_INET6)
        بدل (qtype) {

        حالة NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            إذا (rn->naddrs == (u_short) -1) {
                rn->کوډ = (u_char) کوډ;
                اذهب بل;
            }

            اكسر;

        افتراضي: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            إذا (rn->naddrs6 == (u_short) -1) {
                rn->کوډ = (u_char) کوډ;
                اذهب بل;
            }
        }
#د_که_پای

        بل = rn->انتظار;
        rn->انتظار = عدم;

        ngx_کتار_remove(&rn->کتار);

        ngx_rbtree_delete(&r->name_rbtree, &rn->غوټه);

        /* unlock name mutex */

        طالما (بل) {
            ctx = بل;
            ctx->state = کوډ;
            ctx->valid = ngx_وخت() + (r->valid ? r->valid : 10);
            بل = ctx->بل;

            ctx->handler(ctx);
        }

        ngx_resolver_free_غوټه(r, rn);

        أرجع;
    }

    i = ans;
    naddrs = 0;
    cname = عدم;

    لكل (a = 0; a < nan; a++) {

        پیل = i;

        طالما (i < n) {

            إذا (buf[i] & 0xc0) {
                i += 2;
                اذهب وموندل;
            }

            إذا (buf[i] == 0) {
                i++;
                اذهب test_اوږدتیا;
            }

            i += 1 + buf[i];
        }

        اذهب short_response;

    test_اوږدتیا:

        إذا (i - پیل < 2) {
            err = "invalid name in DNS response";
            اذهب invalid;
        }

    وموندل:

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
            ngx_log_error(r->log_level, r->لوګاریتم, 0,
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
#د_که_پای

        حالة NGX_RESOLVE_CNAME:

            cname = &buf[i];

            اكسر;

        حالة NGX_RESOLVE_DNAME:

            اكسر;

        افتراضي:

            ngx_log_error(r->log_level, r->لوګاریتم, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->لوګاریتم, 0,
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
#د_که_پای

            اكسر;
#د_که_پای

        افتراضي: /* NGX_RESOLVE_A */

            إذا (naddrs == 1) {
                addr = &rn->u.addr;
                rn->naddrs = 1;

            } وإلا {
                addr = ngx_resolver_alloc(r, naddrs * حجم(دننه_addr_t));
                إذا (addr == عدم) {
                    اذهب failed;
                }

                rn->u.addrs = addr;
                rn->naddrs = (u_short) naddrs;
            }

#إذا (NGX_HAVE_INET6 && NGX_SUPPRESS_WARN)
            addr6 = عدم;
#د_که_پای
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
                        اذهب بل;
                    }
#د_که_پای

                    اكسر;
                }
            }

#إذا (NGX_HAVE_INET6)
            وإلا إذا (type == NGX_RESOLVE_AAAA) {

                ngx_memcpy(addr6[j].s6_addr, &buf[i], 16);

                إذا (++j == naddrs) {

                    إذا (rn->naddrs == (u_short) -1) {
                        اذهب بل;
                    }

                    اكسر;
                }
            }
#د_که_پای

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
#د_که_پای

    افتراضي: /* NGX_RESOLVE_A */

        إذا (rn->naddrs == (u_short) -1) {
            rn->naddrs = 0;
        }
    }

    إذا (rn->naddrs != (u_short) -1
#إذا (NGX_HAVE_INET6)
        && rn->naddrs6 != (u_short) -1
#د_که_پای
        && rn->naddrs
#إذا (NGX_HAVE_INET6)
           + rn->naddrs6
#د_که_پای
           > 0)
    {

#إذا (NGX_HAVE_INET6)
    export:
#د_که_پای

        naddrs = rn->naddrs;
#إذا (NGX_HAVE_INET6)
        naddrs += rn->naddrs6;
#د_که_پای

        إذا (naddrs == 1 && rn->naddrs == 1) {
            addrs = عدم;

        } وإلا {
            addrs = ngx_resolver_export(r, rn, 0);
            إذا (addrs == عدم) {
                اذهب failed;
            }
        }

        ngx_کتار_remove(&rn->کتار);

        rn->valid = ngx_وخت() + (r->valid ? r->valid : (وخت_t) rn->ttl);
        rn->expire = ngx_وخت() + r->expire;

        ngx_کتار_ورننباسه_سر(&r->name_expire_کتار, &rn->کتار);

        بل = rn->انتظار;
        rn->انتظار = عدم;

        /* unlock name mutex */

        طالما (بل) {
            ctx = بل;
            ctx->state = NGX_OK;
            ctx->valid = rn->valid;
            ctx->naddrs = naddrs;

            إذا (addrs == عدم) {
                ctx->addrs = &ctx->addr;
                ctx->addr.sockaddr = (بنية sockaddr *) &ctx->ساین;
                ctx->addr.socklen = حجم(بنية sockaddr_دننه);
                ngx_memzero(&ctx->ساین, حجم(بنية sockaddr_دننه));
                ctx->ساین.sin_family = AF_INET;
                ctx->ساین.sin_addr.s_addr = rn->u.addr;

            } وإلا {
                ctx->addrs = addrs;
            }

            بل = ctx->بل;

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
#د_که_پای

        أرجع;
    }

    إذا (cname) {

        /* CNAME only */

        إذا (rn->naddrs == (u_short) -1
#إذا (NGX_HAVE_INET6)
            || rn->naddrs6 == (u_short) -1
#د_که_پای
            )
        {
            اذهب بل;
        }

        إذا (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            اذهب failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لوګاریتم, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_کتار_remove(&rn->کتار);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.معلومات;

        rn->valid = ngx_وخت() + (r->valid ? r->valid : (وخت_t) rn->ttl);
        rn->expire = ngx_وخت() + r->expire;

        ngx_کتار_ورننباسه_سر(&r->name_expire_کتار, &rn->کتار);

        ngx_resolver_free(r, rn->query);
        rn->query = عدم;
#إذا (NGX_HAVE_INET6)
        rn->query6 = عدم;
#د_که_پای

        ctx = rn->انتظار;
        rn->انتظار = عدم;

        إذا (ctx) {

            إذا (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                افعل {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    بل = ctx->بل;

                    ctx->handler(ctx);

                    ctx = بل;
                } طالما (ctx);

                أرجع;
            }

            لكل (بل = ctx; بل; بل = بل->بل) {
                بل->غوټه = عدم;
            }

            (فراغ) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        أرجع;
    }

    ngx_log_error(r->log_level, r->لوګاریتم, 0,
                  "no A or CNAME types in DNS response");
    أرجع;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->لوګاریتم, 0, err);

    أرجع;

failed:

بل:

    /* unlock name mutex */

    أرجع;
}


ساكن فراغ
ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, اندازه_t n,
    ngx_uint_t ident, ngx_uint_t کوډ, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans)
{
    حرف                       *err;
    u_char                     *cname;
    اندازه_t                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    ngx_str_t                   name;
    ngx_uint_t                  type, qident, class, پیل, nsrvs, a, i, j;
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *بل;
    ngx_resolver_srv_t         *srvs;
    ngx_resolver_غوټه_t        *rn;
    ngx_resolver_connection_t  *rec;

    إذا (ngx_resolver_copy(r, &name, buf,
                          buf + حجم(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        أرجع;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لوګاریتم, 0, "resolver qs:%V", &name);

    hash = ngx_دوراني_ازموینه_32_short(name.معلومات, name.len);

    rn = ngx_resolver_lookup_srv(r, &name, hash);

    إذا (rn == عدم || rn->query == عدم) {
        ngx_log_error(r->log_level, r->لوګاریتم, 0,
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
        ngx_log_error(r->log_level, r->لوګاریتم, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.معلومات);
        اذهب failed;
    }

    ngx_resolver_free(r, name.معلومات);

    إذا (trunc) {

        ngx_کتار_remove(&rn->کتار);

        إذا (rn->انتظار == عدم) {
            ngx_rbtree_delete(&r->srv_rbtree, &rn->غوټه);
            ngx_resolver_free_غوټه(r, rn);
            أرجع;
        }

        rec = r->connections.elts;
        rec = &rec[rn->وروستی_connection];

        rn->tcp = 1;

        (فراغ) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);

        rn->expire = ngx_وخت() + r->resend_timeout;

        ngx_کتار_ورننباسه_سر(&r->srv_resend_کتار, &rn->کتار);

        أرجع;
    }

    إذا (کوډ == 0 && rn->کوډ) {
        کوډ = rn->کوډ;
    }

    إذا (کوډ == 0 && nan == 0) {
        کوډ = NGX_RESOLVE_NXDOMAIN;
    }

    إذا (کوډ) {
        بل = rn->انتظار;
        rn->انتظار = عدم;

        ngx_کتار_remove(&rn->کتار);

        ngx_rbtree_delete(&r->srv_rbtree, &rn->غوټه);

        طالما (بل) {
            ctx = بل;
            ctx->state = کوډ;
            ctx->valid = ngx_وخت() + (r->valid ? r->valid : 10);
            بل = ctx->بل;

            ctx->handler(ctx);
        }

        ngx_resolver_free_غوټه(r, rn);

        أرجع;
    }

    i = ans;
    nsrvs = 0;
    cname = عدم;

    لكل (a = 0; a < nan; a++) {

        پیل = i;

        طالما (i < n) {

            إذا (buf[i] & 0xc0) {
                i += 2;
                اذهب وموندل;
            }

            إذا (buf[i] == 0) {
                i++;
                اذهب test_اوږدتیا;
            }

            i += 1 + buf[i];
        }

        اذهب short_response;

    test_اوږدتیا:

        إذا (i - پیل < 2) {
            err = "invalid name DNS response";
            اذهب invalid;
        }

    وموندل:

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
            ngx_log_error(r->log_level, r->لوګاریتم, 0,
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

            ngx_log_error(r->log_level, r->لوګاریتم, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->لوګاریتم, 0,
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

                srvs[j].لومړیتوب = (buf[i] << 8) + buf[i + 1];
                srvs[j].وزن = (buf[i + 2] << 8) + buf[i + 3];

                إذا (srvs[j].وزن == 0) {
                    srvs[j].وزن = 1;
                }

                srvs[j].پورټ = (buf[i + 4] << 8) + buf[i + 5];

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

        ngx_ترتیبول(srvs, nsrvs, حجم(ngx_resolver_srv_t),
                 ngx_resolver_cmp_srvs);

        ngx_resolver_free(r, rn->query);
        rn->query = عدم;

        ngx_کتار_remove(&rn->کتار);

        rn->valid = ngx_وخت() + (r->valid ? r->valid : (وخت_t) rn->ttl);
        rn->expire = ngx_وخت() + r->expire;

        ngx_کتار_ورننباسه_سر(&r->srv_expire_کتار, &rn->کتار);

        بل = rn->انتظار;
        rn->انتظار = عدم;

        طالما (بل) {
            ctx = بل;
            بل = ctx->بل;

            ngx_resolver_resolve_srv_نومونه(ctx, rn);
        }

        أرجع;
    }

    rn->nsrvs = 0;

    إذا (cname) {

        /* CNAME only */

        إذا (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            اذهب failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لوګاریتم, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_کتار_remove(&rn->کتار);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.معلومات;

        rn->valid = ngx_وخت() + (r->valid ? r->valid : (وخت_t) rn->ttl);
        rn->expire = ngx_وخت() + r->expire;

        ngx_کتار_ورننباسه_سر(&r->srv_expire_کتار, &rn->کتار);

        ngx_resolver_free(r, rn->query);
        rn->query = عدم;
#إذا (NGX_HAVE_INET6)
        rn->query6 = عدم;
#د_که_پای

        ctx = rn->انتظار;
        rn->انتظار = عدم;

        إذا (ctx) {

            إذا (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                افعل {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    بل = ctx->بل;

                    ctx->handler(ctx);

                    ctx = بل;
                } طالما (ctx);

                أرجع;
            }

            لكل (بل = ctx; بل; بل = بل->بل) {
                بل->غوټه = عدم;
            }

            (فراغ) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        أرجع;
    }

    ngx_log_error(r->log_level, r->لوګاریتم, 0, "no SRV type in DNS response");

    أرجع;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->لوګاریتم, 0, err);

    أرجع;

failed:

    /* unlock name mutex */

    أرجع;
}


ساكن فراغ
ngx_resolver_resolve_srv_نومونه(ngx_resolver_ctx_t *ctx, ngx_resolver_غوټه_t *rn)
{
    ngx_uint_t                i;
    ngx_resolver_t           *r;
    ngx_resolver_ctx_t       *cctx;
    ngx_resolver_srv_name_t  *srvs;

    r = ctx->resolver;

    ctx->غوټه = عدم;
    ctx->state = NGX_OK;
    ctx->valid = rn->valid;
    ctx->شمېر = rn->nsrvs;

    srvs = ngx_resolver_calloc(r, rn->nsrvs * حجم(ngx_resolver_srv_name_t));
    إذا (srvs == عدم) {
        اذهب failed;
    }

    ctx->srvs = srvs;
    ctx->nsrvs = rn->nsrvs;

    إذا (ctx->event && ctx->event->timer_سېټ) {
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

        cctx = ngx_resolve_پیل(r, عدم);
        إذا (cctx == عدم) {
            اذهب failed;
        }

        cctx->name = srvs[i].name;
        cctx->handler = ngx_resolver_srv_نومونه_handler;
        cctx->معلومات = ctx;
        cctx->srvs = &srvs[i];
        cctx->timeout = ctx->timeout;

        srvs[i].لومړیتوب = rn->u.srvs[i].لومړیتوب;
        srvs[i].وزن = rn->u.srvs[i].وزن;
        srvs[i].پورټ = rn->u.srvs[i].پورټ;
        srvs[i].ctx = cctx;

        إذا (ngx_resolve_name(cctx) == NGX_ERROR) {
            srvs[i].ctx = عدم;
            اذهب failed;
        }
    }

    أرجع;

failed:

    ctx->state = NGX_ERROR;
    ctx->valid = ngx_وخت() + (r->valid ? r->valid : 10);

    ctx->handler(ctx);
}


ساكن فراغ
ngx_resolver_srv_نومونه_handler(ngx_resolver_ctx_t *cctx)
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

    ctx->شمېر--;
    ctx->async |= cctx->async;

    srv->ctx = عدم;
    srv->state = cctx->state;

    إذا (cctx->naddrs) {

        ctx->valid = ngx_min(ctx->valid, cctx->valid);

        addrs = ngx_resolver_calloc(r, cctx->naddrs * حجم(ngx_addr_t));
        إذا (addrs == عدم) {
            srv->state = NGX_ERROR;
            اذهب بشپړ;
        }

        sockaddr = ngx_resolver_alloc(r, cctx->naddrs * حجم(ngx_sockaddr_t));
        إذا (sockaddr == عدم) {
            ngx_resolver_free(r, addrs);
            srv->state = NGX_ERROR;
            اذهب بشپړ;
        }

        لكل (i = 0; i < cctx->naddrs; i++) {
            addrs[i].sockaddr = &sockaddr[i].sockaddr;
            addrs[i].socklen = cctx->addrs[i].socklen;

            ngx_memcpy(&sockaddr[i], cctx->addrs[i].sockaddr,
                       addrs[i].socklen);

            ngx_inet_سېټ_پورټ(addrs[i].sockaddr, srv->پورټ);
        }

        srv->addrs = addrs;
        srv->naddrs = cctx->naddrs;
    }

بشپړ:

    ngx_resolve_name_بشپړ(cctx);

    إذا (ctx->شمېر == 0) {
        ngx_resolver_report_srv(r, ctx);
    }
}


ساكن فراغ
ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, اندازه_t n,
    ngx_uint_t ident, ngx_uint_t کوډ, ngx_uint_t nan)
{
    حرف                 *err;
    اندازه_t                len;
    دننه_addr_t             addr;
    int32_t               ttl;
    ngx_int_t             octet;
    ngx_str_t             name;
    ngx_uint_t            ماسک, type, class, qident, a, i, پیل;
    ngx_کتار_t          *expire_کتار;
    ngx_rbtree_t         *ونه;
    ngx_resolver_an_t    *an;
    ngx_resolver_ctx_t   *ctx, *بل;
    ngx_resolver_غوټه_t  *rn;
#إذا (NGX_HAVE_INET6)
    uint32_t              hash;
    ngx_int_t             digit;
    بنية in6_addr       addr6;
#د_که_پای

    إذا (ngx_resolver_copy(r, &name, buf,
                          buf + حجم(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        أرجع;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لوګاریتم, 0, "resolver qs:%V", &name);

    /* AF_INET */

    addr = 0;
    i = حجم(ngx_resolver_hdr_t);

    لكل (ماسک = 0; ماسک < 32; ماسک += 8) {
        len = buf[i++];

        octet = ngx_atoi(&buf[i], len);
        إذا (octet == NGX_ERROR || octet > 255) {
            اذهب invalid_دننه_addr_arpa;
        }

        addr += octet << ماسک;
        i += len;
    }

    إذا (ngx_strcasecmp(&buf[i], (u_char *) "\7in-addr\4arpa") == 0) {
        i += حجم("\7in-addr\4arpa");

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        ونه = &r->addr_rbtree;
        expire_کتار = &r->addr_expire_کتار;

        اذهب valid;
    }

invalid_دننه_addr_arpa:

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

        hash = ngx_دوراني_ازموینه_32_short(addr6.s6_addr, 16);
        rn = ngx_resolver_lookup_addr6(r, &addr6, hash);

        ونه = &r->addr6_rbtree;
        expire_کتار = &r->addr6_expire_کتار;

        اذهب valid;
    }

invalid_ip6_arpa:
#د_که_پای

    ngx_log_error(r->log_level, r->لوګاریتم, 0,
                  "invalid in-addr.arpa or ip6.arpa name in DNS response");
    ngx_resolver_free(r, name.معلومات);
    أرجع;

valid:

    إذا (rn == عدم || rn->query == عدم) {
        ngx_log_error(r->log_level, r->لوګاریتم, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.معلومات);
        اذهب failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    إذا (ident != qident) {
        ngx_log_error(r->log_level, r->لوګاریتم, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.معلومات);
        اذهب failed;
    }

    ngx_resolver_free(r, name.معلومات);

    إذا (کوډ == 0 && nan == 0) {
        کوډ = NGX_RESOLVE_NXDOMAIN;
    }

    إذا (کوډ) {
        بل = rn->انتظار;
        rn->انتظار = عدم;

        ngx_کتار_remove(&rn->کتار);

        ngx_rbtree_delete(ونه, &rn->غوټه);

        /* unlock addr mutex */

        طالما (بل) {
            ctx = بل;
            ctx->state = کوډ;
            ctx->valid = ngx_وخت() + (r->valid ? r->valid : 10);
            بل = ctx->بل;

            ctx->handler(ctx);
        }

        ngx_resolver_free_غوټه(r, rn);

        أرجع;
    }

    i += حجم(ngx_resolver_qs_t);

    لكل (a = 0; a < nan; a++) {

        پیل = i;

        طالما (i < n) {

            إذا (buf[i] & 0xc0) {
                i += 2;
                اذهب وموندل;
            }

            إذا (buf[i] == 0) {
                i++;
                اذهب test_اوږدتیا;
            }

            i += 1 + buf[i];
        }

        اذهب short_response;

    test_اوږدتیا:

        إذا (i - پیل < 2) {
            err = "invalid name in DNS response";
            اذهب invalid;
        }

    وموندل:

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
            ngx_log_error(r->log_level, r->لوګاریتم, 0,
                          "unexpected RR class %ui in DNS response", class);
            اذهب failed;
        }

        إذا (ttl < 0) {
            ttl = 0;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->لوګاریتم, 0,
                      "resolver qt:%ui cl:%ui len:%uz",
                      type, class, len);

        i += حجم(ngx_resolver_an_t);

        بدل (type) {

        حالة NGX_RESOLVE_PTR:

            اذهب ptr;

        حالة NGX_RESOLVE_CNAME:

            اكسر;

        افتراضي:

            ngx_log_error(r->log_level, r->لوګاریتم, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->لوګاریتم, 0,
                  "no PTR type in DNS response");
    أرجع;

ptr:

    إذا (ngx_resolver_copy(r, &name, buf, buf + i, buf + n) != NGX_OK) {
        اذهب failed;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لوګاریتم, 0, "resolver an:%V", &name);

    إذا (name.len != (اندازه_t) rn->nlen
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

    ngx_کتار_remove(&rn->کتار);

    rn->valid = ngx_وخت() + (r->valid ? r->valid : ttl);
    rn->expire = ngx_وخت() + r->expire;

    ngx_کتار_ورننباسه_سر(expire_کتار, &rn->کتار);

    بل = rn->انتظار;
    rn->انتظار = عدم;

    /* unlock addr mutex */

    طالما (بل) {
        ctx = بل;
        ctx->state = NGX_OK;
        ctx->valid = rn->valid;
        ctx->name = name;
        بل = ctx->بل;

        ctx->handler(ctx);
    }

    ngx_resolver_free(r, name.معلومات);

    أرجع;

short_response:

    err = "short DNS response";

invalid:

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->لوګاریتم, 0, err);

    أرجع;

failed:

    /* unlock addr mutex */

    أرجع;
}


ساكن ngx_resolver_غوټه_t *
ngx_resolver_lookup_name(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_غوټه_t    *غوټه, *sentinel;
    ngx_resolver_غوټه_t  *rn;

    غوټه = r->name_rbtree.ریښه;
    sentinel = r->name_rbtree.sentinel;

    طالما (غوټه != sentinel) {

        إذا (hash < غوټه->کلی) {
            غوټه = غوټه->کیڼ;
            استمر;
        }

        إذا (hash > غوټه->کلی) {
            غوټه = غوټه->ښی;
            استمر;
        }

        /* hash == node->key */

        rn = ngx_resolver_غوټه(غوټه);

        rc = ngx_memn2cmp(name->معلومات, rn->name, name->len, rn->nlen);

        إذا (rc == 0) {
            أرجع rn;
        }

        غوټه = (rc < 0) ? غوټه->کیڼ : غوټه->ښی;
    }

    /* not found */

    أرجع عدم;
}


ساكن ngx_resolver_غوټه_t *
ngx_resolver_lookup_srv(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_غوټه_t    *غوټه, *sentinel;
    ngx_resolver_غوټه_t  *rn;

    غوټه = r->srv_rbtree.ریښه;
    sentinel = r->srv_rbtree.sentinel;

    طالما (غوټه != sentinel) {

        إذا (hash < غوټه->کلی) {
            غوټه = غوټه->کیڼ;
            استمر;
        }

        إذا (hash > غوټه->کلی) {
            غوټه = غوټه->ښی;
            استمر;
        }

        /* hash == node->key */

        rn = ngx_resolver_غوټه(غوټه);

        rc = ngx_memn2cmp(name->معلومات, rn->name, name->len, rn->nlen);

        إذا (rc == 0) {
            أرجع rn;
        }

        غوټه = (rc < 0) ? غوټه->کیڼ : غوټه->ښی;
    }

    /* not found */

    أرجع عدم;
}


ساكن ngx_resolver_غوټه_t *
ngx_resolver_lookup_addr(ngx_resolver_t *r, دننه_addr_t addr)
{
    ngx_rbtree_غوټه_t  *غوټه, *sentinel;

    غوټه = r->addr_rbtree.ریښه;
    sentinel = r->addr_rbtree.sentinel;

    طالما (غوټه != sentinel) {

        إذا (addr < غوټه->کلی) {
            غوټه = غوټه->کیڼ;
            استمر;
        }

        إذا (addr > غوټه->کلی) {
            غوټه = غوټه->ښی;
            استمر;
        }

        /* addr == node->key */

        أرجع ngx_resolver_غوټه(غوټه);
    }

    /* not found */

    أرجع عدم;
}


#إذا (NGX_HAVE_INET6)

ساكن ngx_resolver_غوټه_t *
ngx_resolver_lookup_addr6(ngx_resolver_t *r, بنية in6_addr *addr,
    uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_غوټه_t    *غوټه, *sentinel;
    ngx_resolver_غوټه_t  *rn;

    غوټه = r->addr6_rbtree.ریښه;
    sentinel = r->addr6_rbtree.sentinel;

    طالما (غوټه != sentinel) {

        إذا (hash < غوټه->کلی) {
            غوټه = غوټه->کیڼ;
            استمر;
        }

        إذا (hash > غوټه->کلی) {
            غوټه = غوټه->ښی;
            استمر;
        }

        /* hash == node->key */

        rn = ngx_resolver_غوټه(غوټه);

        rc = ngx_memcmp(addr, &rn->addr6, 16);

        إذا (rc == 0) {
            أرجع rn;
        }

        غوټه = (rc < 0) ? غوټه->کیڼ : غوټه->ښی;
    }

    /* not found */

    أرجع عدم;
}

#د_که_پای


ساكن فراغ
ngx_resolver_rbtree_ورننباسه_ارزښت(ngx_rbtree_غوټه_t *لنډمهاله,
    ngx_rbtree_غوټه_t *غوټه, ngx_rbtree_غوټه_t *sentinel)
{
    ngx_rbtree_غوټه_t    **p;
    ngx_resolver_غوټه_t   *rn, *rn_لنډمهاله;

    لكل ( ;; ) {

        إذا (غوټه->کلی < لنډمهاله->کلی) {

            p = &لنډمهاله->کیڼ;

        } وإلا إذا (غوټه->کلی > لنډمهاله->کلی) {

            p = &لنډمهاله->ښی;

        } وإلا { /* node->key == temp->key */

            rn = ngx_resolver_غوټه(غوټه);
            rn_لنډمهاله = ngx_resolver_غوټه(لنډمهاله);

            p = (ngx_memn2cmp(rn->name, rn_لنډمهاله->name, rn->nlen, rn_لنډمهاله->nlen)
                 < 0) ? &لنډمهاله->کیڼ : &لنډمهاله->ښی;
        }

        إذا (*p == sentinel) {
            اكسر;
        }

        لنډمهاله = *p;
    }

    *p = غوټه;
    غوټه->پلار = لنډمهاله;
    غوټه->کیڼ = sentinel;
    غوټه->ښی = sentinel;
    ngx_rbt_red(غوټه);
}


#إذا (NGX_HAVE_INET6)

ساكن فراغ
ngx_resolver_rbtree_ورننباسه_addr6_ارزښت(ngx_rbtree_غوټه_t *لنډمهاله,
    ngx_rbtree_غوټه_t *غوټه, ngx_rbtree_غوټه_t *sentinel)
{
    ngx_rbtree_غوټه_t    **p;
    ngx_resolver_غوټه_t   *rn, *rn_لنډمهاله;

    لكل ( ;; ) {

        إذا (غوټه->کلی < لنډمهاله->کلی) {

            p = &لنډمهاله->کیڼ;

        } وإلا إذا (غوټه->کلی > لنډمهاله->کلی) {

            p = &لنډمهاله->ښی;

        } وإلا { /* node->key == temp->key */

            rn = ngx_resolver_غوټه(غوټه);
            rn_لنډمهاله = ngx_resolver_غوټه(لنډمهاله);

            p = (ngx_memcmp(&rn->addr6, &rn_لنډمهاله->addr6, 16)
                 < 0) ? &لنډمهاله->کیڼ : &لنډمهاله->ښی;
        }

        إذا (*p == sentinel) {
            اكسر;
        }

        لنډمهاله = *p;
    }

    *p = غوټه;
    غوټه->پلار = لنډمهاله;
    غوټه->کیڼ = sentinel;
    غوټه->ښی = sentinel;
    ngx_rbt_red(غوټه);
}

#د_که_پای


ساكن ngx_int_t
ngx_resolver_create_name_query(ngx_resolver_t *r, ngx_resolver_غوټه_t *rn,
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
#د_که_پای
    إذا (p == عدم) {
        أرجع NGX_ERROR;
    }

    rn->qlen = (u_short) len;
    rn->query = p;

#إذا (NGX_HAVE_INET6)
    إذا (r->ipv6) {
        rn->query6 = r->ipv4 ? (p + len) : p;
    }
#د_که_پای

    query = (ngx_resolver_hdr_t *) p;

    إذا (r->ipv4) {
        ident = ngx_random();

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->لوګاریتم, 0,
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

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->لوګاریتم, 0,
                   "resolve: \"%V\" AAAA %i", name, ident & 0xffff);

    query->ident_hi = (u_char) ((ident >> 8) & 0xff);
    query->ident_lo = (u_char) (ident & 0xff);

    p += حجم(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    qs->type_lo = NGX_RESOLVE_AAAA;
#د_که_پای

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_resolver_create_srv_query(ngx_resolver_t *r, ngx_resolver_غوټه_t *rn,
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

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->لوګاریتم, 0,
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
ngx_resolver_create_addr_query(ngx_resolver_t *r, ngx_resolver_غوټه_t *rn,
    ngx_resolver_addr_t *addr)
{
    u_char               *p, *d;
    اندازه_t                len;
    دننه_addr_t             inaddr;
    ngx_int_t             n;
    ngx_uint_t            ident;
    ngx_resolver_hdr_t   *query;
    بنية sockaddr_دننه   *ساین;
#إذا (NGX_HAVE_INET6)
    بنية sockaddr_in6  *sin6;
#د_که_پای

    بدل (addr->sockaddr->sa_family) {

#إذا (NGX_HAVE_INET6)
    حالة AF_INET6:
        len = حجم(ngx_resolver_hdr_t)
              + 64 + حجم(".ip6.arpa.") - 1
              + حجم(ngx_resolver_qs_t);

        اكسر;
#د_که_پای

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
#د_که_پای

    افتراضي: /* AF_INET */

        ساین = (بنية sockaddr_دننه *) addr->sockaddr;
        inaddr = ntohl(ساین->sin_addr.s_addr);

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
    u_char *وروستی)
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
            اذهب بشپړ;
        }

        إذا (n & 0xc0) {
            إذا ((n & 0xc0) != 0xc0) {
                err = "invalid label type in DNS response";
                اذهب invalid;
            }

            إذا (p >= وروستی) {
                err = "name is out of DNS response";
                اذهب invalid;
            }

            n = ((n & 0x3f) << 8) + *p;
            p = &buf[n];

        } وإلا {
            len += 1 + n;
            p = &p[n];
        }

        إذا (p >= وروستی) {
            err = "name is out of DNS response";
            اذهب invalid;
        }
    }

    err = "compression pointers loop in DNS response";

invalid:

    ngx_log_error(r->log_level, r->لوګاریتم, 0, err);

    أرجع NGX_ERROR;

بشپړ:

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
ngx_resolver_سېټ_timeout(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx)
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
    ctx->event->لوګاریتم = r->لوګاریتم;
    ctx->event->cancelable = ctx->cancelable;
    ctx->ident = -1;

    ngx_جمع_کړه_timer(ctx->event, ctx->timeout);

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
ngx_resolver_free_غوټه(ngx_resolver_t *r, ngx_resolver_غوټه_t *rn)
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
#د_که_پای

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
ngx_resolver_alloc(ngx_resolver_t *r, اندازه_t اندازه)
{
    u_char  *p;

    /* lock alloc mutex */

    p = ngx_alloc(اندازه, r->لوګاریتم);

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
ngx_resolver_export(ngx_resolver_t *r, ngx_resolver_غوټه_t *rn,
    ngx_uint_t rotate)
{
    ngx_uint_t            d, i, j, n;
    دننه_addr_t            *addr;
    ngx_sockaddr_t       *sockaddr;
    بنية sockaddr_دننه   *ساین;
    ngx_resolver_addr_t  *dst;
#إذا (NGX_HAVE_INET6)
    بنية in6_addr      *addr6;
    بنية sockaddr_in6  *sin6;
#د_که_پای

    n = rn->naddrs;
#إذا (NGX_HAVE_INET6)
    n += rn->naddrs6;
#د_که_پای

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
            ساین = &sockaddr[d].sockaddr_دننه;
            ساین->sin_family = AF_INET;
            ساین->sin_addr.s_addr = addr[j++];
            dst[d].sockaddr = (بنية sockaddr *) ساین;
            dst[d++].socklen = حجم(بنية sockaddr_دننه);

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
#د_که_پای

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
            ctx->valid = ngx_وخت() + (r->valid ? r->valid : 10);

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

        ctx->valid = ngx_وخت() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        أرجع;
    }

    addrs = ngx_resolver_calloc(r, naddrs * حجم(ngx_resolver_addr_t));
    إذا (addrs == عدم) {
        ctx->state = NGX_ERROR;
        ctx->valid = ngx_وخت() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        أرجع;
    }

    i = 0;
    n = 0;

    افعل {
        nw = 0;

        لكل (j = i; j < nsrvs; j++) {
            إذا (srvs[j].لومړیتوب != srvs[i].لومړیتوب) {
                اكسر;
            }

            nw += srvs[j].naddrs * srvs[j].وزن;
        }

        إذا (nw == 0) {
            اذهب بل_srv;
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
                addrs[n].لومړیتوب = srvs[k].لومړیتوب;
                addrs[n].وزن = srvs[k].وزن;
                n++;
            }

            إذا (++k == j) {
                k = i;
            }
        }

بل_srv:

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
ngx_resolver_log_error(ngx_log_t *لوګاریتم, u_char *buf, اندازه_t len)
{
    u_char                     *p;
    ngx_resolver_connection_t  *rec;

    p = buf;

    إذا (لوګاریتم->action) {
        p = ngx_snprintf(buf, len, " while %s", لوګاریتم->action);
        len -= p - buf;
    }

    rec = لوګاریتم->معلومات;

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

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->لوګاریتم, 0, "UDP socket %d", s);

    إذا (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->لوګاریتم, ngx_socket_errno,
                      ngx_socket_n " failed");
        أرجع NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->لوګاریتم);

    إذا (c == عدم) {
        إذا (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->لوګاریتم, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        أرجع NGX_ERROR;
    }

    إذا (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->لوګاریتم, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        اذهب failed;
    }

    rev = c->اقرأ;
    wev = c->اكتب;

    rev->لوګاریتم = &rec->لوګاریتم;
    wev->لوګاریتم = &rec->لوګاریتم;

    rec->udp = c;

    c->شمېره = ngx_atomic_fetch_جمع_کړه(ngx_connection_counter, 1);

    c->پیل_وخت = ngx_current_msec;

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->لوګاریتم, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->شمېره);

    rc = connect(s, rec->sockaddr, rec->socklen);

    إذا (rc == -1) {
        ngx_log_error(NGX_LOG_CRIT, &rec->لوګاریتم, ngx_socket_errno,
                      "connect() failed");

        اذهب failed;
    }

    /* UDP sockets are always ready to write */
    wev->ready = 1;

    event = (ngx_event_flags & NGX_USE_CLEAR_EVENT) ?
                /* kqueue, epoll */                 NGX_CLEAR_EVENT:
                /* select, poll, /dev/poll */       NGX_LEVEL_EVENT;
                /* eventport event type has no meaning: oneshot only */

    إذا (ngx_جمع_کړه_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
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

    s = ngx_socket(rec->sockaddr->sa_family, SOCK_بهیر, 0);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->لوګاریتم, 0, "TCP socket %d", s);

    إذا (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->لوګاریتم, ngx_socket_errno,
                      ngx_socket_n " failed");
        أرجع NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->لوګاریتم);

    إذا (c == عدم) {
        إذا (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->لوګاریتم, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        أرجع NGX_ERROR;
    }

    إذا (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->لوګاریتم, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        اذهب failed;
    }

    rev = c->اقرأ;
    wev = c->اكتب;

    rev->لوګاریتم = &rec->لوګاریتم;
    wev->لوګاریتم = &rec->لوګاریتم;

    rec->tcp = c;

    c->شمېره = ngx_atomic_fetch_جمع_کړه(ngx_connection_counter, 1);

    c->پیل_وخت = ngx_current_msec;

    إذا (ngx_جمع_کړه_conn) {
        إذا (ngx_جمع_کړه_conn(c) == NGX_ERROR) {
            اذهب failed;
        }
    }

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->لوګاریتم, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->شمېره);

    rc = connect(s, rec->sockaddr, rec->socklen);

    إذا (rc == -1) {
        err = ngx_socket_errno;


        إذا (err != NGX_EINPROGRESS
#إذا (NGX_WIN32)
            /* Winsock returns WSAEWOULDBLOCK (NGX_EAGAIN) */
            && err != NGX_EAGAIN
#د_که_پای
            )
        {
            إذا (err == NGX_ECONNREFUSED
#إذا (NGX_LINUX)
                /*
                 * Linux returns EAGAIN instead of ECONNREFUSED
                 * for unix sockets if listen queue is full
                 */
                || err == NGX_EAGAIN
#د_که_پای
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

            ngx_log_error(level, &rec->لوګاریتم, err, "connect() to %V failed",
                          &rec->server);

            ngx_close_connection(c);
            rec->tcp = عدم;

            أرجع NGX_ERROR;
        }
    }

    إذا (ngx_جمع_کړه_conn) {
        إذا (rc == -1) {

            /* NGX_EINPROGRESS */

            أرجع NGX_AGAIN;
        }

        ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->لوګاریتم, 0, "connected");

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

    إذا (ngx_جمع_کړه_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        اذهب failed;
    }

    إذا (rc == -1) {

        /* NGX_EINPROGRESS */

        إذا (ngx_جمع_کړه_event(wev, NGX_WRITE_EVENT, event) != NGX_OK) {
            اذهب failed;
        }

        أرجع NGX_AGAIN;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->لوګاریتم, 0, "connected");

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
    ngx_resolver_srv_t  *لومړی, *دوهم;

    لومړی = (ngx_resolver_srv_t *) one;
    دوهم = (ngx_resolver_srv_t *) two;

    p1 = لومړی->لومړیتوب;
    p2 = دوهم->لومړیتوب;

    أرجع p1 - p2;
}
