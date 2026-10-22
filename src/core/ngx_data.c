
/*
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


स्थिर size_t  ngx_data_sizes[] = {
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

    item = ngx_pcalloc(pool, offsetof(ngx_data_item_t, data)
                             + ngx_data_sizes[type]);
    सचे (item == रिक्त) {
        पटिनिवत्त रिक्त;
    }

    item->type = type;

    पटिनिवत्त item;
}


शून्य
ngx_data_add_item(ngx_data_item_t *obj, ngx_str_t *name, ngx_data_item_t *item)
{
    ngx_data_obj_t  *object;

    सचे (obj->type != NGX_DATA_OBJECT_TYPE && obj->type != NGX_DATA_LIST_TYPE) {
        पटिनिवत्त;
    }

    object = &obj->data.object;

    सचे (name) {
        item->name = *name;
    }

    सचे (object->next == रिक्त) {
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
    सचे (obj == रिक्त) {
        पटिनिवत्त रिक्त;
    }

    decl = (ngx_data_decl_t *) data;

    याव (decl->name.len) {

        item = decl->handler(decl->data, pool, ctx);
        सचे (item == रिक्त) {
            पटिनिवत्त रिक्त;
        }

        सचे (item != NGX_DATA_DECLINE) {
            ngx_data_add_item(obj, &decl->name, item);
        }

        decl++;
    }

    पटिनिवत्त obj;
}


ngx_data_item_t *
ngx_data_obj_fields_handler(uintptr_t data, ngx_pool_t *pool, शून्य *ctx,
    ngx_array_t *fields)
{
    ngx_str_t        *मानम्;
    ngx_uint_t        i;
    ngx_data_item_t  *obj, *item;
    ngx_data_decl_t  *decl;

    obj = ngx_data_new_object(pool);
    सचे (obj == रिक्त) {
        पटिनिवत्त रिक्त;
    }

    decl = (ngx_data_decl_t *) data;

    याव (decl->name.len) {

        सचे (fields) {
            मानम् = fields->elts;

            अत्थाय (i = 0; i < fields->nelts; i++) {

                सचे (decl->name.len == मानम्[i].len
                    && ngx_strncmp(decl->name.data, मानम्[i].data,
                                   मानम्[i].len)
                    == 0)
                {
                    गच्छ found;
                }
            }

            decl++;
            अनुवर्त;
        }

found:

        item = decl->handler(decl->data, pool, ctx);
        सचे (item == रिक्त) {
            पटिनिवत्त रिक्त;
        }

        सचे (item != NGX_DATA_DECLINE) {
            ngx_data_add_item(obj, &decl->name, item);
        }

        decl++;
    }

    पटिनिवत्त obj;
}


ngx_data_item_t *
ngx_data_सङ्ख्या_handler(uintptr_t data, ngx_pool_t *pool, शून्य *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    सचे (item == रिक्त) {
        पटिनिवत्त रिक्त;
    }

    item->data.integer = data;

    पटिनिवत्त item;
}


ngx_data_item_t *
ngx_data_string_handler(uintptr_t data, ngx_pool_t *pool, शून्य *ctx)
{
    ngx_str_t        *मानम्, *src;
    ngx_data_item_t  *item;

    item = ngx_data_new_string(pool);
    सचे (item == रिक्त) {
        पटिनिवत्त रिक्त;
    }

    मानम् = &item->data.string;

    src = (ngx_str_t *) data;

    मानम्->len = src->len;

    मानम्->data = ngx_pstrdup(pool, src);
    सचे (मानम्->data == रिक्त) {
        पटिनिवत्त रिक्त;
    }

    पटिनिवत्त item;
}


ngx_data_item_t *
ngx_data_boolean_handler(uintptr_t data, ngx_pool_t *pool, शून्य *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_boolean(pool);
    सचे (item == रिक्त) {
        पटिनिवत्त रिक्त;
    }

    item->data.boolean = data;

    पटिनिवत्त item;
}


ngx_data_item_t *
ngx_data_time_handler(uintptr_t data, ngx_pool_t *pool, शून्य *ctx)
{
    u_char      *p;
    ngx_tm_t     tm;
    ngx_str_t    src;
    ngx_time_t  *tp;
    u_char       iso8601[परिमाण("1974-01-19T13:00:00.000Z") - 1];

    tp = (ngx_time_t *) data;

    ngx_gmtime(tp->sec, &tm);

    p = ngx_sprintf(iso8601, "%4d-%02d-%02dT%02d:%02d:%02d",
                    tm.ngx_tm_year, tm.ngx_tm_mon, tm.ngx_tm_mday,
                    tm.ngx_tm_hour, tm.ngx_tm_min, tm.ngx_tm_sec);

    सचे (tp->msec) {
        p = ngx_sprintf(p, ".%03ui", tp->msec);
    }

    *p++ = 'Z';

    src.data = iso8601;
    src.len = p - iso8601;

    पटिनिवत्त ngx_data_string_handler((uintptr_t) &src, pool, रिक्त);
}


ngx_data_item_t *
ngx_data_struct_int64_handler(uintptr_t data, ngx_pool_t *pool, शून्य *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    सचे (item == रिक्त) {
        पटिनिवत्त रिक्त;
    }

    item->data.integer = *(int64_t *) ((u_char *) ctx + data);

    पटिनिवत्त item;
}


ngx_data_item_t *
ngx_data_struct_int_handler(uintptr_t data, ngx_pool_t *pool, शून्य *ctx)
{
    ngx_int_t  मानम्;

    मानम् = *(ngx_int_t *) ((u_char *) ctx + data);

    पटिनिवत्त ngx_data_सङ्ख्या_handler(मानम्, pool, रिक्त);
}


ngx_data_item_t *
ngx_data_struct_atomic_handler(uintptr_t data, ngx_pool_t *pool, शून्य *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    सचे (item == रिक्त) {
        पटिनिवत्त रिक्त;
    }

    item->data.integer = *(ngx_atomic_uint_t *) ((u_char *) ctx + data);

    पटिनिवत्त item;
}


ngx_data_item_t *
ngx_data_struct_str_handler(uintptr_t data, ngx_pool_t *pool, शून्य *ctx)
{
    ngx_str_t        *मानम्, *src;
    ngx_data_item_t  *item;

    item = ngx_data_new_string(pool);
    सचे (item == रिक्त) {
        पटिनिवत्त रिक्त;
    }

    मानम् = &item->data.string;

    src = (ngx_str_t *) ((u_char *) ctx + data);

    मानम्->len = src->len;

    मानम्->data = ngx_pstrdup(pool, src);
    सचे (मानम्->data == रिक्त) {
        पटिनिवत्त रिक्त;
    }

    पटिनिवत्त item;
}


ngx_data_item_t *
ngx_data_struct_boolean_handler(uintptr_t data, ngx_pool_t *pool, शून्य *ctx)
{
    ngx_uint_t  मानम्;

    मानम् = *(ngx_uint_t *) ((u_char *) ctx + data);

    पटिनिवत्त ngx_data_boolean_handler(मानम्, pool, रिक्त);
}
