
/*
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <ngx_event.h>


#define NGX_SYSLOG_MAX_STR                                                    \
    NGX_MAX_ERROR_STR + حجم("<255>Jan 01 00:00:00 ") - 1                   \
    + (NGX_MAXHOSTNAMELEN - 1) + 1 /* space */                                \
    + 32 /* tag */ + 2 /* colon, space */


ساكن حرف *ngx_syslog_parse_args(ngx_conf_t *cf, ngx_syslog_peer_t *peer);
ساكن ngx_int_t ngx_syslog_init_peer(ngx_syslog_peer_t *peer);
ساكن فراغ ngx_syslog_cleanup(فراغ *data);
ساكن u_char *ngx_syslog_log_error(ngx_log_t *log, u_char *buf, size_t len);


ساكن حرف  *facilities[] = {
    "kern", "user", "mail", "daemon", "auth", "intern", "lpr", "news", "uucp",
    "clock", "authpriv", "ftp", "ntp", "audit", "alert", "cron", "local0",
    "local1", "local2", "local3", "local4", "local5", "local6", "local7",
    عدم
};

/* note 'error/warn' like in nginx.conf, not 'err/warning' */
ساكن حرف  *severities[] = {
    "emerg", "alert", "crit", "error", "warn", "notice", "info", "debug", عدم
};

ساكن ngx_log_t    ngx_syslog_dummy_log;
ساكن ngx_event_t  ngx_syslog_dummy_event;


حرف *
ngx_syslog_process_conf(ngx_conf_t *cf, ngx_syslog_peer_t *peer)
{
    ngx_pool_cleanup_t  *cln;

    peer->facility = NGX_CONF_UNSET_UINT;
    peer->severity = NGX_CONF_UNSET_UINT;

    𐤀𐤌 (ngx_syslog_parse_args(cf, peer) != NGX_CONF_OK) {
        أرجع NGX_CONF_ERROR;
    }

    𐤀𐤌 (peer->server.sockaddr == عدم) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "no syslog server specified");
        أرجع NGX_CONF_ERROR;
    }

    𐤀𐤌 (peer->facility == NGX_CONF_UNSET_UINT) {
        peer->facility = 23; /* local7 */
    }

    𐤀𐤌 (peer->severity == NGX_CONF_UNSET_UINT) {
        peer->severity = 6; /* info */
    }

    𐤀𐤌 (peer->tag.data == عدم) {
        ngx_str_set(&peer->tag, "nginx");
    }

    peer->hostname = &cf->cycle->hostname;
    peer->logp = &cf->cycle->new_log;

    peer->conn.fd = (ngx_socket_t) -1;

    peer->conn.اقرأ = &ngx_syslog_dummy_event;
    peer->conn.اكتب = &ngx_syslog_dummy_event;

    ngx_syslog_dummy_event.log = &ngx_syslog_dummy_log;

    cln = ngx_pool_cleanup_add(cf->pool, 0);
    𐤀𐤌 (cln == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    cln->data = peer;
    cln->handler = ngx_syslog_cleanup;

    أرجع NGX_CONF_OK;
}


ساكن حرف *
ngx_syslog_parse_args(ngx_conf_t *cf, ngx_syslog_peer_t *peer)
{
    u_char      *p, *comma, c;
    size_t       len;
    ngx_str_t   *value;
    ngx_url_t    u;
    ngx_uint_t   i;

    value = cf->args->elts;

    p = value[1].data + حجم("syslog:") - 1;

    لكل ( ;; ) {
        comma = (u_char *) ngx_strchr(p, ',');

        𐤀𐤌 (comma != عدم) {
            len = comma - p;
            *comma = '\0';

        } وإلا {
            len = value[1].data + value[1].len - p;
        }

        𐤀𐤌 (ngx_strncmp(p, "server=", 7) == 0) {

            𐤀𐤌 (peer->server.sockaddr != عدم) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"server\"");
                أرجع NGX_CONF_ERROR;
            }

            ngx_memzero(&u, حجم(ngx_url_t));

            u.url.data = p + 7;
            u.url.len = len - 7;
            u.default_port = 514;

            𐤀𐤌 (ngx_parse_url(cf->pool, &u) != NGX_OK) {
                𐤀𐤌 (u.err) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "%s in syslog server \"%V\"",
                                       u.err, &u.url);
                }

                أرجع NGX_CONF_ERROR;
            }

            peer->server = u.addrs[0];

        } وإلا 𐤀𐤌 (ngx_strncmp(p, "facility=", 9) == 0) {

            𐤀𐤌 (peer->facility != NGX_CONF_UNSET_UINT) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"facility\"");
                أرجع NGX_CONF_ERROR;
            }

            لكل (i = 0; facilities[i] != عدم; i++) {

                𐤀𐤌 (ngx_strcmp(p + 9, facilities[i]) == 0) {
                    peer->facility = i;
                    اذهب next;
                }
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog facility \"%s\"", p + 9);
            أرجع NGX_CONF_ERROR;

        } وإلا 𐤀𐤌 (ngx_strncmp(p, "severity=", 9) == 0) {

            𐤀𐤌 (peer->severity != NGX_CONF_UNSET_UINT) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"severity\"");
                أرجع NGX_CONF_ERROR;
            }

            لكل (i = 0; severities[i] != عدم; i++) {

                𐤀𐤌 (ngx_strcmp(p + 9, severities[i]) == 0) {
                    peer->severity = i;
                    اذهب next;
                }
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog severity \"%s\"", p + 9);
            أرجع NGX_CONF_ERROR;

        } وإلا 𐤀𐤌 (ngx_strncmp(p, "tag=", 4) == 0) {

            𐤀𐤌 (peer->tag.data != عدم) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"tag\"");
                أرجع NGX_CONF_ERROR;
            }

            /*
             * RFC 3164: the TAG is a string of ABNF alphanumeric characters
             * that MUST NOT exceed 32 characters.
             */
            𐤀𐤌 (len - 4 > 32) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "syslog tag length exceeds 32");
                أرجع NGX_CONF_ERROR;
            }

            لكل (i = 4; i < len; i++) {
                c = ngx_tolower(p[i]);

                𐤀𐤌 (c < '0' || (c > '9' && c < 'a' && c != '_') || c > 'z') {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "syslog \"tag\" only allows "
                                       "alphanumeric characters "
                                       "and underscore");
                    أرجع NGX_CONF_ERROR;
                }
            }

            peer->tag.data = p + 4;
            peer->tag.len = len - 4;

        } وإلا 𐤀𐤌 (len == 10 && ngx_strncmp(p, "nohostname", 10) == 0) {
            peer->nohostname = 1;

        } وإلا {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog parameter \"%s\"", p);
            أرجع NGX_CONF_ERROR;
        }

    next:

        𐤀𐤌 (comma == عدم) {
            اكسر;
        }

        p = comma + 1;
    }

    أرجع NGX_CONF_OK;
}


