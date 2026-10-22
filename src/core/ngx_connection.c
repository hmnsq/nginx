
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাৱ <ngx_config.h>
#অন্তর্ভাৱ <ngx_core.h>
#অন্তর্ভাৱ <ngx_event.h>


ngx_os_io_t  ngx_io;


স্থির শূন্য ngx_drain_connections(ngx_cycle_t *cycle);


ngx_listening_t *
ngx_create_listening(ngx_conf_t *cf, সংরচনা sockaddr *sockaddr,
    socklen_t socklen)
{
    size_t            len;
    ngx_listening_t  *ls;
    সংরচনা sockaddr  *sa;
    u_char            text[NGX_SOCKADDR_STRLEN];

    ls = ngx_array_ঠেলো(&cf->cycle->listening);
    যদি (ls == রিক্ত) {
        ঘূৰাই রিক্ত;
    }

    ngx_memzero(ls, পরিমাণ(ngx_listening_t));

    sa = ngx_palloc(cf->pool, socklen);
    যদি (sa == রিক্ত) {
        ঘূৰাই রিক্ত;
    }

    ngx_memcpy(sa, sockaddr, socklen);

    ls->sockaddr = sa;
    ls->socklen = socklen;

    len = ngx_sock_ntop(sa, socklen, text, NGX_SOCKADDR_STRLEN, 1);
    ls->addr_text.len = len;

    ৱিকল্প (ls->sockaddr->sa_family) {
#যদি (NGX_HAVE_INET6)
    পক্ষ AF_INET6:
        ls->addr_text_max_len = NGX_INET6_ADDRSTRLEN;
        ভঙ্গ;
#endif
#যদি (NGX_HAVE_UNIX_DOMAIN)
    পক্ষ AF_UNIX:
        ls->addr_text_max_len = NGX_UNIX_ADDRSTRLEN;
        len++;
        ভঙ্গ;
#endif
    পক্ষ AF_INET:
        ls->addr_text_max_len = NGX_INET_ADDRSTRLEN;
        ভঙ্গ;
    স্ৱভাৱ:
        ls->addr_text_max_len = NGX_SOCKADDR_STRLEN;
        ভঙ্গ;
    }

    ls->addr_text.data = ngx_pnalloc(cf->pool, len);
    যদি (ls->addr_text.data == রিক্ত) {
        ঘূৰাই রিক্ত;
    }

    ngx_memcpy(ls->addr_text.data, text, len);

#যদি !(NGX_WIN32)
    ngx_rbtree_init(&ls->rbtree, &ls->sentinel, ngx_udp_rbtree_ঢোকাও_মান);
#endif

    ls->fd = (ngx_socket_t) -1;
    ls->type = SOCK_STREAM;

    ls->backlog = NGX_LISTEN_BACKLOG;
    ls->rcvbuf = -1;
    ls->sndbuf = -1;

#যদি (NGX_HAVE_SETFIB)
    ls->setfib = -1;
#endif

#যদি (NGX_HAVE_TCP_FASTOPEN)
    ls->fastopen = -1;
#endif

    ঘূৰাই ls;
}


ngx_int_t
ngx_clone_listening(ngx_cycle_t *cycle, ngx_listening_t *ls)
{
#যদি (NGX_HAVE_REUSEPORT)

    ngx_int_t         n;
    ngx_core_conf_t  *ccf;
    ngx_listening_t   ols;

    যদি (!ls->reuseport || ls->worker != 0) {
        ঘূৰাই NGX_OK;
    }

    ols = *ls;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    বাবে (n = 1; n < ccf->worker_processes; n++) {

        /* create a socket for each worker process */

        ls = ngx_array_ঠেলো(&cycle->listening);
        যদি (ls == রিক্ত) {
            ঘূৰাই NGX_ERROR;
        }

        *ls = ols;
        ls->worker = n;
    }

#endif

    ঘূৰাই NGX_OK;
}


