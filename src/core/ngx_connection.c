
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>
#ಅನ್ತರ್ಭಾವ <ngx_event.h>


ngx_os_io_t  ngx_io;


ಸ್ಥಿರ ಶೂನ್ಯ ngx_drain_connections(ngx_ಚಕ್ರ_t *ಚಕ್ರ);


ngx_listening_t *
ngx_create_listening(ngx_conf_t *cf, ರಚನೆ sockaddr *sockaddr,
    socklen_t socklen)
{
    ಗಾತ್ರ_t            len;
    ngx_listening_t  *ls;
    ರಚನೆ sockaddr  *sa;
    u_char            ಪಠ್ಯ[NGX_SOCKADDR_STRLEN];

    ls = ngx_array_ತಳ್ಳು(&cf->ಚಕ್ರ->listening);
    ಆದರೆ (ls == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ngx_memzero(ls, ಗಾತ್ರ(ngx_listening_t));

    sa = ngx_palloc(cf->pool, socklen);
    ಆದರೆ (sa == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ngx_memcpy(sa, sockaddr, socklen);

    ls->sockaddr = sa;
    ls->socklen = socklen;

    len = ngx_sock_ntop(sa, socklen, ಪಠ್ಯ, NGX_SOCKADDR_STRLEN, 1);
    ls->addr_ಪಠ್ಯ.len = len;

    ಆಯ್ಕೆ (ls->sockaddr->sa_family) {
#ಆದರೆ (NGX_HAVE_INET6)
    ಸ್ಥಿತಿ AF_INET6:
        ls->addr_ಪಠ್ಯ_max_len = NGX_INET6_ADDRSTRLEN;
        ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ (NGX_HAVE_UNIX_DOMAIN)
    ಸ್ಥಿತಿ AF_UNIX:
        ls->addr_ಪಠ್ಯ_max_len = NGX_UNIX_ADDRSTRLEN;
        len++;
        ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ
    ಸ್ಥಿತಿ AF_INET:
        ls->addr_ಪಠ್ಯ_max_len = NGX_INET_ADDRSTRLEN;
        ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ls->addr_ಪಠ್ಯ_max_len = NGX_SOCKADDR_STRLEN;
        ನಿಲ್ಲಿಸು;
    }

    ls->addr_ಪಠ್ಯ.ದತ್ತಾಂಶ = ngx_pnalloc(cf->pool, len);
    ಆದರೆ (ls->addr_ಪಠ್ಯ.ದತ್ತಾಂಶ == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ngx_memcpy(ls->addr_ಪಠ್ಯ.ದತ್ತಾಂಶ, ಪಠ್ಯ, len);

#ಆದರೆ !(NGX_WIN32)
    ngx_rbtree_init(&ls->rbtree, &ls->sentinel, ngx_udp_rbtree_ಸೇರಿಸು_ಮೌಲ್ಯ);
#ಆದರೆ_ಅಂತ್ಯ

    ls->fd = (ngx_socket_t) -1;
    ls->type = SOCK_ಹರಿವು;

    ls->backlog = NGX_LISTEN_BACKLOG;
    ls->rcvbuf = -1;
    ls->sndbuf = -1;

#ಆದರೆ (NGX_HAVE_SETFIB)
    ls->setfib = -1;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HAVE_TCP_FASTOPEN)
    ls->fastopen = -1;
#ಆದರೆ_ಅಂತ್ಯ

    ಹಿಂತಿರುಗಿ ls;
}


ngx_int_t
ngx_clone_listening(ngx_ಚಕ್ರ_t *ಚಕ್ರ, ngx_listening_t *ls)
{
#ಆದರೆ (NGX_HAVE_REUSEPORT)

    ngx_int_t         n;
    ngx_core_conf_t  *ccf;
    ngx_listening_t   ols;

    ಆದರೆ (!ls->reuseport || ls->ಕೆಲಸಗಾರ != 0) {
        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    ols = *ls;

    ccf = (ngx_core_conf_t *) ngx_get_conf(ಚಕ್ರ->conf_ctx, ngx_core_module);

    ಗಾಗಿ (n = 1; n < ccf->ಕೆಲಸಗಾರ_processes; n++) {

        /* create a socket for each worker process */

        ls = ngx_array_ತಳ್ಳು(&ಚಕ್ರ->listening);
        ಆದರೆ (ls == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        *ls = ols;
        ls->ಕೆಲಸಗಾರ = n;
    }

#ಆದರೆ_ಅಂತ್ಯ

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ngx_int_t
ngx_ಗಣ_inherited_sockets(ngx_ಚಕ್ರ_t *ಚಕ್ರ)
{
    ಗಾತ್ರ_t                     len;
    ngx_uint_t                 i;
    ngx_listening_t           *ls;
    socklen_t                  olen;
#ಆದರೆ (NGX_HAVE_DEFERRED_ACCEPT || NGX_HAVE_TCP_FASTOPEN)
    ngx_err_t                  err;
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    ರಚನೆ accept_filter_arg   af;
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)
    ಪೂರ್ಣಾಂಕ                        timeout;
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ (NGX_HAVE_REUSEPORT)
    ಪೂರ್ಣಾಂಕ                        reuseport;
#ಆದರೆ_ಅಂತ್ಯ

    ls = ಚಕ್ರ->listening.elts;
    ಗಾಗಿ (i = 0; i < ಚಕ್ರ->listening.nelts; i++) {

        ls[i].sockaddr = ngx_palloc(ಚಕ್ರ->pool, ಗಾತ್ರ(ngx_sockaddr_t));
        ಆದರೆ (ls[i].sockaddr == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ls[i].socklen = ಗಾತ್ರ(ngx_sockaddr_t);
        ಆದರೆ (getsockname(ls[i].fd, ls[i].sockaddr, &ls[i].socklen) == -1) {
            ngx_log_error(NGX_LOG_CRIT, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_socket_errno,
                          "getsockname() of the inherited "
                          "socket #%d failed", ls[i].fd);
            ls[i].ignore = 1;
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (ls[i].socklen > (socklen_t) ಗಾತ್ರ(ngx_sockaddr_t)) {
            ls[i].socklen = ಗಾತ್ರ(ngx_sockaddr_t);
        }

        ಆಯ್ಕೆ (ls[i].sockaddr->sa_family) {

#ಆದರೆ (NGX_HAVE_INET6)
        ಸ್ಥಿತಿ AF_INET6:
            ls[i].addr_ಪಠ್ಯ_max_len = NGX_INET6_ADDRSTRLEN;
            len = NGX_INET6_ADDRSTRLEN + ಗಾತ್ರ("[]:65535") - 1;
            ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HAVE_UNIX_DOMAIN)
        ಸ್ಥಿತಿ AF_UNIX:
            ls[i].addr_ಪಠ್ಯ_max_len = NGX_UNIX_ADDRSTRLEN;
            len = NGX_UNIX_ADDRSTRLEN;
            ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ

        ಸ್ಥಿತಿ AF_INET:
            ls[i].addr_ಪಠ್ಯ_max_len = NGX_INET_ADDRSTRLEN;
            len = NGX_INET_ADDRSTRLEN + ಗಾತ್ರ(":65535") - 1;
            ನಿಲ್ಲಿಸು;

        ಪೂರ್ವನಿಯೋಜಿತ:
            ngx_log_error(NGX_LOG_CRIT, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_socket_errno,
                          "the inherited socket #%d has "
                          "an unsupported protocol family", ls[i].fd);
            ls[i].ignore = 1;
            ಮುಂದುವರಿ;
        }

        ls[i].addr_ಪಠ್ಯ.ದತ್ತಾಂಶ = ngx_pnalloc(ಚಕ್ರ->pool, len);
        ಆದರೆ (ls[i].addr_ಪಠ್ಯ.ದತ್ತಾಂಶ == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        len = ngx_sock_ntop(ls[i].sockaddr, ls[i].socklen,
                            ls[i].addr_ಪಠ್ಯ.ದತ್ತಾಂಶ, len, 1);
        ಆದರೆ (len == 0) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ls[i].addr_ಪಠ್ಯ.len = len;

        ls[i].backlog = NGX_LISTEN_BACKLOG;

        olen = ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ);

        ಆದರೆ (getsockopt(ls[i].fd, SOL_SOCKET, SO_TYPE, (ಶೂನ್ಯ *) &ls[i].type,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_CRIT, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_socket_errno,
                          "getsockopt(SO_TYPE) %V failed", &ls[i].addr_ಪಠ್ಯ);
            ls[i].ignore = 1;
            ಮುಂದುವರಿ;
        }

        olen = ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ);

        ಆದರೆ (getsockopt(ls[i].fd, SOL_SOCKET, SO_RCVBUF, (ಶೂನ್ಯ *) &ls[i].rcvbuf,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_socket_errno,
                          "getsockopt(SO_RCVBUF) %V failed, ignored",
                          &ls[i].addr_ಪಠ್ಯ);

            ls[i].rcvbuf = -1;
        }

        olen = ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ);

        ಆದರೆ (getsockopt(ls[i].fd, SOL_SOCKET, SO_SNDBUF, (ಶೂನ್ಯ *) &ls[i].sndbuf,
                       &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_socket_errno,
                          "getsockopt(SO_SNDBUF) %V failed, ignored",
                          &ls[i].addr_ಪಠ್ಯ);

            ls[i].sndbuf = -1;
        }

#ಆದರೆ 0
        /* SO_SETFIB is currently a set only option */

#ಆದರೆ (NGX_HAVE_SETFIB)

        olen = ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ);

        ಆದರೆ (getsockopt(ls[i].fd, SOL_SOCKET, SO_SETFIB,
                       (ಶೂನ್ಯ *) &ls[i].setfib, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_socket_errno,
                          "getsockopt(SO_SETFIB) %V failed, ignored",
                          &ls[i].addr_ಪಠ್ಯ);

            ls[i].setfib = -1;
        }

#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HAVE_REUSEPORT)

        reuseport = 0;
        olen = ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ SO_REUSEPORT_LB

        ಆದರೆ (getsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT_LB,
                       (ಶೂನ್ಯ *) &reuseport, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_socket_errno,
                          "getsockopt(SO_REUSEPORT_LB) %V failed, ignored",
                          &ls[i].addr_ಪಠ್ಯ);

        } ಇಲ್ಲದಿದ್ದರೆ {
            ls[i].reuseport = reuseport ? 1 : 0;
        }

#ಇಲ್ಲದಿದ್ದರೆ

        ಆದರೆ (getsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT,
                       (ಶೂನ್ಯ *) &reuseport, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_socket_errno,
                          "getsockopt(SO_REUSEPORT) %V failed, ignored",
                          &ls[i].addr_ಪಠ್ಯ);

        } ಇಲ್ಲದಿದ್ದರೆ {
            ls[i].reuseport = reuseport ? 1 : 0;
        }
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ

        ಆದರೆ (ls[i].type != SOCK_ಹರಿವು) {
            ಮುಂದುವರಿ;
        }

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ SO_PROTOCOL

        olen = ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ);

        ಆದರೆ (getsockopt(ls[i].fd, SOL_SOCKET, SO_PROTOCOL,
                       (ಶೂನ್ಯ *) &ls[i].protocol, &olen)
            == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_socket_errno,
                          "getsockopt(SO_PROTOCOL) %V failed, ignored",
                          &ls[i].addr_ಪಠ್ಯ);
            ls[i].protocol = 0;

        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ls[i].protocol == IPPROTO_TCP) {
            ls[i].protocol = 0;
        }

