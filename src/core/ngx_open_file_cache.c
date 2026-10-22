
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


#تعریف_کرو NGX_MIN_READ_AHEAD  (128 * 1024)


ساكن فراغ ngx_open_file_cache_cleanup(فراغ *انکڑے);
#إذا (NGX_HAVE_OPENAT)
ساكن ngx_fd_t ngx_openat_file_owner(ngx_fd_t at_fd, ثابت u_char *name,
    ngx_int_t منوال, ngx_int_t create, ngx_int_t access, ngx_log_t *لوگارتھم);
#إذا (NGX_HAVE_O_رستہ)
ساكن ngx_int_t ngx_file_o_رستہ_info(ngx_fd_t fd, ngx_file_info_t *fi,
    ngx_log_t *لوگارتھم);
#جے_انت
#جے_انت
ساكن ngx_fd_t ngx_open_file_wrapper(ngx_str_t *name,
    ngx_open_file_info_t *دا, ngx_int_t منوال, ngx_int_t create,
    ngx_int_t access, ngx_log_t *لوگارتھم);
ساكن ngx_int_t ngx_file_info_wrapper(ngx_str_t *name,
    ngx_open_file_info_t *دا, ngx_file_info_t *fi, ngx_log_t *لوگارتھم);
ساكن ngx_int_t ngx_open_and_stat_file(ngx_str_t *name,
    ngx_open_file_info_t *دا, ngx_log_t *لوگارتھم);
ساكن فراغ ngx_open_file_جوڑو_event(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_open_file_info_t *دا, ngx_log_t *لوگارتھم);
ساكن فراغ ngx_open_file_cleanup(فراغ *انکڑے);
ساكن فراغ ngx_close_cached_file(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_uint_t min_ورتدا, ngx_log_t *لوگارتھم);
ساكن فراغ ngx_open_file_del_event(ngx_cached_open_file_t *file);
ساكن فراغ ngx_expire_old_cached_files(ngx_open_file_cache_t *cache,
    ngx_uint_t n, ngx_log_t *لوگارتھم);
ساكن فراغ ngx_open_file_cache_rbtree_داخل_کرو_قدر(ngx_rbtree_گرہ_t *عارضی,
    ngx_rbtree_گرہ_t *گرہ, ngx_rbtree_گرہ_t *sentinel);
ساكن ngx_cached_open_file_t *
    ngx_open_file_lookup(ngx_open_file_cache_t *cache, ngx_str_t *name,
    uint32_t hash);
ساكن فراغ ngx_open_file_cache_remove(ngx_event_t *ev);


ngx_open_file_cache_t *
ngx_open_file_cache_init(ngx_pool_t *pool, ngx_uint_t max, ویلا_t inactive)
{
    ngx_pool_cleanup_t     *cln;
    ngx_open_file_cache_t  *cache;

    cache = ngx_palloc(pool, حجم(ngx_open_file_cache_t));
    إذا (cache == عدم) {
        أرجع عدم;
    }

    ngx_rbtree_init(&cache->rbtree, &cache->sentinel,
                    ngx_open_file_cache_rbtree_داخل_کرو_قدر);

    ngx_قطار_init(&cache->expire_قطار);

    cache->current = 0;
    cache->max = max;
    cache->inactive = inactive;

    cln = ngx_pool_cleanup_جوڑو(pool, 0);
    إذا (cln == عدم) {
        أرجع عدم;
    }

    cln->handler = ngx_open_file_cache_cleanup;
    cln->انکڑے = cache;

    أرجع cache;
}


