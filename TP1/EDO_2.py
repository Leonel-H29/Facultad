import numpy as np
import matplotlib.pyplot as plt
from scipy.integrate import ode, odeint, solve_ivp
"""
def y_prima(x,y):
	return 4*np.exp(0.8*x)-0.5*y
	#pass

def y_solucion(x):
	return 3.07692*np.exp(0.8*x)-2.07692/np.exp(0.5*x)
"""
def y_prima2(x,y):
	y1, y2 = y
	return (y2, 2*y2-2*y1+np.exp(2*x)*np.sin(x))
	#pass

def y_solucion(x):
	return (np.exp(2*x)*(np.sin(x)-2*np.cos(x)))/5

a, b = 0, 1
t0 = np.linspace(a,b,2) 
y0=[-0.4, -0.6]
#res = solve_ivp(y_prima, t0, y0, method='LSODA' )
#res = solve_ivp(y_prima, t0, y0, method='LSODA' , rtol=10**(-10) )
res = solve_ivp(y_prima2, t0, y0, method='LSODA') 
print(f"y: {res}")