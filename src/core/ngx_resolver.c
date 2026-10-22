
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_event.h>


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_RESOLVER_UDP_ਮਾਪ   4096

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_RESOLVER_TCP_RSIZE  (2 + 65535)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_RESOLVER_TCP_WSIZE  8192


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
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


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    u_char  type_hi;
    u_char  type_lo;
    u_char  class_hi;
    u_char  class_lo;
} ngx_resolver_qs_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    u_char  type_hi;
    u_char  type_lo;
    u_char  class_hi;
    u_char  class_lo;
    u_char  ttl[4];
    u_char  len_hi;
    u_char  len_lo;
} ngx_resolver_an_t;


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_resolver_ਗੰਢ(n)  ngx_rbtree_ਅੰਕੜੇ(n, ngx_resolver_ਗੰਢ_t, ਗੰਢ)


ਸਥਿਰ ngx_int_t ngx_udp_connect(ngx_resolver_connection_t *rec);
ਸਥਿਰ ngx_int_t ngx_tcp_connect(ngx_resolver_connection_t *rec);


ਸਥਿਰ ਖਾਲੀ ngx_resolver_cleanup(ਖਾਲੀ *ਅੰਕੜੇ);
ਸਥਿਰ ਖਾਲੀ ngx_resolver_cleanup_ਰੁੱਖ(ngx_resolver_t *r, ngx_rbtree_t *ਰੁੱਖ);
ਸਥਿਰ ngx_int_t ngx_resolve_name_locked(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx, ngx_str_t *name);
ਸਥਿਰ ਖਾਲੀ ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *ਰੁੱਖ,
    ngx_ਕਤਾਰ_t *ਕਤਾਰ);
ਸਥਿਰ ngx_int_t ngx_resolver_send_query(ngx_resolver_t *r,
    ngx_resolver_ਗੰਢ_t *rn);
ਸਥਿਰ ngx_int_t ngx_resolver_send_udp_query(ngx_resolver_t *r,
    ngx_resolver_connection_t *rec, u_char *query, u_short qlen);
ਸਥਿਰ ngx_int_t ngx_resolver_send_tcp_query(ngx_resolver_t *r,
    ngx_resolver_connection_t *rec, u_char *query, u_short qlen);
ਸਥਿਰ ngx_int_t ngx_resolver_create_name_query(ngx_resolver_t *r,
    ngx_resolver_ਗੰਢ_t *rn, ngx_str_t *name);
ਸਥਿਰ ngx_int_t ngx_resolver_create_srv_query(ngx_resolver_t *r,
    ngx_resolver_ਗੰਢ_t *rn, ngx_str_t *name);
ਸਥਿਰ ngx_int_t ngx_resolver_create_addr_query(ngx_resolver_t *r,
    ngx_resolver_ਗੰਢ_t *rn, ngx_resolver_addr_t *addr);
ਸਥਿਰ ਖਾਲੀ ngx_resolver_resend_handler(ngx_event_t *ev);
ਸਥਿਰ ਸਮਾਂ_t ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *ਰੁੱਖ,
    ngx_ਕਤਾਰ_t *ਕਤਾਰ);
ਸਥਿਰ ngx_uint_t ngx_resolver_resend_ਖਾਲੀ_ਥਾਵਾਂ(ngx_resolver_t *r);
ਸਥਿਰ ਖਾਲੀ ngx_resolver_udp_read(ngx_event_t *rev);
ਸਥਿਰ ਖਾਲੀ ngx_resolver_tcp_write(ngx_event_t *wev);
ਸਥਿਰ ਖਾਲੀ ngx_resolver_tcp_read(ngx_event_t *rev);
ਸਥਿਰ ਖਾਲੀ ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf,
    ਮਾਪ_t n, ngx_uint_t tcp);
ਸਥਿਰ ਖਾਲੀ ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, ਮਾਪ_t n,
    ngx_uint_t ident, ngx_uint_t ਕੋਡ, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans);
ਸਥਿਰ ਖਾਲੀ ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, ਮਾਪ_t n,
    ngx_uint_t ident, ngx_uint_t ਕੋਡ, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans);
ਸਥਿਰ ਖਾਲੀ ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, ਮਾਪ_t n,
    ngx_uint_t ident, ngx_uint_t ਕੋਡ, ngx_uint_t nan);
ਸਥਿਰ ngx_resolver_ਗੰਢ_t *ngx_resolver_lookup_name(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
ਸਥਿਰ ngx_resolver_ਗੰਢ_t *ngx_resolver_lookup_srv(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
ਸਥਿਰ ngx_resolver_ਗੰਢ_t *ngx_resolver_lookup_addr(ngx_resolver_t *r,
    ਵਿੱਚ_addr_t addr);
ਸਥਿਰ ਖਾਲੀ ngx_resolver_rbtree_ਪਾਓ_ਮੁੱਲ(ngx_rbtree_ਗੰਢ_t *ਆਰਜ਼ੀ,
    ngx_rbtree_ਗੰਢ_t *ਗੰਢ, ngx_rbtree_ਗੰਢ_t *sentinel);
ਸਥਿਰ ngx_int_t ngx_resolver_copy(ngx_resolver_t *r, ngx_str_t *name,
    u_char *buf, u_char *src, u_char *ਆਖਰੀ);
ਸਥਿਰ ngx_int_t ngx_resolver_ਸੈੱਟ_timeout(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx);
ਸਥਿਰ ਖਾਲੀ ngx_resolver_timeout_handler(ngx_event_t *ev);
ਸਥਿਰ ਖਾਲੀ ngx_resolver_free_ਗੰਢ(ngx_resolver_t *r, ngx_resolver_ਗੰਢ_t *rn);
ਸਥਿਰ ਖਾਲੀ *ngx_resolver_alloc(ngx_resolver_t *r, ਮਾਪ_t ਮਾਪ);
ਸਥਿਰ ਖਾਲੀ *ngx_resolver_calloc(ngx_resolver_t *r, ਮਾਪ_t ਮਾਪ);
ਸਥਿਰ ਖਾਲੀ ngx_resolver_free(ngx_resolver_t *r, ਖਾਲੀ *p);
ਸਥਿਰ ਖਾਲੀ ngx_resolver_free_locked(ngx_resolver_t *r, ਖਾਲੀ *p);
ਸਥਿਰ ਖਾਲੀ *ngx_resolver_dup(ngx_resolver_t *r, ਖਾਲੀ *src, ਮਾਪ_t ਮਾਪ);
ਸਥਿਰ ngx_resolver_addr_t *ngx_resolver_export(ngx_resolver_t *r,
    ngx_resolver_ਗੰਢ_t *rn, ngx_uint_t rotate);
ਸਥਿਰ ਖਾਲੀ ngx_resolver_report_srv(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx);
ਸਥਿਰ u_char *ngx_resolver_log_error(ngx_log_t *ਲਘੂਗਣਕ, u_char *buf, ਮਾਪ_t len);
ਸਥਿਰ ਖਾਲੀ ngx_resolver_resolve_srv_ਨਾਂ(ngx_resolver_ctx_t *ctx,
    ngx_resolver_ਗੰਢ_t *rn);
ਸਥਿਰ ਖਾਲੀ ngx_resolver_srv_ਨਾਂ_handler(ngx_resolver_ctx_t *ctx);
ਸਥਿਰ ngx_int_t ngx_resolver_cmp_srvs(ਅਚਲ ਖਾਲੀ *one, ਅਚਲ ਖਾਲੀ *two);

#ਜੇ (NGX_HAVE_INET6)
ਸਥਿਰ ਖਾਲੀ ngx_resolver_rbtree_ਪਾਓ_addr6_ਮੁੱਲ(ngx_rbtree_ਗੰਢ_t *ਆਰਜ਼ੀ,
    ngx_rbtree_ਗੰਢ_t *ਗੰਢ, ngx_rbtree_ਗੰਢ_t *sentinel);
ਸਥਿਰ ngx_resolver_ਗੰਢ_t *ngx_resolver_lookup_addr6(ngx_resolver_t *r,
    ਬਣਤਰ in6_addr *addr, uint32_t hash);
#ਜੇ_ਅੰਤ


ngx_resolver_t *
ngx_resolver_create(ngx_conf_t *cf, ngx_str_t *ਨਾਂ, ngx_uint_t n)
{
    ngx_str_t                   s;
    ngx_url_t                   u;
    ngx_uint_t                  i, j;
    ngx_resolver_t             *r;
    ngx_pool_cleanup_t         *cln;
    ngx_resolver_connection_t  *rec;

    r = ngx_pcalloc(cf->pool, ਆਕਾਰ(ngx_resolver_t));
    ਜੇ (r == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    r->event = ngx_pcalloc(cf->pool, ਆਕਾਰ(ngx_event_t));
    ਜੇ (r->event == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    cln = ngx_pool_cleanup_ਜੋੜੋ(cf->pool, 0);
    ਜੇ (cln == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    cln->handler = ngx_resolver_cleanup;
    cln->ਅੰਕੜੇ = r;

    r->ipv4 = 1;

    ngx_rbtree_init(&r->name_rbtree, &r->name_sentinel,
                    ngx_resolver_rbtree_ਪਾਓ_ਮੁੱਲ);

    ngx_rbtree_init(&r->srv_rbtree, &r->srv_sentinel,
                    ngx_resolver_rbtree_ਪਾਓ_ਮੁੱਲ);

    ngx_rbtree_init(&r->addr_rbtree, &r->addr_sentinel,
                    ngx_rbtree_ਪਾਓ_ਮੁੱਲ);

    ngx_ਕਤਾਰ_init(&r->name_resend_ਕਤਾਰ);
    ngx_ਕਤਾਰ_init(&r->srv_resend_ਕਤਾਰ);
    ngx_ਕਤਾਰ_init(&r->addr_resend_ਕਤਾਰ);

    ngx_ਕਤਾਰ_init(&r->name_expire_ਕਤਾਰ);
    ngx_ਕਤਾਰ_init(&r->srv_expire_ਕਤਾਰ);
    ngx_ਕਤਾਰ_init(&r->addr_expire_ਕਤਾਰ);

#ਜੇ (NGX_HAVE_INET6)
    r->ipv6 = 1;

    ngx_rbtree_init(&r->addr6_rbtree, &r->addr6_sentinel,
                    ngx_resolver_rbtree_ਪਾਓ_addr6_ਮੁੱਲ);

    ngx_ਕਤਾਰ_init(&r->addr6_resend_ਕਤਾਰ);

    ngx_ਕਤਾਰ_init(&r->addr6_expire_ਕਤਾਰ);
#ਜੇ_ਅੰਤ

    r->event->handler = ngx_resolver_resend_handler;
    r->event->ਅੰਕੜੇ = r;
    r->event->ਲਘੂਗਣਕ = &cf->ਚੱਕਰ->new_log;
    r->event->cancelable = 1;
    r->ident = -1;

    r->resend_timeout = 5;
    r->tcp_timeout = 5;
    r->expire = 30;
    r->valid = 0;

    r->ਲਘੂਗਣਕ = &cf->ਚੱਕਰ->new_log;
    r->log_level = NGX_LOG_ERR;

    ਜੇ (n) {
        ਜੇ (ngx_array_init(&r->connections, cf->pool, n,
                           ਆਕਾਰ(ngx_resolver_connection_t))
            != NGX_OK)
        {
            ਵਾਪਸ ਨਿਰਾ;
        }
    }

    ਲਈ (i = 0; i < n; i++) {
        ਜੇ (ngx_strncmp(ਨਾਂ[i].ਅੰਕੜੇ, "valid=", 6) == 0) {
            s.len = ਨਾਂ[i].len - 6;
            s.ਅੰਕੜੇ = ਨਾਂ[i].ਅੰਕੜੇ + 6;

            r->valid = ngx_parse_ਸਮਾਂ(&s, 1);

            ਜੇ (r->valid == (ਸਮਾਂ_t) NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &ਨਾਂ[i]);
                ਵਾਪਸ ਨਿਰਾ;
            }

            ਜਾਰੀ;
        }

#ਜੇ (NGX_HAVE_INET6)
        ਜੇ (ngx_strncmp(ਨਾਂ[i].ਅੰਕੜੇ, "ipv4=", 5) == 0) {

            ਜੇ (ngx_strcmp(&ਨਾਂ[i].ਅੰਕੜੇ[5], "on") == 0) {
                r->ipv4 = 1;

            } ਨਹੀਂਤਾਂ ਜੇ (ngx_strcmp(&ਨਾਂ[i].ਅੰਕੜੇ[5], "off") == 0) {
                r->ipv4 = 0;

            } ਨਹੀਂਤਾਂ {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &ਨਾਂ[i]);
                ਵਾਪਸ ਨਿਰਾ;
            }

            ਜਾਰੀ;
        }

        ਜੇ (ngx_strncmp(ਨਾਂ[i].ਅੰਕੜੇ, "ipv6=", 5) == 0) {

            ਜੇ (ngx_strcmp(&ਨਾਂ[i].ਅੰਕੜੇ[5], "on") == 0) {
                r->ipv6 = 1;

            } ਨਹੀਂਤਾਂ ਜੇ (ngx_strcmp(&ਨਾਂ[i].ਅੰਕੜੇ[5], "off") == 0) {
                r->ipv6 = 0;

            } ਨਹੀਂਤਾਂ {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &ਨਾਂ[i]);
                ਵਾਪਸ ਨਿਰਾ;
            }

            ਜਾਰੀ;
        }
#ਜੇ_ਅੰਤ

        ngx_memzero(&u, ਆਕਾਰ(ngx_url_t));

        u.url = ਨਾਂ[i];
        u.default_ਪੋਰਟ = 53;

        ਜੇ (ngx_parse_url(cf->pool, &u) != NGX_OK) {
            ਜੇ (u.err) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "%s in resolver \"%V\"",
                                   u.err, &u.url);
            }

            ਵਾਪਸ ਨਿਰਾ;
        }

        rec = ngx_array_ਧੱਕੋ_n(&r->connections, u.naddrs);
        ਜੇ (rec == ਨਿਰਾ) {
            ਵਾਪਸ ਨਿਰਾ;
        }

        ngx_memzero(rec, u.naddrs * ਆਕਾਰ(ngx_resolver_connection_t));

        ਲਈ (j = 0; j < u.naddrs; j++) {
            rec[j].sockaddr = u.addrs[j].sockaddr;
            rec[j].socklen = u.addrs[j].socklen;
            rec[j].server = u.addrs[j].name;
            rec[j].resolver = r;
        }
    }

#ਜੇ (NGX_HAVE_INET6)
    ਜੇ (r->ipv4 + r->ipv6 == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"ipv4\" and \"ipv6\" cannot both be \"off\"");
        ਵਾਪਸ ਨਿਰਾ;
    }
#ਜੇ_ਅੰਤ

    ਜੇ (n && r->connections.nelts == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "no name servers defined");
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਵਾਪਸ r;
}


ਸਥਿਰ ਖਾਲੀ
ngx_resolver_cleanup(ਖਾਲੀ *ਅੰਕੜੇ)
{
    ngx_resolver_t  *r = ਅੰਕੜੇ;

    ngx_uint_t                  i;
    ngx_resolver_connection_t  *rec;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_ਚੱਕਰ->ਲਘੂਗਣਕ, 0, "cleanup resolver");

    ngx_resolver_cleanup_ਰੁੱਖ(r, &r->name_rbtree);

    ngx_resolver_cleanup_ਰੁੱਖ(r, &r->srv_rbtree);

    ngx_resolver_cleanup_ਰੁੱਖ(r, &r->addr_rbtree);

#ਜੇ (NGX_HAVE_INET6)
    ngx_resolver_cleanup_ਰੁੱਖ(r, &r->addr6_rbtree);
#ਜੇ_ਅੰਤ

    ਜੇ (r->event->timer_ਸੈੱਟ) {
        ngx_del_timer(r->event);
    }

    rec = r->connections.elts;

    ਲਈ (i = 0; i < r->connections.nelts; i++) {
        ਜੇ (rec[i].udp) {
            ngx_close_connection(rec[i].udp);
        }

        ਜੇ (rec[i].tcp) {
            ngx_close_connection(rec[i].tcp);
        }

        ਜੇ (rec[i].read_buf) {
            ngx_resolver_free(r, rec[i].read_buf->ਸ਼ੁਰੂ);
            ngx_resolver_free(r, rec[i].read_buf);
        }

        ਜੇ (rec[i].write_buf) {
            ngx_resolver_free(r, rec[i].write_buf->ਸ਼ੁਰੂ);
            ngx_resolver_free(r, rec[i].write_buf);
        }
    }
}


ਸਥਿਰ ਖਾਲੀ
ngx_resolver_cleanup_ਰੁੱਖ(ngx_resolver_t *r, ngx_rbtree_t *ਰੁੱਖ)
{
    ngx_resolver_ctx_t   *ctx, *ਅਗਲਾ;
    ngx_resolver_ਗੰਢ_t  *rn;

    ਜਦੋਂ (ਰੁੱਖ->ਜੜ੍ਹ != ਰੁੱਖ->sentinel) {

        rn = ngx_resolver_ਗੰਢ(ngx_rbtree_min(ਰੁੱਖ->ਜੜ੍ਹ, ਰੁੱਖ->sentinel));

        ngx_ਕਤਾਰ_remove(&rn->ਕਤਾਰ);

        ਲਈ (ctx = rn->ਉਡੀਕ; ctx; ctx = ਅਗਲਾ) {
            ਅਗਲਾ = ctx->ਅਗਲਾ;

            ਜੇ (ctx->event) {
                ਜੇ (ctx->event->timer_ਸੈੱਟ) {
                    ngx_del_timer(ctx->event);
                }

                ngx_resolver_free(r, ctx->event);
            }

            ngx_resolver_free(r, ctx);
        }

        ngx_rbtree_delete(ਰੁੱਖ, &rn->ਗੰਢ);

        ngx_resolver_free_ਗੰਢ(r, rn);
    }
}


ngx_resolver_ctx_t *
ngx_resolve_ਸ਼ੁਰੂ(ngx_resolver_t *r, ngx_resolver_ctx_t *ਆਰਜ਼ੀ)
{
    ਵਿੱਚ_addr_t            addr;
    ngx_resolver_ctx_t  *ctx;

    ਜੇ (ਆਰਜ਼ੀ) {
        addr = ngx_inet_addr(ਆਰਜ਼ੀ->name.ਅੰਕੜੇ, ਆਰਜ਼ੀ->name.len);

        ਜੇ (addr != INADDR_NONE) {
            ਆਰਜ਼ੀ->resolver = r;
            ਆਰਜ਼ੀ->state = NGX_OK;
            ਆਰਜ਼ੀ->naddrs = 1;
            ਆਰਜ਼ੀ->addrs = &ਆਰਜ਼ੀ->addr;
            ਆਰਜ਼ੀ->addr.sockaddr = (ਬਣਤਰ sockaddr *) &ਆਰਜ਼ੀ->ਜੀਵਾ;
            ਆਰਜ਼ੀ->addr.socklen = ਆਕਾਰ(ਬਣਤਰ sockaddr_ਵਿੱਚ);
            ngx_memzero(&ਆਰਜ਼ੀ->ਜੀਵਾ, ਆਕਾਰ(ਬਣਤਰ sockaddr_ਵਿੱਚ));
            ਆਰਜ਼ੀ->ਜੀਵਾ.sin_family = AF_INET;
            ਆਰਜ਼ੀ->ਜੀਵਾ.sin_addr.s_addr = addr;
            ਆਰਜ਼ੀ->ਤੇਜ਼ = 1;

            ਵਾਪਸ ਆਰਜ਼ੀ;
        }
    }

    ਜੇ (r->connections.nelts == 0) {
        ਵਾਪਸ NGX_NO_RESOLVER;
    }

    ctx = ngx_resolver_calloc(r, ਆਕਾਰ(ngx_resolver_ctx_t));

    ਜੇ (ctx) {
        ctx->resolver = r;
    }

    ਵਾਪਸ ctx;
}


