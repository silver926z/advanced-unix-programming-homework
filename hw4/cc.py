f = open("john.txt", "r")
cnt = 0
mlen = 0
while True:
    a = f.readline()
    if a == "":
        break
    print(a)
    cnt+=1
    if(len(a) > mlen):
        mlen = len(a)

print("MLL:%d"%(mlen))
print("CNT:%d"%(cnt))
