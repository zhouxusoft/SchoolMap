#include <stdio.h>
#include <string.h> //�ַ�������
#include <stdlib.h> //system����
#include <time.h>   //ʱ���
#include <io.h>     //�ļ��н���
#include <direct.h>   //mkdir()
//#include <stack>    //ջ����������
#define INF 999999
#define M 20

int dist[M][M]; //����
int path[M][M]; //·��
int Stack[M];   //·��ջ
int top;        //ջ��
int counts;     //��¼·����
int visited[M]; //�������

struct spotinfo //������Ϣ�ṹ��
{
    char name[20];  //��������
    char info[500]; //�������
};
struct maps
{
    int n; //�������
    int m; //����
    spotinfo v[M];
    int mapinfo[M][M]; //�ڽӾ���
} g;                   //����ͼ�Ľṹ��

void login();
void map();
void mapmenu();
void search();
void Floyd();
void Floyd_print(int s, int e);
void allroad(int s, int e);

void Creat_spotinfo()
{
    strcpy(g.v[0].name, "����");
    strcpy(g.v[0].info, "����ѧУ�����");
    strcpy(g.v[1].name, "����¥");
    strcpy(g.v[1].info, "ѧУ����������");
    strcpy(g.v[2].name, "����¥");
    strcpy(g.v[2].info, "���������赸������ѧԺ��ѧϰ֮��");
    strcpy(g.v[3].name, "�ؽ�¥");
    strcpy(g.v[3].info, "�������ѧԺ��ѧϰ֮��");
    strcpy(g.v[4].name, "�Ŀ�¥");
    strcpy(g.v[4].info, "˼�������ִ�ʷ��ë�š���ԭ�ȿξ�������");
    strcpy(g.v[5].name, "����¥");
    strcpy(g.v[5].info, "ѧУ������ѧ¥");
    strcpy(g.v[6].name, "ͼ���");
    strcpy(g.v[6].info, "������ϰ��ʥ�أ������������޾���֪ʶ��");
    strcpy(g.v[7].name, "������");
    strcpy(g.v[7].info, "����ѧ���������ڱ�����������ĳ���");
    strcpy(g.v[8].name, "ʳ��");
    strcpy(g.v[8].info, "ѧ���ǵ��ò�����");
    strcpy(g.v[9].name, "�ﾶ��");
    strcpy(g.v[9].info, "����ѧ���������������������ĳ���");
    strcpy(g.v[10].name, "������");
    strcpy(g.v[10].info, "�����15������¥");
    strcpy(g.v[11].name, "������");
    strcpy(g.v[11].info, "�������������к͸����̵꣬����������");
}
void Creat_mapinfo()
{
    int i, j;
    g.n = 12;                 // 12������
    g.m = 18;                 // 18��˫��·��
    for (i = 0; i < g.n; i++) //��ʼ���ڽӾ���
    {
        for (j = 0; j < g.n; j++)
        {
            g.mapinfo[i][j] = INF;
        }
    }
    g.mapinfo[0][1] = g.mapinfo[1][0] = 120; //д��ߵ���Ϣ
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
    char dirs[20] = {"MainFile"}; //���ͬ��Ŀ¼�Ƿ�����ļ�����û���򴴽�
    if (access(dirs, 0))
    {
        mkdir(dirs);
    }
    system("cls");    //����
    login();          //��ת��¼
    Creat_spotinfo(); //����������Ϣ
    mapmenu();        //�˵�
}

