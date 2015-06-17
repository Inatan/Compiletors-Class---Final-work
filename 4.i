loadI 0 => rbss
loadI 0 => fp
loadI 0 => sp
jumpI L0
L0:
addI fp,0 => sp
loadAI rbss,0 => r0
loadI 1 => r4
loadI 2 => r5
addI r4,2 => r3
loadI 3 => r6
addI r3,3 => r2
loadI 4 => r7
addI r2,4 => r1
storeAI r1 => rbss,0