ساكن فراغ
ngx_open_file_cache_cleanup(فراغ *انکڑے)
{
    ngx_open_file_cache_t  *cache = انکڑے;

    ngx_قطار_t             *q;
    ngx_cached_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_چکر->لوگارتھم, 0,
                   "open file cache cleanup");

    لكل ( ;; ) {

        إذا (ngx_قطار_خالی_تھاواں(&cache->expire_قطار)) {
            اكسر;
        }

        q = ngx_قطار_آخری(&cache->expire_قطار);

        file = ngx_قطار_انکڑے(q, ngx_cached_open_file_t, قطار);

        ngx_قطار_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->گرہ);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_چکر->لوگارتھم, 0,
                       "delete cached open file: %s", file->name);

        إذا (!file->err && !file->is_dir) {
            file->close = 1;
            file->گنتی = 0;
            ngx_close_cached_file(cache, file, 0, ngx_چکر->لوگارتھم);

        } وإلا {
            ngx_free(file->name);
            ngx_free(file);
        }
    }

    إذا (cache->current) {
        ngx_log_error(NGX_LOG_ALERT, ngx_چکر->لوگارتھم, 0,
                      "%ui items still left in open file cache",
                      cache->current);
    }

    إذا (cache->rbtree.جڑ != cache->rbtree.sentinel) {
        ngx_log_error(NGX_LOG_ALERT, ngx_چکر->لوگارتھم, 0,
                      "rbtree still is not empty in open file cache");

    }
}


ngx_int_t
ngx_open_cached_file(ngx_open_file_cache_t *cache, ngx_str_t *name,
    ngx_open_file_info_t *دا, ngx_pool_t *pool)
{
    ویلا_t                          now;
    uint32_t                        hash;
    ngx_int_t                       rc;
    ngx_file_info_t                 fi;
    ngx_pool_cleanup_t             *cln;
    ngx_cached_open_file_t         *file;
    ngx_pool_cleanup_file_t        *clnf;
    ngx_open_file_cache_cleanup_t  *ofcln;

    دا->fd = NGX_INVALID_FILE;
    دا->err = 0;

    إذا (cache == عدم) {

        إذا (دا->test_only) {

            إذا (ngx_file_info_wrapper(name, دا, &fi, pool->لوگارتھم)
                == NGX_FILE_ERROR)
            {
                أرجع NGX_ERROR;
            }

            دا->uniq = ngx_file_uniq(&fi);
            دا->mtime = ngx_file_mtime(&fi);
            دا->ناپ = ngx_file_ناپ(&fi);
            دا->fs_ناپ = ngx_file_fs_ناپ(&fi);
            دا->is_dir = ngx_is_dir(&fi);
            دا->is_file = ngx_is_file(&fi);
            دا->is_link = ngx_is_link(&fi);
            دا->is_exec = ngx_is_exec(&fi);

            أرجع NGX_OK;
        }

        cln = ngx_pool_cleanup_جوڑو(pool, حجم(ngx_pool_cleanup_file_t));
        إذا (cln == عدم) {
            أرجع NGX_ERROR;
        }

        rc = ngx_open_and_stat_file(name, دا, pool->لوگارتھم);

        إذا (rc == NGX_OK && !دا->is_dir) {
            cln->handler = ngx_pool_cleanup_file;
            clnf = cln->انکڑے;

            clnf->fd = دا->fd;
            clnf->name = name->انکڑے;
            clnf->لوگارتھم = pool->لوگارتھم;
        }

        أرجع rc;
    }

    cln = ngx_pool_cleanup_جوڑو(pool, حجم(ngx_open_file_cache_cleanup_t));
    إذا (cln == عدم) {
        أرجع NGX_ERROR;
    }

    now = ngx_ویلا();

    hash = ngx_چکری_جانچ_32_long(name->انکڑے, name->len);

    file = ngx_open_file_lookup(cache, name, hash);

    إذا (file) {

        file->ورتدا++;

        ngx_قطار_remove(&file->قطار);

        إذا (file->fd == NGX_INVALID_FILE && file->err == 0 && !file->is_dir) {

            /* file was not used often enough to keep open */

            rc = ngx_open_and_stat_file(name, دا, pool->لوگارتھم);

            إذا (rc != NGX_OK && (دا->err == 0 || !دا->errors)) {
                اذهب failed;
            }

            اذهب جوڑو_event;
        }

        إذا (file->use_event
            || (file->event == عدم
                && (دا->uniq == 0 || دا->uniq == file->uniq)
                && now - file->created < دا->valid
#إذا (NGX_HAVE_OPENAT)
                && دا->disable_symlinks == file->disable_symlinks
                && دا->disable_symlinks_from == file->disable_symlinks_from
#جے_انت
            ))
        {
            إذا (file->err == 0) {

                دا->fd = file->fd;
                دا->uniq = file->uniq;
                دا->mtime = file->mtime;
                دا->ناپ = file->ناپ;

                دا->is_dir = file->is_dir;
                دا->is_file = file->is_file;
                دا->is_link = file->is_link;
                دا->is_exec = file->is_exec;
                دا->is_directio = file->is_directio;

                إذا (!file->is_dir) {
                    file->گنتی++;
                    ngx_open_file_جوڑو_event(cache, file, دا, pool->لوگارتھم);
                }

            } وإلا {
                دا->err = file->err;
#إذا (NGX_HAVE_OPENAT)
                دا->failed = file->disable_symlinks ? ngx_openat_file_n
                                                    : ngx_open_file_n;
#وإلا
                دا->failed = ngx_open_file_n;
#جے_انت
            }

            اذهب لبھیا;
        }

        ngx_log_debug4(NGX_LOG_DEBUG_CORE, pool->لوگارتھم, 0,
                       "retest open file: %s, fd:%d, c:%d, e:%d",
                       file->name, file->fd, file->گنتی, file->err);

        إذا (file->is_dir) {

            /*
             * chances that directory became file are very small
             * so test_dir flag allows to use a single syscall
             * in ngx_file_info() instead of three syscalls
             */

            دا->test_dir = 1;
        }

        دا->fd = file->fd;
        دا->uniq = file->uniq;

        rc = ngx_open_and_stat_file(name, دا, pool->لوگارتھم);

        إذا (rc != NGX_OK && (دا->err == 0 || !دا->errors)) {
            اذهب failed;
        }

        إذا (دا->is_dir) {

            إذا (file->is_dir || file->err) {
                اذهب update;
            }

            /* file became directory */

        } وإلا إذا (دا->err == 0) {  /* file */

            إذا (file->is_dir || file->err) {
                اذهب جوڑو_event;
            }

            إذا (دا->uniq == file->uniq) {

                إذا (file->event) {
                    file->use_event = 1;
                }

                دا->is_directio = file->is_directio;

                اذهب update;
            }

            /* file was changed */

        } وإلا { /* error to cache */

            إذا (file->err || file->is_dir) {
                اذهب update;
            }

            /* file was removed, etc. */
        }

        إذا (file->گنتی == 0) {

            ngx_open_file_del_event(file);

            إذا (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, pool->لوگارتھم, ngx_errno,
                              ngx_close_file_n " \"%V\" failed", name);
            }

            اذهب جوڑو_event;
        }

        ngx_rbtree_delete(&cache->rbtree, &file->گرہ);

        cache->current--;

        file->close = 1;

        اذهب create;
    }

    /* not found */

    rc = ngx_open_and_stat_file(name, دا, pool->لوگارتھم);

    إذا (rc != NGX_OK && (دا->err == 0 || !دا->errors)) {
        اذهب failed;
    }

