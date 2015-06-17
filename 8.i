loadI 0 => rbss
loadI 0 => fp
loadI 0 => sp
jumpI L3
L3:
addI fp,0 => sp
loadAI rbss,0 => r2
cbr r2 => L2,L1
L2:
loadAI rbss,0 => r3
and r2,r3 => r1
cbr r1 => L0,L1
L0:
loadAI rbss,0 => r4
loadI 0 => r5
storeAI r5 => rbss,0
L1:
