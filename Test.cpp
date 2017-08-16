/*#include"Table.h"

extern vector<Token>TokenList;
extern FileStream fileStream;
extern Token token;

extern vector<QT_item> QT;//四元式存储区
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


void main()
{
	
	CopyFile();
	PROGRAM();

	cout << "四元式中间代码生成(" << QT.size() << "条)" << endl;
	for (QT_item & qt : QT)
		cout << "(" << qt.w << "," << qt.o1 << "," << qt.o2 << "," << qt.t << ")" << endl;

	cout << endl << "符号表(" << SYNBL.size() << "条)" << endl;
	cout << "name\ttype\tcat\taddr\tlevel" << endl;
	for (Synbl_item & s : SYNBL)
		cout << s.name << "\t" << s.type << "\t" << s.cat << "\t" << s.addr << "\t" << s.level << endl;

	cout << endl << "类型表(" << TYPEL.size() << "条)" << endl;
	cout << "tval\ttpoint" << endl;
	for (Typel_item & t : TYPEL)
		cout << t.tval << "\t" << t.tpoint << endl;

	cout << endl << "数组表(" << AINEL.size() << "条)" << endl;
	cout << "low\tup\tctp\tclen" << endl;
	for (Ainel_item & a : AINEL)
		cout << a.low << "\t" << a.up << "\t" << a.ctp << "\t" << a.clen << endl;

	cout << endl << "函数表(" << PFINEL.size() << "条)" << endl;
	cout << "level\toff\tfn\tentry\tparam" << endl;
	for (Pfinfl_item & p : PFINEL)
		cout << p.level << "\t" << p.off << "\t" << p.fn << "\t" << p.entry << "\t" << p.param << endl;

	cout << endl << "参数表(" << SYNBL.size() << "条)" << endl;
	cout << "name\ttype\tcat\taddr\tlevel" << endl;
	for (Synbl_item & p : PARAMBL)
		cout << p.name << "\t" << p.type << "\t" << p.cat << "\t" << p.addr << "\t" << p.level << endl;

	cout << endl << "结构表(" << RINFL.size() << "条)" << endl;
	cout << "id\toff\ttp" << endl;
	for (Rinfl_item & r : RINFL)
		cout << r.id << "\t" << r.off << "\t" << r.tp << endl;

	cout << endl << "常量表(" << CONSL.size() << "条)" << endl;
	cout << "type\tcon" << endl;
	for (Consl_item & c: CONSL)
		cout << c.type << "\t" << c.con << endl;

	cout << endl << "整数表(" << INTEGER.size() << "条)" << endl;
	for (int i = 0; i < INTEGER.size(); i++)
		cout << INTEGER.at(i) << endl; 
	
	cout << endl << "布尔表(" << BOOL.size() << "条)" << endl;
	for (int i = 0; i < BOOL.size(); i++)
		cout << BOOL.at(i) << endl;

	cout << endl << "实数表(" << REAL.size() << "条)" << endl;
	for (double & d :REAL)
		cout << d << endl;

	cout << endl << "字符表(" << CHAR.size() << "条)" << endl;
	for (char & c : CHAR)
		cout << c << endl;



	//测试词法分析器
	/*CopyFile();
	while (fileStream.endFlag != 1)
	{
		NextToken();
		cout << token.name << " " << token.type << endl;
	}
	cout << "TokenList:" << endl;
	for (Token &t : TokenList)
	{
		cout << t.name << " " << t.type << endl;
	}

}*/
