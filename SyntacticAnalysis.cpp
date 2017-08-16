#include "Table.h"

extern vector<Synbl_item> SYNBL;//符号表
extern vector<Typel_item> TYPEL;//类型表
extern vector<Ainel_item> AINEL;//数组表
extern vector<Rinfl_item> RINFL;//结构表
extern vector<Pfinfl_item> PFINEL;//函数表
extern vector<Synbl_item> PARAMBL;//参数表
extern vector<Consl_item> CONSL;//常量表
extern vector<int> LENL;//长度表

extern vector<double>REAL;//实数表
extern vector<long>INTEGER;//整数表
extern vector<bool>BOOL;//布尔表
extern vector<char>CHAR;//字符表

extern vector<Token>TokenList;//保存词法分析所得的token串
extern Token token;//当前token
extern FileStream fileStream;
extern int line;//当前行号

vector<Synbl_item> Name_temp;//临时符号表项的vector
vector<Typel_item> TYPEL_temp;//临时类型表
Synbl_item syn_temp;//临时符号表项
stack<Token> SEM;//语义栈
vector<QT_item> QT;//四元式存储区
vector<CONST_QT_item> CONST_QT;//常量定义的四元式暂存
vector<QT_item> PROCE_QT;//过程函数的四元式暂存
string ID_name;//暂存函数名
int function_vn_num = 0;//函数定义时的参数个数
int Vn_Num = 0;//函数调用时的参数个数
int rinfl_off = 0;//结构表的offset
extern int Level;//当前层次号
string Tnum = "t";
int num_T = 1;

int IT_Type(Token t)
{//返回标识符的类型码
	if (t.type == 2)
	{
		//判断当前token是否为结构体或数组未填
		if (t.name.find_first_of('[') != string::npos && t.name.find_first_of('.') != string::npos)
		{
			if (t.name.find_last_of('[') < t.name.find_last_of('.'))
			{
				t.name.erase(0, t.name.find_last_of('.') + 1);//结构体
				int i;
				for (i = 0; i < RINFL.size(); i++)
				{
					if (RINFL.at(i).id == t.name)break;
				}
				if (i < RINFL.size())
				{
					switch (TYPEL.at(RINFL.at(i).tp).tval)
					{
					case 'i':return 3;
					case 'r':return 4;
					case 'c':return 6;
					case 'b':return 29;//布尔型
					case 'a':return 8;//数组型
					case 'd':return 9;//结构型
					default:{cout << "未知的标识符类型码！行号为：" << line << endl; exit(0); }
					}
				}
				else { cout << "未知的标识符类型码！行号为：" << line << endl; exit(0); }
			}
			else
			{
				string array_name = t.name;
				array_name.erase(t.name.find_first_of('['), t.name.size() - t.name.find_first_of('['));//数组名
				array_name.erase(0, t.name.find_last_of('.') + 1);//结构体
				t.name.erase(0, t.name.find_last_of('.') + 1);//结构体
				Ainel_item array_typel = AINEL.at(TYPEL.at(SYNBL.at(search_SYNBL(array_name)).type).tpoint);

				//t.name.erase(t.name.find_first_of('['), t.name.size() - t.name.find_first_of('['));//数组
				while (t.name.find_first_of(',') != string::npos)
				{
					array_typel = AINEL.at(TYPEL.at(array_typel.ctp).tpoint);
					t.name.erase(0, t.name.find_first_of(',') + 1);
				}
				switch (TYPEL.at(array_typel.ctp).tval)
				{
				case 'i':return 3;
				case 'r':return 4;
				case 'c':return 6;
				case 'b':return 29;//布尔型
				case 'a':return 8;//数组型
				case 'd':return 9;//结构型
				default:{cout << "未知的标识符类型码！行号为：" << line << endl; exit(0); }
				}
			}
		}
		if (t.name.find_first_of('[') != string::npos)
		{
			string array_name = t.name;
			array_name.erase(t.name.find_first_of('['), t.name.size() - t.name.find_first_of('['));//数组名
			Ainel_item array_typel = AINEL.at(TYPEL.at(SYNBL.at(search_SYNBL(array_name)).type).tpoint);

			//t.name.erase(t.name.find_first_of('['), t.name.size() - t.name.find_first_of('['));//数组
			while (t.name.find_first_of(',') != string::npos)
			{
				array_typel = AINEL.at(TYPEL.at(array_typel.ctp).tpoint);
				t.name.erase(0, t.name.find_first_of(',') + 1);
			}
			switch (TYPEL.at(array_typel.ctp).tval)
			{
			case 'i':return 3;
			case 'r':return 4;
			case 'c':return 6;
			case 'b':return 29;//布尔型
			case 'a':return 8;//数组型
			case 'd':return 9;//结构型
			default:{cout << "未知的标识符类型码！行号为：" << line << endl; exit(0); }
			}

		}
		else if (t.name.find_last_of('.') != string::npos)
		{
			t.name.erase(0, t.name.find_last_of('.') + 1);//结构体
			int i;
			for (i = 0; i < RINFL.size(); i++)
			{
				if (RINFL.at(i).id == t.name)break;
			}
			if (i < RINFL.size())
			{
				switch (TYPEL.at(RINFL.at(i).tp).tval)
				{
				case 'i':return 3;
				case 'r':return 4;
				case 'c':return 6;
				case 'b':return 29;//布尔型
				case 'a':return 8;//数组型
				case 'd':return 9;//结构型
				default:{cout << "未知的标识符类型码！行号为：" << line << endl; exit(0); }
				}
			}
			else { cout << "未知的标识符类型码！行号为：" << line << endl; exit(0); }
		}
		else if (SYNBL.at(search_SYNBL(t.name)).cat == 'f')
		{
			if (SYNBL.at(search_SYNBL(t.name)).type == -1){ cout << "函数无返回值！行号为：" << line << endl; exit(0); }
		}
		//else 一般数据类型或者函数
		switch (TYPEL.at(SYNBL.at(search_SYNBL(t.name)).type).tval)
		{
		case 'i':return 3;
		case 'r':return 4;
		case 'c':return 6;
		case 'b':return 29;//布尔型
		case 'a':return 8;//数组型
		case 'd':return 9;//结构型
		default:{cout << "未知的标识符类型码！行号为：" << line << endl; exit(0);}
		}
	}
	else return t.type;
}

//将临时变量填入符号表
void Write_T_SYN(Token t1, Token t2)
{//将临时表了
	if (t1.type == 2)
	{
		SYNBL.push_back(SYNBL.at(search_SYNBL(t1.name)));
		SYNBL.back().name = Tnum;
		switch (IT_Type(t1))
		{
		case 3:SYNBL.back().addr = INTEGER.size(); INTEGER.push_back(0); break;
		case 4:SYNBL.back().addr = REAL.size(); REAL.push_back(0.0); break;
		case 6:SYNBL.back().addr = CHAR.size(); CHAR.push_back('\0'); break;
		}
	}
	else if (t2.type == 2)
	{
		SYNBL.push_back(SYNBL.at(search_SYNBL(t2.name)));
		SYNBL.back().name = Tnum;
		switch (IT_Type(t1))
		{
		case 3:SYNBL.back().addr = INTEGER.size(); INTEGER.push_back(0); break;
		case 4:SYNBL.back().addr = REAL.size(); REAL.push_back(0.0); break;
		case 6:SYNBL.back().addr = CHAR.size(); CHAR.push_back('\0'); break;
		}

	}
	else if (t1.type == 3)
	{
		SYNBL.push_back({ Tnum, 0, 'v', INTEGER.size(), Level });
		INTEGER.push_back(0);//初始值为0
	}
	else if (t1.type == 4)
	{
		SYNBL.push_back({ Tnum, 1, 'v', REAL.size(), Level });
		REAL.push_back(0.0);//初始值为0
	}
	else if (t1.type == 6)
	{
		SYNBL.push_back({ Tnum, 2, 'v', CHAR.size(), Level });
		CHAR.push_back('\0');//初始值为0
	}
}

