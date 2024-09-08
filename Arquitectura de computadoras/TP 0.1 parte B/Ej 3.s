#Luego de resolver B.02, almacene en x6 el valor de x5 más uno, y en x7 el valor de x5
#menos uno.

.text
   lui x5, 0x12345
   addi x5,x5,0x678
   addi x6,x5,1
   addi x7,x5,-1