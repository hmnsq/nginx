
/*
 * Copyright (C) Nginx, Inc.
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


రకనిర్వచనం ssize_t   (*ngx_json_నిడివి_pt)(ngx_దత్తాంశం_అంశం_t *అంశం);
రకనిర్వచనం u_char   *(*ngx_json_encode_pt)(u_char *p, ngx_దత్తాంశం_అంశం_t *అంశం);


రకనిర్వచనం నిర్మాణం {
    ngx_json_నిడివి_pt        నిడివి;
    ngx_json_encode_pt        encode;
} ngx_json_encode_t;


స్థిర ssize_t ngx_json_obj_నిడివి(ngx_దత్తాంశం_అంశం_t *అంశం);
స్థిర ssize_t ngx_json_list_నిడివి(ngx_దత్తాంశం_అంశం_t *అంశం);
స్థిర ssize_t ngx_json_string_నిడివి(ngx_దత్తాంశం_అంశం_t *అంశం);
స్థిర ssize_t ngx_json_integer_నిడివి(ngx_దత్తాంశం_అంశం_t *అంశం);
స్థిర ssize_t ngx_json_boolean_నిడివి(ngx_దత్తాంశం_అంశం_t *అంశం);
స్థిర ssize_t ngx_json_null_నిడివి(ngx_దత్తాంశం_అంశం_t *అంశం);

స్థిర u_char *ngx_json_obj_encode(u_char *p, ngx_దత్తాంశం_అంశం_t *అంశం);
స్థిర u_char *ngx_json_list_encode(u_char *p, ngx_దత్తాంశం_అంశం_t *అంశం);
స్థిర u_char *ngx_json_string_encode(u_char *p, ngx_దత్తాంశం_అంశం_t *అంశం);
స్థిర u_char *ngx_json_integer_encode(u_char *p, ngx_దత్తాంశం_అంశం_t *అంశం);
స్థిర u_char *ngx_json_boolean_encode(u_char *p, ngx_దత్తాంశం_అంశం_t *అంశం);
స్థిర u_char *ngx_json_null_encode(u_char *p, ngx_దత్తాంశం_అంశం_t *అంశం);


స్థిర ngx_json_encode_t  ngx_json_encode[] = {

    /* NGX_DATA_OBJECT_TYPE */
    { ngx_json_obj_నిడివి,
      ngx_json_obj_encode },

    /* NGX_DATA_LIST_TYPE */
    { ngx_json_list_నిడివి,
      ngx_json_list_encode },

    /* NGX_DATA_STRING_TYPE */
    { ngx_json_string_నిడివి,
      ngx_json_string_encode },

    /* NGX_DATA_INTEGER_TYPE */
    { ngx_json_integer_నిడివి,
      ngx_json_integer_encode },

    /* NGX_DATA_BOOLEAN_TYPE */
    { ngx_json_boolean_నిడివి,
      ngx_json_boolean_encode },

    /* NGX_DATA_NULL_TYPE */
    { ngx_json_null_నిడివి,
      ngx_json_null_encode },
};


