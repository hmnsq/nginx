
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <ngx_event.h>


ngx_os_io_t  ngx_io;


ساكن فراغ ngx_drain_connections(ngx_cycle_t *cycle);


ngx_listening_t *
ngx_create_listening(ngx_conf_t *cf, بنية sockaddr *sockaddr,
    socklen_t socklen)
{
    size_t            len;
    ngx_listening_t  *ls;
    بنية sockaddr  *sa;
    u_char            text[NGX_SOCKADDR_STRLEN];

    ls = ngx_array_پاڵنان(&cf->cycle->listening);
    إذا (ls == عدم) {
        أرجع عدم;
    }

    ngx_memzero(ls, حجم(ngx_listening_t));

    sa = ngx_palloc(cf->pool, socklen);
    إذا (sa == عدم) {
        أرجع عدم;
    }

    ngx_memcpy(sa, sockaddr, socklen);

    ls->sockaddr = sa;
    ls->socklen = socklen;

    len = ngx_sock_ntop(sa, socklen, text, NGX_SOCKADDR_STRLEN, 1);
    ls->addr_text.len = len;

    بدل (ls->sockaddr->sa_family) {
#إذا (NGX_HAVE_INET6)
    حالة AF_INET6:
        ls->addr_text_max_len = NGX_INET6_ADDRSTRLEN;
        اكسر;
#endif
#إذا (NGX_HAVE_UNIX_DOMAIN)
    حالة AF_UNIX:
        ls->addr_text_max_len = NGX_UNIX_ADDRSTRLEN;
        len++;
        اكسر;
#endif
    حالة AF_INET:
        ls->addr_text_max_len = NGX_INET_ADDRSTRLEN;
        اكسر;
    افتراضي:
        ls->addr_text_max_len = NGX_SOCKADDR_STRLEN;
        اكسر;
    }

    ls->addr_text.data = ngx_pnalloc(cf->pool, len);
    إذا (ls->addr_text.data == عدم) {
        أرجع عدم;
    }

    ngx_memcpy(ls->addr_text.data, text, len);

#إذا !(NGX_WIN32)
    ngx_rbtree_init(&ls->rbtree, &ls->sentinel, ngx_udp_rbtree_زیادکردن_بەها);
#endif

    ls->fd = (ngx_socket_t) -1;
    ls->type = SOCK_STREAM;

    ls->backlog = NGX_LISTEN_BACKLOG;
    ls->rcvbuf = -1;
    ls->sndbuf = -1;

#إذا (NGX_HAVE_SETFIB)
    ls->setfib = -1;
#endif

#إذا (NGX_HAVE_TCP_FASTOPEN)
    ls->fastopen = -1;
#endif

    أرجع ls;
}


ngx_int_t
ngx_clone_listening(ngx_cycle_t *cycle, ngx_listening_t *ls)
{
#إذا (NGX_HAVE_REUSEPORT)

    ngx_int_t         n;
    ngx_core_conf_t  *ccf;
    ngx_listening_t   ols;

    إذا (!ls->reuseport || ls->worker != 0) {
        أرجع NGX_OK;
    }

    ols = *ls;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    لكل (n = 1; n < ccf->worker_processes; n++) {

        /* create a socket for each worker process */

        ls = ngx_array_پاڵنان(&cycle->listening);
        إذا (ls == عدم) {
            أرجع NGX_ERROR;
        }

        *ls = ols;
        ls->worker = n;
    }

#endif

    أرجع NGX_OK;
}


