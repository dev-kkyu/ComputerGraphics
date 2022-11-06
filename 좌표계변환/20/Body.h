#pragma once
#include "RobotParts.h"
class Body : public RobotParts
{
public:
    Body();
    Body(glm::vec3 Color);

    void update() override;
};

