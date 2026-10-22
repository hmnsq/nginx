
/*
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_BPF_H_INCLUDED_
#define _NGX_BPF_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>

#ಅನ್ತರ್ಭಾವ <linux/bpf.h>


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ವರ್ಣ                *name;
    ಪೂರ್ಣಾಙ್ಕ                  offset;
} ngx_bpf_reloc_t;

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ವರ್ಣ                *license;
    ಗಣನಾ bpf_prog_type   type;
    ಸಂರಚನಾ bpf_insn     *ins;
    size_t               nins;
    ngx_bpf_reloc_t     *relocs;
    size_t               nrelocs;
} ngx_bpf_program_t;


ಶೂನ್ಯ ngx_bpf_program_link(ngx_bpf_program_t *program, ಧ್ರುವ ವರ್ಣ *ಚಿಹ್ನೆ,
    ಪೂರ್ಣಾಙ್ಕ fd);
ಪೂರ್ಣಾಙ್ಕ ngx_bpf_load_program(ngx_log_t *log, ngx_bpf_program_t *program);

ಪೂರ್ಣಾಙ್ಕ ngx_bpf_map_create(ngx_log_t *log, ಗಣನಾ bpf_map_type type, ಪೂರ್ಣಾಙ್ಕ ಕೀಲಿ_size,
    ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ_size, ಪೂರ್ಣಾಙ್ಕ max_entries, uint32_t map_flags);
ಪೂರ್ಣಾಙ್ಕ ngx_bpf_map_update(ಪೂರ್ಣಾಙ್ಕ fd, ಧ್ರುವ ಶೂನ್ಯ *ಕೀಲಿ, ಧ್ರುವ ಶೂನ್ಯ *ಮೌಲ್ಯ,
    uint64_t flags);
ಪೂರ್ಣಾಙ್ಕ ngx_bpf_map_delete(ಪೂರ್ಣಾಙ್ಕ fd, ಧ್ರುವ ಶೂನ್ಯ *ಕೀಲಿ);
ಪೂರ್ಣಾಙ್ಕ ngx_bpf_map_lookup(ಪೂರ್ಣಾಙ್ಕ fd, ಧ್ರುವ ಶೂನ್ಯ *ಕೀಲಿ, ಶೂನ್ಯ *ಮೌಲ್ಯ);

#endif /* _NGX_BPF_H_INCLUDED_ */
