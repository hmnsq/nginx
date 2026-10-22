
/*
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_event.h>


#define NGX_SYSLOG_MAX_STR                                                    \
    NGX_MAX_ERROR_STR + parimaa_na("<255>Jan 01 00:00:00 ") - 1                   \
    + (NGX_MAXHOSTNAMELEN - 1) + 1 /* space */                                \
    + 32 /* tag */ + 2 /* colon, space */


static var_na *ngx_syslog_parse_args(ngx_conf_t *cf, ngx_syslog_peer_t *peer);
static ngx_int_t ngx_syslog_init_peer(ngx_syslog_peer_t *peer);
static void ngx_syslog_cleanup(void *data);
static u_char *ngx_syslog_log_error(ngx_log_t *log, u_char *buf, size_t len);


static var_na  *facilities[] = {
    "kern", "user", "mail", "daemon", "auth", "intern", "lpr", "news", "uucp",
    "clock", "authpriv", "ftp", "ntp", "audit", "alert", "cron", "local0",
    "local1", "local2", "local3", "local4", "local5", "local6", "local7",
    NULL
};

/* note 'error/warn' like in nginx.conf, not 'err/warning' */
static var_na  *severities[] = {
    "emerg", "alert", "crit", "error", "warn", "notice", "info", "debug", NULL
};

static ngx_log_t    ngx_syslog_dummy_log;
static ngx_event_t  ngx_syslog_dummy_event;


var_na *
ngx_syslog_process_conf(ngx_conf_t *cf, ngx_syslog_peer_t *peer)
{
    ngx_pool_cleanup_t  *cln;

    peer->facility = NGX_CONF_UNSET_UINT;
    peer->severity = NGX_CONF_UNSET_UINT;

    ប្រសិនបើ (ngx_syslog_parse_args(cf, peer) != NGX_CONF_OK) {
        ត្រឡប់ NGX_CONF_ERROR;
    }

    ប្រសិនបើ (peer->server.sockaddr == NULL) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "no syslog server specified");
        ត្រឡប់ NGX_CONF_ERROR;
    }

    ប្រសិនបើ (peer->facility == NGX_CONF_UNSET_UINT) {
        peer->facility = 23; /* local7 */
    }

    ប្រសិនបើ (peer->severity == NGX_CONF_UNSET_UINT) {
        peer->severity = 6; /* info */
    }

    ប្រសិនបើ (peer->tag.data == NULL) {
        ngx_str_set(&peer->tag, "nginx");
    }

    peer->hostname = &cf->cycle->hostname;
    peer->logp = &cf->cycle->new_log;

    peer->conn.fd = (ngx_socket_t) -1;

    peer->conn.read = &ngx_syslog_dummy_event;
    peer->conn.write = &ngx_syslog_dummy_event;

    ngx_syslog_dummy_event.log = &ngx_syslog_dummy_log;

    cln = ngx_pool_cleanup_add(cf->pool, 0);
    ប្រសិនបើ (cln == NULL) {
        ត្រឡប់ NGX_CONF_ERROR;
    }

    cln->data = peer;
    cln->handler = ngx_syslog_cleanup;

    ត្រឡប់ NGX_CONF_OK;
}


