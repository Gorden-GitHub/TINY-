#include "pch.h"
#include <string>
#include "GLOBALS.H"
#include "PARSE.H"
#include "UTIL.H"
#include "SCAN.H"

using namespace std;

#pragma warning(disable:4996)

/* allocate global variables */
int lineno = 0;
FILE* source;
FILE* listing;
FILE* code;

/* allocate and set tracing flags */
int EchoSource = FALSE;
int TraceScan = FALSE;
int TraceParse = FALSE;
int TraceAnalyze = FALSE;
int TraceCode = FALSE;

int Error = FALSE;

int create_tree(string str1, string str2)
{
	TreeNode* syntaxTree;
	source = fopen(str1.c_str(), "r");
	listing = fopen(str2.c_str(), "w");
    fprintf(listing, "\nTINY Syntax Analyse!\n");
    int linepos = 0;
    int bufsize = 0;
    EOF_flag = FALSE;
    syntaxTree = parse();
    fprintf(listing, "\nTINY Syntax tree:\n");
    printTree(syntaxTree);
    fclose(listing);
    fclose(source);
    return 0;
}