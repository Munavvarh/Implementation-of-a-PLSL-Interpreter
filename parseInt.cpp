/* Implementation of PLSL Interpreter
 * parseInt.cpp
 * Programming Assignment 3
 * Spring 2022
*/

#include <vector>
#include <string>
#include "parseInt.h"
#include "val.h"
//#include "lex.cpp"
using namespace std;
map<string, bool> defVar;
map<string, Token> SymTable;
map<string, Value> TempsResults; //Container of temporary locations of Value objects for results of expressions, variables values and constants 
queue <Value> * ValQue; //declare a pointer variable to a queue of Value objects




namespace Parser {
	bool pushed_back = false;
	LexItem	pushed_token;

	static LexItem GetNextToken(istream& in, int& line) {
		if( pushed_back ) {
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem & t) {
		if( pushed_back ) {
			abort();
		}
		pushed_back = true;
		pushed_token = t;	
	}

}

static int error_count = 0;

int ErrCount()
{
    return error_count;
}

void ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}



bool Prog(istream& in, int& line)
{
	bool f1, f2;
	LexItem tok = Parser::GetNextToken(in, line);
		
	if (tok.GetToken() == PROGRAM) {
		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == IDENT) {
			
			tok = Parser::GetNextToken(in, line);
			if (tok.GetToken() == SEMICOL) {
				f1 = DeclBlock(in, line); 
			
				if(f1) {
					f2 = ProgBody(in, line);
					if(!f2)
					{
						ParseError(line, "Incorrect Program Body.");
						return false;
					}
					
					return true;//Successful Parsing is completed
				}
				else
				{
					ParseError(line, "Incorrect Declaration Section.");
					return false;
				}
			}
			else
			{
				//Parser::PushBackToken(tok);
				ParseError(line-1, "Missing Semicolon in prog.");
				return false;
			}
		}
		else
		{
			ParseError(line, "Missing Program Name.");
			return false;
		}
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else if(tok.GetToken() == DONE && tok.GetLinenum() <= 1){
		ParseError(line, "Empty File");
		return true;
	}
	ParseError(line, "Missing PROGRAM.");
	return false;
}






bool DeclBlock(istream& in, int& line) {
	bool st = false;
	LexItem nextTok;
	
	LexItem tokVal = Parser::GetNextToken(in, line);
	if(tokVal == VAR)
	{
		st = DeclStmt(in, line);
		
		
		
		nextTok = Parser::GetNextToken(in, line);
		if(nextTok == BEGIN )
		{
			Parser::PushBackToken(nextTok);
			return true;
		}
		else 
		{
			ParseError(line, "Syntactic error in Declaration Block.");
			return false;
		}
	}
	else
	{
		ParseError(line, "Non-recognizable Declaration Block.");
		return false;
	}
    
    return st;
	
}





















bool DeclStmt(istream& in, int& line){
	LexItem t = Parser::GetNextToken(in, line);
	bool stVal = true;
	string idVal;
	
	if(t == IDENT){
		idVal = t.GetLexeme();
		if(!(defVar.find(idVal)->second)){
			defVar[idVal] = true;
		}
		else{
			ParseError(line, "Variable Redefination");
			ParseError(line, "Incorrect variable in Declaration Statement.");
			ParseError(line, "Syntactic error in Declaration Block.");
			return false;
		}
	}
	
	t = Parser::GetNextToken(in, line);
	
	if(t == COMMA){
		stVal = DeclStmt(in, line);
		if(!stVal){
			return false;
		}
	}
	else if(t == COLON){
		t = Parser::GetNextToken(in, line);
		if(t == REAL || t == INTEGER || t == STRING){
			for(auto itr = defVar.begin(); itr != defVar.end(); ++itr){
				auto i = SymTable.find(itr->first);
				if(i == SymTable.end()){
					SymTable[itr->first] = t.GetToken();
				}
			}	
		}
		else{
			ParseError(line, "Incorrect Declaration Type.");
			ParseError(line, "Syntactic error in Declaration Block.");
			return false;
		}
		
		t = Parser :: GetNextToken(in, line);
	
		if(t != SEMICOL){
			ParseError(line, "Missing semicolon");
			ParseError(line, "Syntactic error in Declaration Block.");
			return false;
		}
		
	}
	else {
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << t.GetLexeme() << ")" << endl;
		return false;
	}
	
	
	t = Parser :: GetNextToken(in, line);

	if (t == IDENT){
		Parser::PushBackToken(t);
		stVal = DeclStmt(in, line);
		if(!stVal){
			return false;
		}
		return true;
	}	
	Parser::PushBackToken(t);
	return true;
		
}












