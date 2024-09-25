.data 
    texto1: .string "UnA"
    texto2: .string "una frase"
.text
    li a7,1
    la a4,texto1
    la a5,texto2
    jal strCmpi
    ecall
    li a7,10
    ecall
    nop
strCmpi:
    addi a6,x1,0
compare_loop:
    lb a1,0(a4)
    lb a2,0(a5)
    beqz a1,end_cmp
    beqz a2,end_cmp
    jal ra, toLower
    addi a4,a4,1
    addi a5,a5,1
    j compare_loop
equal:
    li a0,0
    ret
toLower:
    li t0,0x20
    li t5,0x5A
    li t6,0x41
    li a3,0x6c
    bge a1,t5,end_toLower
    blt a1,t6,end_toLower
    or a1,a1,t0
    li a3,0x7c
    bge a2,t5,end_toLower
    blt a2,t6,end_toLower
    or a2,a2,t0
    bne a1,a2,not_equal
    ret
end_toLower:
    jalr x0,a3,0
end_cmp:
    li a0,0
    jalr a6
not_equal:
    li a0,-1
    jalr a6