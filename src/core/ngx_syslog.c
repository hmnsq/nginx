
/*
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>
#अन्तर्भाव <ngx_event.h>


#define NGX_SYSLOG_MAX_STR                                                    \
    NGX_MAX_ERROR_STR + परिमाण("<255>Jan 01 00:00:00 ") - 1                   \
    + (NGX_MAXHOSTNAMELEN - 1) + 1 /* space */                                \
    + 32 /* tag */ + 2 /* colon, space */


स्थिर वर्ण *ngx_syslog_parse_args(ngx_conf_t *cf, ngx_syslog_peer_t *peer);
स्थिर ngx_int_t ngx_syslog_init_peer(ngx_syslog_peer_t *peer);
स्थिर शून्य ngx_syslog_cleanup(शून्य *data);
स्थिर u_char *ngx_syslog_log_error(ngx_log_t *log, u_char *buf, size_t len);


स्थिर वर्ण  *facilities[] = {
    "kern", "user", "mail", "daemon", "auth", "intern", "lpr", "news", "uucp",
    "clock", "authpriv", "ftp", "ntp", "audit", "alert", "cron", "local0",
    "local1", "local2", "local3", "local4", "local5", "local6", "local7",
    रिक्त
};

/* note 'error/warn' like in nginx.conf, not 'err/warning' */
स्थिर वर्ण  *severities[] = {
    "emerg", "alert", "crit", "error", "warn", "notice", "info", "debug", रिक्त
};

स्थिर ngx_log_t    ngx_syslog_dummy_log;
स्थिर ngx_event_t  ngx_syslog_dummy_event;


वर्ण *
ngx_syslog_process_conf(ngx_conf_t *cf, ngx_syslog_peer_t *peer)
{
    ngx_pool_cleanup_t  *cln;

    peer->facility = NGX_CONF_UNSET_UINT;
    peer->severity = NGX_CONF_UNSET_UINT;

    सचे (ngx_syslog_parse_args(cf, peer) != NGX_CONF_OK) {
        पटिनिवत्त NGX_CONF_ERROR;
    }

    सचे (peer->server.sockaddr == रिक्त) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "no syslog server specified");
        पटिनिवत्त NGX_CONF_ERROR;
    }

    सचे (peer->facility == NGX_CONF_UNSET_UINT) {
        peer->facility = 23; /* local7 */
    }

    सचे (peer->severity == NGX_CONF_UNSET_UINT) {
        peer->severity = 6; /* info */
    }

    सचे (peer->tag.data == रिक्त) {
        ngx_str_set(&peer->tag, "nginx");
    }

    peer->hostname = &cf->cycle->hostname;
    peer->logp = &cf->cycle->new_log;

    peer->conn.fd = (ngx_socket_t) -1;

    peer->conn.पठ = &ngx_syslog_dummy_event;
    peer->conn.लिख = &ngx_syslog_dummy_event;

    ngx_syslog_dummy_event.log = &ngx_syslog_dummy_log;

    cln = ngx_pool_cleanup_add(cf->pool, 0);
    सचे (cln == रिक्त) {
        पटिनिवत्त NGX_CONF_ERROR;
    }

    cln->data = peer;
    cln->handler = ngx_syslog_cleanup;

    पटिनिवत्त NGX_CONF_OK;
}


