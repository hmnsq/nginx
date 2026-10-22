
/*
 * Copyright (C) Nginx, Inc.
 */


#अगर_अघोषित _NGX_BPF_H_INCLUDED_
#घोषणा _NGX_BPF_H_INCLUDED_


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>

#समावेश <linux/bpf.h>


प्रकार काष्ठा {
    अक्षर                *name;
    पूर्णांक                  offset;
} ngx_bpf_reloc_t;

प्रकार काष्ठा {
    अक्षर                *license;
    क्रमागत bpf_prog_type   type;
    काष्ठा bpf_insn     *ins;
    माप_प्रकार               nins;
    ngx_bpf_reloc_t     *relocs;
    माप_प्रकार               nrelocs;
} ngx_bpf_program_t;


व्योम ngx_bpf_program_link(ngx_bpf_program_t *program, स्थिर अक्षर *प्रतीक,
    पूर्णांक fd);
पूर्णांक ngx_bpf_load_program(ngx_log_t *log, ngx_bpf_program_t *program);

पूर्णांक ngx_bpf_map_create(ngx_log_t *log, क्रमागत bpf_map_type type, पूर्णांक कुंजी_आकार,
    पूर्णांक मूल्य_आकार, पूर्णांक max_entries, uint32_t map_flags);
पूर्णांक ngx_bpf_map_update(पूर्णांक fd, स्थिर व्योम *कुंजी, स्थिर व्योम *मूल्य,
    uint64_t flags);
पूर्णांक ngx_bpf_map_delete(पूर्णांक fd, स्थिर व्योम *कुंजी);
पूर्णांक ngx_bpf_map_lookup(पूर्णांक fd, स्थिर व्योम *कुंजी, व्योम *मूल्य);

#पूर्ण_अगर /* _NGX_BPF_H_INCLUDED_ */
