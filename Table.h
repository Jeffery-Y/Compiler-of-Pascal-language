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
{//�ݴ��ı��ļ��е�Դ����
	long offset;//��ȡλ��
	long length;//���볤��
	bool endFlag;//������־
	string text;//Դ����
};

struct Token
{
	int type;//Token�ķ�����
	string name;
	//bool L;//Ŀ���������ʱ�õ��Ļ�Ծ��Ϣ
};


//===================�ʷ�����������=====================
int Search_KeyWords(string str);//���ҹؼ���
void CopyFile();//��ȡ�ı��ļ��еĴ���
int state_change(int state, char ch);//�ʷ������Զ���
int state_to_type(int state_before, string str);//ʶ��token�����
void NextToken();//��ȡ��һ��token
//======================================================

//===================�﷨����������=====================
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

struct Synbl_item//���ű���
{
	string name;
	int type;//TYPEL�е�ƫ����
	char cat;//������룺f,c,t,d,v
	int addr;//����,�ṹ,����,����ʱΪ�����е�ƫ����;��Ϊ���������ʱΪ���¼�е�ƫ����
	int level;//��κ�
};

struct Typel_item//���ͱ���
{
	char tval;//���ʹ���:i,r,c,b,a,d
	int tpoint;//AINEL��RINFL�е�ƫ����
};

struct Ainel_item//�������
{
	int low;//������½�
	int up;//������Ͻ�
	int ctp;//TYPEL�е�ƫ����
	int clen;//�ɷ����͵ĳ���
};

struct Rinfl_item//�ṹ����
{
	string id;//�ṹ������
	int off;//��ǰ�ṹ��ַ�ĵ�ַ
	int tp;//TYPEL�е�ƫ����
};

struct Consl_item//��������
{
	int type;
	string con;
};

struct Pfinfl_item
{
	int level;//Ƕ�ײ�κ�
	int off;//����
	int fn;//��������
	string entry;//��ڵ�ַ
	int param;//������
};

struct XX_item//TPPTB
{
	int L;
	int offset;
};

//===================����Ϊ���ű�����=====================

int search_CHAR(char char_val);		//�����ַ��������򷵻�offset�����򷵻�-1��
int search_INTEGER(long long_val);	//���������������򷵻�offset�����򷵻�-1��
int search_REAL(double double_val);	//����ʵ���������򷵻�offset�����򷵻�-1��
int search_KEYWL(string string_val);	//���ҹؼ��ֱ������򷵻�offset�����򷵻�-1��
int search_SYNBL(string string_val);  //���ҷ��ű������򷵻�offset�����򷵻�-1��
int search_RINFL(string Struct_name, string item_name);//���ҵ�ǰ�ṹ���е�������򷵻�offset�����򷵻�-1��

//==========================================================

struct QT_item//��Ԫʽ�ṹ��
{
	string w;//���
	string o1;//�������1
	string o2;//�������2
	string t;//���
};

struct CONST_QT_item
{
	QT_item con_qt;//�����������Ԫʽ
	int level;//��ǰ�����Ĳ��
};

//===================�������������=====================

//������
void PROGRAM();
//��������
void SUB_PROGRAM();
//�������弰��������
void DECLARATION();
//�������庯��
void VAR_DECLARATION();
//��д���ű�
void Type_Write_SYNBL();
//�������庯��
void CONST_DECLARATION();
//���Ͷ���
void DECLARATION_TYPE();
//���Ͷ����б�
void TYPE_DECLARATION();
//�ṹ�嶨������
void RECORD_TYPE();
//���̺���
void PSUBPROGRAM_DECLARATION();
//�ӳ�����
void SUBPROGRAM_DECLARATION();
//���̺���head
void PSUBPROGRAM_HEAD();
//�ӳ��������return���Ͷ���
void SUBPROGRAM_HEAD();
//�ӳ���return����
void SUBP_RETURN();
//�ӳ���Ĳ���
void ARGUMENT();
//���̺����Ĳ���
void PARGUMENT();
//����
void TYPE();
//��������
void DETYPE();
//��������
void ARRAY_TYPE();
//��ʶ���б�
void ID_LIST();
//���
void COM_SENTENCE();
//�������
void SEN_SEQUENCE();
//if���
void IF_SENTENCE();
//while���
void WHILE_SENTENCE();
//�ж����
void CONDITION();
//���ʽ���󲿷�
void EVA_SUBPROGRAM_OPTION();
//�������ò���
void FUNCTION_VN();
//���̺���
void PROCEDURE_VN();
//�������ò�������ƥ��
void PARAM_MATCH(string func, Token func_param, int vn_num);
//���ʽ
void EXPRESSION();
//��
void TERM();
//����
void FACTOR();
//�ṹ�����麯������
void ID_SID_OPTION();
//����ʱ����������ű�
void Write_T_SYN(Token t1, Token t2);
//���ر�ʶ����������
int IT_Type(Token t);
//��Ŀ������з����ַ
void var_address();

//==========================================================


//===================����Ϊչʾ���������=====================

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


//===================����Ϊ�Ż��ṹ����=====================

struct DagNode                      //dag����ͼ�еĽ��
{
	int fatherNum;					//�Ƿ��и��ڵ�
	bool isSingle;					//�Ƿ��ǵ���֧
	bool isLeaf;
	bool isVal;                     //�Ƿ��ǳ���         
	double value;                   //ֵ��
	string opt;                       //������
	int NodeNum;					//�ýڵ�ı��
	int RChild, LChild;				//�����ӽڵ�ı��
	int flagNum;					//�����ڸýڵ�ı�����
	string flags[20];				//�����ڸýڵ�ı���
	DagNode()
	{
		isVal = false;
		fatherNum = 0;
		flagNum = 0;
		isLeaf = false;
		isSingle = false;
	}

};

struct VarNode               //Ϊÿ����������ֵ
{
	string str;
	int flagNode;            //�������Ľ����
	VarNode()
	{
		flagNode = 0;
	}
};

int optimize();//����Ż�

//==========================================================


//===================����ΪĿ�������=====================

typedef struct OBJ{
	string op;
	string R;
	string M;
}OBJ;

void Obj_produce();      //Ŀ��������ɺ���
void obj_send(string op, string R, string M);    //Ŀ����뷢��
void Active();    //������Ծ��Ϣ����  0��ʾ����Ծn��1��ʾ��Ծy
void DivideBlock();   //��Ԫʽ�ֿ麯��

//==========================================================

#endif