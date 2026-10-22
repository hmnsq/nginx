
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>
#அந்தர்பாவ <ngx_event.h>


ngx_os_io_t  ngx_io;


நிலையான வெறுமை ngx_drain_connections(ngx_cycle_t *cycle);


ngx_listening_t *
ngx_create_listening(ngx_conf_t *cf, கட்டமைப்பு sockaddr *sockaddr,
    socklen_t socklen)
{
    size_t            len;
    ngx_listening_t  *ls;
    கட்டமைப்பு sockaddr  *sa;
    u_char            text[NGX_SOCKADDR_STRLEN];

    ls = ngx_array_தள்ளு(&cf->cycle->listening);
    எனில் (ls == வெற்று) {
        திரும்பு வெற்று;
    }

    ngx_memzero(ls, அளவு(ngx_listening_t));

    sa = ngx_palloc(cf->pool, socklen);
    எனில் (sa == வெற்று) {
        திரும்பு வெற்று;
    }

    ngx_memcpy(sa, sockaddr, socklen);

    ls->sockaddr = sa;
    ls->socklen = socklen;

    len = ngx_sock_ntop(sa, socklen, text, NGX_SOCKADDR_STRLEN, 1);
    ls->addr_text.len = len;

    தேர்வு (ls->sockaddr->sa_family) {
#எனில் (NGX_HAVE_INET6)
    நிலை AF_INET6:
        ls->addr_text_max_len = NGX_INET6_ADDRSTRLEN;
        நிறுத்து;
#endif
#எனில் (NGX_HAVE_UNIX_DOMAIN)
    நிலை AF_UNIX:
        ls->addr_text_max_len = NGX_UNIX_ADDRSTRLEN;
        len++;
        நிறுத்து;
#endif
    நிலை AF_INET:
        ls->addr_text_max_len = NGX_INET_ADDRSTRLEN;
        நிறுத்து;
    இயல்பு:
        ls->addr_text_max_len = NGX_SOCKADDR_STRLEN;
        நிறுத்து;
    }

    ls->addr_text.data = ngx_pnalloc(cf->pool, len);
    எனில் (ls->addr_text.data == வெற்று) {
        திரும்பு வெற்று;
    }

    ngx_memcpy(ls->addr_text.data, text, len);

#எனில் !(NGX_WIN32)
    ngx_rbtree_init(&ls->rbtree, &ls->sentinel, ngx_udp_rbtree_செருகு_மதிப்பு);
#endif

    ls->fd = (ngx_socket_t) -1;
    ls->type = SOCK_STREAM;

    ls->backlog = NGX_LISTEN_BACKLOG;
    ls->rcvbuf = -1;
    ls->sndbuf = -1;

#எனில் (NGX_HAVE_SETFIB)
    ls->setfib = -1;
#endif

#எனில் (NGX_HAVE_TCP_FASTOPEN)
    ls->fastopen = -1;
#endif

    திரும்பு ls;
}


ngx_int_t
ngx_clone_listening(ngx_cycle_t *cycle, ngx_listening_t *ls)
{
#எனில் (NGX_HAVE_REUSEPORT)

    ngx_int_t         n;
    ngx_core_conf_t  *ccf;
    ngx_listening_t   ols;

    எனில் (!ls->reuseport || ls->worker != 0) {
        திரும்பு NGX_OK;
    }

    ols = *ls;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    க்கு (n = 1; n < ccf->worker_processes; n++) {

        /* create a socket for each worker process */

        ls = ngx_array_தள்ளு(&cycle->listening);
        எனில் (ls == வெற்று) {
            திரும்பு NGX_ERROR;
        }

        *ls = ols;
        ls->worker = n;
    }

#endif

    திரும்பு NGX_OK;
}


