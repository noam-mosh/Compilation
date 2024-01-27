#include "tokens.hpp"
#include <stdio.h>
#include <string>
#include <cstring>
#include <iostream>
#include <cctype>
#include <vector>
#include <algorithm>

using std::vector;
using std::size_t;
using std::string;
using std::strlen;
using std::cout;
using std::endl;

extern int yylex();

void print_comment_token(const char * token)  //printing a comment token
{
	cout << yylineno << " " << token << " //" << endl;
}

void error_manager(char* token){
	int token_len = strlen(token);
	string errorMsg = "";
	string output;
	for (int i = 0; i < token_len - 1; ++i) {
		if(token[i] == '\\') {
            if (token[i+1] == 'x') {
				if (i+1 >= token_len-1){ //Its /x
					errorMsg.append("Error undefined escape sequence ");
					errorMsg.push_back(yytext[i + 1]);
					cout << errorMsg << endl;
					exit(0);
				}
				if (i+2 >= token_len-1){ //Its /xA
					errorMsg.append("Error undefined escape sequence ");
					errorMsg.push_back(yytext[i + 1]);
					if (yytext[i + 2] != '\"') {
						errorMsg.push_back(yytext[i + 2]);
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
		}
		else{
			i ++; 
		}
	}
	return 0;
}

char hex_to_ascii(const char c1, const char c2) {
    if (std::isxdigit(c1) && std::isxdigit(c2)) {
        int digit1 = std::isdigit(c1) ? c1 - '0' : std::tolower(c1) - 'a' + 10;
        int digit2 = std::isdigit(c2) ? c2 - '0' : std::tolower(c2) - 'a' + 10;
        return static_cast<char>(digit1 * 16 + digit2);
    }
    return -1;
}

int escape_seq_handler(const char* token, int i, string& output){
	int token_len = strlen(token);
	vector<char> escape_seq = {'n', 'r', 't', '\\', '\"', '0', 'x'};
	vector<char> escape_seq_replacment = {'\n', '\r', '\t', '\\', '\"', '\0'};
	auto it = find(escape_seq.begin(), escape_seq.end(), token[i]);

	// Check if the character was found
	if (it == escape_seq.end()) return -1;
	else {
		if (*it == 'x'){
			if (i+1 >= token_len-1){ //Its /x
					output.append("Error undefined escape sequence ");
					output.push_back(yytext[i + 1]);
					cout << output << endl;
					exit(0);
				}
				if (i+2 >= token_len-1){ //Its /xA
					output.append("Error undefined escape sequence ");
					output.push_back(yytext[i + 1]);
					if (yytext[i + 2] != '\"') {
						output.push_back(yytext[i + 2]);
					}
					cout << output << endl;
					exit(0);
				}
				int ascii_val = hex_to_ascii(yytext[i + 2], yytext[i + 3]);
				if (ascii_val == -1){ //Bad hex pair! printing erroe messsage
					output.append("Error undefined escape sequence ");
					output.push_back(yytext[i + 1]);
					output.push_back(yytext[i + 2]);
					if (yytext[i + 3] != '\"') {
						output.push_back(yytext[i + 3]);
					}
					cout << output << endl;
					exit(0);
				}
				if (ascii_val != 0) {
					output.push_back(ascii_val);
					return i + 3;
				}
				else{
					return token_len;
				}
		}
		else{ //Replace the escape sequence
			size_t index = std::distance(escape_seq.begin(), it);
			int rep_val = escape_seq_replacment[index];
			if (rep_val != 0) {
					output.push_back(rep_val);
					return i + 1;
				}
			else {//Encountered \0, string ends here
				return token_len;
			}
		}
	}
}

void print_string_token(const char * token){
	int token_len = strlen(token);
	string output;
	for (int i = 0; i < token_len - 1; ++i) {
		if(token[i] == '\\' and i+1 < token_len) {
			int escape_res = escape_seq_handler(token, i + 1, output);
			if (escape_res != -1) {
				i = escape_res;
				break;
			}
			else {
				cout << yylineno << " " << "STRING " << output << endl;
			}
		}
	}
}

void print_token(const int token){
	if (token == UNDEFINED_CHAR) {
        cout << "Error " << yytext << endl;
        exit(0);
    }
	if (token == UNDEFINED_ESCAPE_SEQUENCE) {
        cout << "Error undefined escape sequence " << yytext << endl;
        exit(0);
    }
	if (token == UNCLOSED_STRING) {
        cout << "Error unclosed string" << endl;
        exit(0);
    }
	if (token == COMMENT) {
        print_comment_token("COMMENT");
    }
	if (token == STRING) {
        print_string_token("STRING");
    }
}

int main()
{
	int token;
	while(token = yylex()) {
		print_token(token);
	}
	return 0;
}