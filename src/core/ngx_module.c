
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MAX_DYNAMIC_MODULES  128


ಸ್ಥಿರ ngx_uint_t ngx_module_ಸೂಚ್ಯಂಕ(ngx_ಚಕ್ರ_t *ಚಕ್ರ);
ಸ್ಥಿರ ngx_uint_t ngx_module_ctx_ಸೂಚ್ಯಂಕ(ngx_ಚಕ್ರ_t *ಚಕ್ರ, ngx_uint_t type,
    ngx_uint_t ಸೂಚ್ಯಂಕ);


ngx_uint_t         ngx_max_module;
ಸ್ಥಿರ ngx_uint_t  ngx_modules_n;


ngx_int_t
ngx_preinit_modules(ಶೂನ್ಯ)
{
    ngx_uint_t  i;

    ಗಾಗಿ (i = 0; ngx_modules[i]; i++) {
        ngx_modules[i]->ಸೂಚ್ಯಂಕ = i;
        ngx_modules[i]->name = ngx_module_ಹೆಸರುಗಳು[i];
    }

    ngx_modules_n = i;
    ngx_max_module = ngx_modules_n + NGX_MAX_DYNAMIC_MODULES;

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ngx_int_t
ngx_ಚಕ್ರ_modules(ngx_ಚಕ್ರ_t *ಚಕ್ರ)
{
    /*
     * create a list of modules to be used for this cycle,
     * copy static modules to it
     */

    ಚಕ್ರ->modules = ngx_pcalloc(ಚಕ್ರ->pool, (ngx_max_module + 1)
                                              * ಗಾತ್ರ(ngx_module_t *));
    ಆದರೆ (ಚಕ್ರ->modules == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ngx_memcpy(ಚಕ್ರ->modules, ngx_modules,
               ngx_modules_n * ಗಾತ್ರ(ngx_module_t *));

    ಚಕ್ರ->modules_n = ngx_modules_n;

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ngx_int_t
ngx_init_modules(ngx_ಚಕ್ರ_t *ಚಕ್ರ)
{
    ngx_uint_t  i;

    ಗಾಗಿ (i = 0; ಚಕ್ರ->modules[i]; i++) {
        ಆದರೆ (ಚಕ್ರ->modules[i]->init_module) {
            ಆದರೆ (ಚಕ್ರ->modules[i]->init_module(ಚಕ್ರ) != NGX_OK) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }
        }
    }

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ngx_int_t
ngx_ಎಣಿಕೆ_modules(ngx_ಚಕ್ರ_t *ಚಕ್ರ, ngx_uint_t type)
{
    ngx_uint_t     i, ಮುಂದಿನ, max;
    ngx_module_t  *module;

    ಮುಂದಿನ = 0;
    max = 0;

    /* count appropriate modules, set up their indices */

    ಗಾಗಿ (i = 0; ಚಕ್ರ->modules[i]; i++) {
        module = ಚಕ್ರ->modules[i];

        ಆದರೆ (module->type != type) {
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (module->ctx_ಸೂಚ್ಯಂಕ != NGX_MODULE_UNSET_ಸೂಚ್ಯಂಕ) {

            /* if ctx_index was assigned, preserve it */

            ಆದರೆ (module->ctx_ಸೂಚ್ಯಂಕ > max) {
                max = module->ctx_ಸೂಚ್ಯಂಕ;
            }

            ಆದರೆ (module->ctx_ಸೂಚ್ಯಂಕ == ಮುಂದಿನ) {
                ಮುಂದಿನ++;
            }

            ಮುಂದುವರಿ;
        }

        /* search for some free index */

        module->ctx_ಸೂಚ್ಯಂಕ = ngx_module_ctx_ಸೂಚ್ಯಂಕ(ಚಕ್ರ, type, ಮುಂದಿನ);

        ಆದರೆ (module->ctx_ಸೂಚ್ಯಂಕ > max) {
            max = module->ctx_ಸೂಚ್ಯಂಕ;
        }

        ಮುಂದಿನ = module->ctx_ಸೂಚ್ಯಂಕ + 1;
    }

    /*
     * make sure the number returned is big enough for previous
     * cycle as well, else there will be problems if the number
     * will be stored in a global variable (as it's used to be)
     * and we'll have to roll back to the previous cycle
     */

    ಆದರೆ (ಚಕ್ರ->old_ಚಕ್ರ && ಚಕ್ರ->old_ಚಕ್ರ->modules) {

        ಗಾಗಿ (i = 0; ಚಕ್ರ->old_ಚಕ್ರ->modules[i]; i++) {
            module = ಚಕ್ರ->old_ಚಕ್ರ->modules[i];

            ಆದರೆ (module->type != type) {
                ಮುಂದುವರಿ;
            }

            ಆದರೆ (module->ctx_ಸೂಚ್ಯಂಕ > max) {
                max = module->ctx_ಸೂಚ್ಯಂಕ;
            }
        }
    }

    /* prevent loading of additional modules */

    ಚಕ್ರ->modules_ಬಳಸಿದ = 1;

    ಹಿಂತಿರುಗಿ max + 1;
}


ngx_int_t
ngx_ಕೂಡಿಸು_module(ngx_conf_t *cf, ngx_str_t *file, ngx_module_t *module,
    ಅಕ್ಷರ **ಕ್ರಮ)
{
    ಶೂನ್ಯ               *rv;
    ngx_uint_t          i, m, before;
    ngx_core_module_t  *core_module;

    ಆದರೆ (cf->ಚಕ್ರ->modules_n >= ngx_max_module) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "too many modules loaded");
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಆದರೆ (module->ಆವೃತ್ತಿ != nginx_ಆವೃತ್ತಿ) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "module \"%V\" version %ui instead of %ui",
                           file, module->ಆವೃತ್ತಿ, (ngx_uint_t) nginx_ಆವೃತ್ತಿ);
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಆದರೆ (ngx_strcmp(module->signature, NGX_MODULE_SIGNATURE) != 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "module \"%V\" is not binary compatible",
                           file);
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಗಾಗಿ (m = 0; cf->ಚಕ್ರ->modules[m]; m++) {
        ಆದರೆ (ngx_strcmp(cf->ಚಕ್ರ->modules[m]->name, module->name) == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "module \"%s\" is already loaded",
                               module->name);
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }
    }

    /*
     * if the module wasn't previously loaded, assign an index
     */

    ಆದರೆ (module->ಸೂಚ್ಯಂಕ == NGX_MODULE_UNSET_ಸೂಚ್ಯಂಕ) {
        module->ಸೂಚ್ಯಂಕ = ngx_module_ಸೂಚ್ಯಂಕ(cf->ಚಕ್ರ);

        ಆದರೆ (module->ಸೂಚ್ಯಂಕ >= ngx_max_module) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "too many modules loaded");
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }
    }

    /*
     * put the module into the cycle->modules array
     */

    before = cf->ಚಕ್ರ->modules_n;

    ಆದರೆ (ಕ್ರಮ) {
        ಗಾಗಿ (i = 0; ಕ್ರಮ[i]; i++) {
            ಆದರೆ (ngx_strcmp(ಕ್ರಮ[i], module->name) == 0) {
                i++;
                ನಿಲ್ಲಿಸು;
            }
        }

        ಗಾಗಿ ( /* void */ ; ಕ್ರಮ[i]; i++) {

#ಆದರೆ 0
            ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->ಲಘುಗಣಕ, 0,
                           "module: %s before %s",
                           module->name, ಕ್ರಮ[i]);
#ಆದರೆ_ಅಂತ್ಯ

            ಗಾಗಿ (m = 0; m < before; m++) {
                ಆದರೆ (ngx_strcmp(cf->ಚಕ್ರ->modules[m]->name, ಕ್ರಮ[i]) == 0) {

                    ngx_log_debug3(NGX_LOG_DEBUG_CORE, cf->ಲಘುಗಣಕ, 0,
                                   "module: %s before %s:%i",
                                   module->name, ಕ್ರಮ[i], m);

                    before = m;
                    ನಿಲ್ಲಿಸು;
                }
            }
        }
    }

    /* put the module before modules[before] */

    ಆದರೆ (before != cf->ಚಕ್ರ->modules_n) {
        ngx_memmove(&cf->ಚಕ್ರ->modules[before + 1],
                    &cf->ಚಕ್ರ->modules[before],
                    (cf->ಚಕ್ರ->modules_n - before) * ಗಾತ್ರ(ngx_module_t *));
    }

    cf->ಚಕ್ರ->modules[before] = module;
    cf->ಚಕ್ರ->modules_n++;

    ಆದರೆ (module->type == NGX_CORE_MODULE) {

        /*
         * we are smart enough to initialize core modules;
         * other modules are expected to be loaded before
         * initialization - e.g., http modules must be loaded
         * before http{} block
         */

        core_module = module->ctx;

        ಆದರೆ (core_module->create_conf) {
            rv = core_module->create_conf(cf->ಚಕ್ರ);
            ಆದರೆ (rv == ಖಾಲಿ) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            cf->ಚಕ್ರ->conf_ctx[module->ಸೂಚ್ಯಂಕ] = rv;
        }
    }

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ಸ್ಥಿರ ngx_uint_t
ngx_module_ಸೂಚ್ಯಂಕ(ngx_ಚಕ್ರ_t *ಚಕ್ರ)
{
    ngx_uint_t     i, ಸೂಚ್ಯಂಕ;
    ngx_module_t  *module;

    ಸೂಚ್ಯಂಕ = 0;

again:

    /* find an unused index */

    ಗಾಗಿ (i = 0; ಚಕ್ರ->modules[i]; i++) {
        module = ಚಕ್ರ->modules[i];

        ಆದರೆ (module->ಸೂಚ್ಯಂಕ == ಸೂಚ್ಯಂಕ) {
            ಸೂಚ್ಯಂಕ++;
            ಹೋಗು again;
        }
    }

    /* check previous cycle */

    ಆದರೆ (ಚಕ್ರ->old_ಚಕ್ರ && ಚಕ್ರ->old_ಚಕ್ರ->modules) {

        ಗಾಗಿ (i = 0; ಚಕ್ರ->old_ಚಕ್ರ->modules[i]; i++) {
            module = ಚಕ್ರ->old_ಚಕ್ರ->modules[i];

            ಆದರೆ (module->ಸೂಚ್ಯಂಕ == ಸೂಚ್ಯಂಕ) {
                ಸೂಚ್ಯಂಕ++;
                ಹೋಗು again;
            }
        }
    }

    ಹಿಂತಿರುಗಿ ಸೂಚ್ಯಂಕ;
}


