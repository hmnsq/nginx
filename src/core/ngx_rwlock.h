
/*
 * Copyright (C) Ruslan Ermilov
 * Copyright (C) Nginx, Inc.
 */


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _NGX_RWLOCK_H_INCLUDED_
#ವ್ಯಾಖ್ಯಾನಿಸು _NGX_RWLOCK_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಶೂನ್ಯ ngx_rwlock_wlock(ngx_atomic_t *ಬೀಗ);
ಶೂನ್ಯ ngx_rwlock_rlock(ngx_atomic_t *ಬೀಗ);
ಶೂನ್ಯ ngx_rwlock_unlock(ngx_atomic_t *ಬೀಗ);
ಶೂನ್ಯ ngx_rwlock_downgrade(ngx_atomic_t *ಬೀಗ);


#ಆದರೆ_ಅಂತ್ಯ /* _NGX_RWLOCK_H_INCLUDED_ */
