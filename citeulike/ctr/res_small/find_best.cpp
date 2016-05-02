#include <iostream>
#include <string>
#include <fstream>
#include </usr/local/Cellar/eigen/3.2.8/include/eigen3/Eigen/Dense> 
#include <time.h>
#include <queue>
#include <stdio.h>
#include <stdlib.h>
using namespace Eigen;  
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
	time_t t_start =  time(NULL);
	const char * user_data_dir = "final-U.dat";
	const char * item_data_dir = "final-V.dat";
	const char * result_dir = "final_recom.dat";
	int feature_number = 200;
	int user_number = 2;
	int item_number = 16980;
	int best_number = 30;
	int iteration_user_size = 100;
	double **user = new double * [user_number];
	double **item = new double * [item_number];
	int **best = new int *[user_number];
	for(int i = 0;i<user_number;++i)
		best[i] = new int[ best_number];
	for(int i = 0;i< user_number;++i)
		user[i] = new double[feature_number ];
	for (int i = 0;i<item_number;++i)
		item[i] = new double [item_number];

	ifstream user_data (user_data_dir);
	ifstream item_data( item_data_dir);
	ofstream recommendation(result_dir);
	for(int i = 0;i<user_number;++i)
	{
		for(int j = 0;j<feature_number;++j)
			user_data>>user[i][j];
	}
	for(int i = 0;i<item_number;++i)
	{
		for(int j = 0;j<feature_number;++j)
			item_data>>item[i][j];
	}


	MatrixXd item_temp(feature_number,item_number );
	for(int i = 0;i<item_number;++i)
		for(int j = 0;j<feature_number;++j)
			item_temp(j,i) = item[i][j];
	int iter;
	for(iter = 0;iter< user_number/iteration_user_size;++iter )
	{
		cout<<iter<<endl;
		MatrixXd user_temp(iteration_user_size,feature_number);

		for(int i = 0;i<iteration_user_size;++i)
		{
			for(int j =  0;j<feature_number;++j )
				user_temp(i,j) = user[ iter*iteration_user_size+i][ j ];
		}
		MatrixXd result_matrix = user_temp*item_temp;
		double **  result = new double * [ iteration_user_size];
		for(int i = 0;i<iteration_user_size;++i)
		{
			result[i] = new double [item_number];
			for(int j = 0;j<item_number;++j)
				result[i][j] = result_matrix(i,j);
		}
		for(int i = 0;i<iteration_user_size;++i)
		{

		priority_queue<item_Node,vector<item_Node>,cmp> q;

			for(int j = 0;j<item_number;++j)
			{
				item_Node temp;
				temp.score = result[i][j];
				temp.item_id = j;
				if( q.size()<best_number )
				{
					q.push(temp);
					continue;
				}
				if(temp.score>q.top().score)
				{
					q.pop();
					q.push(temp);
				}

			}
			int index = best_number;
			while (!q.empty())
			{
				best[iter*iteration_user_size+i][--index] = q.top().item_id;
				q.pop();
			}
		}


	}
	cout<<iter<<endl;
	MatrixXd user_temp(user_number%iteration_user_size,feature_number);

	for(int i = 0;i<user_number%iteration_user_size;++i)
		{
			for(int j =  0;j<feature_number;++j )
				user_temp(i,j) = user[ iter*iteration_user_size+i][ j ];
		}
	MatrixXd result_matrix = user_temp*item_temp;
	double **  result = new double * [ user_number%iteration_user_size];
	for(int i = 0;i<user_number%iteration_user_size;++i)
	{
		result[i] = new double [item_number];
		for(int j = 0;j<item_number;++j)
			result[i][j] = result_matrix(i,j);
	}
	for(int i = 0;i<user_number%iteration_user_size;++i)
	{
		priority_queue<item_Node,vector<item_Node>,cmp> q;

		for(int j = 0;j<item_number;++j)
		{
			item_Node temp;
			temp.score = result[i][j];
			temp.item_id = j;
			if( q.size()<best_number )
			{
				q.push(temp);
				continue;
			}
			if(temp.score>q.top().score)
			{
				q.pop();
				q.push(temp);
			}
		}
	int index = best_number;
		while (!q.empty())
		{
			best[iter*iteration_user_size+i][--index] = q.top().item_id;
			q.pop();
		}
	}
user_data.close();
item_data.close();
for(int i = 0;i<user_number;++i)
{
	for(int j = 0;j<best_number;++j)
	{
		recommendation<<best[i][j]<<"\t";
	}
	recommendation<<"\n";
}
recommendation.close();

time_t t_end = time(NULL);
cout<<difftime(t_end,t_start)<<endl;
//m = (m + Matrix3d::Constant(1.2)) * 50;  





}