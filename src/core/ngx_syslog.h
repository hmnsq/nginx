
/*
 * Copyright (C) Nginx, Inc.
 */


#جے_بنا_تعریف _NGX_SYSLOG_H_INCLUDED_
#تعریف_کرو _NGX_SYSLOG_H_INCLUDED_


تعريفنوع بنية {
    ngx_uint_t         facility;
    ngx_uint_t         severity;
    ngx_str_t          ٹیگ;

    ngx_str_t         *hostname;

    ngx_addr_t         server;
    ngx_connection_t   conn;

    ngx_log_t          لوگارتھم;
    ngx_log_t         *logp;

    بنانشانی           busy:1;
    بنانشانی           nohostname:1;
} ngx_syslog_peer_t;


حرف *ngx_syslog_process_conf(ngx_conf_t *cf, ngx_syslog_peer_t *peer);
u_char *ngx_syslog_جوڑو_سرخی(ngx_syslog_peer_t *peer, u_char *buf);
فراغ ngx_syslog_writer(ngx_log_t *لوگارتھم, ngx_uint_t level, u_char *buf,
    ناپ_t len);
ssize_t ngx_syslog_send(ngx_syslog_peer_t *peer, u_char *buf, ناپ_t len);


#جے_انت /* _NGX_SYSLOG_H_INCLUDED_ */