स्थिर वर्ण *
ngx_syslog_parse_args(ngx_conf_t *cf, ngx_syslog_peer_t *peer)
{
    u_char      *p, *comma, c;
    size_t       len;
    ngx_str_t   *मानम्;
    ngx_url_t    u;
    ngx_uint_t   i;

    मानम् = cf->args->elts;

    p = मानम्[1].data + परिमाण("syslog:") - 1;

    अत्थाय ( ;; ) {
        comma = (u_char *) ngx_strchr(p, ',');

        सचे (comma != रिक्त) {
            len = comma - p;
            *comma = '\0';

        } अञ्ञथा {
            len = मानम्[1].data + मानम्[1].len - p;
        }

        सचे (ngx_strncmp(p, "server=", 7) == 0) {

            सचे (peer->server.sockaddr != रिक्त) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"server\"");
                पटिनिवत्त NGX_CONF_ERROR;
            }

            ngx_memzero(&u, परिमाण(ngx_url_t));

            u.url.data = p + 7;
            u.url.len = len - 7;
            u.default_port = 514;

            सचे (ngx_parse_url(cf->pool, &u) != NGX_OK) {
                सचे (u.err) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "%s in syslog server \"%V\"",
                                       u.err, &u.url);
                }

                पटिनिवत्त NGX_CONF_ERROR;
            }

            peer->server = u.addrs[0];

        } अञ्ञथा सचे (ngx_strncmp(p, "facility=", 9) == 0) {

            सचे (peer->facility != NGX_CONF_UNSET_UINT) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"facility\"");
                पटिनिवत्त NGX_CONF_ERROR;
            }

            अत्थाय (i = 0; facilities[i] != रिक्त; i++) {

                सचे (ngx_strcmp(p + 9, facilities[i]) == 0) {
                    peer->facility = i;
                    गच्छ next;
                }
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog facility \"%s\"", p + 9);
            पटिनिवत्त NGX_CONF_ERROR;

        } अञ्ञथा सचे (ngx_strncmp(p, "severity=", 9) == 0) {

            सचे (peer->severity != NGX_CONF_UNSET_UINT) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"severity\"");
                पटिनिवत्त NGX_CONF_ERROR;
            }

            अत्थाय (i = 0; severities[i] != रिक्त; i++) {

                सचे (ngx_strcmp(p + 9, severities[i]) == 0) {
                    peer->severity = i;
                    गच्छ next;
                }
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog severity \"%s\"", p + 9);
            पटिनिवत्त NGX_CONF_ERROR;

        } अञ्ञथा सचे (ngx_strncmp(p, "tag=", 4) == 0) {

            सचे (peer->tag.data != रिक्त) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"tag\"");
                पटिनिवत्त NGX_CONF_ERROR;
            }

            /*
             * RFC 3164: the TAG is a string of ABNF alphanumeric characters
             * that MUST NOT exceed 32 characters.
             */
            सचे (len - 4 > 32) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "syslog tag length exceeds 32");
                पटिनिवत्त NGX_CONF_ERROR;
            }

            अत्थाय (i = 4; i < len; i++) {
                c = ngx_tolower(p[i]);

                सचे (c < '0' || (c > '9' && c < 'a' && c != '_') || c > 'z') {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "syslog \"tag\" only allows "
                                       "alphanumeric characters "
                                       "and underscore");
                    पटिनिवत्त NGX_CONF_ERROR;
                }
            }

            peer->tag.data = p + 4;
            peer->tag.len = len - 4;

        } अञ्ञथा सचे (len == 10 && ngx_strncmp(p, "nohostname", 10) == 0) {
            peer->nohostname = 1;

        } अञ्ञथा {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog parameter \"%s\"", p);
            पटिनिवत्त NGX_CONF_ERROR;
        }

    next:

        सचे (comma == रिक्त) {
            भङ्ग;
        }

        p = comma + 1;
    }

    पटिनिवत्त NGX_CONF_OK;
}


u_char *
ngx_syslog_add_header(ngx_syslog_peer_t *peer, u_char *buf)
{
    ngx_uint_t  pri;

    pri = peer->facility * 8 + peer->severity;

    सचे (peer->nohostname) {
        पटिनिवत्त ngx_sprintf(buf, "<%ui>%V %V: ", pri, &ngx_cached_syslog_time,
                           &peer->tag);
    }

    पटिनिवत्त ngx_sprintf(buf, "<%ui>%V %V %V: ", pri, &ngx_cached_syslog_time,
                       peer->hostname, &peer->tag);
}


