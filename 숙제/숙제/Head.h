#pragma once
#include "RobotParts.h"
class Head : public RobotParts
{
public:
    Head();
    Head(glm::vec3 Color);

    ~Head();
    
    void Update() override;
};

