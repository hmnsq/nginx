
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>
#అన్తర్భావ <ngx_event.h>


ngx_os_io_t  ngx_io;


స్థిర శూన్య ngx_drain_connections(ngx_చక్రం_t *చక్రం);


ngx_listening_t *
ngx_create_listening(ngx_conf_t *cf, నిర్మాణం sockaddr *sockaddr,
    socklen_t socklen)
{
    కొలత_t            len;
    ngx_listening_t  *ls;
    నిర్మాణం sockaddr  *sa;
    u_char            పాఠ్యం[NGX_SOCKADDR_STRLEN];

    ls = ngx_array_నెట్టు(&cf->చక్రం->listening);
    ఒకవేళ (ls == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    ngx_memzero(ls, పరిమాణం(ngx_listening_t));

    sa = ngx_palloc(cf->pool, socklen);
    ఒకవేళ (sa == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    ngx_memcpy(sa, sockaddr, socklen);

    ls->sockaddr = sa;
    ls->socklen = socklen;

    len = ngx_sock_ntop(sa, socklen, పాఠ్యం, NGX_SOCKADDR_STRLEN, 1);
    ls->addr_పాఠ్యం.len = len;

    ఎంపిక (ls->sockaddr->sa_family) {
#ఒకవేళ (NGX_HAVE_INET6)
    స్థితి AF_INET6:
        ls->addr_పాఠ్యం_max_len = NGX_INET6_ADDRSTRLEN;
        ఆపు;
#అయితే_ముగింపు
#ఒకవేళ (NGX_HAVE_UNIX_DOMAIN)
    స్థితి AF_UNIX:
        ls->addr_పాఠ్యం_max_len = NGX_UNIX_ADDRSTRLEN;
        len++;
        ఆపు;
#అయితే_ముగింపు
    స్థితి AF_INET:
        ls->addr_పాఠ్యం_max_len = NGX_INET_ADDRSTRLEN;
        ఆపు;
    అప్రమేయ:
        ls->addr_పాఠ్యం_max_len = NGX_SOCKADDR_STRLEN;
        ఆపు;
    }

    ls->addr_పాఠ్యం.దత్తాంశం = ngx_pnalloc(cf->pool, len);
    ఒకవేళ (ls->addr_పాఠ్యం.దత్తాంశం == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    ngx_memcpy(ls->addr_పాఠ్యం.దత్తాంశం, పాఠ్యం, len);

#ఒకవేళ !(NGX_WIN32)
    ngx_rbtree_init(&ls->rbtree, &ls->sentinel, ngx_udp_rbtree_చొప్పించు_విలువ);
#అయితే_ముగింపు

    ls->fd = (ngx_socket_t) -1;
    ls->type = SOCK_ప్రవాహం;

    ls->backlog = NGX_LISTEN_BACKLOG;
    ls->rcvbuf = -1;
    ls->sndbuf = -1;

#ఒకవేళ (NGX_HAVE_SETFIB)
    ls->setfib = -1;
#అయితే_ముగింపు

#ఒకవేళ (NGX_HAVE_TCP_FASTOPEN)
    ls->fastopen = -1;
#అయితే_ముగింపు

    తిరిగి ls;
}


ngx_int_t
ngx_clone_listening(ngx_చక్రం_t *చక్రం, ngx_listening_t *ls)
{
#ఒకవేళ (NGX_HAVE_REUSEPORT)

    ngx_int_t         n;
    ngx_core_conf_t  *ccf;
    ngx_listening_t   ols;

    ఒకవేళ (!ls->reuseport || ls->కార్మికుడు != 0) {
        తిరిగి NGX_OK;
    }

    ols = *ls;

    ccf = (ngx_core_conf_t *) ngx_get_conf(చక్రం->conf_ctx, ngx_core_module);

    కొరకు (n = 1; n < ccf->కార్మికుడు_processes; n++) {

        /* create a socket for each worker process */

        ls = ngx_array_నెట్టు(&చక్రం->listening);
        ఒకవేళ (ls == ఖాళీ) {
            తిరిగి NGX_ERROR;
        }

        *ls = ols;
        ls->కార్మికుడు = n;
    }

#అయితే_ముగింపు

    తిరిగి NGX_OK;
}


ngx_int_t
ngx_సమితి_inherited_sockets(ngx_చక్రం_t *చక్రం)
{
    కొలత_t                     len;
    ngx_uint_t                 i;
    ngx_listening_t           *ls;
    socklen_t                  olen;
#ఒకవేళ (NGX_HAVE_DEFERRED_ACCEPT || NGX_HAVE_TCP_FASTOPEN)
    ngx_err_t                  err;
#అయితే_ముగింపు
#ఒకవేళ (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    నిర్మాణం accept_filter_arg   af;
#అయితే_ముగింపు
#ఒకవేళ (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)
    పూర్ణాంకం                        timeout;
#అయితే_ముగింపు
#ఒకవేళ (NGX_HAVE_REUSEPORT)
    పూర్ణాంకం                        reuseport;
#అయితే_ముగింపు

    ls = చక్రం->listening.elts;
    కొరకు (i = 0; i < చక్రం->listening.nelts; i++) {

        ls[i].sockaddr = ngx_palloc(చక్రం->pool, పరిమాణం(ngx_sockaddr_t));
        ఒకవేళ (ls[i].sockaddr == ఖాళీ) {
            తిరిగి NGX_ERROR;
        }

        ls[i].socklen = పరిమాణం(ngx_sockaddr_t);
        ఒకవేళ (getsockname(ls[i].fd, ls[i].sockaddr, &ls[i].socklen) == -1) {
            ngx_log_error(NGX_LOG_CRIT, చక్రం->సంవర్గమానం, ngx_socket_errno,
                          "getsockname() of the inherited "
                          "socket #%d failed", ls[i].fd);
            ls[i].ignore = 1;
            కొనసాగు;
        }

        ఒకవేళ (ls[i].socklen > (socklen_t) పరిమాణం(ngx_sockaddr_t)) {
            ls[i].socklen = పరిమాణం(ngx_sockaddr_t);
        }

        ఎంపిక (ls[i].sockaddr->sa_family) {

#ఒకవేళ (NGX_HAVE_INET6)
        స్థితి AF_INET6:
            ls[i].addr_పాఠ్యం_max_len = NGX_INET6_ADDRSTRLEN;
            len = NGX_INET6_ADDRSTRLEN + పరిమాణం("[]:65535") - 1;
            ఆపు;
#అయితే_ముగింపు

#ఒకవేళ (NGX_HAVE_UNIX_DOMAIN)
        స్థితి AF_UNIX:
            ls[i].addr_పాఠ్యం_max_len = NGX_UNIX_ADDRSTRLEN;
            len = NGX_UNIX_ADDRSTRLEN;
            ఆపు;
#అయితే_ముగింపు

        స్థితి AF_INET:
            ls[i].addr_పాఠ్యం_max_len = NGX_INET_ADDRSTRLEN;
            len = NGX_INET_ADDRSTRLEN + పరిమాణం(":65535") - 1;
            ఆపు;

        అప్రమేయ:
            ngx_log_error(NGX_LOG_CRIT, చక్రం->సంవర్గమానం, ngx_socket_errno,
                          "the inherited socket #%d has "
                          "an unsupported protocol family", ls[i].fd);
            ls[i].ignore = 1;
            కొనసాగు;
        }

        ls[i].addr_పాఠ్యం.దత్తాంశం = ngx_pnalloc(చక్రం->pool, len);
        ఒకవేళ (ls[i].addr_పాఠ్యం.దత్తాంశం == ఖాళీ) {
            తిరిగి NGX_ERROR;
        }

        len = ngx_sock_ntop(ls[i].sockaddr, ls[i].socklen,
                            ls[i].addr_పాఠ్యం.దత్తాంశం, len, 1);
        ఒకవేళ (len == 0) {
            తిరిగి NGX_ERROR;
        }

        ls[i].addr_పాఠ్యం.len = len;

        ls[i].backlog = NGX_LISTEN_BACKLOG;

        olen = పరిమాణం(పూర్ణాంకం);

        ఒకవేళ (getsockopt(ls[i].fd, SOL_SOCKET, SO_TYPE, (శూన్య *) &ls[i].type,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_CRIT, చక్రం->సంవర్గమానం, ngx_socket_errno,
                          "getsockopt(SO_TYPE) %V failed", &ls[i].addr_పాఠ్యం);
            ls[i].ignore = 1;
            కొనసాగు;
        }

        olen = పరిమాణం(పూర్ణాంకం);

        ఒకవేళ (getsockopt(ls[i].fd, SOL_SOCKET, SO_RCVBUF, (శూన్య *) &ls[i].rcvbuf,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, చక్రం->సంవర్గమానం, ngx_socket_errno,
                          "getsockopt(SO_RCVBUF) %V failed, ignored",
                          &ls[i].addr_పాఠ్యం);

            ls[i].rcvbuf = -1;
        }

        olen = పరిమాణం(పూర్ణాంకం);

        ఒకవేళ (getsockopt(ls[i].fd, SOL_SOCKET, SO_SNDBUF, (శూన్య *) &ls[i].sndbuf,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, చక్రం->సంవర్గమానం, ngx_socket_errno,
                          "getsockopt(SO_SNDBUF) %V failed, ignored",
                          &ls[i].addr_పాఠ్యం);

            ls[i].sndbuf = -1;
        }

#ఒకవేళ 0
        /* SO_SETFIB is currently a set only option */

#ఒకవేళ (NGX_HAVE_SETFIB)

        olen = పరిమాణం(పూర్ణాంకం);

        ఒకవేళ (getsockopt(ls[i].fd, SOL_SOCKET, SO_SETFIB,
                       (శూన్య *) &ls[i].setfib, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, చక్రం->సంవర్గమానం, ngx_socket_errno,
                          "getsockopt(SO_SETFIB) %V failed, ignored",
                          &ls[i].addr_పాఠ్యం);

            ls[i].setfib = -1;
        }

#అయితే_ముగింపు
#అయితే_ముగింపు

#ఒకవేళ (NGX_HAVE_REUSEPORT)

        reuseport = 0;
        olen = పరిమాణం(పూర్ణాంకం);

#నిర్వచితమైతే SO_REUSEPORT_LB

        ఒకవేళ (getsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT_LB,
                       (శూన్య *) &reuseport, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, చక్రం->సంవర్గమానం, ngx_socket_errno,
                          "getsockopt(SO_REUSEPORT_LB) %V failed, ignored",
                          &ls[i].addr_పాఠ్యం);

        } లేకపోతే {
            ls[i].reuseport = reuseport ? 1 : 0;
        }

#లేకపోతే

        ఒకవేళ (getsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT,
                       (శూన్య *) &reuseport, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, చక్రం->సంవర్గమానం, ngx_socket_errno,
                          "getsockopt(SO_REUSEPORT) %V failed, ignored",
                          &ls[i].addr_పాఠ్యం);

        } లేకపోతే {
            ls[i].reuseport = reuseport ? 1 : 0;
        }
#అయితే_ముగింపు

#అయితే_ముగింపు

        ఒకవేళ (ls[i].type != SOCK_ప్రవాహం) {
            కొనసాగు;
        }

#నిర్వచితమైతే SO_PROTOCOL

        olen = పరిమాణం(పూర్ణాంకం);

        ఒకవేళ (getsockopt(ls[i].fd, SOL_SOCKET, SO_PROTOCOL,
                       (శూన్య *) &ls[i].protocol, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, చక్రం->సంవర్గమానం, ngx_socket_errno,
                          "getsockopt(SO_PROTOCOL) %V failed, ignored",
                          &ls[i].addr_పాఠ్యం);
            ls[i].protocol = 0;

        } లేకపోతే ఒకవేళ (ls[i].protocol == IPPROTO_TCP) {
            ls[i].protocol = 0;
        }

#అయితే_ముగింపు

#ఒకవేళ (NGX_HAVE_TCP_FASTOPEN)

        olen = పరిమాణం(పూర్ణాంకం);

        ఒకవేళ (getsockopt(ls[i].fd, IPPROTO_TCP, TCP_FASTOPEN,
                       (శూన్య *) &ls[i].fastopen, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            ఒకవేళ (err != NGX_EOPNOTSUPP && err != NGX_ENOPROTOOPT
                && err != NGX_EINVAL)
            {
                ngx_log_error(NGX_LOG_NOTICE, చక్రం->సంవర్గమానం, err,
                              "getsockopt(TCP_FASTOPEN) %V failed, ignored",
                              &ls[i].addr_పాఠ్యం);
            }

            ls[i].fastopen = -1;
        }

#అయితే_ముగింపు

#ఒకవేళ (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)

        ngx_memzero(&af, పరిమాణం(నిర్మాణం accept_filter_arg));
        olen = పరిమాణం(నిర్మాణం accept_filter_arg);

        ఒకవేళ (getsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER, &af, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            ఒకవేళ (err == NGX_EINVAL) {
                కొనసాగు;
            }

            ngx_log_error(NGX_LOG_NOTICE, చక్రం->సంవర్గమానం, err,
                          "getsockopt(SO_ACCEPTFILTER) for %V failed, ignored",
                          &ls[i].addr_పాఠ్యం);
            కొనసాగు;
        }

        ఒకవేళ (olen < పరిమాణం(నిర్మాణం accept_filter_arg) || af.af_name[0] == '\0') {
            కొనసాగు;
        }

        ls[i].accept_filter = ngx_palloc(చక్రం->pool, 16);
        ఒకవేళ (ls[i].accept_filter == ఖాళీ) {
            తిరిగి NGX_ERROR;
        }

        (శూన్య) ngx_cpystrn((u_char *) ls[i].accept_filter,
                           (u_char *) af.af_name, 16);
#అయితే_ముగింపు

#ఒకవేళ (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)

        timeout = 0;
        olen = పరిమాణం(పూర్ణాంకం);

        ఒకవేళ (getsockopt(ls[i].fd, IPPROTO_TCP, TCP_DEFER_ACCEPT, &timeout, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            ఒకవేళ (err == NGX_EOPNOTSUPP) {
                కొనసాగు;
            }

            ngx_log_error(NGX_LOG_NOTICE, చక్రం->సంవర్గమానం, err,
                          "getsockopt(TCP_DEFER_ACCEPT) for %V failed, ignored",
                          &ls[i].addr_పాఠ్యం);
            కొనసాగు;
        }

        ఒకవేళ (olen < పరిమాణం(పూర్ణాంకం) || timeout == 0) {
            కొనసాగు;
        }

        ls[i].deferred_accept = 1;
#అయితే_ముగింపు
    }

    తిరిగి NGX_OK;
}


ngx_int_t
ngx_open_listening_sockets(ngx_చక్రం_t *చక్రం)
{
    పూర్ణాంకం               reuseaddr;
    ngx_uint_t        i, tries, failed;
    ngx_err_t         err;
    ngx_log_t        *సంవర్గమానం;
    ngx_socket_t      s;
    ngx_listening_t  *ls;

    reuseaddr = 1;
#ఒకవేళ (NGX_SUPPRESS_WARN)
    failed = 0;
#అయితే_ముగింపు

    సంవర్గమానం = చక్రం->సంవర్గమానం;

    /* TODO: configurable try number */

    కొరకు (tries = 5; tries; tries--) {
        failed = 0;

        /* for each listening socket */

        ls = చక్రం->listening.elts;
        కొరకు (i = 0; i < చక్రం->listening.nelts; i++) {

            ఒకవేళ (ls[i].ignore) {
                కొనసాగు;
            }

#ఒకవేళ (NGX_HAVE_REUSEPORT)

            ఒకవేళ (ls[i].కూడు_reuseport || ls[i].change_protocol) {

                /*
                 * to allow transition from a socket without SO_REUSEPORT
                 * to multiple sockets with SO_REUSEPORT, we have to set
                 * SO_REUSEPORT on the old socket before opening new ones
                 *
                 * to allow transition between different socket protocols
                 * (e.g. IPPROTO_MPTCP), SO_REUSEPORT is set on both old
                 * and new sockets
                 */

                పూర్ణాంకం  reuseport = 1;

#నిర్వచితమైతే SO_REUSEPORT_LB

                ఒకవేళ (setsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT_LB,
                               (స్థిరరాశి శూన్య *) &reuseport, పరిమాణం(పూర్ణాంకం))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_ALERT, చక్రం->సంవర్గమానం, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT_LB) %V failed, "
                                  "ignored",
                                  &ls[i].addr_పాఠ్యం);
                }

#లేకపోతే

                ఒకవేళ (setsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT,
                               (స్థిరరాశి శూన్య *) &reuseport, పరిమాణం(పూర్ణాంకం))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_ALERT, చక్రం->సంవర్గమానం, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT) %V failed, ignored",
                                  &ls[i].addr_పాఠ్యం);
                }
#అయితే_ముగింపు

                ls[i].కూడు_reuseport = 0;
            }
#అయితే_ముగింపు

            ఒకవేళ (ls[i].fd != (ngx_socket_t) -1 && !ls[i].change_protocol) {
                కొనసాగు;
            }

            ఒకవేళ (ls[i].inherited) {

                /* TODO: close on exit */
                /* TODO: nonblocking */
                /* TODO: deferred accept */

                కొనసాగు;
            }

            s = ngx_socket(ls[i].sockaddr->sa_family, ls[i].type,
                           ls[i].protocol);

            ఒకవేళ (s == (ngx_socket_t) -1) {
                ngx_log_error(NGX_LOG_EMERG, సంవర్గమానం, ngx_socket_errno,
                              ngx_socket_n " %V failed", &ls[i].addr_పాఠ్యం);
                తిరిగి NGX_ERROR;
            }

            ఒకవేళ (ls[i].type != SOCK_DGRAM || !ngx_test_config) {

                ఒకవేళ (setsockopt(s, SOL_SOCKET, SO_REUSEADDR,
                               (స్థిరరాశి శూన్య *) &reuseaddr, పరిమాణం(పూర్ణాంకం))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, సంవర్గమానం, ngx_socket_errno,
                                  "setsockopt(SO_REUSEADDR) %V failed",
                                  &ls[i].addr_పాఠ్యం);

                    ఒకవేళ (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, సంవర్గమానం, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_పాఠ్యం);
                    }

                    తిరిగి NGX_ERROR;
                }
            }

#ఒకవేళ (NGX_HAVE_REUSEPORT)

            ఒకవేళ ((ls[i].reuseport || ls[i].change_protocol)
                && !ngx_test_config)
            {
                పూర్ణాంకం  reuseport;

                reuseport = 1;

#నిర్వచితమైతే SO_REUSEPORT_LB

                ఒకవేళ (setsockopt(s, SOL_SOCKET, SO_REUSEPORT_LB,
                               (స్థిరరాశి శూన్య *) &reuseport, పరిమాణం(పూర్ణాంకం))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, సంవర్గమానం, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT_LB) %V failed",
                                  &ls[i].addr_పాఠ్యం);

                    ఒకవేళ (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, సంవర్గమానం, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_పాఠ్యం);
                    }

                    తిరిగి NGX_ERROR;
                }

#లేకపోతే

                ఒకవేళ (setsockopt(s, SOL_SOCKET, SO_REUSEPORT,
                               (స్థిరరాశి శూన్య *) &reuseport, పరిమాణం(పూర్ణాంకం))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, సంవర్గమానం, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT) %V failed",
                                  &ls[i].addr_పాఠ్యం);

                    ఒకవేళ (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, సంవర్గమానం, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_పాఠ్యం);
                    }

                    తిరిగి NGX_ERROR;
                }
#అయితే_ముగింపు
            }
#అయితే_ముగింపు

#ఒకవేళ (NGX_HAVE_INET6 && defined IPV6_V6ONLY)

            ఒకవేళ (ls[i].sockaddr->sa_family == AF_INET6) {
                పూర్ణాంకం  ipv6only;

                ipv6only = ls[i].ipv6only;

                ఒకవేళ (setsockopt(s, IPPROTO_IPV6, IPV6_V6ONLY,
                               (స్థిరరాశి శూన్య *) &ipv6only, పరిమాణం(పూర్ణాంకం))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, సంవర్గమానం, ngx_socket_errno,
                                  "setsockopt(IPV6_V6ONLY) %V failed, ignored",
                                  &ls[i].addr_పాఠ్యం);
                }
            }
