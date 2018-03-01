/*

思路：
	支持旋转到任何角度（以前面、上面的颜色表示），并进行：
		序列操作
		模式匹配
	ABS 是指顶面为黄色，前面为红色的状态


	色盲模式：无视本来的颜色名称
	TODO: 尽量减少硬编码
*/
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>

#define IFOK if(cube.CheckPattern())

using namespace std;



#include <string>
#include <queue>
#include <vector>

queue<int> unsimp;
vector<int> simp;

int tmpc[54];
char OUT[6][12];
int cubic[6][9]={{1,2,3,4,5,6,7,8,9}/*upper*/,{10,11,12,22,23,24,34,35,36},/*left*/
{13,14,15,25,26,27,37,38,39},/*front*/{16,17,18,28,29,30,40,41,42},/*right*/
{19,20,21,31,32,33,43,44,45},/*back*/{46,47,48,49,50,51,52,53,54}};/*bottom*/
int change[12][20]=
{{1,4,7,8,9,6,3,2,10,11,12,13,14,15,16,17,18,19,20,21},//up1(shunshizheng)
{1,2,3,6,9,8,7,4,21,20,19,18,17,16,15,14,13,12,11,10},//up2(counterclock)
{46,47,48,51,54,53,52,49,45,44,43,42,41,40,39,38,37,36,35,34},//down1(special)
{46,49,52,53,54,51,48,47,34,35,36,37,38,39,40,41,42,43,44,45},//down2
{19,31,43,44,45,33,21,20,18,30,42,48,47,46,34,22,10,1,2,3},//b1
{19,20,21,33,45,44,43,31,3,2,1,10,22,34,46,47,48,42,30,18},//b2(zaisuochaomianweizhengdeqingkuangxiakannagemianshizheng)
{10,22,34,35,36,24,12,11,21,33,45,46,49,52,37,25,13,7,4,1},//l1
{10,11,12,24,36,35,34,22,1,4,7,13,25,37,52,49,46,45,33,21},//l2
{13,25,37,38,39,27,15,14,12,24,36,52,53,54,40,28,16,9,8,7},//f1
{13,14,15,27,39,38,37,25,7,8,9,16,28,40,54,53,52,36,24,12},
{16,28,40,41,42,30,18,17,15,27,39,54,51,48,43,31,19,3,6,9},//r1
{16,17,18,30,42,41,40,28,9,6,3,19,31,43,48,51,54,39,27,15}};//f2
int last[9]={52,53,54,49,50,51,46,47,48};
int a[55];
char name[12][4]={"U","Ui","D","Di","B","Bi","L","Li","F","Fi","R","Ri"};
int iptc[6]={3,5,2,4,1,6};
int antiiptc[6]={4,2,0,3,1,5};
int trans[26];
char antitrans[7];
/*char HASH[6];
char HASH_[6];*/
char IMPUT[6][10];
enum COLOR{R,O,B,G,Y,W};
enum OPERATION{R_,D_,L_,B_,F_,U_};
enum FACE{FRONT,BACK,LEFT,RIGHT,TOP,BOTTOM};
// 这是用来描述魔方的相对位置的数据
int operationmap[6][6][6]; // 对于每个方向，每个操作对应于绝对坐标下的哪个操作
int cubemap[6][6][54]; // 对于每个方向，每个色块对应于绝对坐标下的哪个色块
int operationcubemap[6][54]; // 对于每个操作，每个色块对应原来的哪个色块

const int out[6][6] = {
	{-1,-1,W,Y,B,G},
	{-1,-1,Y,W,G,B},
	{Y,W,-1,-1,O,R},
	{W,Y,-1,-1,R,O},
	{G,B,R,O,-1,-1},
	{B,G,O,R,-1,-1}
};
const int opp[6] = {O,R,G,B,W,Y};