static var_na *
ngx_syslog_parse_args(ngx_conf_t *cf, ngx_syslog_peer_t *peer)
{
    u_char      *p, *comma, c;
    size_t       len;
    ngx_str_t   *value;
    ngx_url_t    u;
    ngx_uint_t   i;

    value = cf->args->elts;

    p = value[1].data + parimaa_na("syslog:") - 1;

    សម្រាប់ ( ;; ) {
        comma = (u_char *) ngx_strchr(p, ',');

        ប្រសិនបើ (comma != NULL) {
            len = comma - p;
            *comma = '\0';

        } បើមិនដូច្នេះ {
            len = value[1].data + value[1].len - p;
        }

        ប្រសិនបើ (ngx_strncmp(p, "server=", 7) == 0) {

            ប្រសិនបើ (peer->server.sockaddr != NULL) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"server\"");
                ត្រឡប់ NGX_CONF_ERROR;
            }

            ngx_memzero(&u, parimaa_na(ngx_url_t));

            u.url.data = p + 7;
            u.url.len = len - 7;
            u.default_port = 514;

            ប្រសិនបើ (ngx_parse_url(cf->pool, &u) != NGX_OK) {
                ប្រសិនបើ (u.err) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "%s in syslog server \"%V\"",
                                       u.err, &u.url);
                }

                ត្រឡប់ NGX_CONF_ERROR;
            }

            peer->server = u.addrs[0];

        } បើមិនដូច្នេះ ប្រសិនបើ (ngx_strncmp(p, "facility=", 9) == 0) {

            ប្រសិនបើ (peer->facility != NGX_CONF_UNSET_UINT) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"facility\"");
                ត្រឡប់ NGX_CONF_ERROR;
            }

            សម្រាប់ (i = 0; facilities[i] != NULL; i++) {

                ប្រសិនបើ (ngx_strcmp(p + 9, facilities[i]) == 0) {
                    peer->facility = i;
                    goto next;
                }
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog facility \"%s\"", p + 9);
            ត្រឡប់ NGX_CONF_ERROR;

        } បើមិនដូច្នេះ ប្រសិនបើ (ngx_strncmp(p, "severity=", 9) == 0) {

            ប្រសិនបើ (peer->severity != NGX_CONF_UNSET_UINT) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"severity\"");
                ត្រឡប់ NGX_CONF_ERROR;
            }

            សម្រាប់ (i = 0; severities[i] != NULL; i++) {

                ប្រសិនបើ (ngx_strcmp(p + 9, severities[i]) == 0) {
                    peer->severity = i;
                    goto next;
                }
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog severity \"%s\"", p + 9);
            ត្រឡប់ NGX_CONF_ERROR;

        } បើមិនដូច្នេះ ប្រសិនបើ (ngx_strncmp(p, "tag=", 4) == 0) {

            ប្រសិនបើ (peer->tag.data != NULL) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"tag\"");
                ត្រឡប់ NGX_CONF_ERROR;
            }

            /*
             * RFC 3164: the TAG is a string of ABNF alphanumeric characters
             * that MUST NOT exceed 32 characters.
             */
            ប្រសិនបើ (len - 4 > 32) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "syslog tag length exceeds 32");
                ត្រឡប់ NGX_CONF_ERROR;
            }

            សម្រាប់ (i = 4; i < len; i++) {
                c = ngx_tolower(p[i]);

                ប្រសិនបើ (c < '0' || (c > '9' && c < 'a' && c != '_') || c > 'z') {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "syslog \"tag\" only allows "
                                       "alphanumeric characters "
                                       "and underscore");
                    ត្រឡប់ NGX_CONF_ERROR;
                }
            }

            peer->tag.data = p + 4;
            peer->tag.len = len - 4;

        } បើមិនដូច្នេះ ប្រសិនបើ (len == 10 && ngx_strncmp(p, "nohostname", 10) == 0) {
            peer->nohostname = 1;

        } បើមិនដូច្នេះ {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog parameter \"%s\"", p);
            ត្រឡប់ NGX_CONF_ERROR;
        }

    next:

        ប្រសិនបើ (comma == NULL) {
            bha_nga;
        }

        p = comma + 1;
    }

    ត្រឡប់ NGX_CONF_OK;
}


u_char *
ngx_syslog_add_header(ngx_syslog_peer_t *peer, u_char *buf)
{
    ngx_uint_t  pri;

    pri = peer->facility * 8 + peer->severity;

    ប្រសិនបើ (peer->nohostname) {
        ត្រឡប់ ngx_sprintf(buf, "<%ui>%V %V: ", pri, &ngx_cached_syslog_time,
                           &peer->tag);
    }

    ត្រឡប់ ngx_sprintf(buf, "<%ui>%V %V %V: ", pri, &ngx_cached_syslog_time,
                       peer->hostname, &peer->tag);
}


