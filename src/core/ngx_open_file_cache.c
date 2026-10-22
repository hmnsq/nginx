
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>
#شامل <ngx_event.h>


/*
 * open file cache caches
 *    open file handles with stat() info;
 *    directories stat() info;
 *    files and directories errors: not found, access denied, etc.
 */


#تعریف_کرو NGX_MIN_READ_AHEAD  (128 * 1024)


ساکن خالی ngx_open_file_cache_cleanup(خالی *ڈیٹا);
#اگر (NGX_HAVE_OPENAT)
ساکن ngx_fd_t ngx_openat_file_owner(ngx_fd_t at_fd, ثابت u_char *name,
    ngx_int_t منوال, ngx_int_t create, ngx_int_t access, ngx_log_t *لوگارتھم);
#اگر (NGX_HAVE_O_راستہ)
ساکن ngx_int_t ngx_file_o_راستہ_info(ngx_fd_t fd, ngx_file_info_t *fi,
    ngx_log_t *لوگارتھم);
#اگر_ختم
#اگر_ختم
ساکن ngx_fd_t ngx_open_file_wrapper(ngx_str_t *name,
    ngx_open_file_info_t *کا, ngx_int_t منوال, ngx_int_t create,
    ngx_int_t access, ngx_log_t *لوگارتھم);
ساکن ngx_int_t ngx_file_info_wrapper(ngx_str_t *name,
    ngx_open_file_info_t *کا, ngx_file_info_t *fi, ngx_log_t *لوگارتھم);
ساکن ngx_int_t ngx_open_and_stat_file(ngx_str_t *name,
    ngx_open_file_info_t *کا, ngx_log_t *لوگارتھم);
ساکن خالی ngx_open_file_جوڑو_event(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_open_file_info_t *کا, ngx_log_t *لوگارتھم);
ساکن خالی ngx_open_file_cleanup(خالی *ڈیٹا);
ساکن خالی ngx_close_cached_file(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_uint_t min_استعمال_کرتا, ngx_log_t *لوگارتھم);
ساکن خالی ngx_open_file_del_event(ngx_cached_open_file_t *file);
ساکن خالی ngx_expire_old_cached_files(ngx_open_file_cache_t *cache,
    ngx_uint_t n, ngx_log_t *لوگارتھم);
ساکن خالی ngx_open_file_cache_rbtree_داخل_کرو_قدر(ngx_rbtree_گرہ_t *عارضی,
    ngx_rbtree_گرہ_t *گرہ, ngx_rbtree_گرہ_t *sentinel);
ساکن ngx_cached_open_file_t *
    ngx_open_file_lookup(ngx_open_file_cache_t *cache, ngx_str_t *name,
    uint32_t hash);
ساکن خالی ngx_open_file_cache_remove(ngx_event_t *ev);


ngx_open_file_cache_t *
ngx_open_file_cache_init(ngx_pool_t *pool, ngx_uint_t max, وقت_t inactive)
{
    ngx_pool_cleanup_t     *cln;
    ngx_open_file_cache_t  *cache;

    cache = ngx_palloc(pool, حجم(ngx_open_file_cache_t));
    اگر (cache == تہی) {
        واپس تہی;
    }

    ngx_rbtree_init(&cache->rbtree, &cache->sentinel,
                    ngx_open_file_cache_rbtree_داخل_کرو_قدر);

    ngx_قطار_init(&cache->expire_قطار);

    cache->current = 0;
    cache->max = max;
    cache->inactive = inactive;

    cln = ngx_pool_cleanup_جوڑو(pool, 0);
    اگر (cln == تہی) {
        واپس تہی;
    }

    cln->handler = ngx_open_file_cache_cleanup;
    cln->ڈیٹا = cache;

    واپس cache;
}


