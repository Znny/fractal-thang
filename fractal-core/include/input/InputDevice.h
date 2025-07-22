#pragma once

#include "InputButton.h"

class InputDevice
{
    public:
        InputDevice(int numButtons);
        ~InputDevice();

        void HandleButtonEvent(int button, bool bIsDown);
        void Update(); // Update all button states for frame transitions
        
        inline bool IsButtonDown(int button) const { return buttons[button].IsDown(); }
        inline bool IsButtonUp(int button) const { return buttons[button].IsUp(); }
        inline bool IsButtonPressed(int button) const { return buttons[button].WasPressed(); }
        inline bool IsButtonReleased(int button) const { return buttons[button].WasReleased(); }
        inline bool IsButtonHeld(int button) const { return buttons[button].IsHeld(); }
    
    protected:
        InputButton* buttons;
        int numButtons;
};