
/*
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


typedef ssize_t   (*ngx_json_length_pt)(ngx_data_item_t *item);
typedef u_char   *(*ngx_json_encode_pt)(u_char *p, ngx_data_item_t *item);


typedef struct {
    ngx_json_length_pt        length;
    ngx_json_encode_pt        encode;
} ngx_json_encode_t;


static ssize_t ngx_json_obj_length(ngx_data_item_t *item);
static ssize_t ngx_json_list_length(ngx_data_item_t *item);
static ssize_t ngx_json_string_length(ngx_data_item_t *item);
static ssize_t ngx_json_integer_length(ngx_data_item_t *item);
static ssize_t ngx_json_boolean_length(ngx_data_item_t *item);
static ssize_t ngx_json_null_length(ngx_data_item_t *item);

static u_char *ngx_json_obj_encode(u_char *p, ngx_data_item_t *item);
static u_char *ngx_json_list_encode(u_char *p, ngx_data_item_t *item);
static u_char *ngx_json_string_encode(u_char *p, ngx_data_item_t *item);
static u_char *ngx_json_integer_encode(u_char *p, ngx_data_item_t *item);
static u_char *ngx_json_boolean_encode(u_char *p, ngx_data_item_t *item);
static u_char *ngx_json_null_encode(u_char *p, ngx_data_item_t *item);


static ngx_json_encode_t  ngx_json_encode[] = {

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
    如果 (length == (size_t) NGX_ERROR) {
        返回 NULL;
    }

    buf = ngx_create_temp_buf(pool, length);
    如果 (buf == NULL) {
        返回 NULL;
    }

    buf->last = ngx_json_encode[item->type].encode(buf->last, item);

    返回 buf;
}


static ssize_t
ngx_json_obj_length(ngx_data_item_t *item)
{
    size_t            len, total;
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    total = sizeof("{}") - 1;
    i = obj->item;

    如果 (i) {
        为 ( ;; ) {
            如果 (i->name.len > NGX_MAX_SIZE_T_VALUE - (sizeof("\"\":") - 1)) {
                返回 NGX_ERROR;
            }

            len = sizeof("\"\":") - 1 + i->name.len;

            如果 (len > NGX_MAX_SIZE_T_VALUE - total) {
                返回 NGX_ERROR;
            }

            total += len;

            len = ngx_escape_json(NULL, i->name.data, i->name.len);

            如果 (len > NGX_MAX_SIZE_T_VALUE - total) {
                返回 NGX_ERROR;
            }

            total += len;

            len = ngx_json_encode[i->type].length(i);
            如果 (len == (size_t) NGX_ERROR) {
                返回 NGX_ERROR;
            }

            如果 (len > NGX_MAX_SIZE_T_VALUE - total) {
                返回 NGX_ERROR;
            }

            total += len;

            i = i->next;

            如果 (i == NULL) {
                break;
            }

            如果 (total > NGX_MAX_SIZE_T_VALUE - (sizeof(",") - 1)) {
                返回 NGX_ERROR;
            }

            total += sizeof(",") - 1;
        }
    }

    返回 total;
}


static u_char *
ngx_json_obj_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    i = obj->item;

    *p++ = '{';

    如果 (i) {
        为 ( ;; ) {
            *p++ = '"';

            p = (u_char *) ngx_escape_json(p, i->name.data, i->name.len);

            *p++ = '"';
            *p++ = ':';

            p = ngx_json_encode[i->type].encode(p, i);

            i = i->next;

            如果 (i == NULL) {
                break;
            }

            *p++ = ',';
        }
    }

    *p++ = '}';

    返回 p;
}


static ssize_t
ngx_json_list_length(ngx_data_item_t *item)
{
    size_t            len, total;
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    total = sizeof("[]") - 1;
    i = obj->item;

    如果 (i) {
        为 ( ;; ) {
            len = ngx_json_encode[i->type].length(i);
            如果 (len == (size_t) NGX_ERROR) {
                返回 NGX_ERROR;
            }

            如果 (len > NGX_MAX_SIZE_T_VALUE - total) {
                返回 NGX_ERROR;
            }

            total += len;

            i = i->next;

            如果 (i == NULL) {
                break;
            }

            如果 (total > NGX_MAX_SIZE_T_VALUE - (sizeof(",") - 1)) {
                返回 NGX_ERROR;
            }

            total += sizeof(",") - 1;
        }
    }

    返回 total;
}


static u_char *
ngx_json_list_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    i = obj->item;

    *p++ = '[';

    如果 (i) {
        为 ( ;; ) {
            p = ngx_json_encode[i->type].encode(p, i);

            i = i->next;

            如果 (i == NULL) {
                break;
            }

            *p++ = ',';
        }
    }

    *p++ = ']';

    返回 p;
}


static ssize_t
ngx_json_string_length(ngx_data_item_t *item)
{
    size_t      len, total;
    ngx_str_t  *str;

    str = &item->data.string;

    如果 (str->len > NGX_MAX_SIZE_T_VALUE - (sizeof("\"\"") - 1)) {
        返回 NGX_ERROR;
    }

    total = sizeof("\"\"") - 1 + str->len;

    len = ngx_escape_json(NULL, str->data, str->len);

    如果 (len > NGX_MAX_SIZE_T_VALUE - total) {
        返回 NGX_ERROR;
    }

    返回 total + len;
}


static u_char *
ngx_json_string_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_str_t  *str;

    str = &item->data.string;

    *p++ = '"';

    p = (u_char *) ngx_escape_json(p, str->data, str->len);

    *p++ = '"';

    返回 p;
}


static ssize_t
ngx_json_integer_length(ngx_data_item_t *item)
{
    返回 NGX_INT64_LEN;
}


static u_char *
ngx_json_integer_encode(u_char *p, ngx_data_item_t *item)
{
    返回 ngx_sprintf(p, "%L", item->data.integer);
}


static ssize_t
ngx_json_boolean_length(ngx_data_item_t *item)
{
    返回 sizeof("false") - 1;
}


static u_char *
ngx_json_boolean_encode(u_char *p, ngx_data_item_t *item)
{
    返回 item->data.boolean ? ngx_cpymem(p, "true", 4)
                              : ngx_cpymem(p, "false", 5);
}


static ssize_t
ngx_json_null_length(ngx_data_item_t *item)
{
    返回 sizeof("null") - 1;
}


static u_char *
ngx_json_null_encode(u_char *p, ngx_data_item_t *item)
{
    返回 ngx_cpymem(p, "null", 4);
}
