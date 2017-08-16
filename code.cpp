/*#include "table.h"

void WriteAsm()   //目标代码生成
{

	vector<string> flags(0);//跳转标志栈
	int flagcounter = 0;//标志计数器
	string tflag1, tflag2, tflag3;
	char p[10];
	string dseg1; dseg1 = "";   //接收变量
	string src1; src1 = "";     //接收操作符1
	string src2; src2 = "";     //接收操作符2
	string aim; aim = "";       //接收操作结果

	cout << endl;
	system("pause");
	cout << "************************" << endl;
	cout << "*  第四 部分：代码生成  *" << endl;
	cout << "************************" << endl;
	cout << "目标代码如下：" << endl;

	sentence_all = "DSEG\tSEGMENT\n";

	for (int i = 0; i<num_fuhaobiao; i++){
		if (!strcmp(fuhaobiao[i].type, "int")){  //整型变量
			dseg1 = fuhaobiao[i].name;
			sentence_all += dseg1 + '\t' + "DW " + '\n';
		}
		else if (!strcmp(fuhaobiao[i].type, "float")){   //浮点型变量
			dseg1 = fuhaobiao[i].name;
			sentence_all += dseg1 + '\t' + "DD " + '\n';
		}
		else if (!strcmp(fuhaobiao[i].type, "char")){   //字符型变量
			dseg1 = fuhaobiao[i].name;
			sentence_all += dseg1 + '\t' + "DB " + '\n';
		}
	}


	sentence_all += "DSEG\tENDS\nCSEG\tSEGMENT\nASSUME\tCS:CSEG,DS:DSEG\nSTART:\tMOV AX,DSEG\n\tMOV DS,AX\n";

	int temp;

	for (temp = 0; temp <= num_tempcode; temp++)    //搜索四元式表，生成目标代码
	{

		if ((tempcodechart[temp].symbol == "+") || (tempcodechart[temp].symbol == "-")
			|| (tempcodechart[temp].symbol == "*") || (tempcodechart[temp].symbol == "/"))
			//操作符为"+"或"-"或"*"或"/"
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
			//操作符为"="
		{
			sentence_all = sentence_all
				+ "\tMOV " + tempcodechart[temp].result + ","
				+ tempcodechart[temp].operand1 + "\n";
		}


		else if ((tempcodechart[temp].symbol == "==") || (tempcodechart[temp].symbol == "!=")
			|| (tempcodechart[temp].symbol == ">") || (tempcodechart[temp].symbol == "<")
			|| (tempcodechart[temp].symbol == ">=") || (tempcodechart[temp].symbol == "<=")){
			//操作符为"=="或"!="或">"或"<"或">="或"<="
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
			//操作符为"&&"或"||"
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
			//操作符为"if"或"nextif"
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
			//操作符为"el"
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
			//操作符为"ie"
			tflag1 = flags[flags.size() - 1];
			flags.pop_back();
			sentence_all += tflag1 + ":";
		}

		else if (tempcodechart[temp].symbol == "wh"){
			//操作符为"wh"
			tflag1 = "flag";
			itoa(flagcounter, p, 10);
			flagcounter++;
			tflag1 += p;
			sentence_all = sentence_all + tflag1 + ":";
			flags.push_back(tflag1);
		}

		else if (tempcodechart[temp].symbol == "do"){
			//操作符为"do"
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
			//操作符为"we"
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