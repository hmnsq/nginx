
/*
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_BPF_H_INCLUDED_
#define _NGX_BPF_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>

#include <linux/bpf.h>


typedef struct {
    char                *name;
    số_nguyên                  offset;
} ngx_bpf_reloc_t;

typedef struct {
    char                *license;
    enum bpf_prog_type   type;
    struct bpf_insn     *ins;
    size_t               nins;
    ngx_bpf_reloc_t     *relocs;
    size_t               nrelocs;
} ngx_bpf_program_t;


void ngx_bpf_program_link(ngx_bpf_program_t *program, const char *symbol,
    số_nguyên fd);
số_nguyên ngx_bpf_load_program(ngx_log_t *log, ngx_bpf_program_t *program);

số_nguyên ngx_bpf_map_create(ngx_log_t *log, enum bpf_map_type type, số_nguyên key_size,
    số_nguyên value_size, số_nguyên max_entries, uint32_t map_flags);
số_nguyên ngx_bpf_map_update(số_nguyên fd, const void *key, const void *value,
    uint64_t flags);
số_nguyên ngx_bpf_map_delete(số_nguyên fd, const void *key);
số_nguyên ngx_bpf_map_lookup(số_nguyên fd, const void *key, void *value);

#endif /* _NGX_BPF_H_INCLUDED_ */