#అయితే_ముగింపు
            /* TODO: close on exit */

            ఒకవేళ (ngx_nonblocking(s) == -1) {
                ngx_log_error(NGX_LOG_EMERG, సంవర్గమానం, ngx_socket_errno,
                              ngx_nonblocking_n " %V failed",
                              &ls[i].addr_పాఠ్యం);

                ఒకవేళ (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, సంవర్గమానం, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_పాఠ్యం);
                }

                తిరిగి NGX_ERROR;
            }

            ngx_log_debug2(NGX_LOG_DEBUG_CORE, సంవర్గమానం, 0,
                           "bind() %V #%d ", &ls[i].addr_పాఠ్యం, s);

            ఒకవేళ (bind(s, ls[i].sockaddr, ls[i].socklen) == -1) {
                err = ngx_socket_errno;

                ఒకవేళ (err != NGX_EADDRINUSE || !ngx_test_config) {
                    ngx_log_error(NGX_LOG_EMERG, సంవర్గమానం, err,
                                  "bind() to %V failed", &ls[i].addr_పాఠ్యం);
                }

                ఒకవేళ (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, సంవర్గమానం, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_పాఠ్యం);
                }

                ఒకవేళ (err != NGX_EADDRINUSE) {
                    తిరిగి NGX_ERROR;
                }

                ఒకవేళ (!ngx_test_config) {
                    failed = 1;
                }

                కొనసాగు;
            }

#ఒకవేళ (NGX_HAVE_UNIX_DOMAIN)

            ఒకవేళ (ls[i].sockaddr->sa_family == AF_UNIX) {
                బహుళకం_t   బహుళకం;
                u_char  *name;

                name = ls[i].addr_పాఠ్యం.దత్తాంశం + పరిమాణం("unix:") - 1;
                బహుళకం = (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);

                ఒకవేళ (chmod((అక్షరం *) name, బహుళకం) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, చక్రం->సంవర్గమానం, ngx_errno,
                                  "chmod() \"%s\" failed", name);
                }

                ఒకవేళ (ngx_test_config) {
                    ఒకవేళ (ngx_delete_file(name) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, చక్రం->సంవర్గమానం, ngx_errno,
                                      ngx_delete_file_n " %s failed", name);
                    }
                }
            }
#అయితే_ముగింపు

            ఒకవేళ (ls[i].type != SOCK_ప్రవాహం) {
                ls[i].fd = s;
                ls[i].open = 1;
                కొనసాగు;
            }

            ఒకవేళ (listen(s, ls[i].backlog) == -1) {
                err = ngx_socket_errno;

                /*
                 * on OpenVZ after suspend/resume EADDRINUSE
                 * may be returned by listen() instead of bind(), see
                 * https://bugs.openvz.org/browse/OVZ-5587
                 */

                ఒకవేళ (err != NGX_EADDRINUSE || !ngx_test_config) {
                    ngx_log_error(NGX_LOG_EMERG, సంవర్గమానం, err,
                                  "listen() to %V, backlog %d failed",
                                  &ls[i].addr_పాఠ్యం, ls[i].backlog);
                }

                ఒకవేళ (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, సంవర్గమానం, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_పాఠ్యం);
                }

                ఒకవేళ (err != NGX_EADDRINUSE) {
                    తిరిగి NGX_ERROR;
                }

                ఒకవేళ (!ngx_test_config) {
                    failed = 1;
                }

                కొనసాగు;
            }

            ls[i].listen = 1;

            ls[i].fd = s;
            ls[i].open = 1;
        }

        ఒకవేళ (!failed) {
            ఆపు;
        }

        /* TODO: delay configurable */

        ngx_log_error(NGX_LOG_NOTICE, సంవర్గమానం, 0,
                      "try again to bind() after 500ms");

        ngx_msleep(500);
    }

    ఒకవేళ (failed) {
        ngx_log_error(NGX_LOG_EMERG, సంవర్గమానం, 0, "still could not bind()");
        తిరిగి NGX_ERROR;
    }

    తిరిగి NGX_OK;
}


శూన్య
ngx_configure_listening_sockets(ngx_చక్రం_t *చక్రం)
{
    పూర్ణాంకం                        విలువ;
    ngx_uint_t                 i;
    ngx_listening_t           *ls;

#ఒకవేళ (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    నిర్మాణం accept_filter_arg   af;
#అయితే_ముగింపు

    ls = చక్రం->listening.elts;
    కొరకు (i = 0; i < చక్రం->listening.nelts; i++) {

        ls[i].సంవర్గమానం = *ls[i].logp;

        ఒకవేళ (ls[i].rcvbuf != -1) {
            ఒకవేళ (setsockopt(ls[i].fd, SOL_SOCKET, SO_RCVBUF,
                           (స్థిరరాశి శూన్య *) &ls[i].rcvbuf, పరిమాణం(పూర్ణాంకం))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, చక్రం->సంవర్గమానం, ngx_socket_errno,
                              "setsockopt(SO_RCVBUF, %d) %V failed, ignored",
                              ls[i].rcvbuf, &ls[i].addr_పాఠ్యం);
            }
        }

        ఒకవేళ (ls[i].sndbuf != -1) {
            ఒకవేళ (setsockopt(ls[i].fd, SOL_SOCKET, SO_SNDBUF,
                           (స్థిరరాశి శూన్య *) &ls[i].sndbuf, పరిమాణం(పూర్ణాంకం))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, చక్రం->సంవర్గమానం, ngx_socket_errno,
                              "setsockopt(SO_SNDBUF, %d) %V failed, ignored",
                              ls[i].sndbuf, &ls[i].addr_పాఠ్యం);
            }
        }

        ఒకవేళ (ls[i].keepalive) {
            విలువ = (ls[i].keepalive == 1) ? 1 : 0;

            ఒకవేళ (setsockopt(ls[i].fd, SOL_SOCKET, SO_KEEPALIVE,
                           (స్థిరరాశి శూన్య *) &విలువ, పరిమాణం(పూర్ణాంకం))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, చక్రం->సంవర్గమానం, ngx_socket_errno,
                              "setsockopt(SO_KEEPALIVE, %d) %V failed, ignored",
                              విలువ, &ls[i].addr_పాఠ్యం);
            }
        }

#ఒకవేళ (NGX_HAVE_KEEPALIVE_TUNABLE)

#ఒకవేళ !(NGX_DARWIN)

        ఒకవేళ (ls[i].keepidle) {
            విలువ = ls[i].keepidle;

#ఒకవేళ (NGX_KEEPALIVE_గుణకం)
            విలువ *= NGX_KEEPALIVE_గుణకం;
#అయితే_ముగింపు

            ఒకవేళ (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPIDLE,
                           (స్థిరరాశి శూన్య *) &విలువ, పరిమాణం(పూర్ణాంకం))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, చక్రం->సంవర్గమానం, ngx_socket_errno,
                              "setsockopt(TCP_KEEPIDLE, %d) %V failed, ignored",
                              విలువ, &ls[i].addr_పాఠ్యం);
            }
        }

#అయితే_ముగింపు

        ఒకవేళ (ls[i].keepintvl) {
            విలువ = ls[i].keepintvl;

#ఒకవేళ (NGX_KEEPALIVE_గుణకం)
            విలువ *= NGX_KEEPALIVE_గుణకం;
#అయితే_ముగింపు

            ఒకవేళ (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPINTVL,
                           (స్థిరరాశి శూన్య *) &విలువ, పరిమాణం(పూర్ణాంకం))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, చక్రం->సంవర్గమానం, ngx_socket_errno,
                             "setsockopt(TCP_KEEPINTVL, %d) %V failed, ignored",
                             విలువ, &ls[i].addr_పాఠ్యం);
            }
        }

        ఒకవేళ (ls[i].keepcnt) {
            ఒకవేళ (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPCNT,
                           (స్థిరరాశి శూన్య *) &ls[i].keepcnt, పరిమాణం(పూర్ణాంకం))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, చక్రం->సంవర్గమానం, ngx_socket_errno,
                              "setsockopt(TCP_KEEPCNT, %d) %V failed, ignored",
                              ls[i].keepcnt, &ls[i].addr_పాఠ్యం);
            }
        }

#అయితే_ముగింపు

#ఒకవేళ (NGX_HAVE_SETFIB)
        ఒకవేళ (ls[i].setfib != -1) {
            ఒకవేళ (setsockopt(ls[i].fd, SOL_SOCKET, SO_SETFIB,
                           (స్థిరరాశి శూన్య *) &ls[i].setfib, పరిమాణం(పూర్ణాంకం))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, చక్రం->సంవర్గమానం, ngx_socket_errno,
                              "setsockopt(SO_SETFIB, %d) %V failed, ignored",
                              ls[i].setfib, &ls[i].addr_పాఠ్యం);
            }
        }
#అయితే_ముగింపు

#ఒకవేళ (NGX_HAVE_TCP_FASTOPEN)
        ఒకవేళ (ls[i].fastopen != -1) {
            ఒకవేళ (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_FASTOPEN,
                           (స్థిరరాశి శూన్య *) &ls[i].fastopen, పరిమాణం(పూర్ణాంకం))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, చక్రం->సంవర్గమానం, ngx_socket_errno,
                              "setsockopt(TCP_FASTOPEN, %d) %V failed, ignored",
                              ls[i].fastopen, &ls[i].addr_పాఠ్యం);
            }
        }
#అయితే_ముగింపు

#ఒకవేళ 0
        ఒకవేళ (1) {
            పూర్ణాంకం tcp_nodelay = 1;

            ఒకవేళ (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_NODELAY,
                       (స్థిరరాశి శూన్య *) &tcp_nodelay, పరిమాణం(పూర్ణాంకం))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, చక్రం->సంవర్గమానం, ngx_socket_errno,
                              "setsockopt(TCP_NODELAY) %V failed, ignored",
                              &ls[i].addr_పాఠ్యం);
            }
        }
#అయితే_ముగింపు

        ఒకవేళ (ls[i].listen) {

            /* change backlog via listen() */

            ఒకవేళ (listen(ls[i].fd, ls[i].backlog) == -1) {
                ngx_log_error(NGX_LOG_ALERT, చక్రం->సంవర్గమానం, ngx_socket_errno,
                              "listen() to %V, backlog %d failed, ignored",
                              &ls[i].addr_పాఠ్యం, ls[i].backlog);
            }
        }

        /*
         * setting deferred mode should be last operation on socket,
         * because code may prematurely continue cycle on failure
         */

#ఒకవేళ (NGX_HAVE_DEFERRED_ACCEPT)

#నిర్వచితమైతే SO_ACCEPTFILTER

        ఒకవేళ (ls[i].delete_deferred) {
            ఒకవేళ (setsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER, ఖాళీ, 0)
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, చక్రం->సంవర్గమానం, ngx_socket_errno,
                              "setsockopt(SO_ACCEPTFILTER, NULL) "
                              "for %V failed, ignored",
                              &ls[i].addr_పాఠ్యం);

                ఒకవేళ (ls[i].accept_filter) {
                    ngx_log_error(NGX_LOG_ALERT, చక్రం->సంవర్గమానం, 0,
                                  "could not change the accept filter "
                                  "to \"%s\" for %V, ignored",
                                  ls[i].accept_filter, &ls[i].addr_పాఠ్యం);
                }

                కొనసాగు;
            }

            ls[i].deferred_accept = 0;
        }

        ఒకవేళ (ls[i].కూడు_deferred) {
            ngx_memzero(&af, పరిమాణం(నిర్మాణం accept_filter_arg));
            (శూన్య) ngx_cpystrn((u_char *) af.af_name,
                               (u_char *) ls[i].accept_filter, 16);

            ఒకవేళ (setsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER,
                           &af, పరిమాణం(నిర్మాణం accept_filter_arg))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, చక్రం->సంవర్గమానం, ngx_socket_errno,
                              "setsockopt(SO_ACCEPTFILTER, \"%s\") "
                              "for %V failed, ignored",
                              ls[i].accept_filter, &ls[i].addr_పాఠ్యం);
                కొనసాగు;
            }

            ls[i].deferred_accept = 1;
        }

#అయితే_ముగింపు

#నిర్వచితమైతే TCP_DEFER_ACCEPT

        ఒకవేళ (ls[i].కూడు_deferred || ls[i].delete_deferred) {

            ఒకవేళ (ls[i].కూడు_deferred) {
                /*
                 * There is no way to find out how long a connection was
                 * in queue (and a connection may bypass deferred queue at all
                 * if syncookies were used), hence we use 1 second timeout
                 * here.
                 */
                విలువ = 1;

            } లేకపోతే {
                విలువ = 0;
            }

            ఒకవేళ (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_DEFER_ACCEPT,
                           &విలువ, పరిమాణం(పూర్ణాంకం))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, చక్రం->సంవర్గమానం, ngx_socket_errno,
                              "setsockopt(TCP_DEFER_ACCEPT, %d) for %V failed, "
                              "ignored",
                              విలువ, &ls[i].addr_పాఠ్యం);

                కొనసాగు;
            }
        }

        ఒకవేళ (ls[i].కూడు_deferred) {
            ls[i].deferred_accept = 1;
        }

#అయితే_ముగింపు

#అయితే_ముగింపు /* NGX_HAVE_DEFERRED_ACCEPT */

#ఒకవేళ (NGX_HAVE_IP_RECVDSTADDR)

        ఒకవేళ (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET)
        {
            విలువ = 1;

            ఒకవేళ (setsockopt(ls[i].fd, IPPROTO_IP, IP_RECVDSTADDR,
                           (స్థిరరాశి శూన్య *) &విలువ, పరిమాణం(పూర్ణాంకం))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, చక్రం->సంవర్గమానం, ngx_socket_errno,
                              "setsockopt(IP_RECVDSTADDR) "
                              "for %V failed, ignored",
                              &ls[i].addr_పాఠ్యం);
            }
        }

#లేదావేళ (NGX_HAVE_IP_PKTINFO)

        ఒకవేళ (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET)
        {
            విలువ = 1;

            ఒకవేళ (setsockopt(ls[i].fd, IPPROTO_IP, IP_PKTINFO,
                           (స్థిరరాశి శూన్య *) &విలువ, పరిమాణం(పూర్ణాంకం))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, చక్రం->సంవర్గమానం, ngx_socket_errno,
                              "setsockopt(IP_PKTINFO) "
                              "for %V failed, ignored",
                              &ls[i].addr_పాఠ్యం);
            }
        }

#అయితే_ముగింపు

#ఒకవేళ (NGX_HAVE_INET6 && NGX_HAVE_IPV6_RECVPKTINFO)

        ఒకవేళ (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET6)
        {
            విలువ = 1;

            ఒకవేళ (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_RECVPKTINFO,
                           (స్థిరరాశి శూన్య *) &విలువ, పరిమాణం(పూర్ణాంకం))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, చక్రం->సంవర్గమానం, ngx_socket_errno,
                              "setsockopt(IPV6_RECVPKTINFO) "
                              "for %V failed, ignored",
                              &ls[i].addr_పాఠ్యం);
            }
        }

#అయితే_ముగింపు

#ఒకవేళ (NGX_HAVE_IP_MTU_DISCOVER)

        ఒకవేళ (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET) {
            విలువ = IP_PMTUDISC_DO;

            ఒకవేళ (setsockopt(ls[i].fd, IPPROTO_IP, IP_MTU_DISCOVER,
                           (స్థిరరాశి శూన్య *) &విలువ, పరిమాణం(పూర్ణాంకం))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, చక్రం->సంవర్గమానం, ngx_socket_errno,
                              "setsockopt(IP_MTU_DISCOVER) "
                              "for %V failed, ignored",
                              &ls[i].addr_పాఠ్యం);
            }
        }

#లేదావేళ (NGX_HAVE_IP_DONTFRAG)

        ఒకవేళ (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET) {
            విలువ = 1;

            ఒకవేళ (setsockopt(ls[i].fd, IPPROTO_IP, IP_DONTFRAG,
                           (స్థిరరాశి శూన్య *) &విలువ, పరిమాణం(పూర్ణాంకం))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, చక్రం->సంవర్గమానం, ngx_socket_errno,
                              "setsockopt(IP_DONTFRAG) "
                              "for %V failed, ignored",
                              &ls[i].addr_పాఠ్యం);
            }
        }

#అయితే_ముగింపు

#ఒకవేళ (NGX_HAVE_INET6)

#ఒకవేళ (NGX_HAVE_IPV6_MTU_DISCOVER)

        ఒకవేళ (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET6) {
            విలువ = IPV6_PMTUDISC_DO;

            ఒకవేళ (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_MTU_DISCOVER,
                           (స్థిరరాశి శూన్య *) &విలువ, పరిమాణం(పూర్ణాంకం))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, చక్రం->సంవర్గమానం, ngx_socket_errno,
                              "setsockopt(IPV6_MTU_DISCOVER) "
                              "for %V failed, ignored",
                              &ls[i].addr_పాఠ్యం);
            }
        }

#లేదావేళ (NGX_HAVE_IPV6_DONTFRAG)

        ఒకవేళ (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET6) {
            విలువ = 1;

            ఒకవేళ (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_DONTFRAG,
                           (స్థిరరాశి శూన్య *) &విలువ, పరిమాణం(పూర్ణాంకం))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, చక్రం->సంవర్గమానం, ngx_socket_errno,
                              "setsockopt(IPV6_DONTFRAG) "
                              "for %V failed, ignored",
                              &ls[i].addr_పాఠ్యం);
            }
        }

#అయితే_ముగింపు

#అయితే_ముగింపు
    }

    తిరిగి;
}


శూన్య
ngx_close_listening_sockets(ngx_చక్రం_t *చక్రం)
{
    ngx_uint_t         i;
    ngx_listening_t   *ls;
    ngx_connection_t  *c;

    ngx_accept_mutex_held = 0;
    ngx_use_accept_mutex = 0;

    ls = చక్రం->listening.elts;
    కొరకు (i = 0; i < చక్రం->listening.nelts; i++) {

#ఒకవేళ (NGX_QUIC)
        ఒకవేళ (ls[i].quic
            && (!ls[i].reuseport || !ngx_quic_bpf_enabled(చక్రం)))
        {
            కొనసాగు;
        }
#అయితే_ముగింపు

        c = ls[i].connection;

        ఒకవేళ (c) {
            ఒకవేళ (c->పఠ->చురుకైన) {
                ఒకవేళ (ngx_event_flags & NGX_USE_EPOLL_EVENT) {

                    /*
                     * it seems that Linux-2.6.x OpenVZ sends events
                     * for closed shared listening sockets unless
                     * the events was explicitly deleted
                     */

                    ngx_del_event(c->పఠ, NGX_READ_EVENT, 0);

                } లేకపోతే {
                    ngx_del_event(c->పఠ, NGX_READ_EVENT, NGX_CLOSE_EVENT);
                }
            }

            ఒకవేళ (c->పఠ->timer_సమితి) {
                ngx_del_timer(c->పఠ);
            }

            ngx_free_connection(c);

            c->fd = (ngx_socket_t) -1;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, చక్రం->సంవర్గమానం, 0,
                       "close listening %V #%d ", &ls[i].addr_పాఠ్యం, ls[i].fd);

        ఒకవేళ (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, చక్రం->సంవర్గమానం, ngx_socket_errno,
                          ngx_close_socket_n " %V failed", &ls[i].addr_పాఠ్యం);
        }

#ఒకవేళ (NGX_HAVE_UNIX_DOMAIN)

        ఒకవేళ (ls[i].sockaddr->sa_family == AF_UNIX
            && ngx_process <= NGX_PROCESS_MASTER
            && ngx_new_ద్విమాన == 0
            && (!ls[i].inherited || ngx_getppid() != ngx_మాతృ))
        {
            u_char *name = ls[i].addr_పాఠ్యం.దత్తాంశం + పరిమాణం("unix:") - 1;

            ఒకవేళ (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, చక్రం->సంవర్గమానం, ngx_socket_errno,
                              ngx_delete_file_n " %s failed", name);
            }
        }

#అయితే_ముగింపు

        ls[i].fd = (ngx_socket_t) -1;
    }

    చక్రం->listening.nelts = 0;
}


ngx_connection_t *
ngx_get_connection(ngx_socket_t s, ngx_log_t *సంవర్గమానం)
{
    ngx_uint_t         instance;
    ngx_event_t       *rev, *wev;
    ngx_connection_t  *c;

    /* disable warning: Win32 SOCKET is u_int while UNIX socket is int */

    ఒకవేళ (ngx_చక్రం->files && (ngx_uint_t) s >= ngx_చక్రం->files_n) {
        ngx_log_error(NGX_LOG_ALERT, సంవర్గమానం, 0,
                      "the new socket has number %d, "
                      "but only %ui files are available",
                      s, ngx_చక్రం->files_n);
        తిరిగి ఖాళీ;
    }

    ngx_drain_connections((ngx_చక్రం_t *) ngx_చక్రం);

    c = ngx_చక్రం->free_connections;

    ఒకవేళ (c == ఖాళీ) {
        ngx_log_error(NGX_LOG_ALERT, సంవర్గమానం, 0,
                      "%ui worker_connections are not enough",
                      ngx_చక్రం->connection_n);

        తిరిగి ఖాళీ;
    }

    ngx_చక్రం->free_connections = c->దత్తాంశం;
    ngx_చక్రం->free_connection_n--;

    ఒకవేళ (ngx_చక్రం->files && ngx_చక్రం->files[s] == ఖాళీ) {
        ngx_చక్రం->files[s] = c;
    }

    rev = c->పఠ;
    wev = c->లిఖ;

    ngx_memzero(c, పరిమాణం(ngx_connection_t));

    c->పఠ = rev;
    c->లిఖ = wev;
    c->fd = s;
    c->సంవర్గమానం = సంవర్గమానం;

    instance = rev->instance;

    ngx_memzero(rev, పరిమాణం(ngx_event_t));
    ngx_memzero(wev, పరిమాణం(ngx_event_t));

    rev->instance = !instance;
    wev->instance = !instance;

    rev->సూచీ = NGX_INVALID_సూచీ;
    wev->సూచీ = NGX_INVALID_సూచీ;

    rev->దత్తాంశం = c;
    wev->దత్తాంశం = c;

    wev->లిఖ = 1;

    తిరిగి c;
}


శూన్య
ngx_free_connection(ngx_connection_t *c)
{
    c->దత్తాంశం = ngx_చక్రం->free_connections;
    ngx_చక్రం->free_connections = c;
    ngx_చక్రం->free_connection_n++;

    ఒకవేళ (ngx_చక్రం->files && ngx_చక్రం->files[c->fd] == c) {
        ngx_చక్రం->files[c->fd] = ఖాళీ;
    }
}


శూన్య
ngx_close_connection(ngx_connection_t *c)
{
    ngx_err_t     err;
    ngx_uint_t    log_error, level;
    ngx_socket_t  fd;

    ఒకవేళ (c->fd == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, c->సంవర్గమానం, 0, "connection already closed");
        తిరిగి;
    }

    ఒకవేళ (c->పఠ->timer_సమితి) {
        ngx_del_timer(c->పఠ);
    }

    ఒకవేళ (c->లిఖ->timer_సమితి) {
        ngx_del_timer(c->లిఖ);
    }

    ఒకవేళ (!c->shared) {
        ఒకవేళ (ngx_del_conn) {
            ngx_del_conn(c, NGX_CLOSE_EVENT);

        } లేకపోతే {
            ఒకవేళ (c->పఠ->చురుకైన || c->పఠ->disabled) {
                ngx_del_event(c->పఠ, NGX_READ_EVENT, NGX_CLOSE_EVENT);
            }

            ఒకవేళ (c->లిఖ->చురుకైన || c->లిఖ->disabled) {
                ngx_del_event(c->లిఖ, NGX_WRITE_EVENT, NGX_CLOSE_EVENT);
            }
        }
    }

    ఒకవేళ (c->పఠ->posted) {
        ngx_delete_posted_event(c->పఠ);
    }

    ఒకవేళ (c->లిఖ->posted) {
        ngx_delete_posted_event(c->లిఖ);
    }

    c->పఠ->closed = 1;
    c->లిఖ->closed = 1;

    ngx_reusable_connection(c, 0);

    log_error = c->log_error;

    ngx_free_connection(c);

    fd = c->fd;
    c->fd = (ngx_socket_t) -1;

    ఒకవేళ (c->shared) {
        తిరిగి;
    }

    ఒకవేళ (ngx_close_socket(fd) == -1) {

        err = ngx_socket_errno;

        ఒకవేళ (err == NGX_ECONNRESET || err == NGX_ENOTCONN) {

            ఎంపిక (log_error) {

            స్థితి NGX_ERROR_INFO:
                level = NGX_LOG_INFO;
                ఆపు;

            స్థితి NGX_ERROR_ERR:
                level = NGX_LOG_ERR;
                ఆపు;

            అప్రమేయ:
                level = NGX_LOG_CRIT;
            }

        } లేకపోతే {
            level = NGX_LOG_CRIT;
        }

        ngx_log_error(level, c->సంవర్గమానం, err, ngx_close_socket_n " %d failed", fd);
    }
}


శూన్య
ngx_reusable_connection(ngx_connection_t *c, ngx_uint_t reusable)
{
    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->సంవర్గమానం, 0,
                   "reusable connection: %ui", reusable);

    ఒకవేళ (c->reusable) {
        ngx_వరుస_remove(&c->వరుస);
        ngx_చక్రం->reusable_connections_n--;

#ఒకవేళ (NGX_STAT_STUB)
        (శూన్య) ngx_atomic_fetch_కూడు(ngx_stat_నిరీక్షణ, -1);
#అయితే_ముగింపు
    }

    c->reusable = reusable;

    ఒకవేళ (reusable) {
        /* need cast as ngx_cycle is volatile */

        ngx_వరుస_చొప్పించు_తల(
            (ngx_వరుస_t *) &ngx_చక్రం->reusable_connections_వరుస, &c->వరుస);
        ngx_చక్రం->reusable_connections_n++;

#ఒకవేళ (NGX_STAT_STUB)
        (శూన్య) ngx_atomic_fetch_కూడు(ngx_stat_నిరీక్షణ, 1);
#అయితే_ముగింపు
    }
}


స్థిర శూన్య
ngx_drain_connections(ngx_చక్రం_t *చక్రం)
{
    ngx_uint_t         i, n;
    ngx_వరుస_t       *q;
    ngx_connection_t  *c;

    ఒకవేళ (చక్రం->free_connection_n > చక్రం->connection_n / 16
        || చక్రం->reusable_connections_n == 0)
    {
        తిరిగి;
    }

    ఒకవేళ (చక్రం->connections_reuse_సమయం != ngx_సమయం()) {
        చక్రం->connections_reuse_సమయం = ngx_సమయం();

        ngx_log_error(NGX_LOG_WARN, చక్రం->సంవర్గమానం, 0,
                      "%ui worker_connections are not enough, "
                      "reusing connections",
                      చక్రం->connection_n);
    }

    c = ఖాళీ;
    n = ngx_max(ngx_min(32, చక్రం->reusable_connections_n / 8), 1);

    కొరకు (i = 0; i < n; i++) {
        ఒకవేళ (ngx_వరుస_ఖాళీలు(&చక్రం->reusable_connections_వరుస)) {
            ఆపు;
        }

        q = ngx_వరుస_చివరి(&చక్రం->reusable_connections_వరుస);
        c = ngx_వరుస_దత్తాంశం(q, ngx_connection_t, వరుస);

        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->సంవర్గమానం, 0,
                       "reusing connection");

        c->close = 1;
        c->పఠ->handler(c->పఠ);
    }

    ఒకవేళ (చక్రం->free_connection_n == 0 && c && c->reusable) {

        /*
         * if no connections were freed, try to reuse the last
         * connection again: this should free it as long as
         * previous reuse moved it to lingering close
         */

        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->సంవర్గమానం, 0,
                       "reusing connection again");

        c->close = 1;
        c->పఠ->handler(c->పఠ);
    }
}


శూన్య
ngx_close_idle_connections(ngx_చక్రం_t *చక్రం)
{
    ngx_uint_t         i;
    ngx_connection_t  *c;

    c = చక్రం->connections;

    కొరకు (i = 0; i < చక్రం->connection_n; i++) {

        /* THREAD: lock */

        ఒకవేళ (c[i].fd != (ngx_socket_t) -1 && c[i].idle) {
            c[i].close = 1;
            c[i].పఠ->handler(c[i].పఠ);
        }
    }
}


ngx_int_t
ngx_connection_local_sockaddr(ngx_connection_t *c, ngx_str_t *s,
    ngx_uint_t పోర్టు)
{
    socklen_t             len;
    ngx_uint_t            addr;
    ngx_sockaddr_t        sa;
    నిర్మాణం sockaddr_లోపల   *జ్యా;
#ఒకవేళ (NGX_HAVE_INET6)
    ngx_uint_t            i;
    నిర్మాణం sockaddr_in6  *sin6;
#అయితే_ముగింపు

    addr = 0;

    ఒకవేళ (c->local_socklen) {
        ఎంపిక (c->local_sockaddr->sa_family) {

#ఒకవేళ (NGX_HAVE_INET6)
        స్థితి AF_INET6:
            sin6 = (నిర్మాణం sockaddr_in6 *) c->local_sockaddr;

            కొరకు (i = 0; addr == 0 && i < 16; i++) {
                addr |= sin6->sin6_addr.s6_addr[i];
            }

            ఆపు;
#అయితే_ముగింపు

#ఒకవేళ (NGX_HAVE_UNIX_DOMAIN)
        స్థితి AF_UNIX:
            addr = 1;
            ఆపు;
#అయితే_ముగింపు

        అప్రమేయ: /* AF_INET */
            జ్యా = (నిర్మాణం sockaddr_లోపల *) c->local_sockaddr;
            addr = జ్యా->sin_addr.s_addr;
            ఆపు;
        }
    }

    ఒకవేళ (addr == 0) {

        len = పరిమాణం(ngx_sockaddr_t);

        ఒకవేళ (getsockname(c->fd, &sa.sockaddr, &len) == -1) {
            ngx_connection_error(c, ngx_socket_errno, "getsockname() failed");
            తిరిగి NGX_ERROR;
        }

        c->local_sockaddr = ngx_palloc(c->pool, len);
        ఒకవేళ (c->local_sockaddr == ఖాళీ) {
            తిరిగి NGX_ERROR;
        }

        ngx_memcpy(c->local_sockaddr, &sa, len);

        c->local_socklen = len;
    }

    ఒకవేళ (s == ఖాళీ) {
        తిరిగి NGX_OK;
    }

    s->len = ngx_sock_ntop(c->local_sockaddr, c->local_socklen,
                           s->దత్తాంశం, s->len, పోర్టు);

    తిరిగి NGX_OK;
}


ngx_int_t
ngx_tcp_nodelay(ngx_connection_t *c)
{
    పూర్ణాంకం  tcp_nodelay;

    ఒకవేళ (c->tcp_nodelay != NGX_TCP_NODELAY_UNSET) {
        తిరిగి NGX_OK;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->సంవర్గమానం, 0, "tcp_nodelay");

    tcp_nodelay = 1;

    ఒకవేళ (setsockopt(c->fd, IPPROTO_TCP, TCP_NODELAY,
                   (స్థిరరాశి శూన్య *) &tcp_nodelay, పరిమాణం(పూర్ణాంకం))
        == -1)
    {
#ఒకవేళ (NGX_SOLARIS)
        ఒకవేళ (c->log_error == NGX_ERROR_INFO) {

            /* Solaris returns EINVAL if a socket has been shut down */
            c->log_error = NGX_ERROR_IGNORE_EINVAL;

            ngx_connection_error(c, ngx_socket_errno,
                                 "setsockopt(TCP_NODELAY) failed");

            c->log_error = NGX_ERROR_INFO;

            తిరిగి NGX_ERROR;
        }
#అయితే_ముగింపు

        ngx_connection_error(c, ngx_socket_errno,
                             "setsockopt(TCP_NODELAY) failed");
        తిరిగి NGX_ERROR;
    }

    c->tcp_nodelay = NGX_TCP_NODELAY_సమితి;

    తిరిగి NGX_OK;
}


ngx_int_t
ngx_connection_error(ngx_connection_t *c, ngx_err_t err, అక్షరం *పాఠ్యం)
{
    ngx_uint_t  level;

    /* Winsock may return NGX_ECONNABORTED instead of NGX_ECONNRESET */

    ఒకవేళ ((err == NGX_ECONNRESET
#ఒకవేళ (NGX_WIN32)
         || err == NGX_ECONNABORTED
#అయితే_ముగింపు
        ) && c->log_error == NGX_ERROR_IGNORE_ECONNRESET)
    {
        తిరిగి 0;
    }

#ఒకవేళ (NGX_SOLARIS)
    ఒకవేళ (err == NGX_EINVAL && c->log_error == NGX_ERROR_IGNORE_EINVAL) {
        తిరిగి 0;
    }
#అయితే_ముగింపు

    ఒకవేళ (err == NGX_EMSGSIZE && c->log_error == NGX_ERROR_IGNORE_EMSGSIZE) {
        తిరిగి 0;
    }

#ఒకవేళ (NGX_LINUX)
    ఒకవేళ (err == NGX_EIO && c->ktls_recv) {
        తిరిగి 0;
    }
#అయితే_ముగింపు

    ఒకవేళ (err == 0
        || err == NGX_ECONNRESET
#ఒకవేళ (NGX_WIN32)
        || err == NGX_ECONNABORTED
#లేకపోతే
        || err == NGX_EPIPE
#అయితే_ముగింపు
        || err == NGX_ENOTCONN
        || err == NGX_ETIMEDOUT
        || err == NGX_ECONNREFUSED
        || err == NGX_ENETDOWN
        || err == NGX_ENETUNREACH
        || err == NGX_EHOSTDOWN
        || err == NGX_EHOSTUNREACH)
    {
        ఎంపిక (c->log_error) {

        స్థితి NGX_ERROR_IGNORE_EMSGSIZE:
        స్థితి NGX_ERROR_IGNORE_EINVAL:
        స్థితి NGX_ERROR_IGNORE_ECONNRESET:
        స్థితి NGX_ERROR_INFO:
            level = NGX_LOG_INFO;
            ఆపు;

        అప్రమేయ:
            level = NGX_LOG_ERR;
        }

    } లేకపోతే {
        level = NGX_LOG_ALERT;
    }

    ngx_log_error(level, c->సంవర్గమానం, err, పాఠ్యం);

    తిరిగి NGX_ERROR;
}
