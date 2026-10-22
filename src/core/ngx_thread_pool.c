
/*
 * Copyright (C) Nginx, Inc.
 * Copyright (C) Valentin V. Bartenev
 * Copyright (C) Ruslan Ermilov
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>
#ଅନ୍ତର୍ଭାଵ <ngx_thread_pool.h>


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ngx_array_t               pools;
} ngx_thread_pool_conf_t;


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ngx_thread_task_t        *first;
    ngx_thread_task_t       **last;
} ngx_thread_pool_queue_t;

#define ngx_thread_pool_queue_init(q)                                         \
    (q)->first = ଖାଲି;                                                        \
    (q)->last = &(q)->first


ସଂରଚନା ngx_thread_pool_s {
    ngx_thread_mutex_t        mtx;
    ngx_thread_pool_queue_t   queue;
    ngx_int_t                 waiting;
    ngx_thread_cond_t         cond;

    ngx_log_t                *log;

    ngx_str_t                 name;
    ngx_uint_t                threads;
    ngx_int_t                 max_queue;

    u_char                   *file;
    ngx_uint_t                ଧାଡ଼ି;
};


ସ୍ଥିର ngx_int_t ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *log,
    ngx_pool_t *pool);
ସ୍ଥିର ଶୂନ୍ୟ ngx_thread_pool_destroy(ngx_thread_pool_t *tp);
ସ୍ଥିର ଶୂନ୍ୟ ngx_thread_pool_exit_handler(ଶୂନ୍ୟ *data, ngx_log_t *log);

ସ୍ଥିର ଶୂନ୍ୟ *ngx_thread_pool_cycle(ଶୂନ୍ୟ *data);
ସ୍ଥିର ଶୂନ୍ୟ ngx_thread_pool_handler(ngx_event_t *ev);

ସ୍ଥିର ଅକ୍ଷର *ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf);

ସ୍ଥିର ଶୂନ୍ୟ *ngx_thread_pool_create_conf(ngx_cycle_t *cycle);
ସ୍ଥିର ଅକ୍ଷର *ngx_thread_pool_init_conf(ngx_cycle_t *cycle, ଶୂନ୍ୟ *conf);

ସ୍ଥିର ngx_int_t ngx_thread_pool_init_worker(ngx_cycle_t *cycle);
ସ୍ଥିର ଶୂନ୍ୟ ngx_thread_pool_exit_worker(ngx_cycle_t *cycle);


ସ୍ଥିର ngx_command_t  ngx_thread_pool_commands[] = {

    { ngx_string("thread_pool"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE23,
      ngx_thread_pool,
      0,
      0,
      ଖାଲି },

      ngx_null_command
};


ସ୍ଥିର ngx_core_module_t  ngx_thread_pool_module_ctx = {
    ngx_string("thread_pool"),
    ngx_thread_pool_create_conf,
    ngx_thread_pool_init_conf
};


ngx_module_t  ngx_thread_pool_module = {
    NGX_MODULE_V1,
    &ngx_thread_pool_module_ctx,           /* module context */
    ngx_thread_pool_commands,              /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    ଖାଲି,                                  /* init master */
    ଖାଲି,                                  /* init module */
    ngx_thread_pool_init_worker,           /* init process */
    ଖାଲି,                                  /* init thread */
    ଖାଲି,                                  /* exit thread */
    ngx_thread_pool_exit_worker,           /* exit process */
    ଖାଲି,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


ସ୍ଥିର ngx_str_t  ngx_thread_pool_default = ngx_string("default");

ସ୍ଥିର ngx_uint_t               ngx_thread_pool_task_id;
ସ୍ଥିର ngx_atomic_t             ngx_thread_pool_done_lock;
ସ୍ଥିର ngx_thread_pool_queue_t  ngx_thread_pool_done;


ସ୍ଥିର ngx_int_t
ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *log, ngx_pool_t *pool)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା             err;
    pthread_t       tid;
    ngx_uint_t      n;
    pthread_attr_t  attr;

    ଯଦି (ngx_notify == ଖାଲି) {
        ngx_log_error(NGX_LOG_ALERT, log, 0,
               "the configured event method cannot be used with thread pools");
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ngx_thread_pool_queue_init(&tp->queue);

    ଯଦି (ngx_thread_mutex_create(&tp->mtx, log) != NGX_OK) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ଯଦି (ngx_thread_cond_create(&tp->cond, log) != NGX_OK) {
        (ଶୂନ୍ୟ) ngx_thread_mutex_destroy(&tp->mtx, log);
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    tp->log = log;

    err = pthread_attr_init(&attr);
    ଯଦି (err) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_attr_init() failed");
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    ଯଦି (err) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_attr_setdetachstate() failed");
        ଫେରନ୍ତୁ NGX_ERROR;
    }

#ଯଦି 0
    err = pthread_attr_setstacksize(&attr, PTHREAD_ଗଦା_MIN);
    ଯଦି (err) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_attr_setstacksize() failed");
        ଫେରନ୍ତୁ NGX_ERROR;
    }
