#include <iostream>
#include <queue>
#include <stack>
#include<algorithm>
using namespace std;
#define  MAXVEX 100 /*最大顶点数*/

typedef  char  VertexType;
struct  ALGraphStruct;
typedef  struct  ALGraphStruct  *ALGraph;
const   int     INF = 65536;

/*边表*/
typedef struct EdgeNode{
    int adjVertex;  /*该边所指的顶点的位置*/
    int weight;     /*边的权*/
    struct EdgeNode *nextEdge;  /*指向下一条边的指针*/
}EdgeNode;
/* 顶点表*/
typedef struct VNode{
    VertexType data;   /*顶点信息*/
    EdgeNode *firstEdge; /*指向第一条依附该顶点的边的弧指针*/
}VNode;

struct ALGraphStruct{
    VNode vexs[MAXVEX];
    int vertexNum,edgeNum; /*图的当前顶点数和弧数*/
};
// 遍历辅助数组
bool visited[MAXVEX];
//  Prim辅助数组
struct close{
    VertexType  adjvex;
    int         lowcost;
}closedge[MAXVEX];
// Kruskal 算法辅助数组
struct helpK{
    VertexType  head;
    VertexType  Tail;
    int         lowcost;
}Edge[MAXVEX];
// Kruskal 算法辅助数组Vexset
int Vexset[MAXVEX];
/* 寻找点的位置*/
int locateVertex(ALGraph G,VertexType v);
/* 创建图*/
void CreateALGraph(ALGraphStruct &G);
// 创建无向图
void CreateUG(ALGraphStruct &G);
// 展示图
void Display(ALGraph G);
// 取得边表的第一个顶点
int First(ALGraphStruct G, int v);
// 取得除去v2顶点下，v1顶点下一个邻接点
int Next(ALGraphStruct G, int v,int v2);
// BFS
void BFS(ALGraphStruct G, int v);
void visit(ALGraphStruct G,int v);
// DFS 递归
void DFS(ALGraphStruct G,int v);
// DFS 非递归
void DFS2(ALGraphStruct G, int v);

// Prim 的Min 辅助函数
int Min(close a[],int n);
// Prim算法
int Prim(ALGraphStruct G,VertexType v);

// sort
bool cmp(helpK a, helpK b);
// Kruskal算法
void Kruskal(ALGraphStruct G);


int main() {
//    std::cout << "Hello, World!" << std::endl;
//    return 0;
    ALGraphStruct G{};
    CreateUG(G);
    Display(&G);
    Kruskal(G);
    return 0;
}

/* 寻找点的位置*/
int locateVertex(ALGraph G,VertexType v){
    int i;
    for(i = 0; i < G->vertexNum; i++){
        if(G->vexs[i].data == v)
            return i;
    }
    return -1;
}

/* 创建图  有向带权图*/
void CreateALGraph(ALGraphStruct &G){
    VertexType v1,v2;
    int weight;
    cout<<"请输入顶点和边数"<<endl;
    cin>>G.vertexNum>>G.edgeNum;
    cout<<"请输入顶点数据"<<endl;
    for(int i = 0; i < G.vertexNum; i++){
        cin>>G.vexs[i].data;
        G.vexs[i].firstEdge = nullptr;
    }
    cout<<"请依次输入n组边对应的两个顶点以及权值:"<<endl;
    for(int k = 0; k < G.edgeNum; k++){
        cin>>v1>>v2>>weight;
        int i = locateVertex(&G,v1);
        int j = locateVertex(&G,v2);
        EdgeNode *Edge;
        Edge = (EdgeNode*)malloc(sizeof(EdgeNode));
        Edge->adjVertex = j;
        Edge->weight = weight;
        /* 头插法*/
        Edge->nextEdge = G.vexs[i].firstEdge;
        G.vexs[i].firstEdge = Edge;
    }
}
/*创建无向图*/
void CreateUG(ALGraphStruct &G){
    VertexType v1, v2;
    int weight;
    cout<<"请输入顶点和边数\n";
    cin>>G.vertexNum>>G.edgeNum;
    // 构造顶点
    for(int i = 0; i < G.vertexNum; i++){
        cin>>G.vexs[i].data;
        G.vexs[i].firstEdge = nullptr;
    }
    // 构造边
    for(int k = 0; k < G.edgeNum ;k++){
        cin>>v1>>v2>>weight;
        // 方便在Kruskal算法中使用
        Edge[k]={v1,v2,weight};

        int i = locateVertex(&G,v1);
        int j = locateVertex(&G,v2);
        // 因为为无向图 所以插两遍 a->b b<-a;
        EdgeNode* Edge1;
        Edge1 = (EdgeNode*)malloc(sizeof(EdgeNode));
        Edge1->adjVertex = j;
        Edge1->weight = weight;
        // 头插法
        Edge1->nextEdge = G.vexs[i].firstEdge;
        G.vexs[i].firstEdge = Edge1;
        // 因为为无向图 所以插两遍 a->b b<-a;
        EdgeNode* Edge2;
        Edge2 = (EdgeNode*)malloc(sizeof(EdgeNode));
        Edge2->adjVertex = i;
        Edge2->weight = weight;
        // 头插法
        Edge2->nextEdge = G.vexs[j].firstEdge;
        G.vexs[j].firstEdge = Edge2;


    }
}

