#pragma once
#include "RobotParts.h"
class Arm : public RobotParts
{
private:
    int i;
    float rotate;
    bool isrotDirec;
public:
    Arm();
    Arm(glm::vec3 Color, int i);    //i�� ����

    void update() override;
};

