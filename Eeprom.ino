// funcoes relacionadas ao funcionamento da EEPROM
// tamanho da EEMPROM = 1024.
void configEeprom()
{
    EEPROM.begin();

    EEPROM.get(0, senha[0]);
    EEPROM.get(4, senha[1]);

    EEPROM.end();

    Serial.print("O valor da senha e : ");

    Serial.print(senha[0]);
    Serial.println(senha[1]);
}

/*
 * @brief
 * Recebe os valores salvos no vetor da senha
 * @param n1 recebe o senha[0]
 * @param n2 recebe o senha[1]
 */
void alterarSenha(uint8_t n1, uint8_t n2)
{
    EEPROM.begin();

    EEPROM.update(0, n1);
    EEPROM.update(4, n2);

    EEPROM.end();
}