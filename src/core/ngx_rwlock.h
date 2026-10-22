
/*
 * Copyright (C) Ruslan Ermilov
 * Copyright (C) Nginx, Inc.
 */


#اگر_غیر_تعریف_شدہ _NGX_RWLOCK_H_INCLUDED_
#تعریف_کرو _NGX_RWLOCK_H_INCLUDED_


#شامل <ngx_config.h>
#شامل <ngx_core.h>


خالی ngx_rwlock_wlock(ngx_atomic_t *تالا);
خالی ngx_rwlock_rlock(ngx_atomic_t *تالا);
خالی ngx_rwlock_unlock(ngx_atomic_t *تالا);
خالی ngx_rwlock_downgrade(ngx_atomic_t *تالا);


#اگر_ختم /* _NGX_RWLOCK_H_INCLUDED_ */
