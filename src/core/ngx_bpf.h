
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
    цео                  offset;
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
    цео fd);
цео ngx_bpf_load_program(ngx_log_t *log, ngx_bpf_program_t *program);

цео ngx_bpf_map_create(ngx_log_t *log, enum bpf_map_type type, цео key_size,
    цео value_size, цео max_entries, uint32_t map_flags);
цео ngx_bpf_map_update(цео fd, const void *key, const void *value,
    uint64_t flags);
цео ngx_bpf_map_delete(цео fd, const void *key);
цео ngx_bpf_map_lookup(цео fd, const void *key, void *value);

#endif /* _NGX_BPF_H_INCLUDED_ */
