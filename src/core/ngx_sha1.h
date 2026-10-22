
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#నిర్వచితం_కాకపోతే _NGX_SHA1_H_INCLUDED_
#నిర్వచించు _NGX_SHA1_H_INCLUDED_


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


రకనిర్వచనం నిర్మాణం {
    uint64_t  బైట్లు;
    uint32_t  a, b, c, d, e, f;
    u_char    బఫరు[64];
} ngx_sha1_t;


శూన్య ngx_sha1_init(ngx_sha1_t *ctx);
శూన్య ngx_sha1_update(ngx_sha1_t *ctx, స్థిరరాశి శూన్య *దత్తాంశం, కొలత_t కొలత);
శూన్య ngx_sha1_final(u_char ఫలితం[20], ngx_sha1_t *ctx);


#అయితే_ముగింపు /* _NGX_SHA1_H_INCLUDED_ */
