
/*
 * Copyright (C) Ruslan Ermilov
 * Copyright (C) Nginx, Inc.
 */


#که_نه_تعریف_شوی _NGX_RWLOCK_H_INCLUDED_
#تعریف_کړه _NGX_RWLOCK_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


فراغ ngx_rwlock_wlock(ngx_atomic_t *قلف);
فراغ ngx_rwlock_rlock(ngx_atomic_t *قلف);
فراغ ngx_rwlock_unlock(ngx_atomic_t *قلف);
فراغ ngx_rwlock_downgrade(ngx_atomic_t *قلف);


#د_که_پای /* _NGX_RWLOCK_H_INCLUDED_ */
