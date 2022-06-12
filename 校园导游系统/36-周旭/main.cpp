#include <stdio.h>
#include <string.h> //字符串操作
#include <stdlib.h> //system函数
#include <time.h>   //时间戳
#include <io.h>     //文件夹建立
#include <direct.h>   //mkdir()
//#include <stack>    //栈的适配容器
#define INF 999999
#define M 20

int dist[M][M]; //距离
int path[M][M]; //路径
int Stack[M];   //路径栈
int top;        //栈顶
int counts;     //记录路径数
int visited[M]; //标记数组

struct spotinfo //景点信息结构体
{
    char name[20];  //景点名称
    char info[500]; //景点介绍
};
struct maps
{
    int n; //顶点个数
    int m; //边数
    spotinfo v[M];
    int mapinfo[M][M]; //邻接矩阵
} g;                   //景点图的结构体

void login();
void map();
void mapmenu();
void search();
void Floyd();
void Floyd_print(int s, int e);
void allroad(int s, int e);

void Creat_spotinfo()
{
    strcpy(g.v[0].name, "大门");
    strcpy(g.v[0].info, "这是学校的入口");
    strcpy(g.v[1].name, "行政楼");
    strcpy(g.v[1].info, "学校的行政中心");
    strcpy(g.v[2].name, "艺术楼");
    strcpy(g.v[2].info, "这是音乐舞蹈与美术学院的学习之地");
    strcpy(g.v[3].name, "特教楼");
    strcpy(g.v[3].info, "特殊教育学院的学习之地");
    strcpy(g.v[4].name, "文科楼");
    strcpy(g.v[4].info, "思政、近现代史、毛概、马原等课就在这上");
    strcpy(g.v[5].name, "主教楼");
    strcpy(g.v[5].info, "学校的主教学楼");
    strcpy(g.v[6].name, "图书馆");
    strcpy(g.v[6].info, "这是自习的圣地，里面有无穷无尽的知识。");
    strcpy(g.v[7].name, "体育馆");
    strcpy(g.v[7].info, "这是学生进行室内比赛和体育活动的场所");
    strcpy(g.v[8].name, "食堂");
    strcpy(g.v[8].info, "学生们的用餐区域");
    strcpy(g.v[9].name, "田径场");
    strcpy(g.v[9].info, "这是学生进行室外比赛和体育活动的场所");
    strcpy(g.v[10].name, "宿舍区");
    strcpy(g.v[10].info, "这边有15栋宿舍楼");
    strcpy(g.v[11].name, "购物区");
    strcpy(g.v[11].info, "这里有两个超市和各种商店，物美价廉。");
}
void Creat_mapinfo()
{
    int i, j;
    g.n = 12;                 // 12个景点
    g.m = 18;                 // 18条双向路径
    for (i = 0; i < g.n; i++) //初始化邻接矩阵
    {
        for (j = 0; j < g.n; j++)
        {
            g.mapinfo[i][j] = INF;
        }
    }
    g.mapinfo[0][1] = g.mapinfo[1][0] = 120; //写入边的信息
    g.mapinfo[0][5] = g.mapinfo[5][0] = 260;
    g.mapinfo[0][6] = g.mapinfo[6][0] = 140;
    g.mapinfo[1][2] = g.mapinfo[2][1] = 140;
    g.mapinfo[1][4] = g.mapinfo[4][1] = 100;
    g.mapinfo[2][3] = g.mapinfo[3][2] = 120;
    g.mapinfo[2][4] = g.mapinfo[4][2] = 100;
    g.mapinfo[3][5] = g.mapinfo[5][3] = 200;
    g.mapinfo[4][5] = g.mapinfo[5][4] = 140;
    g.mapinfo[5][6] = g.mapinfo[6][5] = 200;
    g.mapinfo[5][7] = g.mapinfo[7][5] = 280;
    g.mapinfo[5][8] = g.mapinfo[8][5] = 360;
    g.mapinfo[6][7] = g.mapinfo[7][6] = 180;
    g.mapinfo[7][9] = g.mapinfo[9][7] = 120;
    g.mapinfo[8][9] = g.mapinfo[9][8] = 180;
    g.mapinfo[8][10] = g.mapinfo[10][8] = 140;
    g.mapinfo[9][11] = g.mapinfo[11][9] = 140;
    g.mapinfo[10][11] = g.mapinfo[11][10] = 160;
}

int main()
{
    char dirs[20] = {"MainFile"}; //检查同级目录是否存在文件，如没有则创建
    if (access(dirs, 0))
    {
        mkdir(dirs);
    }
    system("cls");    //清屏
    login();          //跳转登录
    Creat_spotinfo(); //创建景点信息
    mapmenu();        //菜单
}

void login()
{
    char admin1[30], password1[30], Radmin[20], Rpassword[20]; //定义账号密码
    char admin2[30], password2[30], ch;
    FILE *fp = fopen("MainFile/register.txt", "a+"); //打开存放密码的文件
    if ((ch = fgetc(fp)) == EOF)                     //如果密码文件为空 则跳转注册界面
    {
        rewind(fp);
        printf("\n============豫章师范学院校园导游助手============\n"); //注册界面
        printf("\n          ------请先创建一个账号------\n\n");
        printf("\t   账号:");
        scanf("%s", admin1);
        printf("\t   密码:");
        scanf("%s", password1);
        fprintf(fp, "%s %s", admin1, password1);
        system("cls");
        printf("            >>>新的账号创建成功<<<");
    }
    rewind(fp);
    printf("\n===========豫章师范学院校园导游助手=============\n"); //登陆界面
    printf("\n            ------账号登录------\n");
    printf("          账号:");
    scanf("%s", admin2);
    printf("          密码:");
    scanf("%s", password2);
    fscanf(fp, "%s", Radmin);    //从文件读取账号
    fscanf(fp, "%s", Rpassword); //从文件读取密码
    if (strcmp(admin2, Radmin) == 0 && strcmp(password2, Rpassword) == 0)
    { //判断账号密码是否正确
        fclose(fp);
        system("cls");
        printf("                                            >>>登录成功<<<");
    }
    else
    {
        system("cls"); //清屏
        printf("             >>>账号或密码错误<<<");
        fclose(fp);
        login();
    }
}

void map()
{
    printf("\n                       ================豫章师范学院校园导游助手==================\n");
    printf("                                     ----------校园地图----------\n");
    printf("========================================================================================================\n");
    printf("=                                                                                                      =\n");
    printf("=  (10)宿舍区                                                                                          =\n");
    printf("=      ⊙ ———————————|(8)食堂                               (5)主教楼                   (3)特教楼       =\n");
    printf("=      |            ⊙ —————————————————————————————————————————⊙ —————————————————————————⊙            =\n");
    printf("=      |            |                                        _—|—_        (4)文科楼       |            =\n");
    printf("=      |            |           |————————————————————————————  |  ———————————⊙————————————⊙ (2)艺术楼  =\n");
    printf("=      ⊙ ———————————|           |                              |              |           |            =\n");
    printf("=  (11)购物区       ⊙ ——————————|                              |              |      |—————            =\n");
    printf("=                (9)田径场      ⊙ ————————————⊙ ——————————|    |     |——————⊙ ———————|                 =\n");
    printf("=                             (7)体育馆    (6)图书馆      |————⊙ ————|    (1)行政楼                    =\n");
    printf("=                                                            (0)大门                                   =\n");
    printf("=                                                                                                      =\n");
    printf("========================================================================================================\n");
}

void mapmenu()
{
    int num, start, end;
    map();
    Creat_mapinfo();
    printf("\n\t\t\t\t\t豫章师范学院欢迎您!\n");
    printf("\n\t\t\t\t1)查询经点信息\n");
    printf("\t\t\t\t2)两景点间的最短路径\n");
    printf("\t\t\t\t3)两景点间的所有路径\n");
    printf("\t\t\t\t0)退出\n");
    printf("\n\t\t\t\t输入对应序号进行操作:");
    scanf("%d", &num);
    printf("\n                                        "); //根据输入进行不同的功能
    if (num == 1)
    {
        system("cls"); //清屏
        map();         //打印地图
        search();      //进入查找函数
    }
    else if (num == 2)
    {
        system("cls"); //清屏
        map();         //打开地图
        printf("\t\t\t\t请输入起点的景点:");
        scanf("%d", &start);
        printf("\t\t\t\t请输入终点的景点:");
        scanf("%d", &end);
        if (start < 0 || start > 11 || end < 0 || end > 11) //判断输入正确性
        {
            system("cls");
            printf("\t\t\t\t>>>您的输入有误<<<");
            mapmenu();
        }
        else if (start == end) //起点和终点相同
        {
            system("cls");
            map();
            printf("\n\t\t\t\t从%s到%s最短距离是:0m\n\n", g.v[start].name, g.v[end].name, dist[start][end]);
            printf("\t\t\t\t%s>>>%s\n\n\t\t\t\t", g.v[start].name, g.v[end].name);
            system("pause");
            system("cls");
            mapmenu();
        }
        else //求最短路径
        {
            Floyd(); //弗洛伊德
            system("cls");
            map();
            printf("\n\t\t\t\t从%s到%s最短距离是:%dm\n\n", g.v[start].name, g.v[end].name, dist[start][end]);
            printf("\t\t\t\t%s>>>", g.v[start].name);
            Floyd_print(start, end);
            printf("%s\n\n\t\t\t\t", g.v[end].name);
            system("pause");
            system("cls");
            mapmenu();
        }
    }
    else if (num == 3)
    {
        system("cls");
        map();
        Creat_mapinfo();
        counts = 1;
        printf("\t\t\t\t请输入起点的景点:");
        scanf("%d", &start);
        printf("\t\t\t\t请输入终点的景点:");
        scanf("%d", &end);
        if (start < 0 || start > 11 || end < 0 || end > 11)
        {
            system("cls");
            printf("\t\t\t\t>>>您的输入有误<<<");
            mapmenu();
        }
        else if (start == end)
        {
            system("cls");
            map();
            printf("\n\t\t\t\t从%s到%s最短距离是:0m\n\n", g.v[start].name, g.v[end].name, dist[start][end]);
            printf("\t\t\t\t%s>>>%s\n\n\t\t\t\t", g.v[start].name, g.v[end].name);
            system("pause");
            system("cls");
            mapmenu();
        }
        else
        {
            system("cls");
            map();
            allroad(start, end);
            printf("\n\t\t\t\t");
            system("pause");
            system("cls");
            mapmenu();
        }
    }
    else if (num == 0)
    {
        system("cls");
        printf(" ==========\n");
        printf("  感谢使用!\n");
        printf(" ==========\n");
        system("pause");
    }
    else
    {
        system("cls");
        printf("\t\t\t\t>>>您的输入有误<<<"); //提示输入错误
        mapmenu();
    }
}

