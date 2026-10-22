
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>
#அந்தர்பாவ <ngx_event.h>


/*
 * open file cache caches
 *    open file handles with stat() info;
 *    directories stat() info;
 *    files and directories errors: not found, access denied, etc.
 */


#define NGX_MIN_READ_AHEAD  (128 * 1024)


நிலையான வெறுமை ngx_open_file_cache_cleanup(வெறுமை *data);
#எனில் (NGX_HAVE_OPENAT)
நிலையான ngx_fd_t ngx_openat_file_owner(ngx_fd_t at_fd, மாறிலி u_char *name,
    ngx_int_t mode, ngx_int_t create, ngx_int_t access, ngx_log_t *log);
#எனில் (NGX_HAVE_O_PATH)
நிலையான ngx_int_t ngx_file_o_path_info(ngx_fd_t fd, ngx_file_info_t *fi,
    ngx_log_t *log);
#endif
#endif
நிலையான ngx_fd_t ngx_open_file_wrapper(ngx_str_t *name,
    ngx_open_file_info_t *of, ngx_int_t mode, ngx_int_t create,
    ngx_int_t access, ngx_log_t *log);
நிலையான ngx_int_t ngx_file_info_wrapper(ngx_str_t *name,
    ngx_open_file_info_t *of, ngx_file_info_t *fi, ngx_log_t *log);
நிலையான ngx_int_t ngx_open_and_stat_file(ngx_str_t *name,
    ngx_open_file_info_t *of, ngx_log_t *log);
நிலையான வெறுமை ngx_open_file_add_event(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_open_file_info_t *of, ngx_log_t *log);
நிலையான வெறுமை ngx_open_file_cleanup(வெறுமை *data);
நிலையான வெறுமை ngx_close_cached_file(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_uint_t min_uses, ngx_log_t *log);
நிலையான வெறுமை ngx_open_file_del_event(ngx_cached_open_file_t *file);
நிலையான வெறுமை ngx_expire_old_cached_files(ngx_open_file_cache_t *cache,
    ngx_uint_t n, ngx_log_t *log);
நிலையான வெறுமை ngx_open_file_cache_rbtree_செருகு_மதிப்பு(ngx_rbtree_முனை_t *temp,
    ngx_rbtree_முனை_t *முனை, ngx_rbtree_முனை_t *sentinel);
நிலையான ngx_cached_open_file_t *
    ngx_open_file_lookup(ngx_open_file_cache_t *cache, ngx_str_t *name,
    uint32_t hash);
நிலையான வெறுமை ngx_open_file_cache_remove(ngx_event_t *ev);


ngx_open_file_cache_t *
ngx_open_file_cache_init(ngx_pool_t *pool, ngx_uint_t max, time_t inactive)
{
    ngx_pool_cleanup_t     *cln;
    ngx_open_file_cache_t  *cache;

    cache = ngx_palloc(pool, அளவு(ngx_open_file_cache_t));
    எனில் (cache == வெற்று) {
        திரும்பு வெற்று;
    }

    ngx_rbtree_init(&cache->rbtree, &cache->sentinel,
                    ngx_open_file_cache_rbtree_செருகு_மதிப்பு);

    ngx_queue_init(&cache->expire_queue);

    cache->current = 0;
    cache->max = max;
    cache->inactive = inactive;

    cln = ngx_pool_cleanup_add(pool, 0);
    எனில் (cln == வெற்று) {
        திரும்பு வெற்று;
    }

    cln->handler = ngx_open_file_cache_cleanup;
    cln->data = cache;

    திரும்பு cache;
}


