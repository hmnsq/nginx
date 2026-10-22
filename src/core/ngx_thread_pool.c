
/*
 * Copyright (C) Nginx, Inc.
 * Copyright (C) Valentin V. Bartenev
 * Copyright (C) Ruslan Ermilov
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <ngx_thread_pool.h>


تعريفنوع بنية {
    ngx_array_t               pools;
} ngx_thread_pool_conf_t;


تعريفنوع بنية {
    ngx_thread_task_t        *پهريون;
    ngx_thread_task_t       **آخري;
} ngx_thread_pool_قطار_t;

#وصف_ڪريو ngx_thread_pool_قطار_init(q)                                         \
    (q)->پهريون = عدم;                                                        \
    (q)->آخري = &(q)->پهريون


بنية ngx_thread_pool_s {
    ngx_thread_mutex_t        mtx;
    ngx_thread_pool_قطار_t   قطار;
    ngx_int_t                 انتظار;
    ngx_thread_cond_t         cond;

    ngx_log_t                *لاگرٿم;

    ngx_str_t                 name;
    ngx_uint_t                ڌاڳا;
    ngx_int_t                 max_قطار;

    u_char                   *file;
    ngx_uint_t                سٽ;
};


ساكن ngx_int_t ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *لاگرٿم,
    ngx_pool_t *pool);
ساكن فراغ ngx_thread_pool_destroy(ngx_thread_pool_t *tp);
ساكن فراغ ngx_thread_pool_exit_handler(فراغ *معلومات, ngx_log_t *لاگرٿم);

ساكن فراغ *ngx_thread_pool_چڪر(فراغ *معلومات);
ساكن فراغ ngx_thread_pool_handler(ngx_event_t *ev);

ساكن حرف *ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);

ساكن فراغ *ngx_thread_pool_create_conf(ngx_چڪر_t *چڪر);
ساكن حرف *ngx_thread_pool_init_conf(ngx_چڪر_t *چڪر, فراغ *conf);

ساكن ngx_int_t ngx_thread_pool_init_ڪم_ڪندڙ(ngx_چڪر_t *چڪر);
ساكن فراغ ngx_thread_pool_exit_ڪم_ڪندڙ(ngx_چڪر_t *چڪر);


ساكن ngx_command_t  ngx_thread_pool_commands[] = {

    { ngx_string("thread_pool"),
      NGX_MAIN_CONF|NGX_سڌو_CONF|NGX_CONF_TAKE23,
      ngx_thread_pool,
      0,
      0,
      عدم },

      ngx_null_command
};


ساكن ngx_core_module_t  ngx_thread_pool_module_ctx = {
    ngx_string("thread_pool"),
    ngx_thread_pool_create_conf,
    ngx_thread_pool_init_conf
};


ngx_module_t  ngx_thread_pool_module = {
    NGX_MODULE_V1,
    &ngx_thread_pool_module_ctx,           /* module context */
    ngx_thread_pool_commands,              /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    عدم,                                  /* init master */
    عدم,                                  /* init module */
    ngx_thread_pool_init_ڪم_ڪندڙ,           /* init process */
    عدم,                                  /* init thread */
    عدم,                                  /* exit thread */
    ngx_thread_pool_exit_ڪم_ڪندڙ,           /* exit process */
    عدم,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


ساكن ngx_str_t  ngx_thread_pool_default = ngx_string("default");

ساكن ngx_uint_t               ngx_thread_pool_task_id;
ساكن ngx_atomic_t             ngx_thread_pool_پورو_تالو;
ساكن ngx_thread_pool_قطار_t  ngx_thread_pool_پورو;


ساكن ngx_int_t
ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *لاگرٿم, ngx_pool_t *pool)
{
    صحيح             err;
    pthread_t       tid;
    ngx_uint_t      n;
    pthread_attr_t  attr;

    إذا (ngx_notify == عدم) {
        ngx_log_error(NGX_LOG_ALERT, لاگرٿم, 0,
               "the configured event method cannot be used with thread pools");
        أرجع NGX_ERROR;
    }

    ngx_thread_pool_قطار_init(&tp->قطار);

    إذا (ngx_thread_mutex_create(&tp->mtx, لاگرٿم) != NGX_OK) {
        أرجع NGX_ERROR;
    }

    إذا (ngx_thread_cond_create(&tp->cond, لاگرٿم) != NGX_OK) {
        (فراغ) ngx_thread_mutex_destroy(&tp->mtx, لاگرٿم);
        أرجع NGX_ERROR;
    }

    tp->لاگرٿم = لاگرٿم;

    err = pthread_attr_init(&attr);
    إذا (err) {
        ngx_log_error(NGX_LOG_ALERT, لاگرٿم, err,
                      "pthread_attr_init() failed");
        أرجع NGX_ERROR;
    }

    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    إذا (err) {
        ngx_log_error(NGX_LOG_ALERT, لاگرٿم, err,
                      "pthread_attr_setdetachstate() failed");
        أرجع NGX_ERROR;
    }

#إذا 0
    err = pthread_attr_setstacksize(&attr, PTHREAD_ڍير_MIN);
    إذا (err) {
        ngx_log_error(NGX_LOG_ALERT, لاگرٿم, err,
                      "pthread_attr_setstacksize() failed");
        أرجع NGX_ERROR;
    }
#جيڪڏهن_پورو

    لكل (n = 0; n < tp->ڌاڳا; n++) {
        err = pthread_create(&tid, &attr, ngx_thread_pool_چڪر, tp);
        إذا (err) {
            ngx_log_error(NGX_LOG_ALERT, لاگرٿم, err,
                          "pthread_create() failed");
            أرجع NGX_ERROR;
        }
    }

    (فراغ) pthread_attr_destroy(&attr);

    أرجع NGX_OK;
}


ساكن فراغ
ngx_thread_pool_destroy(ngx_thread_pool_t *tp)
{
    ngx_uint_t           n;
    ngx_thread_task_t    task;
    متطاير ngx_uint_t  تالو;

    ngx_memzero(&task, حجم(ngx_thread_task_t));

    task.handler = ngx_thread_pool_exit_handler;
    task.ctx = (فراغ *) &تالو;

    لكل (n = 0; n < tp->ڌاڳا; n++) {
        تالو = 1;

        إذا (ngx_thread_task_post(tp, &task) != NGX_OK) {
            أرجع;
        }

        طالما (تالو) {
            ngx_sched_yield();
        }

        task.event.سرگرم = 0;
    }

    (فراغ) ngx_thread_cond_destroy(&tp->cond, tp->لاگرٿم);

    (فراغ) ngx_thread_mutex_destroy(&tp->mtx, tp->لاگرٿم);
}


ساكن فراغ
ngx_thread_pool_exit_handler(فراغ *معلومات, ngx_log_t *لاگرٿم)
{
    ngx_uint_t *تالو = معلومات;

    *تالو = 0;

    pthread_exit(عدم);
}


ngx_thread_task_t *
ngx_thread_task_alloc(ngx_pool_t *pool, سائيز_t سائيز)
{
    ngx_thread_task_t  *task;

    task = ngx_pcalloc(pool, حجم(ngx_thread_task_t) + سائيز);
    إذا (task == عدم) {
        أرجع عدم;
    }

    task->ctx = task + 1;

    أرجع task;
}


ngx_int_t
ngx_thread_task_post(ngx_thread_pool_t *tp, ngx_thread_task_t *task)
{
    إذا (task->event.سرگرم) {
        ngx_log_error(NGX_LOG_ALERT, tp->لاگرٿم, 0,
                      "task #%ui already active", task->id);
        أرجع NGX_ERROR;
    }

    إذا (ngx_thread_mutex_تالو(&tp->mtx, tp->لاگرٿم) != NGX_OK) {
        أرجع NGX_ERROR;
    }

    إذا (tp->انتظار >= tp->max_قطار) {
        (فراغ) ngx_thread_mutex_unlock(&tp->mtx, tp->لاگرٿم);

        ngx_log_error(NGX_LOG_ERR, tp->لاگرٿم, 0,
                      "thread pool \"%V\" queue overflow: %i tasks waiting",
                      &tp->name, tp->انتظار);
        أرجع NGX_ERROR;
    }

    task->event.سرگرم = 1;

    task->id = ngx_thread_pool_task_id++;
    task->ايندڙ = عدم;

    إذا (ngx_thread_cond_signal(&tp->cond, tp->لاگرٿم) != NGX_OK) {
        (فراغ) ngx_thread_mutex_unlock(&tp->mtx, tp->لاگرٿم);
        أرجع NGX_ERROR;
    }

    *tp->قطار.آخري = task;
    tp->قطار.آخري = &task->ايندڙ;

    tp->انتظار++;

    (فراغ) ngx_thread_mutex_unlock(&tp->mtx, tp->لاگرٿم);

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->لاگرٿم, 0,
                   "task #%ui added to thread pool \"%V\"",
                   task->id, &tp->name);

    أرجع NGX_OK;
}


