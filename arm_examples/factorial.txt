factorial:
        stp     x29, x30, [sp, -32]!
        mov     x29, sp
        str     w0, [sp, 28]
        ldr     w0, [sp, 28]
        cmp     w0, 1
        bgt     .L2
        mov     w0, 1
        b       .L3
.L2:
        ldr     w0, [sp, 28]
        sub     w0, w0, #1
        bl      factorial
        mov     w1, w0
        ldr     w0, [sp, 28]
        mul     w0, w1, w0
.L3:
        ldp     x29, x30, [sp], 32
        ret