ಸ್ಥಿರ ngx_uint_t
ngx_module_ctx_ಸೂಚ್ಯಂಕ(ngx_ಚಕ್ರ_t *ಚಕ್ರ, ngx_uint_t type, ngx_uint_t ಸೂಚ್ಯಂಕ)
{
    ngx_uint_t     i;
    ngx_module_t  *module;

again:

    /* find an unused ctx_index */

    ಗಾಗಿ (i = 0; ಚಕ್ರ->modules[i]; i++) {
        module = ಚಕ್ರ->modules[i];

        ಆದರೆ (module->type != type) {
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (module->ctx_ಸೂಚ್ಯಂಕ == ಸೂಚ್ಯಂಕ) {
            ಸೂಚ್ಯಂಕ++;
            ಹೋಗು again;
        }
    }

    /* check previous cycle */

    ಆದರೆ (ಚಕ್ರ->old_ಚಕ್ರ && ಚಕ್ರ->old_ಚಕ್ರ->modules) {

        ಗಾಗಿ (i = 0; ಚಕ್ರ->old_ಚಕ್ರ->modules[i]; i++) {
            module = ಚಕ್ರ->old_ಚಕ್ರ->modules[i];

            ಆದರೆ (module->type != type) {
                ಮುಂದುವರಿ;
            }

            ಆದರೆ (module->ctx_ಸೂಚ್ಯಂಕ == ಸೂಚ್ಯಂಕ) {
                ಸೂಚ್ಯಂಕ++;
                ಹೋಗು again;
            }
        }
    }

    ಹಿಂತಿರುಗಿ ಸೂಚ್ಯಂಕ;
}
