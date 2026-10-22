
/*
 * Copyright (C) Nginx, Inc.
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_event.h>


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_SYSLOG_MAX_STR                                                    \
    NGX_MAX_ERROR_STR + ਆਕਾਰ("<255>Jan 01 00:00:00 ") - 1                   \
    + (NGX_MAXHOSTNAMELEN - 1) + 1 /* space */                                \
    + 32 /*ਟੈਗ*/ + 2 /* colon, space */


ਸਥਿਰ ਅੱਖਰ *ngx_syslog_parse_ਦਲੀਲਾਂ(ngx_conf_t *cf, ngx_syslog_peer_t *peer);
ਸਥਿਰ ngx_int_t ngx_syslog_init_peer(ngx_syslog_peer_t *peer);
ਸਥਿਰ ਖਾਲੀ ngx_syslog_cleanup(ਖਾਲੀ *ਅੰਕੜੇ);
ਸਥਿਰ u_char *ngx_syslog_log_error(ngx_log_t *ਲਘੂਗਣਕ, u_char *buf, ਮਾਪ_t len);


ਸਥਿਰ ਅੱਖਰ  *facilities[] = {
    "kern", "user", "mail", "daemon", "auth", "intern", "lpr", "news", "uucp",
    "clock", "authpriv", "ftp", "ntp", "audit", "alert", "cron", "local0",
    "local1", "local2", "local3", "local4", "local5", "local6", "local7",
    ਨਿਰਾ
};

/* note 'error/warn' like in nginx.conf, not 'err/warning' */
ਸਥਿਰ ਅੱਖਰ  *severities[] = {
    "emerg", "alert", "crit", "error", "warn", "notice", "info", "debug", ਨਿਰਾ
};

ਸਥਿਰ ngx_log_t    ngx_syslog_dummy_log;
ਸਥਿਰ ngx_event_t  ngx_syslog_dummy_event;


ਅੱਖਰ *
ngx_syslog_process_conf(ngx_conf_t *cf, ngx_syslog_peer_t *peer)
{
    ngx_pool_cleanup_t  *cln;

    peer->facility = NGX_CONF_UNSET_UINT;
    peer->severity = NGX_CONF_UNSET_UINT;

    ਜੇ (ngx_syslog_parse_ਦਲੀਲਾਂ(cf, peer) != NGX_CONF_OK) {
        ਵਾਪਸ NGX_CONF_ERROR;
    }

    ਜੇ (peer->server.sockaddr == ਨਿਰਾ) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "no syslog server specified");
        ਵਾਪਸ NGX_CONF_ERROR;
    }

    ਜੇ (peer->facility == NGX_CONF_UNSET_UINT) {
        peer->facility = 23; /* local7 */
    }

    ਜੇ (peer->severity == NGX_CONF_UNSET_UINT) {
        peer->severity = 6; /* info */
    }

    ਜੇ (peer->ਟੈਗ.ਅੰਕੜੇ == ਨਿਰਾ) {
        ngx_str_ਸੈੱਟ(&peer->ਟੈਗ, "nginx");
    }

    peer->hostname = &cf->ਚੱਕਰ->hostname;
    peer->logp = &cf->ਚੱਕਰ->new_log;

    peer->conn.fd = (ngx_socket_t) -1;

    peer->conn.ਪਠ = &ngx_syslog_dummy_event;
    peer->conn.ਲਿਖ = &ngx_syslog_dummy_event;

    ngx_syslog_dummy_event.ਲਘੂਗਣਕ = &ngx_syslog_dummy_log;

    cln = ngx_pool_cleanup_ਜੋੜੋ(cf->pool, 0);
    ਜੇ (cln == ਨਿਰਾ) {
        ਵਾਪਸ NGX_CONF_ERROR;
    }

    cln->ਅੰਕੜੇ = peer;
    cln->handler = ngx_syslog_cleanup;

    ਵਾਪਸ NGX_CONF_OK;
}


ਸਥਿਰ ਅੱਖਰ *
ngx_syslog_parse_ਦਲੀਲਾਂ(ngx_conf_t *cf, ngx_syslog_peer_t *peer)
{
    u_char      *p, *comma, c;
    ਮਾਪ_t       len;
    ngx_str_t   *ਮੁੱਲ;
    ngx_url_t    u;
    ngx_uint_t   i;

    ਮੁੱਲ = cf->ਦਲੀਲਾਂ->elts;

    p = ਮੁੱਲ[1].ਅੰਕੜੇ + ਆਕਾਰ("syslog:") - 1;

    ਲਈ ( ;; ) {
        comma = (u_char *) ngx_strchr(p, ',');

        ਜੇ (comma != ਨਿਰਾ) {
            len = comma - p;
            *comma = '\0';

        } ਨਹੀਂਤਾਂ {
            len = ਮੁੱਲ[1].ਅੰਕੜੇ + ਮੁੱਲ[1].len - p;
        }

        ਜੇ (ngx_strncmp(p, "server=", 7) == 0) {

            ਜੇ (peer->server.sockaddr != ਨਿਰਾ) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"server\"");
                ਵਾਪਸ NGX_CONF_ERROR;
            }

            ngx_memzero(&u, ਆਕਾਰ(ngx_url_t));

            u.url.ਅੰਕੜੇ = p + 7;
            u.url.len = len - 7;
            u.default_ਪੋਰਟ = 514;

            ਜੇ (ngx_parse_url(cf->pool, &u) != NGX_OK) {
                ਜੇ (u.err) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "%s in syslog server \"%V\"",
                                       u.err, &u.url);
                }

                ਵਾਪਸ NGX_CONF_ERROR;
            }

            peer->server = u.addrs[0];

        } ਨਹੀਂਤਾਂ ਜੇ (ngx_strncmp(p, "facility=", 9) == 0) {

            ਜੇ (peer->facility != NGX_CONF_UNSET_UINT) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"facility\"");
                ਵਾਪਸ NGX_CONF_ERROR;
            }

            ਲਈ (i = 0; facilities[i] != ਨਿਰਾ; i++) {

                ਜੇ (ngx_strcmp(p + 9, facilities[i]) == 0) {
                    peer->facility = i;
                    ਜਾਓ ਅਗਲਾ;
                }
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog facility \"%s\"", p + 9);
            ਵਾਪਸ NGX_CONF_ERROR;

        } ਨਹੀਂਤਾਂ ਜੇ (ngx_strncmp(p, "severity=", 9) == 0) {

            ਜੇ (peer->severity != NGX_CONF_UNSET_UINT) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"severity\"");
                ਵਾਪਸ NGX_CONF_ERROR;
            }

            ਲਈ (i = 0; severities[i] != ਨਿਰਾ; i++) {

                ਜੇ (ngx_strcmp(p + 9, severities[i]) == 0) {
                    peer->severity = i;
                    ਜਾਓ ਅਗਲਾ;
                }
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog severity \"%s\"", p + 9);
            ਵਾਪਸ NGX_CONF_ERROR;

        } ਨਹੀਂਤਾਂ ਜੇ (ngx_strncmp(p, "tag=", 4) == 0) {

            ਜੇ (peer->ਟੈਗ.ਅੰਕੜੇ != ਨਿਰਾ) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"tag\"");
                ਵਾਪਸ NGX_CONF_ERROR;
            }

            /*
             * RFC 3164: the TAG is a string of ABNF alphanumeric characters
             * that MUST NOT exceed 32 characters.
             */
            ਜੇ (len - 4 > 32) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "syslog tag length exceeds 32");
                ਵਾਪਸ NGX_CONF_ERROR;
            }

            ਲਈ (i = 4; i < len; i++) {
                c = ngx_tolower(p[i]);

                ਜੇ (c < '0' || (c > '9' && c < 'a' && c != '_') || c > 'z') {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "syslog \"tag\" only allows "
                                       "alphanumeric characters "
                                       "and underscore");
                    ਵਾਪਸ NGX_CONF_ERROR;
                }
            }

            peer->ਟੈਗ.ਅੰਕੜੇ = p + 4;
            peer->ਟੈਗ.len = len - 4;

        } ਨਹੀਂਤਾਂ ਜੇ (len == 10 && ngx_strncmp(p, "nohostname", 10) == 0) {
            peer->nohostname = 1;

        } ਨਹੀਂਤਾਂ {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog parameter \"%s\"", p);
            ਵਾਪਸ NGX_CONF_ERROR;
        }

    ਅਗਲਾ:

        ਜੇ (comma == ਨਿਰਾ) {
            ਰੋਕੋ;
        }

        p = comma + 1;
    }

    ਵਾਪਸ NGX_CONF_OK;
}


u_char *
ngx_syslog_ਜੋੜੋ_ਸਿਰਲੇਖ(ngx_syslog_peer_t *peer, u_char *buf)
{
    ngx_uint_t  pri;

    pri = peer->facility * 8 + peer->severity;

    ਜੇ (peer->nohostname) {
        ਵਾਪਸ ngx_sprintf(buf, "<%ui>%V %V: ", pri, &ngx_cached_syslog_ਸਮਾਂ,
                           &peer->ਟੈਗ);
    }

    ਵਾਪਸ ngx_sprintf(buf, "<%ui>%V %V %V: ", pri, &ngx_cached_syslog_ਸਮਾਂ,
                       peer->hostname, &peer->ਟੈਗ);
}


