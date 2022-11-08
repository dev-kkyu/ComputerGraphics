#pragma once
#include "RobotParts.h"
class Body : public RobotParts
{
public:
    Body();
    Body(glm::vec3 Color);

    ~Body();

    void update() override;
};

