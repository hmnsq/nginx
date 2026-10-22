
/*
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>
#ಅನ್ತರ್ಭಾವ <ngx_event.h>


#define NGX_SYSLOG_MAX_STR                                                    \
    NGX_MAX_ERROR_STR + ಪರಿಮಾಣ("<255>Jan 01 00:00:00 ") - 1                   \
    + (NGX_MAXHOSTNAMELEN - 1) + 1 /* space */                                \
    + 32 /* tag */ + 2 /* colon, space */


ಸ್ಥಿರ ವರ್ಣ *ngx_syslog_parse_args(ngx_conf_t *cf, ngx_syslog_peer_t *peer);
ಸ್ಥಿರ ngx_int_t ngx_syslog_init_peer(ngx_syslog_peer_t *peer);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_syslog_cleanup(ಶೂನ್ಯ *data);
ಸ್ಥಿರ u_char *ngx_syslog_log_error(ngx_log_t *log, u_char *buf, size_t len);


ಸ್ಥಿರ ವರ್ಣ  *facilities[] = {
    "kern", "user", "mail", "daemon", "auth", "intern", "lpr", "news", "uucp",
    "clock", "authpriv", "ftp", "ntp", "audit", "alert", "cron", "local0",
    "local1", "local2", "local3", "local4", "local5", "local6", "local7",
    ರಿಕ್ತ
};

/* note 'error/warn' like in nginx.conf, not 'err/warning' */
ಸ್ಥಿರ ವರ್ಣ  *severities[] = {
    "emerg", "alert", "crit", "error", "warn", "notice", "info", "debug", ರಿಕ್ತ
};

ಸ್ಥಿರ ngx_log_t    ngx_syslog_dummy_log;
ಸ್ಥಿರ ngx_event_t  ngx_syslog_dummy_event;


ವರ್ಣ *
ngx_syslog_process_conf(ngx_conf_t *cf, ngx_syslog_peer_t *peer)
{
    ngx_pool_cleanup_t  *cln;

    peer->facility = NGX_CONF_UNSET_UINT;
    peer->severity = NGX_CONF_UNSET_UINT;

    ಯದಿ (ngx_syslog_parse_args(cf, peer) != NGX_CONF_OK) {
        ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
    }

    ಯದಿ (peer->server.sockaddr == ರಿಕ್ತ) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "no syslog server specified");
        ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
    }

    ಯದಿ (peer->facility == NGX_CONF_UNSET_UINT) {
        peer->facility = 23; /* local7 */
    }

    ಯದಿ (peer->severity == NGX_CONF_UNSET_UINT) {
        peer->severity = 6; /* info */
    }

    ಯದಿ (peer->tag.data == ರಿಕ್ತ) {
        ngx_str_set(&peer->tag, "nginx");
    }

    peer->hostname = &cf->cycle->hostname;
    peer->logp = &cf->cycle->new_log;

    peer->conn.fd = (ngx_socket_t) -1;

    peer->conn.ಪಠ = &ngx_syslog_dummy_event;
    peer->conn.ಲಿಖ = &ngx_syslog_dummy_event;

    ngx_syslog_dummy_event.log = &ngx_syslog_dummy_log;

    cln = ngx_pool_cleanup_add(cf->pool, 0);
    ಯದಿ (cln == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
    }

    cln->data = peer;
    cln->handler = ngx_syslog_cleanup;

    ಪ್ರತ್ಯಾಗಮ NGX_CONF_OK;
}


