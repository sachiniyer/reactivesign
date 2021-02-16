f = open("name.txt", "r")
name = []
back = []
merge = ""

for i in range(11):
    line = f.readline();
    if (i % 2 == 0):
        for j in range(44):
            if (line[j] == " "):
                name.append(i*44 + j)
            else:
                back.append(i*44 +j)
    else:
        for j in range(44):
            if (line[j] == " "):
                name.append(i*44 + (43-j))
            else:
                back.append(i*44 + (43-j))
print(name)
print(back)
