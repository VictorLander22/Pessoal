# 1 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino"
# 2 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino" 2
# 3 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino" 2
# 4 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino" 2
# 5 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino" 2


# 6 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino"
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
# 20 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\SistemaDeSeguranca.ino"
//  Sensor de movimento

static int senha[2] = {0, 0};
static int senhaaux[2] = {0, 0};
char digito[2] = {0, 0};
char estado = 0;
char senhaok = 0;
char tecla = 0;
char confirma = 0;
char smov = 0;
char buzzer = 0;

void setup()
{
    Serial.begin(9600);
    config();
}

void loop()
{
    loopTeclado();
    Serial.print("LOOP");
}
# 1 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\display.ino"
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
# 1 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino"
void config()
{
    
# 3 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino" 3
   (*(volatile uint8_t *)((0x04) + 0x20)) 
# 3 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino"
        = 0xFF;
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
    
# 105 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino" 3
   (*(volatile uint8_t *)((0x05) + 0x20)) 
# 105 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino"
         = 
# 105 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino" 3
           (1 << (3))
# 105 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino"
                      ;

    while ((tecla = LeTecla()) != 'B' | (tecla = LeTecla()) != 'C')
    {
        Smov();
        if (smov == 1)
        {
            
# 112 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino" 3
           (*(volatile uint8_t *)((0x05) + 0x20)) 
# 112 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino"
                 = 
# 112 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino" 3
                   (1 << (5))
# 112 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino"
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
    
# 125 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino" 3
   (*(volatile uint8_t *)((0x05) + 0x20)) 
# 125 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino"
         |= 
# 125 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino" 3
            (1 << (2))
# 125 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino"
                       ;
    while (senhaok == 0)
    {
        Senha();
    }
    buzzer = 0;
    senhaok = 0;
    smov = 0;
    
# 133 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino" 3
   (*(volatile uint8_t *)((0x05) + 0x20)) 
# 133 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino"
         = 
# 133 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino" 3
           (1 << (3))
# 133 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino"
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
    
# 147 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino" 3
   (*(volatile uint8_t *)((0x05) + 0x20)) 
# 147 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino"
         = 
# 147 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino" 3
           (1 << (5)) 
# 147 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino"
                       | 
# 147 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino" 3
                         (1 << (3))
# 147 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino"
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

    
# 160 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino" 3
   (*(volatile uint8_t *)((0x0A) + 0x20)) 
# 160 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino"
        = 0x0F;
    
# 161 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino" 3
   (*(volatile uint8_t *)((0x0B) + 0x20)) 
# 161 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino"
         = 0xFF;
    for (n = 0; n < 4; n++)
    {
        
# 164 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino" 3
       (*(volatile uint8_t *)((0x0B) + 0x20)) 
# 164 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino"
              &= ~
# 164 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino" 3
                  (1 << (
# 164 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino"
                  n
# 164 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino" 3
                  ))
# 164 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino"
                        ;
        linha = 
# 165 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino" 3
               (*(volatile uint8_t *)((0x09) + 0x20)) 
# 165 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino"
                     >> 4;
        for (i = 0; i < 4; i++)
        {
            if (!(linha & 
# 168 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino" 3
                         (1 << (
# 168 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino"
                         i
# 168 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino" 3
                         ))
# 168 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino"
                               ))
            {
                tecla = teclado[i][n];
                while (!((
# 171 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino" 3
                         (*(volatile uint8_t *)((0x09) + 0x20)) 
# 171 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino"
                               >> 4) & 
# 171 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino" 3
                                       (1 << (
# 171 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino"
                                       i
# 171 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino" 3
                                       ))
# 171 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino"
                                             ))
                    ;
            }
        }
        
# 175 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino" 3
       (*(volatile uint8_t *)((0x0B) + 0x20)) 
# 175 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino"
              |= 
# 175 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino" 3
                 (1 << (
# 175 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino"
                 n
# 175 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino" 3
                 ))
# 175 "c:\\Users\\User\\Documents\\Vscode\\Sistema de seguranca\\tecladobuzzer.ino"
                       ;
    }
    return (tecla);
}