ngx_int_t
ngx_set_inherited_sockets(ngx_cycle_t *cycle)
{
    size_t                     len;
    ngx_uint_t                 i;
    ngx_listening_t           *ls;
    socklen_t                  olen;
#যদি (NGX_HAVE_DEFERRED_ACCEPT || NGX_HAVE_TCP_FASTOPEN)
    ngx_err_t                  err;
#endif
#যদি (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    সংরচনা accept_filter_arg   af;
#endif
#যদি (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)
    পূর্ণাঙ্ক                        timeout;
#endif
#যদি (NGX_HAVE_REUSEPORT)
    পূর্ণাঙ্ক                        reuseport;
#endif

    ls = cycle->listening.elts;
    বাবে (i = 0; i < cycle->listening.nelts; i++) {

        ls[i].sockaddr = ngx_palloc(cycle->pool, পরিমাণ(ngx_sockaddr_t));
        যদি (ls[i].sockaddr == রিক্ত) {
            ঘূৰাই NGX_ERROR;
        }

        ls[i].socklen = পরিমাণ(ngx_sockaddr_t);
        যদি (getsockname(ls[i].fd, ls[i].sockaddr, &ls[i].socklen) == -1) {
            ngx_log_error(NGX_LOG_CRIT, cycle->log, ngx_socket_errno,
                          "getsockname() of the inherited "
                          "socket #%d failed", ls[i].fd);
            ls[i].ignore = 1;
            অনুৱর্ত;
        }

        যদি (ls[i].socklen > (socklen_t) পরিমাণ(ngx_sockaddr_t)) {
            ls[i].socklen = পরিমাণ(ngx_sockaddr_t);
        }

        ৱিকল্প (ls[i].sockaddr->sa_family) {

#যদি (NGX_HAVE_INET6)
        পক্ষ AF_INET6:
            ls[i].addr_text_max_len = NGX_INET6_ADDRSTRLEN;
            len = NGX_INET6_ADDRSTRLEN + পরিমাণ("[]:65535") - 1;
            ভঙ্গ;
#endif

#যদি (NGX_HAVE_UNIX_DOMAIN)
        পক্ষ AF_UNIX:
            ls[i].addr_text_max_len = NGX_UNIX_ADDRSTRLEN;
            len = NGX_UNIX_ADDRSTRLEN;
            ভঙ্গ;
#endif

        পক্ষ AF_INET:
            ls[i].addr_text_max_len = NGX_INET_ADDRSTRLEN;
            len = NGX_INET_ADDRSTRLEN + পরিমাণ(":65535") - 1;
            ভঙ্গ;

        স্ৱভাৱ:
            ngx_log_error(NGX_LOG_CRIT, cycle->log, ngx_socket_errno,
                          "the inherited socket #%d has "
                          "an unsupported protocol family", ls[i].fd);
            ls[i].ignore = 1;
            অনুৱর্ত;
        }

        ls[i].addr_text.data = ngx_pnalloc(cycle->pool, len);
        যদি (ls[i].addr_text.data == রিক্ত) {
            ঘূৰাই NGX_ERROR;
        }

        len = ngx_sock_ntop(ls[i].sockaddr, ls[i].socklen,
                            ls[i].addr_text.data, len, 1);
        যদি (len == 0) {
            ঘূৰাই NGX_ERROR;
        }

        ls[i].addr_text.len = len;

        ls[i].backlog = NGX_LISTEN_BACKLOG;

        olen = পরিমাণ(পূর্ণাঙ্ক);

        যদি (getsockopt(ls[i].fd, SOL_SOCKET, SO_TYPE, (শূন্য *) &ls[i].type,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_CRIT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_TYPE) %V failed", &ls[i].addr_text);
            ls[i].ignore = 1;
            অনুৱর্ত;
        }

        olen = পরিমাণ(পূর্ণাঙ্ক);

        যদি (getsockopt(ls[i].fd, SOL_SOCKET, SO_RCVBUF, (শূন্য *) &ls[i].rcvbuf,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_RCVBUF) %V failed, ignored",
                          &ls[i].addr_text);

            ls[i].rcvbuf = -1;
        }

        olen = পরিমাণ(পূর্ণাঙ্ক);

        যদি (getsockopt(ls[i].fd, SOL_SOCKET, SO_SNDBUF, (শূন্য *) &ls[i].sndbuf,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_SNDBUF) %V failed, ignored",
                          &ls[i].addr_text);

            ls[i].sndbuf = -1;
        }

#যদি 0
        /* SO_SETFIB is currently a set only option */

#যদি (NGX_HAVE_SETFIB)

        olen = পরিমাণ(পূর্ণাঙ্ক);

        যদি (getsockopt(ls[i].fd, SOL_SOCKET, SO_SETFIB,
                       (শূন্য *) &ls[i].setfib, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_SETFIB) %V failed, ignored",
                          &ls[i].addr_text);

            ls[i].setfib = -1;
        }

#endif
#endif

#যদি (NGX_HAVE_REUSEPORT)

        reuseport = 0;
        olen = পরিমাণ(পূর্ণাঙ্ক);

#ifdef SO_REUSEPORT_LB

        যদি (getsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT_LB,
                       (শূন্য *) &reuseport, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_REUSEPORT_LB) %V failed, ignored",
                          &ls[i].addr_text);

        } অন্যথা {
            ls[i].reuseport = reuseport ? 1 : 0;
        }

#অন্যথা

        যদি (getsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT,
                       (শূন্য *) &reuseport, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_REUSEPORT) %V failed, ignored",
                          &ls[i].addr_text);

        } অন্যথা {
            ls[i].reuseport = reuseport ? 1 : 0;
        }
#endif

#endif

        যদি (ls[i].type != SOCK_STREAM) {
            অনুৱর্ত;
        }

#ifdef SO_PROTOCOL

        olen = পরিমাণ(পূর্ণাঙ্ক);

        যদি (getsockopt(ls[i].fd, SOL_SOCKET, SO_PROTOCOL,
                       (শূন্য *) &ls[i].protocol, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_PROTOCOL) %V failed, ignored",
                          &ls[i].addr_text);
            ls[i].protocol = 0;

        } অন্যথা যদি (ls[i].protocol == IPPROTO_TCP) {
            ls[i].protocol = 0;
        }

#endif

