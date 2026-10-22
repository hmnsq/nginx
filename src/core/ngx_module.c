
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


#परिभाषय NGX_MAX_DYNAMIC_MODULES  128


स्थिर ngx_uint_t ngx_module_सूचकांक(ngx_आवृत्तिः_t *आवृत्तिः);
स्थिर ngx_uint_t ngx_module_ctx_सूचकांक(ngx_आवृत्तिः_t *आवृत्तिः, ngx_uint_t type,
    ngx_uint_t सूचकांक);


ngx_uint_t         ngx_max_module;
स्थिर ngx_uint_t  ngx_modules_n;


ngx_int_t
ngx_preinit_modules(शून्य)
{
    ngx_uint_t  i;

    प्रति (i = 0; ngx_modules[i]; i++) {
        ngx_modules[i]->सूचकांक = i;
        ngx_modules[i]->name = ngx_module_नामानि[i];
    }

    ngx_modules_n = i;
    ngx_max_module = ngx_modules_n + NGX_MAX_DYNAMIC_MODULES;

    प्रत्यागम NGX_OK;
}


ngx_int_t
ngx_आवृत्तिः_modules(ngx_आवृत्तिः_t *आवृत्तिः)
{
    /*
     * create a list of modules to be used for this cycle,
     * copy static modules to it
     */

    आवृत्तिः->modules = ngx_pcalloc(आवृत्तिः->pool, (ngx_max_module + 1)
                                              * परिमाण(ngx_module_t *));
    यदि (आवृत्तिः->modules == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    ngx_memcpy(आवृत्तिः->modules, ngx_modules,
               ngx_modules_n * परिमाण(ngx_module_t *));

    आवृत्तिः->modules_n = ngx_modules_n;

    प्रत्यागम NGX_OK;
}


ngx_int_t
ngx_init_modules(ngx_आवृत्तिः_t *आवृत्तिः)
{
    ngx_uint_t  i;

    प्रति (i = 0; आवृत्तिः->modules[i]; i++) {
        यदि (आवृत्तिः->modules[i]->init_module) {
            यदि (आवृत्तिः->modules[i]->init_module(आवृत्तिः) != NGX_OK) {
                प्रत्यागम NGX_ERROR;
            }
        }
    }

    प्रत्यागम NGX_OK;
}


ngx_int_t
ngx_गणनम्_modules(ngx_आवृत्तिः_t *आवृत्तिः, ngx_uint_t type)
{
    ngx_uint_t     i, अग्र, max;
    ngx_module_t  *module;

    अग्र = 0;
    max = 0;

    /* count appropriate modules, set up their indices */

    प्रति (i = 0; आवृत्तिः->modules[i]; i++) {
        module = आवृत्तिः->modules[i];

        यदि (module->type != type) {
            अनुवर्त;
        }

        यदि (module->ctx_सूचकांक != NGX_MODULE_UNSET_सूचकांक) {

            /* if ctx_index was assigned, preserve it */

            यदि (module->ctx_सूचकांक > max) {
                max = module->ctx_सूचकांक;
            }

            यदि (module->ctx_सूचकांक == अग्र) {
                अग्र++;
            }

            अनुवर्त;
        }

        /* search for some free index */

        module->ctx_सूचकांक = ngx_module_ctx_सूचकांक(आवृत्तिः, type, अग्र);

        यदि (module->ctx_सूचकांक > max) {
            max = module->ctx_सूचकांक;
        }

        अग्र = module->ctx_सूचकांक + 1;
    }

    /*
     * make sure the number returned is big enough for previous
     * cycle as well, else there will be problems if the number
     * will be stored in a global variable (as it's used to be)
     * and we'll have to roll back to the previous cycle
     */

    यदि (आवृत्तिः->old_आवृत्तिः && आवृत्तिः->old_आवृत्तिः->modules) {

        प्रति (i = 0; आवृत्तिः->old_आवृत्तिः->modules[i]; i++) {
            module = आवृत्तिः->old_आवृत्तिः->modules[i];

            यदि (module->type != type) {
                अनुवर्त;
            }

            यदि (module->ctx_सूचकांक > max) {
                max = module->ctx_सूचकांक;
            }
        }
    }

    /* prevent loading of additional modules */

    आवृत्तिः->modules_प्रयुक्तम् = 1;

    प्रत्यागम max + 1;
}


ngx_int_t
ngx_सङ्कलय_module(ngx_conf_t *cf, ngx_str_t *file, ngx_module_t *module,
    वर्ण **क्रमः)
{
    शून्य               *rv;
    ngx_uint_t          i, m, before;
    ngx_core_module_t  *core_module;

    यदि (cf->आवृत्तिः->modules_n >= ngx_max_module) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "too many modules loaded");
        प्रत्यागम NGX_ERROR;
    }

    यदि (module->संस्करणम् != nginx_संस्करणम्) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "module \"%V\" version %ui instead of %ui",
                           file, module->संस्करणम्, (ngx_uint_t) nginx_संस्करणम्);
        प्रत्यागम NGX_ERROR;
    }

    यदि (ngx_strcmp(module->signature, NGX_MODULE_SIGNATURE) != 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "module \"%V\" is not binary compatible",
                           file);
        प्रत्यागम NGX_ERROR;
    }

    प्रति (m = 0; cf->आवृत्तिः->modules[m]; m++) {
        यदि (ngx_strcmp(cf->आवृत्तिः->modules[m]->name, module->name) == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "module \"%s\" is already loaded",
                               module->name);
            प्रत्यागम NGX_ERROR;
        }
    }

    /*
     * if the module wasn't previously loaded, assign an index
     */

    यदि (module->सूचकांक == NGX_MODULE_UNSET_सूचकांक) {
        module->सूचकांक = ngx_module_सूचकांक(cf->आवृत्तिः);

        यदि (module->सूचकांक >= ngx_max_module) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "too many modules loaded");
            प्रत्यागम NGX_ERROR;
        }
    }

    /*
     * put the module into the cycle->modules array
     */

    before = cf->आवृत्तिः->modules_n;

    यदि (क्रमः) {
        प्रति (i = 0; क्रमः[i]; i++) {
            यदि (ngx_strcmp(क्रमः[i], module->name) == 0) {
                i++;
                भङ्ग;
            }
        }

        प्रति ( /* void */ ; क्रमः[i]; i++) {

#यदि 0
            ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->लघुगणकः, 0,
                           "module: %s before %s",
                           module->name, क्रमः[i]);
#यदि_अन्तः

            प्रति (m = 0; m < before; m++) {
                यदि (ngx_strcmp(cf->आवृत्तिः->modules[m]->name, क्रमः[i]) == 0) {

                    ngx_log_debug3(NGX_LOG_DEBUG_CORE, cf->लघुगणकः, 0,
                                   "module: %s before %s:%i",
                                   module->name, क्रमः[i], m);

                    before = m;
                    भङ्ग;
                }
            }
        }
    }

    /* put the module before modules[before] */

    यदि (before != cf->आवृत्तिः->modules_n) {
        ngx_memmove(&cf->आवृत्तिः->modules[before + 1],
                    &cf->आवृत्तिः->modules[before],
                    (cf->आवृत्तिः->modules_n - before) * परिमाण(ngx_module_t *));
    }

    cf->आवृत्तिः->modules[before] = module;
    cf->आवृत्तिः->modules_n++;

    यदि (module->type == NGX_CORE_MODULE) {

        /*
         * we are smart enough to initialize core modules;
         * other modules are expected to be loaded before
         * initialization - e.g., http modules must be loaded
         * before http{} block
         */

        core_module = module->ctx;

        यदि (core_module->create_conf) {
            rv = core_module->create_conf(cf->आवृत्तिः);
            यदि (rv == रिक्त) {
                प्रत्यागम NGX_ERROR;
            }

            cf->आवृत्तिः->conf_ctx[module->सूचकांक] = rv;
        }
    }

    प्रत्यागम NGX_OK;
}


