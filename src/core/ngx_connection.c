
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>
#अन्तर्भाव <ngx_event.h>


ngx_os_io_t  ngx_io;


स्थिर शून्य ngx_drain_connections(ngx_cycle_t *cycle);


ngx_listening_t *
ngx_create_listening(ngx_conf_t *cf, संरचना sockaddr *sockaddr,
    socklen_t socklen)
{
    size_t            len;
    ngx_listening_t  *ls;
    संरचना sockaddr  *sa;
    u_char            text[NGX_SOCKADDR_STRLEN];

    ls = ngx_array_ढकला(&cf->cycle->listening);
    यदि (ls == रिक्त) {
        प्रत्यागम रिक्त;
    }

    ngx_memzero(ls, परिमाण(ngx_listening_t));

    sa = ngx_palloc(cf->pool, socklen);
    यदि (sa == रिक्त) {
        प्रत्यागम रिक्त;
    }

    ngx_memcpy(sa, sockaddr, socklen);

    ls->sockaddr = sa;
    ls->socklen = socklen;

    len = ngx_sock_ntop(sa, socklen, text, NGX_SOCKADDR_STRLEN, 1);
    ls->addr_text.len = len;

    विकल्प (ls->sockaddr->sa_family) {
#यदि (NGX_HAVE_INET6)
    पक्ष AF_INET6:
        ls->addr_text_max_len = NGX_INET6_ADDRSTRLEN;
        भङ्ग;
#endif
#यदि (NGX_HAVE_UNIX_DOMAIN)
    पक्ष AF_UNIX:
        ls->addr_text_max_len = NGX_UNIX_ADDRSTRLEN;
        len++;
        भङ्ग;
#endif
    पक्ष AF_INET:
        ls->addr_text_max_len = NGX_INET_ADDRSTRLEN;
        भङ्ग;
    स्वभाव:
        ls->addr_text_max_len = NGX_SOCKADDR_STRLEN;
        भङ्ग;
    }

    ls->addr_text.data = ngx_pnalloc(cf->pool, len);
    यदि (ls->addr_text.data == रिक्त) {
        प्रत्यागम रिक्त;
    }

    ngx_memcpy(ls->addr_text.data, text, len);

#यदि !(NGX_WIN32)
    ngx_rbtree_init(&ls->rbtree, &ls->sentinel, ngx_udp_rbtree_घाला_मूल्य);
#endif

    ls->fd = (ngx_socket_t) -1;
    ls->type = SOCK_STREAM;

    ls->backlog = NGX_LISTEN_BACKLOG;
    ls->rcvbuf = -1;
    ls->sndbuf = -1;

#यदि (NGX_HAVE_SETFIB)
    ls->setfib = -1;
#endif

#यदि (NGX_HAVE_TCP_FASTOPEN)
    ls->fastopen = -1;
#endif

    प्रत्यागम ls;
}


