
#include "water_source.h"

WaterSource::WaterSource(const vec3& pos, bool _is_drain) : 
	position{ pos }, height{ 2 }, is_drain{ _is_drain }{

}

GLfloat WaterSource::get_height() {
	if (is_drain)
		return 0;

	++count;
	if (count > 10000)
		count = 0;
	return height + height / 2 * sin(count / 10.0);
}
