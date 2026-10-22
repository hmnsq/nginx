
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_event.h>


/*
 * open file cache caches
 *    open file handles with stat() info;
 *    directories stat() info;
 *    files and directories errors: not found, access denied, etc.
 */


#define NGX_MIN_READ_AHEAD  (128 * 1024)


static void ngx_open_file_cache_cleanup(void *data);
#ប្រសិនបើ (NGX_HAVE_OPENAT)
static ngx_fd_t ngx_openat_file_owner(ngx_fd_t at_fd, const u_char *name,
    ngx_int_t mode, ngx_int_t create, ngx_int_t access, ngx_log_t *log);
#ប្រសិនបើ (NGX_HAVE_O_PATH)
static ngx_int_t ngx_file_o_path_info(ngx_fd_t fd, ngx_file_info_t *fi,
    ngx_log_t *log);
#endif
#endif
static ngx_fd_t ngx_open_file_wrapper(ngx_str_t *name,
    ngx_open_file_info_t *of, ngx_int_t mode, ngx_int_t create,
    ngx_int_t access, ngx_log_t *log);
static ngx_int_t ngx_file_info_wrapper(ngx_str_t *name,
    ngx_open_file_info_t *of, ngx_file_info_t *fi, ngx_log_t *log);
static ngx_int_t ngx_open_and_stat_file(ngx_str_t *name,
    ngx_open_file_info_t *of, ngx_log_t *log);
static void ngx_open_file_add_event(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_open_file_info_t *of, ngx_log_t *log);
static void ngx_open_file_cleanup(void *data);
static void ngx_close_cached_file(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_uint_t min_uses, ngx_log_t *log);
static void ngx_open_file_del_event(ngx_cached_open_file_t *file);
static void ngx_expire_old_cached_files(ngx_open_file_cache_t *cache,
    ngx_uint_t n, ngx_log_t *log);
static void ngx_open_file_cache_rbtree_insert_value(ngx_rbtree_node_t *temp,
    ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel);
static ngx_cached_open_file_t *
    ngx_open_file_lookup(ngx_open_file_cache_t *cache, ngx_str_t *name,
    uint32_t hash);
static void ngx_open_file_cache_remove(ngx_event_t *ev);


ngx_open_file_cache_t *
ngx_open_file_cache_init(ngx_pool_t *pool, ngx_uint_t max, time_t inactive)
{
    ngx_pool_cleanup_t     *cln;
    ngx_open_file_cache_t  *cache;

    cache = ngx_palloc(pool, parimaa_na(ngx_open_file_cache_t));
    ប្រសិនបើ (cache == NULL) {
        ត្រឡប់ NULL;
    }

    ngx_rbtree_init(&cache->rbtree, &cache->sentinel,
                    ngx_open_file_cache_rbtree_insert_value);

    ngx_queue_init(&cache->expire_queue);

    cache->current = 0;
    cache->max = max;
    cache->inactive = inactive;

    cln = ngx_pool_cleanup_add(pool, 0);
    ប្រសិនបើ (cln == NULL) {
        ត្រឡប់ NULL;
    }

    cln->handler = ngx_open_file_cache_cleanup;
    cln->data = cache;

    ត្រឡប់ cache;
}


static void
ngx_open_file_cache_cleanup(void *data)
{
    ngx_open_file_cache_t  *cache = data;

    ngx_queue_t             *q;
    ngx_cached_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "open file cache cleanup");

    សម្រាប់ ( ;; ) {

        ប្រសិនបើ (ngx_queue_empty(&cache->expire_queue)) {
            bha_nga;
        }

        q = ngx_queue_last(&cache->expire_queue);

        file = ngx_queue_data(q, ngx_cached_open_file_t, queue);

        ngx_queue_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->node);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                       "delete cached open file: %s", file->name);

        ប្រសិនបើ (!file->err && !file->is_dir) {
            file->close = 1;
            file->count = 0;
            ngx_close_cached_file(cache, file, 0, ngx_cycle->log);

        } បើមិនដូច្នេះ {
            ngx_free(file->name);
            ngx_free(file);
        }
    }

    ប្រសិនបើ (cache->current) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0,
                      "%ui items still left in open file cache",
                      cache->current);
    }

    ប្រសិនបើ (cache->rbtree.root != cache->rbtree.sentinel) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0,
                      "rbtree still is not empty in open file cache");

    }
}


