.data
    textoMax: .string "El maximo es: "
    textoCan: .string " ,Veces que aparece:"
    vectorUnsigned: .half 100,32799,20,1000,6502,12345,65000,41234,56,65535,20,65535,65535
    vectorPos: .byte 1,2,3,5,7,10
    finPos:
.text
    #Cargo vectores como argumentos
    la a3,vectorUnsigned
    la a4,vectorPos
    la a5,finPos
    
    #Primera parte, busco maximo
    li a7,4
    la a0,textoMax
    ecall
    jal buscaMax
    li a7,1
    ecall
    
    #Segunda parte, busco cuantas veces esta el maximo
    #dentro del vector e imprimo
    li a7,4
    la a0,textoCan
    ecall
    jal buscaCant
    li a7,1
    ecall
    #imprimo CRLF
    li a7,11
    li a0,10
    ecall
    #Tercera parte, imprimo las posiciones del vector original
    #identificadas en vectorPos
    jal imprimePos
    #termino
    li a7,10
    ecall
    
buscaMax:
    li t1, 0
    mv t3,a3
loopMax:
    lhu t0,0(t3)
    addi t3,t3,2
    blt t0,t1,loopMax
    mv t1,t0
    bne t3,a4,loopMax
    mv a0,t1
    ret
buscaCant:
    addi sp,sp,-4
    sw s0,0(sp)
    mv s0,ra
    jal buscaMax
    li t0,0
    mv t3,a3
loopCant:
    lhu t1,0(t3)
    addi t3,t3,2
    bne a0,t1,loopCant
    addi t0,t0,1
    bne t3,a4,loopCant
    mv ra,s0
    lw s0,0(sp)
    mv a0,t0
    ret
imprimePos:
    lb t0,0(a4)
    slli t0,t0,1
    add t1,a3,t0
    lhu a0,0(t1)
    li a7,1
    ecall
    li a0,20
    li a7,11
    ecall
    addi a4,a4,1
    bne a4,a5,imprimePos
    ret