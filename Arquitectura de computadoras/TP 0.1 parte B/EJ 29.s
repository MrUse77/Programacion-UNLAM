.data 
    numero: .string "6234"
    numero2: .string "65"
.text
    li a7,1
    la a1,numero
    jal atoi
    ecall
    li a7,11
    li a0,0x20
    ecall
    la a1,numero2
    jal atoi
    li a7,1
    ecall
    li a7,10
    ecall
    nop
StrLen:
    lb t0,0(a1)
    addi t1,t1,1
    addi a1,a1,1
    bnez t0,StrLen
    addi a3,t1,-1
    mv a1,a2
    ret 
atoi:
    li t4,0
    li t5,0
    li t6,0
    addi a6,x1,0
    mv a2,a1
    jal StrLen
    mv t0,a3
    li t1,0
    li t2,4
    bne t0,t2,handle_hundreds
    lb t3,0(a1)
    addi t3,t3,-48
    slli t4,t3,10
    slli t5,t3,3
    slli t6,t3,4
    add t5,t5,t6
    sub t1,t4,t5
    addi a1,a1,1
    addi t0,t0,-1
handle_hundreds:
    li t2,3
    bne t0,t2,handle_tens
    lb t3,0(a1)
    addi t3,t3,-48
    li t4,0
    slli t4,t3,6
    slli t5,t3,5
    slli t6,t3,2
    add t5,t5,t6
    add t4,t4,t5
    add t1,t1,t4
    addi a1,a1,1
    addi t0,t0,-1
handle_tens:
    li t2,2
    bne t0,t2,handle_units
    lb t3,0(a1)
    addi t3,t3,-48
    li t4,0
    slli t4,t3,3
    slli t5,t3,1
    add t4,t4,t5
    add t1,t1,t4
    addi a1,a1,1
    addi t0,t0,-1
handle_units:
    li t2,1
    bne t0,t2,end_atoi
    lb t3,0(a1)
    addi t3,t3,-48
    add t1,t1,t3
end_atoi:
    mv a0,t1
    mv t1,x0
    mv ra,a6
    ret
    
