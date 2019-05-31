#ifndef OBJECT_SELECT_HPP
#define OBJECT_SELECT_HPP

#include <vector>

#include "../objects.h"
using namespace engine;

class ObjectSelect{
    public:
        std::vector<EMesh*> available_meshes;
        unsigned int actual_object = 0;
        void next();
        void previous();
        void select();
        unsigned int count = 0;

};
#endif