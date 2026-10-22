
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>
#અન્તર્ભાવ <ngx_event.h>


ngx_os_io_t  ngx_io;


સ્થિર શૂન્ય ngx_drain_connections(ngx_cycle_t *cycle);


ngx_listening_t *
ngx_create_listening(ngx_conf_t *cf, સંરચના sockaddr *sockaddr,
    socklen_t socklen)
{
    size_t            len;
    ngx_listening_t  *ls;
    સંરચના sockaddr  *sa;
    u_char            text[NGX_SOCKADDR_STRLEN];

    ls = ngx_array_ધકેલો(&cf->cycle->listening);
    જો (ls == ખાલી) {
        પાછા ખાલી;
    }

    ngx_memzero(ls, કદ(ngx_listening_t));

    sa = ngx_palloc(cf->pool, socklen);
    જો (sa == ખાલી) {
        પાછા ખાલી;
    }

    ngx_memcpy(sa, sockaddr, socklen);

    ls->sockaddr = sa;
    ls->socklen = socklen;

    len = ngx_sock_ntop(sa, socklen, text, NGX_SOCKADDR_STRLEN, 1);
    ls->addr_text.len = len;

    પસંદગી (ls->sockaddr->sa_family) {
#જો (NGX_HAVE_INET6)
    સ્થિતિ AF_INET6:
        ls->addr_text_max_len = NGX_INET6_ADDRSTRLEN;
        તોડો;
#endif
#જો (NGX_HAVE_UNIX_DOMAIN)
    સ્થિતિ AF_UNIX:
        ls->addr_text_max_len = NGX_UNIX_ADDRSTRLEN;
        len++;
        તોડો;
#endif
    સ્થિતિ AF_INET:
        ls->addr_text_max_len = NGX_INET_ADDRSTRLEN;
        તોડો;
    મૂળભૂત:
        ls->addr_text_max_len = NGX_SOCKADDR_STRLEN;
        તોડો;
    }

    ls->addr_text.data = ngx_pnalloc(cf->pool, len);
    જો (ls->addr_text.data == ખાલી) {
        પાછા ખાલી;
    }

    ngx_memcpy(ls->addr_text.data, text, len);

#જો !(NGX_WIN32)
    ngx_rbtree_init(&ls->rbtree, &ls->sentinel, ngx_udp_rbtree_ઉમેરો_કિંમત);
#endif

    ls->fd = (ngx_socket_t) -1;
    ls->type = SOCK_STREAM;

    ls->backlog = NGX_LISTEN_BACKLOG;
    ls->rcvbuf = -1;
    ls->sndbuf = -1;

#જો (NGX_HAVE_SETFIB)
    ls->setfib = -1;
#endif

#જો (NGX_HAVE_TCP_FASTOPEN)
    ls->fastopen = -1;
#endif

    પાછા ls;
}


ngx_int_t
ngx_clone_listening(ngx_cycle_t *cycle, ngx_listening_t *ls)
{
#જો (NGX_HAVE_REUSEPORT)

    ngx_int_t         n;
    ngx_core_conf_t  *ccf;
    ngx_listening_t   ols;

    જો (!ls->reuseport || ls->worker != 0) {
        પાછા NGX_OK;
    }

    ols = *ls;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    માટે (n = 1; n < ccf->worker_processes; n++) {

        /* create a socket for each worker process */

        ls = ngx_array_ધકેલો(&cycle->listening);
        જો (ls == ખાલી) {
            પાછા NGX_ERROR;
        }

        *ls = ols;
        ls->worker = n;
    }

#endif

    પાછા NGX_OK;
}


