
/*
 * Copyright (C) Nginx, Inc.
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


प्रकार ssize_t   (*ngx_json_लंबाई_pt)(ngx_तथ्य_item_t *item);
प्रकार u_char   *(*ngx_json_encode_pt)(u_char *p, ngx_तथ्य_item_t *item);


प्रकार काष्ठा {
    ngx_json_लंबाई_pt        लंबाई;
    ngx_json_encode_pt        encode;
} ngx_json_encode_t;


अटल ssize_t ngx_json_obj_लंबाई(ngx_तथ्य_item_t *item);
अटल ssize_t ngx_json_list_लंबाई(ngx_तथ्य_item_t *item);
अटल ssize_t ngx_json_string_लंबाई(ngx_तथ्य_item_t *item);
अटल ssize_t ngx_json_integer_लंबाई(ngx_तथ्य_item_t *item);
अटल ssize_t ngx_json_boolean_लंबाई(ngx_तथ्य_item_t *item);
अटल ssize_t ngx_json_null_लंबाई(ngx_तथ्य_item_t *item);

अटल u_char *ngx_json_obj_encode(u_char *p, ngx_तथ्य_item_t *item);
अटल u_char *ngx_json_list_encode(u_char *p, ngx_तथ्य_item_t *item);
अटल u_char *ngx_json_string_encode(u_char *p, ngx_तथ्य_item_t *item);
अटल u_char *ngx_json_integer_encode(u_char *p, ngx_तथ्य_item_t *item);
अटल u_char *ngx_json_boolean_encode(u_char *p, ngx_तथ्य_item_t *item);
अटल u_char *ngx_json_null_encode(u_char *p, ngx_तथ्य_item_t *item);


अटल ngx_json_encode_t  ngx_json_encode[] = {

    /* NGX_DATA_OBJECT_TYPE */
    { ngx_json_obj_लंबाई,
      ngx_json_obj_encode },

    /* NGX_DATA_LIST_TYPE */
    { ngx_json_list_लंबाई,
      ngx_json_list_encode },

    /* NGX_DATA_STRING_TYPE */
    { ngx_json_string_लंबाई,
      ngx_json_string_encode },

    /* NGX_DATA_INTEGER_TYPE */
    { ngx_json_integer_लंबाई,
      ngx_json_integer_encode },

    /* NGX_DATA_BOOLEAN_TYPE */
    { ngx_json_boolean_लंबाई,
      ngx_json_boolean_encode },

    /* NGX_DATA_NULL_TYPE */
    { ngx_json_null_लंबाई,
      ngx_json_null_encode },
};


ngx_buf_t *
ngx_json_render(ngx_pool_t *pool, ngx_तथ्य_item_t *item)
{
    माप_प्रकार      लंबाई;
    ngx_buf_t  *buf;

    लंबाई = ngx_json_encode[item->type].लंबाई(item);
    अगर (लंबाई == (माप_प्रकार) NGX_ERROR) {
        वापस शून्य;
    }

    buf = ngx_create_अस्थायी_buf(pool, लंबाई);
    अगर (buf == शून्य) {
        वापस शून्य;
    }

    buf->last = ngx_json_encode[item->type].encode(buf->last, item);

    वापस buf;
}


अटल ssize_t
ngx_json_obj_लंबाई(ngx_तथ्य_item_t *item)
{
    माप_प्रकार            len, योग;
    ngx_तथ्य_obj_t   *obj;
    ngx_तथ्य_item_t  *i;

    obj = &item->तथ्य.object;
    योग = माप("{}") - 1;
    i = obj->item;

    अगर (i) {
        क्रम ( ;; ) {
            अगर (i->name.len > NGX_MAX_आकार_T_मूल्य - (माप("\"\":") - 1)) {
                वापस NGX_ERROR;
            }

            len = माप("\"\":") - 1 + i->name.len;

            अगर (len > NGX_MAX_आकार_T_मूल्य - योग) {
                वापस NGX_ERROR;
            }

            योग += len;

            len = ngx_escape_json(शून्य, i->name.तथ्य, i->name.len);

            अगर (len > NGX_MAX_आकार_T_मूल्य - योग) {
                वापस NGX_ERROR;
            }

            योग += len;

            len = ngx_json_encode[i->type].लंबाई(i);
            अगर (len == (माप_प्रकार) NGX_ERROR) {
                वापस NGX_ERROR;
            }

            अगर (len > NGX_MAX_आकार_T_मूल्य - योग) {
                वापस NGX_ERROR;
            }

            योग += len;

            i = i->अग्र;

            अगर (i == शून्य) {
                अवरोध;
            }

            अगर (योग > NGX_MAX_आकार_T_मूल्य - (माप(",") - 1)) {
                वापस NGX_ERROR;
            }

            योग += माप(",") - 1;
        }
    }

    वापस योग;
}