#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HAVE_TCP_FASTOPEN)

        olen = ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ);

        ಆದರೆ (getsockopt(ls[i].fd, IPPROTO_TCP, TCP_FASTOPEN,
                       (ಶೂನ್ಯ *) &ls[i].fastopen, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            ಆದರೆ (err != NGX_EOPNOTSUPP && err != NGX_ENOPROTOOPT
                && err != NGX_EINVAL)
            {
                ngx_log_error(NGX_LOG_NOTICE, ಚಕ್ರ->ಲಘುಗಣಕ, err,
                              "getsockopt(TCP_FASTOPEN) %V failed, ignored",
                              &ls[i].addr_ಪಠ್ಯ);
            }

            ls[i].fastopen = -1;
        }

#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)

        ngx_memzero(&af, ಗಾತ್ರ(ರಚನೆ accept_filter_arg));
        olen = ಗಾತ್ರ(ರಚನೆ accept_filter_arg);

        ಆದರೆ (getsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER, &af, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            ಆದರೆ (err == NGX_EINVAL) {
                ಮುಂದುವರಿ;
            }

            ngx_log_error(NGX_LOG_NOTICE, ಚಕ್ರ->ಲಘುಗಣಕ, err,
                          "getsockopt(SO_ACCEPTFILTER) for %V failed, ignored",
                          &ls[i].addr_ಪಠ್ಯ);
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (olen < ಗಾತ್ರ(ರಚನೆ accept_filter_arg) || af.af_name[0] == '\0') {
            ಮುಂದುವರಿ;
        }

        ls[i].accept_filter = ngx_palloc(ಚಕ್ರ->pool, 16);
        ಆದರೆ (ls[i].accept_filter == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        (ಶೂನ್ಯ) ngx_cpystrn((u_char *) ls[i].accept_filter,
                           (u_char *) af.af_name, 16);
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)

        timeout = 0;
        olen = ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ);

        ಆದರೆ (getsockopt(ls[i].fd, IPPROTO_TCP, TCP_DEFER_ACCEPT, &timeout, &olen)
            == -1)
        {
            err = ngx_socket_errno;

            ಆದರೆ (err == NGX_EOPNOTSUPP) {
                ಮುಂದುವರಿ;
            }

            ngx_log_error(NGX_LOG_NOTICE, ಚಕ್ರ->ಲಘುಗಣಕ, err,
                          "getsockopt(TCP_DEFER_ACCEPT) for %V failed, ignored",
                          &ls[i].addr_ಪಠ್ಯ);
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (olen < ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ) || timeout == 0) {
            ಮುಂದುವರಿ;
        }

        ls[i].deferred_accept = 1;
#ಆದರೆ_ಅಂತ್ಯ
    }

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ngx_int_t
ngx_open_listening_sockets(ngx_ಚಕ್ರ_t *ಚಕ್ರ)
{
    ಪೂರ್ಣಾಂಕ               reuseaddr;
    ngx_uint_t        i, tries, failed;
    ngx_err_t         err;
    ngx_log_t        *ಲಘುಗಣಕ;
    ngx_socket_t      s;
    ngx_listening_t  *ls;

    reuseaddr = 1;
#ಆದರೆ (NGX_SUPPRESS_WARN)
    failed = 0;
#ಆದರೆ_ಅಂತ್ಯ

    ಲಘುಗಣಕ = ಚಕ್ರ->ಲಘುಗಣಕ;

    /* TODO: configurable try number */

    ಗಾಗಿ (tries = 5; tries; tries--) {
        failed = 0;

        /* for each listening socket */

        ls = ಚಕ್ರ->listening.elts;
        ಗಾಗಿ (i = 0; i < ಚಕ್ರ->listening.nelts; i++) {

            ಆದರೆ (ls[i].ignore) {
                ಮುಂದುವರಿ;
            }

#ಆದರೆ (NGX_HAVE_REUSEPORT)

            ಆದರೆ (ls[i].ಕೂಡಿಸು_reuseport || ls[i].change_protocol) {

                /*
                 * to allow transition from a socket without SO_REUSEPORT
                 * to multiple sockets with SO_REUSEPORT, we have to set
                 * SO_REUSEPORT on the old socket before opening new ones
                 *
                 * to allow transition between different socket protocols
                 * (e.g. IPPROTO_MPTCP), SO_REUSEPORT is set on both old
                 * and new sockets
                 */

                ಪೂರ್ಣಾಂಕ  reuseport = 1;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ SO_REUSEPORT_LB

                ಆದರೆ (setsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT_LB,
                               (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *) &reuseport, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_ALERT, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT_LB) %V failed, "
                                  "ignored",
                                  &ls[i].addr_ಪಠ್ಯ);
                }

#ಇಲ್ಲದಿದ್ದರೆ

                ಆದರೆ (setsockopt(ls[i].fd, SOL_SOCKET, SO_REUSEPORT,
                               (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *) &reuseport, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_ALERT, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT) %V failed, ignored",
                                  &ls[i].addr_ಪಠ್ಯ);
                }
#ಆದರೆ_ಅಂತ್ಯ

                ls[i].ಕೂಡಿಸು_reuseport = 0;
            }
#ಆದರೆ_ಅಂತ್ಯ

            ಆದರೆ (ls[i].fd != (ngx_socket_t) -1 && !ls[i].change_protocol) {
                ಮುಂದುವರಿ;
            }

            ಆದರೆ (ls[i].inherited) {

                /* TODO: close on exit */
                /* TODO: nonblocking */
                /* TODO: deferred accept */

                ಮುಂದುವರಿ;
            }

            s = ngx_socket(ls[i].sockaddr->sa_family, ls[i].type,
                           ls[i].protocol);

            ಆದರೆ (s == (ngx_socket_t) -1) {
                ngx_log_error(NGX_LOG_EMERG, ಲಘುಗಣಕ, ngx_socket_errno,
                              ngx_socket_n " %V failed", &ls[i].addr_ಪಠ್ಯ);
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            ಆದರೆ (ls[i].type != SOCK_DGRAM || !ngx_test_config) {

                ಆದರೆ (setsockopt(s, SOL_SOCKET, SO_REUSEADDR,
                               (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *) &reuseaddr, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, ಲಘುಗಣಕ, ngx_socket_errno,
                                  "setsockopt(SO_REUSEADDR) %V failed",
                                  &ls[i].addr_ಪಠ್ಯ);

                    ಆದರೆ (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, ಲಘುಗಣಕ, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_ಪಠ್ಯ);
                    }

                    ಹಿಂತಿರುಗಿ NGX_ERROR;
                }
            }

#ಆದರೆ (NGX_HAVE_REUSEPORT)

            ಆದರೆ ((ls[i].reuseport || ls[i].change_protocol)
                && !ngx_test_config)
            {
                ಪೂರ್ಣಾಂಕ  reuseport;

                reuseport = 1;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ SO_REUSEPORT_LB

                ಆದರೆ (setsockopt(s, SOL_SOCKET, SO_REUSEPORT_LB,
                               (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *) &reuseport, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, ಲಘುಗಣಕ, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT_LB) %V failed",
                                  &ls[i].addr_ಪಠ್ಯ);

                    ಆದರೆ (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, ಲಘುಗಣಕ, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_ಪಠ್ಯ);
                    }

                    ಹಿಂತಿರುಗಿ NGX_ERROR;
                }

#ಇಲ್ಲದಿದ್ದರೆ

                ಆದರೆ (setsockopt(s, SOL_SOCKET, SO_REUSEPORT,
                               (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *) &reuseport, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, ಲಘುಗಣಕ, ngx_socket_errno,
                                  "setsockopt(SO_REUSEPORT) %V failed",
                                  &ls[i].addr_ಪಠ್ಯ);

                    ಆದರೆ (ngx_close_socket(s) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, ಲಘುಗಣಕ, ngx_socket_errno,
                                      ngx_close_socket_n " %V failed",
                                      &ls[i].addr_ಪಠ್ಯ);
                    }

                    ಹಿಂತಿರುಗಿ NGX_ERROR;
                }
#ಆದರೆ_ಅಂತ್ಯ
            }
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HAVE_INET6 && defined IPV6_V6ONLY)

            ಆದರೆ (ls[i].sockaddr->sa_family == AF_INET6) {
                ಪೂರ್ಣಾಂಕ  ipv6only;

                ipv6only = ls[i].ipv6only;

                ಆದರೆ (setsockopt(s, IPPROTO_IPV6, IPV6_V6ONLY,
                               (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *) &ipv6only, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ))
                    == -1)
                {
                    ngx_log_error(NGX_LOG_EMERG, ಲಘುಗಣಕ, ngx_socket_errno,
                                  "setsockopt(IPV6_V6ONLY) %V failed, ignored",
                                  &ls[i].addr_ಪಠ್ಯ);
                }
            }
#ಆದರೆ_ಅಂತ್ಯ
            /* TODO: close on exit */

            ಆದರೆ (ngx_nonblocking(s) == -1) {
                ngx_log_error(NGX_LOG_EMERG, ಲಘುಗಣಕ, ngx_socket_errno,
                              ngx_nonblocking_n " %V failed",
                              &ls[i].addr_ಪಠ್ಯ);

                ಆದರೆ (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, ಲಘುಗಣಕ, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_ಪಠ್ಯ);
                }

                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            ngx_log_debug2(NGX_LOG_DEBUG_CORE, ಲಘುಗಣಕ, 0,
                           "bind() %V #%d ", &ls[i].addr_ಪಠ್ಯ, s);

            ಆದರೆ (bind(s, ls[i].sockaddr, ls[i].socklen) == -1) {
                err = ngx_socket_errno;

                ಆದರೆ (err != NGX_EADDRINUSE || !ngx_test_config) {
                    ngx_log_error(NGX_LOG_EMERG, ಲಘುಗಣಕ, err,
                                  "bind() to %V failed", &ls[i].addr_ಪಠ್ಯ);
                }

                ಆದರೆ (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, ಲಘುಗಣಕ, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_ಪಠ್ಯ);
                }

                ಆದರೆ (err != NGX_EADDRINUSE) {
                    ಹಿಂತಿರುಗಿ NGX_ERROR;
                }

                ಆದರೆ (!ngx_test_config) {
                    failed = 1;
                }

                ಮುಂದುವರಿ;
            }

