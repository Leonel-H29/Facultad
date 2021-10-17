import numpy as np
import matplotlib.pyplot as plt

#Solucion exacta de la funcion a)

def y_solucion(t):
  return t*np.log(t) +2*t

def y_prima(t,y):
  return 1+(y/t)

"""
#Solucion exacta de la funcion b)
def y_solucion(t):
  return t-np.exp(-5*t)

def y_prima(t,y):
  return 5*np.exp(5*t)*(y-t)**(2)+1
"""

def AdamsBashford2(t0, y0,h,maximo,f_sol,f_p):
	#Tomo los 2 puntos iniciales

	t1 = t0 + h
	y1 = y0 + h * f_p (t0, y0)
	#Calculo los errores
	error_local = np.abs(y1-y0)
	error_global = np.abs(y1-f_sol(t1))
	print(f"0: [t: {t1}, y: {y1}] , [EL: {error_local}, EG: {error_global}]")
	print(f"----------------")
	maximo -=1

	for i in range(maximo):
		k1= 3*f_p(t1, y1)
		k2=f_p(t0, y0)
		#Para la proxima iteraccion
		t0=t1
		y0=y1

		#Calculo Actual
		t1=t0+h 
		y1= y0 + 0.5 * h *(k1-k2)

		#Calculo los errores
		error_local = np.abs(y1-y0)
		error_global = np.abs(y1-f_sol(t1))
		print(f"{i+1}: [t: {t1}, y: {y1}] , [EL: {error_local}, EG: {error_global}]")
		#pass
	


#Intervalos
a=0
b=1
h=10**(-3)
#Valores iniciales
t0=a
y0=y_solucion(t0)
maximo=int((b-a)/h) #Numero de iteracciones

print(f"--------Puntos iniciales")
print(f"0: [t: {t0}, y: {y0}] ")
AdamsBashford2(t0,y0,h,maximo,y_solucion,y_prima)