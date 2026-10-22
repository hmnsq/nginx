
/*
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_BPF_H_INCLUDED_
#define _NGX_BPF_H_INCLUDED_


#شامل <ngx_config.h>
#شامل <ngx_core.h>

#شامل <linux/bpf.h>


تعریفنوع ساختار {
    نویسه                *name;
    صحیح                  offset;
} ngx_bpf_reloc_t;

تعریفنوع ساختار {
    نویسه                *license;
    شمارش bpf_prog_type   type;
    ساختار bpf_insn     *ins;
    size_t               nins;
    ngx_bpf_reloc_t     *relocs;
    size_t               nrelocs;
} ngx_bpf_program_t;


تهی ngx_bpf_program_link(ngx_bpf_program_t *program, ثابت نویسه *نماد,
    صحیح fd);
صحیح ngx_bpf_load_program(ngx_log_t *log, ngx_bpf_program_t *program);

صحیح ngx_bpf_map_create(ngx_log_t *log, شمارش bpf_map_type type, صحیح کلید_size,
    صحیح مقدار_size, صحیح max_entries, uint32_t map_flags);
صحیح ngx_bpf_map_update(صحیح fd, ثابت تهی *کلید, ثابت تهی *مقدار,
    uint64_t flags);
صحیح ngx_bpf_map_delete(صحیح fd, ثابت تهی *کلید);
صحیح ngx_bpf_map_lookup(صحیح fd, ثابت تهی *کلید, تهی *مقدار);

#endif /* _NGX_BPF_H_INCLUDED_ */
