
/*
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع ssize_t   (*ngx_json_لمائی_pt)(ngx_انکڑے_شے_t *شے);
تعريفنوع u_char   *(*ngx_json_encode_pt)(u_char *p, ngx_انکڑے_شے_t *شے);


تعريفنوع بنية {
    ngx_json_لمائی_pt        لمائی;
    ngx_json_encode_pt        encode;
} ngx_json_encode_t;


ساكن ssize_t ngx_json_obj_لمائی(ngx_انکڑے_شے_t *شے);
ساكن ssize_t ngx_json_list_لمائی(ngx_انکڑے_شے_t *شے);
ساكن ssize_t ngx_json_string_لمائی(ngx_انکڑے_شے_t *شے);
ساكن ssize_t ngx_json_integer_لمائی(ngx_انکڑے_شے_t *شے);
ساكن ssize_t ngx_json_boolean_لمائی(ngx_انکڑے_شے_t *شے);
ساكن ssize_t ngx_json_null_لمائی(ngx_انکڑے_شے_t *شے);

ساكن u_char *ngx_json_obj_encode(u_char *p, ngx_انکڑے_شے_t *شے);
ساكن u_char *ngx_json_list_encode(u_char *p, ngx_انکڑے_شے_t *شے);
ساكن u_char *ngx_json_string_encode(u_char *p, ngx_انکڑے_شے_t *شے);
ساكن u_char *ngx_json_integer_encode(u_char *p, ngx_انکڑے_شے_t *شے);
ساكن u_char *ngx_json_boolean_encode(u_char *p, ngx_انکڑے_شے_t *شے);
ساكن u_char *ngx_json_null_encode(u_char *p, ngx_انکڑے_شے_t *شے);


ساكن ngx_json_encode_t  ngx_json_encode[] = {

    /* NGX_DATA_OBJECT_TYPE */
    { ngx_json_obj_لمائی,
      ngx_json_obj_encode },

    /* NGX_DATA_LIST_TYPE */
    { ngx_json_list_لمائی,
      ngx_json_list_encode },

    /* NGX_DATA_STRING_TYPE */
    { ngx_json_string_لمائی,
      ngx_json_string_encode },

    /* NGX_DATA_INTEGER_TYPE */
    { ngx_json_integer_لمائی,
      ngx_json_integer_encode },

    /* NGX_DATA_BOOLEAN_TYPE */
    { ngx_json_boolean_لمائی,
      ngx_json_boolean_encode },

    /* NGX_DATA_NULL_TYPE */
    { ngx_json_null_لمائی,
      ngx_json_null_encode },
};


ngx_buf_t *
ngx_json_render(ngx_pool_t *pool, ngx_انکڑے_شے_t *شے)
{
    ناپ_t      لمائی;
    ngx_buf_t  *buf;

    لمائی = ngx_json_encode[شے->type].لمائی(شے);
    إذا (لمائی == (ناپ_t) NGX_ERROR) {
        أرجع عدم;
    }

    buf = ngx_create_عارضی_buf(pool, لمائی);
    إذا (buf == عدم) {
        أرجع عدم;
    }

    buf->آخری = ngx_json_encode[شے->type].encode(buf->آخری, شے);

    أرجع buf;
}


ساكن ssize_t
ngx_json_obj_لمائی(ngx_انکڑے_شے_t *شے)
{
    ناپ_t            len, کل;
    ngx_انکڑے_obj_t   *obj;
    ngx_انکڑے_شے_t  *i;

    obj = &شے->انکڑے.object;
    کل = حجم("{}") - 1;
    i = obj->شے;

    إذا (i) {
        لكل ( ;; ) {
            إذا (i->name.len > NGX_MAX_ناپ_T_قدر - (حجم("\"\":") - 1)) {
                أرجع NGX_ERROR;
            }

            len = حجم("\"\":") - 1 + i->name.len;

            إذا (len > NGX_MAX_ناپ_T_قدر - کل) {
                أرجع NGX_ERROR;
            }

            کل += len;

            len = ngx_escape_json(عدم, i->name.انکڑے, i->name.len);

            إذا (len > NGX_MAX_ناپ_T_قدر - کل) {
                أرجع NGX_ERROR;
            }

            کل += len;

            len = ngx_json_encode[i->type].لمائی(i);
            إذا (len == (ناپ_t) NGX_ERROR) {
                أرجع NGX_ERROR;
            }

            إذا (len > NGX_MAX_ناپ_T_قدر - کل) {
                أرجع NGX_ERROR;
            }

            کل += len;

            i = i->اگلا;

            إذا (i == عدم) {
                اكسر;
            }

            إذا (کل > NGX_MAX_ناپ_T_قدر - (حجم(",") - 1)) {
                أرجع NGX_ERROR;
            }

            کل += حجم(",") - 1;
        }
    }

    أرجع کل;
}


