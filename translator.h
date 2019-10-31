//
// Created by aydarov on 13.05.2019.
//


#include "pars.h"
#include "hash.h"

#define COMMANDS {"inc", "dec", "add", "adc", "sub", "sbc", "mul", "imul", "div", "idiv", "cdw", "cwd", "cmp", "neg", "and", "or", "xor", "not", "test", "shl", "shr", "rol", "ror", "loop", "jmp", "jeq", "jne", "jg", "jge", "jl", "jle", "mov", "int"}
#define QUANTITY_OF_COMMANDS 33

#define REGISTERS {"ax", "ah", "al", "bx", "bh", "bl", "cx", "ch", "cl", "dx", "dh", "dl"}
#define QUANTITY_OF_REGISTERS 12


#define COMMAND_SIZE 2
#define PLACE_COUNTER_MIN 13
#define REG 1
#define NUM 2
#define ADD 3


char R[QUANTITY_OF_REGISTERS][LENGTH] = REGISTERS;
char C[QUANTITY_OF_COMMANDS][LENGTH] = COMMANDS;
CodeLineInf *codeLineInfForP;
CodeLineInf *codeLineInfForW;
int startValue;
int endValue;
int start;

Map *fillingMap(char array[][LENGTH], int quantity, int value);

Map *fillingMap2(char **arrayOfKeys, int quantity, const int *arrayOfValues);

HashMapInf *
firstPass(HashMapInf **hashMapInfReg, HashMapInf **hashMapInfCom, HashMapInf **hashMapInfLabel, int *errorLine);

int secondPass(HashMapInf **hashMapInfLabel);

int fromHexToInt(char *hex);

long pow_(int a, int n);

void operandsInFirstPass(int i, HashMapInf **hashMapInfReg, int *placeCounter);

void regOperands(int k, int i, HashMapInf **hashMapInfReg, char *operand, int *placeCounter);

void numberOperands(int k, int i, char *operand, int *placeCounter);

void hexOperands(int k, int i, char *operand, int *placeCounter);

HashMapInf *labelsInFirstPass(int i, HashMapInf **hashMapInfLabel, int *current_index, char ***labels, int **labelsData,
                              int *placeCounter);

void resbOrReswOperand(int i, int N);

void resbOrReswSize(int i);

void resbOrResw(int i, int *placeCounter, int N);

void startDir(int i, int *placeCounter, HashMapInf **hashMapInfReg);

void stringOperand(int i, int *placeCounter);

void byteDir(int i, int *placeCounter, HashMapInf **hashMapRegInf);


void byteSize(int i, int *placeCounter);

void wordDir(int i, int *placeCounter);

void wordOperand(int i);

void wordSize(int i);

void sizeValue(int i, int *placeCounter, int size);

void startOperand(int i, int *placeCounter, HashMapInf **hashMapInfReg);

void commands(int i, int *placeCounter, HashMapInf **hashMapInfCom, HashMapInf **hashMapInfReg);

void endDir(int i, const int *placeCounter);

void WRRS(int i, int *placeCounter, HashMapInf **hashMapInfReg);

void startSize(int i);

void operandsInSecondPass(int k, int i, HashMapInf **hashMapInfLabel);

void addressing(int i);