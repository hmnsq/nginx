
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#تعریف_کرو NGX_MAX_DYNAMIC_MODULES  128


ساكن ngx_uint_t ngx_module_اشاریہ(ngx_چکر_t *چکر);
ساكن ngx_uint_t ngx_module_ctx_اشاریہ(ngx_چکر_t *چکر, ngx_uint_t type,
    ngx_uint_t اشاریہ);


ngx_uint_t         ngx_max_module;
ساكن ngx_uint_t  ngx_modules_n;


ngx_int_t
ngx_preinit_modules(فراغ)
{
    ngx_uint_t  i;

    لكل (i = 0; ngx_modules[i]; i++) {
        ngx_modules[i]->اشاریہ = i;
        ngx_modules[i]->name = ngx_module_ناں[i];
    }

    ngx_modules_n = i;
    ngx_max_module = ngx_modules_n + NGX_MAX_DYNAMIC_MODULES;

    أرجع NGX_OK;
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
    إذا (چکر->modules == عدم) {
        أرجع NGX_ERROR;
    }

    ngx_memcpy(چکر->modules, ngx_modules,
               ngx_modules_n * حجم(ngx_module_t *));

    چکر->modules_n = ngx_modules_n;

    أرجع NGX_OK;
}


ngx_int_t
ngx_init_modules(ngx_چکر_t *چکر)
{
    ngx_uint_t  i;

    لكل (i = 0; چکر->modules[i]; i++) {
        إذا (چکر->modules[i]->init_module) {
            إذا (چکر->modules[i]->init_module(چکر) != NGX_OK) {
                أرجع NGX_ERROR;
            }
        }
    }

    أرجع NGX_OK;
}


ngx_int_t
ngx_گنتی_modules(ngx_چکر_t *چکر, ngx_uint_t type)
{
    ngx_uint_t     i, اگلا, max;
    ngx_module_t  *module;

    اگلا = 0;
    max = 0;

    /* count appropriate modules, set up their indices */

    لكل (i = 0; چکر->modules[i]; i++) {
        module = چکر->modules[i];

        إذا (module->type != type) {
            استمر;
        }

        إذا (module->ctx_اشاریہ != NGX_MODULE_UNSET_اشاریہ) {

            /* if ctx_index was assigned, preserve it */

            إذا (module->ctx_اشاریہ > max) {
                max = module->ctx_اشاریہ;
            }

            إذا (module->ctx_اشاریہ == اگلا) {
                اگلا++;
            }

            استمر;
        }

        /* search for some free index */

        module->ctx_اشاریہ = ngx_module_ctx_اشاریہ(چکر, type, اگلا);

        إذا (module->ctx_اشاریہ > max) {
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

    إذا (چکر->old_چکر && چکر->old_چکر->modules) {

        لكل (i = 0; چکر->old_چکر->modules[i]; i++) {
            module = چکر->old_چکر->modules[i];

            إذا (module->type != type) {
                استمر;
            }

            إذا (module->ctx_اشاریہ > max) {
                max = module->ctx_اشاریہ;
            }
        }
    }

    /* prevent loading of additional modules */

    چکر->modules_ورتیا = 1;

    أرجع max + 1;
}


ngx_int_t
ngx_جوڑو_module(ngx_conf_t *cf, ngx_str_t *file, ngx_module_t *module,
    حرف **ترتیب)
{
    فراغ               *rv;
    ngx_uint_t          i, m, before;
    ngx_core_module_t  *core_module;

    إذا (cf->چکر->modules_n >= ngx_max_module) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "too many modules loaded");
        أرجع NGX_ERROR;
    }

    إذا (module->نسخہ != nginx_نسخہ) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "module \"%V\" version %ui instead of %ui",
                           file, module->نسخہ, (ngx_uint_t) nginx_نسخہ);
        أرجع NGX_ERROR;
    }

    إذا (ngx_strcmp(module->signature, NGX_MODULE_SIGNATURE) != 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "module \"%V\" is not binary compatible",
                           file);
        أرجع NGX_ERROR;
    }

    لكل (m = 0; cf->چکر->modules[m]; m++) {
        إذا (ngx_strcmp(cf->چکر->modules[m]->name, module->name) == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "module \"%s\" is already loaded",
                               module->name);
            أرجع NGX_ERROR;
        }
    }

    /*
     * if the module wasn't previously loaded, assign an index
     */

    إذا (module->اشاریہ == NGX_MODULE_UNSET_اشاریہ) {
        module->اشاریہ = ngx_module_اشاریہ(cf->چکر);

        إذا (module->اشاریہ >= ngx_max_module) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "too many modules loaded");
            أرجع NGX_ERROR;
        }
    }

    /*
     * put the module into the cycle->modules array
     */

    before = cf->چکر->modules_n;

    إذا (ترتیب) {
        لكل (i = 0; ترتیب[i]; i++) {
            إذا (ngx_strcmp(ترتیب[i], module->name) == 0) {
                i++;
                اكسر;
            }
        }

        لكل ( /* void */ ; ترتیب[i]; i++) {

#إذا 0
            ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->لوگارتھم, 0,
                           "module: %s before %s",
                           module->name, ترتیب[i]);
#جے_انت

            لكل (m = 0; m < before; m++) {
                إذا (ngx_strcmp(cf->چکر->modules[m]->name, ترتیب[i]) == 0) {

                    ngx_log_debug3(NGX_LOG_DEBUG_CORE, cf->لوگارتھم, 0,
                                   "module: %s before %s:%i",
                                   module->name, ترتیب[i], m);

                    before = m;
                    اكسر;
                }
            }
        }
    }

    /* put the module before modules[before] */

    إذا (before != cf->چکر->modules_n) {
        ngx_memmove(&cf->چکر->modules[before + 1],
                    &cf->چکر->modules[before],
                    (cf->چکر->modules_n - before) * حجم(ngx_module_t *));
    }

    cf->چکر->modules[before] = module;
    cf->چکر->modules_n++;

    إذا (module->type == NGX_CORE_MODULE) {

        /*
         * we are smart enough to initialize core modules;
         * other modules are expected to be loaded before
         * initialization - e.g., http modules must be loaded
         * before http{} block
         */

        core_module = module->ctx;

        إذا (core_module->create_conf) {
            rv = core_module->create_conf(cf->چکر);
            إذا (rv == عدم) {
                أرجع NGX_ERROR;
            }

            cf->چکر->conf_ctx[module->اشاریہ] = rv;
        }
    }

    أرجع NGX_OK;
}