create:

    إذا (cache->current >= cache->max) {
        ngx_expire_old_cached_files(cache, 0, pool->لوگارتھم);
    }

    file = ngx_alloc(حجم(ngx_cached_open_file_t), pool->لوگارتھم);

    إذا (file == عدم) {
        اذهب failed;
    }

    file->name = ngx_alloc(name->len + 1, pool->لوگارتھم);

    إذا (file->name == عدم) {
        ngx_free(file);
        file = عدم;
        اذهب failed;
    }

    ngx_cpystrn(file->name, name->انکڑے, name->len + 1);

    file->گرہ.کلید = hash;

    ngx_rbtree_داخل_کرو(&cache->rbtree, &file->گرہ);

    cache->current++;

    file->ورتدا = 1;
    file->گنتی = 0;
    file->use_event = 0;
    file->event = عدم;

جوڑو_event:

    ngx_open_file_جوڑو_event(cache, file, دا, pool->لوگارتھم);

update:

    file->fd = دا->fd;
    file->err = دا->err;
#إذا (NGX_HAVE_OPENAT)
    file->disable_symlinks = دا->disable_symlinks;
    file->disable_symlinks_from = دا->disable_symlinks_from;
#جے_انت

    إذا (دا->err == 0) {
        file->uniq = دا->uniq;
        file->mtime = دا->mtime;
        file->ناپ = دا->ناپ;

        file->close = 0;

        file->is_dir = دا->is_dir;
        file->is_file = دا->is_file;
        file->is_link = دا->is_link;
        file->is_exec = دا->is_exec;
        file->is_directio = دا->is_directio;

        إذا (!دا->is_dir) {
            file->گنتی++;
        }
    }

    file->created = now;

