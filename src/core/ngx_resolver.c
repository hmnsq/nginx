
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>
#અન્તર્ભાવ <ngx_event.h>


#define NGX_RESOLVER_UDP_SIZE   4096

#define NGX_RESOLVER_TCP_RSIZE  (2 + 65535)
#define NGX_RESOLVER_TCP_WSIZE  8192


પ્રકારવ્યાખ્યા સંરચના {
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


પ્રકારવ્યાખ્યા સંરચના {
    u_char  type_hi;
    u_char  type_lo;
    u_char  class_hi;
    u_char  class_lo;
} ngx_resolver_qs_t;


પ્રકારવ્યાખ્યા સંરચના {
    u_char  type_hi;
    u_char  type_lo;
    u_char  class_hi;
    u_char  class_lo;
    u_char  ttl[4];
    u_char  len_hi;
    u_char  len_lo;
} ngx_resolver_an_t;


#define ngx_resolver_ગાંઠ(n)  ngx_rbtree_data(n, ngx_resolver_ગાંઠ_t, ગાંઠ)


સ્થિર ngx_int_t ngx_udp_connect(ngx_resolver_connection_t *rec);
સ્થિર ngx_int_t ngx_tcp_connect(ngx_resolver_connection_t *rec);


સ્થિર શૂન્ય ngx_resolver_cleanup(શૂન્ય *data);
સ્થિર શૂન્ય ngx_resolver_cleanup_વૃક્ષ(ngx_resolver_t *r, ngx_rbtree_t *વૃક્ષ);
સ્થિર ngx_int_t ngx_resolve_name_locked(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx, ngx_str_t *name);
સ્થિર શૂન્ય ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *વૃક્ષ,
    ngx_queue_t *queue);
સ્થિર ngx_int_t ngx_resolver_send_query(ngx_resolver_t *r,
    ngx_resolver_ગાંઠ_t *rn);
સ્થિર ngx_int_t ngx_resolver_send_udp_query(ngx_resolver_t *r,
    ngx_resolver_connection_t *rec, u_char *query, u_short qlen);
સ્થિર ngx_int_t ngx_resolver_send_tcp_query(ngx_resolver_t *r,
    ngx_resolver_connection_t *rec, u_char *query, u_short qlen);
સ્થિર ngx_int_t ngx_resolver_create_name_query(ngx_resolver_t *r,
    ngx_resolver_ગાંઠ_t *rn, ngx_str_t *name);
સ્થિર ngx_int_t ngx_resolver_create_srv_query(ngx_resolver_t *r,
    ngx_resolver_ગાંઠ_t *rn, ngx_str_t *name);
સ્થિર ngx_int_t ngx_resolver_create_addr_query(ngx_resolver_t *r,
    ngx_resolver_ગાંઠ_t *rn, ngx_resolver_addr_t *addr);
સ્થિર શૂન્ય ngx_resolver_resend_handler(ngx_event_t *ev);
સ્થિર time_t ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *વૃક્ષ,
    ngx_queue_t *queue);
સ્થિર ngx_uint_t ngx_resolver_resend_empty(ngx_resolver_t *r);
સ્થિર શૂન્ય ngx_resolver_udp_read(ngx_event_t *rev);
સ્થિર શૂન્ય ngx_resolver_tcp_write(ngx_event_t *wev);
સ્થિર શૂન્ય ngx_resolver_tcp_read(ngx_event_t *rev);
સ્થિર શૂન્ય ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf,
    size_t n, ngx_uint_t tcp);
સ્થિર શૂન્ય ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans);
સ્થિર શૂન્ય ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans);
સ્થિર શૂન્ય ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan);
સ્થિર ngx_resolver_ગાંઠ_t *ngx_resolver_lookup_name(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
સ્થિર ngx_resolver_ગાંઠ_t *ngx_resolver_lookup_srv(ngx_resolver_t *r,
    ngx_str_t *name, uint32_t hash);
સ્થિર ngx_resolver_ગાંઠ_t *ngx_resolver_lookup_addr(ngx_resolver_t *r,
    in_addr_t addr);
સ્થિર શૂન્ય ngx_resolver_rbtree_ઉમેરો_કિંમત(ngx_rbtree_ગાંઠ_t *temp,
    ngx_rbtree_ગાંઠ_t *ગાંઠ, ngx_rbtree_ગાંઠ_t *sentinel);
સ્થિર ngx_int_t ngx_resolver_copy(ngx_resolver_t *r, ngx_str_t *name,
    u_char *buf, u_char *src, u_char *last);
સ્થિર ngx_int_t ngx_resolver_set_timeout(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ctx);
સ્થિર શૂન્ય ngx_resolver_timeout_handler(ngx_event_t *ev);
સ્થિર શૂન્ય ngx_resolver_free_ગાંઠ(ngx_resolver_t *r, ngx_resolver_ગાંઠ_t *rn);
સ્થિર શૂન્ય *ngx_resolver_alloc(ngx_resolver_t *r, size_t size);
સ્થિર શૂન્ય *ngx_resolver_calloc(ngx_resolver_t *r, size_t size);
સ્થિર શૂન્ય ngx_resolver_free(ngx_resolver_t *r, શૂન્ય *p);
સ્થિર શૂન્ય ngx_resolver_free_locked(ngx_resolver_t *r, શૂન્ય *p);
સ્થિર શૂન્ય *ngx_resolver_dup(ngx_resolver_t *r, શૂન્ય *src, size_t size);
સ્થિર ngx_resolver_addr_t *ngx_resolver_export(ngx_resolver_t *r,
    ngx_resolver_ગાંઠ_t *rn, ngx_uint_t rotate);
સ્થિર શૂન્ય ngx_resolver_report_srv(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx);
સ્થિર u_char *ngx_resolver_log_error(ngx_log_t *log, u_char *buf, size_t len);
સ્થિર શૂન્ય ngx_resolver_resolve_srv_names(ngx_resolver_ctx_t *ctx,
    ngx_resolver_ગાંઠ_t *rn);
સ્થિર શૂન્ય ngx_resolver_srv_names_handler(ngx_resolver_ctx_t *ctx);
સ્થિર ngx_int_t ngx_resolver_cmp_srvs(અચલ શૂન્ય *one, અચલ શૂન્ય *two);

#જો (NGX_HAVE_INET6)
સ્થિર શૂન્ય ngx_resolver_rbtree_ઉમેરો_addr6_કિંમત(ngx_rbtree_ગાંઠ_t *temp,
    ngx_rbtree_ગાંઠ_t *ગાંઠ, ngx_rbtree_ગાંઠ_t *sentinel);
સ્થિર ngx_resolver_ગાંઠ_t *ngx_resolver_lookup_addr6(ngx_resolver_t *r,
    સંરચના in6_addr *addr, uint32_t hash);
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

    r = ngx_pcalloc(cf->pool, કદ(ngx_resolver_t));
    જો (r == ખાલી) {
        પાછા ખાલી;
    }

    r->event = ngx_pcalloc(cf->pool, કદ(ngx_event_t));
    જો (r->event == ખાલી) {
        પાછા ખાલી;
    }

    cln = ngx_pool_cleanup_add(cf->pool, 0);
    જો (cln == ખાલી) {
        પાછા ખાલી;
    }

    cln->handler = ngx_resolver_cleanup;
    cln->data = r;

    r->ipv4 = 1;

    ngx_rbtree_init(&r->name_rbtree, &r->name_sentinel,
                    ngx_resolver_rbtree_ઉમેરો_કિંમત);

    ngx_rbtree_init(&r->srv_rbtree, &r->srv_sentinel,
                    ngx_resolver_rbtree_ઉમેરો_કિંમત);

    ngx_rbtree_init(&r->addr_rbtree, &r->addr_sentinel,
                    ngx_rbtree_ઉમેરો_કિંમત);

    ngx_queue_init(&r->name_resend_queue);
    ngx_queue_init(&r->srv_resend_queue);
    ngx_queue_init(&r->addr_resend_queue);

    ngx_queue_init(&r->name_expire_queue);
    ngx_queue_init(&r->srv_expire_queue);
    ngx_queue_init(&r->addr_expire_queue);

#જો (NGX_HAVE_INET6)
    r->ipv6 = 1;

    ngx_rbtree_init(&r->addr6_rbtree, &r->addr6_sentinel,
                    ngx_resolver_rbtree_ઉમેરો_addr6_કિંમત);

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

    જો (n) {
        જો (ngx_array_init(&r->connections, cf->pool, n,
                           કદ(ngx_resolver_connection_t))
            != NGX_OK)
        {
            પાછા ખાલી;
        }
    }

    માટે (i = 0; i < n; i++) {
        જો (ngx_strncmp(names[i].data, "valid=", 6) == 0) {
            s.len = names[i].len - 6;
            s.data = names[i].data + 6;

            r->valid = ngx_parse_time(&s, 1);

            જો (r->valid == (time_t) NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &names[i]);
                પાછા ખાલી;
            }

            ચાલુ;
        }

#જો (NGX_HAVE_INET6)
        જો (ngx_strncmp(names[i].data, "ipv4=", 5) == 0) {

            જો (ngx_strcmp(&names[i].data[5], "on") == 0) {
                r->ipv4 = 1;

            } નહીંતર જો (ngx_strcmp(&names[i].data[5], "off") == 0) {
                r->ipv4 = 0;

            } નહીંતર {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &names[i]);
                પાછા ખાલી;
            }

            ચાલુ;
        }

        જો (ngx_strncmp(names[i].data, "ipv6=", 5) == 0) {

            જો (ngx_strcmp(&names[i].data[5], "on") == 0) {
                r->ipv6 = 1;

            } નહીંતર જો (ngx_strcmp(&names[i].data[5], "off") == 0) {
                r->ipv6 = 0;

            } નહીંતર {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid parameter: %V", &names[i]);
                પાછા ખાલી;
            }

            ચાલુ;
        }
#endif

        ngx_memzero(&u, કદ(ngx_url_t));

        u.url = names[i];
        u.default_port = 53;

        જો (ngx_parse_url(cf->pool, &u) != NGX_OK) {
            જો (u.err) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "%s in resolver \"%V\"",
                                   u.err, &u.url);
            }

            પાછા ખાલી;
        }

        rec = ngx_array_ધકેલો_n(&r->connections, u.naddrs);
        જો (rec == ખાલી) {
            પાછા ખાલી;
        }

        ngx_memzero(rec, u.naddrs * કદ(ngx_resolver_connection_t));

        માટે (j = 0; j < u.naddrs; j++) {
            rec[j].sockaddr = u.addrs[j].sockaddr;
            rec[j].socklen = u.addrs[j].socklen;
            rec[j].server = u.addrs[j].name;
            rec[j].resolver = r;
        }
    }

#જો (NGX_HAVE_INET6)
    જો (r->ipv4 + r->ipv6 == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"ipv4\" and \"ipv6\" cannot both be \"off\"");
        પાછા ખાલી;
    }
#endif

    જો (n && r->connections.nelts == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "no name servers defined");
        પાછા ખાલી;
    }

    પાછા r;
}


સ્થિર શૂન્ય
ngx_resolver_cleanup(શૂન્ય *data)
{
    ngx_resolver_t  *r = data;

    ngx_uint_t                  i;
    ngx_resolver_connection_t  *rec;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0, "cleanup resolver");

    ngx_resolver_cleanup_વૃક્ષ(r, &r->name_rbtree);

    ngx_resolver_cleanup_વૃક્ષ(r, &r->srv_rbtree);

    ngx_resolver_cleanup_વૃક્ષ(r, &r->addr_rbtree);

#જો (NGX_HAVE_INET6)
    ngx_resolver_cleanup_વૃક્ષ(r, &r->addr6_rbtree);
#endif

    જો (r->event->timer_set) {
        ngx_del_timer(r->event);
    }

    rec = r->connections.elts;

    માટે (i = 0; i < r->connections.nelts; i++) {
        જો (rec[i].udp) {
            ngx_close_connection(rec[i].udp);
        }

        જો (rec[i].tcp) {
            ngx_close_connection(rec[i].tcp);
        }

        જો (rec[i].read_buf) {
            ngx_resolver_free(r, rec[i].read_buf->start);
            ngx_resolver_free(r, rec[i].read_buf);
        }

        જો (rec[i].write_buf) {
            ngx_resolver_free(r, rec[i].write_buf->start);
            ngx_resolver_free(r, rec[i].write_buf);
        }
    }
}


સ્થિર શૂન્ય
ngx_resolver_cleanup_વૃક્ષ(ngx_resolver_t *r, ngx_rbtree_t *વૃક્ષ)
{
    ngx_resolver_ctx_t   *ctx, *next;
    ngx_resolver_ગાંઠ_t  *rn;

    જ્યારે (વૃક્ષ->મૂળ != વૃક્ષ->sentinel) {

        rn = ngx_resolver_ગાંઠ(ngx_rbtree_min(વૃક્ષ->મૂળ, વૃક્ષ->sentinel));

        ngx_queue_remove(&rn->queue);

        માટે (ctx = rn->waiting; ctx; ctx = next) {
            next = ctx->next;

            જો (ctx->event) {
                જો (ctx->event->timer_set) {
                    ngx_del_timer(ctx->event);
                }

                ngx_resolver_free(r, ctx->event);
            }

            ngx_resolver_free(r, ctx);
        }

        ngx_rbtree_delete(વૃક્ષ, &rn->ગાંઠ);

        ngx_resolver_free_ગાંઠ(r, rn);
    }
}


ngx_resolver_ctx_t *
ngx_resolve_start(ngx_resolver_t *r, ngx_resolver_ctx_t *temp)
{
    in_addr_t            addr;
    ngx_resolver_ctx_t  *ctx;

    જો (temp) {
        addr = ngx_inet_addr(temp->name.data, temp->name.len);

        જો (addr != INADDR_NONE) {
            temp->resolver = r;
            temp->state = NGX_OK;
            temp->naddrs = 1;
            temp->addrs = &temp->addr;
            temp->addr.sockaddr = (સંરચના sockaddr *) &temp->sin;
            temp->addr.socklen = કદ(સંરચના sockaddr_in);
            ngx_memzero(&temp->sin, કદ(સંરચના sockaddr_in));
            temp->sin.sin_family = AF_INET;
            temp->sin.sin_addr.s_addr = addr;
            temp->quick = 1;

            પાછા temp;
        }
    }

    જો (r->connections.nelts == 0) {
        પાછા NGX_NO_RESOLVER;
    }

    ctx = ngx_resolver_calloc(r, કદ(ngx_resolver_ctx_t));

    જો (ctx) {
        ctx->resolver = r;
    }

    પાછા ctx;
}


