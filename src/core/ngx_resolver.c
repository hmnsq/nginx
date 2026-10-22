
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>
#समावेश <ngx_event.h>


#घोषणा NGX_RESOLVER_UDP_आकार   4096

#घोषणा NGX_RESOLVER_TCP_RSIZE  (2 + 65535)
#घोषणा NGX_RESOLVER_TCP_WSIZE  8192


प्रकार काष्ठा {
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


प्रकार काष्ठा {
    u_char  type_hi;
    u_char  type_lo;
    u_char  class_hi;
    u_char  class_lo;
} ngx_resolver_qs_t;


प्रकार काष्ठा {
    u_char  type_hi;
    u_char  type_lo;
    u_char  class_hi;
    u_char  class_lo;
    u_char  ttl[4];
    u_char  len_hi;
    u_char  len_lo;
} ngx_resolver_an_t;


#घोषणा ngx_resolver_गाँठ(n)  ngx_rbtree_तथ्य(n, ngx_resolver_गाँठ_t, गाँठ)


अटल ngx_int_t ngx_udp_connect(ngx_resolver_connection_t *rec);
अटल ngx_int_t ngx_tcp_connect(ngx_resolver_connection_t *rec);


अटल व्योम ngx_resolver_cleanup(व्योम *तथ्य);
अटल व्योम ngx_resolver_cleanup_वृक्ष(ngx_resolver_t *r, ngx_rbtree_t *वृक्ष);
अटल ngx_int_t ngx_resolve_name_locked(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx, ngx_str_t *name);
अटल व्योम ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *वृक्ष,
    ngx_queue_t *queue);
अटल ngx_int_t ngx_resolver_send_query(ngx_resolver_t *r,
    ngx_resolver_गाँठ_t *rn);
अटल ngx_int_t ngx_resolver_send_udp_query(ngx_resolver_t *r,
    ngx_resolver_connection_t *rec, u_char *query, u_short qlen);
अटल ngx_int_t ngx_resolver_send_tcp_query(ngx_resolver_t *r,
    ngx_resolver_connection_t *rec, u_char *query, u_short qlen);
अटल ngx_int_t ngx_resolver_create_name_query(ngx_resolver_t *r,
    ngx_resolver_गाँठ_t *rn, ngx_str_t *name);
अटल ngx_int_t ngx_resolver_create_srv_query(ngx_resolver_t *r,
    ngx_resolver_गाँठ_t *rn, ngx_str_t *name);
अटल ngx_int_t ngx_resolver_create_addr_query(ngx_resolver_t *r,
    ngx_resolver_गाँठ_t *rn, ngx_resolver_addr_t *addr);
अटल व्योम ngx_resolver_resend_handler(ngx_event_t *ev);
अटल समय_प्रकार ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *वृक्ष,
    ngx_queue_t *queue);
अटल ngx_uint_t ngx_resolver_resend_empty(ngx_resolver_t *r);
अटल व्योम ngx_resolver_udp_read(ngx_event_t *rev);
अटल व्योम ngx_resolver_tcp_write(ngx_event_t *wev);
अटल व्योम ngx_resolver_tcp_read(ngx_event_t *rev);
अटल व्योम ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf,
    माप_प्रकार n, ngx_uint_t tcp);
अटल व्योम ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, माप_प्रकार n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans);
अटल व्योम ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, माप_प्रकार n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans);
अटल व्योम ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, माप_प्रकार n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan);
अटल ngx_resolver_गाँठ_t *ngx_resolver_lookup_name(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
अटल ngx_resolver_गाँठ_t *ngx_resolver_lookup_srv(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
अटल ngx_resolver_गाँठ_t *ngx_resolver_lookup_addr(ngx_resolver_t *r,
    in_addr_t addr);
अटल व्योम ngx_resolver_rbtree_जोड़ो_मूल्य(ngx_rbtree_गाँठ_t *अस्थायी,
    ngx_rbtree_गाँठ_t *गाँठ, ngx_rbtree_गाँठ_t *sentinel);
अटल ngx_int_t ngx_resolver_copy(ngx_resolver_t *r, ngx_str_t *name,
    u_char *buf, u_char *src, u_char *last);
अटल ngx_int_t ngx_resolver_set_timeout(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx);
अटल व्योम ngx_resolver_timeout_handler(ngx_event_t *ev);
अटल व्योम ngx_resolver_free_गाँठ(ngx_resolver_t *r, ngx_resolver_गाँठ_t *rn);
अटल व्योम *ngx_resolver_alloc(ngx_resolver_t *r, माप_प्रकार आकार);
अटल व्योम *ngx_resolver_calloc(ngx_resolver_t *r, माप_प्रकार आकार);
अटल व्योम ngx_resolver_free(ngx_resolver_t *r, व्योम *p);
अटल व्योम ngx_resolver_free_locked(ngx_resolver_t *r, व्योम *p);
अटल व्योम *ngx_resolver_dup(ngx_resolver_t *r, व्योम *src, माप_प्रकार आकार);
अटल ngx_resolver_addr_t *ngx_resolver_export(ngx_resolver_t *r,
    ngx_resolver_गाँठ_t *rn, ngx_uint_t rotate);
अटल व्योम ngx_resolver_report_srv(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx);
अटल u_char *ngx_resolver_log_error(ngx_log_t *log, u_char *buf, माप_प्रकार len);
अटल व्योम ngx_resolver_resolve_srv_names(ngx_resolver_ctx_t *ctx,
    ngx_resolver_गाँठ_t *rn);
अटल व्योम ngx_resolver_srv_names_handler(ngx_resolver_ctx_t *ctx);
अटल ngx_int_t ngx_resolver_cmp_srvs(स्थिर व्योम *one, स्थिर व्योम *two);

#अगर (NGX_HAVE_INET6)
अटल व्योम ngx_resolver_rbtree_जोड़ो_addr6_मूल्य(ngx_rbtree_गाँठ_t *अस्थायी,
    ngx_rbtree_गाँठ_t *गाँठ, ngx_rbtree_गाँठ_t *sentinel);
अटल ngx_resolver_गाँठ_t *ngx_resolver_lookup_addr6(ngx_resolver_t *r,
    काष्ठा in6_addr *addr, uint32_t hash);
#पूर्ण_अगर


ngx_resolver_t *
ngx_resolver_create(ngx_conf_t *cf, ngx_str_t *names, ngx_uint_t n)
{
    ngx_str_t                   s;
    ngx_url_t                   u;
    ngx_uint_t                  i, j;
    ngx_resolver_t             *r;
    ngx_pool_cleanup_t         *cln;
    ngx_resolver_connection_t  *rec;

    r = ngx_pcalloc(cf->pool, माप(ngx_resolver_t));
    अगर (r == शून्य) {
        वापस शून्य;
    }

    r->event = ngx_pcalloc(cf->pool, माप(ngx_event_t));
    अगर (r->event == शून्य) {
        वापस शून्य;
    }

    cln = ngx_pool_cleanup_add(cf->pool, 0);
    अगर (cln == शून्य) {
        वापस शून्य;
    }

    cln->handler = ngx_resolver_cleanup;
    cln->तथ्य = r;

    r->ipv4 = 1;

    ngx_rbtree_init(&r->name_rbtree, &r->name_sentinel,
                    ngx_resolver_rbtree_जोड़ो_मूल्य);

    ngx_rbtree_init(&r->srv_rbtree, &r->srv_sentinel,
                    ngx_resolver_rbtree_जोड़ो_मूल्य);

    ngx_rbtree_init(&r->addr_rbtree, &r->addr_sentinel,
                    ngx_rbtree_जोड़ो_मूल्य);

    ngx_queue_init(&r->name_resend_queue);
    ngx_queue_init(&r->srv_resend_queue);
    ngx_queue_init(&r->addr_resend_queue);

    ngx_queue_init(&r->name_expire_queue);
    ngx_queue_init(&r->srv_expire_queue);
    ngx_queue_init(&r->addr_expire_queue);

#अगर (NGX_HAVE_INET6)
    r->ipv6 = 1;

    ngx_rbtree_init(&r->addr6_rbtree, &r->addr6_sentinel,
                    ngx_resolver_rbtree_जोड़ो_addr6_मूल्य);

    ngx_queue_init(&r->addr6_resend_queue);

    ngx_queue_init(&r->addr6_expire_queue);
#पूर्ण_अगर

    r->event->handler = ngx_resolver_resend_handler;
    r->event->तथ्य = r;
    r->event->log = &cf->cycle->new_log;
    r->event->cancelable = 1;
    r->ident = -1;

    r->resend_timeout = 5;
    r->tcp_timeout = 5;
    r->expire = 30;
    r->valid = 0;

    r->log = &cf->cycle->new_log;
    r->log_level = NGX_LOG_ERR;

    अगर (n) {
        अगर (ngx_array_init(&r->connections, cf->pool, n,
                           माप(ngx_resolver_connection_t))
            != NGX_OK)
        {
            वापस शून्य;
        }
    }

    क्रम (i = 0; i < n; i++) {
        अगर (ngx_strncmp(names[i].तथ्य, "valid=", 6) == 0) {
            s.len = names[i].len - 6;
            s.तथ्य = names[i].तथ्य + 6;

            r->valid = ngx_parse_time(&s, 1);

            अगर (r->valid == (समय_प्रकार) NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &names[i]);
                वापस शून्य;
            }

            जारी;
        }

#अगर (NGX_HAVE_INET6)
        अगर (ngx_strncmp(names[i].तथ्य, "ipv4=", 5) == 0) {

            अगर (ngx_strcmp(&names[i].तथ्य[5], "on") == 0) {
                r->ipv4 = 1;

            } अन्यथा अगर (ngx_strcmp(&names[i].तथ्य[5], "off") == 0) {
                r->ipv4 = 0;

            } अन्यथा {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &names[i]);
                वापस शून्य;
            }

            जारी;
        }

        अगर (ngx_strncmp(names[i].तथ्य, "ipv6=", 5) == 0) {

            अगर (ngx_strcmp(&names[i].तथ्य[5], "on") == 0) {
                r->ipv6 = 1;

            } अन्यथा अगर (ngx_strcmp(&names[i].तथ्य[5], "off") == 0) {
                r->ipv6 = 0;

            } अन्यथा {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &names[i]);
                वापस शून्य;
            }

            जारी;
        }
#पूर्ण_अगर

        ngx_memzero(&u, माप(ngx_url_t));

        u.url = names[i];
        u.default_port = 53;

        अगर (ngx_parse_url(cf->pool, &u) != NGX_OK) {
            अगर (u.err) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "%s in resolver \"%V\"",
                                   u.err, &u.url);
            }

            वापस शून्य;
        }

        rec = ngx_array_धकेलो_n(&r->connections, u.naddrs);
        अगर (rec == शून्य) {
            वापस शून्य;
        }

        ngx_memzero(rec, u.naddrs * माप(ngx_resolver_connection_t));

        क्रम (j = 0; j < u.naddrs; j++) {
            rec[j].sockaddr = u.addrs[j].sockaddr;
            rec[j].socklen = u.addrs[j].socklen;
            rec[j].server = u.addrs[j].name;
            rec[j].resolver = r;
        }
    }

#अगर (NGX_HAVE_INET6)
    अगर (r->ipv4 + r->ipv6 == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"ipv4\" and \"ipv6\" cannot both be \"off\"");
        वापस शून्य;
    }
#पूर्ण_अगर

    अगर (n && r->connections.nelts == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "no name servers defined");
        वापस शून्य;
    }

    वापस r;
}


अटल व्योम
ngx_resolver_cleanup(व्योम *तथ्य)
{
    ngx_resolver_t  *r = तथ्य;

    ngx_uint_t                  i;
    ngx_resolver_connection_t  *rec;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "cleanup resolver");

    ngx_resolver_cleanup_वृक्ष(r, &r->name_rbtree);

    ngx_resolver_cleanup_वृक्ष(r, &r->srv_rbtree);

    ngx_resolver_cleanup_वृक्ष(r, &r->addr_rbtree);

#अगर (NGX_HAVE_INET6)
    ngx_resolver_cleanup_वृक्ष(r, &r->addr6_rbtree);
#पूर्ण_अगर

    अगर (r->event->timer_set) {
        ngx_del_timer(r->event);
    }

    rec = r->connections.elts;

    क्रम (i = 0; i < r->connections.nelts; i++) {
        अगर (rec[i].udp) {
            ngx_close_connection(rec[i].udp);
        }

        अगर (rec[i].tcp) {
            ngx_close_connection(rec[i].tcp);
        }

        अगर (rec[i].read_buf) {
            ngx_resolver_free(r, rec[i].read_buf->start);
            ngx_resolver_free(r, rec[i].read_buf);
        }

        अगर (rec[i].write_buf) {
            ngx_resolver_free(r, rec[i].write_buf->start);
            ngx_resolver_free(r, rec[i].write_buf);
        }
    }
}


अटल व्योम
ngx_resolver_cleanup_वृक्ष(ngx_resolver_t *r, ngx_rbtree_t *वृक्ष)
{
    ngx_resolver_ctx_t   *ctx, *अग्र;
    ngx_resolver_गाँठ_t  *rn;

    जबतक (वृक्ष->मूल != वृक्ष->sentinel) {

        rn = ngx_resolver_गाँठ(ngx_rbtree_min(वृक्ष->मूल, वृक्ष->sentinel));

        ngx_queue_remove(&rn->queue);

        क्रम (ctx = rn->waiting; ctx; ctx = अग्र) {
            अग्र = ctx->अग्र;

            अगर (ctx->event) {
                अगर (ctx->event->timer_set) {
                    ngx_del_timer(ctx->event);
                }

                ngx_resolver_free(r, ctx->event);
            }

            ngx_resolver_free(r, ctx);
        }

        ngx_rbtree_delete(वृक्ष, &rn->गाँठ);

        ngx_resolver_free_गाँठ(r, rn);
    }
}