ਖਾਲੀ
ngx_syslog_writer(ngx_log_t *ਲਘੂਗਣਕ, ngx_uint_t level, u_char *buf,
    ਮਾਪ_t len)
{
    u_char             *p, ਸੁਨੇਹਾ[NGX_SYSLOG_MAX_STR];
    ngx_uint_t          ਸਿਰ_len;
    ngx_syslog_peer_t  *peer;

    peer = ਲਘੂਗਣਕ->wdata;

    ਜੇ (peer->busy) {
        ਵਾਪਸ;
    }

    peer->busy = 1;
    peer->severity = level - 1;

    p = ngx_syslog_ਜੋੜੋ_ਸਿਰਲੇਖ(peer, ਸੁਨੇਹਾ);
    ਸਿਰ_len = p - ਸੁਨੇਹਾ;

    len -= NGX_LINEFEED_ਮਾਪ;

    ਜੇ (len > NGX_SYSLOG_MAX_STR - ਸਿਰ_len) {
        len = NGX_SYSLOG_MAX_STR - ਸਿਰ_len;
    }

    p = ngx_snprintf(p, len, "%s", buf);

    (ਖਾਲੀ) ngx_syslog_send(peer, ਸੁਨੇਹਾ, p - ਸੁਨੇਹਾ);

    peer->busy = 0;
}


ssize_t
ngx_syslog_send(ngx_syslog_peer_t *peer, u_char *buf, ਮਾਪ_t len)
{
    ssize_t  n;

    ਜੇ (peer->ਲਘੂਗਣਕ.handler == ਨਿਰਾ) {
        peer->ਲਘੂਗਣਕ = *peer->logp;
        peer->ਲਘੂਗਣਕ.handler = ngx_syslog_log_error;
        peer->ਲਘੂਗਣਕ.ਅੰਕੜੇ = peer;
        peer->ਲਘੂਗਣਕ.action = "logging to syslog";
    }

    ਜੇ (peer->conn.fd == (ngx_socket_t) -1) {
        ਜੇ (ngx_syslog_init_peer(peer) != NGX_OK) {
            ਵਾਪਸ NGX_ERROR;
        }
    }

    ਜੇ (ngx_send) {
        n = ngx_send(&peer->conn, buf, len);

    } ਨਹੀਂਤਾਂ {
        /* event module has not yet set ngx_io */
        n = ngx_os_io.send(&peer->conn, buf, len);
    }

    ਜੇ (n == NGX_ERROR) {

        ਜੇ (ngx_close_socket(peer->conn.fd) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &peer->ਲਘੂਗਣਕ, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        peer->conn.fd = (ngx_socket_t) -1;
    }

    ਵਾਪਸ n;
}


ਸਥਿਰ ngx_int_t
ngx_syslog_init_peer(ngx_syslog_peer_t *peer)
{
    ngx_socket_t  fd;

    fd = ngx_socket(peer->server.sockaddr->sa_family, SOCK_DGRAM, 0);
    ਜੇ (fd == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->ਲਘੂਗਣਕ, ngx_socket_errno,
                      ngx_socket_n " failed");
        ਵਾਪਸ NGX_ERROR;
    }

    ਜੇ (ngx_nonblocking(fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->ਲਘੂਗਣਕ, ngx_socket_errno,
                      ngx_nonblocking_n " failed");
        ਜਾਓ failed;
    }

    ਜੇ (connect(fd, peer->server.sockaddr, peer->server.socklen) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->ਲਘੂਗਣਕ, ngx_socket_errno,
                      "connect() failed");
        ਜਾਓ failed;
    }

    peer->conn.fd = fd;
    peer->conn.ਲਘੂਗਣਕ = &peer->ਲਘੂਗਣਕ;

    /* UDP sockets are always ready to write */
    peer->conn.ਲਿਖ->ready = 1;

    ਵਾਪਸ NGX_OK;

failed:

    ਜੇ (ngx_close_socket(fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->ਲਘੂਗਣਕ, ngx_socket_errno,
                      ngx_close_socket_n " failed");
    }

    ਵਾਪਸ NGX_ERROR;
}


ਸਥਿਰ ਖਾਲੀ
ngx_syslog_cleanup(ਖਾਲੀ *ਅੰਕੜੇ)
{
    ngx_syslog_peer_t  *peer = ਅੰਕੜੇ;

    /* prevents further use of this peer */
    peer->busy = 1;

    ਜੇ (peer->conn.fd == (ngx_socket_t) -1) {
        ਵਾਪਸ;
    }

    ਜੇ (ngx_close_socket(peer->conn.fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->ਲਘੂਗਣਕ, ngx_socket_errno,
                      ngx_close_socket_n " failed");
    }
}


ਸਥਿਰ u_char *
ngx_syslog_log_error(ngx_log_t *ਲਘੂਗਣਕ, u_char *buf, ਮਾਪ_t len)
{
    u_char             *p;
    ngx_syslog_peer_t  *peer;

    p = buf;

    ਜੇ (ਲਘੂਗਣਕ->action) {
        p = ngx_snprintf(buf, len, " while %s", ਲਘੂਗਣਕ->action);
        len -= p - buf;
    }

    peer = ਲਘੂਗਣਕ->ਅੰਕੜੇ;

    ਜੇ (peer) {
        p = ngx_snprintf(p, len, ", server: %V", &peer->server.name);
    }

    ਵਾਪਸ p;
}
