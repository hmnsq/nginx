
/*
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_BPF_H_INCLUDED_
#define _NGX_BPF_H_INCLUDED_


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>

#அந்தர்பாவ <linux/bpf.h>


வகைவரை கட்டமைப்பு {
    எழுத்து                *name;
    முழு                  offset;
} ngx_bpf_reloc_t;

வகைவரை கட்டமைப்பு {
    எழுத்து                *license;
    எண்ணிக்கை bpf_prog_type   type;
    கட்டமைப்பு bpf_insn     *ins;
    size_t               nins;
    ngx_bpf_reloc_t     *relocs;
    size_t               nrelocs;
} ngx_bpf_program_t;


வெறுமை ngx_bpf_program_link(ngx_bpf_program_t *program, மாறிலி எழுத்து *குறியீடு,
    முழு fd);
முழு ngx_bpf_load_program(ngx_log_t *log, ngx_bpf_program_t *program);

முழு ngx_bpf_map_create(ngx_log_t *log, எண்ணிக்கை bpf_map_type type, முழு சாவி_size,
    முழு மதிப்பு_size, முழு max_entries, uint32_t map_flags);
முழு ngx_bpf_map_update(முழு fd, மாறிலி வெறுமை *சாவி, மாறிலி வெறுமை *மதிப்பு,
    uint64_t flags);
முழு ngx_bpf_map_delete(முழு fd, மாறிலி வெறுமை *சாவி);
முழு ngx_bpf_map_lookup(முழு fd, மாறிலி வெறுமை *சாவி, வெறுமை *மதிப்பு);

#endif /* _NGX_BPF_H_INCLUDED_ */
