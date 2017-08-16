#include "Table.h"

vector<Synbl_item> SYNBL;//���ű�
vector<Typel_item> TYPEL = { { 'i', -1 }, { 'r', -1 }, { 'c', -1 }, { 'b', -1 }, { 's', -1 } };//���ͱ�
vector<Ainel_item> AINEL;//�����
vector<Rinfl_item> RINFL;//�ṹ��
vector<Pfinfl_item> PFINEL;//������
vector<Synbl_item> PARAMBL;//������
vector<Consl_item> CONSL;//������
vector<int> LENL;//���ȱ�


vector<double>REAL;//ʵ����
vector<long>INTEGER;//������
vector<bool>BOOL;//������
vector<char>CHAR;//�ַ���
vector<XX_item>XX;//TPPTB

int Level = 1;//��ǰ��κ�

//�����ַ��������򷵻�offset�����򷵻�-1��
int search_CHAR(char char_val)
{
	for (int i = CHAR.size() - 1; i >= 0; i--)
	{
		if (char_val == CHAR.at(i))
			return i;
	}
	return -1;
}

//���������������򷵻�offset�����򷵻�-1��
int search_INTEGER(long long_val)
{
	for (int i = INTEGER.size() - 1; i >= 0; i--)
	{
		if (long_val == INTEGER.at(i))
			return i;
	}
	return -1;
}

//����ʵ���������򷵻�offset�����򷵻�-1��
int search_REAL(double double_val)
{
	for (int i = REAL.size() - 1; i >= 0; i--)
	{
		if (double_val == REAL.at(i))
			return i;
	}
	return -1;
}

//���ҷ��ű������򷵻�offset�����򷵻�-1��
int search_SYNBL(string string_val)
{
	for (int i = SYNBL.size() - 1; i >= 0; i--)
	{
		if (SYNBL.at(i).cat == 'f' && SYNBL.at(i).name == string_val)
		{
			return i;//ƫ�������±꣩
		}
		else if (SYNBL.at(i).cat == 'p' && SYNBL.at(i).name == string_val)
		{
			return i;//ƫ�������±꣩
		}
		else if (SYNBL.at(i).level <= Level && SYNBL.at(i).name == string_val)
		{
			return i;//ƫ�������±꣩
		}
	}
	return -1;
}

//���ҵ�ǰ�ṹ���е�������򷵻�offset�����򷵻�-1��
int search_RINFL(string Struct_name, string item_name)
{
	int i = TYPEL.at(SYNBL.at(search_SYNBL(Struct_name)).type).tpoint;
	if (RINFL.at(i).id == item_name)return i;
	for ( i++; RINFL.at(i).off != 0; i++)
	{
		if (RINFL.at(i).id == item_name)return i;//ƫ�������±꣩
	}
	return -1;
}