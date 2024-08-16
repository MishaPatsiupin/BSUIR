#include <dos.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
void printRegister(int reg, char far* pos);
void print();
void prepare();

void interrupt (*oldint1[8])(...); // IRQ 0-7
void interrupt (*oldint2[8])(...); // IRQ 8-15

void interrupt newint08(...) {
    _disable();
    print();
    oldint1[0]();
    _enable();
}
void interrupt newint09(...) {
    prepare();
    oldint1[1]();
    _enable();
}
void interrupt newint0A(...) {
    prepare();
    oldint1[2]();
    _enable();
}
void interrupt newint0B(...) {
    prepare();
    oldint1[3]();
    _enable();
}
void interrupt newint0C(...) {
    prepare();
    oldint1[4]();
    _enable();
}
void interrupt newint0D(...) {
    prepare();
    oldint1[5]();
    _enable();
}
void interrupt newint0E(...) {
    prepare();
    oldint1[6]();
    _enable();
}
void interrupt newint0F(...) {
    prepare();
    oldint1[7]();
    _enable();
}

void interrupt newint78(...) {
    prepare();
    oldint2[0]();
    _enable();
}
void interrupt newint79(...) {
    prepare();
    oldint2[1]();
    _enable();
}
void interrupt newint7A(...) {
    prepare();
    oldint2[2]();
    _enable();
}
void interrupt newint7B(...) {
    prepare();
    oldint2[3]();
    _enable();
}
void interrupt newint7C(...) {
    prepare();
    oldint2[4]();
    _enable();
}
void interrupt newint7D(...) {
    prepare();
    oldint2[5]();
    _enable();
}
void interrupt newint7E(...) {
    prepare();
    oldint2[6]();
    _enable();
}
void interrupt newint7F(...) {
    prepare();
    oldint2[7]();
    _enable();
}

void initialize() {
    int i;
    for (i = 0x70; i <= 0x77; i++) {
        oldint2[i - 0x70] = getvect(i);
    }

    for (i = 0x08; i <= 0x0F; i++) {
        oldint1[i - 0x08] = getvect(i);
    }
    // Установка новых обработчиков для IRQ 0-7
    setvect(0xC0, newint08);
    setvect(0xC1, newint09);
    setvect(0xC2, newint0A);
    setvect(0xC3, newint0B);
    setvect(0xC4, newint0C);
    setvect(0xC5,newint0D);
    setvect(0xC5, newint0D);
    setvect(0xC6, newint0E);
    setvect(0xC7, newint0F);
    // Установка новых обработчиков для IRQ 8-15
    setvect(0x08, newint78);
    setvect(0x09, newint79);
    setvect(0x0A, newint7A);
    setvect(0x0B, newint7B);
    setvect(0x0C, newint7C);
    setvect(0x0D, newint7D);
    setvect(0x0E, newint7E);
    setvect(0x0F, newint7F);

    _disable(); // CLI
    // Инициализация прерываний для Master
    outp(0x20, 0x11); // ICW1 - инициализация Master
    outp(0x21, 0xC0); // ICW2 - базовый вектор для Master
    outp(0x21, 0x04); // ICW3 - номер порта Slave
    outp(0x21, 0x01); // ICW4 - по умолчанию
    // Инициализация прерываний для Slave
    outp(0xA0, 0x11); // ICW1 - инициализация Slave
    outp(0xA1, 0x08); // ICW2 - базовый вектор для Slave
    outp(0xA1, 0x02); // ICW3 - номер порта Master
    outp(0xA1, 0x01); // ICW4 - по умолчанию
    _enable(); // STI
}

int main() {
    unsigned far* fp;
    initialize();
    system("cls");

    printf("mask:   \n");
    printf("obsl:\n");
    printf("requ:\n");

    FP_SEG(fp) = _psp;
    FP_OFF(fp) = 0x2c;
    _dos_freemem(*fp);

    _dos_keep(0, (_DS - _CS) + (_SP / 16) + 1);

    return 0;
}



void printRegister(int reg, char far* pos) {
    for (int i = 0; i < 8; i++) {
        *pos = reg % 2 + '0';
        pos += 2;
        reg = reg >> 1;
    }
}

void print() {
    char temp;
    int i, val;
    char far* screen = (char far*)MK_FP(0xB800, 0); // Доступ к видеопамяти
    screen += 20;

    val = inp(0x21); // Получение значения регистра масок Master
    printRegister(val, screen);

    screen += 18;

    val = inp(0xA1); // Получение значения регистра масок Slave
    printRegister(val, screen);

    screen += 142;

    outp(0x20, 0x0A);
    val = inp(0x20);
    printRegister(val, screen); // Получение значения регистра запросов Master

    screen += 18;

    outp(0xA0, 0x0A);
    val = inp(0xA0);
    printRegister(val, screen); // Получение значения регистра запросов Slave

    screen += 142;

    outp(0x20, 0x0B);
    val = inp(0x20);
    printRegister(val, screen); // Получение значения регистра обслуживания Master

    screen += 18;

    outp(0xA0, 0x0B);
    val = inp(0xA0);
    printRegister(val, screen); // Получение значения регистра обслуживания Slave
}

void prepare() {
    _disable();
    print();
}