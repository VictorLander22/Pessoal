# 1 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\SistemaDeSeguranca.ino"

// Trabalho de Victor Daniel Lander, Larissa Yumi, Carlos Glufke, Pedro Matos, Pablo Luchesi
// para a disciplina de Seminarios cursado em Engenharia de Controle e Automacao na Puc Minas no segundo semestre de 2022
# 5 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\SistemaDeSeguranca.ino" 2
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
# 1 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\Eeprom.ino"
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
# 24 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\Eeprom.ino"
void alterarSenha(uint8_t n1, uint8_t n2)
{
    EEPROM.begin();

    EEPROM.update(0, n1);
    EEPROM.update(4, n2);

    EEPROM.end();
}
# 1 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\display.ino"
/******* PARA USO DO DISPLAY ***********************/
void init_dsp(int l, int c)
{
    lcd.begin(c, l);
}
void putmessage(int l, int c, char *msg)
{
    lcd.setCursor(c, l);
    lcd.print(msg);
}
void putnumber_i(int l, int c, long ni, int nd)
{
    char zero[] = {"0000000000000000"};
    long int nx;
    int i, j;
    nx = ni;
    for (i = 10, j = 1; nx > 9; i *= 10, j++)
        nx = ni / i;
    lcd.setCursor(c, l);
    lcd.print(&zero[16 - nd + j]);
    lcd.print(ni);
}
void putnumber_f(int l, int c, float ni, int nd)
{
    lcd.setCursor(c, l);
    lcd.print(ni, nd);
}
# 1 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\outros.ino"
void printMemoria()
{
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 3 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\outros.ino" 3
                (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 3 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\outros.ino"
                "- SRAM left: ("
# 3 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\outros.ino" 3
                ); &__c[0];}))
# 3 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\outros.ino"
                )));
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
# 1 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
void config()
{
    
# 3 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino" 3
   (*(volatile uint8_t *)((0x04) + 0x20)) 
# 3 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
        = 0xFF;
    init_dsp(2, 16);
    putmessage(0, 0, (char *)" ALARME ");
    putmessage(1, 0, (char *)" Definir Senha ");

    configEeprom();
}

void loopTeclado(void)
{
    if ((tecla = LeTecla()) != 0)
    {
        if (tecla == 'C')
            RedSenha();
        if (tecla == 'A')
            Ativa();
        if (tecla == 'B')
            Desativa();
    }
}
// Função para utilizar a tecla ‘*’ para confirmar a senha digitada
char Confirma(void)
{
    tecla = LeTecla();
    if (tecla == '*')
        confirma = 1;
}
// Função para utilizar a tecla ‘#’ para simular a entrada ativa do Sensor de movimento
char Smov(void)
{
    tecla = LeTecla();
    if (tecla == '#')
        smov = 1;
}
// Função para comparar a senha digitada com a senha salva, caso a senha esteja correta senhaok → TRUE, caso não retorna no display ‘Senha inválida’.
char Senha(void)
{
    putmessage(0, 0, (char *)"DIGITE A SENHA ");
    putmessage(1, 0, (char *)" ");
    digito[0] = digito[1] = 0;
    while (confirma == 0)
    {
        tecla = LeTecla();
        if (tecla >= '0' && tecla <= '9')
        {
            if (++digito[0] == 2)
                senhaaux[0] += tecla - '0';
            else
            {
                senhaaux[0] = (tecla - '0') * 10;
            }
            putnumber_i(1, 7, senhaaux[0], 2);
        }
        Confirma();
    }
    confirma = 0;
    if (senhaaux[0] == senha[0])
        senhaok = 1;
    else
    {
        putmessage(1, 0, (char *)"SENHA INVALIDA ");
        _delay_ms(2000);
    }
}
// Função para redefinir a senha atual. Caso o usuário não saiba a senha atual não é possívelalterar a senha. Depois de informar a senha atual a próxima senha digitada será definida como senha atual.
char RedSenha(void)
{
    while (senhaok == 0)
    {
        Senha();
    }
    putmessage(0, 0, (char *)"DEFINIR SENHA ");
    putmessage(1, 0, (char *)"DIGITE: ");
    digito[0] = digito[1] = 0;
    while (confirma == 0)
    {
        tecla = LeTecla();
        if (tecla >= '0' && tecla <= '9')
        {
            if (++digito[0] == 2)
                senha[0] += tecla - '0';
            else
                senha[0] = (tecla - '0') * 10;
            putnumber_i(1, 8, senha[0], 2);
        }
        Confirma();
        senhaok = 0;
    }
    confirma = 0;
    putmessage(0, 0, (char *)"SENHA DEFINIDA ");
    putmessage(1, 0, (char *)" ");
    putnumber_i(1, 7, senha[0], 2);

    alterarSenha(senha[0], senha[1]);
}
// Função para ativar o funcionamento do alarme. Caso o usuário não saiba a senha atual nãoé possível ativá-lo. A partir disso quando o sensor de movimento for ativado chama a função Buzzer (rotina de alarme disparado)
char Ativa(void)
{
    while (senhaok == 0)
    {
        Senha();
    }
    estado = 1;
    senhaok = 0;
    putmessage(0, 0, (char *)" ALARME ");
    putmessage(1, 0, (char *)" ATIVADO ");
    
# 109 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino" 3
   (*(volatile uint8_t *)((0x05) + 0x20)) 
# 109 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
        = 
# 109 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino" 3
          (1 << (3))
# 109 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
                     ;

    while ((tecla = LeTecla()) != 'B' | (tecla = LeTecla()) != 'C')
    {
        Smov();
        if (smov == 1)
        {
            
# 116 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino" 3
           (*(volatile uint8_t *)((0x05) + 0x20)) 
# 116 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
                = 
# 116 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino" 3
                  (1 << (5))
# 116 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
                                ;
            putmessage(0, 0, (char *)" ALARME ");
            putmessage(1, 0, (char *)" DISPARADO ");
            _delay_ms(2000);
            Buzzer();
        }
    }
}

