#include "pars.h"

CodeLine *newCodeLines() {
    CodeLine *codeLines = (CodeLine *) calloc(LINES, sizeof(CodeLine));
    for (int i = 0; i < LINES; i++) {
        codeLines[i].number = "02";
        codeLines[i].size = (char *) calloc(LENGTH, sizeof(char));
        codeLines[i].label = (char *) calloc(LENGTH_LB, sizeof(char));
        codeLines[i].operation = (char *) calloc(LENGTH, sizeof(char));
        codeLines[i].addressing1 = 0;
        codeLines[i].operand1 = (char *) calloc(LENGTH, sizeof(char));
        codeLines[i].operand2 = (char *) calloc(LENGTH, sizeof(char));
        codeLines[i].addressing2 = 0;
        codeLines[i].comment = (char *) calloc(COMMENT_LENGTH, sizeof(char));
    }
    return codeLines;
}

CodeLineInf *newCodeLinesInf(int lines) {
    CodeLineInf *codeLineInf = (CodeLineInf *) malloc(sizeof(CodeLineInf));
    codeLineInf->codeLine = newCodeLines();
    codeLineInf->lines = lines;
    return codeLineInf;


}

void zeroStep(CodeLine *codeLine, const char *string, int *cursor, int *firstOperand, int right, int *step) {
    if (string[*cursor] != ' ' && *step == 0) {
        while (string[*cursor] != ' ' && *cursor < right) {
            codeLine->operation[(*firstOperand)++] = string[(*cursor)++];
        }
        *step += 1;
    }
}

void
firstStep(CodeLine *codeLine, char *string, int *cursor, int *firstOperand, int *secondOperand, int right, int *step) {
    if (string[*cursor] != ' ' && *step == 1) {
        *firstOperand = 0;
        *secondOperand = 0;
        while (string[*cursor] != ' ' && *cursor < right) {
            if (string[*cursor] == ',') {
                *step += 1;
                *cursor += 1;
                secondStep(codeLine, string, cursor, secondOperand, right, step);
            } else {
                codeLine->operand1[(*firstOperand)++] = string[(*cursor)++];
            }
        }
    }
}

void
secondStep(CodeLine *codeLine, const char *string, int *cursor, int *secondOperand, int right, const int *step) {
    while (string[*cursor] == ' ' && *step == 2) {
        *cursor += 1;
    }
    if (string[*cursor] != ' ' && *step == 2) {
        *secondOperand = 0;
        while (string[*cursor] != ' ' && *cursor < right) {
            if (string[*cursor] != ',')
                codeLine->operand2[(*secondOperand)++] = string[(*cursor)++];
            else
                *cursor += 1;
        }
    }
}

void findLabelOrComment(CodeLine *codeLine, int *label_end, int *comment_start, int end, const char *str) {
    for (int current = 0; current < end; current++) {
        if (str[current] == ';') {
            *comment_start = current;
            for (int cursor = ++current, firstComment = 0; cursor < end; cursor++, firstComment++) {
                codeLine->comment[firstComment] = str[cursor];
            }
        }
        if (str[current] == ':') {
            *label_end = current;
            for (int cursor = 0, firstLabel = 0; cursor < current; cursor++) {
                if (str[cursor] != ' ') {
                    codeLine->label[firstLabel++] = str[cursor];
                }
            }
        }
    }
}

CodeLine parsing(CodeLine *codeLine, int comment_start, int label_end, char *string) {
    int step = 0, firstOperand, secondOperand;
    int right = comment_start;
    for (int cursor = label_end; cursor < right; cursor++) {
        firstOperand = 0;
        secondOperand = 0;
        zeroStep(codeLine, string, &cursor, &firstOperand, right, &step);
        firstStep(codeLine, string, &cursor, &firstOperand, &secondOperand, right, &step);
        secondStep(codeLine, string, &cursor, &secondOperand, right, &step);
    }
    return *codeLine;
}

void preParsing(CodeLine *codeLine, int label_end, int comment_start, int end, char *str) {
    if (comment_start == -1 && label_end == -1) {
        parsing(codeLine, end, 0, str);
    }
    if (comment_start == -1 && label_end != -1) {
        parsing(codeLine, end, ++label_end, str);
    }
    if (comment_start != -1 && label_end == -1) {
        parsing(codeLine, comment_start, 0, str);
    }
    if (comment_start != -1 && label_end != -1) {
        parsing(codeLine, comment_start, ++label_end, str);
    }
}

