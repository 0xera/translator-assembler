//
// Created by aydarov on 12.05.2019.
//

#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COMMENT_LENGTH 100
#define LENGTH 20
#define LENGTH_LB 200
#define LINES 500
#define  FILENAME_R  "codes/code"
#define  FILENAME_WR  "codes/out"
typedef struct {
    char *number;
    char *size;
    char *label;
    char *operation;
    int addressing1;
    char *operand1;
    int addressing2;
    char *operand2;
    char *comment;

} CodeLine;

typedef struct {
    CodeLine *codeLine;
    int lines;
} CodeLineInf;

CodeLine *newCodeLines();


CodeLineInf *newCodeLinesInf(int lines);

CodeLine parsing(CodeLine *codeLine, int comment_start, int label_end, char *string);

CodeLineInf *startParsing();

void printCodeLines(CodeLineInf *codeLineInf);

void printCodeLineInf(CodeLineInf *codeLineInf);

void printCodeLineInf2(CodeLineInf *codeLineInf);

int isRR(int i, CodeLineInf *codeLineInf);

int isWRRSE(int i, CodeLineInf *codeLineInf);

void writeFileHex(CodeLineInf *codeLineInf);

void writeFileDec(CodeLineInf *codeLineInf);

void zeroStep(CodeLine *codeLine, const char *string, int *cursor, int *firstOperand, int right, int *step);

void
firstStep(CodeLine *codeLine, char *string, int *cursor, int *firstOperand, int *secondOperand, int right, int *step);

void
secondStep(CodeLine *codeLine, const char *string, int *cursor, int *secondOperand, int right, const int *step);

void parsLine(CodeLine *codeLine, int i, int end, char *str);

void preParsing(CodeLine *codeLine, int label_end, int comment_start, int end, char *str);

void findLabelOrComment(CodeLine *codeLine, int *label_end, int *comment_start, int end, const char *str);