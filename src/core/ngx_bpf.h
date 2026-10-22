
/*
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_BPF_H_INCLUDED_
#define _NGX_BPF_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>

#अन्तर्भाव <linux/bpf.h>


प्रकारनिर्देश संरचना {
    वर्ण                *name;
    पूर्णाङ्क                  offset;
} ngx_bpf_reloc_t;

प्रकारनिर्देश संरचना {
    वर्ण                *license;
    गणना bpf_prog_type   type;
    संरचना bpf_insn     *ins;
    size_t               nins;
    ngx_bpf_reloc_t     *relocs;
    size_t               nrelocs;
} ngx_bpf_program_t;


शून्य ngx_bpf_program_link(ngx_bpf_program_t *program, ध्रुव वर्ण *चिन्ह,
    पूर्णाङ्क fd);
पूर्णाङ्क ngx_bpf_load_program(ngx_log_t *log, ngx_bpf_program_t *program);

पूर्णाङ्क ngx_bpf_map_create(ngx_log_t *log, गणना bpf_map_type type, पूर्णाङ्क किल्ली_size,
    पूर्णाङ्क मूल्य_size, पूर्णाङ्क max_entries, uint32_t map_flags);
पूर्णाङ्क ngx_bpf_map_update(पूर्णाङ्क fd, ध्रुव शून्य *किल्ली, ध्रुव शून्य *मूल्य,
    uint64_t flags);
पूर्णाङ्क ngx_bpf_map_delete(पूर्णाङ्क fd, ध्रुव शून्य *किल्ली);
पूर्णाङ्क ngx_bpf_map_lookup(पूर्णाङ्क fd, ध्रुव शून्य *किल्ली, शून्य *मूल्य);

#endif /* _NGX_BPF_H_INCLUDED_ */
