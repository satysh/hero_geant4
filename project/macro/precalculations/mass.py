from numpy import pi 

ro_scint = 1.032 # g / cm3
ro_wolfram = 19.25 # g / cm3
NORM = (4. / 3.) * pi


print('введите радиус в сантиметрах: ')
R = int(input())

print('Vdet=', round(NORM * R**3), 'cm3')

# Внутренний слой сцинтилятор
m_scint = 0.
m_wolfram = 0.
r_scint = 0.
r_wolfram = 0.
for ri in range(R):
	r_scint = ri + 0.9
	r_wolfram = ri 
	m_scint +=  (r_scint**3 - r_wolfram**3)  
	m_wolfram += ((r_wolfram + 1.)**3 - r_scint**3)

print('Vscint=', round(m_scint * NORM), 'cm3')
print('Vabsorb=', round(m_wolfram * NORM), 'cm3')
m_scint *= NORM * ro_scint
m_wolfram *= NORM * ro_wolfram 
print('1. масса детектора для случая, когда самый внутренний слой сцинтиллятор:', round((m_scint + m_wolfram) / 10**6, 2), ' [тонн]')
print('   из них ', round(m_scint / 10**6, 2), ' [тонн] сцинтиллятора и ', round(m_wolfram / 10**6, 2), ' [тонн] вольфрама.')


# Внутренний слой вольфрам
m_wolfram = 0.
m_scint = 0.
r_wolfram = 0.
r_scint = 0.
for ri in range(R):
	r_wolfram = ri + 0.1 
	r_scint = ri
	m_wolfram += (r_wolfram**3 - r_scint**3)
	m_scint +=  ((r_scint + 1.)**3 - r_wolfram**3)


print('Vscint=', round(m_scint * NORM), 'cm3')
print('Vabsorb=', round(m_wolfram * NORM), 'cm3')
m_scint *= NORM * ro_wolfram 
m_wolfram *= NORM * ro_scint 
print('2. масса детектора для случая, когда самый внутренний слой вольфрам:', round((m_scint + m_wolfram) / 10**6, 2), ' [тонн]')
print('   из них ', round(m_scint / 10**6, 2), ' [тонн] сцинтиллятора и ', round(m_wolfram / 10**6, 2), ' [тонн] вольфрама.')