void login()
{
    char admin1[30], password1[30], Radmin[20], Rpassword[20]; //�����˺�����
    char admin2[30], password2[30], ch;
    FILE *fp = fopen("MainFile/register.txt", "a+"); //�򿪴��������ļ�
    if ((ch = fgetc(fp)) == EOF)                     //��������ļ�Ϊ�� ����תע�����
    {
        rewind(fp);
        printf("\n============ԥ��ʦ��ѧԺУ԰��������============\n"); //ע�����
        printf("\n          ------���ȴ���һ���˺�------\n\n");
        printf("\t   �˺�:");
        scanf("%s", admin1);
        printf("\t   ����:");
        scanf("%s", password1);
        fprintf(fp, "%s %s", admin1, password1);
        system("cls");
        printf("            >>>�µ��˺Ŵ����ɹ�<<<");
    }
    rewind(fp);
    printf("\n===========ԥ��ʦ��ѧԺУ԰��������=============\n"); //��½����
    printf("\n            ------�˺ŵ�¼------\n");
    printf("          �˺�:");
    scanf("%s", admin2);
    printf("          ����:");
    scanf("%s", password2);
    fscanf(fp, "%s", Radmin);    //���ļ���ȡ�˺�
    fscanf(fp, "%s", Rpassword); //���ļ���ȡ����
    if (strcmp(admin2, Radmin) == 0 && strcmp(password2, Rpassword) == 0)
    { //�ж��˺������Ƿ���ȷ
        fclose(fp);
        system("cls");
        printf("                                            >>>��¼�ɹ�<<<");
    }
    else
    {
        system("cls"); //����
        printf("             >>>�˺Ż��������<<<");
        fclose(fp);
        login();
    }
}

void map()
{
    printf("\n                       ================ԥ��ʦ��ѧԺУ԰��������==================\n");
    printf("                                     ----------У԰��ͼ----------\n");
    printf("========================================================================================================\n");
    printf("=                                                                                                      =\n");
    printf("=  (10)������                                                                                          =\n");
    printf("=      �� ����������������������|(8)ʳ��                               (5)����¥                   (3)�ؽ�¥       =\n");
    printf("=      |            �� ������������������������������������������������������������������������������������ ����������������������������������������������������            =\n");
    printf("=      |            |                                        _��|��_        (4)�Ŀ�¥       |            =\n");
    printf("=      |            |           |��������������������������������������������������������  |  �����������������������ѡ������������������������� (2)����¥  =\n");
    printf("=      �� ����������������������|           |                              |              |           |            =\n");
    printf("=  (11)������       �� ��������������������|                              |              |      |����������            =\n");
    printf("=                (9)�ﾶ��      �� �������������������������� ��������������������|    |     |�������������� ��������������|                 =\n");
    printf("=                             (7)������    (6)ͼ���      |���������� ��������|    (1)����¥                    =\n");
    printf("=                                                            (0)����                                   =\n");
    printf("=                                                                                                      =\n");
    printf("========================================================================================================\n");
}

