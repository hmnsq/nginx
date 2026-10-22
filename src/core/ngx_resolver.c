
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>
#అన్తర్భావ <ngx_event.h>


#నిర్వచించు NGX_RESOLVER_UDP_కొలత   4096

#నిర్వచించు NGX_RESOLVER_TCP_RSIZE  (2 + 65535)
#నిర్వచించు NGX_RESOLVER_TCP_WSIZE  8192


రకనిర్వచనం నిర్మాణం {
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


రకనిర్వచనం నిర్మాణం {
    u_char  type_hi;
    u_char  type_lo;
    u_char  class_hi;
    u_char  class_lo;
} ngx_resolver_qs_t;


రకనిర్వచనం నిర్మాణం {
    u_char  type_hi;
    u_char  type_lo;
    u_char  class_hi;
    u_char  class_lo;
    u_char  ttl[4];
    u_char  len_hi;
    u_char  len_lo;
} ngx_resolver_an_t;


#నిర్వచించు ngx_resolver_కణుపు(n)  ngx_rbtree_దత్తాంశం(n, ngx_resolver_కణుపు_t, కణుపు)


స్థిర ngx_int_t ngx_udp_connect(ngx_resolver_connection_t *rec);
స్థిర ngx_int_t ngx_tcp_connect(ngx_resolver_connection_t *rec);


స్థిర శూన్య ngx_resolver_cleanup(శూన్య *దత్తాంశం);
స్థిర శూన్య ngx_resolver_cleanup_వృక్షం(ngx_resolver_t *r, ngx_rbtree_t *వృక్షం);
స్థిర ngx_int_t ngx_resolve_name_locked(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx, ngx_str_t *name);
స్థిర శూన్య ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *వృక్షం,
    ngx_వరుస_t *వరుస);
స్థిర ngx_int_t ngx_resolver_send_query(ngx_resolver_t *r,
    ngx_resolver_కణుపు_t *rn);
స్థిర ngx_int_t ngx_resolver_send_udp_query(ngx_resolver_t *r,
    ngx_resolver_connection_t *rec, u_char *query, u_short qlen);
స్థిర ngx_int_t ngx_resolver_send_tcp_query(ngx_resolver_t *r,
    ngx_resolver_connection_t *rec, u_char *query, u_short qlen);
స్థిర ngx_int_t ngx_resolver_create_name_query(ngx_resolver_t *r,
    ngx_resolver_కణుపు_t *rn, ngx_str_t *name);
స్థిర ngx_int_t ngx_resolver_create_srv_query(ngx_resolver_t *r,
    ngx_resolver_కణుపు_t *rn, ngx_str_t *name);
స్థిర ngx_int_t ngx_resolver_create_addr_query(ngx_resolver_t *r,
    ngx_resolver_కణుపు_t *rn, ngx_resolver_addr_t *addr);
స్థిర శూన్య ngx_resolver_resend_handler(ngx_event_t *ev);
స్థిర సమయం_t ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *వృక్షం,
    ngx_వరుస_t *వరుస);
స్థిర ngx_uint_t ngx_resolver_resend_ఖాళీలు(ngx_resolver_t *r);
స్థిర శూన్య ngx_resolver_udp_read(ngx_event_t *rev);
స్థిర శూన్య ngx_resolver_tcp_write(ngx_event_t *wev);
స్థిర శూన్య ngx_resolver_tcp_read(ngx_event_t *rev);
స్థిర శూన్య ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf,
    కొలత_t n, ngx_uint_t tcp);
స్థిర శూన్య ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, కొలత_t n,
    ngx_uint_t ident, ngx_uint_t సంకేతం_2, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans);
స్థిర శూన్య ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, కొలత_t n,
    ngx_uint_t ident, ngx_uint_t సంకేతం_2, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans);
స్థిర శూన్య ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, కొలత_t n,
    ngx_uint_t ident, ngx_uint_t సంకేతం_2, ngx_uint_t nan);
స్థిర ngx_resolver_కణుపు_t *ngx_resolver_lookup_name(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
స్థిర ngx_resolver_కణుపు_t *ngx_resolver_lookup_srv(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
స్థిర ngx_resolver_కణుపు_t *ngx_resolver_lookup_addr(ngx_resolver_t *r,
    లోపల_addr_t addr);
స్థిర శూన్య ngx_resolver_rbtree_చొప్పించు_విలువ(ngx_rbtree_కణుపు_t *తాత్కాలిక,
    ngx_rbtree_కణుపు_t *కణుపు, ngx_rbtree_కణుపు_t *sentinel);
స్థిర ngx_int_t ngx_resolver_copy(ngx_resolver_t *r, ngx_str_t *name,
    u_char *buf, u_char *src, u_char *చివరి);
స్థిర ngx_int_t ngx_resolver_సమితి_timeout(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx);
స్థిర శూన్య ngx_resolver_timeout_handler(ngx_event_t *ev);
స్థిర శూన్య ngx_resolver_free_కణుపు(ngx_resolver_t *r, ngx_resolver_కణుపు_t *rn);
స్థిర శూన్య *ngx_resolver_alloc(ngx_resolver_t *r, కొలత_t కొలత);
స్థిర శూన్య *ngx_resolver_calloc(ngx_resolver_t *r, కొలత_t కొలత);
స్థిర శూన్య ngx_resolver_free(ngx_resolver_t *r, శూన్య *p);
స్థిర శూన్య ngx_resolver_free_locked(ngx_resolver_t *r, శూన్య *p);
స్థిర శూన్య *ngx_resolver_dup(ngx_resolver_t *r, శూన్య *src, కొలత_t కొలత);
స్థిర ngx_resolver_addr_t *ngx_resolver_export(ngx_resolver_t *r,
    ngx_resolver_కణుపు_t *rn, ngx_uint_t rotate);
స్థిర శూన్య ngx_resolver_report_srv(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx);
స్థిర u_char *ngx_resolver_log_error(ngx_log_t *సంవర్గమానం, u_char *buf, కొలత_t len);
స్థిర శూన్య ngx_resolver_resolve_srv_పేర్లు(ngx_resolver_ctx_t *ctx,
    ngx_resolver_కణుపు_t *rn);
స్థిర శూన్య ngx_resolver_srv_పేర్లు_handler(ngx_resolver_ctx_t *ctx);
స్థిర ngx_int_t ngx_resolver_cmp_srvs(స్థిరరాశి శూన్య *one, స్థిరరాశి శూన్య *two);

#ఒకవేళ (NGX_HAVE_INET6)
స్థిర శూన్య ngx_resolver_rbtree_చొప్పించు_addr6_విలువ(ngx_rbtree_కణుపు_t *తాత్కాలిక,
    ngx_rbtree_కణుపు_t *కణుపు, ngx_rbtree_కణుపు_t *sentinel);
స్థిర ngx_resolver_కణుపు_t *ngx_resolver_lookup_addr6(ngx_resolver_t *r,
    నిర్మాణం in6_addr *addr, uint32_t hash);
#అయితే_ముగింపు


ngx_resolver_t *
ngx_resolver_create(ngx_conf_t *cf, ngx_str_t *పేర్లు, ngx_uint_t n)
{
    ngx_str_t                   s;
    ngx_url_t                   u;
    ngx_uint_t                  i, j;
    ngx_resolver_t             *r;
    ngx_pool_cleanup_t         *cln;
    ngx_resolver_connection_t  *rec;

    r = ngx_pcalloc(cf->pool, పరిమాణం(ngx_resolver_t));
    ఒకవేళ (r == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    r->event = ngx_pcalloc(cf->pool, పరిమాణం(ngx_event_t));
    ఒకవేళ (r->event == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    cln = ngx_pool_cleanup_కూడు(cf->pool, 0);
    ఒకవేళ (cln == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    cln->handler = ngx_resolver_cleanup;
    cln->దత్తాంశం = r;

    r->ipv4 = 1;

    ngx_rbtree_init(&r->name_rbtree, &r->name_sentinel,
                    ngx_resolver_rbtree_చొప్పించు_విలువ);

    ngx_rbtree_init(&r->srv_rbtree, &r->srv_sentinel,
                    ngx_resolver_rbtree_చొప్పించు_విలువ);

    ngx_rbtree_init(&r->addr_rbtree, &r->addr_sentinel,
                    ngx_rbtree_చొప్పించు_విలువ);

    ngx_వరుస_init(&r->name_resend_వరుస);
    ngx_వరుస_init(&r->srv_resend_వరుస);
    ngx_వరుస_init(&r->addr_resend_వరుస);

    ngx_వరుస_init(&r->name_expire_వరుస);
    ngx_వరుస_init(&r->srv_expire_వరుస);
    ngx_వరుస_init(&r->addr_expire_వరుస);

#ఒకవేళ (NGX_HAVE_INET6)
    r->ipv6 = 1;

    ngx_rbtree_init(&r->addr6_rbtree, &r->addr6_sentinel,
                    ngx_resolver_rbtree_చొప్పించు_addr6_విలువ);

    ngx_వరుస_init(&r->addr6_resend_వరుస);

    ngx_వరుస_init(&r->addr6_expire_వరుస);
#అయితే_ముగింపు

    r->event->handler = ngx_resolver_resend_handler;
    r->event->దత్తాంశం = r;
    r->event->సంవర్గమానం = &cf->చక్రం->new_log;
    r->event->cancelable = 1;
    r->ident = -1;

    r->resend_timeout = 5;
    r->tcp_timeout = 5;
    r->expire = 30;
    r->valid = 0;

    r->సంవర్గమానం = &cf->చక్రం->new_log;
    r->log_level = NGX_LOG_ERR;

    ఒకవేళ (n) {
        ఒకవేళ (ngx_array_init(&r->connections, cf->pool, n,
                           పరిమాణం(ngx_resolver_connection_t))
            != NGX_OK)
        {
            తిరిగి ఖాళీ;
        }
    }

    కొరకు (i = 0; i < n; i++) {
        ఒకవేళ (ngx_strncmp(పేర్లు[i].దత్తాంశం, "valid=", 6) == 0) {
            s.len = పేర్లు[i].len - 6;
            s.దత్తాంశం = పేర్లు[i].దత్తాంశం + 6;

            r->valid = ngx_parse_సమయం(&s, 1);

            ఒకవేళ (r->valid == (సమయం_t) NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &పేర్లు[i]);
                తిరిగి ఖాళీ;
            }

            కొనసాగు;
        }

#ఒకవేళ (NGX_HAVE_INET6)
        ఒకవేళ (ngx_strncmp(పేర్లు[i].దత్తాంశం, "ipv4=", 5) == 0) {

            ఒకవేళ (ngx_strcmp(&పేర్లు[i].దత్తాంశం[5], "on") == 0) {
                r->ipv4 = 1;

            } లేకపోతే ఒకవేళ (ngx_strcmp(&పేర్లు[i].దత్తాంశం[5], "off") == 0) {
                r->ipv4 = 0;

            } లేకపోతే {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &పేర్లు[i]);
                తిరిగి ఖాళీ;
            }

            కొనసాగు;
        }

        ఒకవేళ (ngx_strncmp(పేర్లు[i].దత్తాంశం, "ipv6=", 5) == 0) {

            ఒకవేళ (ngx_strcmp(&పేర్లు[i].దత్తాంశం[5], "on") == 0) {
                r->ipv6 = 1;

            } లేకపోతే ఒకవేళ (ngx_strcmp(&పేర్లు[i].దత్తాంశం[5], "off") == 0) {
                r->ipv6 = 0;

            } లేకపోతే {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &పేర్లు[i]);
                తిరిగి ఖాళీ;
            }

            కొనసాగు;
        }
#అయితే_ముగింపు

        ngx_memzero(&u, పరిమాణం(ngx_url_t));

        u.url = పేర్లు[i];
        u.default_పోర్టు = 53;

        ఒకవేళ (ngx_parse_url(cf->pool, &u) != NGX_OK) {
            ఒకవేళ (u.err) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "%s in resolver \"%V\"",
                                   u.err, &u.url);
            }

            తిరిగి ఖాళీ;
        }

        rec = ngx_array_నెట్టు_n(&r->connections, u.naddrs);
        ఒకవేళ (rec == ఖాళీ) {
            తిరిగి ఖాళీ;
        }

        ngx_memzero(rec, u.naddrs * పరిమాణం(ngx_resolver_connection_t));

        కొరకు (j = 0; j < u.naddrs; j++) {
            rec[j].sockaddr = u.addrs[j].sockaddr;
            rec[j].socklen = u.addrs[j].socklen;
            rec[j].server = u.addrs[j].name;
            rec[j].resolver = r;
        }
    }

#ఒకవేళ (NGX_HAVE_INET6)
    ఒకవేళ (r->ipv4 + r->ipv6 == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"ipv4\" and \"ipv6\" cannot both be \"off\"");
        తిరిగి ఖాళీ;
    }
#అయితే_ముగింపు

    ఒకవేళ (n && r->connections.nelts == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "no name servers defined");
        తిరిగి ఖాళీ;
    }

    తిరిగి r;
}


స్థిర శూన్య
ngx_resolver_cleanup(శూన్య *దత్తాంశం)
{
    ngx_resolver_t  *r = దత్తాంశం;

    ngx_uint_t                  i;
    ngx_resolver_connection_t  *rec;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_చక్రం->సంవర్గమానం, 0, "cleanup resolver");

    ngx_resolver_cleanup_వృక్షం(r, &r->name_rbtree);

    ngx_resolver_cleanup_వృక్షం(r, &r->srv_rbtree);

    ngx_resolver_cleanup_వృక్షం(r, &r->addr_rbtree);

#ఒకవేళ (NGX_HAVE_INET6)
    ngx_resolver_cleanup_వృక్షం(r, &r->addr6_rbtree);
#అయితే_ముగింపు

    ఒకవేళ (r->event->timer_సమితి) {
        ngx_del_timer(r->event);
    }

    rec = r->connections.elts;

    కొరకు (i = 0; i < r->connections.nelts; i++) {
        ఒకవేళ (rec[i].udp) {
            ngx_close_connection(rec[i].udp);
        }

        ఒకవేళ (rec[i].tcp) {
            ngx_close_connection(rec[i].tcp);
        }

        ఒకవేళ (rec[i].read_buf) {
            ngx_resolver_free(r, rec[i].read_buf->ప్రారంభం);
            ngx_resolver_free(r, rec[i].read_buf);
        }

        ఒకవేళ (rec[i].write_buf) {
            ngx_resolver_free(r, rec[i].write_buf->ప్రారంభం);
            ngx_resolver_free(r, rec[i].write_buf);
        }
    }
}


స్థిర శూన్య
ngx_resolver_cleanup_వృక్షం(ngx_resolver_t *r, ngx_rbtree_t *వృక్షం)
{
    ngx_resolver_ctx_t   *ctx, *తదుపరి;
    ngx_resolver_కణుపు_t  *rn;

    అయితే (వృక్షం->మూలం != వృక్షం->sentinel) {

        rn = ngx_resolver_కణుపు(ngx_rbtree_min(వృక్షం->మూలం, వృక్షం->sentinel));

        ngx_వరుస_remove(&rn->వరుస);

        కొరకు (ctx = rn->నిరీక్షణ; ctx; ctx = తదుపరి) {
            తదుపరి = ctx->తదుపరి;

            ఒకవేళ (ctx->event) {
                ఒకవేళ (ctx->event->timer_సమితి) {
                    ngx_del_timer(ctx->event);
                }

                ngx_resolver_free(r, ctx->event);
            }

            ngx_resolver_free(r, ctx);
        }

        ngx_rbtree_delete(వృక్షం, &rn->కణుపు);

        ngx_resolver_free_కణుపు(r, rn);
    }
}


ngx_resolver_ctx_t *
ngx_resolve_ప్రారంభం(ngx_resolver_t *r, ngx_resolver_ctx_t *తాత్కాలిక)
{
    లోపల_addr_t            addr;
    ngx_resolver_ctx_t  *ctx;

    ఒకవేళ (తాత్కాలిక) {
        addr = ngx_inet_addr(తాత్కాలిక->name.దత్తాంశం, తాత్కాలిక->name.len);

        ఒకవేళ (addr != INADDR_NONE) {
            తాత్కాలిక->resolver = r;
            తాత్కాలిక->state = NGX_OK;
            తాత్కాలిక->naddrs = 1;
            తాత్కాలిక->addrs = &తాత్కాలిక->addr;
            తాత్కాలిక->addr.sockaddr = (నిర్మాణం sockaddr *) &తాత్కాలిక->జ్యా;
            తాత్కాలిక->addr.socklen = పరిమాణం(నిర్మాణం sockaddr_లోపల);
            ngx_memzero(&తాత్కాలిక->జ్యా, పరిమాణం(నిర్మాణం sockaddr_లోపల));
            తాత్కాలిక->జ్యా.sin_family = AF_INET;
            తాత్కాలిక->జ్యా.sin_addr.s_addr = addr;
            తాత్కాలిక->శీఘ్ర = 1;

            తిరిగి తాత్కాలిక;
        }
    }

    ఒకవేళ (r->connections.nelts == 0) {
        తిరిగి NGX_NO_RESOLVER;
    }

    ctx = ngx_resolver_calloc(r, పరిమాణం(ngx_resolver_ctx_t));

    ఒకవేళ (ctx) {
        ctx->resolver = r;
    }

    తిరిగి ctx;
}


