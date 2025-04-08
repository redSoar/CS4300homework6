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
#include <stack>
#include <iostream>
using namespace std;

namespace sgraph {
    /**
     * This visitor implements a text rendering that includes the tabs and hyphens used to show levels, and prints the names of each node as shown.
     */
    class RayCastVisitor: public SGNodeVisitor {
        public:
        RayCastVisitor(stack<glm::mat4>& mv) : modelview(mv){
        }
        void visitGroupNode(GroupNode *groupNode) {
            for (int i=0;i<groupNode->getChildren().size();i=i+1) {
                groupNode->getChildren()[i]->accept(this);
            }
        }

        void visitLeafNode(LeafNode *leafNode) {
            string instance = leafNode->getInstanceOf();

            
            if(instance == "box") {
                
            }
            else if(instance == "sphere") {

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

        bool getHit() {
            return hit;
        }


        private:
        stack<glm::mat4>& modelview;    
        bool hit = false;
        int depth = 0;
        int indentation = 3;
   };
}

#endif