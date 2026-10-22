
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


प्रकारव्याख्या संरचना {
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


प्रकारव्याख्या संरचना {
    u_char  type_hi;
    u_char  type_lo;
    u_char  class_hi;
    u_char  class_lo;
} ngx_resolver_qs_t;


प्रकारव्याख्या संरचना {
    u_char  type_hi;
    u_char  type_lo;
    u_char  class_hi;
    u_char  class_lo;
    u_char  ttl[4];
    u_char  len_hi;
    u_char  len_lo;
} ngx_resolver_an_t;


#define ngx_resolver_गाठ(n)  ngx_rbtree_data(n, ngx_resolver_गाठ_t, गाठ)


स्थिर ngx_int_t ngx_udp_connect(ngx_resolver_connection_t *rec);
स्थिर ngx_int_t ngx_tcp_connect(ngx_resolver_connection_t *rec);


स्थिर शून्य ngx_resolver_cleanup(शून्य *data);
स्थिर शून्य ngx_resolver_cleanup_वृक्ष(ngx_resolver_t *r, ngx_rbtree_t *वृक्ष);
स्थिर ngx_int_t ngx_resolve_name_locked(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx, ngx_str_t *name);
स्थिर शून्य ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *वृक्ष,
    ngx_queue_t *queue);
स्थिर ngx_int_t ngx_resolver_send_query(ngx_resolver_t *r,
    ngx_resolver_गाठ_t *rn);
स्थिर ngx_int_t ngx_resolver_send_udp_query(ngx_resolver_t *r,
    ngx_resolver_connection_t *rec, u_char *query, u_short qlen);
स्थिर ngx_int_t ngx_resolver_send_tcp_query(ngx_resolver_t *r,
    ngx_resolver_connection_t *rec, u_char *query, u_short qlen);
स्थिर ngx_int_t ngx_resolver_create_name_query(ngx_resolver_t *r,
    ngx_resolver_गाठ_t *rn, ngx_str_t *name);
स्थिर ngx_int_t ngx_resolver_create_srv_query(ngx_resolver_t *r,
    ngx_resolver_गाठ_t *rn, ngx_str_t *name);
स्थिर ngx_int_t ngx_resolver_create_addr_query(ngx_resolver_t *r,
    ngx_resolver_गाठ_t *rn, ngx_resolver_addr_t *addr);
स्थिर शून्य ngx_resolver_resend_handler(ngx_event_t *ev);
स्थिर समय_प्रकार ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *वृक्ष,
    ngx_queue_t *queue);
स्थिर ngx_uint_t ngx_resolver_resend_empty(ngx_resolver_t *r);
स्थिर शून्य ngx_resolver_udp_read(ngx_event_t *rev);
स्थिर शून्य ngx_resolver_tcp_write(ngx_event_t *wev);
स्थिर शून्य ngx_resolver_tcp_read(ngx_event_t *rev);
स्थिर शून्य ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf,
    माप_प्रकार n, ngx_uint_t tcp);
स्थिर शून्य ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, माप_प्रकार n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans);
स्थिर शून्य ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, माप_प्रकार n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans);
स्थिर शून्य ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, माप_प्रकार n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan);
स्थिर ngx_resolver_गाठ_t *ngx_resolver_lookup_name(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
स्थिर ngx_resolver_गाठ_t *ngx_resolver_lookup_srv(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
स्थिर ngx_resolver_गाठ_t *ngx_resolver_lookup_addr(ngx_resolver_t *r,
    in_addr_t addr);
स्थिर शून्य ngx_resolver_rbtree_घाला_मूल्य(ngx_rbtree_गाठ_t *temp,
    ngx_rbtree_गाठ_t *गाठ, ngx_rbtree_गाठ_t *sentinel);
स्थिर ngx_int_t ngx_resolver_copy(ngx_resolver_t *r, ngx_str_t *name,
    u_char *buf, u_char *src, u_char *last);
स्थिर ngx_int_t ngx_resolver_set_timeout(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx);
स्थिर शून्य ngx_resolver_timeout_handler(ngx_event_t *ev);
स्थिर शून्य ngx_resolver_free_गाठ(ngx_resolver_t *r, ngx_resolver_गाठ_t *rn);
स्थिर शून्य *ngx_resolver_alloc(ngx_resolver_t *r, माप_प्रकार size);
स्थिर शून्य *ngx_resolver_calloc(ngx_resolver_t *r, माप_प्रकार size);
स्थिर शून्य ngx_resolver_free(ngx_resolver_t *r, शून्य *p);
स्थिर शून्य ngx_resolver_free_locked(ngx_resolver_t *r, शून्य *p);
स्थिर शून्य *ngx_resolver_dup(ngx_resolver_t *r, शून्य *src, माप_प्रकार size);
स्थिर ngx_resolver_addr_t *ngx_resolver_export(ngx_resolver_t *r,
    ngx_resolver_गाठ_t *rn, ngx_uint_t rotate);
स्थिर शून्य ngx_resolver_report_srv(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx);
स्थिर u_char *ngx_resolver_log_error(ngx_log_t *log, u_char *buf, माप_प्रकार len);
स्थिर शून्य ngx_resolver_resolve_srv_names(ngx_resolver_ctx_t *ctx,
    ngx_resolver_गाठ_t *rn);
स्थिर शून्य ngx_resolver_srv_names_handler(ngx_resolver_ctx_t *ctx);
स्थिर ngx_int_t ngx_resolver_cmp_srvs(स्थिरांक शून्य *one, स्थिरांक शून्य *two);

#जर (NGX_HAVE_INET6)
स्थिर शून्य ngx_resolver_rbtree_घाला_addr6_मूल्य(ngx_rbtree_गाठ_t *temp,
    ngx_rbtree_गाठ_t *गाठ, ngx_rbtree_गाठ_t *sentinel);
स्थिर ngx_resolver_गाठ_t *ngx_resolver_lookup_addr6(ngx_resolver_t *r,
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

    r = ngx_pcalloc(cf->pool, आकार(ngx_resolver_t));
    जर (r == रिक्त) {
        परत रिक्त;
    }

    r->event = ngx_pcalloc(cf->pool, आकार(ngx_event_t));
    जर (r->event == रिक्त) {
        परत रिक्त;
    }

    cln = ngx_pool_cleanup_add(cf->pool, 0);
    जर (cln == रिक्त) {
        परत रिक्त;
    }

    cln->handler = ngx_resolver_cleanup;
    cln->data = r;

    r->ipv4 = 1;

    ngx_rbtree_init(&r->name_rbtree, &r->name_sentinel,
                    ngx_resolver_rbtree_घाला_मूल्य);

    ngx_rbtree_init(&r->srv_rbtree, &r->srv_sentinel,
                    ngx_resolver_rbtree_घाला_मूल्य);

    ngx_rbtree_init(&r->addr_rbtree, &r->addr_sentinel,
                    ngx_rbtree_घाला_मूल्य);

    ngx_queue_init(&r->name_resend_queue);
    ngx_queue_init(&r->srv_resend_queue);
    ngx_queue_init(&r->addr_resend_queue);

    ngx_queue_init(&r->name_expire_queue);
    ngx_queue_init(&r->srv_expire_queue);
    ngx_queue_init(&r->addr_expire_queue);

#जर (NGX_HAVE_INET6)
    r->ipv6 = 1;

    ngx_rbtree_init(&r->addr6_rbtree, &r->addr6_sentinel,
                    ngx_resolver_rbtree_घाला_addr6_मूल्य);

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

    जर (n) {
        जर (ngx_array_init(&r->connections, cf->pool, n,
                           आकार(ngx_resolver_connection_t))
            != NGX_OK)
        {
            परत रिक्त;
        }
    }

    साठी (i = 0; i < n; i++) {
        जर (ngx_strncmp(names[i].data, "valid=", 6) == 0) {
            s.len = names[i].len - 6;
            s.data = names[i].data + 6;

            r->valid = ngx_parse_time(&s, 1);

            जर (r->valid == (समय_प्रकार) NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &names[i]);
                परत रिक्त;
            }

            सुरूठेवा;
        }

#जर (NGX_HAVE_INET6)
        जर (ngx_strncmp(names[i].data, "ipv4=", 5) == 0) {

            जर (ngx_strcmp(&names[i].data[5], "on") == 0) {
                r->ipv4 = 1;

            } अन्यथा जर (ngx_strcmp(&names[i].data[5], "off") == 0) {
                r->ipv4 = 0;

            } अन्यथा {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &names[i]);
                परत रिक्त;
            }

            सुरूठेवा;
        }

        जर (ngx_strncmp(names[i].data, "ipv6=", 5) == 0) {

            जर (ngx_strcmp(&names[i].data[5], "on") == 0) {
                r->ipv6 = 1;

            } अन्यथा जर (ngx_strcmp(&names[i].data[5], "off") == 0) {
                r->ipv6 = 0;

            } अन्यथा {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &names[i]);
                परत रिक्त;
            }

            सुरूठेवा;
        }
#endif

        ngx_memzero(&u, आकार(ngx_url_t));

        u.url = names[i];
        u.default_port = 53;

        जर (ngx_parse_url(cf->pool, &u) != NGX_OK) {
            जर (u.err) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "%s in resolver \"%V\"",
                                   u.err, &u.url);
            }

            परत रिक्त;
        }

        rec = ngx_array_ढकला_n(&r->connections, u.naddrs);
        जर (rec == रिक्त) {
            परत रिक्त;
        }

        ngx_memzero(rec, u.naddrs * आकार(ngx_resolver_connection_t));

        साठी (j = 0; j < u.naddrs; j++) {
            rec[j].sockaddr = u.addrs[j].sockaddr;
            rec[j].socklen = u.addrs[j].socklen;
            rec[j].server = u.addrs[j].name;
            rec[j].resolver = r;
        }
    }

#जर (NGX_HAVE_INET6)
    जर (r->ipv4 + r->ipv6 == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"ipv4\" and \"ipv6\" cannot both be \"off\"");
        परत रिक्त;
    }
#endif

    जर (n && r->connections.nelts == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "no name servers defined");
        परत रिक्त;
    }

    परत r;
}


स्थिर शून्य
ngx_resolver_cleanup(शून्य *data)
{
    ngx_resolver_t  *r = data;

    ngx_uint_t                  i;
    ngx_resolver_connection_t  *rec;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "cleanup resolver");

    ngx_resolver_cleanup_वृक्ष(r, &r->name_rbtree);

    ngx_resolver_cleanup_वृक्ष(r, &r->srv_rbtree);

    ngx_resolver_cleanup_वृक्ष(r, &r->addr_rbtree);

#जर (NGX_HAVE_INET6)
    ngx_resolver_cleanup_वृक्ष(r, &r->addr6_rbtree);
#endif

    जर (r->event->timer_set) {
        ngx_del_timer(r->event);
    }

    rec = r->connections.elts;

    साठी (i = 0; i < r->connections.nelts; i++) {
        जर (rec[i].udp) {
            ngx_close_connection(rec[i].udp);
        }

        जर (rec[i].tcp) {
            ngx_close_connection(rec[i].tcp);
        }

        जर (rec[i].read_buf) {
            ngx_resolver_free(r, rec[i].read_buf->start);
            ngx_resolver_free(r, rec[i].read_buf);
        }

        जर (rec[i].write_buf) {
            ngx_resolver_free(r, rec[i].write_buf->start);
            ngx_resolver_free(r, rec[i].write_buf);
        }
    }
}


स्थिर शून्य
ngx_resolver_cleanup_वृक्ष(ngx_resolver_t *r, ngx_rbtree_t *वृक्ष)
{
    ngx_resolver_ctx_t   *ctx, *next;
    ngx_resolver_गाठ_t  *rn;

    पर्यंत (वृक्ष->मूळ != वृक्ष->sentinel) {

        rn = ngx_resolver_गाठ(ngx_rbtree_min(वृक्ष->मूळ, वृक्ष->sentinel));

        ngx_queue_remove(&rn->queue);

        साठी (ctx = rn->waiting; ctx; ctx = next) {
            next = ctx->next;

            जर (ctx->event) {
                जर (ctx->event->timer_set) {
                    ngx_del_timer(ctx->event);
                }

                ngx_resolver_free(r, ctx->event);
            }

            ngx_resolver_free(r, ctx);
        }

        ngx_rbtree_delete(वृक्ष, &rn->गाठ);

        ngx_resolver_free_गाठ(r, rn);
    }
}


