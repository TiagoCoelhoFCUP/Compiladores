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
ts_f = 1570667000

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




conn.close()





offsets = np.array(offsets)



# first 2 infections at random


conn = psycopg2.connect("dbname=postgres user=tiago")
register(conn)
cursor_psql = conn.cursor()

#select first 10 taxis in PORTO
sql3 = """select distinct taxi , ts from tracks where st_contains((select st_collect(proj_boundary) from cont_aad_caop2018 where concelho='PORTO'), st_startpoint(proj_track)) ORDER BY ts  limit 10"""

cursor_psql.execute(sql3)
results_i = cursor_psql.fetchall()
t_P=[]
for taxi in results_i:
	t_P.append(taxi[0])

print(t_P)

#select first 10 taxis in LISBON
sql4 = """select distinct taxi , ts from tracks where st_contains((select st_collect(proj_boundary) from cont_aad_caop2018 where concelho='LISBOA'), st_startpoint(proj_track)) ORDER BY ts  limit 10"""

cursor_psql.execute(sql4)
results_j = cursor_psql.fetchall()
t_L=[]
for taxi in results_j:
	t_L.append(taxi[0])
print(t_L)

# select 2 at random, one from each district
import random as rd

start_infection = []
start_infection.append(int(rd.choice(t_P)))
start_infection.append(int(rd.choice(t_L)))
print(start_infection)

#find index in array
i_infection =[0,0]
taxis_l=list(taxis_x.keys())
i_infection[0] , i_infection[1] = taxis_l.index(start_infection[0]) , taxis_l.index(start_infection[1])
print(i_infection)


# initial infection states

s = np.zeros(len(offsets[0]))
i_infection =[0,0]
taxis_l=list(taxis_x.keys())
i_infection[0] , i_infection[1] = taxis_l.index(start_infection[0]) , taxis_l.index(start_infection[1])
print(i_infection)


# initial infection states

s = np.zeros(len(offsets[0]))
s[i_infection[0]] , s[i_infection[1]] = 1 , 1

s[i_infection[0]] , s[i_infection[1]] = 1 , 1

states=[]

for time in range(len(offsets)):
	states.append(s)
print(len(states),len(offsets),len(offsets[0]),len(states[0]))


min_dist=50
count=0
#calculate distances
interactions=[]
for time in range(200):
	print('time:',time)
	time_i=[]
	for t_i in range(len(offsets[0])):
		interacts=[]
		for t_j in range(t_i +1,len(offsets[0])):



			coord_i , coord_j = offsets[time][t_i] , offsets[time][t_j]

			if coord_i.all()  != 0 and coord_j.all() != 0:

				sql_d = """select st_distance(ST_GeomFromText('POINT(""" + str(coord_i[0])+ " " + str(coord_i[1]) + """)',4326),
		ST_GeomFromText('POINT(""" + str(coord_j[0])+ " " + str(coord_j[1]) + """)',4326));"""
				cursor_psql.execute(sql_d)
				results_d = cursor_psql.fetchall()
				distance = int(results_d[0][0])

				if distance <= min_dist:
					interacts.append(t_j)

					count+=1
		time_i.append(interacts)


	interactions.append(time_i)


interactions=np.array(interactions)

print(np.size(interactions))
print(count,'interações')
print(len(interactions),len(interactions[0]))

np.save('interactions', interactions, allow_pickle=True)

interactions = np.load('interactions.npy', allow_pickle=True)

print(np.size(interactions))
print(count,'interações2')
print(len(interactions),len(interactions[0]))

conn.close()
