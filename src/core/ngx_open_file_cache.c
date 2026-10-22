
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>
#অন্তর্ভাব <ngx_event.h>


/*
 * open file cache caches
 *    open file handles with stat() info;
 *    directories stat() info;
 *    files and directories errors: not found, access denied, etc.
 */


#define NGX_MIN_READ_AHEAD  (128 * 1024)


স্থির শূন্য ngx_open_file_cache_cleanup(শূন্য *তথ্য);
#যদি (NGX_HAVE_OPENAT)
স্থির ngx_fd_t ngx_openat_file_owner(ngx_fd_t at_fd, ধ্রুবক u_char *name,
    ngx_int_t mode, ngx_int_t create, ngx_int_t access, ngx_log_t *log);
#যদি (NGX_HAVE_O_PATH)
স্থির ngx_int_t ngx_file_o_path_info(ngx_fd_t fd, ngx_file_info_t *fi,
    ngx_log_t *log);
#endif
#endif
স্থির ngx_fd_t ngx_open_file_wrapper(ngx_str_t *name,
    ngx_open_file_info_t *of, ngx_int_t mode, ngx_int_t create,
    ngx_int_t access, ngx_log_t *log);
স্থির ngx_int_t ngx_file_info_wrapper(ngx_str_t *name,
    ngx_open_file_info_t *of, ngx_file_info_t *fi, ngx_log_t *log);
স্থির ngx_int_t ngx_open_and_stat_file(ngx_str_t *name,
    ngx_open_file_info_t *of, ngx_log_t *log);
স্থির শূন্য ngx_open_file_add_event(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_open_file_info_t *of, ngx_log_t *log);
স্থির শূন্য ngx_open_file_cleanup(শূন্য *তথ্য);
স্থির শূন্য ngx_close_cached_file(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_uint_t min_uses, ngx_log_t *log);
স্থির শূন্য ngx_open_file_del_event(ngx_cached_open_file_t *file);
স্থির শূন্য ngx_expire_old_cached_files(ngx_open_file_cache_t *cache,
    ngx_uint_t n, ngx_log_t *log);
স্থির শূন্য ngx_open_file_cache_rbtree_ঢোকাও_মান(ngx_rbtree_গ্রন্থি_t *অস্থায়ী,
    ngx_rbtree_গ্রন্থি_t *গ্রন্থি, ngx_rbtree_গ্রন্থি_t *sentinel);
স্থির ngx_cached_open_file_t *
    ngx_open_file_lookup(ngx_open_file_cache_t *cache, ngx_str_t *name,
    uint32_t hash);
স্থির শূন্য ngx_open_file_cache_remove(ngx_event_t *ev);


