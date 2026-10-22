
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>
#شامل <ngx_event.h>


ngx_os_io_t  ngx_io;


ساکن خالی ngx_drain_connections(ngx_چکر_t *چکر);


ngx_listening_t *
ngx_create_listening(ngx_conf_t *cf, ساخت sockaddr *sockaddr,
    socklen_t socklen)
{
    سائز_t            len;
    ngx_listening_t  *ls;
    ساخت sockaddr  *sa;
    u_char            متن[NGX_SOCKADDR_STRLEN];

    ls = ngx_array_ڈالو(&cf->چکر->listening);
    اگر (ls == تہی) {
        واپس تہی;
    }

    ngx_memzero(ls, حجم(ngx_listening_t));

    sa = ngx_palloc(cf->pool, socklen);
    اگر (sa == تہی) {
        واپس تہی;
    }

    ngx_memcpy(sa, sockaddr, socklen);

    ls->sockaddr = sa;
    ls->socklen = socklen;

    len = ngx_sock_ntop(sa, socklen, متن, NGX_SOCKADDR_STRLEN, 1);
    ls->addr_متن.len = len;

    انتخاب (ls->sockaddr->sa_family) {
#اگر (NGX_HAVE_INET6)
    حالت AF_INET6:
        ls->addr_متن_max_len = NGX_INET6_ADDRSTRLEN;
        توڑو;
#اگر_ختم
#اگر (NGX_HAVE_UNIX_DOMAIN)
    حالت AF_UNIX:
        ls->addr_متن_max_len = NGX_UNIX_ADDRSTRLEN;
        len++;
        توڑو;
#اگر_ختم
    حالت AF_INET:
        ls->addr_متن_max_len = NGX_INET_ADDRSTRLEN;
        توڑو;
    طےشدہ:
        ls->addr_متن_max_len = NGX_SOCKADDR_STRLEN;
        توڑو;
    }

    ls->addr_متن.ڈیٹا = ngx_pnalloc(cf->pool, len);
    اگر (ls->addr_متن.ڈیٹا == تہی) {
        واپس تہی;
    }

    ngx_memcpy(ls->addr_متن.ڈیٹا, متن, len);

#اگر !(NGX_WIN32)
    ngx_rbtree_init(&ls->rbtree, &ls->sentinel, ngx_udp_rbtree_داخل_کرو_قدر);
#اگر_ختم

    ls->fd = (ngx_socket_t) -1;
    ls->type = SOCK_دھارا;

    ls->backlog = NGX_LISTEN_BACKLOG;
    ls->rcvbuf = -1;
    ls->sndbuf = -1;

#اگر (NGX_HAVE_SETFIB)
    ls->setfib = -1;
#اگر_ختم

#اگر (NGX_HAVE_TCP_FASTOPEN)
    ls->fastopen = -1;
#اگر_ختم

    واپس ls;
}


ngx_int_t
ngx_clone_listening(ngx_چکر_t *چکر, ngx_listening_t *ls)
{
#اگر (NGX_HAVE_REUSEPORT)

    ngx_int_t         n;
    ngx_core_conf_t  *ccf;
    ngx_listening_t   ols;

    اگر (!ls->reuseport || ls->کارکن != 0) {
        واپس NGX_OK;
    }

    ols = *ls;

    ccf = (ngx_core_conf_t *) ngx_get_conf(چکر->conf_ctx, ngx_core_module);

    برائے (n = 1; n < ccf->کارکن_processes; n++) {

        /* create a socket for each worker process */

        ls = ngx_array_ڈالو(&چکر->listening);
        اگر (ls == تہی) {
            واپس NGX_ERROR;
        }

        *ls = ols;
        ls->کارکن = n;
    }

#اگر_ختم

    واپس NGX_OK;
}


