
/*
 * Copyright (C) Nginx, Inc.
 * Copyright (C) Valentin V. Bartenev
 * Copyright (C) Ruslan Ermilov
 */


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>
#અન્તર્ભાવ <ngx_thread_pool.h>


પ્રકારવ્યાખ્યા સંરચના {
    ngx_array_t               pools;
} ngx_thread_pool_conf_t;


પ્રકારવ્યાખ્યા સંરચના {
    ngx_thread_task_t        *first;
    ngx_thread_task_t       **last;
} ngx_thread_pool_queue_t;

#define ngx_thread_pool_queue_init(q)                                         \
    (q)->first = ખાલી;                                                        \
    (q)->last = &(q)->first


સંરચના ngx_thread_pool_s {
    ngx_thread_mutex_t        mtx;
    ngx_thread_pool_queue_t   queue;
    ngx_int_t                 waiting;
    ngx_thread_cond_t         cond;

    ngx_log_t                *log;

    ngx_str_t                 name;
    ngx_uint_t                threads;
    ngx_int_t                 max_queue;

    u_char                   *file;
    ngx_uint_t                લીટી;
};


સ્થિર ngx_int_t ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *log,
    ngx_pool_t *pool);
સ્થિર શૂન્ય ngx_thread_pool_destroy(ngx_thread_pool_t *tp);
સ્થિર શૂન્ય ngx_thread_pool_exit_handler(શૂન્ય *data, ngx_log_t *log);

સ્થિર શૂન્ય *ngx_thread_pool_cycle(શૂન્ય *data);
સ્થિર શૂન્ય ngx_thread_pool_handler(ngx_event_t *ev);

સ્થિર અક્ષર *ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf);

સ્થિર શૂન્ય *ngx_thread_pool_create_conf(ngx_cycle_t *cycle);
સ્થિર અક્ષર *ngx_thread_pool_init_conf(ngx_cycle_t *cycle, શૂન્ય *conf);

સ્થિર ngx_int_t ngx_thread_pool_init_worker(ngx_cycle_t *cycle);
સ્થિર શૂન્ય ngx_thread_pool_exit_worker(ngx_cycle_t *cycle);


સ્થિર ngx_command_t  ngx_thread_pool_commands[] = {

    { ngx_string("thread_pool"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE23,
      ngx_thread_pool,
      0,
      0,
      ખાલી },

      ngx_null_command
};


સ્થિર ngx_core_module_t  ngx_thread_pool_module_ctx = {
    ngx_string("thread_pool"),
    ngx_thread_pool_create_conf,
    ngx_thread_pool_init_conf
};


ngx_module_t  ngx_thread_pool_module = {
    NGX_MODULE_V1,
    &ngx_thread_pool_module_ctx,           /* module context */
    ngx_thread_pool_commands,              /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    ખાલી,                                  /* init master */
    ખાલી,                                  /* init module */
    ngx_thread_pool_init_worker,           /* init process */
    ખાલી,                                  /* init thread */
    ખાલી,                                  /* exit thread */
    ngx_thread_pool_exit_worker,           /* exit process */
    ખાલી,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


સ્થિર ngx_str_t  ngx_thread_pool_default = ngx_string("default");

સ્થિર ngx_uint_t               ngx_thread_pool_task_id;
સ્થિર ngx_atomic_t             ngx_thread_pool_done_lock;
સ્થિર ngx_thread_pool_queue_t  ngx_thread_pool_done;


સ્થિર ngx_int_t
ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *log, ngx_pool_t *pool)
{
    પૂર્ણાંક             err;
    pthread_t       tid;
    ngx_uint_t      n;
    pthread_attr_t  attr;

    જો (ngx_notify == ખાલી) {
        ngx_log_error(NGX_LOG_ALERT, log, 0,
               "the configured event method cannot be used with thread pools");
        પાછા NGX_ERROR;
    }

    ngx_thread_pool_queue_init(&tp->queue);

    જો (ngx_thread_mutex_create(&tp->mtx, log) != NGX_OK) {
        પાછા NGX_ERROR;
    }

    જો (ngx_thread_cond_create(&tp->cond, log) != NGX_OK) {
        (શૂન્ય) ngx_thread_mutex_destroy(&tp->mtx, log);
        પાછા NGX_ERROR;
    }

    tp->log = log;

    err = pthread_attr_init(&attr);
    જો (err) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_attr_init() failed");
        પાછા NGX_ERROR;
    }

    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    જો (err) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_attr_setdetachstate() failed");
        પાછા NGX_ERROR;
    }

