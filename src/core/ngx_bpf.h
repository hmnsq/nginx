
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
    ចំនួនគត់                  offset;
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
    ចំនួនគត់ fd);
ចំនួនគត់ ngx_bpf_load_program(ngx_log_t *log, ngx_bpf_program_t *program);

ចំនួនគត់ ngx_bpf_map_create(ngx_log_t *log, ga_nanaa bpf_map_type type, ចំនួនគត់ key_size,
    ចំនួនគត់ value_size, ចំនួនគត់ max_entries, uint32_t map_flags);
ចំនួនគត់ ngx_bpf_map_update(ចំនួនគត់ fd, const void *key, const void *value,
    uint64_t flags);
ចំនួនគត់ ngx_bpf_map_delete(ចំនួនគត់ fd, const void *key);
ចំនួនគត់ ngx_bpf_map_lookup(ចំនួនគត់ fd, const void *key, void *value);

#endif /* _NGX_BPF_H_INCLUDED_ */