const int opmap_up[6] = {R_,F_,L_,D_,U_,B_};
const int opmap_left[6] = {F_,D_,B_,R_,L_,U_};
const int cmap_up[54] = {36,37,38,39,40,41,42,43,44,53,52,51,50,49,48,47,46,45,24,21,18,25,22,19,26,23,20,29,32,35,28,31,34,27,30,33,17,16,15,14,13,12,11,10,9,0,1,2,3,4,5,6,7,8};
const int cmap_left[54] = {18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,9,10,11,12,13,14,15,16,17,0,1,2,3,4,5,6,7,8,38,41,44,37,40,43,36,39,42,51,48,45,52,49,46,53,50,47};
const int opcmap[6][20] = {
	{2,47,15,38, 5,50,12,41, 8,53,9,44, 27,33,35,29, 28,30,34,32},
	{6,24,15,33, 7,25,16,34, 8,26,17,35, 45,51,53,47, 46,48,52,50},
	{0,36,17,45, 3,39,14,48, 6,42,11,51, 26,20,18,24, 23,19,21,25},
	{29,53,24,36, 32,52,21,37, 35,51,18,38, 9,15,17,11, 10,12,16,14},
	{44,20,45,33, 43,23,46,30, 42,26,47,27, 0,6,8,2, 1,3,7,5},
	{0,27,9,18, 1,28,10,19, 2,29,11,20, 42,44,38,36, 43,41,37,39}
};

bool flag[6][6];


void print_side()
{
    for(int m=0;m<6;m++)
    {
    for(int i=0;i<=8;i++)
        OUT[antiiptc[m]][i]=antitrans[a[cubic[m][i]]];
    }
    return;
}
void print_num()
{
    for(int m=0;m<6;m++)
    {
        for(int i=0;i<9;i++)
            cout << a[cubic[m][i]];
        cout <<endl;
    }
}

void spin(int m)//the exact number of the mark of (+1)//for odd=clock;for =counver
{
		int s[2]={a[change[m-1][0]],a[change[m-1][1]]};int j[3]={a[change[m-1][8]],a[change[m-1][9]],a[change[m-1][10]]};
		for(int i=0;i<6;i++)
			a[change[m-1][i]]=a[change[m-1][i+2]];
		a[change[m-1][6]]=s[0];a[change[m-1][7]]=s[1];
		for(int i=8;i<17;i++)
        {
            a[change[m-1][i]]=a[change[m-1][i+3]];
     //       cout << change[m-1][i] << " "<< change[m-1][i+3]<<endl;
        }
		a[change[m-1][17]]=j[0];a[change[m-1][18]]=j[1];a[change[m-1][19]]=j[2];
		unsimp.push(m);
}

//白色棱块是2,4,6,8的标配！
void cross()
{
    int s[4][7]={{2,22,47,30/*主面*/,32,20,44/*后两个的值是上下两层*/},{4,25,49,33,/*主面*/23,11,35},{8,28,53,24,26,14,38},{6,31,51,27,29,17,41}};
    for(int i=0;i<4;i++)
    {

        if(a[s[i][0]]!=6)
        {   //cout <<"It needs to be changed";
            for(int k=0;k<4;k++)
            {
                for(int j=1;j<4;j++)
                {
                    if(a[s[(k+i)%4][j]]==6)
                    {
                        for(int l=0;l<k;l++)
                            spin(2);
                        for(int l=0;l<j;l++)
                            spin(2*((k+i)%4)+6);
                        for(int l=0;l<k;l++)
                            spin(1);
                        goto label;
                    }
                }
            }
        }
        if(a[s[i][0]]!=6)
        {
            for(int k=0;k<4;k++)
            {
                if(a[s[(k+i)%4][5]]==6)
                {
                    spin(2*((k+i)%4)+6);
                        spin(1);
                    for(int l=0;l<k;l++)
                        spin(2);
                    spin(2*(((k+3)+i)%4)+6);
                    spin(2);
                    for(int l=0;l<k;l++)
                        spin(1);
                        goto label;
                }
                if(a[s[(k+i)%4][6]]==6)
                {
                    for(int l=0;l<k;l++)
                        spin(2);
                    spin(2*((k+i)%4)+5);
                    spin(1);
                    spin(2*(((k+3)+i)%4)+6);
                    spin(2);
                    for(int l=0;l<k;l++)
                        spin(1);
                    goto label;
                }
            }
        }
        label:;
  //      print_side();
    }
     for(int i=0;i<4;i++)
        {
            for(int k=0;k<4;k++)
            {
                if(a[s[(k+i)%4][4]]==a[s[i][5]])
                {
                    for(int j=0;j<k;j++)
                        spin(2);
                    spin(2*((k+i)%4)+5);spin(2*((k+i)%4)+5);
                    for(int j=0;j<k;j++)
                        spin(1);
                    goto label2;
                }
            }
            label2:;
        }
}
void side()
{
    int concern[8][3]={{1,10,21},{7,13,12},{9,16,15},{3,19,18},{34,46,45},{37,52,36},{40,54,39},{43,48,42}};//顺时针转角
    int mid[4]={32,23,26,29};
    for(int i=0;i<4;i++)
    {
      /*  for(int fuck=0;fuck<4;fuck++)
        {
            cout <<a[concern[i+4][0]]<<a[concern[i+4][1]]<<a[concern[i+4][2]]<<a[mid[i]]<<a[mid[(i+1)%4]];
        }*/
        if(a[concern[i+4][1]]!=6||a[concern[i+4][0]]!=a[mid[(i+1)%4]]||a[concern[i+4][2]]!=a[mid[i]])
        {

            for(int k=0;k<4;k++)
            {

                if((a[concern[((i+k)%4)+4][0]]==6&&a[concern[((i+k)%4)+4][1]]==a[mid[i]]&&a[concern[((i+k)%4)+4][2]]==a[mid[(i+1)%4]])||
                   (a[concern[((i+k)%4)+4][1]]==6&&a[concern[((i+k)%4)+4][2]]==a[mid[i]]&&a[concern[((i+k)%4)+4][0]]==a[mid[(i+1)%4]])||
                   (a[concern[((i+k)%4)+4][2]]==6&&a[concern[((i+k)%4)+4][0]]==a[mid[i]]&&a[concern[((i+k)%4)+4][1]]==a[mid[(i+1)%4]]))
                   {
                       spin(2*((i+1+k)%4)+5);
                       spin(1);
                       spin(2*((i+1+k)%4)+6);
                       spin(2);
                   }

            }
            for(int k=0;k<4;k++)
            {
                if((a[concern[(i+k)%4][0]]==6&&a[concern[(i+k)%4][1]]==a[mid[i]]&&a[concern[(i+k)%4][2]]==a[mid[(i+1)%4]])||
                   (a[concern[(i+k)%4][1]]==6&&a[concern[(i+k)%4][2]]==a[mid[i]]&&a[concern[(i+k)%4][0]]==a[mid[(i+1)%4]])||
                   (a[concern[(i+k)%4][2]]==6&&a[concern[(i+k)%4][0]]==a[mid[i]]&&a[concern[(i+k)%4][1]]==a[mid[(i+1)%4]]))
                {
                    for(int j=0;j<k;j++)
                        spin(1);
                    while(a[concern[4+i][1]]!=6||a[concern[4+i][0]]!=a[mid[(i+1)%4]]||a[concern[4+i][2]]!=a[mid[i]])
                    {
                        spin(2*((i+1)%4)+5);spin(1);spin(2*((i+1)%4)+6);spin(2);
                    }
                }
            }
        }
 //   cout << "Fa Q";
   // print_side();
    }
}

/*
bool check_cross()
{
    return (a[47]==a[49]&&a[49]==a[50]&&a[50]==a[51]&&a[51]==a[53]&&a[23]==a[35]&&a[26]==a[38]&&a[29]==a[41]&&a[32]==a[44]);
}
void build()//jianzaomofangde
{
    int p[50];
    srand((unsigned)time(NULL));
    for(int i=0;i<50;i++)
        p[i]=(rand()%12)+1;//12种操作！
    for(int j=0;j<50;j++)
        spin(p[j]);
 //   cout << "NYANYANYAN!";
    return;
}
*/

void imput()
{
    for(int i=0;i<6;i++)
    {
        for(int j=0;j<9;j++)
            cin >> IMPUT[i][j];
    }
    for(int i=0;i<6;i++)
    {
        trans[(int)(IMPUT[i][4]-'A')]=iptc[i];//trans是1-6之间的数
        antitrans[iptc[i]]=IMPUT[i][4];
    }
    for(int i=0;i<5;i++)
    {
        for(int j=0;j<9;j++)
        {
            a[cubic[iptc[i]-1][j]]=trans[(int)(IMPUT[i][j]-'A')];
        }

    }
    for(int j=0;j<9;j++)
        a[last[j]]=trans[(int)(IMPUT[5][j]-'A')];

}

void simplify()
{
    while(!unsimp.empty())
    {
        int a=unsimp.front();
        if(!simp.empty()&&(a-1)^1==(simp.back()-1))
            {simp.pop_back();}
        else
            {simp.push_back(a);}
        unsimp.pop();
    }
}

void first_layer()
{
    imput();
    print_num();
 //   print_side();
//    build();
    //imput();
//    print_side();
 //   cout << "NYAN!";
    cross();
 //   print_side();
    side();
 //   simplify();
    print_num();
    print_side();
    return;
}



void Getopmap(int front,int top)
{
	int i;
	if (!flag[top][opp[front]])
	{
		flag[top][opp[front]] = true;
		for (i = 0;i < 6;i++)
			operationmap[top][opp[front]][i] =
				operationmap[front][top][opmap_up[i]];
		Getopmap(top,opp[front]);
	}
	if (!flag[out[front][top]][top])
	{
		flag[out[front][top]][top] = true;
		for (i = 0;i < 6;i++)
			operationmap[out[front][top]][top][i] =
				operationmap[front][top][opmap_left[i]];
		Getopmap(out[front][top],top);
	}
	return;
}
void Getcmap(int front,int top)
{
	int i;
	if (!flag[top][opp[front]])
	{
		flag[top][opp[front]] = true;
		for (i = 0;i < 54;i++)
			cubemap[top][opp[front]][i] =
				cubemap[front][top][cmap_up[i]];
		Getcmap(top,opp[front]);
	}
	if (!flag[out[front][top]][top])
	{
		flag[out[front][top]][top] = true;
		for (i = 0;i < 54;i++)
			cubemap[out[front][top]][top][i] =
				cubemap[front][top][cmap_left[i]];
		Getcmap(out[front][top],top);
	}
	return;
}

void Init()
{

	int i,j;

	// opmap
	for (i = 0;i < 6;i++)
		for (j = 0;j < 6;j++)
			flag[i][j] = false;
	for (i = 0;i < 6;i++) operationmap[R][Y][i] = i;
	Getopmap(R,Y);

	// cmap
	for (i = 0;i < 6;i++)
		for (j = 0;j < 6;j++)
			flag[i][j] = false;
	for (i = 0;i < 54;i++) cubemap[R][Y][i] = i;
	Getcmap(R,Y);

	// opcmap
	int tmp[54];
	for (i = 0;i < 6;i++)
	{
		for (j = 0;j < 54;j++) tmp[j] = j;
		for (j = 0;j < 19;j++) tmp[opcmap[i][j]] = opcmap[i][j+1];
		tmp[opcmap[i][3]] = opcmap[i][0];
		tmp[opcmap[i][7]] = opcmap[i][4];
		tmp[opcmap[i][11]] = opcmap[i][8];
		tmp[opcmap[i][15]] = opcmap[i][12];
		tmp[opcmap[i][19]] = opcmap[i][16];
		for (j = 0;j < 54;j++) operationcubemap[i][j] = tmp[j];
	}

	return;
}



struct CUBE
{
	// 基础
	int cube_color[54]; // 记录每个色块的颜色，按照输入顺序
	string solution; // 记录解法，其中只包含6种基本操作
	CUBE() {solution.clear();}



	// 方向
	int front,top;
	void SetPos(int fcolor,int tcolor) // 设置魔方的方向,fcolor朝前,tcolor朝上
	{
		front = fcolor;
		top = tcolor;
	}
	void Reset() {SetPos(R,Y);}
	int GetColor(int FACE,int num) {return cube_color[cubemap[front][top][9*FACE+num]];}



