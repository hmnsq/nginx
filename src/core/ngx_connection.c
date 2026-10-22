
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>
#ଅନ୍ତର୍ଭାଵ <ngx_event.h>


ngx_os_io_t  ngx_io;


ସ୍ଥିର ଶୂନ୍ୟ ngx_drain_connections(ngx_cycle_t *cycle);


ngx_listening_t *
ngx_create_listening(ngx_conf_t *cf, ସଂରଚନା sockaddr *sockaddr,
    socklen_t socklen)
{
    size_t            len;
    ngx_listening_t  *ls;
    ସଂରଚନା sockaddr  *sa;
    u_char            text[NGX_SOCKADDR_STRLEN];

    ls = ngx_array_ଠେଲ(&cf->cycle->listening);
    ଯଦି (ls == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ngx_memzero(ls, ଆକାର(ngx_listening_t));

    sa = ngx_palloc(cf->pool, socklen);
    ଯଦି (sa == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ngx_memcpy(sa, sockaddr, socklen);

    ls->sockaddr = sa;
    ls->socklen = socklen;

    len = ngx_sock_ntop(sa, socklen, text, NGX_SOCKADDR_STRLEN, 1);
    ls->addr_text.len = len;

    ବାଛ (ls->sockaddr->sa_family) {
#ଯଦି (NGX_HAVE_INET6)
    ସ୍ଥିତି AF_INET6:
        ls->addr_text_max_len = NGX_INET6_ADDRSTRLEN;
        ଭାଙ୍ଗ;
#endif
#ଯଦି (NGX_HAVE_UNIX_DOMAIN)
    ସ୍ଥିତି AF_UNIX:
        ls->addr_text_max_len = NGX_UNIX_ADDRSTRLEN;
        len++;
        ଭାଙ୍ଗ;
#endif
    ସ୍ଥିତି AF_INET:
        ls->addr_text_max_len = NGX_INET_ADDRSTRLEN;
        ଭାଙ୍ଗ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ls->addr_text_max_len = NGX_SOCKADDR_STRLEN;
        ଭାଙ୍ଗ;
    }

    ls->addr_text.data = ngx_pnalloc(cf->pool, len);
    ଯଦି (ls->addr_text.data == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ngx_memcpy(ls->addr_text.data, text, len);

#ଯଦି !(NGX_WIN32)
    ngx_rbtree_init(&ls->rbtree, &ls->sentinel, ngx_udp_rbtree_ଭର୍ତ୍ତି_ମୂଲ୍ୟ);
#endif

    ls->fd = (ngx_socket_t) -1;
    ls->type = SOCK_STREAM;

    ls->backlog = NGX_LISTEN_BACKLOG;
    ls->rcvbuf = -1;
    ls->sndbuf = -1;

#ଯଦି (NGX_HAVE_SETFIB)
    ls->setfib = -1;
#endif

#ଯଦି (NGX_HAVE_TCP_FASTOPEN)
    ls->fastopen = -1;
#endif

    ଫେରନ୍ତୁ ls;
}


ngx_int_t
ngx_clone_listening(ngx_cycle_t *cycle, ngx_listening_t *ls)
{
#ଯଦି (NGX_HAVE_REUSEPORT)

    ngx_int_t         n;
    ngx_core_conf_t  *ccf;
    ngx_listening_t   ols;

    ଯଦି (!ls->reuseport || ls->worker != 0) {
        ଫେରନ୍ତୁ NGX_OK;
    }

    ols = *ls;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    ପାଇଁ (n = 1; n < ccf->worker_processes; n++) {

        /* create a socket for each worker process */

        ls = ngx_array_ଠେଲ(&cycle->listening);
        ଯଦି (ls == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        *ls = ols;
        ls->worker = n;
    }

#endif

    ଫେରନ୍ତୁ NGX_OK;
}


ngx_int_t
ngx_set_inherited_sockets(ngx_cycle_t *cycle)
{
    size_t                     len;
    ngx_uint_t                 i;
    ngx_listening_t           *ls;
    socklen_t                  olen;
#ଯଦି (NGX_HAVE_DEFERRED_ACCEPT || NGX_HAVE_TCP_FASTOPEN)
    ngx_err_t                  err;
#endif
#ଯଦି (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    ସଂରଚନା accept_filter_arg   af;
#endif
#ଯଦି (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା                        timeout;
#endif
#ଯଦି (NGX_HAVE_REUSEPORT)
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା                        reuseport;
#endif

    ls = cycle->listening.elts;
    ପାଇଁ (i = 0; i < cycle->listening.nelts; i++) {

        ls[i].sockaddr = ngx_palloc(cycle->pool, ଆକାର(ngx_sockaddr_t));
        ଯଦି (ls[i].sockaddr == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ls[i].socklen = ଆକାର(ngx_sockaddr_t);
        ଯଦି (getsockname(ls[i].fd, ls[i].sockaddr, &ls[i].socklen) == -1) {
            ngx_log_error(NGX_LOG_CRIT, cycle->log, ngx_socket_errno,
                          "getsockname() of the inherited "
                          "socket #%d failed", ls[i].fd);
            ls[i].ignore = 1;
            ଜାରି;
        }

        ଯଦି (ls[i].socklen > (socklen_t) ଆକାର(ngx_sockaddr_t)) {
            ls[i].socklen = ଆକାର(ngx_sockaddr_t);
        }

        ବାଛ (ls[i].sockaddr->sa_family) {

#ଯଦି (NGX_HAVE_INET6)
        ସ୍ଥିତି AF_INET6:
            ls[i].addr_text_max_len = NGX_INET6_ADDRSTRLEN;
            len = NGX_INET6_ADDRSTRLEN + ଆକାର("[]:65535") - 1;
            ଭାଙ୍ଗ;
#endif

#ଯଦି (NGX_HAVE_UNIX_DOMAIN)
        ସ୍ଥିତି AF_UNIX:
            ls[i].addr_text_max_len = NGX_UNIX_ADDRSTRLEN;
            len = NGX_UNIX_ADDRSTRLEN;
            ଭାଙ୍ଗ;
#endif

        ସ୍ଥିତି AF_INET:
            ls[i].addr_text_max_len = NGX_INET_ADDRSTRLEN;
            len = NGX_INET_ADDRSTRLEN + ଆକାର(":65535") - 1;
            ଭାଙ୍ଗ;

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            ngx_log_error(NGX_LOG_CRIT, cycle->log, ngx_socket_errno,
                          "the inherited socket #%d has "
                          "an unsupported protocol family", ls[i].fd);
            ls[i].ignore = 1;
            ଜାରି;
        }

        ls[i].addr_text.data = ngx_pnalloc(cycle->pool, len);
        ଯଦି (ls[i].addr_text.data == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        len = ngx_sock_ntop(ls[i].sockaddr, ls[i].socklen,
                            ls[i].addr_text.data, len, 1);
        ଯଦି (len == 0) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ls[i].addr_text.len = len;

        ls[i].backlog = NGX_LISTEN_BACKLOG;

        olen = ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା);

        ଯଦି (getsockopt(ls[i].fd, SOL_SOCKET, SO_TYPE, (ଶୂନ୍ୟ *) &ls[i].type,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_CRIT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_TYPE) %V failed", &ls[i].addr_text);
            ls[i].ignore = 1;
            ଜାରି;
        }

        olen = ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା);

        ଯଦି (getsockopt(ls[i].fd, SOL_SOCKET, SO_RCVBUF, (ଶୂନ୍ୟ *) &ls[i].rcvbuf,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_RCVBUF) %V failed, ignored",
                          &ls[i].addr_text);

            ls[i].rcvbuf = -1;
        }

        olen = ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା);

        ଯଦି (getsockopt(ls[i].fd, SOL_SOCKET, SO_SNDBUF, (ଶୂନ୍ୟ *) &ls[i].sndbuf,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_SNDBUF) %V failed, ignored",
                          &ls[i].addr_text);

            ls[i].sndbuf = -1;
        }

#ଯଦି 0
        /* SO_SETFIB is currently a set only option */

#ଯଦି (NGX_HAVE_SETFIB)

        olen = ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା);

        ଯଦି (getsockopt(ls[i].fd, SOL_SOCKET, SO_SETFIB,
                       (ଶୂନ୍ୟ *) &ls[i].setfib, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_SETFIB) %V failed, ignored",
                          &ls[i].addr_text);

            ls[i].setfib = -1;
        }

#endif
#endif

#ଯଦି (NGX_HAVE_REUSEPORT)

        reuseport = 0;
        olen = ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା);

#ifdef SO_REUSEPORT_LB

        ଯଦି (getsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT_LB,
                       (ଶୂନ୍ୟ *) &reuseport, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_REUSEPORT_LB) %V failed, ignored",
                          &ls[i].addr_text);

        } ଅନ୍ୟଥା {
            ls[i].reuseport = reuseport ? 1 : 0;
        }

#ଅନ୍ୟଥା

        ଯଦି (getsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT,
                       (ଶୂନ୍ୟ *) &reuseport, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_REUSEPORT) %V failed, ignored",
                          &ls[i].addr_text);

        } ଅନ୍ୟଥା {
            ls[i].reuseport = reuseport ? 1 : 0;
        }
#endif

#endif

        ଯଦି (ls[i].type != SOCK_STREAM) {
            ଜାରି;
        }

#ifdef SO_PROTOCOL

        olen = ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା);

        ଯଦି (getsockopt(ls[i].fd, SOL_SOCKET, SO_PROTOCOL,
                       (ଶୂନ୍ୟ *) &ls[i].protocol, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_PROTOCOL) %V failed, ignored",
                          &ls[i].addr_text);
            ls[i].protocol = 0;

        } ଅନ୍ୟଥା ଯଦି (ls[i].protocol == IPPROTO_TCP) {
            ls[i].protocol = 0;
        }

#endif

#ଯଦି (NGX_HAVE_TCP_FASTOPEN)

        olen = ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା);

        ଯଦି (getsockopt(ls[i].fd, IPPROTO_TCP, TCP_FASTOPEN,
                       (ଶୂନ୍ୟ *) &ls[i].fastopen, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            ଯଦି (err != NGX_EOPNOTSUPP && err != NGX_ENOPROTOOPT
                && err != NGX_EINVAL)
            {
                ngx_log_error(NGX_LOG_NOTICE, cycle->log, err,
                              "getsockopt(TCP_FASTOPEN) %V failed, ignored",
                              &ls[i].addr_text);
            }

            ls[i].fastopen = -1;
        }

#endif

#ଯଦି (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)

        ngx_memzero(&af, ଆକାର(ସଂରଚନା accept_filter_arg));
        olen = ଆକାର(ସଂରଚନା accept_filter_arg);

        ଯଦି (getsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER, &af, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            ଯଦି (err == NGX_EINVAL) {
                ଜାରି;
            }

            ngx_log_error(NGX_LOG_NOTICE, cycle->log, err,
                          "getsockopt(SO_ACCEPTFILTER) for %V failed, ignored",
                          &ls[i].addr_text);
            ଜାରି;
        }

        ଯଦି (olen < ଆକାର(ସଂରଚନା accept_filter_arg) || af.af_name[0] == '\0') {
            ଜାରି;
        }

        ls[i].accept_filter = ngx_palloc(cycle->pool, 16);
        ଯଦି (ls[i].accept_filter == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        (ଶୂନ୍ୟ) ngx_cpystrn((u_char *) ls[i].accept_filter,
                           (u_char *) af.af_name, 16);
#endif

#ଯଦି (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)

        timeout = 0;
        olen = ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା);

        ଯଦି (getsockopt(ls[i].fd, IPPROTO_TCP, TCP_DEFER_ACCEPT, &timeout, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            ଯଦି (err == NGX_EOPNOTSUPP) {
                ଜାରି;
            }

            ngx_log_error(NGX_LOG_NOTICE, cycle->log, err,
                          "getsockopt(TCP_DEFER_ACCEPT) for %V failed, ignored",
                          &ls[i].addr_text);
            ଜାରି;
        }

        ଯଦି (olen < ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା) || timeout == 0) {
            ଜାରି;
        }

        ls[i].deferred_accept = 1;
#endif
    }

    ଫେରନ୍ତୁ NGX_OK;
}


