#include <iostream>
using namespace std;
#include <list>
#include <cstring>
#define SIZE 1001
//#define DEBUG


//global
struct token {
    char type[20];
    char value[20];
};
char streams[SIZE] = {'\0'};
char input[SIZE];


//prototypes
token scanner(char *localStreams, size_t* indexPtr);
token scanID(char *localStreams, size_t* indexPtr);
token scanStringLiteral(char *localStreams, size_t* indexPtr);

bool parser(list<string> &tokenList);
void stmts(list<string> &tokenList, list<string>::iterator* tokenIndexPtr, bool* syntacticValidityPtr);
void stmt(list<string> &tokenList, list<string>::iterator* tokenIndexPtr, bool* syntacticValidityPtr);
void primary(list<string> &tokenList, list<string>::iterator* tokenIndexPtr, bool* syntacticValidityPtr);
void primaryTail(list<string> &tokenList, list<string>::iterator* tokenIndexPtr, bool* syntacticValidityPtr);
void match(list<string>::iterator* tokenIndexPtr, char* type, bool* syntacticValidityPtr);

string get(list<string> &myList, size_t listIndex);


int main() {
    static list<string> tokenStreamsType;
    static list<string> tokenStreamsValue;
    size_t index = 0;

    while (cin >> input) {
        strcat(streams, input);
    }

    while (index <= SIZE - 1 && streams[index] != '\0') {
        struct token A_token = scanner(streams, &index);

        tokenStreamsType.push_back(A_token.type);
        tokenStreamsValue.push_back(A_token.value);
    }

    struct token A_end_token = {"$", 0};
    tokenStreamsType.push_back(A_end_token.type);
    tokenStreamsValue.push_back(A_end_token.value);

    //which means bool syntacticValidity = true;
    bool syntacticValidity = parser(tokenStreamsType);

    if (syntacticValidity) {
        for (size_t i = 0; i < tokenStreamsType.size() - 1; i++) {

            string typePtr = get(tokenStreamsType, i);
            if (strcmp(typePtr.c_str(), "STRLIT") == 0) {
                cout << get(tokenStreamsType, i) << " ";
                cout << "\"" << get(tokenStreamsValue, i) << "\"" << endl;
            }
            else {
                cout << get(tokenStreamsType, i) << " ";
                cout << get(tokenStreamsValue, i) << endl;
            }
        }
    }
    else {
        cout << "invalid input" << endl;
    }

    return 0;
}


token scanner(char localStreams[SIZE], size_t* indexPtr) {
    struct token Token = {};

    if ('A' <= localStreams[*indexPtr] && localStreams[*indexPtr] <= 'Z' ||
    'a' <= localStreams[*indexPtr] && localStreams[*indexPtr] <= 'z' ||
    localStreams[*indexPtr] == '_') {

        Token = scanID(localStreams, indexPtr);
    }

    else if (localStreams[*indexPtr] == '\"') {
        Token = scanStringLiteral(localStreams, indexPtr);
    }

    else {
#ifdef DEBUG
        puts("->hi4");
#endif
        switch (localStreams[*indexPtr]) {
            case '(':
                strcpy(Token.type, "LBR");
                strcpy(Token.value, "(");
                *indexPtr += 1;
                break;
            case ')':
                strcpy(Token.type, "RBR");
                strcpy(Token.value, ")");
                *indexPtr += 1;
                break;
            case '.':
                strcpy(Token.type, "DOT");
                strcpy(Token.value, ".");
                *indexPtr += 1;
                break;
        }
    }
#ifdef DEBUG
    puts("one turn");
#endif
    return Token ;
}


token scanID(char *localStreams, size_t* indexPtr) {
    struct token Token = { "ID", 0};

    const char* sPtr;
    bool statementJudging;

    //[A-Za-z_]
    statementJudging = ('A' <= localStreams[*indexPtr] && localStreams[*indexPtr] <= 'Z') ||
                       ('a' <= localStreams[*indexPtr] && localStreams[*indexPtr] <= 'z') ||
                       localStreams[*indexPtr] == '_';
    if (statementJudging && *indexPtr <= strlen(localStreams) - 1) {
        strncat(Token.value, localStreams + *indexPtr, 1);
        *indexPtr += 1;
    }
    else {
        strcpy(Token.type, "InCorrect");
        *indexPtr += 1;
    }

    //[0-9A-Za-z_]*
    statementJudging = ('0' <= localStreams[*indexPtr] && localStreams[*indexPtr] <= '9') ||
                       ('A' <= localStreams[*indexPtr] && localStreams[*indexPtr] <= 'Z') ||
                       ('a' <= localStreams[*indexPtr] && localStreams[*indexPtr] <= 'z') ||
                       localStreams[*indexPtr] == '_';
    while (statementJudging && *indexPtr <= strlen(localStreams) - 1) {
        strncat(Token.value, localStreams + *indexPtr, 1);
        *indexPtr += 1;
        statementJudging = ('0' <= localStreams[*indexPtr] && localStreams[*indexPtr] <= '9') ||
                           ('A' <= localStreams[*indexPtr] && localStreams[*indexPtr] <= 'Z') ||
                           ('a' <= localStreams[*indexPtr] && localStreams[*indexPtr] <= 'z') ||
                           localStreams[*indexPtr] == '_';
    }
#ifdef DEBUG
    puts("hi");
    printf(" %c", localStreams[*indexPtr]);
#endif
    sPtr = strchr(" \"().", localStreams[*indexPtr]);
    //Incorrect
    if (sPtr == nullptr) {
#ifdef DEBUG
        puts("hi2");
#endif
        strcpy(Token.type, "InCorrect");
        *indexPtr += 1;
    }
    //Correct
    else {
#ifdef DEBUG
        puts("hi3");
        do nothing
#endif
    }

    return Token;
}


token scanStringLiteral(char *localStreams, size_t* indexPtr) {
    struct token Token = { "STRLIT", 0};

    // boolean = have_value_in_STRLIT_and_find_the_end_Quotation_mark
    bool statementJudging = false;

    //Start with "
    if (localStreams[*indexPtr] == '\"') {
        *indexPtr += 1;
    }

    //End with " but no value in STRLIT
    if (localStreams[*indexPtr] == '\"') {
        strcpy(Token.type, "InCorrect");
        *indexPtr += 1;
    }
    else {

        //Scan the value
        while (!statementJudging && *indexPtr <= strlen(localStreams) - 1) {

            //Read value
            if (localStreams[*indexPtr] != '\"') {
                strncat(Token.value, localStreams + *indexPtr, 1);
                *indexPtr += 1;
            }

            //Find the end Quotation mark
            else {
                statementJudging = true;
                *indexPtr += 1;
            }
        }

        //Incorrect
        if (!statementJudging) {
            strcpy(Token.type, "InCorrect");
        }
        //Correct
        else {
            //do nothing
        }
    }

    return Token;
}


bool parser(list<string> &tokenList) {
    list<string>::iterator tokenIndex = tokenList.begin();
    bool syntacticValidity = true;

    stmts(tokenList, &tokenIndex, &syntacticValidity);

    return syntacticValidity;
}


void stmts(list<string> &tokenList, list<string>::iterator* tokenIndexPtr, bool* syntacticValidityPtr) {
#ifdef DEBUG
    puts("stmts");
#endif
    if (strcmp((*tokenIndexPtr)->c_str(), "STRLIT") == 0 || strcmp((*tokenIndexPtr)->c_str(), "ID") == 0) {
        stmt(tokenList, tokenIndexPtr, syntacticValidityPtr);
        stmts(tokenList, tokenIndexPtr, syntacticValidityPtr);
    }
    else if(strcmp((*tokenIndexPtr)->c_str(), "$") == 0){
        //do nothing
    }
    else {
        *syntacticValidityPtr = false;
    }
}


