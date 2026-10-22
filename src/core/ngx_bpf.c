
/*
 * Copyright (C) Nginx, Inc.
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_BPF_LOGBUF_ਮਾਪ  (32 * 1024)


ਸਥਿਰ ngx_inline ਪੂਰਨ
ngx_bpf(ਗਿਣਤੀ bpf_cmd cmd, ਸੰਘ bpf_attr *attr, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ)
{
    ਵਾਪਸ syscall(__NR_bpf, cmd, attr, ਮਾਪ);
}


ਖਾਲੀ
ngx_bpf_ਪ੍ਰੋਗਰਾਮ_link(ngx_bpf_ਪ੍ਰੋਗਰਾਮ_t *ਪ੍ਰੋਗਰਾਮ, ਅਚਲ ਅੱਖਰ *ਚਿੰਨ੍ਹ, ਪੂਰਨ fd)
{
    ngx_uint_t        i;
    ngx_bpf_reloc_t  *rl;

    rl = ਪ੍ਰੋਗਰਾਮ->relocs;

    ਲਈ (i = 0; i < ਪ੍ਰੋਗਰਾਮ->nrelocs; i++) {
        ਜੇ (ngx_strcmp(rl[i].name, ਚਿੰਨ੍ਹ) == 0) {
            ਪ੍ਰੋਗਰਾਮ->ins[rl[i].ਵਿੱਥ].src_reg = 1;
            ਪ੍ਰੋਗਰਾਮ->ins[rl[i].ਵਿੱਥ].imm = fd;
        }
    }
}


ਪੂਰਨ
ngx_bpf_ਲੱਦੋ_ਪ੍ਰੋਗਰਾਮ(ngx_log_t *ਲਘੂਗਣਕ, ngx_bpf_ਪ੍ਰੋਗਰਾਮ_t *ਪ੍ਰੋਗਰਾਮ)
{
    ਪੂਰਨ             fd;
    ਸੰਘ bpf_attr  attr;
#ਜੇ (NGX_DEBUG)
    ਅੱਖਰ            buf[NGX_BPF_LOGBUF_ਮਾਪ];
#ਜੇ_ਅੰਤ

    ngx_memzero(&attr, ਆਕਾਰ(ਸੰਘ bpf_attr));

    attr.license = (uintptr_t) ਪ੍ਰੋਗਰਾਮ->license;
    attr.prog_type = ਪ੍ਰੋਗਰਾਮ->type;
    attr.insns = (uintptr_t) ਪ੍ਰੋਗਰਾਮ->ins;
    attr.insn_cnt = ਪ੍ਰੋਗਰਾਮ->nins;

#ਜੇ (NGX_DEBUG)
    /* for verifier errors */
    attr.log_buf = (uintptr_t) buf;
    attr.log_ਮਾਪ = NGX_BPF_LOGBUF_ਮਾਪ;
    attr.log_level = 1;
#ਜੇ_ਅੰਤ

    fd = ngx_bpf(BPF_PROG_ਲੱਦੋ, &attr, ਆਕਾਰ(attr));
    ਜੇ (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, ਲਘੂਗਣਕ, ngx_errno,
                      "failed to load BPF program");

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ਲਘੂਗਣਕ, 0,
                       "bpf verifier: %s", buf);

        ਵਾਪਸ -1;
    }

    ਵਾਪਸ fd;
}


ਪੂਰਨ
ngx_bpf_map_create(ngx_log_t *ਲਘੂਗਣਕ, ਗਿਣਤੀ bpf_map_type type, ਪੂਰਨ ਕੁੰਜੀ_ਮਾਪ,
    ਪੂਰਨ ਮੁੱਲ_ਮਾਪ, ਪੂਰਨ max_entries, uint32_t map_flags)
{
    ਪੂਰਨ             fd;
    ਸੰਘ bpf_attr  attr;

    ngx_memzero(&attr, ਆਕਾਰ(ਸੰਘ bpf_attr));

    attr.map_type = type;
    attr.ਕੁੰਜੀ_ਮਾਪ = ਕੁੰਜੀ_ਮਾਪ;
    attr.ਮੁੱਲ_ਮਾਪ = ਮੁੱਲ_ਮਾਪ;
    attr.max_entries = max_entries;
    attr.map_flags = map_flags;

    fd = ngx_bpf(BPF_MAP_CREATE, &attr, ਆਕਾਰ(attr));
    ਜੇ (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, ਲਘੂਗਣਕ, ngx_errno,
                      "failed to create BPF map");
        ਵਾਪਸ NGX_ERROR;
    }

    ਵਾਪਸ fd;
}


ਪੂਰਨ
ngx_bpf_map_update(ਪੂਰਨ fd, ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ, ਅਚਲ ਖਾਲੀ *ਮੁੱਲ, uint64_t flags)
{
    ਸੰਘ bpf_attr attr;

    ngx_memzero(&attr, ਆਕਾਰ(ਸੰਘ bpf_attr));

    attr.map_fd = fd;
    attr.ਕੁੰਜੀ = (uintptr_t) ਕੁੰਜੀ;
    attr.ਮੁੱਲ = (uintptr_t) ਮੁੱਲ;
    attr.flags = flags;

    ਵਾਪਸ ngx_bpf(BPF_MAP_UPDATE_ELEM, &attr, ਆਕਾਰ(attr));
}


ਪੂਰਨ
ngx_bpf_map_delete(ਪੂਰਨ fd, ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ)
{
    ਸੰਘ bpf_attr attr;

    ngx_memzero(&attr, ਆਕਾਰ(ਸੰਘ bpf_attr));

    attr.map_fd = fd;
    attr.ਕੁੰਜੀ = (uintptr_t) ਕੁੰਜੀ;

    ਵਾਪਸ ngx_bpf(BPF_MAP_DELETE_ELEM, &attr, ਆਕਾਰ(attr));
}


ਪੂਰਨ
ngx_bpf_map_lookup(ਪੂਰਨ fd, ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ, ਖਾਲੀ *ਮੁੱਲ)
{
    ਸੰਘ bpf_attr attr;

    ngx_memzero(&attr, ਆਕਾਰ(ਸੰਘ bpf_attr));

    attr.map_fd = fd;
    attr.ਕੁੰਜੀ = (uintptr_t) ਕੁੰਜੀ;
    attr.ਮੁੱਲ = (uintptr_t) ਮੁੱਲ;

    ਵਾਪਸ ngx_bpf(BPF_MAP_LOOKUP_ELEM, &attr, ਆਕਾਰ(attr));
}
