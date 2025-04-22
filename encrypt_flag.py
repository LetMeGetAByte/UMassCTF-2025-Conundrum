# DO NOT INCLUDE IN PUBLIC FACING FILES

FLAG = b"UMASS{T3rr1bl3_70_D36ug_H0p3fu11y_W0r53_70_Crack}"

def gen_key():
    key = []
    j = 0x44; # change this to change seed
    for i in range(1024):
        key.append(j)
        j = ((j << 1) + j) & 0xFF;

    return key

key = gen_key()
with open("flag.txt", "wb") as f:
    data = []
    for i in range(len(FLAG)):
        data.append(FLAG[i] ^ key[i % len(key)])
    f.write(bytes(data))