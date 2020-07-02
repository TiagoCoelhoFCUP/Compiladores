import numpy as np
import matplotlib.pyplot as plt
import psycopg2
import math
from matplotlib.animation import FuncAnimation
import datetime
import csv
from postgis import Polygon,MultiPolygon
from postgis.psycopg import register

step = 10
conn = psycopg2.connect("dbname=postgres user=tiago")
register(conn)
cursor_psql = conn.cursor()
sql = """select distinct taxi from tracks order by 1"""
cursor_psql.execute(sql)
results = cursor_psql.fetchall()
taxis_x ={}

array_size = int(24*60*60/step)

for row in results:
	taxis_x[int(row[0])] = np.zeros(array_size)

offsets = []
with open('offsets.csv', 'r') as csvFile:
    reader = csv.reader(csvFile)
    i = 0
    for row in reader:
        l = []
        for j in row:
            x,y = j.split()
            x = float(x)
            y= float(y)
            l.append([x,y])
        offsets.append(l)

offsets = np.array(offsets)


min_dist=50
count=0
#calculate distances
interactions=[]
for time in range(len(offsets)):
	print(time)
	time_i=[]
	for t_i in range(len(offsets[0])):
		interacts=[]
		for t_j in range(t_i +1,len(offsets[0])):
			coord_i , coord_j = offsets[time][t_i] , offsets[time][t_j]
			if coord_i.all()  != 0 and coord_j.all() != 0:
				sql_d = """select st_distance(ST_GeomFromText('POINT(""" + str(coord_i[0])+ " " + str(coord_i[1]) + """)',4326),ST_GeomFromText('POINT(""" + str(coord_j[0])+ " " + str(coord_j[1]) + """)',4326));"""
				cursor_psql.execute(sql_d)
				results_d = cursor_psql.fetchall()
				distance = int(results_d[0][0])

				if distance <= min_dist:
					interacts.append(t_j)
					count+=1
		time_i.append(interacts)
	interactions.append(time_i)
interactions=np.array(interactions)


np.save('interactions', interactions, allow_pickle=True)


conn.close()
