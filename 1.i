loadI 0 => rbss
loadI 1024 => fp
loadI 1024 => sp
jumpI L2
L2:
addI fp,0 => sp
L3:
loadAI rbss,0 => r1
cbr r1 => L0,L1
L0:
loadAI rbss,0 => r2
loadI 0 => r3
storeAI r3 => rbss,0
jumpI L3
L1:
