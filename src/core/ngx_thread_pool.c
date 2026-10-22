
/*
 * Copyright (C) Nginx, Inc.
 * Copyright (C) Valentin V. Bartenev
 * Copyright (C) Ruslan Ermilov
 */


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>
#அந்தர்பாவ <ngx_thread_pool.h>


வகைவரை கட்டமைப்பு {
    ngx_array_t               pools;
} ngx_thread_pool_conf_t;


வகைவரை கட்டமைப்பு {
    ngx_thread_task_t        *first;
    ngx_thread_task_t       **last;
} ngx_thread_pool_queue_t;

#define ngx_thread_pool_queue_init(q)                                         \
    (q)->first = வெற்று;                                                        \
    (q)->last = &(q)->first


கட்டமைப்பு ngx_thread_pool_s {
    ngx_thread_mutex_t        mtx;
    ngx_thread_pool_queue_t   queue;
    ngx_int_t                 waiting;
    ngx_thread_cond_t         cond;

    ngx_log_t                *log;

    ngx_str_t                 name;
    ngx_uint_t                threads;
    ngx_int_t                 max_queue;

    u_char                   *file;
    ngx_uint_t                வரி;
};


நிலையான ngx_int_t ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *log,
    ngx_pool_t *pool);
நிலையான வெறுமை ngx_thread_pool_destroy(ngx_thread_pool_t *tp);
நிலையான வெறுமை ngx_thread_pool_exit_handler(வெறுமை *data, ngx_log_t *log);

நிலையான வெறுமை *ngx_thread_pool_cycle(வெறுமை *data);
நிலையான வெறுமை ngx_thread_pool_handler(ngx_event_t *ev);

நிலையான எழுத்து *ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf);

நிலையான வெறுமை *ngx_thread_pool_create_conf(ngx_cycle_t *cycle);
நிலையான எழுத்து *ngx_thread_pool_init_conf(ngx_cycle_t *cycle, வெறுமை *conf);

நிலையான ngx_int_t ngx_thread_pool_init_worker(ngx_cycle_t *cycle);
நிலையான வெறுமை ngx_thread_pool_exit_worker(ngx_cycle_t *cycle);


நிலையான ngx_command_t  ngx_thread_pool_commands[] = {

    { ngx_string("thread_pool"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE23,
      ngx_thread_pool,
      0,
      0,
      வெற்று },

      ngx_null_command
};


நிலையான ngx_core_module_t  ngx_thread_pool_module_ctx = {
    ngx_string("thread_pool"),
    ngx_thread_pool_create_conf,
    ngx_thread_pool_init_conf
};


ngx_module_t  ngx_thread_pool_module = {
    NGX_MODULE_V1,
    &ngx_thread_pool_module_ctx,           /* module context */
    ngx_thread_pool_commands,              /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    வெற்று,                                  /* init master */
    வெற்று,                                  /* init module */
    ngx_thread_pool_init_worker,           /* init process */
    வெற்று,                                  /* init thread */
    வெற்று,                                  /* exit thread */
    ngx_thread_pool_exit_worker,           /* exit process */
    வெற்று,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


நிலையான ngx_str_t  ngx_thread_pool_default = ngx_string("default");

நிலையான ngx_uint_t               ngx_thread_pool_task_id;
நிலையான ngx_atomic_t             ngx_thread_pool_done_lock;
நிலையான ngx_thread_pool_queue_t  ngx_thread_pool_done;


நிலையான ngx_int_t
ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *log, ngx_pool_t *pool)
{
    முழு             err;
    pthread_t       tid;
    ngx_uint_t      n;
    pthread_attr_t  attr;

    எனில் (ngx_notify == வெற்று) {
        ngx_log_error(NGX_LOG_ALERT, log, 0,
               "the configured event method cannot be used with thread pools");
        திரும்பு NGX_ERROR;
    }

    ngx_thread_pool_queue_init(&tp->queue);

    எனில் (ngx_thread_mutex_create(&tp->mtx, log) != NGX_OK) {
        திரும்பு NGX_ERROR;
    }

    எனில் (ngx_thread_cond_create(&tp->cond, log) != NGX_OK) {
        (வெறுமை) ngx_thread_mutex_destroy(&tp->mtx, log);
        திரும்பு NGX_ERROR;
    }

    tp->log = log;

    err = pthread_attr_init(&attr);
    எனில் (err) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_attr_init() failed");
        திரும்பு NGX_ERROR;
    }

    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    எனில் (err) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_attr_setdetachstate() failed");
        திரும்பு NGX_ERROR;
    }