ngx_int_t
ngx_open_listening_sockets(ngx_cycle_t *cycle)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା               reuseaddr;
    ngx_uint_t        i, tries, failed;
    ngx_err_t         err;
    ngx_log_t        *log;
    ngx_socket_t      s;
    ngx_listening_t  *ls;

    reuseaddr = 1;
#ଯଦି (NGX_SUPPRESS_WARN)
    failed = 0;
#endif

    log = cycle->log;

    /* TODO: configurable try number */

    ପାଇଁ (tries = 5; tries; tries--) {
        failed = 0;

        /* for each listening socket */

        ls = cycle->listening.elts;
        ପାଇଁ (i = 0; i < cycle->listening.nelts; i++) {

            ଯଦି (ls[i].ignore) {
                ଜାରି;
            }

#ଯଦି (NGX_HAVE_REUSEPORT)

            ଯଦି (ls[i].add_reuseport || ls[i].change_protocol) {

                /*
                 * to allow transition from a socket without SO_REUSEPORT
                 * to multiple sockets with SO_REUSEPORT, we have to set
                 * SO_REUSEPORT on the old socket before opening new ones
                 *
                 * to allow transition between different socket protocols
                 * (e.g. IPPROTO_MPTCP), SO_REUSEPORT is set on both old
                 * and new sockets
                 */

                ପୂର୍ଣ୍ଣସଂଖ୍ୟା  reuseport = 1;

#ifdef SO_REUSEPORT_LB

                ଯଦି (setsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT_LB,
                               (ଅଚଳ ଶୂନ୍ୟ *) &reuseport, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT_LB) %V failed, "
                                  "ignored",
                                  &ls[i].addr_text);
                }

#ଅନ୍ୟଥା

                ଯଦି (setsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT,
                               (ଅଚଳ ଶୂନ୍ୟ *) &reuseport, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା))
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

            ଯଦି (ls[i].fd != (ngx_socket_t) -1 && !ls[i].change_protocol) {
                ଜାରି;
            }

            ଯଦି (ls[i].inherited) {

                /* TODO: close on exit */
                /* TODO: nonblocking */
                /* TODO: deferred accept */

                ଜାରି;
            }

            s = ngx_socket(ls[i].sockaddr->sa_family, ls[i].type,
                           ls[i].protocol);

            ଯଦି (s == (ngx_socket_t) -1) {
                ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                              ngx_socket_n " %V failed", &ls[i].addr_text);
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            ଯଦି (ls[i].type != SOCK_DGRAM || !ngx_test_config) {

                ଯଦି (setsockopt(s, SOL_SOCKET, SO_REUSEADDR,
                               (ଅଚଳ ଶୂନ୍ୟ *) &reuseaddr, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEADDR) %V failed",
                                  &ls[i].addr_text);

                    ଯଦି (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_text);
                    }

                    ଫେରନ୍ତୁ NGX_ERROR;
                }
            }

#ଯଦି (NGX_HAVE_REUSEPORT)

            ଯଦି ((ls[i].reuseport || ls[i].change_protocol)
                && !ngx_test_config)
            {
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା  reuseport;

                reuseport = 1;

#ifdef SO_REUSEPORT_LB

                ଯଦି (setsockopt(s, SOL_SOCKET, SO_REUSEPORT_LB,
                               (ଅଚଳ ଶୂନ୍ୟ *) &reuseport, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT_LB) %V failed",
                                  &ls[i].addr_text);

                    ଯଦି (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_text);
                    }

                    ଫେରନ୍ତୁ NGX_ERROR;
                }

#ଅନ୍ୟଥା

                ଯଦି (setsockopt(s, SOL_SOCKET, SO_REUSEPORT,
                               (ଅଚଳ ଶୂନ୍ୟ *) &reuseport, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT) %V failed",
                                  &ls[i].addr_text);

                    ଯଦି (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_text);
                    }

                    ଫେରନ୍ତୁ NGX_ERROR;
                }
#endif
            }
#endif

#ଯଦି (NGX_HAVE_INET6 && defined IPV6_V6ONLY)

            ଯଦି (ls[i].sockaddr->sa_family == AF_INET6) {
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା  ipv6only;

                ipv6only = ls[i].ipv6only;

                ଯଦି (setsockopt(s, IPPROTO_IPV6, IPV6_V6ONLY,
                               (ଅଚଳ ଶୂନ୍ୟ *) &ipv6only, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(IPV6_V6ONLY) %V failed, ignored",
                                  &ls[i].addr_text);
                }
            }
#endif
            /* TODO: close on exit */

            ଯଦି (ngx_nonblocking(s) == -1) {
                ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                              ngx_nonblocking_n " %V failed",
                              &ls[i].addr_text);

                ଯଦି (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_text);
                }

                ଫେରନ୍ତୁ NGX_ERROR;
            }

            ngx_log_debug2(NGX_LOG_DEBUG_CORE, log, 0,
                           "bind() %V #%d ", &ls[i].addr_text, s);

            ଯଦି (bind(s, ls[i].sockaddr, ls[i].socklen) == -1) {
                err = ngx_socket_errno;

                ଯଦି (err != NGX_EADDRINUSE || !ngx_test_config) {
                    ngx_log_error(NGX_LOG_EMERG, log, err,
                                  "bind() to %V failed", &ls[i].addr_text);
                }

                ଯଦି (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_text);
                }

                ଯଦି (err != NGX_EADDRINUSE) {
                    ଫେରନ୍ତୁ NGX_ERROR;
                }

                ଯଦି (!ngx_test_config) {
                    failed = 1;
                }

                ଜାରି;
            }

#ଯଦି (NGX_HAVE_UNIX_DOMAIN)

            ଯଦି (ls[i].sockaddr->sa_family == AF_UNIX) {
                mode_t   mode;
                u_char  *name;

                name = ls[i].addr_text.data + ଆକାର("unix:") - 1;
                mode = (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);

                ଯଦି (chmod((ଅକ୍ଷର *) name, mode) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  "chmod() \"%s\" failed", name);
                }

                ଯଦି (ngx_test_config) {
                    ଯଦି (ngx_delete_file(name) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                      ngx_delete_file_n " %s failed", name);
                    }
                }
            }
#endif

            ଯଦି (ls[i].type != SOCK_STREAM) {
                ls[i].fd = s;
                ls[i].open = 1;
                ଜାରି;
            }

            ଯଦି (listen(s, ls[i].backlog) == -1) {
                err = ngx_socket_errno;

                /*
                 * on OpenVZ after suspend/resume EADDRINUSE
                 * may be returned by listen() instead of bind(), see
                 * https://bugs.openvz.org/browse/OVZ-5587
                 */

                ଯଦି (err != NGX_EADDRINUSE || !ngx_test_config) {
                    ngx_log_error(NGX_LOG_EMERG, log, err,
                                  "listen() to %V, backlog %d failed",
                                  &ls[i].addr_text, ls[i].backlog);
                }

                ଯଦି (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_text);
                }

                ଯଦି (err != NGX_EADDRINUSE) {
                    ଫେରନ୍ତୁ NGX_ERROR;
                }

                ଯଦି (!ngx_test_config) {
                    failed = 1;
                }

                ଜାରି;
            }

            ls[i].listen = 1;

            ls[i].fd = s;
            ls[i].open = 1;
        }

        ଯଦି (!failed) {
            ଭାଙ୍ଗ;
        }

        /* TODO: delay configurable */

        ngx_log_error(NGX_LOG_NOTICE, log, 0,
                      "try again to bind() after 500ms");

        ngx_msleep(500);
    }

    ଯଦି (failed) {
        ngx_log_error(NGX_LOG_EMERG, log, 0, "still could not bind()");
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ଫେରନ୍ତୁ NGX_OK;
}


ଶୂନ୍ୟ
ngx_configure_listening_sockets(ngx_cycle_t *cycle)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା                        ମୂଲ୍ୟ;
    ngx_uint_t                 i;
    ngx_listening_t           *ls;

#ଯଦି (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    ସଂରଚନା accept_filter_arg   af;
#endif

    ls = cycle->listening.elts;
    ପାଇଁ (i = 0; i < cycle->listening.nelts; i++) {

        ls[i].log = *ls[i].logp;

        ଯଦି (ls[i].rcvbuf != -1) {
            ଯଦି (setsockopt(ls[i].fd, SOL_SOCKET, SO_RCVBUF,
                           (ଅଚଳ ଶୂନ୍ୟ *) &ls[i].rcvbuf, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_RCVBUF, %d) %V failed, ignored",
                              ls[i].rcvbuf, &ls[i].addr_text);
            }
        }

        ଯଦି (ls[i].sndbuf != -1) {
            ଯଦି (setsockopt(ls[i].fd, SOL_SOCKET, SO_SNDBUF,
                           (ଅଚଳ ଶୂନ୍ୟ *) &ls[i].sndbuf, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_SNDBUF, %d) %V failed, ignored",
                              ls[i].sndbuf, &ls[i].addr_text);
            }
        }

        ଯଦି (ls[i].keepalive) {
            ମୂଲ୍ୟ = (ls[i].keepalive == 1) ? 1 : 0;

            ଯଦି (setsockopt(ls[i].fd, SOL_SOCKET, SO_KEEPALIVE,
                           (ଅଚଳ ଶୂନ୍ୟ *) &ମୂଲ୍ୟ, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_KEEPALIVE, %d) %V failed, ignored",
                              ମୂଲ୍ୟ, &ls[i].addr_text);
            }
        }

#ଯଦି (NGX_HAVE_KEEPALIVE_TUNABLE)

#ଯଦି !(NGX_DARWIN)

        ଯଦି (ls[i].keepidle) {
            ମୂଲ୍ୟ = ls[i].keepidle;

#ଯଦି (NGX_KEEPALIVE_FACTOR)
            ମୂଲ୍ୟ *= NGX_KEEPALIVE_FACTOR;
#endif

            ଯଦି (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPIDLE,
                           (ଅଚଳ ଶୂନ୍ୟ *) &ମୂଲ୍ୟ, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_KEEPIDLE, %d) %V failed, ignored",
                              ମୂଲ୍ୟ, &ls[i].addr_text);
            }
        }

