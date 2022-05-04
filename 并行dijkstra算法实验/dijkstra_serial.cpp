 #include<iostream>
#include<iomanip>
#include<cmath>
 #include<ctime>
 #include<queue>
 #include<vector>
 #include<omp.h>
 using namespace std;

 #define MaxVertexMum 16000 /*最大顶点数*/
#define MAX 2147483647 /*当两点不连通时，则它们之间距离为最大值*/
 typedef int VertexType;/*顶点的类型为整型*/
 typedef int ArcType;/*边的权重为整形*/

 class Graph
 {
public :
    void randCreate();
    void show();
    int firstadj(int v);
    int nextadj(int v, int w);
    void shortestPath(VertexType start_poin);
private:
    VertexType vexs[MaxVertexMum];//顶点集
     VertexType arcs[MaxVertexMum][MaxVertexMum];//边集
     int vexnum,arcnum;//顶点数，边数
     vector<queue<VertexType>> path;
     ArcType dist[MaxVertexMum];
     bool solved[MaxVertexMum];
    
 };

 void Graph::randCreate()
 {
     int max_vexnum,max_arcnum,max_weight;
    max_vexnum = 16000;
    max_arcnum = 500000;
    max_weight = 5;
    this->vexnum = max_vexnum;//
    this->arcnum = max_arcnum;//
    for (int i = 0; i < this->vexnum; i++)
    {
        this->vexs[i] = i;
    }
    //初始化邻接矩阵
     for (int i = 0; i < this->vexnum; i++)
    {
         for (int j = 0; j < this->vexnum; j++)
        {
            this->arcs[i][j] = MAX;
        }
    }
    //随机生成任意边
    for (int k = 0; k < this->arcnum; k++)
    {
        //srand(time(NULL));//设置随机数种子，使每次运行获取的随机数不同
        int i = rand()%max_vexnum;
        int j = rand()%max_vexnum;
        int weight = rand()%max_weight+1;
        this->arcs[i][j] = weight;
    }
 }

 void Graph::show()
 {
      for (int i = 0; i < this->vexnum; i++)
      {
           for (int j = 0; j < this->vexnum; j++)
           {
               cout<<this->arcs[i][j]<<"  ";
           }
           cout<<endl;
      }
 }

 void Graph::shortestPath(VertexType start_point)
 {
     //初始化最短距离数组
     for (int i = 0; i < this->vexnum; i++)
     {
        this->dist[i] = MAX;
     }
     this->dist[start_point] = 0;
     //初始化保存路径的向量
     queue<VertexType>temp;
     temp.push(start_point);
    for (int i = 0; i < this->vexnum; i++)
     {
        path.push_back(temp);
     }
     //初始化solved数组
     for (int i = 0; i < this->vexnum; i++)
     {
        solved[i] = false;
     }

     for (int i = 0; i < this->vexnum; i++)
     {
        if (this->arcs[start_point][i] != MAX)
        {
            dist[i] = this->arcs[start_point][i];
            path[i].push(this->vexs[i]);
        }
     }

     solved[start_point] = true;
     for (int i = 0; i < this->vexnum-1; i++)
     {
         //v为最短路径顶点下标
         int v = start_point;////
         ArcType min = MAX;
        for (int j = 0; j < this->vexnum; j++)
        {
            if (!solved[j] && (dist[j] < min))
            {
                min = dist[j];
                v = j;
            }
        }
        solved[v] = true;////
        //更新所有点的权值
        # pragma omp parallel for
         for (int k = 0; k < this->vexnum; k++)
        {
            int w = this->arcs[v][k];
            if(!solved[k] && w<MAX && dist[v] + w < dist[k])
            {
                dist[k] = dist[v] + w;
                path[k] = path[v];
                path[k].push(this->vexs[k]);
            }
        }
     }

    //  cout<<"从节点"<<start_point<<"到各节点的最短路径长度如下："<<endl;
    //   for (int i = 0; i < this->vexnum; i++)
    //   {
    //       if(dist[i] == MAX)
    //       {
    //           cout<<"无法到达节点"<<this->vexs[i]<<endl;
    //       }
    //       else
    //       {
    //           cout<<"抵达节点"<<this->vexs[i]<<"的最短路径:";
    //           int path_length = path[i].size();
    //           for (int j = 0; j < path_length; j++)
    //           {
    //               cout<<path[i].front()<<" ";
    //               path[i].pop();
    //           }
    //           cout<<"长度为："<<dist[i]<<endl;
    //       }
    //   }
 }

 int main()
 {
     Graph graph;
     graph.randCreate();
    //graph.show();
     VertexType start_point;
     cout<<"请输入起点：";
     cin>>start_point;
     const clock_t begin_time = clock();
     graph.shortestPath(start_point);
     float seconds = float(clock() - begin_time)/ CLOCKS_PER_SEC;
     cout<<"串行执行用时："<<seconds<<"秒"<<endl;
    return 0;
 }