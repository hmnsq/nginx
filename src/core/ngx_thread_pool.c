
/*
 * Copyright (C) Nginx, Inc.
 * Copyright (C) Valentin V. Bartenev
 * Copyright (C) Ruslan Ermilov
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_thread_pool.h>


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_array_t               pools;
} ngx_thread_pool_conf_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_thread_task_t        *ਪਹਿਲਾ;
    ngx_thread_task_t       **ਆਖਰੀ;
} ngx_thread_pool_ਕਤਾਰ_t;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_thread_pool_ਕਤਾਰ_init(q)                                         \
    (q)->ਪਹਿਲਾ = ਨਿਰਾ;                                                        \
    (q)->ਆਖਰੀ = &(q)->ਪਹਿਲਾ


ਬਣਤਰ ngx_thread_pool_s {
    ngx_thread_mutex_t        mtx;
    ngx_thread_pool_ਕਤਾਰ_t   ਕਤਾਰ;
    ngx_int_t                 ਉਡੀਕ;
    ngx_thread_cond_t         cond;

    ngx_log_t                *ਲਘੂਗਣਕ;

    ngx_str_t                 name;
    ngx_uint_t                ਧਾਗੇ;
    ngx_int_t                 max_ਕਤਾਰ;

    u_char                   *file;
    ngx_uint_t                ਸਤਰ;
};


ਸਥਿਰ ngx_int_t ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *ਲਘੂਗਣਕ,
    ngx_pool_t *pool);
ਸਥਿਰ ਖਾਲੀ ngx_thread_pool_destroy(ngx_thread_pool_t *tp);
ਸਥਿਰ ਖਾਲੀ ngx_thread_pool_exit_handler(ਖਾਲੀ *ਅੰਕੜੇ, ngx_log_t *ਲਘੂਗਣਕ);

ਸਥਿਰ ਖਾਲੀ *ngx_thread_pool_ਚੱਕਰ(ਖਾਲੀ *ਅੰਕੜੇ);
ਸਥਿਰ ਖਾਲੀ ngx_thread_pool_handler(ngx_event_t *ev);

ਸਥਿਰ ਅੱਖਰ *ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf);

ਸਥਿਰ ਖਾਲੀ *ngx_thread_pool_create_conf(ngx_ਚੱਕਰ_t *ਚੱਕਰ);
ਸਥਿਰ ਅੱਖਰ *ngx_thread_pool_init_conf(ngx_ਚੱਕਰ_t *ਚੱਕਰ, ਖਾਲੀ *conf);

ਸਥਿਰ ngx_int_t ngx_thread_pool_init_ਕਾਮਾ(ngx_ਚੱਕਰ_t *ਚੱਕਰ);
ਸਥਿਰ ਖਾਲੀ ngx_thread_pool_exit_ਕਾਮਾ(ngx_ਚੱਕਰ_t *ਚੱਕਰ);


ਸਥਿਰ ngx_command_t  ngx_thread_pool_commands[] = {

    { ngx_string("thread_pool"),
      NGX_MAIN_CONF|NGX_ਸਿੱਧਾ_CONF|NGX_CONF_TAKE23,
      ngx_thread_pool,
      0,
      0,
      ਨਿਰਾ },

      ngx_null_command
};


ਸਥਿਰ ngx_core_module_t  ngx_thread_pool_module_ctx = {
    ngx_string("thread_pool"),
    ngx_thread_pool_create_conf,
    ngx_thread_pool_init_conf
};


ngx_module_t  ngx_thread_pool_module = {
    NGX_MODULE_V1,
    &ngx_thread_pool_module_ctx,           /* module context */
    ngx_thread_pool_commands,              /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    ਨਿਰਾ,                                  /* init master */
    ਨਿਰਾ,                                  /* init module */
    ngx_thread_pool_init_ਕਾਮਾ,           /* init process */
    ਨਿਰਾ,                                  /* init thread */
    ਨਿਰਾ,                                  /* exit thread */
    ngx_thread_pool_exit_ਕਾਮਾ,           /* exit process */
    ਨਿਰਾ,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