#જો 0
    err = pthread_attr_setstacksize(&attr, PTHREAD_ઢગલો_MIN);
    જો (err) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_attr_setstacksize() failed");
        પાછા NGX_ERROR;
    }
#endif

    માટે (n = 0; n < tp->threads; n++) {
        err = pthread_create(&tid, &attr, ngx_thread_pool_cycle, tp);
        જો (err) {
            ngx_log_error(NGX_LOG_ALERT, log, err,
                          "pthread_create() failed");
            પાછા NGX_ERROR;
        }
    }

    (શૂન્ય) pthread_attr_destroy(&attr);

    પાછા NGX_OK;
}


સ્થિર શૂન્ય
ngx_thread_pool_destroy(ngx_thread_pool_t *tp)
{
    ngx_uint_t           n;
    ngx_thread_task_t    task;
    અસ્થિર ngx_uint_t  lock;

    ngx_memzero(&task, કદ(ngx_thread_task_t));

    task.handler = ngx_thread_pool_exit_handler;
    task.ctx = (શૂન્ય *) &lock;

    માટે (n = 0; n < tp->threads; n++) {
        lock = 1;

        જો (ngx_thread_task_post(tp, &task) != NGX_OK) {
            પાછા;
        }

        જ્યારે (lock) {
            ngx_sched_yield();
        }

        task.event.active = 0;
    }

    (શૂન્ય) ngx_thread_cond_destroy(&tp->cond, tp->log);

    (શૂન્ય) ngx_thread_mutex_destroy(&tp->mtx, tp->log);
}


સ્થિર શૂન્ય
ngx_thread_pool_exit_handler(શૂન્ય *data, ngx_log_t *log)
{
    ngx_uint_t *lock = data;

    *lock = 0;

    pthread_exit(ખાલી);
}


ngx_thread_task_t *
ngx_thread_task_alloc(ngx_pool_t *pool, size_t size)
{
    ngx_thread_task_t  *task;

    task = ngx_pcalloc(pool, કદ(ngx_thread_task_t) + size);
    જો (task == ખાલી) {
        પાછા ખાલી;
    }

    task->ctx = task + 1;

    પાછા task;
}


ngx_int_t
ngx_thread_task_post(ngx_thread_pool_t *tp, ngx_thread_task_t *task)
{
    જો (task->event.active) {
        ngx_log_error(NGX_LOG_ALERT, tp->log, 0,
                      "task #%ui already active", task->id);
        પાછા NGX_ERROR;
    }

    જો (ngx_thread_mutex_lock(&tp->mtx, tp->log) != NGX_OK) {
        પાછા NGX_ERROR;
    }

    જો (tp->waiting >= tp->max_queue) {
        (શૂન્ય) ngx_thread_mutex_unlock(&tp->mtx, tp->log);

        ngx_log_error(NGX_LOG_ERR, tp->log, 0,
                      "thread pool \"%V\" queue overflow: %i tasks waiting",
                      &tp->name, tp->waiting);
        પાછા NGX_ERROR;
    }

    task->event.active = 1;

    task->id = ngx_thread_pool_task_id++;
    task->next = ખાલી;

    જો (ngx_thread_cond_signal(&tp->cond, tp->log) != NGX_OK) {
        (શૂન્ય) ngx_thread_mutex_unlock(&tp->mtx, tp->log);
        પાછા NGX_ERROR;
    }

    *tp->queue.last = task;
    tp->queue.last = &task->next;

    tp->waiting++;

    (શૂન્ય) ngx_thread_mutex_unlock(&tp->mtx, tp->log);

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->log, 0,
                   "task #%ui added to thread pool \"%V\"",
                   task->id, &tp->name);

    પાછા NGX_OK;
}