void stmt(list<string> &tokenList, list<string>::iterator* tokenIndexPtr, bool* syntacticValidityPtr) {
#ifdef DEBUG
    puts("stmt");
#endif
    if (strcmp((*tokenIndexPtr)->c_str(), "ID") == 0) {
        primary(tokenList, tokenIndexPtr, syntacticValidityPtr);
    }
    else if (strcmp((*tokenIndexPtr)->c_str(), "STRLIT") == 0) {
        char typePtr[] = "STRLIT";
        match(tokenIndexPtr, typePtr, syntacticValidityPtr);
    }
    else if (strcmp((*tokenIndexPtr)->c_str(), "RBR") == 0) {
        //do nothing
    }
    else {
        *syntacticValidityPtr = false;
    }
}


void primary(list<string> &tokenList, list<string>::iterator* tokenIndexPtr, bool* syntacticValidityPtr) {
#ifdef DEBUG
    puts("primary");
#endif
    if (strcmp((*tokenIndexPtr)->c_str(), "ID") == 0) {
        char typePtr[] = "ID";
        match(tokenIndexPtr, typePtr, syntacticValidityPtr);
        primaryTail(tokenList, tokenIndexPtr, syntacticValidityPtr);
    }
    else if(strcmp((*tokenIndexPtr)->c_str(), "$") == 0) {
        //do nothing
    }
    else {
        *syntacticValidityPtr = false;
    }
}


void primaryTail(list<string> &tokenList, list<string>::iterator* tokenIndexPtr, bool* syntacticValidityPtr) {
#ifdef DEBUG
    puts("primaryTail");
#endif
    if (strcmp((*tokenIndexPtr)->c_str(), "DOT") == 0) {
        char typePtr[] = "DOT";
        match(tokenIndexPtr, typePtr, syntacticValidityPtr);
        char typePtr2[] = "ID";
        match(tokenIndexPtr, typePtr2, syntacticValidityPtr);
        primaryTail(tokenList, tokenIndexPtr, syntacticValidityPtr);
    }
    else if(strcmp((*tokenIndexPtr)->c_str(), "LBR") == 0) {

        char typePtr[] = "LBR";
        match(tokenIndexPtr, typePtr, syntacticValidityPtr);

        stmt(tokenList, tokenIndexPtr, syntacticValidityPtr);

        char typePtr2[] = "RBR";
        match(tokenIndexPtr, typePtr2, syntacticValidityPtr);

        primaryTail(tokenList, tokenIndexPtr, syntacticValidityPtr);
    }
    else if(strcmp((*tokenIndexPtr)->c_str(), "$") == 0 ||
    strcmp((*tokenIndexPtr)->c_str(), "STRLIT") == 0 ||
    strcmp((*tokenIndexPtr)->c_str(), "ID") == 0 ||
    strcmp((*tokenIndexPtr)->c_str(), "RBR") == 0) {
        //do nothing
    }
    else {
        *syntacticValidityPtr = false;
    }
}


void match(list<string>::iterator* tokenIndexPtr, char* type, bool* syntacticValidityPtr) {
#ifdef DEBUG
    puts("match");
#endif
    if (strcmp((*tokenIndexPtr)->c_str(), "$") == 0) {
        *syntacticValidityPtr = false;
        //do not plus tokenIndex
    }
    else {
        if (strcmp((*tokenIndexPtr)->c_str(), type) != 0) {
#ifdef DEBUG
            puts("hi");
#endif
            *syntacticValidityPtr = false;
            (*tokenIndexPtr)++;
        }
        else {
#ifdef DEBUG
            puts("hi2");
#endif
            (*tokenIndexPtr)++;
        }
    }
#ifdef DEBUG
    cout << "" << *(*tokenIndexPtr) << "\n";
#endif
}


string get(list<string> &myList, size_t listIndex) {
    list<string>::iterator it = myList.begin();
    for (size_t i = 0; i < listIndex; i++) {
        it++;
    }
#ifdef DEBUG
    printf("%s", it->c_str());
#endif
    return *it;
}