	// 操作
	void Op(int op) // 做一个特定的操作
	{
		int i,tmpcolor[54];
		string tmp;
		op = operationmap[front][top][op];
		for (i = 0;i < 54;i++) tmpcolor[i] = cube_color[operationcubemap[op][i]];
		for (i = 0;i < 54;i++) cube_color[i] = tmpcolor[i];
		switch (op)
		{
			case R_: tmp = "R";break;
			case D_: tmp = "D";break;
			case L_: tmp = "L";break;
			case B_: tmp = "B";break;
			case F_: tmp = "F";break;
			case U_: tmp = "U";break;
			default : break;
		}
		solution += tmp;
		return;
	}
	void Op(string str) // 做一个序列操作，暂时只支持6种基本操作构成的序列 TODO:支持小写操作、xyz操作
	{
		int i,l = str.size();
		for (i = 0;i < l;i++)
			switch (str[i])
			{
				case 'R': Op(R_);break;
				case 'D': Op(D_);break;
				case 'L': Op(L_);break;
				case 'B': Op(B_);break;
				case 'F': Op(F_);break;
				case 'U': Op(U_);break;

				case 'r': Op(L_);SetPos(top,opp[front]);break;
				//case 'd':
				//case 'l':
				//case 'b':
				//case 'f':
				//case 'u':

				default : break;
			}
		return;
	}



	// 模式
	int cube_pattern[54],colormap[6];
	void ClearPattern() {for (int i = 0;i < 54;i++) cube_pattern[i] = -1;}
	void ResetColorMap() {for (int i = 0;i < 6;i++) colormap[i] = i;}
	void SetColorMap(int color,int newcolor) {colormap[color] = newcolor;}
	void SetPattern(int face,int num,int value) {cube_pattern[face*9+num] = value;}
	void SetPattern(int face,string str)
	{
		int i,tmp;
		for (i = 0;i < 9;i++)
		{
			switch (str[i])
			{
				case 'R': tmp = R;break;
				case 'O': tmp = O;break;
				case 'B': tmp = B;break;
				case 'G': tmp = G;break;
				case 'Y': tmp = Y;break;
				case 'W': tmp = W;break;
				default : tmp = -1;break;
			}
			cube_pattern[face*9+i] = tmp;
		}
	}
	bool CheckPattern()
	{
		int i;
		for (i = 0;i < 54;i++)
			if (cube_pattern[i] >= 0 && cube_color[cubemap[front][top][i]] != colormap[cube_pattern[i]])
				return false;
		return true;
	}

};





CUBE cube;

void Read() // 色盲模式
{
	int i,j,ctoc[6];
	char *tmp;
	for (i = 0;i < 6;i++)
	{
	    tmp = OUT[i];

	    cout << tmp << endl;

		for (j = 0;j < 9;j++)
			switch (tmp[j])
			{
				case 'R': tmpc[9*i+j] = R;break;
				case 'O': tmpc[9*i+j] = O;break;
				case 'B': tmpc[9*i+j] = B;break;
				case 'G': tmpc[9*i+j] = G;break;
				case 'Y': tmpc[9*i+j] = Y;break;
				case 'W': tmpc[9*i+j] = W;break;
				default : break;
			}
	}
	for (i = 0;i < 6;i++) ctoc[tmpc[9*i+4]] = i;
	for (i = 0;i < 54;i++) tmpc[i] = ctoc[tmpc[i]];
	//for (i = 0;i < 54;i++) cout << tmpc[i];
	//cout << endl;
	for (i = 0;i < 54;i++) cube.cube_color[i] = tmpc[i];
	return;
}
void Cross();
void F2L();
void OLL();
void PLL();
void Solve()
{
	Cross();
	F2L();
	OLL();
	PLL();
	return;
}
void Print() // TODO 测试
{
	int i,l = cube.solution.size();
	bool flag = false;
    int s=unsimp.size();
    for(int z=0;z<s;z++)
    {
        int a=unsimp.front();
        cout << name[a-1]<<" ";
        unsimp.pop();
    }
	for (i = 0;i < l;i++)
	{
		if (i+3<l && cube.solution[i] == cube.solution[i+1] && cube.solution[i] == cube.solution[i+2]
			&& cube.solution[i] == cube.solution[i+3])
		{
			i += 3;
			continue;
		}

		if (flag) printf(" ");
		else flag = true;
		printf("%c",cube.solution[i]);

		if (i+2<l && cube.solution[i] == cube.solution[i+1] && cube.solution[i] == cube.solution[i+2])
		{
			printf("i");
			i += 2;
			continue;
		}
	}
	printf("\n");
	return;
}