ngx_int_t
ngx_set_inherited_sockets(ngx_cycle_t *cycle)
{
    size_t                     len;
    ngx_uint_t                 i;
    ngx_listening_t           *ls;
    socklen_t                  olen;
#જો (NGX_HAVE_DEFERRED_ACCEPT || NGX_HAVE_TCP_FASTOPEN)
    ngx_err_t                  err;
#endif
#જો (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    સંરચના accept_filter_arg   af;
#endif
#જો (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)
    પૂર્ણાંક                        timeout;
#endif
#જો (NGX_HAVE_REUSEPORT)
    પૂર્ણાંક                        reuseport;
#endif

    ls = cycle->listening.elts;
    માટે (i = 0; i < cycle->listening.nelts; i++) {

        ls[i].sockaddr = ngx_palloc(cycle->pool, કદ(ngx_sockaddr_t));
        જો (ls[i].sockaddr == ખાલી) {
            પાછા NGX_ERROR;
        }

        ls[i].socklen = કદ(ngx_sockaddr_t);
        જો (getsockname(ls[i].fd, ls[i].sockaddr, &ls[i].socklen) == -1) {
            ngx_log_error(NGX_LOG_CRIT, cycle->log, ngx_socket_errno,
                          "getsockname() of the inherited "
                          "socket #%d failed", ls[i].fd);
            ls[i].ignore = 1;
            ચાલુ;
        }

        જો (ls[i].socklen > (socklen_t) કદ(ngx_sockaddr_t)) {
            ls[i].socklen = કદ(ngx_sockaddr_t);
        }

        પસંદગી (ls[i].sockaddr->sa_family) {

#જો (NGX_HAVE_INET6)
        સ્થિતિ AF_INET6:
            ls[i].addr_text_max_len = NGX_INET6_ADDRSTRLEN;
            len = NGX_INET6_ADDRSTRLEN + કદ("[]:65535") - 1;
            તોડો;
#endif

#જો (NGX_HAVE_UNIX_DOMAIN)
        સ્થિતિ AF_UNIX:
            ls[i].addr_text_max_len = NGX_UNIX_ADDRSTRLEN;
            len = NGX_UNIX_ADDRSTRLEN;
            તોડો;
#endif

        સ્થિતિ AF_INET:
            ls[i].addr_text_max_len = NGX_INET_ADDRSTRLEN;
            len = NGX_INET_ADDRSTRLEN + કદ(":65535") - 1;
            તોડો;

        મૂળભૂત:
            ngx_log_error(NGX_LOG_CRIT, cycle->log, ngx_socket_errno,
                          "the inherited socket #%d has "
                          "an unsupported protocol family", ls[i].fd);
            ls[i].ignore = 1;
            ચાલુ;
        }

        ls[i].addr_text.data = ngx_pnalloc(cycle->pool, len);
        જો (ls[i].addr_text.data == ખાલી) {
            પાછા NGX_ERROR;
        }

        len = ngx_sock_ntop(ls[i].sockaddr, ls[i].socklen,
                            ls[i].addr_text.data, len, 1);
        જો (len == 0) {
            પાછા NGX_ERROR;
        }

        ls[i].addr_text.len = len;

        ls[i].backlog = NGX_LISTEN_BACKLOG;

        olen = કદ(પૂર્ણાંક);

        જો (getsockopt(ls[i].fd, SOL_SOCKET, SO_TYPE, (શૂન્ય *) &ls[i].type,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_CRIT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_TYPE) %V failed", &ls[i].addr_text);
            ls[i].ignore = 1;
            ચાલુ;
        }

        olen = કદ(પૂર્ણાંક);

        જો (getsockopt(ls[i].fd, SOL_SOCKET, SO_RCVBUF, (શૂન્ય *) &ls[i].rcvbuf,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_RCVBUF) %V failed, ignored",
                          &ls[i].addr_text);

            ls[i].rcvbuf = -1;
        }

        olen = કદ(પૂર્ણાંક);

        જો (getsockopt(ls[i].fd, SOL_SOCKET, SO_SNDBUF, (શૂન્ય *) &ls[i].sndbuf,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_SNDBUF) %V failed, ignored",
                          &ls[i].addr_text);

            ls[i].sndbuf = -1;
        }

#જો 0
        /* SO_SETFIB is currently a set only option */

#જો (NGX_HAVE_SETFIB)

        olen = કદ(પૂર્ણાંક);

        જો (getsockopt(ls[i].fd, SOL_SOCKET, SO_SETFIB,
                       (શૂન્ય *) &ls[i].setfib, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_SETFIB) %V failed, ignored",
                          &ls[i].addr_text);

            ls[i].setfib = -1;
        }

#endif
#endif

#જો (NGX_HAVE_REUSEPORT)

        reuseport = 0;
        olen = કદ(પૂર્ણાંક);

#ifdef SO_REUSEPORT_LB

        જો (getsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT_LB,
                       (શૂન્ય *) &reuseport, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_REUSEPORT_LB) %V failed, ignored",
                          &ls[i].addr_text);

        } નહીંતર {
            ls[i].reuseport = reuseport ? 1 : 0;
        }

#નહીંતર

        જો (getsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT,
                       (શૂન્ય *) &reuseport, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_REUSEPORT) %V failed, ignored",
                          &ls[i].addr_text);

        } નહીંતર {
            ls[i].reuseport = reuseport ? 1 : 0;
        }
#endif

#endif

        જો (ls[i].type != SOCK_STREAM) {
            ચાલુ;
        }

#ifdef SO_PROTOCOL

        olen = કદ(પૂર્ણાંક);

        જો (getsockopt(ls[i].fd, SOL_SOCKET, SO_PROTOCOL,
                       (શૂન્ય *) &ls[i].protocol, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_PROTOCOL) %V failed, ignored",
                          &ls[i].addr_text);
            ls[i].protocol = 0;

        } નહીંતર જો (ls[i].protocol == IPPROTO_TCP) {
            ls[i].protocol = 0;
        }

#endif

