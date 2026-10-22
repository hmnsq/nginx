
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>
#समावेश <ngx_event.h>


ngx_os_io_t  ngx_io;


अटल व्योम ngx_drain_connections(ngx_cycle_t *cycle);


ngx_listening_t *
ngx_create_listening(ngx_conf_t *cf, काष्ठा sockaddr *sockaddr,
    socklen_t socklen)
{
    माप_प्रकार            len;
    ngx_listening_t  *ls;
    काष्ठा sockaddr  *sa;
    u_char            text[NGX_SOCKADDR_STRLEN];

    ls = ngx_array_धकेलो(&cf->cycle->listening);
    अगर (ls == शून्य) {
        वापस शून्य;
    }

    ngx_memzero(ls, माप(ngx_listening_t));

    sa = ngx_palloc(cf->pool, socklen);
    अगर (sa == शून्य) {
        वापस शून्य;
    }

    ngx_memcpy(sa, sockaddr, socklen);

    ls->sockaddr = sa;
    ls->socklen = socklen;

    len = ngx_sock_ntop(sa, socklen, text, NGX_SOCKADDR_STRLEN, 1);
    ls->addr_text.len = len;

    चयन (ls->sockaddr->sa_family) {
#अगर (NGX_HAVE_INET6)
    हाल AF_INET6:
        ls->addr_text_max_len = NGX_INET6_ADDRSTRLEN;
        अवरोध;
#पूर्ण_अगर
#अगर (NGX_HAVE_UNIX_DOMAIN)
    हाल AF_UNIX:
        ls->addr_text_max_len = NGX_UNIX_ADDRSTRLEN;
        len++;
        अवरोध;
#पूर्ण_अगर
    हाल AF_INET:
        ls->addr_text_max_len = NGX_INET_ADDRSTRLEN;
        अवरोध;
    शेष:
        ls->addr_text_max_len = NGX_SOCKADDR_STRLEN;
        अवरोध;
    }

    ls->addr_text.तथ्य = ngx_pnalloc(cf->pool, len);
    अगर (ls->addr_text.तथ्य == शून्य) {
        वापस शून्य;
    }

    ngx_memcpy(ls->addr_text.तथ्य, text, len);

#अगर !(NGX_WIN32)
    ngx_rbtree_init(&ls->rbtree, &ls->sentinel, ngx_udp_rbtree_जोड़ो_मूल्य);
#पूर्ण_अगर

    ls->fd = (ngx_socket_t) -1;
    ls->type = SOCK_STREAM;

    ls->backlog = NGX_LISTEN_BACKLOG;
    ls->rcvbuf = -1;
    ls->sndbuf = -1;

#अगर (NGX_HAVE_SETFIB)
    ls->setfib = -1;
#पूर्ण_अगर

#अगर (NGX_HAVE_TCP_FASTOPEN)
    ls->fastopen = -1;
#पूर्ण_अगर

    वापस ls;
}


ngx_int_t
ngx_clone_listening(ngx_cycle_t *cycle, ngx_listening_t *ls)
{
#अगर (NGX_HAVE_REUSEPORT)

    ngx_int_t         n;
    ngx_core_conf_t  *ccf;
    ngx_listening_t   ols;

    अगर (!ls->reuseport || ls->worker != 0) {
        वापस NGX_OK;
    }

    ols = *ls;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    क्रम (n = 1; n < ccf->worker_processes; n++) {

        /* create a socket for each worker process */

        ls = ngx_array_धकेलो(&cycle->listening);
        अगर (ls == शून्य) {
            वापस NGX_ERROR;
        }

        *ls = ols;
        ls->worker = n;
    }

#पूर्ण_अगर

    वापस NGX_OK;
}


ngx_int_t
ngx_set_inherited_sockets(ngx_cycle_t *cycle)
{
    माप_प्रकार                     len;
    ngx_uint_t                 i;
    ngx_listening_t           *ls;
    socklen_t                  olen;
#अगर (NGX_HAVE_DEFERRED_ACCEPT || NGX_HAVE_TCP_FASTOPEN)
    ngx_err_t                  err;
#पूर्ण_अगर
#अगर (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    काष्ठा accept_filter_arg   af;
#पूर्ण_अगर
#अगर (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)
    पूर्णांक                        timeout;
#पूर्ण_अगर
#अगर (NGX_HAVE_REUSEPORT)
    पूर्णांक                        reuseport;
#पूर्ण_अगर

    ls = cycle->listening.elts;
    क्रम (i = 0; i < cycle->listening.nelts; i++) {

        ls[i].sockaddr = ngx_palloc(cycle->pool, माप(ngx_sockaddr_t));
        अगर (ls[i].sockaddr == शून्य) {
            वापस NGX_ERROR;
        }

        ls[i].socklen = माप(ngx_sockaddr_t);
        अगर (getsockname(ls[i].fd, ls[i].sockaddr, &ls[i].socklen) == -1) {
            ngx_log_error(NGX_LOG_CRIT, cycle->log, ngx_socket_errno,
                          "getsockname() of the inherited "
                          "socket #%d failed", ls[i].fd);
            ls[i].ignore = 1;
            जारी;
        }

        अगर (ls[i].socklen > (socklen_t) माप(ngx_sockaddr_t)) {
            ls[i].socklen = माप(ngx_sockaddr_t);
        }

        चयन (ls[i].sockaddr->sa_family) {

#अगर (NGX_HAVE_INET6)
        हाल AF_INET6:
            ls[i].addr_text_max_len = NGX_INET6_ADDRSTRLEN;
            len = NGX_INET6_ADDRSTRLEN + माप("[]:65535") - 1;
            अवरोध;
#पूर्ण_अगर

#अगर (NGX_HAVE_UNIX_DOMAIN)
        हाल AF_UNIX:
            ls[i].addr_text_max_len = NGX_UNIX_ADDRSTRLEN;
            len = NGX_UNIX_ADDRSTRLEN;
            अवरोध;
#पूर्ण_अगर

        हाल AF_INET:
            ls[i].addr_text_max_len = NGX_INET_ADDRSTRLEN;
            len = NGX_INET_ADDRSTRLEN + माप(":65535") - 1;
            अवरोध;

        शेष:
            ngx_log_error(NGX_LOG_CRIT, cycle->log, ngx_socket_errno,
                          "the inherited socket #%d has "
                          "an unsupported protocol family", ls[i].fd);
            ls[i].ignore = 1;
            जारी;
        }

        ls[i].addr_text.तथ्य = ngx_pnalloc(cycle->pool, len);
        अगर (ls[i].addr_text.तथ्य == शून्य) {
            वापस NGX_ERROR;
        }

        len = ngx_sock_ntop(ls[i].sockaddr, ls[i].socklen,
                            ls[i].addr_text.तथ्य, len, 1);
        अगर (len == 0) {
            वापस NGX_ERROR;
        }

        ls[i].addr_text.len = len;

        ls[i].backlog = NGX_LISTEN_BACKLOG;

        olen = माप(पूर्णांक);

        अगर (getsockopt(ls[i].fd, SOL_SOCKET, SO_TYPE, (व्योम *) &ls[i].type,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_CRIT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_TYPE) %V failed", &ls[i].addr_text);
            ls[i].ignore = 1;
            जारी;
        }

        olen = माप(पूर्णांक);

        अगर (getsockopt(ls[i].fd, SOL_SOCKET, SO_RCVBUF, (व्योम *) &ls[i].rcvbuf,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_RCVBUF) %V failed, ignored",
                          &ls[i].addr_text);

            ls[i].rcvbuf = -1;
        }

        olen = माप(पूर्णांक);

        अगर (getsockopt(ls[i].fd, SOL_SOCKET, SO_SNDBUF, (व्योम *) &ls[i].sndbuf,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_SNDBUF) %V failed, ignored",
                          &ls[i].addr_text);

            ls[i].sndbuf = -1;
        }

#अगर 0
        /* SO_SETFIB is currently a set only option */

#अगर (NGX_HAVE_SETFIB)

        olen = माप(पूर्णांक);

        अगर (getsockopt(ls[i].fd, SOL_SOCKET, SO_SETFIB,
                       (व्योम *) &ls[i].setfib, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_SETFIB) %V failed, ignored",
                          &ls[i].addr_text);

            ls[i].setfib = -1;
        }

#पूर्ण_अगर
#पूर्ण_अगर

#अगर (NGX_HAVE_REUSEPORT)

        reuseport = 0;
        olen = माप(पूर्णांक);

#अगर_घोषित SO_REUSEPORT_LB

        अगर (getsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT_LB,
                       (व्योम *) &reuseport, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_REUSEPORT_LB) %V failed, ignored",
                          &ls[i].addr_text);

        } अन्यथा {
            ls[i].reuseport = reuseport ? 1 : 0;
        }

#अन्यथा

        अगर (getsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT,
                       (व्योम *) &reuseport, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_REUSEPORT) %V failed, ignored",
                          &ls[i].addr_text);

        } अन्यथा {
            ls[i].reuseport = reuseport ? 1 : 0;
        }
#पूर्ण_अगर

#पूर्ण_अगर

        अगर (ls[i].type != SOCK_STREAM) {
            जारी;
        }

#अगर_घोषित SO_PROTOCOL

        olen = माप(पूर्णांक);

        अगर (getsockopt(ls[i].fd, SOL_SOCKET, SO_PROTOCOL,
                       (व्योम *) &ls[i].protocol, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                          "getsockopt(SO_PROTOCOL) %V failed, ignored",
                          &ls[i].addr_text);
            ls[i].protocol = 0;

        } अन्यथा अगर (ls[i].protocol == IPPROTO_TCP) {
            ls[i].protocol = 0;
        }

#पूर्ण_अगर

