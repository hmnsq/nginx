
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>
#അന്തര്ഭാവ <ngx_event.h>


#define NGX_RESOLVER_UDP_SIZE   4096

#define NGX_RESOLVER_TCP_RSIZE  (2 + 65535)
#define NGX_RESOLVER_TCP_WSIZE  8192


തരനിർവചനം ഘടന {
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


തരനിർവചനം ഘടന {
    u_char  type_hi;
    u_char  type_lo;
    u_char  class_hi;
    u_char  class_lo;
} ngx_resolver_qs_t;


തരനിർവചനം ഘടന {
    u_char  type_hi;
    u_char  type_lo;
    u_char  class_hi;
    u_char  class_lo;
    u_char  ttl[4];
    u_char  len_hi;
    u_char  len_lo;
} ngx_resolver_an_t;


#define ngx_resolver_കണ്ണി(n)  ngx_rbtree_data(n, ngx_resolver_കണ്ണി_t, കണ്ണി)


സ്ഥിതം ngx_int_t ngx_udp_connect(ngx_resolver_connection_t *rec);
സ്ഥിതം ngx_int_t ngx_tcp_connect(ngx_resolver_connection_t *rec);


സ്ഥിതം ശൂന്യം ngx_resolver_cleanup(ശൂന്യം *data);
സ്ഥിതം ശൂന്യം ngx_resolver_cleanup_വൃക്ഷം(ngx_resolver_t *r, ngx_rbtree_t *വൃക്ഷം);
സ്ഥിതം ngx_int_t ngx_resolve_name_locked(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx, ngx_str_t *name);
സ്ഥിതം ശൂന്യം ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *വൃക്ഷം,
    ngx_queue_t *queue);
സ്ഥിതം ngx_int_t ngx_resolver_send_query(ngx_resolver_t *r,
    ngx_resolver_കണ്ണി_t *rn);
സ്ഥിതം ngx_int_t ngx_resolver_send_udp_query(ngx_resolver_t *r,
    ngx_resolver_connection_t *rec, u_char *query, u_short qlen);
സ്ഥിതം ngx_int_t ngx_resolver_send_tcp_query(ngx_resolver_t *r,
    ngx_resolver_connection_t *rec, u_char *query, u_short qlen);
സ്ഥിതം ngx_int_t ngx_resolver_create_name_query(ngx_resolver_t *r,
    ngx_resolver_കണ്ണി_t *rn, ngx_str_t *name);
സ്ഥിതം ngx_int_t ngx_resolver_create_srv_query(ngx_resolver_t *r,
    ngx_resolver_കണ്ണി_t *rn, ngx_str_t *name);
സ്ഥിതം ngx_int_t ngx_resolver_create_addr_query(ngx_resolver_t *r,
    ngx_resolver_കണ്ണി_t *rn, ngx_resolver_addr_t *addr);
സ്ഥിതം ശൂന്യം ngx_resolver_resend_handler(ngx_event_t *ev);
സ്ഥിതം time_t ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *വൃക്ഷം,
    ngx_queue_t *queue);
സ്ഥിതം ngx_uint_t ngx_resolver_resend_empty(ngx_resolver_t *r);
സ്ഥിതം ശൂന്യം ngx_resolver_udp_read(ngx_event_t *rev);
സ്ഥിതം ശൂന്യം ngx_resolver_tcp_write(ngx_event_t *wev);
സ്ഥിതം ശൂന്യം ngx_resolver_tcp_read(ngx_event_t *rev);
സ്ഥിതം ശൂന്യം ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf,
    size_t n, ngx_uint_t tcp);
സ്ഥിതം ശൂന്യം ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans);
സ്ഥിതം ശൂന്യം ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans);
സ്ഥിതം ശൂന്യം ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan);
സ്ഥിതം ngx_resolver_കണ്ണി_t *ngx_resolver_lookup_name(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
സ്ഥിതം ngx_resolver_കണ്ണി_t *ngx_resolver_lookup_srv(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
സ്ഥിതം ngx_resolver_കണ്ണി_t *ngx_resolver_lookup_addr(ngx_resolver_t *r,
    in_addr_t addr);
സ്ഥിതം ശൂന്യം ngx_resolver_rbtree_തിരുകുക_മൂല്യം(ngx_rbtree_കണ്ണി_t *temp,
    ngx_rbtree_കണ്ണി_t *കണ്ണി, ngx_rbtree_കണ്ണി_t *sentinel);
സ്ഥിതം ngx_int_t ngx_resolver_copy(ngx_resolver_t *r, ngx_str_t *name,
    u_char *buf, u_char *src, u_char *last);
സ്ഥിതം ngx_int_t ngx_resolver_set_timeout(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx);
സ്ഥിതം ശൂന്യം ngx_resolver_timeout_handler(ngx_event_t *ev);
സ്ഥിതം ശൂന്യം ngx_resolver_free_കണ്ണി(ngx_resolver_t *r, ngx_resolver_കണ്ണി_t *rn);
സ്ഥിതം ശൂന്യം *ngx_resolver_alloc(ngx_resolver_t *r, size_t size);
സ്ഥിതം ശൂന്യം *ngx_resolver_calloc(ngx_resolver_t *r, size_t size);
സ്ഥിതം ശൂന്യം ngx_resolver_free(ngx_resolver_t *r, ശൂന്യം *p);
സ്ഥിതം ശൂന്യം ngx_resolver_free_locked(ngx_resolver_t *r, ശൂന്യം *p);
സ്ഥിതം ശൂന്യം *ngx_resolver_dup(ngx_resolver_t *r, ശൂന്യം *src, size_t size);
സ്ഥിതം ngx_resolver_addr_t *ngx_resolver_export(ngx_resolver_t *r,
    ngx_resolver_കണ്ണി_t *rn, ngx_uint_t rotate);
സ്ഥിതം ശൂന്യം ngx_resolver_report_srv(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx);
സ്ഥിതം u_char *ngx_resolver_log_error(ngx_log_t *log, u_char *buf, size_t len);
സ്ഥിതം ശൂന്യം ngx_resolver_resolve_srv_names(ngx_resolver_ctx_t *ctx,
    ngx_resolver_കണ്ണി_t *rn);
സ്ഥിതം ശൂന്യം ngx_resolver_srv_names_handler(ngx_resolver_ctx_t *ctx);
സ്ഥിതം ngx_int_t ngx_resolver_cmp_srvs(സ്ഥിരം ശൂന്യം *one, സ്ഥിരം ശൂന്യം *two);

#എങ്കിൽ (NGX_HAVE_INET6)
സ്ഥിതം ശൂന്യം ngx_resolver_rbtree_തിരുകുക_addr6_മൂല്യം(ngx_rbtree_കണ്ണി_t *temp,
    ngx_rbtree_കണ്ണി_t *കണ്ണി, ngx_rbtree_കണ്ണി_t *sentinel);
സ്ഥിതം ngx_resolver_കണ്ണി_t *ngx_resolver_lookup_addr6(ngx_resolver_t *r,
    ഘടന in6_addr *addr, uint32_t hash);
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

    r = ngx_pcalloc(cf->pool, വലുപ്പം(ngx_resolver_t));
    എങ്കിൽ (r == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    r->event = ngx_pcalloc(cf->pool, വലുപ്പം(ngx_event_t));
    എങ്കിൽ (r->event == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    cln = ngx_pool_cleanup_add(cf->pool, 0);
    എങ്കിൽ (cln == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    cln->handler = ngx_resolver_cleanup;
    cln->data = r;

    r->ipv4 = 1;

    ngx_rbtree_init(&r->name_rbtree, &r->name_sentinel,
                    ngx_resolver_rbtree_തിരുകുക_മൂല്യം);

    ngx_rbtree_init(&r->srv_rbtree, &r->srv_sentinel,
                    ngx_resolver_rbtree_തിരുകുക_മൂല്യം);

    ngx_rbtree_init(&r->addr_rbtree, &r->addr_sentinel,
                    ngx_rbtree_തിരുകുക_മൂല്യം);

    ngx_queue_init(&r->name_resend_queue);
    ngx_queue_init(&r->srv_resend_queue);
    ngx_queue_init(&r->addr_resend_queue);

    ngx_queue_init(&r->name_expire_queue);
    ngx_queue_init(&r->srv_expire_queue);
    ngx_queue_init(&r->addr_expire_queue);

#എങ്കിൽ (NGX_HAVE_INET6)
    r->ipv6 = 1;

    ngx_rbtree_init(&r->addr6_rbtree, &r->addr6_sentinel,
                    ngx_resolver_rbtree_തിരുകുക_addr6_മൂല്യം);

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

    എങ്കിൽ (n) {
        എങ്കിൽ (ngx_array_init(&r->connections, cf->pool, n,
                           വലുപ്പം(ngx_resolver_connection_t))
            != NGX_OK)
        {
            തിരികെ ശൂന്യ;
        }
    }

    വേണ്ടി (i = 0; i < n; i++) {
        എങ്കിൽ (ngx_strncmp(names[i].data, "valid=", 6) == 0) {
            s.len = names[i].len - 6;
            s.data = names[i].data + 6;

            r->valid = ngx_parse_time(&s, 1);

            എങ്കിൽ (r->valid == (time_t) NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &names[i]);
                തിരികെ ശൂന്യ;
            }

            തുടരുക;
        }

#എങ്കിൽ (NGX_HAVE_INET6)
        എങ്കിൽ (ngx_strncmp(names[i].data, "ipv4=", 5) == 0) {

            എങ്കിൽ (ngx_strcmp(&names[i].data[5], "on") == 0) {
                r->ipv4 = 1;

            } അല്ലെങ്കിൽ എങ്കിൽ (ngx_strcmp(&names[i].data[5], "off") == 0) {
                r->ipv4 = 0;

            } അല്ലെങ്കിൽ {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &names[i]);
                തിരികെ ശൂന്യ;
            }

            തുടരുക;
        }

        എങ്കിൽ (ngx_strncmp(names[i].data, "ipv6=", 5) == 0) {

            എങ്കിൽ (ngx_strcmp(&names[i].data[5], "on") == 0) {
                r->ipv6 = 1;

            } അല്ലെങ്കിൽ എങ്കിൽ (ngx_strcmp(&names[i].data[5], "off") == 0) {
                r->ipv6 = 0;

            } അല്ലെങ്കിൽ {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &names[i]);
                തിരികെ ശൂന്യ;
            }

            തുടരുക;
        }
#endif

        ngx_memzero(&u, വലുപ്പം(ngx_url_t));

        u.url = names[i];
        u.default_port = 53;

        എങ്കിൽ (ngx_parse_url(cf->pool, &u) != NGX_OK) {
            എങ്കിൽ (u.err) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "%s in resolver \"%V\"",
                                   u.err, &u.url);
            }

            തിരികെ ശൂന്യ;
        }

        rec = ngx_array_തള്ളുക_n(&r->connections, u.naddrs);
        എങ്കിൽ (rec == ശൂന്യ) {
            തിരികെ ശൂന്യ;
        }

        ngx_memzero(rec, u.naddrs * വലുപ്പം(ngx_resolver_connection_t));

        വേണ്ടി (j = 0; j < u.naddrs; j++) {
            rec[j].sockaddr = u.addrs[j].sockaddr;
            rec[j].socklen = u.addrs[j].socklen;
            rec[j].server = u.addrs[j].name;
            rec[j].resolver = r;
        }
    }

#എങ്കിൽ (NGX_HAVE_INET6)
    എങ്കിൽ (r->ipv4 + r->ipv6 == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"ipv4\" and \"ipv6\" cannot both be \"off\"");
        തിരികെ ശൂന്യ;
    }
#endif

    എങ്കിൽ (n && r->connections.nelts == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "no name servers defined");
        തിരികെ ശൂന്യ;
    }

    തിരികെ r;
}


സ്ഥിതം ശൂന്യം
ngx_resolver_cleanup(ശൂന്യം *data)
{
    ngx_resolver_t  *r = data;

    ngx_uint_t                  i;
    ngx_resolver_connection_t  *rec;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "cleanup resolver");

    ngx_resolver_cleanup_വൃക്ഷം(r, &r->name_rbtree);

    ngx_resolver_cleanup_വൃക്ഷം(r, &r->srv_rbtree);

    ngx_resolver_cleanup_വൃക്ഷം(r, &r->addr_rbtree);

#എങ്കിൽ (NGX_HAVE_INET6)
    ngx_resolver_cleanup_വൃക്ഷം(r, &r->addr6_rbtree);
#endif

    എങ്കിൽ (r->event->timer_set) {
        ngx_del_timer(r->event);
    }

    rec = r->connections.elts;

    വേണ്ടി (i = 0; i < r->connections.nelts; i++) {
        എങ്കിൽ (rec[i].udp) {
            ngx_close_connection(rec[i].udp);
        }

        എങ്കിൽ (rec[i].tcp) {
            ngx_close_connection(rec[i].tcp);
        }

        എങ്കിൽ (rec[i].read_buf) {
            ngx_resolver_free(r, rec[i].read_buf->start);
            ngx_resolver_free(r, rec[i].read_buf);
        }

        എങ്കിൽ (rec[i].write_buf) {
            ngx_resolver_free(r, rec[i].write_buf->start);
            ngx_resolver_free(r, rec[i].write_buf);
        }
    }
}


സ്ഥിതം ശൂന്യം
ngx_resolver_cleanup_വൃക്ഷം(ngx_resolver_t *r, ngx_rbtree_t *വൃക്ഷം)
{
    ngx_resolver_ctx_t   *ctx, *next;
    ngx_resolver_കണ്ണി_t  *rn;

    ആകുമ്പോൾ (വൃക്ഷം->വേര് != വൃക്ഷം->sentinel) {

        rn = ngx_resolver_കണ്ണി(ngx_rbtree_min(വൃക്ഷം->വേര്, വൃക്ഷം->sentinel));

        ngx_queue_remove(&rn->queue);

        വേണ്ടി (ctx = rn->waiting; ctx; ctx = next) {
            next = ctx->next;

            എങ്കിൽ (ctx->event) {
                എങ്കിൽ (ctx->event->timer_set) {
                    ngx_del_timer(ctx->event);
                }

                ngx_resolver_free(r, ctx->event);
            }

            ngx_resolver_free(r, ctx);
        }

        ngx_rbtree_delete(വൃക്ഷം, &rn->കണ്ണി);

        ngx_resolver_free_കണ്ണി(r, rn);
    }
}


ngx_resolver_ctx_t *
ngx_resolve_start(ngx_resolver_t *r, ngx_resolver_ctx_t *temp)
{
    in_addr_t            addr;
    ngx_resolver_ctx_t  *ctx;

    എങ്കിൽ (temp) {
        addr = ngx_inet_addr(temp->name.data, temp->name.len);

        എങ്കിൽ (addr != INADDR_NONE) {
            temp->resolver = r;
            temp->state = NGX_OK;
            temp->naddrs = 1;
            temp->addrs = &temp->addr;
            temp->addr.sockaddr = (ഘടന sockaddr *) &temp->sin;
            temp->addr.socklen = വലുപ്പം(ഘടന sockaddr_in);
            ngx_memzero(&temp->sin, വലുപ്പം(ഘടന sockaddr_in));
            temp->sin.sin_family = AF_INET;
            temp->sin.sin_addr.s_addr = addr;
            temp->quick = 1;

            തിരികെ temp;
        }
    }

    എങ്കിൽ (r->connections.nelts == 0) {
        തിരികെ NGX_NO_RESOLVER;
    }

    ctx = ngx_resolver_calloc(r, വലുപ്പം(ngx_resolver_ctx_t));

    എങ്കിൽ (ctx) {
        ctx->resolver = r;
    }

    തിരികെ ctx;
}