ngx_int_t
ngx_resolve_name(ngx_resolver_ctx_t *ctx)
{
    ਮਾਪ_t           slen;
    ngx_int_t        rc;
    ngx_str_t        name;
    ngx_resolver_t  *r;

    r = ctx->resolver;

    ਜੇ (ctx->name.len > 0 && ctx->name.ਅੰਕੜੇ[ctx->name.len - 1] == '.') {
        ctx->name.len--;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->ਲਘੂਗਣਕ, 0,
                   "resolve: \"%V\"", &ctx->name);

    ਜੇ (ctx->ਤੇਜ਼) {
        ctx->handler(ctx);
        ਵਾਪਸ NGX_OK;
    }

    ਜੇ (ctx->service.len) {
        slen = ctx->service.len;

        ਜੇ (ngx_strlchr(ctx->service.ਅੰਕੜੇ,
                        ctx->service.ਅੰਕੜੇ + ctx->service.len, '.')
            == ਨਿਰਾ)
        {
            slen += ਆਕਾਰ("_._tcp") - 1;
        }

        name.len = slen + 1 + ctx->name.len;

        name.ਅੰਕੜੇ = ngx_resolver_alloc(r, name.len);
        ਜੇ (name.ਅੰਕੜੇ == ਨਿਰਾ) {
            ਜਾਓ failed;
        }

        ਜੇ (slen == ctx->service.len) {
            ngx_sprintf(name.ਅੰਕੜੇ, "%V.%V", &ctx->service, &ctx->name);

        } ਨਹੀਂਤਾਂ {
            ngx_sprintf(name.ਅੰਕੜੇ, "_%V._tcp.%V", &ctx->service, &ctx->name);
        }

        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &name);

        ngx_resolver_free(r, name.ਅੰਕੜੇ);

    } ਨਹੀਂਤਾਂ {
        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &ctx->name);
    }

    ਜੇ (rc == NGX_OK) {
        ਵਾਪਸ NGX_OK;
    }

    /* unlock name mutex */

    ਜੇ (rc == NGX_AGAIN) {
        ਵਾਪਸ NGX_OK;
    }

    /* NGX_ERROR */

    ਜੇ (ctx->event) {
        ngx_resolver_free(r, ctx->event);
    }

failed:

    ngx_resolver_free(r, ctx);

    ਵਾਪਸ NGX_ERROR;
}


ਖਾਲੀ
ngx_resolve_name_ਪੂਰਾ(ngx_resolver_ctx_t *ctx)
{
    ngx_uint_t            i;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_ਗੰਢ_t  *rn;

    r = ctx->resolver;

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->ਲਘੂਗਣਕ, 0,
                   "resolve name done: %i", ctx->state);

    ਜੇ (ctx->ਤੇਜ਼) {
        ਵਾਪਸ;
    }

    ਜੇ (ctx->event && ctx->event->timer_ਸੈੱਟ) {
        ngx_del_timer(ctx->event);
    }

    /* lock name mutex */

    ਜੇ (ctx->nsrvs) {
        ਲਈ (i = 0; i < ctx->nsrvs; i++) {
            ਜੇ (ctx->srvs[i].ctx) {
                ngx_resolve_name_ਪੂਰਾ(ctx->srvs[i].ctx);
            }

            ਜੇ (ctx->srvs[i].addrs) {
                ngx_resolver_free(r, ctx->srvs[i].addrs->sockaddr);
                ngx_resolver_free(r, ctx->srvs[i].addrs);
            }

            ngx_resolver_free(r, ctx->srvs[i].name.ਅੰਕੜੇ);
        }

        ngx_resolver_free(r, ctx->srvs);
    }

    ਜੇ (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->ਗੰਢ;

        ਜੇ (rn) {
            p = &rn->ਉਡੀਕ;
            w = rn->ਉਡੀਕ;

            ਜਦੋਂ (w) {
                ਜੇ (w == ctx) {
                    *p = w->ਅਗਲਾ;

                    ਜਾਓ ਪੂਰਾ;
                }

                p = &w->ਅਗਲਾ;
                w = w->ਅਗਲਾ;
            }

            ngx_log_error(NGX_LOG_ALERT, r->ਲਘੂਗਣਕ, 0,
                          "could not cancel %V resolving", &ctx->name);
        }
    }

ਪੂਰਾ:

    ਜੇ (ctx->service.len) {
        ngx_resolver_expire(r, &r->srv_rbtree, &r->srv_expire_ਕਤਾਰ);

    } ਨਹੀਂਤਾਂ {
        ngx_resolver_expire(r, &r->name_rbtree, &r->name_expire_ਕਤਾਰ);
    }

    /* unlock name mutex */

    /* lock alloc mutex */

    ਜੇ (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    ਜੇ (r->event->timer_ਸੈੱਟ && ngx_resolver_resend_ਖਾਲੀ_ਥਾਵਾਂ(r)) {
        ngx_del_timer(r->event);
    }
}


ਸਥਿਰ ngx_int_t
ngx_resolve_name_locked(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx,
    ngx_str_t *name)
{
    uint32_t              hash;
    ngx_int_t             rc;
    ngx_str_t             cname;
    ngx_uint_t            i, naddrs;
    ngx_ਕਤਾਰ_t          *resend_ਕਤਾਰ, *expire_ਕਤਾਰ;
    ngx_rbtree_t         *ਰੁੱਖ;
    ngx_resolver_ctx_t   *ਅਗਲਾ, *ਆਖਰੀ;
    ngx_resolver_addr_t  *addrs;
    ngx_resolver_ਗੰਢ_t  *rn;

    ngx_strlow(name->ਅੰਕੜੇ, name->ਅੰਕੜੇ, name->len);

    hash = ngx_ਚੱਕਰੀ_ਜਾਂਚ_32_short(name->ਅੰਕੜੇ, name->len);

    ਜੇ (ctx->service.len) {
        rn = ngx_resolver_lookup_srv(r, name, hash);

        ਰੁੱਖ = &r->srv_rbtree;
        resend_ਕਤਾਰ = &r->srv_resend_ਕਤਾਰ;
        expire_ਕਤਾਰ = &r->srv_expire_ਕਤਾਰ;

    } ਨਹੀਂਤਾਂ {
        rn = ngx_resolver_lookup_name(r, name, hash);

        ਰੁੱਖ = &r->name_rbtree;
        resend_ਕਤਾਰ = &r->name_resend_ਕਤਾਰ;
        expire_ਕਤਾਰ = &r->name_expire_ਕਤਾਰ;
    }

    ਜੇ (rn) {

        /* ctx can be a list after NGX_RESOLVE_CNAME */
        ਲਈ (ਆਖਰੀ = ctx; ਆਖਰੀ->ਅਗਲਾ; ਆਖਰੀ = ਆਖਰੀ->ਅਗਲਾ);

        ਜੇ (rn->valid >= ngx_ਸਮਾਂ()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->ਲਘੂਗਣਕ, 0, "resolve cached");

            ngx_ਕਤਾਰ_remove(&rn->ਕਤਾਰ);

            rn->expire = ngx_ਸਮਾਂ() + r->expire;

            ngx_ਕਤਾਰ_ਪਾਓ_ਸਿਰ(expire_ਕਤਾਰ, &rn->ਕਤਾਰ);

            naddrs = (rn->naddrs == (u_short) -1) ? 0 : rn->naddrs;
#ਜੇ (NGX_HAVE_INET6)
            naddrs += (rn->naddrs6 == (u_short) -1) ? 0 : rn->naddrs6;
#ਜੇ_ਅੰਤ

            ਜੇ (naddrs) {

                ਜੇ (naddrs == 1 && rn->naddrs == 1) {
                    addrs = ਨਿਰਾ;

                } ਨਹੀਂਤਾਂ {
                    addrs = ngx_resolver_export(r, rn, 1);
                    ਜੇ (addrs == ਨਿਰਾ) {
                        ਵਾਪਸ NGX_ERROR;
                    }
                }

                ਆਖਰੀ->ਅਗਲਾ = rn->ਉਡੀਕ;
                rn->ਉਡੀਕ = ਨਿਰਾ;

                /* unlock name mutex */

                ਕਰੋ {
                    ctx->state = NGX_OK;
                    ctx->valid = rn->valid;
                    ctx->naddrs = naddrs;

                    ਜੇ (addrs == ਨਿਰਾ) {
                        ctx->addrs = &ctx->addr;
                        ctx->addr.sockaddr = (ਬਣਤਰ sockaddr *) &ctx->ਜੀਵਾ;
                        ctx->addr.socklen = ਆਕਾਰ(ਬਣਤਰ sockaddr_ਵਿੱਚ);
                        ngx_memzero(&ctx->ਜੀਵਾ, ਆਕਾਰ(ਬਣਤਰ sockaddr_ਵਿੱਚ));
                        ctx->ਜੀਵਾ.sin_family = AF_INET;
                        ctx->ਜੀਵਾ.sin_addr.s_addr = rn->u.addr;

                    } ਨਹੀਂਤਾਂ {
                        ctx->addrs = addrs;
                    }

                    ਅਗਲਾ = ctx->ਅਗਲਾ;

                    ctx->handler(ctx);

                    ctx = ਅਗਲਾ;
                } ਜਦੋਂ (ctx);

                ਜੇ (addrs != ਨਿਰਾ) {
                    ngx_resolver_free(r, addrs->sockaddr);
                    ngx_resolver_free(r, addrs);
                }

                ਵਾਪਸ NGX_OK;
            }

            ਜੇ (rn->nsrvs) {
                ਆਖਰੀ->ਅਗਲਾ = rn->ਉਡੀਕ;
                rn->ਉਡੀਕ = ਨਿਰਾ;

                /* unlock name mutex */

                ਕਰੋ {
                    ਅਗਲਾ = ctx->ਅਗਲਾ;

                    ngx_resolver_resolve_srv_ਨਾਂ(ctx, rn);

                    ctx = ਅਗਲਾ;
                } ਜਦੋਂ (ctx);

                ਵਾਪਸ NGX_OK;
            }

            /* NGX_RESOLVE_CNAME */

            ਜੇ (ctx->recursion++ < NGX_RESOLVER_MAX_RECURSION) {

                cname.len = rn->cnlen;
                cname.ਅੰਕੜੇ = rn->u.cname;

                ਵਾਪਸ ngx_resolve_name_locked(r, ctx, &cname);
            }

            ਆਖਰੀ->ਅਗਲਾ = rn->ਉਡੀਕ;
            rn->ਉਡੀਕ = ਨਿਰਾ;

            /* unlock name mutex */

            ਕਰੋ {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                ctx->valid = ngx_ਸਮਾਂ() + (r->valid ? r->valid : 10);
                ਅਗਲਾ = ctx->ਅਗਲਾ;

                ctx->handler(ctx);

                ctx = ਅਗਲਾ;
            } ਜਦੋਂ (ctx);

            ਵਾਪਸ NGX_OK;
        }

        ਜੇ (rn->ਉਡੀਕ) {
            ਜੇ (ngx_resolver_ਸੈੱਟ_timeout(r, ctx) != NGX_OK) {
                ਵਾਪਸ NGX_ERROR;
            }

            ਆਖਰੀ->ਅਗਲਾ = rn->ਉਡੀਕ;
            rn->ਉਡੀਕ = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;

            ਕਰੋ {
                ctx->ਗੰਢ = rn;
                ctx = ctx->ਅਗਲਾ;
            } ਜਦੋਂ (ctx);

            ਵਾਪਸ NGX_AGAIN;
        }

        ngx_ਕਤਾਰ_remove(&rn->ਕਤਾਰ);

        /* lock alloc mutex */

        ਜੇ (rn->query) {
            ngx_resolver_free_locked(r, rn->query);
            rn->query = ਨਿਰਾ;
#ਜੇ (NGX_HAVE_INET6)
            rn->query6 = ਨਿਰਾ;
#ਜੇ_ਅੰਤ
        }

        ਜੇ (rn->cnlen) {
            ngx_resolver_free_locked(r, rn->u.cname);
        }

        ਜੇ (rn->naddrs > 1 && rn->naddrs != (u_short) -1) {
            ngx_resolver_free_locked(r, rn->u.addrs);
        }

#ਜੇ (NGX_HAVE_INET6)
        ਜੇ (rn->naddrs6 > 1 && rn->naddrs6 != (u_short) -1) {
            ngx_resolver_free_locked(r, rn->u6.addrs6);
        }
#ਜੇ_ਅੰਤ

        ਜੇ (rn->nsrvs) {
            ਲਈ (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
                ਜੇ (rn->u.srvs[i].name.ਅੰਕੜੇ) {
                    ngx_resolver_free_locked(r, rn->u.srvs[i].name.ਅੰਕੜੇ);
                }
            }

            ngx_resolver_free_locked(r, rn->u.srvs);
        }

        /* unlock alloc mutex */

    } ਨਹੀਂਤਾਂ {

        rn = ngx_resolver_alloc(r, ਆਕਾਰ(ngx_resolver_ਗੰਢ_t));
        ਜੇ (rn == ਨਿਰਾ) {
            ਵਾਪਸ NGX_ERROR;
        }

        rn->name = ngx_resolver_dup(r, name->ਅੰਕੜੇ, name->len);
        ਜੇ (rn->name == ਨਿਰਾ) {
            ngx_resolver_free(r, rn);
            ਵਾਪਸ NGX_ERROR;
        }

        rn->ਗੰਢ.ਕੁੰਜੀ = hash;
        rn->nlen = (u_short) name->len;
        rn->query = ਨਿਰਾ;
#ਜੇ (NGX_HAVE_INET6)
        rn->query6 = ਨਿਰਾ;
#ਜੇ_ਅੰਤ

        ngx_rbtree_ਪਾਓ(ਰੁੱਖ, &rn->ਗੰਢ);
    }

    ਜੇ (ctx->service.len) {
        rc = ngx_resolver_create_srv_query(r, rn, name);

    } ਨਹੀਂਤਾਂ {
        rc = ngx_resolver_create_name_query(r, rn, name);
    }

    ਜੇ (rc == NGX_ERROR) {
        ਜਾਓ failed;
    }

    ਜੇ (rc == NGX_DECLINED) {
        ngx_rbtree_delete(ਰੁੱਖ, &rn->ਗੰਢ);

        ngx_resolver_free(r, rn->query);
        ngx_resolver_free(r, rn->name);
        ngx_resolver_free(r, rn);

        ਕਰੋ {
            ctx->state = NGX_RESOLVE_NXDOMAIN;
            ਅਗਲਾ = ctx->ਅਗਲਾ;

            ctx->handler(ctx);

            ctx = ਅਗਲਾ;
        } ਜਦੋਂ (ctx);

        ਵਾਪਸ NGX_OK;
    }

    rn->ਆਖਰੀ_connection = r->ਆਖਰੀ_connection++;
    ਜੇ (r->ਆਖਰੀ_connection == r->connections.nelts) {
        r->ਆਖਰੀ_connection = 0;
    }

    rn->naddrs = r->ipv4 ? (u_short) -1 : 0;
    rn->tcp = 0;
#ਜੇ (NGX_HAVE_INET6)
    rn->naddrs6 = r->ipv6 ? (u_short) -1 : 0;
    rn->tcp6 = 0;
#ਜੇ_ਅੰਤ
    rn->nsrvs = 0;

    ਜੇ (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->ਆਖਰੀ_connection++;
        ਜੇ (rn->ਆਖਰੀ_connection == r->connections.nelts) {
            rn->ਆਖਰੀ_connection = 0;
        }

        (ਖਾਲੀ) ngx_resolver_send_query(r, rn);
    }

    ਜੇ (ngx_resolver_ਸੈੱਟ_timeout(r, ctx) != NGX_OK) {
        ਜਾਓ failed;
    }

    ਜੇ (ngx_resolver_resend_ਖਾਲੀ_ਥਾਵਾਂ(r)) {
        ngx_ਜੋੜੋ_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_ਸਮਾਂ() + r->resend_timeout;

    ngx_ਕਤਾਰ_ਪਾਓ_ਸਿਰ(resend_ਕਤਾਰ, &rn->ਕਤਾਰ);

    rn->ਕੋਡ = 0;
    rn->cnlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_ਮੁੱਲ;
    rn->ਉਡੀਕ = ctx;

    ctx->state = NGX_AGAIN;
    ctx->async = 1;

    ਕਰੋ {
        ctx->ਗੰਢ = rn;
        ctx = ctx->ਅਗਲਾ;
    } ਜਦੋਂ (ctx);

    ਵਾਪਸ NGX_AGAIN;

failed:

    ngx_rbtree_delete(ਰੁੱਖ, &rn->ਗੰਢ);

    ਜੇ (rn->query) {
        ngx_resolver_free(r, rn->query);
    }

    ngx_resolver_free(r, rn->name);

    ngx_resolver_free(r, rn);

    ਵਾਪਸ NGX_ERROR;
}


ngx_int_t
ngx_resolve_addr(ngx_resolver_ctx_t *ctx)
{
    u_char               *name;
    ਵਿੱਚ_addr_t             addr;
    ngx_ਕਤਾਰ_t          *resend_ਕਤਾਰ, *expire_ਕਤਾਰ;
    ngx_rbtree_t         *ਰੁੱਖ;
    ngx_resolver_t       *r;
    ਬਣਤਰ sockaddr_ਵਿੱਚ   *ਜੀਵਾ;
    ngx_resolver_ਗੰਢ_t  *rn;
#ਜੇ (NGX_HAVE_INET6)
    uint32_t              hash;
    ਬਣਤਰ sockaddr_in6  *sin6;
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_SUPPRESS_WARN)
    addr = 0;