#ಆದರೆ (NGX_HAVE_UNIX_DOMAIN)

            ಆದರೆ (ls[i].sockaddr->sa_family == AF_UNIX) {
                ಬಹುಲಕ_t   ಬಹುಲಕ;
                u_char  *name;

                name = ls[i].addr_ಪಠ್ಯ.ದತ್ತಾಂಶ + ಗಾತ್ರ("unix:") - 1;
                ಬಹುಲಕ = (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);

                ಆದರೆ (chmod((ಅಕ್ಷರ *) name, ಬಹುಲಕ) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_errno,
                                  "chmod() \"%s\" failed", name);
                }

                ಆದರೆ (ngx_test_config) {
                    ಆದರೆ (ngx_delete_file(name) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_errno,
                                      ngx_delete_file_n " %s failed", name);
                    }
                }
            }
#ಆದರೆ_ಅಂತ್ಯ

            ಆದರೆ (ls[i].type != SOCK_ಹರಿವು) {
                ls[i].fd = s;
                ls[i].open = 1;
                ಮುಂದುವರಿ;
            }

            ಆದರೆ (listen(s, ls[i].backlog) == -1) {
                err = ngx_socket_errno;

                /*
                 * on OpenVZ after suspend/resume EADDRINUSE
                 * may be returned by listen() instead of bind(), see
                 * https://bugs.openvz.org/browse/OVZ-5587
                 */

                ಆದರೆ (err != NGX_EADDRINUSE || !ngx_test_config) {
                    ngx_log_error(NGX_LOG_EMERG, ಲಘುಗಣಕ, err,
                                  "listen() to %V, backlog %d failed",
                                  &ls[i].addr_ಪಠ್ಯ, ls[i].backlog);
                }

                ಆದರೆ (ngx_close_socket(s) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, ಲಘುಗಣಕ, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[i].addr_ಪಠ್ಯ);
                }

                ಆದರೆ (err != NGX_EADDRINUSE) {
                    ಹಿಂತಿರುಗಿ NGX_ERROR;
                }

                ಆದರೆ (!ngx_test_config) {
                    failed = 1;
                }

                ಮುಂದುವರಿ;
            }

            ls[i].listen = 1;

            ls[i].fd = s;
            ls[i].open = 1;
        }

        ಆದರೆ (!failed) {
            ನಿಲ್ಲಿಸು;
        }

        /* TODO: delay configurable */

        ngx_log_error(NGX_LOG_NOTICE, ಲಘುಗಣಕ, 0,
                      "try again to bind() after 500ms");

        ngx_msleep(500);
    }

    ಆದರೆ (failed) {
        ngx_log_error(NGX_LOG_EMERG, ಲಘುಗಣಕ, 0, "still could not bind()");
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ಶೂನ್ಯ
ngx_configure_listening_sockets(ngx_ಚಕ್ರ_t *ಚಕ್ರ)
{
    ಪೂರ್ಣಾಂಕ                        ಮೌಲ್ಯ;
    ngx_uint_t                 i;
    ngx_listening_t           *ls;

#ಆದರೆ (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    ರಚನೆ accept_filter_arg   af;
#ಆದರೆ_ಅಂತ್ಯ

    ls = ಚಕ್ರ->listening.elts;
    ಗಾಗಿ (i = 0; i < ಚಕ್ರ->listening.nelts; i++) {

        ls[i].ಲಘುಗಣಕ = *ls[i].logp;

        ಆದರೆ (ls[i].rcvbuf != -1) {
            ಆದರೆ (setsockopt(ls[i].fd, SOL_SOCKET, SO_RCVBUF,
                           (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *) &ls[i].rcvbuf, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_socket_errno,
                              "setsockopt(SO_RCVBUF, %d) %V failed, ignored",
                              ls[i].rcvbuf, &ls[i].addr_ಪಠ್ಯ);
            }
        }

        ಆದರೆ (ls[i].sndbuf != -1) {
            ಆದರೆ (setsockopt(ls[i].fd, SOL_SOCKET, SO_SNDBUF,
                           (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *) &ls[i].sndbuf, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_socket_errno,
                              "setsockopt(SO_SNDBUF, %d) %V failed, ignored",
                              ls[i].sndbuf, &ls[i].addr_ಪಠ್ಯ);
            }
        }

        ಆದರೆ (ls[i].keepalive) {
            ಮೌಲ್ಯ = (ls[i].keepalive == 1) ? 1 : 0;

            ಆದರೆ (setsockopt(ls[i].fd, SOL_SOCKET, SO_KEEPALIVE,
                           (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *) &ಮೌಲ್ಯ, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_socket_errno,
                              "setsockopt(SO_KEEPALIVE, %d) %V failed, ignored",
                              ಮೌಲ್ಯ, &ls[i].addr_ಪಠ್ಯ);
            }
        }

#ಆದರೆ (NGX_HAVE_KEEPALIVE_TUNABLE)

#ಆದರೆ !(NGX_DARWIN)

        ಆದರೆ (ls[i].keepidle) {
            ಮೌಲ್ಯ = ls[i].keepidle;

#ಆದರೆ (NGX_KEEPALIVE_ಗುಣಕ)
            ಮೌಲ್ಯ *= NGX_KEEPALIVE_ಗುಣಕ;
#ಆದರೆ_ಅಂತ್ಯ

            ಆದರೆ (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPIDLE,
                           (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *) &ಮೌಲ್ಯ, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_socket_errno,
                              "setsockopt(TCP_KEEPIDLE, %d) %V failed, ignored",
                              ಮೌಲ್ಯ, &ls[i].addr_ಪಠ್ಯ);
            }
        }

#ಆದರೆ_ಅಂತ್ಯ

        ಆದರೆ (ls[i].keepintvl) {
            ಮೌಲ್ಯ = ls[i].keepintvl;

#ಆದರೆ (NGX_KEEPALIVE_ಗುಣಕ)
            ಮೌಲ್ಯ *= NGX_KEEPALIVE_ಗುಣಕ;
#ಆದರೆ_ಅಂತ್ಯ

            ಆದರೆ (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPINTVL,
                           (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *) &ಮೌಲ್ಯ, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_socket_errno,
                             "setsockopt(TCP_KEEPINTVL, %d) %V failed, ignored",
                             ಮೌಲ್ಯ, &ls[i].addr_ಪಠ್ಯ);
            }
        }

        ಆದರೆ (ls[i].keepcnt) {
            ಆದರೆ (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_KEEPCNT,
                           (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *) &ls[i].keepcnt, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_socket_errno,
                              "setsockopt(TCP_KEEPCNT, %d) %V failed, ignored",
                              ls[i].keepcnt, &ls[i].addr_ಪಠ್ಯ);
            }
        }

#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HAVE_SETFIB)
        ಆದರೆ (ls[i].setfib != -1) {
            ಆದರೆ (setsockopt(ls[i].fd, SOL_SOCKET, SO_SETFIB,
                           (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *) &ls[i].setfib, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_socket_errno,
                              "setsockopt(SO_SETFIB, %d) %V failed, ignored",
                              ls[i].setfib, &ls[i].addr_ಪಠ್ಯ);
            }
        }
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HAVE_TCP_FASTOPEN)
        ಆದರೆ (ls[i].fastopen != -1) {
            ಆದರೆ (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_FASTOPEN,
                           (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *) &ls[i].fastopen, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_socket_errno,
                              "setsockopt(TCP_FASTOPEN, %d) %V failed, ignored",
                              ls[i].fastopen, &ls[i].addr_ಪಠ್ಯ);
            }
        }
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ 0
        ಆದರೆ (1) {
            ಪೂರ್ಣಾಂಕ tcp_nodelay = 1;

            ಆದರೆ (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_NODELAY,
                       (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *) &tcp_nodelay, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_socket_errno,
                              "setsockopt(TCP_NODELAY) %V failed, ignored",
                              &ls[i].addr_ಪಠ್ಯ);
            }
        }
#ಆದರೆ_ಅಂತ್ಯ

        ಆದರೆ (ls[i].listen) {

            /* change backlog via listen() */

            ಆದರೆ (listen(ls[i].fd, ls[i].backlog) == -1) {
                ngx_log_error(NGX_LOG_ALERT, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_socket_errno,
                              "listen() to %V, backlog %d failed, ignored",
                              &ls[i].addr_ಪಠ್ಯ, ls[i].backlog);
            }
        }

        /*
         * setting deferred mode should be last operation on socket,
         * because code may prematurely continue cycle on failure
         */

#ಆದರೆ (NGX_HAVE_DEFERRED_ACCEPT)

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ SO_ACCEPTFILTER

        ಆದರೆ (ls[i].delete_deferred) {
            ಆದರೆ (setsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER, ಖಾಲಿ, 0)
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_socket_errno,
                              "setsockopt(SO_ACCEPTFILTER, NULL) "
                              "for %V failed, ignored",
                              &ls[i].addr_ಪಠ್ಯ);

                ಆದರೆ (ls[i].accept_filter) {
                    ngx_log_error(NGX_LOG_ALERT, ಚಕ್ರ->ಲಘುಗಣಕ, 0,
                                  "could not change the accept filter "
                                  "to \"%s\" for %V, ignored",
                                  ls[i].accept_filter, &ls[i].addr_ಪಠ್ಯ);
                }

                ಮುಂದುವರಿ;
            }

            ls[i].deferred_accept = 0;
        }

        ಆದರೆ (ls[i].ಕೂಡಿಸು_deferred) {
            ngx_memzero(&af, ಗಾತ್ರ(ರಚನೆ accept_filter_arg));
            (ಶೂನ್ಯ) ngx_cpystrn((u_char *) af.af_name,
                               (u_char *) ls[i].accept_filter, 16);

            ಆದರೆ (setsockopt(ls[i].fd, SOL_SOCKET, SO_ACCEPTFILTER,
                           &af, ಗಾತ್ರ(ರಚನೆ accept_filter_arg))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_socket_errno,
                              "setsockopt(SO_ACCEPTFILTER, \"%s\") "
                              "for %V failed, ignored",
                              ls[i].accept_filter, &ls[i].addr_ಪಠ್ಯ);
                ಮುಂದುವರಿ;
            }

            ls[i].deferred_accept = 1;
        }

#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ TCP_DEFER_ACCEPT

        ಆದರೆ (ls[i].ಕೂಡಿಸು_deferred || ls[i].delete_deferred) {

            ಆದರೆ (ls[i].ಕೂಡಿಸು_deferred) {
                /*
                 * There is no way to find out how long a connection was
                 * in queue (and a connection may bypass deferred queue at all
                 * if syncookies were used), hence we use 1 second timeout
                 * here.
                 */
                ಮೌಲ್ಯ = 1;

            } ಇಲ್ಲದಿದ್ದರೆ {
                ಮೌಲ್ಯ = 0;
            }

            ಆದರೆ (setsockopt(ls[i].fd, IPPROTO_TCP, TCP_DEFER_ACCEPT,
                           &ಮೌಲ್ಯ, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_socket_errno,
                              "setsockopt(TCP_DEFER_ACCEPT, %d) for %V failed, "
                              "ignored",
                              ಮೌಲ್ಯ, &ls[i].addr_ಪಠ್ಯ);

                ಮುಂದುವರಿ;
            }
        }

        ಆದರೆ (ls[i].ಕೂಡಿಸು_deferred) {
            ls[i].deferred_accept = 1;
        }

#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ /* NGX_HAVE_DEFERRED_ACCEPT */

#ಆದರೆ (NGX_HAVE_IP_RECVDSTADDR)

        ಆದರೆ (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET)
        {
            ಮೌಲ್ಯ = 1;

            ಆದರೆ (setsockopt(ls[i].fd, IPPROTO_IP, IP_RECVDSTADDR,
                           (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *) &ಮೌಲ್ಯ, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_socket_errno,
                              "setsockopt(IP_RECVDSTADDR) "
                              "for %V failed, ignored",
                              &ls[i].addr_ಪಠ್ಯ);
            }
        }

#ಅಥವಾಆದರೆ (NGX_HAVE_IP_PKTINFO)

        ಆದರೆ (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET)
        {
            ಮೌಲ್ಯ = 1;

            ಆದರೆ (setsockopt(ls[i].fd, IPPROTO_IP, IP_PKTINFO,
                           (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *) &ಮೌಲ್ಯ, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_socket_errno,
                              "setsockopt(IP_PKTINFO) "
                              "for %V failed, ignored",
                              &ls[i].addr_ಪಠ್ಯ);
            }
        }

#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HAVE_INET6 && NGX_HAVE_IPV6_RECVPKTINFO)

        ಆದರೆ (ls[i].wildcard
            && ls[i].type == SOCK_DGRAM
            && ls[i].sockaddr->sa_family == AF_INET6)
        {
            ಮೌಲ್ಯ = 1;

            ಆದರೆ (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_RECVPKTINFO,
                           (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *) &ಮೌಲ್ಯ, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_socket_errno,
                              "setsockopt(IPV6_RECVPKTINFO) "
                              "for %V failed, ignored",
                              &ls[i].addr_ಪಠ್ಯ);
            }
        }

#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HAVE_IP_MTU_DISCOVER)

        ಆದರೆ (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET) {
            ಮೌಲ್ಯ = IP_PMTUDISC_DO;

            ಆದರೆ (setsockopt(ls[i].fd, IPPROTO_IP, IP_MTU_DISCOVER,
                           (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *) &ಮೌಲ್ಯ, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_socket_errno,
                              "setsockopt(IP_MTU_DISCOVER) "
                              "for %V failed, ignored",
                              &ls[i].addr_ಪಠ್ಯ);
            }
        }

