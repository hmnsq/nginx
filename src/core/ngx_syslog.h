
/*
 * Copyright (C) Nginx, Inc.
 */


#جيڪڏهن_وصف_نه_ٿيل _NGX_SYSLOG_H_INCLUDED_
#وصف_ڪريو _NGX_SYSLOG_H_INCLUDED_


تعريفنوع بنية {
    ngx_uint_t         facility;
    ngx_uint_t         severity;
    ngx_str_t          ٽيگ;

    ngx_str_t         *hostname;

    ngx_addr_t         server;
    ngx_connection_t   conn;

    ngx_log_t          لاگرٿم;
    ngx_log_t         *logp;

    بنانشان           busy:1;
    بنانشان           nohostname:1;
} ngx_syslog_peer_t;


حرف *ngx_syslog_process_conf(ngx_conf_t *cf, ngx_syslog_peer_t *peer);
u_char *ngx_syslog_جوڙيو_سرو(ngx_syslog_peer_t *peer, u_char *buf);
فراغ ngx_syslog_writer(ngx_log_t *لاگرٿم, ngx_uint_t level, u_char *buf,
    سائيز_t len);
ssize_t ngx_syslog_send(ngx_syslog_peer_t *peer, u_char *buf, سائيز_t len);


#جيڪڏهن_پورو /* _NGX_SYSLOG_H_INCLUDED_ */