bool IdentList(istream& in, int& line, LexItem& type) {
	bool stVal = false;
	string myVal;
	
	LexItem tokVal = Parser::GetNextToken(in, line);
	if(tokVal == IDENT)
	{
		//set IDENT lexeme to the type tok value
		myVal = tokVal.GetLexeme();
		if (!(defVar.find(myVal)->second))
		{
			defVar[myVal] = true;
			SymTable[myVal] = type.GetToken();
		}	
		else
		{
			ParseError(line, "Variable Redefinition");
			return false;
		}
		
	}
	else
	{
		ParseError(line, "Missing Variable");
		return false;
	}
	
	tokVal = Parser::GetNextToken(in, line);
	
	if (tokVal == COMMA) {
		stVal = IdentList(in, line, type);
	}
	else if(tokVal.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tokVal.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tokVal);
		return true;
	}
	return stVal;
}







//ProgBody ::= BEGIN {Stmt;} END
bool ProgBody(istream& in, int& line) {
    bool stVal;

    LexItem tokVal = Parser::GetNextToken(in, line);

    if (tokVal.GetToken() == BEGIN) {

        stVal = Stmt(in, line);

        while(stVal) {
            tokVal = Parser::GetNextToken(in, line);
            if(tokVal == WRITELN){
                Parser::PushBackToken(tokVal);
                stVal = Stmt(in, line);
                if(!stVal){
                    return false;
                }
                return true;
            }
            if(tokVal != SEMICOL)
            {
                line--;
                ParseError(line, "Missing semicolon in Statement.");
                return false;
            }
            
            

            stVal = Stmt(in, line);
        }

        tokVal = Parser::GetNextToken(in, line);
        if(tokVal == END )
        {
            return true;
        }
        else
        {
            ParseError(line, "Syntactic error in Program Body.");
            return false;
        }
    }
    else
    {
        ParseError(line, "Non-recognizable Program Body.");
        return false;
    }
}//End of ProgBody function



//Stmt is either a WriteLnStmt, ForepeatStmt, IfStmt, or AssigStmt
//Stmt = AssigStmt | IfStmt | WriteStmt | ForStmt 
bool Stmt(istream& in, int& line) {
	bool stVal;
	
	LexItem tokVal = Parser::GetNextToken(in, line);
	
	switch( tokVal.GetToken() ) {

	case WRITELN:
		stVal = WriteLnStmt(in, line);
		
		break;

	case IF:
		stVal = IfStmt(in, line);
		break;

	case IDENT:
		Parser::PushBackToken(tokVal);
        stVal = AssignStmt(in, line);
		
		break;
			
	default:
		Parser::PushBackToken(tokVal);
		return false;
	}

	return stVal;
}//End of Stmt









//WriteStmt:= wi, ExpreList 
bool WriteLnStmt(istream& in, int& line) {
	LexItem t;
	//cout << "in WriteStmt" << endl;
	ValQue = new queue<Value>;
	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {
		
		ParseError(line, "Missing Left Parenthesis");
		return false;
	}
	
	bool ex = ExprList(in, line);
	
	if( !ex ) {
		ParseError(line, "Missing expression after WriteLn");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis");
		return false;
	}
	
	
	//Evaluate: print out the list of expressions' values
	while (!(*ValQue).empty())
	{
		Value nextVal = (*ValQue).front();
		cout << nextVal;
		ValQue->pop();
	}
	cout << endl;

	return ex;
}//End of WriteLnStmt




















//Expr:= Term {(+|-) Term}
bool Expr(istream& in, int& line, Value & retVal) {
	Value val1, val2;
	//cout << "in Expr" << endl;
	bool t1 = Term(in, line, val1);
	LexItem tok;
	
	if( !t1 ) {
		return false;
	}
	retVal = val1;
	
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	//Evaluate: evaluate the expression for addition or subtraction
	while ( tok == PLUS || tok == MINUS ) 
	{
		t1 = Term(in, line, val2);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
		
		if(tok == PLUS)
		{
			retVal = retVal + val2;
			if(retVal.IsErr())
			{
				ParseError(line, "Illegal addition operation.");		
				return false;
			}
		}
		else if(tok == MINUS)
		{
			retVal = retVal - val2;
			if(retVal.IsErr())
			{
				ParseError(line, "Illegal subtraction operation.");
				return false;
			}
		}
			
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
		
		
	}
	Parser::PushBackToken(tok);
	return true;
}