ngx_int_t
ngx_resolve_name(ngx_resolver_ctx_t *ctx)
{
    కొలత_t           slen;
    ngx_int_t        rc;
    ngx_str_t        name;
    ngx_resolver_t  *r;

    r = ctx->resolver;

    ఒకవేళ (ctx->name.len > 0 && ctx->name.దత్తాంశం[ctx->name.len - 1] == '.') {
        ctx->name.len--;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->సంవర్గమానం, 0,
                   "resolve: \"%V\"", &ctx->name);

    ఒకవేళ (ctx->శీఘ్ర) {
        ctx->handler(ctx);
        తిరిగి NGX_OK;
    }

    ఒకవేళ (ctx->service.len) {
        slen = ctx->service.len;

        ఒకవేళ (ngx_strlchr(ctx->service.దత్తాంశం,
                        ctx->service.దత్తాంశం + ctx->service.len, '.')
            == ఖాళీ)
        {
            slen += పరిమాణం("_._tcp") - 1;
        }

        name.len = slen + 1 + ctx->name.len;

        name.దత్తాంశం = ngx_resolver_alloc(r, name.len);
        ఒకవేళ (name.దత్తాంశం == ఖాళీ) {
            వెళ్ళు failed;
        }

        ఒకవేళ (slen == ctx->service.len) {
            ngx_sprintf(name.దత్తాంశం, "%V.%V", &ctx->service, &ctx->name);

        } లేకపోతే {
            ngx_sprintf(name.దత్తాంశం, "_%V._tcp.%V", &ctx->service, &ctx->name);
        }

        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &name);

        ngx_resolver_free(r, name.దత్తాంశం);

    } లేకపోతే {
        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &ctx->name);
    }

    ఒకవేళ (rc == NGX_OK) {
        తిరిగి NGX_OK;
    }

    /* unlock name mutex */

    ఒకవేళ (rc == NGX_AGAIN) {
        తిరిగి NGX_OK;
    }

    /* NGX_ERROR */

    ఒకవేళ (ctx->event) {
        ngx_resolver_free(r, ctx->event);
    }

failed:

    ngx_resolver_free(r, ctx);

    తిరిగి NGX_ERROR;
}


శూన్య
ngx_resolve_name_పూర్తి(ngx_resolver_ctx_t *ctx)
{
    ngx_uint_t            i;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_కణుపు_t  *rn;

    r = ctx->resolver;

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->సంవర్గమానం, 0,
                   "resolve name done: %i", ctx->state);

    ఒకవేళ (ctx->శీఘ్ర) {
        తిరిగి;
    }

    ఒకవేళ (ctx->event && ctx->event->timer_సమితి) {
        ngx_del_timer(ctx->event);
    }

    /* lock name mutex */

    ఒకవేళ (ctx->nsrvs) {
        కొరకు (i = 0; i < ctx->nsrvs; i++) {
            ఒకవేళ (ctx->srvs[i].ctx) {
                ngx_resolve_name_పూర్తి(ctx->srvs[i].ctx);
            }

            ఒకవేళ (ctx->srvs[i].addrs) {
                ngx_resolver_free(r, ctx->srvs[i].addrs->sockaddr);
                ngx_resolver_free(r, ctx->srvs[i].addrs);
            }

            ngx_resolver_free(r, ctx->srvs[i].name.దత్తాంశం);
        }

        ngx_resolver_free(r, ctx->srvs);
    }

    ఒకవేళ (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->కణుపు;

        ఒకవేళ (rn) {
            p = &rn->నిరీక్షణ;
            w = rn->నిరీక్షణ;

            అయితే (w) {
                ఒకవేళ (w == ctx) {
                    *p = w->తదుపరి;

                    వెళ్ళు పూర్తి;
                }

                p = &w->తదుపరి;
                w = w->తదుపరి;
            }

            ngx_log_error(NGX_LOG_ALERT, r->సంవర్గమానం, 0,
                          "could not cancel %V resolving", &ctx->name);
        }
    }

పూర్తి:

    ఒకవేళ (ctx->service.len) {
        ngx_resolver_expire(r, &r->srv_rbtree, &r->srv_expire_వరుస);

    } లేకపోతే {
        ngx_resolver_expire(r, &r->name_rbtree, &r->name_expire_వరుస);
    }

    /* unlock name mutex */

    /* lock alloc mutex */

    ఒకవేళ (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    ఒకవేళ (r->event->timer_సమితి && ngx_resolver_resend_ఖాళీలు(r)) {
        ngx_del_timer(r->event);
    }
}


స్థిర ngx_int_t
ngx_resolve_name_locked(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx,
    ngx_str_t *name)
{
    uint32_t              hash;
    ngx_int_t             rc;
    ngx_str_t             cname;
    ngx_uint_t            i, naddrs;
    ngx_వరుస_t          *resend_వరుస, *expire_వరుస;
    ngx_rbtree_t         *వృక్షం;
    ngx_resolver_ctx_t   *తదుపరి, *చివరి;
    ngx_resolver_addr_t  *addrs;
    ngx_resolver_కణుపు_t  *rn;

    ngx_strlow(name->దత్తాంశం, name->దత్తాంశం, name->len);

    hash = ngx_చక్రీయ_తనిఖీ_32_short(name->దత్తాంశం, name->len);

    ఒకవేళ (ctx->service.len) {
        rn = ngx_resolver_lookup_srv(r, name, hash);

        వృక్షం = &r->srv_rbtree;
        resend_వరుస = &r->srv_resend_వరుస;
        expire_వరుస = &r->srv_expire_వరుస;

    } లేకపోతే {
        rn = ngx_resolver_lookup_name(r, name, hash);

        వృక్షం = &r->name_rbtree;
        resend_వరుస = &r->name_resend_వరుస;
        expire_వరుస = &r->name_expire_వరుస;
    }

    ఒకవేళ (rn) {

        /* ctx can be a list after NGX_RESOLVE_CNAME */
        కొరకు (చివరి = ctx; చివరి->తదుపరి; చివరి = చివరి->తదుపరి);

        ఒకవేళ (rn->valid >= ngx_సమయం()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->సంవర్గమానం, 0, "resolve cached");

            ngx_వరుస_remove(&rn->వరుస);

            rn->expire = ngx_సమయం() + r->expire;

            ngx_వరుస_చొప్పించు_తల(expire_వరుస, &rn->వరుస);

            naddrs = (rn->naddrs == (u_short) -1) ? 0 : rn->naddrs;
#ఒకవేళ (NGX_HAVE_INET6)
            naddrs += (rn->naddrs6 == (u_short) -1) ? 0 : rn->naddrs6;
#అయితే_ముగింపు

            ఒకవేళ (naddrs) {

                ఒకవేళ (naddrs == 1 && rn->naddrs == 1) {
                    addrs = ఖాళీ;

                } లేకపోతే {
                    addrs = ngx_resolver_export(r, rn, 1);
                    ఒకవేళ (addrs == ఖాళీ) {
                        తిరిగి NGX_ERROR;
                    }
                }

                చివరి->తదుపరి = rn->నిరీక్షణ;
                rn->నిరీక్షణ = ఖాళీ;

                /* unlock name mutex */

                చేయి {
                    ctx->state = NGX_OK;
                    ctx->valid = rn->valid;
                    ctx->naddrs = naddrs;

                    ఒకవేళ (addrs == ఖాళీ) {
                        ctx->addrs = &ctx->addr;
                        ctx->addr.sockaddr = (నిర్మాణం sockaddr *) &ctx->జ్యా;
                        ctx->addr.socklen = పరిమాణం(నిర్మాణం sockaddr_లోపల);
                        ngx_memzero(&ctx->జ్యా, పరిమాణం(నిర్మాణం sockaddr_లోపల));
                        ctx->జ్యా.sin_family = AF_INET;
                        ctx->జ్యా.sin_addr.s_addr = rn->u.addr;

                    } లేకపోతే {
                        ctx->addrs = addrs;
                    }

                    తదుపరి = ctx->తదుపరి;

                    ctx->handler(ctx);

                    ctx = తదుపరి;
                } అయితే (ctx);

                ఒకవేళ (addrs != ఖాళీ) {
                    ngx_resolver_free(r, addrs->sockaddr);
                    ngx_resolver_free(r, addrs);
                }

                తిరిగి NGX_OK;
            }

            ఒకవేళ (rn->nsrvs) {
                చివరి->తదుపరి = rn->నిరీక్షణ;
                rn->నిరీక్షణ = ఖాళీ;

                /* unlock name mutex */

                చేయి {
                    తదుపరి = ctx->తదుపరి;

                    ngx_resolver_resolve_srv_పేర్లు(ctx, rn);

                    ctx = తదుపరి;
                } అయితే (ctx);

                తిరిగి NGX_OK;
            }

            /* NGX_RESOLVE_CNAME */

            ఒకవేళ (ctx->recursion++ < NGX_RESOLVER_MAX_RECURSION) {

                cname.len = rn->cnlen;
                cname.దత్తాంశం = rn->u.cname;

                తిరిగి ngx_resolve_name_locked(r, ctx, &cname);
            }

            చివరి->తదుపరి = rn->నిరీక్షణ;
            rn->నిరీక్షణ = ఖాళీ;

            /* unlock name mutex */

            చేయి {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                ctx->valid = ngx_సమయం() + (r->valid ? r->valid : 10);
                తదుపరి = ctx->తదుపరి;

                ctx->handler(ctx);

                ctx = తదుపరి;
            } అయితే (ctx);

            తిరిగి NGX_OK;
        }

        ఒకవేళ (rn->నిరీక్షణ) {
            ఒకవేళ (ngx_resolver_సమితి_timeout(r, ctx) != NGX_OK) {
                తిరిగి NGX_ERROR;
            }

            చివరి->తదుపరి = rn->నిరీక్షణ;
            rn->నిరీక్షణ = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;

            చేయి {
                ctx->కణుపు = rn;
                ctx = ctx->తదుపరి;
            } అయితే (ctx);

            తిరిగి NGX_AGAIN;
        }

        ngx_వరుస_remove(&rn->వరుస);

        /* lock alloc mutex */

        ఒకవేళ (rn->query) {
            ngx_resolver_free_locked(r, rn->query);
            rn->query = ఖాళీ;
#ఒకవేళ (NGX_HAVE_INET6)
            rn->query6 = ఖాళీ;
#అయితే_ముగింపు
        }

        ఒకవేళ (rn->cnlen) {
            ngx_resolver_free_locked(r, rn->u.cname);
        }

        ఒకవేళ (rn->naddrs > 1 && rn->naddrs != (u_short) -1) {
            ngx_resolver_free_locked(r, rn->u.addrs);
        }

#ఒకవేళ (NGX_HAVE_INET6)
        ఒకవేళ (rn->naddrs6 > 1 && rn->naddrs6 != (u_short) -1) {
            ngx_resolver_free_locked(r, rn->u6.addrs6);
        }
#అయితే_ముగింపు

        ఒకవేళ (rn->nsrvs) {
            కొరకు (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
                ఒకవేళ (rn->u.srvs[i].name.దత్తాంశం) {
                    ngx_resolver_free_locked(r, rn->u.srvs[i].name.దత్తాంశం);
                }
            }

            ngx_resolver_free_locked(r, rn->u.srvs);
        }

        /* unlock alloc mutex */

    } లేకపోతే {

        rn = ngx_resolver_alloc(r, పరిమాణం(ngx_resolver_కణుపు_t));
        ఒకవేళ (rn == ఖాళీ) {
            తిరిగి NGX_ERROR;
        }

        rn->name = ngx_resolver_dup(r, name->దత్తాంశం, name->len);
        ఒకవేళ (rn->name == ఖాళీ) {
            ngx_resolver_free(r, rn);
            తిరిగి NGX_ERROR;
        }

        rn->కణుపు.కీలకం = hash;
        rn->nlen = (u_short) name->len;
        rn->query = ఖాళీ;
#ఒకవేళ (NGX_HAVE_INET6)
        rn->query6 = ఖాళీ;
#అయితే_ముగింపు

        ngx_rbtree_చొప్పించు(వృక్షం, &rn->కణుపు);
    }

    ఒకవేళ (ctx->service.len) {
        rc = ngx_resolver_create_srv_query(r, rn, name);

    } లేకపోతే {
        rc = ngx_resolver_create_name_query(r, rn, name);
    }

    ఒకవేళ (rc == NGX_ERROR) {
        వెళ్ళు failed;
    }

    ఒకవేళ (rc == NGX_DECLINED) {
        ngx_rbtree_delete(వృక్షం, &rn->కణుపు);

        ngx_resolver_free(r, rn->query);
        ngx_resolver_free(r, rn->name);
        ngx_resolver_free(r, rn);

        చేయి {
            ctx->state = NGX_RESOLVE_NXDOMAIN;
            తదుపరి = ctx->తదుపరి;

            ctx->handler(ctx);

            ctx = తదుపరి;
        } అయితే (ctx);

        తిరిగి NGX_OK;
    }

    rn->చివరి_connection = r->చివరి_connection++;
    ఒకవేళ (r->చివరి_connection == r->connections.nelts) {
        r->చివరి_connection = 0;
    }

    rn->naddrs = r->ipv4 ? (u_short) -1 : 0;
    rn->tcp = 0;
#ఒకవేళ (NGX_HAVE_INET6)
    rn->naddrs6 = r->ipv6 ? (u_short) -1 : 0;
    rn->tcp6 = 0;
#అయితే_ముగింపు
    rn->nsrvs = 0;

    ఒకవేళ (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->చివరి_connection++;
        ఒకవేళ (rn->చివరి_connection == r->connections.nelts) {
            rn->చివరి_connection = 0;
        }

        (శూన్య) ngx_resolver_send_query(r, rn);
    }

    ఒకవేళ (ngx_resolver_సమితి_timeout(r, ctx) != NGX_OK) {
        వెళ్ళు failed;
    }

    ఒకవేళ (ngx_resolver_resend_ఖాళీలు(r)) {
        ngx_కూడు_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_సమయం() + r->resend_timeout;

    ngx_వరుస_చొప్పించు_తల(resend_వరుస, &rn->వరుస);

    rn->సంకేతం_2 = 0;
    rn->cnlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_విలువ;
    rn->నిరీక్షణ = ctx;

    ctx->state = NGX_AGAIN;
    ctx->async = 1;

    చేయి {
        ctx->కణుపు = rn;
        ctx = ctx->తదుపరి;
    } అయితే (ctx);

    తిరిగి NGX_AGAIN;

failed:

    ngx_rbtree_delete(వృక్షం, &rn->కణుపు);

    ఒకవేళ (rn->query) {
        ngx_resolver_free(r, rn->query);
    }

    ngx_resolver_free(r, rn->name);

    ngx_resolver_free(r, rn);

    తిరిగి NGX_ERROR;
}