ساکن خالی
ngx_open_file_cache_cleanup(خالی *ڈیٹا)
{
    ngx_open_file_cache_t  *cache = ڈیٹا;

    ngx_قطار_t             *q;
    ngx_cached_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_چکر->لوگارتھم, 0,
                   "open file cache cleanup");

    برائے ( ;; ) {

        اگر (ngx_قطار_خالی_خانے(&cache->expire_قطار)) {
            توڑو;
        }

        q = ngx_قطار_آخری(&cache->expire_قطار);

        file = ngx_قطار_ڈیٹا(q, ngx_cached_open_file_t, قطار);

        ngx_قطار_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->گرہ);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_چکر->لوگارتھم, 0,
                       "delete cached open file: %s", file->name);

        اگر (!file->err && !file->is_dir) {
            file->close = 1;
            file->گنتی = 0;
            ngx_close_cached_file(cache, file, 0, ngx_چکر->لوگارتھم);

        } ورنہ {
            ngx_free(file->name);
            ngx_free(file);
        }
    }

    اگر (cache->current) {
        ngx_log_error(NGX_LOG_ALERT, ngx_چکر->لوگارتھم, 0,
                      "%ui items still left in open file cache",
                      cache->current);
    }

    اگر (cache->rbtree.جڑ != cache->rbtree.sentinel) {
        ngx_log_error(NGX_LOG_ALERT, ngx_چکر->لوگارتھم, 0,
                      "rbtree still is not empty in open file cache");

    }
}


ngx_int_t
ngx_open_cached_file(ngx_open_file_cache_t *cache, ngx_str_t *name,
    ngx_open_file_info_t *کا, ngx_pool_t *pool)
{
    وقت_t                          now;
    uint32_t                        hash;
    ngx_int_t                       rc;
    ngx_file_info_t                 fi;
    ngx_pool_cleanup_t             *cln;
    ngx_cached_open_file_t         *file;
    ngx_pool_cleanup_file_t        *clnf;
    ngx_open_file_cache_cleanup_t  *ofcln;

    کا->fd = NGX_INVALID_FILE;
    کا->err = 0;

    اگر (cache == تہی) {

        اگر (کا->test_only) {

            اگر (ngx_file_info_wrapper(name, کا, &fi, pool->لوگارتھم)
                == NGX_FILE_ERROR)
            {
                واپس NGX_ERROR;
            }

            کا->uniq = ngx_file_uniq(&fi);
            کا->mtime = ngx_file_mtime(&fi);
            کا->سائز = ngx_file_سائز(&fi);
            کا->fs_سائز = ngx_file_fs_سائز(&fi);
            کا->is_dir = ngx_is_dir(&fi);
            کا->is_file = ngx_is_file(&fi);
            کا->is_link = ngx_is_link(&fi);
            کا->is_exec = ngx_is_exec(&fi);

            واپس NGX_OK;
        }

        cln = ngx_pool_cleanup_جوڑو(pool, حجم(ngx_pool_cleanup_file_t));
        اگر (cln == تہی) {
            واپس NGX_ERROR;
        }

        rc = ngx_open_and_stat_file(name, کا, pool->لوگارتھم);

        اگر (rc == NGX_OK && !کا->is_dir) {
            cln->handler = ngx_pool_cleanup_file;
            clnf = cln->ڈیٹا;

            clnf->fd = کا->fd;
            clnf->name = name->ڈیٹا;
            clnf->لوگارتھم = pool->لوگارتھم;
        }

        واپس rc;
    }

    cln = ngx_pool_cleanup_جوڑو(pool, حجم(ngx_open_file_cache_cleanup_t));
    اگر (cln == تہی) {
        واپس NGX_ERROR;
    }

    now = ngx_وقت();

    hash = ngx_چکری_جانچ_32_long(name->ڈیٹا, name->len);

    file = ngx_open_file_lookup(cache, name, hash);

    اگر (file) {

        file->استعمال_کرتا++;

        ngx_قطار_remove(&file->قطار);

        اگر (file->fd == NGX_INVALID_FILE && file->err == 0 && !file->is_dir) {

            /* file was not used often enough to keep open */

            rc = ngx_open_and_stat_file(name, کا, pool->لوگارتھم);

            اگر (rc != NGX_OK && (کا->err == 0 || !کا->errors)) {
                جاؤ failed;
            }

            جاؤ جوڑو_event;
        }

        اگر (file->use_event
            || (file->event == تہی
                && (کا->uniq == 0 || کا->uniq == file->uniq)
                && now - file->created < کا->valid
#اگر (NGX_HAVE_OPENAT)
                && کا->disable_symlinks == file->disable_symlinks
                && کا->disable_symlinks_from == file->disable_symlinks_from
#اگر_ختم
            ))
        {
            اگر (file->err == 0) {

                کا->fd = file->fd;
                کا->uniq = file->uniq;
                کا->mtime = file->mtime;
                کا->سائز = file->سائز;

                کا->is_dir = file->is_dir;
                کا->is_file = file->is_file;
                کا->is_link = file->is_link;
                کا->is_exec = file->is_exec;
                کا->is_directio = file->is_directio;

                اگر (!file->is_dir) {
                    file->گنتی++;
                    ngx_open_file_جوڑو_event(cache, file, کا, pool->لوگارتھم);
                }

            } ورنہ {
                کا->err = file->err;
#اگر (NGX_HAVE_OPENAT)
                کا->failed = file->disable_symlinks ? ngx_openat_file_n
                                                    : ngx_open_file_n;
#ورنہ
                کا->failed = ngx_open_file_n;
#اگر_ختم
            }

            جاؤ ملا;
        }

        ngx_log_debug4(NGX_LOG_DEBUG_CORE, pool->لوگارتھم, 0,
                       "retest open file: %s, fd:%d, c:%d, e:%d",
                       file->name, file->fd, file->گنتی, file->err);

        اگر (file->is_dir) {

            /*
             * chances that directory became file are very small
             * so test_dir flag allows to use a single syscall
             * in ngx_file_info() instead of three syscalls
             */

            کا->test_dir = 1;
        }

        کا->fd = file->fd;
        کا->uniq = file->uniq;

        rc = ngx_open_and_stat_file(name, کا, pool->لوگارتھم);

        اگر (rc != NGX_OK && (کا->err == 0 || !کا->errors)) {
            جاؤ failed;
        }

        اگر (کا->is_dir) {

            اگر (file->is_dir || file->err) {
                جاؤ update;
            }

            /* file became directory */

        } ورنہ اگر (کا->err == 0) {  /* file */

            اگر (file->is_dir || file->err) {
                جاؤ جوڑو_event;
            }

            اگر (کا->uniq == file->uniq) {

                اگر (file->event) {
                    file->use_event = 1;
                }

                کا->is_directio = file->is_directio;

                جاؤ update;
            }

            /* file was changed */

        } ورنہ { /* error to cache */

            اگر (file->err || file->is_dir) {
                جاؤ update;
            }

            /* file was removed, etc. */
        }

        اگر (file->گنتی == 0) {

            ngx_open_file_del_event(file);

            اگر (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, pool->لوگارتھم, ngx_errno,
                              ngx_close_file_n " \"%V\" failed", name);
            }

            جاؤ جوڑو_event;
        }

        ngx_rbtree_delete(&cache->rbtree, &file->گرہ);

        cache->current--;

        file->close = 1;

        جاؤ create;
    }

    /* not found */

    rc = ngx_open_and_stat_file(name, کا, pool->لوگارتھم);

    اگر (rc != NGX_OK && (کا->err == 0 || !کا->errors)) {
        جاؤ failed;
    }