#endif

    ପାଇଁ (n = 0; n < tp->threads; n++) {
        err = pthread_create(&tid, &attr, ngx_thread_pool_cycle, tp);
        ଯଦି (err) {
            ngx_log_error(NGX_LOG_ALERT, log, err,
                          "pthread_create() failed");
            ଫେରନ୍ତୁ NGX_ERROR;
        }
    }

    (ଶୂନ୍ୟ) pthread_attr_destroy(&attr);

    ଫେରନ୍ତୁ NGX_OK;
}


ସ୍ଥିର ଶୂନ୍ୟ
ngx_thread_pool_destroy(ngx_thread_pool_t *tp)
{
    ngx_uint_t           n;
    ngx_thread_task_t    task;
    ଅସ୍ଥିର ngx_uint_t  lock;

    ngx_memzero(&task, ଆକାର(ngx_thread_task_t));

    task.handler = ngx_thread_pool_exit_handler;
    task.ctx = (ଶୂନ୍ୟ *) &lock;

    ପାଇଁ (n = 0; n < tp->threads; n++) {
        lock = 1;

        ଯଦି (ngx_thread_task_post(tp, &task) != NGX_OK) {
            ଫେରନ୍ତୁ;
        }

        ଯେତେବେଳେ (lock) {
            ngx_sched_yield();
        }

        task.event.active = 0;
    }

    (ଶୂନ୍ୟ) ngx_thread_cond_destroy(&tp->cond, tp->log);

    (ଶୂନ୍ୟ) ngx_thread_mutex_destroy(&tp->mtx, tp->log);
}


ସ୍ଥିର ଶୂନ୍ୟ
ngx_thread_pool_exit_handler(ଶୂନ୍ୟ *data, ngx_log_t *log)
{
    ngx_uint_t *lock = data;

    *lock = 0;

    pthread_exit(ଖାଲି);
}


