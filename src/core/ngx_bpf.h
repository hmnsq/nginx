
/*
 * Copyright (C) Nginx, Inc.
 */


#جے_بنا_تعریف _NGX_BPF_H_INCLUDED_
#تعریف_کرو _NGX_BPF_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>

#ضمن <linux/bpf.h>


تعريفنوع بنية {
    حرف                *name;
    صحيح                  وتھ;
} ngx_bpf_reloc_t;

تعريفنوع بنية {
    حرف                *license;
    تعداد bpf_prog_type   type;
    بنية bpf_insn     *ins;
    ناپ_t               nins;
    ngx_bpf_reloc_t     *relocs;
    ناپ_t               nrelocs;
} ngx_bpf_پروگرام_t;


فراغ ngx_bpf_پروگرام_link(ngx_bpf_پروگرام_t *پروگرام, ثابت حرف *علامت,
    صحيح fd);
صحيح ngx_bpf_لدو_پروگرام(ngx_log_t *لوگارتھم, ngx_bpf_پروگرام_t *پروگرام);

صحيح ngx_bpf_map_create(ngx_log_t *لوگارتھم, تعداد bpf_map_type type, صحيح کلید_ناپ,
    صحيح قدر_ناپ, صحيح max_entries, uint32_t map_flags);
صحيح ngx_bpf_map_update(صحيح fd, ثابت فراغ *کلید, ثابت فراغ *قدر,
    uint64_t flags);
صحيح ngx_bpf_map_delete(صحيح fd, ثابت فراغ *کلید);
صحيح ngx_bpf_map_lookup(صحيح fd, ثابت فراغ *کلید, فراغ *قدر);

#جے_انت /* _NGX_BPF_H_INCLUDED_ */