void mapmenu()
{
    int num, start, end;
    map();
    Creat_mapinfo();
    printf("\n\t\t\t\t\tԥ��ʦ��ѧԺ��ӭ��!\n");
    printf("\n\t\t\t\t1)��ѯ������Ϣ\n");
    printf("\t\t\t\t2)�����������·��\n");
    printf("\t\t\t\t3)������������·��\n");
    printf("\t\t\t\t0)�˳�\n");
    printf("\n\t\t\t\t�����Ӧ��Ž��в���:");
    scanf("%d", &num);
    printf("\n                                        "); //����������в�ͬ�Ĺ���
    if (num == 1)
    {
        system("cls"); //����
        map();         //��ӡ��ͼ
        search();      //������Һ���
    }
    else if (num == 2)
    {
        system("cls"); //����
        map();         //�򿪵�ͼ
        printf("\t\t\t\t���������ľ���:");
        scanf("%d", &start);
        printf("\t\t\t\t�������յ�ľ���:");
        scanf("%d", &end);
        if (start < 0 || start > 11 || end < 0 || end > 11) //�ж�������ȷ��
        {
            system("cls");
            printf("\t\t\t\t>>>������������<<<");
            mapmenu();
        }
        else if (start == end) //�����յ���ͬ
        {
            system("cls");
            map();
            printf("\n\t\t\t\t��%s��%s��̾�����:0m\n\n", g.v[start].name, g.v[end].name, dist[start][end]);
            printf("\t\t\t\t%s>>>%s\n\n\t\t\t\t", g.v[start].name, g.v[end].name);
            system("pause");
            system("cls");
            mapmenu();
        }
        else //�����·��
        {
            Floyd(); //��������
            system("cls");
            map();
            printf("\n\t\t\t\t��%s��%s��̾�����:%dm\n\n", g.v[start].name, g.v[end].name, dist[start][end]);
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
        printf("\t\t\t\t���������ľ���:");
        scanf("%d", &start);
        printf("\t\t\t\t�������յ�ľ���:");
        scanf("%d", &end);
        if (start < 0 || start > 11 || end < 0 || end > 11)
        {
            system("cls");
            printf("\t\t\t\t>>>������������<<<");
            mapmenu();
        }
        else if (start == end)
        {
            system("cls");
            map();
            printf("\n\t\t\t\t��%s��%s��̾�����:0m\n\n", g.v[start].name, g.v[end].name, dist[start][end]);
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
        printf("  ��лʹ��!\n");
        printf(" ==========\n");
        system("pause");
    }
    else
    {
        system("cls");
        printf("\t\t\t\t>>>������������<<<"); //��ʾ�������
        mapmenu();
    }
}

void search()
{
    int n;
    printf("\t\t\t\t\t     ����99����\n");
    printf("\t\t\t\t    ����������Ҫ��ѯ�ľ�����:");
    scanf("%d", &n);
    if (n == 99)
    {
        system("cls");
        mapmenu();
    }
    else if (n < 0 || n > 11) //�ж�������ȷ��
    {
        system("cls");
        map();
        printf("\n>>>������������<<<\n");
        printf("\t\t\t\t\t����99�˳�\n");
        printf("\t\t\t\t����������Ҫ��ѯ�ľ�����:");
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
        search(); //�ݹ鷵�ز���
    }
}

void Floyd() //��������*
{
    int i, j, k;
    for (i = 0; i < g.n; i++) //��ʼ��������·������
    {
        for (j = 0; j < g.n; j++)
        {
            dist[i][j] = g.mapinfo[i][j];
            if (i != j && dist[i][j] < INF) //�ж��Ƿ�Ϊͬһ�ص����Ƿ���Ե���
            {
                path[i][j] = i;
            }
            else
            {
                path[i][j] = -1; //���ɵ���
            }
        }
    }
    for (k = 0; k < g.n; k++) //���������·��
    {
        for (i = 0; i < g.n; i++)
        {
            for (j = 0; j < g.n; j++)
            {
                if (dist[i][j] > (dist[i][k] + dist[k][j]))
                {
                    dist[i][j] = dist[i][k] + dist[k][j]; //����
                    path[i][j] = k;                       // path���ڼ�¼���·���ϵĽ��
                }
            }
        }
    }
}

void Floyd_print(int s, int e)
{
    if (path[s][e] == -1 || path[s][e] == e || path[s][e] == s) //�ݹ���ֹ����
    {
        return;
    }
    else
    {
        Floyd_print(s, path[s][e]);            //���м����Ϊ�յ������ӡ·��
        printf("%s>>>", g.v[path[s][e]].name); //��ӡ�м侰������
    }
}

void allroad(int s, int e)
{
    int dis = 0;
    int i, j;
    Stack[top] = s;
    top++;          //�����ջ
    visited[s] = 1; //�����ջ
    for (i = 0; i < g.n; i++)
    {
        if (g.mapinfo[s][i] > 0 && g.mapinfo[s][i] != INF && !visited[i])
        {
            //��������ɴ���δ������
            if (i == e)
            {
                for (j = 0; j < top; j++)
                {
                    if (j < top - 1) //ͳ��·������
                    {
                        dis = dis + g.mapinfo[Stack[j]][Stack[j + 1]];
                    }
                }
                dis = dis + g.mapinfo[Stack[top - 1]][e];
                printf("\t��%d��·(%dm):", counts++, dis);
                for (j = 0; j < top; j++)
                {
                    printf("%s>>>", g.v[Stack[j]].name);
                }
                printf("%s\n", g.v[e].name); //��ӡ�յ�
            }
            else //�����յ���Ŵ�ӡ
            {
                allroad(i, e);
                top--;          //֧·ȫ������һ��,�����ջ
                visited[i] = 0; //��ջ����Ϊ�ѳ�ջ�������´η���
            }
        }
    }
}