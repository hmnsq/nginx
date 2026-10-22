
/*
 * Copyright (C) Nginx, Inc.
 */


#यदि_अपरिभाषितम् _NGX_SYSLOG_H_INCLUDED_
#परिभाषय _NGX_SYSLOG_H_INCLUDED_


प्रकारनिर्देश संरचना {
    ngx_uint_t         facility;
    ngx_uint_t         severity;
    ngx_str_t          चिह्नकम्;

    ngx_str_t         *hostname;

    ngx_addr_t         server;
    ngx_connection_t   conn;

    ngx_log_t          लघुगणकः;
    ngx_log_t         *logp;

    अचिह्नित           busy:1;
    अचिह्नित           nohostname:1;
} ngx_syslog_peer_t;


वर्ण *ngx_syslog_process_conf(ngx_conf_t *cf, ngx_syslog_peer_t *peer);
u_char *ngx_syslog_सङ्कलय_शीर्षकम्(ngx_syslog_peer_t *peer, u_char *buf);
शून्य ngx_syslog_writer(ngx_log_t *लघुगणकः, ngx_uint_t level, u_char *buf,
    आकार_t len);
ssize_t ngx_syslog_send(ngx_syslog_peer_t *peer, u_char *buf, आकार_t len);


#यदि_अन्तः /* _NGX_SYSLOG_H_INCLUDED_ */
