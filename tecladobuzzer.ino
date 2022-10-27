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