ngx_resolver_ctx_t *
ngx_resolve_start(ngx_resolver_t *r, ngx_resolver_ctx_t *temp)
{
    in_addr_t            addr;
    ngx_resolver_ctx_t  *ctx;

    जर (temp) {
        addr = ngx_inet_addr(temp->name.data, temp->name.len);

        जर (addr != INADDR_NONE) {
            temp->resolver = r;
            temp->state = NGX_OK;
            temp->naddrs = 1;
            temp->addrs = &temp->addr;
            temp->addr.sockaddr = (संरचना sockaddr *) &temp->sin;
            temp->addr.socklen = आकार(संरचना sockaddr_in);
            ngx_memzero(&temp->sin, आकार(संरचना sockaddr_in));
            temp->sin.sin_family = AF_INET;
            temp->sin.sin_addr.s_addr = addr;
            temp->quick = 1;

            परत temp;
        }
    }

    जर (r->connections.nelts == 0) {
        परत NGX_NO_RESOLVER;
    }

    ctx = ngx_resolver_calloc(r, आकार(ngx_resolver_ctx_t));

    जर (ctx) {
        ctx->resolver = r;
    }

    परत ctx;
}


ngx_int_t
ngx_resolve_name(ngx_resolver_ctx_t *ctx)
{
    माप_प्रकार           slen;
    ngx_int_t        rc;
    ngx_str_t        name;
    ngx_resolver_t  *r;

    r = ctx->resolver;

    जर (ctx->name.len > 0 && ctx->name.data[ctx->name.len - 1] == '.') {
        ctx->name.len--;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve: \"%V\"", &ctx->name);

    जर (ctx->quick) {
        ctx->handler(ctx);
        परत NGX_OK;
    }

    जर (ctx->service.len) {
        slen = ctx->service.len;

        जर (ngx_strlchr(ctx->service.data,
                        ctx->service.data + ctx->service.len, '.')
            == रिक्त)
        {
            slen += आकार("_._tcp") - 1;
        }

        name.len = slen + 1 + ctx->name.len;

        name.data = ngx_resolver_alloc(r, name.len);
        जर (name.data == रिक्त) {
            जा failed;
        }

        जर (slen == ctx->service.len) {
            ngx_sprintf(name.data, "%V.%V", &ctx->service, &ctx->name);

        } अन्यथा {
            ngx_sprintf(name.data, "_%V._tcp.%V", &ctx->service, &ctx->name);
        }

        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &name);

        ngx_resolver_free(r, name.data);

    } अन्यथा {
        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &ctx->name);
    }

    जर (rc == NGX_OK) {
        परत NGX_OK;
    }

    /* unlock name mutex */

    जर (rc == NGX_AGAIN) {
        परत NGX_OK;
    }

    /* NGX_ERROR */

    जर (ctx->event) {
        ngx_resolver_free(r, ctx->event);
    }

failed:

    ngx_resolver_free(r, ctx);

    परत NGX_ERROR;
}


