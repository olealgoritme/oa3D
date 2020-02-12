#ifndef _MATH_UTIL_
#define _MATH_UTIL_

#define PI_PER_DEG (3.1415927f/180.0f)
#define DEG_PER_PI (180.0f/3.1415927f)

#include <cstdio>

// copy from Row-major to Column major matrix
// i.e. from aiMatrix4x4 to glm::mat4
template <typename RM, typename CM>
void CopyMat(const RM& from, CM& to)
{
	to[0][0] = from.a1; to[1][0] = from.a2;
	to[2][0] = from.a3; to[3][0] = from.a4;
	to[0][1] = from.b1; to[1][1] = from.b2;
	to[2][1] = from.b3; to[3][1] = from.b4;
	to[0][2] = from.c1; to[1][2] = from.c2;
	to[2][2] = from.c3; to[3][2] = from.c4;
	to[0][3] = from.d1; to[1][3] = from.d2;
	to[2][3] = from.d3; to[3][3] = from.d4;
}

template <typename T>
void printMat(T& mat, int size, unsigned char * name)
{
	printf("\n%s\n", name);
	for(int i=0; i< size; i++)
	{
		for (int j=0; j< size; j++)
			printf("%8.5f ", mat[j][i]);

		printf("\n");
	}
}

class Triplet
{
public:
	Triplet(const Triplet& t):x(t.x),y(t.y),z(t.z){};
	Triplet(float a=.0f, float b=.0f, float c=.0f):x(a),y(b),z(c){};
	float x, y, z;
};

template <typename T>
Triplet rot2eularZYX(T& m)
{
	float rx = atan2(m[1][2], m[2][2])*DEG_PER_PI;
	float c2 = sqrt(m[0][0]*m[0][0] + m[0][1]*m[0][1]);
	float ry = atan2(-m[0][2], c2)*DEG_PER_PI;
	float rz = atan2(m[0][1], m[0][0])*DEG_PER_PI;

	return Triplet(rz, ry, rx);
}

#endif
