.data 
    palabra: .string "una"
    frase: .string "Esto es UNA frase de prueba"
    palabra2: .string "no"
.text
    la a2,palabra
    la a3, frase
    jal contiene
    li a7,1
    ecall
    la a2,palabra2
    la a3,frase
    jal contiene
    ecall
    li a7,10
    ecall
    nop
contiene:
    addi a6,x1,0
    jal s0, strCmp
    addi a3, a3, 4
    beq a3,a2, return
strCmp:
    lb a4,0(a3)
    lb a5,0(a2)
    beqz a4,end_cmp
    beqz a5,end_cmp
#    bne a4,a5,not_equal
    jal ra, toLower
    addi a2,a2,1
    addi a3,a3,1
    j strCmp
toLower:
    li t0,0x20
    li t5,0x5A
    li t6,0x41
    li a1,0x6c
    bge a4,a5,end_toLower
    blt a4,t6,end_toLower
    or a4,a4,t0
    li a1,0x7c
    bge a4,t5,end_toLower
    blt a5,t6,end_toLower
    or a5,a5,t0
    bne a4,a5,not_equal
    ret
end_toLower:
    ret
end_cmp:
    li a0,0
    jalr a6
not_equal:
    li a0,-1
    jalr a1
return:
    la a0,0
    ret