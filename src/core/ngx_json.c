
/*
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ssize_t   (*ngx_json_ಉದ್ದಳತೆ_pt)(ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ಅಂಶ);
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ u_char   *(*ngx_json_encode_pt)(u_char *p, ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ಅಂಶ);


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_json_ಉದ್ದಳತೆ_pt        ಉದ್ದಳತೆ;
    ngx_json_encode_pt        encode;
} ngx_json_encode_t;


ಸ್ಥಿರ ssize_t ngx_json_obj_ಉದ್ದಳತೆ(ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ಅಂಶ);
ಸ್ಥಿರ ssize_t ngx_json_list_ಉದ್ದಳತೆ(ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ಅಂಶ);
ಸ್ಥಿರ ssize_t ngx_json_string_ಉದ್ದಳತೆ(ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ಅಂಶ);
ಸ್ಥಿರ ssize_t ngx_json_integer_ಉದ್ದಳತೆ(ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ಅಂಶ);
ಸ್ಥಿರ ssize_t ngx_json_boolean_ಉದ್ದಳತೆ(ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ಅಂಶ);
ಸ್ಥಿರ ssize_t ngx_json_null_ಉದ್ದಳತೆ(ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ಅಂಶ);

ಸ್ಥಿರ u_char *ngx_json_obj_encode(u_char *p, ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ಅಂಶ);
ಸ್ಥಿರ u_char *ngx_json_list_encode(u_char *p, ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ಅಂಶ);
ಸ್ಥಿರ u_char *ngx_json_string_encode(u_char *p, ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ಅಂಶ);
ಸ್ಥಿರ u_char *ngx_json_integer_encode(u_char *p, ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ಅಂಶ);
ಸ್ಥಿರ u_char *ngx_json_boolean_encode(u_char *p, ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ಅಂಶ);
ಸ್ಥಿರ u_char *ngx_json_null_encode(u_char *p, ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ಅಂಶ);


ಸ್ಥಿರ ngx_json_encode_t  ngx_json_encode[] = {

    /* NGX_DATA_OBJECT_TYPE */
    { ngx_json_obj_ಉದ್ದಳತೆ,
      ngx_json_obj_encode },

    /* NGX_DATA_LIST_TYPE */
    { ngx_json_list_ಉದ್ದಳತೆ,
      ngx_json_list_encode },

    /* NGX_DATA_STRING_TYPE */
    { ngx_json_string_ಉದ್ದಳತೆ,
      ngx_json_string_encode },

    /* NGX_DATA_INTEGER_TYPE */
    { ngx_json_integer_ಉದ್ದಳತೆ,
      ngx_json_integer_encode },

    /* NGX_DATA_BOOLEAN_TYPE */
    { ngx_json_boolean_ಉದ್ದಳತೆ,
      ngx_json_boolean_encode },

    /* NGX_DATA_NULL_TYPE */
    { ngx_json_null_ಉದ್ದಳತೆ,
      ngx_json_null_encode },
};


