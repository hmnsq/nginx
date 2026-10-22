
/*
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_BPF_H_INCLUDED_
#define _NGX_BPF_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>

#include <linux/bpf.h>


typedef sa_nracanaa {
    var_na                *name;
    จำนวนเต็ม                  offset;
} ngx_bpf_reloc_t;

typedef sa_nracanaa {
    var_na                *license;
    ga_nanaa bpf_prog_type   type;
    sa_nracanaa bpf_insn     *ins;
    size_t               nins;
    ngx_bpf_reloc_t     *relocs;
    size_t               nrelocs;
} ngx_bpf_program_t;


void ngx_bpf_program_link(ngx_bpf_program_t *program, const var_na *symbol,
    จำนวนเต็ม fd);
จำนวนเต็ม ngx_bpf_load_program(ngx_log_t *log, ngx_bpf_program_t *program);

จำนวนเต็ม ngx_bpf_map_create(ngx_log_t *log, ga_nanaa bpf_map_type type, จำนวนเต็ม key_size,
    จำนวนเต็ม value_size, จำนวนเต็ม max_entries, uint32_t map_flags);
จำนวนเต็ม ngx_bpf_map_update(จำนวนเต็ม fd, const void *key, const void *value,
    uint64_t flags);
จำนวนเต็ม ngx_bpf_map_delete(จำนวนเต็ม fd, const void *key);
จำนวนเต็ม ngx_bpf_map_lookup(จำนวนเต็ม fd, const void *key, void *value);

#endif /* _NGX_BPF_H_INCLUDED_ */