#જો (NGX_HAVE_TCP_FASTOPEN)

        olen = કદ(પૂર્ણાંક);

        જો (getsockopt(ls[i].fd, IPPROTO_TCP, TCP_FASTOPEN,
                       (શૂન્ય *) &ls[i].fastopen, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            જો (err != NGX_EOPNOTSUPP && err != NGX_ENOPROTOOPT
                && err != NGX_EINVAL)
            {
                ngx_log_error(NGX_LOG_NOTICE, cycle->log, err,
                              "getsockopt(TCP_FASTOPEN) %V failed, ignored",
                              &ls[i].addr_text);
            }

            ls[i].fastopen = -1;
        }

#endif

#જો (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)

        ngx_memzero(&af, કદ(સંરચના accept_filter_arg));
        olen = કદ(સંરચના accept_filter_arg);

        જો (getsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER, &af, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            જો (err == NGX_EINVAL) {
                ચાલુ;
            }

            ngx_log_error(NGX_LOG_NOTICE, cycle->log, err,
                          "getsockopt(SO_ACCEPTFILTER) for %V failed, ignored",
                          &ls[i].addr_text);
            ચાલુ;
        }

        જો (olen < કદ(સંરચના accept_filter_arg) || af.af_name[0] == '\0') {
            ચાલુ;
        }

        ls[i].accept_filter = ngx_palloc(cycle->pool, 16);
        જો (ls[i].accept_filter == ખાલી) {
            પાછા NGX_ERROR;
        }

        (શૂન્ય) ngx_cpystrn((u_char *) ls[i].accept_filter,
                           (u_char *) af.af_name, 16);
#endif

#જો (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)

        timeout = 0;
        olen = કદ(પૂર્ણાંક);

        જો (getsockopt(ls[i].fd, IPPROTO_TCP, TCP_DEFER_ACCEPT, &timeout, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            જો (err == NGX_EOPNOTSUPP) {
                ચાલુ;
            }

            ngx_log_error(NGX_LOG_NOTICE, cycle->log, err,
                          "getsockopt(TCP_DEFER_ACCEPT) for %V failed, ignored",
                          &ls[i].addr_text);
            ચાલુ;
        }

        જો (olen < કદ(પૂર્ણાંક) || timeout == 0) {
            ચાલુ;
        }

        ls[i].deferred_accept = 1;
#endif
    }

    પાછા NGX_OK;
}


ngx_int_t
ngx_open_listening_sockets(ngx_cycle_t *cycle)
{
    પૂર્ણાંક               reuseaddr;
    ngx_uint_t        i, tries, failed;
    ngx_err_t         err;
    ngx_log_t        *log;
    ngx_socket_t      s;
    ngx_listening_t  *ls;

    reuseaddr = 1;
#જો (NGX_SUPPRESS_WARN)
    failed = 0;
#endif

    log = cycle->log;

    /* TODO: configurable try number */

    માટે (tries = 5; tries; tries--) {
        failed = 0;

        /* for each listening socket */

        ls = cycle->listening.elts;
        માટે (i = 0; i < cycle->listening.nelts; i++) {

            જો (ls[i].ignore) {
                ચાલુ;
            }

#જો (NGX_HAVE_REUSEPORT)

            જો (ls[i].add_reuseport || ls[i].change_protocol) {

                /*
                 * to allow transition from a socket without SO_REUSEPORT
                 * to multiple sockets with SO_REUSEPORT, we have to set
                 * SO_REUSEPORT on the old socket before opening new ones
                 *
                 * to allow transition between different socket protocols
                 * (e.g. IPPROTO_MPTCP), SO_REUSEPORT is set on both old
                 * and new sockets
                 */

                પૂર્ણાંક  reuseport = 1;

#ifdef SO_REUSEPORT_LB

                જો (setsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT_LB,
                               (અચલ શૂન્ય *) &reuseport, કદ(પૂર્ણાંક))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT_LB) %V failed, "
                                  "ignored",
                                  &ls[i].addr_text);
                }

#નહીંતર

                જો (setsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT,
                               (અચલ શૂન્ય *) &reuseport, કદ(પૂર્ણાંક))
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

            જો (ls[i].fd != (ngx_socket_t) -1 && !ls[i].change_protocol) {
                ચાલુ;
            }

            જો (ls[i].inherited) {

                /* TODO: close on exit */
                /* TODO: nonblocking */
                /* TODO: deferred accept */

                ચાલુ;
            }

            s = ngx_socket(ls[i].sockaddr->sa_family, ls[i].type,
                           ls[i].protocol);

            જો (s == (ngx_socket_t) -1) {
                ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                              ngx_socket_n " %V failed", &ls[i].addr_text);
                પાછા NGX_ERROR;
            }

            જો (ls[i].type != SOCK_DGRAM || !ngx_test_config) {

                જો (setsockopt(s, SOL_SOCKET, SO_REUSEADDR,
                               (અચલ શૂન્ય *) &reuseaddr, કદ(પૂર્ણાંક))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEADDR) %V failed",
                                  &ls[i].addr_text);

                    જો (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_text);
                    }

                    પાછા NGX_ERROR;
                }
            }

#જો (NGX_HAVE_REUSEPORT)

            જો ((ls[i].reuseport || ls[i].change_protocol)
                && !ngx_test_config)
            {
                પૂર્ણાંક  reuseport;

                reuseport = 1;

#ifdef SO_REUSEPORT_LB

                જો (setsockopt(s, SOL_SOCKET, SO_REUSEPORT_LB,
                               (અચલ શૂન્ય *) &reuseport, કદ(પૂર્ણાંક))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT_LB) %V failed",
                                  &ls[i].addr_text);

                    જો (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_text);
                    }

                    પાછા NGX_ERROR;
                }

#નહીંતર

                જો (setsockopt(s, SOL_SOCKET, SO_REUSEPORT,
                               (અચલ શૂન્ય *) &reuseport, કદ(પૂર્ણાંક))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT) %V failed",
                                  &ls[i].addr_text);

                    જો (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_text);
                    }

                    પાછા NGX_ERROR;
                }
#endif
            }
#endif

#જો (NGX_HAVE_INET6 && defined IPV6_V6ONLY)

            જો (ls[i].sockaddr->sa_family == AF_INET6) {
                પૂર્ણાંક  ipv6only;

                ipv6only = ls[i].ipv6only;

                જો (setsockopt(s, IPPROTO_IPV6, IPV6_V6ONLY,
                               (અચલ શૂન્ય *) &ipv6only, કદ(પૂર્ણાંક))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(IPV6_V6ONLY) %V failed, ignored",
                                  &ls[i].addr_text);
                }
            }
#endif
            /* TODO: close on exit */

            જો (ngx_nonblocking(s) == -1) {
                ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                              ngx_nonblocking_n " %V failed",
                              &ls[i].addr_text);

                જો (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_text);
                }

                પાછા NGX_ERROR;
            }

            ngx_log_debug2(NGX_LOG_DEBUG_CORE, log, 0,
                           "bind() %V #%d ", &ls[i].addr_text, s);

            જો (bind(s, ls[i].sockaddr, ls[i].socklen) == -1) {
                err = ngx_socket_errno;

                જો (err != NGX_EADDRINUSE || !ngx_test_config) {
                    ngx_log_error(NGX_LOG_EMERG, log, err,
                                  "bind() to %V failed", &ls[i].addr_text);
                }

                જો (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_text);
                }

                જો (err != NGX_EADDRINUSE) {
                    પાછા NGX_ERROR;
                }

                જો (!ngx_test_config) {
                    failed = 1;
                }

                ચાલુ;
            }

#જો (NGX_HAVE_UNIX_DOMAIN)

            જો (ls[i].sockaddr->sa_family == AF_UNIX) {
                mode_t   mode;
                u_char  *name;

                name = ls[i].addr_text.data + કદ("unix:") - 1;
                mode = (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);

                જો (chmod((અક્ષર *) name, mode) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  "chmod() \"%s\" failed", name);
                }

                જો (ngx_test_config) {
                    જો (ngx_delete_file(name) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                      ngx_delete_file_n " %s failed", name);
                    }
                }
            }
#endif

            જો (ls[i].type != SOCK_STREAM) {
                ls[i].fd = s;
                ls[i].open = 1;
                ચાલુ;
            }

            જો (listen(s, ls[i].backlog) == -1) {
                err = ngx_socket_errno;

                /*
                 * on OpenVZ after suspend/resume EADDRINUSE
                 * may be returned by listen() instead of bind(), see
                 * https://bugs.openvz.org/browse/OVZ-5587
                 */

                જો (err != NGX_EADDRINUSE || !ngx_test_config) {
                    ngx_log_error(NGX_LOG_EMERG, log, err,
                                  "listen() to %V, backlog %d failed",
                                  &ls[i].addr_text, ls[i].backlog);
                }

                જો (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_text);
                }

                જો (err != NGX_EADDRINUSE) {
                    પાછા NGX_ERROR;
                }

                જો (!ngx_test_config) {
                    failed = 1;
                }

                ચાલુ;
            }

            ls[i].listen = 1;

            ls[i].fd = s;
            ls[i].open = 1;
        }

        જો (!failed) {
            તોડો;
        }

        /* TODO: delay configurable */

        ngx_log_error(NGX_LOG_NOTICE, log, 0,
                      "try again to bind() after 500ms");

        ngx_msleep(500);
    }

    જો (failed) {
        ngx_log_error(NGX_LOG_EMERG, log, 0, "still could not bind()");
        પાછા NGX_ERROR;
    }

    પાછા NGX_OK;
}


શૂન્ય
ngx_configure_listening_sockets(ngx_cycle_t *cycle)
{
    પૂર્ણાંક                        કિંમત;
    ngx_uint_t                 i;
    ngx_listening_t           *ls;

#જો (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    સંરચના accept_filter_arg   af;
#endif

    ls = cycle->listening.elts;
    માટે (i = 0; i < cycle->listening.nelts; i++) {

        ls[i].log = *ls[i].logp;

        જો (ls[i].rcvbuf != -1) {
            જો (setsockopt(ls[i].fd, SOL_SOCKET, SO_RCVBUF,
                           (અચલ શૂન્ય *) &ls[i].rcvbuf, કદ(પૂર્ણાંક))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_RCVBUF, %d) %V failed, ignored",
                              ls[i].rcvbuf, &ls[i].addr_text);
            }
        }

        જો (ls[i].sndbuf != -1) {
            જો (setsockopt(ls[i].fd, SOL_SOCKET, SO_SNDBUF,
                           (અચલ શૂન્ય *) &ls[i].sndbuf, કદ(પૂર્ણાંક))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_SNDBUF, %d) %V failed, ignored",
                              ls[i].sndbuf, &ls[i].addr_text);
            }
        }

        જો (ls[i].keepalive) {
            કિંમત = (ls[i].keepalive == 1) ? 1 : 0;

            જો (setsockopt(ls[i].fd, SOL_SOCKET, SO_KEEPALIVE,
                           (અચલ શૂન્ય *) &કિંમત, કદ(પૂર્ણાંક))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_KEEPALIVE, %d) %V failed, ignored",
                              કિંમત, &ls[i].addr_text);
            }
        }

#જો (NGX_HAVE_KEEPALIVE_TUNABLE)

