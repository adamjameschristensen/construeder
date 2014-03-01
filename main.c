#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("Hello world!\n");
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
    return 0;
}