ngx_resolver_ctx_t *
ngx_resolve_start(ngx_resolver_t *r, ngx_resolver_ctx_t *अस्थायी)
{
    in_addr_t            addr;
    ngx_resolver_ctx_t  *ctx;

    अगर (अस्थायी) {
        addr = ngx_inet_addr(अस्थायी->name.तथ्य, अस्थायी->name.len);

        अगर (addr != INADDR_NONE) {
            अस्थायी->resolver = r;
            अस्थायी->state = NGX_OK;
            अस्थायी->naddrs = 1;
            अस्थायी->addrs = &अस्थायी->addr;
            अस्थायी->addr.sockaddr = (काष्ठा sockaddr *) &अस्थायी->sin;
            अस्थायी->addr.socklen = माप(काष्ठा sockaddr_in);
            ngx_memzero(&अस्थायी->sin, माप(काष्ठा sockaddr_in));
            अस्थायी->sin.sin_family = AF_INET;
            अस्थायी->sin.sin_addr.s_addr = addr;
            अस्थायी->quick = 1;

            वापस अस्थायी;
        }
    }

    अगर (r->connections.nelts == 0) {
        वापस NGX_NO_RESOLVER;
    }

    ctx = ngx_resolver_calloc(r, माप(ngx_resolver_ctx_t));

    अगर (ctx) {
        ctx->resolver = r;
    }

    वापस ctx;
}


ngx_int_t
ngx_resolve_name(ngx_resolver_ctx_t *ctx)
{
    माप_प्रकार           slen;
    ngx_int_t        rc;
    ngx_str_t        name;
    ngx_resolver_t  *r;

    r = ctx->resolver;

    अगर (ctx->name.len > 0 && ctx->name.तथ्य[ctx->name.len - 1] == '.') {
        ctx->name.len--;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve: \"%V\"", &ctx->name);

    अगर (ctx->quick) {
        ctx->handler(ctx);
        वापस NGX_OK;
    }

    अगर (ctx->service.len) {
        slen = ctx->service.len;

        अगर (ngx_strlchr(ctx->service.तथ्य,
                        ctx->service.तथ्य + ctx->service.len, '.')
            == शून्य)
        {
            slen += माप("_._tcp") - 1;
        }

        name.len = slen + 1 + ctx->name.len;

        name.तथ्य = ngx_resolver_alloc(r, name.len);
        अगर (name.तथ्य == शून्य) {
            जाओ failed;
        }

        अगर (slen == ctx->service.len) {
            ngx_sprintf(name.तथ्य, "%V.%V", &ctx->service, &ctx->name);

        } अन्यथा {
            ngx_sprintf(name.तथ्य, "_%V._tcp.%V", &ctx->service, &ctx->name);
        }

        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &name);

        ngx_resolver_free(r, name.तथ्य);

    } अन्यथा {
        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &ctx->name);
    }

    अगर (rc == NGX_OK) {
        वापस NGX_OK;
    }

    /* unlock name mutex */

    अगर (rc == NGX_AGAIN) {
        वापस NGX_OK;
    }

    /* NGX_ERROR */

    अगर (ctx->event) {
        ngx_resolver_free(r, ctx->event);
    }

failed:

    ngx_resolver_free(r, ctx);

    वापस NGX_ERROR;
}


व्योम
ngx_resolve_name_done(ngx_resolver_ctx_t *ctx)
{
    ngx_uint_t            i;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_गाँठ_t  *rn;

    r = ctx->resolver;

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve name done: %i", ctx->state);

    अगर (ctx->quick) {
        वापस;
    }

    अगर (ctx->event && ctx->event->timer_set) {
        ngx_del_timer(ctx->event);
    }

    /* lock name mutex */

    अगर (ctx->nsrvs) {
        क्रम (i = 0; i < ctx->nsrvs; i++) {
            अगर (ctx->srvs[i].ctx) {
                ngx_resolve_name_done(ctx->srvs[i].ctx);
            }

            अगर (ctx->srvs[i].addrs) {
                ngx_resolver_free(r, ctx->srvs[i].addrs->sockaddr);
                ngx_resolver_free(r, ctx->srvs[i].addrs);
            }

            ngx_resolver_free(r, ctx->srvs[i].name.तथ्य);
        }

        ngx_resolver_free(r, ctx->srvs);
    }

    अगर (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->गाँठ;

        अगर (rn) {
            p = &rn->waiting;
            w = rn->waiting;

            जबतक (w) {
                अगर (w == ctx) {
                    *p = w->अग्र;

                    जाओ done;
                }

                p = &w->अग्र;
                w = w->अग्र;
            }

            ngx_log_error(NGX_LOG_ALERT, r->log, 0,
                          "could not cancel %V resolving", &ctx->name);
        }
    }

done:

    अगर (ctx->service.len) {
        ngx_resolver_expire(r, &r->srv_rbtree, &r->srv_expire_queue);

    } अन्यथा {
        ngx_resolver_expire(r, &r->name_rbtree, &r->name_expire_queue);
    }

    /* unlock name mutex */

    /* lock alloc mutex */

    अगर (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    अगर (r->event->timer_set && ngx_resolver_resend_empty(r)) {
        ngx_del_timer(r->event);
    }
}


अटल ngx_int_t
ngx_resolve_name_locked(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx,
    ngx_str_t *name)
{
    uint32_t              hash;
    ngx_int_t             rc;
    ngx_str_t             cname;
    ngx_uint_t            i, naddrs;
    ngx_queue_t          *resend_queue, *expire_queue;
    ngx_rbtree_t         *वृक्ष;
    ngx_resolver_ctx_t   *अग्र, *last;
    ngx_resolver_addr_t  *addrs;
    ngx_resolver_गाँठ_t  *rn;

    ngx_strlow(name->तथ्य, name->तथ्य, name->len);

    hash = ngx_crc32_short(name->तथ्य, name->len);

    अगर (ctx->service.len) {
        rn = ngx_resolver_lookup_srv(r, name, hash);

        वृक्ष = &r->srv_rbtree;
        resend_queue = &r->srv_resend_queue;
        expire_queue = &r->srv_expire_queue;

    } अन्यथा {
        rn = ngx_resolver_lookup_name(r, name, hash);

        वृक्ष = &r->name_rbtree;
        resend_queue = &r->name_resend_queue;
        expire_queue = &r->name_expire_queue;
    }

    अगर (rn) {

        /* ctx can be a list after NGX_RESOLVE_CNAME */
        क्रम (last = ctx; last->अग्र; last = last->अग्र);

        अगर (rn->valid >= ngx_time()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0, "resolve cached");

            ngx_queue_remove(&rn->queue);

            rn->expire = ngx_time() + r->expire;

            ngx_queue_जोड़ो_शीर्ष(expire_queue, &rn->queue);

            naddrs = (rn->naddrs == (u_short) -1) ? 0 : rn->naddrs;
#अगर (NGX_HAVE_INET6)
            naddrs += (rn->naddrs6 == (u_short) -1) ? 0 : rn->naddrs6;
#पूर्ण_अगर

            अगर (naddrs) {

                अगर (naddrs == 1 && rn->naddrs == 1) {
                    addrs = शून्य;

                } अन्यथा {
                    addrs = ngx_resolver_export(r, rn, 1);
                    अगर (addrs == शून्य) {
                        वापस NGX_ERROR;
                    }
                }

                last->अग्र = rn->waiting;
                rn->waiting = शून्य;

                /* unlock name mutex */

                करो {
                    ctx->state = NGX_OK;
                    ctx->valid = rn->valid;
                    ctx->naddrs = naddrs;

                    अगर (addrs == शून्य) {
                        ctx->addrs = &ctx->addr;
                        ctx->addr.sockaddr = (काष्ठा sockaddr *) &ctx->sin;
                        ctx->addr.socklen = माप(काष्ठा sockaddr_in);
                        ngx_memzero(&ctx->sin, माप(काष्ठा sockaddr_in));
                        ctx->sin.sin_family = AF_INET;
                        ctx->sin.sin_addr.s_addr = rn->u.addr;

                    } अन्यथा {
                        ctx->addrs = addrs;
                    }

                    अग्र = ctx->अग्र;

                    ctx->handler(ctx);

                    ctx = अग्र;
                } जबतक (ctx);

                अगर (addrs != शून्य) {
                    ngx_resolver_free(r, addrs->sockaddr);
                    ngx_resolver_free(r, addrs);
                }

                वापस NGX_OK;
            }

            अगर (rn->nsrvs) {
                last->अग्र = rn->waiting;
                rn->waiting = शून्य;

                /* unlock name mutex */

                करो {
                    अग्र = ctx->अग्र;

                    ngx_resolver_resolve_srv_names(ctx, rn);

                    ctx = अग्र;
                } जबतक (ctx);

                वापस NGX_OK;
            }

            /* NGX_RESOLVE_CNAME */

            अगर (ctx->recursion++ < NGX_RESOLVER_MAX_RECURSION) {

                cname.len = rn->cnlen;
                cname.तथ्य = rn->u.cname;

                वापस ngx_resolve_name_locked(r, ctx, &cname);
            }

            last->अग्र = rn->waiting;
            rn->waiting = शून्य;

            /* unlock name mutex */

            करो {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
                अग्र = ctx->अग्र;

                ctx->handler(ctx);

                ctx = अग्र;
            } जबतक (ctx);

            वापस NGX_OK;
        }

        अगर (rn->waiting) {
            अगर (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
                वापस NGX_ERROR;
            }

            last->अग्र = rn->waiting;
            rn->waiting = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;

            करो {
                ctx->गाँठ = rn;
                ctx = ctx->अग्र;
            } जबतक (ctx);

            वापस NGX_AGAIN;
        }

        ngx_queue_remove(&rn->queue);

        /* lock alloc mutex */

        अगर (rn->query) {
            ngx_resolver_free_locked(r, rn->query);
            rn->query = शून्य;
#अगर (NGX_HAVE_INET6)
            rn->query6 = शून्य;
#पूर्ण_अगर
        }

        अगर (rn->cnlen) {
            ngx_resolver_free_locked(r, rn->u.cname);
        }

        अगर (rn->naddrs > 1 && rn->naddrs != (u_short) -1) {
            ngx_resolver_free_locked(r, rn->u.addrs);
        }

#अगर (NGX_HAVE_INET6)
        अगर (rn->naddrs6 > 1 && rn->naddrs6 != (u_short) -1) {
            ngx_resolver_free_locked(r, rn->u6.addrs6);
        }
#पूर्ण_अगर

        अगर (rn->nsrvs) {
            क्रम (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
                अगर (rn->u.srvs[i].name.तथ्य) {
                    ngx_resolver_free_locked(r, rn->u.srvs[i].name.तथ्य);
                }
            }

            ngx_resolver_free_locked(r, rn->u.srvs);
        }

        /* unlock alloc mutex */

    } अन्यथा {

        rn = ngx_resolver_alloc(r, माप(ngx_resolver_गाँठ_t));
        अगर (rn == शून्य) {
            वापस NGX_ERROR;
        }

        rn->name = ngx_resolver_dup(r, name->तथ्य, name->len);
        अगर (rn->name == शून्य) {
            ngx_resolver_free(r, rn);
            वापस NGX_ERROR;
        }

        rn->गाँठ.कुंजी = hash;
        rn->nlen = (u_short) name->len;
        rn->query = शून्य;
#अगर (NGX_HAVE_INET6)
        rn->query6 = शून्य;
#पूर्ण_अगर

        ngx_rbtree_जोड़ो(वृक्ष, &rn->गाँठ);
    }

    अगर (ctx->service.len) {
        rc = ngx_resolver_create_srv_query(r, rn, name);

    } अन्यथा {
        rc = ngx_resolver_create_name_query(r, rn, name);
    }

    अगर (rc == NGX_ERROR) {
        जाओ failed;
    }

    अगर (rc == NGX_DECLINED) {
        ngx_rbtree_delete(वृक्ष, &rn->गाँठ);

        ngx_resolver_free(r, rn->query);
        ngx_resolver_free(r, rn->name);
        ngx_resolver_free(r, rn);

        करो {
            ctx->state = NGX_RESOLVE_NXDOMAIN;
            अग्र = ctx->अग्र;

            ctx->handler(ctx);

            ctx = अग्र;
        } जबतक (ctx);

        वापस NGX_OK;
    }

    rn->last_connection = r->last_connection++;
    अगर (r->last_connection == r->connections.nelts) {
        r->last_connection = 0;
    }

    rn->naddrs = r->ipv4 ? (u_short) -1 : 0;
    rn->tcp = 0;
#अगर (NGX_HAVE_INET6)
    rn->naddrs6 = r->ipv6 ? (u_short) -1 : 0;
    rn->tcp6 = 0;
#पूर्ण_अगर
    rn->nsrvs = 0;

    अगर (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->last_connection++;
        अगर (rn->last_connection == r->connections.nelts) {
            rn->last_connection = 0;
        }

        (व्योम) ngx_resolver_send_query(r, rn);
    }

    अगर (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
        जाओ failed;
    }

    अगर (ngx_resolver_resend_empty(r)) {
        ngx_add_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_time() + r->resend_timeout;

    ngx_queue_जोड़ो_शीर्ष(resend_queue, &rn->queue);

    rn->code = 0;
    rn->cnlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_मूल्य;
    rn->waiting = ctx;

    ctx->state = NGX_AGAIN;
    ctx->async = 1;

    करो {
        ctx->गाँठ = rn;
        ctx = ctx->अग्र;
    } जबतक (ctx);

    वापस NGX_AGAIN;

failed:

    ngx_rbtree_delete(वृक्ष, &rn->गाँठ);

    अगर (rn->query) {
        ngx_resolver_free(r, rn->query);
    }

    ngx_resolver_free(r, rn->name);

    ngx_resolver_free(r, rn);

    वापस NGX_ERROR;
}


ngx_int_t
ngx_resolve_addr(ngx_resolver_ctx_t *ctx)
{
    u_char               *name;
    in_addr_t             addr;
    ngx_queue_t          *resend_queue, *expire_queue;
    ngx_rbtree_t         *वृक्ष;
    ngx_resolver_t       *r;
    काष्ठा sockaddr_in   *sin;
    ngx_resolver_गाँठ_t  *rn;
#अगर (NGX_HAVE_INET6)
    uint32_t              hash;
    काष्ठा sockaddr_in6  *sin6;
#पूर्ण_अगर

#अगर (NGX_SUPPRESS_WARN)
    addr = 0;
#अगर (NGX_HAVE_INET6)
    hash = 0;
    sin6 = शून्य;
#पूर्ण_अगर
#पूर्ण_अगर

    r = ctx->resolver;

    चयन (ctx->addr.sockaddr->sa_family) {

#अगर (NGX_HAVE_INET6)
    हाल AF_INET6:
        sin6 = (काष्ठा sockaddr_in6 *) ctx->addr.sockaddr;
        hash = ngx_crc32_short(sin6->sin6_addr.s6_addr, 16);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr6(r, &sin6->sin6_addr, hash);

        वृक्ष = &r->addr6_rbtree;
        resend_queue = &r->addr6_resend_queue;
        expire_queue = &r->addr6_expire_queue;

        अवरोध;
#पूर्ण_अगर

    शेष: /* AF_INET */
        sin = (काष्ठा sockaddr_in *) ctx->addr.sockaddr;
        addr = ntohl(sin->sin_addr.s_addr);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        वृक्ष = &r->addr_rbtree;
        resend_queue = &r->addr_resend_queue;
        expire_queue = &r->addr_expire_queue;
    }

    अगर (rn) {

        अगर (rn->valid >= ngx_time()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0, "resolve cached");

            ngx_queue_remove(&rn->queue);

            rn->expire = ngx_time() + r->expire;

            ngx_queue_जोड़ो_शीर्ष(expire_queue, &rn->queue);

            name = ngx_resolver_dup(r, rn->name, rn->nlen);
            अगर (name == शून्य) {
                ngx_resolver_free(r, ctx);
                वापस NGX_ERROR;
            }

            ctx->name.len = rn->nlen;
            ctx->name.तथ्य = name;

            /* unlock addr mutex */

            ctx->state = NGX_OK;
            ctx->valid = rn->valid;

            ctx->handler(ctx);

            ngx_resolver_free(r, name);

            वापस NGX_OK;
        }

        अगर (rn->waiting) {
            अगर (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
                वापस NGX_ERROR;
            }

            ctx->अग्र = rn->waiting;
            rn->waiting = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;
            ctx->गाँठ = rn;

            /* unlock addr mutex */

            वापस NGX_OK;
        }

        ngx_queue_remove(&rn->queue);

        ngx_resolver_free(r, rn->query);
        rn->query = शून्य;
#अगर (NGX_HAVE_INET6)
        rn->query6 = शून्य;
#पूर्ण_अगर

    } अन्यथा {
        rn = ngx_resolver_alloc(r, माप(ngx_resolver_गाँठ_t));
        अगर (rn == शून्य) {
            जाओ failed;
        }

        चयन (ctx->addr.sockaddr->sa_family) {

#अगर (NGX_HAVE_INET6)
        हाल AF_INET6:
            rn->addr6 = sin6->sin6_addr;
            rn->गाँठ.कुंजी = hash;
            अवरोध;
#पूर्ण_अगर

        शेष: /* AF_INET */
            rn->गाँठ.कुंजी = addr;
        }

        rn->query = शून्य;
#अगर (NGX_HAVE_INET6)
        rn->query6 = शून्य;
#पूर्ण_अगर

        ngx_rbtree_जोड़ो(वृक्ष, &rn->गाँठ);
    }

    अगर (ngx_resolver_create_addr_query(r, rn, &ctx->addr) != NGX_OK) {
        जाओ failed;
    }

    rn->last_connection = r->last_connection++;
    अगर (r->last_connection == r->connections.nelts) {
        r->last_connection = 0;
    }

    rn->naddrs = (u_short) -1;
    rn->tcp = 0;
#अगर (NGX_HAVE_INET6)
    rn->naddrs6 = (u_short) -1;
    rn->tcp6 = 0;
#पूर्ण_अगर
    rn->nsrvs = 0;

    अगर (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->last_connection++;
        अगर (rn->last_connection == r->connections.nelts) {
            rn->last_connection = 0;
        }

        (व्योम) ngx_resolver_send_query(r, rn);
    }

    अगर (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
        जाओ failed;
    }

    अगर (ngx_resolver_resend_empty(r)) {
        ngx_add_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_time() + r->resend_timeout;

    ngx_queue_जोड़ो_शीर्ष(resend_queue, &rn->queue);

    rn->code = 0;
    rn->cnlen = 0;
    rn->name = शून्य;
    rn->nlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_मूल्य;
    rn->waiting = ctx;

    /* unlock addr mutex */

    ctx->state = NGX_AGAIN;
    ctx->async = 1;
    ctx->गाँठ = rn;

    वापस NGX_OK;

failed:

    अगर (rn) {
        ngx_rbtree_delete(वृक्ष, &rn->गाँठ);

        अगर (rn->query) {
            ngx_resolver_free(r, rn->query);
        }

        ngx_resolver_free(r, rn);
    }

    /* unlock addr mutex */

    अगर (ctx->event) {
        ngx_resolver_free(r, ctx->event);
    }

    ngx_resolver_free(r, ctx);

    वापस NGX_ERROR;
}


व्योम
ngx_resolve_addr_done(ngx_resolver_ctx_t *ctx)
{
    ngx_queue_t          *expire_queue;
    ngx_rbtree_t         *वृक्ष;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_गाँठ_t  *rn;

    r = ctx->resolver;

    चयन (ctx->addr.sockaddr->sa_family) {

#अगर (NGX_HAVE_INET6)
    हाल AF_INET6:
        वृक्ष = &r->addr6_rbtree;
        expire_queue = &r->addr6_expire_queue;
        अवरोध;
#पूर्ण_अगर

    शेष: /* AF_INET */
        वृक्ष = &r->addr_rbtree;
        expire_queue = &r->addr_expire_queue;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve addr done: %i", ctx->state);

    अगर (ctx->event && ctx->event->timer_set) {
        ngx_del_timer(ctx->event);
    }

    /* lock addr mutex */

    अगर (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->गाँठ;

        अगर (rn) {
            p = &rn->waiting;
            w = rn->waiting;

            जबतक (w) {
                अगर (w == ctx) {
                    *p = w->अग्र;

                    जाओ done;
                }

                p = &w->अग्र;
                w = w->अग्र;
            }
        }

        {
            u_char     text[NGX_SOCKADDR_STRLEN];
            ngx_str_t  addrtext;

            addrtext.तथ्य = text;
            addrtext.len = ngx_sock_ntop(ctx->addr.sockaddr, ctx->addr.socklen,
                                         text, NGX_SOCKADDR_STRLEN, 0);

            ngx_log_error(NGX_LOG_ALERT, r->log, 0,
                          "could not cancel %V resolving", &addrtext);
        }
    }

done:

    ngx_resolver_expire(r, वृक्ष, expire_queue);

    /* unlock addr mutex */

    /* lock alloc mutex */

    अगर (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    अगर (r->event->timer_set && ngx_resolver_resend_empty(r)) {
        ngx_del_timer(r->event);
    }
}


अटल व्योम
ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *वृक्ष, ngx_queue_t *queue)
{
    समय_प्रकार                now;
    ngx_uint_t            i;
    ngx_queue_t          *q;
    ngx_resolver_गाँठ_t  *rn;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver expire");

    now = ngx_time();

    क्रम (i = 0; i < 2; i++) {
        अगर (ngx_queue_empty(queue)) {
            वापस;
        }

        q = ngx_queue_last(queue);

        rn = ngx_queue_तथ्य(q, ngx_resolver_गाँठ_t, queue);

        अगर (now <= rn->expire) {
            वापस;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver expire \"%*s\"", (माप_प्रकार) rn->nlen, rn->name);

        ngx_queue_remove(q);

        ngx_rbtree_delete(वृक्ष, &rn->गाँठ);

        ngx_resolver_free_गाँठ(r, rn);
    }
}


अटल ngx_int_t
ngx_resolver_send_query(ngx_resolver_t *r, ngx_resolver_गाँठ_t *rn)
{
    ngx_int_t                   rc;
    ngx_resolver_connection_t  *rec;

    rec = r->connections.elts;
    rec = &rec[rn->last_connection];

    अगर (rec->log.handler == शून्य) {
        rec->log = *r->log;
        rec->log.handler = ngx_resolver_log_error;
        rec->log.तथ्य = rec;
        rec->log.action = "resolving";
    }

    अगर (rn->query && rn->naddrs == (u_short) -1) {
        rc = rn->tcp ? ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen)
                     : ngx_resolver_send_udp_query(r, rec, rn->query, rn->qlen);

        अगर (rc != NGX_OK) {
            वापस rc;
        }
    }

#अगर (NGX_HAVE_INET6)

    अगर (rn->query6 && rn->naddrs6 == (u_short) -1) {
        rc = rn->tcp6
                    ? ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen)
                    : ngx_resolver_send_udp_query(r, rec, rn->query6, rn->qlen);

        अगर (rc != NGX_OK) {
            वापस rc;
        }
    }

#पूर्ण_अगर

    वापस NGX_OK;
}


अटल ngx_int_t
ngx_resolver_send_udp_query(ngx_resolver_t *r, ngx_resolver_connection_t  *rec,
    u_char *query, u_short qlen)
{
    ssize_t  n;

    अगर (rec->udp == शून्य) {
        अगर (ngx_udp_connect(rec) != NGX_OK) {
            वापस NGX_ERROR;
        }

        rec->udp->तथ्य = rec;
        rec->udp->पढ़ो->handler = ngx_resolver_udp_read;
        rec->udp->पढ़ो->resolver = 1;
    }

    n = ngx_send(rec->udp, query, qlen);

    अगर (n == NGX_ERROR) {
        जाओ failed;
    }

    अगर ((माप_प्रकार) n != (माप_प्रकार) qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->log, 0, "send() incomplete");
        जाओ failed;
    }

    वापस NGX_OK;

failed:

    ngx_close_connection(rec->udp);
    rec->udp = शून्य;

    वापस NGX_ERROR;
}


अटल ngx_int_t
ngx_resolver_send_tcp_query(ngx_resolver_t *r, ngx_resolver_connection_t *rec,
    u_char *query, u_short qlen)
{
    ngx_buf_t  *b;
    ngx_int_t   rc;

    rc = NGX_OK;

    अगर (rec->tcp == शून्य) {
        b = rec->read_buf;

        अगर (b == शून्य) {
            b = ngx_resolver_calloc(r, माप(ngx_buf_t));
            अगर (b == शून्य) {
                वापस NGX_ERROR;
            }

            b->start = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_RSIZE);
            अगर (b->start == शून्य) {
                ngx_resolver_free(r, b);
                वापस NGX_ERROR;
            }

            b->end = b->start + NGX_RESOLVER_TCP_RSIZE;

            rec->read_buf = b;
        }

        b->pos = b->start;
        b->last = b->start;

        b = rec->write_buf;

        अगर (b == शून्य) {
            b = ngx_resolver_calloc(r, माप(ngx_buf_t));
            अगर (b == शून्य) {
                वापस NGX_ERROR;
            }

            b->start = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_WSIZE);
            अगर (b->start == शून्य) {
                ngx_resolver_free(r, b);
                वापस NGX_ERROR;
            }

            b->end = b->start + NGX_RESOLVER_TCP_WSIZE;

            rec->write_buf = b;
        }

        b->pos = b->start;
        b->last = b->start;

        rc = ngx_tcp_connect(rec);
        अगर (rc == NGX_ERROR) {
            वापस NGX_ERROR;
        }

        rec->tcp->तथ्य = rec;
        rec->tcp->ग_लिखो->handler = ngx_resolver_tcp_write;
        rec->tcp->ग_लिखो->cancelable = 1;
        rec->tcp->पढ़ो->handler = ngx_resolver_tcp_read;
        rec->tcp->पढ़ो->resolver = 1;

        ngx_add_timer(rec->tcp->ग_लिखो, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    b = rec->write_buf;

    अगर (b->end - b->last <  2 + qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->log, 0, "buffer overflow");
        वापस NGX_ERROR;
    }

    *b->last++ = (u_char) (qlen >> 8);
    *b->last++ = (u_char) qlen;
    b->last = ngx_cpymem(b->last, query, qlen);

    अगर (rc == NGX_OK) {
        ngx_resolver_tcp_write(rec->tcp->ग_लिखो);
    }

    वापस NGX_OK;
}


अटल व्योम
ngx_resolver_resend_handler(ngx_event_t *ev)
{
    समय_प्रकार           timer, atimer, stimer, ntimer;
#अगर (NGX_HAVE_INET6)
    समय_प्रकार           a6timer;
#पूर्ण_अगर
    ngx_resolver_t  *r;

    r = ev->तथ्य;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver resend handler");

    /* lock name mutex */

    ntimer = ngx_resolver_resend(r, &r->name_rbtree, &r->name_resend_queue);

    stimer = ngx_resolver_resend(r, &r->srv_rbtree, &r->srv_resend_queue);

    /* unlock name mutex */

    /* lock addr mutex */

    atimer = ngx_resolver_resend(r, &r->addr_rbtree, &r->addr_resend_queue);

    /* unlock addr mutex */

#अगर (NGX_HAVE_INET6)

    /* lock addr6 mutex */

    a6timer = ngx_resolver_resend(r, &r->addr6_rbtree, &r->addr6_resend_queue);

    /* unlock addr6 mutex */

#पूर्ण_अगर

    timer = ntimer;

    अगर (timer == 0) {
        timer = atimer;

    } अन्यथा अगर (atimer) {
        timer = ngx_min(timer, atimer);
    }

    अगर (timer == 0) {
        timer = stimer;

    } अन्यथा अगर (stimer) {
        timer = ngx_min(timer, stimer);
    }

#अगर (NGX_HAVE_INET6)

    अगर (timer == 0) {
        timer = a6timer;

    } अन्यथा अगर (a6timer) {
        timer = ngx_min(timer, a6timer);
    }

#पूर्ण_अगर

    अगर (timer) {
        ngx_add_timer(r->event, (ngx_msec_t) (timer * 1000));
    }
}


