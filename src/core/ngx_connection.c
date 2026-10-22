
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
    माप_प्रकार            len;
    ngx_listening_t  *ls;
    संरचना sockaddr  *sa;
    u_char            text[NGX_SOCKADDR_STRLEN];

    ls = ngx_array_ढकला(&cf->cycle->listening);
    जर (ls == रिक्त) {
        परत रिक्त;
    }

    ngx_memzero(ls, आकार(ngx_listening_t));

    sa = ngx_palloc(cf->pool, socklen);
    जर (sa == रिक्त) {
        परत रिक्त;
    }

    ngx_memcpy(sa, sockaddr, socklen);

    ls->sockaddr = sa;
    ls->socklen = socklen;

    len = ngx_sock_ntop(sa, socklen, text, NGX_SOCKADDR_STRLEN, 1);
    ls->addr_text.len = len;

    निवड (ls->sockaddr->sa_family) {
#जर (NGX_HAVE_INET6)
    स्थिती AF_INET6:
        ls->addr_text_max_len = NGX_INET6_ADDRSTRLEN;
        थांबा;
#endif
#जर (NGX_HAVE_UNIX_DOMAIN)
    स्थिती AF_UNIX:
        ls->addr_text_max_len = NGX_UNIX_ADDRSTRLEN;
        len++;
        थांबा;
#endif
    स्थिती AF_INET:
        ls->addr_text_max_len = NGX_INET_ADDRSTRLEN;
        थांबा;
    पूर्वनिर्धारित:
        ls->addr_text_max_len = NGX_SOCKADDR_STRLEN;
        थांबा;
    }

    ls->addr_text.data = ngx_pnalloc(cf->pool, len);
    जर (ls->addr_text.data == रिक्त) {
        परत रिक्त;
    }

    ngx_memcpy(ls->addr_text.data, text, len);

#जर !(NGX_WIN32)
    ngx_rbtree_init(&ls->rbtree, &ls->sentinel, ngx_udp_rbtree_घाला_मूल्य);
#endif

    ls->fd = (ngx_socket_t) -1;
    ls->type = SOCK_STREAM;

    ls->backlog = NGX_LISTEN_BACKLOG;
    ls->rcvbuf = -1;
    ls->sndbuf = -1;

#जर (NGX_HAVE_SETFIB)
    ls->setfib = -1;
#endif

#जर (NGX_HAVE_TCP_FASTOPEN)
    ls->fastopen = -1;
#endif

    परत ls;
}


ngx_int_t
ngx_clone_listening(ngx_cycle_t *cycle, ngx_listening_t *ls)
{
#जर (NGX_HAVE_REUSEPORT)

    ngx_int_t         n;
    ngx_core_conf_t  *ccf;
    ngx_listening_t   ols;

    जर (!ls->reuseport || ls->worker != 0) {
        परत NGX_OK;
    }

    ols = *ls;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    साठी (n = 1; n < ccf->worker_processes; n++) {

        /* create a socket for each worker process */

        ls = ngx_array_ढकला(&cycle->listening);
        जर (ls == रिक्त) {
            परत NGX_ERROR;
        }

        *ls = ols;
        ls->worker = n;
    }

#endif

    परत NGX_OK;
}


