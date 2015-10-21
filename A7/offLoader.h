// Put the names of all group members here 
//Su Wu         21762068
//Yixing Huang  21889201
//Xinyun Li     21884782
#include <fstream>
#include <vector>

#include <glm/glm.hpp>
#include "helper.h"

using namespace std;

// simple structure to hold three integer-values (used for triangle indicies)
struct tri {
  int A;
  int B;
  int C;
};


// Declaration:

class OffObject 
{

public: 
	
  	vector<glm::vec3> vertexList;
	vector<glm::vec3> normalsList;
	vector<tri> faceList;
	
	int noOfFaces;
	int noOfVertices;
	
	OffObject(string filename);
	
};



//TODO: IMPLEMENTATION:

// the constuctor recieves the filename (an .off file) and parses it. The vertices, normals and triangles
// are pushed back into the respective container whereas the NORMALS have to be explicitly computed for each
// vertex. 

OffObject::OffObject(string filename) {
 
	std::ifstream inFile(filename.c_str());
	char tmp[20];

	inFile >> tmp;
	inFile >> noOfVertices;
	inFile >> noOfFaces;
	inFile >> tmp;
	
	//printf("%s, %d, %d, %s \n", tmp, noOfVertices, noOfFaces, tmp);
	// Read Vertex Data and initialize the normals:	
    for (int i=0; i<noOfVertices; i++) 
	{
		glm::vec3 vertex;
        // TODO

        // initalize the normal with (0,0,0)
		glm::vec3 n(0,0,0);
		// add vertex and normal
		float X, Y ,Z;
		inFile >> X;
		inFile >> Y;
		inFile >> Z;
		//printf("%f, %f, %f \n",X,Y,Z);
		vertexList.push_back(glm::vec3(X,Y,Z));
		normalsList.push_back(n);
	}


	// Read Triangle Data:
	tri T;
    for (int i=0; i<noOfFaces; i++) 
	{
        // TODO
        // probably helpful: glm::cross(..,..);	//CHECK DOCUMENTATION!!!
		int NV,A,B,C;
		inFile >> NV;
		inFile >> A;
		inFile >> B;
		inFile >> C;
		
		normalsList[A] += glm::normalize(glm::cross(vertexList[B]-vertexList[A],vertexList[C]-vertexList[A]));
		normalsList[B] += glm::normalize(glm::cross(vertexList[C]-vertexList[B],vertexList[A]-vertexList[B]));
		normalsList[C] += glm::normalize(glm::cross(vertexList[A]-vertexList[C],vertexList[B]-vertexList[C]));
		T.A = A;
		T.B = B;
		T.C = C;
		faceList.push_back(T);
	}
    
    //normalize:
    for (int i=0; i<noOfVertices; i++) 
	{
        // TODO
        // probably helpful: glm::normalize(..);	//CHECK DOCUMENTATION!!!
		//normalsList[i] = glm::normalize(glm::cross(b-a,c-a));
		normalsList[i] = glm::normalize(normalsList[i]);
		//printf("%f, %f, %f \n",normalsList[i].x,normalsList[i].y,normalsList[i].z);
	}      


}