अटल समय_प्रकार
ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *वृक्ष, ngx_queue_t *queue)
{
    समय_प्रकार                now;
    ngx_queue_t          *q;
    ngx_resolver_गाँठ_t  *rn;

    now = ngx_time();

    क्रम ( ;; ) {
        अगर (ngx_queue_empty(queue)) {
            वापस 0;
        }

        q = ngx_queue_last(queue);

        rn = ngx_queue_तथ्य(q, ngx_resolver_गाँठ_t, queue);

        अगर (now < rn->expire) {
            वापस rn->expire - now;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver resend \"%*s\" %p",
                       (माप_प्रकार) rn->nlen, rn->name, rn->waiting);

        ngx_queue_remove(q);

        अगर (rn->waiting) {

            अगर (++rn->last_connection == r->connections.nelts) {
                rn->last_connection = 0;
            }

            (व्योम) ngx_resolver_send_query(r, rn);

            rn->expire = now + r->resend_timeout;

            ngx_queue_जोड़ो_शीर्ष(queue, q);

            जारी;
        }

        ngx_rbtree_delete(वृक्ष, &rn->गाँठ);

        ngx_resolver_free_गाँठ(r, rn);
    }
}


अटल ngx_uint_t
ngx_resolver_resend_empty(ngx_resolver_t *r)
{
    वापस ngx_queue_empty(&r->name_resend_queue)
           && ngx_queue_empty(&r->srv_resend_queue)
#अगर (NGX_HAVE_INET6)
           && ngx_queue_empty(&r->addr6_resend_queue)
#पूर्ण_अगर
           && ngx_queue_empty(&r->addr_resend_queue);
}


अटल व्योम
ngx_resolver_udp_read(ngx_event_t *rev)
{
    ssize_t                     n;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;
    u_char                      buf[NGX_RESOLVER_UDP_आकार];

    c = rev->तथ्य;
    rec = c->तथ्य;

    करो {
        n = ngx_udp_recv(c, buf, NGX_RESOLVER_UDP_आकार);

        अगर (n == NGX_AGAIN) {
            अवरोध;
        }

        अगर (n == NGX_ERROR) {
            जाओ failed;
        }

        ngx_resolver_process_response(rec->resolver, buf, n, 0);

    } जबतक (rev->ready);

    अगर (ngx_handle_read_event(rev, 0) != NGX_OK) {
        जाओ failed;
    }

    वापस;

failed:

    ngx_close_connection(rec->udp);
    rec->udp = शून्य;
}


