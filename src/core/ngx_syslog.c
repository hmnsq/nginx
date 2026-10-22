
/*
 * Copyright (C) Nginx, Inc.
 */


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>
#அந்தர்பாவ <ngx_event.h>


#define NGX_SYSLOG_MAX_STR                                                    \
    NGX_MAX_ERROR_STR + அளவு("<255>Jan 01 00:00:00 ") - 1                   \
    + (NGX_MAXHOSTNAMELEN - 1) + 1 /* space */                                \
    + 32 /* tag */ + 2 /* colon, space */


நிலையான எழுத்து *ngx_syslog_parse_args(ngx_conf_t *cf, ngx_syslog_peer_t *peer);
நிலையான ngx_int_t ngx_syslog_init_peer(ngx_syslog_peer_t *peer);
நிலையான வெறுமை ngx_syslog_cleanup(வெறுமை *data);
நிலையான u_char *ngx_syslog_log_error(ngx_log_t *log, u_char *buf, size_t len);


நிலையான எழுத்து  *facilities[] = {
    "kern", "user", "mail", "daemon", "auth", "intern", "lpr", "news", "uucp",
    "clock", "authpriv", "ftp", "ntp", "audit", "alert", "cron", "local0",
    "local1", "local2", "local3", "local4", "local5", "local6", "local7",
    வெற்று
};

/* note 'error/warn' like in nginx.conf, not 'err/warning' */
நிலையான எழுத்து  *severities[] = {
    "emerg", "alert", "crit", "error", "warn", "notice", "info", "debug", வெற்று
};

நிலையான ngx_log_t    ngx_syslog_dummy_log;
நிலையான ngx_event_t  ngx_syslog_dummy_event;


எழுத்து *
ngx_syslog_process_conf(ngx_conf_t *cf, ngx_syslog_peer_t *peer)
{
    ngx_pool_cleanup_t  *cln;

    peer->facility = NGX_CONF_UNSET_UINT;
    peer->severity = NGX_CONF_UNSET_UINT;

    எனில் (ngx_syslog_parse_args(cf, peer) != NGX_CONF_OK) {
        திரும்பு NGX_CONF_ERROR;
    }

    எனில் (peer->server.sockaddr == வெற்று) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "no syslog server specified");
        திரும்பு NGX_CONF_ERROR;
    }

    எனில் (peer->facility == NGX_CONF_UNSET_UINT) {
        peer->facility = 23; /* local7 */
    }

    எனில் (peer->severity == NGX_CONF_UNSET_UINT) {
        peer->severity = 6; /* info */
    }

    எனில் (peer->tag.data == வெற்று) {
        ngx_str_set(&peer->tag, "nginx");
    }

    peer->hostname = &cf->cycle->hostname;
    peer->logp = &cf->cycle->new_log;

    peer->conn.fd = (ngx_socket_t) -1;

    peer->conn.பட = &ngx_syslog_dummy_event;
    peer->conn.லிக = &ngx_syslog_dummy_event;

    ngx_syslog_dummy_event.log = &ngx_syslog_dummy_log;

    cln = ngx_pool_cleanup_add(cf->pool, 0);
    எனில் (cln == வெற்று) {
        திரும்பு NGX_CONF_ERROR;
    }

    cln->data = peer;
    cln->handler = ngx_syslog_cleanup;

    திரும்பு NGX_CONF_OK;
}


