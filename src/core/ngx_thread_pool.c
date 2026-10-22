
/*
 * Copyright (C) Nginx, Inc.
 * Copyright (C) Valentin V. Bartenev
 * Copyright (C) Ruslan Ermilov
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>
#समावेश <ngx_thread_pool.h>


प्रकार काष्ठा {
    ngx_array_t               pools;
} ngx_thread_pool_conf_t;


प्रकार काष्ठा {
    ngx_thread_task_t        *first;
    ngx_thread_task_t       **last;
} ngx_thread_pool_queue_t;

#घोषणा ngx_thread_pool_queue_init(q)                                         \
    (q)->first = शून्य;                                                        \
    (q)->last = &(q)->first


काष्ठा ngx_thread_pool_s {
    ngx_thread_mutex_t        mtx;
    ngx_thread_pool_queue_t   queue;
    ngx_int_t                 waiting;
    ngx_thread_cond_t         cond;

    ngx_log_t                *log;

    ngx_str_t                 name;
    ngx_uint_t                threads;
    ngx_int_t                 max_queue;

    u_char                   *file;
    ngx_uint_t                पंक्ति;
};


अटल ngx_int_t ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *log,
    ngx_pool_t *pool);
अटल व्योम ngx_thread_pool_destroy(ngx_thread_pool_t *tp);
अटल व्योम ngx_thread_pool_exit_handler(व्योम *तथ्य, ngx_log_t *log);

अटल व्योम *ngx_thread_pool_cycle(व्योम *तथ्य);
अटल व्योम ngx_thread_pool_handler(ngx_event_t *ev);

अटल अक्षर *ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf);

अटल व्योम *ngx_thread_pool_create_conf(ngx_cycle_t *cycle);
अटल अक्षर *ngx_thread_pool_init_conf(ngx_cycle_t *cycle, व्योम *conf);

अटल ngx_int_t ngx_thread_pool_init_worker(ngx_cycle_t *cycle);
अटल व्योम ngx_thread_pool_exit_worker(ngx_cycle_t *cycle);


अटल ngx_command_t  ngx_thread_pool_commands[] = {

    { ngx_string("thread_pool"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE23,
      ngx_thread_pool,
      0,
      0,
      शून्य },

      ngx_null_command
};


अटल ngx_core_module_t  ngx_thread_pool_module_ctx = {
    ngx_string("thread_pool"),
    ngx_thread_pool_create_conf,
    ngx_thread_pool_init_conf
};


ngx_module_t  ngx_thread_pool_module = {
    NGX_MODULE_V1,
    &ngx_thread_pool_module_ctx,           /* module context */
    ngx_thread_pool_commands,              /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    शून्य,                                  /* init master */
    शून्य,                                  /* init module */
    ngx_thread_pool_init_worker,           /* init process */
    शून्य,                                  /* init thread */
    शून्य,                                  /* exit thread */
    ngx_thread_pool_exit_worker,           /* exit process */
    शून्य,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


अटल ngx_str_t  ngx_thread_pool_default = ngx_string("default");

अटल ngx_uint_t               ngx_thread_pool_task_id;
अटल ngx_atomic_t             ngx_thread_pool_done_lock;
अटल ngx_thread_pool_queue_t  ngx_thread_pool_done;


अटल ngx_int_t
ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *log, ngx_pool_t *pool)
{
    पूर्णांक             err;
    pthread_t       tid;
    ngx_uint_t      n;
    pthread_attr_t  attr;

    अगर (ngx_notify == शून्य) {
        ngx_log_error(NGX_LOG_ALERT, log, 0,
               "the configured event method cannot be used with thread pools");
        वापस NGX_ERROR;
    }

    ngx_thread_pool_queue_init(&tp->queue);

    अगर (ngx_thread_mutex_create(&tp->mtx, log) != NGX_OK) {
        वापस NGX_ERROR;
    }

    अगर (ngx_thread_cond_create(&tp->cond, log) != NGX_OK) {
        (व्योम) ngx_thread_mutex_destroy(&tp->mtx, log);
        वापस NGX_ERROR;
    }

    tp->log = log;

    err = pthread_attr_init(&attr);
    अगर (err) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_attr_init() failed");
        वापस NGX_ERROR;
    }

    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    अगर (err) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_attr_setdetachstate() failed");
        वापस NGX_ERROR;
    }

#अगर 0
    err = pthread_attr_setstacksize(&attr, PTHREAD_ढेर_MIN);
    अगर (err) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_attr_setstacksize() failed");
        वापस NGX_ERROR;
    }
#पूर्ण_अगर

    क्रम (n = 0; n < tp->threads; n++) {
        err = pthread_create(&tid, &attr, ngx_thread_pool_cycle, tp);
        अगर (err) {
            ngx_log_error(NGX_LOG_ALERT, log, err,
                          "pthread_create() failed");
            वापस NGX_ERROR;
        }
    }

    (व्योम) pthread_attr_destroy(&attr);

    वापस NGX_OK;
}


अटल व्योम
ngx_thread_pool_destroy(ngx_thread_pool_t *tp)
{
    ngx_uint_t           n;
    ngx_thread_task_t    task;
    अस्थिर ngx_uint_t  lock;

    ngx_memzero(&task, माप(ngx_thread_task_t));

    task.handler = ngx_thread_pool_exit_handler;
    task.ctx = (व्योम *) &lock;

    क्रम (n = 0; n < tp->threads; n++) {
        lock = 1;

        अगर (ngx_thread_task_post(tp, &task) != NGX_OK) {
            वापस;
        }

        जबतक (lock) {
            ngx_sched_yield();
        }

        task.event.active = 0;
    }

    (व्योम) ngx_thread_cond_destroy(&tp->cond, tp->log);

    (व्योम) ngx_thread_mutex_destroy(&tp->mtx, tp->log);
}


अटल व्योम
ngx_thread_pool_exit_handler(व्योम *तथ्य, ngx_log_t *log)
{
    ngx_uint_t *lock = तथ्य;

    *lock = 0;

    pthread_exit(शून्य);
}


ngx_thread_task_t *
ngx_thread_task_alloc(ngx_pool_t *pool, माप_प्रकार आकार)
{
    ngx_thread_task_t  *task;

    task = ngx_pcalloc(pool, माप(ngx_thread_task_t) + आकार);
    अगर (task == शून्य) {
        वापस शून्य;
    }

    task->ctx = task + 1;

    वापस task;
}


ngx_int_t
ngx_thread_task_post(ngx_thread_pool_t *tp, ngx_thread_task_t *task)
{
    अगर (task->event.active) {
        ngx_log_error(NGX_LOG_ALERT, tp->log, 0,
                      "task #%ui already active", task->id);
        वापस NGX_ERROR;
    }

    अगर (ngx_thread_mutex_lock(&tp->mtx, tp->log) != NGX_OK) {
        वापस NGX_ERROR;
    }

    अगर (tp->waiting >= tp->max_queue) {
        (व्योम) ngx_thread_mutex_unlock(&tp->mtx, tp->log);

        ngx_log_error(NGX_LOG_ERR, tp->log, 0,
                      "thread pool \"%V\" queue overflow: %i tasks waiting",
                      &tp->name, tp->waiting);
        वापस NGX_ERROR;
    }

    task->event.active = 1;

    task->id = ngx_thread_pool_task_id++;
    task->अग्र = शून्य;

    अगर (ngx_thread_cond_signal(&tp->cond, tp->log) != NGX_OK) {
        (व्योम) ngx_thread_mutex_unlock(&tp->mtx, tp->log);
        वापस NGX_ERROR;
    }

    *tp->queue.last = task;
    tp->queue.last = &task->अग्र;

    tp->waiting++;

    (व्योम) ngx_thread_mutex_unlock(&tp->mtx, tp->log);

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->log, 0,
                   "task #%ui added to thread pool \"%V\"",
                   task->id, &tp->name);

    वापस NGX_OK;
}


