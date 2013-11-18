#include <iostream>
#include <cmath>
#include <GL/glut.h>
#include<stdio.h>
using namespace std;

#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)
 

typedef struct node{
	float mat[4][4];
	struct node *next;
}node;
typedef struct vertex{
	float mat[4][1];
	float r,g,b;
	struct vertex *next;
}vertex;
node *model;
node *view;
node *head;
float projection[4][4];
vertex *vertex_h;
vertex *vertex_h_copy;
float x,y,wid,ht,w1,h1;
float red=0,blue=0,green=0;
float x_arr[100];
float y_arr[100];
float z_arr[100];
int flag_show=0;
int counter=0;
void mult_matrix(float b[4][4])
{
	float c[4][4];
	int i,j,k;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			c[i][j] = 0;
			for(k=0;k<4;k++)
				c[i][j] = c[i][j] + model->mat[i][k] * b[k][j];
		}
	}
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			model->mat[i][j]=c[i][j];
}
void mult_view_matrix(float b[4][4])
{
	float c[4][4];
	int i,j,k;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			c[i][j] = 0;
			for(k=0;k<4;k++)
				c[i][j] = c[i][j] + view->mat[i][k] * b[k][j];
		}
	}
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			view->mat[i][j]=c[i][j];
}
void calculations()
{
	vertex *t = vertex_h_copy;
	float c[4][4];
	int i,j,k;
	float d[4][4];
	float final[4][1];
	while(t->next!=NULL)
	{
		t=t->next;
		for(i=0;i<4;i++)
		{
			for(j=0;j<1;j++)
			{
				c[i][j] = 0;
				for(k=0;k<4;k++)
					c[i][j] = c[i][j] + model->mat[i][k] * t->mat[k][j];
			}
		}
		for(i=0;i<4;i++)
		{
			for(j=0;j<1;j++)
			{
				d[i][j] = 0;
				for(k=0;k<4;k++)
					d[i][j] = d[i][j] + view->mat[i][k] * c[k][j];
			}
		}
		for(i=0;i<4;i++)
		{
			for(j=0;j<1;j++)
			{
				final[i][j] = 0;
				for(k=0;k<4;k++)
					final[i][j] = final[i][j] +projection[i][k] * d[k][j];
			}
		}
		t->mat[0][0]=final[0][0]/final[3][0];
		t->mat[1][0]=final[1][0]/final[3][0];
		t->mat[2][0]=final[2][0];
		t->mat[3][0]=final[3][0];
		
		t->mat[0][0]=(t->mat[0][0] + 1)*(wid/2);
		t->mat[1][0]=(t->mat[1][0] + 1)*(ht/2);

		t->mat[0][0]+=x;
		t->mat[1][0]+=y;

		t->mat[0][0]=(t->mat[0][0]*2)/w1-1;
		t->mat[1][0]=(t->mat[1][0]*2)/h1-1;
	}
}
void akhil()
{
	counter=0;
	flag_show=0;
}
void LoadIdentity(node *a)
{
	int i,j;	
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			if(i==j)
				a->mat[i][j]=1;
			else
				a->mat[i][j]=0;
		}
	}
	a->next=NULL;
}
void sglModLoadIdentity()
{
	head = (node *)malloc(sizeof(node));
	head->next=NULL;
	model = (node *)malloc(sizeof(node));
	LoadIdentity(model);
}
void sglModPushMatrix()
{
	node *t_head=head;
	while(t_head->next!=NULL)
		t_head=t_head->next;
	node *temp = (node *)malloc(sizeof(node));
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			temp->mat[i][j]=model->mat[i][j];
	t_head->next=temp;
	temp->next=NULL;
}
void sglModPopMatrix()
{
	node *curr=head;
	node *prev=head;
	while(curr->next!=NULL)
	{
		prev=curr;
		curr=curr->next;
	}
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			model->mat[i][j]=curr->mat[i][j];
	prev->next=NULL;
	free(curr);
}
void sglModTranslate(float tx,float ty,float tz)
{
	node *temp;
	temp = (node *)malloc(sizeof(node));
	LoadIdentity(temp);
	temp->mat[0][3]=tx;
	temp->mat[1][3]=ty;
	temp->mat[2][3]=tz;
	mult_matrix(temp->mat);
}
void sglModMatrix(float mat[])
{
	int i,j;
	float c[4][4];
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			c[i][j]=mat[4*i + j];
	mult_matrix(c);
}
void sglModScale(float sx,float sy,float sz)
{
	node *temp;
	temp = (node *)malloc(sizeof(node));
	LoadIdentity(temp);
	temp->mat[0][0]=sx;
	temp->mat[1][1]=sy;
	temp->mat[2][2]=sz;
	mult_matrix(temp->mat);
}
void sglModRotate(float angle,float x,float y,float z)
{
	float d = sqrt(x*x + y*y + z*z);
	x=x/d;
	y=y/d;
	z=z/d;
	float c = cos(DEG2RAD(angle));
	float s = sin(DEG2RAD(angle));
	float m[4][4];
	m[0][0]=x*x*(1-c) + c;
	m[0][1]=x*y*(1-c) - (z*s);
	m[0][2]=x*z*(1-c) + (y*s);
	m[0][3]=0;
	m[1][0]=x*y*(1-c) + (z*s);
	m[1][1]=y*y*(1-c) + c;;
	m[1][2]=y*z*(1-c) - (x*s);
	m[1][3]=0;
	m[2][0]=x*z*(1-c) - (y*s);
	m[2][1]=y*z*(1-c) + (x*s);
	m[2][2]=z*z*(1-c) + c;
	m[2][3]=0;
	m[3][0]=0;
	m[3][1]=0;
	m[3][2]=0;
	m[3][3]=1;
	mult_matrix(m);
}
void sglViewLoadIdentity()
{
	view = (node *)malloc(sizeof(node));
	LoadIdentity(view);
}
void sglViewTranslate(float tx,float ty,float tz)
{
	node *temp;
	temp = (node *)malloc(sizeof(node));
	LoadIdentity(temp);
	temp->mat[0][3]=-tx;
	temp->mat[1][3]=-ty;
	temp->mat[2][3]=-tz;
	mult_view_matrix(temp->mat);
}
void sglViewRotate(float angle, float x, float y, float z)
{
	float d = sqrt(x*x + y*y + z*z);
	x=x/d;
	y=y/d;
	z=z/d;
	float c = cos(DEG2RAD(-angle));
	float s = sin(DEG2RAD(-angle));
	float m[4][4];
	m[0][0]=x*x*(1-c) + c;
	m[0][1]=x*y*(1-c) - (z*s);
	m[0][2]=x*z*(1-c) + (y*s);
	m[0][3]=0;
	m[1][0]=x*y*(1-c) + (z*s);
	m[1][1]=y*y*(1-c) + c;;
	m[1][2]=y*z*(1-c) - (x*s);
	m[1][3]=0;
	m[2][0]=x*z*(1-c) - (y*s);
	m[2][1]=y*z*(1-c) + (x*s);
	m[2][2]=z*z*(1-c) + c;
	m[2][3]=0;
	m[3][0]=0;
	m[3][1]=0;
	m[3][2]=0;
	m[3][3]=1;
	mult_view_matrix(m);
}
void sglViewMatrix(float mat[])
{
	int i,j;
	float c[4][4];
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			c[i][j]=mat[4*i + j];
	mult_view_matrix(c);
}
void sglLookAt(float cx,float cy,float cz,float lx,float ly,float lz,float upx,float upy,float upz)
{
	float a[3];
	a[0]=lx-cx;
	a[1]=ly-cy;
	a[2]=lz-cz;
	float b[3];
	b[0]=upx;
	b[1]=upy;
	b[2]=upz;
	float d=sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]);
	a[0]/=d;
	a[1]/=d;
	a[2]/=d;
	d=sqrt(b[0]*b[0] + b[1]*b[1] + b[2]*b[2]);
	b[0]/=d;
	b[1]/=d;
	b[2]/=d;
	float s[3],u[3];
	s[0]=a[1]*b[2] - b[1]*a[2];
	s[1]=a[2]*b[0] - b[2]*a[0];
	s[2]=a[0]*b[1] - b[0]*a[1];
	d=sqrt(s[0]*s[0] + s[1]*s[1] + s[2]*s[2]);
	s[0]/=d;
	s[1]/=d;
	s[2]/=d;
	u[0]=s[1]*a[2] - a[1]*s[2];
	u[1]=s[2]*a[0] - a[2]*s[0];
	u[2]=s[0]*a[1] - a[0]*s[1];
	d=sqrt(u[0]*u[0] + u[1]*u[1] + u[2]*u[2]);
	u[0]/=d;
	u[1]/=d;
	u[2]/=d;
	float m[4][4];
	m[0][0]=s[0];
	m[0][1]=s[1];
	m[0][2]=s[2];
	m[0][3]=0;
	m[1][0]=u[0];
	m[1][1]=u[1];
	m[1][2]=u[2];
	m[1][3]=0;
	m[2][0]=-a[0];
	m[2][1]=-a[1];
	m[2][2]=-a[2];
	m[2][3]=0;
	m[3][0]=0;
	m[3][1]=0;
	m[3][2]=0;
	m[3][3]=1;
	mult_view_matrix(m);
	sglViewTranslate(cx,cy,cz);
}
void sglProjOrtho(float left,float right,float bottom,float top,float near,float far)
{
	projection[0][0]=2/(right-left);
	projection[0][1]=0;
	projection[0][2]=0;
	projection[0][3]=-(right+left)/(right-left);
	projection[1][0]=0;
	projection[1][1]=2/(top-bottom);
	projection[1][2]=0;
	projection[1][3]=-(top+bottom)/(top-bottom);
	projection[2][0]=0;
	projection[2][1]=0;
	projection[2][2]=2/(far-near);
	projection[2][3]=-(far+near)/(far-near);
	projection[3][0]=0;
	projection[3][1]=0;
	projection[3][2]=0;
	projection[3][3]=1;
}
void sglProjFrustum(float left,float right,float bottom,float top,float near,float far)
{
	projection[0][0]=2*near/(right-left);
	projection[0][1]=0;
	projection[0][2]=(right+left)/(right-left);
	projection[0][3]=0;
	projection[1][0]=0;
	projection[1][1]=2*near/(top-bottom);
	projection[1][2]=(top+bottom)/(top-bottom);
	projection[1][3]=0;
	projection[2][0]=0;
	projection[2][1]=0;
	projection[2][2]=-(far+near)/(far-near);
	projection[2][3]=-2*far*near/(far-near);
	projection[3][0]=0;
	projection[3][1]=0;
	projection[3][2]=-1;
	projection[3][3]=0;
}
void sglViewport(float llx,float lly,float width,float height)
{
	w1=(float)glutGet(GLUT_WINDOW_WIDTH);
	h1=(float)glutGet(GLUT_WINDOW_HEIGHT);
	x=llx;
	y=lly;
	wid = width;
	ht = height;
}
void sglBegin()
{
	if(flag_show==0)
	{
		free(vertex_h);
		vertex_h = (vertex *)malloc(sizeof(vertex));
		vertex_h->next = NULL;
		vertex_h_copy=vertex_h;
	}
	else
	{
		vertex *temp;
		temp=vertex_h;
		while(temp->next!=NULL)
			temp=temp->next;
		vertex_h_copy=temp;
	}
	
	flag_show=1;
}
void sglColour(float r,float g,float b)
{
	red=r;
	green=g;
	blue=b;
}
void sglshow()
{
	vertex *t=vertex_h;
	while(t->next!=NULL)
	{
		t=t->next;
		glBegin(GL_TRIANGLES);
		glColor3f(t->r,t->g,t->b);
		glVertex2f(t->mat[0][0],t->mat[1][0]);
	x_arr[counter]=t->mat[0][0];
	y_arr[counter]=t->mat[1][0];
	z_arr[counter]=t->mat[2][0];
	counter++;
		t=t->next;
		glColor3f(t->r,t->g,t->b);
		glVertex2f(t->mat[0][0],t->mat[1][0]);
	x_arr[counter]=t->mat[0][0];
	y_arr[counter]=t->mat[1][0];
	z_arr[counter]=t->mat[2][0];
	counter++;
		t=t->next;
		glColor3f(t->r,t->g,t->b);
		glVertex2f(t->mat[0][0],t->mat[1][0]);
	x_arr[counter]=t->mat[0][0];
	y_arr[counter]=t->mat[1][0];
	z_arr[counter]=t->mat[2][0];
	counter++;
		glEnd();
	}
	flag_show=0;
}
void sglEnd()
{
	calculations();
//	sglshow();
}
void sglVertex(float x,float y,float z)
{
	vertex *temp;
	vertex *temp2;
	temp2=vertex_h_copy;
	temp = (vertex *)malloc(sizeof(vertex));
	temp->mat[0][0] = x;
	temp->mat[1][0] = y;
	temp->mat[2][0] = z;
	temp->mat[3][0] = 1;
	temp->r=red;
	temp->b=blue;
	temp->g=green;
	while(temp2->next!=NULL)
		temp2=temp2->next;
	temp2->next=temp;
	temp->next = NULL;
}
void sglClear(float r, float g, float b)
{
	glClearColor(r , g , b , 1.0f);
}
