%{
#include "node.h"
#include <iostream>
#include <string>
extern int yylex();
extern void yyerror(const char* s);

// Global for the main block
NBlock* g_MainBlock = nullptr;

// Disable the warnings that Bison creates
#pragma warning(disable: 4065)
%}

/* Add one union member for each Node* type */
%union {
    Node* node;
	NBlock* block;
	NStatement* statement;
	NNumeric* numeric;
	NBoolean* boolean;
	std::string* string;
	int token;
}

%error-verbose

/* Terminal symbols */
%token <string> TINTEGER
%token <token> TLBRACE TRBRACE TSEMI TLPAREN TRPAREN
%token <token> TMAIN TROTATE TFORWARD TIF TELSE TATTACK TRANGED_ATTACK
%token <token> TIS_HUMAN TIS_PASSABLE TIS_RANDOM TIS_ZOMBIE

/* Statements */
%type <block> main_loop block
%type <statement> statement rotate forward ifElse attack rangedAttack
%type <boolean> boolean isHuman isPassable isRandom isZombie

/* Expressions */
%type <numeric> numeric

%%

main_loop	: TMAIN TLBRACE block TRBRACE { g_MainBlock = $3;
                                            g_MainBlock->SetMainBlock();}
;

block		: statement { $$ = new NBlock();
                        ($$)->AddStatement($1); }
/* Add support for multiple statements in a block */
            | block statement { ($1)->AddStatement($2); }
;

//statements
statement	: rotate TSEMI
            | forward TSEMI
            | ifElse
            | attack TSEMI
            | rangedAttack TSEMI
;

//booleans
boolean     : isHuman
            | isPassable
            | isRandom
            | isZombie
;
			
rotate		: TROTATE TLPAREN numeric TRPAREN { $$ = new NRotate($3); }
;

forward     : TFORWARD TLPAREN TRPAREN { $$ = new NForward(); }
;

ifElse      : TIF TLPAREN boolean TRPAREN TLBRACE block TRBRACE TELSE TLBRACE block TRBRACE { $$ = new NIfElse(($3),($6),($10)); }
;

attack      : TATTACK TLPAREN TRPAREN { $$ = new NAttack(); }
;

isHuman     : TIS_HUMAN TLPAREN numeric TRPAREN { $$ = new NIsHuman($3); }
;

isPassable  : TIS_PASSABLE TLPAREN TRPAREN { $$ = new NIsPassable(); }
;

isRandom    : TIS_RANDOM TLPAREN TRPAREN { $$ = new NIsRandom(); }
;

isZombie    : TIS_ZOMBIE TLPAREN numeric TRPAREN { $$ = new NIsZombie($3); }
;

rangedAttack: TRANGED_ATTACK TLPAREN TRPAREN { $$ = new NRangedAttack(); }
;
			
numeric		: TINTEGER { $$ = new NNumeric(*($1)); }
;

%%
