#pragma once

#include <iostream>

class InputButton
{
    public:
        InputButton() : bIsDown(false), bWasDown(false), bJustUpdated(false) {};
        ~InputButton(){};

        inline void Update(bool bIsDown)
        {
            //std::cout << "Updating button state: " << bIsDown << std::endl;
            bWasDown = this->bIsDown;
            this->bIsDown = bIsDown;
            bJustUpdated = true;
        }
        
        inline void ClearJustUpdated() { bJustUpdated = false; }
        inline bool WasJustUpdated() const { return bJustUpdated; }

        inline bool IsUp() const { return !bIsDown; }
        inline bool IsDown() const { return bIsDown; }
        inline bool IsHeld() const { return bIsDown && bWasDown; }
        inline bool WasPressed() const { return bIsDown && !bWasDown; }
        inline bool WasReleased() const { return !bIsDown && bWasDown; }

    private:
        bool bIsDown;
        bool bWasDown;
        bool bJustUpdated; // Flag to prevent double updates
};