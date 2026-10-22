
/*
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_BPF_H_INCLUDED_
#define _NGX_BPF_H_INCLUDED_


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>

#ଅନ୍ତର୍ଭାଵ <linux/bpf.h>


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ଅକ୍ଷର                *name;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା                  offset;
} ngx_bpf_reloc_t;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ଅକ୍ଷର                *license;
    ଗଣନା bpf_prog_type   type;
    ସଂରଚନା bpf_insn     *ins;
    size_t               nins;
    ngx_bpf_reloc_t     *relocs;
    size_t               nrelocs;
} ngx_bpf_program_t;


ଶୂନ୍ୟ ngx_bpf_program_link(ngx_bpf_program_t *program, ଅଚଳ ଅକ୍ଷର *ପ୍ରତୀକ,
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା ngx_bpf_load_program(ngx_log_t *log, ngx_bpf_program_t *program);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ngx_bpf_map_create(ngx_log_t *log, ଗଣନା bpf_map_type type, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଚାବି_size,
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ_size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_entries, uint32_t map_flags);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା ngx_bpf_map_update(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅଚଳ ଶୂନ୍ୟ *ଚାବି, ଅଚଳ ଶୂନ୍ୟ *ମୂଲ୍ୟ,
    uint64_t flags);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା ngx_bpf_map_delete(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅଚଳ ଶୂନ୍ୟ *ଚାବି);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା ngx_bpf_map_lookup(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅଚଳ ଶୂନ୍ୟ *ଚାବି, ଶୂନ୍ୟ *ମୂଲ୍ୟ);

#endif /* _NGX_BPF_H_INCLUDED_ */
