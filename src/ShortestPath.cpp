#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <iterator>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

//Structure to represent a point
typedef struct
{
	double x;
	double y;
	double z;
}Point;

//Structure to represent a face input
typedef struct
{
	int a;
	int b;
	int c;
}Triangle;

//Structure to represent a face
typedef struct
{
	int id;
	int colour;
	int visited;
	int distance;
	int parent;
	int random;
}Face;

int main(int argc, char **argv)
{
	srand(time(NULL));
	int i,j;
	FILE *fp;
	fp = fopen(argv[1],"r");
	if(fp == NULL)
	{
		cout << "Input file not found.\n\n";
		return 0;
	}
	
	vector<Point> points;
	vector<Triangle> triangles;
	Point p;
	Triangle f;
	char c;
	int count=0;
	while(fscanf(fp,"%c",&c) != -1)
	{
		if(c == 'v')
		{
			fscanf(fp,"%lf %lf %lf\n",&p.x,&p.y,&p.z);
			points.push_back(p);
		}
		else if(c == 'f')
		{
			fscanf(fp,"%d %d %d\n",&f.a,&f.b,&f.c);
			triangles.push_back(f);
		}
		else
			continue;
	}
	fclose(fp);
	
	points.erase(points.begin()+points.size()-1);
	points.erase(points.begin()+points.size()-1);
	triangles.erase(triangles.begin()+triangles.size()-1);
	triangles.erase(triangles.begin()+triangles.size()-1);	
	if(strcmp(argv[1],"f16.obj")==0 || strcmp(argv[1],"f16")==0)
		triangles.erase(triangles.begin());
	
	map<pair<int,int>, int> edges;
	map<pair<int,int>, int>::iterator it;
	int N = triangles.size();
	Face faces[N];
	
	//Inserting the halfedges in the hashmap
	for(i=0;i<N;i++)
	{
		edges.insert(pair<pair<int,int>, int>(pair<int,int>(triangles[i].a,triangles[i].b),i));
		edges.insert(pair<pair<int,int>, int>(pair<int,int>(triangles[i].b,triangles[i].c),i));
		edges.insert(pair<pair<int,int>, int>(pair<int,int>(triangles[i].c,triangles[i].a),i));
		
		faces[i].id = i;
		faces[i].colour = -1;
		faces[i].visited = 0;
		faces[i].parent = -1;
		faces[i].distance = 0;
		faces[i].random = rand() % 2;
	}

	vector<int> subset;
	vector<int> ad[N];
	
	//Looping over all the faces
	for(i=0;i<N;i++)
	{
		it = edges.find(pair<int,int>(triangles[i].b,triangles[i].a));
		if(it != edges.end())
		{
			ad[i].push_back((*it).second);
			ad[(*it).second].push_back(i);
		}
		
		it = edges.find(pair<int,int>(triangles[i].c,triangles[i].b));
		if(it != edges.end())
		{
			ad[i].push_back((*it).second);
			ad[(*it).second].push_back(i);
		}
		
		it = edges.find(pair<int,int>(triangles[i].a,triangles[i].c));
		if(it != edges.end())
		{
			ad[i].push_back((*it).second);
			ad[(*it).second].push_back(i);
		}
		
		if(faces[i].random == 1)
			subset.push_back(i);
	}
	
	list<int> queue;

	int face,colour[subset.size()],col,num,u;

	cout << "\nPreparing object file...\n";
	for(j=0;j<subset.size();j++)
	{	
		face = subset[j];
		queue.push_back(face);
		faces[face].visited = 1;
		faces[face].distance = 0;
		num = face;

		while(!queue.empty())
		{
			for(i=0;i<ad[face].size();i++)
			{
				if(faces[ad[face][i]].visited == 0)
				{
					faces[ad[face][i]].visited = 1;
					faces[ad[face][i]].distance = faces[face].distance + 1;
					faces[ad[face][i]].parent = face;
					queue.push_back(ad[face][i]);
				}
			}

			queue.pop_front();
			face = queue.front();
		}
		
		for(i=0;i<N;i++)
		{
			u = i;
			col = rand() % 12;
			while(u != num)
			{
				if(u > triangles.size())
					break;
				faces[u].colour = col;
				u = faces[u].parent;
			}
		}
		
		queue.clear();
		for(i=0;i<N;i++)
		{
			faces[i].visited = 0;
			faces[i].parent = -1;
			faces[i].distance = 0;		
		}
	}
	
	//Preparing the colour.obj file
	fp = fopen("colour.obj","w");
	fprintf(fp,"mtllib ./colour.mtl\n\n");
	for(i=0;i<points.size();i++)
		fprintf(fp,"v %lf %lf %lf\n",points[i].x,points[i].y,points[i].z);
	for(i=0;i<triangles.size();i++)
	{
		if(faces[i].colour == 1)
			fprintf(fp,"usemtl RED\n");
		else if(faces[i].colour == 2)
			fprintf(fp,"usemtl GREEN\n");
		else if(faces[i].colour == 3)
			fprintf(fp,"usemtl BLUE\n");
		else if(faces[i].colour == 4)
			fprintf(fp,"usemtl YELLOW\n");		
		else if(faces[i].colour == 5)
			fprintf(fp,"usemtl ORANGE\n");
		else if(faces[i].colour == 6)
			fprintf(fp,"usemtl BROWN\n");
		else if(faces[i].colour == 7)
			fprintf(fp,"usemtl GRAY\n");
		else if(faces[i].colour == 8)
			fprintf(fp,"usemtl OLIVE\n");
		else if(faces[i].colour == 9)
			fprintf(fp,"usemtl BEET\n");		
		else if(faces[i].colour == 10)
			fprintf(fp,"usemtl WHEAT\n");
		else if(faces[i].colour == 11)
			fprintf(fp,"usemtl CYAN\n");
		else
			fprintf(fp,"usemtl MAROON\n");

		fprintf(fp,"f %d %d %d\n",triangles[i].a,triangles[i].b,triangles[i].c);
	}
	fclose(fp);
	
	//Preparing the colour.mtl file
	fp = fopen("colour.mtl","w");
	fprintf(fp,"newmtl RED\n");
	fprintf(fp,"Kd  1.0000  0.0000  0.0000\n\n");
	fprintf(fp,"newmtl GREEN\n");
	fprintf(fp,"Kd  0.0000  1.0000  0.0000\n\n");
	fprintf(fp,"newmtl BLUE\n");
	fprintf(fp,"Kd  0.0000  0.0000  1.0000\n\n");
	fprintf(fp,"newmtl YELLOW\n");
	fprintf(fp,"Kd  1.0000  1.0000  0.0000\n");
	fprintf(fp,"newmtl ORANGE\n");
	fprintf(fp,"Kd  1.0000  0.6470  0.0000\n");
	fprintf(fp,"newmtl BROWN\n");
	fprintf(fp,"Kd  0.6470  0.1647  0.1647\n\n");
	fprintf(fp,"newmtl GRAY\n");
	fprintf(fp,"Kd  0.5019  0.5019  0.5019\n\n");
	fprintf(fp,"newmtl OLIVE\n");
	fprintf(fp,"Kd  0.5019  0.5019  0.0000\n\n");
	fprintf(fp,"newmtl BEET\n");
	fprintf(fp,"Kd  0.5568  0.2196  0.5568\n");
	fprintf(fp,"newmtl WHEAT\n");
	fprintf(fp,"Kd  0.8039  0.7294  0.5882\n");
	fprintf(fp,"newmtl CYAN\n");
	fprintf(fp,"Kd  0.0000  0.9333  0.9333\n");
	fprintf(fp,"newmtl MAROON\n");
	fprintf(fp,"Kd  0.5019  0.0000  0.0000\n");

	cout << "Object file prepared.\n";
	fclose(fp);
		
	return 0;
}
