
/*
 * Copyright (C) Nginx, Inc.
 */


#اگر_تعریف_نشده _NGX_SYSLOG_H_INCLUDED_
#تعریف_کن _NGX_SYSLOG_H_INCLUDED_


تعريفنوع بنية {
    ngx_uint_t         facility;
    ngx_uint_t         severity;
    ngx_str_t          برچسب;

    ngx_str_t         *hostname;

    ngx_addr_t         server;
    ngx_connection_t   conn;

    ngx_log_t          لگاریتم;
    ngx_log_t         *logp;

    بیعلامت           busy:1;
    بیعلامت           nohostname:1;
} ngx_syslog_peer_t;


حرف *ngx_syslog_process_conf(ngx_conf_t *cf, ngx_syslog_peer_t *peer);
u_char *ngx_syslog_جمع_کن_سرآیند(ngx_syslog_peer_t *peer, u_char *buf);
فراغ ngx_syslog_writer(ngx_log_t *لگاریتم, ngx_uint_t level, u_char *buf,
    اندازه_t len);
ssize_t ngx_syslog_send(ngx_syslog_peer_t *peer, u_char *buf, اندازه_t len);


#پایان_اگر /* _NGX_SYSLOG_H_INCLUDED_ */
