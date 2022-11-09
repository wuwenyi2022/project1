#include<cstdio>
#include<vector>
#include<iostream>
#include <map>
#include <iterator>
#include <assert.h>
#include <fstream>
#include <vector>

using namespace std;
struct Vertex;
struct HalfEdge;
struct Loop;
struct Edge;
struct Face;
struct Solid;
struct Vertex {
	int vid;
	double coordinate[3];
	Vertex* next;
	Vertex* prev;
	HalfEdge* he;
	Vertex(int id, double x, double y, double z) {
		vid = id;
		coordinate[0] = x;
		coordinate[1] = y;
		coordinate[2] = z;
		next = this;
		prev = this;
		he = NULL;
	}
};
struct HalfEdge {
	//半边与点、半边、边、环相关
	Vertex* vtx;
	HalfEdge* prev;
	HalfEdge* next;
	Edge* edge;
	Loop* loop;
	HalfEdge(Vertex* v) {
		vtx = v;
		prev = this;
		next = this;
		edge = NULL;
		loop = NULL;
	}
};
struct Edge {
	HalfEdge* lhe; //the edge's left halfedge
	HalfEdge* rhe; //the edge's right halfedge
	Edge* next;
	Edge* prev;
	Edge() {
		lhe = NULL;
		rhe = NULL;
		next = NULL;
		prev = NULL;
	}
};
struct Loop {
	Loop* prev;
	Loop* next;
	HalfEdge* helist;
	Face* face;
	Loop() {
		prev = this;
		next = this;
		helist = NULL;
		face = NULL;
	}
};
struct Face {
	int fid;
	Face* prev;
	Face* next;
	Solid* solid;
	Loop* floopout;
	Loop* floopin;
	Face(int id) {
		fid = id;
		prev = this;
		next = this;
		solid = NULL;
		floopout = NULL;
		floopin = NULL;
	}
};
struct Solid {
	int sID;
	Solid* prev;
	Solid* next;
	Vertex* vtxlist;
	Face* facelist;
	Edge* edgelist;
	Solid() {
		sID = 1;
		prev = NULL;
		next = NULL;
		vtxlist = NULL;
		facelist = NULL;
		edgelist = NULL;
	}
};
//TODO1
void AddVtx(Solid* s, Vertex* v) {
	if (s->vtxlist == NULL) {
		s->vtxlist = v;
	}
	else {
		v->next = s->vtxlist;
		v->prev = s->vtxlist->prev;
		s->vtxlist->prev->next = v;
		s->vtxlist->prev = v;
	}
}
void AddFace(Solid* s, Face* f) {
	if (s->facelist == NULL) {
		s->facelist = f;
	}
	else {
		f->next = s->facelist;
		f->prev = s->facelist->prev;
		s->facelist->prev->next = f;
		s->facelist->prev = f;
	}
}
void AddEdge(Solid* s, Edge* e) {
	if (s->edgelist == NULL)
	{
		s->edgelist = e;
	}
	else
	{
		Edge* e1 = s->edgelist;
		e->next = e1;
		e->prev = e1->prev;
		e1->prev = e;
		s->edgelist = e;
	}
}
HalfEdge* AddHalfEdge(Edge* newedge, Vertex* v, HalfEdge* position, int sign)
{
	HalfEdge* he;
	if (position->edge == NULL)
	{
		he = position;
	}
	else
	{
		he = new HalfEdge(v);
		position->prev->next = he;
		he->prev = position->prev;
		position->prev = he;
		he->next = position;
	}
	he->edge = newedge;
	he->vtx = v;
	he->loop = position->loop;
	if (sign == 1)
	{
		newedge->lhe = he;
	}
	else
	{
		newedge->rhe = he;
	}
	return he;
}
void AddToLoops(Face* f, Loop* tp)
{
	if (f->floopin == NULL)
	{
		f->floopin = tp;
	}
	else
	{
		tp->next = f->floopin;
		tp->prev = f->floopin->prev;
		f->floopin->prev->next = tp;
		f->floopin->prev = tp;
	}
}

