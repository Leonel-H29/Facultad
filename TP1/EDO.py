import numpy as np
import matplotlib.pyplot as plt
from scipy.integrate import ode, odeint, solve_ivp

def y_prima(x,y):
	return 4*np.exp(0.8*x)-0.5*y
	#pass

def y_solucion(x):
	return 3.07692*np.exp(0.8*x)-2.07692/np.exp(0.5*x)

a, b = 0, 1
#t0= 0
t0 = np.linspace(a,b) 
y0=y_solucion(t0)

print(f"y: { odeint(y_prima, y0, t0) }")

#print(y_solucion(t0))
