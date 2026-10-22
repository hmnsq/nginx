
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>
#അന്തര്ഭാവ <ngx_event.h>


ngx_os_io_t  ngx_io;


സ്ഥിതം ശൂന്യം ngx_drain_connections(ngx_cycle_t *cycle);


ngx_listening_t *
ngx_create_listening(ngx_conf_t *cf, ഘടന sockaddr *sockaddr,
    socklen_t socklen)
{
    size_t            len;
    ngx_listening_t  *ls;
    ഘടന sockaddr  *sa;
    u_char            text[NGX_SOCKADDR_STRLEN];

    ls = ngx_array_തള്ളുക(&cf->cycle->listening);
    എങ്കിൽ (ls == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    ngx_memzero(ls, വലുപ്പം(ngx_listening_t));

    sa = ngx_palloc(cf->pool, socklen);
    എങ്കിൽ (sa == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    ngx_memcpy(sa, sockaddr, socklen);

    ls->sockaddr = sa;
    ls->socklen = socklen;

    len = ngx_sock_ntop(sa, socklen, text, NGX_SOCKADDR_STRLEN, 1);
    ls->addr_text.len = len;

    തിരഞ്ഞെടുപ്പ് (ls->sockaddr->sa_family) {
#എങ്കിൽ (NGX_HAVE_INET6)
    അവസ്ഥ AF_INET6:
        ls->addr_text_max_len = NGX_INET6_ADDRSTRLEN;
        നിർത്തുക;
#endif
#എങ്കിൽ (NGX_HAVE_UNIX_DOMAIN)
    അവസ്ഥ AF_UNIX:
        ls->addr_text_max_len = NGX_UNIX_ADDRSTRLEN;
        len++;
        നിർത്തുക;
#endif
    അവസ്ഥ AF_INET:
        ls->addr_text_max_len = NGX_INET_ADDRSTRLEN;
        നിർത്തുക;
    സ്ഥിരസ്ഥിതി:
        ls->addr_text_max_len = NGX_SOCKADDR_STRLEN;
        നിർത്തുക;
    }

    ls->addr_text.data = ngx_pnalloc(cf->pool, len);
    എങ്കിൽ (ls->addr_text.data == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    ngx_memcpy(ls->addr_text.data, text, len);

#എങ്കിൽ !(NGX_WIN32)
    ngx_rbtree_init(&ls->rbtree, &ls->sentinel, ngx_udp_rbtree_തിരുകുക_മൂല്യം);
#endif

    ls->fd = (ngx_socket_t) -1;
    ls->type = SOCK_STREAM;

    ls->backlog = NGX_LISTEN_BACKLOG;
    ls->rcvbuf = -1;
    ls->sndbuf = -1;

#എങ്കിൽ (NGX_HAVE_SETFIB)
    ls->setfib = -1;
#endif

#എങ്കിൽ (NGX_HAVE_TCP_FASTOPEN)
    ls->fastopen = -1;
#endif

    തിരികെ ls;
}


ngx_int_t
ngx_clone_listening(ngx_cycle_t *cycle, ngx_listening_t *ls)
{
#എങ്കിൽ (NGX_HAVE_REUSEPORT)

    ngx_int_t         n;
    ngx_core_conf_t  *ccf;
    ngx_listening_t   ols;

    എങ്കിൽ (!ls->reuseport || ls->worker != 0) {
        തിരികെ NGX_OK;
    }

    ols = *ls;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    വേണ്ടി (n = 1; n < ccf->worker_processes; n++) {

        /* create a socket for each worker process */

        ls = ngx_array_തള്ളുക(&cycle->listening);
        എങ്കിൽ (ls == ശൂന്യ) {
            തിരികെ NGX_ERROR;
        }

        *ls = ols;
        ls->worker = n;
    }

#endif

    തിരികെ NGX_OK;
}


ngx_int_t
ngx_set_inherited_sockets(ngx_cycle_t *cycle)
{
    size_t                     len;
    ngx_uint_t                 i;
    ngx_listening_t           *ls;
    socklen_t                  olen;
#എങ്കിൽ (NGX_HAVE_DEFERRED_ACCEPT || NGX_HAVE_TCP_FASTOPEN)
    ngx_err_t                  err;
#endif
#എങ്കിൽ (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    ഘടന accept_filter_arg   af;
#endif
#എങ്കിൽ (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)
    പൂർണ്ണസംഖ്യ                        timeout;
#endif
#എങ്കിൽ (NGX_HAVE_REUSEPORT)
    പൂർണ്ണസംഖ്യ                        reuseport;
#endif

    ls = cycle->listening.elts;
    വേണ്ടി (i = 0; i < cycle->listening.nelts; i++) {

        ls[i].sockaddr = ngx_palloc(cycle->pool, വലുപ്പം(ngx_sockaddr_t));
        എങ്കിൽ (ls[i].sockaddr == ശൂന്യ) {
            തിരികെ NGX_ERROR;
        }

        ls[i].socklen = വലുപ്പം(ngx_sockaddr_t);
        എങ്കിൽ (getsockname(ls[i].fd, ls[i].sockaddr, &ls[i].socklen) == -1) {
            ngx_log_error(NGX_LOG_CRIT, cycle->log, ngx_socket_errno,
                          "getsockname() of the inherited "
                          "socket #%d failed", ls[i].fd);
            ls[i].ignore = 1;
            തുടരുക;
        }

        എങ്കിൽ (ls[i].socklen > (socklen_t) വലുപ്പം(ngx_sockaddr_t)) {
            ls[i].socklen = വലുപ്പം(ngx_sockaddr_t);
        }

        തിരഞ്ഞെടുപ്പ് (ls[i].sockaddr->sa_family) {

#എങ്കിൽ (NGX_HAVE_INET6)
        അവസ്ഥ AF_INET6:
            ls[i].addr_text_max_len = NGX_INET6_ADDRSTRLEN;
            len = NGX_INET6_ADDRSTRLEN + വലുപ്പം("[]:65535") - 1;
            നിർത്തുക;
#endif

#എങ്കിൽ (NGX_HAVE_UNIX_DOMAIN)
        അവസ്ഥ AF_UNIX:
            ls[i].addr_text_max_len = NGX_UNIX_ADDRSTRLEN;
            len = NGX_UNIX_ADDRSTRLEN;
            നിർത്തുക;
#endif

        അവസ്ഥ AF_INET:
            ls[i].addr_text_max_len = NGX_INET_ADDRSTRLEN;
            len = NGX_INET_ADDRSTRLEN + വലുപ്പം(":65535") - 1;
            നിർത്തുക;

        സ്ഥിരസ്ഥിതി:
            ngx_log_error(NGX_LOG_CRIT, cycle->log, ngx_socket_errno,
                          "the inherited socket #%d has "
                          "an unsupported protocol family", ls[i].fd);
            ls[i].ignore = 1;
            തുടരുക;
        }

        ls[i].addr_text.data = ngx_pnalloc(cycle->pool, len);
        എങ്കിൽ (ls[i].addr_text.data == ശൂന്യ) {
            തിരികെ NGX_ERROR;
        }

        len = ngx_sock_ntop(ls[i].sockaddr, ls[i].socklen,
                            ls[i].addr_text.data, len, 1);
        എങ്കിൽ (len == 0) {
            തിരികെ NGX_ERROR;
        }

        ls[i].addr_text.len = len;

        ls[i].backlog = NGX_LISTEN_BACKLOG;

        olen = വലുപ്പം(പൂർണ്ണസംഖ്യ);

        എങ്കിൽ (getsockopt(ls[i].fd, SOL_SOCKET, SO_TYPE, (ശൂന്യം *) &ls[i].type,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_CRIT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_TYPE) %V failed", &ls[i].addr_text);
            ls[i].ignore = 1;
            തുടരുക;
        }

        olen = വലുപ്പം(പൂർണ്ണസംഖ്യ);

        എങ്കിൽ (getsockopt(ls[i].fd, SOL_SOCKET, SO_RCVBUF, (ശൂന്യം *) &ls[i].rcvbuf,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_RCVBUF) %V failed, ignored",
                          &ls[i].addr_text);

            ls[i].rcvbuf = -1;
        }

        olen = വലുപ്പം(പൂർണ്ണസംഖ്യ);

        എങ്കിൽ (getsockopt(ls[i].fd, SOL_SOCKET, SO_SNDBUF, (ശൂന്യം *) &ls[i].sndbuf,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_SNDBUF) %V failed, ignored",
                          &ls[i].addr_text);

            ls[i].sndbuf = -1;
        }

#എങ്കിൽ 0
        /* SO_SETFIB is currently a set only option */

#എങ്കിൽ (NGX_HAVE_SETFIB)

        olen = വലുപ്പം(പൂർണ്ണസംഖ്യ);

        എങ്കിൽ (getsockopt(ls[i].fd, SOL_SOCKET, SO_SETFIB,
                       (ശൂന്യം *) &ls[i].setfib, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_SETFIB) %V failed, ignored",
                          &ls[i].addr_text);

            ls[i].setfib = -1;
        }

#endif
#endif

#എങ്കിൽ (NGX_HAVE_REUSEPORT)

        reuseport = 0;
        olen = വലുപ്പം(പൂർണ്ണസംഖ്യ);

#ifdef SO_REUSEPORT_LB

        എങ്കിൽ (getsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT_LB,
                       (ശൂന്യം *) &reuseport, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_REUSEPORT_LB) %V failed, ignored",
                          &ls[i].addr_text);

        } അല്ലെങ്കിൽ {
            ls[i].reuseport = reuseport ? 1 : 0;
        }

