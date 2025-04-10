#ifndef _RAYCASTVISITOR_H_
#define _RAYCASTVISITOR_H_

#include "SGNodeVisitor.h"
#include "GroupNode.h"
#include "LeafNode.h"
#include "TransformNode.h"
#include "RotateTransform.h"
#include "ScaleTransform.h"
#include "TranslateTransform.h"
#include <ShaderProgram.h>
#include <ShaderLocationsVault.h>
#include "ObjectInstance.h"
#include "../Ray3D.h"
#include <stack>
#include <iostream>
using namespace std;

namespace sgraph {
    /**
     * This visitor implements a text rendering that includes the tabs and hyphens used to show levels, and prints the names of each node as shown.
     */
    class RayCastVisitor: public SGNodeVisitor {
        public:
        RayCastVisitor(stack<glm::mat4>& mv, Ray3D ray) : modelview(mv), ray(ray){
        }
        void visitGroupNode(GroupNode *groupNode) {
            for (int i=0;i<groupNode->getChildren().size();i=i+1) {
                groupNode->getChildren()[i]->accept(this);
            }
        }

        void visitLeafNode(LeafNode *leafNode) {
            string instance = leafNode->getInstanceOf();
            glm::mat4 currentModelView = modelview.top();
            glm::vec4 transformedOrigin = glm::inverse(currentModelView) * glm::vec4(ray.getPoint(), 1.0f);
            glm::vec4 transformedDirection = glm::normalize(glm::inverse(currentModelView) * glm::vec4(ray.getDirection(), 0.0f));
            
            if(instance == "box") {
                checkForBoxIntersection(transformedOrigin, transformedDirection, leafNode->getMaterial());
            }
            else if(instance == "sphere") {
                checkForSphereIntersection(transformedOrigin, transformedDirection);
            }
            else if(instance == "cylinder") {

            }
            else if(instance == "cone") {

            }
        }

        void visitTransformNode(TransformNode * transformNode) {
            modelview.push(modelview.top());
            modelview.top() = modelview.top() * transformNode->getTransform();
            if (transformNode->getChildren().size()>0) {
                transformNode->getChildren()[0]->accept(this);
            }
            modelview.pop();
        }

        void visitScaleTransform(ScaleTransform *scaleNode) {
            visitTransformNode(scaleNode);
        }

        void visitTranslateTransform(TranslateTransform *translateNode) {
            visitTransformNode(translateNode);
        }

        void visitRotateTransform(RotateTransform *rotateNode) {
            visitTransformNode(rotateNode);
        }

        HitRecord getHit() {
            return hit;
        }

        private:
        stack<glm::mat4>& modelview;   
        Ray3D ray; 
        HitRecord hit;
        int depth = 0;
        int indentation = 3;

        void checkForBoxIntersection(glm::vec3 s, glm::vec3 v, util::Material mat) {
            float minimums[3];
            float maximums[3];
            float num1 = (-0.5f - s.x)/v.x;
            float num2 = (0.5f - s.x)/v.x;
            float minimum = std::min(num1, num2);
            float maximum = std::max(num1, num2);
            minimums[0] = minimum;
            maximums[0] = maximum;
            num1 = (-0.5f - s.y)/v.y;
            num2 = (0.5f - s.y)/v.y;
            minimum = std::min(num1, num2);
            maximum = std::max(num1, num2);
            minimums[1] = minimum;
            maximums[1] = maximum;
            num1 = (-0.5f - s.z)/v.z;
            num2 = (0.5f - s.z)/v.z;
            minimum = std::min(num1, num2);
            maximum = std::max(num1, num2);
            minimums[2] = minimum;
            maximums[2] = maximum;
            minimum = std::max(minimums[0], minimums[1]);
            minimum = std::max(minimum, minimums[2]);
            maximum = std::min(maximums[0], maximums[1]);
            maximum = std::min(maximum, maximums[2]);

            if (maximum >= minimum) {
                glm::vec3 poi = s + minimum*v;
                glm::vec4 viewPoi = modelview.top() * glm::vec4(poi, 1.0f);

                glm::vec3 normal = calculateNormalForBox(poi);
                glm::vec4 viewNormal = glm::inverse(glm::transpose(modelview.top())) * glm::vec4(normal, 0.0f);

                if (hit.getHit()) {
                    if (hit.getTime() > minimum) {
                        editHit(minimum, viewPoi, viewNormal, mat);
                    }
                }
                else {
                    hit.triggerHit();
                    editHit(minimum, viewPoi, viewNormal, mat);
                }
            }
        }

        void checkForSphereIntersection(glm::vec3 s, glm::vec3 v) {
            float A = glm::dot(v, v);
            float B = 2 * glm::dot(v, s);
            float C = glm::dot(s, s) - 1;
            if((B * B - 4 * A * C) > 0) {
                float t1 = (-B + std::sqrt(B * B - 4 * A * C))/(2 * A);
                float t2 = (-B - std::sqrt(B * B - 4 * A * C))/(2 * A);
                if(t1 > 0 || t2 > 0) {
                    // hit = true;
                }
            }
        }

        void editHit(float time, glm::vec4 intersection, glm::vec4 normalVec, util::Material material) {
            hit.setTime(time);
            hit.setIntersect(glm::vec3(intersection.x, intersection.y, intersection.z));
            hit.setNormal(glm::vec3(normalVec.x, normalVec.y, normalVec.z));
            hit.setMaterial(material);
        }

        glm::vec3 calculateNormalForBox(glm::vec3 poi) {
            if (poi.x == 0.5f) {
                return glm::vec3(1,0,0);
            }
            else if (poi.x == -0.5f) {
                return glm::vec3(-1,0,0);
            }
            else if (poi.y == 0.5f) {
                return glm::vec3(0,1,0);
            }
            else if (poi.y == -0.5f) {
                return glm::vec3(0,-1,0);
            }
            else if (poi.z == 0.5f) {
                return glm::vec3(0,0,1);
            }
            else if (poi.z == -0.5f) {
                return glm::vec3(0,0,-1);
            }
            return glm::vec3(0,0,0);
        }

   };
}

#endif