bool ExprList(istream& in, int& line) {
	bool stVal = false;

    
    Value value;
	stVal = Expr(in, line, value);
	if(!stVal){
		ParseError(line, "Missing Expression");
		return false;
	}
	ValQue->push(value);

	LexItem tok = Parser::GetNextToken(in, line);

	if (tok == COMMA) {
		stVal = ExprList(in, line);
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		return true;
	}
	return stVal;
}




bool AssignStmt(istream& in, int& line) {

    bool varSt = false, st = false;
    LexItem t;

    LexItem val;
    varSt = Var(in, line, val);

    if (varSt) {
        t = Parser::GetNextToken(in, line);

        if (t == ASSOP) {
            Value secval;
            st = Expr(in, line, secval);
            if (!st) {
                ParseError(line, "Missing Expression in Assignment Statment");
                return st;
            } else {
               
             
                if (SymTable.find(val.GetLexeme())->second == REAL) {
                    if (secval.IsReal()) {
                        TempsResults.insert({val.GetLexeme(), secval});
                    } 
                    else if (secval.IsInt()) {
                        float value = secval.GetInt();
                        TempsResults.insert({val.GetLexeme(), Value(value)});
                    } 
                    else {
                        ParseError(line, "Real cannot be assigned this type");
                        return false;
                    }
                } 
                else if (SymTable.find(val.GetLexeme())->second == INTEGER) {
                   
                    if (secval.IsReal()) {
                        int value = secval.GetReal();
                        TempsResults.insert({val.GetLexeme(), Value(value)});
                    }
                    else if (secval.IsInt()) {
                        TempsResults.insert({val.GetLexeme(), secval});
                    } 
                    else {
                        ParseError(line, "Int cannot be assigned this type");
                        return false;
                    }
                } 
                else {
                    TempsResults[val.GetLexeme()] = secval;
                }
            }
        } else if (t.GetToken() == ERR) {
            ParseError(line, "Unrecognized Input Pattern");
            cout << "(" << t.GetLexeme() << ")" << endl;
            return false;
        } else {
            ParseError(line, "Missing Assignment Operator");
            return false;
        }
    } else {
        ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
        return false;
    }
    return st;
} 













