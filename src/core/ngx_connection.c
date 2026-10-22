
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>
#ಅನ್ತರ್ಭಾವ <ngx_event.h>


ngx_os_io_t  ngx_io;


ಸ್ಥಿರ ಶೂನ್ಯ ngx_drain_connections(ngx_cycle_t *cycle);


ngx_listening_t *
ngx_create_listening(ngx_conf_t *cf, ಸಂರಚನಾ sockaddr *sockaddr,
    socklen_t socklen)
{
    size_t            len;
    ngx_listening_t  *ls;
    ಸಂರಚನಾ sockaddr  *sa;
    u_char            text[NGX_SOCKADDR_STRLEN];

    ls = ngx_array_ತಳ್ಳು(&cf->cycle->listening);
    ಯದಿ (ls == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ngx_memzero(ls, ಪರಿಮಾಣ(ngx_listening_t));

    sa = ngx_palloc(cf->pool, socklen);
    ಯದಿ (sa == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ngx_memcpy(sa, sockaddr, socklen);

    ls->sockaddr = sa;
    ls->socklen = socklen;

    len = ngx_sock_ntop(sa, socklen, text, NGX_SOCKADDR_STRLEN, 1);
    ls->addr_text.len = len;

    ವಿಕಲ್ಪ (ls->sockaddr->sa_family) {
#ಯದಿ (NGX_HAVE_INET6)
    ಪಕ್ಷ AF_INET6:
        ls->addr_text_max_len = NGX_INET6_ADDRSTRLEN;
        ಭಙ್ಗ;
#endif
#ಯದಿ (NGX_HAVE_UNIX_DOMAIN)
    ಪಕ್ಷ AF_UNIX:
        ls->addr_text_max_len = NGX_UNIX_ADDRSTRLEN;
        len++;
        ಭಙ್ಗ;
#endif
    ಪಕ್ಷ AF_INET:
        ls->addr_text_max_len = NGX_INET_ADDRSTRLEN;
        ಭಙ್ಗ;
    ಸ್ವಭಾವ:
        ls->addr_text_max_len = NGX_SOCKADDR_STRLEN;
        ಭಙ್ಗ;
    }

    ls->addr_text.data = ngx_pnalloc(cf->pool, len);
    ಯದಿ (ls->addr_text.data == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ngx_memcpy(ls->addr_text.data, text, len);

#ಯದಿ !(NGX_WIN32)
    ngx_rbtree_init(&ls->rbtree, &ls->sentinel, ngx_udp_rbtree_ಸೇರಿಸು_ಮೌಲ್ಯ);
#endif

    ls->fd = (ngx_socket_t) -1;
    ls->type = SOCK_STREAM;

    ls->backlog = NGX_LISTEN_BACKLOG;
    ls->rcvbuf = -1;
    ls->sndbuf = -1;

#ಯದಿ (NGX_HAVE_SETFIB)
    ls->setfib = -1;
#endif

#ಯದಿ (NGX_HAVE_TCP_FASTOPEN)
    ls->fastopen = -1;
#endif

    ಪ್ರತ್ಯಾಗಮ ls;
}


ngx_int_t
ngx_clone_listening(ngx_cycle_t *cycle, ngx_listening_t *ls)
{
#ಯದಿ (NGX_HAVE_REUSEPORT)

    ngx_int_t         n;
    ngx_core_conf_t  *ccf;
    ngx_listening_t   ols;

    ಯದಿ (!ls->reuseport || ls->worker != 0) {
        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    ols = *ls;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    ಪ್ರತಿ (n = 1; n < ccf->worker_processes; n++) {

        /* create a socket for each worker process */

        ls = ngx_array_ತಳ್ಳು(&cycle->listening);
        ಯದಿ (ls == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        *ls = ols;
        ls->worker = n;
    }

#endif

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ngx_int_t
ngx_set_inherited_sockets(ngx_cycle_t *cycle)
{
    size_t                     len;
    ngx_uint_t                 i;
    ngx_listening_t           *ls;
    socklen_t                  olen;
#ಯದಿ (NGX_HAVE_DEFERRED_ACCEPT || NGX_HAVE_TCP_FASTOPEN)
    ngx_err_t                  err;
#endif
#ಯದಿ (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    ಸಂರಚನಾ accept_filter_arg   af;
#endif
#ಯದಿ (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)
    ಪೂರ್ಣಾಙ್ಕ                        timeout;
#endif
#ಯದಿ (NGX_HAVE_REUSEPORT)
    ಪೂರ್ಣಾಙ್ಕ                        reuseport;
#endif

    ls = cycle->listening.elts;
    ಪ್ರತಿ (i = 0; i < cycle->listening.nelts; i++) {

        ls[i].sockaddr = ngx_palloc(cycle->pool, ಪರಿಮಾಣ(ngx_sockaddr_t));
        ಯದಿ (ls[i].sockaddr == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ls[i].socklen = ಪರಿಮಾಣ(ngx_sockaddr_t);
        ಯದಿ (getsockname(ls[i].fd, ls[i].sockaddr, &ls[i].socklen) == -1) {
            ngx_log_error(NGX_LOG_CRIT, cycle->log, ngx_socket_errno,
                          "getsockname() of the inherited "
                          "socket #%d failed", ls[i].fd);
            ls[i].ignore = 1;
            ಅನುವರ್ತ;
        }

        ಯದಿ (ls[i].socklen > (socklen_t) ಪರಿಮಾಣ(ngx_sockaddr_t)) {
            ls[i].socklen = ಪರಿಮಾಣ(ngx_sockaddr_t);
        }

        ವಿಕಲ್ಪ (ls[i].sockaddr->sa_family) {

#ಯದಿ (NGX_HAVE_INET6)
        ಪಕ್ಷ AF_INET6:
            ls[i].addr_text_max_len = NGX_INET6_ADDRSTRLEN;
            len = NGX_INET6_ADDRSTRLEN + ಪರಿಮಾಣ("[]:65535") - 1;
            ಭಙ್ಗ;
#endif

#ಯದಿ (NGX_HAVE_UNIX_DOMAIN)
        ಪಕ್ಷ AF_UNIX:
            ls[i].addr_text_max_len = NGX_UNIX_ADDRSTRLEN;
            len = NGX_UNIX_ADDRSTRLEN;
            ಭಙ್ಗ;
#endif

        ಪಕ್ಷ AF_INET:
            ls[i].addr_text_max_len = NGX_INET_ADDRSTRLEN;
            len = NGX_INET_ADDRSTRLEN + ಪರಿಮಾಣ(":65535") - 1;
            ಭಙ್ಗ;

        ಸ್ವಭಾವ:
            ngx_log_error(NGX_LOG_CRIT, cycle->log, ngx_socket_errno,
                          "the inherited socket #%d has "
                          "an unsupported protocol family", ls[i].fd);
            ls[i].ignore = 1;
            ಅನುವರ್ತ;
        }

        ls[i].addr_text.data = ngx_pnalloc(cycle->pool, len);
        ಯದಿ (ls[i].addr_text.data == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        len = ngx_sock_ntop(ls[i].sockaddr, ls[i].socklen,
                            ls[i].addr_text.data, len, 1);
        ಯದಿ (len == 0) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ls[i].addr_text.len = len;

        ls[i].backlog = NGX_LISTEN_BACKLOG;

        olen = ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ);

        ಯದಿ (getsockopt(ls[i].fd, SOL_SOCKET, SO_TYPE, (ಶೂನ್ಯ *) &ls[i].type,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_CRIT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_TYPE) %V failed", &ls[i].addr_text);
            ls[i].ignore = 1;
            ಅನುವರ್ತ;
        }

        olen = ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ);

        ಯದಿ (getsockopt(ls[i].fd, SOL_SOCKET, SO_RCVBUF, (ಶೂನ್ಯ *) &ls[i].rcvbuf,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_RCVBUF) %V failed, ignored",
                          &ls[i].addr_text);

            ls[i].rcvbuf = -1;
        }

        olen = ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ);

        ಯದಿ (getsockopt(ls[i].fd, SOL_SOCKET, SO_SNDBUF, (ಶೂನ್ಯ *) &ls[i].sndbuf,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_SNDBUF) %V failed, ignored",
                          &ls[i].addr_text);

            ls[i].sndbuf = -1;
        }

#ಯದಿ 0
        /* SO_SETFIB is currently a set only option */

#ಯದಿ (NGX_HAVE_SETFIB)

        olen = ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ);

        ಯದಿ (getsockopt(ls[i].fd, SOL_SOCKET, SO_SETFIB,
                       (ಶೂನ್ಯ *) &ls[i].setfib, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_SETFIB) %V failed, ignored",
                          &ls[i].addr_text);

            ls[i].setfib = -1;
        }

#endif
#endif

#ಯದಿ (NGX_HAVE_REUSEPORT)

        reuseport = 0;
        olen = ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ);

#ifdef SO_REUSEPORT_LB

        ಯದಿ (getsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT_LB,
                       (ಶೂನ್ಯ *) &reuseport, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_REUSEPORT_LB) %V failed, ignored",
                          &ls[i].addr_text);

        } ಅನ್ಯಥಾ {
            ls[i].reuseport = reuseport ? 1 : 0;
        }

#ಅನ್ಯಥಾ

        ಯದಿ (getsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT,
                       (ಶೂನ್ಯ *) &reuseport, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_REUSEPORT) %V failed, ignored",
                          &ls[i].addr_text);

        } ಅನ್ಯಥಾ {
            ls[i].reuseport = reuseport ? 1 : 0;
        }
#endif

#endif

        ಯದಿ (ls[i].type != SOCK_STREAM) {
            ಅನುವರ್ತ;
        }

#ifdef SO_PROTOCOL

        olen = ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ);

        ಯದಿ (getsockopt(ls[i].fd, SOL_SOCKET, SO_PROTOCOL,
                       (ಶೂನ್ಯ *) &ls[i].protocol, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_PROTOCOL) %V failed, ignored",
                          &ls[i].addr_text);
            ls[i].protocol = 0;

        } ಅನ್ಯಥಾ ಯದಿ (ls[i].protocol == IPPROTO_TCP) {
            ls[i].protocol = 0;
        }

#endif

#ಯದಿ (NGX_HAVE_TCP_FASTOPEN)

        olen = ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ);

        ಯದಿ (getsockopt(ls[i].fd, IPPROTO_TCP, TCP_FASTOPEN,
                       (ಶೂನ್ಯ *) &ls[i].fastopen, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            ಯದಿ (err != NGX_EOPNOTSUPP && err != NGX_ENOPROTOOPT
                && err != NGX_EINVAL)
            {
                ngx_log_error(NGX_LOG_NOTICE, cycle->log, err,
                              "getsockopt(TCP_FASTOPEN) %V failed, ignored",
                              &ls[i].addr_text);
            }

            ls[i].fastopen = -1;
        }

#endif

#ಯದಿ (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)

        ngx_memzero(&af, ಪರಿಮಾಣ(ಸಂರಚನಾ accept_filter_arg));
        olen = ಪರಿಮಾಣ(ಸಂರಚನಾ accept_filter_arg);

        ಯದಿ (getsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER, &af, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            ಯದಿ (err == NGX_EINVAL) {
                ಅನುವರ್ತ;
            }

            ngx_log_error(NGX_LOG_NOTICE, cycle->log, err,
                          "getsockopt(SO_ACCEPTFILTER) for %V failed, ignored",
                          &ls[i].addr_text);
            ಅನುವರ್ತ;
        }

        ಯದಿ (olen < ಪರಿಮಾಣ(ಸಂರಚನಾ accept_filter_arg) || af.af_name[0] == '\0') {
            ಅನುವರ್ತ;
        }

        ls[i].accept_filter = ngx_palloc(cycle->pool, 16);
        ಯದಿ (ls[i].accept_filter == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        (ಶೂನ್ಯ) ngx_cpystrn((u_char *) ls[i].accept_filter,
                           (u_char *) af.af_name, 16);
#endif

#ಯದಿ (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)

        timeout = 0;
        olen = ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ);

        ಯದಿ (getsockopt(ls[i].fd, IPPROTO_TCP, TCP_DEFER_ACCEPT, &timeout, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            ಯದಿ (err == NGX_EOPNOTSUPP) {
                ಅನುವರ್ತ;
            }

            ngx_log_error(NGX_LOG_NOTICE, cycle->log, err,
                          "getsockopt(TCP_DEFER_ACCEPT) for %V failed, ignored",
                          &ls[i].addr_text);
            ಅನುವರ್ತ;
        }

        ಯದಿ (olen < ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ) || timeout == 0) {
            ಅನುವರ್ತ;
        }

        ls[i].deferred_accept = 1;
#endif
    }

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ngx_int_t
ngx_open_listening_sockets(ngx_cycle_t *cycle)
{
    ಪೂರ್ಣಾಙ್ಕ               reuseaddr;
    ngx_uint_t        i, tries, failed;
    ngx_err_t         err;
    ngx_log_t        *log;
    ngx_socket_t      s;
    ngx_listening_t  *ls;

    reuseaddr = 1;
#ಯದಿ (NGX_SUPPRESS_WARN)
    failed = 0;
#endif

    log = cycle->log;

    /* TODO: configurable try number */

    ಪ್ರತಿ (tries = 5; tries; tries--) {
        failed = 0;

        /* for each listening socket */

        ls = cycle->listening.elts;
        ಪ್ರತಿ (i = 0; i < cycle->listening.nelts; i++) {

            ಯದಿ (ls[i].ignore) {
                ಅನುವರ್ತ;
            }

#ಯದಿ (NGX_HAVE_REUSEPORT)

            ಯದಿ (ls[i].add_reuseport || ls[i].change_protocol) {

                /*
                 * to allow transition from a socket without SO_REUSEPORT
                 * to multiple sockets with SO_REUSEPORT, we have to set
                 * SO_REUSEPORT on the old socket before opening new ones
                 *
                 * to allow transition between different socket protocols
                 * (e.g. IPPROTO_MPTCP), SO_REUSEPORT is set on both old
                 * and new sockets
                 */

                ಪೂರ್ಣಾಙ್ಕ  reuseport = 1;

#ifdef SO_REUSEPORT_LB

                ಯದಿ (setsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT_LB,
                               (ಧ್ರುವ ಶೂನ್ಯ *) &reuseport, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT_LB) %V failed, "
                                  "ignored",
                                  &ls[i].addr_text);
                }

#ಅನ್ಯಥಾ

                ಯದಿ (setsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT,
                               (ಧ್ರುವ ಶೂನ್ಯ *) &reuseport, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ))
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

            ಯದಿ (ls[i].fd != (ngx_socket_t) -1 && !ls[i].change_protocol) {
                ಅನುವರ್ತ;
            }

            ಯದಿ (ls[i].inherited) {

                /* TODO: close on exit */
                /* TODO: nonblocking */
                /* TODO: deferred accept */

                ಅನುವರ್ತ;
            }

            s = ngx_socket(ls[i].sockaddr->sa_family, ls[i].type,
                           ls[i].protocol);

            ಯದಿ (s == (ngx_socket_t) -1) {
                ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                              ngx_socket_n " %V failed", &ls[i].addr_text);
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            ಯದಿ (ls[i].type != SOCK_DGRAM || !ngx_test_config) {

                ಯದಿ (setsockopt(s, SOL_SOCKET, SO_REUSEADDR,
                               (ಧ್ರುವ ಶೂನ್ಯ *) &reuseaddr, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEADDR) %V failed",
                                  &ls[i].addr_text);

                    ಯದಿ (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_text);
                    }

                    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
                }
            }

