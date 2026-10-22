
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


#నిర్వచించు NGX_MAX_DYNAMIC_MODULES  128


స్థిర ngx_uint_t ngx_module_సూచీ(ngx_చక్రం_t *చక్రం);
స్థిర ngx_uint_t ngx_module_ctx_సూచీ(ngx_చక్రం_t *చక్రం, ngx_uint_t type,
    ngx_uint_t సూచీ);


ngx_uint_t         ngx_max_module;
స్థిర ngx_uint_t  ngx_modules_n;


ngx_int_t
ngx_preinit_modules(శూన్య)
{
    ngx_uint_t  i;

    కొరకు (i = 0; ngx_modules[i]; i++) {
        ngx_modules[i]->సూచీ = i;
        ngx_modules[i]->name = ngx_module_పేర్లు[i];
    }

    ngx_modules_n = i;
    ngx_max_module = ngx_modules_n + NGX_MAX_DYNAMIC_MODULES;

    తిరిగి NGX_OK;
}


ngx_int_t
ngx_చక్రం_modules(ngx_చక్రం_t *చక్రం)
{
    /*
     * create a list of modules to be used for this cycle,
     * copy static modules to it
     */

    చక్రం->modules = ngx_pcalloc(చక్రం->pool, (ngx_max_module + 1)
                                              * పరిమాణం(ngx_module_t *));
    ఒకవేళ (చక్రం->modules == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    ngx_memcpy(చక్రం->modules, ngx_modules,
               ngx_modules_n * పరిమాణం(ngx_module_t *));

    చక్రం->modules_n = ngx_modules_n;

    తిరిగి NGX_OK;
}


ngx_int_t
ngx_init_modules(ngx_చక్రం_t *చక్రం)
{
    ngx_uint_t  i;

    కొరకు (i = 0; చక్రం->modules[i]; i++) {
        ఒకవేళ (చక్రం->modules[i]->init_module) {
            ఒకవేళ (చక్రం->modules[i]->init_module(చక్రం) != NGX_OK) {
                తిరిగి NGX_ERROR;
            }
        }
    }

    తిరిగి NGX_OK;
}


ngx_int_t
ngx_లెక్క_modules(ngx_చక్రం_t *చక్రం, ngx_uint_t type)
{
    ngx_uint_t     i, తదుపరి, max;
    ngx_module_t  *module;

    తదుపరి = 0;
    max = 0;

    /* count appropriate modules, set up their indices */

    కొరకు (i = 0; చక్రం->modules[i]; i++) {
        module = చక్రం->modules[i];

        ఒకవేళ (module->type != type) {
            కొనసాగు;
        }

        ఒకవేళ (module->ctx_సూచీ != NGX_MODULE_UNSET_సూచీ) {

            /* if ctx_index was assigned, preserve it */

            ఒకవేళ (module->ctx_సూచీ > max) {
                max = module->ctx_సూచీ;
            }

            ఒకవేళ (module->ctx_సూచీ == తదుపరి) {
                తదుపరి++;
            }

            కొనసాగు;
        }

        /* search for some free index */

        module->ctx_సూచీ = ngx_module_ctx_సూచీ(చక్రం, type, తదుపరి);

        ఒకవేళ (module->ctx_సూచీ > max) {
            max = module->ctx_సూచీ;
        }

        తదుపరి = module->ctx_సూచీ + 1;
    }

    /*
     * make sure the number returned is big enough for previous
     * cycle as well, else there will be problems if the number
     * will be stored in a global variable (as it's used to be)
     * and we'll have to roll back to the previous cycle
     */

    ఒకవేళ (చక్రం->old_చక్రం && చక్రం->old_చక్రం->modules) {

        కొరకు (i = 0; చక్రం->old_చక్రం->modules[i]; i++) {
            module = చక్రం->old_చక్రం->modules[i];

            ఒకవేళ (module->type != type) {
                కొనసాగు;
            }

            ఒకవేళ (module->ctx_సూచీ > max) {
                max = module->ctx_సూచీ;
            }
        }
    }

    /* prevent loading of additional modules */

    చక్రం->modules_వాడిన = 1;

    తిరిగి max + 1;
}


ngx_int_t
ngx_కూడు_module(ngx_conf_t *cf, ngx_str_t *file, ngx_module_t *module,
    అక్షరం **క్రమం)
{
    శూన్య               *rv;
    ngx_uint_t          i, m, before;
    ngx_core_module_t  *core_module;

    ఒకవేళ (cf->చక్రం->modules_n >= ngx_max_module) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "too many modules loaded");
        తిరిగి NGX_ERROR;
    }

    ఒకవేళ (module->సంస్కరణ != nginx_సంస్కరణ) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "module \"%V\" version %ui instead of %ui",
                           file, module->సంస్కరణ, (ngx_uint_t) nginx_సంస్కరణ);
        తిరిగి NGX_ERROR;
    }

    ఒకవేళ (ngx_strcmp(module->signature, NGX_MODULE_SIGNATURE) != 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "module \"%V\" is not binary compatible",
                           file);
        తిరిగి NGX_ERROR;
    }

    కొరకు (m = 0; cf->చక్రం->modules[m]; m++) {
        ఒకవేళ (ngx_strcmp(cf->చక్రం->modules[m]->name, module->name) == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "module \"%s\" is already loaded",
                               module->name);
            తిరిగి NGX_ERROR;
        }
    }

    /*
     * if the module wasn't previously loaded, assign an index
     */

    ఒకవేళ (module->సూచీ == NGX_MODULE_UNSET_సూచీ) {
        module->సూచీ = ngx_module_సూచీ(cf->చక్రం);

        ఒకవేళ (module->సూచీ >= ngx_max_module) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "too many modules loaded");
            తిరిగి NGX_ERROR;
        }
    }

    /*
     * put the module into the cycle->modules array
     */

    before = cf->చక్రం->modules_n;

    ఒకవేళ (క్రమం) {
        కొరకు (i = 0; క్రమం[i]; i++) {
            ఒకవేళ (ngx_strcmp(క్రమం[i], module->name) == 0) {
                i++;
                ఆపు;
            }
        }

        కొరకు ( /* void */ ; క్రమం[i]; i++) {

#ఒకవేళ 0
            ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->సంవర్గమానం, 0,
                           "module: %s before %s",
                           module->name, క్రమం[i]);
#అయితే_ముగింపు

            కొరకు (m = 0; m < before; m++) {
                ఒకవేళ (ngx_strcmp(cf->చక్రం->modules[m]->name, క్రమం[i]) == 0) {

                    ngx_log_debug3(NGX_LOG_DEBUG_CORE, cf->సంవర్గమానం, 0,
                                   "module: %s before %s:%i",
                                   module->name, క్రమం[i], m);

                    before = m;
                    ఆపు;
                }
            }
        }
    }

    /* put the module before modules[before] */

    ఒకవేళ (before != cf->చక్రం->modules_n) {
        ngx_memmove(&cf->చక్రం->modules[before + 1],
                    &cf->చక్రం->modules[before],
                    (cf->చక్రం->modules_n - before) * పరిమాణం(ngx_module_t *));
    }

    cf->చక్రం->modules[before] = module;
    cf->చక్రం->modules_n++;

    ఒకవేళ (module->type == NGX_CORE_MODULE) {

        /*
         * we are smart enough to initialize core modules;
         * other modules are expected to be loaded before
         * initialization - e.g., http modules must be loaded
         * before http{} block
         */

        core_module = module->ctx;

        ఒకవేళ (core_module->create_conf) {
            rv = core_module->create_conf(cf->చక్రం);
            ఒకవేళ (rv == ఖాళీ) {
                తిరిగి NGX_ERROR;
            }

            cf->చక్రం->conf_ctx[module->సూచీ] = rv;
        }
    }

    తిరిగి NGX_OK;
}


