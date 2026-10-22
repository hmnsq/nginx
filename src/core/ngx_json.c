
/*
 * Copyright (C) Nginx, Inc.
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ssize_t   (*ngx_json_ਲੰਮਾਈ_pt)(ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ਵਸਤੂ);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ u_char   *(*ngx_json_encode_pt)(u_char *p, ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ਵਸਤੂ);


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_json_ਲੰਮਾਈ_pt        ਲੰਮਾਈ;
    ngx_json_encode_pt        encode;
} ngx_json_encode_t;


ਸਥਿਰ ssize_t ngx_json_obj_ਲੰਮਾਈ(ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ਵਸਤੂ);
ਸਥਿਰ ssize_t ngx_json_list_ਲੰਮਾਈ(ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ਵਸਤੂ);
ਸਥਿਰ ssize_t ngx_json_string_ਲੰਮਾਈ(ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ਵਸਤੂ);
ਸਥਿਰ ssize_t ngx_json_integer_ਲੰਮਾਈ(ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ਵਸਤੂ);
ਸਥਿਰ ssize_t ngx_json_boolean_ਲੰਮਾਈ(ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ਵਸਤੂ);
ਸਥਿਰ ssize_t ngx_json_null_ਲੰਮਾਈ(ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ਵਸਤੂ);

ਸਥਿਰ u_char *ngx_json_obj_encode(u_char *p, ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ਵਸਤੂ);
ਸਥਿਰ u_char *ngx_json_list_encode(u_char *p, ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ਵਸਤੂ);
ਸਥਿਰ u_char *ngx_json_string_encode(u_char *p, ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ਵਸਤੂ);
ਸਥਿਰ u_char *ngx_json_integer_encode(u_char *p, ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ਵਸਤੂ);
ਸਥਿਰ u_char *ngx_json_boolean_encode(u_char *p, ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ਵਸਤੂ);
ਸਥਿਰ u_char *ngx_json_null_encode(u_char *p, ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ਵਸਤੂ);


ਸਥਿਰ ngx_json_encode_t  ngx_json_encode[] = {

    /* NGX_DATA_OBJECT_TYPE */
    { ngx_json_obj_ਲੰਮਾਈ,
      ngx_json_obj_encode },

    /* NGX_DATA_LIST_TYPE */
    { ngx_json_list_ਲੰਮਾਈ,
      ngx_json_list_encode },

    /* NGX_DATA_STRING_TYPE */
    { ngx_json_string_ਲੰਮਾਈ,
      ngx_json_string_encode },

    /* NGX_DATA_INTEGER_TYPE */
    { ngx_json_integer_ਲੰਮਾਈ,
      ngx_json_integer_encode },

    /* NGX_DATA_BOOLEAN_TYPE */
    { ngx_json_boolean_ਲੰਮਾਈ,
      ngx_json_boolean_encode },

    /* NGX_DATA_NULL_TYPE */
    { ngx_json_null_ਲੰਮਾਈ,
      ngx_json_null_encode },
};


