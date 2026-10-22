
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_event.h>


ngx_os_io_t  ngx_io;


ਸਥਿਰ ਖਾਲੀ ngx_drain_connections(ngx_ਚੱਕਰ_t *ਚੱਕਰ);


ngx_listening_t *
ngx_create_listening(ngx_conf_t *cf, ਬਣਤਰ sockaddr *sockaddr,
    socklen_t socklen)
{
    ਮਾਪ_t            len;
    ngx_listening_t  *ls;
    ਬਣਤਰ sockaddr  *sa;
    u_char            ਪਾਠ[NGX_SOCKADDR_STRLEN];

    ls = ngx_array_ਧੱਕੋ(&cf->ਚੱਕਰ->listening);
    ਜੇ (ls == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    ngx_memzero(ls, ਆਕਾਰ(ngx_listening_t));

    sa = ngx_palloc(cf->pool, socklen);
    ਜੇ (sa == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    ngx_memcpy(sa, sockaddr, socklen);

    ls->sockaddr = sa;
    ls->socklen = socklen;

    len = ngx_sock_ntop(sa, socklen, ਪਾਠ, NGX_SOCKADDR_STRLEN, 1);
    ls->addr_ਪਾਠ.len = len;

    ਚੋਣ (ls->sockaddr->sa_family) {
#ਜੇ (NGX_HAVE_INET6)
    ਹਾਲਤ AF_INET6:
        ls->addr_ਪਾਠ_max_len = NGX_INET6_ADDRSTRLEN;
        ਰੋਕੋ;
#ਜੇ_ਅੰਤ
#ਜੇ (NGX_HAVE_UNIX_DOMAIN)
    ਹਾਲਤ AF_UNIX:
        ls->addr_ਪਾਠ_max_len = NGX_UNIX_ADDRSTRLEN;
        len++;
        ਰੋਕੋ;
#ਜੇ_ਅੰਤ
    ਹਾਲਤ AF_INET:
        ls->addr_ਪਾਠ_max_len = NGX_INET_ADDRSTRLEN;
        ਰੋਕੋ;
    ਮੂਲ:
        ls->addr_ਪਾਠ_max_len = NGX_SOCKADDR_STRLEN;
        ਰੋਕੋ;
    }

    ls->addr_ਪਾਠ.ਅੰਕੜੇ = ngx_pnalloc(cf->pool, len);
    ਜੇ (ls->addr_ਪਾਠ.ਅੰਕੜੇ == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    ngx_memcpy(ls->addr_ਪਾਠ.ਅੰਕੜੇ, ਪਾਠ, len);

#ਜੇ !(NGX_WIN32)
    ngx_rbtree_init(&ls->rbtree, &ls->sentinel, ngx_udp_rbtree_ਪਾਓ_ਮੁੱਲ);
#ਜੇ_ਅੰਤ

    ls->fd = (ngx_socket_t) -1;
    ls->type = SOCK_ਧਾਰਾ;

    ls->backlog = NGX_LISTEN_BACKLOG;
    ls->rcvbuf = -1;
    ls->sndbuf = -1;

#ਜੇ (NGX_HAVE_SETFIB)
    ls->setfib = -1;
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HAVE_TCP_FASTOPEN)
    ls->fastopen = -1;
#ਜੇ_ਅੰਤ

    ਵਾਪਸ ls;
}


ngx_int_t
ngx_clone_listening(ngx_ਚੱਕਰ_t *ਚੱਕਰ, ngx_listening_t *ls)
{
#ਜੇ (NGX_HAVE_REUSEPORT)

    ngx_int_t         n;
    ngx_core_conf_t  *ccf;
    ngx_listening_t   ols;

    ਜੇ (!ls->reuseport || ls->ਕਾਮਾ != 0) {
        ਵਾਪਸ NGX_OK;
    }

    ols = *ls;

    ccf = (ngx_core_conf_t *) ngx_get_conf(ਚੱਕਰ->conf_ctx, ngx_core_module);

    ਲਈ (n = 1; n < ccf->ਕਾਮਾ_processes; n++) {

        /* create a socket for each worker process */

        ls = ngx_array_ਧੱਕੋ(&ਚੱਕਰ->listening);
        ਜੇ (ls == ਨਿਰਾ) {
            ਵਾਪਸ NGX_ERROR;
        }

        *ls = ols;
        ls->ਕਾਮਾ = n;
    }

#ਜੇ_ਅੰਤ

    ਵਾਪਸ NGX_OK;
}


ngx_int_t
ngx_ਸੈੱਟ_inherited_sockets(ngx_ਚੱਕਰ_t *ਚੱਕਰ)
{
    ਮਾਪ_t                     len;
    ngx_uint_t                 i;
    ngx_listening_t           *ls;
    socklen_t                  olen;
#ਜੇ (NGX_HAVE_DEFERRED_ACCEPT || NGX_HAVE_TCP_FASTOPEN)
    ngx_err_t                  err;
#ਜੇ_ਅੰਤ
#ਜੇ (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    ਬਣਤਰ accept_filter_arg   af;
#ਜੇ_ਅੰਤ
#ਜੇ (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)
    ਪੂਰਨ                        timeout;
#ਜੇ_ਅੰਤ
#ਜੇ (NGX_HAVE_REUSEPORT)
    ਪੂਰਨ                        reuseport;
#ਜੇ_ਅੰਤ

    ls = ਚੱਕਰ->listening.elts;
    ਲਈ (i = 0; i < ਚੱਕਰ->listening.nelts; i++) {

        ls[i].sockaddr = ngx_palloc(ਚੱਕਰ->pool, ਆਕਾਰ(ngx_sockaddr_t));
        ਜੇ (ls[i].sockaddr == ਨਿਰਾ) {
            ਵਾਪਸ NGX_ERROR;
        }

        ls[i].socklen = ਆਕਾਰ(ngx_sockaddr_t);
        ਜੇ (getsockname(ls[i].fd, ls[i].sockaddr, &ls[i].socklen) == -1) {
            ngx_log_error(NGX_LOG_CRIT, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_socket_errno,
                          "getsockname() of the inherited "
                          "socket #%d failed", ls[i].fd);
            ls[i].ignore = 1;
            ਜਾਰੀ;
        }

        ਜੇ (ls[i].socklen > (socklen_t) ਆਕਾਰ(ngx_sockaddr_t)) {
            ls[i].socklen = ਆਕਾਰ(ngx_sockaddr_t);
        }

        ਚੋਣ (ls[i].sockaddr->sa_family) {

#ਜੇ (NGX_HAVE_INET6)
        ਹਾਲਤ AF_INET6:
            ls[i].addr_ਪਾਠ_max_len = NGX_INET6_ADDRSTRLEN;
            len = NGX_INET6_ADDRSTRLEN + ਆਕਾਰ("[]:65535") - 1;
            ਰੋਕੋ;
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HAVE_UNIX_DOMAIN)
        ਹਾਲਤ AF_UNIX:
            ls[i].addr_ਪਾਠ_max_len = NGX_UNIX_ADDRSTRLEN;
            len = NGX_UNIX_ADDRSTRLEN;
            ਰੋਕੋ;
#ਜੇ_ਅੰਤ

        ਹਾਲਤ AF_INET:
            ls[i].addr_ਪਾਠ_max_len = NGX_INET_ADDRSTRLEN;
            len = NGX_INET_ADDRSTRLEN + ਆਕਾਰ(":65535") - 1;
            ਰੋਕੋ;

        ਮੂਲ:
            ngx_log_error(NGX_LOG_CRIT, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_socket_errno,
                          "the inherited socket #%d has "
                          "an unsupported protocol family", ls[i].fd);
            ls[i].ignore = 1;
            ਜਾਰੀ;
        }

        ls[i].addr_ਪਾਠ.ਅੰਕੜੇ = ngx_pnalloc(ਚੱਕਰ->pool, len);
        ਜੇ (ls[i].addr_ਪਾਠ.ਅੰਕੜੇ == ਨਿਰਾ) {
            ਵਾਪਸ NGX_ERROR;
        }

        len = ngx_sock_ntop(ls[i].sockaddr, ls[i].socklen,
                            ls[i].addr_ਪਾਠ.ਅੰਕੜੇ, len, 1);
        ਜੇ (len == 0) {
            ਵਾਪਸ NGX_ERROR;
        }

        ls[i].addr_ਪਾਠ.len = len;

        ls[i].backlog = NGX_LISTEN_BACKLOG;

        olen = ਆਕਾਰ(ਪੂਰਨ);

        ਜੇ (getsockopt(ls[i].fd, SOL_SOCKET, SO_TYPE, (ਖਾਲੀ *) &ls[i].type,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_CRIT, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_socket_errno,
                          "getsockopt(SO_TYPE) %V failed", &ls[i].addr_ਪਾਠ);
            ls[i].ignore = 1;
            ਜਾਰੀ;
        }

        olen = ਆਕਾਰ(ਪੂਰਨ);

        ਜੇ (getsockopt(ls[i].fd, SOL_SOCKET, SO_RCVBUF, (ਖਾਲੀ *) &ls[i].rcvbuf,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_socket_errno,
                          "getsockopt(SO_RCVBUF) %V failed, ignored",
                          &ls[i].addr_ਪਾਠ);

            ls[i].rcvbuf = -1;
        }

        olen = ਆਕਾਰ(ਪੂਰਨ);

        ਜੇ (getsockopt(ls[i].fd, SOL_SOCKET, SO_SNDBUF, (ਖਾਲੀ *) &ls[i].sndbuf,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_socket_errno,
                          "getsockopt(SO_SNDBUF) %V failed, ignored",
                          &ls[i].addr_ਪਾਠ);

            ls[i].sndbuf = -1;
        }

#ਜੇ 0
        /* SO_SETFIB is currently a set only option */

#ਜੇ (NGX_HAVE_SETFIB)

        olen = ਆਕਾਰ(ਪੂਰਨ);

        ਜੇ (getsockopt(ls[i].fd, SOL_SOCKET, SO_SETFIB,
                       (ਖਾਲੀ *) &ls[i].setfib, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_socket_errno,
                          "getsockopt(SO_SETFIB) %V failed, ignored",
                          &ls[i].addr_ਪਾਠ);

            ls[i].setfib = -1;
        }

#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HAVE_REUSEPORT)

        reuseport = 0;
        olen = ਆਕਾਰ(ਪੂਰਨ);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ SO_REUSEPORT_LB

        ਜੇ (getsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT_LB,
                       (ਖਾਲੀ *) &reuseport, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_socket_errno,
                          "getsockopt(SO_REUSEPORT_LB) %V failed, ignored",
                          &ls[i].addr_ਪਾਠ);

        } ਨਹੀਂਤਾਂ {
            ls[i].reuseport = reuseport ? 1 : 0;
        }

#ਨਹੀਂਤਾਂ

        ਜੇ (getsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT,
                       (ਖਾਲੀ *) &reuseport, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_socket_errno,
                          "getsockopt(SO_REUSEPORT) %V failed, ignored",
                          &ls[i].addr_ਪਾਠ);

        } ਨਹੀਂਤਾਂ {
            ls[i].reuseport = reuseport ? 1 : 0;
        }
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ

        ਜੇ (ls[i].type != SOCK_ਧਾਰਾ) {
            ਜਾਰੀ;
        }

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ SO_PROTOCOL

        olen = ਆਕਾਰ(ਪੂਰਨ);

        ਜੇ (getsockopt(ls[i].fd, SOL_SOCKET, SO_PROTOCOL,
                       (ਖਾਲੀ *) &ls[i].protocol, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_socket_errno,
                          "getsockopt(SO_PROTOCOL) %V failed, ignored",
                          &ls[i].addr_ਪਾਠ);
            ls[i].protocol = 0;

        } ਨਹੀਂਤਾਂ ਜੇ (ls[i].protocol == IPPROTO_TCP) {
            ls[i].protocol = 0;
        }

#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HAVE_TCP_FASTOPEN)

        olen = ਆਕਾਰ(ਪੂਰਨ);

        ਜੇ (getsockopt(ls[i].fd, IPPROTO_TCP, TCP_FASTOPEN,
                       (ਖਾਲੀ *) &ls[i].fastopen, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            ਜੇ (err != NGX_EOPNOTSUPP && err != NGX_ENOPROTOOPT
                && err != NGX_EINVAL)
            {
                ngx_log_error(NGX_LOG_NOTICE, ਚੱਕਰ->ਲਘੂਗਣਕ, err,
                              "getsockopt(TCP_FASTOPEN) %V failed, ignored",
                              &ls[i].addr_ਪਾਠ);
            }

            ls[i].fastopen = -1;
        }

#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)

        ngx_memzero(&af, ਆਕਾਰ(ਬਣਤਰ accept_filter_arg));
        olen = ਆਕਾਰ(ਬਣਤਰ accept_filter_arg);

        ਜੇ (getsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER, &af, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            ਜੇ (err == NGX_EINVAL) {
                ਜਾਰੀ;
            }

            ngx_log_error(NGX_LOG_NOTICE, ਚੱਕਰ->ਲਘੂਗਣਕ, err,
                          "getsockopt(SO_ACCEPTFILTER) for %V failed, ignored",
                          &ls[i].addr_ਪਾਠ);
            ਜਾਰੀ;
        }

        ਜੇ (olen < ਆਕਾਰ(ਬਣਤਰ accept_filter_arg) || af.af_name[0] == '\0') {
            ਜਾਰੀ;
        }

        ls[i].accept_filter = ngx_palloc(ਚੱਕਰ->pool, 16);
        ਜੇ (ls[i].accept_filter == ਨਿਰਾ) {
            ਵਾਪਸ NGX_ERROR;
        }

        (ਖਾਲੀ) ngx_cpystrn((u_char *) ls[i].accept_filter,
                           (u_char *) af.af_name, 16);
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)

        timeout = 0;
        olen = ਆਕਾਰ(ਪੂਰਨ);

        ਜੇ (getsockopt(ls[i].fd, IPPROTO_TCP, TCP_DEFER_ACCEPT, &timeout, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            ਜੇ (err == NGX_EOPNOTSUPP) {
                ਜਾਰੀ;
            }

            ngx_log_error(NGX_LOG_NOTICE, ਚੱਕਰ->ਲਘੂਗਣਕ, err,
                          "getsockopt(TCP_DEFER_ACCEPT) for %V failed, ignored",
                          &ls[i].addr_ਪਾਠ);
            ਜਾਰੀ;
        }

        ਜੇ (olen < ਆਕਾਰ(ਪੂਰਨ) || timeout == 0) {
            ਜਾਰੀ;
        }

        ls[i].deferred_accept = 1;
#ਜੇ_ਅੰਤ
    }

    ਵਾਪਸ NGX_OK;
}


