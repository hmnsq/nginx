
/*
 * Copyright (C) Nginx, Inc.
 */


#جيڪڏهن_وصف_نه_ٿيل _NGX_BPF_H_INCLUDED_
#وصف_ڪريو _NGX_BPF_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>

#ضمن <linux/bpf.h>


تعريفنوع بنية {
    حرف                *name;
    صحيح                  وٿي;
} ngx_bpf_reloc_t;

تعريفنوع بنية {
    حرف                *license;
    تعداد bpf_prog_type   type;
    بنية bpf_insn     *ins;
    سائيز_t               nins;
    ngx_bpf_reloc_t     *relocs;
    سائيز_t               nrelocs;
} ngx_bpf_پروگرام_t;


فراغ ngx_bpf_پروگرام_link(ngx_bpf_پروگرام_t *پروگرام, ثابت حرف *نشان,
    صحيح fd);
صحيح ngx_bpf_ڀريو_پروگرام(ngx_log_t *لاگرٿم, ngx_bpf_پروگرام_t *پروگرام);

صحيح ngx_bpf_map_create(ngx_log_t *لاگرٿم, تعداد bpf_map_type type, صحيح ڪنجي_سائيز,
    صحيح قدر_سائيز, صحيح max_entries, uint32_t map_flags);
صحيح ngx_bpf_map_update(صحيح fd, ثابت فراغ *ڪنجي, ثابت فراغ *قدر,
    uint64_t flags);
صحيح ngx_bpf_map_delete(صحيح fd, ثابت فراغ *ڪنجي);
صحيح ngx_bpf_map_lookup(صحيح fd, ثابت فراغ *ڪنجي, فراغ *قدر);

#جيڪڏهن_پورو /* _NGX_BPF_H_INCLUDED_ */
