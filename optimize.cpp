#include "Table.h"

extern vector<QT_item>QT;

vector<QT_item>QT_1;

DagNode *dag[100];           //100��ָ��DagNode�ṹ ָ��  ָ������
VarNode var[100];
int dagNum, varNum;          //�ڵ�����������
//ifstream fin;
//ofstream fout;             //�ļ����룬�ļ����
string str1, str2, str3;
string opt, opt2;                    //�����Ų�����
bool BisNew, CisNew;

bool strEqual(string str1, string str2)         //�ж��ַ����Ƿ����
{
	int i, l;
	l = str1.length();
	i = str2.length();
	if (l != i) return false;
	for (i = 0; i<l; i++)
	if (str1[i] != str2[i]) return false;
	return true;
}
bool isCOpt(string ch)            //�ж��Ƿ������������
{
	string copt = "+,-,*,/";
	size_t found = copt.find(ch);
	if (found == string::npos)
		return false;
	else
		return true;

}
bool isBOpt(string ch2)
{
	string bopt = ">,<,=,<>,<=,>=";
	size_t found = bopt.find(ch2);
	if (found == string::npos)
		return false;
	else
		return true;
}

double Calc(double a, string op, double b)       //�������ʽ����
{
	if (strEqual(op, "+"))return a + b;
	if (strEqual(op, "-"))return a - b;
	if (strEqual(op, "*"))return a * b;
	if (strEqual(op, "/"))return a / b;
	else return 0;
}
bool Bcalc(double a, string op, double b) {
	if (strEqual(op, "=")) return (a == b);
	if (strEqual(op, "<>")) return !(a == b);
	if (strEqual(op, ">")) return (a>b);
	if (strEqual(op, "<")) return (a < b);
	if (strEqual(op, ">=")) return (a >= b);
	if (strEqual(op, "<=")) return (a <= b);
	else return false;
}
int findVar(string str)               //���ұ��������Ƿ��Ѿ�������������Ѿ�����������򷵻ؽ���ţ������򷵻�0
{
	double k;
	int i;
	char s[100];
	string ss;
	ss = str;
	if (str[0] == 43 || str[0] == 45) {
		if (str.length() > 0 && str[1] >= 48 && str[1] <= 57)
		{
			s[0] = str[0];
			for (i = 1; i < str.length(); i++)
				s[i] = str[i];
			s[i] = '\0';
			k = atof(s);
			sprintf_s(s, "%lf", k);              //�����ݸ�ʽ��������ַ��� ��sprintf �İ�ȫ�汾 ��kת����һ���ַ���������s��
			ss = "";
			for (int i = 0; i < strlen(s); i++)
				ss += s[i];
		}
	}
	if (str.length()>0 && str[0] >= 48 && str[0] <= 57)                //why 48 &57 str[0]�б������ʲô�أ�
	{
		for (i = 0; i<str.length(); i++)
			s[i] = str[i];
		s[i] = '\0';
		k = atof(s);
		sprintf_s(s, "%lf", k);              //�����ݸ�ʽ��������ַ��� ��sprintf �İ�ȫ�汾 ��kת����һ���ַ���������s��
		ss = "";
		for (int i = 0; i<strlen(s); i++)
			ss += s[i];
	}
	for (i = 1; i <= varNum; i++)            //varNum��what  �ڵ�����Ŀ?? ����dagͼ�еı������ܺ�
	{
		if (strEqual(ss, var[i].str))
			return i;                       //�ҵ��򷵻ؽ����
	}
	return 0;
}

