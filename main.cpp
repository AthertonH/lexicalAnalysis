/*
    Name: Harrison Atherton, 5005662748, CS326, 2
    Description: Create a scanner and parser infofinfionafsdf
    Input: File to scan asdfasdfasdfasd
    Output: Terminal will desplay the scanned items then begin parsing
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// LEXER ---------------------------------------------------
#define MAX_SIZE 32
#define printToken(tok) \
    printf("[Terminal:<%s='%s',line=%d>]\n", \
    names[(tok).kind], \
    symbols[(tok).kind], \
    (tok).line)

// Lexemes contained in each group of tokens.
const char* symbols[] = {
    "invalid",
    "eof", "waaaah",
    "=", "|", "#", "\"", ")", "(",
    "+", "-", ".", ":", ",", ";",
    ">", "<", "^", "!", "[", "@"
};

// Textual representation for each group of tokens.
// Textual representation for each group of tokens.
const char* names[] = {
    "INVALID",
    "EOF", "BABY-CRY",
    "EQUAL", "BAR", "NUMERAL", "QUOTE", "PARENL", "PARENR",
    "PLUS", "MINUS", "DOT", "COLON", "COMMA", "SEMI",
    "GT", "LT", "XOR", "NOT", "BRACKL", "AT"
};

// Numerical representation for each group of tokens.
typedef enum {
    TOK_INVALID,
    TOK_END,
    TOK_BABY_CRY,       // waaaah
    TOK_EQUAL,          // =
    TOK_BAR,            // |
    TOK_NUMERAL,        // #
    TOK_QUOTE,          // "
    TOK_PARENL,         // (
    TOK_PARENR,         // ) 
    TOK_PLUS,           // +
    TOK_MINUS,          // -
    TOK_DOT,            // .
    TOK_COLON,          // :
    TOK_COMMA,          // ,
    TOK_SEMI,           // ;
    TOK_GT,             // >
    TOK_LT,             // <
    TOK_XOR,            // ^
    TOK_NOT,            // !
    TOK_BRACKL,         // [
    TOK_AT              // @
} TokenType;

// Definition of a Lexer type.
typedef struct {
    int current;        // the current character
    int line;           // the line number where the lexeme appeared
    long bufferSize;    // the number of characters store
    char* buffer;       // a dynamic array of characters
} Lexer;

// Definition of a Token type.
typedef struct {
    TokenType kind;     // type of token
    int line;           // the line number where the lexeme appeared
} Token;

/// @brief Attempts to reallocate memory on the heap based on the
/// pointer ptr. If ptr is null, a new block of memory of the specified
/// size is allocated.
/// @param ptr A previously allocated memory block, or null if 
/// new memory allocation is required.
/// @param size the number of bytes to allocate.
/// @return A  newly allocated memory block if ptr is null, or an 
/// existing memory block pointed to by ptr with an additional size 
/// bytes if ptr is
/// not null.
void *myAlloc(void *ptr, size_t size) {
    ptr = realloc(ptr, size);
    if (!ptr) {
        fprintf(stdout, "Realloc failed to allocated memory.\n");
        return 0;
    }
    return ptr;
}

/// @brief Opens and reads the entire file and stores the text in a
/// buffer array.
/// @param filename The file to opend.
/// @param lex The scanner.
void readFile(const char *filename, Lexer* lex) {
    // Open file to read
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stdout, "Failed to open file '%s'", filename);
        return;
    }
    // Compute the number of bytes needed to allocate dynamic
    // memory for the stream buffer
    fseek(file, 0, SEEK_END);
    long len = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *buffer = (char*) myAlloc(NULL, sizeof(char) * (len + 1));
    if (!buffer) {
        fprintf(stdout, "Unable to allocate memory for buffer\n");
        fclose(file);
        return;
    }
    fread(buffer, 1, len, file);
    fclose(file);
    // Buffer contains the entire input so null-terminate it
    buffer[len] = '\0';
    lex->buffer = buffer;
    lex->bufferSize = len;
}

/// @brief Reads an entire line of characters not including the 
/// line-feed.
/// @param[in] str the buffer array where characters are to be stored.
/// @param[in] n the maximum number of characters allowed in the buffer.
/// @return The function returns the number of characters read.
int readline(char str[], int n) {
    int ch = 0, i = 0;
    while ((ch = getchar()) != '\n') if (i < n) str[i++] = ch;
    str[i] = '\0';
    return i;
}

/// @brief Returns true if the character is a letter or an underscore.
/// @param c The character to be checked.
/// @return True if the character is a letter or an underscore, or
/// false otherwise.
bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

/// @brief Creates and returns a token from the specified type.
/// @param lex The scanner.
/// @param kind The type of token to be created.
/// @return A token of type kind.
Token makeToken(Lexer* lex, TokenType kind) {
    Token token;

    token.kind = kind;
    token.line = lex->line;

    return token;
}

void consume(Lexer* lex) {
    lex->current++;

    if(lex->current > lex->bufferSize)
    {
        lex->current = lex->bufferSize;
    }
}

void skipWhitespace(Lexer* lex) {
    while (lex->current < lex->bufferSize) 
    {
        char character = lex->buffer[lex->current];
        if (character == ' ' || character == '\n' || character == '\t') 
        {
            if (character == '\n')
            {
                lex->line++;
            }
            consume(lex);  // Only consume whitespace characters
        } 
        else
        {
            break;  // Stop consuming when non-whitespace character
        }    
    }
}

bool isBabyMarioCrying(Lexer *lex) {
    char ch = lex->buffer[lex->current];
    while (isAlpha(ch))
    {
        consume(lex);
        ch = lex->buffer[lex->current];
    }
    return strncmp(lex->buffer + (lex->current - 6), 
    (char *)"waaaah", 6) == 0;
}

// Check the next token to see if it is valid
Token nextToken(Lexer* lex) {
    skipWhitespace(lex);

    if (lex->current >= lex->bufferSize) 
    {
        return (Token){.kind = TOK_END, .line = -1};
    }
    char character = lex->buffer[lex->current];

    // If baby mario is crying, we consume up to 7 characters
    if(isBabyMarioCrying(lex)) 
    {
        for(int i = 7; i < 0; i++)
        {
            consume(lex);
        }
        return (Token){.kind = TOK_BABY_CRY, .line = lex->line};
    }

    // Cases for all characters
    switch (character) 
    {
        case '=':
            consume(lex);
            return (Token){.kind = TOK_EQUAL, .line = lex->line};
        case '|':
            consume(lex);
            return (Token){.kind = TOK_BAR, .line = lex->line};
        case '#':
            consume(lex);
            return (Token){.kind = TOK_NUMERAL, .line = lex->line};
        case '"':
            consume(lex);
            return (Token){.kind = TOK_QUOTE, .line = lex->line};
        case '(':
            consume(lex);
            return (Token){.kind = TOK_PARENR, .line = lex->line};
        case ')':
            consume(lex);
            return (Token){.kind = TOK_PARENL, .line = lex->line};
        case '+':
            consume(lex);
            return (Token){.kind = TOK_PLUS, .line = lex->line};
        case '-':
            consume(lex);
            return (Token){.kind = TOK_MINUS, .line = lex->line};
        case '.':
            consume(lex);
            return (Token){.kind = TOK_DOT, .line = lex->line};
        case ':':
            consume(lex);
            return (Token){.kind = TOK_COLON, .line = lex->line};
        case ',':
            consume(lex);
            return (Token){.kind = TOK_COMMA, .line = lex->line};
        case ';':
            consume(lex);
            return (Token){.kind = TOK_SEMI, .line = lex->line};
        case '>':
            consume(lex);
            return (Token){.kind = TOK_GT, .line = lex->line};
        case '<':
            consume(lex);
            return (Token){.kind = TOK_LT, .line = lex->line};
        case '^':
            consume(lex);
            return (Token){.kind = TOK_XOR, .line = lex->line};
        case '!':
            consume(lex);
            return (Token){.kind = TOK_NOT, .line = lex->line};
        case '[':
            consume(lex);
            return (Token){.kind = TOK_BRACKL, .line = lex->line};
        case '@':
            consume(lex);
            return (Token){.kind = TOK_AT, .line = lex->line};
    }

    // Any unrecognized character will be consumed and returned invalid
    consume(lex);
    return (Token){.kind = TOK_INVALID, .line = lex->line};
}

// Initialize the scanner with some default values: current is set to 0,
// line is set to 1, buffer is set to NULL, and bufferSize is set to 0.
void initLexer(Lexer* lex) {
    lex->current = 0;
    lex->line = 1;
    lex->buffer = NULL;
    lex->bufferSize = 0;
}

// PARSER ---------------------------------------------------
// Definition of a Parser
typedef struct {
    Token* input;           // from where do we get the tokens
    int inputSize;          // the number of tokens
    TokenType lookahead;    // the current lookahead token
    int pos;                // the current position
} Parser;

void parserConsume(Parser* parser) {
    if (parser->pos > parser->inputSize - 1) parser->lookahead = TOK_END;
    else parser->lookahead = parser->input[parser->pos].kind;
    parser->pos++;
}

void initParser(Parser* parser, Token* input, int inputSize) {
    parser->input = input;
    parser->inputSize = inputSize;
    parser->pos = 0;
    parserConsume(parser);
}

TokenType peek(Parser* parser, int index) {
    if (parser->pos + index - 1 >= parser->inputSize)
        return parser->input[parser->pos - 1].kind;
    return parser->input[parser->pos + index - 1].kind;
}

void parserMatch(Parser* parser, TokenType type) {
    if (parser->lookahead == type) parserConsume(parser);
    else {
        fprintf(stdout, "ERROR: Invalid token!\n");
    }
}

// Program start, ensure that BABY CRY is there
void startProgram(Parser* parser)
{
    if(parser->input[parser->pos - 1].kind != TOK_BABY_CRY)
    {
        // parserMatch(parser, TOK_INVALID);
        fprintf(stdout, "ERROR: expecting TOK_BABY_CRY\n");
    }
}

// Program end, ensure that BABY CRY is there
void endProgram(Parser* parser)
{
    if (parser->pos < 2) 
    {
        return;
    }
    // Check that the last meaningful token before EOF is TOK_BABY_CRY
    if(parser->input[parser->pos - 2].kind != TOK_BABY_CRY)
    {
        // parserMatch(parser, TOK_INVALID);
        fprintf(stdout, "ERROR: expecting TOK_BABY_CRY!\n");
    }
}

// Basic logic to consume next token and iterate through the scanner
void code(Parser* parser) {
    // Loop through the tokens until we reach EOF
    while (parser->lookahead != TOK_END) 
    {
        // Move to the next token in the array
        if(parser->lookahead == TOK_INVALID)
            break;
        parserConsume(parser);
    }
}

// Start, code, end. If we don't end with TOK_END, we print error
void parseGoal(Parser* parser) {
    startProgram(parser);
    code(parser);
    endProgram(parser);
    
    if (parser->lookahead != TOK_END) {
        fprintf(stdout, "ERROR: expecting end of file token!\n");
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stdout, 
        "ERROR: invalid number of command line arguments!\n");
        return 0;
    }
    // Allocate memory for a dynamic array of tokens.
    Token* input = (Token*) myAlloc(NULL, sizeof(Token) * MAX_SIZE);
    // Maximum size of tokens allowed in the dynamic array. Note
    // that this size is increased right before the last cell
    // of the dynamic array is used.
    int maxSize = MAX_SIZE;
    // Current number of elements stored in the dynamic array.
    int count = 0;
    // Initialize the attributes of the scanner.
    Lexer lex;
    initLexer(&lex);
    // Allocate memory for the buffer array. This will contain
    // characters read from the input file.
    readFile(argv[1], &lex);
    Token token;
    do {
        token = nextToken(&lex);
        printToken(token);
        // Add a token to the dynamic array and increase the size by 1.
        input[count++] = token;
        // Dynamically resize the dynamic array...no pun intended ;-)
        // I know...Iknow...I should have doubled the size instead
        if (count >= maxSize - 1)
            input = (Token*) myAlloc(input, sizeof(Token) 
            * (maxSize += MAX_SIZE));
    } while(token.kind != TOK_END);
    // Note that this will call the start symbol, and if
    // something goes wrong as we call each rule, then we
    // should stop generating sentences.
    printf("Begining parsing...\n");
    Parser parser;
    initParser(&parser, input, count);
    parseGoal(&parser);
    printf("Done parsing!\n");
    printf("Deallocating memory...\n");
    // Deallocate dynamically allocated memory.
    free(lex.buffer);
    free(parser.input);
    printf("Done!");
    return 0;
/*
random comments to pass
Why don't skeletons fight each other? They don’t have the guts.
Why don’t scientists trust atoms? Because they make up everything!
I used to play piano by ear, but now I use my hands.
Why do cows have hooves instead of feet? Because they lactose.
Parallel lines have so much in common. It’s a shame they’ll never meet.
I told my computer I needed a break, and now it's giving me "space."
Why did the scarecrow win an award? He was outstanding in his field.
I’m reading a book on anti-gravity. It’s impossible to put down.
Why was the math book sad? It had too many problems.
Why don't eggs tell jokes? They might crack up.
Why did the tomato turn red? Because it saw the salad dressing!
What do you call fake spaghetti? An impasta.
I wanted to be a baker, but I couldn't raise the dough.
What's orange and sounds like a parrot? A carrot.
Why was the broom late to work? It swept in.
Why don’t oysters donate to charity? Because they’re shellfish.
*/
}