ngx_int_t
ngx_open_listening_sockets(ngx_ਚੱਕਰ_t *ਚੱਕਰ)
{
    ਪੂਰਨ               reuseaddr;
    ngx_uint_t        i, tries, failed;
    ngx_err_t         err;
    ngx_log_t        *ਲਘੂਗਣਕ;
    ngx_socket_t      s;
    ngx_listening_t  *ls;

    reuseaddr = 1;
#ਜੇ (NGX_SUPPRESS_WARN)
    failed = 0;
#ਜੇ_ਅੰਤ

    ਲਘੂਗਣਕ = ਚੱਕਰ->ਲਘੂਗਣਕ;

    /* TODO: configurable try number */

    ਲਈ (tries = 5; tries; tries--) {
        failed = 0;

        /* for each listening socket */

        ls = ਚੱਕਰ->listening.elts;
        ਲਈ (i = 0; i < ਚੱਕਰ->listening.nelts; i++) {

            ਜੇ (ls[i].ignore) {
                ਜਾਰੀ;
            }

#ਜੇ (NGX_HAVE_REUSEPORT)

            ਜੇ (ls[i].ਜੋੜੋ_reuseport || ls[i].change_protocol) {

                /*
                 * to allow transition from a socket without SO_REUSEPORT
                 * to multiple sockets with SO_REUSEPORT, we have to set
                 * SO_REUSEPORT on the old socket before opening new ones
                 *
                 * to allow transition between different socket protocols
                 * (e.g. IPPROTO_MPTCP), SO_REUSEPORT is set on both old
                 * and new sockets
                 */

                ਪੂਰਨ  reuseport = 1;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ SO_REUSEPORT_LB

                ਜੇ (setsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT_LB,
                               (ਅਚਲ ਖਾਲੀ *) &reuseport, ਆਕਾਰ(ਪੂਰਨ))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_ALERT, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT_LB) %V failed, "
                                  "ignored",
                                  &ls[i].addr_ਪਾਠ);
                }

#ਨਹੀਂਤਾਂ

                ਜੇ (setsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT,
                               (ਅਚਲ ਖਾਲੀ *) &reuseport, ਆਕਾਰ(ਪੂਰਨ))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_ALERT, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT) %V failed, ignored",
                                  &ls[i].addr_ਪਾਠ);
                }