अटल व्योम
ngx_resolver_tcp_write(ngx_event_t *wev)
{
    off_t                       sent;
    ssize_t                     n;
    ngx_buf_t                  *b;
    ngx_resolver_t             *r;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;

    c = wev->तथ्य;
    rec = c->तथ्य;
    b = rec->write_buf;
    r = rec->resolver;

    अगर (wev->timedout) {
        जाओ failed;
    }

    sent = c->sent;

    जबतक (wev->ready && b->pos < b->last) {
        n = ngx_send(c, b->pos, b->last - b->pos);

        अगर (n == NGX_AGAIN) {
            अवरोध;
        }

        अगर (n == NGX_ERROR) {
            जाओ failed;
        }

        b->pos += n;
    }

    अगर (b->pos != b->start) {
        b->last = ngx_movemem(b->start, b->pos, b->last - b->pos);
        b->pos = b->start;
    }

    अगर (c->sent != sent) {
        ngx_add_timer(wev, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    अगर (ngx_handle_write_event(wev, 0) != NGX_OK) {
        जाओ failed;
    }

    वापस;

failed:

    ngx_close_connection(c);
    rec->tcp = शून्य;
}


अटल व्योम
ngx_resolver_tcp_read(ngx_event_t *rev)
{
    u_char                     *p;
    माप_प्रकार                      आकार;
    ssize_t                     n;
    u_short                     qlen;
    ngx_buf_t                  *b;
    ngx_resolver_t             *r;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;

    c = rev->तथ्य;
    rec = c->तथ्य;
    b = rec->read_buf;
    r = rec->resolver;

    जबतक (rev->ready) {
        n = ngx_recv(c, b->last, b->end - b->last);

        अगर (n == NGX_AGAIN) {
            अवरोध;
        }

        अगर (n == NGX_ERROR || n == 0) {
            जाओ failed;
        }

        b->last += n;

        क्रम ( ;; ) {
            p = b->pos;
            आकार = b->last - p;

            अगर (आकार < 2) {
                अवरोध;
            }

            qlen = (u_short) *p++ << 8;
            qlen += *p++;

            अगर (आकार < (माप_प्रकार) (2 + qlen)) {
                अवरोध;
            }

            ngx_resolver_process_response(r, p, qlen, 1);

            b->pos += 2 + qlen;
        }

        अगर (b->pos != b->start) {
            b->last = ngx_movemem(b->start, b->pos, b->last - b->pos);
            b->pos = b->start;
        }
    }

    अगर (ngx_handle_read_event(rev, 0) != NGX_OK) {
        जाओ failed;
    }

    वापस;

failed:

    ngx_close_connection(c);
    rec->tcp = शून्य;
}


अटल व्योम
ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf, माप_प्रकार n,
    ngx_uint_t tcp)
{
    अक्षर                 *err;
    ngx_uint_t            i, बार, ident, qident, flags, code, nqs, nan, trunc,
                          qtype, qclass;
#अगर (NGX_HAVE_INET6)
    ngx_uint_t            qident6;
#पूर्ण_अगर
    ngx_queue_t          *q;
    ngx_resolver_qs_t    *qs;
    ngx_resolver_hdr_t   *response;
    ngx_resolver_गाँठ_t  *rn;

    अगर (n < माप(ngx_resolver_hdr_t)) {
        जाओ short_response;
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
    अगर ((flags & 0xf870) != 0x8000 || (trunc && tcp)) {
        ngx_log_error(r->log_level, r->log, 0,
                      "invalid %s DNS response %ui fl:%04Xi",
                      tcp ? "TCP" : "UDP", ident, flags);
        वापस;
    }

    code = flags & 0xf;

    अगर (code == NGX_RESOLVE_FORMERR) {

        बार = 0;

        क्रम (q = ngx_queue_शीर्ष(&r->name_resend_queue);
             q != ngx_queue_sentinel(&r->name_resend_queue) && बार++ < 100;
             q = ngx_queue_अग्र(q))
        {
            rn = ngx_queue_तथ्य(q, ngx_resolver_गाँठ_t, queue);

            अगर (rn->query) {
                qident = (rn->query[0] << 8) + rn->query[1];

                अगर (qident == ident) {
                    जाओ dns_error_name;
                }
            }

#अगर (NGX_HAVE_INET6)
            अगर (rn->query6) {
                qident6 = (rn->query6[0] << 8) + rn->query6[1];

                अगर (qident6 == ident) {
                    जाओ dns_error_name;
                }
            }
#पूर्ण_अगर
        }

        जाओ dns_error;
    }

    अगर (code > NGX_RESOLVE_REFUSED) {
        जाओ dns_error;
    }

    अगर (nqs != 1) {
        err = "invalid number of questions in DNS response";
        जाओ done;
    }

    i = माप(ngx_resolver_hdr_t);

    जबतक (i < (ngx_uint_t) n) {

        अगर (buf[i] & 0xc0) {
            err = "unexpected compression pointer in DNS response";
            जाओ done;
        }

        अगर (buf[i] == '\0') {
            जाओ found;
        }

        i += 1 + buf[i];
    }

    जाओ short_response;

found:

    अगर (i++ == माप(ngx_resolver_hdr_t)) {
        err = "zero-length domain name in DNS response";
        जाओ done;
    }

    अगर (i + माप(ngx_resolver_qs_t) + nan * (2 + माप(ngx_resolver_an_t))
        > (ngx_uint_t) n)
    {
        जाओ short_response;
    }

    qs = (ngx_resolver_qs_t *) &buf[i];

    qtype = (qs->type_hi << 8) + qs->type_lo;
    qclass = (qs->class_hi << 8) + qs->class_lo;

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver DNS response qt:%ui cl:%ui", qtype, qclass);

    अगर (qclass != 1) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unknown query class %ui in DNS response", qclass);
        वापस;
    }

    चयन (qtype) {

    हाल NGX_RESOLVE_A:
#अगर (NGX_HAVE_INET6)
    हाल NGX_RESOLVE_AAAA:
#पूर्ण_अगर

        ngx_resolver_process_a(r, buf, n, ident, code, qtype, nan, trunc,
                               i + माप(ngx_resolver_qs_t));

        अवरोध;

    हाल NGX_RESOLVE_SRV:

        ngx_resolver_process_srv(r, buf, n, ident, code, nan, trunc,
                                 i + माप(ngx_resolver_qs_t));

        अवरोध;

    हाल NGX_RESOLVE_PTR:

        ngx_resolver_process_ptr(r, buf, n, ident, code, nan);

        अवरोध;

    शेष:
        ngx_log_error(r->log_level, r->log, 0,
                      "unknown query type %ui in DNS response", qtype);
        वापस;
    }

    वापस;

short_response:

    err = "short DNS response";

done:

    ngx_log_error(r->log_level, r->log, 0, err);

    वापस;

dns_error_name:

    ngx_log_error(r->log_level, r->log, 0,
                  "DNS error (%ui: %s), query id:%ui, name:\"%*s\"",
                  code, ngx_resolver_strerror(code), ident,
                  (माप_प्रकार) rn->nlen, rn->name);
    वापस;

dns_error:

    ngx_log_error(r->log_level, r->log, 0,
                  "DNS error (%ui: %s), query id:%ui",
                  code, ngx_resolver_strerror(code), ident);
    वापस;
}


अटल व्योम
ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, माप_प्रकार n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans)
{
    अक्षर                       *err;
    u_char                     *cname;
    माप_प्रकार                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    in_addr_t                  *addr;
    ngx_str_t                   name;
    ngx_uint_t                  type, class, qident, naddrs, a, i, j, start;
#अगर (NGX_HAVE_INET6)
    काष्ठा in6_addr            *addr6;
#पूर्ण_अगर
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *अग्र;
    ngx_resolver_गाँठ_t        *rn;
    ngx_resolver_addr_t        *addrs;
    ngx_resolver_connection_t  *rec;

    अगर (ngx_resolver_copy(r, &name, buf,
                          buf + माप(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        वापस;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver qs:%V", &name);

    hash = ngx_crc32_short(name.तथ्य, name.len);

    /* lock name mutex */

    rn = ngx_resolver_lookup_name(r, &name, hash);

    अगर (rn == शून्य) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.तथ्य);
        जाओ failed;
    }

    चयन (qtype) {

#अगर (NGX_HAVE_INET6)
    हाल NGX_RESOLVE_AAAA:

        अगर (rn->query6 == शून्य || rn->naddrs6 != (u_short) -1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.तथ्य);
            जाओ failed;
        }

        अगर (trunc && rn->tcp6) {
            ngx_resolver_free(r, name.तथ्य);
            जाओ failed;
        }

        qident = (rn->query6[0] << 8) + rn->query6[1];

        अवरोध;
#पूर्ण_अगर

    शेष: /* NGX_RESOLVE_A */

        अगर (rn->query == शून्य || rn->naddrs != (u_short) -1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.तथ्य);
            जाओ failed;
        }

        अगर (trunc && rn->tcp) {
            ngx_resolver_free(r, name.तथ्य);
            जाओ failed;
        }

        qident = (rn->query[0] << 8) + rn->query[1];
    }

    अगर (ident != qident) {
        ngx_log_error(r->log_level, r->log, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.तथ्य);
        जाओ failed;
    }

    ngx_resolver_free(r, name.तथ्य);

    अगर (trunc) {

        ngx_queue_remove(&rn->queue);

        अगर (rn->waiting == शून्य) {
            ngx_rbtree_delete(&r->name_rbtree, &rn->गाँठ);
            ngx_resolver_free_गाँठ(r, rn);
            जाओ अग्र;
        }

        rec = r->connections.elts;
        rec = &rec[rn->last_connection];

        चयन (qtype) {

#अगर (NGX_HAVE_INET6)
        हाल NGX_RESOLVE_AAAA:

            rn->tcp6 = 1;

            (व्योम) ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen);

            अवरोध;
#पूर्ण_अगर

        शेष: /* NGX_RESOLVE_A */

            rn->tcp = 1;

            (व्योम) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);
        }

        rn->expire = ngx_time() + r->resend_timeout;

        ngx_queue_जोड़ो_शीर्ष(&r->name_resend_queue, &rn->queue);

        जाओ अग्र;
    }

    अगर (code == 0 && rn->code) {
        code = rn->code;
    }

    अगर (code == 0 && nan == 0) {

#अगर (NGX_HAVE_INET6)
        चयन (qtype) {

        हाल NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            अगर (rn->naddrs == (u_short) -1) {
                जाओ अग्र;
            }

            अगर (rn->naddrs) {
                जाओ export;
            }

            अवरोध;

        शेष: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            अगर (rn->naddrs6 == (u_short) -1) {
                जाओ अग्र;
            }

            अगर (rn->naddrs6) {
                जाओ export;
            }
        }
#पूर्ण_अगर

        code = NGX_RESOLVE_NXDOMAIN;
    }

    अगर (code) {

#अगर (NGX_HAVE_INET6)
        चयन (qtype) {

        हाल NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            अगर (rn->naddrs == (u_short) -1) {
                rn->code = (u_char) code;
                जाओ अग्र;
            }

            अवरोध;

        शेष: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            अगर (rn->naddrs6 == (u_short) -1) {
                rn->code = (u_char) code;
                जाओ अग्र;
            }
        }
#पूर्ण_अगर

        अग्र = rn->waiting;
        rn->waiting = शून्य;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(&r->name_rbtree, &rn->गाँठ);

        /* unlock name mutex */

        जबतक (अग्र) {
            ctx = अग्र;
            ctx->state = code;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            अग्र = ctx->अग्र;

            ctx->handler(ctx);
        }

        ngx_resolver_free_गाँठ(r, rn);

        वापस;
    }

    i = ans;
    naddrs = 0;
    cname = शून्य;

    क्रम (a = 0; a < nan; a++) {

        start = i;

        जबतक (i < n) {

            अगर (buf[i] & 0xc0) {
                i += 2;
                जाओ found;
            }

            अगर (buf[i] == 0) {
                i++;
                जाओ test_लंबाई;
            }

            i += 1 + buf[i];
        }

        जाओ short_response;

    test_लंबाई:

        अगर (i - start < 2) {
            err = "invalid name in DNS response";
            जाओ invalid;
        }

    found:

        अगर (i + माप(ngx_resolver_an_t) >= n) {
            जाओ short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        अगर (class != 1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR class %ui in DNS response", class);
            जाओ failed;
        }

        अगर (ttl < 0) {
            ttl = 0;
        }

        rn->ttl = ngx_min(rn->ttl, (uint32_t) ttl);

        i += माप(ngx_resolver_an_t);

        चयन (type) {

        हाल NGX_RESOLVE_A:

            अगर (qtype != NGX_RESOLVE_A) {
                err = "unexpected A record in DNS response";
                जाओ invalid;
            }

            अगर (len != 4) {
                err = "invalid A record in DNS response";
                जाओ invalid;
            }

            अगर (i + 4 > n) {
                जाओ short_response;
            }

            naddrs++;

            अवरोध;

#अगर (NGX_HAVE_INET6)
        हाल NGX_RESOLVE_AAAA:

            अगर (qtype != NGX_RESOLVE_AAAA) {
                err = "unexpected AAAA record in DNS response";
                जाओ invalid;
            }

            अगर (len != 16) {
                err = "invalid AAAA record in DNS response";
                जाओ invalid;
            }

            अगर (i + 16 > n) {
                जाओ short_response;
            }

            naddrs++;

            अवरोध;
#पूर्ण_अगर

        हाल NGX_RESOLVE_CNAME:

            cname = &buf[i];

            अवरोध;

        हाल NGX_RESOLVE_DNAME:

            अवरोध;

        शेष:

            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver naddrs:%ui cname:%p ttl:%uD",
                   naddrs, cname, rn->ttl);

    अगर (naddrs) {

        चयन (qtype) {

#अगर (NGX_HAVE_INET6)
        हाल NGX_RESOLVE_AAAA:

            अगर (naddrs == 1) {
                addr6 = &rn->u6.addr6;
                rn->naddrs6 = 1;

            } अन्यथा {
                addr6 = ngx_resolver_alloc(r, naddrs * माप(काष्ठा in6_addr));
                अगर (addr6 == शून्य) {
                    जाओ failed;
                }

                rn->u6.addrs6 = addr6;
                rn->naddrs6 = (u_short) naddrs;
            }

#अगर (NGX_SUPPRESS_WARN)
            addr = शून्य;
#पूर्ण_अगर

            अवरोध;
#पूर्ण_अगर

        शेष: /* NGX_RESOLVE_A */

            अगर (naddrs == 1) {
                addr = &rn->u.addr;
                rn->naddrs = 1;

            } अन्यथा {
                addr = ngx_resolver_alloc(r, naddrs * माप(in_addr_t));
                अगर (addr == शून्य) {
                    जाओ failed;
                }

                rn->u.addrs = addr;
                rn->naddrs = (u_short) naddrs;
            }

#अगर (NGX_HAVE_INET6 && NGX_SUPPRESS_WARN)
            addr6 = शून्य;
#पूर्ण_अगर
        }

        j = 0;
        i = ans;

        क्रम (a = 0; a < nan; a++) {

            क्रम ( ;; ) {

                अगर (buf[i] & 0xc0) {
                    i += 2;
                    अवरोध;
                }

                अगर (buf[i] == 0) {
                    i++;
                    अवरोध;
                }

                i += 1 + buf[i];
            }

            an = (ngx_resolver_an_t *) &buf[i];

            type = (an->type_hi << 8) + an->type_lo;
            len = (an->len_hi << 8) + an->len_lo;

            i += माप(ngx_resolver_an_t);

            अगर (type == NGX_RESOLVE_A) {

                addr[j] = htonl((buf[i] << 24) + (buf[i + 1] << 16)
                                + (buf[i + 2] << 8) + (buf[i + 3]));

                अगर (++j == naddrs) {

#अगर (NGX_HAVE_INET6)
                    अगर (rn->naddrs6 == (u_short) -1) {
                        जाओ अग्र;
                    }
#पूर्ण_अगर

                    अवरोध;
                }
            }

#अगर (NGX_HAVE_INET6)
            अन्यथा अगर (type == NGX_RESOLVE_AAAA) {

                ngx_memcpy(addr6[j].s6_addr, &buf[i], 16);

                अगर (++j == naddrs) {

                    अगर (rn->naddrs == (u_short) -1) {
                        जाओ अग्र;
                    }

                    अवरोध;
                }
            }
#पूर्ण_अगर

            i += len;
        }
    }

    चयन (qtype) {

#अगर (NGX_HAVE_INET6)
    हाल NGX_RESOLVE_AAAA:

        अगर (rn->naddrs6 == (u_short) -1) {
            rn->naddrs6 = 0;
        }

        अवरोध;
#पूर्ण_अगर

    शेष: /* NGX_RESOLVE_A */

        अगर (rn->naddrs == (u_short) -1) {
            rn->naddrs = 0;
        }
    }

    अगर (rn->naddrs != (u_short) -1
#अगर (NGX_HAVE_INET6)
        && rn->naddrs6 != (u_short) -1
#पूर्ण_अगर
        && rn->naddrs
#अगर (NGX_HAVE_INET6)
           + rn->naddrs6
#पूर्ण_अगर
           > 0)
    {

#अगर (NGX_HAVE_INET6)
    export:
#पूर्ण_अगर

        naddrs = rn->naddrs;
#अगर (NGX_HAVE_INET6)
        naddrs += rn->naddrs6;
#पूर्ण_अगर

        अगर (naddrs == 1 && rn->naddrs == 1) {
            addrs = शून्य;

        } अन्यथा {
            addrs = ngx_resolver_export(r, rn, 0);
            अगर (addrs == शून्य) {
                जाओ failed;
            }
        }

        ngx_queue_remove(&rn->queue);

        rn->valid = ngx_time() + (r->valid ? r->valid : (समय_प्रकार) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_जोड़ो_शीर्ष(&r->name_expire_queue, &rn->queue);

        अग्र = rn->waiting;
        rn->waiting = शून्य;

        /* unlock name mutex */

        जबतक (अग्र) {
            ctx = अग्र;
            ctx->state = NGX_OK;
            ctx->valid = rn->valid;
            ctx->naddrs = naddrs;

            अगर (addrs == शून्य) {
                ctx->addrs = &ctx->addr;
                ctx->addr.sockaddr = (काष्ठा sockaddr *) &ctx->sin;
                ctx->addr.socklen = माप(काष्ठा sockaddr_in);
                ngx_memzero(&ctx->sin, माप(काष्ठा sockaddr_in));
                ctx->sin.sin_family = AF_INET;
                ctx->sin.sin_addr.s_addr = rn->u.addr;

            } अन्यथा {
                ctx->addrs = addrs;
            }

            अग्र = ctx->अग्र;

            ctx->handler(ctx);
        }

        अगर (addrs != शून्य) {
            ngx_resolver_free(r, addrs->sockaddr);
            ngx_resolver_free(r, addrs);
        }

        ngx_resolver_free(r, rn->query);
        rn->query = शून्य;
#अगर (NGX_HAVE_INET6)
        rn->query6 = शून्य;
#पूर्ण_अगर

        वापस;
    }

    अगर (cname) {

        /* CNAME only */

        अगर (rn->naddrs == (u_short) -1
#अगर (NGX_HAVE_INET6)
            || rn->naddrs6 == (u_short) -1
#पूर्ण_अगर
            )
        {
            जाओ अग्र;
        }

        अगर (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            जाओ failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_queue_remove(&rn->queue);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.तथ्य;

        rn->valid = ngx_time() + (r->valid ? r->valid : (समय_प्रकार) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_जोड़ो_शीर्ष(&r->name_expire_queue, &rn->queue);

        ngx_resolver_free(r, rn->query);
        rn->query = शून्य;
#अगर (NGX_HAVE_INET6)
        rn->query6 = शून्य;
#पूर्ण_अगर

        ctx = rn->waiting;
        rn->waiting = शून्य;

        अगर (ctx) {

            अगर (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                करो {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    अग्र = ctx->अग्र;

                    ctx->handler(ctx);

                    ctx = अग्र;
                } जबतक (ctx);

                वापस;
            }

            क्रम (अग्र = ctx; अग्र; अग्र = अग्र->अग्र) {
                अग्र->गाँठ = शून्य;
            }

            (व्योम) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        वापस;
    }

    ngx_log_error(r->log_level, r->log, 0,
                  "no A or CNAME types in DNS response");
    वापस;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->log, 0, err);

    वापस;

failed:

अग्र:

    /* unlock name mutex */

    वापस;
}


अटल व्योम
ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, माप_प्रकार n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans)
{
    अक्षर                       *err;
    u_char                     *cname;
    माप_प्रकार                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    ngx_str_t                   name;
    ngx_uint_t                  type, qident, class, start, nsrvs, a, i, j;
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *अग्र;
    ngx_resolver_srv_t         *srvs;
    ngx_resolver_गाँठ_t        *rn;
    ngx_resolver_connection_t  *rec;

    अगर (ngx_resolver_copy(r, &name, buf,
                          buf + माप(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        वापस;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver qs:%V", &name);

    hash = ngx_crc32_short(name.तथ्य, name.len);

    rn = ngx_resolver_lookup_srv(r, &name, hash);

    अगर (rn == शून्य || rn->query == शून्य) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.तथ्य);
        जाओ failed;
    }

    अगर (trunc && rn->tcp) {
        ngx_resolver_free(r, name.तथ्य);
        जाओ failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    अगर (ident != qident) {
        ngx_log_error(r->log_level, r->log, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.तथ्य);
        जाओ failed;
    }

    ngx_resolver_free(r, name.तथ्य);

    अगर (trunc) {

        ngx_queue_remove(&rn->queue);

        अगर (rn->waiting == शून्य) {
            ngx_rbtree_delete(&r->srv_rbtree, &rn->गाँठ);
            ngx_resolver_free_गाँठ(r, rn);
            वापस;
        }

        rec = r->connections.elts;
        rec = &rec[rn->last_connection];

        rn->tcp = 1;

        (व्योम) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);

        rn->expire = ngx_time() + r->resend_timeout;

        ngx_queue_जोड़ो_शीर्ष(&r->srv_resend_queue, &rn->queue);

        वापस;
    }

    अगर (code == 0 && rn->code) {
        code = rn->code;
    }

    अगर (code == 0 && nan == 0) {
        code = NGX_RESOLVE_NXDOMAIN;
    }

    अगर (code) {
        अग्र = rn->waiting;
        rn->waiting = शून्य;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(&r->srv_rbtree, &rn->गाँठ);

        जबतक (अग्र) {
            ctx = अग्र;
            ctx->state = code;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            अग्र = ctx->अग्र;

            ctx->handler(ctx);
        }

        ngx_resolver_free_गाँठ(r, rn);

        वापस;
    }

    i = ans;
    nsrvs = 0;
    cname = शून्य;

    क्रम (a = 0; a < nan; a++) {

        start = i;

        जबतक (i < n) {

            अगर (buf[i] & 0xc0) {
                i += 2;
                जाओ found;
            }

            अगर (buf[i] == 0) {
                i++;
                जाओ test_लंबाई;
            }

            i += 1 + buf[i];
        }

        जाओ short_response;

    test_लंबाई:

        अगर (i - start < 2) {
            err = "invalid name DNS response";
            जाओ invalid;
        }

    found:

        अगर (i + माप(ngx_resolver_an_t) >= n) {
            जाओ short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        अगर (class != 1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR class %ui in DNS response", class);
            जाओ failed;
        }

        अगर (ttl < 0) {
            ttl = 0;
        }

        rn->ttl = ngx_min(rn->ttl, (uint32_t) ttl);

        i += माप(ngx_resolver_an_t);

        चयन (type) {

        हाल NGX_RESOLVE_SRV:

            अगर (i + 6 >= n) {
                जाओ short_response;
            }

            अगर (ngx_resolver_copy(r, शून्य, buf, &buf[i + 6], buf + n)
                != NGX_OK)
            {
                जाओ failed;
            }

            nsrvs++;

            अवरोध;

        हाल NGX_RESOLVE_CNAME:

            cname = &buf[i];

            अवरोध;

        हाल NGX_RESOLVE_DNAME:

            अवरोध;

        शेष:

            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver nsrvs:%ui cname:%p ttl:%uD",
                   nsrvs, cname, rn->ttl);

    अगर (nsrvs) {

        srvs = ngx_resolver_calloc(r, nsrvs * माप(ngx_resolver_srv_t));
        अगर (srvs == शून्य) {
            जाओ failed;
        }

        rn->u.srvs = srvs;
        rn->nsrvs = (u_short) nsrvs;

        j = 0;
        i = ans;

        क्रम (a = 0; a < nan; a++) {

            क्रम ( ;; ) {

                अगर (buf[i] & 0xc0) {
                    i += 2;
                    अवरोध;
                }

                अगर (buf[i] == 0) {
                    i++;
                    अवरोध;
                }

                i += 1 + buf[i];
            }

            an = (ngx_resolver_an_t *) &buf[i];

            type = (an->type_hi << 8) + an->type_lo;
            len = (an->len_hi << 8) + an->len_lo;

            i += माप(ngx_resolver_an_t);

            अगर (type == NGX_RESOLVE_SRV) {

                srvs[j].priority = (buf[i] << 8) + buf[i + 1];
                srvs[j].weight = (buf[i + 2] << 8) + buf[i + 3];

                अगर (srvs[j].weight == 0) {
                    srvs[j].weight = 1;
                }

                srvs[j].port = (buf[i + 4] << 8) + buf[i + 5];

                अगर (ngx_resolver_copy(r, &srvs[j].name, buf, &buf[i + 6],
                                      buf + n)
                    != NGX_OK)
                {
                    जाओ failed;
                }

                j++;
            }

            i += len;
        }

        ngx_sort(srvs, nsrvs, माप(ngx_resolver_srv_t),
                 ngx_resolver_cmp_srvs);

        ngx_resolver_free(r, rn->query);
        rn->query = शून्य;

        ngx_queue_remove(&rn->queue);

        rn->valid = ngx_time() + (r->valid ? r->valid : (समय_प्रकार) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_जोड़ो_शीर्ष(&r->srv_expire_queue, &rn->queue);

        अग्र = rn->waiting;
        rn->waiting = शून्य;

        जबतक (अग्र) {
            ctx = अग्र;
            अग्र = ctx->अग्र;

            ngx_resolver_resolve_srv_names(ctx, rn);
        }

        वापस;
    }

    rn->nsrvs = 0;

    अगर (cname) {

        /* CNAME only */

        अगर (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            जाओ failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_queue_remove(&rn->queue);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.तथ्य;

        rn->valid = ngx_time() + (r->valid ? r->valid : (समय_प्रकार) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_जोड़ो_शीर्ष(&r->srv_expire_queue, &rn->queue);

        ngx_resolver_free(r, rn->query);
        rn->query = शून्य;
#अगर (NGX_HAVE_INET6)
        rn->query6 = शून्य;
#पूर्ण_अगर

        ctx = rn->waiting;
        rn->waiting = शून्य;

        अगर (ctx) {

            अगर (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                करो {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    अग्र = ctx->अग्र;

                    ctx->handler(ctx);

                    ctx = अग्र;
                } जबतक (ctx);

                वापस;
            }

            क्रम (अग्र = ctx; अग्र; अग्र = अग्र->अग्र) {
                अग्र->गाँठ = शून्य;
            }

            (व्योम) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        वापस;
    }

    ngx_log_error(r->log_level, r->log, 0, "no SRV type in DNS response");

    वापस;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->log, 0, err);

    वापस;

failed:

    /* unlock name mutex */

    वापस;
}


अटल व्योम
ngx_resolver_resolve_srv_names(ngx_resolver_ctx_t *ctx, ngx_resolver_गाँठ_t *rn)
{
    ngx_uint_t                i;
    ngx_resolver_t           *r;
    ngx_resolver_ctx_t       *cctx;
    ngx_resolver_srv_name_t  *srvs;

    r = ctx->resolver;

    ctx->गाँठ = शून्य;
    ctx->state = NGX_OK;
    ctx->valid = rn->valid;
    ctx->गिनती = rn->nsrvs;

    srvs = ngx_resolver_calloc(r, rn->nsrvs * माप(ngx_resolver_srv_name_t));
    अगर (srvs == शून्य) {
        जाओ failed;
    }

    ctx->srvs = srvs;
    ctx->nsrvs = rn->nsrvs;

    अगर (ctx->event && ctx->event->timer_set) {
        ngx_del_timer(ctx->event);
    }

    क्रम (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
        srvs[i].name.तथ्य = ngx_resolver_alloc(r, rn->u.srvs[i].name.len);
        अगर (srvs[i].name.तथ्य == शून्य) {
            जाओ failed;
        }

        srvs[i].name.len = rn->u.srvs[i].name.len;
        ngx_memcpy(srvs[i].name.तथ्य, rn->u.srvs[i].name.तथ्य,
                   srvs[i].name.len);

        cctx = ngx_resolve_start(r, शून्य);
        अगर (cctx == शून्य) {
            जाओ failed;
        }

        cctx->name = srvs[i].name;
        cctx->handler = ngx_resolver_srv_names_handler;
        cctx->तथ्य = ctx;
        cctx->srvs = &srvs[i];
        cctx->timeout = ctx->timeout;

        srvs[i].priority = rn->u.srvs[i].priority;
        srvs[i].weight = rn->u.srvs[i].weight;
        srvs[i].port = rn->u.srvs[i].port;
        srvs[i].ctx = cctx;

        अगर (ngx_resolve_name(cctx) == NGX_ERROR) {
            srvs[i].ctx = शून्य;
            जाओ failed;
        }
    }

    वापस;

failed:

    ctx->state = NGX_ERROR;
    ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

    ctx->handler(ctx);
}


अटल व्योम
ngx_resolver_srv_names_handler(ngx_resolver_ctx_t *cctx)
{
    ngx_uint_t                i;
    ngx_addr_t               *addrs;
    ngx_resolver_t           *r;
    ngx_sockaddr_t           *sockaddr;
    ngx_resolver_ctx_t       *ctx;
    ngx_resolver_srv_name_t  *srv;

    r = cctx->resolver;
    ctx = cctx->तथ्य;
    srv = cctx->srvs;

    ctx->गिनती--;
    ctx->async |= cctx->async;

    srv->ctx = शून्य;
    srv->state = cctx->state;

    अगर (cctx->naddrs) {

        ctx->valid = ngx_min(ctx->valid, cctx->valid);

        addrs = ngx_resolver_calloc(r, cctx->naddrs * माप(ngx_addr_t));
        अगर (addrs == शून्य) {
            srv->state = NGX_ERROR;
            जाओ done;
        }

        sockaddr = ngx_resolver_alloc(r, cctx->naddrs * माप(ngx_sockaddr_t));
        अगर (sockaddr == शून्य) {
            ngx_resolver_free(r, addrs);
            srv->state = NGX_ERROR;
            जाओ done;
        }

        क्रम (i = 0; i < cctx->naddrs; i++) {
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

    अगर (ctx->गिनती == 0) {
        ngx_resolver_report_srv(r, ctx);
    }
}


अटल व्योम
ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, माप_प्रकार n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan)
{
    अक्षर                 *err;
    माप_प्रकार                len;
    in_addr_t             addr;
    int32_t               ttl;
    ngx_int_t             octet;
    ngx_str_t             name;
    ngx_uint_t            mask, type, class, qident, a, i, start;
    ngx_queue_t          *expire_queue;
    ngx_rbtree_t         *वृक्ष;
    ngx_resolver_an_t    *an;
    ngx_resolver_ctx_t   *ctx, *अग्र;
    ngx_resolver_गाँठ_t  *rn;
#अगर (NGX_HAVE_INET6)
    uint32_t              hash;
    ngx_int_t             digit;
    काष्ठा in6_addr       addr6;
#पूर्ण_अगर

    अगर (ngx_resolver_copy(r, &name, buf,
                          buf + माप(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        वापस;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver qs:%V", &name);

    /* AF_INET */

    addr = 0;
    i = माप(ngx_resolver_hdr_t);

    क्रम (mask = 0; mask < 32; mask += 8) {
        len = buf[i++];

        octet = ngx_atoi(&buf[i], len);
        अगर (octet == NGX_ERROR || octet > 255) {
            जाओ invalid_in_addr_arpa;
        }

        addr += octet << mask;
        i += len;
    }

    अगर (ngx_strcasecmp(&buf[i], (u_char *) "\7in-addr\4arpa") == 0) {
        i += माप("\7in-addr\4arpa");

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        वृक्ष = &r->addr_rbtree;
        expire_queue = &r->addr_expire_queue;

        जाओ valid;
    }

invalid_in_addr_arpa:

#अगर (NGX_HAVE_INET6)

    i = माप(ngx_resolver_hdr_t);

    क्रम (octet = 15; octet >= 0; octet--) {
        अगर (buf[i++] != '\1') {
            जाओ invalid_ip6_arpa;
        }

        digit = ngx_hextoi(&buf[i++], 1);
        अगर (digit == NGX_ERROR) {
            जाओ invalid_ip6_arpa;
        }

        addr6.s6_addr[octet] = (u_char) digit;

        अगर (buf[i++] != '\1') {
            जाओ invalid_ip6_arpa;
        }

        digit = ngx_hextoi(&buf[i++], 1);
        अगर (digit == NGX_ERROR) {
            जाओ invalid_ip6_arpa;
        }

        addr6.s6_addr[octet] += (u_char) (digit * 16);
    }

    अगर (ngx_strcasecmp(&buf[i], (u_char *) "\3ip6\4arpa") == 0) {
        i += माप("\3ip6\4arpa");

        /* lock addr mutex */

        hash = ngx_crc32_short(addr6.s6_addr, 16);
        rn = ngx_resolver_lookup_addr6(r, &addr6, hash);

        वृक्ष = &r->addr6_rbtree;
        expire_queue = &r->addr6_expire_queue;

        जाओ valid;
    }

invalid_ip6_arpa:
#पूर्ण_अगर

    ngx_log_error(r->log_level, r->log, 0,
                  "invalid in-addr.arpa or ip6.arpa name in DNS response");
    ngx_resolver_free(r, name.तथ्य);
    वापस;

valid:

    अगर (rn == शून्य || rn->query == शून्य) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.तथ्य);
        जाओ failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    अगर (ident != qident) {
        ngx_log_error(r->log_level, r->log, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.तथ्य);
        जाओ failed;
    }

    ngx_resolver_free(r, name.तथ्य);

    अगर (code == 0 && nan == 0) {
        code = NGX_RESOLVE_NXDOMAIN;
    }

    अगर (code) {
        अग्र = rn->waiting;
        rn->waiting = शून्य;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(वृक्ष, &rn->गाँठ);

        /* unlock addr mutex */

        जबतक (अग्र) {
            ctx = अग्र;
            ctx->state = code;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            अग्र = ctx->अग्र;

            ctx->handler(ctx);
        }

        ngx_resolver_free_गाँठ(r, rn);

        वापस;
    }

    i += माप(ngx_resolver_qs_t);

    क्रम (a = 0; a < nan; a++) {

        start = i;

        जबतक (i < n) {

            अगर (buf[i] & 0xc0) {
                i += 2;
                जाओ found;
            }

            अगर (buf[i] == 0) {
                i++;
                जाओ test_लंबाई;
            }

            i += 1 + buf[i];
        }

        जाओ short_response;

    test_लंबाई:

        अगर (i - start < 2) {
            err = "invalid name in DNS response";
            जाओ invalid;
        }

    found:

        अगर (i + माप(ngx_resolver_an_t) >= n) {
            जाओ short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        अगर (class != 1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR class %ui in DNS response", class);
            जाओ failed;
        }

        अगर (ttl < 0) {
            ttl = 0;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                      "resolver qt:%ui cl:%ui len:%uz",
                      type, class, len);

        i += माप(ngx_resolver_an_t);

        चयन (type) {

        हाल NGX_RESOLVE_PTR:

            जाओ ptr;

        हाल NGX_RESOLVE_CNAME:

            अवरोध;

        शेष:

            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->log, 0,
                  "no PTR type in DNS response");
    वापस;

ptr:

    अगर (ngx_resolver_copy(r, &name, buf, buf + i, buf + n) != NGX_OK) {
        जाओ failed;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver an:%V", &name);

    अगर (name.len != (माप_प्रकार) rn->nlen
        || ngx_strncmp(name.तथ्य, rn->name, name.len) != 0)
    {
        अगर (rn->nlen) {
            ngx_resolver_free(r, rn->name);
        }

        rn->nlen = (u_short) name.len;
        rn->name = name.तथ्य;

        name.तथ्य = ngx_resolver_dup(r, rn->name, name.len);
        अगर (name.तथ्य == शून्य) {
            जाओ failed;
        }
    }

    ngx_queue_remove(&rn->queue);

    rn->valid = ngx_time() + (r->valid ? r->valid : ttl);
    rn->expire = ngx_time() + r->expire;

    ngx_queue_जोड़ो_शीर्ष(expire_queue, &rn->queue);

    अग्र = rn->waiting;
    rn->waiting = शून्य;

    /* unlock addr mutex */

    जबतक (अग्र) {
        ctx = अग्र;
        ctx->state = NGX_OK;
        ctx->valid = rn->valid;
        ctx->name = name;
        अग्र = ctx->अग्र;

        ctx->handler(ctx);
    }

    ngx_resolver_free(r, name.तथ्य);

    वापस;

short_response:

    err = "short DNS response";

invalid:

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->log, 0, err);

    वापस;

failed:

    /* unlock addr mutex */

    वापस;
}


अटल ngx_resolver_गाँठ_t *
ngx_resolver_lookup_name(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_गाँठ_t    *गाँठ, *sentinel;
    ngx_resolver_गाँठ_t  *rn;

    गाँठ = r->name_rbtree.मूल;
    sentinel = r->name_rbtree.sentinel;

    जबतक (गाँठ != sentinel) {

        अगर (hash < गाँठ->कुंजी) {
            गाँठ = गाँठ->बायाँ;
            जारी;
        }

        अगर (hash > गाँठ->कुंजी) {
            गाँठ = गाँठ->दायाँ;
            जारी;
        }

        /* hash == node->key */

        rn = ngx_resolver_गाँठ(गाँठ);

        rc = ngx_memn2cmp(name->तथ्य, rn->name, name->len, rn->nlen);

        अगर (rc == 0) {
            वापस rn;
        }

        गाँठ = (rc < 0) ? गाँठ->बायाँ : गाँठ->दायाँ;
    }

    /* not found */

    वापस शून्य;
}


अटल ngx_resolver_गाँठ_t *
ngx_resolver_lookup_srv(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_गाँठ_t    *गाँठ, *sentinel;
    ngx_resolver_गाँठ_t  *rn;

    गाँठ = r->srv_rbtree.मूल;
    sentinel = r->srv_rbtree.sentinel;

    जबतक (गाँठ != sentinel) {

        अगर (hash < गाँठ->कुंजी) {
            गाँठ = गाँठ->बायाँ;
            जारी;
        }

        अगर (hash > गाँठ->कुंजी) {
            गाँठ = गाँठ->दायाँ;
            जारी;
        }

        /* hash == node->key */

        rn = ngx_resolver_गाँठ(गाँठ);

        rc = ngx_memn2cmp(name->तथ्य, rn->name, name->len, rn->nlen);

        अगर (rc == 0) {
            वापस rn;
        }

        गाँठ = (rc < 0) ? गाँठ->बायाँ : गाँठ->दायाँ;
    }

    /* not found */

    वापस शून्य;
}


अटल ngx_resolver_गाँठ_t *
ngx_resolver_lookup_addr(ngx_resolver_t *r, in_addr_t addr)
{
    ngx_rbtree_गाँठ_t  *गाँठ, *sentinel;

    गाँठ = r->addr_rbtree.मूल;
    sentinel = r->addr_rbtree.sentinel;

    जबतक (गाँठ != sentinel) {

        अगर (addr < गाँठ->कुंजी) {
            गाँठ = गाँठ->बायाँ;
            जारी;
        }

        अगर (addr > गाँठ->कुंजी) {
            गाँठ = गाँठ->दायाँ;
            जारी;
        }

        /* addr == node->key */

        वापस ngx_resolver_गाँठ(गाँठ);
    }

    /* not found */

    वापस शून्य;
}


#अगर (NGX_HAVE_INET6)

अटल ngx_resolver_गाँठ_t *
ngx_resolver_lookup_addr6(ngx_resolver_t *r, काष्ठा in6_addr *addr,
    uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_गाँठ_t    *गाँठ, *sentinel;
    ngx_resolver_गाँठ_t  *rn;

    गाँठ = r->addr6_rbtree.मूल;
    sentinel = r->addr6_rbtree.sentinel;

    जबतक (गाँठ != sentinel) {

        अगर (hash < गाँठ->कुंजी) {
            गाँठ = गाँठ->बायाँ;
            जारी;
        }

        अगर (hash > गाँठ->कुंजी) {
            गाँठ = गाँठ->दायाँ;
            जारी;
        }

        /* hash == node->key */

        rn = ngx_resolver_गाँठ(गाँठ);

        rc = ngx_memcmp(addr, &rn->addr6, 16);

        अगर (rc == 0) {
            वापस rn;
        }

        गाँठ = (rc < 0) ? गाँठ->बायाँ : गाँठ->दायाँ;
    }

    /* not found */

    वापस शून्य;
}

#पूर्ण_अगर


अटल व्योम
ngx_resolver_rbtree_जोड़ो_मूल्य(ngx_rbtree_गाँठ_t *अस्थायी,
    ngx_rbtree_गाँठ_t *गाँठ, ngx_rbtree_गाँठ_t *sentinel)
{
    ngx_rbtree_गाँठ_t    **p;
    ngx_resolver_गाँठ_t   *rn, *rn_अस्थायी;

    क्रम ( ;; ) {

        अगर (गाँठ->कुंजी < अस्थायी->कुंजी) {

            p = &अस्थायी->बायाँ;

        } अन्यथा अगर (गाँठ->कुंजी > अस्थायी->कुंजी) {

            p = &अस्थायी->दायाँ;

        } अन्यथा { /* node->key == temp->key */

            rn = ngx_resolver_गाँठ(गाँठ);
            rn_अस्थायी = ngx_resolver_गाँठ(अस्थायी);

            p = (ngx_memn2cmp(rn->name, rn_अस्थायी->name, rn->nlen, rn_अस्थायी->nlen)
                 < 0) ? &अस्थायी->बायाँ : &अस्थायी->दायाँ;
        }

        अगर (*p == sentinel) {
            अवरोध;
        }

        अस्थायी = *p;
    }

    *p = गाँठ;
    गाँठ->parent = अस्थायी;
    गाँठ->बायाँ = sentinel;
    गाँठ->दायाँ = sentinel;
    ngx_rbt_red(गाँठ);
}


#अगर (NGX_HAVE_INET6)

अटल व्योम
ngx_resolver_rbtree_जोड़ो_addr6_मूल्य(ngx_rbtree_गाँठ_t *अस्थायी,
    ngx_rbtree_गाँठ_t *गाँठ, ngx_rbtree_गाँठ_t *sentinel)
{
    ngx_rbtree_गाँठ_t    **p;
    ngx_resolver_गाँठ_t   *rn, *rn_अस्थायी;

    क्रम ( ;; ) {

        अगर (गाँठ->कुंजी < अस्थायी->कुंजी) {

            p = &अस्थायी->बायाँ;

        } अन्यथा अगर (गाँठ->कुंजी > अस्थायी->कुंजी) {

            p = &अस्थायी->दायाँ;

        } अन्यथा { /* node->key == temp->key */

            rn = ngx_resolver_गाँठ(गाँठ);
            rn_अस्थायी = ngx_resolver_गाँठ(अस्थायी);

            p = (ngx_memcmp(&rn->addr6, &rn_अस्थायी->addr6, 16)
                 < 0) ? &अस्थायी->बायाँ : &अस्थायी->दायाँ;
        }

        अगर (*p == sentinel) {
            अवरोध;
        }

        अस्थायी = *p;
    }

    *p = गाँठ;
    गाँठ->parent = अस्थायी;
    गाँठ->बायाँ = sentinel;
    गाँठ->दायाँ = sentinel;
    ngx_rbt_red(गाँठ);
}

#पूर्ण_अगर


अटल ngx_int_t
ngx_resolver_create_name_query(ngx_resolver_t *r, ngx_resolver_गाँठ_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    माप_प्रकार               len, nlen;
    ngx_uint_t           ident;
    ngx_resolver_qs_t   *qs;
    ngx_resolver_hdr_t  *query;

    nlen = name->len ? (1 + name->len + 1) : 1;

    len = माप(ngx_resolver_hdr_t) + nlen + माप(ngx_resolver_qs_t);

#अगर (NGX_HAVE_INET6)
    p = ngx_resolver_alloc(r, len * (r->ipv4 + r->ipv6));
#अन्यथा
    p = ngx_resolver_alloc(r, len);
#पूर्ण_अगर
    अगर (p == शून्य) {
        वापस NGX_ERROR;
    }

    rn->qlen = (u_short) len;
    rn->query = p;

#अगर (NGX_HAVE_INET6)
    अगर (r->ipv6) {
        rn->query6 = r->ipv4 ? (p + len) : p;
    }
#पूर्ण_अगर

    query = (ngx_resolver_hdr_t *) p;

    अगर (r->ipv4) {
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

    p += माप(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    /* query type */
    qs->type_hi = 0; qs->type_lo = NGX_RESOLVE_A;

    /* IN query class */
    qs->class_hi = 0; qs->class_lo = 1;

    /* convert "www.example.com" to "\3www\7example\3com\0" */

    len = 0;
    p--;
    *p-- = '\0';

    अगर (name->len == 0)  {
        वापस NGX_DECLINED;
    }

    क्रम (s = name->तथ्य + name->len - 1; s >= name->तथ्य; s--) {
        अगर (*s != '.') {
            *p = *s;
            len++;

        } अन्यथा {
            अगर (len == 0 || len > 255) {
                वापस NGX_DECLINED;
            }

            *p = (u_char) len;
            len = 0;
        }

        p--;
    }

    अगर (len == 0 || len > 255) {
        वापस NGX_DECLINED;
    }

    *p = (u_char) len;

#अगर (NGX_HAVE_INET6)
    अगर (!r->ipv6) {
        वापस NGX_OK;
    }

    p = rn->query6;

    अगर (r->ipv4) {
        ngx_memcpy(p, rn->query, rn->qlen);
    }

    query = (ngx_resolver_hdr_t *) p;

    ident = ngx_random();

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve: \"%V\" AAAA %i", name, ident & 0xffff);

    query->ident_hi = (u_char) ((ident >> 8) & 0xff);
    query->ident_lo = (u_char) (ident & 0xff);

    p += माप(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    qs->type_lo = NGX_RESOLVE_AAAA;
#पूर्ण_अगर

    वापस NGX_OK;
}


अटल ngx_int_t
ngx_resolver_create_srv_query(ngx_resolver_t *r, ngx_resolver_गाँठ_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    माप_प्रकार               len, nlen;
    ngx_uint_t           ident;
    ngx_resolver_qs_t   *qs;
    ngx_resolver_hdr_t  *query;

    nlen = name->len ? (1 + name->len + 1) : 1;

    len = माप(ngx_resolver_hdr_t) + nlen + माप(ngx_resolver_qs_t);

    p = ngx_resolver_alloc(r, len);
    अगर (p == शून्य) {
        वापस NGX_ERROR;
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

    p += माप(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    /* query type */
    qs->type_hi = 0; qs->type_lo = NGX_RESOLVE_SRV;

    /* IN query class */
    qs->class_hi = 0; qs->class_lo = 1;

    /* converts "www.example.com" to "\3www\7example\3com\0" */

    len = 0;
    p--;
    *p-- = '\0';

    अगर (name->len == 0)  {
        वापस NGX_DECLINED;
    }

    क्रम (s = name->तथ्य + name->len - 1; s >= name->तथ्य; s--) {
        अगर (*s != '.') {
            *p = *s;
            len++;

        } अन्यथा {
            अगर (len == 0 || len > 255) {
                वापस NGX_DECLINED;
            }

            *p = (u_char) len;
            len = 0;
        }

        p--;
    }

    अगर (len == 0 || len > 255) {
        वापस NGX_DECLINED;
    }

    *p = (u_char) len;

    वापस NGX_OK;
}


अटल ngx_int_t
ngx_resolver_create_addr_query(ngx_resolver_t *r, ngx_resolver_गाँठ_t *rn,
    ngx_resolver_addr_t *addr)
{
    u_char               *p, *d;
    माप_प्रकार                len;
    in_addr_t             inaddr;
    ngx_int_t             n;
    ngx_uint_t            ident;
    ngx_resolver_hdr_t   *query;
    काष्ठा sockaddr_in   *sin;
#अगर (NGX_HAVE_INET6)
    काष्ठा sockaddr_in6  *sin6;
#पूर्ण_अगर

    चयन (addr->sockaddr->sa_family) {

#अगर (NGX_HAVE_INET6)
    हाल AF_INET6:
        len = माप(ngx_resolver_hdr_t)
              + 64 + माप(".ip6.arpa.") - 1
              + माप(ngx_resolver_qs_t);

        अवरोध;
#पूर्ण_अगर

    शेष: /* AF_INET */
        len = माप(ngx_resolver_hdr_t)
              + माप(".255.255.255.255.in-addr.arpa.") - 1
              + माप(ngx_resolver_qs_t);
    }

    p = ngx_resolver_alloc(r, len);
    अगर (p == शून्य) {
        वापस NGX_ERROR;
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

    p += माप(ngx_resolver_hdr_t);

    चयन (addr->sockaddr->sa_family) {

#अगर (NGX_HAVE_INET6)
    हाल AF_INET6:
        sin6 = (काष्ठा sockaddr_in6 *) addr->sockaddr;

        क्रम (n = 15; n >= 0; n--) {
            p = ngx_sprintf(p, "\1%xd\1%xd",
                            sin6->sin6_addr.s6_addr[n] & 0xf,
                            (sin6->sin6_addr.s6_addr[n] >> 4) & 0xf);
        }

        p = ngx_cpymem(p, "\3ip6\4arpa\0", 10);

        अवरोध;
#पूर्ण_अगर

    शेष: /* AF_INET */

        sin = (काष्ठा sockaddr_in *) addr->sockaddr;
        inaddr = ntohl(sin->sin_addr.s_addr);

        क्रम (n = 0; n < 32; n += 8) {
            d = ngx_sprintf(&p[1], "%ud", (inaddr >> n) & 0xff);
            *p = (u_char) (d - &p[1]);
            p = d;
        }

        p = ngx_cpymem(p, "\7in-addr\4arpa\0", 14);
    }

    /* query type "PTR", IN query class */
    p = ngx_cpymem(p, "\0\14\0\1", 4);

    rn->qlen = (u_short) (p - rn->query);

    वापस NGX_OK;
}


अटल ngx_int_t
ngx_resolver_copy(ngx_resolver_t *r, ngx_str_t *name, u_char *buf, u_char *src,
    u_char *last)
{
    अक्षर        *err;
    u_char      *p, *dst;
    माप_प्रकार       len;
    ngx_uint_t   i, n;

    p = src;
    len = 0;

    /*
     * compression pointers allow to create endless loop, so we set limit;
     * 128 pointers should be enough to store 255-byte name
     */

    क्रम (i = 0; i < 128; i++) {
        n = *p++;

        अगर (n == 0) {
            जाओ done;
        }

        अगर (n & 0xc0) {
            अगर ((n & 0xc0) != 0xc0) {
                err = "invalid label type in DNS response";
                जाओ invalid;
            }

            अगर (p >= last) {
                err = "name is out of DNS response";
                जाओ invalid;
            }

            n = ((n & 0x3f) << 8) + *p;
            p = &buf[n];

        } अन्यथा {
            len += 1 + n;
            p = &p[n];
        }

        अगर (p >= last) {
            err = "name is out of DNS response";
            जाओ invalid;
        }
    }

    err = "compression pointers loop in DNS response";

invalid:

    ngx_log_error(r->log_level, r->log, 0, err);

    वापस NGX_ERROR;

done:

    अगर (name == शून्य) {
        वापस NGX_OK;
    }

    अगर (len == 0) {
        ngx_str_null(name);
        वापस NGX_OK;
    }

    dst = ngx_resolver_alloc(r, len);
    अगर (dst == शून्य) {
        वापस NGX_ERROR;
    }

    name->तथ्य = dst;

    क्रम ( ;; ) {
        n = *src++;

        अगर (n == 0) {
            name->len = dst - name->तथ्य - 1;
            वापस NGX_OK;
        }

        अगर (n & 0xc0) {
            n = ((n & 0x3f) << 8) + *src;
            src = &buf[n];

        } अन्यथा {
            ngx_strlow(dst, src, n);
            dst += n;
            src += n;
            *dst++ = '.';
        }
    }
}


अटल ngx_int_t
ngx_resolver_set_timeout(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx)
{
    अगर (ctx->event || ctx->timeout == 0) {
        वापस NGX_OK;
    }

    ctx->event = ngx_resolver_calloc(r, माप(ngx_event_t));
    अगर (ctx->event == शून्य) {
        वापस NGX_ERROR;
    }

    ctx->event->handler = ngx_resolver_timeout_handler;
    ctx->event->तथ्य = ctx;
    ctx->event->log = r->log;
    ctx->event->cancelable = ctx->cancelable;
    ctx->ident = -1;

    ngx_add_timer(ctx->event, ctx->timeout);

    वापस NGX_OK;
}


अटल व्योम
ngx_resolver_timeout_handler(ngx_event_t *ev)
{
    ngx_resolver_ctx_t  *ctx;

    ctx = ev->तथ्य;

    ctx->state = NGX_RESOLVE_TIMEDOUT;

    ctx->handler(ctx);
}


अटल व्योम
ngx_resolver_free_गाँठ(ngx_resolver_t *r, ngx_resolver_गाँठ_t *rn)
{
    ngx_uint_t  i;

    /* lock alloc mutex */

    अगर (rn->query) {
        ngx_resolver_free_locked(r, rn->query);
    }

    अगर (rn->name) {
        ngx_resolver_free_locked(r, rn->name);
    }

    अगर (rn->cnlen) {
        ngx_resolver_free_locked(r, rn->u.cname);
    }

    अगर (rn->naddrs > 1 && rn->naddrs != (u_short) -1) {
        ngx_resolver_free_locked(r, rn->u.addrs);
    }

#अगर (NGX_HAVE_INET6)
    अगर (rn->naddrs6 > 1 && rn->naddrs6 != (u_short) -1) {
        ngx_resolver_free_locked(r, rn->u6.addrs6);
    }
#पूर्ण_अगर

    अगर (rn->nsrvs) {
        क्रम (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
            अगर (rn->u.srvs[i].name.तथ्य) {
                ngx_resolver_free_locked(r, rn->u.srvs[i].name.तथ्य);
            }
        }

        ngx_resolver_free_locked(r, rn->u.srvs);
    }

    ngx_resolver_free_locked(r, rn);

    /* unlock alloc mutex */
}


अटल व्योम *
ngx_resolver_alloc(ngx_resolver_t *r, माप_प्रकार आकार)
{
    u_char  *p;

    /* lock alloc mutex */

    p = ngx_alloc(आकार, r->log);

    /* unlock alloc mutex */

    वापस p;
}


अटल व्योम *
ngx_resolver_calloc(ngx_resolver_t *r, माप_प्रकार आकार)
{
    u_char  *p;

    p = ngx_resolver_alloc(r, आकार);

    अगर (p) {
        ngx_memzero(p, आकार);
    }

    वापस p;
}


अटल व्योम
ngx_resolver_free(ngx_resolver_t *r, व्योम *p)
{
    /* lock alloc mutex */

    ngx_free(p);

    /* unlock alloc mutex */
}


अटल व्योम
ngx_resolver_free_locked(ngx_resolver_t *r, व्योम *p)
{
    ngx_free(p);
}


अटल व्योम *
ngx_resolver_dup(ngx_resolver_t *r, व्योम *src, माप_प्रकार आकार)
{
    व्योम  *dst;

    dst = ngx_resolver_alloc(r, आकार);

    अगर (dst == शून्य) {
        वापस dst;
    }

    ngx_memcpy(dst, src, आकार);

    वापस dst;
}


अटल ngx_resolver_addr_t *
ngx_resolver_export(ngx_resolver_t *r, ngx_resolver_गाँठ_t *rn,
    ngx_uint_t rotate)
{
    ngx_uint_t            d, i, j, n;
    in_addr_t            *addr;
    ngx_sockaddr_t       *sockaddr;
    काष्ठा sockaddr_in   *sin;
    ngx_resolver_addr_t  *dst;
#अगर (NGX_HAVE_INET6)
    काष्ठा in6_addr      *addr6;
    काष्ठा sockaddr_in6  *sin6;
#पूर्ण_अगर

    n = rn->naddrs;
#अगर (NGX_HAVE_INET6)
    n += rn->naddrs6;
#पूर्ण_अगर

    dst = ngx_resolver_calloc(r, n * माप(ngx_resolver_addr_t));
    अगर (dst == शून्य) {
        वापस शून्य;
    }

    sockaddr = ngx_resolver_calloc(r, n * माप(ngx_sockaddr_t));
    अगर (sockaddr == शून्य) {
        ngx_resolver_free(r, dst);
        वापस शून्य;
    }

    i = 0;
    d = rotate ? ngx_random() % n : 0;

    अगर (rn->naddrs) {
        j = rotate ? ngx_random() % rn->naddrs : 0;

        addr = (rn->naddrs == 1) ? &rn->u.addr : rn->u.addrs;

        करो {
            sin = &sockaddr[d].sockaddr_in;
            sin->sin_family = AF_INET;
            sin->sin_addr.s_addr = addr[j++];
            dst[d].sockaddr = (काष्ठा sockaddr *) sin;
            dst[d++].socklen = माप(काष्ठा sockaddr_in);

            अगर (d == n) {
                d = 0;
            }

            अगर (j == (ngx_uint_t) rn->naddrs) {
                j = 0;
            }
        } जबतक (++i < (ngx_uint_t) rn->naddrs);
    }

#अगर (NGX_HAVE_INET6)
    अगर (rn->naddrs6) {
        j = rotate ? ngx_random() % rn->naddrs6 : 0;

        addr6 = (rn->naddrs6 == 1) ? &rn->u6.addr6 : rn->u6.addrs6;

        करो {
            sin6 = &sockaddr[d].sockaddr_in6;
            sin6->sin6_family = AF_INET6;
            ngx_memcpy(sin6->sin6_addr.s6_addr, addr6[j++].s6_addr, 16);
            dst[d].sockaddr = (काष्ठा sockaddr *) sin6;
            dst[d++].socklen = माप(काष्ठा sockaddr_in6);

            अगर (d == n) {
                d = 0;
            }

            अगर (j == rn->naddrs6) {
                j = 0;
            }
        } जबतक (++i < n);
    }
#पूर्ण_अगर

    वापस dst;
}


अटल व्योम
ngx_resolver_report_srv(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx)
{
    ngx_uint_t                naddrs, nsrvs, nw, i, j, k, l, m, n, w;
    ngx_resolver_addr_t      *addrs;
    ngx_resolver_srv_name_t  *srvs;

    srvs = ctx->srvs;
    nsrvs = ctx->nsrvs;

    naddrs = 0;

    क्रम (i = 0; i < nsrvs; i++) {
        अगर (srvs[i].state == NGX_ERROR) {
            ctx->state = NGX_ERROR;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

            ctx->handler(ctx);
            वापस;
        }

        naddrs += srvs[i].naddrs;
    }

    अगर (naddrs == 0) {
        ctx->state = srvs[0].state;

        क्रम (i = 0; i < nsrvs; i++) {
            अगर (srvs[i].state == NGX_RESOLVE_NXDOMAIN) {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                अवरोध;
            }
        }

        ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        वापस;
    }

    addrs = ngx_resolver_calloc(r, naddrs * माप(ngx_resolver_addr_t));
    अगर (addrs == शून्य) {
        ctx->state = NGX_ERROR;
        ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        वापस;
    }

    i = 0;
    n = 0;

    करो {
        nw = 0;

        क्रम (j = i; j < nsrvs; j++) {
            अगर (srvs[j].priority != srvs[i].priority) {
                अवरोध;
            }

            nw += srvs[j].naddrs * srvs[j].weight;
        }

        अगर (nw == 0) {
            जाओ अग्र_srv;
        }

        w = ngx_random() % nw;

        क्रम (k = i; k < j; k++) {
            अगर (w < srvs[k].naddrs * srvs[k].weight) {
                अवरोध;
            }

            w -= srvs[k].naddrs * srvs[k].weight;
        }

        क्रम (l = i; l < j; l++) {

            क्रम (m = 0; m < srvs[k].naddrs; m++) {
                addrs[n].socklen = srvs[k].addrs[m].socklen;
                addrs[n].sockaddr = srvs[k].addrs[m].sockaddr;
                addrs[n].name = srvs[k].name;
                addrs[n].priority = srvs[k].priority;
                addrs[n].weight = srvs[k].weight;
                n++;
            }

            अगर (++k == j) {
                k = i;
            }
        }

अग्र_srv:

        i = j;

    } जबतक (i < ctx->nsrvs);

    ctx->state = NGX_OK;
    ctx->addrs = addrs;
    ctx->naddrs = naddrs;

    ctx->handler(ctx);

    ngx_resolver_free(r, addrs);
}


अक्षर *
ngx_resolver_strerror(ngx_int_t err)
{
    अटल अक्षर *errors[] = {
        "Format error",     /* FORMERR */
        "Server failure",   /* SERVFAIL */
        "Host not found",   /* NXDOMAIN */
        "Unimplemented",    /* NOTIMP */
        "Operation refused" /* REFUSED */
    };

    अगर (err > 0 && err < 6) {
        वापस errors[err - 1];
    }

    अगर (err == NGX_RESOLVE_TIMEDOUT) {
        वापस "Operation timed out";
    }

    वापस "Unknown error";
}


अटल u_char *
ngx_resolver_log_error(ngx_log_t *log, u_char *buf, माप_प्रकार len)
{
    u_char                     *p;
    ngx_resolver_connection_t  *rec;

    p = buf;

    अगर (log->action) {
        p = ngx_snprintf(buf, len, " while %s", log->action);
        len -= p - buf;
    }

    rec = log->तथ्य;

    अगर (rec) {
        p = ngx_snprintf(p, len, ", resolver: %V", &rec->server);
    }

    वापस p;
}


अटल ngx_int_t
ngx_udp_connect(ngx_resolver_connection_t *rec)
{
    पूर्णांक                rc;
    ngx_int_t          event;
    ngx_event_t       *rev, *wev;
    ngx_socket_t       s;
    ngx_connection_t  *c;

    s = ngx_socket(rec->sockaddr->sa_family, SOCK_DGRAM, 0);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "UDP socket %d", s);

    अगर (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_socket_n " failed");
        वापस NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->log);

    अगर (c == शून्य) {
        अगर (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        वापस NGX_ERROR;
    }

    अगर (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        जाओ failed;
    }

    rev = c->पढ़ो;
    wev = c->ग_लिखो;

    rev->log = &rec->log;
    wev->log = &rec->log;

    rec->udp = c;

    c->संख्या = ngx_atomic_fetch_add(ngx_connection_गणक, 1);

    c->start_time = ngx_current_msec;

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->log, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->संख्या);

    rc = connect(s, rec->sockaddr, rec->socklen);

    अगर (rc == -1) {
        ngx_log_error(NGX_LOG_CRIT, &rec->log, ngx_socket_errno,
                      "connect() failed");

        जाओ failed;
    }

    /* UDP sockets are always ready to write */
    wev->ready = 1;

    event = (ngx_event_flags & NGX_USE_CLEAR_EVENT) ?
                /* kqueue, epoll */                 NGX_CLEAR_EVENT:
                /* select, poll, /dev/poll */       NGX_LEVEL_EVENT;
                /* eventport event type has no meaning: oneshot only */

    अगर (ngx_add_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        जाओ failed;
    }

    वापस NGX_OK;

failed:

    ngx_close_connection(c);
    rec->udp = शून्य;

    वापस NGX_ERROR;
}


अटल ngx_int_t
ngx_tcp_connect(ngx_resolver_connection_t *rec)
{
    पूर्णांक                rc;
    ngx_int_t          event;
    ngx_err_t          err;
    ngx_uint_t         level;
    ngx_socket_t       s;
    ngx_event_t       *rev, *wev;
    ngx_connection_t  *c;

    s = ngx_socket(rec->sockaddr->sa_family, SOCK_STREAM, 0);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "TCP socket %d", s);

    अगर (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_socket_n " failed");
        वापस NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->log);

    अगर (c == शून्य) {
        अगर (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        वापस NGX_ERROR;
    }

    अगर (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        जाओ failed;
    }

    rev = c->पढ़ो;
    wev = c->ग_लिखो;

    rev->log = &rec->log;
    wev->log = &rec->log;

    rec->tcp = c;

    c->संख्या = ngx_atomic_fetch_add(ngx_connection_गणक, 1);

    c->start_time = ngx_current_msec;

    अगर (ngx_add_conn) {
        अगर (ngx_add_conn(c) == NGX_ERROR) {
            जाओ failed;
        }
    }

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->log, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->संख्या);

    rc = connect(s, rec->sockaddr, rec->socklen);

    अगर (rc == -1) {
        err = ngx_socket_errno;


        अगर (err != NGX_EINPROGRESS
#अगर (NGX_WIN32)
            /* Winsock returns WSAEWOULDBLOCK (NGX_EAGAIN) */
            && err != NGX_EAGAIN
#पूर्ण_अगर
            )
        {
            अगर (err == NGX_ECONNREFUSED
#अगर (NGX_LINUX)
                /*
                 * Linux returns EAGAIN instead of ECONNREFUSED
                 * for unix sockets if listen queue is full
                 */
                || err == NGX_EAGAIN
#पूर्ण_अगर
                || err == NGX_ECONNRESET
                || err == NGX_ENETDOWN
                || err == NGX_ENETUNREACH
                || err == NGX_EHOSTDOWN
                || err == NGX_EHOSTUNREACH)
            {
                level = NGX_LOG_ERR;

            } अन्यथा {
                level = NGX_LOG_CRIT;
            }

            ngx_log_error(level, &rec->log, err, "connect() to %V failed",
                          &rec->server);

            ngx_close_connection(c);
            rec->tcp = शून्य;

            वापस NGX_ERROR;
        }
    }

    अगर (ngx_add_conn) {
        अगर (rc == -1) {

            /* NGX_EINPROGRESS */

            वापस NGX_AGAIN;
        }

        ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "connected");

        wev->ready = 1;

        वापस NGX_OK;
    }

    अगर (ngx_event_flags & NGX_USE_CLEAR_EVENT) {

        /* kqueue */

        event = NGX_CLEAR_EVENT;

    } अन्यथा {

        /* select, poll, /dev/poll */

        event = NGX_LEVEL_EVENT;
    }

    अगर (ngx_add_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        जाओ failed;
    }

    अगर (rc == -1) {

        /* NGX_EINPROGRESS */

        अगर (ngx_add_event(wev, NGX_WRITE_EVENT, event) != NGX_OK) {
            जाओ failed;
        }

        वापस NGX_AGAIN;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "connected");

    wev->ready = 1;

    वापस NGX_OK;

failed:

    ngx_close_connection(c);
    rec->tcp = शून्य;

    वापस NGX_ERROR;
}


अटल ngx_int_t
ngx_resolver_cmp_srvs(स्थिर व्योम *one, स्थिर व्योम *two)
{
    ngx_int_t            p1, p2;
    ngx_resolver_srv_t  *first, *second;

    first = (ngx_resolver_srv_t *) one;
    second = (ngx_resolver_srv_t *) two;

    p1 = first->priority;
    p2 = second->priority;

    वापस p1 - p2;
}