शून्य
ngx_syslog_writer(ngx_log_t *log, ngx_uint_t level, u_char *buf,
    size_t len)
{
    u_char             *p, सन्देशः[NGX_SYSLOG_MAX_STR];
    ngx_uint_t          head_len;
    ngx_syslog_peer_t  *peer;

    peer = log->wdata;

    सचे (peer->busy) {
        पटिनिवत्त;
    }

    peer->busy = 1;
    peer->severity = level - 1;

    p = ngx_syslog_add_header(peer, सन्देशः);
    head_len = p - सन्देशः;

    len -= NGX_LINEFEED_SIZE;

    सचे (len > NGX_SYSLOG_MAX_STR - head_len) {
        len = NGX_SYSLOG_MAX_STR - head_len;
    }

    p = ngx_snprintf(p, len, "%s", buf);

    (शून्य) ngx_syslog_send(peer, सन्देशः, p - सन्देशः);

    peer->busy = 0;
}


ssize_t
ngx_syslog_send(ngx_syslog_peer_t *peer, u_char *buf, size_t len)
{
    ssize_t  n;

    सचे (peer->log.handler == रिक्त) {
        peer->log = *peer->logp;
        peer->log.handler = ngx_syslog_log_error;
        peer->log.data = peer;
        peer->log.action = "logging to syslog";
    }

    सचे (peer->conn.fd == (ngx_socket_t) -1) {
        सचे (ngx_syslog_init_peer(peer) != NGX_OK) {
            पटिनिवत्त NGX_ERROR;
        }
    }

    सचे (ngx_send) {
        n = ngx_send(&peer->conn, buf, len);

    } अञ्ञथा {
        /* event module has not yet set ngx_io */
        n = ngx_os_io.send(&peer->conn, buf, len);
    }

    सचे (n == NGX_ERROR) {

        सचे (ngx_close_socket(peer->conn.fd) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        peer->conn.fd = (ngx_socket_t) -1;
    }

    पटिनिवत्त n;
}


स्थिर ngx_int_t
ngx_syslog_init_peer(ngx_syslog_peer_t *peer)
{
    ngx_socket_t  fd;

    fd = ngx_socket(peer->server.sockaddr->sa_family, SOCK_DGRAM, 0);
    सचे (fd == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_socket_n " failed");
        पटिनिवत्त NGX_ERROR;
    }

    सचे (ngx_nonblocking(fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_nonblocking_n " failed");
        गच्छ failed;
    }

    सचे (connect(fd, peer->server.sockaddr, peer->server.socklen) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      "connect() failed");
        गच्छ failed;
    }

    peer->conn.fd = fd;
    peer->conn.log = &peer->log;

    /* UDP sockets are always ready to write */
    peer->conn.लिख->ready = 1;

    पटिनिवत्त NGX_OK;

failed:

    सचे (ngx_close_socket(fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_close_socket_n " failed");
    }

    पटिनिवत्त NGX_ERROR;
}


स्थिर शून्य
ngx_syslog_cleanup(शून्य *data)
{
    ngx_syslog_peer_t  *peer = data;

    /* prevents further use of this peer */
    peer->busy = 1;

    सचे (peer->conn.fd == (ngx_socket_t) -1) {
        पटिनिवत्त;
    }

    सचे (ngx_close_socket(peer->conn.fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_close_socket_n " failed");
    }
}


स्थिर u_char *
ngx_syslog_log_error(ngx_log_t *log, u_char *buf, size_t len)
{
    u_char             *p;
    ngx_syslog_peer_t  *peer;

    p = buf;

    सचे (log->action) {
        p = ngx_snprintf(buf, len, " while %s", log->action);
        len -= p - buf;
    }

    peer = log->data;

    सचे (peer) {
        p = ngx_snprintf(p, len, ", server: %V", &peer->server.name);
    }

    पटिनिवत्त p;
}
