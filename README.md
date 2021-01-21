

This is a Duffing Equation simulation. For me the point of this program 
is to get used to setup openGL and glsl and to get familiar with simulating
oscillation with interresting iterators.

As integrator I use RK4 interator. In the particle system class the Fv is
the force from Duffing Equation and the other force Fa is just a force that
is used because I didn't want that all the points collaps to one point.

The camera works with left-click and mouse motion and with WASD buttons

Some development ideas for this project:
- implement 2NRK4 - iterator
- implement some FMM to the N-body problem 
- make it 3D


to run this program (You need to be in the folder and you have to have glew and glut installed):
g++ src/main.cpp src/App.cpp  -o run -lGLEW -lglut -lGL
./smallParticleSim
 

// equations are from: 
// http://www.cfm.brown.edu/people/dobrush/am34/Mathematica/ch3/duffing.html
// RK4 algorithm:
// https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods
