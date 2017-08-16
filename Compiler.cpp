#include "Table.h"
extern FileStream fileStream;
extern Token token;

extern vector<Token>TokenList;//token序列
extern vector<QT_item> QT;//四元式存储区
extern vector<QT_item>QT_1;//优化后的四元式存储区
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

void show_success()
{
	system("cls");
	cout << "\n\n\n\n\n\n\n\n\n\n\n\n";
	cout << "\t\t\t\t     编译成功！" << endl << endl;
	cout << "\t\t\t敲击回车键进入编译结果查看页面" << endl;
	system("pause");
	show_main();
	system("pause");
}

void show0()
{
	system("cls");
	cout << "********************************************************************************\n";
	cout << "******                           四元式(" << QT.size() << "条)                             ******\n";
	cout << "\n\n";

	for (QT_item & qt : QT)
		cout << "\t\t\t    (" << qt.w << "," << qt.o1 << "," << qt.o2 << "," << qt.t << ")" << endl;

	cout << endl;

	
	cout << "********************************************************************************\n";
	cout << "******                       优化后的四元式(" << QT_1.size() << "条)                         ******\n";
	cout << "\n\n";

	for (QT_item & qt : QT_1)
		cout << "\t\t\t    (" << qt.w << "," << qt.o1 << "," << qt.o2 << "," << qt.t << ")" << endl;

	cout << endl;

	system("pause");
}

void show1()
{
	system("cls");
	cout << "********************************************************************************\n";
	cout << "******                                Token                              ******\n";
	cout << "\n\n";
	cout << "\t\t\t\t" << "名字" << '\t' << "类码" << endl;
	for (int j = 0; j < TokenList.size(); j++)
	{
		cout << "\t\t\t\t" << TokenList.at(j).name << "\t" << TokenList.at(j).type << endl;
	}
	cout << endl;
	system("pause");
}

void show2()
{
	system("cls");
	cout << "********************************************************************************\n";
	cout << "******                              符号表(" << SYNBL.size() << "条)                          ******\n";
	cout << "\n\n";
	cout << "\t\t    " << "序号" << '\t' << "名字" << '\t' << "类型" << '\t' << "种类" << '\t' << "addr" << '\t' << "层次" << '\t' << endl;
	for (int j = 0; j < SYNBL.size(); j++)
	{
		cout << "\t\t    " << j << "\t\t" << SYNBL.at(j).name << '\t' << SYNBL.at(j).type << '\t' << SYNBL.at(j).cat << '\t' << SYNBL.at(j).addr << '\t' << SYNBL.at(j).level << '\t' << endl;
	}
	cout << endl;
	system("pause");
}

void show3()
{
	system("cls");
	cout << "********************************************************************************\n";
	cout << "******                              类型表(" << TYPEL.size() << "条)                          ******\n";
	cout << "\n\n";
	cout << "\t\t\t " << "序号" << '\t' << "类型" << '\t' << "类型位置" << endl;
	for (int j = 0; j < TYPEL.size(); j++)
	{
		cout << "\t\t\t" << j << '\t' << TYPEL.at(j).tval << '\t' << TYPEL.at(j).tpoint << endl;
	}
	cout << endl;
	system("pause");
}

void show4()
{
	system("cls");
	cout << "********************************************************************************\n";
	cout << "******                              数组表(" << AINEL.size() << "条)                           ******\n";
	cout << "\n\n";
	cout << "\t\t" << "序号" << '\t' << "下界" << '\t' << "上界" << '\t' << "类型" << '\t' << "成分长度" << '\t' << endl;
	for (int j = 0; j < AINEL.size(); j++)
	{
		cout << "\t\t" << j << '\t' << AINEL.at(j).low << '\t' << AINEL.at(j).up << '\t' << AINEL.at(j).ctp << '\t' << AINEL.at(j).clen << endl;
	}
	cout << endl;
	system("pause");
}

void show5()
{
	system("cls");
	cout << "********************************************************************************\n";
	cout << "******                              结构表(" << RINFL.size() << "条)                           ******\n";
	cout << "\n\n";
	cout << "\t\t\t" << "序号" << '\t' << "域名" << '\t' << "偏移量" << '\t' << "类型" << endl;
	for (int j = 0; j < RINFL.size(); j++)
	{
		cout << "\t\t\t" << j << '\t' << RINFL.at(j).id << '\t' << RINFL.at(j).off << '\t' << RINFL.at(j).tp << endl;
	}
	cout << endl;
	system("pause");
}

void show6()
{
	system("cls");
	cout << "********************************************************************************\n";
	cout << "******                              函数表(" << PFINEL.size() << "条)                           ******\n";
	cout << "\n\n";
	cout << "\t" << "层次号" << '\t' << "区距" << '\t' << "参数个数" << '\t' << "入口地址" << '\t' << "参数起始位置" << endl;
	for (int j = 0; j < PFINEL.size(); j++)
	{
		cout << "\t" << PFINEL.at(j).level << '\t' << PFINEL.at(j).off << '\t' << PFINEL.at(j).fn << '\t' << '\t' << PFINEL.at(j).entry << '\t' << '\t' << PFINEL.at(j).param << endl;
	}
	cout << endl;
	system("pause");
}

void show7()
{
	system("cls");
	cout << "********************************************************************************\n";
	cout << "******                              参数表(" << PARAMBL.size() << "条)                           ******\n";
	cout << "\n\n";
	cout << "\t\t    " << "序号" << '\t' << "名字" << '\t' << "类型" << '\t' << "种类" << '\t' << "addr" << '\t' << "层次" << '\t' << endl;
	for (int j = 0; j < PARAMBL.size(); j++)
	{
		cout << "\t\t    " << j << "\t\t" << PARAMBL.at(j).name << '\t' << PARAMBL.at(j).type << '\t' << PARAMBL.at(j).cat << '\t' << PARAMBL.at(j).addr << '\t' << PARAMBL.at(j).level << '\t' << endl;
	}
	cout << endl;
	system("pause");
}

void show8()
{
	system("cls");
	cout << "********************************************************************************\n";
	cout << "******                                长度表(" << LENL.size() << "条)                        ******\n";
	cout << "\n\n";
	cout << "\t\t\t\t" << "序号" << '\t' << "长度" << endl;
	for (int j = 0; j < LENL.size(); j++)
	{
		cout << "\t\t\t\t" << j << '\t' << LENL.at(j) << endl;
	}
	cout << endl;
	system("pause");
}

void show9()
{
	system("cls");
	cout << "********************************************************************************\n";
	cout << "******                               常数表                               ******\n";
	cout << "\n\n";

	cout << "\t\t\t\t" << "整数表(" << INTEGER.size() << "条)\n";
	cout << "\t\t\t\t" << "序号" << '\t' << "内容" << endl;
	int j = 0;
	for (j = 0; j < INTEGER.size(); j++)
	{
		cout << "\t\t\t\t" << j << '\t' << INTEGER.at(j) << endl;
	}
	cout << endl;

	cout << "\t\t\t\t" << "实数表(" << REAL.size() << "条)\n";
	cout << "\t\t\t\t" << "序号" << '\t' << "内容" << endl;
	for (j = 0; j < REAL.size(); j++)
	{
		cout << "\t\t\t\t" << j << '\t' << REAL.at(j) << endl;
	}
	cout << endl;

	cout << "\t\t\t\t" << "字符表(" << CHAR.size() << "条)\n";
	cout << "\t\t\t\t" << "序号" << '\t' << "内容" << endl;
	for (j = 0; j < CHAR.size(); j++)
	{
		cout << "\t\t\t\t" << j << '\t' << CHAR.at(j) << endl;
	}
	cout << endl;

	cout << "\t\t\t\t" << "布尔表(" << BOOL.size() << "条)\n";
	cout << "\t\t\t\t" << "序号" << '\t' << "内容" << endl;
	for (j = 0; j < BOOL.size(); j++)
	{
		cout << "\t\t\t\t" << j << '\t' << BOOL.at(j) << endl;
	}
	cout << endl;

	cout << "\t\t\t\t" << "常量表(" << CONSL.size() << "条)\n";
	cout << "\t\t\t\t" << "序号" << '\t' << "内容" << '\t' << "种类" << endl;
	for (j = 0; j < CONSL.size(); j++)
	{
		cout << "\t\t\t\t" << j << '\t' << CONSL.at(j).con << '\t' << CONSL.at(j).type << endl;
	}
	cout << endl;
	system("pause");
}

