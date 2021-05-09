    .section .rodata
    .global index_html
    .type   index_html, %object
    .align  4
index_html:
    .incbin "index.html"
index_html_end:
    .global index_html_size
    .type   index_html_size, %object
    .align  4
index_html_size:
    .int    index_html_end - index_html
