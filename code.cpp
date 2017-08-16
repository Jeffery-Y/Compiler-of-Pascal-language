/*#include "table.h"

void WriteAsm()   //Ŀ���������
{

	vector<string> flags(0);//��ת��־ջ
	int flagcounter = 0;//��־������
	string tflag1, tflag2, tflag3;
	char p[10];
	string dseg1; dseg1 = "";   //���ձ���
	string src1; src1 = "";     //���ղ�����1
	string src2; src2 = "";     //���ղ�����2
	string aim; aim = "";       //���ղ������

	cout << endl;
	system("pause");
	cout << "************************" << endl;
	cout << "*  ���� ���֣���������  *" << endl;
	cout << "************************" << endl;
	cout << "Ŀ��������£�" << endl;

	sentence_all = "DSEG\tSEGMENT\n";

	for (int i = 0; i<num_fuhaobiao; i++){
		if (!strcmp(fuhaobiao[i].type, "int")){  //���ͱ���
			dseg1 = fuhaobiao[i].name;
			sentence_all += dseg1 + '\t' + "DW " + '\n';
		}
		else if (!strcmp(fuhaobiao[i].type, "float")){   //�����ͱ���
			dseg1 = fuhaobiao[i].name;
			sentence_all += dseg1 + '\t' + "DD " + '\n';
		}
		else if (!strcmp(fuhaobiao[i].type, "char")){   //�ַ��ͱ���
			dseg1 = fuhaobiao[i].name;
			sentence_all += dseg1 + '\t' + "DB " + '\n';
		}
	}


	sentence_all += "DSEG\tENDS\nCSEG\tSEGMENT\nASSUME\tCS:CSEG,DS:DSEG\nSTART:\tMOV AX,DSEG\n\tMOV DS,AX\n";

	int temp;

	for (temp = 0; temp <= num_tempcode; temp++)    //������Ԫʽ������Ŀ�����
	{

		if ((tempcodechart[temp].symbol == "+") || (tempcodechart[temp].symbol == "-")
			|| (tempcodechart[temp].symbol == "*") || (tempcodechart[temp].symbol == "/"))
			//������Ϊ"+"��"-"��"*"��"/"
		{
			src1 = tempcodechart[temp].operand1;
			src2 = tempcodechart[temp].operand2;
			aim = tempcodechart[temp].result;

			sentence_all = sentence_all
				+ "\tMOV AX," + tempcodechart[temp].operand1 + "\n"
				+ "\tMOV BX," + tempcodechart[temp].operand2 + "\n";
			if (tempcodechart[temp].symbol == "+"){
				sentence_all += "\tADD AX,BX\n";
			}
			if (tempcodechart[temp].symbol == "-"){
				sentence_all += "\tSUB AX,BX\n";
			}
			if (tempcodechart[temp].symbol == "*"){
				sentence_all += "\tMUL AX,BX\n";
			}
			if (tempcodechart[temp].symbol == "/"){
				sentence_all += "\tDIV AX,BX\n";
			}

			sentence_all += "\tMOV " + tempcodechart[temp].result + ",AX \n";
		}

		else if (tempcodechart[temp].symbol == "=")
			//������Ϊ"="
		{
			sentence_all = sentence_all
				+ "\tMOV " + tempcodechart[temp].result + ","
				+ tempcodechart[temp].operand1 + "\n";
		}


		else if ((tempcodechart[temp].symbol == "==") || (tempcodechart[temp].symbol == "!=")
			|| (tempcodechart[temp].symbol == ">") || (tempcodechart[temp].symbol == "<")
			|| (tempcodechart[temp].symbol == ">=") || (tempcodechart[temp].symbol == "<=")){
			//������Ϊ"=="��"!="��">"��"<"��">="��"<="
			src1 = tempcodechart[temp].operand1;
			src2 = tempcodechart[temp].operand2;
			aim = tempcodechart[temp].result;

			sentence_all += "\tCMP " + src1 + "," + src2 + "\n";
			itoa(flagcounter, p, 10);
			flagcounter++;

			if (tempcodechart[temp].symbol == "=="){
				sentence_all += "\tJNE flag";
			}
			if (tempcodechart[temp].symbol == "!="){
				sentence_all += "\tJE flag";
			}
			if (tempcodechart[temp].symbol == ">"){
				sentence_all += "\tJLE flag";
			}
			if (tempcodechart[temp].symbol == "<"){
				sentence_all += "\tJNL flag";
			}
			if (tempcodechart[temp].symbol == ">="){
				sentence_all += "\tJL flag";
			}
			if (tempcodechart[temp].symbol == "<="){
				sentence_all += "\tJG flag";
			}

			sentence_all += p;
			sentence_all += "\n";
			tflag1 = "flag";
			tflag1 += p;
			sentence_all += "\tMOV " + aim + ",1\n";
			itoa(flagcounter, p, 10);
			flagcounter++;
			sentence_all = sentence_all + "\tJMP flag" + p + "\n";
			tflag2 = "flag";
			tflag2 += p;
			sentence_all = sentence_all + tflag1 + ":\tMOV " + aim + ",0\n" + tflag2 + ":";

		}


		else if ((tempcodechart[temp].symbol == "&&") || (tempcodechart[temp].symbol == "||")){
			//������Ϊ"&&"��"||"
			src1 = tempcodechart[temp].operand1;
			src2 = tempcodechart[temp].operand2;
			aim = tempcodechart[temp].result;

			sentence_all += "\tMOV AX," + src1 + "\n";
			sentence_all += "\tMOV BX," + src2 + "\t\n";

			if (tempcodechart[temp].symbol == "&&"){
				sentence_all += "\tAND AX,BX\n";
			}
			if (tempcodechart[temp].symbol == "||"){
				sentence_all += "\tOR AX,BX\n";
			}

			sentence_all += "\tMOV " + aim + ",AX\n";
		}


		else if ((tempcodechart[temp].symbol == "if") || (tempcodechart[temp].symbol == "nextif")){
			//������Ϊ"if"��"nextif"
			src1 = tempcodechart[temp].operand1;

			sentence_all += "\tCMP " + src1 + ",1\n\tJNE flag";
			itoa(flagcounter, p, 10);
			flagcounter++;
			sentence_all = sentence_all + p + "\n";
			tflag3 = "flag";
			tflag3 += p;
			flags.push_back(tflag3);
		}

		else if (tempcodechart[temp].symbol == "el"){
			//������Ϊ"el"
			tflag1 = flags[flags.size() - 1];
			flags.pop_back();
			sentence_all += "\tJMP flag";
			itoa(flagcounter, p, 10);
			flagcounter++;
			sentence_all += p;
			sentence_all += "\n" + tflag1 + ":";
			tflag3 = "flag";
			tflag3 += p;
			flags.push_back(tflag3);
		}

		else if (tempcodechart[temp].symbol == "ie"){
			//������Ϊ"ie"
			tflag1 = flags[flags.size() - 1];
			flags.pop_back();
			sentence_all += tflag1 + ":";
		}

		else if (tempcodechart[temp].symbol == "wh"){
			//������Ϊ"wh"
			tflag1 = "flag";
			itoa(flagcounter, p, 10);
			flagcounter++;
			tflag1 += p;
			sentence_all = sentence_all + tflag1 + ":";
			flags.push_back(tflag1);
		}

		else if (tempcodechart[temp].symbol == "do"){
			//������Ϊ"do"
			src1 = tempcodechart[temp].operand1;

			sentence_all += "\tCMP " + src1 + ",1\n\tJNE flag";
			itoa(flagcounter, p, 10);
			flagcounter++;
			sentence_all = sentence_all + p + "\n";
			tflag3 = "flag";
			tflag3 += p;
			flags.push_back(tflag3);
		}

		else if (tempcodechart[temp].symbol == "we"){
			//������Ϊ"we"
			tflag1 = flags[flags.size() - 1];
			flags.pop_back();
			tflag2 = flags[flags.size() - 1];
			flags.pop_back();
			sentence_all = sentence_all + "\tJMP " + tflag2 + "\n" + tflag1 + ":";
		}
	}

	sentence_all = sentence_all + "CSEG" + "\t" + "ENDS\n\tEND\tSTART";
	cout << sentence_all << endl;
}*/