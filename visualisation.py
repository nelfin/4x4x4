#!/usr/bin/python
# vim: set ts=4 noet:
import subprocess
import sys
import time
import atexit
import os
import signal

worker = subprocess.Popen(['./worker'],stdin=subprocess.PIPE,stdout=subprocess.PIPE)

from visual import *

@atexit.register
def shutdown():
	global worker
	print "[visual] shutting down"
	worker.kill()
	os.kill(os.getpid(), signal.SIGKILL)
#atexit.register(shutdown)

FPS = 60

DATA_EMPTY='e'
DATA_NOUGHT='x'
DATA_CROSS='o'



#Drawing constants
GRID_COLOR = color.white
NOUGHTS_COLOR = color.green
CROSSES_COLOR = color.red
DOT_COLOR = color.white

BOARD_DIMENSION = 4
GAP_SIZE = 10
BOARD_SIZE = GAP_SIZE * BOARD_DIMENSION
THICKNESSS = 0.25
NOUGHTS_RADIUS = GAP_SIZE/3.0

CROSSES_SIZE = (GAP_SIZE,1,1)
CENTRE_OFFSET = vector(GAP_SIZE/2.0,GAP_SIZE/2.0,GAP_SIZE/2.0) - vector(BOARD_SIZE/2.0,BOARD_SIZE/2.0,BOARD_SIZE/2.0)

board_data = list()
board_models = list()

grid_visible = True
dots_visible = False

#Sends the worker a message and waits for it's reply
def send_worker_message(message):
	worker.stdin.write(serialize_board(board_data)+"\n")
	worker_output = worker.stdout.read(BOARD_DIMENSION ** 3)
	return worker_output

#Draw a sphere for the noughts player.
def make_nought(pos, color=NOUGHTS_COLOR, radius=NOUGHTS_RADIUS):
	nought = sphere(pos=vector(pos)*GAP_SIZE+CENTRE_OFFSET, color=color, radius=radius)
	return nought

#Draw a dot for an empty space.
def make_dot(pos, color=DOT_COLOR, radius=NOUGHTS_RADIUS/10.0):
	dot = make_nought(pos,color,radius)
	dot.visible = dots_visible
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

#Refreshes model list to reflect board_data
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

	
#Draw the grid
grid = frame()
(lambda spacing:(
[box(frame=grid,pos=(x,y,0), size=(THICKNESSS,THICKNESSS,BOARD_SIZE), color=GRID_COLOR) for x in spacing for y in spacing],
[box(frame=grid,pos=(x,0,z), size=(THICKNESSS,BOARD_SIZE,THICKNESSS), color=GRID_COLOR) for x in spacing for z in spacing],
[box(frame=grid,pos=(0,y,z), size=(BOARD_SIZE,THICKNESSS,THICKNESSS), color=GRID_COLOR) for y in spacing for z in spacing]))(
linspace(-BOARD_SIZE/2.0,BOARD_SIZE/2.0,BOARD_DIMENSION+1))

#Also set up the squares format that we can switch to later
squares = frame()
for level in range(0,BOARD_DIMENSION):
	y = level * GAP_SIZE
	curve(frame=squares,pos=[(-BOARD_SIZE/2.0,y-BOARD_SIZE/2.0,-BOARD_SIZE/2.0),(BOARD_SIZE/2.0,y-BOARD_SIZE/2.0,-BOARD_SIZE/2.0),(BOARD_SIZE/2.0,y-BOARD_SIZE/2.0,BOARD_SIZE/2.0),(-BOARD_SIZE/2.0,y-BOARD_SIZE/2.0,BOARD_SIZE/2.0),(-BOARD_SIZE/2.0,y-BOARD_SIZE/2.0,-BOARD_SIZE/2.0)])

squares.visible = dots_visible; #only showing the squares with the dots



#Initialise the data list and the models list
for x in range(0,BOARD_DIMENSION):
	for y in range(0,BOARD_DIMENSION):
		for z in range(0,BOARD_DIMENSION):
			board_data.append(DATA_EMPTY)
			board_models.append( make_dot(pos=(x,y,z)) )
			

#The main loop
while True:
	#Keyboard interaction
	if scene.kb.keys:
		s = scene.kb.getkey()
		if s=="g":
			grid_visible = not grid_visible
			dots_visible = not dots_visible
	
			grid.visible = grid_visible
			squares.visible = dots_visible	
			update_models(board_data);
		if s == 'q':
			sys.exit()
	rate(FPS);

	#Input loop
	reply = send_worker_message(serialize_board(board_data))
	print "[visual] read", reply, "from worker"

	board_data = list(reply)
	update_models(board_data)