bool Var(istream& in, int& line, LexItem & tok)
{
	
	string identstr;

	tok = Parser::GetNextToken(in, line);

	if (tok == IDENT){
		identstr = tok.GetLexeme();
		if (!(defVar.find(identstr)->second))
		{
			ParseError(line, "Undeclared Variable");
			return false;

		}
		return true;
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	return false;
}













bool Term(istream& in, int& line, Value& retVal) {
    Value data1, data2;
    bool t1 = SFactor(in, line, data1);
    LexItem tok;


    if (!t1) {
        return false;
    }

    tok = Parser::GetNextToken(in, line);
    if (tok.GetToken() == ERR) {
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
    }
    while (tok == MULT || tok == DIV )
    {
        t1 = SFactor(in, line, data2);

        //cout << "status of factor2: " << t1<< endl;
        if (!t1) {
            ParseError(line, "Missing operand after operator");
            return false;
        }

        if (tok == MULT) {
            if ((data1.IsInt() || data1.IsReal()) && (data2.IsInt() || data2.IsReal())) {
                data1 = data1 * data2;
            }
            else {
                ParseError(line, "Illegal multiplication operand.");
                return false;
            }
        }

        if (tok == DIV) {
            if(data2.GetInt()==0){
                return false;
            }
            if ((data1.IsInt() || data1.IsReal()) && (data2.IsInt() || data2.IsReal())) {
                data1 = data1 / data2;
            }
            else {
                ParseError(line, "Illegal division operand.");
                return false;
            }
        }


        tok = Parser::GetNextToken(in, line);
        if (tok.GetToken() == ERR) {
            ParseError(line, "Unrecognized Input Pattern");
            cout << "(" << tok.GetLexeme() << ")" << endl;
            return false;
        }

    }
    retVal = data1;
    Parser::PushBackToken(tok);
    return true;
}



bool Factor(istream& in, int& line, int sign, Value & retVal) {
    LexItem tokVal = Parser::GetNextToken(in, line);

    if( tokVal == IDENT ) {
        string lexemeVal = tokVal.GetLexeme();
        if (!(defVar.find(lexemeVal)->second)) {
            ParseError(line, "Using Undefined Variable");
            return false;
        } 
        else if ((TempsResults.find(lexemeVal)->first) != lexemeVal) {
            ParseError(line, "Undefined Variable");
            return false;
        }
        retVal = TempsResults.find(lexemeVal)->second;
        return true;
    }
  
    else if( tokVal == SCONST ) {
        if (sign == 1) {
            ParseError(line, "Illegal Operand Type for Sign Operator");
            return false;
        }
        retVal = Value(tokVal.GetLexeme());
        return true;
    }
    else if( tokVal == ICONST ) {
        int value = stoi(tokVal.GetLexeme());
        retVal = Value(value);
        return true;
    }
 
    else if( tokVal == LPAREN ) {
        Value temp = Value();
        bool ex = Expr(in, line, temp);
        if (!ex) {
            ParseError(line, "Missing expression after (");
            return false;
        }
        if (Parser::GetNextToken(in, line) == RPAREN) {
            retVal = temp;
            return ex;
        } else {
            Parser::PushBackToken(tokVal);
            ParseError(line, "Missing ) after expression");
            return false;
        }
    }
    else if( tokVal == RCONST ) {
        float value = stof(tokVal.GetLexeme());
        retVal = Value(value);
        return true;
    }
    else if(tokVal.GetToken() == ERR){
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tokVal.GetLexeme() << ")" << endl;
        return false;
    }

    return false;
}
















bool SFactor(istream& in, int& line, Value & retVal)
{
	LexItem tokVal = Parser::GetNextToken(in, line);
	bool status;
	int sign = 0;
	
	if(tokVal == PLUS)
	{
		sign = 1;
	}
    else if(tokVal == MINUS )
	{
		sign = -1;
	}
	else
		Parser::PushBackToken(tokVal);

	status = Factor(in, line, sign, retVal);
	return status;
}






bool IfStmt(istream& in, int& line) {
    bool exVal=false, status;
    LexItem t;

    t = Parser::GetNextToken(in, line);
    if( t != LPAREN ) {

        ParseError(line, "Missing Left Parenthesis");
        return false;
    }

    Value val;
    exVal = LogicExpr(in, line, val);

    if( !exVal) {
        ParseError(line, "Missing if statement Logic Expression");
        return false;
    }

    t = Parser::GetNextToken(in, line);
    if(t != RPAREN ) {

        ParseError(line, "Missing Right Parenthesis");
        return false;
    }

    t = Parser::GetNextToken(in, line);
    if(t != THEN)
    {
        ParseError(line, "If-Stmt Syntax Error");
        return false;
    }

    if (val.GetBool() == true) {
        status = Stmt(in, line);
        if(!status)
        {
            ParseError(line, "Missing Statement for If-Stmt Then-Part");
            return false;
        }
        t = Parser::GetNextToken(in, line);
    } else {
        while (t!= ELSE && t!= WRITELN) {
            t = Parser::GetNextToken(in,line);
        }
    }

    if( t == ELSE ) {
        status = Stmt(in, line);
        if(!status)
        {
            ParseError(line, "Missing Statement for If-Stmt Else-Part");
            return false;
        }
        return true;
    }

    Parser::PushBackToken(t);
    return true;
}









bool LogicExpr(istream & in , int & line, Value & retVal) {
    Value data1, data2;
    bool t1 = Expr( in , line, data1);
    LexItem tok;

    if (!t1) {
        return false;
    }
    retVal = data1;

    tok = Parser::GetNextToken( in , line);
    if (tok.GetToken() == ERR) {
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
    }
    if (tok == LTHAN || tok == EQUAL || tok == GTHAN) {
        t1 = Expr( in , line, data2);
        if (!t1) {
            ParseError(line, "Missing expression after relational operator");
            return false;
        }
        if (data1.GetType() == VSTRING || data2.GetType() == VSTRING) {
            ParseError(line, "Run-Time Error-Illegal Mixed Type Operands");
            return false;
        }
        if (tok == EQUAL) {
            retVal = (data1 == data2);
        } 
        else if (tok == GTHAN)
        {
            retVal = (data1 > data2);
        }
        else if (tok == LTHAN) {
            retVal = (data1 < data2);
        }

        return true;
    }
    Parser::PushBackToken(tok);
    return true;
}