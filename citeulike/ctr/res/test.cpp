#include <iostream>
#include <queue>
using namespace std;
struct item_Node
{
	double score;
	int item_id;
};
struct cmp{
	bool operator()(item_Node a, item_Node b)
	{
		return a.score>b.score;
	}
};
int main()
{
	priority_queue<item_Node,vector<item_Node>,cmp> q;
	for(int i = 0;i<10;i++)
	{
		item_Node node;
		node.score = i;
		node.item_id = i;
		q.push(node);
	}
	while(!q.empty())
	{
		cout<<q.top().score<<"\t"<<q.top().item_id<<"\n";
		q.pop();
	}
}