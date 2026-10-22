
/*
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_BPF_LOGBUF_ಗಾತ್ರ  (32 * 1024)


ಸ್ಥಿರ ngx_inline ಪೂರ್ಣಾಂಕ
ngx_bpf(ಎಣಿಕೆ bpf_cmd cmd, ಒಕ್ಕೂಟ bpf_attr *attr, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2)
{
    ಹಿಂತಿರುಗಿ syscall(__NR_bpf, cmd, attr, ಗಾತ್ರ_2);
}


ಶೂನ್ಯ
ngx_bpf_ಕಾರ್ಯಕ್ರಮ_link(ngx_bpf_ಕಾರ್ಯಕ್ರಮ_t *ಕಾರ್ಯಕ್ರಮ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಚಿಹ್ನೆ, ಪೂರ್ಣಾಂಕ fd)
{
    ngx_uint_t        i;
    ngx_bpf_reloc_t  *rl;

    rl = ಕಾರ್ಯಕ್ರಮ->relocs;

    ಗಾಗಿ (i = 0; i < ಕಾರ್ಯಕ್ರಮ->nrelocs; i++) {
        ಆದರೆ (ngx_strcmp(rl[i].name, ಚಿಹ್ನೆ) == 0) {
            ಕಾರ್ಯಕ್ರಮ->ins[rl[i].ಸ್ಥಾನಾಂತರ].src_reg = 1;
            ಕಾರ್ಯಕ್ರಮ->ins[rl[i].ಸ್ಥಾನಾಂತರ].imm = fd;
        }
    }
}


ಪೂರ್ಣಾಂಕ
ngx_bpf_ತುಂಬು_ಕಾರ್ಯಕ್ರಮ(ngx_log_t *ಲಘುಗಣಕ, ngx_bpf_ಕಾರ್ಯಕ್ರಮ_t *ಕಾರ್ಯಕ್ರಮ)
{
    ಪೂರ್ಣಾಂಕ             fd;
    ಒಕ್ಕೂಟ bpf_attr  attr;
#ಆದರೆ (NGX_DEBUG)
    ಅಕ್ಷರ            buf[NGX_BPF_LOGBUF_ಗಾತ್ರ];
#ಆದರೆ_ಅಂತ್ಯ

    ngx_memzero(&attr, ಗಾತ್ರ(ಒಕ್ಕೂಟ bpf_attr));

    attr.license = (uintptr_t) ಕಾರ್ಯಕ್ರಮ->license;
    attr.prog_type = ಕಾರ್ಯಕ್ರಮ->type;
    attr.insns = (uintptr_t) ಕಾರ್ಯಕ್ರಮ->ins;
    attr.insn_cnt = ಕಾರ್ಯಕ್ರಮ->nins;

#ಆದರೆ (NGX_DEBUG)
    /* for verifier errors */
    attr.log_buf = (uintptr_t) buf;
    attr.log_ಗಾತ್ರ = NGX_BPF_LOGBUF_ಗಾತ್ರ;
    attr.log_level = 1;
#ಆದರೆ_ಅಂತ್ಯ

    fd = ngx_bpf(BPF_PROG_ತುಂಬು, &attr, ಗಾತ್ರ(attr));
    ಆದರೆ (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, ಲಘುಗಣಕ, ngx_errno,
                      "failed to load BPF program");

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ಲಘುಗಣಕ, 0,
                       "bpf verifier: %s", buf);

        ಹಿಂತಿರುಗಿ -1;
    }

    ಹಿಂತಿರುಗಿ fd;
}


ಪೂರ್ಣಾಂಕ
ngx_bpf_map_create(ngx_log_t *ಲಘುಗಣಕ, ಎಣಿಕೆ bpf_map_type type, ಪೂರ್ಣಾಂಕ ಕೀಲಿ_ಗಾತ್ರ,
    ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ_ಗಾತ್ರ, ಪೂರ್ಣಾಂಕ max_entries, uint32_t map_flags)
{
    ಪೂರ್ಣಾಂಕ             fd;
    ಒಕ್ಕೂಟ bpf_attr  attr;

    ngx_memzero(&attr, ಗಾತ್ರ(ಒಕ್ಕೂಟ bpf_attr));

    attr.map_type = type;
    attr.ಕೀಲಿ_ಗಾತ್ರ = ಕೀಲಿ_ಗಾತ್ರ;
    attr.ಮೌಲ್ಯ_ಗಾತ್ರ = ಮೌಲ್ಯ_ಗಾತ್ರ;
    attr.max_entries = max_entries;
    attr.map_flags = map_flags;

    fd = ngx_bpf(BPF_MAP_CREATE, &attr, ಗಾತ್ರ(attr));
    ಆದರೆ (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, ಲಘುಗಣಕ, ngx_errno,
                      "failed to create BPF map");
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಹಿಂತಿರುಗಿ fd;
}


ಪೂರ್ಣಾಂಕ
ngx_bpf_map_update(ಪೂರ್ಣಾಂಕ fd, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಕೀಲಿ, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಮೌಲ್ಯ, uint64_t flags)
{
    ಒಕ್ಕೂಟ bpf_attr attr;

    ngx_memzero(&attr, ಗಾತ್ರ(ಒಕ್ಕೂಟ bpf_attr));

    attr.map_fd = fd;
    attr.ಕೀಲಿ = (uintptr_t) ಕೀಲಿ;
    attr.ಮೌಲ್ಯ = (uintptr_t) ಮೌಲ್ಯ;
    attr.flags = flags;

    ಹಿಂತಿರುಗಿ ngx_bpf(BPF_MAP_UPDATE_ELEM, &attr, ಗಾತ್ರ(attr));
}


ಪೂರ್ಣಾಂಕ
ngx_bpf_map_delete(ಪೂರ್ಣಾಂಕ fd, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಕೀಲಿ)
{
    ಒಕ್ಕೂಟ bpf_attr attr;

    ngx_memzero(&attr, ಗಾತ್ರ(ಒಕ್ಕೂಟ bpf_attr));

    attr.map_fd = fd;
    attr.ಕೀಲಿ = (uintptr_t) ಕೀಲಿ;

    ಹಿಂತಿರುಗಿ ngx_bpf(BPF_MAP_DELETE_ELEM, &attr, ಗಾತ್ರ(attr));
}


ಪೂರ್ಣಾಂಕ
ngx_bpf_map_lookup(ಪೂರ್ಣಾಂಕ fd, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಕೀಲಿ, ಶೂನ್ಯ *ಮೌಲ್ಯ)
{
    ಒಕ್ಕೂಟ bpf_attr attr;

    ngx_memzero(&attr, ಗಾತ್ರ(ಒಕ್ಕೂಟ bpf_attr));

    attr.map_fd = fd;
    attr.ಕೀಲಿ = (uintptr_t) ಕೀಲಿ;
    attr.ಮೌಲ್ಯ = (uintptr_t) ಮೌಲ್ಯ;

    ಹಿಂತಿರುಗಿ ngx_bpf(BPF_MAP_LOOKUP_ELEM, &attr, ಗಾತ್ರ(attr));
}