#अगर (NGX_HAVE_TCP_FASTOPEN)

        olen = माप(पूर्णांक);

        अगर (getsockopt(ls[i].fd, IPPROTO_TCP, TCP_FASTOPEN,
                       (व्योम *) &ls[i].fastopen, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            अगर (err != NGX_EOPNOTSUPP && err != NGX_ENOPROTOOPT
                && err != NGX_EINVAL)
            {
                ngx_log_error(NGX_LOG_NOTICE, cycle->log, err,
                              "getsockopt(TCP_FASTOPEN) %V failed, ignored",
                              &ls[i].addr_text);
            }

            ls[i].fastopen = -1;
        }

#पूर्ण_अगर

#अगर (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)

        ngx_memzero(&af, माप(काष्ठा accept_filter_arg));
        olen = माप(काष्ठा accept_filter_arg);

        अगर (getsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER, &af, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            अगर (err == NGX_EINVAL) {
                जारी;
            }

            ngx_log_error(NGX_LOG_NOTICE, cycle->log, err,
                          "getsockopt(SO_ACCEPTFILTER) for %V failed, ignored",
                          &ls[i].addr_text);
            जारी;
        }

        अगर (olen < माप(काष्ठा accept_filter_arg) || af.af_name[0] == '\0') {
            जारी;
        }

        ls[i].accept_filter = ngx_palloc(cycle->pool, 16);
        अगर (ls[i].accept_filter == शून्य) {
            वापस NGX_ERROR;
        }

        (व्योम) ngx_cpystrn((u_char *) ls[i].accept_filter,
                           (u_char *) af.af_name, 16);
#पूर्ण_अगर

#अगर (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)

        timeout = 0;
        olen = माप(पूर्णांक);

        अगर (getsockopt(ls[i].fd, IPPROTO_TCP, TCP_DEFER_ACCEPT, &timeout, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            अगर (err == NGX_EOPNOTSUPP) {
                जारी;
            }

            ngx_log_error(NGX_LOG_NOTICE, cycle->log, err,
                          "getsockopt(TCP_DEFER_ACCEPT) for %V failed, ignored",
                          &ls[i].addr_text);
            जारी;
        }

        अगर (olen < माप(पूर्णांक) || timeout == 0) {
            जारी;
        }

        ls[i].deferred_accept = 1;
#पूर्ण_अगर
    }

    वापस NGX_OK;
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
#अगर (NGX_SUPPRESS_WARN)
    failed = 0;
#पूर्ण_अगर

    log = cycle->log;

    /* TODO: configurable try number */

    क्रम (tries = 5; tries; tries--) {
        failed = 0;

        /* for each listening socket */

        ls = cycle->listening.elts;
        क्रम (i = 0; i < cycle->listening.nelts; i++) {

            अगर (ls[i].ignore) {
                जारी;
            }

#अगर (NGX_HAVE_REUSEPORT)

            अगर (ls[i].add_reuseport || ls[i].change_protocol) {

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

#अगर_घोषित SO_REUSEPORT_LB

                अगर (setsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT_LB,
                               (स्थिर व्योम *) &reuseport, माप(पूर्णांक))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT_LB) %V failed, "
                                  "ignored",
                                  &ls[i].addr_text);
                }

#अन्यथा

                अगर (setsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT,
                               (स्थिर व्योम *) &reuseport, माप(पूर्णांक))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT) %V failed, ignored",
                                  &ls[i].addr_text);
                }
#पूर्ण_अगर

                ls[i].add_reuseport = 0;
            }
#पूर्ण_अगर

            अगर (ls[i].fd != (ngx_socket_t) -1 && !ls[i].change_protocol) {
                जारी;
            }

            अगर (ls[i].inherited) {

                /* TODO: close on exit */
                /* TODO: nonblocking */
                /* TODO: deferred accept */

                जारी;
            }

            s = ngx_socket(ls[i].sockaddr->sa_family, ls[i].type,
                           ls[i].protocol);

            अगर (s == (ngx_socket_t) -1) {
                ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                              ngx_socket_n " %V failed", &ls[i].addr_text);
                वापस NGX_ERROR;
            }

            अगर (ls[i].type != SOCK_DGRAM || !ngx_test_config) {

                अगर (setsockopt(s, SOL_SOCKET, SO_REUSEADDR,
                               (स्थिर व्योम *) &reuseaddr, माप(पूर्णांक))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEADDR) %V failed",
                                  &ls[i].addr_text);

                    अगर (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_text);
                    }

                    वापस NGX_ERROR;
                }
            }

#अगर (NGX_HAVE_REUSEPORT)

            अगर ((ls[i].reuseport || ls[i].change_protocol)
                && !ngx_test_config)
            {
                पूर्णांक  reuseport;

                reuseport = 1;

#अगर_घोषित SO_REUSEPORT_LB

                अगर (setsockopt(s, SOL_SOCKET, SO_REUSEPORT_LB,
                               (स्थिर व्योम *) &reuseport, माप(पूर्णांक))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT_LB) %V failed",
                                  &ls[i].addr_text);

                    अगर (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_text);
                    }

                    वापस NGX_ERROR;
                }

#अन्यथा

                अगर (setsockopt(s, SOL_SOCKET, SO_REUSEPORT,
                               (स्थिर व्योम *) &reuseport, माप(पूर्णांक))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT) %V failed",
                                  &ls[i].addr_text);

                    अगर (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_text);
                    }

                    वापस NGX_ERROR;
                }
#पूर्ण_अगर
            }
#पूर्ण_अगर

#अगर (NGX_HAVE_INET6 && defined IPV6_V6ONLY)

            अगर (ls[i].sockaddr->sa_family == AF_INET6) {
                पूर्णांक  ipv6only;

                ipv6only = ls[i].ipv6only;

                अगर (setsockopt(s, IPPROTO_IPV6, IPV6_V6ONLY,
                               (स्थिर व्योम *) &ipv6only, माप(पूर्णांक))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  "setsockopt(IPV6_V6ONLY) %V failed, ignored",
                                  &ls[i].addr_text);
                }
            }