अटल u_char *
ngx_json_obj_encode(u_char *p, ngx_तथ्य_item_t *item)
{
    ngx_तथ्य_obj_t   *obj;
    ngx_तथ्य_item_t  *i;

    obj = &item->तथ्य.object;
    i = obj->item;

    *p++ = '{';

    अगर (i) {
        क्रम ( ;; ) {
            *p++ = '"';

            p = (u_char *) ngx_escape_json(p, i->name.तथ्य, i->name.len);

            *p++ = '"';
            *p++ = ':';

            p = ngx_json_encode[i->type].encode(p, i);

            i = i->अग्र;

            अगर (i == शून्य) {
                अवरोध;
            }

            *p++ = ',';
        }
    }

    *p++ = '}';

    वापस p;
}


अटल ssize_t
ngx_json_list_लंबाई(ngx_तथ्य_item_t *item)
{
    माप_प्रकार            len, योग;
    ngx_तथ्य_obj_t   *obj;
    ngx_तथ्य_item_t  *i;

    obj = &item->तथ्य.object;
    योग = माप("[]") - 1;
    i = obj->item;

    अगर (i) {
        क्रम ( ;; ) {
            len = ngx_json_encode[i->type].लंबाई(i);
            अगर (len == (माप_प्रकार) NGX_ERROR) {
                वापस NGX_ERROR;
            }

            अगर (len > NGX_MAX_आकार_T_मूल्य - योग) {
                वापस NGX_ERROR;
            }

            योग += len;

            i = i->अग्र;

            अगर (i == शून्य) {
                अवरोध;
            }

            अगर (योग > NGX_MAX_आकार_T_मूल्य - (माप(",") - 1)) {
                वापस NGX_ERROR;
            }

            योग += माप(",") - 1;
        }
    }

    वापस योग;
}


अटल u_char *
ngx_json_list_encode(u_char *p, ngx_तथ्य_item_t *item)
{
    ngx_तथ्य_obj_t   *obj;
    ngx_तथ्य_item_t  *i;

    obj = &item->तथ्य.object;
    i = obj->item;

    *p++ = '[';

    अगर (i) {
        क्रम ( ;; ) {
            p = ngx_json_encode[i->type].encode(p, i);

            i = i->अग्र;

            अगर (i == शून्य) {
                अवरोध;
            }

            *p++ = ',';
        }
    }

    *p++ = ']';

    वापस p;
}


अटल ssize_t
ngx_json_string_लंबाई(ngx_तथ्य_item_t *item)
{
    माप_प्रकार      len, योग;
    ngx_str_t  *str;

    str = &item->तथ्य.string;

    अगर (str->len > NGX_MAX_आकार_T_मूल्य - (माप("\"\"") - 1)) {
        वापस NGX_ERROR;
    }

    योग = माप("\"\"") - 1 + str->len;

    len = ngx_escape_json(शून्य, str->तथ्य, str->len);

    अगर (len > NGX_MAX_आकार_T_मूल्य - योग) {
        वापस NGX_ERROR;
    }

    वापस योग + len;
}


अटल u_char *
ngx_json_string_encode(u_char *p, ngx_तथ्य_item_t *item)
{
    ngx_str_t  *str;

    str = &item->तथ्य.string;

    *p++ = '"';

    p = (u_char *) ngx_escape_json(p, str->तथ्य, str->len);

    *p++ = '"';

    वापस p;
}


अटल ssize_t
ngx_json_integer_लंबाई(ngx_तथ्य_item_t *item)
{
    वापस NGX_INT64_LEN;
}


अटल u_char *
ngx_json_integer_encode(u_char *p, ngx_तथ्य_item_t *item)
{
    वापस ngx_sprintf(p, "%L", item->तथ्य.integer);
}


अटल ssize_t
ngx_json_boolean_लंबाई(ngx_तथ्य_item_t *item)
{
    वापस माप("false") - 1;
}


अटल u_char *
ngx_json_boolean_encode(u_char *p, ngx_तथ्य_item_t *item)
{
    वापस item->तथ्य.boolean ? ngx_cpymem(p, "true", 4)
                              : ngx_cpymem(p, "false", 5);
}


अटल ssize_t
ngx_json_null_लंबाई(ngx_तथ्य_item_t *item)
{
    वापस माप("null") - 1;
}


अटल u_char *
ngx_json_null_encode(u_char *p, ngx_तथ्य_item_t *item)
{
    वापस ngx_cpymem(p, "null", 4);
}
