
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#నిర్వచితం_కాకపోతే NGX_చక్రీయ_తనిఖీ_32_H_INCLUDED
#నిర్వచించు NGX_చక్రీయ_తనిఖీ_32_H_INCLUDED


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


బాహ్య uint32_t  *ngx_చక్రీయ_తనిఖీ_32_పట్టిక_short;
బాహ్య uint32_t   ngx_చక్రీయ_తనిఖీ_32_table256[];
బాహ్య uint32_t   ngx_crc32c_table256[];


స్థిర ngx_inline uint32_t
ngx_చక్రీయ_తనిఖీ_32_short(u_char *p, కొలత_t len)
{
    u_char    c;
    uint32_t  చక్రీయ_తనిఖీ;

    చక్రీయ_తనిఖీ = 0xffffffff;

    అయితే (len--) {
        c = *p++;
        చక్రీయ_తనిఖీ = ngx_చక్రీయ_తనిఖీ_32_పట్టిక_short[(చక్రీయ_తనిఖీ ^ (c & 0xf)) & 0xf] ^ (చక్రీయ_తనిఖీ >> 4);
        చక్రీయ_తనిఖీ = ngx_చక్రీయ_తనిఖీ_32_పట్టిక_short[(చక్రీయ_తనిఖీ ^ (c >> 4)) & 0xf] ^ (చక్రీయ_తనిఖీ >> 4);
    }

    తిరిగి చక్రీయ_తనిఖీ ^ 0xffffffff;
}


స్థిర ngx_inline uint32_t
ngx_చక్రీయ_తనిఖీ_32_long(u_char *p, కొలత_t len)
{
    uint32_t  చక్రీయ_తనిఖీ;

    చక్రీయ_తనిఖీ = 0xffffffff;

    అయితే (len--) {
        చక్రీయ_తనిఖీ = ngx_చక్రీయ_తనిఖీ_32_table256[(చక్రీయ_తనిఖీ ^ *p++) & 0xff] ^ (చక్రీయ_తనిఖీ >> 8);
    }

    తిరిగి చక్రీయ_తనిఖీ ^ 0xffffffff;
}


#నిర్వచించు ngx_చక్రీయ_తనిఖీ_32_init(చక్రీయ_తనిఖీ)                                                   \
    చక్రీయ_తనిఖీ = 0xffffffff


స్థిర ngx_inline శూన్య
ngx_చక్రీయ_తనిఖీ_32_update(uint32_t *చక్రీయ_తనిఖీ, u_char *p, కొలత_t len)
{
    uint32_t  c;

    c = *చక్రీయ_తనిఖీ;

    అయితే (len--) {
        c = ngx_చక్రీయ_తనిఖీ_32_table256[(c ^ *p++) & 0xff] ^ (c >> 8);
    }

    *చక్రీయ_తనిఖీ = c;
}


#నిర్వచించు ngx_చక్రీయ_తనిఖీ_32_final(చక్రీయ_తనిఖీ)                                                  \
    చక్రీయ_తనిఖీ ^= 0xffffffff


స్థిర ngx_inline uint32_t
ngx_crc32c_long(u_char *p, కొలత_t len)
{
    uint32_t  చక్రీయ_తనిఖీ;

    చక్రీయ_తనిఖీ = 0xffffffff;

    అయితే (len--) {
        చక్రీయ_తనిఖీ = ngx_crc32c_table256[(చక్రీయ_తనిఖీ ^ *p++) & 0xff] ^ (చక్రీయ_తనిఖీ >> 8);
    }

    తిరిగి చక్రీయ_తనిఖీ ^ 0xffffffff;
}


#నిర్వచించు ngx_crc32c_init(చక్రీయ_తనిఖీ)                                                  \
    చక్రీయ_తనిఖీ = 0xffffffff


స్థిర ngx_inline శూన్య
ngx_crc32c_update(uint32_t *చక్రీయ_తనిఖీ, u_char *p, కొలత_t len)
{
    uint32_t  c;

    c = *చక్రీయ_తనిఖీ;

    అయితే (len--) {
        c = ngx_crc32c_table256[(c ^ *p++) & 0xff] ^ (c >> 8);
    }

    *చక్రీయ_తనిఖీ = c;
}


#నిర్వచించు ngx_crc32c_final(చక్రీయ_తనిఖీ)                                                 \
    చక్రీయ_తనిఖీ ^= 0xffffffff


ngx_int_t ngx_చక్రీయ_తనిఖీ_32_పట్టిక_init(శూన్య);


#అయితే_ముగింపు /* _NGX_CRC32_H_INCLUDED_ */
