#! /usr/bin/python
from visual import *
import subprocess
import sys


DATA_EMPTY='e'
DATA_NOUGHT='x'
DATA_CROSS='o'

board_data = list()
board_models = list()

#Drawing constants
GRID_COLOR = color.white
NOUGHTS_COLOR = color.green
CROSSES_COLOR = color.red
DOT_COLOR = color.white
DOTS_VISIBLE = False
BOARD_DIMENSION = 4
GAP_SIZE   = 10
BOARD_SIZE = GAP_SIZE * BOARD_DIMENSION
THICKNESSS = 0.25
NOUGHTS_RADIUS = GAP_SIZE/3.0

CROSSES_SIZE = (GAP_SIZE,1,1)
CENTRE_OFFSET = vector(GAP_SIZE/2.0,GAP_SIZE/2.0,GAP_SIZE/2.0) - vector(BOARD_SIZE/2.0,BOARD_SIZE/2.0,BOARD_SIZE/2.0)


#Draw a sphere for the noughts player.
def make_nought(pos, color=NOUGHTS_COLOR, radius=NOUGHTS_RADIUS):
	nought = sphere(pos=vector(pos)*GAP_SIZE+CENTRE_OFFSET, color=color, radius=radius)
	return nought

#Draw a dot for an empty space.
def make_dot(pos, color=DOT_COLOR, radius=NOUGHTS_RADIUS/10.0):
	dot = make_nought(pos,color,radius)
	dot.visible = DOTS_VISIBLE
	return dot

#Draw an X for the crosses player.
def make_cross(pos, color=CROSSES_COLOR, size=CROSSES_SIZE):
	cross = frame()
	box(frame=cross,pos=vector(pos)*GAP_SIZE+CENTRE_OFFSET, axis=(1,1,1), color=color, size=size)
	box(frame=cross,pos=vector(pos)*GAP_SIZE+CENTRE_OFFSET, axis=(-1,1,-1), color=color, size=size)
	return cross

#A function that combines the list into one big string we can send to the worker
def serialize_board(data):
	return ''.join(data)

#Translates a board x y z coordinate into an index for our lists
def coords_to_index(x,y,z):
	return z+y*(BOARD_DIMENSION)+x*(BOARD_DIMENSION*BOARD_DIMENSION)

def update_models(data):
	for x in range(0,BOARD_DIMENSION):
		for y in range(0,BOARD_DIMENSION):
			for z in range(0,BOARD_DIMENSION):
				state = data[coords_to_index(x,y,z)]
				
				#remove the old model
				board_models[coords_to_index(x,y,z)].visible = False
				del board_models[coords_to_index(x,y,z)]
				
				if	 state=='e' :
					board_models.insert(coords_to_index(x,y,z), make_dot(pos=(x,y,z)) )
				elif state=='x' :
					board_models.insert(coords_to_index(x,y,z), make_cross(pos=(x,y,z)) )
				elif state=='o' :
					board_models.insert(coords_to_index(x,y,z), make_nought(pos=(x,y,z)) )
				
				
	return

#Removed the grid as it made the whole thing a lot more confusing. Could be commented in if desired
#The borders of the grid
#for z in range (1,4):
#	curve(pos=[(0.5,-0.5,z-0.5),(0.5,3.5,z-0.5)])
#	curve(pos=[(1.5,-0.5,z-0.5),(1.5,3.5,z-0.5)])
#	curve(pos=[(2.5,-0.5,z-0.5),(2.5,3.5,z-0.5)])
	
#for y in range (0,3):
#	curve(pos=[(0.5,y+0.5,-0.5),(0.5,y+0.5,4-0.5)])
#	curve(pos=[(1.5,y+0.5,-0.5),(1.5,y+0.5,4-0.5)])
#	curve(pos=[(2.5,y+0.5,-0.5),(2.5,y+0.5,4-0.5)])

#The alternative - four sqaures defining levels
#curve(pos=[(0,0,0),(0,0,3),(3,0,3),(3,0,0),(0,0,0)])
#curve(pos=[(0,1,0),(0,1,3),(3,1,3),(3,1,0),(0,1,0)])
#curve(pos=[(0,2,0),(0,2,3),(3,2,3),(3,2,0),(0,2,0)])
#curve(pos=[(0,3,0),(0,3,3),(3,3,3),(3,3,0),(0,3,0)])

#Draw the board (shutup, it's a draft ok)
(lambda spacing:(
[box(pos=(x,y,0), size=(THICKNESSS,THICKNESSS,BOARD_SIZE), color=GRID_COLOR) for x in spacing for y in spacing],
[box(pos=(x,0,z), size=(THICKNESSS,BOARD_SIZE,THICKNESSS), color=GRID_COLOR) for x in spacing for z in spacing],
[box(pos=(0,y,z), size=(BOARD_SIZE,THICKNESSS,THICKNESSS), color=GRID_COLOR) for y in spacing for z in spacing]))(
linspace(-BOARD_SIZE/2.0,BOARD_SIZE/2.0,BOARD_DIMENSION+1))




#Initialise the data list and the models list
for x in range(0,BOARD_DIMENSION):
	for y in range(0,BOARD_DIMENSION):
		for z in range(0,BOARD_DIMENSION):
			board_data.append(DATA_EMPTY)
			board_models.append( make_dot(pos=(x,y,z)) )
			

#Start the worker
worker = subprocess.Popen(['./worker'],stdin=subprocess.PIPE,stdout=subprocess.PIPE)
#Send worker the empty board

#Testing the communication
worker.stdin.write(serialize_board(board_data)+"\n")
worker_output = worker.stdout.readline()
print worker_output

board_data = list(worker_output)
update_models(board_data)





