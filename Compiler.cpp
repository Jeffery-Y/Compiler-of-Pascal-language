#include "Table.h"
extern FileStream fileStream;
extern Token token;

extern vector<Token>TokenList;//token����
extern vector<QT_item> QT;//��Ԫʽ�洢��
extern vector<QT_item>QT_1;//�Ż������Ԫʽ�洢��
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

void show_success()
{
	system("cls");
	cout << "\n\n\n\n\n\n\n\n\n\n\n\n";
	cout << "\t\t\t\t     ����ɹ���" << endl << endl;
	cout << "\t\t\t�û��س�������������鿴ҳ��" << endl;
	system("pause");
	show_main();
	system("pause");
}

void show0()
{
	system("cls");
	cout << "********************************************************************************\n";
	cout << "******                           ��Ԫʽ(" << QT.size() << "��)                             ******\n";
	cout << "\n\n";

	for (QT_item & qt : QT)
		cout << "\t\t\t    (" << qt.w << "," << qt.o1 << "," << qt.o2 << "," << qt.t << ")" << endl;

	cout << endl;

	
	cout << "********************************************************************************\n";
	cout << "******                       �Ż������Ԫʽ(" << QT_1.size() << "��)                         ******\n";
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
	cout << "\t\t\t\t" << "����" << '\t' << "����" << endl;
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
	cout << "******                              ���ű�(" << SYNBL.size() << "��)                          ******\n";
	cout << "\n\n";
	cout << "\t\t    " << "���" << '\t' << "����" << '\t' << "����" << '\t' << "����" << '\t' << "addr" << '\t' << "���" << '\t' << endl;
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
	cout << "******                              ���ͱ�(" << TYPEL.size() << "��)                          ******\n";
	cout << "\n\n";
	cout << "\t\t\t " << "���" << '\t' << "����" << '\t' << "����λ��" << endl;
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
	cout << "******                              �����(" << AINEL.size() << "��)                           ******\n";
	cout << "\n\n";
	cout << "\t\t" << "���" << '\t' << "�½�" << '\t' << "�Ͻ�" << '\t' << "����" << '\t' << "�ɷֳ���" << '\t' << endl;
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
	cout << "******                              �ṹ��(" << RINFL.size() << "��)                           ******\n";
	cout << "\n\n";
	cout << "\t\t\t" << "���" << '\t' << "����" << '\t' << "ƫ����" << '\t' << "����" << endl;
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
	cout << "******                              ������(" << PFINEL.size() << "��)                           ******\n";
	cout << "\n\n";
	cout << "\t" << "��κ�" << '\t' << "����" << '\t' << "��������" << '\t' << "��ڵ�ַ" << '\t' << "������ʼλ��" << endl;
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
	cout << "******                              ������(" << PARAMBL.size() << "��)                           ******\n";
	cout << "\n\n";
	cout << "\t\t    " << "���" << '\t' << "����" << '\t' << "����" << '\t' << "����" << '\t' << "addr" << '\t' << "���" << '\t' << endl;
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
	cout << "******                                ���ȱ�(" << LENL.size() << "��)                        ******\n";
	cout << "\n\n";
	cout << "\t\t\t\t" << "���" << '\t' << "����" << endl;
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
	cout << "******                               ������                               ******\n";
	cout << "\n\n";

	cout << "\t\t\t\t" << "������(" << INTEGER.size() << "��)\n";
	cout << "\t\t\t\t" << "���" << '\t' << "����" << endl;
	int j = 0;
	for (j = 0; j < INTEGER.size(); j++)
	{
		cout << "\t\t\t\t" << j << '\t' << INTEGER.at(j) << endl;
	}
	cout << endl;

	cout << "\t\t\t\t" << "ʵ����(" << REAL.size() << "��)\n";
	cout << "\t\t\t\t" << "���" << '\t' << "����" << endl;
	for (j = 0; j < REAL.size(); j++)
	{
		cout << "\t\t\t\t" << j << '\t' << REAL.at(j) << endl;
	}
	cout << endl;

	cout << "\t\t\t\t" << "�ַ���(" << CHAR.size() << "��)\n";
	cout << "\t\t\t\t" << "���" << '\t' << "����" << endl;
	for (j = 0; j < CHAR.size(); j++)
	{
		cout << "\t\t\t\t" << j << '\t' << CHAR.at(j) << endl;
	}
	cout << endl;

	cout << "\t\t\t\t" << "������(" << BOOL.size() << "��)\n";
	cout << "\t\t\t\t" << "���" << '\t' << "����" << endl;
	for (j = 0; j < BOOL.size(); j++)
	{
		cout << "\t\t\t\t" << j << '\t' << BOOL.at(j) << endl;
	}
	cout << endl;

	cout << "\t\t\t\t" << "������(" << CONSL.size() << "��)\n";
	cout << "\t\t\t\t" << "���" << '\t' << "����" << '\t' << "����" << endl;
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
	cout << "******                         �Ż������Ԫʽ(" << QT_1.size() << "��)                    ******\n";
	cout << "\n\n";

	for (QT_item & qt : QT_1)
		cout << "\t\t\t    (" << qt.w << "," << qt.o1 << "," << qt.o2 << "," << qt.t << ")" << endl;

	cout << endl;
	system("pause");
}

extern int cur_obj_i;
extern OBJ obj[100];                            //Ŀ�������
void showb()
{
	system("cls");
	
	cout << "----------------Ŀ�������------------------" << endl;
	cout << "DSEG         SEGEMENT" << endl;
	var_address();
	cout << "DSEG         ENDS\nCSEG SEGMENT" << endl;
	cout << "         ASSUME     CS:CSEG,DS:DSEG" << endl;
	cout << "STARTS:" << endl;
	for (int i = 0; i<cur_obj_i; i++)
	{
		cout << "       " << i << ": " << obj[i].op << "  " << obj[i].R << "��" << obj[i].M << endl;
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
		cout << "******                                ������                              ******\n";
		cout << "\n\n\n";
		cout << "                                 ���չʾ��\n";
		cout << "                           ______________________\n\n";
		cout << "                               1: Token����\n";
		cout << "                               2: ���ű�\n";
		cout << "                               3: ���ͱ�\n";
		cout << "                               4: �����\n";
		cout << "                               5: �ṹ��\n";
		cout << "                               6: ������\n";
		cout << "                               7: ������\n";
		cout << "                               8: ���ȱ�\n";
		cout << "                               9: ������\n";
		cout << "                               0: ��Ԫʽ\n";
		cout << "                               A: �Ż������Ԫʽ\n";
		cout << "                               B: Ŀ���������\n";
		cout << "\n\n";
		cout << "                     ��ѡ������0��B:___\b\b";
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
	cout << "\t    ��������Ϣ��" << endl << endl;
	cout << "\t    ";

	CopyFile();//��ȡ�ļ�
	PROGRAM();//�ʷ��﷨����

	optimize();//�м�����Ż�

	Active();//��Ծ��Ϣ
	Obj_produce();//Ŀ���������


	show_success();//���չʾ

	system("pause");

	return 0;
}