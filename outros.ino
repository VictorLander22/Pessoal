void printMemoria()
{
    Serial.print(F("- SRAM left: ("));
    Serial.print(freeRam());
    Serial.println(")");
}

int freeRam()
{
    extern int __heap_start, *__brkval;
    int v;
    return (int)&v - (__brkval == 0
                          ? (int)&__heap_start
                          : (int)__brkval);
}