ngx_int_t
ngx_clone_listening(ngx_cycle_t *cycle, ngx_listening_t *ls)
{
#यदि (NGX_HAVE_REUSEPORT)

    ngx_int_t         n;
    ngx_core_conf_t  *ccf;
    ngx_listening_t   ols;

    यदि (!ls->reuseport || ls->worker != 0) {
        प्रत्यागम NGX_OK;
    }

    ols = *ls;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    प्रति (n = 1; n < ccf->worker_processes; n++) {

        /* create a socket for each worker process */

        ls = ngx_array_ढकला(&cycle->listening);
        यदि (ls == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        *ls = ols;
        ls->worker = n;
    }

#endif

    प्रत्यागम NGX_OK;
}


ngx_int_t
ngx_set_inherited_sockets(ngx_cycle_t *cycle)
{
    size_t                     len;
    ngx_uint_t                 i;
    ngx_listening_t           *ls;
    socklen_t                  olen;
#यदि (NGX_HAVE_DEFERRED_ACCEPT || NGX_HAVE_TCP_FASTOPEN)
    ngx_err_t                  err;
#endif
#यदि (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    संरचना accept_filter_arg   af;
#endif
#यदि (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)
    पूर्णाङ्क                        timeout;
#endif
#यदि (NGX_HAVE_REUSEPORT)
    पूर्णाङ्क                        reuseport;
#endif

    ls = cycle->listening.elts;
    प्रति (i = 0; i < cycle->listening.nelts; i++) {

        ls[i].sockaddr = ngx_palloc(cycle->pool, परिमाण(ngx_sockaddr_t));
        यदि (ls[i].sockaddr == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        ls[i].socklen = परिमाण(ngx_sockaddr_t);
        यदि (getsockname(ls[i].fd, ls[i].sockaddr, &ls[i].socklen) == -1) {
            ngx_log_error(NGX_LOG_CRIT, cycle->log, ngx_socket_errno,
                          "getsockname() of the inherited "
                          "socket #%d failed", ls[i].fd);
            ls[i].ignore = 1;
            अनुवर्त;
        }

        यदि (ls[i].socklen > (socklen_t) परिमाण(ngx_sockaddr_t)) {
            ls[i].socklen = परिमाण(ngx_sockaddr_t);
        }

        विकल्प (ls[i].sockaddr->sa_family) {

#यदि (NGX_HAVE_INET6)
        पक्ष AF_INET6:
            ls[i].addr_text_max_len = NGX_INET6_ADDRSTRLEN;
            len = NGX_INET6_ADDRSTRLEN + परिमाण("[]:65535") - 1;
            भङ्ग;
#endif

#यदि (NGX_HAVE_UNIX_DOMAIN)
        पक्ष AF_UNIX:
            ls[i].addr_text_max_len = NGX_UNIX_ADDRSTRLEN;
            len = NGX_UNIX_ADDRSTRLEN;
            भङ्ग;
#endif

        पक्ष AF_INET:
            ls[i].addr_text_max_len = NGX_INET_ADDRSTRLEN;
            len = NGX_INET_ADDRSTRLEN + परिमाण(":65535") - 1;
            भङ्ग;

        स्वभाव:
            ngx_log_error(NGX_LOG_CRIT, cycle->log, ngx_socket_errno,
                          "the inherited socket #%d has "
                          "an unsupported protocol family", ls[i].fd);
            ls[i].ignore = 1;
            अनुवर्त;
        }

        ls[i].addr_text.data = ngx_pnalloc(cycle->pool, len);
        यदि (ls[i].addr_text.data == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        len = ngx_sock_ntop(ls[i].sockaddr, ls[i].socklen,
                            ls[i].addr_text.data, len, 1);
        यदि (len == 0) {
            प्रत्यागम NGX_ERROR;
        }

        ls[i].addr_text.len = len;

        ls[i].backlog = NGX_LISTEN_BACKLOG;

        olen = परिमाण(पूर्णाङ्क);

        यदि (getsockopt(ls[i].fd, SOL_SOCKET, SO_TYPE, (शून्य *) &ls[i].type,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_CRIT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_TYPE) %V failed", &ls[i].addr_text);
            ls[i].ignore = 1;
            अनुवर्त;
        }

        olen = परिमाण(पूर्णाङ्क);

        यदि (getsockopt(ls[i].fd, SOL_SOCKET, SO_RCVBUF, (शून्य *) &ls[i].rcvbuf,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_RCVBUF) %V failed, ignored",
                          &ls[i].addr_text);

            ls[i].rcvbuf = -1;
        }

        olen = परिमाण(पूर्णाङ्क);

        यदि (getsockopt(ls[i].fd, SOL_SOCKET, SO_SNDBUF, (शून्य *) &ls[i].sndbuf,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_SNDBUF) %V failed, ignored",
                          &ls[i].addr_text);

            ls[i].sndbuf = -1;
        }

#यदि 0
        /* SO_SETFIB is currently a set only option */

#यदि (NGX_HAVE_SETFIB)

        olen = परिमाण(पूर्णाङ्क);

        यदि (getsockopt(ls[i].fd, SOL_SOCKET, SO_SETFIB,
                       (शून्य *) &ls[i].setfib, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_SETFIB) %V failed, ignored",
                          &ls[i].addr_text);

            ls[i].setfib = -1;
        }

#endif
#endif

#यदि (NGX_HAVE_REUSEPORT)

        reuseport = 0;
        olen = परिमाण(पूर्णाङ्क);

#ifdef SO_REUSEPORT_LB

        यदि (getsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT_LB,
                       (शून्य *) &reuseport, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_REUSEPORT_LB) %V failed, ignored",
                          &ls[i].addr_text);

        } अन्यथा {
            ls[i].reuseport = reuseport ? 1 : 0;
        }

#अन्यथा

        यदि (getsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT,
                       (शून्य *) &reuseport, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_REUSEPORT) %V failed, ignored",
                          &ls[i].addr_text);

        } अन्यथा {
            ls[i].reuseport = reuseport ? 1 : 0;
        }
#endif

#endif

        यदि (ls[i].type != SOCK_STREAM) {
            अनुवर्त;
        }

#ifdef SO_PROTOCOL

        olen = परिमाण(पूर्णाङ्क);

        यदि (getsockopt(ls[i].fd, SOL_SOCKET, SO_PROTOCOL,
                       (शून्य *) &ls[i].protocol, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_PROTOCOL) %V failed, ignored",
                          &ls[i].addr_text);
            ls[i].protocol = 0;

        } अन्यथा यदि (ls[i].protocol == IPPROTO_TCP) {
            ls[i].protocol = 0;
        }

#endif

#यदि (NGX_HAVE_TCP_FASTOPEN)

        olen = परिमाण(पूर्णाङ्क);

        यदि (getsockopt(ls[i].fd, IPPROTO_TCP, TCP_FASTOPEN,
                       (शून्य *) &ls[i].fastopen, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            यदि (err != NGX_EOPNOTSUPP && err != NGX_ENOPROTOOPT
                && err != NGX_EINVAL)
            {
                ngx_log_error(NGX_LOG_NOTICE, cycle->log, err,
                              "getsockopt(TCP_FASTOPEN) %V failed, ignored",
                              &ls[i].addr_text);
            }

            ls[i].fastopen = -1;
        }

#endif

#यदि (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)

        ngx_memzero(&af, परिमाण(संरचना accept_filter_arg));
        olen = परिमाण(संरचना accept_filter_arg);

        यदि (getsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER, &af, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            यदि (err == NGX_EINVAL) {
                अनुवर्त;
            }

            ngx_log_error(NGX_LOG_NOTICE, cycle->log, err,
                          "getsockopt(SO_ACCEPTFILTER) for %V failed, ignored",
                          &ls[i].addr_text);
            अनुवर्त;
        }

        यदि (olen < परिमाण(संरचना accept_filter_arg) || af.af_name[0] == '\0') {
            अनुवर्त;
        }

        ls[i].accept_filter = ngx_palloc(cycle->pool, 16);
        यदि (ls[i].accept_filter == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        (शून्य) ngx_cpystrn((u_char *) ls[i].accept_filter,
                           (u_char *) af.af_name, 16);
#endif

#यदि (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)

        timeout = 0;
        olen = परिमाण(पूर्णाङ्क);

        यदि (getsockopt(ls[i].fd, IPPROTO_TCP, TCP_DEFER_ACCEPT, &timeout, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            यदि (err == NGX_EOPNOTSUPP) {
                अनुवर्त;
            }

            ngx_log_error(NGX_LOG_NOTICE, cycle->log, err,
                          "getsockopt(TCP_DEFER_ACCEPT) for %V failed, ignored",
                          &ls[i].addr_text);
            अनुवर्त;
        }

        यदि (olen < परिमाण(पूर्णाङ्क) || timeout == 0) {
            अनुवर्त;
        }

        ls[i].deferred_accept = 1;
#endif
    }

    प्रत्यागम NGX_OK;
}


ngx_int_t
ngx_open_listening_sockets(ngx_cycle_t *cycle)
{
    पूर्णाङ्क               reuseaddr;
    ngx_uint_t        i, tries, failed;
    ngx_err_t         err;
    ngx_log_t        *log;
    ngx_socket_t      s;
    ngx_listening_t  *ls;

    reuseaddr = 1;
#यदि (NGX_SUPPRESS_WARN)
    failed = 0;
#endif

    log = cycle->log;

    /* TODO: configurable try number */

    प्रति (tries = 5; tries; tries--) {
        failed = 0;

        /* for each listening socket */

        ls = cycle->listening.elts;
        प्रति (i = 0; i < cycle->listening.nelts; i++) {

            यदि (ls[i].ignore) {
                अनुवर्त;
            }

#यदि (NGX_HAVE_REUSEPORT)

            यदि (ls[i].add_reuseport || ls[i].change_protocol) {

                /*
                 * to allow transition from a socket without SO_REUSEPORT
                 * to multiple sockets with SO_REUSEPORT, we have to set
                 * SO_REUSEPORT on the old socket before opening new ones
                 *
                 * to allow transition between different socket protocols
                 * (e.g. IPPROTO_MPTCP), SO_REUSEPORT is set on both old
                 * and new sockets
                 */

                पूर्णाङ्क  reuseport = 1;

#ifdef SO_REUSEPORT_LB

                यदि (setsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT_LB,
                               (ध्रुव शून्य *) &reuseport, परिमाण(पूर्णाङ्क))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT_LB) %V failed, "
                                  "ignored",
                                  &ls[i].addr_text);
                }

#अन्यथा

                यदि (setsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT,
                               (ध्रुव शून्य *) &reuseport, परिमाण(पूर्णाङ्क))
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

            यदि (ls[i].fd != (ngx_socket_t) -1 && !ls[i].change_protocol) {
                अनुवर्त;
            }

            यदि (ls[i].inherited) {

                /* TODO: close on exit */
                /* TODO: nonblocking */
                /* TODO: deferred accept */

                अनुवर्त;
            }

            s = ngx_socket(ls[i].sockaddr->sa_family, ls[i].type,
                           ls[i].protocol);

            यदि (s == (ngx_socket_t) -1) {
                ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                              ngx_socket_n " %V failed", &ls[i].addr_text);
                प्रत्यागम NGX_ERROR;
            }

            यदि (ls[i].type != SOCK_DGRAM || !ngx_test_config) {

                यदि (setsockopt(s, SOL_SOCKET, SO_REUSEADDR,
                               (ध्रुव शून्य *) &reuseaddr, परिमाण(पूर्णाङ्क))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEADDR) %V failed",
                                  &ls[i].addr_text);

                    यदि (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_text);
                    }

                    प्रत्यागम NGX_ERROR;
                }
            }

#यदि (NGX_HAVE_REUSEPORT)

            यदि ((ls[i].reuseport || ls[i].change_protocol)
                && !ngx_test_config)
            {
                पूर्णाङ्क  reuseport;

                reuseport = 1;

#ifdef SO_REUSEPORT_LB

                यदि (setsockopt(s, SOL_SOCKET, SO_REUSEPORT_LB,
                               (ध्रुव शून्य *) &reuseport, परिमाण(पूर्णाङ्क))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT_LB) %V failed",
                                  &ls[i].addr_text);

                    यदि (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_text);
                    }

                    प्रत्यागम NGX_ERROR;
                }

#अन्यथा

                यदि (setsockopt(s, SOL_SOCKET, SO_REUSEPORT,
                               (ध्रुव शून्य *) &reuseport, परिमाण(पूर्णाङ्क))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT) %V failed",
                                  &ls[i].addr_text);

                    यदि (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_text);
                    }

                    प्रत्यागम NGX_ERROR;
                }
#endif
            }
#endif

#यदि (NGX_HAVE_INET6 && defined IPV6_V6ONLY)

            यदि (ls[i].sockaddr->sa_family == AF_INET6) {
                पूर्णाङ्क  ipv6only;

                ipv6only = ls[i].ipv6only;

                यदि (setsockopt(s, IPPROTO_IPV6, IPV6_V6ONLY,
                               (ध्रुव शून्य *) &ipv6only, परिमाण(पूर्णाङ्क))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(IPV6_V6ONLY) %V failed, ignored",
                                  &ls[i].addr_text);
                }
            }