ਸਥਿਰ ngx_str_t  ngx_thread_pool_default = ngx_string("default");

ਸਥਿਰ ngx_uint_t               ngx_thread_pool_task_id;
ਸਥਿਰ ngx_atomic_t             ngx_thread_pool_ਪੂਰਾ_ਤਾਲਾ;
ਸਥਿਰ ngx_thread_pool_ਕਤਾਰ_t  ngx_thread_pool_ਪੂਰਾ;


ਸਥਿਰ ngx_int_t
ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *ਲਘੂਗਣਕ, ngx_pool_t *pool)
{
    ਪੂਰਨ             err;
    pthread_t       tid;
    ngx_uint_t      n;
    pthread_attr_t  attr;

    ਜੇ (ngx_notify == ਨਿਰਾ) {
        ngx_log_error(NGX_LOG_ALERT, ਲਘੂਗਣਕ, 0,
               "the configured event method cannot be used with thread pools");
        ਵਾਪਸ NGX_ERROR;
    }

    ngx_thread_pool_ਕਤਾਰ_init(&tp->ਕਤਾਰ);

    ਜੇ (ngx_thread_mutex_create(&tp->mtx, ਲਘੂਗਣਕ) != NGX_OK) {
        ਵਾਪਸ NGX_ERROR;
    }

    ਜੇ (ngx_thread_cond_create(&tp->cond, ਲਘੂਗਣਕ) != NGX_OK) {
        (ਖਾਲੀ) ngx_thread_mutex_destroy(&tp->mtx, ਲਘੂਗਣਕ);
        ਵਾਪਸ NGX_ERROR;
    }

    tp->ਲਘੂਗਣਕ = ਲਘੂਗਣਕ;

    err = pthread_attr_init(&attr);
    ਜੇ (err) {
        ngx_log_error(NGX_LOG_ALERT, ਲਘੂਗਣਕ, err,
                      "pthread_attr_init() failed");
        ਵਾਪਸ NGX_ERROR;
    }

    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    ਜੇ (err) {
        ngx_log_error(NGX_LOG_ALERT, ਲਘੂਗਣਕ, err,
                      "pthread_attr_setdetachstate() failed");
        ਵਾਪਸ NGX_ERROR;
    }

#ਜੇ 0
    err = pthread_attr_setstacksize(&attr, PTHREAD_ਢੇਰ_MIN);
    ਜੇ (err) {
        ngx_log_error(NGX_LOG_ALERT, ਲਘੂਗਣਕ, err,
                      "pthread_attr_setstacksize() failed");
        ਵਾਪਸ NGX_ERROR;
    }
#ਜੇ_ਅੰਤ

    ਲਈ (n = 0; n < tp->ਧਾਗੇ; n++) {
        err = pthread_create(&tid, &attr, ngx_thread_pool_ਚੱਕਰ, tp);
        ਜੇ (err) {
            ngx_log_error(NGX_LOG_ALERT, ਲਘੂਗਣਕ, err,
                          "pthread_create() failed");
            ਵਾਪਸ NGX_ERROR;
        }
    }

    (ਖਾਲੀ) pthread_attr_destroy(&attr);

    ਵਾਪਸ NGX_OK;
}


ਸਥਿਰ ਖਾਲੀ
ngx_thread_pool_destroy(ngx_thread_pool_t *tp)
{
    ngx_uint_t           n;
    ngx_thread_task_t    task;
    ਅਸਥਿਰ ngx_uint_t  ਤਾਲਾ;

    ngx_memzero(&task, ਆਕਾਰ(ngx_thread_task_t));

    task.handler = ngx_thread_pool_exit_handler;
    task.ctx = (ਖਾਲੀ *) &ਤਾਲਾ;

    ਲਈ (n = 0; n < tp->ਧਾਗੇ; n++) {
        ਤਾਲਾ = 1;

        ਜੇ (ngx_thread_task_post(tp, &task) != NGX_OK) {
            ਵਾਪਸ;
        }

        ਜਦੋਂ (ਤਾਲਾ) {
            ngx_sched_yield();
        }

        task.event.ਸਰਗਰਮ = 0;
    }

    (ਖਾਲੀ) ngx_thread_cond_destroy(&tp->cond, tp->ਲਘੂਗਣਕ);

    (ਖਾਲੀ) ngx_thread_mutex_destroy(&tp->mtx, tp->ਲਘੂਗਣਕ);
}


