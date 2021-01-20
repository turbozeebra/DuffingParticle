
#include <stdio.h>
#include <stdlib.h>
/* Use glew.h instead of gl.h to get all the GL prototypes declared */
#include <GL/glew.h>
/* Using the GLUT library for the base windowing setup */
#include <GL/freeglut.h>
#include "./common/shader_utils.h"
#include "./common/shader_utils.cpp"
//math library 
#include "../glm/glm.hpp"
#include "../glm/gtc/type_ptr.hpp"
// using chrono for the timer

#include <iostream>
#include <vector>
// our own little classes
#include "Particlesystem.h"
// camera class
#include "Camera.h"

const int PARTICLESIZE =9;
const int WIDTH = 600; 
const int HEIGHT = 500;

GLuint vbo_particles;
GLuint program;
GLint attribute_coord3d;
GLuint vs, fs; //vertex shader and fragment shader
GLfloat move_particles[PARTICLESIZE*3]; //*3 because we are in 3d

Particlesystem pSys;
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 4.0f));
float lastX = 0.5f*WIDTH;
float lastY = 0.5f*HEIGHT;
bool firstMouse = true;
bool mouseLeft = false;



void init_Particles();
float timeNow();
void setUniforms();
void move();
void keyboard(unsigned char key, int x, int y);
void mouse (int button, int state, int x, int y);


void init_Particles(){
  // lets start with 50 particles o'right?
  for(int i = 0; i < pSys.positions.size(); i++ ){
    
    move_particles[i*3]     = pSys.positions[i].x;
    move_particles[i*3 + 1] = pSys.positions[i].y;
    move_particles[i*3 + 2] = pSys.positions[i].z;
  
  }

}
//this happens only once 
int init_resources()
{
  
  pSys.init();
  
  init_Particles();
  
  glGenBuffers(1, &vbo_particles);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_particles);
  glBufferData(GL_ARRAY_BUFFER, sizeof(move_particles), move_particles, GL_STATIC_DRAW); // GL_STATIC_DRAW)
  
  glutInitContextProfile(GLUT_CORE_PROFILE | GLUT_COMPATIBILITY_PROFILE);

  /*
    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << glGetString(GL_VENDOR) << std::endl;
  */

  GLint link_ok = GL_FALSE;

 // vertex shader and fragment shader generation
  if ((vs = create_shader("src/shaders/triangle.v.glsl", GL_VERTEX_SHADER))   == 0) return 0;
  if ((fs = create_shader("src/shaders/triangle.f.glsl", GL_FRAGMENT_SHADER)) == 0) return 0;
  
  //create a program 
  program = glCreateProgram();
  //attach shaders to it
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &link_ok);

  if (!link_ok) {
    fprintf(stderr, "glLinkProgram:");
    print_log(program);
    return 0;
  }

  const char* attribute_name = "coord3d"; 
  attribute_coord3d = glGetAttribLocation(program, attribute_name);
  if (attribute_coord3d == -1) {
    fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
    return 0;
  }

  return 1;

}


void setUniforms(){
    //glUseProgram(program);
    
    //int loc  = glGetUniformLocation(vs,"u_Timer");
    int loc2 = glGetUniformLocation(program,"u_Timer");
    int loc3 = glGetUniformLocation(program,"u_Color");
    int loc4 = glGetUniformLocation(program,"u_View");
    int loc5 = glGetUniformLocation(program,"u_Projection");
    GLfloat t = pSys.timeNow();
    //camera/view transformation
    glm::mat4 view = camera.GetViewMatrix();
    
    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);    
   
    glUniform1f(loc2, t);
    glUniform4f(loc3, 0.5, 0.5, 1.0, 1.0);
    glUniformMatrix4fv(loc4, 1, GL_FALSE, glm::value_ptr(view)); // view matrix
    glUniformMatrix4fv(loc5, 1, GL_FALSE, glm::value_ptr(projection)); // projection matrix

    
}
void move() {
  pSys.move(0.001);
  
  for(int i = 0; i < pSys.positions.size(); i++ ){
    
    move_particles[i*3]     = pSys.positions[i].x;
    move_particles[i*3 + 1] = pSys.positions[i].y;
    move_particles[i*3 + 2] = pSys.positions[i].z;
    
  }
}

