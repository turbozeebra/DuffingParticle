attribute vec3 coord3d;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main(void) {
  
  gl_Position = u_Projection * u_View * vec4(coord3d, 1.0);

}