સ્થિર શૂન્ય *
ngx_thread_pool_cycle(શૂન્ય *data)
{
    ngx_thread_pool_t *tp = data;

    પૂર્ણાંક                 err;
    sigset_t            set;
    ngx_thread_task_t  *task;

#જો 0
    ngx_time_update();
#endif

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, tp->log, 0,
                   "thread in pool \"%V\" started", &tp->name);

    sigfillset(&set);

    sigdelset(&set, SIGILL);
    sigdelset(&set, SIGFPE);
    sigdelset(&set, SIGSEGV);
    sigdelset(&set, SIGBUS);

    err = pthread_sigmask(SIG_BLOCK, &set, ખાલી);
    જો (err) {
        ngx_log_error(NGX_LOG_ALERT, tp->log, err, "pthread_sigmask() failed");
        પાછા ખાલી;
    }

    માટે ( ;; ) {
        જો (ngx_thread_mutex_lock(&tp->mtx, tp->log) != NGX_OK) {
            પાછા ખાલી;
        }

        /* the number may become negative */
        tp->waiting--;

        જ્યારે (tp->queue.first == ખાલી) {
            જો (ngx_thread_cond_wait(&tp->cond, &tp->mtx, tp->log)
                != NGX_OK)
            {
                (શૂન્ય) ngx_thread_mutex_unlock(&tp->mtx, tp->log);
                પાછા ખાલી;
            }
        }

        task = tp->queue.first;
        tp->queue.first = task->next;

        જો (tp->queue.first == ખાલી) {
            tp->queue.last = &tp->queue.first;
        }

        જો (ngx_thread_mutex_unlock(&tp->mtx, tp->log) != NGX_OK) {
            પાછા ખાલી;
        }

#જો 0
        ngx_time_update();
#endif

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->log, 0,
                       "run task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->handler(task->ctx, tp->log);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->log, 0,
                       "complete task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->next = ખાલી;

        ngx_spinlock(&ngx_thread_pool_done_lock, 1, 2048);

        *ngx_thread_pool_done.last = task;
        ngx_thread_pool_done.last = &task->next;

        ngx_memory_barrier();

        ngx_unlock(&ngx_thread_pool_done_lock);

        (શૂન્ય) ngx_notify(ngx_thread_pool_handler);
    }
}


સ્થિર શૂન્ય
ngx_thread_pool_handler(ngx_event_t *ev)
{
    ngx_event_t        *event;
    ngx_thread_task_t  *task;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ev->log, 0, "thread pool handler");

    ngx_spinlock(&ngx_thread_pool_done_lock, 1, 2048);

    task = ngx_thread_pool_done.first;
    ngx_thread_pool_done.first = ખાલી;
    ngx_thread_pool_done.last = &ngx_thread_pool_done.first;

    ngx_memory_barrier();

    ngx_unlock(&ngx_thread_pool_done_lock);

    જ્યારે (task) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->log, 0,
                       "run completion handler for task #%ui", task->id);

        event = &task->event;
        task = task->next;

        event->complete = 1;
        event->active = 0;

        event->handler(event);
    }
}


સ્થિર શૂન્ય *
ngx_thread_pool_create_conf(ngx_cycle_t *cycle)
{
    ngx_thread_pool_conf_t  *tcf;

    tcf = ngx_pcalloc(cycle->pool, કદ(ngx_thread_pool_conf_t));
    જો (tcf == ખાલી) {
        પાછા ખાલી;
    }

    જો (ngx_array_init(&tcf->pools, cycle->pool, 4,
                       કદ(ngx_thread_pool_t *))
        != NGX_OK)
    {
        પાછા ખાલી;
    }

    પાછા tcf;
}


સ્થિર અક્ષર *
ngx_thread_pool_init_conf(ngx_cycle_t *cycle, શૂન્ય *conf)
{
    ngx_thread_pool_conf_t *tcf = conf;

    ngx_uint_t           i;
    ngx_thread_pool_t  **tpp;

    tpp = tcf->pools.elts;

    માટે (i = 0; i < tcf->pools.nelts; i++) {

        જો (tpp[i]->threads) {
            ચાલુ;
        }

        જો (tpp[i]->name.len == ngx_thread_pool_default.len
            && ngx_strncmp(tpp[i]->name.data, ngx_thread_pool_default.data,
                           ngx_thread_pool_default.len)
               == 0)
        {
            tpp[i]->threads = 32;
            tpp[i]->max_queue = 65536;
            ચાલુ;
        }

        ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                      "unknown thread pool \"%V\" in %s:%ui",
                      &tpp[i]->name, tpp[i]->file, tpp[i]->લીટી);

        પાછા NGX_CONF_ERROR;
    }

    પાછા NGX_CONF_OK;
}