//TODO2
Face* findface(Solid* s, int fn)
{
	Face* f;
	f = s->facelist;
	while (f)
	{
		if (f->fid == fn)
		{
			return f;
		}
		f = f->next;
		if (f == s->facelist)
		{
			break;
		}
	}
	return NULL;
}
Vertex* findvertex(Solid* s, int vid) {
	Vertex* v;
	v = s->vtxlist;
	while (v) {
		if (v->vid == vid) {
			return v;
		}
		v = v->next;
		if (v == s->vtxlist) {
			break;
		}
	}
	return NULL;
}
HalfEdge* findhalfedge(Loop* lp, int vid) {
	HalfEdge* he = lp->helist;
	while (he) {
		if (he->vtx->vid == vid)
		{
			break;
		}
		he = he->next;
	}
	return he;
}

//TODO3
HalfEdge* DelHalfEdge(HalfEdge* he)
{
	HalfEdge* tmp;
	if (he->edge == NULL)
	{
		delete he;
		return NULL;
	}
	else if (he->next == he)
	{
		he->edge = NULL;
		return he;
	}
	else
	{
		he->prev->next = he->next;
		he->next->prev = he->prev;
		tmp = he->prev;
		delete he;
		return tmp;
	}
}
Edge* DelEdge(Solid* s, Edge* e)
{
	Edge* tmp;
	tmp = s->edgelist;
	while (tmp)
	{
		if (tmp == e)
		{
			tmp->next->prev = tmp->prev;
			tmp->prev->next = tmp->next;
			return tmp;
		}
		tmp = tmp->next;
		if (tmp == s->edgelist)
		{
			break;
		}
	}
	return NULL;
}
Face* DelFace(Solid* s, Face* f)
{
	Face* tmp;
	tmp = s->facelist;
	while (tmp)
	{
		if (tmp == f)
		{
			tmp->next->prev = tmp->prev;
			tmp->prev->next = tmp->next;
			if (f == s->facelist)
			{
				s->facelist = tmp->next;
			}
			return tmp;
		}
		tmp = tmp->next;
		if (tmp == s->facelist)
		{
			break;
		}
	}
	return NULL;
}
void getmaxnames(Solid* s)
{
	Vertex* v;
	Face* f;
	v = s->vtxlist;
	int maxv = 0;
	while (v)
	{
		if (v->vid > maxv)
		{
			maxv = v->vid;
		}
		v = v->next;
		if (v == s->vtxlist)
		{
			break;
		}
	}
	f = s->facelist;
	int maxf = 0;
	while (f)
	{
		if (f->fid > maxf)
		{
			maxf = f->fid;
		}
		f = f->next;
		if (f == s->facelist)
		{
			break;
		}
	}
}
int getvtxlistsize(Solid* solid) {
	int cnt = 0;
	Vertex* vtxlist = solid->vtxlist;
	Vertex* v = solid->vtxlist;
	while (v) {
		cnt++;
		v = v->next;
		if (v == vtxlist) {
			break;
		}
	}
	return cnt;
}
int getfacelistsize(Solid* solid) {
	int cnt = 0;
	Face* flist = solid->facelist;
	Face* f = solid->facelist;
	while (f) {
		cnt++;
		f = f->next;
		if (f = flist) {
			break;
		}
	}
	return cnt;
}


//这里实现欧拉操作
Solid* mvsf(int fid, int vid, double* point) {
	//新建结构
	Vertex* v = new Vertex(vid, point[0], point[1], point[2]);
	HalfEdge* he = new HalfEdge(v);
	Loop* l = new Loop();
	Face* f = new Face(fid);
	Solid* s = new Solid();
	//构建依赖关系
	f->solid = s;
	f->floopout = l;
	l->helist = he;
	l->face = f;
	he->loop = l;
	AddFace(s, f);
	AddVtx(s, v);
	return s;
}


