
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <ngx_event.h>


/*
 * open file cache caches
 *    open file handles with stat() info;
 *    directories stat() info;
 *    files and directories errors: not found, access denied, etc.
 */


#تعریف_کړه NGX_MIN_READ_AHEAD  (128 * 1024)


ساكن فراغ ngx_open_file_cache_cleanup(فراغ *معلومات);
#إذا (NGX_HAVE_OPENAT)
ساكن ngx_fd_t ngx_openat_file_owner(ngx_fd_t at_fd, ثابت u_char *name,
    ngx_int_t ډېرپېښ, ngx_int_t create, ngx_int_t access, ngx_log_t *لوګاریتم);
#إذا (NGX_HAVE_O_لار)
ساكن ngx_int_t ngx_file_o_لار_info(ngx_fd_t fd, ngx_file_info_t *fi,
    ngx_log_t *لوګاریتم);
#د_که_پای
#د_که_پای
ساكن ngx_fd_t ngx_open_file_wrapper(ngx_str_t *name,
    ngx_open_file_info_t *د, ngx_int_t ډېرپېښ, ngx_int_t create,
    ngx_int_t access, ngx_log_t *لوګاریتم);
ساكن ngx_int_t ngx_file_info_wrapper(ngx_str_t *name,
    ngx_open_file_info_t *د, ngx_file_info_t *fi, ngx_log_t *لوګاریتم);
ساكن ngx_int_t ngx_open_and_stat_file(ngx_str_t *name,
    ngx_open_file_info_t *د, ngx_log_t *لوګاریتم);
ساكن فراغ ngx_open_file_جمع_کړه_event(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_open_file_info_t *د, ngx_log_t *لوګاریتم);
ساكن فراغ ngx_open_file_cleanup(فراغ *معلومات);
ساكن فراغ ngx_close_cached_file(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_uint_t min_کاروي, ngx_log_t *لوګاریتم);
ساكن فراغ ngx_open_file_del_event(ngx_cached_open_file_t *file);
ساكن فراغ ngx_expire_old_cached_files(ngx_open_file_cache_t *cache,
    ngx_uint_t n, ngx_log_t *لوګاریتم);
ساكن فراغ ngx_open_file_cache_rbtree_ورننباسه_ارزښت(ngx_rbtree_غوټه_t *لنډمهاله,
    ngx_rbtree_غوټه_t *غوټه, ngx_rbtree_غوټه_t *sentinel);
ساكن ngx_cached_open_file_t *
    ngx_open_file_lookup(ngx_open_file_cache_t *cache, ngx_str_t *name,
    uint32_t hash);
ساكن فراغ ngx_open_file_cache_remove(ngx_event_t *ev);


ngx_open_file_cache_t *
ngx_open_file_cache_init(ngx_pool_t *pool, ngx_uint_t max, وخت_t inactive)
{
    ngx_pool_cleanup_t     *cln;
    ngx_open_file_cache_t  *cache;

    cache = ngx_palloc(pool, حجم(ngx_open_file_cache_t));
    إذا (cache == عدم) {
        أرجع عدم;
    }

    ngx_rbtree_init(&cache->rbtree, &cache->sentinel,
                    ngx_open_file_cache_rbtree_ورننباسه_ارزښت);

    ngx_کتار_init(&cache->expire_کتار);

    cache->current = 0;
    cache->max = max;
    cache->inactive = inactive;

    cln = ngx_pool_cleanup_جمع_کړه(pool, 0);
    إذا (cln == عدم) {
        أرجع عدم;
    }

    cln->handler = ngx_open_file_cache_cleanup;
    cln->معلومات = cache;

    أرجع cache;
}


