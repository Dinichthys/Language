#ifndef READ_LANG_H
#define READ_LANG_H

#include <stdio.h>
#include <stdlib.h>

#include "language.h"
#include "parse_flags_lang.h"

static const size_t kProgramLen      = 10000;
static const size_t kVarMaxNum       = 200;
static const size_t kVarTablesMaxNum = 100;
static const size_t kTokenNumber     = 27;

static const char kCommentSymbol = '#';
static const char kEOF           = '\0';

__attribute_maybe_unused__ static const char* kRedTextTerminal    = "\033[1;31m";
__attribute_maybe_unused__ static const char* kNormalTextTerminal = "\033[0m";

//-----DSL----------------------------------------------------------------------------------------------------

#define TOKEN_POSITION tokens [*token_index].number_of_line, tokens[*token_index].line_pos

#define SHIFT_TOKEN (*token_index)++;

#define REMOVE_TOKEN (*token_index)--;

#define CHECK_RESULT                \
        if (result != kDoneLang)    \
        {                           \
            return result;          \
        }

#define CHECK_TOKEN_OP(token_type, op_type)\
    ((tokens [*token_index].type == token_type) && (tokens [*token_index].value.operation == op_type))

#define TOKEN_TYPE tokens [*token_index].type

#define CHECK_END                                       \
        if (tokens [*token_index].type == kEndToken)    \
        {                                               \
            return kDoneLang;                           \
        }

//------------------------------------------------------------------------------------------------------------

typedef struct variables
{
    size_t var_num;
    char var_table [kVarMaxNum] [kWordLen];
}variables_t;

typedef struct token
{
    enum NodeType type;

    union value
    {
        double      number;
        char        variable [kWordLen];
        enum OpType operation;
    } value;

    size_t line_pos;
    size_t number_of_line;
} token_t;

typedef struct token_pattern
{
    token_t token;
    const char* str_token;
} token_pattern_t;

#define TOKEN_PATTERN(index, token_type, op_type, key_word)\
    [index]  = {.token = {.type = token_type, {.operation = op_type}}, .str_token = key_word}

