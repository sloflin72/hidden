#include <string>
#include ".\freqtable.h"
using namespace std;
double weights[39]={0.0124248,0.0124248,0.0124248,0.0124248,0.0124248,0.0124248,0.0124248,
0.0124248,0.0124248,0.0124248,
0.0651738,0.0124248,0.0217339,0.0349835,0.1041442,0.0197881,0.0158610,
0.0492888,0.0558094,0.0009033,0.0050529,0.0331490,0.0202124,0.0564513,
0.0596302,0.0137645,0.0008606,0.0497563,0.0515760,0.0729357,0.0225134,
0.0082903,0.0171272,0.0013692,0.0145984,0.0007836,0.1918182,
0.0124248,0.0124248};
CFreqTable::CFreqTable(void):m_nNumberOfNodes(39)
{
	NumberOfNodes=m_nNumberOfNodes;
	for(int i=0;i<NumberOfNodes;i++)
	{
		NodeArray[i]=new node;
		NodeArray[i]->left=NULL;
		NodeArray[i]->right=NULL;
		NodeArray[i]->num=i;
		NodeArray[i]->weight=weights[i];
	}
}

CFreqTable::~CFreqTable(void)
{
	DeleteTree(m_node);
	//for(int i=0;i<m_nNumberOfNodes;i++)
	//{
	//	delete NodeArray[i];
	//}

	//delete[] NodeArray;
}
node * CFreqTable::CreateTree()
{
	node * MainNode;
	while(NumberOfNodes>1)MainNode=MergeNodes();
	return MainNode;
}

node * CFreqTable::MergeNodes()
{
	node * newnode=new node;
	newnode->weight=NodeArray[NumberOfNodes-1]->weight+NodeArray[NumberOfNodes-2]->weight;
	newnode->left=NodeArray[NumberOfNodes-2];
	newnode->right=NodeArray[NumberOfNodes-1];
	newnode->num=-1;
	InsertNewNode(newnode);
	NumberOfNodes--;
	return newnode;
}
void CFreqTable::InsertNewNode(node * newnode)
{
	int i=NumberOfNodes-3;
	while((i>=0)&&(NodeArray[i]->weight<=newnode->weight))
	{
		NodeArray[i+1]=NodeArray[i];
		i--;
	}
	NodeArray[i+1]=newnode;
}
void CFreqTable::GenerateTable(string (&tbl)[39])
{
	SortNodes();
	node * MainNode=CreateTree();
	m_node=MainNode;
	RunTree(MainNode,tbl,-1,"");
}

void CFreqTable::RunTree(node * nextnode,string (&tbl)[39],int flag,string binary)
{
	if(0==flag)
		binary+="0";
	else if(1==flag)
		binary+="1";

	if(nextnode->num!=-1)
		tbl[nextnode->num]=binary;

	if(nextnode->left!=NULL)RunTree(nextnode->left,tbl,0,binary);
	if(nextnode->right!=NULL)RunTree(nextnode->right,tbl,1,binary);
}

void CFreqTable::SortNodes()
{
	node * temp;
	for(int i=0;i<m_nNumberOfNodes;i++)
		for(int j=i+1;j<m_nNumberOfNodes;j++)
		{
			if(NodeArray[i]->weight<NodeArray[j]->weight)
			{
				temp=NodeArray[i];
				NodeArray[i]=NodeArray[j];
				NodeArray[j]=temp;
			}
		}
}

void CFreqTable::DeleteTree(node * MainNode)
{
	if(MainNode->left!=NULL)DeleteTree(MainNode->left);
	if(MainNode->right!=NULL)DeleteTree(MainNode->right);
	delete MainNode;
}