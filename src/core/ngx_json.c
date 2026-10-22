
/*
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


تعریفنوع ssize_t   (*ngx_json_length_pt)(ngx_data_item_t *item);
تعریفنوع u_char   *(*ngx_json_encode_pt)(u_char *p, ngx_data_item_t *item);


تعریفنوع ساختار {
    ngx_json_length_pt        length;
    ngx_json_encode_pt        encode;
} ngx_json_encode_t;


ایستا ssize_t ngx_json_obj_length(ngx_data_item_t *item);
ایستا ssize_t ngx_json_list_length(ngx_data_item_t *item);
ایستا ssize_t ngx_json_string_length(ngx_data_item_t *item);
ایستا ssize_t ngx_json_integer_length(ngx_data_item_t *item);
ایستا ssize_t ngx_json_boolean_length(ngx_data_item_t *item);
ایستا ssize_t ngx_json_null_length(ngx_data_item_t *item);

ایستا u_char *ngx_json_obj_encode(u_char *p, ngx_data_item_t *item);
ایستا u_char *ngx_json_list_encode(u_char *p, ngx_data_item_t *item);
ایستا u_char *ngx_json_string_encode(u_char *p, ngx_data_item_t *item);
ایستا u_char *ngx_json_integer_encode(u_char *p, ngx_data_item_t *item);
ایستا u_char *ngx_json_boolean_encode(u_char *p, ngx_data_item_t *item);
ایستا u_char *ngx_json_null_encode(u_char *p, ngx_data_item_t *item);


ایستا ngx_json_encode_t  ngx_json_encode[] = {

    /* NGX_DATA_OBJECT_TYPE */
    { ngx_json_obj_length,
      ngx_json_obj_encode },

    /* NGX_DATA_LIST_TYPE */
    { ngx_json_list_length,
      ngx_json_list_encode },

    /* NGX_DATA_STRING_TYPE */
    { ngx_json_string_length,
      ngx_json_string_encode },

    /* NGX_DATA_INTEGER_TYPE */
    { ngx_json_integer_length,
      ngx_json_integer_encode },

    /* NGX_DATA_BOOLEAN_TYPE */
    { ngx_json_boolean_length,
      ngx_json_boolean_encode },

    /* NGX_DATA_NULL_TYPE */
    { ngx_json_null_length,
      ngx_json_null_encode },
};


ngx_buf_t *
ngx_json_render(ngx_pool_t *pool, ngx_data_item_t *item)
{
    size_t      length;
    ngx_buf_t  *buf;

    length = ngx_json_encode[item->type].length(item);
    اگر (length == (size_t) NGX_ERROR) {
        بازگشت پوچ;
    }

    buf = ngx_create_temp_buf(pool, length);
    اگر (buf == پوچ) {
        بازگشت پوچ;
    }

    buf->last = ngx_json_encode[item->type].encode(buf->last, item);

    بازگشت buf;
}


ایستا ssize_t
ngx_json_obj_length(ngx_data_item_t *item)
{
    size_t            len, total;
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    total = اندازه("{}") - 1;
    i = obj->item;

    اگر (i) {
        برای ( ;; ) {
            اگر (i->name.len > NGX_MAX_SIZE_T_مقدار - (اندازه("\"\":") - 1)) {
                بازگشت NGX_ERROR;
            }

            len = اندازه("\"\":") - 1 + i->name.len;

            اگر (len > NGX_MAX_SIZE_T_مقدار - total) {
                بازگشت NGX_ERROR;
            }

            total += len;

            len = ngx_escape_json(پوچ, i->name.data, i->name.len);

            اگر (len > NGX_MAX_SIZE_T_مقدار - total) {
                بازگشت NGX_ERROR;
            }

            total += len;

            len = ngx_json_encode[i->type].length(i);
            اگر (len == (size_t) NGX_ERROR) {
                بازگشت NGX_ERROR;
            }

            اگر (len > NGX_MAX_SIZE_T_مقدار - total) {
                بازگشت NGX_ERROR;
            }

            total += len;

            i = i->next;

            اگر (i == پوچ) {
                بشکن;
            }

            اگر (total > NGX_MAX_SIZE_T_مقدار - (اندازه(",") - 1)) {
                بازگشت NGX_ERROR;
            }

            total += اندازه(",") - 1;
        }
    }

    بازگشت total;
}


