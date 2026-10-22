
/*
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_event.h>


#define NGX_SYSLOG_MAX_STR                                                    \
    NGX_MAX_ERROR_STR + sizeof("<255>Jan 01 00:00:00 ") - 1                   \
    + (NGX_MAXHOSTNAMELEN - 1) + 1 /* space */                                \
    + 32 /* tag */ + 2 /* colon, space */


static char *ngx_syslog_parse_args(ngx_conf_t *cf, ngx_syslog_peer_t *peer);
static ngx_int_t ngx_syslog_init_peer(ngx_syslog_peer_t *peer);
static void ngx_syslog_cleanup(void *data);
static u_char *ngx_syslog_log_error(ngx_log_t *log, u_char *buf, size_t len);


static char  *facilities[] = {
    "kern", "user", "mail", "daemon", "auth", "intern", "lpr", "news", "uucp",
    "clock", "authpriv", "ftp", "ntp", "audit", "alert", "cron", "local0",
    "local1", "local2", "local3", "local4", "local5", "local6", "local7",
    NULL
};

/* note 'error/warn' like in nginx.conf, not 'err/warning' */
static char  *severities[] = {
    "emerg", "alert", "crit", "error", "warn", "notice", "info", "debug", NULL
};

static ngx_log_t    ngx_syslog_dummy_log;
static ngx_event_t  ngx_syslog_dummy_event;


char *
ngx_syslog_process_conf(ngx_conf_t *cf, ngx_syslog_peer_t *peer)
{
    ngx_pool_cleanup_t  *cln;

    peer->facility = NGX_CONF_UNSET_UINT;
    peer->severity = NGX_CONF_UNSET_UINT;

    如果 (ngx_syslog_parse_args(cf, peer) != NGX_CONF_OK) {
        返回 NGX_CONF_ERROR;
    }

    如果 (peer->server.sockaddr == NULL) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "no syslog server specified");
        返回 NGX_CONF_ERROR;
    }

    如果 (peer->facility == NGX_CONF_UNSET_UINT) {
        peer->facility = 23; /* local7 */
    }

    如果 (peer->severity == NGX_CONF_UNSET_UINT) {
        peer->severity = 6; /* info */
    }

    如果 (peer->tag.data == NULL) {
        ngx_str_set(&peer->tag, "nginx");
    }

    peer->hostname = &cf->cycle->hostname;
    peer->logp = &cf->cycle->new_log;

    peer->conn.fd = (ngx_socket_t) -1;

    peer->conn.read = &ngx_syslog_dummy_event;
    peer->conn.write = &ngx_syslog_dummy_event;

    ngx_syslog_dummy_event.log = &ngx_syslog_dummy_log;

    cln = ngx_pool_cleanup_add(cf->pool, 0);
    如果 (cln == NULL) {
        返回 NGX_CONF_ERROR;
    }

    cln->data = peer;
    cln->handler = ngx_syslog_cleanup;

    返回 NGX_CONF_OK;
}


static char *
ngx_syslog_parse_args(ngx_conf_t *cf, ngx_syslog_peer_t *peer)
{
    u_char      *p, *comma, c;
    size_t       len;
    ngx_str_t   *value;
    ngx_url_t    u;
    ngx_uint_t   i;

    value = cf->args->elts;

    p = value[1].data + sizeof("syslog:") - 1;

    为 ( ;; ) {
        comma = (u_char *) ngx_strchr(p, ',');

        如果 (comma != NULL) {
            len = comma - p;
            *comma = '\0';

        } 否则 {
            len = value[1].data + value[1].len - p;
        }

        如果 (ngx_strncmp(p, "server=", 7) == 0) {

            如果 (peer->server.sockaddr != NULL) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"server\"");
                返回 NGX_CONF_ERROR;
            }

            ngx_memzero(&u, sizeof(ngx_url_t));

            u.url.data = p + 7;
            u.url.len = len - 7;
            u.default_port = 514;

            如果 (ngx_parse_url(cf->pool, &u) != NGX_OK) {
                如果 (u.err) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "%s in syslog server \"%V\"",
                                       u.err, &u.url);
                }

                返回 NGX_CONF_ERROR;
            }

            peer->server = u.addrs[0];

        } 否则 如果 (ngx_strncmp(p, "facility=", 9) == 0) {

            如果 (peer->facility != NGX_CONF_UNSET_UINT) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"facility\"");
                返回 NGX_CONF_ERROR;
            }

            为 (i = 0; facilities[i] != NULL; i++) {

                如果 (ngx_strcmp(p + 9, facilities[i]) == 0) {
                    peer->facility = i;
                    goto next;
                }
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog facility \"%s\"", p + 9);
            返回 NGX_CONF_ERROR;

        } 否则 如果 (ngx_strncmp(p, "severity=", 9) == 0) {

            如果 (peer->severity != NGX_CONF_UNSET_UINT) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"severity\"");
                返回 NGX_CONF_ERROR;
            }

            为 (i = 0; severities[i] != NULL; i++) {

                如果 (ngx_strcmp(p + 9, severities[i]) == 0) {
                    peer->severity = i;
                    goto next;
                }
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog severity \"%s\"", p + 9);
            返回 NGX_CONF_ERROR;

        } 否则 如果 (ngx_strncmp(p, "tag=", 4) == 0) {

            如果 (peer->tag.data != NULL) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"tag\"");
                返回 NGX_CONF_ERROR;
            }

            /*
             * RFC 3164: the TAG is a string of ABNF alphanumeric characters
             * that MUST NOT exceed 32 characters.
             */
            如果 (len - 4 > 32) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "syslog tag length exceeds 32");
                返回 NGX_CONF_ERROR;
            }

            为 (i = 4; i < len; i++) {
                c = ngx_tolower(p[i]);

                如果 (c < '0' || (c > '9' && c < 'a' && c != '_') || c > 'z') {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "syslog \"tag\" only allows "
                                       "alphanumeric characters "
                                       "and underscore");
                    返回 NGX_CONF_ERROR;
                }
            }

            peer->tag.data = p + 4;
            peer->tag.len = len - 4;

        } 否则 如果 (len == 10 && ngx_strncmp(p, "nohostname", 10) == 0) {
            peer->nohostname = 1;

        } 否则 {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog parameter \"%s\"", p);
            返回 NGX_CONF_ERROR;
        }

    next:

        如果 (comma == NULL) {
            break;
        }

        p = comma + 1;
    }

    返回 NGX_CONF_OK;
}


