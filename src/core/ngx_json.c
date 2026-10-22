
/*
 * Copyright (C) Nginx, Inc.
 */


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>


வகைவரை ssize_t   (*ngx_json_length_pt)(ngx_data_item_t *item);
வகைவரை u_char   *(*ngx_json_encode_pt)(u_char *p, ngx_data_item_t *item);


வகைவரை கட்டமைப்பு {
    ngx_json_length_pt        length;
    ngx_json_encode_pt        encode;
} ngx_json_encode_t;


நிலையான ssize_t ngx_json_obj_length(ngx_data_item_t *item);
நிலையான ssize_t ngx_json_list_length(ngx_data_item_t *item);
நிலையான ssize_t ngx_json_string_length(ngx_data_item_t *item);
நிலையான ssize_t ngx_json_integer_length(ngx_data_item_t *item);
நிலையான ssize_t ngx_json_boolean_length(ngx_data_item_t *item);
நிலையான ssize_t ngx_json_null_length(ngx_data_item_t *item);

நிலையான u_char *ngx_json_obj_encode(u_char *p, ngx_data_item_t *item);
நிலையான u_char *ngx_json_list_encode(u_char *p, ngx_data_item_t *item);
நிலையான u_char *ngx_json_string_encode(u_char *p, ngx_data_item_t *item);
நிலையான u_char *ngx_json_integer_encode(u_char *p, ngx_data_item_t *item);
நிலையான u_char *ngx_json_boolean_encode(u_char *p, ngx_data_item_t *item);
நிலையான u_char *ngx_json_null_encode(u_char *p, ngx_data_item_t *item);


நிலையான ngx_json_encode_t  ngx_json_encode[] = {

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
    எனில் (length == (size_t) NGX_ERROR) {
        திரும்பு வெற்று;
    }

    buf = ngx_create_temp_buf(pool, length);
    எனில் (buf == வெற்று) {
        திரும்பு வெற்று;
    }

    buf->last = ngx_json_encode[item->type].encode(buf->last, item);

    திரும்பு buf;
}


நிலையான ssize_t
ngx_json_obj_length(ngx_data_item_t *item)
{
    size_t            len, total;
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    total = அளவு("{}") - 1;
    i = obj->item;

    எனில் (i) {
        க்கு ( ;; ) {
            எனில் (i->name.len > NGX_MAX_SIZE_T_மதிப்பு - (அளவு("\"\":") - 1)) {
                திரும்பு NGX_ERROR;
            }

            len = அளவு("\"\":") - 1 + i->name.len;

            எனில் (len > NGX_MAX_SIZE_T_மதிப்பு - total) {
                திரும்பு NGX_ERROR;
            }

            total += len;

            len = ngx_escape_json(வெற்று, i->name.data, i->name.len);

            எனில் (len > NGX_MAX_SIZE_T_மதிப்பு - total) {
                திரும்பு NGX_ERROR;
            }

            total += len;

            len = ngx_json_encode[i->type].length(i);
            எனில் (len == (size_t) NGX_ERROR) {
                திரும்பு NGX_ERROR;
            }

            எனில் (len > NGX_MAX_SIZE_T_மதிப்பு - total) {
                திரும்பு NGX_ERROR;
            }

            total += len;

            i = i->next;

            எனில் (i == வெற்று) {
                நிறுத்து;
            }

            எனில் (total > NGX_MAX_SIZE_T_மதிப்பு - (அளவு(",") - 1)) {
                திரும்பு NGX_ERROR;
            }

            total += அளவு(",") - 1;
        }
    }

    திரும்பு total;
}


நிலையான u_char *
ngx_json_obj_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    i = obj->item;

    *p++ = '{';

    எனில் (i) {
        க்கு ( ;; ) {
            *p++ = '"';

            p = (u_char *) ngx_escape_json(p, i->name.data, i->name.len);

            *p++ = '"';
            *p++ = ':';

            p = ngx_json_encode[i->type].encode(p, i);

            i = i->next;

            எனில் (i == வெற்று) {
                நிறுத்து;
            }

            *p++ = ',';
        }
    }

    *p++ = '}';

    திரும்பு p;
}


நிலையான ssize_t
ngx_json_list_length(ngx_data_item_t *item)
{
    size_t            len, total;
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    total = அளவு("[]") - 1;
    i = obj->item;

    எனில் (i) {
        க்கு ( ;; ) {
            len = ngx_json_encode[i->type].length(i);
            எனில் (len == (size_t) NGX_ERROR) {
                திரும்பு NGX_ERROR;
            }

            எனில் (len > NGX_MAX_SIZE_T_மதிப்பு - total) {
                திரும்பு NGX_ERROR;
            }

            total += len;

            i = i->next;

            எனில் (i == வெற்று) {
                நிறுத்து;
            }

            எனில் (total > NGX_MAX_SIZE_T_மதிப்பு - (அளவு(",") - 1)) {
                திரும்பு NGX_ERROR;
            }

            total += அளவு(",") - 1;
        }
    }

    திரும்பு total;
}


நிலையான u_char *
ngx_json_list_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    i = obj->item;

    *p++ = '[';

    எனில் (i) {
        க்கு ( ;; ) {
            p = ngx_json_encode[i->type].encode(p, i);

            i = i->next;

            எனில் (i == வெற்று) {
                நிறுத்து;
            }

            *p++ = ',';
        }
    }

    *p++ = ']';

    திரும்பு p;
}


நிலையான ssize_t
ngx_json_string_length(ngx_data_item_t *item)
{
    size_t      len, total;
    ngx_str_t  *str;

    str = &item->data.string;

    எனில் (str->len > NGX_MAX_SIZE_T_மதிப்பு - (அளவு("\"\"") - 1)) {
        திரும்பு NGX_ERROR;
    }

    total = அளவு("\"\"") - 1 + str->len;

    len = ngx_escape_json(வெற்று, str->data, str->len);

    எனில் (len > NGX_MAX_SIZE_T_மதிப்பு - total) {
        திரும்பு NGX_ERROR;
    }

    திரும்பு total + len;
}


நிலையான u_char *
ngx_json_string_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_str_t  *str;

    str = &item->data.string;

    *p++ = '"';

    p = (u_char *) ngx_escape_json(p, str->data, str->len);

    *p++ = '"';

    திரும்பு p;
}


நிலையான ssize_t
ngx_json_integer_length(ngx_data_item_t *item)
{
    திரும்பு NGX_INT64_LEN;
}


நிலையான u_char *
ngx_json_integer_encode(u_char *p, ngx_data_item_t *item)
{
    திரும்பு ngx_sprintf(p, "%L", item->data.integer);
}


நிலையான ssize_t
ngx_json_boolean_length(ngx_data_item_t *item)
{
    திரும்பு அளவு("false") - 1;
}


நிலையான u_char *
ngx_json_boolean_encode(u_char *p, ngx_data_item_t *item)
{
    திரும்பு item->data.boolean ? ngx_cpymem(p, "true", 4)
                              : ngx_cpymem(p, "false", 5);
}


நிலையான ssize_t
ngx_json_null_length(ngx_data_item_t *item)
{
    திரும்பு அளவு("null") - 1;
}


நிலையான u_char *
ngx_json_null_encode(u_char *p, ngx_data_item_t *item)
{
    திரும்பு ngx_cpymem(p, "null", 4);
}