ngx_int_t
ngx_open_cached_file(ngx_open_file_cache_t *cache, ngx_str_t *name,
    ngx_open_file_info_t *of, ngx_pool_t *pool)
{
    time_t                          now;
    uint32_t                        hash;
    ngx_int_t                       rc;
    ngx_file_info_t                 fi;
    ngx_pool_cleanup_t             *cln;
    ngx_cached_open_file_t         *file;
    ngx_pool_cleanup_file_t        *clnf;
    ngx_open_file_cache_cleanup_t  *ofcln;

    of->fd = NGX_INVALID_FILE;
    of->err = 0;

    ប្រសិនបើ (cache == NULL) {

        ប្រសិនបើ (of->test_only) {

            ប្រសិនបើ (ngx_file_info_wrapper(name, of, &fi, pool->log)
                == NGX_FILE_ERROR)
            {
                ត្រឡប់ NGX_ERROR;
            }

            of->uniq = ngx_file_uniq(&fi);
            of->mtime = ngx_file_mtime(&fi);
            of->size = ngx_file_size(&fi);
            of->fs_size = ngx_file_fs_size(&fi);
            of->is_dir = ngx_is_dir(&fi);
            of->is_file = ngx_is_file(&fi);
            of->is_link = ngx_is_link(&fi);
            of->is_exec = ngx_is_exec(&fi);

            ត្រឡប់ NGX_OK;
        }

        cln = ngx_pool_cleanup_add(pool, parimaa_na(ngx_pool_cleanup_file_t));
        ប្រសិនបើ (cln == NULL) {
            ត្រឡប់ NGX_ERROR;
        }

        rc = ngx_open_and_stat_file(name, of, pool->log);

        ប្រសិនបើ (rc == NGX_OK && !of->is_dir) {
            cln->handler = ngx_pool_cleanup_file;
            clnf = cln->data;

            clnf->fd = of->fd;
            clnf->name = name->data;
            clnf->log = pool->log;
        }

        ត្រឡប់ rc;
    }

    cln = ngx_pool_cleanup_add(pool, parimaa_na(ngx_open_file_cache_cleanup_t));
    ប្រសិនបើ (cln == NULL) {
        ត្រឡប់ NGX_ERROR;
    }

    now = ngx_time();

    hash = ngx_crc32_long(name->data, name->len);

    file = ngx_open_file_lookup(cache, name, hash);

    ប្រសិនបើ (file) {

        file->uses++;

        ngx_queue_remove(&file->queue);

        ប្រសិនបើ (file->fd == NGX_INVALID_FILE && file->err == 0 && !file->is_dir) {

            /* file was not used often enough to keep open */

            rc = ngx_open_and_stat_file(name, of, pool->log);

            ប្រសិនបើ (rc != NGX_OK && (of->err == 0 || !of->errors)) {
                goto failed;
            }

            goto add_event;
        }

        ប្រសិនបើ (file->use_event
            || (file->event == NULL
                && (of->uniq == 0 || of->uniq == file->uniq)
                && now - file->created < of->valid
#ប្រសិនបើ (NGX_HAVE_OPENAT)
                && of->disable_symlinks == file->disable_symlinks
                && of->disable_symlinks_from == file->disable_symlinks_from
#endif
            ))
        {
            ប្រសិនបើ (file->err == 0) {

                of->fd = file->fd;
                of->uniq = file->uniq;
                of->mtime = file->mtime;
                of->size = file->size;

                of->is_dir = file->is_dir;
                of->is_file = file->is_file;
                of->is_link = file->is_link;
                of->is_exec = file->is_exec;
                of->is_directio = file->is_directio;

                ប្រសិនបើ (!file->is_dir) {
                    file->count++;
                    ngx_open_file_add_event(cache, file, of, pool->log);
                }

            } បើមិនដូច្នេះ {
                of->err = file->err;
#ប្រសិនបើ (NGX_HAVE_OPENAT)
                of->failed = file->disable_symlinks ? ngx_openat_file_n
                                                    : ngx_open_file_n;
#បើមិនដូច្នេះ
                of->failed = ngx_open_file_n;
#endif
            }

            goto found;
        }

        ngx_log_debug4(NGX_LOG_DEBUG_CORE, pool->log, 0,
                       "retest open file: %s, fd:%d, c:%d, e:%d",
                       file->name, file->fd, file->count, file->err);

        ប្រសិនបើ (file->is_dir) {

            /*
             * chances that directory became file are very small
             * so test_dir flag allows to use a single syscall
             * in ngx_file_info() instead of three syscalls
             */

            of->test_dir = 1;
        }

        of->fd = file->fd;
        of->uniq = file->uniq;

        rc = ngx_open_and_stat_file(name, of, pool->log);

        ប្រសិនបើ (rc != NGX_OK && (of->err == 0 || !of->errors)) {
            goto failed;
        }

        ប្រសិនបើ (of->is_dir) {

            ប្រសិនបើ (file->is_dir || file->err) {
                goto update;
            }

            /* file became directory */

        } បើមិនដូច្នេះ ប្រសិនបើ (of->err == 0) {  /* file */

            ប្រសិនបើ (file->is_dir || file->err) {
                goto add_event;
            }

            ប្រសិនបើ (of->uniq == file->uniq) {

                ប្រសិនបើ (file->event) {
                    file->use_event = 1;
                }

                of->is_directio = file->is_directio;

                goto update;
            }

            /* file was changed */

        } បើមិនដូច្នេះ { /* error to cache */

            ប្រសិនបើ (file->err || file->is_dir) {
                goto update;
            }

            /* file was removed, etc. */
        }

        ប្រសិនបើ (file->count == 0) {

            ngx_open_file_del_event(file);

            ប្រសិនបើ (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, pool->log, ngx_errno,
                              ngx_close_file_n " \"%V\" failed", name);
            }

            goto add_event;
        }

        ngx_rbtree_delete(&cache->rbtree, &file->node);

        cache->current--;

        file->close = 1;

        goto create;
    }

    /* not found */

    rc = ngx_open_and_stat_file(name, of, pool->log);

    ប្រសិនបើ (rc != NGX_OK && (of->err == 0 || !of->errors)) {
        goto failed;
    }