void parsLine(CodeLine *codeLine, int i, int end, char *str) {
    int comment_start = -1;
    int label_end = -1;
    findLabelOrComment(codeLine, &label_end, &comment_start, end, str);
    preParsing(codeLine, label_end, comment_start, end, str);
}

CodeLineInf *startParsing() {
    FILE *file = fopen(FILENAME_R, "r");
    CodeLine *mCodeLine = newCodeLines();
    char *str = (char *) calloc(LENGTH, sizeof(char));
    char symbol;
    int i = 0, end = 0;
    if (file == NULL) {
        printf("error");
        return NULL;
    }
    do {
        symbol = fgetc(file);
        if (symbol != '\n' && symbol != EOF) {
            str[end++] = symbol;
        } else {
            parsLine(&mCodeLine[i], i, end, str);
            str = (char *) calloc(LENGTH, sizeof(char));
            end = 0;
            i++;
        }
    } while (symbol != EOF);
    fclose(file);
    CodeLineInf *codeLineInf = newCodeLinesInf(i);
    codeLineInf->codeLine = mCodeLine;
    codeLineInf->lines = i;
    return codeLineInf;
}

void printCodeLines(CodeLineInf *codeLineInf) {
    printf("\n");
    for (int one_line = 0; one_line < codeLineInf->lines; one_line++) {
        printf("label: %-10s ", codeLineInf->codeLine[one_line].label);
        printf("operation: %-10s ", codeLineInf->codeLine[one_line].operation);
        printf("operand1: %-10s ", codeLineInf->codeLine[one_line].operand1);
        printf("operand2: %-10s ", codeLineInf->codeLine[one_line].operand2);
        printf("comment: %-20s ", codeLineInf->codeLine[one_line].comment);
        printf("\n");
    }

}

void printCodeLineInf(CodeLineInf *codeLineInf) {
    printf("\n");
    for (int i = 0; i < codeLineInf->lines; i++) {
        if (strlen(codeLineInf->codeLine[i].label) != 0)
            printf("%-6s:", codeLineInf->codeLine[i].label);
        else printf("%-7s", " ");
        if (strlen(codeLineInf->codeLine[i].operation) != 0)
            printf("%-7s", codeLineInf->codeLine[i].operation);
        else printf("%-7s", " ");
        if (strlen(codeLineInf->codeLine[i].operand1) != 0)
            printf("%-6s", codeLineInf->codeLine[i].operand1);
        else printf("%-7s", " ");
        if (strlen(codeLineInf->codeLine[i].operand2) != 0)
            printf(",%-7s", codeLineInf->codeLine[i].operand2);
        else printf("%-7s", " ");
        printf("\n");
    }
}


void printCodeLineInf2(CodeLineInf *codeLineInf) {
    printf("\n");
    for (int i = 0; i < codeLineInf->lines; i++) {
        printf("%s", codeLineInf->codeLine[i].number);
        printf("%s", codeLineInf->codeLine[i].label);
        printf("%s", codeLineInf->codeLine[i].size);
        if (!isWRRSE(i, codeLineInf) &&
            strcmp(codeLineInf->codeLine[i].operation, "START") && strcmp(codeLineInf->codeLine[i].operation, "END"))
            printf("%s", codeLineInf->codeLine[i].operation);
        printf("%s", codeLineInf->codeLine[i].operand1);
        printf("%s", codeLineInf->codeLine[i].operand2);
        printf("\n");
    }

}

void writeFileHex(CodeLineInf *codeLineInf) {
    FILE *file = fopen(FILENAME_WR, "wb");
    for (int i = 0; i < codeLineInf->lines; i++) {
        if (strlen(codeLineInf->codeLine[i].number) != 0)
            fwrite(codeLineInf->codeLine[i].number, (size_t) 1, 1, file);
        if (strlen(codeLineInf->codeLine[i].label) != 0)
            fwrite(&codeLineInf->codeLine[i].label, (size_t) 2, 1, file);
        if (strlen(codeLineInf->codeLine[i].size) != 0)
            fwrite(&codeLineInf->codeLine[i].size, (size_t) 2, 1, file);
        if (strlen(codeLineInf->codeLine[i].operation) != 0 && !isWRRSE(i, codeLineInf) &&
            strcmp(codeLineInf->codeLine[i].operation, "START") && strcmp(codeLineInf->codeLine[i].operation, "END"))
            fwrite(&codeLineInf->codeLine[i].operation, (size_t) 1, 1, file);
        if (strlen(codeLineInf->codeLine[i].operand1) != 0)
            fwrite(&codeLineInf->codeLine[i].operand1, (size_t) 2, 1, file);
        if (strlen(codeLineInf->codeLine[i].operand2) != 0)
            fwrite(&codeLineInf->codeLine[i].operand2, (size_t) 2, 1, file);
    }
    fclose(file);
}

void writeFileDec(CodeLineInf *codeLineInf) {
    FILE *file = fopen(FILENAME_WR, "wb");
    int buff;
    for (int i = 0; i < codeLineInf->lines; i++) {
        if (strlen(codeLineInf->codeLine[i].number) != 0) {
            buff = atoi(codeLineInf->codeLine[i].number);
            fwrite(&buff, (size_t) 1, 1, file);
        }
        if (strlen(codeLineInf->codeLine[i].label) != 0) {
            int buff = atoi(codeLineInf->codeLine[i].label);
            fwrite(&buff, (size_t) 2, 1, file);
        }
        if (strlen(codeLineInf->codeLine[i].size) != 0) {
            int buff = atoi(codeLineInf->codeLine[i].size);
            fwrite(&buff, (size_t) 2, 1, file);
        }
        if (strlen(codeLineInf->codeLine[i].operation) != 0 && !isWRRSE(i, codeLineInf) &&
            strcmp(codeLineInf->codeLine[i].operation, "START") && strcmp(codeLineInf->codeLine[i].operation, "END")) {
            int buff = atoi(codeLineInf->codeLine[i].operation);
            fwrite(&buff, (size_t) 1, 1, file);
        }
        if (strlen(codeLineInf->codeLine[i].operand1) != 0) {
            if (isWRRSE(i, codeLineInf)) {
                if (isRR(i, codeLineInf)) {
                    if (!strcmp(codeLineInf->codeLine[i].operation, "resb")) {
                        buff = 0;
                        fwrite(&buff, (size_t) 1, atoi(codeLineInf->codeLine[i].operand1), file);
                    } else {
                        buff = 0;
                        fwrite(&buff, (size_t) 2, atoi(codeLineInf->codeLine[i].operand1), file);
                    }
                } else if (!strcmp(codeLineInf->codeLine[i].operation, "word")) {
                    buff = atoi(codeLineInf->codeLine[i].operand1);
                    fwrite(&buff, (size_t) 2, 1, file);
                } else if (!strcmp(codeLineInf->codeLine[i].operation, "byte")) {
                    if (codeLineInf->codeLine[i].operand1[0] != '\'' &&
                        codeLineInf->codeLine[i].operand1[0] != '"') {
                        buff = atoi(codeLineInf->codeLine[i].operand1);
                        int len = strlen(codeLineInf->codeLine[i].operand1) / 2;
                        if (strlen(codeLineInf->codeLine[i].operand1) % 2 != 0)
                            len++;
                        fwrite(&buff, (size_t) len, 1, file);
                    } else {
                        int symb_index = 1;
                        while (codeLineInf->codeLine[i].operand1[symb_index] != '\'' &&
                               codeLineInf->codeLine[i].operand1[symb_index] != '"') {
                            int ch = codeLineInf->codeLine[i].operand1[symb_index++];
                            fwrite(&ch, (size_t) 1, 1, file);
                        }
                    }
                } else {
                    buff = atoi(codeLineInf->codeLine[i].operand1);
                    fwrite(&buff, (size_t) 2, 1, file);
                }

            } else {
                buff = atoi(codeLineInf->codeLine[i].operand1);
                fwrite(&buff, (size_t) 2, 1, file);
            }
        }
        if (strlen(codeLineInf->codeLine[i].operand2) != 0) {
            buff = atoi(codeLineInf->codeLine[i].operand2);
            fwrite(&buff, (size_t) 2, 1, file);
        }
    }

    fclose(file);
}


int isWRRSE(int i, CodeLineInf *codeLineInf) {
    if ((!strcmp(codeLineInf->codeLine[i].operation, "word") ||
         !strcmp(codeLineInf->codeLine[i].operation, "resb") ||
         !strcmp(codeLineInf->codeLine[i].operation, "resw") ||
         !strcmp(codeLineInf->codeLine[i].operation, "byte") ||
         !strcmp(codeLineInf->codeLine[i].operation, "START") ||
         !strcmp(codeLineInf->codeLine[i].operation, "END"))) {
        return 1;
    } else {
        return 0;
    }
}

int isRR(int i, CodeLineInf *codeLineInf) {
    if ((!strcmp(codeLineInf->codeLine[i].operation, "resb") ||
         !strcmp(codeLineInf->codeLine[i].operation, "resw"))) {
        return 1;
    } else {
        return 0;
    }
}

//int main() {
//    printCodeLines(startParsing());
//    return 0;
//
//}
