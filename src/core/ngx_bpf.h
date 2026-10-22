
/*
 * Copyright (C) Nginx, Inc.
 */


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _NGX_BPF_H_INCLUDED_
#ವ್ಯಾಖ್ಯಾನಿಸು _NGX_BPF_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>

#ಅನ್ತರ್ಭಾವ <linux/bpf.h>


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ಅಕ್ಷರ                *name;
    ಪೂರ್ಣಾಂಕ                  ಸ್ಥಾನಾಂತರ;
} ngx_bpf_reloc_t;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ಅಕ್ಷರ                *license;
    ಎಣಿಕೆ bpf_prog_type   type;
    ರಚನೆ bpf_insn     *ins;
    ಗಾತ್ರ_t               nins;
    ngx_bpf_reloc_t     *relocs;
    ಗಾತ್ರ_t               nrelocs;
} ngx_bpf_ಕಾರ್ಯಕ್ರಮ_t;


ಶೂನ್ಯ ngx_bpf_ಕಾರ್ಯಕ್ರಮ_link(ngx_bpf_ಕಾರ್ಯಕ್ರಮ_t *ಕಾರ್ಯಕ್ರಮ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಚಿಹ್ನೆ,
    ಪೂರ್ಣಾಂಕ fd);
ಪೂರ್ಣಾಂಕ ngx_bpf_ತುಂಬು_ಕಾರ್ಯಕ್ರಮ(ngx_log_t *ಲಘುಗಣಕ, ngx_bpf_ಕಾರ್ಯಕ್ರಮ_t *ಕಾರ್ಯಕ್ರಮ);

ಪೂರ್ಣಾಂಕ ngx_bpf_map_create(ngx_log_t *ಲಘುಗಣಕ, ಎಣಿಕೆ bpf_map_type type, ಪೂರ್ಣಾಂಕ ಕೀಲಿ_ಗಾತ್ರ,
    ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ_ಗಾತ್ರ, ಪೂರ್ಣಾಂಕ max_entries, uint32_t map_flags);
ಪೂರ್ಣಾಂಕ ngx_bpf_map_update(ಪೂರ್ಣಾಂಕ fd, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಕೀಲಿ, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಮೌಲ್ಯ,
    uint64_t flags);
ಪೂರ್ಣಾಂಕ ngx_bpf_map_delete(ಪೂರ್ಣಾಂಕ fd, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಕೀಲಿ);
ಪೂರ್ಣಾಂಕ ngx_bpf_map_lookup(ಪೂರ್ಣಾಂಕ fd, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಕೀಲಿ, ಶೂನ್ಯ *ಮೌಲ್ಯ);

#ಆದರೆ_ಅಂತ್ಯ /* _NGX_BPF_H_INCLUDED_ */
