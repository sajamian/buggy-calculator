/*
One place that had confused me is my understanding cin.

For starters, cin is itself an object of class istream.
Also, cin is smart. It considers the type of what it's reading in as it parses the string.
Also also, cin strips out all whitespace from it's streams.

So, in the example of our calculator, it's expecting an lval that is an int.
It pulls out whitespace and goes until it sees a character of some sort. At that point, the
switch statement tries to figure out what that character means, does whatever we've told it to,
and then returns back to the for(cin) loop to pull in the next istream.

In this calculator, something like
1 +
1
x
works because we keep spawning new streams until we are fed an 'x'. So, '1' is the first stream,
the space is stripped and the '+' is the next one, the newline terminates the cin but the loop goes
until we see an x, so the next one picks up the number one, and so on and so on.
*/


#include <iostream>
using namespace std;
//--------------------------------------------------------------------------------------------------------------------

inline void keep_window_open()
{
	cin.clear();
	cout << "Please enter a character to exit\n";
	char ch;
	cin >> ch;
	return;
}
//--------------------------------------------------------------------------------------------------------------------

class Token {
public:
    char kind;
    double value;

    Token(char ch) // Make a token from an operator (char only)
        :kind(ch), value(0) { }

    Token(char ch, double val) // Make a token from a number (char and double)
        :kind(ch), value(val) { }
};

//--------------------------------------------------------------------------------------------------------------------
Token get_token() { // Read a token from cin
    char ch;
    cin >> ch;
    switch(ch) {
        case '(': case ')': case '*': case '/': case '+': case '-':
            return Token(ch); //Return a token for an operator (because there is no double arg fed to the Token::Token() function)
        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': case '.':
            {
                cin.putback(ch); // We had told cin to expect a char, but instead looks like we should have expected a double. Put ch back in and try again.
                double val;
                cin >> val;
                return Token('8',val); // Arbitrarily, '8' was the Token.kind for numbers in our calculator.
            }
        default:
            cerr << "bad token" << endl;
    }
}

// I have to declare these here because I intend to use them in primary() if my primary is a ( expression )
double expression();
double term();

// In some ways, primary is the simplest and most complex component of our grammar. It could be as simple as a number, or as complex
// as a parenthesized expression I need to break into terms and primarys to evaluate.
double primary() {
    Token t = get_token();
    switch(t.kind) {
    case '(': // primary has to be able to handle '(' expression ')'
        {
            double d = expression(); // !!!!! Come back to this, but I think we're saying to evaluate an expression, returning a double
            t = get_token();         // !!!!! and then make the next token is a close parenthesis.
            if (t.kind != ')') cerr << "')' expected" << endl;
            return d;
        }
    case '8':
        return t.value; // If it's a number, return the value
    default:
        cerr << "primary expected" << endl;

    }
}

double expression()
{
    double left = term();
    Token t = get_token();
    while (true) { // Keep evaluating an expression for as long as our t.kind is a + or -
            switch(t.kind) {
            case '+':
                left += term();
                t = get_token();
                break;
            case '-':
                left -= term();
                t = get_token();
                break;
            default:
                return left;
            }
    }
}

double term()
{
    double left = primary();
    Token t = get_token();

    while(true) { // Evaluate a term as long as we are fed operands whose kind is * or /
        switch(t.kind) {
        case '*':
            left *= primary();
            t = get_token();
            break;
        case '/':
            {
                double d = primary();
                if(d==0) cerr << "divide by zero" << endl;
                left /= d;
                t = get_token();
                break;
            }
        default:
            return left;

        }
    }
}

int main()
try {
    while(cin)
        cout << expression() << '\n';
    keep_window_open();
}

catch (exception& e) {
    cerr << e.what() << endl;
    keep_window_open();
    return 1;
}
catch (...) {
    cerr << "exception \n";
    keep_window_open();
    return 2;
}