const token_pattern_t kTokenTypes [kTokenNumber] =
{
//----------------------|        Available token meaning          |       Key word for the token        |
//----------------|index| type of node |      meaning of node     |                                     |
//     TOKEN_PATTERN (  0,      kMainFunc,  kMain,                     "Блатной"                           ),
//
//     TOKEN_PATTERN (  1,      kArithm,    kAdd,                      "хапнуть"                           ),
//     TOKEN_PATTERN (  2,      kArithm,    kSub,                      "вырезать"                          ),
//     TOKEN_PATTERN (  3,      kArithm,    kMul,                      "Здравствуйте_я_сотрудник_Сбербанка"),
//     TOKEN_PATTERN (  4,      kArithm,    kDiv,                      "твоя_доля"                         ),
//     TOKEN_PATTERN (  5,      kArithm,    kPow,                      "короновать"                        ),
//     TOKEN_PATTERN (  6,      kArithm,    kSqrt,                      "кореш"                            ),
//
//     TOKEN_PATTERN (  7,      kFunc,      kSin,                      "синенький"                         ),
//     TOKEN_PATTERN (  8,      kFunc,      kCos,                      "костлявый"                         ),
//     TOKEN_PATTERN (  9,      kFunc,      kTg,                       "штанга"                            ),
//     TOKEN_PATTERN (  10,     kFunc,      kCtg,                      "котяра"                            ),
//
//     TOKEN_PATTERN (  11,     kFunc,      kLn,                       "натурал"                           ),
//     TOKEN_PATTERN (  12,     kFunc,      kLog,                      "лох"                               ),
//
//     TOKEN_PATTERN (  13,     kCycle,     kFor,                      "мотать_срок"                       ),
//     TOKEN_PATTERN (  14,     kCycle,     kWhile,                    "чалиться"                          ), // ПОЖИЗНЕННОЕ
//
//     TOKEN_PATTERN (  15,     kCond,      kIf,                       "УДО"                               ),
//
//     TOKEN_PATTERN (  16,     kSym,       kAssign,                   "отжать"                            ),
//     TOKEN_PATTERN (  17,     kSym,       kParenthesesBracketOpen,   "заковали"                          ),
//     TOKEN_PATTERN (  18,     kSym,       kParenthesesBracketClose,  "в_браслеты"                        ),
//     TOKEN_PATTERN (  19,     kSym,       kCurlyBracketOpen,         "век"                               ),
//     TOKEN_PATTERN (  20,     kSym,       kCurlyBracketClose,        "воли_не_видать"                    ),
//     TOKEN_PATTERN (  21,     kSym,       kCommandEnd,               "откинуться"                        ),
//
//     TOKEN_PATTERN (  21,     kFunc,      kIn,                       "проставиться"                      ),
//     TOKEN_PATTERN (  22,     kFunc,      kOut,                      "покукарекай"                       ),

//-------------------|                  Available token meaning                   |    Key word for the token    |
//-------------------|    type of node   |             meaning of node            |                              |
    [0]  = {.token = {.type = kMainFunc, {.operation = kMain}},                    .str_token = "Блатной"        },

    [1]  = {.token = {.type = kArithm,   {.operation = kSub}},                     .str_token = "вырезал"        },
    [2]  = {.token = {.type = kArithm,   {.operation = kAdd}},                     .str_token = "хапнул"         },
    [3]  = {.token = {.type = kArithm,   {.operation = kDiv}},                     .str_token = "твоя_доля"      },
    [4]  = {.token = {.type = kArithm,   {.operation = kMul}},                     .str_token = "Здравствуйте_я_сотрудник_Сбербанка"},
//-------------------нет в процессоре (снизу)
    [5]  = {.token = {.type = kArithm,   {.operation = kPow}},                     .str_token = "короновали"     },
//-------------------нет в процессоре (сверху)
    [6]  = {.token = {.type = kFunc,     {.operation = kSqrt}},                    .str_token = "кореш"          },

    [7]  = {.token = {.type = kFunc,     {.operation = kSin}},                     .str_token = "синенький"      },
    [8]  = {.token = {.type = kFunc,     {.operation = kCos}},                     .str_token = "костлявый"      },
//-------------------нет в процессоре (снизу)
    [9]  = {.token = {.type = kFunc,     {.operation = kTg}},                      .str_token = "штанга"         },
    [10] = {.token = {.type = kFunc,     {.operation = kCtg}},                     .str_token = "котяра"         },

    [11] = {.token = {.type = kFunc,     {.operation = kLn}},                      .str_token = "натурал"        },
    [12] = {.token = {.type = kFunc,     {.operation = kLog}},                     .str_token = "лох"            },
//-------------------нет в процессоре (сверху)
    [13] = {.token = {.type = kCycle,    {.operation = kFor}},                     .str_token = "мотать_срок"    },
    [14] = {.token = {.type = kCycle,    {.operation = kWhile}},                   .str_token = "чалиться"       },

    [15] = {.token = {.type = kCond,     {.operation = kIf}},                      .str_token = "УДО"            },

    [16] = {.token = {.type = kSym,      {.operation = kAssign}},                  .str_token = "отжал"          },
    [17] = {.token = {.type = kSym,      {.operation = kParenthesesBracketOpen}},  .str_token = "заковали"       },
    [18] = {.token = {.type = kSym,      {.operation = kParenthesesBracketClose}}, .str_token = "в_браслеты"     },
    [19] = {.token = {.type = kSym,      {.operation = kCurlyBracketOpen}},        .str_token = "век"            },
    [20] = {.token = {.type = kSym,      {.operation = kCurlyBracketClose}},       .str_token = "воли_не_видать" },
    [21] = {.token = {.type = kSym,      {.operation = kCommandEnd}},              .str_token = "откинуться"     },
    [22] = {.token = {.type = kSym,      {.operation = kComma}},                   .str_token = "перо_под_ребро" },

    [23] = {.token = {.type = kFunc,     {.operation = kIn}},                      .str_token = "проставиться"   },
    [24] = {.token = {.type = kFunc,     {.operation = kOut}},                     .str_token = "покукарекай"    },

    [25] = {.token = {.type = kFunc,     {.operation = kReturn}},                  .str_token = "АТАС_ШМОН"      },

    [26] = {.token = {.type = kType,     {.operation = kDouble}},                  .str_token = "фраер"          }
};

#undef TOKEN_PATTERN

enum LangError ReadProgram (settings_of_program_t* const set, node_t** const root);

#endif // READ_LANG_H