ngx_int_t
ngx_set_inherited_sockets(ngx_cycle_t *cycle)
{
    माप_प्रकार                     len;
    ngx_uint_t                 i;
    ngx_listening_t           *ls;
    socklen_t                  olen;
#जर (NGX_HAVE_DEFERRED_ACCEPT || NGX_HAVE_TCP_FASTOPEN)
    ngx_err_t                  err;
#endif
#जर (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    संरचना accept_filter_arg   af;
#endif
#जर (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)
    पूर्णांक                        timeout;
#endif
#जर (NGX_HAVE_REUSEPORT)
    पूर्णांक                        reuseport;
#endif

    ls = cycle->listening.elts;
    साठी (i = 0; i < cycle->listening.nelts; i++) {

        ls[i].sockaddr = ngx_palloc(cycle->pool, आकार(ngx_sockaddr_t));
        जर (ls[i].sockaddr == रिक्त) {
            परत NGX_ERROR;
        }

        ls[i].socklen = आकार(ngx_sockaddr_t);
        जर (getsockname(ls[i].fd, ls[i].sockaddr, &ls[i].socklen) == -1) {
            ngx_log_error(NGX_LOG_CRIT, cycle->log, ngx_socket_errno,
                          "getsockname() of the inherited "
                          "socket #%d failed", ls[i].fd);
            ls[i].ignore = 1;
            सुरूठेवा;
        }

        जर (ls[i].socklen > (socklen_t) आकार(ngx_sockaddr_t)) {
            ls[i].socklen = आकार(ngx_sockaddr_t);
        }

        निवड (ls[i].sockaddr->sa_family) {

#जर (NGX_HAVE_INET6)
        स्थिती AF_INET6:
            ls[i].addr_text_max_len = NGX_INET6_ADDRSTRLEN;
            len = NGX_INET6_ADDRSTRLEN + आकार("[]:65535") - 1;
            थांबा;
#endif

#जर (NGX_HAVE_UNIX_DOMAIN)
        स्थिती AF_UNIX:
            ls[i].addr_text_max_len = NGX_UNIX_ADDRSTRLEN;
            len = NGX_UNIX_ADDRSTRLEN;
            थांबा;
#endif

        स्थिती AF_INET:
            ls[i].addr_text_max_len = NGX_INET_ADDRSTRLEN;
            len = NGX_INET_ADDRSTRLEN + आकार(":65535") - 1;
            थांबा;

        पूर्वनिर्धारित:
            ngx_log_error(NGX_LOG_CRIT, cycle->log, ngx_socket_errno,
                          "the inherited socket #%d has "
                          "an unsupported protocol family", ls[i].fd);
            ls[i].ignore = 1;
            सुरूठेवा;
        }

        ls[i].addr_text.data = ngx_pnalloc(cycle->pool, len);
        जर (ls[i].addr_text.data == रिक्त) {
            परत NGX_ERROR;
        }

        len = ngx_sock_ntop(ls[i].sockaddr, ls[i].socklen,
                            ls[i].addr_text.data, len, 1);
        जर (len == 0) {
            परत NGX_ERROR;
        }

        ls[i].addr_text.len = len;

        ls[i].backlog = NGX_LISTEN_BACKLOG;

        olen = आकार(पूर्णांक);

        जर (getsockopt(ls[i].fd, SOL_SOCKET, SO_TYPE, (शून्य *) &ls[i].type,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_CRIT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_TYPE) %V failed", &ls[i].addr_text);
            ls[i].ignore = 1;
            सुरूठेवा;
        }

        olen = आकार(पूर्णांक);

        जर (getsockopt(ls[i].fd, SOL_SOCKET, SO_RCVBUF, (शून्य *) &ls[i].rcvbuf,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_RCVBUF) %V failed, ignored",
                          &ls[i].addr_text);

            ls[i].rcvbuf = -1;
        }

        olen = आकार(पूर्णांक);

        जर (getsockopt(ls[i].fd, SOL_SOCKET, SO_SNDBUF, (शून्य *) &ls[i].sndbuf,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_SNDBUF) %V failed, ignored",
                          &ls[i].addr_text);

            ls[i].sndbuf = -1;
        }

#जर 0
        /* SO_SETFIB is currently a set only option */