ساكن u_char *
ngx_json_obj_encode(u_char *p, ngx_انکڑے_شے_t *شے)
{
    ngx_انکڑے_obj_t   *obj;
    ngx_انکڑے_شے_t  *i;

    obj = &شے->انکڑے.object;
    i = obj->شے;

    *p++ = '{';

    إذا (i) {
        لكل ( ;; ) {
            *p++ = '"';

            p = (u_char *) ngx_escape_json(p, i->name.انکڑے, i->name.len);

            *p++ = '"';
            *p++ = ':';

            p = ngx_json_encode[i->type].encode(p, i);

            i = i->اگلا;

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
ngx_json_list_لمائی(ngx_انکڑے_شے_t *شے)
{
    ناپ_t            len, کل;
    ngx_انکڑے_obj_t   *obj;
    ngx_انکڑے_شے_t  *i;

    obj = &شے->انکڑے.object;
    کل = حجم("[]") - 1;
    i = obj->شے;

    إذا (i) {
        لكل ( ;; ) {
            len = ngx_json_encode[i->type].لمائی(i);
            إذا (len == (ناپ_t) NGX_ERROR) {
                أرجع NGX_ERROR;
            }

            إذا (len > NGX_MAX_ناپ_T_قدر - کل) {
                أرجع NGX_ERROR;
            }

            کل += len;

            i = i->اگلا;

            إذا (i == عدم) {
                اكسر;
            }

            إذا (کل > NGX_MAX_ناپ_T_قدر - (حجم(",") - 1)) {
                أرجع NGX_ERROR;
            }

            کل += حجم(",") - 1;
        }
    }

    أرجع کل;
}


ساكن u_char *
ngx_json_list_encode(u_char *p, ngx_انکڑے_شے_t *شے)
{
    ngx_انکڑے_obj_t   *obj;
    ngx_انکڑے_شے_t  *i;

    obj = &شے->انکڑے.object;
    i = obj->شے;

    *p++ = '[';

    إذا (i) {
        لكل ( ;; ) {
            p = ngx_json_encode[i->type].encode(p, i);

            i = i->اگلا;

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
ngx_json_string_لمائی(ngx_انکڑے_شے_t *شے)
{
    ناپ_t      len, کل;
    ngx_str_t  *str;

    str = &شے->انکڑے.string;

    إذا (str->len > NGX_MAX_ناپ_T_قدر - (حجم("\"\"") - 1)) {
        أرجع NGX_ERROR;
    }

    کل = حجم("\"\"") - 1 + str->len;

    len = ngx_escape_json(عدم, str->انکڑے, str->len);

    إذا (len > NGX_MAX_ناپ_T_قدر - کل) {
        أرجع NGX_ERROR;
    }

    أرجع کل + len;
}


ساكن u_char *
ngx_json_string_encode(u_char *p, ngx_انکڑے_شے_t *شے)
{
    ngx_str_t  *str;

    str = &شے->انکڑے.string;

    *p++ = '"';

    p = (u_char *) ngx_escape_json(p, str->انکڑے, str->len);

    *p++ = '"';

    أرجع p;
}


ساكن ssize_t
ngx_json_integer_لمائی(ngx_انکڑے_شے_t *شے)
{
    أرجع NGX_INT64_LEN;
}


ساكن u_char *
ngx_json_integer_encode(u_char *p, ngx_انکڑے_شے_t *شے)
{
    أرجع ngx_sprintf(p, "%L", شے->انکڑے.integer);
}


ساكن ssize_t
ngx_json_boolean_لمائی(ngx_انکڑے_شے_t *شے)
{
    أرجع حجم("false") - 1;
}


ساكن u_char *
ngx_json_boolean_encode(u_char *p, ngx_انکڑے_شے_t *شے)
{
    أرجع شے->انکڑے.boolean ? ngx_cpymem(p, "true", 4)
                              : ngx_cpymem(p, "false", 5);
}


ساكن ssize_t
ngx_json_null_لمائی(ngx_انکڑے_شے_t *شے)
{
    أرجع حجم("null") - 1;
}


ساكن u_char *
ngx_json_null_encode(u_char *p, ngx_انکڑے_شے_t *شے)
{
    أرجع ngx_cpymem(p, "null", 4);
}
