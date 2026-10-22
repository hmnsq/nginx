
/*
 * Copyright (C) Nginx, Inc.
 */


#اگر_تعریف_نشده _NGX_BPF_H_INCLUDED_
#تعریف_کن _NGX_BPF_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>

#ضمن <linux/bpf.h>


تعريفنوع بنية {
    حرف                *name;
    صحيح                  جابه_جایی;
} ngx_bpf_reloc_t;

تعريفنوع بنية {
    حرف                *license;
    تعداد bpf_prog_type   type;
    بنية bpf_insn     *ins;
    اندازه_t               nins;
    ngx_bpf_reloc_t     *relocs;
    اندازه_t               nrelocs;
} ngx_bpf_برنامه_t;


فراغ ngx_bpf_برنامه_link(ngx_bpf_برنامه_t *برنامه, ثابت حرف *نماد,
    صحيح fd);
صحيح ngx_bpf_بارگیری_برنامه(ngx_log_t *لگاریتم, ngx_bpf_برنامه_t *برنامه);

صحيح ngx_bpf_map_create(ngx_log_t *لگاریتم, تعداد bpf_map_type type, صحيح کلید_اندازه,
    صحيح مقدار_اندازه, صحيح max_entries, uint32_t map_flags);
صحيح ngx_bpf_map_update(صحيح fd, ثابت فراغ *کلید, ثابت فراغ *مقدار,
    uint64_t flags);
صحيح ngx_bpf_map_delete(صحيح fd, ثابت فراغ *کلید);
صحيح ngx_bpf_map_lookup(صحيح fd, ثابت فراغ *کلید, فراغ *مقدار);

#پایان_اگر /* _NGX_BPF_H_INCLUDED_ */
