
/*
 * Copyright (C) Nginx, Inc.
 */


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _NGX_BPF_H_INCLUDED_
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _NGX_BPF_H_INCLUDED_


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <linux/bpf.h>


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ਅੱਖਰ                *name;
    ਪੂਰਨ                  ਵਿੱਥ;
} ngx_bpf_reloc_t;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ਅੱਖਰ                *license;
    ਗਿਣਤੀ bpf_prog_type   type;
    ਬਣਤਰ bpf_insn     *ins;
    ਮਾਪ_t               nins;
    ngx_bpf_reloc_t     *relocs;
    ਮਾਪ_t               nrelocs;
} ngx_bpf_ਪ੍ਰੋਗਰਾਮ_t;


ਖਾਲੀ ngx_bpf_ਪ੍ਰੋਗਰਾਮ_link(ngx_bpf_ਪ੍ਰੋਗਰਾਮ_t *ਪ੍ਰੋਗਰਾਮ, ਅਚਲ ਅੱਖਰ *ਚਿੰਨ੍ਹ,
    ਪੂਰਨ fd);
ਪੂਰਨ ngx_bpf_ਲੱਦੋ_ਪ੍ਰੋਗਰਾਮ(ngx_log_t *ਲਘੂਗਣਕ, ngx_bpf_ਪ੍ਰੋਗਰਾਮ_t *ਪ੍ਰੋਗਰਾਮ);

ਪੂਰਨ ngx_bpf_map_create(ngx_log_t *ਲਘੂਗਣਕ, ਗਿਣਤੀ bpf_map_type type, ਪੂਰਨ ਕੁੰਜੀ_ਮਾਪ,
    ਪੂਰਨ ਮੁੱਲ_ਮਾਪ, ਪੂਰਨ max_entries, uint32_t map_flags);
ਪੂਰਨ ngx_bpf_map_update(ਪੂਰਨ fd, ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ, ਅਚਲ ਖਾਲੀ *ਮੁੱਲ,
    uint64_t flags);
ਪੂਰਨ ngx_bpf_map_delete(ਪੂਰਨ fd, ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ);
ਪੂਰਨ ngx_bpf_map_lookup(ਪੂਰਨ fd, ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ, ਖਾਲੀ *ਮੁੱਲ);

#ਜੇ_ਅੰਤ /* _NGX_BPF_H_INCLUDED_ */
