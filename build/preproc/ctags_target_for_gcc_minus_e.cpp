# 1 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino"
# 2 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino" 2
# 3 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino" 2
# 4 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino" 2
//#include <avr/interrupt.h>
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
# 17 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino"
// Simular o Sensor de movimento

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
char estado = 0;
char senhaok = 0;
char tecla = 0;
char confirma = 0;
char smov = 0;
char buzzer = 0;
int main()
{
    
# 38 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino" 3
   (*(volatile uint8_t *)((0x04) + 0x20)) 
# 38 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino"
        = 0xFF;
    init_dsp(2, 16);
    putmessage(0, 0, " ALARME ");
    putmessage(1, 0, " Definir Senha ");

    for (;;)
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
}
// Função para utilizar a tecla ‘*’ para confirmar a senha digitada
char Confirma()
{
    tecla = LeTecla();
    if (tecla == '*')
        confirma = 1;
}
// Função para utilizar a tecla ‘#’ para simular a entrada ativa do Sensor de movimento
char Smov()
{
    tecla = LeTecla();
    if (tecla == '#')
        smov = 1;
}
// Função para comparar a senha digitada com a senha salva, caso a senha esteja correta
// senhaok → TRUE //, caso não retorna no display ‘Senha inválida’.
char Senha()
{
    putmessage(0, 0, " DIGITE A SENHA");
    putmessage(1, 0, " ");
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
        putmessage(1, 0, " SENHA INVALIDA");
        _delay_ms(2000);
    }
}
// Função para redefinir a senha atual. Caso o usuário não saiba a senha atual não é possível
// alterar a senha. Depois de informar a senha atual a próxima senha digitada será definida como
// senha atual.
char RedSenha()
{
    while (senhaok == 0)
    {
        Senha();
    }
    putmessage(0, 0, " DEFINIR SENHA");
    putmessage(1, 0, "DIGITE: ");
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
    putmessage(0, 0, " SENHA DEFINIDA");
    putmessage(1, 0, " ");
    putnumber_i(1, 7, senha[0], 2);
}
// Função para ativar o funcionamento do alarme. Caso o usuário não saiba a senha atual não
//é possível ativá-lo. A partir disso quando o sensor de movimento for ativado chama a função
// Buzzer (rotina de alarme disparado)
char Ativa()
{
    while (senhaok == 0)
    {
        Senha();
    }
    estado = 1;
    senhaok = 0;
    putmessage(0, 0, " ALARME ");
    putmessage(1, 0, " ATIVADO ");
    
# 145 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino" 3
   (*(volatile uint8_t *)((0x05) + 0x20)) 
# 145 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino"
        = 
# 145 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino" 3
          (1 << (3))
# 145 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino"
                     ;
    while ((tecla = LeTecla()) != 'B' || (tecla = LeTecla()) != 'C')
    {
        Smov();
        if (smov == 1)
        {
            
# 151 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino" 3
           (*(volatile uint8_t *)((0x05) + 0x20)) 
# 151 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino"
                = 
# 151 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino" 3
                  (1 << (5))
# 151 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino"
                                ;
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
    buzzer = 1;
    
# 165 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino" 3
   (*(volatile uint8_t *)((0x05) + 0x20)) 
# 165 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino"
        |= 
# 165 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino" 3
           (1 << (2))
# 165 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino"
                         ;
    while (senhaok == 0)
    {
        Senha();
    }
    buzzer = 0;
    senhaok = 0;
    smov = 0;
    
# 173 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino" 3
   (*(volatile uint8_t *)((0x05) + 0x20)) 
# 173 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino"
        = 
# 173 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino" 3
          (1 << (3))
# 173 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino"
                     ;
    putmessage(0, 0, " ALARME ");
    putmessage(1, 0, " ATIVADO ");
}
// Função para desativar o funcionamento do alarme. Caso o usuário não saiba a senha atual
// não é possível desativá-lo.
char Desativa()
{
    while (senhaok == 0)
    {
        Senha();
    }
    estado = 0;
    putmessage(0, 0, " ALARME ");
    putmessage(1, 0, " DESATIVADO ");
    
# 188 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino" 3
   (*(volatile uint8_t *)((0x05) + 0x20)) 
# 188 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino"
        = 
# 188 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino" 3
          (1 << (5)) 
# 188 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino"
                         | 
# 188 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino" 3
                           (1 << (3))
# 188 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino"
                                      ;
    senhaok = 0;
    ;
}


char LeTecla(void)
{
    unsigned char n, i, tecla = 0, linha;
    unsigned char teclado[4][4] = {{'1', '2', '3', 'A'},
                                   {'4', '5', '6', 'B'},
                                   {'7', '8', '9', 'C'},
                                   {'*', '0', '#', 'D'}};

    
# 202 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino" 3
   (*(volatile uint8_t *)((0x0A) + 0x20)) 
# 202 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino"
        = 0x0F;
    
# 203 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino" 3
   (*(volatile uint8_t *)((0x0B) + 0x20)) 
# 203 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino"
         = 0xFF;
    for (n = 0; n < 4; n++)
    {
        
# 206 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino" 3
       (*(volatile uint8_t *)((0x0B) + 0x20)) 
# 206 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino"
              &= ~
# 206 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino" 3
                  (1 << (
# 206 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino"
                  n
# 206 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino" 3
                  ))
# 206 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino"
                        ;
        linha = 
# 207 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino" 3
               (*(volatile uint8_t *)((0x09) + 0x20)) 
# 207 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino"
                     >> 4;
        for (i = 0; i < 4; i++)
        {
            if (!(linha & 
# 210 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino" 3
                         (1 << (
# 210 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino"
                         i
# 210 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino" 3
                         ))
# 210 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino"
                               ))
            {
                tecla = teclado[i][n];
                while (!((
# 213 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino" 3
                         (*(volatile uint8_t *)((0x09) + 0x20)) 
# 213 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino"
                               >> 4) & 
# 213 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino" 3
                                       (1 << (
# 213 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino"
                                       i
# 213 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino" 3
                                       ))
# 213 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino"
                                             ))
                    ;
            }
        }
        
# 217 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino" 3
       (*(volatile uint8_t *)((0x0B) + 0x20)) 
# 217 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino"
              |= 
# 217 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino" 3
                 (1 << (
# 217 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino"
                 n
# 217 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino" 3
                 ))
# 217 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino"
                       ;
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