ساكن فراغ *
ngx_thread_pool_چڪر(فراغ *معلومات)
{
    ngx_thread_pool_t *tp = معلومات;

    صحيح                 err;
    sigset_t            سيٽ;
    ngx_thread_task_t  *task;

#إذا 0
    ngx_وقت_update();
#جيڪڏهن_پورو

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, tp->لاگرٿم, 0,
                   "thread in pool \"%V\" started", &tp->name);

    sigfillset(&سيٽ);

    sigdelset(&سيٽ, SIGILL);
    sigdelset(&سيٽ, SIGFPE);
    sigdelset(&سيٽ, SIGSEGV);
    sigdelset(&سيٽ, SIGBUS);

    err = pthread_sigmask(SIG_بلاڪ, &سيٽ, عدم);
    إذا (err) {
        ngx_log_error(NGX_LOG_ALERT, tp->لاگرٿم, err, "pthread_sigmask() failed");
        أرجع عدم;
    }

    لكل ( ;; ) {
        إذا (ngx_thread_mutex_تالو(&tp->mtx, tp->لاگرٿم) != NGX_OK) {
            أرجع عدم;
        }

        /* the number may become negative */
        tp->انتظار--;

        طالما (tp->قطار.پهريون == عدم) {
            إذا (ngx_thread_cond_wait(&tp->cond, &tp->mtx, tp->لاگرٿم)
                != NGX_OK)
            {
                (فراغ) ngx_thread_mutex_unlock(&tp->mtx, tp->لاگرٿم);
                أرجع عدم;
            }
        }

        task = tp->قطار.پهريون;
        tp->قطار.پهريون = task->ايندڙ;

        إذا (tp->قطار.پهريون == عدم) {
            tp->قطار.آخري = &tp->قطار.پهريون;
        }

        إذا (ngx_thread_mutex_unlock(&tp->mtx, tp->لاگرٿم) != NGX_OK) {
            أرجع عدم;
        }

#إذا 0
        ngx_وقت_update();
#جيڪڏهن_پورو

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->لاگرٿم, 0,
                       "run task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->handler(task->ctx, tp->لاگرٿم);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->لاگرٿم, 0,
                       "complete task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->ايندڙ = عدم;

        ngx_spinlock(&ngx_thread_pool_پورو_تالو, 1, 2048);

        *ngx_thread_pool_پورو.آخري = task;
        ngx_thread_pool_پورو.آخري = &task->ايندڙ;

        ngx_يادگيري_barrier();

        ngx_unlock(&ngx_thread_pool_پورو_تالو);

        (فراغ) ngx_notify(ngx_thread_pool_handler);
    }
}


ساكن فراغ
ngx_thread_pool_handler(ngx_event_t *ev)
{
    ngx_event_t        *event;
    ngx_thread_task_t  *task;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ev->لاگرٿم, 0, "thread pool handler");

    ngx_spinlock(&ngx_thread_pool_پورو_تالو, 1, 2048);

    task = ngx_thread_pool_پورو.پهريون;
    ngx_thread_pool_پورو.پهريون = عدم;
    ngx_thread_pool_پورو.آخري = &ngx_thread_pool_پورو.پهريون;

    ngx_يادگيري_barrier();

    ngx_unlock(&ngx_thread_pool_پورو_تالو);

    طالما (task) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->لاگرٿم, 0,
                       "run completion handler for task #%ui", task->id);

        event = &task->event;
        task = task->ايندڙ;

        event->complete = 1;
        event->سرگرم = 0;

        event->handler(event);
    }
}


ساكن فراغ *
ngx_thread_pool_create_conf(ngx_چڪر_t *چڪر)
{
    ngx_thread_pool_conf_t  *tcf;

    tcf = ngx_pcalloc(چڪر->pool, حجم(ngx_thread_pool_conf_t));
    إذا (tcf == عدم) {
        أرجع عدم;
    }

    إذا (ngx_array_init(&tcf->pools, چڪر->pool, 4,
                       حجم(ngx_thread_pool_t *))
        != NGX_OK)
    {
        أرجع عدم;
    }

    أرجع tcf;
}


ساكن حرف *
ngx_thread_pool_init_conf(ngx_چڪر_t *چڪر, فراغ *conf)
{
    ngx_thread_pool_conf_t *tcf = conf;

    ngx_uint_t           i;
    ngx_thread_pool_t  **tpp;

    tpp = tcf->pools.elts;

    لكل (i = 0; i < tcf->pools.nelts; i++) {

        إذا (tpp[i]->ڌاڳا) {
            استمر;
        }

        إذا (tpp[i]->name.len == ngx_thread_pool_default.len
            && ngx_strncmp(tpp[i]->name.معلومات, ngx_thread_pool_default.معلومات,
                           ngx_thread_pool_default.len)
               == 0)
        {
            tpp[i]->ڌاڳا = 32;
            tpp[i]->max_قطار = 65536;
            استمر;
        }

        ngx_log_error(NGX_LOG_EMERG, چڪر->لاگرٿم, 0,
                      "unknown thread pool \"%V\" in %s:%ui",
                      &tpp[i]->name, tpp[i]->file, tpp[i]->سٽ);

        أرجع NGX_CONF_ERROR;
    }

    أرجع NGX_CONF_OK;
}


