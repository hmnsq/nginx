
/*
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


स्थिर माप_प्रकार  ngx_data_sizes[] = {
    परिमाण(ngx_data_obj_t),            /* NGX_DATA_OBJECT_TYPE */
    परिमाण(ngx_data_obj_t),            /* NGX_DATA_LIST_TYPE */
    परिमाण(ngx_str_t),                 /* NGX_DATA_STRING_TYPE */
    परिमाण(int64_t),                   /* NGX_DATA_INTEGER_TYPE */
    परिमाण(ngx_uint_t),                /* NGX_DATA_BOOLEAN_TYPE */
    0,                                 /* NGX_DATA_NULL_TYPE */
};


ngx_data_item_t *
ngx_data_new_item(ngx_pool_t *pool, ngx_uint_t type)
{
    ngx_data_item_t  *item;

    item = ngx_pcalloc(pool, दुरत्व(ngx_data_item_t, data)
                             + ngx_data_sizes[type]);
    यदि (item == रिक्त) {
        फर्काऊ रिक्त;
    }

    item->type = type;

    फर्काऊ item;
}


शून्य
ngx_data_add_item(ngx_data_item_t *obj, ngx_str_t *name, ngx_data_item_t *item)
{
    ngx_data_obj_t  *object;

    यदि (obj->type != NGX_DATA_OBJECT_TYPE && obj->type != NGX_DATA_LIST_TYPE) {
        फर्काऊ;
    }

    object = &obj->data.object;

    यदि (name) {
        item->name = *name;
    }

    यदि (object->next == रिक्त) {
        object->next = &object->item;
    }

    *object->next = item;
    object->next = &item->next;
}


ngx_data_item_t *
ngx_data_obj_handler(uintptr_t data, ngx_pool_t *pool, शून्य *ctx)
{
    ngx_data_item_t  *obj, *item;
    ngx_data_decl_t  *decl;

    obj = ngx_data_new_object(pool);
    यदि (obj == रिक्त) {
        फर्काऊ रिक्त;
    }

    decl = (ngx_data_decl_t *) data;

    जबसम्म (decl->name.len) {

        item = decl->handler(decl->data, pool, ctx);
        यदि (item == रिक्त) {
            फर्काऊ रिक्त;
        }

        यदि (item != NGX_DATA_DECLINE) {
            ngx_data_add_item(obj, &decl->name, item);
        }

        decl++;
    }

    फर्काऊ obj;
}


ngx_data_item_t *
ngx_data_obj_fields_handler(uintptr_t data, ngx_pool_t *pool, शून्य *ctx,
    ngx_array_t *fields)
{
    ngx_str_t        *मान;
    ngx_uint_t        i;
    ngx_data_item_t  *obj, *item;
    ngx_data_decl_t  *decl;

    obj = ngx_data_new_object(pool);
    यदि (obj == रिक्त) {
        फर्काऊ रिक्त;
    }

    decl = (ngx_data_decl_t *) data;

    जबसम्म (decl->name.len) {

        यदि (fields) {
            मान = fields->elts;

            लागि (i = 0; i < fields->nelts; i++) {

                यदि (decl->name.len == मान[i].len
                    && ngx_strncmp(decl->name.data, मान[i].data,
                                   मान[i].len)
                    == 0)
                {
                    गच्छ found;
                }
            }

            decl++;
            जारी;
        }

found:

        item = decl->handler(decl->data, pool, ctx);
        यदि (item == रिक्त) {
            फर्काऊ रिक्त;
        }

        यदि (item != NGX_DATA_DECLINE) {
            ngx_data_add_item(obj, &decl->name, item);
        }

        decl++;
    }

    फर्काऊ obj;
}


ngx_data_item_t *
ngx_data_सङ्ख्या_handler(uintptr_t data, ngx_pool_t *pool, शून्य *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    यदि (item == रिक्त) {
        फर्काऊ रिक्त;
    }

    item->data.integer = data;

    फर्काऊ item;
}


