
void BTN_A_press()
{
    Piezo.tone(220, 50);
    confirm = true;
}

void BTN_A_release()
{
    
}

void BTN_B_press()
{
    if (Piezo.isMuted())
    {
        Piezo.setMute(false);
    }
    else
    {
        Piezo.setMute(true);
    }
}

void BTN_C_press()
{
    shouldShowMenu = true;
}

void BTN_C_release()
{
    shouldShowMenu = false;
}

void BTN_DOWN_press()
{
}

void BTN_UP_press()
{
}

void BTN_UP_release()
{
}

void BTN_UP_held_300ms()
{
}

void BTN_RIGHT_press()
{
}

void BTN_RIGHT_release()
{
}

void BTN_RIGHT_held_300ms()
{
} 

void BTN_LEFT_press()
{
}

void BTN_LEFT_release()
{
}

void BTN_LEFT_held_300ms()
{
}