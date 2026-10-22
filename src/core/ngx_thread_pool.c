
/*
 * Copyright (C) Nginx, Inc.
 * Copyright (C) Valentin V. Bartenev
 * Copyright (C) Ruslan Ermilov
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>
#ಅನ್ತರ್ಭಾವ <ngx_thread_pool.h>


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ngx_array_t               pools;
} ngx_thread_pool_conf_t;


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ngx_thread_task_t        *first;
    ngx_thread_task_t       **last;
} ngx_thread_pool_queue_t;

#define ngx_thread_pool_queue_init(q)                                         \
    (q)->first = ರಿಕ್ತ;                                                        \
    (q)->last = &(q)->first


ಸಂರಚನಾ ngx_thread_pool_s {
    ngx_thread_mutex_t        mtx;
    ngx_thread_pool_queue_t   queue;
    ngx_int_t                 waiting;
    ngx_thread_cond_t         cond;

    ngx_log_t                *log;

    ngx_str_t                 name;
    ngx_uint_t                threads;
    ngx_int_t                 max_queue;

    u_char                   *file;
    ngx_uint_t                ಸಾಲು;
};


ಸ್ಥಿರ ngx_int_t ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *log,
    ngx_pool_t *pool);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_thread_pool_destroy(ngx_thread_pool_t *tp);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_thread_pool_exit_handler(ಶೂನ್ಯ *data, ngx_log_t *log);

ಸ್ಥಿರ ಶೂನ್ಯ *ngx_thread_pool_cycle(ಶೂನ್ಯ *data);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_thread_pool_handler(ngx_event_t *ev);

ಸ್ಥಿರ ವರ್ಣ *ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);

ಸ್ಥಿರ ಶೂನ್ಯ *ngx_thread_pool_create_conf(ngx_cycle_t *cycle);
ಸ್ಥಿರ ವರ್ಣ *ngx_thread_pool_init_conf(ngx_cycle_t *cycle, ಶೂನ್ಯ *conf);

ಸ್ಥಿರ ngx_int_t ngx_thread_pool_init_worker(ngx_cycle_t *cycle);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_thread_pool_exit_worker(ngx_cycle_t *cycle);


ಸ್ಥಿರ ngx_command_t  ngx_thread_pool_commands[] = {

    { ngx_string("thread_pool"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE23,
      ngx_thread_pool,
      0,
      0,
      ರಿಕ್ತ },

      ngx_null_command
};


ಸ್ಥಿರ ngx_core_module_t  ngx_thread_pool_module_ctx = {
    ngx_string("thread_pool"),
    ngx_thread_pool_create_conf,
    ngx_thread_pool_init_conf
};


ngx_module_t  ngx_thread_pool_module = {
    NGX_MODULE_V1,
    &ngx_thread_pool_module_ctx,           /* module context */
    ngx_thread_pool_commands,              /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    ರಿಕ್ತ,                                  /* init master */
    ರಿಕ್ತ,                                  /* init module */
    ngx_thread_pool_init_worker,           /* init process */
    ರಿಕ್ತ,                                  /* init thread */
    ರಿಕ್ತ,                                  /* exit thread */
    ngx_thread_pool_exit_worker,           /* exit process */
    ರಿಕ್ತ,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


ಸ್ಥಿರ ngx_str_t  ngx_thread_pool_default = ngx_string("default");

ಸ್ಥಿರ ngx_uint_t               ngx_thread_pool_task_id;
ಸ್ಥಿರ ngx_atomic_t             ngx_thread_pool_done_lock;
ಸ್ಥಿರ ngx_thread_pool_queue_t  ngx_thread_pool_done;