#ಯದಿ (NGX_HAVE_REUSEPORT)

            ಯದಿ ((ls[i].reuseport || ls[i].change_protocol)
                && !ngx_test_config)
            {
                ಪೂರ್ಣಾಙ್ಕ  reuseport;

                reuseport = 1;

#ifdef SO_REUSEPORT_LB

                ಯದಿ (setsockopt(s, SOL_SOCKET, SO_REUSEPORT_LB,
                               (ಧ್ರುವ ಶೂನ್ಯ *) &reuseport, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT_LB) %V failed",
                                  &ls[i].addr_text);

                    ಯದಿ (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_text);
                    }

                    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
                }

#ಅನ್ಯಥಾ

                ಯದಿ (setsockopt(s, SOL_SOCKET, SO_REUSEPORT,
                               (ಧ್ರುವ ಶೂನ್ಯ *) &reuseport, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT) %V failed",
                                  &ls[i].addr_text);

                    ಯದಿ (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_text);
                    }

                    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
                }
#endif
            }
#endif

#ಯದಿ (NGX_HAVE_INET6 && defined IPV6_V6ONLY)

            ಯದಿ (ls[i].sockaddr->sa_family == AF_INET6) {
                ಪೂರ್ಣಾಙ್ಕ  ipv6only;

                ipv6only = ls[i].ipv6only;

                ಯದಿ (setsockopt(s, IPPROTO_IPV6, IPV6_V6ONLY,
                               (ಧ್ರುವ ಶೂನ್ಯ *) &ipv6only, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(IPV6_V6ONLY) %V failed, ignored",
                                  &ls[i].addr_text);
                }
            }
#endif
            /* TODO: close on exit */

            ಯದಿ (ngx_nonblocking(s) == -1) {
                ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                              ngx_nonblocking_n " %V failed",
                              &ls[i].addr_text);

                ಯದಿ (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_text);
                }

                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            ngx_log_debug2(NGX_LOG_DEBUG_CORE, log, 0,
                           "bind() %V #%d ", &ls[i].addr_text, s);

            ಯದಿ (bind(s, ls[i].sockaddr, ls[i].socklen) == -1) {
                err = ngx_socket_errno;

                ಯದಿ (err != NGX_EADDRINUSE || !ngx_test_config) {
                    ngx_log_error(NGX_LOG_EMERG, log, err,
                                  "bind() to %V failed", &ls[i].addr_text);
                }

                ಯದಿ (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_text);
                }

                ಯದಿ (err != NGX_EADDRINUSE) {
                    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
                }

                ಯದಿ (!ngx_test_config) {
                    failed = 1;
                }

                ಅನುವರ್ತ;
            }

#ಯದಿ (NGX_HAVE_UNIX_DOMAIN)

            ಯದಿ (ls[i].sockaddr->sa_family == AF_UNIX) {
                mode_t   mode;
                u_char  *name;

                name = ls[i].addr_text.data + ಪರಿಮಾಣ("unix:") - 1;
                mode = (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);

                ಯದಿ (chmod((ವರ್ಣ *) name, mode) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  "chmod() \"%s\" failed", name);
                }

                ಯದಿ (ngx_test_config) {
                    ಯದಿ (ngx_delete_file(name) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                      ngx_delete_file_n " %s failed", name);
                    }
                }
            }
#endif

            ಯದಿ (ls[i].type != SOCK_STREAM) {
                ls[i].fd = s;
                ls[i].open = 1;
                ಅನುವರ್ತ;
            }

            ಯದಿ (listen(s, ls[i].backlog) == -1) {
                err = ngx_socket_errno;

                /*
                 * on OpenVZ after suspend/resume EADDRINUSE
                 * may be returned by listen() instead of bind(), see
                 * https://bugs.openvz.org/browse/OVZ-5587
                 */

                ಯದಿ (err != NGX_EADDRINUSE || !ngx_test_config) {
                    ngx_log_error(NGX_LOG_EMERG, log, err,
                                  "listen() to %V, backlog %d failed",
                                  &ls[i].addr_text, ls[i].backlog);
                }

                ಯದಿ (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_text);
                }

                ಯದಿ (err != NGX_EADDRINUSE) {
                    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
                }

                ಯದಿ (!ngx_test_config) {
                    failed = 1;
                }

                ಅನುವರ್ತ;
            }

            ls[i].listen = 1;

            ls[i].fd = s;
            ls[i].open = 1;
        }

        ಯದಿ (!failed) {
            ಭಙ್ಗ;
        }

        /* TODO: delay configurable */

        ngx_log_error(NGX_LOG_NOTICE, log, 0,
                      "try again to bind() after 500ms");

        ngx_msleep(500);
    }

    ಯದಿ (failed) {
        ngx_log_error(NGX_LOG_EMERG, log, 0, "still could not bind()");
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ಶೂನ್ಯ
ngx_configure_listening_sockets(ngx_cycle_t *cycle)
{
    ಪೂರ್ಣಾಙ್ಕ                        ಮೌಲ್ಯ;
    ngx_uint_t                 i;
    ngx_listening_t           *ls;

#ಯದಿ (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    ಸಂರಚನಾ accept_filter_arg   af;
#endif

    ls = cycle->listening.elts;
    ಪ್ರತಿ (i = 0; i < cycle->listening.nelts; i++) {

        ls[i].log = *ls[i].logp;

        ಯದಿ (ls[i].rcvbuf != -1) {
            ಯದಿ (setsockopt(ls[i].fd, SOL_SOCKET, SO_RCVBUF,
                           (ಧ್ರುವ ಶೂನ್ಯ *) &ls[i].rcvbuf, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_RCVBUF, %d) %V failed, ignored",
                              ls[i].rcvbuf, &ls[i].addr_text);
            }
        }

        ಯದಿ (ls[i].sndbuf != -1) {
            ಯದಿ (setsockopt(ls[i].fd, SOL_SOCKET, SO_SNDBUF,
                           (ಧ್ರುವ ಶೂನ್ಯ *) &ls[i].sndbuf, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_SNDBUF, %d) %V failed, ignored",
                              ls[i].sndbuf, &ls[i].addr_text);
            }
        }

        ಯದಿ (ls[i].keepalive) {
            ಮೌಲ್ಯ = (ls[i].keepalive == 1) ? 1 : 0;

            ಯದಿ (setsockopt(ls[i].fd, SOL_SOCKET, SO_KEEPALIVE,
                           (ಧ್ರುವ ಶೂನ್ಯ *) &ಮೌಲ್ಯ, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_KEEPALIVE, %d) %V failed, ignored",
                              ಮೌಲ್ಯ, &ls[i].addr_text);
            }
        }

#ಯದಿ (NGX_HAVE_KEEPALIVE_TUNABLE)

#ಯದಿ !(NGX_DARWIN)

        ಯದಿ (ls[i].keepidle) {
            ಮೌಲ್ಯ = ls[i].keepidle;

#ಯದಿ (NGX_KEEPALIVE_FACTOR)
            ಮೌಲ್ಯ *= NGX_KEEPALIVE_FACTOR;
#endif

            ಯದಿ (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPIDLE,
                           (ಧ್ರುವ ಶೂನ್ಯ *) &ಮೌಲ್ಯ, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_KEEPIDLE, %d) %V failed, ignored",
                              ಮೌಲ್ಯ, &ls[i].addr_text);
            }
        }

#endif

        ಯದಿ (ls[i].keepintvl) {
            ಮೌಲ್ಯ = ls[i].keepintvl;

#ಯದಿ (NGX_KEEPALIVE_FACTOR)
            ಮೌಲ್ಯ *= NGX_KEEPALIVE_FACTOR;
#endif

            ಯದಿ (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPINTVL,
                           (ಧ್ರುವ ಶೂನ್ಯ *) &ಮೌಲ್ಯ, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                             "setsockopt(TCP_KEEPINTVL, %d) %V failed, ignored",
                             ಮೌಲ್ಯ, &ls[i].addr_text);
            }
        }

        ಯದಿ (ls[i].keepcnt) {
            ಯದಿ (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPCNT,
                           (ಧ್ರುವ ಶೂನ್ಯ *) &ls[i].keepcnt, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_KEEPCNT, %d) %V failed, ignored",
                              ls[i].keepcnt, &ls[i].addr_text);
            }
        }

#endif

#ಯದಿ (NGX_HAVE_SETFIB)
        ಯದಿ (ls[i].setfib != -1) {
            ಯದಿ (setsockopt(ls[i].fd, SOL_SOCKET, SO_SETFIB,
                           (ಧ್ರುವ ಶೂನ್ಯ *) &ls[i].setfib, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_SETFIB, %d) %V failed, ignored",
                              ls[i].setfib, &ls[i].addr_text);
            }
        }
#endif

#ಯದಿ (NGX_HAVE_TCP_FASTOPEN)
        ಯದಿ (ls[i].fastopen != -1) {
            ಯದಿ (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_FASTOPEN,
                           (ಧ್ರುವ ಶೂನ್ಯ *) &ls[i].fastopen, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_FASTOPEN, %d) %V failed, ignored",
                              ls[i].fastopen, &ls[i].addr_text);
            }
        }
#endif

#ಯದಿ 0
        ಯದಿ (1) {
            ಪೂರ್ಣಾಙ್ಕ tcp_nodelay = 1;

            ಯದಿ (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_NODELAY,
                       (ಧ್ರುವ ಶೂನ್ಯ *) &tcp_nodelay, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_NODELAY) %V failed, ignored",
                              &ls[i].addr_text);
            }
        }
#endif

        ಯದಿ (ls[i].listen) {

            /* change backlog via listen() */

            ಯದಿ (listen(ls[i].fd, ls[i].backlog) == -1) {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "listen() to %V, backlog %d failed, ignored",
                              &ls[i].addr_text, ls[i].backlog);
            }
        }

        /*
         * setting deferred mode should be last operation on socket,
         * because code may prematurely continue cycle on failure
         */

#ಯದಿ (NGX_HAVE_DEFERRED_ACCEPT)

#ifdef SO_ACCEPTFILTER

        ಯದಿ (ls[i].delete_deferred) {
            ಯದಿ (setsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER, ರಿಕ್ತ, 0)
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_ACCEPTFILTER, NULL) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);

                ಯದಿ (ls[i].accept_filter) {
                    ngx_log_error(NGX_LOG_ALERT, cycle->log, 0,
                                  "could not change the accept filter "
                                  "to \"%s\" for %V, ignored",
                                  ls[i].accept_filter, &ls[i].addr_text);
                }

                ಅನುವರ್ತ;
            }

            ls[i].deferred_accept = 0;
        }

        ಯದಿ (ls[i].add_deferred) {
            ngx_memzero(&af, ಪರಿಮಾಣ(ಸಂರಚನಾ accept_filter_arg));
            (ಶೂನ್ಯ) ngx_cpystrn((u_char *) af.af_name,
                               (u_char *) ls[i].accept_filter, 16);

            ಯದಿ (setsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER,
                           &af, ಪರಿಮಾಣ(ಸಂರಚನಾ accept_filter_arg))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_ACCEPTFILTER, \"%s\") "
                              "for %V failed, ignored",
                              ls[i].accept_filter, &ls[i].addr_text);
                ಅನುವರ್ತ;
            }

            ls[i].deferred_accept = 1;
        }

#endif

#ifdef TCP_DEFER_ACCEPT

        ಯದಿ (ls[i].add_deferred || ls[i].delete_deferred) {

            ಯದಿ (ls[i].add_deferred) {
                /*
                 * There is no way to find out how long a connection was
                 * in queue (and a connection may bypass deferred queue at all
                 * if syncookies were used), hence we use 1 second timeout
                 * here.
                 */
                ಮೌಲ್ಯ = 1;

            } ಅನ್ಯಥಾ {
                ಮೌಲ್ಯ = 0;
            }

            ಯದಿ (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_DEFER_ACCEPT,
                           &ಮೌಲ್ಯ, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_DEFER_ACCEPT, %d) for %V failed, "
                              "ignored",
                              ಮೌಲ್ಯ, &ls[i].addr_text);

                ಅನುವರ್ತ;
            }
        }

        ಯದಿ (ls[i].add_deferred) {
            ls[i].deferred_accept = 1;
        }

#endif

#endif /* NGX_HAVE_DEFERRED_ACCEPT */

