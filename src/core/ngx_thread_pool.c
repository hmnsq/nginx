
/*
 * Copyright (C) Nginx, Inc.
 * Copyright (C) Valentin V. Bartenev
 * Copyright (C) Ruslan Ermilov
 */


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>
#അന്തര്ഭാവ <ngx_thread_pool.h>


തരനിർവചനം ഘടന {
    ngx_array_t               pools;
} ngx_thread_pool_conf_t;


തരനിർവചനം ഘടന {
    ngx_thread_task_t        *first;
    ngx_thread_task_t       **last;
} ngx_thread_pool_queue_t;

#define ngx_thread_pool_queue_init(q)                                         \
    (q)->first = ശൂന്യ;                                                        \
    (q)->last = &(q)->first


ഘടന ngx_thread_pool_s {
    ngx_thread_mutex_t        mtx;
    ngx_thread_pool_queue_t   queue;
    ngx_int_t                 waiting;
    ngx_thread_cond_t         cond;

    ngx_log_t                *log;

    ngx_str_t                 name;
    ngx_uint_t                threads;
    ngx_int_t                 max_queue;

    u_char                   *file;
    ngx_uint_t                വരി;
};


സ്ഥിതം ngx_int_t ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *log,
    ngx_pool_t *pool);
സ്ഥിതം ശൂന്യം ngx_thread_pool_destroy(ngx_thread_pool_t *tp);
സ്ഥിതം ശൂന്യം ngx_thread_pool_exit_handler(ശൂന്യം *data, ngx_log_t *log);

സ്ഥിതം ശൂന്യം *ngx_thread_pool_cycle(ശൂന്യം *data);
സ്ഥിതം ശൂന്യം ngx_thread_pool_handler(ngx_event_t *ev);

സ്ഥിതം അക്ഷരം *ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf);

സ്ഥിതം ശൂന്യം *ngx_thread_pool_create_conf(ngx_cycle_t *cycle);
സ്ഥിതം അക്ഷരം *ngx_thread_pool_init_conf(ngx_cycle_t *cycle, ശൂന്യം *conf);

സ്ഥിതം ngx_int_t ngx_thread_pool_init_worker(ngx_cycle_t *cycle);
സ്ഥിതം ശൂന്യം ngx_thread_pool_exit_worker(ngx_cycle_t *cycle);


സ്ഥിതം ngx_command_t  ngx_thread_pool_commands[] = {

    { ngx_string("thread_pool"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE23,
      ngx_thread_pool,
      0,
      0,
      ശൂന്യ },

      ngx_null_command
};


സ്ഥിതം ngx_core_module_t  ngx_thread_pool_module_ctx = {
    ngx_string("thread_pool"),
    ngx_thread_pool_create_conf,
    ngx_thread_pool_init_conf
};


ngx_module_t  ngx_thread_pool_module = {
    NGX_MODULE_V1,
    &ngx_thread_pool_module_ctx,           /* module context */
    ngx_thread_pool_commands,              /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    ശൂന്യ,                                  /* init master */
    ശൂന്യ,                                  /* init module */
    ngx_thread_pool_init_worker,           /* init process */
    ശൂന്യ,                                  /* init thread */
    ശൂന്യ,                                  /* exit thread */
    ngx_thread_pool_exit_worker,           /* exit process */
    ശൂന്യ,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


സ്ഥിതം ngx_str_t  ngx_thread_pool_default = ngx_string("default");

സ്ഥിതം ngx_uint_t               ngx_thread_pool_task_id;
സ്ഥിതം ngx_atomic_t             ngx_thread_pool_done_lock;
സ്ഥിതം ngx_thread_pool_queue_t  ngx_thread_pool_done;


സ്ഥിതം ngx_int_t
ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *log, ngx_pool_t *pool)
{
    പൂർണ്ണസംഖ്യ             err;
    pthread_t       tid;
    ngx_uint_t      n;
    pthread_attr_t  attr;

    എങ്കിൽ (ngx_notify == ശൂന്യ) {
        ngx_log_error(NGX_LOG_ALERT, log, 0,
               "the configured event method cannot be used with thread pools");
        തിരികെ NGX_ERROR;
    }

    ngx_thread_pool_queue_init(&tp->queue);

    എങ്കിൽ (ngx_thread_mutex_create(&tp->mtx, log) != NGX_OK) {
        തിരികെ NGX_ERROR;
    }

    എങ്കിൽ (ngx_thread_cond_create(&tp->cond, log) != NGX_OK) {
        (ശൂന്യം) ngx_thread_mutex_destroy(&tp->mtx, log);
        തിരികെ NGX_ERROR;
    }

    tp->log = log;

    err = pthread_attr_init(&attr);
    എങ്കിൽ (err) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_attr_init() failed");
        തിരികെ NGX_ERROR;
    }

    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    എങ്കിൽ (err) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_attr_setdetachstate() failed");
        തിരികെ NGX_ERROR;
    }

