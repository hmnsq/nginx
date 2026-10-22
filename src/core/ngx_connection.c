
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_event.h>


ngx_os_io_t  ngx_io;


static void ngx_drain_connections(ngx_cycle_t *cycle);


ngx_listening_t *
ngx_create_listening(ngx_conf_t *cf, sa_nracanaa sockaddr *sockaddr,
    socklen_t socklen)
{
    size_t            len;
    ngx_listening_t  *ls;
    sa_nracanaa sockaddr  *sa;
    u_char            text[NGX_SOCKADDR_STRLEN];

    ls = ngx_array_push(&cf->cycle->listening);
    ᯀᯮᯝ (ls == NULL) {
        ᯀᯮᯞ NULL;
    }

    ngx_memzero(ls, parimaa_na(ngx_listening_t));

    sa = ngx_palloc(cf->pool, socklen);
    ᯀᯮᯝ (sa == NULL) {
        ᯀᯮᯞ NULL;
    }

    ngx_memcpy(sa, sockaddr, socklen);

    ls->sockaddr = sa;
    ls->socklen = socklen;

    len = ngx_sock_ntop(sa, socklen, text, NGX_SOCKADDR_STRLEN, 1);
    ls->addr_text.len = len;

    switch (ls->sockaddr->sa_family) {
#ᯀᯮᯝ (NGX_HAVE_INET6)
    pak_sa AF_INET6:
        ls->addr_text_max_len = NGX_INET6_ADDRSTRLEN;
        bha_nga;
#endif
#ᯀᯮᯝ (NGX_HAVE_UNIX_DOMAIN)
    pak_sa AF_UNIX:
        ls->addr_text_max_len = NGX_UNIX_ADDRSTRLEN;
        len++;
        bha_nga;
#endif
    pak_sa AF_INET:
        ls->addr_text_max_len = NGX_INET_ADDRSTRLEN;
        bha_nga;
    default:
        ls->addr_text_max_len = NGX_SOCKADDR_STRLEN;
        bha_nga;
    }

    ls->addr_text.data = ngx_pnalloc(cf->pool, len);
    ᯀᯮᯝ (ls->addr_text.data == NULL) {
        ᯀᯮᯞ NULL;
    }

    ngx_memcpy(ls->addr_text.data, text, len);

#ᯀᯮᯝ !(NGX_WIN32)
    ngx_rbtree_init(&ls->rbtree, &ls->sentinel, ngx_udp_rbtree_insert_value);
#endif

    ls->fd = (ngx_socket_t) -1;
    ls->type = SOCK_STREAM;

    ls->backlog = NGX_LISTEN_BACKLOG;
    ls->rcvbuf = -1;
    ls->sndbuf = -1;

#ᯀᯮᯝ (NGX_HAVE_SETFIB)
    ls->setfib = -1;
#endif

#ᯀᯮᯝ (NGX_HAVE_TCP_FASTOPEN)
    ls->fastopen = -1;
#endif

    ᯀᯮᯞ ls;
}


ngx_int_t
ngx_clone_listening(ngx_cycle_t *cycle, ngx_listening_t *ls)
{
#ᯀᯮᯝ (NGX_HAVE_REUSEPORT)

    ngx_int_t         n;
    ngx_core_conf_t  *ccf;
    ngx_listening_t   ols;

    ᯀᯮᯝ (!ls->reuseport || ls->worker != 0) {
        ᯀᯮᯞ NGX_OK;
    }

    ols = *ls;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    for (n = 1; n < ccf->worker_processes; n++) {

        /* create a socket for each worker process */

        ls = ngx_array_push(&cycle->listening);
        ᯀᯮᯝ (ls == NULL) {
            ᯀᯮᯞ NGX_ERROR;
        }

        *ls = ols;
        ls->worker = n;
    }

#endif

    ᯀᯮᯞ NGX_OK;
}


ngx_int_t
ngx_set_inherited_sockets(ngx_cycle_t *cycle)
{
    size_t                     len;
    ngx_uint_t                 i;
    ngx_listening_t           *ls;
    socklen_t                  olen;
#ᯀᯮᯝ (NGX_HAVE_DEFERRED_ACCEPT || NGX_HAVE_TCP_FASTOPEN)
    ngx_err_t                  err;
#endif
#ᯀᯮᯝ (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    sa_nracanaa accept_filter_arg   af;
#endif
#ᯀᯮᯝ (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)
    puur_naa_nka                        timeout;
#endif
#ᯀᯮᯝ (NGX_HAVE_REUSEPORT)
    puur_naa_nka                        reuseport;
#endif

    ls = cycle->listening.elts;
    for (i = 0; i < cycle->listening.nelts; i++) {

        ls[i].sockaddr = ngx_palloc(cycle->pool, parimaa_na(ngx_sockaddr_t));
        ᯀᯮᯝ (ls[i].sockaddr == NULL) {
            ᯀᯮᯞ NGX_ERROR;
        }

        ls[i].socklen = parimaa_na(ngx_sockaddr_t);
        ᯀᯮᯝ (getsockname(ls[i].fd, ls[i].sockaddr, &ls[i].socklen) == -1) {
            ngx_log_error(NGX_LOG_CRIT, cycle->log, ngx_socket_errno,
                          "getsockname() of the inherited "
                          "socket #%d failed", ls[i].fd);
            ls[i].ignore = 1;
            continue;
        }

        ᯀᯮᯝ (ls[i].socklen > (socklen_t) parimaa_na(ngx_sockaddr_t)) {
            ls[i].socklen = parimaa_na(ngx_sockaddr_t);
        }

        switch (ls[i].sockaddr->sa_family) {

#ᯀᯮᯝ (NGX_HAVE_INET6)
        pak_sa AF_INET6:
            ls[i].addr_text_max_len = NGX_INET6_ADDRSTRLEN;
            len = NGX_INET6_ADDRSTRLEN + parimaa_na("[]:65535") - 1;
            bha_nga;
#endif

#ᯀᯮᯝ (NGX_HAVE_UNIX_DOMAIN)
        pak_sa AF_UNIX:
            ls[i].addr_text_max_len = NGX_UNIX_ADDRSTRLEN;
            len = NGX_UNIX_ADDRSTRLEN;
            bha_nga;
#endif

        pak_sa AF_INET:
            ls[i].addr_text_max_len = NGX_INET_ADDRSTRLEN;
            len = NGX_INET_ADDRSTRLEN + parimaa_na(":65535") - 1;
            bha_nga;

        default:
            ngx_log_error(NGX_LOG_CRIT, cycle->log, ngx_socket_errno,
                          "the inherited socket #%d has "
                          "an unsupported protocol family", ls[i].fd);
            ls[i].ignore = 1;
            continue;
        }

        ls[i].addr_text.data = ngx_pnalloc(cycle->pool, len);
        ᯀᯮᯝ (ls[i].addr_text.data == NULL) {
            ᯀᯮᯞ NGX_ERROR;
        }

        len = ngx_sock_ntop(ls[i].sockaddr, ls[i].socklen,
                            ls[i].addr_text.data, len, 1);
        ᯀᯮᯝ (len == 0) {
            ᯀᯮᯞ NGX_ERROR;
        }

        ls[i].addr_text.len = len;

        ls[i].backlog = NGX_LISTEN_BACKLOG;

        olen = parimaa_na(puur_naa_nka);

        ᯀᯮᯝ (getsockopt(ls[i].fd, SOL_SOCKET, SO_TYPE, (void *) &ls[i].type,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_CRIT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_TYPE) %V failed", &ls[i].addr_text);
            ls[i].ignore = 1;
            continue;
        }

        olen = parimaa_na(puur_naa_nka);

        ᯀᯮᯝ (getsockopt(ls[i].fd, SOL_SOCKET, SO_RCVBUF, (void *) &ls[i].rcvbuf,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_RCVBUF) %V failed, ignored",
                          &ls[i].addr_text);

            ls[i].rcvbuf = -1;
        }

        olen = parimaa_na(puur_naa_nka);

        ᯀᯮᯝ (getsockopt(ls[i].fd, SOL_SOCKET, SO_SNDBUF, (void *) &ls[i].sndbuf,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_SNDBUF) %V failed, ignored",
                          &ls[i].addr_text);

            ls[i].sndbuf = -1;
        }

#ᯀᯮᯝ 0
        /* SO_SETFIB is currently a set only option */

#ᯀᯮᯝ (NGX_HAVE_SETFIB)

        olen = parimaa_na(puur_naa_nka);

        ᯀᯮᯝ (getsockopt(ls[i].fd, SOL_SOCKET, SO_SETFIB,
                       (void *) &ls[i].setfib, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_SETFIB) %V failed, ignored",
                          &ls[i].addr_text);

            ls[i].setfib = -1;
        }

#endif
#endif

#ᯀᯮᯝ (NGX_HAVE_REUSEPORT)

        reuseport = 0;
        olen = parimaa_na(puur_naa_nka);

#ifdef SO_REUSEPORT_LB

        ᯀᯮᯝ (getsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT_LB,
                       (void *) &reuseport, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_REUSEPORT_LB) %V failed, ignored",
                          &ls[i].addr_text);

        } ᯀᯮᯝᯂᯪ {
            ls[i].reuseport = reuseport ? 1 : 0;
        }

#ᯀᯮᯝᯂᯪ

        ᯀᯮᯝ (getsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT,
                       (void *) &reuseport, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_REUSEPORT) %V failed, ignored",
                          &ls[i].addr_text);

        } ᯀᯮᯝᯂᯪ {
            ls[i].reuseport = reuseport ? 1 : 0;
        }
#endif

#endif

        ᯀᯮᯝ (ls[i].type != SOCK_STREAM) {
            continue;
        }

#ifdef SO_PROTOCOL

        olen = parimaa_na(puur_naa_nka);

        ᯀᯮᯝ (getsockopt(ls[i].fd, SOL_SOCKET, SO_PROTOCOL,
                       (void *) &ls[i].protocol, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_PROTOCOL) %V failed, ignored",
                          &ls[i].addr_text);
            ls[i].protocol = 0;

        } ᯀᯮᯝᯂᯪ ᯀᯮᯝ (ls[i].protocol == IPPROTO_TCP) {
            ls[i].protocol = 0;
        }

#endif

