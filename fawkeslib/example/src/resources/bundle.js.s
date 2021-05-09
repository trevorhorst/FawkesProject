    .section .rodata
    .global bundle_js
    .type   bundle_js, %object
    .align  4
bundle_js:
    .incbin "bundle.js"
bundle_js_end:
    .global bundle_js_size
    .type   bundle_js_size, %object
    .align  4
bundle_js_size:
    .int    bundle_js_end - bundle_js
