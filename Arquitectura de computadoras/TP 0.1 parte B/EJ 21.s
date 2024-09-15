#Ej 21
.data
    frase: .string "YVAN EHT NIOJ"
.text
    la a1,frase
    jal x1,reverseStr
    li a7,10
    ecall
    nop
reverseStr:
    addi a5,x1,0
    jal a2,StrLen
    jal a2,PrintString
    ret
PrintString:
    lb a0,0(t2)
    li a7,11
    ecall
    addi t2,t2,-1
    bge t2,a1,PrintString
    jr a2
StrLen:
    lb t0,0(a1)
    addi t1,t1,1
    addi a1,a1,1
    bnez t0,StrLen
    sub a1,a1,t1
    addi t2,t1,-1
    add t2,t2,a1
    jr a2
    