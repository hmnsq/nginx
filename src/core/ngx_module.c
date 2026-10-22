
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


#تعریف_کرو NGX_MAX_DYNAMIC_MODULES  128


ساکن ngx_uint_t ngx_module_اشاریہ(ngx_چکر_t *چکر);
ساکن ngx_uint_t ngx_module_ctx_اشاریہ(ngx_چکر_t *چکر, ngx_uint_t type,
    ngx_uint_t اشاریہ);


ngx_uint_t         ngx_max_module;
ساکن ngx_uint_t  ngx_modules_n;


ngx_int_t
ngx_preinit_modules(خالی)
{
    ngx_uint_t  i;

    برائے (i = 0; ngx_modules[i]; i++) {
        ngx_modules[i]->اشاریہ = i;
        ngx_modules[i]->name = ngx_module_نام[i];
    }

    ngx_modules_n = i;
    ngx_max_module = ngx_modules_n + NGX_MAX_DYNAMIC_MODULES;

    واپس NGX_OK;
}


ngx_int_t
ngx_چکر_modules(ngx_چکر_t *چکر)
{
    /*
     * create a list of modules to be used for this cycle,
     * copy static modules to it
     */

    چکر->modules = ngx_pcalloc(چکر->pool, (ngx_max_module + 1)
                                              * حجم(ngx_module_t *));
    اگر (چکر->modules == تہی) {
        واپس NGX_ERROR;
    }

    ngx_memcpy(چکر->modules, ngx_modules,
               ngx_modules_n * حجم(ngx_module_t *));

    چکر->modules_n = ngx_modules_n;

    واپس NGX_OK;
}


ngx_int_t
ngx_init_modules(ngx_چکر_t *چکر)
{
    ngx_uint_t  i;

    برائے (i = 0; چکر->modules[i]; i++) {
        اگر (چکر->modules[i]->init_module) {
            اگر (چکر->modules[i]->init_module(چکر) != NGX_OK) {
                واپس NGX_ERROR;
            }
        }
    }

    واپس NGX_OK;
}


ngx_int_t
ngx_گنتی_modules(ngx_چکر_t *چکر, ngx_uint_t type)
{
    ngx_uint_t     i, اگلا, max;
    ngx_module_t  *module;

    اگلا = 0;
    max = 0;

    /* count appropriate modules, set up their indices */

    برائے (i = 0; چکر->modules[i]; i++) {
        module = چکر->modules[i];

        اگر (module->type != type) {
            جاری;
        }

        اگر (module->ctx_اشاریہ != NGX_MODULE_UNSET_اشاریہ) {

            /* if ctx_index was assigned, preserve it */

            اگر (module->ctx_اشاریہ > max) {
                max = module->ctx_اشاریہ;
            }

            اگر (module->ctx_اشاریہ == اگلا) {
                اگلا++;
            }

            جاری;
        }

        /* search for some free index */

        module->ctx_اشاریہ = ngx_module_ctx_اشاریہ(چکر, type, اگلا);

        اگر (module->ctx_اشاریہ > max) {
            max = module->ctx_اشاریہ;
        }

        اگلا = module->ctx_اشاریہ + 1;
    }

    /*
     * make sure the number returned is big enough for previous
     * cycle as well, else there will be problems if the number
     * will be stored in a global variable (as it's used to be)
     * and we'll have to roll back to the previous cycle
     */

    اگر (چکر->old_چکر && چکر->old_چکر->modules) {

        برائے (i = 0; چکر->old_چکر->modules[i]; i++) {
            module = چکر->old_چکر->modules[i];

            اگر (module->type != type) {
                جاری;
            }

            اگر (module->ctx_اشاریہ > max) {
                max = module->ctx_اشاریہ;
            }
        }
    }

    /* prevent loading of additional modules */

    چکر->modules_مستعمل = 1;

    واپس max + 1;
}