સ્થિર અક્ષર *
ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf)
{
    ngx_str_t          *કિંમત;
    ngx_uint_t          i;
    ngx_thread_pool_t  *tp;

    કિંમત = cf->args->elts;

    tp = ngx_thread_pool_add(cf, &કિંમત[1]);

    જો (tp == ખાલી) {
        પાછા NGX_CONF_ERROR;
    }

    જો (tp->threads) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "duplicate thread pool \"%V\"", &tp->name);
        પાછા NGX_CONF_ERROR;
    }

    tp->max_queue = 65536;

    માટે (i = 2; i < cf->args->nelts; i++) {

        જો (ngx_strncmp(કિંમત[i].data, "threads=", 8) == 0) {

            tp->threads = ngx_atoi(કિંમત[i].data + 8, કિંમત[i].len - 8);

            જો (tp->threads == (ngx_uint_t) NGX_ERROR || tp->threads == 0) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid threads value \"%V\"", &કિંમત[i]);
                પાછા NGX_CONF_ERROR;
            }

            ચાલુ;
        }

        જો (ngx_strncmp(કિંમત[i].data, "max_queue=", 10) == 0) {

            tp->max_queue = ngx_atoi(કિંમત[i].data + 10, કિંમત[i].len - 10);

            જો (tp->max_queue == NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid max_queue value \"%V\"", &કિંમત[i]);
                પાછા NGX_CONF_ERROR;
            }

            ચાલુ;
        }
    }

    જો (tp->threads == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%V\" must have \"threads\" parameter",
                           &cmd->name);
        પાછા NGX_CONF_ERROR;
    }

    પાછા NGX_CONF_OK;
}


ngx_thread_pool_t *
ngx_thread_pool_add(ngx_conf_t *cf, ngx_str_t *name)
{
    ngx_thread_pool_t       *tp, **tpp;
    ngx_thread_pool_conf_t  *tcf;

    જો (name == ખાલી) {
        name = &ngx_thread_pool_default;
    }

    tp = ngx_thread_pool_get(cf->cycle, name);

    જો (tp) {
        પાછા tp;
    }

    tp = ngx_pcalloc(cf->pool, કદ(ngx_thread_pool_t));
    જો (tp == ખાલી) {
        પાછા ખાલી;
    }

    tp->name = *name;
    tp->file = cf->conf_file->file.name.data;
    tp->લીટી = cf->conf_file->લીટી;

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cf->cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    tpp = ngx_array_ધકેલો(&tcf->pools);
    જો (tpp == ખાલી) {
        પાછા ખાલી;
    }

    *tpp = tp;

    પાછા tp;
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

    માટે (i = 0; i < tcf->pools.nelts; i++) {

        જો (tpp[i]->name.len == name->len
            && ngx_strncmp(tpp[i]->name.data, name->data, name->len) == 0)
        {
            પાછા tpp[i];
        }
    }

    પાછા ખાલી;
}


સ્થિર ngx_int_t
ngx_thread_pool_init_worker(ngx_cycle_t *cycle)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    જો (ngx_process != NGX_PROCESS_WORKER
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        પાછા NGX_OK;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    જો (tcf == ખાલી) {
        પાછા NGX_OK;
    }

    ngx_thread_pool_queue_init(&ngx_thread_pool_done);

    tpp = tcf->pools.elts;

    માટે (i = 0; i < tcf->pools.nelts; i++) {
        જો (ngx_thread_pool_init(tpp[i], cycle->log, cycle->pool) != NGX_OK) {
            પાછા NGX_ERROR;
        }
    }

    પાછા NGX_OK;
}


સ્થિર શૂન્ય
ngx_thread_pool_exit_worker(ngx_cycle_t *cycle)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    જો (ngx_process != NGX_PROCESS_WORKER
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        પાછા;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    જો (tcf == ખાલી) {
        પાછા;
    }

    tpp = tcf->pools.elts;

    માટે (i = 0; i < tcf->pools.nelts; i++) {
        ngx_thread_pool_destroy(tpp[i]);
    }
}