#पूर्ण_अगर
            /* TODO: close on exit */

            अगर (ngx_nonblocking(s) == -1) {
                ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                              ngx_nonblocking_n " %V failed",
                              &ls[i].addr_text);

                अगर (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_text);
                }

                वापस NGX_ERROR;
            }

            ngx_log_debug2(NGX_LOG_DEBUG_CORE, log, 0,
                           "bind() %V #%d ", &ls[i].addr_text, s);

            अगर (bind(s, ls[i].sockaddr, ls[i].socklen) == -1) {
                err = ngx_socket_errno;

                अगर (err != NGX_EADDRINUSE || !ngx_test_config) {
                    ngx_log_error(NGX_LOG_EMERG, log, err,
                                  "bind() to %V failed", &ls[i].addr_text);
                }

                अगर (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_text);
                }

                अगर (err != NGX_EADDRINUSE) {
                    वापस NGX_ERROR;
                }

                अगर (!ngx_test_config) {
                    failed = 1;
                }

                जारी;
            }

#अगर (NGX_HAVE_UNIX_DOMAIN)

            अगर (ls[i].sockaddr->sa_family == AF_UNIX) {
                mode_t   mode;
                u_char  *name;

                name = ls[i].addr_text.तथ्य + माप("unix:") - 1;
                mode = (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);

                अगर (chmod((अक्षर *) name, mode) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  "chmod() \"%s\" failed", name);
                }

                अगर (ngx_test_config) {
                    अगर (ngx_delete_file(name) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                      ngx_delete_file_n " %s failed", name);
                    }
                }
            }
#पूर्ण_अगर

            अगर (ls[i].type != SOCK_STREAM) {
                ls[i].fd = s;
                ls[i].खोलो = 1;
                जारी;
            }

            अगर (listen(s, ls[i].backlog) == -1) {
                err = ngx_socket_errno;

                /*
                 * on OpenVZ after suspend/resume EADDRINUSE
                 * may be returned by listen() instead of bind(), see
                 * https://bugs.openvz.org/browse/OVZ-5587
                 */

                अगर (err != NGX_EADDRINUSE || !ngx_test_config) {
                    ngx_log_error(NGX_LOG_EMERG, log, err,
                                  "listen() to %V, backlog %d failed",
                                  &ls[i].addr_text, ls[i].backlog);
                }

                अगर (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_text);
                }

                अगर (err != NGX_EADDRINUSE) {
                    वापस NGX_ERROR;
                }

                अगर (!ngx_test_config) {
                    failed = 1;
                }

                जारी;
            }

            ls[i].listen = 1;

            ls[i].fd = s;
            ls[i].खोलो = 1;
        }

        अगर (!failed) {
            अवरोध;
        }

        /* TODO: delay configurable */

        ngx_log_error(NGX_LOG_NOTICE, log, 0,
                      "try again to bind() after 500ms");

        ngx_msleep(500);
    }

    अगर (failed) {
        ngx_log_error(NGX_LOG_EMERG, log, 0, "still could not bind()");
        वापस NGX_ERROR;
    }

    वापस NGX_OK;
}


व्योम
ngx_configure_listening_sockets(ngx_cycle_t *cycle)
{
    पूर्णांक                        मूल्य;
    ngx_uint_t                 i;
    ngx_listening_t           *ls;

#अगर (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    काष्ठा accept_filter_arg   af;
#पूर्ण_अगर

    ls = cycle->listening.elts;
    क्रम (i = 0; i < cycle->listening.nelts; i++) {

        ls[i].log = *ls[i].logp;

        अगर (ls[i].rcvbuf != -1) {
            अगर (setsockopt(ls[i].fd, SOL_SOCKET, SO_RCVBUF,
                           (स्थिर व्योम *) &ls[i].rcvbuf, माप(पूर्णांक))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_RCVBUF, %d) %V failed, ignored",
                              ls[i].rcvbuf, &ls[i].addr_text);
            }
        }

        अगर (ls[i].sndbuf != -1) {
            अगर (setsockopt(ls[i].fd, SOL_SOCKET, SO_SNDBUF,
                           (स्थिर व्योम *) &ls[i].sndbuf, माप(पूर्णांक))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_SNDBUF, %d) %V failed, ignored",
                              ls[i].sndbuf, &ls[i].addr_text);
            }
        }

        अगर (ls[i].keepalive) {
            मूल्य = (ls[i].keepalive == 1) ? 1 : 0;

            अगर (setsockopt(ls[i].fd, SOL_SOCKET, SO_KEEPALIVE,
                           (स्थिर व्योम *) &मूल्य, माप(पूर्णांक))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_KEEPALIVE, %d) %V failed, ignored",
                              मूल्य, &ls[i].addr_text);
            }
        }

#अगर (NGX_HAVE_KEEPALIVE_TUNABLE)

#अगर !(NGX_DARWIN)

        अगर (ls[i].keepidle) {
            मूल्य = ls[i].keepidle;

#अगर (NGX_KEEPALIVE_FACTOR)
            मूल्य *= NGX_KEEPALIVE_FACTOR;
#पूर्ण_अगर

            अगर (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPIDLE,
                           (स्थिर व्योम *) &मूल्य, माप(पूर्णांक))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_KEEPIDLE, %d) %V failed, ignored",
                              मूल्य, &ls[i].addr_text);
            }
        }

