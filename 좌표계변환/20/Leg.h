#pragma once
#include "RobotParts.h"
class Leg : public RobotParts
{
private:
    int i;
    float rotate;
    bool isrotDirec;
public:
    Leg();
    Leg(glm::vec3 Color, int i);    //i´Â À§Ä¡

    void update() override;
};

