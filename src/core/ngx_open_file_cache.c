
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


#تعریف_کن NGX_MIN_READ_AHEAD  (128 * 1024)


ساكن فراغ ngx_open_file_cache_cleanup(فراغ *داده);
#إذا (NGX_HAVE_OPENAT)
ساكن ngx_fd_t ngx_openat_file_owner(ngx_fd_t at_fd, ثابت u_char *name,
    ngx_int_t نما, ngx_int_t create, ngx_int_t access, ngx_log_t *لگاریتم);
#إذا (NGX_HAVE_O_مسیر)
ساكن ngx_int_t ngx_file_o_مسیر_info(ngx_fd_t fd, ngx_file_info_t *fi,
    ngx_log_t *لگاریتم);
#پایان_اگر
#پایان_اگر
ساكن ngx_fd_t ngx_open_file_wrapper(ngx_str_t *name,
    ngx_open_file_info_t *از, ngx_int_t نما, ngx_int_t create,
    ngx_int_t access, ngx_log_t *لگاریتم);
ساكن ngx_int_t ngx_file_info_wrapper(ngx_str_t *name,
    ngx_open_file_info_t *از, ngx_file_info_t *fi, ngx_log_t *لگاریتم);
ساكن ngx_int_t ngx_open_and_stat_file(ngx_str_t *name,
    ngx_open_file_info_t *از, ngx_log_t *لگاریتم);
ساكن فراغ ngx_open_file_جمع_کن_event(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_open_file_info_t *از, ngx_log_t *لگاریتم);
ساكن فراغ ngx_open_file_cleanup(فراغ *داده);
ساكن فراغ ngx_close_cached_file(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_uint_t min_استفاده_میکند, ngx_log_t *لگاریتم);
ساكن فراغ ngx_open_file_del_event(ngx_cached_open_file_t *file);
ساكن فراغ ngx_expire_old_cached_files(ngx_open_file_cache_t *cache,
    ngx_uint_t n, ngx_log_t *لگاریتم);
ساكن فراغ ngx_open_file_cache_rbtree_درج_مقدار(ngx_rbtree_گره_t *موقت,
    ngx_rbtree_گره_t *گره, ngx_rbtree_گره_t *sentinel);
ساكن ngx_cached_open_file_t *
    ngx_open_file_lookup(ngx_open_file_cache_t *cache, ngx_str_t *name,
    uint32_t hash);
ساكن فراغ ngx_open_file_cache_remove(ngx_event_t *ev);


ngx_open_file_cache_t *
ngx_open_file_cache_init(ngx_pool_t *pool, ngx_uint_t max, زمان_t inactive)
{
    ngx_pool_cleanup_t     *cln;
    ngx_open_file_cache_t  *cache;

    cache = ngx_palloc(pool, حجم(ngx_open_file_cache_t));
    إذا (cache == عدم) {
        أرجع عدم;
    }

    ngx_rbtree_init(&cache->rbtree, &cache->sentinel,
                    ngx_open_file_cache_rbtree_درج_مقدار);

    ngx_صف_init(&cache->expire_صف);

    cache->current = 0;
    cache->max = max;
    cache->inactive = inactive;

    cln = ngx_pool_cleanup_جمع_کن(pool, 0);
    إذا (cln == عدم) {
        أرجع عدم;
    }

    cln->handler = ngx_open_file_cache_cleanup;
    cln->داده = cache;

    أرجع cache;
}