#endif
            /* TODO: close on exit */

            यदि (ngx_nonblocking(s) == -1) {
                ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                              ngx_nonblocking_n " %V failed",
                              &ls[i].addr_text);

                यदि (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_text);
                }

                प्रत्यागम NGX_ERROR;
            }

            ngx_log_debug2(NGX_LOG_DEBUG_CORE, log, 0,
                           "bind() %V #%d ", &ls[i].addr_text, s);

            यदि (bind(s, ls[i].sockaddr, ls[i].socklen) == -1) {
                err = ngx_socket_errno;

                यदि (err != NGX_EADDRINUSE || !ngx_test_config) {
                    ngx_log_error(NGX_LOG_EMERG, log, err,
                                  "bind() to %V failed", &ls[i].addr_text);
                }

                यदि (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_text);
                }

                यदि (err != NGX_EADDRINUSE) {
                    प्रत्यागम NGX_ERROR;
                }

                यदि (!ngx_test_config) {
                    failed = 1;
                }

                अनुवर्त;
            }

#यदि (NGX_HAVE_UNIX_DOMAIN)

            यदि (ls[i].sockaddr->sa_family == AF_UNIX) {
                mode_t   mode;
                u_char  *name;

                name = ls[i].addr_text.data + परिमाण("unix:") - 1;
                mode = (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);

                यदि (chmod((वर्ण *) name, mode) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  "chmod() \"%s\" failed", name);
                }

                यदि (ngx_test_config) {
                    यदि (ngx_delete_file(name) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                      ngx_delete_file_n " %s failed", name);
                    }
                }
            }
#endif

            यदि (ls[i].type != SOCK_STREAM) {
                ls[i].fd = s;
                ls[i].open = 1;
                अनुवर्त;
            }

            यदि (listen(s, ls[i].backlog) == -1) {
                err = ngx_socket_errno;

                /*
                 * on OpenVZ after suspend/resume EADDRINUSE
                 * may be returned by listen() instead of bind(), see
                 * https://bugs.openvz.org/browse/OVZ-5587
                 */

                यदि (err != NGX_EADDRINUSE || !ngx_test_config) {
                    ngx_log_error(NGX_LOG_EMERG, log, err,
                                  "listen() to %V, backlog %d failed",
                                  &ls[i].addr_text, ls[i].backlog);
                }

                यदि (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_text);
                }

                यदि (err != NGX_EADDRINUSE) {
                    प्रत्यागम NGX_ERROR;
                }

                यदि (!ngx_test_config) {
                    failed = 1;
                }

                अनुवर्त;
            }

            ls[i].listen = 1;

            ls[i].fd = s;
            ls[i].open = 1;
        }

        यदि (!failed) {
            भङ्ग;
        }

        /* TODO: delay configurable */

        ngx_log_error(NGX_LOG_NOTICE, log, 0,
                      "try again to bind() after 500ms");

        ngx_msleep(500);
    }

    यदि (failed) {
        ngx_log_error(NGX_LOG_EMERG, log, 0, "still could not bind()");
        प्रत्यागम NGX_ERROR;
    }

    प्रत्यागम NGX_OK;
}


शून्य
ngx_configure_listening_sockets(ngx_cycle_t *cycle)
{
    पूर्णाङ्क                        मूल्य;
    ngx_uint_t                 i;
    ngx_listening_t           *ls;

#यदि (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    संरचना accept_filter_arg   af;
#endif

    ls = cycle->listening.elts;
    प्रति (i = 0; i < cycle->listening.nelts; i++) {

        ls[i].log = *ls[i].logp;

        यदि (ls[i].rcvbuf != -1) {
            यदि (setsockopt(ls[i].fd, SOL_SOCKET, SO_RCVBUF,
                           (ध्रुव शून्य *) &ls[i].rcvbuf, परिमाण(पूर्णाङ्क))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_RCVBUF, %d) %V failed, ignored",
                              ls[i].rcvbuf, &ls[i].addr_text);
            }
        }

        यदि (ls[i].sndbuf != -1) {
            यदि (setsockopt(ls[i].fd, SOL_SOCKET, SO_SNDBUF,
                           (ध्रुव शून्य *) &ls[i].sndbuf, परिमाण(पूर्णाङ्क))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_SNDBUF, %d) %V failed, ignored",
                              ls[i].sndbuf, &ls[i].addr_text);
            }
        }

        यदि (ls[i].keepalive) {
            मूल्य = (ls[i].keepalive == 1) ? 1 : 0;

            यदि (setsockopt(ls[i].fd, SOL_SOCKET, SO_KEEPALIVE,
                           (ध्रुव शून्य *) &मूल्य, परिमाण(पूर्णाङ्क))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_KEEPALIVE, %d) %V failed, ignored",
                              मूल्य, &ls[i].addr_text);
            }
        }

#यदि (NGX_HAVE_KEEPALIVE_TUNABLE)

