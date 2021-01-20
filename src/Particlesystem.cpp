// equations are from: 
// http://www.cfm.brown.edu/people/dobrush/am34/Mathematica/ch3/duffing.html
// RK4 algorithm:
// https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods

#include "Particlesystem.h"
#include <iostream>

//value of rhoo(sigma), rhoo is a vectorfunction

// a as arbitrary 
// this force is just that the particles wouldnt be stuck together
inline glm::vec3 Particlesystem::Fa(glm::vec3 v1, glm::vec3 v2) { 
    glm::vec3 r = (v2 - v1);
    float r3 = r.length();
    r = glm::normalize(r);
    glm::vec3 retVec = (40.f/r3) * r;
    return retVec;
}

inline glm::vec3 Particlesystem::Fv(float t, glm::vec3 vec){
    glm::vec3 retVec ;
    retVec.y = retVec.y -delta * vec.y - beta*vec.x*vec.x*vec.x - alpha*vec.x +  gamma*std::cos(t*omega);
    retVec.x = t*vec.y;
    retVec.z = 0;//std::cos(t);
    return retVec;
}


float Particlesystem::timeNow(){
  auto uNow = std::chrono::high_resolution_clock::now();
  std::chrono::duration<float> time = uNow - begin;
  return time.count();
}

void Particlesystem::init() {
    //setup the timer
    begin = std::chrono::high_resolution_clock::now();    
    t = 0;
    
    alpha = 1.;
    beta  = 4.;
    delta = 0.2;
    gamma = 9.0;
    omega = 0.5;

    for (int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++) 
        {
            glm::vec3 pos(-0.7 + j*0.4, -0.7 + i*0.4, 1.0);
            positions.push_back(pos);
            velocity.push_back(glm::vec3(0.f,0.f,0.f));
        }
    }
}

std::vector<glm::vec3>  Particlesystem::calcFa() {
    
    std::vector<glm::vec3> FaVec(PARTICL);
    // its n-body problem 
    for(int j = 0; j < PARTICL; j++) {
        for(int i = 0; i < PARTICL; i++) {
        // atleast it doesen't go through every particle  
            if(i > j) {
                glm::vec3 ff = Fa(positions[j], positions[i]);
                FaVec[j] -= ff;
                FaVec[i] += ff;
            }
        }
    }
    std::cout << FaVec[0].x << ",  " << FaVec[0].y << ", " << FaVec[0].z << std::endl;
    return FaVec;
}

void Particlesystem::RK4(float h) {
     
    auto fa = calcFa();
    for (int i = 0u; i < positions.size(); ++i) {
        glm::vec3 k1,k2,k3,k4;// = positions[i];
        k1 = Fv(t,positions[i]) + fa[i];
        k2 = Fv(t + h*0.5, positions[i] + 0.5f*h*k1) + 0.5f*h*fa[i];
        k3 = Fv(t + h*0.5, positions[i] + 0.5f*h*k1) + 0.5f*h*fa[i];
        k4 = Fv(t + h, positions[i] + k3) + h*fa[i];
        positions[i] = positions[i] + 0.1666666666f*h*(k1 + k2 + k3 + k4);
        t += h;
        
    }


}
void Particlesystem::move(float step) {
    
    RK4(step);

    std::cout << t << std::endl;
}