#જો !(NGX_DARWIN)

        જો (ls[i].keepidle) {
            કિંમત = ls[i].keepidle;

#જો (NGX_KEEPALIVE_FACTOR)
            કિંમત *= NGX_KEEPALIVE_FACTOR;
#endif

            જો (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPIDLE,
                           (અચલ શૂન્ય *) &કિંમત, કદ(પૂર્ણાંક))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_KEEPIDLE, %d) %V failed, ignored",
                              કિંમત, &ls[i].addr_text);
            }
        }

#endif

        જો (ls[i].keepintvl) {
            કિંમત = ls[i].keepintvl;

#જો (NGX_KEEPALIVE_FACTOR)
            કિંમત *= NGX_KEEPALIVE_FACTOR;
#endif

            જો (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPINTVL,
                           (અચલ શૂન્ય *) &કિંમત, કદ(પૂર્ણાંક))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                             "setsockopt(TCP_KEEPINTVL, %d) %V failed, ignored",
                             કિંમત, &ls[i].addr_text);
            }
        }

        જો (ls[i].keepcnt) {
            જો (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPCNT,
                           (અચલ શૂન્ય *) &ls[i].keepcnt, કદ(પૂર્ણાંક))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_KEEPCNT, %d) %V failed, ignored",
                              ls[i].keepcnt, &ls[i].addr_text);
            }
        }

#endif

#જો (NGX_HAVE_SETFIB)
        જો (ls[i].setfib != -1) {
            જો (setsockopt(ls[i].fd, SOL_SOCKET, SO_SETFIB,
                           (અચલ શૂન્ય *) &ls[i].setfib, કદ(પૂર્ણાંક))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_SETFIB, %d) %V failed, ignored",
                              ls[i].setfib, &ls[i].addr_text);
            }
        }
#endif

#જો (NGX_HAVE_TCP_FASTOPEN)
        જો (ls[i].fastopen != -1) {
            જો (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_FASTOPEN,
                           (અચલ શૂન્ય *) &ls[i].fastopen, કદ(પૂર્ણાંક))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_FASTOPEN, %d) %V failed, ignored",
                              ls[i].fastopen, &ls[i].addr_text);
            }
        }
#endif

#જો 0
        જો (1) {
            પૂર્ણાંક tcp_nodelay = 1;

            જો (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_NODELAY,
                       (અચલ શૂન્ય *) &tcp_nodelay, કદ(પૂર્ણાંક))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_NODELAY) %V failed, ignored",
                              &ls[i].addr_text);
            }
        }
#endif

        જો (ls[i].listen) {

            /* change backlog via listen() */

            જો (listen(ls[i].fd, ls[i].backlog) == -1) {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "listen() to %V, backlog %d failed, ignored",
                              &ls[i].addr_text, ls[i].backlog);
            }
        }

        /*
         * setting deferred mode should be last operation on socket,
         * because code may prematurely continue cycle on failure
         */

#જો (NGX_HAVE_DEFERRED_ACCEPT)

#ifdef SO_ACCEPTFILTER

        જો (ls[i].delete_deferred) {
            જો (setsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER, ખાલી, 0)
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_ACCEPTFILTER, NULL) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);

                જો (ls[i].accept_filter) {
                    ngx_log_error(NGX_LOG_ALERT, cycle->log, 0,
                                  "could not change the accept filter "
                                  "to \"%s\" for %V, ignored",
                                  ls[i].accept_filter, &ls[i].addr_text);
                }

                ચાલુ;
            }

            ls[i].deferred_accept = 0;
        }

        જો (ls[i].add_deferred) {
            ngx_memzero(&af, કદ(સંરચના accept_filter_arg));
            (શૂન્ય) ngx_cpystrn((u_char *) af.af_name,
                               (u_char *) ls[i].accept_filter, 16);

            જો (setsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER,
                           &af, કદ(સંરચના accept_filter_arg))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_ACCEPTFILTER, \"%s\") "
                              "for %V failed, ignored",
                              ls[i].accept_filter, &ls[i].addr_text);
                ચાલુ;
            }

            ls[i].deferred_accept = 1;
        }

#endif

#ifdef TCP_DEFER_ACCEPT

        જો (ls[i].add_deferred || ls[i].delete_deferred) {

            જો (ls[i].add_deferred) {
                /*
                 * There is no way to find out how long a connection was
                 * in queue (and a connection may bypass deferred queue at all
                 * if syncookies were used), hence we use 1 second timeout
                 * here.
                 */
                કિંમત = 1;

            } નહીંતર {
                કિંમત = 0;
            }

            જો (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_DEFER_ACCEPT,
                           &કિંમત, કદ(પૂર્ણાંક))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_DEFER_ACCEPT, %d) for %V failed, "
                              "ignored",
                              કિંમત, &ls[i].addr_text);

                ચાલુ;
            }
        }

        જો (ls[i].add_deferred) {
            ls[i].deferred_accept = 1;
        }

#endif

#endif /* NGX_HAVE_DEFERRED_ACCEPT */

