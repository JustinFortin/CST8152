/*************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2021
*************************************************************
* File name: parser.h
* Compiler: MS Visual Studio 2019
* Author: Svillen Ranev - Paulo Sousa - Abdulah
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013, or 014]
* Assignment: A3.
* Date: Jan 01 2021
* Purpose: This file is the main header for Parser (.h)
* Function list: (...).
*************************************************************/
/* TODO_201: Adjust the function header */

#include "parser.h"

/*************************************************************
 * Process Parser
 ************************************************************/
void startParser(void) {
	lookahead = tokenizer();
	program();
	matchToken(SEOF_T, NO_ATTR);
	printf("%s\n", "PLATY: Source file parsed");
}

/*************************************************************
 * Match Token
 ************************************************************/
 /* TODO_202: Continue the development */
void matchToken(int tokenCode, int tokenAttribute) {
	//int matchFlag = 1;
	if (lookahead.code != tokenCode) {
		syncErrorHandler(tokenCode);
		return;
	}
	switch (lookahead.code) {
	case KW_T: case LOG_OP_T: case ART_OP_T: case REL_OP_T:
		if (lookahead.attribute.intValue != tokenAttribute) {
			syncErrorHandler(tokenCode);
			return;
		}
	default:
		;
	}
	
	if (lookahead.code == SEOF_T) {
		return;
	}
	lookahead = tokenizer();

	if (lookahead.code == ERR_T) {
		printError();
		lookahead = tokenizer();
		++syntaxErrorNumber;
	}
	else {
		syncErrorHandler(tokenCode);
	}
}

/*************************************************************
 * Syncronize Error Handler
 ************************************************************/
 /* TODO_203: Continue the development */
void syncErrorHandler(int syncTokenCode) {
	// Continue the code
	syntaxErrorNumber++;
	while (lookahead.code != syncTokenCode) {
		if (lookahead.code == SEOF_T) {
			exit(syntaxErrorNumber);
		}
		lookahead = tokenizer();
	}
	if (lookahead.code != SEOF_T) {
		lookahead = tokenizer();
	}
	return;

}


/*************************************************************
 * Print Error
 ************************************************************/
 /* TODO_204: Continue the development */
