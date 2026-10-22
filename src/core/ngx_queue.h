
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


#अगर_अघोषित _NGX_QUEUE_H_INCLUDED_
#घोषणा _NGX_QUEUE_H_INCLUDED_


प्रकार काष्ठा ngx_queue_s  ngx_queue_t;

काष्ठा ngx_queue_s {
    ngx_queue_t  *पूर्व;
    ngx_queue_t  *अग्र;
};


#घोषणा ngx_queue_init(q)                                                     \
    (q)->पूर्व = q;                                                            \
    (q)->अग्र = q


#घोषणा ngx_queue_empty(h)                                                    \
    (h == (h)->पूर्व)


#घोषणा ngx_queue_जोड़ो_शीर्ष(h, x)                                           \
    (x)->अग्र = (h)->अग्र;                                                    \
    (x)->अग्र->पूर्व = x;                                                      \
    (x)->पूर्व = h;                                                            \
    (h)->अग्र = x


#घोषणा ngx_queue_जोड़ो_after   ngx_queue_जोड़ो_शीर्ष


#घोषणा ngx_queue_जोड़ो_पुच्छ(h, x)                                           \
    (x)->पूर्व = (h)->पूर्व;                                                    \
    (x)->पूर्व->अग्र = x;                                                      \
    (x)->अग्र = h;                                                            \
    (h)->पूर्व = x


#घोषणा ngx_queue_जोड़ो_before   ngx_queue_जोड़ो_पुच्छ


#घोषणा ngx_queue_शीर्ष(h)                                                     \
    (h)->अग्र


#घोषणा ngx_queue_last(h)                                                     \
    (h)->पूर्व


#घोषणा ngx_queue_sentinel(h)                                                 \
    (h)


#घोषणा ngx_queue_अग्र(q)                                                     \
    (q)->अग्र


#घोषणा ngx_queue_पूर्व(q)                                                     \
    (q)->पूर्व


#अगर (NGX_DEBUG)

#घोषणा ngx_queue_remove(x)                                                   \
    (x)->अग्र->पूर्व = (x)->पूर्व;                                              \
    (x)->पूर्व->अग्र = (x)->अग्र;                                              \
    (x)->पूर्व = शून्य;                                                         \
    (x)->अग्र = शून्य

#अन्यथा

#घोषणा ngx_queue_remove(x)                                                   \
    (x)->अग्र->पूर्व = (x)->पूर्व;                                              \
    (x)->पूर्व->अग्र = (x)->अग्र

#पूर्ण_अगर


#घोषणा ngx_queue_split(h, q, n)                                              \
    (n)->पूर्व = (h)->पूर्व;                                                    \
    (n)->पूर्व->अग्र = n;                                                      \
    (n)->अग्र = q;                                                            \
    (h)->पूर्व = (q)->पूर्व;                                                    \
    (h)->पूर्व->अग्र = h;                                                      \
    (q)->पूर्व = n;


#घोषणा ngx_queue_add(h, n)                                                   \
    (h)->पूर्व->अग्र = (n)->अग्र;                                              \
    (n)->अग्र->पूर्व = (h)->पूर्व;                                              \
    (h)->पूर्व = (n)->पूर्व;                                                    \
    (h)->पूर्व->अग्र = h;


#घोषणा ngx_queue_तथ्य(q, type, link)                                         \
    (type *) ((u_char *) q - दुरत्व(type, link))


ngx_queue_t *ngx_queue_मध्य(ngx_queue_t *queue);
व्योम ngx_queue_sort(ngx_queue_t *queue,
    ngx_int_t (*cmp)(स्थिर ngx_queue_t *, स्थिर ngx_queue_t *));


#पूर्ण_अगर /* _NGX_QUEUE_H_INCLUDED_ */