ngx_int_t
ngx_سیٹ_inherited_sockets(ngx_چکر_t *چکر)
{
    سائز_t                     len;
    ngx_uint_t                 i;
    ngx_listening_t           *ls;
    socklen_t                  olen;
#اگر (NGX_HAVE_DEFERRED_ACCEPT || NGX_HAVE_TCP_FASTOPEN)
    ngx_err_t                  err;
#اگر_ختم
#اگر (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    ساخت accept_filter_arg   af;
#اگر_ختم
#اگر (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)
    صحیح                        timeout;
#اگر_ختم
#اگر (NGX_HAVE_REUSEPORT)
    صحیح                        reuseport;
#اگر_ختم

    ls = چکر->listening.elts;
    برائے (i = 0; i < چکر->listening.nelts; i++) {

        ls[i].sockaddr = ngx_palloc(چکر->pool, حجم(ngx_sockaddr_t));
        اگر (ls[i].sockaddr == تہی) {
            واپس NGX_ERROR;
        }

        ls[i].socklen = حجم(ngx_sockaddr_t);
        اگر (getsockname(ls[i].fd, ls[i].sockaddr, &ls[i].socklen) == -1) {
            ngx_log_error(NGX_LOG_CRIT, چکر->لوگارتھم, ngx_socket_errno,
                          "getsockname() of the inherited "
                          "socket #%d failed", ls[i].fd);
            ls[i].ignore = 1;
            جاری;
        }

        اگر (ls[i].socklen > (socklen_t) حجم(ngx_sockaddr_t)) {
            ls[i].socklen = حجم(ngx_sockaddr_t);
        }

        انتخاب (ls[i].sockaddr->sa_family) {

#اگر (NGX_HAVE_INET6)
        حالت AF_INET6:
            ls[i].addr_متن_max_len = NGX_INET6_ADDRSTRLEN;
            len = NGX_INET6_ADDRSTRLEN + حجم("[]:65535") - 1;
            توڑو;
#اگر_ختم

#اگر (NGX_HAVE_UNIX_DOMAIN)
        حالت AF_UNIX:
            ls[i].addr_متن_max_len = NGX_UNIX_ADDRSTRLEN;
            len = NGX_UNIX_ADDRSTRLEN;
            توڑو;
#اگر_ختم

        حالت AF_INET:
            ls[i].addr_متن_max_len = NGX_INET_ADDRSTRLEN;
            len = NGX_INET_ADDRSTRLEN + حجم(":65535") - 1;
            توڑو;

        طےشدہ:
            ngx_log_error(NGX_LOG_CRIT, چکر->لوگارتھم, ngx_socket_errno,
                          "the inherited socket #%d has "
                          "an unsupported protocol family", ls[i].fd);
            ls[i].ignore = 1;
            جاری;
        }

        ls[i].addr_متن.ڈیٹا = ngx_pnalloc(چکر->pool, len);
        اگر (ls[i].addr_متن.ڈیٹا == تہی) {
            واپس NGX_ERROR;
        }

        len = ngx_sock_ntop(ls[i].sockaddr, ls[i].socklen,
                            ls[i].addr_متن.ڈیٹا, len, 1);
        اگر (len == 0) {
            واپس NGX_ERROR;
        }

        ls[i].addr_متن.len = len;

        ls[i].backlog = NGX_LISTEN_BACKLOG;

        olen = حجم(صحیح);

        اگر (getsockopt(ls[i].fd, SOL_SOCKET, SO_TYPE, (خالی *) &ls[i].type,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_CRIT, چکر->لوگارتھم, ngx_socket_errno,
                          "getsockopt(SO_TYPE) %V failed", &ls[i].addr_متن);
            ls[i].ignore = 1;
            جاری;
        }

        olen = حجم(صحیح);

        اگر (getsockopt(ls[i].fd, SOL_SOCKET, SO_RCVBUF, (خالی *) &ls[i].rcvbuf,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_socket_errno,
                          "getsockopt(SO_RCVBUF) %V failed, ignored",
                          &ls[i].addr_متن);

            ls[i].rcvbuf = -1;
        }

        olen = حجم(صحیح);

        اگر (getsockopt(ls[i].fd, SOL_SOCKET, SO_SNDBUF, (خالی *) &ls[i].sndbuf,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_socket_errno,
                          "getsockopt(SO_SNDBUF) %V failed, ignored",
                          &ls[i].addr_متن);

            ls[i].sndbuf = -1;
        }

#اگر 0
        /* SO_SETFIB is currently a set only option */

#اگر (NGX_HAVE_SETFIB)

        olen = حجم(صحیح);

        اگر (getsockopt(ls[i].fd, SOL_SOCKET, SO_SETFIB,
                       (خالی *) &ls[i].setfib, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_socket_errno,
                          "getsockopt(SO_SETFIB) %V failed, ignored",
                          &ls[i].addr_متن);

            ls[i].setfib = -1;
        }

#اگر_ختم
#اگر_ختم

#اگر (NGX_HAVE_REUSEPORT)

        reuseport = 0;
        olen = حجم(صحیح);

#اگر_تعریف_شدہ SO_REUSEPORT_LB

        اگر (getsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT_LB,
                       (خالی *) &reuseport, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_socket_errno,
                          "getsockopt(SO_REUSEPORT_LB) %V failed, ignored",
                          &ls[i].addr_متن);

        } ورنہ {
            ls[i].reuseport = reuseport ? 1 : 0;
        }

#ورنہ

        اگر (getsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT,
                       (خالی *) &reuseport, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_socket_errno,
                          "getsockopt(SO_REUSEPORT) %V failed, ignored",
                          &ls[i].addr_متن);

        } ورنہ {
            ls[i].reuseport = reuseport ? 1 : 0;
        }
#اگر_ختم

#اگر_ختم

        اگر (ls[i].type != SOCK_دھارا) {
            جاری;
        }

#اگر_تعریف_شدہ SO_PROTOCOL

        olen = حجم(صحیح);

        اگر (getsockopt(ls[i].fd, SOL_SOCKET, SO_PROTOCOL,
                       (خالی *) &ls[i].protocol, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_socket_errno,
                          "getsockopt(SO_PROTOCOL) %V failed, ignored",
                          &ls[i].addr_متن);
            ls[i].protocol = 0;

        } ورنہ اگر (ls[i].protocol == IPPROTO_TCP) {
            ls[i].protocol = 0;
        }

#اگر_ختم

#اگر (NGX_HAVE_TCP_FASTOPEN)

        olen = حجم(صحیح);

        اگر (getsockopt(ls[i].fd, IPPROTO_TCP, TCP_FASTOPEN,
                       (خالی *) &ls[i].fastopen, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            اگر (err != NGX_EOPNOTSUPP && err != NGX_ENOPROTOOPT
                && err != NGX_EINVAL)
            {
                ngx_log_error(NGX_LOG_NOTICE, چکر->لوگارتھم, err,
                              "getsockopt(TCP_FASTOPEN) %V failed, ignored",
                              &ls[i].addr_متن);
            }

            ls[i].fastopen = -1;
        }

#اگر_ختم

#اگر (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)

        ngx_memzero(&af, حجم(ساخت accept_filter_arg));
        olen = حجم(ساخت accept_filter_arg);

        اگر (getsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER, &af, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            اگر (err == NGX_EINVAL) {
                جاری;
            }

            ngx_log_error(NGX_LOG_NOTICE, چکر->لوگارتھم, err,
                          "getsockopt(SO_ACCEPTFILTER) for %V failed, ignored",
                          &ls[i].addr_متن);
            جاری;
        }

        اگر (olen < حجم(ساخت accept_filter_arg) || af.af_name[0] == '\0') {
            جاری;
        }

        ls[i].accept_filter = ngx_palloc(چکر->pool, 16);
        اگر (ls[i].accept_filter == تہی) {
            واپس NGX_ERROR;
        }

        (خالی) ngx_cpystrn((u_char *) ls[i].accept_filter,
                           (u_char *) af.af_name, 16);
#اگر_ختم

#اگر (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)

        timeout = 0;
        olen = حجم(صحیح);

        اگر (getsockopt(ls[i].fd, IPPROTO_TCP, TCP_DEFER_ACCEPT, &timeout, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            اگر (err == NGX_EOPNOTSUPP) {
                جاری;
            }

            ngx_log_error(NGX_LOG_NOTICE, چکر->لوگارتھم, err,
                          "getsockopt(TCP_DEFER_ACCEPT) for %V failed, ignored",
                          &ls[i].addr_متن);
            جاری;
        }

        اگر (olen < حجم(صحیح) || timeout == 0) {
            جاری;
        }

        ls[i].deferred_accept = 1;
#اگر_ختم
    }

    واپس NGX_OK;
}


