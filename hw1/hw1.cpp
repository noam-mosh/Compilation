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
bool stop_printing = false;

void print_comment_token(const char * token)  //printing a comment token
{
	cout << yylineno << " " << token << " //" << endl;
}

char hex_to_ascii(const char c1, const char c2) {
    if (std::isxdigit(c1) && std::isxdigit(c2)) {
        int digit1 = std::isdigit(c1) ? c1 - '0' : std::tolower(c1) - 'a' + 10;
        int digit2 = std::isdigit(c2) ? c2 - '0' : std::tolower(c2) - 'a' + 10;
		if (digit1 > 7) return -1;
		if (digit1 < 2){
			if (digit1 == 0){
				if (std::tolower(c2) == 'a' || std::tolower(c2) == 'd' || c2 == '9'){
					return static_cast<char>(digit1 * 16 + digit2);
				}
			}
			return -1;
		}	
		if (digit1 == 7 && digit2 == 15){
			return -1;
		}
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
	if (it == escape_seq.end()){
		output.clear();
		output.append("Error undefined escape sequence ");
		output.push_back(token[i]);
		cout << output << endl;
		exit(0);
	} 
	else {
		if (*it == 'x'){
			if (i+1 >= token_len-1){ //Its /x
					output.clear();
					output.append("Error undefined escape sequence ");
					output.push_back(yytext[i + 1]);
					cout << output << endl;
					exit(0);
				}
				if (i+2 >= token_len-1){ //Its /xA
					output.clear();
					output.append("Error undefined escape sequence ");
					output.push_back(yytext[i]);
					if (yytext[i + 1] != '\"') {
						output.push_back(yytext[i + 1]);
					}
					cout << output << endl;
					exit(0);
				}
				int ascii_val = hex_to_ascii(yytext[i + 1], yytext[i + 2]);
				if (ascii_val == -1){ //Bad hex pair! printing error messsage
					output.clear();
					output.append("Error undefined escape sequence ");
					output.push_back(yytext[i]);
					output.push_back(yytext[i + 1]);
					output.push_back(yytext[i + 2]);

					cout << output << endl;
					exit(0);
				}
				if (ascii_val != 0) {
					if (stop_printing == false){
						output.push_back(ascii_val);
					}
					return i + 2;
				}
				else{
					return token_len;
				}
		}
		else{ //Replace the escape sequence
			size_t index = std::distance(escape_seq.begin(), it);
			int rep_val = escape_seq_replacment[index];
			if (rep_val != 0) {
					if (stop_printing == false){
						output.push_back(rep_val);
					}
					return i;
				}
			else {//Encountered \0, string ends here
				stop_printing = true;
				return i;
			}
		}
	}
}

void print_string_token(const char * token){
	int token_len = strlen(yytext);
	string output;
	for (int i = 0; i < token_len - 1; ++i) {
		if(yytext[i] == '\\' and i+1 < token_len) {
			int escape_res = escape_seq_handler(yytext, i + 1, output);
			if (escape_res != -1) {
				i = escape_res;
				continue;
			}
			else {
				cout << yylineno << " " << "STRING " << output << endl;
				return;
			}
		} else {	// adding regular char to output
			if (stop_printing == false){
				output.push_back(yytext[i]);
			}		
		}
	}	// printing processed string
	cout << yylineno << " " << "STRING " << output << endl;
}


void print_token(const int token){
	stop_printing = false;
	if (token == UNDEFINED_CHAR) {
        cout << "Error " << yytext << endl;
        exit(0);
    }
	else if (token == UNCLOSED_STRING) {
        cout << "Error unclosed string" << endl;
        exit(0);
    }
	else if (token == COMMENT) {
        print_comment_token("COMMENT");
    }
	else if (token == STRING) {
        print_string_token("STRING");
    }
	else{
		cout << yylineno << " " << TOKENS[token-1] << " " << yytext << endl;
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