#ਜੇ (NGX_HAVE_INET6)
    hash = 0;
    sin6 = ਨਿਰਾ;
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

    r = ctx->resolver;

    ਚੋਣ (ctx->addr.sockaddr->sa_family) {

#ਜੇ (NGX_HAVE_INET6)
    ਹਾਲਤ AF_INET6:
        sin6 = (ਬਣਤਰ sockaddr_in6 *) ctx->addr.sockaddr;
        hash = ngx_ਚੱਕਰੀ_ਜਾਂਚ_32_short(sin6->sin6_addr.s6_addr, 16);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr6(r, &sin6->sin6_addr, hash);

        ਰੁੱਖ = &r->addr6_rbtree;
        resend_ਕਤਾਰ = &r->addr6_resend_ਕਤਾਰ;
        expire_ਕਤਾਰ = &r->addr6_expire_ਕਤਾਰ;

        ਰੋਕੋ;
#ਜੇ_ਅੰਤ

    ਮੂਲ: /* AF_INET */
        ਜੀਵਾ = (ਬਣਤਰ sockaddr_ਵਿੱਚ *) ctx->addr.sockaddr;
        addr = ntohl(ਜੀਵਾ->sin_addr.s_addr);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        ਰੁੱਖ = &r->addr_rbtree;
        resend_ਕਤਾਰ = &r->addr_resend_ਕਤਾਰ;
        expire_ਕਤਾਰ = &r->addr_expire_ਕਤਾਰ;
    }

    ਜੇ (rn) {

        ਜੇ (rn->valid >= ngx_ਸਮਾਂ()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->ਲਘੂਗਣਕ, 0, "resolve cached");

            ngx_ਕਤਾਰ_remove(&rn->ਕਤਾਰ);

            rn->expire = ngx_ਸਮਾਂ() + r->expire;

            ngx_ਕਤਾਰ_ਪਾਓ_ਸਿਰ(expire_ਕਤਾਰ, &rn->ਕਤਾਰ);

            name = ngx_resolver_dup(r, rn->name, rn->nlen);
            ਜੇ (name == ਨਿਰਾ) {
                ngx_resolver_free(r, ctx);
                ਵਾਪਸ NGX_ERROR;
            }

            ctx->name.len = rn->nlen;
            ctx->name.ਅੰਕੜੇ = name;

            /* unlock addr mutex */

            ctx->state = NGX_OK;
            ctx->valid = rn->valid;

            ctx->handler(ctx);

            ngx_resolver_free(r, name);

            ਵਾਪਸ NGX_OK;
        }

        ਜੇ (rn->ਉਡੀਕ) {
            ਜੇ (ngx_resolver_ਸੈੱਟ_timeout(r, ctx) != NGX_OK) {
                ਵਾਪਸ NGX_ERROR;
            }

            ctx->ਅਗਲਾ = rn->ਉਡੀਕ;
            rn->ਉਡੀਕ = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;
            ctx->ਗੰਢ = rn;

            /* unlock addr mutex */

            ਵਾਪਸ NGX_OK;
        }

        ngx_ਕਤਾਰ_remove(&rn->ਕਤਾਰ);

        ngx_resolver_free(r, rn->query);
        rn->query = ਨਿਰਾ;
#ਜੇ (NGX_HAVE_INET6)
        rn->query6 = ਨਿਰਾ;
#ਜੇ_ਅੰਤ

    } ਨਹੀਂਤਾਂ {
        rn = ngx_resolver_alloc(r, ਆਕਾਰ(ngx_resolver_ਗੰਢ_t));
        ਜੇ (rn == ਨਿਰਾ) {
            ਜਾਓ failed;
        }

        ਚੋਣ (ctx->addr.sockaddr->sa_family) {

#ਜੇ (NGX_HAVE_INET6)
        ਹਾਲਤ AF_INET6:
            rn->addr6 = sin6->sin6_addr;
            rn->ਗੰਢ.ਕੁੰਜੀ = hash;
            ਰੋਕੋ;
#ਜੇ_ਅੰਤ

        ਮੂਲ: /* AF_INET */
            rn->ਗੰਢ.ਕੁੰਜੀ = addr;
        }

        rn->query = ਨਿਰਾ;
#ਜੇ (NGX_HAVE_INET6)
        rn->query6 = ਨਿਰਾ;
#ਜੇ_ਅੰਤ

        ngx_rbtree_ਪਾਓ(ਰੁੱਖ, &rn->ਗੰਢ);
    }

    ਜੇ (ngx_resolver_create_addr_query(r, rn, &ctx->addr) != NGX_OK) {
        ਜਾਓ failed;
    }

    rn->ਆਖਰੀ_connection = r->ਆਖਰੀ_connection++;
    ਜੇ (r->ਆਖਰੀ_connection == r->connections.nelts) {
        r->ਆਖਰੀ_connection = 0;
    }

    rn->naddrs = (u_short) -1;
    rn->tcp = 0;
#ਜੇ (NGX_HAVE_INET6)
    rn->naddrs6 = (u_short) -1;
    rn->tcp6 = 0;
#ਜੇ_ਅੰਤ
    rn->nsrvs = 0;

    ਜੇ (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->ਆਖਰੀ_connection++;
        ਜੇ (rn->ਆਖਰੀ_connection == r->connections.nelts) {
            rn->ਆਖਰੀ_connection = 0;
        }

        (ਖਾਲੀ) ngx_resolver_send_query(r, rn);
    }

    ਜੇ (ngx_resolver_ਸੈੱਟ_timeout(r, ctx) != NGX_OK) {
        ਜਾਓ failed;
    }

    ਜੇ (ngx_resolver_resend_ਖਾਲੀ_ਥਾਵਾਂ(r)) {
        ngx_ਜੋੜੋ_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_ਸਮਾਂ() + r->resend_timeout;

    ngx_ਕਤਾਰ_ਪਾਓ_ਸਿਰ(resend_ਕਤਾਰ, &rn->ਕਤਾਰ);

    rn->ਕੋਡ = 0;
    rn->cnlen = 0;
    rn->name = ਨਿਰਾ;
    rn->nlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_ਮੁੱਲ;
    rn->ਉਡੀਕ = ctx;

    /* unlock addr mutex */

    ctx->state = NGX_AGAIN;
    ctx->async = 1;
    ctx->ਗੰਢ = rn;

    ਵਾਪਸ NGX_OK;

failed:

    ਜੇ (rn) {
        ngx_rbtree_delete(ਰੁੱਖ, &rn->ਗੰਢ);

        ਜੇ (rn->query) {
            ngx_resolver_free(r, rn->query);
        }

        ngx_resolver_free(r, rn);
    }

    /* unlock addr mutex */

    ਜੇ (ctx->event) {
        ngx_resolver_free(r, ctx->event);
    }

    ngx_resolver_free(r, ctx);

    ਵਾਪਸ NGX_ERROR;
}


ਖਾਲੀ
ngx_resolve_addr_ਪੂਰਾ(ngx_resolver_ctx_t *ctx)
{
    ngx_ਕਤਾਰ_t          *expire_ਕਤਾਰ;
    ngx_rbtree_t         *ਰੁੱਖ;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_ਗੰਢ_t  *rn;

    r = ctx->resolver;

    ਚੋਣ (ctx->addr.sockaddr->sa_family) {

#ਜੇ (NGX_HAVE_INET6)
    ਹਾਲਤ AF_INET6:
        ਰੁੱਖ = &r->addr6_rbtree;
        expire_ਕਤਾਰ = &r->addr6_expire_ਕਤਾਰ;
        ਰੋਕੋ;
#ਜੇ_ਅੰਤ

    ਮੂਲ: /* AF_INET */
        ਰੁੱਖ = &r->addr_rbtree;
        expire_ਕਤਾਰ = &r->addr_expire_ਕਤਾਰ;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->ਲਘੂਗਣਕ, 0,
                   "resolve addr done: %i", ctx->state);

    ਜੇ (ctx->event && ctx->event->timer_ਸੈੱਟ) {
        ngx_del_timer(ctx->event);
    }

    /* lock addr mutex */

    ਜੇ (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->ਗੰਢ;

        ਜੇ (rn) {
            p = &rn->ਉਡੀਕ;
            w = rn->ਉਡੀਕ;

            ਜਦੋਂ (w) {
                ਜੇ (w == ctx) {
                    *p = w->ਅਗਲਾ;

                    ਜਾਓ ਪੂਰਾ;
                }

                p = &w->ਅਗਲਾ;
                w = w->ਅਗਲਾ;
            }
        }

        {
            u_char     ਪਾਠ[NGX_SOCKADDR_STRLEN];
            ngx_str_t  addrtext;

            addrtext.ਅੰਕੜੇ = ਪਾਠ;
            addrtext.len = ngx_sock_ntop(ctx->addr.sockaddr, ctx->addr.socklen,
                                         ਪਾਠ, NGX_SOCKADDR_STRLEN, 0);

            ngx_log_error(NGX_LOG_ALERT, r->ਲਘੂਗਣਕ, 0,
                          "could not cancel %V resolving", &addrtext);
        }
    }

ਪੂਰਾ:

    ngx_resolver_expire(r, ਰੁੱਖ, expire_ਕਤਾਰ);

    /* unlock addr mutex */

    /* lock alloc mutex */

    ਜੇ (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    ਜੇ (r->event->timer_ਸੈੱਟ && ngx_resolver_resend_ਖਾਲੀ_ਥਾਵਾਂ(r)) {
        ngx_del_timer(r->event);
    }
}


ਸਥਿਰ ਖਾਲੀ
ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *ਰੁੱਖ, ngx_ਕਤਾਰ_t *ਕਤਾਰ)
{
    ਸਮਾਂ_t                now;
    ngx_uint_t            i;
    ngx_ਕਤਾਰ_t          *q;
    ngx_resolver_ਗੰਢ_t  *rn;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->ਲਘੂਗਣਕ, 0, "resolver expire");

    now = ngx_ਸਮਾਂ();

    ਲਈ (i = 0; i < 2; i++) {
        ਜੇ (ngx_ਕਤਾਰ_ਖਾਲੀ_ਥਾਵਾਂ(ਕਤਾਰ)) {
            ਵਾਪਸ;
        }

        q = ngx_ਕਤਾਰ_ਆਖਰੀ(ਕਤਾਰ);

        rn = ngx_ਕਤਾਰ_ਅੰਕੜੇ(q, ngx_resolver_ਗੰਢ_t, ਕਤਾਰ);

        ਜੇ (now <= rn->expire) {
            ਵਾਪਸ;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->ਲਘੂਗਣਕ, 0,
                       "resolver expire \"%*s\"", (ਮਾਪ_t) rn->nlen, rn->name);

        ngx_ਕਤਾਰ_remove(q);

        ngx_rbtree_delete(ਰੁੱਖ, &rn->ਗੰਢ);

        ngx_resolver_free_ਗੰਢ(r, rn);
    }
}


ਸਥਿਰ ngx_int_t
ngx_resolver_send_query(ngx_resolver_t *r, ngx_resolver_ਗੰਢ_t *rn)
{
    ngx_int_t                   rc;
    ngx_resolver_connection_t  *rec;

    rec = r->connections.elts;
    rec = &rec[rn->ਆਖਰੀ_connection];

    ਜੇ (rec->ਲਘੂਗਣਕ.handler == ਨਿਰਾ) {
        rec->ਲਘੂਗਣਕ = *r->ਲਘੂਗਣਕ;
        rec->ਲਘੂਗਣਕ.handler = ngx_resolver_log_error;
        rec->ਲਘੂਗਣਕ.ਅੰਕੜੇ = rec;
        rec->ਲਘੂਗਣਕ.action = "resolving";
    }

    ਜੇ (rn->query && rn->naddrs == (u_short) -1) {
        rc = rn->tcp ? ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen)
                     : ngx_resolver_send_udp_query(r, rec, rn->query, rn->qlen);

        ਜੇ (rc != NGX_OK) {
            ਵਾਪਸ rc;
        }
    }

#ਜੇ (NGX_HAVE_INET6)

    ਜੇ (rn->query6 && rn->naddrs6 == (u_short) -1) {
        rc = rn->tcp6
                    ? ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen)
                    : ngx_resolver_send_udp_query(r, rec, rn->query6, rn->qlen);

        ਜੇ (rc != NGX_OK) {
            ਵਾਪਸ rc;
        }
    }

#ਜੇ_ਅੰਤ

    ਵਾਪਸ NGX_OK;
}


ਸਥਿਰ ngx_int_t
ngx_resolver_send_udp_query(ngx_resolver_t *r, ngx_resolver_connection_t  *rec,
    u_char *query, u_short qlen)
{
    ssize_t  n;

    ਜੇ (rec->udp == ਨਿਰਾ) {
        ਜੇ (ngx_udp_connect(rec) != NGX_OK) {
            ਵਾਪਸ NGX_ERROR;
        }

        rec->udp->ਅੰਕੜੇ = rec;
        rec->udp->ਪਠ->handler = ngx_resolver_udp_read;
        rec->udp->ਪਠ->resolver = 1;
    }

    n = ngx_send(rec->udp, query, qlen);

    ਜੇ (n == NGX_ERROR) {
        ਜਾਓ failed;
    }

    ਜੇ ((ਮਾਪ_t) n != (ਮਾਪ_t) qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->ਲਘੂਗਣਕ, 0, "send() incomplete");
        ਜਾਓ failed;
    }

    ਵਾਪਸ NGX_OK;

failed:

    ngx_close_connection(rec->udp);
    rec->udp = ਨਿਰਾ;

    ਵਾਪਸ NGX_ERROR;
}


ਸਥਿਰ ngx_int_t
ngx_resolver_send_tcp_query(ngx_resolver_t *r, ngx_resolver_connection_t *rec,
    u_char *query, u_short qlen)
{
    ngx_buf_t  *b;
    ngx_int_t   rc;

    rc = NGX_OK;

    ਜੇ (rec->tcp == ਨਿਰਾ) {
        b = rec->read_buf;

        ਜੇ (b == ਨਿਰਾ) {
            b = ngx_resolver_calloc(r, ਆਕਾਰ(ngx_buf_t));
            ਜੇ (b == ਨਿਰਾ) {
                ਵਾਪਸ NGX_ERROR;
            }

            b->ਸ਼ੁਰੂ = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_RSIZE);
            ਜੇ (b->ਸ਼ੁਰੂ == ਨਿਰਾ) {
                ngx_resolver_free(r, b);
                ਵਾਪਸ NGX_ERROR;
            }

            b->ਸਿਰਾ = b->ਸ਼ੁਰੂ + NGX_RESOLVER_TCP_RSIZE;

            rec->read_buf = b;
        }

        b->pos = b->ਸ਼ੁਰੂ;
        b->ਆਖਰੀ = b->ਸ਼ੁਰੂ;

        b = rec->write_buf;

        ਜੇ (b == ਨਿਰਾ) {
            b = ngx_resolver_calloc(r, ਆਕਾਰ(ngx_buf_t));
            ਜੇ (b == ਨਿਰਾ) {
                ਵਾਪਸ NGX_ERROR;
            }

            b->ਸ਼ੁਰੂ = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_WSIZE);
            ਜੇ (b->ਸ਼ੁਰੂ == ਨਿਰਾ) {
                ngx_resolver_free(r, b);
                ਵਾਪਸ NGX_ERROR;
            }

            b->ਸਿਰਾ = b->ਸ਼ੁਰੂ + NGX_RESOLVER_TCP_WSIZE;

            rec->write_buf = b;
        }

        b->pos = b->ਸ਼ੁਰੂ;
        b->ਆਖਰੀ = b->ਸ਼ੁਰੂ;

        rc = ngx_tcp_connect(rec);
        ਜੇ (rc == NGX_ERROR) {
            ਵਾਪਸ NGX_ERROR;
        }

        rec->tcp->ਅੰਕੜੇ = rec;
        rec->tcp->ਲਿਖ->handler = ngx_resolver_tcp_write;
        rec->tcp->ਲਿਖ->cancelable = 1;
        rec->tcp->ਪਠ->handler = ngx_resolver_tcp_read;
        rec->tcp->ਪਠ->resolver = 1;

        ngx_ਜੋੜੋ_timer(rec->tcp->ਲਿਖ, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    b = rec->write_buf;

    ਜੇ (b->ਸਿਰਾ - b->ਆਖਰੀ <  2 + qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->ਲਘੂਗਣਕ, 0, "buffer overflow");
        ਵਾਪਸ NGX_ERROR;
    }

    *b->ਆਖਰੀ++ = (u_char) (qlen >> 8);
    *b->ਆਖਰੀ++ = (u_char) qlen;
    b->ਆਖਰੀ = ngx_cpymem(b->ਆਖਰੀ, query, qlen);

    ਜੇ (rc == NGX_OK) {
        ngx_resolver_tcp_write(rec->tcp->ਲਿਖ);
    }

    ਵਾਪਸ NGX_OK;
}


ਸਥਿਰ ਖਾਲੀ
ngx_resolver_resend_handler(ngx_event_t *ev)
{
    ਸਮਾਂ_t           timer, atimer, stimer, ntimer;
#ਜੇ (NGX_HAVE_INET6)
    ਸਮਾਂ_t           a6timer;
#ਜੇ_ਅੰਤ
    ngx_resolver_t  *r;

    r = ev->ਅੰਕੜੇ;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->ਲਘੂਗਣਕ, 0,
                   "resolver resend handler");

    /* lock name mutex */

    ntimer = ngx_resolver_resend(r, &r->name_rbtree, &r->name_resend_ਕਤਾਰ);

    stimer = ngx_resolver_resend(r, &r->srv_rbtree, &r->srv_resend_ਕਤਾਰ);

    /* unlock name mutex */

    /* lock addr mutex */

    atimer = ngx_resolver_resend(r, &r->addr_rbtree, &r->addr_resend_ਕਤਾਰ);

    /* unlock addr mutex */

#ਜੇ (NGX_HAVE_INET6)

    /* lock addr6 mutex */

    a6timer = ngx_resolver_resend(r, &r->addr6_rbtree, &r->addr6_resend_ਕਤਾਰ);

    /* unlock addr6 mutex */

#ਜੇ_ਅੰਤ

    timer = ntimer;

    ਜੇ (timer == 0) {
        timer = atimer;

    } ਨਹੀਂਤਾਂ ਜੇ (atimer) {
        timer = ngx_min(timer, atimer);
    }

    ਜੇ (timer == 0) {
        timer = stimer;

    } ਨਹੀਂਤਾਂ ਜੇ (stimer) {
        timer = ngx_min(timer, stimer);
    }

#ਜੇ (NGX_HAVE_INET6)

    ਜੇ (timer == 0) {
        timer = a6timer;

    } ਨਹੀਂਤਾਂ ਜੇ (a6timer) {
        timer = ngx_min(timer, a6timer);
    }

#ਜੇ_ਅੰਤ

    ਜੇ (timer) {
        ngx_ਜੋੜੋ_timer(r->event, (ngx_msec_t) (timer * 1000));
    }
}


ਸਥਿਰ ਸਮਾਂ_t
ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *ਰੁੱਖ, ngx_ਕਤਾਰ_t *ਕਤਾਰ)
{
    ਸਮਾਂ_t                now;
    ngx_ਕਤਾਰ_t          *q;
    ngx_resolver_ਗੰਢ_t  *rn;

    now = ngx_ਸਮਾਂ();

    ਲਈ ( ;; ) {
        ਜੇ (ngx_ਕਤਾਰ_ਖਾਲੀ_ਥਾਵਾਂ(ਕਤਾਰ)) {
            ਵਾਪਸ 0;
        }

        q = ngx_ਕਤਾਰ_ਆਖਰੀ(ਕਤਾਰ);

        rn = ngx_ਕਤਾਰ_ਅੰਕੜੇ(q, ngx_resolver_ਗੰਢ_t, ਕਤਾਰ);

        ਜੇ (now < rn->expire) {
            ਵਾਪਸ rn->expire - now;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->ਲਘੂਗਣਕ, 0,
                       "resolver resend \"%*s\" %p",
                       (ਮਾਪ_t) rn->nlen, rn->name, rn->ਉਡੀਕ);

        ngx_ਕਤਾਰ_remove(q);

        ਜੇ (rn->ਉਡੀਕ) {

            ਜੇ (++rn->ਆਖਰੀ_connection == r->connections.nelts) {
                rn->ਆਖਰੀ_connection = 0;
            }

            (ਖਾਲੀ) ngx_resolver_send_query(r, rn);

            rn->expire = now + r->resend_timeout;

            ngx_ਕਤਾਰ_ਪਾਓ_ਸਿਰ(ਕਤਾਰ, q);

            ਜਾਰੀ;
        }

        ngx_rbtree_delete(ਰੁੱਖ, &rn->ਗੰਢ);

        ngx_resolver_free_ਗੰਢ(r, rn);
    }
}


