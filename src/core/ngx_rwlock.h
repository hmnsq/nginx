
/*
 * Copyright (C) Ruslan Ermilov
 * Copyright (C) Nginx, Inc.
 */


#جے_بنا_تعریف _NGX_RWLOCK_H_INCLUDED_
#تعریف_کرو _NGX_RWLOCK_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


فراغ ngx_rwlock_wlock(ngx_atomic_t *جندرا);
فراغ ngx_rwlock_rlock(ngx_atomic_t *جندرا);
فراغ ngx_rwlock_unlock(ngx_atomic_t *جندرا);
فراغ ngx_rwlock_downgrade(ngx_atomic_t *جندرا);


#جے_انت /* _NGX_RWLOCK_H_INCLUDED_ */
