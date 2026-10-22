
/*
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>
#অন্তর্ভাব <ngx_event.h>


#define NGX_SYSLOG_MAX_STR                                                    \
    NGX_MAX_ERROR_STR + আকার("<255>Jan 01 00:00:00 ") - 1                   \
    + (NGX_MAXHOSTNAMELEN - 1) + 1 /* space */                                \
    + 32 /* tag */ + 2 /* colon, space */


স্থির অক্ষর *ngx_syslog_parse_args(ngx_conf_t *cf, ngx_syslog_peer_t *peer);
স্থির ngx_int_t ngx_syslog_init_peer(ngx_syslog_peer_t *peer);
স্থির শূন্য ngx_syslog_cleanup(শূন্য *তথ্য);
স্থির u_char *ngx_syslog_log_error(ngx_log_t *log, u_char *buf, আকার_t len);


স্থির অক্ষর  *facilities[] = {
    "kern", "user", "mail", "daemon", "auth", "intern", "lpr", "news", "uucp",
    "clock", "authpriv", "ftp", "ntp", "audit", "alert", "cron", "local0",
    "local1", "local2", "local3", "local4", "local5", "local6", "local7",
    ফাঁকা
};

/* note 'error/warn' like in nginx.conf, not 'err/warning' */
স্থির অক্ষর  *severities[] = {
    "emerg", "alert", "crit", "error", "warn", "notice", "info", "debug", ফাঁকা
};

স্থির ngx_log_t    ngx_syslog_dummy_log;
স্থির ngx_event_t  ngx_syslog_dummy_event;


অক্ষর *
ngx_syslog_process_conf(ngx_conf_t *cf, ngx_syslog_peer_t *peer)
{
    ngx_pool_cleanup_t  *cln;

    peer->facility = NGX_CONF_UNSET_UINT;
    peer->severity = NGX_CONF_UNSET_UINT;

    যদি (ngx_syslog_parse_args(cf, peer) != NGX_CONF_OK) {
        ফেরত NGX_CONF_ERROR;
    }

    যদি (peer->server.sockaddr == ফাঁকা) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "no syslog server specified");
        ফেরত NGX_CONF_ERROR;
    }

    যদি (peer->facility == NGX_CONF_UNSET_UINT) {
        peer->facility = 23; /* local7 */
    }

    যদি (peer->severity == NGX_CONF_UNSET_UINT) {
        peer->severity = 6; /* info */
    }

    যদি (peer->tag.তথ্য == ফাঁকা) {
        ngx_str_set(&peer->tag, "nginx");
    }

    peer->hostname = &cf->cycle->hostname;
    peer->logp = &cf->cycle->new_log;

    peer->conn.fd = (ngx_socket_t) -1;

    peer->conn.পঠ = &ngx_syslog_dummy_event;
    peer->conn.লিখ = &ngx_syslog_dummy_event;

    ngx_syslog_dummy_event.log = &ngx_syslog_dummy_log;

    cln = ngx_pool_cleanup_add(cf->pool, 0);
    যদি (cln == ফাঁকা) {
        ফেরত NGX_CONF_ERROR;
    }

    cln->তথ্য = peer;
    cln->handler = ngx_syslog_cleanup;

    ফেরত NGX_CONF_OK;
}


স্থির অক্ষর *
ngx_syslog_parse_args(ngx_conf_t *cf, ngx_syslog_peer_t *peer)
{
    u_char      *p, *comma, c;
    আকার_t       len;
    ngx_str_t   *মান;
    ngx_url_t    u;
    ngx_uint_t   i;

    মান = cf->args->elts;

    p = মান[1].তথ্য + আকার("syslog:") - 1;

    জন্য ( ;; ) {
        comma = (u_char *) ngx_strchr(p, ',');

        যদি (comma != ফাঁকা) {
            len = comma - p;
            *comma = '\0';

        } অন্যথা {
            len = মান[1].তথ্য + মান[1].len - p;
        }

        যদি (ngx_strncmp(p, "server=", 7) == 0) {

            যদি (peer->server.sockaddr != ফাঁকা) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"server\"");
                ফেরত NGX_CONF_ERROR;
            }

            ngx_memzero(&u, আকার(ngx_url_t));

            u.url.তথ্য = p + 7;
            u.url.len = len - 7;
            u.default_port = 514;

            যদি (ngx_parse_url(cf->pool, &u) != NGX_OK) {
                যদি (u.err) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "%s in syslog server \"%V\"",
                                       u.err, &u.url);
                }

                ফেরত NGX_CONF_ERROR;
            }

            peer->server = u.addrs[0];

        } অন্যথা যদি (ngx_strncmp(p, "facility=", 9) == 0) {

            যদি (peer->facility != NGX_CONF_UNSET_UINT) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"facility\"");
                ফেরত NGX_CONF_ERROR;
            }

            জন্য (i = 0; facilities[i] != ফাঁকা; i++) {

                যদি (ngx_strcmp(p + 9, facilities[i]) == 0) {
                    peer->facility = i;
                    যাও পরবর্তী;
                }
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog facility \"%s\"", p + 9);
            ফেরত NGX_CONF_ERROR;

        } অন্যথা যদি (ngx_strncmp(p, "severity=", 9) == 0) {

            যদি (peer->severity != NGX_CONF_UNSET_UINT) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"severity\"");
                ফেরত NGX_CONF_ERROR;
            }

            জন্য (i = 0; severities[i] != ফাঁকা; i++) {

                যদি (ngx_strcmp(p + 9, severities[i]) == 0) {
                    peer->severity = i;
                    যাও পরবর্তী;
                }
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog severity \"%s\"", p + 9);
            ফেরত NGX_CONF_ERROR;

        } অন্যথা যদি (ngx_strncmp(p, "tag=", 4) == 0) {

            যদি (peer->tag.তথ্য != ফাঁকা) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"tag\"");
                ফেরত NGX_CONF_ERROR;
            }

            /*
             * RFC 3164: the TAG is a string of ABNF alphanumeric characters
             * that MUST NOT exceed 32 characters.
             */
            যদি (len - 4 > 32) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "syslog tag length exceeds 32");
                ফেরত NGX_CONF_ERROR;
            }

            জন্য (i = 4; i < len; i++) {
                c = ngx_tolower(p[i]);

                যদি (c < '0' || (c > '9' && c < 'a' && c != '_') || c > 'z') {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "syslog \"tag\" only allows "
                                       "alphanumeric characters "
                                       "and underscore");
                    ফেরত NGX_CONF_ERROR;
                }
            }

            peer->tag.তথ্য = p + 4;
            peer->tag.len = len - 4;

        } অন্যথা যদি (len == 10 && ngx_strncmp(p, "nohostname", 10) == 0) {
            peer->nohostname = 1;

        } অন্যথা {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog parameter \"%s\"", p);
            ফেরত NGX_CONF_ERROR;
        }

    পরবর্তী:

        যদি (comma == ফাঁকা) {
            বিরতি;
        }

        p = comma + 1;
    }

    ফেরত NGX_CONF_OK;
}


u_char *
ngx_syslog_add_header(ngx_syslog_peer_t *peer, u_char *buf)
{
    ngx_uint_t  pri;

    pri = peer->facility * 8 + peer->severity;

    যদি (peer->nohostname) {
        ফেরত ngx_sprintf(buf, "<%ui>%V %V: ", pri, &ngx_cached_syslog_time,
                           &peer->tag);
    }

    ফেরত ngx_sprintf(buf, "<%ui>%V %V %V: ", pri, &ngx_cached_syslog_time,
                       peer->hostname, &peer->tag);
}


শূন্য
ngx_syslog_writer(ngx_log_t *log, ngx_uint_t level, u_char *buf,
    আকার_t len)
{
    u_char             *p, বার্তা[NGX_SYSLOG_MAX_STR];
    ngx_uint_t          শীর্ষ_len;
    ngx_syslog_peer_t  *peer;

    peer = log->wdata;

    যদি (peer->busy) {
        ফেরত;
    }

    peer->busy = 1;
    peer->severity = level - 1;

    p = ngx_syslog_add_header(peer, বার্তা);
    শীর্ষ_len = p - বার্তা;

    len -= NGX_LINEFEED_আকার;

    যদি (len > NGX_SYSLOG_MAX_STR - শীর্ষ_len) {
        len = NGX_SYSLOG_MAX_STR - শীর্ষ_len;
    }

    p = ngx_snprintf(p, len, "%s", buf);

    (শূন্য) ngx_syslog_send(peer, বার্তা, p - বার্তা);

    peer->busy = 0;
}


ssize_t
ngx_syslog_send(ngx_syslog_peer_t *peer, u_char *buf, আকার_t len)
{
    ssize_t  n;

    যদি (peer->log.handler == ফাঁকা) {
        peer->log = *peer->logp;
        peer->log.handler = ngx_syslog_log_error;
        peer->log.তথ্য = peer;
        peer->log.action = "logging to syslog";
    }

    যদি (peer->conn.fd == (ngx_socket_t) -1) {
        যদি (ngx_syslog_init_peer(peer) != NGX_OK) {
            ফেরত NGX_ERROR;
        }
    }

    যদি (ngx_send) {
        n = ngx_send(&peer->conn, buf, len);

    } অন্যথা {
        /* event module has not yet set ngx_io */
        n = ngx_os_io.send(&peer->conn, buf, len);
    }

    যদি (n == NGX_ERROR) {

        যদি (ngx_close_socket(peer->conn.fd) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        peer->conn.fd = (ngx_socket_t) -1;
    }

    ফেরত n;
}


স্থির ngx_int_t
ngx_syslog_init_peer(ngx_syslog_peer_t *peer)
{
    ngx_socket_t  fd;

    fd = ngx_socket(peer->server.sockaddr->sa_family, SOCK_DGRAM, 0);
    যদি (fd == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_socket_n " failed");
        ফেরত NGX_ERROR;
    }

    যদি (ngx_nonblocking(fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_nonblocking_n " failed");
        যাও failed;
    }

    যদি (connect(fd, peer->server.sockaddr, peer->server.socklen) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      "connect() failed");
        যাও failed;
    }

    peer->conn.fd = fd;
    peer->conn.log = &peer->log;

    /* UDP sockets are always ready to write */
    peer->conn.লিখ->ready = 1;

    ফেরত NGX_OK;

failed:

    যদি (ngx_close_socket(fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_close_socket_n " failed");
    }

    ফেরত NGX_ERROR;
}


স্থির শূন্য
ngx_syslog_cleanup(শূন্য *তথ্য)
{
    ngx_syslog_peer_t  *peer = তথ্য;

    /* prevents further use of this peer */
    peer->busy = 1;

    যদি (peer->conn.fd == (ngx_socket_t) -1) {
        ফেরত;
    }

    যদি (ngx_close_socket(peer->conn.fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_close_socket_n " failed");
    }
}


স্থির u_char *
ngx_syslog_log_error(ngx_log_t *log, u_char *buf, আকার_t len)
{
    u_char             *p;
    ngx_syslog_peer_t  *peer;

    p = buf;

    যদি (log->action) {
        p = ngx_snprintf(buf, len, " while %s", log->action);
        len -= p - buf;
    }

    peer = log->তথ্য;

    যদি (peer) {
        p = ngx_snprintf(p, len, ", server: %V", &peer->server.name);
    }

    ফেরত p;
}