#ಅಥವಾಆದರೆ (NGX_HAVE_IP_DONTFRAG)

        ಆದರೆ (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET) {
            ಮೌಲ್ಯ = 1;

            ಆದರೆ (setsockopt(ls[i].fd, IPPROTO_IP, IP_DONTFRAG,
                           (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *) &ಮೌಲ್ಯ, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_socket_errno,
                              "setsockopt(IP_DONTFRAG) "
                              "for %V failed, ignored",
                              &ls[i].addr_ಪಠ್ಯ);
            }
        }

#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HAVE_INET6)

#ಆದರೆ (NGX_HAVE_IPV6_MTU_DISCOVER)

        ಆದರೆ (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET6) {
            ಮೌಲ್ಯ = IPV6_PMTUDISC_DO;

            ಆದರೆ (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_MTU_DISCOVER,
                           (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *) &ಮೌಲ್ಯ, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_socket_errno,
                              "setsockopt(IPV6_MTU_DISCOVER) "
                              "for %V failed, ignored",
                              &ls[i].addr_ಪಠ್ಯ);
            }
        }

#ಅಥವಾಆದರೆ (NGX_HAVE_IPV6_DONTFRAG)

        ಆದರೆ (ls[i].quic && ls[i].sockaddr->sa_family == AF_INET6) {
            ಮೌಲ್ಯ = 1;

            ಆದರೆ (setsockopt(ls[i].fd, IPPROTO_IPV6, IPV6_DONTFRAG,
                           (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *) &ಮೌಲ್ಯ, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ))
                == -1)
            {
                ngx_log_error(NGX_LOG_ALERT, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_socket_errno,
                              "setsockopt(IPV6_DONTFRAG) "
                              "for %V failed, ignored",
                              &ls[i].addr_ಪಠ್ಯ);
            }
        }

#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ
    }

    ಹಿಂತಿರುಗಿ;
}


ಶೂನ್ಯ
ngx_close_listening_sockets(ngx_ಚಕ್ರ_t *ಚಕ್ರ)
{
    ngx_uint_t         i;
    ngx_listening_t   *ls;
    ngx_connection_t  *c;

    ngx_accept_mutex_held = 0;
    ngx_use_accept_mutex = 0;

    ls = ಚಕ್ರ->listening.elts;
    ಗಾಗಿ (i = 0; i < ಚಕ್ರ->listening.nelts; i++) {

#ಆದರೆ (NGX_QUIC)
        ಆದರೆ (ls[i].quic
            && (!ls[i].reuseport || !ngx_quic_bpf_enabled(ಚಕ್ರ)))
        {
            ಮುಂದುವರಿ;
        }
#ಆದರೆ_ಅಂತ್ಯ

        c = ls[i].connection;

        ಆದರೆ (c) {
            ಆದರೆ (c->ಪಠ->ಸಕ್ರಿಯ) {
                ಆದರೆ (ngx_event_flags & NGX_USE_EPOLL_EVENT) {

                    /*
                     * it seems that Linux-2.6.x OpenVZ sends events
                     * for closed shared listening sockets unless
                     * the events was explicitly deleted
                     */

                    ngx_del_event(c->ಪಠ, NGX_READ_EVENT, 0);

                } ಇಲ್ಲದಿದ್ದರೆ {
                    ngx_del_event(c->ಪಠ, NGX_READ_EVENT, NGX_CLOSE_EVENT);
                }
            }

            ಆದರೆ (c->ಪಠ->timer_ಗಣ) {
                ngx_del_timer(c->ಪಠ);
            }

            ngx_free_connection(c);

            c->fd = (ngx_socket_t) -1;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, ಚಕ್ರ->ಲಘುಗಣಕ, 0,
                       "close listening %V #%d ", &ls[i].addr_ಪಠ್ಯ, ls[i].fd);

        ಆದರೆ (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_socket_errno,
                          ngx_close_socket_n " %V failed", &ls[i].addr_ಪಠ್ಯ);
        }

#ಆದರೆ (NGX_HAVE_UNIX_DOMAIN)

        ಆದರೆ (ls[i].sockaddr->sa_family == AF_UNIX
            && ngx_process <= NGX_PROCESS_MASTER
            && ngx_new_ದ್ವಿಮಾನ == 0
            && (!ls[i].inherited || ngx_getppid() != ngx_ಪೋಷಕ))
        {
            u_char *name = ls[i].addr_ಪಠ್ಯ.ದತ್ತಾಂಶ + ಗಾತ್ರ("unix:") - 1;

            ಆದರೆ (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_socket_errno,
                              ngx_delete_file_n " %s failed", name);
            }
        }

#ಆದರೆ_ಅಂತ್ಯ

        ls[i].fd = (ngx_socket_t) -1;
    }

    ಚಕ್ರ->listening.nelts = 0;
}


ngx_connection_t *
ngx_get_connection(ngx_socket_t s, ngx_log_t *ಲಘುಗಣಕ)
{
    ngx_uint_t         instance;
    ngx_event_t       *rev, *wev;
    ngx_connection_t  *c;

    /* disable warning: Win32 SOCKET is u_int while UNIX socket is int */

    ಆದರೆ (ngx_ಚಕ್ರ->files && (ngx_uint_t) s >= ngx_ಚಕ್ರ->files_n) {
        ngx_log_error(NGX_LOG_ALERT, ಲಘುಗಣಕ, 0,
                      "the new socket has number %d, "
                      "but only %ui files are available",
                      s, ngx_ಚಕ್ರ->files_n);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ngx_drain_connections((ngx_ಚಕ್ರ_t *) ngx_ಚಕ್ರ);

    c = ngx_ಚಕ್ರ->free_connections;

    ಆದರೆ (c == ಖಾಲಿ) {
        ngx_log_error(NGX_LOG_ALERT, ಲಘುಗಣಕ, 0,
                      "%ui worker_connections are not enough",
                      ngx_ಚಕ್ರ->connection_n);

        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ngx_ಚಕ್ರ->free_connections = c->ದತ್ತಾಂಶ;
    ngx_ಚಕ್ರ->free_connection_n--;

    ಆದರೆ (ngx_ಚಕ್ರ->files && ngx_ಚಕ್ರ->files[s] == ಖಾಲಿ) {
        ngx_ಚಕ್ರ->files[s] = c;
    }

    rev = c->ಪಠ;
    wev = c->ಲಿಖ;

    ngx_memzero(c, ಗಾತ್ರ(ngx_connection_t));

    c->ಪಠ = rev;
    c->ಲಿಖ = wev;
    c->fd = s;
    c->ಲಘುಗಣಕ = ಲಘುಗಣಕ;

    instance = rev->instance;

    ngx_memzero(rev, ಗಾತ್ರ(ngx_event_t));
    ngx_memzero(wev, ಗಾತ್ರ(ngx_event_t));

    rev->instance = !instance;
    wev->instance = !instance;

    rev->ಸೂಚ್ಯಂಕ = NGX_INVALID_ಸೂಚ್ಯಂಕ;
    wev->ಸೂಚ್ಯಂಕ = NGX_INVALID_ಸೂಚ್ಯಂಕ;

    rev->ದತ್ತಾಂಶ = c;
    wev->ದತ್ತಾಂಶ = c;

    wev->ಲಿಖ = 1;

    ಹಿಂತಿರುಗಿ c;
}


ಶೂನ್ಯ
ngx_free_connection(ngx_connection_t *c)
{
    c->ದತ್ತಾಂಶ = ngx_ಚಕ್ರ->free_connections;
    ngx_ಚಕ್ರ->free_connections = c;
    ngx_ಚಕ್ರ->free_connection_n++;

    ಆದರೆ (ngx_ಚಕ್ರ->files && ngx_ಚಕ್ರ->files[c->fd] == c) {
        ngx_ಚಕ್ರ->files[c->fd] = ಖಾಲಿ;
    }
}


ಶೂನ್ಯ
ngx_close_connection(ngx_connection_t *c)
{
    ngx_err_t     err;
    ngx_uint_t    log_error, level;
    ngx_socket_t  fd;

    ಆದರೆ (c->fd == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, c->ಲಘುಗಣಕ, 0, "connection already closed");
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ (c->ಪಠ->timer_ಗಣ) {
        ngx_del_timer(c->ಪಠ);
    }

    ಆದರೆ (c->ಲಿಖ->timer_ಗಣ) {
        ngx_del_timer(c->ಲಿಖ);
    }

    ಆದರೆ (!c->shared) {
        ಆದರೆ (ngx_del_conn) {
            ngx_del_conn(c, NGX_CLOSE_EVENT);

        } ಇಲ್ಲದಿದ್ದರೆ {
            ಆದರೆ (c->ಪಠ->ಸಕ್ರಿಯ || c->ಪಠ->disabled) {
                ngx_del_event(c->ಪಠ, NGX_READ_EVENT, NGX_CLOSE_EVENT);
            }

            ಆದರೆ (c->ಲಿಖ->ಸಕ್ರಿಯ || c->ಲಿಖ->disabled) {
                ngx_del_event(c->ಲಿಖ, NGX_WRITE_EVENT, NGX_CLOSE_EVENT);
            }
        }
    }

    ಆದರೆ (c->ಪಠ->posted) {
        ngx_delete_posted_event(c->ಪಠ);
    }

    ಆದರೆ (c->ಲಿಖ->posted) {
        ngx_delete_posted_event(c->ಲಿಖ);
    }

    c->ಪಠ->closed = 1;
    c->ಲಿಖ->closed = 1;

    ngx_reusable_connection(c, 0);

    log_error = c->log_error;

    ngx_free_connection(c);

    fd = c->fd;
    c->fd = (ngx_socket_t) -1;

    ಆದರೆ (c->shared) {
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ (ngx_close_socket(fd) == -1) {

        err = ngx_socket_errno;

        ಆದರೆ (err == NGX_ECONNRESET || err == NGX_ENOTCONN) {

            ಆಯ್ಕೆ (log_error) {

            ಸ್ಥಿತಿ NGX_ERROR_INFO:
                level = NGX_LOG_INFO;
                ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ NGX_ERROR_ERR:
                level = NGX_LOG_ERR;
                ನಿಲ್ಲಿಸು;

            ಪೂರ್ವನಿಯೋಜಿತ:
                level = NGX_LOG_CRIT;
            }

        } ಇಲ್ಲದಿದ್ದರೆ {
            level = NGX_LOG_CRIT;
        }

        ngx_log_error(level, c->ಲಘುಗಣಕ, err, ngx_close_socket_n " %d failed", fd);
    }
}


ಶೂನ್ಯ
ngx_reusable_connection(ngx_connection_t *c, ngx_uint_t reusable)
{
    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->ಲಘುಗಣಕ, 0,
                   "reusable connection: %ui", reusable);

    ಆದರೆ (c->reusable) {
        ngx_ಸರತಿ_remove(&c->ಸರತಿ);
        ngx_ಚಕ್ರ->reusable_connections_n--;

#ಆದರೆ (NGX_STAT_STUB)
        (ಶೂನ್ಯ) ngx_atomic_fetch_ಕೂಡಿಸು(ngx_stat_ಕಾಯುವಿಕೆ, -1);
#ಆದರೆ_ಅಂತ್ಯ
    }

    c->reusable = reusable;

    ಆದರೆ (reusable) {
        /* need cast as ngx_cycle is volatile */

        ngx_ಸರತಿ_ಸೇರಿಸು_ತಲೆ(
            (ngx_ಸರತಿ_t *) &ngx_ಚಕ್ರ->reusable_connections_ಸರತಿ, &c->ಸರತಿ);
        ngx_ಚಕ್ರ->reusable_connections_n++;

#ಆದರೆ (NGX_STAT_STUB)
        (ಶೂನ್ಯ) ngx_atomic_fetch_ಕೂಡಿಸು(ngx_stat_ಕಾಯುವಿಕೆ, 1);
#ಆದರೆ_ಅಂತ್ಯ
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_drain_connections(ngx_ಚಕ್ರ_t *ಚಕ್ರ)
{
    ngx_uint_t         i, n;
    ngx_ಸರತಿ_t       *q;
    ngx_connection_t  *c;

    ಆದರೆ (ಚಕ್ರ->free_connection_n > ಚಕ್ರ->connection_n / 16
        || ಚಕ್ರ->reusable_connections_n == 0)
    {
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ (ಚಕ್ರ->connections_reuse_ಸಮಯ != ngx_ಸಮಯ()) {
        ಚಕ್ರ->connections_reuse_ಸಮಯ = ngx_ಸಮಯ();

        ngx_log_error(NGX_LOG_WARN, ಚಕ್ರ->ಲಘುಗಣಕ, 0,
                      "%ui worker_connections are not enough, "
                      "reusing connections",
                      ಚಕ್ರ->connection_n);
    }

    c = ಖಾಲಿ;
    n = ngx_max(ngx_min(32, ಚಕ್ರ->reusable_connections_n / 8), 1);

    ಗಾಗಿ (i = 0; i < n; i++) {
        ಆದರೆ (ngx_ಸರತಿ_ಖಾಲಿಗಳು(&ಚಕ್ರ->reusable_connections_ಸರತಿ)) {
            ನಿಲ್ಲಿಸು;
        }

        q = ngx_ಸರತಿ_ಕೊನೆಯ(&ಚಕ್ರ->reusable_connections_ಸರತಿ);
        c = ngx_ಸರತಿ_ದತ್ತಾಂಶ(q, ngx_connection_t, ಸರತಿ);

        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->ಲಘುಗಣಕ, 0,
                       "reusing connection");

        c->close = 1;
        c->ಪಠ->handler(c->ಪಠ);
    }

    ಆದರೆ (ಚಕ್ರ->free_connection_n == 0 && c && c->reusable) {

        /*
         * if no connections were freed, try to reuse the last
         * connection again: this should free it as long as
         * previous reuse moved it to lingering close
         */

        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->ಲಘುಗಣಕ, 0,
                       "reusing connection again");

        c->close = 1;
        c->ಪಠ->handler(c->ಪಠ);
    }
}


ಶೂನ್ಯ
ngx_close_idle_connections(ngx_ಚಕ್ರ_t *ಚಕ್ರ)
{
    ngx_uint_t         i;
    ngx_connection_t  *c;

    c = ಚಕ್ರ->connections;

    ಗಾಗಿ (i = 0; i < ಚಕ್ರ->connection_n; i++) {

        /* THREAD: lock */

        ಆದರೆ (c[i].fd != (ngx_socket_t) -1 && c[i].idle) {
            c[i].close = 1;
            c[i].ಪಠ->handler(c[i].ಪಠ);
        }
    }
}


ngx_int_t
ngx_connection_local_sockaddr(ngx_connection_t *c, ngx_str_t *s,
    ngx_uint_t ಪೋರ್ಟ್)
{
    socklen_t             len;
    ngx_uint_t            addr;
    ngx_sockaddr_t        sa;
    ರಚನೆ sockaddr_ಒಳ   *ಜ್ಯಾ;
#ಆದರೆ (NGX_HAVE_INET6)
    ngx_uint_t            i;
    ರಚನೆ sockaddr_in6  *sin6;
#ಆದರೆ_ಅಂತ್ಯ

    addr = 0;

    ಆದರೆ (c->local_socklen) {
        ಆಯ್ಕೆ (c->local_sockaddr->sa_family) {

#ಆದರೆ (NGX_HAVE_INET6)
        ಸ್ಥಿತಿ AF_INET6:
            sin6 = (ರಚನೆ sockaddr_in6 *) c->local_sockaddr;

            ಗಾಗಿ (i = 0; addr == 0 && i < 16; i++) {
                addr |= sin6->sin6_addr.s6_addr[i];
            }

            ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HAVE_UNIX_DOMAIN)
        ಸ್ಥಿತಿ AF_UNIX:
            addr = 1;
            ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ

        ಪೂರ್ವನಿಯೋಜಿತ: /* AF_INET */
            ಜ್ಯಾ = (ರಚನೆ sockaddr_ಒಳ *) c->local_sockaddr;
            addr = ಜ್ಯಾ->sin_addr.s_addr;
            ನಿಲ್ಲಿಸು;
        }
    }

    ಆದರೆ (addr == 0) {

        len = ಗಾತ್ರ(ngx_sockaddr_t);

        ಆದರೆ (getsockname(c->fd, &sa.sockaddr, &len) == -1) {
            ngx_connection_error(c, ngx_socket_errno, "getsockname() failed");
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        c->local_sockaddr = ngx_palloc(c->pool, len);
        ಆದರೆ (c->local_sockaddr == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ngx_memcpy(c->local_sockaddr, &sa, len);

        c->local_socklen = len;
    }

    ಆದರೆ (s == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    s->len = ngx_sock_ntop(c->local_sockaddr, c->local_socklen,
                           s->ದತ್ತಾಂಶ, s->len, ಪೋರ್ಟ್);

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ngx_int_t
ngx_tcp_nodelay(ngx_connection_t *c)
{
    ಪೂರ್ಣಾಂಕ  tcp_nodelay;

    ಆದರೆ (c->tcp_nodelay != NGX_TCP_NODELAY_UNSET) {
        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->ಲಘುಗಣಕ, 0, "tcp_nodelay");

    tcp_nodelay = 1;

    ಆದರೆ (setsockopt(c->fd, IPPROTO_TCP, TCP_NODELAY,
                   (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *) &tcp_nodelay, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ))
        == -1)
    {
#ಆದರೆ (NGX_SOLARIS)
        ಆದರೆ (c->log_error == NGX_ERROR_INFO) {

            /* Solaris returns EINVAL if a socket has been shut down */
            c->log_error = NGX_ERROR_IGNORE_EINVAL;

            ngx_connection_error(c, ngx_socket_errno,
                                 "setsockopt(TCP_NODELAY) failed");

            c->log_error = NGX_ERROR_INFO;

            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }
#ಆದರೆ_ಅಂತ್ಯ

        ngx_connection_error(c, ngx_socket_errno,
                             "setsockopt(TCP_NODELAY) failed");
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    c->tcp_nodelay = NGX_TCP_NODELAY_ಗಣ;

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ngx_int_t
ngx_connection_error(ngx_connection_t *c, ngx_err_t err, ಅಕ್ಷರ *ಪಠ್ಯ)
{
    ngx_uint_t  level;

    /* Winsock may return NGX_ECONNABORTED instead of NGX_ECONNRESET */

    ಆದರೆ ((err == NGX_ECONNRESET
#ಆದರೆ (NGX_WIN32)
         || err == NGX_ECONNABORTED
#ಆದರೆ_ಅಂತ್ಯ
        ) && c->log_error == NGX_ERROR_IGNORE_ECONNRESET)
    {
        ಹಿಂತಿರುಗಿ 0;
    }

#ಆದರೆ (NGX_SOLARIS)
    ಆದರೆ (err == NGX_EINVAL && c->log_error == NGX_ERROR_IGNORE_EINVAL) {
        ಹಿಂತಿರುಗಿ 0;
    }
#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ (err == NGX_EMSGSIZE && c->log_error == NGX_ERROR_IGNORE_EMSGSIZE) {
        ಹಿಂತಿರುಗಿ 0;
    }

#ಆದರೆ (NGX_LINUX)
    ಆದರೆ (err == NGX_EIO && c->ktls_recv) {
        ಹಿಂತಿರುಗಿ 0;
    }
#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ (err == 0
        || err == NGX_ECONNRESET
#ಆದರೆ (NGX_WIN32)
        || err == NGX_ECONNABORTED
#ಇಲ್ಲದಿದ್ದರೆ
        || err == NGX_EPIPE
#ಆದರೆ_ಅಂತ್ಯ
        || err == NGX_ENOTCONN
        || err == NGX_ETIMEDOUT
        || err == NGX_ECONNREFUSED
        || err == NGX_ENETDOWN
        || err == NGX_ENETUNREACH
        || err == NGX_EHOSTDOWN
        || err == NGX_EHOSTUNREACH)
    {
        ಆಯ್ಕೆ (c->log_error) {

        ಸ್ಥಿತಿ NGX_ERROR_IGNORE_EMSGSIZE:
        ಸ್ಥಿತಿ NGX_ERROR_IGNORE_EINVAL:
        ಸ್ಥಿತಿ NGX_ERROR_IGNORE_ECONNRESET:
        ಸ್ಥಿತಿ NGX_ERROR_INFO:
            level = NGX_LOG_INFO;
            ನಿಲ್ಲಿಸು;

        ಪೂರ್ವನಿಯೋಜಿತ:
            level = NGX_LOG_ERR;
        }

    } ಇಲ್ಲದಿದ್ದರೆ {
        level = NGX_LOG_ALERT;
    }

    ngx_log_error(level, c->ಲಘುಗಣಕ, err, ಪಠ್ಯ);

    ಹಿಂತಿರುಗಿ NGX_ERROR;
}