ngx_buf_t *
ngx_json_render(ngx_pool_t *pool, ngx_దత్తాంశం_అంశం_t *అంశం)
{
    కొలత_t      నిడివి;
    ngx_buf_t  *buf;

    నిడివి = ngx_json_encode[అంశం->type].నిడివి(అంశం);
    ఒకవేళ (నిడివి == (కొలత_t) NGX_ERROR) {
        తిరిగి ఖాళీ;
    }

    buf = ngx_create_తాత్కాలిక_buf(pool, నిడివి);
    ఒకవేళ (buf == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    buf->చివరి = ngx_json_encode[అంశం->type].encode(buf->చివరి, అంశం);

    తిరిగి buf;
}


స్థిర ssize_t
ngx_json_obj_నిడివి(ngx_దత్తాంశం_అంశం_t *అంశం)
{
    కొలత_t            len, మొత్తం;
    ngx_దత్తాంశం_obj_t   *obj;
    ngx_దత్తాంశం_అంశం_t  *i;

    obj = &అంశం->దత్తాంశం.object;
    మొత్తం = పరిమాణం("{}") - 1;
    i = obj->అంశం;

    ఒకవేళ (i) {
        కొరకు ( ;; ) {
            ఒకవేళ (i->name.len > NGX_MAX_కొలత_T_విలువ - (పరిమాణం("\"\":") - 1)) {
                తిరిగి NGX_ERROR;
            }

            len = పరిమాణం("\"\":") - 1 + i->name.len;

            ఒకవేళ (len > NGX_MAX_కొలత_T_విలువ - మొత్తం) {
                తిరిగి NGX_ERROR;
            }

            మొత్తం += len;

            len = ngx_escape_json(ఖాళీ, i->name.దత్తాంశం, i->name.len);

            ఒకవేళ (len > NGX_MAX_కొలత_T_విలువ - మొత్తం) {
                తిరిగి NGX_ERROR;
            }

            మొత్తం += len;

            len = ngx_json_encode[i->type].నిడివి(i);
            ఒకవేళ (len == (కొలత_t) NGX_ERROR) {
                తిరిగి NGX_ERROR;
            }

            ఒకవేళ (len > NGX_MAX_కొలత_T_విలువ - మొత్తం) {
                తిరిగి NGX_ERROR;
            }

            మొత్తం += len;

            i = i->తదుపరి;

            ఒకవేళ (i == ఖాళీ) {
                ఆపు;
            }

            ఒకవేళ (మొత్తం > NGX_MAX_కొలత_T_విలువ - (పరిమాణం(",") - 1)) {
                తిరిగి NGX_ERROR;
            }

            మొత్తం += పరిమాణం(",") - 1;
        }
    }

    తిరిగి మొత్తం;
}


స్థిర u_char *
ngx_json_obj_encode(u_char *p, ngx_దత్తాంశం_అంశం_t *అంశం)
{
    ngx_దత్తాంశం_obj_t   *obj;
    ngx_దత్తాంశం_అంశం_t  *i;

    obj = &అంశం->దత్తాంశం.object;
    i = obj->అంశం;

    *p++ = '{';

    ఒకవేళ (i) {
        కొరకు ( ;; ) {
            *p++ = '"';

            p = (u_char *) ngx_escape_json(p, i->name.దత్తాంశం, i->name.len);

            *p++ = '"';
            *p++ = ':';

            p = ngx_json_encode[i->type].encode(p, i);

            i = i->తదుపరి;

            ఒకవేళ (i == ఖాళీ) {
                ఆపు;
            }

            *p++ = ',';
        }
    }

    *p++ = '}';

    తిరిగి p;
}


స్థిర ssize_t
ngx_json_list_నిడివి(ngx_దత్తాంశం_అంశం_t *అంశం)
{
    కొలత_t            len, మొత్తం;
    ngx_దత్తాంశం_obj_t   *obj;
    ngx_దత్తాంశం_అంశం_t  *i;

    obj = &అంశం->దత్తాంశం.object;
    మొత్తం = పరిమాణం("[]") - 1;
    i = obj->అంశం;

    ఒకవేళ (i) {
        కొరకు ( ;; ) {
            len = ngx_json_encode[i->type].నిడివి(i);
            ఒకవేళ (len == (కొలత_t) NGX_ERROR) {
                తిరిగి NGX_ERROR;
            }

            ఒకవేళ (len > NGX_MAX_కొలత_T_విలువ - మొత్తం) {
                తిరిగి NGX_ERROR;
            }

            మొత్తం += len;

            i = i->తదుపరి;

            ఒకవేళ (i == ఖాళీ) {
                ఆపు;
            }

            ఒకవేళ (మొత్తం > NGX_MAX_కొలత_T_విలువ - (పరిమాణం(",") - 1)) {
                తిరిగి NGX_ERROR;
            }

            మొత్తం += పరిమాణం(",") - 1;
        }
    }

    తిరిగి మొత్తం;
}


స్థిర u_char *
ngx_json_list_encode(u_char *p, ngx_దత్తాంశం_అంశం_t *అంశం)
{
    ngx_దత్తాంశం_obj_t   *obj;
    ngx_దత్తాంశం_అంశం_t  *i;

    obj = &అంశం->దత్తాంశం.object;
    i = obj->అంశం;

    *p++ = '[';

    ఒకవేళ (i) {
        కొరకు ( ;; ) {
            p = ngx_json_encode[i->type].encode(p, i);

            i = i->తదుపరి;

            ఒకవేళ (i == ఖాళీ) {
                ఆపు;
            }

            *p++ = ',';
        }
    }

    *p++ = ']';

    తిరిగి p;
}


స్థిర ssize_t
ngx_json_string_నిడివి(ngx_దత్తాంశం_అంశం_t *అంశం)
{
    కొలత_t      len, మొత్తం;
    ngx_str_t  *str;

    str = &అంశం->దత్తాంశం.string;

    ఒకవేళ (str->len > NGX_MAX_కొలత_T_విలువ - (పరిమాణం("\"\"") - 1)) {
        తిరిగి NGX_ERROR;
    }

    మొత్తం = పరిమాణం("\"\"") - 1 + str->len;

    len = ngx_escape_json(ఖాళీ, str->దత్తాంశం, str->len);

    ఒకవేళ (len > NGX_MAX_కొలత_T_విలువ - మొత్తం) {
        తిరిగి NGX_ERROR;
    }

    తిరిగి మొత్తం + len;
}


స్థిర u_char *
ngx_json_string_encode(u_char *p, ngx_దత్తాంశం_అంశం_t *అంశం)
{
    ngx_str_t  *str;

    str = &అంశం->దత్తాంశం.string;

    *p++ = '"';

    p = (u_char *) ngx_escape_json(p, str->దత్తాంశం, str->len);

    *p++ = '"';

    తిరిగి p;
}


స్థిర ssize_t
ngx_json_integer_నిడివి(ngx_దత్తాంశం_అంశం_t *అంశం)
{
    తిరిగి NGX_INT64_LEN;
}


స్థిర u_char *
ngx_json_integer_encode(u_char *p, ngx_దత్తాంశం_అంశం_t *అంశం)
{
    తిరిగి ngx_sprintf(p, "%L", అంశం->దత్తాంశం.integer);
}


స్థిర ssize_t
ngx_json_boolean_నిడివి(ngx_దత్తాంశం_అంశం_t *అంశం)
{
    తిరిగి పరిమాణం("false") - 1;
}


స్థిర u_char *
ngx_json_boolean_encode(u_char *p, ngx_దత్తాంశం_అంశం_t *అంశం)
{
    తిరిగి అంశం->దత్తాంశం.boolean ? ngx_cpymem(p, "true", 4)
                              : ngx_cpymem(p, "false", 5);
}


స్థిర ssize_t
ngx_json_null_నిడివి(ngx_దత్తాంశం_అంశం_t *అంశం)
{
    తిరిగి పరిమాణం("null") - 1;
}


స్థిర u_char *
ngx_json_null_encode(u_char *p, ngx_దత్తాంశం_అంశం_t *అంశం)
{
    తిరిగి ngx_cpymem(p, "null", 4);
}
