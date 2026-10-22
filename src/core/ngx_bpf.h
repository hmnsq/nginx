
/*
 * Copyright (C) Nginx, Inc.
 */


#اگر_غیر_تعریف_شدہ _NGX_BPF_H_INCLUDED_
#تعریف_کرو _NGX_BPF_H_INCLUDED_


#شامل <ngx_config.h>
#شامل <ngx_core.h>

#شامل <linux/bpf.h>


قسمتعریف ساخت {
    حرف                *name;
    صحیح                  انحراف;
} ngx_bpf_reloc_t;

قسمتعریف ساخت {
    حرف                *license;
    شمار bpf_prog_type   type;
    ساخت bpf_insn     *ins;
    سائز_t               nins;
    ngx_bpf_reloc_t     *relocs;
    سائز_t               nrelocs;
} ngx_bpf_پروگرام_t;


خالی ngx_bpf_پروگرام_link(ngx_bpf_پروگرام_t *پروگرام, ثابت حرف *علامت,
    صحیح fd);
صحیح ngx_bpf_بھرو_پروگرام(ngx_log_t *لوگارتھم, ngx_bpf_پروگرام_t *پروگرام);

صحیح ngx_bpf_map_create(ngx_log_t *لوگارتھم, شمار bpf_map_type type, صحیح کلید_سائز,
    صحیح قدر_سائز, صحیح max_entries, uint32_t map_flags);
صحیح ngx_bpf_map_update(صحیح fd, ثابت خالی *کلید, ثابت خالی *قدر,
    uint64_t flags);
صحیح ngx_bpf_map_delete(صحیح fd, ثابت خالی *کلید);
صحیح ngx_bpf_map_lookup(صحیح fd, ثابت خالی *کلید, خالی *قدر);

#اگر_ختم /* _NGX_BPF_H_INCLUDED_ */
