
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
स्थिर u_char *ngx_syslog_log_error(ngx_log_t *log, u_char *buf, माप_प्रकार len);


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

    यदि (ngx_syslog_parse_args(cf, peer) != NGX_CONF_OK) {
        फर्काऊ NGX_CONF_ERROR;
    }

    यदि (peer->server.sockaddr == रिक्त) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "no syslog server specified");
        फर्काऊ NGX_CONF_ERROR;
    }

    यदि (peer->facility == NGX_CONF_UNSET_UINT) {
        peer->facility = 23; /* local7 */
    }

    यदि (peer->severity == NGX_CONF_UNSET_UINT) {
        peer->severity = 6; /* info */
    }

    यदि (peer->tag.data == रिक्त) {
        ngx_str_set(&peer->tag, "nginx");
    }

    peer->hostname = &cf->cycle->hostname;
    peer->logp = &cf->cycle->new_log;

    peer->conn.fd = (ngx_socket_t) -1;

    peer->conn.पठ = &ngx_syslog_dummy_event;
    peer->conn.लिख = &ngx_syslog_dummy_event;

    ngx_syslog_dummy_event.log = &ngx_syslog_dummy_log;

    cln = ngx_pool_cleanup_add(cf->pool, 0);
    यदि (cln == रिक्त) {
        फर्काऊ NGX_CONF_ERROR;
    }

    cln->data = peer;
    cln->handler = ngx_syslog_cleanup;

    फर्काऊ NGX_CONF_OK;
}


स्थिर वर्ण *
ngx_syslog_parse_args(ngx_conf_t *cf, ngx_syslog_peer_t *peer)
{
    u_char      *p, *comma, c;
    माप_प्रकार       len;
    ngx_str_t   *मान;
    ngx_url_t    u;
    ngx_uint_t   i;

    मान = cf->args->elts;

    p = मान[1].data + परिमाण("syslog:") - 1;

    लागि ( ;; ) {
        comma = (u_char *) ngx_strchr(p, ',');

        यदि (comma != रिक्त) {
            len = comma - p;
            *comma = '\0';

        } अन्यथा {
            len = मान[1].data + मान[1].len - p;
        }

        यदि (ngx_strncmp(p, "server=", 7) == 0) {

            यदि (peer->server.sockaddr != रिक्त) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"server\"");
                फर्काऊ NGX_CONF_ERROR;
            }

            ngx_memzero(&u, परिमाण(ngx_url_t));

            u.url.data = p + 7;
            u.url.len = len - 7;
            u.default_port = 514;

            यदि (ngx_parse_url(cf->pool, &u) != NGX_OK) {
                यदि (u.err) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "%s in syslog server \"%V\"",
                                       u.err, &u.url);
                }

                फर्काऊ NGX_CONF_ERROR;
            }

            peer->server = u.addrs[0];

        } अन्यथा यदि (ngx_strncmp(p, "facility=", 9) == 0) {

            यदि (peer->facility != NGX_CONF_UNSET_UINT) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"facility\"");
                फर्काऊ NGX_CONF_ERROR;
            }

            लागि (i = 0; facilities[i] != रिक्त; i++) {

                यदि (ngx_strcmp(p + 9, facilities[i]) == 0) {
                    peer->facility = i;
                    गच्छ next;
                }
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog facility \"%s\"", p + 9);
            फर्काऊ NGX_CONF_ERROR;

        } अन्यथा यदि (ngx_strncmp(p, "severity=", 9) == 0) {

            यदि (peer->severity != NGX_CONF_UNSET_UINT) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"severity\"");
                फर्काऊ NGX_CONF_ERROR;
            }

            लागि (i = 0; severities[i] != रिक्त; i++) {

                यदि (ngx_strcmp(p + 9, severities[i]) == 0) {
                    peer->severity = i;
                    गच्छ next;
                }
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog severity \"%s\"", p + 9);
            फर्काऊ NGX_CONF_ERROR;

        } अन्यथा यदि (ngx_strncmp(p, "tag=", 4) == 0) {

            यदि (peer->tag.data != रिक्त) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"tag\"");
                फर्काऊ NGX_CONF_ERROR;
            }

            /*
             * RFC 3164: the TAG is a string of ABNF alphanumeric characters
             * that MUST NOT exceed 32 characters.
             */
            यदि (len - 4 > 32) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "syslog tag length exceeds 32");
                फर्काऊ NGX_CONF_ERROR;
            }

            लागि (i = 4; i < len; i++) {
                c = ngx_tolower(p[i]);

                यदि (c < '0' || (c > '9' && c < 'a' && c != '_') || c > 'z') {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "syslog \"tag\" only allows "
                                       "alphanumeric characters "
                                       "and underscore");
                    फर्काऊ NGX_CONF_ERROR;
                }
            }

            peer->tag.data = p + 4;
            peer->tag.len = len - 4;

        } अन्यथा यदि (len == 10 && ngx_strncmp(p, "nohostname", 10) == 0) {
            peer->nohostname = 1;

        } अन्यथा {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog parameter \"%s\"", p);
            फर्काऊ NGX_CONF_ERROR;
        }

    next:

        यदि (comma == रिक्त) {
            रोक;
        }

        p = comma + 1;
    }

    फर्काऊ NGX_CONF_OK;
}


