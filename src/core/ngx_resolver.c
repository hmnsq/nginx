
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <ngx_event.h>


#تعریف_کرو NGX_RESOLVER_UDP_ناپ   4096

#تعریف_کرو NGX_RESOLVER_TCP_RSIZE  (2 + 65535)
#تعریف_کرو NGX_RESOLVER_TCP_WSIZE  8192


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


#تعریف_کرو ngx_resolver_گرہ(n)  ngx_rbtree_انکڑے(n, ngx_resolver_گرہ_t, گرہ)


ساكن ngx_int_t ngx_udp_connect(ngx_resolver_connection_t *rec);
ساكن ngx_int_t ngx_tcp_connect(ngx_resolver_connection_t *rec);


ساكن فراغ ngx_resolver_cleanup(فراغ *انکڑے);
ساكن فراغ ngx_resolver_cleanup_درخت(ngx_resolver_t *r, ngx_rbtree_t *درخت);
ساكن ngx_int_t ngx_resolve_name_locked(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx, ngx_str_t *name);
ساكن فراغ ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *درخت,
    ngx_قطار_t *قطار);
ساكن ngx_int_t ngx_resolver_send_query(ngx_resolver_t *r,
    ngx_resolver_گرہ_t *rn);
ساكن ngx_int_t ngx_resolver_send_udp_query(ngx_resolver_t *r,
    ngx_resolver_connection_t *rec, u_char *query, u_short qlen);
ساكن ngx_int_t ngx_resolver_send_tcp_query(ngx_resolver_t *r,
    ngx_resolver_connection_t *rec, u_char *query, u_short qlen);
ساكن ngx_int_t ngx_resolver_create_name_query(ngx_resolver_t *r,
    ngx_resolver_گرہ_t *rn, ngx_str_t *name);
ساكن ngx_int_t ngx_resolver_create_srv_query(ngx_resolver_t *r,
    ngx_resolver_گرہ_t *rn, ngx_str_t *name);
ساكن ngx_int_t ngx_resolver_create_addr_query(ngx_resolver_t *r,
    ngx_resolver_گرہ_t *rn, ngx_resolver_addr_t *addr);
ساكن فراغ ngx_resolver_resend_handler(ngx_event_t *ev);
ساكن ویلا_t ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *درخت,
    ngx_قطار_t *قطار);
ساكن ngx_uint_t ngx_resolver_resend_خالی_تھاواں(ngx_resolver_t *r);
ساكن فراغ ngx_resolver_udp_read(ngx_event_t *rev);
ساكن فراغ ngx_resolver_tcp_write(ngx_event_t *wev);
ساكن فراغ ngx_resolver_tcp_read(ngx_event_t *rev);
ساكن فراغ ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf,
    ناپ_t n, ngx_uint_t tcp);
ساكن فراغ ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, ناپ_t n,
    ngx_uint_t ident, ngx_uint_t کوڈ, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans);
ساكن فراغ ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, ناپ_t n,
    ngx_uint_t ident, ngx_uint_t کوڈ, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans);
ساكن فراغ ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, ناپ_t n,
    ngx_uint_t ident, ngx_uint_t کوڈ, ngx_uint_t nan);
ساكن ngx_resolver_گرہ_t *ngx_resolver_lookup_name(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
ساكن ngx_resolver_گرہ_t *ngx_resolver_lookup_srv(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
ساكن ngx_resolver_گرہ_t *ngx_resolver_lookup_addr(ngx_resolver_t *r,
    وچ_addr_t addr);
ساكن فراغ ngx_resolver_rbtree_داخل_کرو_قدر(ngx_rbtree_گرہ_t *عارضی,
    ngx_rbtree_گرہ_t *گرہ, ngx_rbtree_گرہ_t *sentinel);
ساكن ngx_int_t ngx_resolver_copy(ngx_resolver_t *r, ngx_str_t *name,
    u_char *buf, u_char *src, u_char *آخری);
ساكن ngx_int_t ngx_resolver_سیٹ_timeout(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx);
ساكن فراغ ngx_resolver_timeout_handler(ngx_event_t *ev);
ساكن فراغ ngx_resolver_free_گرہ(ngx_resolver_t *r, ngx_resolver_گرہ_t *rn);
ساكن فراغ *ngx_resolver_alloc(ngx_resolver_t *r, ناپ_t ناپ);
ساكن فراغ *ngx_resolver_calloc(ngx_resolver_t *r, ناپ_t ناپ);
ساكن فراغ ngx_resolver_free(ngx_resolver_t *r, فراغ *p);
ساكن فراغ ngx_resolver_free_locked(ngx_resolver_t *r, فراغ *p);
ساكن فراغ *ngx_resolver_dup(ngx_resolver_t *r, فراغ *src, ناپ_t ناپ);
ساكن ngx_resolver_addr_t *ngx_resolver_export(ngx_resolver_t *r,
    ngx_resolver_گرہ_t *rn, ngx_uint_t rotate);
ساكن فراغ ngx_resolver_report_srv(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx);
ساكن u_char *ngx_resolver_log_error(ngx_log_t *لوگارتھم, u_char *buf, ناپ_t len);
ساكن فراغ ngx_resolver_resolve_srv_ناں(ngx_resolver_ctx_t *ctx,
    ngx_resolver_گرہ_t *rn);
ساكن فراغ ngx_resolver_srv_ناں_handler(ngx_resolver_ctx_t *ctx);
ساكن ngx_int_t ngx_resolver_cmp_srvs(ثابت فراغ *one, ثابت فراغ *two);

#إذا (NGX_HAVE_INET6)
ساكن فراغ ngx_resolver_rbtree_داخل_کرو_addr6_قدر(ngx_rbtree_گرہ_t *عارضی,
    ngx_rbtree_گرہ_t *گرہ, ngx_rbtree_گرہ_t *sentinel);
ساكن ngx_resolver_گرہ_t *ngx_resolver_lookup_addr6(ngx_resolver_t *r,
    بنية in6_addr *addr, uint32_t hash);
#جے_انت


ngx_resolver_t *
ngx_resolver_create(ngx_conf_t *cf, ngx_str_t *ناں, ngx_uint_t n)
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

    cln = ngx_pool_cleanup_جوڑو(cf->pool, 0);
    إذا (cln == عدم) {
        أرجع عدم;
    }

    cln->handler = ngx_resolver_cleanup;
    cln->انکڑے = r;

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

#إذا (NGX_HAVE_INET6)
    r->ipv6 = 1;

    ngx_rbtree_init(&r->addr6_rbtree, &r->addr6_sentinel,
                    ngx_resolver_rbtree_داخل_کرو_addr6_قدر);

    ngx_قطار_init(&r->addr6_resend_قطار);

    ngx_قطار_init(&r->addr6_expire_قطار);
#جے_انت

    r->event->handler = ngx_resolver_resend_handler;
    r->event->انکڑے = r;
    r->event->لوگارتھم = &cf->چکر->new_log;
    r->event->cancelable = 1;
    r->ident = -1;

    r->resend_timeout = 5;
    r->tcp_timeout = 5;
    r->expire = 30;
    r->valid = 0;

    r->لوگارتھم = &cf->چکر->new_log;
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
        إذا (ngx_strncmp(ناں[i].انکڑے, "valid=", 6) == 0) {
            s.len = ناں[i].len - 6;
            s.انکڑے = ناں[i].انکڑے + 6;

            r->valid = ngx_parse_ویلا(&s, 1);

            إذا (r->valid == (ویلا_t) NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &ناں[i]);
                أرجع عدم;
            }

            استمر;
        }

#إذا (NGX_HAVE_INET6)
        إذا (ngx_strncmp(ناں[i].انکڑے, "ipv4=", 5) == 0) {

            إذا (ngx_strcmp(&ناں[i].انکڑے[5], "on") == 0) {
                r->ipv4 = 1;

            } وإلا إذا (ngx_strcmp(&ناں[i].انکڑے[5], "off") == 0) {
                r->ipv4 = 0;

            } وإلا {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &ناں[i]);
                أرجع عدم;
            }

            استمر;
        }

        إذا (ngx_strncmp(ناں[i].انکڑے, "ipv6=", 5) == 0) {

            إذا (ngx_strcmp(&ناں[i].انکڑے[5], "on") == 0) {
                r->ipv6 = 1;

            } وإلا إذا (ngx_strcmp(&ناں[i].انکڑے[5], "off") == 0) {
                r->ipv6 = 0;

            } وإلا {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &ناں[i]);
                أرجع عدم;
            }

            استمر;
        }
#جے_انت

        ngx_memzero(&u, حجم(ngx_url_t));

        u.url = ناں[i];
        u.default_پورٹ = 53;

        إذا (ngx_parse_url(cf->pool, &u) != NGX_OK) {
            إذا (u.err) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "%s in resolver \"%V\"",
                                   u.err, &u.url);
            }

            أرجع عدم;
        }

        rec = ngx_array_ڈالو_n(&r->connections, u.naddrs);
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
#جے_انت

    إذا (n && r->connections.nelts == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "no name servers defined");
        أرجع عدم;
    }

    أرجع r;
}


ساكن فراغ
ngx_resolver_cleanup(فراغ *انکڑے)
{
    ngx_resolver_t  *r = انکڑے;

    ngx_uint_t                  i;
    ngx_resolver_connection_t  *rec;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_چکر->لوگارتھم, 0, "cleanup resolver");

    ngx_resolver_cleanup_درخت(r, &r->name_rbtree);

    ngx_resolver_cleanup_درخت(r, &r->srv_rbtree);

    ngx_resolver_cleanup_درخت(r, &r->addr_rbtree);

#إذا (NGX_HAVE_INET6)
    ngx_resolver_cleanup_درخت(r, &r->addr6_rbtree);
