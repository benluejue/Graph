#include <iostream>
#include <queue>
#include <stack>
#include<algorithm>
using namespace std;
#define  MAXVEX 100 /*��󶥵���*/

typedef  char  VertexType;
struct  ALGraphStruct;
typedef  struct  ALGraphStruct  *ALGraph;
const   int     INF = 65536;
int a;
/*�߱�*/
typedef struct EdgeNode{
    int adjVertex;  /*�ñ���ָ�Ķ����λ��*/
    int weight;     /*�ߵ�Ȩ*/
    struct EdgeNode *nextEdge;  /*ָ����һ���ߵ�ָ��*/
}EdgeNode;
/* �����*/
typedef struct VNode{
    VertexType data;   /*������Ϣ*/
    EdgeNode *firstEdge; /*ָ���һ�������ö���ıߵĻ�ָ��*/
}VNode;

struct ALGraphStruct{
    VNode vexs[MAXVEX];
    int vertexNum,edgeNum; /*ͼ�ĵ�ǰ�������ͻ���*/
};
// ������������
bool visited[MAXVEX];
//  Prim��������
struct close{
    VertexType  adjvex;
    int         lowcost;
}closedge[MAXVEX];
// Kruskal �㷨��������
struct helpK{
    VertexType  head;
    VertexType  Tail;
    int         lowcost;
}Edge[MAXVEX];
// Kruskal �㷨��������Vexset
int Vexset[MAXVEX];
/* Ѱ�ҵ��λ��*/
int locateVertex(ALGraph G,VertexType v);
/* ����ͼ*/
void CreateALGraph(ALGraphStruct &G);
// ��������ͼ
void CreateUG(ALGraphStruct &G);
// չʾͼ
void Display(ALGraph G);
// ȡ�ñ߱��ĵ�һ������
int First(ALGraphStruct G, int v);
// ȡ�ó�ȥv2�����£�v1������һ���ڽӵ�
int Next(ALGraphStruct G, int v,int v2);
// BFS
void BFS(ALGraphStruct G, int v);
void visit(ALGraphStruct G,int v);
// DFS �ݹ�
void DFS(ALGraphStruct G,int v);
// DFS �ǵݹ�
void DFS2(ALGraphStruct G, int v);

// Prim ��Min ��������
int Min(close a[],int n);
// Prim�㷨
int Prim(ALGraphStruct G,VertexType v);

// sort
bool cmp(helpK a, helpK b);
// Kruskal�㷨
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

/* Ѱ�ҵ��λ��*/
int locateVertex(ALGraph G,VertexType v){
    int i;
    for(i = 0; i < G->vertexNum; i++){
        if(G->vexs[i].data == v)
            return i;
    }
    return -1;
}

/* ����ͼ  �����Ȩͼ*/
void CreateALGraph(ALGraphStruct &G){
    VertexType v1,v2;
    int weight;
    cout<<"�����붥��ͱ���"<<endl;
    cin>>G.vertexNum>>G.edgeNum;
    cout<<"�����붥������"<<endl;
    for(int i = 0; i < G.vertexNum; i++){
        cin>>G.vexs[i].data;
        G.vexs[i].firstEdge = nullptr;
    }
    cout<<"����������n��߶�Ӧ�����������Լ�Ȩֵ:"<<endl;
    for(int k = 0; k < G.edgeNum; k++){
        cin>>v1>>v2>>weight;
        int i = locateVertex(&G,v1);
        int j = locateVertex(&G,v2);
        EdgeNode *Edge;
        Edge = (EdgeNode*)malloc(sizeof(EdgeNode));
        Edge->adjVertex = j;
        Edge->weight = weight;
        /* ͷ�巨*/
        Edge->nextEdge = G.vexs[i].firstEdge;
        G.vexs[i].firstEdge = Edge;
    }
}
/*��������ͼ*/
void CreateUG(ALGraphStruct &G){
    VertexType v1, v2;
    int weight;
    cout<<"�����붥��ͱ���\n";
    cin>>G.vertexNum>>G.edgeNum;
    // ���춥��
    for(int i = 0; i < G.vertexNum; i++){
        cin>>G.vexs[i].data;
        G.vexs[i].firstEdge = nullptr;
    }
    // �����
    for(int k = 0; k < G.edgeNum ;k++){
        cin>>v1>>v2>>weight;
        // ������Kruskal�㷨��ʹ��
        Edge[k]={v1,v2,weight};

        int i = locateVertex(&G,v1);
        int j = locateVertex(&G,v2);
        // ��ΪΪ����ͼ ���Բ����� a->b b<-a;
        EdgeNode* Edge1;
        Edge1 = (EdgeNode*)malloc(sizeof(EdgeNode));
        Edge1->adjVertex = j;
        Edge1->weight = weight;
        // ͷ�巨
        Edge1->nextEdge = G.vexs[i].firstEdge;
        G.vexs[i].firstEdge = Edge1;
        // ��ΪΪ����ͼ ���Բ����� a->b b<-a;
        EdgeNode* Edge2;
        Edge2 = (EdgeNode*)malloc(sizeof(EdgeNode));
        Edge2->adjVertex = i;
        Edge2->weight = weight;
        // ͷ�巨
        Edge2->nextEdge = G.vexs[j].firstEdge;
        G.vexs[j].firstEdge = Edge2;


    }
}

/* չʾͼ*/
void Display(ALGraph G){
    EdgeNode *p;
    cout<<"��ţ�  ���㣬   ���ڱߵĶ���\n";
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
    // ���ҵ��Ǹ���;
    EdgeNode *p;
    p = G.vexs[v].firstEdge;
    // ���ҵ�v��λ��
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
        // ÿ�ζ�����ֵ
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
        // ����v��ֵ
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
    // ��ʼ��
    for(i = 0; i < G.vertexNum; i++){
        if( i != k)
            closedge[i]={v, INF};
    }
    // ��һ���ʼ��ֵ
    EdgeNode* p = G.vexs[k].firstEdge;
    while(p){
        closedge[p->adjVertex]={v, p->weight};
        p = p->nextEdge;
    }
    for(i = 1; i < G.vertexNum; i++){
        k = Min(closedge,G.vertexNum);
        // ��¼��·��
        sum += closedge[k].lowcost;
        // ����ñߵ���������
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
// Kruskal�㷨
void Kruskal(ALGraphStruct G){
    int i;
    // ��ʼ��Edge
    sort(Edge,Edge+G.edgeNum, cmp);
    // ���츨������ ʹ���Ϊ���Ե���ͨ����
    for(i = 0; i<G.vertexNum;i++)
        Vexset[i] = i;
    for(i = 0; i<G.edgeNum;i++){
        int v1 = locateVertex(&G,Edge[i].head);
        int v2 = locateVertex(&G,Edge[i].Tail);
        int vs1 = Vexset[v1];
        int vs2 = Vexset[v2];
        if(vs1 != vs2){
            cout<<Edge[i].head<<" "<<Edge[i].Tail<<" "<<Edge[i].lowcost<<endl;
            // ʹ������������ͨ��һ��
            for(int k = 0;k < G.vertexNum;k++){
                if(Vexset[k] == vs2){
                    Vexset[k] = vs1;
                }
            }
        }
    }
}