#যদি (NGX_HAVE_TCP_FASTOPEN)

        olen = পরিমাণ(পূর্ণাঙ্ক);

        যদি (getsockopt(ls[i].fd, IPPROTO_TCP, TCP_FASTOPEN,
                       (শূন্য *) &ls[i].fastopen, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            যদি (err != NGX_EOPNOTSUPP && err != NGX_ENOPROTOOPT
                && err != NGX_EINVAL)
            {
                ngx_log_error(NGX_LOG_NOTICE, cycle->log, err,
                              "getsockopt(TCP_FASTOPEN) %V failed, ignored",
                              &ls[i].addr_text);
            }

            ls[i].fastopen = -1;
        }

#endif

#যদি (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)

        ngx_memzero(&af, পরিমাণ(সংরচনা accept_filter_arg));
        olen = পরিমাণ(সংরচনা accept_filter_arg);

        যদি (getsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER, &af, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            যদি (err == NGX_EINVAL) {
                অনুৱর্ত;
            }

            ngx_log_error(NGX_LOG_NOTICE, cycle->log, err,
                          "getsockopt(SO_ACCEPTFILTER) for %V failed, ignored",
                          &ls[i].addr_text);
            অনুৱর্ত;
        }

        যদি (olen < পরিমাণ(সংরচনা accept_filter_arg) || af.af_name[0] == '\0') {
            অনুৱর্ত;
        }

        ls[i].accept_filter = ngx_palloc(cycle->pool, 16);
        যদি (ls[i].accept_filter == রিক্ত) {
            ঘূৰাই NGX_ERROR;
        }

        (শূন্য) ngx_cpystrn((u_char *) ls[i].accept_filter,
                           (u_char *) af.af_name, 16);
#endif

#যদি (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)

        timeout = 0;
        olen = পরিমাণ(পূর্ণাঙ্ক);

        যদি (getsockopt(ls[i].fd, IPPROTO_TCP, TCP_DEFER_ACCEPT, &timeout, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            যদি (err == NGX_EOPNOTSUPP) {
                অনুৱর্ত;
            }

            ngx_log_error(NGX_LOG_NOTICE, cycle->log, err,
                          "getsockopt(TCP_DEFER_ACCEPT) for %V failed, ignored",
                          &ls[i].addr_text);
            অনুৱর্ত;
        }

        যদি (olen < পরিমাণ(পূর্ণাঙ্ক) || timeout == 0) {
            অনুৱর্ত;
        }

        ls[i].deferred_accept = 1;
#endif
    }

    ঘূৰাই NGX_OK;
}


ngx_int_t
ngx_open_listening_sockets(ngx_cycle_t *cycle)
{
    পূর্ণাঙ্ক               reuseaddr;
    ngx_uint_t        i, tries, failed;
    ngx_err_t         err;
    ngx_log_t        *log;
    ngx_socket_t      s;
    ngx_listening_t  *ls;

    reuseaddr = 1;
#যদি (NGX_SUPPRESS_WARN)
    failed = 0;
#endif

    log = cycle->log;

    /* TODO: configurable try number */

    বাবে (tries = 5; tries; tries--) {
        failed = 0;

        /* for each listening socket */

        ls = cycle->listening.elts;
        বাবে (i = 0; i < cycle->listening.nelts; i++) {

            যদি (ls[i].ignore) {
                অনুৱর্ত;
            }

#যদি (NGX_HAVE_REUSEPORT)

            যদি (ls[i].add_reuseport || ls[i].change_protocol) {

                /*
                 * to allow transition from a socket without SO_REUSEPORT
                 * to multiple sockets with SO_REUSEPORT, we have to set
                 * SO_REUSEPORT on the old socket before opening new ones
                 *
                 * to allow transition between different socket protocols
                 * (e.g. IPPROTO_MPTCP), SO_REUSEPORT is set on both old
                 * and new sockets
                 */

                পূর্ণাঙ্ক  reuseport = 1;

#ifdef SO_REUSEPORT_LB

                যদি (setsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT_LB,
                               (ধ্রুৱ শূন্য *) &reuseport, পরিমাণ(পূর্ণাঙ্ক))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT_LB) %V failed, "
                                  "ignored",
                                  &ls[i].addr_text);
                }

#অন্যথা

                যদি (setsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT,
                               (ধ্রুৱ শূন্য *) &reuseport, পরিমাণ(পূর্ণাঙ্ক))
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

            যদি (ls[i].fd != (ngx_socket_t) -1 && !ls[i].change_protocol) {
                অনুৱর্ত;
            }

            যদি (ls[i].inherited) {

                /* TODO: close on exit */
                /* TODO: nonblocking */
                /* TODO: deferred accept */

                অনুৱর্ত;
            }

            s = ngx_socket(ls[i].sockaddr->sa_family, ls[i].type,
                           ls[i].protocol);

            যদি (s == (ngx_socket_t) -1) {
                ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                              ngx_socket_n " %V failed", &ls[i].addr_text);
                ঘূৰাই NGX_ERROR;
            }

            যদি (ls[i].type != SOCK_DGRAM || !ngx_test_config) {

                যদি (setsockopt(s, SOL_SOCKET, SO_REUSEADDR,
                               (ধ্রুৱ শূন্য *) &reuseaddr, পরিমাণ(পূর্ণাঙ্ক))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEADDR) %V failed",
                                  &ls[i].addr_text);

                    যদি (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_text);
                    }

                    ঘূৰাই NGX_ERROR;
                }
            }

#যদি (NGX_HAVE_REUSEPORT)

            যদি ((ls[i].reuseport || ls[i].change_protocol)
                && !ngx_test_config)
            {
                পূর্ণাঙ্ক  reuseport;

                reuseport = 1;

#ifdef SO_REUSEPORT_LB

                যদি (setsockopt(s, SOL_SOCKET, SO_REUSEPORT_LB,
                               (ধ্রুৱ শূন্য *) &reuseport, পরিমাণ(পূর্ণাঙ্ক))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT_LB) %V failed",
                                  &ls[i].addr_text);

                    যদি (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_text);
                    }

                    ঘূৰাই NGX_ERROR;
                }

#অন্যথা

                যদি (setsockopt(s, SOL_SOCKET, SO_REUSEPORT,
                               (ধ্রুৱ শূন্য *) &reuseport, পরিমাণ(পূর্ণাঙ্ক))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT) %V failed",
                                  &ls[i].addr_text);

                    যদি (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_text);
                    }

                    ঘূৰাই NGX_ERROR;
                }
