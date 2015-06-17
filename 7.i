loadI 0 => rbss
loadI 0 => fp
loadI 0 => sp
jumpI L0
L0:
addI fp,0 => sp
loadAI rbss,0 => r0
loadI 0 => r3
loadI 1 => r5
loadI 2 => r6
addI r5,2 => r4
mult r3,r4 => r2
loadI 3 => r7
multI r2,3 => r1
storeAI r1 => rbss,0
loadAI rbss,4 => r8
loadAI rbss,0 => r10
loadI 1 => r12
loadAI rbss,0 => r13
mult r12,r13 => r11
add r10,r11 => r9
storeAI r9 => rbss,4