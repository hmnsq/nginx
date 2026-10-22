
/*
 * Copyright (C) Nginx, Inc.
 * Copyright (C) Valentin V. Bartenev
 * Copyright (C) Ruslan Ermilov
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>
#అన్తర్భావ <ngx_thread_pool.h>


రకనిర్వచనం నిర్మాణం {
    ngx_array_t               pools;
} ngx_thread_pool_conf_t;


రకనిర్వచనం నిర్మాణం {
    ngx_thread_task_t        *మొదటి;
    ngx_thread_task_t       **చివరి;
} ngx_thread_pool_వరుస_t;

#నిర్వచించు ngx_thread_pool_వరుస_init(q)                                         \
    (q)->మొదటి = ఖాళీ;                                                        \
    (q)->చివరి = &(q)->మొదటి


నిర్మాణం ngx_thread_pool_s {
    ngx_thread_mutex_t        mtx;
    ngx_thread_pool_వరుస_t   వరుస;
    ngx_int_t                 నిరీక్షణ;
    ngx_thread_cond_t         cond;

    ngx_log_t                *సంవర్గమానం;

    ngx_str_t                 name;
    ngx_uint_t                దారాలు;
    ngx_int_t                 max_వరుస;

    u_char                   *file;
    ngx_uint_t                పంక్తి;
};


స్థిర ngx_int_t ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *సంవర్గమానం,
    ngx_pool_t *pool);
స్థిర శూన్య ngx_thread_pool_destroy(ngx_thread_pool_t *tp);
స్థిర శూన్య ngx_thread_pool_exit_handler(శూన్య *దత్తాంశం, ngx_log_t *సంవర్గమానం);

స్థిర శూన్య *ngx_thread_pool_చక్రం(శూన్య *దత్తాంశం);
స్థిర శూన్య ngx_thread_pool_handler(ngx_event_t *ev);

స్థిర అక్షరం *ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf);

స్థిర శూన్య *ngx_thread_pool_create_conf(ngx_చక్రం_t *చక్రం);
స్థిర అక్షరం *ngx_thread_pool_init_conf(ngx_చక్రం_t *చక్రం, శూన్య *conf);

స్థిర ngx_int_t ngx_thread_pool_init_కార్మికుడు(ngx_చక్రం_t *చక్రం);
స్థిర శూన్య ngx_thread_pool_exit_కార్మికుడు(ngx_చక్రం_t *చక్రం);


స్థిర ngx_command_t  ngx_thread_pool_commands[] = {

    { ngx_string("thread_pool"),
      NGX_MAIN_CONF|NGX_ప్రత్యక్ష_CONF|NGX_CONF_TAKE23,
      ngx_thread_pool,
      0,
      0,
      ఖాళీ },

      ngx_null_command
};


స్థిర ngx_core_module_t  ngx_thread_pool_module_ctx = {
    ngx_string("thread_pool"),
    ngx_thread_pool_create_conf,
    ngx_thread_pool_init_conf
};


ngx_module_t  ngx_thread_pool_module = {
    NGX_MODULE_V1,
    &ngx_thread_pool_module_ctx,           /* module context */
    ngx_thread_pool_commands,              /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    ఖాళీ,                                  /* init master */
    ఖాళీ,                                  /* init module */
    ngx_thread_pool_init_కార్మికుడు,           /* init process */
    ఖాళీ,                                  /* init thread */
    ఖాళీ,                                  /* exit thread */
    ngx_thread_pool_exit_కార్మికుడు,           /* exit process */
    ఖాళీ,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


స్థిర ngx_str_t  ngx_thread_pool_default = ngx_string("default");

స్థిర ngx_uint_t               ngx_thread_pool_task_id;
స్థిర ngx_atomic_t             ngx_thread_pool_పూర్తి_తాళం;
స్థిర ngx_thread_pool_వరుస_t  ngx_thread_pool_పూర్తి;