ngx_data_item_t *
ngx_data_string_handler(uintptr_t data, ngx_pool_t *pool, शून्य *ctx)
{
    ngx_str_t        *मान, *src;
    ngx_data_item_t  *item;

    item = ngx_data_new_string(pool);
    यदि (item == रिक्त) {
        फर्काऊ रिक्त;
    }

    मान = &item->data.string;

    src = (ngx_str_t *) data;

    मान->len = src->len;

    मान->data = ngx_pstrdup(pool, src);
    यदि (मान->data == रिक्त) {
        फर्काऊ रिक्त;
    }

    फर्काऊ item;
}


ngx_data_item_t *
ngx_data_boolean_handler(uintptr_t data, ngx_pool_t *pool, शून्य *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_boolean(pool);
    यदि (item == रिक्त) {
        फर्काऊ रिक्त;
    }

    item->data.boolean = data;

    फर्काऊ item;
}


ngx_data_item_t *
ngx_data_time_handler(uintptr_t data, ngx_pool_t *pool, शून्य *ctx)
{
    u_char      *p;
    ngx_tm_t     पंचांग;
    ngx_str_t    src;
    ngx_time_t  *tp;
    u_char       iso8601[परिमाण("1974-01-19T13:00:00.000Z") - 1];

    tp = (ngx_time_t *) data;

    ngx_gmtime(tp->sec, &पंचांग);

    p = ngx_sprintf(iso8601, "%4d-%02d-%02dT%02d:%02d:%02d",
                    पंचांग.ngx_tm_year, पंचांग.ngx_tm_mon, पंचांग.ngx_tm_mday,
                    पंचांग.ngx_tm_hour, पंचांग.ngx_tm_min, पंचांग.ngx_tm_sec);

    यदि (tp->msec) {
        p = ngx_sprintf(p, ".%03ui", tp->msec);
    }

    *p++ = 'Z';

    src.data = iso8601;
    src.len = p - iso8601;

    फर्काऊ ngx_data_string_handler((uintptr_t) &src, pool, रिक्त);
}


ngx_data_item_t *
ngx_data_struct_int64_handler(uintptr_t data, ngx_pool_t *pool, शून्य *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    यदि (item == रिक्त) {
        फर्काऊ रिक्त;
    }

    item->data.integer = *(int64_t *) ((u_char *) ctx + data);

    फर्काऊ item;
}


ngx_data_item_t *
ngx_data_struct_int_handler(uintptr_t data, ngx_pool_t *pool, शून्य *ctx)
{
    ngx_int_t  मान;

    मान = *(ngx_int_t *) ((u_char *) ctx + data);

    फर्काऊ ngx_data_सङ्ख्या_handler(मान, pool, रिक्त);
}


ngx_data_item_t *
ngx_data_struct_atomic_handler(uintptr_t data, ngx_pool_t *pool, शून्य *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    यदि (item == रिक्त) {
        फर्काऊ रिक्त;
    }

    item->data.integer = *(ngx_atomic_uint_t *) ((u_char *) ctx + data);

    फर्काऊ item;
}


ngx_data_item_t *
ngx_data_struct_str_handler(uintptr_t data, ngx_pool_t *pool, शून्य *ctx)
{
    ngx_str_t        *मान, *src;
    ngx_data_item_t  *item;

    item = ngx_data_new_string(pool);
    यदि (item == रिक्त) {
        फर्काऊ रिक्त;
    }

    मान = &item->data.string;

    src = (ngx_str_t *) ((u_char *) ctx + data);

    मान->len = src->len;

    मान->data = ngx_pstrdup(pool, src);
    यदि (मान->data == रिक्त) {
        फर्काऊ रिक्त;
    }

    फर्काऊ item;
}


ngx_data_item_t *
ngx_data_struct_boolean_handler(uintptr_t data, ngx_pool_t *pool, शून्य *ctx)
{
    ngx_uint_t  मान;

    मान = *(ngx_uint_t *) ((u_char *) ctx + data);

    फर्काऊ ngx_data_boolean_handler(मान, pool, रिक्त);
}
