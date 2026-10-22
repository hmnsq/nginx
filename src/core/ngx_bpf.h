
/*
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_BPF_H_INCLUDED_
#define _NGX_BPF_H_INCLUDED_


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>

#અન્તર્ભાવ <linux/bpf.h>


પ્રકારવ્યાખ્યા સંરચના {
    અક્ષર                *name;
    પૂર્ણાંક                  offset;
} ngx_bpf_reloc_t;

પ્રકારવ્યાખ્યા સંરચના {
    અક્ષર                *license;
    ગણના bpf_prog_type   type;
    સંરચના bpf_insn     *ins;
    size_t               nins;
    ngx_bpf_reloc_t     *relocs;
    size_t               nrelocs;
} ngx_bpf_program_t;


શૂન્ય ngx_bpf_program_link(ngx_bpf_program_t *program, અચલ અક્ષર *પ્રતીક,
    પૂર્ણાંક fd);
પૂર્ણાંક ngx_bpf_load_program(ngx_log_t *log, ngx_bpf_program_t *program);

પૂર્ણાંક ngx_bpf_map_create(ngx_log_t *log, ગણના bpf_map_type type, પૂર્ણાંક ચાવી_size,
    પૂર્ણાંક કિંમત_size, પૂર્ણાંક max_entries, uint32_t map_flags);
પૂર્ણાંક ngx_bpf_map_update(પૂર્ણાંક fd, અચલ શૂન્ય *ચાવી, અચલ શૂન્ય *કિંમત,
    uint64_t flags);
પૂર્ણાંક ngx_bpf_map_delete(પૂર્ણાંક fd, અચલ શૂન્ય *ચાવી);
પૂર્ણાંક ngx_bpf_map_lookup(પૂર્ણાંક fd, અચલ શૂન્ય *ચાવી, શૂન્ય *કિંમત);

#endif /* _NGX_BPF_H_INCLUDED_ */
