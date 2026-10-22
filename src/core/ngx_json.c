
/*
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع ssize_t   (*ngx_json_length_pt)(ngx_data_item_t *item);
تعريفنوع u_char   *(*ngx_json_encode_pt)(u_char *p, ngx_data_item_t *item);


تعريفنوع بنية {
    ngx_json_length_pt        length;
    ngx_json_encode_pt        encode;
} ngx_json_encode_t;


ساكن ssize_t ngx_json_obj_length(ngx_data_item_t *item);
ساكن ssize_t ngx_json_list_length(ngx_data_item_t *item);
ساكن ssize_t ngx_json_string_length(ngx_data_item_t *item);
ساكن ssize_t ngx_json_integer_length(ngx_data_item_t *item);
ساكن ssize_t ngx_json_boolean_length(ngx_data_item_t *item);
ساكن ssize_t ngx_json_null_length(ngx_data_item_t *item);

ساكن u_char *ngx_json_obj_encode(u_char *p, ngx_data_item_t *item);
ساكن u_char *ngx_json_list_encode(u_char *p, ngx_data_item_t *item);
ساكن u_char *ngx_json_string_encode(u_char *p, ngx_data_item_t *item);
ساكن u_char *ngx_json_integer_encode(u_char *p, ngx_data_item_t *item);
ساكن u_char *ngx_json_boolean_encode(u_char *p, ngx_data_item_t *item);
ساكن u_char *ngx_json_null_encode(u_char *p, ngx_data_item_t *item);


ساكن ngx_json_encode_t  ngx_json_encode[] = {

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
    אם (length == (size_t) NGX_ERROR) {
        החזר عدم;
    }

    buf = ngx_create_temp_buf(pool, length);
    אם (buf == عدم) {
        החזר عدم;
    }

    buf->last = ngx_json_encode[item->type].encode(buf->last, item);

    החזר buf;
}


ساكن ssize_t
ngx_json_obj_length(ngx_data_item_t *item)
{
    size_t            len, total;
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    total = حجم("{}") - 1;
    i = obj->item;

    אם (i) {
        עבור ( ;; ) {
            אם (i->name.len > NGX_MAX_SIZE_T_VALUE - (حجم("\"\":") - 1)) {
                החזר NGX_ERROR;
            }

            len = حجم("\"\":") - 1 + i->name.len;

            אם (len > NGX_MAX_SIZE_T_VALUE - total) {
                החזר NGX_ERROR;
            }

            total += len;

            len = ngx_escape_json(عدم, i->name.data, i->name.len);

            אם (len > NGX_MAX_SIZE_T_VALUE - total) {
                החזר NGX_ERROR;
            }

            total += len;

            len = ngx_json_encode[i->type].length(i);
            אם (len == (size_t) NGX_ERROR) {
                החזר NGX_ERROR;
            }

            אם (len > NGX_MAX_SIZE_T_VALUE - total) {
                החזר NGX_ERROR;
            }

            total += len;

            i = i->next;

            אם (i == عدم) {
                اكسر;
            }

            אם (total > NGX_MAX_SIZE_T_VALUE - (حجم(",") - 1)) {
                החזר NGX_ERROR;
            }

            total += حجم(",") - 1;
        }
    }

    החזר total;
}


ساكن u_char *
ngx_json_obj_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    i = obj->item;

    *p++ = '{';

    אם (i) {
        עבור ( ;; ) {
            *p++ = '"';

            p = (u_char *) ngx_escape_json(p, i->name.data, i->name.len);

            *p++ = '"';
            *p++ = ':';

            p = ngx_json_encode[i->type].encode(p, i);

            i = i->next;

            אם (i == عدم) {
                اكسر;
            }

            *p++ = ',';
        }
    }

    *p++ = '}';

    החזר p;
}


ساكن ssize_t
ngx_json_list_length(ngx_data_item_t *item)
{
    size_t            len, total;
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    total = حجم("[]") - 1;
    i = obj->item;

    אם (i) {
        עבור ( ;; ) {
            len = ngx_json_encode[i->type].length(i);
            אם (len == (size_t) NGX_ERROR) {
                החזר NGX_ERROR;
            }

            אם (len > NGX_MAX_SIZE_T_VALUE - total) {
                החזר NGX_ERROR;
            }

            total += len;

            i = i->next;

            אם (i == عدم) {
                اكسر;
            }

            אם (total > NGX_MAX_SIZE_T_VALUE - (حجم(",") - 1)) {
                החזר NGX_ERROR;
            }

            total += حجم(",") - 1;
        }
    }

    החזר total;
}


ساكن u_char *
ngx_json_list_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    i = obj->item;

    *p++ = '[';

    אם (i) {
        עבור ( ;; ) {
            p = ngx_json_encode[i->type].encode(p, i);

            i = i->next;

            אם (i == عدم) {
                اكسر;
            }

            *p++ = ',';
        }
    }

    *p++ = ']';

    החזר p;
}


ساكن ssize_t
ngx_json_string_length(ngx_data_item_t *item)
{
    size_t      len, total;
    ngx_str_t  *str;

    str = &item->data.string;

    אם (str->len > NGX_MAX_SIZE_T_VALUE - (حجم("\"\"") - 1)) {
        החזר NGX_ERROR;
    }

    total = حجم("\"\"") - 1 + str->len;

    len = ngx_escape_json(عدم, str->data, str->len);

    אם (len > NGX_MAX_SIZE_T_VALUE - total) {
        החזר NGX_ERROR;
    }

    החזר total + len;
}


ساكن u_char *
ngx_json_string_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_str_t  *str;

    str = &item->data.string;

    *p++ = '"';

    p = (u_char *) ngx_escape_json(p, str->data, str->len);

    *p++ = '"';

    החזר p;
}


ساكن ssize_t
ngx_json_integer_length(ngx_data_item_t *item)
{
    החזר NGX_INT64_LEN;
}


ساكن u_char *
ngx_json_integer_encode(u_char *p, ngx_data_item_t *item)
{
    החזר ngx_sprintf(p, "%L", item->data.integer);
}


ساكن ssize_t
ngx_json_boolean_length(ngx_data_item_t *item)
{
    החזר حجم("false") - 1;
}


ساكن u_char *
ngx_json_boolean_encode(u_char *p, ngx_data_item_t *item)
{
    החזר item->data.boolean ? ngx_cpymem(p, "true", 4)
                              : ngx_cpymem(p, "false", 5);
}


ساكن ssize_t
ngx_json_null_length(ngx_data_item_t *item)
{
    החזר حجم("null") - 1;
}


ساكن u_char *
ngx_json_null_encode(u_char *p, ngx_data_item_t *item)
{
    החזר ngx_cpymem(p, "null", 4);
}
