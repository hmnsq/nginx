
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>
#অন্তর্ভাব <ngx_event.h>


#define NGX_RESOLVER_UDP_আকার   4096

#define NGX_RESOLVER_TCP_RSIZE  (2 + 65535)
#define NGX_RESOLVER_TCP_WSIZE  8192


প্রকারলেখ কাঠামো {
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


প্রকারলেখ কাঠামো {
    u_char  type_hi;
    u_char  type_lo;
    u_char  class_hi;
    u_char  class_lo;
} ngx_resolver_qs_t;


প্রকারলেখ কাঠামো {
    u_char  type_hi;
    u_char  type_lo;
    u_char  class_hi;
    u_char  class_lo;
    u_char  ttl[4];
    u_char  len_hi;
    u_char  len_lo;
} ngx_resolver_an_t;


#define ngx_resolver_গ্রন্থি(n)  ngx_rbtree_তথ্য(n, ngx_resolver_গ্রন্থি_t, গ্রন্থি)


স্থির ngx_int_t ngx_udp_connect(ngx_resolver_connection_t *rec);
স্থির ngx_int_t ngx_tcp_connect(ngx_resolver_connection_t *rec);


স্থির শূন্য ngx_resolver_cleanup(শূন্য *তথ্য);
স্থির শূন্য ngx_resolver_cleanup_বৃক্ষ(ngx_resolver_t *r, ngx_rbtree_t *বৃক্ষ);
স্থির ngx_int_t ngx_resolve_name_locked(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx, ngx_str_t *name);
স্থির শূন্য ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *বৃক্ষ,
    ngx_queue_t *queue);
স্থির ngx_int_t ngx_resolver_send_query(ngx_resolver_t *r,
    ngx_resolver_গ্রন্থি_t *rn);
স্থির ngx_int_t ngx_resolver_send_udp_query(ngx_resolver_t *r,
    ngx_resolver_connection_t *rec, u_char *query, u_short qlen);
স্থির ngx_int_t ngx_resolver_send_tcp_query(ngx_resolver_t *r,
    ngx_resolver_connection_t *rec, u_char *query, u_short qlen);
স্থির ngx_int_t ngx_resolver_create_name_query(ngx_resolver_t *r,
    ngx_resolver_গ্রন্থি_t *rn, ngx_str_t *name);
স্থির ngx_int_t ngx_resolver_create_srv_query(ngx_resolver_t *r,
    ngx_resolver_গ্রন্থি_t *rn, ngx_str_t *name);
স্থির ngx_int_t ngx_resolver_create_addr_query(ngx_resolver_t *r,
    ngx_resolver_গ্রন্থি_t *rn, ngx_resolver_addr_t *addr);
স্থির শূন্য ngx_resolver_resend_handler(ngx_event_t *ev);
স্থির time_t ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *বৃক্ষ,
    ngx_queue_t *queue);
স্থির ngx_uint_t ngx_resolver_resend_empty(ngx_resolver_t *r);
স্থির শূন্য ngx_resolver_udp_read(ngx_event_t *rev);
স্থির শূন্য ngx_resolver_tcp_write(ngx_event_t *wev);
স্থির শূন্য ngx_resolver_tcp_read(ngx_event_t *rev);
স্থির শূন্য ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf,
    আকার_t n, ngx_uint_t tcp);
স্থির শূন্য ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, আকার_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans);
স্থির শূন্য ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, আকার_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans);
স্থির শূন্য ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, আকার_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan);
স্থির ngx_resolver_গ্রন্থি_t *ngx_resolver_lookup_name(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
স্থির ngx_resolver_গ্রন্থি_t *ngx_resolver_lookup_srv(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
স্থির ngx_resolver_গ্রন্থি_t *ngx_resolver_lookup_addr(ngx_resolver_t *r,
    in_addr_t addr);
স্থির শূন্য ngx_resolver_rbtree_ঢোকাও_মান(ngx_rbtree_গ্রন্থি_t *অস্থায়ী,
    ngx_rbtree_গ্রন্থি_t *গ্রন্থি, ngx_rbtree_গ্রন্থি_t *sentinel);
স্থির ngx_int_t ngx_resolver_copy(ngx_resolver_t *r, ngx_str_t *name,
    u_char *buf, u_char *src, u_char *last);
স্থির ngx_int_t ngx_resolver_set_timeout(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx);
স্থির শূন্য ngx_resolver_timeout_handler(ngx_event_t *ev);
স্থির শূন্য ngx_resolver_free_গ্রন্থি(ngx_resolver_t *r, ngx_resolver_গ্রন্থি_t *rn);
স্থির শূন্য *ngx_resolver_alloc(ngx_resolver_t *r, আকার_t আকার_2);
স্থির শূন্য *ngx_resolver_calloc(ngx_resolver_t *r, আকার_t আকার_2);
স্থির শূন্য ngx_resolver_free(ngx_resolver_t *r, শূন্য *p);
স্থির শূন্য ngx_resolver_free_locked(ngx_resolver_t *r, শূন্য *p);
স্থির শূন্য *ngx_resolver_dup(ngx_resolver_t *r, শূন্য *src, আকার_t আকার_2);
স্থির ngx_resolver_addr_t *ngx_resolver_export(ngx_resolver_t *r,
    ngx_resolver_গ্রন্থি_t *rn, ngx_uint_t rotate);
স্থির শূন্য ngx_resolver_report_srv(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx);
স্থির u_char *ngx_resolver_log_error(ngx_log_t *log, u_char *buf, আকার_t len);
স্থির শূন্য ngx_resolver_resolve_srv_names(ngx_resolver_ctx_t *ctx,
    ngx_resolver_গ্রন্থি_t *rn);
স্থির শূন্য ngx_resolver_srv_names_handler(ngx_resolver_ctx_t *ctx);
স্থির ngx_int_t ngx_resolver_cmp_srvs(ধ্রুবক শূন্য *one, ধ্রুবক শূন্য *two);

#যদি (NGX_HAVE_INET6)
স্থির শূন্য ngx_resolver_rbtree_ঢোকাও_addr6_মান(ngx_rbtree_গ্রন্থি_t *অস্থায়ী,
    ngx_rbtree_গ্রন্থি_t *গ্রন্থি, ngx_rbtree_গ্রন্থি_t *sentinel);
স্থির ngx_resolver_গ্রন্থি_t *ngx_resolver_lookup_addr6(ngx_resolver_t *r,
    কাঠামো in6_addr *addr, uint32_t hash);
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

    r = ngx_pcalloc(cf->pool, আকার(ngx_resolver_t));
    যদি (r == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    r->event = ngx_pcalloc(cf->pool, আকার(ngx_event_t));
    যদি (r->event == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    cln = ngx_pool_cleanup_add(cf->pool, 0);
    যদি (cln == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    cln->handler = ngx_resolver_cleanup;
    cln->তথ্য = r;

    r->ipv4 = 1;

    ngx_rbtree_init(&r->name_rbtree, &r->name_sentinel,
                    ngx_resolver_rbtree_ঢোকাও_মান);

    ngx_rbtree_init(&r->srv_rbtree, &r->srv_sentinel,
                    ngx_resolver_rbtree_ঢোকাও_মান);

    ngx_rbtree_init(&r->addr_rbtree, &r->addr_sentinel,
                    ngx_rbtree_ঢোকাও_মান);

    ngx_queue_init(&r->name_resend_queue);
    ngx_queue_init(&r->srv_resend_queue);
    ngx_queue_init(&r->addr_resend_queue);

    ngx_queue_init(&r->name_expire_queue);
    ngx_queue_init(&r->srv_expire_queue);
    ngx_queue_init(&r->addr_expire_queue);

#যদি (NGX_HAVE_INET6)
    r->ipv6 = 1;

    ngx_rbtree_init(&r->addr6_rbtree, &r->addr6_sentinel,
                    ngx_resolver_rbtree_ঢোকাও_addr6_মান);

    ngx_queue_init(&r->addr6_resend_queue);

    ngx_queue_init(&r->addr6_expire_queue);
#endif

    r->event->handler = ngx_resolver_resend_handler;
    r->event->তথ্য = r;
    r->event->log = &cf->cycle->new_log;
    r->event->cancelable = 1;
    r->ident = -1;

    r->resend_timeout = 5;
    r->tcp_timeout = 5;
    r->expire = 30;
    r->valid = 0;

    r->log = &cf->cycle->new_log;
    r->log_level = NGX_LOG_ERR;

    যদি (n) {
        যদি (ngx_array_init(&r->connections, cf->pool, n,
                           আকার(ngx_resolver_connection_t))
            != NGX_OK)
        {
            ফেরত ফাঁকা;
        }
    }

    জন্য (i = 0; i < n; i++) {
        যদি (ngx_strncmp(names[i].তথ্য, "valid=", 6) == 0) {
            s.len = names[i].len - 6;
            s.তথ্য = names[i].তথ্য + 6;

            r->valid = ngx_parse_time(&s, 1);

            যদি (r->valid == (time_t) NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &names[i]);
                ফেরত ফাঁকা;
            }

            চালু;
        }

#যদি (NGX_HAVE_INET6)
        যদি (ngx_strncmp(names[i].তথ্য, "ipv4=", 5) == 0) {

            যদি (ngx_strcmp(&names[i].তথ্য[5], "on") == 0) {
                r->ipv4 = 1;

            } অন্যথা যদি (ngx_strcmp(&names[i].তথ্য[5], "off") == 0) {
                r->ipv4 = 0;

            } অন্যথা {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &names[i]);
                ফেরত ফাঁকা;
            }

            চালু;
        }

        যদি (ngx_strncmp(names[i].তথ্য, "ipv6=", 5) == 0) {

            যদি (ngx_strcmp(&names[i].তথ্য[5], "on") == 0) {
                r->ipv6 = 1;

            } অন্যথা যদি (ngx_strcmp(&names[i].তথ্য[5], "off") == 0) {
                r->ipv6 = 0;

            } অন্যথা {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &names[i]);
                ফেরত ফাঁকা;
            }

            চালু;
        }
#endif

        ngx_memzero(&u, আকার(ngx_url_t));

        u.url = names[i];
        u.default_port = 53;

        যদি (ngx_parse_url(cf->pool, &u) != NGX_OK) {
            যদি (u.err) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "%s in resolver \"%V\"",
                                   u.err, &u.url);
            }

            ফেরত ফাঁকা;
        }

        rec = ngx_array_ঠেলো_n(&r->connections, u.naddrs);
        যদি (rec == ফাঁকা) {
            ফেরত ফাঁকা;
        }

        ngx_memzero(rec, u.naddrs * আকার(ngx_resolver_connection_t));

        জন্য (j = 0; j < u.naddrs; j++) {
            rec[j].sockaddr = u.addrs[j].sockaddr;
            rec[j].socklen = u.addrs[j].socklen;
            rec[j].server = u.addrs[j].name;
            rec[j].resolver = r;
        }
    }

#যদি (NGX_HAVE_INET6)
    যদি (r->ipv4 + r->ipv6 == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"ipv4\" and \"ipv6\" cannot both be \"off\"");
        ফেরত ফাঁকা;
    }
#endif

    যদি (n && r->connections.nelts == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "no name servers defined");
        ফেরত ফাঁকা;
    }

    ফেরত r;
}


স্থির শূন্য
ngx_resolver_cleanup(শূন্য *তথ্য)
{
    ngx_resolver_t  *r = তথ্য;

    ngx_uint_t                  i;
    ngx_resolver_connection_t  *rec;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "cleanup resolver");

    ngx_resolver_cleanup_বৃক্ষ(r, &r->name_rbtree);

    ngx_resolver_cleanup_বৃক্ষ(r, &r->srv_rbtree);

    ngx_resolver_cleanup_বৃক্ষ(r, &r->addr_rbtree);

#যদি (NGX_HAVE_INET6)
    ngx_resolver_cleanup_বৃক্ষ(r, &r->addr6_rbtree);
#endif

    যদি (r->event->timer_set) {
        ngx_del_timer(r->event);
    }

    rec = r->connections.elts;

    জন্য (i = 0; i < r->connections.nelts; i++) {
        যদি (rec[i].udp) {
            ngx_close_connection(rec[i].udp);
        }

        যদি (rec[i].tcp) {
            ngx_close_connection(rec[i].tcp);
        }

        যদি (rec[i].read_buf) {
            ngx_resolver_free(r, rec[i].read_buf->start);
            ngx_resolver_free(r, rec[i].read_buf);
        }

        যদি (rec[i].write_buf) {
            ngx_resolver_free(r, rec[i].write_buf->start);
            ngx_resolver_free(r, rec[i].write_buf);
        }
    }
}


স্থির শূন্য
ngx_resolver_cleanup_বৃক্ষ(ngx_resolver_t *r, ngx_rbtree_t *বৃক্ষ)
{
    ngx_resolver_ctx_t   *ctx, *পরবর্তী;
    ngx_resolver_গ্রন্থি_t  *rn;

    যখন (বৃক্ষ->মূল != বৃক্ষ->sentinel) {

        rn = ngx_resolver_গ্রন্থি(ngx_rbtree_min(বৃক্ষ->মূল, বৃক্ষ->sentinel));

        ngx_queue_remove(&rn->queue);

        জন্য (ctx = rn->waiting; ctx; ctx = পরবর্তী) {
            পরবর্তী = ctx->পরবর্তী;

            যদি (ctx->event) {
                যদি (ctx->event->timer_set) {
                    ngx_del_timer(ctx->event);
                }

                ngx_resolver_free(r, ctx->event);
            }

            ngx_resolver_free(r, ctx);
        }

        ngx_rbtree_delete(বৃক্ষ, &rn->গ্রন্থি);

        ngx_resolver_free_গ্রন্থি(r, rn);
    }
}