#எனில் 0
    err = pthread_attr_setstacksize(&attr, PTHREAD_அடுக்கு_MIN);
    எனில் (err) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_attr_setstacksize() failed");
        திரும்பு NGX_ERROR;
    }
#endif

    க்கு (n = 0; n < tp->threads; n++) {
        err = pthread_create(&tid, &attr, ngx_thread_pool_cycle, tp);
        எனில் (err) {
            ngx_log_error(NGX_LOG_ALERT, log, err,
                          "pthread_create() failed");
            திரும்பு NGX_ERROR;
        }
    }

    (வெறுமை) pthread_attr_destroy(&attr);

    திரும்பு NGX_OK;
}


நிலையான வெறுமை
ngx_thread_pool_destroy(ngx_thread_pool_t *tp)
{
    ngx_uint_t           n;
    ngx_thread_task_t    task;
    நிலையற்ற ngx_uint_t  lock;

    ngx_memzero(&task, அளவு(ngx_thread_task_t));

    task.handler = ngx_thread_pool_exit_handler;
    task.ctx = (வெறுமை *) &lock;

    க்கு (n = 0; n < tp->threads; n++) {
        lock = 1;

        எனில் (ngx_thread_task_post(tp, &task) != NGX_OK) {
            திரும்பு;
        }

        போது (lock) {
            ngx_sched_yield();
        }

        task.event.active = 0;
    }

    (வெறுமை) ngx_thread_cond_destroy(&tp->cond, tp->log);

    (வெறுமை) ngx_thread_mutex_destroy(&tp->mtx, tp->log);
}


நிலையான வெறுமை
ngx_thread_pool_exit_handler(வெறுமை *data, ngx_log_t *log)
{
    ngx_uint_t *lock = data;

    *lock = 0;

    pthread_exit(வெற்று);
}


ngx_thread_task_t *
ngx_thread_task_alloc(ngx_pool_t *pool, size_t size)
{
    ngx_thread_task_t  *task;

    task = ngx_pcalloc(pool, அளவு(ngx_thread_task_t) + size);
    எனில் (task == வெற்று) {
        திரும்பு வெற்று;
    }

    task->ctx = task + 1;

    திரும்பு task;
}


ngx_int_t
ngx_thread_task_post(ngx_thread_pool_t *tp, ngx_thread_task_t *task)
{
    எனில் (task->event.active) {
        ngx_log_error(NGX_LOG_ALERT, tp->log, 0,
                      "task #%ui already active", task->id);
        திரும்பு NGX_ERROR;
    }

    எனில் (ngx_thread_mutex_lock(&tp->mtx, tp->log) != NGX_OK) {
        திரும்பு NGX_ERROR;
    }

    எனில் (tp->waiting >= tp->max_queue) {
        (வெறுமை) ngx_thread_mutex_unlock(&tp->mtx, tp->log);

        ngx_log_error(NGX_LOG_ERR, tp->log, 0,
                      "thread pool \"%V\" queue overflow: %i tasks waiting",
                      &tp->name, tp->waiting);
        திரும்பு NGX_ERROR;
    }

    task->event.active = 1;

    task->id = ngx_thread_pool_task_id++;
    task->next = வெற்று;

    எனில் (ngx_thread_cond_signal(&tp->cond, tp->log) != NGX_OK) {
        (வெறுமை) ngx_thread_mutex_unlock(&tp->mtx, tp->log);
        திரும்பு NGX_ERROR;
    }

    *tp->queue.last = task;
    tp->queue.last = &task->next;

    tp->waiting++;

    (வெறுமை) ngx_thread_mutex_unlock(&tp->mtx, tp->log);

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->log, 0,
                   "task #%ui added to thread pool \"%V\"",
                   task->id, &tp->name);

    திரும்பு NGX_OK;
}


