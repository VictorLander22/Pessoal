#include <Arduino.h>
#line 1 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\SistemaDeSeguranca.ino"

// Trabalho de Victor Daniel Lander, Larissa Yumi, Carlos Glufke, Pedro Matos, Pablo Luchesi
// para a disciplina de Seminarios cursado em Engenharia de Controle e Automacao na Puc Minas no segundo semestre de 2022
#include "Header.h"
#line 5 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\SistemaDeSeguranca.ino"
void setup();
#line 11 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\SistemaDeSeguranca.ino"
void loop();
#line 3 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\Eeprom.ino"
void configEeprom();
#line 24 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\Eeprom.ino"
void alterarSenha(u_int8t n1, u_int8t n2);
#line 2 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\display.ino"
void init_dsp(int l, int c);
#line 6 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\display.ino"
void putmessage(int l, int c, char *msg);
#line 11 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\display.ino"
void putnumber_i(int l, int c, long ni, int nd);
#line 23 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\display.ino"
void putnumber_f(int l, int c, float ni, int nd);
#line 1 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\outros.ino"
void printMemoria();
#line 7 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\outros.ino"
int freeRam();
#line 1 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
void config();
#line 11 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
void loopTeclado(void);
#line 24 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
char Confirma(void);
#line 31 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
char Smov(void);
#line 38 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
char Senha(void);
#line 68 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
char RedSenha(void);
#line 99 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
char Ativa(void);
#line 126 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
char Buzzer(void);
#line 142 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
char Desativa(void);
#line 156 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
char LeTecla(void);
#line 5 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\SistemaDeSeguranca.ino"
void setup()
{
    Serial.begin(9600);
    config();
}

void loop()
{
    loopTeclado();
    printMemoria();

    Serial.print(senha[0]);
    Serial.println(senha[1]);

    delay(5000);
    // alterarSenha();

    // para resetar toda a memoria EEPROM descomente a funcao abaixo
    // for (int i = 0; i < 1025; i++)
    // {
    //     EEPROM.update(i, 0);
    // }
}
#line 1 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\Eeprom.ino"
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
void alterarSenha(u_int8t n1, u_int8t n2)
{
    EEPROM.begin();

    EEPROM.update(0, n1);
    EEPROM.update(4, n2);

    EEPROM.end();
}
#line 1 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\display.ino"
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
#line 1 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\outros.ino"
void printMemoria()
{
    Serial.print(F("- SRAM left: "));
    Serial.println(freeRam());
}

int freeRam()
{
    extern int __heap_start, *__brkval;
    int v;
    return (int)&v - (__brkval == 0
                          ? (int)&__heap_start
                          : (int)__brkval);
}
#line 1 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
void config()
{
    DDRB = 0xFF;
    init_dsp(2, 16);
    putmessage(0, 0, (char *)" ALARME ");
    putmessage(1, 0, (char *)" Definir Senha ");

    configEeprom();
}

void loopTeclado(void)
{
    if ((tecla = LeTecla()) != 0)
    {
        if (tecla == REDEFINIR)
            RedSenha();
        if (tecla == ATIVAR)
            Ativa();
        if (tecla == DESATIVAR)
            Desativa();
    }
}
// Função para utilizar a tecla ‘*’ para confirmar a senha digitada
char Confirma(void)
{
    tecla = LeTecla();
    if (tecla == CONFIRMA)
        confirma = TRUE;
}
// Função para utilizar a tecla ‘#’ para simular a entrada ativa do Sensor de movimento
char Smov(void)
{
    tecla = LeTecla();
    if (tecla == SMOV)
        smov = TRUE;
}
// Função para comparar a senha digitada com a senha salva, caso a senha esteja correta senhaok → TRUE, caso não retorna no display ‘Senha inválida’.
char Senha(void)
{
    putmessage(0, 0, (char *)"DIGITE A SENHA ");
    putmessage(1, 0, (char *)" ");
    digito[0] = digito[1] = 0;
    while (confirma == FALSE)
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
    while (senhaok == FALSE)
    {
        Senha();
    }
    putmessage(0, 0, (char *)"DEFINIR SENHA ");
    putmessage(1, 0, (char *)"DIGITE: ");
    digito[0] = digito[1] = 0;
    while (confirma == FALSE)
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
        senhaok = FALSE;
    }
    confirma = FALSE;
    putmessage(0, 0, (char *)"SENHA DEFINIDA ");
    putmessage(1, 0, (char *)" ");
    putnumber_i(1, 7, senha[0], 2);

    alterarSenha(senha[0], senha[1]);
}
// Função para ativar o funcionamento do alarme. Caso o usuário não saiba a senha atual nãoé possível ativá-lo. A partir disso quando o sensor de movimento for ativado chama a função Buzzer (rotina de alarme disparado)
char Ativa(void)
{
    while (senhaok == FALSE)
    {
        Senha();
    }
    estado = TRUE;
    senhaok = FALSE;
    putmessage(0, 0, (char *)" ALARME ");
    putmessage(1, 0, (char *)" ATIVADO ");
    LEDS = _BV(VERDE1);

    while ((tecla = LeTecla()) != 'B' | (tecla = LeTecla()) != 'C')
    {
        Smov();
        if (smov == 1)
        {
            LEDS = _BV(VERMELHO1);
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
    LEDS |= _BV(VERMELHO2);
    while (senhaok == 0)
    {
        Senha();
    }
    buzzer = FALSE;
    senhaok = FALSE;
    smov = FALSE;
    LEDS = _BV(VERDE1);
    putmessage(0, 0, (char *)" ALARME ");
    putmessage(1, 0, (char *)" ATIVADO ");
}
// Função para desativar o funcionamento do alarme. Caso o usuário não saiba a senha atual não é possível desativá-lo.
char Desativa(void)
{
    while (senhaok == FALSE)
    {
        Senha();
    }
    estado = 0;
    putmessage(0, 0, (char *)"ALARME ");
    putmessage(1, 0, (char *)"DESATIVADO ");
    LEDS = _BV(VERMELHO1) | _BV(VERDE1);
    senhaok = FALSE;
}
#define LINHA PIND
#define COLUNA PORTD
char LeTecla(void)
{
    unsigned char n, i, tecla = 0, linha;
    unsigned char teclado[4][4] = {{'1', '2', '3', 'A'},
                                   {'4', '5', '6', 'B'},
                                   {'7', '8', '9', 'C'},
                                   {'*', '0', '#', 'D'}};

    DDRD = 0x0F;
    PORTD = 0xFF;
    for (n = 0; n < 4; n++)
    {
        COLUNA &= ~_BV(n);
        linha = LINHA >> 4;
        for (i = 0; i < 4; i++)
        {
            if (!(linha & _BV(i)))
            {
                tecla = teclado[i][n];
                while (!((LINHA >> 4) & _BV(i)))
                    ;
            }
        }
        COLUNA |= _BV(n);
    }
    return (tecla);
}

