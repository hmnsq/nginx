
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#تعریف_کن NGX_MAX_DYNAMIC_MODULES  128


ساكن ngx_uint_t ngx_module_شاخص(ngx_چرخه_t *چرخه);
ساكن ngx_uint_t ngx_module_ctx_شاخص(ngx_چرخه_t *چرخه, ngx_uint_t type,
    ngx_uint_t شاخص);


ngx_uint_t         ngx_max_module;
ساكن ngx_uint_t  ngx_modules_n;


ngx_int_t
ngx_preinit_modules(فراغ)
{
    ngx_uint_t  i;

    لكل (i = 0; ngx_modules[i]; i++) {
        ngx_modules[i]->شاخص = i;
        ngx_modules[i]->name = ngx_module_نامها[i];
    }

    ngx_modules_n = i;
    ngx_max_module = ngx_modules_n + NGX_MAX_DYNAMIC_MODULES;

    أرجع NGX_OK;
}


ngx_int_t
ngx_چرخه_modules(ngx_چرخه_t *چرخه)
{
    /*
     * create a list of modules to be used for this cycle,
     * copy static modules to it
     */

    چرخه->modules = ngx_pcalloc(چرخه->pool, (ngx_max_module + 1)
                                              * حجم(ngx_module_t *));
    إذا (چرخه->modules == عدم) {
        أرجع NGX_ERROR;
    }

    ngx_memcpy(چرخه->modules, ngx_modules,
               ngx_modules_n * حجم(ngx_module_t *));

    چرخه->modules_n = ngx_modules_n;

    أرجع NGX_OK;
}


ngx_int_t
ngx_init_modules(ngx_چرخه_t *چرخه)
{
    ngx_uint_t  i;

    لكل (i = 0; چرخه->modules[i]; i++) {
        إذا (چرخه->modules[i]->init_module) {
            إذا (چرخه->modules[i]->init_module(چرخه) != NGX_OK) {
                أرجع NGX_ERROR;
            }
        }
    }

    أرجع NGX_OK;
}


ngx_int_t
ngx_شمار_modules(ngx_چرخه_t *چرخه, ngx_uint_t type)
{
    ngx_uint_t     i, بعدی, max;
    ngx_module_t  *module;

    بعدی = 0;
    max = 0;

    /* count appropriate modules, set up their indices */

    لكل (i = 0; چرخه->modules[i]; i++) {
        module = چرخه->modules[i];

        إذا (module->type != type) {
            استمر;
        }

        إذا (module->ctx_شاخص != NGX_MODULE_UNSET_شاخص) {

            /* if ctx_index was assigned, preserve it */

            إذا (module->ctx_شاخص > max) {
                max = module->ctx_شاخص;
            }

            إذا (module->ctx_شاخص == بعدی) {
                بعدی++;
            }

            استمر;
        }

        /* search for some free index */

        module->ctx_شاخص = ngx_module_ctx_شاخص(چرخه, type, بعدی);

        إذا (module->ctx_شاخص > max) {
            max = module->ctx_شاخص;
        }

        بعدی = module->ctx_شاخص + 1;
    }

    /*
     * make sure the number returned is big enough for previous
     * cycle as well, else there will be problems if the number
     * will be stored in a global variable (as it's used to be)
     * and we'll have to roll back to the previous cycle
     */

    إذا (چرخه->old_چرخه && چرخه->old_چرخه->modules) {

        لكل (i = 0; چرخه->old_چرخه->modules[i]; i++) {
            module = چرخه->old_چرخه->modules[i];

            إذا (module->type != type) {
                استمر;
            }

            إذا (module->ctx_شاخص > max) {
                max = module->ctx_شاخص;
            }
        }
    }

    /* prevent loading of additional modules */

    چرخه->modules_استفاده_شده = 1;

    أرجع max + 1;
}


