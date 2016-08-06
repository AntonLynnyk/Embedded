void Delay_mks(int mks) // delay(5) = 1 ns
{
    volatile int i;
    mks *= 5;
    for (i = 0; i < mks; i++)
    {
    }
}

void Delay_ms(int ms) // delay(5) = 1 ns
{
    volatile int i;
    for (i = 0; i < ms; i++)
    {
        Delay_mks(1000);
    }
}