ਸਥਿਰ ngx_uint_t
ngx_resolver_resend_ਖਾਲੀ_ਥਾਵਾਂ(ngx_resolver_t *r)
{
    ਵਾਪਸ ngx_ਕਤਾਰ_ਖਾਲੀ_ਥਾਵਾਂ(&r->name_resend_ਕਤਾਰ)
           && ngx_ਕਤਾਰ_ਖਾਲੀ_ਥਾਵਾਂ(&r->srv_resend_ਕਤਾਰ)
#ਜੇ (NGX_HAVE_INET6)
           && ngx_ਕਤਾਰ_ਖਾਲੀ_ਥਾਵਾਂ(&r->addr6_resend_ਕਤਾਰ)
#ਜੇ_ਅੰਤ
           && ngx_ਕਤਾਰ_ਖਾਲੀ_ਥਾਵਾਂ(&r->addr_resend_ਕਤਾਰ);
}


ਸਥਿਰ ਖਾਲੀ
ngx_resolver_udp_read(ngx_event_t *rev)
{
    ssize_t                     n;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;
    u_char                      buf[NGX_RESOLVER_UDP_ਮਾਪ];

    c = rev->ਅੰਕੜੇ;
    rec = c->ਅੰਕੜੇ;

    ਕਰੋ {
        n = ngx_udp_recv(c, buf, NGX_RESOLVER_UDP_ਮਾਪ);

        ਜੇ (n == NGX_AGAIN) {
            ਰੋਕੋ;
        }

        ਜੇ (n == NGX_ERROR) {
            ਜਾਓ failed;
        }

        ngx_resolver_process_response(rec->resolver, buf, n, 0);

    } ਜਦੋਂ (rev->ready);

    ਜੇ (ngx_handle_read_event(rev, 0) != NGX_OK) {
        ਜਾਓ failed;
    }

    ਵਾਪਸ;

failed:

    ngx_close_connection(rec->udp);
    rec->udp = ਨਿਰਾ;
}


ਸਥਿਰ ਖਾਲੀ
ngx_resolver_tcp_write(ngx_event_t *wev)
{
    off_t                       ਭੇਜਿਆ;
    ssize_t                     n;
    ngx_buf_t                  *b;
    ngx_resolver_t             *r;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;

    c = wev->ਅੰਕੜੇ;
    rec = c->ਅੰਕੜੇ;
    b = rec->write_buf;
    r = rec->resolver;

    ਜੇ (wev->timedout) {
        ਜਾਓ failed;
    }

    ਭੇਜਿਆ = c->ਭੇਜਿਆ;

    ਜਦੋਂ (wev->ready && b->pos < b->ਆਖਰੀ) {
        n = ngx_send(c, b->pos, b->ਆਖਰੀ - b->pos);

        ਜੇ (n == NGX_AGAIN) {
            ਰੋਕੋ;
        }

        ਜੇ (n == NGX_ERROR) {
            ਜਾਓ failed;
        }

        b->pos += n;
    }

    ਜੇ (b->pos != b->ਸ਼ੁਰੂ) {
        b->ਆਖਰੀ = ngx_movemem(b->ਸ਼ੁਰੂ, b->pos, b->ਆਖਰੀ - b->pos);
        b->pos = b->ਸ਼ੁਰੂ;
    }

    ਜੇ (c->ਭੇਜਿਆ != ਭੇਜਿਆ) {
        ngx_ਜੋੜੋ_timer(wev, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    ਜੇ (ngx_handle_write_event(wev, 0) != NGX_OK) {
        ਜਾਓ failed;
    }

    ਵਾਪਸ;

failed:

    ngx_close_connection(c);
    rec->tcp = ਨਿਰਾ;
}


ਸਥਿਰ ਖਾਲੀ
ngx_resolver_tcp_read(ngx_event_t *rev)
{
    u_char                     *p;
    ਮਾਪ_t                      ਮਾਪ;
    ssize_t                     n;
    u_short                     qlen;
    ngx_buf_t                  *b;
    ngx_resolver_t             *r;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;

    c = rev->ਅੰਕੜੇ;
    rec = c->ਅੰਕੜੇ;
    b = rec->read_buf;
    r = rec->resolver;

    ਜਦੋਂ (rev->ready) {
        n = ngx_recv(c, b->ਆਖਰੀ, b->ਸਿਰਾ - b->ਆਖਰੀ);

        ਜੇ (n == NGX_AGAIN) {
            ਰੋਕੋ;
        }

        ਜੇ (n == NGX_ERROR || n == 0) {
            ਜਾਓ failed;
        }

        b->ਆਖਰੀ += n;

        ਲਈ ( ;; ) {
            p = b->pos;
            ਮਾਪ = b->ਆਖਰੀ - p;

            ਜੇ (ਮਾਪ < 2) {
                ਰੋਕੋ;
            }

            qlen = (u_short) *p++ << 8;
            qlen += *p++;

            ਜੇ (ਮਾਪ < (ਮਾਪ_t) (2 + qlen)) {
                ਰੋਕੋ;
            }

            ngx_resolver_process_response(r, p, qlen, 1);

            b->pos += 2 + qlen;
        }

        ਜੇ (b->pos != b->ਸ਼ੁਰੂ) {
            b->ਆਖਰੀ = ngx_movemem(b->ਸ਼ੁਰੂ, b->pos, b->ਆਖਰੀ - b->pos);
            b->pos = b->ਸ਼ੁਰੂ;
        }
    }

    ਜੇ (ngx_handle_read_event(rev, 0) != NGX_OK) {
        ਜਾਓ failed;
    }

    ਵਾਪਸ;

failed:

    ngx_close_connection(c);
    rec->tcp = ਨਿਰਾ;
}


ਸਥਿਰ ਖਾਲੀ
ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf, ਮਾਪ_t n,
    ngx_uint_t tcp)
{
    ਅੱਖਰ                 *err;
    ngx_uint_t            i, times, ident, qident, flags, ਕੋਡ, nqs, nan, trunc,
                          qtype, qclass;
#ਜੇ (NGX_HAVE_INET6)
    ngx_uint_t            qident6;
#ਜੇ_ਅੰਤ
    ngx_ਕਤਾਰ_t          *q;
    ngx_resolver_qs_t    *qs;
    ngx_resolver_hdr_t   *response;
    ngx_resolver_ਗੰਢ_t  *rn;

    ਜੇ (n < ਆਕਾਰ(ngx_resolver_hdr_t)) {
        ਜਾਓ short_response;
    }

    response = (ngx_resolver_hdr_t *) buf;

    ident = (response->ident_hi << 8) + response->ident_lo;
    flags = (response->flags_hi << 8) + response->flags_lo;
    nqs = (response->nqs_hi << 8) + response->nqs_lo;
    nan = (response->nan_hi << 8) + response->nan_lo;
    trunc = flags & 0x0200;

    ngx_log_debug6(NGX_LOG_DEBUG_CORE, r->ਲਘੂਗਣਕ, 0,
                   "resolver DNS response %ui fl:%04Xi %ui/%ui/%ud/%ud",
                   ident, flags, nqs, nan,
                   (response->nns_hi << 8) + response->nns_lo,
                   (response->nar_hi << 8) + response->nar_lo);

    /* response to a standard query */
    ਜੇ ((flags & 0xf870) != 0x8000 || (trunc && tcp)) {
        ngx_log_error(r->log_level, r->ਲਘੂਗਣਕ, 0,
                      "invalid %s DNS response %ui fl:%04Xi",
                      tcp ? "TCP" : "UDP", ident, flags);
        ਵਾਪਸ;
    }

    ਕੋਡ = flags & 0xf;

    ਜੇ (ਕੋਡ == NGX_RESOLVE_FORMERR) {

        times = 0;

        ਲਈ (q = ngx_ਕਤਾਰ_ਸਿਰ(&r->name_resend_ਕਤਾਰ);
             q != ngx_ਕਤਾਰ_sentinel(&r->name_resend_ਕਤਾਰ) && times++ < 100;
             q = ngx_ਕਤਾਰ_ਅਗਲਾ(q))
        {
            rn = ngx_ਕਤਾਰ_ਅੰਕੜੇ(q, ngx_resolver_ਗੰਢ_t, ਕਤਾਰ);

            ਜੇ (rn->query) {
                qident = (rn->query[0] << 8) + rn->query[1];

                ਜੇ (qident == ident) {
                    ਜਾਓ dns_error_name;
                }
            }

#ਜੇ (NGX_HAVE_INET6)
            ਜੇ (rn->query6) {
                qident6 = (rn->query6[0] << 8) + rn->query6[1];

                ਜੇ (qident6 == ident) {
                    ਜਾਓ dns_error_name;
                }
            }
#ਜੇ_ਅੰਤ
        }

        ਜਾਓ dns_error;
    }

    ਜੇ (ਕੋਡ > NGX_RESOLVE_REFUSED) {
        ਜਾਓ dns_error;
    }

    ਜੇ (nqs != 1) {
        err = "invalid number of questions in DNS response";
        ਜਾਓ ਪੂਰਾ;
    }

    i = ਆਕਾਰ(ngx_resolver_hdr_t);

    ਜਦੋਂ (i < (ngx_uint_t) n) {

        ਜੇ (buf[i] & 0xc0) {
            err = "unexpected compression pointer in DNS response";
            ਜਾਓ ਪੂਰਾ;
        }

        ਜੇ (buf[i] == '\0') {
            ਜਾਓ ਮਿਲਿਆ;
        }

        i += 1 + buf[i];
    }

    ਜਾਓ short_response;

ਮਿਲਿਆ:

    ਜੇ (i++ == ਆਕਾਰ(ngx_resolver_hdr_t)) {
        err = "zero-length domain name in DNS response";
        ਜਾਓ ਪੂਰਾ;
    }

    ਜੇ (i + ਆਕਾਰ(ngx_resolver_qs_t) + nan * (2 + ਆਕਾਰ(ngx_resolver_an_t))
        > (ngx_uint_t) n)
    {
        ਜਾਓ short_response;
    }

    qs = (ngx_resolver_qs_t *) &buf[i];

    qtype = (qs->type_hi << 8) + qs->type_lo;
    qclass = (qs->class_hi << 8) + qs->class_lo;

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->ਲਘੂਗਣਕ, 0,
                   "resolver DNS response qt:%ui cl:%ui", qtype, qclass);

    ਜੇ (qclass != 1) {
        ngx_log_error(r->log_level, r->ਲਘੂਗਣਕ, 0,
                      "unknown query class %ui in DNS response", qclass);
        ਵਾਪਸ;
    }

    ਚੋਣ (qtype) {

    ਹਾਲਤ NGX_RESOLVE_A:
#ਜੇ (NGX_HAVE_INET6)
    ਹਾਲਤ NGX_RESOLVE_AAAA:
#ਜੇ_ਅੰਤ

        ngx_resolver_process_a(r, buf, n, ident, ਕੋਡ, qtype, nan, trunc,
                               i + ਆਕਾਰ(ngx_resolver_qs_t));

        ਰੋਕੋ;

    ਹਾਲਤ NGX_RESOLVE_SRV:

        ngx_resolver_process_srv(r, buf, n, ident, ਕੋਡ, nan, trunc,
                                 i + ਆਕਾਰ(ngx_resolver_qs_t));

        ਰੋਕੋ;

    ਹਾਲਤ NGX_RESOLVE_PTR:

        ngx_resolver_process_ptr(r, buf, n, ident, ਕੋਡ, nan);

        ਰੋਕੋ;

    ਮੂਲ:
        ngx_log_error(r->log_level, r->ਲਘੂਗਣਕ, 0,
                      "unknown query type %ui in DNS response", qtype);
        ਵਾਪਸ;
    }

    ਵਾਪਸ;

short_response:

    err = "short DNS response";

ਪੂਰਾ:

    ngx_log_error(r->log_level, r->ਲਘੂਗਣਕ, 0, err);

    ਵਾਪਸ;

dns_error_name:

    ngx_log_error(r->log_level, r->ਲਘੂਗਣਕ, 0,
                  "DNS error (%ui: %s), query id:%ui, name:\"%*s\"",
                  ਕੋਡ, ngx_resolver_strerror(ਕੋਡ), ident,
                  (ਮਾਪ_t) rn->nlen, rn->name);
    ਵਾਪਸ;

dns_error:

    ngx_log_error(r->log_level, r->ਲਘੂਗਣਕ, 0,
                  "DNS error (%ui: %s), query id:%ui",
                  ਕੋਡ, ngx_resolver_strerror(ਕੋਡ), ident);
    ਵਾਪਸ;
}