void mev(Solid* solid, int f1, int v1, int v4, double* point)
{
	//新建结构，一个新点，一条边，两个半边
	Vertex* newvertex;
	Edge* newedge;
	newedge = new Edge();
	newvertex = new Vertex(v4, point[0], point[1], point[2]);
	AddVtx(solid, newvertex);
	AddEdge(solid, newedge);

	//查找相关内容，面，半边
	Face* face = findface(solid, f1);
	HalfEdge* he = NULL;
	Loop* lp;
	bool exist = true;
	lp = face->floopout;
	he = lp->helist;
	while (he)
	{
		if (he->vtx->vid == v1)
		{
			break;
		}
		he = he->next;
		if (he == lp->helist)
		{
			cout << "找不到" << endl;
			return;
		}
	}

	//构建依赖

	AddHalfEdge(newedge, he->vtx, he, 0);
	AddHalfEdge(newedge, newvertex, he, 1);
	newvertex->he = he->prev;
	he->vtx->he = he;

}


void mef(Solid* solid, int f1, int v1, int v3, int f2)
{
	//新建结构，一个边，两个半边，一个面，一个环
	Edge* enew = new Edge();
	AddEdge(solid, enew);
	HalfEdge* nhe1, * nhe2;
	Face* fnew = new Face(f2);
	AddFace(solid, fnew);
	Loop* lnew = new Loop();

	//查找相关内容，面，起始点对应的半边，终止点对应的半边
	Face* face1 = findface(solid, f1);
	Loop* lp;
	HalfEdge* he1 = NULL, * he2 = NULL;
	lp = face1->floopout;
	he1 = findhalfedge(lp, v1);
	he2 = findhalfedge(lp, v3);

	//构建依赖,面需要一个外环，外环需要一个面，一个半边序列
	//修改两个外环的半边序列，只要在两个点处修改即可
	HalfEdge* he;
	lnew->face = fnew;
	fnew->floopout = lnew;
	he = he1;
	while (he != he2)
	{
		he->loop = lnew;
		he = he->next;
	}
	nhe2 = AddHalfEdge(enew, he1->vtx, he2, 1);
	nhe1 = AddHalfEdge(enew, he2->vtx, he1, 0);

	nhe1->prev->next = nhe2;
	nhe2->prev->next = nhe1;
	he = nhe1->prev;
	nhe1->prev = nhe2->prev;
	nhe2->prev = he;

	lnew->helist = nhe1;
	he2->loop->helist = nhe2;

}


bool kemr(Solid* solid, int fn, int v1, int v2)
{
	Face* face;
	Loop* lp;
	HalfEdge* he, * h1 = NULL, * h2 = NULL;
	Vertex* v;
	face = findface(solid, fn);
	lp = face->floopout;
	he = lp->helist;
	while (he)
	{
		if ((he->vtx->vid == v1) && (he->next->vtx->vid == v2))
		{
			h1 = he;
			break;
		}
		he = he->next;
		if (he == lp->helist)
		{
			fprintf(stderr, "kemr:error.\n");
			return false;
		}
	}
	he = lp->helist;
	while (he)
	{
		if ((he->vtx->vid == v2) && (he->next->vtx->vid == v1))
		{
			h2 = he;
			break;
		}
		he = he->next;
		if (he == lp->helist)
		{
			fprintf(stderr, "kemr:error\n");
			return false;
		}
	}

	HalfEdge* h3, * h4;
	Loop* oldloop;
	Loop* newloop;
	Edge* killedge;
	oldloop = h1->loop;
	newloop = new Loop();
	//modify
	int a = 0;
	if (h1->next == h1->next->edge->lhe) {
		a = 1;
	}
	switch (a) {
	case 1:
		oldloop->face->floopin = (h1->next)->edge->rhe->loop->face->floopin;
		break;
	case 0:
		oldloop->face->floopin = (h1->next)->edge->lhe->loop->face->floopin;
		break;
	}

	a = 0;
	if (h1->next == h1->next->edge->lhe) {
		a = 1;
	}
	switch (a) {
	case 1:
		h1->next->edge->rhe->loop->face->floopin = NULL;
		break;
	case 0:
		(h1->next)->edge->lhe->loop->face->floopin = NULL;
		break;
	}

	//end modify
	AddToLoops(oldloop->face, newloop);
	//oldloop->face->floops = newloop;
	killedge = h1->edge;
	newloop->face = oldloop->face;

	h3 = h1->next;
	h1->next = h2->next;
	h2->next->prev = h1;
	h2->next = h3;
	h3->prev = h2;

	h4 = h2;
	do
	{
		h4->loop = newloop;
	} while ((h4 = h4->next) != h2);

	oldloop->helist = h3 = DelHalfEdge(h1);
	newloop->helist = h4 = DelHalfEdge(h2);

	h3->vtx->he = (h3->edge) ? h3 : (HalfEdge*)NULL;
	h4->vtx->he = (h4->edge) ? h4 : (HalfEdge*)NULL;

	delete DelEdge(solid, killedge);
	return true;
}