ਸਥਿਰ ਖਾਲੀ
ngx_thread_pool_exit_handler(ਖਾਲੀ *ਅੰਕੜੇ, ngx_log_t *ਲਘੂਗਣਕ)
{
    ngx_uint_t *ਤਾਲਾ = ਅੰਕੜੇ;

    *ਤਾਲਾ = 0;

    pthread_exit(ਨਿਰਾ);
}


ngx_thread_task_t *
ngx_thread_task_alloc(ngx_pool_t *pool, ਮਾਪ_t ਮਾਪ)
{
    ngx_thread_task_t  *task;

    task = ngx_pcalloc(pool, ਆਕਾਰ(ngx_thread_task_t) + ਮਾਪ);
    ਜੇ (task == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    task->ctx = task + 1;

    ਵਾਪਸ task;
}


ngx_int_t
ngx_thread_task_post(ngx_thread_pool_t *tp, ngx_thread_task_t *task)
{
    ਜੇ (task->event.ਸਰਗਰਮ) {
        ngx_log_error(NGX_LOG_ALERT, tp->ਲਘੂਗਣਕ, 0,
                      "task #%ui already active", task->id);
        ਵਾਪਸ NGX_ERROR;
    }

    ਜੇ (ngx_thread_mutex_ਤਾਲਾ(&tp->mtx, tp->ਲਘੂਗਣਕ) != NGX_OK) {
        ਵਾਪਸ NGX_ERROR;
    }

    ਜੇ (tp->ਉਡੀਕ >= tp->max_ਕਤਾਰ) {
        (ਖਾਲੀ) ngx_thread_mutex_unlock(&tp->mtx, tp->ਲਘੂਗਣਕ);

        ngx_log_error(NGX_LOG_ERR, tp->ਲਘੂਗਣਕ, 0,
                      "thread pool \"%V\" queue overflow: %i tasks waiting",
                      &tp->name, tp->ਉਡੀਕ);
        ਵਾਪਸ NGX_ERROR;
    }

    task->event.ਸਰਗਰਮ = 1;

    task->id = ngx_thread_pool_task_id++;
    task->ਅਗਲਾ = ਨਿਰਾ;

    ਜੇ (ngx_thread_cond_signal(&tp->cond, tp->ਲਘੂਗਣਕ) != NGX_OK) {
        (ਖਾਲੀ) ngx_thread_mutex_unlock(&tp->mtx, tp->ਲਘੂਗਣਕ);
        ਵਾਪਸ NGX_ERROR;
    }

    *tp->ਕਤਾਰ.ਆਖਰੀ = task;
    tp->ਕਤਾਰ.ਆਖਰੀ = &task->ਅਗਲਾ;

    tp->ਉਡੀਕ++;

    (ਖਾਲੀ) ngx_thread_mutex_unlock(&tp->mtx, tp->ਲਘੂਗਣਕ);

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->ਲਘੂਗਣਕ, 0,
                   "task #%ui added to thread pool \"%V\"",
                   task->id, &tp->name);

    ਵਾਪਸ NGX_OK;
}


