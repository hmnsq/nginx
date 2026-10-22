
/*
 * Copyright (C) Nginx, Inc.
 */


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>
#અન્તર્ભાવ <ngx_event.h>


#define NGX_SYSLOG_MAX_STR                                                    \
    NGX_MAX_ERROR_STR + કદ("<255>Jan 01 00:00:00 ") - 1                   \
    + (NGX_MAXHOSTNAMELEN - 1) + 1 /* space */                                \
    + 32 /* tag */ + 2 /* colon, space */


સ્થિર અક્ષર *ngx_syslog_parse_args(ngx_conf_t *cf, ngx_syslog_peer_t *peer);
સ્થિર ngx_int_t ngx_syslog_init_peer(ngx_syslog_peer_t *peer);
સ્થિર શૂન્ય ngx_syslog_cleanup(શૂન્ય *data);
સ્થિર u_char *ngx_syslog_log_error(ngx_log_t *log, u_char *buf, size_t len);


સ્થિર અક્ષર  *facilities[] = {
    "kern", "user", "mail", "daemon", "auth", "intern", "lpr", "news", "uucp",
    "clock", "authpriv", "ftp", "ntp", "audit", "alert", "cron", "local0",
    "local1", "local2", "local3", "local4", "local5", "local6", "local7",
    ખાલી
};

/* note 'error/warn' like in nginx.conf, not 'err/warning' */
સ્થિર અક્ષર  *severities[] = {
    "emerg", "alert", "crit", "error", "warn", "notice", "info", "debug", ખાલી
};

સ્થિર ngx_log_t    ngx_syslog_dummy_log;
સ્થિર ngx_event_t  ngx_syslog_dummy_event;


અક્ષર *
ngx_syslog_process_conf(ngx_conf_t *cf, ngx_syslog_peer_t *peer)
{
    ngx_pool_cleanup_t  *cln;

    peer->facility = NGX_CONF_UNSET_UINT;
    peer->severity = NGX_CONF_UNSET_UINT;

    જો (ngx_syslog_parse_args(cf, peer) != NGX_CONF_OK) {
        પાછા NGX_CONF_ERROR;
    }

    જો (peer->server.sockaddr == ખાલી) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "no syslog server specified");
        પાછા NGX_CONF_ERROR;
    }

    જો (peer->facility == NGX_CONF_UNSET_UINT) {
        peer->facility = 23; /* local7 */
    }

    જો (peer->severity == NGX_CONF_UNSET_UINT) {
        peer->severity = 6; /* info */
    }

    જો (peer->tag.data == ખાલી) {
        ngx_str_set(&peer->tag, "nginx");
    }

    peer->hostname = &cf->cycle->hostname;
    peer->logp = &cf->cycle->new_log;

    peer->conn.fd = (ngx_socket_t) -1;

    peer->conn.પઠ = &ngx_syslog_dummy_event;
    peer->conn.લિખ = &ngx_syslog_dummy_event;

    ngx_syslog_dummy_event.log = &ngx_syslog_dummy_log;

    cln = ngx_pool_cleanup_add(cf->pool, 0);
    જો (cln == ખાલી) {
        પાછા NGX_CONF_ERROR;
    }

    cln->data = peer;
    cln->handler = ngx_syslog_cleanup;

    પાછા NGX_CONF_OK;
}


સ્થિર અક્ષર *
ngx_syslog_parse_args(ngx_conf_t *cf, ngx_syslog_peer_t *peer)
{
    u_char      *p, *comma, c;
    size_t       len;
    ngx_str_t   *કિંમત;
    ngx_url_t    u;
    ngx_uint_t   i;

    કિંમત = cf->args->elts;

    p = કિંમત[1].data + કદ("syslog:") - 1;

    માટે ( ;; ) {
        comma = (u_char *) ngx_strchr(p, ',');

        જો (comma != ખાલી) {
            len = comma - p;
            *comma = '\0';

        } નહીંતર {
            len = કિંમત[1].data + કિંમત[1].len - p;
        }

        જો (ngx_strncmp(p, "server=", 7) == 0) {

            જો (peer->server.sockaddr != ખાલી) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"server\"");
                પાછા NGX_CONF_ERROR;
            }

            ngx_memzero(&u, કદ(ngx_url_t));

            u.url.data = p + 7;
            u.url.len = len - 7;
            u.default_port = 514;

            જો (ngx_parse_url(cf->pool, &u) != NGX_OK) {
                જો (u.err) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "%s in syslog server \"%V\"",
                                       u.err, &u.url);
                }

                પાછા NGX_CONF_ERROR;
            }

            peer->server = u.addrs[0];

        } નહીંતર જો (ngx_strncmp(p, "facility=", 9) == 0) {

            જો (peer->facility != NGX_CONF_UNSET_UINT) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"facility\"");
                પાછા NGX_CONF_ERROR;
            }

            માટે (i = 0; facilities[i] != ખાલી; i++) {

                જો (ngx_strcmp(p + 9, facilities[i]) == 0) {
                    peer->facility = i;
                    જાઓ next;
                }
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog facility \"%s\"", p + 9);
            પાછા NGX_CONF_ERROR;

        } નહીંતર જો (ngx_strncmp(p, "severity=", 9) == 0) {

            જો (peer->severity != NGX_CONF_UNSET_UINT) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"severity\"");
                પાછા NGX_CONF_ERROR;
            }

            માટે (i = 0; severities[i] != ખાલી; i++) {

                જો (ngx_strcmp(p + 9, severities[i]) == 0) {
                    peer->severity = i;
                    જાઓ next;
                }
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog severity \"%s\"", p + 9);
            પાછા NGX_CONF_ERROR;

        } નહીંતર જો (ngx_strncmp(p, "tag=", 4) == 0) {

            જો (peer->tag.data != ખાલી) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"tag\"");
                પાછા NGX_CONF_ERROR;
            }

            /*
             * RFC 3164: the TAG is a string of ABNF alphanumeric characters
             * that MUST NOT exceed 32 characters.
             */
            જો (len - 4 > 32) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "syslog tag length exceeds 32");
                પાછા NGX_CONF_ERROR;
            }

            માટે (i = 4; i < len; i++) {
                c = ngx_tolower(p[i]);

                જો (c < '0' || (c > '9' && c < 'a' && c != '_') || c > 'z') {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "syslog \"tag\" only allows "
                                       "alphanumeric characters "
                                       "and underscore");
                    પાછા NGX_CONF_ERROR;
                }
            }

            peer->tag.data = p + 4;
            peer->tag.len = len - 4;

        } નહીંતર જો (len == 10 && ngx_strncmp(p, "nohostname", 10) == 0) {
            peer->nohostname = 1;

        } નહીંતર {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog parameter \"%s\"", p);
            પાછા NGX_CONF_ERROR;
        }

    next:

        જો (comma == ખાલી) {
            તોડો;
        }

        p = comma + 1;
    }

    પાછા NGX_CONF_OK;
}


u_char *
ngx_syslog_add_header(ngx_syslog_peer_t *peer, u_char *buf)
{
    ngx_uint_t  pri;

    pri = peer->facility * 8 + peer->severity;

    જો (peer->nohostname) {
        પાછા ngx_sprintf(buf, "<%ui>%V %V: ", pri, &ngx_cached_syslog_time,
                           &peer->tag);
    }

    પાછા ngx_sprintf(buf, "<%ui>%V %V %V: ", pri, &ngx_cached_syslog_time,
                       peer->hostname, &peer->tag);
}


શૂન્ય
ngx_syslog_writer(ngx_log_t *log, ngx_uint_t level, u_char *buf,
    size_t len)
{
    u_char             *p, સંદેશ[NGX_SYSLOG_MAX_STR];
    ngx_uint_t          head_len;
    ngx_syslog_peer_t  *peer;

    peer = log->wdata;

    જો (peer->busy) {
        પાછા;
    }

    peer->busy = 1;
    peer->severity = level - 1;

    p = ngx_syslog_add_header(peer, સંદેશ);
    head_len = p - સંદેશ;

    len -= NGX_LINEFEED_SIZE;

    જો (len > NGX_SYSLOG_MAX_STR - head_len) {
        len = NGX_SYSLOG_MAX_STR - head_len;
    }

    p = ngx_snprintf(p, len, "%s", buf);

    (શૂન્ય) ngx_syslog_send(peer, સંદેશ, p - સંદેશ);

    peer->busy = 0;
}


ssize_t
ngx_syslog_send(ngx_syslog_peer_t *peer, u_char *buf, size_t len)
{
    ssize_t  n;

    જો (peer->log.handler == ખાલી) {
        peer->log = *peer->logp;
        peer->log.handler = ngx_syslog_log_error;
        peer->log.data = peer;
        peer->log.action = "logging to syslog";
    }

    જો (peer->conn.fd == (ngx_socket_t) -1) {
        જો (ngx_syslog_init_peer(peer) != NGX_OK) {
            પાછા NGX_ERROR;
        }
    }

    જો (ngx_send) {
        n = ngx_send(&peer->conn, buf, len);

    } નહીંતર {
        /* event module has not yet set ngx_io */
        n = ngx_os_io.send(&peer->conn, buf, len);
    }

    જો (n == NGX_ERROR) {

        જો (ngx_close_socket(peer->conn.fd) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        peer->conn.fd = (ngx_socket_t) -1;
    }

    પાછા n;
}


સ્થિર ngx_int_t
ngx_syslog_init_peer(ngx_syslog_peer_t *peer)
{
    ngx_socket_t  fd;

    fd = ngx_socket(peer->server.sockaddr->sa_family, SOCK_DGRAM, 0);
    જો (fd == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_socket_n " failed");
        પાછા NGX_ERROR;
    }

    જો (ngx_nonblocking(fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_nonblocking_n " failed");
        જાઓ failed;
    }

    જો (connect(fd, peer->server.sockaddr, peer->server.socklen) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      "connect() failed");
        જાઓ failed;
    }

    peer->conn.fd = fd;
    peer->conn.log = &peer->log;

    /* UDP sockets are always ready to write */
    peer->conn.લિખ->ready = 1;

    પાછા NGX_OK;

failed:

    જો (ngx_close_socket(fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_close_socket_n " failed");
    }

    પાછા NGX_ERROR;
}


સ્થિર શૂન્ય
ngx_syslog_cleanup(શૂન્ય *data)
{
    ngx_syslog_peer_t  *peer = data;

    /* prevents further use of this peer */
    peer->busy = 1;

    જો (peer->conn.fd == (ngx_socket_t) -1) {
        પાછા;
    }

    જો (ngx_close_socket(peer->conn.fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_close_socket_n " failed");
    }
}


સ્થિર u_char *
ngx_syslog_log_error(ngx_log_t *log, u_char *buf, size_t len)
{
    u_char             *p;
    ngx_syslog_peer_t  *peer;

    p = buf;

    જો (log->action) {
        p = ngx_snprintf(buf, len, " while %s", log->action);
        len -= p - buf;
    }

    peer = log->data;

    જો (peer) {
        p = ngx_snprintf(p, len, ", server: %V", &peer->server.name);
    }

    પાછા p;
}