ساكن فراغ
ngx_open_file_cache_cleanup(فراغ *داده)
{
    ngx_open_file_cache_t  *cache = داده;

    ngx_صف_t             *q;
    ngx_cached_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_چرخه->لگاریتم, 0,
                   "open file cache cleanup");

    لكل ( ;; ) {

        إذا (ngx_صف_خالی(&cache->expire_صف)) {
            اكسر;
        }

        q = ngx_صف_آخرین(&cache->expire_صف);

        file = ngx_صف_داده(q, ngx_cached_open_file_t, صف);

        ngx_صف_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->گره);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_چرخه->لگاریتم, 0,
                       "delete cached open file: %s", file->name);

        إذا (!file->err && !file->is_dir) {
            file->close = 1;
            file->شمار = 0;
            ngx_close_cached_file(cache, file, 0, ngx_چرخه->لگاریتم);

        } وإلا {
            ngx_free(file->name);
            ngx_free(file);
        }
    }

    إذا (cache->current) {
        ngx_log_error(NGX_LOG_ALERT, ngx_چرخه->لگاریتم, 0,
                      "%ui items still left in open file cache",
                      cache->current);
    }

    إذا (cache->rbtree.ریشه != cache->rbtree.sentinel) {
        ngx_log_error(NGX_LOG_ALERT, ngx_چرخه->لگاریتم, 0,
                      "rbtree still is not empty in open file cache");

    }
}


ngx_int_t
ngx_open_cached_file(ngx_open_file_cache_t *cache, ngx_str_t *name,
    ngx_open_file_info_t *از, ngx_pool_t *pool)
{
    زمان_t                          now;
    uint32_t                        hash;
    ngx_int_t                       rc;
    ngx_file_info_t                 fi;
    ngx_pool_cleanup_t             *cln;
    ngx_cached_open_file_t         *file;
    ngx_pool_cleanup_file_t        *clnf;
    ngx_open_file_cache_cleanup_t  *ofcln;

    از->fd = NGX_INVALID_FILE;
    از->err = 0;

    إذا (cache == عدم) {

        إذا (از->test_only) {

            إذا (ngx_file_info_wrapper(name, از, &fi, pool->لگاریتم)
                == NGX_FILE_ERROR)
            {
                أرجع NGX_ERROR;
            }

            از->uniq = ngx_file_uniq(&fi);
            از->mtime = ngx_file_mtime(&fi);
            از->اندازه = ngx_file_اندازه(&fi);
            از->fs_اندازه = ngx_file_fs_اندازه(&fi);
            از->is_dir = ngx_is_dir(&fi);
            از->is_file = ngx_is_file(&fi);
            از->is_link = ngx_is_link(&fi);
            از->is_exec = ngx_is_exec(&fi);

            أرجع NGX_OK;
        }

        cln = ngx_pool_cleanup_جمع_کن(pool, حجم(ngx_pool_cleanup_file_t));
        إذا (cln == عدم) {
            أرجع NGX_ERROR;
        }

        rc = ngx_open_and_stat_file(name, از, pool->لگاریتم);

        إذا (rc == NGX_OK && !از->is_dir) {
            cln->handler = ngx_pool_cleanup_file;
            clnf = cln->داده;

            clnf->fd = از->fd;
            clnf->name = name->داده;
            clnf->لگاریتم = pool->لگاریتم;
        }

        أرجع rc;
    }

    cln = ngx_pool_cleanup_جمع_کن(pool, حجم(ngx_open_file_cache_cleanup_t));
    إذا (cln == عدم) {
        أرجع NGX_ERROR;
    }

    now = ngx_زمان();

    hash = ngx_بررسی_چرخهای_32_long(name->داده, name->len);

    file = ngx_open_file_lookup(cache, name, hash);

    إذا (file) {

        file->استفاده_میکند++;

        ngx_صف_remove(&file->صف);

        إذا (file->fd == NGX_INVALID_FILE && file->err == 0 && !file->is_dir) {

            /* file was not used often enough to keep open */

            rc = ngx_open_and_stat_file(name, از, pool->لگاریتم);

            إذا (rc != NGX_OK && (از->err == 0 || !از->errors)) {
                اذهب failed;
            }

            اذهب جمع_کن_event;
        }

        إذا (file->use_event
            || (file->event == عدم
                && (از->uniq == 0 || از->uniq == file->uniq)
                && now - file->created < از->valid
#إذا (NGX_HAVE_OPENAT)
                && از->disable_symlinks == file->disable_symlinks
                && از->disable_symlinks_from == file->disable_symlinks_from
#پایان_اگر
            ))
        {
            إذا (file->err == 0) {

                از->fd = file->fd;
                از->uniq = file->uniq;
                از->mtime = file->mtime;
                از->اندازه = file->اندازه;

                از->is_dir = file->is_dir;
                از->is_file = file->is_file;
                از->is_link = file->is_link;
                از->is_exec = file->is_exec;
                از->is_directio = file->is_directio;

                إذا (!file->is_dir) {
                    file->شمار++;
                    ngx_open_file_جمع_کن_event(cache, file, از, pool->لگاریتم);
                }

            } وإلا {
                از->err = file->err;
#إذا (NGX_HAVE_OPENAT)
                از->failed = file->disable_symlinks ? ngx_openat_file_n
                                                    : ngx_open_file_n;
#وإلا
                از->failed = ngx_open_file_n;
#پایان_اگر
            }

            اذهب یافت;
        }

        ngx_log_debug4(NGX_LOG_DEBUG_CORE, pool->لگاریتم, 0,
                       "retest open file: %s, fd:%d, c:%d, e:%d",
                       file->name, file->fd, file->شمار, file->err);

        إذا (file->is_dir) {

            /*
             * chances that directory became file are very small
             * so test_dir flag allows to use a single syscall
             * in ngx_file_info() instead of three syscalls
             */

            از->test_dir = 1;
        }

        از->fd = file->fd;
        از->uniq = file->uniq;

        rc = ngx_open_and_stat_file(name, از, pool->لگاریتم);

        إذا (rc != NGX_OK && (از->err == 0 || !از->errors)) {
            اذهب failed;
        }

        إذا (از->is_dir) {

            إذا (file->is_dir || file->err) {
                اذهب update;
            }

            /* file became directory */

        } وإلا إذا (از->err == 0) {  /* file */

            إذا (file->is_dir || file->err) {
                اذهب جمع_کن_event;
            }

            إذا (از->uniq == file->uniq) {

                إذا (file->event) {
                    file->use_event = 1;
                }

                از->is_directio = file->is_directio;

                اذهب update;
            }

            /* file was changed */

        } وإلا { /* error to cache */

            إذا (file->err || file->is_dir) {
                اذهب update;
            }

            /* file was removed, etc. */
        }

        إذا (file->شمار == 0) {

            ngx_open_file_del_event(file);

            إذا (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, pool->لگاریتم, ngx_errno,
                              ngx_close_file_n " \"%V\" failed", name);
            }

            اذهب جمع_کن_event;
        }

        ngx_rbtree_delete(&cache->rbtree, &file->گره);

        cache->current--;

        file->close = 1;

        اذهب create;
    }

    /* not found */

    rc = ngx_open_and_stat_file(name, از, pool->لگاریتم);

    إذا (rc != NGX_OK && (از->err == 0 || !از->errors)) {
        اذهب failed;
    }

create:

    إذا (cache->current >= cache->max) {
        ngx_expire_old_cached_files(cache, 0, pool->لگاریتم);
    }

    file = ngx_alloc(حجم(ngx_cached_open_file_t), pool->لگاریتم);

    إذا (file == عدم) {
        اذهب failed;
    }

    file->name = ngx_alloc(name->len + 1, pool->لگاریتم);

    إذا (file->name == عدم) {
        ngx_free(file);
        file = عدم;
        اذهب failed;
    }

    ngx_cpystrn(file->name, name->داده, name->len + 1);

    file->گره.کلید = hash;

    ngx_rbtree_درج(&cache->rbtree, &file->گره);

    cache->current++;

    file->استفاده_میکند = 1;
    file->شمار = 0;
    file->use_event = 0;
    file->event = عدم;

جمع_کن_event:

    ngx_open_file_جمع_کن_event(cache, file, از, pool->لگاریتم);

update:

    file->fd = از->fd;
    file->err = از->err;
#إذا (NGX_HAVE_OPENAT)
    file->disable_symlinks = از->disable_symlinks;
    file->disable_symlinks_from = از->disable_symlinks_from;
#پایان_اگر

    إذا (از->err == 0) {
        file->uniq = از->uniq;
        file->mtime = از->mtime;
        file->اندازه = از->اندازه;

        file->close = 0;

        file->is_dir = از->is_dir;
        file->is_file = از->is_file;
        file->is_link = از->is_link;
        file->is_exec = از->is_exec;
        file->is_directio = از->is_directio;

        إذا (!از->is_dir) {
            file->شمار++;
        }
    }

    file->created = now;