ngx_int_t
ngx_resolve_addr(ngx_resolver_ctx_t *ctx)
{
    u_char               *name;
    లోపల_addr_t             addr;
    ngx_వరుస_t          *resend_వరుస, *expire_వరుస;
    ngx_rbtree_t         *వృక్షం;
    ngx_resolver_t       *r;
    నిర్మాణం sockaddr_లోపల   *జ్యా;
    ngx_resolver_కణుపు_t  *rn;
#ఒకవేళ (NGX_HAVE_INET6)
    uint32_t              hash;
    నిర్మాణం sockaddr_in6  *sin6;
#అయితే_ముగింపు

#ఒకవేళ (NGX_SUPPRESS_WARN)
    addr = 0;
#ఒకవేళ (NGX_HAVE_INET6)
    hash = 0;
    sin6 = ఖాళీ;
#అయితే_ముగింపు
#అయితే_ముగింపు

    r = ctx->resolver;

    ఎంపిక (ctx->addr.sockaddr->sa_family) {

#ఒకవేళ (NGX_HAVE_INET6)
    స్థితి AF_INET6:
        sin6 = (నిర్మాణం sockaddr_in6 *) ctx->addr.sockaddr;
        hash = ngx_చక్రీయ_తనిఖీ_32_short(sin6->sin6_addr.s6_addr, 16);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr6(r, &sin6->sin6_addr, hash);

        వృక్షం = &r->addr6_rbtree;
        resend_వరుస = &r->addr6_resend_వరుస;
        expire_వరుస = &r->addr6_expire_వరుస;

        ఆపు;
#అయితే_ముగింపు

    అప్రమేయ: /* AF_INET */
        జ్యా = (నిర్మాణం sockaddr_లోపల *) ctx->addr.sockaddr;
        addr = ntohl(జ్యా->sin_addr.s_addr);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        వృక్షం = &r->addr_rbtree;
        resend_వరుస = &r->addr_resend_వరుస;
        expire_వరుస = &r->addr_expire_వరుస;
    }

    ఒకవేళ (rn) {

        ఒకవేళ (rn->valid >= ngx_సమయం()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->సంవర్గమానం, 0, "resolve cached");

            ngx_వరుస_remove(&rn->వరుస);

            rn->expire = ngx_సమయం() + r->expire;

            ngx_వరుస_చొప్పించు_తల(expire_వరుస, &rn->వరుస);

            name = ngx_resolver_dup(r, rn->name, rn->nlen);
            ఒకవేళ (name == ఖాళీ) {
                ngx_resolver_free(r, ctx);
                తిరిగి NGX_ERROR;
            }

            ctx->name.len = rn->nlen;
            ctx->name.దత్తాంశం = name;

            /* unlock addr mutex */

            ctx->state = NGX_OK;
            ctx->valid = rn->valid;

            ctx->handler(ctx);

            ngx_resolver_free(r, name);

            తిరిగి NGX_OK;
        }

        ఒకవేళ (rn->నిరీక్షణ) {
            ఒకవేళ (ngx_resolver_సమితి_timeout(r, ctx) != NGX_OK) {
                తిరిగి NGX_ERROR;
            }

            ctx->తదుపరి = rn->నిరీక్షణ;
            rn->నిరీక్షణ = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;
            ctx->కణుపు = rn;

            /* unlock addr mutex */

            తిరిగి NGX_OK;
        }

        ngx_వరుస_remove(&rn->వరుస);

        ngx_resolver_free(r, rn->query);
        rn->query = ఖాళీ;
#ఒకవేళ (NGX_HAVE_INET6)
        rn->query6 = ఖాళీ;
#అయితే_ముగింపు

    } లేకపోతే {
        rn = ngx_resolver_alloc(r, పరిమాణం(ngx_resolver_కణుపు_t));
        ఒకవేళ (rn == ఖాళీ) {
            వెళ్ళు failed;
        }

        ఎంపిక (ctx->addr.sockaddr->sa_family) {

#ఒకవేళ (NGX_HAVE_INET6)
        స్థితి AF_INET6:
            rn->addr6 = sin6->sin6_addr;
            rn->కణుపు.కీలకం = hash;
            ఆపు;
#అయితే_ముగింపు

        అప్రమేయ: /* AF_INET */
            rn->కణుపు.కీలకం = addr;
        }

        rn->query = ఖాళీ;
#ఒకవేళ (NGX_HAVE_INET6)
        rn->query6 = ఖాళీ;
#అయితే_ముగింపు

        ngx_rbtree_చొప్పించు(వృక్షం, &rn->కణుపు);
    }

    ఒకవేళ (ngx_resolver_create_addr_query(r, rn, &ctx->addr) != NGX_OK) {
        వెళ్ళు failed;
    }

    rn->చివరి_connection = r->చివరి_connection++;
    ఒకవేళ (r->చివరి_connection == r->connections.nelts) {
        r->చివరి_connection = 0;
    }

    rn->naddrs = (u_short) -1;
    rn->tcp = 0;
#ఒకవేళ (NGX_HAVE_INET6)
    rn->naddrs6 = (u_short) -1;
    rn->tcp6 = 0;
#అయితే_ముగింపు
    rn->nsrvs = 0;

    ఒకవేళ (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->చివరి_connection++;
        ఒకవేళ (rn->చివరి_connection == r->connections.nelts) {
            rn->చివరి_connection = 0;
        }

        (శూన్య) ngx_resolver_send_query(r, rn);
    }

    ఒకవేళ (ngx_resolver_సమితి_timeout(r, ctx) != NGX_OK) {
        వెళ్ళు failed;
    }

    ఒకవేళ (ngx_resolver_resend_ఖాళీలు(r)) {
        ngx_కూడు_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_సమయం() + r->resend_timeout;

    ngx_వరుస_చొప్పించు_తల(resend_వరుస, &rn->వరుస);

    rn->సంకేతం_2 = 0;
    rn->cnlen = 0;
    rn->name = ఖాళీ;
    rn->nlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_విలువ;
    rn->నిరీక్షణ = ctx;

    /* unlock addr mutex */

    ctx->state = NGX_AGAIN;
    ctx->async = 1;
    ctx->కణుపు = rn;

    తిరిగి NGX_OK;

failed:

    ఒకవేళ (rn) {
        ngx_rbtree_delete(వృక్షం, &rn->కణుపు);

        ఒకవేళ (rn->query) {
            ngx_resolver_free(r, rn->query);
        }

        ngx_resolver_free(r, rn);
    }

    /* unlock addr mutex */

    ఒకవేళ (ctx->event) {
        ngx_resolver_free(r, ctx->event);
    }

    ngx_resolver_free(r, ctx);

    తిరిగి NGX_ERROR;
}


శూన్య
ngx_resolve_addr_పూర్తి(ngx_resolver_ctx_t *ctx)
{
    ngx_వరుస_t          *expire_వరుస;
    ngx_rbtree_t         *వృక్షం;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_కణుపు_t  *rn;

    r = ctx->resolver;

    ఎంపిక (ctx->addr.sockaddr->sa_family) {

#ఒకవేళ (NGX_HAVE_INET6)
    స్థితి AF_INET6:
        వృక్షం = &r->addr6_rbtree;
        expire_వరుస = &r->addr6_expire_వరుస;
        ఆపు;
#అయితే_ముగింపు

    అప్రమేయ: /* AF_INET */
        వృక్షం = &r->addr_rbtree;
        expire_వరుస = &r->addr_expire_వరుస;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->సంవర్గమానం, 0,
                   "resolve addr done: %i", ctx->state);

    ఒకవేళ (ctx->event && ctx->event->timer_సమితి) {
        ngx_del_timer(ctx->event);
    }

    /* lock addr mutex */

    ఒకవేళ (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->కణుపు;

        ఒకవేళ (rn) {
            p = &rn->నిరీక్షణ;
            w = rn->నిరీక్షణ;

            అయితే (w) {
                ఒకవేళ (w == ctx) {
                    *p = w->తదుపరి;

                    వెళ్ళు పూర్తి;
                }

                p = &w->తదుపరి;
                w = w->తదుపరి;
            }
        }

        {
            u_char     పాఠ్యం[NGX_SOCKADDR_STRLEN];
            ngx_str_t  addrtext;

            addrtext.దత్తాంశం = పాఠ్యం;
            addrtext.len = ngx_sock_ntop(ctx->addr.sockaddr, ctx->addr.socklen,
                                         పాఠ్యం, NGX_SOCKADDR_STRLEN, 0);

            ngx_log_error(NGX_LOG_ALERT, r->సంవర్గమానం, 0,
                          "could not cancel %V resolving", &addrtext);
        }
    }

పూర్తి:

    ngx_resolver_expire(r, వృక్షం, expire_వరుస);

    /* unlock addr mutex */

    /* lock alloc mutex */

    ఒకవేళ (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    ఒకవేళ (r->event->timer_సమితి && ngx_resolver_resend_ఖాళీలు(r)) {
        ngx_del_timer(r->event);
    }
}


స్థిర శూన్య
ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *వృక్షం, ngx_వరుస_t *వరుస)
{
    సమయం_t                now;
    ngx_uint_t            i;
    ngx_వరుస_t          *q;
    ngx_resolver_కణుపు_t  *rn;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->సంవర్గమానం, 0, "resolver expire");

    now = ngx_సమయం();

    కొరకు (i = 0; i < 2; i++) {
        ఒకవేళ (ngx_వరుస_ఖాళీలు(వరుస)) {
            తిరిగి;
        }

        q = ngx_వరుస_చివరి(వరుస);

        rn = ngx_వరుస_దత్తాంశం(q, ngx_resolver_కణుపు_t, వరుస);

        ఒకవేళ (now <= rn->expire) {
            తిరిగి;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->సంవర్గమానం, 0,
                       "resolver expire \"%*s\"", (కొలత_t) rn->nlen, rn->name);

        ngx_వరుస_remove(q);

        ngx_rbtree_delete(వృక్షం, &rn->కణుపు);

        ngx_resolver_free_కణుపు(r, rn);
    }
}


స్థిర ngx_int_t
ngx_resolver_send_query(ngx_resolver_t *r, ngx_resolver_కణుపు_t *rn)
{
    ngx_int_t                   rc;
    ngx_resolver_connection_t  *rec;

    rec = r->connections.elts;
    rec = &rec[rn->చివరి_connection];

    ఒకవేళ (rec->సంవర్గమానం.handler == ఖాళీ) {
        rec->సంవర్గమానం = *r->సంవర్గమానం;
        rec->సంవర్గమానం.handler = ngx_resolver_log_error;
        rec->సంవర్గమానం.దత్తాంశం = rec;
        rec->సంవర్గమానం.action = "resolving";
    }

    ఒకవేళ (rn->query && rn->naddrs == (u_short) -1) {
        rc = rn->tcp ? ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen)
                     : ngx_resolver_send_udp_query(r, rec, rn->query, rn->qlen);

        ఒకవేళ (rc != NGX_OK) {
            తిరిగి rc;
        }
    }

#ఒకవేళ (NGX_HAVE_INET6)

    ఒకవేళ (rn->query6 && rn->naddrs6 == (u_short) -1) {
        rc = rn->tcp6
                    ? ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen)
                    : ngx_resolver_send_udp_query(r, rec, rn->query6, rn->qlen);

        ఒకవేళ (rc != NGX_OK) {
            తిరిగి rc;
        }
    }

#అయితే_ముగింపు

    తిరిగి NGX_OK;
}


స్థిర ngx_int_t
ngx_resolver_send_udp_query(ngx_resolver_t *r, ngx_resolver_connection_t  *rec,
    u_char *query, u_short qlen)
{
    ssize_t  n;

    ఒకవేళ (rec->udp == ఖాళీ) {
        ఒకవేళ (ngx_udp_connect(rec) != NGX_OK) {
            తిరిగి NGX_ERROR;
        }

        rec->udp->దత్తాంశం = rec;
        rec->udp->పఠ->handler = ngx_resolver_udp_read;
        rec->udp->పఠ->resolver = 1;
    }

    n = ngx_send(rec->udp, query, qlen);

    ఒకవేళ (n == NGX_ERROR) {
        వెళ్ళు failed;
    }

    ఒకవేళ ((కొలత_t) n != (కొలత_t) qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->సంవర్గమానం, 0, "send() incomplete");
        వెళ్ళు failed;
    }

    తిరిగి NGX_OK;

failed:

    ngx_close_connection(rec->udp);
    rec->udp = ఖాళీ;

    తిరిగి NGX_ERROR;
}


స్థిర ngx_int_t
ngx_resolver_send_tcp_query(ngx_resolver_t *r, ngx_resolver_connection_t *rec,
    u_char *query, u_short qlen)
{
    ngx_buf_t  *b;
    ngx_int_t   rc;

    rc = NGX_OK;

    ఒకవేళ (rec->tcp == ఖాళీ) {
        b = rec->read_buf;

        ఒకవేళ (b == ఖాళీ) {
            b = ngx_resolver_calloc(r, పరిమాణం(ngx_buf_t));
            ఒకవేళ (b == ఖాళీ) {
                తిరిగి NGX_ERROR;
            }

            b->ప్రారంభం = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_RSIZE);
            ఒకవేళ (b->ప్రారంభం == ఖాళీ) {
                ngx_resolver_free(r, b);
                తిరిగి NGX_ERROR;
            }

            b->చివర = b->ప్రారంభం + NGX_RESOLVER_TCP_RSIZE;

            rec->read_buf = b;
        }

        b->pos = b->ప్రారంభం;
        b->చివరి = b->ప్రారంభం;

        b = rec->write_buf;

        ఒకవేళ (b == ఖాళీ) {
            b = ngx_resolver_calloc(r, పరిమాణం(ngx_buf_t));
            ఒకవేళ (b == ఖాళీ) {
                తిరిగి NGX_ERROR;
            }

            b->ప్రారంభం = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_WSIZE);
            ఒకవేళ (b->ప్రారంభం == ఖాళీ) {
                ngx_resolver_free(r, b);
                తిరిగి NGX_ERROR;
            }

            b->చివర = b->ప్రారంభం + NGX_RESOLVER_TCP_WSIZE;

            rec->write_buf = b;
        }

        b->pos = b->ప్రారంభం;
        b->చివరి = b->ప్రారంభం;

        rc = ngx_tcp_connect(rec);
        ఒకవేళ (rc == NGX_ERROR) {
            తిరిగి NGX_ERROR;
        }

        rec->tcp->దత్తాంశం = rec;
        rec->tcp->లిఖ->handler = ngx_resolver_tcp_write;
        rec->tcp->లిఖ->cancelable = 1;
        rec->tcp->పఠ->handler = ngx_resolver_tcp_read;
        rec->tcp->పఠ->resolver = 1;

        ngx_కూడు_timer(rec->tcp->లిఖ, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    b = rec->write_buf;

    ఒకవేళ (b->చివర - b->చివరి <  2 + qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->సంవర్గమానం, 0, "buffer overflow");
        తిరిగి NGX_ERROR;
    }

    *b->చివరి++ = (u_char) (qlen >> 8);
    *b->చివరి++ = (u_char) qlen;
    b->చివరి = ngx_cpymem(b->చివరి, query, qlen);

    ఒకవేళ (rc == NGX_OK) {
        ngx_resolver_tcp_write(rec->tcp->లిఖ);
    }

    తిరిగి NGX_OK;
}


స్థిర శూన్య
ngx_resolver_resend_handler(ngx_event_t *ev)
{
    సమయం_t           timer, atimer, stimer, ntimer;
#ఒకవేళ (NGX_HAVE_INET6)
    సమయం_t           a6timer;
#అయితే_ముగింపు
    ngx_resolver_t  *r;

    r = ev->దత్తాంశం;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->సంవర్గమానం, 0,
                   "resolver resend handler");

    /* lock name mutex */

    ntimer = ngx_resolver_resend(r, &r->name_rbtree, &r->name_resend_వరుస);

    stimer = ngx_resolver_resend(r, &r->srv_rbtree, &r->srv_resend_వరుస);

    /* unlock name mutex */

    /* lock addr mutex */

    atimer = ngx_resolver_resend(r, &r->addr_rbtree, &r->addr_resend_వరుస);

    /* unlock addr mutex */

#ఒకవేళ (NGX_HAVE_INET6)

    /* lock addr6 mutex */

    a6timer = ngx_resolver_resend(r, &r->addr6_rbtree, &r->addr6_resend_వరుస);

    /* unlock addr6 mutex */

#అయితే_ముగింపు

    timer = ntimer;

    ఒకవేళ (timer == 0) {
        timer = atimer;

    } లేకపోతే ఒకవేళ (atimer) {
        timer = ngx_min(timer, atimer);
    }

    ఒకవేళ (timer == 0) {
        timer = stimer;

    } లేకపోతే ఒకవేళ (stimer) {
        timer = ngx_min(timer, stimer);
    }

#ఒకవేళ (NGX_HAVE_INET6)

    ఒకవేళ (timer == 0) {
        timer = a6timer;

    } లేకపోతే ఒకవేళ (a6timer) {
        timer = ngx_min(timer, a6timer);
    }

#అయితే_ముగింపు

    ఒకవేళ (timer) {
        ngx_కూడు_timer(r->event, (ngx_msec_t) (timer * 1000));
    }
}


స్థిర సమయం_t
ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *వృక్షం, ngx_వరుస_t *వరుస)
{
    సమయం_t                now;
    ngx_వరుస_t          *q;
    ngx_resolver_కణుపు_t  *rn;

    now = ngx_సమయం();

    కొరకు ( ;; ) {
        ఒకవేళ (ngx_వరుస_ఖాళీలు(వరుస)) {
            తిరిగి 0;
        }

        q = ngx_వరుస_చివరి(వరుస);

        rn = ngx_వరుస_దత్తాంశం(q, ngx_resolver_కణుపు_t, వరుస);

        ఒకవేళ (now < rn->expire) {
            తిరిగి rn->expire - now;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->సంవర్గమానం, 0,
                       "resolver resend \"%*s\" %p",
                       (కొలత_t) rn->nlen, rn->name, rn->నిరీక్షణ);

        ngx_వరుస_remove(q);

        ఒకవేళ (rn->నిరీక్షణ) {

            ఒకవేళ (++rn->చివరి_connection == r->connections.nelts) {
                rn->చివరి_connection = 0;
            }

            (శూన్య) ngx_resolver_send_query(r, rn);

            rn->expire = now + r->resend_timeout;

            ngx_వరుస_చొప్పించు_తల(వరుస, q);

            కొనసాగు;
        }

        ngx_rbtree_delete(వృక్షం, &rn->కణుపు);

        ngx_resolver_free_కణుపు(r, rn);
    }
}


స్థిర ngx_uint_t
ngx_resolver_resend_ఖాళీలు(ngx_resolver_t *r)
{
    తిరిగి ngx_వరుస_ఖాళీలు(&r->name_resend_వరుస)
           && ngx_వరుస_ఖాళీలు(&r->srv_resend_వరుస)
#ఒకవేళ (NGX_HAVE_INET6)
           && ngx_వరుస_ఖాళీలు(&r->addr6_resend_వరుస)
#అయితే_ముగింపు
           && ngx_వరుస_ఖాళీలు(&r->addr_resend_వరుస);
}