స్థిర ngx_int_t
ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *సంవర్గమానం, ngx_pool_t *pool)
{
    పూర్ణాంకం             err;
    pthread_t       tid;
    ngx_uint_t      n;
    pthread_attr_t  attr;

    ఒకవేళ (ngx_notify == ఖాళీ) {
        ngx_log_error(NGX_LOG_ALERT, సంవర్గమానం, 0,
               "the configured event method cannot be used with thread pools");
        తిరిగి NGX_ERROR;
    }

    ngx_thread_pool_వరుస_init(&tp->వరుస);

    ఒకవేళ (ngx_thread_mutex_create(&tp->mtx, సంవర్గమానం) != NGX_OK) {
        తిరిగి NGX_ERROR;
    }

    ఒకవేళ (ngx_thread_cond_create(&tp->cond, సంవర్గమానం) != NGX_OK) {
        (శూన్య) ngx_thread_mutex_destroy(&tp->mtx, సంవర్గమానం);
        తిరిగి NGX_ERROR;
    }

    tp->సంవర్గమానం = సంవర్గమానం;

    err = pthread_attr_init(&attr);
    ఒకవేళ (err) {
        ngx_log_error(NGX_LOG_ALERT, సంవర్గమానం, err,
                      "pthread_attr_init() failed");
        తిరిగి NGX_ERROR;
    }

    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    ఒకవేళ (err) {
        ngx_log_error(NGX_LOG_ALERT, సంవర్గమానం, err,
                      "pthread_attr_setdetachstate() failed");
        తిరిగి NGX_ERROR;
    }

#ఒకవేళ 0
    err = pthread_attr_setstacksize(&attr, PTHREAD_దొంతర_MIN);
    ఒకవేళ (err) {
        ngx_log_error(NGX_LOG_ALERT, సంవర్గమానం, err,
                      "pthread_attr_setstacksize() failed");
        తిరిగి NGX_ERROR;
    }
#అయితే_ముగింపు

    కొరకు (n = 0; n < tp->దారాలు; n++) {
        err = pthread_create(&tid, &attr, ngx_thread_pool_చక్రం, tp);
        ఒకవేళ (err) {
            ngx_log_error(NGX_LOG_ALERT, సంవర్గమానం, err,
                          "pthread_create() failed");
            తిరిగి NGX_ERROR;
        }
    }

    (శూన్య) pthread_attr_destroy(&attr);

    తిరిగి NGX_OK;
}


స్థిర శూన్య
ngx_thread_pool_destroy(ngx_thread_pool_t *tp)
{
    ngx_uint_t           n;
    ngx_thread_task_t    task;
    అస్థిర ngx_uint_t  తాళం;

    ngx_memzero(&task, పరిమాణం(ngx_thread_task_t));

    task.handler = ngx_thread_pool_exit_handler;
    task.ctx = (శూన్య *) &తాళం;

    కొరకు (n = 0; n < tp->దారాలు; n++) {
        తాళం = 1;

        ఒకవేళ (ngx_thread_task_post(tp, &task) != NGX_OK) {
            తిరిగి;
        }

        అయితే (తాళం) {
            ngx_sched_yield();
        }

        task.event.చురుకైన = 0;
    }

    (శూన్య) ngx_thread_cond_destroy(&tp->cond, tp->సంవర్గమానం);

    (శూన్య) ngx_thread_mutex_destroy(&tp->mtx, tp->సంవర్గమానం);
}


స్థిర శూన్య
ngx_thread_pool_exit_handler(శూన్య *దత్తాంశం, ngx_log_t *సంవర్గమానం)
{
    ngx_uint_t *తాళం = దత్తాంశం;

    *తాళం = 0;

    pthread_exit(ఖాళీ);
}


