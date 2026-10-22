
/*
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>
#ಅನ್ತರ್ಭಾವ <ngx_event.h>


#ವ್ಯಾಖ್ಯಾನಿಸು NGX_SYSLOG_MAX_STR                                                    \
    NGX_MAX_ERROR_STR + ಗಾತ್ರ("<255>Jan 01 00:00:00 ") - 1                   \
    + (NGX_MAXHOSTNAMELEN - 1) + 1 /* space */                                \
    + 32 /*ಟ್ಯಾಗ್*/ + 2 /* colon, space */


ಸ್ಥಿರ ಅಕ್ಷರ *ngx_syslog_parse_ವಾದಗಳು(ngx_conf_t *cf, ngx_syslog_peer_t *peer);
ಸ್ಥಿರ ngx_int_t ngx_syslog_init_peer(ngx_syslog_peer_t *peer);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_syslog_cleanup(ಶೂನ್ಯ *ದತ್ತಾಂಶ);
ಸ್ಥಿರ u_char *ngx_syslog_log_error(ngx_log_t *ಲಘುಗಣಕ, u_char *buf, ಗಾತ್ರ_t len);


ಸ್ಥಿರ ಅಕ್ಷರ  *facilities[] = {
    "kern", "user", "mail", "daemon", "auth", "intern", "lpr", "news", "uucp",
    "clock", "authpriv", "ftp", "ntp", "audit", "alert", "cron", "local0",
    "local1", "local2", "local3", "local4", "local5", "local6", "local7",
    ಖಾಲಿ
};

/* note 'error/warn' like in nginx.conf, not 'err/warning' */
ಸ್ಥಿರ ಅಕ್ಷರ  *severities[] = {
    "emerg", "alert", "crit", "error", "warn", "notice", "info", "debug", ಖಾಲಿ
};

ಸ್ಥಿರ ngx_log_t    ngx_syslog_dummy_log;
ಸ್ಥಿರ ngx_event_t  ngx_syslog_dummy_event;


ಅಕ್ಷರ *
ngx_syslog_process_conf(ngx_conf_t *cf, ngx_syslog_peer_t *peer)
{
    ngx_pool_cleanup_t  *cln;

    peer->facility = NGX_CONF_UNSET_UINT;
    peer->severity = NGX_CONF_UNSET_UINT;

    ಆದರೆ (ngx_syslog_parse_ವಾದಗಳು(cf, peer) != NGX_CONF_OK) {
        ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
    }

    ಆದರೆ (peer->server.sockaddr == ಖಾಲಿ) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "no syslog server specified");
        ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
    }

    ಆದರೆ (peer->facility == NGX_CONF_UNSET_UINT) {
        peer->facility = 23; /* local7 */
    }

    ಆದರೆ (peer->severity == NGX_CONF_UNSET_UINT) {
        peer->severity = 6; /* info */
    }

    ಆದರೆ (peer->ಟ್ಯಾಗ್.ದತ್ತಾಂಶ == ಖಾಲಿ) {
        ngx_str_ಗಣ(&peer->ಟ್ಯಾಗ್, "nginx");
    }

    peer->hostname = &cf->ಚಕ್ರ->hostname;
    peer->logp = &cf->ಚಕ್ರ->new_log;

    peer->conn.fd = (ngx_socket_t) -1;

    peer->conn.ಪಠ = &ngx_syslog_dummy_event;
    peer->conn.ಲಿಖ = &ngx_syslog_dummy_event;

    ngx_syslog_dummy_event.ಲಘುಗಣಕ = &ngx_syslog_dummy_log;

    cln = ngx_pool_cleanup_ಕೂಡಿಸು(cf->pool, 0);
    ಆದರೆ (cln == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
    }

    cln->ದತ್ತಾಂಶ = peer;
    cln->handler = ngx_syslog_cleanup;

    ಹಿಂತಿರುಗಿ NGX_CONF_OK;
}


ಸ್ಥಿರ ಅಕ್ಷರ *
ngx_syslog_parse_ವಾದಗಳು(ngx_conf_t *cf, ngx_syslog_peer_t *peer)
{
    u_char      *p, *comma, c;
    ಗಾತ್ರ_t       len;
    ngx_str_t   *ಮೌಲ್ಯ;
    ngx_url_t    u;
    ngx_uint_t   i;

    ಮೌಲ್ಯ = cf->ವಾದಗಳು->elts;

    p = ಮೌಲ್ಯ[1].ದತ್ತಾಂಶ + ಗಾತ್ರ("syslog:") - 1;

    ಗಾಗಿ ( ;; ) {
        comma = (u_char *) ngx_strchr(p, ',');

        ಆದರೆ (comma != ಖಾಲಿ) {
            len = comma - p;
            *comma = '\0';

        } ಇಲ್ಲದಿದ್ದರೆ {
            len = ಮೌಲ್ಯ[1].ದತ್ತಾಂಶ + ಮೌಲ್ಯ[1].len - p;
        }

        ಆದರೆ (ngx_strncmp(p, "server=", 7) == 0) {

            ಆದರೆ (peer->server.sockaddr != ಖಾಲಿ) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"server\"");
                ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
            }

            ngx_memzero(&u, ಗಾತ್ರ(ngx_url_t));

            u.url.ದತ್ತಾಂಶ = p + 7;
            u.url.len = len - 7;
            u.default_ಪೋರ್ಟ್ = 514;

            ಆದರೆ (ngx_parse_url(cf->pool, &u) != NGX_OK) {
                ಆದರೆ (u.err) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "%s in syslog server \"%V\"",
                                       u.err, &u.url);
                }

                ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
            }

            peer->server = u.addrs[0];

        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ngx_strncmp(p, "facility=", 9) == 0) {

            ಆದರೆ (peer->facility != NGX_CONF_UNSET_UINT) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"facility\"");
                ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
            }

            ಗಾಗಿ (i = 0; facilities[i] != ಖಾಲಿ; i++) {

                ಆದರೆ (ngx_strcmp(p + 9, facilities[i]) == 0) {
                    peer->facility = i;
                    ಹೋಗು ಮುಂದಿನ;
                }
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog facility \"%s\"", p + 9);
            ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;

        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ngx_strncmp(p, "severity=", 9) == 0) {

            ಆದರೆ (peer->severity != NGX_CONF_UNSET_UINT) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"severity\"");
                ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
            }

            ಗಾಗಿ (i = 0; severities[i] != ಖಾಲಿ; i++) {

                ಆದರೆ (ngx_strcmp(p + 9, severities[i]) == 0) {
                    peer->severity = i;
                    ಹೋಗು ಮುಂದಿನ;
                }
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog severity \"%s\"", p + 9);
            ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;

        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ngx_strncmp(p, "tag=", 4) == 0) {

            ಆದರೆ (peer->ಟ್ಯಾಗ್.ದತ್ತಾಂಶ != ಖಾಲಿ) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"tag\"");
                ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
            }

            /*
             * RFC 3164: the TAG is a string of ABNF alphanumeric characters
             * that MUST NOT exceed 32 characters.
             */
            ಆದರೆ (len - 4 > 32) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "syslog tag length exceeds 32");
                ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
            }

            ಗಾಗಿ (i = 4; i < len; i++) {
                c = ngx_tolower(p[i]);

                ಆದರೆ (c < '0' || (c > '9' && c < 'a' && c != '_') || c > 'z') {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "syslog \"tag\" only allows "
                                       "alphanumeric characters "
                                       "and underscore");
                    ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
                }
            }

            peer->ಟ್ಯಾಗ್.ದತ್ತಾಂಶ = p + 4;
            peer->ಟ್ಯಾಗ್.len = len - 4;

        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (len == 10 && ngx_strncmp(p, "nohostname", 10) == 0) {
            peer->nohostname = 1;

        } ಇಲ್ಲದಿದ್ದರೆ {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog parameter \"%s\"", p);
            ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
        }

    ಮುಂದಿನ:

        ಆದರೆ (comma == ಖಾಲಿ) {
            ನಿಲ್ಲಿಸು;
        }

        p = comma + 1;
    }

    ಹಿಂತಿರುಗಿ NGX_CONF_OK;
}


u_char *
ngx_syslog_ಕೂಡಿಸು_ಶೀರ್ಷಿಕೆ(ngx_syslog_peer_t *peer, u_char *buf)
{
    ngx_uint_t  pri;

    pri = peer->facility * 8 + peer->severity;

    ಆದರೆ (peer->nohostname) {
        ಹಿಂತಿರುಗಿ ngx_sprintf(buf, "<%ui>%V %V: ", pri, &ngx_cached_syslog_ಸಮಯ,
                           &peer->ಟ್ಯಾಗ್);
    }

    ಹಿಂತಿರುಗಿ ngx_sprintf(buf, "<%ui>%V %V %V: ", pri, &ngx_cached_syslog_ಸಮಯ,
                       peer->hostname, &peer->ಟ್ಯಾಗ್);
}


