
/*
 * Copyright (C) Ruslan Ermilov
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_RWLOCK_H_INCLUDED_
#define _NGX_RWLOCK_H_INCLUDED_


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


শূন্য ngx_rwlock_wlock(ngx_atomic_t *lock);
শূন্য ngx_rwlock_rlock(ngx_atomic_t *lock);
শূন্য ngx_rwlock_unlock(ngx_atomic_t *lock);
শূন্য ngx_rwlock_downgrade(ngx_atomic_t *lock);


#endif /* _NGX_RWLOCK_H_INCLUDED_ */