#എങ്കിൽ 0
    err = pthread_attr_setstacksize(&attr, PTHREAD_അട്ടി_MIN);
    എങ്കിൽ (err) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_attr_setstacksize() failed");
        തിരികെ NGX_ERROR;
    }
#endif

    വേണ്ടി (n = 0; n < tp->threads; n++) {
        err = pthread_create(&tid, &attr, ngx_thread_pool_cycle, tp);
        എങ്കിൽ (err) {
            ngx_log_error(NGX_LOG_ALERT, log, err,
                          "pthread_create() failed");
            തിരികെ NGX_ERROR;
        }
    }

    (ശൂന്യം) pthread_attr_destroy(&attr);

    തിരികെ NGX_OK;
}


സ്ഥിതം ശൂന്യം
ngx_thread_pool_destroy(ngx_thread_pool_t *tp)
{
    ngx_uint_t           n;
    ngx_thread_task_t    task;
    അസ്ഥിര ngx_uint_t  lock;

    ngx_memzero(&task, വലുപ്പം(ngx_thread_task_t));

    task.handler = ngx_thread_pool_exit_handler;
    task.ctx = (ശൂന്യം *) &lock;

    വേണ്ടി (n = 0; n < tp->threads; n++) {
        lock = 1;

        എങ്കിൽ (ngx_thread_task_post(tp, &task) != NGX_OK) {
            തിരികെ;
        }

        ആകുമ്പോൾ (lock) {
            ngx_sched_yield();
        }

        task.event.active = 0;
    }

    (ശൂന്യം) ngx_thread_cond_destroy(&tp->cond, tp->log);

    (ശൂന്യം) ngx_thread_mutex_destroy(&tp->mtx, tp->log);
}


സ്ഥിതം ശൂന്യം
ngx_thread_pool_exit_handler(ശൂന്യം *data, ngx_log_t *log)
{
    ngx_uint_t *lock = data;

    *lock = 0;

    pthread_exit(ശൂന്യ);
}


ngx_thread_task_t *
ngx_thread_task_alloc(ngx_pool_t *pool, size_t size)
{
    ngx_thread_task_t  *task;

    task = ngx_pcalloc(pool, വലുപ്പം(ngx_thread_task_t) + size);
    എങ്കിൽ (task == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    task->ctx = task + 1;

    തിരികെ task;
}


ngx_int_t
ngx_thread_task_post(ngx_thread_pool_t *tp, ngx_thread_task_t *task)
{
    എങ്കിൽ (task->event.active) {
        ngx_log_error(NGX_LOG_ALERT, tp->log, 0,
                      "task #%ui already active", task->id);
        തിരികെ NGX_ERROR;
    }

    എങ്കിൽ (ngx_thread_mutex_lock(&tp->mtx, tp->log) != NGX_OK) {
        തിരികെ NGX_ERROR;
    }

    എങ്കിൽ (tp->waiting >= tp->max_queue) {
        (ശൂന്യം) ngx_thread_mutex_unlock(&tp->mtx, tp->log);

        ngx_log_error(NGX_LOG_ERR, tp->log, 0,
                      "thread pool \"%V\" queue overflow: %i tasks waiting",
                      &tp->name, tp->waiting);
        തിരികെ NGX_ERROR;
    }

    task->event.active = 1;

    task->id = ngx_thread_pool_task_id++;
    task->next = ശൂന്യ;

    എങ്കിൽ (ngx_thread_cond_signal(&tp->cond, tp->log) != NGX_OK) {
        (ശൂന്യം) ngx_thread_mutex_unlock(&tp->mtx, tp->log);
        തിരികെ NGX_ERROR;
    }

    *tp->queue.last = task;
    tp->queue.last = &task->next;

    tp->waiting++;

    (ശൂന്യം) ngx_thread_mutex_unlock(&tp->mtx, tp->log);

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->log, 0,
                   "task #%ui added to thread pool \"%V\"",
                   task->id, &tp->name);

    തിരികെ NGX_OK;
}


