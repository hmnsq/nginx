
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
    nếu (length == (size_t) NGX_ERROR) {
        trả_về NULL;
    }

    buf = ngx_create_temp_buf(pool, length);
    nếu (buf == NULL) {
        trả_về NULL;
    }

    buf->last = ngx_json_encode[item->type].encode(buf->last, item);

    trả_về buf;
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

    nếu (i) {
        cho ( ;; ) {
            nếu (i->name.len > NGX_MAX_SIZE_T_VALUE - (sizeof("\"\":") - 1)) {
                trả_về NGX_ERROR;
            }

            len = sizeof("\"\":") - 1 + i->name.len;

            nếu (len > NGX_MAX_SIZE_T_VALUE - total) {
                trả_về NGX_ERROR;
            }

            total += len;

            len = ngx_escape_json(NULL, i->name.data, i->name.len);

            nếu (len > NGX_MAX_SIZE_T_VALUE - total) {
                trả_về NGX_ERROR;
            }

            total += len;

            len = ngx_json_encode[i->type].length(i);
            nếu (len == (size_t) NGX_ERROR) {
                trả_về NGX_ERROR;
            }

            nếu (len > NGX_MAX_SIZE_T_VALUE - total) {
                trả_về NGX_ERROR;
            }

            total += len;

            i = i->next;

            nếu (i == NULL) {
                break;
            }

            nếu (total > NGX_MAX_SIZE_T_VALUE - (sizeof(",") - 1)) {
                trả_về NGX_ERROR;
            }

            total += sizeof(",") - 1;
        }
    }

    trả_về total;
}


static u_char *
ngx_json_obj_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    i = obj->item;

    *p++ = '{';

    nếu (i) {
        cho ( ;; ) {
            *p++ = '"';

            p = (u_char *) ngx_escape_json(p, i->name.data, i->name.len);

            *p++ = '"';
            *p++ = ':';

            p = ngx_json_encode[i->type].encode(p, i);

            i = i->next;

            nếu (i == NULL) {
                break;
            }

            *p++ = ',';
        }
    }

    *p++ = '}';

    trả_về p;
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

    nếu (i) {
        cho ( ;; ) {
            len = ngx_json_encode[i->type].length(i);
            nếu (len == (size_t) NGX_ERROR) {
                trả_về NGX_ERROR;
            }

            nếu (len > NGX_MAX_SIZE_T_VALUE - total) {
                trả_về NGX_ERROR;
            }

            total += len;

            i = i->next;

            nếu (i == NULL) {
                break;
            }

            nếu (total > NGX_MAX_SIZE_T_VALUE - (sizeof(",") - 1)) {
                trả_về NGX_ERROR;
            }

            total += sizeof(",") - 1;
        }
    }

    trả_về total;
}


static u_char *
ngx_json_list_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    i = obj->item;

    *p++ = '[';

    nếu (i) {
        cho ( ;; ) {
            p = ngx_json_encode[i->type].encode(p, i);

            i = i->next;

            nếu (i == NULL) {
                break;
            }

            *p++ = ',';
        }
    }

    *p++ = ']';

    trả_về p;
}


static ssize_t
ngx_json_string_length(ngx_data_item_t *item)
{
    size_t      len, total;
    ngx_str_t  *str;

    str = &item->data.string;

    nếu (str->len > NGX_MAX_SIZE_T_VALUE - (sizeof("\"\"") - 1)) {
        trả_về NGX_ERROR;
    }

    total = sizeof("\"\"") - 1 + str->len;

    len = ngx_escape_json(NULL, str->data, str->len);

    nếu (len > NGX_MAX_SIZE_T_VALUE - total) {
        trả_về NGX_ERROR;
    }

    trả_về total + len;
}


static u_char *
ngx_json_string_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_str_t  *str;

    str = &item->data.string;

    *p++ = '"';

    p = (u_char *) ngx_escape_json(p, str->data, str->len);

    *p++ = '"';

    trả_về p;
}


static ssize_t
ngx_json_integer_length(ngx_data_item_t *item)
{
    trả_về NGX_INT64_LEN;
}


static u_char *
ngx_json_integer_encode(u_char *p, ngx_data_item_t *item)
{
    trả_về ngx_sprintf(p, "%L", item->data.integer);
}


static ssize_t
ngx_json_boolean_length(ngx_data_item_t *item)
{
    trả_về sizeof("false") - 1;
}


static u_char *
ngx_json_boolean_encode(u_char *p, ngx_data_item_t *item)
{
    trả_về item->data.boolean ? ngx_cpymem(p, "true", 4)
                              : ngx_cpymem(p, "false", 5);
}


static ssize_t
ngx_json_null_length(ngx_data_item_t *item)
{
    trả_về sizeof("null") - 1;
}


static u_char *
ngx_json_null_encode(u_char *p, ngx_data_item_t *item)
{
    trả_về ngx_cpymem(p, "null", 4);
}