ngx_resolver_ctx_t *
ngx_resolve_start(ngx_resolver_t *r, ngx_resolver_ctx_t *অস্থায়ী)
{
    in_addr_t            addr;
    ngx_resolver_ctx_t  *ctx;

    যদি (অস্থায়ী) {
        addr = ngx_inet_addr(অস্থায়ী->name.তথ্য, অস্থায়ী->name.len);

        যদি (addr != INADDR_NONE) {
            অস্থায়ী->resolver = r;
            অস্থায়ী->state = NGX_OK;
            অস্থায়ী->naddrs = 1;
            অস্থায়ী->addrs = &অস্থায়ী->addr;
            অস্থায়ী->addr.sockaddr = (কাঠামো sockaddr *) &অস্থায়ী->sin;
            অস্থায়ী->addr.socklen = আকার(কাঠামো sockaddr_in);
            ngx_memzero(&অস্থায়ী->sin, আকার(কাঠামো sockaddr_in));
            অস্থায়ী->sin.sin_family = AF_INET;
            অস্থায়ী->sin.sin_addr.s_addr = addr;
            অস্থায়ী->quick = 1;

            ফেরত অস্থায়ী;
        }
    }

    যদি (r->connections.nelts == 0) {
        ফেরত NGX_NO_RESOLVER;
    }

    ctx = ngx_resolver_calloc(r, আকার(ngx_resolver_ctx_t));

    যদি (ctx) {
        ctx->resolver = r;
    }

    ফেরত ctx;
}


ngx_int_t
ngx_resolve_name(ngx_resolver_ctx_t *ctx)
{
    আকার_t           slen;
    ngx_int_t        rc;
    ngx_str_t        name;
    ngx_resolver_t  *r;

    r = ctx->resolver;

    যদি (ctx->name.len > 0 && ctx->name.তথ্য[ctx->name.len - 1] == '.') {
        ctx->name.len--;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve: \"%V\"", &ctx->name);

    যদি (ctx->quick) {
        ctx->handler(ctx);
        ফেরত NGX_OK;
    }

    যদি (ctx->service.len) {
        slen = ctx->service.len;

        যদি (ngx_strlchr(ctx->service.তথ্য,
                        ctx->service.তথ্য + ctx->service.len, '.')
            == ফাঁকা)
        {
            slen += আকার("_._tcp") - 1;
        }

        name.len = slen + 1 + ctx->name.len;

        name.তথ্য = ngx_resolver_alloc(r, name.len);
        যদি (name.তথ্য == ফাঁকা) {
            যাও failed;
        }

        যদি (slen == ctx->service.len) {
            ngx_sprintf(name.তথ্য, "%V.%V", &ctx->service, &ctx->name);

        } অন্যথা {
            ngx_sprintf(name.তথ্য, "_%V._tcp.%V", &ctx->service, &ctx->name);
        }

        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &name);

        ngx_resolver_free(r, name.তথ্য);

    } অন্যথা {
        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &ctx->name);
    }

    যদি (rc == NGX_OK) {
        ফেরত NGX_OK;
    }

    /* unlock name mutex */

    যদি (rc == NGX_AGAIN) {
        ফেরত NGX_OK;
    }

    /* NGX_ERROR */

    যদি (ctx->event) {
        ngx_resolver_free(r, ctx->event);
    }

failed:

    ngx_resolver_free(r, ctx);

    ফেরত NGX_ERROR;
}


শূন্য
ngx_resolve_name_done(ngx_resolver_ctx_t *ctx)
{
    ngx_uint_t            i;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_গ্রন্থি_t  *rn;

    r = ctx->resolver;

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve name done: %i", ctx->state);

    যদি (ctx->quick) {
        ফেরত;
    }

    যদি (ctx->event && ctx->event->timer_set) {
        ngx_del_timer(ctx->event);
    }

    /* lock name mutex */

    যদি (ctx->nsrvs) {
        জন্য (i = 0; i < ctx->nsrvs; i++) {
            যদি (ctx->srvs[i].ctx) {
                ngx_resolve_name_done(ctx->srvs[i].ctx);
            }

            যদি (ctx->srvs[i].addrs) {
                ngx_resolver_free(r, ctx->srvs[i].addrs->sockaddr);
                ngx_resolver_free(r, ctx->srvs[i].addrs);
            }

            ngx_resolver_free(r, ctx->srvs[i].name.তথ্য);
        }

        ngx_resolver_free(r, ctx->srvs);
    }

    যদি (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->গ্রন্থি;

        যদি (rn) {
            p = &rn->waiting;
            w = rn->waiting;

            যখন (w) {
                যদি (w == ctx) {
                    *p = w->পরবর্তী;

                    যাও done;
                }

                p = &w->পরবর্তী;
                w = w->পরবর্তী;
            }

            ngx_log_error(NGX_LOG_ALERT, r->log, 0,
                          "could not cancel %V resolving", &ctx->name);
        }
    }

done:

    যদি (ctx->service.len) {
        ngx_resolver_expire(r, &r->srv_rbtree, &r->srv_expire_queue);

    } অন্যথা {
        ngx_resolver_expire(r, &r->name_rbtree, &r->name_expire_queue);
    }

    /* unlock name mutex */

    /* lock alloc mutex */

    যদি (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    যদি (r->event->timer_set && ngx_resolver_resend_empty(r)) {
        ngx_del_timer(r->event);
    }
}


স্থির ngx_int_t
ngx_resolve_name_locked(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx,
    ngx_str_t *name)
{
    uint32_t              hash;
    ngx_int_t             rc;
    ngx_str_t             cname;
    ngx_uint_t            i, naddrs;
    ngx_queue_t          *resend_queue, *expire_queue;
    ngx_rbtree_t         *বৃক্ষ;
    ngx_resolver_ctx_t   *পরবর্তী, *last;
    ngx_resolver_addr_t  *addrs;
    ngx_resolver_গ্রন্থি_t  *rn;

    ngx_strlow(name->তথ্য, name->তথ্য, name->len);

    hash = ngx_crc32_short(name->তথ্য, name->len);

    যদি (ctx->service.len) {
        rn = ngx_resolver_lookup_srv(r, name, hash);

        বৃক্ষ = &r->srv_rbtree;
        resend_queue = &r->srv_resend_queue;
        expire_queue = &r->srv_expire_queue;

    } অন্যথা {
        rn = ngx_resolver_lookup_name(r, name, hash);

        বৃক্ষ = &r->name_rbtree;
        resend_queue = &r->name_resend_queue;
        expire_queue = &r->name_expire_queue;
    }

    যদি (rn) {

        /* ctx can be a list after NGX_RESOLVE_CNAME */
        জন্য (last = ctx; last->পরবর্তী; last = last->পরবর্তী);

        যদি (rn->valid >= ngx_time()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0, "resolve cached");

            ngx_queue_remove(&rn->queue);

            rn->expire = ngx_time() + r->expire;

            ngx_queue_ঢোকাও_শীর্ষ(expire_queue, &rn->queue);

            naddrs = (rn->naddrs == (u_short) -1) ? 0 : rn->naddrs;
#যদি (NGX_HAVE_INET6)
            naddrs += (rn->naddrs6 == (u_short) -1) ? 0 : rn->naddrs6;
#endif

            যদি (naddrs) {

                যদি (naddrs == 1 && rn->naddrs == 1) {
                    addrs = ফাঁকা;

                } অন্যথা {
                    addrs = ngx_resolver_export(r, rn, 1);
                    যদি (addrs == ফাঁকা) {
                        ফেরত NGX_ERROR;
                    }
                }

                last->পরবর্তী = rn->waiting;
                rn->waiting = ফাঁকা;

                /* unlock name mutex */

                করো {
                    ctx->state = NGX_OK;
                    ctx->valid = rn->valid;
                    ctx->naddrs = naddrs;

                    যদি (addrs == ফাঁকা) {
                        ctx->addrs = &ctx->addr;
                        ctx->addr.sockaddr = (কাঠামো sockaddr *) &ctx->sin;
                        ctx->addr.socklen = আকার(কাঠামো sockaddr_in);
                        ngx_memzero(&ctx->sin, আকার(কাঠামো sockaddr_in));
                        ctx->sin.sin_family = AF_INET;
                        ctx->sin.sin_addr.s_addr = rn->u.addr;

                    } অন্যথা {
                        ctx->addrs = addrs;
                    }

                    পরবর্তী = ctx->পরবর্তী;

                    ctx->handler(ctx);

                    ctx = পরবর্তী;
                } যখন (ctx);

                যদি (addrs != ফাঁকা) {
                    ngx_resolver_free(r, addrs->sockaddr);
                    ngx_resolver_free(r, addrs);
                }

                ফেরত NGX_OK;
            }

            যদি (rn->nsrvs) {
                last->পরবর্তী = rn->waiting;
                rn->waiting = ফাঁকা;

                /* unlock name mutex */

                করো {
                    পরবর্তী = ctx->পরবর্তী;

                    ngx_resolver_resolve_srv_names(ctx, rn);

                    ctx = পরবর্তী;
                } যখন (ctx);

                ফেরত NGX_OK;
            }

            /* NGX_RESOLVE_CNAME */

            যদি (ctx->recursion++ < NGX_RESOLVER_MAX_RECURSION) {

                cname.len = rn->cnlen;
                cname.তথ্য = rn->u.cname;

                ফেরত ngx_resolve_name_locked(r, ctx, &cname);
            }

            last->পরবর্তী = rn->waiting;
            rn->waiting = ফাঁকা;

            /* unlock name mutex */

            করো {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
                পরবর্তী = ctx->পরবর্তী;

                ctx->handler(ctx);

                ctx = পরবর্তী;
            } যখন (ctx);

            ফেরত NGX_OK;
        }

        যদি (rn->waiting) {
            যদি (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
                ফেরত NGX_ERROR;
            }

            last->পরবর্তী = rn->waiting;
            rn->waiting = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;

            করো {
                ctx->গ্রন্থি = rn;
                ctx = ctx->পরবর্তী;
            } যখন (ctx);

            ফেরত NGX_AGAIN;
        }

        ngx_queue_remove(&rn->queue);

        /* lock alloc mutex */

        যদি (rn->query) {
            ngx_resolver_free_locked(r, rn->query);
            rn->query = ফাঁকা;
#যদি (NGX_HAVE_INET6)
            rn->query6 = ফাঁকা;
#endif
        }

        যদি (rn->cnlen) {
            ngx_resolver_free_locked(r, rn->u.cname);
        }

        যদি (rn->naddrs > 1 && rn->naddrs != (u_short) -1) {
            ngx_resolver_free_locked(r, rn->u.addrs);
        }

#যদি (NGX_HAVE_INET6)
        যদি (rn->naddrs6 > 1 && rn->naddrs6 != (u_short) -1) {
            ngx_resolver_free_locked(r, rn->u6.addrs6);
        }
#endif

        যদি (rn->nsrvs) {
            জন্য (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
                যদি (rn->u.srvs[i].name.তথ্য) {
                    ngx_resolver_free_locked(r, rn->u.srvs[i].name.তথ্য);
                }
            }

            ngx_resolver_free_locked(r, rn->u.srvs);
        }

        /* unlock alloc mutex */

    } অন্যথা {

        rn = ngx_resolver_alloc(r, আকার(ngx_resolver_গ্রন্থি_t));
        যদি (rn == ফাঁকা) {
            ফেরত NGX_ERROR;
        }

        rn->name = ngx_resolver_dup(r, name->তথ্য, name->len);
        যদি (rn->name == ফাঁকা) {
            ngx_resolver_free(r, rn);
            ফেরত NGX_ERROR;
        }

        rn->গ্রন্থি.চাবি = hash;
        rn->nlen = (u_short) name->len;
        rn->query = ফাঁকা;
#যদি (NGX_HAVE_INET6)
        rn->query6 = ফাঁকা;
#endif

        ngx_rbtree_ঢোকাও(বৃক্ষ, &rn->গ্রন্থি);
    }

    যদি (ctx->service.len) {
        rc = ngx_resolver_create_srv_query(r, rn, name);

    } অন্যথা {
        rc = ngx_resolver_create_name_query(r, rn, name);
    }

    যদি (rc == NGX_ERROR) {
        যাও failed;
    }

    যদি (rc == NGX_DECLINED) {
        ngx_rbtree_delete(বৃক্ষ, &rn->গ্রন্থি);

        ngx_resolver_free(r, rn->query);
        ngx_resolver_free(r, rn->name);
        ngx_resolver_free(r, rn);

        করো {
            ctx->state = NGX_RESOLVE_NXDOMAIN;
            পরবর্তী = ctx->পরবর্তী;

            ctx->handler(ctx);

            ctx = পরবর্তী;
        } যখন (ctx);

        ফেরত NGX_OK;
    }

    rn->last_connection = r->last_connection++;
    যদি (r->last_connection == r->connections.nelts) {
        r->last_connection = 0;
    }

    rn->naddrs = r->ipv4 ? (u_short) -1 : 0;
    rn->tcp = 0;
#যদি (NGX_HAVE_INET6)
    rn->naddrs6 = r->ipv6 ? (u_short) -1 : 0;
    rn->tcp6 = 0;
#endif
    rn->nsrvs = 0;

    যদি (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->last_connection++;
        যদি (rn->last_connection == r->connections.nelts) {
            rn->last_connection = 0;
        }

        (শূন্য) ngx_resolver_send_query(r, rn);
    }

    যদি (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
        যাও failed;
    }

    যদি (ngx_resolver_resend_empty(r)) {
        ngx_add_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_time() + r->resend_timeout;

    ngx_queue_ঢোকাও_শীর্ষ(resend_queue, &rn->queue);

    rn->code = 0;
    rn->cnlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_মান;
    rn->waiting = ctx;

    ctx->state = NGX_AGAIN;
    ctx->async = 1;

    করো {
        ctx->গ্রন্থি = rn;
        ctx = ctx->পরবর্তী;
    } যখন (ctx);

    ফেরত NGX_AGAIN;

failed:

    ngx_rbtree_delete(বৃক্ষ, &rn->গ্রন্থি);

    যদি (rn->query) {
        ngx_resolver_free(r, rn->query);
    }

    ngx_resolver_free(r, rn->name);

    ngx_resolver_free(r, rn);

    ফেরত NGX_ERROR;
}


ngx_int_t
ngx_resolve_addr(ngx_resolver_ctx_t *ctx)
{
    u_char               *name;
    in_addr_t             addr;
    ngx_queue_t          *resend_queue, *expire_queue;
    ngx_rbtree_t         *বৃক্ষ;
    ngx_resolver_t       *r;
    কাঠামো sockaddr_in   *sin;
    ngx_resolver_গ্রন্থি_t  *rn;
#যদি (NGX_HAVE_INET6)
    uint32_t              hash;
    কাঠামো sockaddr_in6  *sin6;
#endif

#যদি (NGX_SUPPRESS_WARN)
    addr = 0;
#যদি (NGX_HAVE_INET6)
    hash = 0;
    sin6 = ফাঁকা;
#endif
#endif

    r = ctx->resolver;

    নির্বাচন (ctx->addr.sockaddr->sa_family) {

#যদি (NGX_HAVE_INET6)
    ক্ষেত্র AF_INET6:
        sin6 = (কাঠামো sockaddr_in6 *) ctx->addr.sockaddr;
        hash = ngx_crc32_short(sin6->sin6_addr.s6_addr, 16);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr6(r, &sin6->sin6_addr, hash);

        বৃক্ষ = &r->addr6_rbtree;
        resend_queue = &r->addr6_resend_queue;
        expire_queue = &r->addr6_expire_queue;

        বিরতি;
#endif

    পূর্বনির্ধারিত: /* AF_INET */
        sin = (কাঠামো sockaddr_in *) ctx->addr.sockaddr;
        addr = ntohl(sin->sin_addr.s_addr);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        বৃক্ষ = &r->addr_rbtree;
        resend_queue = &r->addr_resend_queue;
        expire_queue = &r->addr_expire_queue;
    }

    যদি (rn) {

        যদি (rn->valid >= ngx_time()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0, "resolve cached");

            ngx_queue_remove(&rn->queue);

            rn->expire = ngx_time() + r->expire;

            ngx_queue_ঢোকাও_শীর্ষ(expire_queue, &rn->queue);

            name = ngx_resolver_dup(r, rn->name, rn->nlen);
            যদি (name == ফাঁকা) {
                ngx_resolver_free(r, ctx);
                ফেরত NGX_ERROR;
            }

            ctx->name.len = rn->nlen;
            ctx->name.তথ্য = name;

            /* unlock addr mutex */

            ctx->state = NGX_OK;
            ctx->valid = rn->valid;

            ctx->handler(ctx);

            ngx_resolver_free(r, name);

            ফেরত NGX_OK;
        }

        যদি (rn->waiting) {
            যদি (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
                ফেরত NGX_ERROR;
            }

            ctx->পরবর্তী = rn->waiting;
            rn->waiting = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;
            ctx->গ্রন্থি = rn;

            /* unlock addr mutex */

            ফেরত NGX_OK;
        }

        ngx_queue_remove(&rn->queue);

        ngx_resolver_free(r, rn->query);
        rn->query = ফাঁকা;
#যদি (NGX_HAVE_INET6)
        rn->query6 = ফাঁকা;
#endif

    } অন্যথা {
        rn = ngx_resolver_alloc(r, আকার(ngx_resolver_গ্রন্থি_t));
        যদি (rn == ফাঁকা) {
            যাও failed;
        }

        নির্বাচন (ctx->addr.sockaddr->sa_family) {

#যদি (NGX_HAVE_INET6)
        ক্ষেত্র AF_INET6:
            rn->addr6 = sin6->sin6_addr;
            rn->গ্রন্থি.চাবি = hash;
            বিরতি;
#endif

        পূর্বনির্ধারিত: /* AF_INET */
            rn->গ্রন্থি.চাবি = addr;
        }

        rn->query = ফাঁকা;
#যদি (NGX_HAVE_INET6)
        rn->query6 = ফাঁকা;
#endif

        ngx_rbtree_ঢোকাও(বৃক্ষ, &rn->গ্রন্থি);
    }

    যদি (ngx_resolver_create_addr_query(r, rn, &ctx->addr) != NGX_OK) {
        যাও failed;
    }

    rn->last_connection = r->last_connection++;
    যদি (r->last_connection == r->connections.nelts) {
        r->last_connection = 0;
    }

    rn->naddrs = (u_short) -1;
    rn->tcp = 0;
#যদি (NGX_HAVE_INET6)
    rn->naddrs6 = (u_short) -1;
    rn->tcp6 = 0;
#endif
    rn->nsrvs = 0;

    যদি (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->last_connection++;
        যদি (rn->last_connection == r->connections.nelts) {
            rn->last_connection = 0;
        }

        (শূন্য) ngx_resolver_send_query(r, rn);
    }

    যদি (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
        যাও failed;
    }

    যদি (ngx_resolver_resend_empty(r)) {
        ngx_add_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_time() + r->resend_timeout;

    ngx_queue_ঢোকাও_শীর্ষ(resend_queue, &rn->queue);

    rn->code = 0;
    rn->cnlen = 0;
    rn->name = ফাঁকা;
    rn->nlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_মান;
    rn->waiting = ctx;

    /* unlock addr mutex */

    ctx->state = NGX_AGAIN;
    ctx->async = 1;
    ctx->গ্রন্থি = rn;

    ফেরত NGX_OK;

failed:

    যদি (rn) {
        ngx_rbtree_delete(বৃক্ষ, &rn->গ্রন্থি);

        যদি (rn->query) {
            ngx_resolver_free(r, rn->query);
        }

        ngx_resolver_free(r, rn);
    }

    /* unlock addr mutex */

    যদি (ctx->event) {
        ngx_resolver_free(r, ctx->event);
    }

    ngx_resolver_free(r, ctx);

    ফেরত NGX_ERROR;
}


শূন্য
ngx_resolve_addr_done(ngx_resolver_ctx_t *ctx)
{
    ngx_queue_t          *expire_queue;
    ngx_rbtree_t         *বৃক্ষ;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_গ্রন্থি_t  *rn;

    r = ctx->resolver;

    নির্বাচন (ctx->addr.sockaddr->sa_family) {

#যদি (NGX_HAVE_INET6)
    ক্ষেত্র AF_INET6:
        বৃক্ষ = &r->addr6_rbtree;
        expire_queue = &r->addr6_expire_queue;
        বিরতি;
#endif

    পূর্বনির্ধারিত: /* AF_INET */
        বৃক্ষ = &r->addr_rbtree;
        expire_queue = &r->addr_expire_queue;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve addr done: %i", ctx->state);

    যদি (ctx->event && ctx->event->timer_set) {
        ngx_del_timer(ctx->event);
    }

    /* lock addr mutex */

    যদি (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->গ্রন্থি;

        যদি (rn) {
            p = &rn->waiting;
            w = rn->waiting;

            যখন (w) {
                যদি (w == ctx) {
                    *p = w->পরবর্তী;

                    যাও done;
                }

                p = &w->পরবর্তী;
                w = w->পরবর্তী;
            }
        }

        {
            u_char     text[NGX_SOCKADDR_STRLEN];
            ngx_str_t  addrtext;

            addrtext.তথ্য = text;
            addrtext.len = ngx_sock_ntop(ctx->addr.sockaddr, ctx->addr.socklen,
                                         text, NGX_SOCKADDR_STRLEN, 0);

            ngx_log_error(NGX_LOG_ALERT, r->log, 0,
                          "could not cancel %V resolving", &addrtext);
        }
    }

done:

    ngx_resolver_expire(r, বৃক্ষ, expire_queue);

    /* unlock addr mutex */

    /* lock alloc mutex */

    যদি (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    যদি (r->event->timer_set && ngx_resolver_resend_empty(r)) {
        ngx_del_timer(r->event);
    }
}


স্থির শূন্য
ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *বৃক্ষ, ngx_queue_t *queue)
{
    time_t                now;
    ngx_uint_t            i;
    ngx_queue_t          *q;
    ngx_resolver_গ্রন্থি_t  *rn;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver expire");

    now = ngx_time();

    জন্য (i = 0; i < 2; i++) {
        যদি (ngx_queue_empty(queue)) {
            ফেরত;
        }

        q = ngx_queue_last(queue);

        rn = ngx_queue_তথ্য(q, ngx_resolver_গ্রন্থি_t, queue);

        যদি (now <= rn->expire) {
            ফেরত;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver expire \"%*s\"", (আকার_t) rn->nlen, rn->name);

        ngx_queue_remove(q);

        ngx_rbtree_delete(বৃক্ষ, &rn->গ্রন্থি);

        ngx_resolver_free_গ্রন্থি(r, rn);
    }
}


স্থির ngx_int_t
ngx_resolver_send_query(ngx_resolver_t *r, ngx_resolver_গ্রন্থি_t *rn)
{
    ngx_int_t                   rc;
    ngx_resolver_connection_t  *rec;

    rec = r->connections.elts;
    rec = &rec[rn->last_connection];

    যদি (rec->log.handler == ফাঁকা) {
        rec->log = *r->log;
        rec->log.handler = ngx_resolver_log_error;
        rec->log.তথ্য = rec;
        rec->log.action = "resolving";
    }

    যদি (rn->query && rn->naddrs == (u_short) -1) {
        rc = rn->tcp ? ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen)
                     : ngx_resolver_send_udp_query(r, rec, rn->query, rn->qlen);

        যদি (rc != NGX_OK) {
            ফেরত rc;
        }
    }

#যদি (NGX_HAVE_INET6)

    যদি (rn->query6 && rn->naddrs6 == (u_short) -1) {
        rc = rn->tcp6
                    ? ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen)
                    : ngx_resolver_send_udp_query(r, rec, rn->query6, rn->qlen);

        যদি (rc != NGX_OK) {
            ফেরত rc;
        }
    }

#endif

    ফেরত NGX_OK;
}