ngx_int_t
ngx_open_listening_sockets(ngx_چکر_t *چکر)
{
    صحیح               reuseaddr;
    ngx_uint_t        i, tries, failed;
    ngx_err_t         err;
    ngx_log_t        *لوگارتھم;
    ngx_socket_t      s;
    ngx_listening_t  *ls;

    reuseaddr = 1;
#اگر (NGX_SUPPRESS_WARN)
    failed = 0;
#اگر_ختم

    لوگارتھم = چکر->لوگارتھم;

    /* TODO: configurable try number */

    برائے (tries = 5; tries; tries--) {
        failed = 0;

        /* for each listening socket */

        ls = چکر->listening.elts;
        برائے (i = 0; i < چکر->listening.nelts; i++) {

            اگر (ls[i].ignore) {
                جاری;
            }

#اگر (NGX_HAVE_REUSEPORT)

            اگر (ls[i].جوڑو_reuseport || ls[i].change_protocol) {

                /*
                 * to allow transition from a socket without SO_REUSEPORT
                 * to multiple sockets with SO_REUSEPORT, we have to set
                 * SO_REUSEPORT on the old socket before opening new ones
                 *
                 * to allow transition between different socket protocols
                 * (e.g. IPPROTO_MPTCP), SO_REUSEPORT is set on both old
                 * and new sockets
                 */

                صحیح  reuseport = 1;

#اگر_تعریف_شدہ SO_REUSEPORT_LB

                اگر (setsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT_LB,
                               (ثابت خالی *) &reuseport, حجم(صحیح))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT_LB) %V failed, "
                                  "ignored",
                                  &ls[i].addr_متن);
                }

#ورنہ

                اگر (setsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT,
                               (ثابت خالی *) &reuseport, حجم(صحیح))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT) %V failed, ignored",
                                  &ls[i].addr_متن);
                }
#اگر_ختم

                ls[i].جوڑو_reuseport = 0;
            }
#اگر_ختم

            اگر (ls[i].fd != (ngx_socket_t) -1 && !ls[i].change_protocol) {
                جاری;
            }

            اگر (ls[i].inherited) {

                /* TODO: close on exit */
                /* TODO: nonblocking */
                /* TODO: deferred accept */

                جاری;
            }

            s = ngx_socket(ls[i].sockaddr->sa_family, ls[i].type,
                           ls[i].protocol);

            اگر (s == (ngx_socket_t) -1) {
                ngx_log_error(NGX_LOG_EMERG, لوگارتھم, ngx_socket_errno,
                              ngx_socket_n " %V failed", &ls[i].addr_متن);
                واپس NGX_ERROR;
            }

            اگر (ls[i].type != SOCK_DGRAM || !ngx_test_config) {

                اگر (setsockopt(s, SOL_SOCKET, SO_REUSEADDR,
                               (ثابت خالی *) &reuseaddr, حجم(صحیح))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, لوگارتھم, ngx_socket_errno,
                                  "setsockopt(SO_REUSEADDR) %V failed",
                                  &ls[i].addr_متن);

                    اگر (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, لوگارتھم, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_متن);
                    }

                    واپس NGX_ERROR;
                }
            }

#اگر (NGX_HAVE_REUSEPORT)

            اگر ((ls[i].reuseport || ls[i].change_protocol)
                && !ngx_test_config)
            {
                صحیح  reuseport;

                reuseport = 1;

#اگر_تعریف_شدہ SO_REUSEPORT_LB

                اگر (setsockopt(s, SOL_SOCKET, SO_REUSEPORT_LB,
                               (ثابت خالی *) &reuseport, حجم(صحیح))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, لوگارتھم, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT_LB) %V failed",
                                  &ls[i].addr_متن);

                    اگر (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, لوگارتھم, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_متن);
                    }

                    واپس NGX_ERROR;
                }

#ورنہ

                اگر (setsockopt(s, SOL_SOCKET, SO_REUSEPORT,
                               (ثابت خالی *) &reuseport, حجم(صحیح))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, لوگارتھم, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT) %V failed",
                                  &ls[i].addr_متن);

                    اگر (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, لوگارتھم, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_متن);
                    }

                    واپس NGX_ERROR;
                }
#اگر_ختم
            }
#اگر_ختم

#اگر (NGX_HAVE_INET6 && defined IPV6_V6ONLY)

            اگر (ls[i].sockaddr->sa_family == AF_INET6) {
                صحیح  ipv6only;

                ipv6only = ls[i].ipv6only;

                اگر (setsockopt(s, IPPROTO_IPV6, IPV6_V6ONLY,
                               (ثابت خالی *) &ipv6only, حجم(صحیح))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, لوگارتھم, ngx_socket_errno,
                                  "setsockopt(IPV6_V6ONLY) %V failed, ignored",
                                  &ls[i].addr_متن);
                }
            }
