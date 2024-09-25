.data 
    numero: .string "1234"
    numero2: .string "0657"
    decena: .byte 0,10,20,30,40,50,60,70,80,90
    centena: .half 0,100,200,300,400,500,600,700,800,900
    miles: .half 0,1000,2000,3000,4000,5000,6000,7000,8000,9000
.text
    li a7,1
    la a1,numero
    jal atoi4
    ecall
    li a7,11
    li a0,10
    ecall
    li a7,1
    la a1,numero2
    jal atoi4
    ecall
    li a7,10
    ecall
    nop
atoi4:
    li t3,-48
    #miles
    la t2,miles
    lb t0,0(a1)
    add t0,t0,t3
    slli t0,t0,1
    add t2,t2,t0
    lhu t5,0(t2)
    #centenas
    la t2,centena
    lb t0,1(a1)
    add t0,t0,t3
    slli t0,t0,1
    add t2,t2,t0
    lhu t1,0(t2)
    #decenas
    la t2,decena
    lb t0,2(a1)
    add t0,t0,t3
    add t2,t2,t0
    lbu t2,0(t2)
    #unidades
    lb t0,3(a1)
    add t0,t0,t3
    add t0,t0,t1
    add t0,t0,t2
    add a0,t0,t5
    ret
    