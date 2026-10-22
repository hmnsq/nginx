
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MAX_DYNAMIC_MODULES  128


ਸਥਿਰ ngx_uint_t ngx_module_ਸੂਚਕਾਂਕ(ngx_ਚੱਕਰ_t *ਚੱਕਰ);
ਸਥਿਰ ngx_uint_t ngx_module_ctx_ਸੂਚਕਾਂਕ(ngx_ਚੱਕਰ_t *ਚੱਕਰ, ngx_uint_t type,
    ngx_uint_t ਸੂਚਕਾਂਕ);


ngx_uint_t         ngx_max_module;
ਸਥਿਰ ngx_uint_t  ngx_modules_n;


ngx_int_t
ngx_preinit_modules(ਖਾਲੀ)
{
    ngx_uint_t  i;

    ਲਈ (i = 0; ngx_modules[i]; i++) {
        ngx_modules[i]->ਸੂਚਕਾਂਕ = i;
        ngx_modules[i]->name = ngx_module_ਨਾਂ[i];
    }

    ngx_modules_n = i;
    ngx_max_module = ngx_modules_n + NGX_MAX_DYNAMIC_MODULES;

    ਵਾਪਸ NGX_OK;
}


ngx_int_t
ngx_ਚੱਕਰ_modules(ngx_ਚੱਕਰ_t *ਚੱਕਰ)
{
    /*
     * create a list of modules to be used for this cycle,
     * copy static modules to it
     */

    ਚੱਕਰ->modules = ngx_pcalloc(ਚੱਕਰ->pool, (ngx_max_module + 1)
                                              * ਆਕਾਰ(ngx_module_t *));
    ਜੇ (ਚੱਕਰ->modules == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    ngx_memcpy(ਚੱਕਰ->modules, ngx_modules,
               ngx_modules_n * ਆਕਾਰ(ngx_module_t *));

    ਚੱਕਰ->modules_n = ngx_modules_n;

    ਵਾਪਸ NGX_OK;
}


ngx_int_t
ngx_init_modules(ngx_ਚੱਕਰ_t *ਚੱਕਰ)
{
    ngx_uint_t  i;

    ਲਈ (i = 0; ਚੱਕਰ->modules[i]; i++) {
        ਜੇ (ਚੱਕਰ->modules[i]->init_module) {
            ਜੇ (ਚੱਕਰ->modules[i]->init_module(ਚੱਕਰ) != NGX_OK) {
                ਵਾਪਸ NGX_ERROR;
            }
        }
    }

    ਵਾਪਸ NGX_OK;
}


ngx_int_t
ngx_ਗਿਣਤੀ_modules(ngx_ਚੱਕਰ_t *ਚੱਕਰ, ngx_uint_t type)
{
    ngx_uint_t     i, ਅਗਲਾ, max;
    ngx_module_t  *module;

    ਅਗਲਾ = 0;
    max = 0;

    /* count appropriate modules, set up their indices */

    ਲਈ (i = 0; ਚੱਕਰ->modules[i]; i++) {
        module = ਚੱਕਰ->modules[i];

        ਜੇ (module->type != type) {
            ਜਾਰੀ;
        }

        ਜੇ (module->ctx_ਸੂਚਕਾਂਕ != NGX_MODULE_UNSET_ਸੂਚਕਾਂਕ) {

            /* if ctx_index was assigned, preserve it */

            ਜੇ (module->ctx_ਸੂਚਕਾਂਕ > max) {
                max = module->ctx_ਸੂਚਕਾਂਕ;
            }

            ਜੇ (module->ctx_ਸੂਚਕਾਂਕ == ਅਗਲਾ) {
                ਅਗਲਾ++;
            }

            ਜਾਰੀ;
        }

        /* search for some free index */

        module->ctx_ਸੂਚਕਾਂਕ = ngx_module_ctx_ਸੂਚਕਾਂਕ(ਚੱਕਰ, type, ਅਗਲਾ);

        ਜੇ (module->ctx_ਸੂਚਕਾਂਕ > max) {
            max = module->ctx_ਸੂਚਕਾਂਕ;
        }

        ਅਗਲਾ = module->ctx_ਸੂਚਕਾਂਕ + 1;
    }

    /*
     * make sure the number returned is big enough for previous
     * cycle as well, else there will be problems if the number
     * will be stored in a global variable (as it's used to be)
     * and we'll have to roll back to the previous cycle
     */

    ਜੇ (ਚੱਕਰ->old_ਚੱਕਰ && ਚੱਕਰ->old_ਚੱਕਰ->modules) {

        ਲਈ (i = 0; ਚੱਕਰ->old_ਚੱਕਰ->modules[i]; i++) {
            module = ਚੱਕਰ->old_ਚੱਕਰ->modules[i];

            ਜੇ (module->type != type) {
                ਜਾਰੀ;
            }

            ਜੇ (module->ctx_ਸੂਚਕਾਂਕ > max) {
                max = module->ctx_ਸੂਚਕਾਂਕ;
            }
        }
    }

    /* prevent loading of additional modules */

    ਚੱਕਰ->modules_ਵਰਤਿਆ = 1;

    ਵਾਪਸ max + 1;
}


ngx_int_t
ngx_ਜੋੜੋ_module(ngx_conf_t *cf, ngx_str_t *file, ngx_module_t *module,
    ਅੱਖਰ **ਕ੍ਰਮ)
{
    ਖਾਲੀ               *rv;
    ngx_uint_t          i, m, before;
    ngx_core_module_t  *core_module;

    ਜੇ (cf->ਚੱਕਰ->modules_n >= ngx_max_module) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "too many modules loaded");
        ਵਾਪਸ NGX_ERROR;
    }

    ਜੇ (module->ਸੰਸਕਰਣ != nginx_ਸੰਸਕਰਣ) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "module \"%V\" version %ui instead of %ui",
                           file, module->ਸੰਸਕਰਣ, (ngx_uint_t) nginx_ਸੰਸਕਰਣ);
        ਵਾਪਸ NGX_ERROR;
    }

    ਜੇ (ngx_strcmp(module->signature, NGX_MODULE_SIGNATURE) != 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "module \"%V\" is not binary compatible",
                           file);
        ਵਾਪਸ NGX_ERROR;
    }

    ਲਈ (m = 0; cf->ਚੱਕਰ->modules[m]; m++) {
        ਜੇ (ngx_strcmp(cf->ਚੱਕਰ->modules[m]->name, module->name) == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "module \"%s\" is already loaded",
                               module->name);
            ਵਾਪਸ NGX_ERROR;
        }
    }

    /*
     * if the module wasn't previously loaded, assign an index
     */

    ਜੇ (module->ਸੂਚਕਾਂਕ == NGX_MODULE_UNSET_ਸੂਚਕਾਂਕ) {
        module->ਸੂਚਕਾਂਕ = ngx_module_ਸੂਚਕਾਂਕ(cf->ਚੱਕਰ);

        ਜੇ (module->ਸੂਚਕਾਂਕ >= ngx_max_module) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "too many modules loaded");
            ਵਾਪਸ NGX_ERROR;
        }
    }

    /*
     * put the module into the cycle->modules array
     */

    before = cf->ਚੱਕਰ->modules_n;

    ਜੇ (ਕ੍ਰਮ) {
        ਲਈ (i = 0; ਕ੍ਰਮ[i]; i++) {
            ਜੇ (ngx_strcmp(ਕ੍ਰਮ[i], module->name) == 0) {
                i++;
                ਰੋਕੋ;
            }
        }

        ਲਈ ( /* void */ ; ਕ੍ਰਮ[i]; i++) {

#ਜੇ 0
            ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->ਲਘੂਗਣਕ, 0,
                           "module: %s before %s",
                           module->name, ਕ੍ਰਮ[i]);
#ਜੇ_ਅੰਤ

            ਲਈ (m = 0; m < before; m++) {
                ਜੇ (ngx_strcmp(cf->ਚੱਕਰ->modules[m]->name, ਕ੍ਰਮ[i]) == 0) {

                    ngx_log_debug3(NGX_LOG_DEBUG_CORE, cf->ਲਘੂਗਣਕ, 0,
                                   "module: %s before %s:%i",
                                   module->name, ਕ੍ਰਮ[i], m);

                    before = m;
                    ਰੋਕੋ;
                }
            }
        }
    }

    /* put the module before modules[before] */

    ਜੇ (before != cf->ਚੱਕਰ->modules_n) {
        ngx_memmove(&cf->ਚੱਕਰ->modules[before + 1],
                    &cf->ਚੱਕਰ->modules[before],
                    (cf->ਚੱਕਰ->modules_n - before) * ਆਕਾਰ(ngx_module_t *));
    }

    cf->ਚੱਕਰ->modules[before] = module;
    cf->ਚੱਕਰ->modules_n++;

    ਜੇ (module->type == NGX_CORE_MODULE) {

        /*
         * we are smart enough to initialize core modules;
         * other modules are expected to be loaded before
         * initialization - e.g., http modules must be loaded
         * before http{} block
         */

        core_module = module->ctx;

        ਜੇ (core_module->create_conf) {
            rv = core_module->create_conf(cf->ਚੱਕਰ);
            ਜੇ (rv == ਨਿਰਾ) {
                ਵਾਪਸ NGX_ERROR;
            }

            cf->ਚੱਕਰ->conf_ctx[module->ਸੂਚਕਾਂਕ] = rv;
        }
    }

    ਵਾਪਸ NGX_OK;
}


ਸਥਿਰ ngx_uint_t
ngx_module_ਸੂਚਕਾਂਕ(ngx_ਚੱਕਰ_t *ਚੱਕਰ)
{
    ngx_uint_t     i, ਸੂਚਕਾਂਕ;
    ngx_module_t  *module;

    ਸੂਚਕਾਂਕ = 0;

again:

    /* find an unused index */

    ਲਈ (i = 0; ਚੱਕਰ->modules[i]; i++) {
        module = ਚੱਕਰ->modules[i];

        ਜੇ (module->ਸੂਚਕਾਂਕ == ਸੂਚਕਾਂਕ) {
            ਸੂਚਕਾਂਕ++;
            ਜਾਓ again;
        }
    }

    /* check previous cycle */

    ਜੇ (ਚੱਕਰ->old_ਚੱਕਰ && ਚੱਕਰ->old_ਚੱਕਰ->modules) {

        ਲਈ (i = 0; ਚੱਕਰ->old_ਚੱਕਰ->modules[i]; i++) {
            module = ਚੱਕਰ->old_ਚੱਕਰ->modules[i];

            ਜੇ (module->ਸੂਚਕਾਂਕ == ਸੂਚਕਾਂਕ) {
                ਸੂਚਕਾਂਕ++;
                ਜਾਓ again;
            }
        }
    }

    ਵਾਪਸ ਸੂਚਕਾਂਕ;
}