#जर (NGX_HAVE_SETFIB)

        olen = आकार(पूर्णांक);

        जर (getsockopt(ls[i].fd, SOL_SOCKET, SO_SETFIB,
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

#जर (NGX_HAVE_REUSEPORT)

        reuseport = 0;
        olen = आकार(पूर्णांक);

#ifdef SO_REUSEPORT_LB

        जर (getsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT_LB,
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

        जर (getsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT,
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

        जर (ls[i].type != SOCK_STREAM) {
            सुरूठेवा;
        }

#ifdef SO_PROTOCOL

        olen = आकार(पूर्णांक);

        जर (getsockopt(ls[i].fd, SOL_SOCKET, SO_PROTOCOL,
                       (शून्य *) &ls[i].protocol, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_PROTOCOL) %V failed, ignored",
                          &ls[i].addr_text);
            ls[i].protocol = 0;

        } अन्यथा जर (ls[i].protocol == IPPROTO_TCP) {
            ls[i].protocol = 0;
        }

#endif

#जर (NGX_HAVE_TCP_FASTOPEN)

        olen = आकार(पूर्णांक);

        जर (getsockopt(ls[i].fd, IPPROTO_TCP, TCP_FASTOPEN,
                       (शून्य *) &ls[i].fastopen, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            जर (err != NGX_EOPNOTSUPP && err != NGX_ENOPROTOOPT
                && err != NGX_EINVAL)
            {
                ngx_log_error(NGX_LOG_NOTICE, cycle->log, err,
                              "getsockopt(TCP_FASTOPEN) %V failed, ignored",
                              &ls[i].addr_text);
            }

            ls[i].fastopen = -1;
        }

#endif

#जर (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)

        ngx_memzero(&af, आकार(संरचना accept_filter_arg));
        olen = आकार(संरचना accept_filter_arg);

        जर (getsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER, &af, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            जर (err == NGX_EINVAL) {
                सुरूठेवा;
            }

            ngx_log_error(NGX_LOG_NOTICE, cycle->log, err,
                          "getsockopt(SO_ACCEPTFILTER) for %V failed, ignored",
                          &ls[i].addr_text);
            सुरूठेवा;
        }

        जर (olen < आकार(संरचना accept_filter_arg) || af.af_name[0] == '\0') {
            सुरूठेवा;
        }

        ls[i].accept_filter = ngx_palloc(cycle->pool, 16);
        जर (ls[i].accept_filter == रिक्त) {
            परत NGX_ERROR;
        }

        (शून्य) ngx_cpystrn((u_char *) ls[i].accept_filter,
                           (u_char *) af.af_name, 16);
#endif

#जर (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)

        timeout = 0;
        olen = आकार(पूर्णांक);

        जर (getsockopt(ls[i].fd, IPPROTO_TCP, TCP_DEFER_ACCEPT, &timeout, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            जर (err == NGX_EOPNOTSUPP) {
                सुरूठेवा;
            }

            ngx_log_error(NGX_LOG_NOTICE, cycle->log, err,
                          "getsockopt(TCP_DEFER_ACCEPT) for %V failed, ignored",
                          &ls[i].addr_text);
            सुरूठेवा;
        }

        जर (olen < आकार(पूर्णांक) || timeout == 0) {
            सुरूठेवा;
        }

        ls[i].deferred_accept = 1;
#endif
    }

    परत NGX_OK;
}


ngx_int_t
ngx_open_listening_sockets(ngx_cycle_t *cycle)
{
    पूर्णांक               reuseaddr;
    ngx_uint_t        i, tries, failed;
    ngx_err_t         err;
    ngx_log_t        *log;
    ngx_socket_t      s;
    ngx_listening_t  *ls;

    reuseaddr = 1;
#जर (NGX_SUPPRESS_WARN)
    failed = 0;
#endif

    log = cycle->log;

    /* TODO: configurable try number */

    साठी (tries = 5; tries; tries--) {
        failed = 0;

        /* for each listening socket */

        ls = cycle->listening.elts;
        साठी (i = 0; i < cycle->listening.nelts; i++) {

            जर (ls[i].ignore) {
                सुरूठेवा;
            }

#जर (NGX_HAVE_REUSEPORT)

            जर (ls[i].add_reuseport || ls[i].change_protocol) {

                /*
                 * to allow transition from a socket without SO_REUSEPORT
                 * to multiple sockets with SO_REUSEPORT, we have to set
                 * SO_REUSEPORT on the old socket before opening new ones
                 *
                 * to allow transition between different socket protocols
                 * (e.g. IPPROTO_MPTCP), SO_REUSEPORT is set on both old
                 * and new sockets
                 */

                पूर्णांक  reuseport = 1;

#ifdef SO_REUSEPORT_LB

                जर (setsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT_LB,
                               (स्थिरांक शून्य *) &reuseport, आकार(पूर्णांक))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT_LB) %V failed, "
                                  "ignored",
                                  &ls[i].addr_text);
                }

#अन्यथा

                जर (setsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT,
                               (स्थिरांक शून्य *) &reuseport, आकार(पूर्णांक))
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

            जर (ls[i].fd != (ngx_socket_t) -1 && !ls[i].change_protocol) {
                सुरूठेवा;
            }

            जर (ls[i].inherited) {

                /* TODO: close on exit */
                /* TODO: nonblocking */
                /* TODO: deferred accept */

                सुरूठेवा;
            }

            s = ngx_socket(ls[i].sockaddr->sa_family, ls[i].type,
                           ls[i].protocol);

            जर (s == (ngx_socket_t) -1) {
                ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                              ngx_socket_n " %V failed", &ls[i].addr_text);
                परत NGX_ERROR;
            }

            जर (ls[i].type != SOCK_DGRAM || !ngx_test_config) {

                जर (setsockopt(s, SOL_SOCKET, SO_REUSEADDR,
                               (स्थिरांक शून्य *) &reuseaddr, आकार(पूर्णांक))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEADDR) %V failed",
                                  &ls[i].addr_text);

                    जर (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_text);
                    }

                    परत NGX_ERROR;
                }
            }

#जर (NGX_HAVE_REUSEPORT)

            जर ((ls[i].reuseport || ls[i].change_protocol)
                && !ngx_test_config)
            {
                पूर्णांक  reuseport;

                reuseport = 1;

#ifdef SO_REUSEPORT_LB

                जर (setsockopt(s, SOL_SOCKET, SO_REUSEPORT_LB,
                               (स्थिरांक शून्य *) &reuseport, आकार(पूर्णांक))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT_LB) %V failed",
                                  &ls[i].addr_text);

                    जर (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_text);
                    }

                    परत NGX_ERROR;
                }

#अन्यथा

                जर (setsockopt(s, SOL_SOCKET, SO_REUSEPORT,
                               (स्थिरांक शून्य *) &reuseport, आकार(पूर्णांक))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT) %V failed",
                                  &ls[i].addr_text);

                    जर (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_text);
                    }

                    परत NGX_ERROR;
                }
#endif
            }
#endif

