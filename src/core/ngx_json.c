
/*
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


typedef ssize_t   (*ngx_json_length_pt)(ngx_data_item_t *item);
typedef u_char   *(*ngx_json_encode_pt)(u_char *p, ngx_data_item_t *item);


typedef sa_nracanaa {
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
    ถ้า (length == (size_t) NGX_ERROR) {
        คืน NULL;
    }

    buf = ngx_create_temp_buf(pool, length);
    ถ้า (buf == NULL) {
        คืน NULL;
    }

    buf->last = ngx_json_encode[item->type].encode(buf->last, item);

    คืน buf;
}


static ssize_t
ngx_json_obj_length(ngx_data_item_t *item)
{
    size_t            len, total;
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    total = parimaa_na("{}") - 1;
    i = obj->item;

    ถ้า (i) {
        สำหรับ ( ;; ) {
            ถ้า (i->name.len > NGX_MAX_SIZE_T_VALUE - (parimaa_na("\"\":") - 1)) {
                คืน NGX_ERROR;
            }

            len = parimaa_na("\"\":") - 1 + i->name.len;

            ถ้า (len > NGX_MAX_SIZE_T_VALUE - total) {
                คืน NGX_ERROR;
            }

            total += len;

            len = ngx_escape_json(NULL, i->name.data, i->name.len);

            ถ้า (len > NGX_MAX_SIZE_T_VALUE - total) {
                คืน NGX_ERROR;
            }

            total += len;

            len = ngx_json_encode[i->type].length(i);
            ถ้า (len == (size_t) NGX_ERROR) {
                คืน NGX_ERROR;
            }

            ถ้า (len > NGX_MAX_SIZE_T_VALUE - total) {
                คืน NGX_ERROR;
            }

            total += len;

            i = i->next;

            ถ้า (i == NULL) {
                bha_nga;
            }

            ถ้า (total > NGX_MAX_SIZE_T_VALUE - (parimaa_na(",") - 1)) {
                คืน NGX_ERROR;
            }

            total += parimaa_na(",") - 1;
        }
    }

    คืน total;
}


static u_char *
ngx_json_obj_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    i = obj->item;

    *p++ = '{';

    ถ้า (i) {
        สำหรับ ( ;; ) {
            *p++ = '"';

            p = (u_char *) ngx_escape_json(p, i->name.data, i->name.len);

            *p++ = '"';
            *p++ = ':';

            p = ngx_json_encode[i->type].encode(p, i);

            i = i->next;

            ถ้า (i == NULL) {
                bha_nga;
            }

            *p++ = ',';
        }
    }

    *p++ = '}';

    คืน p;
}


static ssize_t
ngx_json_list_length(ngx_data_item_t *item)
{
    size_t            len, total;
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    total = parimaa_na("[]") - 1;
    i = obj->item;

    ถ้า (i) {
        สำหรับ ( ;; ) {
            len = ngx_json_encode[i->type].length(i);
            ถ้า (len == (size_t) NGX_ERROR) {
                คืน NGX_ERROR;
            }

            ถ้า (len > NGX_MAX_SIZE_T_VALUE - total) {
                คืน NGX_ERROR;
            }

            total += len;

            i = i->next;

            ถ้า (i == NULL) {
                bha_nga;
            }

            ถ้า (total > NGX_MAX_SIZE_T_VALUE - (parimaa_na(",") - 1)) {
                คืน NGX_ERROR;
            }

            total += parimaa_na(",") - 1;
        }
    }

    คืน total;
}


static u_char *
ngx_json_list_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    i = obj->item;

    *p++ = '[';

    ถ้า (i) {
        สำหรับ ( ;; ) {
            p = ngx_json_encode[i->type].encode(p, i);

            i = i->next;

            ถ้า (i == NULL) {
                bha_nga;
            }

            *p++ = ',';
        }
    }

    *p++ = ']';

    คืน p;
}


static ssize_t
ngx_json_string_length(ngx_data_item_t *item)
{
    size_t      len, total;
    ngx_str_t  *str;

    str = &item->data.string;

    ถ้า (str->len > NGX_MAX_SIZE_T_VALUE - (parimaa_na("\"\"") - 1)) {
        คืน NGX_ERROR;
    }

    total = parimaa_na("\"\"") - 1 + str->len;

    len = ngx_escape_json(NULL, str->data, str->len);

    ถ้า (len > NGX_MAX_SIZE_T_VALUE - total) {
        คืน NGX_ERROR;
    }

    คืน total + len;
}


static u_char *
ngx_json_string_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_str_t  *str;

    str = &item->data.string;

    *p++ = '"';

    p = (u_char *) ngx_escape_json(p, str->data, str->len);

    *p++ = '"';

    คืน p;
}


static ssize_t
ngx_json_integer_length(ngx_data_item_t *item)
{
    คืน NGX_INT64_LEN;
}


static u_char *
ngx_json_integer_encode(u_char *p, ngx_data_item_t *item)
{
    คืน ngx_sprintf(p, "%L", item->data.integer);
}


static ssize_t
ngx_json_boolean_length(ngx_data_item_t *item)
{
    คืน parimaa_na("false") - 1;
}


static u_char *
ngx_json_boolean_encode(u_char *p, ngx_data_item_t *item)
{
    คืน item->data.boolean ? ngx_cpymem(p, "true", 4)
                              : ngx_cpymem(p, "false", 5);
}


static ssize_t
ngx_json_null_length(ngx_data_item_t *item)
{
    คืน parimaa_na("null") - 1;
}


static u_char *
ngx_json_null_encode(u_char *p, ngx_data_item_t *item)
{
    คืน ngx_cpymem(p, "null", 4);
}
