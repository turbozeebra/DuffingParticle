#include "../glm/glm.hpp"
#include <vector>
#include <chrono> 

class Particlesystem {
    int systemSize;
    public:
    Particlesystem(int size);
    glm::vec3 Fa(glm::vec3 v1, glm::vec3 v2);
    glm::vec3 Fv(float t, glm::vec3 vec);
    void init();
    void move(float step);
    std::vector<glm::vec3>  calcFa();
    void RK4(float h);
    float timeNow();
    int getSystemSize();
    int getSystemSizeTimestwo();
    float delta, alpha, beta, gamma, omega, t;

    std::vector<glm::vec3> positions; 

    const float MASS = 0.01;

    //timer
    std::chrono::_V2::system_clock::time_point begin;
    

    //void eulerstep();
 

   
    

 };
