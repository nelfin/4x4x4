#! /usr/bin/python

from visual import *

GRID_COLOR = color.blue
NOUGHTS_COLOR = color.green
CROSSES_COLOR = color.red
DIMENSIONS = 7
GAP_SIZE   = 10
BOARD_SIZE = GAP_SIZE * DIMENSIONS
THICKNESSS = 0.5
NOUGHTS_RADIUS = GAP_SIZE/3.0
CROSSES_SIZE = (GAP_SIZE,1,1)
CENTRE_OFFSET = vector(GAP_SIZE/2.0,GAP_SIZE/2.0,GAP_SIZE/2.0) - vector(BOARD_SIZE/2.0,BOARD_SIZE/2.0,BOARD_SIZE/2.0)

#Draw a sphere for the noughts player.
def draw_noughts(pos, color=NOUGHTS_COLOR, radius=NOUGHTS_RADIUS):
    sphere(pos=vector(pos)*GAP_SIZE+CENTRE_OFFSET, color=color, radius=radius)

#Draw an X for the crosses player.
def draw_cross(pos, color=CROSSES_COLOR, size=CROSSES_SIZE):
    box(pos=vector(pos)*GAP_SIZE+CENTRE_OFFSET, axis=(1,1,1), color=color, size=size)
    box(pos=vector(pos)*GAP_SIZE+CENTRE_OFFSET, axis=(-1,1,-1), color=color, size=size)

#Draw the board (shutup, it's a draft ok)
(lambda spacing:(
[box(pos=(x,y,0), size=(THICKNESSS,THICKNESSS,BOARD_SIZE), color=GRID_COLOR) for x in spacing for y in spacing],
[box(pos=(x,0,z), size=(THICKNESSS,BOARD_SIZE,THICKNESSS), color=GRID_COLOR) for x in spacing for z in spacing],
[box(pos=(0,y,z), size=(BOARD_SIZE,THICKNESSS,THICKNESSS), color=GRID_COLOR) for y in spacing for z in spacing]))(
linspace(-BOARD_SIZE/2.0,BOARD_SIZE/2.0,DIMENSIONS+1))

for pos in [(x,y,z) for x in xrange(0, DIMENSIONS, 2) for y in xrange(DIMENSIONS) for z in xrange(DIMENSIONS)]:
    rate(800)
    draw_cross(pos)

for pos in [(x,y,z) for x in xrange(1, DIMENSIONS, 2) for y in xrange(DIMENSIONS) for z in xrange(DIMENSIONS)]:
    rate(800)
    draw_noughts(pos)