స్థిర శూన్య
ngx_resolver_udp_read(ngx_event_t *rev)
{
    ssize_t                     n;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;
    u_char                      buf[NGX_RESOLVER_UDP_కొలత];

    c = rev->దత్తాంశం;
    rec = c->దత్తాంశం;

    చేయి {
        n = ngx_udp_recv(c, buf, NGX_RESOLVER_UDP_కొలత);

        ఒకవేళ (n == NGX_AGAIN) {
            ఆపు;
        }

        ఒకవేళ (n == NGX_ERROR) {
            వెళ్ళు failed;
        }

        ngx_resolver_process_response(rec->resolver, buf, n, 0);

    } అయితే (rev->ready);

    ఒకవేళ (ngx_handle_read_event(rev, 0) != NGX_OK) {
        వెళ్ళు failed;
    }

    తిరిగి;

failed:

    ngx_close_connection(rec->udp);
    rec->udp = ఖాళీ;
}


స్థిర శూన్య
ngx_resolver_tcp_write(ngx_event_t *wev)
{
    off_t                       పంపిన;
    ssize_t                     n;
    ngx_buf_t                  *b;
    ngx_resolver_t             *r;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;

    c = wev->దత్తాంశం;
    rec = c->దత్తాంశం;
    b = rec->write_buf;
    r = rec->resolver;

    ఒకవేళ (wev->timedout) {
        వెళ్ళు failed;
    }

    పంపిన = c->పంపిన;

    అయితే (wev->ready && b->pos < b->చివరి) {
        n = ngx_send(c, b->pos, b->చివరి - b->pos);

        ఒకవేళ (n == NGX_AGAIN) {
            ఆపు;
        }

        ఒకవేళ (n == NGX_ERROR) {
            వెళ్ళు failed;
        }

        b->pos += n;
    }

    ఒకవేళ (b->pos != b->ప్రారంభం) {
        b->చివరి = ngx_movemem(b->ప్రారంభం, b->pos, b->చివరి - b->pos);
        b->pos = b->ప్రారంభం;
    }

    ఒకవేళ (c->పంపిన != పంపిన) {
        ngx_కూడు_timer(wev, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    ఒకవేళ (ngx_handle_write_event(wev, 0) != NGX_OK) {
        వెళ్ళు failed;
    }

    తిరిగి;

failed:

    ngx_close_connection(c);
    rec->tcp = ఖాళీ;
}


స్థిర శూన్య
ngx_resolver_tcp_read(ngx_event_t *rev)
{
    u_char                     *p;
    కొలత_t                      కొలత;
    ssize_t                     n;
    u_short                     qlen;
    ngx_buf_t                  *b;
    ngx_resolver_t             *r;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;

    c = rev->దత్తాంశం;
    rec = c->దత్తాంశం;
    b = rec->read_buf;
    r = rec->resolver;

    అయితే (rev->ready) {
        n = ngx_recv(c, b->చివరి, b->చివర - b->చివరి);

        ఒకవేళ (n == NGX_AGAIN) {
            ఆపు;
        }

        ఒకవేళ (n == NGX_ERROR || n == 0) {
            వెళ్ళు failed;
        }

        b->చివరి += n;

        కొరకు ( ;; ) {
            p = b->pos;
            కొలత = b->చివరి - p;

            ఒకవేళ (కొలత < 2) {
                ఆపు;
            }

            qlen = (u_short) *p++ << 8;
            qlen += *p++;

            ఒకవేళ (కొలత < (కొలత_t) (2 + qlen)) {
                ఆపు;
            }

            ngx_resolver_process_response(r, p, qlen, 1);

            b->pos += 2 + qlen;
        }

        ఒకవేళ (b->pos != b->ప్రారంభం) {
            b->చివరి = ngx_movemem(b->ప్రారంభం, b->pos, b->చివరి - b->pos);
            b->pos = b->ప్రారంభం;
        }
    }

    ఒకవేళ (ngx_handle_read_event(rev, 0) != NGX_OK) {
        వెళ్ళు failed;
    }

    తిరిగి;

failed:

    ngx_close_connection(c);
    rec->tcp = ఖాళీ;
}


స్థిర శూన్య
ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf, కొలత_t n,
    ngx_uint_t tcp)
{
    అక్షరం                 *err;
    ngx_uint_t            i, times, ident, qident, flags, సంకేతం_2, nqs, nan, trunc,
                          qtype, qclass;
#ఒకవేళ (NGX_HAVE_INET6)
    ngx_uint_t            qident6;
#అయితే_ముగింపు
    ngx_వరుస_t          *q;
    ngx_resolver_qs_t    *qs;
    ngx_resolver_hdr_t   *response;
    ngx_resolver_కణుపు_t  *rn;

    ఒకవేళ (n < పరిమాణం(ngx_resolver_hdr_t)) {
        వెళ్ళు short_response;
    }

    response = (ngx_resolver_hdr_t *) buf;

    ident = (response->ident_hi << 8) + response->ident_lo;
    flags = (response->flags_hi << 8) + response->flags_lo;
    nqs = (response->nqs_hi << 8) + response->nqs_lo;
    nan = (response->nan_hi << 8) + response->nan_lo;
    trunc = flags & 0x0200;

    ngx_log_debug6(NGX_LOG_DEBUG_CORE, r->సంవర్గమానం, 0,
                   "resolver DNS response %ui fl:%04Xi %ui/%ui/%ud/%ud",
                   ident, flags, nqs, nan,
                   (response->nns_hi << 8) + response->nns_lo,
                   (response->nar_hi << 8) + response->nar_lo);

    /* response to a standard query */
    ఒకవేళ ((flags & 0xf870) != 0x8000 || (trunc && tcp)) {
        ngx_log_error(r->log_level, r->సంవర్గమానం, 0,
                      "invalid %s DNS response %ui fl:%04Xi",
                      tcp ? "TCP" : "UDP", ident, flags);
        తిరిగి;
    }

    సంకేతం_2 = flags & 0xf;

    ఒకవేళ (సంకేతం_2 == NGX_RESOLVE_FORMERR) {

        times = 0;

        కొరకు (q = ngx_వరుస_తల(&r->name_resend_వరుస);
             q != ngx_వరుస_sentinel(&r->name_resend_వరుస) && times++ < 100;
             q = ngx_వరుస_తదుపరి(q))
        {
            rn = ngx_వరుస_దత్తాంశం(q, ngx_resolver_కణుపు_t, వరుస);

            ఒకవేళ (rn->query) {
                qident = (rn->query[0] << 8) + rn->query[1];

                ఒకవేళ (qident == ident) {
                    వెళ్ళు dns_error_name;
                }
            }

#ఒకవేళ (NGX_HAVE_INET6)
            ఒకవేళ (rn->query6) {
                qident6 = (rn->query6[0] << 8) + rn->query6[1];

                ఒకవేళ (qident6 == ident) {
                    వెళ్ళు dns_error_name;
                }
            }
#అయితే_ముగింపు
        }

        వెళ్ళు dns_error;
    }

    ఒకవేళ (సంకేతం_2 > NGX_RESOLVE_REFUSED) {
        వెళ్ళు dns_error;
    }

    ఒకవేళ (nqs != 1) {
        err = "invalid number of questions in DNS response";
        వెళ్ళు పూర్తి;
    }

    i = పరిమాణం(ngx_resolver_hdr_t);

    అయితే (i < (ngx_uint_t) n) {

        ఒకవేళ (buf[i] & 0xc0) {
            err = "unexpected compression pointer in DNS response";
            వెళ్ళు పూర్తి;
        }

        ఒకవేళ (buf[i] == '\0') {
            వెళ్ళు దొరికింది;
        }

        i += 1 + buf[i];
    }

    వెళ్ళు short_response;

దొరికింది:

    ఒకవేళ (i++ == పరిమాణం(ngx_resolver_hdr_t)) {
        err = "zero-length domain name in DNS response";
        వెళ్ళు పూర్తి;
    }

    ఒకవేళ (i + పరిమాణం(ngx_resolver_qs_t) + nan * (2 + పరిమాణం(ngx_resolver_an_t))
        > (ngx_uint_t) n)
    {
        వెళ్ళు short_response;
    }

    qs = (ngx_resolver_qs_t *) &buf[i];

    qtype = (qs->type_hi << 8) + qs->type_lo;
    qclass = (qs->class_hi << 8) + qs->class_lo;

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->సంవర్గమానం, 0,
                   "resolver DNS response qt:%ui cl:%ui", qtype, qclass);

    ఒకవేళ (qclass != 1) {
        ngx_log_error(r->log_level, r->సంవర్గమానం, 0,
                      "unknown query class %ui in DNS response", qclass);
        తిరిగి;
    }

    ఎంపిక (qtype) {

    స్థితి NGX_RESOLVE_A:
#ఒకవేళ (NGX_HAVE_INET6)
    స్థితి NGX_RESOLVE_AAAA:
#అయితే_ముగింపు

        ngx_resolver_process_a(r, buf, n, ident, సంకేతం_2, qtype, nan, trunc,
                               i + పరిమాణం(ngx_resolver_qs_t));

        ఆపు;

    స్థితి NGX_RESOLVE_SRV:

        ngx_resolver_process_srv(r, buf, n, ident, సంకేతం_2, nan, trunc,
                                 i + పరిమాణం(ngx_resolver_qs_t));

        ఆపు;

    స్థితి NGX_RESOLVE_PTR:

        ngx_resolver_process_ptr(r, buf, n, ident, సంకేతం_2, nan);

        ఆపు;

    అప్రమేయ:
        ngx_log_error(r->log_level, r->సంవర్గమానం, 0,
                      "unknown query type %ui in DNS response", qtype);
        తిరిగి;
    }

    తిరిగి;

short_response:

    err = "short DNS response";

పూర్తి:

    ngx_log_error(r->log_level, r->సంవర్గమానం, 0, err);

    తిరిగి;

dns_error_name:

    ngx_log_error(r->log_level, r->సంవర్గమానం, 0,
                  "DNS error (%ui: %s), query id:%ui, name:\"%*s\"",
                  సంకేతం_2, ngx_resolver_strerror(సంకేతం_2), ident,
                  (కొలత_t) rn->nlen, rn->name);
    తిరిగి;

dns_error:

    ngx_log_error(r->log_level, r->సంవర్గమానం, 0,
                  "DNS error (%ui: %s), query id:%ui",
                  సంకేతం_2, ngx_resolver_strerror(సంకేతం_2), ident);
    తిరిగి;
}


