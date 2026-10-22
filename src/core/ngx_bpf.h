
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
    ціле                  offset;
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
    ціле fd);
ціле ngx_bpf_load_program(ngx_log_t *log, ngx_bpf_program_t *program);

ціле ngx_bpf_map_create(ngx_log_t *log, enum bpf_map_type type, ціле key_size,
    ціле value_size, ціле max_entries, uint32_t map_flags);
ціле ngx_bpf_map_update(ціле fd, const void *key, const void *value,
    uint64_t flags);
ціле ngx_bpf_map_delete(ціле fd, const void *key);
ціле ngx_bpf_map_lookup(ціле fd, const void *key, void *value);

#endif /* _NGX_BPF_H_INCLUDED_ */
