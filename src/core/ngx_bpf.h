
/*
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_BPF_H_INCLUDED_
#define _NGX_BPF_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>

#अन्तर्भाव <linux/bpf.h>


प्रकारव्याख्या संरचना {
    अक्षर                *name;
    पूर्णांक                  offset;
} ngx_bpf_reloc_t;

प्रकारव्याख्या संरचना {
    अक्षर                *license;
    गणना bpf_prog_type   type;
    संरचना bpf_insn     *ins;
    माप_प्रकार               nins;
    ngx_bpf_reloc_t     *relocs;
    माप_प्रकार               nrelocs;
} ngx_bpf_program_t;


शून्य ngx_bpf_program_link(ngx_bpf_program_t *program, स्थिरांक अक्षर *चिन्ह,
    पूर्णांक fd);
पूर्णांक ngx_bpf_load_program(ngx_log_t *log, ngx_bpf_program_t *program);

पूर्णांक ngx_bpf_map_create(ngx_log_t *log, गणना bpf_map_type type, पूर्णांक किल्ली_size,
    पूर्णांक मूल्य_size, पूर्णांक max_entries, uint32_t map_flags);
पूर्णांक ngx_bpf_map_update(पूर्णांक fd, स्थिरांक शून्य *किल्ली, स्थिरांक शून्य *मूल्य,
    uint64_t flags);
पूर्णांक ngx_bpf_map_delete(पूर्णांक fd, स्थिरांक शून्य *किल्ली);
पूर्णांक ngx_bpf_map_lookup(पूर्णांक fd, स्थिरांक शून्य *किल्ली, शून्य *मूल्य);

#endif /* _NGX_BPF_H_INCLUDED_ */