స్థిర శూన్య
ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, కొలత_t n,
    ngx_uint_t ident, ngx_uint_t సంకేతం_2, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans)
{
    అక్షరం                       *err;
    u_char                     *cname;
    కొలత_t                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    లోపల_addr_t                  *addr;
    ngx_str_t                   name;
    ngx_uint_t                  type, class, qident, naddrs, a, i, j, ప్రారంభం;
#ఒకవేళ (NGX_HAVE_INET6)
    నిర్మాణం in6_addr            *addr6;
#అయితే_ముగింపు
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *తదుపరి;
    ngx_resolver_కణుపు_t        *rn;
    ngx_resolver_addr_t        *addrs;
    ngx_resolver_connection_t  *rec;

    ఒకవేళ (ngx_resolver_copy(r, &name, buf,
                          buf + పరిమాణం(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        తిరిగి;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->సంవర్గమానం, 0, "resolver qs:%V", &name);

    hash = ngx_చక్రీయ_తనిఖీ_32_short(name.దత్తాంశం, name.len);

    /* lock name mutex */

    rn = ngx_resolver_lookup_name(r, &name, hash);

    ఒకవేళ (rn == ఖాళీ) {
        ngx_log_error(r->log_level, r->సంవర్గమానం, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.దత్తాంశం);
        వెళ్ళు failed;
    }

    ఎంపిక (qtype) {

#ఒకవేళ (NGX_HAVE_INET6)
    స్థితి NGX_RESOLVE_AAAA:

        ఒకవేళ (rn->query6 == ఖాళీ || rn->naddrs6 != (u_short) -1) {
            ngx_log_error(r->log_level, r->సంవర్గమానం, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.దత్తాంశం);
            వెళ్ళు failed;
        }

        ఒకవేళ (trunc && rn->tcp6) {
            ngx_resolver_free(r, name.దత్తాంశం);
            వెళ్ళు failed;
        }

        qident = (rn->query6[0] << 8) + rn->query6[1];

        ఆపు;
#అయితే_ముగింపు

    అప్రమేయ: /* NGX_RESOLVE_A */

        ఒకవేళ (rn->query == ఖాళీ || rn->naddrs != (u_short) -1) {
            ngx_log_error(r->log_level, r->సంవర్గమానం, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.దత్తాంశం);
            వెళ్ళు failed;
        }

        ఒకవేళ (trunc && rn->tcp) {
            ngx_resolver_free(r, name.దత్తాంశం);
            వెళ్ళు failed;
        }

        qident = (rn->query[0] << 8) + rn->query[1];
    }

    ఒకవేళ (ident != qident) {
        ngx_log_error(r->log_level, r->సంవర్గమానం, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.దత్తాంశం);
        వెళ్ళు failed;
    }

    ngx_resolver_free(r, name.దత్తాంశం);

    ఒకవేళ (trunc) {

        ngx_వరుస_remove(&rn->వరుస);

        ఒకవేళ (rn->నిరీక్షణ == ఖాళీ) {
            ngx_rbtree_delete(&r->name_rbtree, &rn->కణుపు);
            ngx_resolver_free_కణుపు(r, rn);
            వెళ్ళు తదుపరి;
        }

        rec = r->connections.elts;
        rec = &rec[rn->చివరి_connection];

        ఎంపిక (qtype) {

#ఒకవేళ (NGX_HAVE_INET6)
        స్థితి NGX_RESOLVE_AAAA:

            rn->tcp6 = 1;

            (శూన్య) ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen);

            ఆపు;
#అయితే_ముగింపు

        అప్రమేయ: /* NGX_RESOLVE_A */

            rn->tcp = 1;

            (శూన్య) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);
        }

        rn->expire = ngx_సమయం() + r->resend_timeout;

        ngx_వరుస_చొప్పించు_తల(&r->name_resend_వరుస, &rn->వరుస);

        వెళ్ళు తదుపరి;
    }

    ఒకవేళ (సంకేతం_2 == 0 && rn->సంకేతం_2) {
        సంకేతం_2 = rn->సంకేతం_2;
    }

    ఒకవేళ (సంకేతం_2 == 0 && nan == 0) {

#ఒకవేళ (NGX_HAVE_INET6)
        ఎంపిక (qtype) {

        స్థితి NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            ఒకవేళ (rn->naddrs == (u_short) -1) {
                వెళ్ళు తదుపరి;
            }

            ఒకవేళ (rn->naddrs) {
                వెళ్ళు export;
            }

            ఆపు;

        అప్రమేయ: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            ఒకవేళ (rn->naddrs6 == (u_short) -1) {
                వెళ్ళు తదుపరి;
            }

            ఒకవేళ (rn->naddrs6) {
                వెళ్ళు export;
            }
        }
#అయితే_ముగింపు

        సంకేతం_2 = NGX_RESOLVE_NXDOMAIN;
    }

    ఒకవేళ (సంకేతం_2) {

#ఒకవేళ (NGX_HAVE_INET6)
        ఎంపిక (qtype) {

        స్థితి NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            ఒకవేళ (rn->naddrs == (u_short) -1) {
                rn->సంకేతం_2 = (u_char) సంకేతం_2;
                వెళ్ళు తదుపరి;
            }

            ఆపు;

        అప్రమేయ: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            ఒకవేళ (rn->naddrs6 == (u_short) -1) {
                rn->సంకేతం_2 = (u_char) సంకేతం_2;
                వెళ్ళు తదుపరి;
            }
        }
#అయితే_ముగింపు

        తదుపరి = rn->నిరీక్షణ;
        rn->నిరీక్షణ = ఖాళీ;

        ngx_వరుస_remove(&rn->వరుస);

        ngx_rbtree_delete(&r->name_rbtree, &rn->కణుపు);

        /* unlock name mutex */

        అయితే (తదుపరి) {
            ctx = తదుపరి;
            ctx->state = సంకేతం_2;
            ctx->valid = ngx_సమయం() + (r->valid ? r->valid : 10);
            తదుపరి = ctx->తదుపరి;

            ctx->handler(ctx);
        }

        ngx_resolver_free_కణుపు(r, rn);

        తిరిగి;
    }

    i = ans;
    naddrs = 0;
    cname = ఖాళీ;

    కొరకు (a = 0; a < nan; a++) {

        ప్రారంభం = i;

        అయితే (i < n) {

            ఒకవేళ (buf[i] & 0xc0) {
                i += 2;
                వెళ్ళు దొరికింది;
            }

            ఒకవేళ (buf[i] == 0) {
                i++;
                వెళ్ళు test_నిడివి;
            }

            i += 1 + buf[i];
        }

        వెళ్ళు short_response;

    test_నిడివి:

        ఒకవేళ (i - ప్రారంభం < 2) {
            err = "invalid name in DNS response";
            వెళ్ళు invalid;
        }

    దొరికింది:

        ఒకవేళ (i + పరిమాణం(ngx_resolver_an_t) >= n) {
            వెళ్ళు short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        ఒకవేళ (class != 1) {
            ngx_log_error(r->log_level, r->సంవర్గమానం, 0,
                          "unexpected RR class %ui in DNS response", class);
            వెళ్ళు failed;
        }

        ఒకవేళ (ttl < 0) {
            ttl = 0;
        }

        rn->ttl = ngx_min(rn->ttl, (uint32_t) ttl);

        i += పరిమాణం(ngx_resolver_an_t);

        ఎంపిక (type) {

        స్థితి NGX_RESOLVE_A:

            ఒకవేళ (qtype != NGX_RESOLVE_A) {
                err = "unexpected A record in DNS response";
                వెళ్ళు invalid;
            }

            ఒకవేళ (len != 4) {
                err = "invalid A record in DNS response";
                వెళ్ళు invalid;
            }

            ఒకవేళ (i + 4 > n) {
                వెళ్ళు short_response;
            }

            naddrs++;

            ఆపు;

#ఒకవేళ (NGX_HAVE_INET6)
        స్థితి NGX_RESOLVE_AAAA:

            ఒకవేళ (qtype != NGX_RESOLVE_AAAA) {
                err = "unexpected AAAA record in DNS response";
                వెళ్ళు invalid;
            }

            ఒకవేళ (len != 16) {
                err = "invalid AAAA record in DNS response";
                వెళ్ళు invalid;
            }

            ఒకవేళ (i + 16 > n) {
                వెళ్ళు short_response;
            }

            naddrs++;

            ఆపు;
#అయితే_ముగింపు

        స్థితి NGX_RESOLVE_CNAME:

            cname = &buf[i];

            ఆపు;

        స్థితి NGX_RESOLVE_DNAME:

            ఆపు;

        అప్రమేయ:

            ngx_log_error(r->log_level, r->సంవర్గమానం, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->సంవర్గమానం, 0,
                   "resolver naddrs:%ui cname:%p ttl:%uD",
                   naddrs, cname, rn->ttl);

    ఒకవేళ (naddrs) {

        ఎంపిక (qtype) {

#ఒకవేళ (NGX_HAVE_INET6)
        స్థితి NGX_RESOLVE_AAAA:

            ఒకవేళ (naddrs == 1) {
                addr6 = &rn->u6.addr6;
                rn->naddrs6 = 1;

            } లేకపోతే {
                addr6 = ngx_resolver_alloc(r, naddrs * పరిమాణం(నిర్మాణం in6_addr));
                ఒకవేళ (addr6 == ఖాళీ) {
                    వెళ్ళు failed;
                }

                rn->u6.addrs6 = addr6;
                rn->naddrs6 = (u_short) naddrs;
            }

#ఒకవేళ (NGX_SUPPRESS_WARN)
            addr = ఖాళీ;
#అయితే_ముగింపు

            ఆపు;
#అయితే_ముగింపు

        అప్రమేయ: /* NGX_RESOLVE_A */

            ఒకవేళ (naddrs == 1) {
                addr = &rn->u.addr;
                rn->naddrs = 1;

            } లేకపోతే {
                addr = ngx_resolver_alloc(r, naddrs * పరిమాణం(లోపల_addr_t));
                ఒకవేళ (addr == ఖాళీ) {
                    వెళ్ళు failed;
                }

                rn->u.addrs = addr;
                rn->naddrs = (u_short) naddrs;
            }

#ఒకవేళ (NGX_HAVE_INET6 && NGX_SUPPRESS_WARN)
            addr6 = ఖాళీ;
#అయితే_ముగింపు
        }

        j = 0;
        i = ans;

        కొరకు (a = 0; a < nan; a++) {

            కొరకు ( ;; ) {

                ఒకవేళ (buf[i] & 0xc0) {
                    i += 2;
                    ఆపు;
                }

                ఒకవేళ (buf[i] == 0) {
                    i++;
                    ఆపు;
                }

                i += 1 + buf[i];
            }

            an = (ngx_resolver_an_t *) &buf[i];

            type = (an->type_hi << 8) + an->type_lo;
            len = (an->len_hi << 8) + an->len_lo;

            i += పరిమాణం(ngx_resolver_an_t);

            ఒకవేళ (type == NGX_RESOLVE_A) {

                addr[j] = htonl((buf[i] << 24) + (buf[i + 1] << 16)
                                + (buf[i + 2] << 8) + (buf[i + 3]));

                ఒకవేళ (++j == naddrs) {

#ఒకవేళ (NGX_HAVE_INET6)
                    ఒకవేళ (rn->naddrs6 == (u_short) -1) {
                        వెళ్ళు తదుపరి;
                    }
#అయితే_ముగింపు

                    ఆపు;
                }
            }

#ఒకవేళ (NGX_HAVE_INET6)
            లేకపోతే ఒకవేళ (type == NGX_RESOLVE_AAAA) {

                ngx_memcpy(addr6[j].s6_addr, &buf[i], 16);

                ఒకవేళ (++j == naddrs) {

                    ఒకవేళ (rn->naddrs == (u_short) -1) {
                        వెళ్ళు తదుపరి;
                    }

                    ఆపు;
                }
            }
#అయితే_ముగింపు

            i += len;
        }
    }

    ఎంపిక (qtype) {

#ఒకవేళ (NGX_HAVE_INET6)
    స్థితి NGX_RESOLVE_AAAA:

        ఒకవేళ (rn->naddrs6 == (u_short) -1) {
            rn->naddrs6 = 0;
        }

        ఆపు;
#అయితే_ముగింపు

    అప్రమేయ: /* NGX_RESOLVE_A */

        ఒకవేళ (rn->naddrs == (u_short) -1) {
            rn->naddrs = 0;
        }
    }

    ఒకవేళ (rn->naddrs != (u_short) -1
#ఒకవేళ (NGX_HAVE_INET6)
        && rn->naddrs6 != (u_short) -1
#అయితే_ముగింపు
        && rn->naddrs
#ఒకవేళ (NGX_HAVE_INET6)
           + rn->naddrs6
#అయితే_ముగింపు
           > 0)
    {

#ఒకవేళ (NGX_HAVE_INET6)
    export:
#అయితే_ముగింపు

        naddrs = rn->naddrs;
#ఒకవేళ (NGX_HAVE_INET6)
        naddrs += rn->naddrs6;
#అయితే_ముగింపు

        ఒకవేళ (naddrs == 1 && rn->naddrs == 1) {
            addrs = ఖాళీ;

        } లేకపోతే {
            addrs = ngx_resolver_export(r, rn, 0);
            ఒకవేళ (addrs == ఖాళీ) {
                వెళ్ళు failed;
            }
        }

        ngx_వరుస_remove(&rn->వరుస);

        rn->valid = ngx_సమయం() + (r->valid ? r->valid : (సమయం_t) rn->ttl);
        rn->expire = ngx_సమయం() + r->expire;

        ngx_వరుస_చొప్పించు_తల(&r->name_expire_వరుస, &rn->వరుస);

        తదుపరి = rn->నిరీక్షణ;
        rn->నిరీక్షణ = ఖాళీ;

        /* unlock name mutex */

        అయితే (తదుపరి) {
            ctx = తదుపరి;
            ctx->state = NGX_OK;
            ctx->valid = rn->valid;
            ctx->naddrs = naddrs;

            ఒకవేళ (addrs == ఖాళీ) {
                ctx->addrs = &ctx->addr;
                ctx->addr.sockaddr = (నిర్మాణం sockaddr *) &ctx->జ్యా;
                ctx->addr.socklen = పరిమాణం(నిర్మాణం sockaddr_లోపల);
                ngx_memzero(&ctx->జ్యా, పరిమాణం(నిర్మాణం sockaddr_లోపల));
                ctx->జ్యా.sin_family = AF_INET;
                ctx->జ్యా.sin_addr.s_addr = rn->u.addr;

            } లేకపోతే {
                ctx->addrs = addrs;
            }

            తదుపరి = ctx->తదుపరి;

            ctx->handler(ctx);
        }

        ఒకవేళ (addrs != ఖాళీ) {
            ngx_resolver_free(r, addrs->sockaddr);
            ngx_resolver_free(r, addrs);
        }

        ngx_resolver_free(r, rn->query);
        rn->query = ఖాళీ;
#ఒకవేళ (NGX_HAVE_INET6)
        rn->query6 = ఖాళీ;
#అయితే_ముగింపు

        తిరిగి;
    }

    ఒకవేళ (cname) {

        /* CNAME only */

        ఒకవేళ (rn->naddrs == (u_short) -1
#ఒకవేళ (NGX_HAVE_INET6)
            || rn->naddrs6 == (u_short) -1
#అయితే_ముగింపు
            )
        {
            వెళ్ళు తదుపరి;
        }

        ఒకవేళ (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            వెళ్ళు failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->సంవర్గమానం, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_వరుస_remove(&rn->వరుస);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.దత్తాంశం;

        rn->valid = ngx_సమయం() + (r->valid ? r->valid : (సమయం_t) rn->ttl);
        rn->expire = ngx_సమయం() + r->expire;

        ngx_వరుస_చొప్పించు_తల(&r->name_expire_వరుస, &rn->వరుస);

        ngx_resolver_free(r, rn->query);
        rn->query = ఖాళీ;
#ఒకవేళ (NGX_HAVE_INET6)
        rn->query6 = ఖాళీ;
#అయితే_ముగింపు

        ctx = rn->నిరీక్షణ;
        rn->నిరీక్షణ = ఖాళీ;

        ఒకవేళ (ctx) {

            ఒకవేళ (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                చేయి {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    తదుపరి = ctx->తదుపరి;

                    ctx->handler(ctx);

                    ctx = తదుపరి;
                } అయితే (ctx);

                తిరిగి;
            }

            కొరకు (తదుపరి = ctx; తదుపరి; తదుపరి = తదుపరి->తదుపరి) {
                తదుపరి->కణుపు = ఖాళీ;
            }

            (శూన్య) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        తిరిగి;
    }

    ngx_log_error(r->log_level, r->సంవర్గమానం, 0,
                  "no A or CNAME types in DNS response");
    తిరిగి;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->సంవర్గమానం, 0, err);

    తిరిగి;

failed:

తదుపరి:

    /* unlock name mutex */

    తిరిగి;
}


స్థిర శూన్య
ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, కొలత_t n,
    ngx_uint_t ident, ngx_uint_t సంకేతం_2, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans)
{
    అక్షరం                       *err;
    u_char                     *cname;
    కొలత_t                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    ngx_str_t                   name;
    ngx_uint_t                  type, qident, class, ప్రారంభం, nsrvs, a, i, j;
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *తదుపరి;
    ngx_resolver_srv_t         *srvs;
    ngx_resolver_కణుపు_t        *rn;
    ngx_resolver_connection_t  *rec;

    ఒకవేళ (ngx_resolver_copy(r, &name, buf,
                          buf + పరిమాణం(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        తిరిగి;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->సంవర్గమానం, 0, "resolver qs:%V", &name);

    hash = ngx_చక్రీయ_తనిఖీ_32_short(name.దత్తాంశం, name.len);

    rn = ngx_resolver_lookup_srv(r, &name, hash);

    ఒకవేళ (rn == ఖాళీ || rn->query == ఖాళీ) {
        ngx_log_error(r->log_level, r->సంవర్గమానం, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.దత్తాంశం);
        వెళ్ళు failed;
    }

    ఒకవేళ (trunc && rn->tcp) {
        ngx_resolver_free(r, name.దత్తాంశం);
        వెళ్ళు failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    ఒకవేళ (ident != qident) {
        ngx_log_error(r->log_level, r->సంవర్గమానం, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.దత్తాంశం);
        వెళ్ళు failed;
    }

    ngx_resolver_free(r, name.దత్తాంశం);

    ఒకవేళ (trunc) {

        ngx_వరుస_remove(&rn->వరుస);

        ఒకవేళ (rn->నిరీక్షణ == ఖాళీ) {
            ngx_rbtree_delete(&r->srv_rbtree, &rn->కణుపు);
            ngx_resolver_free_కణుపు(r, rn);
            తిరిగి;
        }

        rec = r->connections.elts;
        rec = &rec[rn->చివరి_connection];

        rn->tcp = 1;

        (శూన్య) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);

        rn->expire = ngx_సమయం() + r->resend_timeout;

        ngx_వరుస_చొప్పించు_తల(&r->srv_resend_వరుస, &rn->వరుస);

        తిరిగి;
    }

    ఒకవేళ (సంకేతం_2 == 0 && rn->సంకేతం_2) {
        సంకేతం_2 = rn->సంకేతం_2;
    }

    ఒకవేళ (సంకేతం_2 == 0 && nan == 0) {
        సంకేతం_2 = NGX_RESOLVE_NXDOMAIN;
    }

    ఒకవేళ (సంకేతం_2) {
        తదుపరి = rn->నిరీక్షణ;
        rn->నిరీక్షణ = ఖాళీ;

        ngx_వరుస_remove(&rn->వరుస);

        ngx_rbtree_delete(&r->srv_rbtree, &rn->కణుపు);

        అయితే (తదుపరి) {
            ctx = తదుపరి;
            ctx->state = సంకేతం_2;
            ctx->valid = ngx_సమయం() + (r->valid ? r->valid : 10);
            తదుపరి = ctx->తదుపరి;

            ctx->handler(ctx);
        }

        ngx_resolver_free_కణుపు(r, rn);

        తిరిగి;
    }

    i = ans;
    nsrvs = 0;
    cname = ఖాళీ;

    కొరకు (a = 0; a < nan; a++) {

        ప్రారంభం = i;

        అయితే (i < n) {

            ఒకవేళ (buf[i] & 0xc0) {
                i += 2;
                వెళ్ళు దొరికింది;
            }

            ఒకవేళ (buf[i] == 0) {
                i++;
                వెళ్ళు test_నిడివి;
            }

            i += 1 + buf[i];
        }

        వెళ్ళు short_response;

    test_నిడివి:

        ఒకవేళ (i - ప్రారంభం < 2) {
            err = "invalid name DNS response";
            వెళ్ళు invalid;
        }

    దొరికింది:

        ఒకవేళ (i + పరిమాణం(ngx_resolver_an_t) >= n) {
            వెళ్ళు short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        ఒకవేళ (class != 1) {
            ngx_log_error(r->log_level, r->సంవర్గమానం, 0,
                          "unexpected RR class %ui in DNS response", class);
            వెళ్ళు failed;
        }

        ఒకవేళ (ttl < 0) {
            ttl = 0;
        }

        rn->ttl = ngx_min(rn->ttl, (uint32_t) ttl);

        i += పరిమాణం(ngx_resolver_an_t);

        ఎంపిక (type) {

        స్థితి NGX_RESOLVE_SRV:

            ఒకవేళ (i + 6 >= n) {
                వెళ్ళు short_response;
            }

            ఒకవేళ (ngx_resolver_copy(r, ఖాళీ, buf, &buf[i + 6], buf + n)
                != NGX_OK)
            {
                వెళ్ళు failed;
            }

            nsrvs++;

            ఆపు;

        స్థితి NGX_RESOLVE_CNAME:

            cname = &buf[i];

            ఆపు;

        స్థితి NGX_RESOLVE_DNAME:

            ఆపు;

        అప్రమేయ:

            ngx_log_error(r->log_level, r->సంవర్గమానం, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->సంవర్గమానం, 0,
                   "resolver nsrvs:%ui cname:%p ttl:%uD",
                   nsrvs, cname, rn->ttl);

    ఒకవేళ (nsrvs) {

        srvs = ngx_resolver_calloc(r, nsrvs * పరిమాణం(ngx_resolver_srv_t));
        ఒకవేళ (srvs == ఖాళీ) {
            వెళ్ళు failed;
        }

        rn->u.srvs = srvs;
        rn->nsrvs = (u_short) nsrvs;

        j = 0;
        i = ans;

        కొరకు (a = 0; a < nan; a++) {

            కొరకు ( ;; ) {

                ఒకవేళ (buf[i] & 0xc0) {
                    i += 2;
                    ఆపు;
                }

                ఒకవేళ (buf[i] == 0) {
                    i++;
                    ఆపు;
                }

                i += 1 + buf[i];
            }

            an = (ngx_resolver_an_t *) &buf[i];

            type = (an->type_hi << 8) + an->type_lo;
            len = (an->len_hi << 8) + an->len_lo;

            i += పరిమాణం(ngx_resolver_an_t);

            ఒకవేళ (type == NGX_RESOLVE_SRV) {

                srvs[j].ప్రాధాన్యత = (buf[i] << 8) + buf[i + 1];
                srvs[j].బరువు = (buf[i + 2] << 8) + buf[i + 3];

                ఒకవేళ (srvs[j].బరువు == 0) {
                    srvs[j].బరువు = 1;
                }

                srvs[j].పోర్టు = (buf[i + 4] << 8) + buf[i + 5];

                ఒకవేళ (ngx_resolver_copy(r, &srvs[j].name, buf, &buf[i + 6],
                                      buf + n)
                    != NGX_OK)
                {
                    వెళ్ళు failed;
                }

                j++;
            }

            i += len;
        }

        ngx_క్రమీకరించు(srvs, nsrvs, పరిమాణం(ngx_resolver_srv_t),
                 ngx_resolver_cmp_srvs);

        ngx_resolver_free(r, rn->query);
        rn->query = ఖాళీ;

        ngx_వరుస_remove(&rn->వరుస);

        rn->valid = ngx_సమయం() + (r->valid ? r->valid : (సమయం_t) rn->ttl);
        rn->expire = ngx_సమయం() + r->expire;

        ngx_వరుస_చొప్పించు_తల(&r->srv_expire_వరుస, &rn->వరుస);

        తదుపరి = rn->నిరీక్షణ;
        rn->నిరీక్షణ = ఖాళీ;

        అయితే (తదుపరి) {
            ctx = తదుపరి;
            తదుపరి = ctx->తదుపరి;

            ngx_resolver_resolve_srv_పేర్లు(ctx, rn);
        }

        తిరిగి;
    }

    rn->nsrvs = 0;

    ఒకవేళ (cname) {

        /* CNAME only */

        ఒకవేళ (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            వెళ్ళు failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->సంవర్గమానం, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_వరుస_remove(&rn->వరుస);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.దత్తాంశం;

        rn->valid = ngx_సమయం() + (r->valid ? r->valid : (సమయం_t) rn->ttl);
        rn->expire = ngx_సమయం() + r->expire;

        ngx_వరుస_చొప్పించు_తల(&r->srv_expire_వరుస, &rn->వరుస);

        ngx_resolver_free(r, rn->query);
        rn->query = ఖాళీ;
#ఒకవేళ (NGX_HAVE_INET6)
        rn->query6 = ఖాళీ;
#అయితే_ముగింపు

        ctx = rn->నిరీక్షణ;
        rn->నిరీక్షణ = ఖాళీ;

        ఒకవేళ (ctx) {

            ఒకవేళ (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                చేయి {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    తదుపరి = ctx->తదుపరి;

                    ctx->handler(ctx);

                    ctx = తదుపరి;
                } అయితే (ctx);

                తిరిగి;
            }

            కొరకు (తదుపరి = ctx; తదుపరి; తదుపరి = తదుపరి->తదుపరి) {
                తదుపరి->కణుపు = ఖాళీ;
            }

            (శూన్య) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        తిరిగి;
    }

    ngx_log_error(r->log_level, r->సంవర్గమానం, 0, "no SRV type in DNS response");

    తిరిగి;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->సంవర్గమానం, 0, err);

    తిరిగి;

failed:

    /* unlock name mutex */

    తిరిగి;
}


స్థిర శూన్య
ngx_resolver_resolve_srv_పేర్లు(ngx_resolver_ctx_t *ctx, ngx_resolver_కణుపు_t *rn)
{
    ngx_uint_t                i;
    ngx_resolver_t           *r;
    ngx_resolver_ctx_t       *cctx;
    ngx_resolver_srv_name_t  *srvs;

    r = ctx->resolver;

    ctx->కణుపు = ఖాళీ;
    ctx->state = NGX_OK;
    ctx->valid = rn->valid;
    ctx->లెక్క = rn->nsrvs;

    srvs = ngx_resolver_calloc(r, rn->nsrvs * పరిమాణం(ngx_resolver_srv_name_t));
    ఒకవేళ (srvs == ఖాళీ) {
        వెళ్ళు failed;
    }

    ctx->srvs = srvs;
    ctx->nsrvs = rn->nsrvs;

    ఒకవేళ (ctx->event && ctx->event->timer_సమితి) {
        ngx_del_timer(ctx->event);
    }

    కొరకు (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
        srvs[i].name.దత్తాంశం = ngx_resolver_alloc(r, rn->u.srvs[i].name.len);
        ఒకవేళ (srvs[i].name.దత్తాంశం == ఖాళీ) {
            వెళ్ళు failed;
        }

        srvs[i].name.len = rn->u.srvs[i].name.len;
        ngx_memcpy(srvs[i].name.దత్తాంశం, rn->u.srvs[i].name.దత్తాంశం,
                   srvs[i].name.len);

        cctx = ngx_resolve_ప్రారంభం(r, ఖాళీ);
        ఒకవేళ (cctx == ఖాళీ) {
            వెళ్ళు failed;
        }

        cctx->name = srvs[i].name;
        cctx->handler = ngx_resolver_srv_పేర్లు_handler;
        cctx->దత్తాంశం = ctx;
        cctx->srvs = &srvs[i];
        cctx->timeout = ctx->timeout;

        srvs[i].ప్రాధాన్యత = rn->u.srvs[i].ప్రాధాన్యత;
        srvs[i].బరువు = rn->u.srvs[i].బరువు;
        srvs[i].పోర్టు = rn->u.srvs[i].పోర్టు;
        srvs[i].ctx = cctx;

        ఒకవేళ (ngx_resolve_name(cctx) == NGX_ERROR) {
            srvs[i].ctx = ఖాళీ;
            వెళ్ళు failed;
        }
    }

    తిరిగి;

failed:

    ctx->state = NGX_ERROR;
    ctx->valid = ngx_సమయం() + (r->valid ? r->valid : 10);

    ctx->handler(ctx);
}


స్థిర శూన్య
ngx_resolver_srv_పేర్లు_handler(ngx_resolver_ctx_t *cctx)
{
    ngx_uint_t                i;
    ngx_addr_t               *addrs;
    ngx_resolver_t           *r;
    ngx_sockaddr_t           *sockaddr;
    ngx_resolver_ctx_t       *ctx;
    ngx_resolver_srv_name_t  *srv;

    r = cctx->resolver;
    ctx = cctx->దత్తాంశం;
    srv = cctx->srvs;

    ctx->లెక్క--;
    ctx->async |= cctx->async;

    srv->ctx = ఖాళీ;
    srv->state = cctx->state;

    ఒకవేళ (cctx->naddrs) {

        ctx->valid = ngx_min(ctx->valid, cctx->valid);

        addrs = ngx_resolver_calloc(r, cctx->naddrs * పరిమాణం(ngx_addr_t));
        ఒకవేళ (addrs == ఖాళీ) {
            srv->state = NGX_ERROR;
            వెళ్ళు పూర్తి;
        }

        sockaddr = ngx_resolver_alloc(r, cctx->naddrs * పరిమాణం(ngx_sockaddr_t));
        ఒకవేళ (sockaddr == ఖాళీ) {
            ngx_resolver_free(r, addrs);
            srv->state = NGX_ERROR;
            వెళ్ళు పూర్తి;
        }

        కొరకు (i = 0; i < cctx->naddrs; i++) {
            addrs[i].sockaddr = &sockaddr[i].sockaddr;
            addrs[i].socklen = cctx->addrs[i].socklen;

            ngx_memcpy(&sockaddr[i], cctx->addrs[i].sockaddr,
                       addrs[i].socklen);

            ngx_inet_సమితి_పోర్టు(addrs[i].sockaddr, srv->పోర్టు);
        }

        srv->addrs = addrs;
        srv->naddrs = cctx->naddrs;
    }

పూర్తి:

    ngx_resolve_name_పూర్తి(cctx);

    ఒకవేళ (ctx->లెక్క == 0) {
        ngx_resolver_report_srv(r, ctx);
    }
}


స్థిర శూన్య
ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, కొలత_t n,
    ngx_uint_t ident, ngx_uint_t సంకేతం_2, ngx_uint_t nan)
{
    అక్షరం                 *err;
    కొలత_t                len;
    లోపల_addr_t             addr;
    int32_t               ttl;
    ngx_int_t             octet;
    ngx_str_t             name;
    ngx_uint_t            ముసుగు, type, class, qident, a, i, ప్రారంభం;
    ngx_వరుస_t          *expire_వరుస;
    ngx_rbtree_t         *వృక్షం;
    ngx_resolver_an_t    *an;
    ngx_resolver_ctx_t   *ctx, *తదుపరి;
    ngx_resolver_కణుపు_t  *rn;
#ఒకవేళ (NGX_HAVE_INET6)
    uint32_t              hash;
    ngx_int_t             digit;
    నిర్మాణం in6_addr       addr6;
#అయితే_ముగింపు

    ఒకవేళ (ngx_resolver_copy(r, &name, buf,
                          buf + పరిమాణం(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        తిరిగి;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->సంవర్గమానం, 0, "resolver qs:%V", &name);

    /* AF_INET */

    addr = 0;
    i = పరిమాణం(ngx_resolver_hdr_t);

    కొరకు (ముసుగు = 0; ముసుగు < 32; ముసుగు += 8) {
        len = buf[i++];

        octet = ngx_atoi(&buf[i], len);
        ఒకవేళ (octet == NGX_ERROR || octet > 255) {
            వెళ్ళు invalid_లోపల_addr_arpa;
        }

        addr += octet << ముసుగు;
        i += len;
    }

    ఒకవేళ (ngx_strcasecmp(&buf[i], (u_char *) "\7in-addr\4arpa") == 0) {
        i += పరిమాణం("\7in-addr\4arpa");

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        వృక్షం = &r->addr_rbtree;
        expire_వరుస = &r->addr_expire_వరుస;

        వెళ్ళు valid;
    }

invalid_లోపల_addr_arpa:

#ఒకవేళ (NGX_HAVE_INET6)

    i = పరిమాణం(ngx_resolver_hdr_t);

    కొరకు (octet = 15; octet >= 0; octet--) {
        ఒకవేళ (buf[i++] != '\1') {
            వెళ్ళు invalid_ip6_arpa;
        }

        digit = ngx_hextoi(&buf[i++], 1);
        ఒకవేళ (digit == NGX_ERROR) {
            వెళ్ళు invalid_ip6_arpa;
        }

        addr6.s6_addr[octet] = (u_char) digit;

        ఒకవేళ (buf[i++] != '\1') {
            వెళ్ళు invalid_ip6_arpa;
        }

        digit = ngx_hextoi(&buf[i++], 1);
        ఒకవేళ (digit == NGX_ERROR) {
            వెళ్ళు invalid_ip6_arpa;
        }

        addr6.s6_addr[octet] += (u_char) (digit * 16);
    }

    ఒకవేళ (ngx_strcasecmp(&buf[i], (u_char *) "\3ip6\4arpa") == 0) {
        i += పరిమాణం("\3ip6\4arpa");

        /* lock addr mutex */

        hash = ngx_చక్రీయ_తనిఖీ_32_short(addr6.s6_addr, 16);
        rn = ngx_resolver_lookup_addr6(r, &addr6, hash);

        వృక్షం = &r->addr6_rbtree;
        expire_వరుస = &r->addr6_expire_వరుస;

        వెళ్ళు valid;
    }

invalid_ip6_arpa:
#అయితే_ముగింపు

    ngx_log_error(r->log_level, r->సంవర్గమానం, 0,
                  "invalid in-addr.arpa or ip6.arpa name in DNS response");
    ngx_resolver_free(r, name.దత్తాంశం);
    తిరిగి;

valid:

    ఒకవేళ (rn == ఖాళీ || rn->query == ఖాళీ) {
        ngx_log_error(r->log_level, r->సంవర్గమానం, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.దత్తాంశం);
        వెళ్ళు failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    ఒకవేళ (ident != qident) {
        ngx_log_error(r->log_level, r->సంవర్గమానం, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.దత్తాంశం);
        వెళ్ళు failed;
    }

    ngx_resolver_free(r, name.దత్తాంశం);

    ఒకవేళ (సంకేతం_2 == 0 && nan == 0) {
        సంకేతం_2 = NGX_RESOLVE_NXDOMAIN;
    }

    ఒకవేళ (సంకేతం_2) {
        తదుపరి = rn->నిరీక్షణ;
        rn->నిరీక్షణ = ఖాళీ;

        ngx_వరుస_remove(&rn->వరుస);

        ngx_rbtree_delete(వృక్షం, &rn->కణుపు);

        /* unlock addr mutex */

        అయితే (తదుపరి) {
            ctx = తదుపరి;
            ctx->state = సంకేతం_2;
            ctx->valid = ngx_సమయం() + (r->valid ? r->valid : 10);
            తదుపరి = ctx->తదుపరి;

            ctx->handler(ctx);
        }

        ngx_resolver_free_కణుపు(r, rn);

        తిరిగి;
    }

    i += పరిమాణం(ngx_resolver_qs_t);

    కొరకు (a = 0; a < nan; a++) {

        ప్రారంభం = i;

        అయితే (i < n) {

            ఒకవేళ (buf[i] & 0xc0) {
                i += 2;
                వెళ్ళు దొరికింది;
            }

            ఒకవేళ (buf[i] == 0) {
                i++;
                వెళ్ళు test_నిడివి;
            }

            i += 1 + buf[i];
        }

        వెళ్ళు short_response;

    test_నిడివి:

        ఒకవేళ (i - ప్రారంభం < 2) {
            err = "invalid name in DNS response";
            వెళ్ళు invalid;
        }

    దొరికింది:

        ఒకవేళ (i + పరిమాణం(ngx_resolver_an_t) >= n) {
            వెళ్ళు short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        ఒకవేళ (class != 1) {
            ngx_log_error(r->log_level, r->సంవర్గమానం, 0,
                          "unexpected RR class %ui in DNS response", class);
            వెళ్ళు failed;
        }

        ఒకవేళ (ttl < 0) {
            ttl = 0;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->సంవర్గమానం, 0,
                      "resolver qt:%ui cl:%ui len:%uz",
                      type, class, len);

        i += పరిమాణం(ngx_resolver_an_t);

        ఎంపిక (type) {

        స్థితి NGX_RESOLVE_PTR:

            వెళ్ళు ptr;

        స్థితి NGX_RESOLVE_CNAME:

            ఆపు;

        అప్రమేయ:

            ngx_log_error(r->log_level, r->సంవర్గమానం, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->సంవర్గమానం, 0,
                  "no PTR type in DNS response");
    తిరిగి;

ptr:

    ఒకవేళ (ngx_resolver_copy(r, &name, buf, buf + i, buf + n) != NGX_OK) {
        వెళ్ళు failed;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->సంవర్గమానం, 0, "resolver an:%V", &name);

    ఒకవేళ (name.len != (కొలత_t) rn->nlen
        || ngx_strncmp(name.దత్తాంశం, rn->name, name.len) != 0)
    {
        ఒకవేళ (rn->nlen) {
            ngx_resolver_free(r, rn->name);
        }

        rn->nlen = (u_short) name.len;
        rn->name = name.దత్తాంశం;

        name.దత్తాంశం = ngx_resolver_dup(r, rn->name, name.len);
        ఒకవేళ (name.దత్తాంశం == ఖాళీ) {
            వెళ్ళు failed;
        }
    }

    ngx_వరుస_remove(&rn->వరుస);

    rn->valid = ngx_సమయం() + (r->valid ? r->valid : ttl);
    rn->expire = ngx_సమయం() + r->expire;

    ngx_వరుస_చొప్పించు_తల(expire_వరుస, &rn->వరుస);

    తదుపరి = rn->నిరీక్షణ;
    rn->నిరీక్షణ = ఖాళీ;

    /* unlock addr mutex */

    అయితే (తదుపరి) {
        ctx = తదుపరి;
        ctx->state = NGX_OK;
        ctx->valid = rn->valid;
        ctx->name = name;
        తదుపరి = ctx->తదుపరి;

        ctx->handler(ctx);
    }

    ngx_resolver_free(r, name.దత్తాంశం);

    తిరిగి;

short_response:

    err = "short DNS response";

invalid:

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->సంవర్గమానం, 0, err);

    తిరిగి;

failed:

    /* unlock addr mutex */

    తిరిగి;
}


స్థిర ngx_resolver_కణుపు_t *
ngx_resolver_lookup_name(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_కణుపు_t    *కణుపు, *sentinel;
    ngx_resolver_కణుపు_t  *rn;

    కణుపు = r->name_rbtree.మూలం;
    sentinel = r->name_rbtree.sentinel;

    అయితే (కణుపు != sentinel) {

        ఒకవేళ (hash < కణుపు->కీలకం) {
            కణుపు = కణుపు->ఎడమ;
            కొనసాగు;
        }

        ఒకవేళ (hash > కణుపు->కీలకం) {
            కణుపు = కణుపు->కుడి;
            కొనసాగు;
        }

        /* hash == node->key */

        rn = ngx_resolver_కణుపు(కణుపు);

        rc = ngx_memn2cmp(name->దత్తాంశం, rn->name, name->len, rn->nlen);

        ఒకవేళ (rc == 0) {
            తిరిగి rn;
        }

        కణుపు = (rc < 0) ? కణుపు->ఎడమ : కణుపు->కుడి;
    }

    /* not found */

    తిరిగి ఖాళీ;
}


స్థిర ngx_resolver_కణుపు_t *
ngx_resolver_lookup_srv(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_కణుపు_t    *కణుపు, *sentinel;
    ngx_resolver_కణుపు_t  *rn;

    కణుపు = r->srv_rbtree.మూలం;
    sentinel = r->srv_rbtree.sentinel;

    అయితే (కణుపు != sentinel) {

        ఒకవేళ (hash < కణుపు->కీలకం) {
            కణుపు = కణుపు->ఎడమ;
            కొనసాగు;
        }

        ఒకవేళ (hash > కణుపు->కీలకం) {
            కణుపు = కణుపు->కుడి;
            కొనసాగు;
        }

        /* hash == node->key */

        rn = ngx_resolver_కణుపు(కణుపు);

        rc = ngx_memn2cmp(name->దత్తాంశం, rn->name, name->len, rn->nlen);

        ఒకవేళ (rc == 0) {
            తిరిగి rn;
        }

        కణుపు = (rc < 0) ? కణుపు->ఎడమ : కణుపు->కుడి;
    }

    /* not found */

    తిరిగి ఖాళీ;
}


స్థిర ngx_resolver_కణుపు_t *
ngx_resolver_lookup_addr(ngx_resolver_t *r, లోపల_addr_t addr)
{
    ngx_rbtree_కణుపు_t  *కణుపు, *sentinel;

    కణుపు = r->addr_rbtree.మూలం;
    sentinel = r->addr_rbtree.sentinel;

    అయితే (కణుపు != sentinel) {

        ఒకవేళ (addr < కణుపు->కీలకం) {
            కణుపు = కణుపు->ఎడమ;
            కొనసాగు;
        }

        ఒకవేళ (addr > కణుపు->కీలకం) {
            కణుపు = కణుపు->కుడి;
            కొనసాగు;
        }

        /* addr == node->key */

        తిరిగి ngx_resolver_కణుపు(కణుపు);
    }

    /* not found */

    తిరిగి ఖాళీ;
}


#ఒకవేళ (NGX_HAVE_INET6)

స్థిర ngx_resolver_కణుపు_t *
ngx_resolver_lookup_addr6(ngx_resolver_t *r, నిర్మాణం in6_addr *addr,
    uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_కణుపు_t    *కణుపు, *sentinel;
    ngx_resolver_కణుపు_t  *rn;

    కణుపు = r->addr6_rbtree.మూలం;
    sentinel = r->addr6_rbtree.sentinel;

    అయితే (కణుపు != sentinel) {

        ఒకవేళ (hash < కణుపు->కీలకం) {
            కణుపు = కణుపు->ఎడమ;
            కొనసాగు;
        }

        ఒకవేళ (hash > కణుపు->కీలకం) {
            కణుపు = కణుపు->కుడి;
            కొనసాగు;
        }

        /* hash == node->key */

        rn = ngx_resolver_కణుపు(కణుపు);

        rc = ngx_memcmp(addr, &rn->addr6, 16);

        ఒకవేళ (rc == 0) {
            తిరిగి rn;
        }

        కణుపు = (rc < 0) ? కణుపు->ఎడమ : కణుపు->కుడి;
    }

    /* not found */

    తిరిగి ఖాళీ;
}

#అయితే_ముగింపు


స్థిర శూన్య
ngx_resolver_rbtree_చొప్పించు_విలువ(ngx_rbtree_కణుపు_t *తాత్కాలిక,
    ngx_rbtree_కణుపు_t *కణుపు, ngx_rbtree_కణుపు_t *sentinel)
{
    ngx_rbtree_కణుపు_t    **p;
    ngx_resolver_కణుపు_t   *rn, *rn_తాత్కాలిక;

    కొరకు ( ;; ) {

        ఒకవేళ (కణుపు->కీలకం < తాత్కాలిక->కీలకం) {

            p = &తాత్కాలిక->ఎడమ;

        } లేకపోతే ఒకవేళ (కణుపు->కీలకం > తాత్కాలిక->కీలకం) {

            p = &తాత్కాలిక->కుడి;

        } లేకపోతే { /* node->key == temp->key */

            rn = ngx_resolver_కణుపు(కణుపు);
            rn_తాత్కాలిక = ngx_resolver_కణుపు(తాత్కాలిక);

            p = (ngx_memn2cmp(rn->name, rn_తాత్కాలిక->name, rn->nlen, rn_తాత్కాలిక->nlen)
                 < 0) ? &తాత్కాలిక->ఎడమ : &తాత్కాలిక->కుడి;
        }

        ఒకవేళ (*p == sentinel) {
            ఆపు;
        }

        తాత్కాలిక = *p;
    }

    *p = కణుపు;
    కణుపు->మాతృ = తాత్కాలిక;
    కణుపు->ఎడమ = sentinel;
    కణుపు->కుడి = sentinel;
    ngx_rbt_red(కణుపు);
}


#ఒకవేళ (NGX_HAVE_INET6)

స్థిర శూన్య
ngx_resolver_rbtree_చొప్పించు_addr6_విలువ(ngx_rbtree_కణుపు_t *తాత్కాలిక,
    ngx_rbtree_కణుపు_t *కణుపు, ngx_rbtree_కణుపు_t *sentinel)
{
    ngx_rbtree_కణుపు_t    **p;
    ngx_resolver_కణుపు_t   *rn, *rn_తాత్కాలిక;

    కొరకు ( ;; ) {

        ఒకవేళ (కణుపు->కీలకం < తాత్కాలిక->కీలకం) {

            p = &తాత్కాలిక->ఎడమ;

        } లేకపోతే ఒకవేళ (కణుపు->కీలకం > తాత్కాలిక->కీలకం) {

            p = &తాత్కాలిక->కుడి;

        } లేకపోతే { /* node->key == temp->key */

            rn = ngx_resolver_కణుపు(కణుపు);
            rn_తాత్కాలిక = ngx_resolver_కణుపు(తాత్కాలిక);

            p = (ngx_memcmp(&rn->addr6, &rn_తాత్కాలిక->addr6, 16)
                 < 0) ? &తాత్కాలిక->ఎడమ : &తాత్కాలిక->కుడి;
        }

        ఒకవేళ (*p == sentinel) {
            ఆపు;
        }

        తాత్కాలిక = *p;
    }

    *p = కణుపు;
    కణుపు->మాతృ = తాత్కాలిక;
    కణుపు->ఎడమ = sentinel;
    కణుపు->కుడి = sentinel;
    ngx_rbt_red(కణుపు);
}

#అయితే_ముగింపు


స్థిర ngx_int_t
ngx_resolver_create_name_query(ngx_resolver_t *r, ngx_resolver_కణుపు_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    కొలత_t               len, nlen;
    ngx_uint_t           ident;
    ngx_resolver_qs_t   *qs;
    ngx_resolver_hdr_t  *query;

    nlen = name->len ? (1 + name->len + 1) : 1;

    len = పరిమాణం(ngx_resolver_hdr_t) + nlen + పరిమాణం(ngx_resolver_qs_t);

#ఒకవేళ (NGX_HAVE_INET6)
    p = ngx_resolver_alloc(r, len * (r->ipv4 + r->ipv6));
#లేకపోతే
    p = ngx_resolver_alloc(r, len);
#అయితే_ముగింపు
    ఒకవేళ (p == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    rn->qlen = (u_short) len;
    rn->query = p;

#ఒకవేళ (NGX_HAVE_INET6)
    ఒకవేళ (r->ipv6) {
        rn->query6 = r->ipv4 ? (p + len) : p;
    }
#అయితే_ముగింపు

    query = (ngx_resolver_hdr_t *) p;

    ఒకవేళ (r->ipv4) {
        ident = ngx_random();

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->సంవర్గమానం, 0,
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

    p += పరిమాణం(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    /* query type */
    qs->type_hi = 0; qs->type_lo = NGX_RESOLVE_A;

    /* IN query class */
    qs->class_hi = 0; qs->class_lo = 1;

    /* convert "www.example.com" to "\3www\7example\3com\0" */

    len = 0;
    p--;
    *p-- = '\0';

    ఒకవేళ (name->len == 0)  {
        తిరిగి NGX_DECLINED;
    }

    కొరకు (s = name->దత్తాంశం + name->len - 1; s >= name->దత్తాంశం; s--) {
        ఒకవేళ (*s != '.') {
            *p = *s;
            len++;

        } లేకపోతే {
            ఒకవేళ (len == 0 || len > 255) {
                తిరిగి NGX_DECLINED;
            }

            *p = (u_char) len;
            len = 0;
        }

        p--;
    }

    ఒకవేళ (len == 0 || len > 255) {
        తిరిగి NGX_DECLINED;
    }

    *p = (u_char) len;

#ఒకవేళ (NGX_HAVE_INET6)
    ఒకవేళ (!r->ipv6) {
        తిరిగి NGX_OK;
    }

    p = rn->query6;

    ఒకవేళ (r->ipv4) {
        ngx_memcpy(p, rn->query, rn->qlen);
    }

    query = (ngx_resolver_hdr_t *) p;

    ident = ngx_random();

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->సంవర్గమానం, 0,
                   "resolve: \"%V\" AAAA %i", name, ident & 0xffff);

    query->ident_hi = (u_char) ((ident >> 8) & 0xff);
    query->ident_lo = (u_char) (ident & 0xff);

    p += పరిమాణం(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    qs->type_lo = NGX_RESOLVE_AAAA;
#అయితే_ముగింపు

    తిరిగి NGX_OK;
}


స్థిర ngx_int_t
ngx_resolver_create_srv_query(ngx_resolver_t *r, ngx_resolver_కణుపు_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    కొలత_t               len, nlen;
    ngx_uint_t           ident;
    ngx_resolver_qs_t   *qs;
    ngx_resolver_hdr_t  *query;

    nlen = name->len ? (1 + name->len + 1) : 1;

    len = పరిమాణం(ngx_resolver_hdr_t) + nlen + పరిమాణం(ngx_resolver_qs_t);

    p = ngx_resolver_alloc(r, len);
    ఒకవేళ (p == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    rn->qlen = (u_short) len;
    rn->query = p;

    query = (ngx_resolver_hdr_t *) p;

    ident = ngx_random();

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->సంవర్గమానం, 0,
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

    p += పరిమాణం(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    /* query type */
    qs->type_hi = 0; qs->type_lo = NGX_RESOLVE_SRV;

    /* IN query class */
    qs->class_hi = 0; qs->class_lo = 1;

    /* converts "www.example.com" to "\3www\7example\3com\0" */

    len = 0;
    p--;
    *p-- = '\0';

    ఒకవేళ (name->len == 0)  {
        తిరిగి NGX_DECLINED;
    }

    కొరకు (s = name->దత్తాంశం + name->len - 1; s >= name->దత్తాంశం; s--) {
        ఒకవేళ (*s != '.') {
            *p = *s;
            len++;

        } లేకపోతే {
            ఒకవేళ (len == 0 || len > 255) {
                తిరిగి NGX_DECLINED;
            }

            *p = (u_char) len;
            len = 0;
        }

        p--;
    }

    ఒకవేళ (len == 0 || len > 255) {
        తిరిగి NGX_DECLINED;
    }

    *p = (u_char) len;

    తిరిగి NGX_OK;
}


స్థిర ngx_int_t
ngx_resolver_create_addr_query(ngx_resolver_t *r, ngx_resolver_కణుపు_t *rn,
    ngx_resolver_addr_t *addr)
{
    u_char               *p, *d;
    కొలత_t                len;
    లోపల_addr_t             inaddr;
    ngx_int_t             n;
    ngx_uint_t            ident;
    ngx_resolver_hdr_t   *query;
    నిర్మాణం sockaddr_లోపల   *జ్యా;
#ఒకవేళ (NGX_HAVE_INET6)
    నిర్మాణం sockaddr_in6  *sin6;
#అయితే_ముగింపు

    ఎంపిక (addr->sockaddr->sa_family) {

#ఒకవేళ (NGX_HAVE_INET6)
    స్థితి AF_INET6:
        len = పరిమాణం(ngx_resolver_hdr_t)
              + 64 + పరిమాణం(".ip6.arpa.") - 1
              + పరిమాణం(ngx_resolver_qs_t);

        ఆపు;
#అయితే_ముగింపు

    అప్రమేయ: /* AF_INET */
        len = పరిమాణం(ngx_resolver_hdr_t)
              + పరిమాణం(".255.255.255.255.in-addr.arpa.") - 1
              + పరిమాణం(ngx_resolver_qs_t);
    }

    p = ngx_resolver_alloc(r, len);
    ఒకవేళ (p == ఖాళీ) {
        తిరిగి NGX_ERROR;
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

    p += పరిమాణం(ngx_resolver_hdr_t);

    ఎంపిక (addr->sockaddr->sa_family) {

#ఒకవేళ (NGX_HAVE_INET6)
    స్థితి AF_INET6:
        sin6 = (నిర్మాణం sockaddr_in6 *) addr->sockaddr;

        కొరకు (n = 15; n >= 0; n--) {
            p = ngx_sprintf(p, "\1%xd\1%xd",
                            sin6->sin6_addr.s6_addr[n] & 0xf,
                            (sin6->sin6_addr.s6_addr[n] >> 4) & 0xf);
        }

        p = ngx_cpymem(p, "\3ip6\4arpa\0", 10);

        ఆపు;
#అయితే_ముగింపు

    అప్రమేయ: /* AF_INET */

        జ్యా = (నిర్మాణం sockaddr_లోపల *) addr->sockaddr;
        inaddr = ntohl(జ్యా->sin_addr.s_addr);

        కొరకు (n = 0; n < 32; n += 8) {
            d = ngx_sprintf(&p[1], "%ud", (inaddr >> n) & 0xff);
            *p = (u_char) (d - &p[1]);
            p = d;
        }

        p = ngx_cpymem(p, "\7in-addr\4arpa\0", 14);
    }

    /* query type "PTR", IN query class */
    p = ngx_cpymem(p, "\0\14\0\1", 4);

    rn->qlen = (u_short) (p - rn->query);

    తిరిగి NGX_OK;
}


స్థిర ngx_int_t
ngx_resolver_copy(ngx_resolver_t *r, ngx_str_t *name, u_char *buf, u_char *src,
    u_char *చివరి)
{
    అక్షరం        *err;
    u_char      *p, *dst;
    కొలత_t       len;
    ngx_uint_t   i, n;

    p = src;
    len = 0;

    /*
     * compression pointers allow to create endless loop, so we set limit;
     * 128 pointers should be enough to store 255-byte name
     */

    కొరకు (i = 0; i < 128; i++) {
        n = *p++;

        ఒకవేళ (n == 0) {
            వెళ్ళు పూర్తి;
        }

        ఒకవేళ (n & 0xc0) {
            ఒకవేళ ((n & 0xc0) != 0xc0) {
                err = "invalid label type in DNS response";
                వెళ్ళు invalid;
            }

            ఒకవేళ (p >= చివరి) {
                err = "name is out of DNS response";
                వెళ్ళు invalid;
            }

            n = ((n & 0x3f) << 8) + *p;
            p = &buf[n];

        } లేకపోతే {
            len += 1 + n;
            p = &p[n];
        }

        ఒకవేళ (p >= చివరి) {
            err = "name is out of DNS response";
            వెళ్ళు invalid;
        }
    }

    err = "compression pointers loop in DNS response";

invalid:

    ngx_log_error(r->log_level, r->సంవర్గమానం, 0, err);

    తిరిగి NGX_ERROR;

పూర్తి:

    ఒకవేళ (name == ఖాళీ) {
        తిరిగి NGX_OK;
    }

    ఒకవేళ (len == 0) {
        ngx_str_null(name);
        తిరిగి NGX_OK;
    }

    dst = ngx_resolver_alloc(r, len);
    ఒకవేళ (dst == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    name->దత్తాంశం = dst;

    కొరకు ( ;; ) {
        n = *src++;

        ఒకవేళ (n == 0) {
            name->len = dst - name->దత్తాంశం - 1;
            తిరిగి NGX_OK;
        }

        ఒకవేళ (n & 0xc0) {
            n = ((n & 0x3f) << 8) + *src;
            src = &buf[n];

        } లేకపోతే {
            ngx_strlow(dst, src, n);
            dst += n;
            src += n;
            *dst++ = '.';
        }
    }
}


స్థిర ngx_int_t
ngx_resolver_సమితి_timeout(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx)
{
    ఒకవేళ (ctx->event || ctx->timeout == 0) {
        తిరిగి NGX_OK;
    }

    ctx->event = ngx_resolver_calloc(r, పరిమాణం(ngx_event_t));
    ఒకవేళ (ctx->event == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    ctx->event->handler = ngx_resolver_timeout_handler;
    ctx->event->దత్తాంశం = ctx;
    ctx->event->సంవర్గమానం = r->సంవర్గమానం;
    ctx->event->cancelable = ctx->cancelable;
    ctx->ident = -1;

    ngx_కూడు_timer(ctx->event, ctx->timeout);

    తిరిగి NGX_OK;
}


స్థిర శూన్య
ngx_resolver_timeout_handler(ngx_event_t *ev)
{
    ngx_resolver_ctx_t  *ctx;

    ctx = ev->దత్తాంశం;

    ctx->state = NGX_RESOLVE_TIMEDOUT;

    ctx->handler(ctx);
}


స్థిర శూన్య
ngx_resolver_free_కణుపు(ngx_resolver_t *r, ngx_resolver_కణుపు_t *rn)
{
    ngx_uint_t  i;

    /* lock alloc mutex */

    ఒకవేళ (rn->query) {
        ngx_resolver_free_locked(r, rn->query);
    }

    ఒకవేళ (rn->name) {
        ngx_resolver_free_locked(r, rn->name);
    }

    ఒకవేళ (rn->cnlen) {
        ngx_resolver_free_locked(r, rn->u.cname);
    }

    ఒకవేళ (rn->naddrs > 1 && rn->naddrs != (u_short) -1) {
        ngx_resolver_free_locked(r, rn->u.addrs);
    }

#ఒకవేళ (NGX_HAVE_INET6)
    ఒకవేళ (rn->naddrs6 > 1 && rn->naddrs6 != (u_short) -1) {
        ngx_resolver_free_locked(r, rn->u6.addrs6);
    }
#అయితే_ముగింపు

    ఒకవేళ (rn->nsrvs) {
        కొరకు (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
            ఒకవేళ (rn->u.srvs[i].name.దత్తాంశం) {
                ngx_resolver_free_locked(r, rn->u.srvs[i].name.దత్తాంశం);
            }
        }

        ngx_resolver_free_locked(r, rn->u.srvs);
    }

    ngx_resolver_free_locked(r, rn);

    /* unlock alloc mutex */
}


స్థిర శూన్య *
ngx_resolver_alloc(ngx_resolver_t *r, కొలత_t కొలత)
{
    u_char  *p;

    /* lock alloc mutex */

    p = ngx_alloc(కొలత, r->సంవర్గమానం);

    /* unlock alloc mutex */

    తిరిగి p;
}


స్థిర శూన్య *
ngx_resolver_calloc(ngx_resolver_t *r, కొలత_t కొలత)
{
    u_char  *p;

    p = ngx_resolver_alloc(r, కొలత);

    ఒకవేళ (p) {
        ngx_memzero(p, కొలత);
    }

    తిరిగి p;
}


స్థిర శూన్య
ngx_resolver_free(ngx_resolver_t *r, శూన్య *p)
{
    /* lock alloc mutex */

    ngx_free(p);

    /* unlock alloc mutex */
}


స్థిర శూన్య
ngx_resolver_free_locked(ngx_resolver_t *r, శూన్య *p)
{
    ngx_free(p);
}


స్థిర శూన్య *
ngx_resolver_dup(ngx_resolver_t *r, శూన్య *src, కొలత_t కొలత)
{
    శూన్య  *dst;

    dst = ngx_resolver_alloc(r, కొలత);

    ఒకవేళ (dst == ఖాళీ) {
        తిరిగి dst;
    }

    ngx_memcpy(dst, src, కొలత);

    తిరిగి dst;
}


స్థిర ngx_resolver_addr_t *
ngx_resolver_export(ngx_resolver_t *r, ngx_resolver_కణుపు_t *rn,
    ngx_uint_t rotate)
{
    ngx_uint_t            d, i, j, n;
    లోపల_addr_t            *addr;
    ngx_sockaddr_t       *sockaddr;
    నిర్మాణం sockaddr_లోపల   *జ్యా;
    ngx_resolver_addr_t  *dst;
#ఒకవేళ (NGX_HAVE_INET6)
    నిర్మాణం in6_addr      *addr6;
    నిర్మాణం sockaddr_in6  *sin6;
#అయితే_ముగింపు

    n = rn->naddrs;
#ఒకవేళ (NGX_HAVE_INET6)
    n += rn->naddrs6;
#అయితే_ముగింపు

    dst = ngx_resolver_calloc(r, n * పరిమాణం(ngx_resolver_addr_t));
    ఒకవేళ (dst == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    sockaddr = ngx_resolver_calloc(r, n * పరిమాణం(ngx_sockaddr_t));
    ఒకవేళ (sockaddr == ఖాళీ) {
        ngx_resolver_free(r, dst);
        తిరిగి ఖాళీ;
    }

    i = 0;
    d = rotate ? ngx_random() % n : 0;

    ఒకవేళ (rn->naddrs) {
        j = rotate ? ngx_random() % rn->naddrs : 0;

        addr = (rn->naddrs == 1) ? &rn->u.addr : rn->u.addrs;

        చేయి {
            జ్యా = &sockaddr[d].sockaddr_లోపల;
            జ్యా->sin_family = AF_INET;
            జ్యా->sin_addr.s_addr = addr[j++];
            dst[d].sockaddr = (నిర్మాణం sockaddr *) జ్యా;
            dst[d++].socklen = పరిమాణం(నిర్మాణం sockaddr_లోపల);

            ఒకవేళ (d == n) {
                d = 0;
            }

            ఒకవేళ (j == (ngx_uint_t) rn->naddrs) {
                j = 0;
            }
        } అయితే (++i < (ngx_uint_t) rn->naddrs);
    }

#ఒకవేళ (NGX_HAVE_INET6)
    ఒకవేళ (rn->naddrs6) {
        j = rotate ? ngx_random() % rn->naddrs6 : 0;

        addr6 = (rn->naddrs6 == 1) ? &rn->u6.addr6 : rn->u6.addrs6;

        చేయి {
            sin6 = &sockaddr[d].sockaddr_in6;
            sin6->sin6_family = AF_INET6;
            ngx_memcpy(sin6->sin6_addr.s6_addr, addr6[j++].s6_addr, 16);
            dst[d].sockaddr = (నిర్మాణం sockaddr *) sin6;
            dst[d++].socklen = పరిమాణం(నిర్మాణం sockaddr_in6);

            ఒకవేళ (d == n) {
                d = 0;
            }

            ఒకవేళ (j == rn->naddrs6) {
                j = 0;
            }
        } అయితే (++i < n);
    }
#అయితే_ముగింపు

    తిరిగి dst;
}


స్థిర శూన్య
ngx_resolver_report_srv(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx)
{
    ngx_uint_t                naddrs, nsrvs, nw, i, j, k, l, m, n, w;
    ngx_resolver_addr_t      *addrs;
    ngx_resolver_srv_name_t  *srvs;

    srvs = ctx->srvs;
    nsrvs = ctx->nsrvs;

    naddrs = 0;

    కొరకు (i = 0; i < nsrvs; i++) {
        ఒకవేళ (srvs[i].state == NGX_ERROR) {
            ctx->state = NGX_ERROR;
            ctx->valid = ngx_సమయం() + (r->valid ? r->valid : 10);

            ctx->handler(ctx);
            తిరిగి;
        }

        naddrs += srvs[i].naddrs;
    }

    ఒకవేళ (naddrs == 0) {
        ctx->state = srvs[0].state;

        కొరకు (i = 0; i < nsrvs; i++) {
            ఒకవేళ (srvs[i].state == NGX_RESOLVE_NXDOMAIN) {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                ఆపు;
            }
        }

        ctx->valid = ngx_సమయం() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        తిరిగి;
    }

    addrs = ngx_resolver_calloc(r, naddrs * పరిమాణం(ngx_resolver_addr_t));
    ఒకవేళ (addrs == ఖాళీ) {
        ctx->state = NGX_ERROR;
        ctx->valid = ngx_సమయం() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        తిరిగి;
    }

    i = 0;
    n = 0;

    చేయి {
        nw = 0;

        కొరకు (j = i; j < nsrvs; j++) {
            ఒకవేళ (srvs[j].ప్రాధాన్యత != srvs[i].ప్రాధాన్యత) {
                ఆపు;
            }

            nw += srvs[j].naddrs * srvs[j].బరువు;
        }

        ఒకవేళ (nw == 0) {
            వెళ్ళు తదుపరి_srv;
        }

        w = ngx_random() % nw;

        కొరకు (k = i; k < j; k++) {
            ఒకవేళ (w < srvs[k].naddrs * srvs[k].బరువు) {
                ఆపు;
            }

            w -= srvs[k].naddrs * srvs[k].బరువు;
        }

        కొరకు (l = i; l < j; l++) {

            కొరకు (m = 0; m < srvs[k].naddrs; m++) {
                addrs[n].socklen = srvs[k].addrs[m].socklen;
                addrs[n].sockaddr = srvs[k].addrs[m].sockaddr;
                addrs[n].name = srvs[k].name;
                addrs[n].ప్రాధాన్యత = srvs[k].ప్రాధాన్యత;
                addrs[n].బరువు = srvs[k].బరువు;
                n++;
            }

            ఒకవేళ (++k == j) {
                k = i;
            }
        }

తదుపరి_srv:

        i = j;

    } అయితే (i < ctx->nsrvs);

    ctx->state = NGX_OK;
    ctx->addrs = addrs;
    ctx->naddrs = naddrs;

    ctx->handler(ctx);

    ngx_resolver_free(r, addrs);
}


అక్షరం *
ngx_resolver_strerror(ngx_int_t err)
{
    స్థిర అక్షరం *errors[] = {
        "Format error",     /* FORMERR */
        "Server failure",   /* SERVFAIL */
        "Host not found",   /* NXDOMAIN */
        "Unimplemented",    /* NOTIMP */
        "Operation refused" /* REFUSED */
    };

    ఒకవేళ (err > 0 && err < 6) {
        తిరిగి errors[err - 1];
    }

    ఒకవేళ (err == NGX_RESOLVE_TIMEDOUT) {
        తిరిగి "Operation timed out";
    }

    తిరిగి "Unknown error";
}


స్థిర u_char *
ngx_resolver_log_error(ngx_log_t *సంవర్గమానం, u_char *buf, కొలత_t len)
{
    u_char                     *p;
    ngx_resolver_connection_t  *rec;

    p = buf;

    ఒకవేళ (సంవర్గమానం->action) {
        p = ngx_snprintf(buf, len, " while %s", సంవర్గమానం->action);
        len -= p - buf;
    }

    rec = సంవర్గమానం->దత్తాంశం;

    ఒకవేళ (rec) {
        p = ngx_snprintf(p, len, ", resolver: %V", &rec->server);
    }

    తిరిగి p;
}


స్థిర ngx_int_t
ngx_udp_connect(ngx_resolver_connection_t *rec)
{
    పూర్ణాంకం                rc;
    ngx_int_t          event;
    ngx_event_t       *rev, *wev;
    ngx_socket_t       s;
    ngx_connection_t  *c;

    s = ngx_socket(rec->sockaddr->sa_family, SOCK_DGRAM, 0);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->సంవర్గమానం, 0, "UDP socket %d", s);

    ఒకవేళ (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->సంవర్గమానం, ngx_socket_errno,
                      ngx_socket_n " failed");
        తిరిగి NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->సంవర్గమానం);

    ఒకవేళ (c == ఖాళీ) {
        ఒకవేళ (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->సంవర్గమానం, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        తిరిగి NGX_ERROR;
    }

    ఒకవేళ (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->సంవర్గమానం, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        వెళ్ళు failed;
    }

    rev = c->పఠ;
    wev = c->లిఖ;

    rev->సంవర్గమానం = &rec->సంవర్గమానం;
    wev->సంవర్గమానం = &rec->సంవర్గమానం;

    rec->udp = c;

    c->సంఖ్య = ngx_atomic_fetch_కూడు(ngx_connection_counter, 1);

    c->ప్రారంభం_సమయం = ngx_current_msec;

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->సంవర్గమానం, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->సంఖ్య);

    rc = connect(s, rec->sockaddr, rec->socklen);

    ఒకవేళ (rc == -1) {
        ngx_log_error(NGX_LOG_CRIT, &rec->సంవర్గమానం, ngx_socket_errno,
                      "connect() failed");

        వెళ్ళు failed;
    }

    /* UDP sockets are always ready to write */
    wev->ready = 1;

    event = (ngx_event_flags & NGX_USE_CLEAR_EVENT) ?
                /* kqueue, epoll */                 NGX_CLEAR_EVENT:
                /* select, poll, /dev/poll */       NGX_LEVEL_EVENT;
                /* eventport event type has no meaning: oneshot only */

    ఒకవేళ (ngx_కూడు_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        వెళ్ళు failed;
    }

    తిరిగి NGX_OK;

failed:

    ngx_close_connection(c);
    rec->udp = ఖాళీ;

    తిరిగి NGX_ERROR;
}


స్థిర ngx_int_t
ngx_tcp_connect(ngx_resolver_connection_t *rec)
{
    పూర్ణాంకం                rc;
    ngx_int_t          event;
    ngx_err_t          err;
    ngx_uint_t         level;
    ngx_socket_t       s;
    ngx_event_t       *rev, *wev;
    ngx_connection_t  *c;

    s = ngx_socket(rec->sockaddr->sa_family, SOCK_ప్రవాహం, 0);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->సంవర్గమానం, 0, "TCP socket %d", s);

    ఒకవేళ (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->సంవర్గమానం, ngx_socket_errno,
                      ngx_socket_n " failed");
        తిరిగి NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->సంవర్గమానం);

    ఒకవేళ (c == ఖాళీ) {
        ఒకవేళ (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->సంవర్గమానం, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        తిరిగి NGX_ERROR;
    }

    ఒకవేళ (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->సంవర్గమానం, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        వెళ్ళు failed;
    }

    rev = c->పఠ;
    wev = c->లిఖ;

    rev->సంవర్గమానం = &rec->సంవర్గమానం;
    wev->సంవర్గమానం = &rec->సంవర్గమానం;

    rec->tcp = c;

    c->సంఖ్య = ngx_atomic_fetch_కూడు(ngx_connection_counter, 1);

    c->ప్రారంభం_సమయం = ngx_current_msec;

    ఒకవేళ (ngx_కూడు_conn) {
        ఒకవేళ (ngx_కూడు_conn(c) == NGX_ERROR) {
            వెళ్ళు failed;
        }
    }

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->సంవర్గమానం, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->సంఖ్య);

    rc = connect(s, rec->sockaddr, rec->socklen);

    ఒకవేళ (rc == -1) {
        err = ngx_socket_errno;


        ఒకవేళ (err != NGX_EINPROGRESS
#ఒకవేళ (NGX_WIN32)
            /* Winsock returns WSAEWOULDBLOCK (NGX_EAGAIN) */
            && err != NGX_EAGAIN
#అయితే_ముగింపు
            )
        {
            ఒకవేళ (err == NGX_ECONNREFUSED
#ఒకవేళ (NGX_LINUX)
                /*
                 * Linux returns EAGAIN instead of ECONNREFUSED
                 * for unix sockets if listen queue is full
                 */
                || err == NGX_EAGAIN
#అయితే_ముగింపు
                || err == NGX_ECONNRESET
                || err == NGX_ENETDOWN
                || err == NGX_ENETUNREACH
                || err == NGX_EHOSTDOWN
                || err == NGX_EHOSTUNREACH)
            {
                level = NGX_LOG_ERR;

            } లేకపోతే {
                level = NGX_LOG_CRIT;
            }

            ngx_log_error(level, &rec->సంవర్గమానం, err, "connect() to %V failed",
                          &rec->server);

            ngx_close_connection(c);
            rec->tcp = ఖాళీ;

            తిరిగి NGX_ERROR;
        }
    }

    ఒకవేళ (ngx_కూడు_conn) {
        ఒకవేళ (rc == -1) {

            /* NGX_EINPROGRESS */

            తిరిగి NGX_AGAIN;
        }

        ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->సంవర్గమానం, 0, "connected");

        wev->ready = 1;

        తిరిగి NGX_OK;
    }

    ఒకవేళ (ngx_event_flags & NGX_USE_CLEAR_EVENT) {

        /* kqueue */

        event = NGX_CLEAR_EVENT;

    } లేకపోతే {

        /* select, poll, /dev/poll */

        event = NGX_LEVEL_EVENT;
    }

    ఒకవేళ (ngx_కూడు_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        వెళ్ళు failed;
    }

    ఒకవేళ (rc == -1) {

        /* NGX_EINPROGRESS */

        ఒకవేళ (ngx_కూడు_event(wev, NGX_WRITE_EVENT, event) != NGX_OK) {
            వెళ్ళు failed;
        }

        తిరిగి NGX_AGAIN;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->సంవర్గమానం, 0, "connected");

    wev->ready = 1;

    తిరిగి NGX_OK;

failed:

    ngx_close_connection(c);
    rec->tcp = ఖాళీ;

    తిరిగి NGX_ERROR;
}


స్థిర ngx_int_t
ngx_resolver_cmp_srvs(స్థిరరాశి శూన్య *one, స్థిరరాశి శూన్య *two)
{
    ngx_int_t            p1, p2;
    ngx_resolver_srv_t  *మొదటి, *రెండవ;

    మొదటి = (ngx_resolver_srv_t *) one;
    రెండవ = (ngx_resolver_srv_t *) two;

    p1 = మొదటి->ప్రాధాన్యత;
    p2 = రెండవ->ప్రాధాన్యత;

    తిరిగి p1 - p2;
}
