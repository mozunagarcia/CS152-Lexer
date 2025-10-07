#include <vector>
#include <iostream>
#include <string>

struct Token {
    std::string type, value;

    unsigned long line;

    void dump() { std::cerr << type << ", on line " << line << ": " << value << "\n";}
};

struct Lexer {
    const char *it;
    unsigned long line = 1;

    std::vector<Token> operator()() {
        std::vector<Token> tokens;

    
    while(*it) {tokens.push_back(next());}

    tokens.push_back(token("eof"));

    return tokens;
    }

private: 
    Token next() {
    // Handle whitespace
    if (is_whitespace()) return whitespace();

    // Handle comments
    if (it[0] == '/' && it[1] == '/') return comment();

    // Handle operators and symbols
    for (auto digraph : { "==","!=","<<","<=",">>","=>","->",">=" }) {
        if (it[0] == digraph[0] && it[1] == digraph[1]) {
            it += 2;
            return token(digraph);
        }
    }

    for (char monograph : "~^*%():{}[]+-<>!=&|/;,") {
        if (it[0] == monograph) {
            ++it;
            return token({monograph});
        }
    }

    // Handle numbers
    if (is_digit()) {
        std::string value;
        while (is_digit()) value += *it++;
        return token("int", std::move(value));
    }

    // Handle identifiers / keywords
    if (is_letter() || *it == '_') {
        std::string value;
        while (is_letter() || is_digit() || *it == '_') value += *it++;

        for (auto &keyword : {"let", "break", "continue", "return", "loop", "if", "else"}) {
            if (value == keyword) return token(keyword);
        }

        return token("id", std::move(value));
    }

    // End of file
    if (*it == 0) return token("eof");

    // Otherwise, invalid
    std::cerr << "Lexer hit invalid character: '" << *it << "' on line " << line << "\n";
    exit(1);
}


    bool is_whitespace() {
        for(char ch : " \n\t\r\v\f") { if (*it == ch) {return true;} }

        return false;
    }

    Token whitespace () {
        while(is_whitespace()) { 
            if('\n' == *it) { line += 1; }

            ++it;
         }

        return next();
    }

    Token comment () { 
        while(it[0] && it[0] != '\n') { ++it; }

        return next();
    }

    Token token(std::string type, std::string value = "") {

        return {move(type), move(value), line};
    }


    bool is_digit() {

        return *it >= '0' && *it <= '9';
    }


    bool is_letter() {
        
        return *it >= 'a' && *it <= 'z' ||
                *it >= 'A' && *it <= 'Z' ; 
    }

};

int main() {
    Lexer lexer;
    lexer.it = R"(~ ^ * % ( ) : { } [ ] + ,
a b c
= ==
! !=
< << <=
> >> >=
- ->
d3434_43d
_d3434_43d
// comment
3434273
343
-3432
98904783
let break continue return loop if else)";
  // input string to tokenize
    auto tokens = lexer();    // this runs operator()()
    for (auto &t : tokens)
        t.dump();             // print each token
}



