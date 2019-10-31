

#include "translator.h"

HashMapInf *
firstPass(HashMapInf **hashMapInfReg, HashMapInf **hashMapInfCom, HashMapInf **hashMapInfLabel, int *errorLine) {
    int placeCounter = PLACE_COUNTER_MIN;
    int current_index = -1;
    char **labels = (char **) calloc(codeLineInfForP->lines, sizeof(char *));
    int *labelsData = (int *) calloc(codeLineInfForP->lines, sizeof(int));
    (*hashMapInfLabel) = fillingHashMap(fillingMap2(labels, current_index + 1, labelsData), current_index + 1);
    for (int i = 0; i < codeLineInfForP->lines; i++) {
        (*hashMapInfLabel) = labelsInFirstPass(i, hashMapInfLabel, &current_index, &labels, &labelsData, &placeCounter);
        if (strlen(codeLineInfForP->codeLine[i].operation) != 0) {
            if (!strcmp(codeLineInfForP->codeLine[i].operation, "END")) {
                endDir(i, &placeCounter);
                return fillingHashMap(fillingMap2(labels, current_index + 1, labelsData), current_index + 1);
            } else if (isWRRSE(i, codeLineInfForP)) {
                WRRS(i, &placeCounter, hashMapInfReg);
            } else if (isHashElementOf((*hashMapInfCom)->hashMap, codeLineInfForP->codeLine[i].operation, (*hashMapInfCom)->sizeOfMap,
                                       (*hashMapInfCom)->shift)) {
                commands(i, &placeCounter, hashMapInfCom, hashMapInfReg);
                addressing(i);
            } else {
                puts(codeLineInfForP->codeLine[i].operation);
                *errorLine = i + 1;
                return fillingHashMap(fillingMap2(labels, current_index + 1, labelsData), current_index + 1);
            }
        }
    }
    return fillingHashMap(fillingMap2(labels, current_index + 1, labelsData), current_index + 1);
}


void addressing(int i) {
    int addressing = 0;
    if (codeLineInfForW->codeLine[i].addressing1 == REG && codeLineInfForW->codeLine[i].addressing2 == ADD)
        addressing = 1 << 12;
    if (codeLineInfForW->codeLine[i].addressing1 == ADD && codeLineInfForW->codeLine[i].addressing2 == REG)
        addressing = 2 << 12;
    if (codeLineInfForW->codeLine[i].addressing1 == REG && codeLineInfForW->codeLine[i].addressing2 == NUM)
        addressing = 3 << 12;
    if (codeLineInfForW->codeLine[i].addressing1 == NUM && codeLineInfForW->codeLine[i].addressing2 == REG)
        addressing = 4 << 12;
    if (codeLineInfForW->codeLine[i].addressing1 == ADD && codeLineInfForW->codeLine[i].addressing2 == NUM)
        addressing = 5 << 12;
    if (codeLineInfForW->codeLine[i].addressing1 == NUM && codeLineInfForW->codeLine[i].addressing2 == ADD)
        addressing = 6 << 12;
    if (codeLineInfForW->codeLine[i].addressing1 == REG && codeLineInfForW->codeLine[i].addressing2 == REG)
        addressing = 7 << 12;
    if (codeLineInfForW->codeLine[i].addressing1 == ADD && codeLineInfForW->codeLine[i].addressing2 == ADD)
        addressing = 8 << 12;
    if (codeLineInfForW->codeLine[i].addressing1 == REG && codeLineInfForW->codeLine[i].addressing2 == 0)
        addressing = 9 << 12;
    if (codeLineInfForW->codeLine[i].addressing1 == ADD && codeLineInfForW->codeLine[i].addressing2 == 0)
        addressing = 10 << 12;
    if (codeLineInfForW->codeLine[i].addressing1 == NUM && codeLineInfForW->codeLine[i].addressing2 == 0)
        addressing = 11 << 12;
    sprintf(codeLineInfForW->codeLine[i].operation, "%d", atoi(codeLineInfForW->codeLine[i].operation) + addressing);
    sprintf(codeLineInfForP->codeLine[i].operation, "%0.4X", atoi(codeLineInfForW->codeLine[i].operation));

}

void WRRS(int i, int *placeCounter, HashMapInf **hashMapInfReg) {
    if (!strcmp(codeLineInfForP->codeLine[i].operation, "START")) {
        startDir(i, placeCounter, hashMapInfReg);
    } else if (isRR(i, codeLineInfForP)) {
        if (!strcmp(codeLineInfForP->codeLine[i].operation, "resb"))resbOrResw(i, placeCounter, 1);
        if (!strcmp(codeLineInfForP->codeLine[i].operation, "resw"))resbOrResw(i, placeCounter, W);
    } else if (!strcmp(codeLineInfForP->codeLine[i].operation, "byte")) {
        byteDir(i, placeCounter, hashMapInfReg);
    } else if (!strcmp(codeLineInfForP->codeLine[i].operation, "word")) {
        wordDir(i, placeCounter);
    }
}

void endDir(int i, const int *placeCounter) {
    strcpy(codeLineInfForW->codeLine[i].operation, codeLineInfForP->codeLine[i].operation);
    endValue = *placeCounter;
    codeLineInfForP->codeLine[i].number = "03";
    codeLineInfForW->codeLine[i].number = "3";
}

void commands(int i, int *placeCounter, HashMapInf **hashMapInfCom, HashMapInf **hashMapInfReg) {
    sprintf(codeLineInfForW->codeLine[i].operation, "%d",
            getHashElement((*hashMapInfCom)->hashMap, codeLineInfForP->codeLine[i].operation,
                           (*hashMapInfCom)->sizeOfMap,
                           (*hashMapInfCom)->shift));
    sprintf(codeLineInfForP->codeLine[i].operation, "%0.4X",
            getHashElement((*hashMapInfCom)->hashMap, codeLineInfForP->codeLine[i].operation,
                           (*hashMapInfCom)->sizeOfMap,
                           (*hashMapInfCom)->shift));
    operandsInFirstPass(i, hashMapInfReg, placeCounter);
    sprintf(codeLineInfForW->codeLine[i].size, "%d",
            COMMAND_SIZE + atoi(codeLineInfForW->codeLine[i].size));
    sprintf(codeLineInfForP->codeLine[i].size, "%0.4X",
            atoi(codeLineInfForW->codeLine[i].size));
    *placeCounter += COMMAND_SIZE;
}

void wordDir(int i, int *placeCounter) {
    strcpy(codeLineInfForW->codeLine[i].operation, codeLineInfForP->codeLine[i].operation);
    *placeCounter += W;
    wordOperand(i);
    wordSize(i);
}

void wordOperand(int i) {
    sprintf(codeLineInfForW->codeLine[i].operand1, "%d",
            atoi(codeLineInfForP->codeLine[i].operand1));
    sprintf(codeLineInfForP->codeLine[i].operand1, "%0.4X",
            atoi(codeLineInfForP->codeLine[i].operand1));
}

void wordSize(int i) {
    sprintf(codeLineInfForP->codeLine[i].size, "%0.4X",
            W);
    sprintf(codeLineInfForW->codeLine[i].size, "%d",
            W);
}

void byteDir(int i, int *placeCounter, HashMapInf **hashMapInfReg) {
    operandsInFirstPass(i, hashMapInfReg, placeCounter);
    strcpy(codeLineInfForW->codeLine[i].operation, codeLineInfForP->codeLine[i].operation);
    byteSize(i, placeCounter);

}

void byteSize(int i, int *placeCounter) {
    sprintf(codeLineInfForP->codeLine[i].size, "%0.4X",
            strlen(codeLineInfForP->codeLine[i].operand1) / 2);
    sprintf(codeLineInfForW->codeLine[i].size, "%d",
            strlen(codeLineInfForP->codeLine[i].operand1) / 2);
    sizeValue(i, placeCounter, strlen(codeLineInfForP->codeLine[i].operand1) / 2);
}


void startDir(int i, int *placeCounter, HashMapInf **hashMapInfReg) {
    *placeCounter = atoi(codeLineInfForP->codeLine[i].operand1);
    if (*placeCounter < PLACE_COUNTER_MIN)
        *placeCounter = PLACE_COUNTER_MIN;
    codeLineInfForP->codeLine[i].number = "01";
    codeLineInfForW->codeLine[i].number = "1";
    startValue = *placeCounter;
    start = i;
    startOperand(i, placeCounter, hashMapInfReg);

}

void startOperand(int i, int *placeCounter, HashMapInf **hashMapInfReg) {
    strcpy(codeLineInfForW->codeLine[i].operation, codeLineInfForP->codeLine[i].operation);
    operandsInFirstPass(i, hashMapInfReg, placeCounter);

}

void resbOrResw(int i, int *placeCounter, int N) {
    *placeCounter += atoi(codeLineInfForP->codeLine[i].operand1) * N;
    resbOrReswOperand(i, N);
    resbOrReswSize(i);
}


void resbOrReswSize(int i) {
    sprintf(codeLineInfForP->codeLine[i].size, "%0.4X",
            strlen(codeLineInfForP->codeLine[i].operand1) / 2);
    sprintf(codeLineInfForW->codeLine[i].size, "%d",
            strlen(codeLineInfForP->codeLine[i].operand1) / 2);

}

void resbOrReswOperand(int i, int N) {
    sprintf(codeLineInfForW->codeLine[i].operand1, "%s",
            codeLineInfForP->codeLine[i].operand1);
    sprintf(codeLineInfForP->codeLine[i].operand1, "%0*X",
            atoi(codeLineInfForP->codeLine[i].operand1) * 2 + N, 0);

}

HashMapInf *labelsInFirstPass(int i, HashMapInf **hashMapInfLabel, int *current_index, char ***labels, int **labelsData,
                              int *placeCounter) {
    if (i > 0 && i < codeLineInfForP->lines - 1) {
        if (strlen(codeLineInfForP->codeLine[i].label) != 0) {
            if (!isHashElementOf((*hashMapInfLabel)->hashMap, codeLineInfForP->codeLine[i].label, (*hashMapInfLabel)->sizeOfMap,
                                 (*hashMapInfLabel)->shift)) {
                (*labels)[++(*current_index)] = (char *) calloc(strlen(codeLineInfForP->codeLine[i].label), sizeof(char));
                strcpy((*labels)[(*current_index)], codeLineInfForP->codeLine[i].label);
                (*labelsData)[(*current_index)] = *placeCounter;
            }
        }
        sprintf(codeLineInfForP->codeLine[i].label, "%0.4X", *placeCounter);
        sprintf(codeLineInfForW->codeLine[i].label, "%d", *placeCounter);
    }
    return fillingHashMap(fillingMap2((*labels), (*current_index) + 1, (*labelsData)),
                          (*current_index) + 1);
}

void regOperands(int k, int i, HashMapInf **hashMapInfReg, char *operand, int *placeCounter) {


    if (k == 0) {
        codeLineInfForW->codeLine[i].addressing1 = REG;
        sprintf(codeLineInfForW->codeLine[i].operand1, "%d", getHashElement((*hashMapInfReg)->hashMap, operand,
                                                                            (*hashMapInfReg)->sizeOfMap,
                                                                            (*hashMapInfReg)->shift));
        sprintf(codeLineInfForP->codeLine[i].operand1, "%0.4X", getHashElement((*hashMapInfReg)->hashMap, operand,
                                                                               (*hashMapInfReg)->sizeOfMap,
                                                                               (*hashMapInfReg)->shift));
        sizeValue(i, placeCounter, strlen(codeLineInfForP->codeLine[i].operand1) / 2);
    } else {
        codeLineInfForW->codeLine[i].addressing2 = REG;
        sprintf(codeLineInfForW->codeLine[i].operand2, "%d", getHashElement((*hashMapInfReg)->hashMap, operand,
                                                                            (*hashMapInfReg)->sizeOfMap, (*hashMapInfReg)->shift));
        sprintf(codeLineInfForP->codeLine[i].operand2, "%0.4X", getHashElement((*hashMapInfReg)->hashMap, operand,
                                                                               (*hashMapInfReg)->sizeOfMap,
                                                                               (*hashMapInfReg)->shift));
        sizeValue(i, placeCounter, strlen(codeLineInfForP->codeLine[i].operand2) / 2);
    }
}

void numberOperands(int k, int i, char *operand, int *placeCounter) {
    char *buff = (char *) calloc(LENGTH, sizeof(char));
    if (k == 0) {
        codeLineInfForW->codeLine[i].addressing1 = NUM;
        sprintf(codeLineInfForW->codeLine[i].operand1, "%d", atoi(operand));
        sprintf(buff, "%X", atoi(operand));
        if (strlen(buff) % 2 == 0)
            sprintf(codeLineInfForP->codeLine[i].operand1, "%X", atoi(operand));
        else
            sprintf(codeLineInfForP->codeLine[i].operand1, "0%X", atoi(operand));
        sizeValue(i, placeCounter, strlen(codeLineInfForP->codeLine[i].operand1) / 2);
    } else {
        codeLineInfForW->codeLine[i].addressing1 = NUM;
        sprintf(codeLineInfForW->codeLine[i].operand1, "%d", atoi(operand));
        sprintf(buff, "%X", atoi(operand));
        if (strlen(buff) % 2 == 0)
            sprintf(codeLineInfForP->codeLine[i].operand2, "%X", atoi(operand));
        else
            sprintf(codeLineInfForP->codeLine[i].operand2, "0%X", atoi(operand));
        sizeValue(i, placeCounter, strlen(codeLineInfForP->codeLine[i].operand2) / 2);
    }

}

void stringOperand(int i, int *placeCounter) {
    int symb_index = 1;
    char *buff;
    char *buff2 = (char *) calloc(LENGTH, sizeof(char));
    strcpy(codeLineInfForW->codeLine[i].operand1, codeLineInfForP->codeLine[i].operand1);
    while (codeLineInfForP->codeLine[i].operand1[symb_index] != '\'' &&
           codeLineInfForP->codeLine[i].operand1[symb_index] != '"') {
        buff = (char *) calloc(LENGTH, sizeof(char));
        int ch = codeLineInfForP->codeLine[i].operand1[symb_index++];
        sprintf(buff, "%0.2X", ch);
        sprintf(buff2, "%s%s", buff2, buff);
    }
    codeLineInfForW->codeLine[i].addressing1 = NUM;
    codeLineInfForP->codeLine[i].operand1 = buff2;
    sizeValue(i, placeCounter, strlen(codeLineInfForP->codeLine[i].operand1) / 2);
}

void hexOperands(int k, int i, char *operand, int *placeCounter) {
    char *buff = (char *) calloc(LENGTH, sizeof(char));
    strncpy(buff, operand, strlen(operand) - 1);
    strcpy(operand, buff);
    buff = (char *) calloc(LENGTH, sizeof(char));
    if (k == 0) {
        codeLineInfForW->codeLine[i].addressing1 = NUM;
        sprintf(codeLineInfForW->codeLine[i].operand1, "%d", fromHexToInt(operand));
        sprintf(buff, "%d", atoi(operand));
        if (strlen(buff) % 2 == 0)
            sprintf(codeLineInfForP->codeLine[i].operand1, "%d", atoi(operand));
        else
            sprintf(codeLineInfForP->codeLine[i].operand1, "0%d", atoi(operand));
        sizeValue(i, placeCounter, strlen(codeLineInfForP->codeLine[i].operand1) / 2);
    } else {
        codeLineInfForW->codeLine[i].addressing2 = NUM;
        sprintf(codeLineInfForW->codeLine[i].operand2, "%d", fromHexToInt(operand));
        sprintf(buff, "%d", atoi(operand));
        if (strlen(buff) % 2 == 0)
            sprintf(codeLineInfForP->codeLine[i].operand2, "%d", atoi(operand));
        else
            sprintf(codeLineInfForP->codeLine[i].operand2, "0%d", atoi(operand));
        sizeValue(i, placeCounter, strlen(codeLineInfForP->codeLine[i].operand2) / 2);
    }
}

