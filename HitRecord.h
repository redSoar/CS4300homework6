#ifndef __HITRECORD_H__
#define __HITRECORD_H__

#include <glm/glm.hpp>
#include "Material.h"

class HitRecord 
{
public:
    HitRecord(float time, glm::vec3 intersection, glm::vec3 normalVec, util::Material material) {
        t = time;
        intersect = intersection;
        normal = normalVec;
        mat = material;
    }

    float getTime() {
        return t;
    }

    glm::vec3 getIntersect() {
        return intersect;
    }

    glm::vec3 getNormal() {
        return normal;
    }

    util::Material getMaterial() {
        return mat;
    }
    
private:
    float t;
    glm::vec3 intersect;
    glm::vec3 normal;
    util::Material mat;
};

#endif