
// Trabalho de Victor Daniel Lander, Larissa Yumi, Carlos Glufke, Pedro Matos, Pablo Luchesi
// para a disciplina de Seminarios cursado em Engenharia de Controle e Automacao na Puc Minas no segundo semestre de 2022
#include "Header.h"
void setup()
{
    Serial.begin(9600);
    config();
    delay(5000);
    Serial.print(senha[0]);
    Serial.println(senha[1]);
}

void loop()
{
    loopTeclado();
    printMemoria();

    delay(5000);

    // para resetar toda a memoria EEPROM descomente a funcao abaixo
    // for (int i = 0; i < 1025; i++)
    // {
    //     EEPROM.update(i, 0);
    // }
}