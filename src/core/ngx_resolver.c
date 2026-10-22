
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>
#अन्तर्भाव <ngx_event.h>


#परिभाषय NGX_RESOLVER_UDP_आकार   4096

#परिभाषय NGX_RESOLVER_TCP_RSIZE  (2 + 65535)
#परिभाषय NGX_RESOLVER_TCP_WSIZE  8192


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


#परिभाषय ngx_resolver_ग्रन्थिः(n)  ngx_rbtree_दत्त(n, ngx_resolver_ग्रन्थिः_t, ग्रन्थिः)


स्थिर ngx_int_t ngx_udp_connect(ngx_resolver_connection_t *rec);
स्थिर ngx_int_t ngx_tcp_connect(ngx_resolver_connection_t *rec);


स्थिर शून्य ngx_resolver_cleanup(शून्य *दत्त);
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
स्थिर ngx_uint_t ngx_resolver_resend_रिक्तानि(ngx_resolver_t *r);
स्थिर शून्य ngx_resolver_udp_read(ngx_event_t *rev);
स्थिर शून्य ngx_resolver_tcp_write(ngx_event_t *wev);
स्थिर शून्य ngx_resolver_tcp_read(ngx_event_t *rev);
स्थिर शून्य ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf,
    आकार_t n, ngx_uint_t tcp);
स्थिर शून्य ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, आकार_t n,
    ngx_uint_t ident, ngx_uint_t संहिता, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans);
स्थिर शून्य ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, आकार_t n,
    ngx_uint_t ident, ngx_uint_t संहिता, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans);
स्थिर शून्य ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, आकार_t n,
    ngx_uint_t ident, ngx_uint_t संहिता, ngx_uint_t nan);
स्थिर ngx_resolver_ग्रन्थिः_t *ngx_resolver_lookup_name(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
स्थिर ngx_resolver_ग्रन्थिः_t *ngx_resolver_lookup_srv(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
स्थिर ngx_resolver_ग्रन्थिः_t *ngx_resolver_lookup_addr(ngx_resolver_t *r,
    अन्तर्_addr_t addr);
स्थिर शून्य ngx_resolver_rbtree_निवेशय_मानम्(ngx_rbtree_ग्रन्थिः_t *अस्थायी,
    ngx_rbtree_ग्रन्थिः_t *ग्रन्थिः, ngx_rbtree_ग्रन्थिः_t *sentinel);
स्थिर ngx_int_t ngx_resolver_copy(ngx_resolver_t *r, ngx_str_t *name,
    u_char *buf, u_char *src, u_char *चरमः);
स्थिर ngx_int_t ngx_resolver_समुच्चयः_timeout(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx);
स्थिर शून्य ngx_resolver_timeout_handler(ngx_event_t *ev);
स्थिर शून्य ngx_resolver_free_ग्रन्थिः(ngx_resolver_t *r, ngx_resolver_ग्रन्थिः_t *rn);
स्थिर शून्य *ngx_resolver_alloc(ngx_resolver_t *r, आकार_t आकार);
स्थिर शून्य *ngx_resolver_calloc(ngx_resolver_t *r, आकार_t आकार);
स्थिर शून्य ngx_resolver_free(ngx_resolver_t *r, शून्य *p);
स्थिर शून्य ngx_resolver_free_locked(ngx_resolver_t *r, शून्य *p);
स्थिर शून्य *ngx_resolver_dup(ngx_resolver_t *r, शून्य *src, आकार_t आकार);
स्थिर ngx_resolver_addr_t *ngx_resolver_export(ngx_resolver_t *r,
    ngx_resolver_ग्रन्थिः_t *rn, ngx_uint_t rotate);
स्थिर शून्य ngx_resolver_report_srv(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx);
स्थिर u_char *ngx_resolver_log_error(ngx_log_t *लघुगणकः, u_char *buf, आकार_t len);
स्थिर शून्य ngx_resolver_resolve_srv_नामानि(ngx_resolver_ctx_t *ctx,
    ngx_resolver_ग्रन्थिः_t *rn);
स्थिर शून्य ngx_resolver_srv_नामानि_handler(ngx_resolver_ctx_t *ctx);
स्थिर ngx_int_t ngx_resolver_cmp_srvs(ध्रुव शून्य *one, ध्रुव शून्य *two);

#यदि (NGX_HAVE_INET6)
स्थिर शून्य ngx_resolver_rbtree_निवेशय_addr6_मानम्(ngx_rbtree_ग्रन्थिः_t *अस्थायी,
    ngx_rbtree_ग्रन्थिः_t *ग्रन्थिः, ngx_rbtree_ग्रन्थिः_t *sentinel);
स्थिर ngx_resolver_ग्रन्थिः_t *ngx_resolver_lookup_addr6(ngx_resolver_t *r,
    संरचना in6_addr *addr, uint32_t hash);
#यदि_अन्तः


ngx_resolver_t *
ngx_resolver_create(ngx_conf_t *cf, ngx_str_t *नामानि, ngx_uint_t n)
{
    ngx_str_t                   s;
    ngx_url_t                   u;
    ngx_uint_t                  i, j;
    ngx_resolver_t             *r;
    ngx_pool_cleanup_t         *cln;
    ngx_resolver_connection_t  *rec;

    r = ngx_pcalloc(cf->pool, परिमाण(ngx_resolver_t));
    यदि (r == रिक्त) {
        प्रत्यागम रिक्त;
    }

    r->event = ngx_pcalloc(cf->pool, परिमाण(ngx_event_t));
    यदि (r->event == रिक्त) {
        प्रत्यागम रिक्त;
    }

    cln = ngx_pool_cleanup_सङ्कलय(cf->pool, 0);
    यदि (cln == रिक्त) {
        प्रत्यागम रिक्त;
    }

    cln->handler = ngx_resolver_cleanup;
    cln->दत्त = r;

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

#यदि (NGX_HAVE_INET6)
    r->ipv6 = 1;

    ngx_rbtree_init(&r->addr6_rbtree, &r->addr6_sentinel,
                    ngx_resolver_rbtree_निवेशय_addr6_मानम्);

    ngx_queue_init(&r->addr6_resend_queue);

    ngx_queue_init(&r->addr6_expire_queue);
#यदि_अन्तः

    r->event->handler = ngx_resolver_resend_handler;
    r->event->दत्त = r;
    r->event->लघुगणकः = &cf->आवृत्तिः->new_log;
    r->event->cancelable = 1;
    r->ident = -1;

    r->resend_timeout = 5;
    r->tcp_timeout = 5;
    r->expire = 30;
    r->valid = 0;

    r->लघुगणकः = &cf->आवृत्तिः->new_log;
    r->log_level = NGX_LOG_ERR;

    यदि (n) {
        यदि (ngx_array_init(&r->connections, cf->pool, n,
                           परिमाण(ngx_resolver_connection_t))
            != NGX_OK)
        {
            प्रत्यागम रिक्त;
        }
    }

    प्रति (i = 0; i < n; i++) {
        यदि (ngx_strncmp(नामानि[i].दत्त, "valid=", 6) == 0) {
            s.len = नामानि[i].len - 6;
            s.दत्त = नामानि[i].दत्त + 6;

            r->valid = ngx_parse_time(&s, 1);

            यदि (r->valid == (time_t) NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &नामानि[i]);
                प्रत्यागम रिक्त;
            }

            अनुवर्त;
        }

#यदि (NGX_HAVE_INET6)
        यदि (ngx_strncmp(नामानि[i].दत्त, "ipv4=", 5) == 0) {

            यदि (ngx_strcmp(&नामानि[i].दत्त[5], "on") == 0) {
                r->ipv4 = 1;

            } अन्यथा यदि (ngx_strcmp(&नामानि[i].दत्त[5], "off") == 0) {
                r->ipv4 = 0;

            } अन्यथा {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &नामानि[i]);
                प्रत्यागम रिक्त;
            }

            अनुवर्त;
        }

        यदि (ngx_strncmp(नामानि[i].दत्त, "ipv6=", 5) == 0) {

            यदि (ngx_strcmp(&नामानि[i].दत्त[5], "on") == 0) {
                r->ipv6 = 1;

            } अन्यथा यदि (ngx_strcmp(&नामानि[i].दत्त[5], "off") == 0) {
                r->ipv6 = 0;

            } अन्यथा {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &नामानि[i]);
                प्रत्यागम रिक्त;
            }

            अनुवर्त;
        }
#यदि_अन्तः

        ngx_memzero(&u, परिमाण(ngx_url_t));

        u.url = नामानि[i];
        u.default_पत्तनम् = 53;

        यदि (ngx_parse_url(cf->pool, &u) != NGX_OK) {
            यदि (u.err) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "%s in resolver \"%V\"",
                                   u.err, &u.url);
            }

            प्रत्यागम रिक्त;
        }

        rec = ngx_array_प्रक्षिप_n(&r->connections, u.naddrs);
        यदि (rec == रिक्त) {
            प्रत्यागम रिक्त;
        }

        ngx_memzero(rec, u.naddrs * परिमाण(ngx_resolver_connection_t));

        प्रति (j = 0; j < u.naddrs; j++) {
            rec[j].sockaddr = u.addrs[j].sockaddr;
            rec[j].socklen = u.addrs[j].socklen;
            rec[j].server = u.addrs[j].name;
            rec[j].resolver = r;
        }
    }

#यदि (NGX_HAVE_INET6)
    यदि (r->ipv4 + r->ipv6 == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"ipv4\" and \"ipv6\" cannot both be \"off\"");
        प्रत्यागम रिक्त;
    }
#यदि_अन्तः

    यदि (n && r->connections.nelts == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "no name servers defined");
        प्रत्यागम रिक्त;
    }

    प्रत्यागम r;
}


स्थिर शून्य
ngx_resolver_cleanup(शून्य *दत्त)
{
    ngx_resolver_t  *r = दत्त;

    ngx_uint_t                  i;
    ngx_resolver_connection_t  *rec;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_आवृत्तिः->लघुगणकः, 0, "cleanup resolver");

    ngx_resolver_cleanup_वृक्षः(r, &r->name_rbtree);

    ngx_resolver_cleanup_वृक्षः(r, &r->srv_rbtree);

    ngx_resolver_cleanup_वृक्षः(r, &r->addr_rbtree);

#यदि (NGX_HAVE_INET6)
    ngx_resolver_cleanup_वृक्षः(r, &r->addr6_rbtree);
#यदि_अन्तः

    यदि (r->event->timer_समुच्चयः) {
        ngx_del_timer(r->event);
    }

    rec = r->connections.elts;

    प्रति (i = 0; i < r->connections.nelts; i++) {
        यदि (rec[i].udp) {
            ngx_close_connection(rec[i].udp);
        }

        यदि (rec[i].tcp) {
            ngx_close_connection(rec[i].tcp);
        }

        यदि (rec[i].read_buf) {
            ngx_resolver_free(r, rec[i].read_buf->start);
            ngx_resolver_free(r, rec[i].read_buf);
        }

        यदि (rec[i].write_buf) {
            ngx_resolver_free(r, rec[i].write_buf->start);
            ngx_resolver_free(r, rec[i].write_buf);
        }
    }
}


स्थिर शून्य
ngx_resolver_cleanup_वृक्षः(ngx_resolver_t *r, ngx_rbtree_t *वृक्षः)
{
    ngx_resolver_ctx_t   *ctx, *अग्र;
    ngx_resolver_ग्रन्थिः_t  *rn;

    यावत् (वृक्षः->मूलम् != वृक्षः->sentinel) {

        rn = ngx_resolver_ग्रन्थिः(ngx_rbtree_min(वृक्षः->मूलम्, वृक्षः->sentinel));

        ngx_queue_remove(&rn->queue);

        प्रति (ctx = rn->प्रतीक्षा; ctx; ctx = अग्र) {
            अग्र = ctx->अग्र;

            यदि (ctx->event) {
                यदि (ctx->event->timer_समुच्चयः) {
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
ngx_resolve_start(ngx_resolver_t *r, ngx_resolver_ctx_t *अस्थायी)
{
    अन्तर्_addr_t            addr;
    ngx_resolver_ctx_t  *ctx;

    यदि (अस्थायी) {
        addr = ngx_inet_addr(अस्थायी->name.दत्त, अस्थायी->name.len);

        यदि (addr != INADDR_NONE) {
            अस्थायी->resolver = r;
            अस्थायी->state = NGX_OK;
            अस्थायी->naddrs = 1;
            अस्थायी->addrs = &अस्थायी->addr;
            अस्थायी->addr.sockaddr = (संरचना sockaddr *) &अस्थायी->ज्या;
            अस्थायी->addr.socklen = परिमाण(संरचना sockaddr_अन्तर्);
            ngx_memzero(&अस्थायी->ज्या, परिमाण(संरचना sockaddr_अन्तर्));
            अस्थायी->ज्या.sin_family = AF_INET;
            अस्थायी->ज्या.sin_addr.s_addr = addr;
            अस्थायी->शीघ्र = 1;

            प्रत्यागम अस्थायी;
        }
    }

    यदि (r->connections.nelts == 0) {
        प्रत्यागम NGX_NO_RESOLVER;
    }

    ctx = ngx_resolver_calloc(r, परिमाण(ngx_resolver_ctx_t));

    यदि (ctx) {
        ctx->resolver = r;
    }

    प्रत्यागम ctx;
}


ngx_int_t
ngx_resolve_name(ngx_resolver_ctx_t *ctx)
{
    आकार_t           slen;
    ngx_int_t        rc;
    ngx_str_t        name;
    ngx_resolver_t  *r;

    r = ctx->resolver;

    यदि (ctx->name.len > 0 && ctx->name.दत्त[ctx->name.len - 1] == '.') {
        ctx->name.len--;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->लघुगणकः, 0,
                   "resolve: \"%V\"", &ctx->name);

    यदि (ctx->शीघ्र) {
        ctx->handler(ctx);
        प्रत्यागम NGX_OK;
    }

    यदि (ctx->service.len) {
        slen = ctx->service.len;

        यदि (ngx_strlchr(ctx->service.दत्त,
                        ctx->service.दत्त + ctx->service.len, '.')
            == रिक्त)
        {
            slen += परिमाण("_._tcp") - 1;
        }

        name.len = slen + 1 + ctx->name.len;

        name.दत्त = ngx_resolver_alloc(r, name.len);
        यदि (name.दत्त == रिक्त) {
            गच्छ failed;
        }

        यदि (slen == ctx->service.len) {
            ngx_sprintf(name.दत्त, "%V.%V", &ctx->service, &ctx->name);

        } अन्यथा {
            ngx_sprintf(name.दत्त, "_%V._tcp.%V", &ctx->service, &ctx->name);
        }

        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &name);

        ngx_resolver_free(r, name.दत्त);

    } अन्यथा {
        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &ctx->name);
    }

    यदि (rc == NGX_OK) {
        प्रत्यागम NGX_OK;
    }

    /* unlock name mutex */

    यदि (rc == NGX_AGAIN) {
        प्रत्यागम NGX_OK;
    }

    /* NGX_ERROR */

    यदि (ctx->event) {
        ngx_resolver_free(r, ctx->event);
    }

failed:

    ngx_resolver_free(r, ctx);

    प्रत्यागम NGX_ERROR;
}


शून्य
ngx_resolve_name_समाप्तम्(ngx_resolver_ctx_t *ctx)
{
    ngx_uint_t            i;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_ग्रन्थिः_t  *rn;

    r = ctx->resolver;

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->लघुगणकः, 0,
                   "resolve name done: %i", ctx->state);

    यदि (ctx->शीघ्र) {
        प्रत्यागम;
    }

    यदि (ctx->event && ctx->event->timer_समुच्चयः) {
        ngx_del_timer(ctx->event);
    }

    /* lock name mutex */

    यदि (ctx->nsrvs) {
        प्रति (i = 0; i < ctx->nsrvs; i++) {
            यदि (ctx->srvs[i].ctx) {
                ngx_resolve_name_समाप्तम्(ctx->srvs[i].ctx);
            }

            यदि (ctx->srvs[i].addrs) {
                ngx_resolver_free(r, ctx->srvs[i].addrs->sockaddr);
                ngx_resolver_free(r, ctx->srvs[i].addrs);
            }

            ngx_resolver_free(r, ctx->srvs[i].name.दत्त);
        }

        ngx_resolver_free(r, ctx->srvs);
    }

    यदि (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->ग्रन्थिः;

        यदि (rn) {
            p = &rn->प्रतीक्षा;
            w = rn->प्रतीक्षा;

            यावत् (w) {
                यदि (w == ctx) {
                    *p = w->अग्र;

                    गच्छ समाप्तम्;
                }

                p = &w->अग्र;
                w = w->अग्र;
            }

            ngx_log_error(NGX_LOG_ALERT, r->लघुगणकः, 0,
                          "could not cancel %V resolving", &ctx->name);
        }
    }

समाप्तम्:

    यदि (ctx->service.len) {
        ngx_resolver_expire(r, &r->srv_rbtree, &r->srv_expire_queue);

    } अन्यथा {
        ngx_resolver_expire(r, &r->name_rbtree, &r->name_expire_queue);
    }

    /* unlock name mutex */

    /* lock alloc mutex */

    यदि (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    यदि (r->event->timer_समुच्चयः && ngx_resolver_resend_रिक्तानि(r)) {
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
    ngx_resolver_ctx_t   *अग्र, *चरमः;
    ngx_resolver_addr_t  *addrs;
    ngx_resolver_ग्रन्थिः_t  *rn;

    ngx_strlow(name->दत्त, name->दत्त, name->len);

    hash = ngx_चक्रीयपरीक्षा_32_short(name->दत्त, name->len);

    यदि (ctx->service.len) {
        rn = ngx_resolver_lookup_srv(r, name, hash);

        वृक्षः = &r->srv_rbtree;
        resend_queue = &r->srv_resend_queue;
        expire_queue = &r->srv_expire_queue;

    } अन्यथा {
        rn = ngx_resolver_lookup_name(r, name, hash);

        वृक्षः = &r->name_rbtree;
        resend_queue = &r->name_resend_queue;
        expire_queue = &r->name_expire_queue;
    }

    यदि (rn) {

        /* ctx can be a list after NGX_RESOLVE_CNAME */
        प्रति (चरमः = ctx; चरमः->अग्र; चरमः = चरमः->अग्र);

        यदि (rn->valid >= ngx_time()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->लघुगणकः, 0, "resolve cached");

            ngx_queue_remove(&rn->queue);

            rn->expire = ngx_time() + r->expire;

            ngx_queue_निवेशय_शीर्ष(expire_queue, &rn->queue);

            naddrs = (rn->naddrs == (u_short) -1) ? 0 : rn->naddrs;
#यदि (NGX_HAVE_INET6)
            naddrs += (rn->naddrs6 == (u_short) -1) ? 0 : rn->naddrs6;
#यदि_अन्तः

            यदि (naddrs) {

                यदि (naddrs == 1 && rn->naddrs == 1) {
                    addrs = रिक्त;

                } अन्यथा {
                    addrs = ngx_resolver_export(r, rn, 1);
                    यदि (addrs == रिक्त) {
                        प्रत्यागम NGX_ERROR;
                    }
                }

                चरमः->अग्र = rn->प्रतीक्षा;
                rn->प्रतीक्षा = रिक्त;

                /* unlock name mutex */

                कुरु {
                    ctx->state = NGX_OK;
                    ctx->valid = rn->valid;
                    ctx->naddrs = naddrs;

                    यदि (addrs == रिक्त) {
                        ctx->addrs = &ctx->addr;
                        ctx->addr.sockaddr = (संरचना sockaddr *) &ctx->ज्या;
                        ctx->addr.socklen = परिमाण(संरचना sockaddr_अन्तर्);
                        ngx_memzero(&ctx->ज्या, परिमाण(संरचना sockaddr_अन्तर्));
                        ctx->ज्या.sin_family = AF_INET;
                        ctx->ज्या.sin_addr.s_addr = rn->u.addr;

                    } अन्यथा {
                        ctx->addrs = addrs;
                    }

                    अग्र = ctx->अग्र;

                    ctx->handler(ctx);

                    ctx = अग्र;
                } यावत् (ctx);

                यदि (addrs != रिक्त) {
                    ngx_resolver_free(r, addrs->sockaddr);
                    ngx_resolver_free(r, addrs);
                }

                प्रत्यागम NGX_OK;
            }

            यदि (rn->nsrvs) {
                चरमः->अग्र = rn->प्रतीक्षा;
                rn->प्रतीक्षा = रिक्त;

                /* unlock name mutex */

                कुरु {
                    अग्र = ctx->अग्र;

                    ngx_resolver_resolve_srv_नामानि(ctx, rn);

                    ctx = अग्र;
                } यावत् (ctx);

                प्रत्यागम NGX_OK;
            }

            /* NGX_RESOLVE_CNAME */

            यदि (ctx->recursion++ < NGX_RESOLVER_MAX_RECURSION) {

                cname.len = rn->cnlen;
                cname.दत्त = rn->u.cname;

                प्रत्यागम ngx_resolve_name_locked(r, ctx, &cname);
            }

            चरमः->अग्र = rn->प्रतीक्षा;
            rn->प्रतीक्षा = रिक्त;

            /* unlock name mutex */

            कुरु {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
                अग्र = ctx->अग्र;

                ctx->handler(ctx);

                ctx = अग्र;
            } यावत् (ctx);

            प्रत्यागम NGX_OK;
        }

        यदि (rn->प्रतीक्षा) {
            यदि (ngx_resolver_समुच्चयः_timeout(r, ctx) != NGX_OK) {
                प्रत्यागम NGX_ERROR;
            }

            चरमः->अग्र = rn->प्रतीक्षा;
            rn->प्रतीक्षा = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;

            कुरु {
                ctx->ग्रन्थिः = rn;
                ctx = ctx->अग्र;
            } यावत् (ctx);

            प्रत्यागम NGX_AGAIN;
        }

        ngx_queue_remove(&rn->queue);

        /* lock alloc mutex */

        यदि (rn->query) {
            ngx_resolver_free_locked(r, rn->query);
            rn->query = रिक्त;
#यदि (NGX_HAVE_INET6)
            rn->query6 = रिक्त;
#यदि_अन्तः
        }

        यदि (rn->cnlen) {
            ngx_resolver_free_locked(r, rn->u.cname);
        }

        यदि (rn->naddrs > 1 && rn->naddrs != (u_short) -1) {
            ngx_resolver_free_locked(r, rn->u.addrs);
        }

#यदि (NGX_HAVE_INET6)
        यदि (rn->naddrs6 > 1 && rn->naddrs6 != (u_short) -1) {
            ngx_resolver_free_locked(r, rn->u6.addrs6);
        }
#यदि_अन्तः

        यदि (rn->nsrvs) {
            प्रति (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
                यदि (rn->u.srvs[i].name.दत्त) {
                    ngx_resolver_free_locked(r, rn->u.srvs[i].name.दत्त);
                }
            }

            ngx_resolver_free_locked(r, rn->u.srvs);
        }

        /* unlock alloc mutex */

    } अन्यथा {

        rn = ngx_resolver_alloc(r, परिमाण(ngx_resolver_ग्रन्थिः_t));
        यदि (rn == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        rn->name = ngx_resolver_dup(r, name->दत्त, name->len);
        यदि (rn->name == रिक्त) {
            ngx_resolver_free(r, rn);
            प्रत्यागम NGX_ERROR;
        }

        rn->ग्रन्थिः.कुञ्चिका = hash;
        rn->nlen = (u_short) name->len;
        rn->query = रिक्त;
#यदि (NGX_HAVE_INET6)
        rn->query6 = रिक्त;
#यदि_अन्तः

        ngx_rbtree_निवेशय(वृक्षः, &rn->ग्रन्थिः);
    }

    यदि (ctx->service.len) {
        rc = ngx_resolver_create_srv_query(r, rn, name);

    } अन्यथा {
        rc = ngx_resolver_create_name_query(r, rn, name);
    }

    यदि (rc == NGX_ERROR) {
        गच्छ failed;
    }

    यदि (rc == NGX_DECLINED) {
        ngx_rbtree_delete(वृक्षः, &rn->ग्रन्थिः);

        ngx_resolver_free(r, rn->query);
        ngx_resolver_free(r, rn->name);
        ngx_resolver_free(r, rn);

        कुरु {
            ctx->state = NGX_RESOLVE_NXDOMAIN;
            अग्र = ctx->अग्र;

            ctx->handler(ctx);

            ctx = अग्र;
        } यावत् (ctx);

        प्रत्यागम NGX_OK;
    }

    rn->चरमः_connection = r->चरमः_connection++;
    यदि (r->चरमः_connection == r->connections.nelts) {
        r->चरमः_connection = 0;
    }

    rn->naddrs = r->ipv4 ? (u_short) -1 : 0;
    rn->tcp = 0;
#यदि (NGX_HAVE_INET6)
    rn->naddrs6 = r->ipv6 ? (u_short) -1 : 0;
    rn->tcp6 = 0;
#यदि_अन्तः
    rn->nsrvs = 0;

    यदि (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->चरमः_connection++;
        यदि (rn->चरमः_connection == r->connections.nelts) {
            rn->चरमः_connection = 0;
        }

        (शून्य) ngx_resolver_send_query(r, rn);
    }

    यदि (ngx_resolver_समुच्चयः_timeout(r, ctx) != NGX_OK) {
        गच्छ failed;
    }

    यदि (ngx_resolver_resend_रिक्तानि(r)) {
        ngx_सङ्कलय_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_time() + r->resend_timeout;

    ngx_queue_निवेशय_शीर्ष(resend_queue, &rn->queue);

    rn->संहिता = 0;
    rn->cnlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_मानम्;
    rn->प्रतीक्षा = ctx;

    ctx->state = NGX_AGAIN;
    ctx->async = 1;

    कुरु {
        ctx->ग्रन्थिः = rn;
        ctx = ctx->अग्र;
    } यावत् (ctx);

    प्रत्यागम NGX_AGAIN;

failed:

    ngx_rbtree_delete(वृक्षः, &rn->ग्रन्थिः);

    यदि (rn->query) {
        ngx_resolver_free(r, rn->query);
    }

    ngx_resolver_free(r, rn->name);

    ngx_resolver_free(r, rn);

    प्रत्यागम NGX_ERROR;
}


ngx_int_t
ngx_resolve_addr(ngx_resolver_ctx_t *ctx)
{
    u_char               *name;
    अन्तर्_addr_t             addr;
    ngx_queue_t          *resend_queue, *expire_queue;
    ngx_rbtree_t         *वृक्षः;
    ngx_resolver_t       *r;
    संरचना sockaddr_अन्तर्   *ज्या;
    ngx_resolver_ग्रन्थिः_t  *rn;
#यदि (NGX_HAVE_INET6)
    uint32_t              hash;
    संरचना sockaddr_in6  *sin6;
#यदि_अन्तः

#यदि (NGX_SUPPRESS_WARN)
    addr = 0;
#यदि (NGX_HAVE_INET6)
    hash = 0;
    sin6 = रिक्त;
#यदि_अन्तः
#यदि_अन्तः

    r = ctx->resolver;

    विकल्प (ctx->addr.sockaddr->sa_family) {

#यदि (NGX_HAVE_INET6)
    पक्ष AF_INET6:
        sin6 = (संरचना sockaddr_in6 *) ctx->addr.sockaddr;
        hash = ngx_चक्रीयपरीक्षा_32_short(sin6->sin6_addr.s6_addr, 16);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr6(r, &sin6->sin6_addr, hash);

        वृक्षः = &r->addr6_rbtree;
        resend_queue = &r->addr6_resend_queue;
        expire_queue = &r->addr6_expire_queue;

        भङ्ग;
#यदि_अन्तः

    स्वभाव: /* AF_INET */
        ज्या = (संरचना sockaddr_अन्तर् *) ctx->addr.sockaddr;
        addr = ntohl(ज्या->sin_addr.s_addr);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        वृक्षः = &r->addr_rbtree;
        resend_queue = &r->addr_resend_queue;
        expire_queue = &r->addr_expire_queue;
    }

    यदि (rn) {

        यदि (rn->valid >= ngx_time()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->लघुगणकः, 0, "resolve cached");

            ngx_queue_remove(&rn->queue);

            rn->expire = ngx_time() + r->expire;

            ngx_queue_निवेशय_शीर्ष(expire_queue, &rn->queue);

            name = ngx_resolver_dup(r, rn->name, rn->nlen);
            यदि (name == रिक्त) {
                ngx_resolver_free(r, ctx);
                प्रत्यागम NGX_ERROR;
            }

            ctx->name.len = rn->nlen;
            ctx->name.दत्त = name;

            /* unlock addr mutex */

            ctx->state = NGX_OK;
            ctx->valid = rn->valid;

            ctx->handler(ctx);

            ngx_resolver_free(r, name);

            प्रत्यागम NGX_OK;
        }

        यदि (rn->प्रतीक्षा) {
            यदि (ngx_resolver_समुच्चयः_timeout(r, ctx) != NGX_OK) {
                प्रत्यागम NGX_ERROR;
            }

            ctx->अग्र = rn->प्रतीक्षा;
            rn->प्रतीक्षा = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;
            ctx->ग्रन्थिः = rn;

            /* unlock addr mutex */

            प्रत्यागम NGX_OK;
        }

        ngx_queue_remove(&rn->queue);

        ngx_resolver_free(r, rn->query);
        rn->query = रिक्त;
#यदि (NGX_HAVE_INET6)
        rn->query6 = रिक्त;
#यदि_अन्तः

    } अन्यथा {
        rn = ngx_resolver_alloc(r, परिमाण(ngx_resolver_ग्रन्थिः_t));
        यदि (rn == रिक्त) {
            गच्छ failed;
        }

        विकल्प (ctx->addr.sockaddr->sa_family) {

#यदि (NGX_HAVE_INET6)
        पक्ष AF_INET6:
            rn->addr6 = sin6->sin6_addr;
            rn->ग्रन्थिः.कुञ्चिका = hash;
            भङ्ग;
#यदि_अन्तः

        स्वभाव: /* AF_INET */
            rn->ग्रन्थिः.कुञ्चिका = addr;
        }

        rn->query = रिक्त;
#यदि (NGX_HAVE_INET6)
        rn->query6 = रिक्त;
#यदि_अन्तः

        ngx_rbtree_निवेशय(वृक्षः, &rn->ग्रन्थिः);
    }

    यदि (ngx_resolver_create_addr_query(r, rn, &ctx->addr) != NGX_OK) {
        गच्छ failed;
    }

    rn->चरमः_connection = r->चरमः_connection++;
    यदि (r->चरमः_connection == r->connections.nelts) {
        r->चरमः_connection = 0;
    }

    rn->naddrs = (u_short) -1;
    rn->tcp = 0;
#यदि (NGX_HAVE_INET6)
    rn->naddrs6 = (u_short) -1;
    rn->tcp6 = 0;
#यदि_अन्तः
    rn->nsrvs = 0;

    यदि (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->चरमः_connection++;
        यदि (rn->चरमः_connection == r->connections.nelts) {
            rn->चरमः_connection = 0;
        }

        (शून्य) ngx_resolver_send_query(r, rn);
    }

    यदि (ngx_resolver_समुच्चयः_timeout(r, ctx) != NGX_OK) {
        गच्छ failed;
    }

    यदि (ngx_resolver_resend_रिक्तानि(r)) {
        ngx_सङ्कलय_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_time() + r->resend_timeout;

    ngx_queue_निवेशय_शीर्ष(resend_queue, &rn->queue);

    rn->संहिता = 0;
    rn->cnlen = 0;
    rn->name = रिक्त;
    rn->nlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_मानम्;
    rn->प्रतीक्षा = ctx;

    /* unlock addr mutex */

    ctx->state = NGX_AGAIN;
    ctx->async = 1;
    ctx->ग्रन्थिः = rn;

    प्रत्यागम NGX_OK;

failed:

    यदि (rn) {
        ngx_rbtree_delete(वृक्षः, &rn->ग्रन्थिः);

        यदि (rn->query) {
            ngx_resolver_free(r, rn->query);
        }

        ngx_resolver_free(r, rn);
    }

    /* unlock addr mutex */

    यदि (ctx->event) {
        ngx_resolver_free(r, ctx->event);
    }

    ngx_resolver_free(r, ctx);

    प्रत्यागम NGX_ERROR;
}


शून्य
ngx_resolve_addr_समाप्तम्(ngx_resolver_ctx_t *ctx)
{
    ngx_queue_t          *expire_queue;
    ngx_rbtree_t         *वृक्षः;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_ग्रन्थिः_t  *rn;

    r = ctx->resolver;

    विकल्प (ctx->addr.sockaddr->sa_family) {

#यदि (NGX_HAVE_INET6)
    पक्ष AF_INET6:
        वृक्षः = &r->addr6_rbtree;
        expire_queue = &r->addr6_expire_queue;
        भङ्ग;
#यदि_अन्तः

    स्वभाव: /* AF_INET */
        वृक्षः = &r->addr_rbtree;
        expire_queue = &r->addr_expire_queue;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->लघुगणकः, 0,
                   "resolve addr done: %i", ctx->state);

    यदि (ctx->event && ctx->event->timer_समुच्चयः) {
        ngx_del_timer(ctx->event);
    }

    /* lock addr mutex */

    यदि (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->ग्रन्थिः;

        यदि (rn) {
            p = &rn->प्रतीक्षा;
            w = rn->प्रतीक्षा;

            यावत् (w) {
                यदि (w == ctx) {
                    *p = w->अग्र;

                    गच्छ समाप्तम्;
                }

                p = &w->अग्र;
                w = w->अग्र;
            }
        }

        {
            u_char     text[NGX_SOCKADDR_STRLEN];
            ngx_str_t  addrtext;

            addrtext.दत्त = text;
            addrtext.len = ngx_sock_ntop(ctx->addr.sockaddr, ctx->addr.socklen,
                                         text, NGX_SOCKADDR_STRLEN, 0);

            ngx_log_error(NGX_LOG_ALERT, r->लघुगणकः, 0,
                          "could not cancel %V resolving", &addrtext);
        }
    }

समाप्तम्:

    ngx_resolver_expire(r, वृक्षः, expire_queue);

    /* unlock addr mutex */

    /* lock alloc mutex */

    यदि (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    यदि (r->event->timer_समुच्चयः && ngx_resolver_resend_रिक्तानि(r)) {
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

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->लघुगणकः, 0, "resolver expire");

    now = ngx_time();

    प्रति (i = 0; i < 2; i++) {
        यदि (ngx_queue_रिक्तानि(queue)) {
            प्रत्यागम;
        }

        q = ngx_queue_चरमः(queue);

        rn = ngx_queue_दत्त(q, ngx_resolver_ग्रन्थिः_t, queue);

        यदि (now <= rn->expire) {
            प्रत्यागम;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->लघुगणकः, 0,
                       "resolver expire \"%*s\"", (आकार_t) rn->nlen, rn->name);

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
    rec = &rec[rn->चरमः_connection];

    यदि (rec->लघुगणकः.handler == रिक्त) {
        rec->लघुगणकः = *r->लघुगणकः;
        rec->लघुगणकः.handler = ngx_resolver_log_error;
        rec->लघुगणकः.दत्त = rec;
        rec->लघुगणकः.action = "resolving";
    }

    यदि (rn->query && rn->naddrs == (u_short) -1) {
        rc = rn->tcp ? ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen)
                     : ngx_resolver_send_udp_query(r, rec, rn->query, rn->qlen);

        यदि (rc != NGX_OK) {
            प्रत्यागम rc;
        }
    }

#यदि (NGX_HAVE_INET6)

    यदि (rn->query6 && rn->naddrs6 == (u_short) -1) {
        rc = rn->tcp6
                    ? ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen)
                    : ngx_resolver_send_udp_query(r, rec, rn->query6, rn->qlen);

        यदि (rc != NGX_OK) {
            प्रत्यागम rc;
        }
    }

#यदि_अन्तः

    प्रत्यागम NGX_OK;
}


स्थिर ngx_int_t
ngx_resolver_send_udp_query(ngx_resolver_t *r, ngx_resolver_connection_t  *rec,
    u_char *query, u_short qlen)
{
    ssize_t  n;

    यदि (rec->udp == रिक्त) {
        यदि (ngx_udp_connect(rec) != NGX_OK) {
            प्रत्यागम NGX_ERROR;
        }

        rec->udp->दत्त = rec;
        rec->udp->पठ->handler = ngx_resolver_udp_read;
        rec->udp->पठ->resolver = 1;
    }

    n = ngx_send(rec->udp, query, qlen);

    यदि (n == NGX_ERROR) {
        गच्छ failed;
    }

    यदि ((आकार_t) n != (आकार_t) qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->लघुगणकः, 0, "send() incomplete");
        गच्छ failed;
    }

    प्रत्यागम NGX_OK;

failed:

    ngx_close_connection(rec->udp);
    rec->udp = रिक्त;

    प्रत्यागम NGX_ERROR;
}


स्थिर ngx_int_t
ngx_resolver_send_tcp_query(ngx_resolver_t *r, ngx_resolver_connection_t *rec,
    u_char *query, u_short qlen)
{
    ngx_buf_t  *b;
    ngx_int_t   rc;

    rc = NGX_OK;

    यदि (rec->tcp == रिक्त) {
        b = rec->read_buf;

        यदि (b == रिक्त) {
            b = ngx_resolver_calloc(r, परिमाण(ngx_buf_t));
            यदि (b == रिक्त) {
                प्रत्यागम NGX_ERROR;
            }

            b->start = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_RSIZE);
            यदि (b->start == रिक्त) {
                ngx_resolver_free(r, b);
                प्रत्यागम NGX_ERROR;
            }

            b->अन्तः = b->start + NGX_RESOLVER_TCP_RSIZE;

            rec->read_buf = b;
        }

        b->pos = b->start;
        b->चरमः = b->start;

        b = rec->write_buf;

        यदि (b == रिक्त) {
            b = ngx_resolver_calloc(r, परिमाण(ngx_buf_t));
            यदि (b == रिक्त) {
                प्रत्यागम NGX_ERROR;
            }

            b->start = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_WSIZE);
            यदि (b->start == रिक्त) {
                ngx_resolver_free(r, b);
                प्रत्यागम NGX_ERROR;
            }

            b->अन्तः = b->start + NGX_RESOLVER_TCP_WSIZE;

            rec->write_buf = b;
        }

        b->pos = b->start;
        b->चरमः = b->start;

        rc = ngx_tcp_connect(rec);
        यदि (rc == NGX_ERROR) {
            प्रत्यागम NGX_ERROR;
        }

        rec->tcp->दत्त = rec;
        rec->tcp->लिख->handler = ngx_resolver_tcp_write;
        rec->tcp->लिख->cancelable = 1;
        rec->tcp->पठ->handler = ngx_resolver_tcp_read;
        rec->tcp->पठ->resolver = 1;

        ngx_सङ्कलय_timer(rec->tcp->लिख, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    b = rec->write_buf;

    यदि (b->अन्तः - b->चरमः <  2 + qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->लघुगणकः, 0, "buffer overflow");
        प्रत्यागम NGX_ERROR;
    }

    *b->चरमः++ = (u_char) (qlen >> 8);
    *b->चरमः++ = (u_char) qlen;
    b->चरमः = ngx_cpymem(b->चरमः, query, qlen);

    यदि (rc == NGX_OK) {
        ngx_resolver_tcp_write(rec->tcp->लिख);
    }

    प्रत्यागम NGX_OK;
}


स्थिर शून्य
ngx_resolver_resend_handler(ngx_event_t *ev)
{
    time_t           timer, atimer, stimer, ntimer;
#यदि (NGX_HAVE_INET6)
    time_t           a6timer;
#यदि_अन्तः
    ngx_resolver_t  *r;

    r = ev->दत्त;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->लघुगणकः, 0,
                   "resolver resend handler");

    /* lock name mutex */

    ntimer = ngx_resolver_resend(r, &r->name_rbtree, &r->name_resend_queue);

    stimer = ngx_resolver_resend(r, &r->srv_rbtree, &r->srv_resend_queue);

    /* unlock name mutex */

    /* lock addr mutex */

    atimer = ngx_resolver_resend(r, &r->addr_rbtree, &r->addr_resend_queue);

    /* unlock addr mutex */

#यदि (NGX_HAVE_INET6)

    /* lock addr6 mutex */

    a6timer = ngx_resolver_resend(r, &r->addr6_rbtree, &r->addr6_resend_queue);

    /* unlock addr6 mutex */

#यदि_अन्तः

    timer = ntimer;

    यदि (timer == 0) {
        timer = atimer;

    } अन्यथा यदि (atimer) {
        timer = ngx_min(timer, atimer);
    }

    यदि (timer == 0) {
        timer = stimer;

    } अन्यथा यदि (stimer) {
        timer = ngx_min(timer, stimer);
    }

#यदि (NGX_HAVE_INET6)

    यदि (timer == 0) {
        timer = a6timer;

    } अन्यथा यदि (a6timer) {
        timer = ngx_min(timer, a6timer);
    }

#यदि_अन्तः

    यदि (timer) {
        ngx_सङ्कलय_timer(r->event, (ngx_msec_t) (timer * 1000));
    }
}


स्थिर time_t
ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *वृक्षः, ngx_queue_t *queue)
{
    time_t                now;
    ngx_queue_t          *q;
    ngx_resolver_ग्रन्थिः_t  *rn;

    now = ngx_time();

    प्रति ( ;; ) {
        यदि (ngx_queue_रिक्तानि(queue)) {
            प्रत्यागम 0;
        }

        q = ngx_queue_चरमः(queue);

        rn = ngx_queue_दत्त(q, ngx_resolver_ग्रन्थिः_t, queue);

        यदि (now < rn->expire) {
            प्रत्यागम rn->expire - now;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->लघुगणकः, 0,
                       "resolver resend \"%*s\" %p",
                       (आकार_t) rn->nlen, rn->name, rn->प्रतीक्षा);

        ngx_queue_remove(q);

        यदि (rn->प्रतीक्षा) {

            यदि (++rn->चरमः_connection == r->connections.nelts) {
                rn->चरमः_connection = 0;
            }

            (शून्य) ngx_resolver_send_query(r, rn);

            rn->expire = now + r->resend_timeout;

            ngx_queue_निवेशय_शीर्ष(queue, q);

            अनुवर्त;
        }

        ngx_rbtree_delete(वृक्षः, &rn->ग्रन्थिः);

        ngx_resolver_free_ग्रन्थिः(r, rn);
    }
}


स्थिर ngx_uint_t
ngx_resolver_resend_रिक्तानि(ngx_resolver_t *r)
{
    प्रत्यागम ngx_queue_रिक्तानि(&r->name_resend_queue)
           && ngx_queue_रिक्तानि(&r->srv_resend_queue)
#यदि (NGX_HAVE_INET6)
           && ngx_queue_रिक्तानि(&r->addr6_resend_queue)
#यदि_अन्तः
           && ngx_queue_रिक्तानि(&r->addr_resend_queue);
}


स्थिर शून्य
ngx_resolver_udp_read(ngx_event_t *rev)
{
    ssize_t                     n;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;
    u_char                      buf[NGX_RESOLVER_UDP_आकार];

    c = rev->दत्त;
    rec = c->दत्त;

    कुरु {
        n = ngx_udp_recv(c, buf, NGX_RESOLVER_UDP_आकार);

        यदि (n == NGX_AGAIN) {
            भङ्ग;
        }

        यदि (n == NGX_ERROR) {
            गच्छ failed;
        }

        ngx_resolver_process_response(rec->resolver, buf, n, 0);

    } यावत् (rev->ready);

    यदि (ngx_handle_read_event(rev, 0) != NGX_OK) {
        गच्छ failed;
    }

    प्रत्यागम;

failed:

    ngx_close_connection(rec->udp);
    rec->udp = रिक्त;
}


स्थिर शून्य
ngx_resolver_tcp_write(ngx_event_t *wev)
{
    off_t                       प्रेषितम्;
    ssize_t                     n;
    ngx_buf_t                  *b;
    ngx_resolver_t             *r;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;

    c = wev->दत्त;
    rec = c->दत्त;
    b = rec->write_buf;
    r = rec->resolver;

    यदि (wev->timedout) {
        गच्छ failed;
    }

    प्रेषितम् = c->प्रेषितम्;

    यावत् (wev->ready && b->pos < b->चरमः) {
        n = ngx_send(c, b->pos, b->चरमः - b->pos);

        यदि (n == NGX_AGAIN) {
            भङ्ग;
        }

        यदि (n == NGX_ERROR) {
            गच्छ failed;
        }

        b->pos += n;
    }

    यदि (b->pos != b->start) {
        b->चरमः = ngx_movemem(b->start, b->pos, b->चरमः - b->pos);
        b->pos = b->start;
    }

    यदि (c->प्रेषितम् != प्रेषितम्) {
        ngx_सङ्कलय_timer(wev, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    यदि (ngx_handle_write_event(wev, 0) != NGX_OK) {
        गच्छ failed;
    }

    प्रत्यागम;

failed:

    ngx_close_connection(c);
    rec->tcp = रिक्त;
}


स्थिर शून्य
ngx_resolver_tcp_read(ngx_event_t *rev)
{
    u_char                     *p;
    आकार_t                      आकार;
    ssize_t                     n;
    u_short                     qlen;
    ngx_buf_t                  *b;
    ngx_resolver_t             *r;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;

    c = rev->दत्त;
    rec = c->दत्त;
    b = rec->read_buf;
    r = rec->resolver;

    यावत् (rev->ready) {
        n = ngx_recv(c, b->चरमः, b->अन्तः - b->चरमः);

        यदि (n == NGX_AGAIN) {
            भङ्ग;
        }

        यदि (n == NGX_ERROR || n == 0) {
            गच्छ failed;
        }

        b->चरमः += n;

        प्रति ( ;; ) {
            p = b->pos;
            आकार = b->चरमः - p;

            यदि (आकार < 2) {
                भङ्ग;
            }

            qlen = (u_short) *p++ << 8;
            qlen += *p++;

            यदि (आकार < (आकार_t) (2 + qlen)) {
                भङ्ग;
            }

            ngx_resolver_process_response(r, p, qlen, 1);

            b->pos += 2 + qlen;
        }

        यदि (b->pos != b->start) {
            b->चरमः = ngx_movemem(b->start, b->pos, b->चरमः - b->pos);
            b->pos = b->start;
        }
    }

    यदि (ngx_handle_read_event(rev, 0) != NGX_OK) {
        गच्छ failed;
    }

    प्रत्यागम;

failed:

    ngx_close_connection(c);
    rec->tcp = रिक्त;
}


स्थिर शून्य
ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf, आकार_t n,
    ngx_uint_t tcp)
{
    वर्ण                 *err;
    ngx_uint_t            i, times, ident, qident, flags, संहिता, nqs, nan, trunc,
                          qtype, qclass;
#यदि (NGX_HAVE_INET6)
    ngx_uint_t            qident6;
#यदि_अन्तः
    ngx_queue_t          *q;
    ngx_resolver_qs_t    *qs;
    ngx_resolver_hdr_t   *response;
    ngx_resolver_ग्रन्थिः_t  *rn;

    यदि (n < परिमाण(ngx_resolver_hdr_t)) {
        गच्छ short_response;
    }

    response = (ngx_resolver_hdr_t *) buf;

    ident = (response->ident_hi << 8) + response->ident_lo;
    flags = (response->flags_hi << 8) + response->flags_lo;
    nqs = (response->nqs_hi << 8) + response->nqs_lo;
    nan = (response->nan_hi << 8) + response->nan_lo;
    trunc = flags & 0x0200;

    ngx_log_debug6(NGX_LOG_DEBUG_CORE, r->लघुगणकः, 0,
                   "resolver DNS response %ui fl:%04Xi %ui/%ui/%ud/%ud",
                   ident, flags, nqs, nan,
                   (response->nns_hi << 8) + response->nns_lo,
                   (response->nar_hi << 8) + response->nar_lo);

    /* response to a standard query */
    यदि ((flags & 0xf870) != 0x8000 || (trunc && tcp)) {
        ngx_log_error(r->log_level, r->लघुगणकः, 0,
                      "invalid %s DNS response %ui fl:%04Xi",
                      tcp ? "TCP" : "UDP", ident, flags);
        प्रत्यागम;
    }

    संहिता = flags & 0xf;

    यदि (संहिता == NGX_RESOLVE_FORMERR) {

        times = 0;

        प्रति (q = ngx_queue_शीर्ष(&r->name_resend_queue);
             q != ngx_queue_sentinel(&r->name_resend_queue) && times++ < 100;
             q = ngx_queue_अग्र(q))
        {
            rn = ngx_queue_दत्त(q, ngx_resolver_ग्रन्थिः_t, queue);

            यदि (rn->query) {
                qident = (rn->query[0] << 8) + rn->query[1];

                यदि (qident == ident) {
                    गच्छ dns_error_name;
                }
            }

#यदि (NGX_HAVE_INET6)
            यदि (rn->query6) {
                qident6 = (rn->query6[0] << 8) + rn->query6[1];

                यदि (qident6 == ident) {
                    गच्छ dns_error_name;
                }
            }
#यदि_अन्तः
        }

        गच्छ dns_error;
    }

    यदि (संहिता > NGX_RESOLVE_REFUSED) {
        गच्छ dns_error;
    }

    यदि (nqs != 1) {
        err = "invalid number of questions in DNS response";
        गच्छ समाप्तम्;
    }

    i = परिमाण(ngx_resolver_hdr_t);

    यावत् (i < (ngx_uint_t) n) {

        यदि (buf[i] & 0xc0) {
            err = "unexpected compression pointer in DNS response";
            गच्छ समाप्तम्;
        }

        यदि (buf[i] == '\0') {
            गच्छ प्राप्तम्;
        }

        i += 1 + buf[i];
    }

    गच्छ short_response;

प्राप्तम्:

    यदि (i++ == परिमाण(ngx_resolver_hdr_t)) {
        err = "zero-length domain name in DNS response";
        गच्छ समाप्तम्;
    }

    यदि (i + परिमाण(ngx_resolver_qs_t) + nan * (2 + परिमाण(ngx_resolver_an_t))
        > (ngx_uint_t) n)
    {
        गच्छ short_response;
    }

    qs = (ngx_resolver_qs_t *) &buf[i];

    qtype = (qs->type_hi << 8) + qs->type_lo;
    qclass = (qs->class_hi << 8) + qs->class_lo;

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->लघुगणकः, 0,
                   "resolver DNS response qt:%ui cl:%ui", qtype, qclass);

    यदि (qclass != 1) {
        ngx_log_error(r->log_level, r->लघुगणकः, 0,
                      "unknown query class %ui in DNS response", qclass);
        प्रत्यागम;
    }

    विकल्प (qtype) {

    पक्ष NGX_RESOLVE_A:
#यदि (NGX_HAVE_INET6)
    पक्ष NGX_RESOLVE_AAAA:
#यदि_अन्तः

        ngx_resolver_process_a(r, buf, n, ident, संहिता, qtype, nan, trunc,
                               i + परिमाण(ngx_resolver_qs_t));

        भङ्ग;

    पक्ष NGX_RESOLVE_SRV:

        ngx_resolver_process_srv(r, buf, n, ident, संहिता, nan, trunc,
                                 i + परिमाण(ngx_resolver_qs_t));

        भङ्ग;

    पक्ष NGX_RESOLVE_PTR:

        ngx_resolver_process_ptr(r, buf, n, ident, संहिता, nan);

        भङ्ग;

    स्वभाव:
        ngx_log_error(r->log_level, r->लघुगणकः, 0,
                      "unknown query type %ui in DNS response", qtype);
        प्रत्यागम;
    }

    प्रत्यागम;

short_response:

    err = "short DNS response";

समाप्तम्:

    ngx_log_error(r->log_level, r->लघुगणकः, 0, err);

    प्रत्यागम;

dns_error_name:

    ngx_log_error(r->log_level, r->लघुगणकः, 0,
                  "DNS error (%ui: %s), query id:%ui, name:\"%*s\"",
                  संहिता, ngx_resolver_strerror(संहिता), ident,
                  (आकार_t) rn->nlen, rn->name);
    प्रत्यागम;

dns_error:

    ngx_log_error(r->log_level, r->लघुगणकः, 0,
                  "DNS error (%ui: %s), query id:%ui",
                  संहिता, ngx_resolver_strerror(संहिता), ident);
    प्रत्यागम;
}


स्थिर शून्य
ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, आकार_t n,
    ngx_uint_t ident, ngx_uint_t संहिता, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans)
{
    वर्ण                       *err;
    u_char                     *cname;
    आकार_t                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    अन्तर्_addr_t                  *addr;
    ngx_str_t                   name;
    ngx_uint_t                  type, class, qident, naddrs, a, i, j, start;
#यदि (NGX_HAVE_INET6)
    संरचना in6_addr            *addr6;
#यदि_अन्तः
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *अग्र;
    ngx_resolver_ग्रन्थिः_t        *rn;
    ngx_resolver_addr_t        *addrs;
    ngx_resolver_connection_t  *rec;

    यदि (ngx_resolver_copy(r, &name, buf,
                          buf + परिमाण(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        प्रत्यागम;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->लघुगणकः, 0, "resolver qs:%V", &name);

    hash = ngx_चक्रीयपरीक्षा_32_short(name.दत्त, name.len);

    /* lock name mutex */

    rn = ngx_resolver_lookup_name(r, &name, hash);

    यदि (rn == रिक्त) {
        ngx_log_error(r->log_level, r->लघुगणकः, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.दत्त);
        गच्छ failed;
    }

    विकल्प (qtype) {

#यदि (NGX_HAVE_INET6)
    पक्ष NGX_RESOLVE_AAAA:

        यदि (rn->query6 == रिक्त || rn->naddrs6 != (u_short) -1) {
            ngx_log_error(r->log_level, r->लघुगणकः, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.दत्त);
            गच्छ failed;
        }

        यदि (trunc && rn->tcp6) {
            ngx_resolver_free(r, name.दत्त);
            गच्छ failed;
        }

        qident = (rn->query6[0] << 8) + rn->query6[1];

        भङ्ग;
#यदि_अन्तः

    स्वभाव: /* NGX_RESOLVE_A */

        यदि (rn->query == रिक्त || rn->naddrs != (u_short) -1) {
            ngx_log_error(r->log_level, r->लघुगणकः, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.दत्त);
            गच्छ failed;
        }

        यदि (trunc && rn->tcp) {
            ngx_resolver_free(r, name.दत्त);
            गच्छ failed;
        }

        qident = (rn->query[0] << 8) + rn->query[1];
    }

    यदि (ident != qident) {
        ngx_log_error(r->log_level, r->लघुगणकः, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.दत्त);
        गच्छ failed;
    }

    ngx_resolver_free(r, name.दत्त);

    यदि (trunc) {

        ngx_queue_remove(&rn->queue);

        यदि (rn->प्रतीक्षा == रिक्त) {
            ngx_rbtree_delete(&r->name_rbtree, &rn->ग्रन्थिः);
            ngx_resolver_free_ग्रन्थिः(r, rn);
            गच्छ अग्र;
        }

        rec = r->connections.elts;
        rec = &rec[rn->चरमः_connection];

        विकल्प (qtype) {

#यदि (NGX_HAVE_INET6)
        पक्ष NGX_RESOLVE_AAAA:

            rn->tcp6 = 1;

            (शून्य) ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen);

            भङ्ग;
#यदि_अन्तः

        स्वभाव: /* NGX_RESOLVE_A */

            rn->tcp = 1;

            (शून्य) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);
        }

        rn->expire = ngx_time() + r->resend_timeout;

        ngx_queue_निवेशय_शीर्ष(&r->name_resend_queue, &rn->queue);

        गच्छ अग्र;
    }

    यदि (संहिता == 0 && rn->संहिता) {
        संहिता = rn->संहिता;
    }

    यदि (संहिता == 0 && nan == 0) {

#यदि (NGX_HAVE_INET6)
        विकल्प (qtype) {

        पक्ष NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            यदि (rn->naddrs == (u_short) -1) {
                गच्छ अग्र;
            }

            यदि (rn->naddrs) {
                गच्छ export;
            }

            भङ्ग;

        स्वभाव: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            यदि (rn->naddrs6 == (u_short) -1) {
                गच्छ अग्र;
            }

            यदि (rn->naddrs6) {
                गच्छ export;
            }
        }
#यदि_अन्तः

        संहिता = NGX_RESOLVE_NXDOMAIN;
    }

    यदि (संहिता) {

#यदि (NGX_HAVE_INET6)
        विकल्प (qtype) {

        पक्ष NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            यदि (rn->naddrs == (u_short) -1) {
                rn->संहिता = (u_char) संहिता;
                गच्छ अग्र;
            }

            भङ्ग;

        स्वभाव: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            यदि (rn->naddrs6 == (u_short) -1) {
                rn->संहिता = (u_char) संहिता;
                गच्छ अग्र;
            }
        }
#यदि_अन्तः

        अग्र = rn->प्रतीक्षा;
        rn->प्रतीक्षा = रिक्त;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(&r->name_rbtree, &rn->ग्रन्थिः);

        /* unlock name mutex */

        यावत् (अग्र) {
            ctx = अग्र;
            ctx->state = संहिता;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            अग्र = ctx->अग्र;

            ctx->handler(ctx);
        }

        ngx_resolver_free_ग्रन्थिः(r, rn);

        प्रत्यागम;
    }

    i = ans;
    naddrs = 0;
    cname = रिक्त;

    प्रति (a = 0; a < nan; a++) {

        start = i;

        यावत् (i < n) {

            यदि (buf[i] & 0xc0) {
                i += 2;
                गच्छ प्राप्तम्;
            }

            यदि (buf[i] == 0) {
                i++;
                गच्छ test_दीर्घता;
            }

            i += 1 + buf[i];
        }

        गच्छ short_response;

    test_दीर्घता:

        यदि (i - start < 2) {
            err = "invalid name in DNS response";
            गच्छ invalid;
        }

    प्राप्तम्:

        यदि (i + परिमाण(ngx_resolver_an_t) >= n) {
            गच्छ short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        यदि (class != 1) {
            ngx_log_error(r->log_level, r->लघुगणकः, 0,
                          "unexpected RR class %ui in DNS response", class);
            गच्छ failed;
        }

        यदि (ttl < 0) {
            ttl = 0;
        }

        rn->ttl = ngx_min(rn->ttl, (uint32_t) ttl);

        i += परिमाण(ngx_resolver_an_t);

        विकल्प (type) {

        पक्ष NGX_RESOLVE_A:

            यदि (qtype != NGX_RESOLVE_A) {
                err = "unexpected A record in DNS response";
                गच्छ invalid;
            }

            यदि (len != 4) {
                err = "invalid A record in DNS response";
                गच्छ invalid;
            }

            यदि (i + 4 > n) {
                गच्छ short_response;
            }

            naddrs++;

            भङ्ग;

#यदि (NGX_HAVE_INET6)
        पक्ष NGX_RESOLVE_AAAA:

            यदि (qtype != NGX_RESOLVE_AAAA) {
                err = "unexpected AAAA record in DNS response";
                गच्छ invalid;
            }

            यदि (len != 16) {
                err = "invalid AAAA record in DNS response";
                गच्छ invalid;
            }

            यदि (i + 16 > n) {
                गच्छ short_response;
            }

            naddrs++;

            भङ्ग;
#यदि_अन्तः

        पक्ष NGX_RESOLVE_CNAME:

            cname = &buf[i];

            भङ्ग;

        पक्ष NGX_RESOLVE_DNAME:

            भङ्ग;

        स्वभाव:

            ngx_log_error(r->log_level, r->लघुगणकः, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->लघुगणकः, 0,
                   "resolver naddrs:%ui cname:%p ttl:%uD",
                   naddrs, cname, rn->ttl);

    यदि (naddrs) {

        विकल्प (qtype) {

#यदि (NGX_HAVE_INET6)
        पक्ष NGX_RESOLVE_AAAA:

            यदि (naddrs == 1) {
                addr6 = &rn->u6.addr6;
                rn->naddrs6 = 1;

            } अन्यथा {
                addr6 = ngx_resolver_alloc(r, naddrs * परिमाण(संरचना in6_addr));
                यदि (addr6 == रिक्त) {
                    गच्छ failed;
                }

                rn->u6.addrs6 = addr6;
                rn->naddrs6 = (u_short) naddrs;
            }

#यदि (NGX_SUPPRESS_WARN)
            addr = रिक्त;
#यदि_अन्तः

            भङ्ग;
#यदि_अन्तः

        स्वभाव: /* NGX_RESOLVE_A */

            यदि (naddrs == 1) {
                addr = &rn->u.addr;
                rn->naddrs = 1;

            } अन्यथा {
                addr = ngx_resolver_alloc(r, naddrs * परिमाण(अन्तर्_addr_t));
                यदि (addr == रिक्त) {
                    गच्छ failed;
                }

                rn->u.addrs = addr;
                rn->naddrs = (u_short) naddrs;
            }

#यदि (NGX_HAVE_INET6 && NGX_SUPPRESS_WARN)
            addr6 = रिक्त;
#यदि_अन्तः
        }

        j = 0;
        i = ans;

        प्रति (a = 0; a < nan; a++) {

            प्रति ( ;; ) {

                यदि (buf[i] & 0xc0) {
                    i += 2;
                    भङ्ग;
                }

                यदि (buf[i] == 0) {
                    i++;
                    भङ्ग;
                }

                i += 1 + buf[i];
            }

            an = (ngx_resolver_an_t *) &buf[i];

            type = (an->type_hi << 8) + an->type_lo;
            len = (an->len_hi << 8) + an->len_lo;

            i += परिमाण(ngx_resolver_an_t);

            यदि (type == NGX_RESOLVE_A) {

                addr[j] = htonl((buf[i] << 24) + (buf[i + 1] << 16)
                                + (buf[i + 2] << 8) + (buf[i + 3]));

                यदि (++j == naddrs) {

#यदि (NGX_HAVE_INET6)
                    यदि (rn->naddrs6 == (u_short) -1) {
                        गच्छ अग्र;
                    }
#यदि_अन्तः

                    भङ्ग;
                }
            }

#यदि (NGX_HAVE_INET6)
            अन्यथा यदि (type == NGX_RESOLVE_AAAA) {

                ngx_memcpy(addr6[j].s6_addr, &buf[i], 16);

                यदि (++j == naddrs) {

                    यदि (rn->naddrs == (u_short) -1) {
                        गच्छ अग्र;
                    }

                    भङ्ग;
                }
            }
#यदि_अन्तः

            i += len;
        }
    }

    विकल्प (qtype) {

#यदि (NGX_HAVE_INET6)
    पक्ष NGX_RESOLVE_AAAA:

        यदि (rn->naddrs6 == (u_short) -1) {
            rn->naddrs6 = 0;
        }

        भङ्ग;
#यदि_अन्तः

    स्वभाव: /* NGX_RESOLVE_A */

        यदि (rn->naddrs == (u_short) -1) {
            rn->naddrs = 0;
        }
    }

    यदि (rn->naddrs != (u_short) -1
#यदि (NGX_HAVE_INET6)
        && rn->naddrs6 != (u_short) -1
#यदि_अन्तः
        && rn->naddrs
#यदि (NGX_HAVE_INET6)
           + rn->naddrs6
#यदि_अन्तः
           > 0)
    {

#यदि (NGX_HAVE_INET6)
    export:
#यदि_अन्तः

        naddrs = rn->naddrs;
#यदि (NGX_HAVE_INET6)
        naddrs += rn->naddrs6;
#यदि_अन्तः

        यदि (naddrs == 1 && rn->naddrs == 1) {
            addrs = रिक्त;

        } अन्यथा {
            addrs = ngx_resolver_export(r, rn, 0);
            यदि (addrs == रिक्त) {
                गच्छ failed;
            }
        }

        ngx_queue_remove(&rn->queue);

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_निवेशय_शीर्ष(&r->name_expire_queue, &rn->queue);

        अग्र = rn->प्रतीक्षा;
        rn->प्रतीक्षा = रिक्त;

        /* unlock name mutex */

        यावत् (अग्र) {
            ctx = अग्र;
            ctx->state = NGX_OK;
            ctx->valid = rn->valid;
            ctx->naddrs = naddrs;

            यदि (addrs == रिक्त) {
                ctx->addrs = &ctx->addr;
                ctx->addr.sockaddr = (संरचना sockaddr *) &ctx->ज्या;
                ctx->addr.socklen = परिमाण(संरचना sockaddr_अन्तर्);
                ngx_memzero(&ctx->ज्या, परिमाण(संरचना sockaddr_अन्तर्));
                ctx->ज्या.sin_family = AF_INET;
                ctx->ज्या.sin_addr.s_addr = rn->u.addr;

            } अन्यथा {
                ctx->addrs = addrs;
            }

            अग्र = ctx->अग्र;

            ctx->handler(ctx);
        }

        यदि (addrs != रिक्त) {
            ngx_resolver_free(r, addrs->sockaddr);
            ngx_resolver_free(r, addrs);
        }

        ngx_resolver_free(r, rn->query);
        rn->query = रिक्त;
#यदि (NGX_HAVE_INET6)
        rn->query6 = रिक्त;
#यदि_अन्तः

        प्रत्यागम;
    }

    यदि (cname) {

        /* CNAME only */

        यदि (rn->naddrs == (u_short) -1
#यदि (NGX_HAVE_INET6)
            || rn->naddrs6 == (u_short) -1
#यदि_अन्तः
            )
        {
            गच्छ अग्र;
        }

        यदि (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            गच्छ failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->लघुगणकः, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_queue_remove(&rn->queue);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.दत्त;

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_निवेशय_शीर्ष(&r->name_expire_queue, &rn->queue);

        ngx_resolver_free(r, rn->query);
        rn->query = रिक्त;
#यदि (NGX_HAVE_INET6)
        rn->query6 = रिक्त;
#यदि_अन्तः

        ctx = rn->प्रतीक्षा;
        rn->प्रतीक्षा = रिक्त;

        यदि (ctx) {

            यदि (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                कुरु {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    अग्र = ctx->अग्र;

                    ctx->handler(ctx);

                    ctx = अग्र;
                } यावत् (ctx);

                प्रत्यागम;
            }

            प्रति (अग्र = ctx; अग्र; अग्र = अग्र->अग्र) {
                अग्र->ग्रन्थिः = रिक्त;
            }

            (शून्य) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        प्रत्यागम;
    }

    ngx_log_error(r->log_level, r->लघुगणकः, 0,
                  "no A or CNAME types in DNS response");
    प्रत्यागम;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->लघुगणकः, 0, err);

    प्रत्यागम;

failed:

अग्र:

    /* unlock name mutex */

    प्रत्यागम;
}


स्थिर शून्य
ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, आकार_t n,
    ngx_uint_t ident, ngx_uint_t संहिता, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans)
{
    वर्ण                       *err;
    u_char                     *cname;
    आकार_t                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    ngx_str_t                   name;
    ngx_uint_t                  type, qident, class, start, nsrvs, a, i, j;
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *अग्र;
    ngx_resolver_srv_t         *srvs;
    ngx_resolver_ग्रन्थिः_t        *rn;
    ngx_resolver_connection_t  *rec;

    यदि (ngx_resolver_copy(r, &name, buf,
                          buf + परिमाण(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        प्रत्यागम;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->लघुगणकः, 0, "resolver qs:%V", &name);

    hash = ngx_चक्रीयपरीक्षा_32_short(name.दत्त, name.len);

    rn = ngx_resolver_lookup_srv(r, &name, hash);

    यदि (rn == रिक्त || rn->query == रिक्त) {
        ngx_log_error(r->log_level, r->लघुगणकः, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.दत्त);
        गच्छ failed;
    }

    यदि (trunc && rn->tcp) {
        ngx_resolver_free(r, name.दत्त);
        गच्छ failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    यदि (ident != qident) {
        ngx_log_error(r->log_level, r->लघुगणकः, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.दत्त);
        गच्छ failed;
    }

    ngx_resolver_free(r, name.दत्त);

    यदि (trunc) {

        ngx_queue_remove(&rn->queue);

        यदि (rn->प्रतीक्षा == रिक्त) {
            ngx_rbtree_delete(&r->srv_rbtree, &rn->ग्रन्थिः);
            ngx_resolver_free_ग्रन्थिः(r, rn);
            प्रत्यागम;
        }

        rec = r->connections.elts;
        rec = &rec[rn->चरमः_connection];

        rn->tcp = 1;

        (शून्य) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);

        rn->expire = ngx_time() + r->resend_timeout;

        ngx_queue_निवेशय_शीर्ष(&r->srv_resend_queue, &rn->queue);

        प्रत्यागम;
    }

    यदि (संहिता == 0 && rn->संहिता) {
        संहिता = rn->संहिता;
    }

    यदि (संहिता == 0 && nan == 0) {
        संहिता = NGX_RESOLVE_NXDOMAIN;
    }

    यदि (संहिता) {
        अग्र = rn->प्रतीक्षा;
        rn->प्रतीक्षा = रिक्त;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(&r->srv_rbtree, &rn->ग्रन्थिः);

        यावत् (अग्र) {
            ctx = अग्र;
            ctx->state = संहिता;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            अग्र = ctx->अग्र;

            ctx->handler(ctx);
        }

        ngx_resolver_free_ग्रन्थिः(r, rn);

        प्रत्यागम;
    }

    i = ans;
    nsrvs = 0;
    cname = रिक्त;

    प्रति (a = 0; a < nan; a++) {

        start = i;

        यावत् (i < n) {

            यदि (buf[i] & 0xc0) {
                i += 2;
                गच्छ प्राप्तम्;
            }

            यदि (buf[i] == 0) {
                i++;
                गच्छ test_दीर्घता;
            }

            i += 1 + buf[i];
        }

        गच्छ short_response;

    test_दीर्घता:

        यदि (i - start < 2) {
            err = "invalid name DNS response";
            गच्छ invalid;
        }

    प्राप्तम्:

        यदि (i + परिमाण(ngx_resolver_an_t) >= n) {
            गच्छ short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        यदि (class != 1) {
            ngx_log_error(r->log_level, r->लघुगणकः, 0,
                          "unexpected RR class %ui in DNS response", class);
            गच्छ failed;
        }

        यदि (ttl < 0) {
            ttl = 0;
        }

        rn->ttl = ngx_min(rn->ttl, (uint32_t) ttl);

        i += परिमाण(ngx_resolver_an_t);

        विकल्प (type) {

        पक्ष NGX_RESOLVE_SRV:

            यदि (i + 6 >= n) {
                गच्छ short_response;
            }

            यदि (ngx_resolver_copy(r, रिक्त, buf, &buf[i + 6], buf + n)
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

            ngx_log_error(r->log_level, r->लघुगणकः, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->लघुगणकः, 0,
                   "resolver nsrvs:%ui cname:%p ttl:%uD",
                   nsrvs, cname, rn->ttl);

    यदि (nsrvs) {

        srvs = ngx_resolver_calloc(r, nsrvs * परिमाण(ngx_resolver_srv_t));
        यदि (srvs == रिक्त) {
            गच्छ failed;
        }

        rn->u.srvs = srvs;
        rn->nsrvs = (u_short) nsrvs;

        j = 0;
        i = ans;

        प्रति (a = 0; a < nan; a++) {

            प्रति ( ;; ) {

                यदि (buf[i] & 0xc0) {
                    i += 2;
                    भङ्ग;
                }

                यदि (buf[i] == 0) {
                    i++;
                    भङ्ग;
                }

                i += 1 + buf[i];
            }

            an = (ngx_resolver_an_t *) &buf[i];

            type = (an->type_hi << 8) + an->type_lo;
            len = (an->len_hi << 8) + an->len_lo;

            i += परिमाण(ngx_resolver_an_t);

            यदि (type == NGX_RESOLVE_SRV) {

                srvs[j].प्राथमिकता = (buf[i] << 8) + buf[i + 1];
                srvs[j].weight = (buf[i + 2] << 8) + buf[i + 3];

                यदि (srvs[j].weight == 0) {
                    srvs[j].weight = 1;
                }

                srvs[j].पत्तनम् = (buf[i + 4] << 8) + buf[i + 5];

                यदि (ngx_resolver_copy(r, &srvs[j].name, buf, &buf[i + 6],
                                      buf + n)
                    != NGX_OK)
                {
                    गच्छ failed;
                }

                j++;
            }

            i += len;
        }

        ngx_क्रमय(srvs, nsrvs, परिमाण(ngx_resolver_srv_t),
                 ngx_resolver_cmp_srvs);

        ngx_resolver_free(r, rn->query);
        rn->query = रिक्त;

        ngx_queue_remove(&rn->queue);

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_निवेशय_शीर्ष(&r->srv_expire_queue, &rn->queue);

        अग्र = rn->प्रतीक्षा;
        rn->प्रतीक्षा = रिक्त;

        यावत् (अग्र) {
            ctx = अग्र;
            अग्र = ctx->अग्र;

            ngx_resolver_resolve_srv_नामानि(ctx, rn);
        }

        प्रत्यागम;
    }

    rn->nsrvs = 0;

    यदि (cname) {

        /* CNAME only */

        यदि (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            गच्छ failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->लघुगणकः, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_queue_remove(&rn->queue);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.दत्त;

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_निवेशय_शीर्ष(&r->srv_expire_queue, &rn->queue);

        ngx_resolver_free(r, rn->query);
        rn->query = रिक्त;
#यदि (NGX_HAVE_INET6)
        rn->query6 = रिक्त;
#यदि_अन्तः

        ctx = rn->प्रतीक्षा;
        rn->प्रतीक्षा = रिक्त;

        यदि (ctx) {

            यदि (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                कुरु {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    अग्र = ctx->अग्र;

                    ctx->handler(ctx);

                    ctx = अग्र;
                } यावत् (ctx);

                प्रत्यागम;
            }

            प्रति (अग्र = ctx; अग्र; अग्र = अग्र->अग्र) {
                अग्र->ग्रन्थिः = रिक्त;
            }

            (शून्य) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        प्रत्यागम;
    }

    ngx_log_error(r->log_level, r->लघुगणकः, 0, "no SRV type in DNS response");

    प्रत्यागम;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->लघुगणकः, 0, err);

    प्रत्यागम;

failed:

    /* unlock name mutex */

    प्रत्यागम;
}


स्थिर शून्य
ngx_resolver_resolve_srv_नामानि(ngx_resolver_ctx_t *ctx, ngx_resolver_ग्रन्थिः_t *rn)
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
    यदि (srvs == रिक्त) {
        गच्छ failed;
    }

    ctx->srvs = srvs;
    ctx->nsrvs = rn->nsrvs;

    यदि (ctx->event && ctx->event->timer_समुच्चयः) {
        ngx_del_timer(ctx->event);
    }

    प्रति (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
        srvs[i].name.दत्त = ngx_resolver_alloc(r, rn->u.srvs[i].name.len);
        यदि (srvs[i].name.दत्त == रिक्त) {
            गच्छ failed;
        }

        srvs[i].name.len = rn->u.srvs[i].name.len;
        ngx_memcpy(srvs[i].name.दत्त, rn->u.srvs[i].name.दत्त,
                   srvs[i].name.len);

        cctx = ngx_resolve_start(r, रिक्त);
        यदि (cctx == रिक्त) {
            गच्छ failed;
        }

        cctx->name = srvs[i].name;
        cctx->handler = ngx_resolver_srv_नामानि_handler;
        cctx->दत्त = ctx;
        cctx->srvs = &srvs[i];
        cctx->timeout = ctx->timeout;

        srvs[i].प्राथमिकता = rn->u.srvs[i].प्राथमिकता;
        srvs[i].weight = rn->u.srvs[i].weight;
        srvs[i].पत्तनम् = rn->u.srvs[i].पत्तनम्;
        srvs[i].ctx = cctx;

        यदि (ngx_resolve_name(cctx) == NGX_ERROR) {
            srvs[i].ctx = रिक्त;
            गच्छ failed;
        }
    }

    प्रत्यागम;

failed:

    ctx->state = NGX_ERROR;
    ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

    ctx->handler(ctx);
}


स्थिर शून्य
ngx_resolver_srv_नामानि_handler(ngx_resolver_ctx_t *cctx)
{
    ngx_uint_t                i;
    ngx_addr_t               *addrs;
    ngx_resolver_t           *r;
    ngx_sockaddr_t           *sockaddr;
    ngx_resolver_ctx_t       *ctx;
    ngx_resolver_srv_name_t  *srv;

    r = cctx->resolver;
    ctx = cctx->दत्त;
    srv = cctx->srvs;

    ctx->गणनम्--;
    ctx->async |= cctx->async;

    srv->ctx = रिक्त;
    srv->state = cctx->state;

    यदि (cctx->naddrs) {

        ctx->valid = ngx_min(ctx->valid, cctx->valid);

        addrs = ngx_resolver_calloc(r, cctx->naddrs * परिमाण(ngx_addr_t));
        यदि (addrs == रिक्त) {
            srv->state = NGX_ERROR;
            गच्छ समाप्तम्;
        }

        sockaddr = ngx_resolver_alloc(r, cctx->naddrs * परिमाण(ngx_sockaddr_t));
        यदि (sockaddr == रिक्त) {
            ngx_resolver_free(r, addrs);
            srv->state = NGX_ERROR;
            गच्छ समाप्तम्;
        }

        प्रति (i = 0; i < cctx->naddrs; i++) {
            addrs[i].sockaddr = &sockaddr[i].sockaddr;
            addrs[i].socklen = cctx->addrs[i].socklen;

            ngx_memcpy(&sockaddr[i], cctx->addrs[i].sockaddr,
                       addrs[i].socklen);

            ngx_inet_समुच्चयः_पत्तनम्(addrs[i].sockaddr, srv->पत्तनम्);
        }

        srv->addrs = addrs;
        srv->naddrs = cctx->naddrs;
    }

समाप्तम्:

    ngx_resolve_name_समाप्तम्(cctx);

    यदि (ctx->गणनम् == 0) {
        ngx_resolver_report_srv(r, ctx);
    }
}


स्थिर शून्य
ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, आकार_t n,
    ngx_uint_t ident, ngx_uint_t संहिता, ngx_uint_t nan)
{
    वर्ण                 *err;
    आकार_t                len;
    अन्तर्_addr_t             addr;
    int32_t               ttl;
    ngx_int_t             octet;
    ngx_str_t             name;
    ngx_uint_t            आवरणम्, type, class, qident, a, i, start;
    ngx_queue_t          *expire_queue;
    ngx_rbtree_t         *वृक्षः;
    ngx_resolver_an_t    *an;
    ngx_resolver_ctx_t   *ctx, *अग्र;
    ngx_resolver_ग्रन्थिः_t  *rn;
#यदि (NGX_HAVE_INET6)
    uint32_t              hash;
    ngx_int_t             digit;
    संरचना in6_addr       addr6;
#यदि_अन्तः

    यदि (ngx_resolver_copy(r, &name, buf,
                          buf + परिमाण(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        प्रत्यागम;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->लघुगणकः, 0, "resolver qs:%V", &name);

    /* AF_INET */

    addr = 0;
    i = परिमाण(ngx_resolver_hdr_t);

    प्रति (आवरणम् = 0; आवरणम् < 32; आवरणम् += 8) {
        len = buf[i++];

        octet = ngx_atoi(&buf[i], len);
        यदि (octet == NGX_ERROR || octet > 255) {
            गच्छ invalid_अन्तर्_addr_arpa;
        }

        addr += octet << आवरणम्;
        i += len;
    }

    यदि (ngx_strcasecmp(&buf[i], (u_char *) "\7in-addr\4arpa") == 0) {
        i += परिमाण("\7in-addr\4arpa");

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        वृक्षः = &r->addr_rbtree;
        expire_queue = &r->addr_expire_queue;

        गच्छ valid;
    }

invalid_अन्तर्_addr_arpa:

#यदि (NGX_HAVE_INET6)

    i = परिमाण(ngx_resolver_hdr_t);

    प्रति (octet = 15; octet >= 0; octet--) {
        यदि (buf[i++] != '\1') {
            गच्छ invalid_ip6_arpa;
        }

        digit = ngx_hextoi(&buf[i++], 1);
        यदि (digit == NGX_ERROR) {
            गच्छ invalid_ip6_arpa;
        }

        addr6.s6_addr[octet] = (u_char) digit;

        यदि (buf[i++] != '\1') {
            गच्छ invalid_ip6_arpa;
        }

        digit = ngx_hextoi(&buf[i++], 1);
        यदि (digit == NGX_ERROR) {
            गच्छ invalid_ip6_arpa;
        }

        addr6.s6_addr[octet] += (u_char) (digit * 16);
    }

    यदि (ngx_strcasecmp(&buf[i], (u_char *) "\3ip6\4arpa") == 0) {
        i += परिमाण("\3ip6\4arpa");

        /* lock addr mutex */

        hash = ngx_चक्रीयपरीक्षा_32_short(addr6.s6_addr, 16);
        rn = ngx_resolver_lookup_addr6(r, &addr6, hash);

        वृक्षः = &r->addr6_rbtree;
        expire_queue = &r->addr6_expire_queue;

        गच्छ valid;
    }

invalid_ip6_arpa:
#यदि_अन्तः

    ngx_log_error(r->log_level, r->लघुगणकः, 0,
                  "invalid in-addr.arpa or ip6.arpa name in DNS response");
    ngx_resolver_free(r, name.दत्त);
    प्रत्यागम;

valid:

    यदि (rn == रिक्त || rn->query == रिक्त) {
        ngx_log_error(r->log_level, r->लघुगणकः, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.दत्त);
        गच्छ failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    यदि (ident != qident) {
        ngx_log_error(r->log_level, r->लघुगणकः, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.दत्त);
        गच्छ failed;
    }

    ngx_resolver_free(r, name.दत्त);

    यदि (संहिता == 0 && nan == 0) {
        संहिता = NGX_RESOLVE_NXDOMAIN;
    }

    यदि (संहिता) {
        अग्र = rn->प्रतीक्षा;
        rn->प्रतीक्षा = रिक्त;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(वृक्षः, &rn->ग्रन्थिः);

        /* unlock addr mutex */

        यावत् (अग्र) {
            ctx = अग्र;
            ctx->state = संहिता;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            अग्र = ctx->अग्र;

            ctx->handler(ctx);
        }

        ngx_resolver_free_ग्रन्थिः(r, rn);

        प्रत्यागम;
    }

    i += परिमाण(ngx_resolver_qs_t);

    प्रति (a = 0; a < nan; a++) {

        start = i;

        यावत् (i < n) {

            यदि (buf[i] & 0xc0) {
                i += 2;
                गच्छ प्राप्तम्;
            }

            यदि (buf[i] == 0) {
                i++;
                गच्छ test_दीर्घता;
            }

            i += 1 + buf[i];
        }

        गच्छ short_response;

    test_दीर्घता:

        यदि (i - start < 2) {
            err = "invalid name in DNS response";
            गच्छ invalid;
        }

    प्राप्तम्:

        यदि (i + परिमाण(ngx_resolver_an_t) >= n) {
            गच्छ short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        यदि (class != 1) {
            ngx_log_error(r->log_level, r->लघुगणकः, 0,
                          "unexpected RR class %ui in DNS response", class);
            गच्छ failed;
        }

        यदि (ttl < 0) {
            ttl = 0;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->लघुगणकः, 0,
                      "resolver qt:%ui cl:%ui len:%uz",
                      type, class, len);

        i += परिमाण(ngx_resolver_an_t);

        विकल्प (type) {

        पक्ष NGX_RESOLVE_PTR:

            गच्छ ptr;

        पक्ष NGX_RESOLVE_CNAME:

            भङ्ग;

        स्वभाव:

            ngx_log_error(r->log_level, r->लघुगणकः, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->लघुगणकः, 0,
                  "no PTR type in DNS response");
    प्रत्यागम;

ptr:

    यदि (ngx_resolver_copy(r, &name, buf, buf + i, buf + n) != NGX_OK) {
        गच्छ failed;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->लघुगणकः, 0, "resolver an:%V", &name);

    यदि (name.len != (आकार_t) rn->nlen
        || ngx_strncmp(name.दत्त, rn->name, name.len) != 0)
    {
        यदि (rn->nlen) {
            ngx_resolver_free(r, rn->name);
        }

        rn->nlen = (u_short) name.len;
        rn->name = name.दत्त;

        name.दत्त = ngx_resolver_dup(r, rn->name, name.len);
        यदि (name.दत्त == रिक्त) {
            गच्छ failed;
        }
    }

    ngx_queue_remove(&rn->queue);

    rn->valid = ngx_time() + (r->valid ? r->valid : ttl);
    rn->expire = ngx_time() + r->expire;

    ngx_queue_निवेशय_शीर्ष(expire_queue, &rn->queue);

    अग्र = rn->प्रतीक्षा;
    rn->प्रतीक्षा = रिक्त;

    /* unlock addr mutex */

    यावत् (अग्र) {
        ctx = अग्र;
        ctx->state = NGX_OK;
        ctx->valid = rn->valid;
        ctx->name = name;
        अग्र = ctx->अग्र;

        ctx->handler(ctx);
    }

    ngx_resolver_free(r, name.दत्त);

    प्रत्यागम;

short_response:

    err = "short DNS response";

invalid:

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->लघुगणकः, 0, err);

    प्रत्यागम;

failed:

    /* unlock addr mutex */

    प्रत्यागम;
}


स्थिर ngx_resolver_ग्रन्थिः_t *
ngx_resolver_lookup_name(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_ग्रन्थिः_t    *ग्रन्थिः, *sentinel;
    ngx_resolver_ग्रन्थिः_t  *rn;

    ग्रन्थिः = r->name_rbtree.मूलम्;
    sentinel = r->name_rbtree.sentinel;

    यावत् (ग्रन्थिः != sentinel) {

        यदि (hash < ग्रन्थिः->कुञ्चिका) {
            ग्रन्थिः = ग्रन्थिः->वामः;
            अनुवर्त;
        }

        यदि (hash > ग्रन्थिः->कुञ्चिका) {
            ग्रन्थिः = ग्रन्थिः->दक्षिणः;
            अनुवर्त;
        }

        /* hash == node->key */

        rn = ngx_resolver_ग्रन्थिः(ग्रन्थिः);

        rc = ngx_memn2cmp(name->दत्त, rn->name, name->len, rn->nlen);

        यदि (rc == 0) {
            प्रत्यागम rn;
        }

        ग्रन्थिः = (rc < 0) ? ग्रन्थिः->वामः : ग्रन्थिः->दक्षिणः;
    }

    /* not found */

    प्रत्यागम रिक्त;
}


स्थिर ngx_resolver_ग्रन्थिः_t *
ngx_resolver_lookup_srv(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_ग्रन्थिः_t    *ग्रन्थिः, *sentinel;
    ngx_resolver_ग्रन्थिः_t  *rn;

    ग्रन्थिः = r->srv_rbtree.मूलम्;
    sentinel = r->srv_rbtree.sentinel;

    यावत् (ग्रन्थिः != sentinel) {

        यदि (hash < ग्रन्थिः->कुञ्चिका) {
            ग्रन्थिः = ग्रन्थिः->वामः;
            अनुवर्त;
        }

        यदि (hash > ग्रन्थिः->कुञ्चिका) {
            ग्रन्थिः = ग्रन्थिः->दक्षिणः;
            अनुवर्त;
        }

        /* hash == node->key */

        rn = ngx_resolver_ग्रन्थिः(ग्रन्थिः);

        rc = ngx_memn2cmp(name->दत्त, rn->name, name->len, rn->nlen);

        यदि (rc == 0) {
            प्रत्यागम rn;
        }

        ग्रन्थिः = (rc < 0) ? ग्रन्थिः->वामः : ग्रन्थिः->दक्षिणः;
    }

    /* not found */

    प्रत्यागम रिक्त;
}


स्थिर ngx_resolver_ग्रन्थिः_t *
ngx_resolver_lookup_addr(ngx_resolver_t *r, अन्तर्_addr_t addr)
{
    ngx_rbtree_ग्रन्थिः_t  *ग्रन्थिः, *sentinel;

    ग्रन्थिः = r->addr_rbtree.मूलम्;
    sentinel = r->addr_rbtree.sentinel;

    यावत् (ग्रन्थिः != sentinel) {

        यदि (addr < ग्रन्थिः->कुञ्चिका) {
            ग्रन्थिः = ग्रन्थिः->वामः;
            अनुवर्त;
        }

        यदि (addr > ग्रन्थिः->कुञ्चिका) {
            ग्रन्थिः = ग्रन्थिः->दक्षिणः;
            अनुवर्त;
        }

        /* addr == node->key */

        प्रत्यागम ngx_resolver_ग्रन्थिः(ग्रन्थिः);
    }

    /* not found */

    प्रत्यागम रिक्त;
}


#यदि (NGX_HAVE_INET6)

स्थिर ngx_resolver_ग्रन्थिः_t *
ngx_resolver_lookup_addr6(ngx_resolver_t *r, संरचना in6_addr *addr,
    uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_ग्रन्थिः_t    *ग्रन्थिः, *sentinel;
    ngx_resolver_ग्रन्थिः_t  *rn;

    ग्रन्थिः = r->addr6_rbtree.मूलम्;
    sentinel = r->addr6_rbtree.sentinel;

    यावत् (ग्रन्थिः != sentinel) {

        यदि (hash < ग्रन्थिः->कुञ्चिका) {
            ग्रन्थिः = ग्रन्थिः->वामः;
            अनुवर्त;
        }

        यदि (hash > ग्रन्थिः->कुञ्चिका) {
            ग्रन्थिः = ग्रन्थिः->दक्षिणः;
            अनुवर्त;
        }

        /* hash == node->key */

        rn = ngx_resolver_ग्रन्थिः(ग्रन्थिः);

        rc = ngx_memcmp(addr, &rn->addr6, 16);

        यदि (rc == 0) {
            प्रत्यागम rn;
        }

        ग्रन्थिः = (rc < 0) ? ग्रन्थिः->वामः : ग्रन्थिः->दक्षिणः;
    }

    /* not found */

    प्रत्यागम रिक्त;
}

#यदि_अन्तः


स्थिर शून्य
ngx_resolver_rbtree_निवेशय_मानम्(ngx_rbtree_ग्रन्थिः_t *अस्थायी,
    ngx_rbtree_ग्रन्थिः_t *ग्रन्थिः, ngx_rbtree_ग्रन्थिः_t *sentinel)
{
    ngx_rbtree_ग्रन्थिः_t    **p;
    ngx_resolver_ग्रन्थिः_t   *rn, *rn_अस्थायी;

    प्रति ( ;; ) {

        यदि (ग्रन्थिः->कुञ्चिका < अस्थायी->कुञ्चिका) {

            p = &अस्थायी->वामः;

        } अन्यथा यदि (ग्रन्थिः->कुञ्चिका > अस्थायी->कुञ्चिका) {

            p = &अस्थायी->दक्षिणः;

        } अन्यथा { /* node->key == temp->key */

            rn = ngx_resolver_ग्रन्थिः(ग्रन्थिः);
            rn_अस्थायी = ngx_resolver_ग्रन्थिः(अस्थायी);

            p = (ngx_memn2cmp(rn->name, rn_अस्थायी->name, rn->nlen, rn_अस्थायी->nlen)
                 < 0) ? &अस्थायी->वामः : &अस्थायी->दक्षिणः;
        }

        यदि (*p == sentinel) {
            भङ्ग;
        }

        अस्थायी = *p;
    }

    *p = ग्रन्थिः;
    ग्रन्थिः->parent = अस्थायी;
    ग्रन्थिः->वामः = sentinel;
    ग्रन्थिः->दक्षिणः = sentinel;
    ngx_rbt_red(ग्रन्थिः);
}


#यदि (NGX_HAVE_INET6)

स्थिर शून्य
ngx_resolver_rbtree_निवेशय_addr6_मानम्(ngx_rbtree_ग्रन्थिः_t *अस्थायी,
    ngx_rbtree_ग्रन्थिः_t *ग्रन्थिः, ngx_rbtree_ग्रन्थिः_t *sentinel)
{
    ngx_rbtree_ग्रन्थिः_t    **p;
    ngx_resolver_ग्रन्थिः_t   *rn, *rn_अस्थायी;

    प्रति ( ;; ) {

        यदि (ग्रन्थिः->कुञ्चिका < अस्थायी->कुञ्चिका) {

            p = &अस्थायी->वामः;

        } अन्यथा यदि (ग्रन्थिः->कुञ्चिका > अस्थायी->कुञ्चिका) {

            p = &अस्थायी->दक्षिणः;

        } अन्यथा { /* node->key == temp->key */

            rn = ngx_resolver_ग्रन्थिः(ग्रन्थिः);
            rn_अस्थायी = ngx_resolver_ग्रन्थिः(अस्थायी);

            p = (ngx_memcmp(&rn->addr6, &rn_अस्थायी->addr6, 16)
                 < 0) ? &अस्थायी->वामः : &अस्थायी->दक्षिणः;
        }

        यदि (*p == sentinel) {
            भङ्ग;
        }

        अस्थायी = *p;
    }

    *p = ग्रन्थिः;
    ग्रन्थिः->parent = अस्थायी;
    ग्रन्थिः->वामः = sentinel;
    ग्रन्थिः->दक्षिणः = sentinel;
    ngx_rbt_red(ग्रन्थिः);
}

#यदि_अन्तः


स्थिर ngx_int_t
ngx_resolver_create_name_query(ngx_resolver_t *r, ngx_resolver_ग्रन्थिः_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    आकार_t               len, nlen;
    ngx_uint_t           ident;
    ngx_resolver_qs_t   *qs;
    ngx_resolver_hdr_t  *query;

    nlen = name->len ? (1 + name->len + 1) : 1;

    len = परिमाण(ngx_resolver_hdr_t) + nlen + परिमाण(ngx_resolver_qs_t);

#यदि (NGX_HAVE_INET6)
    p = ngx_resolver_alloc(r, len * (r->ipv4 + r->ipv6));
#अन्यथा
    p = ngx_resolver_alloc(r, len);
#यदि_अन्तः
    यदि (p == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    rn->qlen = (u_short) len;
    rn->query = p;

#यदि (NGX_HAVE_INET6)
    यदि (r->ipv6) {
        rn->query6 = r->ipv4 ? (p + len) : p;
    }
#यदि_अन्तः

    query = (ngx_resolver_hdr_t *) p;

    यदि (r->ipv4) {
        ident = ngx_random();

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->लघुगणकः, 0,
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

    यदि (name->len == 0)  {
        प्रत्यागम NGX_DECLINED;
    }

    प्रति (s = name->दत्त + name->len - 1; s >= name->दत्त; s--) {
        यदि (*s != '.') {
            *p = *s;
            len++;

        } अन्यथा {
            यदि (len == 0 || len > 255) {
                प्रत्यागम NGX_DECLINED;
            }

            *p = (u_char) len;
            len = 0;
        }

        p--;
    }

    यदि (len == 0 || len > 255) {
        प्रत्यागम NGX_DECLINED;
    }

    *p = (u_char) len;

#यदि (NGX_HAVE_INET6)
    यदि (!r->ipv6) {
        प्रत्यागम NGX_OK;
    }

    p = rn->query6;

    यदि (r->ipv4) {
        ngx_memcpy(p, rn->query, rn->qlen);
    }

    query = (ngx_resolver_hdr_t *) p;

    ident = ngx_random();

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->लघुगणकः, 0,
                   "resolve: \"%V\" AAAA %i", name, ident & 0xffff);

    query->ident_hi = (u_char) ((ident >> 8) & 0xff);
    query->ident_lo = (u_char) (ident & 0xff);

    p += परिमाण(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    qs->type_lo = NGX_RESOLVE_AAAA;
#यदि_अन्तः

    प्रत्यागम NGX_OK;
}


स्थिर ngx_int_t
ngx_resolver_create_srv_query(ngx_resolver_t *r, ngx_resolver_ग्रन्थिः_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    आकार_t               len, nlen;
    ngx_uint_t           ident;
    ngx_resolver_qs_t   *qs;
    ngx_resolver_hdr_t  *query;

    nlen = name->len ? (1 + name->len + 1) : 1;

    len = परिमाण(ngx_resolver_hdr_t) + nlen + परिमाण(ngx_resolver_qs_t);

    p = ngx_resolver_alloc(r, len);
    यदि (p == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    rn->qlen = (u_short) len;
    rn->query = p;

    query = (ngx_resolver_hdr_t *) p;

    ident = ngx_random();

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->लघुगणकः, 0,
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

    यदि (name->len == 0)  {
        प्रत्यागम NGX_DECLINED;
    }

    प्रति (s = name->दत्त + name->len - 1; s >= name->दत्त; s--) {
        यदि (*s != '.') {
            *p = *s;
            len++;

        } अन्यथा {
            यदि (len == 0 || len > 255) {
                प्रत्यागम NGX_DECLINED;
            }

            *p = (u_char) len;
            len = 0;
        }

        p--;
    }

    यदि (len == 0 || len > 255) {
        प्रत्यागम NGX_DECLINED;
    }

    *p = (u_char) len;

    प्रत्यागम NGX_OK;
}


स्थिर ngx_int_t
ngx_resolver_create_addr_query(ngx_resolver_t *r, ngx_resolver_ग्रन्थिः_t *rn,
    ngx_resolver_addr_t *addr)
{
    u_char               *p, *d;
    आकार_t                len;
    अन्तर्_addr_t             inaddr;
    ngx_int_t             n;
    ngx_uint_t            ident;
    ngx_resolver_hdr_t   *query;
    संरचना sockaddr_अन्तर्   *ज्या;
#यदि (NGX_HAVE_INET6)
    संरचना sockaddr_in6  *sin6;
#यदि_अन्तः

    विकल्प (addr->sockaddr->sa_family) {

#यदि (NGX_HAVE_INET6)
    पक्ष AF_INET6:
        len = परिमाण(ngx_resolver_hdr_t)
              + 64 + परिमाण(".ip6.arpa.") - 1
              + परिमाण(ngx_resolver_qs_t);

        भङ्ग;
#यदि_अन्तः

    स्वभाव: /* AF_INET */
        len = परिमाण(ngx_resolver_hdr_t)
              + परिमाण(".255.255.255.255.in-addr.arpa.") - 1
              + परिमाण(ngx_resolver_qs_t);
    }

    p = ngx_resolver_alloc(r, len);
    यदि (p == रिक्त) {
        प्रत्यागम NGX_ERROR;
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

#यदि (NGX_HAVE_INET6)
    पक्ष AF_INET6:
        sin6 = (संरचना sockaddr_in6 *) addr->sockaddr;

        प्रति (n = 15; n >= 0; n--) {
            p = ngx_sprintf(p, "\1%xd\1%xd",
                            sin6->sin6_addr.s6_addr[n] & 0xf,
                            (sin6->sin6_addr.s6_addr[n] >> 4) & 0xf);
        }

        p = ngx_cpymem(p, "\3ip6\4arpa\0", 10);

        भङ्ग;
#यदि_अन्तः

    स्वभाव: /* AF_INET */

        ज्या = (संरचना sockaddr_अन्तर् *) addr->sockaddr;
        inaddr = ntohl(ज्या->sin_addr.s_addr);

        प्रति (n = 0; n < 32; n += 8) {
            d = ngx_sprintf(&p[1], "%ud", (inaddr >> n) & 0xff);
            *p = (u_char) (d - &p[1]);
            p = d;
        }

        p = ngx_cpymem(p, "\7in-addr\4arpa\0", 14);
    }

    /* query type "PTR", IN query class */
    p = ngx_cpymem(p, "\0\14\0\1", 4);

    rn->qlen = (u_short) (p - rn->query);

    प्रत्यागम NGX_OK;
}


स्थिर ngx_int_t
ngx_resolver_copy(ngx_resolver_t *r, ngx_str_t *name, u_char *buf, u_char *src,
    u_char *चरमः)
{
    वर्ण        *err;
    u_char      *p, *dst;
    आकार_t       len;
    ngx_uint_t   i, n;

    p = src;
    len = 0;

    /*
     * compression pointers allow to create endless loop, so we set limit;
     * 128 pointers should be enough to store 255-byte name
     */

    प्रति (i = 0; i < 128; i++) {
        n = *p++;

        यदि (n == 0) {
            गच्छ समाप्तम्;
        }

        यदि (n & 0xc0) {
            यदि ((n & 0xc0) != 0xc0) {
                err = "invalid label type in DNS response";
                गच्छ invalid;
            }

            यदि (p >= चरमः) {
                err = "name is out of DNS response";
                गच्छ invalid;
            }

            n = ((n & 0x3f) << 8) + *p;
            p = &buf[n];

        } अन्यथा {
            len += 1 + n;
            p = &p[n];
        }

        यदि (p >= चरमः) {
            err = "name is out of DNS response";
            गच्छ invalid;
        }
    }

    err = "compression pointers loop in DNS response";

invalid:

    ngx_log_error(r->log_level, r->लघुगणकः, 0, err);

    प्रत्यागम NGX_ERROR;

समाप्तम्:

    यदि (name == रिक्त) {
        प्रत्यागम NGX_OK;
    }

    यदि (len == 0) {
        ngx_str_null(name);
        प्रत्यागम NGX_OK;
    }

    dst = ngx_resolver_alloc(r, len);
    यदि (dst == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    name->दत्त = dst;

    प्रति ( ;; ) {
        n = *src++;

        यदि (n == 0) {
            name->len = dst - name->दत्त - 1;
            प्रत्यागम NGX_OK;
        }

        यदि (n & 0xc0) {
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


स्थिर ngx_int_t
ngx_resolver_समुच्चयः_timeout(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx)
{
    यदि (ctx->event || ctx->timeout == 0) {
        प्रत्यागम NGX_OK;
    }

    ctx->event = ngx_resolver_calloc(r, परिमाण(ngx_event_t));
    यदि (ctx->event == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    ctx->event->handler = ngx_resolver_timeout_handler;
    ctx->event->दत्त = ctx;
    ctx->event->लघुगणकः = r->लघुगणकः;
    ctx->event->cancelable = ctx->cancelable;
    ctx->ident = -1;

    ngx_सङ्कलय_timer(ctx->event, ctx->timeout);

    प्रत्यागम NGX_OK;
}


स्थिर शून्य
ngx_resolver_timeout_handler(ngx_event_t *ev)
{
    ngx_resolver_ctx_t  *ctx;

    ctx = ev->दत्त;

    ctx->state = NGX_RESOLVE_TIMEDOUT;

    ctx->handler(ctx);
}


स्थिर शून्य
ngx_resolver_free_ग्रन्थिः(ngx_resolver_t *r, ngx_resolver_ग्रन्थिः_t *rn)
{
    ngx_uint_t  i;

    /* lock alloc mutex */

    यदि (rn->query) {
        ngx_resolver_free_locked(r, rn->query);
    }

    यदि (rn->name) {
        ngx_resolver_free_locked(r, rn->name);
    }

    यदि (rn->cnlen) {
        ngx_resolver_free_locked(r, rn->u.cname);
    }

    यदि (rn->naddrs > 1 && rn->naddrs != (u_short) -1) {
        ngx_resolver_free_locked(r, rn->u.addrs);
    }

#यदि (NGX_HAVE_INET6)
    यदि (rn->naddrs6 > 1 && rn->naddrs6 != (u_short) -1) {
        ngx_resolver_free_locked(r, rn->u6.addrs6);
    }
#यदि_अन्तः

    यदि (rn->nsrvs) {
        प्रति (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
            यदि (rn->u.srvs[i].name.दत्त) {
                ngx_resolver_free_locked(r, rn->u.srvs[i].name.दत्त);
            }
        }

        ngx_resolver_free_locked(r, rn->u.srvs);
    }

    ngx_resolver_free_locked(r, rn);

    /* unlock alloc mutex */
}


स्थिर शून्य *
ngx_resolver_alloc(ngx_resolver_t *r, आकार_t आकार)
{
    u_char  *p;

    /* lock alloc mutex */

    p = ngx_alloc(आकार, r->लघुगणकः);

    /* unlock alloc mutex */

    प्रत्यागम p;
}


स्थिर शून्य *
ngx_resolver_calloc(ngx_resolver_t *r, आकार_t आकार)
{
    u_char  *p;

    p = ngx_resolver_alloc(r, आकार);

    यदि (p) {
        ngx_memzero(p, आकार);
    }

    प्रत्यागम p;
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
ngx_resolver_dup(ngx_resolver_t *r, शून्य *src, आकार_t आकार)
{
    शून्य  *dst;

    dst = ngx_resolver_alloc(r, आकार);

    यदि (dst == रिक्त) {
        प्रत्यागम dst;
    }

    ngx_memcpy(dst, src, आकार);

    प्रत्यागम dst;
}


स्थिर ngx_resolver_addr_t *
ngx_resolver_export(ngx_resolver_t *r, ngx_resolver_ग्रन्थिः_t *rn,
    ngx_uint_t rotate)
{
    ngx_uint_t            d, i, j, n;
    अन्तर्_addr_t            *addr;
    ngx_sockaddr_t       *sockaddr;
    संरचना sockaddr_अन्तर्   *ज्या;
    ngx_resolver_addr_t  *dst;
#यदि (NGX_HAVE_INET6)
    संरचना in6_addr      *addr6;
    संरचना sockaddr_in6  *sin6;
#यदि_अन्तः

    n = rn->naddrs;
#यदि (NGX_HAVE_INET6)
    n += rn->naddrs6;
#यदि_अन्तः

    dst = ngx_resolver_calloc(r, n * परिमाण(ngx_resolver_addr_t));
    यदि (dst == रिक्त) {
        प्रत्यागम रिक्त;
    }

    sockaddr = ngx_resolver_calloc(r, n * परिमाण(ngx_sockaddr_t));
    यदि (sockaddr == रिक्त) {
        ngx_resolver_free(r, dst);
        प्रत्यागम रिक्त;
    }

    i = 0;
    d = rotate ? ngx_random() % n : 0;

    यदि (rn->naddrs) {
        j = rotate ? ngx_random() % rn->naddrs : 0;

        addr = (rn->naddrs == 1) ? &rn->u.addr : rn->u.addrs;

        कुरु {
            ज्या = &sockaddr[d].sockaddr_अन्तर्;
            ज्या->sin_family = AF_INET;
            ज्या->sin_addr.s_addr = addr[j++];
            dst[d].sockaddr = (संरचना sockaddr *) ज्या;
            dst[d++].socklen = परिमाण(संरचना sockaddr_अन्तर्);

            यदि (d == n) {
                d = 0;
            }

            यदि (j == (ngx_uint_t) rn->naddrs) {
                j = 0;
            }
        } यावत् (++i < (ngx_uint_t) rn->naddrs);
    }

#यदि (NGX_HAVE_INET6)
    यदि (rn->naddrs6) {
        j = rotate ? ngx_random() % rn->naddrs6 : 0;

        addr6 = (rn->naddrs6 == 1) ? &rn->u6.addr6 : rn->u6.addrs6;

        कुरु {
            sin6 = &sockaddr[d].sockaddr_in6;
            sin6->sin6_family = AF_INET6;
            ngx_memcpy(sin6->sin6_addr.s6_addr, addr6[j++].s6_addr, 16);
            dst[d].sockaddr = (संरचना sockaddr *) sin6;
            dst[d++].socklen = परिमाण(संरचना sockaddr_in6);

            यदि (d == n) {
                d = 0;
            }

            यदि (j == rn->naddrs6) {
                j = 0;
            }
        } यावत् (++i < n);
    }
#यदि_अन्तः

    प्रत्यागम dst;
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

    प्रति (i = 0; i < nsrvs; i++) {
        यदि (srvs[i].state == NGX_ERROR) {
            ctx->state = NGX_ERROR;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

            ctx->handler(ctx);
            प्रत्यागम;
        }

        naddrs += srvs[i].naddrs;
    }

    यदि (naddrs == 0) {
        ctx->state = srvs[0].state;

        प्रति (i = 0; i < nsrvs; i++) {
            यदि (srvs[i].state == NGX_RESOLVE_NXDOMAIN) {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                भङ्ग;
            }
        }

        ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        प्रत्यागम;
    }

    addrs = ngx_resolver_calloc(r, naddrs * परिमाण(ngx_resolver_addr_t));
    यदि (addrs == रिक्त) {
        ctx->state = NGX_ERROR;
        ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        प्रत्यागम;
    }

    i = 0;
    n = 0;

    कुरु {
        nw = 0;

        प्रति (j = i; j < nsrvs; j++) {
            यदि (srvs[j].प्राथमिकता != srvs[i].प्राथमिकता) {
                भङ्ग;
            }

            nw += srvs[j].naddrs * srvs[j].weight;
        }

        यदि (nw == 0) {
            गच्छ अग्र_srv;
        }

        w = ngx_random() % nw;

        प्रति (k = i; k < j; k++) {
            यदि (w < srvs[k].naddrs * srvs[k].weight) {
                भङ्ग;
            }

            w -= srvs[k].naddrs * srvs[k].weight;
        }

        प्रति (l = i; l < j; l++) {

            प्रति (m = 0; m < srvs[k].naddrs; m++) {
                addrs[n].socklen = srvs[k].addrs[m].socklen;
                addrs[n].sockaddr = srvs[k].addrs[m].sockaddr;
                addrs[n].name = srvs[k].name;
                addrs[n].प्राथमिकता = srvs[k].प्राथमिकता;
                addrs[n].weight = srvs[k].weight;
                n++;
            }

            यदि (++k == j) {
                k = i;
            }
        }

अग्र_srv:

        i = j;

    } यावत् (i < ctx->nsrvs);

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

    यदि (err > 0 && err < 6) {
        प्रत्यागम errors[err - 1];
    }

    यदि (err == NGX_RESOLVE_TIMEDOUT) {
        प्रत्यागम "Operation timed out";
    }

    प्रत्यागम "Unknown error";
}


स्थिर u_char *
ngx_resolver_log_error(ngx_log_t *लघुगणकः, u_char *buf, आकार_t len)
{
    u_char                     *p;
    ngx_resolver_connection_t  *rec;

    p = buf;

    यदि (लघुगणकः->action) {
        p = ngx_snprintf(buf, len, " while %s", लघुगणकः->action);
        len -= p - buf;
    }

    rec = लघुगणकः->दत्त;

    यदि (rec) {
        p = ngx_snprintf(p, len, ", resolver: %V", &rec->server);
    }

    प्रत्यागम p;
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

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->लघुगणकः, 0, "UDP socket %d", s);

    यदि (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->लघुगणकः, ngx_socket_errno,
                      ngx_socket_n " failed");
        प्रत्यागम NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->लघुगणकः);

    यदि (c == रिक्त) {
        यदि (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->लघुगणकः, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        प्रत्यागम NGX_ERROR;
    }

    यदि (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->लघुगणकः, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        गच्छ failed;
    }

    rev = c->पठ;
    wev = c->लिख;

    rev->लघुगणकः = &rec->लघुगणकः;
    wev->लघुगणकः = &rec->लघुगणकः;

    rec->udp = c;

    c->सङ्ख्या = ngx_atomic_fetch_सङ्कलय(ngx_connection_गणक, 1);

    c->start_time = ngx_current_msec;

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->लघुगणकः, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->सङ्ख्या);

    rc = connect(s, rec->sockaddr, rec->socklen);

    यदि (rc == -1) {
        ngx_log_error(NGX_LOG_CRIT, &rec->लघुगणकः, ngx_socket_errno,
                      "connect() failed");

        गच्छ failed;
    }

    /* UDP sockets are always ready to write */
    wev->ready = 1;

    event = (ngx_event_flags & NGX_USE_CLEAR_EVENT) ?
                /* kqueue, epoll */                 NGX_CLEAR_EVENT:
                /* select, poll, /dev/poll */       NGX_LEVEL_EVENT;
                /* eventport event type has no meaning: oneshot only */

    यदि (ngx_सङ्कलय_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        गच्छ failed;
    }

    प्रत्यागम NGX_OK;

failed:

    ngx_close_connection(c);
    rec->udp = रिक्त;

    प्रत्यागम NGX_ERROR;
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

    s = ngx_socket(rec->sockaddr->sa_family, SOCK_प्रवाहः, 0);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->लघुगणकः, 0, "TCP socket %d", s);

    यदि (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->लघुगणकः, ngx_socket_errno,
                      ngx_socket_n " failed");
        प्रत्यागम NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->लघुगणकः);

    यदि (c == रिक्त) {
        यदि (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->लघुगणकः, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        प्रत्यागम NGX_ERROR;
    }

    यदि (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->लघुगणकः, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        गच्छ failed;
    }

    rev = c->पठ;
    wev = c->लिख;

    rev->लघुगणकः = &rec->लघुगणकः;
    wev->लघुगणकः = &rec->लघुगणकः;

    rec->tcp = c;

    c->सङ्ख्या = ngx_atomic_fetch_सङ्कलय(ngx_connection_गणक, 1);

    c->start_time = ngx_current_msec;

    यदि (ngx_सङ्कलय_conn) {
        यदि (ngx_सङ्कलय_conn(c) == NGX_ERROR) {
            गच्छ failed;
        }
    }

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->लघुगणकः, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->सङ्ख्या);

    rc = connect(s, rec->sockaddr, rec->socklen);

    यदि (rc == -1) {
        err = ngx_socket_errno;


        यदि (err != NGX_EINPROGRESS
#यदि (NGX_WIN32)
            /* Winsock returns WSAEWOULDBLOCK (NGX_EAGAIN) */
            && err != NGX_EAGAIN
#यदि_अन्तः
            )
        {
            यदि (err == NGX_ECONNREFUSED
#यदि (NGX_LINUX)
                /*
                 * Linux returns EAGAIN instead of ECONNREFUSED
                 * for unix sockets if listen queue is full
                 */
                || err == NGX_EAGAIN
#यदि_अन्तः
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

            ngx_log_error(level, &rec->लघुगणकः, err, "connect() to %V failed",
                          &rec->server);

            ngx_close_connection(c);
            rec->tcp = रिक्त;

            प्रत्यागम NGX_ERROR;
        }
    }

    यदि (ngx_सङ्कलय_conn) {
        यदि (rc == -1) {

            /* NGX_EINPROGRESS */

            प्रत्यागम NGX_AGAIN;
        }

        ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->लघुगणकः, 0, "connected");

        wev->ready = 1;

        प्रत्यागम NGX_OK;
    }

    यदि (ngx_event_flags & NGX_USE_CLEAR_EVENT) {

        /* kqueue */

        event = NGX_CLEAR_EVENT;

    } अन्यथा {

        /* select, poll, /dev/poll */

        event = NGX_LEVEL_EVENT;
    }

    यदि (ngx_सङ्कलय_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        गच्छ failed;
    }

    यदि (rc == -1) {

        /* NGX_EINPROGRESS */

        यदि (ngx_सङ्कलय_event(wev, NGX_WRITE_EVENT, event) != NGX_OK) {
            गच्छ failed;
        }

        प्रत्यागम NGX_AGAIN;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->लघुगणकः, 0, "connected");

    wev->ready = 1;

    प्रत्यागम NGX_OK;

failed:

    ngx_close_connection(c);
    rec->tcp = रिक्त;

    प्रत्यागम NGX_ERROR;
}


स्थिर ngx_int_t
ngx_resolver_cmp_srvs(ध्रुव शून्य *one, ध्रुव शून्य *two)
{
    ngx_int_t            p1, p2;
    ngx_resolver_srv_t  *first, *second;

    first = (ngx_resolver_srv_t *) one;
    second = (ngx_resolver_srv_t *) two;

    p1 = first->प्राथमिकता;
    p2 = second->प्राथमिकता;

    प्रत्यागम p1 - p2;
}