ngx_thread_task_t *
ngx_thread_task_alloc(ngx_pool_t *pool, size_t size)
{
    ngx_thread_task_t  *task;

    task = ngx_pcalloc(pool, ଆକାର(ngx_thread_task_t) + size);
    ଯଦି (task == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    task->ctx = task + 1;

    ଫେରନ୍ତୁ task;
}


ngx_int_t
ngx_thread_task_post(ngx_thread_pool_t *tp, ngx_thread_task_t *task)
{
    ଯଦି (task->event.active) {
        ngx_log_error(NGX_LOG_ALERT, tp->log, 0,
                      "task #%ui already active", task->id);
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ଯଦି (ngx_thread_mutex_lock(&tp->mtx, tp->log) != NGX_OK) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ଯଦି (tp->waiting >= tp->max_queue) {
        (ଶୂନ୍ୟ) ngx_thread_mutex_unlock(&tp->mtx, tp->log);

        ngx_log_error(NGX_LOG_ERR, tp->log, 0,
                      "thread pool \"%V\" queue overflow: %i tasks waiting",
                      &tp->name, tp->waiting);
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    task->event.active = 1;

    task->id = ngx_thread_pool_task_id++;
    task->next = ଖାଲି;

    ଯଦି (ngx_thread_cond_signal(&tp->cond, tp->log) != NGX_OK) {
        (ଶୂନ୍ୟ) ngx_thread_mutex_unlock(&tp->mtx, tp->log);
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    *tp->queue.last = task;
    tp->queue.last = &task->next;

    tp->waiting++;

    (ଶୂନ୍ୟ) ngx_thread_mutex_unlock(&tp->mtx, tp->log);

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->log, 0,
                   "task #%ui added to thread pool \"%V\"",
                   task->id, &tp->name);

    ଫେରନ୍ତୁ NGX_OK;
}


ସ୍ଥିର ଶୂନ୍ୟ *
ngx_thread_pool_cycle(ଶୂନ୍ୟ *data)
{
    ngx_thread_pool_t *tp = data;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା                 err;
    sigset_t            set;
    ngx_thread_task_t  *task;

#ଯଦି 0
    ngx_time_update();
#endif

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, tp->log, 0,
                   "thread in pool \"%V\" started", &tp->name);

    sigfillset(&set);

    sigdelset(&set, SIGILL);
    sigdelset(&set, SIGFPE);
    sigdelset(&set, SIGSEGV);
    sigdelset(&set, SIGBUS);

    err = pthread_sigmask(SIG_BLOCK, &set, ଖାଲି);
    ଯଦି (err) {
        ngx_log_error(NGX_LOG_ALERT, tp->log, err, "pthread_sigmask() failed");
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ପାଇଁ ( ;; ) {
        ଯଦି (ngx_thread_mutex_lock(&tp->mtx, tp->log) != NGX_OK) {
            ଫେରନ୍ତୁ ଖାଲି;
        }

        /* the number may become negative */
        tp->waiting--;

        ଯେତେବେଳେ (tp->queue.first == ଖାଲି) {
            ଯଦି (ngx_thread_cond_wait(&tp->cond, &tp->mtx, tp->log)
                != NGX_OK)
            {
                (ଶୂନ୍ୟ) ngx_thread_mutex_unlock(&tp->mtx, tp->log);
                ଫେରନ୍ତୁ ଖାଲି;
            }
        }

        task = tp->queue.first;
        tp->queue.first = task->next;

        ଯଦି (tp->queue.first == ଖାଲି) {
            tp->queue.last = &tp->queue.first;
        }

        ଯଦି (ngx_thread_mutex_unlock(&tp->mtx, tp->log) != NGX_OK) {
            ଫେରନ୍ତୁ ଖାଲି;
        }

#ଯଦି 0
        ngx_time_update();
#endif

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->log, 0,
                       "run task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->handler(task->ctx, tp->log);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->log, 0,
                       "complete task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->next = ଖାଲି;

        ngx_spinlock(&ngx_thread_pool_done_lock, 1, 2048);

        *ngx_thread_pool_done.last = task;
        ngx_thread_pool_done.last = &task->next;

        ngx_memory_barrier();

        ngx_unlock(&ngx_thread_pool_done_lock);

        (ଶୂନ୍ୟ) ngx_notify(ngx_thread_pool_handler);
    }
}


ସ୍ଥିର ଶୂନ୍ୟ
ngx_thread_pool_handler(ngx_event_t *ev)
{
    ngx_event_t        *event;
    ngx_thread_task_t  *task;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ev->log, 0, "thread pool handler");

    ngx_spinlock(&ngx_thread_pool_done_lock, 1, 2048);

    task = ngx_thread_pool_done.first;
    ngx_thread_pool_done.first = ଖାଲି;
    ngx_thread_pool_done.last = &ngx_thread_pool_done.first;

    ngx_memory_barrier();

    ngx_unlock(&ngx_thread_pool_done_lock);

    ଯେତେବେଳେ (task) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->log, 0,
                       "run completion handler for task #%ui", task->id);

        event = &task->event;
        task = task->next;

        event->complete = 1;
        event->active = 0;

        event->handler(event);
    }
}


