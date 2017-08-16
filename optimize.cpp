#include "Table.h"

extern vector<QT_item>QT;

vector<QT_item>QT_1;

DagNode *dag[100];           //100个指向DagNode结构 指针  指针数组
VarNode var[100];
int dagNum, varNum;          //节点数，变量数
//ifstream fin;
//ofstream fout;             //文件读入，文件输出
string str1, str2, str3;
string opt, opt2;                    //但符号操作符
bool BisNew, CisNew;

bool strEqual(string str1, string str2)         //判断字符串是否相等
{
	int i, l;
	l = str1.length();
	i = str2.length();
	if (l != i) return false;
	for (i = 0; i<l; i++)
	if (str1[i] != str2[i]) return false;
	return true;
}
bool isCOpt(string ch)            //判断是否是运算符函数
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

double Calc(double a, string op, double b)       //常数表达式运算
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
int findVar(string str)               //查找变量变量是否已经被定义过，若已经被定义过，则返回结点编号，否则则返回0
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
			sprintf_s(s, "%lf", k);              //将数据格式化输出到字符串 是sprintf 的安全版本 将k转化成一个字符串保存在s中
			ss = "";
			for (int i = 0; i < strlen(s); i++)
				ss += s[i];
		}
	}
	if (str.length()>0 && str[0] >= 48 && str[0] <= 57)                //why 48 &57 str[0]中保存的是什么呢？
	{
		for (i = 0; i<str.length(); i++)
			s[i] = str[i];
		s[i] = '\0';
		k = atof(s);
		sprintf_s(s, "%lf", k);              //将数据格式化输出到字符串 是sprintf 的安全版本 将k转化成一个字符串保存在s中
		ss = "";
		for (int i = 0; i<strlen(s); i++)
			ss += s[i];
	}
	for (i = 1; i <= varNum; i++)            //varNum是what  节点总数目?? 还是dag图中的变量的总和
	{
		if (strEqual(ss, var[i].str))
			return i;                       //找到则返回结点编号
	}
	return 0;
}

void work4(int n)           //查找是否定义过变量str1，若定义过则先将曾定义的删除，如果没有定义过则将str1变量依附于编号为n的dagnode   
{
	int a, k;
	int i;

	a = findVar(str1);                   //查找是否有相同变量 str1四元式计算中的结果
	if (a == 0)
	{
		varNum++;
		a = varNum;
		var[a].str = str1;
		var[a].flagNode = n;             //变量a依附于结点n
		dag[n]->flagNum++;              //n结点的依附节点数++        
		dag[n]->flags[dag[n]->flagNum] = str1;
	}
	else
	{
		k = var[a].flagNode;                //变量所依附的结点号
		if (!dag[k]->isLeaf)               //所依附的结点不是叶子结点
		{
			for (i = 1; i <= dag[k]->flagNum; i++)
			if (strEqual(str1, dag[k]->flags[i]))
			{
				for (int j = i + 1; j <= dag[k]->flagNum; j++)
					dag[k]->flags[j - 1] = dag[k]->flags[j];    //查找到变量原来所在的位置并删除
				break;
			}
			dag[k]->flagNum--;
		}
		var[a].flagNode = n;
		dag[n]->flagNum++;
		dag[n]->flags[dag[n]->flagNum] = str1;
	}
}

void work32(int b, int c)                  //双变量的运算                
{
	int i, k1, n, k2;
	k1 = var[b].flagNode;
	k2 = var[c].flagNode;
	n = 0;
	for (i = 1; i <= dagNum; i++)              //查找是否有相同的运算已经被定义过
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

void work25(int b, int c)           //两个变量的比较运算                               
{
	int i, k1, n, k2;
	k1 = var[b].flagNode;
	k2 = var[c].flagNode;
	n = 0;
	for (i = 1; i <= dagNum; i++)              //查找是否有相同的运算已经被定义过
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


void work31(int b)                           //只有一个操作数，单操作数时候，把孩子放在右侧
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

void work26(int b, int c)                  //两个常数的比较运算
{
	bool p;
	int k;
	int i;
	char s[100];
	string str;
	p = Bcalc(dag[var[b].flagNode]->value, opt2, dag[var[c].flagNode]->value);
	if (CisNew)                          //第二个操作数之前没有被定义过，则将结点删除，只保留运算结果的结点
	{
		varNum--;
		delete(dag[dagNum]);
		dagNum--;
	}
	if (BisNew)                         //第一个操作数之前没有被定义过，则将结点删除
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





void work24(int b, int c)                  //两个常数的计算
{
	double p;
	int k;
	int i;
	char s[100];
	string str;
	p = Calc(dag[var[b].flagNode]->value, opt, dag[var[c].flagNode]->value);
	if (CisNew)                          //第二个操作数之前没有被定义过，则将结点删除，只保留运算结果的结点
	{
		varNum--;
		delete(dag[dagNum]);
		dagNum--;
	}
	if (BisNew)                  //第一个操作数之前没有被定义过，则将结点删除
	{
		varNum--;
		delete(dag[dagNum]);
		dagNum--;
	}
	sprintf_s(s, "%lf", p);      //将数据格式化输出到字符串 是sprintf 的安全版本  将k转化成一个字符串保存在s中
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
		dag[dagNum]->flags[1] = str;      //常数时flag[0]=0
		dag[dagNum]->flagNum = 1;
		dag[dagNum]->isLeaf = true;
	}
	work4(var[k].flagNode);
}

void work23(int b)                    //单个常数赋值
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
		dag[dagNum]->flags[1] = str;     //flag[0]无定义
		dag[dagNum]->flagNum = 1;
		dag[dagNum]->isLeaf = true;
	}
	work4(var[k].flagNode);
}


