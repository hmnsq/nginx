
/*
 * Copyright (C) Nginx, Inc.
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


ପ୍ରକାରବ୍ୟାଖ୍ୟା ssize_t   (*ngx_json_length_pt)(ngx_data_item_t *item);
ପ୍ରକାରବ୍ୟାଖ୍ୟା u_char   *(*ngx_json_encode_pt)(u_char *p, ngx_data_item_t *item);


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ngx_json_length_pt        length;
    ngx_json_encode_pt        encode;
} ngx_json_encode_t;


ସ୍ଥିର ssize_t ngx_json_obj_length(ngx_data_item_t *item);
ସ୍ଥିର ssize_t ngx_json_list_length(ngx_data_item_t *item);
ସ୍ଥିର ssize_t ngx_json_string_length(ngx_data_item_t *item);
ସ୍ଥିର ssize_t ngx_json_integer_length(ngx_data_item_t *item);
ସ୍ଥିର ssize_t ngx_json_boolean_length(ngx_data_item_t *item);
ସ୍ଥିର ssize_t ngx_json_null_length(ngx_data_item_t *item);

ସ୍ଥିର u_char *ngx_json_obj_encode(u_char *p, ngx_data_item_t *item);
ସ୍ଥିର u_char *ngx_json_list_encode(u_char *p, ngx_data_item_t *item);
ସ୍ଥିର u_char *ngx_json_string_encode(u_char *p, ngx_data_item_t *item);
ସ୍ଥିର u_char *ngx_json_integer_encode(u_char *p, ngx_data_item_t *item);
ସ୍ଥିର u_char *ngx_json_boolean_encode(u_char *p, ngx_data_item_t *item);
ସ୍ଥିର u_char *ngx_json_null_encode(u_char *p, ngx_data_item_t *item);


ସ୍ଥିର ngx_json_encode_t  ngx_json_encode[] = {

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
    ଯଦି (length == (size_t) NGX_ERROR) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    buf = ngx_create_temp_buf(pool, length);
    ଯଦି (buf == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    buf->last = ngx_json_encode[item->type].encode(buf->last, item);

    ଫେରନ୍ତୁ buf;
}


ସ୍ଥିର ssize_t
ngx_json_obj_length(ngx_data_item_t *item)
{
    size_t            len, total;
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    total = ଆକାର("{}") - 1;
    i = obj->item;

    ଯଦି (i) {
        ପାଇଁ ( ;; ) {
            ଯଦି (i->name.len > NGX_MAX_SIZE_T_ମୂଲ୍ୟ - (ଆକାର("\"\":") - 1)) {
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            len = ଆକାର("\"\":") - 1 + i->name.len;

            ଯଦି (len > NGX_MAX_SIZE_T_ମୂଲ୍ୟ - total) {
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            total += len;

            len = ngx_escape_json(ଖାଲି, i->name.data, i->name.len);

            ଯଦି (len > NGX_MAX_SIZE_T_ମୂଲ୍ୟ - total) {
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            total += len;

            len = ngx_json_encode[i->type].length(i);
            ଯଦି (len == (size_t) NGX_ERROR) {
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            ଯଦି (len > NGX_MAX_SIZE_T_ମୂଲ୍ୟ - total) {
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            total += len;

            i = i->next;

            ଯଦି (i == ଖାଲି) {
                ଭାଙ୍ଗ;
            }

            ଯଦି (total > NGX_MAX_SIZE_T_ମୂଲ୍ୟ - (ଆକାର(",") - 1)) {
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            total += ଆକାର(",") - 1;
        }
    }

    ଫେରନ୍ତୁ total;
}


ସ୍ଥିର u_char *
ngx_json_obj_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    i = obj->item;

    *p++ = '{';

    ଯଦି (i) {
        ପାଇଁ ( ;; ) {
            *p++ = '"';

            p = (u_char *) ngx_escape_json(p, i->name.data, i->name.len);

            *p++ = '"';
            *p++ = ':';

            p = ngx_json_encode[i->type].encode(p, i);

            i = i->next;

            ଯଦି (i == ଖାଲି) {
                ଭାଙ୍ଗ;
            }

            *p++ = ',';
        }
    }

    *p++ = '}';

    ଫେରନ୍ତୁ p;
}


ସ୍ଥିର ssize_t
ngx_json_list_length(ngx_data_item_t *item)
{
    size_t            len, total;
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    total = ଆକାର("[]") - 1;
    i = obj->item;

    ଯଦି (i) {
        ପାଇଁ ( ;; ) {
            len = ngx_json_encode[i->type].length(i);
            ଯଦି (len == (size_t) NGX_ERROR) {
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            ଯଦି (len > NGX_MAX_SIZE_T_ମୂଲ୍ୟ - total) {
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            total += len;

            i = i->next;

            ଯଦି (i == ଖାଲି) {
                ଭାଙ୍ଗ;
            }

            ଯଦି (total > NGX_MAX_SIZE_T_ମୂଲ୍ୟ - (ଆକାର(",") - 1)) {
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            total += ଆକାର(",") - 1;
        }
    }

    ଫେରନ୍ତୁ total;
}


ସ୍ଥିର u_char *
ngx_json_list_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    i = obj->item;

    *p++ = '[';

    ଯଦି (i) {
        ପାଇଁ ( ;; ) {
            p = ngx_json_encode[i->type].encode(p, i);

            i = i->next;

            ଯଦି (i == ଖାଲି) {
                ଭାଙ୍ଗ;
            }

            *p++ = ',';
        }
    }

    *p++ = ']';

    ଫେରନ୍ତୁ p;
}


ସ୍ଥିର ssize_t
ngx_json_string_length(ngx_data_item_t *item)
{
    size_t      len, total;
    ngx_str_t  *str;

    str = &item->data.string;

    ଯଦି (str->len > NGX_MAX_SIZE_T_ମୂଲ୍ୟ - (ଆକାର("\"\"") - 1)) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    total = ଆକାର("\"\"") - 1 + str->len;

    len = ngx_escape_json(ଖାଲି, str->data, str->len);

    ଯଦି (len > NGX_MAX_SIZE_T_ମୂଲ୍ୟ - total) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ଫେରନ୍ତୁ total + len;
}


ସ୍ଥିର u_char *
ngx_json_string_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_str_t  *str;

    str = &item->data.string;

    *p++ = '"';

    p = (u_char *) ngx_escape_json(p, str->data, str->len);

    *p++ = '"';

    ଫେରନ୍ତୁ p;
}


ସ୍ଥିର ssize_t
ngx_json_integer_length(ngx_data_item_t *item)
{
    ଫେରନ୍ତୁ NGX_INT64_LEN;
}


ସ୍ଥିର u_char *
ngx_json_integer_encode(u_char *p, ngx_data_item_t *item)
{
    ଫେରନ୍ତୁ ngx_sprintf(p, "%L", item->data.integer);
}


ସ୍ଥିର ssize_t
ngx_json_boolean_length(ngx_data_item_t *item)
{
    ଫେରନ୍ତୁ ଆକାର("false") - 1;
}


ସ୍ଥିର u_char *
ngx_json_boolean_encode(u_char *p, ngx_data_item_t *item)
{
    ଫେରନ୍ତୁ item->data.boolean ? ngx_cpymem(p, "true", 4)
                              : ngx_cpymem(p, "false", 5);
}


ସ୍ଥିର ssize_t
ngx_json_null_length(ngx_data_item_t *item)
{
    ଫେରନ୍ତୁ ଆକାର("null") - 1;
}


ସ୍ଥିର u_char *
ngx_json_null_encode(u_char *p, ngx_data_item_t *item)
{
    ଫେରନ୍ତୁ ngx_cpymem(p, "null", 4);
}
