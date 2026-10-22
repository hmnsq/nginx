
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#وصف_ڪريو NGX_MAX_DYNAMIC_MODULES  128


ساكن ngx_uint_t ngx_module_اشاريو(ngx_چڪر_t *چڪر);
ساكن ngx_uint_t ngx_module_ctx_اشاريو(ngx_چڪر_t *چڪر, ngx_uint_t type,
    ngx_uint_t اشاريو);


ngx_uint_t         ngx_max_module;
ساكن ngx_uint_t  ngx_modules_n;


ngx_int_t
ngx_preinit_modules(فراغ)
{
    ngx_uint_t  i;

    لكل (i = 0; ngx_modules[i]; i++) {
        ngx_modules[i]->اشاريو = i;
        ngx_modules[i]->name = ngx_module_نالا[i];
    }

    ngx_modules_n = i;
    ngx_max_module = ngx_modules_n + NGX_MAX_DYNAMIC_MODULES;

    أرجع NGX_OK;
}


ngx_int_t
ngx_چڪر_modules(ngx_چڪر_t *چڪر)
{
    /*
     * create a list of modules to be used for this cycle,
     * copy static modules to it
     */

    چڪر->modules = ngx_pcalloc(چڪر->pool, (ngx_max_module + 1)
                                              * حجم(ngx_module_t *));
    إذا (چڪر->modules == عدم) {
        أرجع NGX_ERROR;
    }

    ngx_memcpy(چڪر->modules, ngx_modules,
               ngx_modules_n * حجم(ngx_module_t *));

    چڪر->modules_n = ngx_modules_n;

    أرجع NGX_OK;
}


ngx_int_t
ngx_init_modules(ngx_چڪر_t *چڪر)
{
    ngx_uint_t  i;

    لكل (i = 0; چڪر->modules[i]; i++) {
        إذا (چڪر->modules[i]->init_module) {
            إذا (چڪر->modules[i]->init_module(چڪر) != NGX_OK) {
                أرجع NGX_ERROR;
            }
        }
    }

    أرجع NGX_OK;
}


ngx_int_t
ngx_ڳڻپ_modules(ngx_چڪر_t *چڪر, ngx_uint_t type)
{
    ngx_uint_t     i, ايندڙ, max;
    ngx_module_t  *module;

    ايندڙ = 0;
    max = 0;

    /* count appropriate modules, set up their indices */

    لكل (i = 0; چڪر->modules[i]; i++) {
        module = چڪر->modules[i];

        إذا (module->type != type) {
            استمر;
        }

        إذا (module->ctx_اشاريو != NGX_MODULE_UNSET_اشاريو) {

            /* if ctx_index was assigned, preserve it */

            إذا (module->ctx_اشاريو > max) {
                max = module->ctx_اشاريو;
            }

            إذا (module->ctx_اشاريو == ايندڙ) {
                ايندڙ++;
            }

            استمر;
        }

        /* search for some free index */

        module->ctx_اشاريو = ngx_module_ctx_اشاريو(چڪر, type, ايندڙ);

        إذا (module->ctx_اشاريو > max) {
            max = module->ctx_اشاريو;
        }

        ايندڙ = module->ctx_اشاريو + 1;
    }

    /*
     * make sure the number returned is big enough for previous
     * cycle as well, else there will be problems if the number
     * will be stored in a global variable (as it's used to be)
     * and we'll have to roll back to the previous cycle
     */

    إذا (چڪر->old_چڪر && چڪر->old_چڪر->modules) {

        لكل (i = 0; چڪر->old_چڪر->modules[i]; i++) {
            module = چڪر->old_چڪر->modules[i];

            إذا (module->type != type) {
                استمر;
            }

            إذا (module->ctx_اشاريو > max) {
                max = module->ctx_اشاريو;
            }
        }
    }

    /* prevent loading of additional modules */

    چڪر->modules_استعمال_ٿيل = 1;

    أرجع max + 1;
}