ngx_int_t
ngx_resolve_name(ngx_resolver_ctx_t *ctx)
{
    size_t           slen;
    ngx_int_t        rc;
    ngx_str_t        name;
    ngx_resolver_t  *r;

    r = ctx->resolver;

    જો (ctx->name.len > 0 && ctx->name.data[ctx->name.len - 1] == '.') {
        ctx->name.len--;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve: \"%V\"", &ctx->name);

    જો (ctx->quick) {
        ctx->handler(ctx);
        પાછા NGX_OK;
    }

    જો (ctx->service.len) {
        slen = ctx->service.len;

        જો (ngx_strlchr(ctx->service.data,
                        ctx->service.data + ctx->service.len, '.')
            == ખાલી)
        {
            slen += કદ("_._tcp") - 1;
        }

        name.len = slen + 1 + ctx->name.len;

        name.data = ngx_resolver_alloc(r, name.len);
        જો (name.data == ખાલી) {
            જાઓ failed;
        }

        જો (slen == ctx->service.len) {
            ngx_sprintf(name.data, "%V.%V", &ctx->service, &ctx->name);

        } નહીંતર {
            ngx_sprintf(name.data, "_%V._tcp.%V", &ctx->service, &ctx->name);
        }

        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &name);

        ngx_resolver_free(r, name.data);

    } નહીંતર {
        /* lock name mutex */

        rc = ngx_resolve_name_locked(r, ctx, &ctx->name);
    }

    જો (rc == NGX_OK) {
        પાછા NGX_OK;
    }

    /* unlock name mutex */

    જો (rc == NGX_AGAIN) {
        પાછા NGX_OK;
    }

    /* NGX_ERROR */

    જો (ctx->event) {
        ngx_resolver_free(r, ctx->event);
    }

failed:

    ngx_resolver_free(r, ctx);

    પાછા NGX_ERROR;
}


શૂન્ય
ngx_resolve_name_done(ngx_resolver_ctx_t *ctx)
{
    ngx_uint_t            i;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_ગાંઠ_t  *rn;

    r = ctx->resolver;

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve name done: %i", ctx->state);

    જો (ctx->quick) {
        પાછા;
    }

    જો (ctx->event && ctx->event->timer_set) {
        ngx_del_timer(ctx->event);
    }

    /* lock name mutex */

    જો (ctx->nsrvs) {
        માટે (i = 0; i < ctx->nsrvs; i++) {
            જો (ctx->srvs[i].ctx) {
                ngx_resolve_name_done(ctx->srvs[i].ctx);
            }

            જો (ctx->srvs[i].addrs) {
                ngx_resolver_free(r, ctx->srvs[i].addrs->sockaddr);
                ngx_resolver_free(r, ctx->srvs[i].addrs);
            }

            ngx_resolver_free(r, ctx->srvs[i].name.data);
        }

        ngx_resolver_free(r, ctx->srvs);
    }

    જો (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->ગાંઠ;

        જો (rn) {
            p = &rn->waiting;
            w = rn->waiting;

            જ્યારે (w) {
                જો (w == ctx) {
                    *p = w->next;

                    જાઓ done;
                }

                p = &w->next;
                w = w->next;
            }

            ngx_log_error(NGX_LOG_ALERT, r->log, 0,
                          "could not cancel %V resolving", &ctx->name);
        }
    }

done:

    જો (ctx->service.len) {
        ngx_resolver_expire(r, &r->srv_rbtree, &r->srv_expire_queue);

    } નહીંતર {
        ngx_resolver_expire(r, &r->name_rbtree, &r->name_expire_queue);
    }

    /* unlock name mutex */

    /* lock alloc mutex */

    જો (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    જો (r->event->timer_set && ngx_resolver_resend_empty(r)) {
        ngx_del_timer(r->event);
    }
}


સ્થિર ngx_int_t
ngx_resolve_name_locked(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx,
    ngx_str_t *name)
{
    uint32_t              hash;
    ngx_int_t             rc;
    ngx_str_t             cname;
    ngx_uint_t            i, naddrs;
    ngx_queue_t          *resend_queue, *expire_queue;
    ngx_rbtree_t         *વૃક્ષ;
    ngx_resolver_ctx_t   *next, *last;
    ngx_resolver_addr_t  *addrs;
    ngx_resolver_ગાંઠ_t  *rn;

    ngx_strlow(name->data, name->data, name->len);

    hash = ngx_crc32_short(name->data, name->len);

    જો (ctx->service.len) {
        rn = ngx_resolver_lookup_srv(r, name, hash);

        વૃક્ષ = &r->srv_rbtree;
        resend_queue = &r->srv_resend_queue;
        expire_queue = &r->srv_expire_queue;

    } નહીંતર {
        rn = ngx_resolver_lookup_name(r, name, hash);

        વૃક્ષ = &r->name_rbtree;
        resend_queue = &r->name_resend_queue;
        expire_queue = &r->name_expire_queue;
    }

    જો (rn) {

        /* ctx can be a list after NGX_RESOLVE_CNAME */
        માટે (last = ctx; last->next; last = last->next);

        જો (rn->valid >= ngx_time()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0, "resolve cached");

            ngx_queue_remove(&rn->queue);

            rn->expire = ngx_time() + r->expire;

            ngx_queue_ઉમેરો_head(expire_queue, &rn->queue);

            naddrs = (rn->naddrs == (u_short) -1) ? 0 : rn->naddrs;
#જો (NGX_HAVE_INET6)
            naddrs += (rn->naddrs6 == (u_short) -1) ? 0 : rn->naddrs6;
#endif

            જો (naddrs) {

                જો (naddrs == 1 && rn->naddrs == 1) {
                    addrs = ખાલી;

                } નહીંતર {
                    addrs = ngx_resolver_export(r, rn, 1);
                    જો (addrs == ખાલી) {
                        પાછા NGX_ERROR;
                    }
                }

                last->next = rn->waiting;
                rn->waiting = ખાલી;

                /* unlock name mutex */

                કરો {
                    ctx->state = NGX_OK;
                    ctx->valid = rn->valid;
                    ctx->naddrs = naddrs;

                    જો (addrs == ખાલી) {
                        ctx->addrs = &ctx->addr;
                        ctx->addr.sockaddr = (સંરચના sockaddr *) &ctx->sin;
                        ctx->addr.socklen = કદ(સંરચના sockaddr_in);
                        ngx_memzero(&ctx->sin, કદ(સંરચના sockaddr_in));
                        ctx->sin.sin_family = AF_INET;
                        ctx->sin.sin_addr.s_addr = rn->u.addr;

                    } નહીંતર {
                        ctx->addrs = addrs;
                    }

                    next = ctx->next;

                    ctx->handler(ctx);

                    ctx = next;
                } જ્યારે (ctx);

                જો (addrs != ખાલી) {
                    ngx_resolver_free(r, addrs->sockaddr);
                    ngx_resolver_free(r, addrs);
                }

                પાછા NGX_OK;
            }

            જો (rn->nsrvs) {
                last->next = rn->waiting;
                rn->waiting = ખાલી;

                /* unlock name mutex */

                કરો {
                    next = ctx->next;

                    ngx_resolver_resolve_srv_names(ctx, rn);

                    ctx = next;
                } જ્યારે (ctx);

                પાછા NGX_OK;
            }

            /* NGX_RESOLVE_CNAME */

            જો (ctx->recursion++ < NGX_RESOLVER_MAX_RECURSION) {

                cname.len = rn->cnlen;
                cname.data = rn->u.cname;

                પાછા ngx_resolve_name_locked(r, ctx, &cname);
            }

            last->next = rn->waiting;
            rn->waiting = ખાલી;

            /* unlock name mutex */

            કરો {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
                next = ctx->next;

                ctx->handler(ctx);

                ctx = next;
            } જ્યારે (ctx);

            પાછા NGX_OK;
        }

        જો (rn->waiting) {
            જો (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
                પાછા NGX_ERROR;
            }

            last->next = rn->waiting;
            rn->waiting = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;

            કરો {
                ctx->ગાંઠ = rn;
                ctx = ctx->next;
            } જ્યારે (ctx);

            પાછા NGX_AGAIN;
        }

        ngx_queue_remove(&rn->queue);

        /* lock alloc mutex */

        જો (rn->query) {
            ngx_resolver_free_locked(r, rn->query);
            rn->query = ખાલી;
#જો (NGX_HAVE_INET6)
            rn->query6 = ખાલી;
#endif
        }

        જો (rn->cnlen) {
            ngx_resolver_free_locked(r, rn->u.cname);
        }

        જો (rn->naddrs > 1 && rn->naddrs != (u_short) -1) {
            ngx_resolver_free_locked(r, rn->u.addrs);
        }

#જો (NGX_HAVE_INET6)
        જો (rn->naddrs6 > 1 && rn->naddrs6 != (u_short) -1) {
            ngx_resolver_free_locked(r, rn->u6.addrs6);
        }
#endif

        જો (rn->nsrvs) {
            માટે (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
                જો (rn->u.srvs[i].name.data) {
                    ngx_resolver_free_locked(r, rn->u.srvs[i].name.data);
                }
            }

            ngx_resolver_free_locked(r, rn->u.srvs);
        }

        /* unlock alloc mutex */

    } નહીંતર {

        rn = ngx_resolver_alloc(r, કદ(ngx_resolver_ગાંઠ_t));
        જો (rn == ખાલી) {
            પાછા NGX_ERROR;
        }

        rn->name = ngx_resolver_dup(r, name->data, name->len);
        જો (rn->name == ખાલી) {
            ngx_resolver_free(r, rn);
            પાછા NGX_ERROR;
        }

        rn->ગાંઠ.ચાવી = hash;
        rn->nlen = (u_short) name->len;
        rn->query = ખાલી;
#જો (NGX_HAVE_INET6)
        rn->query6 = ખાલી;
#endif

        ngx_rbtree_ઉમેરો(વૃક્ષ, &rn->ગાંઠ);
    }

    જો (ctx->service.len) {
        rc = ngx_resolver_create_srv_query(r, rn, name);

    } નહીંતર {
        rc = ngx_resolver_create_name_query(r, rn, name);
    }

    જો (rc == NGX_ERROR) {
        જાઓ failed;
    }

    જો (rc == NGX_DECLINED) {
        ngx_rbtree_delete(વૃક્ષ, &rn->ગાંઠ);

        ngx_resolver_free(r, rn->query);
        ngx_resolver_free(r, rn->name);
        ngx_resolver_free(r, rn);

        કરો {
            ctx->state = NGX_RESOLVE_NXDOMAIN;
            next = ctx->next;

            ctx->handler(ctx);

            ctx = next;
        } જ્યારે (ctx);

        પાછા NGX_OK;
    }

    rn->last_connection = r->last_connection++;
    જો (r->last_connection == r->connections.nelts) {
        r->last_connection = 0;
    }

    rn->naddrs = r->ipv4 ? (u_short) -1 : 0;
    rn->tcp = 0;
#જો (NGX_HAVE_INET6)
    rn->naddrs6 = r->ipv6 ? (u_short) -1 : 0;
    rn->tcp6 = 0;
#endif
    rn->nsrvs = 0;

    જો (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->last_connection++;
        જો (rn->last_connection == r->connections.nelts) {
            rn->last_connection = 0;
        }

        (શૂન્ય) ngx_resolver_send_query(r, rn);
    }

    જો (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
        જાઓ failed;
    }

    જો (ngx_resolver_resend_empty(r)) {
        ngx_add_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_time() + r->resend_timeout;

    ngx_queue_ઉમેરો_head(resend_queue, &rn->queue);

    rn->code = 0;
    rn->cnlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_કિંમત;
    rn->waiting = ctx;

    ctx->state = NGX_AGAIN;
    ctx->async = 1;

    કરો {
        ctx->ગાંઠ = rn;
        ctx = ctx->next;
    } જ્યારે (ctx);

    પાછા NGX_AGAIN;

failed:

    ngx_rbtree_delete(વૃક્ષ, &rn->ગાંઠ);

    જો (rn->query) {
        ngx_resolver_free(r, rn->query);
    }

    ngx_resolver_free(r, rn->name);

    ngx_resolver_free(r, rn);

    પાછા NGX_ERROR;
}


ngx_int_t
ngx_resolve_addr(ngx_resolver_ctx_t *ctx)
{
    u_char               *name;
    in_addr_t             addr;
    ngx_queue_t          *resend_queue, *expire_queue;
    ngx_rbtree_t         *વૃક્ષ;
    ngx_resolver_t       *r;
    સંરચના sockaddr_in   *sin;
    ngx_resolver_ગાંઠ_t  *rn;
#જો (NGX_HAVE_INET6)
    uint32_t              hash;
    સંરચના sockaddr_in6  *sin6;
#endif

#જો (NGX_SUPPRESS_WARN)
    addr = 0;
#જો (NGX_HAVE_INET6)
    hash = 0;
    sin6 = ખાલી;
#endif
#endif

    r = ctx->resolver;

    પસંદગી (ctx->addr.sockaddr->sa_family) {

#જો (NGX_HAVE_INET6)
    સ્થિતિ AF_INET6:
        sin6 = (સંરચના sockaddr_in6 *) ctx->addr.sockaddr;
        hash = ngx_crc32_short(sin6->sin6_addr.s6_addr, 16);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr6(r, &sin6->sin6_addr, hash);

        વૃક્ષ = &r->addr6_rbtree;
        resend_queue = &r->addr6_resend_queue;
        expire_queue = &r->addr6_expire_queue;

        તોડો;
#endif

    મૂળભૂત: /* AF_INET */
        sin = (સંરચના sockaddr_in *) ctx->addr.sockaddr;
        addr = ntohl(sin->sin_addr.s_addr);

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        વૃક્ષ = &r->addr_rbtree;
        resend_queue = &r->addr_resend_queue;
        expire_queue = &r->addr_expire_queue;
    }

    જો (rn) {

        જો (rn->valid >= ngx_time()) {

            ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0, "resolve cached");

            ngx_queue_remove(&rn->queue);

            rn->expire = ngx_time() + r->expire;

            ngx_queue_ઉમેરો_head(expire_queue, &rn->queue);

            name = ngx_resolver_dup(r, rn->name, rn->nlen);
            જો (name == ખાલી) {
                ngx_resolver_free(r, ctx);
                પાછા NGX_ERROR;
            }

            ctx->name.len = rn->nlen;
            ctx->name.data = name;

            /* unlock addr mutex */

            ctx->state = NGX_OK;
            ctx->valid = rn->valid;

            ctx->handler(ctx);

            ngx_resolver_free(r, name);

            પાછા NGX_OK;
        }

        જો (rn->waiting) {
            જો (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
                પાછા NGX_ERROR;
            }

            ctx->next = rn->waiting;
            rn->waiting = ctx;
            ctx->state = NGX_AGAIN;
            ctx->async = 1;
            ctx->ગાંઠ = rn;

            /* unlock addr mutex */

            પાછા NGX_OK;
        }

        ngx_queue_remove(&rn->queue);

        ngx_resolver_free(r, rn->query);
        rn->query = ખાલી;
#જો (NGX_HAVE_INET6)
        rn->query6 = ખાલી;
#endif

    } નહીંતર {
        rn = ngx_resolver_alloc(r, કદ(ngx_resolver_ગાંઠ_t));
        જો (rn == ખાલી) {
            જાઓ failed;
        }

        પસંદગી (ctx->addr.sockaddr->sa_family) {

#જો (NGX_HAVE_INET6)
        સ્થિતિ AF_INET6:
            rn->addr6 = sin6->sin6_addr;
            rn->ગાંઠ.ચાવી = hash;
            તોડો;
#endif

        મૂળભૂત: /* AF_INET */
            rn->ગાંઠ.ચાવી = addr;
        }

        rn->query = ખાલી;
#જો (NGX_HAVE_INET6)
        rn->query6 = ખાલી;
#endif

        ngx_rbtree_ઉમેરો(વૃક્ષ, &rn->ગાંઠ);
    }

    જો (ngx_resolver_create_addr_query(r, rn, &ctx->addr) != NGX_OK) {
        જાઓ failed;
    }

    rn->last_connection = r->last_connection++;
    જો (r->last_connection == r->connections.nelts) {
        r->last_connection = 0;
    }

    rn->naddrs = (u_short) -1;
    rn->tcp = 0;
#જો (NGX_HAVE_INET6)
    rn->naddrs6 = (u_short) -1;
    rn->tcp6 = 0;
#endif
    rn->nsrvs = 0;

    જો (ngx_resolver_send_query(r, rn) != NGX_OK) {

        /* immediately retry once on failure */

        rn->last_connection++;
        જો (rn->last_connection == r->connections.nelts) {
            rn->last_connection = 0;
        }

        (શૂન્ય) ngx_resolver_send_query(r, rn);
    }

    જો (ngx_resolver_set_timeout(r, ctx) != NGX_OK) {
        જાઓ failed;
    }

    જો (ngx_resolver_resend_empty(r)) {
        ngx_add_timer(r->event, (ngx_msec_t) (r->resend_timeout * 1000));
    }

    rn->expire = ngx_time() + r->resend_timeout;

    ngx_queue_ઉમેરો_head(resend_queue, &rn->queue);

    rn->code = 0;
    rn->cnlen = 0;
    rn->name = ખાલી;
    rn->nlen = 0;
    rn->valid = 0;
    rn->ttl = NGX_MAX_UINT32_કિંમત;
    rn->waiting = ctx;

    /* unlock addr mutex */

    ctx->state = NGX_AGAIN;
    ctx->async = 1;
    ctx->ગાંઠ = rn;

    પાછા NGX_OK;

failed:

    જો (rn) {
        ngx_rbtree_delete(વૃક્ષ, &rn->ગાંઠ);

        જો (rn->query) {
            ngx_resolver_free(r, rn->query);
        }

        ngx_resolver_free(r, rn);
    }

    /* unlock addr mutex */

    જો (ctx->event) {
        ngx_resolver_free(r, ctx->event);
    }

    ngx_resolver_free(r, ctx);

    પાછા NGX_ERROR;
}


