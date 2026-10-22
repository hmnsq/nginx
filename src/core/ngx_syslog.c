
/*
 * Copyright (C) Nginx, Inc.
 */


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>
#അന്തര്ഭാവ <ngx_event.h>


#define NGX_SYSLOG_MAX_STR                                                    \
    NGX_MAX_ERROR_STR + വലുപ്പം("<255>Jan 01 00:00:00 ") - 1                   \
    + (NGX_MAXHOSTNAMELEN - 1) + 1 /* space */                                \
    + 32 /* tag */ + 2 /* colon, space */


സ്ഥിതം അക്ഷരം *ngx_syslog_parse_args(ngx_conf_t *cf, ngx_syslog_peer_t *peer);
സ്ഥിതം ngx_int_t ngx_syslog_init_peer(ngx_syslog_peer_t *peer);
സ്ഥിതം ശൂന്യം ngx_syslog_cleanup(ശൂന്യം *data);
സ്ഥിതം u_char *ngx_syslog_log_error(ngx_log_t *log, u_char *buf, size_t len);


സ്ഥിതം അക്ഷരം  *facilities[] = {
    "kern", "user", "mail", "daemon", "auth", "intern", "lpr", "news", "uucp",
    "clock", "authpriv", "ftp", "ntp", "audit", "alert", "cron", "local0",
    "local1", "local2", "local3", "local4", "local5", "local6", "local7",
    ശൂന്യ
};

/* note 'error/warn' like in nginx.conf, not 'err/warning' */
സ്ഥിതം അക്ഷരം  *severities[] = {
    "emerg", "alert", "crit", "error", "warn", "notice", "info", "debug", ശൂന്യ
};

സ്ഥിതം ngx_log_t    ngx_syslog_dummy_log;
സ്ഥിതം ngx_event_t  ngx_syslog_dummy_event;


അക്ഷരം *
ngx_syslog_process_conf(ngx_conf_t *cf, ngx_syslog_peer_t *peer)
{
    ngx_pool_cleanup_t  *cln;

    peer->facility = NGX_CONF_UNSET_UINT;
    peer->severity = NGX_CONF_UNSET_UINT;

    എങ്കിൽ (ngx_syslog_parse_args(cf, peer) != NGX_CONF_OK) {
        തിരികെ NGX_CONF_ERROR;
    }

    എങ്കിൽ (peer->server.sockaddr == ശൂന്യ) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "no syslog server specified");
        തിരികെ NGX_CONF_ERROR;
    }

    എങ്കിൽ (peer->facility == NGX_CONF_UNSET_UINT) {
        peer->facility = 23; /* local7 */
    }

    എങ്കിൽ (peer->severity == NGX_CONF_UNSET_UINT) {
        peer->severity = 6; /* info */
    }

    എങ്കിൽ (peer->tag.data == ശൂന്യ) {
        ngx_str_set(&peer->tag, "nginx");
    }

    peer->hostname = &cf->cycle->hostname;
    peer->logp = &cf->cycle->new_log;

    peer->conn.fd = (ngx_socket_t) -1;

    peer->conn.പഠ = &ngx_syslog_dummy_event;
    peer->conn.ലിഖ = &ngx_syslog_dummy_event;

    ngx_syslog_dummy_event.log = &ngx_syslog_dummy_log;

    cln = ngx_pool_cleanup_add(cf->pool, 0);
    എങ്കിൽ (cln == ശൂന്യ) {
        തിരികെ NGX_CONF_ERROR;
    }

    cln->data = peer;
    cln->handler = ngx_syslog_cleanup;

    തിരികെ NGX_CONF_OK;
}


