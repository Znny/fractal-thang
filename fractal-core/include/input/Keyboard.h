#pragma once

#include "InputDevice.h"

class Keyboard : public InputDevice
{
    public:
        Keyboard(int numButtons = 512) : InputDevice(numButtons) {};
        ~Keyboard() {};

        // Inherits IsButtonPressed, IsButtonReleased, IsButtonHeld, IsButtonUp, IsButtonDown from InputDevice
        
        // Explicit wrapper methods for Emscripten bindings
        inline bool IsKeyDown(int key) const { return InputDevice::IsButtonDown(key); }
        inline bool IsKeyUp(int key) const { return InputDevice::IsButtonUp(key); }
        inline bool IsKeyPressed(int key) const { return InputDevice::IsButtonPressed(key); }
        inline bool IsKeyReleased(int key) const { return InputDevice::IsButtonReleased(key); }
        inline bool IsKeyHeld(int key) const { return InputDevice::IsButtonHeld(key); }
        
        void Update() { InputDevice::Update(); }
};