#पूर्ण_अगर

        अगर (ls[i].keepintvl) {
            मूल्य = ls[i].keepintvl;

#अगर (NGX_KEEPALIVE_FACTOR)
            मूल्य *= NGX_KEEPALIVE_FACTOR;
#पूर्ण_अगर

            अगर (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPINTVL,
                           (स्थिर व्योम *) &मूल्य, माप(पूर्णांक))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                             "setsockopt(TCP_KEEPINTVL, %d) %V failed, ignored",
                             मूल्य, &ls[i].addr_text);
            }
        }

        अगर (ls[i].keepcnt) {
            अगर (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPCNT,
                           (स्थिर व्योम *) &ls[i].keepcnt, माप(पूर्णांक))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_KEEPCNT, %d) %V failed, ignored",
                              ls[i].keepcnt, &ls[i].addr_text);
            }
        }

#पूर्ण_अगर

#अगर (NGX_HAVE_SETFIB)
        अगर (ls[i].setfib != -1) {
            अगर (setsockopt(ls[i].fd, SOL_SOCKET, SO_SETFIB,
                           (स्थिर व्योम *) &ls[i].setfib, माप(पूर्णांक))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_SETFIB, %d) %V failed, ignored",
                              ls[i].setfib, &ls[i].addr_text);
            }
        }
#पूर्ण_अगर

#अगर (NGX_HAVE_TCP_FASTOPEN)
        अगर (ls[i].fastopen != -1) {
            अगर (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_FASTOPEN,
                           (स्थिर व्योम *) &ls[i].fastopen, माप(पूर्णांक))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_FASTOPEN, %d) %V failed, ignored",
                              ls[i].fastopen, &ls[i].addr_text);
            }
        }
#पूर्ण_अगर

#अगर 0
        अगर (1) {
            पूर्णांक tcp_nodelay = 1;

            अगर (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_NODELAY,
                       (स्थिर व्योम *) &tcp_nodelay, माप(पूर्णांक))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_NODELAY) %V failed, ignored",
                              &ls[i].addr_text);
            }
        }
#पूर्ण_अगर

        अगर (ls[i].listen) {

            /* change backlog via listen() */

            अगर (listen(ls[i].fd, ls[i].backlog) == -1) {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "listen() to %V, backlog %d failed, ignored",
                              &ls[i].addr_text, ls[i].backlog);
            }
        }

        /*
         * setting deferred mode should be last operation on socket,
         * because code may prematurely continue cycle on failure
         */

#अगर (NGX_HAVE_DEFERRED_ACCEPT)

#अगर_घोषित SO_ACCEPTFILTER

        अगर (ls[i].delete_deferred) {
            अगर (setsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER, शून्य, 0)
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_ACCEPTFILTER, NULL) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);

                अगर (ls[i].accept_filter) {
                    ngx_log_error(NGX_LOG_ALERT, cycle->log, 0,
                                  "could not change the accept filter "
                                  "to \"%s\" for %V, ignored",
                                  ls[i].accept_filter, &ls[i].addr_text);
                }

                जारी;
            }

            ls[i].deferred_accept = 0;
        }

        अगर (ls[i].add_deferred) {
            ngx_memzero(&af, माप(काष्ठा accept_filter_arg));
            (व्योम) ngx_cpystrn((u_char *) af.af_name,
                               (u_char *) ls[i].accept_filter, 16);

            अगर (setsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER,
                           &af, माप(काष्ठा accept_filter_arg))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(SO_ACCEPTFILTER, \"%s\") "
                              "for %V failed, ignored",
                              ls[i].accept_filter, &ls[i].addr_text);
                जारी;
            }

            ls[i].deferred_accept = 1;
        }

#पूर्ण_अगर

#अगर_घोषित TCP_DEFER_ACCEPT

        अगर (ls[i].add_deferred || ls[i].delete_deferred) {

            अगर (ls[i].add_deferred) {
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

            अगर (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_DEFER_ACCEPT,
                           &मूल्य, माप(पूर्णांक))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(TCP_DEFER_ACCEPT, %d) for %V failed, "
                              "ignored",
                              मूल्य, &ls[i].addr_text);

                जारी;
            }
        }

        अगर (ls[i].add_deferred) {
            ls[i].deferred_accept = 1;
        }

#पूर्ण_अगर

#पूर्ण_अगर /* NGX_HAVE_DEFERRED_ACCEPT */

