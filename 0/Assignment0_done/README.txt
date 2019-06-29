Shaun Toh Shou'en 
README
Arguments: 
	<< <obj File>
	example: << Torus.obj
	
Extra Implemented Features
Rotation on request, by pressing R. Glut timer function is not used. Instead a redraw is requested again after each draw.

Resources Used:
https://www.khronos.org/opengl/wiki/ 

Bugs:
No real bugs other than the lack of using the glut timer function. glutPostRedisplay(); is called because it is a scheduling flag, marking a window for redisplay, not actually calling a redraw on the spot.


