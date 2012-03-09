import subprocess

visualisation = subprocess.Popen(['python','visualisation.py'],stdin=subprocess.PIPE);
worker = subprocess.Popen(['./worker'],stdin=subprocess.PIPE,stdout=subprocess.PIPE);

visualisation.communicate("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee");

#(output,errors)=worker.communicate(input="test\n");

print output;


