
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


#وصف_ڪريو NGX_MIN_READ_AHEAD  (128 * 1024)


ساكن فراغ ngx_open_file_cache_cleanup(فراغ *معلومات);
#إذا (NGX_HAVE_OPENAT)
ساكن ngx_fd_t ngx_openat_file_owner(ngx_fd_t at_fd, ثابت u_char *name,
    ngx_int_t گهڻائي, ngx_int_t create, ngx_int_t access, ngx_log_t *لاگرٿم);
#إذا (NGX_HAVE_O_رستو)
ساكن ngx_int_t ngx_file_o_رستو_info(ngx_fd_t fd, ngx_file_info_t *fi,
    ngx_log_t *لاگرٿم);
#جيڪڏهن_پورو
#جيڪڏهن_پورو
ساكن ngx_fd_t ngx_open_file_wrapper(ngx_str_t *name,
    ngx_open_file_info_t *جو, ngx_int_t گهڻائي, ngx_int_t create,
    ngx_int_t access, ngx_log_t *لاگرٿم);
ساكن ngx_int_t ngx_file_info_wrapper(ngx_str_t *name,
    ngx_open_file_info_t *جو, ngx_file_info_t *fi, ngx_log_t *لاگرٿم);
ساكن ngx_int_t ngx_open_and_stat_file(ngx_str_t *name,
    ngx_open_file_info_t *جو, ngx_log_t *لاگرٿم);
ساكن فراغ ngx_open_file_جوڙيو_event(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_open_file_info_t *جو, ngx_log_t *لاگرٿم);
ساكن فراغ ngx_open_file_cleanup(فراغ *معلومات);
ساكن فراغ ngx_close_cached_file(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_uint_t min_استعمال_ڪري, ngx_log_t *لاگرٿم);
ساكن فراغ ngx_open_file_del_event(ngx_cached_open_file_t *file);
ساكن فراغ ngx_expire_old_cached_files(ngx_open_file_cache_t *cache,
    ngx_uint_t n, ngx_log_t *لاگرٿم);
ساكن فراغ ngx_open_file_cache_rbtree_وجهو_قدر(ngx_rbtree_ڳنڍ_t *عارضي,
    ngx_rbtree_ڳنڍ_t *ڳنڍ, ngx_rbtree_ڳنڍ_t *sentinel);
ساكن ngx_cached_open_file_t *
    ngx_open_file_lookup(ngx_open_file_cache_t *cache, ngx_str_t *name,
    uint32_t hash);
ساكن فراغ ngx_open_file_cache_remove(ngx_event_t *ev);


ngx_open_file_cache_t *
ngx_open_file_cache_init(ngx_pool_t *pool, ngx_uint_t max, وقت_t inactive)
{
    ngx_pool_cleanup_t     *cln;
    ngx_open_file_cache_t  *cache;

    cache = ngx_palloc(pool, حجم(ngx_open_file_cache_t));
    إذا (cache == عدم) {
        أرجع عدم;
    }

    ngx_rbtree_init(&cache->rbtree, &cache->sentinel,
                    ngx_open_file_cache_rbtree_وجهو_قدر);

    ngx_قطار_init(&cache->expire_قطار);

    cache->current = 0;
    cache->max = max;
    cache->inactive = inactive;

    cln = ngx_pool_cleanup_جوڙيو(pool, 0);
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

    ngx_قطار_t             *q;
    ngx_cached_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_چڪر->لاگرٿم, 0,
                   "open file cache cleanup");

    لكل ( ;; ) {

        إذا (ngx_قطار_خالي_جايون(&cache->expire_قطار)) {
            اكسر;
        }

        q = ngx_قطار_آخري(&cache->expire_قطار);

        file = ngx_قطار_معلومات(q, ngx_cached_open_file_t, قطار);

        ngx_قطار_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->ڳنڍ);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_چڪر->لاگرٿم, 0,
                       "delete cached open file: %s", file->name);

        إذا (!file->err && !file->is_dir) {
            file->close = 1;
            file->ڳڻپ = 0;
            ngx_close_cached_file(cache, file, 0, ngx_چڪر->لاگرٿم);

        } وإلا {
            ngx_free(file->name);
            ngx_free(file);
        }
    }

    إذا (cache->current) {
        ngx_log_error(NGX_LOG_ALERT, ngx_چڪر->لاگرٿم, 0,
                      "%ui items still left in open file cache",
                      cache->current);
    }

    إذا (cache->rbtree.پاڙ != cache->rbtree.sentinel) {
        ngx_log_error(NGX_LOG_ALERT, ngx_چڪر->لاگرٿم, 0,
                      "rbtree still is not empty in open file cache");

    }
}