void search()
{
    int n;
    printf("\t\t\t\t\t     输入99返回\n");
    printf("\t\t\t\t    请输入你想要查询的景点编号:");
    scanf("%d", &n);
    if (n == 99)
    {
        system("cls");
        mapmenu();
    }
    else if (n < 0 || n > 11) //判断输入正确性
    {
        system("cls");
        map();
        printf("\n>>>您的输入有误<<<\n");
        printf("\t\t\t\t\t输入99退出\n");
        printf("\t\t\t\t请输入你想要查询的景点编号:");
    }
    else
    {
        system("cls");
        map();
        printf("\n\t(%d)%s\n", n, g.v[n].name);
        printf("\t%s\n\n\t", g.v[n].info);
        system("pause");
        system("cls");
        map();
        search(); //递归返回查找
    }
}

void Floyd() //弗洛伊德*
{
    int i, j, k;
    for (i = 0; i < g.n; i++) //初始化距离与路径矩阵
    {
        for (j = 0; j < g.n; j++)
        {
            dist[i][j] = g.mapinfo[i][j];
            if (i != j && dist[i][j] < INF) //判断是否为同一地点且是否可以到达
            {
                path[i][j] = i;
            }
            else
            {
                path[i][j] = -1; //不可到达
            }
        }
    }
    for (k = 0; k < g.n; k++) //遍历求最短路径
    {
        for (i = 0; i < g.n; i++)
        {
            for (j = 0; j < g.n; j++)
            {
                if (dist[i][j] > (dist[i][k] + dist[k][j]))
                {
                    dist[i][j] = dist[i][k] + dist[k][j]; //更新
                    path[i][j] = k;                       // path用于记录最短路径上的结点
                }
            }
        }
    }
}

void Floyd_print(int s, int e)
{
    if (path[s][e] == -1 || path[s][e] == e || path[s][e] == s) //递归终止条件
    {
        return;
    }
    else
    {
        Floyd_print(s, path[s][e]);            //将中间点作为终点继续打印路径
        printf("%s>>>", g.v[path[s][e]].name); //打印中间景点名字
    }
}

void allroad(int s, int e)
{
    int dis = 0;
    int i, j;
    Stack[top] = s;
    top++;          //起点入栈
    visited[s] = 1; //标记入栈
    for (i = 0; i < g.n; i++)
    {
        if (g.mapinfo[s][i] > 0 && g.mapinfo[s][i] != INF && !visited[i])
        {
            //表明两点可达且未被访问
            if (i == e)
            {
                for (j = 0; j < top; j++)
                {
                    if (j < top - 1) //统计路径长度
                    {
                        dis = dis + g.mapinfo[Stack[j]][Stack[j + 1]];
                    }
                }
                dis = dis + g.mapinfo[Stack[top - 1]][e];
                printf("\t第%d条路(%dm):", counts++, dis);
                for (j = 0; j < top; j++)
                {
                    printf("%s>>>", g.v[Stack[j]].name);
                }
                printf("%s\n", g.v[e].name); //打印终点
            }
            else //不是终点接着打印
            {
                allroad(i, e);
                top--;          //支路全被访问一遍,顶点出栈
                visited[i] = 0; //出栈点标记为已出栈，允许下次访问
            }
        }
    }
}