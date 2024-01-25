#include "tokens.hpp"
#include <stdio.h>
#include <string>


// bool check_hex_pair(const char char1, const char char2) {
//     if (char1 >= '0' && char1 <= '7' && 
// 	(char2 >= '0' && char2 <= '9' || char2 >= 'A' && char2 <= 'F' || 
// 	char2 >= 'a' && char2 <= 'f')) {
//         return true;
//     }
//     return false;
// }


char replaceHexWithAscii(const char c1, const char c2) { 	// COPY!!!!
	int digit2 = -1;
    if (c1 >= '0' && c1 <= '7') {
        int digit1 = c1 - '0';
        if (c2 >= '0' && c2 <= '9') {
            digit2 = c2 - '0';
        } else if (c2 >= 'a' && c2 <= 'f') {
            digit2 = c2 - 'a' + 10;
        } else if (c2 >= 'A' && c2 <= 'F') {
            digit2 = c2 - 'A' + 10;
        } else {
            return -1;
        }
        return digit1 * 16 + digit2;
    }
    return -1;
}

void showCommentToken(const char * token)  //printing a comment token
{
    printf("%d %s %s\n",yylineno, token, "//");
}


void errorManager(char* token){
	int tokenLen = strlen(token);
	string errorMsg = "";
	string output;
	for (int i = 0; i < tokenLen - 1; ++i) {
		if(token[i] == '\\') {
            if (token[i+1] == 'x') {
				if (i+1 >= tokenLen-1){ //Its /x
					errorMsg.append("Error undefined escape sequence ");
					errorMsg.push_back(yytext[i + 1]);
					cout << errorMsg << endl;
					exit(0);
				}
				if (i+2 >= tokenLen-1){ //Its /xA
					errorMsg.append("Error undefined escape sequence ");
					errorMsg..push_back(yytext[i + 1]);
					if (yytext[i + 2] != '\"') {
						errorMsg..push_back(yytext[i + 2]);
					}
					cout << errorMsg << endl;
					exit(0);
				}
				newVal = replaceHexWithAscii(yytext[i + 2], yytext[i + 3])
				if (newVal == -1){
					//Bad hex pair! printing erroe messsage
					errorMsg.append("Error undefined escape sequence ");
					errorMsg.push_back(yytext[i + 1]);
					errorMsg.push_back(yytext[i + 2]);
					if (yytext[i + 3] != '\"') {
						errorMsg.push_back(yytext[i + 3]);
					}
					cout << errorMsg << endl;
					exit(0);
				}
				if (newVal != 0) {
					output.push_back(newVal);
					i += 3;
				}
				else{
					break;
				}
			}

	/*-----Removed in case we want to hadle in the string func-------*/
	/*-----Will make us do a for loop twice :( -------*/ 

			// else if (token[i+1] == 'n') {
			// 	output.push_back('\n');
			// 	i ++;
			// }
			// else if (token[i+1] == 'r') {
			// 	output.push_back('\r');
			// 	i ++;
			// }
			// else if (token[i+1] == 't') {
			// 	output.push_back('\t');
			// 	i ++;
			// }
			// else if (token[i+1] == '\\') {
			// 	output.push_back('\\');
			// 	i ++;
			// }
			// else if (token[i+1] == '\"') {
			// 	output.push_back('\"');
			// 	i ++;
			// }
			// else if (token[i+1] == '\0') {
			// 	output.push_back('\0');
			// 	return output;
			// }
			// else{
			// 	i ++; 
			// }
		}
		else{
			i ++; 
		}
	}
	return 0;
}


void printToken(const int token){
	if (token == UNCLOSED_STRING) {
        cout << "Error unclosed string" << endl;
        exit(0);
    }
	if (token == UNDEFINED_CHAR) {
        cout << "Error " << yytext << endl;
        exit(0);
    }
	if (token == COMMENT) {
        showCommentToken("COMMENT");
    }


}

int main()
{
	int token;
	while(token = yylex()) {
		printToken(token);
	}
	return 0;
}