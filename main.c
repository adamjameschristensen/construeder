/*
CSC 521 - Project #1

Title: Construeder, the math parser

Author: Adam Christensen

filename: CSC521P1ConstruederAdamChristensen.c

date last modified: 2014.03.03

input:
This application asks for math expressions and variable assignments

process:
The input for the type of expression is used to select the appropriate function.
The input is then used to calculate the corresponding value.

output:
The values or errors are displayed to the console.
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//Declare variables
char tokenCurrent;          //Holds the current token being examined
char variableLetters[10];   //Array for names of variables
int variableValues[10];     //Array for values of variables
int variableArrayPosition=0; //Counter for number of variables used

//Method signatures
void tokenGet();                    //Gets the next token to be examined
void tokenCheck(char,char*);        //Checks the token
void expressionParse();             //Setup stage 1
void expressionStart();             //Setup stage 2
void errorMsg();                    //Print an explaination when errors are encountered
void checkQuit();                   //Quit when "$$" is found
void checkVariableInquiry();        //Checks if ? is invoked to display variable value
void checkVariableAssign();         //Store a value in a variable

//Core calculation methods
int findExpression();       //Entire input "4x + 5 = 20"
int findTerm();             //Chunk "4x"
int findFactor();           //Chunk "4" or "x"
int findNumber();           //Chunk "4" or content of "x"
int findSingleDigit();      //Find digits 2,0 for "20"
int findVariableValue();    //

int main()
{
    /*
    Input: A program in the following language
    program --> 		statement_list $$
    statement_list --> 	statement statement_list
    statement --> 		variable = expression ; | ? variable | variable = const ;
    expression --> 	(variable | const) op (variable | const)
    variable -> 		letter letter*
    const -> 		(digit digit*) | (- digit digit*)
    letter --> 		A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z
    digit --> 		0|1|2|3|4|5|6|7|8|9
    op --> 			+ | - | * | /
    Output: Whenever invalid input is provided, an appropriate error message is printed.
    Whenever "? variable" is input, the value of the variable is output.
    */
    expressionParse(); //Begin the parse
    return 0;
}

void tokenGet()
{
    tokenCurrent = getchar(); //Get the next token
}

void tokenCheck(char tokenCurrentCheck, char* detailsErrorCheck)
{
    if (tokenCurrent == tokenCurrentCheck)  //If the expected token is found
    {
        tokenGet();                         //Get the next token
    }
    else                                    //If the expected token is not found
    {
        errorMsg(detailsErrorCheck);        //Display the appropriate error message
    }
}

void expressionParse()
{
    tokenGet();             //Get the first token
    expressionStart();      //Examine the token
}

void expressionStart()
{
    checkQuit(); //Check to see if the user wants to quit
    checkVariableInquiry(); //Check to see if a variable is being inquired upon
    checkVariableAssign(); //Check to see if a variable is being assigned
	int evaluationResult = findExpression();    //Look for an expression
	if(tokenCurrent == '\n')                     //If the end of the input string is found
	{
        printf("Result: %d\n",evaluationResult);    //End of parse. Print the result.
	}
    else
    {
        if (tokenCurrent == '\p') //If the token has been set to '\p'...
        {
            //Then a variable was stored. Exit method quietly by doing nothing
        }
        else
        {
            errorMsg("additional tokens after your expression");   //Input was not properly formed
        }
    }
	expressionParse(); //Restart parse
}

int findExpression()
{
	int evaluationResult = findTerm();    //Look for a term
    while (tokenCurrent == '+') //If adding
    {
        tokenCheck('+', "plus expected");
        evaluationResult = evaluationResult + findTerm(); //Add the terms
    }
    while (tokenCurrent == '-') //If subtracting
    {
        tokenCheck('-', "minus expected");
        evaluationResult = evaluationResult - findTerm(); //Subtract the terms
    }
    return evaluationResult; //Return result
}

