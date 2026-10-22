
/*
 * Copyright (C) Nginx, Inc.
 */


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _NGX_SYSLOG_H_INCLUDED_
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _NGX_SYSLOG_H_INCLUDED_


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_uint_t         facility;
    ngx_uint_t         severity;
    ngx_str_t          ਟੈਗ;

    ngx_str_t         *hostname;

    ngx_addr_t         server;
    ngx_connection_t   conn;

    ngx_log_t          ਲਘੂਗਣਕ;
    ngx_log_t         *logp;

    ਬਿਨਾਨਿਸ਼ਾਨੀ           busy:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ           nohostname:1;
} ngx_syslog_peer_t;


ਅੱਖਰ *ngx_syslog_process_conf(ngx_conf_t *cf, ngx_syslog_peer_t *peer);
u_char *ngx_syslog_ਜੋੜੋ_ਸਿਰਲੇਖ(ngx_syslog_peer_t *peer, u_char *buf);
ਖਾਲੀ ngx_syslog_writer(ngx_log_t *ਲਘੂਗਣਕ, ngx_uint_t level, u_char *buf,
    ਮਾਪ_t len);
ssize_t ngx_syslog_send(ngx_syslog_peer_t *peer, u_char *buf, ਮਾਪ_t len);


#ਜੇ_ਅੰਤ /* _NGX_SYSLOG_H_INCLUDED_ */