ಸ್ಥಿರ ngx_int_t
ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *log, ngx_pool_t *pool)
{
    ಪೂರ್ಣಾಙ್ಕ             err;
    pthread_t       tid;
    ngx_uint_t      n;
    pthread_attr_t  attr;

    ಯದಿ (ngx_notify == ರಿಕ್ತ) {
        ngx_log_error(NGX_LOG_ALERT, log, 0,
               "the configured event method cannot be used with thread pools");
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ngx_thread_pool_queue_init(&tp->queue);

    ಯದಿ (ngx_thread_mutex_create(&tp->mtx, log) != NGX_OK) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಯದಿ (ngx_thread_cond_create(&tp->cond, log) != NGX_OK) {
        (ಶೂನ್ಯ) ngx_thread_mutex_destroy(&tp->mtx, log);
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    tp->log = log;

    err = pthread_attr_init(&attr);
    ಯದಿ (err) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_attr_init() failed");
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    ಯದಿ (err) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_attr_setdetachstate() failed");
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

#ಯದಿ 0
    err = pthread_attr_setstacksize(&attr, PTHREAD_ರಾಶಿ_MIN);
    ಯದಿ (err) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_attr_setstacksize() failed");
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }
#endif

    ಪ್ರತಿ (n = 0; n < tp->threads; n++) {
        err = pthread_create(&tid, &attr, ngx_thread_pool_cycle, tp);
        ಯದಿ (err) {
            ngx_log_error(NGX_LOG_ALERT, log, err,
                          "pthread_create() failed");
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }
    }

    (ಶೂನ್ಯ) pthread_attr_destroy(&attr);

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_thread_pool_destroy(ngx_thread_pool_t *tp)
{
    ngx_uint_t           n;
    ngx_thread_task_t    task;
    ಚಞ್ಚಲ ngx_uint_t  lock;

    ngx_memzero(&task, ಪರಿಮಾಣ(ngx_thread_task_t));

    task.handler = ngx_thread_pool_exit_handler;
    task.ctx = (ಶೂನ್ಯ *) &lock;

    ಪ್ರತಿ (n = 0; n < tp->threads; n++) {
        lock = 1;

        ಯದಿ (ngx_thread_task_post(tp, &task) != NGX_OK) {
            ಪ್ರತ್ಯಾಗಮ;
        }

        ಯಾವತ್ (lock) {
            ngx_sched_yield();
        }

        task.event.active = 0;
    }

    (ಶೂನ್ಯ) ngx_thread_cond_destroy(&tp->cond, tp->log);

    (ಶೂನ್ಯ) ngx_thread_mutex_destroy(&tp->mtx, tp->log);
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_thread_pool_exit_handler(ಶೂನ್ಯ *data, ngx_log_t *log)
{
    ngx_uint_t *lock = data;

    *lock = 0;

    pthread_exit(ರಿಕ್ತ);
}


ngx_thread_task_t *
ngx_thread_task_alloc(ngx_pool_t *pool, size_t size)
{
    ngx_thread_task_t  *task;

    task = ngx_pcalloc(pool, ಪರಿಮಾಣ(ngx_thread_task_t) + size);
    ಯದಿ (task == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    task->ctx = task + 1;

    ಪ್ರತ್ಯಾಗಮ task;
}


ngx_int_t
ngx_thread_task_post(ngx_thread_pool_t *tp, ngx_thread_task_t *task)
{
    ಯದಿ (task->event.active) {
        ngx_log_error(NGX_LOG_ALERT, tp->log, 0,
                      "task #%ui already active", task->id);
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಯದಿ (ngx_thread_mutex_lock(&tp->mtx, tp->log) != NGX_OK) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಯದಿ (tp->waiting >= tp->max_queue) {
        (ಶೂನ್ಯ) ngx_thread_mutex_unlock(&tp->mtx, tp->log);

        ngx_log_error(NGX_LOG_ERR, tp->log, 0,
                      "thread pool \"%V\" queue overflow: %i tasks waiting",
                      &tp->name, tp->waiting);
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    task->event.active = 1;

    task->id = ngx_thread_pool_task_id++;
    task->next = ರಿಕ್ತ;

    ಯದಿ (ngx_thread_cond_signal(&tp->cond, tp->log) != NGX_OK) {
        (ಶೂನ್ಯ) ngx_thread_mutex_unlock(&tp->mtx, tp->log);
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    *tp->queue.last = task;
    tp->queue.last = &task->next;

    tp->waiting++;

    (ಶೂನ್ಯ) ngx_thread_mutex_unlock(&tp->mtx, tp->log);

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->log, 0,
                   "task #%ui added to thread pool \"%V\"",
                   task->id, &tp->name);

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ಸ್ಥಿರ ಶೂನ್ಯ *
ngx_thread_pool_cycle(ಶೂನ್ಯ *data)
{
    ngx_thread_pool_t *tp = data;

    ಪೂರ್ಣಾಙ್ಕ                 err;
    sigset_t            set;
    ngx_thread_task_t  *task;

#ಯದಿ 0
    ngx_time_update();
#endif

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, tp->log, 0,
                   "thread in pool \"%V\" started", &tp->name);

    sigfillset(&set);

    sigdelset(&set, SIGILL);
    sigdelset(&set, SIGFPE);
    sigdelset(&set, SIGSEGV);
    sigdelset(&set, SIGBUS);

    err = pthread_sigmask(SIG_BLOCK, &set, ರಿಕ್ತ);
    ಯದಿ (err) {
        ngx_log_error(NGX_LOG_ALERT, tp->log, err, "pthread_sigmask() failed");
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಪ್ರತಿ ( ;; ) {
        ಯದಿ (ngx_thread_mutex_lock(&tp->mtx, tp->log) != NGX_OK) {
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        }

        /* the number may become negative */
        tp->waiting--;

        ಯಾವತ್ (tp->queue.first == ರಿಕ್ತ) {
            ಯದಿ (ngx_thread_cond_wait(&tp->cond, &tp->mtx, tp->log)
                != NGX_OK)
            {
                (ಶೂನ್ಯ) ngx_thread_mutex_unlock(&tp->mtx, tp->log);
                ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
            }
        }

        task = tp->queue.first;
        tp->queue.first = task->next;

        ಯದಿ (tp->queue.first == ರಿಕ್ತ) {
            tp->queue.last = &tp->queue.first;
        }

        ಯದಿ (ngx_thread_mutex_unlock(&tp->mtx, tp->log) != NGX_OK) {
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        }

#ಯದಿ 0
        ngx_time_update();
#endif

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->log, 0,
                       "run task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->handler(task->ctx, tp->log);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->log, 0,
                       "complete task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->next = ರಿಕ್ತ;

        ngx_spinlock(&ngx_thread_pool_done_lock, 1, 2048);

        *ngx_thread_pool_done.last = task;
        ngx_thread_pool_done.last = &task->next;

        ngx_memory_barrier();

        ngx_unlock(&ngx_thread_pool_done_lock);

        (ಶೂನ್ಯ) ngx_notify(ngx_thread_pool_handler);
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_thread_pool_handler(ngx_event_t *ev)
{
    ngx_event_t        *event;
    ngx_thread_task_t  *task;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ev->log, 0, "thread pool handler");

    ngx_spinlock(&ngx_thread_pool_done_lock, 1, 2048);

    task = ngx_thread_pool_done.first;
    ngx_thread_pool_done.first = ರಿಕ್ತ;
    ngx_thread_pool_done.last = &ngx_thread_pool_done.first;

    ngx_memory_barrier();

    ngx_unlock(&ngx_thread_pool_done_lock);

    ಯಾವತ್ (task) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->log, 0,
                       "run completion handler for task #%ui", task->id);

        event = &task->event;
        task = task->next;

        event->complete = 1;
        event->active = 0;

        event->handler(event);
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ *
ngx_thread_pool_create_conf(ngx_cycle_t *cycle)
{
    ngx_thread_pool_conf_t  *tcf;

    tcf = ngx_pcalloc(cycle->pool, ಪರಿಮಾಣ(ngx_thread_pool_conf_t));
    ಯದಿ (tcf == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಯದಿ (ngx_array_init(&tcf->pools, cycle->pool, 4,
                       ಪರಿಮಾಣ(ngx_thread_pool_t *))
        != NGX_OK)
    {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಪ್ರತ್ಯಾಗಮ tcf;
}


ಸ್ಥಿರ ವರ್ಣ *
ngx_thread_pool_init_conf(ngx_cycle_t *cycle, ಶೂನ್ಯ *conf)
{
    ngx_thread_pool_conf_t *tcf = conf;

    ngx_uint_t           i;
    ngx_thread_pool_t  **tpp;

    tpp = tcf->pools.elts;

    ಪ್ರತಿ (i = 0; i < tcf->pools.nelts; i++) {

        ಯದಿ (tpp[i]->threads) {
            ಅನುವರ್ತ;
        }

        ಯದಿ (tpp[i]->name.len == ngx_thread_pool_default.len
            && ngx_strncmp(tpp[i]->name.data, ngx_thread_pool_default.data,
                           ngx_thread_pool_default.len)
               == 0)
        {
            tpp[i]->threads = 32;
            tpp[i]->max_queue = 65536;
            ಅನುವರ್ತ;
        }

        ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                      "unknown thread pool \"%V\" in %s:%ui",
                      &tpp[i]->name, tpp[i]->file, tpp[i]->ಸಾಲು);

        ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
    }

    ಪ್ರತ್ಯಾಗಮ NGX_CONF_OK;
}


ಸ್ಥಿರ ವರ್ಣ *
ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ngx_str_t          *ಮೌಲ್ಯ;
    ngx_uint_t          i;
    ngx_thread_pool_t  *tp;

    ಮೌಲ್ಯ = cf->args->elts;

    tp = ngx_thread_pool_add(cf, &ಮೌಲ್ಯ[1]);

    ಯದಿ (tp == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
    }

    ಯದಿ (tp->threads) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "duplicate thread pool \"%V\"", &tp->name);
        ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
    }

    tp->max_queue = 65536;

    ಪ್ರತಿ (i = 2; i < cf->args->nelts; i++) {

        ಯದಿ (ngx_strncmp(ಮೌಲ್ಯ[i].data, "threads=", 8) == 0) {

            tp->threads = ngx_atoi(ಮೌಲ್ಯ[i].data + 8, ಮೌಲ್ಯ[i].len - 8);

            ಯದಿ (tp->threads == (ngx_uint_t) NGX_ERROR || tp->threads == 0) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid threads value \"%V\"", &ಮೌಲ್ಯ[i]);
                ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
            }

            ಅನುವರ್ತ;
        }

        ಯದಿ (ngx_strncmp(ಮೌಲ್ಯ[i].data, "max_queue=", 10) == 0) {

            tp->max_queue = ngx_atoi(ಮೌಲ್ಯ[i].data + 10, ಮೌಲ್ಯ[i].len - 10);

            ಯದಿ (tp->max_queue == NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid max_queue value \"%V\"", &ಮೌಲ್ಯ[i]);
                ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
            }

            ಅನುವರ್ತ;
        }
    }

    ಯದಿ (tp->threads == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%V\" must have \"threads\" parameter",
                           &cmd->name);
        ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
    }

    ಪ್ರತ್ಯಾಗಮ NGX_CONF_OK;
}


ngx_thread_pool_t *
ngx_thread_pool_add(ngx_conf_t *cf, ngx_str_t *name)
{
    ngx_thread_pool_t       *tp, **tpp;
    ngx_thread_pool_conf_t  *tcf;

    ಯದಿ (name == ರಿಕ್ತ) {
        name = &ngx_thread_pool_default;
    }

    tp = ngx_thread_pool_get(cf->cycle, name);

    ಯದಿ (tp) {
        ಪ್ರತ್ಯಾಗಮ tp;
    }

    tp = ngx_pcalloc(cf->pool, ಪರಿಮಾಣ(ngx_thread_pool_t));
    ಯದಿ (tp == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    tp->name = *name;
    tp->file = cf->conf_file->file.name.data;
    tp->ಸಾಲು = cf->conf_file->ಸಾಲು;

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cf->cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    tpp = ngx_array_ತಳ್ಳು(&tcf->pools);
    ಯದಿ (tpp == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    *tpp = tp;

    ಪ್ರತ್ಯಾಗಮ tp;
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

    ಪ್ರತಿ (i = 0; i < tcf->pools.nelts; i++) {

        ಯದಿ (tpp[i]->name.len == name->len
            && ngx_strncmp(tpp[i]->name.data, name->data, name->len) == 0)
        {
            ಪ್ರತ್ಯಾಗಮ tpp[i];
        }
    }

    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}


ಸ್ಥಿರ ngx_int_t
ngx_thread_pool_init_worker(ngx_cycle_t *cycle)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    ಯದಿ (ngx_process != NGX_PROCESS_WORKER
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    ಯದಿ (tcf == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    ngx_thread_pool_queue_init(&ngx_thread_pool_done);

    tpp = tcf->pools.elts;

    ಪ್ರತಿ (i = 0; i < tcf->pools.nelts; i++) {
        ಯದಿ (ngx_thread_pool_init(tpp[i], cycle->log, cycle->pool) != NGX_OK) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }
    }

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_thread_pool_exit_worker(ngx_cycle_t *cycle)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    ಯದಿ (ngx_process != NGX_PROCESS_WORKER
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        ಪ್ರತ್ಯಾಗಮ;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    ಯದಿ (tcf == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ;
    }

    tpp = tcf->pools.elts;

    ಪ್ರತಿ (i = 0; i < tcf->pools.nelts; i++) {
        ngx_thread_pool_destroy(tpp[i]);
    }
}
