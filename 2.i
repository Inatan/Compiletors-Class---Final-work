loadI 0 => rbss
loadI 0 => fp
loadI 0 => sp
jumpI L2
L2:
addI fp,0 => sp
L0:
loadAI rbss,0 => r1
loadI 0 => r2
storeAI r2 => rbss,0
loadAI rbss,0 => r3
cbr r3 => L0,L1
L1:
