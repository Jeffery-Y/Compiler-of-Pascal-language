#include "Table.h"

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

extern vector<Token>TokenList;//����ʷ��������õ�token��
extern Token token;//��ǰtoken
extern FileStream fileStream;
extern int line;//��ǰ�к�

vector<Synbl_item> Name_temp;//��ʱ���ű����vector
vector<Typel_item> TYPEL_temp;//��ʱ���ͱ�
Synbl_item syn_temp;//��ʱ���ű���
stack<Token> SEM;//����ջ
vector<QT_item> QT;//��Ԫʽ�洢��
vector<CONST_QT_item> CONST_QT;//�����������Ԫʽ�ݴ�
vector<QT_item> PROCE_QT;//���̺�������Ԫʽ�ݴ�
string ID_name;//�ݴ溯����
int function_vn_num = 0;//��������ʱ�Ĳ�������
int Vn_Num = 0;//��������ʱ�Ĳ�������
int rinfl_off = 0;//�ṹ���offset
extern int Level;//��ǰ��κ�
string Tnum = "t";
int num_T = 1;

int IT_Type(Token t)
{//���ر�ʶ����������
	if (t.type == 2)
	{
		//�жϵ�ǰtoken�Ƿ�Ϊ�ṹ�������δ��
		if (t.name.find_first_of('[') != string::npos && t.name.find_first_of('.') != string::npos)
		{
			if (t.name.find_last_of('[') < t.name.find_last_of('.'))
			{
				t.name.erase(0, t.name.find_last_of('.') + 1);//�ṹ��
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
					case 'b':return 29;//������
					case 'a':return 8;//������
					case 'd':return 9;//�ṹ��
					default:{cout << "δ֪�ı�ʶ�������룡�к�Ϊ��" << line << endl; exit(0); }
					}
				}
				else { cout << "δ֪�ı�ʶ�������룡�к�Ϊ��" << line << endl; exit(0); }
			}
			else
			{
				string array_name = t.name;
				array_name.erase(t.name.find_first_of('['), t.name.size() - t.name.find_first_of('['));//������
				array_name.erase(0, t.name.find_last_of('.') + 1);//�ṹ��
				t.name.erase(0, t.name.find_last_of('.') + 1);//�ṹ��
				Ainel_item array_typel = AINEL.at(TYPEL.at(SYNBL.at(search_SYNBL(array_name)).type).tpoint);

				//t.name.erase(t.name.find_first_of('['), t.name.size() - t.name.find_first_of('['));//����
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
				case 'b':return 29;//������
				case 'a':return 8;//������
				case 'd':return 9;//�ṹ��
				default:{cout << "δ֪�ı�ʶ�������룡�к�Ϊ��" << line << endl; exit(0); }
				}
			}
		}
		if (t.name.find_first_of('[') != string::npos)
		{
			string array_name = t.name;
			array_name.erase(t.name.find_first_of('['), t.name.size() - t.name.find_first_of('['));//������
			Ainel_item array_typel = AINEL.at(TYPEL.at(SYNBL.at(search_SYNBL(array_name)).type).tpoint);

			//t.name.erase(t.name.find_first_of('['), t.name.size() - t.name.find_first_of('['));//����
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
			case 'b':return 29;//������
			case 'a':return 8;//������
			case 'd':return 9;//�ṹ��
			default:{cout << "δ֪�ı�ʶ�������룡�к�Ϊ��" << line << endl; exit(0); }
			}

		}
		else if (t.name.find_last_of('.') != string::npos)
		{
			t.name.erase(0, t.name.find_last_of('.') + 1);//�ṹ��
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
				case 'b':return 29;//������
				case 'a':return 8;//������
				case 'd':return 9;//�ṹ��
				default:{cout << "δ֪�ı�ʶ�������룡�к�Ϊ��" << line << endl; exit(0); }
				}
			}
			else { cout << "δ֪�ı�ʶ�������룡�к�Ϊ��" << line << endl; exit(0); }
		}
		else if (SYNBL.at(search_SYNBL(t.name)).cat == 'f')
		{
			if (SYNBL.at(search_SYNBL(t.name)).type == -1){ cout << "�����޷���ֵ���к�Ϊ��" << line << endl; exit(0); }
		}
		//else һ���������ͻ��ߺ���
		switch (TYPEL.at(SYNBL.at(search_SYNBL(t.name)).type).tval)
		{
		case 'i':return 3;
		case 'r':return 4;
		case 'c':return 6;
		case 'b':return 29;//������
		case 'a':return 8;//������
		case 'd':return 9;//�ṹ��
		default:{cout << "δ֪�ı�ʶ�������룡�к�Ϊ��" << line << endl; exit(0);}
		}
	}
	else return t.type;
}

//����ʱ����������ű�
void Write_T_SYN(Token t1, Token t2)
{//����ʱ����
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
		INTEGER.push_back(0);//��ʼֵΪ0
	}
	else if (t1.type == 4)
	{
		SYNBL.push_back({ Tnum, 1, 'v', REAL.size(), Level });
		REAL.push_back(0.0);//��ʼֵΪ0
	}
	else if (t1.type == 6)
	{
		SYNBL.push_back({ Tnum, 2, 'v', CHAR.size(), Level });
		CHAR.push_back('\0');//��ʼֵΪ0
	}
}

