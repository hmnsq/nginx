
/*
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


قسمتعریف ssize_t   (*ngx_json_لمبائی_pt)(ngx_ڈیٹا_شے_t *شے);
قسمتعریف u_char   *(*ngx_json_encode_pt)(u_char *p, ngx_ڈیٹا_شے_t *شے);


قسمتعریف ساخت {
    ngx_json_لمبائی_pt        لمبائی;
    ngx_json_encode_pt        encode;
} ngx_json_encode_t;


ساکن ssize_t ngx_json_obj_لمبائی(ngx_ڈیٹا_شے_t *شے);
ساکن ssize_t ngx_json_list_لمبائی(ngx_ڈیٹا_شے_t *شے);
ساکن ssize_t ngx_json_string_لمبائی(ngx_ڈیٹا_شے_t *شے);
ساکن ssize_t ngx_json_integer_لمبائی(ngx_ڈیٹا_شے_t *شے);
ساکن ssize_t ngx_json_boolean_لمبائی(ngx_ڈیٹا_شے_t *شے);
ساکن ssize_t ngx_json_null_لمبائی(ngx_ڈیٹا_شے_t *شے);

ساکن u_char *ngx_json_obj_encode(u_char *p, ngx_ڈیٹا_شے_t *شے);
ساکن u_char *ngx_json_list_encode(u_char *p, ngx_ڈیٹا_شے_t *شے);
ساکن u_char *ngx_json_string_encode(u_char *p, ngx_ڈیٹا_شے_t *شے);
ساکن u_char *ngx_json_integer_encode(u_char *p, ngx_ڈیٹا_شے_t *شے);
ساکن u_char *ngx_json_boolean_encode(u_char *p, ngx_ڈیٹا_شے_t *شے);
ساکن u_char *ngx_json_null_encode(u_char *p, ngx_ڈیٹا_شے_t *شے);


ساکن ngx_json_encode_t  ngx_json_encode[] = {

    /* NGX_DATA_OBJECT_TYPE */
    { ngx_json_obj_لمبائی,
      ngx_json_obj_encode },

    /* NGX_DATA_LIST_TYPE */
    { ngx_json_list_لمبائی,
      ngx_json_list_encode },

    /* NGX_DATA_STRING_TYPE */
    { ngx_json_string_لمبائی,
      ngx_json_string_encode },

    /* NGX_DATA_INTEGER_TYPE */
    { ngx_json_integer_لمبائی,
      ngx_json_integer_encode },

    /* NGX_DATA_BOOLEAN_TYPE */
    { ngx_json_boolean_لمبائی,
      ngx_json_boolean_encode },

    /* NGX_DATA_NULL_TYPE */
    { ngx_json_null_لمبائی,
      ngx_json_null_encode },
};


ngx_buf_t *
ngx_json_render(ngx_pool_t *pool, ngx_ڈیٹا_شے_t *شے)
{
    سائز_t      لمبائی;
    ngx_buf_t  *buf;

    لمبائی = ngx_json_encode[شے->type].لمبائی(شے);
    اگر (لمبائی == (سائز_t) NGX_ERROR) {
        واپس تہی;
    }

    buf = ngx_create_عارضی_buf(pool, لمبائی);
    اگر (buf == تہی) {
        واپس تہی;
    }

    buf->آخری = ngx_json_encode[شے->type].encode(buf->آخری, شے);

    واپس buf;
}


ساکن ssize_t
ngx_json_obj_لمبائی(ngx_ڈیٹا_شے_t *شے)
{
    سائز_t            len, کل;
    ngx_ڈیٹا_obj_t   *obj;
    ngx_ڈیٹا_شے_t  *i;

    obj = &شے->ڈیٹا.object;
    کل = حجم("{}") - 1;
    i = obj->شے;

    اگر (i) {
        برائے ( ;; ) {
            اگر (i->name.len > NGX_MAX_سائز_T_قدر - (حجم("\"\":") - 1)) {
                واپس NGX_ERROR;
            }

            len = حجم("\"\":") - 1 + i->name.len;

            اگر (len > NGX_MAX_سائز_T_قدر - کل) {
                واپس NGX_ERROR;
            }

            کل += len;

            len = ngx_escape_json(تہی, i->name.ڈیٹا, i->name.len);

            اگر (len > NGX_MAX_سائز_T_قدر - کل) {
                واپس NGX_ERROR;
            }

            کل += len;

            len = ngx_json_encode[i->type].لمبائی(i);
            اگر (len == (سائز_t) NGX_ERROR) {
                واپس NGX_ERROR;
            }

            اگر (len > NGX_MAX_سائز_T_قدر - کل) {
                واپس NGX_ERROR;
            }

            کل += len;

            i = i->اگلا;

            اگر (i == تہی) {
                توڑو;
            }

            اگر (کل > NGX_MAX_سائز_T_قدر - (حجم(",") - 1)) {
                واپس NGX_ERROR;
            }

            کل += حجم(",") - 1;
        }
    }

    واپس کل;
}


