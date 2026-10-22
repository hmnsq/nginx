
/*
 * Copyright (C) Ruslan Ermilov
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_RWLOCK_H_INCLUDED_
#define _NGX_RWLOCK_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


فراغ ngx_rwlock_wlock(ngx_atomic_t *lock);
فراغ ngx_rwlock_rlock(ngx_atomic_t *lock);
فراغ ngx_rwlock_unlock(ngx_atomic_t *lock);
فراغ ngx_rwlock_downgrade(ngx_atomic_t *lock);


#endif /* _NGX_RWLOCK_H_INCLUDED_ */
