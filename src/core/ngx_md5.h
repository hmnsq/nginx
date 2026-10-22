
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#నిర్వచితం_కాకపోతే _NGX_MD5_H_INCLUDED_
#నిర్వచించు _NGX_MD5_H_INCLUDED_


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


రకనిర్వచనం నిర్మాణం {
    uint64_t  బైట్లు;
    uint32_t  a, b, c, d;
    u_char    బఫరు[64];
} ngx_md5_t;


శూన్య ngx_md5_init(ngx_md5_t *ctx);
శూన్య ngx_md5_update(ngx_md5_t *ctx, స్థిరరాశి శూన్య *దత్తాంశం, కొలత_t కొలత);
శూన్య ngx_md5_final(u_char ఫలితం[16], ngx_md5_t *ctx);


#అయితే_ముగింపు /* _NGX_MD5_H_INCLUDED_ */