ਸਥਿਰ ngx_uint_t
ngx_module_ctx_ਸੂਚਕਾਂਕ(ngx_ਚੱਕਰ_t *ਚੱਕਰ, ngx_uint_t type, ngx_uint_t ਸੂਚਕਾਂਕ)
{
    ngx_uint_t     i;
    ngx_module_t  *module;

again:

    /* find an unused ctx_index */

    ਲਈ (i = 0; ਚੱਕਰ->modules[i]; i++) {
        module = ਚੱਕਰ->modules[i];

        ਜੇ (module->type != type) {
            ਜਾਰੀ;
        }

        ਜੇ (module->ctx_ਸੂਚਕਾਂਕ == ਸੂਚਕਾਂਕ) {
            ਸੂਚਕਾਂਕ++;
            ਜਾਓ again;
        }
    }

    /* check previous cycle */

    ਜੇ (ਚੱਕਰ->old_ਚੱਕਰ && ਚੱਕਰ->old_ਚੱਕਰ->modules) {

        ਲਈ (i = 0; ਚੱਕਰ->old_ਚੱਕਰ->modules[i]; i++) {
            module = ਚੱਕਰ->old_ਚੱਕਰ->modules[i];

            ਜੇ (module->type != type) {
                ਜਾਰੀ;
            }

            ਜੇ (module->ctx_ਸੂਚਕਾਂਕ == ਸੂਚਕਾਂਕ) {
                ਸੂਚਕਾਂਕ++;
                ਜਾਓ again;
            }
        }
    }

    ਵਾਪਸ ਸੂਚਕਾਂਕ;
}