#endif

        ଯଦି (ls[i].keepintvl) {
            ମୂଲ୍ୟ = ls[i].keepintvl;

#ଯଦି (NGX_KEEPALIVE_FACTOR)
            ମୂଲ୍ୟ *= NGX_KEEPALIVE_FACTOR;
#endif

            ଯଦି (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPINTVL,
                           (ଅଚଳ ଶୂନ୍ୟ *) &ମୂଲ୍ୟ, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                             "setsockopt(TCP_KEEPINTVL, %d) %V failed, ignored",
                             ମୂଲ୍ୟ, &ls[i].addr_text);
            }
        }

        ଯଦି (ls[i].keepcnt) {
            ଯଦି (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPCNT,
                           (ଅଚଳ ଶୂନ୍ୟ *) &ls[i].keepcnt, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_KEEPCNT, %d) %V failed, ignored",
                              ls[i].keepcnt, &ls[i].addr_text);
            }
        }

#endif

#ଯଦି (NGX_HAVE_SETFIB)
        ଯଦି (ls[i].setfib != -1) {
            ଯଦି (setsockopt(ls[i].fd, SOL_SOCKET, SO_SETFIB,
                           (ଅଚଳ ଶୂନ୍ୟ *) &ls[i].setfib, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_SETFIB, %d) %V failed, ignored",
                              ls[i].setfib, &ls[i].addr_text);
            }
        }
#endif

#ଯଦି (NGX_HAVE_TCP_FASTOPEN)
        ଯଦି (ls[i].fastopen != -1) {
            ଯଦି (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_FASTOPEN,
                           (ଅଚଳ ଶୂନ୍ୟ *) &ls[i].fastopen, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_FASTOPEN, %d) %V failed, ignored",
                              ls[i].fastopen, &ls[i].addr_text);
            }
        }
#endif

#ଯଦି 0
        ଯଦି (1) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା tcp_nodelay = 1;

            ଯଦି (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_NODELAY,
                       (ଅଚଳ ଶୂନ୍ୟ *) &tcp_nodelay, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_NODELAY) %V failed, ignored",
                              &ls[i].addr_text);
            }
        }
#endif

        ଯଦି (ls[i].listen) {

            /* change backlog via listen() */

            ଯଦି (listen(ls[i].fd, ls[i].backlog) == -1) {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "listen() to %V, backlog %d failed, ignored",
                              &ls[i].addr_text, ls[i].backlog);
            }
        }

        /*
         * setting deferred mode should be last operation on socket,
         * because code may prematurely continue cycle on failure
         */

#ଯଦି (NGX_HAVE_DEFERRED_ACCEPT)

#ifdef SO_ACCEPTFILTER

        ଯଦି (ls[i].delete_deferred) {
            ଯଦି (setsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER, ଖାଲି, 0)
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_ACCEPTFILTER, NULL) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);

                ଯଦି (ls[i].accept_filter) {
                    ngx_log_error(NGX_LOG_ALERT, cycle->log, 0,
                                  "could not change the accept filter "
                                  "to \"%s\" for %V, ignored",
                                  ls[i].accept_filter, &ls[i].addr_text);
                }

                ଜାରି;
            }

            ls[i].deferred_accept = 0;
        }

        ଯଦି (ls[i].add_deferred) {
            ngx_memzero(&af, ଆକାର(ସଂରଚନା accept_filter_arg));
            (ଶୂନ୍ୟ) ngx_cpystrn((u_char *) af.af_name,
                               (u_char *) ls[i].accept_filter, 16);

            ଯଦି (setsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER,
                           &af, ଆକାର(ସଂରଚନା accept_filter_arg))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_ACCEPTFILTER, \"%s\") "
                              "for %V failed, ignored",
                              ls[i].accept_filter, &ls[i].addr_text);
                ଜାରି;
            }

            ls[i].deferred_accept = 1;
        }

#endif

#ifdef TCP_DEFER_ACCEPT

        ଯଦି (ls[i].add_deferred || ls[i].delete_deferred) {

            ଯଦି (ls[i].add_deferred) {
                /*
                 * There is no way to find out how long a connection was
                 * in queue (and a connection may bypass deferred queue at all
                 * if syncookies were used), hence we use 1 second timeout
                 * here.
                 */
                ମୂଲ୍ୟ = 1;

            } ଅନ୍ୟଥା {
                ମୂଲ୍ୟ = 0;
            }

            ଯଦି (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_DEFER_ACCEPT,
                           &ମୂଲ୍ୟ, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_DEFER_ACCEPT, %d) for %V failed, "
                              "ignored",
                              ମୂଲ୍ୟ, &ls[i].addr_text);

                ଜାରି;
            }
        }

        ଯଦି (ls[i].add_deferred) {
            ls[i].deferred_accept = 1;
        }

#endif

#endif /* NGX_HAVE_DEFERRED_ACCEPT */