நிலையான எழுத்து *
ngx_syslog_parse_args(ngx_conf_t *cf, ngx_syslog_peer_t *peer)
{
    u_char      *p, *comma, c;
    size_t       len;
    ngx_str_t   *மதிப்பு;
    ngx_url_t    u;
    ngx_uint_t   i;

    மதிப்பு = cf->args->elts;

    p = மதிப்பு[1].data + அளவு("syslog:") - 1;

    க்கு ( ;; ) {
        comma = (u_char *) ngx_strchr(p, ',');

        எனில் (comma != வெற்று) {
            len = comma - p;
            *comma = '\0';

        } இல்லையெனில் {
            len = மதிப்பு[1].data + மதிப்பு[1].len - p;
        }

        எனில் (ngx_strncmp(p, "server=", 7) == 0) {

            எனில் (peer->server.sockaddr != வெற்று) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"server\"");
                திரும்பு NGX_CONF_ERROR;
            }

            ngx_memzero(&u, அளவு(ngx_url_t));

            u.url.data = p + 7;
            u.url.len = len - 7;
            u.default_port = 514;

            எனில் (ngx_parse_url(cf->pool, &u) != NGX_OK) {
                எனில் (u.err) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "%s in syslog server \"%V\"",
                                       u.err, &u.url);
                }

                திரும்பு NGX_CONF_ERROR;
            }

            peer->server = u.addrs[0];

        } இல்லையெனில் எனில் (ngx_strncmp(p, "facility=", 9) == 0) {

            எனில் (peer->facility != NGX_CONF_UNSET_UINT) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"facility\"");
                திரும்பு NGX_CONF_ERROR;
            }

            க்கு (i = 0; facilities[i] != வெற்று; i++) {

                எனில் (ngx_strcmp(p + 9, facilities[i]) == 0) {
                    peer->facility = i;
                    செல் next;
                }
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog facility \"%s\"", p + 9);
            திரும்பு NGX_CONF_ERROR;

        } இல்லையெனில் எனில் (ngx_strncmp(p, "severity=", 9) == 0) {

            எனில் (peer->severity != NGX_CONF_UNSET_UINT) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"severity\"");
                திரும்பு NGX_CONF_ERROR;
            }

            க்கு (i = 0; severities[i] != வெற்று; i++) {

                எனில் (ngx_strcmp(p + 9, severities[i]) == 0) {
                    peer->severity = i;
                    செல் next;
                }
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog severity \"%s\"", p + 9);
            திரும்பு NGX_CONF_ERROR;

        } இல்லையெனில் எனில் (ngx_strncmp(p, "tag=", 4) == 0) {

            எனில் (peer->tag.data != வெற்று) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"tag\"");
                திரும்பு NGX_CONF_ERROR;
            }

            /*
             * RFC 3164: the TAG is a string of ABNF alphanumeric characters
             * that MUST NOT exceed 32 characters.
             */
            எனில் (len - 4 > 32) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "syslog tag length exceeds 32");
                திரும்பு NGX_CONF_ERROR;
            }

            க்கு (i = 4; i < len; i++) {
                c = ngx_tolower(p[i]);

                எனில் (c < '0' || (c > '9' && c < 'a' && c != '_') || c > 'z') {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "syslog \"tag\" only allows "
                                       "alphanumeric characters "
                                       "and underscore");
                    திரும்பு NGX_CONF_ERROR;
                }
            }

            peer->tag.data = p + 4;
            peer->tag.len = len - 4;

        } இல்லையெனில் எனில் (len == 10 && ngx_strncmp(p, "nohostname", 10) == 0) {
            peer->nohostname = 1;

        } இல்லையெனில் {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog parameter \"%s\"", p);
            திரும்பு NGX_CONF_ERROR;
        }

    next:

        எனில் (comma == வெற்று) {
            நிறுத்து;
        }

        p = comma + 1;
    }

    திரும்பு NGX_CONF_OK;
}


u_char *
ngx_syslog_add_header(ngx_syslog_peer_t *peer, u_char *buf)
{
    ngx_uint_t  pri;

    pri = peer->facility * 8 + peer->severity;

    எனில் (peer->nohostname) {
        திரும்பு ngx_sprintf(buf, "<%ui>%V %V: ", pri, &ngx_cached_syslog_time,
                           &peer->tag);
    }

    திரும்பு ngx_sprintf(buf, "<%ui>%V %V %V: ", pri, &ngx_cached_syslog_time,
                       peer->hostname, &peer->tag);
}


வெறுமை
ngx_syslog_writer(ngx_log_t *log, ngx_uint_t level, u_char *buf,
    size_t len)
{
    u_char             *p, செய்தி[NGX_SYSLOG_MAX_STR];
    ngx_uint_t          head_len;
    ngx_syslog_peer_t  *peer;

    peer = log->wdata;

    எனில் (peer->busy) {
        திரும்பு;
    }

    peer->busy = 1;
    peer->severity = level - 1;

    p = ngx_syslog_add_header(peer, செய்தி);
    head_len = p - செய்தி;

    len -= NGX_LINEFEED_SIZE;

    எனில் (len > NGX_SYSLOG_MAX_STR - head_len) {
        len = NGX_SYSLOG_MAX_STR - head_len;
    }

    p = ngx_snprintf(p, len, "%s", buf);

    (வெறுமை) ngx_syslog_send(peer, செய்தி, p - செய்தி);

    peer->busy = 0;
}


ssize_t
ngx_syslog_send(ngx_syslog_peer_t *peer, u_char *buf, size_t len)
{
    ssize_t  n;

    எனில் (peer->log.handler == வெற்று) {
        peer->log = *peer->logp;
        peer->log.handler = ngx_syslog_log_error;
        peer->log.data = peer;
        peer->log.action = "logging to syslog";
    }

    எனில் (peer->conn.fd == (ngx_socket_t) -1) {
        எனில் (ngx_syslog_init_peer(peer) != NGX_OK) {
            திரும்பு NGX_ERROR;
        }
    }

    எனில் (ngx_send) {
        n = ngx_send(&peer->conn, buf, len);

    } இல்லையெனில் {
        /* event module has not yet set ngx_io */
        n = ngx_os_io.send(&peer->conn, buf, len);
    }

    எனில் (n == NGX_ERROR) {

        எனில் (ngx_close_socket(peer->conn.fd) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        peer->conn.fd = (ngx_socket_t) -1;
    }

    திரும்பு n;
}


நிலையான ngx_int_t
ngx_syslog_init_peer(ngx_syslog_peer_t *peer)
{
    ngx_socket_t  fd;

    fd = ngx_socket(peer->server.sockaddr->sa_family, SOCK_DGRAM, 0);
    எனில் (fd == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_socket_n " failed");
        திரும்பு NGX_ERROR;
    }

    எனில் (ngx_nonblocking(fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_nonblocking_n " failed");
        செல் failed;
    }

    எனில் (connect(fd, peer->server.sockaddr, peer->server.socklen) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      "connect() failed");
        செல் failed;
    }

    peer->conn.fd = fd;
    peer->conn.log = &peer->log;

    /* UDP sockets are always ready to write */
    peer->conn.லிக->ready = 1;

    திரும்பு NGX_OK;

failed:

    எனில் (ngx_close_socket(fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_close_socket_n " failed");
    }

    திரும்பு NGX_ERROR;
}


நிலையான வெறுமை
ngx_syslog_cleanup(வெறுமை *data)
{
    ngx_syslog_peer_t  *peer = data;

    /* prevents further use of this peer */
    peer->busy = 1;

    எனில் (peer->conn.fd == (ngx_socket_t) -1) {
        திரும்பு;
    }

    எனில் (ngx_close_socket(peer->conn.fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_close_socket_n " failed");
    }
}


நிலையான u_char *
ngx_syslog_log_error(ngx_log_t *log, u_char *buf, size_t len)
{
    u_char             *p;
    ngx_syslog_peer_t  *peer;

    p = buf;

    எனில் (log->action) {
        p = ngx_snprintf(buf, len, " while %s", log->action);
        len -= p - buf;
    }

    peer = log->data;

    எனில் (peer) {
        p = ngx_snprintf(p, len, ", server: %V", &peer->server.name);
    }

    திரும்பு p;
}