ساكن فراغ
ngx_open_file_cache_cleanup(فراغ *معلومات)
{
    ngx_open_file_cache_t  *cache = معلومات;

    ngx_کتار_t             *q;
    ngx_cached_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_دوره->لوګاریتم, 0,
                   "open file cache cleanup");

    لكل ( ;; ) {

        إذا (ngx_کتار_تشه(&cache->expire_کتار)) {
            اكسر;
        }

        q = ngx_کتار_وروستی(&cache->expire_کتار);

        file = ngx_کتار_معلومات(q, ngx_cached_open_file_t, کتار);

        ngx_کتار_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->غوټه);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_دوره->لوګاریتم, 0,
                       "delete cached open file: %s", file->name);

        إذا (!file->err && !file->is_dir) {
            file->close = 1;
            file->شمېر = 0;
            ngx_close_cached_file(cache, file, 0, ngx_دوره->لوګاریتم);

        } وإلا {
            ngx_free(file->name);
            ngx_free(file);
        }
    }

    إذا (cache->current) {
        ngx_log_error(NGX_LOG_ALERT, ngx_دوره->لوګاریتم, 0,
                      "%ui items still left in open file cache",
                      cache->current);
    }

    إذا (cache->rbtree.ریښه != cache->rbtree.sentinel) {
        ngx_log_error(NGX_LOG_ALERT, ngx_دوره->لوګاریتم, 0,
                      "rbtree still is not empty in open file cache");

    }
}


ngx_int_t
ngx_open_cached_file(ngx_open_file_cache_t *cache, ngx_str_t *name,
    ngx_open_file_info_t *د, ngx_pool_t *pool)
{
    وخت_t                          now;
    uint32_t                        hash;
    ngx_int_t                       rc;
    ngx_file_info_t                 fi;
    ngx_pool_cleanup_t             *cln;
    ngx_cached_open_file_t         *file;
    ngx_pool_cleanup_file_t        *clnf;
    ngx_open_file_cache_cleanup_t  *ofcln;

    د->fd = NGX_INVALID_FILE;
    د->err = 0;

    إذا (cache == عدم) {

        إذا (د->test_only) {

            إذا (ngx_file_info_wrapper(name, د, &fi, pool->لوګاریتم)
                == NGX_FILE_ERROR)
            {
                أرجع NGX_ERROR;
            }

            د->uniq = ngx_file_uniq(&fi);
            د->mtime = ngx_file_mtime(&fi);
            د->اندازه = ngx_file_اندازه(&fi);
            د->fs_اندازه = ngx_file_fs_اندازه(&fi);
            د->is_dir = ngx_is_dir(&fi);
            د->is_file = ngx_is_file(&fi);
            د->is_link = ngx_is_link(&fi);
            د->is_exec = ngx_is_exec(&fi);

            أرجع NGX_OK;
        }

        cln = ngx_pool_cleanup_جمع_کړه(pool, حجم(ngx_pool_cleanup_file_t));
        إذا (cln == عدم) {
            أرجع NGX_ERROR;
        }

        rc = ngx_open_and_stat_file(name, د, pool->لوګاریتم);

        إذا (rc == NGX_OK && !د->is_dir) {
            cln->handler = ngx_pool_cleanup_file;
            clnf = cln->معلومات;

            clnf->fd = د->fd;
            clnf->name = name->معلومات;
            clnf->لوګاریتم = pool->لوګاریتم;
        }

        أرجع rc;
    }

    cln = ngx_pool_cleanup_جمع_کړه(pool, حجم(ngx_open_file_cache_cleanup_t));
    إذا (cln == عدم) {
        أرجع NGX_ERROR;
    }

    now = ngx_وخت();

    hash = ngx_دوراني_ازموینه_32_long(name->معلومات, name->len);

    file = ngx_open_file_lookup(cache, name, hash);

    إذا (file) {

        file->کاروي++;

        ngx_کتار_remove(&file->کتار);

        إذا (file->fd == NGX_INVALID_FILE && file->err == 0 && !file->is_dir) {

            /* file was not used often enough to keep open */

            rc = ngx_open_and_stat_file(name, د, pool->لوګاریتم);

            إذا (rc != NGX_OK && (د->err == 0 || !د->errors)) {
                اذهب failed;
            }

            اذهب جمع_کړه_event;
        }

        إذا (file->use_event
            || (file->event == عدم
                && (د->uniq == 0 || د->uniq == file->uniq)
                && now - file->created < د->valid
#إذا (NGX_HAVE_OPENAT)
                && د->disable_symlinks == file->disable_symlinks
                && د->disable_symlinks_from == file->disable_symlinks_from
#د_که_پای
            ))
        {
            إذا (file->err == 0) {

                د->fd = file->fd;
                د->uniq = file->uniq;
                د->mtime = file->mtime;
                د->اندازه = file->اندازه;

                د->is_dir = file->is_dir;
                د->is_file = file->is_file;
                د->is_link = file->is_link;
                د->is_exec = file->is_exec;
                د->is_directio = file->is_directio;

                إذا (!file->is_dir) {
                    file->شمېر++;
                    ngx_open_file_جمع_کړه_event(cache, file, د, pool->لوګاریتم);
                }

            } وإلا {
                د->err = file->err;
#إذا (NGX_HAVE_OPENAT)
                د->failed = file->disable_symlinks ? ngx_openat_file_n
                                                    : ngx_open_file_n;
#وإلا
                د->failed = ngx_open_file_n;
#د_که_پای
            }

            اذهب وموندل;
        }

        ngx_log_debug4(NGX_LOG_DEBUG_CORE, pool->لوګاریتم, 0,
                       "retest open file: %s, fd:%d, c:%d, e:%d",
                       file->name, file->fd, file->شمېر, file->err);

        إذا (file->is_dir) {

            /*
             * chances that directory became file are very small
             * so test_dir flag allows to use a single syscall
             * in ngx_file_info() instead of three syscalls
             */

            د->test_dir = 1;
        }

        د->fd = file->fd;
        د->uniq = file->uniq;

        rc = ngx_open_and_stat_file(name, د, pool->لوګاریتم);

        إذا (rc != NGX_OK && (د->err == 0 || !د->errors)) {
            اذهب failed;
        }

        إذا (د->is_dir) {

            إذا (file->is_dir || file->err) {
                اذهب update;
            }

            /* file became directory */

        } وإلا إذا (د->err == 0) {  /* file */

            إذا (file->is_dir || file->err) {
                اذهب جمع_کړه_event;
            }

            إذا (د->uniq == file->uniq) {

                إذا (file->event) {
                    file->use_event = 1;
                }

                د->is_directio = file->is_directio;

                اذهب update;
            }

            /* file was changed */

        } وإلا { /* error to cache */

            إذا (file->err || file->is_dir) {
                اذهب update;
            }

            /* file was removed, etc. */
        }

        إذا (file->شمېر == 0) {

            ngx_open_file_del_event(file);

            إذا (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, pool->لوګاریتم, ngx_errno,
                              ngx_close_file_n " \"%V\" failed", name);
            }

            اذهب جمع_کړه_event;
        }

        ngx_rbtree_delete(&cache->rbtree, &file->غوټه);

        cache->current--;

        file->close = 1;

        اذهب create;
    }

    /* not found */

    rc = ngx_open_and_stat_file(name, د, pool->لوګاریتم);

    إذا (rc != NGX_OK && (د->err == 0 || !د->errors)) {
        اذهب failed;
    }

create:

    إذا (cache->current >= cache->max) {
        ngx_expire_old_cached_files(cache, 0, pool->لوګاریتم);
    }

    file = ngx_alloc(حجم(ngx_cached_open_file_t), pool->لوګاریتم);

    إذا (file == عدم) {
        اذهب failed;
    }

    file->name = ngx_alloc(name->len + 1, pool->لوګاریتم);

    إذا (file->name == عدم) {
        ngx_free(file);
        file = عدم;
        اذهب failed;
    }

    ngx_cpystrn(file->name, name->معلومات, name->len + 1);

    file->غوټه.کلی = hash;

    ngx_rbtree_ورننباسه(&cache->rbtree, &file->غوټه);

    cache->current++;

    file->کاروي = 1;
    file->شمېر = 0;
    file->use_event = 0;
    file->event = عدم;

جمع_کړه_event:

    ngx_open_file_جمع_کړه_event(cache, file, د, pool->لوګاریتم);

update:

    file->fd = د->fd;
    file->err = د->err;
#إذا (NGX_HAVE_OPENAT)
    file->disable_symlinks = د->disable_symlinks;
    file->disable_symlinks_from = د->disable_symlinks_from;
#د_که_پای

    إذا (د->err == 0) {
        file->uniq = د->uniq;
        file->mtime = د->mtime;
        file->اندازه = د->اندازه;

        file->close = 0;

        file->is_dir = د->is_dir;
        file->is_file = د->is_file;
        file->is_link = د->is_link;
        file->is_exec = د->is_exec;
        file->is_directio = د->is_directio;

        إذا (!د->is_dir) {
            file->شمېر++;
        }
    }

    file->created = now;