void kfmrh(Solid* solid, int fn, int ftodel)
{
	Face* face1 = findface(solid, fn);
	Face* face2 = findface(solid, ftodel);
	Loop* lptmp;
	lptmp = face2->floopout;
	AddToLoops(face1, lptmp);
	if (face2->floopin != NULL)
	{
		lptmp = face2->floopin;
		while (lptmp != face2->floopin)
		{
			lptmp = lptmp->prev;
			AddToLoops(face1, lptmp->next);
		}
		AddToLoops(face1, lptmp);

	}
	delete DelFace(solid, face2);
}


void sweep(Solid* tmp, int fid, double vx, double vy, double vz)
{
	Face* face;
	Loop* lp;
	HalfEdge* first, * scan, * hhe1 = NULL, * hhe2 = NULL;
	Vertex* v;
	int maxv;
	face = findface(tmp, fid);
	getmaxnames(tmp);
	int maxf = getfacelistsize(tmp);
	lp = face->floopout;
	while (lp)
	{
		first = lp->helist;
		scan = first->next;
		v = scan->vtx;
		if (1) {
			maxv = getvtxlistsize(tmp);
			double point[3] = { v->coordinate[0] + vx,
				v->coordinate[1] + vy,
				v->coordinate[2] + vz };
			HalfEdge* he1, * he2;
			he1 = scan;
			he2 = scan;
			HalfEdge* he;
			Vertex* newvertex;
			Edge* newedge;
			newedge = new Edge();
			newvertex = new Vertex(maxv, point[0], point[1], point[2]);
			AddVtx(tmp, newvertex);
			AddEdge(tmp, newedge);
			he = he1;
			while (he != he2)
			{
				he->vtx = newvertex;
				int a = 0;
				if (he == (he)->edge->lhe) {
					a = 1;
				}
				switch (a)
				{
				case 1:
					he = (he)->edge->rhe;
					break;
				case 0:
					he = (he)->edge->lhe;
				}
			}
			AddHalfEdge(newedge, he2->vtx, he1, 0);
			AddHalfEdge(newedge, newvertex, he2, 1);
			newvertex->he = he2->prev;
			he2->vtx->he = he2;
		}
		while (scan != first)
		{
			v = scan->next->vtx;
			maxv = getvtxlistsize(tmp);
			if (1) {
				maxv = getvtxlistsize(tmp);
				double point[3] = { v->coordinate[0] + vx,
					v->coordinate[1] + vy,
					v->coordinate[2] + vz };
				HalfEdge* he1, * he2;
				he1 = scan->next;
				he2 = scan->next;
				HalfEdge* he;
				Vertex* newvertex;
				Edge* newedge;
				newedge = new Edge();
				newvertex = new Vertex(maxv, point[0], point[1], point[2]);
				AddVtx(tmp, newvertex);
				AddEdge(tmp, newedge);
				he = he1;
				while (he != he2)
				{
					he->vtx = newvertex;
					int a = 0;
					if (he == (he)->edge->lhe) {
						a = 1;
					}
					switch (a)
					{
					case 1:
						he = (he)->edge->rhe;
						break;
					case 0:
						he = (he)->edge->lhe;
					}
				}
				AddHalfEdge(newedge, he2->vtx, he1, 0);
				AddHalfEdge(newedge, newvertex, he2, 1);
				newvertex->he = he2->prev;
				he2->vtx->he = he2;
			}
			if (1) {
				int fn = ++maxf;
				hhe1 = scan->prev;
				hhe2 = scan->next->next;
				Face* fnew = new Face(fn);
				Loop* lnew = new Loop();
				Edge* enew = new Edge();
				Face* fold = hhe1->loop->face;
				AddEdge(tmp, enew);
				AddFace(tmp, fnew);
				HalfEdge* he, * nhe1, * nhe2;
				lnew->face = fnew;
				fnew->floopout = lnew;
				he = hhe1;
				while (he != hhe2)
				{
					he->loop = lnew;
					he = he->next;
				}
				nhe2 = AddHalfEdge(enew, hhe1->vtx, hhe2, 1);
				nhe1 = AddHalfEdge(enew, hhe2->vtx, hhe1, 0);

				nhe1->prev->next = nhe2;
				nhe2->prev->next = nhe1;
				he = nhe1->prev;
				nhe1->prev = nhe2->prev;
				nhe2->prev = he;

				lnew->helist = nhe1;
				hhe2->loop->helist = nhe2;
			}
			if (scan->next == scan->next->edge->lhe) {
				scan = scan->next->edge->rhe->next;
			}
			else {
				scan = scan->next->edge->lhe->next;
			}
		}
		if (1) {
			int fn = ++maxf;
			hhe1 = scan->prev;
			hhe2 = scan->next->next;
			Face* fnew = new Face(fn);
			Loop* lnew = new Loop();
			Edge* enew = new Edge();
			Face* fold = hhe1->loop->face;
			AddEdge(tmp, enew);
			AddFace(tmp, fnew);
			HalfEdge* he, * nhe1, * nhe2;
			lnew->face = fnew;
			fnew->floopout = lnew;
			he = hhe1;
			while (he != hhe2)
			{
				he->loop = lnew;
				he = he->next;
			}
			nhe2 = AddHalfEdge(enew, hhe1->vtx, hhe2, 1);
			nhe1 = AddHalfEdge(enew, hhe2->vtx, hhe1, 0);

			nhe1->prev->next = nhe2;
			nhe2->prev->next = nhe1;
			he = nhe1->prev;
			nhe1->prev = nhe2->prev;
			nhe2->prev = he;

			lnew->helist = nhe1;
			hhe2->loop->helist = nhe2;
		}

		if (lp == face->floopout)
		{
			lp = face->floopin;
		}
		else
		{
			lp = lp->next;
			if (lp == face->floopin)
			{
				break;
			}
		}
	}

}