create:

    اگر (cache->current >= cache->max) {
        ngx_expire_old_cached_files(cache, 0, pool->لوگارتھم);
    }

    file = ngx_alloc(حجم(ngx_cached_open_file_t), pool->لوگارتھم);

    اگر (file == تہی) {
        جاؤ failed;
    }

    file->name = ngx_alloc(name->len + 1, pool->لوگارتھم);

    اگر (file->name == تہی) {
        ngx_free(file);
        file = تہی;
        جاؤ failed;
    }

    ngx_cpystrn(file->name, name->ڈیٹا, name->len + 1);

    file->گرہ.کلید = hash;

    ngx_rbtree_داخل_کرو(&cache->rbtree, &file->گرہ);

    cache->current++;

    file->استعمال_کرتا = 1;
    file->گنتی = 0;
    file->use_event = 0;
    file->event = تہی;

جوڑو_event:

    ngx_open_file_جوڑو_event(cache, file, کا, pool->لوگارتھم);

update:

    file->fd = کا->fd;
    file->err = کا->err;
#اگر (NGX_HAVE_OPENAT)
    file->disable_symlinks = کا->disable_symlinks;
    file->disable_symlinks_from = کا->disable_symlinks_from;
#اگر_ختم

    اگر (کا->err == 0) {
        file->uniq = کا->uniq;
        file->mtime = کا->mtime;
        file->سائز = کا->سائز;

        file->close = 0;

        file->is_dir = کا->is_dir;
        file->is_file = کا->is_file;
        file->is_link = کا->is_link;
        file->is_exec = کا->is_exec;
        file->is_directio = کا->is_directio;

        اگر (!کا->is_dir) {
            file->گنتی++;
        }
    }

    file->created = now;