لبھیا:

    file->accessed = now;

    ngx_قطار_داخل_کرو_سر(&cache->expire_قطار, &file->قطار);

    ngx_log_debug5(NGX_LOG_DEBUG_CORE, pool->لوگارتھم, 0,
                   "cached open file: %s, fd:%d, c:%d, e:%d, u:%d",
                   file->name, file->fd, file->گنتی, file->err, file->ورتدا);

    إذا (دا->err == 0) {

        إذا (!دا->is_dir) {
            cln->handler = ngx_open_file_cleanup;
            ofcln = cln->انکڑے;

            ofcln->cache = cache;
            ofcln->file = file;
            ofcln->min_ورتدا = دا->min_ورتدا;
            ofcln->لوگارتھم = pool->لوگارتھم;
        }

        أرجع NGX_OK;
    }

    أرجع NGX_ERROR;

failed:

    إذا (file) {
        ngx_rbtree_delete(&cache->rbtree, &file->گرہ);

        cache->current--;

        إذا (file->گنتی == 0) {

            إذا (file->fd != NGX_INVALID_FILE) {
                إذا (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_ALERT, pool->لوگارتھم, ngx_errno,
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

    إذا (دا->fd != NGX_INVALID_FILE) {
        إذا (ngx_close_file(دا->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, pool->لوگارتھم, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }
    }

    أرجع NGX_ERROR;
}


#إذا (NGX_HAVE_OPENAT)

ساكن ngx_fd_t
ngx_openat_file_owner(ngx_fd_t at_fd, ثابت u_char *name,
    ngx_int_t منوال, ngx_int_t create, ngx_int_t access, ngx_log_t *لوگارتھم)
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

    fd = ngx_openat_file(at_fd, name, منوال, create, access);

    إذا (fd == NGX_INVALID_FILE) {
        أرجع NGX_INVALID_FILE;
    }

    إذا (ngx_file_at_info(at_fd, name, &atfi, AT_SYMLINK_NOFOLLOW)
        == NGX_FILE_ERROR)
    {
        err = ngx_errno;
        اذهب failed;
    }

#إذا (NGX_HAVE_O_رستہ)
    إذا (ngx_file_o_رستہ_info(fd, &fi, لوگارتھم) == NGX_ERROR) {
        err = ngx_errno;
        اذهب failed;
    }
#وإلا
    إذا (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        err = ngx_errno;
        اذهب failed;
    }
#جے_انت

    إذا (fi.st_uid != atfi.st_uid) {
        err = NGX_ELOOP;
        اذهب failed;
    }

    أرجع fd;

failed:

    إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, لوگارتھم, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", name);
    }

    ngx_سیٹ_errno(err);

    أرجع NGX_INVALID_FILE;
}


#إذا (NGX_HAVE_O_رستہ)

ساكن ngx_int_t
ngx_file_o_رستہ_info(ngx_fd_t fd, ngx_file_info_t *fi, ngx_log_t *لوگارتھم)
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

        ngx_log_error(NGX_LOG_NOTICE, لوگارتھم, 0,
                      "fstat(O_PATH) failed with EBADF, "
                      "switching to fstatat(AT_EMPTY_PATH)");

        use_fstat = 0;
    }

    إذا (ngx_file_at_info(fd, "", fi, AT_خالی_تھاواں_رستہ) != NGX_FILE_ERROR) {
        أرجع NGX_OK;
    }

    أرجع NGX_ERROR;
}

#جے_انت

#جے_انت /* NGX_HAVE_OPENAT */


