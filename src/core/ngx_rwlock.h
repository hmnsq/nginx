
/*
 * Copyright (C) Ruslan Ermilov
 * Copyright (C) Nginx, Inc.
 */


#यदि_अपरिभाषितम् _NGX_RWLOCK_H_INCLUDED_
#परिभाषय _NGX_RWLOCK_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


शून्य ngx_rwlock_wlock(ngx_atomic_t *तालकम्);
शून्य ngx_rwlock_rlock(ngx_atomic_t *तालकम्);
शून्य ngx_rwlock_unlock(ngx_atomic_t *तालकम्);
शून्य ngx_rwlock_downgrade(ngx_atomic_t *तालकम्);


#यदि_अन्तः /* _NGX_RWLOCK_H_INCLUDED_ */