ساکن u_char *
ngx_json_obj_encode(u_char *p, ngx_ڈیٹا_شے_t *شے)
{
    ngx_ڈیٹا_obj_t   *obj;
    ngx_ڈیٹا_شے_t  *i;

    obj = &شے->ڈیٹا.object;
    i = obj->شے;

    *p++ = '{';

    اگر (i) {
        برائے ( ;; ) {
            *p++ = '"';

            p = (u_char *) ngx_escape_json(p, i->name.ڈیٹا, i->name.len);

            *p++ = '"';
            *p++ = ':';

            p = ngx_json_encode[i->type].encode(p, i);

            i = i->اگلا;

            اگر (i == تہی) {
                توڑو;
            }

            *p++ = ',';
        }
    }

    *p++ = '}';

    واپس p;
}


ساکن ssize_t
ngx_json_list_لمبائی(ngx_ڈیٹا_شے_t *شے)
{
    سائز_t            len, کل;
    ngx_ڈیٹا_obj_t   *obj;
    ngx_ڈیٹا_شے_t  *i;

    obj = &شے->ڈیٹا.object;
    کل = حجم("[]") - 1;
    i = obj->شے;

    اگر (i) {
        برائے ( ;; ) {
            len = ngx_json_encode[i->type].لمبائی(i);
            اگر (len == (سائز_t) NGX_ERROR) {
                واپس NGX_ERROR;
            }

            اگر (len > NGX_MAX_سائز_T_قدر - کل) {
                واپس NGX_ERROR;
            }

            کل += len;

            i = i->اگلا;

            اگر (i == تہی) {
                توڑو;
            }

            اگر (کل > NGX_MAX_سائز_T_قدر - (حجم(",") - 1)) {
                واپس NGX_ERROR;
            }

            کل += حجم(",") - 1;
        }
    }

    واپس کل;
}


ساکن u_char *
ngx_json_list_encode(u_char *p, ngx_ڈیٹا_شے_t *شے)
{
    ngx_ڈیٹا_obj_t   *obj;
    ngx_ڈیٹا_شے_t  *i;

    obj = &شے->ڈیٹا.object;
    i = obj->شے;

    *p++ = '[';

    اگر (i) {
        برائے ( ;; ) {
            p = ngx_json_encode[i->type].encode(p, i);

            i = i->اگلا;

            اگر (i == تہی) {
                توڑو;
            }

            *p++ = ',';
        }
    }

    *p++ = ']';

    واپس p;
}


ساکن ssize_t
ngx_json_string_لمبائی(ngx_ڈیٹا_شے_t *شے)
{
    سائز_t      len, کل;
    ngx_str_t  *str;

    str = &شے->ڈیٹا.string;

    اگر (str->len > NGX_MAX_سائز_T_قدر - (حجم("\"\"") - 1)) {
        واپس NGX_ERROR;
    }

    کل = حجم("\"\"") - 1 + str->len;

    len = ngx_escape_json(تہی, str->ڈیٹا, str->len);

    اگر (len > NGX_MAX_سائز_T_قدر - کل) {
        واپس NGX_ERROR;
    }

    واپس کل + len;
}


ساکن u_char *
ngx_json_string_encode(u_char *p, ngx_ڈیٹا_شے_t *شے)
{
    ngx_str_t  *str;

    str = &شے->ڈیٹا.string;

    *p++ = '"';

    p = (u_char *) ngx_escape_json(p, str->ڈیٹا, str->len);

    *p++ = '"';

    واپس p;
}


ساکن ssize_t
ngx_json_integer_لمبائی(ngx_ڈیٹا_شے_t *شے)
{
    واپس NGX_INT64_LEN;
}


ساکن u_char *
ngx_json_integer_encode(u_char *p, ngx_ڈیٹا_شے_t *شے)
{
    واپس ngx_sprintf(p, "%L", شے->ڈیٹا.integer);
}


ساکن ssize_t
ngx_json_boolean_لمبائی(ngx_ڈیٹا_شے_t *شے)
{
    واپس حجم("false") - 1;
}


ساکن u_char *
ngx_json_boolean_encode(u_char *p, ngx_ڈیٹا_شے_t *شے)
{
    واپس شے->ڈیٹا.boolean ? ngx_cpymem(p, "true", 4)
                              : ngx_cpymem(p, "false", 5);
}


ساکن ssize_t
ngx_json_null_لمبائی(ngx_ڈیٹا_شے_t *شے)
{
    واپس حجم("null") - 1;
}


ساکن u_char *
ngx_json_null_encode(u_char *p, ngx_ڈیٹا_شے_t *شے)
{
    واپس ngx_cpymem(p, "null", 4);
}
