.data 
    numero: .string "6234"
    numero2: .string "65"
.text
    la a0,numero
    jal atoi
    #ecall
    #li a7,10
    #ecall
    nop
StrLen:
    lb t0,0(a0)
    addi t1,t1,1
    addi a0,a0,1
    bnez t0,StrLen
    sub a1,a1,t1
    addi a2,t1,-1
    add a0,a1,a0
    ret
atoi:
    li t4,0
    li t5,0
    li t6,0
    addi a6,x1,0
    jal StrLen
    mv t0,a2
    li t1,0
    li t2,4
   # bne t0,t2,handle_hundreds
    lb t3,0(a0)
    addi t3,t3,-48
    slli t4,t3,10
    slli t5,t3,3
    slli t6,t3,4
    add t5,t5,t6
    sub t4,t4,t5
    ret