ساكن ngx_fd_t
ngx_open_file_wrapper(ngx_str_t *name, ngx_open_file_info_t *دا,
    ngx_int_t منوال, ngx_int_t create, ngx_int_t access, ngx_log_t *لوگارتھم)
{
    ngx_fd_t  fd;

#إذا !(NGX_HAVE_OPENAT)

    fd = ngx_open_file(name->انکڑے, منوال, create, access);

    إذا (fd == NGX_INVALID_FILE) {
        دا->err = ngx_errno;
        دا->failed = ngx_open_file_n;
        أرجع NGX_INVALID_FILE;
    }

    أرجع fd;

#وإلا

    u_char           *p, *cp, *سرا;
    ngx_fd_t          at_fd;
    ngx_str_t         at_name;

    إذا (دا->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {
        fd = ngx_open_file(name->انکڑے, منوال, create, access);

        إذا (fd == NGX_INVALID_FILE) {
            دا->err = ngx_errno;
            دا->failed = ngx_open_file_n;
            أرجع NGX_INVALID_FILE;
        }

        أرجع fd;
    }

    p = name->انکڑے;
    سرا = p + name->len;

    at_name = *name;

    إذا (دا->disable_symlinks_from) {

        cp = p + دا->disable_symlinks_from;

        *cp = '\0';

        at_fd = ngx_open_file(p, NGX_FILE_تلاش|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        *cp = '/';

        إذا (at_fd == NGX_INVALID_FILE) {
            دا->err = ngx_errno;
            دا->failed = ngx_open_file_n;
            أرجع NGX_INVALID_FILE;
        }

        at_name.len = دا->disable_symlinks_from;
        p = cp + 1;

    } وإلا إذا (*p == '/') {

        at_fd = ngx_open_file("/",
                              NGX_FILE_تلاش|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        إذا (at_fd == NGX_INVALID_FILE) {
            دا->err = ngx_errno;
            دا->failed = ngx_openat_file_n;
            أرجع NGX_INVALID_FILE;
        }

        at_name.len = 1;
        p++;

    } وإلا {
        at_fd = NGX_AT_FDCWD;
    }

    لكل ( ;; ) {
        cp = ngx_strlchr(p, سرا, '/');
        إذا (cp == عدم) {
            اكسر;
        }

        إذا (cp == p) {
            p++;
            استمر;
        }

        *cp = '\0';

        إذا (دا->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER) {
            fd = ngx_openat_file_owner(at_fd, p,
                                       NGX_FILE_تلاش|NGX_FILE_NONBLOCK,
                                       NGX_FILE_OPEN, 0, لوگارتھم);

        } وإلا {
            fd = ngx_openat_file(at_fd, p,
                           NGX_FILE_تلاش|NGX_FILE_NONBLOCK|NGX_FILE_NOFOLLOW,
                           NGX_FILE_OPEN, 0);
        }

        *cp = '/';

        إذا (fd == NGX_INVALID_FILE) {
            دا->err = ngx_errno;
            دا->failed = ngx_openat_file_n;
            اذهب failed;
        }

        إذا (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, لوگارتھم, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", &at_name);
        }

        p = cp + 1;
        at_fd = fd;
        at_name.len = cp - at_name.انکڑے;
    }

    إذا (p == سرا) {

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

        fd = ngx_openat_file(at_fd, ".", منوال, create, access);
        اذهب پورا;
    }

    إذا (دا->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER
        && !(create & (NGX_FILE_CREATE_OR_OPEN|NGX_FILE_TRUNCATE)))
    {
        fd = ngx_openat_file_owner(at_fd, p, منوال, create, access, لوگارتھم);

    } وإلا {
        fd = ngx_openat_file(at_fd, p, منوال|NGX_FILE_NOFOLLOW, create, access);
    }

پورا:

    إذا (fd == NGX_INVALID_FILE) {
        دا->err = ngx_errno;
        دا->failed = ngx_openat_file_n;
    }

failed:

    إذا (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, لوگارتھم, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", &at_name);
    }

    أرجع fd;
#جے_انت
}


ساكن ngx_int_t
ngx_file_info_wrapper(ngx_str_t *name, ngx_open_file_info_t *دا,
    ngx_file_info_t *fi, ngx_log_t *لوگارتھم)
{
    ngx_int_t  rc;

#إذا !(NGX_HAVE_OPENAT)

    rc = ngx_file_info(name->انکڑے, fi);

    إذا (rc == NGX_FILE_ERROR) {
        دا->err = ngx_errno;
        دا->failed = ngx_file_info_n;
        أرجع NGX_FILE_ERROR;
    }

    أرجع rc;

#وإلا

    ngx_fd_t  fd;

    إذا (دا->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {

        rc = ngx_file_info(name->انکڑے, fi);

        إذا (rc == NGX_FILE_ERROR) {
            دا->err = ngx_errno;
            دا->failed = ngx_file_info_n;
            أرجع NGX_FILE_ERROR;
        }

        أرجع rc;
    }

    fd = ngx_open_file_wrapper(name, دا, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,
                               NGX_FILE_OPEN, 0, لوگارتھم);

    إذا (fd == NGX_INVALID_FILE) {
        أرجع NGX_FILE_ERROR;
    }

    rc = ngx_fd_info(fd, fi);

    إذا (rc == NGX_FILE_ERROR) {
        دا->err = ngx_errno;
        دا->failed = ngx_fd_info_n;
    }

    إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, لوگارتھم, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", name);
    }

    أرجع rc;
#جے_انت
}


ساكن ngx_int_t
ngx_open_and_stat_file(ngx_str_t *name, ngx_open_file_info_t *دا,
    ngx_log_t *لوگارتھم)
{
    ngx_fd_t         fd;
    ngx_file_info_t  fi;

    إذا (دا->fd != NGX_INVALID_FILE) {

        إذا (ngx_file_info_wrapper(name, دا, &fi, لوگارتھم) == NGX_FILE_ERROR) {
            دا->fd = NGX_INVALID_FILE;
            أرجع NGX_ERROR;
        }

        إذا (دا->uniq == ngx_file_uniq(&fi)) {
            اذهب پورا;
        }

    } وإلا إذا (دا->test_dir) {

        إذا (ngx_file_info_wrapper(name, دا, &fi, لوگارتھم) == NGX_FILE_ERROR) {
            دا->fd = NGX_INVALID_FILE;
            أرجع NGX_ERROR;
        }

        إذا (ngx_is_dir(&fi)) {
            اذهب پورا;
        }
    }

    إذا (!دا->لوگارتھم) {

        /*
         * Use non-blocking open() not to hang on FIFO files, etc.
         * This flag has no effect on a regular files.
         */

        fd = ngx_open_file_wrapper(name, دا, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,
                                   NGX_FILE_OPEN, 0, لوگارتھم);

    } وإلا {
        fd = ngx_open_file_wrapper(name, دا, NGX_FILE_APPEND,
                                   NGX_FILE_CREATE_OR_OPEN,
                                   NGX_FILE_DEFAULT_ACCESS, لوگارتھم);
    }

    إذا (fd == NGX_INVALID_FILE) {
        دا->fd = NGX_INVALID_FILE;
        أرجع NGX_ERROR;
    }

    إذا (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, لوگارتھم, ngx_errno,
                      ngx_fd_info_n " \"%V\" failed", name);

        إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, لوگارتھم, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        دا->fd = NGX_INVALID_FILE;

        أرجع NGX_ERROR;
    }

    إذا (ngx_is_dir(&fi)) {
        إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, لوگارتھم, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        دا->fd = NGX_INVALID_FILE;

    } وإلا {
        دا->fd = fd;

        إذا (دا->read_ahead && ngx_file_ناپ(&fi) > NGX_MIN_READ_AHEAD) {
            إذا (ngx_read_ahead(fd, دا->read_ahead) == NGX_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, لوگارتھم, ngx_errno,
                              ngx_read_ahead_n " \"%V\" failed", name);
            }
        }

        إذا (دا->directio <= ngx_file_ناپ(&fi)) {
            إذا (ngx_directio_on(fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, لوگارتھم, ngx_errno,
                              ngx_directio_on_n " \"%V\" failed", name);

            } وإلا {
                دا->is_directio = 1;
            }
        }
    }

پورا:

    دا->uniq = ngx_file_uniq(&fi);
    دا->mtime = ngx_file_mtime(&fi);
    دا->ناپ = ngx_file_ناپ(&fi);
    دا->fs_ناپ = ngx_file_fs_ناپ(&fi);
    دا->is_dir = ngx_is_dir(&fi);
    دا->is_file = ngx_is_file(&fi);
    دا->is_link = ngx_is_link(&fi);
    دا->is_exec = ngx_is_exec(&fi);

    أرجع NGX_OK;
}


/*
 * we ignore any possible event setting error and
 * fallback to usual periodic file retests
 */

ساكن فراغ
ngx_open_file_جوڑو_event(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_open_file_info_t *دا, ngx_log_t *لوگارتھم)
{
    ngx_open_file_cache_event_t  *fev;

    إذا (!(ngx_event_flags & NGX_USE_VNODE_EVENT)
        || !دا->events
        || file->event
        || دا->fd == NGX_INVALID_FILE
        || file->ورتدا < دا->min_ورتدا)
    {
        أرجع;
    }

    file->use_event = 0;

    file->event = ngx_calloc(حجم(ngx_event_t), لوگارتھم);
    إذا (file->event== عدم) {
        أرجع;
    }

    fev = ngx_alloc(حجم(ngx_open_file_cache_event_t), لوگارتھم);
    إذا (fev == عدم) {
        ngx_free(file->event);
        file->event = عدم;
        أرجع;
    }

    fev->fd = دا->fd;
    fev->file = file;
    fev->cache = cache;

    file->event->handler = ngx_open_file_cache_remove;
    file->event->انکڑے = fev;

    /*
     * although vnode event may be called while ngx_cycle->poll
     * destruction, however, cleanup procedures are run before any
     * memory freeing and events will be canceled.
     */

    file->event->لوگارتھم = ngx_چکر->لوگارتھم;

    إذا (ngx_جوڑو_event(file->event, NGX_VNODE_EVENT, NGX_ONESHOT_EVENT)
        != NGX_OK)
    {
        ngx_free(file->event->انکڑے);
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
ngx_open_file_cleanup(فراغ *انکڑے)
{
    ngx_open_file_cache_cleanup_t  *c = انکڑے;

    c->file->گنتی--;

    ngx_close_cached_file(c->cache, c->file, c->min_ورتدا, c->لوگارتھم);

    /* drop one or two expired open files */
    ngx_expire_old_cached_files(c->cache, 1, c->لوگارتھم);
}


ساكن فراغ
ngx_close_cached_file(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_uint_t min_ورتدا, ngx_log_t *لوگارتھم)
{
    ngx_log_debug5(NGX_LOG_DEBUG_CORE, لوگارتھم, 0,
                   "close cached open file: %s, fd:%d, c:%d, u:%d, %d",
                   file->name, file->fd, file->گنتی, file->ورتدا, file->close);

    إذا (!file->close) {

        file->accessed = ngx_ویلا();

        ngx_قطار_remove(&file->قطار);

        ngx_قطار_داخل_کرو_سر(&cache->expire_قطار, &file->قطار);

        إذا (file->ورتدا >= min_ورتدا || file->گنتی) {
            أرجع;
        }
    }

    ngx_open_file_del_event(file);

    إذا (file->گنتی) {
        أرجع;
    }

    إذا (file->fd != NGX_INVALID_FILE) {

        إذا (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, لوگارتھم, ngx_errno,
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
                         file->گنتی ? NGX_FLUSH_EVENT : NGX_CLOSE_EVENT);

    ngx_free(file->event->انکڑے);
    ngx_free(file->event);
    file->event = عدم;
    file->use_event = 0;
}


ساكن فراغ
ngx_expire_old_cached_files(ngx_open_file_cache_t *cache, ngx_uint_t n,
    ngx_log_t *لوگارتھم)
{
    ویلا_t                   now;
    ngx_قطار_t             *q;
    ngx_cached_open_file_t  *file;

    now = ngx_ویلا();

    /*
     * n == 1 deletes one or two inactive files
     * n == 0 deletes least recently used file by force
     *        and one or two inactive files
     */

    طالما (n < 3) {

        إذا (ngx_قطار_خالی_تھاواں(&cache->expire_قطار)) {
            أرجع;
        }

        q = ngx_قطار_آخری(&cache->expire_قطار);

        file = ngx_قطار_انکڑے(q, ngx_cached_open_file_t, قطار);

        إذا (n++ != 0 && now - file->accessed <= cache->inactive) {
            أرجع;
        }

        ngx_قطار_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->گرہ);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, لوگارتھم, 0,
                       "expire cached open file: %s", file->name);

        إذا (!file->err && !file->is_dir) {
            file->close = 1;
            ngx_close_cached_file(cache, file, 0, لوگارتھم);

        } وإلا {
            ngx_free(file->name);
            ngx_free(file);
        }
    }
}


ساكن فراغ
ngx_open_file_cache_rbtree_داخل_کرو_قدر(ngx_rbtree_گرہ_t *عارضی,
    ngx_rbtree_گرہ_t *گرہ, ngx_rbtree_گرہ_t *sentinel)
{
    ngx_rbtree_گرہ_t       **p;
    ngx_cached_open_file_t    *file, *file_عارضی;

    لكل ( ;; ) {

        إذا (گرہ->کلید < عارضی->کلید) {

            p = &عارضی->بایاں;

        } وإلا إذا (گرہ->کلید > عارضی->کلید) {

            p = &عارضی->دایاں;

        } وإلا { /* node->key == temp->key */

            file = (ngx_cached_open_file_t *) گرہ;
            file_عارضی = (ngx_cached_open_file_t *) عارضی;

            p = (ngx_strcmp(file->name, file_عارضی->name) < 0)
                    ? &عارضی->بایاں : &عارضی->دایاں;
        }

        إذا (*p == sentinel) {
            اكسر;
        }

        عارضی = *p;
    }

    *p = گرہ;
    گرہ->ماپا = عارضی;
    گرہ->بایاں = sentinel;
    گرہ->دایاں = sentinel;
    ngx_rbt_red(گرہ);
}


ساكن ngx_cached_open_file_t *
ngx_open_file_lookup(ngx_open_file_cache_t *cache, ngx_str_t *name,
    uint32_t hash)
{
    ngx_int_t                rc;
    ngx_rbtree_گرہ_t       *گرہ, *sentinel;
    ngx_cached_open_file_t  *file;

    گرہ = cache->rbtree.جڑ;
    sentinel = cache->rbtree.sentinel;

    طالما (گرہ != sentinel) {

        إذا (hash < گرہ->کلید) {
            گرہ = گرہ->بایاں;
            استمر;
        }

        إذا (hash > گرہ->کلید) {
            گرہ = گرہ->دایاں;
            استمر;
        }

        /* hash == node->key */

        file = (ngx_cached_open_file_t *) گرہ;

        rc = ngx_strcmp(name->انکڑے, file->name);

        إذا (rc == 0) {
            أرجع file;
        }

        گرہ = (rc < 0) ? گرہ->بایاں : گرہ->دایاں;
    }

    أرجع عدم;
}


ساكن فراغ
ngx_open_file_cache_remove(ngx_event_t *ev)
{
    ngx_cached_open_file_t       *file;
    ngx_open_file_cache_event_t  *fev;

    fev = ev->انکڑے;
    file = fev->file;

    ngx_قطار_remove(&file->قطار);

    ngx_rbtree_delete(&fev->cache->rbtree, &file->گرہ);

    fev->cache->current--;

    /* NGX_ONESHOT_EVENT was already deleted */
    file->event = عدم;
    file->use_event = 0;

    file->close = 1;

    ngx_close_cached_file(fev->cache, file, 0, ev->لوگارتھم);

    /* free memory only when fev->cache and fev->file are already not needed */

    ngx_free(ev->انکڑے);
    ngx_free(ev);
}
