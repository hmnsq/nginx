
/*
 * Copyright (C) Nginx, Inc.
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>
#ଅନ୍ତର୍ଭାଵ <ngx_event.h>


#define NGX_SYSLOG_MAX_STR                                                    \
    NGX_MAX_ERROR_STR + ଆକାର("<255>Jan 01 00:00:00 ") - 1                   \
    + (NGX_MAXHOSTNAMELEN - 1) + 1 /* space */                                \
    + 32 /* tag */ + 2 /* colon, space */


ସ୍ଥିର ଅକ୍ଷର *ngx_syslog_parse_args(ngx_conf_t *cf, ngx_syslog_peer_t *peer);
ସ୍ଥିର ngx_int_t ngx_syslog_init_peer(ngx_syslog_peer_t *peer);
ସ୍ଥିର ଶୂନ୍ୟ ngx_syslog_cleanup(ଶୂନ୍ୟ *data);
ସ୍ଥିର u_char *ngx_syslog_log_error(ngx_log_t *log, u_char *buf, size_t len);


ସ୍ଥିର ଅକ୍ଷର  *facilities[] = {
    "kern", "user", "mail", "daemon", "auth", "intern", "lpr", "news", "uucp",
    "clock", "authpriv", "ftp", "ntp", "audit", "alert", "cron", "local0",
    "local1", "local2", "local3", "local4", "local5", "local6", "local7",
    ଖାଲି
};

/* note 'error/warn' like in nginx.conf, not 'err/warning' */
ସ୍ଥିର ଅକ୍ଷର  *severities[] = {
    "emerg", "alert", "crit", "error", "warn", "notice", "info", "debug", ଖାଲି
};

ସ୍ଥିର ngx_log_t    ngx_syslog_dummy_log;
ସ୍ଥିର ngx_event_t  ngx_syslog_dummy_event;


ଅକ୍ଷର *
ngx_syslog_process_conf(ngx_conf_t *cf, ngx_syslog_peer_t *peer)
{
    ngx_pool_cleanup_t  *cln;

    peer->facility = NGX_CONF_UNSET_UINT;
    peer->severity = NGX_CONF_UNSET_UINT;

    ଯଦି (ngx_syslog_parse_args(cf, peer) != NGX_CONF_OK) {
        ଫେରନ୍ତୁ NGX_CONF_ERROR;
    }

    ଯଦି (peer->server.sockaddr == ଖାଲି) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "no syslog server specified");
        ଫେରନ୍ତୁ NGX_CONF_ERROR;
    }

    ଯଦି (peer->facility == NGX_CONF_UNSET_UINT) {
        peer->facility = 23; /* local7 */
    }

    ଯଦି (peer->severity == NGX_CONF_UNSET_UINT) {
        peer->severity = 6; /* info */
    }

    ଯଦି (peer->tag.data == ଖାଲି) {
        ngx_str_set(&peer->tag, "nginx");
    }

    peer->hostname = &cf->cycle->hostname;
    peer->logp = &cf->cycle->new_log;

    peer->conn.fd = (ngx_socket_t) -1;

    peer->conn.ପଠ = &ngx_syslog_dummy_event;
    peer->conn.ଲିଖ = &ngx_syslog_dummy_event;

    ngx_syslog_dummy_event.log = &ngx_syslog_dummy_log;

    cln = ngx_pool_cleanup_add(cf->pool, 0);
    ଯଦି (cln == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_CONF_ERROR;
    }

    cln->data = peer;
    cln->handler = ngx_syslog_cleanup;

    ଫେରନ୍ତୁ NGX_CONF_OK;
}