நிலையான வெறுமை *
ngx_thread_pool_cycle(வெறுமை *data)
{
    ngx_thread_pool_t *tp = data;

    முழு                 err;
    sigset_t            set;
    ngx_thread_task_t  *task;

#எனில் 0
    ngx_time_update();
#endif

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, tp->log, 0,
                   "thread in pool \"%V\" started", &tp->name);

    sigfillset(&set);

    sigdelset(&set, SIGILL);
    sigdelset(&set, SIGFPE);
    sigdelset(&set, SIGSEGV);
    sigdelset(&set, SIGBUS);

    err = pthread_sigmask(SIG_BLOCK, &set, வெற்று);
    எனில் (err) {
        ngx_log_error(NGX_LOG_ALERT, tp->log, err, "pthread_sigmask() failed");
        திரும்பு வெற்று;
    }

    க்கு ( ;; ) {
        எனில் (ngx_thread_mutex_lock(&tp->mtx, tp->log) != NGX_OK) {
            திரும்பு வெற்று;
        }

        /* the number may become negative */
        tp->waiting--;

        போது (tp->queue.first == வெற்று) {
            எனில் (ngx_thread_cond_wait(&tp->cond, &tp->mtx, tp->log)
                != NGX_OK)
            {
                (வெறுமை) ngx_thread_mutex_unlock(&tp->mtx, tp->log);
                திரும்பு வெற்று;
            }
        }

        task = tp->queue.first;
        tp->queue.first = task->next;

        எனில் (tp->queue.first == வெற்று) {
            tp->queue.last = &tp->queue.first;
        }

        எனில் (ngx_thread_mutex_unlock(&tp->mtx, tp->log) != NGX_OK) {
            திரும்பு வெற்று;
        }

#எனில் 0
        ngx_time_update();
#endif

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->log, 0,
                       "run task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->handler(task->ctx, tp->log);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->log, 0,
                       "complete task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->next = வெற்று;

        ngx_spinlock(&ngx_thread_pool_done_lock, 1, 2048);

        *ngx_thread_pool_done.last = task;
        ngx_thread_pool_done.last = &task->next;

        ngx_memory_barrier();

        ngx_unlock(&ngx_thread_pool_done_lock);

        (வெறுமை) ngx_notify(ngx_thread_pool_handler);
    }
}


நிலையான வெறுமை
ngx_thread_pool_handler(ngx_event_t *ev)
{
    ngx_event_t        *event;
    ngx_thread_task_t  *task;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ev->log, 0, "thread pool handler");

    ngx_spinlock(&ngx_thread_pool_done_lock, 1, 2048);

    task = ngx_thread_pool_done.first;
    ngx_thread_pool_done.first = வெற்று;
    ngx_thread_pool_done.last = &ngx_thread_pool_done.first;

    ngx_memory_barrier();

    ngx_unlock(&ngx_thread_pool_done_lock);

    போது (task) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->log, 0,
                       "run completion handler for task #%ui", task->id);

        event = &task->event;
        task = task->next;

        event->complete = 1;
        event->active = 0;

        event->handler(event);
    }
}


நிலையான வெறுமை *
ngx_thread_pool_create_conf(ngx_cycle_t *cycle)
{
    ngx_thread_pool_conf_t  *tcf;

    tcf = ngx_pcalloc(cycle->pool, அளவு(ngx_thread_pool_conf_t));
    எனில் (tcf == வெற்று) {
        திரும்பு வெற்று;
    }

    எனில் (ngx_array_init(&tcf->pools, cycle->pool, 4,
                       அளவு(ngx_thread_pool_t *))
        != NGX_OK)
    {
        திரும்பு வெற்று;
    }

    திரும்பு tcf;
}


நிலையான எழுத்து *
ngx_thread_pool_init_conf(ngx_cycle_t *cycle, வெறுமை *conf)
{
    ngx_thread_pool_conf_t *tcf = conf;

    ngx_uint_t           i;
    ngx_thread_pool_t  **tpp;

    tpp = tcf->pools.elts;

    க்கு (i = 0; i < tcf->pools.nelts; i++) {

        எனில் (tpp[i]->threads) {
            தொடர்;
        }

        எனில் (tpp[i]->name.len == ngx_thread_pool_default.len
            && ngx_strncmp(tpp[i]->name.data, ngx_thread_pool_default.data,
                           ngx_thread_pool_default.len)
               == 0)
        {
            tpp[i]->threads = 32;
            tpp[i]->max_queue = 65536;
            தொடர்;
        }

        ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                      "unknown thread pool \"%V\" in %s:%ui",
                      &tpp[i]->name, tpp[i]->file, tpp[i]->வரி);

        திரும்பு NGX_CONF_ERROR;
    }

    திரும்பு NGX_CONF_OK;
}