ساكن حرف *
ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    ngx_str_t          *قدر;
    ngx_uint_t          i;
    ngx_thread_pool_t  *tp;

    قدر = cf->دليلون->elts;

    tp = ngx_thread_pool_جوڙيو(cf, &قدر[1]);

    إذا (tp == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    إذا (tp->ڌاڳا) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "duplicate thread pool \"%V\"", &tp->name);
        أرجع NGX_CONF_ERROR;
    }

    tp->max_قطار = 65536;

    لكل (i = 2; i < cf->دليلون->nelts; i++) {

        إذا (ngx_strncmp(قدر[i].معلومات, "threads=", 8) == 0) {

            tp->ڌاڳا = ngx_atoi(قدر[i].معلومات + 8, قدر[i].len - 8);

            إذا (tp->ڌاڳا == (ngx_uint_t) NGX_ERROR || tp->ڌاڳا == 0) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid threads value \"%V\"", &قدر[i]);
                أرجع NGX_CONF_ERROR;
            }

            استمر;
        }

        إذا (ngx_strncmp(قدر[i].معلومات, "max_queue=", 10) == 0) {

            tp->max_قطار = ngx_atoi(قدر[i].معلومات + 10, قدر[i].len - 10);

            إذا (tp->max_قطار == NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid max_queue value \"%V\"", &قدر[i]);
                أرجع NGX_CONF_ERROR;
            }

            استمر;
        }
    }

    إذا (tp->ڌاڳا == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%V\" must have \"threads\" parameter",
                           &cmd->name);
        أرجع NGX_CONF_ERROR;
    }

    أرجع NGX_CONF_OK;
}


ngx_thread_pool_t *
ngx_thread_pool_جوڙيو(ngx_conf_t *cf, ngx_str_t *name)
{
    ngx_thread_pool_t       *tp, **tpp;
    ngx_thread_pool_conf_t  *tcf;

    إذا (name == عدم) {
        name = &ngx_thread_pool_default;
    }

    tp = ngx_thread_pool_get(cf->چڪر, name);

    إذا (tp) {
        أرجع tp;
    }

    tp = ngx_pcalloc(cf->pool, حجم(ngx_thread_pool_t));
    إذا (tp == عدم) {
        أرجع عدم;
    }

    tp->name = *name;
    tp->file = cf->conf_file->file.name.معلومات;
    tp->سٽ = cf->conf_file->سٽ;

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cf->چڪر->conf_ctx,
                                                  ngx_thread_pool_module);

    tpp = ngx_array_ڌڪو(&tcf->pools);
    إذا (tpp == عدم) {
        أرجع عدم;
    }

    *tpp = tp;

    أرجع tp;
}


ngx_thread_pool_t *
ngx_thread_pool_get(ngx_چڪر_t *چڪر, ngx_str_t *name)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(چڪر->conf_ctx,
                                                  ngx_thread_pool_module);

    tpp = tcf->pools.elts;

    لكل (i = 0; i < tcf->pools.nelts; i++) {

        إذا (tpp[i]->name.len == name->len
            && ngx_strncmp(tpp[i]->name.معلومات, name->معلومات, name->len) == 0)
        {
            أرجع tpp[i];
        }
    }

    أرجع عدم;
}


ساكن ngx_int_t
ngx_thread_pool_init_ڪم_ڪندڙ(ngx_چڪر_t *چڪر)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    إذا (ngx_process != NGX_PROCESS_ڪم_ڪندڙ
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        أرجع NGX_OK;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(چڪر->conf_ctx,
                                                  ngx_thread_pool_module);

    إذا (tcf == عدم) {
        أرجع NGX_OK;
    }

    ngx_thread_pool_قطار_init(&ngx_thread_pool_پورو);

    tpp = tcf->pools.elts;

    لكل (i = 0; i < tcf->pools.nelts; i++) {
        إذا (ngx_thread_pool_init(tpp[i], چڪر->لاگرٿم, چڪر->pool) != NGX_OK) {
            أرجع NGX_ERROR;
        }
    }

    أرجع NGX_OK;
}


ساكن فراغ
ngx_thread_pool_exit_ڪم_ڪندڙ(ngx_چڪر_t *چڪر)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    إذا (ngx_process != NGX_PROCESS_ڪم_ڪندڙ
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        أرجع;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(چڪر->conf_ctx,
                                                  ngx_thread_pool_module);

    إذا (tcf == عدم) {
        أرجع;
    }

    tpp = tcf->pools.elts;

    لكل (i = 0; i < tcf->pools.nelts; i++) {
        ngx_thread_pool_destroy(tpp[i]);
    }
}
