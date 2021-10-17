
x=[0,5,10,15,20,25]
v=[0,9.85,14.32,17.63,19.34,22.41]

n=len(x) #Cantidad de puntos
x_cuadrado=[n**2 for n in x] #Elevo al cuadrado los elementos de x
y=[n**2 for n in v] #Elevo al cuadrado los elementos de v
xy=[x*y for x,y in zip(x,y)] #Multiplico cada elemento entre x e y

#print(x_cuadrado)
#print(y)
#print(xy)

sumatoria_x=sum(x)
sumatoria_y=sum(y)
sumatoria_xy=sum(xy)
sumatoria_x_cuadrado=sum(x_cuadrado)
#print(f"{sumatoria_x}-{sumatoria_y}-{sumatoria_xy}-{sumatoria_x_cuadrado}")
a1=(n*sumatoria_xy-sumatoria_x*sumatoria_y)/(n*sumatoria_x_cuadrado-sumatoria_x**2)
print(f"a1: {a1}")