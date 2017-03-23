res = "\""
f = open("john.txt", "r")

while True:
    a = f.readline()
    if a == "":
        break
    res += a
    res +="\",\""
res += "\""
print(res.replace("\n",""))
