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

file = open('../../input/IntPam2014.txt')
data = file.readlines()

fluxe = []
sf = 0.
for line in data[1:]:
    e, f = map(float, line.split())
    fluxe.append((e, f))
    sf += f

def func(energy):
    index = lbinsearch(0, len(fluxe) - 1, lcheck, (fluxe, energy))
    return fluxe[index][1]


n = 100000
dx = (fluxe[-1][0] - fluxe[0][0]) / n

energyi = 0.
integral = 0.
for _ in range(n):
    integral += func(energyi) * dx
    energyi += dx

r = 1.25
t = 16. / 10**6
norm = 8. * (pi * r)**2

print('nprotons =', round(norm * integral * t))
print('nprotons =', round(norm * sf * t))