શૂન્ય
ngx_resolve_addr_done(ngx_resolver_ctx_t *ctx)
{
    ngx_queue_t          *expire_queue;
    ngx_rbtree_t         *વૃક્ષ;
    ngx_resolver_t       *r;
    ngx_resolver_ctx_t   *w, **p;
    ngx_resolver_ગાંઠ_t  *rn;

    r = ctx->resolver;

    પસંદગી (ctx->addr.sockaddr->sa_family) {

#જો (NGX_HAVE_INET6)
    સ્થિતિ AF_INET6:
        વૃક્ષ = &r->addr6_rbtree;
        expire_queue = &r->addr6_expire_queue;
        તોડો;
#endif

    મૂળભૂત: /* AF_INET */
        વૃક્ષ = &r->addr_rbtree;
        expire_queue = &r->addr_expire_queue;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve addr done: %i", ctx->state);

    જો (ctx->event && ctx->event->timer_set) {
        ngx_del_timer(ctx->event);
    }

    /* lock addr mutex */

    જો (ctx->state == NGX_AGAIN || ctx->state == NGX_RESOLVE_TIMEDOUT) {

        rn = ctx->ગાંઠ;

        જો (rn) {
            p = &rn->waiting;
            w = rn->waiting;

            જ્યારે (w) {
                જો (w == ctx) {
                    *p = w->next;

                    જાઓ done;
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

    ngx_resolver_expire(r, વૃક્ષ, expire_queue);

    /* unlock addr mutex */

    /* lock alloc mutex */

    જો (ctx->event) {
        ngx_resolver_free_locked(r, ctx->event);
    }

    ngx_resolver_free_locked(r, ctx);

    /* unlock alloc mutex */

    જો (r->event->timer_set && ngx_resolver_resend_empty(r)) {
        ngx_del_timer(r->event);
    }
}


સ્થિર શૂન્ય
ngx_resolver_expire(ngx_resolver_t *r, ngx_rbtree_t *વૃક્ષ, ngx_queue_t *queue)
{
    time_t                now;
    ngx_uint_t            i;
    ngx_queue_t          *q;
    ngx_resolver_ગાંઠ_t  *rn;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver expire");

    now = ngx_time();

    માટે (i = 0; i < 2; i++) {
        જો (ngx_queue_empty(queue)) {
            પાછા;
        }

        q = ngx_queue_last(queue);

        rn = ngx_queue_data(q, ngx_resolver_ગાંઠ_t, queue);

        જો (now <= rn->expire) {
            પાછા;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver expire \"%*s\"", (size_t) rn->nlen, rn->name);

        ngx_queue_remove(q);

        ngx_rbtree_delete(વૃક્ષ, &rn->ગાંઠ);

        ngx_resolver_free_ગાંઠ(r, rn);
    }
}


સ્થિર ngx_int_t
ngx_resolver_send_query(ngx_resolver_t *r, ngx_resolver_ગાંઠ_t *rn)
{
    ngx_int_t                   rc;
    ngx_resolver_connection_t  *rec;

    rec = r->connections.elts;
    rec = &rec[rn->last_connection];

    જો (rec->log.handler == ખાલી) {
        rec->log = *r->log;
        rec->log.handler = ngx_resolver_log_error;
        rec->log.data = rec;
        rec->log.action = "resolving";
    }

    જો (rn->query && rn->naddrs == (u_short) -1) {
        rc = rn->tcp ? ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen)
                     : ngx_resolver_send_udp_query(r, rec, rn->query, rn->qlen);

        જો (rc != NGX_OK) {
            પાછા rc;
        }
    }

#જો (NGX_HAVE_INET6)

    જો (rn->query6 && rn->naddrs6 == (u_short) -1) {
        rc = rn->tcp6
                    ? ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen)
                    : ngx_resolver_send_udp_query(r, rec, rn->query6, rn->qlen);

        જો (rc != NGX_OK) {
            પાછા rc;
        }
    }

#endif

    પાછા NGX_OK;
}


સ્થિર ngx_int_t
ngx_resolver_send_udp_query(ngx_resolver_t *r, ngx_resolver_connection_t  *rec,
    u_char *query, u_short qlen)
{
    ssize_t  n;

    જો (rec->udp == ખાલી) {
        જો (ngx_udp_connect(rec) != NGX_OK) {
            પાછા NGX_ERROR;
        }

        rec->udp->data = rec;
        rec->udp->પઠ->handler = ngx_resolver_udp_read;
        rec->udp->પઠ->resolver = 1;
    }

    n = ngx_send(rec->udp, query, qlen);

    જો (n == NGX_ERROR) {
        જાઓ failed;
    }

    જો ((size_t) n != (size_t) qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->log, 0, "send() incomplete");
        જાઓ failed;
    }

    પાછા NGX_OK;

failed:

    ngx_close_connection(rec->udp);
    rec->udp = ખાલી;

    પાછા NGX_ERROR;
}


સ્થિર ngx_int_t
ngx_resolver_send_tcp_query(ngx_resolver_t *r, ngx_resolver_connection_t *rec,
    u_char *query, u_short qlen)
{
    ngx_buf_t  *b;
    ngx_int_t   rc;

    rc = NGX_OK;

    જો (rec->tcp == ખાલી) {
        b = rec->read_buf;

        જો (b == ખાલી) {
            b = ngx_resolver_calloc(r, કદ(ngx_buf_t));
            જો (b == ખાલી) {
                પાછા NGX_ERROR;
            }

            b->start = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_RSIZE);
            જો (b->start == ખાલી) {
                ngx_resolver_free(r, b);
                પાછા NGX_ERROR;
            }

            b->end = b->start + NGX_RESOLVER_TCP_RSIZE;

            rec->read_buf = b;
        }

        b->pos = b->start;
        b->last = b->start;

        b = rec->write_buf;

        જો (b == ખાલી) {
            b = ngx_resolver_calloc(r, કદ(ngx_buf_t));
            જો (b == ખાલી) {
                પાછા NGX_ERROR;
            }

            b->start = ngx_resolver_alloc(r, NGX_RESOLVER_TCP_WSIZE);
            જો (b->start == ખાલી) {
                ngx_resolver_free(r, b);
                પાછા NGX_ERROR;
            }

            b->end = b->start + NGX_RESOLVER_TCP_WSIZE;

            rec->write_buf = b;
        }

        b->pos = b->start;
        b->last = b->start;

        rc = ngx_tcp_connect(rec);
        જો (rc == NGX_ERROR) {
            પાછા NGX_ERROR;
        }

        rec->tcp->data = rec;
        rec->tcp->લિખ->handler = ngx_resolver_tcp_write;
        rec->tcp->લિખ->cancelable = 1;
        rec->tcp->પઠ->handler = ngx_resolver_tcp_read;
        rec->tcp->પઠ->resolver = 1;

        ngx_add_timer(rec->tcp->લિખ, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    b = rec->write_buf;

    જો (b->end - b->last <  2 + qlen) {
        ngx_log_error(NGX_LOG_CRIT, &rec->log, 0, "buffer overflow");
        પાછા NGX_ERROR;
    }

    *b->last++ = (u_char) (qlen >> 8);
    *b->last++ = (u_char) qlen;
    b->last = ngx_cpymem(b->last, query, qlen);

    જો (rc == NGX_OK) {
        ngx_resolver_tcp_write(rec->tcp->લિખ);
    }

    પાછા NGX_OK;
}


સ્થિર શૂન્ય
ngx_resolver_resend_handler(ngx_event_t *ev)
{
    time_t           timer, atimer, stimer, ntimer;
#જો (NGX_HAVE_INET6)
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

#જો (NGX_HAVE_INET6)

    /* lock addr6 mutex */

    a6timer = ngx_resolver_resend(r, &r->addr6_rbtree, &r->addr6_resend_queue);

    /* unlock addr6 mutex */

#endif

    timer = ntimer;

    જો (timer == 0) {
        timer = atimer;

    } નહીંતર જો (atimer) {
        timer = ngx_min(timer, atimer);
    }

    જો (timer == 0) {
        timer = stimer;

    } નહીંતર જો (stimer) {
        timer = ngx_min(timer, stimer);
    }

#જો (NGX_HAVE_INET6)

    જો (timer == 0) {
        timer = a6timer;

    } નહીંતર જો (a6timer) {
        timer = ngx_min(timer, a6timer);
    }

#endif

    જો (timer) {
        ngx_add_timer(r->event, (ngx_msec_t) (timer * 1000));
    }
}


સ્થિર time_t
ngx_resolver_resend(ngx_resolver_t *r, ngx_rbtree_t *વૃક્ષ, ngx_queue_t *queue)
{
    time_t                now;
    ngx_queue_t          *q;
    ngx_resolver_ગાંઠ_t  *rn;

    now = ngx_time();

    માટે ( ;; ) {
        જો (ngx_queue_empty(queue)) {
            પાછા 0;
        }

        q = ngx_queue_last(queue);

        rn = ngx_queue_data(q, ngx_resolver_ગાંઠ_t, queue);

        જો (now < rn->expire) {
            પાછા rn->expire - now;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver resend \"%*s\" %p",
                       (size_t) rn->nlen, rn->name, rn->waiting);

        ngx_queue_remove(q);

        જો (rn->waiting) {

            જો (++rn->last_connection == r->connections.nelts) {
                rn->last_connection = 0;
            }

            (શૂન્ય) ngx_resolver_send_query(r, rn);

            rn->expire = now + r->resend_timeout;

            ngx_queue_ઉમેરો_head(queue, q);

            ચાલુ;
        }

        ngx_rbtree_delete(વૃક્ષ, &rn->ગાંઠ);

        ngx_resolver_free_ગાંઠ(r, rn);
    }
}


સ્થિર ngx_uint_t
ngx_resolver_resend_empty(ngx_resolver_t *r)
{
    પાછા ngx_queue_empty(&r->name_resend_queue)
           && ngx_queue_empty(&r->srv_resend_queue)
#જો (NGX_HAVE_INET6)
           && ngx_queue_empty(&r->addr6_resend_queue)
#endif
           && ngx_queue_empty(&r->addr_resend_queue);
}


સ્થિર શૂન્ય
ngx_resolver_udp_read(ngx_event_t *rev)
{
    ssize_t                     n;
    ngx_connection_t           *c;
    ngx_resolver_connection_t  *rec;
    u_char                      buf[NGX_RESOLVER_UDP_SIZE];

    c = rev->data;
    rec = c->data;

    કરો {
        n = ngx_udp_recv(c, buf, NGX_RESOLVER_UDP_SIZE);

        જો (n == NGX_AGAIN) {
            તોડો;
        }

        જો (n == NGX_ERROR) {
            જાઓ failed;
        }

        ngx_resolver_process_response(rec->resolver, buf, n, 0);

    } જ્યારે (rev->ready);

    જો (ngx_handle_read_event(rev, 0) != NGX_OK) {
        જાઓ failed;
    }

    પાછા;

failed:

    ngx_close_connection(rec->udp);
    rec->udp = ખાલી;
}


સ્થિર શૂન્ય
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

    જો (wev->timedout) {
        જાઓ failed;
    }

    sent = c->sent;

    જ્યારે (wev->ready && b->pos < b->last) {
        n = ngx_send(c, b->pos, b->last - b->pos);

        જો (n == NGX_AGAIN) {
            તોડો;
        }

        જો (n == NGX_ERROR) {
            જાઓ failed;
        }

        b->pos += n;
    }

    જો (b->pos != b->start) {
        b->last = ngx_movemem(b->start, b->pos, b->last - b->pos);
        b->pos = b->start;
    }

    જો (c->sent != sent) {
        ngx_add_timer(wev, (ngx_msec_t) (r->tcp_timeout * 1000));
    }

    જો (ngx_handle_write_event(wev, 0) != NGX_OK) {
        જાઓ failed;
    }

    પાછા;

failed:

    ngx_close_connection(c);
    rec->tcp = ખાલી;
}


સ્થિર શૂન્ય
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

    જ્યારે (rev->ready) {
        n = ngx_recv(c, b->last, b->end - b->last);

        જો (n == NGX_AGAIN) {
            તોડો;
        }

        જો (n == NGX_ERROR || n == 0) {
            જાઓ failed;
        }

        b->last += n;

        માટે ( ;; ) {
            p = b->pos;
            size = b->last - p;

            જો (size < 2) {
                તોડો;
            }

            qlen = (u_short) *p++ << 8;
            qlen += *p++;

            જો (size < (size_t) (2 + qlen)) {
                તોડો;
            }

            ngx_resolver_process_response(r, p, qlen, 1);

            b->pos += 2 + qlen;
        }

        જો (b->pos != b->start) {
            b->last = ngx_movemem(b->start, b->pos, b->last - b->pos);
            b->pos = b->start;
        }
    }

    જો (ngx_handle_read_event(rev, 0) != NGX_OK) {
        જાઓ failed;
    }

    પાછા;

failed:

    ngx_close_connection(c);
    rec->tcp = ખાલી;
}


સ્થિર શૂન્ય
ngx_resolver_process_response(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t tcp)
{
    અક્ષર                 *err;
    ngx_uint_t            i, times, ident, qident, flags, code, nqs, nan, trunc,
                          qtype, qclass;
#જો (NGX_HAVE_INET6)
    ngx_uint_t            qident6;
#endif
    ngx_queue_t          *q;
    ngx_resolver_qs_t    *qs;
    ngx_resolver_hdr_t   *response;
    ngx_resolver_ગાંઠ_t  *rn;

    જો (n < કદ(ngx_resolver_hdr_t)) {
        જાઓ short_response;
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
    જો ((flags & 0xf870) != 0x8000 || (trunc && tcp)) {
        ngx_log_error(r->log_level, r->log, 0,
                      "invalid %s DNS response %ui fl:%04Xi",
                      tcp ? "TCP" : "UDP", ident, flags);
        પાછા;
    }

    code = flags & 0xf;

    જો (code == NGX_RESOLVE_FORMERR) {

        times = 0;

        માટે (q = ngx_queue_head(&r->name_resend_queue);
             q != ngx_queue_sentinel(&r->name_resend_queue) && times++ < 100;
             q = ngx_queue_next(q))
        {
            rn = ngx_queue_data(q, ngx_resolver_ગાંઠ_t, queue);

            જો (rn->query) {
                qident = (rn->query[0] << 8) + rn->query[1];

                જો (qident == ident) {
                    જાઓ dns_error_name;
                }
            }

#જો (NGX_HAVE_INET6)
            જો (rn->query6) {
                qident6 = (rn->query6[0] << 8) + rn->query6[1];

                જો (qident6 == ident) {
                    જાઓ dns_error_name;
                }
            }
#endif
        }

        જાઓ dns_error;
    }

    જો (code > NGX_RESOLVE_REFUSED) {
        જાઓ dns_error;
    }

    જો (nqs != 1) {
        err = "invalid number of questions in DNS response";
        જાઓ done;
    }

    i = કદ(ngx_resolver_hdr_t);

    જ્યારે (i < (ngx_uint_t) n) {

        જો (buf[i] & 0xc0) {
            err = "unexpected compression pointer in DNS response";
            જાઓ done;
        }

        જો (buf[i] == '\0') {
            જાઓ found;
        }

        i += 1 + buf[i];
    }

    જાઓ short_response;

found:

    જો (i++ == કદ(ngx_resolver_hdr_t)) {
        err = "zero-length domain name in DNS response";
        જાઓ done;
    }

    જો (i + કદ(ngx_resolver_qs_t) + nan * (2 + કદ(ngx_resolver_an_t))
        > (ngx_uint_t) n)
    {
        જાઓ short_response;
    }

    qs = (ngx_resolver_qs_t *) &buf[i];

    qtype = (qs->type_hi << 8) + qs->type_lo;
    qclass = (qs->class_hi << 8) + qs->class_lo;

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver DNS response qt:%ui cl:%ui", qtype, qclass);

    જો (qclass != 1) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unknown query class %ui in DNS response", qclass);
        પાછા;
    }

    પસંદગી (qtype) {

    સ્થિતિ NGX_RESOLVE_A:
#જો (NGX_HAVE_INET6)
    સ્થિતિ NGX_RESOLVE_AAAA:
#endif

        ngx_resolver_process_a(r, buf, n, ident, code, qtype, nan, trunc,
                               i + કદ(ngx_resolver_qs_t));

        તોડો;

    સ્થિતિ NGX_RESOLVE_SRV:

        ngx_resolver_process_srv(r, buf, n, ident, code, nan, trunc,
                                 i + કદ(ngx_resolver_qs_t));

        તોડો;

    સ્થિતિ NGX_RESOLVE_PTR:

        ngx_resolver_process_ptr(r, buf, n, ident, code, nan);

        તોડો;

    મૂળભૂત:
        ngx_log_error(r->log_level, r->log, 0,
                      "unknown query type %ui in DNS response", qtype);
        પાછા;
    }

    પાછા;

short_response:

    err = "short DNS response";

done:

    ngx_log_error(r->log_level, r->log, 0, err);

    પાછા;

dns_error_name:

    ngx_log_error(r->log_level, r->log, 0,
                  "DNS error (%ui: %s), query id:%ui, name:\"%*s\"",
                  code, ngx_resolver_strerror(code), ident,
                  (size_t) rn->nlen, rn->name);
    પાછા;

dns_error:

    ngx_log_error(r->log_level, r->log, 0,
                  "DNS error (%ui: %s), query id:%ui",
                  code, ngx_resolver_strerror(code), ident);
    પાછા;
}


સ્થિર શૂન્ય
ngx_resolver_process_a(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t qtype,
    ngx_uint_t nan, ngx_uint_t trunc, ngx_uint_t ans)
{
    અક્ષર                       *err;
    u_char                     *cname;
    size_t                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    in_addr_t                  *addr;
    ngx_str_t                   name;
    ngx_uint_t                  type, class, qident, naddrs, a, i, j, start;
#જો (NGX_HAVE_INET6)
    સંરચના in6_addr            *addr6;
#endif
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *next;
    ngx_resolver_ગાંઠ_t        *rn;
    ngx_resolver_addr_t        *addrs;
    ngx_resolver_connection_t  *rec;

    જો (ngx_resolver_copy(r, &name, buf,
                          buf + કદ(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        પાછા;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver qs:%V", &name);

    hash = ngx_crc32_short(name.data, name.len);

    /* lock name mutex */

    rn = ngx_resolver_lookup_name(r, &name, hash);

    જો (rn == ખાલી) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.data);
        જાઓ failed;
    }

    પસંદગી (qtype) {

#જો (NGX_HAVE_INET6)
    સ્થિતિ NGX_RESOLVE_AAAA:

        જો (rn->query6 == ખાલી || rn->naddrs6 != (u_short) -1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.data);
            જાઓ failed;
        }

        જો (trunc && rn->tcp6) {
            ngx_resolver_free(r, name.data);
            જાઓ failed;
        }

        qident = (rn->query6[0] << 8) + rn->query6[1];

        તોડો;
#endif

    મૂળભૂત: /* NGX_RESOLVE_A */

        જો (rn->query == ખાલી || rn->naddrs != (u_short) -1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected DNS response for %V", &name);
            ngx_resolver_free(r, name.data);
            જાઓ failed;
        }

        જો (trunc && rn->tcp) {
            ngx_resolver_free(r, name.data);
            જાઓ failed;
        }

        qident = (rn->query[0] << 8) + rn->query[1];
    }

    જો (ident != qident) {
        ngx_log_error(r->log_level, r->log, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.data);
        જાઓ failed;
    }

    ngx_resolver_free(r, name.data);

    જો (trunc) {

        ngx_queue_remove(&rn->queue);

        જો (rn->waiting == ખાલી) {
            ngx_rbtree_delete(&r->name_rbtree, &rn->ગાંઠ);
            ngx_resolver_free_ગાંઠ(r, rn);
            જાઓ next;
        }

        rec = r->connections.elts;
        rec = &rec[rn->last_connection];

        પસંદગી (qtype) {

#જો (NGX_HAVE_INET6)
        સ્થિતિ NGX_RESOLVE_AAAA:

            rn->tcp6 = 1;

            (શૂન્ય) ngx_resolver_send_tcp_query(r, rec, rn->query6, rn->qlen);

            તોડો;
#endif

        મૂળભૂત: /* NGX_RESOLVE_A */

            rn->tcp = 1;

            (શૂન્ય) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);
        }

        rn->expire = ngx_time() + r->resend_timeout;

        ngx_queue_ઉમેરો_head(&r->name_resend_queue, &rn->queue);

        જાઓ next;
    }

    જો (code == 0 && rn->code) {
        code = rn->code;
    }

    જો (code == 0 && nan == 0) {

#જો (NGX_HAVE_INET6)
        પસંદગી (qtype) {

        સ્થિતિ NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            જો (rn->naddrs == (u_short) -1) {
                જાઓ next;
            }

            જો (rn->naddrs) {
                જાઓ export;
            }

            તોડો;

        મૂળભૂત: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            જો (rn->naddrs6 == (u_short) -1) {
                જાઓ next;
            }

            જો (rn->naddrs6) {
                જાઓ export;
            }
        }
#endif

        code = NGX_RESOLVE_NXDOMAIN;
    }

    જો (code) {

#જો (NGX_HAVE_INET6)
        પસંદગી (qtype) {

        સ્થિતિ NGX_RESOLVE_AAAA:

            rn->naddrs6 = 0;

            જો (rn->naddrs == (u_short) -1) {
                rn->code = (u_char) code;
                જાઓ next;
            }

            તોડો;

        મૂળભૂત: /* NGX_RESOLVE_A */

            rn->naddrs = 0;

            જો (rn->naddrs6 == (u_short) -1) {
                rn->code = (u_char) code;
                જાઓ next;
            }
        }
#endif

        next = rn->waiting;
        rn->waiting = ખાલી;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(&r->name_rbtree, &rn->ગાંઠ);

        /* unlock name mutex */

        જ્યારે (next) {
            ctx = next;
            ctx->state = code;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            next = ctx->next;

            ctx->handler(ctx);
        }

        ngx_resolver_free_ગાંઠ(r, rn);

        પાછા;
    }

    i = ans;
    naddrs = 0;
    cname = ખાલી;

    માટે (a = 0; a < nan; a++) {

        start = i;

        જ્યારે (i < n) {

            જો (buf[i] & 0xc0) {
                i += 2;
                જાઓ found;
            }

            જો (buf[i] == 0) {
                i++;
                જાઓ test_length;
            }

            i += 1 + buf[i];
        }

        જાઓ short_response;

    test_length:

        જો (i - start < 2) {
            err = "invalid name in DNS response";
            જાઓ invalid;
        }

    found:

        જો (i + કદ(ngx_resolver_an_t) >= n) {
            જાઓ short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        જો (class != 1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR class %ui in DNS response", class);
            જાઓ failed;
        }

        જો (ttl < 0) {
            ttl = 0;
        }

        rn->ttl = ngx_min(rn->ttl, (uint32_t) ttl);

        i += કદ(ngx_resolver_an_t);

        પસંદગી (type) {

        સ્થિતિ NGX_RESOLVE_A:

            જો (qtype != NGX_RESOLVE_A) {
                err = "unexpected A record in DNS response";
                જાઓ invalid;
            }

            જો (len != 4) {
                err = "invalid A record in DNS response";
                જાઓ invalid;
            }

            જો (i + 4 > n) {
                જાઓ short_response;
            }

            naddrs++;

            તોડો;

#જો (NGX_HAVE_INET6)
        સ્થિતિ NGX_RESOLVE_AAAA:

            જો (qtype != NGX_RESOLVE_AAAA) {
                err = "unexpected AAAA record in DNS response";
                જાઓ invalid;
            }

            જો (len != 16) {
                err = "invalid AAAA record in DNS response";
                જાઓ invalid;
            }

            જો (i + 16 > n) {
                જાઓ short_response;
            }

            naddrs++;

            તોડો;
#endif

        સ્થિતિ NGX_RESOLVE_CNAME:

            cname = &buf[i];

            તોડો;

        સ્થિતિ NGX_RESOLVE_DNAME:

            તોડો;

        મૂળભૂત:

            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver naddrs:%ui cname:%p ttl:%uD",
                   naddrs, cname, rn->ttl);

    જો (naddrs) {

        પસંદગી (qtype) {

#જો (NGX_HAVE_INET6)
        સ્થિતિ NGX_RESOLVE_AAAA:

            જો (naddrs == 1) {
                addr6 = &rn->u6.addr6;
                rn->naddrs6 = 1;

            } નહીંતર {
                addr6 = ngx_resolver_alloc(r, naddrs * કદ(સંરચના in6_addr));
                જો (addr6 == ખાલી) {
                    જાઓ failed;
                }

                rn->u6.addrs6 = addr6;
                rn->naddrs6 = (u_short) naddrs;
            }

#જો (NGX_SUPPRESS_WARN)
            addr = ખાલી;
#endif

            તોડો;
#endif

        મૂળભૂત: /* NGX_RESOLVE_A */

            જો (naddrs == 1) {
                addr = &rn->u.addr;
                rn->naddrs = 1;

            } નહીંતર {
                addr = ngx_resolver_alloc(r, naddrs * કદ(in_addr_t));
                જો (addr == ખાલી) {
                    જાઓ failed;
                }

                rn->u.addrs = addr;
                rn->naddrs = (u_short) naddrs;
            }

#જો (NGX_HAVE_INET6 && NGX_SUPPRESS_WARN)
            addr6 = ખાલી;
#endif
        }

        j = 0;
        i = ans;

        માટે (a = 0; a < nan; a++) {

            માટે ( ;; ) {

                જો (buf[i] & 0xc0) {
                    i += 2;
                    તોડો;
                }

                જો (buf[i] == 0) {
                    i++;
                    તોડો;
                }

                i += 1 + buf[i];
            }

            an = (ngx_resolver_an_t *) &buf[i];

            type = (an->type_hi << 8) + an->type_lo;
            len = (an->len_hi << 8) + an->len_lo;

            i += કદ(ngx_resolver_an_t);

            જો (type == NGX_RESOLVE_A) {

                addr[j] = htonl((buf[i] << 24) + (buf[i + 1] << 16)
                                + (buf[i + 2] << 8) + (buf[i + 3]));

                જો (++j == naddrs) {

#જો (NGX_HAVE_INET6)
                    જો (rn->naddrs6 == (u_short) -1) {
                        જાઓ next;
                    }
#endif

                    તોડો;
                }
            }

#જો (NGX_HAVE_INET6)
            નહીંતર જો (type == NGX_RESOLVE_AAAA) {

                ngx_memcpy(addr6[j].s6_addr, &buf[i], 16);

                જો (++j == naddrs) {

                    જો (rn->naddrs == (u_short) -1) {
                        જાઓ next;
                    }

                    તોડો;
                }
            }
#endif

            i += len;
        }
    }

    પસંદગી (qtype) {

#જો (NGX_HAVE_INET6)
    સ્થિતિ NGX_RESOLVE_AAAA:

        જો (rn->naddrs6 == (u_short) -1) {
            rn->naddrs6 = 0;
        }

        તોડો;
#endif

    મૂળભૂત: /* NGX_RESOLVE_A */

        જો (rn->naddrs == (u_short) -1) {
            rn->naddrs = 0;
        }
    }

    જો (rn->naddrs != (u_short) -1
#જો (NGX_HAVE_INET6)
        && rn->naddrs6 != (u_short) -1
#endif
        && rn->naddrs
#જો (NGX_HAVE_INET6)
           + rn->naddrs6
#endif
           > 0)
    {

#જો (NGX_HAVE_INET6)
    export:
#endif

        naddrs = rn->naddrs;
#જો (NGX_HAVE_INET6)
        naddrs += rn->naddrs6;
#endif

        જો (naddrs == 1 && rn->naddrs == 1) {
            addrs = ખાલી;

        } નહીંતર {
            addrs = ngx_resolver_export(r, rn, 0);
            જો (addrs == ખાલી) {
                જાઓ failed;
            }
        }

        ngx_queue_remove(&rn->queue);

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_ઉમેરો_head(&r->name_expire_queue, &rn->queue);

        next = rn->waiting;
        rn->waiting = ખાલી;

        /* unlock name mutex */

        જ્યારે (next) {
            ctx = next;
            ctx->state = NGX_OK;
            ctx->valid = rn->valid;
            ctx->naddrs = naddrs;

            જો (addrs == ખાલી) {
                ctx->addrs = &ctx->addr;
                ctx->addr.sockaddr = (સંરચના sockaddr *) &ctx->sin;
                ctx->addr.socklen = કદ(સંરચના sockaddr_in);
                ngx_memzero(&ctx->sin, કદ(સંરચના sockaddr_in));
                ctx->sin.sin_family = AF_INET;
                ctx->sin.sin_addr.s_addr = rn->u.addr;

            } નહીંતર {
                ctx->addrs = addrs;
            }

            next = ctx->next;

            ctx->handler(ctx);
        }

        જો (addrs != ખાલી) {
            ngx_resolver_free(r, addrs->sockaddr);
            ngx_resolver_free(r, addrs);
        }

        ngx_resolver_free(r, rn->query);
        rn->query = ખાલી;
#જો (NGX_HAVE_INET6)
        rn->query6 = ખાલી;
#endif

        પાછા;
    }

    જો (cname) {

        /* CNAME only */

        જો (rn->naddrs == (u_short) -1
#જો (NGX_HAVE_INET6)
            || rn->naddrs6 == (u_short) -1
#endif
            )
        {
            જાઓ next;
        }

        જો (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            જાઓ failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_queue_remove(&rn->queue);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.data;

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_ઉમેરો_head(&r->name_expire_queue, &rn->queue);

        ngx_resolver_free(r, rn->query);
        rn->query = ખાલી;
#જો (NGX_HAVE_INET6)
        rn->query6 = ખાલી;
#endif

        ctx = rn->waiting;
        rn->waiting = ખાલી;

        જો (ctx) {

            જો (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                કરો {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    next = ctx->next;

                    ctx->handler(ctx);

                    ctx = next;
                } જ્યારે (ctx);

                પાછા;
            }

            માટે (next = ctx; next; next = next->next) {
                next->ગાંઠ = ખાલી;
            }

            (શૂન્ય) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        પાછા;
    }

    ngx_log_error(r->log_level, r->log, 0,
                  "no A or CNAME types in DNS response");
    પાછા;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->log, 0, err);

    પાછા;

failed:

next:

    /* unlock name mutex */

    પાછા;
}


સ્થિર શૂન્ય
ngx_resolver_process_srv(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan,
    ngx_uint_t trunc, ngx_uint_t ans)
{
    અક્ષર                       *err;
    u_char                     *cname;
    size_t                      len;
    int32_t                     ttl;
    uint32_t                    hash;
    ngx_str_t                   name;
    ngx_uint_t                  type, qident, class, start, nsrvs, a, i, j;
    ngx_resolver_an_t          *an;
    ngx_resolver_ctx_t         *ctx, *next;
    ngx_resolver_srv_t         *srvs;
    ngx_resolver_ગાંઠ_t        *rn;
    ngx_resolver_connection_t  *rec;

    જો (ngx_resolver_copy(r, &name, buf,
                          buf + કદ(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        પાછા;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver qs:%V", &name);

    hash = ngx_crc32_short(name.data, name.len);

    rn = ngx_resolver_lookup_srv(r, &name, hash);

    જો (rn == ખાલી || rn->query == ખાલી) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.data);
        જાઓ failed;
    }

    જો (trunc && rn->tcp) {
        ngx_resolver_free(r, name.data);
        જાઓ failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    જો (ident != qident) {
        ngx_log_error(r->log_level, r->log, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.data);
        જાઓ failed;
    }

    ngx_resolver_free(r, name.data);

    જો (trunc) {

        ngx_queue_remove(&rn->queue);

        જો (rn->waiting == ખાલી) {
            ngx_rbtree_delete(&r->srv_rbtree, &rn->ગાંઠ);
            ngx_resolver_free_ગાંઠ(r, rn);
            પાછા;
        }

        rec = r->connections.elts;
        rec = &rec[rn->last_connection];

        rn->tcp = 1;

        (શૂન્ય) ngx_resolver_send_tcp_query(r, rec, rn->query, rn->qlen);

        rn->expire = ngx_time() + r->resend_timeout;

        ngx_queue_ઉમેરો_head(&r->srv_resend_queue, &rn->queue);

        પાછા;
    }

    જો (code == 0 && rn->code) {
        code = rn->code;
    }

    જો (code == 0 && nan == 0) {
        code = NGX_RESOLVE_NXDOMAIN;
    }

    જો (code) {
        next = rn->waiting;
        rn->waiting = ખાલી;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(&r->srv_rbtree, &rn->ગાંઠ);

        જ્યારે (next) {
            ctx = next;
            ctx->state = code;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            next = ctx->next;

            ctx->handler(ctx);
        }

        ngx_resolver_free_ગાંઠ(r, rn);

        પાછા;
    }

    i = ans;
    nsrvs = 0;
    cname = ખાલી;

    માટે (a = 0; a < nan; a++) {

        start = i;

        જ્યારે (i < n) {

            જો (buf[i] & 0xc0) {
                i += 2;
                જાઓ found;
            }

            જો (buf[i] == 0) {
                i++;
                જાઓ test_length;
            }

            i += 1 + buf[i];
        }

        જાઓ short_response;

    test_length:

        જો (i - start < 2) {
            err = "invalid name DNS response";
            જાઓ invalid;
        }

    found:

        જો (i + કદ(ngx_resolver_an_t) >= n) {
            જાઓ short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        જો (class != 1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR class %ui in DNS response", class);
            જાઓ failed;
        }

        જો (ttl < 0) {
            ttl = 0;
        }

        rn->ttl = ngx_min(rn->ttl, (uint32_t) ttl);

        i += કદ(ngx_resolver_an_t);

        પસંદગી (type) {

        સ્થિતિ NGX_RESOLVE_SRV:

            જો (i + 6 >= n) {
                જાઓ short_response;
            }

            જો (ngx_resolver_copy(r, ખાલી, buf, &buf[i + 6], buf + n)
                != NGX_OK)
            {
                જાઓ failed;
            }

            nsrvs++;

            તોડો;

        સ્થિતિ NGX_RESOLVE_CNAME:

            cname = &buf[i];

            તોડો;

        સ્થિતિ NGX_RESOLVE_DNAME:

            તોડો;

        મૂળભૂત:

            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolver nsrvs:%ui cname:%p ttl:%uD",
                   nsrvs, cname, rn->ttl);

    જો (nsrvs) {

        srvs = ngx_resolver_calloc(r, nsrvs * કદ(ngx_resolver_srv_t));
        જો (srvs == ખાલી) {
            જાઓ failed;
        }

        rn->u.srvs = srvs;
        rn->nsrvs = (u_short) nsrvs;

        j = 0;
        i = ans;

        માટે (a = 0; a < nan; a++) {

            માટે ( ;; ) {

                જો (buf[i] & 0xc0) {
                    i += 2;
                    તોડો;
                }

                જો (buf[i] == 0) {
                    i++;
                    તોડો;
                }

                i += 1 + buf[i];
            }

            an = (ngx_resolver_an_t *) &buf[i];

            type = (an->type_hi << 8) + an->type_lo;
            len = (an->len_hi << 8) + an->len_lo;

            i += કદ(ngx_resolver_an_t);

            જો (type == NGX_RESOLVE_SRV) {

                srvs[j].priority = (buf[i] << 8) + buf[i + 1];
                srvs[j].weight = (buf[i + 2] << 8) + buf[i + 3];

                જો (srvs[j].weight == 0) {
                    srvs[j].weight = 1;
                }

                srvs[j].port = (buf[i + 4] << 8) + buf[i + 5];

                જો (ngx_resolver_copy(r, &srvs[j].name, buf, &buf[i + 6],
                                      buf + n)
                    != NGX_OK)
                {
                    જાઓ failed;
                }

                j++;
            }

            i += len;
        }

        ngx_sort(srvs, nsrvs, કદ(ngx_resolver_srv_t),
                 ngx_resolver_cmp_srvs);

        ngx_resolver_free(r, rn->query);
        rn->query = ખાલી;

        ngx_queue_remove(&rn->queue);

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_ઉમેરો_head(&r->srv_expire_queue, &rn->queue);

        next = rn->waiting;
        rn->waiting = ખાલી;

        જ્યારે (next) {
            ctx = next;
            next = ctx->next;

            ngx_resolver_resolve_srv_names(ctx, rn);
        }

        પાછા;
    }

    rn->nsrvs = 0;

    જો (cname) {

        /* CNAME only */

        જો (ngx_resolver_copy(r, &name, buf, cname, buf + n) != NGX_OK) {
            જાઓ failed;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0,
                       "resolver cname:\"%V\"", &name);

        ngx_queue_remove(&rn->queue);

        rn->cnlen = (u_short) name.len;
        rn->u.cname = name.data;

        rn->valid = ngx_time() + (r->valid ? r->valid : (time_t) rn->ttl);
        rn->expire = ngx_time() + r->expire;

        ngx_queue_ઉમેરો_head(&r->srv_expire_queue, &rn->queue);

        ngx_resolver_free(r, rn->query);
        rn->query = ખાલી;
#જો (NGX_HAVE_INET6)
        rn->query6 = ખાલી;
#endif

        ctx = rn->waiting;
        rn->waiting = ખાલી;

        જો (ctx) {

            જો (ctx->recursion++ >= NGX_RESOLVER_MAX_RECURSION) {

                /* unlock name mutex */

                કરો {
                    ctx->state = NGX_RESOLVE_NXDOMAIN;
                    next = ctx->next;

                    ctx->handler(ctx);

                    ctx = next;
                } જ્યારે (ctx);

                પાછા;
            }

            માટે (next = ctx; next; next = next->next) {
                next->ગાંઠ = ખાલી;
            }

            (શૂન્ય) ngx_resolve_name_locked(r, ctx, &name);
        }

        /* unlock name mutex */

        પાછા;
    }

    ngx_log_error(r->log_level, r->log, 0, "no SRV type in DNS response");

    પાછા;

short_response:

    err = "short DNS response";

invalid:

    /* unlock name mutex */

    ngx_log_error(r->log_level, r->log, 0, err);

    પાછા;

failed:

    /* unlock name mutex */

    પાછા;
}


સ્થિર શૂન્ય
ngx_resolver_resolve_srv_names(ngx_resolver_ctx_t *ctx, ngx_resolver_ગાંઠ_t *rn)
{
    ngx_uint_t                i;
    ngx_resolver_t           *r;
    ngx_resolver_ctx_t       *cctx;
    ngx_resolver_srv_name_t  *srvs;

    r = ctx->resolver;

    ctx->ગાંઠ = ખાલી;
    ctx->state = NGX_OK;
    ctx->valid = rn->valid;
    ctx->ગણતરી = rn->nsrvs;

    srvs = ngx_resolver_calloc(r, rn->nsrvs * કદ(ngx_resolver_srv_name_t));
    જો (srvs == ખાલી) {
        જાઓ failed;
    }

    ctx->srvs = srvs;
    ctx->nsrvs = rn->nsrvs;

    જો (ctx->event && ctx->event->timer_set) {
        ngx_del_timer(ctx->event);
    }

    માટે (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
        srvs[i].name.data = ngx_resolver_alloc(r, rn->u.srvs[i].name.len);
        જો (srvs[i].name.data == ખાલી) {
            જાઓ failed;
        }

        srvs[i].name.len = rn->u.srvs[i].name.len;
        ngx_memcpy(srvs[i].name.data, rn->u.srvs[i].name.data,
                   srvs[i].name.len);

        cctx = ngx_resolve_start(r, ખાલી);
        જો (cctx == ખાલી) {
            જાઓ failed;
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

        જો (ngx_resolve_name(cctx) == NGX_ERROR) {
            srvs[i].ctx = ખાલી;
            જાઓ failed;
        }
    }

    પાછા;

failed:

    ctx->state = NGX_ERROR;
    ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

    ctx->handler(ctx);
}


સ્થિર શૂન્ય
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

    ctx->ગણતરી--;
    ctx->async |= cctx->async;

    srv->ctx = ખાલી;
    srv->state = cctx->state;

    જો (cctx->naddrs) {

        ctx->valid = ngx_min(ctx->valid, cctx->valid);

        addrs = ngx_resolver_calloc(r, cctx->naddrs * કદ(ngx_addr_t));
        જો (addrs == ખાલી) {
            srv->state = NGX_ERROR;
            જાઓ done;
        }

        sockaddr = ngx_resolver_alloc(r, cctx->naddrs * કદ(ngx_sockaddr_t));
        જો (sockaddr == ખાલી) {
            ngx_resolver_free(r, addrs);
            srv->state = NGX_ERROR;
            જાઓ done;
        }

        માટે (i = 0; i < cctx->naddrs; i++) {
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

    જો (ctx->ગણતરી == 0) {
        ngx_resolver_report_srv(r, ctx);
    }
}


સ્થિર શૂન્ય
ngx_resolver_process_ptr(ngx_resolver_t *r, u_char *buf, size_t n,
    ngx_uint_t ident, ngx_uint_t code, ngx_uint_t nan)
{
    અક્ષર                 *err;
    size_t                len;
    in_addr_t             addr;
    int32_t               ttl;
    ngx_int_t             octet;
    ngx_str_t             name;
    ngx_uint_t            mask, type, class, qident, a, i, start;
    ngx_queue_t          *expire_queue;
    ngx_rbtree_t         *વૃક્ષ;
    ngx_resolver_an_t    *an;
    ngx_resolver_ctx_t   *ctx, *next;
    ngx_resolver_ગાંઠ_t  *rn;
#જો (NGX_HAVE_INET6)
    uint32_t              hash;
    ngx_int_t             digit;
    સંરચના in6_addr       addr6;
#endif

    જો (ngx_resolver_copy(r, &name, buf,
                          buf + કદ(ngx_resolver_hdr_t), buf + n)
        != NGX_OK)
    {
        પાછા;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver qs:%V", &name);

    /* AF_INET */

    addr = 0;
    i = કદ(ngx_resolver_hdr_t);

    માટે (mask = 0; mask < 32; mask += 8) {
        len = buf[i++];

        octet = ngx_atoi(&buf[i], len);
        જો (octet == NGX_ERROR || octet > 255) {
            જાઓ invalid_in_addr_arpa;
        }

        addr += octet << mask;
        i += len;
    }

    જો (ngx_strcasecmp(&buf[i], (u_char *) "\7in-addr\4arpa") == 0) {
        i += કદ("\7in-addr\4arpa");

        /* lock addr mutex */

        rn = ngx_resolver_lookup_addr(r, addr);

        વૃક્ષ = &r->addr_rbtree;
        expire_queue = &r->addr_expire_queue;

        જાઓ valid;
    }

invalid_in_addr_arpa:

#જો (NGX_HAVE_INET6)

    i = કદ(ngx_resolver_hdr_t);

    માટે (octet = 15; octet >= 0; octet--) {
        જો (buf[i++] != '\1') {
            જાઓ invalid_ip6_arpa;
        }

        digit = ngx_hextoi(&buf[i++], 1);
        જો (digit == NGX_ERROR) {
            જાઓ invalid_ip6_arpa;
        }

        addr6.s6_addr[octet] = (u_char) digit;

        જો (buf[i++] != '\1') {
            જાઓ invalid_ip6_arpa;
        }

        digit = ngx_hextoi(&buf[i++], 1);
        જો (digit == NGX_ERROR) {
            જાઓ invalid_ip6_arpa;
        }

        addr6.s6_addr[octet] += (u_char) (digit * 16);
    }

    જો (ngx_strcasecmp(&buf[i], (u_char *) "\3ip6\4arpa") == 0) {
        i += કદ("\3ip6\4arpa");

        /* lock addr mutex */

        hash = ngx_crc32_short(addr6.s6_addr, 16);
        rn = ngx_resolver_lookup_addr6(r, &addr6, hash);

        વૃક્ષ = &r->addr6_rbtree;
        expire_queue = &r->addr6_expire_queue;

        જાઓ valid;
    }

invalid_ip6_arpa:
#endif

    ngx_log_error(r->log_level, r->log, 0,
                  "invalid in-addr.arpa or ip6.arpa name in DNS response");
    ngx_resolver_free(r, name.data);
    પાછા;

valid:

    જો (rn == ખાલી || rn->query == ખાલી) {
        ngx_log_error(r->log_level, r->log, 0,
                      "unexpected DNS response for %V", &name);
        ngx_resolver_free(r, name.data);
        જાઓ failed;
    }

    qident = (rn->query[0] << 8) + rn->query[1];

    જો (ident != qident) {
        ngx_log_error(r->log_level, r->log, 0,
                      "wrong ident %ui in DNS response for %V, expect %ui",
                      ident, &name, qident);
        ngx_resolver_free(r, name.data);
        જાઓ failed;
    }

    ngx_resolver_free(r, name.data);

    જો (code == 0 && nan == 0) {
        code = NGX_RESOLVE_NXDOMAIN;
    }

    જો (code) {
        next = rn->waiting;
        rn->waiting = ખાલી;

        ngx_queue_remove(&rn->queue);

        ngx_rbtree_delete(વૃક્ષ, &rn->ગાંઠ);

        /* unlock addr mutex */

        જ્યારે (next) {
            ctx = next;
            ctx->state = code;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);
            next = ctx->next;

            ctx->handler(ctx);
        }

        ngx_resolver_free_ગાંઠ(r, rn);

        પાછા;
    }

    i += કદ(ngx_resolver_qs_t);

    માટે (a = 0; a < nan; a++) {

        start = i;

        જ્યારે (i < n) {

            જો (buf[i] & 0xc0) {
                i += 2;
                જાઓ found;
            }

            જો (buf[i] == 0) {
                i++;
                જાઓ test_length;
            }

            i += 1 + buf[i];
        }

        જાઓ short_response;

    test_length:

        જો (i - start < 2) {
            err = "invalid name in DNS response";
            જાઓ invalid;
        }

    found:

        જો (i + કદ(ngx_resolver_an_t) >= n) {
            જાઓ short_response;
        }

        an = (ngx_resolver_an_t *) &buf[i];

        type = (an->type_hi << 8) + an->type_lo;
        class = (an->class_hi << 8) + an->class_lo;
        len = (an->len_hi << 8) + an->len_lo;
        ttl = (an->ttl[0] << 24) + (an->ttl[1] << 16)
            + (an->ttl[2] << 8) + (an->ttl[3]);

        જો (class != 1) {
            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR class %ui in DNS response", class);
            જાઓ failed;
        }

        જો (ttl < 0) {
            ttl = 0;
        }

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, r->log, 0,
                      "resolver qt:%ui cl:%ui len:%uz",
                      type, class, len);

        i += કદ(ngx_resolver_an_t);

        પસંદગી (type) {

        સ્થિતિ NGX_RESOLVE_PTR:

            જાઓ ptr;

        સ્થિતિ NGX_RESOLVE_CNAME:

            તોડો;

        મૂળભૂત:

            ngx_log_error(r->log_level, r->log, 0,
                          "unexpected RR type %ui in DNS response", type);
        }

        i += len;
    }

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->log, 0,
                  "no PTR type in DNS response");
    પાછા;

ptr:

    જો (ngx_resolver_copy(r, &name, buf, buf + i, buf + n) != NGX_OK) {
        જાઓ failed;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, r->log, 0, "resolver an:%V", &name);

    જો (name.len != (size_t) rn->nlen
        || ngx_strncmp(name.data, rn->name, name.len) != 0)
    {
        જો (rn->nlen) {
            ngx_resolver_free(r, rn->name);
        }

        rn->nlen = (u_short) name.len;
        rn->name = name.data;

        name.data = ngx_resolver_dup(r, rn->name, name.len);
        જો (name.data == ખાલી) {
            જાઓ failed;
        }
    }

    ngx_queue_remove(&rn->queue);

    rn->valid = ngx_time() + (r->valid ? r->valid : ttl);
    rn->expire = ngx_time() + r->expire;

    ngx_queue_ઉમેરો_head(expire_queue, &rn->queue);

    next = rn->waiting;
    rn->waiting = ખાલી;

    /* unlock addr mutex */

    જ્યારે (next) {
        ctx = next;
        ctx->state = NGX_OK;
        ctx->valid = rn->valid;
        ctx->name = name;
        next = ctx->next;

        ctx->handler(ctx);
    }

    ngx_resolver_free(r, name.data);

    પાછા;

short_response:

    err = "short DNS response";

invalid:

    /* unlock addr mutex */

    ngx_log_error(r->log_level, r->log, 0, err);

    પાછા;

failed:

    /* unlock addr mutex */

    પાછા;
}


સ્થિર ngx_resolver_ગાંઠ_t *
ngx_resolver_lookup_name(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_ગાંઠ_t    *ગાંઠ, *sentinel;
    ngx_resolver_ગાંઠ_t  *rn;

    ગાંઠ = r->name_rbtree.મૂળ;
    sentinel = r->name_rbtree.sentinel;

    જ્યારે (ગાંઠ != sentinel) {

        જો (hash < ગાંઠ->ચાવી) {
            ગાંઠ = ગાંઠ->ડાબું;
            ચાલુ;
        }

        જો (hash > ગાંઠ->ચાવી) {
            ગાંઠ = ગાંઠ->જમણું;
            ચાલુ;
        }

        /* hash == node->key */

        rn = ngx_resolver_ગાંઠ(ગાંઠ);

        rc = ngx_memn2cmp(name->data, rn->name, name->len, rn->nlen);

        જો (rc == 0) {
            પાછા rn;
        }

        ગાંઠ = (rc < 0) ? ગાંઠ->ડાબું : ગાંઠ->જમણું;
    }

    /* not found */

    પાછા ખાલી;
}


સ્થિર ngx_resolver_ગાંઠ_t *
ngx_resolver_lookup_srv(ngx_resolver_t *r, ngx_str_t *name, uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_ગાંઠ_t    *ગાંઠ, *sentinel;
    ngx_resolver_ગાંઠ_t  *rn;

    ગાંઠ = r->srv_rbtree.મૂળ;
    sentinel = r->srv_rbtree.sentinel;

    જ્યારે (ગાંઠ != sentinel) {

        જો (hash < ગાંઠ->ચાવી) {
            ગાંઠ = ગાંઠ->ડાબું;
            ચાલુ;
        }

        જો (hash > ગાંઠ->ચાવી) {
            ગાંઠ = ગાંઠ->જમણું;
            ચાલુ;
        }

        /* hash == node->key */

        rn = ngx_resolver_ગાંઠ(ગાંઠ);

        rc = ngx_memn2cmp(name->data, rn->name, name->len, rn->nlen);

        જો (rc == 0) {
            પાછા rn;
        }

        ગાંઠ = (rc < 0) ? ગાંઠ->ડાબું : ગાંઠ->જમણું;
    }

    /* not found */

    પાછા ખાલી;
}


સ્થિર ngx_resolver_ગાંઠ_t *
ngx_resolver_lookup_addr(ngx_resolver_t *r, in_addr_t addr)
{
    ngx_rbtree_ગાંઠ_t  *ગાંઠ, *sentinel;

    ગાંઠ = r->addr_rbtree.મૂળ;
    sentinel = r->addr_rbtree.sentinel;

    જ્યારે (ગાંઠ != sentinel) {

        જો (addr < ગાંઠ->ચાવી) {
            ગાંઠ = ગાંઠ->ડાબું;
            ચાલુ;
        }

        જો (addr > ગાંઠ->ચાવી) {
            ગાંઠ = ગાંઠ->જમણું;
            ચાલુ;
        }

        /* addr == node->key */

        પાછા ngx_resolver_ગાંઠ(ગાંઠ);
    }

    /* not found */

    પાછા ખાલી;
}


#જો (NGX_HAVE_INET6)

સ્થિર ngx_resolver_ગાંઠ_t *
ngx_resolver_lookup_addr6(ngx_resolver_t *r, સંરચના in6_addr *addr,
    uint32_t hash)
{
    ngx_int_t             rc;
    ngx_rbtree_ગાંઠ_t    *ગાંઠ, *sentinel;
    ngx_resolver_ગાંઠ_t  *rn;

    ગાંઠ = r->addr6_rbtree.મૂળ;
    sentinel = r->addr6_rbtree.sentinel;

    જ્યારે (ગાંઠ != sentinel) {

        જો (hash < ગાંઠ->ચાવી) {
            ગાંઠ = ગાંઠ->ડાબું;
            ચાલુ;
        }

        જો (hash > ગાંઠ->ચાવી) {
            ગાંઠ = ગાંઠ->જમણું;
            ચાલુ;
        }

        /* hash == node->key */

        rn = ngx_resolver_ગાંઠ(ગાંઠ);

        rc = ngx_memcmp(addr, &rn->addr6, 16);

        જો (rc == 0) {
            પાછા rn;
        }

        ગાંઠ = (rc < 0) ? ગાંઠ->ડાબું : ગાંઠ->જમણું;
    }

    /* not found */

    પાછા ખાલી;
}

#endif


સ્થિર શૂન્ય
ngx_resolver_rbtree_ઉમેરો_કિંમત(ngx_rbtree_ગાંઠ_t *temp,
    ngx_rbtree_ગાંઠ_t *ગાંઠ, ngx_rbtree_ગાંઠ_t *sentinel)
{
    ngx_rbtree_ગાંઠ_t    **p;
    ngx_resolver_ગાંઠ_t   *rn, *rn_temp;

    માટે ( ;; ) {

        જો (ગાંઠ->ચાવી < temp->ચાવી) {

            p = &temp->ડાબું;

        } નહીંતર જો (ગાંઠ->ચાવી > temp->ચાવી) {

            p = &temp->જમણું;

        } નહીંતર { /* node->key == temp->key */

            rn = ngx_resolver_ગાંઠ(ગાંઠ);
            rn_temp = ngx_resolver_ગાંઠ(temp);

            p = (ngx_memn2cmp(rn->name, rn_temp->name, rn->nlen, rn_temp->nlen)
                 < 0) ? &temp->ડાબું : &temp->જમણું;
        }

        જો (*p == sentinel) {
            તોડો;
        }

        temp = *p;
    }

    *p = ગાંઠ;
    ગાંઠ->parent = temp;
    ગાંઠ->ડાબું = sentinel;
    ગાંઠ->જમણું = sentinel;
    ngx_rbt_red(ગાંઠ);
}


#જો (NGX_HAVE_INET6)

સ્થિર શૂન્ય
ngx_resolver_rbtree_ઉમેરો_addr6_કિંમત(ngx_rbtree_ગાંઠ_t *temp,
    ngx_rbtree_ગાંઠ_t *ગાંઠ, ngx_rbtree_ગાંઠ_t *sentinel)
{
    ngx_rbtree_ગાંઠ_t    **p;
    ngx_resolver_ગાંઠ_t   *rn, *rn_temp;

    માટે ( ;; ) {

        જો (ગાંઠ->ચાવી < temp->ચાવી) {

            p = &temp->ડાબું;

        } નહીંતર જો (ગાંઠ->ચાવી > temp->ચાવી) {

            p = &temp->જમણું;

        } નહીંતર { /* node->key == temp->key */

            rn = ngx_resolver_ગાંઠ(ગાંઠ);
            rn_temp = ngx_resolver_ગાંઠ(temp);

            p = (ngx_memcmp(&rn->addr6, &rn_temp->addr6, 16)
                 < 0) ? &temp->ડાબું : &temp->જમણું;
        }

        જો (*p == sentinel) {
            તોડો;
        }

        temp = *p;
    }

    *p = ગાંઠ;
    ગાંઠ->parent = temp;
    ગાંઠ->ડાબું = sentinel;
    ગાંઠ->જમણું = sentinel;
    ngx_rbt_red(ગાંઠ);
}

#endif


સ્થિર ngx_int_t
ngx_resolver_create_name_query(ngx_resolver_t *r, ngx_resolver_ગાંઠ_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    size_t               len, nlen;
    ngx_uint_t           ident;
    ngx_resolver_qs_t   *qs;
    ngx_resolver_hdr_t  *query;

    nlen = name->len ? (1 + name->len + 1) : 1;

    len = કદ(ngx_resolver_hdr_t) + nlen + કદ(ngx_resolver_qs_t);

#જો (NGX_HAVE_INET6)
    p = ngx_resolver_alloc(r, len * (r->ipv4 + r->ipv6));
#નહીંતર
    p = ngx_resolver_alloc(r, len);
#endif
    જો (p == ખાલી) {
        પાછા NGX_ERROR;
    }

    rn->qlen = (u_short) len;
    rn->query = p;

#જો (NGX_HAVE_INET6)
    જો (r->ipv6) {
        rn->query6 = r->ipv4 ? (p + len) : p;
    }
#endif

    query = (ngx_resolver_hdr_t *) p;

    જો (r->ipv4) {
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

    p += કદ(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    /* query type */
    qs->type_hi = 0; qs->type_lo = NGX_RESOLVE_A;

    /* IN query class */
    qs->class_hi = 0; qs->class_lo = 1;

    /* convert "www.example.com" to "\3www\7example\3com\0" */

    len = 0;
    p--;
    *p-- = '\0';

    જો (name->len == 0)  {
        પાછા NGX_DECLINED;
    }

    માટે (s = name->data + name->len - 1; s >= name->data; s--) {
        જો (*s != '.') {
            *p = *s;
            len++;

        } નહીંતર {
            જો (len == 0 || len > 255) {
                પાછા NGX_DECLINED;
            }

            *p = (u_char) len;
            len = 0;
        }

        p--;
    }

    જો (len == 0 || len > 255) {
        પાછા NGX_DECLINED;
    }

    *p = (u_char) len;

#જો (NGX_HAVE_INET6)
    જો (!r->ipv6) {
        પાછા NGX_OK;
    }

    p = rn->query6;

    જો (r->ipv4) {
        ngx_memcpy(p, rn->query, rn->qlen);
    }

    query = (ngx_resolver_hdr_t *) p;

    ident = ngx_random();

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, r->log, 0,
                   "resolve: \"%V\" AAAA %i", name, ident & 0xffff);

    query->ident_hi = (u_char) ((ident >> 8) & 0xff);
    query->ident_lo = (u_char) (ident & 0xff);

    p += કદ(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    qs->type_lo = NGX_RESOLVE_AAAA;
#endif

    પાછા NGX_OK;
}


સ્થિર ngx_int_t
ngx_resolver_create_srv_query(ngx_resolver_t *r, ngx_resolver_ગાંઠ_t *rn,
    ngx_str_t *name)
{
    u_char              *p, *s;
    size_t               len, nlen;
    ngx_uint_t           ident;
    ngx_resolver_qs_t   *qs;
    ngx_resolver_hdr_t  *query;

    nlen = name->len ? (1 + name->len + 1) : 1;

    len = કદ(ngx_resolver_hdr_t) + nlen + કદ(ngx_resolver_qs_t);

    p = ngx_resolver_alloc(r, len);
    જો (p == ખાલી) {
        પાછા NGX_ERROR;
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

    p += કદ(ngx_resolver_hdr_t) + nlen;

    qs = (ngx_resolver_qs_t *) p;

    /* query type */
    qs->type_hi = 0; qs->type_lo = NGX_RESOLVE_SRV;

    /* IN query class */
    qs->class_hi = 0; qs->class_lo = 1;

    /* converts "www.example.com" to "\3www\7example\3com\0" */

    len = 0;
    p--;
    *p-- = '\0';

    જો (name->len == 0)  {
        પાછા NGX_DECLINED;
    }

    માટે (s = name->data + name->len - 1; s >= name->data; s--) {
        જો (*s != '.') {
            *p = *s;
            len++;

        } નહીંતર {
            જો (len == 0 || len > 255) {
                પાછા NGX_DECLINED;
            }

            *p = (u_char) len;
            len = 0;
        }

        p--;
    }

    જો (len == 0 || len > 255) {
        પાછા NGX_DECLINED;
    }

    *p = (u_char) len;

    પાછા NGX_OK;
}


સ્થિર ngx_int_t
ngx_resolver_create_addr_query(ngx_resolver_t *r, ngx_resolver_ગાંઠ_t *rn,
    ngx_resolver_addr_t *addr)
{
    u_char               *p, *d;
    size_t                len;
    in_addr_t             inaddr;
    ngx_int_t             n;
    ngx_uint_t            ident;
    ngx_resolver_hdr_t   *query;
    સંરચના sockaddr_in   *sin;
#જો (NGX_HAVE_INET6)
    સંરચના sockaddr_in6  *sin6;
#endif

    પસંદગી (addr->sockaddr->sa_family) {

#જો (NGX_HAVE_INET6)
    સ્થિતિ AF_INET6:
        len = કદ(ngx_resolver_hdr_t)
              + 64 + કદ(".ip6.arpa.") - 1
              + કદ(ngx_resolver_qs_t);

        તોડો;
#endif

    મૂળભૂત: /* AF_INET */
        len = કદ(ngx_resolver_hdr_t)
              + કદ(".255.255.255.255.in-addr.arpa.") - 1
              + કદ(ngx_resolver_qs_t);
    }

    p = ngx_resolver_alloc(r, len);
    જો (p == ખાલી) {
        પાછા NGX_ERROR;
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

    p += કદ(ngx_resolver_hdr_t);

    પસંદગી (addr->sockaddr->sa_family) {

#જો (NGX_HAVE_INET6)
    સ્થિતિ AF_INET6:
        sin6 = (સંરચના sockaddr_in6 *) addr->sockaddr;

        માટે (n = 15; n >= 0; n--) {
            p = ngx_sprintf(p, "\1%xd\1%xd",
                            sin6->sin6_addr.s6_addr[n] & 0xf,
                            (sin6->sin6_addr.s6_addr[n] >> 4) & 0xf);
        }

        p = ngx_cpymem(p, "\3ip6\4arpa\0", 10);

        તોડો;
#endif

    મૂળભૂત: /* AF_INET */

        sin = (સંરચના sockaddr_in *) addr->sockaddr;
        inaddr = ntohl(sin->sin_addr.s_addr);

        માટે (n = 0; n < 32; n += 8) {
            d = ngx_sprintf(&p[1], "%ud", (inaddr >> n) & 0xff);
            *p = (u_char) (d - &p[1]);
            p = d;
        }

        p = ngx_cpymem(p, "\7in-addr\4arpa\0", 14);
    }

    /* query type "PTR", IN query class */
    p = ngx_cpymem(p, "\0\14\0\1", 4);

    rn->qlen = (u_short) (p - rn->query);

    પાછા NGX_OK;
}


સ્થિર ngx_int_t
ngx_resolver_copy(ngx_resolver_t *r, ngx_str_t *name, u_char *buf, u_char *src,
    u_char *last)
{
    અક્ષર        *err;
    u_char      *p, *dst;
    size_t       len;
    ngx_uint_t   i, n;

    p = src;
    len = 0;

    /*
     * compression pointers allow to create endless loop, so we set limit;
     * 128 pointers should be enough to store 255-byte name
     */

    માટે (i = 0; i < 128; i++) {
        n = *p++;

        જો (n == 0) {
            જાઓ done;
        }

        જો (n & 0xc0) {
            જો ((n & 0xc0) != 0xc0) {
                err = "invalid label type in DNS response";
                જાઓ invalid;
            }

            જો (p >= last) {
                err = "name is out of DNS response";
                જાઓ invalid;
            }

            n = ((n & 0x3f) << 8) + *p;
            p = &buf[n];

        } નહીંતર {
            len += 1 + n;
            p = &p[n];
        }

        જો (p >= last) {
            err = "name is out of DNS response";
            જાઓ invalid;
        }
    }

    err = "compression pointers loop in DNS response";

invalid:

    ngx_log_error(r->log_level, r->log, 0, err);

    પાછા NGX_ERROR;

done:

    જો (name == ખાલી) {
        પાછા NGX_OK;
    }

    જો (len == 0) {
        ngx_str_null(name);
        પાછા NGX_OK;
    }

    dst = ngx_resolver_alloc(r, len);
    જો (dst == ખાલી) {
        પાછા NGX_ERROR;
    }

    name->data = dst;

    માટે ( ;; ) {
        n = *src++;

        જો (n == 0) {
            name->len = dst - name->data - 1;
            પાછા NGX_OK;
        }

        જો (n & 0xc0) {
            n = ((n & 0x3f) << 8) + *src;
            src = &buf[n];

        } નહીંતર {
            ngx_strlow(dst, src, n);
            dst += n;
            src += n;
            *dst++ = '.';
        }
    }
}


સ્થિર ngx_int_t
ngx_resolver_set_timeout(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx)
{
    જો (ctx->event || ctx->timeout == 0) {
        પાછા NGX_OK;
    }

    ctx->event = ngx_resolver_calloc(r, કદ(ngx_event_t));
    જો (ctx->event == ખાલી) {
        પાછા NGX_ERROR;
    }

    ctx->event->handler = ngx_resolver_timeout_handler;
    ctx->event->data = ctx;
    ctx->event->log = r->log;
    ctx->event->cancelable = ctx->cancelable;
    ctx->ident = -1;

    ngx_add_timer(ctx->event, ctx->timeout);

    પાછા NGX_OK;
}


સ્થિર શૂન્ય
ngx_resolver_timeout_handler(ngx_event_t *ev)
{
    ngx_resolver_ctx_t  *ctx;

    ctx = ev->data;

    ctx->state = NGX_RESOLVE_TIMEDOUT;

    ctx->handler(ctx);
}


સ્થિર શૂન્ય
ngx_resolver_free_ગાંઠ(ngx_resolver_t *r, ngx_resolver_ગાંઠ_t *rn)
{
    ngx_uint_t  i;

    /* lock alloc mutex */

    જો (rn->query) {
        ngx_resolver_free_locked(r, rn->query);
    }

    જો (rn->name) {
        ngx_resolver_free_locked(r, rn->name);
    }

    જો (rn->cnlen) {
        ngx_resolver_free_locked(r, rn->u.cname);
    }

    જો (rn->naddrs > 1 && rn->naddrs != (u_short) -1) {
        ngx_resolver_free_locked(r, rn->u.addrs);
    }

#જો (NGX_HAVE_INET6)
    જો (rn->naddrs6 > 1 && rn->naddrs6 != (u_short) -1) {
        ngx_resolver_free_locked(r, rn->u6.addrs6);
    }
#endif

    જો (rn->nsrvs) {
        માટે (i = 0; i < (ngx_uint_t) rn->nsrvs; i++) {
            જો (rn->u.srvs[i].name.data) {
                ngx_resolver_free_locked(r, rn->u.srvs[i].name.data);
            }
        }

        ngx_resolver_free_locked(r, rn->u.srvs);
    }

    ngx_resolver_free_locked(r, rn);

    /* unlock alloc mutex */
}


સ્થિર શૂન્ય *
ngx_resolver_alloc(ngx_resolver_t *r, size_t size)
{
    u_char  *p;

    /* lock alloc mutex */

    p = ngx_alloc(size, r->log);

    /* unlock alloc mutex */

    પાછા p;
}


સ્થિર શૂન્ય *
ngx_resolver_calloc(ngx_resolver_t *r, size_t size)
{
    u_char  *p;

    p = ngx_resolver_alloc(r, size);

    જો (p) {
        ngx_memzero(p, size);
    }

    પાછા p;
}


સ્થિર શૂન્ય
ngx_resolver_free(ngx_resolver_t *r, શૂન્ય *p)
{
    /* lock alloc mutex */

    ngx_free(p);

    /* unlock alloc mutex */
}


સ્થિર શૂન્ય
ngx_resolver_free_locked(ngx_resolver_t *r, શૂન્ય *p)
{
    ngx_free(p);
}


સ્થિર શૂન્ય *
ngx_resolver_dup(ngx_resolver_t *r, શૂન્ય *src, size_t size)
{
    શૂન્ય  *dst;

    dst = ngx_resolver_alloc(r, size);

    જો (dst == ખાલી) {
        પાછા dst;
    }

    ngx_memcpy(dst, src, size);

    પાછા dst;
}


સ્થિર ngx_resolver_addr_t *
ngx_resolver_export(ngx_resolver_t *r, ngx_resolver_ગાંઠ_t *rn,
    ngx_uint_t rotate)
{
    ngx_uint_t            d, i, j, n;
    in_addr_t            *addr;
    ngx_sockaddr_t       *sockaddr;
    સંરચના sockaddr_in   *sin;
    ngx_resolver_addr_t  *dst;
#જો (NGX_HAVE_INET6)
    સંરચના in6_addr      *addr6;
    સંરચના sockaddr_in6  *sin6;
#endif

    n = rn->naddrs;
#જો (NGX_HAVE_INET6)
    n += rn->naddrs6;
#endif

    dst = ngx_resolver_calloc(r, n * કદ(ngx_resolver_addr_t));
    જો (dst == ખાલી) {
        પાછા ખાલી;
    }

    sockaddr = ngx_resolver_calloc(r, n * કદ(ngx_sockaddr_t));
    જો (sockaddr == ખાલી) {
        ngx_resolver_free(r, dst);
        પાછા ખાલી;
    }

    i = 0;
    d = rotate ? ngx_random() % n : 0;

    જો (rn->naddrs) {
        j = rotate ? ngx_random() % rn->naddrs : 0;

        addr = (rn->naddrs == 1) ? &rn->u.addr : rn->u.addrs;

        કરો {
            sin = &sockaddr[d].sockaddr_in;
            sin->sin_family = AF_INET;
            sin->sin_addr.s_addr = addr[j++];
            dst[d].sockaddr = (સંરચના sockaddr *) sin;
            dst[d++].socklen = કદ(સંરચના sockaddr_in);

            જો (d == n) {
                d = 0;
            }

            જો (j == (ngx_uint_t) rn->naddrs) {
                j = 0;
            }
        } જ્યારે (++i < (ngx_uint_t) rn->naddrs);
    }

#જો (NGX_HAVE_INET6)
    જો (rn->naddrs6) {
        j = rotate ? ngx_random() % rn->naddrs6 : 0;

        addr6 = (rn->naddrs6 == 1) ? &rn->u6.addr6 : rn->u6.addrs6;

        કરો {
            sin6 = &sockaddr[d].sockaddr_in6;
            sin6->sin6_family = AF_INET6;
            ngx_memcpy(sin6->sin6_addr.s6_addr, addr6[j++].s6_addr, 16);
            dst[d].sockaddr = (સંરચના sockaddr *) sin6;
            dst[d++].socklen = કદ(સંરચના sockaddr_in6);

            જો (d == n) {
                d = 0;
            }

            જો (j == rn->naddrs6) {
                j = 0;
            }
        } જ્યારે (++i < n);
    }
#endif

    પાછા dst;
}


સ્થિર શૂન્ય
ngx_resolver_report_srv(ngx_resolver_t *r, ngx_resolver_ctx_t *ctx)
{
    ngx_uint_t                naddrs, nsrvs, nw, i, j, k, l, m, n, w;
    ngx_resolver_addr_t      *addrs;
    ngx_resolver_srv_name_t  *srvs;

    srvs = ctx->srvs;
    nsrvs = ctx->nsrvs;

    naddrs = 0;

    માટે (i = 0; i < nsrvs; i++) {
        જો (srvs[i].state == NGX_ERROR) {
            ctx->state = NGX_ERROR;
            ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

            ctx->handler(ctx);
            પાછા;
        }

        naddrs += srvs[i].naddrs;
    }

    જો (naddrs == 0) {
        ctx->state = srvs[0].state;

        માટે (i = 0; i < nsrvs; i++) {
            જો (srvs[i].state == NGX_RESOLVE_NXDOMAIN) {
                ctx->state = NGX_RESOLVE_NXDOMAIN;
                તોડો;
            }
        }

        ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        પાછા;
    }

    addrs = ngx_resolver_calloc(r, naddrs * કદ(ngx_resolver_addr_t));
    જો (addrs == ખાલી) {
        ctx->state = NGX_ERROR;
        ctx->valid = ngx_time() + (r->valid ? r->valid : 10);

        ctx->handler(ctx);
        પાછા;
    }

    i = 0;
    n = 0;

    કરો {
        nw = 0;

        માટે (j = i; j < nsrvs; j++) {
            જો (srvs[j].priority != srvs[i].priority) {
                તોડો;
            }

            nw += srvs[j].naddrs * srvs[j].weight;
        }

        જો (nw == 0) {
            જાઓ next_srv;
        }

        w = ngx_random() % nw;

        માટે (k = i; k < j; k++) {
            જો (w < srvs[k].naddrs * srvs[k].weight) {
                તોડો;
            }

            w -= srvs[k].naddrs * srvs[k].weight;
        }

        માટે (l = i; l < j; l++) {

            માટે (m = 0; m < srvs[k].naddrs; m++) {
                addrs[n].socklen = srvs[k].addrs[m].socklen;
                addrs[n].sockaddr = srvs[k].addrs[m].sockaddr;
                addrs[n].name = srvs[k].name;
                addrs[n].priority = srvs[k].priority;
                addrs[n].weight = srvs[k].weight;
                n++;
            }

            જો (++k == j) {
                k = i;
            }
        }

next_srv:

        i = j;

    } જ્યારે (i < ctx->nsrvs);

    ctx->state = NGX_OK;
    ctx->addrs = addrs;
    ctx->naddrs = naddrs;

    ctx->handler(ctx);

    ngx_resolver_free(r, addrs);
}


અક્ષર *
ngx_resolver_strerror(ngx_int_t err)
{
    સ્થિર અક્ષર *errors[] = {
        "Format error",     /* FORMERR */
        "Server failure",   /* SERVFAIL */
        "Host not found",   /* NXDOMAIN */
        "Unimplemented",    /* NOTIMP */
        "Operation refused" /* REFUSED */
    };

    જો (err > 0 && err < 6) {
        પાછા errors[err - 1];
    }

    જો (err == NGX_RESOLVE_TIMEDOUT) {
        પાછા "Operation timed out";
    }

    પાછા "Unknown error";
}


સ્થિર u_char *
ngx_resolver_log_error(ngx_log_t *log, u_char *buf, size_t len)
{
    u_char                     *p;
    ngx_resolver_connection_t  *rec;

    p = buf;

    જો (log->action) {
        p = ngx_snprintf(buf, len, " while %s", log->action);
        len -= p - buf;
    }

    rec = log->data;

    જો (rec) {
        p = ngx_snprintf(p, len, ", resolver: %V", &rec->server);
    }

    પાછા p;
}


સ્થિર ngx_int_t
ngx_udp_connect(ngx_resolver_connection_t *rec)
{
    પૂર્ણાંક                rc;
    ngx_int_t          event;
    ngx_event_t       *rev, *wev;
    ngx_socket_t       s;
    ngx_connection_t  *c;

    s = ngx_socket(rec->sockaddr->sa_family, SOCK_DGRAM, 0);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "UDP socket %d", s);

    જો (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_socket_n " failed");
        પાછા NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->log);

    જો (c == ખાલી) {
        જો (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        પાછા NGX_ERROR;
    }

    જો (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        જાઓ failed;
    }

    rev = c->પઠ;
    wev = c->લિખ;

    rev->log = &rec->log;
    wev->log = &rec->log;

    rec->udp = c;

    c->સંખ્યા = ngx_atomic_fetch_add(ngx_connection_counter, 1);

    c->start_time = ngx_current_msec;

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->log, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->સંખ્યા);

    rc = connect(s, rec->sockaddr, rec->socklen);

    જો (rc == -1) {
        ngx_log_error(NGX_LOG_CRIT, &rec->log, ngx_socket_errno,
                      "connect() failed");

        જાઓ failed;
    }

    /* UDP sockets are always ready to write */
    wev->ready = 1;

    event = (ngx_event_flags & NGX_USE_CLEAR_EVENT) ?
                /* kqueue, epoll */                 NGX_CLEAR_EVENT:
                /* select, poll, /dev/poll */       NGX_LEVEL_EVENT;
                /* eventport event type has no meaning: oneshot only */

    જો (ngx_add_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        જાઓ failed;
    }

    પાછા NGX_OK;

failed:

    ngx_close_connection(c);
    rec->udp = ખાલી;

    પાછા NGX_ERROR;
}


સ્થિર ngx_int_t
ngx_tcp_connect(ngx_resolver_connection_t *rec)
{
    પૂર્ણાંક                rc;
    ngx_int_t          event;
    ngx_err_t          err;
    ngx_uint_t         level;
    ngx_socket_t       s;
    ngx_event_t       *rev, *wev;
    ngx_connection_t  *c;

    s = ngx_socket(rec->sockaddr->sa_family, SOCK_STREAM, 0);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "TCP socket %d", s);

    જો (s == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_socket_n " failed");
        પાછા NGX_ERROR;
    }

    c = ngx_get_connection(s, &rec->log);

    જો (c == ખાલી) {
        જો (ngx_close_socket(s) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        પાછા NGX_ERROR;
    }

    જો (ngx_nonblocking(s) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &rec->log, ngx_socket_errno,
                      ngx_nonblocking_n " failed");

        જાઓ failed;
    }

    rev = c->પઠ;
    wev = c->લિખ;

    rev->log = &rec->log;
    wev->log = &rec->log;

    rec->tcp = c;

    c->સંખ્યા = ngx_atomic_fetch_add(ngx_connection_counter, 1);

    c->start_time = ngx_current_msec;

    જો (ngx_add_conn) {
        જો (ngx_add_conn(c) == NGX_ERROR) {
            જાઓ failed;
        }
    }

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, &rec->log, 0,
                   "connect to %V, fd:%d #%uA", &rec->server, s, c->સંખ્યા);

    rc = connect(s, rec->sockaddr, rec->socklen);

    જો (rc == -1) {
        err = ngx_socket_errno;


        જો (err != NGX_EINPROGRESS
#જો (NGX_WIN32)
            /* Winsock returns WSAEWOULDBLOCK (NGX_EAGAIN) */
            && err != NGX_EAGAIN
#endif
            )
        {
            જો (err == NGX_ECONNREFUSED
#જો (NGX_LINUX)
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

            } નહીંતર {
                level = NGX_LOG_CRIT;
            }

            ngx_log_error(level, &rec->log, err, "connect() to %V failed",
                          &rec->server);

            ngx_close_connection(c);
            rec->tcp = ખાલી;

            પાછા NGX_ERROR;
        }
    }

    જો (ngx_add_conn) {
        જો (rc == -1) {

            /* NGX_EINPROGRESS */

            પાછા NGX_AGAIN;
        }

        ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "connected");

        wev->ready = 1;

        પાછા NGX_OK;
    }

    જો (ngx_event_flags & NGX_USE_CLEAR_EVENT) {

        /* kqueue */

        event = NGX_CLEAR_EVENT;

    } નહીંતર {

        /* select, poll, /dev/poll */

        event = NGX_LEVEL_EVENT;
    }

    જો (ngx_add_event(rev, NGX_READ_EVENT, event) != NGX_OK) {
        જાઓ failed;
    }

    જો (rc == -1) {

        /* NGX_EINPROGRESS */

        જો (ngx_add_event(wev, NGX_WRITE_EVENT, event) != NGX_OK) {
            જાઓ failed;
        }

        પાછા NGX_AGAIN;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_EVENT, &rec->log, 0, "connected");

    wev->ready = 1;

    પાછા NGX_OK;

failed:

    ngx_close_connection(c);
    rec->tcp = ખાલી;

    પાછા NGX_ERROR;
}


સ્થિર ngx_int_t
ngx_resolver_cmp_srvs(અચલ શૂન્ય *one, અચલ શૂન્ય *two)
{
    ngx_int_t            p1, p2;
    ngx_resolver_srv_t  *first, *second;

    first = (ngx_resolver_srv_t *) one;
    second = (ngx_resolver_srv_t *) two;

    p1 = first->priority;
    p2 = second->priority;

    પાછા p1 - p2;
}
