
/*
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


প্রকারলেখ ssize_t   (*ngx_json_দৈর্ঘ্য_pt)(ngx_তথ্য_item_t *item);
প্রকারলেখ u_char   *(*ngx_json_encode_pt)(u_char *p, ngx_তথ্য_item_t *item);


প্রকারলেখ কাঠামো {
    ngx_json_দৈর্ঘ্য_pt        দৈর্ঘ্য;
    ngx_json_encode_pt        encode;
} ngx_json_encode_t;


স্থির ssize_t ngx_json_obj_দৈর্ঘ্য(ngx_তথ্য_item_t *item);
স্থির ssize_t ngx_json_list_দৈর্ঘ্য(ngx_তথ্য_item_t *item);
স্থির ssize_t ngx_json_string_দৈর্ঘ্য(ngx_তথ্য_item_t *item);
স্থির ssize_t ngx_json_integer_দৈর্ঘ্য(ngx_তথ্য_item_t *item);
স্থির ssize_t ngx_json_boolean_দৈর্ঘ্য(ngx_তথ্য_item_t *item);
স্থির ssize_t ngx_json_null_দৈর্ঘ্য(ngx_তথ্য_item_t *item);

স্থির u_char *ngx_json_obj_encode(u_char *p, ngx_তথ্য_item_t *item);
স্থির u_char *ngx_json_list_encode(u_char *p, ngx_তথ্য_item_t *item);
স্থির u_char *ngx_json_string_encode(u_char *p, ngx_তথ্য_item_t *item);
স্থির u_char *ngx_json_integer_encode(u_char *p, ngx_তথ্য_item_t *item);
স্থির u_char *ngx_json_boolean_encode(u_char *p, ngx_তথ্য_item_t *item);
স্থির u_char *ngx_json_null_encode(u_char *p, ngx_তথ্য_item_t *item);


স্থির ngx_json_encode_t  ngx_json_encode[] = {

    /* NGX_DATA_OBJECT_TYPE */
    { ngx_json_obj_দৈর্ঘ্য,
      ngx_json_obj_encode },

    /* NGX_DATA_LIST_TYPE */
    { ngx_json_list_দৈর্ঘ্য,
      ngx_json_list_encode },

    /* NGX_DATA_STRING_TYPE */
    { ngx_json_string_দৈর্ঘ্য,
      ngx_json_string_encode },

    /* NGX_DATA_INTEGER_TYPE */
    { ngx_json_integer_দৈর্ঘ্য,
      ngx_json_integer_encode },

    /* NGX_DATA_BOOLEAN_TYPE */
    { ngx_json_boolean_দৈর্ঘ্য,
      ngx_json_boolean_encode },

    /* NGX_DATA_NULL_TYPE */
    { ngx_json_null_দৈর্ঘ্য,
      ngx_json_null_encode },
};


