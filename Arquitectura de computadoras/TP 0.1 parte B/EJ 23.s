.data 
    palabra: .string "una"
    frase: .string "Esto es una frase de prueba"
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
    lb t1,0(a3)
    lb t2,0(a2)
    beqz t1,end_cmp
    beqz t2,end_cmp
    bne t1,t2,not_equal
    addi a2,a2,1
    addi a3,a3,1
    j strCmp
end_cmp:
    li a0,1
    ret
not_equal:
    jr s0
return:
    la a0,0
    ret