ngx_int_t
ngx_set_inherited_sockets(ngx_cycle_t *cycle)
{
    size_t                     len;
    ngx_uint_t                 i;
    ngx_listening_t           *ls;
    socklen_t                  olen;
#எனில் (NGX_HAVE_DEFERRED_ACCEPT || NGX_HAVE_TCP_FASTOPEN)
    ngx_err_t                  err;
#endif
#எனில் (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    கட்டமைப்பு accept_filter_arg   af;
#endif
#எனில் (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)
    முழு                        timeout;
#endif
#எனில் (NGX_HAVE_REUSEPORT)
    முழு                        reuseport;
#endif

    ls = cycle->listening.elts;
    க்கு (i = 0; i < cycle->listening.nelts; i++) {

        ls[i].sockaddr = ngx_palloc(cycle->pool, அளவு(ngx_sockaddr_t));
        எனில் (ls[i].sockaddr == வெற்று) {
            திரும்பு NGX_ERROR;
        }

        ls[i].socklen = அளவு(ngx_sockaddr_t);
        எனில் (getsockname(ls[i].fd, ls[i].sockaddr, &ls[i].socklen) == -1) {
            ngx_log_error(NGX_LOG_CRIT, cycle->log, ngx_socket_errno,
                          "getsockname() of the inherited "
                          "socket #%d failed", ls[i].fd);
            ls[i].ignore = 1;
            தொடர்;
        }

        எனில் (ls[i].socklen > (socklen_t) அளவு(ngx_sockaddr_t)) {
            ls[i].socklen = அளவு(ngx_sockaddr_t);
        }

        தேர்வு (ls[i].sockaddr->sa_family) {

#எனில் (NGX_HAVE_INET6)
        நிலை AF_INET6:
            ls[i].addr_text_max_len = NGX_INET6_ADDRSTRLEN;
            len = NGX_INET6_ADDRSTRLEN + அளவு("[]:65535") - 1;
            நிறுத்து;
#endif

#எனில் (NGX_HAVE_UNIX_DOMAIN)
        நிலை AF_UNIX:
            ls[i].addr_text_max_len = NGX_UNIX_ADDRSTRLEN;
            len = NGX_UNIX_ADDRSTRLEN;
            நிறுத்து;
#endif

        நிலை AF_INET:
            ls[i].addr_text_max_len = NGX_INET_ADDRSTRLEN;
            len = NGX_INET_ADDRSTRLEN + அளவு(":65535") - 1;
            நிறுத்து;

        இயல்பு:
            ngx_log_error(NGX_LOG_CRIT, cycle->log, ngx_socket_errno,
                          "the inherited socket #%d has "
                          "an unsupported protocol family", ls[i].fd);
            ls[i].ignore = 1;
            தொடர்;
        }

        ls[i].addr_text.data = ngx_pnalloc(cycle->pool, len);
        எனில் (ls[i].addr_text.data == வெற்று) {
            திரும்பு NGX_ERROR;
        }

        len = ngx_sock_ntop(ls[i].sockaddr, ls[i].socklen,
                            ls[i].addr_text.data, len, 1);
        எனில் (len == 0) {
            திரும்பு NGX_ERROR;
        }

        ls[i].addr_text.len = len;

        ls[i].backlog = NGX_LISTEN_BACKLOG;

        olen = அளவு(முழு);

        எனில் (getsockopt(ls[i].fd, SOL_SOCKET, SO_TYPE, (வெறுமை *) &ls[i].type,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_CRIT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_TYPE) %V failed", &ls[i].addr_text);
            ls[i].ignore = 1;
            தொடர்;
        }

        olen = அளவு(முழு);

        எனில் (getsockopt(ls[i].fd, SOL_SOCKET, SO_RCVBUF, (வெறுமை *) &ls[i].rcvbuf,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_RCVBUF) %V failed, ignored",
                          &ls[i].addr_text);

            ls[i].rcvbuf = -1;
        }

        olen = அளவு(முழு);

        எனில் (getsockopt(ls[i].fd, SOL_SOCKET, SO_SNDBUF, (வெறுமை *) &ls[i].sndbuf,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_SNDBUF) %V failed, ignored",
                          &ls[i].addr_text);

            ls[i].sndbuf = -1;
        }

#எனில் 0
        /* SO_SETFIB is currently a set only option */

#எனில் (NGX_HAVE_SETFIB)

        olen = அளவு(முழு);

        எனில் (getsockopt(ls[i].fd, SOL_SOCKET, SO_SETFIB,
                       (வெறுமை *) &ls[i].setfib, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_SETFIB) %V failed, ignored",
                          &ls[i].addr_text);

            ls[i].setfib = -1;
        }

#endif
#endif

#எனில் (NGX_HAVE_REUSEPORT)

        reuseport = 0;
        olen = அளவு(முழு);

#ifdef SO_REUSEPORT_LB

        எனில் (getsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT_LB,
                       (வெறுமை *) &reuseport, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_REUSEPORT_LB) %V failed, ignored",
                          &ls[i].addr_text);

        } இல்லையெனில் {
            ls[i].reuseport = reuseport ? 1 : 0;
        }

#இல்லையெனில்

        எனில் (getsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT,
                       (வெறுமை *) &reuseport, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_REUSEPORT) %V failed, ignored",
                          &ls[i].addr_text);

        } இல்லையெனில் {
            ls[i].reuseport = reuseport ? 1 : 0;
        }
#endif

#endif

        எனில் (ls[i].type != SOCK_STREAM) {
            தொடர்;
        }

#ifdef SO_PROTOCOL

        olen = அளவு(முழு);

        எனில் (getsockopt(ls[i].fd, SOL_SOCKET, SO_PROTOCOL,
                       (வெறுமை *) &ls[i].protocol, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_PROTOCOL) %V failed, ignored",
                          &ls[i].addr_text);
            ls[i].protocol = 0;

        } இல்லையெனில் எனில் (ls[i].protocol == IPPROTO_TCP) {
            ls[i].protocol = 0;
        }

#endif