স্থির ngx_int_t
ngx_resolver_send_udp_query(ngx_resolver_t *r, ngx_resolver_connection_t  *rec,
    u_char *query, u_short qlen)
{
    ssize_t  n;

    যদি (rec->udp == ফাঁকা) {
        যদি (ngx_udp_connect(rec) != NGX_OK) {
            ফেরত NGX_ERROR;
        }

        rec->udp->তথ্য = rec;
        rec->udp->পঠ->handler = ngx_resolver_udp_read;
        rec->udp->পঠ->resolver = 1;
    }

    n = ngx_send(rec->udp, query, qlen);

    যদি (n == NGX_ERROR) {
        যাও failed;
    }

    যদি ((আকার_t) n != (আকার_t) qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->log, 0, "send() incomplete");
        যাও failed;
    }

    ফেরত NGX_OK;

failed:

    ngx_close_connection(rec->udp);
    rec->udp = ফাঁকা;

    ফেরত NGX_ERROR;
}


স্থির ngx_int_t
ngx_resolver_send_tcp_query(ngx_resolver_t *r, ngx_resolver_connection_t *rec,
    u_char *query, u_short qlen)
{
    ngx_buf_t  *b;
    ngx_int_t   rc;

    rc = NGX_OK;

    যদি (rec->tcp == ফাঁকা) {
        b = rec->read_buf;

        যদি (b == ফাঁকা) {
            b = ngx_resolver_calloc(r, আকার(ngx_buf_t));
            যদি (b == ফাঁকা) {
                ফেরত NGX_ERROR;
            }

            b->start = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_RSIZE);
            যদি (b->start == ফাঁকা) {
                ngx_resolver_free(r, b);
                ফেরত NGX_ERROR;
            }

            b->end = b->start + NGX_RESOLVER_TCP_RSIZE;

            rec->read_buf = b;
        }

        b->pos = b->start;
        b->last = b->start;

        b = rec->write_buf;

        যদি (b == ফাঁকা) {
            b = ngx_resolver_calloc(r, আকার(ngx_buf_t));
            যদি (b == ফাঁকা) {
                ফেরত NGX_ERROR;
            }

            b->start = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_WSIZE);
            যদি (b->start == ফাঁকা) {
                ngx_resolver_free(r, b);
                ফেরত NGX_ERROR;
            }

            b->end = b->start + NGX_RESOLVER_TCP_WSIZE;

            rec->write_buf = b;
        }

        b->pos = b->start;
        b->last = b->start;

        rc = ngx_tcp_connect(rec);
        যদি (rc == NGX_ERROR) {
            ফেরত NGX_ERROR;
        }

        rec->tcp->তথ্য = rec;
        rec->tcp->লিখ->handler = ngx_resolver_tcp_write;
        rec->tcp->লিখ->cancelable = 1;
        rec->tcp->পঠ->handler = ngx_resolver_tcp_read;
        rec->tcp->পঠ->resolver = 1;

        ngx_add_timer(rec->tcp->লিখ, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    b = rec->write_buf;

    যদি (b->end - b->last <  2 + qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->log, 0, "buffer overflow");
        ফেরত NGX_ERROR;
    }

    *b->last++ = (u_char) (qlen >> 8);
    *b->last++ = (u_char) qlen;
    b->last = ngx_cpymem(b->last, query, qlen);

    যদি (rc == NGX_OK) {
        ngx_resolver_tcp_write(rec->tcp->লিখ);
    }

    ফেরত NGX_OK;
}


স্থির শূন্য
ngx_resolver_resend_handler(ngx_event_t *ev)
{
    time_t           timer, atimer, stimer, ntimer;
#যদি (NGX_HAVE_INET6)
    time_t           a6timer;
#endif
    ngx_resolver_t  *r;

    r = ev->তথ্য;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver resend handler");

    /* lock name mutex */

    ntimer = ngx_resolver_resend(r, &r->name_rbtree, &r->name_resend_queue);

    stimer = ngx_resolver_resend(r, &r->srv_rbtree, &r->srv_resend_queue);

    /* unlock name mutex */

    /* lock addr mutex */

    atimer = ngx_resolver_resend(r, &r->addr_rbtree, &r->addr_resend_queue);

    /* unlock addr mutex */

#যদি (NGX_HAVE_INET6)

    /* lock addr6 mutex */

    a6timer = ngx_resolver_resend(r, &r->addr6_rbtree, &r->addr6_resend_queue);

    /* unlock addr6 mutex */

#endif

    timer = ntimer;

    যদি (timer == 0) {
        timer = atimer;

    } অন্যথা যদি (atimer) {
        timer = ngx_min(timer, atimer);
    }

    যদি (timer == 0) {
        timer = stimer;

    } অন্যথা যদি (stimer) {
        timer = ngx_min(timer, stimer);
    }

#যদি (NGX_HAVE_INET6)

    যদি (timer == 0) {
        timer = a6timer;

    } অন্যথা যদি (a6timer) {
        timer = ngx_min(timer, a6timer);
    }

#endif

    যদি (timer) {
        ngx_add_timer(r->event, (ngx_msec_t) (timer * 1000));
    }
}