void printError() {
	Token t = lookahead;
	printf("PLATY: Syntax error:  Line:%3d\n", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
	case ERR_T: /*Error Token*/
		printf("%s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:/*Source End of File*/
		printf("%d\n", t.attribute.seofType);
		break;
	case AVID_T: case SVID_T:/*Arithmatic and String Variable Identifiers*/
		printf("%s\n", t.attribute.vidLexeme);
		break;
	case FPL_T: /*Floating point literal*/
		printf("%f\n", t.attribute.floatValue);
		break;
	case INL_T:/* Integer literal token */
		printf("%d\n", t.attribute.intValue);
		break;
	case STR_T:/* String literal token */
		printf("%s\n", bGetContent(stringLiteralTable, t.attribute.contentString));
		break;
	case SCC_OP_T: /* String concatenation operator token: ++*/
		printf("NA\n");
		break;
	case ASS_OP_T: /* Assignment operator token */
		printf("NA\n");
		break;
	case ART_OP_T:  /* Arithmetic operator token */
		printf("%d\n", t.attribute.intValue);
		break;
	case REL_OP_T: /* Relational operator token */
		printf("%d\n", t.attribute.intValue);
		break;
	case LOG_OP_T:/* Logical operator token */
		printf("%d\n", t.attribute.intValue);
		break;
	case LPR_T: /* Left parenthesis token */
		printf("NA\n");
		break;
	case RPR_T: /* Right parenthesis token */
		printf("NA\n");
		break;
	case LBR_T:/* Left brace token */
		printf("NA\n");
		break;
	case RBR_T:  /* Right brace token */
		printf("NA\n");
		break;
	case KW_T:  /* Keyword token */
		printf("%s\n", keywordTable[t.attribute.intValue]);
		break;
	case COM_T: /* Comma token */
		printf("NA\n");
		break;
	case EOS_T:/* End of statement (semicolon) */
		printf("NA\n");
		break;
	case RTE_T:/* Run-time error token. Not sure if needed*/
		printf("NA\n");
		break;
	default:
		printf("PLATY: Scanner error: invalid token code: %d\n", t.code);

		// Continue the code
	// Continue the code (other cases)
	}
}

/* TODO_205: Continue the development (all non-terminal functions) */

/*************************************************************
 * Program statement
 * BNF: <program> -> PLATYPUS { <opt_statements> }
 * FIRST(<program>)= {KW_T (MAIN)}.
 ************************************************************/
void program(void) {
	matchToken(KW_T, MAIN);
	matchToken(LBR_T, NO_ATTR);
	optionalStatements();
	matchToken(RBR_T, NO_ATTR);
	printf("%s\n", "PLATY: Program parsed");
}
/*************************************************************
 * Optional statement
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void optionalStatements(void) {
	switch (lookahead.code) {
	case AVID_T: case SVID_T:
		statements();
		break;
	case KW_T:
		if (lookahead.attribute.codeType == IF
			|| lookahead.attribute.codeType == WHILE
			|| lookahead.attribute.codeType == READ
			|| lookahead.attribute.codeType == WRITE) {
			statements();
			break;
		}
	default:
		printf("%s\n", "PLATY: Optional statements parsed");
	}

}
/*************************************************************
 * Statements
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void statements(void) {
	statement();
	statementsPrime();
}
/*************************************************************
 * Statements Prime
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void statementsPrime(void) {
	switch (lookahead.code) {
	case AVID_T: case SVID_T:
		statement();
		statementsPrime();
	case KW_T:
		if (lookahead.attribute.codeType == IF ||
			lookahead.attribute.codeType == WHILE ||
			lookahead.attribute.codeType == READ ||
			lookahead.attribute.codeType == WRITE) {
			statement();
			statementsPrime();
		}
		break;
	}
}
/*************************************************************
 * Statement
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void statement(void) {
	switch (lookahead.code) {
	case AVID_T: case SVID_T:
		assignmentStatement();
		break;
	case KW_T:
		if (lookahead.attribute.codeType == IF) {
			selectionStatememnt();
		}
		if (lookahead.attribute.codeType == WHILE) {
			iterationStatement();
		}
		if (lookahead.attribute.codeType == READ) {
			inputStatement();
		}
		if (lookahead.attribute.codeType == WRITE) {
			outputStatement();
		}
		break;
	default:
		printError();
		break;
	}
}
/*************************************************************
 * Assignment Statement
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void assignmentStatement(void) {
	assignmentExpression();
	matchToken(EOS_T, NO_ATTR);
	printf("%s\n", "PLATY: Assignment Statement Parsed");
}
/*************************************************************
 * Assignment Expression
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void assignmentExpression(void) {
	switch (lookahead.code) {
	case AVID_T:
		matchToken(AVID_T, NO_ATTR);
		matchToken(ASS_OP_T, NO_ATTR);
		arithmaticExpression();
		printf("%s\n", "PLATY: Assignment Expression Parsed");
		break;

	case SVID_T:
		matchToken(SVID_T, NO_ATTR);
		matchToken(ASS_OP_T, NO_ATTR);
		stringExpression();
		printf("%s\n", "PLATY: Assignment Expression Parsed");
		break;

	default:
		printError();
		break;
	}
}
/*************************************************************
 * Selection Statement
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void selectionStatememnt(void) {
	/*Matching If*/
	matchToken(KW_T, IF);
	preCondition();
	matchToken(LPR_T, NO_ATTR);
	conditionalExpression();
	matchToken(RPR_T, NO_ATTR);

	/*Matching Then*/
	matchToken(KW_T, THEN);
	matchToken(LBR_T, NO_ATTR);
	optionalStatements();
	matchToken(RBR_T, NO_ATTR);

	/*Matching Else*/
	matchToken(KW_T, ELSE);
	matchToken(LBR_T, NO_ATTR);
	optionalStatements();
	matchToken(RBR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);

	printf("%s\n", "PLATY: Selection statement parsed");
}
/*************************************************************
 * Iteration Statement
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void iterationStatement(void) {
	/*Matching keyword While*/
	matchToken(KW_T, WHILE);
	preCondition();
	matchToken(LPR_T, NO_ATTR);
	conditionalExpression();
	matchToken(RPR_T, NO_ATTR);
	/*Matching keyword Do*/
	matchToken(KW_T, DO);
	matchToken(LBR_T, NO_ATTR);
	statements();
	matchToken(RBR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);

	printf("%s\n", "PLATY: Iteration Statement Parsed");

}
/*************************************************************
 * Pre Condition
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void preCondition(void) {
	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.intValue == TRUE) {
			matchToken(KW_T, TRUE);
		}
		else if (lookahead.attribute.intValue == FALSE) {
			matchToken(KW_T, FALSE);
		}
		else {
			printError();
			break;
		}
	default:
		printError();
		break;
	}
}
/*************************************************************
 * Input Statement
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void inputStatement(void) {
	matchToken(KW_T, READ);
	matchToken(LPR_T, NO_ATTR);
	variableList();
	matchToken(RPR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("%s\n", "PLATY: Input statement parsed");
}
/*************************************************************
 * Variable List
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void variableList(void) {
	variableIdentifier();
	variableListPrime();
	printf("%s\n", "PLATY: Variable list parsed");
}
/*************************************************************
 * Variable List Prime
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void variableListPrime(void) {
	switch (lookahead.code) {
	case COM_T:
		matchToken(COM_T, NO_ATTR);
		variableIdentifier();
		variableListPrime();
		break;
	}
}
/*************************************************************
 * Variable Identifier
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void variableIdentifier(void) {
	switch (lookahead.code) {
	case AVID_T:
		matchToken(AVID_T, NO_ATTR);
		break;
	case SVID_T:
		matchToken(SVID_T, NO_ATTR);
		break;
	default:
		printError();
		break;
	}
}
/*************************************************************
 * Output Statement
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void outputStatement(void) {
	matchToken(KW_T, WRITE);
	matchToken(LPR_T, NO_ATTR);
	outputStatementPrime();
	matchToken(RPR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("%s\n", "PLATY: Output Statement Parsed");
}
/*************************************************************
 * Output Statement Prime
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void outputStatementPrime(void) {
	switch (lookahead.code) {
	case AVID_T: case SVID_T:
		variableList();
		break;
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		printf("%s\n", "PLATY: Output list (string literal) parsed");
	default:
		printf("%s\n", "PLATY: Output list (empty) parsed");
	}
}
/*************************************************************
 * Opt Variable List
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void optVariableList(void) {
	switch (lookahead.code) {
	case AVID_T: case SVID_T:
		variableList();
		break;
	default:
		printf("%s\n", "PLATY: Output list (empty) parsed");
	}
}
/*************************************************************
 * Output List
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void outputList(void) {
	switch (lookahead.code) {
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		printf("%s\n", "PLATY: Output list (string literal) parsed");
	default:
		optVariableList();
		break;
	}
}
/*************************************************************
 * Arithmatic Expression
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void arithmaticExpression(void) {
	switch (lookahead.code) {
	case ART_OP_T:
		if (lookahead.attribute.arithmeticOperator == ADD ||
			lookahead.attribute.arithmeticOperator == SUB) {
			unaryArithmaticExpression();
		}
		else {
			printError();
			break;
		}
		printf("%s\n", "PLATY: Arithmatic Expression Parsed");
		break;
	case AVID_T: case FPL_T: case INL_T: case LPR_T:
		additiveArithmaticExpression();
		printf("%s\n", "PLATY: Arithmatic Expression Parsed");
	default:
		printError();
		break;
	}
}
/*************************************************************
 * Unary Arithmatic Expression
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void unaryArithmaticExpression(void) {
	switch (lookahead.code) {
	case ART_OP_T:
		if (lookahead.attribute.arithmeticOperator == SUB ||
			lookahead.attribute.arithmeticOperator == ADD) {
			matchToken(ART_OP_T, lookahead.attribute.arithmeticOperator);
			primaryArithmaticExpression();
		}
		else {
			printError();

		}
		break;
	}
	printf("%s\n", "PLATY: Unary arithmetic expression parsed");
}
/*************************************************************
 * Additive Arithmatic Expression
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void additiveArithmaticExpression(void) {
	multiplicativeArithmaticExpression();
	additiveArithmaticExpressionPrime();
}
/*************************************************************
 * Additive Arithmatic Expression Prime
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void additiveArithmaticExpressionPrime(void) {
	if (lookahead.code == ART_OP_T) {
		if (lookahead.attribute.arithmeticOperator == ADD ||
			lookahead.attribute.arithmeticOperator == SUB) {
			matchToken(ART_OP_T, lookahead.attribute.arithmeticOperator);
			multiplicativeArithmaticExpression();
			additiveArithmaticExpressionPrime();
			printf("%s\n", "PLATY: Additive Arithmatic Expression Parsed");
		}
	}
}
/*************************************************************
 * Multiplicative Arithmatic Expression
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void multiplicativeArithmaticExpression(void) {
	primaryArithmaticExpression();
	multiplicativeArithmaticExpressionPrime();

}
/*************************************************************
 * Multiplicative Arithmatic Expression Prime
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void multiplicativeArithmaticExpressionPrime(void) {
	if (lookahead.code == ART_OP_T) {
		if (lookahead.attribute.arithmeticOperator == MUL ||
			lookahead.attribute.arithmeticOperator == DIV) {
			matchToken(ART_OP_T, lookahead.attribute.arithmeticOperator);
			primaryArithmaticExpression();
			multiplicativeArithmaticExpressionPrime();
			printf("%s\n", "PLATY: Multiplicative arithmetic expression parsed");
		}
	}
}
/*************************************************************
 * Primary Arithmatic Expression
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void primaryArithmaticExpression(void) {
	switch (lookahead.code) {
	case AVID_T: case FPL_T: case INL_T:
		matchToken(lookahead.code, NO_ATTR);
		printf("%s\n", "PLATY: Primary arithmetic expression parsed");
		break;
	case LPR_T:
		matchToken(LPR_T, NO_ATTR);
		arithmaticExpression();
		matchToken(RPR_T, NO_ATTR);
		printf("%s\n", "PLATY: Primary arithmetic expression parsed");
		break;
	default:
		printError();
		break;
	}
}
/*************************************************************
 * String Expression
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void stringExpression(void) {
	primaryStringExpression();
	stringExpressionPrime();
	printf("%s\n", "PLATY: String expression parsed");
}
/*************************************************************
 * String Expression Prime
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void stringExpressionPrime(void) {
	if (lookahead.code == SCC_OP_T) {
		matchToken(SCC_OP_T, NO_ATTR);
		primaryStringExpression();
		stringExpressionPrime();
	}
}
/*************************************************************
 * Primary String Expression
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void primaryStringExpression(void) {
	switch (lookahead.code) {
	case SVID_T: case STR_T:
		matchToken(lookahead.code, NO_ATTR);
		printf("%s\n", "PLATY: Primary string expression parsed");
		break;
	default:
		printError();
		break;
	}
}
/*************************************************************
 * Conditional Expression
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void conditionalExpression(void) {
	logicalOrExpression();
	printf("%s\n", "PLATY: Conditional expression parsed");
}
/*************************************************************
 * Logical Or Expression
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void logicalOrExpression(void) {
	logicalAndExpression();
	logicalOrExpressionPrime();
}
/*************************************************************
 * Logical Or Expression Prime
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void logicalOrExpressionPrime(void) {
	if (lookahead.code == LOG_OP_T) {
		if (lookahead.attribute.logicalOperator == OR) {
			matchToken(LOG_OP_T, OR);
			logicalAndExpression();
			logicalOrExpressionPrime();
			printf("%s\n", "PLATY: Logical OR Expression Parsed");
		}
	}
}
/*************************************************************
 * Logical And Expression
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void logicalAndExpression(void) {
	logicalNotExpression();
	logicalAndExpressionPrime();
}
/*************************************************************
 * Logical And Expression Prime
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void logicalAndExpressionPrime(void) {
	if (lookahead.code == LOG_OP_T) {
		if (lookahead.attribute.logicalOperator == AND) {
			matchToken(LOG_OP_T, AND);
			logicalNotExpression();
			logicalAndExpressionPrime();
			printf("%s\n", "PLATY: Logical AND Expression Parsed");
		}
	}
}
/*************************************************************
 * Logical Not Expression
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void logicalNotExpression(void) {
	if (lookahead.code == LOG_OP_T) {
		if (lookahead.attribute.logicalOperator == NOT) {
			matchToken(LOG_OP_T, NOT);
			relationalExpression();
		}
		else {
			relationalExpression();
		}
	}
}
/*************************************************************
 * Relational Expression
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void relationalExpression(void) {
	switch (lookahead.code) {
	case AVID_T: case FPL_T: case INL_T:
		primaryARelationalExpression();
		break;
	case SVID_T: case STR_T:
		primarySRelationalExpression();
		break;
	default:
		printError();
		break;
	}
}
/*************************************************************
 * Relational A Expression
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void relationalAExpression(void) {
	primaryARelationalExpression();
	relationalAExpressionPrime();
	primaryARelationalExpression();
}
/*************************************************************
 * Relational A Expression Prime
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void relationalAExpressionPrime(void) {
	if (lookahead.code == REL_OP_T) {
		matchToken(REL_OP_T, lookahead.attribute.logicalOperator);
	}
	else {
		printError();
	}
}
/*************************************************************
 * Primary A Relational Expression
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void primaryARelationalExpression(void) {
	switch (lookahead.code) {
	case AVID_T: case FPL_T: case INL_T:
		matchToken(lookahead.code, NO_ATTR);
		break;
	default:
		printError();
		break;
	}
	printf("%s\n", "PLATY: Primary a_relational expression parsed");
}
/*************************************************************
 * Relational S Expression
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void relationalSExpression(void) {
	primarySRelationalExpression();
	relationalSExpressionPrime();
	primarySRelationalExpression();
}
/*************************************************************
 * Relational S Expression Prime
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void relationalSExpressionPrime(void) {
	if (lookahead.code == REL_OP_T) {
		matchToken(REL_OP_T, lookahead.attribute.logicalOperator);
	}
	else {
		printError();
	}
}
/*************************************************************
 * Primary S Relational Expression
 * TODO: Include the grammar definition here
 * TODO: Include the first set here
 ************************************************************/
void primarySRelationalExpression(void) {
	primaryStringExpression();
	printf("%s\n", "PLATY: Primary s_relational expression parsed");
}
// TODO: Continue the code...