#അല്ലെങ്കിൽ

        എങ്കിൽ (getsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT,
                       (ശൂന്യം *) &reuseport, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_REUSEPORT) %V failed, ignored",
                          &ls[i].addr_text);

        } അല്ലെങ്കിൽ {
            ls[i].reuseport = reuseport ? 1 : 0;
        }
#endif

#endif

        എങ്കിൽ (ls[i].type != SOCK_STREAM) {
            തുടരുക;
        }

#ifdef SO_PROTOCOL

        olen = വലുപ്പം(പൂർണ്ണസംഖ്യ);

        എങ്കിൽ (getsockopt(ls[i].fd, SOL_SOCKET, SO_PROTOCOL,
                       (ശൂന്യം *) &ls[i].protocol, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_PROTOCOL) %V failed, ignored",
                          &ls[i].addr_text);
            ls[i].protocol = 0;

        } അല്ലെങ്കിൽ എങ്കിൽ (ls[i].protocol == IPPROTO_TCP) {
            ls[i].protocol = 0;
        }

#endif

#എങ്കിൽ (NGX_HAVE_TCP_FASTOPEN)

        olen = വലുപ്പം(പൂർണ്ണസംഖ്യ);

        എങ്കിൽ (getsockopt(ls[i].fd, IPPROTO_TCP, TCP_FASTOPEN,
                       (ശൂന്യം *) &ls[i].fastopen, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            എങ്കിൽ (err != NGX_EOPNOTSUPP && err != NGX_ENOPROTOOPT
                && err != NGX_EINVAL)
            {
                ngx_log_error(NGX_LOG_NOTICE, cycle->log, err,
                              "getsockopt(TCP_FASTOPEN) %V failed, ignored",
                              &ls[i].addr_text);
            }

            ls[i].fastopen = -1;
        }

#endif

#എങ്കിൽ (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)

        ngx_memzero(&af, വലുപ്പം(ഘടന accept_filter_arg));
        olen = വലുപ്പം(ഘടന accept_filter_arg);

        എങ്കിൽ (getsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER, &af, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            എങ്കിൽ (err == NGX_EINVAL) {
                തുടരുക;
            }

            ngx_log_error(NGX_LOG_NOTICE, cycle->log, err,
                          "getsockopt(SO_ACCEPTFILTER) for %V failed, ignored",
                          &ls[i].addr_text);
            തുടരുക;
        }

        എങ്കിൽ (olen < വലുപ്പം(ഘടന accept_filter_arg) || af.af_name[0] == '\0') {
            തുടരുക;
        }

        ls[i].accept_filter = ngx_palloc(cycle->pool, 16);
        എങ്കിൽ (ls[i].accept_filter == ശൂന്യ) {
            തിരികെ NGX_ERROR;
        }

        (ശൂന്യം) ngx_cpystrn((u_char *) ls[i].accept_filter,
                           (u_char *) af.af_name, 16);
#endif

#എങ്കിൽ (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)

        timeout = 0;
        olen = വലുപ്പം(പൂർണ്ണസംഖ്യ);

        എങ്കിൽ (getsockopt(ls[i].fd, IPPROTO_TCP, TCP_DEFER_ACCEPT, &timeout, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            എങ്കിൽ (err == NGX_EOPNOTSUPP) {
                തുടരുക;
            }

            ngx_log_error(NGX_LOG_NOTICE, cycle->log, err,
                          "getsockopt(TCP_DEFER_ACCEPT) for %V failed, ignored",
                          &ls[i].addr_text);
            തുടരുക;
        }

        എങ്കിൽ (olen < വലുപ്പം(പൂർണ്ണസംഖ്യ) || timeout == 0) {
            തുടരുക;
        }

        ls[i].deferred_accept = 1;
#endif
    }

    തിരികെ NGX_OK;
}