#ਜੇ_ਅੰਤ

                ls[i].ਜੋੜੋ_reuseport = 0;
            }
#ਜੇ_ਅੰਤ

            ਜੇ (ls[i].fd != (ngx_socket_t) -1 && !ls[i].change_protocol) {
                ਜਾਰੀ;
            }

            ਜੇ (ls[i].inherited) {

                /* TODO: close on exit */
                /* TODO: nonblocking */
                /* TODO: deferred accept */

                ਜਾਰੀ;
            }

            s = ngx_socket(ls[i].sockaddr->sa_family, ls[i].type,
                           ls[i].protocol);

            ਜੇ (s == (ngx_socket_t) -1) {
                ngx_log_error(NGX_LOG_EMERG, ਲਘੂਗਣਕ, ngx_socket_errno,
                              ngx_socket_n " %V failed", &ls[i].addr_ਪਾਠ);
                ਵਾਪਸ NGX_ERROR;
            }

            ਜੇ (ls[i].type != SOCK_DGRAM || !ngx_test_config) {

                ਜੇ (setsockopt(s, SOL_SOCKET, SO_REUSEADDR,
                               (ਅਚਲ ਖਾਲੀ *) &reuseaddr, ਆਕਾਰ(ਪੂਰਨ))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, ਲਘੂਗਣਕ, ngx_socket_errno,
                                  "setsockopt(SO_REUSEADDR) %V failed",
                                  &ls[i].addr_ਪਾਠ);

                    ਜੇ (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, ਲਘੂਗਣਕ, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_ਪਾਠ);
                    }

                    ਵਾਪਸ NGX_ERROR;
                }
            }

#ਜੇ (NGX_HAVE_REUSEPORT)

            ਜੇ ((ls[i].reuseport || ls[i].change_protocol)
                && !ngx_test_config)
            {
                ਪੂਰਨ  reuseport;

                reuseport = 1;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ SO_REUSEPORT_LB

                ਜੇ (setsockopt(s, SOL_SOCKET, SO_REUSEPORT_LB,
                               (ਅਚਲ ਖਾਲੀ *) &reuseport, ਆਕਾਰ(ਪੂਰਨ))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, ਲਘੂਗਣਕ, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT_LB) %V failed",
                                  &ls[i].addr_ਪਾਠ);

                    ਜੇ (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, ਲਘੂਗਣਕ, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_ਪਾਠ);
                    }

                    ਵਾਪਸ NGX_ERROR;
                }

#ਨਹੀਂਤਾਂ

                ਜੇ (setsockopt(s, SOL_SOCKET, SO_REUSEPORT,
                               (ਅਚਲ ਖਾਲੀ *) &reuseport, ਆਕਾਰ(ਪੂਰਨ))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, ਲਘੂਗਣਕ, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT) %V failed",
                                  &ls[i].addr_ਪਾਠ);

                    ਜੇ (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, ਲਘੂਗਣਕ, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_ਪਾਠ);
                    }

                    ਵਾਪਸ NGX_ERROR;
                }
#ਜੇ_ਅੰਤ
            }
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HAVE_INET6 && defined IPV6_V6ONLY)

            ਜੇ (ls[i].sockaddr->sa_family == AF_INET6) {
                ਪੂਰਨ  ipv6only;

                ipv6only = ls[i].ipv6only;

                ਜੇ (setsockopt(s, IPPROTO_IPV6, IPV6_V6ONLY,
                               (ਅਚਲ ਖਾਲੀ *) &ipv6only, ਆਕਾਰ(ਪੂਰਨ))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, ਲਘੂਗਣਕ, ngx_socket_errno,
                                  "setsockopt(IPV6_V6ONLY) %V failed, ignored",
                                  &ls[i].addr_ਪਾਠ);
                }
            }
#ਜੇ_ਅੰਤ
            /* TODO: close on exit */

            ਜੇ (ngx_nonblocking(s) == -1) {
                ngx_log_error(NGX_LOG_EMERG, ਲਘੂਗਣਕ, ngx_socket_errno,
                              ngx_nonblocking_n " %V failed",
                              &ls[i].addr_ਪਾਠ);

                ਜੇ (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, ਲਘੂਗਣਕ, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_ਪਾਠ);
                }

                ਵਾਪਸ NGX_ERROR;
            }

            ngx_log_debug2(NGX_LOG_DEBUG_CORE, ਲਘੂਗਣਕ, 0,
                           "bind() %V #%d ", &ls[i].addr_ਪਾਠ, s);

            ਜੇ (bind(s, ls[i].sockaddr, ls[i].socklen) == -1) {
                err = ngx_socket_errno;

                ਜੇ (err != NGX_EADDRINUSE || !ngx_test_config) {
                    ngx_log_error(NGX_LOG_EMERG, ਲਘੂਗਣਕ, err,
                                  "bind() to %V failed", &ls[i].addr_ਪਾਠ);
                }

                ਜੇ (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, ਲਘੂਗਣਕ, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_ਪਾਠ);
                }

                ਜੇ (err != NGX_EADDRINUSE) {
                    ਵਾਪਸ NGX_ERROR;
                }

                ਜੇ (!ngx_test_config) {
                    failed = 1;
                }

                ਜਾਰੀ;
            }

