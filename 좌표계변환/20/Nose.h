#pragma once
#include "RobotParts.h"
class Nose : public RobotParts
{
public:
    Nose();
    Nose(glm::vec3 Color);

    void update();
};

