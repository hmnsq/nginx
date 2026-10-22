
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


#define NGX_MAX_DYNAMIC_MODULES  128


স্থির ngx_uint_t ngx_module_সূচক(ngx_cycle_t *cycle);
স্থির ngx_uint_t ngx_module_ctx_সূচক(ngx_cycle_t *cycle, ngx_uint_t type,
    ngx_uint_t সূচক);


ngx_uint_t         ngx_max_module;
স্থির ngx_uint_t  ngx_modules_n;


ngx_int_t
ngx_preinit_modules(শূন্য)
{
    ngx_uint_t  i;

    জন্য (i = 0; ngx_modules[i]; i++) {
        ngx_modules[i]->সূচক = i;
        ngx_modules[i]->name = ngx_module_names[i];
    }

    ngx_modules_n = i;
    ngx_max_module = ngx_modules_n + NGX_MAX_DYNAMIC_MODULES;

    ফেরত NGX_OK;
}


ngx_int_t
ngx_cycle_modules(ngx_cycle_t *cycle)
{
    /*
     * create a list of modules to be used for this cycle,
     * copy static modules to it
     */

    cycle->modules = ngx_pcalloc(cycle->pool, (ngx_max_module + 1)
                                              * আকার(ngx_module_t *));
    যদি (cycle->modules == ফাঁকা) {
        ফেরত NGX_ERROR;
    }

    ngx_memcpy(cycle->modules, ngx_modules,
               ngx_modules_n * আকার(ngx_module_t *));

    cycle->modules_n = ngx_modules_n;

    ফেরত NGX_OK;
}


ngx_int_t
ngx_init_modules(ngx_cycle_t *cycle)
{
    ngx_uint_t  i;

    জন্য (i = 0; cycle->modules[i]; i++) {
        যদি (cycle->modules[i]->init_module) {
            যদি (cycle->modules[i]->init_module(cycle) != NGX_OK) {
                ফেরত NGX_ERROR;
            }
        }
    }

    ফেরত NGX_OK;
}


ngx_int_t
ngx_গুনতি_modules(ngx_cycle_t *cycle, ngx_uint_t type)
{
    ngx_uint_t     i, পরবর্তী, max;
    ngx_module_t  *module;

    পরবর্তী = 0;
    max = 0;

    /* count appropriate modules, set up their indices */

    জন্য (i = 0; cycle->modules[i]; i++) {
        module = cycle->modules[i];

        যদি (module->type != type) {
            চালু;
        }

        যদি (module->ctx_সূচক != NGX_MODULE_UNSET_সূচক) {

            /* if ctx_index was assigned, preserve it */

            যদি (module->ctx_সূচক > max) {
                max = module->ctx_সূচক;
            }

            যদি (module->ctx_সূচক == পরবর্তী) {
                পরবর্তী++;
            }

            চালু;
        }

        /* search for some free index */

        module->ctx_সূচক = ngx_module_ctx_সূচক(cycle, type, পরবর্তী);

        যদি (module->ctx_সূচক > max) {
            max = module->ctx_সূচক;
        }

        পরবর্তী = module->ctx_সূচক + 1;
    }

    /*
     * make sure the number returned is big enough for previous
     * cycle as well, else there will be problems if the number
     * will be stored in a global variable (as it's used to be)
     * and we'll have to roll back to the previous cycle
     */

    যদি (cycle->old_cycle && cycle->old_cycle->modules) {

        জন্য (i = 0; cycle->old_cycle->modules[i]; i++) {
            module = cycle->old_cycle->modules[i];

            যদি (module->type != type) {
                চালু;
            }

            যদি (module->ctx_সূচক > max) {
                max = module->ctx_সূচক;
            }
        }
    }

    /* prevent loading of additional modules */

    cycle->modules_used = 1;

    ফেরত max + 1;
}