நிலையான எழுத்து *
ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf)
{
    ngx_str_t          *மதிப்பு;
    ngx_uint_t          i;
    ngx_thread_pool_t  *tp;

    மதிப்பு = cf->args->elts;

    tp = ngx_thread_pool_add(cf, &மதிப்பு[1]);

    எனில் (tp == வெற்று) {
        திரும்பு NGX_CONF_ERROR;
    }

    எனில் (tp->threads) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "duplicate thread pool \"%V\"", &tp->name);
        திரும்பு NGX_CONF_ERROR;
    }

    tp->max_queue = 65536;

    க்கு (i = 2; i < cf->args->nelts; i++) {

        எனில் (ngx_strncmp(மதிப்பு[i].data, "threads=", 8) == 0) {

            tp->threads = ngx_atoi(மதிப்பு[i].data + 8, மதிப்பு[i].len - 8);

            எனில் (tp->threads == (ngx_uint_t) NGX_ERROR || tp->threads == 0) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid threads value \"%V\"", &மதிப்பு[i]);
                திரும்பு NGX_CONF_ERROR;
            }

            தொடர்;
        }

        எனில் (ngx_strncmp(மதிப்பு[i].data, "max_queue=", 10) == 0) {

            tp->max_queue = ngx_atoi(மதிப்பு[i].data + 10, மதிப்பு[i].len - 10);

            எனில் (tp->max_queue == NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid max_queue value \"%V\"", &மதிப்பு[i]);
                திரும்பு NGX_CONF_ERROR;
            }

            தொடர்;
        }
    }

    எனில் (tp->threads == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%V\" must have \"threads\" parameter",
                           &cmd->name);
        திரும்பு NGX_CONF_ERROR;
    }

    திரும்பு NGX_CONF_OK;
}


ngx_thread_pool_t *
ngx_thread_pool_add(ngx_conf_t *cf, ngx_str_t *name)
{
    ngx_thread_pool_t       *tp, **tpp;
    ngx_thread_pool_conf_t  *tcf;

    எனில் (name == வெற்று) {
        name = &ngx_thread_pool_default;
    }

    tp = ngx_thread_pool_get(cf->cycle, name);

    எனில் (tp) {
        திரும்பு tp;
    }

    tp = ngx_pcalloc(cf->pool, அளவு(ngx_thread_pool_t));
    எனில் (tp == வெற்று) {
        திரும்பு வெற்று;
    }

    tp->name = *name;
    tp->file = cf->conf_file->file.name.data;
    tp->வரி = cf->conf_file->வரி;

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cf->cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    tpp = ngx_array_தள்ளு(&tcf->pools);
    எனில் (tpp == வெற்று) {
        திரும்பு வெற்று;
    }

    *tpp = tp;

    திரும்பு tp;
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

    க்கு (i = 0; i < tcf->pools.nelts; i++) {

        எனில் (tpp[i]->name.len == name->len
            && ngx_strncmp(tpp[i]->name.data, name->data, name->len) == 0)
        {
            திரும்பு tpp[i];
        }
    }

    திரும்பு வெற்று;
}


நிலையான ngx_int_t
ngx_thread_pool_init_worker(ngx_cycle_t *cycle)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    எனில் (ngx_process != NGX_PROCESS_WORKER
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        திரும்பு NGX_OK;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    எனில் (tcf == வெற்று) {
        திரும்பு NGX_OK;
    }

    ngx_thread_pool_queue_init(&ngx_thread_pool_done);

    tpp = tcf->pools.elts;

    க்கு (i = 0; i < tcf->pools.nelts; i++) {
        எனில் (ngx_thread_pool_init(tpp[i], cycle->log, cycle->pool) != NGX_OK) {
            திரும்பு NGX_ERROR;
        }
    }

    திரும்பு NGX_OK;
}


நிலையான வெறுமை
ngx_thread_pool_exit_worker(ngx_cycle_t *cycle)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    எனில் (ngx_process != NGX_PROCESS_WORKER
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        திரும்பு;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    எனில் (tcf == வெற்று) {
        திரும்பு;
    }

    tpp = tcf->pools.elts;

    க்கு (i = 0; i < tcf->pools.nelts; i++) {
        ngx_thread_pool_destroy(tpp[i]);
    }
}