ngx_buf_t *
ngx_json_render(ngx_pool_t *pool, ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ಅಂಶ)
{
    ಗಾತ್ರ_t      ಉದ್ದಳತೆ;
    ngx_buf_t  *buf;

    ಉದ್ದಳತೆ = ngx_json_encode[ಅಂಶ->type].ಉದ್ದಳತೆ(ಅಂಶ);
    ಆದರೆ (ಉದ್ದಳತೆ == (ಗಾತ್ರ_t) NGX_ERROR) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    buf = ngx_create_ತಾತ್ಕಾಲಿಕ_buf(pool, ಉದ್ದಳತೆ);
    ಆದರೆ (buf == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    buf->ಕೊನೆಯ = ngx_json_encode[ಅಂಶ->type].encode(buf->ಕೊನೆಯ, ಅಂಶ);

    ಹಿಂತಿರುಗಿ buf;
}


ಸ್ಥಿರ ssize_t
ngx_json_obj_ಉದ್ದಳತೆ(ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ಅಂಶ)
{
    ಗಾತ್ರ_t            len, ಒಟ್ಟು;
    ngx_ದತ್ತಾಂಶ_obj_t   *obj;
    ngx_ದತ್ತಾಂಶ_ಅಂಶ_t  *i;

    obj = &ಅಂಶ->ದತ್ತಾಂಶ.object;
    ಒಟ್ಟು = ಗಾತ್ರ("{}") - 1;
    i = obj->ಅಂಶ;

    ಆದರೆ (i) {
        ಗಾಗಿ ( ;; ) {
            ಆದರೆ (i->name.len > NGX_MAX_ಗಾತ್ರ_T_ಮೌಲ್ಯ - (ಗಾತ್ರ("\"\":") - 1)) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            len = ಗಾತ್ರ("\"\":") - 1 + i->name.len;

            ಆದರೆ (len > NGX_MAX_ಗಾತ್ರ_T_ಮೌಲ್ಯ - ಒಟ್ಟು) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            ಒಟ್ಟು += len;

            len = ngx_escape_json(ಖಾಲಿ, i->name.ದತ್ತಾಂಶ, i->name.len);

            ಆದರೆ (len > NGX_MAX_ಗಾತ್ರ_T_ಮೌಲ್ಯ - ಒಟ್ಟು) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            ಒಟ್ಟು += len;

            len = ngx_json_encode[i->type].ಉದ್ದಳತೆ(i);
            ಆದರೆ (len == (ಗಾತ್ರ_t) NGX_ERROR) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            ಆದರೆ (len > NGX_MAX_ಗಾತ್ರ_T_ಮೌಲ್ಯ - ಒಟ್ಟು) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            ಒಟ್ಟು += len;

            i = i->ಮುಂದಿನ;

            ಆದರೆ (i == ಖಾಲಿ) {
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (ಒಟ್ಟು > NGX_MAX_ಗಾತ್ರ_T_ಮೌಲ್ಯ - (ಗಾತ್ರ(",") - 1)) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            ಒಟ್ಟು += ಗಾತ್ರ(",") - 1;
        }
    }

    ಹಿಂತಿರುಗಿ ಒಟ್ಟು;
}


ಸ್ಥಿರ u_char *
ngx_json_obj_encode(u_char *p, ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ಅಂಶ)
{
    ngx_ದತ್ತಾಂಶ_obj_t   *obj;
    ngx_ದತ್ತಾಂಶ_ಅಂಶ_t  *i;

    obj = &ಅಂಶ->ದತ್ತಾಂಶ.object;
    i = obj->ಅಂಶ;

    *p++ = '{';

    ಆದರೆ (i) {
        ಗಾಗಿ ( ;; ) {
            *p++ = '"';

            p = (u_char *) ngx_escape_json(p, i->name.ದತ್ತಾಂಶ, i->name.len);

            *p++ = '"';
            *p++ = ':';

            p = ngx_json_encode[i->type].encode(p, i);

            i = i->ಮುಂದಿನ;

            ಆದರೆ (i == ಖಾಲಿ) {
                ನಿಲ್ಲಿಸು;
            }

            *p++ = ',';
        }
    }

    *p++ = '}';

    ಹಿಂತಿರುಗಿ p;
}


ಸ್ಥಿರ ssize_t
ngx_json_list_ಉದ್ದಳತೆ(ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ಅಂಶ)
{
    ಗಾತ್ರ_t            len, ಒಟ್ಟು;
    ngx_ದತ್ತಾಂಶ_obj_t   *obj;
    ngx_ದತ್ತಾಂಶ_ಅಂಶ_t  *i;

    obj = &ಅಂಶ->ದತ್ತಾಂಶ.object;
    ಒಟ್ಟು = ಗಾತ್ರ("[]") - 1;
    i = obj->ಅಂಶ;

    ಆದರೆ (i) {
        ಗಾಗಿ ( ;; ) {
            len = ngx_json_encode[i->type].ಉದ್ದಳತೆ(i);
            ಆದರೆ (len == (ಗಾತ್ರ_t) NGX_ERROR) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            ಆದರೆ (len > NGX_MAX_ಗಾತ್ರ_T_ಮೌಲ್ಯ - ಒಟ್ಟು) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            ಒಟ್ಟು += len;

            i = i->ಮುಂದಿನ;

            ಆದರೆ (i == ಖಾಲಿ) {
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (ಒಟ್ಟು > NGX_MAX_ಗಾತ್ರ_T_ಮೌಲ್ಯ - (ಗಾತ್ರ(",") - 1)) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            ಒಟ್ಟು += ಗಾತ್ರ(",") - 1;
        }
    }

    ಹಿಂತಿರುಗಿ ಒಟ್ಟು;
}


ಸ್ಥಿರ u_char *
ngx_json_list_encode(u_char *p, ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ಅಂಶ)
{
    ngx_ದತ್ತಾಂಶ_obj_t   *obj;
    ngx_ದತ್ತಾಂಶ_ಅಂಶ_t  *i;

    obj = &ಅಂಶ->ದತ್ತಾಂಶ.object;
    i = obj->ಅಂಶ;

    *p++ = '[';

    ಆದರೆ (i) {
        ಗಾಗಿ ( ;; ) {
            p = ngx_json_encode[i->type].encode(p, i);

            i = i->ಮುಂದಿನ;

            ಆದರೆ (i == ಖಾಲಿ) {
                ನಿಲ್ಲಿಸು;
            }

            *p++ = ',';
        }
    }

    *p++ = ']';

    ಹಿಂತಿರುಗಿ p;
}


ಸ್ಥಿರ ssize_t
ngx_json_string_ಉದ್ದಳತೆ(ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ಅಂಶ)
{
    ಗಾತ್ರ_t      len, ಒಟ್ಟು;
    ngx_str_t  *str;

    str = &ಅಂಶ->ದತ್ತಾಂಶ.string;

    ಆದರೆ (str->len > NGX_MAX_ಗಾತ್ರ_T_ಮೌಲ್ಯ - (ಗಾತ್ರ("\"\"") - 1)) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಒಟ್ಟು = ಗಾತ್ರ("\"\"") - 1 + str->len;

    len = ngx_escape_json(ಖಾಲಿ, str->ದತ್ತಾಂಶ, str->len);

    ಆದರೆ (len > NGX_MAX_ಗಾತ್ರ_T_ಮೌಲ್ಯ - ಒಟ್ಟು) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಹಿಂತಿರುಗಿ ಒಟ್ಟು + len;
}


ಸ್ಥಿರ u_char *
ngx_json_string_encode(u_char *p, ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ಅಂಶ)
{
    ngx_str_t  *str;

    str = &ಅಂಶ->ದತ್ತಾಂಶ.string;

    *p++ = '"';

    p = (u_char *) ngx_escape_json(p, str->ದತ್ತಾಂಶ, str->len);

    *p++ = '"';

    ಹಿಂತಿರುಗಿ p;
}


ಸ್ಥಿರ ssize_t
ngx_json_integer_ಉದ್ದಳತೆ(ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ಅಂಶ)
{
    ಹಿಂತಿರುಗಿ NGX_INT64_LEN;
}


ಸ್ಥಿರ u_char *
ngx_json_integer_encode(u_char *p, ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ಅಂಶ)
{
    ಹಿಂತಿರುಗಿ ngx_sprintf(p, "%L", ಅಂಶ->ದತ್ತಾಂಶ.integer);
}


ಸ್ಥಿರ ssize_t
ngx_json_boolean_ಉದ್ದಳತೆ(ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ಅಂಶ)
{
    ಹಿಂತಿರುಗಿ ಗಾತ್ರ("false") - 1;
}


ಸ್ಥಿರ u_char *
ngx_json_boolean_encode(u_char *p, ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ಅಂಶ)
{
    ಹಿಂತಿರುಗಿ ಅಂಶ->ದತ್ತಾಂಶ.boolean ? ngx_cpymem(p, "true", 4)
                              : ngx_cpymem(p, "false", 5);
}


ಸ್ಥಿರ ssize_t
ngx_json_null_ಉದ್ದಳತೆ(ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ಅಂಶ)
{
    ಹಿಂತಿರುಗಿ ಗಾತ್ರ("null") - 1;
}


ಸ್ಥಿರ u_char *
ngx_json_null_encode(u_char *p, ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ಅಂಶ)
{
    ಹಿಂತಿರುಗಿ ngx_cpymem(p, "null", 4);
}