സ്ഥിതം ശൂന്യം *
ngx_thread_pool_cycle(ശൂന്യം *data)
{
    ngx_thread_pool_t *tp = data;

    പൂർണ്ണസംഖ്യ                 err;
    sigset_t            set;
    ngx_thread_task_t  *task;

#എങ്കിൽ 0
    ngx_time_update();
#endif

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, tp->log, 0,
                   "thread in pool \"%V\" started", &tp->name);

    sigfillset(&set);

    sigdelset(&set, SIGILL);
    sigdelset(&set, SIGFPE);
    sigdelset(&set, SIGSEGV);
    sigdelset(&set, SIGBUS);

    err = pthread_sigmask(SIG_BLOCK, &set, ശൂന്യ);
    എങ്കിൽ (err) {
        ngx_log_error(NGX_LOG_ALERT, tp->log, err, "pthread_sigmask() failed");
        തിരികെ ശൂന്യ;
    }

    വേണ്ടി ( ;; ) {
        എങ്കിൽ (ngx_thread_mutex_lock(&tp->mtx, tp->log) != NGX_OK) {
            തിരികെ ശൂന്യ;
        }

        /* the number may become negative */
        tp->waiting--;

        ആകുമ്പോൾ (tp->queue.first == ശൂന്യ) {
            എങ്കിൽ (ngx_thread_cond_wait(&tp->cond, &tp->mtx, tp->log)
                != NGX_OK)
            {
                (ശൂന്യം) ngx_thread_mutex_unlock(&tp->mtx, tp->log);
                തിരികെ ശൂന്യ;
            }
        }

        task = tp->queue.first;
        tp->queue.first = task->next;

        എങ്കിൽ (tp->queue.first == ശൂന്യ) {
            tp->queue.last = &tp->queue.first;
        }

        എങ്കിൽ (ngx_thread_mutex_unlock(&tp->mtx, tp->log) != NGX_OK) {
            തിരികെ ശൂന്യ;
        }

#എങ്കിൽ 0
        ngx_time_update();
#endif

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->log, 0,
                       "run task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->handler(task->ctx, tp->log);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->log, 0,
                       "complete task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->next = ശൂന്യ;

        ngx_spinlock(&ngx_thread_pool_done_lock, 1, 2048);

        *ngx_thread_pool_done.last = task;
        ngx_thread_pool_done.last = &task->next;

        ngx_memory_barrier();

        ngx_unlock(&ngx_thread_pool_done_lock);

        (ശൂന്യം) ngx_notify(ngx_thread_pool_handler);
    }
}


സ്ഥിതം ശൂന്യം
ngx_thread_pool_handler(ngx_event_t *ev)
{
    ngx_event_t        *event;
    ngx_thread_task_t  *task;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ev->log, 0, "thread pool handler");

    ngx_spinlock(&ngx_thread_pool_done_lock, 1, 2048);

    task = ngx_thread_pool_done.first;
    ngx_thread_pool_done.first = ശൂന്യ;
    ngx_thread_pool_done.last = &ngx_thread_pool_done.first;

    ngx_memory_barrier();

    ngx_unlock(&ngx_thread_pool_done_lock);

    ആകുമ്പോൾ (task) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->log, 0,
                       "run completion handler for task #%ui", task->id);

        event = &task->event;
        task = task->next;

        event->complete = 1;
        event->active = 0;

        event->handler(event);
    }
}


സ്ഥിതം ശൂന്യം *
ngx_thread_pool_create_conf(ngx_cycle_t *cycle)
{
    ngx_thread_pool_conf_t  *tcf;

    tcf = ngx_pcalloc(cycle->pool, വലുപ്പം(ngx_thread_pool_conf_t));
    എങ്കിൽ (tcf == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    എങ്കിൽ (ngx_array_init(&tcf->pools, cycle->pool, 4,
                       വലുപ്പം(ngx_thread_pool_t *))
        != NGX_OK)
    {
        തിരികെ ശൂന്യ;
    }

    തിരികെ tcf;
}


സ്ഥിതം അക്ഷരം *
ngx_thread_pool_init_conf(ngx_cycle_t *cycle, ശൂന്യം *conf)
{
    ngx_thread_pool_conf_t *tcf = conf;

    ngx_uint_t           i;
    ngx_thread_pool_t  **tpp;

    tpp = tcf->pools.elts;

    വേണ്ടി (i = 0; i < tcf->pools.nelts; i++) {

        എങ്കിൽ (tpp[i]->threads) {
            തുടരുക;
        }

        എങ്കിൽ (tpp[i]->name.len == ngx_thread_pool_default.len
            && ngx_strncmp(tpp[i]->name.data, ngx_thread_pool_default.data,
                           ngx_thread_pool_default.len)
               == 0)
        {
            tpp[i]->threads = 32;
            tpp[i]->max_queue = 65536;
            തുടരുക;
        }

        ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                      "unknown thread pool \"%V\" in %s:%ui",
                      &tpp[i]->name, tpp[i]->file, tpp[i]->വരി);

        തിരികെ NGX_CONF_ERROR;
    }

    തിരികെ NGX_CONF_OK;
}


