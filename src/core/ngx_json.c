
/*
 * Copyright (C) Nginx, Inc.
 */


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>


തരനിർവചനം ssize_t   (*ngx_json_length_pt)(ngx_data_item_t *item);
തരനിർവചനം u_char   *(*ngx_json_encode_pt)(u_char *p, ngx_data_item_t *item);


തരനിർവചനം ഘടന {
    ngx_json_length_pt        length;
    ngx_json_encode_pt        encode;
} ngx_json_encode_t;


സ്ഥിതം ssize_t ngx_json_obj_length(ngx_data_item_t *item);
സ്ഥിതം ssize_t ngx_json_list_length(ngx_data_item_t *item);
സ്ഥിതം ssize_t ngx_json_string_length(ngx_data_item_t *item);
സ്ഥിതം ssize_t ngx_json_integer_length(ngx_data_item_t *item);
സ്ഥിതം ssize_t ngx_json_boolean_length(ngx_data_item_t *item);
സ്ഥിതം ssize_t ngx_json_null_length(ngx_data_item_t *item);

സ്ഥിതം u_char *ngx_json_obj_encode(u_char *p, ngx_data_item_t *item);
സ്ഥിതം u_char *ngx_json_list_encode(u_char *p, ngx_data_item_t *item);
സ്ഥിതം u_char *ngx_json_string_encode(u_char *p, ngx_data_item_t *item);
സ്ഥിതം u_char *ngx_json_integer_encode(u_char *p, ngx_data_item_t *item);
സ്ഥിതം u_char *ngx_json_boolean_encode(u_char *p, ngx_data_item_t *item);
സ്ഥിതം u_char *ngx_json_null_encode(u_char *p, ngx_data_item_t *item);


സ്ഥിതം ngx_json_encode_t  ngx_json_encode[] = {

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
    എങ്കിൽ (length == (size_t) NGX_ERROR) {
        തിരികെ ശൂന്യ;
    }

    buf = ngx_create_temp_buf(pool, length);
    എങ്കിൽ (buf == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    buf->last = ngx_json_encode[item->type].encode(buf->last, item);

    തിരികെ buf;
}


സ്ഥിതം ssize_t
ngx_json_obj_length(ngx_data_item_t *item)
{
    size_t            len, total;
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    total = വലുപ്പം("{}") - 1;
    i = obj->item;

    എങ്കിൽ (i) {
        വേണ്ടി ( ;; ) {
            എങ്കിൽ (i->name.len > NGX_MAX_SIZE_T_മൂല്യം - (വലുപ്പം("\"\":") - 1)) {
                തിരികെ NGX_ERROR;
            }

            len = വലുപ്പം("\"\":") - 1 + i->name.len;

            എങ്കിൽ (len > NGX_MAX_SIZE_T_മൂല്യം - total) {
                തിരികെ NGX_ERROR;
            }

            total += len;

            len = ngx_escape_json(ശൂന്യ, i->name.data, i->name.len);

            എങ്കിൽ (len > NGX_MAX_SIZE_T_മൂല്യം - total) {
                തിരികെ NGX_ERROR;
            }

            total += len;

            len = ngx_json_encode[i->type].length(i);
            എങ്കിൽ (len == (size_t) NGX_ERROR) {
                തിരികെ NGX_ERROR;
            }

            എങ്കിൽ (len > NGX_MAX_SIZE_T_മൂല്യം - total) {
                തിരികെ NGX_ERROR;
            }

            total += len;

            i = i->next;

            എങ്കിൽ (i == ശൂന്യ) {
                നിർത്തുക;
            }

            എങ്കിൽ (total > NGX_MAX_SIZE_T_മൂല്യം - (വലുപ്പം(",") - 1)) {
                തിരികെ NGX_ERROR;
            }

            total += വലുപ്പം(",") - 1;
        }
    }

    തിരികെ total;
}


സ്ഥിതം u_char *
ngx_json_obj_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    i = obj->item;

    *p++ = '{';

    എങ്കിൽ (i) {
        വേണ്ടി ( ;; ) {
            *p++ = '"';

            p = (u_char *) ngx_escape_json(p, i->name.data, i->name.len);

            *p++ = '"';
            *p++ = ':';

            p = ngx_json_encode[i->type].encode(p, i);

            i = i->next;

            എങ്കിൽ (i == ശൂന്യ) {
                നിർത്തുക;
            }

            *p++ = ',';
        }
    }

    *p++ = '}';

    തിരികെ p;
}


സ്ഥിതം ssize_t
ngx_json_list_length(ngx_data_item_t *item)
{
    size_t            len, total;
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    total = വലുപ്പം("[]") - 1;
    i = obj->item;

    എങ്കിൽ (i) {
        വേണ്ടി ( ;; ) {
            len = ngx_json_encode[i->type].length(i);
            എങ്കിൽ (len == (size_t) NGX_ERROR) {
                തിരികെ NGX_ERROR;
            }

            എങ്കിൽ (len > NGX_MAX_SIZE_T_മൂല്യം - total) {
                തിരികെ NGX_ERROR;
            }

            total += len;

            i = i->next;

            എങ്കിൽ (i == ശൂന്യ) {
                നിർത്തുക;
            }

            എങ്കിൽ (total > NGX_MAX_SIZE_T_മൂല്യം - (വലുപ്പം(",") - 1)) {
                തിരികെ NGX_ERROR;
            }

            total += വലുപ്പം(",") - 1;
        }
    }

    തിരികെ total;
}


സ്ഥിതം u_char *
ngx_json_list_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    i = obj->item;

    *p++ = '[';

    എങ്കിൽ (i) {
        വേണ്ടി ( ;; ) {
            p = ngx_json_encode[i->type].encode(p, i);

            i = i->next;

            എങ്കിൽ (i == ശൂന്യ) {
                നിർത്തുക;
            }

            *p++ = ',';
        }
    }

    *p++ = ']';

    തിരികെ p;
}


സ്ഥിതം ssize_t
ngx_json_string_length(ngx_data_item_t *item)
{
    size_t      len, total;
    ngx_str_t  *str;

    str = &item->data.string;

    എങ്കിൽ (str->len > NGX_MAX_SIZE_T_മൂല്യം - (വലുപ്പം("\"\"") - 1)) {
        തിരികെ NGX_ERROR;
    }

    total = വലുപ്പം("\"\"") - 1 + str->len;

    len = ngx_escape_json(ശൂന്യ, str->data, str->len);

    എങ്കിൽ (len > NGX_MAX_SIZE_T_മൂല്യം - total) {
        തിരികെ NGX_ERROR;
    }

    തിരികെ total + len;
}


സ്ഥിതം u_char *
ngx_json_string_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_str_t  *str;

    str = &item->data.string;

    *p++ = '"';

    p = (u_char *) ngx_escape_json(p, str->data, str->len);

    *p++ = '"';

    തിരികെ p;
}


സ്ഥിതം ssize_t
ngx_json_integer_length(ngx_data_item_t *item)
{
    തിരികെ NGX_INT64_LEN;
}


സ്ഥിതം u_char *
ngx_json_integer_encode(u_char *p, ngx_data_item_t *item)
{
    തിരികെ ngx_sprintf(p, "%L", item->data.integer);
}


സ്ഥിതം ssize_t
ngx_json_boolean_length(ngx_data_item_t *item)
{
    തിരികെ വലുപ്പം("false") - 1;
}


സ്ഥിതം u_char *
ngx_json_boolean_encode(u_char *p, ngx_data_item_t *item)
{
    തിരികെ item->data.boolean ? ngx_cpymem(p, "true", 4)
                              : ngx_cpymem(p, "false", 5);
}


സ്ഥിതം ssize_t
ngx_json_null_length(ngx_data_item_t *item)
{
    തിരികെ വലുപ്പം("null") - 1;
}


സ്ഥിതം u_char *
ngx_json_null_encode(u_char *p, ngx_data_item_t *item)
{
    തിരികെ ngx_cpymem(p, "null", 4);
}
