#ifndef CP_POLYGON_H
#define CP_POLYGON_H

#include <iostream>
using namespace std;
#include <vector>  //保存数组

#define DOUBLE_PI           6.28318530717958647692
#define PI                  3.14159265358979323846
#define HALF_PI             1.57079632679489661923
#define eps 1e-8//精度范围
#define INFINITY -10000//定义无穷远为-10000，用来产生“伪射线”
#define zero(x) (((x)>0?(x):-(x))<eps)//判断是不是0,如果是0返回true，不是0返回false

/*点类*/
class CP_Point
{
public:
    double m_x, m_y; 
public:
    CP_Point(void):m_x(0.0),m_y(0.0){};
	bool operator==(const CP_Point& c) const//重载==运算符为比较点是否重合
	{
		 return zero(c.m_x-m_x)&&zero(c.m_y-m_y);//坐标必须完全匹配
	}
}; // 类CP_Point定义结束
typedef vector<CP_Point> VT_PointArray;//点集合

class CP_Polygon;//先声明，其他类里直接用

typedef vector<int> VT_IntArray;//整数向量（点序号集合）
typedef vector<VT_IntArray> VT_IntArray2; //点集的向量（点序号集合的集合）

/*定义一个线段的结构*/
struct Segment
{
	CP_Point u1, u2;//线段的端点
	bool operator==(const Segment& c) const//重载==运算符为比较线段是否相等
	{
		 return zero(c.u1.m_x-u1.m_x)&&zero(c.u1.m_y-u1.m_y)&&zero(c.u2.m_x-u2.m_x)&&zero(c.u2.m_y-u2.m_y);//坐标必须完全匹配
	}
};

/*环类*/
class CP_Loop
{
public:
    VT_IntArray m_pointIDArray;//点序号集合
    int m_loopIDinRegion;//第几个环
    int m_regionIDinPolygon;//第几个区域
    CP_Polygon* m_polygon;//属于哪个多边形

public:
    CP_Loop(void):m_loopIDinRegion(0),m_regionIDinPolygon(0),m_polygon(NULL) { }
}; // 类CP_Loop定义结束
typedef vector<CP_Loop> VT_LoopArray;//多边形向量

/*区域类*/
class CP_Region
{
public:
    VT_LoopArray m_loopArray;//区域中环集合
    int m_regionIDinPolygon;//第几个区域
    CP_Polygon* m_polygon; //属于哪个多边形

public:
    CP_Region(void):m_regionIDinPolygon(0),m_polygon(NULL) { }
}; // 类CP_Region定义结束
typedef vector<CP_Region> VT_RegionArray;

/*多边形类*/
class CP_Polygon
{
public:
    VT_PointArray m_pointArray;//点坐标集合
    VT_RegionArray m_regionArray;//区域集合

public:
    void mb_clear( ) {m_pointArray.clear( ); m_regionArray.clear( );}//清空多边形，包括点、区域
}; // 类CP_Polygon定义结束

extern void     gb_distanceMinPointLoop(double&d, int& idRegion, int& idLoop,
               CP_Point& pt, CP_Polygon& pn);
// 点和区域的最小距离
extern void     gb_distanceMinPointPolygon(double&d, int& id, CP_Point& pt, CP_Polygon& pn);
//点到多边形边的距离
extern double   gb_distancePointPoint(CP_Point& p1, CP_Point& p2);
//点到点
extern double   gb_distancePointSegment(CP_Point& pt, CP_Point& p1, CP_Point& p2);
//点到线段----注与直线区别

//------重合点拾取
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
extern void     gb_pointConvertFromGlobalToScreen(CP_Point& result, CP_Point pointGlobal, double scale, CP_Point translation, int screenX, int screenY);//真实世界坐标系与屏幕坐标系的坐标变换
extern void     gb_pointConvertFromScreenToGlobal(CP_Point& result, CP_Point pointScreen, double scale, CP_Point translation, int screenX, int screenY);
extern bool     gb_polygonNewInLoopRegular(CP_Polygon& p, int idRegion, int n, double r, double cx, double cy);
extern void     gb_polygonNewOutLoopRegular(CP_Polygon& p, int n, double r, double cx, double cy);
extern bool     gb_removeLoop(CP_Polygon& pn, int idRegion, int idLoop); 
//环中顶点个数3则环不存在了
extern bool     gb_removePoint(CP_Polygon& pn, int id);
extern bool     gb_removeRegion(CP_Polygon& pn, int idRegion);
extern void     gb_subtractOneAboveID(CP_Polygon& pn, int id);
//比id大的序号都要-1
#endif