ngx_int_t
ngx_set_inherited_sockets(ngx_cycle_t *cycle)
{
    size_t                     len;
    ngx_uint_t                 i;
    ngx_listening_t           *ls;
    socklen_t                  olen;
#إذا (NGX_HAVE_DEFERRED_ACCEPT || NGX_HAVE_TCP_FASTOPEN)
    ngx_err_t                  err;
#endif
#إذا (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    بنية accept_filter_arg   af;
#endif
#إذا (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)
    صحيح                        timeout;
#endif
#إذا (NGX_HAVE_REUSEPORT)
    صحيح                        reuseport;
#endif

    ls = cycle->listening.elts;
    لكل (i = 0; i < cycle->listening.nelts; i++) {

        ls[i].sockaddr = ngx_palloc(cycle->pool, حجم(ngx_sockaddr_t));
        إذا (ls[i].sockaddr == عدم) {
            أرجع NGX_ERROR;
        }

        ls[i].socklen = حجم(ngx_sockaddr_t);
        إذا (getsockname(ls[i].fd, ls[i].sockaddr, &ls[i].socklen) == -1) {
            ngx_log_error(NGX_LOG_CRIT, cycle->log, ngx_socket_errno,
                          "getsockname() of the inherited "
                          "socket #%d failed", ls[i].fd);
            ls[i].ignore = 1;
            استمر;
        }

        إذا (ls[i].socklen > (socklen_t) حجم(ngx_sockaddr_t)) {
            ls[i].socklen = حجم(ngx_sockaddr_t);
        }

        بدل (ls[i].sockaddr->sa_family) {

#إذا (NGX_HAVE_INET6)
        حالة AF_INET6:
            ls[i].addr_text_max_len = NGX_INET6_ADDRSTRLEN;
            len = NGX_INET6_ADDRSTRLEN + حجم("[]:65535") - 1;
            اكسر;
#endif

#إذا (NGX_HAVE_UNIX_DOMAIN)
        حالة AF_UNIX:
            ls[i].addr_text_max_len = NGX_UNIX_ADDRSTRLEN;
            len = NGX_UNIX_ADDRSTRLEN;
            اكسر;
#endif

        حالة AF_INET:
            ls[i].addr_text_max_len = NGX_INET_ADDRSTRLEN;
            len = NGX_INET_ADDRSTRLEN + حجم(":65535") - 1;
            اكسر;

        افتراضي:
            ngx_log_error(NGX_LOG_CRIT, cycle->log, ngx_socket_errno,
                          "the inherited socket #%d has "
                          "an unsupported protocol family", ls[i].fd);
            ls[i].ignore = 1;
            استمر;
        }

        ls[i].addr_text.data = ngx_pnalloc(cycle->pool, len);
        إذا (ls[i].addr_text.data == عدم) {
            أرجع NGX_ERROR;
        }

        len = ngx_sock_ntop(ls[i].sockaddr, ls[i].socklen,
                            ls[i].addr_text.data, len, 1);
        إذا (len == 0) {
            أرجع NGX_ERROR;
        }

        ls[i].addr_text.len = len;

        ls[i].backlog = NGX_LISTEN_BACKLOG;

        olen = حجم(صحيح);

        إذا (getsockopt(ls[i].fd, SOL_SOCKET, SO_TYPE, (فراغ *) &ls[i].type,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_CRIT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_TYPE) %V failed", &ls[i].addr_text);
            ls[i].ignore = 1;
            استمر;
        }

        olen = حجم(صحيح);

        إذا (getsockopt(ls[i].fd, SOL_SOCKET, SO_RCVBUF, (فراغ *) &ls[i].rcvbuf,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_RCVBUF) %V failed, ignored",
                          &ls[i].addr_text);

            ls[i].rcvbuf = -1;
        }

        olen = حجم(صحيح);

        إذا (getsockopt(ls[i].fd, SOL_SOCKET, SO_SNDBUF, (فراغ *) &ls[i].sndbuf,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_SNDBUF) %V failed, ignored",
                          &ls[i].addr_text);

            ls[i].sndbuf = -1;
        }

#إذا 0
        /* SO_SETFIB is currently a set only option */

#إذا (NGX_HAVE_SETFIB)

        olen = حجم(صحيح);

        إذا (getsockopt(ls[i].fd, SOL_SOCKET, SO_SETFIB,
                       (فراغ *) &ls[i].setfib, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_SETFIB) %V failed, ignored",
                          &ls[i].addr_text);

            ls[i].setfib = -1;
        }

#endif
#endif

#إذا (NGX_HAVE_REUSEPORT)

        reuseport = 0;
        olen = حجم(صحيح);

#ifdef SO_REUSEPORT_LB

        إذا (getsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT_LB,
                       (فراغ *) &reuseport, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_REUSEPORT_LB) %V failed, ignored",
                          &ls[i].addr_text);

        } وإلا {
            ls[i].reuseport = reuseport ? 1 : 0;
        }

#وإلا

        إذا (getsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT,
                       (فراغ *) &reuseport, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_REUSEPORT) %V failed, ignored",
                          &ls[i].addr_text);

        } وإلا {
            ls[i].reuseport = reuseport ? 1 : 0;
        }
#endif

#endif

        إذا (ls[i].type != SOCK_STREAM) {
            استمر;
        }

#ifdef SO_PROTOCOL

        olen = حجم(صحيح);

        إذا (getsockopt(ls[i].fd, SOL_SOCKET, SO_PROTOCOL,
                       (فراغ *) &ls[i].protocol, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_PROTOCOL) %V failed, ignored",
                          &ls[i].addr_text);
            ls[i].protocol = 0;

        } وإلا إذا (ls[i].protocol == IPPROTO_TCP) {
            ls[i].protocol = 0;
        }

#endif