ngx_thread_task_t *
ngx_thread_task_alloc(ngx_pool_t *pool, కొలత_t కొలత)
{
    ngx_thread_task_t  *task;

    task = ngx_pcalloc(pool, పరిమాణం(ngx_thread_task_t) + కొలత);
    ఒకవేళ (task == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    task->ctx = task + 1;

    తిరిగి task;
}


ngx_int_t
ngx_thread_task_post(ngx_thread_pool_t *tp, ngx_thread_task_t *task)
{
    ఒకవేళ (task->event.చురుకైన) {
        ngx_log_error(NGX_LOG_ALERT, tp->సంవర్గమానం, 0,
                      "task #%ui already active", task->id);
        తిరిగి NGX_ERROR;
    }

    ఒకవేళ (ngx_thread_mutex_తాళం(&tp->mtx, tp->సంవర్గమానం) != NGX_OK) {
        తిరిగి NGX_ERROR;
    }

    ఒకవేళ (tp->నిరీక్షణ >= tp->max_వరుస) {
        (శూన్య) ngx_thread_mutex_unlock(&tp->mtx, tp->సంవర్గమానం);

        ngx_log_error(NGX_LOG_ERR, tp->సంవర్గమానం, 0,
                      "thread pool \"%V\" queue overflow: %i tasks waiting",
                      &tp->name, tp->నిరీక్షణ);
        తిరిగి NGX_ERROR;
    }

    task->event.చురుకైన = 1;

    task->id = ngx_thread_pool_task_id++;
    task->తదుపరి = ఖాళీ;

    ఒకవేళ (ngx_thread_cond_signal(&tp->cond, tp->సంవర్గమానం) != NGX_OK) {
        (శూన్య) ngx_thread_mutex_unlock(&tp->mtx, tp->సంవర్గమానం);
        తిరిగి NGX_ERROR;
    }

    *tp->వరుస.చివరి = task;
    tp->వరుస.చివరి = &task->తదుపరి;

    tp->నిరీక్షణ++;

    (శూన్య) ngx_thread_mutex_unlock(&tp->mtx, tp->సంవర్గమానం);

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->సంవర్గమానం, 0,
                   "task #%ui added to thread pool \"%V\"",
                   task->id, &tp->name);

    తిరిగి NGX_OK;
}


స్థిర శూన్య *
ngx_thread_pool_చక్రం(శూన్య *దత్తాంశం)
{
    ngx_thread_pool_t *tp = దత్తాంశం;

    పూర్ణాంకం                 err;
    sigset_t            సమితి;
    ngx_thread_task_t  *task;

#ఒకవేళ 0
    ngx_సమయం_update();
#అయితే_ముగింపు

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, tp->సంవర్గమానం, 0,
                   "thread in pool \"%V\" started", &tp->name);

    sigfillset(&సమితి);

    sigdelset(&సమితి, SIGILL);
    sigdelset(&సమితి, SIGFPE);
    sigdelset(&సమితి, SIGSEGV);
    sigdelset(&సమితి, SIGBUS);

    err = pthread_sigmask(SIG_ఖండం, &సమితి, ఖాళీ);
    ఒకవేళ (err) {
        ngx_log_error(NGX_LOG_ALERT, tp->సంవర్గమానం, err, "pthread_sigmask() failed");
        తిరిగి ఖాళీ;
    }

    కొరకు ( ;; ) {
        ఒకవేళ (ngx_thread_mutex_తాళం(&tp->mtx, tp->సంవర్గమానం) != NGX_OK) {
            తిరిగి ఖాళీ;
        }

        /* the number may become negative */
        tp->నిరీక్షణ--;

        అయితే (tp->వరుస.మొదటి == ఖాళీ) {
            ఒకవేళ (ngx_thread_cond_wait(&tp->cond, &tp->mtx, tp->సంవర్గమానం)
                != NGX_OK)
            {
                (శూన్య) ngx_thread_mutex_unlock(&tp->mtx, tp->సంవర్గమానం);
                తిరిగి ఖాళీ;
            }
        }

        task = tp->వరుస.మొదటి;
        tp->వరుస.మొదటి = task->తదుపరి;

        ఒకవేళ (tp->వరుస.మొదటి == ఖాళీ) {
            tp->వరుస.చివరి = &tp->వరుస.మొదటి;
        }

        ఒకవేళ (ngx_thread_mutex_unlock(&tp->mtx, tp->సంవర్గమానం) != NGX_OK) {
            తిరిగి ఖాళీ;
        }

#ఒకవేళ 0
        ngx_సమయం_update();
#అయితే_ముగింపు

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->సంవర్గమానం, 0,
                       "run task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->handler(task->ctx, tp->సంవర్గమానం);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->సంవర్గమానం, 0,
                       "complete task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->తదుపరి = ఖాళీ;

        ngx_spinlock(&ngx_thread_pool_పూర్తి_తాళం, 1, 2048);

        *ngx_thread_pool_పూర్తి.చివరి = task;
        ngx_thread_pool_పూర్తి.చివరి = &task->తదుపరి;

        ngx_జ్ఞాపకం_barrier();

        ngx_unlock(&ngx_thread_pool_పూర్తి_తాళం);

        (శూన్య) ngx_notify(ngx_thread_pool_handler);
    }
}


