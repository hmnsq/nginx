
/*
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع ssize_t   (*ngx_json_اوږدتیا_pt)(ngx_معلومات_توکی_t *توکی);
تعريفنوع u_char   *(*ngx_json_encode_pt)(u_char *p, ngx_معلومات_توکی_t *توکی);


تعريفنوع بنية {
    ngx_json_اوږدتیا_pt        اوږدتیا;
    ngx_json_encode_pt        encode;
} ngx_json_encode_t;


ساكن ssize_t ngx_json_obj_اوږدتیا(ngx_معلومات_توکی_t *توکی);
ساكن ssize_t ngx_json_list_اوږدتیا(ngx_معلومات_توکی_t *توکی);
ساكن ssize_t ngx_json_string_اوږدتیا(ngx_معلومات_توکی_t *توکی);
ساكن ssize_t ngx_json_integer_اوږدتیا(ngx_معلومات_توکی_t *توکی);
ساكن ssize_t ngx_json_boolean_اوږدتیا(ngx_معلومات_توکی_t *توکی);
ساكن ssize_t ngx_json_null_اوږدتیا(ngx_معلومات_توکی_t *توکی);

ساكن u_char *ngx_json_obj_encode(u_char *p, ngx_معلومات_توکی_t *توکی);
ساكن u_char *ngx_json_list_encode(u_char *p, ngx_معلومات_توکی_t *توکی);
ساكن u_char *ngx_json_string_encode(u_char *p, ngx_معلومات_توکی_t *توکی);
ساكن u_char *ngx_json_integer_encode(u_char *p, ngx_معلومات_توکی_t *توکی);
ساكن u_char *ngx_json_boolean_encode(u_char *p, ngx_معلومات_توکی_t *توکی);
ساكن u_char *ngx_json_null_encode(u_char *p, ngx_معلومات_توکی_t *توکی);


ساكن ngx_json_encode_t  ngx_json_encode[] = {

    /* NGX_DATA_OBJECT_TYPE */
    { ngx_json_obj_اوږدتیا,
      ngx_json_obj_encode },

    /* NGX_DATA_LIST_TYPE */
    { ngx_json_list_اوږدتیا,
      ngx_json_list_encode },

    /* NGX_DATA_STRING_TYPE */
    { ngx_json_string_اوږدتیا,
      ngx_json_string_encode },

    /* NGX_DATA_INTEGER_TYPE */
    { ngx_json_integer_اوږدتیا,
      ngx_json_integer_encode },

    /* NGX_DATA_BOOLEAN_TYPE */
    { ngx_json_boolean_اوږدتیا,
      ngx_json_boolean_encode },

    /* NGX_DATA_NULL_TYPE */
    { ngx_json_null_اوږدتیا,
      ngx_json_null_encode },
};


ngx_buf_t *
ngx_json_render(ngx_pool_t *pool, ngx_معلومات_توکی_t *توکی)
{
    اندازه_t      اوږدتیا;
    ngx_buf_t  *buf;

    اوږدتیا = ngx_json_encode[توکی->type].اوږدتیا(توکی);
    إذا (اوږدتیا == (اندازه_t) NGX_ERROR) {
        أرجع عدم;
    }

    buf = ngx_create_لنډمهاله_buf(pool, اوږدتیا);
    إذا (buf == عدم) {
        أرجع عدم;
    }

    buf->وروستی = ngx_json_encode[توکی->type].encode(buf->وروستی, توکی);

    أرجع buf;
}


ساكن ssize_t
ngx_json_obj_اوږدتیا(ngx_معلومات_توکی_t *توکی)
{
    اندازه_t            len, ټول;
    ngx_معلومات_obj_t   *obj;
    ngx_معلومات_توکی_t  *i;

    obj = &توکی->معلومات.object;
    ټول = حجم("{}") - 1;
    i = obj->توکی;

    إذا (i) {
        لكل ( ;; ) {
            إذا (i->name.len > NGX_MAX_اندازه_T_ارزښت - (حجم("\"\":") - 1)) {
                أرجع NGX_ERROR;
            }

            len = حجم("\"\":") - 1 + i->name.len;

            إذا (len > NGX_MAX_اندازه_T_ارزښت - ټول) {
                أرجع NGX_ERROR;
            }

            ټول += len;

            len = ngx_escape_json(عدم, i->name.معلومات, i->name.len);

            إذا (len > NGX_MAX_اندازه_T_ارزښت - ټول) {
                أرجع NGX_ERROR;
            }

            ټول += len;

            len = ngx_json_encode[i->type].اوږدتیا(i);
            إذا (len == (اندازه_t) NGX_ERROR) {
                أرجع NGX_ERROR;
            }

            إذا (len > NGX_MAX_اندازه_T_ارزښت - ټول) {
                أرجع NGX_ERROR;
            }

            ټول += len;

            i = i->بل;

            إذا (i == عدم) {
                اكسر;
            }

            إذا (ټول > NGX_MAX_اندازه_T_ارزښت - (حجم(",") - 1)) {
                أرجع NGX_ERROR;
            }

            ټول += حجم(",") - 1;
        }
    }

    أرجع ټول;
}


ساكن u_char *
ngx_json_obj_encode(u_char *p, ngx_معلومات_توکی_t *توکی)
{
    ngx_معلومات_obj_t   *obj;
    ngx_معلومات_توکی_t  *i;

    obj = &توکی->معلومات.object;
    i = obj->توکی;

    *p++ = '{';

    إذا (i) {
        لكل ( ;; ) {
            *p++ = '"';

            p = (u_char *) ngx_escape_json(p, i->name.معلومات, i->name.len);

            *p++ = '"';
            *p++ = ':';

            p = ngx_json_encode[i->type].encode(p, i);

            i = i->بل;

            إذا (i == عدم) {
                اكسر;
            }

            *p++ = ',';
        }
    }

    *p++ = '}';

    أرجع p;
}


ساكن ssize_t
ngx_json_list_اوږدتیا(ngx_معلومات_توکی_t *توکی)
{
    اندازه_t            len, ټول;
    ngx_معلومات_obj_t   *obj;
    ngx_معلومات_توکی_t  *i;

    obj = &توکی->معلومات.object;
    ټول = حجم("[]") - 1;
    i = obj->توکی;

    إذا (i) {
        لكل ( ;; ) {
            len = ngx_json_encode[i->type].اوږدتیا(i);
            إذا (len == (اندازه_t) NGX_ERROR) {
                أرجع NGX_ERROR;
            }

            إذا (len > NGX_MAX_اندازه_T_ارزښت - ټول) {
                أرجع NGX_ERROR;
            }

            ټول += len;

            i = i->بل;

            إذا (i == عدم) {
                اكسر;
            }

            إذا (ټول > NGX_MAX_اندازه_T_ارزښت - (حجم(",") - 1)) {
                أرجع NGX_ERROR;
            }

            ټول += حجم(",") - 1;
        }
    }

    أرجع ټول;
}


ساكن u_char *
ngx_json_list_encode(u_char *p, ngx_معلومات_توکی_t *توکی)
{
    ngx_معلومات_obj_t   *obj;
    ngx_معلومات_توکی_t  *i;

    obj = &توکی->معلومات.object;
    i = obj->توکی;

    *p++ = '[';

    إذا (i) {
        لكل ( ;; ) {
            p = ngx_json_encode[i->type].encode(p, i);

            i = i->بل;

            إذا (i == عدم) {
                اكسر;
            }

            *p++ = ',';
        }
    }

    *p++ = ']';

    أرجع p;
}


ساكن ssize_t
ngx_json_string_اوږدتیا(ngx_معلومات_توکی_t *توکی)
{
    اندازه_t      len, ټول;
    ngx_str_t  *str;

    str = &توکی->معلومات.string;

    إذا (str->len > NGX_MAX_اندازه_T_ارزښت - (حجم("\"\"") - 1)) {
        أرجع NGX_ERROR;
    }

    ټول = حجم("\"\"") - 1 + str->len;

    len = ngx_escape_json(عدم, str->معلومات, str->len);

    إذا (len > NGX_MAX_اندازه_T_ارزښت - ټول) {
        أرجع NGX_ERROR;
    }

    أرجع ټول + len;
}


ساكن u_char *
ngx_json_string_encode(u_char *p, ngx_معلومات_توکی_t *توکی)
{
    ngx_str_t  *str;

    str = &توکی->معلومات.string;

    *p++ = '"';

    p = (u_char *) ngx_escape_json(p, str->معلومات, str->len);

    *p++ = '"';

    أرجع p;
}


ساكن ssize_t
ngx_json_integer_اوږدتیا(ngx_معلومات_توکی_t *توکی)
{
    أرجع NGX_INT64_LEN;
}


ساكن u_char *
ngx_json_integer_encode(u_char *p, ngx_معلومات_توکی_t *توکی)
{
    أرجع ngx_sprintf(p, "%L", توکی->معلومات.integer);
}


ساكن ssize_t
ngx_json_boolean_اوږدتیا(ngx_معلومات_توکی_t *توکی)
{
    أرجع حجم("false") - 1;
}


ساكن u_char *
ngx_json_boolean_encode(u_char *p, ngx_معلومات_توکی_t *توکی)
{
    أرجع توکی->معلومات.boolean ? ngx_cpymem(p, "true", 4)
                              : ngx_cpymem(p, "false", 5);
}


ساكن ssize_t
ngx_json_null_اوږدتیا(ngx_معلومات_توکی_t *توکی)
{
    أرجع حجم("null") - 1;
}


ساكن u_char *
ngx_json_null_encode(u_char *p, ngx_معلومات_توکی_t *توکی)
{
    أرجع ngx_cpymem(p, "null", 4);
}