#إذا (NGX_HAVE_TCP_FASTOPEN)

        olen = حجم(صحيح);

        إذا (getsockopt(ls[i].fd, IPPROTO_TCP, TCP_FASTOPEN,
                       (فراغ *) &ls[i].fastopen, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            إذا (err != NGX_EOPNOTSUPP && err != NGX_ENOPROTOOPT
                && err != NGX_EINVAL)
            {
                ngx_log_error(NGX_LOG_NOTICE, cycle->log, err,
                              "getsockopt(TCP_FASTOPEN) %V failed, ignored",
                              &ls[i].addr_text);
            }

            ls[i].fastopen = -1;
        }

#endif

#إذا (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)

        ngx_memzero(&af, حجم(بنية accept_filter_arg));
        olen = حجم(بنية accept_filter_arg);

        إذا (getsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER, &af, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            إذا (err == NGX_EINVAL) {
                استمر;
            }

            ngx_log_error(NGX_LOG_NOTICE, cycle->log, err,
                          "getsockopt(SO_ACCEPTFILTER) for %V failed, ignored",
                          &ls[i].addr_text);
            استمر;
        }

        إذا (olen < حجم(بنية accept_filter_arg) || af.af_name[0] == '\0') {
            استمر;
        }

        ls[i].accept_filter = ngx_palloc(cycle->pool, 16);
        إذا (ls[i].accept_filter == عدم) {
            أرجع NGX_ERROR;
        }

        (فراغ) ngx_cpystrn((u_char *) ls[i].accept_filter,
                           (u_char *) af.af_name, 16);
#endif

#إذا (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)

        timeout = 0;
        olen = حجم(صحيح);

        إذا (getsockopt(ls[i].fd, IPPROTO_TCP, TCP_DEFER_ACCEPT, &timeout, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            إذا (err == NGX_EOPNOTSUPP) {
                استمر;
            }

            ngx_log_error(NGX_LOG_NOTICE, cycle->log, err,
                          "getsockopt(TCP_DEFER_ACCEPT) for %V failed, ignored",
                          &ls[i].addr_text);
            استمر;
        }

        إذا (olen < حجم(صحيح) || timeout == 0) {
            استمر;
        }

        ls[i].deferred_accept = 1;
#endif
    }

    أرجع NGX_OK;
}


ngx_int_t
ngx_open_listening_sockets(ngx_cycle_t *cycle)
{
    صحيح               reuseaddr;
    ngx_uint_t        i, tries, failed;
    ngx_err_t         err;
    ngx_log_t        *log;
    ngx_socket_t      s;
    ngx_listening_t  *ls;

    reuseaddr = 1;
#إذا (NGX_SUPPRESS_WARN)
    failed = 0;
#endif

    log = cycle->log;

    /* TODO: configurable try number */

    لكل (tries = 5; tries; tries--) {
        failed = 0;

        /* for each listening socket */

        ls = cycle->listening.elts;
        لكل (i = 0; i < cycle->listening.nelts; i++) {

            إذا (ls[i].ignore) {
                استمر;
            }

#إذا (NGX_HAVE_REUSEPORT)

            إذا (ls[i].add_reuseport || ls[i].change_protocol) {

                /*
                 * to allow transition from a socket without SO_REUSEPORT
                 * to multiple sockets with SO_REUSEPORT, we have to set
                 * SO_REUSEPORT on the old socket before opening new ones
                 *
                 * to allow transition between different socket protocols
                 * (e.g. IPPROTO_MPTCP), SO_REUSEPORT is set on both old
                 * and new sockets
                 */

                صحيح  reuseport = 1;

#ifdef SO_REUSEPORT_LB

                إذا (setsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT_LB,
                               (ثابت فراغ *) &reuseport, حجم(صحيح))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT_LB) %V failed, "
                                  "ignored",
                                  &ls[i].addr_text);
                }

#وإلا

                إذا (setsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT,
                               (ثابت فراغ *) &reuseport, حجم(صحيح))
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

            إذا (ls[i].fd != (ngx_socket_t) -1 && !ls[i].change_protocol) {
                استمر;
            }

            إذا (ls[i].inherited) {

                /* TODO: close on exit */
                /* TODO: nonblocking */
                /* TODO: deferred accept */

                استمر;
            }

            s = ngx_socket(ls[i].sockaddr->sa_family, ls[i].type,
                           ls[i].protocol);

            إذا (s == (ngx_socket_t) -1) {
                ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                              ngx_socket_n " %V failed", &ls[i].addr_text);
                أرجع NGX_ERROR;
            }

            إذا (ls[i].type != SOCK_DGRAM || !ngx_test_config) {

                إذا (setsockopt(s, SOL_SOCKET, SO_REUSEADDR,
                               (ثابت فراغ *) &reuseaddr, حجم(صحيح))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEADDR) %V failed",
                                  &ls[i].addr_text);

                    إذا (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_text);
                    }

                    أرجع NGX_ERROR;
                }
            }

#إذا (NGX_HAVE_REUSEPORT)

            إذا ((ls[i].reuseport || ls[i].change_protocol)
                && !ngx_test_config)
            {
                صحيح  reuseport;

                reuseport = 1;

#ifdef SO_REUSEPORT_LB

                إذا (setsockopt(s, SOL_SOCKET, SO_REUSEPORT_LB,
                               (ثابت فراغ *) &reuseport, حجم(صحيح))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT_LB) %V failed",
                                  &ls[i].addr_text);

                    إذا (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_text);
                    }

                    أرجع NGX_ERROR;
                }

#وإلا

                إذا (setsockopt(s, SOL_SOCKET, SO_REUSEPORT,
                               (ثابت فراغ *) &reuseport, حجم(صحيح))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT) %V failed",
                                  &ls[i].addr_text);

                    إذا (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_text);
                    }

                    أرجع NGX_ERROR;
                }
#endif
            }
#endif

#إذا (NGX_HAVE_INET6 && defined IPV6_V6ONLY)

            إذا (ls[i].sockaddr->sa_family == AF_INET6) {
                صحيح  ipv6only;

                ipv6only = ls[i].ipv6only;

                إذا (setsockopt(s, IPPROTO_IPV6, IPV6_V6ONLY,
                               (ثابت فراغ *) &ipv6only, حجم(صحيح))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(IPV6_V6ONLY) %V failed, ignored",
                                  &ls[i].addr_text);
                }
            }