ਸਥਿਰ ਖਾਲੀ *
ngx_thread_pool_ਚੱਕਰ(ਖਾਲੀ *ਅੰਕੜੇ)
{
    ngx_thread_pool_t *tp = ਅੰਕੜੇ;

    ਪੂਰਨ                 err;
    sigset_t            ਸੈੱਟ;
    ngx_thread_task_t  *task;

#ਜੇ 0
    ngx_ਸਮਾਂ_update();
#ਜੇ_ਅੰਤ

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, tp->ਲਘੂਗਣਕ, 0,
                   "thread in pool \"%V\" started", &tp->name);

    sigfillset(&ਸੈੱਟ);

    sigdelset(&ਸੈੱਟ, SIGILL);
    sigdelset(&ਸੈੱਟ, SIGFPE);
    sigdelset(&ਸੈੱਟ, SIGSEGV);
    sigdelset(&ਸੈੱਟ, SIGBUS);

    err = pthread_sigmask(SIG_ਖੰਡ, &ਸੈੱਟ, ਨਿਰਾ);
    ਜੇ (err) {
        ngx_log_error(NGX_LOG_ALERT, tp->ਲਘੂਗਣਕ, err, "pthread_sigmask() failed");
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਲਈ ( ;; ) {
        ਜੇ (ngx_thread_mutex_ਤਾਲਾ(&tp->mtx, tp->ਲਘੂਗਣਕ) != NGX_OK) {
            ਵਾਪਸ ਨਿਰਾ;
        }

        /* the number may become negative */
        tp->ਉਡੀਕ--;

        ਜਦੋਂ (tp->ਕਤਾਰ.ਪਹਿਲਾ == ਨਿਰਾ) {
            ਜੇ (ngx_thread_cond_wait(&tp->cond, &tp->mtx, tp->ਲਘੂਗਣਕ)
                != NGX_OK)
            {
                (ਖਾਲੀ) ngx_thread_mutex_unlock(&tp->mtx, tp->ਲਘੂਗਣਕ);
                ਵਾਪਸ ਨਿਰਾ;
            }
        }

        task = tp->ਕਤਾਰ.ਪਹਿਲਾ;
        tp->ਕਤਾਰ.ਪਹਿਲਾ = task->ਅਗਲਾ;

        ਜੇ (tp->ਕਤਾਰ.ਪਹਿਲਾ == ਨਿਰਾ) {
            tp->ਕਤਾਰ.ਆਖਰੀ = &tp->ਕਤਾਰ.ਪਹਿਲਾ;
        }

        ਜੇ (ngx_thread_mutex_unlock(&tp->mtx, tp->ਲਘੂਗਣਕ) != NGX_OK) {
            ਵਾਪਸ ਨਿਰਾ;
        }

#ਜੇ 0
        ngx_ਸਮਾਂ_update();
#ਜੇ_ਅੰਤ

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->ਲਘੂਗਣਕ, 0,
                       "run task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->handler(task->ctx, tp->ਲਘੂਗਣਕ);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->ਲਘੂਗਣਕ, 0,
                       "complete task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->ਅਗਲਾ = ਨਿਰਾ;

        ngx_spinlock(&ngx_thread_pool_ਪੂਰਾ_ਤਾਲਾ, 1, 2048);

        *ngx_thread_pool_ਪੂਰਾ.ਆਖਰੀ = task;
        ngx_thread_pool_ਪੂਰਾ.ਆਖਰੀ = &task->ਅਗਲਾ;

        ngx_ਸਮ੍ਰਿਤੀ_barrier();

        ngx_unlock(&ngx_thread_pool_ਪੂਰਾ_ਤਾਲਾ);

        (ਖਾਲੀ) ngx_notify(ngx_thread_pool_handler);
    }
}


ਸਥਿਰ ਖਾਲੀ
ngx_thread_pool_handler(ngx_event_t *ev)
{
    ngx_event_t        *event;
    ngx_thread_task_t  *task;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ev->ਲਘੂਗਣਕ, 0, "thread pool handler");

    ngx_spinlock(&ngx_thread_pool_ਪੂਰਾ_ਤਾਲਾ, 1, 2048);

    task = ngx_thread_pool_ਪੂਰਾ.ਪਹਿਲਾ;
    ngx_thread_pool_ਪੂਰਾ.ਪਹਿਲਾ = ਨਿਰਾ;
    ngx_thread_pool_ਪੂਰਾ.ਆਖਰੀ = &ngx_thread_pool_ਪੂਰਾ.ਪਹਿਲਾ;

    ngx_ਸਮ੍ਰਿਤੀ_barrier();

    ngx_unlock(&ngx_thread_pool_ਪੂਰਾ_ਤਾਲਾ);

    ਜਦੋਂ (task) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->ਲਘੂਗਣਕ, 0,
                       "run completion handler for task #%ui", task->id);

        event = &task->event;
        task = task->ਅਗਲਾ;

        event->complete = 1;
        event->ਸਰਗਰਮ = 0;

        event->handler(event);
    }
}