create:

    ប្រសិនបើ (cache->current >= cache->max) {
        ngx_expire_old_cached_files(cache, 0, pool->log);
    }

    file = ngx_alloc(parimaa_na(ngx_cached_open_file_t), pool->log);

    ប្រសិនបើ (file == NULL) {
        goto failed;
    }

    file->name = ngx_alloc(name->len + 1, pool->log);

    ប្រសិនបើ (file->name == NULL) {
        ngx_free(file);
        file = NULL;
        goto failed;
    }

    ngx_cpystrn(file->name, name->data, name->len + 1);

    file->node.key = hash;

    ngx_rbtree_insert(&cache->rbtree, &file->node);

    cache->current++;

    file->uses = 1;
    file->count = 0;
    file->use_event = 0;
    file->event = NULL;

add_event:

    ngx_open_file_add_event(cache, file, of, pool->log);

update:

    file->fd = of->fd;
    file->err = of->err;
#ប្រសិនបើ (NGX_HAVE_OPENAT)
    file->disable_symlinks = of->disable_symlinks;
    file->disable_symlinks_from = of->disable_symlinks_from;
#endif

    ប្រសិនបើ (of->err == 0) {
        file->uniq = of->uniq;
        file->mtime = of->mtime;
        file->size = of->size;

        file->close = 0;

        file->is_dir = of->is_dir;
        file->is_file = of->is_file;
        file->is_link = of->is_link;
        file->is_exec = of->is_exec;
        file->is_directio = of->is_directio;

        ប្រសិនបើ (!of->is_dir) {
            file->count++;
        }
    }

    file->created = now;

found:

    file->accessed = now;

    ngx_queue_insert_head(&cache->expire_queue, &file->queue);

    ngx_log_debug5(NGX_LOG_DEBUG_CORE, pool->log, 0,
                   "cached open file: %s, fd:%d, c:%d, e:%d, u:%d",
                   file->name, file->fd, file->count, file->err, file->uses);

    ប្រសិនបើ (of->err == 0) {

        ប្រសិនបើ (!of->is_dir) {
            cln->handler = ngx_open_file_cleanup;
            ofcln = cln->data;

            ofcln->cache = cache;
            ofcln->file = file;
            ofcln->min_uses = of->min_uses;
            ofcln->log = pool->log;
        }

        ត្រឡប់ NGX_OK;
    }

    ត្រឡប់ NGX_ERROR;