#ಯದಿ (NGX_HAVE_IP_RECVDSTADDR)

        ಯದಿ (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET)
        {
            ಮೌಲ್ಯ = 1;

            ಯದಿ (setsockopt(ls[i].fd, IPPROTO_IP, IP_RECVDSTADDR,
                           (ಧ್ರುವ ಶೂನ್ಯ *) &ಮೌಲ್ಯ, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_RECVDSTADDR) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#ನೋಚೇದ್ಯದಿ (NGX_HAVE_IP_PKTINFO)

        ಯದಿ (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET)
        {
            ಮೌಲ್ಯ = 1;

            ಯದಿ (setsockopt(ls[i].fd, IPPROTO_IP, IP_PKTINFO,
                           (ಧ್ರುವ ಶೂನ್ಯ *) &ಮೌಲ್ಯ, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_PKTINFO) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#endif

#ಯದಿ (NGX_HAVE_INET6 && NGX_HAVE_IPV6_RECVPKTINFO)

        ಯದಿ (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET6)
        {
            ಮೌಲ್ಯ = 1;

            ಯದಿ (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_RECVPKTINFO,
                           (ಧ್ರುವ ಶೂನ್ಯ *) &ಮೌಲ್ಯ, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IPV6_RECVPKTINFO) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#endif

#ಯದಿ (NGX_HAVE_IP_MTU_DISCOVER)

        ಯದಿ (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET) {
            ಮೌಲ್ಯ = IP_PMTUDISC_DO;

            ಯದಿ (setsockopt(ls[i].fd, IPPROTO_IP, IP_MTU_DISCOVER,
                           (ಧ್ರುವ ಶೂನ್ಯ *) &ಮೌಲ್ಯ, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_MTU_DISCOVER) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#ನೋಚೇದ್ಯದಿ (NGX_HAVE_IP_DONTFRAG)

        ಯದಿ (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET) {
            ಮೌಲ್ಯ = 1;

            ಯದಿ (setsockopt(ls[i].fd, IPPROTO_IP, IP_DONTFRAG,
                           (ಧ್ರುವ ಶೂನ್ಯ *) &ಮೌಲ್ಯ, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_DONTFRAG) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#endif

#ಯದಿ (NGX_HAVE_INET6)

#ಯದಿ (NGX_HAVE_IPV6_MTU_DISCOVER)

        ಯದಿ (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET6) {
            ಮೌಲ್ಯ = IPV6_PMTUDISC_DO;

            ಯದಿ (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_MTU_DISCOVER,
                           (ಧ್ರುವ ಶೂನ್ಯ *) &ಮೌಲ್ಯ, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IPV6_MTU_DISCOVER) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#ನೋಚೇದ್ಯದಿ (NGX_HAVE_IPV6_DONTFRAG)

        ಯದಿ (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET6) {
            ಮೌಲ್ಯ = 1;

            ಯದಿ (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_DONTFRAG,
                           (ಧ್ರುವ ಶೂನ್ಯ *) &ಮೌಲ್ಯ, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ))
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

    ಪ್ರತ್ಯಾಗಮ;
}


ಶೂನ್ಯ
ngx_close_listening_sockets(ngx_cycle_t *cycle)
{
    ngx_uint_t         i;
    ngx_listening_t   *ls;
    ngx_connection_t  *c;

    ngx_accept_mutex_held = 0;
    ngx_use_accept_mutex = 0;

    ls = cycle->listening.elts;
    ಪ್ರತಿ (i = 0; i < cycle->listening.nelts; i++) {

#ಯದಿ (NGX_QUIC)
        ಯದಿ (ls[i].quic
            && (!ls[i].reuseport || !ngx_quic_bpf_enabled(cycle)))
        {
            ಅನುವರ್ತ;
        }
#endif

        c = ls[i].connection;

        ಯದಿ (c) {
            ಯದಿ (c->ಪಠ->active) {
                ಯದಿ (ngx_event_flags & NGX_USE_EPOLL_EVENT) {

                    /*
                     * it seems that Linux-2.6.x OpenVZ sends events
                     * for closed shared listening sockets unless
                     * the events was explicitly deleted
                     */

                    ngx_del_event(c->ಪಠ, NGX_READ_EVENT, 0);

                } ಅನ್ಯಥಾ {
                    ngx_del_event(c->ಪಠ, NGX_READ_EVENT, NGX_CLOSE_EVENT);
                }
            }

            ಯದಿ (c->ಪಠ->timer_set) {
                ngx_del_timer(c->ಪಠ);
            }

            ngx_free_connection(c);

            c->fd = (ngx_socket_t) -1;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, cycle->log, 0,
                       "close listening %V #%d ", &ls[i].addr_text, ls[i].fd);

        ಯದಿ (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_socket_errno,
                          ngx_close_socket_n " %V failed", &ls[i].addr_text);
        }

#ಯದಿ (NGX_HAVE_UNIX_DOMAIN)

        ಯದಿ (ls[i].sockaddr->sa_family == AF_UNIX
            && ngx_process <= NGX_PROCESS_MASTER
            && ngx_new_ದ್ವಿಮಾನ == 0
            && (!ls[i].inherited || ngx_getppid() != ngx_parent))
        {
            u_char *name = ls[i].addr_text.data + ಪರಿಮಾಣ("unix:") - 1;

            ಯದಿ (ngx_delete_file(name) == NGX_FILE_ERROR) {
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

    ಯದಿ (ngx_cycle->files && (ngx_uint_t) s >= ngx_cycle->files_n) {
        ngx_log_error(NGX_LOG_ALERT, log, 0,
                      "the new socket has number %d, "
                      "but only %ui files are available",
                      s, ngx_cycle->files_n);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ngx_drain_connections((ngx_cycle_t *) ngx_cycle);

    c = ngx_cycle->free_connections;

    ಯದಿ (c == ರಿಕ್ತ) {
        ngx_log_error(NGX_LOG_ALERT, log, 0,
                      "%ui worker_connections are not enough",
                      ngx_cycle->connection_n);

        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ngx_cycle->free_connections = c->data;
    ngx_cycle->free_connection_n--;

    ಯದಿ (ngx_cycle->files && ngx_cycle->files[s] == ರಿಕ್ತ) {
        ngx_cycle->files[s] = c;
    }

    rev = c->ಪಠ;
    wev = c->ಲಿಖ;

    ngx_memzero(c, ಪರಿಮಾಣ(ngx_connection_t));

    c->ಪಠ = rev;
    c->ಲಿಖ = wev;
    c->fd = s;
    c->log = log;

    instance = rev->instance;

    ngx_memzero(rev, ಪರಿಮಾಣ(ngx_event_t));
    ngx_memzero(wev, ಪರಿಮಾಣ(ngx_event_t));

    rev->instance = !instance;
    wev->instance = !instance;

    rev->index = NGX_INVALID_INDEX;
    wev->index = NGX_INVALID_INDEX;

    rev->data = c;
    wev->data = c;

    wev->ಲಿಖ = 1;

    ಪ್ರತ್ಯಾಗಮ c;
}


ಶೂನ್ಯ
ngx_free_connection(ngx_connection_t *c)
{
    c->data = ngx_cycle->free_connections;
    ngx_cycle->free_connections = c;
    ngx_cycle->free_connection_n++;

    ಯದಿ (ngx_cycle->files && ngx_cycle->files[c->fd] == c) {
        ngx_cycle->files[c->fd] = ರಿಕ್ತ;
    }
}


ಶೂನ್ಯ
ngx_close_connection(ngx_connection_t *c)
{
    ngx_err_t     err;
    ngx_uint_t    log_error, level;
    ngx_socket_t  fd;

    ಯದಿ (c->fd == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0, "connection already closed");
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ (c->ಪಠ->timer_set) {
        ngx_del_timer(c->ಪಠ);
    }

    ಯದಿ (c->ಲಿಖ->timer_set) {
        ngx_del_timer(c->ಲಿಖ);
    }

    ಯದಿ (!c->shared) {
        ಯದಿ (ngx_del_conn) {
            ngx_del_conn(c, NGX_CLOSE_EVENT);

        } ಅನ್ಯಥಾ {
            ಯದಿ (c->ಪಠ->active || c->ಪಠ->disabled) {
                ngx_del_event(c->ಪಠ, NGX_READ_EVENT, NGX_CLOSE_EVENT);
            }

            ಯದಿ (c->ಲಿಖ->active || c->ಲಿಖ->disabled) {
                ngx_del_event(c->ಲಿಖ, NGX_WRITE_EVENT, NGX_CLOSE_EVENT);
            }
        }
    }

    ಯದಿ (c->ಪಠ->posted) {
        ngx_delete_posted_event(c->ಪಠ);
    }

    ಯದಿ (c->ಲಿಖ->posted) {
        ngx_delete_posted_event(c->ಲಿಖ);
    }

    c->ಪಠ->closed = 1;
    c->ಲಿಖ->closed = 1;

    ngx_reusable_connection(c, 0);

    log_error = c->log_error;

    ngx_free_connection(c);

    fd = c->fd;
    c->fd = (ngx_socket_t) -1;

    ಯದಿ (c->shared) {
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ (ngx_close_socket(fd) == -1) {

        err = ngx_socket_errno;

        ಯದಿ (err == NGX_ECONNRESET || err == NGX_ENOTCONN) {

            ವಿಕಲ್ಪ (log_error) {

            ಪಕ್ಷ NGX_ERROR_INFO:
                level = NGX_LOG_INFO;
                ಭಙ್ಗ;

            ಪಕ್ಷ NGX_ERROR_ERR:
                level = NGX_LOG_ERR;
                ಭಙ್ಗ;

            ಸ್ವಭಾವ:
                level = NGX_LOG_CRIT;
            }

        } ಅನ್ಯಥಾ {
            level = NGX_LOG_CRIT;
        }

        ngx_log_error(level, c->log, err, ngx_close_socket_n " %d failed", fd);
    }
}


ಶೂನ್ಯ
ngx_reusable_connection(ngx_connection_t *c, ngx_uint_t reusable)
{
    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "reusable connection: %ui", reusable);

    ಯದಿ (c->reusable) {
        ngx_queue_remove(&c->queue);
        ngx_cycle->reusable_connections_n--;

#ಯದಿ (NGX_STAT_STUB)
        (ಶೂನ್ಯ) ngx_atomic_fetch_add(ngx_stat_waiting, -1);
#endif
    }

    c->reusable = reusable;

    ಯದಿ (reusable) {
        /* need cast as ngx_cycle is volatile */

        ngx_queue_ಸೇರಿಸು_head(
            (ngx_queue_t *) &ngx_cycle->reusable_connections_queue, &c->queue);
        ngx_cycle->reusable_connections_n++;

#ಯದಿ (NGX_STAT_STUB)
        (ಶೂನ್ಯ) ngx_atomic_fetch_add(ngx_stat_waiting, 1);
#endif
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_drain_connections(ngx_cycle_t *cycle)
{
    ngx_uint_t         i, n;
    ngx_queue_t       *q;
    ngx_connection_t  *c;

    ಯದಿ (cycle->free_connection_n > cycle->connection_n / 16
        || cycle->reusable_connections_n == 0)
    {
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ (cycle->connections_reuse_time != ngx_time()) {
        cycle->connections_reuse_time = ngx_time();

        ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                      "%ui worker_connections are not enough, "
                      "reusing connections",
                      cycle->connection_n);
    }

    c = ರಿಕ್ತ;
    n = ngx_max(ngx_min(32, cycle->reusable_connections_n / 8), 1);

    ಪ್ರತಿ (i = 0; i < n; i++) {
        ಯದಿ (ngx_queue_empty(&cycle->reusable_connections_queue)) {
            ಭಙ್ಗ;
        }

        q = ngx_queue_last(&cycle->reusable_connections_queue);
        c = ngx_queue_data(q, ngx_connection_t, queue);

        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "reusing connection");

        c->close = 1;
        c->ಪಠ->handler(c->ಪಠ);
    }

    ಯದಿ (cycle->free_connection_n == 0 && c && c->reusable) {

        /*
         * if no connections were freed, try to reuse the last
         * connection again: this should free it as long as
         * previous reuse moved it to lingering close
         */

        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "reusing connection again");

        c->close = 1;
        c->ಪಠ->handler(c->ಪಠ);
    }
}


ಶೂನ್ಯ
ngx_close_idle_connections(ngx_cycle_t *cycle)
{
    ngx_uint_t         i;
    ngx_connection_t  *c;

    c = cycle->connections;

    ಪ್ರತಿ (i = 0; i < cycle->connection_n; i++) {

        /* THREAD: lock */

        ಯದಿ (c[i].fd != (ngx_socket_t) -1 && c[i].idle) {
            c[i].close = 1;
            c[i].ಪಠ->handler(c[i].ಪಠ);
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
    ಸಂರಚನಾ sockaddr_in   *sin;
#ಯದಿ (NGX_HAVE_INET6)
    ngx_uint_t            i;
    ಸಂರಚನಾ sockaddr_in6  *sin6;
#endif

    addr = 0;

    ಯದಿ (c->local_socklen) {
        ವಿಕಲ್ಪ (c->local_sockaddr->sa_family) {

#ಯದಿ (NGX_HAVE_INET6)
        ಪಕ್ಷ AF_INET6:
            sin6 = (ಸಂರಚನಾ sockaddr_in6 *) c->local_sockaddr;

            ಪ್ರತಿ (i = 0; addr == 0 && i < 16; i++) {
                addr |= sin6->sin6_addr.s6_addr[i];
            }

            ಭಙ್ಗ;
#endif

#ಯದಿ (NGX_HAVE_UNIX_DOMAIN)
        ಪಕ್ಷ AF_UNIX:
            addr = 1;
            ಭಙ್ಗ;
#endif

        ಸ್ವಭಾವ: /* AF_INET */
            sin = (ಸಂರಚನಾ sockaddr_in *) c->local_sockaddr;
            addr = sin->sin_addr.s_addr;
            ಭಙ್ಗ;
        }
    }

    ಯದಿ (addr == 0) {

        len = ಪರಿಮಾಣ(ngx_sockaddr_t);

        ಯದಿ (getsockname(c->fd, &sa.sockaddr, &len) == -1) {
            ngx_connection_error(c, ngx_socket_errno, "getsockname() failed");
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        c->local_sockaddr = ngx_palloc(c->pool, len);
        ಯದಿ (c->local_sockaddr == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ngx_memcpy(c->local_sockaddr, &sa, len);

        c->local_socklen = len;
    }

    ಯದಿ (s == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    s->len = ngx_sock_ntop(c->local_sockaddr, c->local_socklen,
                           s->data, s->len, port);

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ngx_int_t
ngx_tcp_nodelay(ngx_connection_t *c)
{
    ಪೂರ್ಣಾಙ್ಕ  tcp_nodelay;

    ಯದಿ (c->tcp_nodelay != NGX_TCP_NODELAY_UNSET) {
        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0, "tcp_nodelay");

    tcp_nodelay = 1;

    ಯದಿ (setsockopt(c->fd, IPPROTO_TCP, TCP_NODELAY,
                   (ಧ್ರುವ ಶೂನ್ಯ *) &tcp_nodelay, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ))
        == -1)
    {
#ಯದಿ (NGX_SOLARIS)
        ಯದಿ (c->log_error == NGX_ERROR_INFO) {

            /* Solaris returns EINVAL if a socket has been shut down */
            c->log_error = NGX_ERROR_IGNORE_EINVAL;

            ngx_connection_error(c, ngx_socket_errno,
                                 "setsockopt(TCP_NODELAY) failed");

            c->log_error = NGX_ERROR_INFO;

            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }
#endif

        ngx_connection_error(c, ngx_socket_errno,
                             "setsockopt(TCP_NODELAY) failed");
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    c->tcp_nodelay = NGX_TCP_NODELAY_SET;

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ngx_int_t
ngx_connection_error(ngx_connection_t *c, ngx_err_t err, ವರ್ಣ *text)
{
    ngx_uint_t  level;

    /* Winsock may return NGX_ECONNABORTED instead of NGX_ECONNRESET */

    ಯದಿ ((err == NGX_ECONNRESET
#ಯದಿ (NGX_WIN32)
         || err == NGX_ECONNABORTED
#endif
        ) && c->log_error == NGX_ERROR_IGNORE_ECONNRESET)
    {
        ಪ್ರತ್ಯಾಗಮ 0;
    }

#ಯದಿ (NGX_SOLARIS)
    ಯದಿ (err == NGX_EINVAL && c->log_error == NGX_ERROR_IGNORE_EINVAL) {
        ಪ್ರತ್ಯಾಗಮ 0;
    }
#endif

    ಯದಿ (err == NGX_EMSGSIZE && c->log_error == NGX_ERROR_IGNORE_EMSGSIZE) {
        ಪ್ರತ್ಯಾಗಮ 0;
    }

#ಯದಿ (NGX_LINUX)
    ಯದಿ (err == NGX_EIO && c->ktls_recv) {
        ಪ್ರತ್ಯಾಗಮ 0;
    }
#endif

    ಯದಿ (err == 0
        || err == NGX_ECONNRESET
#ಯದಿ (NGX_WIN32)
        || err == NGX_ECONNABORTED
#ಅನ್ಯಥಾ
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
        ವಿಕಲ್ಪ (c->log_error) {

        ಪಕ್ಷ NGX_ERROR_IGNORE_EMSGSIZE:
        ಪಕ್ಷ NGX_ERROR_IGNORE_EINVAL:
        ಪಕ್ಷ NGX_ERROR_IGNORE_ECONNRESET:
        ಪಕ್ಷ NGX_ERROR_INFO:
            level = NGX_LOG_INFO;
            ಭಙ್ಗ;

        ಸ್ವಭಾವ:
            level = NGX_LOG_ERR;
        }

    } ಅನ್ಯಥಾ {
        level = NGX_LOG_ALERT;
    }

    ngx_log_error(level, c->log, err, text);

    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
}
