import csv
from matplotlib import pyplot as plt
import numpy as np
import pandas as pd

"""
def LeerCSV(file):
    data = []
    with open(file) as File:
        reader=csv.reader(File,delimiter=',',quotechar=',',quoting=csv.QUOTE_MINIMAL)
        for row in reader:
            data_row=list(row)
            data.append(data_row)
    #pass
    return data
"""
def LeerCSV(url):
    data = pd.read_csv(url, index_col=None) 
    return data.to_numpy()


def Graficar(data):
    x=np.array(data[:,0])
    y=np.array(data[:,1])

    #Grafico 
    plt.figure(figsize=(16,8))
    plt.plot(x,y,color='green')
    plt.show()

    fft = np.fft.fft(y)
    freq = np.fft.fftfreq(len(y))

    #Grafico | Frecuencia
    plt.figure(figsize=(16,8))
    plt.ylabel("Amplitude")
    plt.xlabel("Frequency [Hz]")
    plt.stem(freq,abs(fft.real),markerfmt=" ")
    plt.show()
    
   
path = 'D:\\Leo\\Facultad\\Trabajos\\3º Año\\Metodos Numericos II\\Metodos-Numericos-II\\TP3\\'
file1 = path +  'muestra1.csv'
file2 = path + 'muestra2.csv'

muestra1 = LeerCSV(file1)
muestra2 = LeerCSV(file2)
#print(muestra1[0][0])
#print("------------------------")
#print(muestra2)

print("GRAFICOS MUESTRA 1")
Graficar(muestra1)
print("------------------------")
print("GRAFICOS MUESTRA 2")
Graficar(muestra2)


    

