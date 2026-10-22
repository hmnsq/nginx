
/*
 * Copyright (C) Nginx, Inc.
 * Copyright (C) Valentin V. Bartenev
 * Copyright (C) Ruslan Ermilov
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>
#अन्तर्भाव <ngx_thread_pool.h>


प्रकारनिर्देश संरचना {
    ngx_array_t               pools;
} ngx_thread_pool_conf_t;


प्रकारनिर्देश संरचना {
    ngx_thread_task_t        *first;
    ngx_thread_task_t       **चरमः;
} ngx_thread_pool_queue_t;

#परिभाषय ngx_thread_pool_queue_init(q)                                         \
    (q)->first = रिक्त;                                                        \
    (q)->चरमः = &(q)->first


संरचना ngx_thread_pool_s {
    ngx_thread_mutex_t        mtx;
    ngx_thread_pool_queue_t   queue;
    ngx_int_t                 प्रतीक्षा;
    ngx_thread_cond_t         cond;

    ngx_log_t                *लघुगणकः;

    ngx_str_t                 name;
    ngx_uint_t                तन्तवः;
    ngx_int_t                 max_queue;

    u_char                   *file;
    ngx_uint_t                पङ्क्तिः;
};


स्थिर ngx_int_t ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *लघुगणकः,
    ngx_pool_t *pool);
स्थिर शून्य ngx_thread_pool_destroy(ngx_thread_pool_t *tp);
स्थिर शून्य ngx_thread_pool_exit_handler(शून्य *दत्त, ngx_log_t *लघुगणकः);

स्थिर शून्य *ngx_thread_pool_आवृत्तिः(शून्य *दत्त);
स्थिर शून्य ngx_thread_pool_handler(ngx_event_t *ev);

स्थिर वर्ण *ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);

स्थिर शून्य *ngx_thread_pool_create_conf(ngx_आवृत्तिः_t *आवृत्तिः);
स्थिर वर्ण *ngx_thread_pool_init_conf(ngx_आवृत्तिः_t *आवृत्तिः, शून्य *conf);

स्थिर ngx_int_t ngx_thread_pool_init_कार्यकर्ता(ngx_आवृत्तिः_t *आवृत्तिः);
स्थिर शून्य ngx_thread_pool_exit_कार्यकर्ता(ngx_आवृत्तिः_t *आवृत्तिः);


स्थिर ngx_command_t  ngx_thread_pool_commands[] = {

    { ngx_string("thread_pool"),
      NGX_MAIN_CONF|NGX_प्रत्यक्षम्_CONF|NGX_CONF_TAKE23,
      ngx_thread_pool,
      0,
      0,
      रिक्त },

      ngx_null_command
};


स्थिर ngx_core_module_t  ngx_thread_pool_module_ctx = {
    ngx_string("thread_pool"),
    ngx_thread_pool_create_conf,
    ngx_thread_pool_init_conf
};


ngx_module_t  ngx_thread_pool_module = {
    NGX_MODULE_V1,
    &ngx_thread_pool_module_ctx,           /* module context */
    ngx_thread_pool_commands,              /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    रिक्त,                                  /* init master */
    रिक्त,                                  /* init module */
    ngx_thread_pool_init_कार्यकर्ता,           /* init process */
    रिक्त,                                  /* init thread */
    रिक्त,                                  /* exit thread */
    ngx_thread_pool_exit_कार्यकर्ता,           /* exit process */
    रिक्त,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


स्थिर ngx_str_t  ngx_thread_pool_default = ngx_string("default");

स्थिर ngx_uint_t               ngx_thread_pool_task_id;
स्थिर ngx_atomic_t             ngx_thread_pool_समाप्तम्_तालकम्;
स्थिर ngx_thread_pool_queue_t  ngx_thread_pool_समाप्तम्;


