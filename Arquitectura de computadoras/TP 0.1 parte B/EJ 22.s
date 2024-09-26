.data 
    cadena1: .string "Hola Mundo!"
    cadena2: .string "Inserte cadena de texto"
    cadena3: .string "Hola Mundo!"
    cadena4: .string "Hola Mundo! mas largo"
.text
    li a7,1
    la a4,cadena1
    la a5,cadena2
    jal strCmp
    ecall
    la a5,cadena3
    jal strCmp
    ecall
    la a5,cadena4
    jal strCmp
    ecall
    li a7,10
    ecall
    nop
strCmp:
    li t0,0
compare_loop:
    lb t1,0(a4)
    lb t2,0(a5)
    beqz t1,end_cmp
    beqz t2,end_cmp
    bne t1,t2,not_equal
    addi a4,a4,1
    addi a5,a5,1
    j compare_loop
end_cmp:
    li a0,0
    ret
not_equal:
    li a0,-1
    ret