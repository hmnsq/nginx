
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>
#അന്തര്ഭാവ <ngx_event.h>


/*
 * open file cache caches
 *    open file handles with stat() info;
 *    directories stat() info;
 *    files and directories errors: not found, access denied, etc.
 */


#define NGX_MIN_READ_AHEAD  (128 * 1024)


സ്ഥിതം ശൂന്യം ngx_open_file_cache_cleanup(ശൂന്യം *data);
#എങ്കിൽ (NGX_HAVE_OPENAT)
സ്ഥിതം ngx_fd_t ngx_openat_file_owner(ngx_fd_t at_fd, സ്ഥിരം u_char *name,
    ngx_int_t mode, ngx_int_t create, ngx_int_t access, ngx_log_t *log);
#എങ്കിൽ (NGX_HAVE_O_PATH)
സ്ഥിതം ngx_int_t ngx_file_o_path_info(ngx_fd_t fd, ngx_file_info_t *fi,
    ngx_log_t *log);
#endif
#endif
സ്ഥിതം ngx_fd_t ngx_open_file_wrapper(ngx_str_t *name,
    ngx_open_file_info_t *of, ngx_int_t mode, ngx_int_t create,
    ngx_int_t access, ngx_log_t *log);
സ്ഥിതം ngx_int_t ngx_file_info_wrapper(ngx_str_t *name,
    ngx_open_file_info_t *of, ngx_file_info_t *fi, ngx_log_t *log);
സ്ഥിതം ngx_int_t ngx_open_and_stat_file(ngx_str_t *name,
    ngx_open_file_info_t *of, ngx_log_t *log);
സ്ഥിതം ശൂന്യം ngx_open_file_add_event(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_open_file_info_t *of, ngx_log_t *log);
സ്ഥിതം ശൂന്യം ngx_open_file_cleanup(ശൂന്യം *data);
സ്ഥിതം ശൂന്യം ngx_close_cached_file(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_uint_t min_uses, ngx_log_t *log);
സ്ഥിതം ശൂന്യം ngx_open_file_del_event(ngx_cached_open_file_t *file);
സ്ഥിതം ശൂന്യം ngx_expire_old_cached_files(ngx_open_file_cache_t *cache,
    ngx_uint_t n, ngx_log_t *log);
സ്ഥിതം ശൂന്യം ngx_open_file_cache_rbtree_തിരുകുക_മൂല്യം(ngx_rbtree_കണ്ണി_t *temp,
    ngx_rbtree_കണ്ണി_t *കണ്ണി, ngx_rbtree_കണ്ണി_t *sentinel);
സ്ഥിതം ngx_cached_open_file_t *
    ngx_open_file_lookup(ngx_open_file_cache_t *cache, ngx_str_t *name,
    uint32_t hash);
സ്ഥിതം ശൂന്യം ngx_open_file_cache_remove(ngx_event_t *ev);


ngx_open_file_cache_t *
ngx_open_file_cache_init(ngx_pool_t *pool, ngx_uint_t max, time_t inactive)
{
    ngx_pool_cleanup_t     *cln;
    ngx_open_file_cache_t  *cache;

    cache = ngx_palloc(pool, വലുപ്പം(ngx_open_file_cache_t));
    എങ്കിൽ (cache == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    ngx_rbtree_init(&cache->rbtree, &cache->sentinel,
                    ngx_open_file_cache_rbtree_തിരുകുക_മൂല്യം);

    ngx_queue_init(&cache->expire_queue);

    cache->current = 0;
    cache->max = max;
    cache->inactive = inactive;

    cln = ngx_pool_cleanup_add(pool, 0);
    എങ്കിൽ (cln == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    cln->handler = ngx_open_file_cache_cleanup;
    cln->data = cache;

    തിരികെ cache;
}


സ്ഥിതം ശൂന്യം
ngx_open_file_cache_cleanup(ശൂന്യം *data)
{
    ngx_open_file_cache_t  *cache = data;

    ngx_queue_t             *q;
    ngx_cached_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "open file cache cleanup");

    വേണ്ടി ( ;; ) {

        എങ്കിൽ (ngx_queue_empty(&cache->expire_queue)) {
            നിർത്തുക;
        }

        q = ngx_queue_last(&cache->expire_queue);

        file = ngx_queue_data(q, ngx_cached_open_file_t, queue);

        ngx_queue_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->കണ്ണി);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                       "delete cached open file: %s", file->name);

        എങ്കിൽ (!file->err && !file->is_dir) {
            file->close = 1;
            file->എണ്ണം = 0;
            ngx_close_cached_file(cache, file, 0, ngx_cycle->log);

        } അല്ലെങ്കിൽ {
            ngx_free(file->name);
            ngx_free(file);
        }
    }

    എങ്കിൽ (cache->current) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0,
                      "%ui items still left in open file cache",
                      cache->current);
    }

    എങ്കിൽ (cache->rbtree.വേര് != cache->rbtree.sentinel) {
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

    എങ്കിൽ (cache == ശൂന്യ) {

        എങ്കിൽ (of->test_only) {

            എങ്കിൽ (ngx_file_info_wrapper(name, of, &fi, pool->log)
                == NGX_FILE_ERROR)
            {
                തിരികെ NGX_ERROR;
            }

            of->uniq = ngx_file_uniq(&fi);
            of->mtime = ngx_file_mtime(&fi);
            of->size = ngx_file_size(&fi);
            of->fs_size = ngx_file_fs_size(&fi);
            of->is_dir = ngx_is_dir(&fi);
            of->is_file = ngx_is_file(&fi);
            of->is_link = ngx_is_link(&fi);
            of->is_exec = ngx_is_exec(&fi);

            തിരികെ NGX_OK;
        }

        cln = ngx_pool_cleanup_add(pool, വലുപ്പം(ngx_pool_cleanup_file_t));
        എങ്കിൽ (cln == ശൂന്യ) {
            തിരികെ NGX_ERROR;
        }

        rc = ngx_open_and_stat_file(name, of, pool->log);

        എങ്കിൽ (rc == NGX_OK && !of->is_dir) {
            cln->handler = ngx_pool_cleanup_file;
            clnf = cln->data;

            clnf->fd = of->fd;
            clnf->name = name->data;
            clnf->log = pool->log;
        }

        തിരികെ rc;
    }

    cln = ngx_pool_cleanup_add(pool, വലുപ്പം(ngx_open_file_cache_cleanup_t));
    എങ്കിൽ (cln == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    now = ngx_time();

    hash = ngx_crc32_long(name->data, name->len);

    file = ngx_open_file_lookup(cache, name, hash);

    എങ്കിൽ (file) {

        file->uses++;

        ngx_queue_remove(&file->queue);

        എങ്കിൽ (file->fd == NGX_INVALID_FILE && file->err == 0 && !file->is_dir) {

            /* file was not used often enough to keep open */

            rc = ngx_open_and_stat_file(name, of, pool->log);

            എങ്കിൽ (rc != NGX_OK && (of->err == 0 || !of->errors)) {
                പോകുക failed;
            }

            പോകുക add_event;
        }

        എങ്കിൽ (file->use_event
            || (file->event == ശൂന്യ
                && (of->uniq == 0 || of->uniq == file->uniq)
                && now - file->created < of->valid
#എങ്കിൽ (NGX_HAVE_OPENAT)
                && of->disable_symlinks == file->disable_symlinks
                && of->disable_symlinks_from == file->disable_symlinks_from
#endif
            ))
        {
            എങ്കിൽ (file->err == 0) {

                of->fd = file->fd;
                of->uniq = file->uniq;
                of->mtime = file->mtime;
                of->size = file->size;

                of->is_dir = file->is_dir;
                of->is_file = file->is_file;
                of->is_link = file->is_link;
                of->is_exec = file->is_exec;
                of->is_directio = file->is_directio;

                എങ്കിൽ (!file->is_dir) {
                    file->എണ്ണം++;
                    ngx_open_file_add_event(cache, file, of, pool->log);
                }

            } അല്ലെങ്കിൽ {
                of->err = file->err;
#എങ്കിൽ (NGX_HAVE_OPENAT)
                of->failed = file->disable_symlinks ? ngx_openat_file_n
                                                    : ngx_open_file_n;
#അല്ലെങ്കിൽ
                of->failed = ngx_open_file_n;
#endif
            }

            പോകുക found;
        }

        ngx_log_debug4(NGX_LOG_DEBUG_CORE, pool->log, 0,
                       "retest open file: %s, fd:%d, c:%d, e:%d",
                       file->name, file->fd, file->എണ്ണം, file->err);

        എങ്കിൽ (file->is_dir) {

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

        എങ്കിൽ (rc != NGX_OK && (of->err == 0 || !of->errors)) {
            പോകുക failed;
        }

        എങ്കിൽ (of->is_dir) {

            എങ്കിൽ (file->is_dir || file->err) {
                പോകുക update;
            }

            /* file became directory */

        } അല്ലെങ്കിൽ എങ്കിൽ (of->err == 0) {  /* file */

            എങ്കിൽ (file->is_dir || file->err) {
                പോകുക add_event;
            }

            എങ്കിൽ (of->uniq == file->uniq) {

                എങ്കിൽ (file->event) {
                    file->use_event = 1;
                }

                of->is_directio = file->is_directio;

                പോകുക update;
            }

            /* file was changed */

        } അല്ലെങ്കിൽ { /* error to cache */

            എങ്കിൽ (file->err || file->is_dir) {
                പോകുക update;
            }

            /* file was removed, etc. */
        }

        എങ്കിൽ (file->എണ്ണം == 0) {

            ngx_open_file_del_event(file);

            എങ്കിൽ (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, pool->log, ngx_errno,
                              ngx_close_file_n " \"%V\" failed", name);
            }

            പോകുക add_event;
        }

        ngx_rbtree_delete(&cache->rbtree, &file->കണ്ണി);

        cache->current--;

        file->close = 1;

        പോകുക create;
    }

    /* not found */

    rc = ngx_open_and_stat_file(name, of, pool->log);

    എങ്കിൽ (rc != NGX_OK && (of->err == 0 || !of->errors)) {
        പോകുക failed;
    }

create:

    എങ്കിൽ (cache->current >= cache->max) {
        ngx_expire_old_cached_files(cache, 0, pool->log);
    }

    file = ngx_alloc(വലുപ്പം(ngx_cached_open_file_t), pool->log);

    എങ്കിൽ (file == ശൂന്യ) {
        പോകുക failed;
    }

    file->name = ngx_alloc(name->len + 1, pool->log);

    എങ്കിൽ (file->name == ശൂന്യ) {
        ngx_free(file);
        file = ശൂന്യ;
        പോകുക failed;
    }

    ngx_cpystrn(file->name, name->data, name->len + 1);

    file->കണ്ണി.താക്കോൽ = hash;

    ngx_rbtree_തിരുകുക(&cache->rbtree, &file->കണ്ണി);

    cache->current++;

    file->uses = 1;
    file->എണ്ണം = 0;
    file->use_event = 0;
    file->event = ശൂന്യ;

add_event:

    ngx_open_file_add_event(cache, file, of, pool->log);

update:

    file->fd = of->fd;
    file->err = of->err;
#എങ്കിൽ (NGX_HAVE_OPENAT)
    file->disable_symlinks = of->disable_symlinks;
    file->disable_symlinks_from = of->disable_symlinks_from;
#endif

    എങ്കിൽ (of->err == 0) {
        file->uniq = of->uniq;
        file->mtime = of->mtime;
        file->size = of->size;

        file->close = 0;

        file->is_dir = of->is_dir;
        file->is_file = of->is_file;
        file->is_link = of->is_link;
        file->is_exec = of->is_exec;
        file->is_directio = of->is_directio;

        എങ്കിൽ (!of->is_dir) {
            file->എണ്ണം++;
        }
    }

    file->created = now;

found:

    file->accessed = now;

    ngx_queue_തിരുകുക_head(&cache->expire_queue, &file->queue);

    ngx_log_debug5(NGX_LOG_DEBUG_CORE, pool->log, 0,
                   "cached open file: %s, fd:%d, c:%d, e:%d, u:%d",
                   file->name, file->fd, file->എണ്ണം, file->err, file->uses);

    എങ്കിൽ (of->err == 0) {

        എങ്കിൽ (!of->is_dir) {
            cln->handler = ngx_open_file_cleanup;
            ofcln = cln->data;

            ofcln->cache = cache;
            ofcln->file = file;
            ofcln->min_uses = of->min_uses;
            ofcln->log = pool->log;
        }

        തിരികെ NGX_OK;
    }

    തിരികെ NGX_ERROR;

failed:

    എങ്കിൽ (file) {
        ngx_rbtree_delete(&cache->rbtree, &file->കണ്ണി);

        cache->current--;

        എങ്കിൽ (file->എണ്ണം == 0) {

            എങ്കിൽ (file->fd != NGX_INVALID_FILE) {
                എങ്കിൽ (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_ALERT, pool->log, ngx_errno,
                                  ngx_close_file_n " \"%s\" failed",
                                  file->name);
                }
            }

            ngx_free(file->name);
            ngx_free(file);

        } അല്ലെങ്കിൽ {
            file->close = 1;
        }
    }

    എങ്കിൽ (of->fd != NGX_INVALID_FILE) {
        എങ്കിൽ (ngx_close_file(of->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, pool->log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }
    }

    തിരികെ NGX_ERROR;
}


#എങ്കിൽ (NGX_HAVE_OPENAT)

സ്ഥിതം ngx_fd_t
ngx_openat_file_owner(ngx_fd_t at_fd, സ്ഥിരം u_char *name,
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

    എങ്കിൽ (fd == NGX_INVALID_FILE) {
        തിരികെ NGX_INVALID_FILE;
    }

    എങ്കിൽ (ngx_file_at_info(at_fd, name, &atfi, AT_SYMLINK_NOFOLLOW)
        == NGX_FILE_ERROR)
    {
        err = ngx_errno;
        പോകുക failed;
    }

#എങ്കിൽ (NGX_HAVE_O_PATH)
    എങ്കിൽ (ngx_file_o_path_info(fd, &fi, log) == NGX_ERROR) {
        err = ngx_errno;
        പോകുക failed;
    }
#അല്ലെങ്കിൽ
    എങ്കിൽ (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        err = ngx_errno;
        പോകുക failed;
    }
#endif

    എങ്കിൽ (fi.st_uid != atfi.st_uid) {
        err = NGX_ELOOP;
        പോകുക failed;
    }

    തിരികെ fd;

failed:

    എങ്കിൽ (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", name);
    }

    ngx_set_errno(err);

    തിരികെ NGX_INVALID_FILE;
}


#എങ്കിൽ (NGX_HAVE_O_PATH)

സ്ഥിതം ngx_int_t
ngx_file_o_path_info(ngx_fd_t fd, ngx_file_info_t *fi, ngx_log_t *log)
{
    സ്ഥിതം ngx_uint_t  use_fstat = 1;

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

    എങ്കിൽ (use_fstat) {
        എങ്കിൽ (ngx_fd_info(fd, fi) != NGX_FILE_ERROR) {
            തിരികെ NGX_OK;
        }

        എങ്കിൽ (ngx_errno != NGX_EBADF) {
            തിരികെ NGX_ERROR;
        }

        ngx_log_error(NGX_LOG_NOTICE, log, 0,
                      "fstat(O_PATH) failed with EBADF, "
                      "switching to fstatat(AT_EMPTY_PATH)");

        use_fstat = 0;
    }

    എങ്കിൽ (ngx_file_at_info(fd, "", fi, AT_EMPTY_PATH) != NGX_FILE_ERROR) {
        തിരികെ NGX_OK;
    }

    തിരികെ NGX_ERROR;
}

#endif

#endif /* NGX_HAVE_OPENAT */


സ്ഥിതം ngx_fd_t
ngx_open_file_wrapper(ngx_str_t *name, ngx_open_file_info_t *of,
    ngx_int_t mode, ngx_int_t create, ngx_int_t access, ngx_log_t *log)
{
    ngx_fd_t  fd;

#എങ്കിൽ !(NGX_HAVE_OPENAT)

    fd = ngx_open_file(name->data, mode, create, access);

    എങ്കിൽ (fd == NGX_INVALID_FILE) {
        of->err = ngx_errno;
        of->failed = ngx_open_file_n;
        തിരികെ NGX_INVALID_FILE;
    }

    തിരികെ fd;

#അല്ലെങ്കിൽ

    u_char           *p, *cp, *end;
    ngx_fd_t          at_fd;
    ngx_str_t         at_name;

    എങ്കിൽ (of->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {
        fd = ngx_open_file(name->data, mode, create, access);

        എങ്കിൽ (fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_open_file_n;
            തിരികെ NGX_INVALID_FILE;
        }

        തിരികെ fd;
    }

    p = name->data;
    end = p + name->len;

    at_name = *name;

    എങ്കിൽ (of->disable_symlinks_from) {

        cp = p + of->disable_symlinks_from;

        *cp = '\0';

        at_fd = ngx_open_file(p, NGX_FILE_തിരയൽ|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        *cp = '/';

        എങ്കിൽ (at_fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_open_file_n;
            തിരികെ NGX_INVALID_FILE;
        }

        at_name.len = of->disable_symlinks_from;
        p = cp + 1;

    } അല്ലെങ്കിൽ എങ്കിൽ (*p == '/') {

        at_fd = ngx_open_file("/",
                              NGX_FILE_തിരയൽ|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        എങ്കിൽ (at_fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_openat_file_n;
            തിരികെ NGX_INVALID_FILE;
        }

        at_name.len = 1;
        p++;

    } അല്ലെങ്കിൽ {
        at_fd = NGX_AT_FDCWD;
    }

    വേണ്ടി ( ;; ) {
        cp = ngx_strlchr(p, end, '/');
        എങ്കിൽ (cp == ശൂന്യ) {
            നിർത്തുക;
        }

        എങ്കിൽ (cp == p) {
            p++;
            തുടരുക;
        }

        *cp = '\0';

        എങ്കിൽ (of->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER) {
            fd = ngx_openat_file_owner(at_fd, p,
                                       NGX_FILE_തിരയൽ|NGX_FILE_NONBLOCK,
                                       NGX_FILE_OPEN, 0, log);

        } അല്ലെങ്കിൽ {
            fd = ngx_openat_file(at_fd, p,
                           NGX_FILE_തിരയൽ|NGX_FILE_NONBLOCK|NGX_FILE_NOFOLLOW,
                           NGX_FILE_OPEN, 0);
        }

        *cp = '/';

        എങ്കിൽ (fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_openat_file_n;
            പോകുക failed;
        }

        എങ്കിൽ (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", &at_name);
        }

        p = cp + 1;
        at_fd = fd;
        at_name.len = cp - at_name.data;
    }

    എങ്കിൽ (p == end) {

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
        പോകുക done;
    }

    എങ്കിൽ (of->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER
        && !(create & (NGX_FILE_CREATE_OR_OPEN|NGX_FILE_TRUNCATE)))
    {
        fd = ngx_openat_file_owner(at_fd, p, mode, create, access, log);

    } അല്ലെങ്കിൽ {
        fd = ngx_openat_file(at_fd, p, mode|NGX_FILE_NOFOLLOW, create, access);
    }

done:

    എങ്കിൽ (fd == NGX_INVALID_FILE) {
        of->err = ngx_errno;
        of->failed = ngx_openat_file_n;
    }

failed:

    എങ്കിൽ (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", &at_name);
    }

    തിരികെ fd;
#endif
}


സ്ഥിതം ngx_int_t
ngx_file_info_wrapper(ngx_str_t *name, ngx_open_file_info_t *of,
    ngx_file_info_t *fi, ngx_log_t *log)
{
    ngx_int_t  rc;

#എങ്കിൽ !(NGX_HAVE_OPENAT)

    rc = ngx_file_info(name->data, fi);

    എങ്കിൽ (rc == NGX_FILE_ERROR) {
        of->err = ngx_errno;
        of->failed = ngx_file_info_n;
        തിരികെ NGX_FILE_ERROR;
    }

    തിരികെ rc;

#അല്ലെങ്കിൽ

    ngx_fd_t  fd;

    എങ്കിൽ (of->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {

        rc = ngx_file_info(name->data, fi);

        എങ്കിൽ (rc == NGX_FILE_ERROR) {
            of->err = ngx_errno;
            of->failed = ngx_file_info_n;
            തിരികെ NGX_FILE_ERROR;
        }

        തിരികെ rc;
    }

    fd = ngx_open_file_wrapper(name, of, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,
                               NGX_FILE_OPEN, 0, log);

    എങ്കിൽ (fd == NGX_INVALID_FILE) {
        തിരികെ NGX_FILE_ERROR;
    }

    rc = ngx_fd_info(fd, fi);

    എങ്കിൽ (rc == NGX_FILE_ERROR) {
        of->err = ngx_errno;
        of->failed = ngx_fd_info_n;
    }

    എങ്കിൽ (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", name);
    }

    തിരികെ rc;
#endif
}


സ്ഥിതം ngx_int_t
ngx_open_and_stat_file(ngx_str_t *name, ngx_open_file_info_t *of,
    ngx_log_t *log)
{
    ngx_fd_t         fd;
    ngx_file_info_t  fi;

    എങ്കിൽ (of->fd != NGX_INVALID_FILE) {

        എങ്കിൽ (ngx_file_info_wrapper(name, of, &fi, log) == NGX_FILE_ERROR) {
            of->fd = NGX_INVALID_FILE;
            തിരികെ NGX_ERROR;
        }

        എങ്കിൽ (of->uniq == ngx_file_uniq(&fi)) {
            പോകുക done;
        }

    } അല്ലെങ്കിൽ എങ്കിൽ (of->test_dir) {

        എങ്കിൽ (ngx_file_info_wrapper(name, of, &fi, log) == NGX_FILE_ERROR) {
            of->fd = NGX_INVALID_FILE;
            തിരികെ NGX_ERROR;
        }

        എങ്കിൽ (ngx_is_dir(&fi)) {
            പോകുക done;
        }
    }

    എങ്കിൽ (!of->log) {

        /*
         * Use non-blocking open() not to hang on FIFO files, etc.
         * This flag has no effect on a regular files.
         */

        fd = ngx_open_file_wrapper(name, of, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,
                                   NGX_FILE_OPEN, 0, log);

    } അല്ലെങ്കിൽ {
        fd = ngx_open_file_wrapper(name, of, NGX_FILE_APPEND,
                                   NGX_FILE_CREATE_OR_OPEN,
                                   NGX_FILE_DEFAULT_ACCESS, log);
    }

    എങ്കിൽ (fd == NGX_INVALID_FILE) {
        of->fd = NGX_INVALID_FILE;
        തിരികെ NGX_ERROR;
    }

    എങ്കിൽ (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, log, ngx_errno,
                      ngx_fd_info_n " \"%V\" failed", name);

        എങ്കിൽ (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        of->fd = NGX_INVALID_FILE;

        തിരികെ NGX_ERROR;
    }

    എങ്കിൽ (ngx_is_dir(&fi)) {
        എങ്കിൽ (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        of->fd = NGX_INVALID_FILE;

    } അല്ലെങ്കിൽ {
        of->fd = fd;

        എങ്കിൽ (of->read_ahead && ngx_file_size(&fi) > NGX_MIN_READ_AHEAD) {
            എങ്കിൽ (ngx_read_ahead(fd, of->read_ahead) == NGX_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                              ngx_read_ahead_n " \"%V\" failed", name);
            }
        }

        എങ്കിൽ (of->directio <= ngx_file_size(&fi)) {
            എങ്കിൽ (ngx_directio_on(fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                              ngx_directio_on_n " \"%V\" failed", name);

            } അല്ലെങ്കിൽ {
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

    തിരികെ NGX_OK;
}


/*
 * we ignore any possible event setting error and
 * fallback to usual periodic file retests
 */

സ്ഥിതം ശൂന്യം
ngx_open_file_add_event(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_open_file_info_t *of, ngx_log_t *log)
{
    ngx_open_file_cache_event_t  *fev;

    എങ്കിൽ (!(ngx_event_flags & NGX_USE_VNODE_EVENT)
        || !of->events
        || file->event
        || of->fd == NGX_INVALID_FILE
        || file->uses < of->min_uses)
    {
        തിരികെ;
    }

    file->use_event = 0;

    file->event = ngx_calloc(വലുപ്പം(ngx_event_t), log);
    എങ്കിൽ (file->event== ശൂന്യ) {
        തിരികെ;
    }

    fev = ngx_alloc(വലുപ്പം(ngx_open_file_cache_event_t), log);
    എങ്കിൽ (fev == ശൂന്യ) {
        ngx_free(file->event);
        file->event = ശൂന്യ;
        തിരികെ;
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

    എങ്കിൽ (ngx_add_event(file->event, NGX_VNODE_EVENT, NGX_ONESHOT_EVENT)
        != NGX_OK)
    {
        ngx_free(file->event->data);
        ngx_free(file->event);
        file->event = ശൂന്യ;
        തിരികെ;
    }

    /*
     * we do not set file->use_event here because there may be a race
     * condition: a file may be deleted between opening the file and
     * adding event, so we rely upon event notification only after
     * one file revalidation on next file access
     */

    തിരികെ;
}


സ്ഥിതം ശൂന്യം
ngx_open_file_cleanup(ശൂന്യം *data)
{
    ngx_open_file_cache_cleanup_t  *c = data;

    c->file->എണ്ണം--;

    ngx_close_cached_file(c->cache, c->file, c->min_uses, c->log);

    /* drop one or two expired open files */
    ngx_expire_old_cached_files(c->cache, 1, c->log);
}


സ്ഥിതം ശൂന്യം
ngx_close_cached_file(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_uint_t min_uses, ngx_log_t *log)
{
    ngx_log_debug5(NGX_LOG_DEBUG_CORE, log, 0,
                   "close cached open file: %s, fd:%d, c:%d, u:%d, %d",
                   file->name, file->fd, file->എണ്ണം, file->uses, file->close);

    എങ്കിൽ (!file->close) {

        file->accessed = ngx_time();

        ngx_queue_remove(&file->queue);

        ngx_queue_തിരുകുക_head(&cache->expire_queue, &file->queue);

        എങ്കിൽ (file->uses >= min_uses || file->എണ്ണം) {
            തിരികെ;
        }
    }

    ngx_open_file_del_event(file);

    എങ്കിൽ (file->എണ്ണം) {
        തിരികെ;
    }

    എങ്കിൽ (file->fd != NGX_INVALID_FILE) {

        എങ്കിൽ (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", file->name);
        }

        file->fd = NGX_INVALID_FILE;
    }

    എങ്കിൽ (!file->close) {
        തിരികെ;
    }

    ngx_free(file->name);
    ngx_free(file);
}


സ്ഥിതം ശൂന്യം
ngx_open_file_del_event(ngx_cached_open_file_t *file)
{
    എങ്കിൽ (file->event == ശൂന്യ) {
        തിരികെ;
    }

    (ശൂന്യം) ngx_del_event(file->event, NGX_VNODE_EVENT,
                         file->എണ്ണം ? NGX_FLUSH_EVENT : NGX_CLOSE_EVENT);

    ngx_free(file->event->data);
    ngx_free(file->event);
    file->event = ശൂന്യ;
    file->use_event = 0;
}


സ്ഥിതം ശൂന്യം
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

    ആകുമ്പോൾ (n < 3) {

        എങ്കിൽ (ngx_queue_empty(&cache->expire_queue)) {
            തിരികെ;
        }

        q = ngx_queue_last(&cache->expire_queue);

        file = ngx_queue_data(q, ngx_cached_open_file_t, queue);

        എങ്കിൽ (n++ != 0 && now - file->accessed <= cache->inactive) {
            തിരികെ;
        }

        ngx_queue_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->കണ്ണി);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0,
                       "expire cached open file: %s", file->name);

        എങ്കിൽ (!file->err && !file->is_dir) {
            file->close = 1;
            ngx_close_cached_file(cache, file, 0, log);

        } അല്ലെങ്കിൽ {
            ngx_free(file->name);
            ngx_free(file);
        }
    }
}


സ്ഥിതം ശൂന്യം
ngx_open_file_cache_rbtree_തിരുകുക_മൂല്യം(ngx_rbtree_കണ്ണി_t *temp,
    ngx_rbtree_കണ്ണി_t *കണ്ണി, ngx_rbtree_കണ്ണി_t *sentinel)
{
    ngx_rbtree_കണ്ണി_t       **p;
    ngx_cached_open_file_t    *file, *file_temp;

    വേണ്ടി ( ;; ) {

        എങ്കിൽ (കണ്ണി->താക്കോൽ < temp->താക്കോൽ) {

            p = &temp->ഇടത്;

        } അല്ലെങ്കിൽ എങ്കിൽ (കണ്ണി->താക്കോൽ > temp->താക്കോൽ) {

            p = &temp->വലത്;

        } അല്ലെങ്കിൽ { /* node->key == temp->key */

            file = (ngx_cached_open_file_t *) കണ്ണി;
            file_temp = (ngx_cached_open_file_t *) temp;

            p = (ngx_strcmp(file->name, file_temp->name) < 0)
                    ? &temp->ഇടത് : &temp->വലത്;
        }

        എങ്കിൽ (*p == sentinel) {
            നിർത്തുക;
        }

        temp = *p;
    }

    *p = കണ്ണി;
    കണ്ണി->parent = temp;
    കണ്ണി->ഇടത് = sentinel;
    കണ്ണി->വലത് = sentinel;
    ngx_rbt_red(കണ്ണി);
}


സ്ഥിതം ngx_cached_open_file_t *
ngx_open_file_lookup(ngx_open_file_cache_t *cache, ngx_str_t *name,
    uint32_t hash)
{
    ngx_int_t                rc;
    ngx_rbtree_കണ്ണി_t       *കണ്ണി, *sentinel;
    ngx_cached_open_file_t  *file;

    കണ്ണി = cache->rbtree.വേര്;
    sentinel = cache->rbtree.sentinel;

    ആകുമ്പോൾ (കണ്ണി != sentinel) {

        എങ്കിൽ (hash < കണ്ണി->താക്കോൽ) {
            കണ്ണി = കണ്ണി->ഇടത്;
            തുടരുക;
        }

        എങ്കിൽ (hash > കണ്ണി->താക്കോൽ) {
            കണ്ണി = കണ്ണി->വലത്;
            തുടരുക;
        }

        /* hash == node->key */

        file = (ngx_cached_open_file_t *) കണ്ണി;

        rc = ngx_strcmp(name->data, file->name);

        എങ്കിൽ (rc == 0) {
            തിരികെ file;
        }

        കണ്ണി = (rc < 0) ? കണ്ണി->ഇടത് : കണ്ണി->വലത്;
    }

    തിരികെ ശൂന്യ;
}


സ്ഥിതം ശൂന്യം
ngx_open_file_cache_remove(ngx_event_t *ev)
{
    ngx_cached_open_file_t       *file;
    ngx_open_file_cache_event_t  *fev;

    fev = ev->data;
    file = fev->file;

    ngx_queue_remove(&file->queue);

    ngx_rbtree_delete(&fev->cache->rbtree, &file->കണ്ണി);

    fev->cache->current--;

    /* NGX_ONESHOT_EVENT was already deleted */
    file->event = ശൂന്യ;
    file->use_event = 0;

    file->close = 1;

    ngx_close_cached_file(fev->cache, file, 0, ev->log);

    /* free memory only when fev->cache and fev->file are already not needed */

    ngx_free(ev->data);
    ngx_free(ev);
}