நிலையான வெறுமை
ngx_open_file_cache_cleanup(வெறுமை *data)
{
    ngx_open_file_cache_t  *cache = data;

    ngx_queue_t             *q;
    ngx_cached_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "open file cache cleanup");

    க்கு ( ;; ) {

        எனில் (ngx_queue_empty(&cache->expire_queue)) {
            நிறுத்து;
        }

        q = ngx_queue_last(&cache->expire_queue);

        file = ngx_queue_data(q, ngx_cached_open_file_t, queue);

        ngx_queue_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->முனை);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                       "delete cached open file: %s", file->name);

        எனில் (!file->err && !file->is_dir) {
            file->close = 1;
            file->மொத்தம் = 0;
            ngx_close_cached_file(cache, file, 0, ngx_cycle->log);

        } இல்லையெனில் {
            ngx_free(file->name);
            ngx_free(file);
        }
    }

    எனில் (cache->current) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0,
                      "%ui items still left in open file cache",
                      cache->current);
    }

    எனில் (cache->rbtree.வேர் != cache->rbtree.sentinel) {
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

    எனில் (cache == வெற்று) {

        எனில் (of->test_only) {

            எனில் (ngx_file_info_wrapper(name, of, &fi, pool->log)
                == NGX_FILE_ERROR)
            {
                திரும்பு NGX_ERROR;
            }

            of->uniq = ngx_file_uniq(&fi);
            of->mtime = ngx_file_mtime(&fi);
            of->size = ngx_file_size(&fi);
            of->fs_size = ngx_file_fs_size(&fi);
            of->is_dir = ngx_is_dir(&fi);
            of->is_file = ngx_is_file(&fi);
            of->is_link = ngx_is_link(&fi);
            of->is_exec = ngx_is_exec(&fi);

            திரும்பு NGX_OK;
        }

        cln = ngx_pool_cleanup_add(pool, அளவு(ngx_pool_cleanup_file_t));
        எனில் (cln == வெற்று) {
            திரும்பு NGX_ERROR;
        }

        rc = ngx_open_and_stat_file(name, of, pool->log);

        எனில் (rc == NGX_OK && !of->is_dir) {
            cln->handler = ngx_pool_cleanup_file;
            clnf = cln->data;

            clnf->fd = of->fd;
            clnf->name = name->data;
            clnf->log = pool->log;
        }

        திரும்பு rc;
    }

    cln = ngx_pool_cleanup_add(pool, அளவு(ngx_open_file_cache_cleanup_t));
    எனில் (cln == வெற்று) {
        திரும்பு NGX_ERROR;
    }

    now = ngx_time();

    hash = ngx_crc32_long(name->data, name->len);

    file = ngx_open_file_lookup(cache, name, hash);

    எனில் (file) {

        file->uses++;

        ngx_queue_remove(&file->queue);

        எனில் (file->fd == NGX_INVALID_FILE && file->err == 0 && !file->is_dir) {

            /* file was not used often enough to keep open */

            rc = ngx_open_and_stat_file(name, of, pool->log);

            எனில் (rc != NGX_OK && (of->err == 0 || !of->errors)) {
                செல் failed;
            }

            செல் add_event;
        }

        எனில் (file->use_event
            || (file->event == வெற்று
                && (of->uniq == 0 || of->uniq == file->uniq)
                && now - file->created < of->valid
#எனில் (NGX_HAVE_OPENAT)
                && of->disable_symlinks == file->disable_symlinks
                && of->disable_symlinks_from == file->disable_symlinks_from
#endif
            ))
        {
            எனில் (file->err == 0) {

                of->fd = file->fd;
                of->uniq = file->uniq;
                of->mtime = file->mtime;
                of->size = file->size;

                of->is_dir = file->is_dir;
                of->is_file = file->is_file;
                of->is_link = file->is_link;
                of->is_exec = file->is_exec;
                of->is_directio = file->is_directio;

                எனில் (!file->is_dir) {
                    file->மொத்தம்++;
                    ngx_open_file_add_event(cache, file, of, pool->log);
                }

            } இல்லையெனில் {
                of->err = file->err;
#எனில் (NGX_HAVE_OPENAT)
                of->failed = file->disable_symlinks ? ngx_openat_file_n
                                                    : ngx_open_file_n;
#இல்லையெனில்
                of->failed = ngx_open_file_n;
#endif
            }

            செல் found;
        }

        ngx_log_debug4(NGX_LOG_DEBUG_CORE, pool->log, 0,
                       "retest open file: %s, fd:%d, c:%d, e:%d",
                       file->name, file->fd, file->மொத்தம், file->err);

        எனில் (file->is_dir) {

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

        எனில் (rc != NGX_OK && (of->err == 0 || !of->errors)) {
            செல் failed;
        }

        எனில் (of->is_dir) {

            எனில் (file->is_dir || file->err) {
                செல் update;
            }

            /* file became directory */

        } இல்லையெனில் எனில் (of->err == 0) {  /* file */

            எனில் (file->is_dir || file->err) {
                செல் add_event;
            }

            எனில் (of->uniq == file->uniq) {

                எனில் (file->event) {
                    file->use_event = 1;
                }

                of->is_directio = file->is_directio;

                செல் update;
            }

            /* file was changed */

        } இல்லையெனில் { /* error to cache */

            எனில் (file->err || file->is_dir) {
                செல் update;
            }

            /* file was removed, etc. */
        }

        எனில் (file->மொத்தம் == 0) {

            ngx_open_file_del_event(file);

            எனில் (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, pool->log, ngx_errno,
                              ngx_close_file_n " \"%V\" failed", name);
            }

            செல் add_event;
        }

        ngx_rbtree_delete(&cache->rbtree, &file->முனை);

        cache->current--;

        file->close = 1;

        செல் create;
    }

    /* not found */

    rc = ngx_open_and_stat_file(name, of, pool->log);

    எனில் (rc != NGX_OK && (of->err == 0 || !of->errors)) {
        செல் failed;
    }

create:

    எனில் (cache->current >= cache->max) {
        ngx_expire_old_cached_files(cache, 0, pool->log);
    }

    file = ngx_alloc(அளவு(ngx_cached_open_file_t), pool->log);

    எனில் (file == வெற்று) {
        செல் failed;
    }

    file->name = ngx_alloc(name->len + 1, pool->log);

    எனில் (file->name == வெற்று) {
        ngx_free(file);
        file = வெற்று;
        செல் failed;
    }

    ngx_cpystrn(file->name, name->data, name->len + 1);

    file->முனை.சாவி = hash;

    ngx_rbtree_செருகு(&cache->rbtree, &file->முனை);

    cache->current++;

    file->uses = 1;
    file->மொத்தம் = 0;
    file->use_event = 0;
    file->event = வெற்று;

add_event:

    ngx_open_file_add_event(cache, file, of, pool->log);

update:

    file->fd = of->fd;
    file->err = of->err;
#எனில் (NGX_HAVE_OPENAT)
    file->disable_symlinks = of->disable_symlinks;
    file->disable_symlinks_from = of->disable_symlinks_from;
#endif

    எனில் (of->err == 0) {
        file->uniq = of->uniq;
        file->mtime = of->mtime;
        file->size = of->size;

        file->close = 0;

        file->is_dir = of->is_dir;
        file->is_file = of->is_file;
        file->is_link = of->is_link;
        file->is_exec = of->is_exec;
        file->is_directio = of->is_directio;

        எனில் (!of->is_dir) {
            file->மொத்தம்++;
        }
    }

    file->created = now;

found:

    file->accessed = now;

    ngx_queue_செருகு_head(&cache->expire_queue, &file->queue);

    ngx_log_debug5(NGX_LOG_DEBUG_CORE, pool->log, 0,
                   "cached open file: %s, fd:%d, c:%d, e:%d, u:%d",
                   file->name, file->fd, file->மொத்தம், file->err, file->uses);

    எனில் (of->err == 0) {

        எனில் (!of->is_dir) {
            cln->handler = ngx_open_file_cleanup;
            ofcln = cln->data;

            ofcln->cache = cache;
            ofcln->file = file;
            ofcln->min_uses = of->min_uses;
            ofcln->log = pool->log;
        }

        திரும்பு NGX_OK;
    }

    திரும்பு NGX_ERROR;

failed:

    எனில் (file) {
        ngx_rbtree_delete(&cache->rbtree, &file->முனை);

        cache->current--;

        எனில் (file->மொத்தம் == 0) {

            எனில் (file->fd != NGX_INVALID_FILE) {
                எனில் (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_ALERT, pool->log, ngx_errno,
                                  ngx_close_file_n " \"%s\" failed",
                                  file->name);
                }
            }

            ngx_free(file->name);
            ngx_free(file);

        } இல்லையெனில் {
            file->close = 1;
        }
    }

    எனில் (of->fd != NGX_INVALID_FILE) {
        எனில் (ngx_close_file(of->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, pool->log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }
    }

    திரும்பு NGX_ERROR;
}