void sizeValue(int i, int *placeCounter, int size) {
    if (strcmp(codeLineInfForP->codeLine[i].operation, "START")) *placeCounter += size;
    sprintf(codeLineInfForW->codeLine[i].size, "%d", size);
    sprintf(codeLineInfForP->codeLine[i].size, "%0.4X", size);
}

void operandsInFirstPass(int i, HashMapInf **hashMapInfReg, int *placeCounter) {
    char *operand;
    for (int k = 0; k <= 1; k++) {
        if (k == 0) operand = codeLineInfForP->codeLine[i].operand1;
        else operand = codeLineInfForP->codeLine[i].operand2;
        if (strlen(operand) != 0) {
            if (isHashElementOf((*hashMapInfReg)->hashMap, operand, (*hashMapInfReg)->sizeOfMap,
                                (*hashMapInfReg)->shift)) {
                regOperands(k, i, hashMapInfReg, operand, placeCounter);
            } else if (operand[strlen(operand) - 1] == 'h') {
                hexOperands(k, i, operand, placeCounter);
            } else if ((operand[0] >= 48 &&
                        operand[0] <= 57)) {
                numberOperands(k, i, operand, placeCounter);
            } else if (codeLineInfForP->codeLine[i].operand1[0] == '\'' ||
                       codeLineInfForP->codeLine[i].operand1[0] == '"') {
                stringOperand(i, placeCounter);
            } else {
                if (k == 0) codeLineInfForW->codeLine[i].addressing1 = ADD;
                else codeLineInfForW->codeLine[i].addressing2 = ADD;
                sprintf(codeLineInfForW->codeLine[i].size, "%d", atoi(codeLineInfForW->codeLine[i].size) + W);
                *placeCounter += W;
            }
        }
    }
}

void startSize(int i) {
    startValue = endValue - startValue;
    sprintf(codeLineInfForP->codeLine[i].size, "%0.4X",
            startValue);
    sprintf(codeLineInfForW->codeLine[i].size, "%d",
            startValue);
}

void operandsInSecondPass(int k, int i, HashMapInf **hashMapInfLabel) {

    if (k == 0) {
        sprintf(codeLineInfForW->codeLine[i].operand1, "%d", getHashElement((*hashMapInfLabel)->hashMap, codeLineInfForP->codeLine[i].operand1,
                                                                            (*hashMapInfLabel)->sizeOfMap,
                                                                            (*hashMapInfLabel)->shift));
        sprintf(codeLineInfForP->codeLine[i].operand1, "%0.4X", getHashElement((*hashMapInfLabel)->hashMap, codeLineInfForP->codeLine[i].operand1,
                                                                               (*hashMapInfLabel)->sizeOfMap,
                                                                               (*hashMapInfLabel)->shift));

    } else {
        sprintf(codeLineInfForW->codeLine[i].operand2, "%d", getHashElement((*hashMapInfLabel)->hashMap, codeLineInfForP->codeLine[i].operand2,
                                                                            (*hashMapInfLabel)->sizeOfMap,
                                                                            (*hashMapInfLabel)->shift));
        sprintf(codeLineInfForP->codeLine[i].operand2, "%0.4X", getHashElement((*hashMapInfLabel)->hashMap, codeLineInfForP->codeLine[i].operand2,
                                                                               (*hashMapInfLabel)->sizeOfMap,
                                                                               (*hashMapInfLabel)->shift));

    }
}