u_char *
ngx_syslog_add_header(ngx_syslog_peer_t *peer, u_char *buf)
{
    ngx_uint_t  pri;

    pri = peer->facility * 8 + peer->severity;

    如果 (peer->nohostname) {
        返回 ngx_sprintf(buf, "<%ui>%V %V: ", pri, &ngx_cached_syslog_time,
                           &peer->tag);
    }

    返回 ngx_sprintf(buf, "<%ui>%V %V %V: ", pri, &ngx_cached_syslog_time,
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

    如果 (peer->busy) {
        返回;
    }

    peer->busy = 1;
    peer->severity = level - 1;

    p = ngx_syslog_add_header(peer, msg);
    head_len = p - msg;

    len -= NGX_LINEFEED_SIZE;

    如果 (len > NGX_SYSLOG_MAX_STR - head_len) {
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

    如果 (peer->log.handler == NULL) {
        peer->log = *peer->logp;
        peer->log.handler = ngx_syslog_log_error;
        peer->log.data = peer;
        peer->log.action = "logging to syslog";
    }

    如果 (peer->conn.fd == (ngx_socket_t) -1) {
        如果 (ngx_syslog_init_peer(peer) != NGX_OK) {
            返回 NGX_ERROR;
        }
    }

    如果 (ngx_send) {
        n = ngx_send(&peer->conn, buf, len);

    } 否则 {
        /* event module has not yet set ngx_io */
        n = ngx_os_io.send(&peer->conn, buf, len);
    }

    如果 (n == NGX_ERROR) {

        如果 (ngx_close_socket(peer->conn.fd) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        peer->conn.fd = (ngx_socket_t) -1;
    }

    返回 n;
}


static ngx_int_t
ngx_syslog_init_peer(ngx_syslog_peer_t *peer)
{
    ngx_socket_t  fd;

    fd = ngx_socket(peer->server.sockaddr->sa_family, SOCK_DGRAM, 0);
    如果 (fd == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_socket_n " failed");
        返回 NGX_ERROR;
    }

    如果 (ngx_nonblocking(fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_nonblocking_n " failed");
        goto failed;
    }

    如果 (connect(fd, peer->server.sockaddr, peer->server.socklen) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      "connect() failed");
        goto failed;
    }

    peer->conn.fd = fd;
    peer->conn.log = &peer->log;

    /* UDP sockets are always ready to write */
    peer->conn.write->ready = 1;

    返回 NGX_OK;

failed:

    如果 (ngx_close_socket(fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->log, ngx_socket_errno,
                      ngx_close_socket_n " failed");
    }

    返回 NGX_ERROR;
}


static void
ngx_syslog_cleanup(void *data)
{
    ngx_syslog_peer_t  *peer = data;

    /* prevents further use of this peer */
    peer->busy = 1;

    如果 (peer->conn.fd == (ngx_socket_t) -1) {
        返回;
    }

    如果 (ngx_close_socket(peer->conn.fd) == -1) {
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

    如果 (log->action) {
        p = ngx_snprintf(buf, len, " while %s", log->action);
        len -= p - buf;
    }

    peer = log->data;

    如果 (peer) {
        p = ngx_snprintf(p, len, ", server: %V", &peer->server.name);
    }

    返回 p;
}
