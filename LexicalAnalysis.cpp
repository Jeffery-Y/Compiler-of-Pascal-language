#include "Table.h"

vector<string>KeyWords = { "array", "begin", "char", "const", "do", "else", "end", "function", "integer", "if", "of", "program", "real", "struct", "then", "var", "while", "type", "record", "procedure" };
vector<Token>TokenList;//保存词法分析所得的token串
FileStream fileStream;
Token token;//当前Token
int line = 1;//行号


int Search_KeyWords(string str)
{
	for (int i = 0; i < KeyWords.size(); i++)
	{
		if (str == KeyWords[i])
			return i;
	}
	return -1;
}

void CopyFile()
{
	char c;
	string filename;//文件
	filename = "test.txt";
	//cout << "please input the document location:";
	//cin >> filename;//输入要打开的文件位置

	ifstream myfile(filename);//打开文件
	if (!myfile.is_open())
	{
		cout << "Read file code.txt failed!" << endl;
		exit(0);
	}

	while (!myfile.eof())																			//将文件内容写入字符串
	{
		myfile.get(c);
		fileStream.text += c;
	}
	fileStream.length = fileStream.text.size() - 1;							//文件结尾重复读取了结尾字符，所以长度需要减一并删除重复的字符
	fileStream.text.erase(fileStream.length, 1);								//删除文件结尾重复读取的字符
	myfile.close();
}

int state_change(int state, char ch)
{//自动机
	switch (state)
	{
	case 1:
		if ('\n' == ch){line++; return 1;}
		else if (' ' == ch || '\t' == ch)return 1;
		else if (ch == 't')return 33;
		else if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))return 2;
		else if (ch >= '0' && ch <= '9')return 3;
		else if (ch == '\'')return 5;
		else if (ch == '"')return 7;
		else if (ch == '+')return 8;
		else if (ch == '-')return 9;
		else if (ch == '*')return 10;
		else if (ch == '/')return 11;
		else if (ch == ':')return 13;
		else if (ch == '>')return 15;
		else if (ch == '<')return 17;
		else if (ch == '.')return 19;
		else if (ch == '=')return 20;
		else if (ch == '[')return 21;
		else if (ch == ']')return 22;
		else if (ch == ';')return 23;
		else if (ch == ',')return 24;
		else if (ch == '{')return 25;
		else if (ch == '}')return 26;
		else if (ch == '(')return 27;
		else if (ch == ')')return 28;
		else { cout << "词法错误！无法识别的字符！行号为：" << line << endl; exit(0); }//无法识别的字符
	case 2:
		if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '_')return 2;
		else return 0;//标识符或关键字
	case 3:
		if (ch >= '0' && ch <= '9')return 3;
		else if (ch == '.')return 4;
		else return 0;//整数
	case 4:
		if (ch >= '0' && ch <= '9')return 32;
		else if (ch == '.')return 30;
		else { cout << "词法错误！行号为：" << line << endl; exit(0); }
	case 5:
		if (ch == '\'')return 12;//字符
		else return 6;
	case 6:
		if (ch == '\'')return 12;//字符
		else { cout << "词法错误！缺少\" ' \"！行号为：" << line << endl; exit(1); }
	case 7:
		if (ch == '"')return 29;//字符串
		else return 7;
	case 8:
	case 9:
	case 10:
	case 12:
	case 14:
	case 16:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
	case 39:return 0;
	case 13:
	case 15:
	case 17:
		if (ch == '=')return state + 1;
		else return 0;
	case 32:
		if (ch >= '0' && ch <= '9')return 32;
		else return 0;//小数
	case 30:
		if (ch >= '0' && ch <= '9')return 31;
		else { cout << "词法错误！无法识别的字符！行号为：" << line << endl; exit(0); }
	case 31:
		if (ch >= '0' && ch <= '9')return 31;
		else return 0;//数组定义时的范围，例如：1..5
	case 33:
		if (ch >= '0' && ch <= '9'){ cout << "该标识符可能与中间变量冲突！请更换变量名！谢谢！行号为：" << line << endl; exit(0); }
		else if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '_')return 2;
		else return 0;//标识符或关键字
	case 11:
		if (ch == '/')return 100;
	case 100:return 0;
	default:{cout << "状态错误！" << endl; exit(2); }
	}
}

int state_to_type(int state_before, string str)
{//token的类别码识别
	switch (state_before)
	{
	case 2:
	case 33:
		if (Search_KeyWords(str) != -1)return 1;
		else return 2;
	case 3:
	case 8:
	case 9:
	case 10:
	case 11:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:return state_before; 
	case 12:return 6;//字符
	case 29:return 7;//字符串
	case 31:return 5;//数组范围
	case 32:return 4;//real型
	case 100:return 100;
	default:{cout << "状态错误！" << endl; exit(3); }
	}
}

void NextToken()
{
	token.name.clear();
	token.type = 0;
	int state = 1, state_before;//当前状态和上一个状态
	int file_end = 0;
	char c;//当前字符c

	while (fileStream.offset < fileStream.length)
	{
		c = fileStream.text[fileStream.offset++];
		state_before = state;
		state = state_change(state, c);	//状态改变
		if (state == 100)
		{
			while (fileStream.offset < fileStream.length && c != '\n')
			{
				c = fileStream.text[fileStream.offset++];
			}
			token.name.clear();
			line++;
			token.type = 0;
			state = 1;
		}
		else if (state > 1)
		{
			token.name += c;	//保存token
		}
		else if (state == 0)
		{
			token.type = state_to_type(state_before, token.name);//识别token的类别码
			fileStream.offset--;
			TokenList.push_back(token);//压入TokenList栈中保存
			fileStream.endFlag = 1;//若该token之后文件中均只剩空格回车等无用字符则判定文件读取结束
			for (int i = fileStream.offset; i < fileStream.length; i++)
			{
				if (fileStream.text[i] > 32)
				{
					fileStream.endFlag = 0;
					break;
				}
			}
			return;
		}
	}

	if (fileStream.offset == fileStream.length){ token.type = state_to_type(state, token.name); fileStream.endFlag = 1; }//最后一个token识别
	TokenList.push_back(token);//压入TokenList栈中保存
	return;
}