ngx_int_t
ngx_open_cached_file(ngx_open_file_cache_t *cache, ngx_str_t *name,
    ngx_open_file_info_t *جو, ngx_pool_t *pool)
{
    وقت_t                          now;
    uint32_t                        hash;
    ngx_int_t                       rc;
    ngx_file_info_t                 fi;
    ngx_pool_cleanup_t             *cln;
    ngx_cached_open_file_t         *file;
    ngx_pool_cleanup_file_t        *clnf;
    ngx_open_file_cache_cleanup_t  *ofcln;

    جو->fd = NGX_INVALID_FILE;
    جو->err = 0;

    إذا (cache == عدم) {

        إذا (جو->test_only) {

            إذا (ngx_file_info_wrapper(name, جو, &fi, pool->لاگرٿم)
                == NGX_FILE_ERROR)
            {
                أرجع NGX_ERROR;
            }

            جو->uniq = ngx_file_uniq(&fi);
            جو->mtime = ngx_file_mtime(&fi);
            جو->سائيز = ngx_file_سائيز(&fi);
            جو->fs_سائيز = ngx_file_fs_سائيز(&fi);
            جو->is_dir = ngx_is_dir(&fi);
            جو->is_file = ngx_is_file(&fi);
            جو->is_link = ngx_is_link(&fi);
            جو->is_exec = ngx_is_exec(&fi);

            أرجع NGX_OK;
        }

        cln = ngx_pool_cleanup_جوڙيو(pool, حجم(ngx_pool_cleanup_file_t));
        إذا (cln == عدم) {
            أرجع NGX_ERROR;
        }

        rc = ngx_open_and_stat_file(name, جو, pool->لاگرٿم);

        إذا (rc == NGX_OK && !جو->is_dir) {
            cln->handler = ngx_pool_cleanup_file;
            clnf = cln->معلومات;

            clnf->fd = جو->fd;
            clnf->name = name->معلومات;
            clnf->لاگرٿم = pool->لاگرٿم;
        }

        أرجع rc;
    }

    cln = ngx_pool_cleanup_جوڙيو(pool, حجم(ngx_open_file_cache_cleanup_t));
    إذا (cln == عدم) {
        أرجع NGX_ERROR;
    }

    now = ngx_وقت();

    hash = ngx_چڪري_جاچ_32_long(name->معلومات, name->len);

    file = ngx_open_file_lookup(cache, name, hash);

    إذا (file) {

        file->استعمال_ڪري++;

        ngx_قطار_remove(&file->قطار);

        إذا (file->fd == NGX_INVALID_FILE && file->err == 0 && !file->is_dir) {

            /* file was not used often enough to keep open */

            rc = ngx_open_and_stat_file(name, جو, pool->لاگرٿم);

            إذا (rc != NGX_OK && (جو->err == 0 || !جو->errors)) {
                اذهب failed;
            }

            اذهب جوڙيو_event;
        }

        إذا (file->use_event
            || (file->event == عدم
                && (جو->uniq == 0 || جو->uniq == file->uniq)
                && now - file->created < جو->valid
#إذا (NGX_HAVE_OPENAT)
                && جو->disable_symlinks == file->disable_symlinks
                && جو->disable_symlinks_from == file->disable_symlinks_from
#جيڪڏهن_پورو
            ))
        {
            إذا (file->err == 0) {

                جو->fd = file->fd;
                جو->uniq = file->uniq;
                جو->mtime = file->mtime;
                جو->سائيز = file->سائيز;

                جو->is_dir = file->is_dir;
                جو->is_file = file->is_file;
                جو->is_link = file->is_link;
                جو->is_exec = file->is_exec;
                جو->is_directio = file->is_directio;

                إذا (!file->is_dir) {
                    file->ڳڻپ++;
                    ngx_open_file_جوڙيو_event(cache, file, جو, pool->لاگرٿم);
                }

            } وإلا {
                جو->err = file->err;
#إذا (NGX_HAVE_OPENAT)
                جو->failed = file->disable_symlinks ? ngx_openat_file_n
                                                    : ngx_open_file_n;
#وإلا
                جو->failed = ngx_open_file_n;
#جيڪڏهن_پورو
            }

            اذهب مليو;
        }

        ngx_log_debug4(NGX_LOG_DEBUG_CORE, pool->لاگرٿم, 0,
                       "retest open file: %s, fd:%d, c:%d, e:%d",
                       file->name, file->fd, file->ڳڻپ, file->err);

        إذا (file->is_dir) {

            /*
             * chances that directory became file are very small
             * so test_dir flag allows to use a single syscall
             * in ngx_file_info() instead of three syscalls
             */

            جو->test_dir = 1;
        }

        جو->fd = file->fd;
        جو->uniq = file->uniq;

        rc = ngx_open_and_stat_file(name, جو, pool->لاگرٿم);

        إذا (rc != NGX_OK && (جو->err == 0 || !جو->errors)) {
            اذهب failed;
        }

        إذا (جو->is_dir) {

            إذا (file->is_dir || file->err) {
                اذهب update;
            }

            /* file became directory */

        } وإلا إذا (جو->err == 0) {  /* file */

            إذا (file->is_dir || file->err) {
                اذهب جوڙيو_event;
            }

            إذا (جو->uniq == file->uniq) {

                إذا (file->event) {
                    file->use_event = 1;
                }

                جو->is_directio = file->is_directio;

                اذهب update;
            }

            /* file was changed */

        } وإلا { /* error to cache */

            إذا (file->err || file->is_dir) {
                اذهب update;
            }

            /* file was removed, etc. */
        }

        إذا (file->ڳڻپ == 0) {

            ngx_open_file_del_event(file);

            إذا (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, pool->لاگرٿم, ngx_errno,
                              ngx_close_file_n " \"%V\" failed", name);
            }

            اذهب جوڙيو_event;
        }

        ngx_rbtree_delete(&cache->rbtree, &file->ڳنڍ);

        cache->current--;

        file->close = 1;

        اذهب create;
    }

    /* not found */

    rc = ngx_open_and_stat_file(name, جو, pool->لاگرٿم);

    إذا (rc != NGX_OK && (جو->err == 0 || !جو->errors)) {
        اذهب failed;
    }

create:

    إذا (cache->current >= cache->max) {
        ngx_expire_old_cached_files(cache, 0, pool->لاگرٿم);
    }

    file = ngx_alloc(حجم(ngx_cached_open_file_t), pool->لاگرٿم);

    إذا (file == عدم) {
        اذهب failed;
    }

    file->name = ngx_alloc(name->len + 1, pool->لاگرٿم);

    إذا (file->name == عدم) {
        ngx_free(file);
        file = عدم;
        اذهب failed;
    }

    ngx_cpystrn(file->name, name->معلومات, name->len + 1);

    file->ڳنڍ.ڪنجي = hash;

    ngx_rbtree_وجهو(&cache->rbtree, &file->ڳنڍ);

    cache->current++;

    file->استعمال_ڪري = 1;
    file->ڳڻپ = 0;
    file->use_event = 0;
    file->event = عدم;

جوڙيو_event:

    ngx_open_file_جوڙيو_event(cache, file, جو, pool->لاگرٿم);

update:

    file->fd = جو->fd;
    file->err = جو->err;
#إذا (NGX_HAVE_OPENAT)
    file->disable_symlinks = جو->disable_symlinks;
    file->disable_symlinks_from = جو->disable_symlinks_from;
#جيڪڏهن_پورو

    إذا (جو->err == 0) {
        file->uniq = جو->uniq;
        file->mtime = جو->mtime;
        file->سائيز = جو->سائيز;

        file->close = 0;

        file->is_dir = جو->is_dir;
        file->is_file = جو->is_file;
        file->is_link = جو->is_link;
        file->is_exec = جو->is_exec;
        file->is_directio = جو->is_directio;

        إذا (!جو->is_dir) {
            file->ڳڻپ++;
        }
    }

    file->created = now;