u_char *
ngx_syslog_add_header(ngx_syslog_peer_t *peer, u_char *buf)
{
    ngx_uint_t  pri;

    pri = peer->facility * 8 + peer->severity;

    𐤀𐤌 (peer->nohostname) {
        أرجع ngx_sprintf(buf, "<%ui>%V %V: ", pri, &ngx_cached_syslog_time,
                           &peer->tag);
    }

    أرجع ngx_sprintf(buf, "<%ui>%V %V %V: ", pri, &ngx_cached_syslog_time,
                       peer->hostname, &peer->tag);
}


فراغ
ngx_syslog_writer(ngx_log_t *log, ngx_uint_t level, u_char *buf,
    size_t len)
{
    u_char             *p, msg[NGX_SYSLOG_MAX_STR];
    ngx_uint_t          head_len;
    ngx_syslog_peer_t  *peer;

    peer = log->wdata;

    𐤀𐤌 (peer->busy) {
        أرجع;
    }

    peer->busy = 1;
    peer->severity = level - 1;

    p = ngx_syslog_add_header(peer, msg);
    head_len = p - msg;

    len -= NGX_LINEFEED_SIZE;

    𐤀𐤌 (len > NGX_SYSLOG_MAX_STR - head_len) {
        len = NGX_SYSLOG_MAX_STR - head_len;
    }

    p = ngx_snprintf(p, len, "%s", buf);

    (فراغ) ngx_syslog_send(peer, msg, p - msg);

    peer->busy = 0;
}


ssize_t
ngx_syslog_send(ngx_syslog_peer_t *peer, u_char *buf, size_t len)
{
    ssize_t  n;

    𐤀𐤌 (peer->log.handler == عدم) {
        peer->log = *peer->logp;
        peer->log.handler = ngx_syslog_log_error;
        peer->log.data = peer;
        peer->log.action = "logging to syslog";
    }

    𐤀𐤌 (peer->conn.fd == (ngx_socket_t) -1) {
        𐤀𐤌 (ngx_syslog_init_peer(peer) != NGX_OK) {
            أرجع NGX_ERROR;
        }
    }

    𐤀𐤌 (ngx_send) {
        n = ngx_send(&peer->conn, buf, len);

    } وإلا {
        /* event module has not yet set ngx_io */
        n = ngx_os_io.send(&peer->conn, buf, len);
    }

    𐤀𐤌 (n == NGX_ERROR) {

        𐤀𐤌 (ngx_close_socket(peer->conn.fd) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        peer->conn.fd = (ngx_socket_t) -1;
    }

    أرجع n;
}


ساكن ngx_int_t
ngx_syslog_init_peer(ngx_syslog_peer_t *peer)
{
    ngx_socket_t  fd;

    fd = ngx_socket(peer->server.sockaddr->sa_family, SOCK_DGRAM, 0);
    𐤀𐤌 (fd == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_socket_n " failed");
        أرجع NGX_ERROR;
    }

    𐤀𐤌 (ngx_nonblocking(fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_nonblocking_n " failed");
        اذهب failed;
    }

    𐤀𐤌 (connect(fd, peer->server.sockaddr, peer->server.socklen) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      "connect() failed");
        اذهب failed;
    }

    peer->conn.fd = fd;
    peer->conn.log = &peer->log;

    /* UDP sockets are always ready to write */
    peer->conn.اكتب->ready = 1;

    أرجع NGX_OK;

failed:

    𐤀𐤌 (ngx_close_socket(fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_close_socket_n " failed");
    }

    أرجع NGX_ERROR;
}


ساكن فراغ
ngx_syslog_cleanup(فراغ *data)
{
    ngx_syslog_peer_t  *peer = data;

    /* prevents further use of this peer */
    peer->busy = 1;

    𐤀𐤌 (peer->conn.fd == (ngx_socket_t) -1) {
        أرجع;
    }

    𐤀𐤌 (ngx_close_socket(peer->conn.fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_close_socket_n " failed");
    }
}


ساكن u_char *
ngx_syslog_log_error(ngx_log_t *log, u_char *buf, size_t len)
{
    u_char             *p;
    ngx_syslog_peer_t  *peer;

    p = buf;

    𐤀𐤌 (log->action) {
        p = ngx_snprintf(buf, len, " while %s", log->action);
        len -= p - buf;
    }

    peer = log->data;

    𐤀𐤌 (peer) {
        p = ngx_snprintf(p, len, ", server: %V", &peer->server.name);
    }

    أرجع p;
}