स्थिर ngx_uint_t
ngx_module_सूचकांक(ngx_आवृत्तिः_t *आवृत्तिः)
{
    ngx_uint_t     i, सूचकांक;
    ngx_module_t  *module;

    सूचकांक = 0;

again:

    /* find an unused index */

    प्रति (i = 0; आवृत्तिः->modules[i]; i++) {
        module = आवृत्तिः->modules[i];

        यदि (module->सूचकांक == सूचकांक) {
            सूचकांक++;
            गच्छ again;
        }
    }

    /* check previous cycle */

    यदि (आवृत्तिः->old_आवृत्तिः && आवृत्तिः->old_आवृत्तिः->modules) {

        प्रति (i = 0; आवृत्तिः->old_आवृत्तिः->modules[i]; i++) {
            module = आवृत्तिः->old_आवृत्तिः->modules[i];

            यदि (module->सूचकांक == सूचकांक) {
                सूचकांक++;
                गच्छ again;
            }
        }
    }

    प्रत्यागम सूचकांक;
}


स्थिर ngx_uint_t
ngx_module_ctx_सूचकांक(ngx_आवृत्तिः_t *आवृत्तिः, ngx_uint_t type, ngx_uint_t सूचकांक)
{
    ngx_uint_t     i;
    ngx_module_t  *module;

again:

    /* find an unused ctx_index */

    प्रति (i = 0; आवृत्तिः->modules[i]; i++) {
        module = आवृत्तिः->modules[i];

        यदि (module->type != type) {
            अनुवर्त;
        }

        यदि (module->ctx_सूचकांक == सूचकांक) {
            सूचकांक++;
            गच्छ again;
        }
    }

    /* check previous cycle */

    यदि (आवृत्तिः->old_आवृत्तिः && आवृत्तिः->old_आवृत्तिः->modules) {

        प्रति (i = 0; आवृत्तिः->old_आवृत्तिः->modules[i]; i++) {
            module = आवृत्तिः->old_आवृत्तिः->modules[i];

            यदि (module->type != type) {
                अनुवर्त;
            }

            यदि (module->ctx_सूचकांक == सूचकांक) {
                सूचकांक++;
                गच्छ again;
            }
        }
    }

    प्रत्यागम सूचकांक;
}
