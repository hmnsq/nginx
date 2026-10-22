
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
    माप_प्रकार               nins;
    ngx_bpf_reloc_t     *relocs;
    माप_प्रकार               nrelocs;
} ngx_bpf_program_t;


शून्य ngx_bpf_program_link(ngx_bpf_program_t *program, अचर वर्ण *चिह्न,
    पूर्णाङ्क fd);
पूर्णाङ्क ngx_bpf_load_program(ngx_log_t *log, ngx_bpf_program_t *program);

पूर्णाङ्क ngx_bpf_map_create(ngx_log_t *log, गणना bpf_map_type type, पूर्णाङ्क साँचो_size,
    पूर्णाङ्क मान_size, पूर्णाङ्क max_entries, uint32_t map_flags);
पूर्णाङ्क ngx_bpf_map_update(पूर्णाङ्क fd, अचर शून्य *साँचो, अचर शून्य *मान,
    uint64_t flags);
पूर्णाङ्क ngx_bpf_map_delete(पूर्णाङ्क fd, अचर शून्य *साँचो);
पूर्णाङ्क ngx_bpf_map_lookup(पूर्णाङ्क fd, अचर शून्य *साँचो, शून्य *मान);

#endif /* _NGX_BPF_H_INCLUDED_ */
