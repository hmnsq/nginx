
/*
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


प्रकारव्याख्या ssize_t   (*ngx_json_length_pt)(ngx_data_item_t *item);
प्रकारव्याख्या u_char   *(*ngx_json_encode_pt)(u_char *p, ngx_data_item_t *item);


प्रकारव्याख्या संरचना {
    ngx_json_length_pt        length;
    ngx_json_encode_pt        encode;
} ngx_json_encode_t;


स्थिर ssize_t ngx_json_obj_length(ngx_data_item_t *item);
स्थिर ssize_t ngx_json_list_length(ngx_data_item_t *item);
स्थिर ssize_t ngx_json_string_length(ngx_data_item_t *item);
स्थिर ssize_t ngx_json_integer_length(ngx_data_item_t *item);
स्थिर ssize_t ngx_json_boolean_length(ngx_data_item_t *item);
स्थिर ssize_t ngx_json_null_length(ngx_data_item_t *item);

स्थिर u_char *ngx_json_obj_encode(u_char *p, ngx_data_item_t *item);
स्थिर u_char *ngx_json_list_encode(u_char *p, ngx_data_item_t *item);
स्थिर u_char *ngx_json_string_encode(u_char *p, ngx_data_item_t *item);
स्थिर u_char *ngx_json_integer_encode(u_char *p, ngx_data_item_t *item);
स्थिर u_char *ngx_json_boolean_encode(u_char *p, ngx_data_item_t *item);
स्थिर u_char *ngx_json_null_encode(u_char *p, ngx_data_item_t *item);


स्थिर ngx_json_encode_t  ngx_json_encode[] = {

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
    माप_प्रकार      length;
    ngx_buf_t  *buf;

    length = ngx_json_encode[item->type].length(item);
    जर (length == (माप_प्रकार) NGX_ERROR) {
        परत रिक्त;
    }

    buf = ngx_create_temp_buf(pool, length);
    जर (buf == रिक्त) {
        परत रिक्त;
    }

    buf->last = ngx_json_encode[item->type].encode(buf->last, item);

    परत buf;
}


स्थिर ssize_t
ngx_json_obj_length(ngx_data_item_t *item)
{
    माप_प्रकार            len, total;
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    total = आकार("{}") - 1;
    i = obj->item;

    जर (i) {
        साठी ( ;; ) {
            जर (i->name.len > NGX_MAX_SIZE_T_मूल्य - (आकार("\"\":") - 1)) {
                परत NGX_ERROR;
            }

            len = आकार("\"\":") - 1 + i->name.len;

            जर (len > NGX_MAX_SIZE_T_मूल्य - total) {
                परत NGX_ERROR;
            }

            total += len;

            len = ngx_escape_json(रिक्त, i->name.data, i->name.len);

            जर (len > NGX_MAX_SIZE_T_मूल्य - total) {
                परत NGX_ERROR;
            }

            total += len;

            len = ngx_json_encode[i->type].length(i);
            जर (len == (माप_प्रकार) NGX_ERROR) {
                परत NGX_ERROR;
            }

            जर (len > NGX_MAX_SIZE_T_मूल्य - total) {
                परत NGX_ERROR;
            }

            total += len;

            i = i->next;

            जर (i == रिक्त) {
                थांबा;
            }

            जर (total > NGX_MAX_SIZE_T_मूल्य - (आकार(",") - 1)) {
                परत NGX_ERROR;
            }

            total += आकार(",") - 1;
        }
    }

    परत total;
}


स्थिर u_char *
ngx_json_obj_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    i = obj->item;

    *p++ = '{';

    जर (i) {
        साठी ( ;; ) {
            *p++ = '"';

            p = (u_char *) ngx_escape_json(p, i->name.data, i->name.len);

            *p++ = '"';
            *p++ = ':';

            p = ngx_json_encode[i->type].encode(p, i);

            i = i->next;

            जर (i == रिक्त) {
                थांबा;
            }

            *p++ = ',';
        }
    }

    *p++ = '}';

    परत p;
}


स्थिर ssize_t
ngx_json_list_length(ngx_data_item_t *item)
{
    माप_प्रकार            len, total;
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    total = आकार("[]") - 1;
    i = obj->item;

    जर (i) {
        साठी ( ;; ) {
            len = ngx_json_encode[i->type].length(i);
            जर (len == (माप_प्रकार) NGX_ERROR) {
                परत NGX_ERROR;
            }

            जर (len > NGX_MAX_SIZE_T_मूल्य - total) {
                परत NGX_ERROR;
            }

            total += len;

            i = i->next;

            जर (i == रिक्त) {
                थांबा;
            }

            जर (total > NGX_MAX_SIZE_T_मूल्य - (आकार(",") - 1)) {
                परत NGX_ERROR;
            }

            total += आकार(",") - 1;
        }
    }

    परत total;
}


स्थिर u_char *
ngx_json_list_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_data_obj_t   *obj;
    ngx_data_item_t  *i;

    obj = &item->data.object;
    i = obj->item;

    *p++ = '[';

    जर (i) {
        साठी ( ;; ) {
            p = ngx_json_encode[i->type].encode(p, i);

            i = i->next;

            जर (i == रिक्त) {
                थांबा;
            }

            *p++ = ',';
        }
    }

    *p++ = ']';

    परत p;
}


स्थिर ssize_t
ngx_json_string_length(ngx_data_item_t *item)
{
    माप_प्रकार      len, total;
    ngx_str_t  *str;

    str = &item->data.string;

    जर (str->len > NGX_MAX_SIZE_T_मूल्य - (आकार("\"\"") - 1)) {
        परत NGX_ERROR;
    }

    total = आकार("\"\"") - 1 + str->len;

    len = ngx_escape_json(रिक्त, str->data, str->len);

    जर (len > NGX_MAX_SIZE_T_मूल्य - total) {
        परत NGX_ERROR;
    }

    परत total + len;
}


स्थिर u_char *
ngx_json_string_encode(u_char *p, ngx_data_item_t *item)
{
    ngx_str_t  *str;

    str = &item->data.string;

    *p++ = '"';

    p = (u_char *) ngx_escape_json(p, str->data, str->len);

    *p++ = '"';

    परत p;
}


स्थिर ssize_t
ngx_json_integer_length(ngx_data_item_t *item)
{
    परत NGX_INT64_LEN;
}


स्थिर u_char *
ngx_json_integer_encode(u_char *p, ngx_data_item_t *item)
{
    परत ngx_sprintf(p, "%L", item->data.integer);
}


स्थिर ssize_t
ngx_json_boolean_length(ngx_data_item_t *item)
{
    परत आकार("false") - 1;
}


स्थिर u_char *
ngx_json_boolean_encode(u_char *p, ngx_data_item_t *item)
{
    परत item->data.boolean ? ngx_cpymem(p, "true", 4)
                              : ngx_cpymem(p, "false", 5);
}


स्थिर ssize_t
ngx_json_null_length(ngx_data_item_t *item)
{
    परत आकार("null") - 1;
}


स्थिर u_char *
ngx_json_null_encode(u_char *p, ngx_data_item_t *item)
{
    परत ngx_cpymem(p, "null", 4);
}
