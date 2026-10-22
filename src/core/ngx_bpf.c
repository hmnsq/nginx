
/*
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>

#परिभाषय NGX_BPF_LOGBUF_आकार  (32 * 1024)


स्थिर ngx_inline पूर्णाङ्क
ngx_bpf(गणना bpf_cmd cmd, सङ्घ bpf_attr *attr, अचिह्नित पूर्णाङ्क आकार)
{
    प्रत्यागम syscall(__NR_bpf, cmd, attr, आकार);
}


शून्य
ngx_bpf_क्रमादेशः_link(ngx_bpf_क्रमादेशः_t *क्रमादेशः, ध्रुव वर्ण *चिह्नम्, पूर्णाङ्क fd)
{
    ngx_uint_t        i;
    ngx_bpf_reloc_t  *rl;

    rl = क्रमादेशः->relocs;

    प्रति (i = 0; i < क्रमादेशः->nrelocs; i++) {
        यदि (ngx_strcmp(rl[i].name, चिह्नम्) == 0) {
            क्रमादेशः->ins[rl[i].विस्थापनम्].src_reg = 1;
            क्रमादेशः->ins[rl[i].विस्थापनम्].imm = fd;
        }
    }
}


पूर्णाङ्क
ngx_bpf_आरोपय_क्रमादेशः(ngx_log_t *लघुगणकः, ngx_bpf_क्रमादेशः_t *क्रमादेशः)
{
    पूर्णाङ्क             fd;
    सङ्घ bpf_attr  attr;
#यदि (NGX_DEBUG)
    वर्ण            buf[NGX_BPF_LOGBUF_आकार];
#यदि_अन्तः

    ngx_memzero(&attr, परिमाण(सङ्घ bpf_attr));

    attr.license = (uintptr_t) क्रमादेशः->license;
    attr.prog_type = क्रमादेशः->type;
    attr.insns = (uintptr_t) क्रमादेशः->ins;
    attr.insn_cnt = क्रमादेशः->nins;

#यदि (NGX_DEBUG)
    /* for verifier errors */
    attr.log_buf = (uintptr_t) buf;
    attr.log_आकार = NGX_BPF_LOGBUF_आकार;
    attr.log_level = 1;
#यदि_अन्तः

    fd = ngx_bpf(BPF_PROG_आरोपय, &attr, परिमाण(attr));
    यदि (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, लघुगणकः, ngx_errno,
                      "failed to load BPF program");

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, लघुगणकः, 0,
                       "bpf verifier: %s", buf);

        प्रत्यागम -1;
    }

    प्रत्यागम fd;
}


पूर्णाङ्क
ngx_bpf_map_create(ngx_log_t *लघुगणकः, गणना bpf_map_type type, पूर्णाङ्क कुञ्चिका_आकार,
    पूर्णाङ्क मानम्_आकार, पूर्णाङ्क max_entries, uint32_t map_flags)
{
    पूर्णाङ्क             fd;
    सङ्घ bpf_attr  attr;

    ngx_memzero(&attr, परिमाण(सङ्घ bpf_attr));

    attr.map_type = type;
    attr.कुञ्चिका_आकार = कुञ्चिका_आकार;
    attr.मानम्_आकार = मानम्_आकार;
    attr.max_entries = max_entries;
    attr.map_flags = map_flags;

    fd = ngx_bpf(BPF_MAP_CREATE, &attr, परिमाण(attr));
    यदि (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, लघुगणकः, ngx_errno,
                      "failed to create BPF map");
        प्रत्यागम NGX_ERROR;
    }

    प्रत्यागम fd;
}


पूर्णाङ्क
ngx_bpf_map_update(पूर्णाङ्क fd, ध्रुव शून्य *कुञ्चिका, ध्रुव शून्य *मानम्, uint64_t flags)
{
    सङ्घ bpf_attr attr;

    ngx_memzero(&attr, परिमाण(सङ्घ bpf_attr));

    attr.map_fd = fd;
    attr.कुञ्चिका = (uintptr_t) कुञ्चिका;
    attr.मानम् = (uintptr_t) मानम्;
    attr.flags = flags;

    प्रत्यागम ngx_bpf(BPF_MAP_UPDATE_ELEM, &attr, परिमाण(attr));
}


पूर्णाङ्क
ngx_bpf_map_delete(पूर्णाङ्क fd, ध्रुव शून्य *कुञ्चिका)
{
    सङ्घ bpf_attr attr;

    ngx_memzero(&attr, परिमाण(सङ्घ bpf_attr));

    attr.map_fd = fd;
    attr.कुञ्चिका = (uintptr_t) कुञ्चिका;

    प्रत्यागम ngx_bpf(BPF_MAP_DELETE_ELEM, &attr, परिमाण(attr));
}


पूर्णाङ्क
ngx_bpf_map_lookup(पूर्णाङ्क fd, ध्रुव शून्य *कुञ्चिका, शून्य *मानम्)
{
    सङ्घ bpf_attr attr;

    ngx_memzero(&attr, परिमाण(सङ्घ bpf_attr));

    attr.map_fd = fd;
    attr.कुञ्चिका = (uintptr_t) कुञ्चिका;
    attr.मानम् = (uintptr_t) मानम्;

    प्रत्यागम ngx_bpf(BPF_MAP_LOOKUP_ELEM, &attr, परिमाण(attr));
}
