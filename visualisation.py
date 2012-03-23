#!/usr/bin/python
# vim: set ts=4 noet:
import subprocess
import sys
import time
import atexit
import os
import signal

#Start the worker
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
DATA_CROSS='x'
DATA_NOUGHT='o'

#Drawing constants
GRID_COLOR = color.white
NOUGHTS_COLOR = color.green
CROSSES_COLOR = color.red
DOT_COLOR = color.white
VICTORY_COLOR = color.yellow

COMPUTER_GAME_OVER_TEXT = "Computer (X) won!"
PLAYER_GAME_OVER_TEXT = "Player (O) won!"

BOARD_DIMENSION = 4
MAX_COORDINATE = BOARD_DIMENSION-1
GAP_SIZE = 10
BOARD_SIZE = GAP_SIZE * BOARD_DIMENSION
THICKNESS = 0.25
NOUGHTS_RADIUS = GAP_SIZE/3.0

CROSSES_SIZE = (GAP_SIZE,1,1)
CENTRE_OFFSET = vector(GAP_SIZE/2.0,GAP_SIZE/2.0,GAP_SIZE/2.0) - vector(BOARD_SIZE/2.0,BOARD_SIZE/2.0,BOARD_SIZE/2.0)

board_data = list()
board_models = list()

grid_visible = True
dots_visible = False

scene.stereo = 'redcyan'

#Sends the worker a message and waits for it's reply
def send_worker_message(message):
    worker.stdin.write(message+"\n")
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
def make_axis():
	axis_offset = GAP_SIZE
	axis_position = (-0.5*BOARD_SIZE-axis_offset,-0.5*BOARD_SIZE,-0.5*BOARD_SIZE-axis_offset)
	y_label_position = (-0.5*BOARD_SIZE-axis_offset,0,-0.5*BOARD_SIZE-axis_offset)
	x_label_position = (0-axis_offset,-0.5*BOARD_SIZE,-0.5*BOARD_SIZE-axis_offset)
	z_label_position = (-0.5*BOARD_SIZE-axis_offset,-0.5*BOARD_SIZE,0-axis_offset)
	label(pos=x_label_position, text='X axis',box=False,height=10,color=color.red)
	label(pos=y_label_position, text='Y axis',box=False,height=10,color=color.orange)
	label(pos=z_label_position, text='Z axis',box=False,height=10,color=color.yellow)
	arrow(pos=axis_position, axis=(BOARD_SIZE/2,0,0), shaftwidth=0.1,color=color.red)
	arrow(pos=axis_position, axis=(0,BOARD_SIZE/2,0), shaftwidth=0.1,color=color.orange)
	arrow(pos=axis_position, axis=(0,0,BOARD_SIZE/2), shaftwidth=0.1,color=color.yellow)
     
#Draw a victory line from start to end
def make_victory(start, end, color=VICTORY_COLOR, radius=THICKNESS*2):
    start = (vector(start)*GAP_SIZE+CENTRE_OFFSET)
    end   = (vector(end  )*GAP_SIZE+CENTRE_OFFSET)
    curve(pos=[start, end], radius=radius, color=color)