ସ୍ଥିର ଶୂନ୍ୟ *
ngx_thread_pool_create_conf(ngx_cycle_t *cycle)
{
    ngx_thread_pool_conf_t  *tcf;

    tcf = ngx_pcalloc(cycle->pool, ଆକାର(ngx_thread_pool_conf_t));
    ଯଦି (tcf == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଯଦି (ngx_array_init(&tcf->pools, cycle->pool, 4,
                       ଆକାର(ngx_thread_pool_t *))
        != NGX_OK)
    {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଫେରନ୍ତୁ tcf;
}


ସ୍ଥିର ଅକ୍ଷର *
ngx_thread_pool_init_conf(ngx_cycle_t *cycle, ଶୂନ୍ୟ *conf)
{
    ngx_thread_pool_conf_t *tcf = conf;

    ngx_uint_t           i;
    ngx_thread_pool_t  **tpp;

    tpp = tcf->pools.elts;

    ପାଇଁ (i = 0; i < tcf->pools.nelts; i++) {

        ଯଦି (tpp[i]->threads) {
            ଜାରି;
        }

        ଯଦି (tpp[i]->name.len == ngx_thread_pool_default.len
            && ngx_strncmp(tpp[i]->name.data, ngx_thread_pool_default.data,
                           ngx_thread_pool_default.len)
               == 0)
        {
            tpp[i]->threads = 32;
            tpp[i]->max_queue = 65536;
            ଜାରି;
        }

        ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                      "unknown thread pool \"%V\" in %s:%ui",
                      &tpp[i]->name, tpp[i]->file, tpp[i]->ଧାଡ଼ି);

        ଫେରନ୍ତୁ NGX_CONF_ERROR;
    }

    ଫେରନ୍ତୁ NGX_CONF_OK;
}


ସ୍ଥିର ଅକ୍ଷର *
ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf)
{
    ngx_str_t          *ମୂଲ୍ୟ;
    ngx_uint_t          i;
    ngx_thread_pool_t  *tp;

    ମୂଲ୍ୟ = cf->args->elts;

    tp = ngx_thread_pool_add(cf, &ମୂଲ୍ୟ[1]);

    ଯଦି (tp == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_CONF_ERROR;
    }

    ଯଦି (tp->threads) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "duplicate thread pool \"%V\"", &tp->name);
        ଫେରନ୍ତୁ NGX_CONF_ERROR;
    }

    tp->max_queue = 65536;

    ପାଇଁ (i = 2; i < cf->args->nelts; i++) {

        ଯଦି (ngx_strncmp(ମୂଲ୍ୟ[i].data, "threads=", 8) == 0) {

            tp->threads = ngx_atoi(ମୂଲ୍ୟ[i].data + 8, ମୂଲ୍ୟ[i].len - 8);

            ଯଦି (tp->threads == (ngx_uint_t) NGX_ERROR || tp->threads == 0) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid threads value \"%V\"", &ମୂଲ୍ୟ[i]);
                ଫେରନ୍ତୁ NGX_CONF_ERROR;
            }

            ଜାରି;
        }

        ଯଦି (ngx_strncmp(ମୂଲ୍ୟ[i].data, "max_queue=", 10) == 0) {

            tp->max_queue = ngx_atoi(ମୂଲ୍ୟ[i].data + 10, ମୂଲ୍ୟ[i].len - 10);

            ଯଦି (tp->max_queue == NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid max_queue value \"%V\"", &ମୂଲ୍ୟ[i]);
                ଫେରନ୍ତୁ NGX_CONF_ERROR;
            }

            ଜାରି;
        }
    }

    ଯଦି (tp->threads == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%V\" must have \"threads\" parameter",
                           &cmd->name);
        ଫେରନ୍ତୁ NGX_CONF_ERROR;
    }

    ଫେରନ୍ତୁ NGX_CONF_OK;
}


ngx_thread_pool_t *
ngx_thread_pool_add(ngx_conf_t *cf, ngx_str_t *name)
{
    ngx_thread_pool_t       *tp, **tpp;
    ngx_thread_pool_conf_t  *tcf;

    ଯଦି (name == ଖାଲି) {
        name = &ngx_thread_pool_default;
    }

    tp = ngx_thread_pool_get(cf->cycle, name);

    ଯଦି (tp) {
        ଫେରନ୍ତୁ tp;
    }

    tp = ngx_pcalloc(cf->pool, ଆକାର(ngx_thread_pool_t));
    ଯଦି (tp == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    tp->name = *name;
    tp->file = cf->conf_file->file.name.data;
    tp->ଧାଡ଼ି = cf->conf_file->ଧାଡ଼ି;

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cf->cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    tpp = ngx_array_ଠେଲ(&tcf->pools);
    ଯଦି (tpp == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    *tpp = tp;

    ଫେରନ୍ତୁ tp;
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

    ପାଇଁ (i = 0; i < tcf->pools.nelts; i++) {

        ଯଦି (tpp[i]->name.len == name->len
            && ngx_strncmp(tpp[i]->name.data, name->data, name->len) == 0)
        {
            ଫେରନ୍ତୁ tpp[i];
        }
    }

    ଫେରନ୍ତୁ ଖାଲି;
}


ସ୍ଥିର ngx_int_t
ngx_thread_pool_init_worker(ngx_cycle_t *cycle)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    ଯଦି (ngx_process != NGX_PROCESS_WORKER
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        ଫେରନ୍ତୁ NGX_OK;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    ଯଦି (tcf == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_OK;
    }

    ngx_thread_pool_queue_init(&ngx_thread_pool_done);

    tpp = tcf->pools.elts;

    ପାଇଁ (i = 0; i < tcf->pools.nelts; i++) {
        ଯଦି (ngx_thread_pool_init(tpp[i], cycle->log, cycle->pool) != NGX_OK) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }
    }

    ଫେରନ୍ତୁ NGX_OK;
}


ସ୍ଥିର ଶୂନ୍ୟ
ngx_thread_pool_exit_worker(ngx_cycle_t *cycle)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    ଯଦି (ngx_process != NGX_PROCESS_WORKER
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        ଫେରନ୍ତୁ;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    ଯଦି (tcf == ଖାଲି) {
        ଫେରନ୍ତୁ;
    }

    tpp = tcf->pools.elts;

    ପାଇଁ (i = 0; i < tcf->pools.nelts; i++) {
        ngx_thread_pool_destroy(tpp[i]);
    }
}
