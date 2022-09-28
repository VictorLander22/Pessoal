#include <avr/io.h>
#include <util/delay.h>
#include <LiquidCrystal.h>
//#include <avr/interrupt.h>
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
#define LEDS PORTB
#define BUZZER PORTB0
#define VERMELHO2 PORTB2
#define VERDE1 PORTB3
#define VERMELHO1 PORTB5
#define ATIVAR 'A'
#define DESATIVAR 'B'
#define REDEFINIR 'C'
#define CONFIRMA '*'
#define TRUE 1
#define FALSE 0
// Simular o Sensor de movimento
#define SMOV '#'
char Smov(void);
char LeTecla(void);
char RedSenha(void);
char Ativa(void);
char Desativa(void);
char Confirma(void);
char Senha(void);
char Buzzer(void);
static int senha[2] = {0, 0};
static int senhaaux[2] = {0, 0};
char digito[2] = {0, 0};
char estado = FALSE;
char senhaok = FALSE;
char tecla = 0;
char confirma = FALSE;
char smov = FALSE;
char buzzer = FALSE;
int main()
{
    DDRB = 0xFF;
    init_dsp(2, 16);
    putmessage(0, 0, " ALARME ");
    putmessage(1, 0, " Definir Senha ");

    for (;;)
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
}
// Função para utilizar a tecla ‘*’ para confirmar a senha digitada
char Confirma()
{
    tecla = LeTecla();
    if (tecla == CONFIRMA)
        confirma = TRUE;
}
// Função para utilizar a tecla ‘#’ para simular a entrada ativa do Sensor de movimento
char Smov()
{
    tecla = LeTecla();
    if (tecla == SMOV)
        smov = TRUE;
}
// Função para comparar a senha digitada com a senha salva, caso a senha esteja correta
// senhaok → TRUE //, caso não retorna no display ‘Senha inválida’.
char Senha()
{
    putmessage(0, 0, " DIGITE A SENHA");
    putmessage(1, 0, " ");
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
    confirma = FALSE;
    if (senhaaux[0] == senha[0])
        senhaok = TRUE;
    else
    {
        putmessage(1, 0, " SENHA INVALIDA");
        _delay_ms(2000);
    }
}
// Função para redefinir a senha atual. Caso o usuário não saiba a senha atual não é possível
// alterar a senha. Depois de informar a senha atual a próxima senha digitada será definida como
// senha atual.
char RedSenha()
{
    while (senhaok == FALSE)
    {
        Senha();
    }
    putmessage(0, 0, " DEFINIR SENHA");
    putmessage(1, 0, "DIGITE: ");
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
    putmessage(0, 0, " SENHA DEFINIDA");
    putmessage(1, 0, " ");
    putnumber_i(1, 7, senha[0], 2);
}
// Função para ativar o funcionamento do alarme. Caso o usuário não saiba a senha atual não
//é possível ativá-lo. A partir disso quando o sensor de movimento for ativado chama a função
// Buzzer (rotina de alarme disparado)
char Ativa()
{
    while (senhaok == FALSE)
    {
        Senha();
    }
    estado = TRUE;
    senhaok = FALSE;
    putmessage(0, 0, " ALARME ");
    putmessage(1, 0, " ATIVADO ");
    LEDS = _BV(VERDE1);
    while ((tecla = LeTecla()) != 'B' | (tecla = LeTecla()) != 'C')
    {
        Smov();
        if (smov == TRUE)
        {
            LEDS = _BV(VERMELHO1);
            putmessage(0, 0, " ALARME ");
            putmessage(1, 0, " DISPARADO ");
            _delay_ms(2000);
            Buzzer();
        }
    }
}

// Função para ativar a rotina de alarme disparado. Liga o buzzer e enquanto a senha correta
// não for digitada o alarme continua disparado.
char Buzzer()
{
    buzzer = TRUE;
    LEDS |= _BV(VERMELHO2);
    while (senhaok == FALSE)
    {
        Senha();
    }
    buzzer = FALSE;
    senhaok = FALSE;
    smov = FALSE;
    LEDS = _BV(VERDE1);
    putmessage(0, 0, " ALARME ");
    putmessage(1, 0, " ATIVADO ");
}
// Função para desativar o funcionamento do alarme. Caso o usuário não saiba a senha atual
// não é possível desativá-lo.
char Desativa()
{
    while (senhaok == FALSE)
    {
        Senha();
    }
    estado = FALSE;
    putmessage(0, 0, " ALARME ");
    putmessage(1, 0, " DESATIVADO ");
    LEDS = _BV(VERMELHO1) | _BV(VERDE1);
    senhaok = FALSE;
    ;
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
/******* PARA USO DO DISPLAY ***********************/
void init_dsp(int l, int c)
{
    lcd.begin(c, l);
}
void putmessage(int l, int c, String i)
{

    char *msg = (char *)(i.c_str());
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