ngx_int_t
ngx_open_listening_sockets(ngx_cycle_t *cycle)
{
    പൂർണ്ണസംഖ്യ               reuseaddr;
    ngx_uint_t        i, tries, failed;
    ngx_err_t         err;
    ngx_log_t        *log;
    ngx_socket_t      s;
    ngx_listening_t  *ls;

    reuseaddr = 1;
#എങ്കിൽ (NGX_SUPPRESS_WARN)
    failed = 0;
#endif

    log = cycle->log;

    /* TODO: configurable try number */

    വേണ്ടി (tries = 5; tries; tries--) {
        failed = 0;

        /* for each listening socket */

        ls = cycle->listening.elts;
        വേണ്ടി (i = 0; i < cycle->listening.nelts; i++) {

            എങ്കിൽ (ls[i].ignore) {
                തുടരുക;
            }

#എങ്കിൽ (NGX_HAVE_REUSEPORT)

            എങ്കിൽ (ls[i].add_reuseport || ls[i].change_protocol) {

                /*
                 * to allow transition from a socket without SO_REUSEPORT
                 * to multiple sockets with SO_REUSEPORT, we have to set
                 * SO_REUSEPORT on the old socket before opening new ones
                 *
                 * to allow transition between different socket protocols
                 * (e.g. IPPROTO_MPTCP), SO_REUSEPORT is set on both old
                 * and new sockets
                 */

                പൂർണ്ണസംഖ്യ  reuseport = 1;

#ifdef SO_REUSEPORT_LB

                എങ്കിൽ (setsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT_LB,
                               (സ്ഥിരം ശൂന്യം *) &reuseport, വലുപ്പം(പൂർണ്ണസംഖ്യ))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT_LB) %V failed, "
                                  "ignored",
                                  &ls[i].addr_text);
                }

#അല്ലെങ്കിൽ

                എങ്കിൽ (setsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT,
                               (സ്ഥിരം ശൂന്യം *) &reuseport, വലുപ്പം(പൂർണ്ണസംഖ്യ))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT) %V failed, ignored",
                                  &ls[i].addr_text);
                }
#endif

                ls[i].add_reuseport = 0;
            }
#endif

            എങ്കിൽ (ls[i].fd != (ngx_socket_t) -1 && !ls[i].change_protocol) {
                തുടരുക;
            }

            എങ്കിൽ (ls[i].inherited) {

                /* TODO: close on exit */
                /* TODO: nonblocking */
                /* TODO: deferred accept */

                തുടരുക;
            }

            s = ngx_socket(ls[i].sockaddr->sa_family, ls[i].type,
                           ls[i].protocol);

            എങ്കിൽ (s == (ngx_socket_t) -1) {
                ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                              ngx_socket_n " %V failed", &ls[i].addr_text);
                തിരികെ NGX_ERROR;
            }

            എങ്കിൽ (ls[i].type != SOCK_DGRAM || !ngx_test_config) {

                എങ്കിൽ (setsockopt(s, SOL_SOCKET, SO_REUSEADDR,
                               (സ്ഥിരം ശൂന്യം *) &reuseaddr, വലുപ്പം(പൂർണ്ണസംഖ്യ))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEADDR) %V failed",
                                  &ls[i].addr_text);

                    എങ്കിൽ (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_text);
                    }

                    തിരികെ NGX_ERROR;
                }
            }

#എങ്കിൽ (NGX_HAVE_REUSEPORT)

            എങ്കിൽ ((ls[i].reuseport || ls[i].change_protocol)
                && !ngx_test_config)
            {
                പൂർണ്ണസംഖ്യ  reuseport;

                reuseport = 1;

#ifdef SO_REUSEPORT_LB

                എങ്കിൽ (setsockopt(s, SOL_SOCKET, SO_REUSEPORT_LB,
                               (സ്ഥിരം ശൂന്യം *) &reuseport, വലുപ്പം(പൂർണ്ണസംഖ്യ))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT_LB) %V failed",
                                  &ls[i].addr_text);

                    എങ്കിൽ (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_text);
                    }

                    തിരികെ NGX_ERROR;
                }

#അല്ലെങ്കിൽ

                എങ്കിൽ (setsockopt(s, SOL_SOCKET, SO_REUSEPORT,
                               (സ്ഥിരം ശൂന്യം *) &reuseport, വലുപ്പം(പൂർണ്ണസംഖ്യ))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT) %V failed",
                                  &ls[i].addr_text);

                    എങ്കിൽ (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_text);
                    }

                    തിരികെ NGX_ERROR;
                }
#endif
            }
#endif

#എങ്കിൽ (NGX_HAVE_INET6 && defined IPV6_V6ONLY)

            എങ്കിൽ (ls[i].sockaddr->sa_family == AF_INET6) {
                പൂർണ്ണസംഖ്യ  ipv6only;

                ipv6only = ls[i].ipv6only;

                എങ്കിൽ (setsockopt(s, IPPROTO_IPV6, IPV6_V6ONLY,
                               (സ്ഥിരം ശൂന്യം *) &ipv6only, വലുപ്പം(പൂർണ്ണസംഖ്യ))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(IPV6_V6ONLY) %V failed, ignored",
                                  &ls[i].addr_text);
                }
            }