وموندل:

    file->accessed = now;

    ngx_کتار_ورننباسه_سر(&cache->expire_کتار, &file->کتار);

    ngx_log_debug5(NGX_LOG_DEBUG_CORE, pool->لوګاریتم, 0,
                   "cached open file: %s, fd:%d, c:%d, e:%d, u:%d",
                   file->name, file->fd, file->شمېر, file->err, file->کاروي);

    إذا (د->err == 0) {

        إذا (!د->is_dir) {
            cln->handler = ngx_open_file_cleanup;
            ofcln = cln->معلومات;

            ofcln->cache = cache;
            ofcln->file = file;
            ofcln->min_کاروي = د->min_کاروي;
            ofcln->لوګاریتم = pool->لوګاریتم;
        }

        أرجع NGX_OK;
    }

    أرجع NGX_ERROR;

failed:

    إذا (file) {
        ngx_rbtree_delete(&cache->rbtree, &file->غوټه);

        cache->current--;

        إذا (file->شمېر == 0) {

            إذا (file->fd != NGX_INVALID_FILE) {
                إذا (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_ALERT, pool->لوګاریتم, ngx_errno,
                                  ngx_close_file_n " \"%s\" failed",
                                  file->name);
                }
            }

            ngx_free(file->name);
            ngx_free(file);

        } وإلا {
            file->close = 1;
        }
    }

    إذا (د->fd != NGX_INVALID_FILE) {
        إذا (ngx_close_file(د->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, pool->لوګاریتم, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }
    }

    أرجع NGX_ERROR;
}


#إذا (NGX_HAVE_OPENAT)

ساكن ngx_fd_t
ngx_openat_file_owner(ngx_fd_t at_fd, ثابت u_char *name,
    ngx_int_t ډېرپېښ, ngx_int_t create, ngx_int_t access, ngx_log_t *لوګاریتم)
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

    fd = ngx_openat_file(at_fd, name, ډېرپېښ, create, access);

    إذا (fd == NGX_INVALID_FILE) {
        أرجع NGX_INVALID_FILE;
    }

    إذا (ngx_file_at_info(at_fd, name, &atfi, AT_SYMLINK_NOFOLLOW)
        == NGX_FILE_ERROR)
    {
        err = ngx_errno;
        اذهب failed;
    }

#إذا (NGX_HAVE_O_لار)
    إذا (ngx_file_o_لار_info(fd, &fi, لوګاریتم) == NGX_ERROR) {
        err = ngx_errno;
        اذهب failed;
    }
#وإلا
    إذا (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        err = ngx_errno;
        اذهب failed;
    }
#د_که_پای

    إذا (fi.st_uid != atfi.st_uid) {
        err = NGX_ELOOP;
        اذهب failed;
    }

    أرجع fd;

failed:

    إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, لوګاریتم, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", name);
    }

    ngx_سېټ_errno(err);

    أرجع NGX_INVALID_FILE;
}


#إذا (NGX_HAVE_O_لار)

ساكن ngx_int_t
ngx_file_o_لار_info(ngx_fd_t fd, ngx_file_info_t *fi, ngx_log_t *لوګاریتم)
{
    ساكن ngx_uint_t  use_fstat = 1;

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

    إذا (use_fstat) {
        إذا (ngx_fd_info(fd, fi) != NGX_FILE_ERROR) {
            أرجع NGX_OK;
        }

        إذا (ngx_errno != NGX_EBADF) {
            أرجع NGX_ERROR;
        }

        ngx_log_error(NGX_LOG_NOTICE, لوګاریتم, 0,
                      "fstat(O_PATH) failed with EBADF, "
                      "switching to fstatat(AT_EMPTY_PATH)");

        use_fstat = 0;
    }

    إذا (ngx_file_at_info(fd, "", fi, AT_تشه_لار) != NGX_FILE_ERROR) {
        أرجع NGX_OK;
    }

    أرجع NGX_ERROR;
}

#د_که_پای

#د_که_پای /* NGX_HAVE_OPENAT */