स्थिर ngx_int_t
ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *लघुगणकः, ngx_pool_t *pool)
{
    पूर्णाङ्क             err;
    pthread_t       tid;
    ngx_uint_t      n;
    pthread_attr_t  attr;

    यदि (ngx_notify == रिक्त) {
        ngx_log_error(NGX_LOG_ALERT, लघुगणकः, 0,
               "the configured event method cannot be used with thread pools");
        प्रत्यागम NGX_ERROR;
    }

    ngx_thread_pool_queue_init(&tp->queue);

    यदि (ngx_thread_mutex_create(&tp->mtx, लघुगणकः) != NGX_OK) {
        प्रत्यागम NGX_ERROR;
    }

    यदि (ngx_thread_cond_create(&tp->cond, लघुगणकः) != NGX_OK) {
        (शून्य) ngx_thread_mutex_destroy(&tp->mtx, लघुगणकः);
        प्रत्यागम NGX_ERROR;
    }

    tp->लघुगणकः = लघुगणकः;

    err = pthread_attr_init(&attr);
    यदि (err) {
        ngx_log_error(NGX_LOG_ALERT, लघुगणकः, err,
                      "pthread_attr_init() failed");
        प्रत्यागम NGX_ERROR;
    }

    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    यदि (err) {
        ngx_log_error(NGX_LOG_ALERT, लघुगणकः, err,
                      "pthread_attr_setdetachstate() failed");
        प्रत्यागम NGX_ERROR;
    }

#यदि 0
    err = pthread_attr_setstacksize(&attr, PTHREAD_राशिः_MIN);
    यदि (err) {
        ngx_log_error(NGX_LOG_ALERT, लघुगणकः, err,
                      "pthread_attr_setstacksize() failed");
        प्रत्यागम NGX_ERROR;
    }
#यदि_अन्तः

    प्रति (n = 0; n < tp->तन्तवः; n++) {
        err = pthread_create(&tid, &attr, ngx_thread_pool_आवृत्तिः, tp);
        यदि (err) {
            ngx_log_error(NGX_LOG_ALERT, लघुगणकः, err,
                          "pthread_create() failed");
            प्रत्यागम NGX_ERROR;
        }
    }

    (शून्य) pthread_attr_destroy(&attr);

    प्रत्यागम NGX_OK;
}


स्थिर शून्य
ngx_thread_pool_destroy(ngx_thread_pool_t *tp)
{
    ngx_uint_t           n;
    ngx_thread_task_t    task;
    चञ्चल ngx_uint_t  तालकम्;

    ngx_memzero(&task, परिमाण(ngx_thread_task_t));

    task.handler = ngx_thread_pool_exit_handler;
    task.ctx = (शून्य *) &तालकम्;

    प्रति (n = 0; n < tp->तन्तवः; n++) {
        तालकम् = 1;

        यदि (ngx_thread_task_post(tp, &task) != NGX_OK) {
            प्रत्यागम;
        }

        यावत् (तालकम्) {
            ngx_sched_yield();
        }

        task.event.सक्रिय = 0;
    }

    (शून्य) ngx_thread_cond_destroy(&tp->cond, tp->लघुगणकः);

    (शून्य) ngx_thread_mutex_destroy(&tp->mtx, tp->लघुगणकः);
}


स्थिर शून्य
ngx_thread_pool_exit_handler(शून्य *दत्त, ngx_log_t *लघुगणकः)
{
    ngx_uint_t *तालकम् = दत्त;

    *तालकम् = 0;

    pthread_exit(रिक्त);
}


ngx_thread_task_t *
ngx_thread_task_alloc(ngx_pool_t *pool, आकार_t आकार)
{
    ngx_thread_task_t  *task;

    task = ngx_pcalloc(pool, परिमाण(ngx_thread_task_t) + आकार);
    यदि (task == रिक्त) {
        प्रत्यागम रिक्त;
    }

    task->ctx = task + 1;

    प्रत्यागम task;
}