अटल व्योम *
ngx_thread_pool_cycle(व्योम *तथ्य)
{
    ngx_thread_pool_t *tp = तथ्य;

    पूर्णांक                 err;
    sigset_t            set;
    ngx_thread_task_t  *task;

#अगर 0
    ngx_time_update();
#पूर्ण_अगर

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, tp->log, 0,
                   "thread in pool \"%V\" started", &tp->name);

    sigfillset(&set);

    sigdelset(&set, संक_अवैध);
    sigdelset(&set, संक_भ_त्रुटि);
    sigdelset(&set, संक_अंश);
    sigdelset(&set, SIGBUS);

    err = pthread_sigmask(SIG_BLOCK, &set, शून्य);
    अगर (err) {
        ngx_log_error(NGX_LOG_ALERT, tp->log, err, "pthread_sigmask() failed");
        वापस शून्य;
    }

    क्रम ( ;; ) {
        अगर (ngx_thread_mutex_lock(&tp->mtx, tp->log) != NGX_OK) {
            वापस शून्य;
        }

        /* the number may become negative */
        tp->waiting--;

        जबतक (tp->queue.first == शून्य) {
            अगर (ngx_thread_cond_wait(&tp->cond, &tp->mtx, tp->log)
                != NGX_OK)
            {
                (व्योम) ngx_thread_mutex_unlock(&tp->mtx, tp->log);
                वापस शून्य;
            }
        }

        task = tp->queue.first;
        tp->queue.first = task->अग्र;

        अगर (tp->queue.first == शून्य) {
            tp->queue.last = &tp->queue.first;
        }

        अगर (ngx_thread_mutex_unlock(&tp->mtx, tp->log) != NGX_OK) {
            वापस शून्य;
        }

#अगर 0
        ngx_time_update();
#पूर्ण_अगर

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->log, 0,
                       "run task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->handler(task->ctx, tp->log);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->log, 0,
                       "complete task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->अग्र = शून्य;

        ngx_spinlock(&ngx_thread_pool_done_lock, 1, 2048);

        *ngx_thread_pool_done.last = task;
        ngx_thread_pool_done.last = &task->अग्र;

        ngx_memory_barrier();

        ngx_unlock(&ngx_thread_pool_done_lock);

        (व्योम) ngx_notify(ngx_thread_pool_handler);
    }
}


अटल व्योम
ngx_thread_pool_handler(ngx_event_t *ev)
{
    ngx_event_t        *event;
    ngx_thread_task_t  *task;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ev->log, 0, "thread pool handler");

    ngx_spinlock(&ngx_thread_pool_done_lock, 1, 2048);

    task = ngx_thread_pool_done.first;
    ngx_thread_pool_done.first = शून्य;
    ngx_thread_pool_done.last = &ngx_thread_pool_done.first;

    ngx_memory_barrier();

    ngx_unlock(&ngx_thread_pool_done_lock);

    जबतक (task) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->log, 0,
                       "run completion handler for task #%ui", task->id);

        event = &task->event;
        task = task->अग्र;

        event->complete = 1;
        event->active = 0;

        event->handler(event);
    }
}


अटल व्योम *
ngx_thread_pool_create_conf(ngx_cycle_t *cycle)
{
    ngx_thread_pool_conf_t  *tcf;

    tcf = ngx_pcalloc(cycle->pool, माप(ngx_thread_pool_conf_t));
    अगर (tcf == शून्य) {
        वापस शून्य;
    }

    अगर (ngx_array_init(&tcf->pools, cycle->pool, 4,
                       माप(ngx_thread_pool_t *))
        != NGX_OK)
    {
        वापस शून्य;
    }

    वापस tcf;
}


अटल अक्षर *
ngx_thread_pool_init_conf(ngx_cycle_t *cycle, व्योम *conf)
{
    ngx_thread_pool_conf_t *tcf = conf;

    ngx_uint_t           i;
    ngx_thread_pool_t  **tpp;

    tpp = tcf->pools.elts;

    क्रम (i = 0; i < tcf->pools.nelts; i++) {

        अगर (tpp[i]->threads) {
            जारी;
        }

        अगर (tpp[i]->name.len == ngx_thread_pool_default.len
            && ngx_strncmp(tpp[i]->name.तथ्य, ngx_thread_pool_default.तथ्य,
                           ngx_thread_pool_default.len)
               == 0)
        {
            tpp[i]->threads = 32;
            tpp[i]->max_queue = 65536;
            जारी;
        }

        ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                      "unknown thread pool \"%V\" in %s:%ui",
                      &tpp[i]->name, tpp[i]->file, tpp[i]->पंक्ति);

        वापस NGX_CONF_ERROR;
    }

    वापस NGX_CONF_OK;
}


अटल अक्षर *
ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf)
{
    ngx_str_t          *मूल्य;
    ngx_uint_t          i;
    ngx_thread_pool_t  *tp;

    मूल्य = cf->args->elts;

    tp = ngx_thread_pool_add(cf, &मूल्य[1]);

    अगर (tp == शून्य) {
        वापस NGX_CONF_ERROR;
    }

    अगर (tp->threads) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "duplicate thread pool \"%V\"", &tp->name);
        वापस NGX_CONF_ERROR;
    }

    tp->max_queue = 65536;

    क्रम (i = 2; i < cf->args->nelts; i++) {

        अगर (ngx_strncmp(मूल्य[i].तथ्य, "threads=", 8) == 0) {

            tp->threads = ngx_atoi(मूल्य[i].तथ्य + 8, मूल्य[i].len - 8);

            अगर (tp->threads == (ngx_uint_t) NGX_ERROR || tp->threads == 0) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid threads value \"%V\"", &मूल्य[i]);
                वापस NGX_CONF_ERROR;
            }

            जारी;
        }

        अगर (ngx_strncmp(मूल्य[i].तथ्य, "max_queue=", 10) == 0) {

            tp->max_queue = ngx_atoi(मूल्य[i].तथ्य + 10, मूल्य[i].len - 10);

            अगर (tp->max_queue == NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid max_queue value \"%V\"", &मूल्य[i]);
                वापस NGX_CONF_ERROR;
            }

            जारी;
        }
    }

    अगर (tp->threads == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%V\" must have \"threads\" parameter",
                           &cmd->name);
        वापस NGX_CONF_ERROR;
    }

    वापस NGX_CONF_OK;
}


ngx_thread_pool_t *
ngx_thread_pool_add(ngx_conf_t *cf, ngx_str_t *name)
{
    ngx_thread_pool_t       *tp, **tpp;
    ngx_thread_pool_conf_t  *tcf;

    अगर (name == शून्य) {
        name = &ngx_thread_pool_default;
    }

    tp = ngx_thread_pool_get(cf->cycle, name);

    अगर (tp) {
        वापस tp;
    }

    tp = ngx_pcalloc(cf->pool, माप(ngx_thread_pool_t));
    अगर (tp == शून्य) {
        वापस शून्य;
    }

    tp->name = *name;
    tp->file = cf->conf_file->file.name.तथ्य;
    tp->पंक्ति = cf->conf_file->पंक्ति;

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cf->cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    tpp = ngx_array_धकेलो(&tcf->pools);
    अगर (tpp == शून्य) {
        वापस शून्य;
    }

    *tpp = tp;

    वापस tp;
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

    क्रम (i = 0; i < tcf->pools.nelts; i++) {

        अगर (tpp[i]->name.len == name->len
            && ngx_strncmp(tpp[i]->name.तथ्य, name->तथ्य, name->len) == 0)
        {
            वापस tpp[i];
        }
    }

    वापस शून्य;
}


अटल ngx_int_t
ngx_thread_pool_init_worker(ngx_cycle_t *cycle)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    अगर (ngx_process != NGX_PROCESS_WORKER
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        वापस NGX_OK;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    अगर (tcf == शून्य) {
        वापस NGX_OK;
    }

    ngx_thread_pool_queue_init(&ngx_thread_pool_done);

    tpp = tcf->pools.elts;

    क्रम (i = 0; i < tcf->pools.nelts; i++) {
        अगर (ngx_thread_pool_init(tpp[i], cycle->log, cycle->pool) != NGX_OK) {
            वापस NGX_ERROR;
        }
    }

    वापस NGX_OK;
}


अटल व्योम
ngx_thread_pool_exit_worker(ngx_cycle_t *cycle)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    अगर (ngx_process != NGX_PROCESS_WORKER
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        वापस;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    अगर (tcf == शून्य) {
        वापस;
    }

    tpp = tcf->pools.elts;

    क्रम (i = 0; i < tcf->pools.nelts; i++) {
        ngx_thread_pool_destroy(tpp[i]);
    }
}
