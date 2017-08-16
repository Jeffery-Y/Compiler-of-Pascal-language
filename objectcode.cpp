#include"Table.h"

extern vector<Token>TokenList;

extern vector<QT_item> QT_1;//四元式存储区
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
#define N 100                 //N是四元式空间的预分配大小
int FJ_IF = 0, JMP_IF = 0, FJ_DO = 0, JMP_WH = 0;                      //当前四元式、函数四元式、目标代码 数组中被处理元素的下标
int cur_qt_i, cur_func_i = 0, cur_obj_i = 0;

int block[10];

OBJ obj[N];                            //目标代码区
string RDL = "0";                //寄存器状态清零

struct ACT      //存活跃信息
{
	string name;
	int L;//活跃信息
};
ACT act[N];

void DivideBlock()   //四元式分块函数
{
	int bar;
	block[0] = 0;
	int bl = 1;
	for (bar = 0; bar < QT_1.size(); bar++)
	{
		while (QT_1.at(bar).w == "if" || QT_1.at(bar).w == "else" || QT_1.at(bar).w == "ifend" || QT_1.at(bar).w == "while" || QT_1.at(bar).w == "whileend");
		block[bl] = bar;    //记录每块的尾地址
		bl++;
	}
	block[bl] = QT_1.size();
	//cout << block[bl] << QT_1.size() << endl;
}

void Active()    //构建活跃性息函数  0表示不活跃n，1表示活跃y
{
	int bar = 0;
	int bar2 = 0;
	//SYNBL.at(search_SYNBL(QT_1.at(bar).o1)).L = 1;
	//SYNBL.at(search_SYNBL(QT_1.at(bar).o2)).L  = 1;
	char comp;
	for (bar = 0; bar < QT_1.size(); bar++)
	{
		comp = QT_1.at(bar).t[0];
		if (comp == 't')
			act[bar].L = 0;
		else
			act[bar].L = 1;
	}                //初始化




	int start = 0;
	int start2 = 0;
	for (bar = start; bar < QT_1.size(); bar++)
	{
		for (bar2 = start2 + 1; bar2 < QT_1.size(); bar2++)
		{
			if (QT_1.at(bar).t == QT_1.at(bar2).t || QT_1.at(bar).t == QT_1.at(bar2).o1 || QT_1.at(bar).t == QT_1.at(bar2).o2)
			{
				act[bar].L = 1;
				//	else if (QT_1.at(bar).o2 = QT_1.at(bar2).t || QT_1.at(bar).o2 = QT_1.at(bar2).o1 || QT_1.at(bar).o2 = QT_1.at(bar2).o2)
				//		SYNBL.at(search_SYNBL(QT_1.at(bar).o2)).L  = 1;
				//	else if (QT_1.at(bar).o1 = QT_1.at(bar2).t || QT_1.at(bar).o1 = QT_1.at(bar2).o1 || QT_1.at(bar).o1 = QT_1.at(bar2).o2)
				//		SYNBL.at(search_SYNBL(QT_1.at(bar).t)).L = 1;
			}
			start2++;

		}
		start2 = start + 1;
		start++;
	}

	//	cout << QT_1.size() << act[4].L<< endl;
}

void obj_send(string op, string R, string M)    //目标代码发送
{
	obj[cur_obj_i].op = op;
	obj[cur_obj_i].R = R;
	obj[cur_obj_i].M = M;
	cur_obj_i++;
}

void Obj_produce()      //目标代码生成函数
{
	int search_i;
	int num = 0;
	cur_qt_i = QT_1.size();
	
	for (search_i = 0; search_i < QT_1.size(); search_i++)              //对四元式块进行遍历
	{
		if (QT_1.at(search_i).w == "+" || QT_1.at(search_i).w == "*")
		{
			if (RDL != "\0")
			if (act[search_i].L = 1 && QT_1.at(search_i).t != QT_1.at(search_i + 1).o1 &&QT_1.at(search_i).t != QT_1.at(search_i + 1).o2)
			{
				obj_send("ST", "R", RDL);
				obj_send("MOV", "R", QT_1.at(search_i).o1);
			}
			if (QT_1.at(search_i).w == "+")        obj_send("ADD", "R", QT_1.at(search_i).o2);
			else if (QT_1.at(search_i).w == "*")   obj_send("MUL", "R", QT_1.at(search_i).o2);
			RDL = QT_1.at(search_i).t;
		}

		if (QT_1.at(search_i).w == "-" || QT_1.at(search_i).w == "/")
		{
			if (RDL != "0")
			if (act[search_i].L = 1 && QT_1.at(search_i).t != QT_1.at(search_i + 1).o1)
				obj_send("ST", "R", RDL);
			obj_send("MOV", "R", QT_1.at(search_i).o1);
			if (QT_1.at(search_i).w == "-")   obj_send("SUB", "R", QT_1.at(search_i).o2);
			else if (QT_1.at(search_i).w == "/")   obj_send("DIV", "R", QT_1.at(search_i).o2);
			RDL = QT_1.at(search_i).t;
		}
		else if (QT_1.at(search_i).w == ":=")
		{
			if (search_SYNBL(QT_1.at(search_i).t) != -1)
			{
				if (SYNBL.at(search_SYNBL(QT_1.at(search_i).t)).cat == 'c')
				{
					search_i++;
					continue;
				}
			}
			//if (RDL != "0")
			/*if (act[search_i].L = 1 && QT_1.at(search_i).t == QT_1.at(search_i + 1).o1)
				obj_send("ST", "R", RDL);*/
			obj_send("MOV", "R", QT_1.at(search_i).o1);
			obj_send("ST", "R", QT_1.at(search_i).t);
			RDL = QT_1.at(search_i).t;
		}
		else if (QT_1.at(search_i).w == "<" || QT_1.at(search_i).w == ">" || QT_1.at(search_i).w == "==")
		{
			if (RDL != "\0")
				obj_send("ST", "R", RDL);
			obj_send("MOV", "R", QT_1.at(search_i).o1);
			if (QT_1.at(search_i).w == "<")  obj_send("LT", "R", QT_1.at(search_i).o2);
			if (QT_1.at(search_i).w == ">")  obj_send("GT", "R", QT_1.at(search_i).o2);
			if (QT_1.at(search_i).w == "==") obj_send("EQ", "R", QT_1.at(search_i).o2);
			RDL = QT_1.at(search_i).t;
		}

		if (QT_1.at(search_i).w == "if")
		{
			FJ_IF = cur_obj_i;
			obj_send("FJ", "R", "\0");                  //等待回填
			continue;
		}
		if (QT_1.at(search_i).w == "else")
		{
			if (RDL != "\0")
				obj_send("ST", "R", RDL);
			JMP_IF = cur_obj_i;
			obj_send("JMP", "\0", "\0");               //等待回填
			string buffer_M;
			int p = 0;  char q;
			p = cur_obj_i / 10;
			q = p + '0';
			buffer_M += q;
			p = cur_obj_i % 10;
			q = '0' + p;
			buffer_M += q;
			obj[FJ_IF].R = buffer_M;
			continue;
		}
		if (QT_1.at(search_i).w == "ifend")                 //等待回填
		{
			string buffer_M;
			int p = 0;  char q;
			p = cur_obj_i / 10;
			q = p + '0';
			buffer_M += q;
			p = cur_obj_i % 10;
			q = '0' + p;
			buffer_M += q;
			obj[JMP_IF].R = buffer_M;
			continue;
		}
		if (QT_1.at(search_i).w == "while")
		{
			JMP_WH = cur_obj_i;
			obj_send("FJ", "R", "\0");                 //等待回填
			continue;
		}
		if (QT_1.at(search_i).w == "do")
		{
			FJ_DO = cur_obj_i;
		}
		if (QT_1.at(search_i).w == "whileend")
		{
			if (RDL != "\0")
				obj_send("ST", "R", RDL);
			string buffer_M;
			int p = 0;  char q;
			p = cur_obj_i / 10;
			q = p + '0';
			buffer_M += q;
			p = cur_obj_i % 10;
			q = '0' + p;
			buffer_M += q;
			obj[JMP_WH].R = buffer_M;
			continue;
		}
	}
	for (int i = 0; i < cur_obj_i; i++)
	{
		if (obj[i].M[0] == '\'')       //字符类型处理
		{
			int exchange;
			exchange = obj[i].M[1];
			obj[i].M = to_string(exchange);
		}
	}
}

void Out_obj()
{
	cout << "----------------目标代码区------------------" << endl;
	cout << "DSEG    SEGEMENT" << endl;
	var_address();
	cout << "DSEG    ENDS\nCSEG SEGMENT" << endl;
	cout << "        ASSUME     CS:CSEG,DS:DSEG" << endl;
	cout << "STARTS:" << endl;
	for (int i = 0; i<cur_obj_i; i++)
	{
		cout << "       " << i << ": " << obj[i].op << "  " << obj[i].R << "   " << obj[i].M << endl;
	}
	cout << "       MOV   R,4CH\n       INT 21H\n    CSEG ENDS\n       END   STARTS" << endl;
	cout << "代码输出完毕！\n" << endl;
}