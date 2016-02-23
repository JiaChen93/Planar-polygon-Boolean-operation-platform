#ifndef CP_POLYGON_H
#define CP_POLYGON_H

#include <iostream>
using namespace std;
#include <vector>  //��������

#define DOUBLE_PI           6.28318530717958647692
#define PI                  3.14159265358979323846
#define HALF_PI             1.57079632679489661923
#define eps 1e-8//���ȷ�Χ
#define INFINITY -10000//��������ԶΪ-10000������������α���ߡ�
#define zero(x) (((x)>0?(x):-(x))<eps)//�ж��ǲ���0,�����0����true������0����false

/*����*/
class CP_Point
{
public:
    double m_x, m_y; 
public:
    CP_Point(void):m_x(0.0),m_y(0.0){};
	bool operator==(const CP_Point& c) const//����==�����Ϊ�Ƚϵ��Ƿ��غ�
	{
		 return zero(c.m_x-m_x)&&zero(c.m_y-m_y);//���������ȫƥ��
	}
}; // ��CP_Point�������
typedef vector<CP_Point> VT_PointArray;//�㼯��

class CP_Polygon;//����������������ֱ����

typedef vector<int> VT_IntArray;//��������������ż��ϣ�
typedef vector<VT_IntArray> VT_IntArray2; //�㼯������������ż��ϵļ��ϣ�

/*����һ���߶εĽṹ*/
struct Segment
{
	CP_Point u1, u2;//�߶εĶ˵�
	bool operator==(const Segment& c) const//����==�����Ϊ�Ƚ��߶��Ƿ����
	{
		 return zero(c.u1.m_x-u1.m_x)&&zero(c.u1.m_y-u1.m_y)&&zero(c.u2.m_x-u2.m_x)&&zero(c.u2.m_y-u2.m_y);//���������ȫƥ��
	}
};

/*����*/
class CP_Loop
{
public:
    VT_IntArray m_pointIDArray;//����ż���
    int m_loopIDinRegion;//�ڼ�����
    int m_regionIDinPolygon;//�ڼ�������
    CP_Polygon* m_polygon;//�����ĸ������

public:
    CP_Loop(void):m_loopIDinRegion(0),m_regionIDinPolygon(0),m_polygon(NULL) { }
}; // ��CP_Loop�������
typedef vector<CP_Loop> VT_LoopArray;//���������

/*������*/
class CP_Region
{
public:
    VT_LoopArray m_loopArray;//�����л�����
    int m_regionIDinPolygon;//�ڼ�������
    CP_Polygon* m_polygon; //�����ĸ������

public:
    CP_Region(void):m_regionIDinPolygon(0),m_polygon(NULL) { }
}; // ��CP_Region�������
typedef vector<CP_Region> VT_RegionArray;

/*�������*/
class CP_Polygon
{
public:
    VT_PointArray m_pointArray;//�����꼯��
    VT_RegionArray m_regionArray;//���򼯺�

public:
    void mb_clear( ) {m_pointArray.clear( ); m_regionArray.clear( );}//��ն���Σ������㡢����
}; // ��CP_Polygon�������

extern void     gb_distanceMinPointLoop(double&d, int& idRegion, int& idLoop,
               CP_Point& pt, CP_Polygon& pn);
// ����������С����
extern void     gb_distanceMinPointPolygon(double&d, int& id, CP_Point& pt, CP_Polygon& pn);
//�㵽����αߵľ���
extern double   gb_distancePointPoint(CP_Point& p1, CP_Point& p2);
//�㵽��
extern double   gb_distancePointSegment(CP_Point& pt, CP_Point& p1, CP_Point& p2);
//�㵽�߶�----ע��ֱ������

//------�غϵ�ʰȡ
extern void     gb_getIntArrayPointInPolygon(VT_IntArray& vi, CP_Polygon& pn, CP_Point& p, double eT);
extern bool     gb_findPointInLoop(CP_Polygon& pn, int& idRegion, int& idLoop, int& idPointInLoop, int pointInPolygon);
extern void     gb_insertPointInPolygon(CP_Polygon& pn, int& idRegion, int& idLoop, int& idPointInLoop, CP_Point& newPoint);

extern void     gb_intArrayInit(VT_IntArray& vi, int data);
extern void     gb_intArrayInitLoop(VT_IntArray& vi, CP_Polygon& pn, int idRgion, int idLoop, double eT);
extern void     gb_intArrayInitPoint(VT_IntArray& vi, CP_Polygon& pn, int v, double eT);
extern void     gb_intArrayInitPointSame(VT_IntArray& vi, CP_Polygon& pn, double eT);
extern void     gb_intArrayInitPolygon(VT_IntArray& vi, CP_Polygon& pn);
extern void     gb_intArrayInitPolygonSamePoint(VT_IntArray& vr, CP_Polygon& pr, VT_IntArray& vs, CP_Polygon& ps, double eT);
extern void     gb_intArrayInitRegion(VT_IntArray& vi, CP_Polygon& pn, int idRegion, double eT);

extern void     gb_moveLoop(CP_Polygon& pn, int idRegion, int idLoop, double vx, double vy);
extern void     gb_movePoint(CP_Polygon& pn, int id, double vx, double vy);
extern void     gb_movePointIntArray(CP_Polygon& pn, VT_IntArray& vi, double vx, double vy);
extern void     gb_movePolygon(CP_Polygon& pn, double vx, double vy);
extern void     gb_moveRegion(CP_Polygon& pn, int idRegion, double vx, double vy);
extern void     gb_pointConvertFromGlobalToScreen(CP_Point& result, CP_Point pointGlobal, double scale, CP_Point translation, int screenX, int screenY);//��ʵ��������ϵ����Ļ����ϵ������任
extern void     gb_pointConvertFromScreenToGlobal(CP_Point& result, CP_Point pointScreen, double scale, CP_Point translation, int screenX, int screenY);
extern bool     gb_polygonNewInLoopRegular(CP_Polygon& p, int idRegion, int n, double r, double cx, double cy);
extern void     gb_polygonNewOutLoopRegular(CP_Polygon& p, int n, double r, double cx, double cy);
extern bool     gb_removeLoop(CP_Polygon& pn, int idRegion, int idLoop); 
//���ж������3�򻷲�������
extern bool     gb_removePoint(CP_Polygon& pn, int id);
extern bool     gb_removeRegion(CP_Polygon& pn, int idRegion);
extern void     gb_subtractOneAboveID(CP_Polygon& pn, int id);
//��id�����Ŷ�Ҫ-1
#endif