ngx_open_file_cache_t *
ngx_open_file_cache_init(ngx_pool_t *pool, ngx_uint_t max, time_t inactive)
{
    ngx_pool_cleanup_t     *cln;
    ngx_open_file_cache_t  *cache;

    cache = ngx_palloc(pool, আকার(ngx_open_file_cache_t));
    যদি (cache == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    ngx_rbtree_init(&cache->rbtree, &cache->sentinel,
                    ngx_open_file_cache_rbtree_ঢোকাও_মান);

    ngx_queue_init(&cache->expire_queue);

    cache->current = 0;
    cache->max = max;
    cache->inactive = inactive;

    cln = ngx_pool_cleanup_add(pool, 0);
    যদি (cln == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    cln->handler = ngx_open_file_cache_cleanup;
    cln->তথ্য = cache;

    ফেরত cache;
}


স্থির শূন্য
ngx_open_file_cache_cleanup(শূন্য *তথ্য)
{
    ngx_open_file_cache_t  *cache = তথ্য;

    ngx_queue_t             *q;
    ngx_cached_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "open file cache cleanup");

    জন্য ( ;; ) {

        যদি (ngx_queue_empty(&cache->expire_queue)) {
            বিরতি;
        }

        q = ngx_queue_last(&cache->expire_queue);

        file = ngx_queue_তথ্য(q, ngx_cached_open_file_t, queue);

        ngx_queue_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->গ্রন্থি);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                       "delete cached open file: %s", file->name);

        যদি (!file->err && !file->is_dir) {
            file->close = 1;
            file->গুনতি = 0;
            ngx_close_cached_file(cache, file, 0, ngx_cycle->log);

        } অন্যথা {
            ngx_free(file->name);
            ngx_free(file);
        }
    }

    যদি (cache->current) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0,
                      "%ui items still left in open file cache",
                      cache->current);
    }

    যদি (cache->rbtree.মূল != cache->rbtree.sentinel) {
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

    যদি (cache == ফাঁকা) {

        যদি (of->test_only) {

            যদি (ngx_file_info_wrapper(name, of, &fi, pool->log)
                == NGX_FILE_ERROR)
            {
                ফেরত NGX_ERROR;
            }

            of->uniq = ngx_file_uniq(&fi);
            of->mtime = ngx_file_mtime(&fi);
            of->আকার_2 = ngx_file_আকার(&fi);
            of->fs_আকার = ngx_file_fs_আকার(&fi);
            of->is_dir = ngx_is_dir(&fi);
            of->is_file = ngx_is_file(&fi);
            of->is_link = ngx_is_link(&fi);
            of->is_exec = ngx_is_exec(&fi);

            ফেরত NGX_OK;
        }

        cln = ngx_pool_cleanup_add(pool, আকার(ngx_pool_cleanup_file_t));
        যদি (cln == ফাঁকা) {
            ফেরত NGX_ERROR;
        }

        rc = ngx_open_and_stat_file(name, of, pool->log);

        যদি (rc == NGX_OK && !of->is_dir) {
            cln->handler = ngx_pool_cleanup_file;
            clnf = cln->তথ্য;

            clnf->fd = of->fd;
            clnf->name = name->তথ্য;
            clnf->log = pool->log;
        }

        ফেরত rc;
    }

    cln = ngx_pool_cleanup_add(pool, আকার(ngx_open_file_cache_cleanup_t));
    যদি (cln == ফাঁকা) {
        ফেরত NGX_ERROR;
    }

    now = ngx_time();

    hash = ngx_crc32_long(name->তথ্য, name->len);

    file = ngx_open_file_lookup(cache, name, hash);

    যদি (file) {

        file->uses++;

        ngx_queue_remove(&file->queue);

        যদি (file->fd == NGX_INVALID_FILE && file->err == 0 && !file->is_dir) {

            /* file was not used often enough to keep open */

            rc = ngx_open_and_stat_file(name, of, pool->log);

            যদি (rc != NGX_OK && (of->err == 0 || !of->errors)) {
                যাও failed;
            }

            যাও add_event;
        }

        যদি (file->use_event
            || (file->event == ফাঁকা
                && (of->uniq == 0 || of->uniq == file->uniq)
                && now - file->created < of->valid
#যদি (NGX_HAVE_OPENAT)
                && of->disable_symlinks == file->disable_symlinks
                && of->disable_symlinks_from == file->disable_symlinks_from
#endif
            ))
        {
            যদি (file->err == 0) {

                of->fd = file->fd;
                of->uniq = file->uniq;
                of->mtime = file->mtime;
                of->আকার_2 = file->আকার_2;

                of->is_dir = file->is_dir;
                of->is_file = file->is_file;
                of->is_link = file->is_link;
                of->is_exec = file->is_exec;
                of->is_directio = file->is_directio;

                যদি (!file->is_dir) {
                    file->গুনতি++;
                    ngx_open_file_add_event(cache, file, of, pool->log);
                }

            } অন্যথা {
                of->err = file->err;
#যদি (NGX_HAVE_OPENAT)
                of->failed = file->disable_symlinks ? ngx_openat_file_n
                                                    : ngx_open_file_n;
#অন্যথা
                of->failed = ngx_open_file_n;
#endif
            }

            যাও found;
        }

        ngx_log_debug4(NGX_LOG_DEBUG_CORE, pool->log, 0,
                       "retest open file: %s, fd:%d, c:%d, e:%d",
                       file->name, file->fd, file->গুনতি, file->err);

        যদি (file->is_dir) {

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

        যদি (rc != NGX_OK && (of->err == 0 || !of->errors)) {
            যাও failed;
        }

        যদি (of->is_dir) {

            যদি (file->is_dir || file->err) {
                যাও update;
            }

            /* file became directory */

        } অন্যথা যদি (of->err == 0) {  /* file */

            যদি (file->is_dir || file->err) {
                যাও add_event;
            }

            যদি (of->uniq == file->uniq) {

                যদি (file->event) {
                    file->use_event = 1;
                }

                of->is_directio = file->is_directio;

                যাও update;
            }

            /* file was changed */

        } অন্যথা { /* error to cache */

            যদি (file->err || file->is_dir) {
                যাও update;
            }

            /* file was removed, etc. */
        }

        যদি (file->গুনতি == 0) {

            ngx_open_file_del_event(file);

            যদি (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, pool->log, ngx_errno,
                              ngx_close_file_n " \"%V\" failed", name);
            }

            যাও add_event;
        }

        ngx_rbtree_delete(&cache->rbtree, &file->গ্রন্থি);

        cache->current--;

        file->close = 1;

        যাও create;
    }

    /* not found */

    rc = ngx_open_and_stat_file(name, of, pool->log);

    যদি (rc != NGX_OK && (of->err == 0 || !of->errors)) {
        যাও failed;
    }

create:

    যদি (cache->current >= cache->max) {
        ngx_expire_old_cached_files(cache, 0, pool->log);
    }

    file = ngx_alloc(আকার(ngx_cached_open_file_t), pool->log);

    যদি (file == ফাঁকা) {
        যাও failed;
    }

    file->name = ngx_alloc(name->len + 1, pool->log);

    যদি (file->name == ফাঁকা) {
        ngx_free(file);
        file = ফাঁকা;
        যাও failed;
    }

    ngx_cpystrn(file->name, name->তথ্য, name->len + 1);

    file->গ্রন্থি.চাবি = hash;

    ngx_rbtree_ঢোকাও(&cache->rbtree, &file->গ্রন্থি);

    cache->current++;

    file->uses = 1;
    file->গুনতি = 0;
    file->use_event = 0;
    file->event = ফাঁকা;

add_event:

    ngx_open_file_add_event(cache, file, of, pool->log);

update:

    file->fd = of->fd;
    file->err = of->err;
#যদি (NGX_HAVE_OPENAT)
    file->disable_symlinks = of->disable_symlinks;
    file->disable_symlinks_from = of->disable_symlinks_from;
#endif

    যদি (of->err == 0) {
        file->uniq = of->uniq;
        file->mtime = of->mtime;
        file->আকার_2 = of->আকার_2;

        file->close = 0;

        file->is_dir = of->is_dir;
        file->is_file = of->is_file;
        file->is_link = of->is_link;
        file->is_exec = of->is_exec;
        file->is_directio = of->is_directio;

        যদি (!of->is_dir) {
            file->গুনতি++;
        }
    }

    file->created = now;

found:

    file->accessed = now;

    ngx_queue_ঢোকাও_শীর্ষ(&cache->expire_queue, &file->queue);

    ngx_log_debug5(NGX_LOG_DEBUG_CORE, pool->log, 0,
                   "cached open file: %s, fd:%d, c:%d, e:%d, u:%d",
                   file->name, file->fd, file->গুনতি, file->err, file->uses);

    যদি (of->err == 0) {

        যদি (!of->is_dir) {
            cln->handler = ngx_open_file_cleanup;
            ofcln = cln->তথ্য;

            ofcln->cache = cache;
            ofcln->file = file;
            ofcln->min_uses = of->min_uses;
            ofcln->log = pool->log;
        }

        ফেরত NGX_OK;
    }

    ফেরত NGX_ERROR;

failed:

    যদি (file) {
        ngx_rbtree_delete(&cache->rbtree, &file->গ্রন্থি);

        cache->current--;

        যদি (file->গুনতি == 0) {

            যদি (file->fd != NGX_INVALID_FILE) {
                যদি (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_ALERT, pool->log, ngx_errno,
                                  ngx_close_file_n " \"%s\" failed",
                                  file->name);
                }
            }

            ngx_free(file->name);
            ngx_free(file);

        } অন্যথা {
            file->close = 1;
        }
    }

    যদি (of->fd != NGX_INVALID_FILE) {
        যদি (ngx_close_file(of->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, pool->log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }
    }

    ফেরত NGX_ERROR;
}


#যদি (NGX_HAVE_OPENAT)

স্থির ngx_fd_t
ngx_openat_file_owner(ngx_fd_t at_fd, ধ্রুবক u_char *name,
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

    যদি (fd == NGX_INVALID_FILE) {
        ফেরত NGX_INVALID_FILE;
    }

    যদি (ngx_file_at_info(at_fd, name, &atfi, AT_SYMLINK_NOFOLLOW)
        == NGX_FILE_ERROR)
    {
        err = ngx_errno;
        যাও failed;
    }

#যদি (NGX_HAVE_O_PATH)
    যদি (ngx_file_o_path_info(fd, &fi, log) == NGX_ERROR) {
        err = ngx_errno;
        যাও failed;
    }
#অন্যথা
    যদি (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        err = ngx_errno;
        যাও failed;
    }
#endif

    যদি (fi.st_uid != atfi.st_uid) {
        err = NGX_ELOOP;
        যাও failed;
    }

    ফেরত fd;

failed:

    যদি (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", name);
    }

    ngx_set_errno(err);

    ফেরত NGX_INVALID_FILE;
}


#যদি (NGX_HAVE_O_PATH)

স্থির ngx_int_t
ngx_file_o_path_info(ngx_fd_t fd, ngx_file_info_t *fi, ngx_log_t *log)
{
    স্থির ngx_uint_t  use_fstat = 1;

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

    যদি (use_fstat) {
        যদি (ngx_fd_info(fd, fi) != NGX_FILE_ERROR) {
            ফেরত NGX_OK;
        }

        যদি (ngx_errno != NGX_EBADF) {
            ফেরত NGX_ERROR;
        }

        ngx_log_error(NGX_LOG_NOTICE, log, 0,
                      "fstat(O_PATH) failed with EBADF, "
                      "switching to fstatat(AT_EMPTY_PATH)");

        use_fstat = 0;
    }

    যদি (ngx_file_at_info(fd, "", fi, AT_EMPTY_PATH) != NGX_FILE_ERROR) {
        ফেরত NGX_OK;
    }

    ফেরত NGX_ERROR;
}

#endif

#endif /* NGX_HAVE_OPENAT */


স্থির ngx_fd_t
ngx_open_file_wrapper(ngx_str_t *name, ngx_open_file_info_t *of,
    ngx_int_t mode, ngx_int_t create, ngx_int_t access, ngx_log_t *log)
{
    ngx_fd_t  fd;

#যদি !(NGX_HAVE_OPENAT)

    fd = ngx_open_file(name->তথ্য, mode, create, access);

    যদি (fd == NGX_INVALID_FILE) {
        of->err = ngx_errno;
        of->failed = ngx_open_file_n;
        ফেরত NGX_INVALID_FILE;
    }

    ফেরত fd;

#অন্যথা

    u_char           *p, *cp, *end;
    ngx_fd_t          at_fd;
    ngx_str_t         at_name;

    যদি (of->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {
        fd = ngx_open_file(name->তথ্য, mode, create, access);

        যদি (fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_open_file_n;
            ফেরত NGX_INVALID_FILE;
        }

        ফেরত fd;
    }

    p = name->তথ্য;
    end = p + name->len;

    at_name = *name;

    যদি (of->disable_symlinks_from) {

        cp = p + of->disable_symlinks_from;

        *cp = '\0';

        at_fd = ngx_open_file(p, NGX_FILE_অনুসন্ধান|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        *cp = '/';

        যদি (at_fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_open_file_n;
            ফেরত NGX_INVALID_FILE;
        }

        at_name.len = of->disable_symlinks_from;
        p = cp + 1;

    } অন্যথা যদি (*p == '/') {

        at_fd = ngx_open_file("/",
                              NGX_FILE_অনুসন্ধান|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        যদি (at_fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_openat_file_n;
            ফেরত NGX_INVALID_FILE;
        }

        at_name.len = 1;
        p++;

    } অন্যথা {
        at_fd = NGX_AT_FDCWD;
    }

    জন্য ( ;; ) {
        cp = ngx_strlchr(p, end, '/');
        যদি (cp == ফাঁকা) {
            বিরতি;
        }

        যদি (cp == p) {
            p++;
            চালু;
        }

        *cp = '\0';

        যদি (of->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER) {
            fd = ngx_openat_file_owner(at_fd, p,
                                       NGX_FILE_অনুসন্ধান|NGX_FILE_NONBLOCK,
                                       NGX_FILE_OPEN, 0, log);

        } অন্যথা {
            fd = ngx_openat_file(at_fd, p,
                           NGX_FILE_অনুসন্ধান|NGX_FILE_NONBLOCK|NGX_FILE_NOFOLLOW,
                           NGX_FILE_OPEN, 0);
        }

        *cp = '/';

        যদি (fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_openat_file_n;
            যাও failed;
        }

        যদি (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", &at_name);
        }

        p = cp + 1;
        at_fd = fd;
        at_name.len = cp - at_name.তথ্য;
    }

    যদি (p == end) {

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
        যাও done;
    }

    যদি (of->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER
        && !(create & (NGX_FILE_CREATE_OR_OPEN|NGX_FILE_TRUNCATE)))
    {
        fd = ngx_openat_file_owner(at_fd, p, mode, create, access, log);

    } অন্যথা {
        fd = ngx_openat_file(at_fd, p, mode|NGX_FILE_NOFOLLOW, create, access);
    }

done:

    যদি (fd == NGX_INVALID_FILE) {
        of->err = ngx_errno;
        of->failed = ngx_openat_file_n;
    }

failed:

    যদি (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", &at_name);
    }

    ফেরত fd;
#endif
}


স্থির ngx_int_t
ngx_file_info_wrapper(ngx_str_t *name, ngx_open_file_info_t *of,
    ngx_file_info_t *fi, ngx_log_t *log)
{
    ngx_int_t  rc;

#যদি !(NGX_HAVE_OPENAT)

    rc = ngx_file_info(name->তথ্য, fi);

    যদি (rc == NGX_FILE_ERROR) {
        of->err = ngx_errno;
        of->failed = ngx_file_info_n;
        ফেরত NGX_FILE_ERROR;
    }

    ফেরত rc;

#অন্যথা

    ngx_fd_t  fd;

    যদি (of->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {

        rc = ngx_file_info(name->তথ্য, fi);

        যদি (rc == NGX_FILE_ERROR) {
            of->err = ngx_errno;
            of->failed = ngx_file_info_n;
            ফেরত NGX_FILE_ERROR;
        }

        ফেরত rc;
    }

    fd = ngx_open_file_wrapper(name, of, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,
                               NGX_FILE_OPEN, 0, log);

    যদি (fd == NGX_INVALID_FILE) {
        ফেরত NGX_FILE_ERROR;
    }

    rc = ngx_fd_info(fd, fi);

    যদি (rc == NGX_FILE_ERROR) {
        of->err = ngx_errno;
        of->failed = ngx_fd_info_n;
    }

    যদি (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", name);
    }

    ফেরত rc;
#endif
}


স্থির ngx_int_t
ngx_open_and_stat_file(ngx_str_t *name, ngx_open_file_info_t *of,
    ngx_log_t *log)
{
    ngx_fd_t         fd;
    ngx_file_info_t  fi;

    যদি (of->fd != NGX_INVALID_FILE) {

        যদি (ngx_file_info_wrapper(name, of, &fi, log) == NGX_FILE_ERROR) {
            of->fd = NGX_INVALID_FILE;
            ফেরত NGX_ERROR;
        }

        যদি (of->uniq == ngx_file_uniq(&fi)) {
            যাও done;
        }

    } অন্যথা যদি (of->test_dir) {

        যদি (ngx_file_info_wrapper(name, of, &fi, log) == NGX_FILE_ERROR) {
            of->fd = NGX_INVALID_FILE;
            ফেরত NGX_ERROR;
        }

        যদি (ngx_is_dir(&fi)) {
            যাও done;
        }
    }

    যদি (!of->log) {

        /*
         * Use non-blocking open() not to hang on FIFO files, etc.
         * This flag has no effect on a regular files.
         */

        fd = ngx_open_file_wrapper(name, of, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,
                                   NGX_FILE_OPEN, 0, log);

    } অন্যথা {
        fd = ngx_open_file_wrapper(name, of, NGX_FILE_APPEND,
                                   NGX_FILE_CREATE_OR_OPEN,
                                   NGX_FILE_DEFAULT_ACCESS, log);
    }

    যদি (fd == NGX_INVALID_FILE) {
        of->fd = NGX_INVALID_FILE;
        ফেরত NGX_ERROR;
    }

    যদি (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, log, ngx_errno,
                      ngx_fd_info_n " \"%V\" failed", name);

        যদি (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        of->fd = NGX_INVALID_FILE;

        ফেরত NGX_ERROR;
    }

    যদি (ngx_is_dir(&fi)) {
        যদি (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        of->fd = NGX_INVALID_FILE;

    } অন্যথা {
        of->fd = fd;

        যদি (of->read_ahead && ngx_file_আকার(&fi) > NGX_MIN_READ_AHEAD) {
            যদি (ngx_read_ahead(fd, of->read_ahead) == NGX_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                              ngx_read_ahead_n " \"%V\" failed", name);
            }
        }

        যদি (of->directio <= ngx_file_আকার(&fi)) {
            যদি (ngx_directio_on(fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                              ngx_directio_on_n " \"%V\" failed", name);

            } অন্যথা {
                of->is_directio = 1;
            }
        }
    }

done:

    of->uniq = ngx_file_uniq(&fi);
    of->mtime = ngx_file_mtime(&fi);
    of->আকার_2 = ngx_file_আকার(&fi);
    of->fs_আকার = ngx_file_fs_আকার(&fi);
    of->is_dir = ngx_is_dir(&fi);
    of->is_file = ngx_is_file(&fi);
    of->is_link = ngx_is_link(&fi);
    of->is_exec = ngx_is_exec(&fi);

    ফেরত NGX_OK;
}


/*
 * we ignore any possible event setting error and
 * fallback to usual periodic file retests
 */

স্থির শূন্য
ngx_open_file_add_event(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_open_file_info_t *of, ngx_log_t *log)
{
    ngx_open_file_cache_event_t  *fev;

    যদি (!(ngx_event_flags & NGX_USE_VNODE_EVENT)
        || !of->events
        || file->event
        || of->fd == NGX_INVALID_FILE
        || file->uses < of->min_uses)
    {
        ফেরত;
    }

    file->use_event = 0;

    file->event = ngx_calloc(আকার(ngx_event_t), log);
    যদি (file->event== ফাঁকা) {
        ফেরত;
    }

    fev = ngx_alloc(আকার(ngx_open_file_cache_event_t), log);
    যদি (fev == ফাঁকা) {
        ngx_free(file->event);
        file->event = ফাঁকা;
        ফেরত;
    }

    fev->fd = of->fd;
    fev->file = file;
    fev->cache = cache;

    file->event->handler = ngx_open_file_cache_remove;
    file->event->তথ্য = fev;

    /*
     * although vnode event may be called while ngx_cycle->poll
     * destruction, however, cleanup procedures are run before any
     * memory freeing and events will be canceled.
     */

    file->event->log = ngx_cycle->log;

    যদি (ngx_add_event(file->event, NGX_VNODE_EVENT, NGX_ONESHOT_EVENT)
        != NGX_OK)
    {
        ngx_free(file->event->তথ্য);
        ngx_free(file->event);
        file->event = ফাঁকা;
        ফেরত;
    }

    /*
     * we do not set file->use_event here because there may be a race
     * condition: a file may be deleted between opening the file and
     * adding event, so we rely upon event notification only after
     * one file revalidation on next file access
     */

    ফেরত;
}


স্থির শূন্য
ngx_open_file_cleanup(শূন্য *তথ্য)
{
    ngx_open_file_cache_cleanup_t  *c = তথ্য;

    c->file->গুনতি--;

    ngx_close_cached_file(c->cache, c->file, c->min_uses, c->log);

    /* drop one or two expired open files */
    ngx_expire_old_cached_files(c->cache, 1, c->log);
}


স্থির শূন্য
ngx_close_cached_file(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_uint_t min_uses, ngx_log_t *log)
{
    ngx_log_debug5(NGX_LOG_DEBUG_CORE, log, 0,
                   "close cached open file: %s, fd:%d, c:%d, u:%d, %d",
                   file->name, file->fd, file->গুনতি, file->uses, file->close);

    যদি (!file->close) {

        file->accessed = ngx_time();

        ngx_queue_remove(&file->queue);

        ngx_queue_ঢোকাও_শীর্ষ(&cache->expire_queue, &file->queue);

        যদি (file->uses >= min_uses || file->গুনতি) {
            ফেরত;
        }
    }

    ngx_open_file_del_event(file);

    যদি (file->গুনতি) {
        ফেরত;
    }

    যদি (file->fd != NGX_INVALID_FILE) {

        যদি (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", file->name);
        }

        file->fd = NGX_INVALID_FILE;
    }

    যদি (!file->close) {
        ফেরত;
    }

    ngx_free(file->name);
    ngx_free(file);
}


স্থির শূন্য
ngx_open_file_del_event(ngx_cached_open_file_t *file)
{
    যদি (file->event == ফাঁকা) {
        ফেরত;
    }

    (শূন্য) ngx_del_event(file->event, NGX_VNODE_EVENT,
                         file->গুনতি ? NGX_FLUSH_EVENT : NGX_CLOSE_EVENT);

    ngx_free(file->event->তথ্য);
    ngx_free(file->event);
    file->event = ফাঁকা;
    file->use_event = 0;
}


স্থির শূন্য
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

    যখন (n < 3) {

        যদি (ngx_queue_empty(&cache->expire_queue)) {
            ফেরত;
        }

        q = ngx_queue_last(&cache->expire_queue);

        file = ngx_queue_তথ্য(q, ngx_cached_open_file_t, queue);

        যদি (n++ != 0 && now - file->accessed <= cache->inactive) {
            ফেরত;
        }

        ngx_queue_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->গ্রন্থি);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0,
                       "expire cached open file: %s", file->name);

        যদি (!file->err && !file->is_dir) {
            file->close = 1;
            ngx_close_cached_file(cache, file, 0, log);

        } অন্যথা {
            ngx_free(file->name);
            ngx_free(file);
        }
    }
}


স্থির শূন্য
ngx_open_file_cache_rbtree_ঢোকাও_মান(ngx_rbtree_গ্রন্থি_t *অস্থায়ী,
    ngx_rbtree_গ্রন্থি_t *গ্রন্থি, ngx_rbtree_গ্রন্থি_t *sentinel)
{
    ngx_rbtree_গ্রন্থি_t       **p;
    ngx_cached_open_file_t    *file, *file_অস্থায়ী;

    জন্য ( ;; ) {

        যদি (গ্রন্থি->চাবি < অস্থায়ী->চাবি) {

            p = &অস্থায়ী->বাম;

        } অন্যথা যদি (গ্রন্থি->চাবি > অস্থায়ী->চাবি) {

            p = &অস্থায়ী->ডান;

        } অন্যথা { /* node->key == temp->key */

            file = (ngx_cached_open_file_t *) গ্রন্থি;
            file_অস্থায়ী = (ngx_cached_open_file_t *) অস্থায়ী;

            p = (ngx_strcmp(file->name, file_অস্থায়ী->name) < 0)
                    ? &অস্থায়ী->বাম : &অস্থায়ী->ডান;
        }

        যদি (*p == sentinel) {
            বিরতি;
        }

        অস্থায়ী = *p;
    }

    *p = গ্রন্থি;
    গ্রন্থি->parent = অস্থায়ী;
    গ্রন্থি->বাম = sentinel;
    গ্রন্থি->ডান = sentinel;
    ngx_rbt_red(গ্রন্থি);
}


স্থির ngx_cached_open_file_t *
ngx_open_file_lookup(ngx_open_file_cache_t *cache, ngx_str_t *name,
    uint32_t hash)
{
    ngx_int_t                rc;
    ngx_rbtree_গ্রন্থি_t       *গ্রন্থি, *sentinel;
    ngx_cached_open_file_t  *file;

    গ্রন্থি = cache->rbtree.মূল;
    sentinel = cache->rbtree.sentinel;

    যখন (গ্রন্থি != sentinel) {

        যদি (hash < গ্রন্থি->চাবি) {
            গ্রন্থি = গ্রন্থি->বাম;
            চালু;
        }

        যদি (hash > গ্রন্থি->চাবি) {
            গ্রন্থি = গ্রন্থি->ডান;
            চালু;
        }

        /* hash == node->key */

        file = (ngx_cached_open_file_t *) গ্রন্থি;

        rc = ngx_strcmp(name->তথ্য, file->name);

        যদি (rc == 0) {
            ফেরত file;
        }

        গ্রন্থি = (rc < 0) ? গ্রন্থি->বাম : গ্রন্থি->ডান;
    }

    ফেরত ফাঁকা;
}


স্থির শূন্য
ngx_open_file_cache_remove(ngx_event_t *ev)
{
    ngx_cached_open_file_t       *file;
    ngx_open_file_cache_event_t  *fev;

    fev = ev->তথ্য;
    file = fev->file;

    ngx_queue_remove(&file->queue);

    ngx_rbtree_delete(&fev->cache->rbtree, &file->গ্রন্থি);

    fev->cache->current--;

    /* NGX_ONESHOT_EVENT was already deleted */
    file->event = ফাঁকা;
    file->use_event = 0;

    file->close = 1;

    ngx_close_cached_file(fev->cache, file, 0, ev->log);

    /* free memory only when fev->cache and fev->file are already not needed */

    ngx_free(ev->তথ্য);
    ngx_free(ev);
}
