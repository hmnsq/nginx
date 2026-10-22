
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>
#ಅನ್ತರ್ಭಾವ <ngx_event.h>


#define NGX_RESOLVER_UDP_SIZE   4096

#define NGX_RESOLVER_TCP_RSIZE  (2 + 65535)
#define NGX_RESOLVER_TCP_WSIZE  8192


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
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


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    u_char  type_hi;
    u_char  type_lo;
    u_char  class_hi;
    u_char  class_lo;
} ngx_resolver_qs_t;


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    u_char  type_hi;
    u_char  type_lo;
    u_char  class_hi;
    u_char  class_lo;
    u_char  ttl[4];
    u_char  len_hi;
    u_char  len_lo;
} ngx_resolver_an_t;


#define ngx_resolver_ಗಂಟು(n)  ngx_rbtree_data(n, ngx_resolver_ಗಂಟು_t, ಗಂಟು)


ಸ್ಥಿರ ngx_int_t ngx_udp_connect(ngx_resolver_connection_t *rec);
ಸ್ಥಿರ ngx_int_t ngx_tcp_connect(ngx_resolver_connection_t *rec);


ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_cleanup(ಶೂನ್ಯ *data);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_cleanup_ವೃಕ್ಷ(ngx_resolver_t *r, ngx_rbtree_t *ವೃಕ್ಷ);
ಸ್ಥಿರ ngx_int_t ngx_resolve_name_locked(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx, ngx_str_t *name);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *ವೃಕ್ಷ,
    ngx_queue_t *queue);
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
ಸ್ಥಿರ time_t ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *ವೃಕ್ಷ,
    ngx_queue_t *queue);
ಸ್ಥಿರ ngx_uint_t ngx_resolver_resend_empty(ngx_resolver_t *r);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_udp_read(ngx_event_t *rev);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_tcp_write(ngx_event_t *wev);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_tcp_read(ngx_event_t *rev);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf,
    size_t n, ngx_uint_t tcp);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan);
ಸ್ಥಿರ ngx_resolver_ಗಂಟು_t *ngx_resolver_lookup_name(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
ಸ್ಥಿರ ngx_resolver_ಗಂಟು_t *ngx_resolver_lookup_srv(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
ಸ್ಥಿರ ngx_resolver_ಗಂಟು_t *ngx_resolver_lookup_addr(ngx_resolver_t *r,
    in_addr_t addr);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_rbtree_ಸೇರಿಸು_ಮೌಲ್ಯ(ngx_rbtree_ಗಂಟು_t *temp,
    ngx_rbtree_ಗಂಟು_t *ಗಂಟು, ngx_rbtree_ಗಂಟು_t *sentinel);
ಸ್ಥಿರ ngx_int_t ngx_resolver_copy(ngx_resolver_t *r, ngx_str_t *name,
    u_char *buf, u_char *src, u_char *last);
ಸ್ಥಿರ ngx_int_t ngx_resolver_set_timeout(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_timeout_handler(ngx_event_t *ev);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_free_ಗಂಟು(ngx_resolver_t *r, ngx_resolver_ಗಂಟು_t *rn);
ಸ್ಥಿರ ಶೂನ್ಯ *ngx_resolver_alloc(ngx_resolver_t *r, size_t size);
ಸ್ಥಿರ ಶೂನ್ಯ *ngx_resolver_calloc(ngx_resolver_t *r, size_t size);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_free(ngx_resolver_t *r, ಶೂನ್ಯ *p);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_free_locked(ngx_resolver_t *r, ಶೂನ್ಯ *p);
ಸ್ಥಿರ ಶೂನ್ಯ *ngx_resolver_dup(ngx_resolver_t *r, ಶೂನ್ಯ *src, size_t size);
ಸ್ಥಿರ ngx_resolver_addr_t *ngx_resolver_export(ngx_resolver_t *r,
    ngx_resolver_ಗಂಟು_t *rn, ngx_uint_t rotate);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_report_srv(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx);
ಸ್ಥಿರ u_char *ngx_resolver_log_error(ngx_log_t *log, u_char *buf, size_t len);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_resolve_srv_names(ngx_resolver_ctx_t *ctx,
    ngx_resolver_ಗಂಟು_t *rn);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_srv_names_handler(ngx_resolver_ctx_t *ctx);
ಸ್ಥಿರ ngx_int_t ngx_resolver_cmp_srvs(ಧ್ರುವ ಶೂನ್ಯ *one, ಧ್ರುವ ಶೂನ್ಯ *two);

#ಯದಿ (NGX_HAVE_INET6)
ಸ್ಥಿರ ಶೂನ್ಯ ngx_resolver_rbtree_ಸೇರಿಸು_addr6_ಮೌಲ್ಯ(ngx_rbtree_ಗಂಟು_t *temp,
    ngx_rbtree_ಗಂಟು_t *ಗಂಟು, ngx_rbtree_ಗಂಟು_t *sentinel);
ಸ್ಥಿರ ngx_resolver_ಗಂಟು_t *ngx_resolver_lookup_addr6(ngx_resolver_t *r,
    ಸಂರಚನಾ in6_addr *addr, uint32_t hash);
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

    r = ngx_pcalloc(cf->pool, ಪರಿಮಾಣ(ngx_resolver_t));
    ಯದಿ (r == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    r->event = ngx_pcalloc(cf->pool, ಪರಿಮಾಣ(ngx_event_t));
    ಯದಿ (r->event == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    cln = ngx_pool_cleanup_add(cf->pool, 0);
    ಯದಿ (cln == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    cln->handler = ngx_resolver_cleanup;
    cln->data = r;

    r->ipv4 = 1;

    ngx_rbtree_init(&r->name_rbtree, &r->name_sentinel,
                    ngx_resolver_rbtree_ಸೇರಿಸು_ಮೌಲ್ಯ);

    ngx_rbtree_init(&r->srv_rbtree, &r->srv_sentinel,
                    ngx_resolver_rbtree_ಸೇರಿಸು_ಮೌಲ್ಯ);

    ngx_rbtree_init(&r->addr_rbtree, &r->addr_sentinel,
                    ngx_rbtree_ಸೇರಿಸು_ಮೌಲ್ಯ);

    ngx_queue_init(&r->name_resend_queue);
    ngx_queue_init(&r->srv_resend_queue);
    ngx_queue_init(&r->addr_resend_queue);

    ngx_queue_init(&r->name_expire_queue);
    ngx_queue_init(&r->srv_expire_queue);
    ngx_queue_init(&r->addr_expire_queue);

#ಯದಿ (NGX_HAVE_INET6)
    r->ipv6 = 1;

    ngx_rbtree_init(&r->addr6_rbtree, &r->addr6_sentinel,
                    ngx_resolver_rbtree_ಸೇರಿಸು_addr6_ಮೌಲ್ಯ);

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

    ಯದಿ (n) {
        ಯದಿ (ngx_array_init(&r->connections, cf->pool, n,
                           ಪರಿಮಾಣ(ngx_resolver_connection_t))
            != NGX_OK)
        {
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        }
    }

    ಪ್ರತಿ (i = 0; i < n; i++) {
        ಯದಿ (ngx_strncmp(names[i].data, "valid=", 6) == 0) {
            s.len = names[i].len - 6;
            s.data = names[i].data + 6;

            r->valid = ngx_parse_time(&s, 1);

            ಯದಿ (r->valid == (time_t) NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &names[i]);
                ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
            }

            ಅನುವರ್ತ;
        }

#ಯದಿ (NGX_HAVE_INET6)
        ಯದಿ (ngx_strncmp(names[i].data, "ipv4=", 5) == 0) {

            ಯದಿ (ngx_strcmp(&names[i].data[5], "on") == 0) {
                r->ipv4 = 1;

            } ಅನ್ಯಥಾ ಯದಿ (ngx_strcmp(&names[i].data[5], "off") == 0) {
                r->ipv4 = 0;

            } ಅನ್ಯಥಾ {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &names[i]);
                ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
            }

            ಅನುವರ್ತ;
        }

        ಯದಿ (ngx_strncmp(names[i].data, "ipv6=", 5) == 0) {

            ಯದಿ (ngx_strcmp(&names[i].data[5], "on") == 0) {
                r->ipv6 = 1;

            } ಅನ್ಯಥಾ ಯದಿ (ngx_strcmp(&names[i].data[5], "off") == 0) {
                r->ipv6 = 0;

            } ಅನ್ಯಥಾ {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &names[i]);
                ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
            }

            ಅನುವರ್ತ;
        }
#endif

        ngx_memzero(&u, ಪರಿಮಾಣ(ngx_url_t));

        u.url = names[i];
        u.default_port = 53;

        ಯದಿ (ngx_parse_url(cf->pool, &u) != NGX_OK) {
            ಯದಿ (u.err) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "%s in resolver \"%V\"",
                                   u.err, &u.url);
            }

            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        }

        rec = ngx_array_ತಳ್ಳು_n(&r->connections, u.naddrs);
        ಯದಿ (rec == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        }

        ngx_memzero(rec, u.naddrs * ಪರಿಮಾಣ(ngx_resolver_connection_t));

        ಪ್ರತಿ (j = 0; j < u.naddrs; j++) {
            rec[j].sockaddr = u.addrs[j].sockaddr;
            rec[j].socklen = u.addrs[j].socklen;
            rec[j].server = u.addrs[j].name;
            rec[j].resolver = r;
        }
    }

#ಯದಿ (NGX_HAVE_INET6)
    ಯದಿ (r->ipv4 + r->ipv6 == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"ipv4\" and \"ipv6\" cannot both be \"off\"");
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }
#endif

    ಯದಿ (n && r->connections.nelts == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "no name servers defined");
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಪ್ರತ್ಯಾಗಮ r;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_resolver_cleanup(ಶೂನ್ಯ *data)
{
    ngx_resolver_t  *r = data;

    ngx_uint_t                  i;
    ngx_resolver_connection_t  *rec;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "cleanup resolver");

    ngx_resolver_cleanup_ವೃಕ್ಷ(r, &r->name_rbtree);

    ngx_resolver_cleanup_ವೃಕ್ಷ(r, &r->srv_rbtree);

    ngx_resolver_cleanup_ವೃಕ್ಷ(r, &r->addr_rbtree);

#ಯದಿ (NGX_HAVE_INET6)
    ngx_resolver_cleanup_ವೃಕ್ಷ(r, &r->addr6_rbtree);
#endif

    ಯದಿ (r->event->timer_set) {
        ngx_del_timer(r->event);
    }

    rec = r->connections.elts;

    ಪ್ರತಿ (i = 0; i < r->connections.nelts; i++) {
        ಯದಿ (rec[i].udp) {
            ngx_close_connection(rec[i].udp);
        }

        ಯದಿ (rec[i].tcp) {
            ngx_close_connection(rec[i].tcp);
        }

        ಯದಿ (rec[i].read_buf) {
            ngx_resolver_free(r, rec[i].read_buf->start);
            ngx_resolver_free(r, rec[i].read_buf);
        }

        ಯದಿ (rec[i].write_buf) {
            ngx_resolver_free(r, rec[i].write_buf->start);
            ngx_resolver_free(r, rec[i].write_buf);
        }
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_resolver_cleanup_ವೃಕ್ಷ(ngx_resolver_t *r, ngx_rbtree_t *ವೃಕ್ಷ)
{
    ngx_resolver_ctx_t   *ctx, *next;
    ngx_resolver_ಗಂಟು_t  *rn;

    ಯಾವತ್ (ವೃಕ್ಷ->ಬೇರು != ವೃಕ್ಷ->sentinel) {

        rn = ngx_resolver_ಗಂಟು(ngx_rbtree_min(ವೃಕ್ಷ->ಬೇರು, ವೃಕ್ಷ->sentinel));

        ngx_queue_remove(&rn->queue);

        ಪ್ರತಿ (ctx = rn->waiting; ctx; ctx = next) {
            next = ctx->next;

            ಯದಿ (ctx->event) {
                ಯದಿ (ctx->event->timer_set) {
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
ngx_resolve_start(ngx_resolver_t *r, ngx_resolver_ctx_t *temp)
{
    in_addr_t            addr;
    ngx_resolver_ctx_t  *ctx;

    ಯದಿ (temp) {
        addr = ngx_inet_addr(temp->name.data, temp->name.len);

        ಯದಿ (addr != INADDR_NONE) {
            temp->resolver = r;
            temp->state = NGX_OK;
            temp->naddrs = 1;
            temp->addrs = &temp->addr;
            temp->addr.sockaddr = (ಸಂರಚನಾ sockaddr *) &temp->sin;
            temp->addr.socklen = ಪರಿಮಾಣ(ಸಂರಚನಾ sockaddr_in);
            ngx_memzero(&temp->sin, ಪರಿಮಾಣ(ಸಂರಚನಾ sockaddr_in));
            temp->sin.sin_family = AF_INET;
            temp->sin.sin_addr.s_addr = addr;
            temp->quick = 1;

            ಪ್ರತ್ಯಾಗಮ temp;
        }
    }

    ಯದಿ (r->connections.nelts == 0) {
        ಪ್ರತ್ಯಾಗಮ NGX_NO_RESOLVER;
    }

    ctx = ngx_resolver_calloc(r, ಪರಿಮಾಣ(ngx_resolver_ctx_t));

    ಯದಿ (ctx) {
        ctx->resolver = r;
    }

    ಪ್ರತ್ಯಾಗಮ ctx;
}


ngx_int_t
ngx_resolve_name(ngx_resolver_ctx_t *ctx)
{
    size_t           slen;
    ngx_int_t        rc;
    ngx_str_t        name;
    ngx_resolver_t  *r;

    r = ctx->resolver;

    ಯದಿ (ctx->name.len > 0 && ctx->name.data[ctx->name.len - 1] == '.') {
        ctx->name.len--;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve: \"%V\"", &ctx->name);

    ಯದಿ (ctx->quick) {
        ctx->handler(ctx);
        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    ಯದಿ (ctx->service.len) {
        slen = ctx->service.len;

        ಯದಿ (ngx_strlchr(ctx->service.data,
                        ctx->service.data + ctx->service.len, '.')
            == ರಿಕ್ತ)
        {
            slen += ಪರಿಮಾಣ("_._tcp") - 1;
        }

        name.len = slen + 1 + ctx->name.len;

        name.data = ngx_resolver_alloc(r, name.len);
        ಯದಿ (name.data == ರಿಕ್ತ) {
            ಗಚ್ಛ failed;
        }

        ಯದಿ (slen == ctx->service.len) {
            ngx_sprintf(name.data, "%V.%V", &ctx->service, &ctx->name);

        } ಅನ್ಯಥಾ {
            ngx_sprintf(name.data, "_%V._tcp.%V", &ctx->service, &ctx->name);
        }

        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &name);

        ngx_resolver_free(r, name.data);

    } ಅನ್ಯಥಾ {
        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &ctx->name);
    }

    ಯದಿ (rc == NGX_OK) {
        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    /* unlock name mutex */

    ಯದಿ (rc == NGX_AGAIN) {
        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    /* NGX_ERROR */

    ಯದಿ (ctx->event) {
        ngx_resolver_free(r, ctx->event);
    }

failed:

    ngx_resolver_free(r, ctx);

    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
}


ಶೂನ್ಯ
ngx_resolve_name_done(ngx_resolver_ctx_t *ctx)
{
    ngx_uint_t            i;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_ಗಂಟು_t  *rn;

    r = ctx->resolver;

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve name done: %i", ctx->state);

    ಯದಿ (ctx->quick) {
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ (ctx->event && ctx->event->timer_set) {
        ngx_del_timer(ctx->event);
    }

    /* lock name mutex */

    ಯದಿ (ctx->nsrvs) {
        ಪ್ರತಿ (i = 0; i < ctx->nsrvs; i++) {
            ಯದಿ (ctx->srvs[i].ctx) {
                ngx_resolve_name_done(ctx->srvs[i].ctx);
            }

            ಯದಿ (ctx->srvs[i].addrs) {
                ngx_resolver_free(r, ctx->srvs[i].addrs->sockaddr);
                ngx_resolver_free(r, ctx->srvs[i].addrs);
            }

            ngx_resolver_free(r, ctx->srvs[i].name.data);
        }

        ngx_resolver_free(r, ctx->srvs);
    }

    ಯದಿ (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->ಗಂಟು;

        ಯದಿ (rn) {
            p = &rn->waiting;
            w = rn->waiting;

            ಯಾವತ್ (w) {
                ಯದಿ (w == ctx) {
                    *p = w->next;

                    ಗಚ್ಛ done;
                }

                p = &w->next;
                w = w->next;
            }

            ngx_log_error(NGX_LOG_ALERT, r->log, 0,
                          "could not cancel %V resolving", &ctx->name);
        }
    }

done:

    ಯದಿ (ctx->service.len) {
        ngx_resolver_expire(r, &r->srv_rbtree, &r->srv_expire_queue);

    } ಅನ್ಯಥಾ {
        ngx_resolver_expire(r, &r->name_rbtree, &r->name_expire_queue);
    }

    /* unlock name mutex */

    /* lock alloc mutex */

    ಯದಿ (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    ಯದಿ (r->event->timer_set && ngx_resolver_resend_empty(r)) {
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
    ngx_queue_t          *resend_queue, *expire_queue;
    ngx_rbtree_t         *ವೃಕ್ಷ;
    ngx_resolver_ctx_t   *next, *last;
    ngx_resolver_addr_t  *addrs;
    ngx_resolver_ಗಂಟು_t  *rn;

    ngx_strlow(name->data, name->data, name->len);

    hash = ngx_crc32_short(name->data, name->len);

    ಯದಿ (ctx->service.len) {
        rn = ngx_resolver_lookup_srv(r, name, hash);

        ವೃಕ್ಷ = &r->srv_rbtree;
        resend_queue = &r->srv_resend_queue;
        expire_queue = &r->srv_expire_queue;

    } ಅನ್ಯಥಾ {
        rn = ngx_resolver_lookup_name(r, name, hash);

        ವೃಕ್ಷ = &r->name_rbtree;
        resend_queue = &r->name_resend_queue;
        expire_queue = &r->name_expire_queue;
    }

    ಯದಿ (rn) {

        /* ctx can be a list after NGX_RESOLVE_CNAME */
        ಪ್ರತಿ (last = ctx; last->next; last = last->next);

        ಯದಿ (rn->valid >= ngx_time()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0, "resolve cached");

            ngx_queue_remove(&rn->queue);

            rn->expire = ngx_time() + r->expire;

            ngx_queue_ಸೇರಿಸು_head(expire_queue, &rn->queue);

            naddrs = (rn->naddrs == (u_short) -1) ? 0 : rn->naddrs;
#ಯದಿ (NGX_HAVE_INET6)
            naddrs += (rn->naddrs6 == (u_short) -1) ? 0 : rn->naddrs6;
#endif

            ಯದಿ (naddrs) {

                ಯದಿ (naddrs == 1 && rn->naddrs == 1) {
                    addrs = ರಿಕ್ತ;

                } ಅನ್ಯಥಾ {
                    addrs = ngx_resolver_export(r, rn, 1);
                    ಯದಿ (addrs == ರಿಕ್ತ) {
                        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
                    }
                }

                last->next = rn->waiting;
                rn->waiting = ರಿಕ್ತ;

                /* unlock name mutex */

                ಕುರು {
                    ctx->state = NGX_OK;
                    ctx->valid = rn->valid;
                    ctx->naddrs = naddrs;

                    ಯದಿ (addrs == ರಿಕ್ತ) {
                        ctx->addrs = &ctx->addr;
                        ctx->addr.sockaddr = (ಸಂರಚನಾ sockaddr *) &ctx->sin;
                        ctx->addr.socklen = ಪರಿಮಾಣ(ಸಂರಚನಾ sockaddr_in);
                        ngx_memzero(&ctx->sin, ಪರಿಮಾಣ(ಸಂರಚನಾ sockaddr_in));
                        ctx->sin.sin_family = AF_INET;
                        ctx->sin.sin_addr.s_addr = rn->u.addr;

                    } ಅನ್ಯಥಾ {
                        ctx->addrs = addrs;
                    }

                    next = ctx->next;

                    ctx->handler(ctx);

                    ctx = next;
                } ಯಾವತ್ (ctx);

                ಯದಿ (addrs != ರಿಕ್ತ) {
                    ngx_resolver_free(r, addrs->sockaddr);
                    ngx_resolver_free(r, addrs);
                }

                ಪ್ರತ್ಯಾಗಮ NGX_OK;
            }

            ಯದಿ (rn->nsrvs) {
                last->next = rn->waiting;
                rn->waiting = ರಿಕ್ತ;

                /* unlock name mutex */

                ಕುರು {
                    next = ctx->next;

                    ngx_resolver_resolve_srv_names(ctx, rn);

                    ctx = next;
                } ಯಾವತ್ (ctx);

                ಪ್ರತ್ಯಾಗಮ NGX_OK;
            }

            /* NGX_RESOLVE_CNAME */

            ಯದಿ (ctx->recursion++ < NGX_RESOLVER_MAX_RECURSION) {

                cname.len = rn->cnlen;
                cname.data = rn->u.cname;

                ಪ್ರತ್ಯಾಗಮ ngx_resolve_name_locked(r, ctx, &cname);
            }

            last->next = rn->waiting;
            rn->waiting = ರಿಕ್ತ;

            /* unlock name mutex */

            ಕುರು {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
                next = ctx->next;

                ctx->handler(ctx);

                ctx = next;
            } ಯಾವತ್ (ctx);

            ಪ್ರತ್ಯಾಗಮ NGX_OK;
        }

        ಯದಿ (rn->waiting) {
            ಯದಿ (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            last->next = rn->waiting;
            rn->waiting = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;

            ಕುರು {
                ctx->ಗಂಟು = rn;
                ctx = ctx->next;
            } ಯಾವತ್ (ctx);

            ಪ್ರತ್ಯಾಗಮ NGX_AGAIN;
        }

        ngx_queue_remove(&rn->queue);

        /* lock alloc mutex */

        ಯದಿ (rn->query) {
            ngx_resolver_free_locked(r, rn->query);
            rn->query = ರಿಕ್ತ;
#ಯದಿ (NGX_HAVE_INET6)
            rn->query6 = ರಿಕ್ತ;
#endif
        }

        ಯದಿ (rn->cnlen) {
            ngx_resolver_free_locked(r, rn->u.cname);
        }

        ಯದಿ (rn->naddrs > 1 && rn->naddrs != (u_short) -1) {
            ngx_resolver_free_locked(r, rn->u.addrs);
        }

#ಯದಿ (NGX_HAVE_INET6)
        ಯದಿ (rn->naddrs6 > 1 && rn->naddrs6 != (u_short) -1) {
            ngx_resolver_free_locked(r, rn->u6.addrs6);
        }
#endif

        ಯದಿ (rn->nsrvs) {
            ಪ್ರತಿ (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
                ಯದಿ (rn->u.srvs[i].name.data) {
                    ngx_resolver_free_locked(r, rn->u.srvs[i].name.data);
                }
            }

            ngx_resolver_free_locked(r, rn->u.srvs);
        }

        /* unlock alloc mutex */

    } ಅನ್ಯಥಾ {

        rn = ngx_resolver_alloc(r, ಪರಿಮಾಣ(ngx_resolver_ಗಂಟು_t));
        ಯದಿ (rn == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        rn->name = ngx_resolver_dup(r, name->data, name->len);
        ಯದಿ (rn->name == ರಿಕ್ತ) {
            ngx_resolver_free(r, rn);
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        rn->ಗಂಟು.ಕೀಲಿ = hash;
        rn->nlen = (u_short) name->len;
        rn->query = ರಿಕ್ತ;
#ಯದಿ (NGX_HAVE_INET6)
        rn->query6 = ರಿಕ್ತ;
#endif

        ngx_rbtree_ಸೇರಿಸು(ವೃಕ್ಷ, &rn->ಗಂಟು);
    }

    ಯದಿ (ctx->service.len) {
        rc = ngx_resolver_create_srv_query(r, rn, name);

    } ಅನ್ಯಥಾ {
        rc = ngx_resolver_create_name_query(r, rn, name);
    }

    ಯದಿ (rc == NGX_ERROR) {
        ಗಚ್ಛ failed;
    }

    ಯದಿ (rc == NGX_DECLINED) {
        ngx_rbtree_delete(ವೃಕ್ಷ, &rn->ಗಂಟು);

        ngx_resolver_free(r, rn->query);
        ngx_resolver_free(r, rn->name);
        ngx_resolver_free(r, rn);

        ಕುರು {
            ctx->state = NGX_RESOLVE_NXDOMAIN;
            next = ctx->next;

            ctx->handler(ctx);

            ctx = next;
        } ಯಾವತ್ (ctx);

        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    rn->last_connection = r->last_connection++;
    ಯದಿ (r->last_connection == r->connections.nelts) {
        r->last_connection = 0;
    }

    rn->naddrs = r->ipv4 ? (u_short) -1 : 0;
    rn->tcp = 0;
#ಯದಿ (NGX_HAVE_INET6)
    rn->naddrs6 = r->ipv6 ? (u_short) -1 : 0;
    rn->tcp6 = 0;
#endif
    rn->nsrvs = 0;

    ಯದಿ (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->last_connection++;
        ಯದಿ (rn->last_connection == r->connections.nelts) {
            rn->last_connection = 0;
        }

        (ಶೂನ್ಯ) ngx_resolver_send_query(r, rn);
    }

    ಯದಿ (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
        ಗಚ್ಛ failed;
    }

    ಯದಿ (ngx_resolver_resend_empty(r)) {
        ngx_add_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_time() + r->resend_timeout;

    ngx_queue_ಸೇರಿಸು_head(resend_queue, &rn->queue);

    rn->code = 0;
    rn->cnlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_ಮೌಲ್ಯ;
    rn->waiting = ctx;

    ctx->state = NGX_AGAIN;
    ctx->async = 1;

    ಕುರು {
        ctx->ಗಂಟು = rn;
        ctx = ctx->next;
    } ಯಾವತ್ (ctx);

    ಪ್ರತ್ಯಾಗಮ NGX_AGAIN;

failed:

    ngx_rbtree_delete(ವೃಕ್ಷ, &rn->ಗಂಟು);

    ಯದಿ (rn->query) {
        ngx_resolver_free(r, rn->query);
    }

    ngx_resolver_free(r, rn->name);

    ngx_resolver_free(r, rn);

    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
}


ngx_int_t
ngx_resolve_addr(ngx_resolver_ctx_t *ctx)
{
    u_char               *name;
    in_addr_t             addr;
    ngx_queue_t          *resend_queue, *expire_queue;
    ngx_rbtree_t         *ವೃಕ್ಷ;
    ngx_resolver_t       *r;
    ಸಂರಚನಾ sockaddr_in   *sin;
    ngx_resolver_ಗಂಟು_t  *rn;
#ಯದಿ (NGX_HAVE_INET6)
    uint32_t              hash;
    ಸಂರಚನಾ sockaddr_in6  *sin6;
#endif

#ಯದಿ (NGX_SUPPRESS_WARN)
    addr = 0;
#ಯದಿ (NGX_HAVE_INET6)
    hash = 0;
    sin6 = ರಿಕ್ತ;
#endif
#endif

    r = ctx->resolver;

    ವಿಕಲ್ಪ (ctx->addr.sockaddr->sa_family) {

#ಯದಿ (NGX_HAVE_INET6)
    ಪಕ್ಷ AF_INET6:
        sin6 = (ಸಂರಚನಾ sockaddr_in6 *) ctx->addr.sockaddr;
        hash = ngx_crc32_short(sin6->sin6_addr.s6_addr, 16);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr6(r, &sin6->sin6_addr, hash);

        ವೃಕ್ಷ = &r->addr6_rbtree;
        resend_queue = &r->addr6_resend_queue;
        expire_queue = &r->addr6_expire_queue;

        ಭಙ್ಗ;
#endif

    ಸ್ವಭಾವ: /* AF_INET */
        sin = (ಸಂರಚನಾ sockaddr_in *) ctx->addr.sockaddr;
        addr = ntohl(sin->sin_addr.s_addr);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        ವೃಕ್ಷ = &r->addr_rbtree;
        resend_queue = &r->addr_resend_queue;
        expire_queue = &r->addr_expire_queue;
    }

    ಯದಿ (rn) {

        ಯದಿ (rn->valid >= ngx_time()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0, "resolve cached");

            ngx_queue_remove(&rn->queue);

            rn->expire = ngx_time() + r->expire;

            ngx_queue_ಸೇರಿಸು_head(expire_queue, &rn->queue);

            name = ngx_resolver_dup(r, rn->name, rn->nlen);
            ಯದಿ (name == ರಿಕ್ತ) {
                ngx_resolver_free(r, ctx);
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            ctx->name.len = rn->nlen;
            ctx->name.data = name;

            /* unlock addr mutex */

            ctx->state = NGX_OK;
            ctx->valid = rn->valid;

            ctx->handler(ctx);

            ngx_resolver_free(r, name);

            ಪ್ರತ್ಯಾಗಮ NGX_OK;
        }

        ಯದಿ (rn->waiting) {
            ಯದಿ (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            ctx->next = rn->waiting;
            rn->waiting = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;
            ctx->ಗಂಟು = rn;

            /* unlock addr mutex */

            ಪ್ರತ್ಯಾಗಮ NGX_OK;
        }

        ngx_queue_remove(&rn->queue);

        ngx_resolver_free(r, rn->query);
        rn->query = ರಿಕ್ತ;
#ಯದಿ (NGX_HAVE_INET6)
        rn->query6 = ರಿಕ್ತ;
#endif

    } ಅನ್ಯಥಾ {
        rn = ngx_resolver_alloc(r, ಪರಿಮಾಣ(ngx_resolver_ಗಂಟು_t));
        ಯದಿ (rn == ರಿಕ್ತ) {
            ಗಚ್ಛ failed;
        }

        ವಿಕಲ್ಪ (ctx->addr.sockaddr->sa_family) {

#ಯದಿ (NGX_HAVE_INET6)
        ಪಕ್ಷ AF_INET6:
            rn->addr6 = sin6->sin6_addr;
            rn->ಗಂಟು.ಕೀಲಿ = hash;
            ಭಙ್ಗ;
#endif

        ಸ್ವಭಾವ: /* AF_INET */
            rn->ಗಂಟು.ಕೀಲಿ = addr;
        }

        rn->query = ರಿಕ್ತ;
#ಯದಿ (NGX_HAVE_INET6)
        rn->query6 = ರಿಕ್ತ;
#endif

        ngx_rbtree_ಸೇರಿಸು(ವೃಕ್ಷ, &rn->ಗಂಟು);
    }

    ಯದಿ (ngx_resolver_create_addr_query(r, rn, &ctx->addr) != NGX_OK) {
        ಗಚ್ಛ failed;
    }

    rn->last_connection = r->last_connection++;
    ಯದಿ (r->last_connection == r->connections.nelts) {
        r->last_connection = 0;
    }

    rn->naddrs = (u_short) -1;
    rn->tcp = 0;
#ಯದಿ (NGX_HAVE_INET6)
    rn->naddrs6 = (u_short) -1;
    rn->tcp6 = 0;
#endif
    rn->nsrvs = 0;

    ಯದಿ (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->last_connection++;
        ಯದಿ (rn->last_connection == r->connections.nelts) {
            rn->last_connection = 0;
        }

        (ಶೂನ್ಯ) ngx_resolver_send_query(r, rn);
    }

    ಯದಿ (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
        ಗಚ್ಛ failed;
    }

    ಯದಿ (ngx_resolver_resend_empty(r)) {
        ngx_add_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_time() + r->resend_timeout;

    ngx_queue_ಸೇರಿಸು_head(resend_queue, &rn->queue);

    rn->code = 0;
    rn->cnlen = 0;
    rn->name = ರಿಕ್ತ;
    rn->nlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_ಮೌಲ್ಯ;
    rn->waiting = ctx;

    /* unlock addr mutex */

    ctx->state = NGX_AGAIN;
    ctx->async = 1;
    ctx->ಗಂಟು = rn;

    ಪ್ರತ್ಯಾಗಮ NGX_OK;

failed:

    ಯದಿ (rn) {
        ngx_rbtree_delete(ವೃಕ್ಷ, &rn->ಗಂಟು);

        ಯದಿ (rn->query) {
            ngx_resolver_free(r, rn->query);
        }

        ngx_resolver_free(r, rn);
    }

    /* unlock addr mutex */

    ಯದಿ (ctx->event) {
        ngx_resolver_free(r, ctx->event);
    }

    ngx_resolver_free(r, ctx);

    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
}


ಶೂನ್ಯ
ngx_resolve_addr_done(ngx_resolver_ctx_t *ctx)
{
    ngx_queue_t          *expire_queue;
    ngx_rbtree_t         *ವೃಕ್ಷ;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_ಗಂಟು_t  *rn;

    r = ctx->resolver;

    ವಿಕಲ್ಪ (ctx->addr.sockaddr->sa_family) {

#ಯದಿ (NGX_HAVE_INET6)
    ಪಕ್ಷ AF_INET6:
        ವೃಕ್ಷ = &r->addr6_rbtree;
        expire_queue = &r->addr6_expire_queue;
        ಭಙ್ಗ;
#endif

    ಸ್ವಭಾವ: /* AF_INET */
        ವೃಕ್ಷ = &r->addr_rbtree;
        expire_queue = &r->addr_expire_queue;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve addr done: %i", ctx->state);

    ಯದಿ (ctx->event && ctx->event->timer_set) {
        ngx_del_timer(ctx->event);
    }

    /* lock addr mutex */

    ಯದಿ (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->ಗಂಟು;

        ಯದಿ (rn) {
            p = &rn->waiting;
            w = rn->waiting;

            ಯಾವತ್ (w) {
                ಯದಿ (w == ctx) {
                    *p = w->next;

                    ಗಚ್ಛ done;
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

    ngx_resolver_expire(r, ವೃಕ್ಷ, expire_queue);

    /* unlock addr mutex */

    /* lock alloc mutex */

    ಯದಿ (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    ಯದಿ (r->event->timer_set && ngx_resolver_resend_empty(r)) {
        ngx_del_timer(r->event);
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *ವೃಕ್ಷ, ngx_queue_t *queue)
{
    time_t                now;
    ngx_uint_t            i;
    ngx_queue_t          *q;
    ngx_resolver_ಗಂಟು_t  *rn;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver expire");

    now = ngx_time();

    ಪ್ರತಿ (i = 0; i < 2; i++) {
        ಯದಿ (ngx_queue_empty(queue)) {
            ಪ್ರತ್ಯಾಗಮ;
        }

        q = ngx_queue_last(queue);

        rn = ngx_queue_data(q, ngx_resolver_ಗಂಟು_t, queue);

        ಯದಿ (now <= rn->expire) {
            ಪ್ರತ್ಯಾಗಮ;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver expire \"%*s\"", (size_t) rn->nlen, rn->name);

        ngx_queue_remove(q);

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
    rec = &rec[rn->last_connection];

    ಯದಿ (rec->log.handler == ರಿಕ್ತ) {
        rec->log = *r->log;
        rec->log.handler = ngx_resolver_log_error;
        rec->log.data = rec;
        rec->log.action = "resolving";
    }

    ಯದಿ (rn->query && rn->naddrs == (u_short) -1) {
        rc = rn->tcp ? ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen)
                     : ngx_resolver_send_udp_query(r, rec, rn->query, rn->qlen);

        ಯದಿ (rc != NGX_OK) {
            ಪ್ರತ್ಯಾಗಮ rc;
        }
    }

#ಯದಿ (NGX_HAVE_INET6)

    ಯದಿ (rn->query6 && rn->naddrs6 == (u_short) -1) {
        rc = rn->tcp6
                    ? ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen)
                    : ngx_resolver_send_udp_query(r, rec, rn->query6, rn->qlen);

        ಯದಿ (rc != NGX_OK) {
            ಪ್ರತ್ಯಾಗಮ rc;
        }
    }

#endif

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ಸ್ಥಿರ ngx_int_t
ngx_resolver_send_udp_query(ngx_resolver_t *r, ngx_resolver_connection_t  *rec,
    u_char *query, u_short qlen)
{
    ssize_t  n;

    ಯದಿ (rec->udp == ರಿಕ್ತ) {
        ಯದಿ (ngx_udp_connect(rec) != NGX_OK) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        rec->udp->data = rec;
        rec->udp->ಪಠ->handler = ngx_resolver_udp_read;
        rec->udp->ಪಠ->resolver = 1;
    }

    n = ngx_send(rec->udp, query, qlen);

    ಯದಿ (n == NGX_ERROR) {
        ಗಚ್ಛ failed;
    }

    ಯದಿ ((size_t) n != (size_t) qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->log, 0, "send() incomplete");
        ಗಚ್ಛ failed;
    }

    ಪ್ರತ್ಯಾಗಮ NGX_OK;

failed:

    ngx_close_connection(rec->udp);
    rec->udp = ರಿಕ್ತ;

    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
}


ಸ್ಥಿರ ngx_int_t
ngx_resolver_send_tcp_query(ngx_resolver_t *r, ngx_resolver_connection_t *rec,
    u_char *query, u_short qlen)
{
    ngx_buf_t  *b;
    ngx_int_t   rc;

    rc = NGX_OK;

    ಯದಿ (rec->tcp == ರಿಕ್ತ) {
        b = rec->read_buf;

        ಯದಿ (b == ರಿಕ್ತ) {
            b = ngx_resolver_calloc(r, ಪರಿಮಾಣ(ngx_buf_t));
            ಯದಿ (b == ರಿಕ್ತ) {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            b->start = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_RSIZE);
            ಯದಿ (b->start == ರಿಕ್ತ) {
                ngx_resolver_free(r, b);
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            b->end = b->start + NGX_RESOLVER_TCP_RSIZE;

            rec->read_buf = b;
        }

        b->pos = b->start;
        b->last = b->start;

        b = rec->write_buf;

        ಯದಿ (b == ರಿಕ್ತ) {
            b = ngx_resolver_calloc(r, ಪರಿಮಾಣ(ngx_buf_t));
            ಯದಿ (b == ರಿಕ್ತ) {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            b->start = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_WSIZE);
            ಯದಿ (b->start == ರಿಕ್ತ) {
                ngx_resolver_free(r, b);
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            b->end = b->start + NGX_RESOLVER_TCP_WSIZE;

            rec->write_buf = b;
        }

        b->pos = b->start;
        b->last = b->start;

        rc = ngx_tcp_connect(rec);
        ಯದಿ (rc == NGX_ERROR) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        rec->tcp->data = rec;
        rec->tcp->ಲಿಖ->handler = ngx_resolver_tcp_write;
        rec->tcp->ಲಿಖ->cancelable = 1;
        rec->tcp->ಪಠ->handler = ngx_resolver_tcp_read;
        rec->tcp->ಪಠ->resolver = 1;

        ngx_add_timer(rec->tcp->ಲಿಖ, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    b = rec->write_buf;

    ಯದಿ (b->end - b->last <  2 + qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->log, 0, "buffer overflow");
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    *b->last++ = (u_char) (qlen >> 8);
    *b->last++ = (u_char) qlen;
    b->last = ngx_cpymem(b->last, query, qlen);

    ಯದಿ (rc == NGX_OK) {
        ngx_resolver_tcp_write(rec->tcp->ಲಿಖ);
    }

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_resolver_resend_handler(ngx_event_t *ev)
{
    time_t           timer, atimer, stimer, ntimer;
#ಯದಿ (NGX_HAVE_INET6)
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

#ಯದಿ (NGX_HAVE_INET6)

    /* lock addr6 mutex */

    a6timer = ngx_resolver_resend(r, &r->addr6_rbtree, &r->addr6_resend_queue);

    /* unlock addr6 mutex */

#endif

    timer = ntimer;

    ಯದಿ (timer == 0) {
        timer = atimer;

    } ಅನ್ಯಥಾ ಯದಿ (atimer) {
        timer = ngx_min(timer, atimer);
    }

    ಯದಿ (timer == 0) {
        timer = stimer;

    } ಅನ್ಯಥಾ ಯದಿ (stimer) {
        timer = ngx_min(timer, stimer);
    }

#ಯದಿ (NGX_HAVE_INET6)

    ಯದಿ (timer == 0) {
        timer = a6timer;

    } ಅನ್ಯಥಾ ಯದಿ (a6timer) {
        timer = ngx_min(timer, a6timer);
    }

#endif

    ಯದಿ (timer) {
        ngx_add_timer(r->event, (ngx_msec_t) (timer * 1000));
    }
}


ಸ್ಥಿರ time_t
ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *ವೃಕ್ಷ, ngx_queue_t *queue)
{
    time_t                now;
    ngx_queue_t          *q;
    ngx_resolver_ಗಂಟು_t  *rn;

    now = ngx_time();

    ಪ್ರತಿ ( ;; ) {
        ಯದಿ (ngx_queue_empty(queue)) {
            ಪ್ರತ್ಯಾಗಮ 0;
        }

        q = ngx_queue_last(queue);

        rn = ngx_queue_data(q, ngx_resolver_ಗಂಟು_t, queue);

        ಯದಿ (now < rn->expire) {
            ಪ್ರತ್ಯಾಗಮ rn->expire - now;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver resend \"%*s\" %p",
                       (size_t) rn->nlen, rn->name, rn->waiting);

        ngx_queue_remove(q);

        ಯದಿ (rn->waiting) {

            ಯದಿ (++rn->last_connection == r->connections.nelts) {
                rn->last_connection = 0;
            }

            (ಶೂನ್ಯ) ngx_resolver_send_query(r, rn);

            rn->expire = now + r->resend_timeout;

            ngx_queue_ಸೇರಿಸು_head(queue, q);

            ಅನುವರ್ತ;
        }

        ngx_rbtree_delete(ವೃಕ್ಷ, &rn->ಗಂಟು);

        ngx_resolver_free_ಗಂಟು(r, rn);
    }
}


ಸ್ಥಿರ ngx_uint_t
ngx_resolver_resend_empty(ngx_resolver_t *r)
{
    ಪ್ರತ್ಯಾಗಮ ngx_queue_empty(&r->name_resend_queue)
           && ngx_queue_empty(&r->srv_resend_queue)
#ಯದಿ (NGX_HAVE_INET6)
           && ngx_queue_empty(&r->addr6_resend_queue)
#endif
           && ngx_queue_empty(&r->addr_resend_queue);
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_resolver_udp_read(ngx_event_t *rev)
{
    ssize_t                     n;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;
    u_char                      buf[NGX_RESOLVER_UDP_SIZE];

    c = rev->data;
    rec = c->data;

    ಕುರು {
        n = ngx_udp_recv(c, buf, NGX_RESOLVER_UDP_SIZE);

        ಯದಿ (n == NGX_AGAIN) {
            ಭಙ್ಗ;
        }

        ಯದಿ (n == NGX_ERROR) {
            ಗಚ್ಛ failed;
        }

        ngx_resolver_process_response(rec->resolver, buf, n, 0);

    } ಯಾವತ್ (rev->ready);

    ಯದಿ (ngx_handle_read_event(rev, 0) != NGX_OK) {
        ಗಚ್ಛ failed;
    }

    ಪ್ರತ್ಯಾಗಮ;

failed:

    ngx_close_connection(rec->udp);
    rec->udp = ರಿಕ್ತ;
}


ಸ್ಥಿರ ಶೂನ್ಯ
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

    ಯದಿ (wev->timedout) {
        ಗಚ್ಛ failed;
    }

    sent = c->sent;

    ಯಾವತ್ (wev->ready && b->pos < b->last) {
        n = ngx_send(c, b->pos, b->last - b->pos);

        ಯದಿ (n == NGX_AGAIN) {
            ಭಙ್ಗ;
        }

        ಯದಿ (n == NGX_ERROR) {
            ಗಚ್ಛ failed;
        }

        b->pos += n;
    }

    ಯದಿ (b->pos != b->start) {
        b->last = ngx_movemem(b->start, b->pos, b->last - b->pos);
        b->pos = b->start;
    }

    ಯದಿ (c->sent != sent) {
        ngx_add_timer(wev, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    ಯದಿ (ngx_handle_write_event(wev, 0) != NGX_OK) {
        ಗಚ್ಛ failed;
    }

    ಪ್ರತ್ಯಾಗಮ;

failed:

    ngx_close_connection(c);
    rec->tcp = ರಿಕ್ತ;
}


ಸ್ಥಿರ ಶೂನ್ಯ
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

    ಯಾವತ್ (rev->ready) {
        n = ngx_recv(c, b->last, b->end - b->last);

        ಯದಿ (n == NGX_AGAIN) {
            ಭಙ್ಗ;
        }

        ಯದಿ (n == NGX_ERROR || n == 0) {
            ಗಚ್ಛ failed;
        }

        b->last += n;

        ಪ್ರತಿ ( ;; ) {
            p = b->pos;
            size = b->last - p;

            ಯದಿ (size < 2) {
                ಭಙ್ಗ;
            }

            qlen = (u_short) *p++ << 8;
            qlen += *p++;

            ಯದಿ (size < (size_t) (2 + qlen)) {
                ಭಙ್ಗ;
            }

            ngx_resolver_process_response(r, p, qlen, 1);

            b->pos += 2 + qlen;
        }

        ಯದಿ (b->pos != b->start) {
            b->last = ngx_movemem(b->start, b->pos, b->last - b->pos);
            b->pos = b->start;
        }
    }

    ಯದಿ (ngx_handle_read_event(rev, 0) != NGX_OK) {
        ಗಚ್ಛ failed;
    }

    ಪ್ರತ್ಯಾಗಮ;

failed:

    ngx_close_connection(c);
    rec->tcp = ರಿಕ್ತ;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t tcp)
{
    ವರ್ಣ                 *err;
    ngx_uint_t            i, times, ident, qident, flags, code, nqs, nan, trunc,
                          qtype, qclass;
#ಯದಿ (NGX_HAVE_INET6)
    ngx_uint_t            qident6;
#endif
    ngx_queue_t          *q;
    ngx_resolver_qs_t    *qs;
    ngx_resolver_hdr_t   *response;
    ngx_resolver_ಗಂಟು_t  *rn;

    ಯದಿ (n < ಪರಿಮಾಣ(ngx_resolver_hdr_t)) {
        ಗಚ್ಛ short_response;
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
    ಯದಿ ((flags & 0xf870) != 0x8000 || (trunc && tcp)) {
        ngx_log_error(r->log_level, r->log, 0,
                      "invalid %s DNS response %ui fl:%04Xi",
                      tcp ? "TCP" : "UDP", ident, flags);
        ಪ್ರತ್ಯಾಗಮ;
    }

    code = flags & 0xf;

    ಯದಿ (code == NGX_RESOLVE_FORMERR) {

        times = 0;

        ಪ್ರತಿ (q = ngx_queue_head(&r->name_resend_queue);
             q != ngx_queue_sentinel(&r->name_resend_queue) && times++ < 100;
             q = ngx_queue_next(q))
        {
            rn = ngx_queue_data(q, ngx_resolver_ಗಂಟು_t, queue);

            ಯದಿ (rn->query) {
                qident = (rn->query[0] << 8) + rn->query[1];

                ಯದಿ (qident == ident) {
                    ಗಚ್ಛ dns_error_name;
                }
            }

#ಯದಿ (NGX_HAVE_INET6)
            ಯದಿ (rn->query6) {
                qident6 = (rn->query6[0] << 8) + rn->query6[1];

                ಯದಿ (qident6 == ident) {
                    ಗಚ್ಛ dns_error_name;
                }
            }
#endif
        }

        ಗಚ್ಛ dns_error;
    }

    ಯದಿ (code > NGX_RESOLVE_REFUSED) {
        ಗಚ್ಛ dns_error;
    }

    ಯದಿ (nqs != 1) {
        err = "invalid number of questions in DNS response";
        ಗಚ್ಛ done;
    }

    i = ಪರಿಮಾಣ(ngx_resolver_hdr_t);

    ಯಾವತ್ (i < (ngx_uint_t) n) {

        ಯದಿ (buf[i] & 0xc0) {
            err = "unexpected compression pointer in DNS response";
            ಗಚ್ಛ done;
        }

        ಯದಿ (buf[i] == '\0') {
            ಗಚ್ಛ found;
        }

        i += 1 + buf[i];
    }

    ಗಚ್ಛ short_response;

found:

    ಯದಿ (i++ == ಪರಿಮಾಣ(ngx_resolver_hdr_t)) {
        err = "zero-length domain name in DNS response";
        ಗಚ್ಛ done;
    }

    ಯದಿ (i + ಪರಿಮಾಣ(ngx_resolver_qs_t) + nan * (2 + ಪರಿಮಾಣ(ngx_resolver_an_t))
        > (ngx_uint_t) n)
    {
        ಗಚ್ಛ short_response;
    }

    qs = (ngx_resolver_qs_t *) &buf[i];

    qtype = (qs->type_hi << 8) + qs->type_lo;
    qclass = (qs->class_hi << 8) + qs->class_lo;

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver DNS response qt:%ui cl:%ui", qtype, qclass);

    ಯದಿ (qclass != 1) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unknown query class %ui in DNS response", qclass);
        ಪ್ರತ್ಯಾಗಮ;
    }

    ವಿಕಲ್ಪ (qtype) {

    ಪಕ್ಷ NGX_RESOLVE_A:
#ಯದಿ (NGX_HAVE_INET6)
    ಪಕ್ಷ NGX_RESOLVE_AAAA:
#endif

        ngx_resolver_process_a(r, buf, n, ident, code, qtype, nan, trunc,
                               i + ಪರಿಮಾಣ(ngx_resolver_qs_t));

        ಭಙ್ಗ;

    ಪಕ್ಷ NGX_RESOLVE_SRV:

        ngx_resolver_process_srv(r, buf, n, ident, code, nan, trunc,
                                 i + ಪರಿಮಾಣ(ngx_resolver_qs_t));

        ಭಙ್ಗ;

    ಪಕ್ಷ NGX_RESOLVE_PTR:

        ngx_resolver_process_ptr(r, buf, n, ident, code, nan);

        ಭಙ್ಗ;

    ಸ್ವಭಾವ:
        ngx_log_error(r->log_level, r->log, 0,
                      "unknown query type %ui in DNS response", qtype);
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಪ್ರತ್ಯಾಗಮ;

short_response:

    err = "short DNS response";

done:

    ngx_log_error(r->log_level, r->log, 0, err);

    ಪ್ರತ್ಯಾಗಮ;

dns_error_name:

    ngx_log_error(r->log_level, r->log, 0,
                  "DNS error (%ui: %s), query id:%ui, name:\"%*s\"",
                  code, ngx_resolver_strerror(code), ident,
                  (size_t) rn->nlen, rn->name);
    ಪ್ರತ್ಯಾಗಮ;

dns_error:

    ngx_log_error(r->log_level, r->log, 0,
                  "DNS error (%ui: %s), query id:%ui",
                  code, ngx_resolver_strerror(code), ident);
    ಪ್ರತ್ಯಾಗಮ;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans)
{
    ವರ್ಣ                       *err;
    u_char                     *cname;
    size_t                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    in_addr_t                  *addr;
    ngx_str_t                   name;
    ngx_uint_t                  type, class, qident, naddrs, a, i, j, start;
#ಯದಿ (NGX_HAVE_INET6)
    ಸಂರಚನಾ in6_addr            *addr6;
#endif
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *next;
    ngx_resolver_ಗಂಟು_t        *rn;
    ngx_resolver_addr_t        *addrs;
    ngx_resolver_connection_t  *rec;

    ಯದಿ (ngx_resolver_copy(r, &name, buf,
                          buf + ಪರಿಮಾಣ(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        ಪ್ರತ್ಯಾಗಮ;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver qs:%V", &name);

    hash = ngx_crc32_short(name.data, name.len);

    /* lock name mutex */

    rn = ngx_resolver_lookup_name(r, &name, hash);

    ಯದಿ (rn == ರಿಕ್ತ) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.data);
        ಗಚ್ಛ failed;
    }

    ವಿಕಲ್ಪ (qtype) {

#ಯದಿ (NGX_HAVE_INET6)
    ಪಕ್ಷ NGX_RESOLVE_AAAA:

        ಯದಿ (rn->query6 == ರಿಕ್ತ || rn->naddrs6 != (u_short) -1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.data);
            ಗಚ್ಛ failed;
        }

        ಯದಿ (trunc && rn->tcp6) {
            ngx_resolver_free(r, name.data);
            ಗಚ್ಛ failed;
        }

        qident = (rn->query6[0] << 8) + rn->query6[1];

        ಭಙ್ಗ;
#endif

    ಸ್ವಭಾವ: /* NGX_RESOLVE_A */

        ಯದಿ (rn->query == ರಿಕ್ತ || rn->naddrs != (u_short) -1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.data);
            ಗಚ್ಛ failed;
        }

        ಯದಿ (trunc && rn->tcp) {
            ngx_resolver_free(r, name.data);
            ಗಚ್ಛ failed;
        }

        qident = (rn->query[0] << 8) + rn->query[1];
    }

    ಯದಿ (ident != qident) {
        ngx_log_error(r->log_level, r->log, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.data);
        ಗಚ್ಛ failed;
    }

    ngx_resolver_free(r, name.data);

    ಯದಿ (trunc) {

        ngx_queue_remove(&rn->queue);

        ಯದಿ (rn->waiting == ರಿಕ್ತ) {
            ngx_rbtree_delete(&r->name_rbtree, &rn->ಗಂಟು);
            ngx_resolver_free_ಗಂಟು(r, rn);
            ಗಚ್ಛ next;
        }

        rec = r->connections.elts;
        rec = &rec[rn->last_connection];

        ವಿಕಲ್ಪ (qtype) {

#ಯದಿ (NGX_HAVE_INET6)
        ಪಕ್ಷ NGX_RESOLVE_AAAA:

            rn->tcp6 = 1;

            (ಶೂನ್ಯ) ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen);

            ಭಙ್ಗ;
#endif

        ಸ್ವಭಾವ: /* NGX_RESOLVE_A */

            rn->tcp = 1;

            (ಶೂನ್ಯ) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);
        }

        rn->expire = ngx_time() + r->resend_timeout;

        ngx_queue_ಸೇರಿಸು_head(&r->name_resend_queue, &rn->queue);

        ಗಚ್ಛ next;
    }

    ಯದಿ (code == 0 && rn->code) {
        code = rn->code;
    }

    ಯದಿ (code == 0 && nan == 0) {

#ಯದಿ (NGX_HAVE_INET6)
        ವಿಕಲ್ಪ (qtype) {

        ಪಕ್ಷ NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            ಯದಿ (rn->naddrs == (u_short) -1) {
                ಗಚ್ಛ next;
            }

            ಯದಿ (rn->naddrs) {
                ಗಚ್ಛ export;
            }

            ಭಙ್ಗ;

        ಸ್ವಭಾವ: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            ಯದಿ (rn->naddrs6 == (u_short) -1) {
                ಗಚ್ಛ next;
            }

            ಯದಿ (rn->naddrs6) {
                ಗಚ್ಛ export;
            }
        }
#endif

        code = NGX_RESOLVE_NXDOMAIN;
    }

    ಯದಿ (code) {

#ಯದಿ (NGX_HAVE_INET6)
        ವಿಕಲ್ಪ (qtype) {

        ಪಕ್ಷ NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            ಯದಿ (rn->naddrs == (u_short) -1) {
                rn->code = (u_char) code;
                ಗಚ್ಛ next;
            }

            ಭಙ್ಗ;

        ಸ್ವಭಾವ: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            ಯದಿ (rn->naddrs6 == (u_short) -1) {
                rn->code = (u_char) code;
                ಗಚ್ಛ next;
            }
        }
#endif

        next = rn->waiting;
        rn->waiting = ರಿಕ್ತ;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(&r->name_rbtree, &rn->ಗಂಟು);

        /* unlock name mutex */

        ಯಾವತ್ (next) {
            ctx = next;
            ctx->state = code;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            next = ctx->next;

            ctx->handler(ctx);
        }

        ngx_resolver_free_ಗಂಟು(r, rn);

        ಪ್ರತ್ಯಾಗಮ;
    }

    i = ans;
    naddrs = 0;
    cname = ರಿಕ್ತ;

    ಪ್ರತಿ (a = 0; a < nan; a++) {

        start = i;

        ಯಾವತ್ (i < n) {

            ಯದಿ (buf[i] & 0xc0) {
                i += 2;
                ಗಚ್ಛ found;
            }

            ಯದಿ (buf[i] == 0) {
                i++;
                ಗಚ್ಛ test_length;
            }

            i += 1 + buf[i];
        }

        ಗಚ್ಛ short_response;

    test_length:

        ಯದಿ (i - start < 2) {
            err = "invalid name in DNS response";
            ಗಚ್ಛ invalid;
        }

    found:

        ಯದಿ (i + ಪರಿಮಾಣ(ngx_resolver_an_t) >= n) {
            ಗಚ್ಛ short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        ಯದಿ (class != 1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR class %ui in DNS response", class);
            ಗಚ್ಛ failed;
        }

        ಯದಿ (ttl < 0) {
            ttl = 0;
        }

        rn->ttl = ngx_min(rn->ttl, (uint32_t) ttl);

        i += ಪರಿಮಾಣ(ngx_resolver_an_t);

        ವಿಕಲ್ಪ (type) {

        ಪಕ್ಷ NGX_RESOLVE_A:

            ಯದಿ (qtype != NGX_RESOLVE_A) {
                err = "unexpected A record in DNS response";
                ಗಚ್ಛ invalid;
            }

            ಯದಿ (len != 4) {
                err = "invalid A record in DNS response";
                ಗಚ್ಛ invalid;
            }

            ಯದಿ (i + 4 > n) {
                ಗಚ್ಛ short_response;
            }

            naddrs++;

            ಭಙ್ಗ;

#ಯದಿ (NGX_HAVE_INET6)
        ಪಕ್ಷ NGX_RESOLVE_AAAA:

            ಯದಿ (qtype != NGX_RESOLVE_AAAA) {
                err = "unexpected AAAA record in DNS response";
                ಗಚ್ಛ invalid;
            }

            ಯದಿ (len != 16) {
                err = "invalid AAAA record in DNS response";
                ಗಚ್ಛ invalid;
            }

            ಯದಿ (i + 16 > n) {
                ಗಚ್ಛ short_response;
            }

            naddrs++;

            ಭಙ್ಗ;
#endif

        ಪಕ್ಷ NGX_RESOLVE_CNAME:

            cname = &buf[i];

            ಭಙ್ಗ;

        ಪಕ್ಷ NGX_RESOLVE_DNAME:

            ಭಙ್ಗ;

        ಸ್ವಭಾವ:

            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver naddrs:%ui cname:%p ttl:%uD",
                   naddrs, cname, rn->ttl);

    ಯದಿ (naddrs) {

        ವಿಕಲ್ಪ (qtype) {

#ಯದಿ (NGX_HAVE_INET6)
        ಪಕ್ಷ NGX_RESOLVE_AAAA:

            ಯದಿ (naddrs == 1) {
                addr6 = &rn->u6.addr6;
                rn->naddrs6 = 1;

            } ಅನ್ಯಥಾ {
                addr6 = ngx_resolver_alloc(r, naddrs * ಪರಿಮಾಣ(ಸಂರಚನಾ in6_addr));
                ಯದಿ (addr6 == ರಿಕ್ತ) {
                    ಗಚ್ಛ failed;
                }

                rn->u6.addrs6 = addr6;
                rn->naddrs6 = (u_short) naddrs;
            }

#ಯದಿ (NGX_SUPPRESS_WARN)
            addr = ರಿಕ್ತ;
#endif

            ಭಙ್ಗ;
#endif

        ಸ್ವಭಾವ: /* NGX_RESOLVE_A */

            ಯದಿ (naddrs == 1) {
                addr = &rn->u.addr;
                rn->naddrs = 1;

            } ಅನ್ಯಥಾ {
                addr = ngx_resolver_alloc(r, naddrs * ಪರಿಮಾಣ(in_addr_t));
                ಯದಿ (addr == ರಿಕ್ತ) {
                    ಗಚ್ಛ failed;
                }

                rn->u.addrs = addr;
                rn->naddrs = (u_short) naddrs;
            }

#ಯದಿ (NGX_HAVE_INET6 && NGX_SUPPRESS_WARN)
            addr6 = ರಿಕ್ತ;
#endif
        }

        j = 0;
        i = ans;

        ಪ್ರತಿ (a = 0; a < nan; a++) {

            ಪ್ರತಿ ( ;; ) {

                ಯದಿ (buf[i] & 0xc0) {
                    i += 2;
                    ಭಙ್ಗ;
                }

                ಯದಿ (buf[i] == 0) {
                    i++;
                    ಭಙ್ಗ;
                }

                i += 1 + buf[i];
            }

            an = (ngx_resolver_an_t *) &buf[i];

            type = (an->type_hi << 8) + an->type_lo;
            len = (an->len_hi << 8) + an->len_lo;

            i += ಪರಿಮಾಣ(ngx_resolver_an_t);

            ಯದಿ (type == NGX_RESOLVE_A) {

                addr[j] = htonl((buf[i] << 24) + (buf[i + 1] << 16)
                                + (buf[i + 2] << 8) + (buf[i + 3]));

                ಯದಿ (++j == naddrs) {

#ಯದಿ (NGX_HAVE_INET6)
                    ಯದಿ (rn->naddrs6 == (u_short) -1) {
                        ಗಚ್ಛ next;
                    }
#endif

                    ಭಙ್ಗ;
                }
            }

#ಯದಿ (NGX_HAVE_INET6)
            ಅನ್ಯಥಾ ಯದಿ (type == NGX_RESOLVE_AAAA) {

                ngx_memcpy(addr6[j].s6_addr, &buf[i], 16);

                ಯದಿ (++j == naddrs) {

                    ಯದಿ (rn->naddrs == (u_short) -1) {
                        ಗಚ್ಛ next;
                    }

                    ಭಙ್ಗ;
                }
            }
#endif

            i += len;
        }
    }

    ವಿಕಲ್ಪ (qtype) {

#ಯದಿ (NGX_HAVE_INET6)
    ಪಕ್ಷ NGX_RESOLVE_AAAA:

        ಯದಿ (rn->naddrs6 == (u_short) -1) {
            rn->naddrs6 = 0;
        }

        ಭಙ್ಗ;
#endif

    ಸ್ವಭಾವ: /* NGX_RESOLVE_A */

        ಯದಿ (rn->naddrs == (u_short) -1) {
            rn->naddrs = 0;
        }
    }

    ಯದಿ (rn->naddrs != (u_short) -1
#ಯದಿ (NGX_HAVE_INET6)
        && rn->naddrs6 != (u_short) -1
#endif
        && rn->naddrs
#ಯದಿ (NGX_HAVE_INET6)
           + rn->naddrs6
#endif
           > 0)
    {

#ಯದಿ (NGX_HAVE_INET6)
    export:
#endif

        naddrs = rn->naddrs;
#ಯದಿ (NGX_HAVE_INET6)
        naddrs += rn->naddrs6;
#endif

        ಯದಿ (naddrs == 1 && rn->naddrs == 1) {
            addrs = ರಿಕ್ತ;

        } ಅನ್ಯಥಾ {
            addrs = ngx_resolver_export(r, rn, 0);
            ಯದಿ (addrs == ರಿಕ್ತ) {
                ಗಚ್ಛ failed;
            }
        }

        ngx_queue_remove(&rn->queue);

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_ಸೇರಿಸು_head(&r->name_expire_queue, &rn->queue);

        next = rn->waiting;
        rn->waiting = ರಿಕ್ತ;

        /* unlock name mutex */

        ಯಾವತ್ (next) {
            ctx = next;
            ctx->state = NGX_OK;
            ctx->valid = rn->valid;
            ctx->naddrs = naddrs;

            ಯದಿ (addrs == ರಿಕ್ತ) {
                ctx->addrs = &ctx->addr;
                ctx->addr.sockaddr = (ಸಂರಚನಾ sockaddr *) &ctx->sin;
                ctx->addr.socklen = ಪರಿಮಾಣ(ಸಂರಚನಾ sockaddr_in);
                ngx_memzero(&ctx->sin, ಪರಿಮಾಣ(ಸಂರಚನಾ sockaddr_in));
                ctx->sin.sin_family = AF_INET;
                ctx->sin.sin_addr.s_addr = rn->u.addr;

            } ಅನ್ಯಥಾ {
                ctx->addrs = addrs;
            }

            next = ctx->next;

            ctx->handler(ctx);
        }

        ಯದಿ (addrs != ರಿಕ್ತ) {
            ngx_resolver_free(r, addrs->sockaddr);
            ngx_resolver_free(r, addrs);
        }

        ngx_resolver_free(r, rn->query);
        rn->query = ರಿಕ್ತ;
#ಯದಿ (NGX_HAVE_INET6)
        rn->query6 = ರಿಕ್ತ;
#endif

        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ (cname) {

        /* CNAME only */

        ಯದಿ (rn->naddrs == (u_short) -1
#ಯದಿ (NGX_HAVE_INET6)
            || rn->naddrs6 == (u_short) -1
#endif
            )
        {
            ಗಚ್ಛ next;
        }

        ಯದಿ (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            ಗಚ್ಛ failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_queue_remove(&rn->queue);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.data;

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_ಸೇರಿಸು_head(&r->name_expire_queue, &rn->queue);

        ngx_resolver_free(r, rn->query);
        rn->query = ರಿಕ್ತ;
#ಯದಿ (NGX_HAVE_INET6)
        rn->query6 = ರಿಕ್ತ;
#endif

        ctx = rn->waiting;
        rn->waiting = ರಿಕ್ತ;

        ಯದಿ (ctx) {

            ಯದಿ (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                ಕುರು {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    next = ctx->next;

                    ctx->handler(ctx);

                    ctx = next;
                } ಯಾವತ್ (ctx);

                ಪ್ರತ್ಯಾಗಮ;
            }

            ಪ್ರತಿ (next = ctx; next; next = next->next) {
                next->ಗಂಟು = ರಿಕ್ತ;
            }

            (ಶೂನ್ಯ) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        ಪ್ರತ್ಯಾಗಮ;
    }

    ngx_log_error(r->log_level, r->log, 0,
                  "no A or CNAME types in DNS response");
    ಪ್ರತ್ಯಾಗಮ;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->log, 0, err);

    ಪ್ರತ್ಯಾಗಮ;

failed:

next:

    /* unlock name mutex */

    ಪ್ರತ್ಯಾಗಮ;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans)
{
    ವರ್ಣ                       *err;
    u_char                     *cname;
    size_t                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    ngx_str_t                   name;
    ngx_uint_t                  type, qident, class, start, nsrvs, a, i, j;
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *next;
    ngx_resolver_srv_t         *srvs;
    ngx_resolver_ಗಂಟು_t        *rn;
    ngx_resolver_connection_t  *rec;

    ಯದಿ (ngx_resolver_copy(r, &name, buf,
                          buf + ಪರಿಮಾಣ(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        ಪ್ರತ್ಯಾಗಮ;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver qs:%V", &name);

    hash = ngx_crc32_short(name.data, name.len);

    rn = ngx_resolver_lookup_srv(r, &name, hash);

    ಯದಿ (rn == ರಿಕ್ತ || rn->query == ರಿಕ್ತ) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.data);
        ಗಚ್ಛ failed;
    }

    ಯದಿ (trunc && rn->tcp) {
        ngx_resolver_free(r, name.data);
        ಗಚ್ಛ failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    ಯದಿ (ident != qident) {
        ngx_log_error(r->log_level, r->log, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.data);
        ಗಚ್ಛ failed;
    }

    ngx_resolver_free(r, name.data);

    ಯದಿ (trunc) {

        ngx_queue_remove(&rn->queue);

        ಯದಿ (rn->waiting == ರಿಕ್ತ) {
            ngx_rbtree_delete(&r->srv_rbtree, &rn->ಗಂಟು);
            ngx_resolver_free_ಗಂಟು(r, rn);
            ಪ್ರತ್ಯಾಗಮ;
        }

        rec = r->connections.elts;
        rec = &rec[rn->last_connection];

        rn->tcp = 1;

        (ಶೂನ್ಯ) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);

        rn->expire = ngx_time() + r->resend_timeout;

        ngx_queue_ಸೇರಿಸು_head(&r->srv_resend_queue, &rn->queue);

        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ (code == 0 && rn->code) {
        code = rn->code;
    }

    ಯದಿ (code == 0 && nan == 0) {
        code = NGX_RESOLVE_NXDOMAIN;
    }

    ಯದಿ (code) {
        next = rn->waiting;
        rn->waiting = ರಿಕ್ತ;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(&r->srv_rbtree, &rn->ಗಂಟು);

        ಯಾವತ್ (next) {
            ctx = next;
            ctx->state = code;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            next = ctx->next;

            ctx->handler(ctx);
        }

        ngx_resolver_free_ಗಂಟು(r, rn);

        ಪ್ರತ್ಯಾಗಮ;
    }

    i = ans;
    nsrvs = 0;
    cname = ರಿಕ್ತ;

    ಪ್ರತಿ (a = 0; a < nan; a++) {

        start = i;

        ಯಾವತ್ (i < n) {

            ಯದಿ (buf[i] & 0xc0) {
                i += 2;
                ಗಚ್ಛ found;
            }

            ಯದಿ (buf[i] == 0) {
                i++;
                ಗಚ್ಛ test_length;
            }

            i += 1 + buf[i];
        }

        ಗಚ್ಛ short_response;

    test_length:

        ಯದಿ (i - start < 2) {
            err = "invalid name DNS response";
            ಗಚ್ಛ invalid;
        }

    found:

        ಯದಿ (i + ಪರಿಮಾಣ(ngx_resolver_an_t) >= n) {
            ಗಚ್ಛ short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        ಯದಿ (class != 1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR class %ui in DNS response", class);
            ಗಚ್ಛ failed;
        }

        ಯದಿ (ttl < 0) {
            ttl = 0;
        }

        rn->ttl = ngx_min(rn->ttl, (uint32_t) ttl);

        i += ಪರಿಮಾಣ(ngx_resolver_an_t);

        ವಿಕಲ್ಪ (type) {

        ಪಕ್ಷ NGX_RESOLVE_SRV:

            ಯದಿ (i + 6 >= n) {
                ಗಚ್ಛ short_response;
            }

            ಯದಿ (ngx_resolver_copy(r, ರಿಕ್ತ, buf, &buf[i + 6], buf + n)
                != NGX_OK)
            {
                ಗಚ್ಛ failed;
            }

            nsrvs++;

            ಭಙ್ಗ;

        ಪಕ್ಷ NGX_RESOLVE_CNAME:

            cname = &buf[i];

            ಭಙ್ಗ;

        ಪಕ್ಷ NGX_RESOLVE_DNAME:

            ಭಙ್ಗ;

        ಸ್ವಭಾವ:

            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver nsrvs:%ui cname:%p ttl:%uD",
                   nsrvs, cname, rn->ttl);

    ಯದಿ (nsrvs) {

        srvs = ngx_resolver_calloc(r, nsrvs * ಪರಿಮಾಣ(ngx_resolver_srv_t));
        ಯದಿ (srvs == ರಿಕ್ತ) {
            ಗಚ್ಛ failed;
        }

        rn->u.srvs = srvs;
        rn->nsrvs = (u_short) nsrvs;

        j = 0;
        i = ans;

        ಪ್ರತಿ (a = 0; a < nan; a++) {

            ಪ್ರತಿ ( ;; ) {

                ಯದಿ (buf[i] & 0xc0) {
                    i += 2;
                    ಭಙ್ಗ;
                }

                ಯದಿ (buf[i] == 0) {
                    i++;
                    ಭಙ್ಗ;
                }

                i += 1 + buf[i];
            }

            an = (ngx_resolver_an_t *) &buf[i];

            type = (an->type_hi << 8) + an->type_lo;
            len = (an->len_hi << 8) + an->len_lo;

            i += ಪರಿಮಾಣ(ngx_resolver_an_t);

            ಯದಿ (type == NGX_RESOLVE_SRV) {

                srvs[j].priority = (buf[i] << 8) + buf[i + 1];
                srvs[j].weight = (buf[i + 2] << 8) + buf[i + 3];

                ಯದಿ (srvs[j].weight == 0) {
                    srvs[j].weight = 1;
                }

                srvs[j].port = (buf[i + 4] << 8) + buf[i + 5];

                ಯದಿ (ngx_resolver_copy(r, &srvs[j].name, buf, &buf[i + 6],
                                      buf + n)
                    != NGX_OK)
                {
                    ಗಚ್ಛ failed;
                }

                j++;
            }

            i += len;
        }

        ngx_sort(srvs, nsrvs, ಪರಿಮಾಣ(ngx_resolver_srv_t),
                 ngx_resolver_cmp_srvs);

        ngx_resolver_free(r, rn->query);
        rn->query = ರಿಕ್ತ;

        ngx_queue_remove(&rn->queue);

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_ಸೇರಿಸು_head(&r->srv_expire_queue, &rn->queue);

        next = rn->waiting;
        rn->waiting = ರಿಕ್ತ;

        ಯಾವತ್ (next) {
            ctx = next;
            next = ctx->next;

            ngx_resolver_resolve_srv_names(ctx, rn);
        }

        ಪ್ರತ್ಯಾಗಮ;
    }

    rn->nsrvs = 0;

    ಯದಿ (cname) {

        /* CNAME only */

        ಯದಿ (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            ಗಚ್ಛ failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_queue_remove(&rn->queue);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.data;

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_ಸೇರಿಸು_head(&r->srv_expire_queue, &rn->queue);

        ngx_resolver_free(r, rn->query);
        rn->query = ರಿಕ್ತ;
#ಯದಿ (NGX_HAVE_INET6)
        rn->query6 = ರಿಕ್ತ;
#endif

        ctx = rn->waiting;
        rn->waiting = ರಿಕ್ತ;

        ಯದಿ (ctx) {

            ಯದಿ (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                ಕುರು {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    next = ctx->next;

                    ctx->handler(ctx);

                    ctx = next;
                } ಯಾವತ್ (ctx);

                ಪ್ರತ್ಯಾಗಮ;
            }

            ಪ್ರತಿ (next = ctx; next; next = next->next) {
                next->ಗಂಟು = ರಿಕ್ತ;
            }

            (ಶೂನ್ಯ) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        ಪ್ರತ್ಯಾಗಮ;
    }

    ngx_log_error(r->log_level, r->log, 0, "no SRV type in DNS response");

    ಪ್ರತ್ಯಾಗಮ;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->log, 0, err);

    ಪ್ರತ್ಯಾಗಮ;

failed:

    /* unlock name mutex */

    ಪ್ರತ್ಯಾಗಮ;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_resolver_resolve_srv_names(ngx_resolver_ctx_t *ctx, ngx_resolver_ಗಂಟು_t *rn)
{
    ngx_uint_t                i;
    ngx_resolver_t           *r;
    ngx_resolver_ctx_t       *cctx;
    ngx_resolver_srv_name_t  *srvs;

    r = ctx->resolver;

    ctx->ಗಂಟು = ರಿಕ್ತ;
    ctx->state = NGX_OK;
    ctx->valid = rn->valid;
    ctx->ಎಣಿಕೆ = rn->nsrvs;

    srvs = ngx_resolver_calloc(r, rn->nsrvs * ಪರಿಮಾಣ(ngx_resolver_srv_name_t));
    ಯದಿ (srvs == ರಿಕ್ತ) {
        ಗಚ್ಛ failed;
    }

    ctx->srvs = srvs;
    ctx->nsrvs = rn->nsrvs;

    ಯದಿ (ctx->event && ctx->event->timer_set) {
        ngx_del_timer(ctx->event);
    }

    ಪ್ರತಿ (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
        srvs[i].name.data = ngx_resolver_alloc(r, rn->u.srvs[i].name.len);
        ಯದಿ (srvs[i].name.data == ರಿಕ್ತ) {
            ಗಚ್ಛ failed;
        }

        srvs[i].name.len = rn->u.srvs[i].name.len;
        ngx_memcpy(srvs[i].name.data, rn->u.srvs[i].name.data,
                   srvs[i].name.len);

        cctx = ngx_resolve_start(r, ರಿಕ್ತ);
        ಯದಿ (cctx == ರಿಕ್ತ) {
            ಗಚ್ಛ failed;
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

        ಯದಿ (ngx_resolve_name(cctx) == NGX_ERROR) {
            srvs[i].ctx = ರಿಕ್ತ;
            ಗಚ್ಛ failed;
        }
    }

    ಪ್ರತ್ಯಾಗಮ;

failed:

    ctx->state = NGX_ERROR;
    ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

    ctx->handler(ctx);
}


ಸ್ಥಿರ ಶೂನ್ಯ
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

    ctx->ಎಣಿಕೆ--;
    ctx->async |= cctx->async;

    srv->ctx = ರಿಕ್ತ;
    srv->state = cctx->state;

    ಯದಿ (cctx->naddrs) {

        ctx->valid = ngx_min(ctx->valid, cctx->valid);

        addrs = ngx_resolver_calloc(r, cctx->naddrs * ಪರಿಮಾಣ(ngx_addr_t));
        ಯದಿ (addrs == ರಿಕ್ತ) {
            srv->state = NGX_ERROR;
            ಗಚ್ಛ done;
        }

        sockaddr = ngx_resolver_alloc(r, cctx->naddrs * ಪರಿಮಾಣ(ngx_sockaddr_t));
        ಯದಿ (sockaddr == ರಿಕ್ತ) {
            ngx_resolver_free(r, addrs);
            srv->state = NGX_ERROR;
            ಗಚ್ಛ done;
        }

        ಪ್ರತಿ (i = 0; i < cctx->naddrs; i++) {
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

    ಯದಿ (ctx->ಎಣಿಕೆ == 0) {
        ngx_resolver_report_srv(r, ctx);
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan)
{
    ವರ್ಣ                 *err;
    size_t                len;
    in_addr_t             addr;
    int32_t               ttl;
    ngx_int_t             octet;
    ngx_str_t             name;
    ngx_uint_t            mask, type, class, qident, a, i, start;
    ngx_queue_t          *expire_queue;
    ngx_rbtree_t         *ವೃಕ್ಷ;
    ngx_resolver_an_t    *an;
    ngx_resolver_ctx_t   *ctx, *next;
    ngx_resolver_ಗಂಟು_t  *rn;
#ಯದಿ (NGX_HAVE_INET6)
    uint32_t              hash;
    ngx_int_t             digit;
    ಸಂರಚನಾ in6_addr       addr6;
#endif

    ಯದಿ (ngx_resolver_copy(r, &name, buf,
                          buf + ಪರಿಮಾಣ(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        ಪ್ರತ್ಯಾಗಮ;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver qs:%V", &name);

    /* AF_INET */

    addr = 0;
    i = ಪರಿಮಾಣ(ngx_resolver_hdr_t);

    ಪ್ರತಿ (mask = 0; mask < 32; mask += 8) {
        len = buf[i++];

        octet = ngx_atoi(&buf[i], len);
        ಯದಿ (octet == NGX_ERROR || octet > 255) {
            ಗಚ್ಛ invalid_in_addr_arpa;
        }

        addr += octet << mask;
        i += len;
    }

    ಯದಿ (ngx_strcasecmp(&buf[i], (u_char *) "\7in-addr\4arpa") == 0) {
        i += ಪರಿಮಾಣ("\7in-addr\4arpa");

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        ವೃಕ್ಷ = &r->addr_rbtree;
        expire_queue = &r->addr_expire_queue;

        ಗಚ್ಛ valid;
    }

invalid_in_addr_arpa:

#ಯದಿ (NGX_HAVE_INET6)

    i = ಪರಿಮಾಣ(ngx_resolver_hdr_t);

    ಪ್ರತಿ (octet = 15; octet >= 0; octet--) {
        ಯದಿ (buf[i++] != '\1') {
            ಗಚ್ಛ invalid_ip6_arpa;
        }

        digit = ngx_hextoi(&buf[i++], 1);
        ಯದಿ (digit == NGX_ERROR) {
            ಗಚ್ಛ invalid_ip6_arpa;
        }

        addr6.s6_addr[octet] = (u_char) digit;

        ಯದಿ (buf[i++] != '\1') {
            ಗಚ್ಛ invalid_ip6_arpa;
        }

        digit = ngx_hextoi(&buf[i++], 1);
        ಯದಿ (digit == NGX_ERROR) {
            ಗಚ್ಛ invalid_ip6_arpa;
        }

        addr6.s6_addr[octet] += (u_char) (digit * 16);
    }

    ಯದಿ (ngx_strcasecmp(&buf[i], (u_char *) "\3ip6\4arpa") == 0) {
        i += ಪರಿಮಾಣ("\3ip6\4arpa");

        /* lock addr mutex */

        hash = ngx_crc32_short(addr6.s6_addr, 16);
        rn = ngx_resolver_lookup_addr6(r, &addr6, hash);

        ವೃಕ್ಷ = &r->addr6_rbtree;
        expire_queue = &r->addr6_expire_queue;

        ಗಚ್ಛ valid;
    }

invalid_ip6_arpa:
#endif

    ngx_log_error(r->log_level, r->log, 0,
                  "invalid in-addr.arpa or ip6.arpa name in DNS response");
    ngx_resolver_free(r, name.data);
    ಪ್ರತ್ಯಾಗಮ;

valid:

    ಯದಿ (rn == ರಿಕ್ತ || rn->query == ರಿಕ್ತ) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.data);
        ಗಚ್ಛ failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    ಯದಿ (ident != qident) {
        ngx_log_error(r->log_level, r->log, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.data);
        ಗಚ್ಛ failed;
    }

    ngx_resolver_free(r, name.data);

    ಯದಿ (code == 0 && nan == 0) {
        code = NGX_RESOLVE_NXDOMAIN;
    }

    ಯದಿ (code) {
        next = rn->waiting;
        rn->waiting = ರಿಕ್ತ;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(ವೃಕ್ಷ, &rn->ಗಂಟು);

        /* unlock addr mutex */

        ಯಾವತ್ (next) {
            ctx = next;
            ctx->state = code;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            next = ctx->next;

            ctx->handler(ctx);
        }

        ngx_resolver_free_ಗಂಟು(r, rn);

        ಪ್ರತ್ಯಾಗಮ;
    }

    i += ಪರಿಮಾಣ(ngx_resolver_qs_t);

    ಪ್ರತಿ (a = 0; a < nan; a++) {

        start = i;

        ಯಾವತ್ (i < n) {

            ಯದಿ (buf[i] & 0xc0) {
                i += 2;
                ಗಚ್ಛ found;
            }

            ಯದಿ (buf[i] == 0) {
                i++;
                ಗಚ್ಛ test_length;
            }

            i += 1 + buf[i];
        }

        ಗಚ್ಛ short_response;

    test_length:

        ಯದಿ (i - start < 2) {
            err = "invalid name in DNS response";
            ಗಚ್ಛ invalid;
        }

    found:

        ಯದಿ (i + ಪರಿಮಾಣ(ngx_resolver_an_t) >= n) {
            ಗಚ್ಛ short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        ಯದಿ (class != 1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR class %ui in DNS response", class);
            ಗಚ್ಛ failed;
        }

        ಯದಿ (ttl < 0) {
            ttl = 0;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                      "resolver qt:%ui cl:%ui len:%uz",
                      type, class, len);

        i += ಪರಿಮಾಣ(ngx_resolver_an_t);

        ವಿಕಲ್ಪ (type) {

        ಪಕ್ಷ NGX_RESOLVE_PTR:

            ಗಚ್ಛ ptr;

        ಪಕ್ಷ NGX_RESOLVE_CNAME:

            ಭಙ್ಗ;

        ಸ್ವಭಾವ:

            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->log, 0,
                  "no PTR type in DNS response");
    ಪ್ರತ್ಯಾಗಮ;

ptr:

    ಯದಿ (ngx_resolver_copy(r, &name, buf, buf + i, buf + n) != NGX_OK) {
        ಗಚ್ಛ failed;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver an:%V", &name);

    ಯದಿ (name.len != (size_t) rn->nlen
        || ngx_strncmp(name.data, rn->name, name.len) != 0)
    {
        ಯದಿ (rn->nlen) {
            ngx_resolver_free(r, rn->name);
        }

        rn->nlen = (u_short) name.len;
        rn->name = name.data;

        name.data = ngx_resolver_dup(r, rn->name, name.len);
        ಯದಿ (name.data == ರಿಕ್ತ) {
            ಗಚ್ಛ failed;
        }
    }

    ngx_queue_remove(&rn->queue);

    rn->valid = ngx_time() + (r->valid ? r->valid : ttl);
    rn->expire = ngx_time() + r->expire;

    ngx_queue_ಸೇರಿಸು_head(expire_queue, &rn->queue);

    next = rn->waiting;
    rn->waiting = ರಿಕ್ತ;

    /* unlock addr mutex */

    ಯಾವತ್ (next) {
        ctx = next;
        ctx->state = NGX_OK;
        ctx->valid = rn->valid;
        ctx->name = name;
        next = ctx->next;

        ctx->handler(ctx);
    }

    ngx_resolver_free(r, name.data);

    ಪ್ರತ್ಯಾಗಮ;

short_response:

    err = "short DNS response";

invalid:

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->log, 0, err);

    ಪ್ರತ್ಯಾಗಮ;

failed:

    /* unlock addr mutex */

    ಪ್ರತ್ಯಾಗಮ;
}


ಸ್ಥಿರ ngx_resolver_ಗಂಟು_t *
ngx_resolver_lookup_name(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_ಗಂಟು_t    *ಗಂಟು, *sentinel;
    ngx_resolver_ಗಂಟು_t  *rn;

    ಗಂಟು = r->name_rbtree.ಬೇರು;
    sentinel = r->name_rbtree.sentinel;

    ಯಾವತ್ (ಗಂಟು != sentinel) {

        ಯದಿ (hash < ಗಂಟು->ಕೀಲಿ) {
            ಗಂಟು = ಗಂಟು->ಎಡ;
            ಅನುವರ್ತ;
        }

        ಯದಿ (hash > ಗಂಟು->ಕೀಲಿ) {
            ಗಂಟು = ಗಂಟು->ಬಲ;
            ಅನುವರ್ತ;
        }

        /* hash == node->key */

        rn = ngx_resolver_ಗಂಟು(ಗಂಟು);

        rc = ngx_memn2cmp(name->data, rn->name, name->len, rn->nlen);

        ಯದಿ (rc == 0) {
            ಪ್ರತ್ಯಾಗಮ rn;
        }

        ಗಂಟು = (rc < 0) ? ಗಂಟು->ಎಡ : ಗಂಟು->ಬಲ;
    }

    /* not found */

    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}


ಸ್ಥಿರ ngx_resolver_ಗಂಟು_t *
ngx_resolver_lookup_srv(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_ಗಂಟು_t    *ಗಂಟು, *sentinel;
    ngx_resolver_ಗಂಟು_t  *rn;

    ಗಂಟು = r->srv_rbtree.ಬೇರು;
    sentinel = r->srv_rbtree.sentinel;

    ಯಾವತ್ (ಗಂಟು != sentinel) {

        ಯದಿ (hash < ಗಂಟು->ಕೀಲಿ) {
            ಗಂಟು = ಗಂಟು->ಎಡ;
            ಅನುವರ್ತ;
        }

        ಯದಿ (hash > ಗಂಟು->ಕೀಲಿ) {
            ಗಂಟು = ಗಂಟು->ಬಲ;
            ಅನುವರ್ತ;
        }

        /* hash == node->key */

        rn = ngx_resolver_ಗಂಟು(ಗಂಟು);

        rc = ngx_memn2cmp(name->data, rn->name, name->len, rn->nlen);

        ಯದಿ (rc == 0) {
            ಪ್ರತ್ಯಾಗಮ rn;
        }

        ಗಂಟು = (rc < 0) ? ಗಂಟು->ಎಡ : ಗಂಟು->ಬಲ;
    }

    /* not found */

    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}


ಸ್ಥಿರ ngx_resolver_ಗಂಟು_t *
ngx_resolver_lookup_addr(ngx_resolver_t *r, in_addr_t addr)
{
    ngx_rbtree_ಗಂಟು_t  *ಗಂಟು, *sentinel;

    ಗಂಟು = r->addr_rbtree.ಬೇರು;
    sentinel = r->addr_rbtree.sentinel;

    ಯಾವತ್ (ಗಂಟು != sentinel) {

        ಯದಿ (addr < ಗಂಟು->ಕೀಲಿ) {
            ಗಂಟು = ಗಂಟು->ಎಡ;
            ಅನುವರ್ತ;
        }

        ಯದಿ (addr > ಗಂಟು->ಕೀಲಿ) {
            ಗಂಟು = ಗಂಟು->ಬಲ;
            ಅನುವರ್ತ;
        }

        /* addr == node->key */

        ಪ್ರತ್ಯಾಗಮ ngx_resolver_ಗಂಟು(ಗಂಟು);
    }

    /* not found */

    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}


#ಯದಿ (NGX_HAVE_INET6)

ಸ್ಥಿರ ngx_resolver_ಗಂಟು_t *
ngx_resolver_lookup_addr6(ngx_resolver_t *r, ಸಂರಚನಾ in6_addr *addr,
    uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_ಗಂಟು_t    *ಗಂಟು, *sentinel;
    ngx_resolver_ಗಂಟು_t  *rn;

    ಗಂಟು = r->addr6_rbtree.ಬೇರು;
    sentinel = r->addr6_rbtree.sentinel;

    ಯಾವತ್ (ಗಂಟು != sentinel) {

        ಯದಿ (hash < ಗಂಟು->ಕೀಲಿ) {
            ಗಂಟು = ಗಂಟು->ಎಡ;
            ಅನುವರ್ತ;
        }

        ಯದಿ (hash > ಗಂಟು->ಕೀಲಿ) {
            ಗಂಟು = ಗಂಟು->ಬಲ;
            ಅನುವರ್ತ;
        }

        /* hash == node->key */

        rn = ngx_resolver_ಗಂಟು(ಗಂಟು);

        rc = ngx_memcmp(addr, &rn->addr6, 16);

        ಯದಿ (rc == 0) {
            ಪ್ರತ್ಯಾಗಮ rn;
        }

        ಗಂಟು = (rc < 0) ? ಗಂಟು->ಎಡ : ಗಂಟು->ಬಲ;
    }

    /* not found */

    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

#endif


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_resolver_rbtree_ಸೇರಿಸು_ಮೌಲ್ಯ(ngx_rbtree_ಗಂಟು_t *temp,
    ngx_rbtree_ಗಂಟು_t *ಗಂಟು, ngx_rbtree_ಗಂಟು_t *sentinel)
{
    ngx_rbtree_ಗಂಟು_t    **p;
    ngx_resolver_ಗಂಟು_t   *rn, *rn_temp;

    ಪ್ರತಿ ( ;; ) {

        ಯದಿ (ಗಂಟು->ಕೀಲಿ < temp->ಕೀಲಿ) {

            p = &temp->ಎಡ;

        } ಅನ್ಯಥಾ ಯದಿ (ಗಂಟು->ಕೀಲಿ > temp->ಕೀಲಿ) {

            p = &temp->ಬಲ;

        } ಅನ್ಯಥಾ { /* node->key == temp->key */

            rn = ngx_resolver_ಗಂಟು(ಗಂಟು);
            rn_temp = ngx_resolver_ಗಂಟು(temp);

            p = (ngx_memn2cmp(rn->name, rn_temp->name, rn->nlen, rn_temp->nlen)
                 < 0) ? &temp->ಎಡ : &temp->ಬಲ;
        }

        ಯದಿ (*p == sentinel) {
            ಭಙ್ಗ;
        }

        temp = *p;
    }

    *p = ಗಂಟು;
    ಗಂಟು->parent = temp;
    ಗಂಟು->ಎಡ = sentinel;
    ಗಂಟು->ಬಲ = sentinel;
    ngx_rbt_red(ಗಂಟು);
}


#ಯದಿ (NGX_HAVE_INET6)

ಸ್ಥಿರ ಶೂನ್ಯ
ngx_resolver_rbtree_ಸೇರಿಸು_addr6_ಮೌಲ್ಯ(ngx_rbtree_ಗಂಟು_t *temp,
    ngx_rbtree_ಗಂಟು_t *ಗಂಟು, ngx_rbtree_ಗಂಟು_t *sentinel)
{
    ngx_rbtree_ಗಂಟು_t    **p;
    ngx_resolver_ಗಂಟು_t   *rn, *rn_temp;

    ಪ್ರತಿ ( ;; ) {

        ಯದಿ (ಗಂಟು->ಕೀಲಿ < temp->ಕೀಲಿ) {

            p = &temp->ಎಡ;

        } ಅನ್ಯಥಾ ಯದಿ (ಗಂಟು->ಕೀಲಿ > temp->ಕೀಲಿ) {

            p = &temp->ಬಲ;

        } ಅನ್ಯಥಾ { /* node->key == temp->key */

            rn = ngx_resolver_ಗಂಟು(ಗಂಟು);
            rn_temp = ngx_resolver_ಗಂಟು(temp);

            p = (ngx_memcmp(&rn->addr6, &rn_temp->addr6, 16)
                 < 0) ? &temp->ಎಡ : &temp->ಬಲ;
        }

        ಯದಿ (*p == sentinel) {
            ಭಙ್ಗ;
        }

        temp = *p;
    }

    *p = ಗಂಟು;
    ಗಂಟು->parent = temp;
    ಗಂಟು->ಎಡ = sentinel;
    ಗಂಟು->ಬಲ = sentinel;
    ngx_rbt_red(ಗಂಟು);
}

#endif


ಸ್ಥಿರ ngx_int_t
ngx_resolver_create_name_query(ngx_resolver_t *r, ngx_resolver_ಗಂಟು_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    size_t               len, nlen;
    ngx_uint_t           ident;
    ngx_resolver_qs_t   *qs;
    ngx_resolver_hdr_t  *query;

    nlen = name->len ? (1 + name->len + 1) : 1;

    len = ಪರಿಮಾಣ(ngx_resolver_hdr_t) + nlen + ಪರಿಮಾಣ(ngx_resolver_qs_t);

#ಯದಿ (NGX_HAVE_INET6)
    p = ngx_resolver_alloc(r, len * (r->ipv4 + r->ipv6));
#ಅನ್ಯಥಾ
    p = ngx_resolver_alloc(r, len);
#endif
    ಯದಿ (p == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    rn->qlen = (u_short) len;
    rn->query = p;

#ಯದಿ (NGX_HAVE_INET6)
    ಯದಿ (r->ipv6) {
        rn->query6 = r->ipv4 ? (p + len) : p;
    }
#endif

    query = (ngx_resolver_hdr_t *) p;

    ಯದಿ (r->ipv4) {
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

    p += ಪರಿಮಾಣ(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    /* query type */
    qs->type_hi = 0; qs->type_lo = NGX_RESOLVE_A;

    /* IN query class */
    qs->class_hi = 0; qs->class_lo = 1;

    /* convert "www.example.com" to "\3www\7example\3com\0" */

    len = 0;
    p--;
    *p-- = '\0';

    ಯದಿ (name->len == 0)  {
        ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
    }

    ಪ್ರತಿ (s = name->data + name->len - 1; s >= name->data; s--) {
        ಯದಿ (*s != '.') {
            *p = *s;
            len++;

        } ಅನ್ಯಥಾ {
            ಯದಿ (len == 0 || len > 255) {
                ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
            }

            *p = (u_char) len;
            len = 0;
        }

        p--;
    }

    ಯದಿ (len == 0 || len > 255) {
        ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
    }

    *p = (u_char) len;

#ಯದಿ (NGX_HAVE_INET6)
    ಯದಿ (!r->ipv6) {
        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    p = rn->query6;

    ಯದಿ (r->ipv4) {
        ngx_memcpy(p, rn->query, rn->qlen);
    }

    query = (ngx_resolver_hdr_t *) p;

    ident = ngx_random();

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve: \"%V\" AAAA %i", name, ident & 0xffff);

    query->ident_hi = (u_char) ((ident >> 8) & 0xff);
    query->ident_lo = (u_char) (ident & 0xff);

    p += ಪರಿಮಾಣ(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    qs->type_lo = NGX_RESOLVE_AAAA;
#endif

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ಸ್ಥಿರ ngx_int_t
ngx_resolver_create_srv_query(ngx_resolver_t *r, ngx_resolver_ಗಂಟು_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    size_t               len, nlen;
    ngx_uint_t           ident;
    ngx_resolver_qs_t   *qs;
    ngx_resolver_hdr_t  *query;

    nlen = name->len ? (1 + name->len + 1) : 1;

    len = ಪರಿಮಾಣ(ngx_resolver_hdr_t) + nlen + ಪರಿಮಾಣ(ngx_resolver_qs_t);

    p = ngx_resolver_alloc(r, len);
    ಯದಿ (p == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
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

    p += ಪರಿಮಾಣ(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    /* query type */
    qs->type_hi = 0; qs->type_lo = NGX_RESOLVE_SRV;

    /* IN query class */
    qs->class_hi = 0; qs->class_lo = 1;

    /* converts "www.example.com" to "\3www\7example\3com\0" */

    len = 0;
    p--;
    *p-- = '\0';

    ಯದಿ (name->len == 0)  {
        ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
    }

    ಪ್ರತಿ (s = name->data + name->len - 1; s >= name->data; s--) {
        ಯದಿ (*s != '.') {
            *p = *s;
            len++;

        } ಅನ್ಯಥಾ {
            ಯದಿ (len == 0 || len > 255) {
                ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
            }

            *p = (u_char) len;
            len = 0;
        }

        p--;
    }

    ಯದಿ (len == 0 || len > 255) {
        ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
    }

    *p = (u_char) len;

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ಸ್ಥಿರ ngx_int_t
ngx_resolver_create_addr_query(ngx_resolver_t *r, ngx_resolver_ಗಂಟು_t *rn,
    ngx_resolver_addr_t *addr)
{
    u_char               *p, *d;
    size_t                len;
    in_addr_t             inaddr;
    ngx_int_t             n;
    ngx_uint_t            ident;
    ngx_resolver_hdr_t   *query;
    ಸಂರಚನಾ sockaddr_in   *sin;
#ಯದಿ (NGX_HAVE_INET6)
    ಸಂರಚನಾ sockaddr_in6  *sin6;
#endif

    ವಿಕಲ್ಪ (addr->sockaddr->sa_family) {

#ಯದಿ (NGX_HAVE_INET6)
    ಪಕ್ಷ AF_INET6:
        len = ಪರಿಮಾಣ(ngx_resolver_hdr_t)
              + 64 + ಪರಿಮಾಣ(".ip6.arpa.") - 1
              + ಪರಿಮಾಣ(ngx_resolver_qs_t);

        ಭಙ್ಗ;
#endif

    ಸ್ವಭಾವ: /* AF_INET */
        len = ಪರಿಮಾಣ(ngx_resolver_hdr_t)
              + ಪರಿಮಾಣ(".255.255.255.255.in-addr.arpa.") - 1
              + ಪರಿಮಾಣ(ngx_resolver_qs_t);
    }

    p = ngx_resolver_alloc(r, len);
    ಯದಿ (p == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
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

    p += ಪರಿಮಾಣ(ngx_resolver_hdr_t);

    ವಿಕಲ್ಪ (addr->sockaddr->sa_family) {

#ಯದಿ (NGX_HAVE_INET6)
    ಪಕ್ಷ AF_INET6:
        sin6 = (ಸಂರಚನಾ sockaddr_in6 *) addr->sockaddr;

        ಪ್ರತಿ (n = 15; n >= 0; n--) {
            p = ngx_sprintf(p, "\1%xd\1%xd",
                            sin6->sin6_addr.s6_addr[n] & 0xf,
                            (sin6->sin6_addr.s6_addr[n] >> 4) & 0xf);
        }

        p = ngx_cpymem(p, "\3ip6\4arpa\0", 10);

        ಭಙ್ಗ;
#endif

    ಸ್ವಭಾವ: /* AF_INET */

        sin = (ಸಂರಚನಾ sockaddr_in *) addr->sockaddr;
        inaddr = ntohl(sin->sin_addr.s_addr);

        ಪ್ರತಿ (n = 0; n < 32; n += 8) {
            d = ngx_sprintf(&p[1], "%ud", (inaddr >> n) & 0xff);
            *p = (u_char) (d - &p[1]);
            p = d;
        }

        p = ngx_cpymem(p, "\7in-addr\4arpa\0", 14);
    }

    /* query type "PTR", IN query class */
    p = ngx_cpymem(p, "\0\14\0\1", 4);

    rn->qlen = (u_short) (p - rn->query);

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ಸ್ಥಿರ ngx_int_t
ngx_resolver_copy(ngx_resolver_t *r, ngx_str_t *name, u_char *buf, u_char *src,
    u_char *last)
{
    ವರ್ಣ        *err;
    u_char      *p, *dst;
    size_t       len;
    ngx_uint_t   i, n;

    p = src;
    len = 0;

    /*
     * compression pointers allow to create endless loop, so we set limit;
     * 128 pointers should be enough to store 255-byte name
     */

    ಪ್ರತಿ (i = 0; i < 128; i++) {
        n = *p++;

        ಯದಿ (n == 0) {
            ಗಚ್ಛ done;
        }

        ಯದಿ (n & 0xc0) {
            ಯದಿ ((n & 0xc0) != 0xc0) {
                err = "invalid label type in DNS response";
                ಗಚ್ಛ invalid;
            }

            ಯದಿ (p >= last) {
                err = "name is out of DNS response";
                ಗಚ್ಛ invalid;
            }

            n = ((n & 0x3f) << 8) + *p;
            p = &buf[n];

        } ಅನ್ಯಥಾ {
            len += 1 + n;
            p = &p[n];
        }

        ಯದಿ (p >= last) {
            err = "name is out of DNS response";
            ಗಚ್ಛ invalid;
        }
    }

    err = "compression pointers loop in DNS response";

invalid:

    ngx_log_error(r->log_level, r->log, 0, err);

    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;

done:

    ಯದಿ (name == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    ಯದಿ (len == 0) {
        ngx_str_null(name);
        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    dst = ngx_resolver_alloc(r, len);
    ಯದಿ (dst == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    name->data = dst;

    ಪ್ರತಿ ( ;; ) {
        n = *src++;

        ಯದಿ (n == 0) {
            name->len = dst - name->data - 1;
            ಪ್ರತ್ಯಾಗಮ NGX_OK;
        }

        ಯದಿ (n & 0xc0) {
            n = ((n & 0x3f) << 8) + *src;
            src = &buf[n];

        } ಅನ್ಯಥಾ {
            ngx_strlow(dst, src, n);
            dst += n;
            src += n;
            *dst++ = '.';
        }
    }
}


ಸ್ಥಿರ ngx_int_t
ngx_resolver_set_timeout(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx)
{
    ಯದಿ (ctx->event || ctx->timeout == 0) {
        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    ctx->event = ngx_resolver_calloc(r, ಪರಿಮಾಣ(ngx_event_t));
    ಯದಿ (ctx->event == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ctx->event->handler = ngx_resolver_timeout_handler;
    ctx->event->data = ctx;
    ctx->event->log = r->log;
    ctx->event->cancelable = ctx->cancelable;
    ctx->ident = -1;

    ngx_add_timer(ctx->event, ctx->timeout);

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_resolver_timeout_handler(ngx_event_t *ev)
{
    ngx_resolver_ctx_t  *ctx;

    ctx = ev->data;

    ctx->state = NGX_RESOLVE_TIMEDOUT;

    ctx->handler(ctx);
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_resolver_free_ಗಂಟು(ngx_resolver_t *r, ngx_resolver_ಗಂಟು_t *rn)
{
    ngx_uint_t  i;

    /* lock alloc mutex */

    ಯದಿ (rn->query) {
        ngx_resolver_free_locked(r, rn->query);
    }

    ಯದಿ (rn->name) {
        ngx_resolver_free_locked(r, rn->name);
    }

    ಯದಿ (rn->cnlen) {
        ngx_resolver_free_locked(r, rn->u.cname);
    }

    ಯದಿ (rn->naddrs > 1 && rn->naddrs != (u_short) -1) {
        ngx_resolver_free_locked(r, rn->u.addrs);
    }

#ಯದಿ (NGX_HAVE_INET6)
    ಯದಿ (rn->naddrs6 > 1 && rn->naddrs6 != (u_short) -1) {
        ngx_resolver_free_locked(r, rn->u6.addrs6);
    }
#endif

    ಯದಿ (rn->nsrvs) {
        ಪ್ರತಿ (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
            ಯದಿ (rn->u.srvs[i].name.data) {
                ngx_resolver_free_locked(r, rn->u.srvs[i].name.data);
            }
        }

        ngx_resolver_free_locked(r, rn->u.srvs);
    }

    ngx_resolver_free_locked(r, rn);

    /* unlock alloc mutex */
}


ಸ್ಥಿರ ಶೂನ್ಯ *
ngx_resolver_alloc(ngx_resolver_t *r, size_t size)
{
    u_char  *p;

    /* lock alloc mutex */

    p = ngx_alloc(size, r->log);

    /* unlock alloc mutex */

    ಪ್ರತ್ಯಾಗಮ p;
}


ಸ್ಥಿರ ಶೂನ್ಯ *
ngx_resolver_calloc(ngx_resolver_t *r, size_t size)
{
    u_char  *p;

    p = ngx_resolver_alloc(r, size);

    ಯದಿ (p) {
        ngx_memzero(p, size);
    }

    ಪ್ರತ್ಯಾಗಮ p;
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
ngx_resolver_dup(ngx_resolver_t *r, ಶೂನ್ಯ *src, size_t size)
{
    ಶೂನ್ಯ  *dst;

    dst = ngx_resolver_alloc(r, size);

    ಯದಿ (dst == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ dst;
    }

    ngx_memcpy(dst, src, size);

    ಪ್ರತ್ಯಾಗಮ dst;
}


ಸ್ಥಿರ ngx_resolver_addr_t *
ngx_resolver_export(ngx_resolver_t *r, ngx_resolver_ಗಂಟು_t *rn,
    ngx_uint_t rotate)
{
    ngx_uint_t            d, i, j, n;
    in_addr_t            *addr;
    ngx_sockaddr_t       *sockaddr;
    ಸಂರಚನಾ sockaddr_in   *sin;
    ngx_resolver_addr_t  *dst;
#ಯದಿ (NGX_HAVE_INET6)
    ಸಂರಚನಾ in6_addr      *addr6;
    ಸಂರಚನಾ sockaddr_in6  *sin6;
#endif

    n = rn->naddrs;
#ಯದಿ (NGX_HAVE_INET6)
    n += rn->naddrs6;
#endif

    dst = ngx_resolver_calloc(r, n * ಪರಿಮಾಣ(ngx_resolver_addr_t));
    ಯದಿ (dst == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    sockaddr = ngx_resolver_calloc(r, n * ಪರಿಮಾಣ(ngx_sockaddr_t));
    ಯದಿ (sockaddr == ರಿಕ್ತ) {
        ngx_resolver_free(r, dst);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    i = 0;
    d = rotate ? ngx_random() % n : 0;

    ಯದಿ (rn->naddrs) {
        j = rotate ? ngx_random() % rn->naddrs : 0;

        addr = (rn->naddrs == 1) ? &rn->u.addr : rn->u.addrs;

        ಕುರು {
            sin = &sockaddr[d].sockaddr_in;
            sin->sin_family = AF_INET;
            sin->sin_addr.s_addr = addr[j++];
            dst[d].sockaddr = (ಸಂರಚನಾ sockaddr *) sin;
            dst[d++].socklen = ಪರಿಮಾಣ(ಸಂರಚನಾ sockaddr_in);

            ಯದಿ (d == n) {
                d = 0;
            }

            ಯದಿ (j == (ngx_uint_t) rn->naddrs) {
                j = 0;
            }
        } ಯಾವತ್ (++i < (ngx_uint_t) rn->naddrs);
    }

#ಯದಿ (NGX_HAVE_INET6)
    ಯದಿ (rn->naddrs6) {
        j = rotate ? ngx_random() % rn->naddrs6 : 0;

        addr6 = (rn->naddrs6 == 1) ? &rn->u6.addr6 : rn->u6.addrs6;

        ಕುರು {
            sin6 = &sockaddr[d].sockaddr_in6;
            sin6->sin6_family = AF_INET6;
            ngx_memcpy(sin6->sin6_addr.s6_addr, addr6[j++].s6_addr, 16);
            dst[d].sockaddr = (ಸಂರಚನಾ sockaddr *) sin6;
            dst[d++].socklen = ಪರಿಮಾಣ(ಸಂರಚನಾ sockaddr_in6);

            ಯದಿ (d == n) {
                d = 0;
            }

            ಯದಿ (j == rn->naddrs6) {
                j = 0;
            }
        } ಯಾವತ್ (++i < n);
    }
#endif

    ಪ್ರತ್ಯಾಗಮ dst;
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

    ಪ್ರತಿ (i = 0; i < nsrvs; i++) {
        ಯದಿ (srvs[i].state == NGX_ERROR) {
            ctx->state = NGX_ERROR;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

            ctx->handler(ctx);
            ಪ್ರತ್ಯಾಗಮ;
        }

        naddrs += srvs[i].naddrs;
    }

    ಯದಿ (naddrs == 0) {
        ctx->state = srvs[0].state;

        ಪ್ರತಿ (i = 0; i < nsrvs; i++) {
            ಯದಿ (srvs[i].state == NGX_RESOLVE_NXDOMAIN) {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                ಭಙ್ಗ;
            }
        }

        ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        ಪ್ರತ್ಯಾಗಮ;
    }

    addrs = ngx_resolver_calloc(r, naddrs * ಪರಿಮಾಣ(ngx_resolver_addr_t));
    ಯದಿ (addrs == ರಿಕ್ತ) {
        ctx->state = NGX_ERROR;
        ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        ಪ್ರತ್ಯಾಗಮ;
    }

    i = 0;
    n = 0;

    ಕುರು {
        nw = 0;

        ಪ್ರತಿ (j = i; j < nsrvs; j++) {
            ಯದಿ (srvs[j].priority != srvs[i].priority) {
                ಭಙ್ಗ;
            }

            nw += srvs[j].naddrs * srvs[j].weight;
        }

        ಯದಿ (nw == 0) {
            ಗಚ್ಛ next_srv;
        }

        w = ngx_random() % nw;

        ಪ್ರತಿ (k = i; k < j; k++) {
            ಯದಿ (w < srvs[k].naddrs * srvs[k].weight) {
                ಭಙ್ಗ;
            }

            w -= srvs[k].naddrs * srvs[k].weight;
        }

        ಪ್ರತಿ (l = i; l < j; l++) {

            ಪ್ರತಿ (m = 0; m < srvs[k].naddrs; m++) {
                addrs[n].socklen = srvs[k].addrs[m].socklen;
                addrs[n].sockaddr = srvs[k].addrs[m].sockaddr;
                addrs[n].name = srvs[k].name;
                addrs[n].priority = srvs[k].priority;
                addrs[n].weight = srvs[k].weight;
                n++;
            }

            ಯದಿ (++k == j) {
                k = i;
            }
        }

next_srv:

        i = j;

    } ಯಾವತ್ (i < ctx->nsrvs);

    ctx->state = NGX_OK;
    ctx->addrs = addrs;
    ctx->naddrs = naddrs;

    ctx->handler(ctx);

    ngx_resolver_free(r, addrs);
}


ವರ್ಣ *
ngx_resolver_strerror(ngx_int_t err)
{
    ಸ್ಥಿರ ವರ್ಣ *errors[] = {
        "Format error",     /* FORMERR */
        "Server failure",   /* SERVFAIL */
        "Host not found",   /* NXDOMAIN */
        "Unimplemented",    /* NOTIMP */
        "Operation refused" /* REFUSED */
    };

    ಯದಿ (err > 0 && err < 6) {
        ಪ್ರತ್ಯಾಗಮ errors[err - 1];
    }

    ಯದಿ (err == NGX_RESOLVE_TIMEDOUT) {
        ಪ್ರತ್ಯಾಗಮ "Operation timed out";
    }

    ಪ್ರತ್ಯಾಗಮ "Unknown error";
}


ಸ್ಥಿರ u_char *
ngx_resolver_log_error(ngx_log_t *log, u_char *buf, size_t len)
{
    u_char                     *p;
    ngx_resolver_connection_t  *rec;

    p = buf;

    ಯದಿ (log->action) {
        p = ngx_snprintf(buf, len, " while %s", log->action);
        len -= p - buf;
    }

    rec = log->data;

    ಯದಿ (rec) {
        p = ngx_snprintf(p, len, ", resolver: %V", &rec->server);
    }

    ಪ್ರತ್ಯಾಗಮ p;
}


ಸ್ಥಿರ ngx_int_t
ngx_udp_connect(ngx_resolver_connection_t *rec)
{
    ಪೂರ್ಣಾಙ್ಕ                rc;
    ngx_int_t          event;
    ngx_event_t       *rev, *wev;
    ngx_socket_t       s;
    ngx_connection_t  *c;

    s = ngx_socket(rec->sockaddr->sa_family, SOCK_DGRAM, 0);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "UDP socket %d", s);

    ಯದಿ (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_socket_n " failed");
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->log);

    ಯದಿ (c == ರಿಕ್ತ) {
        ಯದಿ (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಯದಿ (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        ಗಚ್ಛ failed;
    }

    rev = c->ಪಠ;
    wev = c->ಲಿಖ;

    rev->log = &rec->log;
    wev->log = &rec->log;

    rec->udp = c;

    c->ಸಂಖ್ಯೆ = ngx_atomic_fetch_add(ngx_connection_counter, 1);

    c->start_time = ngx_current_msec;

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->log, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->ಸಂಖ್ಯೆ);

    rc = connect(s, rec->sockaddr, rec->socklen);

    ಯದಿ (rc == -1) {
        ngx_log_error(NGX_LOG_CRIT, &rec->log, ngx_socket_errno,
                      "connect() failed");

        ಗಚ್ಛ failed;
    }

    /* UDP sockets are always ready to write */
    wev->ready = 1;

    event = (ngx_event_flags & NGX_USE_CLEAR_EVENT) ?
                /* kqueue, epoll */                 NGX_CLEAR_EVENT:
                /* select, poll, /dev/poll */       NGX_LEVEL_EVENT;
                /* eventport event type has no meaning: oneshot only */

    ಯದಿ (ngx_add_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        ಗಚ್ಛ failed;
    }

    ಪ್ರತ್ಯಾಗಮ NGX_OK;

failed:

    ngx_close_connection(c);
    rec->udp = ರಿಕ್ತ;

    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
}


ಸ್ಥಿರ ngx_int_t
ngx_tcp_connect(ngx_resolver_connection_t *rec)
{
    ಪೂರ್ಣಾಙ್ಕ                rc;
    ngx_int_t          event;
    ngx_err_t          err;
    ngx_uint_t         level;
    ngx_socket_t       s;
    ngx_event_t       *rev, *wev;
    ngx_connection_t  *c;

    s = ngx_socket(rec->sockaddr->sa_family, SOCK_STREAM, 0);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "TCP socket %d", s);

    ಯದಿ (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_socket_n " failed");
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->log);

    ಯದಿ (c == ರಿಕ್ತ) {
        ಯದಿ (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಯದಿ (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        ಗಚ್ಛ failed;
    }

    rev = c->ಪಠ;
    wev = c->ಲಿಖ;

    rev->log = &rec->log;
    wev->log = &rec->log;

    rec->tcp = c;

    c->ಸಂಖ್ಯೆ = ngx_atomic_fetch_add(ngx_connection_counter, 1);

    c->start_time = ngx_current_msec;

    ಯದಿ (ngx_add_conn) {
        ಯದಿ (ngx_add_conn(c) == NGX_ERROR) {
            ಗಚ್ಛ failed;
        }
    }

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->log, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->ಸಂಖ್ಯೆ);

    rc = connect(s, rec->sockaddr, rec->socklen);

    ಯದಿ (rc == -1) {
        err = ngx_socket_errno;


        ಯದಿ (err != NGX_EINPROGRESS
#ಯದಿ (NGX_WIN32)
            /* Winsock returns WSAEWOULDBLOCK (NGX_EAGAIN) */
            && err != NGX_EAGAIN
#endif
            )
        {
            ಯದಿ (err == NGX_ECONNREFUSED
#ಯದಿ (NGX_LINUX)
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

            } ಅನ್ಯಥಾ {
                level = NGX_LOG_CRIT;
            }

            ngx_log_error(level, &rec->log, err, "connect() to %V failed",
                          &rec->server);

            ngx_close_connection(c);
            rec->tcp = ರಿಕ್ತ;

            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }
    }

    ಯದಿ (ngx_add_conn) {
        ಯದಿ (rc == -1) {

            /* NGX_EINPROGRESS */

            ಪ್ರತ್ಯಾಗಮ NGX_AGAIN;
        }

        ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "connected");

        wev->ready = 1;

        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    ಯದಿ (ngx_event_flags & NGX_USE_CLEAR_EVENT) {

        /* kqueue */

        event = NGX_CLEAR_EVENT;

    } ಅನ್ಯಥಾ {

        /* select, poll, /dev/poll */

        event = NGX_LEVEL_EVENT;
    }

    ಯದಿ (ngx_add_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        ಗಚ್ಛ failed;
    }

    ಯದಿ (rc == -1) {

        /* NGX_EINPROGRESS */

        ಯದಿ (ngx_add_event(wev, NGX_WRITE_EVENT, event) != NGX_OK) {
            ಗಚ್ಛ failed;
        }

        ಪ್ರತ್ಯಾಗಮ NGX_AGAIN;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "connected");

    wev->ready = 1;

    ಪ್ರತ್ಯಾಗಮ NGX_OK;

failed:

    ngx_close_connection(c);
    rec->tcp = ರಿಕ್ತ;

    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
}


ಸ್ಥಿರ ngx_int_t
ngx_resolver_cmp_srvs(ಧ್ರುವ ಶೂನ್ಯ *one, ಧ್ರುವ ಶೂನ್ಯ *two)
{
    ngx_int_t            p1, p2;
    ngx_resolver_srv_t  *first, *second;

    first = (ngx_resolver_srv_t *) one;
    second = (ngx_resolver_srv_t *) two;

    p1 = first->priority;
    p2 = second->priority;

    ಪ್ರತ್ಯಾಗಮ p1 - p2;
}
