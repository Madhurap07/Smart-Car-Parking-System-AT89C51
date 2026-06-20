#include <reg51.h>

/* ========== PIN DEFINITIONS ========== */
#define LCD P1

sbit RS = P2^5;
sbit RW = P2^6;
sbit EN = P2^7;

sbit MOTOR_P = P2^0;
sbit MOTOR_N = P2^1;

/* Keypad rows and columns */
sbit R1 = P3^0;
sbit R2 = P3^1;
sbit R3 = P3^2;
sbit R4 = P3^3;
sbit C1 = P3^4;
sbit C2 = P3^5;
sbit C3 = P3^6;

/* Variables */
unsigned char code_in[5];
bit slot1 = 0, slot2 = 0, slot3 = 0, slot4 = 0;

/* ========== FUNCTION PROTOTYPES ========== */
void delay_ms(unsigned int);
void lcd_cmd(unsigned char);
void lcd_data(unsigned char);
void lcd_print(char *);
void lcd_init(void);
char get_key(void);
bit check_code(char *input, char *valid);
void show_slots(void);

/* ========== MAIN PROGRAM ========== */
void main(void)
{
    unsigned char i;
    char k;

    MOTOR_P = MOTOR_N = 0;
    lcd_init();

    /* Startup message */
    lcd_cmd(0x01);
    lcd_print("SMART CAR PARKING ");
    lcd_cmd(0xC0);
    lcd_print("USING 8051 MCU");
    delay_ms(1500);

    while (1)
    {
        lcd_cmd(0x01);
        lcd_print("ENTER USER CODE:");
        lcd_cmd(0xC0);
        lcd_print("CODE: ");

        lcd_cmd(0x94);
        lcd_print("SLOTS:");
        show_slots();

        lcd_cmd(0xC6);  // after "CODE: "

        for (i = 0; i < 4; )
        {
            k = get_key();
            if (k != 0)
            {
                code_in[i++] = k;
                lcd_data('*');
            }
        }
        code_in[4] = '\0';
        delay_ms(300);

        lcd_cmd(0x01);

        /* ========== CODE CHECKS ========== */
        if (check_code(code_in, "1234"))
        {
            if (!slot1)
            {
                slot1 = 1;
                lcd_print("WELCOME SHARMA");
                lcd_cmd(0xC0);
                lcd_print("SLOT 1 OPENED");
                MOTOR_P = 1; MOTOR_N = 0; delay_ms(800);
            }
            else
            {
                slot1 = 0;
                lcd_print("GOODBYE SHARMA");
                lcd_cmd(0xC0);
                lcd_print("SLOT 1 FREED");
                MOTOR_P = 0; MOTOR_N = 1; delay_ms(800);
            }
        }
        else if (check_code(code_in, "2345"))
        {
            if (!slot2)
            {
                slot2 = 1;
                lcd_print("WELCOME MURTHY");
                lcd_cmd(0xC0);
                lcd_print("SLOT 2 OPENED");
                MOTOR_P = 1; MOTOR_N = 0; delay_ms(800);
            }
            else
            {
                slot2 = 0;
                lcd_print("GOODBYE MURTHY");
                lcd_cmd(0xC0);
                lcd_print("SLOT 2 FREED");
                MOTOR_P = 0; MOTOR_N = 1; delay_ms(800);
            }
        }
        else if (check_code(code_in, "3456"))
        {
            if (!slot3)
            {
                slot3 = 1;
                lcd_print("WELCOME KHAN");
                lcd_cmd(0xC0);
                lcd_print("SLOT 3 OPENED");
                MOTOR_P = 1; MOTOR_N = 0; delay_ms(800);
            }
            else
            {
                slot3 = 0;
                lcd_print("GOODBYE KHAN");
                lcd_cmd(0xC0);
                lcd_print("SLOT 3 FREED");
                MOTOR_P = 0; MOTOR_N = 1; delay_ms(800);
            }
        }
        else if (check_code(code_in, "4567"))
        {
            if (!slot4)
            {
                slot4 = 1;
                lcd_print("WELCOME REDDY");
                lcd_cmd(0xC0);
                lcd_print("SLOT 4 OPENED");
                MOTOR_P = 1; MOTOR_N = 0; delay_ms(800);
            }
            else
            {
                slot4 = 0;
                lcd_print("GOODBYE REDDY");
                lcd_cmd(0xC0);
                lcd_print("SLOT 4 FREED");
                MOTOR_P = 0; MOTOR_N = 1; delay_ms(800);
            }
        }
        else
        {
            lcd_print("INVALID CODE");
            lcd_cmd(0xC0);
            lcd_print("ACCESS DENIED");
        }

        MOTOR_P = 0; MOTOR_N = 0;
        delay_ms(1200);

        lcd_cmd(0x94);
        lcd_print("SLOTS:");
        show_slots();
        delay_ms(1000);
    }
}

/* ========== FUNCTIONS ========== */

void show_slots(void)
{
    lcd_cmd(0xD4);  // 4th line
    if (slot1) lcd_print("[因"); else lcd_print("[ ]");
    if (slot2) lcd_print("[因"); else lcd_print("[ ]");
    if (slot3) lcd_print("[因"); else lcd_print("[ ]");
    if (slot4) lcd_print("[因"); else lcd_print("[ ]");
}

/* Simple 4x3 keypad scanning */
char get_key(void)
{
    char key = 0;
    R1 = R2 = R3 = R4 = 1;

    /* Drive each row low and check columns */
    R1 = 0;
    if (!C1) { key='1'; while(!C1); }
    if (!C2) { key='2'; while(!C2); }
    if (!C3) { key='3'; while(!C3); }
    R1 = 1;

    R2 = 0;
    if (!C1) { key='4'; while(!C1); }
    if (!C2) { key='5'; while(!C2); }
    if (!C3) { key='6'; while(!C3); }
    R2 = 1;

    R3 = 0;
    if (!C1) { key='7'; while(!C1); }
    if (!C2) { key='8'; while(!C2); }
    if (!C3) { key='9'; while(!C3); }
    R3 = 1;

    R4 = 0;
    if (!C2) { key='0'; while(!C2); }
    R4 = 1;

    return key;
}

bit check_code(char *input, char *valid)
{
    unsigned char i;
    for (i = 0; i < 4; i++)
        if (input[i] != valid[i]) return 0;
    return 1;
}

/* LCD functions */
void lcd_cmd(unsigned char val)
{
    LCD = val;
    RS = 0; RW = 0; EN = 1;
    delay_ms(2);
    EN = 0;
}

void lcd_data(unsigned char val)
{
    LCD = val;
    RS = 1; RW = 0; EN = 1;
    delay_ms(2);
    EN = 0;
}

void lcd_print(char *s)
{
    while (*s) lcd_data(*s++);
}

void lcd_init(void)
{
    delay_ms(20);
    lcd_cmd(0x38);
    lcd_cmd(0x0C);
    lcd_cmd(0x01);
    lcd_cmd(0x80);
    delay_ms(5);
}

/* Delay (approx 1ms per loop @11.0592MHz) */
void delay_ms(unsigned int ms)
{
    unsigned int i,j;
    for (i=0;i<ms;i++)
        for (j=0;j<250;j++);
}