int main()
{
	Init();
	first_layer();

	Print();

	Read();
	Solve();
	Print();
	return 0;
}





























const int SIDE[4] = {R,B,O,G};



void Cross()
{
	// 由SYB完成
	return;
}



void FL2(int x)
{
	cube.Reset();
	cube.ClearPattern();
	cube.ResetColorMap();

	cube.SetPos(x,TOP);
	int r = cube.GetColor(FRONT,4);
	int g = cube.GetColor(RIGHT,4);
	if (r == cube.GetColor(FRONT,5) && g == cube.GetColor(RIGHT,3)) return;

	int i;
	for (i = 0;i < 4;i++)
	{
		cube.SetPos(i,TOP);
		if ((r == cube.GetColor(FRONT,5) && g == cube.GetColor(RIGHT,3))
			|| (g == cube.GetColor(FRONT,5) && r == cube.GetColor(RIGHT,3)))
		{
			cube.Op("RUUURRRUUUFFFUF");
			break;
		}
	}

	cube.SetPos(x,TOP);
	for (i = 0;i < 4;i++)
	{
		if ((r == cube.GetColor(FRONT,1) && g == cube.GetColor(TOP,7))
			|| (g == cube.GetColor(RIGHT,1) && r == cube.GetColor(TOP,5)))
			break;
		cube.Op("U");
	}
	if (i == 4) printf("F2L_FL2_ERROR!\n");

	if (r == cube.GetColor(FRONT,1) && g == cube.GetColor(TOP,7)) cube.Op("URUUURRRUUUFFFUF");
	else cube.Op("UUUFFFUFURUUURRR");
	return;
}
void FL2()
{
	for (int i = 0;i < 4;i++) FL2(i);
	return;
}
void F2L()
{
	// 第一层角块由SYB完成
	FL2();
	return;
}



void UPCROSS()
{
	cube.Reset();
	cube.ClearPattern();
	cube.ResetColorMap();
	// 检验是否已经完成
	cube.SetPattern(TOP," Y YYY Y ");	IFOK return;
	// 一字的情况
	cube.SetPattern(TOP," Y  Y  Y ");	IFOK cube.Op("U");
	cube.SetPattern(TOP,"   YYY   ");	IFOK {cube.Op("FRURRRUUUFFF");return;}
	// 拐弯的情况
	cube.SetPattern(TOP," Y  YY   ");	IFOK cube.Op("U");
	cube.SetPattern(TOP,"    YY Y ");	IFOK cube.Op("U");
	cube.SetPattern(TOP,"   YY  Y ");	IFOK cube.Op("U");
	cube.SetPattern(TOP," Y YY    ");	IFOK {cube.Op("FURUUURRRFFF");return;}
	// 一点的情况
	cube.Op("FRURRRUUUFFFUUFURUUURRRFFF");

	return;
}
bool CheckTop(string s) // 为了顶层专门搞得。。。设置顶层（包括侧面）的模式
{
	cube.SetPattern(TOP,s.substr(0,9));
	cube.SetPattern(FRONT,s.substr(9,3)+"      ");
	cube.SetPattern(RIGHT,s.substr(12,3)+"      ");
	cube.SetPattern(BACK,s.substr(15,3)+"      ");
	cube.SetPattern(LEFT,s.substr(18,3)+"      ");
	return cube.CheckPattern();
}
void UPFACE()
{
	cube.Reset();
	cube.ClearPattern();
	cube.ResetColorMap();

	cube.SetPattern(TOP,"YYYYYYYYY");	IFOK return;
	for (int i = 1;i <= 4;i++)
	{
		if (CheckTop(" Y YYY YY     Y  Y  Y")) {cube.Op("RRRUURURRRUR");return;}
		if (CheckTop(" YYYYY Y Y  Y     Y  ")) {cube.Op("RUURRRUUURUUURRR");return;}
		if (CheckTop(" YYYYY YYY       Y   ")) {cube.Op("rURRRUUUrrrFRFFF");return;}
		if (CheckTop(" YYYYYYY   Y      Y  ")) {cube.Op("FFFrURRRUUUrrrFR");return;}
		if (CheckTop(" Y YYYYYY      Y Y   ")) {cube.Op("RRDDDRUURRRDRUUR");return;}
		if (CheckTop(" Y YYY Y Y Y   Y Y   ")) {cube.Op("RUURRRUUURURRRUUURUUURRR");return;}
		if (CheckTop(" Y YYY Y   Y   Y  Y Y")) {cube.Op("RUURRUUURRUUURRUUR");return;}
		cube.Op("U");
	}

	printf("OLL_UPFACE_ERRER!\n");
	return;
}
void OLL()
{
	UPCROSS(); // 完成
	UPFACE(); // TODO 测试
	return;
}



bool CORNER_COMPLETE()
{
	cube.Reset();
	cube.ClearPattern();
	cube.ResetColorMap();
	// 判断这一步是否已经完成了（即使顶层没有转到正确位置也算完成）
	// 同时如果顶层没有转到正确位置，将其转到正确位置
	if (cube.GetColor(FRONT,0) == cube.GetColor(FRONT,2)
		&& cube.GetColor(LEFT,0) == cube.GetColor(LEFT,2)
		&& cube.GetColor(BACK,0) == cube.GetColor(BACK,2)
		&& cube.GetColor(RIGHT,0) == cube.GetColor(RIGHT,2))
	{
		while (cube.GetColor(FRONT,0) != R) cube.Op("U");
		return true;
	}
	return false;
}
bool CORNER_CHECK()
{
	cube.Reset();
	cube.ClearPattern();
	cube.ResetColorMap();
	if (cube.GetColor(FRONT,0) == cube.GetColor(FRONT,2)
		|| cube.GetColor(LEFT,0) == cube.GetColor(LEFT,2)
		|| cube.GetColor(BACK,0) == cube.GetColor(BACK,2)
		|| cube.GetColor(RIGHT,0) == cube.GetColor(RIGHT,2))
	{
		while (cube.GetColor(RIGHT,0) != cube.GetColor(RIGHT,2)) cube.Op("U");
		return true;
	}
	return false;
}
void CORNER() // 这一步不是用的给的教程上的方法
{
	// 检验是否已经完成
	if (CORNER_COMPLETE()) return;
	CORNER_CHECK();
	cube.Op("RRFFRRRBBBRFFRRRBRRR");
	if (CORNER_COMPLETE()) return;
	CORNER_CHECK();
	cube.Op("RRFFRRRBBBRFFRRRBRRR");
	if (CORNER_COMPLETE()) return;
	printf("PLL_CORNER_ERROR!\n");
	return;
}
void EDGE()
{
	cube.Reset();
	cube.ClearPattern();
	cube.ResetColorMap();
	// 检验是否已经完成
	cube.SetPattern(FRONT,1,R);cube.SetPattern(LEFT,1,B);
	cube.SetPattern(BACK,1,O);cube.SetPattern(RIGHT,1,G);
	IFOK return;

	// 四个棱块都没对上的三种情况，随意执行一种操作
	cube.ClearPattern();cube.SetPattern(FRONT,1,O);cube.SetPattern(BACK,1,R);	IFOK cube.Op("FFULRRRFFLLLRUFF");
	cube.ClearPattern();cube.SetPattern(FRONT,1,B);cube.SetPattern(BACK,1,G);	IFOK cube.Op("FFULRRRFFLLLRUFF");
	cube.ClearPattern();cube.SetPattern(FRONT,1,G);cube.SetPattern(BACK,1,B);	IFOK cube.Op("FFULRRRFFLLLRUFF");
	// 反复转魔方 直到背面棱块对上了

	int i;
	for (i = 0;i < 4;i++)
	{
		cube.SetPos(SIDE[i],Y);
		cube.ClearPattern();
		cube.SetPattern(BACK,1,opp[SIDE[i]]);
		IFOK break;
	}

	// 判断顺时针还是逆时针，执行操作
	cube.ClearPattern();cube.SetPattern(FRONT,1,out[SIDE[i]][Y]);	IFOK cube.Op("FFULRRRFFLLLRUFF");
	cube.ClearPattern();cube.SetPattern(FRONT,1,out[Y][SIDE[i]]);	IFOK cube.Op("FFUUULRRRFFLLLRUUUFF");

	return;
}
void PLL()
{
	CORNER(); // 完成
	EDGE(); // 完成
	return;
}