int secondPass(HashMapInf **hashMapInfLabel) {
    for (int i = 0; i < codeLineInfForP->lines; i++) {
        if (i == start) {
            if (!strcmp(codeLineInfForP->codeLine[i].operation, "START")) {
                startSize(start);
            }
        } else if (!isWRRSE(i, codeLineInfForP) || !strcmp(codeLineInfForP->codeLine[i].operation, "END")) {
            for (int k = 0; k <= 1; k++) {
                if (k == 0) {

                    if (strlen(codeLineInfForP->codeLine[i].operand1) != 0) {
                        if (isHashElementOf((*hashMapInfLabel)->hashMap, codeLineInfForP->codeLine[i].operand1,
                                            (*hashMapInfLabel)->sizeOfMap,
                                            (*hashMapInfLabel)->shift)) {
                            operandsInSecondPass(k, i, hashMapInfLabel);
                        }
                    }
                } else {
                    if (strlen(codeLineInfForP->codeLine[i].operand2) != 0) {

                        if (isHashElementOf((*hashMapInfLabel)->hashMap, codeLineInfForP->codeLine[i].operand2,
                                            (*hashMapInfLabel)->sizeOfMap,
                                            (*hashMapInfLabel)->shift)) {
                            operandsInSecondPass(k, i, hashMapInfLabel);
                        }
                    }
                }
            }
        }
    }
    return 0;
}

long pow_(int a, int n) {
    long res = 1;
    while (n-- > 0) res *= a;
    return res;
}

int fromHexToInt(char *hex) {
    int result = 0;
    for (int i = 0; i < strlen(hex); i++) {
        switch (hex[i]) {
            case 'a':
            case 'A':
                result += 10 * pow_(16, strlen(hex) - i - 1);
                break;

            case 'b':
            case 'B':
                result += 11 * pow_(16, strlen(hex) - i - 1);
                break;

            case 'c':
            case 'C':
                result += 12 * pow_(16, strlen(hex) - i - 1);
                break;

            case 'd':
            case 'D':
                result += 13 * pow_(16, strlen(hex) - i - 1);
                break;

            case 'e':
            case 'E':
                result += 14 * pow_(16, strlen(hex) - i - 1);
                break;

            case 'f':
            case 'F':
                result += 15 * pow_(16, strlen(hex) - i - 1);
                break;

            default:
                result += ((int) hex[i] - 48) * pow_(16, strlen(hex) - i - 1);
                break;
        }
    }

    return result;
}

Map *fillingMap(char array[][LENGTH], int quantity, int value) {
    Map *map = (Map *) calloc(quantity, sizeof(Map));
    for (int i = 0; i < quantity; i++) {
        map[i].key = (char *) calloc(LENGTH, sizeof(char));
        strcpy(map[i].key, array[i]);
        map[i].value = value++;
    }
    return map;
}

Map *fillingMap2(char **arrayOfKeys, int quantity, const int *arrayOfValues) {
    Map *map = (Map *) calloc(quantity, sizeof(Map));
    for (int i = 0; i < quantity; i++) {
        map[i].key = (char *) calloc(LENGTH, sizeof(char));
        strcpy(map[i].key, arrayOfKeys[i]);
        map[i].value = arrayOfValues[i];
    }
    return map;
}

int main() {
    system("chcp 65001");
    printf("Initialization...\n");

    HashMapInf *hashMapInfLabel = newHashMapInf(1);

    HashMapInf *hashMapInfReg = fillingHashMap(fillingMap(R, QUANTITY_OF_REGISTERS, 1), QUANTITY_OF_REGISTERS);

    HashMapInf *hashMapInfCom = fillingHashMap(fillingMap(C, QUANTITY_OF_COMMANDS, 13), QUANTITY_OF_COMMANDS);

    codeLineInfForP = startParsing();
    codeLineInfForW = newCodeLinesInf(codeLineInfForP->lines);
    printCodeLineInf(codeLineInfForP);
    printf("\n\nFirst pass:\n");

    int errorLine = -1;
    hashMapInfLabel = firstPass(&hashMapInfReg, &hashMapInfCom, &hashMapInfLabel,
                                &errorLine);
    if (errorLine != -1) {
        printf("Error in line %d.Unknown operator!\n", errorLine);
        return 0;
    }

    printCodeLineInf(codeLineInfForP);

    printf("\n\nSecond pass:\n");

    int res_code = secondPass(&hashMapInfLabel);
    if (res_code)
        printf("Error in line %d.Unknown identifier!\n", res_code);

    printCodeLineInf(codeLineInfForP);

    printf("\n\nResult:\n");

    printCodeLineInf2(codeLineInfForP);

    writeFileDec(codeLineInfForW);

    return 0;
}