/* 展示图*/
void Display(ALGraph G){
    EdgeNode *p;
    cout<<"编号，  顶点，   相邻边的顶点\n";
    for(int i = 0; i < G->vertexNum; i++){
        printf("%4d  %4c",i, G->vexs[i].data);
        for(p = G->vexs[i].firstEdge; p != nullptr; p = p->nextEdge){
            char c = G->vexs[p->adjVertex].data;
            printf("%8d(%c)",p->adjVertex,c);
        }
        cout<<endl;
    }
}
int First(ALGraphStruct G, int v){
   // cout<<"v  "<<v<<endl;
    EdgeNode *p = G.vexs[v].firstEdge;
    if(p){
        return p->adjVertex;
    }else
        return -1;
}

int Next(ALGraphStruct G, int v,int v2){
    int index;
    // 先找到那个点;
    EdgeNode *p;
    p = G.vexs[v].firstEdge;
    // 先找到v的位置
    for(p;p != nullptr; p = p->nextEdge){
        if( p->adjVertex == v2)
            break;
    }

    if( p->nextEdge!= nullptr )
        return p->nextEdge->adjVertex;
    else
        return -1;
}
void visit(ALGraphStruct G,int v){
    printf("%c ",G.vexs[v].data);
}

void BFS(ALGraphStruct G, int v){
    visit(G,v);
    visited[v] = true;
    queue<int>que;
    que.push(v);
    while( !que.empty() ){
        // 每次都赋新值
        v = que.front();
        que.pop();
        int w = First(G,v);
       // cout<<w<<endl;
        while( w != -1){
            if( !visited[w] ){
                visit(G,w);
                visited[w] = true;
                que.push(w);
            }
            w = Next(G,v,w);
        }
    }
}

void DFS(ALGraphStruct G,int v){
    visit(G,v);
    visited[v] = true;
    EdgeNode *p = G.vexs[v].firstEdge;
    while (p != nullptr ){
        if( !visited[ p->adjVertex ] )
            DFS(G,p->adjVertex);
        p = p->nextEdge;
    }
}
void DFS2(ALGraphStruct G, int v){
    stack<int>stk;
    stk.push(v);
    while( !stk.empty() ){
        // 更新v的值
        v = stk.top();
        stk.pop();
        if( !visited[v] ){
            visited[v] = true;
            visit(G,v);
        }
        int w = First(G,v);
        while(w != -1){
            if( !visited[w] )
                stk.push(w);
            w = Next(G,v,w);
        }

    }
}

int Min(close a[],int n){
    int k = -1;
    int min = INF;
    for(int i = 0; i < n; i++){
        if(  a[i].lowcost !=0 && a[i].lowcost < min){
            min = a[i].lowcost;
            k = i;
        }
    }
    return k;
}

int Prim(ALGraphStruct G,VertexType v){
    int sum,i,k;
    sum = 0;
    k = locateVertex(&G, v);
    closedge[k].lowcost = 0;
    closedge[k].adjvex = v;
    // 初始化
    for(i = 0; i < G.vertexNum; i++){
        if( i != k)
            closedge[i]={v, INF};
    }
    // 第一遍初始化值
    EdgeNode* p = G.vexs[k].firstEdge;
    while(p){
        closedge[p->adjVertex]={v, p->weight};
        p = p->nextEdge;
    }
    for(i = 1; i < G.vertexNum; i++){
        k = Min(closedge,G.vertexNum);
        // 记录总路径
        sum += closedge[k].lowcost;
        // 输出该边的两个顶点
        cout<<closedge[k].adjvex<<"->"<<G.vexs[k].data<<endl;

        closedge[k].lowcost = 0;
        EdgeNode *p2 = G.vexs[k].firstEdge;
        while( p2 ){
            if(closedge[p2->adjVertex].lowcost > p2->weight){
                closedge[p2->adjVertex]= {G.vexs[k].data ,p2->weight};
            }
            p2 = p2->nextEdge;
        }
    }
    cout<<sum;
    return sum;
}

bool cmp(helpK a, helpK b){
    return a.lowcost<b.lowcost;
}
// Kruskal算法
void Kruskal(ALGraphStruct G){
    int i;
    // 初始化Edge
    sort(Edge,Edge+G.edgeNum, cmp);
    // 构造辅助函数 使其成为各自的连通分量
    for(i = 0; i<G.vertexNum;i++)
        Vexset[i] = i;
    for(i = 0; i<G.edgeNum;i++){
        int v1 = locateVertex(&G,Edge[i].head);
        int v2 = locateVertex(&G,Edge[i].Tail);
        int vs1 = Vexset[v1];
        int vs2 = Vexset[v2];
        if(vs1 != vs2){
            cout<<Edge[i].head<<" "<<Edge[i].Tail<<" "<<Edge[i].lowcost<<endl;
            // 使得两个分量联通在一起
            for(int k = 0;k < G.vertexNum;k++){
                if(Vexset[k] == vs2){
                    Vexset[k] = vs1;
                }
            }
        }
    }
}

