
/*
 * Copyright (C) Nginx, Inc.
 */


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>


પ્રકારવ્યાખ્યા ssize_t   (*ngx_json_length_pt)(ngx_data_item_t *item);
પ્રકારવ્યાખ્યા u_char   *(*ngx_json_encode_pt)(u_char *p, ngx_data_item_t *item);


પ્રકારવ્યાખ્યા સંરચના {
    ngx_json_length_pt        length;
    ngx_json_encode_pt        encode;
} ngx_json_encode_t;


સ્થિર ssize_t ngx_json_obj_length(ngx_data_item_t *item);
સ્થિર ssize_t ngx_json_list_length(ngx_data_item_t *item);
સ્થિર ssize_t ngx_json_string_length(ngx_data_item_t *item);
સ્થિર ssize_t ngx_json_integer_length(ngx_data_item_t *item);
સ્થિર ssize_t ngx_json_boolean_length(ngx_data_item_t *item);
સ્થિર ssize_t ngx_json_null_length(ngx_data_item_t *item);

સ્થિર u_char *ngx_json_obj_encode(u_char *p, ngx_data_item_t *item);
સ્થિર u_char *ngx_json_list_encode(u_char *p, ngx_data_item_t *item);
સ્થિર u_char *ngx_json_string_encode(u_char *p, ngx_data_item_t *item);
સ્થિર u_char *ngx_json_integer_encode(u_char *p, ngx_data_item_t *item);
સ્થિર u_char *ngx_json_boolean_encode(u_char *p, ngx_data_item_t *item);
સ્થિર u_char *ngx_json_null_encode(u_char *p, ngx_data_item_t *item);


સ્થિર ngx_json_encode_t  ngx_json_encode[] = {

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
    જો (length == (size_t) NGX_ERROR) {
        પાછા ખાલી;
    }

    buf = ngx_create_temp_buf(pool, length);
    જો (buf == ખાલી) {
        પાછા ખાલી;
    }

    buf->last = ngx_json_encode[item->type].encode(buf->last, item);

    પાછા buf;
}


સ્થિર ssize_t
ngx_json_obj_length(ngx_data_item_t *item)
{
    size_t            len, total;
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    total = કદ("{}") - 1;
    i = obj->item;

    જો (i) {
        માટે ( ;; ) {
            જો (i->name.len > NGX_MAX_SIZE_T_કિંમત - (કદ("\"\":") - 1)) {
                પાછા NGX_ERROR;
            }

            len = કદ("\"\":") - 1 + i->name.len;

            જો (len > NGX_MAX_SIZE_T_કિંમત - total) {
                પાછા NGX_ERROR;
            }

            total += len;

            len = ngx_escape_json(ખાલી, i->name.data, i->name.len);

            જો (len > NGX_MAX_SIZE_T_કિંમત - total) {
                પાછા NGX_ERROR;
            }

            total += len;

            len = ngx_json_encode[i->type].length(i);
            જો (len == (size_t) NGX_ERROR) {
                પાછા NGX_ERROR;
            }

            જો (len > NGX_MAX_SIZE_T_કિંમત - total) {
                પાછા NGX_ERROR;
            }

            total += len;

            i = i->next;

            જો (i == ખાલી) {
                તોડો;
            }

            જો (total > NGX_MAX_SIZE_T_કિંમત - (કદ(",") - 1)) {
                પાછા NGX_ERROR;
            }

            total += કદ(",") - 1;
        }
    }

    પાછા total;
}


સ્થિર u_char *
ngx_json_obj_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    i = obj->item;

    *p++ = '{';

    જો (i) {
        માટે ( ;; ) {
            *p++ = '"';

            p = (u_char *) ngx_escape_json(p, i->name.data, i->name.len);

            *p++ = '"';
            *p++ = ':';

            p = ngx_json_encode[i->type].encode(p, i);

            i = i->next;

            જો (i == ખાલી) {
                તોડો;
            }

            *p++ = ',';
        }
    }

    *p++ = '}';

    પાછા p;
}


સ્થિર ssize_t
ngx_json_list_length(ngx_data_item_t *item)
{
    size_t            len, total;
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    total = કદ("[]") - 1;
    i = obj->item;

    જો (i) {
        માટે ( ;; ) {
            len = ngx_json_encode[i->type].length(i);
            જો (len == (size_t) NGX_ERROR) {
                પાછા NGX_ERROR;
            }

            જો (len > NGX_MAX_SIZE_T_કિંમત - total) {
                પાછા NGX_ERROR;
            }

            total += len;

            i = i->next;

            જો (i == ખાલી) {
                તોડો;
            }

            જો (total > NGX_MAX_SIZE_T_કિંમત - (કદ(",") - 1)) {
                પાછા NGX_ERROR;
            }

            total += કદ(",") - 1;
        }
    }

    પાછા total;
}


સ્થિર u_char *
ngx_json_list_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    i = obj->item;

    *p++ = '[';

    જો (i) {
        માટે ( ;; ) {
            p = ngx_json_encode[i->type].encode(p, i);

            i = i->next;

            જો (i == ખાલી) {
                તોડો;
            }

            *p++ = ',';
        }
    }

    *p++ = ']';

    પાછા p;
}


સ્થિર ssize_t
ngx_json_string_length(ngx_data_item_t *item)
{
    size_t      len, total;
    ngx_str_t  *str;

    str = &item->data.string;

    જો (str->len > NGX_MAX_SIZE_T_કિંમત - (કદ("\"\"") - 1)) {
        પાછા NGX_ERROR;
    }

    total = કદ("\"\"") - 1 + str->len;

    len = ngx_escape_json(ખાલી, str->data, str->len);

    જો (len > NGX_MAX_SIZE_T_કિંમત - total) {
        પાછા NGX_ERROR;
    }

    પાછા total + len;
}


સ્થિર u_char *
ngx_json_string_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_str_t  *str;

    str = &item->data.string;

    *p++ = '"';

    p = (u_char *) ngx_escape_json(p, str->data, str->len);

    *p++ = '"';

    પાછા p;
}


સ્થિર ssize_t
ngx_json_integer_length(ngx_data_item_t *item)
{
    પાછા NGX_INT64_LEN;
}


સ્થિર u_char *
ngx_json_integer_encode(u_char *p, ngx_data_item_t *item)
{
    પાછા ngx_sprintf(p, "%L", item->data.integer);
}


સ્થિર ssize_t
ngx_json_boolean_length(ngx_data_item_t *item)
{
    પાછા કદ("false") - 1;
}


સ્થિર u_char *
ngx_json_boolean_encode(u_char *p, ngx_data_item_t *item)
{
    પાછા item->data.boolean ? ngx_cpymem(p, "true", 4)
                              : ngx_cpymem(p, "false", 5);
}


સ્થિર ssize_t
ngx_json_null_length(ngx_data_item_t *item)
{
    પાછા કદ("null") - 1;
}


સ્થિર u_char *
ngx_json_null_encode(u_char *p, ngx_data_item_t *item)
{
    પાછા ngx_cpymem(p, "null", 4);
}
