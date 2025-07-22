#include "input/Mouse.h"

Mouse::Mouse(int numButtons) : InputDevice(numButtons), 
                               x(0), 
                               y(0), 
                               lastX(0), 
                               lastY(0), 
                               deltaX(0), 
                               deltaY(0) {}

Mouse::~Mouse()
{
}

void Mouse::HandleMouseMoveEvent(double x, double y)
{
    bRecievedMovement = true;
    lastX = this->x;
    lastY = this->y;
    deltaX = x - lastX;
    deltaY = y - lastY;
    this->x = x;
    this->y = y;
}

void Mouse::Update()
{
    InputDevice::Update();
    if(!bRecievedMovement)
    {
        lastX = this->x;
        lastY = this->y;
        deltaX = 0;
        deltaY = 0;
    }
    bRecievedMovement = false;
}