స్థిర ngx_uint_t
ngx_module_సూచీ(ngx_చక్రం_t *చక్రం)
{
    ngx_uint_t     i, సూచీ;
    ngx_module_t  *module;

    సూచీ = 0;

again:

    /* find an unused index */

    కొరకు (i = 0; చక్రం->modules[i]; i++) {
        module = చక్రం->modules[i];

        ఒకవేళ (module->సూచీ == సూచీ) {
            సూచీ++;
            వెళ్ళు again;
        }
    }

    /* check previous cycle */

    ఒకవేళ (చక్రం->old_చక్రం && చక్రం->old_చక్రం->modules) {

        కొరకు (i = 0; చక్రం->old_చక్రం->modules[i]; i++) {
            module = చక్రం->old_చక్రం->modules[i];

            ఒకవేళ (module->సూచీ == సూచీ) {
                సూచీ++;
                వెళ్ళు again;
            }
        }
    }

    తిరిగి సూచీ;
}


స్థిర ngx_uint_t
ngx_module_ctx_సూచీ(ngx_చక్రం_t *చక్రం, ngx_uint_t type, ngx_uint_t సూచీ)
{
    ngx_uint_t     i;
    ngx_module_t  *module;

again:

    /* find an unused ctx_index */

    కొరకు (i = 0; చక్రం->modules[i]; i++) {
        module = చక్రం->modules[i];

        ఒకవేళ (module->type != type) {
            కొనసాగు;
        }

        ఒకవేళ (module->ctx_సూచీ == సూచీ) {
            సూచీ++;
            వెళ్ళు again;
        }
    }

    /* check previous cycle */

    ఒకవేళ (చక్రం->old_చక్రం && చక్రం->old_చక్రం->modules) {

        కొరకు (i = 0; చక్రం->old_చక్రం->modules[i]; i++) {
            module = చక్రం->old_చక్రం->modules[i];

            ఒకవేళ (module->type != type) {
                కొనసాగు;
            }

            ఒకవేళ (module->ctx_సూచీ == సూచీ) {
                సూచీ++;
                వెళ్ళు again;
            }
        }
    }

    తిరిగి సూచీ;
}
