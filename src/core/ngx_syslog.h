
/*
 * Copyright (C) Nginx, Inc.
 */


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _NGX_SYSLOG_H_INCLUDED_
#ವ್ಯಾಖ್ಯಾನಿಸು _NGX_SYSLOG_H_INCLUDED_


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_uint_t         facility;
    ngx_uint_t         severity;
    ngx_str_t          ಟ್ಯಾಗ್;

    ngx_str_t         *hostname;

    ngx_addr_t         server;
    ngx_connection_t   conn;

    ngx_log_t          ಲಘುಗಣಕ;
    ngx_log_t         *logp;

    ಚಿಹ್ನೆರಹಿತ           busy:1;
    ಚಿಹ್ನೆರಹಿತ           nohostname:1;
} ngx_syslog_peer_t;


ಅಕ್ಷರ *ngx_syslog_process_conf(ngx_conf_t *cf, ngx_syslog_peer_t *peer);
u_char *ngx_syslog_ಕೂಡಿಸು_ಶೀರ್ಷಿಕೆ(ngx_syslog_peer_t *peer, u_char *buf);
ಶೂನ್ಯ ngx_syslog_writer(ngx_log_t *ಲಘುಗಣಕ, ngx_uint_t level, u_char *buf,
    ಗಾತ್ರ_t len);
ssize_t ngx_syslog_send(ngx_syslog_peer_t *peer, u_char *buf, ಗಾತ್ರ_t len);


#ಆದರೆ_ಅಂತ್ಯ /* _NGX_SYSLOG_H_INCLUDED_ */