#اگر_ختم
            /* TODO: close on exit */

            اگر (ngx_nonblocking(s) == -1) {
                ngx_log_error(NGX_LOG_EMERG, لوگارتھم, ngx_socket_errno,
                              ngx_nonblocking_n " %V failed",
                              &ls[i].addr_متن);

                اگر (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, لوگارتھم, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_متن);
                }

                واپس NGX_ERROR;
            }

            ngx_log_debug2(NGX_LOG_DEBUG_CORE, لوگارتھم, 0,
                           "bind() %V #%d ", &ls[i].addr_متن, s);

            اگر (bind(s, ls[i].sockaddr, ls[i].socklen) == -1) {
                err = ngx_socket_errno;

                اگر (err != NGX_EADDRINUSE || !ngx_test_config) {
                    ngx_log_error(NGX_LOG_EMERG, لوگارتھم, err,
                                  "bind() to %V failed", &ls[i].addr_متن);
                }

                اگر (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, لوگارتھم, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_متن);
                }

                اگر (err != NGX_EADDRINUSE) {
                    واپس NGX_ERROR;
                }

                اگر (!ngx_test_config) {
                    failed = 1;
                }

                جاری;
            }

#اگر (NGX_HAVE_UNIX_DOMAIN)

            اگر (ls[i].sockaddr->sa_family == AF_UNIX) {
                منوال_t   منوال;
                u_char  *name;

                name = ls[i].addr_متن.ڈیٹا + حجم("unix:") - 1;
                منوال = (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);

                اگر (chmod((حرف *) name, منوال) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_errno,
                                  "chmod() \"%s\" failed", name);
                }

                اگر (ngx_test_config) {
                    اگر (ngx_delete_file(name) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_errno,
                                      ngx_delete_file_n " %s failed", name);
                    }
                }
            }
#اگر_ختم

            اگر (ls[i].type != SOCK_دھارا) {
                ls[i].fd = s;
                ls[i].open = 1;
                جاری;
            }

            اگر (listen(s, ls[i].backlog) == -1) {
                err = ngx_socket_errno;

                /*
                 * on OpenVZ after suspend/resume EADDRINUSE
                 * may be returned by listen() instead of bind(), see
                 * https://bugs.openvz.org/browse/OVZ-5587
                 */

                اگر (err != NGX_EADDRINUSE || !ngx_test_config) {
                    ngx_log_error(NGX_LOG_EMERG, لوگارتھم, err,
                                  "listen() to %V, backlog %d failed",
                                  &ls[i].addr_متن, ls[i].backlog);
                }

                اگر (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, لوگارتھم, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_متن);
                }

                اگر (err != NGX_EADDRINUSE) {
                    واپس NGX_ERROR;
                }

                اگر (!ngx_test_config) {
                    failed = 1;
                }

                جاری;
            }

            ls[i].listen = 1;

            ls[i].fd = s;
            ls[i].open = 1;
        }

        اگر (!failed) {
            توڑو;
        }

        /* TODO: delay configurable */

        ngx_log_error(NGX_LOG_NOTICE, لوگارتھم, 0,
                      "try again to bind() after 500ms");

        ngx_msleep(500);
    }

    اگر (failed) {
        ngx_log_error(NGX_LOG_EMERG, لوگارتھم, 0, "still could not bind()");
        واپس NGX_ERROR;
    }

    واپس NGX_OK;
}


خالی
ngx_configure_listening_sockets(ngx_چکر_t *چکر)
{
    صحیح                        قدر;
    ngx_uint_t                 i;
    ngx_listening_t           *ls;

#اگر (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    ساخت accept_filter_arg   af;
#اگر_ختم

    ls = چکر->listening.elts;
    برائے (i = 0; i < چکر->listening.nelts; i++) {

        ls[i].لوگارتھم = *ls[i].logp;

        اگر (ls[i].rcvbuf != -1) {
            اگر (setsockopt(ls[i].fd, SOL_SOCKET, SO_RCVBUF,
                           (ثابت خالی *) &ls[i].rcvbuf, حجم(صحیح))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_socket_errno,
                              "setsockopt(SO_RCVBUF, %d) %V failed, ignored",
                              ls[i].rcvbuf, &ls[i].addr_متن);
            }
        }

        اگر (ls[i].sndbuf != -1) {
            اگر (setsockopt(ls[i].fd, SOL_SOCKET, SO_SNDBUF,
                           (ثابت خالی *) &ls[i].sndbuf, حجم(صحیح))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_socket_errno,
                              "setsockopt(SO_SNDBUF, %d) %V failed, ignored",
                              ls[i].sndbuf, &ls[i].addr_متن);
            }
        }

        اگر (ls[i].keepalive) {
            قدر = (ls[i].keepalive == 1) ? 1 : 0;

            اگر (setsockopt(ls[i].fd, SOL_SOCKET, SO_KEEPALIVE,
                           (ثابت خالی *) &قدر, حجم(صحیح))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_socket_errno,
                              "setsockopt(SO_KEEPALIVE, %d) %V failed, ignored",
                              قدر, &ls[i].addr_متن);
            }
        }

#اگر (NGX_HAVE_KEEPALIVE_TUNABLE)

#اگر !(NGX_DARWIN)

        اگر (ls[i].keepidle) {
            قدر = ls[i].keepidle;

#اگر (NGX_KEEPALIVE_ضربی_عدد)
            قدر *= NGX_KEEPALIVE_ضربی_عدد;
#اگر_ختم

            اگر (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPIDLE,
                           (ثابت خالی *) &قدر, حجم(صحیح))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_socket_errno,
                              "setsockopt(TCP_KEEPIDLE, %d) %V failed, ignored",
                              قدر, &ls[i].addr_متن);
            }
        }

