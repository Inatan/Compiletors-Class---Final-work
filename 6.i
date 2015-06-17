loadI 0 => rbss
loadI 0 => fp
loadI 0 => sp
jumpI L2
L2:
addI fp,0 => sp
L3:
loadAI rbss,0 => r2
loadI 0 => r3
cmp_EQ r2,r3 => r1
cbr r1 => L0,L1
L0:
loadAI rbss,0 => r4
loadI 1 => r6
loadI 2 => r7
addI r6,2 => r5
storeAI r5 => rbss,0
jumpI L3
L1:
