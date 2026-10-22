
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
    පූර්ණ_සංඛ්‍යා                  offset;
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
    පූර්ණ_සංඛ්‍යා fd);
පූර්ණ_සංඛ්‍යා ngx_bpf_load_program(ngx_log_t *log, ngx_bpf_program_t *program);

පූර්ණ_සංඛ්‍යා ngx_bpf_map_create(ngx_log_t *log, ga_nanaa bpf_map_type type, පූර්ණ_සංඛ්‍යා key_size,
    පූර්ණ_සංඛ්‍යා value_size, පූර්ණ_සංඛ්‍යා max_entries, uint32_t map_flags);
පූර්ණ_සංඛ්‍යා ngx_bpf_map_update(පූර්ණ_සංඛ්‍යා fd, const void *key, const void *value,
    uint64_t flags);
පූර්ණ_සංඛ්‍යා ngx_bpf_map_delete(පූර්ණ_සංඛ්‍යා fd, const void *key);
පූර්ණ_සංඛ්‍යා ngx_bpf_map_lookup(පූර්ණ_සංඛ්‍යා fd, const void *key, void *value);

#endif /* _NGX_BPF_H_INCLUDED_ */
