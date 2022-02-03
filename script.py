import matplotlib.pyplot as plt

fig = plt.figure(figsize=(9, 6))
ax = fig.add_subplot()

f = open("data.txt", "r")
area = []
angle = []
flag = 0
for line in f:
    for num in line.strip().split(';'):
        if flag == 0:
            angle.append(float(num))
            flag = 1
            continue
        else:
            area.append(float(num))
            flag = 0

f.close()
delta = angle[1] - angle[0]
ax.bar(angle, area, width=delta, color='magenta', edgecolor='black')
plt.ylabel('S, mm^2')
plt.xlabel('theta, degrees')

plt.show()