ساكن ngx_fd_t
ngx_open_file_wrapper(ngx_str_t *name, ngx_open_file_info_t *د,
    ngx_int_t ډېرپېښ, ngx_int_t create, ngx_int_t access, ngx_log_t *لوګاریتم)
{
    ngx_fd_t  fd;

#إذا !(NGX_HAVE_OPENAT)

    fd = ngx_open_file(name->معلومات, ډېرپېښ, create, access);

    إذا (fd == NGX_INVALID_FILE) {
        د->err = ngx_errno;
        د->failed = ngx_open_file_n;
        أرجع NGX_INVALID_FILE;
    }

    أرجع fd;

#وإلا

    u_char           *p, *cp, *پای_ټکی;
    ngx_fd_t          at_fd;
    ngx_str_t         at_name;

    إذا (د->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {
        fd = ngx_open_file(name->معلومات, ډېرپېښ, create, access);

        إذا (fd == NGX_INVALID_FILE) {
            د->err = ngx_errno;
            د->failed = ngx_open_file_n;
            أرجع NGX_INVALID_FILE;
        }

        أرجع fd;
    }

    p = name->معلومات;
    پای_ټکی = p + name->len;

    at_name = *name;

    إذا (د->disable_symlinks_from) {

        cp = p + د->disable_symlinks_from;

        *cp = '\0';

        at_fd = ngx_open_file(p, NGX_FILE_پلټنه|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        *cp = '/';

        إذا (at_fd == NGX_INVALID_FILE) {
            د->err = ngx_errno;
            د->failed = ngx_open_file_n;
            أرجع NGX_INVALID_FILE;
        }

        at_name.len = د->disable_symlinks_from;
        p = cp + 1;

    } وإلا إذا (*p == '/') {

        at_fd = ngx_open_file("/",
                              NGX_FILE_پلټنه|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        إذا (at_fd == NGX_INVALID_FILE) {
            د->err = ngx_errno;
            د->failed = ngx_openat_file_n;
            أرجع NGX_INVALID_FILE;
        }

        at_name.len = 1;
        p++;

    } وإلا {
        at_fd = NGX_AT_FDCWD;
    }

    لكل ( ;; ) {
        cp = ngx_strlchr(p, پای_ټکی, '/');
        إذا (cp == عدم) {
            اكسر;
        }

        إذا (cp == p) {
            p++;
            استمر;
        }

        *cp = '\0';

        إذا (د->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER) {
            fd = ngx_openat_file_owner(at_fd, p,
                                       NGX_FILE_پلټنه|NGX_FILE_NONBLOCK,
                                       NGX_FILE_OPEN, 0, لوګاریتم);

        } وإلا {
            fd = ngx_openat_file(at_fd, p,
                           NGX_FILE_پلټنه|NGX_FILE_NONBLOCK|NGX_FILE_NOFOLLOW,
                           NGX_FILE_OPEN, 0);
        }

        *cp = '/';

        إذا (fd == NGX_INVALID_FILE) {
            د->err = ngx_errno;
            د->failed = ngx_openat_file_n;
            اذهب failed;
        }

        إذا (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, لوګاریتم, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", &at_name);
        }

        p = cp + 1;
        at_fd = fd;
        at_name.len = cp - at_name.معلومات;
    }

    إذا (p == پای_ټکی) {

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

        fd = ngx_openat_file(at_fd, ".", ډېرپېښ, create, access);
        اذهب بشپړ;
    }

    إذا (د->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER
        && !(create & (NGX_FILE_CREATE_OR_OPEN|NGX_FILE_TRUNCATE)))
    {
        fd = ngx_openat_file_owner(at_fd, p, ډېرپېښ, create, access, لوګاریتم);

    } وإلا {
        fd = ngx_openat_file(at_fd, p, ډېرپېښ|NGX_FILE_NOFOLLOW, create, access);
    }

بشپړ:

    إذا (fd == NGX_INVALID_FILE) {
        د->err = ngx_errno;
        د->failed = ngx_openat_file_n;
    }

failed:

    إذا (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, لوګاریتم, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", &at_name);
    }

    أرجع fd;
#د_که_پای
}


ساكن ngx_int_t
ngx_file_info_wrapper(ngx_str_t *name, ngx_open_file_info_t *د,
    ngx_file_info_t *fi, ngx_log_t *لوګاریتم)
{
    ngx_int_t  rc;

#إذا !(NGX_HAVE_OPENAT)

    rc = ngx_file_info(name->معلومات, fi);

    إذا (rc == NGX_FILE_ERROR) {
        د->err = ngx_errno;
        د->failed = ngx_file_info_n;
        أرجع NGX_FILE_ERROR;
    }

    أرجع rc;

#وإلا

    ngx_fd_t  fd;

    إذا (د->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {

        rc = ngx_file_info(name->معلومات, fi);

        إذا (rc == NGX_FILE_ERROR) {
            د->err = ngx_errno;
            د->failed = ngx_file_info_n;
            أرجع NGX_FILE_ERROR;
        }

        أرجع rc;
    }

    fd = ngx_open_file_wrapper(name, د, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,
                               NGX_FILE_OPEN, 0, لوګاریتم);

    إذا (fd == NGX_INVALID_FILE) {
        أرجع NGX_FILE_ERROR;
    }

    rc = ngx_fd_info(fd, fi);

    إذا (rc == NGX_FILE_ERROR) {
        د->err = ngx_errno;
        د->failed = ngx_fd_info_n;
    }

    إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, لوګاریتم, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", name);
    }

    أرجع rc;
#د_که_پای
}


ساكن ngx_int_t
ngx_open_and_stat_file(ngx_str_t *name, ngx_open_file_info_t *د,
    ngx_log_t *لوګاریتم)
{
    ngx_fd_t         fd;
    ngx_file_info_t  fi;

    إذا (د->fd != NGX_INVALID_FILE) {

        إذا (ngx_file_info_wrapper(name, د, &fi, لوګاریتم) == NGX_FILE_ERROR) {
            د->fd = NGX_INVALID_FILE;
            أرجع NGX_ERROR;
        }

        إذا (د->uniq == ngx_file_uniq(&fi)) {
            اذهب بشپړ;
        }

    } وإلا إذا (د->test_dir) {

        إذا (ngx_file_info_wrapper(name, د, &fi, لوګاریتم) == NGX_FILE_ERROR) {
            د->fd = NGX_INVALID_FILE;
            أرجع NGX_ERROR;
        }

        إذا (ngx_is_dir(&fi)) {
            اذهب بشپړ;
        }
    }

    إذا (!د->لوګاریتم) {

        /*
         * Use non-blocking open() not to hang on FIFO files, etc.
         * This flag has no effect on a regular files.
         */

        fd = ngx_open_file_wrapper(name, د, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,
                                   NGX_FILE_OPEN, 0, لوګاریتم);

    } وإلا {
        fd = ngx_open_file_wrapper(name, د, NGX_FILE_APPEND,
                                   NGX_FILE_CREATE_OR_OPEN,
                                   NGX_FILE_DEFAULT_ACCESS, لوګاریتم);
    }

    إذا (fd == NGX_INVALID_FILE) {
        د->fd = NGX_INVALID_FILE;
        أرجع NGX_ERROR;
    }

    إذا (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, لوګاریتم, ngx_errno,
                      ngx_fd_info_n " \"%V\" failed", name);

        إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, لوګاریتم, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        د->fd = NGX_INVALID_FILE;

        أرجع NGX_ERROR;
    }

    إذا (ngx_is_dir(&fi)) {
        إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, لوګاریتم, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        د->fd = NGX_INVALID_FILE;

    } وإلا {
        د->fd = fd;

        إذا (د->read_ahead && ngx_file_اندازه(&fi) > NGX_MIN_READ_AHEAD) {
            إذا (ngx_read_ahead(fd, د->read_ahead) == NGX_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, لوګاریتم, ngx_errno,
                              ngx_read_ahead_n " \"%V\" failed", name);
            }
        }

        إذا (د->directio <= ngx_file_اندازه(&fi)) {
            إذا (ngx_directio_on(fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, لوګاریتم, ngx_errno,
                              ngx_directio_on_n " \"%V\" failed", name);

            } وإلا {
                د->is_directio = 1;
            }
        }
    }

بشپړ:

    د->uniq = ngx_file_uniq(&fi);
    د->mtime = ngx_file_mtime(&fi);
    د->اندازه = ngx_file_اندازه(&fi);
    د->fs_اندازه = ngx_file_fs_اندازه(&fi);
    د->is_dir = ngx_is_dir(&fi);
    د->is_file = ngx_is_file(&fi);
    د->is_link = ngx_is_link(&fi);
    د->is_exec = ngx_is_exec(&fi);

    أرجع NGX_OK;
}


/*
 * we ignore any possible event setting error and
 * fallback to usual periodic file retests
 */

ساكن فراغ
ngx_open_file_جمع_کړه_event(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_open_file_info_t *د, ngx_log_t *لوګاریتم)
{
    ngx_open_file_cache_event_t  *fev;

    إذا (!(ngx_event_flags & NGX_USE_VNODE_EVENT)
        || !د->events
        || file->event
        || د->fd == NGX_INVALID_FILE
        || file->کاروي < د->min_کاروي)
    {
        أرجع;
    }

    file->use_event = 0;

    file->event = ngx_calloc(حجم(ngx_event_t), لوګاریتم);
    إذا (file->event== عدم) {
        أرجع;
    }

    fev = ngx_alloc(حجم(ngx_open_file_cache_event_t), لوګاریتم);
    إذا (fev == عدم) {
        ngx_free(file->event);
        file->event = عدم;
        أرجع;
    }

    fev->fd = د->fd;
    fev->file = file;
    fev->cache = cache;

    file->event->handler = ngx_open_file_cache_remove;
    file->event->معلومات = fev;

    /*
     * although vnode event may be called while ngx_cycle->poll
     * destruction, however, cleanup procedures are run before any
     * memory freeing and events will be canceled.
     */

    file->event->لوګاریتم = ngx_دوره->لوګاریتم;

    إذا (ngx_جمع_کړه_event(file->event, NGX_VNODE_EVENT, NGX_ONESHOT_EVENT)
        != NGX_OK)
    {
        ngx_free(file->event->معلومات);
        ngx_free(file->event);
        file->event = عدم;
        أرجع;
    }

    /*
     * we do not set file->use_event here because there may be a race
     * condition: a file may be deleted between opening the file and
     * adding event, so we rely upon event notification only after
     * one file revalidation on next file access
     */

    أرجع;
}


ساكن فراغ
ngx_open_file_cleanup(فراغ *معلومات)
{
    ngx_open_file_cache_cleanup_t  *c = معلومات;

    c->file->شمېر--;

    ngx_close_cached_file(c->cache, c->file, c->min_کاروي, c->لوګاریتم);

    /* drop one or two expired open files */
    ngx_expire_old_cached_files(c->cache, 1, c->لوګاریتم);
}


ساكن فراغ
ngx_close_cached_file(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_uint_t min_کاروي, ngx_log_t *لوګاریتم)
{
    ngx_log_debug5(NGX_LOG_DEBUG_CORE, لوګاریتم, 0,
                   "close cached open file: %s, fd:%d, c:%d, u:%d, %d",
                   file->name, file->fd, file->شمېر, file->کاروي, file->close);

    إذا (!file->close) {

        file->accessed = ngx_وخت();

        ngx_کتار_remove(&file->کتار);

        ngx_کتار_ورننباسه_سر(&cache->expire_کتار, &file->کتار);

        إذا (file->کاروي >= min_کاروي || file->شمېر) {
            أرجع;
        }
    }

    ngx_open_file_del_event(file);

    إذا (file->شمېر) {
        أرجع;
    }

    إذا (file->fd != NGX_INVALID_FILE) {

        إذا (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, لوګاریتم, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", file->name);
        }

        file->fd = NGX_INVALID_FILE;
    }

    إذا (!file->close) {
        أرجع;
    }

    ngx_free(file->name);
    ngx_free(file);
}


ساكن فراغ
ngx_open_file_del_event(ngx_cached_open_file_t *file)
{
    إذا (file->event == عدم) {
        أرجع;
    }

    (فراغ) ngx_del_event(file->event, NGX_VNODE_EVENT,
                         file->شمېر ? NGX_FLUSH_EVENT : NGX_CLOSE_EVENT);

    ngx_free(file->event->معلومات);
    ngx_free(file->event);
    file->event = عدم;
    file->use_event = 0;
}


ساكن فراغ
ngx_expire_old_cached_files(ngx_open_file_cache_t *cache, ngx_uint_t n,
    ngx_log_t *لوګاریتم)
{
    وخت_t                   now;
    ngx_کتار_t             *q;
    ngx_cached_open_file_t  *file;

    now = ngx_وخت();

    /*
     * n == 1 deletes one or two inactive files
     * n == 0 deletes least recently used file by force
     *        and one or two inactive files
     */

    طالما (n < 3) {

        إذا (ngx_کتار_تشه(&cache->expire_کتار)) {
            أرجع;
        }

        q = ngx_کتار_وروستی(&cache->expire_کتار);

        file = ngx_کتار_معلومات(q, ngx_cached_open_file_t, کتار);

        إذا (n++ != 0 && now - file->accessed <= cache->inactive) {
            أرجع;
        }

        ngx_کتار_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->غوټه);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, لوګاریتم, 0,
                       "expire cached open file: %s", file->name);

        إذا (!file->err && !file->is_dir) {
            file->close = 1;
            ngx_close_cached_file(cache, file, 0, لوګاریتم);

        } وإلا {
            ngx_free(file->name);
            ngx_free(file);
        }
    }
}