int findTerm()
{
	int evaluationResult = findFactor();    //Look for a factor
	while (tokenCurrent == '*') //If multiplying
	{
        tokenCheck('*',"multiply expected");
        evaluationResult = evaluationResult * findFactor(); //Multiply
	}
	while (tokenCurrent == '/')
	{
        tokenCheck('/',"divide expected"); //If dividing
        int evaluationResultTemp = findFactor(); //Check the factor
        if (evaluationResultTemp != 0) //If the result is nonzero
        {
            evaluationResult = evaluationResult / evaluationResultTemp; //Calculate
        }
        else
        {
            errorMsg("divide by zero"); //If the factor was zero, report error
        }
	}
	return evaluationResult; //Return result
}

int findFactor()
{
    int evaluationResult = findNumber();    //Look for a number
    return evaluationResult; //Return result
}

int findNumber()
{
    int evaluationResult;
    if (isalpha(tokenCurrent)) //If the current token is alpha
    {
        evaluationResult = findVariableValue(); //Check for a variable
    }
    else
    {
        evaluationResult = findSingleDigit(); //Look for all single digits in the number
        while (isdigit(tokenCurrent)) //While there are more digits in a sequence
        {
            evaluationResult = 10 * evaluationResult + findSingleDigit(); //Combine them
        }
    }
    return evaluationResult; //Return result
}

int findSingleDigit()
{
        int evaluationResult;
        if (isdigit(tokenCurrent))                  //If the current token is a digit
        {
            evaluationResult = tokenCurrent - 48;  //Subtract the value of char "0" and save it to the current evaluationResult
            tokenCheck(tokenCurrent,"I am error");
        }
        else
        {
            if (tokenCurrent == '\n') //If the end of the string is found
            {
                tokenCurrent = '\p'; //Set the token to '\p'
            }
            else
            {
                errorMsg("a digit was expected");       //Display error message if a digit is not found
            }
        }
        return evaluationResult; //Return result
}

int findVariableValue()
{
    int evaluationResult;
    if (isalpha(tokenCurrent)) //Check for alpha variable name
    {
        int i;
        for(i=0; i<10;i++) //For all spots in array
        {
            if (variableLetters[i] == tokenCurrent) //Check for current variable name
            {
                evaluationResult = variableValues[i]; //If found, provide value
            }
        }
        tokenGet(); //Eat a token
    }
    return evaluationResult; //Return result
}

void checkVariableInquiry()
{
    if (tokenCurrent == '?') //If current token is a '?'
    {
        tokenGet(); //Get next token
        if (isalpha(tokenCurrent)) //If that is an alpha
        {
            int i;
            for(i=0; i<10;i++) //For each spot in the variable array
            {
                if (variableLetters[i] == tokenCurrent) //If current name is found
                {
                    printf("> %d\n",variableValues[i]); //Provide the value
                }
            }
        }
        else
        {
            errorMsg("Not a proper variable"); //Error if the variable is not found
        }
    }
}

checkVariableAssign()
{
    int foundSpot=0; //Boolean for variable storage confirmation
    if (isalpha(tokenCurrent)) //If current token is alpha
    {
        char varLetter = tokenCurrent;  //Save variable name
        tokenGet();                     //Advance a token
        if (tokenCurrent == '=')        //If current token is '='
        {
            tokenGet();                 //Advance a token
            int tempResult = findExpression(); //Find result of expression after '='
            int i;
            for(i=0; i<10;i++) //For each spot in variable array
            {
                if (variableLetters[i] == varLetter) //If a match to the current variable
                {
                    variableValues[i] = tempResult; //Overwrite the value
                    foundSpot=1; //Indicate the variable was successfully stored
                }
            }
            if (foundSpot == 0) //If the variable didn't already exist
            {
                variableValues[variableArrayPosition] = tempResult; //Put the value in the next available spot
                variableLetters[variableArrayPosition] = varLetter; //And save the name of the variable
                variableArrayPosition++; //Increment the counter
            }
        }
    }
}

void errorMsg(char* detailsError)
{
    printf("Error, %s\n",detailsError); //Display the passed error message
    expressionParse(); //Restart program
}

void checkQuit()
{
    if (tokenCurrent == '$') //If one '$' found
	{
        tokenGet();
        if (tokenCurrent == '$') //If two '$' found
	    {
            printf("Quit");
            exit(0); //Quit
	    }
	    else
        {
            errorMsg("a single $ is not a valid input"); //Error if only one '$'
        }
    }
}