void onDisplay()
{

  move();
  setUniforms();
  glClearColor(0.f, 0.f, 0.f, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(program);
  glEnableVertexAttribArray(attribute_coord3d);
  /* Describe our vertices array to OpenGL (it can't guess its format automatically) */
  glBindBuffer(GL_ARRAY_BUFFER, vbo_particles);
  glVertexAttribPointer(
    attribute_coord3d, // attribute
    3,                 // number of elements per vertex, here (x,y,z)
    GL_FLOAT,          // the type of each element
    GL_FALSE,          // take our values as-is
    0,                 // no extra data between each position
    0                  // offset of first element
  );
  /* Push each element in buffer_vertices to the vertex shader */
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(move_particles), &move_particles);
  glDrawArrays(GL_POINTS, 0, PARTICLESIZE); // glDrawArrays(GL_TRIANGLES, 0, 3);
  //here it changes the buffer or uploads the new values to the old buffer

  glDisableVertexAttribArray(attribute_coord3d);
  glutSwapBuffers();

  
  
}

void free_resources()
{
  glDeleteProgram(program);
  glDeleteBuffers(1, &vbo_particles);
}

void timer(int)
{
  unsigned int fps = 1000/60; //60 fps
  glutPostRedisplay();
  glutTimerFunc(fps, timer,0);
}

void keyboard(unsigned char key, int x, int y) {
  //Camera_Movement::RIGHT
  switch(key) {
    case 'a' : camera.ProcessKeyboard(Camera_Movement::LEFT,0.01f); 
             break;       
    case 'd' : camera.ProcessKeyboard(Camera_Movement::RIGHT,0.01f); 
             break;
    case 'w' : camera.ProcessKeyboard(Camera_Movement::FORWARD,0.01f); 
             break;
    case 's' : camera.ProcessKeyboard(Camera_Movement::BACKWARD,0.01f); 
             break;         
  }
}

void mouse (int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON){// } && state == GLUT_DOWN){
    mouseLeft = true;
    if( state == GLUT_UP){
      firstMouse = true;
  }
  } else {
    mouseLeft = false;
  }
  
}

void motion (int x, int y) {
  if(mouseLeft){ 
    if (firstMouse)
    {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }
  
    float xoffset = x - lastX;
    float yoffset = lastY - y; 
    lastX = x;
    lastY = y;
    camera.ProcessMouseMovement(xoffset,yoffset);
   
  }
  
}


int main(int argc, char* argv[])
{
    
    glutInit(&argc, argv);
    glutInitContextVersion(2,0);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE); // |GLUT_DEPTH|GLUT_ALPHA
    glutInitWindowSize(WIDTH , HEIGHT); //
    glutCreateWindow("Test program");

    GLenum glew_status = glewInit();
    if (glew_status != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
        return 1;
    }

    if (!GLEW_VERSION_2_0) {
        fprintf(stderr, "Error: your graphic card does not support OpenGL 2.0\n");
        return 1;
    }
     
    if (init_resources()) {
        
        glutInitDisplayMode(GLUT_RGBA);
        glutDisplayFunc(onDisplay);
        glutKeyboardFunc(keyboard);
        glutMouseFunc(mouse);
        glutMotionFunc(motion);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glutTimerFunc(0,timer, 0);
        glutMainLoop();
    }
    
    free_resources();
    return 0;
}
/*


void init_Particles(){
  //five rows of 10 particles
  float incX = (3.0/30.0);pSys.init();
  float incY = (3.0/25.0);
  float startX = -0.5; 
  float startY = 0.0;
  for(int i = 0; i < 5; i++){
    for(int j = 0; j < 10; j++) {
      particles[j*2 + i*20] = startX + j*incX;
      particles[j*2 + 1 + i*20] = startY + i*incY;

    }
  }

}


*/