#اگر_ختم

        اگر (ls[i].keepintvl) {
            قدر = ls[i].keepintvl;

#اگر (NGX_KEEPALIVE_ضربی_عدد)
            قدر *= NGX_KEEPALIVE_ضربی_عدد;
#اگر_ختم

            اگر (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPINTVL,
                           (ثابت خالی *) &قدر, حجم(صحیح))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_socket_errno,
                             "setsockopt(TCP_KEEPINTVL, %d) %V failed, ignored",
                             قدر, &ls[i].addr_متن);
            }
        }

        اگر (ls[i].keepcnt) {
            اگر (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPCNT,
                           (ثابت خالی *) &ls[i].keepcnt, حجم(صحیح))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_socket_errno,
                              "setsockopt(TCP_KEEPCNT, %d) %V failed, ignored",
                              ls[i].keepcnt, &ls[i].addr_متن);
            }
        }

#اگر_ختم

#اگر (NGX_HAVE_SETFIB)
        اگر (ls[i].setfib != -1) {
            اگر (setsockopt(ls[i].fd, SOL_SOCKET, SO_SETFIB,
                           (ثابت خالی *) &ls[i].setfib, حجم(صحیح))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_socket_errno,
                              "setsockopt(SO_SETFIB, %d) %V failed, ignored",
                              ls[i].setfib, &ls[i].addr_متن);
            }
        }
#اگر_ختم

#اگر (NGX_HAVE_TCP_FASTOPEN)
        اگر (ls[i].fastopen != -1) {
            اگر (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_FASTOPEN,
                           (ثابت خالی *) &ls[i].fastopen, حجم(صحیح))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_socket_errno,
                              "setsockopt(TCP_FASTOPEN, %d) %V failed, ignored",
                              ls[i].fastopen, &ls[i].addr_متن);
            }
        }
#اگر_ختم

#اگر 0
        اگر (1) {
            صحیح tcp_nodelay = 1;

            اگر (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_NODELAY,
                       (ثابت خالی *) &tcp_nodelay, حجم(صحیح))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_socket_errno,
                              "setsockopt(TCP_NODELAY) %V failed, ignored",
                              &ls[i].addr_متن);
            }
        }
#اگر_ختم

        اگر (ls[i].listen) {

            /* change backlog via listen() */

            اگر (listen(ls[i].fd, ls[i].backlog) == -1) {
                ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_socket_errno,
                              "listen() to %V, backlog %d failed, ignored",
                              &ls[i].addr_متن, ls[i].backlog);
            }
        }

        /*
         * setting deferred mode should be last operation on socket,
         * because code may prematurely continue cycle on failure
         */

#اگر (NGX_HAVE_DEFERRED_ACCEPT)

#اگر_تعریف_شدہ SO_ACCEPTFILTER

        اگر (ls[i].delete_deferred) {
            اگر (setsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER, تہی, 0)
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_socket_errno,
                              "setsockopt(SO_ACCEPTFILTER, NULL) "
                              "for %V failed, ignored",
                              &ls[i].addr_متن);

                اگر (ls[i].accept_filter) {
                    ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, 0,
                                  "could not change the accept filter "
                                  "to \"%s\" for %V, ignored",
                                  ls[i].accept_filter, &ls[i].addr_متن);
                }

                جاری;
            }

            ls[i].deferred_accept = 0;
        }

        اگر (ls[i].جوڑو_deferred) {
            ngx_memzero(&af, حجم(ساخت accept_filter_arg));
            (خالی) ngx_cpystrn((u_char *) af.af_name,
                               (u_char *) ls[i].accept_filter, 16);

            اگر (setsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER,
                           &af, حجم(ساخت accept_filter_arg))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_socket_errno,
                              "setsockopt(SO_ACCEPTFILTER, \"%s\") "
                              "for %V failed, ignored",
                              ls[i].accept_filter, &ls[i].addr_متن);
                جاری;
            }

            ls[i].deferred_accept = 1;
        }

#اگر_ختم

#اگر_تعریف_شدہ TCP_DEFER_ACCEPT

        اگر (ls[i].جوڑو_deferred || ls[i].delete_deferred) {

            اگر (ls[i].جوڑو_deferred) {
                /*
                 * There is no way to find out how long a connection was
                 * in queue (and a connection may bypass deferred queue at all
                 * if syncookies were used), hence we use 1 second timeout
                 * here.
                 */
                قدر = 1;

            } ورنہ {
                قدر = 0;
            }

            اگر (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_DEFER_ACCEPT,
                           &قدر, حجم(صحیح))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_socket_errno,
                              "setsockopt(TCP_DEFER_ACCEPT, %d) for %V failed, "
                              "ignored",
                              قدر, &ls[i].addr_متن);

                جاری;
            }
        }

        اگر (ls[i].جوڑو_deferred) {
            ls[i].deferred_accept = 1;
        }

#اگر_ختم

#اگر_ختم /* NGX_HAVE_DEFERRED_ACCEPT */

#اگر (NGX_HAVE_IP_RECVDSTADDR)

        اگر (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET)
        {
            قدر = 1;

            اگر (setsockopt(ls[i].fd, IPPROTO_IP, IP_RECVDSTADDR,
                           (ثابت خالی *) &قدر, حجم(صحیح))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_socket_errno,
                              "setsockopt(IP_RECVDSTADDR) "
                              "for %V failed, ignored",
                              &ls[i].addr_متن);
            }
        }

#وگرنہ_اگر (NGX_HAVE_IP_PKTINFO)

        اگر (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET)
        {
            قدر = 1;

            اگر (setsockopt(ls[i].fd, IPPROTO_IP, IP_PKTINFO,
                           (ثابت خالی *) &قدر, حجم(صحیح))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_socket_errno,
                              "setsockopt(IP_PKTINFO) "
                              "for %V failed, ignored",
                              &ls[i].addr_متن);
            }
        }

#اگر_ختم

#اگر (NGX_HAVE_INET6 && NGX_HAVE_IPV6_RECVPKTINFO)

        اگر (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET6)
        {
            قدر = 1;

            اگر (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_RECVPKTINFO,
                           (ثابت خالی *) &قدر, حجم(صحیح))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_socket_errno,
                              "setsockopt(IPV6_RECVPKTINFO) "
                              "for %V failed, ignored",
                              &ls[i].addr_متن);
            }
        }

