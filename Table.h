#ifndef LEXICAL
#define LEXICAL

#include <fstream>
#include <iostream>
#include <stack>
#include <array>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

struct FileStream
{//暂存文本文件中的源代码
	long offset;//读取位置
	long length;//代码长度
	bool endFlag;//结束标志
	string text;//源代码
};

struct Token
{
	int type;//Token的分类码
	string name;
	//bool L;//目标代码生成时用到的活跃信息
};


//===================词法分析函数区=====================
int Search_KeyWords(string str);//查找关键字
void CopyFile();//读取文本文件中的代码
int state_change(int state, char ch);//词法分析自动机
int state_to_type(int state_before, string str);//识别token类别码
void NextToken();//读取下一个token
//======================================================

//===================语法分析函数区=====================
/*void PROGRAM();
void SUB_PROGRAM();

void ID_LIST();
void TYPE();
void COM_SENTENCE();
void SEN_SEQUENCE();
void EVA_SENTENCE();
void EXPRESSION();
void TERM();
void FACTOR();
*/
//======================================================

struct Synbl_item//符号表项
{
	string name;
	int type;//TYPEL中的偏移量
	char cat;//种类编码：f,c,t,d,v
	int addr;//常数,结构,数组,函数时为各表中的偏移量;当为变量或参数时为活动记录中的偏移量
	int level;//层次号
};

struct Typel_item//类型表项
{
	char tval;//类型代码:i,r,c,b,a,d
	int tpoint;//AINEL或RINFL中的偏移量
};

struct Ainel_item//数组表项
{
	int low;//数组的下界
	int up;//数组的上界
	int ctp;//TYPEL中的偏移量
	int clen;//成分类型的长度
};

struct Rinfl_item//结构表项
{
	string id;//结构表域名
	int off;//当前结构首址的地址
	int tp;//TYPEL中的偏移量
};

struct Consl_item//常量表项
{
	int type;
	string con;
};

struct Pfinfl_item
{
	int level;//嵌套层次号
	int off;//区距
	int fn;//参数个数
	string entry;//入口地址
	int param;//参数表
};

struct XX_item//TPPTB
{
	int L;
	int offset;
};

//===================以下为符号表函数区=====================

int search_CHAR(char char_val);		//查找字符表，存在则返回offset，否则返回-1；
int search_INTEGER(long long_val);	//查找整数表，存在则返回offset，否则返回-1；
int search_REAL(double double_val);	//查找实数表，存在则返回offset，否则返回-1；
int search_KEYWL(string string_val);	//查找关键字表，存在则返回offset，否则返回-1；
int search_SYNBL(string string_val);  //查找符号表，存在则返回offset，否则返回-1；
int search_RINFL(string Struct_name, string item_name);//查找当前结构体中的项，存在则返回offset，否则返回-1；

//==========================================================

struct QT_item//四元式结构体
{
	string w;//算符
	string o1;//运算对象1
	string o2;//运算对象2
	string t;//结果
};

struct CONST_QT_item
{
	QT_item con_qt;//常量定义的四元式
	int level;//当前常量的层次
};

//===================语义分析函数区=====================

//主函数
void PROGRAM();
//主函数体
void SUB_PROGRAM();
//变量定义及类型声明
void DECLARATION();
//变量定义函数
void VAR_DECLARATION();
//填写符号表
void Type_Write_SYNBL();
//常量定义函数
void CONST_DECLARATION();
//类型定义
void DECLARATION_TYPE();
//类型定义列表
void TYPE_DECLARATION();
//结构体定义类型
void RECORD_TYPE();
//过程函数
void PSUBPROGRAM_DECLARATION();
//子程序定义
void SUBPROGRAM_DECLARATION();
//过程函数head
void PSUBPROGRAM_HEAD();
//子程序参数和return类型定义
void SUBPROGRAM_HEAD();
//子程序return类型
void SUBP_RETURN();
//子程序的参数
void ARGUMENT();
//过程函数的参数
void PARGUMENT();
//类型
void TYPE();
//声明类型
void DETYPE();
//数组类型
void ARRAY_TYPE();
//标识符列表
void ID_LIST();
//语句
void COM_SENTENCE();
//操作语句
void SEN_SEQUENCE();
//if语句
void IF_SENTENCE();
//while语句
void WHILE_SENTENCE();
//判断语句
void CONDITION();
//表达式语句后部分
void EVA_SUBPROGRAM_OPTION();
//函数调用参数
void FUNCTION_VN();
//过程函数
void PROCEDURE_VN();
//函数调用参数类型匹配
void PARAM_MATCH(string func, Token func_param, int vn_num);
//表达式
void EXPRESSION();
//项
void TERM();
//因子
void FACTOR();
//结构体数组函数因子
void ID_SID_OPTION();
//将临时变量填入符号表
void Write_T_SYN(Token t1, Token t2);
//返回标识符的类型码
int IT_Type(Token t);
//在目标代码中分配地址
void var_address();

//==========================================================


//===================以下为展示结果函数区=====================

void show_success();
void show_main();
void show0();
void show1();
void show2();
void show3();
void show4();
void show5();
void show6();
void show7();
void show8();
void show9();

//==========================================================


//===================以下为优化结构体区=====================

struct DagNode                      //dag有向图中的结点
{
	int fatherNum;					//是否有父节点
	bool isSingle;					//是否是单分支
	bool isLeaf;
	bool isVal;                     //是否是常数         
	double value;                   //值？
	string opt;                       //操作符
	int NodeNum;					//该节点的编号
	int RChild, LChild;				//左右子节点的编号
	int flagNum;					//依附于该节点的变量数
	string flags[20];				//依附于该节点的变量
	DagNode()
	{
		isVal = false;
		fatherNum = 0;
		flagNum = 0;
		isLeaf = false;
		isSingle = false;
	}

};

struct VarNode               //为每个变量给初值
{
	string str;
	int flagNode;            //所依附的结点编号
	VarNode()
	{
		flagNode = 0;
	}
};

int optimize();//后端优化

//==========================================================


//===================以下为目标代码区=====================

typedef struct OBJ{
	string op;
	string R;
	string M;
}OBJ;

void Obj_produce();      //目标代码生成函数
void obj_send(string op, string R, string M);    //目标代码发送
void Active();    //构建活跃性息函数  0表示不活跃n，1表示活跃y
void DivideBlock();   //四元式分块函数

//==========================================================

#endif