#ଯଦି (NGX_HAVE_IP_RECVDSTADDR)

        ଯଦି (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET)
        {
            ମୂଲ୍ୟ = 1;

            ଯଦି (setsockopt(ls[i].fd, IPPROTO_IP, IP_RECVDSTADDR,
                           (ଅଚଳ ଶୂନ୍ୟ *) &ମୂଲ୍ୟ, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_RECVDSTADDR) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#ନଚେତଯଦି (NGX_HAVE_IP_PKTINFO)

        ଯଦି (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET)
        {
            ମୂଲ୍ୟ = 1;

            ଯଦି (setsockopt(ls[i].fd, IPPROTO_IP, IP_PKTINFO,
                           (ଅଚଳ ଶୂନ୍ୟ *) &ମୂଲ୍ୟ, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_PKTINFO) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#endif

#ଯଦି (NGX_HAVE_INET6 && NGX_HAVE_IPV6_RECVPKTINFO)

        ଯଦି (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET6)
        {
            ମୂଲ୍ୟ = 1;

            ଯଦି (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_RECVPKTINFO,
                           (ଅଚଳ ଶୂନ୍ୟ *) &ମୂଲ୍ୟ, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IPV6_RECVPKTINFO) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#endif

#ଯଦି (NGX_HAVE_IP_MTU_DISCOVER)

        ଯଦି (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET) {
            ମୂଲ୍ୟ = IP_PMTUDISC_DO;

            ଯଦି (setsockopt(ls[i].fd, IPPROTO_IP, IP_MTU_DISCOVER,
                           (ଅଚଳ ଶୂନ୍ୟ *) &ମୂଲ୍ୟ, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_MTU_DISCOVER) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#ନଚେତଯଦି (NGX_HAVE_IP_DONTFRAG)

        ଯଦି (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET) {
            ମୂଲ୍ୟ = 1;

            ଯଦି (setsockopt(ls[i].fd, IPPROTO_IP, IP_DONTFRAG,
                           (ଅଚଳ ଶୂନ୍ୟ *) &ମୂଲ୍ୟ, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_DONTFRAG) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#endif

#ଯଦି (NGX_HAVE_INET6)

#ଯଦି (NGX_HAVE_IPV6_MTU_DISCOVER)

        ଯଦି (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET6) {
            ମୂଲ୍ୟ = IPV6_PMTUDISC_DO;

            ଯଦି (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_MTU_DISCOVER,
                           (ଅଚଳ ଶୂନ୍ୟ *) &ମୂଲ୍ୟ, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IPV6_MTU_DISCOVER) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#ନଚେତଯଦି (NGX_HAVE_IPV6_DONTFRAG)

        ଯଦି (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET6) {
            ମୂଲ୍ୟ = 1;

            ଯଦି (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_DONTFRAG,
                           (ଅଚଳ ଶୂନ୍ୟ *) &ମୂଲ୍ୟ, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା))
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

    ଫେରନ୍ତୁ;
}


ଶୂନ୍ୟ
ngx_close_listening_sockets(ngx_cycle_t *cycle)
{
    ngx_uint_t         i;
    ngx_listening_t   *ls;
    ngx_connection_t  *c;

    ngx_accept_mutex_held = 0;
    ngx_use_accept_mutex = 0;

    ls = cycle->listening.elts;
    ପାଇଁ (i = 0; i < cycle->listening.nelts; i++) {

#ଯଦି (NGX_QUIC)
        ଯଦି (ls[i].quic
            && (!ls[i].reuseport || !ngx_quic_bpf_enabled(cycle)))
        {
            ଜାରି;
        }
#endif

        c = ls[i].connection;

        ଯଦି (c) {
            ଯଦି (c->ପଠ->active) {
                ଯଦି (ngx_event_flags & NGX_USE_EPOLL_EVENT) {

                    /*
                     * it seems that Linux-2.6.x OpenVZ sends events
                     * for closed shared listening sockets unless
                     * the events was explicitly deleted
                     */

                    ngx_del_event(c->ପଠ, NGX_READ_EVENT, 0);

                } ଅନ୍ୟଥା {
                    ngx_del_event(c->ପଠ, NGX_READ_EVENT, NGX_CLOSE_EVENT);
                }
            }

            ଯଦି (c->ପଠ->timer_set) {
                ngx_del_timer(c->ପଠ);
            }

            ngx_free_connection(c);

            c->fd = (ngx_socket_t) -1;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, cycle->log, 0,
                       "close listening %V #%d ", &ls[i].addr_text, ls[i].fd);

        ଯଦି (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_socket_errno,
                          ngx_close_socket_n " %V failed", &ls[i].addr_text);
        }

#ଯଦି (NGX_HAVE_UNIX_DOMAIN)

        ଯଦି (ls[i].sockaddr->sa_family == AF_UNIX
            && ngx_process <= NGX_PROCESS_MASTER
            && ngx_new_ଦ୍ୱିମିକ == 0
            && (!ls[i].inherited || ngx_getppid() != ngx_parent))
        {
            u_char *name = ls[i].addr_text.data + ଆକାର("unix:") - 1;

            ଯଦି (ngx_delete_file(name) == NGX_FILE_ERROR) {
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

    ଯଦି (ngx_cycle->files && (ngx_uint_t) s >= ngx_cycle->files_n) {
        ngx_log_error(NGX_LOG_ALERT, log, 0,
                      "the new socket has number %d, "
                      "but only %ui files are available",
                      s, ngx_cycle->files_n);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ngx_drain_connections((ngx_cycle_t *) ngx_cycle);

    c = ngx_cycle->free_connections;

    ଯଦି (c == ଖାଲି) {
        ngx_log_error(NGX_LOG_ALERT, log, 0,
                      "%ui worker_connections are not enough",
                      ngx_cycle->connection_n);

        ଫେରନ୍ତୁ ଖାଲି;
    }

    ngx_cycle->free_connections = c->data;
    ngx_cycle->free_connection_n--;

    ଯଦି (ngx_cycle->files && ngx_cycle->files[s] == ଖାଲି) {
        ngx_cycle->files[s] = c;
    }

    rev = c->ପଠ;
    wev = c->ଲିଖ;

    ngx_memzero(c, ଆକାର(ngx_connection_t));

    c->ପଠ = rev;
    c->ଲିଖ = wev;
    c->fd = s;
    c->log = log;

    instance = rev->instance;

    ngx_memzero(rev, ଆକାର(ngx_event_t));
    ngx_memzero(wev, ଆକାର(ngx_event_t));

    rev->instance = !instance;
    wev->instance = !instance;

    rev->index = NGX_INVALID_INDEX;
    wev->index = NGX_INVALID_INDEX;

    rev->data = c;
    wev->data = c;

    wev->ଲିଖ = 1;

    ଫେରନ୍ତୁ c;
}


ଶୂନ୍ୟ
ngx_free_connection(ngx_connection_t *c)
{
    c->data = ngx_cycle->free_connections;
    ngx_cycle->free_connections = c;
    ngx_cycle->free_connection_n++;

    ଯଦି (ngx_cycle->files && ngx_cycle->files[c->fd] == c) {
        ngx_cycle->files[c->fd] = ଖାଲି;
    }
}


ଶୂନ୍ୟ
ngx_close_connection(ngx_connection_t *c)
{
    ngx_err_t     err;
    ngx_uint_t    log_error, level;
    ngx_socket_t  fd;

    ଯଦି (c->fd == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0, "connection already closed");
        ଫେରନ୍ତୁ;
    }

    ଯଦି (c->ପଠ->timer_set) {
        ngx_del_timer(c->ପଠ);
    }

    ଯଦି (c->ଲିଖ->timer_set) {
        ngx_del_timer(c->ଲିଖ);
    }

    ଯଦି (!c->shared) {
        ଯଦି (ngx_del_conn) {
            ngx_del_conn(c, NGX_CLOSE_EVENT);

        } ଅନ୍ୟଥା {
            ଯଦି (c->ପଠ->active || c->ପଠ->disabled) {
                ngx_del_event(c->ପଠ, NGX_READ_EVENT, NGX_CLOSE_EVENT);
            }

            ଯଦି (c->ଲିଖ->active || c->ଲିଖ->disabled) {
                ngx_del_event(c->ଲିଖ, NGX_WRITE_EVENT, NGX_CLOSE_EVENT);
            }
        }
    }

    ଯଦି (c->ପଠ->posted) {
        ngx_delete_posted_event(c->ପଠ);
    }

    ଯଦି (c->ଲିଖ->posted) {
        ngx_delete_posted_event(c->ଲିଖ);
    }

    c->ପଠ->closed = 1;
    c->ଲିଖ->closed = 1;

    ngx_reusable_connection(c, 0);

    log_error = c->log_error;

    ngx_free_connection(c);

    fd = c->fd;
    c->fd = (ngx_socket_t) -1;

    ଯଦି (c->shared) {
        ଫେରନ୍ତୁ;
    }

    ଯଦି (ngx_close_socket(fd) == -1) {

        err = ngx_socket_errno;

        ଯଦି (err == NGX_ECONNRESET || err == NGX_ENOTCONN) {

            ବାଛ (log_error) {

            ସ୍ଥିତି NGX_ERROR_INFO:
                level = NGX_LOG_INFO;
                ଭାଙ୍ଗ;

            ସ୍ଥିତି NGX_ERROR_ERR:
                level = NGX_LOG_ERR;
                ଭାଙ୍ଗ;

            ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                level = NGX_LOG_CRIT;
            }

        } ଅନ୍ୟଥା {
            level = NGX_LOG_CRIT;
        }

        ngx_log_error(level, c->log, err, ngx_close_socket_n " %d failed", fd);
    }
}


ଶୂନ୍ୟ
ngx_reusable_connection(ngx_connection_t *c, ngx_uint_t reusable)
{
    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "reusable connection: %ui", reusable);

    ଯଦି (c->reusable) {
        ngx_queue_remove(&c->queue);
        ngx_cycle->reusable_connections_n--;

#ଯଦି (NGX_STAT_STUB)
        (ଶୂନ୍ୟ) ngx_atomic_fetch_add(ngx_stat_waiting, -1);
#endif
    }

    c->reusable = reusable;

    ଯଦି (reusable) {
        /* need cast as ngx_cycle is volatile */

        ngx_queue_ଭର୍ତ୍ତି_head(
            (ngx_queue_t *) &ngx_cycle->reusable_connections_queue, &c->queue);
        ngx_cycle->reusable_connections_n++;

#ଯଦି (NGX_STAT_STUB)
        (ଶୂନ୍ୟ) ngx_atomic_fetch_add(ngx_stat_waiting, 1);
#endif
    }
}


ସ୍ଥିର ଶୂନ୍ୟ
ngx_drain_connections(ngx_cycle_t *cycle)
{
    ngx_uint_t         i, n;
    ngx_queue_t       *q;
    ngx_connection_t  *c;

    ଯଦି (cycle->free_connection_n > cycle->connection_n / 16
        || cycle->reusable_connections_n == 0)
    {
        ଫେରନ୍ତୁ;
    }

    ଯଦି (cycle->connections_reuse_time != ngx_time()) {
        cycle->connections_reuse_time = ngx_time();

        ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                      "%ui worker_connections are not enough, "
                      "reusing connections",
                      cycle->connection_n);
    }

    c = ଖାଲି;
    n = ngx_max(ngx_min(32, cycle->reusable_connections_n / 8), 1);

    ପାଇଁ (i = 0; i < n; i++) {
        ଯଦି (ngx_queue_empty(&cycle->reusable_connections_queue)) {
            ଭାଙ୍ଗ;
        }

        q = ngx_queue_last(&cycle->reusable_connections_queue);
        c = ngx_queue_data(q, ngx_connection_t, queue);

        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "reusing connection");

        c->close = 1;
        c->ପଠ->handler(c->ପଠ);
    }

    ଯଦି (cycle->free_connection_n == 0 && c && c->reusable) {

        /*
         * if no connections were freed, try to reuse the last
         * connection again: this should free it as long as
         * previous reuse moved it to lingering close
         */

        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "reusing connection again");

        c->close = 1;
        c->ପଠ->handler(c->ପଠ);
    }
}


ଶୂନ୍ୟ
ngx_close_idle_connections(ngx_cycle_t *cycle)
{
    ngx_uint_t         i;
    ngx_connection_t  *c;

    c = cycle->connections;

    ପାଇଁ (i = 0; i < cycle->connection_n; i++) {

        /* THREAD: lock */

        ଯଦି (c[i].fd != (ngx_socket_t) -1 && c[i].idle) {
            c[i].close = 1;
            c[i].ପଠ->handler(c[i].ପଠ);
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
    ସଂରଚନା sockaddr_in   *sin;
#ଯଦି (NGX_HAVE_INET6)
    ngx_uint_t            i;
    ସଂରଚନା sockaddr_in6  *sin6;
#endif

    addr = 0;

    ଯଦି (c->local_socklen) {
        ବାଛ (c->local_sockaddr->sa_family) {

#ଯଦି (NGX_HAVE_INET6)
        ସ୍ଥିତି AF_INET6:
            sin6 = (ସଂରଚନା sockaddr_in6 *) c->local_sockaddr;

            ପାଇଁ (i = 0; addr == 0 && i < 16; i++) {
                addr |= sin6->sin6_addr.s6_addr[i];
            }

            ଭାଙ୍ଗ;
#endif

#ଯଦି (NGX_HAVE_UNIX_DOMAIN)
        ସ୍ଥିତି AF_UNIX:
            addr = 1;
            ଭାଙ୍ଗ;
#endif

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ: /* AF_INET */
            sin = (ସଂରଚନା sockaddr_in *) c->local_sockaddr;
            addr = sin->sin_addr.s_addr;
            ଭାଙ୍ଗ;
        }
    }

    ଯଦି (addr == 0) {

        len = ଆକାର(ngx_sockaddr_t);

        ଯଦି (getsockname(c->fd, &sa.sockaddr, &len) == -1) {
            ngx_connection_error(c, ngx_socket_errno, "getsockname() failed");
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        c->local_sockaddr = ngx_palloc(c->pool, len);
        ଯଦି (c->local_sockaddr == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ngx_memcpy(c->local_sockaddr, &sa, len);

        c->local_socklen = len;
    }

    ଯଦି (s == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_OK;
    }

    s->len = ngx_sock_ntop(c->local_sockaddr, c->local_socklen,
                           s->data, s->len, port);

    ଫେରନ୍ତୁ NGX_OK;
}


ngx_int_t
ngx_tcp_nodelay(ngx_connection_t *c)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା  tcp_nodelay;

    ଯଦି (c->tcp_nodelay != NGX_TCP_NODELAY_UNSET) {
        ଫେରନ୍ତୁ NGX_OK;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0, "tcp_nodelay");

    tcp_nodelay = 1;

    ଯଦି (setsockopt(c->fd, IPPROTO_TCP, TCP_NODELAY,
                   (ଅଚଳ ଶୂନ୍ୟ *) &tcp_nodelay, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା))
        == -1)
    {
#ଯଦି (NGX_SOLARIS)
        ଯଦି (c->log_error == NGX_ERROR_INFO) {

            /* Solaris returns EINVAL if a socket has been shut down */
            c->log_error = NGX_ERROR_IGNORE_EINVAL;

            ngx_connection_error(c, ngx_socket_errno,
                                 "setsockopt(TCP_NODELAY) failed");

            c->log_error = NGX_ERROR_INFO;

            ଫେରନ୍ତୁ NGX_ERROR;
        }
#endif

        ngx_connection_error(c, ngx_socket_errno,
                             "setsockopt(TCP_NODELAY) failed");
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    c->tcp_nodelay = NGX_TCP_NODELAY_SET;

    ଫେରନ୍ତୁ NGX_OK;
}


ngx_int_t
ngx_connection_error(ngx_connection_t *c, ngx_err_t err, ଅକ୍ଷର *text)
{
    ngx_uint_t  level;

    /* Winsock may return NGX_ECONNABORTED instead of NGX_ECONNRESET */

    ଯଦି ((err == NGX_ECONNRESET
#ଯଦି (NGX_WIN32)
         || err == NGX_ECONNABORTED
#endif
        ) && c->log_error == NGX_ERROR_IGNORE_ECONNRESET)
    {
        ଫେରନ୍ତୁ 0;
    }

#ଯଦି (NGX_SOLARIS)
    ଯଦି (err == NGX_EINVAL && c->log_error == NGX_ERROR_IGNORE_EINVAL) {
        ଫେରନ୍ତୁ 0;
    }
#endif

    ଯଦି (err == NGX_EMSGSIZE && c->log_error == NGX_ERROR_IGNORE_EMSGSIZE) {
        ଫେରନ୍ତୁ 0;
    }

#ଯଦି (NGX_LINUX)
    ଯଦି (err == NGX_EIO && c->ktls_recv) {
        ଫେରନ୍ତୁ 0;
    }
#endif

    ଯଦି (err == 0
        || err == NGX_ECONNRESET
#ଯଦି (NGX_WIN32)
        || err == NGX_ECONNABORTED
#ଅନ୍ୟଥା
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
        ବାଛ (c->log_error) {

        ସ୍ଥିତି NGX_ERROR_IGNORE_EMSGSIZE:
        ସ୍ଥିତି NGX_ERROR_IGNORE_EINVAL:
        ସ୍ଥିତି NGX_ERROR_IGNORE_ECONNRESET:
        ସ୍ଥିତି NGX_ERROR_INFO:
            level = NGX_LOG_INFO;
            ଭାଙ୍ଗ;

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            level = NGX_LOG_ERR;
        }

    } ଅନ୍ୟଥା {
        level = NGX_LOG_ALERT;
    }

    ngx_log_error(level, c->log, err, text);

    ଫେରନ୍ତୁ NGX_ERROR;
}
