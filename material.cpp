#include "material.h"

#include "vec3.h"

vec3 reflect(const vec3& incident, const vec3& normal) {
    return incident - 2 * dot(incident, normal) * normal; // we compute a ray perpendicular to the incident one
}
