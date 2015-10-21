#include "helper.h"

class cameraSystem {
public:
	vec3 position;	// position-vector
	
	// orthonormal system of directions:
	vec3 viewDir;	// viewing direction
	vec3 upDir;		// up-vector
	vec3 rightDir;	// right-vector (cross product of viewing- and up-direction)

	// initialize the system with values suitable for the underlying scene.
	cameraSystem() {
		position = vec3(50.f,80.f,-160.f);  
		viewDir = vec3(-0.240947f, -0.428651f, 0.870756f);
		upDir = vec3(-0.0848183f, 0.903053f, 0.421079f);
		rightDir = vec3(-0.96683f, 0.0276017f, -0.253944f);
	}
   
	// move the system forward along the viewing direction	 
	void moveForward(float delta) {
		position = position + (delta*viewDir);
	}

	void moveBackward(float delta) {
		position = position - (viewDir*delta);
	}

	void yaw(float angle) {
		rotMat3x3 R(upDir,angle);
		viewDir = R*viewDir;
		rightDir = R*rightDir;
	}

void pitch(float angle) {
	 rotMat3x3 R(rightDir,angle);
	 viewDir = R*viewDir;
	 upDir = R*upDir;
	}


	void roll(float angle) {
		rotMat3x3 R(viewDir,angle);
		rightDir = R*rightDir;
		upDir = R*upDir;
	}

};

