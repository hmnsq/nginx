
/*
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع ssize_t   (*ngx_json_ڊيگهائي_pt)(ngx_معلومات_شي_t *شي);
تعريفنوع u_char   *(*ngx_json_encode_pt)(u_char *p, ngx_معلومات_شي_t *شي);


تعريفنوع بنية {
    ngx_json_ڊيگهائي_pt        ڊيگهائي;
    ngx_json_encode_pt        encode;
} ngx_json_encode_t;


ساكن ssize_t ngx_json_obj_ڊيگهائي(ngx_معلومات_شي_t *شي);
ساكن ssize_t ngx_json_list_ڊيگهائي(ngx_معلومات_شي_t *شي);
ساكن ssize_t ngx_json_string_ڊيگهائي(ngx_معلومات_شي_t *شي);
ساكن ssize_t ngx_json_integer_ڊيگهائي(ngx_معلومات_شي_t *شي);
ساكن ssize_t ngx_json_boolean_ڊيگهائي(ngx_معلومات_شي_t *شي);
ساكن ssize_t ngx_json_null_ڊيگهائي(ngx_معلومات_شي_t *شي);

ساكن u_char *ngx_json_obj_encode(u_char *p, ngx_معلومات_شي_t *شي);
ساكن u_char *ngx_json_list_encode(u_char *p, ngx_معلومات_شي_t *شي);
ساكن u_char *ngx_json_string_encode(u_char *p, ngx_معلومات_شي_t *شي);
ساكن u_char *ngx_json_integer_encode(u_char *p, ngx_معلومات_شي_t *شي);
ساكن u_char *ngx_json_boolean_encode(u_char *p, ngx_معلومات_شي_t *شي);
ساكن u_char *ngx_json_null_encode(u_char *p, ngx_معلومات_شي_t *شي);


ساكن ngx_json_encode_t  ngx_json_encode[] = {

    /* NGX_DATA_OBJECT_TYPE */
    { ngx_json_obj_ڊيگهائي,
      ngx_json_obj_encode },

    /* NGX_DATA_LIST_TYPE */
    { ngx_json_list_ڊيگهائي,
      ngx_json_list_encode },

    /* NGX_DATA_STRING_TYPE */
    { ngx_json_string_ڊيگهائي,
      ngx_json_string_encode },

    /* NGX_DATA_INTEGER_TYPE */
    { ngx_json_integer_ڊيگهائي,
      ngx_json_integer_encode },

    /* NGX_DATA_BOOLEAN_TYPE */
    { ngx_json_boolean_ڊيگهائي,
      ngx_json_boolean_encode },

    /* NGX_DATA_NULL_TYPE */
    { ngx_json_null_ڊيگهائي,
      ngx_json_null_encode },
};


ngx_buf_t *
ngx_json_render(ngx_pool_t *pool, ngx_معلومات_شي_t *شي)
{
    سائيز_t      ڊيگهائي;
    ngx_buf_t  *buf;

    ڊيگهائي = ngx_json_encode[شي->type].ڊيگهائي(شي);
    إذا (ڊيگهائي == (سائيز_t) NGX_ERROR) {
        أرجع عدم;
    }

    buf = ngx_create_عارضي_buf(pool, ڊيگهائي);
    إذا (buf == عدم) {
        أرجع عدم;
    }

    buf->آخري = ngx_json_encode[شي->type].encode(buf->آخري, شي);

    أرجع buf;
}


ساكن ssize_t
ngx_json_obj_ڊيگهائي(ngx_معلومات_شي_t *شي)
{
    سائيز_t            len, ڪل;
    ngx_معلومات_obj_t   *obj;
    ngx_معلومات_شي_t  *i;

    obj = &شي->معلومات.object;
    ڪل = حجم("{}") - 1;
    i = obj->شي;

    إذا (i) {
        لكل ( ;; ) {
            إذا (i->name.len > NGX_MAX_سائيز_T_قدر - (حجم("\"\":") - 1)) {
                أرجع NGX_ERROR;
            }

            len = حجم("\"\":") - 1 + i->name.len;

            إذا (len > NGX_MAX_سائيز_T_قدر - ڪل) {
                أرجع NGX_ERROR;
            }

            ڪل += len;

            len = ngx_escape_json(عدم, i->name.معلومات, i->name.len);

            إذا (len > NGX_MAX_سائيز_T_قدر - ڪل) {
                أرجع NGX_ERROR;
            }

            ڪل += len;

            len = ngx_json_encode[i->type].ڊيگهائي(i);
            إذا (len == (سائيز_t) NGX_ERROR) {
                أرجع NGX_ERROR;
            }

            إذا (len > NGX_MAX_سائيز_T_قدر - ڪل) {
                أرجع NGX_ERROR;
            }

            ڪل += len;

            i = i->ايندڙ;

            إذا (i == عدم) {
                اكسر;
            }

            إذا (ڪل > NGX_MAX_سائيز_T_قدر - (حجم(",") - 1)) {
                أرجع NGX_ERROR;
            }

            ڪل += حجم(",") - 1;
        }
    }

    أرجع ڪل;
}


