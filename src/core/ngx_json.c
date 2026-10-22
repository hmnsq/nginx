
/*
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಪ್ರಕಾರನಿರ್ದೇಶ ssize_t   (*ngx_json_length_pt)(ngx_data_item_t *item);
ಪ್ರಕಾರನಿರ್ದೇಶ u_char   *(*ngx_json_encode_pt)(u_char *p, ngx_data_item_t *item);


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ngx_json_length_pt        length;
    ngx_json_encode_pt        encode;
} ngx_json_encode_t;


ಸ್ಥಿರ ssize_t ngx_json_obj_length(ngx_data_item_t *item);
ಸ್ಥಿರ ssize_t ngx_json_list_length(ngx_data_item_t *item);
ಸ್ಥಿರ ssize_t ngx_json_string_length(ngx_data_item_t *item);
ಸ್ಥಿರ ssize_t ngx_json_integer_length(ngx_data_item_t *item);
ಸ್ಥಿರ ssize_t ngx_json_boolean_length(ngx_data_item_t *item);
ಸ್ಥಿರ ssize_t ngx_json_null_length(ngx_data_item_t *item);

ಸ್ಥಿರ u_char *ngx_json_obj_encode(u_char *p, ngx_data_item_t *item);
ಸ್ಥಿರ u_char *ngx_json_list_encode(u_char *p, ngx_data_item_t *item);
ಸ್ಥಿರ u_char *ngx_json_string_encode(u_char *p, ngx_data_item_t *item);
ಸ್ಥಿರ u_char *ngx_json_integer_encode(u_char *p, ngx_data_item_t *item);
ಸ್ಥಿರ u_char *ngx_json_boolean_encode(u_char *p, ngx_data_item_t *item);
ಸ್ಥಿರ u_char *ngx_json_null_encode(u_char *p, ngx_data_item_t *item);


ಸ್ಥಿರ ngx_json_encode_t  ngx_json_encode[] = {

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
    ಯದಿ (length == (size_t) NGX_ERROR) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    buf = ngx_create_temp_buf(pool, length);
    ಯದಿ (buf == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    buf->last = ngx_json_encode[item->type].encode(buf->last, item);

    ಪ್ರತ್ಯಾಗಮ buf;
}


ಸ್ಥಿರ ssize_t
ngx_json_obj_length(ngx_data_item_t *item)
{
    size_t            len, total;
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    total = ಪರಿಮಾಣ("{}") - 1;
    i = obj->item;

    ಯದಿ (i) {
        ಪ್ರತಿ ( ;; ) {
            ಯದಿ (i->name.len > NGX_MAX_SIZE_T_ಮೌಲ್ಯ - (ಪರಿಮಾಣ("\"\":") - 1)) {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            len = ಪರಿಮಾಣ("\"\":") - 1 + i->name.len;

            ಯದಿ (len > NGX_MAX_SIZE_T_ಮೌಲ್ಯ - total) {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            total += len;

            len = ngx_escape_json(ರಿಕ್ತ, i->name.data, i->name.len);

            ಯದಿ (len > NGX_MAX_SIZE_T_ಮೌಲ್ಯ - total) {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            total += len;

            len = ngx_json_encode[i->type].length(i);
            ಯದಿ (len == (size_t) NGX_ERROR) {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            ಯದಿ (len > NGX_MAX_SIZE_T_ಮೌಲ್ಯ - total) {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            total += len;

            i = i->next;

            ಯದಿ (i == ರಿಕ್ತ) {
                ಭಙ್ಗ;
            }

            ಯದಿ (total > NGX_MAX_SIZE_T_ಮೌಲ್ಯ - (ಪರಿಮಾಣ(",") - 1)) {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            total += ಪರಿಮಾಣ(",") - 1;
        }
    }

    ಪ್ರತ್ಯಾಗಮ total;
}


ಸ್ಥಿರ u_char *
ngx_json_obj_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    i = obj->item;

    *p++ = '{';

    ಯದಿ (i) {
        ಪ್ರತಿ ( ;; ) {
            *p++ = '"';

            p = (u_char *) ngx_escape_json(p, i->name.data, i->name.len);

            *p++ = '"';
            *p++ = ':';

            p = ngx_json_encode[i->type].encode(p, i);

            i = i->next;

            ಯದಿ (i == ರಿಕ್ತ) {
                ಭಙ್ಗ;
            }

            *p++ = ',';
        }
    }

    *p++ = '}';

    ಪ್ರತ್ಯಾಗಮ p;
}


ಸ್ಥಿರ ssize_t
ngx_json_list_length(ngx_data_item_t *item)
{
    size_t            len, total;
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    total = ಪರಿಮಾಣ("[]") - 1;
    i = obj->item;

    ಯದಿ (i) {
        ಪ್ರತಿ ( ;; ) {
            len = ngx_json_encode[i->type].length(i);
            ಯದಿ (len == (size_t) NGX_ERROR) {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            ಯದಿ (len > NGX_MAX_SIZE_T_ಮೌಲ್ಯ - total) {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            total += len;

            i = i->next;

            ಯದಿ (i == ರಿಕ್ತ) {
                ಭಙ್ಗ;
            }

            ಯದಿ (total > NGX_MAX_SIZE_T_ಮೌಲ್ಯ - (ಪರಿಮಾಣ(",") - 1)) {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            total += ಪರಿಮಾಣ(",") - 1;
        }
    }

    ಪ್ರತ್ಯಾಗಮ total;
}


ಸ್ಥಿರ u_char *
ngx_json_list_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    i = obj->item;

    *p++ = '[';

    ಯದಿ (i) {
        ಪ್ರತಿ ( ;; ) {
            p = ngx_json_encode[i->type].encode(p, i);

            i = i->next;

            ಯದಿ (i == ರಿಕ್ತ) {
                ಭಙ್ಗ;
            }

            *p++ = ',';
        }
    }

    *p++ = ']';

    ಪ್ರತ್ಯಾಗಮ p;
}


ಸ್ಥಿರ ssize_t
ngx_json_string_length(ngx_data_item_t *item)
{
    size_t      len, total;
    ngx_str_t  *str;

    str = &item->data.string;

    ಯದಿ (str->len > NGX_MAX_SIZE_T_ಮೌಲ್ಯ - (ಪರಿಮಾಣ("\"\"") - 1)) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    total = ಪರಿಮಾಣ("\"\"") - 1 + str->len;

    len = ngx_escape_json(ರಿಕ್ತ, str->data, str->len);

    ಯದಿ (len > NGX_MAX_SIZE_T_ಮೌಲ್ಯ - total) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಪ್ರತ್ಯಾಗಮ total + len;
}


ಸ್ಥಿರ u_char *
ngx_json_string_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_str_t  *str;

    str = &item->data.string;

    *p++ = '"';

    p = (u_char *) ngx_escape_json(p, str->data, str->len);

    *p++ = '"';

    ಪ್ರತ್ಯಾಗಮ p;
}


ಸ್ಥಿರ ssize_t
ngx_json_integer_length(ngx_data_item_t *item)
{
    ಪ್ರತ್ಯಾಗಮ NGX_INT64_LEN;
}


ಸ್ಥಿರ u_char *
ngx_json_integer_encode(u_char *p, ngx_data_item_t *item)
{
    ಪ್ರತ್ಯಾಗಮ ngx_sprintf(p, "%L", item->data.integer);
}


ಸ್ಥಿರ ssize_t
ngx_json_boolean_length(ngx_data_item_t *item)
{
    ಪ್ರತ್ಯಾಗಮ ಪರಿಮಾಣ("false") - 1;
}


ಸ್ಥಿರ u_char *
ngx_json_boolean_encode(u_char *p, ngx_data_item_t *item)
{
    ಪ್ರತ್ಯಾಗಮ item->data.boolean ? ngx_cpymem(p, "true", 4)
                              : ngx_cpymem(p, "false", 5);
}


ಸ್ಥಿರ ssize_t
ngx_json_null_length(ngx_data_item_t *item)
{
    ಪ್ರತ್ಯಾಗಮ ಪರಿಮಾಣ("null") - 1;
}


ಸ್ಥಿರ u_char *
ngx_json_null_encode(u_char *p, ngx_data_item_t *item)
{
    ಪ್ರತ್ಯಾಗಮ ngx_cpymem(p, "null", 4);
}