ngx_int_t
ngx_جوڑو_module(ngx_conf_t *cf, ngx_str_t *file, ngx_module_t *module,
    حرف **ترتیب)
{
    خالی               *rv;
    ngx_uint_t          i, m, before;
    ngx_core_module_t  *core_module;

    اگر (cf->چکر->modules_n >= ngx_max_module) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "too many modules loaded");
        واپس NGX_ERROR;
    }

    اگر (module->نسخہ != nginx_نسخہ) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "module \"%V\" version %ui instead of %ui",
                           file, module->نسخہ, (ngx_uint_t) nginx_نسخہ);
        واپس NGX_ERROR;
    }

    اگر (ngx_strcmp(module->signature, NGX_MODULE_SIGNATURE) != 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "module \"%V\" is not binary compatible",
                           file);
        واپس NGX_ERROR;
    }

    برائے (m = 0; cf->چکر->modules[m]; m++) {
        اگر (ngx_strcmp(cf->چکر->modules[m]->name, module->name) == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "module \"%s\" is already loaded",
                               module->name);
            واپس NGX_ERROR;
        }
    }

    /*
     * if the module wasn't previously loaded, assign an index
     */

    اگر (module->اشاریہ == NGX_MODULE_UNSET_اشاریہ) {
        module->اشاریہ = ngx_module_اشاریہ(cf->چکر);

        اگر (module->اشاریہ >= ngx_max_module) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "too many modules loaded");
            واپس NGX_ERROR;
        }
    }

    /*
     * put the module into the cycle->modules array
     */

    before = cf->چکر->modules_n;

    اگر (ترتیب) {
        برائے (i = 0; ترتیب[i]; i++) {
            اگر (ngx_strcmp(ترتیب[i], module->name) == 0) {
                i++;
                توڑو;
            }
        }

        برائے ( /* void */ ; ترتیب[i]; i++) {

#اگر 0
            ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->لوگارتھم, 0,
                           "module: %s before %s",
                           module->name, ترتیب[i]);
#اگر_ختم

            برائے (m = 0; m < before; m++) {
                اگر (ngx_strcmp(cf->چکر->modules[m]->name, ترتیب[i]) == 0) {

                    ngx_log_debug3(NGX_LOG_DEBUG_CORE, cf->لوگارتھم, 0,
                                   "module: %s before %s:%i",
                                   module->name, ترتیب[i], m);

                    before = m;
                    توڑو;
                }
            }
        }
    }

    /* put the module before modules[before] */

    اگر (before != cf->چکر->modules_n) {
        ngx_memmove(&cf->چکر->modules[before + 1],
                    &cf->چکر->modules[before],
                    (cf->چکر->modules_n - before) * حجم(ngx_module_t *));
    }

    cf->چکر->modules[before] = module;
    cf->چکر->modules_n++;

    اگر (module->type == NGX_CORE_MODULE) {

        /*
         * we are smart enough to initialize core modules;
         * other modules are expected to be loaded before
         * initialization - e.g., http modules must be loaded
         * before http{} block
         */

        core_module = module->ctx;

        اگر (core_module->create_conf) {
            rv = core_module->create_conf(cf->چکر);
            اگر (rv == تہی) {
                واپس NGX_ERROR;
            }

            cf->چکر->conf_ctx[module->اشاریہ] = rv;
        }
    }

    واپس NGX_OK;
}


ساکن ngx_uint_t
ngx_module_اشاریہ(ngx_چکر_t *چکر)
{
    ngx_uint_t     i, اشاریہ;
    ngx_module_t  *module;

    اشاریہ = 0;

again:

    /* find an unused index */

    برائے (i = 0; چکر->modules[i]; i++) {
        module = چکر->modules[i];

        اگر (module->اشاریہ == اشاریہ) {
            اشاریہ++;
            جاؤ again;
        }
    }

    /* check previous cycle */

    اگر (چکر->old_چکر && چکر->old_چکر->modules) {

        برائے (i = 0; چکر->old_چکر->modules[i]; i++) {
            module = چکر->old_چکر->modules[i];

            اگر (module->اشاریہ == اشاریہ) {
                اشاریہ++;
                جاؤ again;
            }
        }
    }

    واپس اشاریہ;
}


ساکن ngx_uint_t
ngx_module_ctx_اشاریہ(ngx_چکر_t *چکر, ngx_uint_t type, ngx_uint_t اشاریہ)
{
    ngx_uint_t     i;
    ngx_module_t  *module;

again:

    /* find an unused ctx_index */

    برائے (i = 0; چکر->modules[i]; i++) {
        module = چکر->modules[i];

        اگر (module->type != type) {
            جاری;
        }

        اگر (module->ctx_اشاریہ == اشاریہ) {
            اشاریہ++;
            جاؤ again;
        }
    }

    /* check previous cycle */

    اگر (چکر->old_چکر && چکر->old_چکر->modules) {

        برائے (i = 0; چکر->old_چکر->modules[i]; i++) {
            module = چکر->old_چکر->modules[i];

            اگر (module->type != type) {
                جاری;
            }

            اگر (module->ctx_اشاریہ == اشاریہ) {
                اشاریہ++;
                جاؤ again;
            }
        }
    }

    واپس اشاریہ;
}