void wirepaint(Solid* s) {
	glBegin(GL_LINES);
	Edge* tmp;
	tmp = s->edgelist;
	while (tmp)
	{
		glVertex3f(tmp->lhe->vtx->coordinate[0], tmp->lhe->vtx->coordinate[1], tmp->lhe->vtx->coordinate[2]);
		glVertex3f(tmp->rhe->vtx->coordinate[0], tmp->rhe->vtx->coordinate[1], tmp->rhe->vtx->coordinate[2]);
		tmp = tmp->next;
		if (tmp == s->edgelist)
		{
			break;
		}
	}
	glEnd();
}


void Print(Solid* s, const std::string& path)
{
	//创建流对象
	std::ofstream ofs{ path };
	ofs << "BRP" << std::endl;

	Face* f = s->facelist;
	Loop* lp;
	HalfEdge* he;
	while (f)
	{
		ofs << "Face ID:" << f->fid << endl;
		lp = f->floopout;
		ofs << "\tThe out Loop:\n";
		he = lp->helist;
		while (he)
		{
			ofs << he->vtx->vid << ' ';
			he = he->next;
			if (he == lp->helist)
			{
				break;
			}
		}
		ofs << endl;
		f = f->next;
		if (f == s->facelist)
		{
			break;
		}
	}
}
