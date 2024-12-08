#mismo que el 33 solo que buscar el minimo
.data
    vector: .word 100,-10, 20,1000,66500,1000000,-20,34,56
    finvec:
.text
    la a1,vector
    la a2,finvec
    jal buscaMin
    li a7,1
    ecall
    li a7,10
    ecall
    nop
buscaMin:
    mv t1,a1
loop:
    lw t0,0(t1)
    blt t0,s0,guardar
    addi t1,t1,4
    bne t1,a2,loop
    j fin
guardar:
    mv s0,t0
    j loop
fin:
    mv a0,s0
    ret