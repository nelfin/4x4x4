from visual import *

board_dimension = 4;

data_empty='e';
data_naught='x';
data_cross='o';

#TODO: this assumes grid size 4
#The borders of the grid
#for z in range (1,4):
#	curve(pos=[(0.5,-0.5,z-0.5),(0.5,3.5,z-0.5)]);
#	curve(pos=[(1.5,-0.5,z-0.5),(1.5,3.5,z-0.5)]);
#	curve(pos=[(2.5,-0.5,z-0.5),(2.5,3.5,z-0.5)]);
	
#for y in range (0,3):
#	curve(pos=[(0.5,y+0.5,-0.5),(0.5,y+0.5,4-0.5)]);
#	curve(pos=[(1.5,y+0.5,-0.5),(1.5,y+0.5,4-0.5)]);
#	curve(pos=[(2.5,y+0.5,-0.5),(2.5,y+0.5,4-0.5)]);
curve(pos=[(0,0,0),(0,0,4),(4,0,4),(4,0,0),(0,0,0)]);
curve(pos=[(0,1,0),(0,1,4),(4,1,4),(4,1,0),(0,1,0)]);
curve(pos=[(0,2,0),(0,2,4),(4,2,4),(4,2,0),(0,2,0)]);
curve(pos=[(0,3,0),(0,3,4),(4,3,4),(4,3,0),(0,3,0)]);

board_data = list();
board_models = list();
for x in range(0,board_dimension):
	for y in range(0,board_dimension):
		for z in range(0,board_dimension):
			board_data.append(data_empty);
			board_models.append( sphere (pos=(x,y,z), radius=0.02, color=color.white) );