//�ṹ�����麯������
void ID_SID_OPTION()
{
	if (SYNBL.at(search_SYNBL(token.name)).cat == 'v' || SYNBL.at(search_SYNBL(token.name)).cat == 'f' || SYNBL.at(search_SYNBL(token.name)).cat == 'n' || SYNBL.at(search_SYNBL(token.name)).cat == 'e' || SYNBL.at(search_SYNBL(token.name)).cat == 'c')//|| SYNBL.at(search_SYNBL(token.name)).cat == 'n' || SYNBL.at(search_SYNBL(token.name)).cat == 'f'
	{
		SEM.push({ 2, token.name });
		NextToken();

		if (token.name == ".")//����ΪID���ǵ�ɾ��SEM.top
		{//�ṹ��
			NextToken();
			if (token.type != 2){ cout << "�ṹ�������.����ȱ��ʶ�����к�Ϊ��" << line << endl; exit(0); }

			if (TYPEL.at(SYNBL.at(search_SYNBL(SEM.top().name)).type).tval != 'd'){ cout << "�ñ������ǽṹ�壡�к�Ϊ��" << line << endl; exit(0); }
			if (search_RINFL(SEM.top().name, token.name) == -1){ cout << "�ýṹ����û�и���к�Ϊ��" << line << endl; exit(0); }

			SEM.top().name += '.';
			SEM.top().name += token.name;
			Token Struct_Node = token;//�ݴ浱ǰ��ʶ�������ж��Ƿ�Ϊ�ṹ������
			NextToken();
			while (token.name == ".")
			{
				NextToken();
				if (token.type != 2){ cout << "�ṹ�������.����ȱ��ʶ�����к�Ϊ��" << line << endl; exit(0); }

				if (TYPEL.at(SYNBL.at(search_SYNBL(Struct_Node.name)).type).tval != 'd'){ cout << "�ñ������ǽṹ�壡�к�Ϊ��" << line << endl; exit(0); }
				if (search_RINFL(Struct_Node.name, token.name) == -1){ cout << "�ýṹ����û�и���к�Ϊ��" << line << endl; exit(0); }

				SEM.top().name += '.';
				SEM.top().name += token.name;
				Struct_Node = token;
			}
			if (token.name == "[")
			{
				if (TYPEL.at(SYNBL.at(search_SYNBL(Struct_Node.name)).type).tval != 'a'){ cout << "�ñ����������飡�к�Ϊ��" << line << endl; exit(0); }

				Ainel_item array_typel = AINEL.at(TYPEL.at(SYNBL.at(search_SYNBL(Struct_Node.name)).type).tpoint);
				string array_num = token.name;//�ݴ�������±�
				//SEM.pop();
				NextToken();
				if (IT_Type(token) != 3){ cout << "�﷨��������ʹ�������к�Ϊ��" << line << endl; exit(0); }

				if (token.type == 3 && (stoi(token.name) > array_typel.up || stoi(token.name) < array_typel.low))
				{
					cout << "����Խ�磡�к�Ϊ��" << line << endl; exit(0);
				}

				array_num += token.name;
				NextToken();

				if (TYPEL.at(array_typel.ctp).tval != 'a' && token.name == ","){ cout << "��������ά�ȣ��к�Ϊ��" << line << endl; exit(0); }
				array_typel = AINEL.at(TYPEL.at(array_typel.ctp).tpoint);

				while (token.name == ",")
				{
					array_num += token.name;
					NextToken();
					if (IT_Type(token) != 3){ cout << "�﷨��������ʹ�������к�Ϊ��" << line << endl; exit(0); }

					if (token.type == 3 && (stoi(token.name) > array_typel.up || stoi(token.name) < array_typel.low))
					{
						cout << "����Խ�磡�к�Ϊ��" << line << endl; exit(0);
					}

					array_num += token.name;
					NextToken();

					//�ж�����ά���Ƿ�Խ��

					if (TYPEL.at(array_typel.ctp).tval != 'a' && token.name == ","){ cout << "��������ά�ȣ��к�Ϊ��" << line << endl; exit(0); }
					//if (TYPEL.at(array_typel.ctp).tval == 'a' && token.name != ","){ cout << "��ȷ��Ҫʹ����������ֱ�ӽ���������Ҫ�����Ƿ���һ���� �к�Ϊ��" << line << endl; exit(0); }
					if (token.name == ",")
						array_typel = AINEL.at(TYPEL.at(array_typel.ctp).tpoint);

				}
				if (token.name != "]"){ cout << "�﷨��������ʹ������ȱ��]���к�Ϊ��" << line << endl; exit(0); }
				array_num += token.name;
				SEM.top().name += array_num;
				NextToken();
				/*
				string array_num = token.name;//�ݴ�������±�
				if (TYPEL.at(SYNBL.at(search_SYNBL(Struct_Node.name)).type).tval != 'a'){ cout << "�ñ����������飡�к�Ϊ��" << line << endl; exit(0); }
				//SEM.pop();
				NextToken();
				if (IT_Type(token) != 3){ cout << "�﷨��������ʹ�������к�Ϊ��" << line << endl; exit(0); }
				array_num += token.name;
				NextToken();
				while (token.name == ",")
				{
					array_num += token.name;
					NextToken();
					if (IT_Type(token) != 3){ cout << "�﷨��������ʹ�������к�Ϊ��" << line << endl; exit(0); }
					array_num += token.name;
					NextToken();

				}
				if (token.name != "]"){ cout << "�﷨��������ʹ������ȱ��]���к�Ϊ��" << line << endl; exit(0); }
				array_num += token.name;
				SEM.top().name += array_num;
				NextToken();*/
			}
		}
		else if (token.name == "[")
		{//����
			
			
			if (TYPEL.at(SYNBL.at(search_SYNBL(SEM.top().name)).type).tval != 'a'){ cout << "�ñ����������飡�к�Ϊ��" << line << endl; exit(0); }
			
			Ainel_item array_typel = AINEL.at(TYPEL.at(SYNBL.at(search_SYNBL(SEM.top().name)).type).tpoint);
			string array_num = token.name;//�ݴ�������±�
			//SEM.pop();
			NextToken();
			if (IT_Type(token) != 3){ cout << "�﷨��������ʹ�������к�Ϊ��" << line << endl; exit(0); }
			
			if (token.type == 3 && (stoi(token.name) > array_typel.up || stoi(token.name) < array_typel.low))
			{
				cout << "����Խ�磡�к�Ϊ��" << line << endl; exit(0);
			}

			array_num += token.name;
			NextToken();

			if (TYPEL.at(array_typel.ctp).tval != 'a' && token.name == ","){ cout << "��������ά�ȣ��к�Ϊ��" << line << endl; exit(0); }
			array_typel = AINEL.at(TYPEL.at(array_typel.ctp).tpoint);
			
			while (token.name == ",")
			{
				array_num += token.name;
				NextToken();
				if (IT_Type(token) != 3){ cout << "�﷨��������ʹ�������к�Ϊ��" << line << endl; exit(0); }

				if (token.type == 3 && (stoi(token.name) > array_typel.up || stoi(token.name) < array_typel.low))
				{
					cout << "����Խ�磡�к�Ϊ��" << line << endl; exit(0);
				}
				
				array_num += token.name;
				NextToken();
				
				//�ж�����ά���Ƿ�Խ��

				if (TYPEL.at(array_typel.ctp).tval != 'a' && token.name == ","){ cout << "��������ά�ȣ��к�Ϊ��" << line << endl; exit(0); }
				//if (TYPEL.at(array_typel.ctp).tval == 'a' && token.name != ","){ cout << "��ȷ��Ҫʹ����������ֱ�ӽ���������Ҫ�����Ƿ���һ���� �к�Ϊ��" << line << endl; exit(0); }
				if (token.name == ",")
					array_typel = AINEL.at(TYPEL.at(array_typel.ctp).tpoint);

			}
			if (token.name != "]"){ cout << "�﷨��������ʹ������ȱ��]���к�Ϊ��" << line << endl; exit(0); }
			array_num += token.name;
			SEM.top().name += array_num;
			NextToken();
		}
		else if (token.name == "(")
		{//��������
			if (SYNBL.at(search_SYNBL(SEM.top().name)).cat == 'f')
			{
				NextToken();
				FUNCTION_VN();
				QT.push_back({ "call", SEM.top().name, to_string(function_vn_num), "_" });

				//����ֵ�ж�
				//�з���ֵʱ����ҪSEM.pop����
				//SEM.pop();
				Vn_Num = 0;
				if (token.name != ")"){ cout << "��������ȱ���������к�Ϊ��" << line << endl; exit(0); }
				NextToken();
			}
			/*else if ((SYNBL.at(search_SYNBL(SEM.top().name)).cat == 'p'))
			{
				NextToken();
				PROCEDURE_VN();
				QT.push_back({ "call", SEM.top().name, "_" , "_" });
				if (token.name != ")"){ cout << "��������ȱ���������к�Ϊ��" << line << endl; exit(0); }
				Vn_Num = 0;
				NextToken();
			}*/
			else {cout << "�﷨���󣡷Ǻ��������С������к�Ϊ��" << line << endl; exit(0); }
			
		}
		else  return;
	}
	else { cout << "�ñ�ʶ�����ǿɲ����������к�Ϊ��" << line << endl; exit(0); }

}

//����
void FACTOR()
{
	if (token.type == 2)
	{ 
		if (search_SYNBL(token.name) == -1){ cout << token.name << "û�б��������к�Ϊ��" << line << endl; exit(0); }
		//����ǰtokenѹ������ջ
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
		else { cout << "�﷨���󣡸��ź������ʹ����к�Ϊ��" << line << endl; exit(0); }
	}
	else if (token.name == "(")
	{ 
		NextToken(); 
		EXPRESSION();
		if (token.name == ")"){ NextToken(); return; }
	}
	else { cout <<  "�﷨���󣡱��ʽȱ���ӣ��к�Ϊ��" << line << endl; exit(0); }
}

//��
void TERM()
{
	FACTOR();
	while (token.name == "*" || token.name == "/")
	{
		if (token.name == "*")
		{
			NextToken();
			FACTOR();
			//������Ԫʽ
			Token t1 = SEM.top();
			SEM.pop();
			Token t2 = SEM.top();
			SEM.pop();
			//�ж��������������Ƿ�һ��,�����������������ṹ��δ��δ��
			if (IT_Type(t1) != IT_Type(t2)){ cout << "�����������Ͳ�ƥ�䣡�к�Ϊ��" << line << endl; exit(0); }
			Tnum += to_string(num_T++);
			QT.push_back({ "*", t2.name, t1.name, Tnum });
			//cout << "(" << QT.back().w << "," << QT.back().o1 << "," << QT.back().o2 << "," << QT.back().t << ")" << endl;
			
			SEM.push({ 2, Tnum });
			//���м����t������ű�
			Write_T_SYN(t1, t2);
			Tnum.erase(1, Tnum.size() - 1);
		}
		else if (token.name == "/")
		{
			NextToken();
			FACTOR();
			//������Ԫʽ
			Token t1 = SEM.top();
			SEM.pop();
			Token t2 = SEM.top();
			SEM.pop();

			//�ж��������������Ƿ�һ��,�����������������ṹ��δ��δ��
			if (IT_Type(t1) != IT_Type(t2)){ cout << "�����������Ͳ�ƥ�䣡�к�Ϊ��" << line << endl; exit(0); }
			Tnum += to_string(num_T++);
			QT.push_back({ "/", t2.name, t1.name, Tnum });
			//cout << "(" << QT.back().w << "," << QT.back().o1 << "," << QT.back().o2 << "," << QT.back().t << ")" << endl;
			SEM.push({ 2, Tnum });
			//���м����t������ű�
			Write_T_SYN(t1, t2);

			Tnum.erase(1, Tnum.size() - 1);
		}
	}
}

