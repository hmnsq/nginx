
/*
 * Copyright (C) Nginx, Inc.
 */


#اگر_غیر_تعریف_شدہ _NGX_SYSLOG_H_INCLUDED_
#تعریف_کرو _NGX_SYSLOG_H_INCLUDED_


قسمتعریف ساخت {
    ngx_uint_t         facility;
    ngx_uint_t         severity;
    ngx_str_t          لیبل;

    ngx_str_t         *hostname;

    ngx_addr_t         server;
    ngx_connection_t   conn;

    ngx_log_t          لوگارتھم;
    ngx_log_t         *logp;

    غیرعلامتی           busy:1;
    غیرعلامتی           nohostname:1;
} ngx_syslog_peer_t;


حرف *ngx_syslog_process_conf(ngx_conf_t *cf, ngx_syslog_peer_t *peer);
u_char *ngx_syslog_جوڑو_سرخی(ngx_syslog_peer_t *peer, u_char *buf);
خالی ngx_syslog_writer(ngx_log_t *لوگارتھم, ngx_uint_t level, u_char *buf,
    سائز_t len);
ssize_t ngx_syslog_send(ngx_syslog_peer_t *peer, u_char *buf, سائز_t len);


#اگر_ختم /* _NGX_SYSLOG_H_INCLUDED_ */