مليو:

    file->accessed = now;

    ngx_قطار_وجهو_سر(&cache->expire_قطار, &file->قطار);

    ngx_log_debug5(NGX_LOG_DEBUG_CORE, pool->لاگرٿم, 0,
                   "cached open file: %s, fd:%d, c:%d, e:%d, u:%d",
                   file->name, file->fd, file->ڳڻپ, file->err, file->استعمال_ڪري);

    إذا (جو->err == 0) {

        إذا (!جو->is_dir) {
            cln->handler = ngx_open_file_cleanup;
            ofcln = cln->معلومات;

            ofcln->cache = cache;
            ofcln->file = file;
            ofcln->min_استعمال_ڪري = جو->min_استعمال_ڪري;
            ofcln->لاگرٿم = pool->لاگرٿم;
        }

        أرجع NGX_OK;
    }

    أرجع NGX_ERROR;

failed:

    إذا (file) {
        ngx_rbtree_delete(&cache->rbtree, &file->ڳنڍ);

        cache->current--;

        إذا (file->ڳڻپ == 0) {

            إذا (file->fd != NGX_INVALID_FILE) {
                إذا (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_ALERT, pool->لاگرٿم, ngx_errno,
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

    إذا (جو->fd != NGX_INVALID_FILE) {
        إذا (ngx_close_file(جو->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, pool->لاگرٿم, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }
    }

    أرجع NGX_ERROR;
}


#إذا (NGX_HAVE_OPENAT)

ساكن ngx_fd_t
ngx_openat_file_owner(ngx_fd_t at_fd, ثابت u_char *name,
    ngx_int_t گهڻائي, ngx_int_t create, ngx_int_t access, ngx_log_t *لاگرٿم)
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

    fd = ngx_openat_file(at_fd, name, گهڻائي, create, access);

    إذا (fd == NGX_INVALID_FILE) {
        أرجع NGX_INVALID_FILE;
    }

    إذا (ngx_file_at_info(at_fd, name, &atfi, AT_SYMLINK_NOFOLLOW)
        == NGX_FILE_ERROR)
    {
        err = ngx_errno;
        اذهب failed;
    }

#إذا (NGX_HAVE_O_رستو)
    إذا (ngx_file_o_رستو_info(fd, &fi, لاگرٿم) == NGX_ERROR) {
        err = ngx_errno;
        اذهب failed;
    }
#وإلا
    إذا (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        err = ngx_errno;
        اذهب failed;
    }
#جيڪڏهن_پورو

    إذا (fi.st_uid != atfi.st_uid) {
        err = NGX_ELOOP;
        اذهب failed;
    }

    أرجع fd;

failed:

    إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, لاگرٿم, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", name);
    }

    ngx_سيٽ_errno(err);

    أرجع NGX_INVALID_FILE;
}


#إذا (NGX_HAVE_O_رستو)

ساكن ngx_int_t
ngx_file_o_رستو_info(ngx_fd_t fd, ngx_file_info_t *fi, ngx_log_t *لاگرٿم)
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

        ngx_log_error(NGX_LOG_NOTICE, لاگرٿم, 0,
                      "fstat(O_PATH) failed with EBADF, "
                      "switching to fstatat(AT_EMPTY_PATH)");

        use_fstat = 0;
    }

    إذا (ngx_file_at_info(fd, "", fi, AT_خالي_جايون_رستو) != NGX_FILE_ERROR) {
        أرجع NGX_OK;
    }

    أرجع NGX_ERROR;
}

#جيڪڏهن_پورو

#جيڪڏهن_پورو /* NGX_HAVE_OPENAT */


ساكن ngx_fd_t
ngx_open_file_wrapper(ngx_str_t *name, ngx_open_file_info_t *جو,
    ngx_int_t گهڻائي, ngx_int_t create, ngx_int_t access, ngx_log_t *لاگرٿم)
{
    ngx_fd_t  fd;

#إذا !(NGX_HAVE_OPENAT)

    fd = ngx_open_file(name->معلومات, گهڻائي, create, access);

    إذا (fd == NGX_INVALID_FILE) {
        جو->err = ngx_errno;
        جو->failed = ngx_open_file_n;
        أرجع NGX_INVALID_FILE;
    }

    أرجع fd;

#وإلا

    u_char           *p, *cp, *پڇاڙي;
    ngx_fd_t          at_fd;
    ngx_str_t         at_name;

    إذا (جو->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {
        fd = ngx_open_file(name->معلومات, گهڻائي, create, access);

        إذا (fd == NGX_INVALID_FILE) {
            جو->err = ngx_errno;
            جو->failed = ngx_open_file_n;
            أرجع NGX_INVALID_FILE;
        }

        أرجع fd;
    }

    p = name->معلومات;
    پڇاڙي = p + name->len;

    at_name = *name;

    إذا (جو->disable_symlinks_from) {

        cp = p + جو->disable_symlinks_from;

        *cp = '\0';

        at_fd = ngx_open_file(p, NGX_FILE_ڳولا|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        *cp = '/';

        إذا (at_fd == NGX_INVALID_FILE) {
            جو->err = ngx_errno;
            جو->failed = ngx_open_file_n;
            أرجع NGX_INVALID_FILE;
        }

        at_name.len = جو->disable_symlinks_from;
        p = cp + 1;

    } وإلا إذا (*p == '/') {

        at_fd = ngx_open_file("/",
                              NGX_FILE_ڳولا|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        إذا (at_fd == NGX_INVALID_FILE) {
            جو->err = ngx_errno;
            جو->failed = ngx_openat_file_n;
            أرجع NGX_INVALID_FILE;
        }

        at_name.len = 1;
        p++;

    } وإلا {
        at_fd = NGX_AT_FDCWD;
    }

    لكل ( ;; ) {
        cp = ngx_strlchr(p, پڇاڙي, '/');
        إذا (cp == عدم) {
            اكسر;
        }

        إذا (cp == p) {
            p++;
            استمر;
        }

        *cp = '\0';

        إذا (جو->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER) {
            fd = ngx_openat_file_owner(at_fd, p,
                                       NGX_FILE_ڳولا|NGX_FILE_NONBLOCK,
                                       NGX_FILE_OPEN, 0, لاگرٿم);

        } وإلا {
            fd = ngx_openat_file(at_fd, p,
                           NGX_FILE_ڳولا|NGX_FILE_NONBLOCK|NGX_FILE_NOFOLLOW,
                           NGX_FILE_OPEN, 0);
        }

        *cp = '/';

        إذا (fd == NGX_INVALID_FILE) {
            جو->err = ngx_errno;
            جو->failed = ngx_openat_file_n;
            اذهب failed;
        }

        إذا (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, لاگرٿم, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", &at_name);
        }

        p = cp + 1;
        at_fd = fd;
        at_name.len = cp - at_name.معلومات;
    }

    إذا (p == پڇاڙي) {

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

        fd = ngx_openat_file(at_fd, ".", گهڻائي, create, access);
        اذهب پورو;
    }

    إذا (جو->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER
        && !(create & (NGX_FILE_CREATE_OR_OPEN|NGX_FILE_TRUNCATE)))
    {
        fd = ngx_openat_file_owner(at_fd, p, گهڻائي, create, access, لاگرٿم);

    } وإلا {
        fd = ngx_openat_file(at_fd, p, گهڻائي|NGX_FILE_NOFOLLOW, create, access);
    }

پورو:

    إذا (fd == NGX_INVALID_FILE) {
        جو->err = ngx_errno;
        جو->failed = ngx_openat_file_n;
    }

failed:

    إذا (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, لاگرٿم, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", &at_name);
    }

    أرجع fd;
#جيڪڏهن_پورو
}


ساكن ngx_int_t
ngx_file_info_wrapper(ngx_str_t *name, ngx_open_file_info_t *جو,
    ngx_file_info_t *fi, ngx_log_t *لاگرٿم)
{
    ngx_int_t  rc;

#إذا !(NGX_HAVE_OPENAT)

    rc = ngx_file_info(name->معلومات, fi);

    إذا (rc == NGX_FILE_ERROR) {
        جو->err = ngx_errno;
        جو->failed = ngx_file_info_n;
        أرجع NGX_FILE_ERROR;
    }

    أرجع rc;

#وإلا

    ngx_fd_t  fd;

    إذا (جو->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {

        rc = ngx_file_info(name->معلومات, fi);

        إذا (rc == NGX_FILE_ERROR) {
            جو->err = ngx_errno;
            جو->failed = ngx_file_info_n;
            أرجع NGX_FILE_ERROR;
        }

        أرجع rc;
    }

    fd = ngx_open_file_wrapper(name, جو, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,
                               NGX_FILE_OPEN, 0, لاگرٿم);

    إذا (fd == NGX_INVALID_FILE) {
        أرجع NGX_FILE_ERROR;
    }

    rc = ngx_fd_info(fd, fi);

    إذا (rc == NGX_FILE_ERROR) {
        جو->err = ngx_errno;
        جو->failed = ngx_fd_info_n;
    }

    إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, لاگرٿم, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", name);
    }

    أرجع rc;
#جيڪڏهن_پورو
}


ساكن ngx_int_t
ngx_open_and_stat_file(ngx_str_t *name, ngx_open_file_info_t *جو,
    ngx_log_t *لاگرٿم)
{
    ngx_fd_t         fd;
    ngx_file_info_t  fi;

    إذا (جو->fd != NGX_INVALID_FILE) {

        إذا (ngx_file_info_wrapper(name, جو, &fi, لاگرٿم) == NGX_FILE_ERROR) {
            جو->fd = NGX_INVALID_FILE;
            أرجع NGX_ERROR;
        }

        إذا (جو->uniq == ngx_file_uniq(&fi)) {
            اذهب پورو;
        }

    } وإلا إذا (جو->test_dir) {

        إذا (ngx_file_info_wrapper(name, جو, &fi, لاگرٿم) == NGX_FILE_ERROR) {
            جو->fd = NGX_INVALID_FILE;
            أرجع NGX_ERROR;
        }

        إذا (ngx_is_dir(&fi)) {
            اذهب پورو;
        }
    }

    إذا (!جو->لاگرٿم) {

        /*
         * Use non-blocking open() not to hang on FIFO files, etc.
         * This flag has no effect on a regular files.
         */

        fd = ngx_open_file_wrapper(name, جو, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,
                                   NGX_FILE_OPEN, 0, لاگرٿم);

    } وإلا {
        fd = ngx_open_file_wrapper(name, جو, NGX_FILE_APPEND,
                                   NGX_FILE_CREATE_OR_OPEN,
                                   NGX_FILE_DEFAULT_ACCESS, لاگرٿم);
    }

    إذا (fd == NGX_INVALID_FILE) {
        جو->fd = NGX_INVALID_FILE;
        أرجع NGX_ERROR;
    }

    إذا (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, لاگرٿم, ngx_errno,
                      ngx_fd_info_n " \"%V\" failed", name);

        إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, لاگرٿم, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        جو->fd = NGX_INVALID_FILE;

        أرجع NGX_ERROR;
    }

    إذا (ngx_is_dir(&fi)) {
        إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, لاگرٿم, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        جو->fd = NGX_INVALID_FILE;

    } وإلا {
        جو->fd = fd;

        إذا (جو->read_ahead && ngx_file_سائيز(&fi) > NGX_MIN_READ_AHEAD) {
            إذا (ngx_read_ahead(fd, جو->read_ahead) == NGX_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, لاگرٿم, ngx_errno,
                              ngx_read_ahead_n " \"%V\" failed", name);
            }
        }

        إذا (جو->directio <= ngx_file_سائيز(&fi)) {
            إذا (ngx_directio_on(fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, لاگرٿم, ngx_errno,
                              ngx_directio_on_n " \"%V\" failed", name);

            } وإلا {
                جو->is_directio = 1;
            }
        }
    }

پورو:

    جو->uniq = ngx_file_uniq(&fi);
    جو->mtime = ngx_file_mtime(&fi);
    جو->سائيز = ngx_file_سائيز(&fi);
    جو->fs_سائيز = ngx_file_fs_سائيز(&fi);
    جو->is_dir = ngx_is_dir(&fi);
    جو->is_file = ngx_is_file(&fi);
    جو->is_link = ngx_is_link(&fi);
    جو->is_exec = ngx_is_exec(&fi);

    أرجع NGX_OK;
}


/*
 * we ignore any possible event setting error and
 * fallback to usual periodic file retests
 */

ساكن فراغ
ngx_open_file_جوڙيو_event(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_open_file_info_t *جو, ngx_log_t *لاگرٿم)
{
    ngx_open_file_cache_event_t  *fev;

    إذا (!(ngx_event_flags & NGX_USE_VNODE_EVENT)
        || !جو->events
        || file->event
        || جو->fd == NGX_INVALID_FILE
        || file->استعمال_ڪري < جو->min_استعمال_ڪري)
    {
        أرجع;
    }

    file->use_event = 0;

    file->event = ngx_calloc(حجم(ngx_event_t), لاگرٿم);
    إذا (file->event== عدم) {
        أرجع;
    }

    fev = ngx_alloc(حجم(ngx_open_file_cache_event_t), لاگرٿم);
    إذا (fev == عدم) {
        ngx_free(file->event);
        file->event = عدم;
        أرجع;
    }

    fev->fd = جو->fd;
    fev->file = file;
    fev->cache = cache;

    file->event->handler = ngx_open_file_cache_remove;
    file->event->معلومات = fev;

    /*
     * although vnode event may be called while ngx_cycle->poll
     * destruction, however, cleanup procedures are run before any
     * memory freeing and events will be canceled.
     */

    file->event->لاگرٿم = ngx_چڪر->لاگرٿم;

    إذا (ngx_جوڙيو_event(file->event, NGX_VNODE_EVENT, NGX_ONESHOT_EVENT)
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

    c->file->ڳڻپ--;

    ngx_close_cached_file(c->cache, c->file, c->min_استعمال_ڪري, c->لاگرٿم);

    /* drop one or two expired open files */
    ngx_expire_old_cached_files(c->cache, 1, c->لاگرٿم);
}


ساكن فراغ
ngx_close_cached_file(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_uint_t min_استعمال_ڪري, ngx_log_t *لاگرٿم)
{
    ngx_log_debug5(NGX_LOG_DEBUG_CORE, لاگرٿم, 0,
                   "close cached open file: %s, fd:%d, c:%d, u:%d, %d",
                   file->name, file->fd, file->ڳڻپ, file->استعمال_ڪري, file->close);

    إذا (!file->close) {

        file->accessed = ngx_وقت();

        ngx_قطار_remove(&file->قطار);

        ngx_قطار_وجهو_سر(&cache->expire_قطار, &file->قطار);

        إذا (file->استعمال_ڪري >= min_استعمال_ڪري || file->ڳڻپ) {
            أرجع;
        }
    }

    ngx_open_file_del_event(file);

    إذا (file->ڳڻپ) {
        أرجع;
    }

    إذا (file->fd != NGX_INVALID_FILE) {

        إذا (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, لاگرٿم, ngx_errno,
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
                         file->ڳڻپ ? NGX_FLUSH_EVENT : NGX_CLOSE_EVENT);

    ngx_free(file->event->معلومات);
    ngx_free(file->event);
    file->event = عدم;
    file->use_event = 0;
}


ساكن فراغ
ngx_expire_old_cached_files(ngx_open_file_cache_t *cache, ngx_uint_t n,
    ngx_log_t *لاگرٿم)
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

    طالما (n < 3) {

        إذا (ngx_قطار_خالي_جايون(&cache->expire_قطار)) {
            أرجع;
        }

        q = ngx_قطار_آخري(&cache->expire_قطار);

        file = ngx_قطار_معلومات(q, ngx_cached_open_file_t, قطار);

        إذا (n++ != 0 && now - file->accessed <= cache->inactive) {
            أرجع;
        }

        ngx_قطار_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->ڳنڍ);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, لاگرٿم, 0,
                       "expire cached open file: %s", file->name);

        إذا (!file->err && !file->is_dir) {
            file->close = 1;
            ngx_close_cached_file(cache, file, 0, لاگرٿم);

        } وإلا {
            ngx_free(file->name);
            ngx_free(file);
        }
    }
}