ایستا u_char *
ngx_json_obj_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    i = obj->item;

    *p++ = '{';

    اگر (i) {
        برای ( ;; ) {
            *p++ = '"';

            p = (u_char *) ngx_escape_json(p, i->name.data, i->name.len);

            *p++ = '"';
            *p++ = ':';

            p = ngx_json_encode[i->type].encode(p, i);

            i = i->next;

            اگر (i == پوچ) {
                بشکن;
            }

            *p++ = ',';
        }
    }

    *p++ = '}';

    بازگشت p;
}


ایستا ssize_t
ngx_json_list_length(ngx_data_item_t *item)
{
    size_t            len, total;
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    total = اندازه("[]") - 1;
    i = obj->item;

    اگر (i) {
        برای ( ;; ) {
            len = ngx_json_encode[i->type].length(i);
            اگر (len == (size_t) NGX_ERROR) {
                بازگشت NGX_ERROR;
            }

            اگر (len > NGX_MAX_SIZE_T_مقدار - total) {
                بازگشت NGX_ERROR;
            }

            total += len;

            i = i->next;

            اگر (i == پوچ) {
                بشکن;
            }

            اگر (total > NGX_MAX_SIZE_T_مقدار - (اندازه(",") - 1)) {
                بازگشت NGX_ERROR;
            }

            total += اندازه(",") - 1;
        }
    }

    بازگشت total;
}


ایستا u_char *
ngx_json_list_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    i = obj->item;

    *p++ = '[';

    اگر (i) {
        برای ( ;; ) {
            p = ngx_json_encode[i->type].encode(p, i);

            i = i->next;

            اگر (i == پوچ) {
                بشکن;
            }

            *p++ = ',';
        }
    }

    *p++ = ']';

    بازگشت p;
}


ایستا ssize_t
ngx_json_string_length(ngx_data_item_t *item)
{
    size_t      len, total;
    ngx_str_t  *str;

    str = &item->data.string;

    اگر (str->len > NGX_MAX_SIZE_T_مقدار - (اندازه("\"\"") - 1)) {
        بازگشت NGX_ERROR;
    }

    total = اندازه("\"\"") - 1 + str->len;

    len = ngx_escape_json(پوچ, str->data, str->len);

    اگر (len > NGX_MAX_SIZE_T_مقدار - total) {
        بازگشت NGX_ERROR;
    }

    بازگشت total + len;
}


ایستا u_char *
ngx_json_string_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_str_t  *str;

    str = &item->data.string;

    *p++ = '"';

    p = (u_char *) ngx_escape_json(p, str->data, str->len);

    *p++ = '"';

    بازگشت p;
}


ایستا ssize_t
ngx_json_integer_length(ngx_data_item_t *item)
{
    بازگشت NGX_INT64_LEN;
}


ایستا u_char *
ngx_json_integer_encode(u_char *p, ngx_data_item_t *item)
{
    بازگشت ngx_sprintf(p, "%L", item->data.integer);
}


ایستا ssize_t
ngx_json_boolean_length(ngx_data_item_t *item)
{
    بازگشت اندازه("false") - 1;
}


ایستا u_char *
ngx_json_boolean_encode(u_char *p, ngx_data_item_t *item)
{
    بازگشت item->data.boolean ? ngx_cpymem(p, "true", 4)
                              : ngx_cpymem(p, "false", 5);
}


ایستا ssize_t
ngx_json_null_length(ngx_data_item_t *item)
{
    بازگشت اندازه("null") - 1;
}


ایستا u_char *
ngx_json_null_encode(u_char *p, ngx_data_item_t *item)
{
    بازگشت ngx_cpymem(p, "null", 4);
}
