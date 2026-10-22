
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
    ᰀᰧ (length == (size_t) NGX_ERROR) {
        ᰀᰧᰌ NULL;
    }

    buf = ngx_create_temp_buf(pool, length);
    ᰀᰧ (buf == NULL) {
        ᰀᰧᰌ NULL;
    }

    buf->last = ngx_json_encode[item->type].encode(buf->last, item);

    ᰀᰧᰌ buf;
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

    ᰀᰧ (i) {
        for ( ;; ) {
            ᰀᰧ (i->name.len > NGX_MAX_SIZE_T_VALUE - (parimaa_na("\"\":") - 1)) {
                ᰀᰧᰌ NGX_ERROR;
            }

            len = parimaa_na("\"\":") - 1 + i->name.len;

            ᰀᰧ (len > NGX_MAX_SIZE_T_VALUE - total) {
                ᰀᰧᰌ NGX_ERROR;
            }

            total += len;

            len = ngx_escape_json(NULL, i->name.data, i->name.len);

            ᰀᰧ (len > NGX_MAX_SIZE_T_VALUE - total) {
                ᰀᰧᰌ NGX_ERROR;
            }

            total += len;

            len = ngx_json_encode[i->type].length(i);
            ᰀᰧ (len == (size_t) NGX_ERROR) {
                ᰀᰧᰌ NGX_ERROR;
            }

            ᰀᰧ (len > NGX_MAX_SIZE_T_VALUE - total) {
                ᰀᰧᰌ NGX_ERROR;
            }

            total += len;

            i = i->next;

            ᰀᰧ (i == NULL) {
                bha_nga;
            }

            ᰀᰧ (total > NGX_MAX_SIZE_T_VALUE - (parimaa_na(",") - 1)) {
                ᰀᰧᰌ NGX_ERROR;
            }

            total += parimaa_na(",") - 1;
        }
    }

    ᰀᰧᰌ total;
}


static u_char *
ngx_json_obj_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    i = obj->item;

    *p++ = '{';

    ᰀᰧ (i) {
        for ( ;; ) {
            *p++ = '"';

            p = (u_char *) ngx_escape_json(p, i->name.data, i->name.len);

            *p++ = '"';
            *p++ = ':';

            p = ngx_json_encode[i->type].encode(p, i);

            i = i->next;

            ᰀᰧ (i == NULL) {
                bha_nga;
            }

            *p++ = ',';
        }
    }

    *p++ = '}';

    ᰀᰧᰌ p;
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

    ᰀᰧ (i) {
        for ( ;; ) {
            len = ngx_json_encode[i->type].length(i);
            ᰀᰧ (len == (size_t) NGX_ERROR) {
                ᰀᰧᰌ NGX_ERROR;
            }

            ᰀᰧ (len > NGX_MAX_SIZE_T_VALUE - total) {
                ᰀᰧᰌ NGX_ERROR;
            }

            total += len;

            i = i->next;

            ᰀᰧ (i == NULL) {
                bha_nga;
            }

            ᰀᰧ (total > NGX_MAX_SIZE_T_VALUE - (parimaa_na(",") - 1)) {
                ᰀᰧᰌ NGX_ERROR;
            }

            total += parimaa_na(",") - 1;
        }
    }

    ᰀᰧᰌ total;
}


static u_char *
ngx_json_list_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    i = obj->item;

    *p++ = '[';

    ᰀᰧ (i) {
        for ( ;; ) {
            p = ngx_json_encode[i->type].encode(p, i);

            i = i->next;

            ᰀᰧ (i == NULL) {
                bha_nga;
            }

            *p++ = ',';
        }
    }

    *p++ = ']';

    ᰀᰧᰌ p;
}


static ssize_t
ngx_json_string_length(ngx_data_item_t *item)
{
    size_t      len, total;
    ngx_str_t  *str;

    str = &item->data.string;

    ᰀᰧ (str->len > NGX_MAX_SIZE_T_VALUE - (parimaa_na("\"\"") - 1)) {
        ᰀᰧᰌ NGX_ERROR;
    }

    total = parimaa_na("\"\"") - 1 + str->len;

    len = ngx_escape_json(NULL, str->data, str->len);

    ᰀᰧ (len > NGX_MAX_SIZE_T_VALUE - total) {
        ᰀᰧᰌ NGX_ERROR;
    }

    ᰀᰧᰌ total + len;
}


static u_char *
ngx_json_string_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_str_t  *str;

    str = &item->data.string;

    *p++ = '"';

    p = (u_char *) ngx_escape_json(p, str->data, str->len);

    *p++ = '"';

    ᰀᰧᰌ p;
}


static ssize_t
ngx_json_integer_length(ngx_data_item_t *item)
{
    ᰀᰧᰌ NGX_INT64_LEN;
}


static u_char *
ngx_json_integer_encode(u_char *p, ngx_data_item_t *item)
{
    ᰀᰧᰌ ngx_sprintf(p, "%L", item->data.integer);
}


static ssize_t
ngx_json_boolean_length(ngx_data_item_t *item)
{
    ᰀᰧᰌ parimaa_na("false") - 1;
}


static u_char *
ngx_json_boolean_encode(u_char *p, ngx_data_item_t *item)
{
    ᰀᰧᰌ item->data.boolean ? ngx_cpymem(p, "true", 4)
                              : ngx_cpymem(p, "false", 5);
}


static ssize_t
ngx_json_null_length(ngx_data_item_t *item)
{
    ᰀᰧᰌ parimaa_na("null") - 1;
}


static u_char *
ngx_json_null_encode(u_char *p, ngx_data_item_t *item)
{
    ᰀᰧᰌ ngx_cpymem(p, "null", 4);
}