ساكن ngx_uint_t
ngx_module_اشاریہ(ngx_چکر_t *چکر)
{
    ngx_uint_t     i, اشاریہ;
    ngx_module_t  *module;

    اشاریہ = 0;

again:

    /* find an unused index */

    لكل (i = 0; چکر->modules[i]; i++) {
        module = چکر->modules[i];

        إذا (module->اشاریہ == اشاریہ) {
            اشاریہ++;
            اذهب again;
        }
    }

    /* check previous cycle */

    إذا (چکر->old_چکر && چکر->old_چکر->modules) {

        لكل (i = 0; چکر->old_چکر->modules[i]; i++) {
            module = چکر->old_چکر->modules[i];

            إذا (module->اشاریہ == اشاریہ) {
                اشاریہ++;
                اذهب again;
            }
        }
    }

    أرجع اشاریہ;
}


ساكن ngx_uint_t
ngx_module_ctx_اشاریہ(ngx_چکر_t *چکر, ngx_uint_t type, ngx_uint_t اشاریہ)
{
    ngx_uint_t     i;
    ngx_module_t  *module;

again:

    /* find an unused ctx_index */

    لكل (i = 0; چکر->modules[i]; i++) {
        module = چکر->modules[i];

        إذا (module->type != type) {
            استمر;
        }

        إذا (module->ctx_اشاریہ == اشاریہ) {
            اشاریہ++;
            اذهب again;
        }
    }

    /* check previous cycle */

    إذا (چکر->old_چکر && چکر->old_چکر->modules) {

        لكل (i = 0; چکر->old_چکر->modules[i]; i++) {
            module = چکر->old_چکر->modules[i];

            إذا (module->type != type) {
                استمر;
            }

            إذا (module->ctx_اشاریہ == اشاریہ) {
                اشاریہ++;
                اذهب again;
            }
        }
    }

    أرجع اشاریہ;
}