ساكن فراغ
ngx_open_file_cache_rbtree_ورننباسه_ارزښت(ngx_rbtree_غوټه_t *لنډمهاله,
    ngx_rbtree_غوټه_t *غوټه, ngx_rbtree_غوټه_t *sentinel)
{
    ngx_rbtree_غوټه_t       **p;
    ngx_cached_open_file_t    *file, *file_لنډمهاله;

    لكل ( ;; ) {

        إذا (غوټه->کلی < لنډمهاله->کلی) {

            p = &لنډمهاله->کیڼ;

        } وإلا إذا (غوټه->کلی > لنډمهاله->کلی) {

            p = &لنډمهاله->ښی;

        } وإلا { /* node->key == temp->key */

            file = (ngx_cached_open_file_t *) غوټه;
            file_لنډمهاله = (ngx_cached_open_file_t *) لنډمهاله;

            p = (ngx_strcmp(file->name, file_لنډمهاله->name) < 0)
                    ? &لنډمهاله->کیڼ : &لنډمهاله->ښی;
        }

        إذا (*p == sentinel) {
            اكسر;
        }

        لنډمهاله = *p;
    }

    *p = غوټه;
    غوټه->پلار = لنډمهاله;
    غوټه->کیڼ = sentinel;
    غوټه->ښی = sentinel;
    ngx_rbt_red(غوټه);
}


