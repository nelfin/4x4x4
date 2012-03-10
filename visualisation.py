from visual import *
import subprocess
import sys

board_dimension = 4;

data_empty='e';
data_naught='x';
data_cross='o';

board_data = list();
board_models = list();

#A function that combines the list into one big string we can send to the worker
def serialize_board(data):
	return ''.join(data);

#Translates a board x y z coordinate into an index for our lists
def coords_to_index(x,y,z):
	return z+y*(board_dimension)+x*(board_dimension*board_dimension);
	
def update_models(data):
	for x in range(0,board_dimension):
		for y in range(0,board_dimension):
			for z in range(0,board_dimension):
				state = data[coords_to_index(x,y,z)];
				
				#remove the old model
				board_models[coords_to_index(x,y,z)].visible = False;
				del board_models[coords_to_index(x,y,z)];
				
				if	 state=='e' :
					board_models.insert(coords_to_index(x,y,z), sphere 	(pos=(x,y,z), radius=0.02, color=color.white));
				elif state=='x' :
					board_models.insert(coords_to_index(x,y,z), curve 	(pos=[(x,y,z),(x-0.4,y,z-0.4),(x+0.4,y,z+0.4),(x,y,z),(x-0.4,y,z+0.4),(x+0.4,y,z-0.4)], color=color.green));
				elif state=='o' :
					board_models.insert(coords_to_index(x,y,z), ring	(pos=(x,y,z), axis=(0,1,0), radius=0.4, thickness=0.03, color=color.red));
				
				
	return;

#Removed the grid as it made the whole thing a lot more confusing. Could be commented in if desired
#The borders of the grid
#for z in range (1,4):
#	curve(pos=[(0.5,-0.5,z-0.5),(0.5,3.5,z-0.5)]);
#	curve(pos=[(1.5,-0.5,z-0.5),(1.5,3.5,z-0.5)]);
#	curve(pos=[(2.5,-0.5,z-0.5),(2.5,3.5,z-0.5)]);
	
#for y in range (0,3):
#	curve(pos=[(0.5,y+0.5,-0.5),(0.5,y+0.5,4-0.5)]);
#	curve(pos=[(1.5,y+0.5,-0.5),(1.5,y+0.5,4-0.5)]);
#	curve(pos=[(2.5,y+0.5,-0.5),(2.5,y+0.5,4-0.5)]);

#The alternative - four sqaures defining levels
curve(pos=[(0,0,0),(0,0,3),(3,0,3),(3,0,0),(0,0,0)]);
curve(pos=[(0,1,0),(0,1,3),(3,1,3),(3,1,0),(0,1,0)]);
curve(pos=[(0,2,0),(0,2,3),(3,2,3),(3,2,0),(0,2,0)]);
curve(pos=[(0,3,0),(0,3,3),(3,3,3),(3,3,0),(0,3,0)]);




#Initialise the data list and the models list
for x in range(0,board_dimension):
	for y in range(0,board_dimension):
		for z in range(0,board_dimension):
			board_data.append(data_empty);
			board_models.append( sphere (pos=(x,y,z), radius=0.02, color=color.white) );
			

#Start the worker
worker = subprocess.Popen(['./worker'],stdin=subprocess.PIPE,stdout=subprocess.PIPE);
#Send worker the empty board
#(worker_output,errors)=worker.communicate(input=serialize_board(board_data));

worker.stdin.write(serialize_board(board_data)+"\n");
worker_output = worker.stdout.readline();
print worker_output;
board_data = list(worker_output);
update_models(board_data);

#worker.stdin.write(serialize_board(board_data));