ngx_int_t
ngx_جوڙيو_module(ngx_conf_t *cf, ngx_str_t *file, ngx_module_t *module,
    حرف **ترتيب)
{
    فراغ               *rv;
    ngx_uint_t          i, m, before;
    ngx_core_module_t  *core_module;

    إذا (cf->چڪر->modules_n >= ngx_max_module) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "too many modules loaded");
        أرجع NGX_ERROR;
    }

    إذا (module->نسخو != nginx_نسخو) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "module \"%V\" version %ui instead of %ui",
                           file, module->نسخو, (ngx_uint_t) nginx_نسخو);
        أرجع NGX_ERROR;
    }

    إذا (ngx_strcmp(module->signature, NGX_MODULE_SIGNATURE) != 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "module \"%V\" is not binary compatible",
                           file);
        أرجع NGX_ERROR;
    }

    لكل (m = 0; cf->چڪر->modules[m]; m++) {
        إذا (ngx_strcmp(cf->چڪر->modules[m]->name, module->name) == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "module \"%s\" is already loaded",
                               module->name);
            أرجع NGX_ERROR;
        }
    }

    /*
     * if the module wasn't previously loaded, assign an index
     */

    إذا (module->اشاريو == NGX_MODULE_UNSET_اشاريو) {
        module->اشاريو = ngx_module_اشاريو(cf->چڪر);

        إذا (module->اشاريو >= ngx_max_module) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "too many modules loaded");
            أرجع NGX_ERROR;
        }
    }

    /*
     * put the module into the cycle->modules array
     */

    before = cf->چڪر->modules_n;

    إذا (ترتيب) {
        لكل (i = 0; ترتيب[i]; i++) {
            إذا (ngx_strcmp(ترتيب[i], module->name) == 0) {
                i++;
                اكسر;
            }
        }

        لكل ( /* void */ ; ترتيب[i]; i++) {

#إذا 0
            ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->لاگرٿم, 0,
                           "module: %s before %s",
                           module->name, ترتيب[i]);
#جيڪڏهن_پورو

            لكل (m = 0; m < before; m++) {
                إذا (ngx_strcmp(cf->چڪر->modules[m]->name, ترتيب[i]) == 0) {

                    ngx_log_debug3(NGX_LOG_DEBUG_CORE, cf->لاگرٿم, 0,
                                   "module: %s before %s:%i",
                                   module->name, ترتيب[i], m);

                    before = m;
                    اكسر;
                }
            }
        }
    }

    /* put the module before modules[before] */

    إذا (before != cf->چڪر->modules_n) {
        ngx_memmove(&cf->چڪر->modules[before + 1],
                    &cf->چڪر->modules[before],
                    (cf->چڪر->modules_n - before) * حجم(ngx_module_t *));
    }

    cf->چڪر->modules[before] = module;
    cf->چڪر->modules_n++;

    إذا (module->type == NGX_CORE_MODULE) {

        /*
         * we are smart enough to initialize core modules;
         * other modules are expected to be loaded before
         * initialization - e.g., http modules must be loaded
         * before http{} block
         */

        core_module = module->ctx;

        إذا (core_module->create_conf) {
            rv = core_module->create_conf(cf->چڪر);
            إذا (rv == عدم) {
                أرجع NGX_ERROR;
            }

            cf->چڪر->conf_ctx[module->اشاريو] = rv;
        }
    }

    أرجع NGX_OK;
}


ساكن ngx_uint_t
ngx_module_اشاريو(ngx_چڪر_t *چڪر)
{
    ngx_uint_t     i, اشاريو;
    ngx_module_t  *module;

    اشاريو = 0;

again:

    /* find an unused index */

    لكل (i = 0; چڪر->modules[i]; i++) {
        module = چڪر->modules[i];

        إذا (module->اشاريو == اشاريو) {
            اشاريو++;
            اذهب again;
        }
    }

    /* check previous cycle */

    إذا (چڪر->old_چڪر && چڪر->old_چڪر->modules) {

        لكل (i = 0; چڪر->old_چڪر->modules[i]; i++) {
            module = چڪر->old_چڪر->modules[i];

            إذا (module->اشاريو == اشاريو) {
                اشاريو++;
                اذهب again;
            }
        }
    }

    أرجع اشاريو;
}


ساكن ngx_uint_t
ngx_module_ctx_اشاريو(ngx_چڪر_t *چڪر, ngx_uint_t type, ngx_uint_t اشاريو)
{
    ngx_uint_t     i;
    ngx_module_t  *module;

again:

    /* find an unused ctx_index */

    لكل (i = 0; چڪر->modules[i]; i++) {
        module = چڪر->modules[i];

        إذا (module->type != type) {
            استمر;
        }

        إذا (module->ctx_اشاريو == اشاريو) {
            اشاريو++;
            اذهب again;
        }
    }

    /* check previous cycle */

    إذا (چڪر->old_چڪر && چڪر->old_چڪر->modules) {

        لكل (i = 0; چڪر->old_چڪر->modules[i]; i++) {
            module = چڪر->old_چڪر->modules[i];

            إذا (module->type != type) {
                استمر;
            }

            إذا (module->ctx_اشاريو == اشاريو) {
                اشاريو++;
                اذهب again;
            }
        }
    }

    أرجع اشاريو;
}
