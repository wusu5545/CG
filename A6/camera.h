// Put the names of all group members here 
//Su Wu         21762068
//Yixing Huang  21889201
//Xinyun Li     21884782
/// Implement the camera system entirely here, which means: don't do only the declaration here and
/// create another file camera.cpp for the implementation. You might have been told that it is
/// bad programming style to put everything in a .h file, but it's perfectly fine for such a small
/// class! Plus, I don't want to open two files to read your solutions of this class!

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>


class cameraSystem {

  public:

    glm::vec4 position;   // position-vector

    // orthonormal system of directions:
    glm::vec4 viewDir;    // viewing direction
    glm::vec4 upDir;      // up-vector
    glm::vec4 rightDir;   // right-vector (cross product of viewing- and up-direction)


    // initialize the system with values suitable for the underlying scene.
    cameraSystem() {
       // TODO
       position = glm::vec4(50,80,-160,1);
       viewDir = glm::vec4(glm::normalize(glm::vec3(-position.x,-position.y,-position.z)),1.0);//center = position + viewDir,center=(0,0,0);
       upDir = glm::vec4(0,1,0,1);
       rightDir = glm::vec4(glm::normalize(glm::cross(glm::vec3(viewDir),glm::vec3(upDir))),1.0);
    }

// move the system forward along the viewing direction
   void moveForward(float delta) {
    // TODO
    position = position + viewDir * delta;
   }

 //
    void moveBackward(float delta) {
    // TODO
    position = position - viewDir * delta;
   }

   void yaw(float angle) {
      // TODO
      // dont forget to update the other Vectors!!
      glm::mat4 rotationY = glm::rotate(glm::mat4(1.0),angle,glm::vec3(upDir));
      upDir = glm::normalize(rotationY * upDir);
      viewDir = glm::normalize(rotationY * viewDir);
      rightDir = glm::normalize(rotationY * rightDir);
   }


   void pitch(float angle) {
    // TODO
    // dont forget to update the other Vectors!!
      glm::mat4 rotationX =glm::rotate(glm::mat4(1.0),angle,glm::vec3(rightDir));
      upDir = glm::normalize(rotationX * upDir);
      viewDir = glm::normalize(rotationX * viewDir);
      rightDir = glm::normalize(rotationX * rightDir);
   }


   void roll(float angle) {
      // TODO
     // dont forget to update the other Vectors!!
     glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0),angle,glm::vec3(viewDir));
     upDir = glm::normalize(rotationZ * upDir);
     viewDir = glm::normalize(rotationZ * viewDir);
     rightDir = glm::normalize(rotationZ * rightDir);
   }

};