ngx_int_t
ngx_resolve_name(ngx_resolver_ctx_t *ctx)
{
    size_t           slen;
    ngx_int_t        rc;
    ngx_str_t        name;
    ngx_resolver_t  *r;

    r = ctx->resolver;

    എങ്കിൽ (ctx->name.len > 0 && ctx->name.data[ctx->name.len - 1] == '.') {
        ctx->name.len--;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve: \"%V\"", &ctx->name);

    എങ്കിൽ (ctx->quick) {
        ctx->handler(ctx);
        തിരികെ NGX_OK;
    }

    എങ്കിൽ (ctx->service.len) {
        slen = ctx->service.len;

        എങ്കിൽ (ngx_strlchr(ctx->service.data,
                        ctx->service.data + ctx->service.len, '.')
            == ശൂന്യ)
        {
            slen += വലുപ്പം("_._tcp") - 1;
        }

        name.len = slen + 1 + ctx->name.len;

        name.data = ngx_resolver_alloc(r, name.len);
        എങ്കിൽ (name.data == ശൂന്യ) {
            പോകുക failed;
        }

        എങ്കിൽ (slen == ctx->service.len) {
            ngx_sprintf(name.data, "%V.%V", &ctx->service, &ctx->name);

        } അല്ലെങ്കിൽ {
            ngx_sprintf(name.data, "_%V._tcp.%V", &ctx->service, &ctx->name);
        }

        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &name);

        ngx_resolver_free(r, name.data);

    } അല്ലെങ്കിൽ {
        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &ctx->name);
    }

    എങ്കിൽ (rc == NGX_OK) {
        തിരികെ NGX_OK;
    }

    /* unlock name mutex */

    എങ്കിൽ (rc == NGX_AGAIN) {
        തിരികെ NGX_OK;
    }

    /* NGX_ERROR */

    എങ്കിൽ (ctx->event) {
        ngx_resolver_free(r, ctx->event);
    }

failed:

    ngx_resolver_free(r, ctx);

    തിരികെ NGX_ERROR;
}


ശൂന്യം
ngx_resolve_name_done(ngx_resolver_ctx_t *ctx)
{
    ngx_uint_t            i;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_കണ്ണി_t  *rn;

    r = ctx->resolver;

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve name done: %i", ctx->state);

    എങ്കിൽ (ctx->quick) {
        തിരികെ;
    }

    എങ്കിൽ (ctx->event && ctx->event->timer_set) {
        ngx_del_timer(ctx->event);
    }

    /* lock name mutex */

    എങ്കിൽ (ctx->nsrvs) {
        വേണ്ടി (i = 0; i < ctx->nsrvs; i++) {
            എങ്കിൽ (ctx->srvs[i].ctx) {
                ngx_resolve_name_done(ctx->srvs[i].ctx);
            }

            എങ്കിൽ (ctx->srvs[i].addrs) {
                ngx_resolver_free(r, ctx->srvs[i].addrs->sockaddr);
                ngx_resolver_free(r, ctx->srvs[i].addrs);
            }

            ngx_resolver_free(r, ctx->srvs[i].name.data);
        }

        ngx_resolver_free(r, ctx->srvs);
    }

    എങ്കിൽ (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->കണ്ണി;

        എങ്കിൽ (rn) {
            p = &rn->waiting;
            w = rn->waiting;

            ആകുമ്പോൾ (w) {
                എങ്കിൽ (w == ctx) {
                    *p = w->next;

                    പോകുക done;
                }

                p = &w->next;
                w = w->next;
            }

            ngx_log_error(NGX_LOG_ALERT, r->log, 0,
                          "could not cancel %V resolving", &ctx->name);
        }
    }

done:

    എങ്കിൽ (ctx->service.len) {
        ngx_resolver_expire(r, &r->srv_rbtree, &r->srv_expire_queue);

    } അല്ലെങ്കിൽ {
        ngx_resolver_expire(r, &r->name_rbtree, &r->name_expire_queue);
    }

    /* unlock name mutex */

    /* lock alloc mutex */

    എങ്കിൽ (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    എങ്കിൽ (r->event->timer_set && ngx_resolver_resend_empty(r)) {
        ngx_del_timer(r->event);
    }
}


സ്ഥിതം ngx_int_t
ngx_resolve_name_locked(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx,
    ngx_str_t *name)
{
    uint32_t              hash;
    ngx_int_t             rc;
    ngx_str_t             cname;
    ngx_uint_t            i, naddrs;
    ngx_queue_t          *resend_queue, *expire_queue;
    ngx_rbtree_t         *വൃക്ഷം;
    ngx_resolver_ctx_t   *next, *last;
    ngx_resolver_addr_t  *addrs;
    ngx_resolver_കണ്ണി_t  *rn;

    ngx_strlow(name->data, name->data, name->len);

    hash = ngx_crc32_short(name->data, name->len);

    എങ്കിൽ (ctx->service.len) {
        rn = ngx_resolver_lookup_srv(r, name, hash);

        വൃക്ഷം = &r->srv_rbtree;
        resend_queue = &r->srv_resend_queue;
        expire_queue = &r->srv_expire_queue;

    } അല്ലെങ്കിൽ {
        rn = ngx_resolver_lookup_name(r, name, hash);

        വൃക്ഷം = &r->name_rbtree;
        resend_queue = &r->name_resend_queue;
        expire_queue = &r->name_expire_queue;
    }

    എങ്കിൽ (rn) {

        /* ctx can be a list after NGX_RESOLVE_CNAME */
        വേണ്ടി (last = ctx; last->next; last = last->next);

        എങ്കിൽ (rn->valid >= ngx_time()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0, "resolve cached");

            ngx_queue_remove(&rn->queue);

            rn->expire = ngx_time() + r->expire;

            ngx_queue_തിരുകുക_head(expire_queue, &rn->queue);

            naddrs = (rn->naddrs == (u_short) -1) ? 0 : rn->naddrs;
#എങ്കിൽ (NGX_HAVE_INET6)
            naddrs += (rn->naddrs6 == (u_short) -1) ? 0 : rn->naddrs6;
#endif

            എങ്കിൽ (naddrs) {

                എങ്കിൽ (naddrs == 1 && rn->naddrs == 1) {
                    addrs = ശൂന്യ;

                } അല്ലെങ്കിൽ {
                    addrs = ngx_resolver_export(r, rn, 1);
                    എങ്കിൽ (addrs == ശൂന്യ) {
                        തിരികെ NGX_ERROR;
                    }
                }

                last->next = rn->waiting;
                rn->waiting = ശൂന്യ;

                /* unlock name mutex */

                ചെയ്യുക {
                    ctx->state = NGX_OK;
                    ctx->valid = rn->valid;
                    ctx->naddrs = naddrs;

                    എങ്കിൽ (addrs == ശൂന്യ) {
                        ctx->addrs = &ctx->addr;
                        ctx->addr.sockaddr = (ഘടന sockaddr *) &ctx->sin;
                        ctx->addr.socklen = വലുപ്പം(ഘടന sockaddr_in);
                        ngx_memzero(&ctx->sin, വലുപ്പം(ഘടന sockaddr_in));
                        ctx->sin.sin_family = AF_INET;
                        ctx->sin.sin_addr.s_addr = rn->u.addr;

                    } അല്ലെങ്കിൽ {
                        ctx->addrs = addrs;
                    }

                    next = ctx->next;

                    ctx->handler(ctx);

                    ctx = next;
                } ആകുമ്പോൾ (ctx);

                എങ്കിൽ (addrs != ശൂന്യ) {
                    ngx_resolver_free(r, addrs->sockaddr);
                    ngx_resolver_free(r, addrs);
                }

                തിരികെ NGX_OK;
            }

            എങ്കിൽ (rn->nsrvs) {
                last->next = rn->waiting;
                rn->waiting = ശൂന്യ;

                /* unlock name mutex */

                ചെയ്യുക {
                    next = ctx->next;

                    ngx_resolver_resolve_srv_names(ctx, rn);

                    ctx = next;
                } ആകുമ്പോൾ (ctx);

                തിരികെ NGX_OK;
            }

            /* NGX_RESOLVE_CNAME */

            എങ്കിൽ (ctx->recursion++ < NGX_RESOLVER_MAX_RECURSION) {

                cname.len = rn->cnlen;
                cname.data = rn->u.cname;

                തിരികെ ngx_resolve_name_locked(r, ctx, &cname);
            }

            last->next = rn->waiting;
            rn->waiting = ശൂന്യ;

            /* unlock name mutex */

            ചെയ്യുക {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
                next = ctx->next;

                ctx->handler(ctx);

                ctx = next;
            } ആകുമ്പോൾ (ctx);

            തിരികെ NGX_OK;
        }

        എങ്കിൽ (rn->waiting) {
            എങ്കിൽ (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
                തിരികെ NGX_ERROR;
            }

            last->next = rn->waiting;
            rn->waiting = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;

            ചെയ്യുക {
                ctx->കണ്ണി = rn;
                ctx = ctx->next;
            } ആകുമ്പോൾ (ctx);

            തിരികെ NGX_AGAIN;
        }

        ngx_queue_remove(&rn->queue);

        /* lock alloc mutex */

        എങ്കിൽ (rn->query) {
            ngx_resolver_free_locked(r, rn->query);
            rn->query = ശൂന്യ;
#എങ്കിൽ (NGX_HAVE_INET6)
            rn->query6 = ശൂന്യ;
#endif
        }

        എങ്കിൽ (rn->cnlen) {
            ngx_resolver_free_locked(r, rn->u.cname);
        }

        എങ്കിൽ (rn->naddrs > 1 && rn->naddrs != (u_short) -1) {
            ngx_resolver_free_locked(r, rn->u.addrs);
        }

#എങ്കിൽ (NGX_HAVE_INET6)
        എങ്കിൽ (rn->naddrs6 > 1 && rn->naddrs6 != (u_short) -1) {
            ngx_resolver_free_locked(r, rn->u6.addrs6);
        }
#endif

        എങ്കിൽ (rn->nsrvs) {
            വേണ്ടി (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
                എങ്കിൽ (rn->u.srvs[i].name.data) {
                    ngx_resolver_free_locked(r, rn->u.srvs[i].name.data);
                }
            }

            ngx_resolver_free_locked(r, rn->u.srvs);
        }

        /* unlock alloc mutex */

    } അല്ലെങ്കിൽ {

        rn = ngx_resolver_alloc(r, വലുപ്പം(ngx_resolver_കണ്ണി_t));
        എങ്കിൽ (rn == ശൂന്യ) {
            തിരികെ NGX_ERROR;
        }

        rn->name = ngx_resolver_dup(r, name->data, name->len);
        എങ്കിൽ (rn->name == ശൂന്യ) {
            ngx_resolver_free(r, rn);
            തിരികെ NGX_ERROR;
        }

        rn->കണ്ണി.താക്കോൽ = hash;
        rn->nlen = (u_short) name->len;
        rn->query = ശൂന്യ;
#എങ്കിൽ (NGX_HAVE_INET6)
        rn->query6 = ശൂന്യ;
#endif

        ngx_rbtree_തിരുകുക(വൃക്ഷം, &rn->കണ്ണി);
    }

    എങ്കിൽ (ctx->service.len) {
        rc = ngx_resolver_create_srv_query(r, rn, name);

    } അല്ലെങ്കിൽ {
        rc = ngx_resolver_create_name_query(r, rn, name);
    }

    എങ്കിൽ (rc == NGX_ERROR) {
        പോകുക failed;
    }

    എങ്കിൽ (rc == NGX_DECLINED) {
        ngx_rbtree_delete(വൃക്ഷം, &rn->കണ്ണി);

        ngx_resolver_free(r, rn->query);
        ngx_resolver_free(r, rn->name);
        ngx_resolver_free(r, rn);

        ചെയ്യുക {
            ctx->state = NGX_RESOLVE_NXDOMAIN;
            next = ctx->next;

            ctx->handler(ctx);

            ctx = next;
        } ആകുമ്പോൾ (ctx);

        തിരികെ NGX_OK;
    }

    rn->last_connection = r->last_connection++;
    എങ്കിൽ (r->last_connection == r->connections.nelts) {
        r->last_connection = 0;
    }

    rn->naddrs = r->ipv4 ? (u_short) -1 : 0;
    rn->tcp = 0;
#എങ്കിൽ (NGX_HAVE_INET6)
    rn->naddrs6 = r->ipv6 ? (u_short) -1 : 0;
    rn->tcp6 = 0;
#endif
    rn->nsrvs = 0;

    എങ്കിൽ (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->last_connection++;
        എങ്കിൽ (rn->last_connection == r->connections.nelts) {
            rn->last_connection = 0;
        }

        (ശൂന്യം) ngx_resolver_send_query(r, rn);
    }

    എങ്കിൽ (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
        പോകുക failed;
    }

    എങ്കിൽ (ngx_resolver_resend_empty(r)) {
        ngx_add_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_time() + r->resend_timeout;

    ngx_queue_തിരുകുക_head(resend_queue, &rn->queue);

    rn->code = 0;
    rn->cnlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_മൂല്യം;
    rn->waiting = ctx;

    ctx->state = NGX_AGAIN;
    ctx->async = 1;

    ചെയ്യുക {
        ctx->കണ്ണി = rn;
        ctx = ctx->next;
    } ആകുമ്പോൾ (ctx);

    തിരികെ NGX_AGAIN;

failed:

    ngx_rbtree_delete(വൃക്ഷം, &rn->കണ്ണി);

    എങ്കിൽ (rn->query) {
        ngx_resolver_free(r, rn->query);
    }

    ngx_resolver_free(r, rn->name);

    ngx_resolver_free(r, rn);

    തിരികെ NGX_ERROR;
}


ngx_int_t
ngx_resolve_addr(ngx_resolver_ctx_t *ctx)
{
    u_char               *name;
    in_addr_t             addr;
    ngx_queue_t          *resend_queue, *expire_queue;
    ngx_rbtree_t         *വൃക്ഷം;
    ngx_resolver_t       *r;
    ഘടന sockaddr_in   *sin;
    ngx_resolver_കണ്ണി_t  *rn;
#എങ്കിൽ (NGX_HAVE_INET6)
    uint32_t              hash;
    ഘടന sockaddr_in6  *sin6;
#endif

#എങ്കിൽ (NGX_SUPPRESS_WARN)
    addr = 0;
#എങ്കിൽ (NGX_HAVE_INET6)
    hash = 0;
    sin6 = ശൂന്യ;
#endif
#endif

    r = ctx->resolver;

    തിരഞ്ഞെടുപ്പ് (ctx->addr.sockaddr->sa_family) {

#എങ്കിൽ (NGX_HAVE_INET6)
    അവസ്ഥ AF_INET6:
        sin6 = (ഘടന sockaddr_in6 *) ctx->addr.sockaddr;
        hash = ngx_crc32_short(sin6->sin6_addr.s6_addr, 16);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr6(r, &sin6->sin6_addr, hash);

        വൃക്ഷം = &r->addr6_rbtree;
        resend_queue = &r->addr6_resend_queue;
        expire_queue = &r->addr6_expire_queue;

        നിർത്തുക;
#endif

    സ്ഥിരസ്ഥിതി: /* AF_INET */
        sin = (ഘടന sockaddr_in *) ctx->addr.sockaddr;
        addr = ntohl(sin->sin_addr.s_addr);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        വൃക്ഷം = &r->addr_rbtree;
        resend_queue = &r->addr_resend_queue;
        expire_queue = &r->addr_expire_queue;
    }

    എങ്കിൽ (rn) {

        എങ്കിൽ (rn->valid >= ngx_time()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0, "resolve cached");

            ngx_queue_remove(&rn->queue);

            rn->expire = ngx_time() + r->expire;

            ngx_queue_തിരുകുക_head(expire_queue, &rn->queue);

            name = ngx_resolver_dup(r, rn->name, rn->nlen);
            എങ്കിൽ (name == ശൂന്യ) {
                ngx_resolver_free(r, ctx);
                തിരികെ NGX_ERROR;
            }

            ctx->name.len = rn->nlen;
            ctx->name.data = name;

            /* unlock addr mutex */

            ctx->state = NGX_OK;
            ctx->valid = rn->valid;

            ctx->handler(ctx);

            ngx_resolver_free(r, name);

            തിരികെ NGX_OK;
        }

        എങ്കിൽ (rn->waiting) {
            എങ്കിൽ (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
                തിരികെ NGX_ERROR;
            }

            ctx->next = rn->waiting;
            rn->waiting = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;
            ctx->കണ്ണി = rn;

            /* unlock addr mutex */

            തിരികെ NGX_OK;
        }

        ngx_queue_remove(&rn->queue);

        ngx_resolver_free(r, rn->query);
        rn->query = ശൂന്യ;
#എങ്കിൽ (NGX_HAVE_INET6)
        rn->query6 = ശൂന്യ;
#endif

    } അല്ലെങ്കിൽ {
        rn = ngx_resolver_alloc(r, വലുപ്പം(ngx_resolver_കണ്ണി_t));
        എങ്കിൽ (rn == ശൂന്യ) {
            പോകുക failed;
        }

        തിരഞ്ഞെടുപ്പ് (ctx->addr.sockaddr->sa_family) {

#എങ്കിൽ (NGX_HAVE_INET6)
        അവസ്ഥ AF_INET6:
            rn->addr6 = sin6->sin6_addr;
            rn->കണ്ണി.താക്കോൽ = hash;
            നിർത്തുക;
#endif

        സ്ഥിരസ്ഥിതി: /* AF_INET */
            rn->കണ്ണി.താക്കോൽ = addr;
        }

        rn->query = ശൂന്യ;
#എങ്കിൽ (NGX_HAVE_INET6)
        rn->query6 = ശൂന്യ;
#endif

        ngx_rbtree_തിരുകുക(വൃക്ഷം, &rn->കണ്ണി);
    }

    എങ്കിൽ (ngx_resolver_create_addr_query(r, rn, &ctx->addr) != NGX_OK) {
        പോകുക failed;
    }

    rn->last_connection = r->last_connection++;
    എങ്കിൽ (r->last_connection == r->connections.nelts) {
        r->last_connection = 0;
    }

    rn->naddrs = (u_short) -1;
    rn->tcp = 0;
#എങ്കിൽ (NGX_HAVE_INET6)
    rn->naddrs6 = (u_short) -1;
    rn->tcp6 = 0;
#endif
    rn->nsrvs = 0;

    എങ്കിൽ (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->last_connection++;
        എങ്കിൽ (rn->last_connection == r->connections.nelts) {
            rn->last_connection = 0;
        }

        (ശൂന്യം) ngx_resolver_send_query(r, rn);
    }

    എങ്കിൽ (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
        പോകുക failed;
    }

    എങ്കിൽ (ngx_resolver_resend_empty(r)) {
        ngx_add_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_time() + r->resend_timeout;

    ngx_queue_തിരുകുക_head(resend_queue, &rn->queue);

    rn->code = 0;
    rn->cnlen = 0;
    rn->name = ശൂന്യ;
    rn->nlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_മൂല്യം;
    rn->waiting = ctx;

    /* unlock addr mutex */

    ctx->state = NGX_AGAIN;
    ctx->async = 1;
    ctx->കണ്ണി = rn;

    തിരികെ NGX_OK;

failed:

    എങ്കിൽ (rn) {
        ngx_rbtree_delete(വൃക്ഷം, &rn->കണ്ണി);

        എങ്കിൽ (rn->query) {
            ngx_resolver_free(r, rn->query);
        }

        ngx_resolver_free(r, rn);
    }

    /* unlock addr mutex */

    എങ്കിൽ (ctx->event) {
        ngx_resolver_free(r, ctx->event);
    }

    ngx_resolver_free(r, ctx);

    തിരികെ NGX_ERROR;
}


ശൂന്യം
ngx_resolve_addr_done(ngx_resolver_ctx_t *ctx)
{
    ngx_queue_t          *expire_queue;
    ngx_rbtree_t         *വൃക്ഷം;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_കണ്ണി_t  *rn;

    r = ctx->resolver;

    തിരഞ്ഞെടുപ്പ് (ctx->addr.sockaddr->sa_family) {

#എങ്കിൽ (NGX_HAVE_INET6)
    അവസ്ഥ AF_INET6:
        വൃക്ഷം = &r->addr6_rbtree;
        expire_queue = &r->addr6_expire_queue;
        നിർത്തുക;
#endif

    സ്ഥിരസ്ഥിതി: /* AF_INET */
        വൃക്ഷം = &r->addr_rbtree;
        expire_queue = &r->addr_expire_queue;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve addr done: %i", ctx->state);

    എങ്കിൽ (ctx->event && ctx->event->timer_set) {
        ngx_del_timer(ctx->event);
    }

    /* lock addr mutex */

    എങ്കിൽ (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->കണ്ണി;

        എങ്കിൽ (rn) {
            p = &rn->waiting;
            w = rn->waiting;

            ആകുമ്പോൾ (w) {
                എങ്കിൽ (w == ctx) {
                    *p = w->next;

                    പോകുക done;
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

    ngx_resolver_expire(r, വൃക്ഷം, expire_queue);

    /* unlock addr mutex */

    /* lock alloc mutex */

    എങ്കിൽ (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    എങ്കിൽ (r->event->timer_set && ngx_resolver_resend_empty(r)) {
        ngx_del_timer(r->event);
    }
}


സ്ഥിതം ശൂന്യം
ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *വൃക്ഷം, ngx_queue_t *queue)
{
    time_t                now;
    ngx_uint_t            i;
    ngx_queue_t          *q;
    ngx_resolver_കണ്ണി_t  *rn;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver expire");

    now = ngx_time();

    വേണ്ടി (i = 0; i < 2; i++) {
        എങ്കിൽ (ngx_queue_empty(queue)) {
            തിരികെ;
        }

        q = ngx_queue_last(queue);

        rn = ngx_queue_data(q, ngx_resolver_കണ്ണി_t, queue);

        എങ്കിൽ (now <= rn->expire) {
            തിരികെ;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver expire \"%*s\"", (size_t) rn->nlen, rn->name);

        ngx_queue_remove(q);

        ngx_rbtree_delete(വൃക്ഷം, &rn->കണ്ണി);

        ngx_resolver_free_കണ്ണി(r, rn);
    }
}


സ്ഥിതം ngx_int_t
ngx_resolver_send_query(ngx_resolver_t *r, ngx_resolver_കണ്ണി_t *rn)
{
    ngx_int_t                   rc;
    ngx_resolver_connection_t  *rec;

    rec = r->connections.elts;
    rec = &rec[rn->last_connection];

    എങ്കിൽ (rec->log.handler == ശൂന്യ) {
        rec->log = *r->log;
        rec->log.handler = ngx_resolver_log_error;
        rec->log.data = rec;
        rec->log.action = "resolving";
    }

    എങ്കിൽ (rn->query && rn->naddrs == (u_short) -1) {
        rc = rn->tcp ? ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen)
                     : ngx_resolver_send_udp_query(r, rec, rn->query, rn->qlen);

        എങ്കിൽ (rc != NGX_OK) {
            തിരികെ rc;
        }
    }

#എങ്കിൽ (NGX_HAVE_INET6)

    എങ്കിൽ (rn->query6 && rn->naddrs6 == (u_short) -1) {
        rc = rn->tcp6
                    ? ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen)
                    : ngx_resolver_send_udp_query(r, rec, rn->query6, rn->qlen);

        എങ്കിൽ (rc != NGX_OK) {
            തിരികെ rc;
        }
    }

#endif

    തിരികെ NGX_OK;
}


സ്ഥിതം ngx_int_t
ngx_resolver_send_udp_query(ngx_resolver_t *r, ngx_resolver_connection_t  *rec,
    u_char *query, u_short qlen)
{
    ssize_t  n;

    എങ്കിൽ (rec->udp == ശൂന്യ) {
        എങ്കിൽ (ngx_udp_connect(rec) != NGX_OK) {
            തിരികെ NGX_ERROR;
        }

        rec->udp->data = rec;
        rec->udp->പഠ->handler = ngx_resolver_udp_read;
        rec->udp->പഠ->resolver = 1;
    }

    n = ngx_send(rec->udp, query, qlen);

    എങ്കിൽ (n == NGX_ERROR) {
        പോകുക failed;
    }

    എങ്കിൽ ((size_t) n != (size_t) qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->log, 0, "send() incomplete");
        പോകുക failed;
    }

    തിരികെ NGX_OK;

failed:

    ngx_close_connection(rec->udp);
    rec->udp = ശൂന്യ;

    തിരികെ NGX_ERROR;
}


സ്ഥിതം ngx_int_t
ngx_resolver_send_tcp_query(ngx_resolver_t *r, ngx_resolver_connection_t *rec,
    u_char *query, u_short qlen)
{
    ngx_buf_t  *b;
    ngx_int_t   rc;

    rc = NGX_OK;

    എങ്കിൽ (rec->tcp == ശൂന്യ) {
        b = rec->read_buf;

        എങ്കിൽ (b == ശൂന്യ) {
            b = ngx_resolver_calloc(r, വലുപ്പം(ngx_buf_t));
            എങ്കിൽ (b == ശൂന്യ) {
                തിരികെ NGX_ERROR;
            }

            b->start = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_RSIZE);
            എങ്കിൽ (b->start == ശൂന്യ) {
                ngx_resolver_free(r, b);
                തിരികെ NGX_ERROR;
            }

            b->end = b->start + NGX_RESOLVER_TCP_RSIZE;

            rec->read_buf = b;
        }

        b->pos = b->start;
        b->last = b->start;

        b = rec->write_buf;

        എങ്കിൽ (b == ശൂന്യ) {
            b = ngx_resolver_calloc(r, വലുപ്പം(ngx_buf_t));
            എങ്കിൽ (b == ശൂന്യ) {
                തിരികെ NGX_ERROR;
            }

            b->start = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_WSIZE);
            എങ്കിൽ (b->start == ശൂന്യ) {
                ngx_resolver_free(r, b);
                തിരികെ NGX_ERROR;
            }

            b->end = b->start + NGX_RESOLVER_TCP_WSIZE;

            rec->write_buf = b;
        }

        b->pos = b->start;
        b->last = b->start;

        rc = ngx_tcp_connect(rec);
        എങ്കിൽ (rc == NGX_ERROR) {
            തിരികെ NGX_ERROR;
        }

        rec->tcp->data = rec;
        rec->tcp->ലിഖ->handler = ngx_resolver_tcp_write;
        rec->tcp->ലിഖ->cancelable = 1;
        rec->tcp->പഠ->handler = ngx_resolver_tcp_read;
        rec->tcp->പഠ->resolver = 1;

        ngx_add_timer(rec->tcp->ലിഖ, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    b = rec->write_buf;

    എങ്കിൽ (b->end - b->last <  2 + qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->log, 0, "buffer overflow");
        തിരികെ NGX_ERROR;
    }

    *b->last++ = (u_char) (qlen >> 8);
    *b->last++ = (u_char) qlen;
    b->last = ngx_cpymem(b->last, query, qlen);

    എങ്കിൽ (rc == NGX_OK) {
        ngx_resolver_tcp_write(rec->tcp->ലിഖ);
    }

    തിരികെ NGX_OK;
}


സ്ഥിതം ശൂന്യം
ngx_resolver_resend_handler(ngx_event_t *ev)
{
    time_t           timer, atimer, stimer, ntimer;
#എങ്കിൽ (NGX_HAVE_INET6)
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

#എങ്കിൽ (NGX_HAVE_INET6)

    /* lock addr6 mutex */

    a6timer = ngx_resolver_resend(r, &r->addr6_rbtree, &r->addr6_resend_queue);

    /* unlock addr6 mutex */

#endif

    timer = ntimer;

    എങ്കിൽ (timer == 0) {
        timer = atimer;

    } അല്ലെങ്കിൽ എങ്കിൽ (atimer) {
        timer = ngx_min(timer, atimer);
    }

    എങ്കിൽ (timer == 0) {
        timer = stimer;

    } അല്ലെങ്കിൽ എങ്കിൽ (stimer) {
        timer = ngx_min(timer, stimer);
    }

#എങ്കിൽ (NGX_HAVE_INET6)

    എങ്കിൽ (timer == 0) {
        timer = a6timer;

    } അല്ലെങ്കിൽ എങ്കിൽ (a6timer) {
        timer = ngx_min(timer, a6timer);
    }

#endif

    എങ്കിൽ (timer) {
        ngx_add_timer(r->event, (ngx_msec_t) (timer * 1000));
    }
}


സ്ഥിതം time_t
ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *വൃക്ഷം, ngx_queue_t *queue)
{
    time_t                now;
    ngx_queue_t          *q;
    ngx_resolver_കണ്ണി_t  *rn;

    now = ngx_time();

    വേണ്ടി ( ;; ) {
        എങ്കിൽ (ngx_queue_empty(queue)) {
            തിരികെ 0;
        }

        q = ngx_queue_last(queue);

        rn = ngx_queue_data(q, ngx_resolver_കണ്ണി_t, queue);

        എങ്കിൽ (now < rn->expire) {
            തിരികെ rn->expire - now;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver resend \"%*s\" %p",
                       (size_t) rn->nlen, rn->name, rn->waiting);

        ngx_queue_remove(q);

        എങ്കിൽ (rn->waiting) {

            എങ്കിൽ (++rn->last_connection == r->connections.nelts) {
                rn->last_connection = 0;
            }

            (ശൂന്യം) ngx_resolver_send_query(r, rn);

            rn->expire = now + r->resend_timeout;

            ngx_queue_തിരുകുക_head(queue, q);

            തുടരുക;
        }

        ngx_rbtree_delete(വൃക്ഷം, &rn->കണ്ണി);

        ngx_resolver_free_കണ്ണി(r, rn);
    }
}


സ്ഥിതം ngx_uint_t
ngx_resolver_resend_empty(ngx_resolver_t *r)
{
    തിരികെ ngx_queue_empty(&r->name_resend_queue)
           && ngx_queue_empty(&r->srv_resend_queue)
#എങ്കിൽ (NGX_HAVE_INET6)
           && ngx_queue_empty(&r->addr6_resend_queue)
#endif
           && ngx_queue_empty(&r->addr_resend_queue);
}


സ്ഥിതം ശൂന്യം
ngx_resolver_udp_read(ngx_event_t *rev)
{
    ssize_t                     n;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;
    u_char                      buf[NGX_RESOLVER_UDP_SIZE];

    c = rev->data;
    rec = c->data;

    ചെയ്യുക {
        n = ngx_udp_recv(c, buf, NGX_RESOLVER_UDP_SIZE);

        എങ്കിൽ (n == NGX_AGAIN) {
            നിർത്തുക;
        }

        എങ്കിൽ (n == NGX_ERROR) {
            പോകുക failed;
        }

        ngx_resolver_process_response(rec->resolver, buf, n, 0);

    } ആകുമ്പോൾ (rev->ready);

    എങ്കിൽ (ngx_handle_read_event(rev, 0) != NGX_OK) {
        പോകുക failed;
    }

    തിരികെ;

failed:

    ngx_close_connection(rec->udp);
    rec->udp = ശൂന്യ;
}


സ്ഥിതം ശൂന്യം
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

    എങ്കിൽ (wev->timedout) {
        പോകുക failed;
    }

    sent = c->sent;

    ആകുമ്പോൾ (wev->ready && b->pos < b->last) {
        n = ngx_send(c, b->pos, b->last - b->pos);

        എങ്കിൽ (n == NGX_AGAIN) {
            നിർത്തുക;
        }

        എങ്കിൽ (n == NGX_ERROR) {
            പോകുക failed;
        }

        b->pos += n;
    }

    എങ്കിൽ (b->pos != b->start) {
        b->last = ngx_movemem(b->start, b->pos, b->last - b->pos);
        b->pos = b->start;
    }

    എങ്കിൽ (c->sent != sent) {
        ngx_add_timer(wev, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    എങ്കിൽ (ngx_handle_write_event(wev, 0) != NGX_OK) {
        പോകുക failed;
    }

    തിരികെ;

failed:

    ngx_close_connection(c);
    rec->tcp = ശൂന്യ;
}


സ്ഥിതം ശൂന്യം
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

    ആകുമ്പോൾ (rev->ready) {
        n = ngx_recv(c, b->last, b->end - b->last);

        എങ്കിൽ (n == NGX_AGAIN) {
            നിർത്തുക;
        }

        എങ്കിൽ (n == NGX_ERROR || n == 0) {
            പോകുക failed;
        }

        b->last += n;

        വേണ്ടി ( ;; ) {
            p = b->pos;
            size = b->last - p;

            എങ്കിൽ (size < 2) {
                നിർത്തുക;
            }

            qlen = (u_short) *p++ << 8;
            qlen += *p++;

            എങ്കിൽ (size < (size_t) (2 + qlen)) {
                നിർത്തുക;
            }

            ngx_resolver_process_response(r, p, qlen, 1);

            b->pos += 2 + qlen;
        }

        എങ്കിൽ (b->pos != b->start) {
            b->last = ngx_movemem(b->start, b->pos, b->last - b->pos);
            b->pos = b->start;
        }
    }

    എങ്കിൽ (ngx_handle_read_event(rev, 0) != NGX_OK) {
        പോകുക failed;
    }

    തിരികെ;

failed:

    ngx_close_connection(c);
    rec->tcp = ശൂന്യ;
}


സ്ഥിതം ശൂന്യം
ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t tcp)
{
    അക്ഷരം                 *err;
    ngx_uint_t            i, times, ident, qident, flags, code, nqs, nan, trunc,
                          qtype, qclass;
#എങ്കിൽ (NGX_HAVE_INET6)
    ngx_uint_t            qident6;
#endif
    ngx_queue_t          *q;
    ngx_resolver_qs_t    *qs;
    ngx_resolver_hdr_t   *response;
    ngx_resolver_കണ്ണി_t  *rn;

    എങ്കിൽ (n < വലുപ്പം(ngx_resolver_hdr_t)) {
        പോകുക short_response;
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
    എങ്കിൽ ((flags & 0xf870) != 0x8000 || (trunc && tcp)) {
        ngx_log_error(r->log_level, r->log, 0,
                      "invalid %s DNS response %ui fl:%04Xi",
                      tcp ? "TCP" : "UDP", ident, flags);
        തിരികെ;
    }

    code = flags & 0xf;

    എങ്കിൽ (code == NGX_RESOLVE_FORMERR) {

        times = 0;

        വേണ്ടി (q = ngx_queue_head(&r->name_resend_queue);
             q != ngx_queue_sentinel(&r->name_resend_queue) && times++ < 100;
             q = ngx_queue_next(q))
        {
            rn = ngx_queue_data(q, ngx_resolver_കണ്ണി_t, queue);

            എങ്കിൽ (rn->query) {
                qident = (rn->query[0] << 8) + rn->query[1];

                എങ്കിൽ (qident == ident) {
                    പോകുക dns_error_name;
                }
            }

#എങ്കിൽ (NGX_HAVE_INET6)
            എങ്കിൽ (rn->query6) {
                qident6 = (rn->query6[0] << 8) + rn->query6[1];

                എങ്കിൽ (qident6 == ident) {
                    പോകുക dns_error_name;
                }
            }
#endif
        }

        പോകുക dns_error;
    }

    എങ്കിൽ (code > NGX_RESOLVE_REFUSED) {
        പോകുക dns_error;
    }

    എങ്കിൽ (nqs != 1) {
        err = "invalid number of questions in DNS response";
        പോകുക done;
    }

    i = വലുപ്പം(ngx_resolver_hdr_t);

    ആകുമ്പോൾ (i < (ngx_uint_t) n) {

        എങ്കിൽ (buf[i] & 0xc0) {
            err = "unexpected compression pointer in DNS response";
            പോകുക done;
        }

        എങ്കിൽ (buf[i] == '\0') {
            പോകുക found;
        }

        i += 1 + buf[i];
    }

    പോകുക short_response;

found:

    എങ്കിൽ (i++ == വലുപ്പം(ngx_resolver_hdr_t)) {
        err = "zero-length domain name in DNS response";
        പോകുക done;
    }

    എങ്കിൽ (i + വലുപ്പം(ngx_resolver_qs_t) + nan * (2 + വലുപ്പം(ngx_resolver_an_t))
        > (ngx_uint_t) n)
    {
        പോകുക short_response;
    }

    qs = (ngx_resolver_qs_t *) &buf[i];

    qtype = (qs->type_hi << 8) + qs->type_lo;
    qclass = (qs->class_hi << 8) + qs->class_lo;

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver DNS response qt:%ui cl:%ui", qtype, qclass);

    എങ്കിൽ (qclass != 1) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unknown query class %ui in DNS response", qclass);
        തിരികെ;
    }

    തിരഞ്ഞെടുപ്പ് (qtype) {

    അവസ്ഥ NGX_RESOLVE_A:
#എങ്കിൽ (NGX_HAVE_INET6)
    അവസ്ഥ NGX_RESOLVE_AAAA:
#endif

        ngx_resolver_process_a(r, buf, n, ident, code, qtype, nan, trunc,
                               i + വലുപ്പം(ngx_resolver_qs_t));

        നിർത്തുക;

    അവസ്ഥ NGX_RESOLVE_SRV:

        ngx_resolver_process_srv(r, buf, n, ident, code, nan, trunc,
                                 i + വലുപ്പം(ngx_resolver_qs_t));

        നിർത്തുക;

    അവസ്ഥ NGX_RESOLVE_PTR:

        ngx_resolver_process_ptr(r, buf, n, ident, code, nan);

        നിർത്തുക;

    സ്ഥിരസ്ഥിതി:
        ngx_log_error(r->log_level, r->log, 0,
                      "unknown query type %ui in DNS response", qtype);
        തിരികെ;
    }

    തിരികെ;

short_response:

    err = "short DNS response";

done:

    ngx_log_error(r->log_level, r->log, 0, err);

    തിരികെ;

dns_error_name:

    ngx_log_error(r->log_level, r->log, 0,
                  "DNS error (%ui: %s), query id:%ui, name:\"%*s\"",
                  code, ngx_resolver_strerror(code), ident,
                  (size_t) rn->nlen, rn->name);
    തിരികെ;

dns_error:

    ngx_log_error(r->log_level, r->log, 0,
                  "DNS error (%ui: %s), query id:%ui",
                  code, ngx_resolver_strerror(code), ident);
    തിരികെ;
}


സ്ഥിതം ശൂന്യം
ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans)
{
    അക്ഷരം                       *err;
    u_char                     *cname;
    size_t                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    in_addr_t                  *addr;
    ngx_str_t                   name;
    ngx_uint_t                  type, class, qident, naddrs, a, i, j, start;
#എങ്കിൽ (NGX_HAVE_INET6)
    ഘടന in6_addr            *addr6;
#endif
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *next;
    ngx_resolver_കണ്ണി_t        *rn;
    ngx_resolver_addr_t        *addrs;
    ngx_resolver_connection_t  *rec;

    എങ്കിൽ (ngx_resolver_copy(r, &name, buf,
                          buf + വലുപ്പം(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        തിരികെ;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver qs:%V", &name);

    hash = ngx_crc32_short(name.data, name.len);

    /* lock name mutex */

    rn = ngx_resolver_lookup_name(r, &name, hash);

    എങ്കിൽ (rn == ശൂന്യ) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.data);
        പോകുക failed;
    }

    തിരഞ്ഞെടുപ്പ് (qtype) {

#എങ്കിൽ (NGX_HAVE_INET6)
    അവസ്ഥ NGX_RESOLVE_AAAA:

        എങ്കിൽ (rn->query6 == ശൂന്യ || rn->naddrs6 != (u_short) -1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.data);
            പോകുക failed;
        }

        എങ്കിൽ (trunc && rn->tcp6) {
            ngx_resolver_free(r, name.data);
            പോകുക failed;
        }

        qident = (rn->query6[0] << 8) + rn->query6[1];

        നിർത്തുക;
#endif

    സ്ഥിരസ്ഥിതി: /* NGX_RESOLVE_A */

        എങ്കിൽ (rn->query == ശൂന്യ || rn->naddrs != (u_short) -1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.data);
            പോകുക failed;
        }

        എങ്കിൽ (trunc && rn->tcp) {
            ngx_resolver_free(r, name.data);
            പോകുക failed;
        }

        qident = (rn->query[0] << 8) + rn->query[1];
    }

    എങ്കിൽ (ident != qident) {
        ngx_log_error(r->log_level, r->log, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.data);
        പോകുക failed;
    }

    ngx_resolver_free(r, name.data);

    എങ്കിൽ (trunc) {

        ngx_queue_remove(&rn->queue);

        എങ്കിൽ (rn->waiting == ശൂന്യ) {
            ngx_rbtree_delete(&r->name_rbtree, &rn->കണ്ണി);
            ngx_resolver_free_കണ്ണി(r, rn);
            പോകുക next;
        }

        rec = r->connections.elts;
        rec = &rec[rn->last_connection];

        തിരഞ്ഞെടുപ്പ് (qtype) {

#എങ്കിൽ (NGX_HAVE_INET6)
        അവസ്ഥ NGX_RESOLVE_AAAA:

            rn->tcp6 = 1;

            (ശൂന്യം) ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen);

            നിർത്തുക;
#endif

        സ്ഥിരസ്ഥിതി: /* NGX_RESOLVE_A */

            rn->tcp = 1;

            (ശൂന്യം) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);
        }

        rn->expire = ngx_time() + r->resend_timeout;

        ngx_queue_തിരുകുക_head(&r->name_resend_queue, &rn->queue);

        പോകുക next;
    }

    എങ്കിൽ (code == 0 && rn->code) {
        code = rn->code;
    }

    എങ്കിൽ (code == 0 && nan == 0) {

#എങ്കിൽ (NGX_HAVE_INET6)
        തിരഞ്ഞെടുപ്പ് (qtype) {

        അവസ്ഥ NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            എങ്കിൽ (rn->naddrs == (u_short) -1) {
                പോകുക next;
            }

            എങ്കിൽ (rn->naddrs) {
                പോകുക export;
            }

            നിർത്തുക;

        സ്ഥിരസ്ഥിതി: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            എങ്കിൽ (rn->naddrs6 == (u_short) -1) {
                പോകുക next;
            }

            എങ്കിൽ (rn->naddrs6) {
                പോകുക export;
            }
        }
#endif

        code = NGX_RESOLVE_NXDOMAIN;
    }

    എങ്കിൽ (code) {

#എങ്കിൽ (NGX_HAVE_INET6)
        തിരഞ്ഞെടുപ്പ് (qtype) {

        അവസ്ഥ NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            എങ്കിൽ (rn->naddrs == (u_short) -1) {
                rn->code = (u_char) code;
                പോകുക next;
            }

            നിർത്തുക;

        സ്ഥിരസ്ഥിതി: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            എങ്കിൽ (rn->naddrs6 == (u_short) -1) {
                rn->code = (u_char) code;
                പോകുക next;
            }
        }
#endif

        next = rn->waiting;
        rn->waiting = ശൂന്യ;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(&r->name_rbtree, &rn->കണ്ണി);

        /* unlock name mutex */

        ആകുമ്പോൾ (next) {
            ctx = next;
            ctx->state = code;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            next = ctx->next;

            ctx->handler(ctx);
        }

        ngx_resolver_free_കണ്ണി(r, rn);

        തിരികെ;
    }

    i = ans;
    naddrs = 0;
    cname = ശൂന്യ;

    വേണ്ടി (a = 0; a < nan; a++) {

        start = i;

        ആകുമ്പോൾ (i < n) {

            എങ്കിൽ (buf[i] & 0xc0) {
                i += 2;
                പോകുക found;
            }

            എങ്കിൽ (buf[i] == 0) {
                i++;
                പോകുക test_length;
            }

            i += 1 + buf[i];
        }

        പോകുക short_response;

    test_length:

        എങ്കിൽ (i - start < 2) {
            err = "invalid name in DNS response";
            പോകുക invalid;
        }

    found:

        എങ്കിൽ (i + വലുപ്പം(ngx_resolver_an_t) >= n) {
            പോകുക short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        എങ്കിൽ (class != 1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR class %ui in DNS response", class);
            പോകുക failed;
        }

        എങ്കിൽ (ttl < 0) {
            ttl = 0;
        }

        rn->ttl = ngx_min(rn->ttl, (uint32_t) ttl);

        i += വലുപ്പം(ngx_resolver_an_t);

        തിരഞ്ഞെടുപ്പ് (type) {

        അവസ്ഥ NGX_RESOLVE_A:

            എങ്കിൽ (qtype != NGX_RESOLVE_A) {
                err = "unexpected A record in DNS response";
                പോകുക invalid;
            }

            എങ്കിൽ (len != 4) {
                err = "invalid A record in DNS response";
                പോകുക invalid;
            }

            എങ്കിൽ (i + 4 > n) {
                പോകുക short_response;
            }

            naddrs++;

            നിർത്തുക;

#എങ്കിൽ (NGX_HAVE_INET6)
        അവസ്ഥ NGX_RESOLVE_AAAA:

            എങ്കിൽ (qtype != NGX_RESOLVE_AAAA) {
                err = "unexpected AAAA record in DNS response";
                പോകുക invalid;
            }

            എങ്കിൽ (len != 16) {
                err = "invalid AAAA record in DNS response";
                പോകുക invalid;
            }

            എങ്കിൽ (i + 16 > n) {
                പോകുക short_response;
            }

            naddrs++;

            നിർത്തുക;
#endif

        അവസ്ഥ NGX_RESOLVE_CNAME:

            cname = &buf[i];

            നിർത്തുക;

        അവസ്ഥ NGX_RESOLVE_DNAME:

            നിർത്തുക;

        സ്ഥിരസ്ഥിതി:

            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver naddrs:%ui cname:%p ttl:%uD",
                   naddrs, cname, rn->ttl);

    എങ്കിൽ (naddrs) {

        തിരഞ്ഞെടുപ്പ് (qtype) {

#എങ്കിൽ (NGX_HAVE_INET6)
        അവസ്ഥ NGX_RESOLVE_AAAA:

            എങ്കിൽ (naddrs == 1) {
                addr6 = &rn->u6.addr6;
                rn->naddrs6 = 1;

            } അല്ലെങ്കിൽ {
                addr6 = ngx_resolver_alloc(r, naddrs * വലുപ്പം(ഘടന in6_addr));
                എങ്കിൽ (addr6 == ശൂന്യ) {
                    പോകുക failed;
                }

                rn->u6.addrs6 = addr6;
                rn->naddrs6 = (u_short) naddrs;
            }

#എങ്കിൽ (NGX_SUPPRESS_WARN)
            addr = ശൂന്യ;
#endif

            നിർത്തുക;
#endif

        സ്ഥിരസ്ഥിതി: /* NGX_RESOLVE_A */

            എങ്കിൽ (naddrs == 1) {
                addr = &rn->u.addr;
                rn->naddrs = 1;

            } അല്ലെങ്കിൽ {
                addr = ngx_resolver_alloc(r, naddrs * വലുപ്പം(in_addr_t));
                എങ്കിൽ (addr == ശൂന്യ) {
                    പോകുക failed;
                }

                rn->u.addrs = addr;
                rn->naddrs = (u_short) naddrs;
            }

#എങ്കിൽ (NGX_HAVE_INET6 && NGX_SUPPRESS_WARN)
            addr6 = ശൂന്യ;
#endif
        }

        j = 0;
        i = ans;

        വേണ്ടി (a = 0; a < nan; a++) {

            വേണ്ടി ( ;; ) {

                എങ്കിൽ (buf[i] & 0xc0) {
                    i += 2;
                    നിർത്തുക;
                }

                എങ്കിൽ (buf[i] == 0) {
                    i++;
                    നിർത്തുക;
                }

                i += 1 + buf[i];
            }

            an = (ngx_resolver_an_t *) &buf[i];

            type = (an->type_hi << 8) + an->type_lo;
            len = (an->len_hi << 8) + an->len_lo;

            i += വലുപ്പം(ngx_resolver_an_t);

            എങ്കിൽ (type == NGX_RESOLVE_A) {

                addr[j] = htonl((buf[i] << 24) + (buf[i + 1] << 16)
                                + (buf[i + 2] << 8) + (buf[i + 3]));

                എങ്കിൽ (++j == naddrs) {

#എങ്കിൽ (NGX_HAVE_INET6)
                    എങ്കിൽ (rn->naddrs6 == (u_short) -1) {
                        പോകുക next;
                    }
#endif

                    നിർത്തുക;
                }
            }

#എങ്കിൽ (NGX_HAVE_INET6)
            അല്ലെങ്കിൽ എങ്കിൽ (type == NGX_RESOLVE_AAAA) {

                ngx_memcpy(addr6[j].s6_addr, &buf[i], 16);

                എങ്കിൽ (++j == naddrs) {

                    എങ്കിൽ (rn->naddrs == (u_short) -1) {
                        പോകുക next;
                    }

                    നിർത്തുക;
                }
            }
#endif

            i += len;
        }
    }

    തിരഞ്ഞെടുപ്പ് (qtype) {

#എങ്കിൽ (NGX_HAVE_INET6)
    അവസ്ഥ NGX_RESOLVE_AAAA:

        എങ്കിൽ (rn->naddrs6 == (u_short) -1) {
            rn->naddrs6 = 0;
        }

        നിർത്തുക;
#endif

    സ്ഥിരസ്ഥിതി: /* NGX_RESOLVE_A */

        എങ്കിൽ (rn->naddrs == (u_short) -1) {
            rn->naddrs = 0;
        }
    }

    എങ്കിൽ (rn->naddrs != (u_short) -1
#എങ്കിൽ (NGX_HAVE_INET6)
        && rn->naddrs6 != (u_short) -1
#endif
        && rn->naddrs
#എങ്കിൽ (NGX_HAVE_INET6)
           + rn->naddrs6
#endif
           > 0)
    {

#എങ്കിൽ (NGX_HAVE_INET6)
    export:
#endif

        naddrs = rn->naddrs;
#എങ്കിൽ (NGX_HAVE_INET6)
        naddrs += rn->naddrs6;
#endif

        എങ്കിൽ (naddrs == 1 && rn->naddrs == 1) {
            addrs = ശൂന്യ;

        } അല്ലെങ്കിൽ {
            addrs = ngx_resolver_export(r, rn, 0);
            എങ്കിൽ (addrs == ശൂന്യ) {
                പോകുക failed;
            }
        }

        ngx_queue_remove(&rn->queue);

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_തിരുകുക_head(&r->name_expire_queue, &rn->queue);

        next = rn->waiting;
        rn->waiting = ശൂന്യ;

        /* unlock name mutex */

        ആകുമ്പോൾ (next) {
            ctx = next;
            ctx->state = NGX_OK;
            ctx->valid = rn->valid;
            ctx->naddrs = naddrs;

            എങ്കിൽ (addrs == ശൂന്യ) {
                ctx->addrs = &ctx->addr;
                ctx->addr.sockaddr = (ഘടന sockaddr *) &ctx->sin;
                ctx->addr.socklen = വലുപ്പം(ഘടന sockaddr_in);
                ngx_memzero(&ctx->sin, വലുപ്പം(ഘടന sockaddr_in));
                ctx->sin.sin_family = AF_INET;
                ctx->sin.sin_addr.s_addr = rn->u.addr;

            } അല്ലെങ്കിൽ {
                ctx->addrs = addrs;
            }

            next = ctx->next;

            ctx->handler(ctx);
        }

        എങ്കിൽ (addrs != ശൂന്യ) {
            ngx_resolver_free(r, addrs->sockaddr);
            ngx_resolver_free(r, addrs);
        }

        ngx_resolver_free(r, rn->query);
        rn->query = ശൂന്യ;
#എങ്കിൽ (NGX_HAVE_INET6)
        rn->query6 = ശൂന്യ;
#endif

        തിരികെ;
    }

    എങ്കിൽ (cname) {

        /* CNAME only */

        എങ്കിൽ (rn->naddrs == (u_short) -1
#എങ്കിൽ (NGX_HAVE_INET6)
            || rn->naddrs6 == (u_short) -1
#endif
            )
        {
            പോകുക next;
        }

        എങ്കിൽ (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            പോകുക failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_queue_remove(&rn->queue);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.data;

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_തിരുകുക_head(&r->name_expire_queue, &rn->queue);

        ngx_resolver_free(r, rn->query);
        rn->query = ശൂന്യ;
#എങ്കിൽ (NGX_HAVE_INET6)
        rn->query6 = ശൂന്യ;
#endif

        ctx = rn->waiting;
        rn->waiting = ശൂന്യ;

        എങ്കിൽ (ctx) {

            എങ്കിൽ (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                ചെയ്യുക {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    next = ctx->next;

                    ctx->handler(ctx);

                    ctx = next;
                } ആകുമ്പോൾ (ctx);

                തിരികെ;
            }

            വേണ്ടി (next = ctx; next; next = next->next) {
                next->കണ്ണി = ശൂന്യ;
            }

            (ശൂന്യം) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        തിരികെ;
    }

    ngx_log_error(r->log_level, r->log, 0,
                  "no A or CNAME types in DNS response");
    തിരികെ;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->log, 0, err);

    തിരികെ;

failed:

next:

    /* unlock name mutex */

    തിരികെ;
}


സ്ഥിതം ശൂന്യം
ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans)
{
    അക്ഷരം                       *err;
    u_char                     *cname;
    size_t                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    ngx_str_t                   name;
    ngx_uint_t                  type, qident, class, start, nsrvs, a, i, j;
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *next;
    ngx_resolver_srv_t         *srvs;
    ngx_resolver_കണ്ണി_t        *rn;
    ngx_resolver_connection_t  *rec;

    എങ്കിൽ (ngx_resolver_copy(r, &name, buf,
                          buf + വലുപ്പം(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        തിരികെ;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver qs:%V", &name);

    hash = ngx_crc32_short(name.data, name.len);

    rn = ngx_resolver_lookup_srv(r, &name, hash);

    എങ്കിൽ (rn == ശൂന്യ || rn->query == ശൂന്യ) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.data);
        പോകുക failed;
    }

    എങ്കിൽ (trunc && rn->tcp) {
        ngx_resolver_free(r, name.data);
        പോകുക failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    എങ്കിൽ (ident != qident) {
        ngx_log_error(r->log_level, r->log, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.data);
        പോകുക failed;
    }

    ngx_resolver_free(r, name.data);

    എങ്കിൽ (trunc) {

        ngx_queue_remove(&rn->queue);

        എങ്കിൽ (rn->waiting == ശൂന്യ) {
            ngx_rbtree_delete(&r->srv_rbtree, &rn->കണ്ണി);
            ngx_resolver_free_കണ്ണി(r, rn);
            തിരികെ;
        }

        rec = r->connections.elts;
        rec = &rec[rn->last_connection];

        rn->tcp = 1;

        (ശൂന്യം) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);

        rn->expire = ngx_time() + r->resend_timeout;

        ngx_queue_തിരുകുക_head(&r->srv_resend_queue, &rn->queue);

        തിരികെ;
    }

    എങ്കിൽ (code == 0 && rn->code) {
        code = rn->code;
    }

    എങ്കിൽ (code == 0 && nan == 0) {
        code = NGX_RESOLVE_NXDOMAIN;
    }

    എങ്കിൽ (code) {
        next = rn->waiting;
        rn->waiting = ശൂന്യ;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(&r->srv_rbtree, &rn->കണ്ണി);

        ആകുമ്പോൾ (next) {
            ctx = next;
            ctx->state = code;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            next = ctx->next;

            ctx->handler(ctx);
        }

        ngx_resolver_free_കണ്ണി(r, rn);

        തിരികെ;
    }

    i = ans;
    nsrvs = 0;
    cname = ശൂന്യ;

    വേണ്ടി (a = 0; a < nan; a++) {

        start = i;

        ആകുമ്പോൾ (i < n) {

            എങ്കിൽ (buf[i] & 0xc0) {
                i += 2;
                പോകുക found;
            }

            എങ്കിൽ (buf[i] == 0) {
                i++;
                പോകുക test_length;
            }

            i += 1 + buf[i];
        }

        പോകുക short_response;

    test_length:

        എങ്കിൽ (i - start < 2) {
            err = "invalid name DNS response";
            പോകുക invalid;
        }

    found:

        എങ്കിൽ (i + വലുപ്പം(ngx_resolver_an_t) >= n) {
            പോകുക short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        എങ്കിൽ (class != 1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR class %ui in DNS response", class);
            പോകുക failed;
        }

        എങ്കിൽ (ttl < 0) {
            ttl = 0;
        }

        rn->ttl = ngx_min(rn->ttl, (uint32_t) ttl);

        i += വലുപ്പം(ngx_resolver_an_t);

        തിരഞ്ഞെടുപ്പ് (type) {

        അവസ്ഥ NGX_RESOLVE_SRV:

            എങ്കിൽ (i + 6 >= n) {
                പോകുക short_response;
            }

            എങ്കിൽ (ngx_resolver_copy(r, ശൂന്യ, buf, &buf[i + 6], buf + n)
                != NGX_OK)
            {
                പോകുക failed;
            }

            nsrvs++;

            നിർത്തുക;

        അവസ്ഥ NGX_RESOLVE_CNAME:

            cname = &buf[i];

            നിർത്തുക;

        അവസ്ഥ NGX_RESOLVE_DNAME:

            നിർത്തുക;

        സ്ഥിരസ്ഥിതി:

            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver nsrvs:%ui cname:%p ttl:%uD",
                   nsrvs, cname, rn->ttl);

    എങ്കിൽ (nsrvs) {

        srvs = ngx_resolver_calloc(r, nsrvs * വലുപ്പം(ngx_resolver_srv_t));
        എങ്കിൽ (srvs == ശൂന്യ) {
            പോകുക failed;
        }

        rn->u.srvs = srvs;
        rn->nsrvs = (u_short) nsrvs;

        j = 0;
        i = ans;

        വേണ്ടി (a = 0; a < nan; a++) {

            വേണ്ടി ( ;; ) {

                എങ്കിൽ (buf[i] & 0xc0) {
                    i += 2;
                    നിർത്തുക;
                }

                എങ്കിൽ (buf[i] == 0) {
                    i++;
                    നിർത്തുക;
                }

                i += 1 + buf[i];
            }

            an = (ngx_resolver_an_t *) &buf[i];

            type = (an->type_hi << 8) + an->type_lo;
            len = (an->len_hi << 8) + an->len_lo;

            i += വലുപ്പം(ngx_resolver_an_t);

            എങ്കിൽ (type == NGX_RESOLVE_SRV) {

                srvs[j].priority = (buf[i] << 8) + buf[i + 1];
                srvs[j].weight = (buf[i + 2] << 8) + buf[i + 3];

                എങ്കിൽ (srvs[j].weight == 0) {
                    srvs[j].weight = 1;
                }

                srvs[j].port = (buf[i + 4] << 8) + buf[i + 5];

                എങ്കിൽ (ngx_resolver_copy(r, &srvs[j].name, buf, &buf[i + 6],
                                      buf + n)
                    != NGX_OK)
                {
                    പോകുക failed;
                }

                j++;
            }

            i += len;
        }

        ngx_sort(srvs, nsrvs, വലുപ്പം(ngx_resolver_srv_t),
                 ngx_resolver_cmp_srvs);

        ngx_resolver_free(r, rn->query);
        rn->query = ശൂന്യ;

        ngx_queue_remove(&rn->queue);

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_തിരുകുക_head(&r->srv_expire_queue, &rn->queue);

        next = rn->waiting;
        rn->waiting = ശൂന്യ;

        ആകുമ്പോൾ (next) {
            ctx = next;
            next = ctx->next;

            ngx_resolver_resolve_srv_names(ctx, rn);
        }

        തിരികെ;
    }

    rn->nsrvs = 0;

    എങ്കിൽ (cname) {

        /* CNAME only */

        എങ്കിൽ (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            പോകുക failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_queue_remove(&rn->queue);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.data;

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_തിരുകുക_head(&r->srv_expire_queue, &rn->queue);

        ngx_resolver_free(r, rn->query);
        rn->query = ശൂന്യ;
#എങ്കിൽ (NGX_HAVE_INET6)
        rn->query6 = ശൂന്യ;
#endif

        ctx = rn->waiting;
        rn->waiting = ശൂന്യ;

        എങ്കിൽ (ctx) {

            എങ്കിൽ (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                ചെയ്യുക {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    next = ctx->next;

                    ctx->handler(ctx);

                    ctx = next;
                } ആകുമ്പോൾ (ctx);

                തിരികെ;
            }

            വേണ്ടി (next = ctx; next; next = next->next) {
                next->കണ്ണി = ശൂന്യ;
            }

            (ശൂന്യം) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        തിരികെ;
    }

    ngx_log_error(r->log_level, r->log, 0, "no SRV type in DNS response");

    തിരികെ;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->log, 0, err);

    തിരികെ;

failed:

    /* unlock name mutex */

    തിരികെ;
}


സ്ഥിതം ശൂന്യം
ngx_resolver_resolve_srv_names(ngx_resolver_ctx_t *ctx, ngx_resolver_കണ്ണി_t *rn)
{
    ngx_uint_t                i;
    ngx_resolver_t           *r;
    ngx_resolver_ctx_t       *cctx;
    ngx_resolver_srv_name_t  *srvs;

    r = ctx->resolver;

    ctx->കണ്ണി = ശൂന്യ;
    ctx->state = NGX_OK;
    ctx->valid = rn->valid;
    ctx->എണ്ണം = rn->nsrvs;

    srvs = ngx_resolver_calloc(r, rn->nsrvs * വലുപ്പം(ngx_resolver_srv_name_t));
    എങ്കിൽ (srvs == ശൂന്യ) {
        പോകുക failed;
    }

    ctx->srvs = srvs;
    ctx->nsrvs = rn->nsrvs;

    എങ്കിൽ (ctx->event && ctx->event->timer_set) {
        ngx_del_timer(ctx->event);
    }

    വേണ്ടി (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
        srvs[i].name.data = ngx_resolver_alloc(r, rn->u.srvs[i].name.len);
        എങ്കിൽ (srvs[i].name.data == ശൂന്യ) {
            പോകുക failed;
        }

        srvs[i].name.len = rn->u.srvs[i].name.len;
        ngx_memcpy(srvs[i].name.data, rn->u.srvs[i].name.data,
                   srvs[i].name.len);

        cctx = ngx_resolve_start(r, ശൂന്യ);
        എങ്കിൽ (cctx == ശൂന്യ) {
            പോകുക failed;
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

        എങ്കിൽ (ngx_resolve_name(cctx) == NGX_ERROR) {
            srvs[i].ctx = ശൂന്യ;
            പോകുക failed;
        }
    }

    തിരികെ;

failed:

    ctx->state = NGX_ERROR;
    ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

    ctx->handler(ctx);
}


സ്ഥിതം ശൂന്യം
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

    ctx->എണ്ണം--;
    ctx->async |= cctx->async;

    srv->ctx = ശൂന്യ;
    srv->state = cctx->state;

    എങ്കിൽ (cctx->naddrs) {

        ctx->valid = ngx_min(ctx->valid, cctx->valid);

        addrs = ngx_resolver_calloc(r, cctx->naddrs * വലുപ്പം(ngx_addr_t));
        എങ്കിൽ (addrs == ശൂന്യ) {
            srv->state = NGX_ERROR;
            പോകുക done;
        }

        sockaddr = ngx_resolver_alloc(r, cctx->naddrs * വലുപ്പം(ngx_sockaddr_t));
        എങ്കിൽ (sockaddr == ശൂന്യ) {
            ngx_resolver_free(r, addrs);
            srv->state = NGX_ERROR;
            പോകുക done;
        }

        വേണ്ടി (i = 0; i < cctx->naddrs; i++) {
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

    എങ്കിൽ (ctx->എണ്ണം == 0) {
        ngx_resolver_report_srv(r, ctx);
    }
}


സ്ഥിതം ശൂന്യം
ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan)
{
    അക്ഷരം                 *err;
    size_t                len;
    in_addr_t             addr;
    int32_t               ttl;
    ngx_int_t             octet;
    ngx_str_t             name;
    ngx_uint_t            mask, type, class, qident, a, i, start;
    ngx_queue_t          *expire_queue;
    ngx_rbtree_t         *വൃക്ഷം;
    ngx_resolver_an_t    *an;
    ngx_resolver_ctx_t   *ctx, *next;
    ngx_resolver_കണ്ണി_t  *rn;
#എങ്കിൽ (NGX_HAVE_INET6)
    uint32_t              hash;
    ngx_int_t             digit;
    ഘടന in6_addr       addr6;
#endif

    എങ്കിൽ (ngx_resolver_copy(r, &name, buf,
                          buf + വലുപ്പം(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        തിരികെ;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver qs:%V", &name);

    /* AF_INET */

    addr = 0;
    i = വലുപ്പം(ngx_resolver_hdr_t);

    വേണ്ടി (mask = 0; mask < 32; mask += 8) {
        len = buf[i++];

        octet = ngx_atoi(&buf[i], len);
        എങ്കിൽ (octet == NGX_ERROR || octet > 255) {
            പോകുക invalid_in_addr_arpa;
        }

        addr += octet << mask;
        i += len;
    }

    എങ്കിൽ (ngx_strcasecmp(&buf[i], (u_char *) "\7in-addr\4arpa") == 0) {
        i += വലുപ്പം("\7in-addr\4arpa");

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        വൃക്ഷം = &r->addr_rbtree;
        expire_queue = &r->addr_expire_queue;

        പോകുക valid;
    }

invalid_in_addr_arpa:

#എങ്കിൽ (NGX_HAVE_INET6)

    i = വലുപ്പം(ngx_resolver_hdr_t);

    വേണ്ടി (octet = 15; octet >= 0; octet--) {
        എങ്കിൽ (buf[i++] != '\1') {
            പോകുക invalid_ip6_arpa;
        }

        digit = ngx_hextoi(&buf[i++], 1);
        എങ്കിൽ (digit == NGX_ERROR) {
            പോകുക invalid_ip6_arpa;
        }

        addr6.s6_addr[octet] = (u_char) digit;

        എങ്കിൽ (buf[i++] != '\1') {
            പോകുക invalid_ip6_arpa;
        }

        digit = ngx_hextoi(&buf[i++], 1);
        എങ്കിൽ (digit == NGX_ERROR) {
            പോകുക invalid_ip6_arpa;
        }

        addr6.s6_addr[octet] += (u_char) (digit * 16);
    }

    എങ്കിൽ (ngx_strcasecmp(&buf[i], (u_char *) "\3ip6\4arpa") == 0) {
        i += വലുപ്പം("\3ip6\4arpa");

        /* lock addr mutex */

        hash = ngx_crc32_short(addr6.s6_addr, 16);
        rn = ngx_resolver_lookup_addr6(r, &addr6, hash);

        വൃക്ഷം = &r->addr6_rbtree;
        expire_queue = &r->addr6_expire_queue;

        പോകുക valid;
    }

invalid_ip6_arpa:
#endif

    ngx_log_error(r->log_level, r->log, 0,
                  "invalid in-addr.arpa or ip6.arpa name in DNS response");
    ngx_resolver_free(r, name.data);
    തിരികെ;

valid:

    എങ്കിൽ (rn == ശൂന്യ || rn->query == ശൂന്യ) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.data);
        പോകുക failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    എങ്കിൽ (ident != qident) {
        ngx_log_error(r->log_level, r->log, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.data);
        പോകുക failed;
    }

    ngx_resolver_free(r, name.data);

    എങ്കിൽ (code == 0 && nan == 0) {
        code = NGX_RESOLVE_NXDOMAIN;
    }

    എങ്കിൽ (code) {
        next = rn->waiting;
        rn->waiting = ശൂന്യ;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(വൃക്ഷം, &rn->കണ്ണി);

        /* unlock addr mutex */

        ആകുമ്പോൾ (next) {
            ctx = next;
            ctx->state = code;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            next = ctx->next;

            ctx->handler(ctx);
        }

        ngx_resolver_free_കണ്ണി(r, rn);

        തിരികെ;
    }

    i += വലുപ്പം(ngx_resolver_qs_t);

    വേണ്ടി (a = 0; a < nan; a++) {

        start = i;

        ആകുമ്പോൾ (i < n) {

            എങ്കിൽ (buf[i] & 0xc0) {
                i += 2;
                പോകുക found;
            }

            എങ്കിൽ (buf[i] == 0) {
                i++;
                പോകുക test_length;
            }

            i += 1 + buf[i];
        }

        പോകുക short_response;

    test_length:

        എങ്കിൽ (i - start < 2) {
            err = "invalid name in DNS response";
            പോകുക invalid;
        }

    found:

        എങ്കിൽ (i + വലുപ്പം(ngx_resolver_an_t) >= n) {
            പോകുക short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        എങ്കിൽ (class != 1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR class %ui in DNS response", class);
            പോകുക failed;
        }

        എങ്കിൽ (ttl < 0) {
            ttl = 0;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                      "resolver qt:%ui cl:%ui len:%uz",
                      type, class, len);

        i += വലുപ്പം(ngx_resolver_an_t);

        തിരഞ്ഞെടുപ്പ് (type) {

        അവസ്ഥ NGX_RESOLVE_PTR:

            പോകുക ptr;

        അവസ്ഥ NGX_RESOLVE_CNAME:

            നിർത്തുക;

        സ്ഥിരസ്ഥിതി:

            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->log, 0,
                  "no PTR type in DNS response");
    തിരികെ;

ptr:

    എങ്കിൽ (ngx_resolver_copy(r, &name, buf, buf + i, buf + n) != NGX_OK) {
        പോകുക failed;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver an:%V", &name);

    എങ്കിൽ (name.len != (size_t) rn->nlen
        || ngx_strncmp(name.data, rn->name, name.len) != 0)
    {
        എങ്കിൽ (rn->nlen) {
            ngx_resolver_free(r, rn->name);
        }

        rn->nlen = (u_short) name.len;
        rn->name = name.data;

        name.data = ngx_resolver_dup(r, rn->name, name.len);
        എങ്കിൽ (name.data == ശൂന്യ) {
            പോകുക failed;
        }
    }

    ngx_queue_remove(&rn->queue);

    rn->valid = ngx_time() + (r->valid ? r->valid : ttl);
    rn->expire = ngx_time() + r->expire;

    ngx_queue_തിരുകുക_head(expire_queue, &rn->queue);

    next = rn->waiting;
    rn->waiting = ശൂന്യ;

    /* unlock addr mutex */

    ആകുമ്പോൾ (next) {
        ctx = next;
        ctx->state = NGX_OK;
        ctx->valid = rn->valid;
        ctx->name = name;
        next = ctx->next;

        ctx->handler(ctx);
    }

    ngx_resolver_free(r, name.data);

    തിരികെ;

short_response:

    err = "short DNS response";

invalid:

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->log, 0, err);

    തിരികെ;

failed:

    /* unlock addr mutex */

    തിരികെ;
}


സ്ഥിതം ngx_resolver_കണ്ണി_t *
ngx_resolver_lookup_name(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_കണ്ണി_t    *കണ്ണി, *sentinel;
    ngx_resolver_കണ്ണി_t  *rn;

    കണ്ണി = r->name_rbtree.വേര്;
    sentinel = r->name_rbtree.sentinel;

    ആകുമ്പോൾ (കണ്ണി != sentinel) {

        എങ്കിൽ (hash < കണ്ണി->താക്കോൽ) {
            കണ്ണി = കണ്ണി->ഇടത്;
            തുടരുക;
        }

        എങ്കിൽ (hash > കണ്ണി->താക്കോൽ) {
            കണ്ണി = കണ്ണി->വലത്;
            തുടരുക;
        }

        /* hash == node->key */

        rn = ngx_resolver_കണ്ണി(കണ്ണി);

        rc = ngx_memn2cmp(name->data, rn->name, name->len, rn->nlen);

        എങ്കിൽ (rc == 0) {
            തിരികെ rn;
        }

        കണ്ണി = (rc < 0) ? കണ്ണി->ഇടത് : കണ്ണി->വലത്;
    }

    /* not found */

    തിരികെ ശൂന്യ;
}


സ്ഥിതം ngx_resolver_കണ്ണി_t *
ngx_resolver_lookup_srv(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_കണ്ണി_t    *കണ്ണി, *sentinel;
    ngx_resolver_കണ്ണി_t  *rn;

    കണ്ണി = r->srv_rbtree.വേര്;
    sentinel = r->srv_rbtree.sentinel;

    ആകുമ്പോൾ (കണ്ണി != sentinel) {

        എങ്കിൽ (hash < കണ്ണി->താക്കോൽ) {
            കണ്ണി = കണ്ണി->ഇടത്;
            തുടരുക;
        }

        എങ്കിൽ (hash > കണ്ണി->താക്കോൽ) {
            കണ്ണി = കണ്ണി->വലത്;
            തുടരുക;
        }

        /* hash == node->key */

        rn = ngx_resolver_കണ്ണി(കണ്ണി);

        rc = ngx_memn2cmp(name->data, rn->name, name->len, rn->nlen);

        എങ്കിൽ (rc == 0) {
            തിരികെ rn;
        }

        കണ്ണി = (rc < 0) ? കണ്ണി->ഇടത് : കണ്ണി->വലത്;
    }

    /* not found */

    തിരികെ ശൂന്യ;
}


സ്ഥിതം ngx_resolver_കണ്ണി_t *
ngx_resolver_lookup_addr(ngx_resolver_t *r, in_addr_t addr)
{
    ngx_rbtree_കണ്ണി_t  *കണ്ണി, *sentinel;

    കണ്ണി = r->addr_rbtree.വേര്;
    sentinel = r->addr_rbtree.sentinel;

    ആകുമ്പോൾ (കണ്ണി != sentinel) {

        എങ്കിൽ (addr < കണ്ണി->താക്കോൽ) {
            കണ്ണി = കണ്ണി->ഇടത്;
            തുടരുക;
        }

        എങ്കിൽ (addr > കണ്ണി->താക്കോൽ) {
            കണ്ണി = കണ്ണി->വലത്;
            തുടരുക;
        }

        /* addr == node->key */

        തിരികെ ngx_resolver_കണ്ണി(കണ്ണി);
    }

    /* not found */

    തിരികെ ശൂന്യ;
}


#എങ്കിൽ (NGX_HAVE_INET6)

സ്ഥിതം ngx_resolver_കണ്ണി_t *
ngx_resolver_lookup_addr6(ngx_resolver_t *r, ഘടന in6_addr *addr,
    uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_കണ്ണി_t    *കണ്ണി, *sentinel;
    ngx_resolver_കണ്ണി_t  *rn;

    കണ്ണി = r->addr6_rbtree.വേര്;
    sentinel = r->addr6_rbtree.sentinel;

    ആകുമ്പോൾ (കണ്ണി != sentinel) {

        എങ്കിൽ (hash < കണ്ണി->താക്കോൽ) {
            കണ്ണി = കണ്ണി->ഇടത്;
            തുടരുക;
        }

        എങ്കിൽ (hash > കണ്ണി->താക്കോൽ) {
            കണ്ണി = കണ്ണി->വലത്;
            തുടരുക;
        }

        /* hash == node->key */

        rn = ngx_resolver_കണ്ണി(കണ്ണി);

        rc = ngx_memcmp(addr, &rn->addr6, 16);

        എങ്കിൽ (rc == 0) {
            തിരികെ rn;
        }

        കണ്ണി = (rc < 0) ? കണ്ണി->ഇടത് : കണ്ണി->വലത്;
    }

    /* not found */

    തിരികെ ശൂന്യ;
}

#endif


സ്ഥിതം ശൂന്യം
ngx_resolver_rbtree_തിരുകുക_മൂല്യം(ngx_rbtree_കണ്ണി_t *temp,
    ngx_rbtree_കണ്ണി_t *കണ്ണി, ngx_rbtree_കണ്ണി_t *sentinel)
{
    ngx_rbtree_കണ്ണി_t    **p;
    ngx_resolver_കണ്ണി_t   *rn, *rn_temp;

    വേണ്ടി ( ;; ) {

        എങ്കിൽ (കണ്ണി->താക്കോൽ < temp->താക്കോൽ) {

            p = &temp->ഇടത്;

        } അല്ലെങ്കിൽ എങ്കിൽ (കണ്ണി->താക്കോൽ > temp->താക്കോൽ) {

            p = &temp->വലത്;

        } അല്ലെങ്കിൽ { /* node->key == temp->key */

            rn = ngx_resolver_കണ്ണി(കണ്ണി);
            rn_temp = ngx_resolver_കണ്ണി(temp);

            p = (ngx_memn2cmp(rn->name, rn_temp->name, rn->nlen, rn_temp->nlen)
                 < 0) ? &temp->ഇടത് : &temp->വലത്;
        }

        എങ്കിൽ (*p == sentinel) {
            നിർത്തുക;
        }

        temp = *p;
    }

    *p = കണ്ണി;
    കണ്ണി->parent = temp;
    കണ്ണി->ഇടത് = sentinel;
    കണ്ണി->വലത് = sentinel;
    ngx_rbt_red(കണ്ണി);
}


#എങ്കിൽ (NGX_HAVE_INET6)

സ്ഥിതം ശൂന്യം
ngx_resolver_rbtree_തിരുകുക_addr6_മൂല്യം(ngx_rbtree_കണ്ണി_t *temp,
    ngx_rbtree_കണ്ണി_t *കണ്ണി, ngx_rbtree_കണ്ണി_t *sentinel)
{
    ngx_rbtree_കണ്ണി_t    **p;
    ngx_resolver_കണ്ണി_t   *rn, *rn_temp;

    വേണ്ടി ( ;; ) {

        എങ്കിൽ (കണ്ണി->താക്കോൽ < temp->താക്കോൽ) {

            p = &temp->ഇടത്;

        } അല്ലെങ്കിൽ എങ്കിൽ (കണ്ണി->താക്കോൽ > temp->താക്കോൽ) {

            p = &temp->വലത്;

        } അല്ലെങ്കിൽ { /* node->key == temp->key */

            rn = ngx_resolver_കണ്ണി(കണ്ണി);
            rn_temp = ngx_resolver_കണ്ണി(temp);

            p = (ngx_memcmp(&rn->addr6, &rn_temp->addr6, 16)
                 < 0) ? &temp->ഇടത് : &temp->വലത്;
        }

        എങ്കിൽ (*p == sentinel) {
            നിർത്തുക;
        }

        temp = *p;
    }

    *p = കണ്ണി;
    കണ്ണി->parent = temp;
    കണ്ണി->ഇടത് = sentinel;
    കണ്ണി->വലത് = sentinel;
    ngx_rbt_red(കണ്ണി);
}

#endif


സ്ഥിതം ngx_int_t
ngx_resolver_create_name_query(ngx_resolver_t *r, ngx_resolver_കണ്ണി_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    size_t               len, nlen;
    ngx_uint_t           ident;
    ngx_resolver_qs_t   *qs;
    ngx_resolver_hdr_t  *query;

    nlen = name->len ? (1 + name->len + 1) : 1;

    len = വലുപ്പം(ngx_resolver_hdr_t) + nlen + വലുപ്പം(ngx_resolver_qs_t);

#എങ്കിൽ (NGX_HAVE_INET6)
    p = ngx_resolver_alloc(r, len * (r->ipv4 + r->ipv6));
#അല്ലെങ്കിൽ
    p = ngx_resolver_alloc(r, len);
#endif
    എങ്കിൽ (p == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    rn->qlen = (u_short) len;
    rn->query = p;

#എങ്കിൽ (NGX_HAVE_INET6)
    എങ്കിൽ (r->ipv6) {
        rn->query6 = r->ipv4 ? (p + len) : p;
    }
#endif

    query = (ngx_resolver_hdr_t *) p;

    എങ്കിൽ (r->ipv4) {
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

    p += വലുപ്പം(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    /* query type */
    qs->type_hi = 0; qs->type_lo = NGX_RESOLVE_A;

    /* IN query class */
    qs->class_hi = 0; qs->class_lo = 1;

    /* convert "www.example.com" to "\3www\7example\3com\0" */

    len = 0;
    p--;
    *p-- = '\0';

    എങ്കിൽ (name->len == 0)  {
        തിരികെ NGX_DECLINED;
    }

    വേണ്ടി (s = name->data + name->len - 1; s >= name->data; s--) {
        എങ്കിൽ (*s != '.') {
            *p = *s;
            len++;

        } അല്ലെങ്കിൽ {
            എങ്കിൽ (len == 0 || len > 255) {
                തിരികെ NGX_DECLINED;
            }

            *p = (u_char) len;
            len = 0;
        }

        p--;
    }

    എങ്കിൽ (len == 0 || len > 255) {
        തിരികെ NGX_DECLINED;
    }

    *p = (u_char) len;

#എങ്കിൽ (NGX_HAVE_INET6)
    എങ്കിൽ (!r->ipv6) {
        തിരികെ NGX_OK;
    }

    p = rn->query6;

    എങ്കിൽ (r->ipv4) {
        ngx_memcpy(p, rn->query, rn->qlen);
    }

    query = (ngx_resolver_hdr_t *) p;

    ident = ngx_random();

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve: \"%V\" AAAA %i", name, ident & 0xffff);

    query->ident_hi = (u_char) ((ident >> 8) & 0xff);
    query->ident_lo = (u_char) (ident & 0xff);

    p += വലുപ്പം(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    qs->type_lo = NGX_RESOLVE_AAAA;
#endif

    തിരികെ NGX_OK;
}


സ്ഥിതം ngx_int_t
ngx_resolver_create_srv_query(ngx_resolver_t *r, ngx_resolver_കണ്ണി_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    size_t               len, nlen;
    ngx_uint_t           ident;
    ngx_resolver_qs_t   *qs;
    ngx_resolver_hdr_t  *query;

    nlen = name->len ? (1 + name->len + 1) : 1;

    len = വലുപ്പം(ngx_resolver_hdr_t) + nlen + വലുപ്പം(ngx_resolver_qs_t);

    p = ngx_resolver_alloc(r, len);
    എങ്കിൽ (p == ശൂന്യ) {
        തിരികെ NGX_ERROR;
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

    p += വലുപ്പം(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    /* query type */
    qs->type_hi = 0; qs->type_lo = NGX_RESOLVE_SRV;

    /* IN query class */
    qs->class_hi = 0; qs->class_lo = 1;

    /* converts "www.example.com" to "\3www\7example\3com\0" */

    len = 0;
    p--;
    *p-- = '\0';

    എങ്കിൽ (name->len == 0)  {
        തിരികെ NGX_DECLINED;
    }

    വേണ്ടി (s = name->data + name->len - 1; s >= name->data; s--) {
        എങ്കിൽ (*s != '.') {
            *p = *s;
            len++;

        } അല്ലെങ്കിൽ {
            എങ്കിൽ (len == 0 || len > 255) {
                തിരികെ NGX_DECLINED;
            }

            *p = (u_char) len;
            len = 0;
        }

        p--;
    }

    എങ്കിൽ (len == 0 || len > 255) {
        തിരികെ NGX_DECLINED;
    }

    *p = (u_char) len;

    തിരികെ NGX_OK;
}


സ്ഥിതം ngx_int_t
ngx_resolver_create_addr_query(ngx_resolver_t *r, ngx_resolver_കണ്ണി_t *rn,
    ngx_resolver_addr_t *addr)
{
    u_char               *p, *d;
    size_t                len;
    in_addr_t             inaddr;
    ngx_int_t             n;
    ngx_uint_t            ident;
    ngx_resolver_hdr_t   *query;
    ഘടന sockaddr_in   *sin;
#എങ്കിൽ (NGX_HAVE_INET6)
    ഘടന sockaddr_in6  *sin6;
#endif

    തിരഞ്ഞെടുപ്പ് (addr->sockaddr->sa_family) {

#എങ്കിൽ (NGX_HAVE_INET6)
    അവസ്ഥ AF_INET6:
        len = വലുപ്പം(ngx_resolver_hdr_t)
              + 64 + വലുപ്പം(".ip6.arpa.") - 1
              + വലുപ്പം(ngx_resolver_qs_t);

        നിർത്തുക;
#endif

    സ്ഥിരസ്ഥിതി: /* AF_INET */
        len = വലുപ്പം(ngx_resolver_hdr_t)
              + വലുപ്പം(".255.255.255.255.in-addr.arpa.") - 1
              + വലുപ്പം(ngx_resolver_qs_t);
    }

    p = ngx_resolver_alloc(r, len);
    എങ്കിൽ (p == ശൂന്യ) {
        തിരികെ NGX_ERROR;
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

    p += വലുപ്പം(ngx_resolver_hdr_t);

    തിരഞ്ഞെടുപ്പ് (addr->sockaddr->sa_family) {

#എങ്കിൽ (NGX_HAVE_INET6)
    അവസ്ഥ AF_INET6:
        sin6 = (ഘടന sockaddr_in6 *) addr->sockaddr;

        വേണ്ടി (n = 15; n >= 0; n--) {
            p = ngx_sprintf(p, "\1%xd\1%xd",
                            sin6->sin6_addr.s6_addr[n] & 0xf,
                            (sin6->sin6_addr.s6_addr[n] >> 4) & 0xf);
        }

        p = ngx_cpymem(p, "\3ip6\4arpa\0", 10);

        നിർത്തുക;
#endif

    സ്ഥിരസ്ഥിതി: /* AF_INET */

        sin = (ഘടന sockaddr_in *) addr->sockaddr;
        inaddr = ntohl(sin->sin_addr.s_addr);

        വേണ്ടി (n = 0; n < 32; n += 8) {
            d = ngx_sprintf(&p[1], "%ud", (inaddr >> n) & 0xff);
            *p = (u_char) (d - &p[1]);
            p = d;
        }

        p = ngx_cpymem(p, "\7in-addr\4arpa\0", 14);
    }

    /* query type "PTR", IN query class */
    p = ngx_cpymem(p, "\0\14\0\1", 4);

    rn->qlen = (u_short) (p - rn->query);

    തിരികെ NGX_OK;
}


സ്ഥിതം ngx_int_t
ngx_resolver_copy(ngx_resolver_t *r, ngx_str_t *name, u_char *buf, u_char *src,
    u_char *last)
{
    അക്ഷരം        *err;
    u_char      *p, *dst;
    size_t       len;
    ngx_uint_t   i, n;

    p = src;
    len = 0;

    /*
     * compression pointers allow to create endless loop, so we set limit;
     * 128 pointers should be enough to store 255-byte name
     */

    വേണ്ടി (i = 0; i < 128; i++) {
        n = *p++;

        എങ്കിൽ (n == 0) {
            പോകുക done;
        }

        എങ്കിൽ (n & 0xc0) {
            എങ്കിൽ ((n & 0xc0) != 0xc0) {
                err = "invalid label type in DNS response";
                പോകുക invalid;
            }

            എങ്കിൽ (p >= last) {
                err = "name is out of DNS response";
                പോകുക invalid;
            }

            n = ((n & 0x3f) << 8) + *p;
            p = &buf[n];

        } അല്ലെങ്കിൽ {
            len += 1 + n;
            p = &p[n];
        }

        എങ്കിൽ (p >= last) {
            err = "name is out of DNS response";
            പോകുക invalid;
        }
    }

    err = "compression pointers loop in DNS response";

invalid:

    ngx_log_error(r->log_level, r->log, 0, err);

    തിരികെ NGX_ERROR;

done:

    എങ്കിൽ (name == ശൂന്യ) {
        തിരികെ NGX_OK;
    }

    എങ്കിൽ (len == 0) {
        ngx_str_null(name);
        തിരികെ NGX_OK;
    }

    dst = ngx_resolver_alloc(r, len);
    എങ്കിൽ (dst == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    name->data = dst;

    വേണ്ടി ( ;; ) {
        n = *src++;

        എങ്കിൽ (n == 0) {
            name->len = dst - name->data - 1;
            തിരികെ NGX_OK;
        }

        എങ്കിൽ (n & 0xc0) {
            n = ((n & 0x3f) << 8) + *src;
            src = &buf[n];

        } അല്ലെങ്കിൽ {
            ngx_strlow(dst, src, n);
            dst += n;
            src += n;
            *dst++ = '.';
        }
    }
}


സ്ഥിതം ngx_int_t
ngx_resolver_set_timeout(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx)
{
    എങ്കിൽ (ctx->event || ctx->timeout == 0) {
        തിരികെ NGX_OK;
    }

    ctx->event = ngx_resolver_calloc(r, വലുപ്പം(ngx_event_t));
    എങ്കിൽ (ctx->event == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    ctx->event->handler = ngx_resolver_timeout_handler;
    ctx->event->data = ctx;
    ctx->event->log = r->log;
    ctx->event->cancelable = ctx->cancelable;
    ctx->ident = -1;

    ngx_add_timer(ctx->event, ctx->timeout);

    തിരികെ NGX_OK;
}


സ്ഥിതം ശൂന്യം
ngx_resolver_timeout_handler(ngx_event_t *ev)
{
    ngx_resolver_ctx_t  *ctx;

    ctx = ev->data;

    ctx->state = NGX_RESOLVE_TIMEDOUT;

    ctx->handler(ctx);
}


സ്ഥിതം ശൂന്യം
ngx_resolver_free_കണ്ണി(ngx_resolver_t *r, ngx_resolver_കണ്ണി_t *rn)
{
    ngx_uint_t  i;

    /* lock alloc mutex */

    എങ്കിൽ (rn->query) {
        ngx_resolver_free_locked(r, rn->query);
    }

    എങ്കിൽ (rn->name) {
        ngx_resolver_free_locked(r, rn->name);
    }

    എങ്കിൽ (rn->cnlen) {
        ngx_resolver_free_locked(r, rn->u.cname);
    }

    എങ്കിൽ (rn->naddrs > 1 && rn->naddrs != (u_short) -1) {
        ngx_resolver_free_locked(r, rn->u.addrs);
    }

#എങ്കിൽ (NGX_HAVE_INET6)
    എങ്കിൽ (rn->naddrs6 > 1 && rn->naddrs6 != (u_short) -1) {
        ngx_resolver_free_locked(r, rn->u6.addrs6);
    }
#endif

    എങ്കിൽ (rn->nsrvs) {
        വേണ്ടി (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
            എങ്കിൽ (rn->u.srvs[i].name.data) {
                ngx_resolver_free_locked(r, rn->u.srvs[i].name.data);
            }
        }

        ngx_resolver_free_locked(r, rn->u.srvs);
    }

    ngx_resolver_free_locked(r, rn);

    /* unlock alloc mutex */
}


സ്ഥിതം ശൂന്യം *
ngx_resolver_alloc(ngx_resolver_t *r, size_t size)
{
    u_char  *p;

    /* lock alloc mutex */

    p = ngx_alloc(size, r->log);

    /* unlock alloc mutex */

    തിരികെ p;
}


സ്ഥിതം ശൂന്യം *
ngx_resolver_calloc(ngx_resolver_t *r, size_t size)
{
    u_char  *p;

    p = ngx_resolver_alloc(r, size);

    എങ്കിൽ (p) {
        ngx_memzero(p, size);
    }

    തിരികെ p;
}


സ്ഥിതം ശൂന്യം
ngx_resolver_free(ngx_resolver_t *r, ശൂന്യം *p)
{
    /* lock alloc mutex */

    ngx_free(p);

    /* unlock alloc mutex */
}


സ്ഥിതം ശൂന്യം
ngx_resolver_free_locked(ngx_resolver_t *r, ശൂന്യം *p)
{
    ngx_free(p);
}


സ്ഥിതം ശൂന്യം *
ngx_resolver_dup(ngx_resolver_t *r, ശൂന്യം *src, size_t size)
{
    ശൂന്യം  *dst;

    dst = ngx_resolver_alloc(r, size);

    എങ്കിൽ (dst == ശൂന്യ) {
        തിരികെ dst;
    }

    ngx_memcpy(dst, src, size);

    തിരികെ dst;
}


സ്ഥിതം ngx_resolver_addr_t *
ngx_resolver_export(ngx_resolver_t *r, ngx_resolver_കണ്ണി_t *rn,
    ngx_uint_t rotate)
{
    ngx_uint_t            d, i, j, n;
    in_addr_t            *addr;
    ngx_sockaddr_t       *sockaddr;
    ഘടന sockaddr_in   *sin;
    ngx_resolver_addr_t  *dst;
#എങ്കിൽ (NGX_HAVE_INET6)
    ഘടന in6_addr      *addr6;
    ഘടന sockaddr_in6  *sin6;
#endif

    n = rn->naddrs;
#എങ്കിൽ (NGX_HAVE_INET6)
    n += rn->naddrs6;
#endif

    dst = ngx_resolver_calloc(r, n * വലുപ്പം(ngx_resolver_addr_t));
    എങ്കിൽ (dst == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    sockaddr = ngx_resolver_calloc(r, n * വലുപ്പം(ngx_sockaddr_t));
    എങ്കിൽ (sockaddr == ശൂന്യ) {
        ngx_resolver_free(r, dst);
        തിരികെ ശൂന്യ;
    }

    i = 0;
    d = rotate ? ngx_random() % n : 0;

    എങ്കിൽ (rn->naddrs) {
        j = rotate ? ngx_random() % rn->naddrs : 0;

        addr = (rn->naddrs == 1) ? &rn->u.addr : rn->u.addrs;

        ചെയ്യുക {
            sin = &sockaddr[d].sockaddr_in;
            sin->sin_family = AF_INET;
            sin->sin_addr.s_addr = addr[j++];
            dst[d].sockaddr = (ഘടന sockaddr *) sin;
            dst[d++].socklen = വലുപ്പം(ഘടന sockaddr_in);

            എങ്കിൽ (d == n) {
                d = 0;
            }

            എങ്കിൽ (j == (ngx_uint_t) rn->naddrs) {
                j = 0;
            }
        } ആകുമ്പോൾ (++i < (ngx_uint_t) rn->naddrs);
    }

#എങ്കിൽ (NGX_HAVE_INET6)
    എങ്കിൽ (rn->naddrs6) {
        j = rotate ? ngx_random() % rn->naddrs6 : 0;

        addr6 = (rn->naddrs6 == 1) ? &rn->u6.addr6 : rn->u6.addrs6;

        ചെയ്യുക {
            sin6 = &sockaddr[d].sockaddr_in6;
            sin6->sin6_family = AF_INET6;
            ngx_memcpy(sin6->sin6_addr.s6_addr, addr6[j++].s6_addr, 16);
            dst[d].sockaddr = (ഘടന sockaddr *) sin6;
            dst[d++].socklen = വലുപ്പം(ഘടന sockaddr_in6);

            എങ്കിൽ (d == n) {
                d = 0;
            }

            എങ്കിൽ (j == rn->naddrs6) {
                j = 0;
            }
        } ആകുമ്പോൾ (++i < n);
    }
#endif

    തിരികെ dst;
}


സ്ഥിതം ശൂന്യം
ngx_resolver_report_srv(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx)
{
    ngx_uint_t                naddrs, nsrvs, nw, i, j, k, l, m, n, w;
    ngx_resolver_addr_t      *addrs;
    ngx_resolver_srv_name_t  *srvs;

    srvs = ctx->srvs;
    nsrvs = ctx->nsrvs;

    naddrs = 0;

    വേണ്ടി (i = 0; i < nsrvs; i++) {
        എങ്കിൽ (srvs[i].state == NGX_ERROR) {
            ctx->state = NGX_ERROR;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

            ctx->handler(ctx);
            തിരികെ;
        }

        naddrs += srvs[i].naddrs;
    }

    എങ്കിൽ (naddrs == 0) {
        ctx->state = srvs[0].state;

        വേണ്ടി (i = 0; i < nsrvs; i++) {
            എങ്കിൽ (srvs[i].state == NGX_RESOLVE_NXDOMAIN) {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                നിർത്തുക;
            }
        }

        ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        തിരികെ;
    }

    addrs = ngx_resolver_calloc(r, naddrs * വലുപ്പം(ngx_resolver_addr_t));
    എങ്കിൽ (addrs == ശൂന്യ) {
        ctx->state = NGX_ERROR;
        ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        തിരികെ;
    }

    i = 0;
    n = 0;

    ചെയ്യുക {
        nw = 0;

        വേണ്ടി (j = i; j < nsrvs; j++) {
            എങ്കിൽ (srvs[j].priority != srvs[i].priority) {
                നിർത്തുക;
            }

            nw += srvs[j].naddrs * srvs[j].weight;
        }

        എങ്കിൽ (nw == 0) {
            പോകുക next_srv;
        }

        w = ngx_random() % nw;

        വേണ്ടി (k = i; k < j; k++) {
            എങ്കിൽ (w < srvs[k].naddrs * srvs[k].weight) {
                നിർത്തുക;
            }

            w -= srvs[k].naddrs * srvs[k].weight;
        }

        വേണ്ടി (l = i; l < j; l++) {

            വേണ്ടി (m = 0; m < srvs[k].naddrs; m++) {
                addrs[n].socklen = srvs[k].addrs[m].socklen;
                addrs[n].sockaddr = srvs[k].addrs[m].sockaddr;
                addrs[n].name = srvs[k].name;
                addrs[n].priority = srvs[k].priority;
                addrs[n].weight = srvs[k].weight;
                n++;
            }

            എങ്കിൽ (++k == j) {
                k = i;
            }
        }

next_srv:

        i = j;

    } ആകുമ്പോൾ (i < ctx->nsrvs);

    ctx->state = NGX_OK;
    ctx->addrs = addrs;
    ctx->naddrs = naddrs;

    ctx->handler(ctx);

    ngx_resolver_free(r, addrs);
}


അക്ഷരം *
ngx_resolver_strerror(ngx_int_t err)
{
    സ്ഥിതം അക്ഷരം *errors[] = {
        "Format error",     /* FORMERR */
        "Server failure",   /* SERVFAIL */
        "Host not found",   /* NXDOMAIN */
        "Unimplemented",    /* NOTIMP */
        "Operation refused" /* REFUSED */
    };

    എങ്കിൽ (err > 0 && err < 6) {
        തിരികെ errors[err - 1];
    }

    എങ്കിൽ (err == NGX_RESOLVE_TIMEDOUT) {
        തിരികെ "Operation timed out";
    }

    തിരികെ "Unknown error";
}


സ്ഥിതം u_char *
ngx_resolver_log_error(ngx_log_t *log, u_char *buf, size_t len)
{
    u_char                     *p;
    ngx_resolver_connection_t  *rec;

    p = buf;

    എങ്കിൽ (log->action) {
        p = ngx_snprintf(buf, len, " while %s", log->action);
        len -= p - buf;
    }

    rec = log->data;

    എങ്കിൽ (rec) {
        p = ngx_snprintf(p, len, ", resolver: %V", &rec->server);
    }

    തിരികെ p;
}


സ്ഥിതം ngx_int_t
ngx_udp_connect(ngx_resolver_connection_t *rec)
{
    പൂർണ്ണസംഖ്യ                rc;
    ngx_int_t          event;
    ngx_event_t       *rev, *wev;
    ngx_socket_t       s;
    ngx_connection_t  *c;

    s = ngx_socket(rec->sockaddr->sa_family, SOCK_DGRAM, 0);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "UDP socket %d", s);

    എങ്കിൽ (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_socket_n " failed");
        തിരികെ NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->log);

    എങ്കിൽ (c == ശൂന്യ) {
        എങ്കിൽ (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        തിരികെ NGX_ERROR;
    }

    എങ്കിൽ (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        പോകുക failed;
    }

    rev = c->പഠ;
    wev = c->ലിഖ;

    rev->log = &rec->log;
    wev->log = &rec->log;

    rec->udp = c;

    c->സംഖ്യ = ngx_atomic_fetch_add(ngx_connection_counter, 1);

    c->start_time = ngx_current_msec;

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->log, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->സംഖ്യ);

    rc = connect(s, rec->sockaddr, rec->socklen);

    എങ്കിൽ (rc == -1) {
        ngx_log_error(NGX_LOG_CRIT, &rec->log, ngx_socket_errno,
                      "connect() failed");

        പോകുക failed;
    }

    /* UDP sockets are always ready to write */
    wev->ready = 1;

    event = (ngx_event_flags & NGX_USE_CLEAR_EVENT) ?
                /* kqueue, epoll */                 NGX_CLEAR_EVENT:
                /* select, poll, /dev/poll */       NGX_LEVEL_EVENT;
                /* eventport event type has no meaning: oneshot only */

    എങ്കിൽ (ngx_add_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        പോകുക failed;
    }

    തിരികെ NGX_OK;

failed:

    ngx_close_connection(c);
    rec->udp = ശൂന്യ;

    തിരികെ NGX_ERROR;
}


സ്ഥിതം ngx_int_t
ngx_tcp_connect(ngx_resolver_connection_t *rec)
{
    പൂർണ്ണസംഖ്യ                rc;
    ngx_int_t          event;
    ngx_err_t          err;
    ngx_uint_t         level;
    ngx_socket_t       s;
    ngx_event_t       *rev, *wev;
    ngx_connection_t  *c;

    s = ngx_socket(rec->sockaddr->sa_family, SOCK_STREAM, 0);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "TCP socket %d", s);

    എങ്കിൽ (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_socket_n " failed");
        തിരികെ NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->log);

    എങ്കിൽ (c == ശൂന്യ) {
        എങ്കിൽ (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        തിരികെ NGX_ERROR;
    }

    എങ്കിൽ (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        പോകുക failed;
    }

    rev = c->പഠ;
    wev = c->ലിഖ;

    rev->log = &rec->log;
    wev->log = &rec->log;

    rec->tcp = c;

    c->സംഖ്യ = ngx_atomic_fetch_add(ngx_connection_counter, 1);

    c->start_time = ngx_current_msec;

    എങ്കിൽ (ngx_add_conn) {
        എങ്കിൽ (ngx_add_conn(c) == NGX_ERROR) {
            പോകുക failed;
        }
    }

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->log, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->സംഖ്യ);

    rc = connect(s, rec->sockaddr, rec->socklen);

    എങ്കിൽ (rc == -1) {
        err = ngx_socket_errno;


        എങ്കിൽ (err != NGX_EINPROGRESS
#എങ്കിൽ (NGX_WIN32)
            /* Winsock returns WSAEWOULDBLOCK (NGX_EAGAIN) */
            && err != NGX_EAGAIN
#endif
            )
        {
            എങ്കിൽ (err == NGX_ECONNREFUSED
#എങ്കിൽ (NGX_LINUX)
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

            } അല്ലെങ്കിൽ {
                level = NGX_LOG_CRIT;
            }

            ngx_log_error(level, &rec->log, err, "connect() to %V failed",
                          &rec->server);

            ngx_close_connection(c);
            rec->tcp = ശൂന്യ;

            തിരികെ NGX_ERROR;
        }
    }

    എങ്കിൽ (ngx_add_conn) {
        എങ്കിൽ (rc == -1) {

            /* NGX_EINPROGRESS */

            തിരികെ NGX_AGAIN;
        }

        ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "connected");

        wev->ready = 1;

        തിരികെ NGX_OK;
    }

    എങ്കിൽ (ngx_event_flags & NGX_USE_CLEAR_EVENT) {

        /* kqueue */

        event = NGX_CLEAR_EVENT;

    } അല്ലെങ്കിൽ {

        /* select, poll, /dev/poll */

        event = NGX_LEVEL_EVENT;
    }

    എങ്കിൽ (ngx_add_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        പോകുക failed;
    }

    എങ്കിൽ (rc == -1) {

        /* NGX_EINPROGRESS */

        എങ്കിൽ (ngx_add_event(wev, NGX_WRITE_EVENT, event) != NGX_OK) {
            പോകുക failed;
        }

        തിരികെ NGX_AGAIN;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "connected");

    wev->ready = 1;

    തിരികെ NGX_OK;

failed:

    ngx_close_connection(c);
    rec->tcp = ശൂന്യ;

    തിരികെ NGX_ERROR;
}


സ്ഥിതം ngx_int_t
ngx_resolver_cmp_srvs(സ്ഥിരം ശൂന്യം *one, സ്ഥിരം ശൂന്യം *two)
{
    ngx_int_t            p1, p2;
    ngx_resolver_srv_t  *first, *second;

    first = (ngx_resolver_srv_t *) one;
    second = (ngx_resolver_srv_t *) two;

    p1 = first->priority;
    p2 = second->priority;

    തിരികെ p1 - p2;
}