ngx_int_t
ngx_جمع_کن_module(ngx_conf_t *cf, ngx_str_t *file, ngx_module_t *module,
    حرف **ترتیب)
{
    فراغ               *rv;
    ngx_uint_t          i, m, before;
    ngx_core_module_t  *core_module;

    إذا (cf->چرخه->modules_n >= ngx_max_module) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "too many modules loaded");
        أرجع NGX_ERROR;
    }

    إذا (module->نسخه != nginx_نسخه) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "module \"%V\" version %ui instead of %ui",
                           file, module->نسخه, (ngx_uint_t) nginx_نسخه);
        أرجع NGX_ERROR;
    }

    إذا (ngx_strcmp(module->signature, NGX_MODULE_SIGNATURE) != 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "module \"%V\" is not binary compatible",
                           file);
        أرجع NGX_ERROR;
    }

    لكل (m = 0; cf->چرخه->modules[m]; m++) {
        إذا (ngx_strcmp(cf->چرخه->modules[m]->name, module->name) == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "module \"%s\" is already loaded",
                               module->name);
            أرجع NGX_ERROR;
        }
    }

    /*
     * if the module wasn't previously loaded, assign an index
     */

    إذا (module->شاخص == NGX_MODULE_UNSET_شاخص) {
        module->شاخص = ngx_module_شاخص(cf->چرخه);

        إذا (module->شاخص >= ngx_max_module) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "too many modules loaded");
            أرجع NGX_ERROR;
        }
    }

    /*
     * put the module into the cycle->modules array
     */

    before = cf->چرخه->modules_n;

    إذا (ترتیب) {
        لكل (i = 0; ترتیب[i]; i++) {
            إذا (ngx_strcmp(ترتیب[i], module->name) == 0) {
                i++;
                اكسر;
            }
        }

        لكل ( /* void */ ; ترتیب[i]; i++) {

#إذا 0
            ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->لگاریتم, 0,
                           "module: %s before %s",
                           module->name, ترتیب[i]);
#پایان_اگر

            لكل (m = 0; m < before; m++) {
                إذا (ngx_strcmp(cf->چرخه->modules[m]->name, ترتیب[i]) == 0) {

                    ngx_log_debug3(NGX_LOG_DEBUG_CORE, cf->لگاریتم, 0,
                                   "module: %s before %s:%i",
                                   module->name, ترتیب[i], m);

                    before = m;
                    اكسر;
                }
            }
        }
    }

    /* put the module before modules[before] */

    إذا (before != cf->چرخه->modules_n) {
        ngx_memmove(&cf->چرخه->modules[before + 1],
                    &cf->چرخه->modules[before],
                    (cf->چرخه->modules_n - before) * حجم(ngx_module_t *));
    }

    cf->چرخه->modules[before] = module;
    cf->چرخه->modules_n++;

    إذا (module->type == NGX_CORE_MODULE) {

        /*
         * we are smart enough to initialize core modules;
         * other modules are expected to be loaded before
         * initialization - e.g., http modules must be loaded
         * before http{} block
         */

        core_module = module->ctx;

        إذا (core_module->create_conf) {
            rv = core_module->create_conf(cf->چرخه);
            إذا (rv == عدم) {
                أرجع NGX_ERROR;
            }

            cf->چرخه->conf_ctx[module->شاخص] = rv;
        }
    }

    أرجع NGX_OK;
}


ساكن ngx_uint_t
ngx_module_شاخص(ngx_چرخه_t *چرخه)
{
    ngx_uint_t     i, شاخص;
    ngx_module_t  *module;

    شاخص = 0;

again:

    /* find an unused index */

    لكل (i = 0; چرخه->modules[i]; i++) {
        module = چرخه->modules[i];

        إذا (module->شاخص == شاخص) {
            شاخص++;
            اذهب again;
        }
    }

    /* check previous cycle */

    إذا (چرخه->old_چرخه && چرخه->old_چرخه->modules) {

        لكل (i = 0; چرخه->old_چرخه->modules[i]; i++) {
            module = چرخه->old_چرخه->modules[i];

            إذا (module->شاخص == شاخص) {
                شاخص++;
                اذهب again;
            }
        }
    }

    أرجع شاخص;
}


ساكن ngx_uint_t
ngx_module_ctx_شاخص(ngx_چرخه_t *چرخه, ngx_uint_t type, ngx_uint_t شاخص)
{
    ngx_uint_t     i;
    ngx_module_t  *module;

again:

    /* find an unused ctx_index */

    لكل (i = 0; چرخه->modules[i]; i++) {
        module = چرخه->modules[i];

        إذا (module->type != type) {
            استمر;
        }

        إذا (module->ctx_شاخص == شاخص) {
            شاخص++;
            اذهب again;
        }
    }

    /* check previous cycle */

    إذا (چرخه->old_چرخه && چرخه->old_چرخه->modules) {

        لكل (i = 0; چرخه->old_چرخه->modules[i]; i++) {
            module = چرخه->old_چرخه->modules[i];

            إذا (module->type != type) {
                استمر;
            }

            إذا (module->ctx_شاخص == شاخص) {
                شاخص++;
                اذهب again;
            }
        }
    }

    أرجع شاخص;
}