#endif
            /* TODO: close on exit */

            എങ്കിൽ (ngx_nonblocking(s) == -1) {
                ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                              ngx_nonblocking_n " %V failed",
                              &ls[i].addr_text);

                എങ്കിൽ (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_text);
                }

                തിരികെ NGX_ERROR;
            }

            ngx_log_debug2(NGX_LOG_DEBUG_CORE, log, 0,
                           "bind() %V #%d ", &ls[i].addr_text, s);

            എങ്കിൽ (bind(s, ls[i].sockaddr, ls[i].socklen) == -1) {
                err = ngx_socket_errno;

                എങ്കിൽ (err != NGX_EADDRINUSE || !ngx_test_config) {
                    ngx_log_error(NGX_LOG_EMERG, log, err,
                                  "bind() to %V failed", &ls[i].addr_text);
                }

                എങ്കിൽ (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_text);
                }

                എങ്കിൽ (err != NGX_EADDRINUSE) {
                    തിരികെ NGX_ERROR;
                }

                എങ്കിൽ (!ngx_test_config) {
                    failed = 1;
                }

                തുടരുക;
            }

#എങ്കിൽ (NGX_HAVE_UNIX_DOMAIN)

            എങ്കിൽ (ls[i].sockaddr->sa_family == AF_UNIX) {
                mode_t   mode;
                u_char  *name;

                name = ls[i].addr_text.data + വലുപ്പം("unix:") - 1;
                mode = (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);

                എങ്കിൽ (chmod((അക്ഷരം *) name, mode) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  "chmod() \"%s\" failed", name);
                }

                എങ്കിൽ (ngx_test_config) {
                    എങ്കിൽ (ngx_delete_file(name) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                      ngx_delete_file_n " %s failed", name);
                    }
                }
            }
#endif

            എങ്കിൽ (ls[i].type != SOCK_STREAM) {
                ls[i].fd = s;
                ls[i].open = 1;
                തുടരുക;
            }

            എങ്കിൽ (listen(s, ls[i].backlog) == -1) {
                err = ngx_socket_errno;

                /*
                 * on OpenVZ after suspend/resume EADDRINUSE
                 * may be returned by listen() instead of bind(), see
                 * https://bugs.openvz.org/browse/OVZ-5587
                 */

                എങ്കിൽ (err != NGX_EADDRINUSE || !ngx_test_config) {
                    ngx_log_error(NGX_LOG_EMERG, log, err,
                                  "listen() to %V, backlog %d failed",
                                  &ls[i].addr_text, ls[i].backlog);
                }

                എങ്കിൽ (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_text);
                }

                എങ്കിൽ (err != NGX_EADDRINUSE) {
                    തിരികെ NGX_ERROR;
                }

                എങ്കിൽ (!ngx_test_config) {
                    failed = 1;
                }

                തുടരുക;
            }

            ls[i].listen = 1;

            ls[i].fd = s;
            ls[i].open = 1;
        }

        എങ്കിൽ (!failed) {
            നിർത്തുക;
        }

        /* TODO: delay configurable */

        ngx_log_error(NGX_LOG_NOTICE, log, 0,
                      "try again to bind() after 500ms");

        ngx_msleep(500);
    }

    എങ്കിൽ (failed) {
        ngx_log_error(NGX_LOG_EMERG, log, 0, "still could not bind()");
        തിരികെ NGX_ERROR;
    }

    തിരികെ NGX_OK;
}


ശൂന്യം
ngx_configure_listening_sockets(ngx_cycle_t *cycle)
{
    പൂർണ്ണസംഖ്യ                        മൂല്യം;
    ngx_uint_t                 i;
    ngx_listening_t           *ls;

#എങ്കിൽ (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    ഘടന accept_filter_arg   af;
#endif

    ls = cycle->listening.elts;
    വേണ്ടി (i = 0; i < cycle->listening.nelts; i++) {

        ls[i].log = *ls[i].logp;

        എങ്കിൽ (ls[i].rcvbuf != -1) {
            എങ്കിൽ (setsockopt(ls[i].fd, SOL_SOCKET, SO_RCVBUF,
                           (സ്ഥിരം ശൂന്യം *) &ls[i].rcvbuf, വലുപ്പം(പൂർണ്ണസംഖ്യ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_RCVBUF, %d) %V failed, ignored",
                              ls[i].rcvbuf, &ls[i].addr_text);
            }
        }

        എങ്കിൽ (ls[i].sndbuf != -1) {
            എങ്കിൽ (setsockopt(ls[i].fd, SOL_SOCKET, SO_SNDBUF,
                           (സ്ഥിരം ശൂന്യം *) &ls[i].sndbuf, വലുപ്പം(പൂർണ്ണസംഖ്യ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_SNDBUF, %d) %V failed, ignored",
                              ls[i].sndbuf, &ls[i].addr_text);
            }
        }

        എങ്കിൽ (ls[i].keepalive) {
            മൂല്യം = (ls[i].keepalive == 1) ? 1 : 0;

            എങ്കിൽ (setsockopt(ls[i].fd, SOL_SOCKET, SO_KEEPALIVE,
                           (സ്ഥിരം ശൂന്യം *) &മൂല്യം, വലുപ്പം(പൂർണ്ണസംഖ്യ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_KEEPALIVE, %d) %V failed, ignored",
                              മൂല്യം, &ls[i].addr_text);
            }
        }

#എങ്കിൽ (NGX_HAVE_KEEPALIVE_TUNABLE)

#എങ്കിൽ !(NGX_DARWIN)

        എങ്കിൽ (ls[i].keepidle) {
            മൂല്യം = ls[i].keepidle;

#എങ്കിൽ (NGX_KEEPALIVE_FACTOR)
            മൂല്യം *= NGX_KEEPALIVE_FACTOR;
#endif

            എങ്കിൽ (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPIDLE,
                           (സ്ഥിരം ശൂന്യം *) &മൂല്യം, വലുപ്പം(പൂർണ്ണസംഖ്യ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_KEEPIDLE, %d) %V failed, ignored",
                              മൂല്യം, &ls[i].addr_text);
            }
        }

#endif

        എങ്കിൽ (ls[i].keepintvl) {
            മൂല്യം = ls[i].keepintvl;

#എങ്കിൽ (NGX_KEEPALIVE_FACTOR)
            മൂല്യം *= NGX_KEEPALIVE_FACTOR;
#endif

            എങ്കിൽ (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPINTVL,
                           (സ്ഥിരം ശൂന്യം *) &മൂല്യം, വലുപ്പം(പൂർണ്ണസംഖ്യ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                             "setsockopt(TCP_KEEPINTVL, %d) %V failed, ignored",
                             മൂല്യം, &ls[i].addr_text);
            }
        }

        എങ്കിൽ (ls[i].keepcnt) {
            എങ്കിൽ (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPCNT,
                           (സ്ഥിരം ശൂന്യം *) &ls[i].keepcnt, വലുപ്പം(പൂർണ്ണസംഖ്യ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_KEEPCNT, %d) %V failed, ignored",
                              ls[i].keepcnt, &ls[i].addr_text);
            }
        }

