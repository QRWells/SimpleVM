constexpr auto *FACTORIAL =
    R"(
    jmp     main // jump to main
Fac:
    dup
    push    1
    sub
    dup
    bnz     next
    pop
    ret
next:
    call    Fac
    mul
    ret
main:
    push    5
    call    Fac
    out
    halt
)";