#endif
            }
#endif

#যদি (NGX_HAVE_INET6 && defined IPV6_V6ONLY)

            যদি (ls[i].sockaddr->sa_family == AF_INET6) {
                পূর্ণাঙ্ক  ipv6only;

                ipv6only = ls[i].ipv6only;

                যদি (setsockopt(s, IPPROTO_IPV6, IPV6_V6ONLY,
                               (ধ্রুৱ শূন্য *) &ipv6only, পরিমাণ(পূর্ণাঙ্ক))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(IPV6_V6ONLY) %V failed, ignored",
                                  &ls[i].addr_text);
                }
            }
#endif
            /* TODO: close on exit */

            যদি (ngx_nonblocking(s) == -1) {
                ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                              ngx_nonblocking_n " %V failed",
                              &ls[i].addr_text);

                যদি (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_text);
                }

                ঘূৰাই NGX_ERROR;
            }

            ngx_log_debug2(NGX_LOG_DEBUG_CORE, log, 0,
                           "bind() %V #%d ", &ls[i].addr_text, s);

            যদি (bind(s, ls[i].sockaddr, ls[i].socklen) == -1) {
                err = ngx_socket_errno;

                যদি (err != NGX_EADDRINUSE || !ngx_test_config) {
                    ngx_log_error(NGX_LOG_EMERG, log, err,
                                  "bind() to %V failed", &ls[i].addr_text);
                }

                যদি (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_text);
                }

                যদি (err != NGX_EADDRINUSE) {
                    ঘূৰাই NGX_ERROR;
                }

                যদি (!ngx_test_config) {
                    failed = 1;
                }

                অনুৱর্ত;
            }

#যদি (NGX_HAVE_UNIX_DOMAIN)

            যদি (ls[i].sockaddr->sa_family == AF_UNIX) {
                mode_t   mode;
                u_char  *name;

                name = ls[i].addr_text.data + পরিমাণ("unix:") - 1;
                mode = (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);

                যদি (chmod((ৱর্ণ *) name, mode) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  "chmod() \"%s\" failed", name);
                }

                যদি (ngx_test_config) {
                    যদি (ngx_delete_file(name) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                      ngx_delete_file_n " %s failed", name);
                    }
                }
            }
#endif

            যদি (ls[i].type != SOCK_STREAM) {
                ls[i].fd = s;
                ls[i].open = 1;
                অনুৱর্ত;
            }

            যদি (listen(s, ls[i].backlog) == -1) {
                err = ngx_socket_errno;

                /*
                 * on OpenVZ after suspend/resume EADDRINUSE
                 * may be returned by listen() instead of bind(), see
                 * https://bugs.openvz.org/browse/OVZ-5587
                 */

                যদি (err != NGX_EADDRINUSE || !ngx_test_config) {
                    ngx_log_error(NGX_LOG_EMERG, log, err,
                                  "listen() to %V, backlog %d failed",
                                  &ls[i].addr_text, ls[i].backlog);
                }

                যদি (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_text);
                }

                যদি (err != NGX_EADDRINUSE) {
                    ঘূৰাই NGX_ERROR;
                }

                যদি (!ngx_test_config) {
                    failed = 1;
                }

                অনুৱর্ত;
            }

            ls[i].listen = 1;

            ls[i].fd = s;
            ls[i].open = 1;
        }

        যদি (!failed) {
            ভঙ্গ;
        }

        /* TODO: delay configurable */

        ngx_log_error(NGX_LOG_NOTICE, log, 0,
                      "try again to bind() after 500ms");

        ngx_msleep(500);
    }

    যদি (failed) {
        ngx_log_error(NGX_LOG_EMERG, log, 0, "still could not bind()");
        ঘূৰাই NGX_ERROR;
    }

    ঘূৰাই NGX_OK;
}


শূন্য
ngx_configure_listening_sockets(ngx_cycle_t *cycle)
{
    পূর্ণাঙ্ক                        মান;
    ngx_uint_t                 i;
    ngx_listening_t           *ls;

#যদি (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    সংরচনা accept_filter_arg   af;
#endif

    ls = cycle->listening.elts;
    বাবে (i = 0; i < cycle->listening.nelts; i++) {

        ls[i].log = *ls[i].logp;

        যদি (ls[i].rcvbuf != -1) {
            যদি (setsockopt(ls[i].fd, SOL_SOCKET, SO_RCVBUF,
                           (ধ্রুৱ শূন্য *) &ls[i].rcvbuf, পরিমাণ(পূর্ণাঙ্ক))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_RCVBUF, %d) %V failed, ignored",
                              ls[i].rcvbuf, &ls[i].addr_text);
            }
        }

        যদি (ls[i].sndbuf != -1) {
            যদি (setsockopt(ls[i].fd, SOL_SOCKET, SO_SNDBUF,
                           (ধ্রুৱ শূন্য *) &ls[i].sndbuf, পরিমাণ(পূর্ণাঙ্ক))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_SNDBUF, %d) %V failed, ignored",
                              ls[i].sndbuf, &ls[i].addr_text);
            }
        }

        যদি (ls[i].keepalive) {
            মান = (ls[i].keepalive == 1) ? 1 : 0;

            যদি (setsockopt(ls[i].fd, SOL_SOCKET, SO_KEEPALIVE,
                           (ধ্রুৱ শূন্য *) &মান, পরিমাণ(পূর্ণাঙ্ক))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_KEEPALIVE, %d) %V failed, ignored",
                              মান, &ls[i].addr_text);
            }
        }

#যদি (NGX_HAVE_KEEPALIVE_TUNABLE)