u_char *
ngx_syslog_add_header(ngx_syslog_peer_t *peer, u_char *buf)
{
    ngx_uint_t  pri;

    pri = peer->facility * 8 + peer->severity;

    यदि (peer->nohostname) {
        फर्काऊ ngx_sprintf(buf, "<%ui>%V %V: ", pri, &ngx_cached_syslog_time,
                           &peer->tag);
    }

    फर्काऊ ngx_sprintf(buf, "<%ui>%V %V %V: ", pri, &ngx_cached_syslog_time,
                       peer->hostname, &peer->tag);
}


शून्य
ngx_syslog_writer(ngx_log_t *log, ngx_uint_t level, u_char *buf,
    माप_प्रकार len)
{
    u_char             *p, सन्देश[NGX_SYSLOG_MAX_STR];
    ngx_uint_t          head_len;
    ngx_syslog_peer_t  *peer;

    peer = log->wdata;

    यदि (peer->busy) {
        फर्काऊ;
    }

    peer->busy = 1;
    peer->severity = level - 1;

    p = ngx_syslog_add_header(peer, सन्देश);
    head_len = p - सन्देश;

    len -= NGX_LINEFEED_SIZE;

    यदि (len > NGX_SYSLOG_MAX_STR - head_len) {
        len = NGX_SYSLOG_MAX_STR - head_len;
    }

    p = ngx_snprintf(p, len, "%s", buf);

    (शून्य) ngx_syslog_send(peer, सन्देश, p - सन्देश);

    peer->busy = 0;
}


ssize_t
ngx_syslog_send(ngx_syslog_peer_t *peer, u_char *buf, माप_प्रकार len)
{
    ssize_t  n;

    यदि (peer->log.handler == रिक्त) {
        peer->log = *peer->logp;
        peer->log.handler = ngx_syslog_log_error;
        peer->log.data = peer;
        peer->log.action = "logging to syslog";
    }

    यदि (peer->conn.fd == (ngx_socket_t) -1) {
        यदि (ngx_syslog_init_peer(peer) != NGX_OK) {
            फर्काऊ NGX_ERROR;
        }
    }

    यदि (ngx_send) {
        n = ngx_send(&peer->conn, buf, len);

    } अन्यथा {
        /* event module has not yet set ngx_io */
        n = ngx_os_io.send(&peer->conn, buf, len);
    }

    यदि (n == NGX_ERROR) {

        यदि (ngx_close_socket(peer->conn.fd) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        peer->conn.fd = (ngx_socket_t) -1;
    }

    फर्काऊ n;
}


स्थिर ngx_int_t
ngx_syslog_init_peer(ngx_syslog_peer_t *peer)
{
    ngx_socket_t  fd;

    fd = ngx_socket(peer->server.sockaddr->sa_family, SOCK_DGRAM, 0);
    यदि (fd == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_socket_n " failed");
        फर्काऊ NGX_ERROR;
    }

    यदि (ngx_nonblocking(fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_nonblocking_n " failed");
        गच्छ failed;
    }

    यदि (connect(fd, peer->server.sockaddr, peer->server.socklen) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      "connect() failed");
        गच्छ failed;
    }

    peer->conn.fd = fd;
    peer->conn.log = &peer->log;

    /* UDP sockets are always ready to write */
    peer->conn.लिख->ready = 1;

    फर्काऊ NGX_OK;

failed:

    यदि (ngx_close_socket(fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_close_socket_n " failed");
    }

    फर्काऊ NGX_ERROR;
}


स्थिर शून्य
ngx_syslog_cleanup(शून्य *data)
{
    ngx_syslog_peer_t  *peer = data;

    /* prevents further use of this peer */
    peer->busy = 1;

    यदि (peer->conn.fd == (ngx_socket_t) -1) {
        फर्काऊ;
    }

    यदि (ngx_close_socket(peer->conn.fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_close_socket_n " failed");
    }
}


स्थिर u_char *
ngx_syslog_log_error(ngx_log_t *log, u_char *buf, माप_प्रकार len)
{
    u_char             *p;
    ngx_syslog_peer_t  *peer;

    p = buf;

    यदि (log->action) {
        p = ngx_snprintf(buf, len, " while %s", log->action);
        len -= p - buf;
    }

    peer = log->data;

    यदि (peer) {
        p = ngx_snprintf(p, len, ", server: %V", &peer->server.name);
    }

    फर्काऊ p;
}