ngx_buf_t *
ngx_json_render(ngx_pool_t *pool, ngx_তথ্য_item_t *item)
{
    আকার_t      দৈর্ঘ্য;
    ngx_buf_t  *buf;

    দৈর্ঘ্য = ngx_json_encode[item->type].দৈর্ঘ্য(item);
    যদি (দৈর্ঘ্য == (আকার_t) NGX_ERROR) {
        ফেরত ফাঁকা;
    }

    buf = ngx_create_অস্থায়ী_buf(pool, দৈর্ঘ্য);
    যদি (buf == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    buf->last = ngx_json_encode[item->type].encode(buf->last, item);

    ফেরত buf;
}


স্থির ssize_t
ngx_json_obj_দৈর্ঘ্য(ngx_তথ্য_item_t *item)
{
    আকার_t            len, যোগ;
    ngx_তথ্য_obj_t   *obj;
    ngx_তথ্য_item_t  *i;

    obj = &item->তথ্য.object;
    যোগ = আকার("{}") - 1;
    i = obj->item;

    যদি (i) {
        জন্য ( ;; ) {
            যদি (i->name.len > NGX_MAX_আকার_T_মান - (আকার("\"\":") - 1)) {
                ফেরত NGX_ERROR;
            }

            len = আকার("\"\":") - 1 + i->name.len;

            যদি (len > NGX_MAX_আকার_T_মান - যোগ) {
                ফেরত NGX_ERROR;
            }

            যোগ += len;

            len = ngx_escape_json(ফাঁকা, i->name.তথ্য, i->name.len);

            যদি (len > NGX_MAX_আকার_T_মান - যোগ) {
                ফেরত NGX_ERROR;
            }

            যোগ += len;

            len = ngx_json_encode[i->type].দৈর্ঘ্য(i);
            যদি (len == (আকার_t) NGX_ERROR) {
                ফেরত NGX_ERROR;
            }

            যদি (len > NGX_MAX_আকার_T_মান - যোগ) {
                ফেরত NGX_ERROR;
            }

            যোগ += len;

            i = i->পরবর্তী;

            যদি (i == ফাঁকা) {
                বিরতি;
            }

            যদি (যোগ > NGX_MAX_আকার_T_মান - (আকার(",") - 1)) {
                ফেরত NGX_ERROR;
            }

            যোগ += আকার(",") - 1;
        }
    }

    ফেরত যোগ;
}


স্থির u_char *
ngx_json_obj_encode(u_char *p, ngx_তথ্য_item_t *item)
{
    ngx_তথ্য_obj_t   *obj;
    ngx_তথ্য_item_t  *i;

    obj = &item->তথ্য.object;
    i = obj->item;

    *p++ = '{';

    যদি (i) {
        জন্য ( ;; ) {
            *p++ = '"';

            p = (u_char *) ngx_escape_json(p, i->name.তথ্য, i->name.len);

            *p++ = '"';
            *p++ = ':';

            p = ngx_json_encode[i->type].encode(p, i);

            i = i->পরবর্তী;

            যদি (i == ফাঁকা) {
                বিরতি;
            }

            *p++ = ',';
        }
    }

    *p++ = '}';

    ফেরত p;
}


স্থির ssize_t
ngx_json_list_দৈর্ঘ্য(ngx_তথ্য_item_t *item)
{
    আকার_t            len, যোগ;
    ngx_তথ্য_obj_t   *obj;
    ngx_তথ্য_item_t  *i;

    obj = &item->তথ্য.object;
    যোগ = আকার("[]") - 1;
    i = obj->item;

    যদি (i) {
        জন্য ( ;; ) {
            len = ngx_json_encode[i->type].দৈর্ঘ্য(i);
            যদি (len == (আকার_t) NGX_ERROR) {
                ফেরত NGX_ERROR;
            }

            যদি (len > NGX_MAX_আকার_T_মান - যোগ) {
                ফেরত NGX_ERROR;
            }

            যোগ += len;

            i = i->পরবর্তী;

            যদি (i == ফাঁকা) {
                বিরতি;
            }

            যদি (যোগ > NGX_MAX_আকার_T_মান - (আকার(",") - 1)) {
                ফেরত NGX_ERROR;
            }

            যোগ += আকার(",") - 1;
        }
    }

    ফেরত যোগ;
}


স্থির u_char *
ngx_json_list_encode(u_char *p, ngx_তথ্য_item_t *item)
{
    ngx_তথ্য_obj_t   *obj;
    ngx_তথ্য_item_t  *i;

    obj = &item->তথ্য.object;
    i = obj->item;

    *p++ = '[';

    যদি (i) {
        জন্য ( ;; ) {
            p = ngx_json_encode[i->type].encode(p, i);

            i = i->পরবর্তী;

            যদি (i == ফাঁকা) {
                বিরতি;
            }

            *p++ = ',';
        }
    }

    *p++ = ']';

    ফেরত p;
}


স্থির ssize_t
ngx_json_string_দৈর্ঘ্য(ngx_তথ্য_item_t *item)
{
    আকার_t      len, যোগ;
    ngx_str_t  *str;

    str = &item->তথ্য.string;

    যদি (str->len > NGX_MAX_আকার_T_মান - (আকার("\"\"") - 1)) {
        ফেরত NGX_ERROR;
    }

    যোগ = আকার("\"\"") - 1 + str->len;

    len = ngx_escape_json(ফাঁকা, str->তথ্য, str->len);

    যদি (len > NGX_MAX_আকার_T_মান - যোগ) {
        ফেরত NGX_ERROR;
    }

    ফেরত যোগ + len;
}


স্থির u_char *
ngx_json_string_encode(u_char *p, ngx_তথ্য_item_t *item)
{
    ngx_str_t  *str;

    str = &item->তথ্য.string;

    *p++ = '"';

    p = (u_char *) ngx_escape_json(p, str->তথ্য, str->len);

    *p++ = '"';

    ফেরত p;
}


স্থির ssize_t
ngx_json_integer_দৈর্ঘ্য(ngx_তথ্য_item_t *item)
{
    ফেরত NGX_INT64_LEN;
}


স্থির u_char *
ngx_json_integer_encode(u_char *p, ngx_তথ্য_item_t *item)
{
    ফেরত ngx_sprintf(p, "%L", item->তথ্য.integer);
}


স্থির ssize_t
ngx_json_boolean_দৈর্ঘ্য(ngx_তথ্য_item_t *item)
{
    ফেরত আকার("false") - 1;
}


স্থির u_char *
ngx_json_boolean_encode(u_char *p, ngx_তথ্য_item_t *item)
{
    ফেরত item->তথ্য.boolean ? ngx_cpymem(p, "true", 4)
                              : ngx_cpymem(p, "false", 5);
}


স্থির ssize_t
ngx_json_null_দৈর্ঘ্য(ngx_তথ্য_item_t *item)
{
    ফেরত আকার("null") - 1;
}


স্থির u_char *
ngx_json_null_encode(u_char *p, ngx_তথ্য_item_t *item)
{
    ফেরত ngx_cpymem(p, "null", 4);
}
