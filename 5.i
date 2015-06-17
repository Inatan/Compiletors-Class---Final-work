loadI 0 => rbss
loadI 0 => fp
loadI 0 => sp
jumpI L0
L0:
addI fp,8 => sp
loadAI fp,0 => r0
loadI 0 => r1
storeAI r1 => fp,0
loadAI fp,4 => r2
loadAI fp,0 => r3
storeAI r3 => fp,4