#Determines if the game is over, and if so, where the victory line is
def game_over(data):

    #All vertical victory lines (Y)
    for x in range(0,BOARD_DIMENSION):
        for z in range(0,BOARD_DIMENSION):
            possible_victor = data[coords_to_index(x,0,z)]
            vertical_victory = True
            if possible_victor==DATA_EMPTY:
                continue
            for y in range(1,BOARD_DIMENSION):
                vertical_victory = vertical_victory and (data[coords_to_index(x,y,z)]==possible_victor)
            if vertical_victory:
                return (True,(x,0,z),(x,MAX_COORDINATE,z))
                
    #All horizontal victory lines (X)
    for x in range(0,BOARD_DIMENSION):
        for y in range(0,BOARD_DIMENSION):
            possible_victor = data[coords_to_index(x,y,0)]
            horizontal_victory = True
            if possible_victor==DATA_EMPTY:
                continue
            for z in range(1,BOARD_DIMENSION):
                horizontal_victory = horizontal_victory and (data[coords_to_index(x,y,z)]==possible_victor)
            if horizontal_victory:
                return (True,(x,y,0),(x,y,MAX_COORDINATE))
                
    #All depth victory lines (Z)
    for y in range(0,BOARD_DIMENSION):
        for z in range(0,BOARD_DIMENSION):
            possible_victor = data[coords_to_index(0,y,z)]
            depth_victory = True
            if possible_victor==DATA_EMPTY:
                continue
            for x in range(1,BOARD_DIMENSION):
                horizontal_victory = horizontal_victory and (data[coords_to_index(x,y,z)]==possible_victor)
            if horizontal_victory:
                return (True,(0,y,z),(MAX_COORDINATE,y,z))
                
    #All Z-slice cross lines
    for z in range(0,BOARD_DIMENSION):
        cross_victory_1 = True
        cross_victory_2 = True
        possible_victor_1 = data[coords_to_index(0,0,z)]
        possible_victor_2 = data[coords_to_index(0,MAX_COORDINATE,z)]
        if possible_victor_1==DATA_EMPTY or possible_victor_2==DATA_EMPTY:
            continue
        for xy in range(1,BOARD_DIMENSION):
                cross_victory_1 = cross_victory_1 and (data[coords_to_index(xy,xy,z)]==possible_victor_1)
                cross_victory_2 = cross_victory_2 and (data[coords_to_index(xy,MAX_COORDINATE-xy,z)]==possible_victor_2)
        if cross_victory_1:
            return (True,(0,0,z),(MAX_COORDINATE,MAX_COORDINATE,z))
        if cross_victory_2:
            return (True,(MAX_COORDINATE,0,z),(MAX_COORDINATE,0,z))
                
    #All X-slice cross lines
    for x in range(0,BOARD_DIMENSION):
        cross_victory_1 = True
        cross_victory_2 = True
        possible_victor_1 = data[coords_to_index(x,0,0)]
        possible_victor_2 = data[coords_to_index(x,0,MAX_COORDINATE)]
        if possible_victor_1==DATA_EMPTY or possible_victor_2==DATA_EMPTY:
            continue
        for yz in range(1,BOARD_DIMENSION):
                cross_victory_1 = cross_victory_1 and (data[coords_to_index(x,yz,yz)]==possible_victor_1)
                cross_victory_2 = cross_victory_2 and (data[coords_to_index(x,yz,MAX_COORDINATE-yz)]==possible_victor_2)
        if cross_victory_1:
            return (True,(x,0,0),(x,MAX_COORDINATE,MAX_COORDINATE))
        if cross_victory_2:
            return (True,(x,MAX_COORDINATE,0),(x,MAX_COORDINATE,0))
                
    #All Y-slice cross lines
    for y in range(0,BOARD_DIMENSION):
        cross_victory_1 = True
        cross_victory_2 = True
        possible_victor_1 = data[coords_to_index(0,y,0)]
        possible_victor_2 = data[coords_to_index(MAX_COORDINATE,y,0)]
        if possible_victor_1==DATA_EMPTY or possible_victor_2==DATA_EMPTY:
            continue
        for xz in range(1,BOARD_DIMENSION):
                cross_victory_1 = cross_victory_1 and (data[coords_to_index(xz,y,xz)]==possible_victor_1)
                cross_victory_2 = cross_victory_2 and (data[coords_to_index(MAX_COORDINATE-xz,y,xz)]==possible_victor_2)
        if cross_victory_1:
            return (True,(0,0,z),(MAX_COORDINATE,MAX_COORDINATE,z))
        if cross_victory_2:
            return (True,(MAX_COORDINATE,0,z),(0,MAX_COORDINATE,z))
    #The 4 corner diagonals
    corner_1_victory = True
    corner_1_mark = data[coords_to_index(0,0,0)]
    corner_2_victory = True
    corner_2_mark = data[coords_to_index(MAX_COORDINATE,0,0)]
    corner_3_victory = True
    corner_3_mark = data[coords_to_index(0,MAX_COORDINATE,0)]
    corner_4_victory = True
    corner_4_mark = data[coords_to_index(MAX_COORDINATE,MAX_COORDINATE,0)]
    for vary in range(0,BOARD_DIMENSION):
        corner_1_victory = corner_1_victory and (data[coords_to_index(vary,vary,vary)] == corner_1_mark)
        corner_2_victory = corner_2_victory and (data[coords_to_index(MAX_COORDINATE-vary,vary,vary)] == corner_2_mark)
        corner_3_victory = corner_3_victory and (data[coords_to_index(vary,MAX_COORDINATE-vary,vary)] == corner_3_mark)
        corner_4_victory = corner_4_victory and (data[coords_to_index(MAX_COORDINATE-vary,MAX_COORDINATE-vary,vary)] == corner_4_mark)
    if corner_1_victory and (corner_1_mark != DATA_EMPTY):
        return (True,(0,0,0),(MAX_COORDINATE,MAX_COORDINATE,MAX_COORDINATE))
    if corner_2_victory and (corner_2_mark != DATA_EMPTY):
        return (True,(MAX_COORDINATE,0,0),(MAX_COORDINATE,MAX_COORDINATE,MAX_COORDINATE))
    if corner_3_victory and (corner_3_mark != DATA_EMPTY):
        return (True,(0,MAX_COORDINATE,0),(MAX_COORDINATE,0,MAX_COORDINATE))
    if corner_4_victory and (corner_4_mark != DATA_EMPTY):
        return (True,(MAX_COORDINATE,MAX_COORDINATE,0),(0,0,MAX_COORDINATE))
    #Game is not over
    return (False,(0,0,0),(0,0,0))
                
#Handles a possible game-over scenario
def check_game_over(game_over_message):
    (over, start_index, end_index) = game_over(board_data)
    if over:
        make_victory(start_index,end_index)
        print "[visual]",game_over_message
        raw_input("Press enter to exit")
        sys.exit(0)

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
                
                if state == DATA_EMPTY:
                    board_models.insert(coords_to_index(x,y,z), make_dot(pos=(x,y,z)) )
                elif state == DATA_CROSS:
                    board_models.insert(coords_to_index(x,y,z), make_cross(pos=(x,y,z)) )
                elif state == DATA_NOUGHT:
                    board_models.insert(coords_to_index(x,y,z), make_nought(pos=(x,y,z)) )
                
                
    return
    
def place((x, y, z), mark):
    board_data[coords_to_index(x, y, z)] = mark
    
#Draw the grid
grid = frame()
(lambda spacing:(
[box(frame=grid,pos=(x,y,0), size=(THICKNESS,THICKNESS,BOARD_SIZE), color=GRID_COLOR) for x in spacing for y in spacing],
[box(frame=grid,pos=(x,0,z), size=(THICKNESS,BOARD_SIZE,THICKNESS), color=GRID_COLOR) for x in spacing for z in spacing],
[box(frame=grid,pos=(0,y,z), size=(BOARD_SIZE,THICKNESS,THICKNESS), color=GRID_COLOR) for y in spacing for z in spacing]))(
linspace(-BOARD_SIZE/2.0,BOARD_SIZE/2.0,BOARD_DIMENSION+1))
#Draw the axis
make_axis()
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
    try:
        #Keyboard interaction
        if scene.kb.keys:
            s = scene.kb.getkey()
            if s=='g':
                grid_visible = not grid_visible
                dots_visible = not dots_visible
        
                grid.visible = grid_visible
                squares.visible = dots_visible
                update_models(board_data);
            if s == 'q':
                sys.exit()


        rate(FPS);
        
        
        while True:
            cmd = raw_input("[visual] Enter space-separated coordinates>>> ")
            if cmd.startswith("q"):
                sys.exit()
            try:
                x, y, z = map(int, cmd.split())
            except ValueError:
                print "[visual] Please enter 3 values"
                continue
            if board_data[coords_to_index(x, y, z)] == DATA_EMPTY:
                place((x, y, z), DATA_NOUGHT)
                break
        
        update_models(board_data)
        check_game_over(PLAYER_GAME_OVER_TEXT)
    
        #Input loop
        reply = send_worker_message(" ".join(map(str, [x, y, z])))
        print "[visual] read board state:",reply," from worker\n"
        board_data = list(reply)
        update_models(board_data)
        check_game_over(COMPUTER_GAME_OVER_TEXT)

    except KeyboardInterrupt:
        worker.kill()
        break
