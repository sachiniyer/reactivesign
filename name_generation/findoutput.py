f = open("name.txt", "r")
name = []
back = []
merge = ""
rows = 11
columns = 44
for i in range(rows):
    line = f.readline();
    if (i % 2 == 0):
        for j in range(columns):
            if (line[j] == " "):
                name.append(i*columns + j)
            else:
                back.append(i*columns +j)
    else:
        for j in range(44):
            if (line[j] == " "):
                name.append(i*columns + (columns-1-j))
            else:
                back.append(i*columns + (columns-j))
print(name)
print(back)
