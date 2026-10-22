
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>
#अन्तर्भाव <ngx_event.h>


#define NGX_RESOLVER_UDP_SIZE   4096

#define NGX_RESOLVER_TCP_RSIZE  (2 + 65535)
#define NGX_RESOLVER_TCP_WSIZE  8192


प्रकारनिर्देश संरचना {
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


प्रकारनिर्देश संरचना {
    u_char  type_hi;
    u_char  type_lo;
    u_char  class_hi;
    u_char  class_lo;
} ngx_resolver_qs_t;


प्रकारनिर्देश संरचना {
    u_char  type_hi;
    u_char  type_lo;
    u_char  class_hi;
    u_char  class_lo;
    u_char  ttl[4];
    u_char  len_hi;
    u_char  len_lo;
} ngx_resolver_an_t;


#define ngx_resolver_ग्रन्थिः(n)  ngx_rbtree_data(n, ngx_resolver_ग्रन्थिः_t, ग्रन्थिः)


स्थिर ngx_int_t ngx_udp_connect(ngx_resolver_connection_t *rec);
स्थिर ngx_int_t ngx_tcp_connect(ngx_resolver_connection_t *rec);


स्थिर शून्य ngx_resolver_cleanup(शून्य *data);
स्थिर शून्य ngx_resolver_cleanup_वृक्षः(ngx_resolver_t *r, ngx_rbtree_t *वृक्षः);
स्थिर ngx_int_t ngx_resolve_name_locked(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx, ngx_str_t *name);
स्थिर शून्य ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *वृक्षः,
    ngx_queue_t *queue);
स्थिर ngx_int_t ngx_resolver_send_query(ngx_resolver_t *r,
    ngx_resolver_ग्रन्थिः_t *rn);
स्थिर ngx_int_t ngx_resolver_send_udp_query(ngx_resolver_t *r,
    ngx_resolver_connection_t *rec, u_char *query, u_short qlen);
स्थिर ngx_int_t ngx_resolver_send_tcp_query(ngx_resolver_t *r,
    ngx_resolver_connection_t *rec, u_char *query, u_short qlen);
स्थिर ngx_int_t ngx_resolver_create_name_query(ngx_resolver_t *r,
    ngx_resolver_ग्रन्थिः_t *rn, ngx_str_t *name);
स्थिर ngx_int_t ngx_resolver_create_srv_query(ngx_resolver_t *r,
    ngx_resolver_ग्रन्थिः_t *rn, ngx_str_t *name);
स्थिर ngx_int_t ngx_resolver_create_addr_query(ngx_resolver_t *r,
    ngx_resolver_ग्रन्थिः_t *rn, ngx_resolver_addr_t *addr);
स्थिर शून्य ngx_resolver_resend_handler(ngx_event_t *ev);
स्थिर time_t ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *वृक्षः,
    ngx_queue_t *queue);
स्थिर ngx_uint_t ngx_resolver_resend_empty(ngx_resolver_t *r);
स्थिर शून्य ngx_resolver_udp_read(ngx_event_t *rev);
स्थिर शून्य ngx_resolver_tcp_write(ngx_event_t *wev);
स्थिर शून्य ngx_resolver_tcp_read(ngx_event_t *rev);
स्थिर शून्य ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf,
    size_t n, ngx_uint_t tcp);
स्थिर शून्य ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans);
स्थिर शून्य ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans);
स्थिर शून्य ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan);
स्थिर ngx_resolver_ग्रन्थिः_t *ngx_resolver_lookup_name(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
स्थिर ngx_resolver_ग्रन्थिः_t *ngx_resolver_lookup_srv(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
स्थिर ngx_resolver_ग्रन्थिः_t *ngx_resolver_lookup_addr(ngx_resolver_t *r,
    in_addr_t addr);
स्थिर शून्य ngx_resolver_rbtree_निवेशय_मानम्(ngx_rbtree_ग्रन्थिः_t *temp,
    ngx_rbtree_ग्रन्थिः_t *ग्रन्थिः, ngx_rbtree_ग्रन्थिः_t *sentinel);
स्थिर ngx_int_t ngx_resolver_copy(ngx_resolver_t *r, ngx_str_t *name,
    u_char *buf, u_char *src, u_char *last);
स्थिर ngx_int_t ngx_resolver_set_timeout(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx);
स्थिर शून्य ngx_resolver_timeout_handler(ngx_event_t *ev);
स्थिर शून्य ngx_resolver_free_ग्रन्थिः(ngx_resolver_t *r, ngx_resolver_ग्रन्थिः_t *rn);
स्थिर शून्य *ngx_resolver_alloc(ngx_resolver_t *r, size_t size);
स्थिर शून्य *ngx_resolver_calloc(ngx_resolver_t *r, size_t size);
स्थिर शून्य ngx_resolver_free(ngx_resolver_t *r, शून्य *p);
स्थिर शून्य ngx_resolver_free_locked(ngx_resolver_t *r, शून्य *p);
स्थिर शून्य *ngx_resolver_dup(ngx_resolver_t *r, शून्य *src, size_t size);
स्थिर ngx_resolver_addr_t *ngx_resolver_export(ngx_resolver_t *r,
    ngx_resolver_ग्रन्थिः_t *rn, ngx_uint_t rotate);
स्थिर शून्य ngx_resolver_report_srv(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx);
स्थिर u_char *ngx_resolver_log_error(ngx_log_t *log, u_char *buf, size_t len);
स्थिर शून्य ngx_resolver_resolve_srv_names(ngx_resolver_ctx_t *ctx,
    ngx_resolver_ग्रन्थिः_t *rn);
स्थिर शून्य ngx_resolver_srv_names_handler(ngx_resolver_ctx_t *ctx);
स्थिर ngx_int_t ngx_resolver_cmp_srvs(ध्रुव शून्य *one, ध्रुव शून्य *two);

#सचे (NGX_HAVE_INET6)
स्थिर शून्य ngx_resolver_rbtree_निवेशय_addr6_मानम्(ngx_rbtree_ग्रन्थिः_t *temp,
    ngx_rbtree_ग्रन्थिः_t *ग्रन्थिः, ngx_rbtree_ग्रन्थिः_t *sentinel);
स्थिर ngx_resolver_ग्रन्थिः_t *ngx_resolver_lookup_addr6(ngx_resolver_t *r,
    संरचना in6_addr *addr, uint32_t hash);
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

    r = ngx_pcalloc(cf->pool, परिमाण(ngx_resolver_t));
    सचे (r == रिक्त) {
        पटिनिवत्त रिक्त;
    }

    r->event = ngx_pcalloc(cf->pool, परिमाण(ngx_event_t));
    सचे (r->event == रिक्त) {
        पटिनिवत्त रिक्त;
    }

    cln = ngx_pool_cleanup_add(cf->pool, 0);
    सचे (cln == रिक्त) {
        पटिनिवत्त रिक्त;
    }

    cln->handler = ngx_resolver_cleanup;
    cln->data = r;

    r->ipv4 = 1;

    ngx_rbtree_init(&r->name_rbtree, &r->name_sentinel,
                    ngx_resolver_rbtree_निवेशय_मानम्);

    ngx_rbtree_init(&r->srv_rbtree, &r->srv_sentinel,
                    ngx_resolver_rbtree_निवेशय_मानम्);

    ngx_rbtree_init(&r->addr_rbtree, &r->addr_sentinel,
                    ngx_rbtree_निवेशय_मानम्);

    ngx_queue_init(&r->name_resend_queue);
    ngx_queue_init(&r->srv_resend_queue);
    ngx_queue_init(&r->addr_resend_queue);

    ngx_queue_init(&r->name_expire_queue);
    ngx_queue_init(&r->srv_expire_queue);
    ngx_queue_init(&r->addr_expire_queue);

#सचे (NGX_HAVE_INET6)
    r->ipv6 = 1;

    ngx_rbtree_init(&r->addr6_rbtree, &r->addr6_sentinel,
                    ngx_resolver_rbtree_निवेशय_addr6_मानम्);

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

    सचे (n) {
        सचे (ngx_array_init(&r->connections, cf->pool, n,
                           परिमाण(ngx_resolver_connection_t))
            != NGX_OK)
        {
            पटिनिवत्त रिक्त;
        }
    }

    अत्थाय (i = 0; i < n; i++) {
        सचे (ngx_strncmp(names[i].data, "valid=", 6) == 0) {
            s.len = names[i].len - 6;
            s.data = names[i].data + 6;

            r->valid = ngx_parse_time(&s, 1);

            सचे (r->valid == (time_t) NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &names[i]);
                पटिनिवत्त रिक्त;
            }

            अनुवर्त;
        }

#सचे (NGX_HAVE_INET6)
        सचे (ngx_strncmp(names[i].data, "ipv4=", 5) == 0) {

            सचे (ngx_strcmp(&names[i].data[5], "on") == 0) {
                r->ipv4 = 1;

            } अञ्ञथा सचे (ngx_strcmp(&names[i].data[5], "off") == 0) {
                r->ipv4 = 0;

            } अञ्ञथा {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &names[i]);
                पटिनिवत्त रिक्त;
            }

            अनुवर्त;
        }

        सचे (ngx_strncmp(names[i].data, "ipv6=", 5) == 0) {

            सचे (ngx_strcmp(&names[i].data[5], "on") == 0) {
                r->ipv6 = 1;

            } अञ्ञथा सचे (ngx_strcmp(&names[i].data[5], "off") == 0) {
                r->ipv6 = 0;

            } अञ्ञथा {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &names[i]);
                पटिनिवत्त रिक्त;
            }

            अनुवर्त;
        }
#endif

        ngx_memzero(&u, परिमाण(ngx_url_t));

        u.url = names[i];
        u.default_port = 53;

        सचे (ngx_parse_url(cf->pool, &u) != NGX_OK) {
            सचे (u.err) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "%s in resolver \"%V\"",
                                   u.err, &u.url);
            }

            पटिनिवत्त रिक्त;
        }

        rec = ngx_array_प्रक्षिप_n(&r->connections, u.naddrs);
        सचे (rec == रिक्त) {
            पटिनिवत्त रिक्त;
        }

        ngx_memzero(rec, u.naddrs * परिमाण(ngx_resolver_connection_t));

        अत्थाय (j = 0; j < u.naddrs; j++) {
            rec[j].sockaddr = u.addrs[j].sockaddr;
            rec[j].socklen = u.addrs[j].socklen;
            rec[j].server = u.addrs[j].name;
            rec[j].resolver = r;
        }
    }

#सचे (NGX_HAVE_INET6)
    सचे (r->ipv4 + r->ipv6 == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"ipv4\" and \"ipv6\" cannot both be \"off\"");
        पटिनिवत्त रिक्त;
    }
#endif

    सचे (n && r->connections.nelts == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "no name servers defined");
        पटिनिवत्त रिक्त;
    }

    पटिनिवत्त r;
}


स्थिर शून्य
ngx_resolver_cleanup(शून्य *data)
{
    ngx_resolver_t  *r = data;

    ngx_uint_t                  i;
    ngx_resolver_connection_t  *rec;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "cleanup resolver");

    ngx_resolver_cleanup_वृक्षः(r, &r->name_rbtree);

    ngx_resolver_cleanup_वृक्षः(r, &r->srv_rbtree);

    ngx_resolver_cleanup_वृक्षः(r, &r->addr_rbtree);

#सचे (NGX_HAVE_INET6)
    ngx_resolver_cleanup_वृक्षः(r, &r->addr6_rbtree);
#endif

    सचे (r->event->timer_set) {
        ngx_del_timer(r->event);
    }

    rec = r->connections.elts;

    अत्थाय (i = 0; i < r->connections.nelts; i++) {
        सचे (rec[i].udp) {
            ngx_close_connection(rec[i].udp);
        }

        सचे (rec[i].tcp) {
            ngx_close_connection(rec[i].tcp);
        }

        सचे (rec[i].read_buf) {
            ngx_resolver_free(r, rec[i].read_buf->start);
            ngx_resolver_free(r, rec[i].read_buf);
        }

        सचे (rec[i].write_buf) {
            ngx_resolver_free(r, rec[i].write_buf->start);
            ngx_resolver_free(r, rec[i].write_buf);
        }
    }
}


स्थिर शून्य
ngx_resolver_cleanup_वृक्षः(ngx_resolver_t *r, ngx_rbtree_t *वृक्षः)
{
    ngx_resolver_ctx_t   *ctx, *next;
    ngx_resolver_ग्रन्थिः_t  *rn;

    याव (वृक्षः->मूलम् != वृक्षः->sentinel) {

        rn = ngx_resolver_ग्रन्थिः(ngx_rbtree_min(वृक्षः->मूलम्, वृक्षः->sentinel));

        ngx_queue_remove(&rn->queue);

        अत्थाय (ctx = rn->waiting; ctx; ctx = next) {
            next = ctx->next;

            सचे (ctx->event) {
                सचे (ctx->event->timer_set) {
                    ngx_del_timer(ctx->event);
                }

                ngx_resolver_free(r, ctx->event);
            }

            ngx_resolver_free(r, ctx);
        }

        ngx_rbtree_delete(वृक्षः, &rn->ग्रन्थिः);

        ngx_resolver_free_ग्रन्थिः(r, rn);
    }
}


ngx_resolver_ctx_t *
ngx_resolve_start(ngx_resolver_t *r, ngx_resolver_ctx_t *temp)
{
    in_addr_t            addr;
    ngx_resolver_ctx_t  *ctx;

    सचे (temp) {
        addr = ngx_inet_addr(temp->name.data, temp->name.len);

        सचे (addr != INADDR_NONE) {
            temp->resolver = r;
            temp->state = NGX_OK;
            temp->naddrs = 1;
            temp->addrs = &temp->addr;
            temp->addr.sockaddr = (संरचना sockaddr *) &temp->sin;
            temp->addr.socklen = परिमाण(संरचना sockaddr_in);
            ngx_memzero(&temp->sin, परिमाण(संरचना sockaddr_in));
            temp->sin.sin_family = AF_INET;
            temp->sin.sin_addr.s_addr = addr;
            temp->quick = 1;

            पटिनिवत्त temp;
        }
    }

    सचे (r->connections.nelts == 0) {
        पटिनिवत्त NGX_NO_RESOLVER;
    }

    ctx = ngx_resolver_calloc(r, परिमाण(ngx_resolver_ctx_t));

    सचे (ctx) {
        ctx->resolver = r;
    }

    पटिनिवत्त ctx;
}


ngx_int_t
ngx_resolve_name(ngx_resolver_ctx_t *ctx)
{
    size_t           slen;
    ngx_int_t        rc;
    ngx_str_t        name;
    ngx_resolver_t  *r;

    r = ctx->resolver;

    सचे (ctx->name.len > 0 && ctx->name.data[ctx->name.len - 1] == '.') {
        ctx->name.len--;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve: \"%V\"", &ctx->name);

    सचे (ctx->quick) {
        ctx->handler(ctx);
        पटिनिवत्त NGX_OK;
    }

    सचे (ctx->service.len) {
        slen = ctx->service.len;

        सचे (ngx_strlchr(ctx->service.data,
                        ctx->service.data + ctx->service.len, '.')
            == रिक्त)
        {
            slen += परिमाण("_._tcp") - 1;
        }

        name.len = slen + 1 + ctx->name.len;

        name.data = ngx_resolver_alloc(r, name.len);
        सचे (name.data == रिक्त) {
            गच्छ failed;
        }

        सचे (slen == ctx->service.len) {
            ngx_sprintf(name.data, "%V.%V", &ctx->service, &ctx->name);

        } अञ्ञथा {
            ngx_sprintf(name.data, "_%V._tcp.%V", &ctx->service, &ctx->name);
        }

        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &name);

        ngx_resolver_free(r, name.data);

    } अञ्ञथा {
        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &ctx->name);
    }

    सचे (rc == NGX_OK) {
        पटिनिवत्त NGX_OK;
    }

    /* unlock name mutex */

    सचे (rc == NGX_AGAIN) {
        पटिनिवत्त NGX_OK;
    }

    /* NGX_ERROR */

    सचे (ctx->event) {
        ngx_resolver_free(r, ctx->event);
    }

failed:

    ngx_resolver_free(r, ctx);

    पटिनिवत्त NGX_ERROR;
}


शून्य
ngx_resolve_name_done(ngx_resolver_ctx_t *ctx)
{
    ngx_uint_t            i;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_ग्रन्थिः_t  *rn;

    r = ctx->resolver;

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve name done: %i", ctx->state);

    सचे (ctx->quick) {
        पटिनिवत्त;
    }

    सचे (ctx->event && ctx->event->timer_set) {
        ngx_del_timer(ctx->event);
    }

    /* lock name mutex */

    सचे (ctx->nsrvs) {
        अत्थाय (i = 0; i < ctx->nsrvs; i++) {
            सचे (ctx->srvs[i].ctx) {
                ngx_resolve_name_done(ctx->srvs[i].ctx);
            }

            सचे (ctx->srvs[i].addrs) {
                ngx_resolver_free(r, ctx->srvs[i].addrs->sockaddr);
                ngx_resolver_free(r, ctx->srvs[i].addrs);
            }

            ngx_resolver_free(r, ctx->srvs[i].name.data);
        }

        ngx_resolver_free(r, ctx->srvs);
    }

    सचे (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->ग्रन्थिः;

        सचे (rn) {
            p = &rn->waiting;
            w = rn->waiting;

            याव (w) {
                सचे (w == ctx) {
                    *p = w->next;

                    गच्छ done;
                }

                p = &w->next;
                w = w->next;
            }

            ngx_log_error(NGX_LOG_ALERT, r->log, 0,
                          "could not cancel %V resolving", &ctx->name);
        }
    }

done:

    सचे (ctx->service.len) {
        ngx_resolver_expire(r, &r->srv_rbtree, &r->srv_expire_queue);

    } अञ्ञथा {
        ngx_resolver_expire(r, &r->name_rbtree, &r->name_expire_queue);
    }

    /* unlock name mutex */

    /* lock alloc mutex */

    सचे (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    सचे (r->event->timer_set && ngx_resolver_resend_empty(r)) {
        ngx_del_timer(r->event);
    }
}


स्थिर ngx_int_t
ngx_resolve_name_locked(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx,
    ngx_str_t *name)
{
    uint32_t              hash;
    ngx_int_t             rc;
    ngx_str_t             cname;
    ngx_uint_t            i, naddrs;
    ngx_queue_t          *resend_queue, *expire_queue;
    ngx_rbtree_t         *वृक्षः;
    ngx_resolver_ctx_t   *next, *last;
    ngx_resolver_addr_t  *addrs;
    ngx_resolver_ग्रन्थिः_t  *rn;

    ngx_strlow(name->data, name->data, name->len);

    hash = ngx_crc32_short(name->data, name->len);

    सचे (ctx->service.len) {
        rn = ngx_resolver_lookup_srv(r, name, hash);

        वृक्षः = &r->srv_rbtree;
        resend_queue = &r->srv_resend_queue;
        expire_queue = &r->srv_expire_queue;

    } अञ्ञथा {
        rn = ngx_resolver_lookup_name(r, name, hash);

        वृक्षः = &r->name_rbtree;
        resend_queue = &r->name_resend_queue;
        expire_queue = &r->name_expire_queue;
    }

    सचे (rn) {

        /* ctx can be a list after NGX_RESOLVE_CNAME */
        अत्थाय (last = ctx; last->next; last = last->next);

        सचे (rn->valid >= ngx_time()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0, "resolve cached");

            ngx_queue_remove(&rn->queue);

            rn->expire = ngx_time() + r->expire;

            ngx_queue_निवेशय_head(expire_queue, &rn->queue);

            naddrs = (rn->naddrs == (u_short) -1) ? 0 : rn->naddrs;
#सचे (NGX_HAVE_INET6)
            naddrs += (rn->naddrs6 == (u_short) -1) ? 0 : rn->naddrs6;
#endif

            सचे (naddrs) {

                सचे (naddrs == 1 && rn->naddrs == 1) {
                    addrs = रिक्त;

                } अञ्ञथा {
                    addrs = ngx_resolver_export(r, rn, 1);
                    सचे (addrs == रिक्त) {
                        पटिनिवत्त NGX_ERROR;
                    }
                }

                last->next = rn->waiting;
                rn->waiting = रिक्त;

                /* unlock name mutex */

                करोहि {
                    ctx->state = NGX_OK;
                    ctx->valid = rn->valid;
                    ctx->naddrs = naddrs;

                    सचे (addrs == रिक्त) {
                        ctx->addrs = &ctx->addr;
                        ctx->addr.sockaddr = (संरचना sockaddr *) &ctx->sin;
                        ctx->addr.socklen = परिमाण(संरचना sockaddr_in);
                        ngx_memzero(&ctx->sin, परिमाण(संरचना sockaddr_in));
                        ctx->sin.sin_family = AF_INET;
                        ctx->sin.sin_addr.s_addr = rn->u.addr;

                    } अञ्ञथा {
                        ctx->addrs = addrs;
                    }

                    next = ctx->next;

                    ctx->handler(ctx);

                    ctx = next;
                } याव (ctx);

                सचे (addrs != रिक्त) {
                    ngx_resolver_free(r, addrs->sockaddr);
                    ngx_resolver_free(r, addrs);
                }

                पटिनिवत्त NGX_OK;
            }

            सचे (rn->nsrvs) {
                last->next = rn->waiting;
                rn->waiting = रिक्त;

                /* unlock name mutex */

                करोहि {
                    next = ctx->next;

                    ngx_resolver_resolve_srv_names(ctx, rn);

                    ctx = next;
                } याव (ctx);

                पटिनिवत्त NGX_OK;
            }

            /* NGX_RESOLVE_CNAME */

            सचे (ctx->recursion++ < NGX_RESOLVER_MAX_RECURSION) {

                cname.len = rn->cnlen;
                cname.data = rn->u.cname;

                पटिनिवत्त ngx_resolve_name_locked(r, ctx, &cname);
            }

            last->next = rn->waiting;
            rn->waiting = रिक्त;

            /* unlock name mutex */

            करोहि {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
                next = ctx->next;

                ctx->handler(ctx);

                ctx = next;
            } याव (ctx);

            पटिनिवत्त NGX_OK;
        }

        सचे (rn->waiting) {
            सचे (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
                पटिनिवत्त NGX_ERROR;
            }

            last->next = rn->waiting;
            rn->waiting = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;

            करोहि {
                ctx->ग्रन्थिः = rn;
                ctx = ctx->next;
            } याव (ctx);

            पटिनिवत्त NGX_AGAIN;
        }

        ngx_queue_remove(&rn->queue);

        /* lock alloc mutex */

        सचे (rn->query) {
            ngx_resolver_free_locked(r, rn->query);
            rn->query = रिक्त;
#सचे (NGX_HAVE_INET6)
            rn->query6 = रिक्त;
#endif
        }

        सचे (rn->cnlen) {
            ngx_resolver_free_locked(r, rn->u.cname);
        }

        सचे (rn->naddrs > 1 && rn->naddrs != (u_short) -1) {
            ngx_resolver_free_locked(r, rn->u.addrs);
        }

#सचे (NGX_HAVE_INET6)
        सचे (rn->naddrs6 > 1 && rn->naddrs6 != (u_short) -1) {
            ngx_resolver_free_locked(r, rn->u6.addrs6);
        }
#endif

        सचे (rn->nsrvs) {
            अत्थाय (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
                सचे (rn->u.srvs[i].name.data) {
                    ngx_resolver_free_locked(r, rn->u.srvs[i].name.data);
                }
            }

            ngx_resolver_free_locked(r, rn->u.srvs);
        }

        /* unlock alloc mutex */

    } अञ्ञथा {

        rn = ngx_resolver_alloc(r, परिमाण(ngx_resolver_ग्रन्थिः_t));
        सचे (rn == रिक्त) {
            पटिनिवत्त NGX_ERROR;
        }

        rn->name = ngx_resolver_dup(r, name->data, name->len);
        सचे (rn->name == रिक्त) {
            ngx_resolver_free(r, rn);
            पटिनिवत्त NGX_ERROR;
        }

        rn->ग्रन्थिः.कुञ्चिका = hash;
        rn->nlen = (u_short) name->len;
        rn->query = रिक्त;
#सचे (NGX_HAVE_INET6)
        rn->query6 = रिक्त;
#endif

        ngx_rbtree_निवेशय(वृक्षः, &rn->ग्रन्थिः);
    }

    सचे (ctx->service.len) {
        rc = ngx_resolver_create_srv_query(r, rn, name);

    } अञ्ञथा {
        rc = ngx_resolver_create_name_query(r, rn, name);
    }

    सचे (rc == NGX_ERROR) {
        गच्छ failed;
    }

    सचे (rc == NGX_DECLINED) {
        ngx_rbtree_delete(वृक्षः, &rn->ग्रन्थिः);

        ngx_resolver_free(r, rn->query);
        ngx_resolver_free(r, rn->name);
        ngx_resolver_free(r, rn);

        करोहि {
            ctx->state = NGX_RESOLVE_NXDOMAIN;
            next = ctx->next;

            ctx->handler(ctx);

            ctx = next;
        } याव (ctx);

        पटिनिवत्त NGX_OK;
    }

    rn->last_connection = r->last_connection++;
    सचे (r->last_connection == r->connections.nelts) {
        r->last_connection = 0;
    }

    rn->naddrs = r->ipv4 ? (u_short) -1 : 0;
    rn->tcp = 0;
#सचे (NGX_HAVE_INET6)
    rn->naddrs6 = r->ipv6 ? (u_short) -1 : 0;
    rn->tcp6 = 0;
#endif
    rn->nsrvs = 0;

    सचे (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->last_connection++;
        सचे (rn->last_connection == r->connections.nelts) {
            rn->last_connection = 0;
        }

        (शून्य) ngx_resolver_send_query(r, rn);
    }

    सचे (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
        गच्छ failed;
    }

    सचे (ngx_resolver_resend_empty(r)) {
        ngx_add_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_time() + r->resend_timeout;

    ngx_queue_निवेशय_head(resend_queue, &rn->queue);

    rn->code = 0;
    rn->cnlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_मानम्;
    rn->waiting = ctx;

    ctx->state = NGX_AGAIN;
    ctx->async = 1;

    करोहि {
        ctx->ग्रन्थिः = rn;
        ctx = ctx->next;
    } याव (ctx);

    पटिनिवत्त NGX_AGAIN;

failed:

    ngx_rbtree_delete(वृक्षः, &rn->ग्रन्थिः);

    सचे (rn->query) {
        ngx_resolver_free(r, rn->query);
    }

    ngx_resolver_free(r, rn->name);

    ngx_resolver_free(r, rn);

    पटिनिवत्त NGX_ERROR;
}


ngx_int_t
ngx_resolve_addr(ngx_resolver_ctx_t *ctx)
{
    u_char               *name;
    in_addr_t             addr;
    ngx_queue_t          *resend_queue, *expire_queue;
    ngx_rbtree_t         *वृक्षः;
    ngx_resolver_t       *r;
    संरचना sockaddr_in   *sin;
    ngx_resolver_ग्रन्थिः_t  *rn;
#सचे (NGX_HAVE_INET6)
    uint32_t              hash;
    संरचना sockaddr_in6  *sin6;
#endif

#सचे (NGX_SUPPRESS_WARN)
    addr = 0;
#सचे (NGX_HAVE_INET6)
    hash = 0;
    sin6 = रिक्त;
#endif
#endif

    r = ctx->resolver;

    विकल्प (ctx->addr.sockaddr->sa_family) {

#सचे (NGX_HAVE_INET6)
    पक्ष AF_INET6:
        sin6 = (संरचना sockaddr_in6 *) ctx->addr.sockaddr;
        hash = ngx_crc32_short(sin6->sin6_addr.s6_addr, 16);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr6(r, &sin6->sin6_addr, hash);

        वृक्षः = &r->addr6_rbtree;
        resend_queue = &r->addr6_resend_queue;
        expire_queue = &r->addr6_expire_queue;

        भङ्ग;
#endif

    स्वभाव: /* AF_INET */
        sin = (संरचना sockaddr_in *) ctx->addr.sockaddr;
        addr = ntohl(sin->sin_addr.s_addr);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        वृक्षः = &r->addr_rbtree;
        resend_queue = &r->addr_resend_queue;
        expire_queue = &r->addr_expire_queue;
    }

    सचे (rn) {

        सचे (rn->valid >= ngx_time()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0, "resolve cached");

            ngx_queue_remove(&rn->queue);

            rn->expire = ngx_time() + r->expire;

            ngx_queue_निवेशय_head(expire_queue, &rn->queue);

            name = ngx_resolver_dup(r, rn->name, rn->nlen);
            सचे (name == रिक्त) {
                ngx_resolver_free(r, ctx);
                पटिनिवत्त NGX_ERROR;
            }

            ctx->name.len = rn->nlen;
            ctx->name.data = name;

            /* unlock addr mutex */

            ctx->state = NGX_OK;
            ctx->valid = rn->valid;

            ctx->handler(ctx);

            ngx_resolver_free(r, name);

            पटिनिवत्त NGX_OK;
        }

        सचे (rn->waiting) {
            सचे (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
                पटिनिवत्त NGX_ERROR;
            }

            ctx->next = rn->waiting;
            rn->waiting = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;
            ctx->ग्रन्थिः = rn;

            /* unlock addr mutex */

            पटिनिवत्त NGX_OK;
        }

        ngx_queue_remove(&rn->queue);

        ngx_resolver_free(r, rn->query);
        rn->query = रिक्त;
#सचे (NGX_HAVE_INET6)
        rn->query6 = रिक्त;
#endif

    } अञ्ञथा {
        rn = ngx_resolver_alloc(r, परिमाण(ngx_resolver_ग्रन्थिः_t));
        सचे (rn == रिक्त) {
            गच्छ failed;
        }

        विकल्प (ctx->addr.sockaddr->sa_family) {

#सचे (NGX_HAVE_INET6)
        पक्ष AF_INET6:
            rn->addr6 = sin6->sin6_addr;
            rn->ग्रन्थिः.कुञ्चिका = hash;
            भङ्ग;
#endif

        स्वभाव: /* AF_INET */
            rn->ग्रन्थिः.कुञ्चिका = addr;
        }

        rn->query = रिक्त;
#सचे (NGX_HAVE_INET6)
        rn->query6 = रिक्त;
#endif

        ngx_rbtree_निवेशय(वृक्षः, &rn->ग्रन्थिः);
    }

    सचे (ngx_resolver_create_addr_query(r, rn, &ctx->addr) != NGX_OK) {
        गच्छ failed;
    }

    rn->last_connection = r->last_connection++;
    सचे (r->last_connection == r->connections.nelts) {
        r->last_connection = 0;
    }

    rn->naddrs = (u_short) -1;
    rn->tcp = 0;
#सचे (NGX_HAVE_INET6)
    rn->naddrs6 = (u_short) -1;
    rn->tcp6 = 0;
#endif
    rn->nsrvs = 0;

    सचे (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->last_connection++;
        सचे (rn->last_connection == r->connections.nelts) {
            rn->last_connection = 0;
        }

        (शून्य) ngx_resolver_send_query(r, rn);
    }

    सचे (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
        गच्छ failed;
    }

    सचे (ngx_resolver_resend_empty(r)) {
        ngx_add_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_time() + r->resend_timeout;

    ngx_queue_निवेशय_head(resend_queue, &rn->queue);

    rn->code = 0;
    rn->cnlen = 0;
    rn->name = रिक्त;
    rn->nlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_मानम्;
    rn->waiting = ctx;

    /* unlock addr mutex */

    ctx->state = NGX_AGAIN;
    ctx->async = 1;
    ctx->ग्रन्थिः = rn;

    पटिनिवत्त NGX_OK;

failed:

    सचे (rn) {
        ngx_rbtree_delete(वृक्षः, &rn->ग्रन्थिः);

        सचे (rn->query) {
            ngx_resolver_free(r, rn->query);
        }

        ngx_resolver_free(r, rn);
    }

    /* unlock addr mutex */

    सचे (ctx->event) {
        ngx_resolver_free(r, ctx->event);
    }

    ngx_resolver_free(r, ctx);

    पटिनिवत्त NGX_ERROR;
}


शून्य
ngx_resolve_addr_done(ngx_resolver_ctx_t *ctx)
{
    ngx_queue_t          *expire_queue;
    ngx_rbtree_t         *वृक्षः;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_ग्रन्थिः_t  *rn;

    r = ctx->resolver;

    विकल्प (ctx->addr.sockaddr->sa_family) {

#सचे (NGX_HAVE_INET6)
    पक्ष AF_INET6:
        वृक्षः = &r->addr6_rbtree;
        expire_queue = &r->addr6_expire_queue;
        भङ्ग;
#endif

    स्वभाव: /* AF_INET */
        वृक्षः = &r->addr_rbtree;
        expire_queue = &r->addr_expire_queue;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve addr done: %i", ctx->state);

    सचे (ctx->event && ctx->event->timer_set) {
        ngx_del_timer(ctx->event);
    }

    /* lock addr mutex */

    सचे (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->ग्रन्थिः;

        सचे (rn) {
            p = &rn->waiting;
            w = rn->waiting;

            याव (w) {
                सचे (w == ctx) {
                    *p = w->next;

                    गच्छ done;
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

    ngx_resolver_expire(r, वृक्षः, expire_queue);

    /* unlock addr mutex */

    /* lock alloc mutex */

    सचे (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    सचे (r->event->timer_set && ngx_resolver_resend_empty(r)) {
        ngx_del_timer(r->event);
    }
}


स्थिर शून्य
ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *वृक्षः, ngx_queue_t *queue)
{
    time_t                now;
    ngx_uint_t            i;
    ngx_queue_t          *q;
    ngx_resolver_ग्रन्थिः_t  *rn;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver expire");

    now = ngx_time();

    अत्थाय (i = 0; i < 2; i++) {
        सचे (ngx_queue_empty(queue)) {
            पटिनिवत्त;
        }

        q = ngx_queue_last(queue);

        rn = ngx_queue_data(q, ngx_resolver_ग्रन्थिः_t, queue);

        सचे (now <= rn->expire) {
            पटिनिवत्त;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver expire \"%*s\"", (size_t) rn->nlen, rn->name);

        ngx_queue_remove(q);

        ngx_rbtree_delete(वृक्षः, &rn->ग्रन्थिः);

        ngx_resolver_free_ग्रन्थिः(r, rn);
    }
}


स्थिर ngx_int_t
ngx_resolver_send_query(ngx_resolver_t *r, ngx_resolver_ग्रन्थिः_t *rn)
{
    ngx_int_t                   rc;
    ngx_resolver_connection_t  *rec;

    rec = r->connections.elts;
    rec = &rec[rn->last_connection];

    सचे (rec->log.handler == रिक्त) {
        rec->log = *r->log;
        rec->log.handler = ngx_resolver_log_error;
        rec->log.data = rec;
        rec->log.action = "resolving";
    }

    सचे (rn->query && rn->naddrs == (u_short) -1) {
        rc = rn->tcp ? ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen)
                     : ngx_resolver_send_udp_query(r, rec, rn->query, rn->qlen);

        सचे (rc != NGX_OK) {
            पटिनिवत्त rc;
        }
    }

#सचे (NGX_HAVE_INET6)

    सचे (rn->query6 && rn->naddrs6 == (u_short) -1) {
        rc = rn->tcp6
                    ? ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen)
                    : ngx_resolver_send_udp_query(r, rec, rn->query6, rn->qlen);

        सचे (rc != NGX_OK) {
            पटिनिवत्त rc;
        }
    }

#endif

    पटिनिवत्त NGX_OK;
}


स्थिर ngx_int_t
ngx_resolver_send_udp_query(ngx_resolver_t *r, ngx_resolver_connection_t  *rec,
    u_char *query, u_short qlen)
{
    ssize_t  n;

    सचे (rec->udp == रिक्त) {
        सचे (ngx_udp_connect(rec) != NGX_OK) {
            पटिनिवत्त NGX_ERROR;
        }

        rec->udp->data = rec;
        rec->udp->पठ->handler = ngx_resolver_udp_read;
        rec->udp->पठ->resolver = 1;
    }

    n = ngx_send(rec->udp, query, qlen);

    सचे (n == NGX_ERROR) {
        गच्छ failed;
    }

    सचे ((size_t) n != (size_t) qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->log, 0, "send() incomplete");
        गच्छ failed;
    }

    पटिनिवत्त NGX_OK;

failed:

    ngx_close_connection(rec->udp);
    rec->udp = रिक्त;

    पटिनिवत्त NGX_ERROR;
}


स्थिर ngx_int_t
ngx_resolver_send_tcp_query(ngx_resolver_t *r, ngx_resolver_connection_t *rec,
    u_char *query, u_short qlen)
{
    ngx_buf_t  *b;
    ngx_int_t   rc;

    rc = NGX_OK;

    सचे (rec->tcp == रिक्त) {
        b = rec->read_buf;

        सचे (b == रिक्त) {
            b = ngx_resolver_calloc(r, परिमाण(ngx_buf_t));
            सचे (b == रिक्त) {
                पटिनिवत्त NGX_ERROR;
            }

            b->start = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_RSIZE);
            सचे (b->start == रिक्त) {
                ngx_resolver_free(r, b);
                पटिनिवत्त NGX_ERROR;
            }

            b->end = b->start + NGX_RESOLVER_TCP_RSIZE;

            rec->read_buf = b;
        }

        b->pos = b->start;
        b->last = b->start;

        b = rec->write_buf;

        सचे (b == रिक्त) {
            b = ngx_resolver_calloc(r, परिमाण(ngx_buf_t));
            सचे (b == रिक्त) {
                पटिनिवत्त NGX_ERROR;
            }

            b->start = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_WSIZE);
            सचे (b->start == रिक्त) {
                ngx_resolver_free(r, b);
                पटिनिवत्त NGX_ERROR;
            }

            b->end = b->start + NGX_RESOLVER_TCP_WSIZE;

            rec->write_buf = b;
        }

        b->pos = b->start;
        b->last = b->start;

        rc = ngx_tcp_connect(rec);
        सचे (rc == NGX_ERROR) {
            पटिनिवत्त NGX_ERROR;
        }

        rec->tcp->data = rec;
        rec->tcp->लिख->handler = ngx_resolver_tcp_write;
        rec->tcp->लिख->cancelable = 1;
        rec->tcp->पठ->handler = ngx_resolver_tcp_read;
        rec->tcp->पठ->resolver = 1;

        ngx_add_timer(rec->tcp->लिख, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    b = rec->write_buf;

    सचे (b->end - b->last <  2 + qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->log, 0, "buffer overflow");
        पटिनिवत्त NGX_ERROR;
    }

    *b->last++ = (u_char) (qlen >> 8);
    *b->last++ = (u_char) qlen;
    b->last = ngx_cpymem(b->last, query, qlen);

    सचे (rc == NGX_OK) {
        ngx_resolver_tcp_write(rec->tcp->लिख);
    }

    पटिनिवत्त NGX_OK;
}


स्थिर शून्य
ngx_resolver_resend_handler(ngx_event_t *ev)
{
    time_t           timer, atimer, stimer, ntimer;
#सचे (NGX_HAVE_INET6)
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

#सचे (NGX_HAVE_INET6)

    /* lock addr6 mutex */

    a6timer = ngx_resolver_resend(r, &r->addr6_rbtree, &r->addr6_resend_queue);

    /* unlock addr6 mutex */

#endif

    timer = ntimer;

    सचे (timer == 0) {
        timer = atimer;

    } अञ्ञथा सचे (atimer) {
        timer = ngx_min(timer, atimer);
    }

    सचे (timer == 0) {
        timer = stimer;

    } अञ्ञथा सचे (stimer) {
        timer = ngx_min(timer, stimer);
    }

#सचे (NGX_HAVE_INET6)

    सचे (timer == 0) {
        timer = a6timer;

    } अञ्ञथा सचे (a6timer) {
        timer = ngx_min(timer, a6timer);
    }

#endif

    सचे (timer) {
        ngx_add_timer(r->event, (ngx_msec_t) (timer * 1000));
    }
}


स्थिर time_t
ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *वृक्षः, ngx_queue_t *queue)
{
    time_t                now;
    ngx_queue_t          *q;
    ngx_resolver_ग्रन्थिः_t  *rn;

    now = ngx_time();

    अत्थाय ( ;; ) {
        सचे (ngx_queue_empty(queue)) {
            पटिनिवत्त 0;
        }

        q = ngx_queue_last(queue);

        rn = ngx_queue_data(q, ngx_resolver_ग्रन्थिः_t, queue);

        सचे (now < rn->expire) {
            पटिनिवत्त rn->expire - now;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver resend \"%*s\" %p",
                       (size_t) rn->nlen, rn->name, rn->waiting);

        ngx_queue_remove(q);

        सचे (rn->waiting) {

            सचे (++rn->last_connection == r->connections.nelts) {
                rn->last_connection = 0;
            }

            (शून्य) ngx_resolver_send_query(r, rn);

            rn->expire = now + r->resend_timeout;

            ngx_queue_निवेशय_head(queue, q);

            अनुवर्त;
        }

        ngx_rbtree_delete(वृक्षः, &rn->ग्रन्थिः);

        ngx_resolver_free_ग्रन्थिः(r, rn);
    }
}


स्थिर ngx_uint_t
ngx_resolver_resend_empty(ngx_resolver_t *r)
{
    पटिनिवत्त ngx_queue_empty(&r->name_resend_queue)
           && ngx_queue_empty(&r->srv_resend_queue)
#सचे (NGX_HAVE_INET6)
           && ngx_queue_empty(&r->addr6_resend_queue)
#endif
           && ngx_queue_empty(&r->addr_resend_queue);
}


स्थिर शून्य
ngx_resolver_udp_read(ngx_event_t *rev)
{
    ssize_t                     n;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;
    u_char                      buf[NGX_RESOLVER_UDP_SIZE];

    c = rev->data;
    rec = c->data;

    करोहि {
        n = ngx_udp_recv(c, buf, NGX_RESOLVER_UDP_SIZE);

        सचे (n == NGX_AGAIN) {
            भङ्ग;
        }

        सचे (n == NGX_ERROR) {
            गच्छ failed;
        }

        ngx_resolver_process_response(rec->resolver, buf, n, 0);

    } याव (rev->ready);

    सचे (ngx_handle_read_event(rev, 0) != NGX_OK) {
        गच्छ failed;
    }

    पटिनिवत्त;

failed:

    ngx_close_connection(rec->udp);
    rec->udp = रिक्त;
}


स्थिर शून्य
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

    सचे (wev->timedout) {
        गच्छ failed;
    }

    sent = c->sent;

    याव (wev->ready && b->pos < b->last) {
        n = ngx_send(c, b->pos, b->last - b->pos);

        सचे (n == NGX_AGAIN) {
            भङ्ग;
        }

        सचे (n == NGX_ERROR) {
            गच्छ failed;
        }

        b->pos += n;
    }

    सचे (b->pos != b->start) {
        b->last = ngx_movemem(b->start, b->pos, b->last - b->pos);
        b->pos = b->start;
    }

    सचे (c->sent != sent) {
        ngx_add_timer(wev, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    सचे (ngx_handle_write_event(wev, 0) != NGX_OK) {
        गच्छ failed;
    }

    पटिनिवत्त;

failed:

    ngx_close_connection(c);
    rec->tcp = रिक्त;
}


स्थिर शून्य
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

    याव (rev->ready) {
        n = ngx_recv(c, b->last, b->end - b->last);

        सचे (n == NGX_AGAIN) {
            भङ्ग;
        }

        सचे (n == NGX_ERROR || n == 0) {
            गच्छ failed;
        }

        b->last += n;

        अत्थाय ( ;; ) {
            p = b->pos;
            size = b->last - p;

            सचे (size < 2) {
                भङ्ग;
            }

            qlen = (u_short) *p++ << 8;
            qlen += *p++;

            सचे (size < (size_t) (2 + qlen)) {
                भङ्ग;
            }

            ngx_resolver_process_response(r, p, qlen, 1);

            b->pos += 2 + qlen;
        }

        सचे (b->pos != b->start) {
            b->last = ngx_movemem(b->start, b->pos, b->last - b->pos);
            b->pos = b->start;
        }
    }

    सचे (ngx_handle_read_event(rev, 0) != NGX_OK) {
        गच्छ failed;
    }

    पटिनिवत्त;

failed:

    ngx_close_connection(c);
    rec->tcp = रिक्त;
}


स्थिर शून्य
ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t tcp)
{
    वर्ण                 *err;
    ngx_uint_t            i, times, ident, qident, flags, code, nqs, nan, trunc,
                          qtype, qclass;
#सचे (NGX_HAVE_INET6)
    ngx_uint_t            qident6;
#endif
    ngx_queue_t          *q;
    ngx_resolver_qs_t    *qs;
    ngx_resolver_hdr_t   *response;
    ngx_resolver_ग्रन्थिः_t  *rn;

    सचे (n < परिमाण(ngx_resolver_hdr_t)) {
        गच्छ short_response;
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
    सचे ((flags & 0xf870) != 0x8000 || (trunc && tcp)) {
        ngx_log_error(r->log_level, r->log, 0,
                      "invalid %s DNS response %ui fl:%04Xi",
                      tcp ? "TCP" : "UDP", ident, flags);
        पटिनिवत्त;
    }

    code = flags & 0xf;

    सचे (code == NGX_RESOLVE_FORMERR) {

        times = 0;

        अत्थाय (q = ngx_queue_head(&r->name_resend_queue);
             q != ngx_queue_sentinel(&r->name_resend_queue) && times++ < 100;
             q = ngx_queue_next(q))
        {
            rn = ngx_queue_data(q, ngx_resolver_ग्रन्थिः_t, queue);

            सचे (rn->query) {
                qident = (rn->query[0] << 8) + rn->query[1];

                सचे (qident == ident) {
                    गच्छ dns_error_name;
                }
            }

#सचे (NGX_HAVE_INET6)
            सचे (rn->query6) {
                qident6 = (rn->query6[0] << 8) + rn->query6[1];

                सचे (qident6 == ident) {
                    गच्छ dns_error_name;
                }
            }
#endif
        }

        गच्छ dns_error;
    }

    सचे (code > NGX_RESOLVE_REFUSED) {
        गच्छ dns_error;
    }

    सचे (nqs != 1) {
        err = "invalid number of questions in DNS response";
        गच्छ done;
    }

    i = परिमाण(ngx_resolver_hdr_t);

    याव (i < (ngx_uint_t) n) {

        सचे (buf[i] & 0xc0) {
            err = "unexpected compression pointer in DNS response";
            गच्छ done;
        }

        सचे (buf[i] == '\0') {
            गच्छ found;
        }

        i += 1 + buf[i];
    }

    गच्छ short_response;

found:

    सचे (i++ == परिमाण(ngx_resolver_hdr_t)) {
        err = "zero-length domain name in DNS response";
        गच्छ done;
    }

    सचे (i + परिमाण(ngx_resolver_qs_t) + nan * (2 + परिमाण(ngx_resolver_an_t))
        > (ngx_uint_t) n)
    {
        गच्छ short_response;
    }

    qs = (ngx_resolver_qs_t *) &buf[i];

    qtype = (qs->type_hi << 8) + qs->type_lo;
    qclass = (qs->class_hi << 8) + qs->class_lo;

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver DNS response qt:%ui cl:%ui", qtype, qclass);

    सचे (qclass != 1) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unknown query class %ui in DNS response", qclass);
        पटिनिवत्त;
    }

    विकल्प (qtype) {

    पक्ष NGX_RESOLVE_A:
#सचे (NGX_HAVE_INET6)
    पक्ष NGX_RESOLVE_AAAA:
#endif

        ngx_resolver_process_a(r, buf, n, ident, code, qtype, nan, trunc,
                               i + परिमाण(ngx_resolver_qs_t));

        भङ्ग;

    पक्ष NGX_RESOLVE_SRV:

        ngx_resolver_process_srv(r, buf, n, ident, code, nan, trunc,
                                 i + परिमाण(ngx_resolver_qs_t));

        भङ्ग;

    पक्ष NGX_RESOLVE_PTR:

        ngx_resolver_process_ptr(r, buf, n, ident, code, nan);

        भङ्ग;

    स्वभाव:
        ngx_log_error(r->log_level, r->log, 0,
                      "unknown query type %ui in DNS response", qtype);
        पटिनिवत्त;
    }

    पटिनिवत्त;

short_response:

    err = "short DNS response";

done:

    ngx_log_error(r->log_level, r->log, 0, err);

    पटिनिवत्त;

dns_error_name:

    ngx_log_error(r->log_level, r->log, 0,
                  "DNS error (%ui: %s), query id:%ui, name:\"%*s\"",
                  code, ngx_resolver_strerror(code), ident,
                  (size_t) rn->nlen, rn->name);
    पटिनिवत्त;

dns_error:

    ngx_log_error(r->log_level, r->log, 0,
                  "DNS error (%ui: %s), query id:%ui",
                  code, ngx_resolver_strerror(code), ident);
    पटिनिवत्त;
}


स्थिर शून्य
ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans)
{
    वर्ण                       *err;
    u_char                     *cname;
    size_t                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    in_addr_t                  *addr;
    ngx_str_t                   name;
    ngx_uint_t                  type, class, qident, naddrs, a, i, j, start;
#सचे (NGX_HAVE_INET6)
    संरचना in6_addr            *addr6;
#endif
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *next;
    ngx_resolver_ग्रन्थिः_t        *rn;
    ngx_resolver_addr_t        *addrs;
    ngx_resolver_connection_t  *rec;

    सचे (ngx_resolver_copy(r, &name, buf,
                          buf + परिमाण(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        पटिनिवत्त;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver qs:%V", &name);

    hash = ngx_crc32_short(name.data, name.len);

    /* lock name mutex */

    rn = ngx_resolver_lookup_name(r, &name, hash);

    सचे (rn == रिक्त) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.data);
        गच्छ failed;
    }

    विकल्प (qtype) {

#सचे (NGX_HAVE_INET6)
    पक्ष NGX_RESOLVE_AAAA:

        सचे (rn->query6 == रिक्त || rn->naddrs6 != (u_short) -1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.data);
            गच्छ failed;
        }

        सचे (trunc && rn->tcp6) {
            ngx_resolver_free(r, name.data);
            गच्छ failed;
        }

        qident = (rn->query6[0] << 8) + rn->query6[1];

        भङ्ग;
#endif

    स्वभाव: /* NGX_RESOLVE_A */

        सचे (rn->query == रिक्त || rn->naddrs != (u_short) -1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.data);
            गच्छ failed;
        }

        सचे (trunc && rn->tcp) {
            ngx_resolver_free(r, name.data);
            गच्छ failed;
        }

        qident = (rn->query[0] << 8) + rn->query[1];
    }

    सचे (ident != qident) {
        ngx_log_error(r->log_level, r->log, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.data);
        गच्छ failed;
    }

    ngx_resolver_free(r, name.data);

    सचे (trunc) {

        ngx_queue_remove(&rn->queue);

        सचे (rn->waiting == रिक्त) {
            ngx_rbtree_delete(&r->name_rbtree, &rn->ग्रन्थिः);
            ngx_resolver_free_ग्रन्थिः(r, rn);
            गच्छ next;
        }

        rec = r->connections.elts;
        rec = &rec[rn->last_connection];

        विकल्प (qtype) {

#सचे (NGX_HAVE_INET6)
        पक्ष NGX_RESOLVE_AAAA:

            rn->tcp6 = 1;

            (शून्य) ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen);

            भङ्ग;
#endif

        स्वभाव: /* NGX_RESOLVE_A */

            rn->tcp = 1;

            (शून्य) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);
        }

        rn->expire = ngx_time() + r->resend_timeout;

        ngx_queue_निवेशय_head(&r->name_resend_queue, &rn->queue);

        गच्छ next;
    }

    सचे (code == 0 && rn->code) {
        code = rn->code;
    }

    सचे (code == 0 && nan == 0) {

#सचे (NGX_HAVE_INET6)
        विकल्प (qtype) {

        पक्ष NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            सचे (rn->naddrs == (u_short) -1) {
                गच्छ next;
            }

            सचे (rn->naddrs) {
                गच्छ export;
            }

            भङ्ग;

        स्वभाव: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            सचे (rn->naddrs6 == (u_short) -1) {
                गच्छ next;
            }

            सचे (rn->naddrs6) {
                गच्छ export;
            }
        }
#endif

        code = NGX_RESOLVE_NXDOMAIN;
    }

    सचे (code) {

#सचे (NGX_HAVE_INET6)
        विकल्प (qtype) {

        पक्ष NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            सचे (rn->naddrs == (u_short) -1) {
                rn->code = (u_char) code;
                गच्छ next;
            }

            भङ्ग;

        स्वभाव: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            सचे (rn->naddrs6 == (u_short) -1) {
                rn->code = (u_char) code;
                गच्छ next;
            }
        }
#endif

        next = rn->waiting;
        rn->waiting = रिक्त;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(&r->name_rbtree, &rn->ग्रन्थिः);

        /* unlock name mutex */

        याव (next) {
            ctx = next;
            ctx->state = code;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            next = ctx->next;

            ctx->handler(ctx);
        }

        ngx_resolver_free_ग्रन्थिः(r, rn);

        पटिनिवत्त;
    }

    i = ans;
    naddrs = 0;
    cname = रिक्त;

    अत्थाय (a = 0; a < nan; a++) {

        start = i;

        याव (i < n) {

            सचे (buf[i] & 0xc0) {
                i += 2;
                गच्छ found;
            }

            सचे (buf[i] == 0) {
                i++;
                गच्छ test_length;
            }

            i += 1 + buf[i];
        }

        गच्छ short_response;

    test_length:

        सचे (i - start < 2) {
            err = "invalid name in DNS response";
            गच्छ invalid;
        }

    found:

        सचे (i + परिमाण(ngx_resolver_an_t) >= n) {
            गच्छ short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        सचे (class != 1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR class %ui in DNS response", class);
            गच्छ failed;
        }

        सचे (ttl < 0) {
            ttl = 0;
        }

        rn->ttl = ngx_min(rn->ttl, (uint32_t) ttl);

        i += परिमाण(ngx_resolver_an_t);

        विकल्प (type) {

        पक्ष NGX_RESOLVE_A:

            सचे (qtype != NGX_RESOLVE_A) {
                err = "unexpected A record in DNS response";
                गच्छ invalid;
            }

            सचे (len != 4) {
                err = "invalid A record in DNS response";
                गच्छ invalid;
            }

            सचे (i + 4 > n) {
                गच्छ short_response;
            }

            naddrs++;

            भङ्ग;

#सचे (NGX_HAVE_INET6)
        पक्ष NGX_RESOLVE_AAAA:

            सचे (qtype != NGX_RESOLVE_AAAA) {
                err = "unexpected AAAA record in DNS response";
                गच्छ invalid;
            }

            सचे (len != 16) {
                err = "invalid AAAA record in DNS response";
                गच्छ invalid;
            }

            सचे (i + 16 > n) {
                गच्छ short_response;
            }

            naddrs++;

            भङ्ग;
#endif

        पक्ष NGX_RESOLVE_CNAME:

            cname = &buf[i];

            भङ्ग;

        पक्ष NGX_RESOLVE_DNAME:

            भङ्ग;

        स्वभाव:

            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver naddrs:%ui cname:%p ttl:%uD",
                   naddrs, cname, rn->ttl);

    सचे (naddrs) {

        विकल्प (qtype) {

#सचे (NGX_HAVE_INET6)
        पक्ष NGX_RESOLVE_AAAA:

            सचे (naddrs == 1) {
                addr6 = &rn->u6.addr6;
                rn->naddrs6 = 1;

            } अञ्ञथा {
                addr6 = ngx_resolver_alloc(r, naddrs * परिमाण(संरचना in6_addr));
                सचे (addr6 == रिक्त) {
                    गच्छ failed;
                }

                rn->u6.addrs6 = addr6;
                rn->naddrs6 = (u_short) naddrs;
            }

#सचे (NGX_SUPPRESS_WARN)
            addr = रिक्त;
#endif

            भङ्ग;
#endif

        स्वभाव: /* NGX_RESOLVE_A */

            सचे (naddrs == 1) {
                addr = &rn->u.addr;
                rn->naddrs = 1;

            } अञ्ञथा {
                addr = ngx_resolver_alloc(r, naddrs * परिमाण(in_addr_t));
                सचे (addr == रिक्त) {
                    गच्छ failed;
                }

                rn->u.addrs = addr;
                rn->naddrs = (u_short) naddrs;
            }

#सचे (NGX_HAVE_INET6 && NGX_SUPPRESS_WARN)
            addr6 = रिक्त;
#endif
        }

        j = 0;
        i = ans;

        अत्थाय (a = 0; a < nan; a++) {

            अत्थाय ( ;; ) {

                सचे (buf[i] & 0xc0) {
                    i += 2;
                    भङ्ग;
                }

                सचे (buf[i] == 0) {
                    i++;
                    भङ्ग;
                }

                i += 1 + buf[i];
            }

            an = (ngx_resolver_an_t *) &buf[i];

            type = (an->type_hi << 8) + an->type_lo;
            len = (an->len_hi << 8) + an->len_lo;

            i += परिमाण(ngx_resolver_an_t);

            सचे (type == NGX_RESOLVE_A) {

                addr[j] = htonl((buf[i] << 24) + (buf[i + 1] << 16)
                                + (buf[i + 2] << 8) + (buf[i + 3]));

                सचे (++j == naddrs) {

#सचे (NGX_HAVE_INET6)
                    सचे (rn->naddrs6 == (u_short) -1) {
                        गच्छ next;
                    }
#endif

                    भङ्ग;
                }
            }

#सचे (NGX_HAVE_INET6)
            अञ्ञथा सचे (type == NGX_RESOLVE_AAAA) {

                ngx_memcpy(addr6[j].s6_addr, &buf[i], 16);

                सचे (++j == naddrs) {

                    सचे (rn->naddrs == (u_short) -1) {
                        गच्छ next;
                    }

                    भङ्ग;
                }
            }
#endif

            i += len;
        }
    }

    विकल्प (qtype) {

#सचे (NGX_HAVE_INET6)
    पक्ष NGX_RESOLVE_AAAA:

        सचे (rn->naddrs6 == (u_short) -1) {
            rn->naddrs6 = 0;
        }

        भङ्ग;
#endif

    स्वभाव: /* NGX_RESOLVE_A */

        सचे (rn->naddrs == (u_short) -1) {
            rn->naddrs = 0;
        }
    }

    सचे (rn->naddrs != (u_short) -1
#सचे (NGX_HAVE_INET6)
        && rn->naddrs6 != (u_short) -1
#endif
        && rn->naddrs
#सचे (NGX_HAVE_INET6)
           + rn->naddrs6
#endif
           > 0)
    {

#सचे (NGX_HAVE_INET6)
    export:
#endif

        naddrs = rn->naddrs;
#सचे (NGX_HAVE_INET6)
        naddrs += rn->naddrs6;
#endif

        सचे (naddrs == 1 && rn->naddrs == 1) {
            addrs = रिक्त;

        } अञ्ञथा {
            addrs = ngx_resolver_export(r, rn, 0);
            सचे (addrs == रिक्त) {
                गच्छ failed;
            }
        }

        ngx_queue_remove(&rn->queue);

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_निवेशय_head(&r->name_expire_queue, &rn->queue);

        next = rn->waiting;
        rn->waiting = रिक्त;

        /* unlock name mutex */

        याव (next) {
            ctx = next;
            ctx->state = NGX_OK;
            ctx->valid = rn->valid;
            ctx->naddrs = naddrs;

            सचे (addrs == रिक्त) {
                ctx->addrs = &ctx->addr;
                ctx->addr.sockaddr = (संरचना sockaddr *) &ctx->sin;
                ctx->addr.socklen = परिमाण(संरचना sockaddr_in);
                ngx_memzero(&ctx->sin, परिमाण(संरचना sockaddr_in));
                ctx->sin.sin_family = AF_INET;
                ctx->sin.sin_addr.s_addr = rn->u.addr;

            } अञ्ञथा {
                ctx->addrs = addrs;
            }

            next = ctx->next;

            ctx->handler(ctx);
        }

        सचे (addrs != रिक्त) {
            ngx_resolver_free(r, addrs->sockaddr);
            ngx_resolver_free(r, addrs);
        }

        ngx_resolver_free(r, rn->query);
        rn->query = रिक्त;
#सचे (NGX_HAVE_INET6)
        rn->query6 = रिक्त;
#endif

        पटिनिवत्त;
    }

    सचे (cname) {

        /* CNAME only */

        सचे (rn->naddrs == (u_short) -1
#सचे (NGX_HAVE_INET6)
            || rn->naddrs6 == (u_short) -1
#endif
            )
        {
            गच्छ next;
        }

        सचे (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            गच्छ failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_queue_remove(&rn->queue);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.data;

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_निवेशय_head(&r->name_expire_queue, &rn->queue);

        ngx_resolver_free(r, rn->query);
        rn->query = रिक्त;
#सचे (NGX_HAVE_INET6)
        rn->query6 = रिक्त;
#endif

        ctx = rn->waiting;
        rn->waiting = रिक्त;

        सचे (ctx) {

            सचे (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                करोहि {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    next = ctx->next;

                    ctx->handler(ctx);

                    ctx = next;
                } याव (ctx);

                पटिनिवत्त;
            }

            अत्थाय (next = ctx; next; next = next->next) {
                next->ग्रन्थिः = रिक्त;
            }

            (शून्य) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        पटिनिवत्त;
    }

    ngx_log_error(r->log_level, r->log, 0,
                  "no A or CNAME types in DNS response");
    पटिनिवत्त;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->log, 0, err);

    पटिनिवत्त;

failed:

next:

    /* unlock name mutex */

    पटिनिवत्त;
}


स्थिर शून्य
ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans)
{
    वर्ण                       *err;
    u_char                     *cname;
    size_t                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    ngx_str_t                   name;
    ngx_uint_t                  type, qident, class, start, nsrvs, a, i, j;
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *next;
    ngx_resolver_srv_t         *srvs;
    ngx_resolver_ग्रन्थिः_t        *rn;
    ngx_resolver_connection_t  *rec;

    सचे (ngx_resolver_copy(r, &name, buf,
                          buf + परिमाण(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        पटिनिवत्त;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver qs:%V", &name);

    hash = ngx_crc32_short(name.data, name.len);

    rn = ngx_resolver_lookup_srv(r, &name, hash);

    सचे (rn == रिक्त || rn->query == रिक्त) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.data);
        गच्छ failed;
    }

    सचे (trunc && rn->tcp) {
        ngx_resolver_free(r, name.data);
        गच्छ failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    सचे (ident != qident) {
        ngx_log_error(r->log_level, r->log, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.data);
        गच्छ failed;
    }

    ngx_resolver_free(r, name.data);

    सचे (trunc) {

        ngx_queue_remove(&rn->queue);

        सचे (rn->waiting == रिक्त) {
            ngx_rbtree_delete(&r->srv_rbtree, &rn->ग्रन्थिः);
            ngx_resolver_free_ग्रन्थिः(r, rn);
            पटिनिवत्त;
        }

        rec = r->connections.elts;
        rec = &rec[rn->last_connection];

        rn->tcp = 1;

        (शून्य) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);

        rn->expire = ngx_time() + r->resend_timeout;

        ngx_queue_निवेशय_head(&r->srv_resend_queue, &rn->queue);

        पटिनिवत्त;
    }

    सचे (code == 0 && rn->code) {
        code = rn->code;
    }

    सचे (code == 0 && nan == 0) {
        code = NGX_RESOLVE_NXDOMAIN;
    }

    सचे (code) {
        next = rn->waiting;
        rn->waiting = रिक्त;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(&r->srv_rbtree, &rn->ग्रन्थिः);

        याव (next) {
            ctx = next;
            ctx->state = code;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            next = ctx->next;

            ctx->handler(ctx);
        }

        ngx_resolver_free_ग्रन्थिः(r, rn);

        पटिनिवत्त;
    }

    i = ans;
    nsrvs = 0;
    cname = रिक्त;

    अत्थाय (a = 0; a < nan; a++) {

        start = i;

        याव (i < n) {

            सचे (buf[i] & 0xc0) {
                i += 2;
                गच्छ found;
            }

            सचे (buf[i] == 0) {
                i++;
                गच्छ test_length;
            }

            i += 1 + buf[i];
        }

        गच्छ short_response;

    test_length:

        सचे (i - start < 2) {
            err = "invalid name DNS response";
            गच्छ invalid;
        }

    found:

        सचे (i + परिमाण(ngx_resolver_an_t) >= n) {
            गच्छ short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        सचे (class != 1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR class %ui in DNS response", class);
            गच्छ failed;
        }

        सचे (ttl < 0) {
            ttl = 0;
        }

        rn->ttl = ngx_min(rn->ttl, (uint32_t) ttl);

        i += परिमाण(ngx_resolver_an_t);

        विकल्प (type) {

        पक्ष NGX_RESOLVE_SRV:

            सचे (i + 6 >= n) {
                गच्छ short_response;
            }

            सचे (ngx_resolver_copy(r, रिक्त, buf, &buf[i + 6], buf + n)
                != NGX_OK)
            {
                गच्छ failed;
            }

            nsrvs++;

            भङ्ग;

        पक्ष NGX_RESOLVE_CNAME:

            cname = &buf[i];

            भङ्ग;

        पक्ष NGX_RESOLVE_DNAME:

            भङ्ग;

        स्वभाव:

            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver nsrvs:%ui cname:%p ttl:%uD",
                   nsrvs, cname, rn->ttl);

    सचे (nsrvs) {

        srvs = ngx_resolver_calloc(r, nsrvs * परिमाण(ngx_resolver_srv_t));
        सचे (srvs == रिक्त) {
            गच्छ failed;
        }

        rn->u.srvs = srvs;
        rn->nsrvs = (u_short) nsrvs;

        j = 0;
        i = ans;

        अत्थाय (a = 0; a < nan; a++) {

            अत्थाय ( ;; ) {

                सचे (buf[i] & 0xc0) {
                    i += 2;
                    भङ्ग;
                }

                सचे (buf[i] == 0) {
                    i++;
                    भङ्ग;
                }

                i += 1 + buf[i];
            }

            an = (ngx_resolver_an_t *) &buf[i];

            type = (an->type_hi << 8) + an->type_lo;
            len = (an->len_hi << 8) + an->len_lo;

            i += परिमाण(ngx_resolver_an_t);

            सचे (type == NGX_RESOLVE_SRV) {

                srvs[j].priority = (buf[i] << 8) + buf[i + 1];
                srvs[j].weight = (buf[i + 2] << 8) + buf[i + 3];

                सचे (srvs[j].weight == 0) {
                    srvs[j].weight = 1;
                }

                srvs[j].port = (buf[i + 4] << 8) + buf[i + 5];

                सचे (ngx_resolver_copy(r, &srvs[j].name, buf, &buf[i + 6],
                                      buf + n)
                    != NGX_OK)
                {
                    गच्छ failed;
                }

                j++;
            }

            i += len;
        }

        ngx_sort(srvs, nsrvs, परिमाण(ngx_resolver_srv_t),
                 ngx_resolver_cmp_srvs);

        ngx_resolver_free(r, rn->query);
        rn->query = रिक्त;

        ngx_queue_remove(&rn->queue);

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_निवेशय_head(&r->srv_expire_queue, &rn->queue);

        next = rn->waiting;
        rn->waiting = रिक्त;

        याव (next) {
            ctx = next;
            next = ctx->next;

            ngx_resolver_resolve_srv_names(ctx, rn);
        }

        पटिनिवत्त;
    }

    rn->nsrvs = 0;

    सचे (cname) {

        /* CNAME only */

        सचे (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            गच्छ failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_queue_remove(&rn->queue);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.data;

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_निवेशय_head(&r->srv_expire_queue, &rn->queue);

        ngx_resolver_free(r, rn->query);
        rn->query = रिक्त;
#सचे (NGX_HAVE_INET6)
        rn->query6 = रिक्त;
#endif

        ctx = rn->waiting;
        rn->waiting = रिक्त;

        सचे (ctx) {

            सचे (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                करोहि {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    next = ctx->next;

                    ctx->handler(ctx);

                    ctx = next;
                } याव (ctx);

                पटिनिवत्त;
            }

            अत्थाय (next = ctx; next; next = next->next) {
                next->ग्रन्थिः = रिक्त;
            }

            (शून्य) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        पटिनिवत्त;
    }

    ngx_log_error(r->log_level, r->log, 0, "no SRV type in DNS response");

    पटिनिवत्त;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->log, 0, err);

    पटिनिवत्त;

failed:

    /* unlock name mutex */

    पटिनिवत्त;
}


स्थिर शून्य
ngx_resolver_resolve_srv_names(ngx_resolver_ctx_t *ctx, ngx_resolver_ग्रन्थिः_t *rn)
{
    ngx_uint_t                i;
    ngx_resolver_t           *r;
    ngx_resolver_ctx_t       *cctx;
    ngx_resolver_srv_name_t  *srvs;

    r = ctx->resolver;

    ctx->ग्रन्थिः = रिक्त;
    ctx->state = NGX_OK;
    ctx->valid = rn->valid;
    ctx->गणनम् = rn->nsrvs;

    srvs = ngx_resolver_calloc(r, rn->nsrvs * परिमाण(ngx_resolver_srv_name_t));
    सचे (srvs == रिक्त) {
        गच्छ failed;
    }

    ctx->srvs = srvs;
    ctx->nsrvs = rn->nsrvs;

    सचे (ctx->event && ctx->event->timer_set) {
        ngx_del_timer(ctx->event);
    }

    अत्थाय (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
        srvs[i].name.data = ngx_resolver_alloc(r, rn->u.srvs[i].name.len);
        सचे (srvs[i].name.data == रिक्त) {
            गच्छ failed;
        }

        srvs[i].name.len = rn->u.srvs[i].name.len;
        ngx_memcpy(srvs[i].name.data, rn->u.srvs[i].name.data,
                   srvs[i].name.len);

        cctx = ngx_resolve_start(r, रिक्त);
        सचे (cctx == रिक्त) {
            गच्छ failed;
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

        सचे (ngx_resolve_name(cctx) == NGX_ERROR) {
            srvs[i].ctx = रिक्त;
            गच्छ failed;
        }
    }

    पटिनिवत्त;

failed:

    ctx->state = NGX_ERROR;
    ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

    ctx->handler(ctx);
}


स्थिर शून्य
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

    ctx->गणनम्--;
    ctx->async |= cctx->async;

    srv->ctx = रिक्त;
    srv->state = cctx->state;

    सचे (cctx->naddrs) {

        ctx->valid = ngx_min(ctx->valid, cctx->valid);

        addrs = ngx_resolver_calloc(r, cctx->naddrs * परिमाण(ngx_addr_t));
        सचे (addrs == रिक्त) {
            srv->state = NGX_ERROR;
            गच्छ done;
        }

        sockaddr = ngx_resolver_alloc(r, cctx->naddrs * परिमाण(ngx_sockaddr_t));
        सचे (sockaddr == रिक्त) {
            ngx_resolver_free(r, addrs);
            srv->state = NGX_ERROR;
            गच्छ done;
        }

        अत्थाय (i = 0; i < cctx->naddrs; i++) {
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

    सचे (ctx->गणनम् == 0) {
        ngx_resolver_report_srv(r, ctx);
    }
}


स्थिर शून्य
ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan)
{
    वर्ण                 *err;
    size_t                len;
    in_addr_t             addr;
    int32_t               ttl;
    ngx_int_t             octet;
    ngx_str_t             name;
    ngx_uint_t            mask, type, class, qident, a, i, start;
    ngx_queue_t          *expire_queue;
    ngx_rbtree_t         *वृक्षः;
    ngx_resolver_an_t    *an;
    ngx_resolver_ctx_t   *ctx, *next;
    ngx_resolver_ग्रन्थिः_t  *rn;
#सचे (NGX_HAVE_INET6)
    uint32_t              hash;
    ngx_int_t             digit;
    संरचना in6_addr       addr6;
#endif

    सचे (ngx_resolver_copy(r, &name, buf,
                          buf + परिमाण(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        पटिनिवत्त;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver qs:%V", &name);

    /* AF_INET */

    addr = 0;
    i = परिमाण(ngx_resolver_hdr_t);

    अत्थाय (mask = 0; mask < 32; mask += 8) {
        len = buf[i++];

        octet = ngx_atoi(&buf[i], len);
        सचे (octet == NGX_ERROR || octet > 255) {
            गच्छ invalid_in_addr_arpa;
        }

        addr += octet << mask;
        i += len;
    }

    सचे (ngx_strcasecmp(&buf[i], (u_char *) "\7in-addr\4arpa") == 0) {
        i += परिमाण("\7in-addr\4arpa");

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        वृक्षः = &r->addr_rbtree;
        expire_queue = &r->addr_expire_queue;

        गच्छ valid;
    }

invalid_in_addr_arpa:

#सचे (NGX_HAVE_INET6)

    i = परिमाण(ngx_resolver_hdr_t);

    अत्थाय (octet = 15; octet >= 0; octet--) {
        सचे (buf[i++] != '\1') {
            गच्छ invalid_ip6_arpa;
        }

        digit = ngx_hextoi(&buf[i++], 1);
        सचे (digit == NGX_ERROR) {
            गच्छ invalid_ip6_arpa;
        }

        addr6.s6_addr[octet] = (u_char) digit;

        सचे (buf[i++] != '\1') {
            गच्छ invalid_ip6_arpa;
        }

        digit = ngx_hextoi(&buf[i++], 1);
        सचे (digit == NGX_ERROR) {
            गच्छ invalid_ip6_arpa;
        }

        addr6.s6_addr[octet] += (u_char) (digit * 16);
    }

    सचे (ngx_strcasecmp(&buf[i], (u_char *) "\3ip6\4arpa") == 0) {
        i += परिमाण("\3ip6\4arpa");

        /* lock addr mutex */

        hash = ngx_crc32_short(addr6.s6_addr, 16);
        rn = ngx_resolver_lookup_addr6(r, &addr6, hash);

        वृक्षः = &r->addr6_rbtree;
        expire_queue = &r->addr6_expire_queue;

        गच्छ valid;
    }

invalid_ip6_arpa:
#endif

    ngx_log_error(r->log_level, r->log, 0,
                  "invalid in-addr.arpa or ip6.arpa name in DNS response");
    ngx_resolver_free(r, name.data);
    पटिनिवत्त;

valid:

    सचे (rn == रिक्त || rn->query == रिक्त) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.data);
        गच्छ failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    सचे (ident != qident) {
        ngx_log_error(r->log_level, r->log, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.data);
        गच्छ failed;
    }

    ngx_resolver_free(r, name.data);

    सचे (code == 0 && nan == 0) {
        code = NGX_RESOLVE_NXDOMAIN;
    }

    सचे (code) {
        next = rn->waiting;
        rn->waiting = रिक्त;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(वृक्षः, &rn->ग्रन्थिः);

        /* unlock addr mutex */

        याव (next) {
            ctx = next;
            ctx->state = code;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            next = ctx->next;

            ctx->handler(ctx);
        }

        ngx_resolver_free_ग्रन्थिः(r, rn);

        पटिनिवत्त;
    }

    i += परिमाण(ngx_resolver_qs_t);

    अत्थाय (a = 0; a < nan; a++) {

        start = i;

        याव (i < n) {

            सचे (buf[i] & 0xc0) {
                i += 2;
                गच्छ found;
            }

            सचे (buf[i] == 0) {
                i++;
                गच्छ test_length;
            }

            i += 1 + buf[i];
        }

        गच्छ short_response;

    test_length:

        सचे (i - start < 2) {
            err = "invalid name in DNS response";
            गच्छ invalid;
        }

    found:

        सचे (i + परिमाण(ngx_resolver_an_t) >= n) {
            गच्छ short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        सचे (class != 1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR class %ui in DNS response", class);
            गच्छ failed;
        }

        सचे (ttl < 0) {
            ttl = 0;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                      "resolver qt:%ui cl:%ui len:%uz",
                      type, class, len);

        i += परिमाण(ngx_resolver_an_t);

        विकल्प (type) {

        पक्ष NGX_RESOLVE_PTR:

            गच्छ ptr;

        पक्ष NGX_RESOLVE_CNAME:

            भङ्ग;

        स्वभाव:

            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->log, 0,
                  "no PTR type in DNS response");
    पटिनिवत्त;

ptr:

    सचे (ngx_resolver_copy(r, &name, buf, buf + i, buf + n) != NGX_OK) {
        गच्छ failed;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver an:%V", &name);

    सचे (name.len != (size_t) rn->nlen
        || ngx_strncmp(name.data, rn->name, name.len) != 0)
    {
        सचे (rn->nlen) {
            ngx_resolver_free(r, rn->name);
        }

        rn->nlen = (u_short) name.len;
        rn->name = name.data;

        name.data = ngx_resolver_dup(r, rn->name, name.len);
        सचे (name.data == रिक्त) {
            गच्छ failed;
        }
    }

    ngx_queue_remove(&rn->queue);

    rn->valid = ngx_time() + (r->valid ? r->valid : ttl);
    rn->expire = ngx_time() + r->expire;

    ngx_queue_निवेशय_head(expire_queue, &rn->queue);

    next = rn->waiting;
    rn->waiting = रिक्त;

    /* unlock addr mutex */

    याव (next) {
        ctx = next;
        ctx->state = NGX_OK;
        ctx->valid = rn->valid;
        ctx->name = name;
        next = ctx->next;

        ctx->handler(ctx);
    }

    ngx_resolver_free(r, name.data);

    पटिनिवत्त;

short_response:

    err = "short DNS response";

invalid:

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->log, 0, err);

    पटिनिवत्त;

failed:

    /* unlock addr mutex */

    पटिनिवत्त;
}


स्थिर ngx_resolver_ग्रन्थिः_t *
ngx_resolver_lookup_name(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_ग्रन्थिः_t    *ग्रन्थिः, *sentinel;
    ngx_resolver_ग्रन्थिः_t  *rn;

    ग्रन्थिः = r->name_rbtree.मूलम्;
    sentinel = r->name_rbtree.sentinel;

    याव (ग्रन्थिः != sentinel) {

        सचे (hash < ग्रन्थिः->कुञ्चिका) {
            ग्रन्थिः = ग्रन्थिः->वामः;
            अनुवर्त;
        }

        सचे (hash > ग्रन्थिः->कुञ्चिका) {
            ग्रन्थिः = ग्रन्थिः->दक्षिणः;
            अनुवर्त;
        }

        /* hash == node->key */

        rn = ngx_resolver_ग्रन्थिः(ग्रन्थिः);

        rc = ngx_memn2cmp(name->data, rn->name, name->len, rn->nlen);

        सचे (rc == 0) {
            पटिनिवत्त rn;
        }

        ग्रन्थिः = (rc < 0) ? ग्रन्थिः->वामः : ग्रन्थिः->दक्षिणः;
    }

    /* not found */

    पटिनिवत्त रिक्त;
}


स्थिर ngx_resolver_ग्रन्थिः_t *
ngx_resolver_lookup_srv(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_ग्रन्थिः_t    *ग्रन्थिः, *sentinel;
    ngx_resolver_ग्रन्थिः_t  *rn;

    ग्रन्थिः = r->srv_rbtree.मूलम्;
    sentinel = r->srv_rbtree.sentinel;

    याव (ग्रन्थिः != sentinel) {

        सचे (hash < ग्रन्थिः->कुञ्चिका) {
            ग्रन्थिः = ग्रन्थिः->वामः;
            अनुवर्त;
        }

        सचे (hash > ग्रन्थिः->कुञ्चिका) {
            ग्रन्थिः = ग्रन्थिः->दक्षिणः;
            अनुवर्त;
        }

        /* hash == node->key */

        rn = ngx_resolver_ग्रन्थिः(ग्रन्थिः);

        rc = ngx_memn2cmp(name->data, rn->name, name->len, rn->nlen);

        सचे (rc == 0) {
            पटिनिवत्त rn;
        }

        ग्रन्थिः = (rc < 0) ? ग्रन्थिः->वामः : ग्रन्थिः->दक्षिणः;
    }

    /* not found */

    पटिनिवत्त रिक्त;
}


स्थिर ngx_resolver_ग्रन्थिः_t *
ngx_resolver_lookup_addr(ngx_resolver_t *r, in_addr_t addr)
{
    ngx_rbtree_ग्रन्थिः_t  *ग्रन्थिः, *sentinel;

    ग्रन्थिः = r->addr_rbtree.मूलम्;
    sentinel = r->addr_rbtree.sentinel;

    याव (ग्रन्थिः != sentinel) {

        सचे (addr < ग्रन्थिः->कुञ्चिका) {
            ग्रन्थिः = ग्रन्थिः->वामः;
            अनुवर्त;
        }

        सचे (addr > ग्रन्थिः->कुञ्चिका) {
            ग्रन्थिः = ग्रन्थिः->दक्षिणः;
            अनुवर्त;
        }

        /* addr == node->key */

        पटिनिवत्त ngx_resolver_ग्रन्थिः(ग्रन्थिः);
    }

    /* not found */

    पटिनिवत्त रिक्त;
}


#सचे (NGX_HAVE_INET6)

स्थिर ngx_resolver_ग्रन्थिः_t *
ngx_resolver_lookup_addr6(ngx_resolver_t *r, संरचना in6_addr *addr,
    uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_ग्रन्थिः_t    *ग्रन्थिः, *sentinel;
    ngx_resolver_ग्रन्थिः_t  *rn;

    ग्रन्थिः = r->addr6_rbtree.मूलम्;
    sentinel = r->addr6_rbtree.sentinel;

    याव (ग्रन्थिः != sentinel) {

        सचे (hash < ग्रन्थिः->कुञ्चिका) {
            ग्रन्थिः = ग्रन्थिः->वामः;
            अनुवर्त;
        }

        सचे (hash > ग्रन्थिः->कुञ्चिका) {
            ग्रन्थिः = ग्रन्थिः->दक्षिणः;
            अनुवर्त;
        }

        /* hash == node->key */

        rn = ngx_resolver_ग्रन्थिः(ग्रन्थिः);

        rc = ngx_memcmp(addr, &rn->addr6, 16);

        सचे (rc == 0) {
            पटिनिवत्त rn;
        }

        ग्रन्थिः = (rc < 0) ? ग्रन्थिः->वामः : ग्रन्थिः->दक्षिणः;
    }

    /* not found */

    पटिनिवत्त रिक्त;
}

#endif


स्थिर शून्य
ngx_resolver_rbtree_निवेशय_मानम्(ngx_rbtree_ग्रन्थिः_t *temp,
    ngx_rbtree_ग्रन्थिः_t *ग्रन्थिः, ngx_rbtree_ग्रन्थिः_t *sentinel)
{
    ngx_rbtree_ग्रन्थिः_t    **p;
    ngx_resolver_ग्रन्थिः_t   *rn, *rn_temp;

    अत्थाय ( ;; ) {

        सचे (ग्रन्थिः->कुञ्चिका < temp->कुञ्चिका) {

            p = &temp->वामः;

        } अञ्ञथा सचे (ग्रन्थिः->कुञ्चिका > temp->कुञ्चिका) {

            p = &temp->दक्षिणः;

        } अञ्ञथा { /* node->key == temp->key */

            rn = ngx_resolver_ग्रन्थिः(ग्रन्थिः);
            rn_temp = ngx_resolver_ग्रन्थिः(temp);

            p = (ngx_memn2cmp(rn->name, rn_temp->name, rn->nlen, rn_temp->nlen)
                 < 0) ? &temp->वामः : &temp->दक्षिणः;
        }

        सचे (*p == sentinel) {
            भङ्ग;
        }

        temp = *p;
    }

    *p = ग्रन्थिः;
    ग्रन्थिः->parent = temp;
    ग्रन्थिः->वामः = sentinel;
    ग्रन्थिः->दक्षिणः = sentinel;
    ngx_rbt_red(ग्रन्थिः);
}


#सचे (NGX_HAVE_INET6)

स्थिर शून्य
ngx_resolver_rbtree_निवेशय_addr6_मानम्(ngx_rbtree_ग्रन्थिः_t *temp,
    ngx_rbtree_ग्रन्थिः_t *ग्रन्थिः, ngx_rbtree_ग्रन्थिः_t *sentinel)
{
    ngx_rbtree_ग्रन्थिः_t    **p;
    ngx_resolver_ग्रन्थिः_t   *rn, *rn_temp;

    अत्थाय ( ;; ) {

        सचे (ग्रन्थिः->कुञ्चिका < temp->कुञ्चिका) {

            p = &temp->वामः;

        } अञ्ञथा सचे (ग्रन्थिः->कुञ्चिका > temp->कुञ्चिका) {

            p = &temp->दक्षिणः;

        } अञ्ञथा { /* node->key == temp->key */

            rn = ngx_resolver_ग्रन्थिः(ग्रन्थिः);
            rn_temp = ngx_resolver_ग्रन्थिः(temp);

            p = (ngx_memcmp(&rn->addr6, &rn_temp->addr6, 16)
                 < 0) ? &temp->वामः : &temp->दक्षिणः;
        }

        सचे (*p == sentinel) {
            भङ्ग;
        }

        temp = *p;
    }

    *p = ग्रन्थिः;
    ग्रन्थिः->parent = temp;
    ग्रन्थिः->वामः = sentinel;
    ग्रन्थिः->दक्षिणः = sentinel;
    ngx_rbt_red(ग्रन्थिः);
}

#endif


स्थिर ngx_int_t
ngx_resolver_create_name_query(ngx_resolver_t *r, ngx_resolver_ग्रन्थिः_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    size_t               len, nlen;
    ngx_uint_t           ident;
    ngx_resolver_qs_t   *qs;
    ngx_resolver_hdr_t  *query;

    nlen = name->len ? (1 + name->len + 1) : 1;

    len = परिमाण(ngx_resolver_hdr_t) + nlen + परिमाण(ngx_resolver_qs_t);

#सचे (NGX_HAVE_INET6)
    p = ngx_resolver_alloc(r, len * (r->ipv4 + r->ipv6));
#अञ्ञथा
    p = ngx_resolver_alloc(r, len);
#endif
    सचे (p == रिक्त) {
        पटिनिवत्त NGX_ERROR;
    }

    rn->qlen = (u_short) len;
    rn->query = p;

#सचे (NGX_HAVE_INET6)
    सचे (r->ipv6) {
        rn->query6 = r->ipv4 ? (p + len) : p;
    }
#endif

    query = (ngx_resolver_hdr_t *) p;

    सचे (r->ipv4) {
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

    p += परिमाण(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    /* query type */
    qs->type_hi = 0; qs->type_lo = NGX_RESOLVE_A;

    /* IN query class */
    qs->class_hi = 0; qs->class_lo = 1;

    /* convert "www.example.com" to "\3www\7example\3com\0" */

    len = 0;
    p--;
    *p-- = '\0';

    सचे (name->len == 0)  {
        पटिनिवत्त NGX_DECLINED;
    }

    अत्थाय (s = name->data + name->len - 1; s >= name->data; s--) {
        सचे (*s != '.') {
            *p = *s;
            len++;

        } अञ्ञथा {
            सचे (len == 0 || len > 255) {
                पटिनिवत्त NGX_DECLINED;
            }

            *p = (u_char) len;
            len = 0;
        }

        p--;
    }

    सचे (len == 0 || len > 255) {
        पटिनिवत्त NGX_DECLINED;
    }

    *p = (u_char) len;

#सचे (NGX_HAVE_INET6)
    सचे (!r->ipv6) {
        पटिनिवत्त NGX_OK;
    }

    p = rn->query6;

    सचे (r->ipv4) {
        ngx_memcpy(p, rn->query, rn->qlen);
    }

    query = (ngx_resolver_hdr_t *) p;

    ident = ngx_random();

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve: \"%V\" AAAA %i", name, ident & 0xffff);

    query->ident_hi = (u_char) ((ident >> 8) & 0xff);
    query->ident_lo = (u_char) (ident & 0xff);

    p += परिमाण(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    qs->type_lo = NGX_RESOLVE_AAAA;
#endif

    पटिनिवत्त NGX_OK;
}


स्थिर ngx_int_t
ngx_resolver_create_srv_query(ngx_resolver_t *r, ngx_resolver_ग्रन्थिः_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    size_t               len, nlen;
    ngx_uint_t           ident;
    ngx_resolver_qs_t   *qs;
    ngx_resolver_hdr_t  *query;

    nlen = name->len ? (1 + name->len + 1) : 1;

    len = परिमाण(ngx_resolver_hdr_t) + nlen + परिमाण(ngx_resolver_qs_t);

    p = ngx_resolver_alloc(r, len);
    सचे (p == रिक्त) {
        पटिनिवत्त NGX_ERROR;
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

    p += परिमाण(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    /* query type */
    qs->type_hi = 0; qs->type_lo = NGX_RESOLVE_SRV;

    /* IN query class */
    qs->class_hi = 0; qs->class_lo = 1;

    /* converts "www.example.com" to "\3www\7example\3com\0" */

    len = 0;
    p--;
    *p-- = '\0';

    सचे (name->len == 0)  {
        पटिनिवत्त NGX_DECLINED;
    }

    अत्थाय (s = name->data + name->len - 1; s >= name->data; s--) {
        सचे (*s != '.') {
            *p = *s;
            len++;

        } अञ्ञथा {
            सचे (len == 0 || len > 255) {
                पटिनिवत्त NGX_DECLINED;
            }

            *p = (u_char) len;
            len = 0;
        }

        p--;
    }

    सचे (len == 0 || len > 255) {
        पटिनिवत्त NGX_DECLINED;
    }

    *p = (u_char) len;

    पटिनिवत्त NGX_OK;
}


स्थिर ngx_int_t
ngx_resolver_create_addr_query(ngx_resolver_t *r, ngx_resolver_ग्रन्थिः_t *rn,
    ngx_resolver_addr_t *addr)
{
    u_char               *p, *d;
    size_t                len;
    in_addr_t             inaddr;
    ngx_int_t             n;
    ngx_uint_t            ident;
    ngx_resolver_hdr_t   *query;
    संरचना sockaddr_in   *sin;
#सचे (NGX_HAVE_INET6)
    संरचना sockaddr_in6  *sin6;
#endif

    विकल्प (addr->sockaddr->sa_family) {

#सचे (NGX_HAVE_INET6)
    पक्ष AF_INET6:
        len = परिमाण(ngx_resolver_hdr_t)
              + 64 + परिमाण(".ip6.arpa.") - 1
              + परिमाण(ngx_resolver_qs_t);

        भङ्ग;
#endif

    स्वभाव: /* AF_INET */
        len = परिमाण(ngx_resolver_hdr_t)
              + परिमाण(".255.255.255.255.in-addr.arpa.") - 1
              + परिमाण(ngx_resolver_qs_t);
    }

    p = ngx_resolver_alloc(r, len);
    सचे (p == रिक्त) {
        पटिनिवत्त NGX_ERROR;
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

    p += परिमाण(ngx_resolver_hdr_t);

    विकल्प (addr->sockaddr->sa_family) {

#सचे (NGX_HAVE_INET6)
    पक्ष AF_INET6:
        sin6 = (संरचना sockaddr_in6 *) addr->sockaddr;

        अत्थाय (n = 15; n >= 0; n--) {
            p = ngx_sprintf(p, "\1%xd\1%xd",
                            sin6->sin6_addr.s6_addr[n] & 0xf,
                            (sin6->sin6_addr.s6_addr[n] >> 4) & 0xf);
        }

        p = ngx_cpymem(p, "\3ip6\4arpa\0", 10);

        भङ्ग;
#endif

    स्वभाव: /* AF_INET */

        sin = (संरचना sockaddr_in *) addr->sockaddr;
        inaddr = ntohl(sin->sin_addr.s_addr);

        अत्थाय (n = 0; n < 32; n += 8) {
            d = ngx_sprintf(&p[1], "%ud", (inaddr >> n) & 0xff);
            *p = (u_char) (d - &p[1]);
            p = d;
        }

        p = ngx_cpymem(p, "\7in-addr\4arpa\0", 14);
    }

    /* query type "PTR", IN query class */
    p = ngx_cpymem(p, "\0\14\0\1", 4);

    rn->qlen = (u_short) (p - rn->query);

    पटिनिवत्त NGX_OK;
}


स्थिर ngx_int_t
ngx_resolver_copy(ngx_resolver_t *r, ngx_str_t *name, u_char *buf, u_char *src,
    u_char *last)
{
    वर्ण        *err;
    u_char      *p, *dst;
    size_t       len;
    ngx_uint_t   i, n;

    p = src;
    len = 0;

    /*
     * compression pointers allow to create endless loop, so we set limit;
     * 128 pointers should be enough to store 255-byte name
     */

    अत्थाय (i = 0; i < 128; i++) {
        n = *p++;

        सचे (n == 0) {
            गच्छ done;
        }

        सचे (n & 0xc0) {
            सचे ((n & 0xc0) != 0xc0) {
                err = "invalid label type in DNS response";
                गच्छ invalid;
            }

            सचे (p >= last) {
                err = "name is out of DNS response";
                गच्छ invalid;
            }

            n = ((n & 0x3f) << 8) + *p;
            p = &buf[n];

        } अञ्ञथा {
            len += 1 + n;
            p = &p[n];
        }

        सचे (p >= last) {
            err = "name is out of DNS response";
            गच्छ invalid;
        }
    }

    err = "compression pointers loop in DNS response";

invalid:

    ngx_log_error(r->log_level, r->log, 0, err);

    पटिनिवत्त NGX_ERROR;

done:

    सचे (name == रिक्त) {
        पटिनिवत्त NGX_OK;
    }

    सचे (len == 0) {
        ngx_str_null(name);
        पटिनिवत्त NGX_OK;
    }

    dst = ngx_resolver_alloc(r, len);
    सचे (dst == रिक्त) {
        पटिनिवत्त NGX_ERROR;
    }

    name->data = dst;

    अत्थाय ( ;; ) {
        n = *src++;

        सचे (n == 0) {
            name->len = dst - name->data - 1;
            पटिनिवत्त NGX_OK;
        }

        सचे (n & 0xc0) {
            n = ((n & 0x3f) << 8) + *src;
            src = &buf[n];

        } अञ्ञथा {
            ngx_strlow(dst, src, n);
            dst += n;
            src += n;
            *dst++ = '.';
        }
    }
}


स्थिर ngx_int_t
ngx_resolver_set_timeout(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx)
{
    सचे (ctx->event || ctx->timeout == 0) {
        पटिनिवत्त NGX_OK;
    }

    ctx->event = ngx_resolver_calloc(r, परिमाण(ngx_event_t));
    सचे (ctx->event == रिक्त) {
        पटिनिवत्त NGX_ERROR;
    }

    ctx->event->handler = ngx_resolver_timeout_handler;
    ctx->event->data = ctx;
    ctx->event->log = r->log;
    ctx->event->cancelable = ctx->cancelable;
    ctx->ident = -1;

    ngx_add_timer(ctx->event, ctx->timeout);

    पटिनिवत्त NGX_OK;
}


स्थिर शून्य
ngx_resolver_timeout_handler(ngx_event_t *ev)
{
    ngx_resolver_ctx_t  *ctx;

    ctx = ev->data;

    ctx->state = NGX_RESOLVE_TIMEDOUT;

    ctx->handler(ctx);
}


स्थिर शून्य
ngx_resolver_free_ग्रन्थिः(ngx_resolver_t *r, ngx_resolver_ग्रन्थिः_t *rn)
{
    ngx_uint_t  i;

    /* lock alloc mutex */

    सचे (rn->query) {
        ngx_resolver_free_locked(r, rn->query);
    }

    सचे (rn->name) {
        ngx_resolver_free_locked(r, rn->name);
    }

    सचे (rn->cnlen) {
        ngx_resolver_free_locked(r, rn->u.cname);
    }

    सचे (rn->naddrs > 1 && rn->naddrs != (u_short) -1) {
        ngx_resolver_free_locked(r, rn->u.addrs);
    }

#सचे (NGX_HAVE_INET6)
    सचे (rn->naddrs6 > 1 && rn->naddrs6 != (u_short) -1) {
        ngx_resolver_free_locked(r, rn->u6.addrs6);
    }
#endif

    सचे (rn->nsrvs) {
        अत्थाय (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
            सचे (rn->u.srvs[i].name.data) {
                ngx_resolver_free_locked(r, rn->u.srvs[i].name.data);
            }
        }

        ngx_resolver_free_locked(r, rn->u.srvs);
    }

    ngx_resolver_free_locked(r, rn);

    /* unlock alloc mutex */
}


स्थिर शून्य *
ngx_resolver_alloc(ngx_resolver_t *r, size_t size)
{
    u_char  *p;

    /* lock alloc mutex */

    p = ngx_alloc(size, r->log);

    /* unlock alloc mutex */

    पटिनिवत्त p;
}


स्थिर शून्य *
ngx_resolver_calloc(ngx_resolver_t *r, size_t size)
{
    u_char  *p;

    p = ngx_resolver_alloc(r, size);

    सचे (p) {
        ngx_memzero(p, size);
    }

    पटिनिवत्त p;
}


स्थिर शून्य
ngx_resolver_free(ngx_resolver_t *r, शून्य *p)
{
    /* lock alloc mutex */

    ngx_free(p);

    /* unlock alloc mutex */
}


स्थिर शून्य
ngx_resolver_free_locked(ngx_resolver_t *r, शून्य *p)
{
    ngx_free(p);
}


स्थिर शून्य *
ngx_resolver_dup(ngx_resolver_t *r, शून्य *src, size_t size)
{
    शून्य  *dst;

    dst = ngx_resolver_alloc(r, size);

    सचे (dst == रिक्त) {
        पटिनिवत्त dst;
    }

    ngx_memcpy(dst, src, size);

    पटिनिवत्त dst;
}


स्थिर ngx_resolver_addr_t *
ngx_resolver_export(ngx_resolver_t *r, ngx_resolver_ग्रन्थिः_t *rn,
    ngx_uint_t rotate)
{
    ngx_uint_t            d, i, j, n;
    in_addr_t            *addr;
    ngx_sockaddr_t       *sockaddr;
    संरचना sockaddr_in   *sin;
    ngx_resolver_addr_t  *dst;
#सचे (NGX_HAVE_INET6)
    संरचना in6_addr      *addr6;
    संरचना sockaddr_in6  *sin6;
#endif

    n = rn->naddrs;
#सचे (NGX_HAVE_INET6)
    n += rn->naddrs6;
#endif

    dst = ngx_resolver_calloc(r, n * परिमाण(ngx_resolver_addr_t));
    सचे (dst == रिक्त) {
        पटिनिवत्त रिक्त;
    }

    sockaddr = ngx_resolver_calloc(r, n * परिमाण(ngx_sockaddr_t));
    सचे (sockaddr == रिक्त) {
        ngx_resolver_free(r, dst);
        पटिनिवत्त रिक्त;
    }

    i = 0;
    d = rotate ? ngx_random() % n : 0;

    सचे (rn->naddrs) {
        j = rotate ? ngx_random() % rn->naddrs : 0;

        addr = (rn->naddrs == 1) ? &rn->u.addr : rn->u.addrs;

        करोहि {
            sin = &sockaddr[d].sockaddr_in;
            sin->sin_family = AF_INET;
            sin->sin_addr.s_addr = addr[j++];
            dst[d].sockaddr = (संरचना sockaddr *) sin;
            dst[d++].socklen = परिमाण(संरचना sockaddr_in);

            सचे (d == n) {
                d = 0;
            }

            सचे (j == (ngx_uint_t) rn->naddrs) {
                j = 0;
            }
        } याव (++i < (ngx_uint_t) rn->naddrs);
    }

#सचे (NGX_HAVE_INET6)
    सचे (rn->naddrs6) {
        j = rotate ? ngx_random() % rn->naddrs6 : 0;

        addr6 = (rn->naddrs6 == 1) ? &rn->u6.addr6 : rn->u6.addrs6;

        करोहि {
            sin6 = &sockaddr[d].sockaddr_in6;
            sin6->sin6_family = AF_INET6;
            ngx_memcpy(sin6->sin6_addr.s6_addr, addr6[j++].s6_addr, 16);
            dst[d].sockaddr = (संरचना sockaddr *) sin6;
            dst[d++].socklen = परिमाण(संरचना sockaddr_in6);

            सचे (d == n) {
                d = 0;
            }

            सचे (j == rn->naddrs6) {
                j = 0;
            }
        } याव (++i < n);
    }
#endif

    पटिनिवत्त dst;
}


स्थिर शून्य
ngx_resolver_report_srv(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx)
{
    ngx_uint_t                naddrs, nsrvs, nw, i, j, k, l, m, n, w;
    ngx_resolver_addr_t      *addrs;
    ngx_resolver_srv_name_t  *srvs;

    srvs = ctx->srvs;
    nsrvs = ctx->nsrvs;

    naddrs = 0;

    अत्थाय (i = 0; i < nsrvs; i++) {
        सचे (srvs[i].state == NGX_ERROR) {
            ctx->state = NGX_ERROR;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

            ctx->handler(ctx);
            पटिनिवत्त;
        }

        naddrs += srvs[i].naddrs;
    }

    सचे (naddrs == 0) {
        ctx->state = srvs[0].state;

        अत्थाय (i = 0; i < nsrvs; i++) {
            सचे (srvs[i].state == NGX_RESOLVE_NXDOMAIN) {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                भङ्ग;
            }
        }

        ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        पटिनिवत्त;
    }

    addrs = ngx_resolver_calloc(r, naddrs * परिमाण(ngx_resolver_addr_t));
    सचे (addrs == रिक्त) {
        ctx->state = NGX_ERROR;
        ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        पटिनिवत्त;
    }

    i = 0;
    n = 0;

    करोहि {
        nw = 0;

        अत्थाय (j = i; j < nsrvs; j++) {
            सचे (srvs[j].priority != srvs[i].priority) {
                भङ्ग;
            }

            nw += srvs[j].naddrs * srvs[j].weight;
        }

        सचे (nw == 0) {
            गच्छ next_srv;
        }

        w = ngx_random() % nw;

        अत्थाय (k = i; k < j; k++) {
            सचे (w < srvs[k].naddrs * srvs[k].weight) {
                भङ्ग;
            }

            w -= srvs[k].naddrs * srvs[k].weight;
        }

        अत्थाय (l = i; l < j; l++) {

            अत्थाय (m = 0; m < srvs[k].naddrs; m++) {
                addrs[n].socklen = srvs[k].addrs[m].socklen;
                addrs[n].sockaddr = srvs[k].addrs[m].sockaddr;
                addrs[n].name = srvs[k].name;
                addrs[n].priority = srvs[k].priority;
                addrs[n].weight = srvs[k].weight;
                n++;
            }

            सचे (++k == j) {
                k = i;
            }
        }

next_srv:

        i = j;

    } याव (i < ctx->nsrvs);

    ctx->state = NGX_OK;
    ctx->addrs = addrs;
    ctx->naddrs = naddrs;

    ctx->handler(ctx);

    ngx_resolver_free(r, addrs);
}


वर्ण *
ngx_resolver_strerror(ngx_int_t err)
{
    स्थिर वर्ण *errors[] = {
        "Format error",     /* FORMERR */
        "Server failure",   /* SERVFAIL */
        "Host not found",   /* NXDOMAIN */
        "Unimplemented",    /* NOTIMP */
        "Operation refused" /* REFUSED */
    };

    सचे (err > 0 && err < 6) {
        पटिनिवत्त errors[err - 1];
    }

    सचे (err == NGX_RESOLVE_TIMEDOUT) {
        पटिनिवत्त "Operation timed out";
    }

    पटिनिवत्त "Unknown error";
}


स्थिर u_char *
ngx_resolver_log_error(ngx_log_t *log, u_char *buf, size_t len)
{
    u_char                     *p;
    ngx_resolver_connection_t  *rec;

    p = buf;

    सचे (log->action) {
        p = ngx_snprintf(buf, len, " while %s", log->action);
        len -= p - buf;
    }

    rec = log->data;

    सचे (rec) {
        p = ngx_snprintf(p, len, ", resolver: %V", &rec->server);
    }

    पटिनिवत्त p;
}


स्थिर ngx_int_t
ngx_udp_connect(ngx_resolver_connection_t *rec)
{
    पूर्णाङ्क                rc;
    ngx_int_t          event;
    ngx_event_t       *rev, *wev;
    ngx_socket_t       s;
    ngx_connection_t  *c;

    s = ngx_socket(rec->sockaddr->sa_family, SOCK_DGRAM, 0);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "UDP socket %d", s);

    सचे (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_socket_n " failed");
        पटिनिवत्त NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->log);

    सचे (c == रिक्त) {
        सचे (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        पटिनिवत्त NGX_ERROR;
    }

    सचे (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        गच्छ failed;
    }

    rev = c->पठ;
    wev = c->लिख;

    rev->log = &rec->log;
    wev->log = &rec->log;

    rec->udp = c;

    c->सङ्ख्या = ngx_atomic_fetch_add(ngx_connection_counter, 1);

    c->start_time = ngx_current_msec;

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->log, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->सङ्ख्या);

    rc = connect(s, rec->sockaddr, rec->socklen);

    सचे (rc == -1) {
        ngx_log_error(NGX_LOG_CRIT, &rec->log, ngx_socket_errno,
                      "connect() failed");

        गच्छ failed;
    }

    /* UDP sockets are always ready to write */
    wev->ready = 1;

    event = (ngx_event_flags & NGX_USE_CLEAR_EVENT) ?
                /* kqueue, epoll */                 NGX_CLEAR_EVENT:
                /* select, poll, /dev/poll */       NGX_LEVEL_EVENT;
                /* eventport event type has no meaning: oneshot only */

    सचे (ngx_add_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        गच्छ failed;
    }

    पटिनिवत्त NGX_OK;

failed:

    ngx_close_connection(c);
    rec->udp = रिक्त;

    पटिनिवत्त NGX_ERROR;
}


स्थिर ngx_int_t
ngx_tcp_connect(ngx_resolver_connection_t *rec)
{
    पूर्णाङ्क                rc;
    ngx_int_t          event;
    ngx_err_t          err;
    ngx_uint_t         level;
    ngx_socket_t       s;
    ngx_event_t       *rev, *wev;
    ngx_connection_t  *c;

    s = ngx_socket(rec->sockaddr->sa_family, SOCK_STREAM, 0);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "TCP socket %d", s);

    सचे (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_socket_n " failed");
        पटिनिवत्त NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->log);

    सचे (c == रिक्त) {
        सचे (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        पटिनिवत्त NGX_ERROR;
    }

    सचे (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        गच्छ failed;
    }

    rev = c->पठ;
    wev = c->लिख;

    rev->log = &rec->log;
    wev->log = &rec->log;

    rec->tcp = c;

    c->सङ्ख्या = ngx_atomic_fetch_add(ngx_connection_counter, 1);

    c->start_time = ngx_current_msec;

    सचे (ngx_add_conn) {
        सचे (ngx_add_conn(c) == NGX_ERROR) {
            गच्छ failed;
        }
    }

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->log, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->सङ्ख्या);

    rc = connect(s, rec->sockaddr, rec->socklen);

    सचे (rc == -1) {
        err = ngx_socket_errno;


        सचे (err != NGX_EINPROGRESS
#सचे (NGX_WIN32)
            /* Winsock returns WSAEWOULDBLOCK (NGX_EAGAIN) */
            && err != NGX_EAGAIN
#endif
            )
        {
            सचे (err == NGX_ECONNREFUSED
#सचे (NGX_LINUX)
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

            } अञ्ञथा {
                level = NGX_LOG_CRIT;
            }

            ngx_log_error(level, &rec->log, err, "connect() to %V failed",
                          &rec->server);

            ngx_close_connection(c);
            rec->tcp = रिक्त;

            पटिनिवत्त NGX_ERROR;
        }
    }

    सचे (ngx_add_conn) {
        सचे (rc == -1) {

            /* NGX_EINPROGRESS */

            पटिनिवत्त NGX_AGAIN;
        }

        ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "connected");

        wev->ready = 1;

        पटिनिवत्त NGX_OK;
    }

    सचे (ngx_event_flags & NGX_USE_CLEAR_EVENT) {

        /* kqueue */

        event = NGX_CLEAR_EVENT;

    } अञ्ञथा {

        /* select, poll, /dev/poll */

        event = NGX_LEVEL_EVENT;
    }

    सचे (ngx_add_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        गच्छ failed;
    }

    सचे (rc == -1) {

        /* NGX_EINPROGRESS */

        सचे (ngx_add_event(wev, NGX_WRITE_EVENT, event) != NGX_OK) {
            गच्छ failed;
        }

        पटिनिवत्त NGX_AGAIN;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "connected");

    wev->ready = 1;

    पटिनिवत्त NGX_OK;

failed:

    ngx_close_connection(c);
    rec->tcp = रिक्त;

    पटिनिवत्त NGX_ERROR;
}


स्थिर ngx_int_t
ngx_resolver_cmp_srvs(ध्रुव शून्य *one, ध्रुव शून्य *two)
{
    ngx_int_t            p1, p2;
    ngx_resolver_srv_t  *first, *second;

    first = (ngx_resolver_srv_t *) one;
    second = (ngx_resolver_srv_t *) two;

    p1 = first->priority;
    p2 = second->priority;

    पटिनिवत्त p1 - p2;
}