স্থির time_t
ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *বৃক্ষ, ngx_queue_t *queue)
{
    time_t                now;
    ngx_queue_t          *q;
    ngx_resolver_গ্রন্থি_t  *rn;

    now = ngx_time();

    জন্য ( ;; ) {
        যদি (ngx_queue_empty(queue)) {
            ফেরত 0;
        }

        q = ngx_queue_last(queue);

        rn = ngx_queue_তথ্য(q, ngx_resolver_গ্রন্থি_t, queue);

        যদি (now < rn->expire) {
            ফেরত rn->expire - now;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver resend \"%*s\" %p",
                       (আকার_t) rn->nlen, rn->name, rn->waiting);

        ngx_queue_remove(q);

        যদি (rn->waiting) {

            যদি (++rn->last_connection == r->connections.nelts) {
                rn->last_connection = 0;
            }

            (শূন্য) ngx_resolver_send_query(r, rn);

            rn->expire = now + r->resend_timeout;

            ngx_queue_ঢোকাও_শীর্ষ(queue, q);

            চালু;
        }

        ngx_rbtree_delete(বৃক্ষ, &rn->গ্রন্থি);

        ngx_resolver_free_গ্রন্থি(r, rn);
    }
}


স্থির ngx_uint_t
ngx_resolver_resend_empty(ngx_resolver_t *r)
{
    ফেরত ngx_queue_empty(&r->name_resend_queue)
           && ngx_queue_empty(&r->srv_resend_queue)
#যদি (NGX_HAVE_INET6)
           && ngx_queue_empty(&r->addr6_resend_queue)
#endif
           && ngx_queue_empty(&r->addr_resend_queue);
}


স্থির শূন্য
ngx_resolver_udp_read(ngx_event_t *rev)
{
    ssize_t                     n;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;
    u_char                      buf[NGX_RESOLVER_UDP_আকার];

    c = rev->তথ্য;
    rec = c->তথ্য;

    করো {
        n = ngx_udp_recv(c, buf, NGX_RESOLVER_UDP_আকার);

        যদি (n == NGX_AGAIN) {
            বিরতি;
        }

        যদি (n == NGX_ERROR) {
            যাও failed;
        }

        ngx_resolver_process_response(rec->resolver, buf, n, 0);

    } যখন (rev->ready);

    যদি (ngx_handle_read_event(rev, 0) != NGX_OK) {
        যাও failed;
    }

    ফেরত;

failed:

    ngx_close_connection(rec->udp);
    rec->udp = ফাঁকা;
}


