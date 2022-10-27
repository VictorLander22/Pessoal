
#include <avr/io.h>
#include <util/delay.h>
#include <LiquidCrystal.h>
#include <avr/interrupt.h>

#include <EEPROM.h>

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
#define SMOV '#'

static int senha[2];
static int senhaaux[2];
char digito[2] = {0, 0};
char estado = 0;
char senhaok = 0;
char tecla = 0;
char confirma = 0;
char smov = 0;
char buzzer = 0;
