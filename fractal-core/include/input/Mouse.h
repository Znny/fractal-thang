#pragma once

#include "InputDevice.h"

class Mouse : public InputDevice
{
    public:
        Mouse(int numButtons = 8);
        ~Mouse();

        void HandleMouseMoveEvent(double x, double y);

        inline double GetX() const { return x; }
        inline double GetY() const { return y; }
        inline double GetDeltaX() const { return deltaX; }
        inline double GetDeltaY() const { return deltaY; }
        
        // Inherits IsButtonPressed, IsButtonReleased, IsButtonHeld, IsButtonUp, IsButtonDown from InputDevice
        
        // Explicit wrapper methods for Emscripten bindings
        inline bool IsButtonDown(int button) const { return InputDevice::IsButtonDown(button); }
        inline bool IsButtonUp(int button) const { return InputDevice::IsButtonUp(button); }
        inline bool IsButtonPressed(int button) const { return InputDevice::IsButtonPressed(button); }
        inline bool IsButtonReleased(int button) const { return InputDevice::IsButtonReleased(button); }
        inline bool IsButtonHeld(int button) const { return InputDevice::IsButtonHeld(button); }
        
        void Update();
    private:
        double x, y;
        double lastX, lastY;
        double deltaX, deltaY;
        bool bRecievedMovement;
};