
/*
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_BPF_H_INCLUDED_
#define _NGX_BPF_H_INCLUDED_


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>

#অন্তর্ভাব <linux/bpf.h>


প্রকারলেখ কাঠামো {
    অক্ষর                *name;
    পূর্ণসংখ্যা                  offset;
} ngx_bpf_reloc_t;

প্রকারলেখ কাঠামো {
    অক্ষর                *license;
    গণনা bpf_prog_type   type;
    কাঠামো bpf_insn     *ins;
    আকার_t               nins;
    ngx_bpf_reloc_t     *relocs;
    আকার_t               nrelocs;
} ngx_bpf_program_t;


শূন্য ngx_bpf_program_link(ngx_bpf_program_t *program, ধ্রুবক অক্ষর *প্রতীক,
    পূর্ণসংখ্যা fd);
পূর্ণসংখ্যা ngx_bpf_load_program(ngx_log_t *log, ngx_bpf_program_t *program);

পূর্ণসংখ্যা ngx_bpf_map_create(ngx_log_t *log, গণনা bpf_map_type type, পূর্ণসংখ্যা চাবি_আকার,
    পূর্ণসংখ্যা মান_আকার, পূর্ণসংখ্যা max_entries, uint32_t map_flags);
পূর্ণসংখ্যা ngx_bpf_map_update(পূর্ণসংখ্যা fd, ধ্রুবক শূন্য *চাবি, ধ্রুবক শূন্য *মান,
    uint64_t flags);
পূর্ণসংখ্যা ngx_bpf_map_delete(পূর্ণসংখ্যা fd, ধ্রুবক শূন্য *চাবি);
পূর্ণসংখ্যা ngx_bpf_map_lookup(পূর্ণসংখ্যা fd, ধ্রুবক শূন্য *চাবি, শূন্য *মান);

#endif /* _NGX_BPF_H_INCLUDED_ */