স্থির শূন্য
ngx_resolver_tcp_write(ngx_event_t *wev)
{
    off_t                       sent;
    ssize_t                     n;
    ngx_buf_t                  *b;
    ngx_resolver_t             *r;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;

    c = wev->তথ্য;
    rec = c->তথ্য;
    b = rec->write_buf;
    r = rec->resolver;

    যদি (wev->timedout) {
        যাও failed;
    }

    sent = c->sent;

    যখন (wev->ready && b->pos < b->last) {
        n = ngx_send(c, b->pos, b->last - b->pos);

        যদি (n == NGX_AGAIN) {
            বিরতি;
        }

        যদি (n == NGX_ERROR) {
            যাও failed;
        }

        b->pos += n;
    }

    যদি (b->pos != b->start) {
        b->last = ngx_movemem(b->start, b->pos, b->last - b->pos);
        b->pos = b->start;
    }

    যদি (c->sent != sent) {
        ngx_add_timer(wev, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    যদি (ngx_handle_write_event(wev, 0) != NGX_OK) {
        যাও failed;
    }

    ফেরত;

failed:

    ngx_close_connection(c);
    rec->tcp = ফাঁকা;
}


স্থির শূন্য
ngx_resolver_tcp_read(ngx_event_t *rev)
{
    u_char                     *p;
    আকার_t                      আকার_2;
    ssize_t                     n;
    u_short                     qlen;
    ngx_buf_t                  *b;
    ngx_resolver_t             *r;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;

    c = rev->তথ্য;
    rec = c->তথ্য;
    b = rec->read_buf;
    r = rec->resolver;

    যখন (rev->ready) {
        n = ngx_recv(c, b->last, b->end - b->last);

        যদি (n == NGX_AGAIN) {
            বিরতি;
        }

        যদি (n == NGX_ERROR || n == 0) {
            যাও failed;
        }

        b->last += n;

        জন্য ( ;; ) {
            p = b->pos;
            আকার_2 = b->last - p;

            যদি (আকার_2 < 2) {
                বিরতি;
            }

            qlen = (u_short) *p++ << 8;
            qlen += *p++;

            যদি (আকার_2 < (আকার_t) (2 + qlen)) {
                বিরতি;
            }

            ngx_resolver_process_response(r, p, qlen, 1);

            b->pos += 2 + qlen;
        }

        যদি (b->pos != b->start) {
            b->last = ngx_movemem(b->start, b->pos, b->last - b->pos);
            b->pos = b->start;
        }
    }

    যদি (ngx_handle_read_event(rev, 0) != NGX_OK) {
        যাও failed;
    }

    ফেরত;

failed:

    ngx_close_connection(c);
    rec->tcp = ফাঁকা;
}


স্থির শূন্য
ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf, আকার_t n,
    ngx_uint_t tcp)
{
    অক্ষর                 *err;
    ngx_uint_t            i, times, ident, qident, flags, code, nqs, nan, trunc,
                          qtype, qclass;
#যদি (NGX_HAVE_INET6)
    ngx_uint_t            qident6;
#endif
    ngx_queue_t          *q;
    ngx_resolver_qs_t    *qs;
    ngx_resolver_hdr_t   *response;
    ngx_resolver_গ্রন্থি_t  *rn;

    যদি (n < আকার(ngx_resolver_hdr_t)) {
        যাও short_response;
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
    যদি ((flags & 0xf870) != 0x8000 || (trunc && tcp)) {
        ngx_log_error(r->log_level, r->log, 0,
                      "invalid %s DNS response %ui fl:%04Xi",
                      tcp ? "TCP" : "UDP", ident, flags);
        ফেরত;
    }

    code = flags & 0xf;

    যদি (code == NGX_RESOLVE_FORMERR) {

        times = 0;

        জন্য (q = ngx_queue_শীর্ষ(&r->name_resend_queue);
             q != ngx_queue_sentinel(&r->name_resend_queue) && times++ < 100;
             q = ngx_queue_পরবর্তী(q))
        {
            rn = ngx_queue_তথ্য(q, ngx_resolver_গ্রন্থি_t, queue);

            যদি (rn->query) {
                qident = (rn->query[0] << 8) + rn->query[1];

                যদি (qident == ident) {
                    যাও dns_error_name;
                }
            }

#যদি (NGX_HAVE_INET6)
            যদি (rn->query6) {
                qident6 = (rn->query6[0] << 8) + rn->query6[1];

                যদি (qident6 == ident) {
                    যাও dns_error_name;
                }
            }
#endif
        }

        যাও dns_error;
    }

    যদি (code > NGX_RESOLVE_REFUSED) {
        যাও dns_error;
    }

    যদি (nqs != 1) {
        err = "invalid number of questions in DNS response";
        যাও done;
    }

    i = আকার(ngx_resolver_hdr_t);

    যখন (i < (ngx_uint_t) n) {

        যদি (buf[i] & 0xc0) {
            err = "unexpected compression pointer in DNS response";
            যাও done;
        }

        যদি (buf[i] == '\0') {
            যাও found;
        }

        i += 1 + buf[i];
    }

    যাও short_response;

found:

    যদি (i++ == আকার(ngx_resolver_hdr_t)) {
        err = "zero-length domain name in DNS response";
        যাও done;
    }

    যদি (i + আকার(ngx_resolver_qs_t) + nan * (2 + আকার(ngx_resolver_an_t))
        > (ngx_uint_t) n)
    {
        যাও short_response;
    }

    qs = (ngx_resolver_qs_t *) &buf[i];

    qtype = (qs->type_hi << 8) + qs->type_lo;
    qclass = (qs->class_hi << 8) + qs->class_lo;

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver DNS response qt:%ui cl:%ui", qtype, qclass);

    যদি (qclass != 1) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unknown query class %ui in DNS response", qclass);
        ফেরত;
    }

    নির্বাচন (qtype) {

    ক্ষেত্র NGX_RESOLVE_A:
#যদি (NGX_HAVE_INET6)
    ক্ষেত্র NGX_RESOLVE_AAAA:
#endif

        ngx_resolver_process_a(r, buf, n, ident, code, qtype, nan, trunc,
                               i + আকার(ngx_resolver_qs_t));

        বিরতি;

    ক্ষেত্র NGX_RESOLVE_SRV:

        ngx_resolver_process_srv(r, buf, n, ident, code, nan, trunc,
                                 i + আকার(ngx_resolver_qs_t));

        বিরতি;

    ক্ষেত্র NGX_RESOLVE_PTR:

        ngx_resolver_process_ptr(r, buf, n, ident, code, nan);

        বিরতি;

    পূর্বনির্ধারিত:
        ngx_log_error(r->log_level, r->log, 0,
                      "unknown query type %ui in DNS response", qtype);
        ফেরত;
    }

    ফেরত;

short_response:

    err = "short DNS response";

done:

    ngx_log_error(r->log_level, r->log, 0, err);

    ফেরত;

dns_error_name:

    ngx_log_error(r->log_level, r->log, 0,
                  "DNS error (%ui: %s), query id:%ui, name:\"%*s\"",
                  code, ngx_resolver_strerror(code), ident,
                  (আকার_t) rn->nlen, rn->name);
    ফেরত;

dns_error:

    ngx_log_error(r->log_level, r->log, 0,
                  "DNS error (%ui: %s), query id:%ui",
                  code, ngx_resolver_strerror(code), ident);
    ফেরত;
}


স্থির শূন্য
ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, আকার_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans)
{
    অক্ষর                       *err;
    u_char                     *cname;
    আকার_t                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    in_addr_t                  *addr;
    ngx_str_t                   name;
    ngx_uint_t                  type, class, qident, naddrs, a, i, j, start;
#যদি (NGX_HAVE_INET6)
    কাঠামো in6_addr            *addr6;
#endif
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *পরবর্তী;
    ngx_resolver_গ্রন্থি_t        *rn;
    ngx_resolver_addr_t        *addrs;
    ngx_resolver_connection_t  *rec;

    যদি (ngx_resolver_copy(r, &name, buf,
                          buf + আকার(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        ফেরত;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver qs:%V", &name);

    hash = ngx_crc32_short(name.তথ্য, name.len);

    /* lock name mutex */

    rn = ngx_resolver_lookup_name(r, &name, hash);

    যদি (rn == ফাঁকা) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.তথ্য);
        যাও failed;
    }

    নির্বাচন (qtype) {

#যদি (NGX_HAVE_INET6)
    ক্ষেত্র NGX_RESOLVE_AAAA:

        যদি (rn->query6 == ফাঁকা || rn->naddrs6 != (u_short) -1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.তথ্য);
            যাও failed;
        }

        যদি (trunc && rn->tcp6) {
            ngx_resolver_free(r, name.তথ্য);
            যাও failed;
        }

        qident = (rn->query6[0] << 8) + rn->query6[1];

        বিরতি;
#endif

    পূর্বনির্ধারিত: /* NGX_RESOLVE_A */

        যদি (rn->query == ফাঁকা || rn->naddrs != (u_short) -1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.তথ্য);
            যাও failed;
        }

        যদি (trunc && rn->tcp) {
            ngx_resolver_free(r, name.তথ্য);
            যাও failed;
        }

        qident = (rn->query[0] << 8) + rn->query[1];
    }

    যদি (ident != qident) {
        ngx_log_error(r->log_level, r->log, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.তথ্য);
        যাও failed;
    }

    ngx_resolver_free(r, name.তথ্য);

    যদি (trunc) {

        ngx_queue_remove(&rn->queue);

        যদি (rn->waiting == ফাঁকা) {
            ngx_rbtree_delete(&r->name_rbtree, &rn->গ্রন্থি);
            ngx_resolver_free_গ্রন্থি(r, rn);
            যাও পরবর্তী;
        }

        rec = r->connections.elts;
        rec = &rec[rn->last_connection];

        নির্বাচন (qtype) {

#যদি (NGX_HAVE_INET6)
        ক্ষেত্র NGX_RESOLVE_AAAA:

            rn->tcp6 = 1;

            (শূন্য) ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen);

            বিরতি;
#endif

        পূর্বনির্ধারিত: /* NGX_RESOLVE_A */

            rn->tcp = 1;

            (শূন্য) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);
        }

        rn->expire = ngx_time() + r->resend_timeout;

        ngx_queue_ঢোকাও_শীর্ষ(&r->name_resend_queue, &rn->queue);

        যাও পরবর্তী;
    }

    যদি (code == 0 && rn->code) {
        code = rn->code;
    }

    যদি (code == 0 && nan == 0) {

#যদি (NGX_HAVE_INET6)
        নির্বাচন (qtype) {

        ক্ষেত্র NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            যদি (rn->naddrs == (u_short) -1) {
                যাও পরবর্তী;
            }

            যদি (rn->naddrs) {
                যাও export;
            }

            বিরতি;

        পূর্বনির্ধারিত: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            যদি (rn->naddrs6 == (u_short) -1) {
                যাও পরবর্তী;
            }

            যদি (rn->naddrs6) {
                যাও export;
            }
        }
#endif

        code = NGX_RESOLVE_NXDOMAIN;
    }

    যদি (code) {

#যদি (NGX_HAVE_INET6)
        নির্বাচন (qtype) {

        ক্ষেত্র NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            যদি (rn->naddrs == (u_short) -1) {
                rn->code = (u_char) code;
                যাও পরবর্তী;
            }

            বিরতি;

        পূর্বনির্ধারিত: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            যদি (rn->naddrs6 == (u_short) -1) {
                rn->code = (u_char) code;
                যাও পরবর্তী;
            }
        }
#endif

        পরবর্তী = rn->waiting;
        rn->waiting = ফাঁকা;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(&r->name_rbtree, &rn->গ্রন্থি);

        /* unlock name mutex */

        যখন (পরবর্তী) {
            ctx = পরবর্তী;
            ctx->state = code;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            পরবর্তী = ctx->পরবর্তী;

            ctx->handler(ctx);
        }

        ngx_resolver_free_গ্রন্থি(r, rn);

        ফেরত;
    }

    i = ans;
    naddrs = 0;
    cname = ফাঁকা;

    জন্য (a = 0; a < nan; a++) {

        start = i;

        যখন (i < n) {

            যদি (buf[i] & 0xc0) {
                i += 2;
                যাও found;
            }

            যদি (buf[i] == 0) {
                i++;
                যাও test_দৈর্ঘ্য;
            }

            i += 1 + buf[i];
        }

        যাও short_response;

    test_দৈর্ঘ্য:

        যদি (i - start < 2) {
            err = "invalid name in DNS response";
            যাও invalid;
        }

    found:

        যদি (i + আকার(ngx_resolver_an_t) >= n) {
            যাও short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        যদি (class != 1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR class %ui in DNS response", class);
            যাও failed;
        }

        যদি (ttl < 0) {
            ttl = 0;
        }

        rn->ttl = ngx_min(rn->ttl, (uint32_t) ttl);

        i += আকার(ngx_resolver_an_t);

        নির্বাচন (type) {

        ক্ষেত্র NGX_RESOLVE_A:

            যদি (qtype != NGX_RESOLVE_A) {
                err = "unexpected A record in DNS response";
                যাও invalid;
            }

            যদি (len != 4) {
                err = "invalid A record in DNS response";
                যাও invalid;
            }

            যদি (i + 4 > n) {
                যাও short_response;
            }

            naddrs++;

            বিরতি;

#যদি (NGX_HAVE_INET6)
        ক্ষেত্র NGX_RESOLVE_AAAA:

            যদি (qtype != NGX_RESOLVE_AAAA) {
                err = "unexpected AAAA record in DNS response";
                যাও invalid;
            }

            যদি (len != 16) {
                err = "invalid AAAA record in DNS response";
                যাও invalid;
            }

            যদি (i + 16 > n) {
                যাও short_response;
            }

            naddrs++;

            বিরতি;
#endif

        ক্ষেত্র NGX_RESOLVE_CNAME:

            cname = &buf[i];

            বিরতি;

        ক্ষেত্র NGX_RESOLVE_DNAME:

            বিরতি;

        পূর্বনির্ধারিত:

            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver naddrs:%ui cname:%p ttl:%uD",
                   naddrs, cname, rn->ttl);

    যদি (naddrs) {

        নির্বাচন (qtype) {

#যদি (NGX_HAVE_INET6)
        ক্ষেত্র NGX_RESOLVE_AAAA:

            যদি (naddrs == 1) {
                addr6 = &rn->u6.addr6;
                rn->naddrs6 = 1;

            } অন্যথা {
                addr6 = ngx_resolver_alloc(r, naddrs * আকার(কাঠামো in6_addr));
                যদি (addr6 == ফাঁকা) {
                    যাও failed;
                }

                rn->u6.addrs6 = addr6;
                rn->naddrs6 = (u_short) naddrs;
            }

#যদি (NGX_SUPPRESS_WARN)
            addr = ফাঁকা;
#endif

            বিরতি;
#endif

        পূর্বনির্ধারিত: /* NGX_RESOLVE_A */

            যদি (naddrs == 1) {
                addr = &rn->u.addr;
                rn->naddrs = 1;

            } অন্যথা {
                addr = ngx_resolver_alloc(r, naddrs * আকার(in_addr_t));
                যদি (addr == ফাঁকা) {
                    যাও failed;
                }

                rn->u.addrs = addr;
                rn->naddrs = (u_short) naddrs;
            }

#যদি (NGX_HAVE_INET6 && NGX_SUPPRESS_WARN)
            addr6 = ফাঁকা;
#endif
        }

        j = 0;
        i = ans;

        জন্য (a = 0; a < nan; a++) {

            জন্য ( ;; ) {

                যদি (buf[i] & 0xc0) {
                    i += 2;
                    বিরতি;
                }

                যদি (buf[i] == 0) {
                    i++;
                    বিরতি;
                }

                i += 1 + buf[i];
            }

            an = (ngx_resolver_an_t *) &buf[i];

            type = (an->type_hi << 8) + an->type_lo;
            len = (an->len_hi << 8) + an->len_lo;

            i += আকার(ngx_resolver_an_t);

            যদি (type == NGX_RESOLVE_A) {

                addr[j] = htonl((buf[i] << 24) + (buf[i + 1] << 16)
                                + (buf[i + 2] << 8) + (buf[i + 3]));

                যদি (++j == naddrs) {

#যদি (NGX_HAVE_INET6)
                    যদি (rn->naddrs6 == (u_short) -1) {
                        যাও পরবর্তী;
                    }
#endif

                    বিরতি;
                }
            }

#যদি (NGX_HAVE_INET6)
            অন্যথা যদি (type == NGX_RESOLVE_AAAA) {

                ngx_memcpy(addr6[j].s6_addr, &buf[i], 16);

                যদি (++j == naddrs) {

                    যদি (rn->naddrs == (u_short) -1) {
                        যাও পরবর্তী;
                    }

                    বিরতি;
                }
            }
#endif

            i += len;
        }
    }

    নির্বাচন (qtype) {

#যদি (NGX_HAVE_INET6)
    ক্ষেত্র NGX_RESOLVE_AAAA:

        যদি (rn->naddrs6 == (u_short) -1) {
            rn->naddrs6 = 0;
        }

        বিরতি;
#endif

    পূর্বনির্ধারিত: /* NGX_RESOLVE_A */

        যদি (rn->naddrs == (u_short) -1) {
            rn->naddrs = 0;
        }
    }

    যদি (rn->naddrs != (u_short) -1
#যদি (NGX_HAVE_INET6)
        && rn->naddrs6 != (u_short) -1
#endif
        && rn->naddrs
#যদি (NGX_HAVE_INET6)
           + rn->naddrs6
#endif
           > 0)
    {

#যদি (NGX_HAVE_INET6)
    export:
#endif

        naddrs = rn->naddrs;
#যদি (NGX_HAVE_INET6)
        naddrs += rn->naddrs6;
#endif

        যদি (naddrs == 1 && rn->naddrs == 1) {
            addrs = ফাঁকা;

        } অন্যথা {
            addrs = ngx_resolver_export(r, rn, 0);
            যদি (addrs == ফাঁকা) {
                যাও failed;
            }
        }

        ngx_queue_remove(&rn->queue);

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_ঢোকাও_শীর্ষ(&r->name_expire_queue, &rn->queue);

        পরবর্তী = rn->waiting;
        rn->waiting = ফাঁকা;

        /* unlock name mutex */

        যখন (পরবর্তী) {
            ctx = পরবর্তী;
            ctx->state = NGX_OK;
            ctx->valid = rn->valid;
            ctx->naddrs = naddrs;

            যদি (addrs == ফাঁকা) {
                ctx->addrs = &ctx->addr;
                ctx->addr.sockaddr = (কাঠামো sockaddr *) &ctx->sin;
                ctx->addr.socklen = আকার(কাঠামো sockaddr_in);
                ngx_memzero(&ctx->sin, আকার(কাঠামো sockaddr_in));
                ctx->sin.sin_family = AF_INET;
                ctx->sin.sin_addr.s_addr = rn->u.addr;

            } অন্যথা {
                ctx->addrs = addrs;
            }

            পরবর্তী = ctx->পরবর্তী;

            ctx->handler(ctx);
        }

        যদি (addrs != ফাঁকা) {
            ngx_resolver_free(r, addrs->sockaddr);
            ngx_resolver_free(r, addrs);
        }

        ngx_resolver_free(r, rn->query);
        rn->query = ফাঁকা;
#যদি (NGX_HAVE_INET6)
        rn->query6 = ফাঁকা;
#endif

        ফেরত;
    }

    যদি (cname) {

        /* CNAME only */

        যদি (rn->naddrs == (u_short) -1
#যদি (NGX_HAVE_INET6)
            || rn->naddrs6 == (u_short) -1
#endif
            )
        {
            যাও পরবর্তী;
        }

        যদি (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            যাও failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_queue_remove(&rn->queue);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.তথ্য;

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_ঢোকাও_শীর্ষ(&r->name_expire_queue, &rn->queue);

        ngx_resolver_free(r, rn->query);
        rn->query = ফাঁকা;
#যদি (NGX_HAVE_INET6)
        rn->query6 = ফাঁকা;
#endif

        ctx = rn->waiting;
        rn->waiting = ফাঁকা;

        যদি (ctx) {

            যদি (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                করো {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    পরবর্তী = ctx->পরবর্তী;

                    ctx->handler(ctx);

                    ctx = পরবর্তী;
                } যখন (ctx);

                ফেরত;
            }

            জন্য (পরবর্তী = ctx; পরবর্তী; পরবর্তী = পরবর্তী->পরবর্তী) {
                পরবর্তী->গ্রন্থি = ফাঁকা;
            }

            (শূন্য) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        ফেরত;
    }

    ngx_log_error(r->log_level, r->log, 0,
                  "no A or CNAME types in DNS response");
    ফেরত;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->log, 0, err);

    ফেরত;

failed:

পরবর্তী:

    /* unlock name mutex */

    ফেরত;
}


স্থির শূন্য
ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, আকার_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans)
{
    অক্ষর                       *err;
    u_char                     *cname;
    আকার_t                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    ngx_str_t                   name;
    ngx_uint_t                  type, qident, class, start, nsrvs, a, i, j;
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *পরবর্তী;
    ngx_resolver_srv_t         *srvs;
    ngx_resolver_গ্রন্থি_t        *rn;
    ngx_resolver_connection_t  *rec;

    যদি (ngx_resolver_copy(r, &name, buf,
                          buf + আকার(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        ফেরত;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver qs:%V", &name);

    hash = ngx_crc32_short(name.তথ্য, name.len);

    rn = ngx_resolver_lookup_srv(r, &name, hash);

    যদি (rn == ফাঁকা || rn->query == ফাঁকা) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.তথ্য);
        যাও failed;
    }

    যদি (trunc && rn->tcp) {
        ngx_resolver_free(r, name.তথ্য);
        যাও failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    যদি (ident != qident) {
        ngx_log_error(r->log_level, r->log, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.তথ্য);
        যাও failed;
    }

    ngx_resolver_free(r, name.তথ্য);

    যদি (trunc) {

        ngx_queue_remove(&rn->queue);

        যদি (rn->waiting == ফাঁকা) {
            ngx_rbtree_delete(&r->srv_rbtree, &rn->গ্রন্থি);
            ngx_resolver_free_গ্রন্থি(r, rn);
            ফেরত;
        }

        rec = r->connections.elts;
        rec = &rec[rn->last_connection];

        rn->tcp = 1;

        (শূন্য) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);

        rn->expire = ngx_time() + r->resend_timeout;

        ngx_queue_ঢোকাও_শীর্ষ(&r->srv_resend_queue, &rn->queue);

        ফেরত;
    }

    যদি (code == 0 && rn->code) {
        code = rn->code;
    }

    যদি (code == 0 && nan == 0) {
        code = NGX_RESOLVE_NXDOMAIN;
    }

    যদি (code) {
        পরবর্তী = rn->waiting;
        rn->waiting = ফাঁকা;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(&r->srv_rbtree, &rn->গ্রন্থি);

        যখন (পরবর্তী) {
            ctx = পরবর্তী;
            ctx->state = code;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            পরবর্তী = ctx->পরবর্তী;

            ctx->handler(ctx);
        }

        ngx_resolver_free_গ্রন্থি(r, rn);

        ফেরত;
    }

    i = ans;
    nsrvs = 0;
    cname = ফাঁকা;

    জন্য (a = 0; a < nan; a++) {

        start = i;

        যখন (i < n) {

            যদি (buf[i] & 0xc0) {
                i += 2;
                যাও found;
            }

            যদি (buf[i] == 0) {
                i++;
                যাও test_দৈর্ঘ্য;
            }

            i += 1 + buf[i];
        }

        যাও short_response;

    test_দৈর্ঘ্য:

        যদি (i - start < 2) {
            err = "invalid name DNS response";
            যাও invalid;
        }

    found:

        যদি (i + আকার(ngx_resolver_an_t) >= n) {
            যাও short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        যদি (class != 1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR class %ui in DNS response", class);
            যাও failed;
        }

        যদি (ttl < 0) {
            ttl = 0;
        }

        rn->ttl = ngx_min(rn->ttl, (uint32_t) ttl);

        i += আকার(ngx_resolver_an_t);

        নির্বাচন (type) {

        ক্ষেত্র NGX_RESOLVE_SRV:

            যদি (i + 6 >= n) {
                যাও short_response;
            }

            যদি (ngx_resolver_copy(r, ফাঁকা, buf, &buf[i + 6], buf + n)
                != NGX_OK)
            {
                যাও failed;
            }

            nsrvs++;

            বিরতি;

        ক্ষেত্র NGX_RESOLVE_CNAME:

            cname = &buf[i];

            বিরতি;

        ক্ষেত্র NGX_RESOLVE_DNAME:

            বিরতি;

        পূর্বনির্ধারিত:

            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver nsrvs:%ui cname:%p ttl:%uD",
                   nsrvs, cname, rn->ttl);

    যদি (nsrvs) {

        srvs = ngx_resolver_calloc(r, nsrvs * আকার(ngx_resolver_srv_t));
        যদি (srvs == ফাঁকা) {
            যাও failed;
        }

        rn->u.srvs = srvs;
        rn->nsrvs = (u_short) nsrvs;

        j = 0;
        i = ans;

        জন্য (a = 0; a < nan; a++) {

            জন্য ( ;; ) {

                যদি (buf[i] & 0xc0) {
                    i += 2;
                    বিরতি;
                }

                যদি (buf[i] == 0) {
                    i++;
                    বিরতি;
                }

                i += 1 + buf[i];
            }

            an = (ngx_resolver_an_t *) &buf[i];

            type = (an->type_hi << 8) + an->type_lo;
            len = (an->len_hi << 8) + an->len_lo;

            i += আকার(ngx_resolver_an_t);

            যদি (type == NGX_RESOLVE_SRV) {

                srvs[j].priority = (buf[i] << 8) + buf[i + 1];
                srvs[j].weight = (buf[i + 2] << 8) + buf[i + 3];

                যদি (srvs[j].weight == 0) {
                    srvs[j].weight = 1;
                }

                srvs[j].port = (buf[i + 4] << 8) + buf[i + 5];

                যদি (ngx_resolver_copy(r, &srvs[j].name, buf, &buf[i + 6],
                                      buf + n)
                    != NGX_OK)
                {
                    যাও failed;
                }

                j++;
            }

            i += len;
        }

        ngx_sort(srvs, nsrvs, আকার(ngx_resolver_srv_t),
                 ngx_resolver_cmp_srvs);

        ngx_resolver_free(r, rn->query);
        rn->query = ফাঁকা;

        ngx_queue_remove(&rn->queue);

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_ঢোকাও_শীর্ষ(&r->srv_expire_queue, &rn->queue);

        পরবর্তী = rn->waiting;
        rn->waiting = ফাঁকা;

        যখন (পরবর্তী) {
            ctx = পরবর্তী;
            পরবর্তী = ctx->পরবর্তী;

            ngx_resolver_resolve_srv_names(ctx, rn);
        }

        ফেরত;
    }

    rn->nsrvs = 0;

    যদি (cname) {

        /* CNAME only */

        যদি (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            যাও failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_queue_remove(&rn->queue);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.তথ্য;

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_ঢোকাও_শীর্ষ(&r->srv_expire_queue, &rn->queue);

        ngx_resolver_free(r, rn->query);
        rn->query = ফাঁকা;
#যদি (NGX_HAVE_INET6)
        rn->query6 = ফাঁকা;
#endif

        ctx = rn->waiting;
        rn->waiting = ফাঁকা;

        যদি (ctx) {

            যদি (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                করো {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    পরবর্তী = ctx->পরবর্তী;

                    ctx->handler(ctx);

                    ctx = পরবর্তী;
                } যখন (ctx);

                ফেরত;
            }

            জন্য (পরবর্তী = ctx; পরবর্তী; পরবর্তী = পরবর্তী->পরবর্তী) {
                পরবর্তী->গ্রন্থি = ফাঁকা;
            }

            (শূন্য) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        ফেরত;
    }

    ngx_log_error(r->log_level, r->log, 0, "no SRV type in DNS response");

    ফেরত;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->log, 0, err);

    ফেরত;

failed:

    /* unlock name mutex */

    ফেরত;
}


স্থির শূন্য
ngx_resolver_resolve_srv_names(ngx_resolver_ctx_t *ctx, ngx_resolver_গ্রন্থি_t *rn)
{
    ngx_uint_t                i;
    ngx_resolver_t           *r;
    ngx_resolver_ctx_t       *cctx;
    ngx_resolver_srv_name_t  *srvs;

    r = ctx->resolver;

    ctx->গ্রন্থি = ফাঁকা;
    ctx->state = NGX_OK;
    ctx->valid = rn->valid;
    ctx->গুনতি = rn->nsrvs;

    srvs = ngx_resolver_calloc(r, rn->nsrvs * আকার(ngx_resolver_srv_name_t));
    যদি (srvs == ফাঁকা) {
        যাও failed;
    }

    ctx->srvs = srvs;
    ctx->nsrvs = rn->nsrvs;

    যদি (ctx->event && ctx->event->timer_set) {
        ngx_del_timer(ctx->event);
    }

    জন্য (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
        srvs[i].name.তথ্য = ngx_resolver_alloc(r, rn->u.srvs[i].name.len);
        যদি (srvs[i].name.তথ্য == ফাঁকা) {
            যাও failed;
        }

        srvs[i].name.len = rn->u.srvs[i].name.len;
        ngx_memcpy(srvs[i].name.তথ্য, rn->u.srvs[i].name.তথ্য,
                   srvs[i].name.len);

        cctx = ngx_resolve_start(r, ফাঁকা);
        যদি (cctx == ফাঁকা) {
            যাও failed;
        }

        cctx->name = srvs[i].name;
        cctx->handler = ngx_resolver_srv_names_handler;
        cctx->তথ্য = ctx;
        cctx->srvs = &srvs[i];
        cctx->timeout = ctx->timeout;

        srvs[i].priority = rn->u.srvs[i].priority;
        srvs[i].weight = rn->u.srvs[i].weight;
        srvs[i].port = rn->u.srvs[i].port;
        srvs[i].ctx = cctx;

        যদি (ngx_resolve_name(cctx) == NGX_ERROR) {
            srvs[i].ctx = ফাঁকা;
            যাও failed;
        }
    }

    ফেরত;

failed:

    ctx->state = NGX_ERROR;
    ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

    ctx->handler(ctx);
}


স্থির শূন্য
ngx_resolver_srv_names_handler(ngx_resolver_ctx_t *cctx)
{
    ngx_uint_t                i;
    ngx_addr_t               *addrs;
    ngx_resolver_t           *r;
    ngx_sockaddr_t           *sockaddr;
    ngx_resolver_ctx_t       *ctx;
    ngx_resolver_srv_name_t  *srv;

    r = cctx->resolver;
    ctx = cctx->তথ্য;
    srv = cctx->srvs;

    ctx->গুনতি--;
    ctx->async |= cctx->async;

    srv->ctx = ফাঁকা;
    srv->state = cctx->state;

    যদি (cctx->naddrs) {

        ctx->valid = ngx_min(ctx->valid, cctx->valid);

        addrs = ngx_resolver_calloc(r, cctx->naddrs * আকার(ngx_addr_t));
        যদি (addrs == ফাঁকা) {
            srv->state = NGX_ERROR;
            যাও done;
        }

        sockaddr = ngx_resolver_alloc(r, cctx->naddrs * আকার(ngx_sockaddr_t));
        যদি (sockaddr == ফাঁকা) {
            ngx_resolver_free(r, addrs);
            srv->state = NGX_ERROR;
            যাও done;
        }

        জন্য (i = 0; i < cctx->naddrs; i++) {
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

    যদি (ctx->গুনতি == 0) {
        ngx_resolver_report_srv(r, ctx);
    }
}


স্থির শূন্য
ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, আকার_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan)
{
    অক্ষর                 *err;
    আকার_t                len;
    in_addr_t             addr;
    int32_t               ttl;
    ngx_int_t             octet;
    ngx_str_t             name;
    ngx_uint_t            mask, type, class, qident, a, i, start;
    ngx_queue_t          *expire_queue;
    ngx_rbtree_t         *বৃক্ষ;
    ngx_resolver_an_t    *an;
    ngx_resolver_ctx_t   *ctx, *পরবর্তী;
    ngx_resolver_গ্রন্থি_t  *rn;
#যদি (NGX_HAVE_INET6)
    uint32_t              hash;
    ngx_int_t             digit;
    কাঠামো in6_addr       addr6;
#endif

    যদি (ngx_resolver_copy(r, &name, buf,
                          buf + আকার(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        ফেরত;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver qs:%V", &name);

    /* AF_INET */

    addr = 0;
    i = আকার(ngx_resolver_hdr_t);

    জন্য (mask = 0; mask < 32; mask += 8) {
        len = buf[i++];

        octet = ngx_atoi(&buf[i], len);
        যদি (octet == NGX_ERROR || octet > 255) {
            যাও invalid_in_addr_arpa;
        }

        addr += octet << mask;
        i += len;
    }

    যদি (ngx_strcasecmp(&buf[i], (u_char *) "\7in-addr\4arpa") == 0) {
        i += আকার("\7in-addr\4arpa");

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        বৃক্ষ = &r->addr_rbtree;
        expire_queue = &r->addr_expire_queue;

        যাও valid;
    }

invalid_in_addr_arpa:

#যদি (NGX_HAVE_INET6)

    i = আকার(ngx_resolver_hdr_t);

    জন্য (octet = 15; octet >= 0; octet--) {
        যদি (buf[i++] != '\1') {
            যাও invalid_ip6_arpa;
        }

        digit = ngx_hextoi(&buf[i++], 1);
        যদি (digit == NGX_ERROR) {
            যাও invalid_ip6_arpa;
        }

        addr6.s6_addr[octet] = (u_char) digit;

        যদি (buf[i++] != '\1') {
            যাও invalid_ip6_arpa;
        }

        digit = ngx_hextoi(&buf[i++], 1);
        যদি (digit == NGX_ERROR) {
            যাও invalid_ip6_arpa;
        }

        addr6.s6_addr[octet] += (u_char) (digit * 16);
    }

    যদি (ngx_strcasecmp(&buf[i], (u_char *) "\3ip6\4arpa") == 0) {
        i += আকার("\3ip6\4arpa");

        /* lock addr mutex */

        hash = ngx_crc32_short(addr6.s6_addr, 16);
        rn = ngx_resolver_lookup_addr6(r, &addr6, hash);

        বৃক্ষ = &r->addr6_rbtree;
        expire_queue = &r->addr6_expire_queue;

        যাও valid;
    }

invalid_ip6_arpa:
#endif

    ngx_log_error(r->log_level, r->log, 0,
                  "invalid in-addr.arpa or ip6.arpa name in DNS response");
    ngx_resolver_free(r, name.তথ্য);
    ফেরত;

valid:

    যদি (rn == ফাঁকা || rn->query == ফাঁকা) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.তথ্য);
        যাও failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    যদি (ident != qident) {
        ngx_log_error(r->log_level, r->log, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.তথ্য);
        যাও failed;
    }

    ngx_resolver_free(r, name.তথ্য);

    যদি (code == 0 && nan == 0) {
        code = NGX_RESOLVE_NXDOMAIN;
    }

    যদি (code) {
        পরবর্তী = rn->waiting;
        rn->waiting = ফাঁকা;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(বৃক্ষ, &rn->গ্রন্থি);

        /* unlock addr mutex */

        যখন (পরবর্তী) {
            ctx = পরবর্তী;
            ctx->state = code;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            পরবর্তী = ctx->পরবর্তী;

            ctx->handler(ctx);
        }

        ngx_resolver_free_গ্রন্থি(r, rn);

        ফেরত;
    }

    i += আকার(ngx_resolver_qs_t);

    জন্য (a = 0; a < nan; a++) {

        start = i;

        যখন (i < n) {

            যদি (buf[i] & 0xc0) {
                i += 2;
                যাও found;
            }

            যদি (buf[i] == 0) {
                i++;
                যাও test_দৈর্ঘ্য;
            }

            i += 1 + buf[i];
        }

        যাও short_response;

    test_দৈর্ঘ্য:

        যদি (i - start < 2) {
            err = "invalid name in DNS response";
            যাও invalid;
        }

    found:

        যদি (i + আকার(ngx_resolver_an_t) >= n) {
            যাও short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        যদি (class != 1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR class %ui in DNS response", class);
            যাও failed;
        }

        যদি (ttl < 0) {
            ttl = 0;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                      "resolver qt:%ui cl:%ui len:%uz",
                      type, class, len);

        i += আকার(ngx_resolver_an_t);

        নির্বাচন (type) {

        ক্ষেত্র NGX_RESOLVE_PTR:

            যাও ptr;

        ক্ষেত্র NGX_RESOLVE_CNAME:

            বিরতি;

        পূর্বনির্ধারিত:

            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->log, 0,
                  "no PTR type in DNS response");
    ফেরত;

ptr:

    যদি (ngx_resolver_copy(r, &name, buf, buf + i, buf + n) != NGX_OK) {
        যাও failed;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver an:%V", &name);

    যদি (name.len != (আকার_t) rn->nlen
        || ngx_strncmp(name.তথ্য, rn->name, name.len) != 0)
    {
        যদি (rn->nlen) {
            ngx_resolver_free(r, rn->name);
        }

        rn->nlen = (u_short) name.len;
        rn->name = name.তথ্য;

        name.তথ্য = ngx_resolver_dup(r, rn->name, name.len);
        যদি (name.তথ্য == ফাঁকা) {
            যাও failed;
        }
    }

    ngx_queue_remove(&rn->queue);

    rn->valid = ngx_time() + (r->valid ? r->valid : ttl);
    rn->expire = ngx_time() + r->expire;

    ngx_queue_ঢোকাও_শীর্ষ(expire_queue, &rn->queue);

    পরবর্তী = rn->waiting;
    rn->waiting = ফাঁকা;

    /* unlock addr mutex */

    যখন (পরবর্তী) {
        ctx = পরবর্তী;
        ctx->state = NGX_OK;
        ctx->valid = rn->valid;
        ctx->name = name;
        পরবর্তী = ctx->পরবর্তী;

        ctx->handler(ctx);
    }

    ngx_resolver_free(r, name.তথ্য);

    ফেরত;

short_response:

    err = "short DNS response";

invalid:

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->log, 0, err);

    ফেরত;

failed:

    /* unlock addr mutex */

    ফেরত;
}


স্থির ngx_resolver_গ্রন্থি_t *
ngx_resolver_lookup_name(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_গ্রন্থি_t    *গ্রন্থি, *sentinel;
    ngx_resolver_গ্রন্থি_t  *rn;

    গ্রন্থি = r->name_rbtree.মূল;
    sentinel = r->name_rbtree.sentinel;

    যখন (গ্রন্থি != sentinel) {

        যদি (hash < গ্রন্থি->চাবি) {
            গ্রন্থি = গ্রন্থি->বাম;
            চালু;
        }

        যদি (hash > গ্রন্থি->চাবি) {
            গ্রন্থি = গ্রন্থি->ডান;
            চালু;
        }

        /* hash == node->key */

        rn = ngx_resolver_গ্রন্থি(গ্রন্থি);

        rc = ngx_memn2cmp(name->তথ্য, rn->name, name->len, rn->nlen);

        যদি (rc == 0) {
            ফেরত rn;
        }

        গ্রন্থি = (rc < 0) ? গ্রন্থি->বাম : গ্রন্থি->ডান;
    }

    /* not found */

    ফেরত ফাঁকা;
}


স্থির ngx_resolver_গ্রন্থি_t *
ngx_resolver_lookup_srv(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_গ্রন্থি_t    *গ্রন্থি, *sentinel;
    ngx_resolver_গ্রন্থি_t  *rn;

    গ্রন্থি = r->srv_rbtree.মূল;
    sentinel = r->srv_rbtree.sentinel;

    যখন (গ্রন্থি != sentinel) {

        যদি (hash < গ্রন্থি->চাবি) {
            গ্রন্থি = গ্রন্থি->বাম;
            চালু;
        }

        যদি (hash > গ্রন্থি->চাবি) {
            গ্রন্থি = গ্রন্থি->ডান;
            চালু;
        }

        /* hash == node->key */

        rn = ngx_resolver_গ্রন্থি(গ্রন্থি);

        rc = ngx_memn2cmp(name->তথ্য, rn->name, name->len, rn->nlen);

        যদি (rc == 0) {
            ফেরত rn;
        }

        গ্রন্থি = (rc < 0) ? গ্রন্থি->বাম : গ্রন্থি->ডান;
    }

    /* not found */

    ফেরত ফাঁকা;
}


স্থির ngx_resolver_গ্রন্থি_t *
ngx_resolver_lookup_addr(ngx_resolver_t *r, in_addr_t addr)
{
    ngx_rbtree_গ্রন্থি_t  *গ্রন্থি, *sentinel;

    গ্রন্থি = r->addr_rbtree.মূল;
    sentinel = r->addr_rbtree.sentinel;

    যখন (গ্রন্থি != sentinel) {

        যদি (addr < গ্রন্থি->চাবি) {
            গ্রন্থি = গ্রন্থি->বাম;
            চালু;
        }

        যদি (addr > গ্রন্থি->চাবি) {
            গ্রন্থি = গ্রন্থি->ডান;
            চালু;
        }

        /* addr == node->key */

        ফেরত ngx_resolver_গ্রন্থি(গ্রন্থি);
    }

    /* not found */

    ফেরত ফাঁকা;
}


#যদি (NGX_HAVE_INET6)

স্থির ngx_resolver_গ্রন্থি_t *
ngx_resolver_lookup_addr6(ngx_resolver_t *r, কাঠামো in6_addr *addr,
    uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_গ্রন্থি_t    *গ্রন্থি, *sentinel;
    ngx_resolver_গ্রন্থি_t  *rn;

    গ্রন্থি = r->addr6_rbtree.মূল;
    sentinel = r->addr6_rbtree.sentinel;

    যখন (গ্রন্থি != sentinel) {

        যদি (hash < গ্রন্থি->চাবি) {
            গ্রন্থি = গ্রন্থি->বাম;
            চালু;
        }

        যদি (hash > গ্রন্থি->চাবি) {
            গ্রন্থি = গ্রন্থি->ডান;
            চালু;
        }

        /* hash == node->key */

        rn = ngx_resolver_গ্রন্থি(গ্রন্থি);

        rc = ngx_memcmp(addr, &rn->addr6, 16);

        যদি (rc == 0) {
            ফেরত rn;
        }

        গ্রন্থি = (rc < 0) ? গ্রন্থি->বাম : গ্রন্থি->ডান;
    }

    /* not found */

    ফেরত ফাঁকা;
}

#endif


স্থির শূন্য
ngx_resolver_rbtree_ঢোকাও_মান(ngx_rbtree_গ্রন্থি_t *অস্থায়ী,
    ngx_rbtree_গ্রন্থি_t *গ্রন্থি, ngx_rbtree_গ্রন্থি_t *sentinel)
{
    ngx_rbtree_গ্রন্থি_t    **p;
    ngx_resolver_গ্রন্থি_t   *rn, *rn_অস্থায়ী;

    জন্য ( ;; ) {

        যদি (গ্রন্থি->চাবি < অস্থায়ী->চাবি) {

            p = &অস্থায়ী->বাম;

        } অন্যথা যদি (গ্রন্থি->চাবি > অস্থায়ী->চাবি) {

            p = &অস্থায়ী->ডান;

        } অন্যথা { /* node->key == temp->key */

            rn = ngx_resolver_গ্রন্থি(গ্রন্থি);
            rn_অস্থায়ী = ngx_resolver_গ্রন্থি(অস্থায়ী);

            p = (ngx_memn2cmp(rn->name, rn_অস্থায়ী->name, rn->nlen, rn_অস্থায়ী->nlen)
                 < 0) ? &অস্থায়ী->বাম : &অস্থায়ী->ডান;
        }

        যদি (*p == sentinel) {
            বিরতি;
        }

        অস্থায়ী = *p;
    }

    *p = গ্রন্থি;
    গ্রন্থি->parent = অস্থায়ী;
    গ্রন্থি->বাম = sentinel;
    গ্রন্থি->ডান = sentinel;
    ngx_rbt_red(গ্রন্থি);
}


#যদি (NGX_HAVE_INET6)

স্থির শূন্য
ngx_resolver_rbtree_ঢোকাও_addr6_মান(ngx_rbtree_গ্রন্থি_t *অস্থায়ী,
    ngx_rbtree_গ্রন্থি_t *গ্রন্থি, ngx_rbtree_গ্রন্থি_t *sentinel)
{
    ngx_rbtree_গ্রন্থি_t    **p;
    ngx_resolver_গ্রন্থি_t   *rn, *rn_অস্থায়ী;

    জন্য ( ;; ) {

        যদি (গ্রন্থি->চাবি < অস্থায়ী->চাবি) {

            p = &অস্থায়ী->বাম;

        } অন্যথা যদি (গ্রন্থি->চাবি > অস্থায়ী->চাবি) {

            p = &অস্থায়ী->ডান;

        } অন্যথা { /* node->key == temp->key */

            rn = ngx_resolver_গ্রন্থি(গ্রন্থি);
            rn_অস্থায়ী = ngx_resolver_গ্রন্থি(অস্থায়ী);

            p = (ngx_memcmp(&rn->addr6, &rn_অস্থায়ী->addr6, 16)
                 < 0) ? &অস্থায়ী->বাম : &অস্থায়ী->ডান;
        }

        যদি (*p == sentinel) {
            বিরতি;
        }

        অস্থায়ী = *p;
    }

    *p = গ্রন্থি;
    গ্রন্থি->parent = অস্থায়ী;
    গ্রন্থি->বাম = sentinel;
    গ্রন্থি->ডান = sentinel;
    ngx_rbt_red(গ্রন্থি);
}

#endif


স্থির ngx_int_t
ngx_resolver_create_name_query(ngx_resolver_t *r, ngx_resolver_গ্রন্থি_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    আকার_t               len, nlen;
    ngx_uint_t           ident;
    ngx_resolver_qs_t   *qs;
    ngx_resolver_hdr_t  *query;

    nlen = name->len ? (1 + name->len + 1) : 1;

    len = আকার(ngx_resolver_hdr_t) + nlen + আকার(ngx_resolver_qs_t);

#যদি (NGX_HAVE_INET6)
    p = ngx_resolver_alloc(r, len * (r->ipv4 + r->ipv6));
#অন্যথা
    p = ngx_resolver_alloc(r, len);
#endif
    যদি (p == ফাঁকা) {
        ফেরত NGX_ERROR;
    }

    rn->qlen = (u_short) len;
    rn->query = p;

#যদি (NGX_HAVE_INET6)
    যদি (r->ipv6) {
        rn->query6 = r->ipv4 ? (p + len) : p;
    }
#endif

    query = (ngx_resolver_hdr_t *) p;

    যদি (r->ipv4) {
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

    p += আকার(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    /* query type */
    qs->type_hi = 0; qs->type_lo = NGX_RESOLVE_A;

    /* IN query class */
    qs->class_hi = 0; qs->class_lo = 1;

    /* convert "www.example.com" to "\3www\7example\3com\0" */

    len = 0;
    p--;
    *p-- = '\0';

    যদি (name->len == 0)  {
        ফেরত NGX_DECLINED;
    }

    জন্য (s = name->তথ্য + name->len - 1; s >= name->তথ্য; s--) {
        যদি (*s != '.') {
            *p = *s;
            len++;

        } অন্যথা {
            যদি (len == 0 || len > 255) {
                ফেরত NGX_DECLINED;
            }

            *p = (u_char) len;
            len = 0;
        }

        p--;
    }

    যদি (len == 0 || len > 255) {
        ফেরত NGX_DECLINED;
    }

    *p = (u_char) len;

#যদি (NGX_HAVE_INET6)
    যদি (!r->ipv6) {
        ফেরত NGX_OK;
    }

    p = rn->query6;

    যদি (r->ipv4) {
        ngx_memcpy(p, rn->query, rn->qlen);
    }

    query = (ngx_resolver_hdr_t *) p;

    ident = ngx_random();

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve: \"%V\" AAAA %i", name, ident & 0xffff);

    query->ident_hi = (u_char) ((ident >> 8) & 0xff);
    query->ident_lo = (u_char) (ident & 0xff);

    p += আকার(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    qs->type_lo = NGX_RESOLVE_AAAA;
#endif

    ফেরত NGX_OK;
}


স্থির ngx_int_t
ngx_resolver_create_srv_query(ngx_resolver_t *r, ngx_resolver_গ্রন্থি_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    আকার_t               len, nlen;
    ngx_uint_t           ident;
    ngx_resolver_qs_t   *qs;
    ngx_resolver_hdr_t  *query;

    nlen = name->len ? (1 + name->len + 1) : 1;

    len = আকার(ngx_resolver_hdr_t) + nlen + আকার(ngx_resolver_qs_t);

    p = ngx_resolver_alloc(r, len);
    যদি (p == ফাঁকা) {
        ফেরত NGX_ERROR;
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

    p += আকার(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    /* query type */
    qs->type_hi = 0; qs->type_lo = NGX_RESOLVE_SRV;

    /* IN query class */
    qs->class_hi = 0; qs->class_lo = 1;

    /* converts "www.example.com" to "\3www\7example\3com\0" */

    len = 0;
    p--;
    *p-- = '\0';

    যদি (name->len == 0)  {
        ফেরত NGX_DECLINED;
    }

    জন্য (s = name->তথ্য + name->len - 1; s >= name->তথ্য; s--) {
        যদি (*s != '.') {
            *p = *s;
            len++;

        } অন্যথা {
            যদি (len == 0 || len > 255) {
                ফেরত NGX_DECLINED;
            }

            *p = (u_char) len;
            len = 0;
        }

        p--;
    }

    যদি (len == 0 || len > 255) {
        ফেরত NGX_DECLINED;
    }

    *p = (u_char) len;

    ফেরত NGX_OK;
}


স্থির ngx_int_t
ngx_resolver_create_addr_query(ngx_resolver_t *r, ngx_resolver_গ্রন্থি_t *rn,
    ngx_resolver_addr_t *addr)
{
    u_char               *p, *d;
    আকার_t                len;
    in_addr_t             inaddr;
    ngx_int_t             n;
    ngx_uint_t            ident;
    ngx_resolver_hdr_t   *query;
    কাঠামো sockaddr_in   *sin;
#যদি (NGX_HAVE_INET6)
    কাঠামো sockaddr_in6  *sin6;
#endif

    নির্বাচন (addr->sockaddr->sa_family) {

#যদি (NGX_HAVE_INET6)
    ক্ষেত্র AF_INET6:
        len = আকার(ngx_resolver_hdr_t)
              + 64 + আকার(".ip6.arpa.") - 1
              + আকার(ngx_resolver_qs_t);

        বিরতি;
#endif

    পূর্বনির্ধারিত: /* AF_INET */
        len = আকার(ngx_resolver_hdr_t)
              + আকার(".255.255.255.255.in-addr.arpa.") - 1
              + আকার(ngx_resolver_qs_t);
    }

    p = ngx_resolver_alloc(r, len);
    যদি (p == ফাঁকা) {
        ফেরত NGX_ERROR;
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

    p += আকার(ngx_resolver_hdr_t);

    নির্বাচন (addr->sockaddr->sa_family) {

#যদি (NGX_HAVE_INET6)
    ক্ষেত্র AF_INET6:
        sin6 = (কাঠামো sockaddr_in6 *) addr->sockaddr;

        জন্য (n = 15; n >= 0; n--) {
            p = ngx_sprintf(p, "\1%xd\1%xd",
                            sin6->sin6_addr.s6_addr[n] & 0xf,
                            (sin6->sin6_addr.s6_addr[n] >> 4) & 0xf);
        }

        p = ngx_cpymem(p, "\3ip6\4arpa\0", 10);

        বিরতি;
#endif

    পূর্বনির্ধারিত: /* AF_INET */

        sin = (কাঠামো sockaddr_in *) addr->sockaddr;
        inaddr = ntohl(sin->sin_addr.s_addr);

        জন্য (n = 0; n < 32; n += 8) {
            d = ngx_sprintf(&p[1], "%ud", (inaddr >> n) & 0xff);
            *p = (u_char) (d - &p[1]);
            p = d;
        }

        p = ngx_cpymem(p, "\7in-addr\4arpa\0", 14);
    }

    /* query type "PTR", IN query class */
    p = ngx_cpymem(p, "\0\14\0\1", 4);

    rn->qlen = (u_short) (p - rn->query);

    ফেরত NGX_OK;
}


স্থির ngx_int_t
ngx_resolver_copy(ngx_resolver_t *r, ngx_str_t *name, u_char *buf, u_char *src,
    u_char *last)
{
    অক্ষর        *err;
    u_char      *p, *dst;
    আকার_t       len;
    ngx_uint_t   i, n;

    p = src;
    len = 0;

    /*
     * compression pointers allow to create endless loop, so we set limit;
     * 128 pointers should be enough to store 255-byte name
     */

    জন্য (i = 0; i < 128; i++) {
        n = *p++;

        যদি (n == 0) {
            যাও done;
        }

        যদি (n & 0xc0) {
            যদি ((n & 0xc0) != 0xc0) {
                err = "invalid label type in DNS response";
                যাও invalid;
            }

            যদি (p >= last) {
                err = "name is out of DNS response";
                যাও invalid;
            }

            n = ((n & 0x3f) << 8) + *p;
            p = &buf[n];

        } অন্যথা {
            len += 1 + n;
            p = &p[n];
        }

        যদি (p >= last) {
            err = "name is out of DNS response";
            যাও invalid;
        }
    }

    err = "compression pointers loop in DNS response";

invalid:

    ngx_log_error(r->log_level, r->log, 0, err);

    ফেরত NGX_ERROR;

done:

    যদি (name == ফাঁকা) {
        ফেরত NGX_OK;
    }

    যদি (len == 0) {
        ngx_str_null(name);
        ফেরত NGX_OK;
    }

    dst = ngx_resolver_alloc(r, len);
    যদি (dst == ফাঁকা) {
        ফেরত NGX_ERROR;
    }

    name->তথ্য = dst;

    জন্য ( ;; ) {
        n = *src++;

        যদি (n == 0) {
            name->len = dst - name->তথ্য - 1;
            ফেরত NGX_OK;
        }

        যদি (n & 0xc0) {
            n = ((n & 0x3f) << 8) + *src;
            src = &buf[n];

        } অন্যথা {
            ngx_strlow(dst, src, n);
            dst += n;
            src += n;
            *dst++ = '.';
        }
    }
}


স্থির ngx_int_t
ngx_resolver_set_timeout(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx)
{
    যদি (ctx->event || ctx->timeout == 0) {
        ফেরত NGX_OK;
    }

    ctx->event = ngx_resolver_calloc(r, আকার(ngx_event_t));
    যদি (ctx->event == ফাঁকা) {
        ফেরত NGX_ERROR;
    }

    ctx->event->handler = ngx_resolver_timeout_handler;
    ctx->event->তথ্য = ctx;
    ctx->event->log = r->log;
    ctx->event->cancelable = ctx->cancelable;
    ctx->ident = -1;

    ngx_add_timer(ctx->event, ctx->timeout);

    ফেরত NGX_OK;
}


স্থির শূন্য
ngx_resolver_timeout_handler(ngx_event_t *ev)
{
    ngx_resolver_ctx_t  *ctx;

    ctx = ev->তথ্য;

    ctx->state = NGX_RESOLVE_TIMEDOUT;

    ctx->handler(ctx);
}


স্থির শূন্য
ngx_resolver_free_গ্রন্থি(ngx_resolver_t *r, ngx_resolver_গ্রন্থি_t *rn)
{
    ngx_uint_t  i;

    /* lock alloc mutex */

    যদি (rn->query) {
        ngx_resolver_free_locked(r, rn->query);
    }

    যদি (rn->name) {
        ngx_resolver_free_locked(r, rn->name);
    }

    যদি (rn->cnlen) {
        ngx_resolver_free_locked(r, rn->u.cname);
    }

    যদি (rn->naddrs > 1 && rn->naddrs != (u_short) -1) {
        ngx_resolver_free_locked(r, rn->u.addrs);
    }

#যদি (NGX_HAVE_INET6)
    যদি (rn->naddrs6 > 1 && rn->naddrs6 != (u_short) -1) {
        ngx_resolver_free_locked(r, rn->u6.addrs6);
    }
#endif

    যদি (rn->nsrvs) {
        জন্য (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
            যদি (rn->u.srvs[i].name.তথ্য) {
                ngx_resolver_free_locked(r, rn->u.srvs[i].name.তথ্য);
            }
        }

        ngx_resolver_free_locked(r, rn->u.srvs);
    }

    ngx_resolver_free_locked(r, rn);

    /* unlock alloc mutex */
}


স্থির শূন্য *
ngx_resolver_alloc(ngx_resolver_t *r, আকার_t আকার_2)
{
    u_char  *p;

    /* lock alloc mutex */

    p = ngx_alloc(আকার_2, r->log);

    /* unlock alloc mutex */

    ফেরত p;
}


স্থির শূন্য *
ngx_resolver_calloc(ngx_resolver_t *r, আকার_t আকার_2)
{
    u_char  *p;

    p = ngx_resolver_alloc(r, আকার_2);

    যদি (p) {
        ngx_memzero(p, আকার_2);
    }

    ফেরত p;
}


স্থির শূন্য
ngx_resolver_free(ngx_resolver_t *r, শূন্য *p)
{
    /* lock alloc mutex */

    ngx_free(p);

    /* unlock alloc mutex */
}


স্থির শূন্য
ngx_resolver_free_locked(ngx_resolver_t *r, শূন্য *p)
{
    ngx_free(p);
}


স্থির শূন্য *
ngx_resolver_dup(ngx_resolver_t *r, শূন্য *src, আকার_t আকার_2)
{
    শূন্য  *dst;

    dst = ngx_resolver_alloc(r, আকার_2);

    যদি (dst == ফাঁকা) {
        ফেরত dst;
    }

    ngx_memcpy(dst, src, আকার_2);

    ফেরত dst;
}


স্থির ngx_resolver_addr_t *
ngx_resolver_export(ngx_resolver_t *r, ngx_resolver_গ্রন্থি_t *rn,
    ngx_uint_t rotate)
{
    ngx_uint_t            d, i, j, n;
    in_addr_t            *addr;
    ngx_sockaddr_t       *sockaddr;
    কাঠামো sockaddr_in   *sin;
    ngx_resolver_addr_t  *dst;
#যদি (NGX_HAVE_INET6)
    কাঠামো in6_addr      *addr6;
    কাঠামো sockaddr_in6  *sin6;
#endif

    n = rn->naddrs;
#যদি (NGX_HAVE_INET6)
    n += rn->naddrs6;
#endif

    dst = ngx_resolver_calloc(r, n * আকার(ngx_resolver_addr_t));
    যদি (dst == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    sockaddr = ngx_resolver_calloc(r, n * আকার(ngx_sockaddr_t));
    যদি (sockaddr == ফাঁকা) {
        ngx_resolver_free(r, dst);
        ফেরত ফাঁকা;
    }

    i = 0;
    d = rotate ? ngx_random() % n : 0;

    যদি (rn->naddrs) {
        j = rotate ? ngx_random() % rn->naddrs : 0;

        addr = (rn->naddrs == 1) ? &rn->u.addr : rn->u.addrs;

        করো {
            sin = &sockaddr[d].sockaddr_in;
            sin->sin_family = AF_INET;
            sin->sin_addr.s_addr = addr[j++];
            dst[d].sockaddr = (কাঠামো sockaddr *) sin;
            dst[d++].socklen = আকার(কাঠামো sockaddr_in);

            যদি (d == n) {
                d = 0;
            }

            যদি (j == (ngx_uint_t) rn->naddrs) {
                j = 0;
            }
        } যখন (++i < (ngx_uint_t) rn->naddrs);
    }

#যদি (NGX_HAVE_INET6)
    যদি (rn->naddrs6) {
        j = rotate ? ngx_random() % rn->naddrs6 : 0;

        addr6 = (rn->naddrs6 == 1) ? &rn->u6.addr6 : rn->u6.addrs6;

        করো {
            sin6 = &sockaddr[d].sockaddr_in6;
            sin6->sin6_family = AF_INET6;
            ngx_memcpy(sin6->sin6_addr.s6_addr, addr6[j++].s6_addr, 16);
            dst[d].sockaddr = (কাঠামো sockaddr *) sin6;
            dst[d++].socklen = আকার(কাঠামো sockaddr_in6);

            যদি (d == n) {
                d = 0;
            }

            যদি (j == rn->naddrs6) {
                j = 0;
            }
        } যখন (++i < n);
    }
#endif

    ফেরত dst;
}


স্থির শূন্য
ngx_resolver_report_srv(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx)
{
    ngx_uint_t                naddrs, nsrvs, nw, i, j, k, l, m, n, w;
    ngx_resolver_addr_t      *addrs;
    ngx_resolver_srv_name_t  *srvs;

    srvs = ctx->srvs;
    nsrvs = ctx->nsrvs;

    naddrs = 0;

    জন্য (i = 0; i < nsrvs; i++) {
        যদি (srvs[i].state == NGX_ERROR) {
            ctx->state = NGX_ERROR;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

            ctx->handler(ctx);
            ফেরত;
        }

        naddrs += srvs[i].naddrs;
    }

    যদি (naddrs == 0) {
        ctx->state = srvs[0].state;

        জন্য (i = 0; i < nsrvs; i++) {
            যদি (srvs[i].state == NGX_RESOLVE_NXDOMAIN) {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                বিরতি;
            }
        }

        ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        ফেরত;
    }

    addrs = ngx_resolver_calloc(r, naddrs * আকার(ngx_resolver_addr_t));
    যদি (addrs == ফাঁকা) {
        ctx->state = NGX_ERROR;
        ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        ফেরত;
    }

    i = 0;
    n = 0;

    করো {
        nw = 0;

        জন্য (j = i; j < nsrvs; j++) {
            যদি (srvs[j].priority != srvs[i].priority) {
                বিরতি;
            }

            nw += srvs[j].naddrs * srvs[j].weight;
        }

        যদি (nw == 0) {
            যাও পরবর্তী_srv;
        }

        w = ngx_random() % nw;

        জন্য (k = i; k < j; k++) {
            যদি (w < srvs[k].naddrs * srvs[k].weight) {
                বিরতি;
            }

            w -= srvs[k].naddrs * srvs[k].weight;
        }

        জন্য (l = i; l < j; l++) {

            জন্য (m = 0; m < srvs[k].naddrs; m++) {
                addrs[n].socklen = srvs[k].addrs[m].socklen;
                addrs[n].sockaddr = srvs[k].addrs[m].sockaddr;
                addrs[n].name = srvs[k].name;
                addrs[n].priority = srvs[k].priority;
                addrs[n].weight = srvs[k].weight;
                n++;
            }

            যদি (++k == j) {
                k = i;
            }
        }

পরবর্তী_srv:

        i = j;

    } যখন (i < ctx->nsrvs);

    ctx->state = NGX_OK;
    ctx->addrs = addrs;
    ctx->naddrs = naddrs;

    ctx->handler(ctx);

    ngx_resolver_free(r, addrs);
}


অক্ষর *
ngx_resolver_strerror(ngx_int_t err)
{
    স্থির অক্ষর *errors[] = {
        "Format error",     /* FORMERR */
        "Server failure",   /* SERVFAIL */
        "Host not found",   /* NXDOMAIN */
        "Unimplemented",    /* NOTIMP */
        "Operation refused" /* REFUSED */
    };

    যদি (err > 0 && err < 6) {
        ফেরত errors[err - 1];
    }

    যদি (err == NGX_RESOLVE_TIMEDOUT) {
        ফেরত "Operation timed out";
    }

    ফেরত "Unknown error";
}


স্থির u_char *
ngx_resolver_log_error(ngx_log_t *log, u_char *buf, আকার_t len)
{
    u_char                     *p;
    ngx_resolver_connection_t  *rec;

    p = buf;

    যদি (log->action) {
        p = ngx_snprintf(buf, len, " while %s", log->action);
        len -= p - buf;
    }

    rec = log->তথ্য;

    যদি (rec) {
        p = ngx_snprintf(p, len, ", resolver: %V", &rec->server);
    }

    ফেরত p;
}


স্থির ngx_int_t
ngx_udp_connect(ngx_resolver_connection_t *rec)
{
    পূর্ণসংখ্যা                rc;
    ngx_int_t          event;
    ngx_event_t       *rev, *wev;
    ngx_socket_t       s;
    ngx_connection_t  *c;

    s = ngx_socket(rec->sockaddr->sa_family, SOCK_DGRAM, 0);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "UDP socket %d", s);

    যদি (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_socket_n " failed");
        ফেরত NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->log);

    যদি (c == ফাঁকা) {
        যদি (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        ফেরত NGX_ERROR;
    }

    যদি (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        যাও failed;
    }

    rev = c->পঠ;
    wev = c->লিখ;

    rev->log = &rec->log;
    wev->log = &rec->log;

    rec->udp = c;

    c->সংখ্যা = ngx_atomic_fetch_add(ngx_connection_গণক, 1);

    c->start_time = ngx_current_msec;

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->log, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->সংখ্যা);

    rc = connect(s, rec->sockaddr, rec->socklen);

    যদি (rc == -1) {
        ngx_log_error(NGX_LOG_CRIT, &rec->log, ngx_socket_errno,
                      "connect() failed");

        যাও failed;
    }

    /* UDP sockets are always ready to write */
    wev->ready = 1;

    event = (ngx_event_flags & NGX_USE_CLEAR_EVENT) ?
                /* kqueue, epoll */                 NGX_CLEAR_EVENT:
                /* select, poll, /dev/poll */       NGX_LEVEL_EVENT;
                /* eventport event type has no meaning: oneshot only */

    যদি (ngx_add_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        যাও failed;
    }

    ফেরত NGX_OK;

failed:

    ngx_close_connection(c);
    rec->udp = ফাঁকা;

    ফেরত NGX_ERROR;
}


স্থির ngx_int_t
ngx_tcp_connect(ngx_resolver_connection_t *rec)
{
    পূর্ণসংখ্যা                rc;
    ngx_int_t          event;
    ngx_err_t          err;
    ngx_uint_t         level;
    ngx_socket_t       s;
    ngx_event_t       *rev, *wev;
    ngx_connection_t  *c;

    s = ngx_socket(rec->sockaddr->sa_family, SOCK_STREAM, 0);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "TCP socket %d", s);

    যদি (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_socket_n " failed");
        ফেরত NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->log);

    যদি (c == ফাঁকা) {
        যদি (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        ফেরত NGX_ERROR;
    }

    যদি (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        যাও failed;
    }

    rev = c->পঠ;
    wev = c->লিখ;

    rev->log = &rec->log;
    wev->log = &rec->log;

    rec->tcp = c;

    c->সংখ্যা = ngx_atomic_fetch_add(ngx_connection_গণক, 1);

    c->start_time = ngx_current_msec;

    যদি (ngx_add_conn) {
        যদি (ngx_add_conn(c) == NGX_ERROR) {
            যাও failed;
        }
    }

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->log, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->সংখ্যা);

    rc = connect(s, rec->sockaddr, rec->socklen);

    যদি (rc == -1) {
        err = ngx_socket_errno;


        যদি (err != NGX_EINPROGRESS
#যদি (NGX_WIN32)
            /* Winsock returns WSAEWOULDBLOCK (NGX_EAGAIN) */
            && err != NGX_EAGAIN
#endif
            )
        {
            যদি (err == NGX_ECONNREFUSED
#যদি (NGX_LINUX)
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

            } অন্যথা {
                level = NGX_LOG_CRIT;
            }

            ngx_log_error(level, &rec->log, err, "connect() to %V failed",
                          &rec->server);

            ngx_close_connection(c);
            rec->tcp = ফাঁকা;

            ফেরত NGX_ERROR;
        }
    }

    যদি (ngx_add_conn) {
        যদি (rc == -1) {

            /* NGX_EINPROGRESS */

            ফেরত NGX_AGAIN;
        }

        ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "connected");

        wev->ready = 1;

        ফেরত NGX_OK;
    }

    যদি (ngx_event_flags & NGX_USE_CLEAR_EVENT) {

        /* kqueue */

        event = NGX_CLEAR_EVENT;

    } অন্যথা {

        /* select, poll, /dev/poll */

        event = NGX_LEVEL_EVENT;
    }

    যদি (ngx_add_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        যাও failed;
    }

    যদি (rc == -1) {

        /* NGX_EINPROGRESS */

        যদি (ngx_add_event(wev, NGX_WRITE_EVENT, event) != NGX_OK) {
            যাও failed;
        }

        ফেরত NGX_AGAIN;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "connected");

    wev->ready = 1;

    ফেরত NGX_OK;

failed:

    ngx_close_connection(c);
    rec->tcp = ফাঁকা;

    ফেরত NGX_ERROR;
}


স্থির ngx_int_t
ngx_resolver_cmp_srvs(ধ্রুবক শূন্য *one, ধ্রুবক শূন্য *two)
{
    ngx_int_t            p1, p2;
    ngx_resolver_srv_t  *first, *second;

    first = (ngx_resolver_srv_t *) one;
    second = (ngx_resolver_srv_t *) two;

    p1 = first->priority;
    p2 = second->priority;

    ফেরত p1 - p2;
}
