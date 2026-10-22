
/*
 * Copyright (C) Nginx, Inc.
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>
#समावेश <ngx_event.h>


#घोषणा NGX_SYSLOG_MAX_STR                                                    \
    NGX_MAX_ERROR_STR + माप("<255>Jan 01 00:00:00 ") - 1                   \
    + (NGX_MAXHOSTNAMELEN - 1) + 1 /* space */                                \
    + 32 /* tag */ + 2 /* colon, space */


अटल अक्षर *ngx_syslog_parse_args(ngx_conf_t *cf, ngx_syslog_peer_t *peer);
अटल ngx_int_t ngx_syslog_init_peer(ngx_syslog_peer_t *peer);
अटल व्योम ngx_syslog_cleanup(व्योम *तथ्य);
अटल u_char *ngx_syslog_log_error(ngx_log_t *log, u_char *buf, माप_प्रकार len);


अटल अक्षर  *facilities[] = {
    "kern", "user", "mail", "daemon", "auth", "intern", "lpr", "news", "uucp",
    "clock", "authpriv", "ftp", "ntp", "audit", "alert", "cron", "local0",
    "local1", "local2", "local3", "local4", "local5", "local6", "local7",
    शून्य
};

/* note 'error/warn' like in nginx.conf, not 'err/warning' */
अटल अक्षर  *severities[] = {
    "emerg", "alert", "crit", "error", "warn", "notice", "info", "debug", शून्य
};

अटल ngx_log_t    ngx_syslog_dummy_log;
अटल ngx_event_t  ngx_syslog_dummy_event;


अक्षर *
ngx_syslog_process_conf(ngx_conf_t *cf, ngx_syslog_peer_t *peer)
{
    ngx_pool_cleanup_t  *cln;

    peer->facility = NGX_CONF_UNSET_UINT;
    peer->severity = NGX_CONF_UNSET_UINT;

    अगर (ngx_syslog_parse_args(cf, peer) != NGX_CONF_OK) {
        वापस NGX_CONF_ERROR;
    }

    अगर (peer->server.sockaddr == शून्य) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "no syslog server specified");
        वापस NGX_CONF_ERROR;
    }

    अगर (peer->facility == NGX_CONF_UNSET_UINT) {
        peer->facility = 23; /* local7 */
    }

    अगर (peer->severity == NGX_CONF_UNSET_UINT) {
        peer->severity = 6; /* info */
    }

    अगर (peer->tag.तथ्य == शून्य) {
        ngx_str_set(&peer->tag, "nginx");
    }

    peer->hostname = &cf->cycle->hostname;
    peer->logp = &cf->cycle->new_log;

    peer->conn.fd = (ngx_socket_t) -1;

    peer->conn.पढ़ो = &ngx_syslog_dummy_event;
    peer->conn.ग_लिखो = &ngx_syslog_dummy_event;

    ngx_syslog_dummy_event.log = &ngx_syslog_dummy_log;

    cln = ngx_pool_cleanup_add(cf->pool, 0);
    अगर (cln == शून्य) {
        वापस NGX_CONF_ERROR;
    }

    cln->तथ्य = peer;
    cln->handler = ngx_syslog_cleanup;

    वापस NGX_CONF_OK;
}


अटल अक्षर *
ngx_syslog_parse_args(ngx_conf_t *cf, ngx_syslog_peer_t *peer)
{
    u_char      *p, *comma, c;
    माप_प्रकार       len;
    ngx_str_t   *मूल्य;
    ngx_url_t    u;
    ngx_uint_t   i;

    मूल्य = cf->args->elts;

    p = मूल्य[1].तथ्य + माप("syslog:") - 1;

    क्रम ( ;; ) {
        comma = (u_char *) ngx_strchr(p, ',');

        अगर (comma != शून्य) {
            len = comma - p;
            *comma = '\0';

        } अन्यथा {
            len = मूल्य[1].तथ्य + मूल्य[1].len - p;
        }

        अगर (ngx_strncmp(p, "server=", 7) == 0) {

            अगर (peer->server.sockaddr != शून्य) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"server\"");
                वापस NGX_CONF_ERROR;
            }

            ngx_memzero(&u, माप(ngx_url_t));

            u.url.तथ्य = p + 7;
            u.url.len = len - 7;
            u.default_port = 514;

            अगर (ngx_parse_url(cf->pool, &u) != NGX_OK) {
                अगर (u.err) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "%s in syslog server \"%V\"",
                                       u.err, &u.url);
                }

                वापस NGX_CONF_ERROR;
            }

            peer->server = u.addrs[0];

        } अन्यथा अगर (ngx_strncmp(p, "facility=", 9) == 0) {

            अगर (peer->facility != NGX_CONF_UNSET_UINT) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"facility\"");
                वापस NGX_CONF_ERROR;
            }

            क्रम (i = 0; facilities[i] != शून्य; i++) {

                अगर (ngx_strcmp(p + 9, facilities[i]) == 0) {
                    peer->facility = i;
                    जाओ अग्र;
                }
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog facility \"%s\"", p + 9);
            वापस NGX_CONF_ERROR;

        } अन्यथा अगर (ngx_strncmp(p, "severity=", 9) == 0) {

            अगर (peer->severity != NGX_CONF_UNSET_UINT) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"severity\"");
                वापस NGX_CONF_ERROR;
            }

            क्रम (i = 0; severities[i] != शून्य; i++) {

                अगर (ngx_strcmp(p + 9, severities[i]) == 0) {
                    peer->severity = i;
                    जाओ अग्र;
                }
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog severity \"%s\"", p + 9);
            वापस NGX_CONF_ERROR;

        } अन्यथा अगर (ngx_strncmp(p, "tag=", 4) == 0) {

            अगर (peer->tag.तथ्य != शून्य) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"tag\"");
                वापस NGX_CONF_ERROR;
            }

            /*
             * RFC 3164: the TAG is a string of ABNF alphanumeric characters
             * that MUST NOT exceed 32 characters.
             */
            अगर (len - 4 > 32) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "syslog tag length exceeds 32");
                वापस NGX_CONF_ERROR;
            }

            क्रम (i = 4; i < len; i++) {
                c = ngx_tolower(p[i]);

                अगर (c < '0' || (c > '9' && c < 'a' && c != '_') || c > 'z') {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "syslog \"tag\" only allows "
                                       "alphanumeric characters "
                                       "and underscore");
                    वापस NGX_CONF_ERROR;
                }
            }

            peer->tag.तथ्य = p + 4;
            peer->tag.len = len - 4;

        } अन्यथा अगर (len == 10 && ngx_strncmp(p, "nohostname", 10) == 0) {
            peer->nohostname = 1;

        } अन्यथा {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog parameter \"%s\"", p);
            वापस NGX_CONF_ERROR;
        }

    अग्र:

        अगर (comma == शून्य) {
            अवरोध;
        }

        p = comma + 1;
    }

    वापस NGX_CONF_OK;
}