ସ୍ଥିର ଅକ୍ଷର *
ngx_syslog_parse_args(ngx_conf_t *cf, ngx_syslog_peer_t *peer)
{
    u_char      *p, *comma, c;
    size_t       len;
    ngx_str_t   *ମୂଲ୍ୟ;
    ngx_url_t    u;
    ngx_uint_t   i;

    ମୂଲ୍ୟ = cf->args->elts;

    p = ମୂଲ୍ୟ[1].data + ଆକାର("syslog:") - 1;

    ପାଇଁ ( ;; ) {
        comma = (u_char *) ngx_strchr(p, ',');

        ଯଦି (comma != ଖାଲି) {
            len = comma - p;
            *comma = '\0';

        } ଅନ୍ୟଥା {
            len = ମୂଲ୍ୟ[1].data + ମୂଲ୍ୟ[1].len - p;
        }

        ଯଦି (ngx_strncmp(p, "server=", 7) == 0) {

            ଯଦି (peer->server.sockaddr != ଖାଲି) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"server\"");
                ଫେରନ୍ତୁ NGX_CONF_ERROR;
            }

            ngx_memzero(&u, ଆକାର(ngx_url_t));

            u.url.data = p + 7;
            u.url.len = len - 7;
            u.default_port = 514;

            ଯଦି (ngx_parse_url(cf->pool, &u) != NGX_OK) {
                ଯଦି (u.err) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "%s in syslog server \"%V\"",
                                       u.err, &u.url);
                }

                ଫେରନ୍ତୁ NGX_CONF_ERROR;
            }

            peer->server = u.addrs[0];

        } ଅନ୍ୟଥା ଯଦି (ngx_strncmp(p, "facility=", 9) == 0) {

            ଯଦି (peer->facility != NGX_CONF_UNSET_UINT) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"facility\"");
                ଫେରନ୍ତୁ NGX_CONF_ERROR;
            }

            ପାଇଁ (i = 0; facilities[i] != ଖାଲି; i++) {

                ଯଦି (ngx_strcmp(p + 9, facilities[i]) == 0) {
                    peer->facility = i;
                    ଯାଅ next;
                }
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog facility \"%s\"", p + 9);
            ଫେରନ୍ତୁ NGX_CONF_ERROR;

        } ଅନ୍ୟଥା ଯଦି (ngx_strncmp(p, "severity=", 9) == 0) {

            ଯଦି (peer->severity != NGX_CONF_UNSET_UINT) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"severity\"");
                ଫେରନ୍ତୁ NGX_CONF_ERROR;
            }

            ପାଇଁ (i = 0; severities[i] != ଖାଲି; i++) {

                ଯଦି (ngx_strcmp(p + 9, severities[i]) == 0) {
                    peer->severity = i;
                    ଯାଅ next;
                }
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog severity \"%s\"", p + 9);
            ଫେରନ୍ତୁ NGX_CONF_ERROR;

        } ଅନ୍ୟଥା ଯଦି (ngx_strncmp(p, "tag=", 4) == 0) {

            ଯଦି (peer->tag.data != ଖାଲି) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"tag\"");
                ଫେରନ୍ତୁ NGX_CONF_ERROR;
            }

            /*
             * RFC 3164: the TAG is a string of ABNF alphanumeric characters
             * that MUST NOT exceed 32 characters.
             */
            ଯଦି (len - 4 > 32) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "syslog tag length exceeds 32");
                ଫେରନ୍ତୁ NGX_CONF_ERROR;
            }

            ପାଇଁ (i = 4; i < len; i++) {
                c = ngx_tolower(p[i]);

                ଯଦି (c < '0' || (c > '9' && c < 'a' && c != '_') || c > 'z') {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "syslog \"tag\" only allows "
                                       "alphanumeric characters "
                                       "and underscore");
                    ଫେରନ୍ତୁ NGX_CONF_ERROR;
                }
            }

            peer->tag.data = p + 4;
            peer->tag.len = len - 4;

        } ଅନ୍ୟଥା ଯଦି (len == 10 && ngx_strncmp(p, "nohostname", 10) == 0) {
            peer->nohostname = 1;

        } ଅନ୍ୟଥା {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog parameter \"%s\"", p);
            ଫେରନ୍ତୁ NGX_CONF_ERROR;
        }

    next:

        ଯଦି (comma == ଖାଲି) {
            ଭାଙ୍ଗ;
        }

        p = comma + 1;
    }

    ଫେରନ୍ତୁ NGX_CONF_OK;
}


u_char *
ngx_syslog_add_header(ngx_syslog_peer_t *peer, u_char *buf)
{
    ngx_uint_t  pri;

    pri = peer->facility * 8 + peer->severity;

    ଯଦି (peer->nohostname) {
        ଫେରନ୍ତୁ ngx_sprintf(buf, "<%ui>%V %V: ", pri, &ngx_cached_syslog_time,
                           &peer->tag);
    }

    ଫେରନ୍ତୁ ngx_sprintf(buf, "<%ui>%V %V %V: ", pri, &ngx_cached_syslog_time,
                       peer->hostname, &peer->tag);
}


ଶୂନ୍ୟ
ngx_syslog_writer(ngx_log_t *log, ngx_uint_t level, u_char *buf,
    size_t len)
{
    u_char             *p, ସନ୍ଦେଶ[NGX_SYSLOG_MAX_STR];
    ngx_uint_t          head_len;
    ngx_syslog_peer_t  *peer;

    peer = log->wdata;

    ଯଦି (peer->busy) {
        ଫେରନ୍ତୁ;
    }

    peer->busy = 1;
    peer->severity = level - 1;

    p = ngx_syslog_add_header(peer, ସନ୍ଦେଶ);
    head_len = p - ସନ୍ଦେଶ;

    len -= NGX_LINEFEED_SIZE;

    ଯଦି (len > NGX_SYSLOG_MAX_STR - head_len) {
        len = NGX_SYSLOG_MAX_STR - head_len;
    }

    p = ngx_snprintf(p, len, "%s", buf);

    (ଶୂନ୍ୟ) ngx_syslog_send(peer, ସନ୍ଦେଶ, p - ସନ୍ଦେଶ);

    peer->busy = 0;
}


ssize_t
ngx_syslog_send(ngx_syslog_peer_t *peer, u_char *buf, size_t len)
{
    ssize_t  n;

    ଯଦି (peer->log.handler == ଖାଲି) {
        peer->log = *peer->logp;
        peer->log.handler = ngx_syslog_log_error;
        peer->log.data = peer;
        peer->log.action = "logging to syslog";
    }

    ଯଦି (peer->conn.fd == (ngx_socket_t) -1) {
        ଯଦି (ngx_syslog_init_peer(peer) != NGX_OK) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }
    }

    ଯଦି (ngx_send) {
        n = ngx_send(&peer->conn, buf, len);

    } ଅନ୍ୟଥା {
        /* event module has not yet set ngx_io */
        n = ngx_os_io.send(&peer->conn, buf, len);
    }

    ଯଦି (n == NGX_ERROR) {

        ଯଦି (ngx_close_socket(peer->conn.fd) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        peer->conn.fd = (ngx_socket_t) -1;
    }

    ଫେରନ୍ତୁ n;
}


ସ୍ଥିର ngx_int_t
ngx_syslog_init_peer(ngx_syslog_peer_t *peer)
{
    ngx_socket_t  fd;

    fd = ngx_socket(peer->server.sockaddr->sa_family, SOCK_DGRAM, 0);
    ଯଦି (fd == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_socket_n " failed");
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ଯଦି (ngx_nonblocking(fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_nonblocking_n " failed");
        ଯାଅ failed;
    }

    ଯଦି (connect(fd, peer->server.sockaddr, peer->server.socklen) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      "connect() failed");
        ଯାଅ failed;
    }

    peer->conn.fd = fd;
    peer->conn.log = &peer->log;

    /* UDP sockets are always ready to write */
    peer->conn.ଲିଖ->ready = 1;

    ଫେରନ୍ତୁ NGX_OK;

failed:

    ଯଦି (ngx_close_socket(fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_close_socket_n " failed");
    }

    ଫେରନ୍ତୁ NGX_ERROR;
}


ସ୍ଥିର ଶୂନ୍ୟ
ngx_syslog_cleanup(ଶୂନ୍ୟ *data)
{
    ngx_syslog_peer_t  *peer = data;

    /* prevents further use of this peer */
    peer->busy = 1;

    ଯଦି (peer->conn.fd == (ngx_socket_t) -1) {
        ଫେରନ୍ତୁ;
    }

    ଯଦି (ngx_close_socket(peer->conn.fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_close_socket_n " failed");
    }
}


ସ୍ଥିର u_char *
ngx_syslog_log_error(ngx_log_t *log, u_char *buf, size_t len)
{
    u_char             *p;
    ngx_syslog_peer_t  *peer;

    p = buf;

    ଯଦି (log->action) {
        p = ngx_snprintf(buf, len, " while %s", log->action);
        len -= p - buf;
    }

    peer = log->data;

    ଯଦି (peer) {
        p = ngx_snprintf(p, len, ", server: %V", &peer->server.name);
    }

    ଫେରନ୍ତୁ p;
}