//结构体数组函数因子
void ID_SID_OPTION()
{
	if (SYNBL.at(search_SYNBL(token.name)).cat == 'v' || SYNBL.at(search_SYNBL(token.name)).cat == 'f' || SYNBL.at(search_SYNBL(token.name)).cat == 'n' || SYNBL.at(search_SYNBL(token.name)).cat == 'e' || SYNBL.at(search_SYNBL(token.name)).cat == 'c')//|| SYNBL.at(search_SYNBL(token.name)).cat == 'n' || SYNBL.at(search_SYNBL(token.name)).cat == 'f'
	{
		SEM.push({ 2, token.name });
		NextToken();

		if (token.name == ".")//若不为ID，记得删掉SEM.top
		{//结构体
			NextToken();
			if (token.type != 2){ cout << "结构体变量“.”后缺标识符！行号为：" << line << endl; exit(0); }

			if (TYPEL.at(SYNBL.at(search_SYNBL(SEM.top().name)).type).tval != 'd'){ cout << "该变量不是结构体！行号为：" << line << endl; exit(0); }
			if (search_RINFL(SEM.top().name, token.name) == -1){ cout << "该结构体中没有该项！行号为：" << line << endl; exit(0); }

			SEM.top().name += '.';
			SEM.top().name += token.name;
			Token Struct_Node = token;//暂存当前标识符用来判断是否为结构体类型
			NextToken();
			while (token.name == ".")
			{
				NextToken();
				if (token.type != 2){ cout << "结构体变量“.”后缺标识符！行号为：" << line << endl; exit(0); }

				if (TYPEL.at(SYNBL.at(search_SYNBL(Struct_Node.name)).type).tval != 'd'){ cout << "该变量不是结构体！行号为：" << line << endl; exit(0); }
				if (search_RINFL(Struct_Node.name, token.name) == -1){ cout << "该结构体中没有该项！行号为：" << line << endl; exit(0); }

				SEM.top().name += '.';
				SEM.top().name += token.name;
				Struct_Node = token;
			}
			if (token.name == "[")
			{
				if (TYPEL.at(SYNBL.at(search_SYNBL(Struct_Node.name)).type).tval != 'a'){ cout << "该变量不是数组！行号为：" << line << endl; exit(0); }

				Ainel_item array_typel = AINEL.at(TYPEL.at(SYNBL.at(search_SYNBL(Struct_Node.name)).type).tpoint);
				string array_num = token.name;//暂存数组的下标
				//SEM.pop();
				NextToken();
				if (IT_Type(token) != 3){ cout << "语法错误！数组使用有误！行号为：" << line << endl; exit(0); }

				if (token.type == 3 && (stoi(token.name) > array_typel.up || stoi(token.name) < array_typel.low))
				{
					cout << "数组越界！行号为：" << line << endl; exit(0);
				}

				array_num += token.name;
				NextToken();

				if (TYPEL.at(array_typel.ctp).tval != 'a' && token.name == ","){ cout << "超出数组维度！行号为：" << line << endl; exit(0); }
				array_typel = AINEL.at(TYPEL.at(array_typel.ctp).tpoint);

				while (token.name == ",")
				{
					array_num += token.name;
					NextToken();
					if (IT_Type(token) != 3){ cout << "语法错误！数组使用有误！行号为：" << line << endl; exit(0); }

					if (token.type == 3 && (stoi(token.name) > array_typel.up || stoi(token.name) < array_typel.low))
					{
						cout << "数组越界！行号为：" << line << endl; exit(0);
					}

					array_num += token.name;
					NextToken();

					//判断数组维度是否越界

					if (TYPEL.at(array_typel.ctp).tval != 'a' && token.name == ","){ cout << "超出数组维度！行号为：" << line << endl; exit(0); }
					//if (TYPEL.at(array_typel.ctp).tval == 'a' && token.name != ","){ cout << "您确认要使用数组类型直接进行运算吗？要不还是放我一码呗 行号为：" << line << endl; exit(0); }
					if (token.name == ",")
						array_typel = AINEL.at(TYPEL.at(array_typel.ctp).tpoint);

				}
				if (token.name != "]"){ cout << "语法错误！数组使用有误！缺“]”行号为：" << line << endl; exit(0); }
				array_num += token.name;
				SEM.top().name += array_num;
				NextToken();
				/*
				string array_num = token.name;//暂存数组的下标
				if (TYPEL.at(SYNBL.at(search_SYNBL(Struct_Node.name)).type).tval != 'a'){ cout << "该变量不是数组！行号为：" << line << endl; exit(0); }
				//SEM.pop();
				NextToken();
				if (IT_Type(token) != 3){ cout << "语法错误！数组使用有误！行号为：" << line << endl; exit(0); }
				array_num += token.name;
				NextToken();
				while (token.name == ",")
				{
					array_num += token.name;
					NextToken();
					if (IT_Type(token) != 3){ cout << "语法错误！数组使用有误！行号为：" << line << endl; exit(0); }
					array_num += token.name;
					NextToken();

				}
				if (token.name != "]"){ cout << "语法错误！数组使用有误！缺“]”行号为：" << line << endl; exit(0); }
				array_num += token.name;
				SEM.top().name += array_num;
				NextToken();*/
			}
		}
		else if (token.name == "[")
		{//数组
			
			
			if (TYPEL.at(SYNBL.at(search_SYNBL(SEM.top().name)).type).tval != 'a'){ cout << "该变量不是数组！行号为：" << line << endl; exit(0); }
			
			Ainel_item array_typel = AINEL.at(TYPEL.at(SYNBL.at(search_SYNBL(SEM.top().name)).type).tpoint);
			string array_num = token.name;//暂存数组的下标
			//SEM.pop();
			NextToken();
			if (IT_Type(token) != 3){ cout << "语法错误！数组使用有误！行号为：" << line << endl; exit(0); }
			
			if (token.type == 3 && (stoi(token.name) > array_typel.up || stoi(token.name) < array_typel.low))
			{
				cout << "数组越界！行号为：" << line << endl; exit(0);
			}

			array_num += token.name;
			NextToken();

			if (TYPEL.at(array_typel.ctp).tval != 'a' && token.name == ","){ cout << "超出数组维度！行号为：" << line << endl; exit(0); }
			array_typel = AINEL.at(TYPEL.at(array_typel.ctp).tpoint);
			
			while (token.name == ",")
			{
				array_num += token.name;
				NextToken();
				if (IT_Type(token) != 3){ cout << "语法错误！数组使用有误！行号为：" << line << endl; exit(0); }

				if (token.type == 3 && (stoi(token.name) > array_typel.up || stoi(token.name) < array_typel.low))
				{
					cout << "数组越界！行号为：" << line << endl; exit(0);
				}
				
				array_num += token.name;
				NextToken();
				
				//判断数组维度是否越界

				if (TYPEL.at(array_typel.ctp).tval != 'a' && token.name == ","){ cout << "超出数组维度！行号为：" << line << endl; exit(0); }
				//if (TYPEL.at(array_typel.ctp).tval == 'a' && token.name != ","){ cout << "您确认要使用数组类型直接进行运算吗？要不还是放我一码呗 行号为：" << line << endl; exit(0); }
				if (token.name == ",")
					array_typel = AINEL.at(TYPEL.at(array_typel.ctp).tpoint);

			}
			if (token.name != "]"){ cout << "语法错误！数组使用有误！缺“]”行号为：" << line << endl; exit(0); }
			array_num += token.name;
			SEM.top().name += array_num;
			NextToken();
		}
		else if (token.name == "(")
		{//函数调用
			if (SYNBL.at(search_SYNBL(SEM.top().name)).cat == 'f')
			{
				NextToken();
				FUNCTION_VN();
				QT.push_back({ "call", SEM.top().name, to_string(function_vn_num), "_" });

				//返回值判断
				//有返回值时不需要SEM.pop（）
				//SEM.pop();
				Vn_Num = 0;
				if (token.name != ")"){ cout << "函数调用缺“）”！行号为：" << line << endl; exit(0); }
				NextToken();
			}
			/*else if ((SYNBL.at(search_SYNBL(SEM.top().name)).cat == 'p'))
			{
				NextToken();
				PROCEDURE_VN();
				QT.push_back({ "call", SEM.top().name, "_" , "_" });
				if (token.name != ")"){ cout << "函数调用缺“）”！行号为：" << line << endl; exit(0); }
				Vn_Num = 0;
				NextToken();
			}*/
			else {cout << "语法错误！非函数名后有“（”行号为：" << line << endl; exit(0); }
			
		}
		else  return;
	}
	else { cout << "该标识符不是可操作变量！行号为：" << line << endl; exit(0); }

}

//因子
void FACTOR()
{
	if (token.type == 2)
	{ 
		if (search_SYNBL(token.name) == -1){ cout << token.name << "没有被声明！行号为：" << line << endl; exit(0); }
		//将当前token压入语义栈
		//SEM.push(token);
		ID_SID_OPTION();

		//NextToken(); 
		return; 
	}
	else if (token.type == 3){ SEM.push(token); NextToken(); return; }
	else if (token.type == 4){ SEM.push(token); NextToken(); return; }
	else if (token.type == 6){ SEM.push(token); NextToken(); return; }
	else if (token.type == 7){ SEM.push(token); NextToken(); return; }
	else if (token.name == "-")
	{
		NextToken();
		if (token.type == 3){ SEM.push({ token.type, "-" + token.name }); NextToken(); return; }
		else if (token.type == 4){ SEM.push({ token.type, "-" + token.name }); NextToken(); return; }
		else { cout << "语法错误！负号后常数类型错误！行号为：" << line << endl; exit(0); }
	}
	else if (token.name == "(")
	{ 
		NextToken(); 
		EXPRESSION();
		if (token.name == ")"){ NextToken(); return; }
	}
	else { cout <<  "语法错误！表达式缺因子！行号为：" << line << endl; exit(0); }
}

//项
void TERM()
{
	FACTOR();
	while (token.name == "*" || token.name == "/")
	{
		if (token.name == "*")
		{
			NextToken();
			FACTOR();
			//保存四元式
			Token t1 = SEM.top();
			SEM.pop();
			Token t2 = SEM.top();
			SEM.pop();
			//判断两操作数类型是否一致,若操作数中有数组或结构体未做未填
			if (IT_Type(t1) != IT_Type(t2)){ cout << "两操作数类型不匹配！行号为：" << line << endl; exit(0); }
			Tnum += to_string(num_T++);
			QT.push_back({ "*", t2.name, t1.name, Tnum });
			//cout << "(" << QT.back().w << "," << QT.back().o1 << "," << QT.back().o2 << "," << QT.back().t << ")" << endl;
			
			SEM.push({ 2, Tnum });
			//将中间变量t存入符号表
			Write_T_SYN(t1, t2);
			Tnum.erase(1, Tnum.size() - 1);
		}
		else if (token.name == "/")
		{
			NextToken();
			FACTOR();
			//保存四元式
			Token t1 = SEM.top();
			SEM.pop();
			Token t2 = SEM.top();
			SEM.pop();

			//判断两操作数类型是否一致,若操作数中有数组或结构体未做未填
			if (IT_Type(t1) != IT_Type(t2)){ cout << "两操作数类型不匹配！行号为：" << line << endl; exit(0); }
			Tnum += to_string(num_T++);
			QT.push_back({ "/", t2.name, t1.name, Tnum });
			//cout << "(" << QT.back().w << "," << QT.back().o1 << "," << QT.back().o2 << "," << QT.back().t << ")" << endl;
			SEM.push({ 2, Tnum });
			//将中间变量t存入符号表
			Write_T_SYN(t1, t2);

			Tnum.erase(1, Tnum.size() - 1);
		}
	}
}

//表达式
void EXPRESSION()
{
	TERM();
	while (token.name == "+" || token.name == "-")
	{
		if (token.name == "+")
		{
			NextToken();
			TERM();
			//保存四元式
			Token t1 = SEM.top();
			SEM.pop();
			Token t2 = SEM.top();
			SEM.pop();
			if (IT_Type(t1) != IT_Type(t2)){ cout << "两操作数类型不匹配！行号为：" << line << endl; exit(0); }
			Tnum += to_string(num_T++);
			QT.push_back({ "+", t2.name, t1.name, Tnum });
			//cout << "(" << QT.back().w << "," << QT.back().o1 << "," << QT.back().o2 << "," << QT.back().t << ")" << endl;
			SEM.push({ 2, Tnum });
			//将中间变量t存入符号表
			Write_T_SYN(t1, t2);

			Tnum.erase(1, Tnum.size() - 1);
		}
		else if (token.name == "-")
		{
			NextToken();
			TERM();
			//保存四元式
			Token t1 = SEM.top();
			SEM.pop();
			Token t2 = SEM.top();
			SEM.pop();
			if (IT_Type(t1) != IT_Type(t2)){ cout << "两操作数类型不匹配！行号为：" << line << endl; exit(0); }
			Tnum += to_string(num_T++);
			QT.push_back({ "-", t2.name, t1.name, Tnum });
			//cout << "(" << QT.back().w << "," << QT.back().o1 << "," << QT.back().o2 << "," << QT.back().t << ")" << endl;
			SEM.push({ 2, Tnum });
			//将中间变量t存入符号表
			Write_T_SYN(t1, t2);

			Tnum.erase(1, Tnum.size() - 1);
		}
	}
}

//函数调用参数类型匹配
void PARAM_MATCH(string func, Token func_param, int vn_num)
{
	if (vn_num > PFINEL.at(SYNBL.at(search_SYNBL(SEM.top().name)).addr).fn)
	{
		cout << "函数调用时传递参数过多！行号为：" << line << endl; exit(0);
	}
	if (SYNBL.at(search_SYNBL(SEM.top().name) + Vn_Num + 1).cat == 'n')
	{
		if (func_param.type == 3 || func_param.type == 4 || func_param.type == 6 || func_param.type == 7 || SYNBL.at(search_SYNBL(token.name)).cat == 'c')
		{
			cout << "函数换名形参不可传入常量！行号为：" << line << endl; exit(0);
		}
	}
	int param_match_temp1;
	switch (TYPEL.at(SYNBL.at(search_SYNBL(func) + vn_num + 1).type).tval)
	{
	case 'i':param_match_temp1 = 3; break;
	case 'r':param_match_temp1 = 4; break;
	case 'c':param_match_temp1 = 6; break;
	case 'b':param_match_temp1 = 29; break;//布尔型
	case 'a':param_match_temp1 = 8; break;//数组型
	case 'd':param_match_temp1 = 9; break;//结构型
	default:{cout << "未知的标识符类型码！行号为：" << line << endl; exit(0); }
	}
	
	//int param_match_temp1 = IT_Type({ 2, SYNBL.at(search_SYNBL(func) + vn_num + 1).name });
	int param_match_temp2 = IT_Type(func_param);//'i';// TYPEL.at(SYNBL.at(search_SYNBL(func_param.name)).type).tval;//3;//IT_Type(func_param);
	if (param_match_temp1 == param_match_temp2)
	{
		return;
	}
	else
	{
		cout << "函数调用参数不匹配！行号为：" << line << endl; exit(0);
	}

}

//函数调用参数
void FUNCTION_VN()
{//函数传参类型匹配未填
	if (token.type == 2)
	{
		//判断参数类型是否匹配
		PARAM_MATCH(SEM.top().name, token, Vn_Num);

		if (SYNBL.at(search_SYNBL(token.name)).cat == 'v' || SYNBL.at(search_SYNBL(token.name)).cat == 'c')
		{
			QT.push_back({ SEM.top().name, token.name, "_", "_" });
			Vn_Num++;
			NextToken();
			if (token.name == ",")
			{
				NextToken();
				FUNCTION_VN();
			}
			else return;
		}
		else { cout << "函数参数错误！行号为：" << line << endl; exit(0); }
	}
	else if (token.type == 3 || token.type == 4 || token.type == 6 || token.type == 7)
	{
		//判断参数类型是否匹配
		PARAM_MATCH(SEM.top().name, token, Vn_Num);

		QT.push_back({ SEM.top().name, token.name, "_", "_" });
		Vn_Num++;
		NextToken();
		if (token.name == ",")
		{
			NextToken();
			FUNCTION_VN();
		}
		else return;
	}
	else return;
}

//过程函数调用参数
void PROCEDURE_VN()
{
	if (token.type == 2)
	{
		//判断参数类型是否匹配
		PARAM_MATCH(SEM.top().name, token, Vn_Num);

		for (QT_item & q:PROCE_QT)
		{
			if (q.o1 == SYNBL.at(search_SYNBL(SEM.top().name) + Vn_Num + 1).name)
				q.o1 = token.name;
			else if (q.o2 == SYNBL.at(search_SYNBL(SEM.top().name) + Vn_Num + 1).name)
				q.o2 = token.name;
			else if (q.t == SYNBL.at(search_SYNBL(SEM.top().name) + Vn_Num + 1).name)
				q.t = token.name;
		}

		if (SYNBL.at(search_SYNBL(token.name)).cat == 'v' || SYNBL.at(search_SYNBL(token.name)).cat == 'c')
		{
			//QT.push_back({ SEM.top().name, token.name, "_", "_" });
			Vn_Num++;
			NextToken();
			if (token.name == ",")
			{
				NextToken();
				PROCEDURE_VN();
			}
			else return;
		}
		else { cout << "函数参数错误！行号为：" << line << endl; exit(0); }
	}
	else if (token.type == 3 || token.type == 4 || token.type == 6 || token.type == 7)
	{
		//判断参数类型是否匹配
		PARAM_MATCH(SEM.top().name, token, Vn_Num);

		for (QT_item & q : PROCE_QT)
		{
			if (q.o1 == SYNBL.at(search_SYNBL(SEM.top().name) + Vn_Num + 1).name)
				q.o1 = token.name;
			else if (q.o2 == SYNBL.at(search_SYNBL(SEM.top().name) + Vn_Num + 1).name)
				q.o2 = token.name;
			else if (q.t == SYNBL.at(search_SYNBL(SEM.top().name) + Vn_Num + 1).name)
				q.t = token.name;
		}

		//QT.push_back({ SEM.top().name, token.name, "_", "_" });
		Vn_Num++;
		NextToken();
		if (token.name == ",")
		{
			NextToken();
			PROCEDURE_VN();
		}
		else return;
	}
	else return;
}

//表达式语句后部分
void EVA_SUBPROGRAM_OPTION()
{
	SEM.push({ 2, token.name });
	//换名形参、赋值形参没做
	if (SYNBL.at(search_SYNBL(token.name)).cat == 'v' || SYNBL.at(search_SYNBL(token.name)).cat == 'n' || SYNBL.at(search_SYNBL(token.name)).cat == 'e')//|| SYNBL.at(search_SYNBL(token.name)).cat == 'n' || SYNBL.at(search_SYNBL(token.name)).cat == 'f'
	{//SYNBL.at(search_SYNBL(SEM.top().name)).cat == 'f'
		NextToken();
		if (token.name == ":=")
		{//表达式
			//类型匹配时暂未考虑数组和结构体
			NextToken();
			EXPRESSION();

			//生成四元式
			Token t1 = SEM.top();
			SEM.pop();
			Token t2 = SEM.top();
			SEM.pop();
			//赋值前类型匹配
			if (IT_Type(t1) != IT_Type(t2)){ cout << "赋值语句两操作数类型不匹配！行号为：" << line << endl; exit(0); }
			QT.push_back({ ":=", t1.name, "_", t2.name });
			//cout << "(" << QT.back().w << "," << QT.back().o1 << "," << QT.back().o2 << "," << QT.back().t << ")" << endl;

		}
		else if (token.name == ".")//若不为ID，记得删掉SEM.top
		{//结构体
			NextToken();
			if (token.type != 2){ cout << "结构体变量“.”后缺标识符！行号为：" << line << endl; exit(0); }

			if (TYPEL.at(SYNBL.at(search_SYNBL(SEM.top().name)).type).tval != 'd'){ cout << "该变量不是结构体！行号为：" << line << endl; exit(0); }
			if (search_RINFL(SEM.top().name, token.name) == -1){ cout << "该结构体中没有该项！行号为：" << line << endl; exit(0); }

			SEM.top().name += '.';
			SEM.top().name += token.name;
			Token Struct_Node = token;//暂存当前标识符用来判断是否为结构体类型
			NextToken();
			while (token.name == ".")
			{
				NextToken();
				if (token.type != 2){ cout << "结构体变量“.”后缺标识符！行号为：" << line << endl; exit(0); }
			
				if (TYPEL.at(SYNBL.at(search_SYNBL(Struct_Node.name)).type).tval != 'd'){ cout << "该变量不是结构体！行号为：" << line << endl; exit(0); }
				if (search_RINFL(Struct_Node.name, token.name) == -1){ cout << "该结构体中没有该项！行号为：" << line << endl; exit(0); }
				
				SEM.top().name += '.';
				SEM.top().name += token.name;
				Struct_Node = token;
			}
			if (token.name == "[")
			{

				if (TYPEL.at(SYNBL.at(search_SYNBL(Struct_Node.name)).type).tval != 'a'){ cout << "该变量不是数组！行号为：" << line << endl; exit(0); }

				Ainel_item array_typel = AINEL.at(TYPEL.at(SYNBL.at(search_SYNBL(Struct_Node.name)).type).tpoint);
				string array_num = token.name;//暂存数组的下标
				//SEM.pop();
				NextToken();
				if (IT_Type(token) != 3){ cout << "语法错误！数组使用有误！行号为：" << line << endl; exit(0); }
				
				if (token.type == 3 && (stoi(token.name) > array_typel.up || stoi(token.name) < array_typel.low))
				{
					cout << "数组越界！行号为：" << line << endl; exit(0);
				}
				
				array_num += token.name;
				NextToken();

				if (TYPEL.at(array_typel.ctp).tval != 'a' && token.name == ","){ cout << "超出数组维度！行号为：" << line << endl; exit(0); }
				array_typel = AINEL.at(TYPEL.at(array_typel.ctp).tpoint);

				while (token.name == ",")
				{
					array_num += token.name;
					NextToken();
					if (IT_Type(token) != 3){ cout << "语法错误！数组使用有误！行号为：" << line << endl; exit(0); }

					if (token.type == 3 && (stoi(token.name) > array_typel.up || stoi(token.name) < array_typel.low))
					{
						cout << "数组越界！行号为：" << line << endl; exit(0);
					}

					array_num += token.name;
					NextToken();

					//判断数组维度是否越界

					if (TYPEL.at(array_typel.ctp).tval != 'a' && token.name == ","){ cout << "超出数组维度！行号为：" << line << endl; exit(0); }
					//if (TYPEL.at(array_typel.ctp).tval == 'a' && token.name != ","){ cout << "您确认要使用数组类型直接进行运算吗？要不还是放我一码呗 行号为：" << line << endl; exit(0); }
					if (token.name == ",")
						array_typel = AINEL.at(TYPEL.at(array_typel.ctp).tpoint);

				}
				if (token.name != "]"){ cout << "语法错误！数组使用有误！缺“]”行号为：" << line << endl; exit(0); }
				array_num += token.name;
				SEM.top().name += array_num;
				NextToken();
				
			}
			if (token.name == ":=")
			{
				NextToken();
				EXPRESSION();

				//生成四元式
				Token t1 = SEM.top();
				SEM.pop();
				Token t2 = SEM.top();
				SEM.pop();
				//赋值前类型匹配
				if (IT_Type(t1) != IT_Type(t2)){ cout << "赋值语句两操作数类型不匹配！行号为：" << line << endl; exit(0); }
				QT.push_back({ ":=", t1.name, "_", t2.name });
				//cout << "(" << QT.back().w << "," << QT.back().o1 << "," << QT.back().o2 << "," << QT.back().t << ")" << endl;
			}
			else { cout << "表达式缺少“：=”！行号为：" << line << endl; exit(0); }
		}
		else if (token.name == "[")
		{//数组
			if (TYPEL.at(SYNBL.at(search_SYNBL(SEM.top().name)).type).tval != 'a'){ cout << "该变量不是数组！行号为：" << line << endl; exit(0); }

			Ainel_item array_typel = AINEL.at(TYPEL.at(SYNBL.at(search_SYNBL(SEM.top().name)).type).tpoint);
			string array_num = token.name;//暂存数组的下标
			//SEM.pop();
			NextToken();
			if (IT_Type(token) != 3){ cout << "语法错误！数组使用有误！行号为：" << line << endl; exit(0); }
			
			if (token.type == 3 && (stoi(token.name) > array_typel.up || stoi(token.name) < array_typel.low))
			{
				cout << "数组越界！行号为：" << line << endl; exit(0);
			}
			
			array_num += token.name;
			NextToken();

			if (TYPEL.at(array_typel.ctp).tval != 'a' && token.name == ","){ cout << "超出数组维度！行号为：" << line << endl; exit(0); }
			array_typel = AINEL.at(TYPEL.at(array_typel.ctp).tpoint);

			while (token.name == ",")
			{
				array_num += token.name;
				NextToken();
				if (IT_Type(token) != 3){ cout << "语法错误！数组使用有误！行号为：" << line << endl; exit(0); }

				if (token.type == 3 && (stoi(token.name) > array_typel.up || stoi(token.name) < array_typel.low))
				{
					cout << "数组越界！行号为：" << line << endl; exit(0);
				}

				array_num += token.name;
				NextToken();

				//判断数组维度是否越界

				if (TYPEL.at(array_typel.ctp).tval != 'a' && token.name == ","){ cout << "超出数组维度！行号为：" << line << endl; exit(0); }
				//if (TYPEL.at(array_typel.ctp).tval == 'a' && token.name != ","){ cout << "您确认要使用数组类型直接进行运算吗？要不还是放我一码呗 行号为：" << line << endl; exit(0); }
				if (token.name == ",")
					array_typel = AINEL.at(TYPEL.at(array_typel.ctp).tpoint);

			}

			if (token.name != "]"){ cout << "语法错误！数组使用有误！缺“]”行号为：" << line << endl; exit(0); }
			array_num += token.name;
			SEM.top().name += array_num;
			NextToken();
			if (token.name != ":="){ cout << "语法错误！表达式缺：=！行号为：" << line << endl; exit(0); }
			NextToken();
			EXPRESSION();

			//生成四元式
			Token t1 = SEM.top();
			SEM.pop();
			Token t2 = SEM.top();
			SEM.pop();
			//赋值前类型匹配
			if (IT_Type(t1) != IT_Type(t2)){ cout << "赋值语句两操作数类型不匹配！行号为：" << line << endl; exit(0); }
			QT.push_back({ ":=", t1.name, "_", t2.name });
			//cout << "(" << QT.back().w << "," << QT.back().o1 << "," << QT.back().o2 << "," << QT.back().t << ")" << endl;
		}
		else  { cout << "表达式语法错误！行号为：" << line << endl; exit(0); }
	}
	else if (SYNBL.at(search_SYNBL(token.name)).cat == 'f')
	{//函数调用
		NextToken();
		if (token.name != "("){ cout << "语法错误！函数调用缺“（”！行号为：" << line << endl; exit(0); }
		NextToken();
		
		PROCEDURE_VN();

		//函数调用匹配参数个数
		if (Vn_Num != PFINEL.at(SYNBL.at(search_SYNBL(SEM.top().name)).addr).fn){ cout << "函数调用参数个数不匹配！行号为：" << line << endl; exit(0); }
		
		QT.push_back({ "call", SEM.top().name, to_string(Vn_Num), "_" });
		SEM.pop();
		Vn_Num = 0;//参数个数置0
		if (token.name != ")"){ cout << "函数调用缺“）”！行号为：" << line << endl; exit(0); }
		NextToken();
	}
	else if (SYNBL.at(search_SYNBL(token.name)).cat == 'p')
	{//过程函数调用
		NextToken();
		if (token.name != "("){ cout << "语法错误！函数调用缺“（”！行号为：" << line << endl; exit(0); }
		NextToken();

		QT.push_back({ "callproce", SEM.top().name, "_", "_" });

		vector<QT_item> QT_temp;
		for (QT_item & q : PROCE_QT)
			QT_temp.push_back(q);

		PROCEDURE_VN();

		for (QT_item & q : PROCE_QT)
			QT.push_back(q);

		for (QT_item & q : QT_temp)
			PROCE_QT.push_back(q);

		//函数调用匹配参数个数
		if (Vn_Num != PFINEL.at(SYNBL.at(search_SYNBL(SEM.top().name)).addr).fn){ cout << "函数调用参数个数不匹配！行号为：" << line << endl; exit(0); }

		QT.push_back({ "endpro", SEM.top().name, "_",  "_" });
		SEM.pop();
		Vn_Num = 0;//参数个数置0
		if (token.name != ")"){ cout << "函数调用缺“）”！行号为：" << line << endl; exit(0); }
		NextToken();
	}
	else  { cout << "该标识符不是变量也不是函数！不可操作！行号为：" << line << endl; exit(0); }
}

//判断语句
void CONDITION()
{
	EXPRESSION();
	if (token.name == ">=" || token.name == "<=" || token.name == "=" || token.name == "<" || token.name == ">")
	{
		string logic_operator = token.name;
		NextToken();
		EXPRESSION();
		//保存四元式
		Token t1 = SEM.top();
		SEM.pop();
		Token t2 = SEM.top();
		SEM.pop();
		if (IT_Type(t1) != IT_Type(t2)){ cout << "两操作数类型不匹配！行号为：" << line << endl; exit(0); }
		if (IT_Type(t1) != 3 && IT_Type(t1) != 4 && IT_Type(t1) != 6){ cout << "判断语句不支持操作数类型！行号为：" << line << endl; exit(0); }
		Tnum += to_string(num_T++);
		QT.push_back({ logic_operator, t2.name, t1.name, Tnum });
		//cout << "(" << QT.back().w << "," << QT.back().o1 << "," << QT.back().o2 << "," << QT.back().t << ")" << endl;
		SEM.push({ 2, Tnum });
		//将中间变量t存入符号表 布尔表
		SYNBL.push_back({ Tnum, 3, 'v', BOOL.size(), Level });
		BOOL.push_back(true);
		//Write_T_SYN(t1, t2);

		Tnum.erase(1, Tnum.size() - 1);
	}
}

//while语句
void WHILE_SENTENCE()
{
	//生成while四元式
	QT.push_back({ "while", "_", "_", "_" });

	CONDITION();

	if (token.name != "do"){ cout << "if语句中缺“then”！语法错误！行号为：" << line << endl; exit(0); }
	NextToken();

	//生成while do四元式
	QT.push_back({ "do", SEM.top().name, "_", "_" });
	SEM.pop();

	if (token.name != "begin"){ cout << "if语句中缺“begin”！语法错误！行号为：" << line << endl; exit(0); }
	NextToken();


	SEN_SEQUENCE();
	if (token.name != "end"){ cout << "if语句中缺“end”！语法错误！行号为：" << line << endl; exit(0); }

	//生成while end四元式
	QT.push_back({ "whileend", "_", "_", "_" });

	NextToken();
	if (token.name != ";"){ cout << "if语句中end后缺“;”！语法错误！行号为：" << line << endl; exit(0); }
	NextToken();
}

//if语句
void IF_SENTENCE()
{
	CONDITION();
	if (token.name != "then"){ cout << "if语句中缺“then”！语法错误！行号为：" << line << endl; exit(0); }
	NextToken();
	if (token.name != "begin"){ cout << "if语句中缺“begin”！语法错误！行号为：" << line << endl; exit(0); }
	NextToken();
	//生成if四元式
	QT.push_back({ "if", SEM.top().name, "_", "_" });
	SEM.pop();

	SEN_SEQUENCE();
	if (token.name == "else")
	{
		//生成if else四元式
		QT.push_back({ "else", "_", "_", "_" });

		NextToken();
		SEN_SEQUENCE();
		if (token.name != "end"){ cout << "if语句中缺“end”！语法错误！行号为：" << line << endl; exit(0); }

		//生成if end四元式
		QT.push_back({ "ifend", "_", "_", "_" });

		NextToken();
		if (token.name != ";"){ cout << "if语句中end后缺“;”！语法错误！行号为：" << line << endl; exit(0); }
		NextToken();
	}
	//else { cout << "if语句中缺“else”！语法错误！行号为：" << line << endl; exit(0); }
	//if语句中不一定有else

}

//操作语句
void SEN_SEQUENCE()
{
	if (token.type == 2)
	{
		if (search_SYNBL(token.name) == -1){ cout << token.name << "标识符未定义！行号为：" << line << endl; exit(0); }
		EVA_SUBPROGRAM_OPTION();
		if (token.name == "end")return;
		if (token.name != ";"){ cout << "语法错误！语句后缺\";\"行号为：" << line << endl; exit(0); }
		NextToken();
		SEN_SEQUENCE();
		/*ID_name = token.name;//保存当前标识符，在该标识符为函数时使用
		//查询当前被赋值变量是否已声明
		if (SYNBL.at(search_SYNBL(token.name)).cat == 'v')
		{//表达式
			SEM.push({ 2, ID_name});
			EVA_SUBPROGRAM_OPTION();

			//将EXPRESSION的结果存入结果变量中
			Token t1 = SEM.top();
			SEM.pop();
			Token t2 = SEM.top();
			SEM.pop();
			//赋值前类型匹配
			if (IT_Type(t1) != IT_Type(t2)){ cout << "赋值语句两操作数类型不匹配！行号为：" << line << endl; exit(0); }
			QT.push_back({ ":=", t1.name, "\0", t2.name });
			//cout << "(" << QT.back().w << "," << QT.back().o1 << "," << QT.back().o2 << "," << QT.back().t << ")" << endl;
		}
		else if (SYNBL.at(search_SYNBL(token.name)).cat == 'f')
		{//函数调用
			EVA_SUBPROGRAM_OPTION();
			//产生函数调用,传参暂未处理
			if (function_vn_num == 0)
				QT.push_back({ ID_name, "\0", "\0", "\0", });
			else { cout << "函数调用有参数！行号为：" << line << endl; exit(0); }

			ID_name.clear();//函数调用结束
		}
	
		else{ cout << token.name << "没有被声明！行号为：" << line << endl; exit(0); }
		
		if (search_SYNBL(token.name) == -1){ cout << token.name << "没有被声明！行号为：" << line << endl; exit(0); }

		SEM.push(token);
		NextToken();*/

	}
	else if (token.name == "if")
	{
		NextToken();
		IF_SENTENCE();
		SEN_SEQUENCE();
	}
	else if (token.name == "while")
	{
		NextToken();
		WHILE_SENTENCE();
		SEN_SEQUENCE();
	}
	else if (token.name == ";"){ NextToken(); SEN_SEQUENCE(); return; }
	else return;
}

//语句
void COM_SENTENCE()
{
	if (token.name == "begin")
	{
		NextToken();

		//插入常量定义的四元式
		for (int i = 0; i < SYNBL.size(); i++)
			if (SYNBL.at(i).cat == 'c' && SYNBL.at(i).level <= Level)
				QT.push_back({":=", CONSL.at(SYNBL.at(i).addr).con, "_", SYNBL.at(i).name});
		


		SEN_SEQUENCE();
		if (token.name == "end")
		{ //生成函数结束标志四元式
			QT.push_back({ "end", SEM.top().name, "_", "_" });
			SEM.pop();
			NextToken(); 
			return; 
		}
		else { cout << "缺“end”关键字！语法错误！行号为：" << line  << endl; exit(0); }
	}
	else { cout << "缺“begin”关键字！语法错误！行号为：" << line  << endl; exit(0); }
}

//标识符列表
void ID_LIST()
{
	if (token.type == 2)
	{	//将当前声明的变量暂存临时符号表
		if (search_SYNBL(token.name) == -1)
			Name_temp.push_back({ token.name, -1, '\0', -1, Level });//当前标识符初始化时type，addr均为-1，cat为‘\0’
			
		else { cout << "变量重定义！行号为：" << line << endl; exit(0); }

		NextToken();
		while (token.name == ",")
		{
			NextToken();
			if (token.type == 2) 
			{
				//将当前声明的变量存入临时符号表
				//SYNBL.push_back({ token.name, -1, 'v', -1, Level });
				if (search_SYNBL(token.name) == -1)
					Name_temp.push_back({ token.name, -1, '\0', -1, Level });//当前标识符初始化时type，addr均为-1，cat为‘\0’
					//将当前声明的变量暂存临时符号表
				else { cout << "变量重定义！行号为：" << line << endl; exit(0); }
				NextToken();
			}
			else{ cout << "缺标识符！语法错误！行号为：" << line << endl; exit(0); }
		}
	}
	else { cout << "缺标识符！语法错误！行号为：" << line  << endl; exit(0); }

}

//数组类型
void ARRAY_TYPE()
{
	if (token.name == "integer")
	{
		AINEL.back().ctp = 0;
		AINEL.back().clen = 4;
		NextToken();
	}
	else if (token.name == "real")
	{
		AINEL.back().ctp = 1;
		AINEL.back().clen = 8;
		NextToken();
	}
	else if (token.name == "char")
	{
		AINEL.back().ctp = 2;
		AINEL.back().clen = 1;
		NextToken();
	}
	else if (token.name == "bool")
	{
		AINEL.back().ctp = 3;
		AINEL.back().clen = 1;
		NextToken();
	}
	else if (token.type == 2)
	{//填写类型信息
		if (search_SYNBL(token.name) != -1 && SYNBL.at(search_SYNBL(token.name)).cat == 't')
		{//填写类型信息
			AINEL.back().ctp = SYNBL.at(search_SYNBL(token.name)).type;
			AINEL.back().clen = LENL.at(SYNBL.at(search_SYNBL(token.name)).addr);
		}
		else { cout << "变量类型错误！语法错误！行号为：" << line << endl; exit(0); }
		NextToken();
	}
	else if (token.name == "array")
	{

		NextToken();
		if (token.name != "["){ cout << "数组定义缺界符\"[\"！语法错误！行号为：" << line << endl; exit(0); }
		NextToken();
		if (token.type != 5){ cout << "数组定义，缺数组范围！语法错误！行号为：" << line << endl; exit(0); }
		//填写符号表类型表数组表

		string low = "\0", up = "\0";
		int subscript;
		for (subscript = 0; token.name[subscript] != '.'; subscript++)low += token.name[subscript];
		up = token.name.erase(0, subscript + 2);//数组的上界，对token进行理论修改！
		up = token.name;

		AINEL.back().ctp = TYPEL.size();
		TYPEL.push_back({ 'a', AINEL.size() });
		AINEL.push_back({ stoi(low), stoi(up), -1, 0 });//数组表的ctp和clen初始为-1和0 未填

		//填写符号表类型表数组表未填
		/*for (int i = 0; i < Name_temp.size(); i++)
		{
			Name_temp.at(i).cat = 't';
			Name_temp.at(i).type = TYPEL.size();
			Name_temp.at(i).addr = LENL.size();
			//TYPEL_temp.push_back({ 'a', AINEL.size() });
			//Name_temp.at(i).addr = INTEGER.size();
		}
		TYPEL.push_back({ 'a', AINEL.size() });
		AINEL.push_back({ stoi(low), stoi(up), -1, 0 });//数组表的ctp和clen初始为-1和0 未填
		LENL.push_back(0);//长度表的初值为0  未填*/

		NextToken();
		if (token.name != "]"){ cout << "数组定义缺界符\"]\"！语法错误！行号为：" << line << endl; exit(0); }
		NextToken();
		if (token.name != "of"){ cout << "数组定义缺关键字\"of\"！语法错误！行号为：" << line << endl; exit(0); }
		NextToken();

		ARRAY_TYPE();
		//填写数组表类型信息
		int ainel_num;
		for (ainel_num = AINEL.size() - 1; AINEL.at(ainel_num).clen != 0; ainel_num--);
		AINEL.at(ainel_num).clen = AINEL.at(ainel_num + 1).clen * (AINEL.at(ainel_num + 1).up - AINEL.at(ainel_num + 1).low + 1);//填写数组表的长度


	}
	else { cout << "变量定义缺类型！语法错误！行号为：" << line << endl; exit(0); }

}

//声明类型
void DETYPE()
{
	if (token.name == "integer")
	{
		SYNBL.back().type = 0;
		LENL.push_back(4);

		NextToken();
	}
	else if (token.name == "real")
	{
		SYNBL.back().type = 1;
		LENL.push_back(8);

		NextToken();
	}
	else if (token.name == "char")
	{
		SYNBL.back().type = 2;
		LENL.push_back(1);

		NextToken();
	}
	else if (token.name == "bool")
	{
		SYNBL.back().type = 3;
		LENL.push_back(1);

		NextToken();
	}
	else if (token.type == 2)
	{//填写类型信息
		if (search_SYNBL(token.name) != -1 && SYNBL.at(search_SYNBL(token.name)).cat == 't')
		{//填写类型信息
			SYNBL.back().type = SYNBL.at(search_SYNBL(token.name)).type;
			LENL.push_back(LENL.at(SYNBL.at(search_SYNBL(token.name)).addr));

			NextToken();
		}
		else { cout << "变量类型错误！语法错误！行号为：" << line << endl; exit(0); }
	}
	else if (token.name == "array")
	{
		NextToken();
		if (token.name != "["){ cout << "数组定义缺界符\"[\"！语法错误！行号为：" << line << endl; exit(0); }
		NextToken();
		if (token.type != 5){ cout << "数组定义，缺数组范围！语法错误！行号为：" << line << endl; exit(0); }

		string low = "\0", up = "\0";
		int subscript;
		for (subscript = 0; token.name[subscript] != '.'; subscript++)low += token.name[subscript];
		up = token.name.erase(0, subscript + 2);//数组的上界，对token进行理论修改！
		up = token.name;

		//填写符号表类型表数组表
		SYNBL.back().type = TYPEL.size();
		SYNBL.back().addr = LENL.size();
		TYPEL.push_back({ 'a', AINEL.size() });
		AINEL.push_back({ stoi(low), stoi(up), -1, 0 });//数组表的ctp和clen初始为-1和0 未填
		LENL.push_back(0);//长度表的初值为0  未填

		NextToken();
		if (token.name != "]"){ cout << "数组定义缺界符\"]\"！语法错误！行号为：" << line << endl; exit(0); }
		NextToken();
		if (token.name != "of"){ cout << "数组定义缺关键字\"of\"！语法错误！行号为：" << line << endl; exit(0); }
		NextToken();

		ARRAY_TYPE();
		//填写数组表类型信息
		int i;
		for (i = SYNBL.size() - 1; i >= 0; i--)
		{
			if (SYNBL.at(i).cat == 't' && SYNBL.at(i).addr == LENL.size() - 1)
				break;
		}
		LENL.back() = AINEL.at(TYPEL.at(SYNBL.at(i).type).tpoint).clen * (AINEL.at(TYPEL.at(SYNBL.at(i).type).tpoint).up - AINEL.at(TYPEL.at(SYNBL.at(i).type).tpoint).low + 1);
	}
	else { cout << "变量定义缺类型！语法错误！行号为：" << line << endl; exit(0); }
}

//类型
void TYPE()
{
	if (token.name == "integer")
	{
		for (int i = 0; i < Name_temp.size(); i++)
		{
			Name_temp.at(i).type = 0;
			//Name_temp.at(i).addr = INTEGER.size();

		}
		//将临时符号表中的项填入符号表
		//for (int i = 0; i < Name_temp.size(); i++)
		//{
		//	syn_temp.name = Name_temp.at(i);
		//	syn_temp.addr = INTEGER.size();
		//	INTEGER.push_back(0);//变量初始值为0
		//	SYNBL.push_back(syn_temp);//填写符号表
		//}
		//Name_temp.clear();
		NextToken();
	}
	else if (token.name == "real")
	{
		for (int i = 0; i < Name_temp.size(); i++)
		{
			Name_temp.at(i).type = 1;
			//Name_temp.at(i).addr = REAL.size();

		}
		NextToken();
	}
	else if (token.name == "char")
	{
		for (int i = 0; i < Name_temp.size(); i++)
		{
			Name_temp.at(i).type = 2;
			//Name_temp.at(i).addr = INTEGER.size();

		}
		NextToken();
	}
	else if (token.type == 2)
	{//填写类型信息
		if (search_SYNBL(token.name) != -1 && SYNBL.at(search_SYNBL(token.name)).cat == 't')
		{//填写类型信息
			for (int i = 0; i < Name_temp.size(); i++)
			{
				Name_temp.at(i).type = SYNBL.at(search_SYNBL(token.name)).type;
				Name_temp.at(i).addr = SYNBL.at(search_SYNBL(token.name)).addr;
				//Name_temp.at(i).addr = INTEGER.size();
			}
		}
		else { cout << "变量类型错误！语法错误！行号为：" << line << endl; exit(0); }
		NextToken();
	}
	else if (token.name == "array")
	{

		NextToken();
		if (token.name != "["){ cout << "数组定义缺界符\"[\"！语法错误！行号为：" << line << endl; exit(0); }
		NextToken();
		if (token.type != 5){ cout << "数组定义，缺数组范围！语法错误！行号为：" << line << endl; exit(0); }
		//填写符号表类型表数组表

		string low = "\0", up = "\0";
		int subscript;
		for (subscript = 0; token.name[subscript] != '.'; subscript++)low += token.name[subscript];
		up = token.name.erase(0, subscript + 2);//数组的上界，对token进行理论修改！
		up = token.name;
		//for (subscript += 2; token.name[subscript] != '.'; subscript++)up += token.name[subscript];
		//填写符号表类型表数组表未填
		for (int i = 0; i < Name_temp.size(); i++)
		{
			Name_temp.at(i).type = TYPEL.size();
			Name_temp.at(i).addr = LENL.size();
			//TYPEL_temp.push_back({ 'a', AINEL.size() });
			//Name_temp.at(i).addr = INTEGER.size();
		}
		TYPEL.push_back({ 'a', AINEL.size() });
		AINEL.push_back({ stoi(low), stoi(up), -1, 0 });//数组表的ctp和clen初始为-1和0 未填
		LENL.push_back(0);//长度表的初值为0  未填

		NextToken();
		if (token.name != "]"){ cout << "数组定义缺界符\"]\"！语法错误！行号为：" << line << endl; exit(0); }
		NextToken();
		if (token.name != "of"){ cout << "数组定义缺关键字\"of\"！语法错误！行号为：" << line << endl; exit(0); }
		NextToken();

		ARRAY_TYPE();
		//填写数组表类型信息
		LENL.back() = AINEL.at(TYPEL.at(Name_temp.at(0).type).tpoint).clen * (AINEL.at(TYPEL.at(Name_temp.at(0).type).tpoint).up - AINEL.at(TYPEL.at(Name_temp.at(0).type).tpoint).low + 1);
	}
	else { cout << "变量定义缺类型！语法错误！行号为：" << line << endl; exit(0); }

}

//子程序的参数
void ARGUMENT()
{
	if (token.type == 2)
	{//VF赋值形参 种类cat为'e'
		function_vn_num++;//参数个数增加

		//填写符号表和参数表
		Name_temp.push_back({ token.name, -1, 'e', -1, Level });//符号表暂存 类型type，地址addr未填 
		NextToken();
		if (token.name != ":"){ cout << "子函数定义中参数名后缺\"：\" ！语法错误！行号为：" << line << endl; exit(0); }
		NextToken();

		TYPE();
		//填写符号表和参数表
		Type_Write_SYNBL();//填写符号表
		//PARAMBL.push_back(SYNBL.back());//填写参数表

		for (int param_num = 0; param_num < Name_temp.size(); param_num++)
		{
			PARAMBL.push_back(Name_temp.at(param_num));//填写参数表
		}

		Name_temp.clear();
		if (token.name == ";")
		{
			NextToken();
			ARGUMENT();
		}
	}
	else if (token.name == "var")
	{//VN换名形参
		NextToken();
		//填写符号表和参数表
		if (token.type != 2){ cout << "子函数定义中缺标识符！语法错误！行号为：" << line << endl; exit(0); }
		
		function_vn_num++;//参数个数增加
		
		Name_temp.push_back({ token.name, -1, 'n', -1, Level });//符号表暂存 类型type，地址addr未填
		NextToken();
		if (token.name != ":"){ cout << "子函数定义中参数名后缺\"：\" ！语法错误！行号为：" << line << endl; exit(0); }
		NextToken();
		
		TYPE();
		//填写符号表和参数表
		Type_Write_SYNBL();//填写符号表

		for (int param_num = 0; param_num < Name_temp.size(); param_num++)
		{
			PARAMBL.push_back(Name_temp.at(param_num));//填写参数表
		}

		Name_temp.clear();

		if (token.name == ";")
		{
			NextToken();
			ARGUMENT();
		}
	}
	else return;
}

//过程函数的参数
void PARGUMENT()
{
	if (token.type == 2)
	{
		//NextToken();
		//填写符号表和参数表
		if (token.type != 2){ cout << "子函数定义中缺标识符！语法错误！行号为：" << line << endl; exit(0); }

		function_vn_num++;//参数个数增加

		Name_temp.push_back({ token.name, -1, 'n', -1, Level });//符号表暂存 类型type，地址addr未填
		NextToken();
		if (token.name != ":"){ cout << "子函数定义中参数名后缺\"：\" ！语法错误！行号为：" << line << endl; exit(0); }
		NextToken();

		TYPE();
		//填写符号表和参数表
		Type_Write_SYNBL();//填写符号表

		for (int param_num = 0; param_num < Name_temp.size(); param_num++)
		{
			PARAMBL.push_back(Name_temp.at(param_num));//填写参数表
		}

		Name_temp.clear();

		if (token.name == ";")
		{
			NextToken();
			PARGUMENT();
		}
	}
	else return;
}

//子程序return类型
void SUBP_RETURN()
{
	if (token.name == ":")
	{
		NextToken();
		Name_temp.push_back(SYNBL.at(search_SYNBL(SEM.top().name)));
		TYPE();
		if (Name_temp.size() != 1 || Name_temp.at(0).cat != 'f'){ cout << "暂存符号表没有清空！请立刻修改bug！行号为：" << line << endl; exit(0); }
		SYNBL.at(search_SYNBL(SEM.top().name)) = Name_temp.at(0);//填写函数的 返回类型 
		Name_temp.clear();
	}
	else return;
}

//子程序参数和return类型定义
void SUBPROGRAM_HEAD()
{
	if (token.name != "("){ cout << "子函数定义中函数名后缺\"（\" ！语法错误！行号为：" << line << endl; exit(0); }
	NextToken();
	ARGUMENT();
	PFINEL.at(SYNBL.at(search_SYNBL(SEM.top().name)).addr).fn = function_vn_num;//填写函数表的参数个数
	function_vn_num = 0;//参数个数置0
	if (token.name != ")"){ cout << "子函数定义中参数的\"（\"后缺\")\" ！语法错误！行号为：" << line << endl; exit(0); }
	NextToken();
	SUBP_RETURN();
}

//过程函数head
void PSUBPROGRAM_HEAD()
{
	if (token.name != "("){ cout << "子函数定义中函数名后缺\"（\" ！语法错误！行号为：" << line << endl; exit(0); }
	NextToken();
	PARGUMENT();
	PFINEL.at(SYNBL.at(search_SYNBL(SEM.top().name)).addr).fn = function_vn_num;//填写函数表的参数个数
	function_vn_num = 0;//参数个数置0
	if (token.name != ")"){ cout << "子函数定义中参数的\"（\"后缺\")\" ！语法错误！行号为：" << line << endl; exit(0); }
	NextToken();
}

//子程序定义
void SUBPROGRAM_DECLARATION()
{
	SUBPROGRAM_HEAD();
	DECLARATION();
	COM_SENTENCE();
}

//过程函数
void PSUBPROGRAM_DECLARATION()
{
	PSUBPROGRAM_HEAD();
	DECLARATION();
	COM_SENTENCE();
}

//结构体定义类型
void RECORD_TYPE()
{
	if (token.name == "integer")
	{
		for (int i = 0; i < Name_temp.size(); i++)
		{
			Name_temp.at(i).cat = 'd';
			Name_temp.at(i).type = 0;
			Name_temp.at(i).addr = LENL.size();
			LENL.push_back(4);
		}
		NextToken();
	}
	else if (token.name == "real")
	{
		for (int i = 0; i < Name_temp.size(); i++)
		{
			Name_temp.at(i).cat = 'd';
			Name_temp.at(i).type = 1;
			Name_temp.at(i).addr = LENL.size();
			LENL.push_back(8);
		}
		NextToken();
	}
	else if (token.name == "char")
	{
		for (int i = 0; i < Name_temp.size(); i++)
		{
			Name_temp.at(i).cat = 'd';
			Name_temp.at(i).type = 2;
			Name_temp.at(i).addr = LENL.size();
			LENL.push_back(1);
		}
		NextToken();
	}
	else if (token.name == "bool")
	{
		for (int i = 0; i < Name_temp.size(); i++)
		{
			Name_temp.at(i).cat = 'd';
			Name_temp.at(i).type = 3;
			Name_temp.at(i).addr = LENL.size();
			LENL.push_back(1);
		}
		NextToken();
	}
	else if (token.type == 2)
	{//填写类型信息
		if (search_SYNBL(token.name) != -1 && SYNBL.at(search_SYNBL(token.name)).cat == 't')
		{//填写类型信息
			for (int i = 0; i < Name_temp.size(); i++)
			{
				Name_temp.at(i).cat = 'd';
				Name_temp.at(i).type = SYNBL.at(search_SYNBL(token.name)).type;
				Name_temp.at(i).addr = LENL.size();
				LENL.push_back(LENL.at(SYNBL.at(search_SYNBL(token.name)).addr));
			}
		}
		else { cout << "变量类型错误！语法错误！行号为：" << line << endl; exit(0); }
		NextToken();
	}
	else if (token.name == "array")
	{
		NextToken();
		if (token.name != "["){ cout << "数组定义缺界符\"[\"！语法错误！行号为：" << line << endl; exit(0); }
		NextToken();
		if (token.type != 5){ cout << "数组定义，缺数组范围！语法错误！行号为：" << line << endl; exit(0); }
		//填写符号表类型表数组表

		string low = "\0", up = "\0";
		int subscript;
		for (subscript = 0; token.name[subscript] != '.'; subscript++)low += token.name[subscript];
		up = token.name.erase(0, subscript + 2);//数组的上界，对token进行理论修改！
		up = token.name;
		//for (subscript += 2; token.name[subscript] != '.'; subscript++)up += token.name[subscript];
		//填写符号表类型表数组表未填
		for (int i = 0; i < Name_temp.size(); i++)
		{
			Name_temp.at(i).cat = 'd';
			Name_temp.at(i).type = TYPEL.size();
			Name_temp.at(i).addr = LENL.size();
			//TYPEL_temp.push_back({ 'a', AINEL.size() });
			//Name_temp.at(i).addr = INTEGER.size();
		}
		TYPEL.push_back({ 'a', AINEL.size() });
		AINEL.push_back({ stoi(low), stoi(up), -1, 0 });//数组表的ctp和clen初始为-1和0 未填
		LENL.push_back(0);//长度表的初值为0  未填

		NextToken();
		if (token.name != "]"){ cout << "数组定义缺界符\"]\"！语法错误！行号为：" << line << endl; exit(0); }
		NextToken();
		if (token.name != "of"){ cout << "数组定义缺关键字\"of\"！语法错误！行号为：" << line << endl; exit(0); }
		NextToken();

		ARRAY_TYPE();
		//填写数组表类型信息
		LENL.back() = AINEL.at(TYPEL.at(Name_temp.at(0).type).tpoint).clen * (AINEL.at(TYPEL.at(Name_temp.at(0).type).tpoint).up - AINEL.at(TYPEL.at(Name_temp.at(0).type).tpoint).low + 1);
	}
	else { cout << "变量定义缺类型！语法错误！行号为：" << line << endl; exit(0); }
}

//类型定义列表
void TYPE_DECLARATION()
{
	ID_LIST();
	if (token.name == ":")
	{
		NextToken();
		RECORD_TYPE();
		//插入语义动作，结构体成员填入符号表
		
		for (int i = 0; i < Name_temp.size(); i++)
		{
			SYNBL.push_back(Name_temp.at(i));
			RINFL.push_back({ Name_temp.at(i).name, rinfl_off, Name_temp.at(i).type });
			rinfl_off += LENL.at(Name_temp.at(i).addr);
		}
		
		//if (i < 0) { cout << "找不到结构体指向的长度表！行号为：" << line << endl; exit(0); }

		Name_temp.clear();

		if (token.name == ";")
		{
			NextToken();
			if (token.type == 2)TYPE_DECLARATION();
			else return;
		}
		else { cout << "结构体定义中缺“;”！语法错误！行号为：" << line << endl; exit(0); }
	}
	else { cout << "结构体定义中标识符后缺“:”！语法错误！行号为：" << line << endl; exit(0); }
}

//类型定义
void DECLARATION_TYPE()
{
	if (token.name == "record")
	{
		NextToken();
		SYNBL.back().type = TYPEL.size();
		TYPEL.push_back({ 'd', RINFL.size() });//填写结构体的类型表
		
		TYPE_DECLARATION();

		int i = LENL.size() - 1;
		for (; i >= 0; i--)
		{//填写结构体的长度表
			if (LENL.at(i) == -1)
				LENL.at(i) = rinfl_off;
		}
		rinfl_off = 0;//结构表长度暂存清0
		if (token.name == "end")
		{//填写结构体符号表未填
			NextToken();
			if (token.name == ";"){ NextToken(); return; }
			else { cout << "结构体定义end后缺“;”！语法错误！行号为：" << line << endl; exit(0); }
		}
		else { cout << "结构体定义缺“end”！语法错误！行号为：" << line << endl; exit(0); }
	}
	else
	{//类型更名
		//TYPEL.push_back({ 'd', RINFL.size() });//填写结构体的类型表

		DETYPE();//类型定义不正确！未填写类型表

		if (token.name == ";"){ NextToken(); return; }
		else { cout << "类型定义缺“;”！语法错误！行号为：" << line << endl; exit(0); }
	}
}

//常量定义函数
void CONST_DECLARATION()
{
	if (token.type != 2){ cout << "语法错误！常量定义缺标识符!行号为：" << line << endl; exit(0); }
	if (search_SYNBL(token.name) != -1){ cout << "重定义！行号为：" << line << endl; exit(0); }
	string const_name = token.name;

	//CONST_QT.push_back({ {":=", "\0", "_", token.name}, Level });//暂存常量定义四元式，常量还没有放进去

	NextToken();
	if (token.name == "=")
	{
		NextToken();

		if (token.type == 3 || token.type == 4 )//填写符号表
		{//整数实数
			SYNBL.push_back({ const_name, token.type - 3, 'c', CONSL.size(), Level });
			CONSL.push_back({ token.type, token.name });
		}
		else if (token.type == 6)
		{//字符字符串
			SYNBL.push_back({ const_name, 2, 'c', CONSL.size(), Level });
			CONSL.push_back({ token.type, token.name });
		}
		else if (token.type == 7)
		{//字符串
			SYNBL.push_back({ const_name, 4, 'c', CONSL.size(), Level });
			CONSL.push_back({ token.type, token.name });
		}
		else { cout << "错误！常量定义缺少常量！行号为：" << line << endl; exit(0); }
		
		NextToken();
		if (token.name == ";")
		{
			NextToken();
			if (token.type == 2)CONST_DECLARATION();
		}
		else { cout << "语法错误！常量定义缺少\";\"！行号为：" << line << endl; exit(0); }
	}
	else { cout << "语法错误！常量定义缺少\"=\"! 行号为：" << line << endl; exit(0); }
}

//填写符号表
void Type_Write_SYNBL()
{
	for (int i = 0; i < Name_temp.size(); i++)
	{
		if (TYPEL.at(Name_temp.at(i).type).tval == 'i')
		{
			Name_temp.at(i).addr = INTEGER.size();
			INTEGER.push_back(0);//变量初始化为0
			SYNBL.push_back(Name_temp.at(i));
		}
		else if (TYPEL.at(Name_temp.at(i).type).tval == 'r')
		{
			Name_temp.at(i).addr = REAL.size();
			REAL.push_back(0.0);//变量初始化为0.0
			SYNBL.push_back(Name_temp.at(i));
		}
		else if (TYPEL.at(Name_temp.at(i).type).tval == 'c')
		{
			Name_temp.at(i).addr = CHAR.size();
			CHAR.push_back('\0');//变量初始化为'\0'
			SYNBL.push_back(Name_temp.at(i));
		}
		else if (TYPEL.at(Name_temp.at(i).type).tval == 'b')
		{
			Name_temp.at(i).addr = BOOL.size();
			BOOL.push_back(true);//变量初始化为true
			SYNBL.push_back(Name_temp.at(i));
		}
		else if (TYPEL.at(Name_temp.at(i).type).tval == 'a')
		{
			SYNBL.push_back(Name_temp.at(i));
		}
		else if (TYPEL.at(Name_temp.at(i).type).tval == 'd')
		{//定义结构体类型变量不再填写长度表
			/*Name_temp.at(i).addr = LENL.size();

			int j = 0;
			for (; j < SYNBL.size(); j++)
			{
				if (SYNBL.at(j).type == Name_temp.at(i).type)
				{
					LENL.push_back(LENL.at(SYNBL.at(j).addr));
					break;
				}
			}

			//LENL.push_back(0);//结构体长度表未填*/
			SYNBL.push_back(Name_temp.at(i));
		}
		else { cout << "类型表中类型有误！行号为：" << line << endl; exit(0); }
	}
	//Name_temp.clear();
}

//变量定义函数
void VAR_DECLARATION()
{
	ID_LIST();
	if (token.name == ":")
	{
		NextToken();
		TYPE();
		//填写符号表
		for (int i = 0; i < Name_temp.size(); i++)
		{
			Name_temp.at(i).cat = 'v';
		}

		Type_Write_SYNBL();//填写符号表
		Name_temp.clear();
		if (token.name == ";")
		{
			NextToken();
			if(token.type == 2)VAR_DECLARATION();
		}
		else { cout << "语法错误！变量定义缺少\";\"！行号为：" << line << endl; exit(0); }
	}
	else { cout << "语法错误！变量定义缺少\":\"! 行号为：" << line << endl; exit(0);}
}

//变量定义及类型声明
void DECLARATION()
{
	if (token.name == "type")
	{//结构体，符号表未填
		NextToken();
		if (token.type == 2)
		{
			if (search_SYNBL(token.name) == -1)
			{
				//将类型填入符号表

				//Name_temp.push_back(token.name);
				SYNBL.push_back({ token.name, -1, 't', LENL.size(), Level });//填写类型的符号表
				//LENL.push_back(-1);
				//当前结构体长度初始为0
				//TYPEL.push_back({ 'd', AINEL.size() });
				//AINEL.push_back();

				NextToken();
				if (token.name == "=")
				{
					NextToken();
					DECLARATION_TYPE();

					DECLARATION();
				
				}
				else { cout << "语法错误！type定义错误！行号为：" << line << endl; exit(0); }
			}
			else { cout << "变量重定义！行号为：" << line << endl; exit(0); }
		}
		else { cout << "类型声明缺少标识符！行号为：" << line << endl; exit(0); }
	}
	else if (token.name == "var")
	{//变量定义，符号表
		NextToken();

		VAR_DECLARATION();
		DECLARATION();
	}
	else if (token.name == "const")
	{//常量定义，符号表

		NextToken();

		CONST_DECLARATION();
		DECLARATION();
	}
	else if (token.name == "function")
	{//函数定义，符号表未填
		NextToken();
		if (token.type == 2)
		{
			Level++;//函数层次加一
			SEM.push(token);//压栈
			SYNBL.push_back({ token.name, -1, 'f', PFINEL.size(), Level });//类型type即返回值未填
			PFINEL.push_back({ Level, 3, 0, token.name, PARAMBL.size() });//参数个数未填
			
			QT.push_back({ "function", token.name, "_", "_" });

			NextToken();
			SUBPROGRAM_DECLARATION();
			if (token.name != ";"){ cout << "子函数定义缺“；”！行号为：" << line << endl; exit(0); }
			NextToken();
			Level--;//函数层次减一
		}
		else { cout << "函数定义缺少标识符！行号为：" << line << endl; exit(0); }
	}
	else if (token.name == "procedure")
	{//过程函数定义，符号表未填
		NextToken();
		if (token.type == 2)
		{
			Level++;//函数层次加一
			SEM.push(token);//压栈
			SYNBL.push_back({ token.name, -1, 'p', PFINEL.size(), Level });//类型type即返回值未填
			PFINEL.push_back({ Level, 3, 0, token.name, PARAMBL.size() });//参数个数未填

			QT.push_back({ "procedure", token.name, "_", "_" });
			//PROCE_QT.push_back({ "procedure", token.name, "_", "_" });

			NextToken();
			PSUBPROGRAM_DECLARATION();
			int k = 0;
			for (; k < QT.size();k++)
			{
				if (QT.at(k).w == "procedure")break;
			}
			k++;
			for (; k < QT.size() - 1; k++)
			{
				PROCE_QT.push_back(QT.at(k));
			}

			if (token.name != ";"){ cout << "子函数定义缺“；”！行号为：" << line << endl; exit(0); }
			NextToken();
			Level--;//函数层次减一
		}
		else { cout << "函数定义缺少标识符！行号为：" << line << endl; exit(0); }
	}
	else return;
}

//主函数体
void SUB_PROGRAM()
{
	DECLARATION();
	COM_SENTENCE();
}

//主函数
void PROGRAM()
{
	NextToken();
	if (token.name == "program")
	{
		NextToken();
		if (token.type == 2)
		{
			SEM.push(token);
			//string program_name = token.name;
			//将当前函数名填入符号表
			SYNBL.push_back({token.name, -1, 'p', -1, Level}); 
			//将函数声明压入四元式区
			QT.push_back({ "program", token.name, "_", "_" });
			//cout << "(" << QT.back().w << "," << QT.back().o1 << "," << QT.back().o2 << "," << QT.back().t << ")" << endl;

			NextToken();
			SUB_PROGRAM();
			if (token.name == "." || fileStream.endFlag == 1)
			{ 
				//判断语义栈SEM栈空
				if (!SEM.empty()){ cout << "语义栈SEM不空！" << endl; exit(0); }

				//cout << "语法正确！" << endl; 
				return; 
			}
			else { cout << "缺“.”！语法错误！行号为：" << line  << endl; exit(0); }
		}
		else { cout << "缺标识符！语法错误！行号为：" << line  << endl; exit(0); }
	}
	else { cout << "缺“program”！语法错误！行号为：" << line  << endl; exit(0); }
}

//分配地址输出？
void outspace(int i)
{
	for (; i > 1; i--)
		cout << "?,";
}

//在目标代码中分配地址
void var_address()
{
	int	lenght = 0;
	for (Synbl_item & s : SYNBL)
	{
		if (s.cat == 'v')
		{
			if (TYPEL.at(s.type).tval == 'i')
				cout << s.name << "    DD    ?" << endl;
			else if (TYPEL.at(s.type).tval == 'r')
				cout << s.name << "    DD    ?,?" << endl;
			else if (TYPEL.at(s.type).tval == 'b')
				cout << s.name << "    DB    ?" << endl;
			else if (TYPEL.at(s.type).tval == 'c')
				cout << s.name << "    DB    ?" << endl;
			else if (TYPEL.at(s.type).tval == 'a')
			{
				cout << s.name << "    DB    ";
				outspace(LENL.at(s.addr));
				cout << "?" << endl;
			}
			else if (TYPEL.at(s.type).tval == 'd')
			{
				cout << s.name << "    DB    ";
				outspace(LENL.at(s.addr));
				cout << "?" << endl;
			}
		}
		else if (s.cat == 'c')
		{
			if (TYPEL.at(s.type).tval == 'i')
				cout << s.name << "    DD    " << CONSL.at(s.addr).con << endl;
			else if (TYPEL.at(s.type).tval == 'r')
				cout << s.name << "    DD    " << CONSL.at(s.addr).con << endl;
			else if (TYPEL.at(s.type).tval == 'b')
				cout << s.name << "    DB    " << CONSL.at(s.addr).con << endl;
			else if (TYPEL.at(s.type).tval == 'c')
				cout << s.name << "    DB    " << CONSL.at(s.addr).con << endl;
		}
	}
}
