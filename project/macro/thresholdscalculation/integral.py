from math import pi

def lcheck(index, params):
    seq, x = params
    return seq[index][0] >= x

def lbinsearch(l, r, ckeck, checkparams):
    while l < r:
        m = (l + r) // 2
        if ckeck(m, checkparams):
            r = m
        else:
            l = m + 1
    return l

file = open('../../input/IntPam2009.txt')
data = file.readlines()

fluxe = []

for line in data[1:]:
    f, e = map(float, line.split())
    fluxe.append((f, e))

def func(energy):
    index = lbinsearch(0, len(fluxe) - 1, lcheck, (fluxe, energy))
    return fluxe[index][1]


n = 100000
dx = fluxe[-1][0] / n

energyi = 0.
integral = 0.
for _ in range(n):
    integral += func(energyi) * dx
    energyi += dx

r = 1.25
t = 1 / 16
norm = 16. * pi**2 * r**2 * t
print(norm * integral)



