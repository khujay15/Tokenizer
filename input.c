#include <stdio.h>
/* Definition of Function
 * @Param char *orig : Original String, Real data
 * @Param char *diff : Comparable String, Constant data
 */
int comp_str(char *orig, char *diff)
{
	int res;
	while ( *orig != '\0' && *diff != '\0' && (*orig) == (*diff))
	{
		orig++; diff++;
	}
	res = (int)(*orig) - (int)(*diff);
	if (res != 0) res = 1;
	return res;
}

const char* data_type[] =
{ "enum", "struct", "union", "bool", "unsigned", "double", "short", "long", "float", "int", "char", "void", "EOD"};

int main()
{
	enum parse_result { NO_ARG, DOUBLE_QUOTE, PREPROCESSOR, PRE_ARG, DATA_TYPE, STRING, FUNCTION,WHITE_SPACE,RETURN};
	enum parse_result flag;
	flag = NO_ARG;
	char buf[2048];
	char *cp;
	cp = buf;
	char c;

	while ( c = getchar() )
	{
		if ( c == EOF ) break; /* End Of File */

		if ( flag != DOUBLE_QUOTE )
		{
			if ( ( c != '#' ) 
			&& ( c != '<' ) 
			&& ( c != '>' ) 
			&& ( c != ' ' ) 
			&& ( c != '\t' ) 
			&& ( c != '\n' )
			&& ( c != '(' ) 
			&& ( c != ')' ) 
			&& ( c != ',' ) 
			&& ( c != '*' ) 
			&& ( c != '{' ) 
			&& ( c != '}' ) 
			&& ( c != '[' ) 
			&& ( c != ']' ) 
			&& ( c != ';' ) )
			{
				if ( c == '\"' )
				{
					flag = DOUBLE_QUOTE;
				}
				else if ( c == ' ' || c == '\t' || c == '\n' )
				{	flag = WHITE_SPACE;
					cp++;
				}
				else
				{
					*cp = c;
					cp++;
				}
			}
			else /* # < > WS ( ) , * [ ] */
			{
				/* Start of DATA_TYPE flag Filtering */
				if ( c == '(' && flag == NO_ARG ) flag = FUNCTION;
				
				char **datatype;
				datatype=data_type;
				*cp = '\0';
				while (comp_str(buf, *datatype))
				{
					if (!comp_str(*datatype, "EOD")) 
						break;

					datatype++;
				}
				if ( flag == DATA_TYPE ) flag = NO_ARG;
				if (comp_str(*datatype, "EOD"))
				 flag = DATA_TYPE; 
				if( !comp_str(buf,"return") ) flag = RETURN;
				/* Start of DATA_TYPE flag Filtering */
				printf ("%s", buf);

				if ( cp != buf )
				{
					/* Print Token to Debugging */
					switch (flag)
					{
						case NO_ARG : printf("\t: NO_ARG"); break;
						case PREPROCESSOR : printf("\t: PREPROCESSOR"); break;
						case PRE_ARG : printf("\t: PRE_ARG"); break;
						case DATA_TYPE : printf("\t: DATA_TYPE"); break;
						case FUNCTION : printf("\t: FUNCTION"); break;
						case STRING: printf("\t: STRING"); break;
						case RETURN: printf("\t: RETURN"); break;
						default:
							break;
					}
					printf("\n");
				}

				if ( c == '#' ) flag = PREPROCESSOR;
				if ( c == '<' && flag == PREPROCESSOR ) flag = PRE_ARG;
				if ( c == '>' && flag == PRE_ARG ) flag = NO_ARG;
				if ( c == '(' && flag == NO_ARG ) flag = FUNCTION;
				if ( c == ')' && flag == FUNCTION ) flag = NO_ARG;
				if ( flag != STRING ) cp = buf;
				if ( flag == STRING ) flag=NO_ARG; cp =buf;
			}
		} /* if ( flag != DOUBLE_QUOTE ) */
		else
		{
			if ( c == '\"' ) 
			{
				flag = STRING;			
			}
			else
			{
				*cp = c;
				cp++;	
				
			}
		} /* End of if ( flag != DOUBLE_QUOTE ) */

	}
	return 0;
}