//���ʽ
void EXPRESSION()
{
	TERM();
	while (token.name == "+" || token.name == "-")
	{
		if (token.name == "+")
		{
			NextToken();
			TERM();
			//������Ԫʽ
			Token t1 = SEM.top();
			SEM.pop();
			Token t2 = SEM.top();
			SEM.pop();
			if (IT_Type(t1) != IT_Type(t2)){ cout << "�����������Ͳ�ƥ�䣡�к�Ϊ��" << line << endl; exit(0); }
			Tnum += to_string(num_T++);
			QT.push_back({ "+", t2.name, t1.name, Tnum });
			//cout << "(" << QT.back().w << "," << QT.back().o1 << "," << QT.back().o2 << "," << QT.back().t << ")" << endl;
			SEM.push({ 2, Tnum });
			//���м����t������ű�
			Write_T_SYN(t1, t2);

			Tnum.erase(1, Tnum.size() - 1);
		}
		else if (token.name == "-")
		{
			NextToken();
			TERM();
			//������Ԫʽ
			Token t1 = SEM.top();
			SEM.pop();
			Token t2 = SEM.top();
			SEM.pop();
			if (IT_Type(t1) != IT_Type(t2)){ cout << "�����������Ͳ�ƥ�䣡�к�Ϊ��" << line << endl; exit(0); }
			Tnum += to_string(num_T++);
			QT.push_back({ "-", t2.name, t1.name, Tnum });
			//cout << "(" << QT.back().w << "," << QT.back().o1 << "," << QT.back().o2 << "," << QT.back().t << ")" << endl;
			SEM.push({ 2, Tnum });
			//���м����t������ű�
			Write_T_SYN(t1, t2);

			Tnum.erase(1, Tnum.size() - 1);
		}
	}
}

//�������ò�������ƥ��
void PARAM_MATCH(string func, Token func_param, int vn_num)
{
	if (vn_num > PFINEL.at(SYNBL.at(search_SYNBL(SEM.top().name)).addr).fn)
	{
		cout << "��������ʱ���ݲ������࣡�к�Ϊ��" << line << endl; exit(0);
	}
	if (SYNBL.at(search_SYNBL(SEM.top().name) + Vn_Num + 1).cat == 'n')
	{
		if (func_param.type == 3 || func_param.type == 4 || func_param.type == 6 || func_param.type == 7 || SYNBL.at(search_SYNBL(token.name)).cat == 'c')
		{
			cout << "���������ββ��ɴ��볣�����к�Ϊ��" << line << endl; exit(0);
		}
	}
	int param_match_temp1;
	switch (TYPEL.at(SYNBL.at(search_SYNBL(func) + vn_num + 1).type).tval)
	{
	case 'i':param_match_temp1 = 3; break;
	case 'r':param_match_temp1 = 4; break;
	case 'c':param_match_temp1 = 6; break;
	case 'b':param_match_temp1 = 29; break;//������
	case 'a':param_match_temp1 = 8; break;//������
	case 'd':param_match_temp1 = 9; break;//�ṹ��
	default:{cout << "δ֪�ı�ʶ�������룡�к�Ϊ��" << line << endl; exit(0); }
	}
	
	//int param_match_temp1 = IT_Type({ 2, SYNBL.at(search_SYNBL(func) + vn_num + 1).name });
	int param_match_temp2 = IT_Type(func_param);//'i';// TYPEL.at(SYNBL.at(search_SYNBL(func_param.name)).type).tval;//3;//IT_Type(func_param);
	if (param_match_temp1 == param_match_temp2)
	{
		return;
	}
	else
	{
		cout << "�������ò�����ƥ�䣡�к�Ϊ��" << line << endl; exit(0);
	}

}