#जर (NGX_HAVE_INET6 && defined IPV6_V6ONLY)

            जर (ls[i].sockaddr->sa_family == AF_INET6) {
                पूर्णांक  ipv6only;

                ipv6only = ls[i].ipv6only;

                जर (setsockopt(s, IPPROTO_IPV6, IPV6_V6ONLY,
                               (स्थिरांक शून्य *) &ipv6only, आकार(पूर्णांक))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(IPV6_V6ONLY) %V failed, ignored",
                                  &ls[i].addr_text);
                }
            }
#endif
            /* TODO: close on exit */

            जर (ngx_nonblocking(s) == -1) {
                ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                              ngx_nonblocking_n " %V failed",
                              &ls[i].addr_text);

                जर (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_text);
                }

                परत NGX_ERROR;
            }

            ngx_log_debug2(NGX_LOG_DEBUG_CORE, log, 0,
                           "bind() %V #%d ", &ls[i].addr_text, s);

            जर (bind(s, ls[i].sockaddr, ls[i].socklen) == -1) {
                err = ngx_socket_errno;

                जर (err != NGX_EADDRINUSE || !ngx_test_config) {
                    ngx_log_error(NGX_LOG_EMERG, log, err,
                                  "bind() to %V failed", &ls[i].addr_text);
                }

                जर (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_text);
                }

                जर (err != NGX_EADDRINUSE) {
                    परत NGX_ERROR;
                }

                जर (!ngx_test_config) {
                    failed = 1;
                }

                सुरूठेवा;
            }

#जर (NGX_HAVE_UNIX_DOMAIN)

            जर (ls[i].sockaddr->sa_family == AF_UNIX) {
                mode_t   mode;
                u_char  *name;

                name = ls[i].addr_text.data + आकार("unix:") - 1;
                mode = (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);

                जर (chmod((अक्षर *) name, mode) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  "chmod() \"%s\" failed", name);
                }

                जर (ngx_test_config) {
                    जर (ngx_delete_file(name) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                      ngx_delete_file_n " %s failed", name);
                    }
                }
            }
#endif

            जर (ls[i].type != SOCK_STREAM) {
                ls[i].fd = s;
                ls[i].खोलो = 1;
                सुरूठेवा;
            }

            जर (listen(s, ls[i].backlog) == -1) {
                err = ngx_socket_errno;

                /*
                 * on OpenVZ after suspend/resume EADDRINUSE
                 * may be returned by listen() instead of bind(), see
                 * https://bugs.openvz.org/browse/OVZ-5587
                 */

                जर (err != NGX_EADDRINUSE || !ngx_test_config) {
                    ngx_log_error(NGX_LOG_EMERG, log, err,
                                  "listen() to %V, backlog %d failed",
                                  &ls[i].addr_text, ls[i].backlog);
                }

                जर (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_text);
                }

                जर (err != NGX_EADDRINUSE) {
                    परत NGX_ERROR;
                }

                जर (!ngx_test_config) {
                    failed = 1;
                }

                सुरूठेवा;
            }

            ls[i].listen = 1;

            ls[i].fd = s;
            ls[i].खोलो = 1;
        }

        जर (!failed) {
            थांबा;
        }

        /* TODO: delay configurable */

        ngx_log_error(NGX_LOG_NOTICE, log, 0,
                      "try again to bind() after 500ms");

        ngx_msleep(500);
    }

    जर (failed) {
        ngx_log_error(NGX_LOG_EMERG, log, 0, "still could not bind()");
        परत NGX_ERROR;
    }

    परत NGX_OK;
}


शून्य
ngx_configure_listening_sockets(ngx_cycle_t *cycle)
{
    पूर्णांक                        मूल्य;
    ngx_uint_t                 i;
    ngx_listening_t           *ls;

#जर (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    संरचना accept_filter_arg   af;
#endif

    ls = cycle->listening.elts;
    साठी (i = 0; i < cycle->listening.nelts; i++) {

        ls[i].log = *ls[i].logp;

        जर (ls[i].rcvbuf != -1) {
            जर (setsockopt(ls[i].fd, SOL_SOCKET, SO_RCVBUF,
                           (स्थिरांक शून्य *) &ls[i].rcvbuf, आकार(पूर्णांक))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_RCVBUF, %d) %V failed, ignored",
                              ls[i].rcvbuf, &ls[i].addr_text);
            }
        }

        जर (ls[i].sndbuf != -1) {
            जर (setsockopt(ls[i].fd, SOL_SOCKET, SO_SNDBUF,
                           (स्थिरांक शून्य *) &ls[i].sndbuf, आकार(पूर्णांक))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_SNDBUF, %d) %V failed, ignored",
                              ls[i].sndbuf, &ls[i].addr_text);
            }
        }

        जर (ls[i].keepalive) {
            मूल्य = (ls[i].keepalive == 1) ? 1 : 0;

            जर (setsockopt(ls[i].fd, SOL_SOCKET, SO_KEEPALIVE,
                           (स्थिरांक शून्य *) &मूल्य, आकार(पूर्णांक))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_KEEPALIVE, %d) %V failed, ignored",
                              मूल्य, &ls[i].addr_text);
            }
        }

#जर (NGX_HAVE_KEEPALIVE_TUNABLE)

#जर !(NGX_DARWIN)

        जर (ls[i].keepidle) {
            मूल्य = ls[i].keepidle;

#जर (NGX_KEEPALIVE_FACTOR)
            मूल्य *= NGX_KEEPALIVE_FACTOR;
#endif

            जर (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPIDLE,
                           (स्थिरांक शून्य *) &मूल्य, आकार(पूर्णांक))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_KEEPIDLE, %d) %V failed, ignored",
                              मूल्य, &ls[i].addr_text);
            }
        }

#endif

        जर (ls[i].keepintvl) {
            मूल्य = ls[i].keepintvl;

#जर (NGX_KEEPALIVE_FACTOR)
            मूल्य *= NGX_KEEPALIVE_FACTOR;
#endif

            जर (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPINTVL,
                           (स्थिरांक शून्य *) &मूल्य, आकार(पूर्णांक))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                             "setsockopt(TCP_KEEPINTVL, %d) %V failed, ignored",
                             मूल्य, &ls[i].addr_text);
            }
        }

        जर (ls[i].keepcnt) {
            जर (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPCNT,
                           (स्थिरांक शून्य *) &ls[i].keepcnt, आकार(पूर्णांक))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_KEEPCNT, %d) %V failed, ignored",
                              ls[i].keepcnt, &ls[i].addr_text);
            }
        }

#endif

#जर (NGX_HAVE_SETFIB)
        जर (ls[i].setfib != -1) {
            जर (setsockopt(ls[i].fd, SOL_SOCKET, SO_SETFIB,
                           (स्थिरांक शून्य *) &ls[i].setfib, आकार(पूर्णांक))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_SETFIB, %d) %V failed, ignored",
                              ls[i].setfib, &ls[i].addr_text);
            }
        }
#endif

#जर (NGX_HAVE_TCP_FASTOPEN)
        जर (ls[i].fastopen != -1) {
            जर (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_FASTOPEN,
                           (स्थिरांक शून्य *) &ls[i].fastopen, आकार(पूर्णांक))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_FASTOPEN, %d) %V failed, ignored",
                              ls[i].fastopen, &ls[i].addr_text);
            }
        }
#endif

#जर 0
        जर (1) {
            पूर्णांक tcp_nodelay = 1;

            जर (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_NODELAY,
                       (स्थिरांक शून्य *) &tcp_nodelay, आकार(पूर्णांक))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_NODELAY) %V failed, ignored",
                              &ls[i].addr_text);
            }
        }
#endif

        जर (ls[i].listen) {

            /* change backlog via listen() */

            जर (listen(ls[i].fd, ls[i].backlog) == -1) {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "listen() to %V, backlog %d failed, ignored",
                              &ls[i].addr_text, ls[i].backlog);
            }
        }

        /*
         * setting deferred mode should be last operation on socket,
         * because code may prematurely continue cycle on failure
         */

#जर (NGX_HAVE_DEFERRED_ACCEPT)

#ifdef SO_ACCEPTFILTER

        जर (ls[i].delete_deferred) {
            जर (setsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER, रिक्त, 0)
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_ACCEPTFILTER, NULL) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);

                जर (ls[i].accept_filter) {
                    ngx_log_error(NGX_LOG_ALERT, cycle->log, 0,
                                  "could not change the accept filter "
                                  "to \"%s\" for %V, ignored",
                                  ls[i].accept_filter, &ls[i].addr_text);
                }

                सुरूठेवा;
            }

            ls[i].deferred_accept = 0;
        }

        जर (ls[i].add_deferred) {
            ngx_memzero(&af, आकार(संरचना accept_filter_arg));
            (शून्य) ngx_cpystrn((u_char *) af.af_name,
                               (u_char *) ls[i].accept_filter, 16);

            जर (setsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER,
                           &af, आकार(संरचना accept_filter_arg))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_ACCEPTFILTER, \"%s\") "
                              "for %V failed, ignored",
                              ls[i].accept_filter, &ls[i].addr_text);
                सुरूठेवा;
            }

            ls[i].deferred_accept = 1;
        }

#endif

#ifdef TCP_DEFER_ACCEPT

        जर (ls[i].add_deferred || ls[i].delete_deferred) {

            जर (ls[i].add_deferred) {
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

            जर (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_DEFER_ACCEPT,
                           &मूल्य, आकार(पूर्णांक))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_DEFER_ACCEPT, %d) for %V failed, "
                              "ignored",
                              मूल्य, &ls[i].addr_text);

                सुरूठेवा;
            }
        }

        जर (ls[i].add_deferred) {
            ls[i].deferred_accept = 1;
        }

#endif

#endif /* NGX_HAVE_DEFERRED_ACCEPT */

#जर (NGX_HAVE_IP_RECVDSTADDR)

        जर (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET)
        {
            मूल्य = 1;

            जर (setsockopt(ls[i].fd, IPPROTO_IP, IP_RECVDSTADDR,
                           (स्थिरांक शून्य *) &मूल्य, आकार(पूर्णांक))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_RECVDSTADDR) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#किंवाजर (NGX_HAVE_IP_PKTINFO)

        जर (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET)
        {
            मूल्य = 1;

            जर (setsockopt(ls[i].fd, IPPROTO_IP, IP_PKTINFO,
                           (स्थिरांक शून्य *) &मूल्य, आकार(पूर्णांक))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_PKTINFO) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#endif

#जर (NGX_HAVE_INET6 && NGX_HAVE_IPV6_RECVPKTINFO)

        जर (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET6)
        {
            मूल्य = 1;

            जर (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_RECVPKTINFO,
                           (स्थिरांक शून्य *) &मूल्य, आकार(पूर्णांक))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IPV6_RECVPKTINFO) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#endif

#जर (NGX_HAVE_IP_MTU_DISCOVER)

        जर (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET) {
            मूल्य = IP_PMTUDISC_DO;

            जर (setsockopt(ls[i].fd, IPPROTO_IP, IP_MTU_DISCOVER,
                           (स्थिरांक शून्य *) &मूल्य, आकार(पूर्णांक))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_MTU_DISCOVER) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#किंवाजर (NGX_HAVE_IP_DONTFRAG)

        जर (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET) {
            मूल्य = 1;

            जर (setsockopt(ls[i].fd, IPPROTO_IP, IP_DONTFRAG,
                           (स्थिरांक शून्य *) &मूल्य, आकार(पूर्णांक))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_DONTFRAG) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#endif

#जर (NGX_HAVE_INET6)

#जर (NGX_HAVE_IPV6_MTU_DISCOVER)

        जर (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET6) {
            मूल्य = IPV6_PMTUDISC_DO;

            जर (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_MTU_DISCOVER,
                           (स्थिरांक शून्य *) &मूल्य, आकार(पूर्णांक))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IPV6_MTU_DISCOVER) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#किंवाजर (NGX_HAVE_IPV6_DONTFRAG)

        जर (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET6) {
            मूल्य = 1;

            जर (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_DONTFRAG,
                           (स्थिरांक शून्य *) &मूल्य, आकार(पूर्णांक))
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

    परत;
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
    साठी (i = 0; i < cycle->listening.nelts; i++) {

#जर (NGX_QUIC)
        जर (ls[i].quic
            && (!ls[i].reuseport || !ngx_quic_bpf_enabled(cycle)))
        {
            सुरूठेवा;
        }
#endif

        c = ls[i].connection;

        जर (c) {
            जर (c->पठ->active) {
                जर (ngx_event_flags & NGX_USE_EPOLL_EVENT) {

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

            जर (c->पठ->timer_set) {
                ngx_del_timer(c->पठ);
            }

            ngx_free_connection(c);

            c->fd = (ngx_socket_t) -1;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, cycle->log, 0,
                       "close listening %V #%d ", &ls[i].addr_text, ls[i].fd);

        जर (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_socket_errno,
                          ngx_close_socket_n " %V failed", &ls[i].addr_text);
        }

#जर (NGX_HAVE_UNIX_DOMAIN)

        जर (ls[i].sockaddr->sa_family == AF_UNIX
            && ngx_process <= NGX_PROCESS_MASTER
            && ngx_new_द्विमान == 0
            && (!ls[i].inherited || ngx_getppid() != ngx_parent))
        {
            u_char *name = ls[i].addr_text.data + आकार("unix:") - 1;

            जर (ngx_delete_file(name) == NGX_FILE_ERROR) {
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

    जर (ngx_cycle->files && (ngx_uint_t) s >= ngx_cycle->files_n) {
        ngx_log_error(NGX_LOG_ALERT, log, 0,
                      "the new socket has number %d, "
                      "but only %ui files are available",
                      s, ngx_cycle->files_n);
        परत रिक्त;
    }

    ngx_drain_connections((ngx_cycle_t *) ngx_cycle);

    c = ngx_cycle->free_connections;

    जर (c == रिक्त) {
        ngx_log_error(NGX_LOG_ALERT, log, 0,
                      "%ui worker_connections are not enough",
                      ngx_cycle->connection_n);

        परत रिक्त;
    }

    ngx_cycle->free_connections = c->data;
    ngx_cycle->free_connection_n--;

    जर (ngx_cycle->files && ngx_cycle->files[s] == रिक्त) {
        ngx_cycle->files[s] = c;
    }

    rev = c->पठ;
    wev = c->लिख;

    ngx_memzero(c, आकार(ngx_connection_t));

    c->पठ = rev;
    c->लिख = wev;
    c->fd = s;
    c->log = log;

    instance = rev->instance;

    ngx_memzero(rev, आकार(ngx_event_t));
    ngx_memzero(wev, आकार(ngx_event_t));

    rev->instance = !instance;
    wev->instance = !instance;

    rev->index = NGX_INVALID_INDEX;
    wev->index = NGX_INVALID_INDEX;

    rev->data = c;
    wev->data = c;

    wev->लिख = 1;

    परत c;
}


शून्य
ngx_free_connection(ngx_connection_t *c)
{
    c->data = ngx_cycle->free_connections;
    ngx_cycle->free_connections = c;
    ngx_cycle->free_connection_n++;

    जर (ngx_cycle->files && ngx_cycle->files[c->fd] == c) {
        ngx_cycle->files[c->fd] = रिक्त;
    }
}


शून्य
ngx_close_connection(ngx_connection_t *c)
{
    ngx_err_t     err;
    ngx_uint_t    log_error, level;
    ngx_socket_t  fd;

    जर (c->fd == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0, "connection already closed");
        परत;
    }

    जर (c->पठ->timer_set) {
        ngx_del_timer(c->पठ);
    }

    जर (c->लिख->timer_set) {
        ngx_del_timer(c->लिख);
    }

    जर (!c->shared) {
        जर (ngx_del_conn) {
            ngx_del_conn(c, NGX_CLOSE_EVENT);

        } अन्यथा {
            जर (c->पठ->active || c->पठ->disabled) {
                ngx_del_event(c->पठ, NGX_READ_EVENT, NGX_CLOSE_EVENT);
            }

            जर (c->लिख->active || c->लिख->disabled) {
                ngx_del_event(c->लिख, NGX_WRITE_EVENT, NGX_CLOSE_EVENT);
            }
        }
    }

    जर (c->पठ->posted) {
        ngx_delete_posted_event(c->पठ);
    }

    जर (c->लिख->posted) {
        ngx_delete_posted_event(c->लिख);
    }

    c->पठ->closed = 1;
    c->लिख->closed = 1;

    ngx_reusable_connection(c, 0);

    log_error = c->log_error;

    ngx_free_connection(c);

    fd = c->fd;
    c->fd = (ngx_socket_t) -1;

    जर (c->shared) {
        परत;
    }

    जर (ngx_close_socket(fd) == -1) {

        err = ngx_socket_errno;

        जर (err == NGX_ECONNRESET || err == NGX_ENOTCONN) {

            निवड (log_error) {

            स्थिती NGX_ERROR_INFO:
                level = NGX_LOG_INFO;
                थांबा;

            स्थिती NGX_ERROR_ERR:
                level = NGX_LOG_ERR;
                थांबा;

            पूर्वनिर्धारित:
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

    जर (c->reusable) {
        ngx_queue_remove(&c->queue);
        ngx_cycle->reusable_connections_n--;

#जर (NGX_STAT_STUB)
        (शून्य) ngx_atomic_fetch_add(ngx_stat_waiting, -1);
#endif
    }

    c->reusable = reusable;

    जर (reusable) {
        /* need cast as ngx_cycle is volatile */

        ngx_queue_घाला_head(
            (ngx_queue_t *) &ngx_cycle->reusable_connections_queue, &c->queue);
        ngx_cycle->reusable_connections_n++;

#जर (NGX_STAT_STUB)
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

    जर (cycle->free_connection_n > cycle->connection_n / 16
        || cycle->reusable_connections_n == 0)
    {
        परत;
    }

    जर (cycle->connections_reuse_time != ngx_time()) {
        cycle->connections_reuse_time = ngx_time();

        ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                      "%ui worker_connections are not enough, "
                      "reusing connections",
                      cycle->connection_n);
    }

    c = रिक्त;
    n = ngx_max(ngx_min(32, cycle->reusable_connections_n / 8), 1);

    साठी (i = 0; i < n; i++) {
        जर (ngx_queue_empty(&cycle->reusable_connections_queue)) {
            थांबा;
        }

        q = ngx_queue_last(&cycle->reusable_connections_queue);
        c = ngx_queue_data(q, ngx_connection_t, queue);

        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "reusing connection");

        c->बंद = 1;
        c->पठ->handler(c->पठ);
    }

    जर (cycle->free_connection_n == 0 && c && c->reusable) {

        /*
         * if no connections were freed, try to reuse the last
         * connection again: this should free it as long as
         * previous reuse moved it to lingering close
         */

        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "reusing connection again");

        c->बंद = 1;
        c->पठ->handler(c->पठ);
    }
}


शून्य
ngx_close_idle_connections(ngx_cycle_t *cycle)
{
    ngx_uint_t         i;
    ngx_connection_t  *c;

    c = cycle->connections;

    साठी (i = 0; i < cycle->connection_n; i++) {

        /* THREAD: lock */

        जर (c[i].fd != (ngx_socket_t) -1 && c[i].idle) {
            c[i].बंद = 1;
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
#जर (NGX_HAVE_INET6)
    ngx_uint_t            i;
    संरचना sockaddr_in6  *sin6;
#endif

    addr = 0;

    जर (c->local_socklen) {
        निवड (c->local_sockaddr->sa_family) {

#जर (NGX_HAVE_INET6)
        स्थिती AF_INET6:
            sin6 = (संरचना sockaddr_in6 *) c->local_sockaddr;

            साठी (i = 0; addr == 0 && i < 16; i++) {
                addr |= sin6->sin6_addr.s6_addr[i];
            }

            थांबा;
#endif

#जर (NGX_HAVE_UNIX_DOMAIN)
        स्थिती AF_UNIX:
            addr = 1;
            थांबा;
#endif

        पूर्वनिर्धारित: /* AF_INET */
            sin = (संरचना sockaddr_in *) c->local_sockaddr;
            addr = sin->sin_addr.s_addr;
            थांबा;
        }
    }

    जर (addr == 0) {

        len = आकार(ngx_sockaddr_t);

        जर (getsockname(c->fd, &sa.sockaddr, &len) == -1) {
            ngx_connection_error(c, ngx_socket_errno, "getsockname() failed");
            परत NGX_ERROR;
        }

        c->local_sockaddr = ngx_palloc(c->pool, len);
        जर (c->local_sockaddr == रिक्त) {
            परत NGX_ERROR;
        }

        ngx_memcpy(c->local_sockaddr, &sa, len);

        c->local_socklen = len;
    }

    जर (s == रिक्त) {
        परत NGX_OK;
    }

    s->len = ngx_sock_ntop(c->local_sockaddr, c->local_socklen,
                           s->data, s->len, port);

    परत NGX_OK;
}


ngx_int_t
ngx_tcp_nodelay(ngx_connection_t *c)
{
    पूर्णांक  tcp_nodelay;

    जर (c->tcp_nodelay != NGX_TCP_NODELAY_UNSET) {
        परत NGX_OK;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0, "tcp_nodelay");

    tcp_nodelay = 1;

    जर (setsockopt(c->fd, IPPROTO_TCP, TCP_NODELAY,
                   (स्थिरांक शून्य *) &tcp_nodelay, आकार(पूर्णांक))
        == -1)
    {
#जर (NGX_SOLARIS)
        जर (c->log_error == NGX_ERROR_INFO) {

            /* Solaris returns EINVAL if a socket has been shut down */
            c->log_error = NGX_ERROR_IGNORE_EINVAL;

            ngx_connection_error(c, ngx_socket_errno,
                                 "setsockopt(TCP_NODELAY) failed");

            c->log_error = NGX_ERROR_INFO;

            परत NGX_ERROR;
        }
#endif

        ngx_connection_error(c, ngx_socket_errno,
                             "setsockopt(TCP_NODELAY) failed");
        परत NGX_ERROR;
    }

    c->tcp_nodelay = NGX_TCP_NODELAY_SET;

    परत NGX_OK;
}


ngx_int_t
ngx_connection_error(ngx_connection_t *c, ngx_err_t err, अक्षर *text)
{
    ngx_uint_t  level;

    /* Winsock may return NGX_ECONNABORTED instead of NGX_ECONNRESET */

    जर ((err == NGX_ECONNRESET
#जर (NGX_WIN32)
         || err == NGX_ECONNABORTED
#endif
        ) && c->log_error == NGX_ERROR_IGNORE_ECONNRESET)
    {
        परत 0;
    }

#जर (NGX_SOLARIS)
    जर (err == NGX_EINVAL && c->log_error == NGX_ERROR_IGNORE_EINVAL) {
        परत 0;
    }
#endif

    जर (err == NGX_EMSGSIZE && c->log_error == NGX_ERROR_IGNORE_EMSGSIZE) {
        परत 0;
    }

#जर (NGX_LINUX)
    जर (err == NGX_EIO && c->ktls_recv) {
        परत 0;
    }
#endif

    जर (err == 0
        || err == NGX_ECONNRESET
#जर (NGX_WIN32)
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
        निवड (c->log_error) {

        स्थिती NGX_ERROR_IGNORE_EMSGSIZE:
        स्थिती NGX_ERROR_IGNORE_EINVAL:
        स्थिती NGX_ERROR_IGNORE_ECONNRESET:
        स्थिती NGX_ERROR_INFO:
            level = NGX_LOG_INFO;
            थांबा;

        पूर्वनिर्धारित:
            level = NGX_LOG_ERR;
        }

    } अन्यथा {
        level = NGX_LOG_ALERT;
    }

    ngx_log_error(level, c->log, err, text);

    परत NGX_ERROR;
}
