#include "material.h"

#include "vec3.h"

vec3 reflect(const vec3& incident, const vec3& normal) {
    return incident - 2 * dot(incident, normal) * normal; // we compute a ray perpendicular to the incident one
}

vec3 refract(const vec3& incident, const vec3& normal, double etai_over_etat) {
    auto cos_theta = dot(-incident, normal);
    vec3 r_out_parallel = etai_over_etat * (incident + cos_theta * normal);
    vec3 r_out_perp = -sqrt(1.0 - r_out_parallel.length_squared()) * normal;
    return r_out_parallel + r_out_perp;
}

double schlick(double cosine, double ref_idx) {
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}