#ਜੇ (NGX_HAVE_UNIX_DOMAIN)

            ਜੇ (ls[i].sockaddr->sa_family == AF_UNIX) {
                ਬਹੁਲਕ_t   ਬਹੁਲਕ;
                u_char  *name;

                name = ls[i].addr_ਪਾਠ.ਅੰਕੜੇ + ਆਕਾਰ("unix:") - 1;
                ਬਹੁਲਕ = (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);

                ਜੇ (chmod((ਅੱਖਰ *) name, ਬਹੁਲਕ) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_errno,
                                  "chmod() \"%s\" failed", name);
                }

                ਜੇ (ngx_test_config) {
                    ਜੇ (ngx_delete_file(name) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_errno,
                                      ngx_delete_file_n " %s failed", name);
                    }
                }
            }
#ਜੇ_ਅੰਤ

            ਜੇ (ls[i].type != SOCK_ਧਾਰਾ) {
                ls[i].fd = s;
                ls[i].open = 1;
                ਜਾਰੀ;
            }

            ਜੇ (listen(s, ls[i].backlog) == -1) {
                err = ngx_socket_errno;

                /*
                 * on OpenVZ after suspend/resume EADDRINUSE
                 * may be returned by listen() instead of bind(), see
                 * https://bugs.openvz.org/browse/OVZ-5587
                 */

                ਜੇ (err != NGX_EADDRINUSE || !ngx_test_config) {
                    ngx_log_error(NGX_LOG_EMERG, ਲਘੂਗਣਕ, err,
                                  "listen() to %V, backlog %d failed",
                                  &ls[i].addr_ਪਾਠ, ls[i].backlog);
                }

                ਜੇ (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, ਲਘੂਗਣਕ, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_ਪਾਠ);
                }

                ਜੇ (err != NGX_EADDRINUSE) {
                    ਵਾਪਸ NGX_ERROR;
                }

                ਜੇ (!ngx_test_config) {
                    failed = 1;
                }

                ਜਾਰੀ;
            }

            ls[i].listen = 1;

            ls[i].fd = s;
            ls[i].open = 1;
        }

        ਜੇ (!failed) {
            ਰੋਕੋ;
        }

        /* TODO: delay configurable */

        ngx_log_error(NGX_LOG_NOTICE, ਲਘੂਗਣਕ, 0,
                      "try again to bind() after 500ms");

        ngx_msleep(500);
    }

    ਜੇ (failed) {
        ngx_log_error(NGX_LOG_EMERG, ਲਘੂਗਣਕ, 0, "still could not bind()");
        ਵਾਪਸ NGX_ERROR;
    }

    ਵਾਪਸ NGX_OK;
}