#endif
            /* TODO: close on exit */

            إذا (ngx_nonblocking(s) == -1) {
                ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                              ngx_nonblocking_n " %V failed",
                              &ls[i].addr_text);

                إذا (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_text);
                }

                أرجع NGX_ERROR;
            }

            ngx_log_debug2(NGX_LOG_DEBUG_CORE, log, 0,
                           "bind() %V #%d ", &ls[i].addr_text, s);

            إذا (bind(s, ls[i].sockaddr, ls[i].socklen) == -1) {
                err = ngx_socket_errno;

                إذا (err != NGX_EADDRINUSE || !ngx_test_config) {
                    ngx_log_error(NGX_LOG_EMERG, log, err,
                                  "bind() to %V failed", &ls[i].addr_text);
                }

                إذا (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_text);
                }

                إذا (err != NGX_EADDRINUSE) {
                    أرجع NGX_ERROR;
                }

                إذا (!ngx_test_config) {
                    failed = 1;
                }

                استمر;
            }

#إذا (NGX_HAVE_UNIX_DOMAIN)

            إذا (ls[i].sockaddr->sa_family == AF_UNIX) {
                mode_t   mode;
                u_char  *name;

                name = ls[i].addr_text.data + حجم("unix:") - 1;
                mode = (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);

                إذا (chmod((حرف *) name, mode) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  "chmod() \"%s\" failed", name);
                }

                إذا (ngx_test_config) {
                    إذا (ngx_delete_file(name) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                      ngx_delete_file_n " %s failed", name);
                    }
                }
            }
#endif

            إذا (ls[i].type != SOCK_STREAM) {
                ls[i].fd = s;
                ls[i].open = 1;
                استمر;
            }

            إذا (listen(s, ls[i].backlog) == -1) {
                err = ngx_socket_errno;

                /*
                 * on OpenVZ after suspend/resume EADDRINUSE
                 * may be returned by listen() instead of bind(), see
                 * https://bugs.openvz.org/browse/OVZ-5587
                 */

                إذا (err != NGX_EADDRINUSE || !ngx_test_config) {
                    ngx_log_error(NGX_LOG_EMERG, log, err,
                                  "listen() to %V, backlog %d failed",
                                  &ls[i].addr_text, ls[i].backlog);
                }

                إذا (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_text);
                }

                إذا (err != NGX_EADDRINUSE) {
                    أرجع NGX_ERROR;
                }

                إذا (!ngx_test_config) {
                    failed = 1;
                }

                استمر;
            }

            ls[i].listen = 1;

            ls[i].fd = s;
            ls[i].open = 1;
        }

        إذا (!failed) {
            اكسر;
        }

        /* TODO: delay configurable */

        ngx_log_error(NGX_LOG_NOTICE, log, 0,
                      "try again to bind() after 500ms");

        ngx_msleep(500);
    }

    إذا (failed) {
        ngx_log_error(NGX_LOG_EMERG, log, 0, "still could not bind()");
        أرجع NGX_ERROR;
    }

    أرجع NGX_OK;
}


فراغ
ngx_configure_listening_sockets(ngx_cycle_t *cycle)
{
    صحيح                        بەها;
    ngx_uint_t                 i;
    ngx_listening_t           *ls;

#إذا (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    بنية accept_filter_arg   af;
#endif

    ls = cycle->listening.elts;
    لكل (i = 0; i < cycle->listening.nelts; i++) {

        ls[i].log = *ls[i].logp;

        إذا (ls[i].rcvbuf != -1) {
            إذا (setsockopt(ls[i].fd, SOL_SOCKET, SO_RCVBUF,
                           (ثابت فراغ *) &ls[i].rcvbuf, حجم(صحيح))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_RCVBUF, %d) %V failed, ignored",
                              ls[i].rcvbuf, &ls[i].addr_text);
            }
        }

        إذا (ls[i].sndbuf != -1) {
            إذا (setsockopt(ls[i].fd, SOL_SOCKET, SO_SNDBUF,
                           (ثابت فراغ *) &ls[i].sndbuf, حجم(صحيح))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_SNDBUF, %d) %V failed, ignored",
                              ls[i].sndbuf, &ls[i].addr_text);
            }
        }

        إذا (ls[i].keepalive) {
            بەها = (ls[i].keepalive == 1) ? 1 : 0;

            إذا (setsockopt(ls[i].fd, SOL_SOCKET, SO_KEEPALIVE,
                           (ثابت فراغ *) &بەها, حجم(صحيح))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_KEEPALIVE, %d) %V failed, ignored",
                              بەها, &ls[i].addr_text);
            }
        }

#إذا (NGX_HAVE_KEEPALIVE_TUNABLE)

#إذا !(NGX_DARWIN)

        إذا (ls[i].keepidle) {
            بەها = ls[i].keepidle;

#إذا (NGX_KEEPALIVE_FACTOR)
            بەها *= NGX_KEEPALIVE_FACTOR;
#endif

            إذا (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPIDLE,
                           (ثابت فراغ *) &بەها, حجم(صحيح))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_KEEPIDLE, %d) %V failed, ignored",
                              بەها, &ls[i].addr_text);
            }
        }

