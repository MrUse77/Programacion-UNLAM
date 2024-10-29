.data
    datos: .word 4,7,10,100,24,56,12,0
    fdatos:
.text
    la a0,datos
    la a1,fdatos
    jal empaquetar
    la a0,datos
    la a1,fdatos
    jal imprimir_datos
fin:
    beq x0,x0,fin
empaquetar:
    mv t1,a0
    li t2,0
loop:
    lw t0,0(a0)
    sw t2,0(a0)
    sb t0,0(t1)
    
    beq a0,a1,finLoop
    addi a0,a0,4
    addi t1,t1,1
    j loop
finLoop:
    ret
imprimir_datos:
    mv t1,a0
loopImprimir:
    lw a0,0(t1)
    li a7,1
    ecall
    li a7,11
    li a0,0x2c
    ecall
    beq t1,a1,finLoop
    addi t1,t1,4
    j loopImprimir