#include "Table.h"

vector<Synbl_item> SYNBL;//符号表
vector<Typel_item> TYPEL = { { 'i', -1 }, { 'r', -1 }, { 'c', -1 }, { 'b', -1 }, { 's', -1 } };//类型表
vector<Ainel_item> AINEL;//数组表
vector<Rinfl_item> RINFL;//结构表
vector<Pfinfl_item> PFINEL;//函数表
vector<Synbl_item> PARAMBL;//参数表
vector<Consl_item> CONSL;//常量表
vector<int> LENL;//长度表


vector<double>REAL;//实数表
vector<long>INTEGER;//整数表
vector<bool>BOOL;//布尔表
vector<char>CHAR;//字符表
vector<XX_item>XX;//TPPTB

int Level = 1;//当前层次号

//查找字符表，存在则返回offset，否则返回-1；
int search_CHAR(char char_val)
{
	for (int i = CHAR.size() - 1; i >= 0; i--)
	{
		if (char_val == CHAR.at(i))
			return i;
	}
	return -1;
}

//查找整数表，存在则返回offset，否则返回-1；
int search_INTEGER(long long_val)
{
	for (int i = INTEGER.size() - 1; i >= 0; i--)
	{
		if (long_val == INTEGER.at(i))
			return i;
	}
	return -1;
}

//查找实数表，存在则返回offset，否则返回-1；
int search_REAL(double double_val)
{
	for (int i = REAL.size() - 1; i >= 0; i--)
	{
		if (double_val == REAL.at(i))
			return i;
	}
	return -1;
}

//查找符号表，存在则返回offset，否则返回-1；
int search_SYNBL(string string_val)
{
	for (int i = SYNBL.size() - 1; i >= 0; i--)
	{
		if (SYNBL.at(i).cat == 'f' && SYNBL.at(i).name == string_val)
		{
			return i;//偏移量（下标）
		}
		else if (SYNBL.at(i).cat == 'p' && SYNBL.at(i).name == string_val)
		{
			return i;//偏移量（下标）
		}
		else if (SYNBL.at(i).level <= Level && SYNBL.at(i).name == string_val)
		{
			return i;//偏移量（下标）
		}
	}
	return -1;
}

//查找当前结构体中的项，存在则返回offset，否则返回-1；
int search_RINFL(string Struct_name, string item_name)
{
	int i = TYPEL.at(SYNBL.at(search_SYNBL(Struct_name)).type).tpoint;
	if (RINFL.at(i).id == item_name)return i;
	for ( i++; RINFL.at(i).off != 0; i++)
	{
		if (RINFL.at(i).id == item_name)return i;//偏移量（下标）
	}
	return -1;
}