ملا:

    file->accessed = now;

    ngx_قطار_داخل_کرو_سر(&cache->expire_قطار, &file->قطار);

    ngx_log_debug5(NGX_LOG_DEBUG_CORE, pool->لوگارتھم, 0,
                   "cached open file: %s, fd:%d, c:%d, e:%d, u:%d",
                   file->name, file->fd, file->گنتی, file->err, file->استعمال_کرتا);

    اگر (کا->err == 0) {

        اگر (!کا->is_dir) {
            cln->handler = ngx_open_file_cleanup;
            ofcln = cln->ڈیٹا;

            ofcln->cache = cache;
            ofcln->file = file;
            ofcln->min_استعمال_کرتا = کا->min_استعمال_کرتا;
            ofcln->لوگارتھم = pool->لوگارتھم;
        }

        واپس NGX_OK;
    }

    واپس NGX_ERROR;

failed:

    اگر (file) {
        ngx_rbtree_delete(&cache->rbtree, &file->گرہ);

        cache->current--;

        اگر (file->گنتی == 0) {

            اگر (file->fd != NGX_INVALID_FILE) {
                اگر (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_ALERT, pool->لوگارتھم, ngx_errno,
                                  ngx_close_file_n " \"%s\" failed",
                                  file->name);
                }
            }

            ngx_free(file->name);
            ngx_free(file);

        } ورنہ {
            file->close = 1;
        }
    }

    اگر (کا->fd != NGX_INVALID_FILE) {
        اگر (ngx_close_file(کا->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, pool->لوگارتھم, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }
    }

    واپس NGX_ERROR;
}


#اگر (NGX_HAVE_OPENAT)

ساکن ngx_fd_t
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

    اگر (fd == NGX_INVALID_FILE) {
        واپس NGX_INVALID_FILE;
    }

    اگر (ngx_file_at_info(at_fd, name, &atfi, AT_SYMLINK_NOFOLLOW)
        == NGX_FILE_ERROR)
    {
        err = ngx_errno;
        جاؤ failed;
    }

#اگر (NGX_HAVE_O_راستہ)
    اگر (ngx_file_o_راستہ_info(fd, &fi, لوگارتھم) == NGX_ERROR) {
        err = ngx_errno;
        جاؤ failed;
    }
#ورنہ
    اگر (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        err = ngx_errno;
        جاؤ failed;
    }
#اگر_ختم

    اگر (fi.st_uid != atfi.st_uid) {
        err = NGX_ELOOP;
        جاؤ failed;
    }

    واپس fd;

failed:

    اگر (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, لوگارتھم, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", name);
    }

    ngx_سیٹ_errno(err);

    واپس NGX_INVALID_FILE;
}


#اگر (NGX_HAVE_O_راستہ)

ساکن ngx_int_t
ngx_file_o_راستہ_info(ngx_fd_t fd, ngx_file_info_t *fi, ngx_log_t *لوگارتھم)
{
    ساکن ngx_uint_t  use_fstat = 1;

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

    اگر (use_fstat) {
        اگر (ngx_fd_info(fd, fi) != NGX_FILE_ERROR) {
            واپس NGX_OK;
        }

        اگر (ngx_errno != NGX_EBADF) {
            واپس NGX_ERROR;
        }

        ngx_log_error(NGX_LOG_NOTICE, لوگارتھم, 0,
                      "fstat(O_PATH) failed with EBADF, "
                      "switching to fstatat(AT_EMPTY_PATH)");

        use_fstat = 0;
    }

    اگر (ngx_file_at_info(fd, "", fi, AT_خالی_خانے_راستہ) != NGX_FILE_ERROR) {
        واپس NGX_OK;
    }

    واپس NGX_ERROR;
}

#اگر_ختم

#اگر_ختم /* NGX_HAVE_OPENAT */