void work22(int b, int c)              //区分两个操作数时候是常数运算还是含有变量的运算
{
	if (dag[var[c].flagNode]->isVal && dag[var[b].flagNode]->isVal)
		work24(b, c);
	else
		work32(b, c);
}
void work33(int b, int c)              //区分两个操作数时候是常数运算还是含有变量的运算
{
	if (dag[var[c].flagNode]->isVal && dag[var[b].flagNode]->isVal)
		work26(b, c);
	else
		work25(b, c);
}


void work21(int b)                       //区分单操作数时候是变量运算还是常数运算
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
		str2 = QT[nn].o1;             //读第一个操作数，保存在str2中
		str3 = QT[nn].o2;
		str1 = QT[nn].t;              //运算结果保存在str1中

		if (strEqual(QT[nn].w, ":="))
		{
			ListType = 0;            //形如（：=，3.14，_，T0）即T0：=3.14
		}
		if (isCOpt(QT[nn].w))     //操作数的运算

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

		b = findVar(str2);          //查找第二个操作数是否已经被定义过
		BisNew = false;
		if (b == 0)
		{
			BisNew = true;               //若第二个操作数没有被定义过BisNew为true
			dagNum++;
			varNum++;
			b = varNum;
			var[b].str = str2;
			var[b].flagNode = dagNum;
			dag[dagNum] = new DagNode();
			if (str2[0] == 45 || str2[0] == 43) {
				s[0] = str2[0];
				if (str2[1] >= 48 && str2[1] <= 57)          //ASCII表的中对应0-9
				{
					dag[dagNum]->isVal = true;               //isVal 表示是常数
					for (i = 1; i < str2.length(); i++)
						s[i] = str2[i];
					s[i] = '\0';
					dag[dagNum]->value = atof(s);
				}
			}
			if (str2[0] >= 48 && str2[0] <= 57)          //ASCII表的中对应0-9
			{
				dag[dagNum]->isVal = true;               //isVal 表示是常数
				for (i = 0; i<str2.length(); i++)
					s[i] = str2[i];
				s[i] = '\0';
				dag[dagNum]->value = atof(s);
			}
			dag[dagNum]->flags[1] = str2;
			dag[dagNum]->flagNum = 1;
			dag[dagNum]->isLeaf = true;      //常数为叶子结点
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
					  CisNew = true;               //没有找到和第三个操作数相同的数值时，CisNew=true
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
					  CisNew = true;               //没有找到和第三个操作数相同的数值时，CisNew=true
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
				  work33(b, c);       //调用两个操作数的计算
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
			for (int j = 2; j <= dag[i]->flagNum; j++)    //如果第一个是临时变量，如果后面附加结点有全局，则互换
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

	int count, x;            //count控制while外层循环，x控制内层for循环
	int ss = 0;
	int ee = 0;              //ss是模块的开始，ee是模块的结束的下一个四元组

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
			ee = x;             //lb是一个模块的开始，上一个模块的结束
			funcn += QT[x].o1;     //存放所定义的function的名字
			work1(ss, ee);      //ee不会被执行
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
		//	ee = x;               //if在本模块中，作为本模块的结束
		//	work1(ss, ee);
		//	QTpush();
		//	QT_1.push_back({ QT[x].w,QT[x].o1,QT[x].o2,QT[x].t });
		//	//	QT_1.push_back({ " ", " ", " ", " " });
		//	count = x + 1;
		//	ee = x + 1;
		//	continue;
		//}

		if (find1 != string::npos) {
			ee = x;            //if在本模块中，作为本模块的结束
			work1(ss, ee);
			QTpush();
			QT_1.push_back({ QT[x].w, QT[x].o1, QT[x].o2, QT[x].t });
			//	QT_1.push_back({ " ", " ", " ", " " });
			count = x + 1;
			ee = x + 1;
			continue;
		}
		if (strEqual(QT[x].w, "ifend")) {
			ee = x;            //if在本模块中，作为本模块的结束
			work1(ss, ee);
			QTpush();
			QT_1.push_back({ QT[x].w, QT[x].o1, QT[x].o2, QT[x].t });
			//	QT_1.push_back({ " ", " ", " ", " " });
			count = x + 1;
			ee = x + 1;
			continue;
		}
		if (strEqual(QT[x].w, "whileend")) {
			ee = x;            //if在本模块中，作为本模块的结束
			work1(ss, ee);
			QTpush();
			QT_1.push_back({ QT[x].w, QT[x].o1, QT[x].o2, QT[x].t });
			//	QT_1.push_back({ " ", " ", " ", " " });
			count = x + 1;
			ee = x + 1;
			continue;
		}
		if (find4 != string::npos) {
			ee = x;            //if在本模块中，作为本模块的结束
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
			ee = x;             //lb是一个模块的开始，上一个模块的结束
			work1(ss, ee);      //ee不会被执行
			QTpush();
			//	QT_1.push_back({ " ", " ", " ", " " });
			QT_1.push_back({ QT[x].w, QT[x].o1, QT[x].o2, QT[x].t });
			count = x + 1;
			continue;

		}
	}



	/*QT.clear();
	//查找删除，把原来的代码

	QT.assign(QT_1.begin(), QT_1.end());

	for (int i = 0; i < QT.size(); i++)
	{
		cout << '(' << QT[i].w.c_str() << ',' << QT[i].o1.c_str() << ',' << QT[i].o2.c_str() << ',' << QT[i].t.c_str() << ')' << endl;
	}                            //测试输出用

	//cout << funcn.c_str();*/

	return 0;
}