#جے_انت

    إذا (r->event->timer_سیٹ) {
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
    ngx_resolver_ctx_t   *ctx, *اگلا;
    ngx_resolver_گرہ_t  *rn;

    طالما (درخت->جڑ != درخت->sentinel) {

        rn = ngx_resolver_گرہ(ngx_rbtree_min(درخت->جڑ, درخت->sentinel));

        ngx_قطار_remove(&rn->قطار);

        لكل (ctx = rn->اڈیک; ctx; ctx = اگلا) {
            اگلا = ctx->اگلا;

            إذا (ctx->event) {
                إذا (ctx->event->timer_سیٹ) {
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
ngx_resolve_شروع(ngx_resolver_t *r, ngx_resolver_ctx_t *عارضی)
{
    وچ_addr_t            addr;
    ngx_resolver_ctx_t  *ctx;

    إذا (عارضی) {
        addr = ngx_inet_addr(عارضی->name.انکڑے, عارضی->name.len);

        إذا (addr != INADDR_NONE) {
            عارضی->resolver = r;
            عارضی->state = NGX_OK;
            عارضی->naddrs = 1;
            عارضی->addrs = &عارضی->addr;
            عارضی->addr.sockaddr = (بنية sockaddr *) &عارضی->جیب;
            عارضی->addr.socklen = حجم(بنية sockaddr_وچ);
            ngx_memzero(&عارضی->جیب, حجم(بنية sockaddr_وچ));
            عارضی->جیب.sin_family = AF_INET;
            عارضی->جیب.sin_addr.s_addr = addr;
            عارضی->تیز = 1;

            أرجع عارضی;
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
    ناپ_t           slen;
    ngx_int_t        rc;
    ngx_str_t        name;
    ngx_resolver_t  *r;

    r = ctx->resolver;

    إذا (ctx->name.len > 0 && ctx->name.انکڑے[ctx->name.len - 1] == '.') {
        ctx->name.len--;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0,
                   "resolve: \"%V\"", &ctx->name);

    إذا (ctx->تیز) {
        ctx->handler(ctx);
        أرجع NGX_OK;
    }

    إذا (ctx->service.len) {
        slen = ctx->service.len;

        إذا (ngx_strlchr(ctx->service.انکڑے,
                        ctx->service.انکڑے + ctx->service.len, '.')
            == عدم)
        {
            slen += حجم("_._tcp") - 1;
        }

        name.len = slen + 1 + ctx->name.len;

        name.انکڑے = ngx_resolver_alloc(r, name.len);
        إذا (name.انکڑے == عدم) {
            اذهب failed;
        }

        إذا (slen == ctx->service.len) {
            ngx_sprintf(name.انکڑے, "%V.%V", &ctx->service, &ctx->name);

        } وإلا {
            ngx_sprintf(name.انکڑے, "_%V._tcp.%V", &ctx->service, &ctx->name);
        }

        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &name);

        ngx_resolver_free(r, name.انکڑے);

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
ngx_resolve_name_پورا(ngx_resolver_ctx_t *ctx)
{
    ngx_uint_t            i;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_گرہ_t  *rn;

    r = ctx->resolver;

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0,
                   "resolve name done: %i", ctx->state);

    إذا (ctx->تیز) {
        أرجع;
    }

    إذا (ctx->event && ctx->event->timer_سیٹ) {
        ngx_del_timer(ctx->event);
    }

    /* lock name mutex */

    إذا (ctx->nsrvs) {
        لكل (i = 0; i < ctx->nsrvs; i++) {
            إذا (ctx->srvs[i].ctx) {
                ngx_resolve_name_پورا(ctx->srvs[i].ctx);
            }

            إذا (ctx->srvs[i].addrs) {
                ngx_resolver_free(r, ctx->srvs[i].addrs->sockaddr);
                ngx_resolver_free(r, ctx->srvs[i].addrs);
            }

            ngx_resolver_free(r, ctx->srvs[i].name.انکڑے);
        }

        ngx_resolver_free(r, ctx->srvs);
    }

    إذا (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->گرہ;

        إذا (rn) {
            p = &rn->اڈیک;
            w = rn->اڈیک;

            طالما (w) {
                إذا (w == ctx) {
                    *p = w->اگلا;

                    اذهب پورا;
                }

                p = &w->اگلا;
                w = w->اگلا;
            }

            ngx_log_error(NGX_LOG_ALERT, r->لوگارتھم, 0,
                          "could not cancel %V resolving", &ctx->name);
        }
    }

پورا:

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

    إذا (r->event->timer_سیٹ && ngx_resolver_resend_خالی_تھاواں(r)) {
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
    ngx_rbtree_t         *درخت;
    ngx_resolver_ctx_t   *اگلا, *آخری;
    ngx_resolver_addr_t  *addrs;
    ngx_resolver_گرہ_t  *rn;

    ngx_strlow(name->انکڑے, name->انکڑے, name->len);

    hash = ngx_چکری_جانچ_32_short(name->انکڑے, name->len);

    إذا (ctx->service.len) {
        rn = ngx_resolver_lookup_srv(r, name, hash);

        درخت = &r->srv_rbtree;
        resend_قطار = &r->srv_resend_قطار;
        expire_قطار = &r->srv_expire_قطار;

    } وإلا {
        rn = ngx_resolver_lookup_name(r, name, hash);

        درخت = &r->name_rbtree;
        resend_قطار = &r->name_resend_قطار;
        expire_قطار = &r->name_expire_قطار;
    }

    إذا (rn) {

        /* ctx can be a list after NGX_RESOLVE_CNAME */
        لكل (آخری = ctx; آخری->اگلا; آخری = آخری->اگلا);

        إذا (rn->valid >= ngx_ویلا()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0, "resolve cached");

            ngx_قطار_remove(&rn->قطار);

            rn->expire = ngx_ویلا() + r->expire;

            ngx_قطار_داخل_کرو_سر(expire_قطار, &rn->قطار);

            naddrs = (rn->naddrs == (u_short) -1) ? 0 : rn->naddrs;
#إذا (NGX_HAVE_INET6)
            naddrs += (rn->naddrs6 == (u_short) -1) ? 0 : rn->naddrs6;
#جے_انت

            إذا (naddrs) {

                إذا (naddrs == 1 && rn->naddrs == 1) {
                    addrs = عدم;

                } وإلا {
                    addrs = ngx_resolver_export(r, rn, 1);
                    إذا (addrs == عدم) {
                        أرجع NGX_ERROR;
                    }
                }

                آخری->اگلا = rn->اڈیک;
                rn->اڈیک = عدم;

                /* unlock name mutex */

                افعل {
                    ctx->state = NGX_OK;
                    ctx->valid = rn->valid;
                    ctx->naddrs = naddrs;

                    إذا (addrs == عدم) {
                        ctx->addrs = &ctx->addr;
                        ctx->addr.sockaddr = (بنية sockaddr *) &ctx->جیب;
                        ctx->addr.socklen = حجم(بنية sockaddr_وچ);
                        ngx_memzero(&ctx->جیب, حجم(بنية sockaddr_وچ));
                        ctx->جیب.sin_family = AF_INET;
                        ctx->جیب.sin_addr.s_addr = rn->u.addr;

                    } وإلا {
                        ctx->addrs = addrs;
                    }

                    اگلا = ctx->اگلا;

                    ctx->handler(ctx);

                    ctx = اگلا;
                } طالما (ctx);

                إذا (addrs != عدم) {
                    ngx_resolver_free(r, addrs->sockaddr);
                    ngx_resolver_free(r, addrs);
                }

                أرجع NGX_OK;
            }

            إذا (rn->nsrvs) {
                آخری->اگلا = rn->اڈیک;
                rn->اڈیک = عدم;

                /* unlock name mutex */

                افعل {
                    اگلا = ctx->اگلا;

                    ngx_resolver_resolve_srv_ناں(ctx, rn);

                    ctx = اگلا;
                } طالما (ctx);

                أرجع NGX_OK;
            }

            /* NGX_RESOLVE_CNAME */

            إذا (ctx->recursion++ < NGX_RESOLVER_MAX_RECURSION) {

                cname.len = rn->cnlen;
                cname.انکڑے = rn->u.cname;

                أرجع ngx_resolve_name_locked(r, ctx, &cname);
            }

            آخری->اگلا = rn->اڈیک;
            rn->اڈیک = عدم;

            /* unlock name mutex */

            افعل {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                ctx->valid = ngx_ویلا() + (r->valid ? r->valid : 10);
                اگلا = ctx->اگلا;

                ctx->handler(ctx);

                ctx = اگلا;
            } طالما (ctx);

            أرجع NGX_OK;
        }

        إذا (rn->اڈیک) {
            إذا (ngx_resolver_سیٹ_timeout(r, ctx) != NGX_OK) {
                أرجع NGX_ERROR;
            }

            آخری->اگلا = rn->اڈیک;
            rn->اڈیک = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;

            افعل {
                ctx->گرہ = rn;
                ctx = ctx->اگلا;
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
#جے_انت
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
#جے_انت

        إذا (rn->nsrvs) {
            لكل (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
                إذا (rn->u.srvs[i].name.انکڑے) {
                    ngx_resolver_free_locked(r, rn->u.srvs[i].name.انکڑے);
                }
            }

            ngx_resolver_free_locked(r, rn->u.srvs);
        }

        /* unlock alloc mutex */

    } وإلا {

        rn = ngx_resolver_alloc(r, حجم(ngx_resolver_گرہ_t));
        إذا (rn == عدم) {
            أرجع NGX_ERROR;
        }

        rn->name = ngx_resolver_dup(r, name->انکڑے, name->len);
        إذا (rn->name == عدم) {
            ngx_resolver_free(r, rn);
            أرجع NGX_ERROR;
        }

        rn->گرہ.کلید = hash;
        rn->nlen = (u_short) name->len;
        rn->query = عدم;
#إذا (NGX_HAVE_INET6)
        rn->query6 = عدم;
#جے_انت

        ngx_rbtree_داخل_کرو(درخت, &rn->گرہ);
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
        ngx_rbtree_delete(درخت, &rn->گرہ);

        ngx_resolver_free(r, rn->query);
        ngx_resolver_free(r, rn->name);
        ngx_resolver_free(r, rn);

        افعل {
            ctx->state = NGX_RESOLVE_NXDOMAIN;
            اگلا = ctx->اگلا;

            ctx->handler(ctx);

            ctx = اگلا;
        } طالما (ctx);

        أرجع NGX_OK;
    }

    rn->آخری_connection = r->آخری_connection++;
    إذا (r->آخری_connection == r->connections.nelts) {
        r->آخری_connection = 0;
    }

    rn->naddrs = r->ipv4 ? (u_short) -1 : 0;
    rn->tcp = 0;
#إذا (NGX_HAVE_INET6)
    rn->naddrs6 = r->ipv6 ? (u_short) -1 : 0;
    rn->tcp6 = 0;
#جے_انت
    rn->nsrvs = 0;

    إذا (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->آخری_connection++;
        إذا (rn->آخری_connection == r->connections.nelts) {
            rn->آخری_connection = 0;
        }

        (فراغ) ngx_resolver_send_query(r, rn);
    }

    إذا (ngx_resolver_سیٹ_timeout(r, ctx) != NGX_OK) {
        اذهب failed;
    }

    إذا (ngx_resolver_resend_خالی_تھاواں(r)) {
        ngx_جوڑو_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_ویلا() + r->resend_timeout;

    ngx_قطار_داخل_کرو_سر(resend_قطار, &rn->قطار);

    rn->کوڈ = 0;
    rn->cnlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_قدر;
    rn->اڈیک = ctx;

    ctx->state = NGX_AGAIN;
    ctx->async = 1;

    افعل {
        ctx->گرہ = rn;
        ctx = ctx->اگلا;
    } طالما (ctx);

    أرجع NGX_AGAIN;

failed:

    ngx_rbtree_delete(درخت, &rn->گرہ);

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
    وچ_addr_t             addr;
    ngx_قطار_t          *resend_قطار, *expire_قطار;
    ngx_rbtree_t         *درخت;
    ngx_resolver_t       *r;
    بنية sockaddr_وچ   *جیب;
    ngx_resolver_گرہ_t  *rn;
#إذا (NGX_HAVE_INET6)
    uint32_t              hash;
    بنية sockaddr_in6  *sin6;
#جے_انت

#إذا (NGX_SUPPRESS_WARN)
    addr = 0;
#إذا (NGX_HAVE_INET6)
    hash = 0;
    sin6 = عدم;
#جے_انت
#جے_انت

    r = ctx->resolver;

    بدل (ctx->addr.sockaddr->sa_family) {

#إذا (NGX_HAVE_INET6)
    حالة AF_INET6:
        sin6 = (بنية sockaddr_in6 *) ctx->addr.sockaddr;
        hash = ngx_چکری_جانچ_32_short(sin6->sin6_addr.s6_addr, 16);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr6(r, &sin6->sin6_addr, hash);

        درخت = &r->addr6_rbtree;
        resend_قطار = &r->addr6_resend_قطار;
        expire_قطار = &r->addr6_expire_قطار;

        اكسر;
#جے_انت

    افتراضي: /* AF_INET */
        جیب = (بنية sockaddr_وچ *) ctx->addr.sockaddr;
        addr = ntohl(جیب->sin_addr.s_addr);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        درخت = &r->addr_rbtree;
        resend_قطار = &r->addr_resend_قطار;
        expire_قطار = &r->addr_expire_قطار;
    }

    إذا (rn) {

        إذا (rn->valid >= ngx_ویلا()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0, "resolve cached");

            ngx_قطار_remove(&rn->قطار);

            rn->expire = ngx_ویلا() + r->expire;

            ngx_قطار_داخل_کرو_سر(expire_قطار, &rn->قطار);

            name = ngx_resolver_dup(r, rn->name, rn->nlen);
            إذا (name == عدم) {
                ngx_resolver_free(r, ctx);
                أرجع NGX_ERROR;
            }

            ctx->name.len = rn->nlen;
            ctx->name.انکڑے = name;

            /* unlock addr mutex */

            ctx->state = NGX_OK;
            ctx->valid = rn->valid;

            ctx->handler(ctx);

            ngx_resolver_free(r, name);

            أرجع NGX_OK;
        }

        إذا (rn->اڈیک) {
            إذا (ngx_resolver_سیٹ_timeout(r, ctx) != NGX_OK) {
                أرجع NGX_ERROR;
            }

            ctx->اگلا = rn->اڈیک;
            rn->اڈیک = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;
            ctx->گرہ = rn;

            /* unlock addr mutex */

            أرجع NGX_OK;
        }

        ngx_قطار_remove(&rn->قطار);

        ngx_resolver_free(r, rn->query);
        rn->query = عدم;
#إذا (NGX_HAVE_INET6)
        rn->query6 = عدم;
#جے_انت

    } وإلا {
        rn = ngx_resolver_alloc(r, حجم(ngx_resolver_گرہ_t));
        إذا (rn == عدم) {
            اذهب failed;
        }

        بدل (ctx->addr.sockaddr->sa_family) {

#إذا (NGX_HAVE_INET6)
        حالة AF_INET6:
            rn->addr6 = sin6->sin6_addr;
            rn->گرہ.کلید = hash;
            اكسر;
#جے_انت

        افتراضي: /* AF_INET */
            rn->گرہ.کلید = addr;
        }

        rn->query = عدم;
#إذا (NGX_HAVE_INET6)
        rn->query6 = عدم;
#جے_انت

        ngx_rbtree_داخل_کرو(درخت, &rn->گرہ);
    }

    إذا (ngx_resolver_create_addr_query(r, rn, &ctx->addr) != NGX_OK) {
        اذهب failed;
    }

    rn->آخری_connection = r->آخری_connection++;
    إذا (r->آخری_connection == r->connections.nelts) {
        r->آخری_connection = 0;
    }

    rn->naddrs = (u_short) -1;
    rn->tcp = 0;
#إذا (NGX_HAVE_INET6)
    rn->naddrs6 = (u_short) -1;
    rn->tcp6 = 0;
#جے_انت
    rn->nsrvs = 0;

    إذا (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->آخری_connection++;
        إذا (rn->آخری_connection == r->connections.nelts) {
            rn->آخری_connection = 0;
        }

        (فراغ) ngx_resolver_send_query(r, rn);
    }

    إذا (ngx_resolver_سیٹ_timeout(r, ctx) != NGX_OK) {
        اذهب failed;
    }

    إذا (ngx_resolver_resend_خالی_تھاواں(r)) {
        ngx_جوڑو_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_ویلا() + r->resend_timeout;

    ngx_قطار_داخل_کرو_سر(resend_قطار, &rn->قطار);

    rn->کوڈ = 0;
    rn->cnlen = 0;
    rn->name = عدم;
    rn->nlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_قدر;
    rn->اڈیک = ctx;

    /* unlock addr mutex */

    ctx->state = NGX_AGAIN;
    ctx->async = 1;
    ctx->گرہ = rn;

    أرجع NGX_OK;

failed:

    إذا (rn) {
        ngx_rbtree_delete(درخت, &rn->گرہ);

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
ngx_resolve_addr_پورا(ngx_resolver_ctx_t *ctx)
{
    ngx_قطار_t          *expire_قطار;
    ngx_rbtree_t         *درخت;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_گرہ_t  *rn;

    r = ctx->resolver;

    بدل (ctx->addr.sockaddr->sa_family) {

#إذا (NGX_HAVE_INET6)
    حالة AF_INET6:
        درخت = &r->addr6_rbtree;
        expire_قطار = &r->addr6_expire_قطار;
        اكسر;
#جے_انت

    افتراضي: /* AF_INET */
        درخت = &r->addr_rbtree;
        expire_قطار = &r->addr_expire_قطار;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0,
                   "resolve addr done: %i", ctx->state);

    إذا (ctx->event && ctx->event->timer_سیٹ) {
        ngx_del_timer(ctx->event);
    }

    /* lock addr mutex */

    إذا (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->گرہ;

        إذا (rn) {
            p = &rn->اڈیک;
            w = rn->اڈیک;

            طالما (w) {
                إذا (w == ctx) {
                    *p = w->اگلا;

                    اذهب پورا;
                }

                p = &w->اگلا;
                w = w->اگلا;
            }
        }

        {
            u_char     متن[NGX_SOCKADDR_STRLEN];
            ngx_str_t  addrtext;

            addrtext.انکڑے = متن;
            addrtext.len = ngx_sock_ntop(ctx->addr.sockaddr, ctx->addr.socklen,
                                         متن, NGX_SOCKADDR_STRLEN, 0);

            ngx_log_error(NGX_LOG_ALERT, r->لوگارتھم, 0,
                          "could not cancel %V resolving", &addrtext);
        }
    }

پورا:

    ngx_resolver_expire(r, درخت, expire_قطار);

    /* unlock addr mutex */

    /* lock alloc mutex */

    إذا (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    إذا (r->event->timer_سیٹ && ngx_resolver_resend_خالی_تھاواں(r)) {
        ngx_del_timer(r->event);
    }
}


ساكن فراغ
ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *درخت, ngx_قطار_t *قطار)
{
    ویلا_t                now;
    ngx_uint_t            i;
    ngx_قطار_t          *q;
    ngx_resolver_گرہ_t  *rn;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0, "resolver expire");

    now = ngx_ویلا();

    لكل (i = 0; i < 2; i++) {
        إذا (ngx_قطار_خالی_تھاواں(قطار)) {
            أرجع;
        }

        q = ngx_قطار_آخری(قطار);

        rn = ngx_قطار_انکڑے(q, ngx_resolver_گرہ_t, قطار);

        إذا (now <= rn->expire) {
            أرجع;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0,
                       "resolver expire \"%*s\"", (ناپ_t) rn->nlen, rn->name);

        ngx_قطار_remove(q);

        ngx_rbtree_delete(درخت, &rn->گرہ);

        ngx_resolver_free_گرہ(r, rn);
    }
}


ساكن ngx_int_t
ngx_resolver_send_query(ngx_resolver_t *r, ngx_resolver_گرہ_t *rn)
{
    ngx_int_t                   rc;
    ngx_resolver_connection_t  *rec;

    rec = r->connections.elts;
    rec = &rec[rn->آخری_connection];

    إذا (rec->لوگارتھم.handler == عدم) {
        rec->لوگارتھم = *r->لوگارتھم;
        rec->لوگارتھم.handler = ngx_resolver_log_error;
        rec->لوگارتھم.انکڑے = rec;
        rec->لوگارتھم.action = "resolving";
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

#جے_انت

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

        rec->udp->انکڑے = rec;
        rec->udp->اقرأ->handler = ngx_resolver_udp_read;
        rec->udp->اقرأ->resolver = 1;
    }

    n = ngx_send(rec->udp, query, qlen);

    إذا (n == NGX_ERROR) {
        اذهب failed;
    }

    إذا ((ناپ_t) n != (ناپ_t) qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->لوگارتھم, 0, "send() incomplete");
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

            b->سرا = b->شروع + NGX_RESOLVER_TCP_RSIZE;

            rec->read_buf = b;
        }

        b->pos = b->شروع;
        b->آخری = b->شروع;

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

            b->سرا = b->شروع + NGX_RESOLVER_TCP_WSIZE;

            rec->write_buf = b;
        }

        b->pos = b->شروع;
        b->آخری = b->شروع;

        rc = ngx_tcp_connect(rec);
        إذا (rc == NGX_ERROR) {
            أرجع NGX_ERROR;
        }

        rec->tcp->انکڑے = rec;
        rec->tcp->اكتب->handler = ngx_resolver_tcp_write;
        rec->tcp->اكتب->cancelable = 1;
        rec->tcp->اقرأ->handler = ngx_resolver_tcp_read;
        rec->tcp->اقرأ->resolver = 1;

        ngx_جوڑو_timer(rec->tcp->اكتب, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    b = rec->write_buf;

    إذا (b->سرا - b->آخری <  2 + qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->لوگارتھم, 0, "buffer overflow");
        أرجع NGX_ERROR;
    }

    *b->آخری++ = (u_char) (qlen >> 8);
    *b->آخری++ = (u_char) qlen;
    b->آخری = ngx_cpymem(b->آخری, query, qlen);

    إذا (rc == NGX_OK) {
        ngx_resolver_tcp_write(rec->tcp->اكتب);
    }

    أرجع NGX_OK;
}


ساكن فراغ
ngx_resolver_resend_handler(ngx_event_t *ev)
{
    ویلا_t           timer, atimer, stimer, ntimer;
#إذا (NGX_HAVE_INET6)
    ویلا_t           a6timer;
#جے_انت
    ngx_resolver_t  *r;

    r = ev->انکڑے;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0,
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

#جے_انت

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

#جے_انت

    إذا (timer) {
        ngx_جوڑو_timer(r->event, (ngx_msec_t) (timer * 1000));
    }
}


ساكن ویلا_t
ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *درخت, ngx_قطار_t *قطار)
{
    ویلا_t                now;
    ngx_قطار_t          *q;
    ngx_resolver_گرہ_t  *rn;

    now = ngx_ویلا();

    لكل ( ;; ) {
        إذا (ngx_قطار_خالی_تھاواں(قطار)) {
            أرجع 0;
        }

        q = ngx_قطار_آخری(قطار);

        rn = ngx_قطار_انکڑے(q, ngx_resolver_گرہ_t, قطار);

        إذا (now < rn->expire) {
            أرجع rn->expire - now;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0,
                       "resolver resend \"%*s\" %p",
                       (ناپ_t) rn->nlen, rn->name, rn->اڈیک);

        ngx_قطار_remove(q);

        إذا (rn->اڈیک) {

            إذا (++rn->آخری_connection == r->connections.nelts) {
                rn->آخری_connection = 0;
            }

            (فراغ) ngx_resolver_send_query(r, rn);

            rn->expire = now + r->resend_timeout;

            ngx_قطار_داخل_کرو_سر(قطار, q);

            استمر;
        }

        ngx_rbtree_delete(درخت, &rn->گرہ);

        ngx_resolver_free_گرہ(r, rn);
    }
}


ساكن ngx_uint_t
ngx_resolver_resend_خالی_تھاواں(ngx_resolver_t *r)
{
    أرجع ngx_قطار_خالی_تھاواں(&r->name_resend_قطار)
           && ngx_قطار_خالی_تھاواں(&r->srv_resend_قطار)
#إذا (NGX_HAVE_INET6)
           && ngx_قطار_خالی_تھاواں(&r->addr6_resend_قطار)
#جے_انت
           && ngx_قطار_خالی_تھاواں(&r->addr_resend_قطار);
}


ساكن فراغ
ngx_resolver_udp_read(ngx_event_t *rev)
{
    ssize_t                     n;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;
    u_char                      buf[NGX_RESOLVER_UDP_ناپ];

    c = rev->انکڑے;
    rec = c->انکڑے;

    افعل {
        n = ngx_udp_recv(c, buf, NGX_RESOLVER_UDP_ناپ);

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
    off_t                       گھلیا;
    ssize_t                     n;
    ngx_buf_t                  *b;
    ngx_resolver_t             *r;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;

    c = wev->انکڑے;
    rec = c->انکڑے;
    b = rec->write_buf;
    r = rec->resolver;

    إذا (wev->timedout) {
        اذهب failed;
    }

    گھلیا = c->گھلیا;

    طالما (wev->ready && b->pos < b->آخری) {
        n = ngx_send(c, b->pos, b->آخری - b->pos);

        إذا (n == NGX_AGAIN) {
            اكسر;
        }

        إذا (n == NGX_ERROR) {
            اذهب failed;
        }

        b->pos += n;
    }

    إذا (b->pos != b->شروع) {
        b->آخری = ngx_movemem(b->شروع, b->pos, b->آخری - b->pos);
        b->pos = b->شروع;
    }

    إذا (c->گھلیا != گھلیا) {
        ngx_جوڑو_timer(wev, (ngx_msec_t) (r->tcp_timeout * 1000));
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
    ناپ_t                      ناپ;
    ssize_t                     n;
    u_short                     qlen;
    ngx_buf_t                  *b;
    ngx_resolver_t             *r;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;

    c = rev->انکڑے;
    rec = c->انکڑے;
    b = rec->read_buf;
    r = rec->resolver;

    طالما (rev->ready) {
        n = ngx_recv(c, b->آخری, b->سرا - b->آخری);

        إذا (n == NGX_AGAIN) {
            اكسر;
        }

        إذا (n == NGX_ERROR || n == 0) {
            اذهب failed;
        }

        b->آخری += n;

        لكل ( ;; ) {
            p = b->pos;
            ناپ = b->آخری - p;

            إذا (ناپ < 2) {
                اكسر;
            }

            qlen = (u_short) *p++ << 8;
            qlen += *p++;

            إذا (ناپ < (ناپ_t) (2 + qlen)) {
                اكسر;
            }

            ngx_resolver_process_response(r, p, qlen, 1);

            b->pos += 2 + qlen;
        }

        إذا (b->pos != b->شروع) {
            b->آخری = ngx_movemem(b->شروع, b->pos, b->آخری - b->pos);
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
ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf, ناپ_t n,
    ngx_uint_t tcp)
{
    حرف                 *err;
    ngx_uint_t            i, times, ident, qident, flags, کوڈ, nqs, nan, trunc,
                          qtype, qclass;
#إذا (NGX_HAVE_INET6)
    ngx_uint_t            qident6;
#جے_انت
    ngx_قطار_t          *q;
    ngx_resolver_qs_t    *qs;
    ngx_resolver_hdr_t   *response;
    ngx_resolver_گرہ_t  *rn;

    إذا (n < حجم(ngx_resolver_hdr_t)) {
        اذهب short_response;
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
    إذا ((flags & 0xf870) != 0x8000 || (trunc && tcp)) {
        ngx_log_error(r->log_level, r->لوگارتھم, 0,
                      "invalid %s DNS response %ui fl:%04Xi",
                      tcp ? "TCP" : "UDP", ident, flags);
        أرجع;
    }

    کوڈ = flags & 0xf;

    إذا (کوڈ == NGX_RESOLVE_FORMERR) {

        times = 0;

        لكل (q = ngx_قطار_سر(&r->name_resend_قطار);
             q != ngx_قطار_sentinel(&r->name_resend_قطار) && times++ < 100;
             q = ngx_قطار_اگلا(q))
        {
            rn = ngx_قطار_انکڑے(q, ngx_resolver_گرہ_t, قطار);

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
#جے_انت
        }

        اذهب dns_error;
    }

    إذا (کوڈ > NGX_RESOLVE_REFUSED) {
        اذهب dns_error;
    }

    إذا (nqs != 1) {
        err = "invalid number of questions in DNS response";
        اذهب پورا;
    }

    i = حجم(ngx_resolver_hdr_t);

    طالما (i < (ngx_uint_t) n) {

        إذا (buf[i] & 0xc0) {
            err = "unexpected compression pointer in DNS response";
            اذهب پورا;
        }

        إذا (buf[i] == '\0') {
            اذهب لبھیا;
        }

        i += 1 + buf[i];
    }

    اذهب short_response;

لبھیا:

    إذا (i++ == حجم(ngx_resolver_hdr_t)) {
        err = "zero-length domain name in DNS response";
        اذهب پورا;
    }

    إذا (i + حجم(ngx_resolver_qs_t) + nan * (2 + حجم(ngx_resolver_an_t))
        > (ngx_uint_t) n)
    {
        اذهب short_response;
    }

    qs = (ngx_resolver_qs_t *) &buf[i];

    qtype = (qs->type_hi << 8) + qs->type_lo;
    qclass = (qs->class_hi << 8) + qs->class_lo;

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0,
                   "resolver DNS response qt:%ui cl:%ui", qtype, qclass);

    إذا (qclass != 1) {
        ngx_log_error(r->log_level, r->لوگارتھم, 0,
                      "unknown query class %ui in DNS response", qclass);
        أرجع;
    }

    بدل (qtype) {

    حالة NGX_RESOLVE_A:
#إذا (NGX_HAVE_INET6)
    حالة NGX_RESOLVE_AAAA:
#جے_انت

        ngx_resolver_process_a(r, buf, n, ident, کوڈ, qtype, nan, trunc,
                               i + حجم(ngx_resolver_qs_t));

        اكسر;

    حالة NGX_RESOLVE_SRV:

        ngx_resolver_process_srv(r, buf, n, ident, کوڈ, nan, trunc,
                                 i + حجم(ngx_resolver_qs_t));

        اكسر;

    حالة NGX_RESOLVE_PTR:

        ngx_resolver_process_ptr(r, buf, n, ident, کوڈ, nan);

        اكسر;

    افتراضي:
        ngx_log_error(r->log_level, r->لوگارتھم, 0,
                      "unknown query type %ui in DNS response", qtype);
        أرجع;
    }

    أرجع;

short_response:

    err = "short DNS response";

پورا:

    ngx_log_error(r->log_level, r->لوگارتھم, 0, err);

    أرجع;

dns_error_name:

    ngx_log_error(r->log_level, r->لوگارتھم, 0,
                  "DNS error (%ui: %s), query id:%ui, name:\"%*s\"",
                  کوڈ, ngx_resolver_strerror(کوڈ), ident,
                  (ناپ_t) rn->nlen, rn->name);
    أرجع;

dns_error:

    ngx_log_error(r->log_level, r->لوگارتھم, 0,
                  "DNS error (%ui: %s), query id:%ui",
                  کوڈ, ngx_resolver_strerror(کوڈ), ident);
    أرجع;
}


ساكن فراغ
ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, ناپ_t n,
    ngx_uint_t ident, ngx_uint_t کوڈ, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans)
{
    حرف                       *err;
    u_char                     *cname;
    ناپ_t                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    وچ_addr_t                  *addr;
    ngx_str_t                   name;
    ngx_uint_t                  type, class, qident, naddrs, a, i, j, شروع;
#إذا (NGX_HAVE_INET6)
    بنية in6_addr            *addr6;
#جے_انت
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *اگلا;
    ngx_resolver_گرہ_t        *rn;
    ngx_resolver_addr_t        *addrs;
    ngx_resolver_connection_t  *rec;

    إذا (ngx_resolver_copy(r, &name, buf,
                          buf + حجم(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        أرجع;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0, "resolver qs:%V", &name);

    hash = ngx_چکری_جانچ_32_short(name.انکڑے, name.len);

    /* lock name mutex */

    rn = ngx_resolver_lookup_name(r, &name, hash);

    إذا (rn == عدم) {
        ngx_log_error(r->log_level, r->لوگارتھم, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.انکڑے);
        اذهب failed;
    }

    بدل (qtype) {

#إذا (NGX_HAVE_INET6)
    حالة NGX_RESOLVE_AAAA:

        إذا (rn->query6 == عدم || rn->naddrs6 != (u_short) -1) {
            ngx_log_error(r->log_level, r->لوگارتھم, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.انکڑے);
            اذهب failed;
        }

        إذا (trunc && rn->tcp6) {
            ngx_resolver_free(r, name.انکڑے);
            اذهب failed;
        }

        qident = (rn->query6[0] << 8) + rn->query6[1];

        اكسر;
#جے_انت

    افتراضي: /* NGX_RESOLVE_A */

        إذا (rn->query == عدم || rn->naddrs != (u_short) -1) {
            ngx_log_error(r->log_level, r->لوگارتھم, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.انکڑے);
            اذهب failed;
        }

        إذا (trunc && rn->tcp) {
            ngx_resolver_free(r, name.انکڑے);
            اذهب failed;
        }

        qident = (rn->query[0] << 8) + rn->query[1];
    }

    إذا (ident != qident) {
        ngx_log_error(r->log_level, r->لوگارتھم, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.انکڑے);
        اذهب failed;
    }

    ngx_resolver_free(r, name.انکڑے);

    إذا (trunc) {

        ngx_قطار_remove(&rn->قطار);

        إذا (rn->اڈیک == عدم) {
            ngx_rbtree_delete(&r->name_rbtree, &rn->گرہ);
            ngx_resolver_free_گرہ(r, rn);
            اذهب اگلا;
        }

        rec = r->connections.elts;
        rec = &rec[rn->آخری_connection];

        بدل (qtype) {

#إذا (NGX_HAVE_INET6)
        حالة NGX_RESOLVE_AAAA:

            rn->tcp6 = 1;

            (فراغ) ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen);

            اكسر;
#جے_انت

        افتراضي: /* NGX_RESOLVE_A */

            rn->tcp = 1;

            (فراغ) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);
        }

        rn->expire = ngx_ویلا() + r->resend_timeout;

        ngx_قطار_داخل_کرو_سر(&r->name_resend_قطار, &rn->قطار);

        اذهب اگلا;
    }

    إذا (کوڈ == 0 && rn->کوڈ) {
        کوڈ = rn->کوڈ;
    }

    إذا (کوڈ == 0 && nan == 0) {

#إذا (NGX_HAVE_INET6)
        بدل (qtype) {

        حالة NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            إذا (rn->naddrs == (u_short) -1) {
                اذهب اگلا;
            }

            إذا (rn->naddrs) {
                اذهب export;
            }

            اكسر;

        افتراضي: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            إذا (rn->naddrs6 == (u_short) -1) {
                اذهب اگلا;
            }

            إذا (rn->naddrs6) {
                اذهب export;
            }
        }
#جے_انت

        کوڈ = NGX_RESOLVE_NXDOMAIN;
    }

    إذا (کوڈ) {

#إذا (NGX_HAVE_INET6)
        بدل (qtype) {

        حالة NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            إذا (rn->naddrs == (u_short) -1) {
                rn->کوڈ = (u_char) کوڈ;
                اذهب اگلا;
            }

            اكسر;

        افتراضي: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            إذا (rn->naddrs6 == (u_short) -1) {
                rn->کوڈ = (u_char) کوڈ;
                اذهب اگلا;
            }
        }
#جے_انت

        اگلا = rn->اڈیک;
        rn->اڈیک = عدم;

        ngx_قطار_remove(&rn->قطار);

        ngx_rbtree_delete(&r->name_rbtree, &rn->گرہ);

        /* unlock name mutex */

        طالما (اگلا) {
            ctx = اگلا;
            ctx->state = کوڈ;
            ctx->valid = ngx_ویلا() + (r->valid ? r->valid : 10);
            اگلا = ctx->اگلا;

            ctx->handler(ctx);
        }

        ngx_resolver_free_گرہ(r, rn);

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
                اذهب لبھیا;
            }

            إذا (buf[i] == 0) {
                i++;
                اذهب test_لمائی;
            }

            i += 1 + buf[i];
        }

        اذهب short_response;

    test_لمائی:

        إذا (i - شروع < 2) {
            err = "invalid name in DNS response";
            اذهب invalid;
        }

    لبھیا:

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
            ngx_log_error(r->log_level, r->لوگارتھم, 0,
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
#جے_انت

        حالة NGX_RESOLVE_CNAME:

            cname = &buf[i];

            اكسر;

        حالة NGX_RESOLVE_DNAME:

            اكسر;

        افتراضي:

            ngx_log_error(r->log_level, r->لوگارتھم, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0,
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
#جے_انت

            اكسر;
#جے_انت

        افتراضي: /* NGX_RESOLVE_A */

            إذا (naddrs == 1) {
                addr = &rn->u.addr;
                rn->naddrs = 1;

            } وإلا {
                addr = ngx_resolver_alloc(r, naddrs * حجم(وچ_addr_t));
                إذا (addr == عدم) {
                    اذهب failed;
                }

                rn->u.addrs = addr;
                rn->naddrs = (u_short) naddrs;
            }

#إذا (NGX_HAVE_INET6 && NGX_SUPPRESS_WARN)
            addr6 = عدم;
#جے_انت
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
                        اذهب اگلا;
                    }
#جے_انت

                    اكسر;
                }
            }

#إذا (NGX_HAVE_INET6)
            وإلا إذا (type == NGX_RESOLVE_AAAA) {

                ngx_memcpy(addr6[j].s6_addr, &buf[i], 16);

                إذا (++j == naddrs) {

                    إذا (rn->naddrs == (u_short) -1) {
                        اذهب اگلا;
                    }

                    اكسر;
                }
            }
#جے_انت

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
#جے_انت

    افتراضي: /* NGX_RESOLVE_A */

        إذا (rn->naddrs == (u_short) -1) {
            rn->naddrs = 0;
        }
    }

    إذا (rn->naddrs != (u_short) -1
#إذا (NGX_HAVE_INET6)
        && rn->naddrs6 != (u_short) -1
#جے_انت
        && rn->naddrs
#إذا (NGX_HAVE_INET6)
           + rn->naddrs6
#جے_انت
           > 0)
    {

#إذا (NGX_HAVE_INET6)
    export:
#جے_انت

        naddrs = rn->naddrs;
#إذا (NGX_HAVE_INET6)
        naddrs += rn->naddrs6;
#جے_انت

        إذا (naddrs == 1 && rn->naddrs == 1) {
            addrs = عدم;

        } وإلا {
            addrs = ngx_resolver_export(r, rn, 0);
            إذا (addrs == عدم) {
                اذهب failed;
            }
        }

        ngx_قطار_remove(&rn->قطار);

        rn->valid = ngx_ویلا() + (r->valid ? r->valid : (ویلا_t) rn->ttl);
        rn->expire = ngx_ویلا() + r->expire;

        ngx_قطار_داخل_کرو_سر(&r->name_expire_قطار, &rn->قطار);

        اگلا = rn->اڈیک;
        rn->اڈیک = عدم;

        /* unlock name mutex */

        طالما (اگلا) {
            ctx = اگلا;
            ctx->state = NGX_OK;
            ctx->valid = rn->valid;
            ctx->naddrs = naddrs;

            إذا (addrs == عدم) {
                ctx->addrs = &ctx->addr;
                ctx->addr.sockaddr = (بنية sockaddr *) &ctx->جیب;
                ctx->addr.socklen = حجم(بنية sockaddr_وچ);
                ngx_memzero(&ctx->جیب, حجم(بنية sockaddr_وچ));
                ctx->جیب.sin_family = AF_INET;
                ctx->جیب.sin_addr.s_addr = rn->u.addr;

            } وإلا {
                ctx->addrs = addrs;
            }

            اگلا = ctx->اگلا;

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
#جے_انت

        أرجع;
    }

    إذا (cname) {

        /* CNAME only */

        إذا (rn->naddrs == (u_short) -1
#إذا (NGX_HAVE_INET6)
            || rn->naddrs6 == (u_short) -1
#جے_انت
            )
        {
            اذهب اگلا;
        }

        إذا (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            اذهب failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_قطار_remove(&rn->قطار);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.انکڑے;

        rn->valid = ngx_ویلا() + (r->valid ? r->valid : (ویلا_t) rn->ttl);
        rn->expire = ngx_ویلا() + r->expire;

        ngx_قطار_داخل_کرو_سر(&r->name_expire_قطار, &rn->قطار);

        ngx_resolver_free(r, rn->query);
        rn->query = عدم;
#إذا (NGX_HAVE_INET6)
        rn->query6 = عدم;
#جے_انت

        ctx = rn->اڈیک;
        rn->اڈیک = عدم;

        إذا (ctx) {

            إذا (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                افعل {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    اگلا = ctx->اگلا;

                    ctx->handler(ctx);

                    ctx = اگلا;
                } طالما (ctx);

                أرجع;
            }

            لكل (اگلا = ctx; اگلا; اگلا = اگلا->اگلا) {
                اگلا->گرہ = عدم;
            }

            (فراغ) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        أرجع;
    }

    ngx_log_error(r->log_level, r->لوگارتھم, 0,
                  "no A or CNAME types in DNS response");
    أرجع;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->لوگارتھم, 0, err);

    أرجع;

failed:

اگلا:

    /* unlock name mutex */

    أرجع;
}


ساكن فراغ
ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, ناپ_t n,
    ngx_uint_t ident, ngx_uint_t کوڈ, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans)
{
    حرف                       *err;
    u_char                     *cname;
    ناپ_t                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    ngx_str_t                   name;
    ngx_uint_t                  type, qident, class, شروع, nsrvs, a, i, j;
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *اگلا;
    ngx_resolver_srv_t         *srvs;
    ngx_resolver_گرہ_t        *rn;
    ngx_resolver_connection_t  *rec;

    إذا (ngx_resolver_copy(r, &name, buf,
                          buf + حجم(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        أرجع;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0, "resolver qs:%V", &name);

    hash = ngx_چکری_جانچ_32_short(name.انکڑے, name.len);

    rn = ngx_resolver_lookup_srv(r, &name, hash);

    إذا (rn == عدم || rn->query == عدم) {
        ngx_log_error(r->log_level, r->لوگارتھم, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.انکڑے);
        اذهب failed;
    }

    إذا (trunc && rn->tcp) {
        ngx_resolver_free(r, name.انکڑے);
        اذهب failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    إذا (ident != qident) {
        ngx_log_error(r->log_level, r->لوگارتھم, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.انکڑے);
        اذهب failed;
    }

    ngx_resolver_free(r, name.انکڑے);

    إذا (trunc) {

        ngx_قطار_remove(&rn->قطار);

        إذا (rn->اڈیک == عدم) {
            ngx_rbtree_delete(&r->srv_rbtree, &rn->گرہ);
            ngx_resolver_free_گرہ(r, rn);
            أرجع;
        }

        rec = r->connections.elts;
        rec = &rec[rn->آخری_connection];

        rn->tcp = 1;

        (فراغ) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);

        rn->expire = ngx_ویلا() + r->resend_timeout;

        ngx_قطار_داخل_کرو_سر(&r->srv_resend_قطار, &rn->قطار);

        أرجع;
    }

    إذا (کوڈ == 0 && rn->کوڈ) {
        کوڈ = rn->کوڈ;
    }

    إذا (کوڈ == 0 && nan == 0) {
        کوڈ = NGX_RESOLVE_NXDOMAIN;
    }

    إذا (کوڈ) {
        اگلا = rn->اڈیک;
        rn->اڈیک = عدم;

        ngx_قطار_remove(&rn->قطار);

        ngx_rbtree_delete(&r->srv_rbtree, &rn->گرہ);

        طالما (اگلا) {
            ctx = اگلا;
            ctx->state = کوڈ;
            ctx->valid = ngx_ویلا() + (r->valid ? r->valid : 10);
            اگلا = ctx->اگلا;

            ctx->handler(ctx);
        }

        ngx_resolver_free_گرہ(r, rn);

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
                اذهب لبھیا;
            }

            إذا (buf[i] == 0) {
                i++;
                اذهب test_لمائی;
            }

            i += 1 + buf[i];
        }

        اذهب short_response;

    test_لمائی:

        إذا (i - شروع < 2) {
            err = "invalid name DNS response";
            اذهب invalid;
        }

    لبھیا:

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
            ngx_log_error(r->log_level, r->لوگارتھم, 0,
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

            ngx_log_error(r->log_level, r->لوگارتھم, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0,
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

                srvs[j].ترجیح = (buf[i] << 8) + buf[i + 1];
                srvs[j].بھار = (buf[i + 2] << 8) + buf[i + 3];

                إذا (srvs[j].بھار == 0) {
                    srvs[j].بھار = 1;
                }

                srvs[j].پورٹ = (buf[i + 4] << 8) + buf[i + 5];

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

        ngx_ترتیب_دیو(srvs, nsrvs, حجم(ngx_resolver_srv_t),
                 ngx_resolver_cmp_srvs);

        ngx_resolver_free(r, rn->query);
        rn->query = عدم;

        ngx_قطار_remove(&rn->قطار);

        rn->valid = ngx_ویلا() + (r->valid ? r->valid : (ویلا_t) rn->ttl);
        rn->expire = ngx_ویلا() + r->expire;

        ngx_قطار_داخل_کرو_سر(&r->srv_expire_قطار, &rn->قطار);

        اگلا = rn->اڈیک;
        rn->اڈیک = عدم;

        طالما (اگلا) {
            ctx = اگلا;
            اگلا = ctx->اگلا;

            ngx_resolver_resolve_srv_ناں(ctx, rn);
        }

        أرجع;
    }

    rn->nsrvs = 0;

    إذا (cname) {

        /* CNAME only */

        إذا (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            اذهب failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_قطار_remove(&rn->قطار);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.انکڑے;

        rn->valid = ngx_ویلا() + (r->valid ? r->valid : (ویلا_t) rn->ttl);
        rn->expire = ngx_ویلا() + r->expire;

        ngx_قطار_داخل_کرو_سر(&r->srv_expire_قطار, &rn->قطار);

        ngx_resolver_free(r, rn->query);
        rn->query = عدم;
#إذا (NGX_HAVE_INET6)
        rn->query6 = عدم;
#جے_انت

        ctx = rn->اڈیک;
        rn->اڈیک = عدم;

        إذا (ctx) {

            إذا (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                افعل {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    اگلا = ctx->اگلا;

                    ctx->handler(ctx);

                    ctx = اگلا;
                } طالما (ctx);

                أرجع;
            }

            لكل (اگلا = ctx; اگلا; اگلا = اگلا->اگلا) {
                اگلا->گرہ = عدم;
            }

            (فراغ) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        أرجع;
    }

    ngx_log_error(r->log_level, r->لوگارتھم, 0, "no SRV type in DNS response");

    أرجع;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->لوگارتھم, 0, err);

    أرجع;

failed:

    /* unlock name mutex */

    أرجع;
}


ساكن فراغ
ngx_resolver_resolve_srv_ناں(ngx_resolver_ctx_t *ctx, ngx_resolver_گرہ_t *rn)
{
    ngx_uint_t                i;
    ngx_resolver_t           *r;
    ngx_resolver_ctx_t       *cctx;
    ngx_resolver_srv_name_t  *srvs;

    r = ctx->resolver;

    ctx->گرہ = عدم;
    ctx->state = NGX_OK;
    ctx->valid = rn->valid;
    ctx->گنتی = rn->nsrvs;

    srvs = ngx_resolver_calloc(r, rn->nsrvs * حجم(ngx_resolver_srv_name_t));
    إذا (srvs == عدم) {
        اذهب failed;
    }

    ctx->srvs = srvs;
    ctx->nsrvs = rn->nsrvs;

    إذا (ctx->event && ctx->event->timer_سیٹ) {
        ngx_del_timer(ctx->event);
    }

    لكل (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
        srvs[i].name.انکڑے = ngx_resolver_alloc(r, rn->u.srvs[i].name.len);
        إذا (srvs[i].name.انکڑے == عدم) {
            اذهب failed;
        }

        srvs[i].name.len = rn->u.srvs[i].name.len;
        ngx_memcpy(srvs[i].name.انکڑے, rn->u.srvs[i].name.انکڑے,
                   srvs[i].name.len);

        cctx = ngx_resolve_شروع(r, عدم);
        إذا (cctx == عدم) {
            اذهب failed;
        }

        cctx->name = srvs[i].name;
        cctx->handler = ngx_resolver_srv_ناں_handler;
        cctx->انکڑے = ctx;
        cctx->srvs = &srvs[i];
        cctx->timeout = ctx->timeout;

        srvs[i].ترجیح = rn->u.srvs[i].ترجیح;
        srvs[i].بھار = rn->u.srvs[i].بھار;
        srvs[i].پورٹ = rn->u.srvs[i].پورٹ;
        srvs[i].ctx = cctx;

        إذا (ngx_resolve_name(cctx) == NGX_ERROR) {
            srvs[i].ctx = عدم;
            اذهب failed;
        }
    }

    أرجع;

failed:

    ctx->state = NGX_ERROR;
    ctx->valid = ngx_ویلا() + (r->valid ? r->valid : 10);

    ctx->handler(ctx);
}


ساكن فراغ
ngx_resolver_srv_ناں_handler(ngx_resolver_ctx_t *cctx)
{
    ngx_uint_t                i;
    ngx_addr_t               *addrs;
    ngx_resolver_t           *r;
    ngx_sockaddr_t           *sockaddr;
    ngx_resolver_ctx_t       *ctx;
    ngx_resolver_srv_name_t  *srv;

    r = cctx->resolver;
    ctx = cctx->انکڑے;
    srv = cctx->srvs;

    ctx->گنتی--;
    ctx->async |= cctx->async;

    srv->ctx = عدم;
    srv->state = cctx->state;

    إذا (cctx->naddrs) {

        ctx->valid = ngx_min(ctx->valid, cctx->valid);

        addrs = ngx_resolver_calloc(r, cctx->naddrs * حجم(ngx_addr_t));
        إذا (addrs == عدم) {
            srv->state = NGX_ERROR;
            اذهب پورا;
        }

        sockaddr = ngx_resolver_alloc(r, cctx->naddrs * حجم(ngx_sockaddr_t));
        إذا (sockaddr == عدم) {
            ngx_resolver_free(r, addrs);
            srv->state = NGX_ERROR;
            اذهب پورا;
        }

        لكل (i = 0; i < cctx->naddrs; i++) {
            addrs[i].sockaddr = &sockaddr[i].sockaddr;
            addrs[i].socklen = cctx->addrs[i].socklen;

            ngx_memcpy(&sockaddr[i], cctx->addrs[i].sockaddr,
                       addrs[i].socklen);

            ngx_inet_سیٹ_پورٹ(addrs[i].sockaddr, srv->پورٹ);
        }

        srv->addrs = addrs;
        srv->naddrs = cctx->naddrs;
    }

پورا:

    ngx_resolve_name_پورا(cctx);

    إذا (ctx->گنتی == 0) {
        ngx_resolver_report_srv(r, ctx);
    }
}


ساكن فراغ
ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, ناپ_t n,
    ngx_uint_t ident, ngx_uint_t کوڈ, ngx_uint_t nan)
{
    حرف                 *err;
    ناپ_t                len;
    وچ_addr_t             addr;
    int32_t               ttl;
    ngx_int_t             octet;
    ngx_str_t             name;
    ngx_uint_t            نقاب, type, class, qident, a, i, شروع;
    ngx_قطار_t          *expire_قطار;
    ngx_rbtree_t         *درخت;
    ngx_resolver_an_t    *an;
    ngx_resolver_ctx_t   *ctx, *اگلا;
    ngx_resolver_گرہ_t  *rn;
#إذا (NGX_HAVE_INET6)
    uint32_t              hash;
    ngx_int_t             digit;
    بنية in6_addr       addr6;
#جے_انت

    إذا (ngx_resolver_copy(r, &name, buf,
                          buf + حجم(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        أرجع;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0, "resolver qs:%V", &name);

    /* AF_INET */

    addr = 0;
    i = حجم(ngx_resolver_hdr_t);

    لكل (نقاب = 0; نقاب < 32; نقاب += 8) {
        len = buf[i++];

        octet = ngx_atoi(&buf[i], len);
        إذا (octet == NGX_ERROR || octet > 255) {
            اذهب invalid_وچ_addr_arpa;
        }

        addr += octet << نقاب;
        i += len;
    }

    إذا (ngx_strcasecmp(&buf[i], (u_char *) "\7in-addr\4arpa") == 0) {
        i += حجم("\7in-addr\4arpa");

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        درخت = &r->addr_rbtree;
        expire_قطار = &r->addr_expire_قطار;

        اذهب valid;
    }

invalid_وچ_addr_arpa:

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

        hash = ngx_چکری_جانچ_32_short(addr6.s6_addr, 16);
        rn = ngx_resolver_lookup_addr6(r, &addr6, hash);

        درخت = &r->addr6_rbtree;
        expire_قطار = &r->addr6_expire_قطار;

        اذهب valid;
    }

invalid_ip6_arpa:
#جے_انت

    ngx_log_error(r->log_level, r->لوگارتھم, 0,
                  "invalid in-addr.arpa or ip6.arpa name in DNS response");
    ngx_resolver_free(r, name.انکڑے);
    أرجع;

valid:

    إذا (rn == عدم || rn->query == عدم) {
        ngx_log_error(r->log_level, r->لوگارتھم, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.انکڑے);
        اذهب failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    إذا (ident != qident) {
        ngx_log_error(r->log_level, r->لوگارتھم, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.انکڑے);
        اذهب failed;
    }

    ngx_resolver_free(r, name.انکڑے);

    إذا (کوڈ == 0 && nan == 0) {
        کوڈ = NGX_RESOLVE_NXDOMAIN;
    }

    إذا (کوڈ) {
        اگلا = rn->اڈیک;
        rn->اڈیک = عدم;

        ngx_قطار_remove(&rn->قطار);

        ngx_rbtree_delete(درخت, &rn->گرہ);

        /* unlock addr mutex */

        طالما (اگلا) {
            ctx = اگلا;
            ctx->state = کوڈ;
            ctx->valid = ngx_ویلا() + (r->valid ? r->valid : 10);
            اگلا = ctx->اگلا;

            ctx->handler(ctx);
        }

        ngx_resolver_free_گرہ(r, rn);

        أرجع;
    }

    i += حجم(ngx_resolver_qs_t);

    لكل (a = 0; a < nan; a++) {

        شروع = i;

        طالما (i < n) {

            إذا (buf[i] & 0xc0) {
                i += 2;
                اذهب لبھیا;
            }

            إذا (buf[i] == 0) {
                i++;
                اذهب test_لمائی;
            }

            i += 1 + buf[i];
        }

        اذهب short_response;

    test_لمائی:

        إذا (i - شروع < 2) {
            err = "invalid name in DNS response";
            اذهب invalid;
        }

    لبھیا:

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
            ngx_log_error(r->log_level, r->لوگارتھم, 0,
                          "unexpected RR class %ui in DNS response", class);
            اذهب failed;
        }

        إذا (ttl < 0) {
            ttl = 0;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0,
                      "resolver qt:%ui cl:%ui len:%uz",
                      type, class, len);

        i += حجم(ngx_resolver_an_t);

        بدل (type) {

        حالة NGX_RESOLVE_PTR:

            اذهب ptr;

        حالة NGX_RESOLVE_CNAME:

            اكسر;

        افتراضي:

            ngx_log_error(r->log_level, r->لوگارتھم, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->لوگارتھم, 0,
                  "no PTR type in DNS response");
    أرجع;

ptr:

    إذا (ngx_resolver_copy(r, &name, buf, buf + i, buf + n) != NGX_OK) {
        اذهب failed;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0, "resolver an:%V", &name);

    إذا (name.len != (ناپ_t) rn->nlen
        || ngx_strncmp(name.انکڑے, rn->name, name.len) != 0)
    {
        إذا (rn->nlen) {
            ngx_resolver_free(r, rn->name);
        }

        rn->nlen = (u_short) name.len;
        rn->name = name.انکڑے;

        name.انکڑے = ngx_resolver_dup(r, rn->name, name.len);
        إذا (name.انکڑے == عدم) {
            اذهب failed;
        }
    }

    ngx_قطار_remove(&rn->قطار);

    rn->valid = ngx_ویلا() + (r->valid ? r->valid : ttl);
    rn->expire = ngx_ویلا() + r->expire;

    ngx_قطار_داخل_کرو_سر(expire_قطار, &rn->قطار);

    اگلا = rn->اڈیک;
    rn->اڈیک = عدم;

    /* unlock addr mutex */

    طالما (اگلا) {
        ctx = اگلا;
        ctx->state = NGX_OK;
        ctx->valid = rn->valid;
        ctx->name = name;
        اگلا = ctx->اگلا;

        ctx->handler(ctx);
    }

    ngx_resolver_free(r, name.انکڑے);

    أرجع;

short_response:

    err = "short DNS response";

invalid:

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->لوگارتھم, 0, err);

    أرجع;

failed:

    /* unlock addr mutex */

    أرجع;
}


ساكن ngx_resolver_گرہ_t *
ngx_resolver_lookup_name(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_گرہ_t    *گرہ, *sentinel;
    ngx_resolver_گرہ_t  *rn;

    گرہ = r->name_rbtree.جڑ;
    sentinel = r->name_rbtree.sentinel;

    طالما (گرہ != sentinel) {

        إذا (hash < گرہ->کلید) {
            گرہ = گرہ->بایاں;
            استمر;
        }

        إذا (hash > گرہ->کلید) {
            گرہ = گرہ->دایاں;
            استمر;
        }

        /* hash == node->key */

        rn = ngx_resolver_گرہ(گرہ);

        rc = ngx_memn2cmp(name->انکڑے, rn->name, name->len, rn->nlen);

        إذا (rc == 0) {
            أرجع rn;
        }

        گرہ = (rc < 0) ? گرہ->بایاں : گرہ->دایاں;
    }

    /* not found */

    أرجع عدم;
}


ساكن ngx_resolver_گرہ_t *
ngx_resolver_lookup_srv(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_گرہ_t    *گرہ, *sentinel;
    ngx_resolver_گرہ_t  *rn;

    گرہ = r->srv_rbtree.جڑ;
    sentinel = r->srv_rbtree.sentinel;

    طالما (گرہ != sentinel) {

        إذا (hash < گرہ->کلید) {
            گرہ = گرہ->بایاں;
            استمر;
        }

        إذا (hash > گرہ->کلید) {
            گرہ = گرہ->دایاں;
            استمر;
        }

        /* hash == node->key */

        rn = ngx_resolver_گرہ(گرہ);

        rc = ngx_memn2cmp(name->انکڑے, rn->name, name->len, rn->nlen);

        إذا (rc == 0) {
            أرجع rn;
        }

        گرہ = (rc < 0) ? گرہ->بایاں : گرہ->دایاں;
    }

    /* not found */

    أرجع عدم;
}


ساكن ngx_resolver_گرہ_t *
ngx_resolver_lookup_addr(ngx_resolver_t *r, وچ_addr_t addr)
{
    ngx_rbtree_گرہ_t  *گرہ, *sentinel;

    گرہ = r->addr_rbtree.جڑ;
    sentinel = r->addr_rbtree.sentinel;

    طالما (گرہ != sentinel) {

        إذا (addr < گرہ->کلید) {
            گرہ = گرہ->بایاں;
            استمر;
        }

        إذا (addr > گرہ->کلید) {
            گرہ = گرہ->دایاں;
            استمر;
        }

        /* addr == node->key */

        أرجع ngx_resolver_گرہ(گرہ);
    }

    /* not found */

    أرجع عدم;
}


#إذا (NGX_HAVE_INET6)

ساكن ngx_resolver_گرہ_t *
ngx_resolver_lookup_addr6(ngx_resolver_t *r, بنية in6_addr *addr,
    uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_گرہ_t    *گرہ, *sentinel;
    ngx_resolver_گرہ_t  *rn;

    گرہ = r->addr6_rbtree.جڑ;
    sentinel = r->addr6_rbtree.sentinel;

    طالما (گرہ != sentinel) {

        إذا (hash < گرہ->کلید) {
            گرہ = گرہ->بایاں;
            استمر;
        }

        إذا (hash > گرہ->کلید) {
            گرہ = گرہ->دایاں;
            استمر;
        }

        /* hash == node->key */

        rn = ngx_resolver_گرہ(گرہ);

        rc = ngx_memcmp(addr, &rn->addr6, 16);

        إذا (rc == 0) {
            أرجع rn;
        }

        گرہ = (rc < 0) ? گرہ->بایاں : گرہ->دایاں;
    }

    /* not found */

    أرجع عدم;
}

#جے_انت


ساكن فراغ
ngx_resolver_rbtree_داخل_کرو_قدر(ngx_rbtree_گرہ_t *عارضی,
    ngx_rbtree_گرہ_t *گرہ, ngx_rbtree_گرہ_t *sentinel)
{
    ngx_rbtree_گرہ_t    **p;
    ngx_resolver_گرہ_t   *rn, *rn_عارضی;

    لكل ( ;; ) {

        إذا (گرہ->کلید < عارضی->کلید) {

            p = &عارضی->بایاں;

        } وإلا إذا (گرہ->کلید > عارضی->کلید) {

            p = &عارضی->دایاں;

        } وإلا { /* node->key == temp->key */

            rn = ngx_resolver_گرہ(گرہ);
            rn_عارضی = ngx_resolver_گرہ(عارضی);

            p = (ngx_memn2cmp(rn->name, rn_عارضی->name, rn->nlen, rn_عارضی->nlen)
                 < 0) ? &عارضی->بایاں : &عارضی->دایاں;
        }

        إذا (*p == sentinel) {
            اكسر;
        }

        عارضی = *p;
    }

    *p = گرہ;
    گرہ->ماپا = عارضی;
    گرہ->بایاں = sentinel;
    گرہ->دایاں = sentinel;
    ngx_rbt_red(گرہ);
}


#إذا (NGX_HAVE_INET6)

ساكن فراغ
ngx_resolver_rbtree_داخل_کرو_addr6_قدر(ngx_rbtree_گرہ_t *عارضی,
    ngx_rbtree_گرہ_t *گرہ, ngx_rbtree_گرہ_t *sentinel)
{
    ngx_rbtree_گرہ_t    **p;
    ngx_resolver_گرہ_t   *rn, *rn_عارضی;

    لكل ( ;; ) {

        إذا (گرہ->کلید < عارضی->کلید) {

            p = &عارضی->بایاں;

        } وإلا إذا (گرہ->کلید > عارضی->کلید) {

            p = &عارضی->دایاں;

        } وإلا { /* node->key == temp->key */

            rn = ngx_resolver_گرہ(گرہ);
            rn_عارضی = ngx_resolver_گرہ(عارضی);

            p = (ngx_memcmp(&rn->addr6, &rn_عارضی->addr6, 16)
                 < 0) ? &عارضی->بایاں : &عارضی->دایاں;
        }

        إذا (*p == sentinel) {
            اكسر;
        }

        عارضی = *p;
    }

    *p = گرہ;
    گرہ->ماپا = عارضی;
    گرہ->بایاں = sentinel;
    گرہ->دایاں = sentinel;
    ngx_rbt_red(گرہ);
}

#جے_انت


ساكن ngx_int_t
ngx_resolver_create_name_query(ngx_resolver_t *r, ngx_resolver_گرہ_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    ناپ_t               len, nlen;
    ngx_uint_t           ident;
    ngx_resolver_qs_t   *qs;
    ngx_resolver_hdr_t  *query;

    nlen = name->len ? (1 + name->len + 1) : 1;

    len = حجم(ngx_resolver_hdr_t) + nlen + حجم(ngx_resolver_qs_t);

#إذا (NGX_HAVE_INET6)
    p = ngx_resolver_alloc(r, len * (r->ipv4 + r->ipv6));
#وإلا
    p = ngx_resolver_alloc(r, len);
#جے_انت
    إذا (p == عدم) {
        أرجع NGX_ERROR;
    }

    rn->qlen = (u_short) len;
    rn->query = p;

#إذا (NGX_HAVE_INET6)
    إذا (r->ipv6) {
        rn->query6 = r->ipv4 ? (p + len) : p;
    }
#جے_انت

    query = (ngx_resolver_hdr_t *) p;

    إذا (r->ipv4) {
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

    إذا (name->len == 0)  {
        أرجع NGX_DECLINED;
    }

    لكل (s = name->انکڑے + name->len - 1; s >= name->انکڑے; s--) {
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

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->لوگارتھم, 0,
                   "resolve: \"%V\" AAAA %i", name, ident & 0xffff);

    query->ident_hi = (u_char) ((ident >> 8) & 0xff);
    query->ident_lo = (u_char) (ident & 0xff);

    p += حجم(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    qs->type_lo = NGX_RESOLVE_AAAA;
#جے_انت

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_resolver_create_srv_query(ngx_resolver_t *r, ngx_resolver_گرہ_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    ناپ_t               len, nlen;
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

    إذا (name->len == 0)  {
        أرجع NGX_DECLINED;
    }

    لكل (s = name->انکڑے + name->len - 1; s >= name->انکڑے; s--) {
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
ngx_resolver_create_addr_query(ngx_resolver_t *r, ngx_resolver_گرہ_t *rn,
    ngx_resolver_addr_t *addr)
{
    u_char               *p, *d;
    ناپ_t                len;
    وچ_addr_t             inaddr;
    ngx_int_t             n;
    ngx_uint_t            ident;
    ngx_resolver_hdr_t   *query;
    بنية sockaddr_وچ   *جیب;
#إذا (NGX_HAVE_INET6)
    بنية sockaddr_in6  *sin6;
#جے_انت

    بدل (addr->sockaddr->sa_family) {

#إذا (NGX_HAVE_INET6)
    حالة AF_INET6:
        len = حجم(ngx_resolver_hdr_t)
              + 64 + حجم(".ip6.arpa.") - 1
              + حجم(ngx_resolver_qs_t);

        اكسر;
#جے_انت

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
#جے_انت

    افتراضي: /* AF_INET */

        جیب = (بنية sockaddr_وچ *) addr->sockaddr;
        inaddr = ntohl(جیب->sin_addr.s_addr);

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
    u_char *آخری)
{
    حرف        *err;
    u_char      *p, *dst;
    ناپ_t       len;
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
            اذهب پورا;
        }

        إذا (n & 0xc0) {
            إذا ((n & 0xc0) != 0xc0) {
                err = "invalid label type in DNS response";
                اذهب invalid;
            }

            إذا (p >= آخری) {
                err = "name is out of DNS response";
                اذهب invalid;
            }

            n = ((n & 0x3f) << 8) + *p;
            p = &buf[n];

        } وإلا {
            len += 1 + n;
            p = &p[n];
        }

        إذا (p >= آخری) {
            err = "name is out of DNS response";
            اذهب invalid;
        }
    }

    err = "compression pointers loop in DNS response";

invalid:

    ngx_log_error(r->log_level, r->لوگارتھم, 0, err);

    أرجع NGX_ERROR;

پورا:

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

    name->انکڑے = dst;

    لكل ( ;; ) {
        n = *src++;

        إذا (n == 0) {
            name->len = dst - name->انکڑے - 1;
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
ngx_resolver_سیٹ_timeout(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx)
{
    إذا (ctx->event || ctx->timeout == 0) {
        أرجع NGX_OK;
    }

    ctx->event = ngx_resolver_calloc(r, حجم(ngx_event_t));
    إذا (ctx->event == عدم) {
        أرجع NGX_ERROR;
    }

    ctx->event->handler = ngx_resolver_timeout_handler;
    ctx->event->انکڑے = ctx;
    ctx->event->لوگارتھم = r->لوگارتھم;
    ctx->event->cancelable = ctx->cancelable;
    ctx->ident = -1;

    ngx_جوڑو_timer(ctx->event, ctx->timeout);

    أرجع NGX_OK;
}


ساكن فراغ
ngx_resolver_timeout_handler(ngx_event_t *ev)
{
    ngx_resolver_ctx_t  *ctx;

    ctx = ev->انکڑے;

    ctx->state = NGX_RESOLVE_TIMEDOUT;

    ctx->handler(ctx);
}


ساكن فراغ
ngx_resolver_free_گرہ(ngx_resolver_t *r, ngx_resolver_گرہ_t *rn)
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
#جے_انت

    إذا (rn->nsrvs) {
        لكل (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
            إذا (rn->u.srvs[i].name.انکڑے) {
                ngx_resolver_free_locked(r, rn->u.srvs[i].name.انکڑے);
            }
        }

        ngx_resolver_free_locked(r, rn->u.srvs);
    }

    ngx_resolver_free_locked(r, rn);

    /* unlock alloc mutex */
}


ساكن فراغ *
ngx_resolver_alloc(ngx_resolver_t *r, ناپ_t ناپ)
{
    u_char  *p;

    /* lock alloc mutex */

    p = ngx_alloc(ناپ, r->لوگارتھم);

    /* unlock alloc mutex */

    أرجع p;
}


ساكن فراغ *
ngx_resolver_calloc(ngx_resolver_t *r, ناپ_t ناپ)
{
    u_char  *p;

    p = ngx_resolver_alloc(r, ناپ);

    إذا (p) {
        ngx_memzero(p, ناپ);
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
ngx_resolver_dup(ngx_resolver_t *r, فراغ *src, ناپ_t ناپ)
{
    فراغ  *dst;

    dst = ngx_resolver_alloc(r, ناپ);

    إذا (dst == عدم) {
        أرجع dst;
    }

    ngx_memcpy(dst, src, ناپ);

    أرجع dst;
}


ساكن ngx_resolver_addr_t *
ngx_resolver_export(ngx_resolver_t *r, ngx_resolver_گرہ_t *rn,
    ngx_uint_t rotate)
{
    ngx_uint_t            d, i, j, n;
    وچ_addr_t            *addr;
    ngx_sockaddr_t       *sockaddr;
    بنية sockaddr_وچ   *جیب;
    ngx_resolver_addr_t  *dst;
#إذا (NGX_HAVE_INET6)
    بنية in6_addr      *addr6;
    بنية sockaddr_in6  *sin6;
#جے_انت

    n = rn->naddrs;
#إذا (NGX_HAVE_INET6)
    n += rn->naddrs6;
#جے_انت

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
            جیب = &sockaddr[d].sockaddr_وچ;
            جیب->sin_family = AF_INET;
            جیب->sin_addr.s_addr = addr[j++];
            dst[d].sockaddr = (بنية sockaddr *) جیب;
            dst[d++].socklen = حجم(بنية sockaddr_وچ);

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
#جے_انت

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
            ctx->valid = ngx_ویلا() + (r->valid ? r->valid : 10);

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

        ctx->valid = ngx_ویلا() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        أرجع;
    }

    addrs = ngx_resolver_calloc(r, naddrs * حجم(ngx_resolver_addr_t));
    إذا (addrs == عدم) {
        ctx->state = NGX_ERROR;
        ctx->valid = ngx_ویلا() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        أرجع;
    }

    i = 0;
    n = 0;

    افعل {
        nw = 0;

        لكل (j = i; j < nsrvs; j++) {
            إذا (srvs[j].ترجیح != srvs[i].ترجیح) {
                اكسر;
            }

            nw += srvs[j].naddrs * srvs[j].بھار;
        }

        إذا (nw == 0) {
            اذهب اگلا_srv;
        }

        w = ngx_random() % nw;

        لكل (k = i; k < j; k++) {
            إذا (w < srvs[k].naddrs * srvs[k].بھار) {
                اكسر;
            }

            w -= srvs[k].naddrs * srvs[k].بھار;
        }

        لكل (l = i; l < j; l++) {

            لكل (m = 0; m < srvs[k].naddrs; m++) {
                addrs[n].socklen = srvs[k].addrs[m].socklen;
                addrs[n].sockaddr = srvs[k].addrs[m].sockaddr;
                addrs[n].name = srvs[k].name;
                addrs[n].ترجیح = srvs[k].ترجیح;
                addrs[n].بھار = srvs[k].بھار;
                n++;
            }

            إذا (++k == j) {
                k = i;
            }
        }

اگلا_srv:

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
ngx_resolver_log_error(ngx_log_t *لوگارتھم, u_char *buf, ناپ_t len)
{
    u_char                     *p;
    ngx_resolver_connection_t  *rec;

    p = buf;

    إذا (لوگارتھم->action) {
        p = ngx_snprintf(buf, len, " while %s", لوگارتھم->action);
        len -= p - buf;
    }

    rec = لوگارتھم->انکڑے;

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

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->لوگارتھم, 0, "UDP socket %d", s);

    إذا (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->لوگارتھم, ngx_socket_errno,
                      ngx_socket_n " failed");
        أرجع NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->لوگارتھم);

    إذا (c == عدم) {
        إذا (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->لوگارتھم, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        أرجع NGX_ERROR;
    }

    إذا (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->لوگارتھم, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        اذهب failed;
    }

    rev = c->اقرأ;
    wev = c->اكتب;

    rev->لوگارتھم = &rec->لوگارتھم;
    wev->لوگارتھم = &rec->لوگارتھم;

    rec->udp = c;

    c->نمبر = ngx_atomic_fetch_جوڑو(ngx_connection_counter, 1);

    c->شروع_ویلا = ngx_current_msec;

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->لوگارتھم, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->نمبر);

    rc = connect(s, rec->sockaddr, rec->socklen);

    إذا (rc == -1) {
        ngx_log_error(NGX_LOG_CRIT, &rec->لوگارتھم, ngx_socket_errno,
                      "connect() failed");

        اذهب failed;
    }

    /* UDP sockets are always ready to write */
    wev->ready = 1;

    event = (ngx_event_flags & NGX_USE_CLEAR_EVENT) ?
                /* kqueue, epoll */                 NGX_CLEAR_EVENT:
                /* select, poll, /dev/poll */       NGX_LEVEL_EVENT;
                /* eventport event type has no meaning: oneshot only */

    إذا (ngx_جوڑو_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
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

    s = ngx_socket(rec->sockaddr->sa_family, SOCK_دھارا, 0);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->لوگارتھم, 0, "TCP socket %d", s);

    إذا (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->لوگارتھم, ngx_socket_errno,
                      ngx_socket_n " failed");
        أرجع NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->لوگارتھم);

    إذا (c == عدم) {
        إذا (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->لوگارتھم, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        أرجع NGX_ERROR;
    }

    إذا (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->لوگارتھم, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        اذهب failed;
    }

    rev = c->اقرأ;
    wev = c->اكتب;

    rev->لوگارتھم = &rec->لوگارتھم;
    wev->لوگارتھم = &rec->لوگارتھم;

    rec->tcp = c;

    c->نمبر = ngx_atomic_fetch_جوڑو(ngx_connection_counter, 1);

    c->شروع_ویلا = ngx_current_msec;

    إذا (ngx_جوڑو_conn) {
        إذا (ngx_جوڑو_conn(c) == NGX_ERROR) {
            اذهب failed;
        }
    }

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->لوگارتھم, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->نمبر);

    rc = connect(s, rec->sockaddr, rec->socklen);

    إذا (rc == -1) {
        err = ngx_socket_errno;


        إذا (err != NGX_EINPROGRESS
#إذا (NGX_WIN32)
            /* Winsock returns WSAEWOULDBLOCK (NGX_EAGAIN) */
            && err != NGX_EAGAIN
#جے_انت
            )
        {
            إذا (err == NGX_ECONNREFUSED
#إذا (NGX_LINUX)
                /*
                 * Linux returns EAGAIN instead of ECONNREFUSED
                 * for unix sockets if listen queue is full
                 */
                || err == NGX_EAGAIN
#جے_انت
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

            ngx_log_error(level, &rec->لوگارتھم, err, "connect() to %V failed",
                          &rec->server);

            ngx_close_connection(c);
            rec->tcp = عدم;

            أرجع NGX_ERROR;
        }
    }

    إذا (ngx_جوڑو_conn) {
        إذا (rc == -1) {

            /* NGX_EINPROGRESS */

            أرجع NGX_AGAIN;
        }

        ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->لوگارتھم, 0, "connected");

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

    إذا (ngx_جوڑو_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        اذهب failed;
    }

    إذا (rc == -1) {

        /* NGX_EINPROGRESS */

        إذا (ngx_جوڑو_event(wev, NGX_WRITE_EVENT, event) != NGX_OK) {
            اذهب failed;
        }

        أرجع NGX_AGAIN;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->لوگارتھم, 0, "connected");

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
    ngx_resolver_srv_t  *پہلا, *دوجا;

    پہلا = (ngx_resolver_srv_t *) one;
    دوجا = (ngx_resolver_srv_t *) two;

    p1 = پہلا->ترجیح;
    p2 = دوجا->ترجیح;

    أرجع p1 - p2;
}
