
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>
#ଅନ୍ତର୍ଭାଵ <ngx_event.h>


#define NGX_RESOLVER_UDP_SIZE   4096

#define NGX_RESOLVER_TCP_RSIZE  (2 + 65535)
#define NGX_RESOLVER_TCP_WSIZE  8192


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
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


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    u_char  type_hi;
    u_char  type_lo;
    u_char  class_hi;
    u_char  class_lo;
} ngx_resolver_qs_t;


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    u_char  type_hi;
    u_char  type_lo;
    u_char  class_hi;
    u_char  class_lo;
    u_char  ttl[4];
    u_char  len_hi;
    u_char  len_lo;
} ngx_resolver_an_t;


#define ngx_resolver_ଗଣ୍ଠି(n)  ngx_rbtree_data(n, ngx_resolver_ଗଣ୍ଠି_t, ଗଣ୍ଠି)


ସ୍ଥିର ngx_int_t ngx_udp_connect(ngx_resolver_connection_t *rec);
ସ୍ଥିର ngx_int_t ngx_tcp_connect(ngx_resolver_connection_t *rec);


ସ୍ଥିର ଶୂନ୍ୟ ngx_resolver_cleanup(ଶୂନ୍ୟ *data);
ସ୍ଥିର ଶୂନ୍ୟ ngx_resolver_cleanup_ବୃକ୍ଷ(ngx_resolver_t *r, ngx_rbtree_t *ବୃକ୍ଷ);
ସ୍ଥିର ngx_int_t ngx_resolve_name_locked(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx, ngx_str_t *name);
ସ୍ଥିର ଶୂନ୍ୟ ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *ବୃକ୍ଷ,
    ngx_queue_t *queue);
ସ୍ଥିର ngx_int_t ngx_resolver_send_query(ngx_resolver_t *r,
    ngx_resolver_ଗଣ୍ଠି_t *rn);
ସ୍ଥିର ngx_int_t ngx_resolver_send_udp_query(ngx_resolver_t *r,
    ngx_resolver_connection_t *rec, u_char *query, u_short qlen);
ସ୍ଥିର ngx_int_t ngx_resolver_send_tcp_query(ngx_resolver_t *r,
    ngx_resolver_connection_t *rec, u_char *query, u_short qlen);
ସ୍ଥିର ngx_int_t ngx_resolver_create_name_query(ngx_resolver_t *r,
    ngx_resolver_ଗଣ୍ଠି_t *rn, ngx_str_t *name);
ସ୍ଥିର ngx_int_t ngx_resolver_create_srv_query(ngx_resolver_t *r,
    ngx_resolver_ଗଣ୍ଠି_t *rn, ngx_str_t *name);
ସ୍ଥିର ngx_int_t ngx_resolver_create_addr_query(ngx_resolver_t *r,
    ngx_resolver_ଗଣ୍ଠି_t *rn, ngx_resolver_addr_t *addr);
ସ୍ଥିର ଶୂନ୍ୟ ngx_resolver_resend_handler(ngx_event_t *ev);
ସ୍ଥିର time_t ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *ବୃକ୍ଷ,
    ngx_queue_t *queue);
ସ୍ଥିର ngx_uint_t ngx_resolver_resend_empty(ngx_resolver_t *r);
ସ୍ଥିର ଶୂନ୍ୟ ngx_resolver_udp_read(ngx_event_t *rev);
ସ୍ଥିର ଶୂନ୍ୟ ngx_resolver_tcp_write(ngx_event_t *wev);
ସ୍ଥିର ଶୂନ୍ୟ ngx_resolver_tcp_read(ngx_event_t *rev);
ସ୍ଥିର ଶୂନ୍ୟ ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf,
    size_t n, ngx_uint_t tcp);
ସ୍ଥିର ଶୂନ୍ୟ ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans);
ସ୍ଥିର ଶୂନ୍ୟ ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans);
ସ୍ଥିର ଶୂନ୍ୟ ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan);
ସ୍ଥିର ngx_resolver_ଗଣ୍ଠି_t *ngx_resolver_lookup_name(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
ସ୍ଥିର ngx_resolver_ଗଣ୍ଠି_t *ngx_resolver_lookup_srv(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
ସ୍ଥିର ngx_resolver_ଗଣ୍ଠି_t *ngx_resolver_lookup_addr(ngx_resolver_t *r,
    in_addr_t addr);
ସ୍ଥିର ଶୂନ୍ୟ ngx_resolver_rbtree_ଭର୍ତ୍ତି_ମୂଲ୍ୟ(ngx_rbtree_ଗଣ୍ଠି_t *temp,
    ngx_rbtree_ଗଣ୍ଠି_t *ଗଣ୍ଠି, ngx_rbtree_ଗଣ୍ଠି_t *sentinel);
ସ୍ଥିର ngx_int_t ngx_resolver_copy(ngx_resolver_t *r, ngx_str_t *name,
    u_char *buf, u_char *src, u_char *last);
ସ୍ଥିର ngx_int_t ngx_resolver_set_timeout(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx);
ସ୍ଥିର ଶୂନ୍ୟ ngx_resolver_timeout_handler(ngx_event_t *ev);
ସ୍ଥିର ଶୂନ୍ୟ ngx_resolver_free_ଗଣ୍ଠି(ngx_resolver_t *r, ngx_resolver_ଗଣ୍ଠି_t *rn);
ସ୍ଥିର ଶୂନ୍ୟ *ngx_resolver_alloc(ngx_resolver_t *r, size_t size);
ସ୍ଥିର ଶୂନ୍ୟ *ngx_resolver_calloc(ngx_resolver_t *r, size_t size);
ସ୍ଥିର ଶୂନ୍ୟ ngx_resolver_free(ngx_resolver_t *r, ଶୂନ୍ୟ *p);
ସ୍ଥିର ଶୂନ୍ୟ ngx_resolver_free_locked(ngx_resolver_t *r, ଶୂନ୍ୟ *p);
ସ୍ଥିର ଶୂନ୍ୟ *ngx_resolver_dup(ngx_resolver_t *r, ଶୂନ୍ୟ *src, size_t size);
ସ୍ଥିର ngx_resolver_addr_t *ngx_resolver_export(ngx_resolver_t *r,
    ngx_resolver_ଗଣ୍ଠି_t *rn, ngx_uint_t rotate);
ସ୍ଥିର ଶୂନ୍ୟ ngx_resolver_report_srv(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx);
ସ୍ଥିର u_char *ngx_resolver_log_error(ngx_log_t *log, u_char *buf, size_t len);
ସ୍ଥିର ଶୂନ୍ୟ ngx_resolver_resolve_srv_names(ngx_resolver_ctx_t *ctx,
    ngx_resolver_ଗଣ୍ଠି_t *rn);
ସ୍ଥିର ଶୂନ୍ୟ ngx_resolver_srv_names_handler(ngx_resolver_ctx_t *ctx);
ସ୍ଥିର ngx_int_t ngx_resolver_cmp_srvs(ଅଚଳ ଶୂନ୍ୟ *one, ଅଚଳ ଶୂନ୍ୟ *two);

#ଯଦି (NGX_HAVE_INET6)
ସ୍ଥିର ଶୂନ୍ୟ ngx_resolver_rbtree_ଭର୍ତ୍ତି_addr6_ମୂଲ୍ୟ(ngx_rbtree_ଗଣ୍ଠି_t *temp,
    ngx_rbtree_ଗଣ୍ଠି_t *ଗଣ୍ଠି, ngx_rbtree_ଗଣ୍ଠି_t *sentinel);
ସ୍ଥିର ngx_resolver_ଗଣ୍ଠି_t *ngx_resolver_lookup_addr6(ngx_resolver_t *r,
    ସଂରଚନା in6_addr *addr, uint32_t hash);
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

    r = ngx_pcalloc(cf->pool, ଆକାର(ngx_resolver_t));
    ଯଦି (r == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    r->event = ngx_pcalloc(cf->pool, ଆକାର(ngx_event_t));
    ଯଦି (r->event == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    cln = ngx_pool_cleanup_add(cf->pool, 0);
    ଯଦି (cln == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    cln->handler = ngx_resolver_cleanup;
    cln->data = r;

    r->ipv4 = 1;

    ngx_rbtree_init(&r->name_rbtree, &r->name_sentinel,
                    ngx_resolver_rbtree_ଭର୍ତ୍ତି_ମୂଲ୍ୟ);

    ngx_rbtree_init(&r->srv_rbtree, &r->srv_sentinel,
                    ngx_resolver_rbtree_ଭର୍ତ୍ତି_ମୂଲ୍ୟ);

    ngx_rbtree_init(&r->addr_rbtree, &r->addr_sentinel,
                    ngx_rbtree_ଭର୍ତ୍ତି_ମୂଲ୍ୟ);

    ngx_queue_init(&r->name_resend_queue);
    ngx_queue_init(&r->srv_resend_queue);
    ngx_queue_init(&r->addr_resend_queue);

    ngx_queue_init(&r->name_expire_queue);
    ngx_queue_init(&r->srv_expire_queue);
    ngx_queue_init(&r->addr_expire_queue);

#ଯଦି (NGX_HAVE_INET6)
    r->ipv6 = 1;

    ngx_rbtree_init(&r->addr6_rbtree, &r->addr6_sentinel,
                    ngx_resolver_rbtree_ଭର୍ତ୍ତି_addr6_ମୂଲ୍ୟ);

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

    ଯଦି (n) {
        ଯଦି (ngx_array_init(&r->connections, cf->pool, n,
                           ଆକାର(ngx_resolver_connection_t))
            != NGX_OK)
        {
            ଫେରନ୍ତୁ ଖାଲି;
        }
    }

    ପାଇଁ (i = 0; i < n; i++) {
        ଯଦି (ngx_strncmp(names[i].data, "valid=", 6) == 0) {
            s.len = names[i].len - 6;
            s.data = names[i].data + 6;

            r->valid = ngx_parse_time(&s, 1);

            ଯଦି (r->valid == (time_t) NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &names[i]);
                ଫେରନ୍ତୁ ଖାଲି;
            }

            ଜାରି;
        }

#ଯଦି (NGX_HAVE_INET6)
        ଯଦି (ngx_strncmp(names[i].data, "ipv4=", 5) == 0) {

            ଯଦି (ngx_strcmp(&names[i].data[5], "on") == 0) {
                r->ipv4 = 1;

            } ଅନ୍ୟଥା ଯଦି (ngx_strcmp(&names[i].data[5], "off") == 0) {
                r->ipv4 = 0;

            } ଅନ୍ୟଥା {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &names[i]);
                ଫେରନ୍ତୁ ଖାଲି;
            }

            ଜାରି;
        }

        ଯଦି (ngx_strncmp(names[i].data, "ipv6=", 5) == 0) {

            ଯଦି (ngx_strcmp(&names[i].data[5], "on") == 0) {
                r->ipv6 = 1;

            } ଅନ୍ୟଥା ଯଦି (ngx_strcmp(&names[i].data[5], "off") == 0) {
                r->ipv6 = 0;

            } ଅନ୍ୟଥା {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &names[i]);
                ଫେରନ୍ତୁ ଖାଲି;
            }

            ଜାରି;
        }
#endif

        ngx_memzero(&u, ଆକାର(ngx_url_t));

        u.url = names[i];
        u.default_port = 53;

        ଯଦି (ngx_parse_url(cf->pool, &u) != NGX_OK) {
            ଯଦି (u.err) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "%s in resolver \"%V\"",
                                   u.err, &u.url);
            }

            ଫେରନ୍ତୁ ଖାଲି;
        }

        rec = ngx_array_ଠେଲ_n(&r->connections, u.naddrs);
        ଯଦି (rec == ଖାଲି) {
            ଫେରନ୍ତୁ ଖାଲି;
        }

        ngx_memzero(rec, u.naddrs * ଆକାର(ngx_resolver_connection_t));

        ପାଇଁ (j = 0; j < u.naddrs; j++) {
            rec[j].sockaddr = u.addrs[j].sockaddr;
            rec[j].socklen = u.addrs[j].socklen;
            rec[j].server = u.addrs[j].name;
            rec[j].resolver = r;
        }
    }

#ଯଦି (NGX_HAVE_INET6)
    ଯଦି (r->ipv4 + r->ipv6 == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"ipv4\" and \"ipv6\" cannot both be \"off\"");
        ଫେରନ୍ତୁ ଖାଲି;
    }
#endif

    ଯଦି (n && r->connections.nelts == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "no name servers defined");
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଫେରନ୍ତୁ r;
}


ସ୍ଥିର ଶୂନ୍ୟ
ngx_resolver_cleanup(ଶୂନ୍ୟ *data)
{
    ngx_resolver_t  *r = data;

    ngx_uint_t                  i;
    ngx_resolver_connection_t  *rec;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "cleanup resolver");

    ngx_resolver_cleanup_ବୃକ୍ଷ(r, &r->name_rbtree);

    ngx_resolver_cleanup_ବୃକ୍ଷ(r, &r->srv_rbtree);

    ngx_resolver_cleanup_ବୃକ୍ଷ(r, &r->addr_rbtree);

#ଯଦି (NGX_HAVE_INET6)
    ngx_resolver_cleanup_ବୃକ୍ଷ(r, &r->addr6_rbtree);
#endif

    ଯଦି (r->event->timer_set) {
        ngx_del_timer(r->event);
    }

    rec = r->connections.elts;

    ପାଇଁ (i = 0; i < r->connections.nelts; i++) {
        ଯଦି (rec[i].udp) {
            ngx_close_connection(rec[i].udp);
        }

        ଯଦି (rec[i].tcp) {
            ngx_close_connection(rec[i].tcp);
        }

        ଯଦି (rec[i].read_buf) {
            ngx_resolver_free(r, rec[i].read_buf->start);
            ngx_resolver_free(r, rec[i].read_buf);
        }

        ଯଦି (rec[i].write_buf) {
            ngx_resolver_free(r, rec[i].write_buf->start);
            ngx_resolver_free(r, rec[i].write_buf);
        }
    }
}


ସ୍ଥିର ଶୂନ୍ୟ
ngx_resolver_cleanup_ବୃକ୍ଷ(ngx_resolver_t *r, ngx_rbtree_t *ବୃକ୍ଷ)
{
    ngx_resolver_ctx_t   *ctx, *next;
    ngx_resolver_ଗଣ୍ଠି_t  *rn;

    ଯେତେବେଳେ (ବୃକ୍ଷ->ମୂଳ != ବୃକ୍ଷ->sentinel) {

        rn = ngx_resolver_ଗଣ୍ଠି(ngx_rbtree_min(ବୃକ୍ଷ->ମୂଳ, ବୃକ୍ଷ->sentinel));

        ngx_queue_remove(&rn->queue);

        ପାଇଁ (ctx = rn->waiting; ctx; ctx = next) {
            next = ctx->next;

            ଯଦି (ctx->event) {
                ଯଦି (ctx->event->timer_set) {
                    ngx_del_timer(ctx->event);
                }

                ngx_resolver_free(r, ctx->event);
            }

            ngx_resolver_free(r, ctx);
        }

        ngx_rbtree_delete(ବୃକ୍ଷ, &rn->ଗଣ୍ଠି);

        ngx_resolver_free_ଗଣ୍ଠି(r, rn);
    }
}


ngx_resolver_ctx_t *
ngx_resolve_start(ngx_resolver_t *r, ngx_resolver_ctx_t *temp)
{
    in_addr_t            addr;
    ngx_resolver_ctx_t  *ctx;

    ଯଦି (temp) {
        addr = ngx_inet_addr(temp->name.data, temp->name.len);

        ଯଦି (addr != INADDR_NONE) {
            temp->resolver = r;
            temp->state = NGX_OK;
            temp->naddrs = 1;
            temp->addrs = &temp->addr;
            temp->addr.sockaddr = (ସଂରଚନା sockaddr *) &temp->sin;
            temp->addr.socklen = ଆକାର(ସଂରଚନା sockaddr_in);
            ngx_memzero(&temp->sin, ଆକାର(ସଂରଚନା sockaddr_in));
            temp->sin.sin_family = AF_INET;
            temp->sin.sin_addr.s_addr = addr;
            temp->quick = 1;

            ଫେରନ୍ତୁ temp;
        }
    }

    ଯଦି (r->connections.nelts == 0) {
        ଫେରନ୍ତୁ NGX_NO_RESOLVER;
    }

    ctx = ngx_resolver_calloc(r, ଆକାର(ngx_resolver_ctx_t));

    ଯଦି (ctx) {
        ctx->resolver = r;
    }

    ଫେରନ୍ତୁ ctx;
}


ngx_int_t
ngx_resolve_name(ngx_resolver_ctx_t *ctx)
{
    size_t           slen;
    ngx_int_t        rc;
    ngx_str_t        name;
    ngx_resolver_t  *r;

    r = ctx->resolver;

    ଯଦି (ctx->name.len > 0 && ctx->name.data[ctx->name.len - 1] == '.') {
        ctx->name.len--;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve: \"%V\"", &ctx->name);

    ଯଦି (ctx->quick) {
        ctx->handler(ctx);
        ଫେରନ୍ତୁ NGX_OK;
    }

    ଯଦି (ctx->service.len) {
        slen = ctx->service.len;

        ଯଦି (ngx_strlchr(ctx->service.data,
                        ctx->service.data + ctx->service.len, '.')
            == ଖାଲି)
        {
            slen += ଆକାର("_._tcp") - 1;
        }

        name.len = slen + 1 + ctx->name.len;

        name.data = ngx_resolver_alloc(r, name.len);
        ଯଦି (name.data == ଖାଲି) {
            ଯାଅ failed;
        }

        ଯଦି (slen == ctx->service.len) {
            ngx_sprintf(name.data, "%V.%V", &ctx->service, &ctx->name);

        } ଅନ୍ୟଥା {
            ngx_sprintf(name.data, "_%V._tcp.%V", &ctx->service, &ctx->name);
        }

        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &name);

        ngx_resolver_free(r, name.data);

    } ଅନ୍ୟଥା {
        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &ctx->name);
    }

    ଯଦି (rc == NGX_OK) {
        ଫେରନ୍ତୁ NGX_OK;
    }

    /* unlock name mutex */

    ଯଦି (rc == NGX_AGAIN) {
        ଫେରନ୍ତୁ NGX_OK;
    }

    /* NGX_ERROR */

    ଯଦି (ctx->event) {
        ngx_resolver_free(r, ctx->event);
    }

failed:

    ngx_resolver_free(r, ctx);

    ଫେରନ୍ତୁ NGX_ERROR;
}


ଶୂନ୍ୟ
ngx_resolve_name_done(ngx_resolver_ctx_t *ctx)
{
    ngx_uint_t            i;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_ଗଣ୍ଠି_t  *rn;

    r = ctx->resolver;

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve name done: %i", ctx->state);

    ଯଦି (ctx->quick) {
        ଫେରନ୍ତୁ;
    }

    ଯଦି (ctx->event && ctx->event->timer_set) {
        ngx_del_timer(ctx->event);
    }

    /* lock name mutex */

    ଯଦି (ctx->nsrvs) {
        ପାଇଁ (i = 0; i < ctx->nsrvs; i++) {
            ଯଦି (ctx->srvs[i].ctx) {
                ngx_resolve_name_done(ctx->srvs[i].ctx);
            }

            ଯଦି (ctx->srvs[i].addrs) {
                ngx_resolver_free(r, ctx->srvs[i].addrs->sockaddr);
                ngx_resolver_free(r, ctx->srvs[i].addrs);
            }

            ngx_resolver_free(r, ctx->srvs[i].name.data);
        }

        ngx_resolver_free(r, ctx->srvs);
    }

    ଯଦି (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->ଗଣ୍ଠି;

        ଯଦି (rn) {
            p = &rn->waiting;
            w = rn->waiting;

            ଯେତେବେଳେ (w) {
                ଯଦି (w == ctx) {
                    *p = w->next;

                    ଯାଅ done;
                }

                p = &w->next;
                w = w->next;
            }

            ngx_log_error(NGX_LOG_ALERT, r->log, 0,
                          "could not cancel %V resolving", &ctx->name);
        }
    }

done:

    ଯଦି (ctx->service.len) {
        ngx_resolver_expire(r, &r->srv_rbtree, &r->srv_expire_queue);

    } ଅନ୍ୟଥା {
        ngx_resolver_expire(r, &r->name_rbtree, &r->name_expire_queue);
    }

    /* unlock name mutex */

    /* lock alloc mutex */

    ଯଦି (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    ଯଦି (r->event->timer_set && ngx_resolver_resend_empty(r)) {
        ngx_del_timer(r->event);
    }
}


ସ୍ଥିର ngx_int_t
ngx_resolve_name_locked(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx,
    ngx_str_t *name)
{
    uint32_t              hash;
    ngx_int_t             rc;
    ngx_str_t             cname;
    ngx_uint_t            i, naddrs;
    ngx_queue_t          *resend_queue, *expire_queue;
    ngx_rbtree_t         *ବୃକ୍ଷ;
    ngx_resolver_ctx_t   *next, *last;
    ngx_resolver_addr_t  *addrs;
    ngx_resolver_ଗଣ୍ଠି_t  *rn;

    ngx_strlow(name->data, name->data, name->len);

    hash = ngx_crc32_short(name->data, name->len);

    ଯଦି (ctx->service.len) {
        rn = ngx_resolver_lookup_srv(r, name, hash);

        ବୃକ୍ଷ = &r->srv_rbtree;
        resend_queue = &r->srv_resend_queue;
        expire_queue = &r->srv_expire_queue;

    } ଅନ୍ୟଥା {
        rn = ngx_resolver_lookup_name(r, name, hash);

        ବୃକ୍ଷ = &r->name_rbtree;
        resend_queue = &r->name_resend_queue;
        expire_queue = &r->name_expire_queue;
    }

    ଯଦି (rn) {

        /* ctx can be a list after NGX_RESOLVE_CNAME */
        ପାଇଁ (last = ctx; last->next; last = last->next);

        ଯଦି (rn->valid >= ngx_time()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0, "resolve cached");

            ngx_queue_remove(&rn->queue);

            rn->expire = ngx_time() + r->expire;

            ngx_queue_ଭର୍ତ୍ତି_head(expire_queue, &rn->queue);

            naddrs = (rn->naddrs == (u_short) -1) ? 0 : rn->naddrs;
#ଯଦି (NGX_HAVE_INET6)
            naddrs += (rn->naddrs6 == (u_short) -1) ? 0 : rn->naddrs6;
#endif

            ଯଦି (naddrs) {

                ଯଦି (naddrs == 1 && rn->naddrs == 1) {
                    addrs = ଖାଲି;

                } ଅନ୍ୟଥା {
                    addrs = ngx_resolver_export(r, rn, 1);
                    ଯଦି (addrs == ଖାଲି) {
                        ଫେରନ୍ତୁ NGX_ERROR;
                    }
                }

                last->next = rn->waiting;
                rn->waiting = ଖାଲି;

                /* unlock name mutex */

                କର {
                    ctx->state = NGX_OK;
                    ctx->valid = rn->valid;
                    ctx->naddrs = naddrs;

                    ଯଦି (addrs == ଖାଲି) {
                        ctx->addrs = &ctx->addr;
                        ctx->addr.sockaddr = (ସଂରଚନା sockaddr *) &ctx->sin;
                        ctx->addr.socklen = ଆକାର(ସଂରଚନା sockaddr_in);
                        ngx_memzero(&ctx->sin, ଆକାର(ସଂରଚନା sockaddr_in));
                        ctx->sin.sin_family = AF_INET;
                        ctx->sin.sin_addr.s_addr = rn->u.addr;

                    } ଅନ୍ୟଥା {
                        ctx->addrs = addrs;
                    }

                    next = ctx->next;

                    ctx->handler(ctx);

                    ctx = next;
                } ଯେତେବେଳେ (ctx);

                ଯଦି (addrs != ଖାଲି) {
                    ngx_resolver_free(r, addrs->sockaddr);
                    ngx_resolver_free(r, addrs);
                }

                ଫେରନ୍ତୁ NGX_OK;
            }

            ଯଦି (rn->nsrvs) {
                last->next = rn->waiting;
                rn->waiting = ଖାଲି;

                /* unlock name mutex */

                କର {
                    next = ctx->next;

                    ngx_resolver_resolve_srv_names(ctx, rn);

                    ctx = next;
                } ଯେତେବେଳେ (ctx);

                ଫେରନ୍ତୁ NGX_OK;
            }

            /* NGX_RESOLVE_CNAME */

            ଯଦି (ctx->recursion++ < NGX_RESOLVER_MAX_RECURSION) {

                cname.len = rn->cnlen;
                cname.data = rn->u.cname;

                ଫେରନ୍ତୁ ngx_resolve_name_locked(r, ctx, &cname);
            }

            last->next = rn->waiting;
            rn->waiting = ଖାଲି;

            /* unlock name mutex */

            କର {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
                next = ctx->next;

                ctx->handler(ctx);

                ctx = next;
            } ଯେତେବେଳେ (ctx);

            ଫେରନ୍ତୁ NGX_OK;
        }

        ଯଦି (rn->waiting) {
            ଯଦି (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            last->next = rn->waiting;
            rn->waiting = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;

            କର {
                ctx->ଗଣ୍ଠି = rn;
                ctx = ctx->next;
            } ଯେତେବେଳେ (ctx);

            ଫେରନ୍ତୁ NGX_AGAIN;
        }

        ngx_queue_remove(&rn->queue);

        /* lock alloc mutex */

        ଯଦି (rn->query) {
            ngx_resolver_free_locked(r, rn->query);
            rn->query = ଖାଲି;
#ଯଦି (NGX_HAVE_INET6)
            rn->query6 = ଖାଲି;
#endif
        }

        ଯଦି (rn->cnlen) {
            ngx_resolver_free_locked(r, rn->u.cname);
        }

        ଯଦି (rn->naddrs > 1 && rn->naddrs != (u_short) -1) {
            ngx_resolver_free_locked(r, rn->u.addrs);
        }

#ଯଦି (NGX_HAVE_INET6)
        ଯଦି (rn->naddrs6 > 1 && rn->naddrs6 != (u_short) -1) {
            ngx_resolver_free_locked(r, rn->u6.addrs6);
        }
#endif

        ଯଦି (rn->nsrvs) {
            ପାଇଁ (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
                ଯଦି (rn->u.srvs[i].name.data) {
                    ngx_resolver_free_locked(r, rn->u.srvs[i].name.data);
                }
            }

            ngx_resolver_free_locked(r, rn->u.srvs);
        }

        /* unlock alloc mutex */

    } ଅନ୍ୟଥା {

        rn = ngx_resolver_alloc(r, ଆକାର(ngx_resolver_ଗଣ୍ଠି_t));
        ଯଦି (rn == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        rn->name = ngx_resolver_dup(r, name->data, name->len);
        ଯଦି (rn->name == ଖାଲି) {
            ngx_resolver_free(r, rn);
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        rn->ଗଣ୍ଠି.ଚାବି = hash;
        rn->nlen = (u_short) name->len;
        rn->query = ଖାଲି;
#ଯଦି (NGX_HAVE_INET6)
        rn->query6 = ଖାଲି;
#endif

        ngx_rbtree_ଭର୍ତ୍ତି(ବୃକ୍ଷ, &rn->ଗଣ୍ଠି);
    }

    ଯଦି (ctx->service.len) {
        rc = ngx_resolver_create_srv_query(r, rn, name);

    } ଅନ୍ୟଥା {
        rc = ngx_resolver_create_name_query(r, rn, name);
    }

    ଯଦି (rc == NGX_ERROR) {
        ଯାଅ failed;
    }

    ଯଦି (rc == NGX_DECLINED) {
        ngx_rbtree_delete(ବୃକ୍ଷ, &rn->ଗଣ୍ଠି);

        ngx_resolver_free(r, rn->query);
        ngx_resolver_free(r, rn->name);
        ngx_resolver_free(r, rn);

        କର {
            ctx->state = NGX_RESOLVE_NXDOMAIN;
            next = ctx->next;

            ctx->handler(ctx);

            ctx = next;
        } ଯେତେବେଳେ (ctx);

        ଫେରନ୍ତୁ NGX_OK;
    }

    rn->last_connection = r->last_connection++;
    ଯଦି (r->last_connection == r->connections.nelts) {
        r->last_connection = 0;
    }

    rn->naddrs = r->ipv4 ? (u_short) -1 : 0;
    rn->tcp = 0;
#ଯଦି (NGX_HAVE_INET6)
    rn->naddrs6 = r->ipv6 ? (u_short) -1 : 0;
    rn->tcp6 = 0;
#endif
    rn->nsrvs = 0;

    ଯଦି (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->last_connection++;
        ଯଦି (rn->last_connection == r->connections.nelts) {
            rn->last_connection = 0;
        }

        (ଶୂନ୍ୟ) ngx_resolver_send_query(r, rn);
    }

    ଯଦି (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
        ଯାଅ failed;
    }

    ଯଦି (ngx_resolver_resend_empty(r)) {
        ngx_add_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_time() + r->resend_timeout;

    ngx_queue_ଭର୍ତ୍ତି_head(resend_queue, &rn->queue);

    rn->code = 0;
    rn->cnlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_ମୂଲ୍ୟ;
    rn->waiting = ctx;

    ctx->state = NGX_AGAIN;
    ctx->async = 1;

    କର {
        ctx->ଗଣ୍ଠି = rn;
        ctx = ctx->next;
    } ଯେତେବେଳେ (ctx);

    ଫେରନ୍ତୁ NGX_AGAIN;

failed:

    ngx_rbtree_delete(ବୃକ୍ଷ, &rn->ଗଣ୍ଠି);

    ଯଦି (rn->query) {
        ngx_resolver_free(r, rn->query);
    }

    ngx_resolver_free(r, rn->name);

    ngx_resolver_free(r, rn);

    ଫେରନ୍ତୁ NGX_ERROR;
}


ngx_int_t
ngx_resolve_addr(ngx_resolver_ctx_t *ctx)
{
    u_char               *name;
    in_addr_t             addr;
    ngx_queue_t          *resend_queue, *expire_queue;
    ngx_rbtree_t         *ବୃକ୍ଷ;
    ngx_resolver_t       *r;
    ସଂରଚନା sockaddr_in   *sin;
    ngx_resolver_ଗଣ୍ଠି_t  *rn;
#ଯଦି (NGX_HAVE_INET6)
    uint32_t              hash;
    ସଂରଚନା sockaddr_in6  *sin6;
#endif

#ଯଦି (NGX_SUPPRESS_WARN)
    addr = 0;
#ଯଦି (NGX_HAVE_INET6)
    hash = 0;
    sin6 = ଖାଲି;
#endif
#endif

    r = ctx->resolver;

    ବାଛ (ctx->addr.sockaddr->sa_family) {

#ଯଦି (NGX_HAVE_INET6)
    ସ୍ଥିତି AF_INET6:
        sin6 = (ସଂରଚନା sockaddr_in6 *) ctx->addr.sockaddr;
        hash = ngx_crc32_short(sin6->sin6_addr.s6_addr, 16);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr6(r, &sin6->sin6_addr, hash);

        ବୃକ୍ଷ = &r->addr6_rbtree;
        resend_queue = &r->addr6_resend_queue;
        expire_queue = &r->addr6_expire_queue;

        ଭାଙ୍ଗ;
#endif

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: /* AF_INET */
        sin = (ସଂରଚନା sockaddr_in *) ctx->addr.sockaddr;
        addr = ntohl(sin->sin_addr.s_addr);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        ବୃକ୍ଷ = &r->addr_rbtree;
        resend_queue = &r->addr_resend_queue;
        expire_queue = &r->addr_expire_queue;
    }

    ଯଦି (rn) {

        ଯଦି (rn->valid >= ngx_time()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0, "resolve cached");

            ngx_queue_remove(&rn->queue);

            rn->expire = ngx_time() + r->expire;

            ngx_queue_ଭର୍ତ୍ତି_head(expire_queue, &rn->queue);

            name = ngx_resolver_dup(r, rn->name, rn->nlen);
            ଯଦି (name == ଖାଲି) {
                ngx_resolver_free(r, ctx);
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            ctx->name.len = rn->nlen;
            ctx->name.data = name;

            /* unlock addr mutex */

            ctx->state = NGX_OK;
            ctx->valid = rn->valid;

            ctx->handler(ctx);

            ngx_resolver_free(r, name);

            ଫେରନ୍ତୁ NGX_OK;
        }

        ଯଦି (rn->waiting) {
            ଯଦି (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            ctx->next = rn->waiting;
            rn->waiting = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;
            ctx->ଗଣ୍ଠି = rn;

            /* unlock addr mutex */

            ଫେରନ୍ତୁ NGX_OK;
        }

        ngx_queue_remove(&rn->queue);

        ngx_resolver_free(r, rn->query);
        rn->query = ଖାଲି;
#ଯଦି (NGX_HAVE_INET6)
        rn->query6 = ଖାଲି;
#endif

    } ଅନ୍ୟଥା {
        rn = ngx_resolver_alloc(r, ଆକାର(ngx_resolver_ଗଣ୍ଠି_t));
        ଯଦି (rn == ଖାଲି) {
            ଯାଅ failed;
        }

        ବାଛ (ctx->addr.sockaddr->sa_family) {

#ଯଦି (NGX_HAVE_INET6)
        ସ୍ଥିତି AF_INET6:
            rn->addr6 = sin6->sin6_addr;
            rn->ଗଣ୍ଠି.ଚାବି = hash;
            ଭାଙ୍ଗ;
#endif

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ: /* AF_INET */
            rn->ଗଣ୍ଠି.ଚାବି = addr;
        }

        rn->query = ଖାଲି;
#ଯଦି (NGX_HAVE_INET6)
        rn->query6 = ଖାଲି;
#endif

        ngx_rbtree_ଭର୍ତ୍ତି(ବୃକ୍ଷ, &rn->ଗଣ୍ଠି);
    }

    ଯଦି (ngx_resolver_create_addr_query(r, rn, &ctx->addr) != NGX_OK) {
        ଯାଅ failed;
    }

    rn->last_connection = r->last_connection++;
    ଯଦି (r->last_connection == r->connections.nelts) {
        r->last_connection = 0;
    }

    rn->naddrs = (u_short) -1;
    rn->tcp = 0;
#ଯଦି (NGX_HAVE_INET6)
    rn->naddrs6 = (u_short) -1;
    rn->tcp6 = 0;
#endif
    rn->nsrvs = 0;

    ଯଦି (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->last_connection++;
        ଯଦି (rn->last_connection == r->connections.nelts) {
            rn->last_connection = 0;
        }

        (ଶୂନ୍ୟ) ngx_resolver_send_query(r, rn);
    }

    ଯଦି (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
        ଯାଅ failed;
    }

    ଯଦି (ngx_resolver_resend_empty(r)) {
        ngx_add_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_time() + r->resend_timeout;

    ngx_queue_ଭର୍ତ୍ତି_head(resend_queue, &rn->queue);

    rn->code = 0;
    rn->cnlen = 0;
    rn->name = ଖାଲି;
    rn->nlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_ମୂଲ୍ୟ;
    rn->waiting = ctx;

    /* unlock addr mutex */

    ctx->state = NGX_AGAIN;
    ctx->async = 1;
    ctx->ଗଣ୍ଠି = rn;

    ଫେରନ୍ତୁ NGX_OK;

failed:

    ଯଦି (rn) {
        ngx_rbtree_delete(ବୃକ୍ଷ, &rn->ଗଣ୍ଠି);

        ଯଦି (rn->query) {
            ngx_resolver_free(r, rn->query);
        }

        ngx_resolver_free(r, rn);
    }

    /* unlock addr mutex */

    ଯଦି (ctx->event) {
        ngx_resolver_free(r, ctx->event);
    }

    ngx_resolver_free(r, ctx);

    ଫେରନ୍ତୁ NGX_ERROR;
}


ଶୂନ୍ୟ
ngx_resolve_addr_done(ngx_resolver_ctx_t *ctx)
{
    ngx_queue_t          *expire_queue;
    ngx_rbtree_t         *ବୃକ୍ଷ;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_ଗଣ୍ଠି_t  *rn;

    r = ctx->resolver;

    ବାଛ (ctx->addr.sockaddr->sa_family) {

#ଯଦି (NGX_HAVE_INET6)
    ସ୍ଥିତି AF_INET6:
        ବୃକ୍ଷ = &r->addr6_rbtree;
        expire_queue = &r->addr6_expire_queue;
        ଭାଙ୍ଗ;
#endif

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: /* AF_INET */
        ବୃକ୍ଷ = &r->addr_rbtree;
        expire_queue = &r->addr_expire_queue;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve addr done: %i", ctx->state);

    ଯଦି (ctx->event && ctx->event->timer_set) {
        ngx_del_timer(ctx->event);
    }

    /* lock addr mutex */

    ଯଦି (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->ଗଣ୍ଠି;

        ଯଦି (rn) {
            p = &rn->waiting;
            w = rn->waiting;

            ଯେତେବେଳେ (w) {
                ଯଦି (w == ctx) {
                    *p = w->next;

                    ଯାଅ done;
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

    ngx_resolver_expire(r, ବୃକ୍ଷ, expire_queue);

    /* unlock addr mutex */

    /* lock alloc mutex */

    ଯଦି (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    ଯଦି (r->event->timer_set && ngx_resolver_resend_empty(r)) {
        ngx_del_timer(r->event);
    }
}


ସ୍ଥିର ଶୂନ୍ୟ
ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *ବୃକ୍ଷ, ngx_queue_t *queue)
{
    time_t                now;
    ngx_uint_t            i;
    ngx_queue_t          *q;
    ngx_resolver_ଗଣ୍ଠି_t  *rn;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver expire");

    now = ngx_time();

    ପାଇଁ (i = 0; i < 2; i++) {
        ଯଦି (ngx_queue_empty(queue)) {
            ଫେରନ୍ତୁ;
        }

        q = ngx_queue_last(queue);

        rn = ngx_queue_data(q, ngx_resolver_ଗଣ୍ଠି_t, queue);

        ଯଦି (now <= rn->expire) {
            ଫେରନ୍ତୁ;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver expire \"%*s\"", (size_t) rn->nlen, rn->name);

        ngx_queue_remove(q);

        ngx_rbtree_delete(ବୃକ୍ଷ, &rn->ଗଣ୍ଠି);

        ngx_resolver_free_ଗଣ୍ଠି(r, rn);
    }
}


ସ୍ଥିର ngx_int_t
ngx_resolver_send_query(ngx_resolver_t *r, ngx_resolver_ଗଣ୍ଠି_t *rn)
{
    ngx_int_t                   rc;
    ngx_resolver_connection_t  *rec;

    rec = r->connections.elts;
    rec = &rec[rn->last_connection];

    ଯଦି (rec->log.handler == ଖାଲି) {
        rec->log = *r->log;
        rec->log.handler = ngx_resolver_log_error;
        rec->log.data = rec;
        rec->log.action = "resolving";
    }

    ଯଦି (rn->query && rn->naddrs == (u_short) -1) {
        rc = rn->tcp ? ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen)
                     : ngx_resolver_send_udp_query(r, rec, rn->query, rn->qlen);

        ଯଦି (rc != NGX_OK) {
            ଫେରନ୍ତୁ rc;
        }
    }

#ଯଦି (NGX_HAVE_INET6)

    ଯଦି (rn->query6 && rn->naddrs6 == (u_short) -1) {
        rc = rn->tcp6
                    ? ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen)
                    : ngx_resolver_send_udp_query(r, rec, rn->query6, rn->qlen);

        ଯଦି (rc != NGX_OK) {
            ଫେରନ୍ତୁ rc;
        }
    }

#endif

    ଫେରନ୍ତୁ NGX_OK;
}


ସ୍ଥିର ngx_int_t
ngx_resolver_send_udp_query(ngx_resolver_t *r, ngx_resolver_connection_t  *rec,
    u_char *query, u_short qlen)
{
    ssize_t  n;

    ଯଦି (rec->udp == ଖାଲି) {
        ଯଦି (ngx_udp_connect(rec) != NGX_OK) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        rec->udp->data = rec;
        rec->udp->ପଠ->handler = ngx_resolver_udp_read;
        rec->udp->ପଠ->resolver = 1;
    }

    n = ngx_send(rec->udp, query, qlen);

    ଯଦି (n == NGX_ERROR) {
        ଯାଅ failed;
    }

    ଯଦି ((size_t) n != (size_t) qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->log, 0, "send() incomplete");
        ଯାଅ failed;
    }

    ଫେରନ୍ତୁ NGX_OK;

failed:

    ngx_close_connection(rec->udp);
    rec->udp = ଖାଲି;

    ଫେରନ୍ତୁ NGX_ERROR;
}


ସ୍ଥିର ngx_int_t
ngx_resolver_send_tcp_query(ngx_resolver_t *r, ngx_resolver_connection_t *rec,
    u_char *query, u_short qlen)
{
    ngx_buf_t  *b;
    ngx_int_t   rc;

    rc = NGX_OK;

    ଯଦି (rec->tcp == ଖାଲି) {
        b = rec->read_buf;

        ଯଦି (b == ଖାଲି) {
            b = ngx_resolver_calloc(r, ଆକାର(ngx_buf_t));
            ଯଦି (b == ଖାଲି) {
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            b->start = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_RSIZE);
            ଯଦି (b->start == ଖାଲି) {
                ngx_resolver_free(r, b);
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            b->end = b->start + NGX_RESOLVER_TCP_RSIZE;

            rec->read_buf = b;
        }

        b->pos = b->start;
        b->last = b->start;

        b = rec->write_buf;

        ଯଦି (b == ଖାଲି) {
            b = ngx_resolver_calloc(r, ଆକାର(ngx_buf_t));
            ଯଦି (b == ଖାଲି) {
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            b->start = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_WSIZE);
            ଯଦି (b->start == ଖାଲି) {
                ngx_resolver_free(r, b);
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            b->end = b->start + NGX_RESOLVER_TCP_WSIZE;

            rec->write_buf = b;
        }

        b->pos = b->start;
        b->last = b->start;

        rc = ngx_tcp_connect(rec);
        ଯଦି (rc == NGX_ERROR) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        rec->tcp->data = rec;
        rec->tcp->ଲିଖ->handler = ngx_resolver_tcp_write;
        rec->tcp->ଲିଖ->cancelable = 1;
        rec->tcp->ପଠ->handler = ngx_resolver_tcp_read;
        rec->tcp->ପଠ->resolver = 1;

        ngx_add_timer(rec->tcp->ଲିଖ, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    b = rec->write_buf;

    ଯଦି (b->end - b->last <  2 + qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->log, 0, "buffer overflow");
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    *b->last++ = (u_char) (qlen >> 8);
    *b->last++ = (u_char) qlen;
    b->last = ngx_cpymem(b->last, query, qlen);

    ଯଦି (rc == NGX_OK) {
        ngx_resolver_tcp_write(rec->tcp->ଲିଖ);
    }

    ଫେରନ୍ତୁ NGX_OK;
}


ସ୍ଥିର ଶୂନ୍ୟ
ngx_resolver_resend_handler(ngx_event_t *ev)
{
    time_t           timer, atimer, stimer, ntimer;
#ଯଦି (NGX_HAVE_INET6)
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

#ଯଦି (NGX_HAVE_INET6)

    /* lock addr6 mutex */

    a6timer = ngx_resolver_resend(r, &r->addr6_rbtree, &r->addr6_resend_queue);

    /* unlock addr6 mutex */

#endif

    timer = ntimer;

    ଯଦି (timer == 0) {
        timer = atimer;

    } ଅନ୍ୟଥା ଯଦି (atimer) {
        timer = ngx_min(timer, atimer);
    }

    ଯଦି (timer == 0) {
        timer = stimer;

    } ଅନ୍ୟଥା ଯଦି (stimer) {
        timer = ngx_min(timer, stimer);
    }

#ଯଦି (NGX_HAVE_INET6)

    ଯଦି (timer == 0) {
        timer = a6timer;

    } ଅନ୍ୟଥା ଯଦି (a6timer) {
        timer = ngx_min(timer, a6timer);
    }

#endif

    ଯଦି (timer) {
        ngx_add_timer(r->event, (ngx_msec_t) (timer * 1000));
    }
}


ସ୍ଥିର time_t
ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *ବୃକ୍ଷ, ngx_queue_t *queue)
{
    time_t                now;
    ngx_queue_t          *q;
    ngx_resolver_ଗଣ୍ଠି_t  *rn;

    now = ngx_time();

    ପାଇଁ ( ;; ) {
        ଯଦି (ngx_queue_empty(queue)) {
            ଫେରନ୍ତୁ 0;
        }

        q = ngx_queue_last(queue);

        rn = ngx_queue_data(q, ngx_resolver_ଗଣ୍ଠି_t, queue);

        ଯଦି (now < rn->expire) {
            ଫେରନ୍ତୁ rn->expire - now;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver resend \"%*s\" %p",
                       (size_t) rn->nlen, rn->name, rn->waiting);

        ngx_queue_remove(q);

        ଯଦି (rn->waiting) {

            ଯଦି (++rn->last_connection == r->connections.nelts) {
                rn->last_connection = 0;
            }

            (ଶୂନ୍ୟ) ngx_resolver_send_query(r, rn);

            rn->expire = now + r->resend_timeout;

            ngx_queue_ଭର୍ତ୍ତି_head(queue, q);

            ଜାରି;
        }

        ngx_rbtree_delete(ବୃକ୍ଷ, &rn->ଗଣ୍ଠି);

        ngx_resolver_free_ଗଣ୍ଠି(r, rn);
    }
}


ସ୍ଥିର ngx_uint_t
ngx_resolver_resend_empty(ngx_resolver_t *r)
{
    ଫେରନ୍ତୁ ngx_queue_empty(&r->name_resend_queue)
           && ngx_queue_empty(&r->srv_resend_queue)
#ଯଦି (NGX_HAVE_INET6)
           && ngx_queue_empty(&r->addr6_resend_queue)
#endif
           && ngx_queue_empty(&r->addr_resend_queue);
}


ସ୍ଥିର ଶୂନ୍ୟ
ngx_resolver_udp_read(ngx_event_t *rev)
{
    ssize_t                     n;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;
    u_char                      buf[NGX_RESOLVER_UDP_SIZE];

    c = rev->data;
    rec = c->data;

    କର {
        n = ngx_udp_recv(c, buf, NGX_RESOLVER_UDP_SIZE);

        ଯଦି (n == NGX_AGAIN) {
            ଭାଙ୍ଗ;
        }

        ଯଦି (n == NGX_ERROR) {
            ଯାଅ failed;
        }

        ngx_resolver_process_response(rec->resolver, buf, n, 0);

    } ଯେତେବେଳେ (rev->ready);

    ଯଦି (ngx_handle_read_event(rev, 0) != NGX_OK) {
        ଯାଅ failed;
    }

    ଫେରନ୍ତୁ;

failed:

    ngx_close_connection(rec->udp);
    rec->udp = ଖାଲି;
}


ସ୍ଥିର ଶୂନ୍ୟ
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

    ଯଦି (wev->timedout) {
        ଯାଅ failed;
    }

    sent = c->sent;

    ଯେତେବେଳେ (wev->ready && b->pos < b->last) {
        n = ngx_send(c, b->pos, b->last - b->pos);

        ଯଦି (n == NGX_AGAIN) {
            ଭାଙ୍ଗ;
        }

        ଯଦି (n == NGX_ERROR) {
            ଯାଅ failed;
        }

        b->pos += n;
    }

    ଯଦି (b->pos != b->start) {
        b->last = ngx_movemem(b->start, b->pos, b->last - b->pos);
        b->pos = b->start;
    }

    ଯଦି (c->sent != sent) {
        ngx_add_timer(wev, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    ଯଦି (ngx_handle_write_event(wev, 0) != NGX_OK) {
        ଯାଅ failed;
    }

    ଫେରନ୍ତୁ;

failed:

    ngx_close_connection(c);
    rec->tcp = ଖାଲି;
}


ସ୍ଥିର ଶୂନ୍ୟ
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

    ଯେତେବେଳେ (rev->ready) {
        n = ngx_recv(c, b->last, b->end - b->last);

        ଯଦି (n == NGX_AGAIN) {
            ଭାଙ୍ଗ;
        }

        ଯଦି (n == NGX_ERROR || n == 0) {
            ଯାଅ failed;
        }

        b->last += n;

        ପାଇଁ ( ;; ) {
            p = b->pos;
            size = b->last - p;

            ଯଦି (size < 2) {
                ଭାଙ୍ଗ;
            }

            qlen = (u_short) *p++ << 8;
            qlen += *p++;

            ଯଦି (size < (size_t) (2 + qlen)) {
                ଭାଙ୍ଗ;
            }

            ngx_resolver_process_response(r, p, qlen, 1);

            b->pos += 2 + qlen;
        }

        ଯଦି (b->pos != b->start) {
            b->last = ngx_movemem(b->start, b->pos, b->last - b->pos);
            b->pos = b->start;
        }
    }

    ଯଦି (ngx_handle_read_event(rev, 0) != NGX_OK) {
        ଯାଅ failed;
    }

    ଫେରନ୍ତୁ;

failed:

    ngx_close_connection(c);
    rec->tcp = ଖାଲି;
}


ସ୍ଥିର ଶୂନ୍ୟ
ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t tcp)
{
    ଅକ୍ଷର                 *err;
    ngx_uint_t            i, times, ident, qident, flags, code, nqs, nan, trunc,
                          qtype, qclass;
#ଯଦି (NGX_HAVE_INET6)
    ngx_uint_t            qident6;
#endif
    ngx_queue_t          *q;
    ngx_resolver_qs_t    *qs;
    ngx_resolver_hdr_t   *response;
    ngx_resolver_ଗଣ୍ଠି_t  *rn;

    ଯଦି (n < ଆକାର(ngx_resolver_hdr_t)) {
        ଯାଅ short_response;
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
    ଯଦି ((flags & 0xf870) != 0x8000 || (trunc && tcp)) {
        ngx_log_error(r->log_level, r->log, 0,
                      "invalid %s DNS response %ui fl:%04Xi",
                      tcp ? "TCP" : "UDP", ident, flags);
        ଫେରନ୍ତୁ;
    }

    code = flags & 0xf;

    ଯଦି (code == NGX_RESOLVE_FORMERR) {

        times = 0;

        ପାଇଁ (q = ngx_queue_head(&r->name_resend_queue);
             q != ngx_queue_sentinel(&r->name_resend_queue) && times++ < 100;
             q = ngx_queue_next(q))
        {
            rn = ngx_queue_data(q, ngx_resolver_ଗଣ୍ଠି_t, queue);

            ଯଦି (rn->query) {
                qident = (rn->query[0] << 8) + rn->query[1];

                ଯଦି (qident == ident) {
                    ଯାଅ dns_error_name;
                }
            }

#ଯଦି (NGX_HAVE_INET6)
            ଯଦି (rn->query6) {
                qident6 = (rn->query6[0] << 8) + rn->query6[1];

                ଯଦି (qident6 == ident) {
                    ଯାଅ dns_error_name;
                }
            }
#endif
        }

        ଯାଅ dns_error;
    }

    ଯଦି (code > NGX_RESOLVE_REFUSED) {
        ଯାଅ dns_error;
    }

    ଯଦି (nqs != 1) {
        err = "invalid number of questions in DNS response";
        ଯାଅ done;
    }

    i = ଆକାର(ngx_resolver_hdr_t);

    ଯେତେବେଳେ (i < (ngx_uint_t) n) {

        ଯଦି (buf[i] & 0xc0) {
            err = "unexpected compression pointer in DNS response";
            ଯାଅ done;
        }

        ଯଦି (buf[i] == '\0') {
            ଯାଅ found;
        }

        i += 1 + buf[i];
    }

    ଯାଅ short_response;

found:

    ଯଦି (i++ == ଆକାର(ngx_resolver_hdr_t)) {
        err = "zero-length domain name in DNS response";
        ଯାଅ done;
    }

    ଯଦି (i + ଆକାର(ngx_resolver_qs_t) + nan * (2 + ଆକାର(ngx_resolver_an_t))
        > (ngx_uint_t) n)
    {
        ଯାଅ short_response;
    }

    qs = (ngx_resolver_qs_t *) &buf[i];

    qtype = (qs->type_hi << 8) + qs->type_lo;
    qclass = (qs->class_hi << 8) + qs->class_lo;

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver DNS response qt:%ui cl:%ui", qtype, qclass);

    ଯଦି (qclass != 1) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unknown query class %ui in DNS response", qclass);
        ଫେରନ୍ତୁ;
    }

    ବାଛ (qtype) {

    ସ୍ଥିତି NGX_RESOLVE_A:
#ଯଦି (NGX_HAVE_INET6)
    ସ୍ଥିତି NGX_RESOLVE_AAAA:
#endif

        ngx_resolver_process_a(r, buf, n, ident, code, qtype, nan, trunc,
                               i + ଆକାର(ngx_resolver_qs_t));

        ଭାଙ୍ଗ;

    ସ୍ଥିତି NGX_RESOLVE_SRV:

        ngx_resolver_process_srv(r, buf, n, ident, code, nan, trunc,
                                 i + ଆକାର(ngx_resolver_qs_t));

        ଭାଙ୍ଗ;

    ସ୍ଥିତି NGX_RESOLVE_PTR:

        ngx_resolver_process_ptr(r, buf, n, ident, code, nan);

        ଭାଙ୍ଗ;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ngx_log_error(r->log_level, r->log, 0,
                      "unknown query type %ui in DNS response", qtype);
        ଫେରନ୍ତୁ;
    }

    ଫେରନ୍ତୁ;

short_response:

    err = "short DNS response";

done:

    ngx_log_error(r->log_level, r->log, 0, err);

    ଫେରନ୍ତୁ;

dns_error_name:

    ngx_log_error(r->log_level, r->log, 0,
                  "DNS error (%ui: %s), query id:%ui, name:\"%*s\"",
                  code, ngx_resolver_strerror(code), ident,
                  (size_t) rn->nlen, rn->name);
    ଫେରନ୍ତୁ;

dns_error:

    ngx_log_error(r->log_level, r->log, 0,
                  "DNS error (%ui: %s), query id:%ui",
                  code, ngx_resolver_strerror(code), ident);
    ଫେରନ୍ତୁ;
}


ସ୍ଥିର ଶୂନ୍ୟ
ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans)
{
    ଅକ୍ଷର                       *err;
    u_char                     *cname;
    size_t                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    in_addr_t                  *addr;
    ngx_str_t                   name;
    ngx_uint_t                  type, class, qident, naddrs, a, i, j, start;
#ଯଦି (NGX_HAVE_INET6)
    ସଂରଚନା in6_addr            *addr6;
#endif
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *next;
    ngx_resolver_ଗଣ୍ଠି_t        *rn;
    ngx_resolver_addr_t        *addrs;
    ngx_resolver_connection_t  *rec;

    ଯଦି (ngx_resolver_copy(r, &name, buf,
                          buf + ଆକାର(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        ଫେରନ୍ତୁ;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver qs:%V", &name);

    hash = ngx_crc32_short(name.data, name.len);

    /* lock name mutex */

    rn = ngx_resolver_lookup_name(r, &name, hash);

    ଯଦି (rn == ଖାଲି) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.data);
        ଯାଅ failed;
    }

    ବାଛ (qtype) {

#ଯଦି (NGX_HAVE_INET6)
    ସ୍ଥିତି NGX_RESOLVE_AAAA:

        ଯଦି (rn->query6 == ଖାଲି || rn->naddrs6 != (u_short) -1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.data);
            ଯାଅ failed;
        }

        ଯଦି (trunc && rn->tcp6) {
            ngx_resolver_free(r, name.data);
            ଯାଅ failed;
        }

        qident = (rn->query6[0] << 8) + rn->query6[1];

        ଭାଙ୍ଗ;
#endif

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: /* NGX_RESOLVE_A */

        ଯଦି (rn->query == ଖାଲି || rn->naddrs != (u_short) -1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.data);
            ଯାଅ failed;
        }

        ଯଦି (trunc && rn->tcp) {
            ngx_resolver_free(r, name.data);
            ଯାଅ failed;
        }

        qident = (rn->query[0] << 8) + rn->query[1];
    }

    ଯଦି (ident != qident) {
        ngx_log_error(r->log_level, r->log, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.data);
        ଯାଅ failed;
    }

    ngx_resolver_free(r, name.data);

    ଯଦି (trunc) {

        ngx_queue_remove(&rn->queue);

        ଯଦି (rn->waiting == ଖାଲି) {
            ngx_rbtree_delete(&r->name_rbtree, &rn->ଗଣ୍ଠି);
            ngx_resolver_free_ଗଣ୍ଠି(r, rn);
            ଯାଅ next;
        }

        rec = r->connections.elts;
        rec = &rec[rn->last_connection];

        ବାଛ (qtype) {

#ଯଦି (NGX_HAVE_INET6)
        ସ୍ଥିତି NGX_RESOLVE_AAAA:

            rn->tcp6 = 1;

            (ଶୂନ୍ୟ) ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen);

            ଭାଙ୍ଗ;
#endif

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ: /* NGX_RESOLVE_A */

            rn->tcp = 1;

            (ଶୂନ୍ୟ) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);
        }

        rn->expire = ngx_time() + r->resend_timeout;

        ngx_queue_ଭର୍ତ୍ତି_head(&r->name_resend_queue, &rn->queue);

        ଯାଅ next;
    }

    ଯଦି (code == 0 && rn->code) {
        code = rn->code;
    }

    ଯଦି (code == 0 && nan == 0) {

#ଯଦି (NGX_HAVE_INET6)
        ବାଛ (qtype) {

        ସ୍ଥିତି NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            ଯଦି (rn->naddrs == (u_short) -1) {
                ଯାଅ next;
            }

            ଯଦି (rn->naddrs) {
                ଯାଅ export;
            }

            ଭାଙ୍ଗ;

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            ଯଦି (rn->naddrs6 == (u_short) -1) {
                ଯାଅ next;
            }

            ଯଦି (rn->naddrs6) {
                ଯାଅ export;
            }
        }
#endif

        code = NGX_RESOLVE_NXDOMAIN;
    }

    ଯଦି (code) {

#ଯଦି (NGX_HAVE_INET6)
        ବାଛ (qtype) {

        ସ୍ଥିତି NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            ଯଦି (rn->naddrs == (u_short) -1) {
                rn->code = (u_char) code;
                ଯାଅ next;
            }

            ଭାଙ୍ଗ;

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            ଯଦି (rn->naddrs6 == (u_short) -1) {
                rn->code = (u_char) code;
                ଯାଅ next;
            }
        }
#endif

        next = rn->waiting;
        rn->waiting = ଖାଲି;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(&r->name_rbtree, &rn->ଗଣ୍ଠି);

        /* unlock name mutex */

        ଯେତେବେଳେ (next) {
            ctx = next;
            ctx->state = code;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            next = ctx->next;

            ctx->handler(ctx);
        }

        ngx_resolver_free_ଗଣ୍ଠି(r, rn);

        ଫେରନ୍ତୁ;
    }

    i = ans;
    naddrs = 0;
    cname = ଖାଲି;

    ପାଇଁ (a = 0; a < nan; a++) {

        start = i;

        ଯେତେବେଳେ (i < n) {

            ଯଦି (buf[i] & 0xc0) {
                i += 2;
                ଯାଅ found;
            }

            ଯଦି (buf[i] == 0) {
                i++;
                ଯାଅ test_length;
            }

            i += 1 + buf[i];
        }

        ଯାଅ short_response;

    test_length:

        ଯଦି (i - start < 2) {
            err = "invalid name in DNS response";
            ଯାଅ invalid;
        }

    found:

        ଯଦି (i + ଆକାର(ngx_resolver_an_t) >= n) {
            ଯାଅ short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        ଯଦି (class != 1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR class %ui in DNS response", class);
            ଯାଅ failed;
        }

        ଯଦି (ttl < 0) {
            ttl = 0;
        }

        rn->ttl = ngx_min(rn->ttl, (uint32_t) ttl);

        i += ଆକାର(ngx_resolver_an_t);

        ବାଛ (type) {

        ସ୍ଥିତି NGX_RESOLVE_A:

            ଯଦି (qtype != NGX_RESOLVE_A) {
                err = "unexpected A record in DNS response";
                ଯାଅ invalid;
            }

            ଯଦି (len != 4) {
                err = "invalid A record in DNS response";
                ଯାଅ invalid;
            }

            ଯଦି (i + 4 > n) {
                ଯାଅ short_response;
            }

            naddrs++;

            ଭାଙ୍ଗ;

#ଯଦି (NGX_HAVE_INET6)
        ସ୍ଥିତି NGX_RESOLVE_AAAA:

            ଯଦି (qtype != NGX_RESOLVE_AAAA) {
                err = "unexpected AAAA record in DNS response";
                ଯାଅ invalid;
            }

            ଯଦି (len != 16) {
                err = "invalid AAAA record in DNS response";
                ଯାଅ invalid;
            }

            ଯଦି (i + 16 > n) {
                ଯାଅ short_response;
            }

            naddrs++;

            ଭାଙ୍ଗ;
#endif

        ସ୍ଥିତି NGX_RESOLVE_CNAME:

            cname = &buf[i];

            ଭାଙ୍ଗ;

        ସ୍ଥିତି NGX_RESOLVE_DNAME:

            ଭାଙ୍ଗ;

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:

            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver naddrs:%ui cname:%p ttl:%uD",
                   naddrs, cname, rn->ttl);

    ଯଦି (naddrs) {

        ବାଛ (qtype) {

#ଯଦି (NGX_HAVE_INET6)
        ସ୍ଥିତି NGX_RESOLVE_AAAA:

            ଯଦି (naddrs == 1) {
                addr6 = &rn->u6.addr6;
                rn->naddrs6 = 1;

            } ଅନ୍ୟଥା {
                addr6 = ngx_resolver_alloc(r, naddrs * ଆକାର(ସଂରଚନା in6_addr));
                ଯଦି (addr6 == ଖାଲି) {
                    ଯାଅ failed;
                }

                rn->u6.addrs6 = addr6;
                rn->naddrs6 = (u_short) naddrs;
            }

#ଯଦି (NGX_SUPPRESS_WARN)
            addr = ଖାଲି;
#endif

            ଭାଙ୍ଗ;
#endif

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ: /* NGX_RESOLVE_A */

            ଯଦି (naddrs == 1) {
                addr = &rn->u.addr;
                rn->naddrs = 1;

            } ଅନ୍ୟଥା {
                addr = ngx_resolver_alloc(r, naddrs * ଆକାର(in_addr_t));
                ଯଦି (addr == ଖାଲି) {
                    ଯାଅ failed;
                }

                rn->u.addrs = addr;
                rn->naddrs = (u_short) naddrs;
            }

#ଯଦି (NGX_HAVE_INET6 && NGX_SUPPRESS_WARN)
            addr6 = ଖାଲି;
#endif
        }

        j = 0;
        i = ans;

        ପାଇଁ (a = 0; a < nan; a++) {

            ପାଇଁ ( ;; ) {

                ଯଦି (buf[i] & 0xc0) {
                    i += 2;
                    ଭାଙ୍ଗ;
                }

                ଯଦି (buf[i] == 0) {
                    i++;
                    ଭାଙ୍ଗ;
                }

                i += 1 + buf[i];
            }

            an = (ngx_resolver_an_t *) &buf[i];

            type = (an->type_hi << 8) + an->type_lo;
            len = (an->len_hi << 8) + an->len_lo;

            i += ଆକାର(ngx_resolver_an_t);

            ଯଦି (type == NGX_RESOLVE_A) {

                addr[j] = htonl((buf[i] << 24) + (buf[i + 1] << 16)
                                + (buf[i + 2] << 8) + (buf[i + 3]));

                ଯଦି (++j == naddrs) {

#ଯଦି (NGX_HAVE_INET6)
                    ଯଦି (rn->naddrs6 == (u_short) -1) {
                        ଯାଅ next;
                    }
#endif

                    ଭାଙ୍ଗ;
                }
            }

#ଯଦି (NGX_HAVE_INET6)
            ଅନ୍ୟଥା ଯଦି (type == NGX_RESOLVE_AAAA) {

                ngx_memcpy(addr6[j].s6_addr, &buf[i], 16);

                ଯଦି (++j == naddrs) {

                    ଯଦି (rn->naddrs == (u_short) -1) {
                        ଯାଅ next;
                    }

                    ଭାଙ୍ଗ;
                }
            }
#endif

            i += len;
        }
    }

    ବାଛ (qtype) {

#ଯଦି (NGX_HAVE_INET6)
    ସ୍ଥିତି NGX_RESOLVE_AAAA:

        ଯଦି (rn->naddrs6 == (u_short) -1) {
            rn->naddrs6 = 0;
        }

        ଭାଙ୍ଗ;
#endif

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: /* NGX_RESOLVE_A */

        ଯଦି (rn->naddrs == (u_short) -1) {
            rn->naddrs = 0;
        }
    }

    ଯଦି (rn->naddrs != (u_short) -1
#ଯଦି (NGX_HAVE_INET6)
        && rn->naddrs6 != (u_short) -1
#endif
        && rn->naddrs
#ଯଦି (NGX_HAVE_INET6)
           + rn->naddrs6
#endif
           > 0)
    {

#ଯଦି (NGX_HAVE_INET6)
    export:
#endif

        naddrs = rn->naddrs;
#ଯଦି (NGX_HAVE_INET6)
        naddrs += rn->naddrs6;
#endif

        ଯଦି (naddrs == 1 && rn->naddrs == 1) {
            addrs = ଖାଲି;

        } ଅନ୍ୟଥା {
            addrs = ngx_resolver_export(r, rn, 0);
            ଯଦି (addrs == ଖାଲି) {
                ଯାଅ failed;
            }
        }

        ngx_queue_remove(&rn->queue);

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_ଭର୍ତ୍ତି_head(&r->name_expire_queue, &rn->queue);

        next = rn->waiting;
        rn->waiting = ଖାଲି;

        /* unlock name mutex */

        ଯେତେବେଳେ (next) {
            ctx = next;
            ctx->state = NGX_OK;
            ctx->valid = rn->valid;
            ctx->naddrs = naddrs;

            ଯଦି (addrs == ଖାଲି) {
                ctx->addrs = &ctx->addr;
                ctx->addr.sockaddr = (ସଂରଚନା sockaddr *) &ctx->sin;
                ctx->addr.socklen = ଆକାର(ସଂରଚନା sockaddr_in);
                ngx_memzero(&ctx->sin, ଆକାର(ସଂରଚନା sockaddr_in));
                ctx->sin.sin_family = AF_INET;
                ctx->sin.sin_addr.s_addr = rn->u.addr;

            } ଅନ୍ୟଥା {
                ctx->addrs = addrs;
            }

            next = ctx->next;

            ctx->handler(ctx);
        }

        ଯଦି (addrs != ଖାଲି) {
            ngx_resolver_free(r, addrs->sockaddr);
            ngx_resolver_free(r, addrs);
        }

        ngx_resolver_free(r, rn->query);
        rn->query = ଖାଲି;
#ଯଦି (NGX_HAVE_INET6)
        rn->query6 = ଖାଲି;
#endif

        ଫେରନ୍ତୁ;
    }

    ଯଦି (cname) {

        /* CNAME only */

        ଯଦି (rn->naddrs == (u_short) -1
#ଯଦି (NGX_HAVE_INET6)
            || rn->naddrs6 == (u_short) -1
#endif
            )
        {
            ଯାଅ next;
        }

        ଯଦି (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            ଯାଅ failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_queue_remove(&rn->queue);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.data;

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_ଭର୍ତ୍ତି_head(&r->name_expire_queue, &rn->queue);

        ngx_resolver_free(r, rn->query);
        rn->query = ଖାଲି;
#ଯଦି (NGX_HAVE_INET6)
        rn->query6 = ଖାଲି;
#endif

        ctx = rn->waiting;
        rn->waiting = ଖାଲି;

        ଯଦି (ctx) {

            ଯଦି (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                କର {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    next = ctx->next;

                    ctx->handler(ctx);

                    ctx = next;
                } ଯେତେବେଳେ (ctx);

                ଫେରନ୍ତୁ;
            }

            ପାଇଁ (next = ctx; next; next = next->next) {
                next->ଗଣ୍ଠି = ଖାଲି;
            }

            (ଶୂନ୍ୟ) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        ଫେରନ୍ତୁ;
    }

    ngx_log_error(r->log_level, r->log, 0,
                  "no A or CNAME types in DNS response");
    ଫେରନ୍ତୁ;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->log, 0, err);

    ଫେରନ୍ତୁ;

failed:

next:

    /* unlock name mutex */

    ଫେରନ୍ତୁ;
}


ସ୍ଥିର ଶୂନ୍ୟ
ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans)
{
    ଅକ୍ଷର                       *err;
    u_char                     *cname;
    size_t                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    ngx_str_t                   name;
    ngx_uint_t                  type, qident, class, start, nsrvs, a, i, j;
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *next;
    ngx_resolver_srv_t         *srvs;
    ngx_resolver_ଗଣ୍ଠି_t        *rn;
    ngx_resolver_connection_t  *rec;

    ଯଦି (ngx_resolver_copy(r, &name, buf,
                          buf + ଆକାର(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        ଫେରନ୍ତୁ;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver qs:%V", &name);

    hash = ngx_crc32_short(name.data, name.len);

    rn = ngx_resolver_lookup_srv(r, &name, hash);

    ଯଦି (rn == ଖାଲି || rn->query == ଖାଲି) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.data);
        ଯାଅ failed;
    }

    ଯଦି (trunc && rn->tcp) {
        ngx_resolver_free(r, name.data);
        ଯାଅ failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    ଯଦି (ident != qident) {
        ngx_log_error(r->log_level, r->log, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.data);
        ଯାଅ failed;
    }

    ngx_resolver_free(r, name.data);

    ଯଦି (trunc) {

        ngx_queue_remove(&rn->queue);

        ଯଦି (rn->waiting == ଖାଲି) {
            ngx_rbtree_delete(&r->srv_rbtree, &rn->ଗଣ୍ଠି);
            ngx_resolver_free_ଗଣ୍ଠି(r, rn);
            ଫେରନ୍ତୁ;
        }

        rec = r->connections.elts;
        rec = &rec[rn->last_connection];

        rn->tcp = 1;

        (ଶୂନ୍ୟ) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);

        rn->expire = ngx_time() + r->resend_timeout;

        ngx_queue_ଭର୍ତ୍ତି_head(&r->srv_resend_queue, &rn->queue);

        ଫେରନ୍ତୁ;
    }

    ଯଦି (code == 0 && rn->code) {
        code = rn->code;
    }

    ଯଦି (code == 0 && nan == 0) {
        code = NGX_RESOLVE_NXDOMAIN;
    }

    ଯଦି (code) {
        next = rn->waiting;
        rn->waiting = ଖାଲି;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(&r->srv_rbtree, &rn->ଗଣ୍ଠି);

        ଯେତେବେଳେ (next) {
            ctx = next;
            ctx->state = code;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            next = ctx->next;

            ctx->handler(ctx);
        }

        ngx_resolver_free_ଗଣ୍ଠି(r, rn);

        ଫେରନ୍ତୁ;
    }

    i = ans;
    nsrvs = 0;
    cname = ଖାଲି;

    ପାଇଁ (a = 0; a < nan; a++) {

        start = i;

        ଯେତେବେଳେ (i < n) {

            ଯଦି (buf[i] & 0xc0) {
                i += 2;
                ଯାଅ found;
            }

            ଯଦି (buf[i] == 0) {
                i++;
                ଯାଅ test_length;
            }

            i += 1 + buf[i];
        }

        ଯାଅ short_response;

    test_length:

        ଯଦି (i - start < 2) {
            err = "invalid name DNS response";
            ଯାଅ invalid;
        }

    found:

        ଯଦି (i + ଆକାର(ngx_resolver_an_t) >= n) {
            ଯାଅ short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        ଯଦି (class != 1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR class %ui in DNS response", class);
            ଯାଅ failed;
        }

        ଯଦି (ttl < 0) {
            ttl = 0;
        }

        rn->ttl = ngx_min(rn->ttl, (uint32_t) ttl);

        i += ଆକାର(ngx_resolver_an_t);

        ବାଛ (type) {

        ସ୍ଥିତି NGX_RESOLVE_SRV:

            ଯଦି (i + 6 >= n) {
                ଯାଅ short_response;
            }

            ଯଦି (ngx_resolver_copy(r, ଖାଲି, buf, &buf[i + 6], buf + n)
                != NGX_OK)
            {
                ଯାଅ failed;
            }

            nsrvs++;

            ଭାଙ୍ଗ;

        ସ୍ଥିତି NGX_RESOLVE_CNAME:

            cname = &buf[i];

            ଭାଙ୍ଗ;

        ସ୍ଥିତି NGX_RESOLVE_DNAME:

            ଭାଙ୍ଗ;

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:

            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver nsrvs:%ui cname:%p ttl:%uD",
                   nsrvs, cname, rn->ttl);

    ଯଦି (nsrvs) {

        srvs = ngx_resolver_calloc(r, nsrvs * ଆକାର(ngx_resolver_srv_t));
        ଯଦି (srvs == ଖାଲି) {
            ଯାଅ failed;
        }

        rn->u.srvs = srvs;
        rn->nsrvs = (u_short) nsrvs;

        j = 0;
        i = ans;

        ପାଇଁ (a = 0; a < nan; a++) {

            ପାଇଁ ( ;; ) {

                ଯଦି (buf[i] & 0xc0) {
                    i += 2;
                    ଭାଙ୍ଗ;
                }

                ଯଦି (buf[i] == 0) {
                    i++;
                    ଭାଙ୍ଗ;
                }

                i += 1 + buf[i];
            }

            an = (ngx_resolver_an_t *) &buf[i];

            type = (an->type_hi << 8) + an->type_lo;
            len = (an->len_hi << 8) + an->len_lo;

            i += ଆକାର(ngx_resolver_an_t);

            ଯଦି (type == NGX_RESOLVE_SRV) {

                srvs[j].priority = (buf[i] << 8) + buf[i + 1];
                srvs[j].weight = (buf[i + 2] << 8) + buf[i + 3];

                ଯଦି (srvs[j].weight == 0) {
                    srvs[j].weight = 1;
                }

                srvs[j].port = (buf[i + 4] << 8) + buf[i + 5];

                ଯଦି (ngx_resolver_copy(r, &srvs[j].name, buf, &buf[i + 6],
                                      buf + n)
                    != NGX_OK)
                {
                    ଯାଅ failed;
                }

                j++;
            }

            i += len;
        }

        ngx_sort(srvs, nsrvs, ଆକାର(ngx_resolver_srv_t),
                 ngx_resolver_cmp_srvs);

        ngx_resolver_free(r, rn->query);
        rn->query = ଖାଲି;

        ngx_queue_remove(&rn->queue);

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_ଭର୍ତ୍ତି_head(&r->srv_expire_queue, &rn->queue);

        next = rn->waiting;
        rn->waiting = ଖାଲି;

        ଯେତେବେଳେ (next) {
            ctx = next;
            next = ctx->next;

            ngx_resolver_resolve_srv_names(ctx, rn);
        }

        ଫେରନ୍ତୁ;
    }

    rn->nsrvs = 0;

    ଯଦି (cname) {

        /* CNAME only */

        ଯଦି (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            ଯାଅ failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_queue_remove(&rn->queue);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.data;

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_ଭର୍ତ୍ତି_head(&r->srv_expire_queue, &rn->queue);

        ngx_resolver_free(r, rn->query);
        rn->query = ଖାଲି;
#ଯଦି (NGX_HAVE_INET6)
        rn->query6 = ଖାଲି;
#endif

        ctx = rn->waiting;
        rn->waiting = ଖାଲି;

        ଯଦି (ctx) {

            ଯଦି (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                କର {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    next = ctx->next;

                    ctx->handler(ctx);

                    ctx = next;
                } ଯେତେବେଳେ (ctx);

                ଫେରନ୍ତୁ;
            }

            ପାଇଁ (next = ctx; next; next = next->next) {
                next->ଗଣ୍ଠି = ଖାଲି;
            }

            (ଶୂନ୍ୟ) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        ଫେରନ୍ତୁ;
    }

    ngx_log_error(r->log_level, r->log, 0, "no SRV type in DNS response");

    ଫେରନ୍ତୁ;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->log, 0, err);

    ଫେରନ୍ତୁ;

failed:

    /* unlock name mutex */

    ଫେରନ୍ତୁ;
}


ସ୍ଥିର ଶୂନ୍ୟ
ngx_resolver_resolve_srv_names(ngx_resolver_ctx_t *ctx, ngx_resolver_ଗଣ୍ଠି_t *rn)
{
    ngx_uint_t                i;
    ngx_resolver_t           *r;
    ngx_resolver_ctx_t       *cctx;
    ngx_resolver_srv_name_t  *srvs;

    r = ctx->resolver;

    ctx->ଗଣ୍ଠି = ଖାଲି;
    ctx->state = NGX_OK;
    ctx->valid = rn->valid;
    ctx->ଗଣନା_2 = rn->nsrvs;

    srvs = ngx_resolver_calloc(r, rn->nsrvs * ଆକାର(ngx_resolver_srv_name_t));
    ଯଦି (srvs == ଖାଲି) {
        ଯାଅ failed;
    }

    ctx->srvs = srvs;
    ctx->nsrvs = rn->nsrvs;

    ଯଦି (ctx->event && ctx->event->timer_set) {
        ngx_del_timer(ctx->event);
    }

    ପାଇଁ (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
        srvs[i].name.data = ngx_resolver_alloc(r, rn->u.srvs[i].name.len);
        ଯଦି (srvs[i].name.data == ଖାଲି) {
            ଯାଅ failed;
        }

        srvs[i].name.len = rn->u.srvs[i].name.len;
        ngx_memcpy(srvs[i].name.data, rn->u.srvs[i].name.data,
                   srvs[i].name.len);

        cctx = ngx_resolve_start(r, ଖାଲି);
        ଯଦି (cctx == ଖାଲି) {
            ଯାଅ failed;
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

        ଯଦି (ngx_resolve_name(cctx) == NGX_ERROR) {
            srvs[i].ctx = ଖାଲି;
            ଯାଅ failed;
        }
    }

    ଫେରନ୍ତୁ;

failed:

    ctx->state = NGX_ERROR;
    ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

    ctx->handler(ctx);
}


ସ୍ଥିର ଶୂନ୍ୟ
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

    ctx->ଗଣନା_2--;
    ctx->async |= cctx->async;

    srv->ctx = ଖାଲି;
    srv->state = cctx->state;

    ଯଦି (cctx->naddrs) {

        ctx->valid = ngx_min(ctx->valid, cctx->valid);

        addrs = ngx_resolver_calloc(r, cctx->naddrs * ଆକାର(ngx_addr_t));
        ଯଦି (addrs == ଖାଲି) {
            srv->state = NGX_ERROR;
            ଯାଅ done;
        }

        sockaddr = ngx_resolver_alloc(r, cctx->naddrs * ଆକାର(ngx_sockaddr_t));
        ଯଦି (sockaddr == ଖାଲି) {
            ngx_resolver_free(r, addrs);
            srv->state = NGX_ERROR;
            ଯାଅ done;
        }

        ପାଇଁ (i = 0; i < cctx->naddrs; i++) {
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

    ଯଦି (ctx->ଗଣନା_2 == 0) {
        ngx_resolver_report_srv(r, ctx);
    }
}


ସ୍ଥିର ଶୂନ୍ୟ
ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan)
{
    ଅକ୍ଷର                 *err;
    size_t                len;
    in_addr_t             addr;
    int32_t               ttl;
    ngx_int_t             octet;
    ngx_str_t             name;
    ngx_uint_t            mask, type, class, qident, a, i, start;
    ngx_queue_t          *expire_queue;
    ngx_rbtree_t         *ବୃକ୍ଷ;
    ngx_resolver_an_t    *an;
    ngx_resolver_ctx_t   *ctx, *next;
    ngx_resolver_ଗଣ୍ଠି_t  *rn;
#ଯଦି (NGX_HAVE_INET6)
    uint32_t              hash;
    ngx_int_t             digit;
    ସଂରଚନା in6_addr       addr6;
#endif

    ଯଦି (ngx_resolver_copy(r, &name, buf,
                          buf + ଆକାର(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        ଫେରନ୍ତୁ;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver qs:%V", &name);

    /* AF_INET */

    addr = 0;
    i = ଆକାର(ngx_resolver_hdr_t);

    ପାଇଁ (mask = 0; mask < 32; mask += 8) {
        len = buf[i++];

        octet = ngx_atoi(&buf[i], len);
        ଯଦି (octet == NGX_ERROR || octet > 255) {
            ଯାଅ invalid_in_addr_arpa;
        }

        addr += octet << mask;
        i += len;
    }

    ଯଦି (ngx_strcasecmp(&buf[i], (u_char *) "\7in-addr\4arpa") == 0) {
        i += ଆକାର("\7in-addr\4arpa");

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        ବୃକ୍ଷ = &r->addr_rbtree;
        expire_queue = &r->addr_expire_queue;

        ଯାଅ valid;
    }

invalid_in_addr_arpa:

#ଯଦି (NGX_HAVE_INET6)

    i = ଆକାର(ngx_resolver_hdr_t);

    ପାଇଁ (octet = 15; octet >= 0; octet--) {
        ଯଦି (buf[i++] != '\1') {
            ଯାଅ invalid_ip6_arpa;
        }

        digit = ngx_hextoi(&buf[i++], 1);
        ଯଦି (digit == NGX_ERROR) {
            ଯାଅ invalid_ip6_arpa;
        }

        addr6.s6_addr[octet] = (u_char) digit;

        ଯଦି (buf[i++] != '\1') {
            ଯାଅ invalid_ip6_arpa;
        }

        digit = ngx_hextoi(&buf[i++], 1);
        ଯଦି (digit == NGX_ERROR) {
            ଯାଅ invalid_ip6_arpa;
        }

        addr6.s6_addr[octet] += (u_char) (digit * 16);
    }

    ଯଦି (ngx_strcasecmp(&buf[i], (u_char *) "\3ip6\4arpa") == 0) {
        i += ଆକାର("\3ip6\4arpa");

        /* lock addr mutex */

        hash = ngx_crc32_short(addr6.s6_addr, 16);
        rn = ngx_resolver_lookup_addr6(r, &addr6, hash);

        ବୃକ୍ଷ = &r->addr6_rbtree;
        expire_queue = &r->addr6_expire_queue;

        ଯାଅ valid;
    }

invalid_ip6_arpa:
#endif

    ngx_log_error(r->log_level, r->log, 0,
                  "invalid in-addr.arpa or ip6.arpa name in DNS response");
    ngx_resolver_free(r, name.data);
    ଫେରନ୍ତୁ;

valid:

    ଯଦି (rn == ଖାଲି || rn->query == ଖାଲି) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.data);
        ଯାଅ failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    ଯଦି (ident != qident) {
        ngx_log_error(r->log_level, r->log, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.data);
        ଯାଅ failed;
    }

    ngx_resolver_free(r, name.data);

    ଯଦି (code == 0 && nan == 0) {
        code = NGX_RESOLVE_NXDOMAIN;
    }

    ଯଦି (code) {
        next = rn->waiting;
        rn->waiting = ଖାଲି;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(ବୃକ୍ଷ, &rn->ଗଣ୍ଠି);

        /* unlock addr mutex */

        ଯେତେବେଳେ (next) {
            ctx = next;
            ctx->state = code;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            next = ctx->next;

            ctx->handler(ctx);
        }

        ngx_resolver_free_ଗଣ୍ଠି(r, rn);

        ଫେରନ୍ତୁ;
    }

    i += ଆକାର(ngx_resolver_qs_t);

    ପାଇଁ (a = 0; a < nan; a++) {

        start = i;

        ଯେତେବେଳେ (i < n) {

            ଯଦି (buf[i] & 0xc0) {
                i += 2;
                ଯାଅ found;
            }

            ଯଦି (buf[i] == 0) {
                i++;
                ଯାଅ test_length;
            }

            i += 1 + buf[i];
        }

        ଯାଅ short_response;

    test_length:

        ଯଦି (i - start < 2) {
            err = "invalid name in DNS response";
            ଯାଅ invalid;
        }

    found:

        ଯଦି (i + ଆକାର(ngx_resolver_an_t) >= n) {
            ଯାଅ short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        ଯଦି (class != 1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR class %ui in DNS response", class);
            ଯାଅ failed;
        }

        ଯଦି (ttl < 0) {
            ttl = 0;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                      "resolver qt:%ui cl:%ui len:%uz",
                      type, class, len);

        i += ଆକାର(ngx_resolver_an_t);

        ବାଛ (type) {

        ସ୍ଥିତି NGX_RESOLVE_PTR:

            ଯାଅ ptr;

        ସ୍ଥିତି NGX_RESOLVE_CNAME:

            ଭାଙ୍ଗ;

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:

            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->log, 0,
                  "no PTR type in DNS response");
    ଫେରନ୍ତୁ;

ptr:

    ଯଦି (ngx_resolver_copy(r, &name, buf, buf + i, buf + n) != NGX_OK) {
        ଯାଅ failed;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver an:%V", &name);

    ଯଦି (name.len != (size_t) rn->nlen
        || ngx_strncmp(name.data, rn->name, name.len) != 0)
    {
        ଯଦି (rn->nlen) {
            ngx_resolver_free(r, rn->name);
        }

        rn->nlen = (u_short) name.len;
        rn->name = name.data;

        name.data = ngx_resolver_dup(r, rn->name, name.len);
        ଯଦି (name.data == ଖାଲି) {
            ଯାଅ failed;
        }
    }

    ngx_queue_remove(&rn->queue);

    rn->valid = ngx_time() + (r->valid ? r->valid : ttl);
    rn->expire = ngx_time() + r->expire;

    ngx_queue_ଭର୍ତ୍ତି_head(expire_queue, &rn->queue);

    next = rn->waiting;
    rn->waiting = ଖାଲି;

    /* unlock addr mutex */

    ଯେତେବେଳେ (next) {
        ctx = next;
        ctx->state = NGX_OK;
        ctx->valid = rn->valid;
        ctx->name = name;
        next = ctx->next;

        ctx->handler(ctx);
    }

    ngx_resolver_free(r, name.data);

    ଫେରନ୍ତୁ;

short_response:

    err = "short DNS response";

invalid:

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->log, 0, err);

    ଫେରନ୍ତୁ;

failed:

    /* unlock addr mutex */

    ଫେରନ୍ତୁ;
}


ସ୍ଥିର ngx_resolver_ଗଣ୍ଠି_t *
ngx_resolver_lookup_name(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_ଗଣ୍ଠି_t    *ଗଣ୍ଠି, *sentinel;
    ngx_resolver_ଗଣ୍ଠି_t  *rn;

    ଗଣ୍ଠି = r->name_rbtree.ମୂଳ;
    sentinel = r->name_rbtree.sentinel;

    ଯେତେବେଳେ (ଗଣ୍ଠି != sentinel) {

        ଯଦି (hash < ଗଣ୍ଠି->ଚାବି) {
            ଗଣ୍ଠି = ଗଣ୍ଠି->ବାମ;
            ଜାରି;
        }

        ଯଦି (hash > ଗଣ୍ଠି->ଚାବି) {
            ଗଣ୍ଠି = ଗଣ୍ଠି->ଡାହାଣ;
            ଜାରି;
        }

        /* hash == node->key */

        rn = ngx_resolver_ଗଣ୍ଠି(ଗଣ୍ଠି);

        rc = ngx_memn2cmp(name->data, rn->name, name->len, rn->nlen);

        ଯଦି (rc == 0) {
            ଫେରନ୍ତୁ rn;
        }

        ଗଣ୍ଠି = (rc < 0) ? ଗଣ୍ଠି->ବାମ : ଗଣ୍ଠି->ଡାହାଣ;
    }

    /* not found */

    ଫେରନ୍ତୁ ଖାଲି;
}


ସ୍ଥିର ngx_resolver_ଗଣ୍ଠି_t *
ngx_resolver_lookup_srv(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_ଗଣ୍ଠି_t    *ଗଣ୍ଠି, *sentinel;
    ngx_resolver_ଗଣ୍ଠି_t  *rn;

    ଗଣ୍ଠି = r->srv_rbtree.ମୂଳ;
    sentinel = r->srv_rbtree.sentinel;

    ଯେତେବେଳେ (ଗଣ୍ଠି != sentinel) {

        ଯଦି (hash < ଗଣ୍ଠି->ଚାବି) {
            ଗଣ୍ଠି = ଗଣ୍ଠି->ବାମ;
            ଜାରି;
        }

        ଯଦି (hash > ଗଣ୍ଠି->ଚାବି) {
            ଗଣ୍ଠି = ଗଣ୍ଠି->ଡାହାଣ;
            ଜାରି;
        }

        /* hash == node->key */

        rn = ngx_resolver_ଗଣ୍ଠି(ଗଣ୍ଠି);

        rc = ngx_memn2cmp(name->data, rn->name, name->len, rn->nlen);

        ଯଦି (rc == 0) {
            ଫେରନ୍ତୁ rn;
        }

        ଗଣ୍ଠି = (rc < 0) ? ଗଣ୍ଠି->ବାମ : ଗଣ୍ଠି->ଡାହାଣ;
    }

    /* not found */

    ଫେରନ୍ତୁ ଖାଲି;
}


ସ୍ଥିର ngx_resolver_ଗଣ୍ଠି_t *
ngx_resolver_lookup_addr(ngx_resolver_t *r, in_addr_t addr)
{
    ngx_rbtree_ଗଣ୍ଠି_t  *ଗଣ୍ଠି, *sentinel;

    ଗଣ୍ଠି = r->addr_rbtree.ମୂଳ;
    sentinel = r->addr_rbtree.sentinel;

    ଯେତେବେଳେ (ଗଣ୍ଠି != sentinel) {

        ଯଦି (addr < ଗଣ୍ଠି->ଚାବି) {
            ଗଣ୍ଠି = ଗଣ୍ଠି->ବାମ;
            ଜାରି;
        }

        ଯଦି (addr > ଗଣ୍ଠି->ଚାବି) {
            ଗଣ୍ଠି = ଗଣ୍ଠି->ଡାହାଣ;
            ଜାରି;
        }

        /* addr == node->key */

        ଫେରନ୍ତୁ ngx_resolver_ଗଣ୍ଠି(ଗଣ୍ଠି);
    }

    /* not found */

    ଫେରନ୍ତୁ ଖାଲି;
}


#ଯଦି (NGX_HAVE_INET6)

ସ୍ଥିର ngx_resolver_ଗଣ୍ଠି_t *
ngx_resolver_lookup_addr6(ngx_resolver_t *r, ସଂରଚନା in6_addr *addr,
    uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_ଗଣ୍ଠି_t    *ଗଣ୍ଠି, *sentinel;
    ngx_resolver_ଗଣ୍ଠି_t  *rn;

    ଗଣ୍ଠି = r->addr6_rbtree.ମୂଳ;
    sentinel = r->addr6_rbtree.sentinel;

    ଯେତେବେଳେ (ଗଣ୍ଠି != sentinel) {

        ଯଦି (hash < ଗଣ୍ଠି->ଚାବି) {
            ଗଣ୍ଠି = ଗଣ୍ଠି->ବାମ;
            ଜାରି;
        }

        ଯଦି (hash > ଗଣ୍ଠି->ଚାବି) {
            ଗଣ୍ଠି = ଗଣ୍ଠି->ଡାହାଣ;
            ଜାରି;
        }

        /* hash == node->key */

        rn = ngx_resolver_ଗଣ୍ଠି(ଗଣ୍ଠି);

        rc = ngx_memcmp(addr, &rn->addr6, 16);

        ଯଦି (rc == 0) {
            ଫେରନ୍ତୁ rn;
        }

        ଗଣ୍ଠି = (rc < 0) ? ଗଣ୍ଠି->ବାମ : ଗଣ୍ଠି->ଡାହାଣ;
    }

    /* not found */

    ଫେରନ୍ତୁ ଖାଲି;
}

#endif


ସ୍ଥିର ଶୂନ୍ୟ
ngx_resolver_rbtree_ଭର୍ତ୍ତି_ମୂଲ୍ୟ(ngx_rbtree_ଗଣ୍ଠି_t *temp,
    ngx_rbtree_ଗଣ୍ଠି_t *ଗଣ୍ଠି, ngx_rbtree_ଗଣ୍ଠି_t *sentinel)
{
    ngx_rbtree_ଗଣ୍ଠି_t    **p;
    ngx_resolver_ଗଣ୍ଠି_t   *rn, *rn_temp;

    ପାଇଁ ( ;; ) {

        ଯଦି (ଗଣ୍ଠି->ଚାବି < temp->ଚାବି) {

            p = &temp->ବାମ;

        } ଅନ୍ୟଥା ଯଦି (ଗଣ୍ଠି->ଚାବି > temp->ଚାବି) {

            p = &temp->ଡାହାଣ;

        } ଅନ୍ୟଥା { /* node->key == temp->key */

            rn = ngx_resolver_ଗଣ୍ଠି(ଗଣ୍ଠି);
            rn_temp = ngx_resolver_ଗଣ୍ଠି(temp);

            p = (ngx_memn2cmp(rn->name, rn_temp->name, rn->nlen, rn_temp->nlen)
                 < 0) ? &temp->ବାମ : &temp->ଡାହାଣ;
        }

        ଯଦି (*p == sentinel) {
            ଭାଙ୍ଗ;
        }

        temp = *p;
    }

    *p = ଗଣ୍ଠି;
    ଗଣ୍ଠି->parent = temp;
    ଗଣ୍ଠି->ବାମ = sentinel;
    ଗଣ୍ଠି->ଡାହାଣ = sentinel;
    ngx_rbt_red(ଗଣ୍ଠି);
}


#ଯଦି (NGX_HAVE_INET6)

ସ୍ଥିର ଶୂନ୍ୟ
ngx_resolver_rbtree_ଭର୍ତ୍ତି_addr6_ମୂଲ୍ୟ(ngx_rbtree_ଗଣ୍ଠି_t *temp,
    ngx_rbtree_ଗଣ୍ଠି_t *ଗଣ୍ଠି, ngx_rbtree_ଗଣ୍ଠି_t *sentinel)
{
    ngx_rbtree_ଗଣ୍ଠି_t    **p;
    ngx_resolver_ଗଣ୍ଠି_t   *rn, *rn_temp;

    ପାଇଁ ( ;; ) {

        ଯଦି (ଗଣ୍ଠି->ଚାବି < temp->ଚାବି) {

            p = &temp->ବାମ;

        } ଅନ୍ୟଥା ଯଦି (ଗଣ୍ଠି->ଚାବି > temp->ଚାବି) {

            p = &temp->ଡାହାଣ;

        } ଅନ୍ୟଥା { /* node->key == temp->key */

            rn = ngx_resolver_ଗଣ୍ଠି(ଗଣ୍ଠି);
            rn_temp = ngx_resolver_ଗଣ୍ଠି(temp);

            p = (ngx_memcmp(&rn->addr6, &rn_temp->addr6, 16)
                 < 0) ? &temp->ବାମ : &temp->ଡାହାଣ;
        }

        ଯଦି (*p == sentinel) {
            ଭାଙ୍ଗ;
        }

        temp = *p;
    }

    *p = ଗଣ୍ଠି;
    ଗଣ୍ଠି->parent = temp;
    ଗଣ୍ଠି->ବାମ = sentinel;
    ଗଣ୍ଠି->ଡାହାଣ = sentinel;
    ngx_rbt_red(ଗଣ୍ଠି);
}

#endif


ସ୍ଥିର ngx_int_t
ngx_resolver_create_name_query(ngx_resolver_t *r, ngx_resolver_ଗଣ୍ଠି_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    size_t               len, nlen;
    ngx_uint_t           ident;
    ngx_resolver_qs_t   *qs;
    ngx_resolver_hdr_t  *query;

    nlen = name->len ? (1 + name->len + 1) : 1;

    len = ଆକାର(ngx_resolver_hdr_t) + nlen + ଆକାର(ngx_resolver_qs_t);

#ଯଦି (NGX_HAVE_INET6)
    p = ngx_resolver_alloc(r, len * (r->ipv4 + r->ipv6));
#ଅନ୍ୟଥା
    p = ngx_resolver_alloc(r, len);
#endif
    ଯଦି (p == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    rn->qlen = (u_short) len;
    rn->query = p;

#ଯଦି (NGX_HAVE_INET6)
    ଯଦି (r->ipv6) {
        rn->query6 = r->ipv4 ? (p + len) : p;
    }
#endif

    query = (ngx_resolver_hdr_t *) p;

    ଯଦି (r->ipv4) {
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

    p += ଆକାର(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    /* query type */
    qs->type_hi = 0; qs->type_lo = NGX_RESOLVE_A;

    /* IN query class */
    qs->class_hi = 0; qs->class_lo = 1;

    /* convert "www.example.com" to "\3www\7example\3com\0" */

    len = 0;
    p--;
    *p-- = '\0';

    ଯଦି (name->len == 0)  {
        ଫେରନ୍ତୁ NGX_DECLINED;
    }

    ପାଇଁ (s = name->data + name->len - 1; s >= name->data; s--) {
        ଯଦି (*s != '.') {
            *p = *s;
            len++;

        } ଅନ୍ୟଥା {
            ଯଦି (len == 0 || len > 255) {
                ଫେରନ୍ତୁ NGX_DECLINED;
            }

            *p = (u_char) len;
            len = 0;
        }

        p--;
    }

    ଯଦି (len == 0 || len > 255) {
        ଫେରନ୍ତୁ NGX_DECLINED;
    }

    *p = (u_char) len;

#ଯଦି (NGX_HAVE_INET6)
    ଯଦି (!r->ipv6) {
        ଫେରନ୍ତୁ NGX_OK;
    }

    p = rn->query6;

    ଯଦି (r->ipv4) {
        ngx_memcpy(p, rn->query, rn->qlen);
    }

    query = (ngx_resolver_hdr_t *) p;

    ident = ngx_random();

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve: \"%V\" AAAA %i", name, ident & 0xffff);

    query->ident_hi = (u_char) ((ident >> 8) & 0xff);
    query->ident_lo = (u_char) (ident & 0xff);

    p += ଆକାର(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    qs->type_lo = NGX_RESOLVE_AAAA;
#endif

    ଫେରନ୍ତୁ NGX_OK;
}


ସ୍ଥିର ngx_int_t
ngx_resolver_create_srv_query(ngx_resolver_t *r, ngx_resolver_ଗଣ୍ଠି_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    size_t               len, nlen;
    ngx_uint_t           ident;
    ngx_resolver_qs_t   *qs;
    ngx_resolver_hdr_t  *query;

    nlen = name->len ? (1 + name->len + 1) : 1;

    len = ଆକାର(ngx_resolver_hdr_t) + nlen + ଆକାର(ngx_resolver_qs_t);

    p = ngx_resolver_alloc(r, len);
    ଯଦି (p == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
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

    p += ଆକାର(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    /* query type */
    qs->type_hi = 0; qs->type_lo = NGX_RESOLVE_SRV;

    /* IN query class */
    qs->class_hi = 0; qs->class_lo = 1;

    /* converts "www.example.com" to "\3www\7example\3com\0" */

    len = 0;
    p--;
    *p-- = '\0';

    ଯଦି (name->len == 0)  {
        ଫେରନ୍ତୁ NGX_DECLINED;
    }

    ପାଇଁ (s = name->data + name->len - 1; s >= name->data; s--) {
        ଯଦି (*s != '.') {
            *p = *s;
            len++;

        } ଅନ୍ୟଥା {
            ଯଦି (len == 0 || len > 255) {
                ଫେରନ୍ତୁ NGX_DECLINED;
            }

            *p = (u_char) len;
            len = 0;
        }

        p--;
    }

    ଯଦି (len == 0 || len > 255) {
        ଫେରନ୍ତୁ NGX_DECLINED;
    }

    *p = (u_char) len;

    ଫେରନ୍ତୁ NGX_OK;
}


ସ୍ଥିର ngx_int_t
ngx_resolver_create_addr_query(ngx_resolver_t *r, ngx_resolver_ଗଣ୍ଠି_t *rn,
    ngx_resolver_addr_t *addr)
{
    u_char               *p, *d;
    size_t                len;
    in_addr_t             inaddr;
    ngx_int_t             n;
    ngx_uint_t            ident;
    ngx_resolver_hdr_t   *query;
    ସଂରଚନା sockaddr_in   *sin;
#ଯଦି (NGX_HAVE_INET6)
    ସଂରଚନା sockaddr_in6  *sin6;
#endif

    ବାଛ (addr->sockaddr->sa_family) {

#ଯଦି (NGX_HAVE_INET6)
    ସ୍ଥିତି AF_INET6:
        len = ଆକାର(ngx_resolver_hdr_t)
              + 64 + ଆକାର(".ip6.arpa.") - 1
              + ଆକାର(ngx_resolver_qs_t);

        ଭାଙ୍ଗ;
#endif

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: /* AF_INET */
        len = ଆକାର(ngx_resolver_hdr_t)
              + ଆକାର(".255.255.255.255.in-addr.arpa.") - 1
              + ଆକାର(ngx_resolver_qs_t);
    }

    p = ngx_resolver_alloc(r, len);
    ଯଦି (p == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
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

    p += ଆକାର(ngx_resolver_hdr_t);

    ବାଛ (addr->sockaddr->sa_family) {

#ଯଦି (NGX_HAVE_INET6)
    ସ୍ଥିତି AF_INET6:
        sin6 = (ସଂରଚନା sockaddr_in6 *) addr->sockaddr;

        ପାଇଁ (n = 15; n >= 0; n--) {
            p = ngx_sprintf(p, "\1%xd\1%xd",
                            sin6->sin6_addr.s6_addr[n] & 0xf,
                            (sin6->sin6_addr.s6_addr[n] >> 4) & 0xf);
        }

        p = ngx_cpymem(p, "\3ip6\4arpa\0", 10);

        ଭାଙ୍ଗ;
#endif

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: /* AF_INET */

        sin = (ସଂରଚନା sockaddr_in *) addr->sockaddr;
        inaddr = ntohl(sin->sin_addr.s_addr);

        ପାଇଁ (n = 0; n < 32; n += 8) {
            d = ngx_sprintf(&p[1], "%ud", (inaddr >> n) & 0xff);
            *p = (u_char) (d - &p[1]);
            p = d;
        }

        p = ngx_cpymem(p, "\7in-addr\4arpa\0", 14);
    }

    /* query type "PTR", IN query class */
    p = ngx_cpymem(p, "\0\14\0\1", 4);

    rn->qlen = (u_short) (p - rn->query);

    ଫେରନ୍ତୁ NGX_OK;
}


ସ୍ଥିର ngx_int_t
ngx_resolver_copy(ngx_resolver_t *r, ngx_str_t *name, u_char *buf, u_char *src,
    u_char *last)
{
    ଅକ୍ଷର        *err;
    u_char      *p, *dst;
    size_t       len;
    ngx_uint_t   i, n;

    p = src;
    len = 0;

    /*
     * compression pointers allow to create endless loop, so we set limit;
     * 128 pointers should be enough to store 255-byte name
     */

    ପାଇଁ (i = 0; i < 128; i++) {
        n = *p++;

        ଯଦି (n == 0) {
            ଯାଅ done;
        }

        ଯଦି (n & 0xc0) {
            ଯଦି ((n & 0xc0) != 0xc0) {
                err = "invalid label type in DNS response";
                ଯାଅ invalid;
            }

            ଯଦି (p >= last) {
                err = "name is out of DNS response";
                ଯାଅ invalid;
            }

            n = ((n & 0x3f) << 8) + *p;
            p = &buf[n];

        } ଅନ୍ୟଥା {
            len += 1 + n;
            p = &p[n];
        }

        ଯଦି (p >= last) {
            err = "name is out of DNS response";
            ଯାଅ invalid;
        }
    }

    err = "compression pointers loop in DNS response";

invalid:

    ngx_log_error(r->log_level, r->log, 0, err);

    ଫେରନ୍ତୁ NGX_ERROR;

done:

    ଯଦି (name == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_OK;
    }

    ଯଦି (len == 0) {
        ngx_str_null(name);
        ଫେରନ୍ତୁ NGX_OK;
    }

    dst = ngx_resolver_alloc(r, len);
    ଯଦି (dst == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    name->data = dst;

    ପାଇଁ ( ;; ) {
        n = *src++;

        ଯଦି (n == 0) {
            name->len = dst - name->data - 1;
            ଫେରନ୍ତୁ NGX_OK;
        }

        ଯଦି (n & 0xc0) {
            n = ((n & 0x3f) << 8) + *src;
            src = &buf[n];

        } ଅନ୍ୟଥା {
            ngx_strlow(dst, src, n);
            dst += n;
            src += n;
            *dst++ = '.';
        }
    }
}


ସ୍ଥିର ngx_int_t
ngx_resolver_set_timeout(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx)
{
    ଯଦି (ctx->event || ctx->timeout == 0) {
        ଫେରନ୍ତୁ NGX_OK;
    }

    ctx->event = ngx_resolver_calloc(r, ଆକାର(ngx_event_t));
    ଯଦି (ctx->event == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ctx->event->handler = ngx_resolver_timeout_handler;
    ctx->event->data = ctx;
    ctx->event->log = r->log;
    ctx->event->cancelable = ctx->cancelable;
    ctx->ident = -1;

    ngx_add_timer(ctx->event, ctx->timeout);

    ଫେରନ୍ତୁ NGX_OK;
}


ସ୍ଥିର ଶୂନ୍ୟ
ngx_resolver_timeout_handler(ngx_event_t *ev)
{
    ngx_resolver_ctx_t  *ctx;

    ctx = ev->data;

    ctx->state = NGX_RESOLVE_TIMEDOUT;

    ctx->handler(ctx);
}


ସ୍ଥିର ଶୂନ୍ୟ
ngx_resolver_free_ଗଣ୍ଠି(ngx_resolver_t *r, ngx_resolver_ଗଣ୍ଠି_t *rn)
{
    ngx_uint_t  i;

    /* lock alloc mutex */

    ଯଦି (rn->query) {
        ngx_resolver_free_locked(r, rn->query);
    }

    ଯଦି (rn->name) {
        ngx_resolver_free_locked(r, rn->name);
    }

    ଯଦି (rn->cnlen) {
        ngx_resolver_free_locked(r, rn->u.cname);
    }

    ଯଦି (rn->naddrs > 1 && rn->naddrs != (u_short) -1) {
        ngx_resolver_free_locked(r, rn->u.addrs);
    }

#ଯଦି (NGX_HAVE_INET6)
    ଯଦି (rn->naddrs6 > 1 && rn->naddrs6 != (u_short) -1) {
        ngx_resolver_free_locked(r, rn->u6.addrs6);
    }
#endif

    ଯଦି (rn->nsrvs) {
        ପାଇଁ (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
            ଯଦି (rn->u.srvs[i].name.data) {
                ngx_resolver_free_locked(r, rn->u.srvs[i].name.data);
            }
        }

        ngx_resolver_free_locked(r, rn->u.srvs);
    }

    ngx_resolver_free_locked(r, rn);

    /* unlock alloc mutex */
}


ସ୍ଥିର ଶୂନ୍ୟ *
ngx_resolver_alloc(ngx_resolver_t *r, size_t size)
{
    u_char  *p;

    /* lock alloc mutex */

    p = ngx_alloc(size, r->log);

    /* unlock alloc mutex */

    ଫେରନ୍ତୁ p;
}


ସ୍ଥିର ଶୂନ୍ୟ *
ngx_resolver_calloc(ngx_resolver_t *r, size_t size)
{
    u_char  *p;

    p = ngx_resolver_alloc(r, size);

    ଯଦି (p) {
        ngx_memzero(p, size);
    }

    ଫେରନ୍ତୁ p;
}


ସ୍ଥିର ଶୂନ୍ୟ
ngx_resolver_free(ngx_resolver_t *r, ଶୂନ୍ୟ *p)
{
    /* lock alloc mutex */

    ngx_free(p);

    /* unlock alloc mutex */
}


ସ୍ଥିର ଶୂନ୍ୟ
ngx_resolver_free_locked(ngx_resolver_t *r, ଶୂନ୍ୟ *p)
{
    ngx_free(p);
}


ସ୍ଥିର ଶୂନ୍ୟ *
ngx_resolver_dup(ngx_resolver_t *r, ଶୂନ୍ୟ *src, size_t size)
{
    ଶୂନ୍ୟ  *dst;

    dst = ngx_resolver_alloc(r, size);

    ଯଦି (dst == ଖାଲି) {
        ଫେରନ୍ତୁ dst;
    }

    ngx_memcpy(dst, src, size);

    ଫେରନ୍ତୁ dst;
}


ସ୍ଥିର ngx_resolver_addr_t *
ngx_resolver_export(ngx_resolver_t *r, ngx_resolver_ଗଣ୍ଠି_t *rn,
    ngx_uint_t rotate)
{
    ngx_uint_t            d, i, j, n;
    in_addr_t            *addr;
    ngx_sockaddr_t       *sockaddr;
    ସଂରଚନା sockaddr_in   *sin;
    ngx_resolver_addr_t  *dst;
#ଯଦି (NGX_HAVE_INET6)
    ସଂରଚନା in6_addr      *addr6;
    ସଂରଚନା sockaddr_in6  *sin6;
#endif

    n = rn->naddrs;
#ଯଦି (NGX_HAVE_INET6)
    n += rn->naddrs6;
#endif

    dst = ngx_resolver_calloc(r, n * ଆକାର(ngx_resolver_addr_t));
    ଯଦି (dst == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    sockaddr = ngx_resolver_calloc(r, n * ଆକାର(ngx_sockaddr_t));
    ଯଦି (sockaddr == ଖାଲି) {
        ngx_resolver_free(r, dst);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    i = 0;
    d = rotate ? ngx_random() % n : 0;

    ଯଦି (rn->naddrs) {
        j = rotate ? ngx_random() % rn->naddrs : 0;

        addr = (rn->naddrs == 1) ? &rn->u.addr : rn->u.addrs;

        କର {
            sin = &sockaddr[d].sockaddr_in;
            sin->sin_family = AF_INET;
            sin->sin_addr.s_addr = addr[j++];
            dst[d].sockaddr = (ସଂରଚନା sockaddr *) sin;
            dst[d++].socklen = ଆକାର(ସଂରଚନା sockaddr_in);

            ଯଦି (d == n) {
                d = 0;
            }

            ଯଦି (j == (ngx_uint_t) rn->naddrs) {
                j = 0;
            }
        } ଯେତେବେଳେ (++i < (ngx_uint_t) rn->naddrs);
    }

#ଯଦି (NGX_HAVE_INET6)
    ଯଦି (rn->naddrs6) {
        j = rotate ? ngx_random() % rn->naddrs6 : 0;

        addr6 = (rn->naddrs6 == 1) ? &rn->u6.addr6 : rn->u6.addrs6;

        କର {
            sin6 = &sockaddr[d].sockaddr_in6;
            sin6->sin6_family = AF_INET6;
            ngx_memcpy(sin6->sin6_addr.s6_addr, addr6[j++].s6_addr, 16);
            dst[d].sockaddr = (ସଂରଚନା sockaddr *) sin6;
            dst[d++].socklen = ଆକାର(ସଂରଚନା sockaddr_in6);

            ଯଦି (d == n) {
                d = 0;
            }

            ଯଦି (j == rn->naddrs6) {
                j = 0;
            }
        } ଯେତେବେଳେ (++i < n);
    }
#endif

    ଫେରନ୍ତୁ dst;
}


ସ୍ଥିର ଶୂନ୍ୟ
ngx_resolver_report_srv(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx)
{
    ngx_uint_t                naddrs, nsrvs, nw, i, j, k, l, m, n, w;
    ngx_resolver_addr_t      *addrs;
    ngx_resolver_srv_name_t  *srvs;

    srvs = ctx->srvs;
    nsrvs = ctx->nsrvs;

    naddrs = 0;

    ପାଇଁ (i = 0; i < nsrvs; i++) {
        ଯଦି (srvs[i].state == NGX_ERROR) {
            ctx->state = NGX_ERROR;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

            ctx->handler(ctx);
            ଫେରନ୍ତୁ;
        }

        naddrs += srvs[i].naddrs;
    }

    ଯଦି (naddrs == 0) {
        ctx->state = srvs[0].state;

        ପାଇଁ (i = 0; i < nsrvs; i++) {
            ଯଦି (srvs[i].state == NGX_RESOLVE_NXDOMAIN) {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                ଭାଙ୍ଗ;
            }
        }

        ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        ଫେରନ୍ତୁ;
    }

    addrs = ngx_resolver_calloc(r, naddrs * ଆକାର(ngx_resolver_addr_t));
    ଯଦି (addrs == ଖାଲି) {
        ctx->state = NGX_ERROR;
        ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        ଫେରନ୍ତୁ;
    }

    i = 0;
    n = 0;

    କର {
        nw = 0;

        ପାଇଁ (j = i; j < nsrvs; j++) {
            ଯଦି (srvs[j].priority != srvs[i].priority) {
                ଭାଙ୍ଗ;
            }

            nw += srvs[j].naddrs * srvs[j].weight;
        }

        ଯଦି (nw == 0) {
            ଯାଅ next_srv;
        }

        w = ngx_random() % nw;

        ପାଇଁ (k = i; k < j; k++) {
            ଯଦି (w < srvs[k].naddrs * srvs[k].weight) {
                ଭାଙ୍ଗ;
            }

            w -= srvs[k].naddrs * srvs[k].weight;
        }

        ପାଇଁ (l = i; l < j; l++) {

            ପାଇଁ (m = 0; m < srvs[k].naddrs; m++) {
                addrs[n].socklen = srvs[k].addrs[m].socklen;
                addrs[n].sockaddr = srvs[k].addrs[m].sockaddr;
                addrs[n].name = srvs[k].name;
                addrs[n].priority = srvs[k].priority;
                addrs[n].weight = srvs[k].weight;
                n++;
            }

            ଯଦି (++k == j) {
                k = i;
            }
        }

next_srv:

        i = j;

    } ଯେତେବେଳେ (i < ctx->nsrvs);

    ctx->state = NGX_OK;
    ctx->addrs = addrs;
    ctx->naddrs = naddrs;

    ctx->handler(ctx);

    ngx_resolver_free(r, addrs);
}


ଅକ୍ଷର *
ngx_resolver_strerror(ngx_int_t err)
{
    ସ୍ଥିର ଅକ୍ଷର *errors[] = {
        "Format error",     /* FORMERR */
        "Server failure",   /* SERVFAIL */
        "Host not found",   /* NXDOMAIN */
        "Unimplemented",    /* NOTIMP */
        "Operation refused" /* REFUSED */
    };

    ଯଦି (err > 0 && err < 6) {
        ଫେରନ୍ତୁ errors[err - 1];
    }

    ଯଦି (err == NGX_RESOLVE_TIMEDOUT) {
        ଫେରନ୍ତୁ "Operation timed out";
    }

    ଫେରନ୍ତୁ "Unknown error";
}


ସ୍ଥିର u_char *
ngx_resolver_log_error(ngx_log_t *log, u_char *buf, size_t len)
{
    u_char                     *p;
    ngx_resolver_connection_t  *rec;

    p = buf;

    ଯଦି (log->action) {
        p = ngx_snprintf(buf, len, " while %s", log->action);
        len -= p - buf;
    }

    rec = log->data;

    ଯଦି (rec) {
        p = ngx_snprintf(p, len, ", resolver: %V", &rec->server);
    }

    ଫେରନ୍ତୁ p;
}


ସ୍ଥିର ngx_int_t
ngx_udp_connect(ngx_resolver_connection_t *rec)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା                rc;
    ngx_int_t          event;
    ngx_event_t       *rev, *wev;
    ngx_socket_t       s;
    ngx_connection_t  *c;

    s = ngx_socket(rec->sockaddr->sa_family, SOCK_DGRAM, 0);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "UDP socket %d", s);

    ଯଦି (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_socket_n " failed");
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->log);

    ଯଦି (c == ଖାଲି) {
        ଯଦି (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ଯଦି (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        ଯାଅ failed;
    }

    rev = c->ପଠ;
    wev = c->ଲିଖ;

    rev->log = &rec->log;
    wev->log = &rec->log;

    rec->udp = c;

    c->ସଂଖ୍ୟା = ngx_atomic_fetch_add(ngx_connection_counter, 1);

    c->start_time = ngx_current_msec;

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->log, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->ସଂଖ୍ୟା);

    rc = connect(s, rec->sockaddr, rec->socklen);

    ଯଦି (rc == -1) {
        ngx_log_error(NGX_LOG_CRIT, &rec->log, ngx_socket_errno,
                      "connect() failed");

        ଯାଅ failed;
    }

    /* UDP sockets are always ready to write */
    wev->ready = 1;

    event = (ngx_event_flags & NGX_USE_CLEAR_EVENT) ?
                /* kqueue, epoll */                 NGX_CLEAR_EVENT:
                /* select, poll, /dev/poll */       NGX_LEVEL_EVENT;
                /* eventport event type has no meaning: oneshot only */

    ଯଦି (ngx_add_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        ଯାଅ failed;
    }

    ଫେରନ୍ତୁ NGX_OK;

failed:

    ngx_close_connection(c);
    rec->udp = ଖାଲି;

    ଫେରନ୍ତୁ NGX_ERROR;
}


ସ୍ଥିର ngx_int_t
ngx_tcp_connect(ngx_resolver_connection_t *rec)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା                rc;
    ngx_int_t          event;
    ngx_err_t          err;
    ngx_uint_t         level;
    ngx_socket_t       s;
    ngx_event_t       *rev, *wev;
    ngx_connection_t  *c;

    s = ngx_socket(rec->sockaddr->sa_family, SOCK_STREAM, 0);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "TCP socket %d", s);

    ଯଦି (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_socket_n " failed");
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->log);

    ଯଦି (c == ଖାଲି) {
        ଯଦି (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ଯଦି (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        ଯାଅ failed;
    }

    rev = c->ପଠ;
    wev = c->ଲିଖ;

    rev->log = &rec->log;
    wev->log = &rec->log;

    rec->tcp = c;

    c->ସଂଖ୍ୟା = ngx_atomic_fetch_add(ngx_connection_counter, 1);

    c->start_time = ngx_current_msec;

    ଯଦି (ngx_add_conn) {
        ଯଦି (ngx_add_conn(c) == NGX_ERROR) {
            ଯାଅ failed;
        }
    }

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->log, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->ସଂଖ୍ୟା);

    rc = connect(s, rec->sockaddr, rec->socklen);

    ଯଦି (rc == -1) {
        err = ngx_socket_errno;


        ଯଦି (err != NGX_EINPROGRESS
#ଯଦି (NGX_WIN32)
            /* Winsock returns WSAEWOULDBLOCK (NGX_EAGAIN) */
            && err != NGX_EAGAIN
#endif
            )
        {
            ଯଦି (err == NGX_ECONNREFUSED
#ଯଦି (NGX_LINUX)
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

            } ଅନ୍ୟଥା {
                level = NGX_LOG_CRIT;
            }

            ngx_log_error(level, &rec->log, err, "connect() to %V failed",
                          &rec->server);

            ngx_close_connection(c);
            rec->tcp = ଖାଲି;

            ଫେରନ୍ତୁ NGX_ERROR;
        }
    }

    ଯଦି (ngx_add_conn) {
        ଯଦି (rc == -1) {

            /* NGX_EINPROGRESS */

            ଫେରନ୍ତୁ NGX_AGAIN;
        }

        ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "connected");

        wev->ready = 1;

        ଫେରନ୍ତୁ NGX_OK;
    }

    ଯଦି (ngx_event_flags & NGX_USE_CLEAR_EVENT) {

        /* kqueue */

        event = NGX_CLEAR_EVENT;

    } ଅନ୍ୟଥା {

        /* select, poll, /dev/poll */

        event = NGX_LEVEL_EVENT;
    }

    ଯଦି (ngx_add_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        ଯାଅ failed;
    }

    ଯଦି (rc == -1) {

        /* NGX_EINPROGRESS */

        ଯଦି (ngx_add_event(wev, NGX_WRITE_EVENT, event) != NGX_OK) {
            ଯାଅ failed;
        }

        ଫେରନ୍ତୁ NGX_AGAIN;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "connected");

    wev->ready = 1;

    ଫେରନ୍ତୁ NGX_OK;

failed:

    ngx_close_connection(c);
    rec->tcp = ଖାଲି;

    ଫେରନ୍ତୁ NGX_ERROR;
}


ସ୍ଥିର ngx_int_t
ngx_resolver_cmp_srvs(ଅଚଳ ଶୂନ୍ୟ *one, ଅଚଳ ଶୂନ୍ୟ *two)
{
    ngx_int_t            p1, p2;
    ngx_resolver_srv_t  *first, *second;

    first = (ngx_resolver_srv_t *) one;
    second = (ngx_resolver_srv_t *) two;

    p1 = first->priority;
    p2 = second->priority;

    ଫେରନ୍ତୁ p1 - p2;
}
