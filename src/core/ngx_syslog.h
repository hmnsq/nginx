
/*
 * Copyright (C) Nginx, Inc.
 */


#अगर_अघोषित _NGX_SYSLOG_H_INCLUDED_
#घोषणा _NGX_SYSLOG_H_INCLUDED_


प्रकार काष्ठा {
    ngx_uint_t         facility;
    ngx_uint_t         severity;
    ngx_str_t          tag;

    ngx_str_t         *hostname;

    ngx_addr_t         server;
    ngx_connection_t   conn;

    ngx_log_t          log;
    ngx_log_t         *logp;

    अचिन्हित           busy:1;
    अचिन्हित           nohostname:1;
} ngx_syslog_peer_t;


अक्षर *ngx_syslog_process_conf(ngx_conf_t *cf, ngx_syslog_peer_t *peer);
u_char *ngx_syslog_add_header(ngx_syslog_peer_t *peer, u_char *buf);
व्योम ngx_syslog_writer(ngx_log_t *log, ngx_uint_t level, u_char *buf,
    माप_प्रकार len);
ssize_t ngx_syslog_send(ngx_syslog_peer_t *peer, u_char *buf, माप_प्रकार len);


#पूर्ण_अगर /* _NGX_SYSLOG_H_INCLUDED_ */