ਸਥਿਰ ਖਾਲੀ *
ngx_thread_pool_create_conf(ngx_ਚੱਕਰ_t *ਚੱਕਰ)
{
    ngx_thread_pool_conf_t  *tcf;

    tcf = ngx_pcalloc(ਚੱਕਰ->pool, ਆਕਾਰ(ngx_thread_pool_conf_t));
    ਜੇ (tcf == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਜੇ (ngx_array_init(&tcf->pools, ਚੱਕਰ->pool, 4,
                       ਆਕਾਰ(ngx_thread_pool_t *))
        != NGX_OK)
    {
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਵਾਪਸ tcf;
}


ਸਥਿਰ ਅੱਖਰ *
ngx_thread_pool_init_conf(ngx_ਚੱਕਰ_t *ਚੱਕਰ, ਖਾਲੀ *conf)
{
    ngx_thread_pool_conf_t *tcf = conf;

    ngx_uint_t           i;
    ngx_thread_pool_t  **tpp;

    tpp = tcf->pools.elts;

    ਲਈ (i = 0; i < tcf->pools.nelts; i++) {

        ਜੇ (tpp[i]->ਧਾਗੇ) {
            ਜਾਰੀ;
        }

        ਜੇ (tpp[i]->name.len == ngx_thread_pool_default.len
            && ngx_strncmp(tpp[i]->name.ਅੰਕੜੇ, ngx_thread_pool_default.ਅੰਕੜੇ,
                           ngx_thread_pool_default.len)
               == 0)
        {
            tpp[i]->ਧਾਗੇ = 32;
            tpp[i]->max_ਕਤਾਰ = 65536;
            ਜਾਰੀ;
        }

        ngx_log_error(NGX_LOG_EMERG, ਚੱਕਰ->ਲਘੂਗਣਕ, 0,
                      "unknown thread pool \"%V\" in %s:%ui",
                      &tpp[i]->name, tpp[i]->file, tpp[i]->ਸਤਰ);

        ਵਾਪਸ NGX_CONF_ERROR;
    }

    ਵਾਪਸ NGX_CONF_OK;
}


ਸਥਿਰ ਅੱਖਰ *
ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf)
{
    ngx_str_t          *ਮੁੱਲ;
    ngx_uint_t          i;
    ngx_thread_pool_t  *tp;

    ਮੁੱਲ = cf->ਦਲੀਲਾਂ->elts;

    tp = ngx_thread_pool_ਜੋੜੋ(cf, &ਮੁੱਲ[1]);

    ਜੇ (tp == ਨਿਰਾ) {
        ਵਾਪਸ NGX_CONF_ERROR;
    }

    ਜੇ (tp->ਧਾਗੇ) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "duplicate thread pool \"%V\"", &tp->name);
        ਵਾਪਸ NGX_CONF_ERROR;
    }

    tp->max_ਕਤਾਰ = 65536;

    ਲਈ (i = 2; i < cf->ਦਲੀਲਾਂ->nelts; i++) {

        ਜੇ (ngx_strncmp(ਮੁੱਲ[i].ਅੰਕੜੇ, "threads=", 8) == 0) {

            tp->ਧਾਗੇ = ngx_atoi(ਮੁੱਲ[i].ਅੰਕੜੇ + 8, ਮੁੱਲ[i].len - 8);

            ਜੇ (tp->ਧਾਗੇ == (ngx_uint_t) NGX_ERROR || tp->ਧਾਗੇ == 0) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid threads value \"%V\"", &ਮੁੱਲ[i]);
                ਵਾਪਸ NGX_CONF_ERROR;
            }

            ਜਾਰੀ;
        }

        ਜੇ (ngx_strncmp(ਮੁੱਲ[i].ਅੰਕੜੇ, "max_queue=", 10) == 0) {

            tp->max_ਕਤਾਰ = ngx_atoi(ਮੁੱਲ[i].ਅੰਕੜੇ + 10, ਮੁੱਲ[i].len - 10);

            ਜੇ (tp->max_ਕਤਾਰ == NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid max_queue value \"%V\"", &ਮੁੱਲ[i]);
                ਵਾਪਸ NGX_CONF_ERROR;
            }

            ਜਾਰੀ;
        }
    }

    ਜੇ (tp->ਧਾਗੇ == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%V\" must have \"threads\" parameter",
                           &cmd->name);
        ਵਾਪਸ NGX_CONF_ERROR;
    }

    ਵਾਪਸ NGX_CONF_OK;
}


