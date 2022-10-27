#include <Arduino.h>
#line 1 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\SistemaDeSeguranca.ino"


#include "Header.h"
#line 4 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\SistemaDeSeguranca.ino"
void setup();
#line 10 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\SistemaDeSeguranca.ino"
void loop();
#line 2 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\display.ino"
void init_dsp(int l, int c);
#line 6 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\display.ino"
void putmessage(int l, int c, char *msg);
#line 11 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\display.ino"
void putnumber_i(int l, int c, long ni, int nd);
#line 23 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\display.ino"
void putnumber_f(int l, int c, float ni, int nd);
#line 1 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
void config();
#line 9 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
void loopTeclado(void);
#line 22 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
char Confirma(void);
#line 29 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
char Smov(void);
#line 36 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
char Senha(void);
#line 66 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
char RedSenha(void);
#line 95 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
char Ativa(void);
#line 122 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
char Buzzer(void);
#line 138 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
char Desativa(void);
#line 152 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
char LeTecla(void);
#line 4 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\SistemaDeSeguranca.ino"
void setup()
{
    Serial.begin(9600);
    config();
}

void loop()
{
    loopTeclado();
    // Serial.print("Victor");
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
#line 1 "c:\\Users\\User\\Documents\\Vscode\\SistemaDeSeguranca\\tecladobuzzer.ino"
void config()
{
    DDRB = 0xFF;
    init_dsp(2, 16);
    putmessage(0, 0, (char *)" ALARME ");
    putmessage(1, 0, (char *)" Definir Senha ");
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
    PORTB = _BV(PORTB3);

    while ((tecla = LeTecla()) != 'B' | (tecla = LeTecla()) != 'C')
    {
        Smov();
        if (smov == 1)
        {
            PORTB = _BV(PORTB5);
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
    PORTB |= _BV(PORTB2);
    while (senhaok == 0)
    {
        Senha();
    }
    buzzer = 0;
    senhaok = 0;
    smov = 0;
    PORTB = _BV(PORTB3);
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
    PORTB = _BV(PORTB5) | _BV(PORTB3);
    senhaok = 0;
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

