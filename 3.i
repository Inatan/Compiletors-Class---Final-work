loadI 0 => rbss
loadI 0 => fp
loadI 0 => sp
jumpI L2
L2:
addI fp,4 => sp
L3:
loadAI fp,0 => r2
loadI 0 => r3
cmp_GT r2,r3 => r1
cbr r1 => L0,L1
L0:
loadAI fp,0 => r4
loadI 1 => r6
loadI 1 => r7
addI r6,1 => r5
storeAI r5 => fp,0
jumpI L3
L1:
