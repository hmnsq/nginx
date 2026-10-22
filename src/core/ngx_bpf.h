
/*
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_BPF_H_INCLUDED_
#define _NGX_BPF_H_INCLUDED_


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>

#অন্তর্ভাব <linux/bpf.h>


প্রকারনির্দেশ সংরচনা {
    বর্ণ                *name;
    পূর্ণাঙ্ক                  offset;
} ngx_bpf_reloc_t;

প্রকারনির্দেশ সংরচনা {
    বর্ণ                *license;
    গণনা bpf_prog_type   type;
    সংরচনা bpf_insn     *ins;
    size_t               nins;
    ngx_bpf_reloc_t     *relocs;
    size_t               nrelocs;
} ngx_bpf_program_t;


শূন্য ngx_bpf_program_link(ngx_bpf_program_t *program, ধ্রুব বর্ণ *প্রতীক,
    পূর্ণাঙ্ক fd);
পূর্ণাঙ্ক ngx_bpf_load_program(ngx_log_t *log, ngx_bpf_program_t *program);

পূর্ণাঙ্ক ngx_bpf_map_create(ngx_log_t *log, গণনা bpf_map_type type, পূর্ণাঙ্ক চাবি_size,
    পূর্ণাঙ্ক মান_size, পূর্ণাঙ্ক max_entries, uint32_t map_flags);
পূর্ণাঙ্ক ngx_bpf_map_update(পূর্ণাঙ্ক fd, ধ্রুব শূন্য *চাবি, ধ্রুব শূন্য *মান,
    uint64_t flags);
পূর্ণাঙ্ক ngx_bpf_map_delete(পূর্ণাঙ্ক fd, ধ্রুব শূন্য *চাবি);
পূর্ণাঙ্ক ngx_bpf_map_lookup(পূর্ণাঙ্ক fd, ধ্রুব শূন্য *চাবি, শূন্য *মান);

#endif /* _NGX_BPF_H_INCLUDED_ */