ngx_thread_pool_t *
ngx_thread_pool_ਜੋੜੋ(ngx_conf_t *cf, ngx_str_t *name)
{
    ngx_thread_pool_t       *tp, **tpp;
    ngx_thread_pool_conf_t  *tcf;

    ਜੇ (name == ਨਿਰਾ) {
        name = &ngx_thread_pool_default;
    }

    tp = ngx_thread_pool_get(cf->ਚੱਕਰ, name);

    ਜੇ (tp) {
        ਵਾਪਸ tp;
    }

    tp = ngx_pcalloc(cf->pool, ਆਕਾਰ(ngx_thread_pool_t));
    ਜੇ (tp == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    tp->name = *name;
    tp->file = cf->conf_file->file.name.ਅੰਕੜੇ;
    tp->ਸਤਰ = cf->conf_file->ਸਤਰ;

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cf->ਚੱਕਰ->conf_ctx,
                                                  ngx_thread_pool_module);

    tpp = ngx_array_ਧੱਕੋ(&tcf->pools);
    ਜੇ (tpp == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    *tpp = tp;

    ਵਾਪਸ tp;
}


ngx_thread_pool_t *
ngx_thread_pool_get(ngx_ਚੱਕਰ_t *ਚੱਕਰ, ngx_str_t *name)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(ਚੱਕਰ->conf_ctx,
                                                  ngx_thread_pool_module);

    tpp = tcf->pools.elts;

    ਲਈ (i = 0; i < tcf->pools.nelts; i++) {

        ਜੇ (tpp[i]->name.len == name->len
            && ngx_strncmp(tpp[i]->name.ਅੰਕੜੇ, name->ਅੰਕੜੇ, name->len) == 0)
        {
            ਵਾਪਸ tpp[i];
        }
    }

    ਵਾਪਸ ਨਿਰਾ;
}


ਸਥਿਰ ngx_int_t
ngx_thread_pool_init_ਕਾਮਾ(ngx_ਚੱਕਰ_t *ਚੱਕਰ)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    ਜੇ (ngx_process != NGX_PROCESS_ਕਾਮਾ
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        ਵਾਪਸ NGX_OK;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(ਚੱਕਰ->conf_ctx,
                                                  ngx_thread_pool_module);

    ਜੇ (tcf == ਨਿਰਾ) {
        ਵਾਪਸ NGX_OK;
    }

    ngx_thread_pool_ਕਤਾਰ_init(&ngx_thread_pool_ਪੂਰਾ);

    tpp = tcf->pools.elts;

    ਲਈ (i = 0; i < tcf->pools.nelts; i++) {
        ਜੇ (ngx_thread_pool_init(tpp[i], ਚੱਕਰ->ਲਘੂਗਣਕ, ਚੱਕਰ->pool) != NGX_OK) {
            ਵਾਪਸ NGX_ERROR;
        }
    }

    ਵਾਪਸ NGX_OK;
}


ਸਥਿਰ ਖਾਲੀ
ngx_thread_pool_exit_ਕਾਮਾ(ngx_ਚੱਕਰ_t *ਚੱਕਰ)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    ਜੇ (ngx_process != NGX_PROCESS_ਕਾਮਾ
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        ਵਾਪਸ;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(ਚੱਕਰ->conf_ctx,
                                                  ngx_thread_pool_module);

    ਜੇ (tcf == ਨਿਰਾ) {
        ਵਾਪਸ;
    }

    tpp = tcf->pools.elts;

    ਲਈ (i = 0; i < tcf->pools.nelts; i++) {
        ngx_thread_pool_destroy(tpp[i]);
    }
}
