
/*
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_BPF_H_INCLUDED_
#define _NGX_BPF_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>

#ضمن <linux/bpf.h>


تعريفنوع بنية {
    حرف                *name;
    صحيح                  offset;
} ngx_bpf_reloc_t;

تعريفنوع بنية {
    حرف                *license;
    تعداد bpf_prog_type   type;
    بنية bpf_insn     *ins;
    size_t               nins;
    ngx_bpf_reloc_t     *relocs;
    size_t               nrelocs;
} ngx_bpf_program_t;


فراغ ngx_bpf_program_link(ngx_bpf_program_t *program, ثابت حرف *سيمبول,
    صحيح fd);
صحيح ngx_bpf_load_program(ngx_log_t *log, ngx_bpf_program_t *program);

صحيح ngx_bpf_map_create(ngx_log_t *log, تعداد bpf_map_type type, صحيح کونچي_size,
    صحيح نيلاي_size, صحيح max_entries, uint32_t map_flags);
صحيح ngx_bpf_map_update(صحيح fd, ثابت فراغ *کونچي, ثابت فراغ *نيلاي,
    uint64_t flags);
صحيح ngx_bpf_map_delete(صحيح fd, ثابت فراغ *کونچي);
صحيح ngx_bpf_map_lookup(صحيح fd, ثابت فراغ *کونچي, فراغ *نيلاي);

#endif /* _NGX_BPF_H_INCLUDED_ */