ಶೂನ್ಯ
ngx_syslog_writer(ngx_log_t *ಲಘುಗಣಕ, ngx_uint_t level, u_char *buf,
    ಗಾತ್ರ_t len)
{
    u_char             *p, ಸಂದೇಶ[NGX_SYSLOG_MAX_STR];
    ngx_uint_t          ತಲೆ_len;
    ngx_syslog_peer_t  *peer;

    peer = ಲಘುಗಣಕ->wdata;

    ಆದರೆ (peer->busy) {
        ಹಿಂತಿರುಗಿ;
    }

    peer->busy = 1;
    peer->severity = level - 1;

    p = ngx_syslog_ಕೂಡಿಸು_ಶೀರ್ಷಿಕೆ(peer, ಸಂದೇಶ);
    ತಲೆ_len = p - ಸಂದೇಶ;

    len -= NGX_LINEFEED_ಗಾತ್ರ;

    ಆದರೆ (len > NGX_SYSLOG_MAX_STR - ತಲೆ_len) {
        len = NGX_SYSLOG_MAX_STR - ತಲೆ_len;
    }

    p = ngx_snprintf(p, len, "%s", buf);

    (ಶೂನ್ಯ) ngx_syslog_send(peer, ಸಂದೇಶ, p - ಸಂದೇಶ);

    peer->busy = 0;
}


ssize_t
ngx_syslog_send(ngx_syslog_peer_t *peer, u_char *buf, ಗಾತ್ರ_t len)
{
    ssize_t  n;

    ಆದರೆ (peer->ಲಘುಗಣಕ.handler == ಖಾಲಿ) {
        peer->ಲಘುಗಣಕ = *peer->logp;
        peer->ಲಘುಗಣಕ.handler = ngx_syslog_log_error;
        peer->ಲಘುಗಣಕ.ದತ್ತಾಂಶ = peer;
        peer->ಲಘುಗಣಕ.action = "logging to syslog";
    }

    ಆದರೆ (peer->conn.fd == (ngx_socket_t) -1) {
        ಆದರೆ (ngx_syslog_init_peer(peer) != NGX_OK) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }
    }

    ಆದರೆ (ngx_send) {
        n = ngx_send(&peer->conn, buf, len);

    } ಇಲ್ಲದಿದ್ದರೆ {
        /* event module has not yet set ngx_io */
        n = ngx_os_io.send(&peer->conn, buf, len);
    }

    ಆದರೆ (n == NGX_ERROR) {

        ಆದರೆ (ngx_close_socket(peer->conn.fd) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &peer->ಲಘುಗಣಕ, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        peer->conn.fd = (ngx_socket_t) -1;
    }

    ಹಿಂತಿರುಗಿ n;
}


ಸ್ಥಿರ ngx_int_t
ngx_syslog_init_peer(ngx_syslog_peer_t *peer)
{
    ngx_socket_t  fd;

    fd = ngx_socket(peer->server.sockaddr->sa_family, SOCK_DGRAM, 0);
    ಆದರೆ (fd == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->ಲಘುಗಣಕ, ngx_socket_errno,
                      ngx_socket_n " failed");
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಆದರೆ (ngx_nonblocking(fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->ಲಘುಗಣಕ, ngx_socket_errno,
                      ngx_nonblocking_n " failed");
        ಹೋಗು failed;
    }

    ಆದರೆ (connect(fd, peer->server.sockaddr, peer->server.socklen) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->ಲಘುಗಣಕ, ngx_socket_errno,
                      "connect() failed");
        ಹೋಗು failed;
    }

    peer->conn.fd = fd;
    peer->conn.ಲಘುಗಣಕ = &peer->ಲಘುಗಣಕ;

    /* UDP sockets are always ready to write */
    peer->conn.ಲಿಖ->ready = 1;

    ಹಿಂತಿರುಗಿ NGX_OK;

failed:

    ಆದರೆ (ngx_close_socket(fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->ಲಘುಗಣಕ, ngx_socket_errno,
                      ngx_close_socket_n " failed");
    }

    ಹಿಂತಿರುಗಿ NGX_ERROR;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_syslog_cleanup(ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
    ngx_syslog_peer_t  *peer = ದತ್ತಾಂಶ;

    /* prevents further use of this peer */
    peer->busy = 1;

    ಆದರೆ (peer->conn.fd == (ngx_socket_t) -1) {
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ (ngx_close_socket(peer->conn.fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->ಲಘುಗಣಕ, ngx_socket_errno,
                      ngx_close_socket_n " failed");
    }
}


ಸ್ಥಿರ u_char *
ngx_syslog_log_error(ngx_log_t *ಲಘುಗಣಕ, u_char *buf, ಗಾತ್ರ_t len)
{
    u_char             *p;
    ngx_syslog_peer_t  *peer;

    p = buf;

    ಆದರೆ (ಲಘುಗಣಕ->action) {
        p = ngx_snprintf(buf, len, " while %s", ಲಘುಗಣಕ->action);
        len -= p - buf;
    }

    peer = ಲಘುಗಣಕ->ದತ್ತಾಂಶ;

    ಆದರೆ (peer) {
        p = ngx_snprintf(p, len, ", server: %V", &peer->server.name);
    }

    ಹಿಂತಿರುಗಿ p;
}