#अगर (NGX_HAVE_IP_RECVDSTADDR)

        अगर (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET)
        {
            मूल्य = 1;

            अगर (setsockopt(ls[i].fd, IPPROTO_IP, IP_RECVDSTADDR,
                           (स्थिर व्योम *) &मूल्य, माप(पूर्णांक))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_RECVDSTADDR) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#या_अगर (NGX_HAVE_IP_PKTINFO)

        अगर (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET)
        {
            मूल्य = 1;

            अगर (setsockopt(ls[i].fd, IPPROTO_IP, IP_PKTINFO,
                           (स्थिर व्योम *) &मूल्य, माप(पूर्णांक))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_PKTINFO) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#पूर्ण_अगर

#अगर (NGX_HAVE_INET6 && NGX_HAVE_IPV6_RECVPKTINFO)

        अगर (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET6)
        {
            मूल्य = 1;

            अगर (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_RECVPKTINFO,
                           (स्थिर व्योम *) &मूल्य, माप(पूर्णांक))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IPV6_RECVPKTINFO) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#पूर्ण_अगर

#अगर (NGX_HAVE_IP_MTU_DISCOVER)

        अगर (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET) {
            मूल्य = IP_PMTUDISC_DO;

            अगर (setsockopt(ls[i].fd, IPPROTO_IP, IP_MTU_DISCOVER,
                           (स्थिर व्योम *) &मूल्य, माप(पूर्णांक))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_MTU_DISCOVER) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#या_अगर (NGX_HAVE_IP_DONTFRAG)

        अगर (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET) {
            मूल्य = 1;

            अगर (setsockopt(ls[i].fd, IPPROTO_IP, IP_DONTFRAG,
                           (स्थिर व्योम *) &मूल्य, माप(पूर्णांक))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IP_DONTFRAG) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#पूर्ण_अगर

#अगर (NGX_HAVE_INET6)

#अगर (NGX_HAVE_IPV6_MTU_DISCOVER)

        अगर (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET6) {
            मूल्य = IPV6_PMTUDISC_DO;

            अगर (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_MTU_DISCOVER,
                           (स्थिर व्योम *) &मूल्य, माप(पूर्णांक))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IPV6_MTU_DISCOVER) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#या_अगर (NGX_HAVE_IPV6_DONTFRAG)

        अगर (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET6) {
            मूल्य = 1;

            अगर (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_DONTFRAG,
                           (स्थिर व्योम *) &मूल्य, माप(पूर्णांक))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_socket_errno,
                              "setsockopt(IPV6_DONTFRAG) "
                              "for %V failed, ignored",
                              &ls[i].addr_text);
            }
        }

#पूर्ण_अगर

#पूर्ण_अगर
    }

    वापस;
}


व्योम
ngx_close_listening_sockets(ngx_cycle_t *cycle)
{
    ngx_uint_t         i;
    ngx_listening_t   *ls;
    ngx_connection_t  *c;

    ngx_accept_mutex_held = 0;
    ngx_use_accept_mutex = 0;

    ls = cycle->listening.elts;
    क्रम (i = 0; i < cycle->listening.nelts; i++) {

#अगर (NGX_QUIC)
        अगर (ls[i].quic
            && (!ls[i].reuseport || !ngx_quic_bpf_enabled(cycle)))
        {
            जारी;
        }
#पूर्ण_अगर

        c = ls[i].connection;

        अगर (c) {
            अगर (c->पढ़ो->active) {
                अगर (ngx_event_flags & NGX_USE_EPOLL_EVENT) {

                    /*
                     * it seems that Linux-2.6.x OpenVZ sends events
                     * for closed shared listening sockets unless
                     * the events was explicitly deleted
                     */

                    ngx_del_event(c->पढ़ो, NGX_READ_EVENT, 0);

                } अन्यथा {
                    ngx_del_event(c->पढ़ो, NGX_READ_EVENT, NGX_CLOSE_EVENT);
                }
            }

            अगर (c->पढ़ो->timer_set) {
                ngx_del_timer(c->पढ़ो);
            }

            ngx_free_connection(c);

            c->fd = (ngx_socket_t) -1;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, cycle->log, 0,
                       "close listening %V #%d ", &ls[i].addr_text, ls[i].fd);

        अगर (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_socket_errno,
                          ngx_close_socket_n " %V failed", &ls[i].addr_text);
        }

#अगर (NGX_HAVE_UNIX_DOMAIN)

        अगर (ls[i].sockaddr->sa_family == AF_UNIX
            && ngx_process <= NGX_PROCESS_MASTER
            && ngx_new_द्विआधारी == 0
            && (!ls[i].inherited || ngx_getppid() != ngx_parent))
        {
            u_char *name = ls[i].addr_text.तथ्य + माप("unix:") - 1;

            अगर (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_socket_errno,
                              ngx_delete_file_n " %s failed", name);
            }
        }

#पूर्ण_अगर

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

    अगर (ngx_cycle->files && (ngx_uint_t) s >= ngx_cycle->files_n) {
        ngx_log_error(NGX_LOG_ALERT, log, 0,
                      "the new socket has number %d, "
                      "but only %ui files are available",
                      s, ngx_cycle->files_n);
        वापस शून्य;
    }

    ngx_drain_connections((ngx_cycle_t *) ngx_cycle);

    c = ngx_cycle->free_connections;

    अगर (c == शून्य) {
        ngx_log_error(NGX_LOG_ALERT, log, 0,
                      "%ui worker_connections are not enough",
                      ngx_cycle->connection_n);

        वापस शून्य;
    }

    ngx_cycle->free_connections = c->तथ्य;
    ngx_cycle->free_connection_n--;

    अगर (ngx_cycle->files && ngx_cycle->files[s] == शून्य) {
        ngx_cycle->files[s] = c;
    }

    rev = c->पढ़ो;
    wev = c->ग_लिखो;

    ngx_memzero(c, माप(ngx_connection_t));

    c->पढ़ो = rev;
    c->ग_लिखो = wev;
    c->fd = s;
    c->log = log;

    instance = rev->instance;

    ngx_memzero(rev, माप(ngx_event_t));
    ngx_memzero(wev, माप(ngx_event_t));

    rev->instance = !instance;
    wev->instance = !instance;

    rev->सूचकांक = NGX_INVALID_सूचकांक;
    wev->सूचकांक = NGX_INVALID_सूचकांक;

    rev->तथ्य = c;
    wev->तथ्य = c;

    wev->ग_लिखो = 1;

    वापस c;
}


