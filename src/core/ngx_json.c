
/*
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


প্রকারনির্দেশ ssize_t   (*ngx_json_length_pt)(ngx_data_item_t *item);
প্রকারনির্দেশ u_char   *(*ngx_json_encode_pt)(u_char *p, ngx_data_item_t *item);


প্রকারনির্দেশ সংরচনা {
    ngx_json_length_pt        length;
    ngx_json_encode_pt        encode;
} ngx_json_encode_t;


স্থির ssize_t ngx_json_obj_length(ngx_data_item_t *item);
স্থির ssize_t ngx_json_list_length(ngx_data_item_t *item);
স্থির ssize_t ngx_json_string_length(ngx_data_item_t *item);
স্থির ssize_t ngx_json_integer_length(ngx_data_item_t *item);
স্থির ssize_t ngx_json_boolean_length(ngx_data_item_t *item);
স্থির ssize_t ngx_json_null_length(ngx_data_item_t *item);

স্থির u_char *ngx_json_obj_encode(u_char *p, ngx_data_item_t *item);
স্থির u_char *ngx_json_list_encode(u_char *p, ngx_data_item_t *item);
স্থির u_char *ngx_json_string_encode(u_char *p, ngx_data_item_t *item);
স্থির u_char *ngx_json_integer_encode(u_char *p, ngx_data_item_t *item);
স্থির u_char *ngx_json_boolean_encode(u_char *p, ngx_data_item_t *item);
স্থির u_char *ngx_json_null_encode(u_char *p, ngx_data_item_t *item);


স্থির ngx_json_encode_t  ngx_json_encode[] = {

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
    যদি (length == (size_t) NGX_ERROR) {
        প্রত্যাগম রিক্ত;
    }

    buf = ngx_create_temp_buf(pool, length);
    যদি (buf == রিক্ত) {
        প্রত্যাগম রিক্ত;
    }

    buf->last = ngx_json_encode[item->type].encode(buf->last, item);

    প্রত্যাগম buf;
}


স্থির ssize_t
ngx_json_obj_length(ngx_data_item_t *item)
{
    size_t            len, total;
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    total = পরিমাণ("{}") - 1;
    i = obj->item;

    যদি (i) {
        প্রতি ( ;; ) {
            যদি (i->name.len > NGX_MAX_SIZE_T_মান - (পরিমাণ("\"\":") - 1)) {
                প্রত্যাগম NGX_ERROR;
            }

            len = পরিমাণ("\"\":") - 1 + i->name.len;

            যদি (len > NGX_MAX_SIZE_T_মান - total) {
                প্রত্যাগম NGX_ERROR;
            }

            total += len;

            len = ngx_escape_json(রিক্ত, i->name.data, i->name.len);

            যদি (len > NGX_MAX_SIZE_T_মান - total) {
                প্রত্যাগম NGX_ERROR;
            }

            total += len;

            len = ngx_json_encode[i->type].length(i);
            যদি (len == (size_t) NGX_ERROR) {
                প্রত্যাগম NGX_ERROR;
            }

            যদি (len > NGX_MAX_SIZE_T_মান - total) {
                প্রত্যাগম NGX_ERROR;
            }

            total += len;

            i = i->next;

            যদি (i == রিক্ত) {
                ভঙ্গ;
            }

            যদি (total > NGX_MAX_SIZE_T_মান - (পরিমাণ(",") - 1)) {
                প্রত্যাগম NGX_ERROR;
            }

            total += পরিমাণ(",") - 1;
        }
    }

    প্রত্যাগম total;
}


স্থির u_char *
ngx_json_obj_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    i = obj->item;

    *p++ = '{';

    যদি (i) {
        প্রতি ( ;; ) {
            *p++ = '"';

            p = (u_char *) ngx_escape_json(p, i->name.data, i->name.len);

            *p++ = '"';
            *p++ = ':';

            p = ngx_json_encode[i->type].encode(p, i);

            i = i->next;

            যদি (i == রিক্ত) {
                ভঙ্গ;
            }

            *p++ = ',';
        }
    }

    *p++ = '}';

    প্রত্যাগম p;
}


স্থির ssize_t
ngx_json_list_length(ngx_data_item_t *item)
{
    size_t            len, total;
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    total = পরিমাণ("[]") - 1;
    i = obj->item;

    যদি (i) {
        প্রতি ( ;; ) {
            len = ngx_json_encode[i->type].length(i);
            যদি (len == (size_t) NGX_ERROR) {
                প্রত্যাগম NGX_ERROR;
            }

            যদি (len > NGX_MAX_SIZE_T_মান - total) {
                প্রত্যাগম NGX_ERROR;
            }

            total += len;

            i = i->next;

            যদি (i == রিক্ত) {
                ভঙ্গ;
            }

            যদি (total > NGX_MAX_SIZE_T_মান - (পরিমাণ(",") - 1)) {
                প্রত্যাগম NGX_ERROR;
            }

            total += পরিমাণ(",") - 1;
        }
    }

    প্রত্যাগম total;
}


স্থির u_char *
ngx_json_list_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    i = obj->item;

    *p++ = '[';

    যদি (i) {
        প্রতি ( ;; ) {
            p = ngx_json_encode[i->type].encode(p, i);

            i = i->next;

            যদি (i == রিক্ত) {
                ভঙ্গ;
            }

            *p++ = ',';
        }
    }

    *p++ = ']';

    প্রত্যাগম p;
}


স্থির ssize_t
ngx_json_string_length(ngx_data_item_t *item)
{
    size_t      len, total;
    ngx_str_t  *str;

    str = &item->data.string;

    যদি (str->len > NGX_MAX_SIZE_T_মান - (পরিমাণ("\"\"") - 1)) {
        প্রত্যাগম NGX_ERROR;
    }

    total = পরিমাণ("\"\"") - 1 + str->len;

    len = ngx_escape_json(রিক্ত, str->data, str->len);

    যদি (len > NGX_MAX_SIZE_T_মান - total) {
        প্রত্যাগম NGX_ERROR;
    }

    প্রত্যাগম total + len;
}


স্থির u_char *
ngx_json_string_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_str_t  *str;

    str = &item->data.string;

    *p++ = '"';

    p = (u_char *) ngx_escape_json(p, str->data, str->len);

    *p++ = '"';

    প্রত্যাগম p;
}


স্থির ssize_t
ngx_json_integer_length(ngx_data_item_t *item)
{
    প্রত্যাগম NGX_INT64_LEN;
}


স্থির u_char *
ngx_json_integer_encode(u_char *p, ngx_data_item_t *item)
{
    প্রত্যাগম ngx_sprintf(p, "%L", item->data.integer);
}


স্থির ssize_t
ngx_json_boolean_length(ngx_data_item_t *item)
{
    প্রত্যাগম পরিমাণ("false") - 1;
}


স্থির u_char *
ngx_json_boolean_encode(u_char *p, ngx_data_item_t *item)
{
    প্রত্যাগম item->data.boolean ? ngx_cpymem(p, "true", 4)
                              : ngx_cpymem(p, "false", 5);
}


স্থির ssize_t
ngx_json_null_length(ngx_data_item_t *item)
{
    প্রত্যাগম পরিমাণ("null") - 1;
}


স্থির u_char *
ngx_json_null_encode(u_char *p, ngx_data_item_t *item)
{
    প্রত্যাগম ngx_cpymem(p, "null", 4);
}
