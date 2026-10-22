
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


#यदि_अपरिभाषितम् _NGX_QUEUE_H_INCLUDED_
#परिभाषय _NGX_QUEUE_H_INCLUDED_


प्रकारनिर्देश संरचना ngx_queue_s  ngx_queue_t;

संरचना ngx_queue_s {
    ngx_queue_t  *पूर्व;
    ngx_queue_t  *अग्र;
};


#परिभाषय ngx_queue_init(q)                                                     \
    (q)->पूर्व = q;                                                            \
    (q)->अग्र = q


#परिभाषय ngx_queue_रिक्तानि(h)                                                    \
    (h == (h)->पूर्व)


#परिभाषय ngx_queue_निवेशय_शीर्ष(h, x)                                           \
    (x)->अग्र = (h)->अग्र;                                                    \
    (x)->अग्र->पूर्व = x;                                                      \
    (x)->पूर्व = h;                                                            \
    (h)->अग्र = x


#परिभाषय ngx_queue_निवेशय_after   ngx_queue_निवेशय_शीर्ष


#परिभाषय ngx_queue_निवेशय_पुच्छ(h, x)                                           \
    (x)->पूर्व = (h)->पूर्व;                                                    \
    (x)->पूर्व->अग्र = x;                                                      \
    (x)->अग्र = h;                                                            \
    (h)->पूर्व = x


#परिभाषय ngx_queue_निवेशय_before   ngx_queue_निवेशय_पुच्छ


#परिभाषय ngx_queue_शीर्ष(h)                                                     \
    (h)->अग्र


#परिभाषय ngx_queue_चरमः(h)                                                     \
    (h)->पूर्व


#परिभाषय ngx_queue_sentinel(h)                                                 \
    (h)


#परिभाषय ngx_queue_अग्र(q)                                                     \
    (q)->अग्र


#परिभाषय ngx_queue_पूर्व(q)                                                     \
    (q)->पूर्व


#यदि (NGX_DEBUG)

#परिभाषय ngx_queue_remove(x)                                                   \
    (x)->अग्र->पूर्व = (x)->पूर्व;                                              \
    (x)->पूर्व->अग्र = (x)->अग्र;                                              \
    (x)->पूर्व = रिक्त;                                                         \
    (x)->अग्र = रिक्त

#अन्यथा

#परिभाषय ngx_queue_remove(x)                                                   \
    (x)->अग्र->पूर्व = (x)->पूर्व;                                              \
    (x)->पूर्व->अग्र = (x)->अग्र

#यदि_अन्तः


#परिभाषय ngx_queue_विभज(h, q, n)                                              \
    (n)->पूर्व = (h)->पूर्व;                                                    \
    (n)->पूर्व->अग्र = n;                                                      \
    (n)->अग्र = q;                                                            \
    (h)->पूर्व = (q)->पूर्व;                                                    \
    (h)->पूर्व->अग्र = h;                                                      \
    (q)->पूर्व = n;


#परिभाषय ngx_queue_सङ्कलय(h, n)                                                   \
    (h)->पूर्व->अग्र = (n)->अग्र;                                              \
    (n)->अग्र->पूर्व = (h)->पूर्व;                                              \
    (h)->पूर्व = (n)->पूर्व;                                                    \
    (h)->पूर्व->अग्र = h;


#परिभाषय ngx_queue_दत्त(q, type, link)                                         \
    (type *) ((u_char *) q - offsetof(type, link))


ngx_queue_t *ngx_queue_मध्यः(ngx_queue_t *queue);
शून्य ngx_queue_क्रमय(ngx_queue_t *queue,
    ngx_int_t (*cmp)(ध्रुव ngx_queue_t *, ध्रुव ngx_queue_t *));


#यदि_अन्तः /* _NGX_QUEUE_H_INCLUDED_ */
