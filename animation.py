import numpy as np
import matplotlib.pyplot as plt
import psycopg2
import math
from matplotlib.animation import FuncAnimation
import datetime
import csv
from postgis import Polygon,MultiPolygon
from postgis.psycopg import register
import matplotlib.gridspec as gridspec

og_border = [-120000, 165000, -310000, 285000]
curr_border = [-120000, 165000, -310000, 285000]
new_border = []


def get_new_border(i):
	global curr_border
	global og_border
	global new_border

	index = count_infected[i].index(max(count_infected[i]))

	print(max(count_infected[i]))
	print(index)
	print(distrito_nome[index])

	x_min = distrito_coords[index][0]
	y_min = distrito_coords[index][1]
	x_max = distrito_coords[index][2]
	y_max = distrito_coords[index][3]

	new_border = [x_min,x_max,y_min,y_max]

	periodo = 500
	t = 150
	quant = [abs((-120000 - x_min)/t),abs((165000 - x_max)/t),abs((-310000 - y_min)/t),abs((285000 - y_max)/t)]

	x_min,x_max,y_min,y_max = curr_border[0], curr_border[1], curr_border[2], curr_border[3]

	if i> periodo and i%periodo <= t:
		new_x_min,new_x_max,new_y_min,new_y_max = new_border[0], new_border[1], new_border[2], new_border[3]


		if x_min < new_x_min:
			x_min = x_min + quant[0]
		if x_max > new_x_max:
			x_max = x_max - quant[1]
		if y_min < new_y_min:
			y_min = y_min + quant[2]
		if y_max > new_y_max:
			y_max = y_max - quant[3]

	else:
		new_x_min,new_x_max,new_y_min,new_y_max = og_border[0], og_border[1], og_border[2], og_border[3]
		if i%periodo > periodo/2:
			if x_min != new_x_min or x_max != new_x_max or y_min != new_y_min or y_max != new_y_max:
				if x_min > new_x_min:
					x_min = x_min - quant[0]
				if x_max < new_x_max:
					x_max = x_max + quant[1]
				if y_min > new_y_min:
					y_min = y_min - quant[2]
				if y_max < new_y_max:
					y_max = y_max + quant[3]

	curr_border = [x_min,x_max,y_min,y_max]


def animate(i):
	ax.set_title(datetime.datetime.utcfromtimestamp(ts_i+i*10))

	get_new_border(i)
	ax.set(xlim=(curr_border[0],curr_border[1]),ylim=(curr_border[2], curr_border[3]))

	color_array = []
	for state in states[i]:
		if state == 1:
			color_array.append((0.92,0,0))
		if state == 0:
			color_array.append((0.20,0.66,0.28))

	scat.set_offsets(offsets[i])
	scat.set_color(np.array(color_array))

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

interactions = np.load('interactions.npy', allow_pickle=True)

#Get every district boundries
cursor_psql = conn.cursor()
sql = "select distrito,st_envelope(st_union(proj_boundary)),st_astext(st_union(proj_boundary)) from cont_aad_caop2018 group by distrito"

cursor_psql.execute(sql)
results = cursor_psql.fetchall()

distrito_nome = []
distrito_coords = []
distrito_sql = []
count_infected = []

for i in range(len(offsets)):
	count_infected.append([])
	for j in range(len(results)):
		count_infected[i].append(0)

for row in results:
	envelope = row[1]
	xys = envelope.coords[0]
	xs,ys = [],[]
	for (x,y) in xys:
		xs.append(x)
		ys.append(y)

	x_max = max(xs)
	x_min = min(xs)
	y_max = max(ys)
	y_min = min(ys)

	comprimento = abs(x_max - x_min)
	altura = abs(y_max - y_min)
	addon = (comprimento * 2.09 - altura)/2
	y_max = y_max + addon
	y_min = y_min - addon

	distrito_nome.append(row[0])
	distrito_coords.append((x_min,y_min,x_max,y_max))
	distrito_sql.append(row[2])

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
rd.seed(123)

start_infection = []
start_infection.append(int(rd.choice(t_P)))
start_infection.append(int(rd.choice(t_L)))
for i in range(len(offsets)):
	count_infected[i][distrito_nome.index('PORTO')] = 1
	count_infected[i][distrito_nome.index('LISBOA')] = 1
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
prob = 1

for time in range(len(offsets)):
	states.append(s)
print(len(states),len(offsets),len(offsets[0]),len(states[0]),len(interactions),len(interactions[0]))

for ts in range(len(interactions)):
	for index in range(len(interactions[0])):
		for taxi in interactions[ts][index]:
			if states[ts][index] == 1 and states[ts][taxi] == 0:
				if rd.random() <= prob:
					for i in range(ts+1,len(interactions)):
						states[i][taxi] = 1

					for j in range(len(distrito_coords)):
						sql = """select st_within(ST_GeomFromText('POINT(""" + str(offsets[ts][taxi][0])+ " " + str(offsets[ts][taxi][1]) + """)',3763), ST_GeomFromText('""" + distrito_sql[j] + """',3763));"""
						cursor_psql.execute(sql)
						results = cursor_psql.fetchall()

						if results[0][0] == True:
							print("entrou")
							for k in range(ts+1,len(offsets)):
								count_infected[k][j] = count_infected[k][j] + 1
							break

			if states[ts][index] == 0 and states[ts][taxi] == 1:
				if rd.random() <= prob:
					for i in range(ts+1,len(interactions)):
						states[i][index] = 1

					for j in range(len(distrito_coords)):
						sql = """select st_within(ST_GeomFromText('POINT(""" + str(offsets[ts][index][0])+ " " + str(offsets[ts][index][1]) + """)',3763), ST_GeomFromText('""" + distrito_sql[j] + """',3763));"""
						cursor_psql.execute(sql)
						results = cursor_psql.fetchall()

						if results[0][0] == True:
							print("entrou")
							for k in range(ts+1,len(offsets)):
								count_infected[k][j] = count_infected[k][j] + 1
							break



ts_i=1570665600
scale=1/3000000

xs_min, xs_max, ys_min, ys_max = -120000, 165000, -310000, 285000
width_in_inches = (xs_max-xs_min)/0.0254*1.1
height_in_inches = (ys_max-ys_min)/0.0254*1.1

# Create 2x2 sub plots
gs = gridspec.GridSpec(2, 2)

fig = plt.figure(figsize=(width_in_inches*2*scale , height_in_inches*scale))

ax = fig.add_subplot(gs[:, 0]) # span all rows, col 0

ax2 = fig.add_subplot(gs[0, 1]) # row 0, col 1

ax3 = fig.add_subplot(gs[1, 1]) # row 1, col 1

ax.axis('off')
ax.set(xlim=(xs_min, xs_max), ylim=(ys_min, ys_max))

cursor_psql = conn.cursor()
sql = "select distrito,st_union(proj_boundary) from cont_aad_caop2018 group by distrito"

cursor_psql.execute(sql)
results = cursor_psql.fetchall()

xs , ys = [],[]
for row in results:
    geom = row[1]
    if type(geom) is MultiPolygon:
        for pol in geom:
            xys = pol[0].coords
            xs, ys = [],[]
            for (x,y) in xys:
                xs.append(x)
                ys.append(y)
            ax.plot(xs,ys,color='black',lw='0.2')
    if type(geom) is Polygon:
        xys = geom[0].coords
        xs, ys = [],[]
        for (x,y) in xys:
            xs.append(x)
            ys.append(y)
        ax.plot(xs,ys,color='black',lw='0.2')

x,y = [],[]
for i in offsets[0]:
	x.append(i[0])
	y.append(i[1])

color_array = []
for state in states[0]:
	if state == 1:
		color_array.append((0.92,0,0))
	if state == 0:
		color_array.append((0.20,0.66,0.28))

scat = ax.scatter(x,y,s=2,color=color_array)

anim = FuncAnimation(
	fig, animate, interval=10, frames=len(offsets)-1, repeat = False)

plt.draw()
plt.show()