//�������ò���
void FUNCTION_VN()
{//������������ƥ��δ��
	if (token.type == 2)
	{
		//�жϲ��������Ƿ�ƥ��
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
		else { cout << "�������������к�Ϊ��" << line << endl; exit(0); }
	}
	else if (token.type == 3 || token.type == 4 || token.type == 6 || token.type == 7)
	{
		//�жϲ��������Ƿ�ƥ��
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

//���̺������ò���
void PROCEDURE_VN()
{
	if (token.type == 2)
	{
		//�жϲ��������Ƿ�ƥ��
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
		else { cout << "�������������к�Ϊ��" << line << endl; exit(0); }
	}
	else if (token.type == 3 || token.type == 4 || token.type == 6 || token.type == 7)
	{
		//�жϲ��������Ƿ�ƥ��
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

//���ʽ���󲿷�
void EVA_SUBPROGRAM_OPTION()
{
	SEM.push({ 2, token.name });
	//�����βΡ���ֵ�β�û��
	if (SYNBL.at(search_SYNBL(token.name)).cat == 'v' || SYNBL.at(search_SYNBL(token.name)).cat == 'n' || SYNBL.at(search_SYNBL(token.name)).cat == 'e')//|| SYNBL.at(search_SYNBL(token.name)).cat == 'n' || SYNBL.at(search_SYNBL(token.name)).cat == 'f'
	{//SYNBL.at(search_SYNBL(SEM.top().name)).cat == 'f'
		NextToken();
		if (token.name == ":=")
		{//���ʽ
			//����ƥ��ʱ��δ��������ͽṹ��
			NextToken();
			EXPRESSION();

			//������Ԫʽ
			Token t1 = SEM.top();
			SEM.pop();
			Token t2 = SEM.top();
			SEM.pop();
			//��ֵǰ����ƥ��
			if (IT_Type(t1) != IT_Type(t2)){ cout << "��ֵ��������������Ͳ�ƥ�䣡�к�Ϊ��" << line << endl; exit(0); }
			QT.push_back({ ":=", t1.name, "_", t2.name });
			//cout << "(" << QT.back().w << "," << QT.back().o1 << "," << QT.back().o2 << "," << QT.back().t << ")" << endl;

		}
		else if (token.name == ".")//����ΪID���ǵ�ɾ��SEM.top
		{//�ṹ��
			NextToken();
			if (token.type != 2){ cout << "�ṹ�������.����ȱ��ʶ�����к�Ϊ��" << line << endl; exit(0); }

			if (TYPEL.at(SYNBL.at(search_SYNBL(SEM.top().name)).type).tval != 'd'){ cout << "�ñ������ǽṹ�壡�к�Ϊ��" << line << endl; exit(0); }
			if (search_RINFL(SEM.top().name, token.name) == -1){ cout << "�ýṹ����û�и���к�Ϊ��" << line << endl; exit(0); }

			SEM.top().name += '.';
			SEM.top().name += token.name;
			Token Struct_Node = token;//�ݴ浱ǰ��ʶ�������ж��Ƿ�Ϊ�ṹ������
			NextToken();
			while (token.name == ".")
			{
				NextToken();
				if (token.type != 2){ cout << "�ṹ�������.����ȱ��ʶ�����к�Ϊ��" << line << endl; exit(0); }
			
				if (TYPEL.at(SYNBL.at(search_SYNBL(Struct_Node.name)).type).tval != 'd'){ cout << "�ñ������ǽṹ�壡�к�Ϊ��" << line << endl; exit(0); }
				if (search_RINFL(Struct_Node.name, token.name) == -1){ cout << "�ýṹ����û�и���к�Ϊ��" << line << endl; exit(0); }
				
				SEM.top().name += '.';
				SEM.top().name += token.name;
				Struct_Node = token;
			}
			if (token.name == "[")
			{

				if (TYPEL.at(SYNBL.at(search_SYNBL(Struct_Node.name)).type).tval != 'a'){ cout << "�ñ����������飡�к�Ϊ��" << line << endl; exit(0); }

				Ainel_item array_typel = AINEL.at(TYPEL.at(SYNBL.at(search_SYNBL(Struct_Node.name)).type).tpoint);
				string array_num = token.name;//�ݴ�������±�
				//SEM.pop();
				NextToken();
				if (IT_Type(token) != 3){ cout << "�﷨��������ʹ�������к�Ϊ��" << line << endl; exit(0); }
				
				if (token.type == 3 && (stoi(token.name) > array_typel.up || stoi(token.name) < array_typel.low))
				{
					cout << "����Խ�磡�к�Ϊ��" << line << endl; exit(0);
				}
				
				array_num += token.name;
				NextToken();

				if (TYPEL.at(array_typel.ctp).tval != 'a' && token.name == ","){ cout << "��������ά�ȣ��к�Ϊ��" << line << endl; exit(0); }
				array_typel = AINEL.at(TYPEL.at(array_typel.ctp).tpoint);

				while (token.name == ",")
				{
					array_num += token.name;
					NextToken();
					if (IT_Type(token) != 3){ cout << "�﷨��������ʹ�������к�Ϊ��" << line << endl; exit(0); }

					if (token.type == 3 && (stoi(token.name) > array_typel.up || stoi(token.name) < array_typel.low))
					{
						cout << "����Խ�磡�к�Ϊ��" << line << endl; exit(0);
					}

					array_num += token.name;
					NextToken();

					//�ж�����ά���Ƿ�Խ��

					if (TYPEL.at(array_typel.ctp).tval != 'a' && token.name == ","){ cout << "��������ά�ȣ��к�Ϊ��" << line << endl; exit(0); }
					//if (TYPEL.at(array_typel.ctp).tval == 'a' && token.name != ","){ cout << "��ȷ��Ҫʹ����������ֱ�ӽ���������Ҫ�����Ƿ���һ���� �к�Ϊ��" << line << endl; exit(0); }
					if (token.name == ",")
						array_typel = AINEL.at(TYPEL.at(array_typel.ctp).tpoint);

				}
				if (token.name != "]"){ cout << "�﷨��������ʹ������ȱ��]���к�Ϊ��" << line << endl; exit(0); }
				array_num += token.name;
				SEM.top().name += array_num;
				NextToken();
				
			}
			if (token.name == ":=")
			{
				NextToken();
				EXPRESSION();

				//������Ԫʽ
				Token t1 = SEM.top();
				SEM.pop();
				Token t2 = SEM.top();
				SEM.pop();
				//��ֵǰ����ƥ��
				if (IT_Type(t1) != IT_Type(t2)){ cout << "��ֵ��������������Ͳ�ƥ�䣡�к�Ϊ��" << line << endl; exit(0); }
				QT.push_back({ ":=", t1.name, "_", t2.name });
				//cout << "(" << QT.back().w << "," << QT.back().o1 << "," << QT.back().o2 << "," << QT.back().t << ")" << endl;
			}
			else { cout << "���ʽȱ�١���=�����к�Ϊ��" << line << endl; exit(0); }
		}
		else if (token.name == "[")
		{//����
			if (TYPEL.at(SYNBL.at(search_SYNBL(SEM.top().name)).type).tval != 'a'){ cout << "�ñ����������飡�к�Ϊ��" << line << endl; exit(0); }

			Ainel_item array_typel = AINEL.at(TYPEL.at(SYNBL.at(search_SYNBL(SEM.top().name)).type).tpoint);
			string array_num = token.name;//�ݴ�������±�
			//SEM.pop();
			NextToken();
			if (IT_Type(token) != 3){ cout << "�﷨��������ʹ�������к�Ϊ��" << line << endl; exit(0); }
			
			if (token.type == 3 && (stoi(token.name) > array_typel.up || stoi(token.name) < array_typel.low))
			{
				cout << "����Խ�磡�к�Ϊ��" << line << endl; exit(0);
			}
			
			array_num += token.name;
			NextToken();

			if (TYPEL.at(array_typel.ctp).tval != 'a' && token.name == ","){ cout << "��������ά�ȣ��к�Ϊ��" << line << endl; exit(0); }
			array_typel = AINEL.at(TYPEL.at(array_typel.ctp).tpoint);

			while (token.name == ",")
			{
				array_num += token.name;
				NextToken();
				if (IT_Type(token) != 3){ cout << "�﷨��������ʹ�������к�Ϊ��" << line << endl; exit(0); }

				if (token.type == 3 && (stoi(token.name) > array_typel.up || stoi(token.name) < array_typel.low))
				{
					cout << "����Խ�磡�к�Ϊ��" << line << endl; exit(0);
				}

				array_num += token.name;
				NextToken();

				//�ж�����ά���Ƿ�Խ��

				if (TYPEL.at(array_typel.ctp).tval != 'a' && token.name == ","){ cout << "��������ά�ȣ��к�Ϊ��" << line << endl; exit(0); }
				//if (TYPEL.at(array_typel.ctp).tval == 'a' && token.name != ","){ cout << "��ȷ��Ҫʹ����������ֱ�ӽ���������Ҫ�����Ƿ���һ���� �к�Ϊ��" << line << endl; exit(0); }
				if (token.name == ",")
					array_typel = AINEL.at(TYPEL.at(array_typel.ctp).tpoint);

			}

			if (token.name != "]"){ cout << "�﷨��������ʹ������ȱ��]���к�Ϊ��" << line << endl; exit(0); }
			array_num += token.name;
			SEM.top().name += array_num;
			NextToken();
			if (token.name != ":="){ cout << "�﷨���󣡱��ʽȱ��=���к�Ϊ��" << line << endl; exit(0); }
			NextToken();
			EXPRESSION();

			//������Ԫʽ
			Token t1 = SEM.top();
			SEM.pop();
			Token t2 = SEM.top();
			SEM.pop();
			//��ֵǰ����ƥ��
			if (IT_Type(t1) != IT_Type(t2)){ cout << "��ֵ��������������Ͳ�ƥ�䣡�к�Ϊ��" << line << endl; exit(0); }
			QT.push_back({ ":=", t1.name, "_", t2.name });
			//cout << "(" << QT.back().w << "," << QT.back().o1 << "," << QT.back().o2 << "," << QT.back().t << ")" << endl;
		}
		else  { cout << "���ʽ�﷨�����к�Ϊ��" << line << endl; exit(0); }
	}
	else if (SYNBL.at(search_SYNBL(token.name)).cat == 'f')
	{//��������
		NextToken();
		if (token.name != "("){ cout << "�﷨���󣡺�������ȱ���������к�Ϊ��" << line << endl; exit(0); }
		NextToken();
		
		PROCEDURE_VN();

		//��������ƥ���������
		if (Vn_Num != PFINEL.at(SYNBL.at(search_SYNBL(SEM.top().name)).addr).fn){ cout << "�������ò���������ƥ�䣡�к�Ϊ��" << line << endl; exit(0); }
		
		QT.push_back({ "call", SEM.top().name, to_string(Vn_Num), "_" });
		SEM.pop();
		Vn_Num = 0;//����������0
		if (token.name != ")"){ cout << "��������ȱ���������к�Ϊ��" << line << endl; exit(0); }
		NextToken();
	}
	else if (SYNBL.at(search_SYNBL(token.name)).cat == 'p')
	{//���̺�������
		NextToken();
		if (token.name != "("){ cout << "�﷨���󣡺�������ȱ���������к�Ϊ��" << line << endl; exit(0); }
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

		//��������ƥ���������
		if (Vn_Num != PFINEL.at(SYNBL.at(search_SYNBL(SEM.top().name)).addr).fn){ cout << "�������ò���������ƥ�䣡�к�Ϊ��" << line << endl; exit(0); }

		QT.push_back({ "endpro", SEM.top().name, "_",  "_" });
		SEM.pop();
		Vn_Num = 0;//����������0
		if (token.name != ")"){ cout << "��������ȱ���������к�Ϊ��" << line << endl; exit(0); }
		NextToken();
	}
	else  { cout << "�ñ�ʶ�����Ǳ���Ҳ���Ǻ��������ɲ������к�Ϊ��" << line << endl; exit(0); }
}

//�ж����
void CONDITION()
{
	EXPRESSION();
	if (token.name == ">=" || token.name == "<=" || token.name == "=" || token.name == "<" || token.name == ">")
	{
		string logic_operator = token.name;
		NextToken();
		EXPRESSION();
		//������Ԫʽ
		Token t1 = SEM.top();
		SEM.pop();
		Token t2 = SEM.top();
		SEM.pop();
		if (IT_Type(t1) != IT_Type(t2)){ cout << "�����������Ͳ�ƥ�䣡�к�Ϊ��" << line << endl; exit(0); }
		if (IT_Type(t1) != 3 && IT_Type(t1) != 4 && IT_Type(t1) != 6){ cout << "�ж���䲻֧�ֲ��������ͣ��к�Ϊ��" << line << endl; exit(0); }
		Tnum += to_string(num_T++);
		QT.push_back({ logic_operator, t2.name, t1.name, Tnum });
		//cout << "(" << QT.back().w << "," << QT.back().o1 << "," << QT.back().o2 << "," << QT.back().t << ")" << endl;
		SEM.push({ 2, Tnum });
		//���м����t������ű� ������
		SYNBL.push_back({ Tnum, 3, 'v', BOOL.size(), Level });
		BOOL.push_back(true);
		//Write_T_SYN(t1, t2);

		Tnum.erase(1, Tnum.size() - 1);
	}
}

//while���
void WHILE_SENTENCE()
{
	//����while��Ԫʽ
	QT.push_back({ "while", "_", "_", "_" });

	CONDITION();

	if (token.name != "do"){ cout << "if�����ȱ��then�����﷨�����к�Ϊ��" << line << endl; exit(0); }
	NextToken();

	//����while do��Ԫʽ
	QT.push_back({ "do", SEM.top().name, "_", "_" });
	SEM.pop();

	if (token.name != "begin"){ cout << "if�����ȱ��begin�����﷨�����к�Ϊ��" << line << endl; exit(0); }
	NextToken();


	SEN_SEQUENCE();
	if (token.name != "end"){ cout << "if�����ȱ��end�����﷨�����к�Ϊ��" << line << endl; exit(0); }

	//����while end��Ԫʽ
	QT.push_back({ "whileend", "_", "_", "_" });

	NextToken();
	if (token.name != ";"){ cout << "if�����end��ȱ��;�����﷨�����к�Ϊ��" << line << endl; exit(0); }
	NextToken();
}

//if���
void IF_SENTENCE()
{
	CONDITION();
	if (token.name != "then"){ cout << "if�����ȱ��then�����﷨�����к�Ϊ��" << line << endl; exit(0); }
	NextToken();
	if (token.name != "begin"){ cout << "if�����ȱ��begin�����﷨�����к�Ϊ��" << line << endl; exit(0); }
	NextToken();
	//����if��Ԫʽ
	QT.push_back({ "if", SEM.top().name, "_", "_" });
	SEM.pop();

	SEN_SEQUENCE();
	if (token.name == "else")
	{
		//����if else��Ԫʽ
		QT.push_back({ "else", "_", "_", "_" });

		NextToken();
		SEN_SEQUENCE();
		if (token.name != "end"){ cout << "if�����ȱ��end�����﷨�����к�Ϊ��" << line << endl; exit(0); }

		//����if end��Ԫʽ
		QT.push_back({ "ifend", "_", "_", "_" });

		NextToken();
		if (token.name != ";"){ cout << "if�����end��ȱ��;�����﷨�����к�Ϊ��" << line << endl; exit(0); }
		NextToken();
	}
	//else { cout << "if�����ȱ��else�����﷨�����к�Ϊ��" << line << endl; exit(0); }
	//if����в�һ����else

}

//�������
void SEN_SEQUENCE()
{
	if (token.type == 2)
	{
		if (search_SYNBL(token.name) == -1){ cout << token.name << "��ʶ��δ���壡�к�Ϊ��" << line << endl; exit(0); }
		EVA_SUBPROGRAM_OPTION();
		if (token.name == "end")return;
		if (token.name != ";"){ cout << "�﷨��������ȱ\";\"�к�Ϊ��" << line << endl; exit(0); }
		NextToken();
		SEN_SEQUENCE();
		/*ID_name = token.name;//���浱ǰ��ʶ�����ڸñ�ʶ��Ϊ����ʱʹ��
		//��ѯ��ǰ����ֵ�����Ƿ�������
		if (SYNBL.at(search_SYNBL(token.name)).cat == 'v')
		{//���ʽ
			SEM.push({ 2, ID_name});
			EVA_SUBPROGRAM_OPTION();

			//��EXPRESSION�Ľ��������������
			Token t1 = SEM.top();
			SEM.pop();
			Token t2 = SEM.top();
			SEM.pop();
			//��ֵǰ����ƥ��
			if (IT_Type(t1) != IT_Type(t2)){ cout << "��ֵ��������������Ͳ�ƥ�䣡�к�Ϊ��" << line << endl; exit(0); }
			QT.push_back({ ":=", t1.name, "\0", t2.name });
			//cout << "(" << QT.back().w << "," << QT.back().o1 << "," << QT.back().o2 << "," << QT.back().t << ")" << endl;
		}
		else if (SYNBL.at(search_SYNBL(token.name)).cat == 'f')
		{//��������
			EVA_SUBPROGRAM_OPTION();
			//������������,������δ����
			if (function_vn_num == 0)
				QT.push_back({ ID_name, "\0", "\0", "\0", });
			else { cout << "���������в������к�Ϊ��" << line << endl; exit(0); }

			ID_name.clear();//�������ý���
		}
	
		else{ cout << token.name << "û�б��������к�Ϊ��" << line << endl; exit(0); }
		
		if (search_SYNBL(token.name) == -1){ cout << token.name << "û�б��������к�Ϊ��" << line << endl; exit(0); }

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

//���
void COM_SENTENCE()
{
	if (token.name == "begin")
	{
		NextToken();

		//���볣���������Ԫʽ
		for (int i = 0; i < SYNBL.size(); i++)
			if (SYNBL.at(i).cat == 'c' && SYNBL.at(i).level <= Level)
				QT.push_back({":=", CONSL.at(SYNBL.at(i).addr).con, "_", SYNBL.at(i).name});
		


		SEN_SEQUENCE();
		if (token.name == "end")
		{ //���ɺ���������־��Ԫʽ
			QT.push_back({ "end", SEM.top().name, "_", "_" });
			SEM.pop();
			NextToken(); 
			return; 
		}
		else { cout << "ȱ��end���ؼ��֣��﷨�����к�Ϊ��" << line  << endl; exit(0); }
	}
	else { cout << "ȱ��begin���ؼ��֣��﷨�����к�Ϊ��" << line  << endl; exit(0); }
}

//��ʶ���б�
void ID_LIST()
{
	if (token.type == 2)
	{	//����ǰ�����ı����ݴ���ʱ���ű�
		if (search_SYNBL(token.name) == -1)
			Name_temp.push_back({ token.name, -1, '\0', -1, Level });//��ǰ��ʶ����ʼ��ʱtype��addr��Ϊ-1��catΪ��\0��
			
		else { cout << "�����ض��壡�к�Ϊ��" << line << endl; exit(0); }

		NextToken();
		while (token.name == ",")
		{
			NextToken();
			if (token.type == 2) 
			{
				//����ǰ�����ı���������ʱ���ű�
				//SYNBL.push_back({ token.name, -1, 'v', -1, Level });
				if (search_SYNBL(token.name) == -1)
					Name_temp.push_back({ token.name, -1, '\0', -1, Level });//��ǰ��ʶ����ʼ��ʱtype��addr��Ϊ-1��catΪ��\0��
					//����ǰ�����ı����ݴ���ʱ���ű�
				else { cout << "�����ض��壡�к�Ϊ��" << line << endl; exit(0); }
				NextToken();
			}
			else{ cout << "ȱ��ʶ�����﷨�����к�Ϊ��" << line << endl; exit(0); }
		}
	}
	else { cout << "ȱ��ʶ�����﷨�����к�Ϊ��" << line  << endl; exit(0); }

}

//��������
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
	{//��д������Ϣ
		if (search_SYNBL(token.name) != -1 && SYNBL.at(search_SYNBL(token.name)).cat == 't')
		{//��д������Ϣ
			AINEL.back().ctp = SYNBL.at(search_SYNBL(token.name)).type;
			AINEL.back().clen = LENL.at(SYNBL.at(search_SYNBL(token.name)).addr);
		}
		else { cout << "�������ʹ����﷨�����к�Ϊ��" << line << endl; exit(0); }
		NextToken();
	}
	else if (token.name == "array")
	{

		NextToken();
		if (token.name != "["){ cout << "���鶨��ȱ���\"[\"���﷨�����к�Ϊ��" << line << endl; exit(0); }
		NextToken();
		if (token.type != 5){ cout << "���鶨�壬ȱ���鷶Χ���﷨�����к�Ϊ��" << line << endl; exit(0); }
		//��д���ű����ͱ������

		string low = "\0", up = "\0";
		int subscript;
		for (subscript = 0; token.name[subscript] != '.'; subscript++)low += token.name[subscript];
		up = token.name.erase(0, subscript + 2);//������Ͻ磬��token���������޸ģ�
		up = token.name;

		AINEL.back().ctp = TYPEL.size();
		TYPEL.push_back({ 'a', AINEL.size() });
		AINEL.push_back({ stoi(low), stoi(up), -1, 0 });//������ctp��clen��ʼΪ-1��0 δ��

		//��д���ű����ͱ������δ��
		/*for (int i = 0; i < Name_temp.size(); i++)
		{
			Name_temp.at(i).cat = 't';
			Name_temp.at(i).type = TYPEL.size();
			Name_temp.at(i).addr = LENL.size();
			//TYPEL_temp.push_back({ 'a', AINEL.size() });
			//Name_temp.at(i).addr = INTEGER.size();
		}
		TYPEL.push_back({ 'a', AINEL.size() });
		AINEL.push_back({ stoi(low), stoi(up), -1, 0 });//������ctp��clen��ʼΪ-1��0 δ��
		LENL.push_back(0);//���ȱ�ĳ�ֵΪ0  δ��*/

		NextToken();
		if (token.name != "]"){ cout << "���鶨��ȱ���\"]\"���﷨�����к�Ϊ��" << line << endl; exit(0); }
		NextToken();
		if (token.name != "of"){ cout << "���鶨��ȱ�ؼ���\"of\"���﷨�����к�Ϊ��" << line << endl; exit(0); }
		NextToken();

		ARRAY_TYPE();
		//��д�����������Ϣ
		int ainel_num;
		for (ainel_num = AINEL.size() - 1; AINEL.at(ainel_num).clen != 0; ainel_num--);
		AINEL.at(ainel_num).clen = AINEL.at(ainel_num + 1).clen * (AINEL.at(ainel_num + 1).up - AINEL.at(ainel_num + 1).low + 1);//��д�����ĳ���


	}
	else { cout << "��������ȱ���ͣ��﷨�����к�Ϊ��" << line << endl; exit(0); }

}

//��������
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
	{//��д������Ϣ
		if (search_SYNBL(token.name) != -1 && SYNBL.at(search_SYNBL(token.name)).cat == 't')
		{//��д������Ϣ
			SYNBL.back().type = SYNBL.at(search_SYNBL(token.name)).type;
			LENL.push_back(LENL.at(SYNBL.at(search_SYNBL(token.name)).addr));

			NextToken();
		}
		else { cout << "�������ʹ����﷨�����к�Ϊ��" << line << endl; exit(0); }
	}
	else if (token.name == "array")
	{
		NextToken();
		if (token.name != "["){ cout << "���鶨��ȱ���\"[\"���﷨�����к�Ϊ��" << line << endl; exit(0); }
		NextToken();
		if (token.type != 5){ cout << "���鶨�壬ȱ���鷶Χ���﷨�����к�Ϊ��" << line << endl; exit(0); }

		string low = "\0", up = "\0";
		int subscript;
		for (subscript = 0; token.name[subscript] != '.'; subscript++)low += token.name[subscript];
		up = token.name.erase(0, subscript + 2);//������Ͻ磬��token���������޸ģ�
		up = token.name;

		//��д���ű����ͱ������
		SYNBL.back().type = TYPEL.size();
		SYNBL.back().addr = LENL.size();
		TYPEL.push_back({ 'a', AINEL.size() });
		AINEL.push_back({ stoi(low), stoi(up), -1, 0 });//������ctp��clen��ʼΪ-1��0 δ��
		LENL.push_back(0);//���ȱ�ĳ�ֵΪ0  δ��

		NextToken();
		if (token.name != "]"){ cout << "���鶨��ȱ���\"]\"���﷨�����к�Ϊ��" << line << endl; exit(0); }
		NextToken();
		if (token.name != "of"){ cout << "���鶨��ȱ�ؼ���\"of\"���﷨�����к�Ϊ��" << line << endl; exit(0); }
		NextToken();

		ARRAY_TYPE();
		//��д�����������Ϣ
		int i;
		for (i = SYNBL.size() - 1; i >= 0; i--)
		{
			if (SYNBL.at(i).cat == 't' && SYNBL.at(i).addr == LENL.size() - 1)
				break;
		}
		LENL.back() = AINEL.at(TYPEL.at(SYNBL.at(i).type).tpoint).clen * (AINEL.at(TYPEL.at(SYNBL.at(i).type).tpoint).up - AINEL.at(TYPEL.at(SYNBL.at(i).type).tpoint).low + 1);
	}
	else { cout << "��������ȱ���ͣ��﷨�����к�Ϊ��" << line << endl; exit(0); }
}

//����
void TYPE()
{
	if (token.name == "integer")
	{
		for (int i = 0; i < Name_temp.size(); i++)
		{
			Name_temp.at(i).type = 0;
			//Name_temp.at(i).addr = INTEGER.size();

		}
		//����ʱ���ű��е���������ű�
		//for (int i = 0; i < Name_temp.size(); i++)
		//{
		//	syn_temp.name = Name_temp.at(i);
		//	syn_temp.addr = INTEGER.size();
		//	INTEGER.push_back(0);//������ʼֵΪ0
		//	SYNBL.push_back(syn_temp);//��д���ű�
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
	{//��д������Ϣ
		if (search_SYNBL(token.name) != -1 && SYNBL.at(search_SYNBL(token.name)).cat == 't')
		{//��д������Ϣ
			for (int i = 0; i < Name_temp.size(); i++)
			{
				Name_temp.at(i).type = SYNBL.at(search_SYNBL(token.name)).type;
				Name_temp.at(i).addr = SYNBL.at(search_SYNBL(token.name)).addr;
				//Name_temp.at(i).addr = INTEGER.size();
			}
		}
		else { cout << "�������ʹ����﷨�����к�Ϊ��" << line << endl; exit(0); }
		NextToken();
	}
	else if (token.name == "array")
	{

		NextToken();
		if (token.name != "["){ cout << "���鶨��ȱ���\"[\"���﷨�����к�Ϊ��" << line << endl; exit(0); }
		NextToken();
		if (token.type != 5){ cout << "���鶨�壬ȱ���鷶Χ���﷨�����к�Ϊ��" << line << endl; exit(0); }
		//��д���ű����ͱ������

		string low = "\0", up = "\0";
		int subscript;
		for (subscript = 0; token.name[subscript] != '.'; subscript++)low += token.name[subscript];
		up = token.name.erase(0, subscript + 2);//������Ͻ磬��token���������޸ģ�
		up = token.name;
		//for (subscript += 2; token.name[subscript] != '.'; subscript++)up += token.name[subscript];
		//��д���ű����ͱ������δ��
		for (int i = 0; i < Name_temp.size(); i++)
		{
			Name_temp.at(i).type = TYPEL.size();
			Name_temp.at(i).addr = LENL.size();
			//TYPEL_temp.push_back({ 'a', AINEL.size() });
			//Name_temp.at(i).addr = INTEGER.size();
		}
		TYPEL.push_back({ 'a', AINEL.size() });
		AINEL.push_back({ stoi(low), stoi(up), -1, 0 });//������ctp��clen��ʼΪ-1��0 δ��
		LENL.push_back(0);//���ȱ�ĳ�ֵΪ0  δ��

		NextToken();
		if (token.name != "]"){ cout << "���鶨��ȱ���\"]\"���﷨�����к�Ϊ��" << line << endl; exit(0); }
		NextToken();
		if (token.name != "of"){ cout << "���鶨��ȱ�ؼ���\"of\"���﷨�����к�Ϊ��" << line << endl; exit(0); }
		NextToken();

		ARRAY_TYPE();
		//��д�����������Ϣ
		LENL.back() = AINEL.at(TYPEL.at(Name_temp.at(0).type).tpoint).clen * (AINEL.at(TYPEL.at(Name_temp.at(0).type).tpoint).up - AINEL.at(TYPEL.at(Name_temp.at(0).type).tpoint).low + 1);
	}
	else { cout << "��������ȱ���ͣ��﷨�����к�Ϊ��" << line << endl; exit(0); }

}

//�ӳ���Ĳ���
void ARGUMENT()
{
	if (token.type == 2)
	{//VF��ֵ�β� ����catΪ'e'
		function_vn_num++;//������������

		//��д���ű�Ͳ�����
		Name_temp.push_back({ token.name, -1, 'e', -1, Level });//���ű��ݴ� ����type����ַaddrδ�� 
		NextToken();
		if (token.name != ":"){ cout << "�Ӻ��������в�������ȱ\"��\" ���﷨�����к�Ϊ��" << line << endl; exit(0); }
		NextToken();

		TYPE();
		//��д���ű�Ͳ�����
		Type_Write_SYNBL();//��д���ű�
		//PARAMBL.push_back(SYNBL.back());//��д������

		for (int param_num = 0; param_num < Name_temp.size(); param_num++)
		{
			PARAMBL.push_back(Name_temp.at(param_num));//��д������
		}

		Name_temp.clear();
		if (token.name == ";")
		{
			NextToken();
			ARGUMENT();
		}
	}
	else if (token.name == "var")
	{//VN�����β�
		NextToken();
		//��д���ű�Ͳ�����
		if (token.type != 2){ cout << "�Ӻ���������ȱ��ʶ�����﷨�����к�Ϊ��" << line << endl; exit(0); }
		
		function_vn_num++;//������������
		
		Name_temp.push_back({ token.name, -1, 'n', -1, Level });//���ű��ݴ� ����type����ַaddrδ��
		NextToken();
		if (token.name != ":"){ cout << "�Ӻ��������в�������ȱ\"��\" ���﷨�����к�Ϊ��" << line << endl; exit(0); }
		NextToken();
		
		TYPE();
		//��д���ű�Ͳ�����
		Type_Write_SYNBL();//��д���ű�

		for (int param_num = 0; param_num < Name_temp.size(); param_num++)
		{
			PARAMBL.push_back(Name_temp.at(param_num));//��д������
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

//���̺����Ĳ���
void PARGUMENT()
{
	if (token.type == 2)
	{
		//NextToken();
		//��д���ű�Ͳ�����
		if (token.type != 2){ cout << "�Ӻ���������ȱ��ʶ�����﷨�����к�Ϊ��" << line << endl; exit(0); }

		function_vn_num++;//������������

		Name_temp.push_back({ token.name, -1, 'n', -1, Level });//���ű��ݴ� ����type����ַaddrδ��
		NextToken();
		if (token.name != ":"){ cout << "�Ӻ��������в�������ȱ\"��\" ���﷨�����к�Ϊ��" << line << endl; exit(0); }
		NextToken();

		TYPE();
		//��д���ű�Ͳ�����
		Type_Write_SYNBL();//��д���ű�

		for (int param_num = 0; param_num < Name_temp.size(); param_num++)
		{
			PARAMBL.push_back(Name_temp.at(param_num));//��д������
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

//�ӳ���return����
void SUBP_RETURN()
{
	if (token.name == ":")
	{
		NextToken();
		Name_temp.push_back(SYNBL.at(search_SYNBL(SEM.top().name)));
		TYPE();
		if (Name_temp.size() != 1 || Name_temp.at(0).cat != 'f'){ cout << "�ݴ���ű�û����գ��������޸�bug���к�Ϊ��" << line << endl; exit(0); }
		SYNBL.at(search_SYNBL(SEM.top().name)) = Name_temp.at(0);//��д������ �������� 
		Name_temp.clear();
	}
	else return;
}

//�ӳ��������return���Ͷ���
void SUBPROGRAM_HEAD()
{
	if (token.name != "("){ cout << "�Ӻ��������к�������ȱ\"��\" ���﷨�����к�Ϊ��" << line << endl; exit(0); }
	NextToken();
	ARGUMENT();
	PFINEL.at(SYNBL.at(search_SYNBL(SEM.top().name)).addr).fn = function_vn_num;//��д������Ĳ�������
	function_vn_num = 0;//����������0
	if (token.name != ")"){ cout << "�Ӻ��������в�����\"��\"��ȱ\")\" ���﷨�����к�Ϊ��" << line << endl; exit(0); }
	NextToken();
	SUBP_RETURN();
}

//���̺���head
void PSUBPROGRAM_HEAD()
{
	if (token.name != "("){ cout << "�Ӻ��������к�������ȱ\"��\" ���﷨�����к�Ϊ��" << line << endl; exit(0); }
	NextToken();
	PARGUMENT();
	PFINEL.at(SYNBL.at(search_SYNBL(SEM.top().name)).addr).fn = function_vn_num;//��д������Ĳ�������
	function_vn_num = 0;//����������0
	if (token.name != ")"){ cout << "�Ӻ��������в�����\"��\"��ȱ\")\" ���﷨�����к�Ϊ��" << line << endl; exit(0); }
	NextToken();
}

//�ӳ�����
void SUBPROGRAM_DECLARATION()
{
	SUBPROGRAM_HEAD();
	DECLARATION();
	COM_SENTENCE();
}

//���̺���
void PSUBPROGRAM_DECLARATION()
{
	PSUBPROGRAM_HEAD();
	DECLARATION();
	COM_SENTENCE();
}

//�ṹ�嶨������
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
	{//��д������Ϣ
		if (search_SYNBL(token.name) != -1 && SYNBL.at(search_SYNBL(token.name)).cat == 't')
		{//��д������Ϣ
			for (int i = 0; i < Name_temp.size(); i++)
			{
				Name_temp.at(i).cat = 'd';
				Name_temp.at(i).type = SYNBL.at(search_SYNBL(token.name)).type;
				Name_temp.at(i).addr = LENL.size();
				LENL.push_back(LENL.at(SYNBL.at(search_SYNBL(token.name)).addr));
			}
		}
		else { cout << "�������ʹ����﷨�����к�Ϊ��" << line << endl; exit(0); }
		NextToken();
	}
	else if (token.name == "array")
	{
		NextToken();
		if (token.name != "["){ cout << "���鶨��ȱ���\"[\"���﷨�����к�Ϊ��" << line << endl; exit(0); }
		NextToken();
		if (token.type != 5){ cout << "���鶨�壬ȱ���鷶Χ���﷨�����к�Ϊ��" << line << endl; exit(0); }
		//��д���ű����ͱ������

		string low = "\0", up = "\0";
		int subscript;
		for (subscript = 0; token.name[subscript] != '.'; subscript++)low += token.name[subscript];
		up = token.name.erase(0, subscript + 2);//������Ͻ磬��token���������޸ģ�
		up = token.name;
		//for (subscript += 2; token.name[subscript] != '.'; subscript++)up += token.name[subscript];
		//��д���ű����ͱ������δ��
		for (int i = 0; i < Name_temp.size(); i++)
		{
			Name_temp.at(i).cat = 'd';
			Name_temp.at(i).type = TYPEL.size();
			Name_temp.at(i).addr = LENL.size();
			//TYPEL_temp.push_back({ 'a', AINEL.size() });
			//Name_temp.at(i).addr = INTEGER.size();
		}
		TYPEL.push_back({ 'a', AINEL.size() });
		AINEL.push_back({ stoi(low), stoi(up), -1, 0 });//������ctp��clen��ʼΪ-1��0 δ��
		LENL.push_back(0);//���ȱ�ĳ�ֵΪ0  δ��

		NextToken();
		if (token.name != "]"){ cout << "���鶨��ȱ���\"]\"���﷨�����к�Ϊ��" << line << endl; exit(0); }
		NextToken();
		if (token.name != "of"){ cout << "���鶨��ȱ�ؼ���\"of\"���﷨�����к�Ϊ��" << line << endl; exit(0); }
		NextToken();

		ARRAY_TYPE();
		//��д�����������Ϣ
		LENL.back() = AINEL.at(TYPEL.at(Name_temp.at(0).type).tpoint).clen * (AINEL.at(TYPEL.at(Name_temp.at(0).type).tpoint).up - AINEL.at(TYPEL.at(Name_temp.at(0).type).tpoint).low + 1);
	}
	else { cout << "��������ȱ���ͣ��﷨�����к�Ϊ��" << line << endl; exit(0); }
}

//���Ͷ����б�
void TYPE_DECLARATION()
{
	ID_LIST();
	if (token.name == ":")
	{
		NextToken();
		RECORD_TYPE();
		//�������嶯�����ṹ���Ա������ű�
		
		for (int i = 0; i < Name_temp.size(); i++)
		{
			SYNBL.push_back(Name_temp.at(i));
			RINFL.push_back({ Name_temp.at(i).name, rinfl_off, Name_temp.at(i).type });
			rinfl_off += LENL.at(Name_temp.at(i).addr);
		}
		
		//if (i < 0) { cout << "�Ҳ����ṹ��ָ��ĳ��ȱ��к�Ϊ��" << line << endl; exit(0); }

		Name_temp.clear();

		if (token.name == ";")
		{
			NextToken();
			if (token.type == 2)TYPE_DECLARATION();
			else return;
		}
		else { cout << "�ṹ�嶨����ȱ��;�����﷨�����к�Ϊ��" << line << endl; exit(0); }
	}
	else { cout << "�ṹ�嶨���б�ʶ����ȱ��:�����﷨�����к�Ϊ��" << line << endl; exit(0); }
}

//���Ͷ���
void DECLARATION_TYPE()
{
	if (token.name == "record")
	{
		NextToken();
		SYNBL.back().type = TYPEL.size();
		TYPEL.push_back({ 'd', RINFL.size() });//��д�ṹ������ͱ�
		
		TYPE_DECLARATION();

		int i = LENL.size() - 1;
		for (; i >= 0; i--)
		{//��д�ṹ��ĳ��ȱ�
			if (LENL.at(i) == -1)
				LENL.at(i) = rinfl_off;
		}
		rinfl_off = 0;//�ṹ�����ݴ���0
		if (token.name == "end")
		{//��д�ṹ����ű�δ��
			NextToken();
			if (token.name == ";"){ NextToken(); return; }
			else { cout << "�ṹ�嶨��end��ȱ��;�����﷨�����к�Ϊ��" << line << endl; exit(0); }
		}
		else { cout << "�ṹ�嶨��ȱ��end�����﷨�����к�Ϊ��" << line << endl; exit(0); }
	}
	else
	{//���͸���
		//TYPEL.push_back({ 'd', RINFL.size() });//��д�ṹ������ͱ�

		DETYPE();//���Ͷ��岻��ȷ��δ��д���ͱ�

		if (token.name == ";"){ NextToken(); return; }
		else { cout << "���Ͷ���ȱ��;�����﷨�����к�Ϊ��" << line << endl; exit(0); }
	}
}

//�������庯��
void CONST_DECLARATION()
{
	if (token.type != 2){ cout << "�﷨���󣡳�������ȱ��ʶ��!�к�Ϊ��" << line << endl; exit(0); }
	if (search_SYNBL(token.name) != -1){ cout << "�ض��壡�к�Ϊ��" << line << endl; exit(0); }
	string const_name = token.name;

	//CONST_QT.push_back({ {":=", "\0", "_", token.name}, Level });//�ݴ泣��������Ԫʽ��������û�зŽ�ȥ

	NextToken();
	if (token.name == "=")
	{
		NextToken();

		if (token.type == 3 || token.type == 4 )//��д���ű�
		{//����ʵ��
			SYNBL.push_back({ const_name, token.type - 3, 'c', CONSL.size(), Level });
			CONSL.push_back({ token.type, token.name });
		}
		else if (token.type == 6)
		{//�ַ��ַ���
			SYNBL.push_back({ const_name, 2, 'c', CONSL.size(), Level });
			CONSL.push_back({ token.type, token.name });
		}
		else if (token.type == 7)
		{//�ַ���
			SYNBL.push_back({ const_name, 4, 'c', CONSL.size(), Level });
			CONSL.push_back({ token.type, token.name });
		}
		else { cout << "���󣡳�������ȱ�ٳ������к�Ϊ��" << line << endl; exit(0); }
		
		NextToken();
		if (token.name == ";")
		{
			NextToken();
			if (token.type == 2)CONST_DECLARATION();
		}
		else { cout << "�﷨���󣡳�������ȱ��\";\"���к�Ϊ��" << line << endl; exit(0); }
	}
	else { cout << "�﷨���󣡳�������ȱ��\"=\"! �к�Ϊ��" << line << endl; exit(0); }
}

//��д���ű�
void Type_Write_SYNBL()
{
	for (int i = 0; i < Name_temp.size(); i++)
	{
		if (TYPEL.at(Name_temp.at(i).type).tval == 'i')
		{
			Name_temp.at(i).addr = INTEGER.size();
			INTEGER.push_back(0);//������ʼ��Ϊ0
			SYNBL.push_back(Name_temp.at(i));
		}
		else if (TYPEL.at(Name_temp.at(i).type).tval == 'r')
		{
			Name_temp.at(i).addr = REAL.size();
			REAL.push_back(0.0);//������ʼ��Ϊ0.0
			SYNBL.push_back(Name_temp.at(i));
		}
		else if (TYPEL.at(Name_temp.at(i).type).tval == 'c')
		{
			Name_temp.at(i).addr = CHAR.size();
			CHAR.push_back('\0');//������ʼ��Ϊ'\0'
			SYNBL.push_back(Name_temp.at(i));
		}
		else if (TYPEL.at(Name_temp.at(i).type).tval == 'b')
		{
			Name_temp.at(i).addr = BOOL.size();
			BOOL.push_back(true);//������ʼ��Ϊtrue
			SYNBL.push_back(Name_temp.at(i));
		}
		else if (TYPEL.at(Name_temp.at(i).type).tval == 'a')
		{
			SYNBL.push_back(Name_temp.at(i));
		}
		else if (TYPEL.at(Name_temp.at(i).type).tval == 'd')
		{//����ṹ�����ͱ���������д���ȱ�
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

			//LENL.push_back(0);//�ṹ�峤�ȱ�δ��*/
			SYNBL.push_back(Name_temp.at(i));
		}
		else { cout << "���ͱ������������к�Ϊ��" << line << endl; exit(0); }
	}
	//Name_temp.clear();
}

//�������庯��
void VAR_DECLARATION()
{
	ID_LIST();
	if (token.name == ":")
	{
		NextToken();
		TYPE();
		//��д���ű�
		for (int i = 0; i < Name_temp.size(); i++)
		{
			Name_temp.at(i).cat = 'v';
		}

		Type_Write_SYNBL();//��д���ű�
		Name_temp.clear();
		if (token.name == ";")
		{
			NextToken();
			if(token.type == 2)VAR_DECLARATION();
		}
		else { cout << "�﷨���󣡱�������ȱ��\";\"���к�Ϊ��" << line << endl; exit(0); }
	}
	else { cout << "�﷨���󣡱�������ȱ��\":\"! �к�Ϊ��" << line << endl; exit(0);}
}

//�������弰��������
void DECLARATION()
{
	if (token.name == "type")
	{//�ṹ�壬���ű�δ��
		NextToken();
		if (token.type == 2)
		{
			if (search_SYNBL(token.name) == -1)
			{
				//������������ű�

				//Name_temp.push_back(token.name);
				SYNBL.push_back({ token.name, -1, 't', LENL.size(), Level });//��д���͵ķ��ű�
				//LENL.push_back(-1);
				//��ǰ�ṹ�峤�ȳ�ʼΪ0
				//TYPEL.push_back({ 'd', AINEL.size() });
				//AINEL.push_back();

				NextToken();
				if (token.name == "=")
				{
					NextToken();
					DECLARATION_TYPE();

					DECLARATION();
				
				}
				else { cout << "�﷨����type��������к�Ϊ��" << line << endl; exit(0); }
			}
			else { cout << "�����ض��壡�к�Ϊ��" << line << endl; exit(0); }
		}
		else { cout << "��������ȱ�ٱ�ʶ�����к�Ϊ��" << line << endl; exit(0); }
	}
	else if (token.name == "var")
	{//�������壬���ű�
		NextToken();

		VAR_DECLARATION();
		DECLARATION();
	}
	else if (token.name == "const")
	{//�������壬���ű�

		NextToken();

		CONST_DECLARATION();
		DECLARATION();
	}
	else if (token.name == "function")
	{//�������壬���ű�δ��
		NextToken();
		if (token.type == 2)
		{
			Level++;//������μ�һ
			SEM.push(token);//ѹջ
			SYNBL.push_back({ token.name, -1, 'f', PFINEL.size(), Level });//����type������ֵδ��
			PFINEL.push_back({ Level, 3, 0, token.name, PARAMBL.size() });//��������δ��
			
			QT.push_back({ "function", token.name, "_", "_" });

			NextToken();
			SUBPROGRAM_DECLARATION();
			if (token.name != ";"){ cout << "�Ӻ�������ȱ���������к�Ϊ��" << line << endl; exit(0); }
			NextToken();
			Level--;//������μ�һ
		}
		else { cout << "��������ȱ�ٱ�ʶ�����к�Ϊ��" << line << endl; exit(0); }
	}
	else if (token.name == "procedure")
	{//���̺������壬���ű�δ��
		NextToken();
		if (token.type == 2)
		{
			Level++;//������μ�һ
			SEM.push(token);//ѹջ
			SYNBL.push_back({ token.name, -1, 'p', PFINEL.size(), Level });//����type������ֵδ��
			PFINEL.push_back({ Level, 3, 0, token.name, PARAMBL.size() });//��������δ��

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

			if (token.name != ";"){ cout << "�Ӻ�������ȱ���������к�Ϊ��" << line << endl; exit(0); }
			NextToken();
			Level--;//������μ�һ
		}
		else { cout << "��������ȱ�ٱ�ʶ�����к�Ϊ��" << line << endl; exit(0); }
	}
	else return;
}

//��������
void SUB_PROGRAM()
{
	DECLARATION();
	COM_SENTENCE();
}

//������
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
			//����ǰ������������ű�
			SYNBL.push_back({token.name, -1, 'p', -1, Level}); 
			//����������ѹ����Ԫʽ��
			QT.push_back({ "program", token.name, "_", "_" });
			//cout << "(" << QT.back().w << "," << QT.back().o1 << "," << QT.back().o2 << "," << QT.back().t << ")" << endl;

			NextToken();
			SUB_PROGRAM();
			if (token.name == "." || fileStream.endFlag == 1)
			{ 
				//�ж�����ջSEMջ��
				if (!SEM.empty()){ cout << "����ջSEM���գ�" << endl; exit(0); }

				//cout << "�﷨��ȷ��" << endl; 
				return; 
			}
			else { cout << "ȱ��.�����﷨�����к�Ϊ��" << line  << endl; exit(0); }
		}
		else { cout << "ȱ��ʶ�����﷨�����к�Ϊ��" << line  << endl; exit(0); }
	}
	else { cout << "ȱ��program�����﷨�����к�Ϊ��" << line  << endl; exit(0); }
}

//�����ַ�����
void outspace(int i)
{
	for (; i > 1; i--)
		cout << "?,";
}

//��Ŀ������з����ַ
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
