.text
    li a7,1
    li a0,25
    jal div8 
    ecall
    li a0,10
    li a7,11
    ecall #enter
    li a7,1
    li a0,-25
    jal div8
    ecall
    li a7,10
    ecall
    nop
div8:
    srai a0,a0,3
    bltz a0,div8_negative
    j fin
div8_negative:
    addi a0,a0,1
fin:
    ret