#যদি !(NGX_DARWIN)

        যদি (ls[i].keepidle) {
            মান = ls[i].keepidle;

#যদি (NGX_KEEPALIVE_FACTOR)
            মান *= NGX_KEEPALIVE_FACTOR;
#endif

            যদি (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPIDLE,
                           (ধ্রুৱ শূন্য *) &মান, পরিমাণ(পূর্ণাঙ্ক))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_KEEPIDLE, %d) %V failed, ignored",
                              মান, &ls[i].addr_text);
            }
        }

#endif

        যদি (ls[i].keepintvl) {
            মান = ls[i].keepintvl;

#যদি (NGX_KEEPALIVE_FACTOR)
            মান *= NGX_KEEPALIVE_FACTOR;
#endif

            যদি (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPINTVL,
                           (ধ্রুৱ শূন্য *) &মান, পরিমাণ(পূর্ণাঙ্ক))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                             "setsockopt(TCP_KEEPINTVL, %d) %V failed, ignored",
                             মান, &ls[i].addr_text);
            }
        }

        যদি (ls[i].keepcnt) {
            যদি (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPCNT,
                           (ধ্রুৱ শূন্য *) &ls[i].keepcnt, পরিমাণ(পূর্ণাঙ্ক))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_KEEPCNT, %d) %V failed, ignored",
                              ls[i].keepcnt, &ls[i].addr_text);
            }
        }

#endif

#যদি (NGX_HAVE_SETFIB)
        যদি (ls[i].setfib != -1) {
            যদি (setsockopt(ls[i].fd, SOL_SOCKET, SO_SETFIB,
                           (ধ্রুৱ শূন্য *) &ls[i].setfib, পরিমাণ(পূর্ণাঙ্ক))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_SETFIB, %d) %V failed, ignored",
                              ls[i].setfib, &ls[i].addr_text);
            }
        }
#endif

#যদি (NGX_HAVE_TCP_FASTOPEN)
        যদি (ls[i].fastopen != -1) {
            যদি (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_FASTOPEN,
                           (ধ্রুৱ শূন্য *) &ls[i].fastopen, পরিমাণ(পূর্ণাঙ্ক))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_FASTOPEN, %d) %V failed, ignored",
                              ls[i].fastopen, &ls[i].addr_text);
            }
        }
#endif

#যদি 0
        যদি (1) {
            পূর্ণাঙ্ক tcp_nodelay = 1;

            যদি (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_NODELAY,
                       (ধ্রুৱ শূন্য *) &tcp_nodelay, পরিমাণ(পূর্ণাঙ্ক))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_NODELAY) %V failed, ignored",
                              &ls[i].addr_text);
            }
        }
#endif

        যদি (ls[i].listen) {

            /* change backlog via listen() */

            যদি (listen(ls[i].fd, ls[i].backlog) == -1) {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "listen() to %V, backlog %d failed, ignored",
                              &ls[i].addr_text, ls[i].backlog);
            }
        }

        /*
         * setting deferred mode should be last operation on socket,
         * because code may prematurely continue cycle on failure
         */

#যদি (NGX_HAVE_DEFERRED_ACCEPT)

#ifdef SO_ACCEPTFILTER

        যদি (ls[i].delete_deferred) {
            যদি (setsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER, রিক্ত, 0)
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_ACCEPTFILTER, NULL) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);

                যদি (ls[i].accept_filter) {
                    ngx_log_error(NGX_LOG_ALERT, cycle->log, 0,
                                  "could not change the accept filter "
                                  "to \"%s\" for %V, ignored",
                                  ls[i].accept_filter, &ls[i].addr_text);
                }

                অনুৱর্ত;
            }

            ls[i].deferred_accept = 0;
        }

        যদি (ls[i].add_deferred) {
            ngx_memzero(&af, পরিমাণ(সংরচনা accept_filter_arg));
            (শূন্য) ngx_cpystrn((u_char *) af.af_name,
                               (u_char *) ls[i].accept_filter, 16);

            যদি (setsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER,
                           &af, পরিমাণ(সংরচনা accept_filter_arg))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_ACCEPTFILTER, \"%s\") "
                              "for %V failed, ignored",
                              ls[i].accept_filter, &ls[i].addr_text);
                অনুৱর্ত;
            }

            ls[i].deferred_accept = 1;
        }

#endif

#ifdef TCP_DEFER_ACCEPT

        যদি (ls[i].add_deferred || ls[i].delete_deferred) {

            যদি (ls[i].add_deferred) {
                /*
                 * There is no way to find out how long a connection was
                 * in queue (and a connection may bypass deferred queue at all
                 * if syncookies were used), hence we use 1 second timeout
                 * here.
                 */
                মান = 1;

            } অন্যথা {
                মান = 0;
            }

            যদি (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_DEFER_ACCEPT,
                           &মান, পরিমাণ(পূর্ণাঙ্ক))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_DEFER_ACCEPT, %d) for %V failed, "
                              "ignored",
                              মান, &ls[i].addr_text);

                অনুৱর্ত;
            }
        }

        যদি (ls[i].add_deferred) {
            ls[i].deferred_accept = 1;
        }

#endif

#endif /* NGX_HAVE_DEFERRED_ACCEPT */

#যদি (NGX_HAVE_IP_RECVDSTADDR)

        যদি (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET)
        {
            মান = 1;

            যদি (setsockopt(ls[i].fd, IPPROTO_IP, IP_RECVDSTADDR,
                           (ধ্রুৱ শূন্য *) &মান, পরিমাণ(পূর্ণাঙ্ক))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_RECVDSTADDR) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#নোচেদ্যদি (NGX_HAVE_IP_PKTINFO)

        যদি (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET)
        {
            মান = 1;

            যদি (setsockopt(ls[i].fd, IPPROTO_IP, IP_PKTINFO,
                           (ধ্রুৱ শূন্য *) &মান, পরিমাণ(পূর্ণাঙ্ক))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_PKTINFO) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#endif

#যদি (NGX_HAVE_INET6 && NGX_HAVE_IPV6_RECVPKTINFO)

        যদি (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET6)
        {
            মান = 1;

            যদি (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_RECVPKTINFO,
                           (ধ্রুৱ শূন্য *) &মান, পরিমাণ(পূর্ণাঙ্ক))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IPV6_RECVPKTINFO) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#endif

#যদি (NGX_HAVE_IP_MTU_DISCOVER)

        যদি (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET) {
            মান = IP_PMTUDISC_DO;

            যদি (setsockopt(ls[i].fd, IPPROTO_IP, IP_MTU_DISCOVER,
                           (ধ্রুৱ শূন্য *) &মান, পরিমাণ(পূর্ণাঙ্ক))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_MTU_DISCOVER) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#নোচেদ্যদি (NGX_HAVE_IP_DONTFRAG)

        যদি (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET) {
            মান = 1;

            যদি (setsockopt(ls[i].fd, IPPROTO_IP, IP_DONTFRAG,
                           (ধ্রুৱ শূন্য *) &মান, পরিমাণ(পূর্ণাঙ্ক))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_DONTFRAG) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#endif

#যদি (NGX_HAVE_INET6)

#যদি (NGX_HAVE_IPV6_MTU_DISCOVER)

        যদি (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET6) {
            মান = IPV6_PMTUDISC_DO;

            যদি (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_MTU_DISCOVER,
                           (ধ্রুৱ শূন্য *) &মান, পরিমাণ(পূর্ণাঙ্ক))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IPV6_MTU_DISCOVER) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#নোচেদ্যদি (NGX_HAVE_IPV6_DONTFRAG)

        যদি (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET6) {
            মান = 1;

            যদি (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_DONTFRAG,
                           (ধ্রুৱ শূন্য *) &মান, পরিমাণ(পূর্ণাঙ্ক))
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

    ঘূৰাই;
}


শূন্য
ngx_close_listening_sockets(ngx_cycle_t *cycle)
{
    ngx_uint_t         i;
    ngx_listening_t   *ls;
    ngx_connection_t  *c;

    ngx_accept_mutex_held = 0;
    ngx_use_accept_mutex = 0;

    ls = cycle->listening.elts;
    বাবে (i = 0; i < cycle->listening.nelts; i++) {

#যদি (NGX_QUIC)
        যদি (ls[i].quic
            && (!ls[i].reuseport || !ngx_quic_bpf_enabled(cycle)))
        {
            অনুৱর্ত;
        }
#endif

        c = ls[i].connection;

        যদি (c) {
            যদি (c->পঠ->active) {
                যদি (ngx_event_flags & NGX_USE_EPOLL_EVENT) {

                    /*
                     * it seems that Linux-2.6.x OpenVZ sends events
                     * for closed shared listening sockets unless
                     * the events was explicitly deleted
                     */

                    ngx_del_event(c->পঠ, NGX_READ_EVENT, 0);

                } অন্যথা {
                    ngx_del_event(c->পঠ, NGX_READ_EVENT, NGX_CLOSE_EVENT);
                }
            }

            যদি (c->পঠ->timer_set) {
                ngx_del_timer(c->পঠ);
            }

            ngx_free_connection(c);

            c->fd = (ngx_socket_t) -1;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, cycle->log, 0,
                       "close listening %V #%d ", &ls[i].addr_text, ls[i].fd);

        যদি (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_socket_errno,
                          ngx_close_socket_n " %V failed", &ls[i].addr_text);
        }

#যদি (NGX_HAVE_UNIX_DOMAIN)

        যদি (ls[i].sockaddr->sa_family == AF_UNIX
            && ngx_process <= NGX_PROCESS_MASTER
            && ngx_new_দ্বিমিক == 0
            && (!ls[i].inherited || ngx_getppid() != ngx_parent))
        {
            u_char *name = ls[i].addr_text.data + পরিমাণ("unix:") - 1;

            যদি (ngx_delete_file(name) == NGX_FILE_ERROR) {
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

    যদি (ngx_cycle->files && (ngx_uint_t) s >= ngx_cycle->files_n) {
        ngx_log_error(NGX_LOG_ALERT, log, 0,
                      "the new socket has number %d, "
                      "but only %ui files are available",
                      s, ngx_cycle->files_n);
        ঘূৰাই রিক্ত;
    }

    ngx_drain_connections((ngx_cycle_t *) ngx_cycle);

    c = ngx_cycle->free_connections;

    যদি (c == রিক্ত) {
        ngx_log_error(NGX_LOG_ALERT, log, 0,
                      "%ui worker_connections are not enough",
                      ngx_cycle->connection_n);

        ঘূৰাই রিক্ত;
    }

    ngx_cycle->free_connections = c->data;
    ngx_cycle->free_connection_n--;

    যদি (ngx_cycle->files && ngx_cycle->files[s] == রিক্ত) {
        ngx_cycle->files[s] = c;
    }

    rev = c->পঠ;
    wev = c->লিখ;

    ngx_memzero(c, পরিমাণ(ngx_connection_t));

    c->পঠ = rev;
    c->লিখ = wev;
    c->fd = s;
    c->log = log;

    instance = rev->instance;

    ngx_memzero(rev, পরিমাণ(ngx_event_t));
    ngx_memzero(wev, পরিমাণ(ngx_event_t));

    rev->instance = !instance;
    wev->instance = !instance;

    rev->index = NGX_INVALID_INDEX;
    wev->index = NGX_INVALID_INDEX;

    rev->data = c;
    wev->data = c;

    wev->লিখ = 1;

    ঘূৰাই c;
}


শূন্য
ngx_free_connection(ngx_connection_t *c)
{
    c->data = ngx_cycle->free_connections;
    ngx_cycle->free_connections = c;
    ngx_cycle->free_connection_n++;

    যদি (ngx_cycle->files && ngx_cycle->files[c->fd] == c) {
        ngx_cycle->files[c->fd] = রিক্ত;
    }
}


শূন্য
ngx_close_connection(ngx_connection_t *c)
{
    ngx_err_t     err;
    ngx_uint_t    log_error, level;
    ngx_socket_t  fd;

    যদি (c->fd == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0, "connection already closed");
        ঘূৰাই;
    }

    যদি (c->পঠ->timer_set) {
        ngx_del_timer(c->পঠ);
    }

    যদি (c->লিখ->timer_set) {
        ngx_del_timer(c->লিখ);
    }

    যদি (!c->shared) {
        যদি (ngx_del_conn) {
            ngx_del_conn(c, NGX_CLOSE_EVENT);

        } অন্যথা {
            যদি (c->পঠ->active || c->পঠ->disabled) {
                ngx_del_event(c->পঠ, NGX_READ_EVENT, NGX_CLOSE_EVENT);
            }

            যদি (c->লিখ->active || c->লিখ->disabled) {
                ngx_del_event(c->লিখ, NGX_WRITE_EVENT, NGX_CLOSE_EVENT);
            }
        }
    }

    যদি (c->পঠ->posted) {
        ngx_delete_posted_event(c->পঠ);
    }

    যদি (c->লিখ->posted) {
        ngx_delete_posted_event(c->লিখ);
    }

    c->পঠ->closed = 1;
    c->লিখ->closed = 1;

    ngx_reusable_connection(c, 0);

    log_error = c->log_error;

    ngx_free_connection(c);

    fd = c->fd;
    c->fd = (ngx_socket_t) -1;

    যদি (c->shared) {
        ঘূৰাই;
    }

    যদি (ngx_close_socket(fd) == -1) {

        err = ngx_socket_errno;

        যদি (err == NGX_ECONNRESET || err == NGX_ENOTCONN) {

            ৱিকল্প (log_error) {

            পক্ষ NGX_ERROR_INFO:
                level = NGX_LOG_INFO;
                ভঙ্গ;

            পক্ষ NGX_ERROR_ERR:
                level = NGX_LOG_ERR;
                ভঙ্গ;

            স্ৱভাৱ:
                level = NGX_LOG_CRIT;
            }

        } অন্যথা {
            level = NGX_LOG_CRIT;
        }

        ngx_log_error(level, c->log, err, ngx_close_socket_n " %d failed", fd);
    }
}


শূন্য
ngx_reusable_connection(ngx_connection_t *c, ngx_uint_t reusable)
{
    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "reusable connection: %ui", reusable);

    যদি (c->reusable) {
        ngx_queue_remove(&c->queue);
        ngx_cycle->reusable_connections_n--;

#যদি (NGX_STAT_STUB)
        (শূন্য) ngx_atomic_fetch_add(ngx_stat_waiting, -1);
#endif
    }

    c->reusable = reusable;

    যদি (reusable) {
        /* need cast as ngx_cycle is volatile */

        ngx_queue_ঢোকাও_head(
            (ngx_queue_t *) &ngx_cycle->reusable_connections_queue, &c->queue);
        ngx_cycle->reusable_connections_n++;

#যদি (NGX_STAT_STUB)
        (শূন্য) ngx_atomic_fetch_add(ngx_stat_waiting, 1);
#endif
    }
}


স্থির শূন্য
ngx_drain_connections(ngx_cycle_t *cycle)
{
    ngx_uint_t         i, n;
    ngx_queue_t       *q;
    ngx_connection_t  *c;

    যদি (cycle->free_connection_n > cycle->connection_n / 16
        || cycle->reusable_connections_n == 0)
    {
        ঘূৰাই;
    }

    যদি (cycle->connections_reuse_time != ngx_time()) {
        cycle->connections_reuse_time = ngx_time();

        ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                      "%ui worker_connections are not enough, "
                      "reusing connections",
                      cycle->connection_n);
    }

    c = রিক্ত;
    n = ngx_max(ngx_min(32, cycle->reusable_connections_n / 8), 1);

    বাবে (i = 0; i < n; i++) {
        যদি (ngx_queue_empty(&cycle->reusable_connections_queue)) {
            ভঙ্গ;
        }

        q = ngx_queue_last(&cycle->reusable_connections_queue);
        c = ngx_queue_data(q, ngx_connection_t, queue);

        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "reusing connection");

        c->close = 1;
        c->পঠ->handler(c->পঠ);
    }

    যদি (cycle->free_connection_n == 0 && c && c->reusable) {

        /*
         * if no connections were freed, try to reuse the last
         * connection again: this should free it as long as
         * previous reuse moved it to lingering close
         */

        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "reusing connection again");

        c->close = 1;
        c->পঠ->handler(c->পঠ);
    }
}


শূন্য
ngx_close_idle_connections(ngx_cycle_t *cycle)
{
    ngx_uint_t         i;
    ngx_connection_t  *c;

    c = cycle->connections;

    বাবে (i = 0; i < cycle->connection_n; i++) {

        /* THREAD: lock */

        যদি (c[i].fd != (ngx_socket_t) -1 && c[i].idle) {
            c[i].close = 1;
            c[i].পঠ->handler(c[i].পঠ);
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
    সংরচনা sockaddr_in   *sin;
#যদি (NGX_HAVE_INET6)
    ngx_uint_t            i;
    সংরচনা sockaddr_in6  *sin6;
#endif

    addr = 0;

    যদি (c->local_socklen) {
        ৱিকল্প (c->local_sockaddr->sa_family) {

#যদি (NGX_HAVE_INET6)
        পক্ষ AF_INET6:
            sin6 = (সংরচনা sockaddr_in6 *) c->local_sockaddr;

            বাবে (i = 0; addr == 0 && i < 16; i++) {
                addr |= sin6->sin6_addr.s6_addr[i];
            }

            ভঙ্গ;
#endif

#যদি (NGX_HAVE_UNIX_DOMAIN)
        পক্ষ AF_UNIX:
            addr = 1;
            ভঙ্গ;
#endif

        স্ৱভাৱ: /* AF_INET */
            sin = (সংরচনা sockaddr_in *) c->local_sockaddr;
            addr = sin->sin_addr.s_addr;
            ভঙ্গ;
        }
    }

    যদি (addr == 0) {

        len = পরিমাণ(ngx_sockaddr_t);

        যদি (getsockname(c->fd, &sa.sockaddr, &len) == -1) {
            ngx_connection_error(c, ngx_socket_errno, "getsockname() failed");
            ঘূৰাই NGX_ERROR;
        }

        c->local_sockaddr = ngx_palloc(c->pool, len);
        যদি (c->local_sockaddr == রিক্ত) {
            ঘূৰাই NGX_ERROR;
        }

        ngx_memcpy(c->local_sockaddr, &sa, len);

        c->local_socklen = len;
    }

    যদি (s == রিক্ত) {
        ঘূৰাই NGX_OK;
    }

    s->len = ngx_sock_ntop(c->local_sockaddr, c->local_socklen,
                           s->data, s->len, port);

    ঘূৰাই NGX_OK;
}


ngx_int_t
ngx_tcp_nodelay(ngx_connection_t *c)
{
    পূর্ণাঙ্ক  tcp_nodelay;

    যদি (c->tcp_nodelay != NGX_TCP_NODELAY_UNSET) {
        ঘূৰাই NGX_OK;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0, "tcp_nodelay");

    tcp_nodelay = 1;

    যদি (setsockopt(c->fd, IPPROTO_TCP, TCP_NODELAY,
                   (ধ্রুৱ শূন্য *) &tcp_nodelay, পরিমাণ(পূর্ণাঙ্ক))
        == -1)
    {
#যদি (NGX_SOLARIS)
        যদি (c->log_error == NGX_ERROR_INFO) {

            /* Solaris returns EINVAL if a socket has been shut down */
            c->log_error = NGX_ERROR_IGNORE_EINVAL;

            ngx_connection_error(c, ngx_socket_errno,
                                 "setsockopt(TCP_NODELAY) failed");

            c->log_error = NGX_ERROR_INFO;

            ঘূৰাই NGX_ERROR;
        }
#endif

        ngx_connection_error(c, ngx_socket_errno,
                             "setsockopt(TCP_NODELAY) failed");
        ঘূৰাই NGX_ERROR;
    }

    c->tcp_nodelay = NGX_TCP_NODELAY_SET;

    ঘূৰাই NGX_OK;
}


ngx_int_t
ngx_connection_error(ngx_connection_t *c, ngx_err_t err, ৱর্ণ *text)
{
    ngx_uint_t  level;

    /* Winsock may return NGX_ECONNABORTED instead of NGX_ECONNRESET */

    যদি ((err == NGX_ECONNRESET
#যদি (NGX_WIN32)
         || err == NGX_ECONNABORTED
#endif
        ) && c->log_error == NGX_ERROR_IGNORE_ECONNRESET)
    {
        ঘূৰাই 0;
    }

#যদি (NGX_SOLARIS)
    যদি (err == NGX_EINVAL && c->log_error == NGX_ERROR_IGNORE_EINVAL) {
        ঘূৰাই 0;
    }
#endif

    যদি (err == NGX_EMSGSIZE && c->log_error == NGX_ERROR_IGNORE_EMSGSIZE) {
        ঘূৰাই 0;
    }

#যদি (NGX_LINUX)
    যদি (err == NGX_EIO && c->ktls_recv) {
        ঘূৰাই 0;
    }
#endif

    যদি (err == 0
        || err == NGX_ECONNRESET
#যদি (NGX_WIN32)
        || err == NGX_ECONNABORTED
#অন্যথা
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
        ৱিকল্প (c->log_error) {

        পক্ষ NGX_ERROR_IGNORE_EMSGSIZE:
        পক্ষ NGX_ERROR_IGNORE_EINVAL:
        পক্ষ NGX_ERROR_IGNORE_ECONNRESET:
        পক্ষ NGX_ERROR_INFO:
            level = NGX_LOG_INFO;
            ভঙ্গ;

        স্ৱভাৱ:
            level = NGX_LOG_ERR;
        }

    } অন্যথা {
        level = NGX_LOG_ALERT;
    }

    ngx_log_error(level, c->log, err, text);

    ঘূৰাই NGX_ERROR;
}