void work4(int n)           //�����Ƿ��������str1������������Ƚ��������ɾ�������û�ж������str1���������ڱ��Ϊn��dagnode   
{
	int a, k;
	int i;

	a = findVar(str1);                   //�����Ƿ�����ͬ���� str1��Ԫʽ�����еĽ��
	if (a == 0)
	{
		varNum++;
		a = varNum;
		var[a].str = str1;
		var[a].flagNode = n;             //����a�����ڽ��n
		dag[n]->flagNum++;              //n���������ڵ���++        
		dag[n]->flags[dag[n]->flagNum] = str1;
	}
	else
	{
		k = var[a].flagNode;                //�����������Ľ���
		if (!dag[k]->isLeaf)               //�������Ľ�㲻��Ҷ�ӽ��
		{
			for (i = 1; i <= dag[k]->flagNum; i++)
			if (strEqual(str1, dag[k]->flags[i]))
			{
				for (int j = i + 1; j <= dag[k]->flagNum; j++)
					dag[k]->flags[j - 1] = dag[k]->flags[j];    //���ҵ�����ԭ�����ڵ�λ�ò�ɾ��
				break;
			}
			dag[k]->flagNum--;
		}
		var[a].flagNode = n;
		dag[n]->flagNum++;
		dag[n]->flags[dag[n]->flagNum] = str1;
	}
}

void work32(int b, int c)                  //˫����������                
{
	int i, k1, n, k2;
	k1 = var[b].flagNode;
	k2 = var[c].flagNode;
	n = 0;
	for (i = 1; i <= dagNum; i++)              //�����Ƿ�����ͬ�������Ѿ��������
	if (strEqual(dag[i]->opt, opt) && dag[i]->LChild == k1 && dag[i]->RChild == k2)
	{
		n = i;
		break;
	}
	if (n == 0)
	{
		dagNum++;
		n = dagNum;
		dag[n] = new DagNode();
		dag[n]->LChild = k1;
		dag[n]->RChild = k2;
		dag[k1]->fatherNum++;
		dag[k2]->fatherNum++;
		dag[n]->opt = opt;
	}
	work4(n);
}

void work25(int b, int c)           //���������ıȽ�����                               
{
	int i, k1, n, k2;
	k1 = var[b].flagNode;
	k2 = var[c].flagNode;
	n = 0;
	for (i = 1; i <= dagNum; i++)              //�����Ƿ�����ͬ�������Ѿ��������
	if (strEqual(dag[i]->opt, opt2) && dag[i]->LChild == k1 && dag[i]->RChild == k2)
	{
		n = i;
		break;
	}
	if (n == 0)
	{
		dagNum++;
		n = dagNum;
		dag[n] = new DagNode();
		dag[n]->LChild = k1;
		dag[n]->RChild = k2;
		dag[k1]->fatherNum++;
		dag[k2]->fatherNum++;
		dag[n]->opt = opt2;
	}
	work4(n);
}


void work31(int b)                           //ֻ��һ������������������ʱ�򣬰Ѻ��ӷ����Ҳ�
{
	int i, k, n;
	k = var[b].flagNode;
	n = 0;
	for (i = 1; i <= dagNum; i++)
	if (dag[i]->opt == opt && dag[i]->isSingle && dag[i]->RChild == k)
	{
		n = i;
		break;
	}
	if (n == 0)
	{
		dagNum++;
		n = dagNum;
		dag[n] = new DagNode();
		dag[n]->isSingle = true;
		dag[n]->LChild = k;
		dag[k]->fatherNum++;
		dag[n]->opt = opt;
	}
	work4(n);

}

void work26(int b, int c)                  //���������ıȽ�����
{
	bool p;
	int k;
	int i;
	char s[100];
	string str;
	p = Bcalc(dag[var[b].flagNode]->value, opt2, dag[var[c].flagNode]->value);
	if (CisNew)                          //�ڶ���������֮ǰû�б���������򽫽��ɾ����ֻ�����������Ľ��
	{
		varNum--;
		delete(dag[dagNum]);
		dagNum--;
	}
	if (BisNew)                         //��һ��������֮ǰû�б���������򽫽��ɾ��
	{
		varNum--;
		delete(dag[dagNum]);
		dagNum--;
	}
	if (p)
		str = "true";
	else str = "false";
	dagNum++;
	varNum++;
	k = varNum;
	var[k].str = str;
	var[k].flagNode = dagNum;
	dag[dagNum] = new DagNode();
	//dag[dagNum]->isVal = true;
	//dag[dagNum]->value = p;
	dag[dagNum]->flags[1] = str;
	dag[dagNum]->flagNum = 1;
	dag[dagNum]->isLeaf = true;
	work4(var[k].flagNode);
}





