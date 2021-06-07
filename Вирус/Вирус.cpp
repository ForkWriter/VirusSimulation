// Вирус.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <vector>
#include <fstream>

using namespace std;

const int cluster_size = 1000;
const int cluster_count = 100;
const int l = 10;
const int cl = 20;
const long int population = 100000; // дописать
static long int man_id = 1;

struct point
{
	point* links[l];
	bool ing[l];
	bool plague = false;
	long int num;
};


struct cluster
{
	int num;
	bool check = false;
	cluster* links[cl];
	point *clust[cluster_size];
};

vector<cluster*> Graph()
{
	vector<cluster*> People;
	short int cluster_num, cluster_point, cluster_pointl;
	for (short int i = 0; i < cluster_count; i++)
	{
		People.push_back(new cluster);
		People[i]->num = i;
		for (short int j = 0; j < l; j++)
			People[i]->links[j] = nullptr;
		for (short int j = 0; j < cluster_size; j++)
		{
			People[i]->clust[j] = new point;
			People[i]->clust[j]->num = man_id;
			man_id++;
			for (short int g = 0; g < l; g++)
				People[i]->clust[j]->links[g] = nullptr;
		}
	}
	for (short int i = 0; i < cluster_count; i++)
	{
		if (i < cluster_count - 1)
			People[i]->links[0] = People[i + 1];
		else
			People[i]->links[0] = People[0];
		People[i]->clust[0]->links[0] = People[i]->links[0]->clust[0];
		People[i]->links[0]->clust[0]->links[0] = People[i]->clust[0];
		People[i]->clust[0]->ing[0] = true;
		People[i]->links[0]->clust[0]->ing[0] = true;
		for (short int j = 1; j < cl; j++)
		{
			bool ochka = false;
			cluster_num = rand() % (cluster_count - 2);
			while (cluster_num == i)
				cluster_num = rand() % (cluster_count - 2);
			People[i]->links[j] = People[cluster_num];

			for (short int g = 0; g < l; g++)
			{
				cluster_point = rand() % (cluster_size - 2);
				cluster_pointl = rand() % (cluster_size - 2);

				int sch = 0;

				while (People[i]->clust[cluster_pointl]->ing[g] == true || People[i]->links[j]->clust[cluster_point]->ing[g] == true)
				{
					cluster_point = rand() % (cluster_size - 2);
					cluster_pointl = rand() % (cluster_size - 2);
					sch++;
					if (sch == 100)
						break;
				}

				if (People[i]->clust[cluster_pointl]->ing[g] == true)
					continue;

				ochka = true;

				People[i]->clust[cluster_pointl]->links[g] = People[i]->links[j]->clust[cluster_point];
				People[i]->links[j]->clust[cluster_point]->links[g] = People[i]->clust[cluster_pointl];

				People[i]->clust[cluster_pointl]->ing[g] = true;
				People[i]->links[j]->clust[cluster_point]->ing[g] = true;
			}
			if (ochka == false)
				People[i]->links[j] = nullptr;


			for (short int j = 0; j < cluster_size - l; j++)
			{
				for (short int g = 0; g < l; g++)
				{
					if (People[i]->clust[j]->links[g] != nullptr || People[i]->clust[j + g + 1]->ing[g] == true)
						continue;

					People[i]->clust[j]->links[g] = People[i]->clust[j+g+1];
					People[i]->clust[j+g+1]->links[g] = People[i]->clust[j];
				}
			}
		}
	}
	return People;
}

void conn_chek(cluster* Man)
{
	Man->check = true;
	for (int i = 0; i < cl; i++)
	{
		if (Man->links[i] != nullptr && Man->links[i]->check == false)
			conn_chek(Man->links[i]);
	}
}


bool connectivity(vector<cluster*> People)
{
	conn_chek(People[0]);
	bool check = false;
	for (int i = 0; i < cluster_count; i++)
	{
		if (People[i]->check == false)
			return false;
	}
	return true;
}

int main()
{
	srand(time(0));
	long int plague_count = 0;
	bool check = false;
	vector<cluster*> People;
	while (check == false)
	{
		People = Graph();
		check = connectivity(People);
	}
	//People = Graph();
	//cout << man_id;
	ofstream out;
	out.open("out.json");
	out << "[" << endl;
	for (int i = 0; i < cluster_count; i++)
	{
		for (int j = 0; j < cluster_size; j++)
		{
			out << "{";
			out << "\"Id\": " << People[i]->clust[j]->num << ",\"Links\": [";
			for (int g = 0; g < l-1; g++)
			{
				if (People[i]->clust[j]->links[g] != nullptr)
					out << People[i]->clust[j]->links[g]->num << ",";
			}
			if ((i == (cluster_count - 1)) && (j == (cluster_size - 1)))
				if (People[i]->clust[j]->links[4] != nullptr)
					out << People[i]->clust[j]->links[4]->num << "]}" << endl;
				else
					out << "]}" << endl;
			else
				if (People[i]->clust[j]->links[4] != nullptr)
					out << People[i]->clust[j]->links[4]->num << "]}," << endl;
				else
					out << "]}," << endl;
				//out << People[i]->clust[j]->links[4]->num << "]}," << endl;
		}
	}
	out << "]";
	return 0;
}