ngx_int_t
ngx_thread_task_post(ngx_thread_pool_t *tp, ngx_thread_task_t *task)
{
    यदि (task->event.सक्रिय) {
        ngx_log_error(NGX_LOG_ALERT, tp->लघुगणकः, 0,
                      "task #%ui already active", task->id);
        प्रत्यागम NGX_ERROR;
    }

    यदि (ngx_thread_mutex_तालकम्(&tp->mtx, tp->लघुगणकः) != NGX_OK) {
        प्रत्यागम NGX_ERROR;
    }

    यदि (tp->प्रतीक्षा >= tp->max_queue) {
        (शून्य) ngx_thread_mutex_unlock(&tp->mtx, tp->लघुगणकः);

        ngx_log_error(NGX_LOG_ERR, tp->लघुगणकः, 0,
                      "thread pool \"%V\" queue overflow: %i tasks waiting",
                      &tp->name, tp->प्रतीक्षा);
        प्रत्यागम NGX_ERROR;
    }

    task->event.सक्रिय = 1;

    task->id = ngx_thread_pool_task_id++;
    task->अग्र = रिक्त;

    यदि (ngx_thread_cond_signal(&tp->cond, tp->लघुगणकः) != NGX_OK) {
        (शून्य) ngx_thread_mutex_unlock(&tp->mtx, tp->लघुगणकः);
        प्रत्यागम NGX_ERROR;
    }

    *tp->queue.चरमः = task;
    tp->queue.चरमः = &task->अग्र;

    tp->प्रतीक्षा++;

    (शून्य) ngx_thread_mutex_unlock(&tp->mtx, tp->लघुगणकः);

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->लघुगणकः, 0,
                   "task #%ui added to thread pool \"%V\"",
                   task->id, &tp->name);

    प्रत्यागम NGX_OK;
}


स्थिर शून्य *
ngx_thread_pool_आवृत्तिः(शून्य *दत्त)
{
    ngx_thread_pool_t *tp = दत्त;

    पूर्णाङ्क                 err;
    sigset_t            समुच्चयः;
    ngx_thread_task_t  *task;

#यदि 0
    ngx_time_update();
#यदि_अन्तः

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, tp->लघुगणकः, 0,
                   "thread in pool \"%V\" started", &tp->name);

    sigfillset(&समुच्चयः);

    sigdelset(&समुच्चयः, SIGILL);
    sigdelset(&समुच्चयः, SIGFPE);
    sigdelset(&समुच्चयः, SIGSEGV);
    sigdelset(&समुच्चयः, SIGBUS);

    err = pthread_sigmask(SIG_खण्डम्, &समुच्चयः, रिक्त);
    यदि (err) {
        ngx_log_error(NGX_LOG_ALERT, tp->लघुगणकः, err, "pthread_sigmask() failed");
        प्रत्यागम रिक्त;
    }

    प्रति ( ;; ) {
        यदि (ngx_thread_mutex_तालकम्(&tp->mtx, tp->लघुगणकः) != NGX_OK) {
            प्रत्यागम रिक्त;
        }

        /* the number may become negative */
        tp->प्रतीक्षा--;

        यावत् (tp->queue.first == रिक्त) {
            यदि (ngx_thread_cond_wait(&tp->cond, &tp->mtx, tp->लघुगणकः)
                != NGX_OK)
            {
                (शून्य) ngx_thread_mutex_unlock(&tp->mtx, tp->लघुगणकः);
                प्रत्यागम रिक्त;
            }
        }

        task = tp->queue.first;
        tp->queue.first = task->अग्र;

        यदि (tp->queue.first == रिक्त) {
            tp->queue.चरमः = &tp->queue.first;
        }

        यदि (ngx_thread_mutex_unlock(&tp->mtx, tp->लघुगणकः) != NGX_OK) {
            प्रत्यागम रिक्त;
        }

#यदि 0
        ngx_time_update();
#यदि_अन्तः

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->लघुगणकः, 0,
                       "run task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->handler(task->ctx, tp->लघुगणकः);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->लघुगणकः, 0,
                       "complete task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->अग्र = रिक्त;

        ngx_spinlock(&ngx_thread_pool_समाप्तम्_तालकम्, 1, 2048);

        *ngx_thread_pool_समाप्तम्.चरमः = task;
        ngx_thread_pool_समाप्तम्.चरमः = &task->अग्र;

        ngx_स्मृतिः_barrier();

        ngx_unlock(&ngx_thread_pool_समाप्तम्_तालकम्);

        (शून्य) ngx_notify(ngx_thread_pool_handler);
    }
}