ساكن u_char *
ngx_json_obj_encode(u_char *p, ngx_معلومات_شي_t *شي)
{
    ngx_معلومات_obj_t   *obj;
    ngx_معلومات_شي_t  *i;

    obj = &شي->معلومات.object;
    i = obj->شي;

    *p++ = '{';

    إذا (i) {
        لكل ( ;; ) {
            *p++ = '"';

            p = (u_char *) ngx_escape_json(p, i->name.معلومات, i->name.len);

            *p++ = '"';
            *p++ = ':';

            p = ngx_json_encode[i->type].encode(p, i);

            i = i->ايندڙ;

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
ngx_json_list_ڊيگهائي(ngx_معلومات_شي_t *شي)
{
    سائيز_t            len, ڪل;
    ngx_معلومات_obj_t   *obj;
    ngx_معلومات_شي_t  *i;

    obj = &شي->معلومات.object;
    ڪل = حجم("[]") - 1;
    i = obj->شي;

    إذا (i) {
        لكل ( ;; ) {
            len = ngx_json_encode[i->type].ڊيگهائي(i);
            إذا (len == (سائيز_t) NGX_ERROR) {
                أرجع NGX_ERROR;
            }

            إذا (len > NGX_MAX_سائيز_T_قدر - ڪل) {
                أرجع NGX_ERROR;
            }

            ڪل += len;

            i = i->ايندڙ;

            إذا (i == عدم) {
                اكسر;
            }

            إذا (ڪل > NGX_MAX_سائيز_T_قدر - (حجم(",") - 1)) {
                أرجع NGX_ERROR;
            }

            ڪل += حجم(",") - 1;
        }
    }

    أرجع ڪل;
}


ساكن u_char *
ngx_json_list_encode(u_char *p, ngx_معلومات_شي_t *شي)
{
    ngx_معلومات_obj_t   *obj;
    ngx_معلومات_شي_t  *i;

    obj = &شي->معلومات.object;
    i = obj->شي;

    *p++ = '[';

    إذا (i) {
        لكل ( ;; ) {
            p = ngx_json_encode[i->type].encode(p, i);

            i = i->ايندڙ;

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
ngx_json_string_ڊيگهائي(ngx_معلومات_شي_t *شي)
{
    سائيز_t      len, ڪل;
    ngx_str_t  *str;

    str = &شي->معلومات.string;

    إذا (str->len > NGX_MAX_سائيز_T_قدر - (حجم("\"\"") - 1)) {
        أرجع NGX_ERROR;
    }

    ڪل = حجم("\"\"") - 1 + str->len;

    len = ngx_escape_json(عدم, str->معلومات, str->len);

    إذا (len > NGX_MAX_سائيز_T_قدر - ڪل) {
        أرجع NGX_ERROR;
    }

    أرجع ڪل + len;
}


ساكن u_char *
ngx_json_string_encode(u_char *p, ngx_معلومات_شي_t *شي)
{
    ngx_str_t  *str;

    str = &شي->معلومات.string;

    *p++ = '"';

    p = (u_char *) ngx_escape_json(p, str->معلومات, str->len);

    *p++ = '"';

    أرجع p;
}


ساكن ssize_t
ngx_json_integer_ڊيگهائي(ngx_معلومات_شي_t *شي)
{
    أرجع NGX_INT64_LEN;
}


ساكن u_char *
ngx_json_integer_encode(u_char *p, ngx_معلومات_شي_t *شي)
{
    أرجع ngx_sprintf(p, "%L", شي->معلومات.integer);
}


ساكن ssize_t
ngx_json_boolean_ڊيگهائي(ngx_معلومات_شي_t *شي)
{
    أرجع حجم("false") - 1;
}


ساكن u_char *
ngx_json_boolean_encode(u_char *p, ngx_معلومات_شي_t *شي)
{
    أرجع شي->معلومات.boolean ? ngx_cpymem(p, "true", 4)
                              : ngx_cpymem(p, "false", 5);
}


ساكن ssize_t
ngx_json_null_ڊيگهائي(ngx_معلومات_شي_t *شي)
{
    أرجع حجم("null") - 1;
}


ساكن u_char *
ngx_json_null_encode(u_char *p, ngx_معلومات_شي_t *شي)
{
    أرجع ngx_cpymem(p, "null", 4);
}