#எனில் (NGX_HAVE_TCP_FASTOPEN)

        olen = அளவு(முழு);

        எனில் (getsockopt(ls[i].fd, IPPROTO_TCP, TCP_FASTOPEN,
                       (வெறுமை *) &ls[i].fastopen, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            எனில் (err != NGX_EOPNOTSUPP && err != NGX_ENOPROTOOPT
                && err != NGX_EINVAL)
            {
                ngx_log_error(NGX_LOG_NOTICE, cycle->log, err,
                              "getsockopt(TCP_FASTOPEN) %V failed, ignored",
                              &ls[i].addr_text);
            }

            ls[i].fastopen = -1;
        }

#endif

#எனில் (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)

        ngx_memzero(&af, அளவு(கட்டமைப்பு accept_filter_arg));
        olen = அளவு(கட்டமைப்பு accept_filter_arg);

        எனில் (getsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER, &af, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            எனில் (err == NGX_EINVAL) {
                தொடர்;
            }

            ngx_log_error(NGX_LOG_NOTICE, cycle->log, err,
                          "getsockopt(SO_ACCEPTFILTER) for %V failed, ignored",
                          &ls[i].addr_text);
            தொடர்;
        }

        எனில் (olen < அளவு(கட்டமைப்பு accept_filter_arg) || af.af_name[0] == '\0') {
            தொடர்;
        }

        ls[i].accept_filter = ngx_palloc(cycle->pool, 16);
        எனில் (ls[i].accept_filter == வெற்று) {
            திரும்பு NGX_ERROR;
        }

        (வெறுமை) ngx_cpystrn((u_char *) ls[i].accept_filter,
                           (u_char *) af.af_name, 16);
#endif

#எனில் (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)

        timeout = 0;
        olen = அளவு(முழு);

        எனில் (getsockopt(ls[i].fd, IPPROTO_TCP, TCP_DEFER_ACCEPT, &timeout, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            எனில் (err == NGX_EOPNOTSUPP) {
                தொடர்;
            }

            ngx_log_error(NGX_LOG_NOTICE, cycle->log, err,
                          "getsockopt(TCP_DEFER_ACCEPT) for %V failed, ignored",
                          &ls[i].addr_text);
            தொடர்;
        }

        எனில் (olen < அளவு(முழு) || timeout == 0) {
            தொடர்;
        }

        ls[i].deferred_accept = 1;
#endif
    }

    திரும்பு NGX_OK;
}


ngx_int_t
ngx_open_listening_sockets(ngx_cycle_t *cycle)
{
    முழு               reuseaddr;
    ngx_uint_t        i, tries, failed;
    ngx_err_t         err;
    ngx_log_t        *log;
    ngx_socket_t      s;
    ngx_listening_t  *ls;

    reuseaddr = 1;
#எனில் (NGX_SUPPRESS_WARN)
    failed = 0;
#endif

    log = cycle->log;

    /* TODO: configurable try number */

    க்கு (tries = 5; tries; tries--) {
        failed = 0;

        /* for each listening socket */

        ls = cycle->listening.elts;
        க்கு (i = 0; i < cycle->listening.nelts; i++) {

            எனில் (ls[i].ignore) {
                தொடர்;
            }

#எனில் (NGX_HAVE_REUSEPORT)

            எனில் (ls[i].add_reuseport || ls[i].change_protocol) {

                /*
                 * to allow transition from a socket without SO_REUSEPORT
                 * to multiple sockets with SO_REUSEPORT, we have to set
                 * SO_REUSEPORT on the old socket before opening new ones
                 *
                 * to allow transition between different socket protocols
                 * (e.g. IPPROTO_MPTCP), SO_REUSEPORT is set on both old
                 * and new sockets
                 */

                முழு  reuseport = 1;

#ifdef SO_REUSEPORT_LB

                எனில் (setsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT_LB,
                               (மாறிலி வெறுமை *) &reuseport, அளவு(முழு))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT_LB) %V failed, "
                                  "ignored",
                                  &ls[i].addr_text);
                }

#இல்லையெனில்

                எனில் (setsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT,
                               (மாறிலி வெறுமை *) &reuseport, அளவு(முழு))
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

            எனில் (ls[i].fd != (ngx_socket_t) -1 && !ls[i].change_protocol) {
                தொடர்;
            }

            எனில் (ls[i].inherited) {

                /* TODO: close on exit */
                /* TODO: nonblocking */
                /* TODO: deferred accept */

                தொடர்;
            }

            s = ngx_socket(ls[i].sockaddr->sa_family, ls[i].type,
                           ls[i].protocol);

            எனில் (s == (ngx_socket_t) -1) {
                ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                              ngx_socket_n " %V failed", &ls[i].addr_text);
                திரும்பு NGX_ERROR;
            }

            எனில் (ls[i].type != SOCK_DGRAM || !ngx_test_config) {

                எனில் (setsockopt(s, SOL_SOCKET, SO_REUSEADDR,
                               (மாறிலி வெறுமை *) &reuseaddr, அளவு(முழு))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEADDR) %V failed",
                                  &ls[i].addr_text);

                    எனில் (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_text);
                    }

                    திரும்பு NGX_ERROR;
                }
            }

#எனில் (NGX_HAVE_REUSEPORT)

            எனில் ((ls[i].reuseport || ls[i].change_protocol)
                && !ngx_test_config)
            {
                முழு  reuseport;

                reuseport = 1;

#ifdef SO_REUSEPORT_LB

                எனில் (setsockopt(s, SOL_SOCKET, SO_REUSEPORT_LB,
                               (மாறிலி வெறுமை *) &reuseport, அளவு(முழு))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT_LB) %V failed",
                                  &ls[i].addr_text);

                    எனில் (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_text);
                    }

                    திரும்பு NGX_ERROR;
                }

#இல்லையெனில்

                எனில் (setsockopt(s, SOL_SOCKET, SO_REUSEPORT,
                               (மாறிலி வெறுமை *) &reuseport, அளவு(முழு))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT) %V failed",
                                  &ls[i].addr_text);

                    எனில் (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_text);
                    }

                    திரும்பு NGX_ERROR;
                }
#endif
            }
#endif

#எனில் (NGX_HAVE_INET6 && defined IPV6_V6ONLY)

            எனில் (ls[i].sockaddr->sa_family == AF_INET6) {
                முழு  ipv6only;

                ipv6only = ls[i].ipv6only;

                எனில் (setsockopt(s, IPPROTO_IPV6, IPV6_V6ONLY,
                               (மாறிலி வெறுமை *) &ipv6only, அளவு(முழு))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(IPV6_V6ONLY) %V failed, ignored",
                                  &ls[i].addr_text);
                }
            }
#endif
            /* TODO: close on exit */

            எனில் (ngx_nonblocking(s) == -1) {
                ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                              ngx_nonblocking_n " %V failed",
                              &ls[i].addr_text);

                எனில் (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_text);
                }

                திரும்பு NGX_ERROR;
            }

            ngx_log_debug2(NGX_LOG_DEBUG_CORE, log, 0,
                           "bind() %V #%d ", &ls[i].addr_text, s);

            எனில் (bind(s, ls[i].sockaddr, ls[i].socklen) == -1) {
                err = ngx_socket_errno;

                எனில் (err != NGX_EADDRINUSE || !ngx_test_config) {
                    ngx_log_error(NGX_LOG_EMERG, log, err,
                                  "bind() to %V failed", &ls[i].addr_text);
                }

                எனில் (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_text);
                }

                எனில் (err != NGX_EADDRINUSE) {
                    திரும்பு NGX_ERROR;
                }

                எனில் (!ngx_test_config) {
                    failed = 1;
                }

                தொடர்;
            }

#எனில் (NGX_HAVE_UNIX_DOMAIN)

            எனில் (ls[i].sockaddr->sa_family == AF_UNIX) {
                mode_t   mode;
                u_char  *name;

                name = ls[i].addr_text.data + அளவு("unix:") - 1;
                mode = (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);

                எனில் (chmod((எழுத்து *) name, mode) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  "chmod() \"%s\" failed", name);
                }

                எனில் (ngx_test_config) {
                    எனில் (ngx_delete_file(name) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                      ngx_delete_file_n " %s failed", name);
                    }
                }
            }
#endif

            எனில் (ls[i].type != SOCK_STREAM) {
                ls[i].fd = s;
                ls[i].open = 1;
                தொடர்;
            }

            எனில் (listen(s, ls[i].backlog) == -1) {
                err = ngx_socket_errno;

                /*
                 * on OpenVZ after suspend/resume EADDRINUSE
                 * may be returned by listen() instead of bind(), see
                 * https://bugs.openvz.org/browse/OVZ-5587
                 */

                எனில் (err != NGX_EADDRINUSE || !ngx_test_config) {
                    ngx_log_error(NGX_LOG_EMERG, log, err,
                                  "listen() to %V, backlog %d failed",
                                  &ls[i].addr_text, ls[i].backlog);
                }

                எனில் (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_text);
                }

                எனில் (err != NGX_EADDRINUSE) {
                    திரும்பு NGX_ERROR;
                }

                எனில் (!ngx_test_config) {
                    failed = 1;
                }

                தொடர்;
            }

            ls[i].listen = 1;

            ls[i].fd = s;
            ls[i].open = 1;
        }

        எனில் (!failed) {
            நிறுத்து;
        }

        /* TODO: delay configurable */

        ngx_log_error(NGX_LOG_NOTICE, log, 0,
                      "try again to bind() after 500ms");

        ngx_msleep(500);
    }

    எனில் (failed) {
        ngx_log_error(NGX_LOG_EMERG, log, 0, "still could not bind()");
        திரும்பு NGX_ERROR;
    }

    திரும்பு NGX_OK;
}


வெறுமை
ngx_configure_listening_sockets(ngx_cycle_t *cycle)
{
    முழு                        மதிப்பு;
    ngx_uint_t                 i;
    ngx_listening_t           *ls;

#எனில் (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    கட்டமைப்பு accept_filter_arg   af;
#endif

    ls = cycle->listening.elts;
    க்கு (i = 0; i < cycle->listening.nelts; i++) {

        ls[i].log = *ls[i].logp;

        எனில் (ls[i].rcvbuf != -1) {
            எனில் (setsockopt(ls[i].fd, SOL_SOCKET, SO_RCVBUF,
                           (மாறிலி வெறுமை *) &ls[i].rcvbuf, அளவு(முழு))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_RCVBUF, %d) %V failed, ignored",
                              ls[i].rcvbuf, &ls[i].addr_text);
            }
        }

        எனில் (ls[i].sndbuf != -1) {
            எனில் (setsockopt(ls[i].fd, SOL_SOCKET, SO_SNDBUF,
                           (மாறிலி வெறுமை *) &ls[i].sndbuf, அளவு(முழு))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_SNDBUF, %d) %V failed, ignored",
                              ls[i].sndbuf, &ls[i].addr_text);
            }
        }

        எனில் (ls[i].keepalive) {
            மதிப்பு = (ls[i].keepalive == 1) ? 1 : 0;

            எனில் (setsockopt(ls[i].fd, SOL_SOCKET, SO_KEEPALIVE,
                           (மாறிலி வெறுமை *) &மதிப்பு, அளவு(முழு))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_KEEPALIVE, %d) %V failed, ignored",
                              மதிப்பு, &ls[i].addr_text);
            }
        }

#எனில் (NGX_HAVE_KEEPALIVE_TUNABLE)

