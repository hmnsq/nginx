
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>
#ಅನ್ತರ್ಭಾವ <ngx_event.h>


#ವ್ಯಾಖ್ಯಾನಿಸು NGX_RESOLVER_UDP_ಗಾತ್ರ   4096

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_RESOLVER_TCP_RSIZE  (2 + 65535)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_RESOLVER_TCP_WSIZE  8192


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
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


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    u_char  type_hi;
    u_char  type_lo;
    u_char  class_hi;
    u_char  class_lo;
} ngx_resolver_qs_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    u_char  type_hi;
    u_char  type_lo;
    u_char  class_hi;
    u_char  class_lo;
    u_char  ttl[4];
    u_char  len_hi;
    u_char  len_lo;
} ngx_resolver_an_t;


#ವ್ಯಾಖ್ಯಾನಿಸು ngx_resolver_ಗಂಟು(n)  ngx_rbtree_ದತ್ತಾಂಶ(n, ngx_resolver_ಗಂಟು_t, ಗಂಟು)


ಸ್ಥಿರ ngx_int_t ngx_udp_connect(ngx_resolver_connection_t *rec);
ಸ್ಥಿರ ngx_int_t ngx_tcp_connect(ngx_resolver_connection_t *rec);


ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_cleanup(ಶೂನ್ಯ *ದತ್ತಾಂಶ);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_cleanup_ವೃಕ್ಷ(ngx_resolver_t *r, ngx_rbtree_t *ವೃಕ್ಷ);
ಸ್ಥಿರ ngx_int_t ngx_resolve_name_locked(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx, ngx_str_t *name);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *ವೃಕ್ಷ,
    ngx_ಸರತಿ_t *ಸರತಿ);
ಸ್ಥಿರ ngx_int_t ngx_resolver_send_query(ngx_resolver_t *r,
    ngx_resolver_ಗಂಟು_t *rn);
ಸ್ಥಿರ ngx_int_t ngx_resolver_send_udp_query(ngx_resolver_t *r,
    ngx_resolver_connection_t *rec, u_char *query, u_short qlen);
ಸ್ಥಿರ ngx_int_t ngx_resolver_send_tcp_query(ngx_resolver_t *r,
    ngx_resolver_connection_t *rec, u_char *query, u_short qlen);
ಸ್ಥಿರ ngx_int_t ngx_resolver_create_name_query(ngx_resolver_t *r,
    ngx_resolver_ಗಂಟು_t *rn, ngx_str_t *name);
ಸ್ಥಿರ ngx_int_t ngx_resolver_create_srv_query(ngx_resolver_t *r,
    ngx_resolver_ಗಂಟು_t *rn, ngx_str_t *name);
ಸ್ಥಿರ ngx_int_t ngx_resolver_create_addr_query(ngx_resolver_t *r,
    ngx_resolver_ಗಂಟು_t *rn, ngx_resolver_addr_t *addr);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_resend_handler(ngx_event_t *ev);
ಸ್ಥಿರ ಸಮಯ_t ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *ವೃಕ್ಷ,
    ngx_ಸರತಿ_t *ಸರತಿ);
ಸ್ಥಿರ ngx_uint_t ngx_resolver_resend_ಖಾಲಿಗಳು(ngx_resolver_t *r);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_udp_read(ngx_event_t *rev);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_tcp_write(ngx_event_t *wev);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_tcp_read(ngx_event_t *rev);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf,
    ಗಾತ್ರ_t n, ngx_uint_t tcp);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, ಗಾತ್ರ_t n,
    ngx_uint_t ident, ngx_uint_t ಸಂಕೇತ_2, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, ಗಾತ್ರ_t n,
    ngx_uint_t ident, ngx_uint_t ಸಂಕೇತ_2, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, ಗಾತ್ರ_t n,
    ngx_uint_t ident, ngx_uint_t ಸಂಕೇತ_2, ngx_uint_t nan);