व्योम
ngx_free_connection(ngx_connection_t *c)
{
    c->तथ्य = ngx_cycle->free_connections;
    ngx_cycle->free_connections = c;
    ngx_cycle->free_connection_n++;

    अगर (ngx_cycle->files && ngx_cycle->files[c->fd] == c) {
        ngx_cycle->files[c->fd] = शून्य;
    }
}


व्योम
ngx_close_connection(ngx_connection_t *c)
{
    ngx_err_t     err;
    ngx_uint_t    log_error, level;
    ngx_socket_t  fd;

    अगर (c->fd == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0, "connection already closed");
        वापस;
    }

    अगर (c->पढ़ो->timer_set) {
        ngx_del_timer(c->पढ़ो);
    }

    अगर (c->ग_लिखो->timer_set) {
        ngx_del_timer(c->ग_लिखो);
    }

    अगर (!c->shared) {
        अगर (ngx_del_conn) {
            ngx_del_conn(c, NGX_CLOSE_EVENT);

        } अन्यथा {
            अगर (c->पढ़ो->active || c->पढ़ो->disabled) {
                ngx_del_event(c->पढ़ो, NGX_READ_EVENT, NGX_CLOSE_EVENT);
            }

            अगर (c->ग_लिखो->active || c->ग_लिखो->disabled) {
                ngx_del_event(c->ग_लिखो, NGX_WRITE_EVENT, NGX_CLOSE_EVENT);
            }
        }
    }

    अगर (c->पढ़ो->posted) {
        ngx_delete_posted_event(c->पढ़ो);
    }

    अगर (c->ग_लिखो->posted) {
        ngx_delete_posted_event(c->ग_लिखो);
    }

    c->पढ़ो->closed = 1;
    c->ग_लिखो->closed = 1;

    ngx_reusable_connection(c, 0);

    log_error = c->log_error;

    ngx_free_connection(c);

    fd = c->fd;
    c->fd = (ngx_socket_t) -1;

    अगर (c->shared) {
        वापस;
    }

    अगर (ngx_close_socket(fd) == -1) {

        err = ngx_socket_errno;

        अगर (err == NGX_ECONNRESET || err == NGX_ENOTCONN) {

            चयन (log_error) {

            हाल NGX_ERROR_INFO:
                level = NGX_LOG_INFO;
                अवरोध;

            हाल NGX_ERROR_ERR:
                level = NGX_LOG_ERR;
                अवरोध;

            शेष:
                level = NGX_LOG_CRIT;
            }

        } अन्यथा {
            level = NGX_LOG_CRIT;
        }

        ngx_log_error(level, c->log, err, ngx_close_socket_n " %d failed", fd);
    }
}


व्योम
ngx_reusable_connection(ngx_connection_t *c, ngx_uint_t reusable)
{
    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "reusable connection: %ui", reusable);

    अगर (c->reusable) {
        ngx_queue_remove(&c->queue);
        ngx_cycle->reusable_connections_n--;

#अगर (NGX_STAT_STUB)
        (व्योम) ngx_atomic_fetch_add(ngx_stat_waiting, -1);
#पूर्ण_अगर
    }

    c->reusable = reusable;

    अगर (reusable) {
        /* need cast as ngx_cycle is volatile */

        ngx_queue_जोड़ो_शीर्ष(
            (ngx_queue_t *) &ngx_cycle->reusable_connections_queue, &c->queue);
        ngx_cycle->reusable_connections_n++;

#अगर (NGX_STAT_STUB)
        (व्योम) ngx_atomic_fetch_add(ngx_stat_waiting, 1);
#पूर्ण_अगर
    }
}


अटल व्योम
ngx_drain_connections(ngx_cycle_t *cycle)
{
    ngx_uint_t         i, n;
    ngx_queue_t       *q;
    ngx_connection_t  *c;

    अगर (cycle->free_connection_n > cycle->connection_n / 16
        || cycle->reusable_connections_n == 0)
    {
        वापस;
    }

    अगर (cycle->connections_reuse_time != ngx_time()) {
        cycle->connections_reuse_time = ngx_time();

        ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                      "%ui worker_connections are not enough, "
                      "reusing connections",
                      cycle->connection_n);
    }

    c = शून्य;
    n = ngx_max(ngx_min(32, cycle->reusable_connections_n / 8), 1);

    क्रम (i = 0; i < n; i++) {
        अगर (ngx_queue_empty(&cycle->reusable_connections_queue)) {
            अवरोध;
        }

        q = ngx_queue_last(&cycle->reusable_connections_queue);
        c = ngx_queue_तथ्य(q, ngx_connection_t, queue);

        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "reusing connection");

        c->बंद = 1;
        c->पढ़ो->handler(c->पढ़ो);
    }

    अगर (cycle->free_connection_n == 0 && c && c->reusable) {

        /*
         * if no connections were freed, try to reuse the last
         * connection again: this should free it as long as
         * previous reuse moved it to lingering close
         */

        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "reusing connection again");

        c->बंद = 1;
        c->पढ़ो->handler(c->पढ़ो);
    }
}