#எனில் !(NGX_DARWIN)

        எனில் (ls[i].keepidle) {
            மதிப்பு = ls[i].keepidle;

#எனில் (NGX_KEEPALIVE_FACTOR)
            மதிப்பு *= NGX_KEEPALIVE_FACTOR;
#endif

            எனில் (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPIDLE,
                           (மாறிலி வெறுமை *) &மதிப்பு, அளவு(முழு))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_KEEPIDLE, %d) %V failed, ignored",
                              மதிப்பு, &ls[i].addr_text);
            }
        }

#endif

        எனில் (ls[i].keepintvl) {
            மதிப்பு = ls[i].keepintvl;

#எனில் (NGX_KEEPALIVE_FACTOR)
            மதிப்பு *= NGX_KEEPALIVE_FACTOR;
#endif

            எனில் (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPINTVL,
                           (மாறிலி வெறுமை *) &மதிப்பு, அளவு(முழு))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                             "setsockopt(TCP_KEEPINTVL, %d) %V failed, ignored",
                             மதிப்பு, &ls[i].addr_text);
            }
        }

        எனில் (ls[i].keepcnt) {
            எனில் (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPCNT,
                           (மாறிலி வெறுமை *) &ls[i].keepcnt, அளவு(முழு))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_KEEPCNT, %d) %V failed, ignored",
                              ls[i].keepcnt, &ls[i].addr_text);
            }
        }

#endif

#எனில் (NGX_HAVE_SETFIB)
        எனில் (ls[i].setfib != -1) {
            எனில் (setsockopt(ls[i].fd, SOL_SOCKET, SO_SETFIB,
                           (மாறிலி வெறுமை *) &ls[i].setfib, அளவு(முழு))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_SETFIB, %d) %V failed, ignored",
                              ls[i].setfib, &ls[i].addr_text);
            }
        }
#endif

#எனில் (NGX_HAVE_TCP_FASTOPEN)
        எனில் (ls[i].fastopen != -1) {
            எனில் (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_FASTOPEN,
                           (மாறிலி வெறுமை *) &ls[i].fastopen, அளவு(முழு))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_FASTOPEN, %d) %V failed, ignored",
                              ls[i].fastopen, &ls[i].addr_text);
            }
        }
#endif

#எனில் 0
        எனில் (1) {
            முழு tcp_nodelay = 1;

            எனில் (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_NODELAY,
                       (மாறிலி வெறுமை *) &tcp_nodelay, அளவு(முழு))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_NODELAY) %V failed, ignored",
                              &ls[i].addr_text);
            }
        }
#endif

        எனில் (ls[i].listen) {

            /* change backlog via listen() */

            எனில் (listen(ls[i].fd, ls[i].backlog) == -1) {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "listen() to %V, backlog %d failed, ignored",
                              &ls[i].addr_text, ls[i].backlog);
            }
        }

        /*
         * setting deferred mode should be last operation on socket,
         * because code may prematurely continue cycle on failure
         */

#எனில் (NGX_HAVE_DEFERRED_ACCEPT)

#ifdef SO_ACCEPTFILTER

        எனில் (ls[i].delete_deferred) {
            எனில் (setsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER, வெற்று, 0)
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_ACCEPTFILTER, NULL) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);

                எனில் (ls[i].accept_filter) {
                    ngx_log_error(NGX_LOG_ALERT, cycle->log, 0,
                                  "could not change the accept filter "
                                  "to \"%s\" for %V, ignored",
                                  ls[i].accept_filter, &ls[i].addr_text);
                }

                தொடர்;
            }

            ls[i].deferred_accept = 0;
        }

        எனில் (ls[i].add_deferred) {
            ngx_memzero(&af, அளவு(கட்டமைப்பு accept_filter_arg));
            (வெறுமை) ngx_cpystrn((u_char *) af.af_name,
                               (u_char *) ls[i].accept_filter, 16);

            எனில் (setsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER,
                           &af, அளவு(கட்டமைப்பு accept_filter_arg))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_ACCEPTFILTER, \"%s\") "
                              "for %V failed, ignored",
                              ls[i].accept_filter, &ls[i].addr_text);
                தொடர்;
            }

            ls[i].deferred_accept = 1;
        }

#endif

#ifdef TCP_DEFER_ACCEPT

        எனில் (ls[i].add_deferred || ls[i].delete_deferred) {

            எனில் (ls[i].add_deferred) {
                /*
                 * There is no way to find out how long a connection was
                 * in queue (and a connection may bypass deferred queue at all
                 * if syncookies were used), hence we use 1 second timeout
                 * here.
                 */
                மதிப்பு = 1;

            } இல்லையெனில் {
                மதிப்பு = 0;
            }

            எனில் (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_DEFER_ACCEPT,
                           &மதிப்பு, அளவு(முழு))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_DEFER_ACCEPT, %d) for %V failed, "
                              "ignored",
                              மதிப்பு, &ls[i].addr_text);

                தொடர்;
            }
        }

        எனில் (ls[i].add_deferred) {
            ls[i].deferred_accept = 1;
        }

#endif

#endif /* NGX_HAVE_DEFERRED_ACCEPT */