ਖਾਲੀ
ngx_configure_listening_sockets(ngx_ਚੱਕਰ_t *ਚੱਕਰ)
{
    ਪੂਰਨ                        ਮੁੱਲ;
    ngx_uint_t                 i;
    ngx_listening_t           *ls;

#ਜੇ (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    ਬਣਤਰ accept_filter_arg   af;
#ਜੇ_ਅੰਤ

    ls = ਚੱਕਰ->listening.elts;
    ਲਈ (i = 0; i < ਚੱਕਰ->listening.nelts; i++) {

        ls[i].ਲਘੂਗਣਕ = *ls[i].logp;

        ਜੇ (ls[i].rcvbuf != -1) {
            ਜੇ (setsockopt(ls[i].fd, SOL_SOCKET, SO_RCVBUF,
                           (ਅਚਲ ਖਾਲੀ *) &ls[i].rcvbuf, ਆਕਾਰ(ਪੂਰਨ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_socket_errno,
                              "setsockopt(SO_RCVBUF, %d) %V failed, ignored",
                              ls[i].rcvbuf, &ls[i].addr_ਪਾਠ);
            }
        }

        ਜੇ (ls[i].sndbuf != -1) {
            ਜੇ (setsockopt(ls[i].fd, SOL_SOCKET, SO_SNDBUF,
                           (ਅਚਲ ਖਾਲੀ *) &ls[i].sndbuf, ਆਕਾਰ(ਪੂਰਨ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_socket_errno,
                              "setsockopt(SO_SNDBUF, %d) %V failed, ignored",
                              ls[i].sndbuf, &ls[i].addr_ਪਾਠ);
            }
        }

        ਜੇ (ls[i].keepalive) {
            ਮੁੱਲ = (ls[i].keepalive == 1) ? 1 : 0;

            ਜੇ (setsockopt(ls[i].fd, SOL_SOCKET, SO_KEEPALIVE,
                           (ਅਚਲ ਖਾਲੀ *) &ਮੁੱਲ, ਆਕਾਰ(ਪੂਰਨ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_socket_errno,
                              "setsockopt(SO_KEEPALIVE, %d) %V failed, ignored",
                              ਮੁੱਲ, &ls[i].addr_ਪਾਠ);
            }
        }

#ਜੇ (NGX_HAVE_KEEPALIVE_TUNABLE)

#ਜੇ !(NGX_DARWIN)

        ਜੇ (ls[i].keepidle) {
            ਮੁੱਲ = ls[i].keepidle;

#ਜੇ (NGX_KEEPALIVE_ਗੁਣਕ)
            ਮੁੱਲ *= NGX_KEEPALIVE_ਗੁਣਕ;
#ਜੇ_ਅੰਤ

            ਜੇ (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPIDLE,
                           (ਅਚਲ ਖਾਲੀ *) &ਮੁੱਲ, ਆਕਾਰ(ਪੂਰਨ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_socket_errno,
                              "setsockopt(TCP_KEEPIDLE, %d) %V failed, ignored",
                              ਮੁੱਲ, &ls[i].addr_ਪਾਠ);
            }
        }

#ਜੇ_ਅੰਤ

        ਜੇ (ls[i].keepintvl) {
            ਮੁੱਲ = ls[i].keepintvl;

#ਜੇ (NGX_KEEPALIVE_ਗੁਣਕ)
            ਮੁੱਲ *= NGX_KEEPALIVE_ਗੁਣਕ;
#ਜੇ_ਅੰਤ

            ਜੇ (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPINTVL,
                           (ਅਚਲ ਖਾਲੀ *) &ਮੁੱਲ, ਆਕਾਰ(ਪੂਰਨ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_socket_errno,
                             "setsockopt(TCP_KEEPINTVL, %d) %V failed, ignored",
                             ਮੁੱਲ, &ls[i].addr_ਪਾਠ);
            }
        }

        ਜੇ (ls[i].keepcnt) {
            ਜੇ (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPCNT,
                           (ਅਚਲ ਖਾਲੀ *) &ls[i].keepcnt, ਆਕਾਰ(ਪੂਰਨ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_socket_errno,
                              "setsockopt(TCP_KEEPCNT, %d) %V failed, ignored",
                              ls[i].keepcnt, &ls[i].addr_ਪਾਠ);
            }
        }

#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HAVE_SETFIB)
        ਜੇ (ls[i].setfib != -1) {
            ਜੇ (setsockopt(ls[i].fd, SOL_SOCKET, SO_SETFIB,
                           (ਅਚਲ ਖਾਲੀ *) &ls[i].setfib, ਆਕਾਰ(ਪੂਰਨ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_socket_errno,
                              "setsockopt(SO_SETFIB, %d) %V failed, ignored",
                              ls[i].setfib, &ls[i].addr_ਪਾਠ);
            }
        }
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HAVE_TCP_FASTOPEN)
        ਜੇ (ls[i].fastopen != -1) {
            ਜੇ (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_FASTOPEN,
                           (ਅਚਲ ਖਾਲੀ *) &ls[i].fastopen, ਆਕਾਰ(ਪੂਰਨ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_socket_errno,
                              "setsockopt(TCP_FASTOPEN, %d) %V failed, ignored",
                              ls[i].fastopen, &ls[i].addr_ਪਾਠ);
            }
        }
#ਜੇ_ਅੰਤ

#ਜੇ 0
        ਜੇ (1) {
            ਪੂਰਨ tcp_nodelay = 1;

            ਜੇ (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_NODELAY,
                       (ਅਚਲ ਖਾਲੀ *) &tcp_nodelay, ਆਕਾਰ(ਪੂਰਨ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_socket_errno,
                              "setsockopt(TCP_NODELAY) %V failed, ignored",
                              &ls[i].addr_ਪਾਠ);
            }
        }
#ਜੇ_ਅੰਤ

        ਜੇ (ls[i].listen) {

            /* change backlog via listen() */

            ਜੇ (listen(ls[i].fd, ls[i].backlog) == -1) {
                ngx_log_error(NGX_LOG_ALERT, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_socket_errno,
                              "listen() to %V, backlog %d failed, ignored",
                              &ls[i].addr_ਪਾਠ, ls[i].backlog);
            }
        }

        /*
         * setting deferred mode should be last operation on socket,
         * because code may prematurely continue cycle on failure
         */

#ਜੇ (NGX_HAVE_DEFERRED_ACCEPT)

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ SO_ACCEPTFILTER

        ਜੇ (ls[i].delete_deferred) {
            ਜੇ (setsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER, ਨਿਰਾ, 0)
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_socket_errno,
                              "setsockopt(SO_ACCEPTFILTER, NULL) "
                              "for %V failed, ignored",
                              &ls[i].addr_ਪਾਠ);

                ਜੇ (ls[i].accept_filter) {
                    ngx_log_error(NGX_LOG_ALERT, ਚੱਕਰ->ਲਘੂਗਣਕ, 0,
                                  "could not change the accept filter "
                                  "to \"%s\" for %V, ignored",
                                  ls[i].accept_filter, &ls[i].addr_ਪਾਠ);
                }

                ਜਾਰੀ;
            }

            ls[i].deferred_accept = 0;
        }

        ਜੇ (ls[i].ਜੋੜੋ_deferred) {
            ngx_memzero(&af, ਆਕਾਰ(ਬਣਤਰ accept_filter_arg));
            (ਖਾਲੀ) ngx_cpystrn((u_char *) af.af_name,
                               (u_char *) ls[i].accept_filter, 16);

            ਜੇ (setsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER,
                           &af, ਆਕਾਰ(ਬਣਤਰ accept_filter_arg))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_socket_errno,
                              "setsockopt(SO_ACCEPTFILTER, \"%s\") "
                              "for %V failed, ignored",
                              ls[i].accept_filter, &ls[i].addr_ਪਾਠ);
                ਜਾਰੀ;
            }

            ls[i].deferred_accept = 1;
        }

#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ TCP_DEFER_ACCEPT

        ਜੇ (ls[i].ਜੋੜੋ_deferred || ls[i].delete_deferred) {

            ਜੇ (ls[i].ਜੋੜੋ_deferred) {
                /*
                 * There is no way to find out how long a connection was
                 * in queue (and a connection may bypass deferred queue at all
                 * if syncookies were used), hence we use 1 second timeout
                 * here.
                 */
                ਮੁੱਲ = 1;

            } ਨਹੀਂਤਾਂ {
                ਮੁੱਲ = 0;
            }

            ਜੇ (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_DEFER_ACCEPT,
                           &ਮੁੱਲ, ਆਕਾਰ(ਪੂਰਨ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_socket_errno,
                              "setsockopt(TCP_DEFER_ACCEPT, %d) for %V failed, "
                              "ignored",
                              ਮੁੱਲ, &ls[i].addr_ਪਾਠ);

                ਜਾਰੀ;
            }
        }

        ਜੇ (ls[i].ਜੋੜੋ_deferred) {
            ls[i].deferred_accept = 1;
        }

#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ /* NGX_HAVE_DEFERRED_ACCEPT */

#ਜੇ (NGX_HAVE_IP_RECVDSTADDR)

        ਜੇ (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET)
        {
            ਮੁੱਲ = 1;

            ਜੇ (setsockopt(ls[i].fd, IPPROTO_IP, IP_RECVDSTADDR,
                           (ਅਚਲ ਖਾਲੀ *) &ਮੁੱਲ, ਆਕਾਰ(ਪੂਰਨ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_socket_errno,
                              "setsockopt(IP_RECVDSTADDR) "
                              "for %V failed, ignored",
                              &ls[i].addr_ਪਾਠ);
            }
        }

#ਜਾਂਜੇ (NGX_HAVE_IP_PKTINFO)

        ਜੇ (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET)
        {
            ਮੁੱਲ = 1;

            ਜੇ (setsockopt(ls[i].fd, IPPROTO_IP, IP_PKTINFO,
                           (ਅਚਲ ਖਾਲੀ *) &ਮੁੱਲ, ਆਕਾਰ(ਪੂਰਨ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_socket_errno,
                              "setsockopt(IP_PKTINFO) "
                              "for %V failed, ignored",
                              &ls[i].addr_ਪਾਠ);
            }
        }

#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HAVE_INET6 && NGX_HAVE_IPV6_RECVPKTINFO)

        ਜੇ (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET6)
        {
            ਮੁੱਲ = 1;

            ਜੇ (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_RECVPKTINFO,
                           (ਅਚਲ ਖਾਲੀ *) &ਮੁੱਲ, ਆਕਾਰ(ਪੂਰਨ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_socket_errno,
                              "setsockopt(IPV6_RECVPKTINFO) "
                              "for %V failed, ignored",
                              &ls[i].addr_ਪਾਠ);
            }
        }

#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HAVE_IP_MTU_DISCOVER)

        ਜੇ (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET) {
            ਮੁੱਲ = IP_PMTUDISC_DO;

            ਜੇ (setsockopt(ls[i].fd, IPPROTO_IP, IP_MTU_DISCOVER,
                           (ਅਚਲ ਖਾਲੀ *) &ਮੁੱਲ, ਆਕਾਰ(ਪੂਰਨ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_socket_errno,
                              "setsockopt(IP_MTU_DISCOVER) "
                              "for %V failed, ignored",
                              &ls[i].addr_ਪਾਠ);
            }
        }

#ਜਾਂਜੇ (NGX_HAVE_IP_DONTFRAG)

        ਜੇ (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET) {
            ਮੁੱਲ = 1;

            ਜੇ (setsockopt(ls[i].fd, IPPROTO_IP, IP_DONTFRAG,
                           (ਅਚਲ ਖਾਲੀ *) &ਮੁੱਲ, ਆਕਾਰ(ਪੂਰਨ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_socket_errno,
                              "setsockopt(IP_DONTFRAG) "
                              "for %V failed, ignored",
                              &ls[i].addr_ਪਾਠ);
            }
        }

#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HAVE_INET6)

#ਜੇ (NGX_HAVE_IPV6_MTU_DISCOVER)

        ਜੇ (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET6) {
            ਮੁੱਲ = IPV6_PMTUDISC_DO;

            ਜੇ (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_MTU_DISCOVER,
                           (ਅਚਲ ਖਾਲੀ *) &ਮੁੱਲ, ਆਕਾਰ(ਪੂਰਨ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_socket_errno,
                              "setsockopt(IPV6_MTU_DISCOVER) "
                              "for %V failed, ignored",
                              &ls[i].addr_ਪਾਠ);
            }
        }

#ਜਾਂਜੇ (NGX_HAVE_IPV6_DONTFRAG)

        ਜੇ (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET6) {
            ਮੁੱਲ = 1;

            ਜੇ (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_DONTFRAG,
                           (ਅਚਲ ਖਾਲੀ *) &ਮੁੱਲ, ਆਕਾਰ(ਪੂਰਨ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_socket_errno,
                              "setsockopt(IPV6_DONTFRAG) "
                              "for %V failed, ignored",
                              &ls[i].addr_ਪਾਠ);
            }
        }

#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ
    }

    ਵਾਪਸ;
}


ਖਾਲੀ
ngx_close_listening_sockets(ngx_ਚੱਕਰ_t *ਚੱਕਰ)
{
    ngx_uint_t         i;
    ngx_listening_t   *ls;
    ngx_connection_t  *c;

    ngx_accept_mutex_held = 0;
    ngx_use_accept_mutex = 0;

    ls = ਚੱਕਰ->listening.elts;
    ਲਈ (i = 0; i < ਚੱਕਰ->listening.nelts; i++) {

#ਜੇ (NGX_QUIC)
        ਜੇ (ls[i].quic
            && (!ls[i].reuseport || !ngx_quic_bpf_enabled(ਚੱਕਰ)))
        {
            ਜਾਰੀ;
        }
#ਜੇ_ਅੰਤ

        c = ls[i].connection;

        ਜੇ (c) {
            ਜੇ (c->ਪਠ->ਸਰਗਰਮ) {
                ਜੇ (ngx_event_flags & NGX_USE_EPOLL_EVENT) {

                    /*
                     * it seems that Linux-2.6.x OpenVZ sends events
                     * for closed shared listening sockets unless
                     * the events was explicitly deleted
                     */

                    ngx_del_event(c->ਪਠ, NGX_READ_EVENT, 0);

                } ਨਹੀਂਤਾਂ {
                    ngx_del_event(c->ਪਠ, NGX_READ_EVENT, NGX_CLOSE_EVENT);
                }
            }

            ਜੇ (c->ਪਠ->timer_ਸੈੱਟ) {
                ngx_del_timer(c->ਪਠ);
            }

            ngx_free_connection(c);

            c->fd = (ngx_socket_t) -1;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, ਚੱਕਰ->ਲਘੂਗਣਕ, 0,
                       "close listening %V #%d ", &ls[i].addr_ਪਾਠ, ls[i].fd);

        ਜੇ (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_socket_errno,
                          ngx_close_socket_n " %V failed", &ls[i].addr_ਪਾਠ);
        }

#ਜੇ (NGX_HAVE_UNIX_DOMAIN)

        ਜੇ (ls[i].sockaddr->sa_family == AF_UNIX
            && ngx_process <= NGX_PROCESS_MASTER
            && ngx_new_ਦੁਆਧਾਰੀ == 0
            && (!ls[i].inherited || ngx_getppid() != ngx_ਮਾਪਾ))
        {
            u_char *name = ls[i].addr_ਪਾਠ.ਅੰਕੜੇ + ਆਕਾਰ("unix:") - 1;

            ਜੇ (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_socket_errno,
                              ngx_delete_file_n " %s failed", name);
            }
        }

#ਜੇ_ਅੰਤ

        ls[i].fd = (ngx_socket_t) -1;
    }

    ਚੱਕਰ->listening.nelts = 0;
}


ngx_connection_t *
ngx_get_connection(ngx_socket_t s, ngx_log_t *ਲਘੂਗਣਕ)
{
    ngx_uint_t         instance;
    ngx_event_t       *rev, *wev;
    ngx_connection_t  *c;

    /* disable warning: Win32 SOCKET is u_int while UNIX socket is int */

    ਜੇ (ngx_ਚੱਕਰ->files && (ngx_uint_t) s >= ngx_ਚੱਕਰ->files_n) {
        ngx_log_error(NGX_LOG_ALERT, ਲਘੂਗਣਕ, 0,
                      "the new socket has number %d, "
                      "but only %ui files are available",
                      s, ngx_ਚੱਕਰ->files_n);
        ਵਾਪਸ ਨਿਰਾ;
    }

    ngx_drain_connections((ngx_ਚੱਕਰ_t *) ngx_ਚੱਕਰ);

    c = ngx_ਚੱਕਰ->free_connections;

    ਜੇ (c == ਨਿਰਾ) {
        ngx_log_error(NGX_LOG_ALERT, ਲਘੂਗਣਕ, 0,
                      "%ui worker_connections are not enough",
                      ngx_ਚੱਕਰ->connection_n);

        ਵਾਪਸ ਨਿਰਾ;
    }

    ngx_ਚੱਕਰ->free_connections = c->ਅੰਕੜੇ;
    ngx_ਚੱਕਰ->free_connection_n--;

    ਜੇ (ngx_ਚੱਕਰ->files && ngx_ਚੱਕਰ->files[s] == ਨਿਰਾ) {
        ngx_ਚੱਕਰ->files[s] = c;
    }

    rev = c->ਪਠ;
    wev = c->ਲਿਖ;

    ngx_memzero(c, ਆਕਾਰ(ngx_connection_t));

    c->ਪਠ = rev;
    c->ਲਿਖ = wev;
    c->fd = s;
    c->ਲਘੂਗਣਕ = ਲਘੂਗਣਕ;

    instance = rev->instance;

    ngx_memzero(rev, ਆਕਾਰ(ngx_event_t));
    ngx_memzero(wev, ਆਕਾਰ(ngx_event_t));

    rev->instance = !instance;
    wev->instance = !instance;

    rev->ਸੂਚਕਾਂਕ = NGX_INVALID_ਸੂਚਕਾਂਕ;
    wev->ਸੂਚਕਾਂਕ = NGX_INVALID_ਸੂਚਕਾਂਕ;

    rev->ਅੰਕੜੇ = c;
    wev->ਅੰਕੜੇ = c;

    wev->ਲਿਖ = 1;

    ਵਾਪਸ c;
}


ਖਾਲੀ
ngx_free_connection(ngx_connection_t *c)
{
    c->ਅੰਕੜੇ = ngx_ਚੱਕਰ->free_connections;
    ngx_ਚੱਕਰ->free_connections = c;
    ngx_ਚੱਕਰ->free_connection_n++;

    ਜੇ (ngx_ਚੱਕਰ->files && ngx_ਚੱਕਰ->files[c->fd] == c) {
        ngx_ਚੱਕਰ->files[c->fd] = ਨਿਰਾ;
    }
}


ਖਾਲੀ
ngx_close_connection(ngx_connection_t *c)
{
    ngx_err_t     err;
    ngx_uint_t    log_error, level;
    ngx_socket_t  fd;

    ਜੇ (c->fd == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, c->ਲਘੂਗਣਕ, 0, "connection already closed");
        ਵਾਪਸ;
    }

    ਜੇ (c->ਪਠ->timer_ਸੈੱਟ) {
        ngx_del_timer(c->ਪਠ);
    }

    ਜੇ (c->ਲਿਖ->timer_ਸੈੱਟ) {
        ngx_del_timer(c->ਲਿਖ);
    }

    ਜੇ (!c->shared) {
        ਜੇ (ngx_del_conn) {
            ngx_del_conn(c, NGX_CLOSE_EVENT);

        } ਨਹੀਂਤਾਂ {
            ਜੇ (c->ਪਠ->ਸਰਗਰਮ || c->ਪਠ->disabled) {
                ngx_del_event(c->ਪਠ, NGX_READ_EVENT, NGX_CLOSE_EVENT);
            }

            ਜੇ (c->ਲਿਖ->ਸਰਗਰਮ || c->ਲਿਖ->disabled) {
                ngx_del_event(c->ਲਿਖ, NGX_WRITE_EVENT, NGX_CLOSE_EVENT);
            }
        }
    }

    ਜੇ (c->ਪਠ->posted) {
        ngx_delete_posted_event(c->ਪਠ);
    }

    ਜੇ (c->ਲਿਖ->posted) {
        ngx_delete_posted_event(c->ਲਿਖ);
    }

    c->ਪਠ->closed = 1;
    c->ਲਿਖ->closed = 1;

    ngx_reusable_connection(c, 0);

    log_error = c->log_error;

    ngx_free_connection(c);

    fd = c->fd;
    c->fd = (ngx_socket_t) -1;

    ਜੇ (c->shared) {
        ਵਾਪਸ;
    }

    ਜੇ (ngx_close_socket(fd) == -1) {

        err = ngx_socket_errno;

        ਜੇ (err == NGX_ECONNRESET || err == NGX_ENOTCONN) {

            ਚੋਣ (log_error) {

            ਹਾਲਤ NGX_ERROR_INFO:
                level = NGX_LOG_INFO;
                ਰੋਕੋ;

            ਹਾਲਤ NGX_ERROR_ERR:
                level = NGX_LOG_ERR;
                ਰੋਕੋ;

            ਮੂਲ:
                level = NGX_LOG_CRIT;
            }

        } ਨਹੀਂਤਾਂ {
            level = NGX_LOG_CRIT;
        }

        ngx_log_error(level, c->ਲਘੂਗਣਕ, err, ngx_close_socket_n " %d failed", fd);
    }
}


ਖਾਲੀ
ngx_reusable_connection(ngx_connection_t *c, ngx_uint_t reusable)
{
    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->ਲਘੂਗਣਕ, 0,
                   "reusable connection: %ui", reusable);

    ਜੇ (c->reusable) {
        ngx_ਕਤਾਰ_remove(&c->ਕਤਾਰ);
        ngx_ਚੱਕਰ->reusable_connections_n--;

#ਜੇ (NGX_STAT_STUB)
        (ਖਾਲੀ) ngx_atomic_fetch_ਜੋੜੋ(ngx_stat_ਉਡੀਕ, -1);
#ਜੇ_ਅੰਤ
    }

    c->reusable = reusable;

    ਜੇ (reusable) {
        /* need cast as ngx_cycle is volatile */

        ngx_ਕਤਾਰ_ਪਾਓ_ਸਿਰ(
            (ngx_ਕਤਾਰ_t *) &ngx_ਚੱਕਰ->reusable_connections_ਕਤਾਰ, &c->ਕਤਾਰ);
        ngx_ਚੱਕਰ->reusable_connections_n++;

#ਜੇ (NGX_STAT_STUB)
        (ਖਾਲੀ) ngx_atomic_fetch_ਜੋੜੋ(ngx_stat_ਉਡੀਕ, 1);
#ਜੇ_ਅੰਤ
    }
}


