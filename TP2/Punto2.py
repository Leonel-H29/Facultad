import csv

path = 'D:\\Leo\\Facultad\\Trabajos\\3º Año\\Metodos Numericos II\\Metodos-Numericos-II\\TP2\\'
file = path +  'time_series_covid19_confirmed_global.csv'

data=[]
with open(file) as File:
    reader = csv.reader(File, delimiter=',', quotechar=',',
                        quoting=csv.QUOTE_MINIMAL)
    for row in reader:
    	data_row=list(row)
    	data.append(data_row)
        #print(row)
print(data)



	