ساکن ngx_fd_t
ngx_open_file_wrapper(ngx_str_t *name, ngx_open_file_info_t *کا,
    ngx_int_t منوال, ngx_int_t create, ngx_int_t access, ngx_log_t *لوگارتھم)
{
    ngx_fd_t  fd;

#اگر !(NGX_HAVE_OPENAT)

    fd = ngx_open_file(name->ڈیٹا, منوال, create, access);

    اگر (fd == NGX_INVALID_FILE) {
        کا->err = ngx_errno;
        کا->failed = ngx_open_file_n;
        واپس NGX_INVALID_FILE;
    }

    واپس fd;

#ورنہ

    u_char           *p, *cp, *انتہا;
    ngx_fd_t          at_fd;
    ngx_str_t         at_name;

    اگر (کا->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {
        fd = ngx_open_file(name->ڈیٹا, منوال, create, access);

        اگر (fd == NGX_INVALID_FILE) {
            کا->err = ngx_errno;
            کا->failed = ngx_open_file_n;
            واپس NGX_INVALID_FILE;
        }

        واپس fd;
    }

    p = name->ڈیٹا;
    انتہا = p + name->len;

    at_name = *name;

    اگر (کا->disable_symlinks_from) {

        cp = p + کا->disable_symlinks_from;

        *cp = '\0';

        at_fd = ngx_open_file(p, NGX_FILE_تلاش|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        *cp = '/';

        اگر (at_fd == NGX_INVALID_FILE) {
            کا->err = ngx_errno;
            کا->failed = ngx_open_file_n;
            واپس NGX_INVALID_FILE;
        }

        at_name.len = کا->disable_symlinks_from;
        p = cp + 1;

    } ورنہ اگر (*p == '/') {

        at_fd = ngx_open_file("/",
                              NGX_FILE_تلاش|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        اگر (at_fd == NGX_INVALID_FILE) {
            کا->err = ngx_errno;
            کا->failed = ngx_openat_file_n;
            واپس NGX_INVALID_FILE;
        }

        at_name.len = 1;
        p++;

    } ورنہ {
        at_fd = NGX_AT_FDCWD;
    }

    برائے ( ;; ) {
        cp = ngx_strlchr(p, انتہا, '/');
        اگر (cp == تہی) {
            توڑو;
        }

        اگر (cp == p) {
            p++;
            جاری;
        }

        *cp = '\0';

        اگر (کا->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER) {
            fd = ngx_openat_file_owner(at_fd, p,
                                       NGX_FILE_تلاش|NGX_FILE_NONBLOCK,
                                       NGX_FILE_OPEN, 0, لوگارتھم);

        } ورنہ {
            fd = ngx_openat_file(at_fd, p,
                           NGX_FILE_تلاش|NGX_FILE_NONBLOCK|NGX_FILE_NOFOLLOW,
                           NGX_FILE_OPEN, 0);
        }

        *cp = '/';

        اگر (fd == NGX_INVALID_FILE) {
            کا->err = ngx_errno;
            کا->failed = ngx_openat_file_n;
            جاؤ failed;
        }

        اگر (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, لوگارتھم, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", &at_name);
        }

        p = cp + 1;
        at_fd = fd;
        at_name.len = cp - at_name.ڈیٹا;
    }

    اگر (p == انتہا) {

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
        جاؤ مکمل;
    }

    اگر (کا->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER
        && !(create & (NGX_FILE_CREATE_OR_OPEN|NGX_FILE_TRUNCATE)))
    {
        fd = ngx_openat_file_owner(at_fd, p, منوال, create, access, لوگارتھم);

    } ورنہ {
        fd = ngx_openat_file(at_fd, p, منوال|NGX_FILE_NOFOLLOW, create, access);
    }

مکمل:

    اگر (fd == NGX_INVALID_FILE) {
        کا->err = ngx_errno;
        کا->failed = ngx_openat_file_n;
    }

failed:

    اگر (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, لوگارتھم, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", &at_name);
    }

    واپس fd;
#اگر_ختم
}


ساکن ngx_int_t
ngx_file_info_wrapper(ngx_str_t *name, ngx_open_file_info_t *کا,
    ngx_file_info_t *fi, ngx_log_t *لوگارتھم)
{
    ngx_int_t  rc;

#اگر !(NGX_HAVE_OPENAT)

    rc = ngx_file_info(name->ڈیٹا, fi);

    اگر (rc == NGX_FILE_ERROR) {
        کا->err = ngx_errno;
        کا->failed = ngx_file_info_n;
        واپس NGX_FILE_ERROR;
    }

    واپس rc;

#ورنہ

    ngx_fd_t  fd;

    اگر (کا->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {

        rc = ngx_file_info(name->ڈیٹا, fi);

        اگر (rc == NGX_FILE_ERROR) {
            کا->err = ngx_errno;
            کا->failed = ngx_file_info_n;
            واپس NGX_FILE_ERROR;
        }

        واپس rc;
    }

    fd = ngx_open_file_wrapper(name, کا, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,
                               NGX_FILE_OPEN, 0, لوگارتھم);

    اگر (fd == NGX_INVALID_FILE) {
        واپس NGX_FILE_ERROR;
    }

    rc = ngx_fd_info(fd, fi);

    اگر (rc == NGX_FILE_ERROR) {
        کا->err = ngx_errno;
        کا->failed = ngx_fd_info_n;
    }

    اگر (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, لوگارتھم, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", name);
    }

    واپس rc;
#اگر_ختم
}


ساکن ngx_int_t
ngx_open_and_stat_file(ngx_str_t *name, ngx_open_file_info_t *کا,
    ngx_log_t *لوگارتھم)
{
    ngx_fd_t         fd;
    ngx_file_info_t  fi;

    اگر (کا->fd != NGX_INVALID_FILE) {

        اگر (ngx_file_info_wrapper(name, کا, &fi, لوگارتھم) == NGX_FILE_ERROR) {
            کا->fd = NGX_INVALID_FILE;
            واپس NGX_ERROR;
        }

        اگر (کا->uniq == ngx_file_uniq(&fi)) {
            جاؤ مکمل;
        }

    } ورنہ اگر (کا->test_dir) {

        اگر (ngx_file_info_wrapper(name, کا, &fi, لوگارتھم) == NGX_FILE_ERROR) {
            کا->fd = NGX_INVALID_FILE;
            واپس NGX_ERROR;
        }

        اگر (ngx_is_dir(&fi)) {
            جاؤ مکمل;
        }
    }

    اگر (!کا->لوگارتھم) {

        /*
         * Use non-blocking open() not to hang on FIFO files, etc.
         * This flag has no effect on a regular files.
         */

        fd = ngx_open_file_wrapper(name, کا, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,
                                   NGX_FILE_OPEN, 0, لوگارتھم);

    } ورنہ {
        fd = ngx_open_file_wrapper(name, کا, NGX_FILE_APPEND,
                                   NGX_FILE_CREATE_OR_OPEN,
                                   NGX_FILE_DEFAULT_ACCESS, لوگارتھم);
    }

    اگر (fd == NGX_INVALID_FILE) {
        کا->fd = NGX_INVALID_FILE;
        واپس NGX_ERROR;
    }

    اگر (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, لوگارتھم, ngx_errno,
                      ngx_fd_info_n " \"%V\" failed", name);

        اگر (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, لوگارتھم, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        کا->fd = NGX_INVALID_FILE;

        واپس NGX_ERROR;
    }

    اگر (ngx_is_dir(&fi)) {
        اگر (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, لوگارتھم, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        کا->fd = NGX_INVALID_FILE;

    } ورنہ {
        کا->fd = fd;

        اگر (کا->read_ahead && ngx_file_سائز(&fi) > NGX_MIN_READ_AHEAD) {
            اگر (ngx_read_ahead(fd, کا->read_ahead) == NGX_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, لوگارتھم, ngx_errno,
                              ngx_read_ahead_n " \"%V\" failed", name);
            }
        }

        اگر (کا->directio <= ngx_file_سائز(&fi)) {
            اگر (ngx_directio_on(fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, لوگارتھم, ngx_errno,
                              ngx_directio_on_n " \"%V\" failed", name);

            } ورنہ {
                کا->is_directio = 1;
            }
        }
    }

مکمل:

    کا->uniq = ngx_file_uniq(&fi);
    کا->mtime = ngx_file_mtime(&fi);
    کا->سائز = ngx_file_سائز(&fi);
    کا->fs_سائز = ngx_file_fs_سائز(&fi);
    کا->is_dir = ngx_is_dir(&fi);
    کا->is_file = ngx_is_file(&fi);
    کا->is_link = ngx_is_link(&fi);
    کا->is_exec = ngx_is_exec(&fi);

    واپس NGX_OK;
}


/*
 * we ignore any possible event setting error and
 * fallback to usual periodic file retests
 */

ساکن خالی
ngx_open_file_جوڑو_event(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_open_file_info_t *کا, ngx_log_t *لوگارتھم)
{
    ngx_open_file_cache_event_t  *fev;

    اگر (!(ngx_event_flags & NGX_USE_VNODE_EVENT)
        || !کا->events
        || file->event
        || کا->fd == NGX_INVALID_FILE
        || file->استعمال_کرتا < کا->min_استعمال_کرتا)
    {
        واپس;
    }

    file->use_event = 0;

    file->event = ngx_calloc(حجم(ngx_event_t), لوگارتھم);
    اگر (file->event== تہی) {
        واپس;
    }

    fev = ngx_alloc(حجم(ngx_open_file_cache_event_t), لوگارتھم);
    اگر (fev == تہی) {
        ngx_free(file->event);
        file->event = تہی;
        واپس;
    }

    fev->fd = کا->fd;
    fev->file = file;
    fev->cache = cache;

    file->event->handler = ngx_open_file_cache_remove;
    file->event->ڈیٹا = fev;

    /*
     * although vnode event may be called while ngx_cycle->poll
     * destruction, however, cleanup procedures are run before any
     * memory freeing and events will be canceled.
     */

    file->event->لوگارتھم = ngx_چکر->لوگارتھم;

    اگر (ngx_جوڑو_event(file->event, NGX_VNODE_EVENT, NGX_ONESHOT_EVENT)
        != NGX_OK)
    {
        ngx_free(file->event->ڈیٹا);
        ngx_free(file->event);
        file->event = تہی;
        واپس;
    }

    /*
     * we do not set file->use_event here because there may be a race
     * condition: a file may be deleted between opening the file and
     * adding event, so we rely upon event notification only after
     * one file revalidation on next file access
     */

    واپس;
}


ساکن خالی
ngx_open_file_cleanup(خالی *ڈیٹا)
{
    ngx_open_file_cache_cleanup_t  *c = ڈیٹا;

    c->file->گنتی--;

    ngx_close_cached_file(c->cache, c->file, c->min_استعمال_کرتا, c->لوگارتھم);

    /* drop one or two expired open files */
    ngx_expire_old_cached_files(c->cache, 1, c->لوگارتھم);
}


ساکن خالی
ngx_close_cached_file(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_uint_t min_استعمال_کرتا, ngx_log_t *لوگارتھم)
{
    ngx_log_debug5(NGX_LOG_DEBUG_CORE, لوگارتھم, 0,
                   "close cached open file: %s, fd:%d, c:%d, u:%d, %d",
                   file->name, file->fd, file->گنتی, file->استعمال_کرتا, file->close);

    اگر (!file->close) {

        file->accessed = ngx_وقت();

        ngx_قطار_remove(&file->قطار);

        ngx_قطار_داخل_کرو_سر(&cache->expire_قطار, &file->قطار);

        اگر (file->استعمال_کرتا >= min_استعمال_کرتا || file->گنتی) {
            واپس;
        }
    }

    ngx_open_file_del_event(file);

    اگر (file->گنتی) {
        واپس;
    }

    اگر (file->fd != NGX_INVALID_FILE) {

        اگر (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, لوگارتھم, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", file->name);
        }

        file->fd = NGX_INVALID_FILE;
    }

    اگر (!file->close) {
        واپس;
    }

    ngx_free(file->name);
    ngx_free(file);
}


ساکن خالی
ngx_open_file_del_event(ngx_cached_open_file_t *file)
{
    اگر (file->event == تہی) {
        واپس;
    }

    (خالی) ngx_del_event(file->event, NGX_VNODE_EVENT,
                         file->گنتی ? NGX_FLUSH_EVENT : NGX_CLOSE_EVENT);

    ngx_free(file->event->ڈیٹا);
    ngx_free(file->event);
    file->event = تہی;
    file->use_event = 0;
}


ساکن خالی
ngx_expire_old_cached_files(ngx_open_file_cache_t *cache, ngx_uint_t n,
    ngx_log_t *لوگارتھم)
{
    وقت_t                   now;
    ngx_قطار_t             *q;
    ngx_cached_open_file_t  *file;

    now = ngx_وقت();

    /*
     * n == 1 deletes one or two inactive files
     * n == 0 deletes least recently used file by force
     *        and one or two inactive files
     */

    جب (n < 3) {

        اگر (ngx_قطار_خالی_خانے(&cache->expire_قطار)) {
            واپس;
        }

        q = ngx_قطار_آخری(&cache->expire_قطار);

        file = ngx_قطار_ڈیٹا(q, ngx_cached_open_file_t, قطار);

        اگر (n++ != 0 && now - file->accessed <= cache->inactive) {
            واپس;
        }

        ngx_قطار_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->گرہ);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, لوگارتھم, 0,
                       "expire cached open file: %s", file->name);

        اگر (!file->err && !file->is_dir) {
            file->close = 1;
            ngx_close_cached_file(cache, file, 0, لوگارتھم);

        } ورنہ {
            ngx_free(file->name);
            ngx_free(file);
        }
    }
}


ساکن خالی
ngx_open_file_cache_rbtree_داخل_کرو_قدر(ngx_rbtree_گرہ_t *عارضی,
    ngx_rbtree_گرہ_t *گرہ, ngx_rbtree_گرہ_t *sentinel)
{
    ngx_rbtree_گرہ_t       **p;
    ngx_cached_open_file_t    *file, *file_عارضی;

    برائے ( ;; ) {

        اگر (گرہ->کلید < عارضی->کلید) {

            p = &عارضی->بایاں;

        } ورنہ اگر (گرہ->کلید > عارضی->کلید) {

            p = &عارضی->دایاں;

        } ورنہ { /* node->key == temp->key */

            file = (ngx_cached_open_file_t *) گرہ;
            file_عارضی = (ngx_cached_open_file_t *) عارضی;

            p = (ngx_strcmp(file->name, file_عارضی->name) < 0)
                    ? &عارضی->بایاں : &عارضی->دایاں;
        }

        اگر (*p == sentinel) {
            توڑو;
        }

        عارضی = *p;
    }

    *p = گرہ;
    گرہ->والد = عارضی;
    گرہ->بایاں = sentinel;
    گرہ->دایاں = sentinel;
    ngx_rbt_red(گرہ);
}


ساکن ngx_cached_open_file_t *
ngx_open_file_lookup(ngx_open_file_cache_t *cache, ngx_str_t *name,
    uint32_t hash)
{
    ngx_int_t                rc;
    ngx_rbtree_گرہ_t       *گرہ, *sentinel;
    ngx_cached_open_file_t  *file;

    گرہ = cache->rbtree.جڑ;
    sentinel = cache->rbtree.sentinel;

    جب (گرہ != sentinel) {

        اگر (hash < گرہ->کلید) {
            گرہ = گرہ->بایاں;
            جاری;
        }

        اگر (hash > گرہ->کلید) {
            گرہ = گرہ->دایاں;
            جاری;
        }

        /* hash == node->key */

        file = (ngx_cached_open_file_t *) گرہ;

        rc = ngx_strcmp(name->ڈیٹا, file->name);

        اگر (rc == 0) {
            واپس file;
        }

        گرہ = (rc < 0) ? گرہ->بایاں : گرہ->دایاں;
    }

    واپس تہی;
}


ساکن خالی
ngx_open_file_cache_remove(ngx_event_t *ev)
{
    ngx_cached_open_file_t       *file;
    ngx_open_file_cache_event_t  *fev;

    fev = ev->ڈیٹا;
    file = fev->file;

    ngx_قطار_remove(&file->قطار);

    ngx_rbtree_delete(&fev->cache->rbtree, &file->گرہ);

    fev->cache->current--;

    /* NGX_ONESHOT_EVENT was already deleted */
    file->event = تہی;
    file->use_event = 0;

    file->close = 1;

    ngx_close_cached_file(fev->cache, file, 0, ev->لوگارتھم);

    /* free memory only when fev->cache and fev->file are already not needed */

    ngx_free(ev->ڈیٹا);
    ngx_free(ev);
}
