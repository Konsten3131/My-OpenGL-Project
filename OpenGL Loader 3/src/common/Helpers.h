#ifndef __Helpers_H__
#define __Helpers_H__

void LogError(char* string, ...);

bool CheckForGLErrors(const char* file = __FILE__, int line = __LINE__);

char* LoadCompleteFile(const char* filename, const char* extension);
double MyGetSystemTime();

void GetMouseCoordinates(int* mx, int* my);

bool loadOBJ(const char * path,
             std::vector<Vector3> & out_vertices,
             std::vector<Vector2> & out_uvs,
             std::vector<Vector3> & out_normals);

void findModelsDimension(const std::vector<Vector3> & vertices, Vector3 & out_Dimensions);
void getCameraRotCoords(Vector3 & outCords,float angle, float radius);
float degrees2Radians(float angle);
float lerp(float value1, float value2, float amount);


#endif //__Helpers_H__