#எனில் (NGX_HAVE_IP_RECVDSTADDR)

        எனில் (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET)
        {
            மதிப்பு = 1;

            எனில் (setsockopt(ls[i].fd, IPPROTO_IP, IP_RECVDSTADDR,
                           (மாறிலி வெறுமை *) &மதிப்பு, அளவு(முழு))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_RECVDSTADDR) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#அல்லதெனில் (NGX_HAVE_IP_PKTINFO)

        எனில் (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET)
        {
            மதிப்பு = 1;

            எனில் (setsockopt(ls[i].fd, IPPROTO_IP, IP_PKTINFO,
                           (மாறிலி வெறுமை *) &மதிப்பு, அளவு(முழு))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_PKTINFO) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#endif

#எனில் (NGX_HAVE_INET6 && NGX_HAVE_IPV6_RECVPKTINFO)

        எனில் (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET6)
        {
            மதிப்பு = 1;

            எனில் (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_RECVPKTINFO,
                           (மாறிலி வெறுமை *) &மதிப்பு, அளவு(முழு))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IPV6_RECVPKTINFO) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#endif

#எனில் (NGX_HAVE_IP_MTU_DISCOVER)

        எனில் (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET) {
            மதிப்பு = IP_PMTUDISC_DO;

            எனில் (setsockopt(ls[i].fd, IPPROTO_IP, IP_MTU_DISCOVER,
                           (மாறிலி வெறுமை *) &மதிப்பு, அளவு(முழு))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_MTU_DISCOVER) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#அல்லதெனில் (NGX_HAVE_IP_DONTFRAG)

        எனில் (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET) {
            மதிப்பு = 1;

            எனில் (setsockopt(ls[i].fd, IPPROTO_IP, IP_DONTFRAG,
                           (மாறிலி வெறுமை *) &மதிப்பு, அளவு(முழு))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_DONTFRAG) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#endif

#எனில் (NGX_HAVE_INET6)

#எனில் (NGX_HAVE_IPV6_MTU_DISCOVER)

        எனில் (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET6) {
            மதிப்பு = IPV6_PMTUDISC_DO;

            எனில் (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_MTU_DISCOVER,
                           (மாறிலி வெறுமை *) &மதிப்பு, அளவு(முழு))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IPV6_MTU_DISCOVER) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#அல்லதெனில் (NGX_HAVE_IPV6_DONTFRAG)

        எனில் (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET6) {
            மதிப்பு = 1;

            எனில் (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_DONTFRAG,
                           (மாறிலி வெறுமை *) &மதிப்பு, அளவு(முழு))
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

    திரும்பு;
}


வெறுமை
ngx_close_listening_sockets(ngx_cycle_t *cycle)
{
    ngx_uint_t         i;
    ngx_listening_t   *ls;
    ngx_connection_t  *c;

    ngx_accept_mutex_held = 0;
    ngx_use_accept_mutex = 0;

    ls = cycle->listening.elts;
    க்கு (i = 0; i < cycle->listening.nelts; i++) {

#எனில் (NGX_QUIC)
        எனில் (ls[i].quic
            && (!ls[i].reuseport || !ngx_quic_bpf_enabled(cycle)))
        {
            தொடர்;
        }
#endif

        c = ls[i].connection;

        எனில் (c) {
            எனில் (c->பட->active) {
                எனில் (ngx_event_flags & NGX_USE_EPOLL_EVENT) {

                    /*
                     * it seems that Linux-2.6.x OpenVZ sends events
                     * for closed shared listening sockets unless
                     * the events was explicitly deleted
                     */

                    ngx_del_event(c->பட, NGX_READ_EVENT, 0);

                } இல்லையெனில் {
                    ngx_del_event(c->பட, NGX_READ_EVENT, NGX_CLOSE_EVENT);
                }
            }

            எனில் (c->பட->timer_set) {
                ngx_del_timer(c->பட);
            }

            ngx_free_connection(c);

            c->fd = (ngx_socket_t) -1;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, cycle->log, 0,
                       "close listening %V #%d ", &ls[i].addr_text, ls[i].fd);

        எனில் (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_socket_errno,
                          ngx_close_socket_n " %V failed", &ls[i].addr_text);
        }

#எனில் (NGX_HAVE_UNIX_DOMAIN)

        எனில் (ls[i].sockaddr->sa_family == AF_UNIX
            && ngx_process <= NGX_PROCESS_MASTER
            && ngx_new_இருமம் == 0
            && (!ls[i].inherited || ngx_getppid() != ngx_parent))
        {
            u_char *name = ls[i].addr_text.data + அளவு("unix:") - 1;

            எனில் (ngx_delete_file(name) == NGX_FILE_ERROR) {
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

    எனில் (ngx_cycle->files && (ngx_uint_t) s >= ngx_cycle->files_n) {
        ngx_log_error(NGX_LOG_ALERT, log, 0,
                      "the new socket has number %d, "
                      "but only %ui files are available",
                      s, ngx_cycle->files_n);
        திரும்பு வெற்று;
    }

    ngx_drain_connections((ngx_cycle_t *) ngx_cycle);

    c = ngx_cycle->free_connections;

    எனில் (c == வெற்று) {
        ngx_log_error(NGX_LOG_ALERT, log, 0,
                      "%ui worker_connections are not enough",
                      ngx_cycle->connection_n);

        திரும்பு வெற்று;
    }

    ngx_cycle->free_connections = c->data;
    ngx_cycle->free_connection_n--;

    எனில் (ngx_cycle->files && ngx_cycle->files[s] == வெற்று) {
        ngx_cycle->files[s] = c;
    }

    rev = c->பட;
    wev = c->லிக;

    ngx_memzero(c, அளவு(ngx_connection_t));

    c->பட = rev;
    c->லிக = wev;
    c->fd = s;
    c->log = log;

    instance = rev->instance;

    ngx_memzero(rev, அளவு(ngx_event_t));
    ngx_memzero(wev, அளவு(ngx_event_t));

    rev->instance = !instance;
    wev->instance = !instance;

    rev->index = NGX_INVALID_INDEX;
    wev->index = NGX_INVALID_INDEX;

    rev->data = c;
    wev->data = c;

    wev->லிக = 1;

    திரும்பு c;
}


வெறுமை
ngx_free_connection(ngx_connection_t *c)
{
    c->data = ngx_cycle->free_connections;
    ngx_cycle->free_connections = c;
    ngx_cycle->free_connection_n++;

    எனில் (ngx_cycle->files && ngx_cycle->files[c->fd] == c) {
        ngx_cycle->files[c->fd] = வெற்று;
    }
}


வெறுமை
ngx_close_connection(ngx_connection_t *c)
{
    ngx_err_t     err;
    ngx_uint_t    log_error, level;
    ngx_socket_t  fd;

    எனில் (c->fd == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0, "connection already closed");
        திரும்பு;
    }

    எனில் (c->பட->timer_set) {
        ngx_del_timer(c->பட);
    }

    எனில் (c->லிக->timer_set) {
        ngx_del_timer(c->லிக);
    }

    எனில் (!c->shared) {
        எனில் (ngx_del_conn) {
            ngx_del_conn(c, NGX_CLOSE_EVENT);

        } இல்லையெனில் {
            எனில் (c->பட->active || c->பட->disabled) {
                ngx_del_event(c->பட, NGX_READ_EVENT, NGX_CLOSE_EVENT);
            }

            எனில் (c->லிக->active || c->லிக->disabled) {
                ngx_del_event(c->லிக, NGX_WRITE_EVENT, NGX_CLOSE_EVENT);
            }
        }
    }

    எனில் (c->பட->posted) {
        ngx_delete_posted_event(c->பட);
    }

    எனில் (c->லிக->posted) {
        ngx_delete_posted_event(c->லிக);
    }

    c->பட->closed = 1;
    c->லிக->closed = 1;

    ngx_reusable_connection(c, 0);

    log_error = c->log_error;

    ngx_free_connection(c);

    fd = c->fd;
    c->fd = (ngx_socket_t) -1;

    எனில் (c->shared) {
        திரும்பு;
    }

    எனில் (ngx_close_socket(fd) == -1) {

        err = ngx_socket_errno;

        எனில் (err == NGX_ECONNRESET || err == NGX_ENOTCONN) {

            தேர்வு (log_error) {

            நிலை NGX_ERROR_INFO:
                level = NGX_LOG_INFO;
                நிறுத்து;

            நிலை NGX_ERROR_ERR:
                level = NGX_LOG_ERR;
                நிறுத்து;

            இயல்பு:
                level = NGX_LOG_CRIT;
            }

        } இல்லையெனில் {
            level = NGX_LOG_CRIT;
        }

        ngx_log_error(level, c->log, err, ngx_close_socket_n " %d failed", fd);
    }
}


வெறுமை
ngx_reusable_connection(ngx_connection_t *c, ngx_uint_t reusable)
{
    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "reusable connection: %ui", reusable);

    எனில் (c->reusable) {
        ngx_queue_remove(&c->queue);
        ngx_cycle->reusable_connections_n--;

#எனில் (NGX_STAT_STUB)
        (வெறுமை) ngx_atomic_fetch_add(ngx_stat_waiting, -1);
#endif
    }

    c->reusable = reusable;

    எனில் (reusable) {
        /* need cast as ngx_cycle is volatile */

        ngx_queue_செருகு_head(
            (ngx_queue_t *) &ngx_cycle->reusable_connections_queue, &c->queue);
        ngx_cycle->reusable_connections_n++;

#எனில் (NGX_STAT_STUB)
        (வெறுமை) ngx_atomic_fetch_add(ngx_stat_waiting, 1);
#endif
    }
}


நிலையான வெறுமை
ngx_drain_connections(ngx_cycle_t *cycle)
{
    ngx_uint_t         i, n;
    ngx_queue_t       *q;
    ngx_connection_t  *c;

    எனில் (cycle->free_connection_n > cycle->connection_n / 16
        || cycle->reusable_connections_n == 0)
    {
        திரும்பு;
    }

    எனில் (cycle->connections_reuse_time != ngx_time()) {
        cycle->connections_reuse_time = ngx_time();

        ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                      "%ui worker_connections are not enough, "
                      "reusing connections",
                      cycle->connection_n);
    }

    c = வெற்று;
    n = ngx_max(ngx_min(32, cycle->reusable_connections_n / 8), 1);

    க்கு (i = 0; i < n; i++) {
        எனில் (ngx_queue_empty(&cycle->reusable_connections_queue)) {
            நிறுத்து;
        }

        q = ngx_queue_last(&cycle->reusable_connections_queue);
        c = ngx_queue_data(q, ngx_connection_t, queue);

        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "reusing connection");

        c->close = 1;
        c->பட->handler(c->பட);
    }

    எனில் (cycle->free_connection_n == 0 && c && c->reusable) {

        /*
         * if no connections were freed, try to reuse the last
         * connection again: this should free it as long as
         * previous reuse moved it to lingering close
         */

        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "reusing connection again");

        c->close = 1;
        c->பட->handler(c->பட);
    }
}


வெறுமை
ngx_close_idle_connections(ngx_cycle_t *cycle)
{
    ngx_uint_t         i;
    ngx_connection_t  *c;

    c = cycle->connections;

    க்கு (i = 0; i < cycle->connection_n; i++) {

        /* THREAD: lock */

        எனில் (c[i].fd != (ngx_socket_t) -1 && c[i].idle) {
            c[i].close = 1;
            c[i].பட->handler(c[i].பட);
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
    கட்டமைப்பு sockaddr_in   *sin;
#எனில் (NGX_HAVE_INET6)
    ngx_uint_t            i;
    கட்டமைப்பு sockaddr_in6  *sin6;
#endif

    addr = 0;

    எனில் (c->local_socklen) {
        தேர்வு (c->local_sockaddr->sa_family) {

#எனில் (NGX_HAVE_INET6)
        நிலை AF_INET6:
            sin6 = (கட்டமைப்பு sockaddr_in6 *) c->local_sockaddr;

            க்கு (i = 0; addr == 0 && i < 16; i++) {
                addr |= sin6->sin6_addr.s6_addr[i];
            }

            நிறுத்து;
#endif

#எனில் (NGX_HAVE_UNIX_DOMAIN)
        நிலை AF_UNIX:
            addr = 1;
            நிறுத்து;
#endif

        இயல்பு: /* AF_INET */
            sin = (கட்டமைப்பு sockaddr_in *) c->local_sockaddr;
            addr = sin->sin_addr.s_addr;
            நிறுத்து;
        }
    }

    எனில் (addr == 0) {

        len = அளவு(ngx_sockaddr_t);

        எனில் (getsockname(c->fd, &sa.sockaddr, &len) == -1) {
            ngx_connection_error(c, ngx_socket_errno, "getsockname() failed");
            திரும்பு NGX_ERROR;
        }

        c->local_sockaddr = ngx_palloc(c->pool, len);
        எனில் (c->local_sockaddr == வெற்று) {
            திரும்பு NGX_ERROR;
        }

        ngx_memcpy(c->local_sockaddr, &sa, len);

        c->local_socklen = len;
    }

    எனில் (s == வெற்று) {
        திரும்பு NGX_OK;
    }

    s->len = ngx_sock_ntop(c->local_sockaddr, c->local_socklen,
                           s->data, s->len, port);

    திரும்பு NGX_OK;
}


ngx_int_t
ngx_tcp_nodelay(ngx_connection_t *c)
{
    முழு  tcp_nodelay;

    எனில் (c->tcp_nodelay != NGX_TCP_NODELAY_UNSET) {
        திரும்பு NGX_OK;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0, "tcp_nodelay");

    tcp_nodelay = 1;

    எனில் (setsockopt(c->fd, IPPROTO_TCP, TCP_NODELAY,
                   (மாறிலி வெறுமை *) &tcp_nodelay, அளவு(முழு))
        == -1)
    {
#எனில் (NGX_SOLARIS)
        எனில் (c->log_error == NGX_ERROR_INFO) {

            /* Solaris returns EINVAL if a socket has been shut down */
            c->log_error = NGX_ERROR_IGNORE_EINVAL;

            ngx_connection_error(c, ngx_socket_errno,
                                 "setsockopt(TCP_NODELAY) failed");

            c->log_error = NGX_ERROR_INFO;

            திரும்பு NGX_ERROR;
        }
#endif

        ngx_connection_error(c, ngx_socket_errno,
                             "setsockopt(TCP_NODELAY) failed");
        திரும்பு NGX_ERROR;
    }

    c->tcp_nodelay = NGX_TCP_NODELAY_SET;

    திரும்பு NGX_OK;
}


ngx_int_t
ngx_connection_error(ngx_connection_t *c, ngx_err_t err, எழுத்து *text)
{
    ngx_uint_t  level;

    /* Winsock may return NGX_ECONNABORTED instead of NGX_ECONNRESET */

    எனில் ((err == NGX_ECONNRESET
#எனில் (NGX_WIN32)
         || err == NGX_ECONNABORTED
#endif
        ) && c->log_error == NGX_ERROR_IGNORE_ECONNRESET)
    {
        திரும்பு 0;
    }

#எனில் (NGX_SOLARIS)
    எனில் (err == NGX_EINVAL && c->log_error == NGX_ERROR_IGNORE_EINVAL) {
        திரும்பு 0;
    }
#endif

    எனில் (err == NGX_EMSGSIZE && c->log_error == NGX_ERROR_IGNORE_EMSGSIZE) {
        திரும்பு 0;
    }

#எனில் (NGX_LINUX)
    எனில் (err == NGX_EIO && c->ktls_recv) {
        திரும்பு 0;
    }
#endif

    எனில் (err == 0
        || err == NGX_ECONNRESET
#எனில் (NGX_WIN32)
        || err == NGX_ECONNABORTED
#இல்லையெனில்
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
        தேர்வு (c->log_error) {

        நிலை NGX_ERROR_IGNORE_EMSGSIZE:
        நிலை NGX_ERROR_IGNORE_EINVAL:
        நிலை NGX_ERROR_IGNORE_ECONNRESET:
        நிலை NGX_ERROR_INFO:
            level = NGX_LOG_INFO;
            நிறுத்து;

        இயல்பு:
            level = NGX_LOG_ERR;
        }

    } இல்லையெனில் {
        level = NGX_LOG_ALERT;
    }

    ngx_log_error(level, c->log, err, text);

    திரும்பு NGX_ERROR;
}