void
ngx_syslog_writer(ngx_log_t *log, ngx_uint_t level, u_char *buf,
    size_t len)
{
    u_char             *p, msg[NGX_SYSLOG_MAX_STR];
    ngx_uint_t          head_len;
    ngx_syslog_peer_t  *peer;

    peer = log->wdata;

    ប្រសិនបើ (peer->busy) {
        ត្រឡប់;
    }

    peer->busy = 1;
    peer->severity = level - 1;

    p = ngx_syslog_add_header(peer, msg);
    head_len = p - msg;

    len -= NGX_LINEFEED_SIZE;

    ប្រសិនបើ (len > NGX_SYSLOG_MAX_STR - head_len) {
        len = NGX_SYSLOG_MAX_STR - head_len;
    }

    p = ngx_snprintf(p, len, "%s", buf);

    (void) ngx_syslog_send(peer, msg, p - msg);

    peer->busy = 0;
}


ssize_t
ngx_syslog_send(ngx_syslog_peer_t *peer, u_char *buf, size_t len)
{
    ssize_t  n;

    ប្រសិនបើ (peer->log.handler == NULL) {
        peer->log = *peer->logp;
        peer->log.handler = ngx_syslog_log_error;
        peer->log.data = peer;
        peer->log.action = "logging to syslog";
    }

    ប្រសិនបើ (peer->conn.fd == (ngx_socket_t) -1) {
        ប្រសិនបើ (ngx_syslog_init_peer(peer) != NGX_OK) {
            ត្រឡប់ NGX_ERROR;
        }
    }

    ប្រសិនបើ (ngx_send) {
        n = ngx_send(&peer->conn, buf, len);

    } បើមិនដូច្នេះ {
        /* event module has not yet set ngx_io */
        n = ngx_os_io.send(&peer->conn, buf, len);
    }

    ប្រសិនបើ (n == NGX_ERROR) {

        ប្រសិនបើ (ngx_close_socket(peer->conn.fd) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        peer->conn.fd = (ngx_socket_t) -1;
    }

    ត្រឡប់ n;
}


static ngx_int_t
ngx_syslog_init_peer(ngx_syslog_peer_t *peer)
{
    ngx_socket_t  fd;

    fd = ngx_socket(peer->server.sockaddr->sa_family, SOCK_DGRAM, 0);
    ប្រសិនបើ (fd == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_socket_n " failed");
        ត្រឡប់ NGX_ERROR;
    }

    ប្រសិនបើ (ngx_nonblocking(fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_nonblocking_n " failed");
        goto failed;
    }

    ប្រសិនបើ (connect(fd, peer->server.sockaddr, peer->server.socklen) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      "connect() failed");
        goto failed;
    }

    peer->conn.fd = fd;
    peer->conn.log = &peer->log;

    /* UDP sockets are always ready to write */
    peer->conn.write->ready = 1;

    ត្រឡប់ NGX_OK;

failed:

    ប្រសិនបើ (ngx_close_socket(fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_close_socket_n " failed");
    }

    ត្រឡប់ NGX_ERROR;
}


static void
ngx_syslog_cleanup(void *data)
{
    ngx_syslog_peer_t  *peer = data;

    /* prevents further use of this peer */
    peer->busy = 1;

    ប្រសិនបើ (peer->conn.fd == (ngx_socket_t) -1) {
        ត្រឡប់;
    }

    ប្រសិនបើ (ngx_close_socket(peer->conn.fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_close_socket_n " failed");
    }
}


static u_char *
ngx_syslog_log_error(ngx_log_t *log, u_char *buf, size_t len)
{
    u_char             *p;
    ngx_syslog_peer_t  *peer;

    p = buf;

    ប្រសិនបើ (log->action) {
        p = ngx_snprintf(buf, len, " while %s", log->action);
        len -= p - buf;
    }

    peer = log->data;

    ប្រសិនបើ (peer) {
        p = ngx_snprintf(p, len, ", server: %V", &peer->server.name);
    }

    ត្រឡប់ p;
}