शून्य
ngx_resolve_name_done(ngx_resolver_ctx_t *ctx)
{
    ngx_uint_t            i;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_गाठ_t  *rn;

    r = ctx->resolver;

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve name done: %i", ctx->state);

    जर (ctx->quick) {
        परत;
    }

    जर (ctx->event && ctx->event->timer_set) {
        ngx_del_timer(ctx->event);
    }

    /* lock name mutex */

    जर (ctx->nsrvs) {
        साठी (i = 0; i < ctx->nsrvs; i++) {
            जर (ctx->srvs[i].ctx) {
                ngx_resolve_name_done(ctx->srvs[i].ctx);
            }

            जर (ctx->srvs[i].addrs) {
                ngx_resolver_free(r, ctx->srvs[i].addrs->sockaddr);
                ngx_resolver_free(r, ctx->srvs[i].addrs);
            }

            ngx_resolver_free(r, ctx->srvs[i].name.data);
        }

        ngx_resolver_free(r, ctx->srvs);
    }

    जर (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->गाठ;

        जर (rn) {
            p = &rn->waiting;
            w = rn->waiting;

            पर्यंत (w) {
                जर (w == ctx) {
                    *p = w->next;

                    जा done;
                }

                p = &w->next;
                w = w->next;
            }

            ngx_log_error(NGX_LOG_ALERT, r->log, 0,
                          "could not cancel %V resolving", &ctx->name);
        }
    }

done:

    जर (ctx->service.len) {
        ngx_resolver_expire(r, &r->srv_rbtree, &r->srv_expire_queue);

    } अन्यथा {
        ngx_resolver_expire(r, &r->name_rbtree, &r->name_expire_queue);
    }

    /* unlock name mutex */

    /* lock alloc mutex */

    जर (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    जर (r->event->timer_set && ngx_resolver_resend_empty(r)) {
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
    ngx_rbtree_t         *वृक्ष;
    ngx_resolver_ctx_t   *next, *last;
    ngx_resolver_addr_t  *addrs;
    ngx_resolver_गाठ_t  *rn;

    ngx_strlow(name->data, name->data, name->len);

    hash = ngx_crc32_short(name->data, name->len);

    जर (ctx->service.len) {
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

    जर (rn) {

        /* ctx can be a list after NGX_RESOLVE_CNAME */
        साठी (last = ctx; last->next; last = last->next);

        जर (rn->valid >= ngx_time()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0, "resolve cached");

            ngx_queue_remove(&rn->queue);

            rn->expire = ngx_time() + r->expire;

            ngx_queue_घाला_head(expire_queue, &rn->queue);

            naddrs = (rn->naddrs == (u_short) -1) ? 0 : rn->naddrs;
#जर (NGX_HAVE_INET6)
            naddrs += (rn->naddrs6 == (u_short) -1) ? 0 : rn->naddrs6;
#endif

            जर (naddrs) {

                जर (naddrs == 1 && rn->naddrs == 1) {
                    addrs = रिक्त;

                } अन्यथा {
                    addrs = ngx_resolver_export(r, rn, 1);
                    जर (addrs == रिक्त) {
                        परत NGX_ERROR;
                    }
                }

                last->next = rn->waiting;
                rn->waiting = रिक्त;

                /* unlock name mutex */

                करा {
                    ctx->state = NGX_OK;
                    ctx->valid = rn->valid;
                    ctx->naddrs = naddrs;

                    जर (addrs == रिक्त) {
                        ctx->addrs = &ctx->addr;
                        ctx->addr.sockaddr = (संरचना sockaddr *) &ctx->sin;
                        ctx->addr.socklen = आकार(संरचना sockaddr_in);
                        ngx_memzero(&ctx->sin, आकार(संरचना sockaddr_in));
                        ctx->sin.sin_family = AF_INET;
                        ctx->sin.sin_addr.s_addr = rn->u.addr;

                    } अन्यथा {
                        ctx->addrs = addrs;
                    }

                    next = ctx->next;

                    ctx->handler(ctx);

                    ctx = next;
                } पर्यंत (ctx);

                जर (addrs != रिक्त) {
                    ngx_resolver_free(r, addrs->sockaddr);
                    ngx_resolver_free(r, addrs);
                }

                परत NGX_OK;
            }

            जर (rn->nsrvs) {
                last->next = rn->waiting;
                rn->waiting = रिक्त;

                /* unlock name mutex */

                करा {
                    next = ctx->next;

                    ngx_resolver_resolve_srv_names(ctx, rn);

                    ctx = next;
                } पर्यंत (ctx);

                परत NGX_OK;
            }

            /* NGX_RESOLVE_CNAME */

            जर (ctx->recursion++ < NGX_RESOLVER_MAX_RECURSION) {

                cname.len = rn->cnlen;
                cname.data = rn->u.cname;

                परत ngx_resolve_name_locked(r, ctx, &cname);
            }

            last->next = rn->waiting;
            rn->waiting = रिक्त;

            /* unlock name mutex */

            करा {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
                next = ctx->next;

                ctx->handler(ctx);

                ctx = next;
            } पर्यंत (ctx);

            परत NGX_OK;
        }

        जर (rn->waiting) {
            जर (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
                परत NGX_ERROR;
            }

            last->next = rn->waiting;
            rn->waiting = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;

            करा {
                ctx->गाठ = rn;
                ctx = ctx->next;
            } पर्यंत (ctx);

            परत NGX_AGAIN;
        }

        ngx_queue_remove(&rn->queue);

        /* lock alloc mutex */

        जर (rn->query) {
            ngx_resolver_free_locked(r, rn->query);
            rn->query = रिक्त;
#जर (NGX_HAVE_INET6)
            rn->query6 = रिक्त;
#endif
        }

        जर (rn->cnlen) {
            ngx_resolver_free_locked(r, rn->u.cname);
        }

        जर (rn->naddrs > 1 && rn->naddrs != (u_short) -1) {
            ngx_resolver_free_locked(r, rn->u.addrs);
        }

#जर (NGX_HAVE_INET6)
        जर (rn->naddrs6 > 1 && rn->naddrs6 != (u_short) -1) {
            ngx_resolver_free_locked(r, rn->u6.addrs6);
        }
#endif

        जर (rn->nsrvs) {
            साठी (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
                जर (rn->u.srvs[i].name.data) {
                    ngx_resolver_free_locked(r, rn->u.srvs[i].name.data);
                }
            }

            ngx_resolver_free_locked(r, rn->u.srvs);
        }

        /* unlock alloc mutex */

    } अन्यथा {

        rn = ngx_resolver_alloc(r, आकार(ngx_resolver_गाठ_t));
        जर (rn == रिक्त) {
            परत NGX_ERROR;
        }

        rn->name = ngx_resolver_dup(r, name->data, name->len);
        जर (rn->name == रिक्त) {
            ngx_resolver_free(r, rn);
            परत NGX_ERROR;
        }

        rn->गाठ.किल्ली = hash;
        rn->nlen = (u_short) name->len;
        rn->query = रिक्त;
#जर (NGX_HAVE_INET6)
        rn->query6 = रिक्त;
#endif

        ngx_rbtree_घाला(वृक्ष, &rn->गाठ);
    }

    जर (ctx->service.len) {
        rc = ngx_resolver_create_srv_query(r, rn, name);

    } अन्यथा {
        rc = ngx_resolver_create_name_query(r, rn, name);
    }

    जर (rc == NGX_ERROR) {
        जा failed;
    }

    जर (rc == NGX_DECLINED) {
        ngx_rbtree_delete(वृक्ष, &rn->गाठ);

        ngx_resolver_free(r, rn->query);
        ngx_resolver_free(r, rn->name);
        ngx_resolver_free(r, rn);

        करा {
            ctx->state = NGX_RESOLVE_NXDOMAIN;
            next = ctx->next;

            ctx->handler(ctx);

            ctx = next;
        } पर्यंत (ctx);

        परत NGX_OK;
    }

    rn->last_connection = r->last_connection++;
    जर (r->last_connection == r->connections.nelts) {
        r->last_connection = 0;
    }

    rn->naddrs = r->ipv4 ? (u_short) -1 : 0;
    rn->tcp = 0;
#जर (NGX_HAVE_INET6)
    rn->naddrs6 = r->ipv6 ? (u_short) -1 : 0;
    rn->tcp6 = 0;
#endif
    rn->nsrvs = 0;

    जर (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->last_connection++;
        जर (rn->last_connection == r->connections.nelts) {
            rn->last_connection = 0;
        }

        (शून्य) ngx_resolver_send_query(r, rn);
    }

    जर (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
        जा failed;
    }

    जर (ngx_resolver_resend_empty(r)) {
        ngx_add_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_time() + r->resend_timeout;

    ngx_queue_घाला_head(resend_queue, &rn->queue);

    rn->code = 0;
    rn->cnlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_मूल्य;
    rn->waiting = ctx;

    ctx->state = NGX_AGAIN;
    ctx->async = 1;

    करा {
        ctx->गाठ = rn;
        ctx = ctx->next;
    } पर्यंत (ctx);

    परत NGX_AGAIN;

failed:

    ngx_rbtree_delete(वृक्ष, &rn->गाठ);

    जर (rn->query) {
        ngx_resolver_free(r, rn->query);
    }

    ngx_resolver_free(r, rn->name);

    ngx_resolver_free(r, rn);

    परत NGX_ERROR;
}


ngx_int_t
ngx_resolve_addr(ngx_resolver_ctx_t *ctx)
{
    u_char               *name;
    in_addr_t             addr;
    ngx_queue_t          *resend_queue, *expire_queue;
    ngx_rbtree_t         *वृक्ष;
    ngx_resolver_t       *r;
    संरचना sockaddr_in   *sin;
    ngx_resolver_गाठ_t  *rn;
#जर (NGX_HAVE_INET6)
    uint32_t              hash;
    संरचना sockaddr_in6  *sin6;
#endif

#जर (NGX_SUPPRESS_WARN)
    addr = 0;
#जर (NGX_HAVE_INET6)
    hash = 0;
    sin6 = रिक्त;
#endif
#endif

    r = ctx->resolver;

    निवड (ctx->addr.sockaddr->sa_family) {

#जर (NGX_HAVE_INET6)
    स्थिती AF_INET6:
        sin6 = (संरचना sockaddr_in6 *) ctx->addr.sockaddr;
        hash = ngx_crc32_short(sin6->sin6_addr.s6_addr, 16);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr6(r, &sin6->sin6_addr, hash);

        वृक्ष = &r->addr6_rbtree;
        resend_queue = &r->addr6_resend_queue;
        expire_queue = &r->addr6_expire_queue;

        थांबा;
#endif

    पूर्वनिर्धारित: /* AF_INET */
        sin = (संरचना sockaddr_in *) ctx->addr.sockaddr;
        addr = ntohl(sin->sin_addr.s_addr);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        वृक्ष = &r->addr_rbtree;
        resend_queue = &r->addr_resend_queue;
        expire_queue = &r->addr_expire_queue;
    }

    जर (rn) {

        जर (rn->valid >= ngx_time()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0, "resolve cached");

            ngx_queue_remove(&rn->queue);

            rn->expire = ngx_time() + r->expire;

            ngx_queue_घाला_head(expire_queue, &rn->queue);

            name = ngx_resolver_dup(r, rn->name, rn->nlen);
            जर (name == रिक्त) {
                ngx_resolver_free(r, ctx);
                परत NGX_ERROR;
            }

            ctx->name.len = rn->nlen;
            ctx->name.data = name;

            /* unlock addr mutex */

            ctx->state = NGX_OK;
            ctx->valid = rn->valid;

            ctx->handler(ctx);

            ngx_resolver_free(r, name);

            परत NGX_OK;
        }

        जर (rn->waiting) {
            जर (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
                परत NGX_ERROR;
            }

            ctx->next = rn->waiting;
            rn->waiting = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;
            ctx->गाठ = rn;

            /* unlock addr mutex */

            परत NGX_OK;
        }

        ngx_queue_remove(&rn->queue);

        ngx_resolver_free(r, rn->query);
        rn->query = रिक्त;
#जर (NGX_HAVE_INET6)
        rn->query6 = रिक्त;
#endif

    } अन्यथा {
        rn = ngx_resolver_alloc(r, आकार(ngx_resolver_गाठ_t));
        जर (rn == रिक्त) {
            जा failed;
        }

        निवड (ctx->addr.sockaddr->sa_family) {

#जर (NGX_HAVE_INET6)
        स्थिती AF_INET6:
            rn->addr6 = sin6->sin6_addr;
            rn->गाठ.किल्ली = hash;
            थांबा;
#endif

        पूर्वनिर्धारित: /* AF_INET */
            rn->गाठ.किल्ली = addr;
        }

        rn->query = रिक्त;
#जर (NGX_HAVE_INET6)
        rn->query6 = रिक्त;
#endif

        ngx_rbtree_घाला(वृक्ष, &rn->गाठ);
    }

    जर (ngx_resolver_create_addr_query(r, rn, &ctx->addr) != NGX_OK) {
        जा failed;
    }

    rn->last_connection = r->last_connection++;
    जर (r->last_connection == r->connections.nelts) {
        r->last_connection = 0;
    }

    rn->naddrs = (u_short) -1;
    rn->tcp = 0;
#जर (NGX_HAVE_INET6)
    rn->naddrs6 = (u_short) -1;
    rn->tcp6 = 0;
#endif
    rn->nsrvs = 0;

    जर (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->last_connection++;
        जर (rn->last_connection == r->connections.nelts) {
            rn->last_connection = 0;
        }

        (शून्य) ngx_resolver_send_query(r, rn);
    }

    जर (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
        जा failed;
    }

    जर (ngx_resolver_resend_empty(r)) {
        ngx_add_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_time() + r->resend_timeout;

    ngx_queue_घाला_head(resend_queue, &rn->queue);

    rn->code = 0;
    rn->cnlen = 0;
    rn->name = रिक्त;
    rn->nlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_मूल्य;
    rn->waiting = ctx;

    /* unlock addr mutex */

    ctx->state = NGX_AGAIN;
    ctx->async = 1;
    ctx->गाठ = rn;

    परत NGX_OK;

failed:

    जर (rn) {
        ngx_rbtree_delete(वृक्ष, &rn->गाठ);

        जर (rn->query) {
            ngx_resolver_free(r, rn->query);
        }

        ngx_resolver_free(r, rn);
    }

    /* unlock addr mutex */

    जर (ctx->event) {
        ngx_resolver_free(r, ctx->event);
    }

    ngx_resolver_free(r, ctx);

    परत NGX_ERROR;
}


शून्य
ngx_resolve_addr_done(ngx_resolver_ctx_t *ctx)
{
    ngx_queue_t          *expire_queue;
    ngx_rbtree_t         *वृक्ष;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_गाठ_t  *rn;

    r = ctx->resolver;

    निवड (ctx->addr.sockaddr->sa_family) {

#जर (NGX_HAVE_INET6)
    स्थिती AF_INET6:
        वृक्ष = &r->addr6_rbtree;
        expire_queue = &r->addr6_expire_queue;
        थांबा;
#endif

    पूर्वनिर्धारित: /* AF_INET */
        वृक्ष = &r->addr_rbtree;
        expire_queue = &r->addr_expire_queue;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve addr done: %i", ctx->state);

    जर (ctx->event && ctx->event->timer_set) {
        ngx_del_timer(ctx->event);
    }

    /* lock addr mutex */

    जर (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->गाठ;

        जर (rn) {
            p = &rn->waiting;
            w = rn->waiting;

            पर्यंत (w) {
                जर (w == ctx) {
                    *p = w->next;

                    जा done;
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

    ngx_resolver_expire(r, वृक्ष, expire_queue);

    /* unlock addr mutex */

    /* lock alloc mutex */

    जर (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    जर (r->event->timer_set && ngx_resolver_resend_empty(r)) {
        ngx_del_timer(r->event);
    }
}


स्थिर शून्य
ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *वृक्ष, ngx_queue_t *queue)
{
    समय_प्रकार                now;
    ngx_uint_t            i;
    ngx_queue_t          *q;
    ngx_resolver_गाठ_t  *rn;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver expire");

    now = ngx_time();

    साठी (i = 0; i < 2; i++) {
        जर (ngx_queue_empty(queue)) {
            परत;
        }

        q = ngx_queue_last(queue);

        rn = ngx_queue_data(q, ngx_resolver_गाठ_t, queue);

        जर (now <= rn->expire) {
            परत;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver expire \"%*s\"", (माप_प्रकार) rn->nlen, rn->name);

        ngx_queue_remove(q);

        ngx_rbtree_delete(वृक्ष, &rn->गाठ);

        ngx_resolver_free_गाठ(r, rn);
    }
}


स्थिर ngx_int_t
ngx_resolver_send_query(ngx_resolver_t *r, ngx_resolver_गाठ_t *rn)
{
    ngx_int_t                   rc;
    ngx_resolver_connection_t  *rec;

    rec = r->connections.elts;
    rec = &rec[rn->last_connection];

    जर (rec->log.handler == रिक्त) {
        rec->log = *r->log;
        rec->log.handler = ngx_resolver_log_error;
        rec->log.data = rec;
        rec->log.action = "resolving";
    }

    जर (rn->query && rn->naddrs == (u_short) -1) {
        rc = rn->tcp ? ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen)
                     : ngx_resolver_send_udp_query(r, rec, rn->query, rn->qlen);

        जर (rc != NGX_OK) {
            परत rc;
        }
    }

#जर (NGX_HAVE_INET6)

    जर (rn->query6 && rn->naddrs6 == (u_short) -1) {
        rc = rn->tcp6
                    ? ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen)
                    : ngx_resolver_send_udp_query(r, rec, rn->query6, rn->qlen);

        जर (rc != NGX_OK) {
            परत rc;
        }
    }

#endif

    परत NGX_OK;
}


स्थिर ngx_int_t
ngx_resolver_send_udp_query(ngx_resolver_t *r, ngx_resolver_connection_t  *rec,
    u_char *query, u_short qlen)
{
    ssize_t  n;

    जर (rec->udp == रिक्त) {
        जर (ngx_udp_connect(rec) != NGX_OK) {
            परत NGX_ERROR;
        }

        rec->udp->data = rec;
        rec->udp->पठ->handler = ngx_resolver_udp_read;
        rec->udp->पठ->resolver = 1;
    }

    n = ngx_send(rec->udp, query, qlen);

    जर (n == NGX_ERROR) {
        जा failed;
    }

    जर ((माप_प्रकार) n != (माप_प्रकार) qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->log, 0, "send() incomplete");
        जा failed;
    }

    परत NGX_OK;

failed:

    ngx_close_connection(rec->udp);
    rec->udp = रिक्त;

    परत NGX_ERROR;
}


स्थिर ngx_int_t
ngx_resolver_send_tcp_query(ngx_resolver_t *r, ngx_resolver_connection_t *rec,
    u_char *query, u_short qlen)
{
    ngx_buf_t  *b;
    ngx_int_t   rc;

    rc = NGX_OK;

    जर (rec->tcp == रिक्त) {
        b = rec->read_buf;

        जर (b == रिक्त) {
            b = ngx_resolver_calloc(r, आकार(ngx_buf_t));
            जर (b == रिक्त) {
                परत NGX_ERROR;
            }

            b->start = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_RSIZE);
            जर (b->start == रिक्त) {
                ngx_resolver_free(r, b);
                परत NGX_ERROR;
            }

            b->end = b->start + NGX_RESOLVER_TCP_RSIZE;

            rec->read_buf = b;
        }

        b->pos = b->start;
        b->last = b->start;

        b = rec->write_buf;

        जर (b == रिक्त) {
            b = ngx_resolver_calloc(r, आकार(ngx_buf_t));
            जर (b == रिक्त) {
                परत NGX_ERROR;
            }

            b->start = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_WSIZE);
            जर (b->start == रिक्त) {
                ngx_resolver_free(r, b);
                परत NGX_ERROR;
            }

            b->end = b->start + NGX_RESOLVER_TCP_WSIZE;

            rec->write_buf = b;
        }

        b->pos = b->start;
        b->last = b->start;

        rc = ngx_tcp_connect(rec);
        जर (rc == NGX_ERROR) {
            परत NGX_ERROR;
        }

        rec->tcp->data = rec;
        rec->tcp->लिख->handler = ngx_resolver_tcp_write;
        rec->tcp->लिख->cancelable = 1;
        rec->tcp->पठ->handler = ngx_resolver_tcp_read;
        rec->tcp->पठ->resolver = 1;

        ngx_add_timer(rec->tcp->लिख, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    b = rec->write_buf;

    जर (b->end - b->last <  2 + qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->log, 0, "buffer overflow");
        परत NGX_ERROR;
    }

    *b->last++ = (u_char) (qlen >> 8);
    *b->last++ = (u_char) qlen;
    b->last = ngx_cpymem(b->last, query, qlen);

    जर (rc == NGX_OK) {
        ngx_resolver_tcp_write(rec->tcp->लिख);
    }

    परत NGX_OK;
}


स्थिर शून्य
ngx_resolver_resend_handler(ngx_event_t *ev)
{
    समय_प्रकार           timer, atimer, stimer, ntimer;
#जर (NGX_HAVE_INET6)
    समय_प्रकार           a6timer;
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

#जर (NGX_HAVE_INET6)

    /* lock addr6 mutex */

    a6timer = ngx_resolver_resend(r, &r->addr6_rbtree, &r->addr6_resend_queue);

    /* unlock addr6 mutex */

#endif

    timer = ntimer;

    जर (timer == 0) {
        timer = atimer;

    } अन्यथा जर (atimer) {
        timer = ngx_min(timer, atimer);
    }

    जर (timer == 0) {
        timer = stimer;

    } अन्यथा जर (stimer) {
        timer = ngx_min(timer, stimer);
    }

#जर (NGX_HAVE_INET6)

    जर (timer == 0) {
        timer = a6timer;

    } अन्यथा जर (a6timer) {
        timer = ngx_min(timer, a6timer);
    }

#endif

    जर (timer) {
        ngx_add_timer(r->event, (ngx_msec_t) (timer * 1000));
    }
}


स्थिर समय_प्रकार
ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *वृक्ष, ngx_queue_t *queue)
{
    समय_प्रकार                now;
    ngx_queue_t          *q;
    ngx_resolver_गाठ_t  *rn;

    now = ngx_time();

    साठी ( ;; ) {
        जर (ngx_queue_empty(queue)) {
            परत 0;
        }

        q = ngx_queue_last(queue);

        rn = ngx_queue_data(q, ngx_resolver_गाठ_t, queue);

        जर (now < rn->expire) {
            परत rn->expire - now;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver resend \"%*s\" %p",
                       (माप_प्रकार) rn->nlen, rn->name, rn->waiting);

        ngx_queue_remove(q);

        जर (rn->waiting) {

            जर (++rn->last_connection == r->connections.nelts) {
                rn->last_connection = 0;
            }

            (शून्य) ngx_resolver_send_query(r, rn);

            rn->expire = now + r->resend_timeout;

            ngx_queue_घाला_head(queue, q);

            सुरूठेवा;
        }

        ngx_rbtree_delete(वृक्ष, &rn->गाठ);

        ngx_resolver_free_गाठ(r, rn);
    }
}


स्थिर ngx_uint_t
ngx_resolver_resend_empty(ngx_resolver_t *r)
{
    परत ngx_queue_empty(&r->name_resend_queue)
           && ngx_queue_empty(&r->srv_resend_queue)
#जर (NGX_HAVE_INET6)
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

    करा {
        n = ngx_udp_recv(c, buf, NGX_RESOLVER_UDP_SIZE);

        जर (n == NGX_AGAIN) {
            थांबा;
        }

        जर (n == NGX_ERROR) {
            जा failed;
        }

        ngx_resolver_process_response(rec->resolver, buf, n, 0);

    } पर्यंत (rev->ready);

    जर (ngx_handle_read_event(rev, 0) != NGX_OK) {
        जा failed;
    }

    परत;

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

    जर (wev->timedout) {
        जा failed;
    }

    sent = c->sent;

    पर्यंत (wev->ready && b->pos < b->last) {
        n = ngx_send(c, b->pos, b->last - b->pos);

        जर (n == NGX_AGAIN) {
            थांबा;
        }

        जर (n == NGX_ERROR) {
            जा failed;
        }

        b->pos += n;
    }

    जर (b->pos != b->start) {
        b->last = ngx_movemem(b->start, b->pos, b->last - b->pos);
        b->pos = b->start;
    }

    जर (c->sent != sent) {
        ngx_add_timer(wev, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    जर (ngx_handle_write_event(wev, 0) != NGX_OK) {
        जा failed;
    }

    परत;

failed:

    ngx_close_connection(c);
    rec->tcp = रिक्त;
}


स्थिर शून्य
ngx_resolver_tcp_read(ngx_event_t *rev)
{
    u_char                     *p;
    माप_प्रकार                      size;
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

    पर्यंत (rev->ready) {
        n = ngx_recv(c, b->last, b->end - b->last);

        जर (n == NGX_AGAIN) {
            थांबा;
        }

        जर (n == NGX_ERROR || n == 0) {
            जा failed;
        }

        b->last += n;

        साठी ( ;; ) {
            p = b->pos;
            size = b->last - p;

            जर (size < 2) {
                थांबा;
            }

            qlen = (u_short) *p++ << 8;
            qlen += *p++;

            जर (size < (माप_प्रकार) (2 + qlen)) {
                थांबा;
            }

            ngx_resolver_process_response(r, p, qlen, 1);

            b->pos += 2 + qlen;
        }

        जर (b->pos != b->start) {
            b->last = ngx_movemem(b->start, b->pos, b->last - b->pos);
            b->pos = b->start;
        }
    }

    जर (ngx_handle_read_event(rev, 0) != NGX_OK) {
        जा failed;
    }

    परत;

failed:

    ngx_close_connection(c);
    rec->tcp = रिक्त;
}


स्थिर शून्य
ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf, माप_प्रकार n,
    ngx_uint_t tcp)
{
    अक्षर                 *err;
    ngx_uint_t            i, बार, ident, qident, flags, code, nqs, nan, trunc,
                          qtype, qclass;
#जर (NGX_HAVE_INET6)
    ngx_uint_t            qident6;
#endif
    ngx_queue_t          *q;
    ngx_resolver_qs_t    *qs;
    ngx_resolver_hdr_t   *response;
    ngx_resolver_गाठ_t  *rn;

    जर (n < आकार(ngx_resolver_hdr_t)) {
        जा short_response;
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
    जर ((flags & 0xf870) != 0x8000 || (trunc && tcp)) {
        ngx_log_error(r->log_level, r->log, 0,
                      "invalid %s DNS response %ui fl:%04Xi",
                      tcp ? "TCP" : "UDP", ident, flags);
        परत;
    }

    code = flags & 0xf;

    जर (code == NGX_RESOLVE_FORMERR) {

        बार = 0;

        साठी (q = ngx_queue_head(&r->name_resend_queue);
             q != ngx_queue_sentinel(&r->name_resend_queue) && बार++ < 100;
             q = ngx_queue_next(q))
        {
            rn = ngx_queue_data(q, ngx_resolver_गाठ_t, queue);

            जर (rn->query) {
                qident = (rn->query[0] << 8) + rn->query[1];

                जर (qident == ident) {
                    जा dns_error_name;
                }
            }

#जर (NGX_HAVE_INET6)
            जर (rn->query6) {
                qident6 = (rn->query6[0] << 8) + rn->query6[1];

                जर (qident6 == ident) {
                    जा dns_error_name;
                }
            }
#endif
        }

        जा dns_error;
    }

    जर (code > NGX_RESOLVE_REFUSED) {
        जा dns_error;
    }

    जर (nqs != 1) {
        err = "invalid number of questions in DNS response";
        जा done;
    }

    i = आकार(ngx_resolver_hdr_t);

    पर्यंत (i < (ngx_uint_t) n) {

        जर (buf[i] & 0xc0) {
            err = "unexpected compression pointer in DNS response";
            जा done;
        }

        जर (buf[i] == '\0') {
            जा found;
        }

        i += 1 + buf[i];
    }

    जा short_response;

found:

    जर (i++ == आकार(ngx_resolver_hdr_t)) {
        err = "zero-length domain name in DNS response";
        जा done;
    }

    जर (i + आकार(ngx_resolver_qs_t) + nan * (2 + आकार(ngx_resolver_an_t))
        > (ngx_uint_t) n)
    {
        जा short_response;
    }

    qs = (ngx_resolver_qs_t *) &buf[i];

    qtype = (qs->type_hi << 8) + qs->type_lo;
    qclass = (qs->class_hi << 8) + qs->class_lo;

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver DNS response qt:%ui cl:%ui", qtype, qclass);

    जर (qclass != 1) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unknown query class %ui in DNS response", qclass);
        परत;
    }

    निवड (qtype) {

    स्थिती NGX_RESOLVE_A:
#जर (NGX_HAVE_INET6)
    स्थिती NGX_RESOLVE_AAAA:
#endif

        ngx_resolver_process_a(r, buf, n, ident, code, qtype, nan, trunc,
                               i + आकार(ngx_resolver_qs_t));

        थांबा;

    स्थिती NGX_RESOLVE_SRV:

        ngx_resolver_process_srv(r, buf, n, ident, code, nan, trunc,
                                 i + आकार(ngx_resolver_qs_t));

        थांबा;

    स्थिती NGX_RESOLVE_PTR:

        ngx_resolver_process_ptr(r, buf, n, ident, code, nan);

        थांबा;

    पूर्वनिर्धारित:
        ngx_log_error(r->log_level, r->log, 0,
                      "unknown query type %ui in DNS response", qtype);
        परत;
    }

    परत;

short_response:

    err = "short DNS response";

done:

    ngx_log_error(r->log_level, r->log, 0, err);

    परत;

dns_error_name:

    ngx_log_error(r->log_level, r->log, 0,
                  "DNS error (%ui: %s), query id:%ui, name:\"%*s\"",
                  code, ngx_resolver_strerror(code), ident,
                  (माप_प्रकार) rn->nlen, rn->name);
    परत;

dns_error:

    ngx_log_error(r->log_level, r->log, 0,
                  "DNS error (%ui: %s), query id:%ui",
                  code, ngx_resolver_strerror(code), ident);
    परत;
}


स्थिर शून्य
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
#जर (NGX_HAVE_INET6)
    संरचना in6_addr            *addr6;
#endif
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *next;
    ngx_resolver_गाठ_t        *rn;
    ngx_resolver_addr_t        *addrs;
    ngx_resolver_connection_t  *rec;

    जर (ngx_resolver_copy(r, &name, buf,
                          buf + आकार(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        परत;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver qs:%V", &name);

    hash = ngx_crc32_short(name.data, name.len);

    /* lock name mutex */

    rn = ngx_resolver_lookup_name(r, &name, hash);

    जर (rn == रिक्त) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.data);
        जा failed;
    }

    निवड (qtype) {

#जर (NGX_HAVE_INET6)
    स्थिती NGX_RESOLVE_AAAA:

        जर (rn->query6 == रिक्त || rn->naddrs6 != (u_short) -1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.data);
            जा failed;
        }

        जर (trunc && rn->tcp6) {
            ngx_resolver_free(r, name.data);
            जा failed;
        }

        qident = (rn->query6[0] << 8) + rn->query6[1];

        थांबा;
#endif

    पूर्वनिर्धारित: /* NGX_RESOLVE_A */

        जर (rn->query == रिक्त || rn->naddrs != (u_short) -1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.data);
            जा failed;
        }

        जर (trunc && rn->tcp) {
            ngx_resolver_free(r, name.data);
            जा failed;
        }

        qident = (rn->query[0] << 8) + rn->query[1];
    }

    जर (ident != qident) {
        ngx_log_error(r->log_level, r->log, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.data);
        जा failed;
    }

    ngx_resolver_free(r, name.data);

    जर (trunc) {

        ngx_queue_remove(&rn->queue);

        जर (rn->waiting == रिक्त) {
            ngx_rbtree_delete(&r->name_rbtree, &rn->गाठ);
            ngx_resolver_free_गाठ(r, rn);
            जा next;
        }

        rec = r->connections.elts;
        rec = &rec[rn->last_connection];

        निवड (qtype) {

#जर (NGX_HAVE_INET6)
        स्थिती NGX_RESOLVE_AAAA:

            rn->tcp6 = 1;

            (शून्य) ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen);

            थांबा;
#endif

        पूर्वनिर्धारित: /* NGX_RESOLVE_A */

            rn->tcp = 1;

            (शून्य) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);
        }

        rn->expire = ngx_time() + r->resend_timeout;

        ngx_queue_घाला_head(&r->name_resend_queue, &rn->queue);

        जा next;
    }

    जर (code == 0 && rn->code) {
        code = rn->code;
    }

    जर (code == 0 && nan == 0) {

#जर (NGX_HAVE_INET6)
        निवड (qtype) {

        स्थिती NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            जर (rn->naddrs == (u_short) -1) {
                जा next;
            }

            जर (rn->naddrs) {
                जा export;
            }

            थांबा;

        पूर्वनिर्धारित: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            जर (rn->naddrs6 == (u_short) -1) {
                जा next;
            }

            जर (rn->naddrs6) {
                जा export;
            }
        }
#endif

        code = NGX_RESOLVE_NXDOMAIN;
    }

    जर (code) {

#जर (NGX_HAVE_INET6)
        निवड (qtype) {

        स्थिती NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            जर (rn->naddrs == (u_short) -1) {
                rn->code = (u_char) code;
                जा next;
            }

            थांबा;

        पूर्वनिर्धारित: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            जर (rn->naddrs6 == (u_short) -1) {
                rn->code = (u_char) code;
                जा next;
            }
        }
#endif

        next = rn->waiting;
        rn->waiting = रिक्त;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(&r->name_rbtree, &rn->गाठ);

        /* unlock name mutex */

        पर्यंत (next) {
            ctx = next;
            ctx->state = code;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            next = ctx->next;

            ctx->handler(ctx);
        }

        ngx_resolver_free_गाठ(r, rn);

        परत;
    }

    i = ans;
    naddrs = 0;
    cname = रिक्त;

    साठी (a = 0; a < nan; a++) {

        start = i;

        पर्यंत (i < n) {

            जर (buf[i] & 0xc0) {
                i += 2;
                जा found;
            }

            जर (buf[i] == 0) {
                i++;
                जा test_length;
            }

            i += 1 + buf[i];
        }

        जा short_response;

    test_length:

        जर (i - start < 2) {
            err = "invalid name in DNS response";
            जा invalid;
        }

    found:

        जर (i + आकार(ngx_resolver_an_t) >= n) {
            जा short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        जर (class != 1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR class %ui in DNS response", class);
            जा failed;
        }

        जर (ttl < 0) {
            ttl = 0;
        }

        rn->ttl = ngx_min(rn->ttl, (uint32_t) ttl);

        i += आकार(ngx_resolver_an_t);

        निवड (type) {

        स्थिती NGX_RESOLVE_A:

            जर (qtype != NGX_RESOLVE_A) {
                err = "unexpected A record in DNS response";
                जा invalid;
            }

            जर (len != 4) {
                err = "invalid A record in DNS response";
                जा invalid;
            }

            जर (i + 4 > n) {
                जा short_response;
            }

            naddrs++;

            थांबा;

#जर (NGX_HAVE_INET6)
        स्थिती NGX_RESOLVE_AAAA:

            जर (qtype != NGX_RESOLVE_AAAA) {
                err = "unexpected AAAA record in DNS response";
                जा invalid;
            }

            जर (len != 16) {
                err = "invalid AAAA record in DNS response";
                जा invalid;
            }

            जर (i + 16 > n) {
                जा short_response;
            }

            naddrs++;

            थांबा;
#endif

        स्थिती NGX_RESOLVE_CNAME:

            cname = &buf[i];

            थांबा;

        स्थिती NGX_RESOLVE_DNAME:

            थांबा;

        पूर्वनिर्धारित:

            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver naddrs:%ui cname:%p ttl:%uD",
                   naddrs, cname, rn->ttl);

    जर (naddrs) {

        निवड (qtype) {

#जर (NGX_HAVE_INET6)
        स्थिती NGX_RESOLVE_AAAA:

            जर (naddrs == 1) {
                addr6 = &rn->u6.addr6;
                rn->naddrs6 = 1;

            } अन्यथा {
                addr6 = ngx_resolver_alloc(r, naddrs * आकार(संरचना in6_addr));
                जर (addr6 == रिक्त) {
                    जा failed;
                }

                rn->u6.addrs6 = addr6;
                rn->naddrs6 = (u_short) naddrs;
            }

#जर (NGX_SUPPRESS_WARN)
            addr = रिक्त;
#endif

            थांबा;
#endif

        पूर्वनिर्धारित: /* NGX_RESOLVE_A */

            जर (naddrs == 1) {
                addr = &rn->u.addr;
                rn->naddrs = 1;

            } अन्यथा {
                addr = ngx_resolver_alloc(r, naddrs * आकार(in_addr_t));
                जर (addr == रिक्त) {
                    जा failed;
                }

                rn->u.addrs = addr;
                rn->naddrs = (u_short) naddrs;
            }

#जर (NGX_HAVE_INET6 && NGX_SUPPRESS_WARN)
            addr6 = रिक्त;
#endif
        }

        j = 0;
        i = ans;

        साठी (a = 0; a < nan; a++) {

            साठी ( ;; ) {

                जर (buf[i] & 0xc0) {
                    i += 2;
                    थांबा;
                }

                जर (buf[i] == 0) {
                    i++;
                    थांबा;
                }

                i += 1 + buf[i];
            }

            an = (ngx_resolver_an_t *) &buf[i];

            type = (an->type_hi << 8) + an->type_lo;
            len = (an->len_hi << 8) + an->len_lo;

            i += आकार(ngx_resolver_an_t);

            जर (type == NGX_RESOLVE_A) {

                addr[j] = htonl((buf[i] << 24) + (buf[i + 1] << 16)
                                + (buf[i + 2] << 8) + (buf[i + 3]));

                जर (++j == naddrs) {

#जर (NGX_HAVE_INET6)
                    जर (rn->naddrs6 == (u_short) -1) {
                        जा next;
                    }
#endif

                    थांबा;
                }
            }

#जर (NGX_HAVE_INET6)
            अन्यथा जर (type == NGX_RESOLVE_AAAA) {

                ngx_memcpy(addr6[j].s6_addr, &buf[i], 16);

                जर (++j == naddrs) {

                    जर (rn->naddrs == (u_short) -1) {
                        जा next;
                    }

                    थांबा;
                }
            }
#endif

            i += len;
        }
    }

    निवड (qtype) {

#जर (NGX_HAVE_INET6)
    स्थिती NGX_RESOLVE_AAAA:

        जर (rn->naddrs6 == (u_short) -1) {
            rn->naddrs6 = 0;
        }

        थांबा;
#endif

    पूर्वनिर्धारित: /* NGX_RESOLVE_A */

        जर (rn->naddrs == (u_short) -1) {
            rn->naddrs = 0;
        }
    }

    जर (rn->naddrs != (u_short) -1
#जर (NGX_HAVE_INET6)
        && rn->naddrs6 != (u_short) -1
#endif
        && rn->naddrs
#जर (NGX_HAVE_INET6)
           + rn->naddrs6
#endif
           > 0)
    {

#जर (NGX_HAVE_INET6)
    export:
#endif

        naddrs = rn->naddrs;
#जर (NGX_HAVE_INET6)
        naddrs += rn->naddrs6;
#endif

        जर (naddrs == 1 && rn->naddrs == 1) {
            addrs = रिक्त;

        } अन्यथा {
            addrs = ngx_resolver_export(r, rn, 0);
            जर (addrs == रिक्त) {
                जा failed;
            }
        }

        ngx_queue_remove(&rn->queue);

        rn->valid = ngx_time() + (r->valid ? r->valid : (समय_प्रकार) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_घाला_head(&r->name_expire_queue, &rn->queue);

        next = rn->waiting;
        rn->waiting = रिक्त;

        /* unlock name mutex */

        पर्यंत (next) {
            ctx = next;
            ctx->state = NGX_OK;
            ctx->valid = rn->valid;
            ctx->naddrs = naddrs;

            जर (addrs == रिक्त) {
                ctx->addrs = &ctx->addr;
                ctx->addr.sockaddr = (संरचना sockaddr *) &ctx->sin;
                ctx->addr.socklen = आकार(संरचना sockaddr_in);
                ngx_memzero(&ctx->sin, आकार(संरचना sockaddr_in));
                ctx->sin.sin_family = AF_INET;
                ctx->sin.sin_addr.s_addr = rn->u.addr;

            } अन्यथा {
                ctx->addrs = addrs;
            }

            next = ctx->next;

            ctx->handler(ctx);
        }

        जर (addrs != रिक्त) {
            ngx_resolver_free(r, addrs->sockaddr);
            ngx_resolver_free(r, addrs);
        }

        ngx_resolver_free(r, rn->query);
        rn->query = रिक्त;
#जर (NGX_HAVE_INET6)
        rn->query6 = रिक्त;
#endif

        परत;
    }

    जर (cname) {

        /* CNAME only */

        जर (rn->naddrs == (u_short) -1
#जर (NGX_HAVE_INET6)
            || rn->naddrs6 == (u_short) -1
#endif
            )
        {
            जा next;
        }

        जर (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            जा failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_queue_remove(&rn->queue);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.data;

        rn->valid = ngx_time() + (r->valid ? r->valid : (समय_प्रकार) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_घाला_head(&r->name_expire_queue, &rn->queue);

        ngx_resolver_free(r, rn->query);
        rn->query = रिक्त;
#जर (NGX_HAVE_INET6)
        rn->query6 = रिक्त;
#endif

        ctx = rn->waiting;
        rn->waiting = रिक्त;

        जर (ctx) {

            जर (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                करा {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    next = ctx->next;

                    ctx->handler(ctx);

                    ctx = next;
                } पर्यंत (ctx);

                परत;
            }

            साठी (next = ctx; next; next = next->next) {
                next->गाठ = रिक्त;
            }

            (शून्य) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        परत;
    }

    ngx_log_error(r->log_level, r->log, 0,
                  "no A or CNAME types in DNS response");
    परत;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->log, 0, err);

    परत;

failed:

next:

    /* unlock name mutex */

    परत;
}


स्थिर शून्य
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
    ngx_resolver_ctx_t         *ctx, *next;
    ngx_resolver_srv_t         *srvs;
    ngx_resolver_गाठ_t        *rn;
    ngx_resolver_connection_t  *rec;

    जर (ngx_resolver_copy(r, &name, buf,
                          buf + आकार(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        परत;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver qs:%V", &name);

    hash = ngx_crc32_short(name.data, name.len);

    rn = ngx_resolver_lookup_srv(r, &name, hash);

    जर (rn == रिक्त || rn->query == रिक्त) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.data);
        जा failed;
    }

    जर (trunc && rn->tcp) {
        ngx_resolver_free(r, name.data);
        जा failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    जर (ident != qident) {
        ngx_log_error(r->log_level, r->log, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.data);
        जा failed;
    }

    ngx_resolver_free(r, name.data);

    जर (trunc) {

        ngx_queue_remove(&rn->queue);

        जर (rn->waiting == रिक्त) {
            ngx_rbtree_delete(&r->srv_rbtree, &rn->गाठ);
            ngx_resolver_free_गाठ(r, rn);
            परत;
        }

        rec = r->connections.elts;
        rec = &rec[rn->last_connection];

        rn->tcp = 1;

        (शून्य) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);

        rn->expire = ngx_time() + r->resend_timeout;

        ngx_queue_घाला_head(&r->srv_resend_queue, &rn->queue);

        परत;
    }

    जर (code == 0 && rn->code) {
        code = rn->code;
    }

    जर (code == 0 && nan == 0) {
        code = NGX_RESOLVE_NXDOMAIN;
    }

    जर (code) {
        next = rn->waiting;
        rn->waiting = रिक्त;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(&r->srv_rbtree, &rn->गाठ);

        पर्यंत (next) {
            ctx = next;
            ctx->state = code;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            next = ctx->next;

            ctx->handler(ctx);
        }

        ngx_resolver_free_गाठ(r, rn);

        परत;
    }

    i = ans;
    nsrvs = 0;
    cname = रिक्त;

    साठी (a = 0; a < nan; a++) {

        start = i;

        पर्यंत (i < n) {

            जर (buf[i] & 0xc0) {
                i += 2;
                जा found;
            }

            जर (buf[i] == 0) {
                i++;
                जा test_length;
            }

            i += 1 + buf[i];
        }

        जा short_response;

    test_length:

        जर (i - start < 2) {
            err = "invalid name DNS response";
            जा invalid;
        }

    found:

        जर (i + आकार(ngx_resolver_an_t) >= n) {
            जा short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        जर (class != 1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR class %ui in DNS response", class);
            जा failed;
        }

        जर (ttl < 0) {
            ttl = 0;
        }

        rn->ttl = ngx_min(rn->ttl, (uint32_t) ttl);

        i += आकार(ngx_resolver_an_t);

        निवड (type) {

        स्थिती NGX_RESOLVE_SRV:

            जर (i + 6 >= n) {
                जा short_response;
            }

            जर (ngx_resolver_copy(r, रिक्त, buf, &buf[i + 6], buf + n)
                != NGX_OK)
            {
                जा failed;
            }

            nsrvs++;

            थांबा;

        स्थिती NGX_RESOLVE_CNAME:

            cname = &buf[i];

            थांबा;

        स्थिती NGX_RESOLVE_DNAME:

            थांबा;

        पूर्वनिर्धारित:

            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver nsrvs:%ui cname:%p ttl:%uD",
                   nsrvs, cname, rn->ttl);

    जर (nsrvs) {

        srvs = ngx_resolver_calloc(r, nsrvs * आकार(ngx_resolver_srv_t));
        जर (srvs == रिक्त) {
            जा failed;
        }

        rn->u.srvs = srvs;
        rn->nsrvs = (u_short) nsrvs;

        j = 0;
        i = ans;

        साठी (a = 0; a < nan; a++) {

            साठी ( ;; ) {

                जर (buf[i] & 0xc0) {
                    i += 2;
                    थांबा;
                }

                जर (buf[i] == 0) {
                    i++;
                    थांबा;
                }

                i += 1 + buf[i];
            }

            an = (ngx_resolver_an_t *) &buf[i];

            type = (an->type_hi << 8) + an->type_lo;
            len = (an->len_hi << 8) + an->len_lo;

            i += आकार(ngx_resolver_an_t);

            जर (type == NGX_RESOLVE_SRV) {

                srvs[j].priority = (buf[i] << 8) + buf[i + 1];
                srvs[j].weight = (buf[i + 2] << 8) + buf[i + 3];

                जर (srvs[j].weight == 0) {
                    srvs[j].weight = 1;
                }

                srvs[j].port = (buf[i + 4] << 8) + buf[i + 5];

                जर (ngx_resolver_copy(r, &srvs[j].name, buf, &buf[i + 6],
                                      buf + n)
                    != NGX_OK)
                {
                    जा failed;
                }

                j++;
            }

            i += len;
        }

        ngx_sort(srvs, nsrvs, आकार(ngx_resolver_srv_t),
                 ngx_resolver_cmp_srvs);

        ngx_resolver_free(r, rn->query);
        rn->query = रिक्त;

        ngx_queue_remove(&rn->queue);

        rn->valid = ngx_time() + (r->valid ? r->valid : (समय_प्रकार) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_घाला_head(&r->srv_expire_queue, &rn->queue);

        next = rn->waiting;
        rn->waiting = रिक्त;

        पर्यंत (next) {
            ctx = next;
            next = ctx->next;

            ngx_resolver_resolve_srv_names(ctx, rn);
        }

        परत;
    }

    rn->nsrvs = 0;

    जर (cname) {

        /* CNAME only */

        जर (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            जा failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_queue_remove(&rn->queue);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.data;

        rn->valid = ngx_time() + (r->valid ? r->valid : (समय_प्रकार) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_घाला_head(&r->srv_expire_queue, &rn->queue);

        ngx_resolver_free(r, rn->query);
        rn->query = रिक्त;
#जर (NGX_HAVE_INET6)
        rn->query6 = रिक्त;
#endif

        ctx = rn->waiting;
        rn->waiting = रिक्त;

        जर (ctx) {

            जर (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                करा {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    next = ctx->next;

                    ctx->handler(ctx);

                    ctx = next;
                } पर्यंत (ctx);

                परत;
            }

            साठी (next = ctx; next; next = next->next) {
                next->गाठ = रिक्त;
            }

            (शून्य) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        परत;
    }

    ngx_log_error(r->log_level, r->log, 0, "no SRV type in DNS response");

    परत;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->log, 0, err);

    परत;

failed:

    /* unlock name mutex */

    परत;
}


स्थिर शून्य
ngx_resolver_resolve_srv_names(ngx_resolver_ctx_t *ctx, ngx_resolver_गाठ_t *rn)
{
    ngx_uint_t                i;
    ngx_resolver_t           *r;
    ngx_resolver_ctx_t       *cctx;
    ngx_resolver_srv_name_t  *srvs;

    r = ctx->resolver;

    ctx->गाठ = रिक्त;
    ctx->state = NGX_OK;
    ctx->valid = rn->valid;
    ctx->मोजणी = rn->nsrvs;

    srvs = ngx_resolver_calloc(r, rn->nsrvs * आकार(ngx_resolver_srv_name_t));
    जर (srvs == रिक्त) {
        जा failed;
    }

    ctx->srvs = srvs;
    ctx->nsrvs = rn->nsrvs;

    जर (ctx->event && ctx->event->timer_set) {
        ngx_del_timer(ctx->event);
    }

    साठी (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
        srvs[i].name.data = ngx_resolver_alloc(r, rn->u.srvs[i].name.len);
        जर (srvs[i].name.data == रिक्त) {
            जा failed;
        }

        srvs[i].name.len = rn->u.srvs[i].name.len;
        ngx_memcpy(srvs[i].name.data, rn->u.srvs[i].name.data,
                   srvs[i].name.len);

        cctx = ngx_resolve_start(r, रिक्त);
        जर (cctx == रिक्त) {
            जा failed;
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

        जर (ngx_resolve_name(cctx) == NGX_ERROR) {
            srvs[i].ctx = रिक्त;
            जा failed;
        }
    }

    परत;

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

    ctx->मोजणी--;
    ctx->async |= cctx->async;

    srv->ctx = रिक्त;
    srv->state = cctx->state;

    जर (cctx->naddrs) {

        ctx->valid = ngx_min(ctx->valid, cctx->valid);

        addrs = ngx_resolver_calloc(r, cctx->naddrs * आकार(ngx_addr_t));
        जर (addrs == रिक्त) {
            srv->state = NGX_ERROR;
            जा done;
        }

        sockaddr = ngx_resolver_alloc(r, cctx->naddrs * आकार(ngx_sockaddr_t));
        जर (sockaddr == रिक्त) {
            ngx_resolver_free(r, addrs);
            srv->state = NGX_ERROR;
            जा done;
        }

        साठी (i = 0; i < cctx->naddrs; i++) {
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

    जर (ctx->मोजणी == 0) {
        ngx_resolver_report_srv(r, ctx);
    }
}


स्थिर शून्य
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
    ngx_resolver_ctx_t   *ctx, *next;
    ngx_resolver_गाठ_t  *rn;
#जर (NGX_HAVE_INET6)
    uint32_t              hash;
    ngx_int_t             digit;
    संरचना in6_addr       addr6;
#endif

    जर (ngx_resolver_copy(r, &name, buf,
                          buf + आकार(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        परत;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver qs:%V", &name);

    /* AF_INET */

    addr = 0;
    i = आकार(ngx_resolver_hdr_t);

    साठी (mask = 0; mask < 32; mask += 8) {
        len = buf[i++];

        octet = ngx_atoi(&buf[i], len);
        जर (octet == NGX_ERROR || octet > 255) {
            जा invalid_in_addr_arpa;
        }

        addr += octet << mask;
        i += len;
    }

    जर (ngx_strcasecmp(&buf[i], (u_char *) "\7in-addr\4arpa") == 0) {
        i += आकार("\7in-addr\4arpa");

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        वृक्ष = &r->addr_rbtree;
        expire_queue = &r->addr_expire_queue;

        जा valid;
    }

invalid_in_addr_arpa:

#जर (NGX_HAVE_INET6)

    i = आकार(ngx_resolver_hdr_t);

    साठी (octet = 15; octet >= 0; octet--) {
        जर (buf[i++] != '\1') {
            जा invalid_ip6_arpa;
        }

        digit = ngx_hextoi(&buf[i++], 1);
        जर (digit == NGX_ERROR) {
            जा invalid_ip6_arpa;
        }

        addr6.s6_addr[octet] = (u_char) digit;

        जर (buf[i++] != '\1') {
            जा invalid_ip6_arpa;
        }

        digit = ngx_hextoi(&buf[i++], 1);
        जर (digit == NGX_ERROR) {
            जा invalid_ip6_arpa;
        }

        addr6.s6_addr[octet] += (u_char) (digit * 16);
    }

    जर (ngx_strcasecmp(&buf[i], (u_char *) "\3ip6\4arpa") == 0) {
        i += आकार("\3ip6\4arpa");

        /* lock addr mutex */

        hash = ngx_crc32_short(addr6.s6_addr, 16);
        rn = ngx_resolver_lookup_addr6(r, &addr6, hash);

        वृक्ष = &r->addr6_rbtree;
        expire_queue = &r->addr6_expire_queue;

        जा valid;
    }

invalid_ip6_arpa:
#endif

    ngx_log_error(r->log_level, r->log, 0,
                  "invalid in-addr.arpa or ip6.arpa name in DNS response");
    ngx_resolver_free(r, name.data);
    परत;

valid:

    जर (rn == रिक्त || rn->query == रिक्त) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.data);
        जा failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    जर (ident != qident) {
        ngx_log_error(r->log_level, r->log, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.data);
        जा failed;
    }

    ngx_resolver_free(r, name.data);

    जर (code == 0 && nan == 0) {
        code = NGX_RESOLVE_NXDOMAIN;
    }

    जर (code) {
        next = rn->waiting;
        rn->waiting = रिक्त;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(वृक्ष, &rn->गाठ);

        /* unlock addr mutex */

        पर्यंत (next) {
            ctx = next;
            ctx->state = code;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            next = ctx->next;

            ctx->handler(ctx);
        }

        ngx_resolver_free_गाठ(r, rn);

        परत;
    }

    i += आकार(ngx_resolver_qs_t);

    साठी (a = 0; a < nan; a++) {

        start = i;

        पर्यंत (i < n) {

            जर (buf[i] & 0xc0) {
                i += 2;
                जा found;
            }

            जर (buf[i] == 0) {
                i++;
                जा test_length;
            }

            i += 1 + buf[i];
        }

        जा short_response;

    test_length:

        जर (i - start < 2) {
            err = "invalid name in DNS response";
            जा invalid;
        }

    found:

        जर (i + आकार(ngx_resolver_an_t) >= n) {
            जा short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        जर (class != 1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR class %ui in DNS response", class);
            जा failed;
        }

        जर (ttl < 0) {
            ttl = 0;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                      "resolver qt:%ui cl:%ui len:%uz",
                      type, class, len);

        i += आकार(ngx_resolver_an_t);

        निवड (type) {

        स्थिती NGX_RESOLVE_PTR:

            जा ptr;

        स्थिती NGX_RESOLVE_CNAME:

            थांबा;

        पूर्वनिर्धारित:

            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->log, 0,
                  "no PTR type in DNS response");
    परत;

ptr:

    जर (ngx_resolver_copy(r, &name, buf, buf + i, buf + n) != NGX_OK) {
        जा failed;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver an:%V", &name);

    जर (name.len != (माप_प्रकार) rn->nlen
        || ngx_strncmp(name.data, rn->name, name.len) != 0)
    {
        जर (rn->nlen) {
            ngx_resolver_free(r, rn->name);
        }

        rn->nlen = (u_short) name.len;
        rn->name = name.data;

        name.data = ngx_resolver_dup(r, rn->name, name.len);
        जर (name.data == रिक्त) {
            जा failed;
        }
    }

    ngx_queue_remove(&rn->queue);

    rn->valid = ngx_time() + (r->valid ? r->valid : ttl);
    rn->expire = ngx_time() + r->expire;

    ngx_queue_घाला_head(expire_queue, &rn->queue);

    next = rn->waiting;
    rn->waiting = रिक्त;

    /* unlock addr mutex */

    पर्यंत (next) {
        ctx = next;
        ctx->state = NGX_OK;
        ctx->valid = rn->valid;
        ctx->name = name;
        next = ctx->next;

        ctx->handler(ctx);
    }

    ngx_resolver_free(r, name.data);

    परत;

short_response:

    err = "short DNS response";

invalid:

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->log, 0, err);

    परत;

failed:

    /* unlock addr mutex */

    परत;
}


स्थिर ngx_resolver_गाठ_t *
ngx_resolver_lookup_name(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_गाठ_t    *गाठ, *sentinel;
    ngx_resolver_गाठ_t  *rn;

    गाठ = r->name_rbtree.मूळ;
    sentinel = r->name_rbtree.sentinel;

    पर्यंत (गाठ != sentinel) {

        जर (hash < गाठ->किल्ली) {
            गाठ = गाठ->डावा;
            सुरूठेवा;
        }

        जर (hash > गाठ->किल्ली) {
            गाठ = गाठ->उजवा;
            सुरूठेवा;
        }

        /* hash == node->key */

        rn = ngx_resolver_गाठ(गाठ);

        rc = ngx_memn2cmp(name->data, rn->name, name->len, rn->nlen);

        जर (rc == 0) {
            परत rn;
        }

        गाठ = (rc < 0) ? गाठ->डावा : गाठ->उजवा;
    }

    /* not found */

    परत रिक्त;
}


स्थिर ngx_resolver_गाठ_t *
ngx_resolver_lookup_srv(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_गाठ_t    *गाठ, *sentinel;
    ngx_resolver_गाठ_t  *rn;

    गाठ = r->srv_rbtree.मूळ;
    sentinel = r->srv_rbtree.sentinel;

    पर्यंत (गाठ != sentinel) {

        जर (hash < गाठ->किल्ली) {
            गाठ = गाठ->डावा;
            सुरूठेवा;
        }

        जर (hash > गाठ->किल्ली) {
            गाठ = गाठ->उजवा;
            सुरूठेवा;
        }

        /* hash == node->key */

        rn = ngx_resolver_गाठ(गाठ);

        rc = ngx_memn2cmp(name->data, rn->name, name->len, rn->nlen);

        जर (rc == 0) {
            परत rn;
        }

        गाठ = (rc < 0) ? गाठ->डावा : गाठ->उजवा;
    }

    /* not found */

    परत रिक्त;
}


स्थिर ngx_resolver_गाठ_t *
ngx_resolver_lookup_addr(ngx_resolver_t *r, in_addr_t addr)
{
    ngx_rbtree_गाठ_t  *गाठ, *sentinel;

    गाठ = r->addr_rbtree.मूळ;
    sentinel = r->addr_rbtree.sentinel;

    पर्यंत (गाठ != sentinel) {

        जर (addr < गाठ->किल्ली) {
            गाठ = गाठ->डावा;
            सुरूठेवा;
        }

        जर (addr > गाठ->किल्ली) {
            गाठ = गाठ->उजवा;
            सुरूठेवा;
        }

        /* addr == node->key */

        परत ngx_resolver_गाठ(गाठ);
    }

    /* not found */

    परत रिक्त;
}


#जर (NGX_HAVE_INET6)

स्थिर ngx_resolver_गाठ_t *
ngx_resolver_lookup_addr6(ngx_resolver_t *r, संरचना in6_addr *addr,
    uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_गाठ_t    *गाठ, *sentinel;
    ngx_resolver_गाठ_t  *rn;

    गाठ = r->addr6_rbtree.मूळ;
    sentinel = r->addr6_rbtree.sentinel;

    पर्यंत (गाठ != sentinel) {

        जर (hash < गाठ->किल्ली) {
            गाठ = गाठ->डावा;
            सुरूठेवा;
        }

        जर (hash > गाठ->किल्ली) {
            गाठ = गाठ->उजवा;
            सुरूठेवा;
        }

        /* hash == node->key */

        rn = ngx_resolver_गाठ(गाठ);

        rc = ngx_memcmp(addr, &rn->addr6, 16);

        जर (rc == 0) {
            परत rn;
        }

        गाठ = (rc < 0) ? गाठ->डावा : गाठ->उजवा;
    }

    /* not found */

    परत रिक्त;
}

#endif


स्थिर शून्य
ngx_resolver_rbtree_घाला_मूल्य(ngx_rbtree_गाठ_t *temp,
    ngx_rbtree_गाठ_t *गाठ, ngx_rbtree_गाठ_t *sentinel)
{
    ngx_rbtree_गाठ_t    **p;
    ngx_resolver_गाठ_t   *rn, *rn_temp;

    साठी ( ;; ) {

        जर (गाठ->किल्ली < temp->किल्ली) {

            p = &temp->डावा;

        } अन्यथा जर (गाठ->किल्ली > temp->किल्ली) {

            p = &temp->उजवा;

        } अन्यथा { /* node->key == temp->key */

            rn = ngx_resolver_गाठ(गाठ);
            rn_temp = ngx_resolver_गाठ(temp);

            p = (ngx_memn2cmp(rn->name, rn_temp->name, rn->nlen, rn_temp->nlen)
                 < 0) ? &temp->डावा : &temp->उजवा;
        }

        जर (*p == sentinel) {
            थांबा;
        }

        temp = *p;
    }

    *p = गाठ;
    गाठ->parent = temp;
    गाठ->डावा = sentinel;
    गाठ->उजवा = sentinel;
    ngx_rbt_red(गाठ);
}


#जर (NGX_HAVE_INET6)

स्थिर शून्य
ngx_resolver_rbtree_घाला_addr6_मूल्य(ngx_rbtree_गाठ_t *temp,
    ngx_rbtree_गाठ_t *गाठ, ngx_rbtree_गाठ_t *sentinel)
{
    ngx_rbtree_गाठ_t    **p;
    ngx_resolver_गाठ_t   *rn, *rn_temp;

    साठी ( ;; ) {

        जर (गाठ->किल्ली < temp->किल्ली) {

            p = &temp->डावा;

        } अन्यथा जर (गाठ->किल्ली > temp->किल्ली) {

            p = &temp->उजवा;

        } अन्यथा { /* node->key == temp->key */

            rn = ngx_resolver_गाठ(गाठ);
            rn_temp = ngx_resolver_गाठ(temp);

            p = (ngx_memcmp(&rn->addr6, &rn_temp->addr6, 16)
                 < 0) ? &temp->डावा : &temp->उजवा;
        }

        जर (*p == sentinel) {
            थांबा;
        }

        temp = *p;
    }

    *p = गाठ;
    गाठ->parent = temp;
    गाठ->डावा = sentinel;
    गाठ->उजवा = sentinel;
    ngx_rbt_red(गाठ);
}

#endif


स्थिर ngx_int_t
ngx_resolver_create_name_query(ngx_resolver_t *r, ngx_resolver_गाठ_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    माप_प्रकार               len, nlen;
    ngx_uint_t           ident;
    ngx_resolver_qs_t   *qs;
    ngx_resolver_hdr_t  *query;

    nlen = name->len ? (1 + name->len + 1) : 1;

    len = आकार(ngx_resolver_hdr_t) + nlen + आकार(ngx_resolver_qs_t);

#जर (NGX_HAVE_INET6)
    p = ngx_resolver_alloc(r, len * (r->ipv4 + r->ipv6));
#अन्यथा
    p = ngx_resolver_alloc(r, len);
#endif
    जर (p == रिक्त) {
        परत NGX_ERROR;
    }

    rn->qlen = (u_short) len;
    rn->query = p;

#जर (NGX_HAVE_INET6)
    जर (r->ipv6) {
        rn->query6 = r->ipv4 ? (p + len) : p;
    }
#endif

    query = (ngx_resolver_hdr_t *) p;

    जर (r->ipv4) {
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

    p += आकार(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    /* query type */
    qs->type_hi = 0; qs->type_lo = NGX_RESOLVE_A;

    /* IN query class */
    qs->class_hi = 0; qs->class_lo = 1;

    /* convert "www.example.com" to "\3www\7example\3com\0" */

    len = 0;
    p--;
    *p-- = '\0';

    जर (name->len == 0)  {
        परत NGX_DECLINED;
    }

    साठी (s = name->data + name->len - 1; s >= name->data; s--) {
        जर (*s != '.') {
            *p = *s;
            len++;

        } अन्यथा {
            जर (len == 0 || len > 255) {
                परत NGX_DECLINED;
            }

            *p = (u_char) len;
            len = 0;
        }

        p--;
    }

    जर (len == 0 || len > 255) {
        परत NGX_DECLINED;
    }

    *p = (u_char) len;

#जर (NGX_HAVE_INET6)
    जर (!r->ipv6) {
        परत NGX_OK;
    }

    p = rn->query6;

    जर (r->ipv4) {
        ngx_memcpy(p, rn->query, rn->qlen);
    }

    query = (ngx_resolver_hdr_t *) p;

    ident = ngx_random();

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve: \"%V\" AAAA %i", name, ident & 0xffff);

    query->ident_hi = (u_char) ((ident >> 8) & 0xff);
    query->ident_lo = (u_char) (ident & 0xff);

    p += आकार(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    qs->type_lo = NGX_RESOLVE_AAAA;
#endif

    परत NGX_OK;
}


स्थिर ngx_int_t
ngx_resolver_create_srv_query(ngx_resolver_t *r, ngx_resolver_गाठ_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    माप_प्रकार               len, nlen;
    ngx_uint_t           ident;
    ngx_resolver_qs_t   *qs;
    ngx_resolver_hdr_t  *query;

    nlen = name->len ? (1 + name->len + 1) : 1;

    len = आकार(ngx_resolver_hdr_t) + nlen + आकार(ngx_resolver_qs_t);

    p = ngx_resolver_alloc(r, len);
    जर (p == रिक्त) {
        परत NGX_ERROR;
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

    p += आकार(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    /* query type */
    qs->type_hi = 0; qs->type_lo = NGX_RESOLVE_SRV;

    /* IN query class */
    qs->class_hi = 0; qs->class_lo = 1;

    /* converts "www.example.com" to "\3www\7example\3com\0" */

    len = 0;
    p--;
    *p-- = '\0';

    जर (name->len == 0)  {
        परत NGX_DECLINED;
    }

    साठी (s = name->data + name->len - 1; s >= name->data; s--) {
        जर (*s != '.') {
            *p = *s;
            len++;

        } अन्यथा {
            जर (len == 0 || len > 255) {
                परत NGX_DECLINED;
            }

            *p = (u_char) len;
            len = 0;
        }

        p--;
    }

    जर (len == 0 || len > 255) {
        परत NGX_DECLINED;
    }

    *p = (u_char) len;

    परत NGX_OK;
}


स्थिर ngx_int_t
ngx_resolver_create_addr_query(ngx_resolver_t *r, ngx_resolver_गाठ_t *rn,
    ngx_resolver_addr_t *addr)
{
    u_char               *p, *d;
    माप_प्रकार                len;
    in_addr_t             inaddr;
    ngx_int_t             n;
    ngx_uint_t            ident;
    ngx_resolver_hdr_t   *query;
    संरचना sockaddr_in   *sin;
#जर (NGX_HAVE_INET6)
    संरचना sockaddr_in6  *sin6;
#endif

    निवड (addr->sockaddr->sa_family) {

#जर (NGX_HAVE_INET6)
    स्थिती AF_INET6:
        len = आकार(ngx_resolver_hdr_t)
              + 64 + आकार(".ip6.arpa.") - 1
              + आकार(ngx_resolver_qs_t);

        थांबा;
#endif

    पूर्वनिर्धारित: /* AF_INET */
        len = आकार(ngx_resolver_hdr_t)
              + आकार(".255.255.255.255.in-addr.arpa.") - 1
              + आकार(ngx_resolver_qs_t);
    }

    p = ngx_resolver_alloc(r, len);
    जर (p == रिक्त) {
        परत NGX_ERROR;
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

    p += आकार(ngx_resolver_hdr_t);

    निवड (addr->sockaddr->sa_family) {

#जर (NGX_HAVE_INET6)
    स्थिती AF_INET6:
        sin6 = (संरचना sockaddr_in6 *) addr->sockaddr;

        साठी (n = 15; n >= 0; n--) {
            p = ngx_sprintf(p, "\1%xd\1%xd",
                            sin6->sin6_addr.s6_addr[n] & 0xf,
                            (sin6->sin6_addr.s6_addr[n] >> 4) & 0xf);
        }

        p = ngx_cpymem(p, "\3ip6\4arpa\0", 10);

        थांबा;
#endif

    पूर्वनिर्धारित: /* AF_INET */

        sin = (संरचना sockaddr_in *) addr->sockaddr;
        inaddr = ntohl(sin->sin_addr.s_addr);

        साठी (n = 0; n < 32; n += 8) {
            d = ngx_sprintf(&p[1], "%ud", (inaddr >> n) & 0xff);
            *p = (u_char) (d - &p[1]);
            p = d;
        }

        p = ngx_cpymem(p, "\7in-addr\4arpa\0", 14);
    }

    /* query type "PTR", IN query class */
    p = ngx_cpymem(p, "\0\14\0\1", 4);

    rn->qlen = (u_short) (p - rn->query);

    परत NGX_OK;
}


स्थिर ngx_int_t
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

    साठी (i = 0; i < 128; i++) {
        n = *p++;

        जर (n == 0) {
            जा done;
        }

        जर (n & 0xc0) {
            जर ((n & 0xc0) != 0xc0) {
                err = "invalid label type in DNS response";
                जा invalid;
            }

            जर (p >= last) {
                err = "name is out of DNS response";
                जा invalid;
            }

            n = ((n & 0x3f) << 8) + *p;
            p = &buf[n];

        } अन्यथा {
            len += 1 + n;
            p = &p[n];
        }

        जर (p >= last) {
            err = "name is out of DNS response";
            जा invalid;
        }
    }

    err = "compression pointers loop in DNS response";

invalid:

    ngx_log_error(r->log_level, r->log, 0, err);

    परत NGX_ERROR;

done:

    जर (name == रिक्त) {
        परत NGX_OK;
    }

    जर (len == 0) {
        ngx_str_null(name);
        परत NGX_OK;
    }

    dst = ngx_resolver_alloc(r, len);
    जर (dst == रिक्त) {
        परत NGX_ERROR;
    }

    name->data = dst;

    साठी ( ;; ) {
        n = *src++;

        जर (n == 0) {
            name->len = dst - name->data - 1;
            परत NGX_OK;
        }

        जर (n & 0xc0) {
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
ngx_resolver_set_timeout(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx)
{
    जर (ctx->event || ctx->timeout == 0) {
        परत NGX_OK;
    }

    ctx->event = ngx_resolver_calloc(r, आकार(ngx_event_t));
    जर (ctx->event == रिक्त) {
        परत NGX_ERROR;
    }

    ctx->event->handler = ngx_resolver_timeout_handler;
    ctx->event->data = ctx;
    ctx->event->log = r->log;
    ctx->event->cancelable = ctx->cancelable;
    ctx->ident = -1;

    ngx_add_timer(ctx->event, ctx->timeout);

    परत NGX_OK;
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
ngx_resolver_free_गाठ(ngx_resolver_t *r, ngx_resolver_गाठ_t *rn)
{
    ngx_uint_t  i;

    /* lock alloc mutex */

    जर (rn->query) {
        ngx_resolver_free_locked(r, rn->query);
    }

    जर (rn->name) {
        ngx_resolver_free_locked(r, rn->name);
    }

    जर (rn->cnlen) {
        ngx_resolver_free_locked(r, rn->u.cname);
    }

    जर (rn->naddrs > 1 && rn->naddrs != (u_short) -1) {
        ngx_resolver_free_locked(r, rn->u.addrs);
    }

#जर (NGX_HAVE_INET6)
    जर (rn->naddrs6 > 1 && rn->naddrs6 != (u_short) -1) {
        ngx_resolver_free_locked(r, rn->u6.addrs6);
    }
#endif

    जर (rn->nsrvs) {
        साठी (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
            जर (rn->u.srvs[i].name.data) {
                ngx_resolver_free_locked(r, rn->u.srvs[i].name.data);
            }
        }

        ngx_resolver_free_locked(r, rn->u.srvs);
    }

    ngx_resolver_free_locked(r, rn);

    /* unlock alloc mutex */
}


स्थिर शून्य *
ngx_resolver_alloc(ngx_resolver_t *r, माप_प्रकार size)
{
    u_char  *p;

    /* lock alloc mutex */

    p = ngx_alloc(size, r->log);

    /* unlock alloc mutex */

    परत p;
}


स्थिर शून्य *
ngx_resolver_calloc(ngx_resolver_t *r, माप_प्रकार size)
{
    u_char  *p;

    p = ngx_resolver_alloc(r, size);

    जर (p) {
        ngx_memzero(p, size);
    }

    परत p;
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
ngx_resolver_dup(ngx_resolver_t *r, शून्य *src, माप_प्रकार size)
{
    शून्य  *dst;

    dst = ngx_resolver_alloc(r, size);

    जर (dst == रिक्त) {
        परत dst;
    }

    ngx_memcpy(dst, src, size);

    परत dst;
}


स्थिर ngx_resolver_addr_t *
ngx_resolver_export(ngx_resolver_t *r, ngx_resolver_गाठ_t *rn,
    ngx_uint_t rotate)
{
    ngx_uint_t            d, i, j, n;
    in_addr_t            *addr;
    ngx_sockaddr_t       *sockaddr;
    संरचना sockaddr_in   *sin;
    ngx_resolver_addr_t  *dst;
#जर (NGX_HAVE_INET6)
    संरचना in6_addr      *addr6;
    संरचना sockaddr_in6  *sin6;
#endif

    n = rn->naddrs;
#जर (NGX_HAVE_INET6)
    n += rn->naddrs6;
#endif

    dst = ngx_resolver_calloc(r, n * आकार(ngx_resolver_addr_t));
    जर (dst == रिक्त) {
        परत रिक्त;
    }

    sockaddr = ngx_resolver_calloc(r, n * आकार(ngx_sockaddr_t));
    जर (sockaddr == रिक्त) {
        ngx_resolver_free(r, dst);
        परत रिक्त;
    }

    i = 0;
    d = rotate ? ngx_random() % n : 0;

    जर (rn->naddrs) {
        j = rotate ? ngx_random() % rn->naddrs : 0;

        addr = (rn->naddrs == 1) ? &rn->u.addr : rn->u.addrs;

        करा {
            sin = &sockaddr[d].sockaddr_in;
            sin->sin_family = AF_INET;
            sin->sin_addr.s_addr = addr[j++];
            dst[d].sockaddr = (संरचना sockaddr *) sin;
            dst[d++].socklen = आकार(संरचना sockaddr_in);

            जर (d == n) {
                d = 0;
            }

            जर (j == (ngx_uint_t) rn->naddrs) {
                j = 0;
            }
        } पर्यंत (++i < (ngx_uint_t) rn->naddrs);
    }

#जर (NGX_HAVE_INET6)
    जर (rn->naddrs6) {
        j = rotate ? ngx_random() % rn->naddrs6 : 0;

        addr6 = (rn->naddrs6 == 1) ? &rn->u6.addr6 : rn->u6.addrs6;

        करा {
            sin6 = &sockaddr[d].sockaddr_in6;
            sin6->sin6_family = AF_INET6;
            ngx_memcpy(sin6->sin6_addr.s6_addr, addr6[j++].s6_addr, 16);
            dst[d].sockaddr = (संरचना sockaddr *) sin6;
            dst[d++].socklen = आकार(संरचना sockaddr_in6);

            जर (d == n) {
                d = 0;
            }

            जर (j == rn->naddrs6) {
                j = 0;
            }
        } पर्यंत (++i < n);
    }
#endif

    परत dst;
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

    साठी (i = 0; i < nsrvs; i++) {
        जर (srvs[i].state == NGX_ERROR) {
            ctx->state = NGX_ERROR;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

            ctx->handler(ctx);
            परत;
        }

        naddrs += srvs[i].naddrs;
    }

    जर (naddrs == 0) {
        ctx->state = srvs[0].state;

        साठी (i = 0; i < nsrvs; i++) {
            जर (srvs[i].state == NGX_RESOLVE_NXDOMAIN) {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                थांबा;
            }
        }

        ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        परत;
    }

    addrs = ngx_resolver_calloc(r, naddrs * आकार(ngx_resolver_addr_t));
    जर (addrs == रिक्त) {
        ctx->state = NGX_ERROR;
        ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        परत;
    }

    i = 0;
    n = 0;

    करा {
        nw = 0;

        साठी (j = i; j < nsrvs; j++) {
            जर (srvs[j].priority != srvs[i].priority) {
                थांबा;
            }

            nw += srvs[j].naddrs * srvs[j].weight;
        }

        जर (nw == 0) {
            जा next_srv;
        }

        w = ngx_random() % nw;

        साठी (k = i; k < j; k++) {
            जर (w < srvs[k].naddrs * srvs[k].weight) {
                थांबा;
            }

            w -= srvs[k].naddrs * srvs[k].weight;
        }

        साठी (l = i; l < j; l++) {

            साठी (m = 0; m < srvs[k].naddrs; m++) {
                addrs[n].socklen = srvs[k].addrs[m].socklen;
                addrs[n].sockaddr = srvs[k].addrs[m].sockaddr;
                addrs[n].name = srvs[k].name;
                addrs[n].priority = srvs[k].priority;
                addrs[n].weight = srvs[k].weight;
                n++;
            }

            जर (++k == j) {
                k = i;
            }
        }

next_srv:

        i = j;

    } पर्यंत (i < ctx->nsrvs);

    ctx->state = NGX_OK;
    ctx->addrs = addrs;
    ctx->naddrs = naddrs;

    ctx->handler(ctx);

    ngx_resolver_free(r, addrs);
}


अक्षर *
ngx_resolver_strerror(ngx_int_t err)
{
    स्थिर अक्षर *errors[] = {
        "Format error",     /* FORMERR */
        "Server failure",   /* SERVFAIL */
        "Host not found",   /* NXDOMAIN */
        "Unimplemented",    /* NOTIMP */
        "Operation refused" /* REFUSED */
    };

    जर (err > 0 && err < 6) {
        परत errors[err - 1];
    }

    जर (err == NGX_RESOLVE_TIMEDOUT) {
        परत "Operation timed out";
    }

    परत "Unknown error";
}


स्थिर u_char *
ngx_resolver_log_error(ngx_log_t *log, u_char *buf, माप_प्रकार len)
{
    u_char                     *p;
    ngx_resolver_connection_t  *rec;

    p = buf;

    जर (log->action) {
        p = ngx_snprintf(buf, len, " while %s", log->action);
        len -= p - buf;
    }

    rec = log->data;

    जर (rec) {
        p = ngx_snprintf(p, len, ", resolver: %V", &rec->server);
    }

    परत p;
}


स्थिर ngx_int_t
ngx_udp_connect(ngx_resolver_connection_t *rec)
{
    पूर्णांक                rc;
    ngx_int_t          event;
    ngx_event_t       *rev, *wev;
    ngx_socket_t       s;
    ngx_connection_t  *c;

    s = ngx_socket(rec->sockaddr->sa_family, SOCK_DGRAM, 0);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "UDP socket %d", s);

    जर (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_socket_n " failed");
        परत NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->log);

    जर (c == रिक्त) {
        जर (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        परत NGX_ERROR;
    }

    जर (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        जा failed;
    }

    rev = c->पठ;
    wev = c->लिख;

    rev->log = &rec->log;
    wev->log = &rec->log;

    rec->udp = c;

    c->संख्या = ngx_atomic_fetch_add(ngx_connection_counter, 1);

    c->start_time = ngx_current_msec;

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->log, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->संख्या);

    rc = connect(s, rec->sockaddr, rec->socklen);

    जर (rc == -1) {
        ngx_log_error(NGX_LOG_CRIT, &rec->log, ngx_socket_errno,
                      "connect() failed");

        जा failed;
    }

    /* UDP sockets are always ready to write */
    wev->ready = 1;

    event = (ngx_event_flags & NGX_USE_CLEAR_EVENT) ?
                /* kqueue, epoll */                 NGX_CLEAR_EVENT:
                /* select, poll, /dev/poll */       NGX_LEVEL_EVENT;
                /* eventport event type has no meaning: oneshot only */

    जर (ngx_add_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        जा failed;
    }

    परत NGX_OK;

failed:

    ngx_close_connection(c);
    rec->udp = रिक्त;

    परत NGX_ERROR;
}


स्थिर ngx_int_t
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

    जर (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_socket_n " failed");
        परत NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->log);

    जर (c == रिक्त) {
        जर (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        परत NGX_ERROR;
    }

    जर (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        जा failed;
    }

    rev = c->पठ;
    wev = c->लिख;

    rev->log = &rec->log;
    wev->log = &rec->log;

    rec->tcp = c;

    c->संख्या = ngx_atomic_fetch_add(ngx_connection_counter, 1);

    c->start_time = ngx_current_msec;

    जर (ngx_add_conn) {
        जर (ngx_add_conn(c) == NGX_ERROR) {
            जा failed;
        }
    }

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->log, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->संख्या);

    rc = connect(s, rec->sockaddr, rec->socklen);

    जर (rc == -1) {
        err = ngx_socket_errno;


        जर (err != NGX_EINPROGRESS
#जर (NGX_WIN32)
            /* Winsock returns WSAEWOULDBLOCK (NGX_EAGAIN) */
            && err != NGX_EAGAIN
#endif
            )
        {
            जर (err == NGX_ECONNREFUSED
#जर (NGX_LINUX)
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

            } अन्यथा {
                level = NGX_LOG_CRIT;
            }

            ngx_log_error(level, &rec->log, err, "connect() to %V failed",
                          &rec->server);

            ngx_close_connection(c);
            rec->tcp = रिक्त;

            परत NGX_ERROR;
        }
    }

    जर (ngx_add_conn) {
        जर (rc == -1) {

            /* NGX_EINPROGRESS */

            परत NGX_AGAIN;
        }

        ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "connected");

        wev->ready = 1;

        परत NGX_OK;
    }

    जर (ngx_event_flags & NGX_USE_CLEAR_EVENT) {

        /* kqueue */

        event = NGX_CLEAR_EVENT;

    } अन्यथा {

        /* select, poll, /dev/poll */

        event = NGX_LEVEL_EVENT;
    }

    जर (ngx_add_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        जा failed;
    }

    जर (rc == -1) {

        /* NGX_EINPROGRESS */

        जर (ngx_add_event(wev, NGX_WRITE_EVENT, event) != NGX_OK) {
            जा failed;
        }

        परत NGX_AGAIN;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "connected");

    wev->ready = 1;

    परत NGX_OK;

failed:

    ngx_close_connection(c);
    rec->tcp = रिक्त;

    परत NGX_ERROR;
}


स्थिर ngx_int_t
ngx_resolver_cmp_srvs(स्थिरांक शून्य *one, स्थिरांक शून्य *two)
{
    ngx_int_t            p1, p2;
    ngx_resolver_srv_t  *first, *second;

    first = (ngx_resolver_srv_t *) one;
    second = (ngx_resolver_srv_t *) two;

    p1 = first->priority;
    p2 = second->priority;

    परत p1 - p2;
}
