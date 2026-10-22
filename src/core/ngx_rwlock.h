
/*
 * Copyright (C) Ruslan Ermilov
 * Copyright (C) Nginx, Inc.
 */


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _NGX_RWLOCK_H_INCLUDED_
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _NGX_RWLOCK_H_INCLUDED_


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


ਖਾਲੀ ngx_rwlock_wlock(ngx_atomic_t *ਤਾਲਾ);
ਖਾਲੀ ngx_rwlock_rlock(ngx_atomic_t *ਤਾਲਾ);
ਖਾਲੀ ngx_rwlock_unlock(ngx_atomic_t *ਤਾਲਾ);
ਖਾਲੀ ngx_rwlock_downgrade(ngx_atomic_t *ਤਾਲਾ);


#ਜੇ_ਅੰਤ /* _NGX_RWLOCK_H_INCLUDED_ */
