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
sfluxes = 0.
for line in data[1:]:
    e, f = map(float, line.split())
    sfluxes += f
    fluxe.append((e, f))

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

r = 0.62
t = 128 / 10**6
norm = 16. * pi**2 * r**2
print('sfluxes=', round(sfluxes))
print('sfluxes * norm =', round(sfluxes * norm))
print('sfluxes * norm * t=', round(sfluxes * norm * t))
print('norm=', round(norm))
print('integral=', round(integral))
print('norm * integral =', round(norm * integral))
print(round(norm * integral * t))