व्योम
ngx_close_idle_connections(ngx_cycle_t *cycle)
{
    ngx_uint_t         i;
    ngx_connection_t  *c;

    c = cycle->connections;

    क्रम (i = 0; i < cycle->connection_n; i++) {

        /* THREAD: lock */

        अगर (c[i].fd != (ngx_socket_t) -1 && c[i].idle) {
            c[i].बंद = 1;
            c[i].पढ़ो->handler(c[i].पढ़ो);
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
    काष्ठा sockaddr_in   *sin;
#अगर (NGX_HAVE_INET6)
    ngx_uint_t            i;
    काष्ठा sockaddr_in6  *sin6;
#पूर्ण_अगर

    addr = 0;

    अगर (c->local_socklen) {
        चयन (c->local_sockaddr->sa_family) {

#अगर (NGX_HAVE_INET6)
        हाल AF_INET6:
            sin6 = (काष्ठा sockaddr_in6 *) c->local_sockaddr;

            क्रम (i = 0; addr == 0 && i < 16; i++) {
                addr |= sin6->sin6_addr.s6_addr[i];
            }

            अवरोध;
#पूर्ण_अगर

#अगर (NGX_HAVE_UNIX_DOMAIN)
        हाल AF_UNIX:
            addr = 1;
            अवरोध;
#पूर्ण_अगर

        शेष: /* AF_INET */
            sin = (काष्ठा sockaddr_in *) c->local_sockaddr;
            addr = sin->sin_addr.s_addr;
            अवरोध;
        }
    }

    अगर (addr == 0) {

        len = माप(ngx_sockaddr_t);

        अगर (getsockname(c->fd, &sa.sockaddr, &len) == -1) {
            ngx_connection_error(c, ngx_socket_errno, "getsockname() failed");
            वापस NGX_ERROR;
        }

        c->local_sockaddr = ngx_palloc(c->pool, len);
        अगर (c->local_sockaddr == शून्य) {
            वापस NGX_ERROR;
        }

        ngx_memcpy(c->local_sockaddr, &sa, len);

        c->local_socklen = len;
    }

    अगर (s == शून्य) {
        वापस NGX_OK;
    }

    s->len = ngx_sock_ntop(c->local_sockaddr, c->local_socklen,
                           s->तथ्य, s->len, port);

    वापस NGX_OK;
}


ngx_int_t
ngx_tcp_nodelay(ngx_connection_t *c)
{
    पूर्णांक  tcp_nodelay;

    अगर (c->tcp_nodelay != NGX_TCP_NODELAY_UNSET) {
        वापस NGX_OK;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0, "tcp_nodelay");

    tcp_nodelay = 1;

    अगर (setsockopt(c->fd, IPPROTO_TCP, TCP_NODELAY,
                   (स्थिर व्योम *) &tcp_nodelay, माप(पूर्णांक))
        == -1)
    {
#अगर (NGX_SOLARIS)
        अगर (c->log_error == NGX_ERROR_INFO) {

            /* Solaris returns EINVAL if a socket has been shut down */
            c->log_error = NGX_ERROR_IGNORE_EINVAL;

            ngx_connection_error(c, ngx_socket_errno,
                                 "setsockopt(TCP_NODELAY) failed");

            c->log_error = NGX_ERROR_INFO;

            वापस NGX_ERROR;
        }
#पूर्ण_अगर

        ngx_connection_error(c, ngx_socket_errno,
                             "setsockopt(TCP_NODELAY) failed");
        वापस NGX_ERROR;
    }

    c->tcp_nodelay = NGX_TCP_NODELAY_SET;

    वापस NGX_OK;
}


ngx_int_t
ngx_connection_error(ngx_connection_t *c, ngx_err_t err, अक्षर *text)
{
    ngx_uint_t  level;

    /* Winsock may return NGX_ECONNABORTED instead of NGX_ECONNRESET */

    अगर ((err == NGX_ECONNRESET
#अगर (NGX_WIN32)
         || err == NGX_ECONNABORTED
#पूर्ण_अगर
        ) && c->log_error == NGX_ERROR_IGNORE_ECONNRESET)
    {
        वापस 0;
    }

#अगर (NGX_SOLARIS)
    अगर (err == NGX_EINVAL && c->log_error == NGX_ERROR_IGNORE_EINVAL) {
        वापस 0;
    }
#पूर्ण_अगर

    अगर (err == NGX_EMSGSIZE && c->log_error == NGX_ERROR_IGNORE_EMSGSIZE) {
        वापस 0;
    }

#अगर (NGX_LINUX)
    अगर (err == NGX_EIO && c->ktls_recv) {
        वापस 0;
    }
#पूर्ण_अगर

    अगर (err == 0
        || err == NGX_ECONNRESET
#अगर (NGX_WIN32)
        || err == NGX_ECONNABORTED
#अन्यथा
        || err == NGX_EPIPE
#पूर्ण_अगर
        || err == NGX_ENOTCONN
        || err == NGX_ETIMEDOUT
        || err == NGX_ECONNREFUSED
        || err == NGX_ENETDOWN
        || err == NGX_ENETUNREACH
        || err == NGX_EHOSTDOWN
        || err == NGX_EHOSTUNREACH)
    {
        चयन (c->log_error) {

        हाल NGX_ERROR_IGNORE_EMSGSIZE:
        हाल NGX_ERROR_IGNORE_EINVAL:
        हाल NGX_ERROR_IGNORE_ECONNRESET:
        हाल NGX_ERROR_INFO:
            level = NGX_LOG_INFO;
            अवरोध;

        शेष:
            level = NGX_LOG_ERR;
        }

    } अन्यथा {
        level = NGX_LOG_ALERT;
    }

    ngx_log_error(level, c->log, err, text);

    वापस NGX_ERROR;
}
