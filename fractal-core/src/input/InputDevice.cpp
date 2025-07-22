#include "input/InputDevice.h"

InputDevice::InputDevice(int numButtons) : numButtons(numButtons)
{
    buttons = new InputButton[numButtons];
}

InputDevice::~InputDevice()
{
    delete[] buttons;
}

void InputDevice::HandleButtonEvent(int key, bool bIsDown)
{
    if (key >= 0 && key < numButtons)
    {
        buttons[key].Update(bIsDown);
    }
}

void InputDevice::Update()
{
    // Update all button states to transition from PRESSED to HELD
    // Skip buttons that were just updated this frame
    for (int i = 0; i < numButtons; i++)
    {
        if (buttons[i].IsDown() && !buttons[i].WasJustUpdated()) {
            // If button is down and wasn't just updated, transition from PRESSED to HELD
            buttons[i].Update(true);
        }
        // Clear the just updated flag for next frame
        buttons[i].ClearJustUpdated();
    }
}