ngx_int_t
ngx_add_module(ngx_conf_t *cf, ngx_str_t *file, ngx_module_t *module,
    অক্ষর **order)
{
    শূন্য               *rv;
    ngx_uint_t          i, m, before;
    ngx_core_module_t  *core_module;

    যদি (cf->cycle->modules_n >= ngx_max_module) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "too many modules loaded");
        ফেরত NGX_ERROR;
    }

    যদি (module->version != nginx_version) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "module \"%V\" version %ui instead of %ui",
                           file, module->version, (ngx_uint_t) nginx_version);
        ফেরত NGX_ERROR;
    }

    যদি (ngx_strcmp(module->signature, NGX_MODULE_SIGNATURE) != 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "module \"%V\" is not binary compatible",
                           file);
        ফেরত NGX_ERROR;
    }

    জন্য (m = 0; cf->cycle->modules[m]; m++) {
        যদি (ngx_strcmp(cf->cycle->modules[m]->name, module->name) == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "module \"%s\" is already loaded",
                               module->name);
            ফেরত NGX_ERROR;
        }
    }

    /*
     * if the module wasn't previously loaded, assign an index
     */

    যদি (module->সূচক == NGX_MODULE_UNSET_সূচক) {
        module->সূচক = ngx_module_সূচক(cf->cycle);

        যদি (module->সূচক >= ngx_max_module) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "too many modules loaded");
            ফেরত NGX_ERROR;
        }
    }

    /*
     * put the module into the cycle->modules array
     */

    before = cf->cycle->modules_n;

    যদি (order) {
        জন্য (i = 0; order[i]; i++) {
            যদি (ngx_strcmp(order[i], module->name) == 0) {
                i++;
                বিরতি;
            }
        }

        জন্য ( /* void */ ; order[i]; i++) {

#যদি 0
            ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->log, 0,
                           "module: %s before %s",
                           module->name, order[i]);
#endif

            জন্য (m = 0; m < before; m++) {
                যদি (ngx_strcmp(cf->cycle->modules[m]->name, order[i]) == 0) {

                    ngx_log_debug3(NGX_LOG_DEBUG_CORE, cf->log, 0,
                                   "module: %s before %s:%i",
                                   module->name, order[i], m);

                    before = m;
                    বিরতি;
                }
            }
        }
    }

    /* put the module before modules[before] */

    যদি (before != cf->cycle->modules_n) {
        ngx_memmove(&cf->cycle->modules[before + 1],
                    &cf->cycle->modules[before],
                    (cf->cycle->modules_n - before) * আকার(ngx_module_t *));
    }

    cf->cycle->modules[before] = module;
    cf->cycle->modules_n++;

    যদি (module->type == NGX_CORE_MODULE) {

        /*
         * we are smart enough to initialize core modules;
         * other modules are expected to be loaded before
         * initialization - e.g., http modules must be loaded
         * before http{} block
         */

        core_module = module->ctx;

        যদি (core_module->create_conf) {
            rv = core_module->create_conf(cf->cycle);
            যদি (rv == ফাঁকা) {
                ফেরত NGX_ERROR;
            }

            cf->cycle->conf_ctx[module->সূচক] = rv;
        }
    }

    ফেরত NGX_OK;
}


স্থির ngx_uint_t
ngx_module_সূচক(ngx_cycle_t *cycle)
{
    ngx_uint_t     i, সূচক;
    ngx_module_t  *module;

    সূচক = 0;

again:

    /* find an unused index */

    জন্য (i = 0; cycle->modules[i]; i++) {
        module = cycle->modules[i];

        যদি (module->সূচক == সূচক) {
            সূচক++;
            যাও again;
        }
    }

    /* check previous cycle */

    যদি (cycle->old_cycle && cycle->old_cycle->modules) {

        জন্য (i = 0; cycle->old_cycle->modules[i]; i++) {
            module = cycle->old_cycle->modules[i];

            যদি (module->সূচক == সূচক) {
                সূচক++;
                যাও again;
            }
        }
    }

    ফেরত সূচক;
}


স্থির ngx_uint_t
ngx_module_ctx_সূচক(ngx_cycle_t *cycle, ngx_uint_t type, ngx_uint_t সূচক)
{
    ngx_uint_t     i;
    ngx_module_t  *module;

again:

    /* find an unused ctx_index */

    জন্য (i = 0; cycle->modules[i]; i++) {
        module = cycle->modules[i];

        যদি (module->type != type) {
            চালু;
        }

        যদি (module->ctx_সূচক == সূচক) {
            সূচক++;
            যাও again;
        }
    }

    /* check previous cycle */

    যদি (cycle->old_cycle && cycle->old_cycle->modules) {

        জন্য (i = 0; cycle->old_cycle->modules[i]; i++) {
            module = cycle->old_cycle->modules[i];

            যদি (module->type != type) {
                চালু;
            }

            যদি (module->ctx_সূচক == সূচক) {
                সূচক++;
                যাও again;
            }
        }
    }

    ফেরত সূচক;
}
