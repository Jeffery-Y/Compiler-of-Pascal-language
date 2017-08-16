/*#include"Table.h"

extern vector<Token>TokenList;
extern FileStream fileStream;
extern Token token;

extern vector<QT_item> QT;//��Ԫʽ�洢��
extern vector<Synbl_item> SYNBL;//���ű�
extern vector<Typel_item> TYPEL;//���ͱ�
extern vector<Ainel_item> AINEL;//�����
extern vector<Rinfl_item> RINFL;//�ṹ��
extern vector<Pfinfl_item> PFINEL;//������
extern vector<Synbl_item> PARAMBL;//������
extern vector<Consl_item> CONSL;//������
extern vector<int> LENL;//���ȱ�

extern vector<double>REAL;//ʵ����
extern vector<long>INTEGER;//������
extern vector<bool>BOOL;//������
extern vector<char>CHAR;//�ַ���


void main()
{
	
	CopyFile();
	PROGRAM();

	cout << "��Ԫʽ�м��������(" << QT.size() << "��)" << endl;
	for (QT_item & qt : QT)
		cout << "(" << qt.w << "," << qt.o1 << "," << qt.o2 << "," << qt.t << ")" << endl;

	cout << endl << "���ű�(" << SYNBL.size() << "��)" << endl;
	cout << "name\ttype\tcat\taddr\tlevel" << endl;
	for (Synbl_item & s : SYNBL)
		cout << s.name << "\t" << s.type << "\t" << s.cat << "\t" << s.addr << "\t" << s.level << endl;

	cout << endl << "���ͱ�(" << TYPEL.size() << "��)" << endl;
	cout << "tval\ttpoint" << endl;
	for (Typel_item & t : TYPEL)
		cout << t.tval << "\t" << t.tpoint << endl;

	cout << endl << "�����(" << AINEL.size() << "��)" << endl;
	cout << "low\tup\tctp\tclen" << endl;
	for (Ainel_item & a : AINEL)
		cout << a.low << "\t" << a.up << "\t" << a.ctp << "\t" << a.clen << endl;

	cout << endl << "������(" << PFINEL.size() << "��)" << endl;
	cout << "level\toff\tfn\tentry\tparam" << endl;
	for (Pfinfl_item & p : PFINEL)
		cout << p.level << "\t" << p.off << "\t" << p.fn << "\t" << p.entry << "\t" << p.param << endl;

	cout << endl << "������(" << SYNBL.size() << "��)" << endl;
	cout << "name\ttype\tcat\taddr\tlevel" << endl;
	for (Synbl_item & p : PARAMBL)
		cout << p.name << "\t" << p.type << "\t" << p.cat << "\t" << p.addr << "\t" << p.level << endl;

	cout << endl << "�ṹ��(" << RINFL.size() << "��)" << endl;
	cout << "id\toff\ttp" << endl;
	for (Rinfl_item & r : RINFL)
		cout << r.id << "\t" << r.off << "\t" << r.tp << endl;

	cout << endl << "������(" << CONSL.size() << "��)" << endl;
	cout << "type\tcon" << endl;
	for (Consl_item & c: CONSL)
		cout << c.type << "\t" << c.con << endl;

	cout << endl << "������(" << INTEGER.size() << "��)" << endl;
	for (int i = 0; i < INTEGER.size(); i++)
		cout << INTEGER.at(i) << endl; 
	
	cout << endl << "������(" << BOOL.size() << "��)" << endl;
	for (int i = 0; i < BOOL.size(); i++)
		cout << BOOL.at(i) << endl;

	cout << endl << "ʵ����(" << REAL.size() << "��)" << endl;
	for (double & d :REAL)
		cout << d << endl;

	cout << endl << "�ַ���(" << CHAR.size() << "��)" << endl;
	for (char & c : CHAR)
		cout << c << endl;



	//���Դʷ�������
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