സ്ഥിതം അക്ഷരം *
ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf)
{
    ngx_str_t          *മൂല്യം;
    ngx_uint_t          i;
    ngx_thread_pool_t  *tp;

    മൂല്യം = cf->args->elts;

    tp = ngx_thread_pool_add(cf, &മൂല്യം[1]);

    എങ്കിൽ (tp == ശൂന്യ) {
        തിരികെ NGX_CONF_ERROR;
    }

    എങ്കിൽ (tp->threads) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "duplicate thread pool \"%V\"", &tp->name);
        തിരികെ NGX_CONF_ERROR;
    }

    tp->max_queue = 65536;

    വേണ്ടി (i = 2; i < cf->args->nelts; i++) {

        എങ്കിൽ (ngx_strncmp(മൂല്യം[i].data, "threads=", 8) == 0) {

            tp->threads = ngx_atoi(മൂല്യം[i].data + 8, മൂല്യം[i].len - 8);

            എങ്കിൽ (tp->threads == (ngx_uint_t) NGX_ERROR || tp->threads == 0) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid threads value \"%V\"", &മൂല്യം[i]);
                തിരികെ NGX_CONF_ERROR;
            }

            തുടരുക;
        }

        എങ്കിൽ (ngx_strncmp(മൂല്യം[i].data, "max_queue=", 10) == 0) {

            tp->max_queue = ngx_atoi(മൂല്യം[i].data + 10, മൂല്യം[i].len - 10);

            എങ്കിൽ (tp->max_queue == NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid max_queue value \"%V\"", &മൂല്യം[i]);
                തിരികെ NGX_CONF_ERROR;
            }

            തുടരുക;
        }
    }

    എങ്കിൽ (tp->threads == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%V\" must have \"threads\" parameter",
                           &cmd->name);
        തിരികെ NGX_CONF_ERROR;
    }

    തിരികെ NGX_CONF_OK;
}


ngx_thread_pool_t *
ngx_thread_pool_add(ngx_conf_t *cf, ngx_str_t *name)
{
    ngx_thread_pool_t       *tp, **tpp;
    ngx_thread_pool_conf_t  *tcf;

    എങ്കിൽ (name == ശൂന്യ) {
        name = &ngx_thread_pool_default;
    }

    tp = ngx_thread_pool_get(cf->cycle, name);

    എങ്കിൽ (tp) {
        തിരികെ tp;
    }

    tp = ngx_pcalloc(cf->pool, വലുപ്പം(ngx_thread_pool_t));
    എങ്കിൽ (tp == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    tp->name = *name;
    tp->file = cf->conf_file->file.name.data;
    tp->വരി = cf->conf_file->വരി;

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cf->cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    tpp = ngx_array_തള്ളുക(&tcf->pools);
    എങ്കിൽ (tpp == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    *tpp = tp;

    തിരികെ tp;
}


ngx_thread_pool_t *
ngx_thread_pool_get(ngx_cycle_t *cycle, ngx_str_t *name)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    tpp = tcf->pools.elts;

    വേണ്ടി (i = 0; i < tcf->pools.nelts; i++) {

        എങ്കിൽ (tpp[i]->name.len == name->len
            && ngx_strncmp(tpp[i]->name.data, name->data, name->len) == 0)
        {
            തിരികെ tpp[i];
        }
    }

    തിരികെ ശൂന്യ;
}


സ്ഥിതം ngx_int_t
ngx_thread_pool_init_worker(ngx_cycle_t *cycle)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    എങ്കിൽ (ngx_process != NGX_PROCESS_WORKER
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        തിരികെ NGX_OK;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    എങ്കിൽ (tcf == ശൂന്യ) {
        തിരികെ NGX_OK;
    }

    ngx_thread_pool_queue_init(&ngx_thread_pool_done);

    tpp = tcf->pools.elts;

    വേണ്ടി (i = 0; i < tcf->pools.nelts; i++) {
        എങ്കിൽ (ngx_thread_pool_init(tpp[i], cycle->log, cycle->pool) != NGX_OK) {
            തിരികെ NGX_ERROR;
        }
    }

    തിരികെ NGX_OK;
}


സ്ഥിതം ശൂന്യം
ngx_thread_pool_exit_worker(ngx_cycle_t *cycle)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    എങ്കിൽ (ngx_process != NGX_PROCESS_WORKER
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        തിരികെ;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    എങ്കിൽ (tcf == ശൂന്യ) {
        തിരികെ;
    }

    tpp = tcf->pools.elts;

    വേണ്ടി (i = 0; i < tcf->pools.nelts; i++) {
        ngx_thread_pool_destroy(tpp[i]);
    }
}