#यदि !(NGX_DARWIN)

        यदि (ls[i].keepidle) {
            मूल्य = ls[i].keepidle;

#यदि (NGX_KEEPALIVE_FACTOR)
            मूल्य *= NGX_KEEPALIVE_FACTOR;
#endif

            यदि (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPIDLE,
                           (ध्रुव शून्य *) &मूल्य, परिमाण(पूर्णाङ्क))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_KEEPIDLE, %d) %V failed, ignored",
                              मूल्य, &ls[i].addr_text);
            }
        }

#endif

        यदि (ls[i].keepintvl) {
            मूल्य = ls[i].keepintvl;

#यदि (NGX_KEEPALIVE_FACTOR)
            मूल्य *= NGX_KEEPALIVE_FACTOR;
#endif

            यदि (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPINTVL,
                           (ध्रुव शून्य *) &मूल्य, परिमाण(पूर्णाङ्क))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                             "setsockopt(TCP_KEEPINTVL, %d) %V failed, ignored",
                             मूल्य, &ls[i].addr_text);
            }
        }

        यदि (ls[i].keepcnt) {
            यदि (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPCNT,
                           (ध्रुव शून्य *) &ls[i].keepcnt, परिमाण(पूर्णाङ्क))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_KEEPCNT, %d) %V failed, ignored",
                              ls[i].keepcnt, &ls[i].addr_text);
            }
        }

#endif

#यदि (NGX_HAVE_SETFIB)
        यदि (ls[i].setfib != -1) {
            यदि (setsockopt(ls[i].fd, SOL_SOCKET, SO_SETFIB,
                           (ध्रुव शून्य *) &ls[i].setfib, परिमाण(पूर्णाङ्क))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_SETFIB, %d) %V failed, ignored",
                              ls[i].setfib, &ls[i].addr_text);
            }
        }
#endif

#यदि (NGX_HAVE_TCP_FASTOPEN)
        यदि (ls[i].fastopen != -1) {
            यदि (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_FASTOPEN,
                           (ध्रुव शून्य *) &ls[i].fastopen, परिमाण(पूर्णाङ्क))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_FASTOPEN, %d) %V failed, ignored",
                              ls[i].fastopen, &ls[i].addr_text);
            }
        }
#endif

#यदि 0
        यदि (1) {
            पूर्णाङ्क tcp_nodelay = 1;

            यदि (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_NODELAY,
                       (ध्रुव शून्य *) &tcp_nodelay, परिमाण(पूर्णाङ्क))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_NODELAY) %V failed, ignored",
                              &ls[i].addr_text);
            }
        }
#endif

        यदि (ls[i].listen) {

            /* change backlog via listen() */

            यदि (listen(ls[i].fd, ls[i].backlog) == -1) {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "listen() to %V, backlog %d failed, ignored",
                              &ls[i].addr_text, ls[i].backlog);
            }
        }

        /*
         * setting deferred mode should be last operation on socket,
         * because code may prematurely continue cycle on failure
         */

#यदि (NGX_HAVE_DEFERRED_ACCEPT)

#ifdef SO_ACCEPTFILTER

        यदि (ls[i].delete_deferred) {
            यदि (setsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER, रिक्त, 0)
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_ACCEPTFILTER, NULL) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);

                यदि (ls[i].accept_filter) {
                    ngx_log_error(NGX_LOG_ALERT, cycle->log, 0,
                                  "could not change the accept filter "
                                  "to \"%s\" for %V, ignored",
                                  ls[i].accept_filter, &ls[i].addr_text);
                }

                अनुवर्त;
            }

            ls[i].deferred_accept = 0;
        }

        यदि (ls[i].add_deferred) {
            ngx_memzero(&af, परिमाण(संरचना accept_filter_arg));
            (शून्य) ngx_cpystrn((u_char *) af.af_name,
                               (u_char *) ls[i].accept_filter, 16);

            यदि (setsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER,
                           &af, परिमाण(संरचना accept_filter_arg))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_ACCEPTFILTER, \"%s\") "
                              "for %V failed, ignored",
                              ls[i].accept_filter, &ls[i].addr_text);
                अनुवर्त;
            }

            ls[i].deferred_accept = 1;
        }

#endif

#ifdef TCP_DEFER_ACCEPT

        यदि (ls[i].add_deferred || ls[i].delete_deferred) {

            यदि (ls[i].add_deferred) {
                /*
                 * There is no way to find out how long a connection was
                 * in queue (and a connection may bypass deferred queue at all
                 * if syncookies were used), hence we use 1 second timeout
                 * here.
                 */
                मूल्य = 1;

            } अन्यथा {
                मूल्य = 0;
            }

            यदि (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_DEFER_ACCEPT,
                           &मूल्य, परिमाण(पूर्णाङ्क))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_DEFER_ACCEPT, %d) for %V failed, "
                              "ignored",
                              मूल्य, &ls[i].addr_text);

                अनुवर्त;
            }
        }

        यदि (ls[i].add_deferred) {
            ls[i].deferred_accept = 1;
        }

#endif

#endif /* NGX_HAVE_DEFERRED_ACCEPT */

#यदि (NGX_HAVE_IP_RECVDSTADDR)

        यदि (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET)
        {
            मूल्य = 1;

            यदि (setsockopt(ls[i].fd, IPPROTO_IP, IP_RECVDSTADDR,
                           (ध्रुव शून्य *) &मूल्य, परिमाण(पूर्णाङ्क))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_RECVDSTADDR) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#नोचेद्यदि (NGX_HAVE_IP_PKTINFO)

        यदि (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET)
        {
            मूल्य = 1;

            यदि (setsockopt(ls[i].fd, IPPROTO_IP, IP_PKTINFO,
                           (ध्रुव शून्य *) &मूल्य, परिमाण(पूर्णाङ्क))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_PKTINFO) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#endif

#यदि (NGX_HAVE_INET6 && NGX_HAVE_IPV6_RECVPKTINFO)

        यदि (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET6)
        {
            मूल्य = 1;

            यदि (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_RECVPKTINFO,
                           (ध्रुव शून्य *) &मूल्य, परिमाण(पूर्णाङ्क))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IPV6_RECVPKTINFO) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#endif

#यदि (NGX_HAVE_IP_MTU_DISCOVER)

        यदि (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET) {
            मूल्य = IP_PMTUDISC_DO;

            यदि (setsockopt(ls[i].fd, IPPROTO_IP, IP_MTU_DISCOVER,
                           (ध्रुव शून्य *) &मूल्य, परिमाण(पूर्णाङ्क))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_MTU_DISCOVER) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#नोचेद्यदि (NGX_HAVE_IP_DONTFRAG)

        यदि (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET) {
            मूल्य = 1;

            यदि (setsockopt(ls[i].fd, IPPROTO_IP, IP_DONTFRAG,
                           (ध्रुव शून्य *) &मूल्य, परिमाण(पूर्णाङ्क))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_DONTFRAG) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#endif

#यदि (NGX_HAVE_INET6)

#यदि (NGX_HAVE_IPV6_MTU_DISCOVER)

        यदि (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET6) {
            मूल्य = IPV6_PMTUDISC_DO;

            यदि (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_MTU_DISCOVER,
                           (ध्रुव शून्य *) &मूल्य, परिमाण(पूर्णाङ्क))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IPV6_MTU_DISCOVER) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#नोचेद्यदि (NGX_HAVE_IPV6_DONTFRAG)

        यदि (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET6) {
            मूल्य = 1;

            यदि (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_DONTFRAG,
                           (ध्रुव शून्य *) &मूल्य, परिमाण(पूर्णाङ्क))
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

    प्रत्यागम;
}


शून्य
ngx_close_listening_sockets(ngx_cycle_t *cycle)
{
    ngx_uint_t         i;
    ngx_listening_t   *ls;
    ngx_connection_t  *c;

    ngx_accept_mutex_held = 0;
    ngx_use_accept_mutex = 0;

    ls = cycle->listening.elts;
    प्रति (i = 0; i < cycle->listening.nelts; i++) {

#यदि (NGX_QUIC)
        यदि (ls[i].quic
            && (!ls[i].reuseport || !ngx_quic_bpf_enabled(cycle)))
        {
            अनुवर्त;
        }
#endif

        c = ls[i].connection;

        यदि (c) {
            यदि (c->पठ->active) {
                यदि (ngx_event_flags & NGX_USE_EPOLL_EVENT) {

                    /*
                     * it seems that Linux-2.6.x OpenVZ sends events
                     * for closed shared listening sockets unless
                     * the events was explicitly deleted
                     */

                    ngx_del_event(c->पठ, NGX_READ_EVENT, 0);

                } अन्यथा {
                    ngx_del_event(c->पठ, NGX_READ_EVENT, NGX_CLOSE_EVENT);
                }
            }

            यदि (c->पठ->timer_set) {
                ngx_del_timer(c->पठ);
            }

            ngx_free_connection(c);

            c->fd = (ngx_socket_t) -1;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, cycle->log, 0,
                       "close listening %V #%d ", &ls[i].addr_text, ls[i].fd);

        यदि (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_socket_errno,
                          ngx_close_socket_n " %V failed", &ls[i].addr_text);
        }

#यदि (NGX_HAVE_UNIX_DOMAIN)

        यदि (ls[i].sockaddr->sa_family == AF_UNIX
            && ngx_process <= NGX_PROCESS_MASTER
            && ngx_new_द्विमान == 0
            && (!ls[i].inherited || ngx_getppid() != ngx_parent))
        {
            u_char *name = ls[i].addr_text.data + परिमाण("unix:") - 1;

            यदि (ngx_delete_file(name) == NGX_FILE_ERROR) {
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

    यदि (ngx_cycle->files && (ngx_uint_t) s >= ngx_cycle->files_n) {
        ngx_log_error(NGX_LOG_ALERT, log, 0,
                      "the new socket has number %d, "
                      "but only %ui files are available",
                      s, ngx_cycle->files_n);
        प्रत्यागम रिक्त;
    }

    ngx_drain_connections((ngx_cycle_t *) ngx_cycle);

    c = ngx_cycle->free_connections;

    यदि (c == रिक्त) {
        ngx_log_error(NGX_LOG_ALERT, log, 0,
                      "%ui worker_connections are not enough",
                      ngx_cycle->connection_n);

        प्रत्यागम रिक्त;
    }

    ngx_cycle->free_connections = c->data;
    ngx_cycle->free_connection_n--;

    यदि (ngx_cycle->files && ngx_cycle->files[s] == रिक्त) {
        ngx_cycle->files[s] = c;
    }

    rev = c->पठ;
    wev = c->लिख;

    ngx_memzero(c, परिमाण(ngx_connection_t));

    c->पठ = rev;
    c->लिख = wev;
    c->fd = s;
    c->log = log;

    instance = rev->instance;

    ngx_memzero(rev, परिमाण(ngx_event_t));
    ngx_memzero(wev, परिमाण(ngx_event_t));

    rev->instance = !instance;
    wev->instance = !instance;

    rev->index = NGX_INVALID_INDEX;
    wev->index = NGX_INVALID_INDEX;

    rev->data = c;
    wev->data = c;

    wev->लिख = 1;

    प्रत्यागम c;
}


शून्य
ngx_free_connection(ngx_connection_t *c)
{
    c->data = ngx_cycle->free_connections;
    ngx_cycle->free_connections = c;
    ngx_cycle->free_connection_n++;

    यदि (ngx_cycle->files && ngx_cycle->files[c->fd] == c) {
        ngx_cycle->files[c->fd] = रिक्त;
    }
}


शून्य
ngx_close_connection(ngx_connection_t *c)
{
    ngx_err_t     err;
    ngx_uint_t    log_error, level;
    ngx_socket_t  fd;

    यदि (c->fd == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0, "connection already closed");
        प्रत्यागम;
    }

    यदि (c->पठ->timer_set) {
        ngx_del_timer(c->पठ);
    }

    यदि (c->लिख->timer_set) {
        ngx_del_timer(c->लिख);
    }

    यदि (!c->shared) {
        यदि (ngx_del_conn) {
            ngx_del_conn(c, NGX_CLOSE_EVENT);

        } अन्यथा {
            यदि (c->पठ->active || c->पठ->disabled) {
                ngx_del_event(c->पठ, NGX_READ_EVENT, NGX_CLOSE_EVENT);
            }

            यदि (c->लिख->active || c->लिख->disabled) {
                ngx_del_event(c->लिख, NGX_WRITE_EVENT, NGX_CLOSE_EVENT);
            }
        }
    }

    यदि (c->पठ->posted) {
        ngx_delete_posted_event(c->पठ);
    }

    यदि (c->लिख->posted) {
        ngx_delete_posted_event(c->लिख);
    }

    c->पठ->closed = 1;
    c->लिख->closed = 1;

    ngx_reusable_connection(c, 0);

    log_error = c->log_error;

    ngx_free_connection(c);

    fd = c->fd;
    c->fd = (ngx_socket_t) -1;

    यदि (c->shared) {
        प्रत्यागम;
    }

    यदि (ngx_close_socket(fd) == -1) {

        err = ngx_socket_errno;

        यदि (err == NGX_ECONNRESET || err == NGX_ENOTCONN) {

            विकल्प (log_error) {

            पक्ष NGX_ERROR_INFO:
                level = NGX_LOG_INFO;
                भङ्ग;

            पक्ष NGX_ERROR_ERR:
                level = NGX_LOG_ERR;
                भङ्ग;

            स्वभाव:
                level = NGX_LOG_CRIT;
            }

        } अन्यथा {
            level = NGX_LOG_CRIT;
        }

        ngx_log_error(level, c->log, err, ngx_close_socket_n " %d failed", fd);
    }
}


शून्य
ngx_reusable_connection(ngx_connection_t *c, ngx_uint_t reusable)
{
    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "reusable connection: %ui", reusable);

    यदि (c->reusable) {
        ngx_queue_remove(&c->queue);
        ngx_cycle->reusable_connections_n--;

#यदि (NGX_STAT_STUB)
        (शून्य) ngx_atomic_fetch_add(ngx_stat_waiting, -1);
#endif
    }

    c->reusable = reusable;

    यदि (reusable) {
        /* need cast as ngx_cycle is volatile */

        ngx_queue_घाला_head(
            (ngx_queue_t *) &ngx_cycle->reusable_connections_queue, &c->queue);
        ngx_cycle->reusable_connections_n++;

#यदि (NGX_STAT_STUB)
        (शून्य) ngx_atomic_fetch_add(ngx_stat_waiting, 1);
#endif
    }
}


स्थिर शून्य
ngx_drain_connections(ngx_cycle_t *cycle)
{
    ngx_uint_t         i, n;
    ngx_queue_t       *q;
    ngx_connection_t  *c;

    यदि (cycle->free_connection_n > cycle->connection_n / 16
        || cycle->reusable_connections_n == 0)
    {
        प्रत्यागम;
    }

    यदि (cycle->connections_reuse_time != ngx_time()) {
        cycle->connections_reuse_time = ngx_time();

        ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                      "%ui worker_connections are not enough, "
                      "reusing connections",
                      cycle->connection_n);
    }

    c = रिक्त;
    n = ngx_max(ngx_min(32, cycle->reusable_connections_n / 8), 1);

    प्रति (i = 0; i < n; i++) {
        यदि (ngx_queue_empty(&cycle->reusable_connections_queue)) {
            भङ्ग;
        }

        q = ngx_queue_last(&cycle->reusable_connections_queue);
        c = ngx_queue_data(q, ngx_connection_t, queue);

        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "reusing connection");

        c->close = 1;
        c->पठ->handler(c->पठ);
    }

    यदि (cycle->free_connection_n == 0 && c && c->reusable) {

        /*
         * if no connections were freed, try to reuse the last
         * connection again: this should free it as long as
         * previous reuse moved it to lingering close
         */

        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "reusing connection again");

        c->close = 1;
        c->पठ->handler(c->पठ);
    }
}


शून्य
ngx_close_idle_connections(ngx_cycle_t *cycle)
{
    ngx_uint_t         i;
    ngx_connection_t  *c;

    c = cycle->connections;

    प्रति (i = 0; i < cycle->connection_n; i++) {

        /* THREAD: lock */

        यदि (c[i].fd != (ngx_socket_t) -1 && c[i].idle) {
            c[i].close = 1;
            c[i].पठ->handler(c[i].पठ);
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
    संरचना sockaddr_in   *sin;
#यदि (NGX_HAVE_INET6)
    ngx_uint_t            i;
    संरचना sockaddr_in6  *sin6;
#endif

    addr = 0;

    यदि (c->local_socklen) {
        विकल्प (c->local_sockaddr->sa_family) {

#यदि (NGX_HAVE_INET6)
        पक्ष AF_INET6:
            sin6 = (संरचना sockaddr_in6 *) c->local_sockaddr;

            प्रति (i = 0; addr == 0 && i < 16; i++) {
                addr |= sin6->sin6_addr.s6_addr[i];
            }

            भङ्ग;
#endif

#यदि (NGX_HAVE_UNIX_DOMAIN)
        पक्ष AF_UNIX:
            addr = 1;
            भङ्ग;
#endif

        स्वभाव: /* AF_INET */
            sin = (संरचना sockaddr_in *) c->local_sockaddr;
            addr = sin->sin_addr.s_addr;
            भङ्ग;
        }
    }

    यदि (addr == 0) {

        len = परिमाण(ngx_sockaddr_t);

        यदि (getsockname(c->fd, &sa.sockaddr, &len) == -1) {
            ngx_connection_error(c, ngx_socket_errno, "getsockname() failed");
            प्रत्यागम NGX_ERROR;
        }

        c->local_sockaddr = ngx_palloc(c->pool, len);
        यदि (c->local_sockaddr == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        ngx_memcpy(c->local_sockaddr, &sa, len);

        c->local_socklen = len;
    }

    यदि (s == रिक्त) {
        प्रत्यागम NGX_OK;
    }

    s->len = ngx_sock_ntop(c->local_sockaddr, c->local_socklen,
                           s->data, s->len, port);

    प्रत्यागम NGX_OK;
}


ngx_int_t
ngx_tcp_nodelay(ngx_connection_t *c)
{
    पूर्णाङ्क  tcp_nodelay;

    यदि (c->tcp_nodelay != NGX_TCP_NODELAY_UNSET) {
        प्रत्यागम NGX_OK;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0, "tcp_nodelay");

    tcp_nodelay = 1;

    यदि (setsockopt(c->fd, IPPROTO_TCP, TCP_NODELAY,
                   (ध्रुव शून्य *) &tcp_nodelay, परिमाण(पूर्णाङ्क))
        == -1)
    {
#यदि (NGX_SOLARIS)
        यदि (c->log_error == NGX_ERROR_INFO) {

            /* Solaris returns EINVAL if a socket has been shut down */
            c->log_error = NGX_ERROR_IGNORE_EINVAL;

            ngx_connection_error(c, ngx_socket_errno,
                                 "setsockopt(TCP_NODELAY) failed");

            c->log_error = NGX_ERROR_INFO;

            प्रत्यागम NGX_ERROR;
        }
#endif

        ngx_connection_error(c, ngx_socket_errno,
                             "setsockopt(TCP_NODELAY) failed");
        प्रत्यागम NGX_ERROR;
    }

    c->tcp_nodelay = NGX_TCP_NODELAY_SET;

    प्रत्यागम NGX_OK;
}


ngx_int_t
ngx_connection_error(ngx_connection_t *c, ngx_err_t err, वर्ण *text)
{
    ngx_uint_t  level;

    /* Winsock may return NGX_ECONNABORTED instead of NGX_ECONNRESET */

    यदि ((err == NGX_ECONNRESET
#यदि (NGX_WIN32)
         || err == NGX_ECONNABORTED
#endif
        ) && c->log_error == NGX_ERROR_IGNORE_ECONNRESET)
    {
        प्रत्यागम 0;
    }

#यदि (NGX_SOLARIS)
    यदि (err == NGX_EINVAL && c->log_error == NGX_ERROR_IGNORE_EINVAL) {
        प्रत्यागम 0;
    }
#endif

    यदि (err == NGX_EMSGSIZE && c->log_error == NGX_ERROR_IGNORE_EMSGSIZE) {
        प्रत्यागम 0;
    }

#यदि (NGX_LINUX)
    यदि (err == NGX_EIO && c->ktls_recv) {
        प्रत्यागम 0;
    }
#endif

    यदि (err == 0
        || err == NGX_ECONNRESET
#यदि (NGX_WIN32)
        || err == NGX_ECONNABORTED
#अन्यथा
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
        विकल्प (c->log_error) {

        पक्ष NGX_ERROR_IGNORE_EMSGSIZE:
        पक्ष NGX_ERROR_IGNORE_EINVAL:
        पक्ष NGX_ERROR_IGNORE_ECONNRESET:
        पक्ष NGX_ERROR_INFO:
            level = NGX_LOG_INFO;
            भङ्ग;

        स्वभाव:
            level = NGX_LOG_ERR;
        }

    } अन्यथा {
        level = NGX_LOG_ALERT;
    }

    ngx_log_error(level, c->log, err, text);

    प्रत्यागम NGX_ERROR;
}
