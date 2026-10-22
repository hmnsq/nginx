
/*
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


স্থির আকার_t  ngx_তথ্য_sizes[] = {
    আকার(ngx_তথ্য_obj_t),            /* NGX_DATA_OBJECT_TYPE */
    আকার(ngx_তথ্য_obj_t),            /* NGX_DATA_LIST_TYPE */
    আকার(ngx_str_t),                 /* NGX_DATA_STRING_TYPE */
    আকার(int64_t),                   /* NGX_DATA_INTEGER_TYPE */
    আকার(ngx_uint_t),                /* NGX_DATA_BOOLEAN_TYPE */
    0,                                 /* NGX_DATA_NULL_TYPE */
};


ngx_তথ্য_item_t *
ngx_তথ্য_new_item(ngx_pool_t *pool, ngx_uint_t type)
{
    ngx_তথ্য_item_t  *item;

    item = ngx_pcalloc(pool, offsetof(ngx_তথ্য_item_t, তথ্য)
                             + ngx_তথ্য_sizes[type]);
    যদি (item == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    item->type = type;

    ফেরত item;
}


শূন্য
ngx_তথ্য_add_item(ngx_তথ্য_item_t *obj, ngx_str_t *name, ngx_তথ্য_item_t *item)
{
    ngx_তথ্য_obj_t  *object;

    যদি (obj->type != NGX_তথ্য_OBJECT_TYPE && obj->type != NGX_তথ্য_LIST_TYPE) {
        ফেরত;
    }

    object = &obj->তথ্য.object;

    যদি (name) {
        item->name = *name;
    }

    যদি (object->পরবর্তী == ফাঁকা) {
        object->পরবর্তী = &object->item;
    }

    *object->পরবর্তী = item;
    object->পরবর্তী = &item->পরবর্তী;
}


ngx_তথ্য_item_t *
ngx_তথ্য_obj_handler(uintptr_t তথ্য, ngx_pool_t *pool, শূন্য *ctx)
{
    ngx_তথ্য_item_t  *obj, *item;
    ngx_তথ্য_decl_t  *decl;

    obj = ngx_তথ্য_new_object(pool);
    যদি (obj == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    decl = (ngx_তথ্য_decl_t *) তথ্য;

    যখন (decl->name.len) {

        item = decl->handler(decl->তথ্য, pool, ctx);
        যদি (item == ফাঁকা) {
            ফেরত ফাঁকা;
        }

        যদি (item != NGX_তথ্য_DECLINE) {
            ngx_তথ্য_add_item(obj, &decl->name, item);
        }

        decl++;
    }

    ফেরত obj;
}


ngx_তথ্য_item_t *
ngx_তথ্য_obj_fields_handler(uintptr_t তথ্য, ngx_pool_t *pool, শূন্য *ctx,
    ngx_array_t *fields)
{
    ngx_str_t        *মান;
    ngx_uint_t        i;
    ngx_তথ্য_item_t  *obj, *item;
    ngx_তথ্য_decl_t  *decl;

    obj = ngx_তথ্য_new_object(pool);
    যদি (obj == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    decl = (ngx_তথ্য_decl_t *) তথ্য;

    যখন (decl->name.len) {

        যদি (fields) {
            মান = fields->elts;

            জন্য (i = 0; i < fields->nelts; i++) {

                যদি (decl->name.len == মান[i].len
                    && ngx_strncmp(decl->name.তথ্য, মান[i].তথ্য,
                                   মান[i].len)
                    == 0)
                {
                    যাও found;
                }
            }

            decl++;
            চালু;
        }

found:

        item = decl->handler(decl->তথ্য, pool, ctx);
        যদি (item == ফাঁকা) {
            ফেরত ফাঁকা;
        }

        যদি (item != NGX_তথ্য_DECLINE) {
            ngx_তথ্য_add_item(obj, &decl->name, item);
        }

        decl++;
    }

    ফেরত obj;
}


ngx_তথ্য_item_t *
ngx_তথ্য_সংখ্যা_handler(uintptr_t তথ্য, ngx_pool_t *pool, শূন্য *ctx)
{
    ngx_তথ্য_item_t  *item;

    item = ngx_তথ্য_new_integer(pool);
    যদি (item == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    item->তথ্য.integer = তথ্য;

    ফেরত item;
}


ngx_তথ্য_item_t *
ngx_তথ্য_string_handler(uintptr_t তথ্য, ngx_pool_t *pool, শূন্য *ctx)
{
    ngx_str_t        *মান, *src;
    ngx_তথ্য_item_t  *item;

    item = ngx_তথ্য_new_string(pool);
    যদি (item == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    মান = &item->তথ্য.string;

    src = (ngx_str_t *) তথ্য;

    মান->len = src->len;

    মান->তথ্য = ngx_pstrdup(pool, src);
    যদি (মান->তথ্য == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    ফেরত item;
}


ngx_তথ্য_item_t *
ngx_তথ্য_boolean_handler(uintptr_t তথ্য, ngx_pool_t *pool, শূন্য *ctx)
{
    ngx_তথ্য_item_t  *item;

    item = ngx_তথ্য_new_boolean(pool);
    যদি (item == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    item->তথ্য.boolean = তথ্য;

    ফেরত item;
}


ngx_তথ্য_item_t *
ngx_তথ্য_time_handler(uintptr_t তথ্য, ngx_pool_t *pool, শূন্য *ctx)
{
    u_char      *p;
    ngx_tm_t     tm;
    ngx_str_t    src;
    ngx_time_t  *tp;
    u_char       iso8601[আকার("1974-01-19T13:00:00.000Z") - 1];

    tp = (ngx_time_t *) তথ্য;

    ngx_gmtime(tp->sec, &tm);

    p = ngx_sprintf(iso8601, "%4d-%02d-%02dT%02d:%02d:%02d",
                    tm.ngx_tm_year, tm.ngx_tm_mon, tm.ngx_tm_mday,
                    tm.ngx_tm_hour, tm.ngx_tm_min, tm.ngx_tm_sec);

    যদি (tp->msec) {
        p = ngx_sprintf(p, ".%03ui", tp->msec);
    }

    *p++ = 'Z';

    src.তথ্য = iso8601;
    src.len = p - iso8601;

    ফেরত ngx_তথ্য_string_handler((uintptr_t) &src, pool, ফাঁকা);
}


ngx_তথ্য_item_t *
ngx_তথ্য_struct_int64_handler(uintptr_t তথ্য, ngx_pool_t *pool, শূন্য *ctx)
{
    ngx_তথ্য_item_t  *item;

    item = ngx_তথ্য_new_integer(pool);
    যদি (item == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    item->তথ্য.integer = *(int64_t *) ((u_char *) ctx + তথ্য);

    ফেরত item;
}


ngx_তথ্য_item_t *
ngx_তথ্য_struct_int_handler(uintptr_t তথ্য, ngx_pool_t *pool, শূন্য *ctx)
{
    ngx_int_t  মান;

    মান = *(ngx_int_t *) ((u_char *) ctx + তথ্য);

    ফেরত ngx_তথ্য_সংখ্যা_handler(মান, pool, ফাঁকা);
}


ngx_তথ্য_item_t *
ngx_তথ্য_struct_atomic_handler(uintptr_t তথ্য, ngx_pool_t *pool, শূন্য *ctx)
{
    ngx_তথ্য_item_t  *item;

    item = ngx_তথ্য_new_integer(pool);
    যদি (item == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    item->তথ্য.integer = *(ngx_atomic_uint_t *) ((u_char *) ctx + তথ্য);

    ফেরত item;
}


ngx_তথ্য_item_t *
ngx_তথ্য_struct_str_handler(uintptr_t তথ্য, ngx_pool_t *pool, শূন্য *ctx)
{
    ngx_str_t        *মান, *src;
    ngx_তথ্য_item_t  *item;

    item = ngx_তথ্য_new_string(pool);
    যদি (item == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    মান = &item->তথ্য.string;

    src = (ngx_str_t *) ((u_char *) ctx + তথ্য);

    মান->len = src->len;

    মান->তথ্য = ngx_pstrdup(pool, src);
    যদি (মান->তথ্য == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    ফেরত item;
}


ngx_তথ্য_item_t *
ngx_তথ্য_struct_boolean_handler(uintptr_t তথ্য, ngx_pool_t *pool, শূন্য *ctx)
{
    ngx_uint_t  মান;

    মান = *(ngx_uint_t *) ((u_char *) ctx + তথ্য);

    ফেরত ngx_তথ্য_boolean_handler(মান, pool, ফাঁকা);
}