స్థిర శూన్య
ngx_thread_pool_handler(ngx_event_t *ev)
{
    ngx_event_t        *event;
    ngx_thread_task_t  *task;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ev->సంవర్గమానం, 0, "thread pool handler");

    ngx_spinlock(&ngx_thread_pool_పూర్తి_తాళం, 1, 2048);

    task = ngx_thread_pool_పూర్తి.మొదటి;
    ngx_thread_pool_పూర్తి.మొదటి = ఖాళీ;
    ngx_thread_pool_పూర్తి.చివరి = &ngx_thread_pool_పూర్తి.మొదటి;

    ngx_జ్ఞాపకం_barrier();

    ngx_unlock(&ngx_thread_pool_పూర్తి_తాళం);

    అయితే (task) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->సంవర్గమానం, 0,
                       "run completion handler for task #%ui", task->id);

        event = &task->event;
        task = task->తదుపరి;

        event->complete = 1;
        event->చురుకైన = 0;

        event->handler(event);
    }
}


స్థిర శూన్య *
ngx_thread_pool_create_conf(ngx_చక్రం_t *చక్రం)
{
    ngx_thread_pool_conf_t  *tcf;

    tcf = ngx_pcalloc(చక్రం->pool, పరిమాణం(ngx_thread_pool_conf_t));
    ఒకవేళ (tcf == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    ఒకవేళ (ngx_array_init(&tcf->pools, చక్రం->pool, 4,
                       పరిమాణం(ngx_thread_pool_t *))
        != NGX_OK)
    {
        తిరిగి ఖాళీ;
    }

    తిరిగి tcf;
}


స్థిర అక్షరం *
ngx_thread_pool_init_conf(ngx_చక్రం_t *చక్రం, శూన్య *conf)
{
    ngx_thread_pool_conf_t *tcf = conf;

    ngx_uint_t           i;
    ngx_thread_pool_t  **tpp;

    tpp = tcf->pools.elts;

    కొరకు (i = 0; i < tcf->pools.nelts; i++) {

        ఒకవేళ (tpp[i]->దారాలు) {
            కొనసాగు;
        }

        ఒకవేళ (tpp[i]->name.len == ngx_thread_pool_default.len
            && ngx_strncmp(tpp[i]->name.దత్తాంశం, ngx_thread_pool_default.దత్తాంశం,
                           ngx_thread_pool_default.len)
               == 0)
        {
            tpp[i]->దారాలు = 32;
            tpp[i]->max_వరుస = 65536;
            కొనసాగు;
        }

        ngx_log_error(NGX_LOG_EMERG, చక్రం->సంవర్గమానం, 0,
                      "unknown thread pool \"%V\" in %s:%ui",
                      &tpp[i]->name, tpp[i]->file, tpp[i]->పంక్తి);

        తిరిగి NGX_CONF_ERROR;
    }

    తిరిగి NGX_CONF_OK;
}


స్థిర అక్షరం *
ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf)
{
    ngx_str_t          *విలువ;
    ngx_uint_t          i;
    ngx_thread_pool_t  *tp;

    విలువ = cf->వాదనలు->elts;

    tp = ngx_thread_pool_కూడు(cf, &విలువ[1]);

    ఒకవేళ (tp == ఖాళీ) {
        తిరిగి NGX_CONF_ERROR;
    }

    ఒకవేళ (tp->దారాలు) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "duplicate thread pool \"%V\"", &tp->name);
        తిరిగి NGX_CONF_ERROR;
    }

    tp->max_వరుస = 65536;

    కొరకు (i = 2; i < cf->వాదనలు->nelts; i++) {

        ఒకవేళ (ngx_strncmp(విలువ[i].దత్తాంశం, "threads=", 8) == 0) {

            tp->దారాలు = ngx_atoi(విలువ[i].దత్తాంశం + 8, విలువ[i].len - 8);

            ఒకవేళ (tp->దారాలు == (ngx_uint_t) NGX_ERROR || tp->దారాలు == 0) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid threads value \"%V\"", &విలువ[i]);
                తిరిగి NGX_CONF_ERROR;
            }

            కొనసాగు;
        }

        ఒకవేళ (ngx_strncmp(విలువ[i].దత్తాంశం, "max_queue=", 10) == 0) {

            tp->max_వరుస = ngx_atoi(విలువ[i].దత్తాంశం + 10, విలువ[i].len - 10);

            ఒకవేళ (tp->max_వరుస == NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid max_queue value \"%V\"", &విలువ[i]);
                తిరిగి NGX_CONF_ERROR;
            }

            కొనసాగు;
        }
    }

    ఒకవేళ (tp->దారాలు == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%V\" must have \"threads\" parameter",
                           &cmd->name);
        తిరిగి NGX_CONF_ERROR;
    }

    తిరిగి NGX_CONF_OK;
}