یافت:

    file->accessed = now;

    ngx_صف_درج_سر(&cache->expire_صف, &file->صف);

    ngx_log_debug5(NGX_LOG_DEBUG_CORE, pool->لگاریتم, 0,
                   "cached open file: %s, fd:%d, c:%d, e:%d, u:%d",
                   file->name, file->fd, file->شمار, file->err, file->استفاده_میکند);

    إذا (از->err == 0) {

        إذا (!از->is_dir) {
            cln->handler = ngx_open_file_cleanup;
            ofcln = cln->داده;

            ofcln->cache = cache;
            ofcln->file = file;
            ofcln->min_استفاده_میکند = از->min_استفاده_میکند;
            ofcln->لگاریتم = pool->لگاریتم;
        }

        أرجع NGX_OK;
    }

    أرجع NGX_ERROR;

failed:

    إذا (file) {
        ngx_rbtree_delete(&cache->rbtree, &file->گره);

        cache->current--;

        إذا (file->شمار == 0) {

            إذا (file->fd != NGX_INVALID_FILE) {
                إذا (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_ALERT, pool->لگاریتم, ngx_errno,
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

    إذا (از->fd != NGX_INVALID_FILE) {
        إذا (ngx_close_file(از->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, pool->لگاریتم, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }
    }

    أرجع NGX_ERROR;
}


#إذا (NGX_HAVE_OPENAT)

ساكن ngx_fd_t
ngx_openat_file_owner(ngx_fd_t at_fd, ثابت u_char *name,
    ngx_int_t نما, ngx_int_t create, ngx_int_t access, ngx_log_t *لگاریتم)
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

    fd = ngx_openat_file(at_fd, name, نما, create, access);

    إذا (fd == NGX_INVALID_FILE) {
        أرجع NGX_INVALID_FILE;
    }

    إذا (ngx_file_at_info(at_fd, name, &atfi, AT_SYMLINK_NOFOLLOW)
        == NGX_FILE_ERROR)
    {
        err = ngx_errno;
        اذهب failed;
    }

#إذا (NGX_HAVE_O_مسیر)
    إذا (ngx_file_o_مسیر_info(fd, &fi, لگاریتم) == NGX_ERROR) {
        err = ngx_errno;
        اذهب failed;
    }
#وإلا
    إذا (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        err = ngx_errno;
        اذهب failed;
    }
#پایان_اگر

    إذا (fi.st_uid != atfi.st_uid) {
        err = NGX_ELOOP;
        اذهب failed;
    }

    أرجع fd;

failed:

    إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, لگاریتم, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", name);
    }

    ngx_مجموعه_errno(err);

    أرجع NGX_INVALID_FILE;
}


#إذا (NGX_HAVE_O_مسیر)

ساكن ngx_int_t
ngx_file_o_مسیر_info(ngx_fd_t fd, ngx_file_info_t *fi, ngx_log_t *لگاریتم)
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

        ngx_log_error(NGX_LOG_NOTICE, لگاریتم, 0,
                      "fstat(O_PATH) failed with EBADF, "
                      "switching to fstatat(AT_EMPTY_PATH)");

        use_fstat = 0;
    }

    إذا (ngx_file_at_info(fd, "", fi, AT_خالی_مسیر) != NGX_FILE_ERROR) {
        أرجع NGX_OK;
    }

    أرجع NGX_ERROR;
}

#پایان_اگر

