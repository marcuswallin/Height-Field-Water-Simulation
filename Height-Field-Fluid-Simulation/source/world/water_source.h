#ifndef WATER_SOURCE_H
#define WATER_SOURCE_H

#include "VectorUtils3.h"
class WaterSource {
public:
	WaterSource() = default;
	WaterSource(const vec3& pos);
	vec3 position;
	GLfloat get_height();
private:
	int count{ 0 };
	GLfloat height;

};


#endif
