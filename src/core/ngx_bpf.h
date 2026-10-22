
/*
 * Copyright (C) Nginx, Inc.
 */


#यदि_अपरिभाषितम् _NGX_BPF_H_INCLUDED_
#परिभाषय _NGX_BPF_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>

#अन्तर्भाव <linux/bpf.h>


प्रकारनिर्देश संरचना {
    वर्ण                *name;
    पूर्णाङ्क                  विस्थापनम्;
} ngx_bpf_reloc_t;

प्रकारनिर्देश संरचना {
    वर्ण                *license;
    गणना bpf_prog_type   type;
    संरचना bpf_insn     *ins;
    आकार_t               nins;
    ngx_bpf_reloc_t     *relocs;
    आकार_t               nrelocs;
} ngx_bpf_क्रमादेशः_t;


शून्य ngx_bpf_क्रमादेशः_link(ngx_bpf_क्रमादेशः_t *क्रमादेशः, ध्रुव वर्ण *चिह्नम्,
    पूर्णाङ्क fd);
पूर्णाङ्क ngx_bpf_आरोपय_क्रमादेशः(ngx_log_t *लघुगणकः, ngx_bpf_क्रमादेशः_t *क्रमादेशः);

पूर्णाङ्क ngx_bpf_map_create(ngx_log_t *लघुगणकः, गणना bpf_map_type type, पूर्णाङ्क कुञ्चिका_आकार,
    पूर्णाङ्क मानम्_आकार, पूर्णाङ्क max_entries, uint32_t map_flags);
पूर्णाङ्क ngx_bpf_map_update(पूर्णाङ्क fd, ध्रुव शून्य *कुञ्चिका, ध्रुव शून्य *मानम्,
    uint64_t flags);
पूर्णाङ्क ngx_bpf_map_delete(पूर्णाङ्क fd, ध्रुव शून्य *कुञ्चिका);
पूर्णाङ्क ngx_bpf_map_lookup(पूर्णाङ्क fd, ध्रुव शून्य *कुञ्चिका, शून्य *मानम्);

#यदि_अन्तः /* _NGX_BPF_H_INCLUDED_ */