ಸ್ಥಿರ ngx_resolver_ಗಂಟು_t *ngx_resolver_lookup_name(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
ಸ್ಥಿರ ngx_resolver_ಗಂಟು_t *ngx_resolver_lookup_srv(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
ಸ್ಥಿರ ngx_resolver_ಗಂಟು_t *ngx_resolver_lookup_addr(ngx_resolver_t *r,
    ಒಳ_addr_t addr);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_rbtree_ಸೇರಿಸು_ಮೌಲ್ಯ(ngx_rbtree_ಗಂಟು_t *ತಾತ್ಕಾಲಿಕ,
    ngx_rbtree_ಗಂಟು_t *ಗಂಟು, ngx_rbtree_ಗಂಟು_t *sentinel);
ಸ್ಥಿರ ngx_int_t ngx_resolver_copy(ngx_resolver_t *r, ngx_str_t *name,
    u_char *buf, u_char *src, u_char *ಕೊನೆಯ);
ಸ್ಥಿರ ngx_int_t ngx_resolver_ಗಣ_timeout(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_timeout_handler(ngx_event_t *ev);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_free_ಗಂಟು(ngx_resolver_t *r, ngx_resolver_ಗಂಟು_t *rn);
ಸ್ಥಿರ ಶೂನ್ಯ *ngx_resolver_alloc(ngx_resolver_t *r, ಗಾತ್ರ_t ಗಾತ್ರ_2);
ಸ್ಥಿರ ಶೂನ್ಯ *ngx_resolver_calloc(ngx_resolver_t *r, ಗಾತ್ರ_t ಗಾತ್ರ_2);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_free(ngx_resolver_t *r, ಶೂನ್ಯ *p);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_free_locked(ngx_resolver_t *r, ಶೂನ್ಯ *p);
ಸ್ಥಿರ ಶೂನ್ಯ *ngx_resolver_dup(ngx_resolver_t *r, ಶೂನ್ಯ *src, ಗಾತ್ರ_t ಗಾತ್ರ_2);
ಸ್ಥಿರ ngx_resolver_addr_t *ngx_resolver_export(ngx_resolver_t *r,
    ngx_resolver_ಗಂಟು_t *rn, ngx_uint_t rotate);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_report_srv(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx);
ಸ್ಥಿರ u_char *ngx_resolver_log_error(ngx_log_t *ಲಘುಗಣಕ, u_char *buf, ಗಾತ್ರ_t len);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_resolve_srv_ಹೆಸರುಗಳು(ngx_resolver_ctx_t *ctx,
    ngx_resolver_ಗಂಟು_t *rn);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_srv_ಹೆಸರುಗಳು_handler(ngx_resolver_ctx_t *ctx);
ಸ್ಥಿರ ngx_int_t ngx_resolver_cmp_srvs(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *one, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *two);

#ಆದರೆ (NGX_HAVE_INET6)
ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_rbtree_ಸೇರಿಸು_addr6_ಮೌಲ್ಯ(ngx_rbtree_ಗಂಟು_t *ತಾತ್ಕಾಲಿಕ,
    ngx_rbtree_ಗಂಟು_t *ಗಂಟು, ngx_rbtree_ಗಂಟು_t *sentinel);
ಸ್ಥಿರ ngx_resolver_ಗಂಟು_t *ngx_resolver_lookup_addr6(ngx_resolver_t *r,
    ರಚನೆ in6_addr *addr, uint32_t hash);
#ಆದರೆ_ಅಂತ್ಯ


ngx_resolver_t *
ngx_resolver_create(ngx_conf_t *cf, ngx_str_t *ಹೆಸರುಗಳು, ngx_uint_t n)
{
    ngx_str_t                   s;
    ngx_url_t                   u;
    ngx_uint_t                  i, j;
    ngx_resolver_t             *r;
    ngx_pool_cleanup_t         *cln;
    ngx_resolver_connection_t  *rec;

    r = ngx_pcalloc(cf->pool, ಗಾತ್ರ(ngx_resolver_t));
    ಆದರೆ (r == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    r->event = ngx_pcalloc(cf->pool, ಗಾತ್ರ(ngx_event_t));
    ಆದರೆ (r->event == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    cln = ngx_pool_cleanup_ಕೂಡಿಸು(cf->pool, 0);
    ಆದರೆ (cln == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    cln->handler = ngx_resolver_cleanup;
    cln->ದತ್ತಾಂಶ = r;

    r->ipv4 = 1;

    ngx_rbtree_init(&r->name_rbtree, &r->name_sentinel,
                    ngx_resolver_rbtree_ಸೇರಿಸು_ಮೌಲ್ಯ);

    ngx_rbtree_init(&r->srv_rbtree, &r->srv_sentinel,
                    ngx_resolver_rbtree_ಸೇರಿಸು_ಮೌಲ್ಯ);

    ngx_rbtree_init(&r->addr_rbtree, &r->addr_sentinel,
                    ngx_rbtree_ಸೇರಿಸು_ಮೌಲ್ಯ);

    ngx_ಸರತಿ_init(&r->name_resend_ಸರತಿ);
    ngx_ಸರತಿ_init(&r->srv_resend_ಸರತಿ);
    ngx_ಸರತಿ_init(&r->addr_resend_ಸರತಿ);

    ngx_ಸರತಿ_init(&r->name_expire_ಸರತಿ);
    ngx_ಸರತಿ_init(&r->srv_expire_ಸರತಿ);
    ngx_ಸರತಿ_init(&r->addr_expire_ಸರತಿ);

#ಆದರೆ (NGX_HAVE_INET6)
    r->ipv6 = 1;

    ngx_rbtree_init(&r->addr6_rbtree, &r->addr6_sentinel,
                    ngx_resolver_rbtree_ಸೇರಿಸು_addr6_ಮೌಲ್ಯ);

    ngx_ಸರತಿ_init(&r->addr6_resend_ಸರತಿ);

    ngx_ಸರತಿ_init(&r->addr6_expire_ಸರತಿ);
#ಆದರೆ_ಅಂತ್ಯ

    r->event->handler = ngx_resolver_resend_handler;
    r->event->ದತ್ತಾಂಶ = r;
    r->event->ಲಘುಗಣಕ = &cf->ಚಕ್ರ->new_log;
    r->event->cancelable = 1;
    r->ident = -1;

    r->resend_timeout = 5;
    r->tcp_timeout = 5;
    r->expire = 30;
    r->valid = 0;

    r->ಲಘುಗಣಕ = &cf->ಚಕ್ರ->new_log;
    r->log_level = NGX_LOG_ERR;

    ಆದರೆ (n) {
        ಆದರೆ (ngx_array_init(&r->connections, cf->pool, n,
                           ಗಾತ್ರ(ngx_resolver_connection_t))
            != NGX_OK)
        {
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }
    }

    ಗಾಗಿ (i = 0; i < n; i++) {
        ಆದರೆ (ngx_strncmp(ಹೆಸರುಗಳು[i].ದತ್ತಾಂಶ, "valid=", 6) == 0) {
            s.len = ಹೆಸರುಗಳು[i].len - 6;
            s.ದತ್ತಾಂಶ = ಹೆಸರುಗಳು[i].ದತ್ತಾಂಶ + 6;

            r->valid = ngx_parse_ಸಮಯ(&s, 1);

            ಆದರೆ (r->valid == (ಸಮಯ_t) NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &ಹೆಸರುಗಳು[i]);
                ಹಿಂತಿರುಗಿ ಖಾಲಿ;
            }

            ಮುಂದುವರಿ;
        }

#ಆದರೆ (NGX_HAVE_INET6)
        ಆದರೆ (ngx_strncmp(ಹೆಸರುಗಳು[i].ದತ್ತಾಂಶ, "ipv4=", 5) == 0) {

            ಆದರೆ (ngx_strcmp(&ಹೆಸರುಗಳು[i].ದತ್ತಾಂಶ[5], "on") == 0) {
                r->ipv4 = 1;

            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ngx_strcmp(&ಹೆಸರುಗಳು[i].ದತ್ತಾಂಶ[5], "off") == 0) {
                r->ipv4 = 0;

            } ಇಲ್ಲದಿದ್ದರೆ {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &ಹೆಸರುಗಳು[i]);
                ಹಿಂತಿರುಗಿ ಖಾಲಿ;
            }

            ಮುಂದುವರಿ;
        }

        ಆದರೆ (ngx_strncmp(ಹೆಸರುಗಳು[i].ದತ್ತಾಂಶ, "ipv6=", 5) == 0) {

            ಆದರೆ (ngx_strcmp(&ಹೆಸರುಗಳು[i].ದತ್ತಾಂಶ[5], "on") == 0) {
                r->ipv6 = 1;

            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ngx_strcmp(&ಹೆಸರುಗಳು[i].ದತ್ತಾಂಶ[5], "off") == 0) {
                r->ipv6 = 0;

            } ಇಲ್ಲದಿದ್ದರೆ {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &ಹೆಸರುಗಳು[i]);
                ಹಿಂತಿರುಗಿ ಖಾಲಿ;
            }

            ಮುಂದುವರಿ;
        }
#ಆದರೆ_ಅಂತ್ಯ

        ngx_memzero(&u, ಗಾತ್ರ(ngx_url_t));

        u.url = ಹೆಸರುಗಳು[i];
        u.default_ಪೋರ್ಟ್ = 53;

        ಆದರೆ (ngx_parse_url(cf->pool, &u) != NGX_OK) {
            ಆದರೆ (u.err) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "%s in resolver \"%V\"",
                                   u.err, &u.url);
            }

            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }

        rec = ngx_array_ತಳ್ಳು_n(&r->connections, u.naddrs);
        ಆದರೆ (rec == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }

        ngx_memzero(rec, u.naddrs * ಗಾತ್ರ(ngx_resolver_connection_t));

        ಗಾಗಿ (j = 0; j < u.naddrs; j++) {
            rec[j].sockaddr = u.addrs[j].sockaddr;
            rec[j].socklen = u.addrs[j].socklen;
            rec[j].server = u.addrs[j].name;
            rec[j].resolver = r;
        }
    }

#ಆದರೆ (NGX_HAVE_INET6)
    ಆದರೆ (r->ipv4 + r->ipv6 == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"ipv4\" and \"ipv6\" cannot both be \"off\"");
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }
#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ (n && r->connections.nelts == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "no name servers defined");
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಹಿಂತಿರುಗಿ r;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_resolver_cleanup(ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
    ngx_resolver_t  *r = ದತ್ತಾಂಶ;

    ngx_uint_t                  i;
    ngx_resolver_connection_t  *rec;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_ಚಕ್ರ->ಲಘುಗಣಕ, 0, "cleanup resolver");

    ngx_resolver_cleanup_ವೃಕ್ಷ(r, &r->name_rbtree);

    ngx_resolver_cleanup_ವೃಕ್ಷ(r, &r->srv_rbtree);

    ngx_resolver_cleanup_ವೃಕ್ಷ(r, &r->addr_rbtree);

#ಆದರೆ (NGX_HAVE_INET6)
    ngx_resolver_cleanup_ವೃಕ್ಷ(r, &r->addr6_rbtree);
#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ (r->event->timer_ಗಣ) {
        ngx_del_timer(r->event);
    }

    rec = r->connections.elts;

    ಗಾಗಿ (i = 0; i < r->connections.nelts; i++) {
        ಆದರೆ (rec[i].udp) {
            ngx_close_connection(rec[i].udp);
        }

        ಆದರೆ (rec[i].tcp) {
            ngx_close_connection(rec[i].tcp);
        }

        ಆದರೆ (rec[i].read_buf) {
            ngx_resolver_free(r, rec[i].read_buf->ಆರಂಭ);
            ngx_resolver_free(r, rec[i].read_buf);
        }

        ಆದರೆ (rec[i].write_buf) {
            ngx_resolver_free(r, rec[i].write_buf->ಆರಂಭ);
            ngx_resolver_free(r, rec[i].write_buf);
        }
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_resolver_cleanup_ವೃಕ್ಷ(ngx_resolver_t *r, ngx_rbtree_t *ವೃಕ್ಷ)
{
    ngx_resolver_ctx_t   *ctx, *ಮುಂದಿನ;
    ngx_resolver_ಗಂಟು_t  *rn;

    ಆಗ (ವೃಕ್ಷ->ಬೇರು != ವೃಕ್ಷ->sentinel) {

        rn = ngx_resolver_ಗಂಟು(ngx_rbtree_min(ವೃಕ್ಷ->ಬೇರು, ವೃಕ್ಷ->sentinel));

        ngx_ಸರತಿ_remove(&rn->ಸರತಿ);

        ಗಾಗಿ (ctx = rn->ಕಾಯುವಿಕೆ; ctx; ctx = ಮುಂದಿನ) {
            ಮುಂದಿನ = ctx->ಮುಂದಿನ;

            ಆದರೆ (ctx->event) {
                ಆದರೆ (ctx->event->timer_ಗಣ) {
                    ngx_del_timer(ctx->event);
                }

                ngx_resolver_free(r, ctx->event);
            }

            ngx_resolver_free(r, ctx);
        }

        ngx_rbtree_delete(ವೃಕ್ಷ, &rn->ಗಂಟು);

        ngx_resolver_free_ಗಂಟು(r, rn);
    }
}


ngx_resolver_ctx_t *
ngx_resolve_ಆರಂಭ(ngx_resolver_t *r, ngx_resolver_ctx_t *ತಾತ್ಕಾಲಿಕ)
{
    ಒಳ_addr_t            addr;
    ngx_resolver_ctx_t  *ctx;

    ಆದರೆ (ತಾತ್ಕಾಲಿಕ) {
        addr = ngx_inet_addr(ತಾತ್ಕಾಲಿಕ->name.ದತ್ತಾಂಶ, ತಾತ್ಕಾಲಿಕ->name.len);

        ಆದರೆ (addr != INADDR_NONE) {
            ತಾತ್ಕಾಲಿಕ->resolver = r;
            ತಾತ್ಕಾಲಿಕ->state = NGX_OK;
            ತಾತ್ಕಾಲಿಕ->naddrs = 1;
            ತಾತ್ಕಾಲಿಕ->addrs = &ತಾತ್ಕಾಲಿಕ->addr;
            ತಾತ್ಕಾಲಿಕ->addr.sockaddr = (ರಚನೆ sockaddr *) &ತಾತ್ಕಾಲಿಕ->ಜ್ಯಾ;
            ತಾತ್ಕಾಲಿಕ->addr.socklen = ಗಾತ್ರ(ರಚನೆ sockaddr_ಒಳ);
            ngx_memzero(&ತಾತ್ಕಾಲಿಕ->ಜ್ಯಾ, ಗಾತ್ರ(ರಚನೆ sockaddr_ಒಳ));
            ತಾತ್ಕಾಲಿಕ->ಜ್ಯಾ.sin_family = AF_INET;
            ತಾತ್ಕಾಲಿಕ->ಜ್ಯಾ.sin_addr.s_addr = addr;
            ತಾತ್ಕಾಲಿಕ->ಶೀಘ್ರ = 1;

            ಹಿಂತಿರುಗಿ ತಾತ್ಕಾಲಿಕ;
        }
    }

    ಆದರೆ (r->connections.nelts == 0) {
        ಹಿಂತಿರುಗಿ NGX_NO_RESOLVER;
    }

    ctx = ngx_resolver_calloc(r, ಗಾತ್ರ(ngx_resolver_ctx_t));

    ಆದರೆ (ctx) {
        ctx->resolver = r;
    }

    ಹಿಂತಿರುಗಿ ctx;
}


ngx_int_t
ngx_resolve_name(ngx_resolver_ctx_t *ctx)
{
    ಗಾತ್ರ_t           slen;
    ngx_int_t        rc;
    ngx_str_t        name;
    ngx_resolver_t  *r;

    r = ctx->resolver;

    ಆದರೆ (ctx->name.len > 0 && ctx->name.ದತ್ತಾಂಶ[ctx->name.len - 1] == '.') {
        ctx->name.len--;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->ಲಘುಗಣಕ, 0,
                   "resolve: \"%V\"", &ctx->name);

    ಆದರೆ (ctx->ಶೀಘ್ರ) {
        ctx->handler(ctx);
        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    ಆದರೆ (ctx->service.len) {
        slen = ctx->service.len;

        ಆದರೆ (ngx_strlchr(ctx->service.ದತ್ತಾಂಶ,
                        ctx->service.ದತ್ತಾಂಶ + ctx->service.len, '.')
            == ಖಾಲಿ)
        {
            slen += ಗಾತ್ರ("_._tcp") - 1;
        }

        name.len = slen + 1 + ctx->name.len;

        name.ದತ್ತಾಂಶ = ngx_resolver_alloc(r, name.len);
        ಆದರೆ (name.ದತ್ತಾಂಶ == ಖಾಲಿ) {
            ಹೋಗು failed;
        }

        ಆದರೆ (slen == ctx->service.len) {
            ngx_sprintf(name.ದತ್ತಾಂಶ, "%V.%V", &ctx->service, &ctx->name);

        } ಇಲ್ಲದಿದ್ದರೆ {
            ngx_sprintf(name.ದತ್ತಾಂಶ, "_%V._tcp.%V", &ctx->service, &ctx->name);
        }

        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &name);

        ngx_resolver_free(r, name.ದತ್ತಾಂಶ);

    } ಇಲ್ಲದಿದ್ದರೆ {
        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &ctx->name);
    }

    ಆದರೆ (rc == NGX_OK) {
        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    /* unlock name mutex */

    ಆದರೆ (rc == NGX_AGAIN) {
        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    /* NGX_ERROR */

    ಆದರೆ (ctx->event) {
        ngx_resolver_free(r, ctx->event);
    }

failed:

    ngx_resolver_free(r, ctx);

    ಹಿಂತಿರುಗಿ NGX_ERROR;
}


ಶೂನ್ಯ
ngx_resolve_name_ಮುಗಿದ(ngx_resolver_ctx_t *ctx)
{
    ngx_uint_t            i;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_ಗಂಟು_t  *rn;

    r = ctx->resolver;

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->ಲಘುಗಣಕ, 0,
                   "resolve name done: %i", ctx->state);

    ಆದರೆ (ctx->ಶೀಘ್ರ) {
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ (ctx->event && ctx->event->timer_ಗಣ) {
        ngx_del_timer(ctx->event);
    }

    /* lock name mutex */

    ಆದರೆ (ctx->nsrvs) {
        ಗಾಗಿ (i = 0; i < ctx->nsrvs; i++) {
            ಆದರೆ (ctx->srvs[i].ctx) {
                ngx_resolve_name_ಮುಗಿದ(ctx->srvs[i].ctx);
            }

            ಆದರೆ (ctx->srvs[i].addrs) {
                ngx_resolver_free(r, ctx->srvs[i].addrs->sockaddr);
                ngx_resolver_free(r, ctx->srvs[i].addrs);
            }

            ngx_resolver_free(r, ctx->srvs[i].name.ದತ್ತಾಂಶ);
        }

        ngx_resolver_free(r, ctx->srvs);
    }

    ಆದರೆ (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->ಗಂಟು;

        ಆದರೆ (rn) {
            p = &rn->ಕಾಯುವಿಕೆ;
            w = rn->ಕಾಯುವಿಕೆ;

            ಆಗ (w) {
                ಆದರೆ (w == ctx) {
                    *p = w->ಮುಂದಿನ;

                    ಹೋಗು ಮುಗಿದ;
                }

                p = &w->ಮುಂದಿನ;
                w = w->ಮುಂದಿನ;
            }

            ngx_log_error(NGX_LOG_ALERT, r->ಲಘುಗಣಕ, 0,
                          "could not cancel %V resolving", &ctx->name);
        }
    }

ಮುಗಿದ:

    ಆದರೆ (ctx->service.len) {
        ngx_resolver_expire(r, &r->srv_rbtree, &r->srv_expire_ಸರತಿ);

    } ಇಲ್ಲದಿದ್ದರೆ {
        ngx_resolver_expire(r, &r->name_rbtree, &r->name_expire_ಸರತಿ);
    }

    /* unlock name mutex */

    /* lock alloc mutex */

    ಆದರೆ (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    ಆದರೆ (r->event->timer_ಗಣ && ngx_resolver_resend_ಖಾಲಿಗಳು(r)) {
        ngx_del_timer(r->event);
    }
}


ಸ್ಥಿರ ngx_int_t
ngx_resolve_name_locked(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx,
    ngx_str_t *name)
{
    uint32_t              hash;
    ngx_int_t             rc;
    ngx_str_t             cname;
    ngx_uint_t            i, naddrs;
    ngx_ಸರತಿ_t          *resend_ಸರತಿ, *expire_ಸರತಿ;
    ngx_rbtree_t         *ವೃಕ್ಷ;
    ngx_resolver_ctx_t   *ಮುಂದಿನ, *ಕೊನೆಯ;
    ngx_resolver_addr_t  *addrs;
    ngx_resolver_ಗಂಟು_t  *rn;

    ngx_strlow(name->ದತ್ತಾಂಶ, name->ದತ್ತಾಂಶ, name->len);

    hash = ngx_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32_short(name->ದತ್ತಾಂಶ, name->len);

    ಆದರೆ (ctx->service.len) {
        rn = ngx_resolver_lookup_srv(r, name, hash);

        ವೃಕ್ಷ = &r->srv_rbtree;
        resend_ಸರತಿ = &r->srv_resend_ಸರತಿ;
        expire_ಸರತಿ = &r->srv_expire_ಸರತಿ;

    } ಇಲ್ಲದಿದ್ದರೆ {
        rn = ngx_resolver_lookup_name(r, name, hash);

        ವೃಕ್ಷ = &r->name_rbtree;
        resend_ಸರತಿ = &r->name_resend_ಸರತಿ;
        expire_ಸರತಿ = &r->name_expire_ಸರತಿ;
    }

    ಆದರೆ (rn) {

        /* ctx can be a list after NGX_RESOLVE_CNAME */
        ಗಾಗಿ (ಕೊನೆಯ = ctx; ಕೊನೆಯ->ಮುಂದಿನ; ಕೊನೆಯ = ಕೊನೆಯ->ಮುಂದಿನ);

        ಆದರೆ (rn->valid >= ngx_ಸಮಯ()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->ಲಘುಗಣಕ, 0, "resolve cached");

            ngx_ಸರತಿ_remove(&rn->ಸರತಿ);

            rn->expire = ngx_ಸಮಯ() + r->expire;

            ngx_ಸರತಿ_ಸೇರಿಸು_ತಲೆ(expire_ಸರತಿ, &rn->ಸರತಿ);

            naddrs = (rn->naddrs == (u_short) -1) ? 0 : rn->naddrs;
#ಆದರೆ (NGX_HAVE_INET6)
            naddrs += (rn->naddrs6 == (u_short) -1) ? 0 : rn->naddrs6;
#ಆದರೆ_ಅಂತ್ಯ

            ಆದರೆ (naddrs) {

                ಆದರೆ (naddrs == 1 && rn->naddrs == 1) {
                    addrs = ಖಾಲಿ;

                } ಇಲ್ಲದಿದ್ದರೆ {
                    addrs = ngx_resolver_export(r, rn, 1);
                    ಆದರೆ (addrs == ಖಾಲಿ) {
                        ಹಿಂತಿರುಗಿ NGX_ERROR;
                    }
                }

                ಕೊನೆಯ->ಮುಂದಿನ = rn->ಕಾಯುವಿಕೆ;
                rn->ಕಾಯುವಿಕೆ = ಖಾಲಿ;

                /* unlock name mutex */

                ಮಾಡು {
                    ctx->state = NGX_OK;
                    ctx->valid = rn->valid;
                    ctx->naddrs = naddrs;

                    ಆದರೆ (addrs == ಖಾಲಿ) {
                        ctx->addrs = &ctx->addr;
                        ctx->addr.sockaddr = (ರಚನೆ sockaddr *) &ctx->ಜ್ಯಾ;
                        ctx->addr.socklen = ಗಾತ್ರ(ರಚನೆ sockaddr_ಒಳ);
                        ngx_memzero(&ctx->ಜ್ಯಾ, ಗಾತ್ರ(ರಚನೆ sockaddr_ಒಳ));
                        ctx->ಜ್ಯಾ.sin_family = AF_INET;
                        ctx->ಜ್ಯಾ.sin_addr.s_addr = rn->u.addr;

                    } ಇಲ್ಲದಿದ್ದರೆ {
                        ctx->addrs = addrs;
                    }

                    ಮುಂದಿನ = ctx->ಮುಂದಿನ;

                    ctx->handler(ctx);

                    ctx = ಮುಂದಿನ;
                } ಆಗ (ctx);

                ಆದರೆ (addrs != ಖಾಲಿ) {
                    ngx_resolver_free(r, addrs->sockaddr);
                    ngx_resolver_free(r, addrs);
                }

                ಹಿಂತಿರುಗಿ NGX_OK;
            }

            ಆದರೆ (rn->nsrvs) {
                ಕೊನೆಯ->ಮುಂದಿನ = rn->ಕಾಯುವಿಕೆ;
                rn->ಕಾಯುವಿಕೆ = ಖಾಲಿ;

                /* unlock name mutex */

                ಮಾಡು {
                    ಮುಂದಿನ = ctx->ಮುಂದಿನ;

                    ngx_resolver_resolve_srv_ಹೆಸರುಗಳು(ctx, rn);

                    ctx = ಮುಂದಿನ;
                } ಆಗ (ctx);

                ಹಿಂತಿರುಗಿ NGX_OK;
            }

            /* NGX_RESOLVE_CNAME */

            ಆದರೆ (ctx->recursion++ < NGX_RESOLVER_MAX_RECURSION) {

                cname.len = rn->cnlen;
                cname.ದತ್ತಾಂಶ = rn->u.cname;

                ಹಿಂತಿರುಗಿ ngx_resolve_name_locked(r, ctx, &cname);
            }

            ಕೊನೆಯ->ಮುಂದಿನ = rn->ಕಾಯುವಿಕೆ;
            rn->ಕಾಯುವಿಕೆ = ಖಾಲಿ;

            /* unlock name mutex */

            ಮಾಡು {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                ctx->valid = ngx_ಸಮಯ() + (r->valid ? r->valid : 10);
                ಮುಂದಿನ = ctx->ಮುಂದಿನ;

                ctx->handler(ctx);

                ctx = ಮುಂದಿನ;
            } ಆಗ (ctx);

            ಹಿಂತಿರುಗಿ NGX_OK;
        }

        ಆದರೆ (rn->ಕಾಯುವಿಕೆ) {
            ಆದರೆ (ngx_resolver_ಗಣ_timeout(r, ctx) != NGX_OK) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            ಕೊನೆಯ->ಮುಂದಿನ = rn->ಕಾಯುವಿಕೆ;
            rn->ಕಾಯುವಿಕೆ = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;

            ಮಾಡು {
                ctx->ಗಂಟು = rn;
                ctx = ctx->ಮುಂದಿನ;
            } ಆಗ (ctx);

            ಹಿಂತಿರುಗಿ NGX_AGAIN;
        }

        ngx_ಸರತಿ_remove(&rn->ಸರತಿ);

        /* lock alloc mutex */

        ಆದರೆ (rn->query) {
            ngx_resolver_free_locked(r, rn->query);
            rn->query = ಖಾಲಿ;
#ಆದರೆ (NGX_HAVE_INET6)
            rn->query6 = ಖಾಲಿ;
#ಆದರೆ_ಅಂತ್ಯ
        }

        ಆದರೆ (rn->cnlen) {
            ngx_resolver_free_locked(r, rn->u.cname);
        }

        ಆದರೆ (rn->naddrs > 1 && rn->naddrs != (u_short) -1) {
            ngx_resolver_free_locked(r, rn->u.addrs);
        }

#ಆದರೆ (NGX_HAVE_INET6)
        ಆದರೆ (rn->naddrs6 > 1 && rn->naddrs6 != (u_short) -1) {
            ngx_resolver_free_locked(r, rn->u6.addrs6);
        }
#ಆದರೆ_ಅಂತ್ಯ

        ಆದರೆ (rn->nsrvs) {
            ಗಾಗಿ (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
                ಆದರೆ (rn->u.srvs[i].name.ದತ್ತಾಂಶ) {
                    ngx_resolver_free_locked(r, rn->u.srvs[i].name.ದತ್ತಾಂಶ);
                }
            }

            ngx_resolver_free_locked(r, rn->u.srvs);
        }

        /* unlock alloc mutex */

    } ಇಲ್ಲದಿದ್ದರೆ {

        rn = ngx_resolver_alloc(r, ಗಾತ್ರ(ngx_resolver_ಗಂಟು_t));
        ಆದರೆ (rn == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        rn->name = ngx_resolver_dup(r, name->ದತ್ತಾಂಶ, name->len);
        ಆದರೆ (rn->name == ಖಾಲಿ) {
            ngx_resolver_free(r, rn);
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        rn->ಗಂಟು.ಕೀಲಿ = hash;
        rn->nlen = (u_short) name->len;
        rn->query = ಖಾಲಿ;
#ಆದರೆ (NGX_HAVE_INET6)
        rn->query6 = ಖಾಲಿ;
#ಆದರೆ_ಅಂತ್ಯ

        ngx_rbtree_ಸೇರಿಸು(ವೃಕ್ಷ, &rn->ಗಂಟು);
    }

    ಆದರೆ (ctx->service.len) {
        rc = ngx_resolver_create_srv_query(r, rn, name);

    } ಇಲ್ಲದಿದ್ದರೆ {
        rc = ngx_resolver_create_name_query(r, rn, name);
    }

    ಆದರೆ (rc == NGX_ERROR) {
        ಹೋಗು failed;
    }

    ಆದರೆ (rc == NGX_DECLINED) {
        ngx_rbtree_delete(ವೃಕ್ಷ, &rn->ಗಂಟು);

        ngx_resolver_free(r, rn->query);
        ngx_resolver_free(r, rn->name);
        ngx_resolver_free(r, rn);

        ಮಾಡು {
            ctx->state = NGX_RESOLVE_NXDOMAIN;
            ಮುಂದಿನ = ctx->ಮುಂದಿನ;

            ctx->handler(ctx);

            ctx = ಮುಂದಿನ;
        } ಆಗ (ctx);

        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    rn->ಕೊನೆಯ_connection = r->ಕೊನೆಯ_connection++;
    ಆದರೆ (r->ಕೊನೆಯ_connection == r->connections.nelts) {
        r->ಕೊನೆಯ_connection = 0;
    }

    rn->naddrs = r->ipv4 ? (u_short) -1 : 0;
    rn->tcp = 0;
#ಆದರೆ (NGX_HAVE_INET6)
    rn->naddrs6 = r->ipv6 ? (u_short) -1 : 0;
    rn->tcp6 = 0;
#ಆದರೆ_ಅಂತ್ಯ
    rn->nsrvs = 0;

    ಆದರೆ (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->ಕೊನೆಯ_connection++;
        ಆದರೆ (rn->ಕೊನೆಯ_connection == r->connections.nelts) {
            rn->ಕೊನೆಯ_connection = 0;
        }

        (ಶೂನ್ಯ) ngx_resolver_send_query(r, rn);
    }

    ಆದರೆ (ngx_resolver_ಗಣ_timeout(r, ctx) != NGX_OK) {
        ಹೋಗು failed;
    }

    ಆದರೆ (ngx_resolver_resend_ಖಾಲಿಗಳು(r)) {
        ngx_ಕೂಡಿಸು_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_ಸಮಯ() + r->resend_timeout;

    ngx_ಸರತಿ_ಸೇರಿಸು_ತಲೆ(resend_ಸರತಿ, &rn->ಸರತಿ);

    rn->ಸಂಕೇತ_2 = 0;
    rn->cnlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_ಮೌಲ್ಯ;
    rn->ಕಾಯುವಿಕೆ = ctx;

    ctx->state = NGX_AGAIN;
    ctx->async = 1;

    ಮಾಡು {
        ctx->ಗಂಟು = rn;
        ctx = ctx->ಮುಂದಿನ;
    } ಆಗ (ctx);

    ಹಿಂತಿರುಗಿ NGX_AGAIN;

failed:

    ngx_rbtree_delete(ವೃಕ್ಷ, &rn->ಗಂಟು);

    ಆದರೆ (rn->query) {
        ngx_resolver_free(r, rn->query);
    }

    ngx_resolver_free(r, rn->name);

    ngx_resolver_free(r, rn);

    ಹಿಂತಿರುಗಿ NGX_ERROR;
}


ngx_int_t
ngx_resolve_addr(ngx_resolver_ctx_t *ctx)
{
    u_char               *name;
    ಒಳ_addr_t             addr;
    ngx_ಸರತಿ_t          *resend_ಸರತಿ, *expire_ಸರತಿ;
    ngx_rbtree_t         *ವೃಕ್ಷ;
    ngx_resolver_t       *r;
    ರಚನೆ sockaddr_ಒಳ   *ಜ್ಯಾ;
    ngx_resolver_ಗಂಟು_t  *rn;
#ಆದರೆ (NGX_HAVE_INET6)
    uint32_t              hash;
    ರಚನೆ sockaddr_in6  *sin6;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_SUPPRESS_WARN)
    addr = 0;
#ಆದರೆ (NGX_HAVE_INET6)
    hash = 0;
    sin6 = ಖಾಲಿ;
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

    r = ctx->resolver;

    ಆಯ್ಕೆ (ctx->addr.sockaddr->sa_family) {

#ಆದರೆ (NGX_HAVE_INET6)
    ಸ್ಥಿತಿ AF_INET6:
        sin6 = (ರಚನೆ sockaddr_in6 *) ctx->addr.sockaddr;
        hash = ngx_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32_short(sin6->sin6_addr.s6_addr, 16);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr6(r, &sin6->sin6_addr, hash);

        ವೃಕ್ಷ = &r->addr6_rbtree;
        resend_ಸರತಿ = &r->addr6_resend_ಸರತಿ;
        expire_ಸರತಿ = &r->addr6_expire_ಸರತಿ;

        ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ

    ಪೂರ್ವನಿಯೋಜಿತ: /* AF_INET */
        ಜ್ಯಾ = (ರಚನೆ sockaddr_ಒಳ *) ctx->addr.sockaddr;
        addr = ntohl(ಜ್ಯಾ->sin_addr.s_addr);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        ವೃಕ್ಷ = &r->addr_rbtree;
        resend_ಸರತಿ = &r->addr_resend_ಸರತಿ;
        expire_ಸರತಿ = &r->addr_expire_ಸರತಿ;
    }

    ಆದರೆ (rn) {

        ಆದರೆ (rn->valid >= ngx_ಸಮಯ()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->ಲಘುಗಣಕ, 0, "resolve cached");

            ngx_ಸರತಿ_remove(&rn->ಸರತಿ);

            rn->expire = ngx_ಸಮಯ() + r->expire;

            ngx_ಸರತಿ_ಸೇರಿಸು_ತಲೆ(expire_ಸರತಿ, &rn->ಸರತಿ);

            name = ngx_resolver_dup(r, rn->name, rn->nlen);
            ಆದರೆ (name == ಖಾಲಿ) {
                ngx_resolver_free(r, ctx);
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            ctx->name.len = rn->nlen;
            ctx->name.ದತ್ತಾಂಶ = name;

            /* unlock addr mutex */

            ctx->state = NGX_OK;
            ctx->valid = rn->valid;

            ctx->handler(ctx);

            ngx_resolver_free(r, name);

            ಹಿಂತಿರುಗಿ NGX_OK;
        }

        ಆದರೆ (rn->ಕಾಯುವಿಕೆ) {
            ಆದರೆ (ngx_resolver_ಗಣ_timeout(r, ctx) != NGX_OK) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            ctx->ಮುಂದಿನ = rn->ಕಾಯುವಿಕೆ;
            rn->ಕಾಯುವಿಕೆ = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;
            ctx->ಗಂಟು = rn;

            /* unlock addr mutex */

            ಹಿಂತಿರುಗಿ NGX_OK;
        }

        ngx_ಸರತಿ_remove(&rn->ಸರತಿ);

        ngx_resolver_free(r, rn->query);
        rn->query = ಖಾಲಿ;
#ಆದರೆ (NGX_HAVE_INET6)
        rn->query6 = ಖಾಲಿ;
#ಆದರೆ_ಅಂತ್ಯ

    } ಇಲ್ಲದಿದ್ದರೆ {
        rn = ngx_resolver_alloc(r, ಗಾತ್ರ(ngx_resolver_ಗಂಟು_t));
        ಆದರೆ (rn == ಖಾಲಿ) {
            ಹೋಗು failed;
        }

        ಆಯ್ಕೆ (ctx->addr.sockaddr->sa_family) {

#ಆದರೆ (NGX_HAVE_INET6)
        ಸ್ಥಿತಿ AF_INET6:
            rn->addr6 = sin6->sin6_addr;
            rn->ಗಂಟು.ಕೀಲಿ = hash;
            ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ

        ಪೂರ್ವನಿಯೋಜಿತ: /* AF_INET */
            rn->ಗಂಟು.ಕೀಲಿ = addr;
        }

        rn->query = ಖಾಲಿ;
#ಆದರೆ (NGX_HAVE_INET6)
        rn->query6 = ಖಾಲಿ;
#ಆದರೆ_ಅಂತ್ಯ

        ngx_rbtree_ಸೇರಿಸು(ವೃಕ್ಷ, &rn->ಗಂಟು);
    }

    ಆದರೆ (ngx_resolver_create_addr_query(r, rn, &ctx->addr) != NGX_OK) {
        ಹೋಗು failed;
    }

    rn->ಕೊನೆಯ_connection = r->ಕೊನೆಯ_connection++;
    ಆದರೆ (r->ಕೊನೆಯ_connection == r->connections.nelts) {
        r->ಕೊನೆಯ_connection = 0;
    }

    rn->naddrs = (u_short) -1;
    rn->tcp = 0;
#ಆದರೆ (NGX_HAVE_INET6)
    rn->naddrs6 = (u_short) -1;
    rn->tcp6 = 0;
#ಆದರೆ_ಅಂತ್ಯ
    rn->nsrvs = 0;

    ಆದರೆ (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->ಕೊನೆಯ_connection++;
        ಆದರೆ (rn->ಕೊನೆಯ_connection == r->connections.nelts) {
            rn->ಕೊನೆಯ_connection = 0;
        }

        (ಶೂನ್ಯ) ngx_resolver_send_query(r, rn);
    }

    ಆದರೆ (ngx_resolver_ಗಣ_timeout(r, ctx) != NGX_OK) {
        ಹೋಗು failed;
    }

    ಆದರೆ (ngx_resolver_resend_ಖಾಲಿಗಳು(r)) {
        ngx_ಕೂಡಿಸು_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_ಸಮಯ() + r->resend_timeout;

    ngx_ಸರತಿ_ಸೇರಿಸು_ತಲೆ(resend_ಸರತಿ, &rn->ಸರತಿ);

    rn->ಸಂಕೇತ_2 = 0;
    rn->cnlen = 0;
    rn->name = ಖಾಲಿ;
    rn->nlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_ಮೌಲ್ಯ;
    rn->ಕಾಯುವಿಕೆ = ctx;

    /* unlock addr mutex */

    ctx->state = NGX_AGAIN;
    ctx->async = 1;
    ctx->ಗಂಟು = rn;

    ಹಿಂತಿರುಗಿ NGX_OK;

failed:

    ಆದರೆ (rn) {
        ngx_rbtree_delete(ವೃಕ್ಷ, &rn->ಗಂಟು);

        ಆದರೆ (rn->query) {
            ngx_resolver_free(r, rn->query);
        }

        ngx_resolver_free(r, rn);
    }

    /* unlock addr mutex */

    ಆದರೆ (ctx->event) {
        ngx_resolver_free(r, ctx->event);
    }

    ngx_resolver_free(r, ctx);

    ಹಿಂತಿರುಗಿ NGX_ERROR;
}


ಶೂನ್ಯ
ngx_resolve_addr_ಮುಗಿದ(ngx_resolver_ctx_t *ctx)
{
    ngx_ಸರತಿ_t          *expire_ಸರತಿ;
    ngx_rbtree_t         *ವೃಕ್ಷ;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_ಗಂಟು_t  *rn;

    r = ctx->resolver;

    ಆಯ್ಕೆ (ctx->addr.sockaddr->sa_family) {

#ಆದರೆ (NGX_HAVE_INET6)
    ಸ್ಥಿತಿ AF_INET6:
        ವೃಕ್ಷ = &r->addr6_rbtree;
        expire_ಸರತಿ = &r->addr6_expire_ಸರತಿ;
        ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ

    ಪೂರ್ವನಿಯೋಜಿತ: /* AF_INET */
        ವೃಕ್ಷ = &r->addr_rbtree;
        expire_ಸರತಿ = &r->addr_expire_ಸರತಿ;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->ಲಘುಗಣಕ, 0,
                   "resolve addr done: %i", ctx->state);

    ಆದರೆ (ctx->event && ctx->event->timer_ಗಣ) {
        ngx_del_timer(ctx->event);
    }

    /* lock addr mutex */

    ಆದರೆ (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->ಗಂಟು;

        ಆದರೆ (rn) {
            p = &rn->ಕಾಯುವಿಕೆ;
            w = rn->ಕಾಯುವಿಕೆ;

            ಆಗ (w) {
                ಆದರೆ (w == ctx) {
                    *p = w->ಮುಂದಿನ;

                    ಹೋಗು ಮುಗಿದ;
                }

                p = &w->ಮುಂದಿನ;
                w = w->ಮುಂದಿನ;
            }
        }

        {
            u_char     ಪಠ್ಯ[NGX_SOCKADDR_STRLEN];
            ngx_str_t  addrtext;

            addrtext.ದತ್ತಾಂಶ = ಪಠ್ಯ;
            addrtext.len = ngx_sock_ntop(ctx->addr.sockaddr, ctx->addr.socklen,
                                         ಪಠ್ಯ, NGX_SOCKADDR_STRLEN, 0);

            ngx_log_error(NGX_LOG_ALERT, r->ಲಘುಗಣಕ, 0,
                          "could not cancel %V resolving", &addrtext);
        }
    }

ಮುಗಿದ:

    ngx_resolver_expire(r, ವೃಕ್ಷ, expire_ಸರತಿ);

    /* unlock addr mutex */

    /* lock alloc mutex */

    ಆದರೆ (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    ಆದರೆ (r->event->timer_ಗಣ && ngx_resolver_resend_ಖಾಲಿಗಳು(r)) {
        ngx_del_timer(r->event);
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *ವೃಕ್ಷ, ngx_ಸರತಿ_t *ಸರತಿ)
{
    ಸಮಯ_t                now;
    ngx_uint_t            i;
    ngx_ಸರತಿ_t          *q;
    ngx_resolver_ಗಂಟು_t  *rn;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->ಲಘುಗಣಕ, 0, "resolver expire");

    now = ngx_ಸಮಯ();

    ಗಾಗಿ (i = 0; i < 2; i++) {
        ಆದರೆ (ngx_ಸರತಿ_ಖಾಲಿಗಳು(ಸರತಿ)) {
            ಹಿಂತಿರುಗಿ;
        }

        q = ngx_ಸರತಿ_ಕೊನೆಯ(ಸರತಿ);

        rn = ngx_ಸರತಿ_ದತ್ತಾಂಶ(q, ngx_resolver_ಗಂಟು_t, ಸರತಿ);

        ಆದರೆ (now <= rn->expire) {
            ಹಿಂತಿರುಗಿ;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->ಲಘುಗಣಕ, 0,
                       "resolver expire \"%*s\"", (ಗಾತ್ರ_t) rn->nlen, rn->name);

        ngx_ಸರತಿ_remove(q);

        ngx_rbtree_delete(ವೃಕ್ಷ, &rn->ಗಂಟು);

        ngx_resolver_free_ಗಂಟು(r, rn);
    }
}


ಸ್ಥಿರ ngx_int_t
ngx_resolver_send_query(ngx_resolver_t *r, ngx_resolver_ಗಂಟು_t *rn)
{
    ngx_int_t                   rc;
    ngx_resolver_connection_t  *rec;

    rec = r->connections.elts;
    rec = &rec[rn->ಕೊನೆಯ_connection];

    ಆದರೆ (rec->ಲಘುಗಣಕ.handler == ಖಾಲಿ) {
        rec->ಲಘುಗಣಕ = *r->ಲಘುಗಣಕ;
        rec->ಲಘುಗಣಕ.handler = ngx_resolver_log_error;
        rec->ಲಘುಗಣಕ.ದತ್ತಾಂಶ = rec;
        rec->ಲಘುಗಣಕ.action = "resolving";
    }

    ಆದರೆ (rn->query && rn->naddrs == (u_short) -1) {
        rc = rn->tcp ? ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen)
                     : ngx_resolver_send_udp_query(r, rec, rn->query, rn->qlen);

        ಆದರೆ (rc != NGX_OK) {
            ಹಿಂತಿರುಗಿ rc;
        }
    }

#ಆದರೆ (NGX_HAVE_INET6)

    ಆದರೆ (rn->query6 && rn->naddrs6 == (u_short) -1) {
        rc = rn->tcp6
                    ? ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen)
                    : ngx_resolver_send_udp_query(r, rec, rn->query6, rn->qlen);

        ಆದರೆ (rc != NGX_OK) {
            ಹಿಂತಿರುಗಿ rc;
        }
    }

#ಆದರೆ_ಅಂತ್ಯ

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ಸ್ಥಿರ ngx_int_t
ngx_resolver_send_udp_query(ngx_resolver_t *r, ngx_resolver_connection_t  *rec,
    u_char *query, u_short qlen)
{
    ssize_t  n;

    ಆದರೆ (rec->udp == ಖಾಲಿ) {
        ಆದರೆ (ngx_udp_connect(rec) != NGX_OK) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        rec->udp->ದತ್ತಾಂಶ = rec;
        rec->udp->ಪಠ->handler = ngx_resolver_udp_read;
        rec->udp->ಪಠ->resolver = 1;
    }

    n = ngx_send(rec->udp, query, qlen);

    ಆದರೆ (n == NGX_ERROR) {
        ಹೋಗು failed;
    }

    ಆದರೆ ((ಗಾತ್ರ_t) n != (ಗಾತ್ರ_t) qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->ಲಘುಗಣಕ, 0, "send() incomplete");
        ಹೋಗು failed;
    }

    ಹಿಂತಿರುಗಿ NGX_OK;

failed:

    ngx_close_connection(rec->udp);
    rec->udp = ಖಾಲಿ;

    ಹಿಂತಿರುಗಿ NGX_ERROR;
}


ಸ್ಥಿರ ngx_int_t
ngx_resolver_send_tcp_query(ngx_resolver_t *r, ngx_resolver_connection_t *rec,
    u_char *query, u_short qlen)
{
    ngx_buf_t  *b;
    ngx_int_t   rc;

    rc = NGX_OK;

    ಆದರೆ (rec->tcp == ಖಾಲಿ) {
        b = rec->read_buf;

        ಆದರೆ (b == ಖಾಲಿ) {
            b = ngx_resolver_calloc(r, ಗಾತ್ರ(ngx_buf_t));
            ಆದರೆ (b == ಖಾಲಿ) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            b->ಆರಂಭ = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_RSIZE);
            ಆದರೆ (b->ಆರಂಭ == ಖಾಲಿ) {
                ngx_resolver_free(r, b);
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            b->ಕೊನೆ = b->ಆರಂಭ + NGX_RESOLVER_TCP_RSIZE;

            rec->read_buf = b;
        }

        b->pos = b->ಆರಂಭ;
        b->ಕೊನೆಯ = b->ಆರಂಭ;

        b = rec->write_buf;

        ಆದರೆ (b == ಖಾಲಿ) {
            b = ngx_resolver_calloc(r, ಗಾತ್ರ(ngx_buf_t));
            ಆದರೆ (b == ಖಾಲಿ) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            b->ಆರಂಭ = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_WSIZE);
            ಆದರೆ (b->ಆರಂಭ == ಖಾಲಿ) {
                ngx_resolver_free(r, b);
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            b->ಕೊನೆ = b->ಆರಂಭ + NGX_RESOLVER_TCP_WSIZE;

            rec->write_buf = b;
        }

        b->pos = b->ಆರಂಭ;
        b->ಕೊನೆಯ = b->ಆರಂಭ;

        rc = ngx_tcp_connect(rec);
        ಆದರೆ (rc == NGX_ERROR) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        rec->tcp->ದತ್ತಾಂಶ = rec;
        rec->tcp->ಲಿಖ->handler = ngx_resolver_tcp_write;
        rec->tcp->ಲಿಖ->cancelable = 1;
        rec->tcp->ಪಠ->handler = ngx_resolver_tcp_read;
        rec->tcp->ಪಠ->resolver = 1;

        ngx_ಕೂಡಿಸು_timer(rec->tcp->ಲಿಖ, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    b = rec->write_buf;

    ಆದರೆ (b->ಕೊನೆ - b->ಕೊನೆಯ <  2 + qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->ಲಘುಗಣಕ, 0, "buffer overflow");
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    *b->ಕೊನೆಯ++ = (u_char) (qlen >> 8);
    *b->ಕೊನೆಯ++ = (u_char) qlen;
    b->ಕೊನೆಯ = ngx_cpymem(b->ಕೊನೆಯ, query, qlen);

    ಆದರೆ (rc == NGX_OK) {
        ngx_resolver_tcp_write(rec->tcp->ಲಿಖ);
    }

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_resolver_resend_handler(ngx_event_t *ev)
{
    ಸಮಯ_t           timer, atimer, stimer, ntimer;
#ಆದರೆ (NGX_HAVE_INET6)
    ಸಮಯ_t           a6timer;
#ಆದರೆ_ಅಂತ್ಯ
    ngx_resolver_t  *r;

    r = ev->ದತ್ತಾಂಶ;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->ಲಘುಗಣಕ, 0,
                   "resolver resend handler");

    /* lock name mutex */

    ntimer = ngx_resolver_resend(r, &r->name_rbtree, &r->name_resend_ಸರತಿ);

    stimer = ngx_resolver_resend(r, &r->srv_rbtree, &r->srv_resend_ಸರತಿ);

    /* unlock name mutex */

    /* lock addr mutex */

    atimer = ngx_resolver_resend(r, &r->addr_rbtree, &r->addr_resend_ಸರತಿ);

    /* unlock addr mutex */

#ಆದರೆ (NGX_HAVE_INET6)

    /* lock addr6 mutex */

    a6timer = ngx_resolver_resend(r, &r->addr6_rbtree, &r->addr6_resend_ಸರತಿ);

    /* unlock addr6 mutex */

#ಆದರೆ_ಅಂತ್ಯ

    timer = ntimer;

    ಆದರೆ (timer == 0) {
        timer = atimer;

    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (atimer) {
        timer = ngx_min(timer, atimer);
    }

    ಆದರೆ (timer == 0) {
        timer = stimer;

    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (stimer) {
        timer = ngx_min(timer, stimer);
    }

#ಆದರೆ (NGX_HAVE_INET6)

    ಆದರೆ (timer == 0) {
        timer = a6timer;

    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (a6timer) {
        timer = ngx_min(timer, a6timer);
    }

#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ (timer) {
        ngx_ಕೂಡಿಸು_timer(r->event, (ngx_msec_t) (timer * 1000));
    }
}


ಸ್ಥಿರ ಸಮಯ_t
ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *ವೃಕ್ಷ, ngx_ಸರತಿ_t *ಸರತಿ)
{
    ಸಮಯ_t                now;
    ngx_ಸರತಿ_t          *q;
    ngx_resolver_ಗಂಟು_t  *rn;

    now = ngx_ಸಮಯ();

    ಗಾಗಿ ( ;; ) {
        ಆದರೆ (ngx_ಸರತಿ_ಖಾಲಿಗಳು(ಸರತಿ)) {
            ಹಿಂತಿರುಗಿ 0;
        }

        q = ngx_ಸರತಿ_ಕೊನೆಯ(ಸರತಿ);

        rn = ngx_ಸರತಿ_ದತ್ತಾಂಶ(q, ngx_resolver_ಗಂಟು_t, ಸರತಿ);

        ಆದರೆ (now < rn->expire) {
            ಹಿಂತಿರುಗಿ rn->expire - now;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->ಲಘುಗಣಕ, 0,
                       "resolver resend \"%*s\" %p",
                       (ಗಾತ್ರ_t) rn->nlen, rn->name, rn->ಕಾಯುವಿಕೆ);

        ngx_ಸರತಿ_remove(q);

        ಆದರೆ (rn->ಕಾಯುವಿಕೆ) {

            ಆದರೆ (++rn->ಕೊನೆಯ_connection == r->connections.nelts) {
                rn->ಕೊನೆಯ_connection = 0;
            }

            (ಶೂನ್ಯ) ngx_resolver_send_query(r, rn);

            rn->expire = now + r->resend_timeout;

            ngx_ಸರತಿ_ಸೇರಿಸು_ತಲೆ(ಸರತಿ, q);

            ಮುಂದುವರಿ;
        }

        ngx_rbtree_delete(ವೃಕ್ಷ, &rn->ಗಂಟು);

        ngx_resolver_free_ಗಂಟು(r, rn);
    }
}


ಸ್ಥಿರ ngx_uint_t
ngx_resolver_resend_ಖಾಲಿಗಳು(ngx_resolver_t *r)
{
    ಹಿಂತಿರುಗಿ ngx_ಸರತಿ_ಖಾಲಿಗಳು(&r->name_resend_ಸರತಿ)
           && ngx_ಸರತಿ_ಖಾಲಿಗಳು(&r->srv_resend_ಸರತಿ)
#ಆದರೆ (NGX_HAVE_INET6)
           && ngx_ಸರತಿ_ಖಾಲಿಗಳು(&r->addr6_resend_ಸರತಿ)
#ಆದರೆ_ಅಂತ್ಯ
           && ngx_ಸರತಿ_ಖಾಲಿಗಳು(&r->addr_resend_ಸರತಿ);
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_resolver_udp_read(ngx_event_t *rev)
{
    ssize_t                     n;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;
    u_char                      buf[NGX_RESOLVER_UDP_ಗಾತ್ರ];

    c = rev->ದತ್ತಾಂಶ;
    rec = c->ದತ್ತಾಂಶ;

    ಮಾಡು {
        n = ngx_udp_recv(c, buf, NGX_RESOLVER_UDP_ಗಾತ್ರ);

        ಆದರೆ (n == NGX_AGAIN) {
            ನಿಲ್ಲಿಸು;
        }

        ಆದರೆ (n == NGX_ERROR) {
            ಹೋಗು failed;
        }

        ngx_resolver_process_response(rec->resolver, buf, n, 0);

    } ಆಗ (rev->ready);

    ಆದರೆ (ngx_handle_read_event(rev, 0) != NGX_OK) {
        ಹೋಗು failed;
    }

    ಹಿಂತಿರುಗಿ;

failed:

    ngx_close_connection(rec->udp);
    rec->udp = ಖಾಲಿ;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_resolver_tcp_write(ngx_event_t *wev)
{
    off_t                       ಕಳುಹಿಸಿದ;
    ssize_t                     n;
    ngx_buf_t                  *b;
    ngx_resolver_t             *r;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;

    c = wev->ದತ್ತಾಂಶ;
    rec = c->ದತ್ತಾಂಶ;
    b = rec->write_buf;
    r = rec->resolver;

    ಆದರೆ (wev->timedout) {
        ಹೋಗು failed;
    }

    ಕಳುಹಿಸಿದ = c->ಕಳುಹಿಸಿದ;

    ಆಗ (wev->ready && b->pos < b->ಕೊನೆಯ) {
        n = ngx_send(c, b->pos, b->ಕೊನೆಯ - b->pos);

        ಆದರೆ (n == NGX_AGAIN) {
            ನಿಲ್ಲಿಸು;
        }

        ಆದರೆ (n == NGX_ERROR) {
            ಹೋಗು failed;
        }

        b->pos += n;
    }

    ಆದರೆ (b->pos != b->ಆರಂಭ) {
        b->ಕೊನೆಯ = ngx_movemem(b->ಆರಂಭ, b->pos, b->ಕೊನೆಯ - b->pos);
        b->pos = b->ಆರಂಭ;
    }

    ಆದರೆ (c->ಕಳುಹಿಸಿದ != ಕಳುಹಿಸಿದ) {
        ngx_ಕೂಡಿಸು_timer(wev, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    ಆದರೆ (ngx_handle_write_event(wev, 0) != NGX_OK) {
        ಹೋಗು failed;
    }

    ಹಿಂತಿರುಗಿ;

failed:

    ngx_close_connection(c);
    rec->tcp = ಖಾಲಿ;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_resolver_tcp_read(ngx_event_t *rev)
{
    u_char                     *p;
    ಗಾತ್ರ_t                      ಗಾತ್ರ_2;
    ssize_t                     n;
    u_short                     qlen;
    ngx_buf_t                  *b;
    ngx_resolver_t             *r;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;

    c = rev->ದತ್ತಾಂಶ;
    rec = c->ದತ್ತಾಂಶ;
    b = rec->read_buf;
    r = rec->resolver;

    ಆಗ (rev->ready) {
        n = ngx_recv(c, b->ಕೊನೆಯ, b->ಕೊನೆ - b->ಕೊನೆಯ);

        ಆದರೆ (n == NGX_AGAIN) {
            ನಿಲ್ಲಿಸು;
        }

        ಆದರೆ (n == NGX_ERROR || n == 0) {
            ಹೋಗು failed;
        }

        b->ಕೊನೆಯ += n;

        ಗಾಗಿ ( ;; ) {
            p = b->pos;
            ಗಾತ್ರ_2 = b->ಕೊನೆಯ - p;

            ಆದರೆ (ಗಾತ್ರ_2 < 2) {
                ನಿಲ್ಲಿಸು;
            }

            qlen = (u_short) *p++ << 8;
            qlen += *p++;

            ಆದರೆ (ಗಾತ್ರ_2 < (ಗಾತ್ರ_t) (2 + qlen)) {
                ನಿಲ್ಲಿಸು;
            }

            ngx_resolver_process_response(r, p, qlen, 1);

            b->pos += 2 + qlen;
        }

        ಆದರೆ (b->pos != b->ಆರಂಭ) {
            b->ಕೊನೆಯ = ngx_movemem(b->ಆರಂಭ, b->pos, b->ಕೊನೆಯ - b->pos);
            b->pos = b->ಆರಂಭ;
        }
    }

    ಆದರೆ (ngx_handle_read_event(rev, 0) != NGX_OK) {
        ಹೋಗು failed;
    }

    ಹಿಂತಿರುಗಿ;

failed:

    ngx_close_connection(c);
    rec->tcp = ಖಾಲಿ;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf, ಗಾತ್ರ_t n,
    ngx_uint_t tcp)
{
    ಅಕ್ಷರ                 *err;
    ngx_uint_t            i, times, ident, qident, flags, ಸಂಕೇತ_2, nqs, nan, trunc,
                          qtype, qclass;
#ಆದರೆ (NGX_HAVE_INET6)
    ngx_uint_t            qident6;
#ಆದರೆ_ಅಂತ್ಯ
    ngx_ಸರತಿ_t          *q;
    ngx_resolver_qs_t    *qs;
    ngx_resolver_hdr_t   *response;
    ngx_resolver_ಗಂಟು_t  *rn;

    ಆದರೆ (n < ಗಾತ್ರ(ngx_resolver_hdr_t)) {
        ಹೋಗು short_response;
    }

    response = (ngx_resolver_hdr_t *) buf;

    ident = (response->ident_hi << 8) + response->ident_lo;
    flags = (response->flags_hi << 8) + response->flags_lo;
    nqs = (response->nqs_hi << 8) + response->nqs_lo;
    nan = (response->nan_hi << 8) + response->nan_lo;
    trunc = flags & 0x0200;

    ngx_log_debug6(NGX_LOG_DEBUG_CORE, r->ಲಘುಗಣಕ, 0,
                   "resolver DNS response %ui fl:%04Xi %ui/%ui/%ud/%ud",
                   ident, flags, nqs, nan,
                   (response->nns_hi << 8) + response->nns_lo,
                   (response->nar_hi << 8) + response->nar_lo);

    /* response to a standard query */
    ಆದರೆ ((flags & 0xf870) != 0x8000 || (trunc && tcp)) {
        ngx_log_error(r->log_level, r->ಲಘುಗಣಕ, 0,
                      "invalid %s DNS response %ui fl:%04Xi",
                      tcp ? "TCP" : "UDP", ident, flags);
        ಹಿಂತಿರುಗಿ;
    }

    ಸಂಕೇತ_2 = flags & 0xf;

    ಆದರೆ (ಸಂಕೇತ_2 == NGX_RESOLVE_FORMERR) {

        times = 0;

        ಗಾಗಿ (q = ngx_ಸರತಿ_ತಲೆ(&r->name_resend_ಸರತಿ);
             q != ngx_ಸರತಿ_sentinel(&r->name_resend_ಸರತಿ) && times++ < 100;
             q = ngx_ಸರತಿ_ಮುಂದಿನ(q))
        {
            rn = ngx_ಸರತಿ_ದತ್ತಾಂಶ(q, ngx_resolver_ಗಂಟು_t, ಸರತಿ);

            ಆದರೆ (rn->query) {
                qident = (rn->query[0] << 8) + rn->query[1];

                ಆದರೆ (qident == ident) {
                    ಹೋಗು dns_error_name;
                }
            }

#ಆದರೆ (NGX_HAVE_INET6)
            ಆದರೆ (rn->query6) {
                qident6 = (rn->query6[0] << 8) + rn->query6[1];

                ಆದರೆ (qident6 == ident) {
                    ಹೋಗು dns_error_name;
                }
            }
#ಆದರೆ_ಅಂತ್ಯ
        }

        ಹೋಗು dns_error;
    }

    ಆದರೆ (ಸಂಕೇತ_2 > NGX_RESOLVE_REFUSED) {
        ಹೋಗು dns_error;
    }

    ಆದರೆ (nqs != 1) {
        err = "invalid number of questions in DNS response";
        ಹೋಗು ಮುಗಿದ;
    }

    i = ಗಾತ್ರ(ngx_resolver_hdr_t);

    ಆಗ (i < (ngx_uint_t) n) {

        ಆದರೆ (buf[i] & 0xc0) {
            err = "unexpected compression pointer in DNS response";
            ಹೋಗು ಮುಗಿದ;
        }

        ಆದರೆ (buf[i] == '\0') {
            ಹೋಗು ಸಿಕ್ಕಿತು;
        }

        i += 1 + buf[i];
    }

    ಹೋಗು short_response;

ಸಿಕ್ಕಿತು:

    ಆದರೆ (i++ == ಗಾತ್ರ(ngx_resolver_hdr_t)) {
        err = "zero-length domain name in DNS response";
        ಹೋಗು ಮುಗಿದ;
    }

    ಆದರೆ (i + ಗಾತ್ರ(ngx_resolver_qs_t) + nan * (2 + ಗಾತ್ರ(ngx_resolver_an_t))
        > (ngx_uint_t) n)
    {
        ಹೋಗು short_response;
    }

    qs = (ngx_resolver_qs_t *) &buf[i];

    qtype = (qs->type_hi << 8) + qs->type_lo;
    qclass = (qs->class_hi << 8) + qs->class_lo;

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->ಲಘುಗಣಕ, 0,
                   "resolver DNS response qt:%ui cl:%ui", qtype, qclass);

    ಆದರೆ (qclass != 1) {
        ngx_log_error(r->log_level, r->ಲಘುಗಣಕ, 0,
                      "unknown query class %ui in DNS response", qclass);
        ಹಿಂತಿರುಗಿ;
    }

    ಆಯ್ಕೆ (qtype) {

    ಸ್ಥಿತಿ NGX_RESOLVE_A:
#ಆದರೆ (NGX_HAVE_INET6)
    ಸ್ಥಿತಿ NGX_RESOLVE_AAAA:
#ಆದರೆ_ಅಂತ್ಯ

        ngx_resolver_process_a(r, buf, n, ident, ಸಂಕೇತ_2, qtype, nan, trunc,
                               i + ಗಾತ್ರ(ngx_resolver_qs_t));

        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ NGX_RESOLVE_SRV:

        ngx_resolver_process_srv(r, buf, n, ident, ಸಂಕೇತ_2, nan, trunc,
                                 i + ಗಾತ್ರ(ngx_resolver_qs_t));

        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ NGX_RESOLVE_PTR:

        ngx_resolver_process_ptr(r, buf, n, ident, ಸಂಕೇತ_2, nan);

        ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:
        ngx_log_error(r->log_level, r->ಲಘುಗಣಕ, 0,
                      "unknown query type %ui in DNS response", qtype);
        ಹಿಂತಿರುಗಿ;
    }

    ಹಿಂತಿರುಗಿ;

short_response:

    err = "short DNS response";

ಮುಗಿದ:

    ngx_log_error(r->log_level, r->ಲಘುಗಣಕ, 0, err);

    ಹಿಂತಿರುಗಿ;

dns_error_name:

    ngx_log_error(r->log_level, r->ಲಘುಗಣಕ, 0,
                  "DNS error (%ui: %s), query id:%ui, name:\"%*s\"",
                  ಸಂಕೇತ_2, ngx_resolver_strerror(ಸಂಕೇತ_2), ident,
                  (ಗಾತ್ರ_t) rn->nlen, rn->name);
    ಹಿಂತಿರುಗಿ;

dns_error:

    ngx_log_error(r->log_level, r->ಲಘುಗಣಕ, 0,
                  "DNS error (%ui: %s), query id:%ui",
                  ಸಂಕೇತ_2, ngx_resolver_strerror(ಸಂಕೇತ_2), ident);
    ಹಿಂತಿರುಗಿ;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, ಗಾತ್ರ_t n,
    ngx_uint_t ident, ngx_uint_t ಸಂಕೇತ_2, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans)
{
    ಅಕ್ಷರ                       *err;
    u_char                     *cname;
    ಗಾತ್ರ_t                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    ಒಳ_addr_t                  *addr;
    ngx_str_t                   name;
    ngx_uint_t                  type, class, qident, naddrs, a, i, j, ಆರಂಭ;
#ಆದರೆ (NGX_HAVE_INET6)
    ರಚನೆ in6_addr            *addr6;
#ಆದರೆ_ಅಂತ್ಯ
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *ಮುಂದಿನ;
    ngx_resolver_ಗಂಟು_t        *rn;
    ngx_resolver_addr_t        *addrs;
    ngx_resolver_connection_t  *rec;

    ಆದರೆ (ngx_resolver_copy(r, &name, buf,
                          buf + ಗಾತ್ರ(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        ಹಿಂತಿರುಗಿ;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->ಲಘುಗಣಕ, 0, "resolver qs:%V", &name);

    hash = ngx_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32_short(name.ದತ್ತಾಂಶ, name.len);

    /* lock name mutex */

    rn = ngx_resolver_lookup_name(r, &name, hash);

    ಆದರೆ (rn == ಖಾಲಿ) {
        ngx_log_error(r->log_level, r->ಲಘುಗಣಕ, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.ದತ್ತಾಂಶ);
        ಹೋಗು failed;
    }

    ಆಯ್ಕೆ (qtype) {

#ಆದರೆ (NGX_HAVE_INET6)
    ಸ್ಥಿತಿ NGX_RESOLVE_AAAA:

        ಆದರೆ (rn->query6 == ಖಾಲಿ || rn->naddrs6 != (u_short) -1) {
            ngx_log_error(r->log_level, r->ಲಘುಗಣಕ, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.ದತ್ತಾಂಶ);
            ಹೋಗು failed;
        }

        ಆದರೆ (trunc && rn->tcp6) {
            ngx_resolver_free(r, name.ದತ್ತಾಂಶ);
            ಹೋಗು failed;
        }

        qident = (rn->query6[0] << 8) + rn->query6[1];

        ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ

    ಪೂರ್ವನಿಯೋಜಿತ: /* NGX_RESOLVE_A */

        ಆದರೆ (rn->query == ಖಾಲಿ || rn->naddrs != (u_short) -1) {
            ngx_log_error(r->log_level, r->ಲಘುಗಣಕ, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.ದತ್ತಾಂಶ);
            ಹೋಗು failed;
        }

        ಆದರೆ (trunc && rn->tcp) {
            ngx_resolver_free(r, name.ದತ್ತಾಂಶ);
            ಹೋಗು failed;
        }

        qident = (rn->query[0] << 8) + rn->query[1];
    }

    ಆದರೆ (ident != qident) {
        ngx_log_error(r->log_level, r->ಲಘುಗಣಕ, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.ದತ್ತಾಂಶ);
        ಹೋಗು failed;
    }

    ngx_resolver_free(r, name.ದತ್ತಾಂಶ);

    ಆದರೆ (trunc) {

        ngx_ಸರತಿ_remove(&rn->ಸರತಿ);

        ಆದರೆ (rn->ಕಾಯುವಿಕೆ == ಖಾಲಿ) {
            ngx_rbtree_delete(&r->name_rbtree, &rn->ಗಂಟು);
            ngx_resolver_free_ಗಂಟು(r, rn);
            ಹೋಗು ಮುಂದಿನ;
        }

        rec = r->connections.elts;
        rec = &rec[rn->ಕೊನೆಯ_connection];

        ಆಯ್ಕೆ (qtype) {

#ಆದರೆ (NGX_HAVE_INET6)
        ಸ್ಥಿತಿ NGX_RESOLVE_AAAA:

            rn->tcp6 = 1;

            (ಶೂನ್ಯ) ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen);

            ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ

        ಪೂರ್ವನಿಯೋಜಿತ: /* NGX_RESOLVE_A */

            rn->tcp = 1;

            (ಶೂನ್ಯ) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);
        }

        rn->expire = ngx_ಸಮಯ() + r->resend_timeout;

        ngx_ಸರತಿ_ಸೇರಿಸು_ತಲೆ(&r->name_resend_ಸರತಿ, &rn->ಸರತಿ);

        ಹೋಗು ಮುಂದಿನ;
    }

    ಆದರೆ (ಸಂಕೇತ_2 == 0 && rn->ಸಂಕೇತ_2) {
        ಸಂಕೇತ_2 = rn->ಸಂಕೇತ_2;
    }

    ಆದರೆ (ಸಂಕೇತ_2 == 0 && nan == 0) {

#ಆದರೆ (NGX_HAVE_INET6)
        ಆಯ್ಕೆ (qtype) {

        ಸ್ಥಿತಿ NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            ಆದರೆ (rn->naddrs == (u_short) -1) {
                ಹೋಗು ಮುಂದಿನ;
            }

            ಆದರೆ (rn->naddrs) {
                ಹೋಗು export;
            }

            ನಿಲ್ಲಿಸು;

        ಪೂರ್ವನಿಯೋಜಿತ: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            ಆದರೆ (rn->naddrs6 == (u_short) -1) {
                ಹೋಗು ಮುಂದಿನ;
            }

            ಆದರೆ (rn->naddrs6) {
                ಹೋಗು export;
            }
        }
#ಆದರೆ_ಅಂತ್ಯ

        ಸಂಕೇತ_2 = NGX_RESOLVE_NXDOMAIN;
    }

    ಆದರೆ (ಸಂಕೇತ_2) {

#ಆದರೆ (NGX_HAVE_INET6)
        ಆಯ್ಕೆ (qtype) {

        ಸ್ಥಿತಿ NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            ಆದರೆ (rn->naddrs == (u_short) -1) {
                rn->ಸಂಕೇತ_2 = (u_char) ಸಂಕೇತ_2;
                ಹೋಗು ಮುಂದಿನ;
            }

            ನಿಲ್ಲಿಸು;

        ಪೂರ್ವನಿಯೋಜಿತ: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            ಆದರೆ (rn->naddrs6 == (u_short) -1) {
                rn->ಸಂಕೇತ_2 = (u_char) ಸಂಕೇತ_2;
                ಹೋಗು ಮುಂದಿನ;
            }
        }
#ಆದರೆ_ಅಂತ್ಯ

        ಮುಂದಿನ = rn->ಕಾಯುವಿಕೆ;
        rn->ಕಾಯುವಿಕೆ = ಖಾಲಿ;

        ngx_ಸರತಿ_remove(&rn->ಸರತಿ);

        ngx_rbtree_delete(&r->name_rbtree, &rn->ಗಂಟು);

        /* unlock name mutex */

        ಆಗ (ಮುಂದಿನ) {
            ctx = ಮುಂದಿನ;
            ctx->state = ಸಂಕೇತ_2;
            ctx->valid = ngx_ಸಮಯ() + (r->valid ? r->valid : 10);
            ಮುಂದಿನ = ctx->ಮುಂದಿನ;

            ctx->handler(ctx);
        }

        ngx_resolver_free_ಗಂಟು(r, rn);

        ಹಿಂತಿರುಗಿ;
    }

    i = ans;
    naddrs = 0;
    cname = ಖಾಲಿ;

    ಗಾಗಿ (a = 0; a < nan; a++) {

        ಆರಂಭ = i;

        ಆಗ (i < n) {

            ಆದರೆ (buf[i] & 0xc0) {
                i += 2;
                ಹೋಗು ಸಿಕ್ಕಿತು;
            }

            ಆದರೆ (buf[i] == 0) {
                i++;
                ಹೋಗು test_ಉದ್ದಳತೆ;
            }

            i += 1 + buf[i];
        }

        ಹೋಗು short_response;

    test_ಉದ್ದಳತೆ:

        ಆದರೆ (i - ಆರಂಭ < 2) {
            err = "invalid name in DNS response";
            ಹೋಗು invalid;
        }

    ಸಿಕ್ಕಿತು:

        ಆದರೆ (i + ಗಾತ್ರ(ngx_resolver_an_t) >= n) {
            ಹೋಗು short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        ಆದರೆ (class != 1) {
            ngx_log_error(r->log_level, r->ಲಘುಗಣಕ, 0,
                          "unexpected RR class %ui in DNS response", class);
            ಹೋಗು failed;
        }

        ಆದರೆ (ttl < 0) {
            ttl = 0;
        }

        rn->ttl = ngx_min(rn->ttl, (uint32_t) ttl);

        i += ಗಾತ್ರ(ngx_resolver_an_t);

        ಆಯ್ಕೆ (type) {

        ಸ್ಥಿತಿ NGX_RESOLVE_A:

            ಆದರೆ (qtype != NGX_RESOLVE_A) {
                err = "unexpected A record in DNS response";
                ಹೋಗು invalid;
            }

            ಆದರೆ (len != 4) {
                err = "invalid A record in DNS response";
                ಹೋಗು invalid;
            }

            ಆದರೆ (i + 4 > n) {
                ಹೋಗು short_response;
            }

            naddrs++;

            ನಿಲ್ಲಿಸು;

#ಆದರೆ (NGX_HAVE_INET6)
        ಸ್ಥಿತಿ NGX_RESOLVE_AAAA:

            ಆದರೆ (qtype != NGX_RESOLVE_AAAA) {
                err = "unexpected AAAA record in DNS response";
                ಹೋಗು invalid;
            }

            ಆದರೆ (len != 16) {
                err = "invalid AAAA record in DNS response";
                ಹೋಗು invalid;
            }

            ಆದರೆ (i + 16 > n) {
                ಹೋಗು short_response;
            }

            naddrs++;

            ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ

        ಸ್ಥಿತಿ NGX_RESOLVE_CNAME:

            cname = &buf[i];

            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ NGX_RESOLVE_DNAME:

            ನಿಲ್ಲಿಸು;

        ಪೂರ್ವನಿಯೋಜಿತ:

            ngx_log_error(r->log_level, r->ಲಘುಗಣಕ, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->ಲಘುಗಣಕ, 0,
                   "resolver naddrs:%ui cname:%p ttl:%uD",
                   naddrs, cname, rn->ttl);

    ಆದರೆ (naddrs) {

        ಆಯ್ಕೆ (qtype) {

#ಆದರೆ (NGX_HAVE_INET6)
        ಸ್ಥಿತಿ NGX_RESOLVE_AAAA:

            ಆದರೆ (naddrs == 1) {
                addr6 = &rn->u6.addr6;
                rn->naddrs6 = 1;

            } ಇಲ್ಲದಿದ್ದರೆ {
                addr6 = ngx_resolver_alloc(r, naddrs * ಗಾತ್ರ(ರಚನೆ in6_addr));
                ಆದರೆ (addr6 == ಖಾಲಿ) {
                    ಹೋಗು failed;
                }

                rn->u6.addrs6 = addr6;
                rn->naddrs6 = (u_short) naddrs;
            }

#ಆದರೆ (NGX_SUPPRESS_WARN)
            addr = ಖಾಲಿ;
#ಆದರೆ_ಅಂತ್ಯ

            ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ

        ಪೂರ್ವನಿಯೋಜಿತ: /* NGX_RESOLVE_A */

            ಆದರೆ (naddrs == 1) {
                addr = &rn->u.addr;
                rn->naddrs = 1;

            } ಇಲ್ಲದಿದ್ದರೆ {
                addr = ngx_resolver_alloc(r, naddrs * ಗಾತ್ರ(ಒಳ_addr_t));
                ಆದರೆ (addr == ಖಾಲಿ) {
                    ಹೋಗು failed;
                }

                rn->u.addrs = addr;
                rn->naddrs = (u_short) naddrs;
            }

#ಆದರೆ (NGX_HAVE_INET6 && NGX_SUPPRESS_WARN)
            addr6 = ಖಾಲಿ;
#ಆದರೆ_ಅಂತ್ಯ
        }

        j = 0;
        i = ans;

        ಗಾಗಿ (a = 0; a < nan; a++) {

            ಗಾಗಿ ( ;; ) {

                ಆದರೆ (buf[i] & 0xc0) {
                    i += 2;
                    ನಿಲ್ಲಿಸು;
                }

                ಆದರೆ (buf[i] == 0) {
                    i++;
                    ನಿಲ್ಲಿಸು;
                }

                i += 1 + buf[i];
            }

            an = (ngx_resolver_an_t *) &buf[i];

            type = (an->type_hi << 8) + an->type_lo;
            len = (an->len_hi << 8) + an->len_lo;

            i += ಗಾತ್ರ(ngx_resolver_an_t);

            ಆದರೆ (type == NGX_RESOLVE_A) {

                addr[j] = htonl((buf[i] << 24) + (buf[i + 1] << 16)
                                + (buf[i + 2] << 8) + (buf[i + 3]));

                ಆದರೆ (++j == naddrs) {

#ಆದರೆ (NGX_HAVE_INET6)
                    ಆದರೆ (rn->naddrs6 == (u_short) -1) {
                        ಹೋಗು ಮುಂದಿನ;
                    }
#ಆದರೆ_ಅಂತ್ಯ

                    ನಿಲ್ಲಿಸು;
                }
            }

#ಆದರೆ (NGX_HAVE_INET6)
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (type == NGX_RESOLVE_AAAA) {

                ngx_memcpy(addr6[j].s6_addr, &buf[i], 16);

                ಆದರೆ (++j == naddrs) {

                    ಆದರೆ (rn->naddrs == (u_short) -1) {
                        ಹೋಗು ಮುಂದಿನ;
                    }

                    ನಿಲ್ಲಿಸು;
                }
            }
#ಆದರೆ_ಅಂತ್ಯ

            i += len;
        }
    }

    ಆಯ್ಕೆ (qtype) {

#ಆದರೆ (NGX_HAVE_INET6)
    ಸ್ಥಿತಿ NGX_RESOLVE_AAAA:

        ಆದರೆ (rn->naddrs6 == (u_short) -1) {
            rn->naddrs6 = 0;
        }

        ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ

    ಪೂರ್ವನಿಯೋಜಿತ: /* NGX_RESOLVE_A */

        ಆದರೆ (rn->naddrs == (u_short) -1) {
            rn->naddrs = 0;
        }
    }

    ಆದರೆ (rn->naddrs != (u_short) -1
#ಆದರೆ (NGX_HAVE_INET6)
        && rn->naddrs6 != (u_short) -1
#ಆದರೆ_ಅಂತ್ಯ
        && rn->naddrs
#ಆದರೆ (NGX_HAVE_INET6)
           + rn->naddrs6
#ಆದರೆ_ಅಂತ್ಯ
           > 0)
    {

#ಆದರೆ (NGX_HAVE_INET6)
    export:
#ಆದರೆ_ಅಂತ್ಯ

        naddrs = rn->naddrs;
#ಆದರೆ (NGX_HAVE_INET6)
        naddrs += rn->naddrs6;
#ಆದರೆ_ಅಂತ್ಯ

        ಆದರೆ (naddrs == 1 && rn->naddrs == 1) {
            addrs = ಖಾಲಿ;

        } ಇಲ್ಲದಿದ್ದರೆ {
            addrs = ngx_resolver_export(r, rn, 0);
            ಆದರೆ (addrs == ಖಾಲಿ) {
                ಹೋಗು failed;
            }
        }

        ngx_ಸರತಿ_remove(&rn->ಸರತಿ);

        rn->valid = ngx_ಸಮಯ() + (r->valid ? r->valid : (ಸಮಯ_t) rn->ttl);
        rn->expire = ngx_ಸಮಯ() + r->expire;

        ngx_ಸರತಿ_ಸೇರಿಸು_ತಲೆ(&r->name_expire_ಸರತಿ, &rn->ಸರತಿ);

        ಮುಂದಿನ = rn->ಕಾಯುವಿಕೆ;
        rn->ಕಾಯುವಿಕೆ = ಖಾಲಿ;

        /* unlock name mutex */

        ಆಗ (ಮುಂದಿನ) {
            ctx = ಮುಂದಿನ;
            ctx->state = NGX_OK;
            ctx->valid = rn->valid;
            ctx->naddrs = naddrs;

            ಆದರೆ (addrs == ಖಾಲಿ) {
                ctx->addrs = &ctx->addr;
                ctx->addr.sockaddr = (ರಚನೆ sockaddr *) &ctx->ಜ್ಯಾ;
                ctx->addr.socklen = ಗಾತ್ರ(ರಚನೆ sockaddr_ಒಳ);
                ngx_memzero(&ctx->ಜ್ಯಾ, ಗಾತ್ರ(ರಚನೆ sockaddr_ಒಳ));
                ctx->ಜ್ಯಾ.sin_family = AF_INET;
                ctx->ಜ್ಯಾ.sin_addr.s_addr = rn->u.addr;

            } ಇಲ್ಲದಿದ್ದರೆ {
                ctx->addrs = addrs;
            }

            ಮುಂದಿನ = ctx->ಮುಂದಿನ;

            ctx->handler(ctx);
        }

        ಆದರೆ (addrs != ಖಾಲಿ) {
            ngx_resolver_free(r, addrs->sockaddr);
            ngx_resolver_free(r, addrs);
        }

        ngx_resolver_free(r, rn->query);
        rn->query = ಖಾಲಿ;
#ಆದರೆ (NGX_HAVE_INET6)
        rn->query6 = ಖಾಲಿ;
#ಆದರೆ_ಅಂತ್ಯ

        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ (cname) {

        /* CNAME only */

        ಆದರೆ (rn->naddrs == (u_short) -1
#ಆದರೆ (NGX_HAVE_INET6)
            || rn->naddrs6 == (u_short) -1
#ಆದರೆ_ಅಂತ್ಯ
            )
        {
            ಹೋಗು ಮುಂದಿನ;
        }

        ಆದರೆ (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            ಹೋಗು failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->ಲಘುಗಣಕ, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_ಸರತಿ_remove(&rn->ಸರತಿ);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.ದತ್ತಾಂಶ;

        rn->valid = ngx_ಸಮಯ() + (r->valid ? r->valid : (ಸಮಯ_t) rn->ttl);
        rn->expire = ngx_ಸಮಯ() + r->expire;

        ngx_ಸರತಿ_ಸೇರಿಸು_ತಲೆ(&r->name_expire_ಸರತಿ, &rn->ಸರತಿ);

        ngx_resolver_free(r, rn->query);
        rn->query = ಖಾಲಿ;
#ಆದರೆ (NGX_HAVE_INET6)
        rn->query6 = ಖಾಲಿ;
#ಆದರೆ_ಅಂತ್ಯ

        ctx = rn->ಕಾಯುವಿಕೆ;
        rn->ಕಾಯುವಿಕೆ = ಖಾಲಿ;

        ಆದರೆ (ctx) {

            ಆದರೆ (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                ಮಾಡು {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    ಮುಂದಿನ = ctx->ಮುಂದಿನ;

                    ctx->handler(ctx);

                    ctx = ಮುಂದಿನ;
                } ಆಗ (ctx);

                ಹಿಂತಿರುಗಿ;
            }

            ಗಾಗಿ (ಮುಂದಿನ = ctx; ಮುಂದಿನ; ಮುಂದಿನ = ಮುಂದಿನ->ಮುಂದಿನ) {
                ಮುಂದಿನ->ಗಂಟು = ಖಾಲಿ;
            }

            (ಶೂನ್ಯ) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        ಹಿಂತಿರುಗಿ;
    }

    ngx_log_error(r->log_level, r->ಲಘುಗಣಕ, 0,
                  "no A or CNAME types in DNS response");
    ಹಿಂತಿರುಗಿ;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->ಲಘುಗಣಕ, 0, err);

    ಹಿಂತಿರುಗಿ;

failed:

ಮುಂದಿನ:

    /* unlock name mutex */

    ಹಿಂತಿರುಗಿ;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, ಗಾತ್ರ_t n,
    ngx_uint_t ident, ngx_uint_t ಸಂಕೇತ_2, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans)
{
    ಅಕ್ಷರ                       *err;
    u_char                     *cname;
    ಗಾತ್ರ_t                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    ngx_str_t                   name;
    ngx_uint_t                  type, qident, class, ಆರಂಭ, nsrvs, a, i, j;
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *ಮುಂದಿನ;
    ngx_resolver_srv_t         *srvs;
    ngx_resolver_ಗಂಟು_t        *rn;
    ngx_resolver_connection_t  *rec;

    ಆದರೆ (ngx_resolver_copy(r, &name, buf,
                          buf + ಗಾತ್ರ(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        ಹಿಂತಿರುಗಿ;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->ಲಘುಗಣಕ, 0, "resolver qs:%V", &name);

    hash = ngx_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32_short(name.ದತ್ತಾಂಶ, name.len);

    rn = ngx_resolver_lookup_srv(r, &name, hash);

    ಆದರೆ (rn == ಖಾಲಿ || rn->query == ಖಾಲಿ) {
        ngx_log_error(r->log_level, r->ಲಘುಗಣಕ, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.ದತ್ತಾಂಶ);
        ಹೋಗು failed;
    }

    ಆದರೆ (trunc && rn->tcp) {
        ngx_resolver_free(r, name.ದತ್ತಾಂಶ);
        ಹೋಗು failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    ಆದರೆ (ident != qident) {
        ngx_log_error(r->log_level, r->ಲಘುಗಣಕ, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.ದತ್ತಾಂಶ);
        ಹೋಗು failed;
    }

    ngx_resolver_free(r, name.ದತ್ತಾಂಶ);

    ಆದರೆ (trunc) {

        ngx_ಸರತಿ_remove(&rn->ಸರತಿ);

        ಆದರೆ (rn->ಕಾಯುವಿಕೆ == ಖಾಲಿ) {
            ngx_rbtree_delete(&r->srv_rbtree, &rn->ಗಂಟು);
            ngx_resolver_free_ಗಂಟು(r, rn);
            ಹಿಂತಿರುಗಿ;
        }

        rec = r->connections.elts;
        rec = &rec[rn->ಕೊನೆಯ_connection];

        rn->tcp = 1;

        (ಶೂನ್ಯ) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);

        rn->expire = ngx_ಸಮಯ() + r->resend_timeout;

        ngx_ಸರತಿ_ಸೇರಿಸು_ತಲೆ(&r->srv_resend_ಸರತಿ, &rn->ಸರತಿ);

        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ (ಸಂಕೇತ_2 == 0 && rn->ಸಂಕೇತ_2) {
        ಸಂಕೇತ_2 = rn->ಸಂಕೇತ_2;
    }

    ಆದರೆ (ಸಂಕೇತ_2 == 0 && nan == 0) {
        ಸಂಕೇತ_2 = NGX_RESOLVE_NXDOMAIN;
    }

    ಆದರೆ (ಸಂಕೇತ_2) {
        ಮುಂದಿನ = rn->ಕಾಯುವಿಕೆ;
        rn->ಕಾಯುವಿಕೆ = ಖಾಲಿ;

        ngx_ಸರತಿ_remove(&rn->ಸರತಿ);

        ngx_rbtree_delete(&r->srv_rbtree, &rn->ಗಂಟು);

        ಆಗ (ಮುಂದಿನ) {
            ctx = ಮುಂದಿನ;
            ctx->state = ಸಂಕೇತ_2;
            ctx->valid = ngx_ಸಮಯ() + (r->valid ? r->valid : 10);
            ಮುಂದಿನ = ctx->ಮುಂದಿನ;

            ctx->handler(ctx);
        }

        ngx_resolver_free_ಗಂಟು(r, rn);

        ಹಿಂತಿರುಗಿ;
    }

    i = ans;
    nsrvs = 0;
    cname = ಖಾಲಿ;

    ಗಾಗಿ (a = 0; a < nan; a++) {

        ಆರಂಭ = i;

        ಆಗ (i < n) {

            ಆದರೆ (buf[i] & 0xc0) {
                i += 2;
                ಹೋಗು ಸಿಕ್ಕಿತು;
            }

            ಆದರೆ (buf[i] == 0) {
                i++;
                ಹೋಗು test_ಉದ್ದಳತೆ;
            }

            i += 1 + buf[i];
        }

        ಹೋಗು short_response;

    test_ಉದ್ದಳತೆ:

        ಆದರೆ (i - ಆರಂಭ < 2) {
            err = "invalid name DNS response";
            ಹೋಗು invalid;
        }

    ಸಿಕ್ಕಿತು:

        ಆದರೆ (i + ಗಾತ್ರ(ngx_resolver_an_t) >= n) {
            ಹೋಗು short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        ಆದರೆ (class != 1) {
            ngx_log_error(r->log_level, r->ಲಘುಗಣಕ, 0,
                          "unexpected RR class %ui in DNS response", class);
            ಹೋಗು failed;
        }

        ಆದರೆ (ttl < 0) {
            ttl = 0;
        }

        rn->ttl = ngx_min(rn->ttl, (uint32_t) ttl);

        i += ಗಾತ್ರ(ngx_resolver_an_t);

        ಆಯ್ಕೆ (type) {

        ಸ್ಥಿತಿ NGX_RESOLVE_SRV:

            ಆದರೆ (i + 6 >= n) {
                ಹೋಗು short_response;
            }

            ಆದರೆ (ngx_resolver_copy(r, ಖಾಲಿ, buf, &buf[i + 6], buf + n)
                != NGX_OK)
            {
                ಹೋಗು failed;
            }

            nsrvs++;

            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ NGX_RESOLVE_CNAME:

            cname = &buf[i];

            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ NGX_RESOLVE_DNAME:

            ನಿಲ್ಲಿಸು;

        ಪೂರ್ವನಿಯೋಜಿತ:

            ngx_log_error(r->log_level, r->ಲಘುಗಣಕ, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->ಲಘುಗಣಕ, 0,
                   "resolver nsrvs:%ui cname:%p ttl:%uD",
                   nsrvs, cname, rn->ttl);

    ಆದರೆ (nsrvs) {

        srvs = ngx_resolver_calloc(r, nsrvs * ಗಾತ್ರ(ngx_resolver_srv_t));
        ಆದರೆ (srvs == ಖಾಲಿ) {
            ಹೋಗು failed;
        }

        rn->u.srvs = srvs;
        rn->nsrvs = (u_short) nsrvs;

        j = 0;
        i = ans;

        ಗಾಗಿ (a = 0; a < nan; a++) {

            ಗಾಗಿ ( ;; ) {

                ಆದರೆ (buf[i] & 0xc0) {
                    i += 2;
                    ನಿಲ್ಲಿಸು;
                }

                ಆದರೆ (buf[i] == 0) {
                    i++;
                    ನಿಲ್ಲಿಸು;
                }

                i += 1 + buf[i];
            }

            an = (ngx_resolver_an_t *) &buf[i];

            type = (an->type_hi << 8) + an->type_lo;
            len = (an->len_hi << 8) + an->len_lo;

            i += ಗಾತ್ರ(ngx_resolver_an_t);

            ಆದರೆ (type == NGX_RESOLVE_SRV) {

                srvs[j].ಆದ್ಯತೆ = (buf[i] << 8) + buf[i + 1];
                srvs[j].ತೂಕ = (buf[i + 2] << 8) + buf[i + 3];

                ಆದರೆ (srvs[j].ತೂಕ == 0) {
                    srvs[j].ತೂಕ = 1;
                }

                srvs[j].ಪೋರ್ಟ್ = (buf[i + 4] << 8) + buf[i + 5];

                ಆದರೆ (ngx_resolver_copy(r, &srvs[j].name, buf, &buf[i + 6],
                                      buf + n)
                    != NGX_OK)
                {
                    ಹೋಗು failed;
                }

                j++;
            }

            i += len;
        }

        ngx_ವಿಂಗಡಿಸು(srvs, nsrvs, ಗಾತ್ರ(ngx_resolver_srv_t),
                 ngx_resolver_cmp_srvs);

        ngx_resolver_free(r, rn->query);
        rn->query = ಖಾಲಿ;

        ngx_ಸರತಿ_remove(&rn->ಸರತಿ);

        rn->valid = ngx_ಸಮಯ() + (r->valid ? r->valid : (ಸಮಯ_t) rn->ttl);
        rn->expire = ngx_ಸಮಯ() + r->expire;

        ngx_ಸರತಿ_ಸೇರಿಸು_ತಲೆ(&r->srv_expire_ಸರತಿ, &rn->ಸರತಿ);

        ಮುಂದಿನ = rn->ಕಾಯುವಿಕೆ;
        rn->ಕಾಯುವಿಕೆ = ಖಾಲಿ;

        ಆಗ (ಮುಂದಿನ) {
            ctx = ಮುಂದಿನ;
            ಮುಂದಿನ = ctx->ಮುಂದಿನ;

            ngx_resolver_resolve_srv_ಹೆಸರುಗಳು(ctx, rn);
        }

        ಹಿಂತಿರುಗಿ;
    }

    rn->nsrvs = 0;

    ಆದರೆ (cname) {

        /* CNAME only */

        ಆದರೆ (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            ಹೋಗು failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->ಲಘುಗಣಕ, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_ಸರತಿ_remove(&rn->ಸರತಿ);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.ದತ್ತಾಂಶ;

        rn->valid = ngx_ಸಮಯ() + (r->valid ? r->valid : (ಸಮಯ_t) rn->ttl);
        rn->expire = ngx_ಸಮಯ() + r->expire;

        ngx_ಸರತಿ_ಸೇರಿಸು_ತಲೆ(&r->srv_expire_ಸರತಿ, &rn->ಸರತಿ);

        ngx_resolver_free(r, rn->query);
        rn->query = ಖಾಲಿ;
#ಆದರೆ (NGX_HAVE_INET6)
        rn->query6 = ಖಾಲಿ;
#ಆದರೆ_ಅಂತ್ಯ

        ctx = rn->ಕಾಯುವಿಕೆ;
        rn->ಕಾಯುವಿಕೆ = ಖಾಲಿ;

        ಆದರೆ (ctx) {

            ಆದರೆ (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                ಮಾಡು {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    ಮುಂದಿನ = ctx->ಮುಂದಿನ;

                    ctx->handler(ctx);

                    ctx = ಮುಂದಿನ;
                } ಆಗ (ctx);

                ಹಿಂತಿರುಗಿ;
            }

            ಗಾಗಿ (ಮುಂದಿನ = ctx; ಮುಂದಿನ; ಮುಂದಿನ = ಮುಂದಿನ->ಮುಂದಿನ) {
                ಮುಂದಿನ->ಗಂಟು = ಖಾಲಿ;
            }

            (ಶೂನ್ಯ) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        ಹಿಂತಿರುಗಿ;
    }

    ngx_log_error(r->log_level, r->ಲಘುಗಣಕ, 0, "no SRV type in DNS response");

    ಹಿಂತಿರುಗಿ;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->ಲಘುಗಣಕ, 0, err);

    ಹಿಂತಿರುಗಿ;

failed:

    /* unlock name mutex */

    ಹಿಂತಿರುಗಿ;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_resolver_resolve_srv_ಹೆಸರುಗಳು(ngx_resolver_ctx_t *ctx, ngx_resolver_ಗಂಟು_t *rn)
{
    ngx_uint_t                i;
    ngx_resolver_t           *r;
    ngx_resolver_ctx_t       *cctx;
    ngx_resolver_srv_name_t  *srvs;

    r = ctx->resolver;

    ctx->ಗಂಟು = ಖಾಲಿ;
    ctx->state = NGX_OK;
    ctx->valid = rn->valid;
    ctx->ಎಣಿಕೆ_2 = rn->nsrvs;

    srvs = ngx_resolver_calloc(r, rn->nsrvs * ಗಾತ್ರ(ngx_resolver_srv_name_t));
    ಆದರೆ (srvs == ಖಾಲಿ) {
        ಹೋಗು failed;
    }

    ctx->srvs = srvs;
    ctx->nsrvs = rn->nsrvs;

    ಆದರೆ (ctx->event && ctx->event->timer_ಗಣ) {
        ngx_del_timer(ctx->event);
    }

    ಗಾಗಿ (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
        srvs[i].name.ದತ್ತಾಂಶ = ngx_resolver_alloc(r, rn->u.srvs[i].name.len);
        ಆದರೆ (srvs[i].name.ದತ್ತಾಂಶ == ಖಾಲಿ) {
            ಹೋಗು failed;
        }

        srvs[i].name.len = rn->u.srvs[i].name.len;
        ngx_memcpy(srvs[i].name.ದತ್ತಾಂಶ, rn->u.srvs[i].name.ದತ್ತಾಂಶ,
                   srvs[i].name.len);

        cctx = ngx_resolve_ಆರಂಭ(r, ಖಾಲಿ);
        ಆದರೆ (cctx == ಖಾಲಿ) {
            ಹೋಗು failed;
        }

        cctx->name = srvs[i].name;
        cctx->handler = ngx_resolver_srv_ಹೆಸರುಗಳು_handler;
        cctx->ದತ್ತಾಂಶ = ctx;
        cctx->srvs = &srvs[i];
        cctx->timeout = ctx->timeout;

        srvs[i].ಆದ್ಯತೆ = rn->u.srvs[i].ಆದ್ಯತೆ;
        srvs[i].ತೂಕ = rn->u.srvs[i].ತೂಕ;
        srvs[i].ಪೋರ್ಟ್ = rn->u.srvs[i].ಪೋರ್ಟ್;
        srvs[i].ctx = cctx;

        ಆದರೆ (ngx_resolve_name(cctx) == NGX_ERROR) {
            srvs[i].ctx = ಖಾಲಿ;
            ಹೋಗು failed;
        }
    }

    ಹಿಂತಿರುಗಿ;

failed:

    ctx->state = NGX_ERROR;
    ctx->valid = ngx_ಸಮಯ() + (r->valid ? r->valid : 10);

    ctx->handler(ctx);
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_resolver_srv_ಹೆಸರುಗಳು_handler(ngx_resolver_ctx_t *cctx)
{
    ngx_uint_t                i;
    ngx_addr_t               *addrs;
    ngx_resolver_t           *r;
    ngx_sockaddr_t           *sockaddr;
    ngx_resolver_ctx_t       *ctx;
    ngx_resolver_srv_name_t  *srv;

    r = cctx->resolver;
    ctx = cctx->ದತ್ತಾಂಶ;
    srv = cctx->srvs;

    ctx->ಎಣಿಕೆ_2--;
    ctx->async |= cctx->async;

    srv->ctx = ಖಾಲಿ;
    srv->state = cctx->state;

    ಆದರೆ (cctx->naddrs) {

        ctx->valid = ngx_min(ctx->valid, cctx->valid);

        addrs = ngx_resolver_calloc(r, cctx->naddrs * ಗಾತ್ರ(ngx_addr_t));
        ಆದರೆ (addrs == ಖಾಲಿ) {
            srv->state = NGX_ERROR;
            ಹೋಗು ಮುಗಿದ;
        }

        sockaddr = ngx_resolver_alloc(r, cctx->naddrs * ಗಾತ್ರ(ngx_sockaddr_t));
        ಆದರೆ (sockaddr == ಖಾಲಿ) {
            ngx_resolver_free(r, addrs);
            srv->state = NGX_ERROR;
            ಹೋಗು ಮುಗಿದ;
        }

        ಗಾಗಿ (i = 0; i < cctx->naddrs; i++) {
            addrs[i].sockaddr = &sockaddr[i].sockaddr;
            addrs[i].socklen = cctx->addrs[i].socklen;

            ngx_memcpy(&sockaddr[i], cctx->addrs[i].sockaddr,
                       addrs[i].socklen);

            ngx_inet_ಗಣ_ಪೋರ್ಟ್(addrs[i].sockaddr, srv->ಪೋರ್ಟ್);
        }

        srv->addrs = addrs;
        srv->naddrs = cctx->naddrs;
    }

ಮುಗಿದ:

    ngx_resolve_name_ಮುಗಿದ(cctx);

    ಆದರೆ (ctx->ಎಣಿಕೆ_2 == 0) {
        ngx_resolver_report_srv(r, ctx);
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, ಗಾತ್ರ_t n,
    ngx_uint_t ident, ngx_uint_t ಸಂಕೇತ_2, ngx_uint_t nan)
{
    ಅಕ್ಷರ                 *err;
    ಗಾತ್ರ_t                len;
    ಒಳ_addr_t             addr;
    int32_t               ttl;
    ngx_int_t             octet;
    ngx_str_t             name;
    ngx_uint_t            ಮುಖವಾಡ, type, class, qident, a, i, ಆರಂಭ;
    ngx_ಸರತಿ_t          *expire_ಸರತಿ;
    ngx_rbtree_t         *ವೃಕ್ಷ;
    ngx_resolver_an_t    *an;
    ngx_resolver_ctx_t   *ctx, *ಮುಂದಿನ;
    ngx_resolver_ಗಂಟು_t  *rn;
#ಆದರೆ (NGX_HAVE_INET6)
    uint32_t              hash;
    ngx_int_t             digit;
    ರಚನೆ in6_addr       addr6;
#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ (ngx_resolver_copy(r, &name, buf,
                          buf + ಗಾತ್ರ(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        ಹಿಂತಿರುಗಿ;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->ಲಘುಗಣಕ, 0, "resolver qs:%V", &name);

    /* AF_INET */

    addr = 0;
    i = ಗಾತ್ರ(ngx_resolver_hdr_t);

    ಗಾಗಿ (ಮುಖವಾಡ = 0; ಮುಖವಾಡ < 32; ಮುಖವಾಡ += 8) {
        len = buf[i++];

        octet = ngx_atoi(&buf[i], len);
        ಆದರೆ (octet == NGX_ERROR || octet > 255) {
            ಹೋಗು invalid_ಒಳ_addr_arpa;
        }

        addr += octet << ಮುಖವಾಡ;
        i += len;
    }

    ಆದರೆ (ngx_strcasecmp(&buf[i], (u_char *) "\7in-addr\4arpa") == 0) {
        i += ಗಾತ್ರ("\7in-addr\4arpa");

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        ವೃಕ್ಷ = &r->addr_rbtree;
        expire_ಸರತಿ = &r->addr_expire_ಸರತಿ;

        ಹೋಗು valid;
    }

invalid_ಒಳ_addr_arpa:

#ಆದರೆ (NGX_HAVE_INET6)

    i = ಗಾತ್ರ(ngx_resolver_hdr_t);

    ಗಾಗಿ (octet = 15; octet >= 0; octet--) {
        ಆದರೆ (buf[i++] != '\1') {
            ಹೋಗು invalid_ip6_arpa;
        }

        digit = ngx_hextoi(&buf[i++], 1);
        ಆದರೆ (digit == NGX_ERROR) {
            ಹೋಗು invalid_ip6_arpa;
        }

        addr6.s6_addr[octet] = (u_char) digit;

        ಆದರೆ (buf[i++] != '\1') {
            ಹೋಗು invalid_ip6_arpa;
        }

        digit = ngx_hextoi(&buf[i++], 1);
        ಆದರೆ (digit == NGX_ERROR) {
            ಹೋಗು invalid_ip6_arpa;
        }

        addr6.s6_addr[octet] += (u_char) (digit * 16);
    }

    ಆದರೆ (ngx_strcasecmp(&buf[i], (u_char *) "\3ip6\4arpa") == 0) {
        i += ಗಾತ್ರ("\3ip6\4arpa");

        /* lock addr mutex */

        hash = ngx_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32_short(addr6.s6_addr, 16);
        rn = ngx_resolver_lookup_addr6(r, &addr6, hash);

        ವೃಕ್ಷ = &r->addr6_rbtree;
        expire_ಸರತಿ = &r->addr6_expire_ಸರತಿ;

        ಹೋಗು valid;
    }

invalid_ip6_arpa:
#ಆದರೆ_ಅಂತ್ಯ

    ngx_log_error(r->log_level, r->ಲಘುಗಣಕ, 0,
                  "invalid in-addr.arpa or ip6.arpa name in DNS response");
    ngx_resolver_free(r, name.ದತ್ತಾಂಶ);
    ಹಿಂತಿರುಗಿ;

valid:

    ಆದರೆ (rn == ಖಾಲಿ || rn->query == ಖಾಲಿ) {
        ngx_log_error(r->log_level, r->ಲಘುಗಣಕ, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.ದತ್ತಾಂಶ);
        ಹೋಗು failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    ಆದರೆ (ident != qident) {
        ngx_log_error(r->log_level, r->ಲಘುಗಣಕ, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.ದತ್ತಾಂಶ);
        ಹೋಗು failed;
    }

    ngx_resolver_free(r, name.ದತ್ತಾಂಶ);

    ಆದರೆ (ಸಂಕೇತ_2 == 0 && nan == 0) {
        ಸಂಕೇತ_2 = NGX_RESOLVE_NXDOMAIN;
    }

    ಆದರೆ (ಸಂಕೇತ_2) {
        ಮುಂದಿನ = rn->ಕಾಯುವಿಕೆ;
        rn->ಕಾಯುವಿಕೆ = ಖಾಲಿ;

        ngx_ಸರತಿ_remove(&rn->ಸರತಿ);

        ngx_rbtree_delete(ವೃಕ್ಷ, &rn->ಗಂಟು);

        /* unlock addr mutex */

        ಆಗ (ಮುಂದಿನ) {
            ctx = ಮುಂದಿನ;
            ctx->state = ಸಂಕೇತ_2;
            ctx->valid = ngx_ಸಮಯ() + (r->valid ? r->valid : 10);
            ಮುಂದಿನ = ctx->ಮುಂದಿನ;

            ctx->handler(ctx);
        }

        ngx_resolver_free_ಗಂಟು(r, rn);

        ಹಿಂತಿರುಗಿ;
    }

    i += ಗಾತ್ರ(ngx_resolver_qs_t);

    ಗಾಗಿ (a = 0; a < nan; a++) {

        ಆರಂಭ = i;

        ಆಗ (i < n) {

            ಆದರೆ (buf[i] & 0xc0) {
                i += 2;
                ಹೋಗು ಸಿಕ್ಕಿತು;
            }

            ಆದರೆ (buf[i] == 0) {
                i++;
                ಹೋಗು test_ಉದ್ದಳತೆ;
            }

            i += 1 + buf[i];
        }

        ಹೋಗು short_response;

    test_ಉದ್ದಳತೆ:

        ಆದರೆ (i - ಆರಂಭ < 2) {
            err = "invalid name in DNS response";
            ಹೋಗು invalid;
        }

    ಸಿಕ್ಕಿತು:

        ಆದರೆ (i + ಗಾತ್ರ(ngx_resolver_an_t) >= n) {
            ಹೋಗು short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        ಆದರೆ (class != 1) {
            ngx_log_error(r->log_level, r->ಲಘುಗಣಕ, 0,
                          "unexpected RR class %ui in DNS response", class);
            ಹೋಗು failed;
        }

        ಆದರೆ (ttl < 0) {
            ttl = 0;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->ಲಘುಗಣಕ, 0,
                      "resolver qt:%ui cl:%ui len:%uz",
                      type, class, len);

        i += ಗಾತ್ರ(ngx_resolver_an_t);

        ಆಯ್ಕೆ (type) {

        ಸ್ಥಿತಿ NGX_RESOLVE_PTR:

            ಹೋಗು ptr;

        ಸ್ಥಿತಿ NGX_RESOLVE_CNAME:

            ನಿಲ್ಲಿಸು;

        ಪೂರ್ವನಿಯೋಜಿತ:

            ngx_log_error(r->log_level, r->ಲಘುಗಣಕ, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->ಲಘುಗಣಕ, 0,
                  "no PTR type in DNS response");
    ಹಿಂತಿರುಗಿ;

ptr:

    ಆದರೆ (ngx_resolver_copy(r, &name, buf, buf + i, buf + n) != NGX_OK) {
        ಹೋಗು failed;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->ಲಘುಗಣಕ, 0, "resolver an:%V", &name);

    ಆದರೆ (name.len != (ಗಾತ್ರ_t) rn->nlen
        || ngx_strncmp(name.ದತ್ತಾಂಶ, rn->name, name.len) != 0)
    {
        ಆದರೆ (rn->nlen) {
            ngx_resolver_free(r, rn->name);
        }

        rn->nlen = (u_short) name.len;
        rn->name = name.ದತ್ತಾಂಶ;

        name.ದತ್ತಾಂಶ = ngx_resolver_dup(r, rn->name, name.len);
        ಆದರೆ (name.ದತ್ತಾಂಶ == ಖಾಲಿ) {
            ಹೋಗು failed;
        }
    }

    ngx_ಸರತಿ_remove(&rn->ಸರತಿ);

    rn->valid = ngx_ಸಮಯ() + (r->valid ? r->valid : ttl);
    rn->expire = ngx_ಸಮಯ() + r->expire;

    ngx_ಸರತಿ_ಸೇರಿಸು_ತಲೆ(expire_ಸರತಿ, &rn->ಸರತಿ);

    ಮುಂದಿನ = rn->ಕಾಯುವಿಕೆ;
    rn->ಕಾಯುವಿಕೆ = ಖಾಲಿ;

    /* unlock addr mutex */

    ಆಗ (ಮುಂದಿನ) {
        ctx = ಮುಂದಿನ;
        ctx->state = NGX_OK;
        ctx->valid = rn->valid;
        ctx->name = name;
        ಮುಂದಿನ = ctx->ಮುಂದಿನ;

        ctx->handler(ctx);
    }

    ngx_resolver_free(r, name.ದತ್ತಾಂಶ);

    ಹಿಂತಿರುಗಿ;

short_response:

    err = "short DNS response";

invalid:

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->ಲಘುಗಣಕ, 0, err);

    ಹಿಂತಿರುಗಿ;

failed:

    /* unlock addr mutex */

    ಹಿಂತಿರುಗಿ;
}


ಸ್ಥಿರ ngx_resolver_ಗಂಟು_t *
ngx_resolver_lookup_name(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_ಗಂಟು_t    *ಗಂಟು, *sentinel;
    ngx_resolver_ಗಂಟು_t  *rn;

    ಗಂಟು = r->name_rbtree.ಬೇರು;
    sentinel = r->name_rbtree.sentinel;

    ಆಗ (ಗಂಟು != sentinel) {

        ಆದರೆ (hash < ಗಂಟು->ಕೀಲಿ) {
            ಗಂಟು = ಗಂಟು->ಎಡ;
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (hash > ಗಂಟು->ಕೀಲಿ) {
            ಗಂಟು = ಗಂಟು->ಬಲ;
            ಮುಂದುವರಿ;
        }

        /* hash == node->key */

        rn = ngx_resolver_ಗಂಟು(ಗಂಟು);

        rc = ngx_memn2cmp(name->ದತ್ತಾಂಶ, rn->name, name->len, rn->nlen);

        ಆದರೆ (rc == 0) {
            ಹಿಂತಿರುಗಿ rn;
        }

        ಗಂಟು = (rc < 0) ? ಗಂಟು->ಎಡ : ಗಂಟು->ಬಲ;
    }

    /* not found */

    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}


ಸ್ಥಿರ ngx_resolver_ಗಂಟು_t *
ngx_resolver_lookup_srv(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_ಗಂಟು_t    *ಗಂಟು, *sentinel;
    ngx_resolver_ಗಂಟು_t  *rn;

    ಗಂಟು = r->srv_rbtree.ಬೇರು;
    sentinel = r->srv_rbtree.sentinel;

    ಆಗ (ಗಂಟು != sentinel) {

        ಆದರೆ (hash < ಗಂಟು->ಕೀಲಿ) {
            ಗಂಟು = ಗಂಟು->ಎಡ;
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (hash > ಗಂಟು->ಕೀಲಿ) {
            ಗಂಟು = ಗಂಟು->ಬಲ;
            ಮುಂದುವರಿ;
        }

        /* hash == node->key */

        rn = ngx_resolver_ಗಂಟು(ಗಂಟು);

        rc = ngx_memn2cmp(name->ದತ್ತಾಂಶ, rn->name, name->len, rn->nlen);

        ಆದರೆ (rc == 0) {
            ಹಿಂತಿರುಗಿ rn;
        }

        ಗಂಟು = (rc < 0) ? ಗಂಟು->ಎಡ : ಗಂಟು->ಬಲ;
    }

    /* not found */

    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}


ಸ್ಥಿರ ngx_resolver_ಗಂಟು_t *
ngx_resolver_lookup_addr(ngx_resolver_t *r, ಒಳ_addr_t addr)
{
    ngx_rbtree_ಗಂಟು_t  *ಗಂಟು, *sentinel;

    ಗಂಟು = r->addr_rbtree.ಬೇರು;
    sentinel = r->addr_rbtree.sentinel;

    ಆಗ (ಗಂಟು != sentinel) {

        ಆದರೆ (addr < ಗಂಟು->ಕೀಲಿ) {
            ಗಂಟು = ಗಂಟು->ಎಡ;
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (addr > ಗಂಟು->ಕೀಲಿ) {
            ಗಂಟು = ಗಂಟು->ಬಲ;
            ಮುಂದುವರಿ;
        }

        /* addr == node->key */

        ಹಿಂತಿರುಗಿ ngx_resolver_ಗಂಟು(ಗಂಟು);
    }

    /* not found */

    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}


#ಆದರೆ (NGX_HAVE_INET6)

ಸ್ಥಿರ ngx_resolver_ಗಂಟು_t *
ngx_resolver_lookup_addr6(ngx_resolver_t *r, ರಚನೆ in6_addr *addr,
    uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_ಗಂಟು_t    *ಗಂಟು, *sentinel;
    ngx_resolver_ಗಂಟು_t  *rn;

    ಗಂಟು = r->addr6_rbtree.ಬೇರು;
    sentinel = r->addr6_rbtree.sentinel;

    ಆಗ (ಗಂಟು != sentinel) {

        ಆದರೆ (hash < ಗಂಟು->ಕೀಲಿ) {
            ಗಂಟು = ಗಂಟು->ಎಡ;
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (hash > ಗಂಟು->ಕೀಲಿ) {
            ಗಂಟು = ಗಂಟು->ಬಲ;
            ಮುಂದುವರಿ;
        }

        /* hash == node->key */

        rn = ngx_resolver_ಗಂಟು(ಗಂಟು);

        rc = ngx_memcmp(addr, &rn->addr6, 16);

        ಆದರೆ (rc == 0) {
            ಹಿಂತಿರುಗಿ rn;
        }

        ಗಂಟು = (rc < 0) ? ಗಂಟು->ಎಡ : ಗಂಟು->ಬಲ;
    }

    /* not found */

    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

#ಆದರೆ_ಅಂತ್ಯ


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_resolver_rbtree_ಸೇರಿಸು_ಮೌಲ್ಯ(ngx_rbtree_ಗಂಟು_t *ತಾತ್ಕಾಲಿಕ,
    ngx_rbtree_ಗಂಟು_t *ಗಂಟು, ngx_rbtree_ಗಂಟು_t *sentinel)
{
    ngx_rbtree_ಗಂಟು_t    **p;
    ngx_resolver_ಗಂಟು_t   *rn, *rn_ತಾತ್ಕಾಲಿಕ;

    ಗಾಗಿ ( ;; ) {

        ಆದರೆ (ಗಂಟು->ಕೀಲಿ < ತಾತ್ಕಾಲಿಕ->ಕೀಲಿ) {

            p = &ತಾತ್ಕಾಲಿಕ->ಎಡ;

        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಗಂಟು->ಕೀಲಿ > ತಾತ್ಕಾಲಿಕ->ಕೀಲಿ) {

            p = &ತಾತ್ಕಾಲಿಕ->ಬಲ;

        } ಇಲ್ಲದಿದ್ದರೆ { /* node->key == temp->key */

            rn = ngx_resolver_ಗಂಟು(ಗಂಟು);
            rn_ತಾತ್ಕಾಲಿಕ = ngx_resolver_ಗಂಟು(ತಾತ್ಕಾಲಿಕ);

            p = (ngx_memn2cmp(rn->name, rn_ತಾತ್ಕಾಲಿಕ->name, rn->nlen, rn_ತಾತ್ಕಾಲಿಕ->nlen)
                 < 0) ? &ತಾತ್ಕಾಲಿಕ->ಎಡ : &ತಾತ್ಕಾಲಿಕ->ಬಲ;
        }

        ಆದರೆ (*p == sentinel) {
            ನಿಲ್ಲಿಸು;
        }

        ತಾತ್ಕಾಲಿಕ = *p;
    }

    *p = ಗಂಟು;
    ಗಂಟು->ಪೋಷಕ = ತಾತ್ಕಾಲಿಕ;
    ಗಂಟು->ಎಡ = sentinel;
    ಗಂಟು->ಬಲ = sentinel;
    ngx_rbt_red(ಗಂಟು);
}


#ಆದರೆ (NGX_HAVE_INET6)

ಸ್ಥಿರ ಶೂನ್ಯ
ngx_resolver_rbtree_ಸೇರಿಸು_addr6_ಮೌಲ್ಯ(ngx_rbtree_ಗಂಟು_t *ತಾತ್ಕಾಲಿಕ,
    ngx_rbtree_ಗಂಟು_t *ಗಂಟು, ngx_rbtree_ಗಂಟು_t *sentinel)
{
    ngx_rbtree_ಗಂಟು_t    **p;
    ngx_resolver_ಗಂಟು_t   *rn, *rn_ತಾತ್ಕಾಲಿಕ;

    ಗಾಗಿ ( ;; ) {

        ಆದರೆ (ಗಂಟು->ಕೀಲಿ < ತಾತ್ಕಾಲಿಕ->ಕೀಲಿ) {

            p = &ತಾತ್ಕಾಲಿಕ->ಎಡ;

        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಗಂಟು->ಕೀಲಿ > ತಾತ್ಕಾಲಿಕ->ಕೀಲಿ) {

            p = &ತಾತ್ಕಾಲಿಕ->ಬಲ;

        } ಇಲ್ಲದಿದ್ದರೆ { /* node->key == temp->key */

            rn = ngx_resolver_ಗಂಟು(ಗಂಟು);
            rn_ತಾತ್ಕಾಲಿಕ = ngx_resolver_ಗಂಟು(ತಾತ್ಕಾಲಿಕ);

            p = (ngx_memcmp(&rn->addr6, &rn_ತಾತ್ಕಾಲಿಕ->addr6, 16)
                 < 0) ? &ತಾತ್ಕಾಲಿಕ->ಎಡ : &ತಾತ್ಕಾಲಿಕ->ಬಲ;
        }

        ಆದರೆ (*p == sentinel) {
            ನಿಲ್ಲಿಸು;
        }

        ತಾತ್ಕಾಲಿಕ = *p;
    }

    *p = ಗಂಟು;
    ಗಂಟು->ಪೋಷಕ = ತಾತ್ಕಾಲಿಕ;
    ಗಂಟು->ಎಡ = sentinel;
    ಗಂಟು->ಬಲ = sentinel;
    ngx_rbt_red(ಗಂಟು);
}

#ಆದರೆ_ಅಂತ್ಯ


ಸ್ಥಿರ ngx_int_t
ngx_resolver_create_name_query(ngx_resolver_t *r, ngx_resolver_ಗಂಟು_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    ಗಾತ್ರ_t               len, nlen;
    ngx_uint_t           ident;
    ngx_resolver_qs_t   *qs;
    ngx_resolver_hdr_t  *query;

    nlen = name->len ? (1 + name->len + 1) : 1;

    len = ಗಾತ್ರ(ngx_resolver_hdr_t) + nlen + ಗಾತ್ರ(ngx_resolver_qs_t);

#ಆದರೆ (NGX_HAVE_INET6)
    p = ngx_resolver_alloc(r, len * (r->ipv4 + r->ipv6));
#ಇಲ್ಲದಿದ್ದರೆ
    p = ngx_resolver_alloc(r, len);
#ಆದರೆ_ಅಂತ್ಯ
    ಆದರೆ (p == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    rn->qlen = (u_short) len;
    rn->query = p;

#ಆದರೆ (NGX_HAVE_INET6)
    ಆದರೆ (r->ipv6) {
        rn->query6 = r->ipv4 ? (p + len) : p;
    }
#ಆದರೆ_ಅಂತ್ಯ

    query = (ngx_resolver_hdr_t *) p;

    ಆದರೆ (r->ipv4) {
        ident = ngx_random();

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->ಲಘುಗಣಕ, 0,
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

    p += ಗಾತ್ರ(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    /* query type */
    qs->type_hi = 0; qs->type_lo = NGX_RESOLVE_A;

    /* IN query class */
    qs->class_hi = 0; qs->class_lo = 1;

    /* convert "www.example.com" to "\3www\7example\3com\0" */

    len = 0;
    p--;
    *p-- = '\0';

    ಆದರೆ (name->len == 0)  {
        ಹಿಂತಿರುಗಿ NGX_DECLINED;
    }

    ಗಾಗಿ (s = name->ದತ್ತಾಂಶ + name->len - 1; s >= name->ದತ್ತಾಂಶ; s--) {
        ಆದರೆ (*s != '.') {
            *p = *s;
            len++;

        } ಇಲ್ಲದಿದ್ದರೆ {
            ಆದರೆ (len == 0 || len > 255) {
                ಹಿಂತಿರುಗಿ NGX_DECLINED;
            }

            *p = (u_char) len;
            len = 0;
        }

        p--;
    }

    ಆದರೆ (len == 0 || len > 255) {
        ಹಿಂತಿರುಗಿ NGX_DECLINED;
    }

    *p = (u_char) len;

#ಆದರೆ (NGX_HAVE_INET6)
    ಆದರೆ (!r->ipv6) {
        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    p = rn->query6;

    ಆದರೆ (r->ipv4) {
        ngx_memcpy(p, rn->query, rn->qlen);
    }

    query = (ngx_resolver_hdr_t *) p;

    ident = ngx_random();

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->ಲಘುಗಣಕ, 0,
                   "resolve: \"%V\" AAAA %i", name, ident & 0xffff);

    query->ident_hi = (u_char) ((ident >> 8) & 0xff);
    query->ident_lo = (u_char) (ident & 0xff);

    p += ಗಾತ್ರ(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    qs->type_lo = NGX_RESOLVE_AAAA;
#ಆದರೆ_ಅಂತ್ಯ

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ಸ್ಥಿರ ngx_int_t
ngx_resolver_create_srv_query(ngx_resolver_t *r, ngx_resolver_ಗಂಟು_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    ಗಾತ್ರ_t               len, nlen;
    ngx_uint_t           ident;
    ngx_resolver_qs_t   *qs;
    ngx_resolver_hdr_t  *query;

    nlen = name->len ? (1 + name->len + 1) : 1;

    len = ಗಾತ್ರ(ngx_resolver_hdr_t) + nlen + ಗಾತ್ರ(ngx_resolver_qs_t);

    p = ngx_resolver_alloc(r, len);
    ಆದರೆ (p == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    rn->qlen = (u_short) len;
    rn->query = p;

    query = (ngx_resolver_hdr_t *) p;

    ident = ngx_random();

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->ಲಘುಗಣಕ, 0,
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

    p += ಗಾತ್ರ(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    /* query type */
    qs->type_hi = 0; qs->type_lo = NGX_RESOLVE_SRV;

    /* IN query class */
    qs->class_hi = 0; qs->class_lo = 1;

    /* converts "www.example.com" to "\3www\7example\3com\0" */

    len = 0;
    p--;
    *p-- = '\0';

    ಆದರೆ (name->len == 0)  {
        ಹಿಂತಿರುಗಿ NGX_DECLINED;
    }

    ಗಾಗಿ (s = name->ದತ್ತಾಂಶ + name->len - 1; s >= name->ದತ್ತಾಂಶ; s--) {
        ಆದರೆ (*s != '.') {
            *p = *s;
            len++;

        } ಇಲ್ಲದಿದ್ದರೆ {
            ಆದರೆ (len == 0 || len > 255) {
                ಹಿಂತಿರುಗಿ NGX_DECLINED;
            }

            *p = (u_char) len;
            len = 0;
        }

        p--;
    }

    ಆದರೆ (len == 0 || len > 255) {
        ಹಿಂತಿರುಗಿ NGX_DECLINED;
    }

    *p = (u_char) len;

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ಸ್ಥಿರ ngx_int_t
ngx_resolver_create_addr_query(ngx_resolver_t *r, ngx_resolver_ಗಂಟು_t *rn,
    ngx_resolver_addr_t *addr)
{
    u_char               *p, *d;
    ಗಾತ್ರ_t                len;
    ಒಳ_addr_t             inaddr;
    ngx_int_t             n;
    ngx_uint_t            ident;
    ngx_resolver_hdr_t   *query;
    ರಚನೆ sockaddr_ಒಳ   *ಜ್ಯಾ;
#ಆದರೆ (NGX_HAVE_INET6)
    ರಚನೆ sockaddr_in6  *sin6;
#ಆದರೆ_ಅಂತ್ಯ

    ಆಯ್ಕೆ (addr->sockaddr->sa_family) {

#ಆದರೆ (NGX_HAVE_INET6)
    ಸ್ಥಿತಿ AF_INET6:
        len = ಗಾತ್ರ(ngx_resolver_hdr_t)
              + 64 + ಗಾತ್ರ(".ip6.arpa.") - 1
              + ಗಾತ್ರ(ngx_resolver_qs_t);

        ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ

    ಪೂರ್ವನಿಯೋಜಿತ: /* AF_INET */
        len = ಗಾತ್ರ(ngx_resolver_hdr_t)
              + ಗಾತ್ರ(".255.255.255.255.in-addr.arpa.") - 1
              + ಗಾತ್ರ(ngx_resolver_qs_t);
    }

    p = ngx_resolver_alloc(r, len);
    ಆದರೆ (p == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
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

    p += ಗಾತ್ರ(ngx_resolver_hdr_t);

    ಆಯ್ಕೆ (addr->sockaddr->sa_family) {

#ಆದರೆ (NGX_HAVE_INET6)
    ಸ್ಥಿತಿ AF_INET6:
        sin6 = (ರಚನೆ sockaddr_in6 *) addr->sockaddr;

        ಗಾಗಿ (n = 15; n >= 0; n--) {
            p = ngx_sprintf(p, "\1%xd\1%xd",
                            sin6->sin6_addr.s6_addr[n] & 0xf,
                            (sin6->sin6_addr.s6_addr[n] >> 4) & 0xf);
        }

        p = ngx_cpymem(p, "\3ip6\4arpa\0", 10);

        ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ

    ಪೂರ್ವನಿಯೋಜಿತ: /* AF_INET */

        ಜ್ಯಾ = (ರಚನೆ sockaddr_ಒಳ *) addr->sockaddr;
        inaddr = ntohl(ಜ್ಯಾ->sin_addr.s_addr);

        ಗಾಗಿ (n = 0; n < 32; n += 8) {
            d = ngx_sprintf(&p[1], "%ud", (inaddr >> n) & 0xff);
            *p = (u_char) (d - &p[1]);
            p = d;
        }

        p = ngx_cpymem(p, "\7in-addr\4arpa\0", 14);
    }

    /* query type "PTR", IN query class */
    p = ngx_cpymem(p, "\0\14\0\1", 4);

    rn->qlen = (u_short) (p - rn->query);

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ಸ್ಥಿರ ngx_int_t
ngx_resolver_copy(ngx_resolver_t *r, ngx_str_t *name, u_char *buf, u_char *src,
    u_char *ಕೊನೆಯ)
{
    ಅಕ್ಷರ        *err;
    u_char      *p, *dst;
    ಗಾತ್ರ_t       len;
    ngx_uint_t   i, n;

    p = src;
    len = 0;

    /*
     * compression pointers allow to create endless loop, so we set limit;
     * 128 pointers should be enough to store 255-byte name
     */

    ಗಾಗಿ (i = 0; i < 128; i++) {
        n = *p++;

        ಆದರೆ (n == 0) {
            ಹೋಗು ಮುಗಿದ;
        }

        ಆದರೆ (n & 0xc0) {
            ಆದರೆ ((n & 0xc0) != 0xc0) {
                err = "invalid label type in DNS response";
                ಹೋಗು invalid;
            }

            ಆದರೆ (p >= ಕೊನೆಯ) {
                err = "name is out of DNS response";
                ಹೋಗು invalid;
            }

            n = ((n & 0x3f) << 8) + *p;
            p = &buf[n];

        } ಇಲ್ಲದಿದ್ದರೆ {
            len += 1 + n;
            p = &p[n];
        }

        ಆದರೆ (p >= ಕೊನೆಯ) {
            err = "name is out of DNS response";
            ಹೋಗು invalid;
        }
    }

    err = "compression pointers loop in DNS response";

invalid:

    ngx_log_error(r->log_level, r->ಲಘುಗಣಕ, 0, err);

    ಹಿಂತಿರುಗಿ NGX_ERROR;

ಮುಗಿದ:

    ಆದರೆ (name == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    ಆದರೆ (len == 0) {
        ngx_str_null(name);
        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    dst = ngx_resolver_alloc(r, len);
    ಆದರೆ (dst == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    name->ದತ್ತಾಂಶ = dst;

    ಗಾಗಿ ( ;; ) {
        n = *src++;

        ಆದರೆ (n == 0) {
            name->len = dst - name->ದತ್ತಾಂಶ - 1;
            ಹಿಂತಿರುಗಿ NGX_OK;
        }

        ಆದರೆ (n & 0xc0) {
            n = ((n & 0x3f) << 8) + *src;
            src = &buf[n];

        } ಇಲ್ಲದಿದ್ದರೆ {
            ngx_strlow(dst, src, n);
            dst += n;
            src += n;
            *dst++ = '.';
        }
    }
}


ಸ್ಥಿರ ngx_int_t
ngx_resolver_ಗಣ_timeout(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx)
{
    ಆದರೆ (ctx->event || ctx->timeout == 0) {
        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    ctx->event = ngx_resolver_calloc(r, ಗಾತ್ರ(ngx_event_t));
    ಆದರೆ (ctx->event == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ctx->event->handler = ngx_resolver_timeout_handler;
    ctx->event->ದತ್ತಾಂಶ = ctx;
    ctx->event->ಲಘುಗಣಕ = r->ಲಘುಗಣಕ;
    ctx->event->cancelable = ctx->cancelable;
    ctx->ident = -1;

    ngx_ಕೂಡಿಸು_timer(ctx->event, ctx->timeout);

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_resolver_timeout_handler(ngx_event_t *ev)
{
    ngx_resolver_ctx_t  *ctx;

    ctx = ev->ದತ್ತಾಂಶ;

    ctx->state = NGX_RESOLVE_TIMEDOUT;

    ctx->handler(ctx);
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_resolver_free_ಗಂಟು(ngx_resolver_t *r, ngx_resolver_ಗಂಟು_t *rn)
{
    ngx_uint_t  i;

    /* lock alloc mutex */

    ಆದರೆ (rn->query) {
        ngx_resolver_free_locked(r, rn->query);
    }

    ಆದರೆ (rn->name) {
        ngx_resolver_free_locked(r, rn->name);
    }

    ಆದರೆ (rn->cnlen) {
        ngx_resolver_free_locked(r, rn->u.cname);
    }

    ಆದರೆ (rn->naddrs > 1 && rn->naddrs != (u_short) -1) {
        ngx_resolver_free_locked(r, rn->u.addrs);
    }

#ಆದರೆ (NGX_HAVE_INET6)
    ಆದರೆ (rn->naddrs6 > 1 && rn->naddrs6 != (u_short) -1) {
        ngx_resolver_free_locked(r, rn->u6.addrs6);
    }
#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ (rn->nsrvs) {
        ಗಾಗಿ (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
            ಆದರೆ (rn->u.srvs[i].name.ದತ್ತಾಂಶ) {
                ngx_resolver_free_locked(r, rn->u.srvs[i].name.ದತ್ತಾಂಶ);
            }
        }

        ngx_resolver_free_locked(r, rn->u.srvs);
    }

    ngx_resolver_free_locked(r, rn);

    /* unlock alloc mutex */
}


ಸ್ಥಿರ ಶೂನ್ಯ *
ngx_resolver_alloc(ngx_resolver_t *r, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    u_char  *p;

    /* lock alloc mutex */

    p = ngx_alloc(ಗಾತ್ರ_2, r->ಲಘುಗಣಕ);

    /* unlock alloc mutex */

    ಹಿಂತಿರುಗಿ p;
}


ಸ್ಥಿರ ಶೂನ್ಯ *
ngx_resolver_calloc(ngx_resolver_t *r, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    u_char  *p;

    p = ngx_resolver_alloc(r, ಗಾತ್ರ_2);

    ಆದರೆ (p) {
        ngx_memzero(p, ಗಾತ್ರ_2);
    }

    ಹಿಂತಿರುಗಿ p;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_resolver_free(ngx_resolver_t *r, ಶೂನ್ಯ *p)
{
    /* lock alloc mutex */

    ngx_free(p);

    /* unlock alloc mutex */
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_resolver_free_locked(ngx_resolver_t *r, ಶೂನ್ಯ *p)
{
    ngx_free(p);
}


ಸ್ಥಿರ ಶೂನ್ಯ *
ngx_resolver_dup(ngx_resolver_t *r, ಶೂನ್ಯ *src, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    ಶೂನ್ಯ  *dst;

    dst = ngx_resolver_alloc(r, ಗಾತ್ರ_2);

    ಆದರೆ (dst == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ dst;
    }

    ngx_memcpy(dst, src, ಗಾತ್ರ_2);

    ಹಿಂತಿರುಗಿ dst;
}


ಸ್ಥಿರ ngx_resolver_addr_t *
ngx_resolver_export(ngx_resolver_t *r, ngx_resolver_ಗಂಟು_t *rn,
    ngx_uint_t rotate)
{
    ngx_uint_t            d, i, j, n;
    ಒಳ_addr_t            *addr;
    ngx_sockaddr_t       *sockaddr;
    ರಚನೆ sockaddr_ಒಳ   *ಜ್ಯಾ;
    ngx_resolver_addr_t  *dst;
#ಆದರೆ (NGX_HAVE_INET6)
    ರಚನೆ in6_addr      *addr6;
    ರಚನೆ sockaddr_in6  *sin6;
#ಆದರೆ_ಅಂತ್ಯ

    n = rn->naddrs;
#ಆದರೆ (NGX_HAVE_INET6)
    n += rn->naddrs6;
#ಆದರೆ_ಅಂತ್ಯ

    dst = ngx_resolver_calloc(r, n * ಗಾತ್ರ(ngx_resolver_addr_t));
    ಆದರೆ (dst == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    sockaddr = ngx_resolver_calloc(r, n * ಗಾತ್ರ(ngx_sockaddr_t));
    ಆದರೆ (sockaddr == ಖಾಲಿ) {
        ngx_resolver_free(r, dst);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    i = 0;
    d = rotate ? ngx_random() % n : 0;

    ಆದರೆ (rn->naddrs) {
        j = rotate ? ngx_random() % rn->naddrs : 0;

        addr = (rn->naddrs == 1) ? &rn->u.addr : rn->u.addrs;

        ಮಾಡು {
            ಜ್ಯಾ = &sockaddr[d].sockaddr_ಒಳ;
            ಜ್ಯಾ->sin_family = AF_INET;
            ಜ್ಯಾ->sin_addr.s_addr = addr[j++];
            dst[d].sockaddr = (ರಚನೆ sockaddr *) ಜ್ಯಾ;
            dst[d++].socklen = ಗಾತ್ರ(ರಚನೆ sockaddr_ಒಳ);

            ಆದರೆ (d == n) {
                d = 0;
            }

            ಆದರೆ (j == (ngx_uint_t) rn->naddrs) {
                j = 0;
            }
        } ಆಗ (++i < (ngx_uint_t) rn->naddrs);
    }

#ಆದರೆ (NGX_HAVE_INET6)
    ಆದರೆ (rn->naddrs6) {
        j = rotate ? ngx_random() % rn->naddrs6 : 0;

        addr6 = (rn->naddrs6 == 1) ? &rn->u6.addr6 : rn->u6.addrs6;

        ಮಾಡು {
            sin6 = &sockaddr[d].sockaddr_in6;
            sin6->sin6_family = AF_INET6;
            ngx_memcpy(sin6->sin6_addr.s6_addr, addr6[j++].s6_addr, 16);
            dst[d].sockaddr = (ರಚನೆ sockaddr *) sin6;
            dst[d++].socklen = ಗಾತ್ರ(ರಚನೆ sockaddr_in6);

            ಆದರೆ (d == n) {
                d = 0;
            }

            ಆದರೆ (j == rn->naddrs6) {
                j = 0;
            }
        } ಆಗ (++i < n);
    }
#ಆದರೆ_ಅಂತ್ಯ

    ಹಿಂತಿರುಗಿ dst;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_resolver_report_srv(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx)
{
    ngx_uint_t                naddrs, nsrvs, nw, i, j, k, l, m, n, w;
    ngx_resolver_addr_t      *addrs;
    ngx_resolver_srv_name_t  *srvs;

    srvs = ctx->srvs;
    nsrvs = ctx->nsrvs;

    naddrs = 0;

    ಗಾಗಿ (i = 0; i < nsrvs; i++) {
        ಆದರೆ (srvs[i].state == NGX_ERROR) {
            ctx->state = NGX_ERROR;
            ctx->valid = ngx_ಸಮಯ() + (r->valid ? r->valid : 10);

            ctx->handler(ctx);
            ಹಿಂತಿರುಗಿ;
        }

        naddrs += srvs[i].naddrs;
    }

    ಆದರೆ (naddrs == 0) {
        ctx->state = srvs[0].state;

        ಗಾಗಿ (i = 0; i < nsrvs; i++) {
            ಆದರೆ (srvs[i].state == NGX_RESOLVE_NXDOMAIN) {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                ನಿಲ್ಲಿಸು;
            }
        }

        ctx->valid = ngx_ಸಮಯ() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        ಹಿಂತಿರುಗಿ;
    }

    addrs = ngx_resolver_calloc(r, naddrs * ಗಾತ್ರ(ngx_resolver_addr_t));
    ಆದರೆ (addrs == ಖಾಲಿ) {
        ctx->state = NGX_ERROR;
        ctx->valid = ngx_ಸಮಯ() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        ಹಿಂತಿರುಗಿ;
    }

    i = 0;
    n = 0;

    ಮಾಡು {
        nw = 0;

        ಗಾಗಿ (j = i; j < nsrvs; j++) {
            ಆದರೆ (srvs[j].ಆದ್ಯತೆ != srvs[i].ಆದ್ಯತೆ) {
                ನಿಲ್ಲಿಸು;
            }

            nw += srvs[j].naddrs * srvs[j].ತೂಕ;
        }

        ಆದರೆ (nw == 0) {
            ಹೋಗು ಮುಂದಿನ_srv;
        }

        w = ngx_random() % nw;

        ಗಾಗಿ (k = i; k < j; k++) {
            ಆದರೆ (w < srvs[k].naddrs * srvs[k].ತೂಕ) {
                ನಿಲ್ಲಿಸು;
            }

            w -= srvs[k].naddrs * srvs[k].ತೂಕ;
        }

        ಗಾಗಿ (l = i; l < j; l++) {

            ಗಾಗಿ (m = 0; m < srvs[k].naddrs; m++) {
                addrs[n].socklen = srvs[k].addrs[m].socklen;
                addrs[n].sockaddr = srvs[k].addrs[m].sockaddr;
                addrs[n].name = srvs[k].name;
                addrs[n].ಆದ್ಯತೆ = srvs[k].ಆದ್ಯತೆ;
                addrs[n].ತೂಕ = srvs[k].ತೂಕ;
                n++;
            }

            ಆದರೆ (++k == j) {
                k = i;
            }
        }

ಮುಂದಿನ_srv:

        i = j;

    } ಆಗ (i < ctx->nsrvs);

    ctx->state = NGX_OK;
    ctx->addrs = addrs;
    ctx->naddrs = naddrs;

    ctx->handler(ctx);

    ngx_resolver_free(r, addrs);
}


ಅಕ್ಷರ *
ngx_resolver_strerror(ngx_int_t err)
{
    ಸ್ಥಿರ ಅಕ್ಷರ *errors[] = {
        "Format error",     /* FORMERR */
        "Server failure",   /* SERVFAIL */
        "Host not found",   /* NXDOMAIN */
        "Unimplemented",    /* NOTIMP */
        "Operation refused" /* REFUSED */
    };

    ಆದರೆ (err > 0 && err < 6) {
        ಹಿಂತಿರುಗಿ errors[err - 1];
    }

    ಆದರೆ (err == NGX_RESOLVE_TIMEDOUT) {
        ಹಿಂತಿರುಗಿ "Operation timed out";
    }

    ಹಿಂತಿರುಗಿ "Unknown error";
}


ಸ್ಥಿರ u_char *
ngx_resolver_log_error(ngx_log_t *ಲಘುಗಣಕ, u_char *buf, ಗಾತ್ರ_t len)
{
    u_char                     *p;
    ngx_resolver_connection_t  *rec;

    p = buf;

    ಆದರೆ (ಲಘುಗಣಕ->action) {
        p = ngx_snprintf(buf, len, " while %s", ಲಘುಗಣಕ->action);
        len -= p - buf;
    }

    rec = ಲಘುಗಣಕ->ದತ್ತಾಂಶ;

    ಆದರೆ (rec) {
        p = ngx_snprintf(p, len, ", resolver: %V", &rec->server);
    }

    ಹಿಂತಿರುಗಿ p;
}


ಸ್ಥಿರ ngx_int_t
ngx_udp_connect(ngx_resolver_connection_t *rec)
{
    ಪೂರ್ಣಾಂಕ                rc;
    ngx_int_t          event;
    ngx_event_t       *rev, *wev;
    ngx_socket_t       s;
    ngx_connection_t  *c;

    s = ngx_socket(rec->sockaddr->sa_family, SOCK_DGRAM, 0);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->ಲಘುಗಣಕ, 0, "UDP socket %d", s);

    ಆದರೆ (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->ಲಘುಗಣಕ, ngx_socket_errno,
                      ngx_socket_n " failed");
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->ಲಘುಗಣಕ);

    ಆದರೆ (c == ಖಾಲಿ) {
        ಆದರೆ (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->ಲಘುಗಣಕ, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಆದರೆ (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->ಲಘುಗಣಕ, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        ಹೋಗು failed;
    }

    rev = c->ಪಠ;
    wev = c->ಲಿಖ;

    rev->ಲಘುಗಣಕ = &rec->ಲಘುಗಣಕ;
    wev->ಲಘುಗಣಕ = &rec->ಲಘುಗಣಕ;

    rec->udp = c;

    c->ಸಂಖ್ಯೆ = ngx_atomic_fetch_ಕೂಡಿಸು(ngx_connection_counter, 1);

    c->ಆರಂಭ_ಸಮಯ = ngx_current_msec;

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->ಲಘುಗಣಕ, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->ಸಂಖ್ಯೆ);

    rc = connect(s, rec->sockaddr, rec->socklen);

    ಆದರೆ (rc == -1) {
        ngx_log_error(NGX_LOG_CRIT, &rec->ಲಘುಗಣಕ, ngx_socket_errno,
                      "connect() failed");

        ಹೋಗು failed;
    }

    /* UDP sockets are always ready to write */
    wev->ready = 1;

    event = (ngx_event_flags & NGX_USE_CLEAR_EVENT) ?
                /* kqueue, epoll */                 NGX_CLEAR_EVENT:
                /* select, poll, /dev/poll */       NGX_LEVEL_EVENT;
                /* eventport event type has no meaning: oneshot only */

    ಆದರೆ (ngx_ಕೂಡಿಸು_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        ಹೋಗು failed;
    }

    ಹಿಂತಿರುಗಿ NGX_OK;

failed:

    ngx_close_connection(c);
    rec->udp = ಖಾಲಿ;

    ಹಿಂತಿರುಗಿ NGX_ERROR;
}


ಸ್ಥಿರ ngx_int_t
ngx_tcp_connect(ngx_resolver_connection_t *rec)
{
    ಪೂರ್ಣಾಂಕ                rc;
    ngx_int_t          event;
    ngx_err_t          err;
    ngx_uint_t         level;
    ngx_socket_t       s;
    ngx_event_t       *rev, *wev;
    ngx_connection_t  *c;

    s = ngx_socket(rec->sockaddr->sa_family, SOCK_ಹರಿವು, 0);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->ಲಘುಗಣಕ, 0, "TCP socket %d", s);

    ಆದರೆ (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->ಲಘುಗಣಕ, ngx_socket_errno,
                      ngx_socket_n " failed");
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->ಲಘುಗಣಕ);

    ಆದರೆ (c == ಖಾಲಿ) {
        ಆದರೆ (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->ಲಘುಗಣಕ, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಆದರೆ (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->ಲಘುಗಣಕ, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        ಹೋಗು failed;
    }

    rev = c->ಪಠ;
    wev = c->ಲಿಖ;

    rev->ಲಘುಗಣಕ = &rec->ಲಘುಗಣಕ;
    wev->ಲಘುಗಣಕ = &rec->ಲಘುಗಣಕ;

    rec->tcp = c;

    c->ಸಂಖ್ಯೆ = ngx_atomic_fetch_ಕೂಡಿಸು(ngx_connection_counter, 1);

    c->ಆರಂಭ_ಸಮಯ = ngx_current_msec;

    ಆದರೆ (ngx_ಕೂಡಿಸು_conn) {
        ಆದರೆ (ngx_ಕೂಡಿಸು_conn(c) == NGX_ERROR) {
            ಹೋಗು failed;
        }
    }

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->ಲಘುಗಣಕ, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->ಸಂಖ್ಯೆ);

    rc = connect(s, rec->sockaddr, rec->socklen);

    ಆದರೆ (rc == -1) {
        err = ngx_socket_errno;


        ಆದರೆ (err != NGX_EINPROGRESS
#ಆದರೆ (NGX_WIN32)
            /* Winsock returns WSAEWOULDBLOCK (NGX_EAGAIN) */
            && err != NGX_EAGAIN
#ಆದರೆ_ಅಂತ್ಯ
            )
        {
            ಆದರೆ (err == NGX_ECONNREFUSED
#ಆದರೆ (NGX_LINUX)
                /*
                 * Linux returns EAGAIN instead of ECONNREFUSED
                 * for unix sockets if listen queue is full
                 */
                || err == NGX_EAGAIN
#ಆದರೆ_ಅಂತ್ಯ
                || err == NGX_ECONNRESET
                || err == NGX_ENETDOWN
                || err == NGX_ENETUNREACH
                || err == NGX_EHOSTDOWN
                || err == NGX_EHOSTUNREACH)
            {
                level = NGX_LOG_ERR;

            } ಇಲ್ಲದಿದ್ದರೆ {
                level = NGX_LOG_CRIT;
            }

            ngx_log_error(level, &rec->ಲಘುಗಣಕ, err, "connect() to %V failed",
                          &rec->server);

            ngx_close_connection(c);
            rec->tcp = ಖಾಲಿ;

            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }
    }

    ಆದರೆ (ngx_ಕೂಡಿಸು_conn) {
        ಆದರೆ (rc == -1) {

            /* NGX_EINPROGRESS */

            ಹಿಂತಿರುಗಿ NGX_AGAIN;
        }

        ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->ಲಘುಗಣಕ, 0, "connected");

        wev->ready = 1;

        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    ಆದರೆ (ngx_event_flags & NGX_USE_CLEAR_EVENT) {

        /* kqueue */

        event = NGX_CLEAR_EVENT;

    } ಇಲ್ಲದಿದ್ದರೆ {

        /* select, poll, /dev/poll */

        event = NGX_LEVEL_EVENT;
    }

    ಆದರೆ (ngx_ಕೂಡಿಸು_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        ಹೋಗು failed;
    }

    ಆದರೆ (rc == -1) {

        /* NGX_EINPROGRESS */

        ಆದರೆ (ngx_ಕೂಡಿಸು_event(wev, NGX_WRITE_EVENT, event) != NGX_OK) {
            ಹೋಗು failed;
        }

        ಹಿಂತಿರುಗಿ NGX_AGAIN;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->ಲಘುಗಣಕ, 0, "connected");

    wev->ready = 1;

    ಹಿಂತಿರುಗಿ NGX_OK;

failed:

    ngx_close_connection(c);
    rec->tcp = ಖಾಲಿ;

    ಹಿಂತಿರುಗಿ NGX_ERROR;
}


ಸ್ಥಿರ ngx_int_t
ngx_resolver_cmp_srvs(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *one, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *two)
{
    ngx_int_t            p1, p2;
    ngx_resolver_srv_t  *ಮೊದಲ, *ಎರಡನೇ;

    ಮೊದಲ = (ngx_resolver_srv_t *) one;
    ಎರಡನೇ = (ngx_resolver_srv_t *) two;

    p1 = ಮೊದಲ->ಆದ್ಯತೆ;
    p2 = ಎರಡನೇ->ಆದ್ಯತೆ;

    ಹಿಂತಿರುಗಿ p1 - p2;
}
