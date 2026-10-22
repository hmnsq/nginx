
/*
 * Copyright (C) Nginx, Inc.
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>

#घोषणा NGX_BPF_LOGBUF_आकार  (32 * 1024)


अटल ngx_inline पूर्णांक
ngx_bpf(क्रमागत bpf_cmd cmd, जोड़ bpf_attr *attr, अचिन्हित पूर्णांक आकार)
{
    वापस syscall(__NR_bpf, cmd, attr, आकार);
}


व्योम
ngx_bpf_program_link(ngx_bpf_program_t *program, स्थिर अक्षर *प्रतीक, पूर्णांक fd)
{
    ngx_uint_t        i;
    ngx_bpf_reloc_t  *rl;

    rl = program->relocs;

    क्रम (i = 0; i < program->nrelocs; i++) {
        अगर (ngx_strcmp(rl[i].name, प्रतीक) == 0) {
            program->ins[rl[i].offset].src_reg = 1;
            program->ins[rl[i].offset].imm = fd;
        }
    }
}


पूर्णांक
ngx_bpf_load_program(ngx_log_t *log, ngx_bpf_program_t *program)
{
    पूर्णांक             fd;
    जोड़ bpf_attr  attr;
#अगर (NGX_DEBUG)
    अक्षर            buf[NGX_BPF_LOGBUF_आकार];
#पूर्ण_अगर

    ngx_memzero(&attr, माप(जोड़ bpf_attr));

    attr.license = (uintptr_t) program->license;
    attr.prog_type = program->type;
    attr.insns = (uintptr_t) program->ins;
    attr.insn_cnt = program->nins;

#अगर (NGX_DEBUG)
    /* for verifier errors */
    attr.log_buf = (uintptr_t) buf;
    attr.log_आकार = NGX_BPF_LOGBUF_आकार;
    attr.log_level = 1;
#पूर्ण_अगर

    fd = ngx_bpf(BPF_PROG_LOAD, &attr, माप(attr));
    अगर (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      "failed to load BPF program");

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0,
                       "bpf verifier: %s", buf);

        वापस -1;
    }

    वापस fd;
}


पूर्णांक
ngx_bpf_map_create(ngx_log_t *log, क्रमागत bpf_map_type type, पूर्णांक कुंजी_आकार,
    पूर्णांक मूल्य_आकार, पूर्णांक max_entries, uint32_t map_flags)
{
    पूर्णांक             fd;
    जोड़ bpf_attr  attr;

    ngx_memzero(&attr, माप(जोड़ bpf_attr));

    attr.map_type = type;
    attr.कुंजी_आकार = कुंजी_आकार;
    attr.मूल्य_आकार = मूल्य_आकार;
    attr.max_entries = max_entries;
    attr.map_flags = map_flags;

    fd = ngx_bpf(BPF_MAP_CREATE, &attr, माप(attr));
    अगर (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      "failed to create BPF map");
        वापस NGX_ERROR;
    }

    वापस fd;
}


पूर्णांक
ngx_bpf_map_update(पूर्णांक fd, स्थिर व्योम *कुंजी, स्थिर व्योम *मूल्य, uint64_t flags)
{
    जोड़ bpf_attr attr;

    ngx_memzero(&attr, माप(जोड़ bpf_attr));

    attr.map_fd = fd;
    attr.कुंजी = (uintptr_t) कुंजी;
    attr.मूल्य = (uintptr_t) मूल्य;
    attr.flags = flags;

    वापस ngx_bpf(BPF_MAP_UPDATE_ELEM, &attr, माप(attr));
}


पूर्णांक
ngx_bpf_map_delete(पूर्णांक fd, स्थिर व्योम *कुंजी)
{
    जोड़ bpf_attr attr;

    ngx_memzero(&attr, माप(जोड़ bpf_attr));

    attr.map_fd = fd;
    attr.कुंजी = (uintptr_t) कुंजी;

    वापस ngx_bpf(BPF_MAP_DELETE_ELEM, &attr, माप(attr));
}


पूर्णांक
ngx_bpf_map_lookup(पूर्णांक fd, स्थिर व्योम *कुंजी, व्योम *मूल्य)
{
    जोड़ bpf_attr attr;

    ngx_memzero(&attr, माप(जोड़ bpf_attr));

    attr.map_fd = fd;
    attr.कुंजी = (uintptr_t) कुंजी;
    attr.मूल्य = (uintptr_t) मूल्य;

    वापस ngx_bpf(BPF_MAP_LOOKUP_ELEM, &attr, माप(attr));
}