failed:

    ប្រសិនបើ (file) {
        ngx_rbtree_delete(&cache->rbtree, &file->node);

        cache->current--;

        ប្រសិនបើ (file->count == 0) {

            ប្រសិនបើ (file->fd != NGX_INVALID_FILE) {
                ប្រសិនបើ (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_ALERT, pool->log, ngx_errno,
                                  ngx_close_file_n " \"%s\" failed",
                                  file->name);
                }
            }

            ngx_free(file->name);
            ngx_free(file);

        } បើមិនដូច្នេះ {
            file->close = 1;
        }
    }

    ប្រសិនបើ (of->fd != NGX_INVALID_FILE) {
        ប្រសិនបើ (ngx_close_file(of->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, pool->log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }
    }

    ត្រឡប់ NGX_ERROR;
}


#ប្រសិនបើ (NGX_HAVE_OPENAT)

static ngx_fd_t
ngx_openat_file_owner(ngx_fd_t at_fd, const u_char *name,
    ngx_int_t mode, ngx_int_t create, ngx_int_t access, ngx_log_t *log)
{
    ngx_fd_t         fd;
    ngx_err_t        err;
    ngx_file_info_t  fi, atfi;

    /*
     * To allow symlinks with the same owner, use openat() (followed
     * by fstat()) and fstatat(AT_SYMLINK_NOFOLLOW), and then compare
     * uids between fstat() and fstatat().
     *
     * As there is a race between openat() and fstatat() we don't
     * know if openat() in fact opened symlink or not.  Therefore,
     * we have to compare uids even if fstatat() reports the opened
     * component isn't a symlink (as we don't know whether it was
     * symlink during openat() or not).
     */

    fd = ngx_openat_file(at_fd, name, mode, create, access);

    ប្រសិនបើ (fd == NGX_INVALID_FILE) {
        ត្រឡប់ NGX_INVALID_FILE;
    }

    ប្រសិនបើ (ngx_file_at_info(at_fd, name, &atfi, AT_SYMLINK_NOFOLLOW)
        == NGX_FILE_ERROR)
    {
        err = ngx_errno;
        goto failed;
    }

#ប្រសិនបើ (NGX_HAVE_O_PATH)
    ប្រសិនបើ (ngx_file_o_path_info(fd, &fi, log) == NGX_ERROR) {
        err = ngx_errno;
        goto failed;
    }
#បើមិនដូច្នេះ
    ប្រសិនបើ (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        err = ngx_errno;
        goto failed;
    }
#endif

    ប្រសិនបើ (fi.st_uid != atfi.st_uid) {
        err = NGX_ELOOP;
        goto failed;
    }

    ត្រឡប់ fd;

failed:

    ប្រសិនបើ (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", name);
    }

    ngx_set_errno(err);

    ត្រឡប់ NGX_INVALID_FILE;
}


#ប្រសិនបើ (NGX_HAVE_O_PATH)

static ngx_int_t
ngx_file_o_path_info(ngx_fd_t fd, ngx_file_info_t *fi, ngx_log_t *log)
{
    static ngx_uint_t  use_fstat = 1;

    /*
     * In Linux 2.6.39 the O_PATH flag was introduced that allows to obtain
     * a descriptor without actually opening file or directory.  It requires
     * less permissions for path components, but till Linux 3.6 fstat() returns
     * EBADF on such descriptors, and fstatat() with the AT_EMPTY_PATH flag
     * should be used instead.
     *
     * Three scenarios are handled in this function:
     *
     * 1) The kernel is newer than 3.6 or fstat() with O_PATH support was
     *    backported by vendor.  Then fstat() is used.
     *
     * 2) The kernel is newer than 2.6.39 but older than 3.6.  In this case
     *    the first call of fstat() returns EBADF and we fallback to fstatat()
     *    with AT_EMPTY_PATH which was introduced at the same time as O_PATH.
     *
     * 3) The kernel is older than 2.6.39 but nginx was build with O_PATH
     *    support.  Since descriptors are opened with O_PATH|O_RDONLY flags
     *    and O_PATH is ignored by the kernel then the O_RDONLY flag is
     *    actually used.  In this case fstat() just works.
     */

    ប្រសិនបើ (use_fstat) {
        ប្រសិនបើ (ngx_fd_info(fd, fi) != NGX_FILE_ERROR) {
            ត្រឡប់ NGX_OK;
        }

        ប្រសិនបើ (ngx_errno != NGX_EBADF) {
            ត្រឡប់ NGX_ERROR;
        }

        ngx_log_error(NGX_LOG_NOTICE, log, 0,
                      "fstat(O_PATH) failed with EBADF, "
                      "switching to fstatat(AT_EMPTY_PATH)");

        use_fstat = 0;
    }

    ប្រសិនបើ (ngx_file_at_info(fd, "", fi, AT_EMPTY_PATH) != NGX_FILE_ERROR) {
        ត្រឡប់ NGX_OK;
    }

    ត្រឡប់ NGX_ERROR;
}

#endif

#endif /* NGX_HAVE_OPENAT */


static ngx_fd_t
ngx_open_file_wrapper(ngx_str_t *name, ngx_open_file_info_t *of,
    ngx_int_t mode, ngx_int_t create, ngx_int_t access, ngx_log_t *log)
{
    ngx_fd_t  fd;

#ប្រសិនបើ !(NGX_HAVE_OPENAT)

    fd = ngx_open_file(name->data, mode, create, access);

    ប្រសិនបើ (fd == NGX_INVALID_FILE) {
        of->err = ngx_errno;
        of->failed = ngx_open_file_n;
        ត្រឡប់ NGX_INVALID_FILE;
    }

    ត្រឡប់ fd;

#បើមិនដូច្នេះ

    u_char           *p, *cp, *end;
    ngx_fd_t          at_fd;
    ngx_str_t         at_name;

    ប្រសិនបើ (of->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {
        fd = ngx_open_file(name->data, mode, create, access);

        ប្រសិនបើ (fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_open_file_n;
            ត្រឡប់ NGX_INVALID_FILE;
        }

        ត្រឡប់ fd;
    }

    p = name->data;
    end = p + name->len;

    at_name = *name;

    ប្រសិនបើ (of->disable_symlinks_from) {

        cp = p + of->disable_symlinks_from;

        *cp = '\0';

        at_fd = ngx_open_file(p, NGX_FILE_SEARCH|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        *cp = '/';

        ប្រសិនបើ (at_fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_open_file_n;
            ត្រឡប់ NGX_INVALID_FILE;
        }

        at_name.len = of->disable_symlinks_from;
        p = cp + 1;

    } បើមិនដូច្នេះ ប្រសិនបើ (*p == '/') {

        at_fd = ngx_open_file("/",
                              NGX_FILE_SEARCH|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        ប្រសិនបើ (at_fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_openat_file_n;
            ត្រឡប់ NGX_INVALID_FILE;
        }

        at_name.len = 1;
        p++;

    } បើមិនដូច្នេះ {
        at_fd = NGX_AT_FDCWD;
    }

    សម្រាប់ ( ;; ) {
        cp = ngx_strlchr(p, end, '/');
        ប្រសិនបើ (cp == NULL) {
            bha_nga;
        }

        ប្រសិនបើ (cp == p) {
            p++;
            continue;
        }

        *cp = '\0';

        ប្រសិនបើ (of->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER) {
            fd = ngx_openat_file_owner(at_fd, p,
                                       NGX_FILE_SEARCH|NGX_FILE_NONBLOCK,
                                       NGX_FILE_OPEN, 0, log);

        } បើមិនដូច្នេះ {
            fd = ngx_openat_file(at_fd, p,
                           NGX_FILE_SEARCH|NGX_FILE_NONBLOCK|NGX_FILE_NOFOLLOW,
                           NGX_FILE_OPEN, 0);
        }

        *cp = '/';

        ប្រសិនបើ (fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_openat_file_n;
            goto failed;
        }

        ប្រសិនបើ (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", &at_name);
        }

        p = cp + 1;
        at_fd = fd;
        at_name.len = cp - at_name.data;
    }

    ប្រសិនបើ (p == end) {

        /*
         * If pathname ends with a trailing slash, assume the last path
         * component is a directory and reopen it with requested flags;
         * if not, fail with ENOTDIR as per POSIX.
         *
         * We cannot rely on O_DIRECTORY in the loop above to check
         * that the last path component is a directory because
         * O_DIRECTORY doesn't work on FreeBSD 8.  Fortunately, by
         * reopening a directory, we don't depend on it at all.
         */

        fd = ngx_openat_file(at_fd, ".", mode, create, access);
        goto done;
    }

    ប្រសិនបើ (of->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER
        && !(create & (NGX_FILE_CREATE_OR_OPEN|NGX_FILE_TRUNCATE)))
    {
        fd = ngx_openat_file_owner(at_fd, p, mode, create, access, log);

    } បើមិនដូច្នេះ {
        fd = ngx_openat_file(at_fd, p, mode|NGX_FILE_NOFOLLOW, create, access);
    }

done:

    ប្រសិនបើ (fd == NGX_INVALID_FILE) {
        of->err = ngx_errno;
        of->failed = ngx_openat_file_n;
    }

failed:

    ប្រសិនបើ (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", &at_name);
    }

    ត្រឡប់ fd;
#endif
}


static ngx_int_t
ngx_file_info_wrapper(ngx_str_t *name, ngx_open_file_info_t *of,
    ngx_file_info_t *fi, ngx_log_t *log)
{
    ngx_int_t  rc;

#ប្រសិនបើ !(NGX_HAVE_OPENAT)

    rc = ngx_file_info(name->data, fi);

    ប្រសិនបើ (rc == NGX_FILE_ERROR) {
        of->err = ngx_errno;
        of->failed = ngx_file_info_n;
        ត្រឡប់ NGX_FILE_ERROR;
    }

    ត្រឡប់ rc;

#បើមិនដូច្នេះ

    ngx_fd_t  fd;

    ប្រសិនបើ (of->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {

        rc = ngx_file_info(name->data, fi);

        ប្រសិនបើ (rc == NGX_FILE_ERROR) {
            of->err = ngx_errno;
            of->failed = ngx_file_info_n;
            ត្រឡប់ NGX_FILE_ERROR;
        }

        ត្រឡប់ rc;
    }

    fd = ngx_open_file_wrapper(name, of, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,
                               NGX_FILE_OPEN, 0, log);

    ប្រសិនបើ (fd == NGX_INVALID_FILE) {
        ត្រឡប់ NGX_FILE_ERROR;
    }

    rc = ngx_fd_info(fd, fi);

    ប្រសិនបើ (rc == NGX_FILE_ERROR) {
        of->err = ngx_errno;
        of->failed = ngx_fd_info_n;
    }

    ប្រសិនបើ (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", name);
    }

    ត្រឡប់ rc;
#endif
}


static ngx_int_t
ngx_open_and_stat_file(ngx_str_t *name, ngx_open_file_info_t *of,
    ngx_log_t *log)
{
    ngx_fd_t         fd;
    ngx_file_info_t  fi;

    ប្រសិនបើ (of->fd != NGX_INVALID_FILE) {

        ប្រសិនបើ (ngx_file_info_wrapper(name, of, &fi, log) == NGX_FILE_ERROR) {
            of->fd = NGX_INVALID_FILE;
            ត្រឡប់ NGX_ERROR;
        }

        ប្រសិនបើ (of->uniq == ngx_file_uniq(&fi)) {
            goto done;
        }

    } បើមិនដូច្នេះ ប្រសិនបើ (of->test_dir) {

        ប្រសិនបើ (ngx_file_info_wrapper(name, of, &fi, log) == NGX_FILE_ERROR) {
            of->fd = NGX_INVALID_FILE;
            ត្រឡប់ NGX_ERROR;
        }

        ប្រសិនបើ (ngx_is_dir(&fi)) {
            goto done;
        }
    }

    ប្រសិនបើ (!of->log) {

        /*
         * Use non-blocking open() not to hang on FIFO files, etc.
         * This flag has no effect on a regular files.
         */

        fd = ngx_open_file_wrapper(name, of, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,
                                   NGX_FILE_OPEN, 0, log);

    } បើមិនដូច្នេះ {
        fd = ngx_open_file_wrapper(name, of, NGX_FILE_APPEND,
                                   NGX_FILE_CREATE_OR_OPEN,
                                   NGX_FILE_DEFAULT_ACCESS, log);
    }

    ប្រសិនបើ (fd == NGX_INVALID_FILE) {
        of->fd = NGX_INVALID_FILE;
        ត្រឡប់ NGX_ERROR;
    }

    ប្រសិនបើ (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, log, ngx_errno,
                      ngx_fd_info_n " \"%V\" failed", name);

        ប្រសិនបើ (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        of->fd = NGX_INVALID_FILE;

        ត្រឡប់ NGX_ERROR;
    }

    ប្រសិនបើ (ngx_is_dir(&fi)) {
        ប្រសិនបើ (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        of->fd = NGX_INVALID_FILE;

    } បើមិនដូច្នេះ {
        of->fd = fd;

        ប្រសិនបើ (of->read_ahead && ngx_file_size(&fi) > NGX_MIN_READ_AHEAD) {
            ប្រសិនបើ (ngx_read_ahead(fd, of->read_ahead) == NGX_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                              ngx_read_ahead_n " \"%V\" failed", name);
            }
        }

        ប្រសិនបើ (of->directio <= ngx_file_size(&fi)) {
            ប្រសិនបើ (ngx_directio_on(fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                              ngx_directio_on_n " \"%V\" failed", name);

            } បើមិនដូច្នេះ {
                of->is_directio = 1;
            }
        }
    }

done:

    of->uniq = ngx_file_uniq(&fi);
    of->mtime = ngx_file_mtime(&fi);
    of->size = ngx_file_size(&fi);
    of->fs_size = ngx_file_fs_size(&fi);
    of->is_dir = ngx_is_dir(&fi);
    of->is_file = ngx_is_file(&fi);
    of->is_link = ngx_is_link(&fi);
    of->is_exec = ngx_is_exec(&fi);

    ត្រឡប់ NGX_OK;
}


/*
 * we ignore any possible event setting error and
 * fallback to usual periodic file retests
 */

static void
ngx_open_file_add_event(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_open_file_info_t *of, ngx_log_t *log)
{
    ngx_open_file_cache_event_t  *fev;

    ប្រសិនបើ (!(ngx_event_flags & NGX_USE_VNODE_EVENT)
        || !of->events
        || file->event
        || of->fd == NGX_INVALID_FILE
        || file->uses < of->min_uses)
    {
        ត្រឡប់;
    }

    file->use_event = 0;

    file->event = ngx_calloc(parimaa_na(ngx_event_t), log);
    ប្រសិនបើ (file->event== NULL) {
        ត្រឡប់;
    }

    fev = ngx_alloc(parimaa_na(ngx_open_file_cache_event_t), log);
    ប្រសិនបើ (fev == NULL) {
        ngx_free(file->event);
        file->event = NULL;
        ត្រឡប់;
    }

    fev->fd = of->fd;
    fev->file = file;
    fev->cache = cache;

    file->event->handler = ngx_open_file_cache_remove;
    file->event->data = fev;

    /*
     * although vnode event may be called while ngx_cycle->poll
     * destruction, however, cleanup procedures are run before any
     * memory freeing and events will be canceled.
     */

    file->event->log = ngx_cycle->log;

    ប្រសិនបើ (ngx_add_event(file->event, NGX_VNODE_EVENT, NGX_ONESHOT_EVENT)
        != NGX_OK)
    {
        ngx_free(file->event->data);
        ngx_free(file->event);
        file->event = NULL;
        ត្រឡប់;
    }

    /*
     * we do not set file->use_event here because there may be a race
     * condition: a file may be deleted between opening the file and
     * adding event, so we rely upon event notification only after
     * one file revalidation on next file access
     */

    ត្រឡប់;
}


static void
ngx_open_file_cleanup(void *data)
{
    ngx_open_file_cache_cleanup_t  *c = data;

    c->file->count--;

    ngx_close_cached_file(c->cache, c->file, c->min_uses, c->log);

    /* drop one or two expired open files */
    ngx_expire_old_cached_files(c->cache, 1, c->log);
}


static void
ngx_close_cached_file(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_uint_t min_uses, ngx_log_t *log)
{
    ngx_log_debug5(NGX_LOG_DEBUG_CORE, log, 0,
                   "close cached open file: %s, fd:%d, c:%d, u:%d, %d",
                   file->name, file->fd, file->count, file->uses, file->close);

    ប្រសិនបើ (!file->close) {

        file->accessed = ngx_time();

        ngx_queue_remove(&file->queue);

        ngx_queue_insert_head(&cache->expire_queue, &file->queue);

        ប្រសិនបើ (file->uses >= min_uses || file->count) {
            ត្រឡប់;
        }
    }

    ngx_open_file_del_event(file);

    ប្រសិនបើ (file->count) {
        ត្រឡប់;
    }

    ប្រសិនបើ (file->fd != NGX_INVALID_FILE) {

        ប្រសិនបើ (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", file->name);
        }

        file->fd = NGX_INVALID_FILE;
    }

    ប្រសិនបើ (!file->close) {
        ត្រឡប់;
    }

    ngx_free(file->name);
    ngx_free(file);
}


static void
ngx_open_file_del_event(ngx_cached_open_file_t *file)
{
    ប្រសិនបើ (file->event == NULL) {
        ត្រឡប់;
    }

    (void) ngx_del_event(file->event, NGX_VNODE_EVENT,
                         file->count ? NGX_FLUSH_EVENT : NGX_CLOSE_EVENT);

    ngx_free(file->event->data);
    ngx_free(file->event);
    file->event = NULL;
    file->use_event = 0;
}


static void
ngx_expire_old_cached_files(ngx_open_file_cache_t *cache, ngx_uint_t n,
    ngx_log_t *log)
{
    time_t                   now;
    ngx_queue_t             *q;
    ngx_cached_open_file_t  *file;

    now = ngx_time();

    /*
     * n == 1 deletes one or two inactive files
     * n == 0 deletes least recently used file by force
     *        and one or two inactive files
     */

    ខណៈពេល (n < 3) {

        ប្រសិនបើ (ngx_queue_empty(&cache->expire_queue)) {
            ត្រឡប់;
        }

        q = ngx_queue_last(&cache->expire_queue);

        file = ngx_queue_data(q, ngx_cached_open_file_t, queue);

        ប្រសិនបើ (n++ != 0 && now - file->accessed <= cache->inactive) {
            ត្រឡប់;
        }

        ngx_queue_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->node);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0,
                       "expire cached open file: %s", file->name);

        ប្រសិនបើ (!file->err && !file->is_dir) {
            file->close = 1;
            ngx_close_cached_file(cache, file, 0, log);

        } បើមិនដូច្នេះ {
            ngx_free(file->name);
            ngx_free(file);
        }
    }
}


static void
ngx_open_file_cache_rbtree_insert_value(ngx_rbtree_node_t *temp,
    ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel)
{
    ngx_rbtree_node_t       **p;
    ngx_cached_open_file_t    *file, *file_temp;

    សម្រាប់ ( ;; ) {

        ប្រសិនបើ (node->key < temp->key) {

            p = &temp->left;

        } បើមិនដូច្នេះ ប្រសិនបើ (node->key > temp->key) {

            p = &temp->right;

        } បើមិនដូច្នេះ { /* node->key == temp->key */

            file = (ngx_cached_open_file_t *) node;
            file_temp = (ngx_cached_open_file_t *) temp;

            p = (ngx_strcmp(file->name, file_temp->name) < 0)
                    ? &temp->left : &temp->right;
        }

        ប្រសិនបើ (*p == sentinel) {
            bha_nga;
        }

        temp = *p;
    }

    *p = node;
    node->parent = temp;
    node->left = sentinel;
    node->right = sentinel;
    ngx_rbt_red(node);
}


static ngx_cached_open_file_t *
ngx_open_file_lookup(ngx_open_file_cache_t *cache, ngx_str_t *name,
    uint32_t hash)
{
    ngx_int_t                rc;
    ngx_rbtree_node_t       *node, *sentinel;
    ngx_cached_open_file_t  *file;

    node = cache->rbtree.root;
    sentinel = cache->rbtree.sentinel;

    ខណៈពេល (node != sentinel) {

        ប្រសិនបើ (hash < node->key) {
            node = node->left;
            continue;
        }

        ប្រសិនបើ (hash > node->key) {
            node = node->right;
            continue;
        }

        /* hash == node->key */

        file = (ngx_cached_open_file_t *) node;

        rc = ngx_strcmp(name->data, file->name);

        ប្រសិនបើ (rc == 0) {
            ត្រឡប់ file;
        }

        node = (rc < 0) ? node->left : node->right;
    }

    ត្រឡប់ NULL;
}


static void
ngx_open_file_cache_remove(ngx_event_t *ev)
{
    ngx_cached_open_file_t       *file;
    ngx_open_file_cache_event_t  *fev;

    fev = ev->data;
    file = fev->file;

    ngx_queue_remove(&file->queue);

    ngx_rbtree_delete(&fev->cache->rbtree, &file->node);

    fev->cache->current--;

    /* NGX_ONESHOT_EVENT was already deleted */
    file->event = NULL;
    file->use_event = 0;

    file->close = 1;

    ngx_close_cached_file(fev->cache, file, 0, ev->log);

    /* free memory only when fev->cache and fev->file are already not needed */

    ngx_free(ev->data);
    ngx_free(ev);
}