स्थिर शून्य
ngx_thread_pool_handler(ngx_event_t *ev)
{
    ngx_event_t        *event;
    ngx_thread_task_t  *task;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ev->लघुगणकः, 0, "thread pool handler");

    ngx_spinlock(&ngx_thread_pool_समाप्तम्_तालकम्, 1, 2048);

    task = ngx_thread_pool_समाप्तम्.first;
    ngx_thread_pool_समाप्तम्.first = रिक्त;
    ngx_thread_pool_समाप्तम्.चरमः = &ngx_thread_pool_समाप्तम्.first;

    ngx_स्मृतिः_barrier();

    ngx_unlock(&ngx_thread_pool_समाप्तम्_तालकम्);

    यावत् (task) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->लघुगणकः, 0,
                       "run completion handler for task #%ui", task->id);

        event = &task->event;
        task = task->अग्र;

        event->complete = 1;
        event->सक्रिय = 0;

        event->handler(event);
    }
}


स्थिर शून्य *
ngx_thread_pool_create_conf(ngx_आवृत्तिः_t *आवृत्तिः)
{
    ngx_thread_pool_conf_t  *tcf;

    tcf = ngx_pcalloc(आवृत्तिः->pool, परिमाण(ngx_thread_pool_conf_t));
    यदि (tcf == रिक्त) {
        प्रत्यागम रिक्त;
    }

    यदि (ngx_array_init(&tcf->pools, आवृत्तिः->pool, 4,
                       परिमाण(ngx_thread_pool_t *))
        != NGX_OK)
    {
        प्रत्यागम रिक्त;
    }

    प्रत्यागम tcf;
}


स्थिर वर्ण *
ngx_thread_pool_init_conf(ngx_आवृत्तिः_t *आवृत्तिः, शून्य *conf)
{
    ngx_thread_pool_conf_t *tcf = conf;

    ngx_uint_t           i;
    ngx_thread_pool_t  **tpp;

    tpp = tcf->pools.elts;

    प्रति (i = 0; i < tcf->pools.nelts; i++) {

        यदि (tpp[i]->तन्तवः) {
            अनुवर्त;
        }

        यदि (tpp[i]->name.len == ngx_thread_pool_default.len
            && ngx_strncmp(tpp[i]->name.दत्त, ngx_thread_pool_default.दत्त,
                           ngx_thread_pool_default.len)
               == 0)
        {
            tpp[i]->तन्तवः = 32;
            tpp[i]->max_queue = 65536;
            अनुवर्त;
        }

        ngx_log_error(NGX_LOG_EMERG, आवृत्तिः->लघुगणकः, 0,
                      "unknown thread pool \"%V\" in %s:%ui",
                      &tpp[i]->name, tpp[i]->file, tpp[i]->पङ्क्तिः);

        प्रत्यागम NGX_CONF_ERROR;
    }

    प्रत्यागम NGX_CONF_OK;
}


स्थिर वर्ण *
ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    ngx_str_t          *मानम्;
    ngx_uint_t          i;
    ngx_thread_pool_t  *tp;

    मानम् = cf->तर्काः->elts;

    tp = ngx_thread_pool_सङ्कलय(cf, &मानम्[1]);

    यदि (tp == रिक्त) {
        प्रत्यागम NGX_CONF_ERROR;
    }

    यदि (tp->तन्तवः) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "duplicate thread pool \"%V\"", &tp->name);
        प्रत्यागम NGX_CONF_ERROR;
    }

    tp->max_queue = 65536;

    प्रति (i = 2; i < cf->तर्काः->nelts; i++) {

        यदि (ngx_strncmp(मानम्[i].दत्त, "threads=", 8) == 0) {

            tp->तन्तवः = ngx_atoi(मानम्[i].दत्त + 8, मानम्[i].len - 8);

            यदि (tp->तन्तवः == (ngx_uint_t) NGX_ERROR || tp->तन्तवः == 0) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid threads value \"%V\"", &मानम्[i]);
                प्रत्यागम NGX_CONF_ERROR;
            }

            अनुवर्त;
        }

        यदि (ngx_strncmp(मानम्[i].दत्त, "max_queue=", 10) == 0) {

            tp->max_queue = ngx_atoi(मानम्[i].दत्त + 10, मानम्[i].len - 10);

            यदि (tp->max_queue == NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid max_queue value \"%V\"", &मानम्[i]);
                प्रत्यागम NGX_CONF_ERROR;
            }

            अनुवर्त;
        }
    }

    यदि (tp->तन्तवः == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%V\" must have \"threads\" parameter",
                           &cmd->name);
        प्रत्यागम NGX_CONF_ERROR;
    }

    प्रत्यागम NGX_CONF_OK;
}