#જો (NGX_HAVE_IP_RECVDSTADDR)

        જો (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET)
        {
            કિંમત = 1;

            જો (setsockopt(ls[i].fd, IPPROTO_IP, IP_RECVDSTADDR,
                           (અચલ શૂન્ય *) &કિંમત, કદ(પૂર્ણાંક))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_RECVDSTADDR) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#અથવાજો (NGX_HAVE_IP_PKTINFO)

        જો (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET)
        {
            કિંમત = 1;

            જો (setsockopt(ls[i].fd, IPPROTO_IP, IP_PKTINFO,
                           (અચલ શૂન્ય *) &કિંમત, કદ(પૂર્ણાંક))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_PKTINFO) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#endif

#જો (NGX_HAVE_INET6 && NGX_HAVE_IPV6_RECVPKTINFO)

        જો (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET6)
        {
            કિંમત = 1;

            જો (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_RECVPKTINFO,
                           (અચલ શૂન્ય *) &કિંમત, કદ(પૂર્ણાંક))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IPV6_RECVPKTINFO) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#endif

#જો (NGX_HAVE_IP_MTU_DISCOVER)

        જો (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET) {
            કિંમત = IP_PMTUDISC_DO;

            જો (setsockopt(ls[i].fd, IPPROTO_IP, IP_MTU_DISCOVER,
                           (અચલ શૂન્ય *) &કિંમત, કદ(પૂર્ણાંક))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_MTU_DISCOVER) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#અથવાજો (NGX_HAVE_IP_DONTFRAG)

        જો (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET) {
            કિંમત = 1;

            જો (setsockopt(ls[i].fd, IPPROTO_IP, IP_DONTFRAG,
                           (અચલ શૂન્ય *) &કિંમત, કદ(પૂર્ણાંક))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_DONTFRAG) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#endif

#જો (NGX_HAVE_INET6)

#જો (NGX_HAVE_IPV6_MTU_DISCOVER)

        જો (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET6) {
            કિંમત = IPV6_PMTUDISC_DO;

            જો (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_MTU_DISCOVER,
                           (અચલ શૂન્ય *) &કિંમત, કદ(પૂર્ણાંક))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IPV6_MTU_DISCOVER) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#અથવાજો (NGX_HAVE_IPV6_DONTFRAG)

        જો (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET6) {
            કિંમત = 1;

            જો (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_DONTFRAG,
                           (અચલ શૂન્ય *) &કિંમત, કદ(પૂર્ણાંક))
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

    પાછા;
}


શૂન્ય
ngx_close_listening_sockets(ngx_cycle_t *cycle)
{
    ngx_uint_t         i;
    ngx_listening_t   *ls;
    ngx_connection_t  *c;

    ngx_accept_mutex_held = 0;
    ngx_use_accept_mutex = 0;

    ls = cycle->listening.elts;
    માટે (i = 0; i < cycle->listening.nelts; i++) {

#જો (NGX_QUIC)
        જો (ls[i].quic
            && (!ls[i].reuseport || !ngx_quic_bpf_enabled(cycle)))
        {
            ચાલુ;
        }
#endif

        c = ls[i].connection;

        જો (c) {
            જો (c->પઠ->active) {
                જો (ngx_event_flags & NGX_USE_EPOLL_EVENT) {

                    /*
                     * it seems that Linux-2.6.x OpenVZ sends events
                     * for closed shared listening sockets unless
                     * the events was explicitly deleted
                     */

                    ngx_del_event(c->પઠ, NGX_READ_EVENT, 0);

                } નહીંતર {
                    ngx_del_event(c->પઠ, NGX_READ_EVENT, NGX_CLOSE_EVENT);
                }
            }

            જો (c->પઠ->timer_set) {
                ngx_del_timer(c->પઠ);
            }

            ngx_free_connection(c);

            c->fd = (ngx_socket_t) -1;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, cycle->log, 0,
                       "close listening %V #%d ", &ls[i].addr_text, ls[i].fd);

        જો (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_socket_errno,
                          ngx_close_socket_n " %V failed", &ls[i].addr_text);
        }

#જો (NGX_HAVE_UNIX_DOMAIN)

        જો (ls[i].sockaddr->sa_family == AF_UNIX
            && ngx_process <= NGX_PROCESS_MASTER
            && ngx_new_દ્વિઅંકી == 0
            && (!ls[i].inherited || ngx_getppid() != ngx_parent))
        {
            u_char *name = ls[i].addr_text.data + કદ("unix:") - 1;

            જો (ngx_delete_file(name) == NGX_FILE_ERROR) {
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

    જો (ngx_cycle->files && (ngx_uint_t) s >= ngx_cycle->files_n) {
        ngx_log_error(NGX_LOG_ALERT, log, 0,
                      "the new socket has number %d, "
                      "but only %ui files are available",
                      s, ngx_cycle->files_n);
        પાછા ખાલી;
    }

    ngx_drain_connections((ngx_cycle_t *) ngx_cycle);

    c = ngx_cycle->free_connections;

    જો (c == ખાલી) {
        ngx_log_error(NGX_LOG_ALERT, log, 0,
                      "%ui worker_connections are not enough",
                      ngx_cycle->connection_n);

        પાછા ખાલી;
    }

    ngx_cycle->free_connections = c->data;
    ngx_cycle->free_connection_n--;

    જો (ngx_cycle->files && ngx_cycle->files[s] == ખાલી) {
        ngx_cycle->files[s] = c;
    }

    rev = c->પઠ;
    wev = c->લિખ;

    ngx_memzero(c, કદ(ngx_connection_t));

    c->પઠ = rev;
    c->લિખ = wev;
    c->fd = s;
    c->log = log;

    instance = rev->instance;

    ngx_memzero(rev, કદ(ngx_event_t));
    ngx_memzero(wev, કદ(ngx_event_t));

    rev->instance = !instance;
    wev->instance = !instance;

    rev->index = NGX_INVALID_INDEX;
    wev->index = NGX_INVALID_INDEX;

    rev->data = c;
    wev->data = c;

    wev->લિખ = 1;

    પાછા c;
}


શૂન્ય
ngx_free_connection(ngx_connection_t *c)
{
    c->data = ngx_cycle->free_connections;
    ngx_cycle->free_connections = c;
    ngx_cycle->free_connection_n++;

    જો (ngx_cycle->files && ngx_cycle->files[c->fd] == c) {
        ngx_cycle->files[c->fd] = ખાલી;
    }
}


શૂન્ય
ngx_close_connection(ngx_connection_t *c)
{
    ngx_err_t     err;
    ngx_uint_t    log_error, level;
    ngx_socket_t  fd;

    જો (c->fd == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0, "connection already closed");
        પાછા;
    }

    જો (c->પઠ->timer_set) {
        ngx_del_timer(c->પઠ);
    }

    જો (c->લિખ->timer_set) {
        ngx_del_timer(c->લિખ);
    }

    જો (!c->shared) {
        જો (ngx_del_conn) {
            ngx_del_conn(c, NGX_CLOSE_EVENT);

        } નહીંતર {
            જો (c->પઠ->active || c->પઠ->disabled) {
                ngx_del_event(c->પઠ, NGX_READ_EVENT, NGX_CLOSE_EVENT);
            }

            જો (c->લિખ->active || c->લિખ->disabled) {
                ngx_del_event(c->લિખ, NGX_WRITE_EVENT, NGX_CLOSE_EVENT);
            }
        }
    }

    જો (c->પઠ->posted) {
        ngx_delete_posted_event(c->પઠ);
    }

    જો (c->લિખ->posted) {
        ngx_delete_posted_event(c->લિખ);
    }

    c->પઠ->closed = 1;
    c->લિખ->closed = 1;

    ngx_reusable_connection(c, 0);

    log_error = c->log_error;

    ngx_free_connection(c);

    fd = c->fd;
    c->fd = (ngx_socket_t) -1;

    જો (c->shared) {
        પાછા;
    }

    જો (ngx_close_socket(fd) == -1) {

        err = ngx_socket_errno;

        જો (err == NGX_ECONNRESET || err == NGX_ENOTCONN) {

            પસંદગી (log_error) {

            સ્થિતિ NGX_ERROR_INFO:
                level = NGX_LOG_INFO;
                તોડો;

            સ્થિતિ NGX_ERROR_ERR:
                level = NGX_LOG_ERR;
                તોડો;

            મૂળભૂત:
                level = NGX_LOG_CRIT;
            }

        } નહીંતર {
            level = NGX_LOG_CRIT;
        }

        ngx_log_error(level, c->log, err, ngx_close_socket_n " %d failed", fd);
    }
}


શૂન્ય
ngx_reusable_connection(ngx_connection_t *c, ngx_uint_t reusable)
{
    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "reusable connection: %ui", reusable);

    જો (c->reusable) {
        ngx_queue_remove(&c->queue);
        ngx_cycle->reusable_connections_n--;

#જો (NGX_STAT_STUB)
        (શૂન્ય) ngx_atomic_fetch_add(ngx_stat_waiting, -1);
#endif
    }

    c->reusable = reusable;

    જો (reusable) {
        /* need cast as ngx_cycle is volatile */

        ngx_queue_ઉમેરો_head(
            (ngx_queue_t *) &ngx_cycle->reusable_connections_queue, &c->queue);
        ngx_cycle->reusable_connections_n++;

#જો (NGX_STAT_STUB)
        (શૂન્ય) ngx_atomic_fetch_add(ngx_stat_waiting, 1);
#endif
    }
}


સ્થિર શૂન્ય
ngx_drain_connections(ngx_cycle_t *cycle)
{
    ngx_uint_t         i, n;
    ngx_queue_t       *q;
    ngx_connection_t  *c;

    જો (cycle->free_connection_n > cycle->connection_n / 16
        || cycle->reusable_connections_n == 0)
    {
        પાછા;
    }

    જો (cycle->connections_reuse_time != ngx_time()) {
        cycle->connections_reuse_time = ngx_time();

        ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                      "%ui worker_connections are not enough, "
                      "reusing connections",
                      cycle->connection_n);
    }

    c = ખાલી;
    n = ngx_max(ngx_min(32, cycle->reusable_connections_n / 8), 1);

    માટે (i = 0; i < n; i++) {
        જો (ngx_queue_empty(&cycle->reusable_connections_queue)) {
            તોડો;
        }

        q = ngx_queue_last(&cycle->reusable_connections_queue);
        c = ngx_queue_data(q, ngx_connection_t, queue);

        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "reusing connection");

        c->close = 1;
        c->પઠ->handler(c->પઠ);
    }

    જો (cycle->free_connection_n == 0 && c && c->reusable) {

        /*
         * if no connections were freed, try to reuse the last
         * connection again: this should free it as long as
         * previous reuse moved it to lingering close
         */

        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "reusing connection again");

        c->close = 1;
        c->પઠ->handler(c->પઠ);
    }
}


શૂન્ય
ngx_close_idle_connections(ngx_cycle_t *cycle)
{
    ngx_uint_t         i;
    ngx_connection_t  *c;

    c = cycle->connections;

    માટે (i = 0; i < cycle->connection_n; i++) {

        /* THREAD: lock */

        જો (c[i].fd != (ngx_socket_t) -1 && c[i].idle) {
            c[i].close = 1;
            c[i].પઠ->handler(c[i].પઠ);
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
    સંરચના sockaddr_in   *sin;
#જો (NGX_HAVE_INET6)
    ngx_uint_t            i;
    સંરચના sockaddr_in6  *sin6;
#endif

    addr = 0;

    જો (c->local_socklen) {
        પસંદગી (c->local_sockaddr->sa_family) {

#જો (NGX_HAVE_INET6)
        સ્થિતિ AF_INET6:
            sin6 = (સંરચના sockaddr_in6 *) c->local_sockaddr;

            માટે (i = 0; addr == 0 && i < 16; i++) {
                addr |= sin6->sin6_addr.s6_addr[i];
            }

            તોડો;
#endif

#જો (NGX_HAVE_UNIX_DOMAIN)
        સ્થિતિ AF_UNIX:
            addr = 1;
            તોડો;
#endif

        મૂળભૂત: /* AF_INET */
            sin = (સંરચના sockaddr_in *) c->local_sockaddr;
            addr = sin->sin_addr.s_addr;
            તોડો;
        }
    }

    જો (addr == 0) {

        len = કદ(ngx_sockaddr_t);

        જો (getsockname(c->fd, &sa.sockaddr, &len) == -1) {
            ngx_connection_error(c, ngx_socket_errno, "getsockname() failed");
            પાછા NGX_ERROR;
        }

        c->local_sockaddr = ngx_palloc(c->pool, len);
        જો (c->local_sockaddr == ખાલી) {
            પાછા NGX_ERROR;
        }

        ngx_memcpy(c->local_sockaddr, &sa, len);

        c->local_socklen = len;
    }

    જો (s == ખાલી) {
        પાછા NGX_OK;
    }

    s->len = ngx_sock_ntop(c->local_sockaddr, c->local_socklen,
                           s->data, s->len, port);

    પાછા NGX_OK;
}


ngx_int_t
ngx_tcp_nodelay(ngx_connection_t *c)
{
    પૂર્ણાંક  tcp_nodelay;

    જો (c->tcp_nodelay != NGX_TCP_NODELAY_UNSET) {
        પાછા NGX_OK;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0, "tcp_nodelay");

    tcp_nodelay = 1;

    જો (setsockopt(c->fd, IPPROTO_TCP, TCP_NODELAY,
                   (અચલ શૂન્ય *) &tcp_nodelay, કદ(પૂર્ણાંક))
        == -1)
    {
#જો (NGX_SOLARIS)
        જો (c->log_error == NGX_ERROR_INFO) {

            /* Solaris returns EINVAL if a socket has been shut down */
            c->log_error = NGX_ERROR_IGNORE_EINVAL;

            ngx_connection_error(c, ngx_socket_errno,
                                 "setsockopt(TCP_NODELAY) failed");

            c->log_error = NGX_ERROR_INFO;

            પાછા NGX_ERROR;
        }
#endif

        ngx_connection_error(c, ngx_socket_errno,
                             "setsockopt(TCP_NODELAY) failed");
        પાછા NGX_ERROR;
    }

    c->tcp_nodelay = NGX_TCP_NODELAY_SET;

    પાછા NGX_OK;
}


ngx_int_t
ngx_connection_error(ngx_connection_t *c, ngx_err_t err, અક્ષર *text)
{
    ngx_uint_t  level;

    /* Winsock may return NGX_ECONNABORTED instead of NGX_ECONNRESET */

    જો ((err == NGX_ECONNRESET
#જો (NGX_WIN32)
         || err == NGX_ECONNABORTED
#endif
        ) && c->log_error == NGX_ERROR_IGNORE_ECONNRESET)
    {
        પાછા 0;
    }

#જો (NGX_SOLARIS)
    જો (err == NGX_EINVAL && c->log_error == NGX_ERROR_IGNORE_EINVAL) {
        પાછા 0;
    }
#endif

    જો (err == NGX_EMSGSIZE && c->log_error == NGX_ERROR_IGNORE_EMSGSIZE) {
        પાછા 0;
    }

#જો (NGX_LINUX)
    જો (err == NGX_EIO && c->ktls_recv) {
        પાછા 0;
    }
#endif

    જો (err == 0
        || err == NGX_ECONNRESET
#જો (NGX_WIN32)
        || err == NGX_ECONNABORTED
#નહીંતર
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
        પસંદગી (c->log_error) {

        સ્થિતિ NGX_ERROR_IGNORE_EMSGSIZE:
        સ્થિતિ NGX_ERROR_IGNORE_EINVAL:
        સ્થિતિ NGX_ERROR_IGNORE_ECONNRESET:
        સ્થિતિ NGX_ERROR_INFO:
            level = NGX_LOG_INFO;
            તોડો;

        મૂળભૂત:
            level = NGX_LOG_ERR;
        }

    } નહીંતર {
        level = NGX_LOG_ALERT;
    }

    ngx_log_error(level, c->log, err, text);

    પાછા NGX_ERROR;
}