#اگر_ختم

#اگر (NGX_HAVE_IP_MTU_DISCOVER)

        اگر (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET) {
            قدر = IP_PMTUDISC_DO;

            اگر (setsockopt(ls[i].fd, IPPROTO_IP, IP_MTU_DISCOVER,
                           (ثابت خالی *) &قدر, حجم(صحیح))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_socket_errno,
                              "setsockopt(IP_MTU_DISCOVER) "
                              "for %V failed, ignored",
                              &ls[i].addr_متن);
            }
        }

#وگرنہ_اگر (NGX_HAVE_IP_DONTFRAG)

        اگر (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET) {
            قدر = 1;

            اگر (setsockopt(ls[i].fd, IPPROTO_IP, IP_DONTFRAG,
                           (ثابت خالی *) &قدر, حجم(صحیح))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_socket_errno,
                              "setsockopt(IP_DONTFRAG) "
                              "for %V failed, ignored",
                              &ls[i].addr_متن);
            }
        }

#اگر_ختم

#اگر (NGX_HAVE_INET6)

#اگر (NGX_HAVE_IPV6_MTU_DISCOVER)

        اگر (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET6) {
            قدر = IPV6_PMTUDISC_DO;

            اگر (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_MTU_DISCOVER,
                           (ثابت خالی *) &قدر, حجم(صحیح))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_socket_errno,
                              "setsockopt(IPV6_MTU_DISCOVER) "
                              "for %V failed, ignored",
                              &ls[i].addr_متن);
            }
        }

#وگرنہ_اگر (NGX_HAVE_IPV6_DONTFRAG)

        اگر (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET6) {
            قدر = 1;

            اگر (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_DONTFRAG,
                           (ثابت خالی *) &قدر, حجم(صحیح))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_socket_errno,
                              "setsockopt(IPV6_DONTFRAG) "
                              "for %V failed, ignored",
                              &ls[i].addr_متن);
            }
        }

#اگر_ختم

#اگر_ختم
    }

    واپس;
}


خالی
ngx_close_listening_sockets(ngx_چکر_t *چکر)
{
    ngx_uint_t         i;
    ngx_listening_t   *ls;
    ngx_connection_t  *c;

    ngx_accept_mutex_held = 0;
    ngx_use_accept_mutex = 0;

    ls = چکر->listening.elts;
    برائے (i = 0; i < چکر->listening.nelts; i++) {

#اگر (NGX_QUIC)
        اگر (ls[i].quic
            && (!ls[i].reuseport || !ngx_quic_bpf_enabled(چکر)))
        {
            جاری;
        }
#اگر_ختم

        c = ls[i].connection;

        اگر (c) {
            اگر (c->اقرأ->فعال) {
                اگر (ngx_event_flags & NGX_USE_EPOLL_EVENT) {

                    /*
                     * it seems that Linux-2.6.x OpenVZ sends events
                     * for closed shared listening sockets unless
                     * the events was explicitly deleted
                     */

                    ngx_del_event(c->اقرأ, NGX_READ_EVENT, 0);

                } ورنہ {
                    ngx_del_event(c->اقرأ, NGX_READ_EVENT, NGX_CLOSE_EVENT);
                }
            }

            اگر (c->اقرأ->timer_سیٹ) {
                ngx_del_timer(c->اقرأ);
            }

            ngx_free_connection(c);

            c->fd = (ngx_socket_t) -1;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, چکر->لوگارتھم, 0,
                       "close listening %V #%d ", &ls[i].addr_متن, ls[i].fd);

        اگر (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_socket_errno,
                          ngx_close_socket_n " %V failed", &ls[i].addr_متن);
        }

#اگر (NGX_HAVE_UNIX_DOMAIN)

        اگر (ls[i].sockaddr->sa_family == AF_UNIX
            && ngx_process <= NGX_PROCESS_MASTER
            && ngx_new_ثنائی == 0
            && (!ls[i].inherited || ngx_getppid() != ngx_والد))
        {
            u_char *name = ls[i].addr_متن.ڈیٹا + حجم("unix:") - 1;

            اگر (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_socket_errno,
                              ngx_delete_file_n " %s failed", name);
            }
        }

#اگر_ختم

        ls[i].fd = (ngx_socket_t) -1;
    }

    چکر->listening.nelts = 0;
}


ngx_connection_t *
ngx_get_connection(ngx_socket_t s, ngx_log_t *لوگارتھم)
{
    ngx_uint_t         instance;
    ngx_event_t       *rev, *wev;
    ngx_connection_t  *c;

    /* disable warning: Win32 SOCKET is u_int while UNIX socket is int */

    اگر (ngx_چکر->files && (ngx_uint_t) s >= ngx_چکر->files_n) {
        ngx_log_error(NGX_LOG_ALERT, لوگارتھم, 0,
                      "the new socket has number %d, "
                      "but only %ui files are available",
                      s, ngx_چکر->files_n);
        واپس تہی;
    }

    ngx_drain_connections((ngx_چکر_t *) ngx_چکر);

    c = ngx_چکر->free_connections;

    اگر (c == تہی) {
        ngx_log_error(NGX_LOG_ALERT, لوگارتھم, 0,
                      "%ui worker_connections are not enough",
                      ngx_چکر->connection_n);

        واپس تہی;
    }

    ngx_چکر->free_connections = c->ڈیٹا;
    ngx_چکر->free_connection_n--;

    اگر (ngx_چکر->files && ngx_چکر->files[s] == تہی) {
        ngx_چکر->files[s] = c;
    }

    rev = c->اقرأ;
    wev = c->اكتب;

    ngx_memzero(c, حجم(ngx_connection_t));

    c->اقرأ = rev;
    c->اكتب = wev;
    c->fd = s;
    c->لوگارتھم = لوگارتھم;

    instance = rev->instance;

    ngx_memzero(rev, حجم(ngx_event_t));
    ngx_memzero(wev, حجم(ngx_event_t));

    rev->instance = !instance;
    wev->instance = !instance;

    rev->اشاریہ = NGX_INVALID_اشاریہ;
    wev->اشاریہ = NGX_INVALID_اشاریہ;

    rev->ڈیٹا = c;
    wev->ڈیٹا = c;

    wev->اكتب = 1;

    واپس c;
}