സ്ഥിതം അക്ഷരം *
ngx_syslog_parse_args(ngx_conf_t *cf, ngx_syslog_peer_t *peer)
{
    u_char      *p, *comma, c;
    size_t       len;
    ngx_str_t   *മൂല്യം;
    ngx_url_t    u;
    ngx_uint_t   i;

    മൂല്യം = cf->args->elts;

    p = മൂല്യം[1].data + വലുപ്പം("syslog:") - 1;

    വേണ്ടി ( ;; ) {
        comma = (u_char *) ngx_strchr(p, ',');

        എങ്കിൽ (comma != ശൂന്യ) {
            len = comma - p;
            *comma = '\0';

        } അല്ലെങ്കിൽ {
            len = മൂല്യം[1].data + മൂല്യം[1].len - p;
        }

        എങ്കിൽ (ngx_strncmp(p, "server=", 7) == 0) {

            എങ്കിൽ (peer->server.sockaddr != ശൂന്യ) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"server\"");
                തിരികെ NGX_CONF_ERROR;
            }

            ngx_memzero(&u, വലുപ്പം(ngx_url_t));

            u.url.data = p + 7;
            u.url.len = len - 7;
            u.default_port = 514;

            എങ്കിൽ (ngx_parse_url(cf->pool, &u) != NGX_OK) {
                എങ്കിൽ (u.err) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "%s in syslog server \"%V\"",
                                       u.err, &u.url);
                }

                തിരികെ NGX_CONF_ERROR;
            }

            peer->server = u.addrs[0];

        } അല്ലെങ്കിൽ എങ്കിൽ (ngx_strncmp(p, "facility=", 9) == 0) {

            എങ്കിൽ (peer->facility != NGX_CONF_UNSET_UINT) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"facility\"");
                തിരികെ NGX_CONF_ERROR;
            }

            വേണ്ടി (i = 0; facilities[i] != ശൂന്യ; i++) {

                എങ്കിൽ (ngx_strcmp(p + 9, facilities[i]) == 0) {
                    peer->facility = i;
                    പോകുക next;
                }
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog facility \"%s\"", p + 9);
            തിരികെ NGX_CONF_ERROR;

        } അല്ലെങ്കിൽ എങ്കിൽ (ngx_strncmp(p, "severity=", 9) == 0) {

            എങ്കിൽ (peer->severity != NGX_CONF_UNSET_UINT) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"severity\"");
                തിരികെ NGX_CONF_ERROR;
            }

            വേണ്ടി (i = 0; severities[i] != ശൂന്യ; i++) {

                എങ്കിൽ (ngx_strcmp(p + 9, severities[i]) == 0) {
                    peer->severity = i;
                    പോകുക next;
                }
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog severity \"%s\"", p + 9);
            തിരികെ NGX_CONF_ERROR;

        } അല്ലെങ്കിൽ എങ്കിൽ (ngx_strncmp(p, "tag=", 4) == 0) {

            എങ്കിൽ (peer->tag.data != ശൂന്യ) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"tag\"");
                തിരികെ NGX_CONF_ERROR;
            }

            /*
             * RFC 3164: the TAG is a string of ABNF alphanumeric characters
             * that MUST NOT exceed 32 characters.
             */
            എങ്കിൽ (len - 4 > 32) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "syslog tag length exceeds 32");
                തിരികെ NGX_CONF_ERROR;
            }

            വേണ്ടി (i = 4; i < len; i++) {
                c = ngx_tolower(p[i]);

                എങ്കിൽ (c < '0' || (c > '9' && c < 'a' && c != '_') || c > 'z') {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "syslog \"tag\" only allows "
                                       "alphanumeric characters "
                                       "and underscore");
                    തിരികെ NGX_CONF_ERROR;
                }
            }

            peer->tag.data = p + 4;
            peer->tag.len = len - 4;

        } അല്ലെങ്കിൽ എങ്കിൽ (len == 10 && ngx_strncmp(p, "nohostname", 10) == 0) {
            peer->nohostname = 1;

        } അല്ലെങ്കിൽ {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog parameter \"%s\"", p);
            തിരികെ NGX_CONF_ERROR;
        }

    next:

        എങ്കിൽ (comma == ശൂന്യ) {
            നിർത്തുക;
        }

        p = comma + 1;
    }

    തിരികെ NGX_CONF_OK;
}


u_char *
ngx_syslog_add_header(ngx_syslog_peer_t *peer, u_char *buf)
{
    ngx_uint_t  pri;

    pri = peer->facility * 8 + peer->severity;

    എങ്കിൽ (peer->nohostname) {
        തിരികെ ngx_sprintf(buf, "<%ui>%V %V: ", pri, &ngx_cached_syslog_time,
                           &peer->tag);
    }

    തിരികെ ngx_sprintf(buf, "<%ui>%V %V %V: ", pri, &ngx_cached_syslog_time,
                       peer->hostname, &peer->tag);
}


ശൂന്യം
ngx_syslog_writer(ngx_log_t *log, ngx_uint_t level, u_char *buf,
    size_t len)
{
    u_char             *p, സന്ദേശം[NGX_SYSLOG_MAX_STR];
    ngx_uint_t          head_len;
    ngx_syslog_peer_t  *peer;

    peer = log->wdata;

    എങ്കിൽ (peer->busy) {
        തിരികെ;
    }

    peer->busy = 1;
    peer->severity = level - 1;

    p = ngx_syslog_add_header(peer, സന്ദേശം);
    head_len = p - സന്ദേശം;

    len -= NGX_LINEFEED_SIZE;

    എങ്കിൽ (len > NGX_SYSLOG_MAX_STR - head_len) {
        len = NGX_SYSLOG_MAX_STR - head_len;
    }

    p = ngx_snprintf(p, len, "%s", buf);

    (ശൂന്യം) ngx_syslog_send(peer, സന്ദേശം, p - സന്ദേശം);

    peer->busy = 0;
}


ssize_t
ngx_syslog_send(ngx_syslog_peer_t *peer, u_char *buf, size_t len)
{
    ssize_t  n;

    എങ്കിൽ (peer->log.handler == ശൂന്യ) {
        peer->log = *peer->logp;
        peer->log.handler = ngx_syslog_log_error;
        peer->log.data = peer;
        peer->log.action = "logging to syslog";
    }

    എങ്കിൽ (peer->conn.fd == (ngx_socket_t) -1) {
        എങ്കിൽ (ngx_syslog_init_peer(peer) != NGX_OK) {
            തിരികെ NGX_ERROR;
        }
    }

    എങ്കിൽ (ngx_send) {
        n = ngx_send(&peer->conn, buf, len);

    } അല്ലെങ്കിൽ {
        /* event module has not yet set ngx_io */
        n = ngx_os_io.send(&peer->conn, buf, len);
    }

    എങ്കിൽ (n == NGX_ERROR) {

        എങ്കിൽ (ngx_close_socket(peer->conn.fd) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        peer->conn.fd = (ngx_socket_t) -1;
    }

    തിരികെ n;
}


സ്ഥിതം ngx_int_t
ngx_syslog_init_peer(ngx_syslog_peer_t *peer)
{
    ngx_socket_t  fd;

    fd = ngx_socket(peer->server.sockaddr->sa_family, SOCK_DGRAM, 0);
    എങ്കിൽ (fd == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_socket_n " failed");
        തിരികെ NGX_ERROR;
    }

    എങ്കിൽ (ngx_nonblocking(fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_nonblocking_n " failed");
        പോകുക failed;
    }

    എങ്കിൽ (connect(fd, peer->server.sockaddr, peer->server.socklen) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      "connect() failed");
        പോകുക failed;
    }

    peer->conn.fd = fd;
    peer->conn.log = &peer->log;

    /* UDP sockets are always ready to write */
    peer->conn.ലിഖ->ready = 1;

    തിരികെ NGX_OK;

failed:

    എങ്കിൽ (ngx_close_socket(fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_close_socket_n " failed");
    }

    തിരികെ NGX_ERROR;
}


സ്ഥിതം ശൂന്യം
ngx_syslog_cleanup(ശൂന്യം *data)
{
    ngx_syslog_peer_t  *peer = data;

    /* prevents further use of this peer */
    peer->busy = 1;

    എങ്കിൽ (peer->conn.fd == (ngx_socket_t) -1) {
        തിരികെ;
    }

    എങ്കിൽ (ngx_close_socket(peer->conn.fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_close_socket_n " failed");
    }
}


സ്ഥിതം u_char *
ngx_syslog_log_error(ngx_log_t *log, u_char *buf, size_t len)
{
    u_char             *p;
    ngx_syslog_peer_t  *peer;

    p = buf;

    എങ്കിൽ (log->action) {
        p = ngx_snprintf(buf, len, " while %s", log->action);
        len -= p - buf;
    }

    peer = log->data;

    എങ്കിൽ (peer) {
        p = ngx_snprintf(p, len, ", server: %V", &peer->server.name);
    }

    തിരികെ p;
}