ਸਥਿਰ ਖਾਲੀ
ngx_drain_connections(ngx_ਚੱਕਰ_t *ਚੱਕਰ)
{
    ngx_uint_t         i, n;
    ngx_ਕਤਾਰ_t       *q;
    ngx_connection_t  *c;

    ਜੇ (ਚੱਕਰ->free_connection_n > ਚੱਕਰ->connection_n / 16
        || ਚੱਕਰ->reusable_connections_n == 0)
    {
        ਵਾਪਸ;
    }

    ਜੇ (ਚੱਕਰ->connections_reuse_ਸਮਾਂ != ngx_ਸਮਾਂ()) {
        ਚੱਕਰ->connections_reuse_ਸਮਾਂ = ngx_ਸਮਾਂ();

        ngx_log_error(NGX_LOG_WARN, ਚੱਕਰ->ਲਘੂਗਣਕ, 0,
                      "%ui worker_connections are not enough, "
                      "reusing connections",
                      ਚੱਕਰ->connection_n);
    }

    c = ਨਿਰਾ;
    n = ngx_max(ngx_min(32, ਚੱਕਰ->reusable_connections_n / 8), 1);

    ਲਈ (i = 0; i < n; i++) {
        ਜੇ (ngx_ਕਤਾਰ_ਖਾਲੀ_ਥਾਵਾਂ(&ਚੱਕਰ->reusable_connections_ਕਤਾਰ)) {
            ਰੋਕੋ;
        }

        q = ngx_ਕਤਾਰ_ਆਖਰੀ(&ਚੱਕਰ->reusable_connections_ਕਤਾਰ);
        c = ngx_ਕਤਾਰ_ਅੰਕੜੇ(q, ngx_connection_t, ਕਤਾਰ);

        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->ਲਘੂਗਣਕ, 0,
                       "reusing connection");

        c->close = 1;
        c->ਪਠ->handler(c->ਪਠ);
    }

    ਜੇ (ਚੱਕਰ->free_connection_n == 0 && c && c->reusable) {

        /*
         * if no connections were freed, try to reuse the last
         * connection again: this should free it as long as
         * previous reuse moved it to lingering close
         */

        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->ਲਘੂਗਣਕ, 0,
                       "reusing connection again");

        c->close = 1;
        c->ਪਠ->handler(c->ਪਠ);
    }
}


ਖਾਲੀ
ngx_close_idle_connections(ngx_ਚੱਕਰ_t *ਚੱਕਰ)
{
    ngx_uint_t         i;
    ngx_connection_t  *c;

    c = ਚੱਕਰ->connections;

    ਲਈ (i = 0; i < ਚੱਕਰ->connection_n; i++) {

        /* THREAD: lock */

        ਜੇ (c[i].fd != (ngx_socket_t) -1 && c[i].idle) {
            c[i].close = 1;
            c[i].ਪਠ->handler(c[i].ਪਠ);
        }
    }
}


ngx_int_t
ngx_connection_local_sockaddr(ngx_connection_t *c, ngx_str_t *s,
    ngx_uint_t ਪੋਰਟ)
{
    socklen_t             len;
    ngx_uint_t            addr;
    ngx_sockaddr_t        sa;
    ਬਣਤਰ sockaddr_ਵਿੱਚ   *ਜੀਵਾ;
#ਜੇ (NGX_HAVE_INET6)
    ngx_uint_t            i;
    ਬਣਤਰ sockaddr_in6  *sin6;
#ਜੇ_ਅੰਤ

    addr = 0;

    ਜੇ (c->local_socklen) {
        ਚੋਣ (c->local_sockaddr->sa_family) {

#ਜੇ (NGX_HAVE_INET6)
        ਹਾਲਤ AF_INET6:
            sin6 = (ਬਣਤਰ sockaddr_in6 *) c->local_sockaddr;

            ਲਈ (i = 0; addr == 0 && i < 16; i++) {
                addr |= sin6->sin6_addr.s6_addr[i];
            }

            ਰੋਕੋ;
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HAVE_UNIX_DOMAIN)
        ਹਾਲਤ AF_UNIX:
            addr = 1;
            ਰੋਕੋ;
#ਜੇ_ਅੰਤ

        ਮੂਲ: /* AF_INET */
            ਜੀਵਾ = (ਬਣਤਰ sockaddr_ਵਿੱਚ *) c->local_sockaddr;
            addr = ਜੀਵਾ->sin_addr.s_addr;
            ਰੋਕੋ;
        }
    }

    ਜੇ (addr == 0) {

        len = ਆਕਾਰ(ngx_sockaddr_t);

        ਜੇ (getsockname(c->fd, &sa.sockaddr, &len) == -1) {
            ngx_connection_error(c, ngx_socket_errno, "getsockname() failed");
            ਵਾਪਸ NGX_ERROR;
        }

        c->local_sockaddr = ngx_palloc(c->pool, len);
        ਜੇ (c->local_sockaddr == ਨਿਰਾ) {
            ਵਾਪਸ NGX_ERROR;
        }

        ngx_memcpy(c->local_sockaddr, &sa, len);

        c->local_socklen = len;
    }

    ਜੇ (s == ਨਿਰਾ) {
        ਵਾਪਸ NGX_OK;
    }

    s->len = ngx_sock_ntop(c->local_sockaddr, c->local_socklen,
                           s->ਅੰਕੜੇ, s->len, ਪੋਰਟ);

    ਵਾਪਸ NGX_OK;
}


ngx_int_t
ngx_tcp_nodelay(ngx_connection_t *c)
{
    ਪੂਰਨ  tcp_nodelay;

    ਜੇ (c->tcp_nodelay != NGX_TCP_NODELAY_UNSET) {
        ਵਾਪਸ NGX_OK;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->ਲਘੂਗਣਕ, 0, "tcp_nodelay");

    tcp_nodelay = 1;

    ਜੇ (setsockopt(c->fd, IPPROTO_TCP, TCP_NODELAY,
                   (ਅਚਲ ਖਾਲੀ *) &tcp_nodelay, ਆਕਾਰ(ਪੂਰਨ))
        == -1)
    {
#ਜੇ (NGX_SOLARIS)
        ਜੇ (c->log_error == NGX_ERROR_INFO) {

            /* Solaris returns EINVAL if a socket has been shut down */
            c->log_error = NGX_ERROR_IGNORE_EINVAL;

            ngx_connection_error(c, ngx_socket_errno,
                                 "setsockopt(TCP_NODELAY) failed");

            c->log_error = NGX_ERROR_INFO;

            ਵਾਪਸ NGX_ERROR;
        }
#ਜੇ_ਅੰਤ

        ngx_connection_error(c, ngx_socket_errno,
                             "setsockopt(TCP_NODELAY) failed");
        ਵਾਪਸ NGX_ERROR;
    }

    c->tcp_nodelay = NGX_TCP_NODELAY_ਸੈੱਟ;

    ਵਾਪਸ NGX_OK;
}


ngx_int_t
ngx_connection_error(ngx_connection_t *c, ngx_err_t err, ਅੱਖਰ *ਪਾਠ)
{
    ngx_uint_t  level;

    /* Winsock may return NGX_ECONNABORTED instead of NGX_ECONNRESET */

    ਜੇ ((err == NGX_ECONNRESET
#ਜੇ (NGX_WIN32)
         || err == NGX_ECONNABORTED
#ਜੇ_ਅੰਤ
        ) && c->log_error == NGX_ERROR_IGNORE_ECONNRESET)
    {
        ਵਾਪਸ 0;
    }

#ਜੇ (NGX_SOLARIS)
    ਜੇ (err == NGX_EINVAL && c->log_error == NGX_ERROR_IGNORE_EINVAL) {
        ਵਾਪਸ 0;
    }
#ਜੇ_ਅੰਤ

    ਜੇ (err == NGX_EMSGSIZE && c->log_error == NGX_ERROR_IGNORE_EMSGSIZE) {
        ਵਾਪਸ 0;
    }

#ਜੇ (NGX_LINUX)
    ਜੇ (err == NGX_EIO && c->ktls_recv) {
        ਵਾਪਸ 0;
    }
#ਜੇ_ਅੰਤ

    ਜੇ (err == 0
        || err == NGX_ECONNRESET
#ਜੇ (NGX_WIN32)
        || err == NGX_ECONNABORTED
#ਨਹੀਂਤਾਂ
        || err == NGX_EPIPE
#ਜੇ_ਅੰਤ
        || err == NGX_ENOTCONN
        || err == NGX_ETIMEDOUT
        || err == NGX_ECONNREFUSED
        || err == NGX_ENETDOWN
        || err == NGX_ENETUNREACH
        || err == NGX_EHOSTDOWN
        || err == NGX_EHOSTUNREACH)
    {
        ਚੋਣ (c->log_error) {

        ਹਾਲਤ NGX_ERROR_IGNORE_EMSGSIZE:
        ਹਾਲਤ NGX_ERROR_IGNORE_EINVAL:
        ਹਾਲਤ NGX_ERROR_IGNORE_ECONNRESET:
        ਹਾਲਤ NGX_ERROR_INFO:
            level = NGX_LOG_INFO;
            ਰੋਕੋ;

        ਮੂਲ:
            level = NGX_LOG_ERR;
        }

    } ਨਹੀਂਤਾਂ {
        level = NGX_LOG_ALERT;
    }

    ngx_log_error(level, c->ਲਘੂਗਣਕ, err, ਪਾਠ);

    ਵਾਪਸ NGX_ERROR;
}