ngx_thread_pool_t *
ngx_thread_pool_सङ्कलय(ngx_conf_t *cf, ngx_str_t *name)
{
    ngx_thread_pool_t       *tp, **tpp;
    ngx_thread_pool_conf_t  *tcf;

    यदि (name == रिक्त) {
        name = &ngx_thread_pool_default;
    }

    tp = ngx_thread_pool_get(cf->आवृत्तिः, name);

    यदि (tp) {
        प्रत्यागम tp;
    }

    tp = ngx_pcalloc(cf->pool, परिमाण(ngx_thread_pool_t));
    यदि (tp == रिक्त) {
        प्रत्यागम रिक्त;
    }

    tp->name = *name;
    tp->file = cf->conf_file->file.name.दत्त;
    tp->पङ्क्तिः = cf->conf_file->पङ्क्तिः;

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cf->आवृत्तिः->conf_ctx,
                                                  ngx_thread_pool_module);

    tpp = ngx_array_प्रक्षिप(&tcf->pools);
    यदि (tpp == रिक्त) {
        प्रत्यागम रिक्त;
    }

    *tpp = tp;

    प्रत्यागम tp;
}


ngx_thread_pool_t *
ngx_thread_pool_get(ngx_आवृत्तिः_t *आवृत्तिः, ngx_str_t *name)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(आवृत्तिः->conf_ctx,
                                                  ngx_thread_pool_module);

    tpp = tcf->pools.elts;

    प्रति (i = 0; i < tcf->pools.nelts; i++) {

        यदि (tpp[i]->name.len == name->len
            && ngx_strncmp(tpp[i]->name.दत्त, name->दत्त, name->len) == 0)
        {
            प्रत्यागम tpp[i];
        }
    }

    प्रत्यागम रिक्त;
}


स्थिर ngx_int_t
ngx_thread_pool_init_कार्यकर्ता(ngx_आवृत्तिः_t *आवृत्तिः)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    यदि (ngx_process != NGX_PROCESS_कार्यकर्ता
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        प्रत्यागम NGX_OK;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(आवृत्तिः->conf_ctx,
                                                  ngx_thread_pool_module);

    यदि (tcf == रिक्त) {
        प्रत्यागम NGX_OK;
    }

    ngx_thread_pool_queue_init(&ngx_thread_pool_समाप्तम्);

    tpp = tcf->pools.elts;

    प्रति (i = 0; i < tcf->pools.nelts; i++) {
        यदि (ngx_thread_pool_init(tpp[i], आवृत्तिः->लघुगणकः, आवृत्तिः->pool) != NGX_OK) {
            प्रत्यागम NGX_ERROR;
        }
    }

    प्रत्यागम NGX_OK;
}


स्थिर शून्य
ngx_thread_pool_exit_कार्यकर्ता(ngx_आवृत्तिः_t *आवृत्तिः)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    यदि (ngx_process != NGX_PROCESS_कार्यकर्ता
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        प्रत्यागम;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(आवृत्तिः->conf_ctx,
                                                  ngx_thread_pool_module);

    यदि (tcf == रिक्त) {
        प्रत्यागम;
    }

    tpp = tcf->pools.elts;

    प्रति (i = 0; i < tcf->pools.nelts; i++) {
        ngx_thread_pool_destroy(tpp[i]);
    }
}