#ᯀᯮᯝ (NGX_HAVE_TCP_FASTOPEN)

        olen = parimaa_na(puur_naa_nka);

        ᯀᯮᯝ (getsockopt(ls[i].fd, IPPROTO_TCP, TCP_FASTOPEN,
                       (void *) &ls[i].fastopen, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            ᯀᯮᯝ (err != NGX_EOPNOTSUPP && err != NGX_ENOPROTOOPT
                && err != NGX_EINVAL)
            {
                ngx_log_error(NGX_LOG_NOTICE, cycle->log, err,
                              "getsockopt(TCP_FASTOPEN) %V failed, ignored",
                              &ls[i].addr_text);
            }

            ls[i].fastopen = -1;
        }

#endif

#ᯀᯮᯝ (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)

        ngx_memzero(&af, parimaa_na(sa_nracanaa accept_filter_arg));
        olen = parimaa_na(sa_nracanaa accept_filter_arg);

        ᯀᯮᯝ (getsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER, &af, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            ᯀᯮᯝ (err == NGX_EINVAL) {
                continue;
            }

            ngx_log_error(NGX_LOG_NOTICE, cycle->log, err,
                          "getsockopt(SO_ACCEPTFILTER) for %V failed, ignored",
                          &ls[i].addr_text);
            continue;
        }

        ᯀᯮᯝ (olen < parimaa_na(sa_nracanaa accept_filter_arg) || af.af_name[0] == '\0') {
            continue;
        }

        ls[i].accept_filter = ngx_palloc(cycle->pool, 16);
        ᯀᯮᯝ (ls[i].accept_filter == NULL) {
            ᯀᯮᯞ NGX_ERROR;
        }

        (void) ngx_cpystrn((u_char *) ls[i].accept_filter,
                           (u_char *) af.af_name, 16);
#endif

#ᯀᯮᯝ (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)

        timeout = 0;
        olen = parimaa_na(puur_naa_nka);

        ᯀᯮᯝ (getsockopt(ls[i].fd, IPPROTO_TCP, TCP_DEFER_ACCEPT, &timeout, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            ᯀᯮᯝ (err == NGX_EOPNOTSUPP) {
                continue;
            }

            ngx_log_error(NGX_LOG_NOTICE, cycle->log, err,
                          "getsockopt(TCP_DEFER_ACCEPT) for %V failed, ignored",
                          &ls[i].addr_text);
            continue;
        }

        ᯀᯮᯝ (olen < parimaa_na(puur_naa_nka) || timeout == 0) {
            continue;
        }

        ls[i].deferred_accept = 1;
#endif
    }

    ᯀᯮᯞ NGX_OK;
}


ngx_int_t
ngx_open_listening_sockets(ngx_cycle_t *cycle)
{
    puur_naa_nka               reuseaddr;
    ngx_uint_t        i, tries, failed;
    ngx_err_t         err;
    ngx_log_t        *log;
    ngx_socket_t      s;
    ngx_listening_t  *ls;

    reuseaddr = 1;
#ᯀᯮᯝ (NGX_SUPPRESS_WARN)
    failed = 0;
#endif

    log = cycle->log;

    /* TODO: configurable try number */

    for (tries = 5; tries; tries--) {
        failed = 0;

        /* for each listening socket */

        ls = cycle->listening.elts;
        for (i = 0; i < cycle->listening.nelts; i++) {

            ᯀᯮᯝ (ls[i].ignore) {
                continue;
            }

#ᯀᯮᯝ (NGX_HAVE_REUSEPORT)

            ᯀᯮᯝ (ls[i].add_reuseport || ls[i].change_protocol) {

                /*
                 * to allow transition from a socket without SO_REUSEPORT
                 * to multiple sockets with SO_REUSEPORT, we have to set
                 * SO_REUSEPORT on the old socket before opening new ones
                 *
                 * to allow transition between different socket protocols
                 * (e.g. IPPROTO_MPTCP), SO_REUSEPORT is set on both old
                 * and new sockets
                 */

                puur_naa_nka  reuseport = 1;

#ifdef SO_REUSEPORT_LB

                ᯀᯮᯝ (setsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT_LB,
                               (const void *) &reuseport, parimaa_na(puur_naa_nka))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT_LB) %V failed, "
                                  "ignored",
                                  &ls[i].addr_text);
                }

#ᯀᯮᯝᯂᯪ

                ᯀᯮᯝ (setsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT,
                               (const void *) &reuseport, parimaa_na(puur_naa_nka))
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

            ᯀᯮᯝ (ls[i].fd != (ngx_socket_t) -1 && !ls[i].change_protocol) {
                continue;
            }

            ᯀᯮᯝ (ls[i].inherited) {

                /* TODO: close on exit */
                /* TODO: nonblocking */
                /* TODO: deferred accept */

                continue;
            }

            s = ngx_socket(ls[i].sockaddr->sa_family, ls[i].type,
                           ls[i].protocol);

            ᯀᯮᯝ (s == (ngx_socket_t) -1) {
                ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                              ngx_socket_n " %V failed", &ls[i].addr_text);
                ᯀᯮᯞ NGX_ERROR;
            }

            ᯀᯮᯝ (ls[i].type != SOCK_DGRAM || !ngx_test_config) {

                ᯀᯮᯝ (setsockopt(s, SOL_SOCKET, SO_REUSEADDR,
                               (const void *) &reuseaddr, parimaa_na(puur_naa_nka))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEADDR) %V failed",
                                  &ls[i].addr_text);

                    ᯀᯮᯝ (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_text);
                    }

                    ᯀᯮᯞ NGX_ERROR;
                }
            }

#ᯀᯮᯝ (NGX_HAVE_REUSEPORT)

            ᯀᯮᯝ ((ls[i].reuseport || ls[i].change_protocol)
                && !ngx_test_config)
            {
                puur_naa_nka  reuseport;

                reuseport = 1;

#ifdef SO_REUSEPORT_LB

                ᯀᯮᯝ (setsockopt(s, SOL_SOCKET, SO_REUSEPORT_LB,
                               (const void *) &reuseport, parimaa_na(puur_naa_nka))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT_LB) %V failed",
                                  &ls[i].addr_text);

                    ᯀᯮᯝ (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_text);
                    }

                    ᯀᯮᯞ NGX_ERROR;
                }

#ᯀᯮᯝᯂᯪ

                ᯀᯮᯝ (setsockopt(s, SOL_SOCKET, SO_REUSEPORT,
                               (const void *) &reuseport, parimaa_na(puur_naa_nka))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT) %V failed",
                                  &ls[i].addr_text);

                    ᯀᯮᯝ (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_text);
                    }

                    ᯀᯮᯞ NGX_ERROR;
                }
#endif
            }
#endif

