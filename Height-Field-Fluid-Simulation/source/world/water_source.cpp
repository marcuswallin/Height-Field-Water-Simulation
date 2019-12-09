
#include "water_source.h"

WaterSource::WaterSource(const vec3& pos) : position{ pos }, height{ 2 } {

}

GLfloat WaterSource::get_height() {

	++count;
	if (count > 10000)
		count = 0;
	return height + height / 2 * sin(count / 10.0);
}