void work24(int b, int c)                  //���������ļ���
{
	double p;
	int k;
	int i;
	char s[100];
	string str;
	p = Calc(dag[var[b].flagNode]->value, opt, dag[var[c].flagNode]->value);
	if (CisNew)                          //�ڶ���������֮ǰû�б���������򽫽��ɾ����ֻ�����������Ľ��
	{
		varNum--;
		delete(dag[dagNum]);
		dagNum--;
	}
	if (BisNew)                  //��һ��������֮ǰû�б���������򽫽��ɾ��
	{
		varNum--;
		delete(dag[dagNum]);
		dagNum--;
	}
	sprintf_s(s, "%lf", p);      //�����ݸ�ʽ��������ַ��� ��sprintf �İ�ȫ�汾  ��kת����һ���ַ���������s��
	str = "";
	for (i = 0; i<strlen(s); i++)
		str += s[i];
	k = findVar(str);
	if (k == 0)
	{
		dagNum++;
		varNum++;
		k = varNum;
		var[k].str = str;
		var[k].flagNode = dagNum;
		dag[dagNum] = new DagNode();
		dag[dagNum]->isVal = true;
		dag[dagNum]->value = p;
		dag[dagNum]->flags[1] = str;      //����ʱflag[0]=0
		dag[dagNum]->flagNum = 1;
		dag[dagNum]->isLeaf = true;
	}
	work4(var[k].flagNode);
}

void work23(int b)                    //����������ֵ
{
	double p;
	int k;
	int i;
	char s[100];
	string str;
	p = Calc(0, opt, dag[var[b].flagNode]->value);
	if (BisNew)
	{
		varNum--;
		delete(dag[dagNum]);
		dagNum--;
	}
	sprintf_s(s, "%lf", p);
	str = "";
	for (i = 0; i<strlen(s); i++)
		str += s[i];
	k = findVar(str);
	if (k == 0)
	{
		dagNum++;
		varNum++;
		k = varNum;
		var[k].str = str;
		var[k].flagNode = dagNum;
		dag[dagNum] = new DagNode();
		dag[dagNum]->isVal = true;
		dag[dagNum]->value = p;
		dag[dagNum]->flags[1] = str;     //flag[0]�޶���
		dag[dagNum]->flagNum = 1;
		dag[dagNum]->isLeaf = true;
	}
	work4(var[k].flagNode);
}


void work22(int b, int c)              //��������������ʱ���ǳ������㻹�Ǻ��б���������
{
	if (dag[var[c].flagNode]->isVal && dag[var[b].flagNode]->isVal)
		work24(b, c);
	else
		work32(b, c);
}
void work33(int b, int c)              //��������������ʱ���ǳ������㻹�Ǻ��б���������
{
	if (dag[var[c].flagNode]->isVal && dag[var[b].flagNode]->isVal)
		work26(b, c);
	else
		work25(b, c);
}


void work21(int b)                       //���ֵ�������ʱ���Ǳ������㻹�ǳ�������
{
	if (dag[var[b].flagNode]->isVal)
		work23(b);
	else
		work31(b);
}


void work1(int sta, int fin)
{
	varNum = 0;
	dagNum = 0;
	string str;
	int  b, c;
	int ListType;

	char s[100];
	int i;
	int nn = 0;
	for (nn = sta; nn<fin; nn++)
	{
		str2 = QT[nn].o1;             //����һ����������������str2��
		str3 = QT[nn].o2;
		str1 = QT[nn].t;              //������������str1��

		if (strEqual(QT[nn].w, ":="))
		{
			ListType = 0;            //���磨��=��3.14��_��T0����T0��=3.14
		}
		if (isCOpt(QT[nn].w))     //������������

		{

			opt = QT[nn].w;
			if (strEqual(str3, "_"))
				ListType = 1;
			else
				ListType = 2;

		}
		if (isBOpt(QT[nn].w))
		{
			opt2 = QT[nn].w;
			ListType = 3;
		}

		b = findVar(str2);          //���ҵڶ����������Ƿ��Ѿ��������
		BisNew = false;
		if (b == 0)
		{
			BisNew = true;               //���ڶ���������û�б������BisNewΪtrue
			dagNum++;
			varNum++;
			b = varNum;
			var[b].str = str2;
			var[b].flagNode = dagNum;
			dag[dagNum] = new DagNode();
			if (str2[0] == 45 || str2[0] == 43) {
				s[0] = str2[0];
				if (str2[1] >= 48 && str2[1] <= 57)          //ASCII����ж�Ӧ0-9
				{
					dag[dagNum]->isVal = true;               //isVal ��ʾ�ǳ���
					for (i = 1; i < str2.length(); i++)
						s[i] = str2[i];
					s[i] = '\0';
					dag[dagNum]->value = atof(s);
				}
			}
			if (str2[0] >= 48 && str2[0] <= 57)          //ASCII����ж�Ӧ0-9
			{
				dag[dagNum]->isVal = true;               //isVal ��ʾ�ǳ���
				for (i = 0; i<str2.length(); i++)
					s[i] = str2[i];
				s[i] = '\0';
				dag[dagNum]->value = atof(s);
			}
			dag[dagNum]->flags[1] = str2;
			dag[dagNum]->flagNum = 1;
			dag[dagNum]->isLeaf = true;      //����ΪҶ�ӽ��
		}
		switch (ListType)
		{
		case 0: {
					work4(var[b].flagNode);
		}
			break;
		case 1: work21(b);
			break;

		case 2:
		{
				  c = findVar(str3);
				  CisNew = false;
				  if (c == 0)
				  {
					  CisNew = true;               //û���ҵ��͵�������������ͬ����ֵʱ��CisNew=true
					  dagNum++;
					  varNum++;
					  c = varNum;
					  var[c].str = str3;
					  var[c].flagNode = dagNum;
					  dag[dagNum] = new DagNode();
					  if (str3[0] == 43 || str3[0] == 45) {
						  if (str3[1] >= 48 && str3[1] <= 57)
						  {
							  s[0] = str3[0];
							  dag[dagNum]->isVal = true;
							  for (i = 1; i < str3.length(); i++)
								  s[i] = str3[i];
							  s[i] = '\0';
							  dag[dagNum]->value = atof(s);
						  }
					  }
					  if (str3[0] >= 48 && str3[0] <= 57)
					  {
						  dag[dagNum]->isVal = true;
						  for (i = 0; i<str3.length(); i++)
							  s[i] = str3[i];
						  s[i] = '\0';
						  dag[dagNum]->value = atof(s);
					  }
					  dag[dagNum]->flags[1] = str3;
					  dag[dagNum]->flagNum = 1;
					  dag[dagNum]->isLeaf = true;
				  }
				  work22(b, c);
				  break;
		}

		case 3:
		{
				  c = findVar(str3);
				  CisNew = false;
				  if (c == 0)
				  {
					  CisNew = true;               //û���ҵ��͵�������������ͬ����ֵʱ��CisNew=true
					  dagNum++;
					  varNum++;
					  c = varNum;
					  var[c].str = str3;
					  var[c].flagNode = dagNum;
					  dag[dagNum] = new DagNode();
					  if (str3[0] == 43 || str3[0] == 45) {
						  if (str3[1] >= 48 && str3[1] <= 57)
						  {
							  s[0] = str3[0];
							  dag[dagNum]->isVal = true;
							  for (i = 1; i < str3.length(); i++)
								  s[i] = str3[i];
							  s[i] = '\0';
							  dag[dagNum]->value = atof(s);
						  }
					  }
					  if (str3[0] >= 48 && str3[0] <= 57)
					  {
						  dag[dagNum]->isVal = true;
						  for (i = 0; i<str3.length(); i++)
							  s[i] = str3[i];
						  s[i] = '\0';
						  dag[dagNum]->value = atof(s);
					  }
					  dag[dagNum]->flags[1] = str3;
					  dag[dagNum]->flagNum = 1;
					  dag[dagNum]->isLeaf = true;
				  }
				  work33(b, c);       //���������������ļ���
				  break;
		}
		}

	}
}
void QTpush() {
	int k = 0;
	int i, j;
	string str;
	string comp = "t0 t1 t2 t3 t4 t5 t6 t7 t8 t9 t10 t11 t12 t13 t14 t15 t16";
	for (i = 1; i <= dagNum; i++)
	{
		size_t found1 = comp.find(dag[i]->flags[1]);
		if (found1 != string::npos) {
			for (int j = 2; j <= dag[i]->flagNum; j++)    //�����һ������ʱ������������渽�ӽ����ȫ�֣��򻥻�
			{
				size_t found2 = comp.find(dag[i]->flags[j]);
				if (found2 == string::npos) {
					string t = dag[i]->flags[1];
					dag[i]->flags[1] = dag[i]->flags[j];
					dag[i]->flags[j] = t;
					break;
				}
			}
		}
	}
	for (i = 1; i <= dagNum; i++) {
		if (dag[i]->isLeaf)
		{
			if (dag[i]->flagNum > 1)
			{
				for (j = 2; j <= dag[i]->flagNum; j++)
				{
					size_t found = comp.find(dag[i]->flags[j]);
					if (found == string::npos)
						QT_1.push_back({ ":=", dag[i]->flags[1], "_", dag[i]->flags[j] });


				}
			}
		}
		else
		{

			if (!dag[i]->isSingle)
			{

				QT_1.push_back({ dag[i]->opt, dag[dag[i]->LChild]->flags[1], dag[dag[i]->RChild]->flags[1], dag[i]->flags[1] });

			}

			else
			{
				QT_1.push_back({ dag[i]->opt, dag[dag[i]->LChild]->flags[1], "_", dag[i]->flags[1] });

			}

			for (j = 2; j <= dag[i]->flagNum; j++)
			{

				size_t found = comp.find(dag[i]->flags[j]);
				if (found == string::npos)

					QT_1.push_back({ ":=", dag[i]->flags[1], "_", dag[i]->flags[j] });



			}
		}
	}

}
int optimize()
{

	int count, x;            //count����while���ѭ����x�����ڲ�forѭ��
	int ss = 0;
	int ee = 0;              //ss��ģ��Ŀ�ʼ��ee��ģ��Ľ�������һ����Ԫ��

	string funcn = "";
	count = x = 0;
	string last = "gt,else,end,while";
	string first = "lb";
	string doif = "do,if";
	for (int x = count; x< QT.size(); x++) {
		ss = ee;
		size_t find1 = last.find(QT[x].w);
		size_t find2 = first.find(QT[x].w);
		//size_t find3 = funcn.find(QT[x].w);
		size_t find4 = doif.find(QT[x].w);


		if (strEqual(QT[x].w, "program"))
		{
			ss = x + 1;

			QT_1.push_back({ QT[x].w, QT[x].o1, QT[x].o2, QT[x].t });
			continue;
		}

		if (strEqual(QT[x].w, "function"))
		{
			ee = x;             //lb��һ��ģ��Ŀ�ʼ����һ��ģ��Ľ���
			funcn += QT[x].o1;     //����������function������
			work1(ss, ee);      //ee���ᱻִ��
			QTpush();
			//	QT_1.push_back({ " ", " ", " ", " " });
			QT_1.push_back({ QT[x].w, QT[x].o1, QT[x].o2, QT[x].t });
			count = x + 1;
			continue;
		}
		if (strEqual(QT[x].w, "call")) {
			int z = 1;
			size_t find3 = funcn.find(QT[x - z].w);
			while (find3 != string::npos)
			{
				z++;
				find3 = funcn.find(QT[x - z].w);
			}
			z--;
			ee = x - z;
			work1(ss, ee);
			QTpush();
			for (int j = x - z; j <= x; j++) {
				QT_1.push_back({ QT[j].w, QT[j].o1, QT[j].o2, QT[j].t });
			}
			//	QT_1.push_back({ " ", " ", " ", " " });
			count = x + 1;
			ee = x + 1;
			continue;
		}
		//if (strEqual(QT[x].w, "while")) {
		//	ee = x;               //if�ڱ�ģ���У���Ϊ��ģ��Ľ���
		//	work1(ss, ee);
		//	QTpush();
		//	QT_1.push_back({ QT[x].w,QT[x].o1,QT[x].o2,QT[x].t });
		//	//	QT_1.push_back({ " ", " ", " ", " " });
		//	count = x + 1;
		//	ee = x + 1;
		//	continue;
		//}

		if (find1 != string::npos) {
			ee = x;            //if�ڱ�ģ���У���Ϊ��ģ��Ľ���
			work1(ss, ee);
			QTpush();
			QT_1.push_back({ QT[x].w, QT[x].o1, QT[x].o2, QT[x].t });
			//	QT_1.push_back({ " ", " ", " ", " " });
			count = x + 1;
			ee = x + 1;
			continue;
		}
		if (strEqual(QT[x].w, "ifend")) {
			ee = x;            //if�ڱ�ģ���У���Ϊ��ģ��Ľ���
			work1(ss, ee);
			QTpush();
			QT_1.push_back({ QT[x].w, QT[x].o1, QT[x].o2, QT[x].t });
			//	QT_1.push_back({ " ", " ", " ", " " });
			count = x + 1;
			ee = x + 1;
			continue;
		}
		if (strEqual(QT[x].w, "whileend")) {
			ee = x;            //if�ڱ�ģ���У���Ϊ��ģ��Ľ���
			work1(ss, ee);
			QTpush();
			QT_1.push_back({ QT[x].w, QT[x].o1, QT[x].o2, QT[x].t });
			//	QT_1.push_back({ " ", " ", " ", " " });
			count = x + 1;
			ee = x + 1;
			continue;
		}
		if (find4 != string::npos) {
			ee = x;            //if�ڱ�ģ���У���Ϊ��ģ��Ľ���
			work1(ss, ee);
			QTpush();
			int zz = 0;
			for (int j = 1; j <= varNum; j++) {
				if (strEqual(QT[x].o1, var[j].str))
					zz = j;
			}
			if (zz)
				QT_1.push_back({ QT[x].w, dag[var[zz].flagNode]->flags[1], QT[x].o2, QT[x].t });

			else

				QT_1.push_back({ QT[x].w, QT[x].o1, QT[x].o2, QT[x].t });

			count = x + 1;
			ee = x + 1;
			continue;
		}
		if (find2 != string::npos) {
			ee = x;             //lb��һ��ģ��Ŀ�ʼ����һ��ģ��Ľ���
			work1(ss, ee);      //ee���ᱻִ��
			QTpush();
			//	QT_1.push_back({ " ", " ", " ", " " });
			QT_1.push_back({ QT[x].w, QT[x].o1, QT[x].o2, QT[x].t });
			count = x + 1;
			continue;

		}
	}



	/*QT.clear();
	//����ɾ������ԭ���Ĵ���

	QT.assign(QT_1.begin(), QT_1.end());

	for (int i = 0; i < QT.size(); i++)
	{
		cout << '(' << QT[i].w.c_str() << ',' << QT[i].o1.c_str() << ',' << QT[i].o2.c_str() << ',' << QT[i].t.c_str() << ')' << endl;
	}                            //���������

	//cout << funcn.c_str();*/

	return 0;
}



