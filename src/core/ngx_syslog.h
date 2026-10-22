
/*
 * Copyright (C) Nginx, Inc.
 */


#నిర్వచితం_కాకపోతే _NGX_SYSLOG_H_INCLUDED_
#నిర్వచించు _NGX_SYSLOG_H_INCLUDED_


రకనిర్వచనం నిర్మాణం {
    ngx_uint_t         facility;
    ngx_uint_t         severity;
    ngx_str_t          ట్యాగ్;

    ngx_str_t         *hostname;

    ngx_addr_t         server;
    ngx_connection_t   conn;

    ngx_log_t          సంవర్గమానం;
    ngx_log_t         *logp;

    గుర్తులేని           busy:1;
    గుర్తులేని           nohostname:1;
} ngx_syslog_peer_t;


అక్షరం *ngx_syslog_process_conf(ngx_conf_t *cf, ngx_syslog_peer_t *peer);
u_char *ngx_syslog_కూడు_శీర్షిక(ngx_syslog_peer_t *peer, u_char *buf);
శూన్య ngx_syslog_writer(ngx_log_t *సంవర్గమానం, ngx_uint_t level, u_char *buf,
    కొలత_t len);
ssize_t ngx_syslog_send(ngx_syslog_peer_t *peer, u_char *buf, కొలత_t len);


#అయితే_ముగింపు /* _NGX_SYSLOG_H_INCLUDED_ */