خالی
ngx_free_connection(ngx_connection_t *c)
{
    c->ڈیٹا = ngx_چکر->free_connections;
    ngx_چکر->free_connections = c;
    ngx_چکر->free_connection_n++;

    اگر (ngx_چکر->files && ngx_چکر->files[c->fd] == c) {
        ngx_چکر->files[c->fd] = تہی;
    }
}


خالی
ngx_close_connection(ngx_connection_t *c)
{
    ngx_err_t     err;
    ngx_uint_t    log_error, level;
    ngx_socket_t  fd;

    اگر (c->fd == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, c->لوگارتھم, 0, "connection already closed");
        واپس;
    }

    اگر (c->اقرأ->timer_سیٹ) {
        ngx_del_timer(c->اقرأ);
    }

    اگر (c->اكتب->timer_سیٹ) {
        ngx_del_timer(c->اكتب);
    }

    اگر (!c->shared) {
        اگر (ngx_del_conn) {
            ngx_del_conn(c, NGX_CLOSE_EVENT);

        } ورنہ {
            اگر (c->اقرأ->فعال || c->اقرأ->disabled) {
                ngx_del_event(c->اقرأ, NGX_READ_EVENT, NGX_CLOSE_EVENT);
            }

            اگر (c->اكتب->فعال || c->اكتب->disabled) {
                ngx_del_event(c->اكتب, NGX_WRITE_EVENT, NGX_CLOSE_EVENT);
            }
        }
    }

    اگر (c->اقرأ->posted) {
        ngx_delete_posted_event(c->اقرأ);
    }

    اگر (c->اكتب->posted) {
        ngx_delete_posted_event(c->اكتب);
    }

    c->اقرأ->closed = 1;
    c->اكتب->closed = 1;

    ngx_reusable_connection(c, 0);

    log_error = c->log_error;

    ngx_free_connection(c);

    fd = c->fd;
    c->fd = (ngx_socket_t) -1;

    اگر (c->shared) {
        واپس;
    }

    اگر (ngx_close_socket(fd) == -1) {

        err = ngx_socket_errno;

        اگر (err == NGX_ECONNRESET || err == NGX_ENOTCONN) {

            انتخاب (log_error) {

            حالت NGX_ERROR_INFO:
                level = NGX_LOG_INFO;
                توڑو;

            حالت NGX_ERROR_ERR:
                level = NGX_LOG_ERR;
                توڑو;

            طےشدہ:
                level = NGX_LOG_CRIT;
            }

        } ورنہ {
            level = NGX_LOG_CRIT;
        }

        ngx_log_error(level, c->لوگارتھم, err, ngx_close_socket_n " %d failed", fd);
    }
}


خالی
ngx_reusable_connection(ngx_connection_t *c, ngx_uint_t reusable)
{
    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->لوگارتھم, 0,
                   "reusable connection: %ui", reusable);

    اگر (c->reusable) {
        ngx_قطار_remove(&c->قطار);
        ngx_چکر->reusable_connections_n--;

#اگر (NGX_STAT_STUB)
        (خالی) ngx_atomic_fetch_جوڑو(ngx_stat_انتظار, -1);
#اگر_ختم
    }

    c->reusable = reusable;

    اگر (reusable) {
        /* need cast as ngx_cycle is volatile */

        ngx_قطار_داخل_کرو_سر(
            (ngx_قطار_t *) &ngx_چکر->reusable_connections_قطار, &c->قطار);
        ngx_چکر->reusable_connections_n++;

#اگر (NGX_STAT_STUB)
        (خالی) ngx_atomic_fetch_جوڑو(ngx_stat_انتظار, 1);
#اگر_ختم
    }
}


ساکن خالی
ngx_drain_connections(ngx_چکر_t *چکر)
{
    ngx_uint_t         i, n;
    ngx_قطار_t       *q;
    ngx_connection_t  *c;

    اگر (چکر->free_connection_n > چکر->connection_n / 16
        || چکر->reusable_connections_n == 0)
    {
        واپس;
    }

    اگر (چکر->connections_reuse_وقت != ngx_وقت()) {
        چکر->connections_reuse_وقت = ngx_وقت();

        ngx_log_error(NGX_LOG_WARN, چکر->لوگارتھم, 0,
                      "%ui worker_connections are not enough, "
                      "reusing connections",
                      چکر->connection_n);
    }

    c = تہی;
    n = ngx_max(ngx_min(32, چکر->reusable_connections_n / 8), 1);

    برائے (i = 0; i < n; i++) {
        اگر (ngx_قطار_خالی_خانے(&چکر->reusable_connections_قطار)) {
            توڑو;
        }

        q = ngx_قطار_آخری(&چکر->reusable_connections_قطار);
        c = ngx_قطار_ڈیٹا(q, ngx_connection_t, قطار);

        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->لوگارتھم, 0,
                       "reusing connection");

        c->close = 1;
        c->اقرأ->handler(c->اقرأ);
    }

    اگر (چکر->free_connection_n == 0 && c && c->reusable) {

        /*
         * if no connections were freed, try to reuse the last
         * connection again: this should free it as long as
         * previous reuse moved it to lingering close
         */

        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->لوگارتھم, 0,
                       "reusing connection again");

        c->close = 1;
        c->اقرأ->handler(c->اقرأ);
    }
}


خالی
ngx_close_idle_connections(ngx_چکر_t *چکر)
{
    ngx_uint_t         i;
    ngx_connection_t  *c;

    c = چکر->connections;

    برائے (i = 0; i < چکر->connection_n; i++) {

        /* THREAD: lock */

        اگر (c[i].fd != (ngx_socket_t) -1 && c[i].idle) {
            c[i].close = 1;
            c[i].اقرأ->handler(c[i].اقرأ);
        }
    }
}


ngx_int_t
ngx_connection_local_sockaddr(ngx_connection_t *c, ngx_str_t *s,
    ngx_uint_t پورٹ)
{
    socklen_t             len;
    ngx_uint_t            addr;
    ngx_sockaddr_t        sa;
    ساخت sockaddr_اندر   *جیب;
#اگر (NGX_HAVE_INET6)
    ngx_uint_t            i;
    ساخت sockaddr_in6  *sin6;
#اگر_ختم

    addr = 0;

    اگر (c->local_socklen) {
        انتخاب (c->local_sockaddr->sa_family) {

#اگر (NGX_HAVE_INET6)
        حالت AF_INET6:
            sin6 = (ساخت sockaddr_in6 *) c->local_sockaddr;

            برائے (i = 0; addr == 0 && i < 16; i++) {
                addr |= sin6->sin6_addr.s6_addr[i];
            }

            توڑو;
#اگر_ختم

#اگر (NGX_HAVE_UNIX_DOMAIN)
        حالت AF_UNIX:
            addr = 1;
            توڑو;
#اگر_ختم

        طےشدہ: /* AF_INET */
            جیب = (ساخت sockaddr_اندر *) c->local_sockaddr;
            addr = جیب->sin_addr.s_addr;
            توڑو;
        }
    }

    اگر (addr == 0) {

        len = حجم(ngx_sockaddr_t);

        اگر (getsockname(c->fd, &sa.sockaddr, &len) == -1) {
            ngx_connection_error(c, ngx_socket_errno, "getsockname() failed");
            واپس NGX_ERROR;
        }

        c->local_sockaddr = ngx_palloc(c->pool, len);
        اگر (c->local_sockaddr == تہی) {
            واپس NGX_ERROR;
        }

        ngx_memcpy(c->local_sockaddr, &sa, len);

        c->local_socklen = len;
    }

    اگر (s == تہی) {
        واپس NGX_OK;
    }

    s->len = ngx_sock_ntop(c->local_sockaddr, c->local_socklen,
                           s->ڈیٹا, s->len, پورٹ);

    واپس NGX_OK;
}


ngx_int_t
ngx_tcp_nodelay(ngx_connection_t *c)
{
    صحیح  tcp_nodelay;

    اگر (c->tcp_nodelay != NGX_TCP_NODELAY_UNSET) {
        واپس NGX_OK;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->لوگارتھم, 0, "tcp_nodelay");

    tcp_nodelay = 1;

    اگر (setsockopt(c->fd, IPPROTO_TCP, TCP_NODELAY,
                   (ثابت خالی *) &tcp_nodelay, حجم(صحیح))
        == -1)
    {
#اگر (NGX_SOLARIS)
        اگر (c->log_error == NGX_ERROR_INFO) {

            /* Solaris returns EINVAL if a socket has been shut down */
            c->log_error = NGX_ERROR_IGNORE_EINVAL;

            ngx_connection_error(c, ngx_socket_errno,
                                 "setsockopt(TCP_NODELAY) failed");

            c->log_error = NGX_ERROR_INFO;

            واپس NGX_ERROR;
        }
#اگر_ختم

        ngx_connection_error(c, ngx_socket_errno,
                             "setsockopt(TCP_NODELAY) failed");
        واپس NGX_ERROR;
    }

    c->tcp_nodelay = NGX_TCP_NODELAY_سیٹ;

    واپس NGX_OK;
}


ngx_int_t
ngx_connection_error(ngx_connection_t *c, ngx_err_t err, حرف *متن)
{
    ngx_uint_t  level;

    /* Winsock may return NGX_ECONNABORTED instead of NGX_ECONNRESET */

    اگر ((err == NGX_ECONNRESET
#اگر (NGX_WIN32)
         || err == NGX_ECONNABORTED
#اگر_ختم
        ) && c->log_error == NGX_ERROR_IGNORE_ECONNRESET)
    {
        واپس 0;
    }

#اگر (NGX_SOLARIS)
    اگر (err == NGX_EINVAL && c->log_error == NGX_ERROR_IGNORE_EINVAL) {
        واپس 0;
    }
#اگر_ختم

    اگر (err == NGX_EMSGSIZE && c->log_error == NGX_ERROR_IGNORE_EMSGSIZE) {
        واپس 0;
    }

#اگر (NGX_LINUX)
    اگر (err == NGX_EIO && c->ktls_recv) {
        واپس 0;
    }
#اگر_ختم

    اگر (err == 0
        || err == NGX_ECONNRESET
#اگر (NGX_WIN32)
        || err == NGX_ECONNABORTED
#ورنہ
        || err == NGX_EPIPE
#اگر_ختم
        || err == NGX_ENOTCONN
        || err == NGX_ETIMEDOUT
        || err == NGX_ECONNREFUSED
        || err == NGX_ENETDOWN
        || err == NGX_ENETUNREACH
        || err == NGX_EHOSTDOWN
        || err == NGX_EHOSTUNREACH)
    {
        انتخاب (c->log_error) {

        حالت NGX_ERROR_IGNORE_EMSGSIZE:
        حالت NGX_ERROR_IGNORE_EINVAL:
        حالت NGX_ERROR_IGNORE_ECONNRESET:
        حالت NGX_ERROR_INFO:
            level = NGX_LOG_INFO;
            توڑو;

        طےشدہ:
            level = NGX_LOG_ERR;
        }

    } ورنہ {
        level = NGX_LOG_ALERT;
    }

    ngx_log_error(level, c->لوگارتھم, err, متن);

    واپس NGX_ERROR;
}
