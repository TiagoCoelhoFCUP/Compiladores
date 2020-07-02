import numpy as np
import matplotlib.pyplot as plt
import psycopg2
import math
from matplotlib.animation import FuncAnimation
import datetime
from postgis import Polygon,MultiPolygon
from postgis.psycopg import register
#define the step in seconds of the animation

step = 10
conn = psycopg2.connect("dbname=postgres user=tiago")
register(conn)
cursor_psql = conn.cursor()
sql = """select distinct taxi from tracks order by 1"""
cursor_psql.execute(sql)
results = cursor_psql.fetchall()
taxis_x ={}
taxis_y ={}
ts_i = 1570665600
#porque  este tempo final? ha registos mais tarde
ts_f = 1570670000

array_size = int(24*60*60/step)

for row in results:
	taxis_x[int(row[0])] = np.zeros(array_size)
	taxis_y[int(row[0])] = np.zeros(array_size)


for i in range(ts_i,ts_f,10):
 	sql = "select taxi,st_pointn(proj_track," + str(i) + "-ts) from tracks where ts<" + str(i) + " and ts+st_numpoints(proj_track)>" + str(i)
 	cursor_psql.execute(sql)
 	results = cursor_psql.fetchall()
 	for row in results:
 		x,y = row[1].coords
 		taxis_x[int(row[0])][int((i-ts_i)/10)] = x
 		taxis_y[int(row[0])][int((i-ts_i)/10)] = y

offsets = []
for i in range(array_size):
	l = []
	for j in taxis_x:
		l.append([taxis_x[j][i],taxis_y[j][i]])
	offsets.append(l)

for i in offsets:
    print("%f %f" %(i[0][0],i[0][1]),end='')
    for j in range(1,len(i)):
        print(",%f %f" %(i[j][0],i[j][1]),end='')
    print("")


offsets = np.array(offsets)


min_dist=50
count=0
#calculate distances
interactions=[]
for time in range(360):
	time_i=[]
	for t_i in range(len(offsets[0])):
		interacts=[]
		for t_j in range(t_i +1,len(offsets[0])):
			coord_i , coord_j = offsets[time][t_i] , offsets[time][t_j]
			if coord_i.all()  != 0 and coord_j.all() != 0:
				sql_d = """select st_distance(ST_GeomFromText('POINT(""" + str(coord_i[0])+ " " + str(coord_i[1]) + """)',3763),ST_GeomFromText('POINT(""" + str(coord_j[0])+ " " + str(coord_j[1]) + """)',3763));"""
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