ngx_buf_t *
ngx_json_render(ngx_pool_t *pool, ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ਵਸਤੂ)
{
    ਮਾਪ_t      ਲੰਮਾਈ;
    ngx_buf_t  *buf;

    ਲੰਮਾਈ = ngx_json_encode[ਵਸਤੂ->type].ਲੰਮਾਈ(ਵਸਤੂ);
    ਜੇ (ਲੰਮਾਈ == (ਮਾਪ_t) NGX_ERROR) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    buf = ngx_create_ਆਰਜ਼ੀ_buf(pool, ਲੰਮਾਈ);
    ਜੇ (buf == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    buf->ਆਖਰੀ = ngx_json_encode[ਵਸਤੂ->type].encode(buf->ਆਖਰੀ, ਵਸਤੂ);

    ਵਾਪਸ buf;
}


ਸਥਿਰ ssize_t
ngx_json_obj_ਲੰਮਾਈ(ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ਵਸਤੂ)
{
    ਮਾਪ_t            len, ਕੁੱਲ;
    ngx_ਅੰਕੜੇ_obj_t   *obj;
    ngx_ਅੰਕੜੇ_ਵਸਤੂ_t  *i;

    obj = &ਵਸਤੂ->ਅੰਕੜੇ.object;
    ਕੁੱਲ = ਆਕਾਰ("{}") - 1;
    i = obj->ਵਸਤੂ;

    ਜੇ (i) {
        ਲਈ ( ;; ) {
            ਜੇ (i->name.len > NGX_MAX_ਮਾਪ_T_ਮੁੱਲ - (ਆਕਾਰ("\"\":") - 1)) {
                ਵਾਪਸ NGX_ERROR;
            }

            len = ਆਕਾਰ("\"\":") - 1 + i->name.len;

            ਜੇ (len > NGX_MAX_ਮਾਪ_T_ਮੁੱਲ - ਕੁੱਲ) {
                ਵਾਪਸ NGX_ERROR;
            }

            ਕੁੱਲ += len;

            len = ngx_escape_json(ਨਿਰਾ, i->name.ਅੰਕੜੇ, i->name.len);

            ਜੇ (len > NGX_MAX_ਮਾਪ_T_ਮੁੱਲ - ਕੁੱਲ) {
                ਵਾਪਸ NGX_ERROR;
            }

            ਕੁੱਲ += len;

            len = ngx_json_encode[i->type].ਲੰਮਾਈ(i);
            ਜੇ (len == (ਮਾਪ_t) NGX_ERROR) {
                ਵਾਪਸ NGX_ERROR;
            }

            ਜੇ (len > NGX_MAX_ਮਾਪ_T_ਮੁੱਲ - ਕੁੱਲ) {
                ਵਾਪਸ NGX_ERROR;
            }

            ਕੁੱਲ += len;

            i = i->ਅਗਲਾ;

            ਜੇ (i == ਨਿਰਾ) {
                ਰੋਕੋ;
            }

            ਜੇ (ਕੁੱਲ > NGX_MAX_ਮਾਪ_T_ਮੁੱਲ - (ਆਕਾਰ(",") - 1)) {
                ਵਾਪਸ NGX_ERROR;
            }

            ਕੁੱਲ += ਆਕਾਰ(",") - 1;
        }
    }

    ਵਾਪਸ ਕੁੱਲ;
}


ਸਥਿਰ u_char *
ngx_json_obj_encode(u_char *p, ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ਵਸਤੂ)
{
    ngx_ਅੰਕੜੇ_obj_t   *obj;
    ngx_ਅੰਕੜੇ_ਵਸਤੂ_t  *i;

    obj = &ਵਸਤੂ->ਅੰਕੜੇ.object;
    i = obj->ਵਸਤੂ;

    *p++ = '{';

    ਜੇ (i) {
        ਲਈ ( ;; ) {
            *p++ = '"';

            p = (u_char *) ngx_escape_json(p, i->name.ਅੰਕੜੇ, i->name.len);

            *p++ = '"';
            *p++ = ':';

            p = ngx_json_encode[i->type].encode(p, i);

            i = i->ਅਗਲਾ;

            ਜੇ (i == ਨਿਰਾ) {
                ਰੋਕੋ;
            }

            *p++ = ',';
        }
    }

    *p++ = '}';

    ਵਾਪਸ p;
}


ਸਥਿਰ ssize_t
ngx_json_list_ਲੰਮਾਈ(ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ਵਸਤੂ)
{
    ਮਾਪ_t            len, ਕੁੱਲ;
    ngx_ਅੰਕੜੇ_obj_t   *obj;
    ngx_ਅੰਕੜੇ_ਵਸਤੂ_t  *i;

    obj = &ਵਸਤੂ->ਅੰਕੜੇ.object;
    ਕੁੱਲ = ਆਕਾਰ("[]") - 1;
    i = obj->ਵਸਤੂ;

    ਜੇ (i) {
        ਲਈ ( ;; ) {
            len = ngx_json_encode[i->type].ਲੰਮਾਈ(i);
            ਜੇ (len == (ਮਾਪ_t) NGX_ERROR) {
                ਵਾਪਸ NGX_ERROR;
            }

            ਜੇ (len > NGX_MAX_ਮਾਪ_T_ਮੁੱਲ - ਕੁੱਲ) {
                ਵਾਪਸ NGX_ERROR;
            }

            ਕੁੱਲ += len;

            i = i->ਅਗਲਾ;

            ਜੇ (i == ਨਿਰਾ) {
                ਰੋਕੋ;
            }

            ਜੇ (ਕੁੱਲ > NGX_MAX_ਮਾਪ_T_ਮੁੱਲ - (ਆਕਾਰ(",") - 1)) {
                ਵਾਪਸ NGX_ERROR;
            }

            ਕੁੱਲ += ਆਕਾਰ(",") - 1;
        }
    }

    ਵਾਪਸ ਕੁੱਲ;
}


ਸਥਿਰ u_char *
ngx_json_list_encode(u_char *p, ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ਵਸਤੂ)
{
    ngx_ਅੰਕੜੇ_obj_t   *obj;
    ngx_ਅੰਕੜੇ_ਵਸਤੂ_t  *i;

    obj = &ਵਸਤੂ->ਅੰਕੜੇ.object;
    i = obj->ਵਸਤੂ;

    *p++ = '[';

    ਜੇ (i) {
        ਲਈ ( ;; ) {
            p = ngx_json_encode[i->type].encode(p, i);

            i = i->ਅਗਲਾ;

            ਜੇ (i == ਨਿਰਾ) {
                ਰੋਕੋ;
            }

            *p++ = ',';
        }
    }

    *p++ = ']';

    ਵਾਪਸ p;
}


ਸਥਿਰ ssize_t
ngx_json_string_ਲੰਮਾਈ(ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ਵਸਤੂ)
{
    ਮਾਪ_t      len, ਕੁੱਲ;
    ngx_str_t  *str;

    str = &ਵਸਤੂ->ਅੰਕੜੇ.string;

    ਜੇ (str->len > NGX_MAX_ਮਾਪ_T_ਮੁੱਲ - (ਆਕਾਰ("\"\"") - 1)) {
        ਵਾਪਸ NGX_ERROR;
    }

    ਕੁੱਲ = ਆਕਾਰ("\"\"") - 1 + str->len;

    len = ngx_escape_json(ਨਿਰਾ, str->ਅੰਕੜੇ, str->len);

    ਜੇ (len > NGX_MAX_ਮਾਪ_T_ਮੁੱਲ - ਕੁੱਲ) {
        ਵਾਪਸ NGX_ERROR;
    }

    ਵਾਪਸ ਕੁੱਲ + len;
}


ਸਥਿਰ u_char *
ngx_json_string_encode(u_char *p, ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ਵਸਤੂ)
{
    ngx_str_t  *str;

    str = &ਵਸਤੂ->ਅੰਕੜੇ.string;

    *p++ = '"';

    p = (u_char *) ngx_escape_json(p, str->ਅੰਕੜੇ, str->len);

    *p++ = '"';

    ਵਾਪਸ p;
}


ਸਥਿਰ ssize_t
ngx_json_integer_ਲੰਮਾਈ(ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ਵਸਤੂ)
{
    ਵਾਪਸ NGX_INT64_LEN;
}


ਸਥਿਰ u_char *
ngx_json_integer_encode(u_char *p, ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ਵਸਤੂ)
{
    ਵਾਪਸ ngx_sprintf(p, "%L", ਵਸਤੂ->ਅੰਕੜੇ.integer);
}


ਸਥਿਰ ssize_t
ngx_json_boolean_ਲੰਮਾਈ(ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ਵਸਤੂ)
{
    ਵਾਪਸ ਆਕਾਰ("false") - 1;
}


ਸਥਿਰ u_char *
ngx_json_boolean_encode(u_char *p, ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ਵਸਤੂ)
{
    ਵਾਪਸ ਵਸਤੂ->ਅੰਕੜੇ.boolean ? ngx_cpymem(p, "true", 4)
                              : ngx_cpymem(p, "false", 5);
}


ਸਥਿਰ ssize_t
ngx_json_null_ਲੰਮਾਈ(ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ਵਸਤੂ)
{
    ਵਾਪਸ ਆਕਾਰ("null") - 1;
}


ਸਥਿਰ u_char *
ngx_json_null_encode(u_char *p, ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ਵਸਤੂ)
{
    ਵਾਪਸ ngx_cpymem(p, "null", 4);
}