ਸਥਿਰ ਖਾਲੀ
ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, ਮਾਪ_t n,
    ngx_uint_t ident, ngx_uint_t ਕੋਡ, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans)
{
    ਅੱਖਰ                       *err;
    u_char                     *cname;
    ਮਾਪ_t                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    ਵਿੱਚ_addr_t                  *addr;
    ngx_str_t                   name;
    ngx_uint_t                  type, class, qident, naddrs, a, i, j, ਸ਼ੁਰੂ;
#ਜੇ (NGX_HAVE_INET6)
    ਬਣਤਰ in6_addr            *addr6;
#ਜੇ_ਅੰਤ
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *ਅਗਲਾ;
    ngx_resolver_ਗੰਢ_t        *rn;
    ngx_resolver_addr_t        *addrs;
    ngx_resolver_connection_t  *rec;

    ਜੇ (ngx_resolver_copy(r, &name, buf,
                          buf + ਆਕਾਰ(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        ਵਾਪਸ;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->ਲਘੂਗਣਕ, 0, "resolver qs:%V", &name);

    hash = ngx_ਚੱਕਰੀ_ਜਾਂਚ_32_short(name.ਅੰਕੜੇ, name.len);

    /* lock name mutex */

    rn = ngx_resolver_lookup_name(r, &name, hash);

    ਜੇ (rn == ਨਿਰਾ) {
        ngx_log_error(r->log_level, r->ਲਘੂਗਣਕ, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.ਅੰਕੜੇ);
        ਜਾਓ failed;
    }

    ਚੋਣ (qtype) {

#ਜੇ (NGX_HAVE_INET6)
    ਹਾਲਤ NGX_RESOLVE_AAAA:

        ਜੇ (rn->query6 == ਨਿਰਾ || rn->naddrs6 != (u_short) -1) {
            ngx_log_error(r->log_level, r->ਲਘੂਗਣਕ, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.ਅੰਕੜੇ);
            ਜਾਓ failed;
        }

        ਜੇ (trunc && rn->tcp6) {
            ngx_resolver_free(r, name.ਅੰਕੜੇ);
            ਜਾਓ failed;
        }

        qident = (rn->query6[0] << 8) + rn->query6[1];

        ਰੋਕੋ;
#ਜੇ_ਅੰਤ

    ਮੂਲ: /* NGX_RESOLVE_A */

        ਜੇ (rn->query == ਨਿਰਾ || rn->naddrs != (u_short) -1) {
            ngx_log_error(r->log_level, r->ਲਘੂਗਣਕ, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.ਅੰਕੜੇ);
            ਜਾਓ failed;
        }

        ਜੇ (trunc && rn->tcp) {
            ngx_resolver_free(r, name.ਅੰਕੜੇ);
            ਜਾਓ failed;
        }

        qident = (rn->query[0] << 8) + rn->query[1];
    }

    ਜੇ (ident != qident) {
        ngx_log_error(r->log_level, r->ਲਘੂਗਣਕ, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.ਅੰਕੜੇ);
        ਜਾਓ failed;
    }

    ngx_resolver_free(r, name.ਅੰਕੜੇ);

    ਜੇ (trunc) {

        ngx_ਕਤਾਰ_remove(&rn->ਕਤਾਰ);

        ਜੇ (rn->ਉਡੀਕ == ਨਿਰਾ) {
            ngx_rbtree_delete(&r->name_rbtree, &rn->ਗੰਢ);
            ngx_resolver_free_ਗੰਢ(r, rn);
            ਜਾਓ ਅਗਲਾ;
        }

        rec = r->connections.elts;
        rec = &rec[rn->ਆਖਰੀ_connection];

        ਚੋਣ (qtype) {

#ਜੇ (NGX_HAVE_INET6)
        ਹਾਲਤ NGX_RESOLVE_AAAA:

            rn->tcp6 = 1;

            (ਖਾਲੀ) ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen);

            ਰੋਕੋ;
#ਜੇ_ਅੰਤ

        ਮੂਲ: /* NGX_RESOLVE_A */

            rn->tcp = 1;

            (ਖਾਲੀ) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);
        }

        rn->expire = ngx_ਸਮਾਂ() + r->resend_timeout;

        ngx_ਕਤਾਰ_ਪਾਓ_ਸਿਰ(&r->name_resend_ਕਤਾਰ, &rn->ਕਤਾਰ);

        ਜਾਓ ਅਗਲਾ;
    }

    ਜੇ (ਕੋਡ == 0 && rn->ਕੋਡ) {
        ਕੋਡ = rn->ਕੋਡ;
    }

    ਜੇ (ਕੋਡ == 0 && nan == 0) {

#ਜੇ (NGX_HAVE_INET6)
        ਚੋਣ (qtype) {

        ਹਾਲਤ NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            ਜੇ (rn->naddrs == (u_short) -1) {
                ਜਾਓ ਅਗਲਾ;
            }

            ਜੇ (rn->naddrs) {
                ਜਾਓ export;
            }

            ਰੋਕੋ;

        ਮੂਲ: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            ਜੇ (rn->naddrs6 == (u_short) -1) {
                ਜਾਓ ਅਗਲਾ;
            }

            ਜੇ (rn->naddrs6) {
                ਜਾਓ export;
            }
        }
#ਜੇ_ਅੰਤ

        ਕੋਡ = NGX_RESOLVE_NXDOMAIN;
    }

    ਜੇ (ਕੋਡ) {

#ਜੇ (NGX_HAVE_INET6)
        ਚੋਣ (qtype) {

        ਹਾਲਤ NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            ਜੇ (rn->naddrs == (u_short) -1) {
                rn->ਕੋਡ = (u_char) ਕੋਡ;
                ਜਾਓ ਅਗਲਾ;
            }

            ਰੋਕੋ;

        ਮੂਲ: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            ਜੇ (rn->naddrs6 == (u_short) -1) {
                rn->ਕੋਡ = (u_char) ਕੋਡ;
                ਜਾਓ ਅਗਲਾ;
            }
        }
#ਜੇ_ਅੰਤ

        ਅਗਲਾ = rn->ਉਡੀਕ;
        rn->ਉਡੀਕ = ਨਿਰਾ;

        ngx_ਕਤਾਰ_remove(&rn->ਕਤਾਰ);

        ngx_rbtree_delete(&r->name_rbtree, &rn->ਗੰਢ);

        /* unlock name mutex */

        ਜਦੋਂ (ਅਗਲਾ) {
            ctx = ਅਗਲਾ;
            ctx->state = ਕੋਡ;
            ctx->valid = ngx_ਸਮਾਂ() + (r->valid ? r->valid : 10);
            ਅਗਲਾ = ctx->ਅਗਲਾ;

            ctx->handler(ctx);
        }

        ngx_resolver_free_ਗੰਢ(r, rn);

        ਵਾਪਸ;
    }

    i = ans;
    naddrs = 0;
    cname = ਨਿਰਾ;

    ਲਈ (a = 0; a < nan; a++) {

        ਸ਼ੁਰੂ = i;

        ਜਦੋਂ (i < n) {

            ਜੇ (buf[i] & 0xc0) {
                i += 2;
                ਜਾਓ ਮਿਲਿਆ;
            }

            ਜੇ (buf[i] == 0) {
                i++;
                ਜਾਓ test_ਲੰਮਾਈ;
            }

            i += 1 + buf[i];
        }

        ਜਾਓ short_response;

    test_ਲੰਮਾਈ:

        ਜੇ (i - ਸ਼ੁਰੂ < 2) {
            err = "invalid name in DNS response";
            ਜਾਓ invalid;
        }

    ਮਿਲਿਆ:

        ਜੇ (i + ਆਕਾਰ(ngx_resolver_an_t) >= n) {
            ਜਾਓ short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        ਜੇ (class != 1) {
            ngx_log_error(r->log_level, r->ਲਘੂਗਣਕ, 0,
                          "unexpected RR class %ui in DNS response", class);
            ਜਾਓ failed;
        }

        ਜੇ (ttl < 0) {
            ttl = 0;
        }

        rn->ttl = ngx_min(rn->ttl, (uint32_t) ttl);

        i += ਆਕਾਰ(ngx_resolver_an_t);

        ਚੋਣ (type) {

        ਹਾਲਤ NGX_RESOLVE_A:

            ਜੇ (qtype != NGX_RESOLVE_A) {
                err = "unexpected A record in DNS response";
                ਜਾਓ invalid;
            }

            ਜੇ (len != 4) {
                err = "invalid A record in DNS response";
                ਜਾਓ invalid;
            }

            ਜੇ (i + 4 > n) {
                ਜਾਓ short_response;
            }

            naddrs++;

            ਰੋਕੋ;

#ਜੇ (NGX_HAVE_INET6)
        ਹਾਲਤ NGX_RESOLVE_AAAA:

            ਜੇ (qtype != NGX_RESOLVE_AAAA) {
                err = "unexpected AAAA record in DNS response";
                ਜਾਓ invalid;
            }

            ਜੇ (len != 16) {
                err = "invalid AAAA record in DNS response";
                ਜਾਓ invalid;
            }

            ਜੇ (i + 16 > n) {
                ਜਾਓ short_response;
            }

            naddrs++;

            ਰੋਕੋ;
#ਜੇ_ਅੰਤ

        ਹਾਲਤ NGX_RESOLVE_CNAME:

            cname = &buf[i];

            ਰੋਕੋ;

        ਹਾਲਤ NGX_RESOLVE_DNAME:

            ਰੋਕੋ;

        ਮੂਲ:

            ngx_log_error(r->log_level, r->ਲਘੂਗਣਕ, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->ਲਘੂਗਣਕ, 0,
                   "resolver naddrs:%ui cname:%p ttl:%uD",
                   naddrs, cname, rn->ttl);

    ਜੇ (naddrs) {

        ਚੋਣ (qtype) {

#ਜੇ (NGX_HAVE_INET6)
        ਹਾਲਤ NGX_RESOLVE_AAAA:

            ਜੇ (naddrs == 1) {
                addr6 = &rn->u6.addr6;
                rn->naddrs6 = 1;

            } ਨਹੀਂਤਾਂ {
                addr6 = ngx_resolver_alloc(r, naddrs * ਆਕਾਰ(ਬਣਤਰ in6_addr));
                ਜੇ (addr6 == ਨਿਰਾ) {
                    ਜਾਓ failed;
                }

                rn->u6.addrs6 = addr6;
                rn->naddrs6 = (u_short) naddrs;
            }

#ਜੇ (NGX_SUPPRESS_WARN)
            addr = ਨਿਰਾ;
#ਜੇ_ਅੰਤ

            ਰੋਕੋ;
#ਜੇ_ਅੰਤ

        ਮੂਲ: /* NGX_RESOLVE_A */

            ਜੇ (naddrs == 1) {
                addr = &rn->u.addr;
                rn->naddrs = 1;

            } ਨਹੀਂਤਾਂ {
                addr = ngx_resolver_alloc(r, naddrs * ਆਕਾਰ(ਵਿੱਚ_addr_t));
                ਜੇ (addr == ਨਿਰਾ) {
                    ਜਾਓ failed;
                }

                rn->u.addrs = addr;
                rn->naddrs = (u_short) naddrs;
            }

#ਜੇ (NGX_HAVE_INET6 && NGX_SUPPRESS_WARN)
            addr6 = ਨਿਰਾ;
#ਜੇ_ਅੰਤ
        }

        j = 0;
        i = ans;

        ਲਈ (a = 0; a < nan; a++) {

            ਲਈ ( ;; ) {

                ਜੇ (buf[i] & 0xc0) {
                    i += 2;
                    ਰੋਕੋ;
                }

                ਜੇ (buf[i] == 0) {
                    i++;
                    ਰੋਕੋ;
                }

                i += 1 + buf[i];
            }

            an = (ngx_resolver_an_t *) &buf[i];

            type = (an->type_hi << 8) + an->type_lo;
            len = (an->len_hi << 8) + an->len_lo;

            i += ਆਕਾਰ(ngx_resolver_an_t);

            ਜੇ (type == NGX_RESOLVE_A) {

                addr[j] = htonl((buf[i] << 24) + (buf[i + 1] << 16)
                                + (buf[i + 2] << 8) + (buf[i + 3]));

                ਜੇ (++j == naddrs) {

#ਜੇ (NGX_HAVE_INET6)
                    ਜੇ (rn->naddrs6 == (u_short) -1) {
                        ਜਾਓ ਅਗਲਾ;
                    }
#ਜੇ_ਅੰਤ

                    ਰੋਕੋ;
                }
            }

#ਜੇ (NGX_HAVE_INET6)
            ਨਹੀਂਤਾਂ ਜੇ (type == NGX_RESOLVE_AAAA) {

                ngx_memcpy(addr6[j].s6_addr, &buf[i], 16);

                ਜੇ (++j == naddrs) {

                    ਜੇ (rn->naddrs == (u_short) -1) {
                        ਜਾਓ ਅਗਲਾ;
                    }

                    ਰੋਕੋ;
                }
            }
#ਜੇ_ਅੰਤ

            i += len;
        }
    }

    ਚੋਣ (qtype) {

#ਜੇ (NGX_HAVE_INET6)
    ਹਾਲਤ NGX_RESOLVE_AAAA:

        ਜੇ (rn->naddrs6 == (u_short) -1) {
            rn->naddrs6 = 0;
        }

        ਰੋਕੋ;
#ਜੇ_ਅੰਤ

    ਮੂਲ: /* NGX_RESOLVE_A */

        ਜੇ (rn->naddrs == (u_short) -1) {
            rn->naddrs = 0;
        }
    }

    ਜੇ (rn->naddrs != (u_short) -1
#ਜੇ (NGX_HAVE_INET6)
        && rn->naddrs6 != (u_short) -1
#ਜੇ_ਅੰਤ
        && rn->naddrs
#ਜੇ (NGX_HAVE_INET6)
           + rn->naddrs6
#ਜੇ_ਅੰਤ
           > 0)
    {

#ਜੇ (NGX_HAVE_INET6)
    export:
#ਜੇ_ਅੰਤ

        naddrs = rn->naddrs;
#ਜੇ (NGX_HAVE_INET6)
        naddrs += rn->naddrs6;
#ਜੇ_ਅੰਤ

        ਜੇ (naddrs == 1 && rn->naddrs == 1) {
            addrs = ਨਿਰਾ;

        } ਨਹੀਂਤਾਂ {
            addrs = ngx_resolver_export(r, rn, 0);
            ਜੇ (addrs == ਨਿਰਾ) {
                ਜਾਓ failed;
            }
        }

        ngx_ਕਤਾਰ_remove(&rn->ਕਤਾਰ);

        rn->valid = ngx_ਸਮਾਂ() + (r->valid ? r->valid : (ਸਮਾਂ_t) rn->ttl);
        rn->expire = ngx_ਸਮਾਂ() + r->expire;

        ngx_ਕਤਾਰ_ਪਾਓ_ਸਿਰ(&r->name_expire_ਕਤਾਰ, &rn->ਕਤਾਰ);

        ਅਗਲਾ = rn->ਉਡੀਕ;
        rn->ਉਡੀਕ = ਨਿਰਾ;

        /* unlock name mutex */

        ਜਦੋਂ (ਅਗਲਾ) {
            ctx = ਅਗਲਾ;
            ctx->state = NGX_OK;
            ctx->valid = rn->valid;
            ctx->naddrs = naddrs;

            ਜੇ (addrs == ਨਿਰਾ) {
                ctx->addrs = &ctx->addr;
                ctx->addr.sockaddr = (ਬਣਤਰ sockaddr *) &ctx->ਜੀਵਾ;
                ctx->addr.socklen = ਆਕਾਰ(ਬਣਤਰ sockaddr_ਵਿੱਚ);
                ngx_memzero(&ctx->ਜੀਵਾ, ਆਕਾਰ(ਬਣਤਰ sockaddr_ਵਿੱਚ));
                ctx->ਜੀਵਾ.sin_family = AF_INET;
                ctx->ਜੀਵਾ.sin_addr.s_addr = rn->u.addr;

            } ਨਹੀਂਤਾਂ {
                ctx->addrs = addrs;
            }

            ਅਗਲਾ = ctx->ਅਗਲਾ;

            ctx->handler(ctx);
        }

        ਜੇ (addrs != ਨਿਰਾ) {
            ngx_resolver_free(r, addrs->sockaddr);
            ngx_resolver_free(r, addrs);
        }

        ngx_resolver_free(r, rn->query);
        rn->query = ਨਿਰਾ;
#ਜੇ (NGX_HAVE_INET6)
        rn->query6 = ਨਿਰਾ;
#ਜੇ_ਅੰਤ

        ਵਾਪਸ;
    }

    ਜੇ (cname) {

        /* CNAME only */

        ਜੇ (rn->naddrs == (u_short) -1
#ਜੇ (NGX_HAVE_INET6)
            || rn->naddrs6 == (u_short) -1
#ਜੇ_ਅੰਤ
            )
        {
            ਜਾਓ ਅਗਲਾ;
        }

        ਜੇ (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            ਜਾਓ failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->ਲਘੂਗਣਕ, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_ਕਤਾਰ_remove(&rn->ਕਤਾਰ);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.ਅੰਕੜੇ;

        rn->valid = ngx_ਸਮਾਂ() + (r->valid ? r->valid : (ਸਮਾਂ_t) rn->ttl);
        rn->expire = ngx_ਸਮਾਂ() + r->expire;

        ngx_ਕਤਾਰ_ਪਾਓ_ਸਿਰ(&r->name_expire_ਕਤਾਰ, &rn->ਕਤਾਰ);

        ngx_resolver_free(r, rn->query);
        rn->query = ਨਿਰਾ;
#ਜੇ (NGX_HAVE_INET6)
        rn->query6 = ਨਿਰਾ;
#ਜੇ_ਅੰਤ

        ctx = rn->ਉਡੀਕ;
        rn->ਉਡੀਕ = ਨਿਰਾ;

        ਜੇ (ctx) {

            ਜੇ (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                ਕਰੋ {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    ਅਗਲਾ = ctx->ਅਗਲਾ;

                    ctx->handler(ctx);

                    ctx = ਅਗਲਾ;
                } ਜਦੋਂ (ctx);

                ਵਾਪਸ;
            }

            ਲਈ (ਅਗਲਾ = ctx; ਅਗਲਾ; ਅਗਲਾ = ਅਗਲਾ->ਅਗਲਾ) {
                ਅਗਲਾ->ਗੰਢ = ਨਿਰਾ;
            }

            (ਖਾਲੀ) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        ਵਾਪਸ;
    }

    ngx_log_error(r->log_level, r->ਲਘੂਗਣਕ, 0,
                  "no A or CNAME types in DNS response");
    ਵਾਪਸ;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->ਲਘੂਗਣਕ, 0, err);

    ਵਾਪਸ;

failed:

ਅਗਲਾ:

    /* unlock name mutex */

    ਵਾਪਸ;
}


ਸਥਿਰ ਖਾਲੀ
ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, ਮਾਪ_t n,
    ngx_uint_t ident, ngx_uint_t ਕੋਡ, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans)
{
    ਅੱਖਰ                       *err;
    u_char                     *cname;
    ਮਾਪ_t                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    ngx_str_t                   name;
    ngx_uint_t                  type, qident, class, ਸ਼ੁਰੂ, nsrvs, a, i, j;
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *ਅਗਲਾ;
    ngx_resolver_srv_t         *srvs;
    ngx_resolver_ਗੰਢ_t        *rn;
    ngx_resolver_connection_t  *rec;

    ਜੇ (ngx_resolver_copy(r, &name, buf,
                          buf + ਆਕਾਰ(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        ਵਾਪਸ;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->ਲਘੂਗਣਕ, 0, "resolver qs:%V", &name);

    hash = ngx_ਚੱਕਰੀ_ਜਾਂਚ_32_short(name.ਅੰਕੜੇ, name.len);

    rn = ngx_resolver_lookup_srv(r, &name, hash);

    ਜੇ (rn == ਨਿਰਾ || rn->query == ਨਿਰਾ) {
        ngx_log_error(r->log_level, r->ਲਘੂਗਣਕ, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.ਅੰਕੜੇ);
        ਜਾਓ failed;
    }

    ਜੇ (trunc && rn->tcp) {
        ngx_resolver_free(r, name.ਅੰਕੜੇ);
        ਜਾਓ failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    ਜੇ (ident != qident) {
        ngx_log_error(r->log_level, r->ਲਘੂਗਣਕ, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.ਅੰਕੜੇ);
        ਜਾਓ failed;
    }

    ngx_resolver_free(r, name.ਅੰਕੜੇ);

    ਜੇ (trunc) {

        ngx_ਕਤਾਰ_remove(&rn->ਕਤਾਰ);

        ਜੇ (rn->ਉਡੀਕ == ਨਿਰਾ) {
            ngx_rbtree_delete(&r->srv_rbtree, &rn->ਗੰਢ);
            ngx_resolver_free_ਗੰਢ(r, rn);
            ਵਾਪਸ;
        }

        rec = r->connections.elts;
        rec = &rec[rn->ਆਖਰੀ_connection];

        rn->tcp = 1;

        (ਖਾਲੀ) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);

        rn->expire = ngx_ਸਮਾਂ() + r->resend_timeout;

        ngx_ਕਤਾਰ_ਪਾਓ_ਸਿਰ(&r->srv_resend_ਕਤਾਰ, &rn->ਕਤਾਰ);

        ਵਾਪਸ;
    }

    ਜੇ (ਕੋਡ == 0 && rn->ਕੋਡ) {
        ਕੋਡ = rn->ਕੋਡ;
    }

    ਜੇ (ਕੋਡ == 0 && nan == 0) {
        ਕੋਡ = NGX_RESOLVE_NXDOMAIN;
    }

    ਜੇ (ਕੋਡ) {
        ਅਗਲਾ = rn->ਉਡੀਕ;
        rn->ਉਡੀਕ = ਨਿਰਾ;

        ngx_ਕਤਾਰ_remove(&rn->ਕਤਾਰ);

        ngx_rbtree_delete(&r->srv_rbtree, &rn->ਗੰਢ);

        ਜਦੋਂ (ਅਗਲਾ) {
            ctx = ਅਗਲਾ;
            ctx->state = ਕੋਡ;
            ctx->valid = ngx_ਸਮਾਂ() + (r->valid ? r->valid : 10);
            ਅਗਲਾ = ctx->ਅਗਲਾ;

            ctx->handler(ctx);
        }

        ngx_resolver_free_ਗੰਢ(r, rn);

        ਵਾਪਸ;
    }

    i = ans;
    nsrvs = 0;
    cname = ਨਿਰਾ;

    ਲਈ (a = 0; a < nan; a++) {

        ਸ਼ੁਰੂ = i;

        ਜਦੋਂ (i < n) {

            ਜੇ (buf[i] & 0xc0) {
                i += 2;
                ਜਾਓ ਮਿਲਿਆ;
            }

            ਜੇ (buf[i] == 0) {
                i++;
                ਜਾਓ test_ਲੰਮਾਈ;
            }

            i += 1 + buf[i];
        }

        ਜਾਓ short_response;

    test_ਲੰਮਾਈ:

        ਜੇ (i - ਸ਼ੁਰੂ < 2) {
            err = "invalid name DNS response";
            ਜਾਓ invalid;
        }

    ਮਿਲਿਆ:

        ਜੇ (i + ਆਕਾਰ(ngx_resolver_an_t) >= n) {
            ਜਾਓ short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        ਜੇ (class != 1) {
            ngx_log_error(r->log_level, r->ਲਘੂਗਣਕ, 0,
                          "unexpected RR class %ui in DNS response", class);
            ਜਾਓ failed;
        }

        ਜੇ (ttl < 0) {
            ttl = 0;
        }

        rn->ttl = ngx_min(rn->ttl, (uint32_t) ttl);

        i += ਆਕਾਰ(ngx_resolver_an_t);

        ਚੋਣ (type) {

        ਹਾਲਤ NGX_RESOLVE_SRV:

            ਜੇ (i + 6 >= n) {
                ਜਾਓ short_response;
            }

            ਜੇ (ngx_resolver_copy(r, ਨਿਰਾ, buf, &buf[i + 6], buf + n)
                != NGX_OK)
            {
                ਜਾਓ failed;
            }

            nsrvs++;

            ਰੋਕੋ;

        ਹਾਲਤ NGX_RESOLVE_CNAME:

            cname = &buf[i];

            ਰੋਕੋ;

        ਹਾਲਤ NGX_RESOLVE_DNAME:

            ਰੋਕੋ;

        ਮੂਲ:

            ngx_log_error(r->log_level, r->ਲਘੂਗਣਕ, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->ਲਘੂਗਣਕ, 0,
                   "resolver nsrvs:%ui cname:%p ttl:%uD",
                   nsrvs, cname, rn->ttl);

    ਜੇ (nsrvs) {

        srvs = ngx_resolver_calloc(r, nsrvs * ਆਕਾਰ(ngx_resolver_srv_t));
        ਜੇ (srvs == ਨਿਰਾ) {
            ਜਾਓ failed;
        }

        rn->u.srvs = srvs;
        rn->nsrvs = (u_short) nsrvs;

        j = 0;
        i = ans;

        ਲਈ (a = 0; a < nan; a++) {

            ਲਈ ( ;; ) {

                ਜੇ (buf[i] & 0xc0) {
                    i += 2;
                    ਰੋਕੋ;
                }

                ਜੇ (buf[i] == 0) {
                    i++;
                    ਰੋਕੋ;
                }

                i += 1 + buf[i];
            }

            an = (ngx_resolver_an_t *) &buf[i];

            type = (an->type_hi << 8) + an->type_lo;
            len = (an->len_hi << 8) + an->len_lo;

            i += ਆਕਾਰ(ngx_resolver_an_t);

            ਜੇ (type == NGX_RESOLVE_SRV) {

                srvs[j].ਤਰਜੀਹ = (buf[i] << 8) + buf[i + 1];
                srvs[j].ਭਾਰ = (buf[i + 2] << 8) + buf[i + 3];

                ਜੇ (srvs[j].ਭਾਰ == 0) {
                    srvs[j].ਭਾਰ = 1;
                }

                srvs[j].ਪੋਰਟ = (buf[i + 4] << 8) + buf[i + 5];

                ਜੇ (ngx_resolver_copy(r, &srvs[j].name, buf, &buf[i + 6],
                                      buf + n)
                    != NGX_OK)
                {
                    ਜਾਓ failed;
                }

                j++;
            }

            i += len;
        }

        ngx_ਛਾਂਟੋ(srvs, nsrvs, ਆਕਾਰ(ngx_resolver_srv_t),
                 ngx_resolver_cmp_srvs);

        ngx_resolver_free(r, rn->query);
        rn->query = ਨਿਰਾ;

        ngx_ਕਤਾਰ_remove(&rn->ਕਤਾਰ);

        rn->valid = ngx_ਸਮਾਂ() + (r->valid ? r->valid : (ਸਮਾਂ_t) rn->ttl);
        rn->expire = ngx_ਸਮਾਂ() + r->expire;

        ngx_ਕਤਾਰ_ਪਾਓ_ਸਿਰ(&r->srv_expire_ਕਤਾਰ, &rn->ਕਤਾਰ);

        ਅਗਲਾ = rn->ਉਡੀਕ;
        rn->ਉਡੀਕ = ਨਿਰਾ;

        ਜਦੋਂ (ਅਗਲਾ) {
            ctx = ਅਗਲਾ;
            ਅਗਲਾ = ctx->ਅਗਲਾ;

            ngx_resolver_resolve_srv_ਨਾਂ(ctx, rn);
        }

        ਵਾਪਸ;
    }

    rn->nsrvs = 0;

    ਜੇ (cname) {

        /* CNAME only */

        ਜੇ (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            ਜਾਓ failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->ਲਘੂਗਣਕ, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_ਕਤਾਰ_remove(&rn->ਕਤਾਰ);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.ਅੰਕੜੇ;

        rn->valid = ngx_ਸਮਾਂ() + (r->valid ? r->valid : (ਸਮਾਂ_t) rn->ttl);
        rn->expire = ngx_ਸਮਾਂ() + r->expire;

        ngx_ਕਤਾਰ_ਪਾਓ_ਸਿਰ(&r->srv_expire_ਕਤਾਰ, &rn->ਕਤਾਰ);

        ngx_resolver_free(r, rn->query);
        rn->query = ਨਿਰਾ;
#ਜੇ (NGX_HAVE_INET6)
        rn->query6 = ਨਿਰਾ;
#ਜੇ_ਅੰਤ

        ctx = rn->ਉਡੀਕ;
        rn->ਉਡੀਕ = ਨਿਰਾ;

        ਜੇ (ctx) {

            ਜੇ (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                ਕਰੋ {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    ਅਗਲਾ = ctx->ਅਗਲਾ;

                    ctx->handler(ctx);

                    ctx = ਅਗਲਾ;
                } ਜਦੋਂ (ctx);

                ਵਾਪਸ;
            }

            ਲਈ (ਅਗਲਾ = ctx; ਅਗਲਾ; ਅਗਲਾ = ਅਗਲਾ->ਅਗਲਾ) {
                ਅਗਲਾ->ਗੰਢ = ਨਿਰਾ;
            }

            (ਖਾਲੀ) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        ਵਾਪਸ;
    }

    ngx_log_error(r->log_level, r->ਲਘੂਗਣਕ, 0, "no SRV type in DNS response");

    ਵਾਪਸ;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->ਲਘੂਗਣਕ, 0, err);

    ਵਾਪਸ;

failed:

    /* unlock name mutex */

    ਵਾਪਸ;
}


ਸਥਿਰ ਖਾਲੀ
ngx_resolver_resolve_srv_ਨਾਂ(ngx_resolver_ctx_t *ctx, ngx_resolver_ਗੰਢ_t *rn)
{
    ngx_uint_t                i;
    ngx_resolver_t           *r;
    ngx_resolver_ctx_t       *cctx;
    ngx_resolver_srv_name_t  *srvs;

    r = ctx->resolver;

    ctx->ਗੰਢ = ਨਿਰਾ;
    ctx->state = NGX_OK;
    ctx->valid = rn->valid;
    ctx->ਗਿਣਤੀ_2 = rn->nsrvs;

    srvs = ngx_resolver_calloc(r, rn->nsrvs * ਆਕਾਰ(ngx_resolver_srv_name_t));
    ਜੇ (srvs == ਨਿਰਾ) {
        ਜਾਓ failed;
    }

    ctx->srvs = srvs;
    ctx->nsrvs = rn->nsrvs;

    ਜੇ (ctx->event && ctx->event->timer_ਸੈੱਟ) {
        ngx_del_timer(ctx->event);
    }

    ਲਈ (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
        srvs[i].name.ਅੰਕੜੇ = ngx_resolver_alloc(r, rn->u.srvs[i].name.len);
        ਜੇ (srvs[i].name.ਅੰਕੜੇ == ਨਿਰਾ) {
            ਜਾਓ failed;
        }

        srvs[i].name.len = rn->u.srvs[i].name.len;
        ngx_memcpy(srvs[i].name.ਅੰਕੜੇ, rn->u.srvs[i].name.ਅੰਕੜੇ,
                   srvs[i].name.len);

        cctx = ngx_resolve_ਸ਼ੁਰੂ(r, ਨਿਰਾ);
        ਜੇ (cctx == ਨਿਰਾ) {
            ਜਾਓ failed;
        }

        cctx->name = srvs[i].name;
        cctx->handler = ngx_resolver_srv_ਨਾਂ_handler;
        cctx->ਅੰਕੜੇ = ctx;
        cctx->srvs = &srvs[i];
        cctx->timeout = ctx->timeout;

        srvs[i].ਤਰਜੀਹ = rn->u.srvs[i].ਤਰਜੀਹ;
        srvs[i].ਭਾਰ = rn->u.srvs[i].ਭਾਰ;
        srvs[i].ਪੋਰਟ = rn->u.srvs[i].ਪੋਰਟ;
        srvs[i].ctx = cctx;

        ਜੇ (ngx_resolve_name(cctx) == NGX_ERROR) {
            srvs[i].ctx = ਨਿਰਾ;
            ਜਾਓ failed;
        }
    }

    ਵਾਪਸ;

failed:

    ctx->state = NGX_ERROR;
    ctx->valid = ngx_ਸਮਾਂ() + (r->valid ? r->valid : 10);

    ctx->handler(ctx);
}


ਸਥਿਰ ਖਾਲੀ
ngx_resolver_srv_ਨਾਂ_handler(ngx_resolver_ctx_t *cctx)
{
    ngx_uint_t                i;
    ngx_addr_t               *addrs;
    ngx_resolver_t           *r;
    ngx_sockaddr_t           *sockaddr;
    ngx_resolver_ctx_t       *ctx;
    ngx_resolver_srv_name_t  *srv;

    r = cctx->resolver;
    ctx = cctx->ਅੰਕੜੇ;
    srv = cctx->srvs;

    ctx->ਗਿਣਤੀ_2--;
    ctx->async |= cctx->async;

    srv->ctx = ਨਿਰਾ;
    srv->state = cctx->state;

    ਜੇ (cctx->naddrs) {

        ctx->valid = ngx_min(ctx->valid, cctx->valid);

        addrs = ngx_resolver_calloc(r, cctx->naddrs * ਆਕਾਰ(ngx_addr_t));
        ਜੇ (addrs == ਨਿਰਾ) {
            srv->state = NGX_ERROR;
            ਜਾਓ ਪੂਰਾ;
        }

        sockaddr = ngx_resolver_alloc(r, cctx->naddrs * ਆਕਾਰ(ngx_sockaddr_t));
        ਜੇ (sockaddr == ਨਿਰਾ) {
            ngx_resolver_free(r, addrs);
            srv->state = NGX_ERROR;
            ਜਾਓ ਪੂਰਾ;
        }

        ਲਈ (i = 0; i < cctx->naddrs; i++) {
            addrs[i].sockaddr = &sockaddr[i].sockaddr;
            addrs[i].socklen = cctx->addrs[i].socklen;

            ngx_memcpy(&sockaddr[i], cctx->addrs[i].sockaddr,
                       addrs[i].socklen);

            ngx_inet_ਸੈੱਟ_ਪੋਰਟ(addrs[i].sockaddr, srv->ਪੋਰਟ);
        }

        srv->addrs = addrs;
        srv->naddrs = cctx->naddrs;
    }

ਪੂਰਾ:

    ngx_resolve_name_ਪੂਰਾ(cctx);

    ਜੇ (ctx->ਗਿਣਤੀ_2 == 0) {
        ngx_resolver_report_srv(r, ctx);
    }
}


ਸਥਿਰ ਖਾਲੀ
ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, ਮਾਪ_t n,
    ngx_uint_t ident, ngx_uint_t ਕੋਡ, ngx_uint_t nan)
{
    ਅੱਖਰ                 *err;
    ਮਾਪ_t                len;
    ਵਿੱਚ_addr_t             addr;
    int32_t               ttl;
    ngx_int_t             octet;
    ngx_str_t             name;
    ngx_uint_t            ਨਕਾਬ, type, class, qident, a, i, ਸ਼ੁਰੂ;
    ngx_ਕਤਾਰ_t          *expire_ਕਤਾਰ;
    ngx_rbtree_t         *ਰੁੱਖ;
    ngx_resolver_an_t    *an;
    ngx_resolver_ctx_t   *ctx, *ਅਗਲਾ;
    ngx_resolver_ਗੰਢ_t  *rn;
#ਜੇ (NGX_HAVE_INET6)
    uint32_t              hash;
    ngx_int_t             digit;
    ਬਣਤਰ in6_addr       addr6;
#ਜੇ_ਅੰਤ

    ਜੇ (ngx_resolver_copy(r, &name, buf,
                          buf + ਆਕਾਰ(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        ਵਾਪਸ;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->ਲਘੂਗਣਕ, 0, "resolver qs:%V", &name);

    /* AF_INET */

    addr = 0;
    i = ਆਕਾਰ(ngx_resolver_hdr_t);

    ਲਈ (ਨਕਾਬ = 0; ਨਕਾਬ < 32; ਨਕਾਬ += 8) {
        len = buf[i++];

        octet = ngx_atoi(&buf[i], len);
        ਜੇ (octet == NGX_ERROR || octet > 255) {
            ਜਾਓ invalid_ਵਿੱਚ_addr_arpa;
        }

        addr += octet << ਨਕਾਬ;
        i += len;
    }

    ਜੇ (ngx_strcasecmp(&buf[i], (u_char *) "\7in-addr\4arpa") == 0) {
        i += ਆਕਾਰ("\7in-addr\4arpa");

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        ਰੁੱਖ = &r->addr_rbtree;
        expire_ਕਤਾਰ = &r->addr_expire_ਕਤਾਰ;

        ਜਾਓ valid;
    }

invalid_ਵਿੱਚ_addr_arpa:

#ਜੇ (NGX_HAVE_INET6)

    i = ਆਕਾਰ(ngx_resolver_hdr_t);

    ਲਈ (octet = 15; octet >= 0; octet--) {
        ਜੇ (buf[i++] != '\1') {
            ਜਾਓ invalid_ip6_arpa;
        }

        digit = ngx_hextoi(&buf[i++], 1);
        ਜੇ (digit == NGX_ERROR) {
            ਜਾਓ invalid_ip6_arpa;
        }

        addr6.s6_addr[octet] = (u_char) digit;

        ਜੇ (buf[i++] != '\1') {
            ਜਾਓ invalid_ip6_arpa;
        }

        digit = ngx_hextoi(&buf[i++], 1);
        ਜੇ (digit == NGX_ERROR) {
            ਜਾਓ invalid_ip6_arpa;
        }

        addr6.s6_addr[octet] += (u_char) (digit * 16);
    }

    ਜੇ (ngx_strcasecmp(&buf[i], (u_char *) "\3ip6\4arpa") == 0) {
        i += ਆਕਾਰ("\3ip6\4arpa");

        /* lock addr mutex */

        hash = ngx_ਚੱਕਰੀ_ਜਾਂਚ_32_short(addr6.s6_addr, 16);
        rn = ngx_resolver_lookup_addr6(r, &addr6, hash);

        ਰੁੱਖ = &r->addr6_rbtree;
        expire_ਕਤਾਰ = &r->addr6_expire_ਕਤਾਰ;

        ਜਾਓ valid;
    }

invalid_ip6_arpa:
#ਜੇ_ਅੰਤ

    ngx_log_error(r->log_level, r->ਲਘੂਗਣਕ, 0,
                  "invalid in-addr.arpa or ip6.arpa name in DNS response");
    ngx_resolver_free(r, name.ਅੰਕੜੇ);
    ਵਾਪਸ;

valid:

    ਜੇ (rn == ਨਿਰਾ || rn->query == ਨਿਰਾ) {
        ngx_log_error(r->log_level, r->ਲਘੂਗਣਕ, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.ਅੰਕੜੇ);
        ਜਾਓ failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    ਜੇ (ident != qident) {
        ngx_log_error(r->log_level, r->ਲਘੂਗਣਕ, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.ਅੰਕੜੇ);
        ਜਾਓ failed;
    }

    ngx_resolver_free(r, name.ਅੰਕੜੇ);

    ਜੇ (ਕੋਡ == 0 && nan == 0) {
        ਕੋਡ = NGX_RESOLVE_NXDOMAIN;
    }

    ਜੇ (ਕੋਡ) {
        ਅਗਲਾ = rn->ਉਡੀਕ;
        rn->ਉਡੀਕ = ਨਿਰਾ;

        ngx_ਕਤਾਰ_remove(&rn->ਕਤਾਰ);

        ngx_rbtree_delete(ਰੁੱਖ, &rn->ਗੰਢ);

        /* unlock addr mutex */

        ਜਦੋਂ (ਅਗਲਾ) {
            ctx = ਅਗਲਾ;
            ctx->state = ਕੋਡ;
            ctx->valid = ngx_ਸਮਾਂ() + (r->valid ? r->valid : 10);
            ਅਗਲਾ = ctx->ਅਗਲਾ;

            ctx->handler(ctx);
        }

        ngx_resolver_free_ਗੰਢ(r, rn);

        ਵਾਪਸ;
    }

    i += ਆਕਾਰ(ngx_resolver_qs_t);

    ਲਈ (a = 0; a < nan; a++) {

        ਸ਼ੁਰੂ = i;

        ਜਦੋਂ (i < n) {

            ਜੇ (buf[i] & 0xc0) {
                i += 2;
                ਜਾਓ ਮਿਲਿਆ;
            }

            ਜੇ (buf[i] == 0) {
                i++;
                ਜਾਓ test_ਲੰਮਾਈ;
            }

            i += 1 + buf[i];
        }

        ਜਾਓ short_response;

    test_ਲੰਮਾਈ:

        ਜੇ (i - ਸ਼ੁਰੂ < 2) {
            err = "invalid name in DNS response";
            ਜਾਓ invalid;
        }

    ਮਿਲਿਆ:

        ਜੇ (i + ਆਕਾਰ(ngx_resolver_an_t) >= n) {
            ਜਾਓ short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        ਜੇ (class != 1) {
            ngx_log_error(r->log_level, r->ਲਘੂਗਣਕ, 0,
                          "unexpected RR class %ui in DNS response", class);
            ਜਾਓ failed;
        }

        ਜੇ (ttl < 0) {
            ttl = 0;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->ਲਘੂਗਣਕ, 0,
                      "resolver qt:%ui cl:%ui len:%uz",
                      type, class, len);

        i += ਆਕਾਰ(ngx_resolver_an_t);

        ਚੋਣ (type) {

        ਹਾਲਤ NGX_RESOLVE_PTR:

            ਜਾਓ ptr;

        ਹਾਲਤ NGX_RESOLVE_CNAME:

            ਰੋਕੋ;

        ਮੂਲ:

            ngx_log_error(r->log_level, r->ਲਘੂਗਣਕ, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->ਲਘੂਗਣਕ, 0,
                  "no PTR type in DNS response");
    ਵਾਪਸ;

ptr:

    ਜੇ (ngx_resolver_copy(r, &name, buf, buf + i, buf + n) != NGX_OK) {
        ਜਾਓ failed;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->ਲਘੂਗਣਕ, 0, "resolver an:%V", &name);

    ਜੇ (name.len != (ਮਾਪ_t) rn->nlen
        || ngx_strncmp(name.ਅੰਕੜੇ, rn->name, name.len) != 0)
    {
        ਜੇ (rn->nlen) {
            ngx_resolver_free(r, rn->name);
        }

        rn->nlen = (u_short) name.len;
        rn->name = name.ਅੰਕੜੇ;

        name.ਅੰਕੜੇ = ngx_resolver_dup(r, rn->name, name.len);
        ਜੇ (name.ਅੰਕੜੇ == ਨਿਰਾ) {
            ਜਾਓ failed;
        }
    }

    ngx_ਕਤਾਰ_remove(&rn->ਕਤਾਰ);

    rn->valid = ngx_ਸਮਾਂ() + (r->valid ? r->valid : ttl);
    rn->expire = ngx_ਸਮਾਂ() + r->expire;

    ngx_ਕਤਾਰ_ਪਾਓ_ਸਿਰ(expire_ਕਤਾਰ, &rn->ਕਤਾਰ);

    ਅਗਲਾ = rn->ਉਡੀਕ;
    rn->ਉਡੀਕ = ਨਿਰਾ;

    /* unlock addr mutex */

    ਜਦੋਂ (ਅਗਲਾ) {
        ctx = ਅਗਲਾ;
        ctx->state = NGX_OK;
        ctx->valid = rn->valid;
        ctx->name = name;
        ਅਗਲਾ = ctx->ਅਗਲਾ;

        ctx->handler(ctx);
    }

    ngx_resolver_free(r, name.ਅੰਕੜੇ);

    ਵਾਪਸ;

short_response:

    err = "short DNS response";

invalid:

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->ਲਘੂਗਣਕ, 0, err);

    ਵਾਪਸ;

failed:

    /* unlock addr mutex */

    ਵਾਪਸ;
}


ਸਥਿਰ ngx_resolver_ਗੰਢ_t *
ngx_resolver_lookup_name(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_ਗੰਢ_t    *ਗੰਢ, *sentinel;
    ngx_resolver_ਗੰਢ_t  *rn;

    ਗੰਢ = r->name_rbtree.ਜੜ੍ਹ;
    sentinel = r->name_rbtree.sentinel;

    ਜਦੋਂ (ਗੰਢ != sentinel) {

        ਜੇ (hash < ਗੰਢ->ਕੁੰਜੀ) {
            ਗੰਢ = ਗੰਢ->ਖੱਬਾ;
            ਜਾਰੀ;
        }

        ਜੇ (hash > ਗੰਢ->ਕੁੰਜੀ) {
            ਗੰਢ = ਗੰਢ->ਸੱਜਾ;
            ਜਾਰੀ;
        }

        /* hash == node->key */

        rn = ngx_resolver_ਗੰਢ(ਗੰਢ);

        rc = ngx_memn2cmp(name->ਅੰਕੜੇ, rn->name, name->len, rn->nlen);

        ਜੇ (rc == 0) {
            ਵਾਪਸ rn;
        }

        ਗੰਢ = (rc < 0) ? ਗੰਢ->ਖੱਬਾ : ਗੰਢ->ਸੱਜਾ;
    }

    /* not found */

    ਵਾਪਸ ਨਿਰਾ;
}


ਸਥਿਰ ngx_resolver_ਗੰਢ_t *
ngx_resolver_lookup_srv(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_ਗੰਢ_t    *ਗੰਢ, *sentinel;
    ngx_resolver_ਗੰਢ_t  *rn;

    ਗੰਢ = r->srv_rbtree.ਜੜ੍ਹ;
    sentinel = r->srv_rbtree.sentinel;

    ਜਦੋਂ (ਗੰਢ != sentinel) {

        ਜੇ (hash < ਗੰਢ->ਕੁੰਜੀ) {
            ਗੰਢ = ਗੰਢ->ਖੱਬਾ;
            ਜਾਰੀ;
        }

        ਜੇ (hash > ਗੰਢ->ਕੁੰਜੀ) {
            ਗੰਢ = ਗੰਢ->ਸੱਜਾ;
            ਜਾਰੀ;
        }

        /* hash == node->key */

        rn = ngx_resolver_ਗੰਢ(ਗੰਢ);

        rc = ngx_memn2cmp(name->ਅੰਕੜੇ, rn->name, name->len, rn->nlen);

        ਜੇ (rc == 0) {
            ਵਾਪਸ rn;
        }

        ਗੰਢ = (rc < 0) ? ਗੰਢ->ਖੱਬਾ : ਗੰਢ->ਸੱਜਾ;
    }

    /* not found */

    ਵਾਪਸ ਨਿਰਾ;
}


ਸਥਿਰ ngx_resolver_ਗੰਢ_t *
ngx_resolver_lookup_addr(ngx_resolver_t *r, ਵਿੱਚ_addr_t addr)
{
    ngx_rbtree_ਗੰਢ_t  *ਗੰਢ, *sentinel;

    ਗੰਢ = r->addr_rbtree.ਜੜ੍ਹ;
    sentinel = r->addr_rbtree.sentinel;

    ਜਦੋਂ (ਗੰਢ != sentinel) {

        ਜੇ (addr < ਗੰਢ->ਕੁੰਜੀ) {
            ਗੰਢ = ਗੰਢ->ਖੱਬਾ;
            ਜਾਰੀ;
        }

        ਜੇ (addr > ਗੰਢ->ਕੁੰਜੀ) {
            ਗੰਢ = ਗੰਢ->ਸੱਜਾ;
            ਜਾਰੀ;
        }

        /* addr == node->key */

        ਵਾਪਸ ngx_resolver_ਗੰਢ(ਗੰਢ);
    }

    /* not found */

    ਵਾਪਸ ਨਿਰਾ;
}


#ਜੇ (NGX_HAVE_INET6)

ਸਥਿਰ ngx_resolver_ਗੰਢ_t *
ngx_resolver_lookup_addr6(ngx_resolver_t *r, ਬਣਤਰ in6_addr *addr,
    uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_ਗੰਢ_t    *ਗੰਢ, *sentinel;
    ngx_resolver_ਗੰਢ_t  *rn;

    ਗੰਢ = r->addr6_rbtree.ਜੜ੍ਹ;
    sentinel = r->addr6_rbtree.sentinel;

    ਜਦੋਂ (ਗੰਢ != sentinel) {

        ਜੇ (hash < ਗੰਢ->ਕੁੰਜੀ) {
            ਗੰਢ = ਗੰਢ->ਖੱਬਾ;
            ਜਾਰੀ;
        }

        ਜੇ (hash > ਗੰਢ->ਕੁੰਜੀ) {
            ਗੰਢ = ਗੰਢ->ਸੱਜਾ;
            ਜਾਰੀ;
        }

        /* hash == node->key */

        rn = ngx_resolver_ਗੰਢ(ਗੰਢ);

        rc = ngx_memcmp(addr, &rn->addr6, 16);

        ਜੇ (rc == 0) {
            ਵਾਪਸ rn;
        }

        ਗੰਢ = (rc < 0) ? ਗੰਢ->ਖੱਬਾ : ਗੰਢ->ਸੱਜਾ;
    }

    /* not found */

    ਵਾਪਸ ਨਿਰਾ;
}

#ਜੇ_ਅੰਤ


ਸਥਿਰ ਖਾਲੀ
ngx_resolver_rbtree_ਪਾਓ_ਮੁੱਲ(ngx_rbtree_ਗੰਢ_t *ਆਰਜ਼ੀ,
    ngx_rbtree_ਗੰਢ_t *ਗੰਢ, ngx_rbtree_ਗੰਢ_t *sentinel)
{
    ngx_rbtree_ਗੰਢ_t    **p;
    ngx_resolver_ਗੰਢ_t   *rn, *rn_ਆਰਜ਼ੀ;

    ਲਈ ( ;; ) {

        ਜੇ (ਗੰਢ->ਕੁੰਜੀ < ਆਰਜ਼ੀ->ਕੁੰਜੀ) {

            p = &ਆਰਜ਼ੀ->ਖੱਬਾ;

        } ਨਹੀਂਤਾਂ ਜੇ (ਗੰਢ->ਕੁੰਜੀ > ਆਰਜ਼ੀ->ਕੁੰਜੀ) {

            p = &ਆਰਜ਼ੀ->ਸੱਜਾ;

        } ਨਹੀਂਤਾਂ { /* node->key == temp->key */

            rn = ngx_resolver_ਗੰਢ(ਗੰਢ);
            rn_ਆਰਜ਼ੀ = ngx_resolver_ਗੰਢ(ਆਰਜ਼ੀ);

            p = (ngx_memn2cmp(rn->name, rn_ਆਰਜ਼ੀ->name, rn->nlen, rn_ਆਰਜ਼ੀ->nlen)
                 < 0) ? &ਆਰਜ਼ੀ->ਖੱਬਾ : &ਆਰਜ਼ੀ->ਸੱਜਾ;
        }

        ਜੇ (*p == sentinel) {
            ਰੋਕੋ;
        }

        ਆਰਜ਼ੀ = *p;
    }

    *p = ਗੰਢ;
    ਗੰਢ->ਮਾਪਾ = ਆਰਜ਼ੀ;
    ਗੰਢ->ਖੱਬਾ = sentinel;
    ਗੰਢ->ਸੱਜਾ = sentinel;
    ngx_rbt_red(ਗੰਢ);
}


#ਜੇ (NGX_HAVE_INET6)

ਸਥਿਰ ਖਾਲੀ
ngx_resolver_rbtree_ਪਾਓ_addr6_ਮੁੱਲ(ngx_rbtree_ਗੰਢ_t *ਆਰਜ਼ੀ,
    ngx_rbtree_ਗੰਢ_t *ਗੰਢ, ngx_rbtree_ਗੰਢ_t *sentinel)
{
    ngx_rbtree_ਗੰਢ_t    **p;
    ngx_resolver_ਗੰਢ_t   *rn, *rn_ਆਰਜ਼ੀ;

    ਲਈ ( ;; ) {

        ਜੇ (ਗੰਢ->ਕੁੰਜੀ < ਆਰਜ਼ੀ->ਕੁੰਜੀ) {

            p = &ਆਰਜ਼ੀ->ਖੱਬਾ;

        } ਨਹੀਂਤਾਂ ਜੇ (ਗੰਢ->ਕੁੰਜੀ > ਆਰਜ਼ੀ->ਕੁੰਜੀ) {

            p = &ਆਰਜ਼ੀ->ਸੱਜਾ;

        } ਨਹੀਂਤਾਂ { /* node->key == temp->key */

            rn = ngx_resolver_ਗੰਢ(ਗੰਢ);
            rn_ਆਰਜ਼ੀ = ngx_resolver_ਗੰਢ(ਆਰਜ਼ੀ);

            p = (ngx_memcmp(&rn->addr6, &rn_ਆਰਜ਼ੀ->addr6, 16)
                 < 0) ? &ਆਰਜ਼ੀ->ਖੱਬਾ : &ਆਰਜ਼ੀ->ਸੱਜਾ;
        }

        ਜੇ (*p == sentinel) {
            ਰੋਕੋ;
        }

        ਆਰਜ਼ੀ = *p;
    }

    *p = ਗੰਢ;
    ਗੰਢ->ਮਾਪਾ = ਆਰਜ਼ੀ;
    ਗੰਢ->ਖੱਬਾ = sentinel;
    ਗੰਢ->ਸੱਜਾ = sentinel;
    ngx_rbt_red(ਗੰਢ);
}

#ਜੇ_ਅੰਤ


ਸਥਿਰ ngx_int_t
ngx_resolver_create_name_query(ngx_resolver_t *r, ngx_resolver_ਗੰਢ_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    ਮਾਪ_t               len, nlen;
    ngx_uint_t           ident;
    ngx_resolver_qs_t   *qs;
    ngx_resolver_hdr_t  *query;

    nlen = name->len ? (1 + name->len + 1) : 1;

    len = ਆਕਾਰ(ngx_resolver_hdr_t) + nlen + ਆਕਾਰ(ngx_resolver_qs_t);

#ਜੇ (NGX_HAVE_INET6)
    p = ngx_resolver_alloc(r, len * (r->ipv4 + r->ipv6));
#ਨਹੀਂਤਾਂ
    p = ngx_resolver_alloc(r, len);
#ਜੇ_ਅੰਤ
    ਜੇ (p == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    rn->qlen = (u_short) len;
    rn->query = p;

#ਜੇ (NGX_HAVE_INET6)
    ਜੇ (r->ipv6) {
        rn->query6 = r->ipv4 ? (p + len) : p;
    }
#ਜੇ_ਅੰਤ

    query = (ngx_resolver_hdr_t *) p;

    ਜੇ (r->ipv4) {
        ident = ngx_random();

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->ਲਘੂਗਣਕ, 0,
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

    p += ਆਕਾਰ(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    /* query type */
    qs->type_hi = 0; qs->type_lo = NGX_RESOLVE_A;

    /* IN query class */
    qs->class_hi = 0; qs->class_lo = 1;

    /* convert "www.example.com" to "\3www\7example\3com\0" */

    len = 0;
    p--;
    *p-- = '\0';

    ਜੇ (name->len == 0)  {
        ਵਾਪਸ NGX_DECLINED;
    }

    ਲਈ (s = name->ਅੰਕੜੇ + name->len - 1; s >= name->ਅੰਕੜੇ; s--) {
        ਜੇ (*s != '.') {
            *p = *s;
            len++;

        } ਨਹੀਂਤਾਂ {
            ਜੇ (len == 0 || len > 255) {
                ਵਾਪਸ NGX_DECLINED;
            }

            *p = (u_char) len;
            len = 0;
        }

        p--;
    }

    ਜੇ (len == 0 || len > 255) {
        ਵਾਪਸ NGX_DECLINED;
    }

    *p = (u_char) len;

#ਜੇ (NGX_HAVE_INET6)
    ਜੇ (!r->ipv6) {
        ਵਾਪਸ NGX_OK;
    }

    p = rn->query6;

    ਜੇ (r->ipv4) {
        ngx_memcpy(p, rn->query, rn->qlen);
    }

    query = (ngx_resolver_hdr_t *) p;

    ident = ngx_random();

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->ਲਘੂਗਣਕ, 0,
                   "resolve: \"%V\" AAAA %i", name, ident & 0xffff);

    query->ident_hi = (u_char) ((ident >> 8) & 0xff);
    query->ident_lo = (u_char) (ident & 0xff);

    p += ਆਕਾਰ(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    qs->type_lo = NGX_RESOLVE_AAAA;
#ਜੇ_ਅੰਤ

    ਵਾਪਸ NGX_OK;
}


ਸਥਿਰ ngx_int_t
ngx_resolver_create_srv_query(ngx_resolver_t *r, ngx_resolver_ਗੰਢ_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    ਮਾਪ_t               len, nlen;
    ngx_uint_t           ident;
    ngx_resolver_qs_t   *qs;
    ngx_resolver_hdr_t  *query;

    nlen = name->len ? (1 + name->len + 1) : 1;

    len = ਆਕਾਰ(ngx_resolver_hdr_t) + nlen + ਆਕਾਰ(ngx_resolver_qs_t);

    p = ngx_resolver_alloc(r, len);
    ਜੇ (p == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    rn->qlen = (u_short) len;
    rn->query = p;

    query = (ngx_resolver_hdr_t *) p;

    ident = ngx_random();

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->ਲਘੂਗਣਕ, 0,
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

    p += ਆਕਾਰ(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    /* query type */
    qs->type_hi = 0; qs->type_lo = NGX_RESOLVE_SRV;

    /* IN query class */
    qs->class_hi = 0; qs->class_lo = 1;

    /* converts "www.example.com" to "\3www\7example\3com\0" */

    len = 0;
    p--;
    *p-- = '\0';

    ਜੇ (name->len == 0)  {
        ਵਾਪਸ NGX_DECLINED;
    }

    ਲਈ (s = name->ਅੰਕੜੇ + name->len - 1; s >= name->ਅੰਕੜੇ; s--) {
        ਜੇ (*s != '.') {
            *p = *s;
            len++;

        } ਨਹੀਂਤਾਂ {
            ਜੇ (len == 0 || len > 255) {
                ਵਾਪਸ NGX_DECLINED;
            }

            *p = (u_char) len;
            len = 0;
        }

        p--;
    }

    ਜੇ (len == 0 || len > 255) {
        ਵਾਪਸ NGX_DECLINED;
    }

    *p = (u_char) len;

    ਵਾਪਸ NGX_OK;
}


ਸਥਿਰ ngx_int_t
ngx_resolver_create_addr_query(ngx_resolver_t *r, ngx_resolver_ਗੰਢ_t *rn,
    ngx_resolver_addr_t *addr)
{
    u_char               *p, *d;
    ਮਾਪ_t                len;
    ਵਿੱਚ_addr_t             inaddr;
    ngx_int_t             n;
    ngx_uint_t            ident;
    ngx_resolver_hdr_t   *query;
    ਬਣਤਰ sockaddr_ਵਿੱਚ   *ਜੀਵਾ;
#ਜੇ (NGX_HAVE_INET6)
    ਬਣਤਰ sockaddr_in6  *sin6;
#ਜੇ_ਅੰਤ

    ਚੋਣ (addr->sockaddr->sa_family) {

#ਜੇ (NGX_HAVE_INET6)
    ਹਾਲਤ AF_INET6:
        len = ਆਕਾਰ(ngx_resolver_hdr_t)
              + 64 + ਆਕਾਰ(".ip6.arpa.") - 1
              + ਆਕਾਰ(ngx_resolver_qs_t);

        ਰੋਕੋ;
#ਜੇ_ਅੰਤ

    ਮੂਲ: /* AF_INET */
        len = ਆਕਾਰ(ngx_resolver_hdr_t)
              + ਆਕਾਰ(".255.255.255.255.in-addr.arpa.") - 1
              + ਆਕਾਰ(ngx_resolver_qs_t);
    }

    p = ngx_resolver_alloc(r, len);
    ਜੇ (p == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
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

    p += ਆਕਾਰ(ngx_resolver_hdr_t);

    ਚੋਣ (addr->sockaddr->sa_family) {

#ਜੇ (NGX_HAVE_INET6)
    ਹਾਲਤ AF_INET6:
        sin6 = (ਬਣਤਰ sockaddr_in6 *) addr->sockaddr;

        ਲਈ (n = 15; n >= 0; n--) {
            p = ngx_sprintf(p, "\1%xd\1%xd",
                            sin6->sin6_addr.s6_addr[n] & 0xf,
                            (sin6->sin6_addr.s6_addr[n] >> 4) & 0xf);
        }

        p = ngx_cpymem(p, "\3ip6\4arpa\0", 10);

        ਰੋਕੋ;
#ਜੇ_ਅੰਤ

    ਮੂਲ: /* AF_INET */

        ਜੀਵਾ = (ਬਣਤਰ sockaddr_ਵਿੱਚ *) addr->sockaddr;
        inaddr = ntohl(ਜੀਵਾ->sin_addr.s_addr);

        ਲਈ (n = 0; n < 32; n += 8) {
            d = ngx_sprintf(&p[1], "%ud", (inaddr >> n) & 0xff);
            *p = (u_char) (d - &p[1]);
            p = d;
        }

        p = ngx_cpymem(p, "\7in-addr\4arpa\0", 14);
    }

    /* query type "PTR", IN query class */
    p = ngx_cpymem(p, "\0\14\0\1", 4);

    rn->qlen = (u_short) (p - rn->query);

    ਵਾਪਸ NGX_OK;
}


ਸਥਿਰ ngx_int_t
ngx_resolver_copy(ngx_resolver_t *r, ngx_str_t *name, u_char *buf, u_char *src,
    u_char *ਆਖਰੀ)
{
    ਅੱਖਰ        *err;
    u_char      *p, *dst;
    ਮਾਪ_t       len;
    ngx_uint_t   i, n;

    p = src;
    len = 0;

    /*
     * compression pointers allow to create endless loop, so we set limit;
     * 128 pointers should be enough to store 255-byte name
     */

    ਲਈ (i = 0; i < 128; i++) {
        n = *p++;

        ਜੇ (n == 0) {
            ਜਾਓ ਪੂਰਾ;
        }

        ਜੇ (n & 0xc0) {
            ਜੇ ((n & 0xc0) != 0xc0) {
                err = "invalid label type in DNS response";
                ਜਾਓ invalid;
            }

            ਜੇ (p >= ਆਖਰੀ) {
                err = "name is out of DNS response";
                ਜਾਓ invalid;
            }

            n = ((n & 0x3f) << 8) + *p;
            p = &buf[n];

        } ਨਹੀਂਤਾਂ {
            len += 1 + n;
            p = &p[n];
        }

        ਜੇ (p >= ਆਖਰੀ) {
            err = "name is out of DNS response";
            ਜਾਓ invalid;
        }
    }

    err = "compression pointers loop in DNS response";

invalid:

    ngx_log_error(r->log_level, r->ਲਘੂਗਣਕ, 0, err);

    ਵਾਪਸ NGX_ERROR;

ਪੂਰਾ:

    ਜੇ (name == ਨਿਰਾ) {
        ਵਾਪਸ NGX_OK;
    }

    ਜੇ (len == 0) {
        ngx_str_null(name);
        ਵਾਪਸ NGX_OK;
    }

    dst = ngx_resolver_alloc(r, len);
    ਜੇ (dst == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    name->ਅੰਕੜੇ = dst;

    ਲਈ ( ;; ) {
        n = *src++;

        ਜੇ (n == 0) {
            name->len = dst - name->ਅੰਕੜੇ - 1;
            ਵਾਪਸ NGX_OK;
        }

        ਜੇ (n & 0xc0) {
            n = ((n & 0x3f) << 8) + *src;
            src = &buf[n];

        } ਨਹੀਂਤਾਂ {
            ngx_strlow(dst, src, n);
            dst += n;
            src += n;
            *dst++ = '.';
        }
    }
}


ਸਥਿਰ ngx_int_t
ngx_resolver_ਸੈੱਟ_timeout(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx)
{
    ਜੇ (ctx->event || ctx->timeout == 0) {
        ਵਾਪਸ NGX_OK;
    }

    ctx->event = ngx_resolver_calloc(r, ਆਕਾਰ(ngx_event_t));
    ਜੇ (ctx->event == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    ctx->event->handler = ngx_resolver_timeout_handler;
    ctx->event->ਅੰਕੜੇ = ctx;
    ctx->event->ਲਘੂਗਣਕ = r->ਲਘੂਗਣਕ;
    ctx->event->cancelable = ctx->cancelable;
    ctx->ident = -1;

    ngx_ਜੋੜੋ_timer(ctx->event, ctx->timeout);

    ਵਾਪਸ NGX_OK;
}


ਸਥਿਰ ਖਾਲੀ
ngx_resolver_timeout_handler(ngx_event_t *ev)
{
    ngx_resolver_ctx_t  *ctx;

    ctx = ev->ਅੰਕੜੇ;

    ctx->state = NGX_RESOLVE_TIMEDOUT;

    ctx->handler(ctx);
}


ਸਥਿਰ ਖਾਲੀ
ngx_resolver_free_ਗੰਢ(ngx_resolver_t *r, ngx_resolver_ਗੰਢ_t *rn)
{
    ngx_uint_t  i;

    /* lock alloc mutex */

    ਜੇ (rn->query) {
        ngx_resolver_free_locked(r, rn->query);
    }

    ਜੇ (rn->name) {
        ngx_resolver_free_locked(r, rn->name);
    }

    ਜੇ (rn->cnlen) {
        ngx_resolver_free_locked(r, rn->u.cname);
    }

    ਜੇ (rn->naddrs > 1 && rn->naddrs != (u_short) -1) {
        ngx_resolver_free_locked(r, rn->u.addrs);
    }

#ਜੇ (NGX_HAVE_INET6)
    ਜੇ (rn->naddrs6 > 1 && rn->naddrs6 != (u_short) -1) {
        ngx_resolver_free_locked(r, rn->u6.addrs6);
    }
#ਜੇ_ਅੰਤ

    ਜੇ (rn->nsrvs) {
        ਲਈ (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
            ਜੇ (rn->u.srvs[i].name.ਅੰਕੜੇ) {
                ngx_resolver_free_locked(r, rn->u.srvs[i].name.ਅੰਕੜੇ);
            }
        }

        ngx_resolver_free_locked(r, rn->u.srvs);
    }

    ngx_resolver_free_locked(r, rn);

    /* unlock alloc mutex */
}


ਸਥਿਰ ਖਾਲੀ *
ngx_resolver_alloc(ngx_resolver_t *r, ਮਾਪ_t ਮਾਪ)
{
    u_char  *p;

    /* lock alloc mutex */

    p = ngx_alloc(ਮਾਪ, r->ਲਘੂਗਣਕ);

    /* unlock alloc mutex */

    ਵਾਪਸ p;
}


ਸਥਿਰ ਖਾਲੀ *
ngx_resolver_calloc(ngx_resolver_t *r, ਮਾਪ_t ਮਾਪ)
{
    u_char  *p;

    p = ngx_resolver_alloc(r, ਮਾਪ);

    ਜੇ (p) {
        ngx_memzero(p, ਮਾਪ);
    }

    ਵਾਪਸ p;
}


ਸਥਿਰ ਖਾਲੀ
ngx_resolver_free(ngx_resolver_t *r, ਖਾਲੀ *p)
{
    /* lock alloc mutex */

    ngx_free(p);

    /* unlock alloc mutex */
}


ਸਥਿਰ ਖਾਲੀ
ngx_resolver_free_locked(ngx_resolver_t *r, ਖਾਲੀ *p)
{
    ngx_free(p);
}


ਸਥਿਰ ਖਾਲੀ *
ngx_resolver_dup(ngx_resolver_t *r, ਖਾਲੀ *src, ਮਾਪ_t ਮਾਪ)
{
    ਖਾਲੀ  *dst;

    dst = ngx_resolver_alloc(r, ਮਾਪ);

    ਜੇ (dst == ਨਿਰਾ) {
        ਵਾਪਸ dst;
    }

    ngx_memcpy(dst, src, ਮਾਪ);

    ਵਾਪਸ dst;
}


ਸਥਿਰ ngx_resolver_addr_t *
ngx_resolver_export(ngx_resolver_t *r, ngx_resolver_ਗੰਢ_t *rn,
    ngx_uint_t rotate)
{
    ngx_uint_t            d, i, j, n;
    ਵਿੱਚ_addr_t            *addr;
    ngx_sockaddr_t       *sockaddr;
    ਬਣਤਰ sockaddr_ਵਿੱਚ   *ਜੀਵਾ;
    ngx_resolver_addr_t  *dst;
#ਜੇ (NGX_HAVE_INET6)
    ਬਣਤਰ in6_addr      *addr6;
    ਬਣਤਰ sockaddr_in6  *sin6;
#ਜੇ_ਅੰਤ

    n = rn->naddrs;
#ਜੇ (NGX_HAVE_INET6)
    n += rn->naddrs6;
#ਜੇ_ਅੰਤ

    dst = ngx_resolver_calloc(r, n * ਆਕਾਰ(ngx_resolver_addr_t));
    ਜੇ (dst == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    sockaddr = ngx_resolver_calloc(r, n * ਆਕਾਰ(ngx_sockaddr_t));
    ਜੇ (sockaddr == ਨਿਰਾ) {
        ngx_resolver_free(r, dst);
        ਵਾਪਸ ਨਿਰਾ;
    }

    i = 0;
    d = rotate ? ngx_random() % n : 0;

    ਜੇ (rn->naddrs) {
        j = rotate ? ngx_random() % rn->naddrs : 0;

        addr = (rn->naddrs == 1) ? &rn->u.addr : rn->u.addrs;

        ਕਰੋ {
            ਜੀਵਾ = &sockaddr[d].sockaddr_ਵਿੱਚ;
            ਜੀਵਾ->sin_family = AF_INET;
            ਜੀਵਾ->sin_addr.s_addr = addr[j++];
            dst[d].sockaddr = (ਬਣਤਰ sockaddr *) ਜੀਵਾ;
            dst[d++].socklen = ਆਕਾਰ(ਬਣਤਰ sockaddr_ਵਿੱਚ);

            ਜੇ (d == n) {
                d = 0;
            }

            ਜੇ (j == (ngx_uint_t) rn->naddrs) {
                j = 0;
            }
        } ਜਦੋਂ (++i < (ngx_uint_t) rn->naddrs);
    }

#ਜੇ (NGX_HAVE_INET6)
    ਜੇ (rn->naddrs6) {
        j = rotate ? ngx_random() % rn->naddrs6 : 0;

        addr6 = (rn->naddrs6 == 1) ? &rn->u6.addr6 : rn->u6.addrs6;

        ਕਰੋ {
            sin6 = &sockaddr[d].sockaddr_in6;
            sin6->sin6_family = AF_INET6;
            ngx_memcpy(sin6->sin6_addr.s6_addr, addr6[j++].s6_addr, 16);
            dst[d].sockaddr = (ਬਣਤਰ sockaddr *) sin6;
            dst[d++].socklen = ਆਕਾਰ(ਬਣਤਰ sockaddr_in6);

            ਜੇ (d == n) {
                d = 0;
            }

            ਜੇ (j == rn->naddrs6) {
                j = 0;
            }
        } ਜਦੋਂ (++i < n);
    }
#ਜੇ_ਅੰਤ

    ਵਾਪਸ dst;
}


ਸਥਿਰ ਖਾਲੀ
ngx_resolver_report_srv(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx)
{
    ngx_uint_t                naddrs, nsrvs, nw, i, j, k, l, m, n, w;
    ngx_resolver_addr_t      *addrs;
    ngx_resolver_srv_name_t  *srvs;

    srvs = ctx->srvs;
    nsrvs = ctx->nsrvs;

    naddrs = 0;

    ਲਈ (i = 0; i < nsrvs; i++) {
        ਜੇ (srvs[i].state == NGX_ERROR) {
            ctx->state = NGX_ERROR;
            ctx->valid = ngx_ਸਮਾਂ() + (r->valid ? r->valid : 10);

            ctx->handler(ctx);
            ਵਾਪਸ;
        }

        naddrs += srvs[i].naddrs;
    }

    ਜੇ (naddrs == 0) {
        ctx->state = srvs[0].state;

        ਲਈ (i = 0; i < nsrvs; i++) {
            ਜੇ (srvs[i].state == NGX_RESOLVE_NXDOMAIN) {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                ਰੋਕੋ;
            }
        }

        ctx->valid = ngx_ਸਮਾਂ() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        ਵਾਪਸ;
    }

    addrs = ngx_resolver_calloc(r, naddrs * ਆਕਾਰ(ngx_resolver_addr_t));
    ਜੇ (addrs == ਨਿਰਾ) {
        ctx->state = NGX_ERROR;
        ctx->valid = ngx_ਸਮਾਂ() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        ਵਾਪਸ;
    }

    i = 0;
    n = 0;

    ਕਰੋ {
        nw = 0;

        ਲਈ (j = i; j < nsrvs; j++) {
            ਜੇ (srvs[j].ਤਰਜੀਹ != srvs[i].ਤਰਜੀਹ) {
                ਰੋਕੋ;
            }

            nw += srvs[j].naddrs * srvs[j].ਭਾਰ;
        }

        ਜੇ (nw == 0) {
            ਜਾਓ ਅਗਲਾ_srv;
        }

        w = ngx_random() % nw;

        ਲਈ (k = i; k < j; k++) {
            ਜੇ (w < srvs[k].naddrs * srvs[k].ਭਾਰ) {
                ਰੋਕੋ;
            }

            w -= srvs[k].naddrs * srvs[k].ਭਾਰ;
        }

        ਲਈ (l = i; l < j; l++) {

            ਲਈ (m = 0; m < srvs[k].naddrs; m++) {
                addrs[n].socklen = srvs[k].addrs[m].socklen;
                addrs[n].sockaddr = srvs[k].addrs[m].sockaddr;
                addrs[n].name = srvs[k].name;
                addrs[n].ਤਰਜੀਹ = srvs[k].ਤਰਜੀਹ;
                addrs[n].ਭਾਰ = srvs[k].ਭਾਰ;
                n++;
            }

            ਜੇ (++k == j) {
                k = i;
            }
        }

ਅਗਲਾ_srv:

        i = j;

    } ਜਦੋਂ (i < ctx->nsrvs);

    ctx->state = NGX_OK;
    ctx->addrs = addrs;
    ctx->naddrs = naddrs;

    ctx->handler(ctx);

    ngx_resolver_free(r, addrs);
}


ਅੱਖਰ *
ngx_resolver_strerror(ngx_int_t err)
{
    ਸਥਿਰ ਅੱਖਰ *errors[] = {
        "Format error",     /* FORMERR */
        "Server failure",   /* SERVFAIL */
        "Host not found",   /* NXDOMAIN */
        "Unimplemented",    /* NOTIMP */
        "Operation refused" /* REFUSED */
    };

    ਜੇ (err > 0 && err < 6) {
        ਵਾਪਸ errors[err - 1];
    }

    ਜੇ (err == NGX_RESOLVE_TIMEDOUT) {
        ਵਾਪਸ "Operation timed out";
    }

    ਵਾਪਸ "Unknown error";
}


ਸਥਿਰ u_char *
ngx_resolver_log_error(ngx_log_t *ਲਘੂਗਣਕ, u_char *buf, ਮਾਪ_t len)
{
    u_char                     *p;
    ngx_resolver_connection_t  *rec;

    p = buf;

    ਜੇ (ਲਘੂਗਣਕ->action) {
        p = ngx_snprintf(buf, len, " while %s", ਲਘੂਗਣਕ->action);
        len -= p - buf;
    }

    rec = ਲਘੂਗਣਕ->ਅੰਕੜੇ;

    ਜੇ (rec) {
        p = ngx_snprintf(p, len, ", resolver: %V", &rec->server);
    }

    ਵਾਪਸ p;
}


ਸਥਿਰ ngx_int_t
ngx_udp_connect(ngx_resolver_connection_t *rec)
{
    ਪੂਰਨ                rc;
    ngx_int_t          event;
    ngx_event_t       *rev, *wev;
    ngx_socket_t       s;
    ngx_connection_t  *c;

    s = ngx_socket(rec->sockaddr->sa_family, SOCK_DGRAM, 0);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->ਲਘੂਗਣਕ, 0, "UDP socket %d", s);

    ਜੇ (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->ਲਘੂਗਣਕ, ngx_socket_errno,
                      ngx_socket_n " failed");
        ਵਾਪਸ NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->ਲਘੂਗਣਕ);

    ਜੇ (c == ਨਿਰਾ) {
        ਜੇ (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->ਲਘੂਗਣਕ, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        ਵਾਪਸ NGX_ERROR;
    }

    ਜੇ (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->ਲਘੂਗਣਕ, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        ਜਾਓ failed;
    }

    rev = c->ਪਠ;
    wev = c->ਲਿਖ;

    rev->ਲਘੂਗਣਕ = &rec->ਲਘੂਗਣਕ;
    wev->ਲਘੂਗਣਕ = &rec->ਲਘੂਗਣਕ;

    rec->udp = c;

    c->ਸੰਖਿਆ = ngx_atomic_fetch_ਜੋੜੋ(ngx_connection_counter, 1);

    c->ਸ਼ੁਰੂ_ਸਮਾਂ = ngx_current_msec;

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->ਲਘੂਗਣਕ, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->ਸੰਖਿਆ);

    rc = connect(s, rec->sockaddr, rec->socklen);

    ਜੇ (rc == -1) {
        ngx_log_error(NGX_LOG_CRIT, &rec->ਲਘੂਗਣਕ, ngx_socket_errno,
                      "connect() failed");

        ਜਾਓ failed;
    }

    /* UDP sockets are always ready to write */
    wev->ready = 1;

    event = (ngx_event_flags & NGX_USE_CLEAR_EVENT) ?
                /* kqueue, epoll */                 NGX_CLEAR_EVENT:
                /* select, poll, /dev/poll */       NGX_LEVEL_EVENT;
                /* eventport event type has no meaning: oneshot only */

    ਜੇ (ngx_ਜੋੜੋ_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        ਜਾਓ failed;
    }

    ਵਾਪਸ NGX_OK;

failed:

    ngx_close_connection(c);
    rec->udp = ਨਿਰਾ;

    ਵਾਪਸ NGX_ERROR;
}


ਸਥਿਰ ngx_int_t
ngx_tcp_connect(ngx_resolver_connection_t *rec)
{
    ਪੂਰਨ                rc;
    ngx_int_t          event;
    ngx_err_t          err;
    ngx_uint_t         level;
    ngx_socket_t       s;
    ngx_event_t       *rev, *wev;
    ngx_connection_t  *c;

    s = ngx_socket(rec->sockaddr->sa_family, SOCK_ਧਾਰਾ, 0);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->ਲਘੂਗਣਕ, 0, "TCP socket %d", s);

    ਜੇ (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->ਲਘੂਗਣਕ, ngx_socket_errno,
                      ngx_socket_n " failed");
        ਵਾਪਸ NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->ਲਘੂਗਣਕ);

    ਜੇ (c == ਨਿਰਾ) {
        ਜੇ (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->ਲਘੂਗਣਕ, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        ਵਾਪਸ NGX_ERROR;
    }

    ਜੇ (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->ਲਘੂਗਣਕ, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        ਜਾਓ failed;
    }

    rev = c->ਪਠ;
    wev = c->ਲਿਖ;

    rev->ਲਘੂਗਣਕ = &rec->ਲਘੂਗਣਕ;
    wev->ਲਘੂਗਣਕ = &rec->ਲਘੂਗਣਕ;

    rec->tcp = c;

    c->ਸੰਖਿਆ = ngx_atomic_fetch_ਜੋੜੋ(ngx_connection_counter, 1);

    c->ਸ਼ੁਰੂ_ਸਮਾਂ = ngx_current_msec;

    ਜੇ (ngx_ਜੋੜੋ_conn) {
        ਜੇ (ngx_ਜੋੜੋ_conn(c) == NGX_ERROR) {
            ਜਾਓ failed;
        }
    }

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->ਲਘੂਗਣਕ, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->ਸੰਖਿਆ);

    rc = connect(s, rec->sockaddr, rec->socklen);

    ਜੇ (rc == -1) {
        err = ngx_socket_errno;


        ਜੇ (err != NGX_EINPROGRESS
#ਜੇ (NGX_WIN32)
            /* Winsock returns WSAEWOULDBLOCK (NGX_EAGAIN) */
            && err != NGX_EAGAIN
#ਜੇ_ਅੰਤ
            )
        {
            ਜੇ (err == NGX_ECONNREFUSED
#ਜੇ (NGX_LINUX)
                /*
                 * Linux returns EAGAIN instead of ECONNREFUSED
                 * for unix sockets if listen queue is full
                 */
                || err == NGX_EAGAIN
#ਜੇ_ਅੰਤ
                || err == NGX_ECONNRESET
                || err == NGX_ENETDOWN
                || err == NGX_ENETUNREACH
                || err == NGX_EHOSTDOWN
                || err == NGX_EHOSTUNREACH)
            {
                level = NGX_LOG_ERR;

            } ਨਹੀਂਤਾਂ {
                level = NGX_LOG_CRIT;
            }

            ngx_log_error(level, &rec->ਲਘੂਗਣਕ, err, "connect() to %V failed",
                          &rec->server);

            ngx_close_connection(c);
            rec->tcp = ਨਿਰਾ;

            ਵਾਪਸ NGX_ERROR;
        }
    }

    ਜੇ (ngx_ਜੋੜੋ_conn) {
        ਜੇ (rc == -1) {

            /* NGX_EINPROGRESS */

            ਵਾਪਸ NGX_AGAIN;
        }

        ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->ਲਘੂਗਣਕ, 0, "connected");

        wev->ready = 1;

        ਵਾਪਸ NGX_OK;
    }

    ਜੇ (ngx_event_flags & NGX_USE_CLEAR_EVENT) {

        /* kqueue */

        event = NGX_CLEAR_EVENT;

    } ਨਹੀਂਤਾਂ {

        /* select, poll, /dev/poll */

        event = NGX_LEVEL_EVENT;
    }

    ਜੇ (ngx_ਜੋੜੋ_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        ਜਾਓ failed;
    }

    ਜੇ (rc == -1) {

        /* NGX_EINPROGRESS */

        ਜੇ (ngx_ਜੋੜੋ_event(wev, NGX_WRITE_EVENT, event) != NGX_OK) {
            ਜਾਓ failed;
        }

        ਵਾਪਸ NGX_AGAIN;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->ਲਘੂਗਣਕ, 0, "connected");

    wev->ready = 1;

    ਵਾਪਸ NGX_OK;

failed:

    ngx_close_connection(c);
    rec->tcp = ਨਿਰਾ;

    ਵਾਪਸ NGX_ERROR;
}


ਸਥਿਰ ngx_int_t
ngx_resolver_cmp_srvs(ਅਚਲ ਖਾਲੀ *one, ਅਚਲ ਖਾਲੀ *two)
{
    ngx_int_t            p1, p2;
    ngx_resolver_srv_t  *ਪਹਿਲਾ, *ਦੂਜਾ;

    ਪਹਿਲਾ = (ngx_resolver_srv_t *) one;
    ਦੂਜਾ = (ngx_resolver_srv_t *) two;

    p1 = ਪਹਿਲਾ->ਤਰਜੀਹ;
    p2 = ਦੂਜਾ->ਤਰਜੀਹ;

    ਵਾਪਸ p1 - p2;
}