#எனில் (NGX_HAVE_OPENAT)

நிலையான ngx_fd_t
ngx_openat_file_owner(ngx_fd_t at_fd, மாறிலி u_char *name,
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

    எனில் (fd == NGX_INVALID_FILE) {
        திரும்பு NGX_INVALID_FILE;
    }

    எனில் (ngx_file_at_info(at_fd, name, &atfi, AT_SYMLINK_NOFOLLOW)
        == NGX_FILE_ERROR)
    {
        err = ngx_errno;
        செல் failed;
    }

#எனில் (NGX_HAVE_O_PATH)
    எனில் (ngx_file_o_path_info(fd, &fi, log) == NGX_ERROR) {
        err = ngx_errno;
        செல் failed;
    }
#இல்லையெனில்
    எனில் (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        err = ngx_errno;
        செல் failed;
    }
#endif

    எனில் (fi.st_uid != atfi.st_uid) {
        err = NGX_ELOOP;
        செல் failed;
    }

    திரும்பு fd;

failed:

    எனில் (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", name);
    }

    ngx_set_errno(err);

    திரும்பு NGX_INVALID_FILE;
}


#எனில் (NGX_HAVE_O_PATH)

நிலையான ngx_int_t
ngx_file_o_path_info(ngx_fd_t fd, ngx_file_info_t *fi, ngx_log_t *log)
{
    நிலையான ngx_uint_t  use_fstat = 1;

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

    எனில் (use_fstat) {
        எனில் (ngx_fd_info(fd, fi) != NGX_FILE_ERROR) {
            திரும்பு NGX_OK;
        }

        எனில் (ngx_errno != NGX_EBADF) {
            திரும்பு NGX_ERROR;
        }

        ngx_log_error(NGX_LOG_NOTICE, log, 0,
                      "fstat(O_PATH) failed with EBADF, "
                      "switching to fstatat(AT_EMPTY_PATH)");

        use_fstat = 0;
    }

    எனில் (ngx_file_at_info(fd, "", fi, AT_EMPTY_PATH) != NGX_FILE_ERROR) {
        திரும்பு NGX_OK;
    }

    திரும்பு NGX_ERROR;
}

#endif

#endif /* NGX_HAVE_OPENAT */


நிலையான ngx_fd_t
ngx_open_file_wrapper(ngx_str_t *name, ngx_open_file_info_t *of,
    ngx_int_t mode, ngx_int_t create, ngx_int_t access, ngx_log_t *log)
{
    ngx_fd_t  fd;

#எனில் !(NGX_HAVE_OPENAT)

    fd = ngx_open_file(name->data, mode, create, access);

    எனில் (fd == NGX_INVALID_FILE) {
        of->err = ngx_errno;
        of->failed = ngx_open_file_n;
        திரும்பு NGX_INVALID_FILE;
    }

    திரும்பு fd;

#இல்லையெனில்

    u_char           *p, *cp, *end;
    ngx_fd_t          at_fd;
    ngx_str_t         at_name;

    எனில் (of->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {
        fd = ngx_open_file(name->data, mode, create, access);

        எனில் (fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_open_file_n;
            திரும்பு NGX_INVALID_FILE;
        }

        திரும்பு fd;
    }

    p = name->data;
    end = p + name->len;

    at_name = *name;

    எனில் (of->disable_symlinks_from) {

        cp = p + of->disable_symlinks_from;

        *cp = '\0';

        at_fd = ngx_open_file(p, NGX_FILE_தேடல்|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        *cp = '/';

        எனில் (at_fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_open_file_n;
            திரும்பு NGX_INVALID_FILE;
        }

        at_name.len = of->disable_symlinks_from;
        p = cp + 1;

    } இல்லையெனில் எனில் (*p == '/') {

        at_fd = ngx_open_file("/",
                              NGX_FILE_தேடல்|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        எனில் (at_fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_openat_file_n;
            திரும்பு NGX_INVALID_FILE;
        }

        at_name.len = 1;
        p++;

    } இல்லையெனில் {
        at_fd = NGX_AT_FDCWD;
    }

    க்கு ( ;; ) {
        cp = ngx_strlchr(p, end, '/');
        எனில் (cp == வெற்று) {
            நிறுத்து;
        }

        எனில் (cp == p) {
            p++;
            தொடர்;
        }

        *cp = '\0';

        எனில் (of->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER) {
            fd = ngx_openat_file_owner(at_fd, p,
                                       NGX_FILE_தேடல்|NGX_FILE_NONBLOCK,
                                       NGX_FILE_OPEN, 0, log);

        } இல்லையெனில் {
            fd = ngx_openat_file(at_fd, p,
                           NGX_FILE_தேடல்|NGX_FILE_NONBLOCK|NGX_FILE_NOFOLLOW,
                           NGX_FILE_OPEN, 0);
        }

        *cp = '/';

        எனில் (fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_openat_file_n;
            செல் failed;
        }

        எனில் (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", &at_name);
        }

        p = cp + 1;
        at_fd = fd;
        at_name.len = cp - at_name.data;
    }

    எனில் (p == end) {

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
        செல் done;
    }

    எனில் (of->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER
        && !(create & (NGX_FILE_CREATE_OR_OPEN|NGX_FILE_TRUNCATE)))
    {
        fd = ngx_openat_file_owner(at_fd, p, mode, create, access, log);

    } இல்லையெனில் {
        fd = ngx_openat_file(at_fd, p, mode|NGX_FILE_NOFOLLOW, create, access);
    }

done:

    எனில் (fd == NGX_INVALID_FILE) {
        of->err = ngx_errno;
        of->failed = ngx_openat_file_n;
    }

failed:

    எனில் (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", &at_name);
    }

    திரும்பு fd;
#endif
}


நிலையான ngx_int_t
ngx_file_info_wrapper(ngx_str_t *name, ngx_open_file_info_t *of,
    ngx_file_info_t *fi, ngx_log_t *log)
{
    ngx_int_t  rc;

#எனில் !(NGX_HAVE_OPENAT)

    rc = ngx_file_info(name->data, fi);

    எனில் (rc == NGX_FILE_ERROR) {
        of->err = ngx_errno;
        of->failed = ngx_file_info_n;
        திரும்பு NGX_FILE_ERROR;
    }

    திரும்பு rc;

#இல்லையெனில்

    ngx_fd_t  fd;

    எனில் (of->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {

        rc = ngx_file_info(name->data, fi);

        எனில் (rc == NGX_FILE_ERROR) {
            of->err = ngx_errno;
            of->failed = ngx_file_info_n;
            திரும்பு NGX_FILE_ERROR;
        }

        திரும்பு rc;
    }

    fd = ngx_open_file_wrapper(name, of, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,
                               NGX_FILE_OPEN, 0, log);

    எனில் (fd == NGX_INVALID_FILE) {
        திரும்பு NGX_FILE_ERROR;
    }

    rc = ngx_fd_info(fd, fi);

    எனில் (rc == NGX_FILE_ERROR) {
        of->err = ngx_errno;
        of->failed = ngx_fd_info_n;
    }

    எனில் (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", name);
    }

    திரும்பு rc;
#endif
}


நிலையான ngx_int_t
ngx_open_and_stat_file(ngx_str_t *name, ngx_open_file_info_t *of,
    ngx_log_t *log)
{
    ngx_fd_t         fd;
    ngx_file_info_t  fi;

    எனில் (of->fd != NGX_INVALID_FILE) {

        எனில் (ngx_file_info_wrapper(name, of, &fi, log) == NGX_FILE_ERROR) {
            of->fd = NGX_INVALID_FILE;
            திரும்பு NGX_ERROR;
        }

        எனில் (of->uniq == ngx_file_uniq(&fi)) {
            செல் done;
        }

    } இல்லையெனில் எனில் (of->test_dir) {

        எனில் (ngx_file_info_wrapper(name, of, &fi, log) == NGX_FILE_ERROR) {
            of->fd = NGX_INVALID_FILE;
            திரும்பு NGX_ERROR;
        }

        எனில் (ngx_is_dir(&fi)) {
            செல் done;
        }
    }

    எனில் (!of->log) {

        /*
         * Use non-blocking open() not to hang on FIFO files, etc.
         * This flag has no effect on a regular files.
         */

        fd = ngx_open_file_wrapper(name, of, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,
                                   NGX_FILE_OPEN, 0, log);

    } இல்லையெனில் {
        fd = ngx_open_file_wrapper(name, of, NGX_FILE_APPEND,
                                   NGX_FILE_CREATE_OR_OPEN,
                                   NGX_FILE_DEFAULT_ACCESS, log);
    }

    எனில் (fd == NGX_INVALID_FILE) {
        of->fd = NGX_INVALID_FILE;
        திரும்பு NGX_ERROR;
    }

    எனில் (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, log, ngx_errno,
                      ngx_fd_info_n " \"%V\" failed", name);

        எனில் (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        of->fd = NGX_INVALID_FILE;

        திரும்பு NGX_ERROR;
    }

    எனில் (ngx_is_dir(&fi)) {
        எனில் (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        of->fd = NGX_INVALID_FILE;

    } இல்லையெனில் {
        of->fd = fd;

        எனில் (of->read_ahead && ngx_file_size(&fi) > NGX_MIN_READ_AHEAD) {
            எனில் (ngx_read_ahead(fd, of->read_ahead) == NGX_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                              ngx_read_ahead_n " \"%V\" failed", name);
            }
        }

        எனில் (of->directio <= ngx_file_size(&fi)) {
            எனில் (ngx_directio_on(fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                              ngx_directio_on_n " \"%V\" failed", name);

            } இல்லையெனில் {
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

    திரும்பு NGX_OK;
}


/*
 * we ignore any possible event setting error and
 * fallback to usual periodic file retests
 */

நிலையான வெறுமை
ngx_open_file_add_event(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_open_file_info_t *of, ngx_log_t *log)
{
    ngx_open_file_cache_event_t  *fev;

    எனில் (!(ngx_event_flags & NGX_USE_VNODE_EVENT)
        || !of->events
        || file->event
        || of->fd == NGX_INVALID_FILE
        || file->uses < of->min_uses)
    {
        திரும்பு;
    }

    file->use_event = 0;

    file->event = ngx_calloc(அளவு(ngx_event_t), log);
    எனில் (file->event== வெற்று) {
        திரும்பு;
    }

    fev = ngx_alloc(அளவு(ngx_open_file_cache_event_t), log);
    எனில் (fev == வெற்று) {
        ngx_free(file->event);
        file->event = வெற்று;
        திரும்பு;
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

    எனில் (ngx_add_event(file->event, NGX_VNODE_EVENT, NGX_ONESHOT_EVENT)
        != NGX_OK)
    {
        ngx_free(file->event->data);
        ngx_free(file->event);
        file->event = வெற்று;
        திரும்பு;
    }

    /*
     * we do not set file->use_event here because there may be a race
     * condition: a file may be deleted between opening the file and
     * adding event, so we rely upon event notification only after
     * one file revalidation on next file access
     */

    திரும்பு;
}


நிலையான வெறுமை
ngx_open_file_cleanup(வெறுமை *data)
{
    ngx_open_file_cache_cleanup_t  *c = data;

    c->file->மொத்தம்--;

    ngx_close_cached_file(c->cache, c->file, c->min_uses, c->log);

    /* drop one or two expired open files */
    ngx_expire_old_cached_files(c->cache, 1, c->log);
}


நிலையான வெறுமை
ngx_close_cached_file(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_uint_t min_uses, ngx_log_t *log)
{
    ngx_log_debug5(NGX_LOG_DEBUG_CORE, log, 0,
                   "close cached open file: %s, fd:%d, c:%d, u:%d, %d",
                   file->name, file->fd, file->மொத்தம், file->uses, file->close);

    எனில் (!file->close) {

        file->accessed = ngx_time();

        ngx_queue_remove(&file->queue);

        ngx_queue_செருகு_head(&cache->expire_queue, &file->queue);

        எனில் (file->uses >= min_uses || file->மொத்தம்) {
            திரும்பு;
        }
    }

    ngx_open_file_del_event(file);

    எனில் (file->மொத்தம்) {
        திரும்பு;
    }

    எனில் (file->fd != NGX_INVALID_FILE) {

        எனில் (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", file->name);
        }

        file->fd = NGX_INVALID_FILE;
    }

    எனில் (!file->close) {
        திரும்பு;
    }

    ngx_free(file->name);
    ngx_free(file);
}


நிலையான வெறுமை
ngx_open_file_del_event(ngx_cached_open_file_t *file)
{
    எனில் (file->event == வெற்று) {
        திரும்பு;
    }

    (வெறுமை) ngx_del_event(file->event, NGX_VNODE_EVENT,
                         file->மொத்தம் ? NGX_FLUSH_EVENT : NGX_CLOSE_EVENT);

    ngx_free(file->event->data);
    ngx_free(file->event);
    file->event = வெற்று;
    file->use_event = 0;
}


நிலையான வெறுமை
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

    போது (n < 3) {

        எனில் (ngx_queue_empty(&cache->expire_queue)) {
            திரும்பு;
        }

        q = ngx_queue_last(&cache->expire_queue);

        file = ngx_queue_data(q, ngx_cached_open_file_t, queue);

        எனில் (n++ != 0 && now - file->accessed <= cache->inactive) {
            திரும்பு;
        }

        ngx_queue_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->முனை);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0,
                       "expire cached open file: %s", file->name);

        எனில் (!file->err && !file->is_dir) {
            file->close = 1;
            ngx_close_cached_file(cache, file, 0, log);

        } இல்லையெனில் {
            ngx_free(file->name);
            ngx_free(file);
        }
    }
}


நிலையான வெறுமை
ngx_open_file_cache_rbtree_செருகு_மதிப்பு(ngx_rbtree_முனை_t *temp,
    ngx_rbtree_முனை_t *முனை, ngx_rbtree_முனை_t *sentinel)
{
    ngx_rbtree_முனை_t       **p;
    ngx_cached_open_file_t    *file, *file_temp;

    க்கு ( ;; ) {

        எனில் (முனை->சாவி < temp->சாவி) {

            p = &temp->இடது;

        } இல்லையெனில் எனில் (முனை->சாவி > temp->சாவி) {

            p = &temp->வலது;

        } இல்லையெனில் { /* node->key == temp->key */

            file = (ngx_cached_open_file_t *) முனை;
            file_temp = (ngx_cached_open_file_t *) temp;

            p = (ngx_strcmp(file->name, file_temp->name) < 0)
                    ? &temp->இடது : &temp->வலது;
        }

        எனில் (*p == sentinel) {
            நிறுத்து;
        }

        temp = *p;
    }

    *p = முனை;
    முனை->parent = temp;
    முனை->இடது = sentinel;
    முனை->வலது = sentinel;
    ngx_rbt_red(முனை);
}


நிலையான ngx_cached_open_file_t *
ngx_open_file_lookup(ngx_open_file_cache_t *cache, ngx_str_t *name,
    uint32_t hash)
{
    ngx_int_t                rc;
    ngx_rbtree_முனை_t       *முனை, *sentinel;
    ngx_cached_open_file_t  *file;

    முனை = cache->rbtree.வேர்;
    sentinel = cache->rbtree.sentinel;

    போது (முனை != sentinel) {

        எனில் (hash < முனை->சாவி) {
            முனை = முனை->இடது;
            தொடர்;
        }

        எனில் (hash > முனை->சாவி) {
            முனை = முனை->வலது;
            தொடர்;
        }

        /* hash == node->key */

        file = (ngx_cached_open_file_t *) முனை;

        rc = ngx_strcmp(name->data, file->name);

        எனில் (rc == 0) {
            திரும்பு file;
        }

        முனை = (rc < 0) ? முனை->இடது : முனை->வலது;
    }

    திரும்பு வெற்று;
}


நிலையான வெறுமை
ngx_open_file_cache_remove(ngx_event_t *ev)
{
    ngx_cached_open_file_t       *file;
    ngx_open_file_cache_event_t  *fev;

    fev = ev->data;
    file = fev->file;

    ngx_queue_remove(&file->queue);

    ngx_rbtree_delete(&fev->cache->rbtree, &file->முனை);

    fev->cache->current--;

    /* NGX_ONESHOT_EVENT was already deleted */
    file->event = வெற்று;
    file->use_event = 0;

    file->close = 1;

    ngx_close_cached_file(fev->cache, file, 0, ev->log);

    /* free memory only when fev->cache and fev->file are already not needed */

    ngx_free(ev->data);
    ngx_free(ev);
}
