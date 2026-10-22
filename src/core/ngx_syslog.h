
/*
 * Copyright (C) Nginx, Inc.
 */


#که_نه_تعریف_شوی _NGX_SYSLOG_H_INCLUDED_
#تعریف_کړه _NGX_SYSLOG_H_INCLUDED_


تعريفنوع بنية {
    ngx_uint_t         facility;
    ngx_uint_t         severity;
    ngx_str_t          نښان;

    ngx_str_t         *hostname;

    ngx_addr_t         server;
    ngx_connection_t   conn;

    ngx_log_t          لوګاریتم;
    ngx_log_t         *logp;

    بینښه           busy:1;
    بینښه           nohostname:1;
} ngx_syslog_peer_t;


حرف *ngx_syslog_process_conf(ngx_conf_t *cf, ngx_syslog_peer_t *peer);
u_char *ngx_syslog_جمع_کړه_سرلیک(ngx_syslog_peer_t *peer, u_char *buf);
فراغ ngx_syslog_writer(ngx_log_t *لوګاریتم, ngx_uint_t level, u_char *buf,
    اندازه_t len);
ssize_t ngx_syslog_send(ngx_syslog_peer_t *peer, u_char *buf, اندازه_t len);


#د_که_پای /* _NGX_SYSLOG_H_INCLUDED_ */