u_char *
ngx_syslog_add_header(ngx_syslog_peer_t *peer, u_char *buf)
{
    ngx_uint_t  pri;

    pri = peer->facility * 8 + peer->severity;

    अगर (peer->nohostname) {
        वापस ngx_sprintf(buf, "<%ui>%V %V: ", pri, &ngx_cached_syslog_time,
                           &peer->tag);
    }

    वापस ngx_sprintf(buf, "<%ui>%V %V %V: ", pri, &ngx_cached_syslog_time,
                       peer->hostname, &peer->tag);
}


व्योम
ngx_syslog_writer(ngx_log_t *log, ngx_uint_t level, u_char *buf,
    माप_प्रकार len)
{
    u_char             *p, संदेश[NGX_SYSLOG_MAX_STR];
    ngx_uint_t          शीर्ष_len;
    ngx_syslog_peer_t  *peer;

    peer = log->wdata;

    अगर (peer->busy) {
        वापस;
    }

    peer->busy = 1;
    peer->severity = level - 1;

    p = ngx_syslog_add_header(peer, संदेश);
    शीर्ष_len = p - संदेश;

    len -= NGX_LINEFEED_आकार;

    अगर (len > NGX_SYSLOG_MAX_STR - शीर्ष_len) {
        len = NGX_SYSLOG_MAX_STR - शीर्ष_len;
    }

    p = ngx_snprintf(p, len, "%s", buf);

    (व्योम) ngx_syslog_send(peer, संदेश, p - संदेश);

    peer->busy = 0;
}


ssize_t
ngx_syslog_send(ngx_syslog_peer_t *peer, u_char *buf, माप_प्रकार len)
{
    ssize_t  n;

    अगर (peer->log.handler == शून्य) {
        peer->log = *peer->logp;
        peer->log.handler = ngx_syslog_log_error;
        peer->log.तथ्य = peer;
        peer->log.action = "logging to syslog";
    }

    अगर (peer->conn.fd == (ngx_socket_t) -1) {
        अगर (ngx_syslog_init_peer(peer) != NGX_OK) {
            वापस NGX_ERROR;
        }
    }

    अगर (ngx_send) {
        n = ngx_send(&peer->conn, buf, len);

    } अन्यथा {
        /* event module has not yet set ngx_io */
        n = ngx_os_io.send(&peer->conn, buf, len);
    }

    अगर (n == NGX_ERROR) {

        अगर (ngx_close_socket(peer->conn.fd) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        peer->conn.fd = (ngx_socket_t) -1;
    }

    वापस n;
}


अटल ngx_int_t
ngx_syslog_init_peer(ngx_syslog_peer_t *peer)
{
    ngx_socket_t  fd;

    fd = ngx_socket(peer->server.sockaddr->sa_family, SOCK_DGRAM, 0);
    अगर (fd == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_socket_n " failed");
        वापस NGX_ERROR;
    }

    अगर (ngx_nonblocking(fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_nonblocking_n " failed");
        जाओ failed;
    }

    अगर (connect(fd, peer->server.sockaddr, peer->server.socklen) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      "connect() failed");
        जाओ failed;
    }

    peer->conn.fd = fd;
    peer->conn.log = &peer->log;

    /* UDP sockets are always ready to write */
    peer->conn.ग_लिखो->ready = 1;

    वापस NGX_OK;

failed:

    अगर (ngx_close_socket(fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_close_socket_n " failed");
    }

    वापस NGX_ERROR;
}


अटल व्योम
ngx_syslog_cleanup(व्योम *तथ्य)
{
    ngx_syslog_peer_t  *peer = तथ्य;

    /* prevents further use of this peer */
    peer->busy = 1;

    अगर (peer->conn.fd == (ngx_socket_t) -1) {
        वापस;
    }

    अगर (ngx_close_socket(peer->conn.fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_close_socket_n " failed");
    }
}


अटल u_char *
ngx_syslog_log_error(ngx_log_t *log, u_char *buf, माप_प्रकार len)
{
    u_char             *p;
    ngx_syslog_peer_t  *peer;

    p = buf;

    अगर (log->action) {
        p = ngx_snprintf(buf, len, " while %s", log->action);
        len -= p - buf;
    }

    peer = log->तथ्य;

    अगर (peer) {
        p = ngx_snprintf(p, len, ", server: %V", &peer->server.name);
    }

    वापस p;
}
