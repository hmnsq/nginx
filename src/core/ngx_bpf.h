
/*
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_BPF_H_INCLUDED_
#define _NGX_BPF_H_INCLUDED_


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>

#അന്തര്ഭാവ <linux/bpf.h>


തരനിർവചനം ഘടന {
    അക്ഷരം                *name;
    പൂർണ്ണസംഖ്യ                  offset;
} ngx_bpf_reloc_t;

തരനിർവചനം ഘടന {
    അക്ഷരം                *license;
    എണ്ണൽ bpf_prog_type   type;
    ഘടന bpf_insn     *ins;
    size_t               nins;
    ngx_bpf_reloc_t     *relocs;
    size_t               nrelocs;
} ngx_bpf_program_t;


ശൂന്യം ngx_bpf_program_link(ngx_bpf_program_t *program, സ്ഥിരം അക്ഷരം *ചിഹ്നം,
    പൂർണ്ണസംഖ്യ fd);
പൂർണ്ണസംഖ്യ ngx_bpf_load_program(ngx_log_t *log, ngx_bpf_program_t *program);

പൂർണ്ണസംഖ്യ ngx_bpf_map_create(ngx_log_t *log, എണ്ണൽ bpf_map_type type, പൂർണ്ണസംഖ്യ താക്കോൽ_size,
    പൂർണ്ണസംഖ്യ മൂല്യം_size, പൂർണ്ണസംഖ്യ max_entries, uint32_t map_flags);
പൂർണ്ണസംഖ്യ ngx_bpf_map_update(പൂർണ്ണസംഖ്യ fd, സ്ഥിരം ശൂന്യം *താക്കോൽ, സ്ഥിരം ശൂന്യം *മൂല്യം,
    uint64_t flags);
പൂർണ്ണസംഖ്യ ngx_bpf_map_delete(പൂർണ്ണസംഖ്യ fd, സ്ഥിരം ശൂന്യം *താക്കോൽ);
പൂർണ്ണസംഖ്യ ngx_bpf_map_lookup(പൂർണ്ണസംഖ്യ fd, സ്ഥിരം ശൂന്യം *താക്കോൽ, ശൂന്യം *മൂല്യം);

#endif /* _NGX_BPF_H_INCLUDED_ */
