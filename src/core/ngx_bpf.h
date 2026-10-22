
/*
 * Copyright (C) Nginx, Inc.
 */


#که_نه_تعریف_شوی _NGX_BPF_H_INCLUDED_
#تعریف_کړه _NGX_BPF_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>

#ضمن <linux/bpf.h>


تعريفنوع بنية {
    حرف                *name;
    صحيح                  دننه_واټن;
} ngx_bpf_reloc_t;

تعريفنوع بنية {
    حرف                *license;
    تعداد bpf_prog_type   type;
    بنية bpf_insn     *ins;
    اندازه_t               nins;
    ngx_bpf_reloc_t     *relocs;
    اندازه_t               nrelocs;
} ngx_bpf_پروګرام_t;


فراغ ngx_bpf_پروګرام_link(ngx_bpf_پروګرام_t *پروګرام, ثابت حرف *نښه,
    صحيح fd);
صحيح ngx_bpf_بارول_پروګرام(ngx_log_t *لوګاریتم, ngx_bpf_پروګرام_t *پروګرام);

صحيح ngx_bpf_map_create(ngx_log_t *لوګاریتم, تعداد bpf_map_type type, صحيح کلی_اندازه,
    صحيح ارزښت_اندازه, صحيح max_entries, uint32_t map_flags);
صحيح ngx_bpf_map_update(صحيح fd, ثابت فراغ *کلی, ثابت فراغ *ارزښت,
    uint64_t flags);
صحيح ngx_bpf_map_delete(صحيح fd, ثابت فراغ *کلی);
صحيح ngx_bpf_map_lookup(صحيح fd, ثابت فراغ *کلی, فراغ *ارزښت);

#د_که_پای /* _NGX_BPF_H_INCLUDED_ */