ngx_thread_pool_t *
ngx_thread_pool_కూడు(ngx_conf_t *cf, ngx_str_t *name)
{
    ngx_thread_pool_t       *tp, **tpp;
    ngx_thread_pool_conf_t  *tcf;

    ఒకవేళ (name == ఖాళీ) {
        name = &ngx_thread_pool_default;
    }

    tp = ngx_thread_pool_get(cf->చక్రం, name);

    ఒకవేళ (tp) {
        తిరిగి tp;
    }

    tp = ngx_pcalloc(cf->pool, పరిమాణం(ngx_thread_pool_t));
    ఒకవేళ (tp == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    tp->name = *name;
    tp->file = cf->conf_file->file.name.దత్తాంశం;
    tp->పంక్తి = cf->conf_file->పంక్తి;

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cf->చక్రం->conf_ctx,
                                                  ngx_thread_pool_module);

    tpp = ngx_array_నెట్టు(&tcf->pools);
    ఒకవేళ (tpp == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    *tpp = tp;

    తిరిగి tp;
}


ngx_thread_pool_t *
ngx_thread_pool_get(ngx_చక్రం_t *చక్రం, ngx_str_t *name)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(చక్రం->conf_ctx,
                                                  ngx_thread_pool_module);

    tpp = tcf->pools.elts;

    కొరకు (i = 0; i < tcf->pools.nelts; i++) {

        ఒకవేళ (tpp[i]->name.len == name->len
            && ngx_strncmp(tpp[i]->name.దత్తాంశం, name->దత్తాంశం, name->len) == 0)
        {
            తిరిగి tpp[i];
        }
    }

    తిరిగి ఖాళీ;
}


స్థిర ngx_int_t
ngx_thread_pool_init_కార్మికుడు(ngx_చక్రం_t *చక్రం)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    ఒకవేళ (ngx_process != NGX_PROCESS_కార్మికుడు
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        తిరిగి NGX_OK;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(చక్రం->conf_ctx,
                                                  ngx_thread_pool_module);

    ఒకవేళ (tcf == ఖాళీ) {
        తిరిగి NGX_OK;
    }

    ngx_thread_pool_వరుస_init(&ngx_thread_pool_పూర్తి);

    tpp = tcf->pools.elts;

    కొరకు (i = 0; i < tcf->pools.nelts; i++) {
        ఒకవేళ (ngx_thread_pool_init(tpp[i], చక్రం->సంవర్గమానం, చక్రం->pool) != NGX_OK) {
            తిరిగి NGX_ERROR;
        }
    }

    తిరిగి NGX_OK;
}


స్థిర శూన్య
ngx_thread_pool_exit_కార్మికుడు(ngx_చక్రం_t *చక్రం)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    ఒకవేళ (ngx_process != NGX_PROCESS_కార్మికుడు
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        తిరిగి;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(చక్రం->conf_ctx,
                                                  ngx_thread_pool_module);

    ఒకవేళ (tcf == ఖాళీ) {
        తిరిగి;
    }

    tpp = tcf->pools.elts;

    కొరకు (i = 0; i < tcf->pools.nelts; i++) {
        ngx_thread_pool_destroy(tpp[i]);
    }
}