// Função para ativar a rotina de alarme disparado. Liga o buzzer e enquanto a senha correta não for digitada o alarme continua disparado.
char Buzzer(void)
{
    buzzer = 1;
    
# 129 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino" 3
   (*(volatile uint8_t *)((0x05) + 0x20)) 
# 129 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
        |= 
# 129 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino" 3
           (1 << (2))
# 129 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
                         ;
    while (senhaok == 0)
    {
        Senha();
    }
    buzzer = 0;
    senhaok = 0;
    smov = 0;
    
# 137 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino" 3
   (*(volatile uint8_t *)((0x05) + 0x20)) 
# 137 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
        = 
# 137 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino" 3
          (1 << (3))
# 137 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
                     ;
    putmessage(0, 0, (char *)" ALARME ");
    putmessage(1, 0, (char *)" ATIVADO ");
}
// Função para desativar o funcionamento do alarme. Caso o usuário não saiba a senha atual não é possível desativá-lo.
char Desativa(void)
{
    while (senhaok == 0)
    {
        Senha();
    }
    estado = 0;
    putmessage(0, 0, (char *)"ALARME ");
    putmessage(1, 0, (char *)"DESATIVADO ");
    
# 151 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino" 3
   (*(volatile uint8_t *)((0x05) + 0x20)) 
# 151 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
        = 
# 151 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino" 3
          (1 << (5)) 
# 151 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
                         | 
# 151 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino" 3
                           (1 << (3))
# 151 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
                                      ;
    senhaok = 0;
}


char LeTecla(void)
{
    unsigned char n, i, tecla = 0, linha;
    unsigned char teclado[4][4] = {{'1', '2', '3', 'A'},
                                   {'4', '5', '6', 'B'},
                                   {'7', '8', '9', 'C'},
                                   {'*', '0', '#', 'D'}};

    
# 164 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino" 3
   (*(volatile uint8_t *)((0x0A) + 0x20)) 
# 164 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
        = 0x0F;
    
# 165 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino" 3
   (*(volatile uint8_t *)((0x0B) + 0x20)) 
# 165 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
         = 0xFF;
    for (n = 0; n < 4; n++)
    {
        
# 168 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino" 3
       (*(volatile uint8_t *)((0x0B) + 0x20)) 
# 168 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
              &= ~
# 168 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino" 3
                  (1 << (
# 168 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
                  n
# 168 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino" 3
                  ))
# 168 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
                        ;
        linha = 
# 169 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino" 3
               (*(volatile uint8_t *)((0x09) + 0x20)) 
# 169 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
                     >> 4;
        for (i = 0; i < 4; i++)
        {
            if (!(linha & 
# 172 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino" 3
                         (1 << (
# 172 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
                         i
# 172 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino" 3
                         ))
# 172 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
                               ))
            {
                tecla = teclado[i][n];
                while (!((
# 175 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino" 3
                         (*(volatile uint8_t *)((0x09) + 0x20)) 
# 175 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
                               >> 4) & 
# 175 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino" 3
                                       (1 << (
# 175 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
                                       i
# 175 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino" 3
                                       ))
# 175 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
                                             ))
                    ;
            }
        }
        
# 179 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino" 3
       (*(volatile uint8_t *)((0x0B) + 0x20)) 
# 179 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
              |= 
# 179 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino" 3
                 (1 << (
# 179 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
                 n
# 179 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino" 3
                 ))
# 179 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
                       ;
    }
    return (tecla);
}