#پایان_اگر /* NGX_HAVE_OPENAT */


ساكن ngx_fd_t
ngx_open_file_wrapper(ngx_str_t *name, ngx_open_file_info_t *از,
    ngx_int_t نما, ngx_int_t create, ngx_int_t access, ngx_log_t *لگاریتم)
{
    ngx_fd_t  fd;

#إذا !(NGX_HAVE_OPENAT)

    fd = ngx_open_file(name->داده, نما, create, access);

    إذا (fd == NGX_INVALID_FILE) {
        از->err = ngx_errno;
        از->failed = ngx_open_file_n;
        أرجع NGX_INVALID_FILE;
    }

    أرجع fd;

#وإلا

    u_char           *p, *cp, *انتها;
    ngx_fd_t          at_fd;
    ngx_str_t         at_name;

    إذا (از->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {
        fd = ngx_open_file(name->داده, نما, create, access);

        إذا (fd == NGX_INVALID_FILE) {
            از->err = ngx_errno;
            از->failed = ngx_open_file_n;
            أرجع NGX_INVALID_FILE;
        }

        أرجع fd;
    }

    p = name->داده;
    انتها = p + name->len;

    at_name = *name;

    إذا (از->disable_symlinks_from) {

        cp = p + از->disable_symlinks_from;

        *cp = '\0';

        at_fd = ngx_open_file(p, NGX_FILE_جستجو|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        *cp = '/';

        إذا (at_fd == NGX_INVALID_FILE) {
            از->err = ngx_errno;
            از->failed = ngx_open_file_n;
            أرجع NGX_INVALID_FILE;
        }

        at_name.len = از->disable_symlinks_from;
        p = cp + 1;

    } وإلا إذا (*p == '/') {

        at_fd = ngx_open_file("/",
                              NGX_FILE_جستجو|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        إذا (at_fd == NGX_INVALID_FILE) {
            از->err = ngx_errno;
            از->failed = ngx_openat_file_n;
            أرجع NGX_INVALID_FILE;
        }

        at_name.len = 1;
        p++;

    } وإلا {
        at_fd = NGX_AT_FDCWD;
    }

    لكل ( ;; ) {
        cp = ngx_strlchr(p, انتها, '/');
        إذا (cp == عدم) {
            اكسر;
        }

        إذا (cp == p) {
            p++;
            استمر;
        }

        *cp = '\0';

        إذا (از->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER) {
            fd = ngx_openat_file_owner(at_fd, p,
                                       NGX_FILE_جستجو|NGX_FILE_NONBLOCK,
                                       NGX_FILE_OPEN, 0, لگاریتم);

        } وإلا {
            fd = ngx_openat_file(at_fd, p,
                           NGX_FILE_جستجو|NGX_FILE_NONBLOCK|NGX_FILE_NOFOLLOW,
                           NGX_FILE_OPEN, 0);
        }

        *cp = '/';

        إذا (fd == NGX_INVALID_FILE) {
            از->err = ngx_errno;
            از->failed = ngx_openat_file_n;
            اذهب failed;
        }

        إذا (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, لگاریتم, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", &at_name);
        }

        p = cp + 1;
        at_fd = fd;
        at_name.len = cp - at_name.داده;
    }

    إذا (p == انتها) {

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

        fd = ngx_openat_file(at_fd, ".", نما, create, access);
        اذهب انجامشده;
    }

    إذا (از->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER
        && !(create & (NGX_FILE_CREATE_OR_OPEN|NGX_FILE_TRUNCATE)))
    {
        fd = ngx_openat_file_owner(at_fd, p, نما, create, access, لگاریتم);

    } وإلا {
        fd = ngx_openat_file(at_fd, p, نما|NGX_FILE_NOFOLLOW, create, access);
    }

انجامشده:

    إذا (fd == NGX_INVALID_FILE) {
        از->err = ngx_errno;
        از->failed = ngx_openat_file_n;
    }

failed:

    إذا (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, لگاریتم, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", &at_name);
    }

    أرجع fd;
#پایان_اگر
}


ساكن ngx_int_t
ngx_file_info_wrapper(ngx_str_t *name, ngx_open_file_info_t *از,
    ngx_file_info_t *fi, ngx_log_t *لگاریتم)
{
    ngx_int_t  rc;

#إذا !(NGX_HAVE_OPENAT)

    rc = ngx_file_info(name->داده, fi);

    إذا (rc == NGX_FILE_ERROR) {
        از->err = ngx_errno;
        از->failed = ngx_file_info_n;
        أرجع NGX_FILE_ERROR;
    }

    أرجع rc;

#وإلا

    ngx_fd_t  fd;

    إذا (از->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {

        rc = ngx_file_info(name->داده, fi);

        إذا (rc == NGX_FILE_ERROR) {
            از->err = ngx_errno;
            از->failed = ngx_file_info_n;
            أرجع NGX_FILE_ERROR;
        }

        أرجع rc;
    }

    fd = ngx_open_file_wrapper(name, از, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,
                               NGX_FILE_OPEN, 0, لگاریتم);

    إذا (fd == NGX_INVALID_FILE) {
        أرجع NGX_FILE_ERROR;
    }

    rc = ngx_fd_info(fd, fi);

    إذا (rc == NGX_FILE_ERROR) {
        از->err = ngx_errno;
        از->failed = ngx_fd_info_n;
    }

    إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, لگاریتم, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", name);
    }

    أرجع rc;
#پایان_اگر
}


ساكن ngx_int_t
ngx_open_and_stat_file(ngx_str_t *name, ngx_open_file_info_t *از,
    ngx_log_t *لگاریتم)
{
    ngx_fd_t         fd;
    ngx_file_info_t  fi;

    إذا (از->fd != NGX_INVALID_FILE) {

        إذا (ngx_file_info_wrapper(name, از, &fi, لگاریتم) == NGX_FILE_ERROR) {
            از->fd = NGX_INVALID_FILE;
            أرجع NGX_ERROR;
        }

        إذا (از->uniq == ngx_file_uniq(&fi)) {
            اذهب انجامشده;
        }

    } وإلا إذا (از->test_dir) {

        إذا (ngx_file_info_wrapper(name, از, &fi, لگاریتم) == NGX_FILE_ERROR) {
            از->fd = NGX_INVALID_FILE;
            أرجع NGX_ERROR;
        }

        إذا (ngx_is_dir(&fi)) {
            اذهب انجامشده;
        }
    }

    إذا (!از->لگاریتم) {

        /*
         * Use non-blocking open() not to hang on FIFO files, etc.
         * This flag has no effect on a regular files.
         */

        fd = ngx_open_file_wrapper(name, از, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,
                                   NGX_FILE_OPEN, 0, لگاریتم);

    } وإلا {
        fd = ngx_open_file_wrapper(name, از, NGX_FILE_APPEND,
                                   NGX_FILE_CREATE_OR_OPEN,
                                   NGX_FILE_DEFAULT_ACCESS, لگاریتم);
    }

    إذا (fd == NGX_INVALID_FILE) {
        از->fd = NGX_INVALID_FILE;
        أرجع NGX_ERROR;
    }

    إذا (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, لگاریتم, ngx_errno,
                      ngx_fd_info_n " \"%V\" failed", name);

        إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, لگاریتم, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        از->fd = NGX_INVALID_FILE;

        أرجع NGX_ERROR;
    }

    إذا (ngx_is_dir(&fi)) {
        إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, لگاریتم, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        از->fd = NGX_INVALID_FILE;

    } وإلا {
        از->fd = fd;

        إذا (از->read_ahead && ngx_file_اندازه(&fi) > NGX_MIN_READ_AHEAD) {
            إذا (ngx_read_ahead(fd, از->read_ahead) == NGX_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, لگاریتم, ngx_errno,
                              ngx_read_ahead_n " \"%V\" failed", name);
            }
        }

        إذا (از->directio <= ngx_file_اندازه(&fi)) {
            إذا (ngx_directio_on(fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, لگاریتم, ngx_errno,
                              ngx_directio_on_n " \"%V\" failed", name);

            } وإلا {
                از->is_directio = 1;
            }
        }
    }

انجامشده:

    از->uniq = ngx_file_uniq(&fi);
    از->mtime = ngx_file_mtime(&fi);
    از->اندازه = ngx_file_اندازه(&fi);
    از->fs_اندازه = ngx_file_fs_اندازه(&fi);
    از->is_dir = ngx_is_dir(&fi);
    از->is_file = ngx_is_file(&fi);
    از->is_link = ngx_is_link(&fi);
    از->is_exec = ngx_is_exec(&fi);

    أرجع NGX_OK;
}


/*
 * we ignore any possible event setting error and
 * fallback to usual periodic file retests
 */

ساكن فراغ
ngx_open_file_جمع_کن_event(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_open_file_info_t *از, ngx_log_t *لگاریتم)
{
    ngx_open_file_cache_event_t  *fev;

    إذا (!(ngx_event_flags & NGX_USE_VNODE_EVENT)
        || !از->events
        || file->event
        || از->fd == NGX_INVALID_FILE
        || file->استفاده_میکند < از->min_استفاده_میکند)
    {
        أرجع;
    }

    file->use_event = 0;

    file->event = ngx_calloc(حجم(ngx_event_t), لگاریتم);
    إذا (file->event== عدم) {
        أرجع;
    }

    fev = ngx_alloc(حجم(ngx_open_file_cache_event_t), لگاریتم);
    إذا (fev == عدم) {
        ngx_free(file->event);
        file->event = عدم;
        أرجع;
    }

    fev->fd = از->fd;
    fev->file = file;
    fev->cache = cache;

    file->event->handler = ngx_open_file_cache_remove;
    file->event->داده = fev;

    /*
     * although vnode event may be called while ngx_cycle->poll
     * destruction, however, cleanup procedures are run before any
     * memory freeing and events will be canceled.
     */

    file->event->لگاریتم = ngx_چرخه->لگاریتم;

    إذا (ngx_جمع_کن_event(file->event, NGX_VNODE_EVENT, NGX_ONESHOT_EVENT)
        != NGX_OK)
    {
        ngx_free(file->event->داده);
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
ngx_open_file_cleanup(فراغ *داده)
{
    ngx_open_file_cache_cleanup_t  *c = داده;

    c->file->شمار--;

    ngx_close_cached_file(c->cache, c->file, c->min_استفاده_میکند, c->لگاریتم);

    /* drop one or two expired open files */
    ngx_expire_old_cached_files(c->cache, 1, c->لگاریتم);
}


ساكن فراغ
ngx_close_cached_file(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_uint_t min_استفاده_میکند, ngx_log_t *لگاریتم)
{
    ngx_log_debug5(NGX_LOG_DEBUG_CORE, لگاریتم, 0,
                   "close cached open file: %s, fd:%d, c:%d, u:%d, %d",
                   file->name, file->fd, file->شمار, file->استفاده_میکند, file->close);

    إذا (!file->close) {

        file->accessed = ngx_زمان();

        ngx_صف_remove(&file->صف);

        ngx_صف_درج_سر(&cache->expire_صف, &file->صف);

        إذا (file->استفاده_میکند >= min_استفاده_میکند || file->شمار) {
            أرجع;
        }
    }

    ngx_open_file_del_event(file);

    إذا (file->شمار) {
        أرجع;
    }

    إذا (file->fd != NGX_INVALID_FILE) {

        إذا (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, لگاریتم, ngx_errno,
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
                         file->شمار ? NGX_FLUSH_EVENT : NGX_CLOSE_EVENT);

    ngx_free(file->event->داده);
    ngx_free(file->event);
    file->event = عدم;
    file->use_event = 0;
}


ساكن فراغ
ngx_expire_old_cached_files(ngx_open_file_cache_t *cache, ngx_uint_t n,
    ngx_log_t *لگاریتم)
{
    زمان_t                   now;
    ngx_صف_t             *q;
    ngx_cached_open_file_t  *file;

    now = ngx_زمان();

    /*
     * n == 1 deletes one or two inactive files
     * n == 0 deletes least recently used file by force
     *        and one or two inactive files
     */

    طالما (n < 3) {

        إذا (ngx_صف_خالی(&cache->expire_صف)) {
            أرجع;
        }

        q = ngx_صف_آخرین(&cache->expire_صف);

        file = ngx_صف_داده(q, ngx_cached_open_file_t, صف);

        إذا (n++ != 0 && now - file->accessed <= cache->inactive) {
            أرجع;
        }

        ngx_صف_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->گره);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, لگاریتم, 0,
                       "expire cached open file: %s", file->name);

        إذا (!file->err && !file->is_dir) {
            file->close = 1;
            ngx_close_cached_file(cache, file, 0, لگاریتم);

        } وإلا {
            ngx_free(file->name);
            ngx_free(file);
        }
    }
}


ساكن فراغ
ngx_open_file_cache_rbtree_درج_مقدار(ngx_rbtree_گره_t *موقت,
    ngx_rbtree_گره_t *گره, ngx_rbtree_گره_t *sentinel)
{
    ngx_rbtree_گره_t       **p;
    ngx_cached_open_file_t    *file, *file_موقت;

    لكل ( ;; ) {

        إذا (گره->کلید < موقت->کلید) {

            p = &موقت->چپ;

        } وإلا إذا (گره->کلید > موقت->کلید) {

            p = &موقت->راست;

        } وإلا { /* node->key == temp->key */

            file = (ngx_cached_open_file_t *) گره;
            file_موقت = (ngx_cached_open_file_t *) موقت;

            p = (ngx_strcmp(file->name, file_موقت->name) < 0)
                    ? &موقت->چپ : &موقت->راست;
        }

        إذا (*p == sentinel) {
            اكسر;
        }

        موقت = *p;
    }

    *p = گره;
    گره->والد = موقت;
    گره->چپ = sentinel;
    گره->راست = sentinel;
    ngx_rbt_red(گره);
}


ساكن ngx_cached_open_file_t *
ngx_open_file_lookup(ngx_open_file_cache_t *cache, ngx_str_t *name,
    uint32_t hash)
{
    ngx_int_t                rc;
    ngx_rbtree_گره_t       *گره, *sentinel;
    ngx_cached_open_file_t  *file;

    گره = cache->rbtree.ریشه;
    sentinel = cache->rbtree.sentinel;

    طالما (گره != sentinel) {

        إذا (hash < گره->کلید) {
            گره = گره->چپ;
            استمر;
        }

        إذا (hash > گره->کلید) {
            گره = گره->راست;
            استمر;
        }

        /* hash == node->key */

        file = (ngx_cached_open_file_t *) گره;

        rc = ngx_strcmp(name->داده, file->name);

        إذا (rc == 0) {
            أرجع file;
        }

        گره = (rc < 0) ? گره->چپ : گره->راست;
    }

    أرجع عدم;
}


ساكن فراغ
ngx_open_file_cache_remove(ngx_event_t *ev)
{
    ngx_cached_open_file_t       *file;
    ngx_open_file_cache_event_t  *fev;

    fev = ev->داده;
    file = fev->file;

    ngx_صف_remove(&file->صف);

    ngx_rbtree_delete(&fev->cache->rbtree, &file->گره);

    fev->cache->current--;

    /* NGX_ONESHOT_EVENT was already deleted */
    file->event = عدم;
    file->use_event = 0;

    file->close = 1;

    ngx_close_cached_file(fev->cache, file, 0, ev->لگاریتم);

    /* free memory only when fev->cache and fev->file are already not needed */

    ngx_free(ev->داده);
    ngx_free(ev);
}
