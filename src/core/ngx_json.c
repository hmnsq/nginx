
/*
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


प्रकारनिर्देश ssize_t   (*ngx_json_दीर्घता_pt)(ngx_दत्त_item_t *item);
प्रकारनिर्देश u_char   *(*ngx_json_encode_pt)(u_char *p, ngx_दत्त_item_t *item);


प्रकारनिर्देश संरचना {
    ngx_json_दीर्घता_pt        दीर्घता;
    ngx_json_encode_pt        encode;
} ngx_json_encode_t;


स्थिर ssize_t ngx_json_obj_दीर्घता(ngx_दत्त_item_t *item);
स्थिर ssize_t ngx_json_list_दीर्घता(ngx_दत्त_item_t *item);
स्थिर ssize_t ngx_json_string_दीर्घता(ngx_दत्त_item_t *item);
स्थिर ssize_t ngx_json_integer_दीर्घता(ngx_दत्त_item_t *item);
स्थिर ssize_t ngx_json_boolean_दीर्घता(ngx_दत्त_item_t *item);
स्थिर ssize_t ngx_json_null_दीर्घता(ngx_दत्त_item_t *item);

स्थिर u_char *ngx_json_obj_encode(u_char *p, ngx_दत्त_item_t *item);
स्थिर u_char *ngx_json_list_encode(u_char *p, ngx_दत्त_item_t *item);
स्थिर u_char *ngx_json_string_encode(u_char *p, ngx_दत्त_item_t *item);
स्थिर u_char *ngx_json_integer_encode(u_char *p, ngx_दत्त_item_t *item);
स्थिर u_char *ngx_json_boolean_encode(u_char *p, ngx_दत्त_item_t *item);
स्थिर u_char *ngx_json_null_encode(u_char *p, ngx_दत्त_item_t *item);


स्थिर ngx_json_encode_t  ngx_json_encode[] = {

    /* NGX_DATA_OBJECT_TYPE */
    { ngx_json_obj_दीर्घता,
      ngx_json_obj_encode },

    /* NGX_DATA_LIST_TYPE */
    { ngx_json_list_दीर्घता,
      ngx_json_list_encode },

    /* NGX_DATA_STRING_TYPE */
    { ngx_json_string_दीर्घता,
      ngx_json_string_encode },

    /* NGX_DATA_INTEGER_TYPE */
    { ngx_json_integer_दीर्घता,
      ngx_json_integer_encode },

    /* NGX_DATA_BOOLEAN_TYPE */
    { ngx_json_boolean_दीर्घता,
      ngx_json_boolean_encode },

    /* NGX_DATA_NULL_TYPE */
    { ngx_json_null_दीर्घता,
      ngx_json_null_encode },
};


ngx_buf_t *
ngx_json_render(ngx_pool_t *pool, ngx_दत्त_item_t *item)
{
    आकार_t      दीर्घता;
    ngx_buf_t  *buf;

    दीर्घता = ngx_json_encode[item->type].दीर्घता(item);
    यदि (दीर्घता == (आकार_t) NGX_ERROR) {
        प्रत्यागम रिक्त;
    }

    buf = ngx_create_अस्थायी_buf(pool, दीर्घता);
    यदि (buf == रिक्त) {
        प्रत्यागम रिक्त;
    }

    buf->चरमः = ngx_json_encode[item->type].encode(buf->चरमः, item);

    प्रत्यागम buf;
}


स्थिर ssize_t
ngx_json_obj_दीर्घता(ngx_दत्त_item_t *item)
{
    आकार_t            len, योग;
    ngx_दत्त_obj_t   *obj;
    ngx_दत्त_item_t  *i;

    obj = &item->दत्त.object;
    योग = परिमाण("{}") - 1;
    i = obj->item;

    यदि (i) {
        प्रति ( ;; ) {
            यदि (i->name.len > NGX_MAX_आकार_T_मानम् - (परिमाण("\"\":") - 1)) {
                प्रत्यागम NGX_ERROR;
            }

            len = परिमाण("\"\":") - 1 + i->name.len;

            यदि (len > NGX_MAX_आकार_T_मानम् - योग) {
                प्रत्यागम NGX_ERROR;
            }

            योग += len;

            len = ngx_escape_json(रिक्त, i->name.दत्त, i->name.len);

            यदि (len > NGX_MAX_आकार_T_मानम् - योग) {
                प्रत्यागम NGX_ERROR;
            }

            योग += len;

            len = ngx_json_encode[i->type].दीर्घता(i);
            यदि (len == (आकार_t) NGX_ERROR) {
                प्रत्यागम NGX_ERROR;
            }

            यदि (len > NGX_MAX_आकार_T_मानम् - योग) {
                प्रत्यागम NGX_ERROR;
            }

            योग += len;

            i = i->अग्र;

            यदि (i == रिक्त) {
                भङ्ग;
            }

            यदि (योग > NGX_MAX_आकार_T_मानम् - (परिमाण(",") - 1)) {
                प्रत्यागम NGX_ERROR;
            }

            योग += परिमाण(",") - 1;
        }
    }

    प्रत्यागम योग;
}


स्थिर u_char *
ngx_json_obj_encode(u_char *p, ngx_दत्त_item_t *item)
{
    ngx_दत्त_obj_t   *obj;
    ngx_दत्त_item_t  *i;

    obj = &item->दत्त.object;
    i = obj->item;

    *p++ = '{';

    यदि (i) {
        प्रति ( ;; ) {
            *p++ = '"';

            p = (u_char *) ngx_escape_json(p, i->name.दत्त, i->name.len);

            *p++ = '"';
            *p++ = ':';

            p = ngx_json_encode[i->type].encode(p, i);

            i = i->अग्र;

            यदि (i == रिक्त) {
                भङ्ग;
            }

            *p++ = ',';
        }
    }

    *p++ = '}';

    प्रत्यागम p;
}


स्थिर ssize_t
ngx_json_list_दीर्घता(ngx_दत्त_item_t *item)
{
    आकार_t            len, योग;
    ngx_दत्त_obj_t   *obj;
    ngx_दत्त_item_t  *i;

    obj = &item->दत्त.object;
    योग = परिमाण("[]") - 1;
    i = obj->item;

    यदि (i) {
        प्रति ( ;; ) {
            len = ngx_json_encode[i->type].दीर्घता(i);
            यदि (len == (आकार_t) NGX_ERROR) {
                प्रत्यागम NGX_ERROR;
            }

            यदि (len > NGX_MAX_आकार_T_मानम् - योग) {
                प्रत्यागम NGX_ERROR;
            }

            योग += len;

            i = i->अग्र;

            यदि (i == रिक्त) {
                भङ्ग;
            }

            यदि (योग > NGX_MAX_आकार_T_मानम् - (परिमाण(",") - 1)) {
                प्रत्यागम NGX_ERROR;
            }

            योग += परिमाण(",") - 1;
        }
    }

    प्रत्यागम योग;
}


स्थिर u_char *
ngx_json_list_encode(u_char *p, ngx_दत्त_item_t *item)
{
    ngx_दत्त_obj_t   *obj;
    ngx_दत्त_item_t  *i;

    obj = &item->दत्त.object;
    i = obj->item;

    *p++ = '[';

    यदि (i) {
        प्रति ( ;; ) {
            p = ngx_json_encode[i->type].encode(p, i);

            i = i->अग्र;

            यदि (i == रिक्त) {
                भङ्ग;
            }

            *p++ = ',';
        }
    }

    *p++ = ']';

    प्रत्यागम p;
}


स्थिर ssize_t
ngx_json_string_दीर्घता(ngx_दत्त_item_t *item)
{
    आकार_t      len, योग;
    ngx_str_t  *str;

    str = &item->दत्त.string;

    यदि (str->len > NGX_MAX_आकार_T_मानम् - (परिमाण("\"\"") - 1)) {
        प्रत्यागम NGX_ERROR;
    }

    योग = परिमाण("\"\"") - 1 + str->len;

    len = ngx_escape_json(रिक्त, str->दत्त, str->len);

    यदि (len > NGX_MAX_आकार_T_मानम् - योग) {
        प्रत्यागम NGX_ERROR;
    }

    प्रत्यागम योग + len;
}


स्थिर u_char *
ngx_json_string_encode(u_char *p, ngx_दत्त_item_t *item)
{
    ngx_str_t  *str;

    str = &item->दत्त.string;

    *p++ = '"';

    p = (u_char *) ngx_escape_json(p, str->दत्त, str->len);

    *p++ = '"';

    प्रत्यागम p;
}


स्थिर ssize_t
ngx_json_integer_दीर्घता(ngx_दत्त_item_t *item)
{
    प्रत्यागम NGX_INT64_LEN;
}


स्थिर u_char *
ngx_json_integer_encode(u_char *p, ngx_दत्त_item_t *item)
{
    प्रत्यागम ngx_sprintf(p, "%L", item->दत्त.integer);
}


स्थिर ssize_t
ngx_json_boolean_दीर्घता(ngx_दत्त_item_t *item)
{
    प्रत्यागम परिमाण("false") - 1;
}


स्थिर u_char *
ngx_json_boolean_encode(u_char *p, ngx_दत्त_item_t *item)
{
    प्रत्यागम item->दत्त.boolean ? ngx_cpymem(p, "true", 4)
                              : ngx_cpymem(p, "false", 5);
}


स्थिर ssize_t
ngx_json_null_दीर्घता(ngx_दत्त_item_t *item)
{
    प्रत्यागम परिमाण("null") - 1;
}


स्थिर u_char *
ngx_json_null_encode(u_char *p, ngx_दत्त_item_t *item)
{
    प्रत्यागम ngx_cpymem(p, "null", 4);
}