#endif

#എങ്കിൽ (NGX_HAVE_SETFIB)
        എങ്കിൽ (ls[i].setfib != -1) {
            എങ്കിൽ (setsockopt(ls[i].fd, SOL_SOCKET, SO_SETFIB,
                           (സ്ഥിരം ശൂന്യം *) &ls[i].setfib, വലുപ്പം(പൂർണ്ണസംഖ്യ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_SETFIB, %d) %V failed, ignored",
                              ls[i].setfib, &ls[i].addr_text);
            }
        }
#endif

#എങ്കിൽ (NGX_HAVE_TCP_FASTOPEN)
        എങ്കിൽ (ls[i].fastopen != -1) {
            എങ്കിൽ (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_FASTOPEN,
                           (സ്ഥിരം ശൂന്യം *) &ls[i].fastopen, വലുപ്പം(പൂർണ്ണസംഖ്യ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_FASTOPEN, %d) %V failed, ignored",
                              ls[i].fastopen, &ls[i].addr_text);
            }
        }
#endif

#എങ്കിൽ 0
        എങ്കിൽ (1) {
            പൂർണ്ണസംഖ്യ tcp_nodelay = 1;

            എങ്കിൽ (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_NODELAY,
                       (സ്ഥിരം ശൂന്യം *) &tcp_nodelay, വലുപ്പം(പൂർണ്ണസംഖ്യ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_NODELAY) %V failed, ignored",
                              &ls[i].addr_text);
            }
        }
#endif

        എങ്കിൽ (ls[i].listen) {

            /* change backlog via listen() */

            എങ്കിൽ (listen(ls[i].fd, ls[i].backlog) == -1) {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "listen() to %V, backlog %d failed, ignored",
                              &ls[i].addr_text, ls[i].backlog);
            }
        }

        /*
         * setting deferred mode should be last operation on socket,
         * because code may prematurely continue cycle on failure
         */

#എങ്കിൽ (NGX_HAVE_DEFERRED_ACCEPT)

#ifdef SO_ACCEPTFILTER

        എങ്കിൽ (ls[i].delete_deferred) {
            എങ്കിൽ (setsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER, ശൂന്യ, 0)
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_ACCEPTFILTER, NULL) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);

                എങ്കിൽ (ls[i].accept_filter) {
                    ngx_log_error(NGX_LOG_ALERT, cycle->log, 0,
                                  "could not change the accept filter "
                                  "to \"%s\" for %V, ignored",
                                  ls[i].accept_filter, &ls[i].addr_text);
                }

                തുടരുക;
            }

            ls[i].deferred_accept = 0;
        }

        എങ്കിൽ (ls[i].add_deferred) {
            ngx_memzero(&af, വലുപ്പം(ഘടന accept_filter_arg));
            (ശൂന്യം) ngx_cpystrn((u_char *) af.af_name,
                               (u_char *) ls[i].accept_filter, 16);

            എങ്കിൽ (setsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER,
                           &af, വലുപ്പം(ഘടന accept_filter_arg))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_ACCEPTFILTER, \"%s\") "
                              "for %V failed, ignored",
                              ls[i].accept_filter, &ls[i].addr_text);
                തുടരുക;
            }

            ls[i].deferred_accept = 1;
        }

#endif

#ifdef TCP_DEFER_ACCEPT

        എങ്കിൽ (ls[i].add_deferred || ls[i].delete_deferred) {

            എങ്കിൽ (ls[i].add_deferred) {
                /*
                 * There is no way to find out how long a connection was
                 * in queue (and a connection may bypass deferred queue at all
                 * if syncookies were used), hence we use 1 second timeout
                 * here.
                 */
                മൂല്യം = 1;

            } അല്ലെങ്കിൽ {
                മൂല്യം = 0;
            }

            എങ്കിൽ (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_DEFER_ACCEPT,
                           &മൂല്യം, വലുപ്പം(പൂർണ്ണസംഖ്യ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_DEFER_ACCEPT, %d) for %V failed, "
                              "ignored",
                              മൂല്യം, &ls[i].addr_text);

                തുടരുക;
            }
        }

        എങ്കിൽ (ls[i].add_deferred) {
            ls[i].deferred_accept = 1;
        }

#endif

#endif /* NGX_HAVE_DEFERRED_ACCEPT */

