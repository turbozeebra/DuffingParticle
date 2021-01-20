
uniform float u_Timer;
uniform vec4 u_Color;

void main(void) {
  
  gl_FragColor[0] = 0.5 + 0.4*cos(u_Timer);//0.1;
  gl_FragColor[1] = 0.5 + 0.3*sin(u_Timer);   //*u_Color.r;
  gl_FragColor[2] = u_Color.b;   //*abs(cos(u_Timer));
  gl_FragColor[3] = 1.0;  //floor(mod(gl_FragCoord.x, 100.0));

}