#endif

        إذا (ls[i].keepintvl) {
            بەها = ls[i].keepintvl;

#إذا (NGX_KEEPALIVE_FACTOR)
            بەها *= NGX_KEEPALIVE_FACTOR;
#endif

            إذا (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPINTVL,
                           (ثابت فراغ *) &بەها, حجم(صحيح))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                             "setsockopt(TCP_KEEPINTVL, %d) %V failed, ignored",
                             بەها, &ls[i].addr_text);
            }
        }

        إذا (ls[i].keepcnt) {
            إذا (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPCNT,
                           (ثابت فراغ *) &ls[i].keepcnt, حجم(صحيح))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_KEEPCNT, %d) %V failed, ignored",
                              ls[i].keepcnt, &ls[i].addr_text);
            }
        }

#endif

#إذا (NGX_HAVE_SETFIB)
        إذا (ls[i].setfib != -1) {
            إذا (setsockopt(ls[i].fd, SOL_SOCKET, SO_SETFIB,
                           (ثابت فراغ *) &ls[i].setfib, حجم(صحيح))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_SETFIB, %d) %V failed, ignored",
                              ls[i].setfib, &ls[i].addr_text);
            }
        }
#endif

#إذا (NGX_HAVE_TCP_FASTOPEN)
        إذا (ls[i].fastopen != -1) {
            إذا (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_FASTOPEN,
                           (ثابت فراغ *) &ls[i].fastopen, حجم(صحيح))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_FASTOPEN, %d) %V failed, ignored",
                              ls[i].fastopen, &ls[i].addr_text);
            }
        }
#endif

#إذا 0
        إذا (1) {
            صحيح tcp_nodelay = 1;

            إذا (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_NODELAY,
                       (ثابت فراغ *) &tcp_nodelay, حجم(صحيح))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_NODELAY) %V failed, ignored",
                              &ls[i].addr_text);
            }
        }
#endif

        إذا (ls[i].listen) {

            /* change backlog via listen() */

            إذا (listen(ls[i].fd, ls[i].backlog) == -1) {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "listen() to %V, backlog %d failed, ignored",
                              &ls[i].addr_text, ls[i].backlog);
            }
        }

        /*
         * setting deferred mode should be last operation on socket,
         * because code may prematurely continue cycle on failure
         */

#إذا (NGX_HAVE_DEFERRED_ACCEPT)

#ifdef SO_ACCEPTFILTER

        إذا (ls[i].delete_deferred) {
            إذا (setsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER, عدم, 0)
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_ACCEPTFILTER, NULL) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);

                إذا (ls[i].accept_filter) {
                    ngx_log_error(NGX_LOG_ALERT, cycle->log, 0,
                                  "could not change the accept filter "
                                  "to \"%s\" for %V, ignored",
                                  ls[i].accept_filter, &ls[i].addr_text);
                }

                استمر;
            }

            ls[i].deferred_accept = 0;
        }

        إذا (ls[i].add_deferred) {
            ngx_memzero(&af, حجم(بنية accept_filter_arg));
            (فراغ) ngx_cpystrn((u_char *) af.af_name,
                               (u_char *) ls[i].accept_filter, 16);

            إذا (setsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER,
                           &af, حجم(بنية accept_filter_arg))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_ACCEPTFILTER, \"%s\") "
                              "for %V failed, ignored",
                              ls[i].accept_filter, &ls[i].addr_text);
                استمر;
            }

            ls[i].deferred_accept = 1;
        }

#endif

#ifdef TCP_DEFER_ACCEPT

        إذا (ls[i].add_deferred || ls[i].delete_deferred) {

            إذا (ls[i].add_deferred) {
                /*
                 * There is no way to find out how long a connection was
                 * in queue (and a connection may bypass deferred queue at all
                 * if syncookies were used), hence we use 1 second timeout
                 * here.
                 */
                بەها = 1;

            } وإلا {
                بەها = 0;
            }

            إذا (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_DEFER_ACCEPT,
                           &بەها, حجم(صحيح))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_DEFER_ACCEPT, %d) for %V failed, "
                              "ignored",
                              بەها, &ls[i].addr_text);

                استمر;
            }
        }

        إذا (ls[i].add_deferred) {
            ls[i].deferred_accept = 1;
        }

#endif

#endif /* NGX_HAVE_DEFERRED_ACCEPT */

#إذا (NGX_HAVE_IP_RECVDSTADDR)

        إذا (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET)
        {
            بەها = 1;

            إذا (setsockopt(ls[i].fd, IPPROTO_IP, IP_RECVDSTADDR,
                           (ثابت فراغ *) &بەها, حجم(صحيح))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_RECVDSTADDR) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#وإلاإذا (NGX_HAVE_IP_PKTINFO)

        إذا (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET)
        {
            بەها = 1;

            إذا (setsockopt(ls[i].fd, IPPROTO_IP, IP_PKTINFO,
                           (ثابت فراغ *) &بەها, حجم(صحيح))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_PKTINFO) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#endif

#إذا (NGX_HAVE_INET6 && NGX_HAVE_IPV6_RECVPKTINFO)

        إذا (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET6)
        {
            بەها = 1;

            إذا (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_RECVPKTINFO,
                           (ثابت فراغ *) &بەها, حجم(صحيح))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IPV6_RECVPKTINFO) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#endif

#إذا (NGX_HAVE_IP_MTU_DISCOVER)

        إذا (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET) {
            بەها = IP_PMTUDISC_DO;

            إذا (setsockopt(ls[i].fd, IPPROTO_IP, IP_MTU_DISCOVER,
                           (ثابت فراغ *) &بەها, حجم(صحيح))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_MTU_DISCOVER) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#وإلاإذا (NGX_HAVE_IP_DONTFRAG)

        إذا (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET) {
            بەها = 1;

            إذا (setsockopt(ls[i].fd, IPPROTO_IP, IP_DONTFRAG,
                           (ثابت فراغ *) &بەها, حجم(صحيح))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_DONTFRAG) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#endif

#إذا (NGX_HAVE_INET6)

#إذا (NGX_HAVE_IPV6_MTU_DISCOVER)

        إذا (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET6) {
            بەها = IPV6_PMTUDISC_DO;

            إذا (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_MTU_DISCOVER,
                           (ثابت فراغ *) &بەها, حجم(صحيح))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IPV6_MTU_DISCOVER) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#وإلاإذا (NGX_HAVE_IPV6_DONTFRAG)

        إذا (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET6) {
            بەها = 1;

            إذا (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_DONTFRAG,
                           (ثابت فراغ *) &بەها, حجم(صحيح))
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

    أرجع;
}


فراغ
ngx_close_listening_sockets(ngx_cycle_t *cycle)
{
    ngx_uint_t         i;
    ngx_listening_t   *ls;
    ngx_connection_t  *c;

    ngx_accept_mutex_held = 0;
    ngx_use_accept_mutex = 0;

    ls = cycle->listening.elts;
    لكل (i = 0; i < cycle->listening.nelts; i++) {

#إذا (NGX_QUIC)
        إذا (ls[i].quic
            && (!ls[i].reuseport || !ngx_quic_bpf_enabled(cycle)))
        {
            استمر;
        }
#endif

        c = ls[i].connection;

        إذا (c) {
            إذا (c->اقرأ->active) {
                إذا (ngx_event_flags & NGX_USE_EPOLL_EVENT) {

                    /*
                     * it seems that Linux-2.6.x OpenVZ sends events
                     * for closed shared listening sockets unless
                     * the events was explicitly deleted
                     */

                    ngx_del_event(c->اقرأ, NGX_READ_EVENT, 0);

                } وإلا {
                    ngx_del_event(c->اقرأ, NGX_READ_EVENT, NGX_CLOSE_EVENT);
                }
            }

            إذا (c->اقرأ->timer_set) {
                ngx_del_timer(c->اقرأ);
            }

            ngx_free_connection(c);

            c->fd = (ngx_socket_t) -1;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, cycle->log, 0,
                       "close listening %V #%d ", &ls[i].addr_text, ls[i].fd);

        إذا (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_socket_errno,
                          ngx_close_socket_n " %V failed", &ls[i].addr_text);
        }

#إذا (NGX_HAVE_UNIX_DOMAIN)

        إذا (ls[i].sockaddr->sa_family == AF_UNIX
            && ngx_process <= NGX_PROCESS_MASTER
            && ngx_new_دووانەیی == 0
            && (!ls[i].inherited || ngx_getppid() != ngx_parent))
        {
            u_char *name = ls[i].addr_text.data + حجم("unix:") - 1;

            إذا (ngx_delete_file(name) == NGX_FILE_ERROR) {
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

    إذا (ngx_cycle->files && (ngx_uint_t) s >= ngx_cycle->files_n) {
        ngx_log_error(NGX_LOG_ALERT, log, 0,
                      "the new socket has number %d, "
                      "but only %ui files are available",
                      s, ngx_cycle->files_n);
        أرجع عدم;
    }

    ngx_drain_connections((ngx_cycle_t *) ngx_cycle);

    c = ngx_cycle->free_connections;

    إذا (c == عدم) {
        ngx_log_error(NGX_LOG_ALERT, log, 0,
                      "%ui worker_connections are not enough",
                      ngx_cycle->connection_n);

        أرجع عدم;
    }

    ngx_cycle->free_connections = c->data;
    ngx_cycle->free_connection_n--;

    إذا (ngx_cycle->files && ngx_cycle->files[s] == عدم) {
        ngx_cycle->files[s] = c;
    }

    rev = c->اقرأ;
    wev = c->اكتب;

    ngx_memzero(c, حجم(ngx_connection_t));

    c->اقرأ = rev;
    c->اكتب = wev;
    c->fd = s;
    c->log = log;

    instance = rev->instance;

    ngx_memzero(rev, حجم(ngx_event_t));
    ngx_memzero(wev, حجم(ngx_event_t));

    rev->instance = !instance;
    wev->instance = !instance;

    rev->index = NGX_INVALID_INDEX;
    wev->index = NGX_INVALID_INDEX;

    rev->data = c;
    wev->data = c;

    wev->اكتب = 1;

    أرجع c;
}


فراغ
ngx_free_connection(ngx_connection_t *c)
{
    c->data = ngx_cycle->free_connections;
    ngx_cycle->free_connections = c;
    ngx_cycle->free_connection_n++;

    إذا (ngx_cycle->files && ngx_cycle->files[c->fd] == c) {
        ngx_cycle->files[c->fd] = عدم;
    }
}


فراغ
ngx_close_connection(ngx_connection_t *c)
{
    ngx_err_t     err;
    ngx_uint_t    log_error, level;
    ngx_socket_t  fd;

    إذا (c->fd == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0, "connection already closed");
        أرجع;
    }

    إذا (c->اقرأ->timer_set) {
        ngx_del_timer(c->اقرأ);
    }

    إذا (c->اكتب->timer_set) {
        ngx_del_timer(c->اكتب);
    }

    إذا (!c->shared) {
        إذا (ngx_del_conn) {
            ngx_del_conn(c, NGX_CLOSE_EVENT);

        } وإلا {
            إذا (c->اقرأ->active || c->اقرأ->disabled) {
                ngx_del_event(c->اقرأ, NGX_READ_EVENT, NGX_CLOSE_EVENT);
            }

            إذا (c->اكتب->active || c->اكتب->disabled) {
                ngx_del_event(c->اكتب, NGX_WRITE_EVENT, NGX_CLOSE_EVENT);
            }
        }
    }

    إذا (c->اقرأ->posted) {
        ngx_delete_posted_event(c->اقرأ);
    }

    إذا (c->اكتب->posted) {
        ngx_delete_posted_event(c->اكتب);
    }

    c->اقرأ->closed = 1;
    c->اكتب->closed = 1;

    ngx_reusable_connection(c, 0);

    log_error = c->log_error;

    ngx_free_connection(c);

    fd = c->fd;
    c->fd = (ngx_socket_t) -1;

    إذا (c->shared) {
        أرجع;
    }

    إذا (ngx_close_socket(fd) == -1) {

        err = ngx_socket_errno;

        إذا (err == NGX_ECONNRESET || err == NGX_ENOTCONN) {

            بدل (log_error) {

            حالة NGX_ERROR_INFO:
                level = NGX_LOG_INFO;
                اكسر;

            حالة NGX_ERROR_ERR:
                level = NGX_LOG_ERR;
                اكسر;

            افتراضي:
                level = NGX_LOG_CRIT;
            }

        } وإلا {
            level = NGX_LOG_CRIT;
        }

        ngx_log_error(level, c->log, err, ngx_close_socket_n " %d failed", fd);
    }
}


فراغ
ngx_reusable_connection(ngx_connection_t *c, ngx_uint_t reusable)
{
    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "reusable connection: %ui", reusable);

    إذا (c->reusable) {
        ngx_queue_remove(&c->queue);
        ngx_cycle->reusable_connections_n--;

#إذا (NGX_STAT_STUB)
        (فراغ) ngx_atomic_fetch_add(ngx_stat_waiting, -1);
#endif
    }

    c->reusable = reusable;

    إذا (reusable) {
        /* need cast as ngx_cycle is volatile */

        ngx_queue_زیادکردن_head(
            (ngx_queue_t *) &ngx_cycle->reusable_connections_queue, &c->queue);
        ngx_cycle->reusable_connections_n++;

#إذا (NGX_STAT_STUB)
        (فراغ) ngx_atomic_fetch_add(ngx_stat_waiting, 1);
#endif
    }
}


ساكن فراغ
ngx_drain_connections(ngx_cycle_t *cycle)
{
    ngx_uint_t         i, n;
    ngx_queue_t       *q;
    ngx_connection_t  *c;

    إذا (cycle->free_connection_n > cycle->connection_n / 16
        || cycle->reusable_connections_n == 0)
    {
        أرجع;
    }

    إذا (cycle->connections_reuse_time != ngx_time()) {
        cycle->connections_reuse_time = ngx_time();

        ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                      "%ui worker_connections are not enough, "
                      "reusing connections",
                      cycle->connection_n);
    }

    c = عدم;
    n = ngx_max(ngx_min(32, cycle->reusable_connections_n / 8), 1);

    لكل (i = 0; i < n; i++) {
        إذا (ngx_queue_empty(&cycle->reusable_connections_queue)) {
            اكسر;
        }

        q = ngx_queue_last(&cycle->reusable_connections_queue);
        c = ngx_queue_data(q, ngx_connection_t, queue);

        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "reusing connection");

        c->close = 1;
        c->اقرأ->handler(c->اقرأ);
    }

    إذا (cycle->free_connection_n == 0 && c && c->reusable) {

        /*
         * if no connections were freed, try to reuse the last
         * connection again: this should free it as long as
         * previous reuse moved it to lingering close
         */

        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "reusing connection again");

        c->close = 1;
        c->اقرأ->handler(c->اقرأ);
    }
}


فراغ
ngx_close_idle_connections(ngx_cycle_t *cycle)
{
    ngx_uint_t         i;
    ngx_connection_t  *c;

    c = cycle->connections;

    لكل (i = 0; i < cycle->connection_n; i++) {

        /* THREAD: lock */

        إذا (c[i].fd != (ngx_socket_t) -1 && c[i].idle) {
            c[i].close = 1;
            c[i].اقرأ->handler(c[i].اقرأ);
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
    بنية sockaddr_in   *sin;
#إذا (NGX_HAVE_INET6)
    ngx_uint_t            i;
    بنية sockaddr_in6  *sin6;
#endif

    addr = 0;

    إذا (c->local_socklen) {
        بدل (c->local_sockaddr->sa_family) {

#إذا (NGX_HAVE_INET6)
        حالة AF_INET6:
            sin6 = (بنية sockaddr_in6 *) c->local_sockaddr;

            لكل (i = 0; addr == 0 && i < 16; i++) {
                addr |= sin6->sin6_addr.s6_addr[i];
            }

            اكسر;
#endif

#إذا (NGX_HAVE_UNIX_DOMAIN)
        حالة AF_UNIX:
            addr = 1;
            اكسر;
#endif

        افتراضي: /* AF_INET */
            sin = (بنية sockaddr_in *) c->local_sockaddr;
            addr = sin->sin_addr.s_addr;
            اكسر;
        }
    }

    إذا (addr == 0) {

        len = حجم(ngx_sockaddr_t);

        إذا (getsockname(c->fd, &sa.sockaddr, &len) == -1) {
            ngx_connection_error(c, ngx_socket_errno, "getsockname() failed");
            أرجع NGX_ERROR;
        }

        c->local_sockaddr = ngx_palloc(c->pool, len);
        إذا (c->local_sockaddr == عدم) {
            أرجع NGX_ERROR;
        }

        ngx_memcpy(c->local_sockaddr, &sa, len);

        c->local_socklen = len;
    }

    إذا (s == عدم) {
        أرجع NGX_OK;
    }

    s->len = ngx_sock_ntop(c->local_sockaddr, c->local_socklen,
                           s->data, s->len, port);

    أرجع NGX_OK;
}


ngx_int_t
ngx_tcp_nodelay(ngx_connection_t *c)
{
    صحيح  tcp_nodelay;

    إذا (c->tcp_nodelay != NGX_TCP_NODELAY_UNSET) {
        أرجع NGX_OK;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0, "tcp_nodelay");

    tcp_nodelay = 1;

    إذا (setsockopt(c->fd, IPPROTO_TCP, TCP_NODELAY,
                   (ثابت فراغ *) &tcp_nodelay, حجم(صحيح))
        == -1)
    {
#إذا (NGX_SOLARIS)
        إذا (c->log_error == NGX_ERROR_INFO) {

            /* Solaris returns EINVAL if a socket has been shut down */
            c->log_error = NGX_ERROR_IGNORE_EINVAL;

            ngx_connection_error(c, ngx_socket_errno,
                                 "setsockopt(TCP_NODELAY) failed");

            c->log_error = NGX_ERROR_INFO;

            أرجع NGX_ERROR;
        }
#endif

        ngx_connection_error(c, ngx_socket_errno,
                             "setsockopt(TCP_NODELAY) failed");
        أرجع NGX_ERROR;
    }

    c->tcp_nodelay = NGX_TCP_NODELAY_SET;

    أرجع NGX_OK;
}


ngx_int_t
ngx_connection_error(ngx_connection_t *c, ngx_err_t err, حرف *text)
{
    ngx_uint_t  level;

    /* Winsock may return NGX_ECONNABORTED instead of NGX_ECONNRESET */

    إذا ((err == NGX_ECONNRESET
#إذا (NGX_WIN32)
         || err == NGX_ECONNABORTED
#endif
        ) && c->log_error == NGX_ERROR_IGNORE_ECONNRESET)
    {
        أرجع 0;
    }

#إذا (NGX_SOLARIS)
    إذا (err == NGX_EINVAL && c->log_error == NGX_ERROR_IGNORE_EINVAL) {
        أرجع 0;
    }
#endif

    إذا (err == NGX_EMSGSIZE && c->log_error == NGX_ERROR_IGNORE_EMSGSIZE) {
        أرجع 0;
    }

#إذا (NGX_LINUX)
    إذا (err == NGX_EIO && c->ktls_recv) {
        أرجع 0;
    }
#endif

    إذا (err == 0
        || err == NGX_ECONNRESET
#إذا (NGX_WIN32)
        || err == NGX_ECONNABORTED
#وإلا
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
        بدل (c->log_error) {

        حالة NGX_ERROR_IGNORE_EMSGSIZE:
        حالة NGX_ERROR_IGNORE_EINVAL:
        حالة NGX_ERROR_IGNORE_ECONNRESET:
        حالة NGX_ERROR_INFO:
            level = NGX_LOG_INFO;
            اكسر;

        افتراضي:
            level = NGX_LOG_ERR;
        }

    } وإلا {
        level = NGX_LOG_ALERT;
    }

    ngx_log_error(level, c->log, err, text);

    أرجع NGX_ERROR;
}