ಸ್ಥಿರ ವರ್ಣ *
ngx_syslog_parse_args(ngx_conf_t *cf, ngx_syslog_peer_t *peer)
{
    u_char      *p, *comma, c;
    size_t       len;
    ngx_str_t   *ಮೌಲ್ಯ;
    ngx_url_t    u;
    ngx_uint_t   i;

    ಮೌಲ್ಯ = cf->args->elts;

    p = ಮೌಲ್ಯ[1].data + ಪರಿಮಾಣ("syslog:") - 1;

    ಪ್ರತಿ ( ;; ) {
        comma = (u_char *) ngx_strchr(p, ',');

        ಯದಿ (comma != ರಿಕ್ತ) {
            len = comma - p;
            *comma = '\0';

        } ಅನ್ಯಥಾ {
            len = ಮೌಲ್ಯ[1].data + ಮೌಲ್ಯ[1].len - p;
        }

        ಯದಿ (ngx_strncmp(p, "server=", 7) == 0) {

            ಯದಿ (peer->server.sockaddr != ರಿಕ್ತ) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"server\"");
                ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
            }

            ngx_memzero(&u, ಪರಿಮಾಣ(ngx_url_t));

            u.url.data = p + 7;
            u.url.len = len - 7;
            u.default_port = 514;

            ಯದಿ (ngx_parse_url(cf->pool, &u) != NGX_OK) {
                ಯದಿ (u.err) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "%s in syslog server \"%V\"",
                                       u.err, &u.url);
                }

                ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
            }

            peer->server = u.addrs[0];

        } ಅನ್ಯಥಾ ಯದಿ (ngx_strncmp(p, "facility=", 9) == 0) {

            ಯದಿ (peer->facility != NGX_CONF_UNSET_UINT) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"facility\"");
                ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
            }

            ಪ್ರತಿ (i = 0; facilities[i] != ರಿಕ್ತ; i++) {

                ಯದಿ (ngx_strcmp(p + 9, facilities[i]) == 0) {
                    peer->facility = i;
                    ಗಚ್ಛ next;
                }
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog facility \"%s\"", p + 9);
            ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;

        } ಅನ್ಯಥಾ ಯದಿ (ngx_strncmp(p, "severity=", 9) == 0) {

            ಯದಿ (peer->severity != NGX_CONF_UNSET_UINT) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"severity\"");
                ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
            }

            ಪ್ರತಿ (i = 0; severities[i] != ರಿಕ್ತ; i++) {

                ಯದಿ (ngx_strcmp(p + 9, severities[i]) == 0) {
                    peer->severity = i;
                    ಗಚ್ಛ next;
                }
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog severity \"%s\"", p + 9);
            ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;

        } ಅನ್ಯಥಾ ಯದಿ (ngx_strncmp(p, "tag=", 4) == 0) {

            ಯದಿ (peer->tag.data != ರಿಕ್ತ) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"tag\"");
                ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
            }

            /*
             * RFC 3164: the TAG is a string of ABNF alphanumeric characters
             * that MUST NOT exceed 32 characters.
             */
            ಯದಿ (len - 4 > 32) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "syslog tag length exceeds 32");
                ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
            }

            ಪ್ರತಿ (i = 4; i < len; i++) {
                c = ngx_tolower(p[i]);

                ಯದಿ (c < '0' || (c > '9' && c < 'a' && c != '_') || c > 'z') {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "syslog \"tag\" only allows "
                                       "alphanumeric characters "
                                       "and underscore");
                    ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
                }
            }

            peer->tag.data = p + 4;
            peer->tag.len = len - 4;

        } ಅನ್ಯಥಾ ಯದಿ (len == 10 && ngx_strncmp(p, "nohostname", 10) == 0) {
            peer->nohostname = 1;

        } ಅನ್ಯಥಾ {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog parameter \"%s\"", p);
            ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
        }

    next:

        ಯದಿ (comma == ರಿಕ್ತ) {
            ಭಙ್ಗ;
        }

        p = comma + 1;
    }

    ಪ್ರತ್ಯಾಗಮ NGX_CONF_OK;
}


u_char *
ngx_syslog_add_header(ngx_syslog_peer_t *peer, u_char *buf)
{
    ngx_uint_t  pri;

    pri = peer->facility * 8 + peer->severity;

    ಯದಿ (peer->nohostname) {
        ಪ್ರತ್ಯಾಗಮ ngx_sprintf(buf, "<%ui>%V %V: ", pri, &ngx_cached_syslog_time,
                           &peer->tag);
    }

    ಪ್ರತ್ಯಾಗಮ ngx_sprintf(buf, "<%ui>%V %V %V: ", pri, &ngx_cached_syslog_time,
                       peer->hostname, &peer->tag);
}


ಶೂನ್ಯ
ngx_syslog_writer(ngx_log_t *log, ngx_uint_t level, u_char *buf,
    size_t len)
{
    u_char             *p, ಸಂದೇಶ[NGX_SYSLOG_MAX_STR];
    ngx_uint_t          head_len;
    ngx_syslog_peer_t  *peer;

    peer = log->wdata;

    ಯದಿ (peer->busy) {
        ಪ್ರತ್ಯಾಗಮ;
    }

    peer->busy = 1;
    peer->severity = level - 1;

    p = ngx_syslog_add_header(peer, ಸಂದೇಶ);
    head_len = p - ಸಂದೇಶ;

    len -= NGX_LINEFEED_SIZE;

    ಯದಿ (len > NGX_SYSLOG_MAX_STR - head_len) {
        len = NGX_SYSLOG_MAX_STR - head_len;
    }

    p = ngx_snprintf(p, len, "%s", buf);

    (ಶೂನ್ಯ) ngx_syslog_send(peer, ಸಂದೇಶ, p - ಸಂದೇಶ);

    peer->busy = 0;
}


ssize_t
ngx_syslog_send(ngx_syslog_peer_t *peer, u_char *buf, size_t len)
{
    ssize_t  n;

    ಯದಿ (peer->log.handler == ರಿಕ್ತ) {
        peer->log = *peer->logp;
        peer->log.handler = ngx_syslog_log_error;
        peer->log.data = peer;
        peer->log.action = "logging to syslog";
    }

    ಯದಿ (peer->conn.fd == (ngx_socket_t) -1) {
        ಯದಿ (ngx_syslog_init_peer(peer) != NGX_OK) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }
    }

    ಯದಿ (ngx_send) {
        n = ngx_send(&peer->conn, buf, len);

    } ಅನ್ಯಥಾ {
        /* event module has not yet set ngx_io */
        n = ngx_os_io.send(&peer->conn, buf, len);
    }

    ಯದಿ (n == NGX_ERROR) {

        ಯದಿ (ngx_close_socket(peer->conn.fd) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        peer->conn.fd = (ngx_socket_t) -1;
    }

    ಪ್ರತ್ಯಾಗಮ n;
}


ಸ್ಥಿರ ngx_int_t
ngx_syslog_init_peer(ngx_syslog_peer_t *peer)
{
    ngx_socket_t  fd;

    fd = ngx_socket(peer->server.sockaddr->sa_family, SOCK_DGRAM, 0);
    ಯದಿ (fd == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_socket_n " failed");
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಯದಿ (ngx_nonblocking(fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_nonblocking_n " failed");
        ಗಚ್ಛ failed;
    }

    ಯದಿ (connect(fd, peer->server.sockaddr, peer->server.socklen) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      "connect() failed");
        ಗಚ್ಛ failed;
    }

    peer->conn.fd = fd;
    peer->conn.log = &peer->log;

    /* UDP sockets are always ready to write */
    peer->conn.ಲಿಖ->ready = 1;

    ಪ್ರತ್ಯಾಗಮ NGX_OK;

failed:

    ಯದಿ (ngx_close_socket(fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_close_socket_n " failed");
    }

    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_syslog_cleanup(ಶೂನ್ಯ *data)
{
    ngx_syslog_peer_t  *peer = data;

    /* prevents further use of this peer */
    peer->busy = 1;

    ಯದಿ (peer->conn.fd == (ngx_socket_t) -1) {
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ (ngx_close_socket(peer->conn.fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_close_socket_n " failed");
    }
}


ಸ್ಥಿರ u_char *
ngx_syslog_log_error(ngx_log_t *log, u_char *buf, size_t len)
{
    u_char             *p;
    ngx_syslog_peer_t  *peer;

    p = buf;

    ಯದಿ (log->action) {
        p = ngx_snprintf(buf, len, " while %s", log->action);
        len -= p - buf;
    }

    peer = log->data;

    ಯದಿ (peer) {
        p = ngx_snprintf(p, len, ", server: %V", &peer->server.name);
    }

    ಪ್ರತ್ಯಾಗಮ p;
}