#എങ്കിൽ (NGX_HAVE_IP_RECVDSTADDR)

        എങ്കിൽ (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET)
        {
            മൂല്യം = 1;

            എങ്കിൽ (setsockopt(ls[i].fd, IPPROTO_IP, IP_RECVDSTADDR,
                           (സ്ഥിരം ശൂന്യം *) &മൂല്യം, വലുപ്പം(പൂർണ്ണസംഖ്യ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_RECVDSTADDR) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#അല്ലെങ്കിലെങ്കിൽ (NGX_HAVE_IP_PKTINFO)

        എങ്കിൽ (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET)
        {
            മൂല്യം = 1;

            എങ്കിൽ (setsockopt(ls[i].fd, IPPROTO_IP, IP_PKTINFO,
                           (സ്ഥിരം ശൂന്യം *) &മൂല്യം, വലുപ്പം(പൂർണ്ണസംഖ്യ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_PKTINFO) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#endif

#എങ്കിൽ (NGX_HAVE_INET6 && NGX_HAVE_IPV6_RECVPKTINFO)

        എങ്കിൽ (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET6)
        {
            മൂല്യം = 1;

            എങ്കിൽ (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_RECVPKTINFO,
                           (സ്ഥിരം ശൂന്യം *) &മൂല്യം, വലുപ്പം(പൂർണ്ണസംഖ്യ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IPV6_RECVPKTINFO) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#endif

#എങ്കിൽ (NGX_HAVE_IP_MTU_DISCOVER)

        എങ്കിൽ (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET) {
            മൂല്യം = IP_PMTUDISC_DO;

            എങ്കിൽ (setsockopt(ls[i].fd, IPPROTO_IP, IP_MTU_DISCOVER,
                           (സ്ഥിരം ശൂന്യം *) &മൂല്യം, വലുപ്പം(പൂർണ്ണസംഖ്യ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_MTU_DISCOVER) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#അല്ലെങ്കിലെങ്കിൽ (NGX_HAVE_IP_DONTFRAG)

        എങ്കിൽ (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET) {
            മൂല്യം = 1;

            എങ്കിൽ (setsockopt(ls[i].fd, IPPROTO_IP, IP_DONTFRAG,
                           (സ്ഥിരം ശൂന്യം *) &മൂല്യം, വലുപ്പം(പൂർണ്ണസംഖ്യ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_DONTFRAG) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#endif

#എങ്കിൽ (NGX_HAVE_INET6)

#എങ്കിൽ (NGX_HAVE_IPV6_MTU_DISCOVER)

        എങ്കിൽ (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET6) {
            മൂല്യം = IPV6_PMTUDISC_DO;

            എങ്കിൽ (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_MTU_DISCOVER,
                           (സ്ഥിരം ശൂന്യം *) &മൂല്യം, വലുപ്പം(പൂർണ്ണസംഖ്യ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IPV6_MTU_DISCOVER) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#അല്ലെങ്കിലെങ്കിൽ (NGX_HAVE_IPV6_DONTFRAG)

        എങ്കിൽ (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET6) {
            മൂല്യം = 1;

            എങ്കിൽ (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_DONTFRAG,
                           (സ്ഥിരം ശൂന്യം *) &മൂല്യം, വലുപ്പം(പൂർണ്ണസംഖ്യ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IPV6_DONTFRAG) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#endif

#endif
    }

    തിരികെ;
}


ശൂന്യം
ngx_close_listening_sockets(ngx_cycle_t *cycle)
{
    ngx_uint_t         i;
    ngx_listening_t   *ls;
    ngx_connection_t  *c;

    ngx_accept_mutex_held = 0;
    ngx_use_accept_mutex = 0;

    ls = cycle->listening.elts;
    വേണ്ടി (i = 0; i < cycle->listening.nelts; i++) {

#എങ്കിൽ (NGX_QUIC)
        എങ്കിൽ (ls[i].quic
            && (!ls[i].reuseport || !ngx_quic_bpf_enabled(cycle)))
        {
            തുടരുക;
        }
#endif

        c = ls[i].connection;

        എങ്കിൽ (c) {
            എങ്കിൽ (c->പഠ->active) {
                എങ്കിൽ (ngx_event_flags & NGX_USE_EPOLL_EVENT) {

                    /*
                     * it seems that Linux-2.6.x OpenVZ sends events
                     * for closed shared listening sockets unless
                     * the events was explicitly deleted
                     */

                    ngx_del_event(c->പഠ, NGX_READ_EVENT, 0);

                } അല്ലെങ്കിൽ {
                    ngx_del_event(c->പഠ, NGX_READ_EVENT, NGX_CLOSE_EVENT);
                }
            }

            എങ്കിൽ (c->പഠ->timer_set) {
                ngx_del_timer(c->പഠ);
            }

            ngx_free_connection(c);

            c->fd = (ngx_socket_t) -1;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, cycle->log, 0,
                       "close listening %V #%d ", &ls[i].addr_text, ls[i].fd);

        എങ്കിൽ (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_socket_errno,
                          ngx_close_socket_n " %V failed", &ls[i].addr_text);
        }

#എങ്കിൽ (NGX_HAVE_UNIX_DOMAIN)

        എങ്കിൽ (ls[i].sockaddr->sa_family == AF_UNIX
            && ngx_process <= NGX_PROCESS_MASTER
            && ngx_new_ദ്വിമാന == 0
            && (!ls[i].inherited || ngx_getppid() != ngx_parent))
        {
            u_char *name = ls[i].addr_text.data + വലുപ്പം("unix:") - 1;

            എങ്കിൽ (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_socket_errno,
                              ngx_delete_file_n " %s failed", name);
            }
        }

#endif

        ls[i].fd = (ngx_socket_t) -1;
    }

    cycle->listening.nelts = 0;
}


ngx_connection_t *
ngx_get_connection(ngx_socket_t s, ngx_log_t *log)
{
    ngx_uint_t         instance;
    ngx_event_t       *rev, *wev;
    ngx_connection_t  *c;

    /* disable warning: Win32 SOCKET is u_int while UNIX socket is int */

    എങ്കിൽ (ngx_cycle->files && (ngx_uint_t) s >= ngx_cycle->files_n) {
        ngx_log_error(NGX_LOG_ALERT, log, 0,
                      "the new socket has number %d, "
                      "but only %ui files are available",
                      s, ngx_cycle->files_n);
        തിരികെ ശൂന്യ;
    }

    ngx_drain_connections((ngx_cycle_t *) ngx_cycle);

    c = ngx_cycle->free_connections;

    എങ്കിൽ (c == ശൂന്യ) {
        ngx_log_error(NGX_LOG_ALERT, log, 0,
                      "%ui worker_connections are not enough",
                      ngx_cycle->connection_n);

        തിരികെ ശൂന്യ;
    }

    ngx_cycle->free_connections = c->data;
    ngx_cycle->free_connection_n--;

    എങ്കിൽ (ngx_cycle->files && ngx_cycle->files[s] == ശൂന്യ) {
        ngx_cycle->files[s] = c;
    }

    rev = c->പഠ;
    wev = c->ലിഖ;

    ngx_memzero(c, വലുപ്പം(ngx_connection_t));

    c->പഠ = rev;
    c->ലിഖ = wev;
    c->fd = s;
    c->log = log;

    instance = rev->instance;

    ngx_memzero(rev, വലുപ്പം(ngx_event_t));
    ngx_memzero(wev, വലുപ്പം(ngx_event_t));

    rev->instance = !instance;
    wev->instance = !instance;

    rev->index = NGX_INVALID_INDEX;
    wev->index = NGX_INVALID_INDEX;

    rev->data = c;
    wev->data = c;

    wev->ലിഖ = 1;

    തിരികെ c;
}


ശൂന്യം
ngx_free_connection(ngx_connection_t *c)
{
    c->data = ngx_cycle->free_connections;
    ngx_cycle->free_connections = c;
    ngx_cycle->free_connection_n++;

    എങ്കിൽ (ngx_cycle->files && ngx_cycle->files[c->fd] == c) {
        ngx_cycle->files[c->fd] = ശൂന്യ;
    }
}


ശൂന്യം
ngx_close_connection(ngx_connection_t *c)
{
    ngx_err_t     err;
    ngx_uint_t    log_error, level;
    ngx_socket_t  fd;

    എങ്കിൽ (c->fd == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0, "connection already closed");
        തിരികെ;
    }

    എങ്കിൽ (c->പഠ->timer_set) {
        ngx_del_timer(c->പഠ);
    }

    എങ്കിൽ (c->ലിഖ->timer_set) {
        ngx_del_timer(c->ലിഖ);
    }

    എങ്കിൽ (!c->shared) {
        എങ്കിൽ (ngx_del_conn) {
            ngx_del_conn(c, NGX_CLOSE_EVENT);

        } അല്ലെങ്കിൽ {
            എങ്കിൽ (c->പഠ->active || c->പഠ->disabled) {
                ngx_del_event(c->പഠ, NGX_READ_EVENT, NGX_CLOSE_EVENT);
            }

            എങ്കിൽ (c->ലിഖ->active || c->ലിഖ->disabled) {
                ngx_del_event(c->ലിഖ, NGX_WRITE_EVENT, NGX_CLOSE_EVENT);
            }
        }
    }

    എങ്കിൽ (c->പഠ->posted) {
        ngx_delete_posted_event(c->പഠ);
    }

    എങ്കിൽ (c->ലിഖ->posted) {
        ngx_delete_posted_event(c->ലിഖ);
    }

    c->പഠ->closed = 1;
    c->ലിഖ->closed = 1;

    ngx_reusable_connection(c, 0);

    log_error = c->log_error;

    ngx_free_connection(c);

    fd = c->fd;
    c->fd = (ngx_socket_t) -1;

    എങ്കിൽ (c->shared) {
        തിരികെ;
    }

    എങ്കിൽ (ngx_close_socket(fd) == -1) {

        err = ngx_socket_errno;

        എങ്കിൽ (err == NGX_ECONNRESET || err == NGX_ENOTCONN) {

            തിരഞ്ഞെടുപ്പ് (log_error) {

            അവസ്ഥ NGX_ERROR_INFO:
                level = NGX_LOG_INFO;
                നിർത്തുക;

            അവസ്ഥ NGX_ERROR_ERR:
                level = NGX_LOG_ERR;
                നിർത്തുക;

            സ്ഥിരസ്ഥിതി:
                level = NGX_LOG_CRIT;
            }

        } അല്ലെങ്കിൽ {
            level = NGX_LOG_CRIT;
        }

        ngx_log_error(level, c->log, err, ngx_close_socket_n " %d failed", fd);
    }
}


ശൂന്യം
ngx_reusable_connection(ngx_connection_t *c, ngx_uint_t reusable)
{
    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "reusable connection: %ui", reusable);

    എങ്കിൽ (c->reusable) {
        ngx_queue_remove(&c->queue);
        ngx_cycle->reusable_connections_n--;

#എങ്കിൽ (NGX_STAT_STUB)
        (ശൂന്യം) ngx_atomic_fetch_add(ngx_stat_waiting, -1);
#endif
    }

    c->reusable = reusable;

    എങ്കിൽ (reusable) {
        /* need cast as ngx_cycle is volatile */

        ngx_queue_തിരുകുക_head(
            (ngx_queue_t *) &ngx_cycle->reusable_connections_queue, &c->queue);
        ngx_cycle->reusable_connections_n++;

#എങ്കിൽ (NGX_STAT_STUB)
        (ശൂന്യം) ngx_atomic_fetch_add(ngx_stat_waiting, 1);
#endif
    }
}


സ്ഥിതം ശൂന്യം
ngx_drain_connections(ngx_cycle_t *cycle)
{
    ngx_uint_t         i, n;
    ngx_queue_t       *q;
    ngx_connection_t  *c;

    എങ്കിൽ (cycle->free_connection_n > cycle->connection_n / 16
        || cycle->reusable_connections_n == 0)
    {
        തിരികെ;
    }

    എങ്കിൽ (cycle->connections_reuse_time != ngx_time()) {
        cycle->connections_reuse_time = ngx_time();

        ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                      "%ui worker_connections are not enough, "
                      "reusing connections",
                      cycle->connection_n);
    }

    c = ശൂന്യ;
    n = ngx_max(ngx_min(32, cycle->reusable_connections_n / 8), 1);

    വേണ്ടി (i = 0; i < n; i++) {
        എങ്കിൽ (ngx_queue_empty(&cycle->reusable_connections_queue)) {
            നിർത്തുക;
        }

        q = ngx_queue_last(&cycle->reusable_connections_queue);
        c = ngx_queue_data(q, ngx_connection_t, queue);

        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "reusing connection");

        c->close = 1;
        c->പഠ->handler(c->പഠ);
    }

    എങ്കിൽ (cycle->free_connection_n == 0 && c && c->reusable) {

        /*
         * if no connections were freed, try to reuse the last
         * connection again: this should free it as long as
         * previous reuse moved it to lingering close
         */

        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "reusing connection again");

        c->close = 1;
        c->പഠ->handler(c->പഠ);
    }
}


ശൂന്യം
ngx_close_idle_connections(ngx_cycle_t *cycle)
{
    ngx_uint_t         i;
    ngx_connection_t  *c;

    c = cycle->connections;

    വേണ്ടി (i = 0; i < cycle->connection_n; i++) {

        /* THREAD: lock */

        എങ്കിൽ (c[i].fd != (ngx_socket_t) -1 && c[i].idle) {
            c[i].close = 1;
            c[i].പഠ->handler(c[i].പഠ);
        }
    }
}


ngx_int_t
ngx_connection_local_sockaddr(ngx_connection_t *c, ngx_str_t *s,
    ngx_uint_t port)
{
    socklen_t             len;
    ngx_uint_t            addr;
    ngx_sockaddr_t        sa;
    ഘടന sockaddr_in   *sin;
#എങ്കിൽ (NGX_HAVE_INET6)
    ngx_uint_t            i;
    ഘടന sockaddr_in6  *sin6;
#endif

    addr = 0;

    എങ്കിൽ (c->local_socklen) {
        തിരഞ്ഞെടുപ്പ് (c->local_sockaddr->sa_family) {

#എങ്കിൽ (NGX_HAVE_INET6)
        അവസ്ഥ AF_INET6:
            sin6 = (ഘടന sockaddr_in6 *) c->local_sockaddr;

            വേണ്ടി (i = 0; addr == 0 && i < 16; i++) {
                addr |= sin6->sin6_addr.s6_addr[i];
            }

            നിർത്തുക;
#endif

#എങ്കിൽ (NGX_HAVE_UNIX_DOMAIN)
        അവസ്ഥ AF_UNIX:
            addr = 1;
            നിർത്തുക;
#endif

        സ്ഥിരസ്ഥിതി: /* AF_INET */
            sin = (ഘടന sockaddr_in *) c->local_sockaddr;
            addr = sin->sin_addr.s_addr;
            നിർത്തുക;
        }
    }

    എങ്കിൽ (addr == 0) {

        len = വലുപ്പം(ngx_sockaddr_t);

        എങ്കിൽ (getsockname(c->fd, &sa.sockaddr, &len) == -1) {
            ngx_connection_error(c, ngx_socket_errno, "getsockname() failed");
            തിരികെ NGX_ERROR;
        }

        c->local_sockaddr = ngx_palloc(c->pool, len);
        എങ്കിൽ (c->local_sockaddr == ശൂന്യ) {
            തിരികെ NGX_ERROR;
        }

        ngx_memcpy(c->local_sockaddr, &sa, len);

        c->local_socklen = len;
    }

    എങ്കിൽ (s == ശൂന്യ) {
        തിരികെ NGX_OK;
    }

    s->len = ngx_sock_ntop(c->local_sockaddr, c->local_socklen,
                           s->data, s->len, port);

    തിരികെ NGX_OK;
}


ngx_int_t
ngx_tcp_nodelay(ngx_connection_t *c)
{
    പൂർണ്ണസംഖ്യ  tcp_nodelay;

    എങ്കിൽ (c->tcp_nodelay != NGX_TCP_NODELAY_UNSET) {
        തിരികെ NGX_OK;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0, "tcp_nodelay");

    tcp_nodelay = 1;

    എങ്കിൽ (setsockopt(c->fd, IPPROTO_TCP, TCP_NODELAY,
                   (സ്ഥിരം ശൂന്യം *) &tcp_nodelay, വലുപ്പം(പൂർണ്ണസംഖ്യ))
        == -1)
    {
#എങ്കിൽ (NGX_SOLARIS)
        എങ്കിൽ (c->log_error == NGX_ERROR_INFO) {

            /* Solaris returns EINVAL if a socket has been shut down */
            c->log_error = NGX_ERROR_IGNORE_EINVAL;

            ngx_connection_error(c, ngx_socket_errno,
                                 "setsockopt(TCP_NODELAY) failed");

            c->log_error = NGX_ERROR_INFO;

            തിരികെ NGX_ERROR;
        }
#endif

        ngx_connection_error(c, ngx_socket_errno,
                             "setsockopt(TCP_NODELAY) failed");
        തിരികെ NGX_ERROR;
    }

    c->tcp_nodelay = NGX_TCP_NODELAY_SET;

    തിരികെ NGX_OK;
}


ngx_int_t
ngx_connection_error(ngx_connection_t *c, ngx_err_t err, അക്ഷരം *text)
{
    ngx_uint_t  level;

    /* Winsock may return NGX_ECONNABORTED instead of NGX_ECONNRESET */

    എങ്കിൽ ((err == NGX_ECONNRESET
#എങ്കിൽ (NGX_WIN32)
         || err == NGX_ECONNABORTED
#endif
        ) && c->log_error == NGX_ERROR_IGNORE_ECONNRESET)
    {
        തിരികെ 0;
    }

#എങ്കിൽ (NGX_SOLARIS)
    എങ്കിൽ (err == NGX_EINVAL && c->log_error == NGX_ERROR_IGNORE_EINVAL) {
        തിരികെ 0;
    }
#endif

    എങ്കിൽ (err == NGX_EMSGSIZE && c->log_error == NGX_ERROR_IGNORE_EMSGSIZE) {
        തിരികെ 0;
    }

#എങ്കിൽ (NGX_LINUX)
    എങ്കിൽ (err == NGX_EIO && c->ktls_recv) {
        തിരികെ 0;
    }
#endif

    എങ്കിൽ (err == 0
        || err == NGX_ECONNRESET
#എങ്കിൽ (NGX_WIN32)
        || err == NGX_ECONNABORTED
#അല്ലെങ്കിൽ
        || err == NGX_EPIPE
#endif
        || err == NGX_ENOTCONN
        || err == NGX_ETIMEDOUT
        || err == NGX_ECONNREFUSED
        || err == NGX_ENETDOWN
        || err == NGX_ENETUNREACH
        || err == NGX_EHOSTDOWN
        || err == NGX_EHOSTUNREACH)
    {
        തിരഞ്ഞെടുപ്പ് (c->log_error) {

        അവസ്ഥ NGX_ERROR_IGNORE_EMSGSIZE:
        അവസ്ഥ NGX_ERROR_IGNORE_EINVAL:
        അവസ്ഥ NGX_ERROR_IGNORE_ECONNRESET:
        അവസ്ഥ NGX_ERROR_INFO:
            level = NGX_LOG_INFO;
            നിർത്തുക;

        സ്ഥിരസ്ഥിതി:
            level = NGX_LOG_ERR;
        }

    } അല്ലെങ്കിൽ {
        level = NGX_LOG_ALERT;
    }

    ngx_log_error(level, c->log, err, text);

    തിരികെ NGX_ERROR;
}