ساكن ngx_cached_open_file_t *
ngx_open_file_lookup(ngx_open_file_cache_t *cache, ngx_str_t *name,
    uint32_t hash)
{
    ngx_int_t                rc;
    ngx_rbtree_غوټه_t       *غوټه, *sentinel;
    ngx_cached_open_file_t  *file;

    غوټه = cache->rbtree.ریښه;
    sentinel = cache->rbtree.sentinel;

    طالما (غوټه != sentinel) {

        إذا (hash < غوټه->کلی) {
            غوټه = غوټه->کیڼ;
            استمر;
        }

        إذا (hash > غوټه->کلی) {
            غوټه = غوټه->ښی;
            استمر;
        }

        /* hash == node->key */

        file = (ngx_cached_open_file_t *) غوټه;

        rc = ngx_strcmp(name->معلومات, file->name);

        إذا (rc == 0) {
            أرجع file;
        }

        غوټه = (rc < 0) ? غوټه->کیڼ : غوټه->ښی;
    }

    أرجع عدم;
}


ساكن فراغ
ngx_open_file_cache_remove(ngx_event_t *ev)
{
    ngx_cached_open_file_t       *file;
    ngx_open_file_cache_event_t  *fev;

    fev = ev->معلومات;
    file = fev->file;

    ngx_کتار_remove(&file->کتار);

    ngx_rbtree_delete(&fev->cache->rbtree, &file->غوټه);

    fev->cache->current--;

    /* NGX_ONESHOT_EVENT was already deleted */
    file->event = عدم;
    file->use_event = 0;

    file->close = 1;

    ngx_close_cached_file(fev->cache, file, 0, ev->لوګاریتم);

    /* free memory only when fev->cache and fev->file are already not needed */

    ngx_free(ev->معلومات);
    ngx_free(ev);
}
