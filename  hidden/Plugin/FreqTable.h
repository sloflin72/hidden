#pragma once
using namespace std;
typedef struct node
{
	int num;
	double weight;
	string binary;
	node * left;
	node * right;
}node;
class CFreqTable
{
	int NumberOfNodes;
	const int m_nNumberOfNodes;
	node * MergeNodes();
	node * CreateTree();
	void SortNodes();
	void InsertNewNode(node * newnode);
	void RunTree(node * nextnode,string (&tbl)[39],int flag,string binary);
	void DeleteTree(node * MainNode);
	node * NodeArray[39];
	node * m_node;
public:
	void GenerateTable(string (&tbl)[39]);
	CFreqTable(void);
	~CFreqTable(void);
	
};
