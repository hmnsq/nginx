
/*
 * Copyright (C) Ruslan Ermilov
 * Copyright (C) Nginx, Inc.
 */


#جيڪڏهن_وصف_نه_ٿيل _NGX_RWLOCK_H_INCLUDED_
#وصف_ڪريو _NGX_RWLOCK_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


فراغ ngx_rwlock_wlock(ngx_atomic_t *تالو);
فراغ ngx_rwlock_rlock(ngx_atomic_t *تالو);
فراغ ngx_rwlock_unlock(ngx_atomic_t *تالو);
فراغ ngx_rwlock_downgrade(ngx_atomic_t *تالو);


#جيڪڏهن_پورو /* _NGX_RWLOCK_H_INCLUDED_ */