ساكن فراغ
ngx_open_file_cache_rbtree_وجهو_قدر(ngx_rbtree_ڳنڍ_t *عارضي,
    ngx_rbtree_ڳنڍ_t *ڳنڍ, ngx_rbtree_ڳنڍ_t *sentinel)
{
    ngx_rbtree_ڳنڍ_t       **p;
    ngx_cached_open_file_t    *file, *file_عارضي;

    لكل ( ;; ) {

        إذا (ڳنڍ->ڪنجي < عارضي->ڪنجي) {

            p = &عارضي->کاٻو;

        } وإلا إذا (ڳنڍ->ڪنجي > عارضي->ڪنجي) {

            p = &عارضي->ساڄو;

        } وإلا { /* node->key == temp->key */

            file = (ngx_cached_open_file_t *) ڳنڍ;
            file_عارضي = (ngx_cached_open_file_t *) عارضي;

            p = (ngx_strcmp(file->name, file_عارضي->name) < 0)
                    ? &عارضي->کاٻو : &عارضي->ساڄو;
        }

        إذا (*p == sentinel) {
            اكسر;
        }

        عارضي = *p;
    }

    *p = ڳنڍ;
    ڳنڍ->والد = عارضي;
    ڳنڍ->کاٻو = sentinel;
    ڳنڍ->ساڄو = sentinel;
    ngx_rbt_red(ڳنڍ);
}


ساكن ngx_cached_open_file_t *
ngx_open_file_lookup(ngx_open_file_cache_t *cache, ngx_str_t *name,
    uint32_t hash)
{
    ngx_int_t                rc;
    ngx_rbtree_ڳنڍ_t       *ڳنڍ, *sentinel;
    ngx_cached_open_file_t  *file;

    ڳنڍ = cache->rbtree.پاڙ;
    sentinel = cache->rbtree.sentinel;

    طالما (ڳنڍ != sentinel) {

        إذا (hash < ڳنڍ->ڪنجي) {
            ڳنڍ = ڳنڍ->کاٻو;
            استمر;
        }

        إذا (hash > ڳنڍ->ڪنجي) {
            ڳنڍ = ڳنڍ->ساڄو;
            استمر;
        }

        /* hash == node->key */

        file = (ngx_cached_open_file_t *) ڳنڍ;

        rc = ngx_strcmp(name->معلومات, file->name);

        إذا (rc == 0) {
            أرجع file;
        }

        ڳنڍ = (rc < 0) ? ڳنڍ->کاٻو : ڳنڍ->ساڄو;
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

    ngx_قطار_remove(&file->قطار);

    ngx_rbtree_delete(&fev->cache->rbtree, &file->ڳنڍ);

    fev->cache->current--;

    /* NGX_ONESHOT_EVENT was already deleted */
    file->event = عدم;
    file->use_event = 0;

    file->close = 1;

    ngx_close_cached_file(fev->cache, file, 0, ev->لاگرٿم);

    /* free memory only when fev->cache and fev->file are already not needed */

    ngx_free(ev->معلومات);
    ngx_free(ev);
}