void showa()
{
	system("cls");
	cout << "********************************************************************************\n";
	cout << "******                         优化后的四元式(" << QT_1.size() << "条)                    ******\n";
	cout << "\n\n";

	for (QT_item & qt : QT_1)
		cout << "\t\t\t    (" << qt.w << "," << qt.o1 << "," << qt.o2 << "," << qt.t << ")" << endl;

	cout << endl;
	system("pause");
}

extern int cur_obj_i;
extern OBJ obj[100];                            //目标代码区
void showb()
{
	system("cls");
	
	cout << "----------------目标代码区------------------" << endl;
	cout << "DSEG         SEGEMENT" << endl;
	var_address();
	cout << "DSEG         ENDS\nCSEG SEGMENT" << endl;
	cout << "         ASSUME     CS:CSEG,DS:DSEG" << endl;
	cout << "STARTS:" << endl;
	for (int i = 0; i<cur_obj_i; i++)
	{
		cout << "       " << i << ": " << obj[i].op << "  " << obj[i].R << "，" << obj[i].M << endl;
	}
	cout << "       MOV   AX,4CH\n       INT 21H\nCSEG ENDS\n       END   STARTS" << endl;

	cout << endl;
	system("pause");
}

void show_main()
{
	char cmd;
	while (1)
	{
		system("cls");
		//cout << "\n\n";
		cout << "********************************************************************************\n";
		cout << "******                                编译器                              ******\n";
		cout << "\n\n\n";
		cout << "                                 结果展示：\n";
		cout << "                           ______________________\n\n";
		cout << "                               1: Token序列\n";
		cout << "                               2: 符号表\n";
		cout << "                               3: 类型表\n";
		cout << "                               4: 数组表\n";
		cout << "                               5: 结构表\n";
		cout << "                               6: 函数表\n";
		cout << "                               7: 参数表\n";
		cout << "                               8: 长度表\n";
		cout << "                               9: 常量表\n";
		cout << "                               0: 四元式\n";
		cout << "                               A: 优化后的四元式\n";
		cout << "                               B: 目标代码生成\n";
		cout << "\n\n";
		cout << "                     请选择命令0—B:___\b\b";
		fflush(stdin);
		cmd = getc(stdin);
		switch (cmd)
		{
		case '0':show0(); break;
		case '1': show1(); break;
		case '2': show2(); break;
		case '3': show3(); break;
		case '4': show4(); break;
		case '5': show5(); break;
		case '6': show6(); break;
		case '7': show7(); break;
		case '8': show8(); break;
		case '9': show9(); break;
		case 'A':
		case 'a': showa(); break;
		case 'b':
		case 'B': showb(); break;
		case 'c':
		case 'C': return;
		default:
			break;
		}
	}
}

int main()
{
	system("color F9");
	system("cls");
	cout << "\n\n\n";
	cout << "\t    编译结果信息：" << endl << endl;
	cout << "\t    ";

	CopyFile();//读取文件
	PROGRAM();//词法语法语义

	optimize();//中间代码优化

	Active();//活跃信息
	Obj_produce();//目标代码生成


	show_success();//结果展示

	system("pause");

	return 0;
}