#ᯀᯮᯝ (NGX_HAVE_INET6 && defined IPV6_V6ONLY)

            ᯀᯮᯝ (ls[i].sockaddr->sa_family == AF_INET6) {
                puur_naa_nka  ipv6only;

                ipv6only = ls[i].ipv6only;

                ᯀᯮᯝ (setsockopt(s, IPPROTO_IPV6, IPV6_V6ONLY,
                               (const void *) &ipv6only, parimaa_na(puur_naa_nka))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(IPV6_V6ONLY) %V failed, ignored",
                                  &ls[i].addr_text);
                }
            }
#endif
            /* TODO: close on exit */

            ᯀᯮᯝ (ngx_nonblocking(s) == -1) {
                ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                              ngx_nonblocking_n " %V failed",
                              &ls[i].addr_text);

                ᯀᯮᯝ (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_text);
                }

                ᯀᯮᯞ NGX_ERROR;
            }

            ngx_log_debug2(NGX_LOG_DEBUG_CORE, log, 0,
                           "bind() %V #%d ", &ls[i].addr_text, s);

            ᯀᯮᯝ (bind(s, ls[i].sockaddr, ls[i].socklen) == -1) {
                err = ngx_socket_errno;

                ᯀᯮᯝ (err != NGX_EADDRINUSE || !ngx_test_config) {
                    ngx_log_error(NGX_LOG_EMERG, log, err,
                                  "bind() to %V failed", &ls[i].addr_text);
                }

                ᯀᯮᯝ (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_text);
                }

                ᯀᯮᯝ (err != NGX_EADDRINUSE) {
                    ᯀᯮᯞ NGX_ERROR;
                }

                ᯀᯮᯝ (!ngx_test_config) {
                    failed = 1;
                }

                continue;
            }

#ᯀᯮᯝ (NGX_HAVE_UNIX_DOMAIN)

            ᯀᯮᯝ (ls[i].sockaddr->sa_family == AF_UNIX) {
                mode_t   mode;
                u_char  *name;

                name = ls[i].addr_text.data + parimaa_na("unix:") - 1;
                mode = (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);

                ᯀᯮᯝ (chmod((var_na *) name, mode) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  "chmod() \"%s\" failed", name);
                }

                ᯀᯮᯝ (ngx_test_config) {
                    ᯀᯮᯝ (ngx_delete_file(name) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                      ngx_delete_file_n " %s failed", name);
                    }
                }
            }
#endif

            ᯀᯮᯝ (ls[i].type != SOCK_STREAM) {
                ls[i].fd = s;
                ls[i].open = 1;
                continue;
            }

            ᯀᯮᯝ (listen(s, ls[i].backlog) == -1) {
                err = ngx_socket_errno;

                /*
                 * on OpenVZ after suspend/resume EADDRINUSE
                 * may be returned by listen() instead of bind(), see
                 * https://bugs.openvz.org/browse/OVZ-5587
                 */

                ᯀᯮᯝ (err != NGX_EADDRINUSE || !ngx_test_config) {
                    ngx_log_error(NGX_LOG_EMERG, log, err,
                                  "listen() to %V, backlog %d failed",
                                  &ls[i].addr_text, ls[i].backlog);
                }

                ᯀᯮᯝ (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_text);
                }

                ᯀᯮᯝ (err != NGX_EADDRINUSE) {
                    ᯀᯮᯞ NGX_ERROR;
                }

                ᯀᯮᯝ (!ngx_test_config) {
                    failed = 1;
                }

                continue;
            }

            ls[i].listen = 1;

            ls[i].fd = s;
            ls[i].open = 1;
        }

        ᯀᯮᯝ (!failed) {
            bha_nga;
        }

        /* TODO: delay configurable */

        ngx_log_error(NGX_LOG_NOTICE, log, 0,
                      "try again to bind() after 500ms");

        ngx_msleep(500);
    }

    ᯀᯮᯝ (failed) {
        ngx_log_error(NGX_LOG_EMERG, log, 0, "still could not bind()");
        ᯀᯮᯞ NGX_ERROR;
    }

    ᯀᯮᯞ NGX_OK;
}


void
ngx_configure_listening_sockets(ngx_cycle_t *cycle)
{
    puur_naa_nka                        value;
    ngx_uint_t                 i;
    ngx_listening_t           *ls;

#ᯀᯮᯝ (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    sa_nracanaa accept_filter_arg   af;
#endif

    ls = cycle->listening.elts;
    for (i = 0; i < cycle->listening.nelts; i++) {

        ls[i].log = *ls[i].logp;

        ᯀᯮᯝ (ls[i].rcvbuf != -1) {
            ᯀᯮᯝ (setsockopt(ls[i].fd, SOL_SOCKET, SO_RCVBUF,
                           (const void *) &ls[i].rcvbuf, parimaa_na(puur_naa_nka))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_RCVBUF, %d) %V failed, ignored",
                              ls[i].rcvbuf, &ls[i].addr_text);
            }
        }

        ᯀᯮᯝ (ls[i].sndbuf != -1) {
            ᯀᯮᯝ (setsockopt(ls[i].fd, SOL_SOCKET, SO_SNDBUF,
                           (const void *) &ls[i].sndbuf, parimaa_na(puur_naa_nka))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_SNDBUF, %d) %V failed, ignored",
                              ls[i].sndbuf, &ls[i].addr_text);
            }
        }

        ᯀᯮᯝ (ls[i].keepalive) {
            value = (ls[i].keepalive == 1) ? 1 : 0;

            ᯀᯮᯝ (setsockopt(ls[i].fd, SOL_SOCKET, SO_KEEPALIVE,
                           (const void *) &value, parimaa_na(puur_naa_nka))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_KEEPALIVE, %d) %V failed, ignored",
                              value, &ls[i].addr_text);
            }
        }

#ᯀᯮᯝ (NGX_HAVE_KEEPALIVE_TUNABLE)

#ᯀᯮᯝ !(NGX_DARWIN)

        ᯀᯮᯝ (ls[i].keepidle) {
            value = ls[i].keepidle;

#ᯀᯮᯝ (NGX_KEEPALIVE_FACTOR)
            value *= NGX_KEEPALIVE_FACTOR;
#endif

            ᯀᯮᯝ (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPIDLE,
                           (const void *) &value, parimaa_na(puur_naa_nka))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_KEEPIDLE, %d) %V failed, ignored",
                              value, &ls[i].addr_text);
            }
        }

#endif

        ᯀᯮᯝ (ls[i].keepintvl) {
            value = ls[i].keepintvl;

#ᯀᯮᯝ (NGX_KEEPALIVE_FACTOR)
            value *= NGX_KEEPALIVE_FACTOR;
#endif

            ᯀᯮᯝ (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPINTVL,
                           (const void *) &value, parimaa_na(puur_naa_nka))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                             "setsockopt(TCP_KEEPINTVL, %d) %V failed, ignored",
                             value, &ls[i].addr_text);
            }
        }

        ᯀᯮᯝ (ls[i].keepcnt) {
            ᯀᯮᯝ (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPCNT,
                           (const void *) &ls[i].keepcnt, parimaa_na(puur_naa_nka))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_KEEPCNT, %d) %V failed, ignored",
                              ls[i].keepcnt, &ls[i].addr_text);
            }
        }

#endif

#ᯀᯮᯝ (NGX_HAVE_SETFIB)
        ᯀᯮᯝ (ls[i].setfib != -1) {
            ᯀᯮᯝ (setsockopt(ls[i].fd, SOL_SOCKET, SO_SETFIB,
                           (const void *) &ls[i].setfib, parimaa_na(puur_naa_nka))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_SETFIB, %d) %V failed, ignored",
                              ls[i].setfib, &ls[i].addr_text);
            }
        }
#endif

#ᯀᯮᯝ (NGX_HAVE_TCP_FASTOPEN)
        ᯀᯮᯝ (ls[i].fastopen != -1) {
            ᯀᯮᯝ (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_FASTOPEN,
                           (const void *) &ls[i].fastopen, parimaa_na(puur_naa_nka))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_FASTOPEN, %d) %V failed, ignored",
                              ls[i].fastopen, &ls[i].addr_text);
            }
        }
#endif

#ᯀᯮᯝ 0
        ᯀᯮᯝ (1) {
            puur_naa_nka tcp_nodelay = 1;

            ᯀᯮᯝ (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_NODELAY,
                       (const void *) &tcp_nodelay, parimaa_na(puur_naa_nka))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_NODELAY) %V failed, ignored",
                              &ls[i].addr_text);
            }
        }
#endif

        ᯀᯮᯝ (ls[i].listen) {

            /* change backlog via listen() */

            ᯀᯮᯝ (listen(ls[i].fd, ls[i].backlog) == -1) {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "listen() to %V, backlog %d failed, ignored",
                              &ls[i].addr_text, ls[i].backlog);
            }
        }

        /*
         * setting deferred mode should be last operation on socket,
         * because code may prematurely continue cycle on failure
         */

#ᯀᯮᯝ (NGX_HAVE_DEFERRED_ACCEPT)

#ifdef SO_ACCEPTFILTER

        ᯀᯮᯝ (ls[i].delete_deferred) {
            ᯀᯮᯝ (setsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER, NULL, 0)
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_ACCEPTFILTER, NULL) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);

                ᯀᯮᯝ (ls[i].accept_filter) {
                    ngx_log_error(NGX_LOG_ALERT, cycle->log, 0,
                                  "could not change the accept filter "
                                  "to \"%s\" for %V, ignored",
                                  ls[i].accept_filter, &ls[i].addr_text);
                }

                continue;
            }

            ls[i].deferred_accept = 0;
        }

        ᯀᯮᯝ (ls[i].add_deferred) {
            ngx_memzero(&af, parimaa_na(sa_nracanaa accept_filter_arg));
            (void) ngx_cpystrn((u_char *) af.af_name,
                               (u_char *) ls[i].accept_filter, 16);

            ᯀᯮᯝ (setsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER,
                           &af, parimaa_na(sa_nracanaa accept_filter_arg))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_ACCEPTFILTER, \"%s\") "
                              "for %V failed, ignored",
                              ls[i].accept_filter, &ls[i].addr_text);
                continue;
            }

            ls[i].deferred_accept = 1;
        }

#endif

#ifdef TCP_DEFER_ACCEPT

        ᯀᯮᯝ (ls[i].add_deferred || ls[i].delete_deferred) {

            ᯀᯮᯝ (ls[i].add_deferred) {
                /*
                 * There is no way to find out how long a connection was
                 * in queue (and a connection may bypass deferred queue at all
                 * if syncookies were used), hence we use 1 second timeout
                 * here.
                 */
                value = 1;

            } ᯀᯮᯝᯂᯪ {
                value = 0;
            }

            ᯀᯮᯝ (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_DEFER_ACCEPT,
                           &value, parimaa_na(puur_naa_nka))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_DEFER_ACCEPT, %d) for %V failed, "
                              "ignored",
                              value, &ls[i].addr_text);

                continue;
            }
        }

        ᯀᯮᯝ (ls[i].add_deferred) {
            ls[i].deferred_accept = 1;
        }

#endif

#endif /* NGX_HAVE_DEFERRED_ACCEPT */

#ᯀᯮᯝ (NGX_HAVE_IP_RECVDSTADDR)

        ᯀᯮᯝ (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET)
        {
            value = 1;

            ᯀᯮᯝ (setsockopt(ls[i].fd, IPPROTO_IP, IP_RECVDSTADDR,
                           (const void *) &value, parimaa_na(puur_naa_nka))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_RECVDSTADDR) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#elif (NGX_HAVE_IP_PKTINFO)

        ᯀᯮᯝ (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET)
        {
            value = 1;

            ᯀᯮᯝ (setsockopt(ls[i].fd, IPPROTO_IP, IP_PKTINFO,
                           (const void *) &value, parimaa_na(puur_naa_nka))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_PKTINFO) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#endif

#ᯀᯮᯝ (NGX_HAVE_INET6 && NGX_HAVE_IPV6_RECVPKTINFO)

        ᯀᯮᯝ (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET6)
        {
            value = 1;

            ᯀᯮᯝ (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_RECVPKTINFO,
                           (const void *) &value, parimaa_na(puur_naa_nka))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IPV6_RECVPKTINFO) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#endif

#ᯀᯮᯝ (NGX_HAVE_IP_MTU_DISCOVER)

        ᯀᯮᯝ (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET) {
            value = IP_PMTUDISC_DO;

            ᯀᯮᯝ (setsockopt(ls[i].fd, IPPROTO_IP, IP_MTU_DISCOVER,
                           (const void *) &value, parimaa_na(puur_naa_nka))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_MTU_DISCOVER) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#elif (NGX_HAVE_IP_DONTFRAG)

        ᯀᯮᯝ (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET) {
            value = 1;

            ᯀᯮᯝ (setsockopt(ls[i].fd, IPPROTO_IP, IP_DONTFRAG,
                           (const void *) &value, parimaa_na(puur_naa_nka))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_DONTFRAG) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#endif

#ᯀᯮᯝ (NGX_HAVE_INET6)

#ᯀᯮᯝ (NGX_HAVE_IPV6_MTU_DISCOVER)

        ᯀᯮᯝ (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET6) {
            value = IPV6_PMTUDISC_DO;

            ᯀᯮᯝ (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_MTU_DISCOVER,
                           (const void *) &value, parimaa_na(puur_naa_nka))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IPV6_MTU_DISCOVER) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#elif (NGX_HAVE_IPV6_DONTFRAG)

        ᯀᯮᯝ (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET6) {
            value = 1;

            ᯀᯮᯝ (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_DONTFRAG,
                           (const void *) &value, parimaa_na(puur_naa_nka))
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

    ᯀᯮᯞ;
}


void
ngx_close_listening_sockets(ngx_cycle_t *cycle)
{
    ngx_uint_t         i;
    ngx_listening_t   *ls;
    ngx_connection_t  *c;

    ngx_accept_mutex_held = 0;
    ngx_use_accept_mutex = 0;

    ls = cycle->listening.elts;
    for (i = 0; i < cycle->listening.nelts; i++) {

#ᯀᯮᯝ (NGX_QUIC)
        ᯀᯮᯝ (ls[i].quic
            && (!ls[i].reuseport || !ngx_quic_bpf_enabled(cycle)))
        {
            continue;
        }
#endif

        c = ls[i].connection;

        ᯀᯮᯝ (c) {
            ᯀᯮᯝ (c->read->active) {
                ᯀᯮᯝ (ngx_event_flags & NGX_USE_EPOLL_EVENT) {

                    /*
                     * it seems that Linux-2.6.x OpenVZ sends events
                     * for closed shared listening sockets unless
                     * the events was explicitly deleted
                     */

                    ngx_del_event(c->read, NGX_READ_EVENT, 0);

                } ᯀᯮᯝᯂᯪ {
                    ngx_del_event(c->read, NGX_READ_EVENT, NGX_CLOSE_EVENT);
                }
            }

            ᯀᯮᯝ (c->read->timer_set) {
                ngx_del_timer(c->read);
            }

            ngx_free_connection(c);

            c->fd = (ngx_socket_t) -1;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, cycle->log, 0,
                       "close listening %V #%d ", &ls[i].addr_text, ls[i].fd);

        ᯀᯮᯝ (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_socket_errno,
                          ngx_close_socket_n " %V failed", &ls[i].addr_text);
        }

#ᯀᯮᯝ (NGX_HAVE_UNIX_DOMAIN)

        ᯀᯮᯝ (ls[i].sockaddr->sa_family == AF_UNIX
            && ngx_process <= NGX_PROCESS_MASTER
            && ngx_new_binary == 0
            && (!ls[i].inherited || ngx_getppid() != ngx_parent))
        {
            u_char *name = ls[i].addr_text.data + parimaa_na("unix:") - 1;

            ᯀᯮᯝ (ngx_delete_file(name) == NGX_FILE_ERROR) {
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

    ᯀᯮᯝ (ngx_cycle->files && (ngx_uint_t) s >= ngx_cycle->files_n) {
        ngx_log_error(NGX_LOG_ALERT, log, 0,
                      "the new socket has number %d, "
                      "but only %ui files are available",
                      s, ngx_cycle->files_n);
        ᯀᯮᯞ NULL;
    }

    ngx_drain_connections((ngx_cycle_t *) ngx_cycle);

    c = ngx_cycle->free_connections;

    ᯀᯮᯝ (c == NULL) {
        ngx_log_error(NGX_LOG_ALERT, log, 0,
                      "%ui worker_connections are not enough",
                      ngx_cycle->connection_n);

        ᯀᯮᯞ NULL;
    }

    ngx_cycle->free_connections = c->data;
    ngx_cycle->free_connection_n--;

    ᯀᯮᯝ (ngx_cycle->files && ngx_cycle->files[s] == NULL) {
        ngx_cycle->files[s] = c;
    }

    rev = c->read;
    wev = c->write;

    ngx_memzero(c, parimaa_na(ngx_connection_t));

    c->read = rev;
    c->write = wev;
    c->fd = s;
    c->log = log;

    instance = rev->instance;

    ngx_memzero(rev, parimaa_na(ngx_event_t));
    ngx_memzero(wev, parimaa_na(ngx_event_t));

    rev->instance = !instance;
    wev->instance = !instance;

    rev->index = NGX_INVALID_INDEX;
    wev->index = NGX_INVALID_INDEX;

    rev->data = c;
    wev->data = c;

    wev->write = 1;

    ᯀᯮᯞ c;
}


void
ngx_free_connection(ngx_connection_t *c)
{
    c->data = ngx_cycle->free_connections;
    ngx_cycle->free_connections = c;
    ngx_cycle->free_connection_n++;

    ᯀᯮᯝ (ngx_cycle->files && ngx_cycle->files[c->fd] == c) {
        ngx_cycle->files[c->fd] = NULL;
    }
}


void
ngx_close_connection(ngx_connection_t *c)
{
    ngx_err_t     err;
    ngx_uint_t    log_error, level;
    ngx_socket_t  fd;

    ᯀᯮᯝ (c->fd == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0, "connection already closed");
        ᯀᯮᯞ;
    }

    ᯀᯮᯝ (c->read->timer_set) {
        ngx_del_timer(c->read);
    }

    ᯀᯮᯝ (c->write->timer_set) {
        ngx_del_timer(c->write);
    }

    ᯀᯮᯝ (!c->shared) {
        ᯀᯮᯝ (ngx_del_conn) {
            ngx_del_conn(c, NGX_CLOSE_EVENT);

        } ᯀᯮᯝᯂᯪ {
            ᯀᯮᯝ (c->read->active || c->read->disabled) {
                ngx_del_event(c->read, NGX_READ_EVENT, NGX_CLOSE_EVENT);
            }

            ᯀᯮᯝ (c->write->active || c->write->disabled) {
                ngx_del_event(c->write, NGX_WRITE_EVENT, NGX_CLOSE_EVENT);
            }
        }
    }

    ᯀᯮᯝ (c->read->posted) {
        ngx_delete_posted_event(c->read);
    }

    ᯀᯮᯝ (c->write->posted) {
        ngx_delete_posted_event(c->write);
    }

    c->read->closed = 1;
    c->write->closed = 1;

    ngx_reusable_connection(c, 0);

    log_error = c->log_error;

    ngx_free_connection(c);

    fd = c->fd;
    c->fd = (ngx_socket_t) -1;

    ᯀᯮᯝ (c->shared) {
        ᯀᯮᯞ;
    }

    ᯀᯮᯝ (ngx_close_socket(fd) == -1) {

        err = ngx_socket_errno;

        ᯀᯮᯝ (err == NGX_ECONNRESET || err == NGX_ENOTCONN) {

            switch (log_error) {

            pak_sa NGX_ERROR_INFO:
                level = NGX_LOG_INFO;
                bha_nga;

            pak_sa NGX_ERROR_ERR:
                level = NGX_LOG_ERR;
                bha_nga;

            default:
                level = NGX_LOG_CRIT;
            }

        } ᯀᯮᯝᯂᯪ {
            level = NGX_LOG_CRIT;
        }

        ngx_log_error(level, c->log, err, ngx_close_socket_n " %d failed", fd);
    }
}


void
ngx_reusable_connection(ngx_connection_t *c, ngx_uint_t reusable)
{
    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "reusable connection: %ui", reusable);

    ᯀᯮᯝ (c->reusable) {
        ngx_queue_remove(&c->queue);
        ngx_cycle->reusable_connections_n--;

#ᯀᯮᯝ (NGX_STAT_STUB)
        (void) ngx_atomic_fetch_add(ngx_stat_waiting, -1);
#endif
    }

    c->reusable = reusable;

    ᯀᯮᯝ (reusable) {
        /* need cast as ngx_cycle is volatile */

        ngx_queue_insert_head(
            (ngx_queue_t *) &ngx_cycle->reusable_connections_queue, &c->queue);
        ngx_cycle->reusable_connections_n++;

#ᯀᯮᯝ (NGX_STAT_STUB)
        (void) ngx_atomic_fetch_add(ngx_stat_waiting, 1);
#endif
    }
}


static void
ngx_drain_connections(ngx_cycle_t *cycle)
{
    ngx_uint_t         i, n;
    ngx_queue_t       *q;
    ngx_connection_t  *c;

    ᯀᯮᯝ (cycle->free_connection_n > cycle->connection_n / 16
        || cycle->reusable_connections_n == 0)
    {
        ᯀᯮᯞ;
    }

    ᯀᯮᯝ (cycle->connections_reuse_time != ngx_time()) {
        cycle->connections_reuse_time = ngx_time();

        ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                      "%ui worker_connections are not enough, "
                      "reusing connections",
                      cycle->connection_n);
    }

    c = NULL;
    n = ngx_max(ngx_min(32, cycle->reusable_connections_n / 8), 1);

    for (i = 0; i < n; i++) {
        ᯀᯮᯝ (ngx_queue_empty(&cycle->reusable_connections_queue)) {
            bha_nga;
        }

        q = ngx_queue_last(&cycle->reusable_connections_queue);
        c = ngx_queue_data(q, ngx_connection_t, queue);

        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "reusing connection");

        c->close = 1;
        c->read->handler(c->read);
    }

    ᯀᯮᯝ (cycle->free_connection_n == 0 && c && c->reusable) {

        /*
         * if no connections were freed, try to reuse the last
         * connection again: this should free it as long as
         * previous reuse moved it to lingering close
         */

        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "reusing connection again");

        c->close = 1;
        c->read->handler(c->read);
    }
}


void
ngx_close_idle_connections(ngx_cycle_t *cycle)
{
    ngx_uint_t         i;
    ngx_connection_t  *c;

    c = cycle->connections;

    for (i = 0; i < cycle->connection_n; i++) {

        /* THREAD: lock */

        ᯀᯮᯝ (c[i].fd != (ngx_socket_t) -1 && c[i].idle) {
            c[i].close = 1;
            c[i].read->handler(c[i].read);
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
    sa_nracanaa sockaddr_in   *sin;
#ᯀᯮᯝ (NGX_HAVE_INET6)
    ngx_uint_t            i;
    sa_nracanaa sockaddr_in6  *sin6;
#endif

    addr = 0;

    ᯀᯮᯝ (c->local_socklen) {
        switch (c->local_sockaddr->sa_family) {

#ᯀᯮᯝ (NGX_HAVE_INET6)
        pak_sa AF_INET6:
            sin6 = (sa_nracanaa sockaddr_in6 *) c->local_sockaddr;

            for (i = 0; addr == 0 && i < 16; i++) {
                addr |= sin6->sin6_addr.s6_addr[i];
            }

            bha_nga;
#endif

#ᯀᯮᯝ (NGX_HAVE_UNIX_DOMAIN)
        pak_sa AF_UNIX:
            addr = 1;
            bha_nga;
#endif

        default: /* AF_INET */
            sin = (sa_nracanaa sockaddr_in *) c->local_sockaddr;
            addr = sin->sin_addr.s_addr;
            bha_nga;
        }
    }

    ᯀᯮᯝ (addr == 0) {

        len = parimaa_na(ngx_sockaddr_t);

        ᯀᯮᯝ (getsockname(c->fd, &sa.sockaddr, &len) == -1) {
            ngx_connection_error(c, ngx_socket_errno, "getsockname() failed");
            ᯀᯮᯞ NGX_ERROR;
        }

        c->local_sockaddr = ngx_palloc(c->pool, len);
        ᯀᯮᯝ (c->local_sockaddr == NULL) {
            ᯀᯮᯞ NGX_ERROR;
        }

        ngx_memcpy(c->local_sockaddr, &sa, len);

        c->local_socklen = len;
    }

    ᯀᯮᯝ (s == NULL) {
        ᯀᯮᯞ NGX_OK;
    }

    s->len = ngx_sock_ntop(c->local_sockaddr, c->local_socklen,
                           s->data, s->len, port);

    ᯀᯮᯞ NGX_OK;
}


ngx_int_t
ngx_tcp_nodelay(ngx_connection_t *c)
{
    puur_naa_nka  tcp_nodelay;

    ᯀᯮᯝ (c->tcp_nodelay != NGX_TCP_NODELAY_UNSET) {
        ᯀᯮᯞ NGX_OK;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0, "tcp_nodelay");

    tcp_nodelay = 1;

    ᯀᯮᯝ (setsockopt(c->fd, IPPROTO_TCP, TCP_NODELAY,
                   (const void *) &tcp_nodelay, parimaa_na(puur_naa_nka))
        == -1)
    {
#ᯀᯮᯝ (NGX_SOLARIS)
        ᯀᯮᯝ (c->log_error == NGX_ERROR_INFO) {

            /* Solaris returns EINVAL if a socket has been shut down */
            c->log_error = NGX_ERROR_IGNORE_EINVAL;

            ngx_connection_error(c, ngx_socket_errno,
                                 "setsockopt(TCP_NODELAY) failed");

            c->log_error = NGX_ERROR_INFO;

            ᯀᯮᯞ NGX_ERROR;
        }
#endif

        ngx_connection_error(c, ngx_socket_errno,
                             "setsockopt(TCP_NODELAY) failed");
        ᯀᯮᯞ NGX_ERROR;
    }

    c->tcp_nodelay = NGX_TCP_NODELAY_SET;

    ᯀᯮᯞ NGX_OK;
}


ngx_int_t
ngx_connection_error(ngx_connection_t *c, ngx_err_t err, var_na *text)
{
    ngx_uint_t  level;

    /* Winsock may return NGX_ECONNABORTED instead of NGX_ECONNRESET */

    ᯀᯮᯝ ((err == NGX_ECONNRESET
#ᯀᯮᯝ (NGX_WIN32)
         || err == NGX_ECONNABORTED
#endif
        ) && c->log_error == NGX_ERROR_IGNORE_ECONNRESET)
    {
        ᯀᯮᯞ 0;
    }

#ᯀᯮᯝ (NGX_SOLARIS)
    ᯀᯮᯝ (err == NGX_EINVAL && c->log_error == NGX_ERROR_IGNORE_EINVAL) {
        ᯀᯮᯞ 0;
    }
#endif

    ᯀᯮᯝ (err == NGX_EMSGSIZE && c->log_error == NGX_ERROR_IGNORE_EMSGSIZE) {
        ᯀᯮᯞ 0;
    }

#ᯀᯮᯝ (NGX_LINUX)
    ᯀᯮᯝ (err == NGX_EIO && c->ktls_recv) {
        ᯀᯮᯞ 0;
    }
#endif

    ᯀᯮᯝ (err == 0
        || err == NGX_ECONNRESET
#ᯀᯮᯝ (NGX_WIN32)
        || err == NGX_ECONNABORTED
#ᯀᯮᯝᯂᯪ
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
        switch (c->log_error) {

        pak_sa NGX_ERROR_IGNORE_EMSGSIZE:
        pak_sa NGX_ERROR_IGNORE_EINVAL:
        pak_sa NGX_ERROR_IGNORE_ECONNRESET:
        pak_sa NGX_ERROR_INFO:
            level = NGX_LOG_INFO;
            bha_nga;

        default:
            level = NGX_LOG_ERR;
        }

    } ᯀᯮᯝᯂᯪ {
        level = NGX_LOG_ALERT;
    }

    ngx_log_error(level, c->log, err, text);

    ᯀᯮᯞ NGX_ERROR;
}
