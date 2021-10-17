import numpy as np
import matplotlib.pyplot as plt

#Solucion exacta de la funcion a)

def y_solucion(t):
  return t*np.log(t) +2*t

def y_prima(t,y):
  return 1+(y/t)

#Solucion exacta de la funcion b)
"""
def y_solucion(t):
  return t-np.exp(-5*t)

def y_prima(t,y):
  return 5*np.exp(5*t)*(y-t)**(2)+1
"""

#Predictor
def EulerExplicito(t, y, h , f_p):
	return y + h *f_p(t,y)

#Corrector
def Trapecio(t0, t1, y0,y1,h,f_p):
	k1=f_p(t0, y0)
	k2=f_p(t1, y1)
	y_posterior= y0 + 0.5 * h *(k1+k2)

	return y_posterior


def Huen(t0, y0,h,maximo,f_sol,f_p, E):
	t1=t0+h
	
	i, j, count = 0,0,0 #Contadores

	for i in range(maximo):
		error = E + 1 #Error
		y1=EulerExplicito(t0, y0, h, f_p) ##PREDICTOR

		##CORRECTOR
		while np.abs(error) > E and count<maximo:
			y_anterior = y1
			y1= Trapecio(t0, t1, y0, y1,h, f_p)
			error = np.abs(y_anterior-y1)
			count +=1
			"""
			Muestro el error y la cantidad de iteracciones.
			"""
			print(f"****{j+1}: [y: {y1}] , [Error: {error}, Iteracciones: {count}]") 
			#pass
		#pass
		count=0

		#Calculo los errores
		error_local = np.abs(y1-y0)
		error_global = np.abs(y1-f_sol(t1))
		print(f"{i+1}: [t: {t1}, y: {y1}] , [EL: {error_local}, EG: {error_global}]")

		#Para la proxima iteraccion
		t0=t1
		t1 = t0 + h
		y0=y1
		


#Intervalos
a=1
b=2
h=0.2
#Valores iniciales
t0=a
y0=y_solucion(t0)
maximo=int((b-a)/h) #Numero de iteracciones
error=10**(-2)

print(f"0: [t: {t0}, y: {y0}] ")
Huen(t0,y0,h,maximo,y_solucion,y_prima, error)