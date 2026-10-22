
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _NGX_OPEN_FILE_CACHE_H_INCLUDED_
#ವ್ಯಾಖ್ಯಾನಿಸು _NGX_OPEN_FILE_CACHE_H_INCLUDED_


#ವ್ಯಾಖ್ಯಾನಿಸು NGX_OPEN_FILE_DIRECTIO_OFF  NGX_MAX_OFF_T_ಮೌಲ್ಯ


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_fd_t                 fd;
    ngx_file_uniq_t          uniq;
    ಸಮಯ_t                   mtime;
    off_t                    ಗಾತ್ರ_2;
    off_t                    fs_ಗಾತ್ರ;
    off_t                    directio;
    ಗಾತ್ರ_t                   read_ahead;

    ngx_err_t                err;
    ಅಕ್ಷರ                    *failed;

    ಸಮಯ_t                   valid;

    ngx_uint_t               min_ಬಳಸುತ್ತದೆ;

#ಆದರೆ (NGX_HAVE_OPENAT)
    ಗಾತ್ರ_t                   disable_symlinks_from;
    ಚಿಹ್ನೆರಹಿತ                 disable_symlinks:2;
#ಆದರೆ_ಅಂತ್ಯ

    ಚಿಹ್ನೆರಹಿತ                 test_dir:1;
    ಚಿಹ್ನೆರಹಿತ                 test_only:1;
    ಚಿಹ್ನೆರಹಿತ                 ಲಘುಗಣಕ:1;
    ಚಿಹ್ನೆರಹಿತ                 errors:1;
    ಚಿಹ್ನೆರಹಿತ                 events:1;

    ಚಿಹ್ನೆರಹಿತ                 is_dir:1;
    ಚಿಹ್ನೆರಹಿತ                 is_file:1;
    ಚಿಹ್ನೆರಹಿತ                 is_link:1;
    ಚಿಹ್ನೆರಹಿತ                 is_exec:1;
    ಚಿಹ್ನೆರಹಿತ                 is_directio:1;
} ngx_open_file_info_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ngx_cached_open_file_s  ngx_cached_open_file_t;

ರಚನೆ ngx_cached_open_file_s {
    ngx_rbtree_ಗಂಟು_t        ಗಂಟು;
    ngx_ಸರತಿ_t              ಸರತಿ;

    u_char                  *name;
    ಸಮಯ_t                   created;
    ಸಮಯ_t                   accessed;

    ngx_fd_t                 fd;
    ngx_file_uniq_t          uniq;
    ಸಮಯ_t                   mtime;
    off_t                    ಗಾತ್ರ_2;
    ngx_err_t                err;

    uint32_t                 ಬಳಸುತ್ತದೆ;

#ಆದರೆ (NGX_HAVE_OPENAT)
    ಗಾತ್ರ_t                   disable_symlinks_from;
    ಚಿಹ್ನೆರಹಿತ                 disable_symlinks:2;
#ಆದರೆ_ಅಂತ್ಯ

    ಚಿಹ್ನೆರಹಿತ                 ಎಣಿಕೆ_2:24;
    ಚಿಹ್ನೆರಹಿತ                 close:1;
    ಚಿಹ್ನೆರಹಿತ                 use_event:1;

    ಚಿಹ್ನೆರಹಿತ                 is_dir:1;
    ಚಿಹ್ನೆರಹಿತ                 is_file:1;
    ಚಿಹ್ನೆರಹಿತ                 is_link:1;
    ಚಿಹ್ನೆರಹಿತ                 is_exec:1;
    ಚಿಹ್ನೆರಹಿತ                 is_directio:1;

    ngx_event_t             *event;
};


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_rbtree_t             rbtree;
    ngx_rbtree_ಗಂಟು_t        sentinel;
    ngx_ಸರತಿ_t              expire_ಸರತಿ;

    ngx_uint_t               current;
    ngx_uint_t               max;
    ಸಮಯ_t                   inactive;
} ngx_open_file_cache_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_open_file_cache_t   *cache;
    ngx_cached_open_file_t  *file;
    ngx_uint_t               min_ಬಳಸುತ್ತದೆ;
    ngx_log_t               *ಲಘುಗಣಕ;
} ngx_open_file_cache_cleanup_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {

    /* ngx_connection_t stub to allow use c->fd as event ident */
    ಶೂನ್ಯ                    *ದತ್ತಾಂಶ;
    ngx_event_t             *ಪಠ;
    ngx_event_t             *ಲಿಖ;
    ngx_fd_t                 fd;

    ngx_cached_open_file_t  *file;
    ngx_open_file_cache_t   *cache;
} ngx_open_file_cache_event_t;


ngx_open_file_cache_t *ngx_open_file_cache_init(ngx_pool_t *pool,
    ngx_uint_t max, ಸಮಯ_t inactive);
ngx_int_t ngx_open_cached_file(ngx_open_file_cache_t *cache, ngx_str_t *name,
    ngx_open_file_info_t *ದ, ngx_pool_t *pool);


#ಆದರೆ_ಅಂತ್ಯ /* _NGX_OPEN_FILE_CACHE_H_INCLUDED_ */
