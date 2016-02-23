// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://go.microsoft.com/fwlink/?LinkId=238214。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// CP_PolygonPlatformView.cpp : CCP_PolygonPlatformView 类的实现
//

#include "stdafx.h"
#include<algorithm>
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "CP_PolygonPlatform.h"
#endif

#include "CP_PolygonPlatformDoc.h"
#include "CP_PolygonPlatformView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif





CP_Polygon tempA, tempB;//分别为A和B添加交点后的多边形
CP_Polygon AboolB;//布尔运算结果
vector<Segment> SegCoinSame;//A、B重合且同向线段数组，【线段端点顺序为在A中的顺序】
vector<Segment> SegCoinReve;//A、B重合且反向线段数组，【线段端点顺序为在A中的顺序】

// CCP_PolygonPlatformView

IMPLEMENT_DYNCREATE(CCP_PolygonPlatformView, CView)

BEGIN_MESSAGE_MAP(CCP_PolygonPlatformView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CCP_PolygonPlatformView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_UPDATE_COMMAND_UI(ID_COMBO_AorB, &CCP_PolygonPlatformView::OnUpdateComboAorb)
	ON_COMMAND(ID_COMBO_AorB, &CCP_PolygonPlatformView::OnComboAorb)
	ON_COMMAND(ID_EDGE_NUMBER, &CCP_PolygonPlatformView::OnEdgeNumber)
	ON_COMMAND(ID_TOLERANCE, &CCP_PolygonPlatformView::OnTolerance)
	ON_COMMAND(ID_NEW_RIGHT_OUTLOOP, &CCP_PolygonPlatformView::OnNewRightOutloop)
	ON_COMMAND(ID_VIEW_STANDARD, &CCP_PolygonPlatformView::OnViewStandard)
	ON_COMMAND(ID_VIEW_FIT, &CCP_PolygonPlatformView::OnViewFit)
	ON_COMMAND(ID_SELECT_POINT, &CCP_PolygonPlatformView::OnSelectPoint)
	ON_UPDATE_COMMAND_UI(ID_SELECT_POINT, &CCP_PolygonPlatformView::OnUpdateSelectPoint)
	ON_UPDATE_COMMAND_UI(ID_SELECT_LOOP, &CCP_PolygonPlatformView::OnUpdateSelectLoop)
	ON_COMMAND(ID_SELECT_LOOP, &CCP_PolygonPlatformView::OnSelectLoop)
	ON_UPDATE_COMMAND_UI(ID_SELECT_REGION, &CCP_PolygonPlatformView::OnUpdateSelectRegion)
	ON_COMMAND(ID_SELECT_REGION, &CCP_PolygonPlatformView::OnSelectRegion)
	ON_UPDATE_COMMAND_UI(ID_SELECT_POLYGON, &CCP_PolygonPlatformView::OnUpdateSelectPolygon)
	ON_COMMAND(ID_SELECT_POLYGON, &CCP_PolygonPlatformView::OnSelectPolygon)
	ON_UPDATE_COMMAND_UI(ID_SELECT_TRIANGLE, &CCP_PolygonPlatformView::OnUpdateSelectTriangle)
	ON_COMMAND(ID_SELECT_TRIANGLE, &CCP_PolygonPlatformView::OnSelectTriangle)
	ON_COMMAND(ID_SELECT_ONLY, &CCP_PolygonPlatformView::OnSelectOnly)
	ON_UPDATE_COMMAND_UI(ID_SELECT_ONLY, &CCP_PolygonPlatformView::OnUpdateSelectOnly)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_NEW_RIGHT_INLOOP, &CCP_PolygonPlatformView::OnNewRightInloop)
	ON_COMMAND(ID_ADD_OUTLOOP, &CCP_PolygonPlatformView::OnAddOutloop)
	ON_COMMAND(ID_ADD_INLOOP, &CCP_PolygonPlatformView::OnAddInloop)
	ON_COMMAND(ID_ADD_POINT, &CCP_PolygonPlatformView::OnAddPoint)
	ON_COMMAND(ID_DELETE, &CCP_PolygonPlatformView::OnDelete)
	ON_UPDATE_COMMAND_UI(ID_MOVE_SAME, &CCP_PolygonPlatformView::OnUpdateMoveSame)
	ON_COMMAND(ID_MOVE_SAME, &CCP_PolygonPlatformView::OnMoveSame)
	ON_UPDATE_COMMAND_UI(ID_VIEW_A, &CCP_PolygonPlatformView::OnUpdateViewA)
	ON_COMMAND(ID_VIEW_A, &CCP_PolygonPlatformView::OnViewA)
	ON_UPDATE_COMMAND_UI(ID_VIEW_B, &CCP_PolygonPlatformView::OnUpdateViewB)
	ON_COMMAND(ID_VIEW_B, &CCP_PolygonPlatformView::OnViewB)
	ON_UPDATE_COMMAND_UI(ID_VIEW_POINT_ID, &CCP_PolygonPlatformView::OnUpdateViewPointId)
	ON_COMMAND(ID_VIEW_POINT_ID, &CCP_PolygonPlatformView::OnViewPointId)
	ON_COMMAND(ID_VIEW_T_FACE, &CCP_PolygonPlatformView::OnViewTFace)
	ON_COMMAND(ID_VIEW_T_EDGE, &CCP_PolygonPlatformView::OnViewTEdge)
	ON_UPDATE_COMMAND_UI(ID_VIEW_T_FACE, &CCP_PolygonPlatformView::OnUpdateViewTFace)
	ON_COMMAND(ID_POLYGON_UNION, &CCP_PolygonPlatformView::OnPolygonUnion)
	ON_COMMAND(ID_VIEW_RESULT, &CCP_PolygonPlatformView::OnViewResult)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESULT, &CCP_PolygonPlatformView::OnUpdateViewResult)
	ON_COMMAND(ID_POLYGON_INTERSECTION, &CCP_PolygonPlatformView::OnPolygonIntersection)
	ON_COMMAND(ID_POLYGON_A_B, &CCP_PolygonPlatformView::OnPolygonAB)
	ON_COMMAND(ID_POLYGON_B_A, &CCP_PolygonPlatformView::OnPolygonBA)
	ON_COMMAND(ID_CHECK, &CCP_PolygonPlatformView::OnCheck)
END_MESSAGE_MAP()

// CCP_PolygonPlatformView 构造/析构

CCP_PolygonPlatformView::CCP_PolygonPlatformView()
{
	// TODO: 在此处添加构造代码

}

CCP_PolygonPlatformView::~CCP_PolygonPlatformView()
{
}

//!!
void  CCP_PolygonPlatformView::mb_statusSetText(char* s1, char* s2)
{
	CRect r;
	// View中获取MainFrame指针
	CMainFrame *pMainFrame=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	//获取状态栏指针
	CMFCRibbonStatusBar *statusBar=
		(CMFCRibbonStatusBar *)pMainFrame->GetDescendantWindow(AFX_IDW_STATUS_BAR);
	//获取RibbonStatusBar上的元素
	CMFCRibbonLabel *locLabel;
	if (s1!=NULL)
	{
		locLabel=(CMFCRibbonLabel *)statusBar->GetElement(0);
		if (locLabel!=NULL)
		{
			r = locLabel->GetRect( );
			if (r.right-r.left<300)
				r.right+=300;
			locLabel->SetRect(&r);
			locLabel->SetText(s1);
		} // if结束
	} // 外if结束
	if (s2!=NULL)
	{
		locLabel=(CMFCRibbonLabel *)statusBar->GetExElement(0);
		if (locLabel!=NULL)
		{
			r = locLabel->GetRect( );
			if (r.right-r.left<300)
				r.left-=300;
			locLabel->SetRect(&r);
			locLabel->SetText(s2);
		} // if结束
	} // 外if结束
	statusBar->Invalidate( );
} // 类CCP_PolygonPlatformView的成员函数mb_statusSetText结束

//!!
BOOL CCP_PolygonPlatformView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CCP_PolygonPlatformView 绘制

void CCP_PolygonPlatformView::OnDraw(CDC* pDC)
{
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	CRect r;
	GetClientRect(& r);
	if (pDoc->m_flagShowTriangleFace)
	{
		CBrush brush(RGB(155, 155, 50));
		CBrush* brushOld = (CBrush*)pDC->SelectObject(&brush);;
		CPoint p[3];
		p[0].x = 100;
		p[0].y = 100;
		p[1].x = 200;
		p[1].y = 100;
		p[2].x = 200;
		p[2].y = 200;
		CRgn rgn;
		rgn.CreatePolygonRgn(p, 3, ALTERNATE); 
		pDC->FillRgn(&rgn, &brush); 
		pDC->SelectObject(brushOld);
	} // if (pDoc->m_flagShowTriangleFace)结束
	if (!pDoc->m_flagShowSelect)
	{
		if (pDoc->m_flagShowA)//显示A
		{
			gb_drawPolygonLoop(pDC, pDoc->m_a,
				pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
				255, 0, 0,
				0, 255, 0,
				3);
			gb_drawPolygonPoint(pDC, pDoc->m_a,
				pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
				0, 0, 0,
				8);
			if (pDoc->m_flagShowPointID)
				gb_drawPolygonPointID(pDC, pDoc->m_a,
				pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
				0, 0, 0);
		} // if结束
		if (pDoc->m_flagShowB)//显示B
		{
			gb_drawPolygonLoop(pDC, pDoc->m_b,
				pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
				255, 0, 255,
				0, 0, 255,
				3);
			gb_drawPolygonPoint(pDC, pDoc->m_b,
				pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
				0, 0, 0,
				8);
			if (pDoc->m_flagShowPointID)
				gb_drawPolygonPointID(pDC, pDoc->m_b,
				pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
				0, 0, 255);
		} // if结束
		if(pDoc->m_a.m_pointArray.size()&&pDoc->m_b.m_pointArray.size()&&pDoc->m_flagShowResult)//显示布尔运算结果
		{
			/*显示构造的多边形*/
			gb_drawPolygonLoop(pDC, AboolB,
				pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
				79, 79, 79,
				255, 165, 0,
				3);
			gb_drawPolygonPoint(pDC, AboolB,
				pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
				0, 0, 0,
				8);
			if (pDoc->m_flagShowPointID)
				gb_drawPolygonPointID(pDC, AboolB,
				pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
				0, 0, 255);
		}
	} // if(!pDoc->m_flagShowSelect)结束: 显示所有的基本内容
	if (pDoc->m_flagSelect) // 显示选择集
	{
		switch(pDoc->m_flagSelectType)
		{
		case 1: // 点。
			if (pDoc->m_flagSelectPolygon==0&&pDoc->m_a.m_pointArray.size()!=0)
				gb_drawPointGlobal(pDC, pDoc->m_a.m_pointArray[pDoc->m_flagSelectID],
				pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
				50, 50, 50,
				6); // A
			else if(pDoc->m_flagSelectPolygon==1&&pDoc->m_b.m_pointArray.size()!=0) 
				gb_drawPointGlobal(pDC, pDoc->m_b.m_pointArray[pDoc->m_flagSelectID],
				pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
				50, 50, 50,
				6); // B
			break;
		case 2: // 环。
			if (pDoc->m_flagSelectPolygon==0&&pDoc->m_a.m_pointArray.size()!=0)
			{
				if (pDoc->m_flagSelectID==0)
					gb_drawLoop(pDC, pDoc->m_a.m_regionArray[pDoc->m_flagSelectRegion].m_loopArray[pDoc->m_flagSelectID],
					pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
					255, 100, 100,
					5); // 外环
				else
					gb_drawLoop(pDC, pDoc->m_a.m_regionArray[pDoc->m_flagSelectRegion].m_loopArray[pDoc->m_flagSelectID],
					pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
					100, 255, 50,
					5); // 内环
			} // A
			else if(pDoc->m_flagSelectPolygon==1&&pDoc->m_b.m_pointArray.size()!=0)
			{
				if (pDoc->m_flagSelectID==0)
					gb_drawLoop(pDC, pDoc->m_b.m_regionArray[pDoc->m_flagSelectRegion].m_loopArray[pDoc->m_flagSelectID],
					pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
					200, 100, 200,
					5); // 外环
				else
					gb_drawLoop(pDC, pDoc->m_b.m_regionArray[pDoc->m_flagSelectRegion].m_loopArray[pDoc->m_flagSelectID],
					pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
					100, 50, 255,
					5); // 内环
			} // B
			break;
		case 3: // 区域。
			int i, n;
			if (pDoc->m_flagSelectPolygon==0&&pDoc->m_a.m_pointArray.size()!=0)
			{
				n = pDoc->m_a.m_regionArray[pDoc->m_flagSelectRegion].m_loopArray.size( );
				for (i=0; i<n; i++)
					if (i==0)
						gb_drawLoop(pDC, pDoc->m_a.m_regionArray[pDoc->m_flagSelectRegion].m_loopArray[i],
						pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
						255, 100, 100,
						5); // 外环
					else
						gb_drawLoop(pDC, pDoc->m_a.m_regionArray[pDoc->m_flagSelectRegion].m_loopArray[i],
						pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
						100, 255, 50,
						5); // 内环
			} // A
			else if(pDoc->m_flagSelectPolygon==1&&pDoc->m_b.m_pointArray.size()!=0)
			{
				n = pDoc->m_b.m_regionArray[pDoc->m_flagSelectRegion].m_loopArray.size( );
				for (i=0; i<n; i++)
					if (i==0)
						gb_drawLoop(pDC, pDoc->m_b.m_regionArray[pDoc->m_flagSelectRegion].m_loopArray[i],
						pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
						200, 100, 200,
						5); // 外环
					else
						gb_drawLoop(pDC, pDoc->m_b.m_regionArray[pDoc->m_flagSelectRegion].m_loopArray[i],
						pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
						100, 50, 255,
						5); // 内环
			} // B
			break;
		case 4: // 多边形。
			if (pDoc->m_flagSelectPolygon==0&&pDoc->m_a.m_pointArray.size()!=0)
			{
				gb_drawPolygonLoop(pDC, pDoc->m_a,
					pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
					255, 100, 100,
					100, 255, 50,
					5);
				gb_drawPolygonPoint(pDC, pDoc->m_a,
					pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
					50, 50, 50,
					10);
			} // A
			else if(pDoc->m_flagSelectPolygon==1&&pDoc->m_b.m_pointArray.size()!=0)
			{
				gb_drawPolygonLoop(pDC, pDoc->m_b,
					pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
					200, 100, 200,
					100, 50, 255,
					5);
				gb_drawPolygonPoint(pDC, pDoc->m_b,
					pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
					50, 50, 50,
					10);
			} // B
			break;
		case 5: // 剖分三角形。
			break;
		} // switch结束
	} // if(pDoc->m_flagSelect)结束
	if (pDoc->m_flagAdd == 1) // 外环
	{
		gb_drawPointArrayLine(pDC, pDoc->m_flagAddPointArray,
			pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
			200, 0, 255,
			5);
		gb_drawPointArrayPoint(pDC, pDoc->m_flagAddPointArray,
			pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
			0, 0, 0,
			16);
	} // if (pDoc->m_flagAdd == 1)结束
	if (pDoc->m_flagAdd == 2) // 内环
	{
		gb_drawPointArrayLine(pDC, pDoc->m_flagAddPointArray,
			pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
			0, 200, 255,
			5);
		gb_drawPointArrayPoint(pDC, pDoc->m_flagAddPointArray,
			pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
			0, 0, 0,
			16);
	} // if (pDoc->m_flagAdd == 2)结束
}

/*绘制AboolB中的边*/
void gb_drawEdge(CDC* pDC, Segment& edge,
				 double scale, CP_Point translation, int screenX, int screenY, 
				 int r, int g, int b, int size)
{
	CPen pen(0, size, RGB(r, g, b));
	CPen * penOld = (CPen *) pDC->SelectObject(&pen);
	CP_Point pg = edge.u1;
	CP_Point ps;
	gb_pointConvertFromGlobalToScreen(ps, pg, scale, translation, screenX, screenY);
	pDC->MoveTo((int)(ps.m_x+0.5), (int)(ps.m_y+0.5));
	pg = edge.u2;
	gb_pointConvertFromGlobalToScreen(ps, pg, scale, translation, screenX, screenY);
	pDC->LineTo((int)(ps.m_x+0.5), (int)(ps.m_y+0.5));
	pDC->SelectObject(penOld);
}

//!!!
void gb_drawLoop(CDC* pDC, CP_Loop& p, 
				 double scale, CP_Point translation, int screenX, int screenY, 
				 int r, int g, int b, int size)
{
	int n = p.m_pointIDArray.size( );
	if (n<=0)
		return;
	CPen pen(0, size, RGB(r, g, b));
	CPen * penOld = (CPen *) pDC->SelectObject(&pen);
	int i, k;
	i = p.m_pointIDArray[n-1];
	CP_Point pg = p.m_polygon->m_pointArray[i];
	CP_Point ps;
	gb_pointConvertFromGlobalToScreen(ps, pg, scale, translation, screenX, screenY);
	pDC->MoveTo((int)(ps.m_x+0.5), (int)(ps.m_y+0.5));
	for (k=0; k<n; k++)
	{
		i = p.m_pointIDArray[k];
		pg = p.m_polygon->m_pointArray[i];
		gb_pointConvertFromGlobalToScreen(ps, pg, scale, translation, screenX, screenY);
		pDC->LineTo((int)(ps.m_x+0.5), (int)(ps.m_y+0.5));
	} //for结束
	pDC->SelectObject(penOld);
} // 函数gb_drawPolygonLoop结束

//!!
void gb_drawPointArrayLine(CDC* pDC, VT_PointArray& pa,
						   double scale, CP_Point translation, int screenX, int screenY,
						   int r, int g, int b, int size)
{
	int n = pa.size( );
	if (n<=0)
		return;
	CPen pen(0, size, RGB(r, g, b));
	CPen * penOld = (CPen *) pDC->SelectObject(&pen);
	int i;
	CP_Point pg = pa[0];
	CP_Point ps;
	gb_pointConvertFromGlobalToScreen(ps, pg, scale, translation, screenX, screenY);
	pDC->MoveTo((int)(ps.m_x+0.5), (int)(ps.m_y+0.5));
	for (i=0; i<n; i++)
	{
		pg = pa[i];
		gb_pointConvertFromGlobalToScreen(ps, pg, scale, translation, screenX, screenY);
		pDC->LineTo((int)(ps.m_x+0.5), (int)(ps.m_y+0.5));
	} //for结束
	pDC->SelectObject(penOld);
} // 函数gb_drawPointArrayLine结束

//!!
void gb_drawPointArrayPoint(CDC* pDC, VT_PointArray& pa,
							double scale, CP_Point translation, int screenX, int screenY,
							int r, int g, int b, int size)
{
	int n = pa.size( );
	int i;
	for (i=0; i<n; i++)
	{
		gb_drawPointGlobal(pDC, pa[i], scale, translation, screenX, screenY, r, g, b, size);
	} // for结束
} // 函数gb_drawPointArrayPoint结束

//!!
void gb_drawPointGlobal(CDC* pDC, CP_Point pointGlobal, double scale, CP_Point translation, int screenX, int screenY, int r, int g, int b, int size)
{
	CP_Point ps;
	gb_pointConvertFromGlobalToScreen(ps, pointGlobal, scale, translation,screenX, screenY);
	gb_drawPointScreen(pDC, (int)(ps.m_x+0.5), (int)(ps.m_y+0.5), r, g, b, size);
} // 函数gb_drawPointScreen结束

//!!
void gb_drawPointScreen(CDC* pDC, int x, int y, int r, int g, int b, int size)
{
	CBrush brush(RGB(r, g, b));
	CBrush* brushOld = (CBrush*)pDC->SelectObject(&brush);;
	// 绘制格子
	CRect rect(x - size, y - size, x + size, y + size);
	pDC->Ellipse(&rect);
	pDC->SelectObject(brushOld);
} // 函数gb_drawPointScreen结束

//!!
void gb_drawPolygonLoop(CDC* pDC, CP_Polygon& p,
						double scale, CP_Point translation, int screenX, int screenY,
						int outR, int outG, int outB, 
						int inR,  int inG,  int inB, 
						int size)
{
	unsigned int i, k;
	for (i=0; i<p.m_regionArray.size( ); i++)
	{
		for (k=0; k<p.m_regionArray[i].m_loopArray.size( ); k++)
		{
			if (k==0)
				gb_drawLoop(pDC, p.m_regionArray[i].m_loopArray[k], 
				scale, translation,screenX, screenY, 
				outR, outG, outB,size);
			else
				gb_drawLoop(pDC, p.m_regionArray[i].m_loopArray[k], 
				scale, translation,screenX, screenY, 
				inR, inG, inB,size);
		} // 内部for结束
	} // 外部for结束
} // 函数gb_drawPolygonLoop结束

//!!
void gb_drawPolygonPoint(CDC* pDC, CP_Polygon& p, double scale, CP_Point translation, int screenX, int screenY, int r, int g, int b, int size)
{
	int n = p.m_pointArray.size( );
	int i;
	for (i=0; i<n; i++)
	{
		gb_drawPointGlobal(pDC, p.m_pointArray[i], scale, translation, screenX, screenY, r, g, b, size);
	} // for结束
} // 函数gb_drawPolygonPoint结束



//!!
void gb_drawPolygonPointID(CDC* pDC, CP_Polygon& p,
						   double scale, CP_Point translation, int screenX, int screenY,
						   int r, int g, int b)
{
	COLORREF cOld = pDC->SetTextColor(RGB(r, g, b));
	int nr = p.m_regionArray.size( );
	int nL, nLv, ir, iL, iLv, v;
	CP_Point ps;
	char buffer[100];
	for (ir=0; ir<nr; ir++)
	{
		nL = p.m_regionArray[ir].m_loopArray.size( );
		for (iL=0; iL<nL; iL++)
		{
			nLv =  p.m_regionArray[ir].m_loopArray[iL].m_pointIDArray.size( );
			for (iLv=0; iLv<nLv; iLv++)
			{
				v = p.m_regionArray[ir].m_loopArray[iL].m_pointIDArray[iLv];
				gb_pointConvertFromGlobalToScreen(ps, p.m_pointArray[v], 
					scale, translation,screenX, screenY);
				sprintf_s(buffer, 100, "[%1d]R%1dL%1dV%1d", v, ir, iL, iLv);
				pDC->TextOutA((int)(ps.m_x+0.5), (int)(ps.m_y+0.5), buffer);
			} // for(iLv)结束
		} // for(iL)结束
	} // for(ir)结束
	pDC->SetTextColor(cOld);
} // 函数gb_drawPolygonPointID结束

/*计算线段长度的平方*/
double seglength(CP_Point p1, CP_Point p2)
{
	return pow((p1.m_x-p2.m_x),2)+pow((p1.m_y-p2.m_y),2);
}

/*计算交叉乘积(P1-P0)x(P2-P0)*/
double xmult(CP_Point p1, CP_Point p2, CP_Point p0)
{
	return (p1.m_x - p0.m_x)*(p2.m_y - p0.m_y) - (p2.m_x - p0.m_x)*(p1.m_y - p0.m_y);
}

/*判点是否在线段上,包括端点*/
int dot_online_in(CP_Point p, CP_Point l1, CP_Point l2)
{
	return zero(xmult(p, l1, l2)) 
		&& (l1.m_x - p.m_x)*(l2.m_x - p.m_x)<eps 
		&& (l1.m_y - p.m_y)*(l2.m_y - p.m_y)<eps;
}

/*判两点在线段同侧,点在线段上返回0*/
int same_side(CP_Point p1, CP_Point p2, CP_Point l1, CP_Point l2)
{
	return xmult(l1, p1, l2)*xmult(l1, p2, l2)>eps;
}

/*判两直线平行*/
int parallel(CP_Point u1, CP_Point u2, CP_Point v1, CP_Point v2)
{
	double temp = (u1.m_x - u2.m_x)*(v1.m_y - v2.m_y) - (v1.m_x - v2.m_x)*(u1.m_y - u2.m_y);
	return zero(temp);
}

/*判三点共线*/
int dots_inline(CP_Point p1, CP_Point p2, CP_Point p3)
{
	return zero(xmult(p1, p2, p3));
}

/*判两线段相交,包括端点和部分重合*/
int intersect_in(CP_Point u1, CP_Point u2, CP_Point v1, CP_Point v2)
{
	if (!dots_inline(u1, u2, v1) || !dots_inline(u1, u2, v2))
	{
		return !same_side(u1, u2, v1, v2) && !same_side(v1, v2, u1, u2);
	}	
	return dot_online_in(u1, v1, v2) || dot_online_in(u2, v1, v2) || dot_online_in(v1, u1, u2) || dot_online_in(v2, u1, u2);
}

/*计算两线段交点,请先判线段是否相交(同时还是要判断是否平行!)*/
CP_Point LineSegInter(CP_Point u1, CP_Point u2, CP_Point v1, CP_Point v2)
{
	CP_Point ret = u1;
	double t = ((u1.m_x - v1.m_x)*(v1.m_y - v2.m_y) - (u1.m_y - v1.m_y)*(v1.m_x - v2.m_x)) 
		/ ((u1.m_x - u2.m_x)*(v1.m_y - v2.m_y) - (u1.m_y - u2.m_y)*(v1.m_x - v2.m_x));
	ret.m_x += (u2.m_x - u1.m_x)*t;
	ret.m_y += (u2.m_y - u1.m_y)*t;
	return ret;
}

bool between(double v, double u1, double u2)
{
	double min = u1<u2 ? u1 : u2;
	double max = u1>=u2 ? u1 : u2;
	if(v>min && v<max)
	{
		return true;
	}
	else
	{
		return false;
	}
}
/*计算、添加交点，得到tempA和tempB*/
void AddNodes(CCP_PolygonPlatformDoc* pDoc)
{
	Segment temp;//临时线段结构变量

	/*将A、B多边形赋给临时变量*/
	tempA = pDoc->m_a;
	tempB = pDoc->m_b;
	SegCoinSame.clear();//清空重合线段数组
	SegCoinReve.clear();

	/*循环A、B的每条边，计算交点*/
	CP_Point u1, u2, v1, v2, node;//两条线段的端点和它们的交点
	int ARs, ALs,APs, BRs, BLs, BPs;//分别表示A和B中区域、环和点数组的大小

	ARs=tempA.m_regionArray.size();
	for(int iA=0; iA<ARs; iA++)//A每个区域
	{
		ALs = tempA.m_regionArray[iA].m_loopArray.size();
		for(int jA=0; jA<ALs; jA++)//A每个环
		{
			APs = tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray.size();
			for(int kA=0; kA<APs; kA++)//A每个点（序号）
			{
				APs = tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray.size();
				/*给线段端点的两个点赋坐标值*/
				u1 = tempA.m_pointArray[tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray[kA]];
				u2 = tempA.m_pointArray[tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray[(kA+1)%APs]];

				/*匹配B中的每条线段*/
				BRs = tempB.m_regionArray.size();
				for(int iB=0; iB<BRs; iB++)//B每个区域
				{
					BLs = tempB.m_regionArray[iB].m_loopArray.size();
					for(int jB=0; jB<BLs; jB++)//B每个环
					{
						BPs = tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray.size();
						for(int kB=0; kB<BPs; kB++)//B每个点
						{
							BPs = tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray.size();
							/*给线段端点的两个点赋坐标值*/
							v1 = tempB.m_pointArray[tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray[kB]];
							v2 = tempB.m_pointArray[tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray[(kB+1)%BPs]];

							/*判断相交/重合，求交点*/
							if(zero(u1.m_x-v1.m_x)&&zero(u1.m_y-v1.m_y) && zero(u2.m_x-v2.m_x)&&zero(u2.m_y-v2.m_y))//两边完全重合且同向
							{
								temp.u1 = u1;
								temp.u2 = u2;
								SegCoinSame.push_back(temp);//插入同向数组中(顺序为线段在A中的顺序)
							}
							else if(zero(u1.m_x-v2.m_x)&&zero(u1.m_y-v2.m_y) && zero(u2.m_x-v1.m_x)&&zero(u2.m_y-v1.m_y))//两边完全重合且反向
							{
								temp.u1 = u1;
								temp.u2 = u2;
								SegCoinReve.push_back(temp);//插入反向数组中(顺序为线段在A中的顺序)
							}
							else if (parallel(u1, u2, v1, v2)&&!dots_inline(u1,u2,v1) || !intersect_in(u1, u2, v1, v2))//不相交
							{//无交点
							}
							else if (parallel(u1, u2, v1, v2) && dots_inline(u1, u2, v1))//重合
							{//分为部分重合和完全重合
								
								//下面是完全重合中的两种情况
								//else if ((pow(v1.m_x - v2.m_x, 2) + pow(v1.m_y - v2.m_y, 2)) - (pow(u1.m_x - u2.m_x, 2) + pow(u1.m_y - u2.m_y, 2))>eps)
								if(between(u1.m_x,v1.m_x,v2.m_x) && between(u2.m_x,v1.m_x,v2.m_x))	
								{//A中线段较短,在B中线段中添加点
									/*在多边形点坐标集合中插入点*/
									if(seglength(v1,u1)-seglength(v1,u2)<-eps)//u1在前，AB同向
									{   /*先插入u1,再插入u2*/
										tempB.m_pointArray.push_back(u1);
										tempB.m_pointArray.push_back(u2);
										temp.u1 = u1;
										temp.u2 = u2;
										SegCoinSame.push_back(temp);//插入同向数组中(顺序为线段在A中的顺序)
									}
									else//u2在前,AB反向
									{   /*先插入u2,再插入u1*/
										tempB.m_pointArray.push_back(u2);
										tempB.m_pointArray.push_back(u1);	
										temp.u1 = u1;
										temp.u2 = u2;
										SegCoinReve.push_back(temp);//插入反向数组中(顺序为线段在A中的顺序)
									}
									/*在多边形的环中的点序列号集合中插入点的序号*/
									tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray.insert(tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray.begin()+kB+1,tempB.m_pointArray.size()-2);
									tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray.insert(tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray.begin()+kB+2,tempB.m_pointArray.size()-1);
									kB += 2;//向后跳两个点，跳过插入的点
								}
								else if(between(v1.m_x,u1.m_x,u2.m_x) && between(v2.m_x,u1.m_x,u2.m_x))
								{//B中线段较短,在A中线段中添加点
									/*在多边形点坐标集合中插入点*/
									if(seglength(u1,v1)-seglength(u1,v2)<-eps)//v1在前，AB同向
									{   /*先插入v1,再插入v2*/
										tempA.m_pointArray.push_back(v1);
										tempA.m_pointArray.push_back(v2);
										temp.u1 = v1;
										temp.u2 = v2;
										SegCoinSame.push_back(temp);//插入同向数组中(顺序为线段在A中的顺序)
									}
									else//v2在前,AB反向
									{   /*先插入v2,再插入v1*/
										tempA.m_pointArray.push_back(v2);
										tempA.m_pointArray.push_back(v1);		
										temp.u1 = v2;
										temp.u2 = v1;
										SegCoinReve.push_back(temp);//插入反向数组中(顺序为线段在A中的顺序)
									}
									/*在多边形的环中的点序列号集合中插入点的序号*/
									tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray.insert(tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray.begin()+kA+1,tempA.m_pointArray.size()-2);
									tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray.insert(tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray.begin()+kA+2,tempA.m_pointArray.size()-1);
									//不向后跳点,而是重新给线段的第二个点赋值
									u2 = tempA.m_pointArray[tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray[(kA+1)%APs]];
								}
								else if(!between(v1.m_x,u1.m_x,u2.m_x) && !between(v2.m_x,u1.m_x,u2.m_x)&& !between(v1.m_y,u1.m_y,u2.m_y) && !between(v2.m_y,u1.m_y,u2.m_y)
									&& !between(u1.m_x,v1.m_x,v2.m_x) && !between(u2.m_x,v1.m_x,v2.m_x)&& !between(u1.m_y,v1.m_y,v2.m_y) && !between(u2.m_y,v1.m_y,v2.m_y))
								{
									continue;
								}
								else//部分重合
								{
									if((u1.m_x-v1.m_x)*(u2.m_x-v2.m_x)+(u1.m_y-v1.m_y)*(u2.m_y-v2.m_y) >= eps)//两边同向
									{
;										if((v1.m_x-u1.m_x)*(v1.m_x-u2.m_x)+(v1.m_y-u1.m_y)*(v1.m_y-u2.m_y) < -eps)//排布是u1->v1->u2->v2
										{//在A中插入v1，在B中插入u2。将v1->u2加入同向重合边集合
											tempA.m_pointArray.push_back(v1);
											tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray.insert(tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray.begin()+kA+1,tempA.m_pointArray.size()-1);
											tempB.m_pointArray.push_back(u2);
											tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray.insert(tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray.begin()+kB+1,tempB.m_pointArray.size()-1);
											temp.u1 = v1;
											temp.u2 = u2;
											//SegCoinSame.push_back(temp);
											kB += 1;//向后跳一个点，跳过插入的点
											//MessageBox( NULL , TEXT("这是对话框1") , TEXT("你好") , MB_ICONINFORMATION|MB_YESNO);
										}
										else//排布是v1->u1->v2->u2
										{//在A中插入v2，在B中插入u1。将u1->v2加入同向重s合边集合
											tempA.m_pointArray.push_back(v2);
											tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray.insert(tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray.begin()+kA+1,tempA.m_pointArray.size()-1);
											tempB.m_pointArray.push_back(u1);
											tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray.insert(tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray.begin()+kB+1,tempB.m_pointArray.size()-1);
											temp.u1 = u1;
											temp.u2 = v2;
											//SegCoinSame.push_back(temp);
											kB += 1;//向后跳一个点，跳过插入的点
											//MessageBox( NULL , TEXT("这是对话框2") , TEXT("你好") , MB_ICONINFORMATION|MB_YESNO);
										}
									}
									else//两边反向
									{
										if((v2.m_x-u1.m_x)*(v2.m_x-u2.m_x)+(v2.m_y-u1.m_y)*(v2.m_y-u2.m_y) < -eps)//排布是u1->v2->u2->v1
										{//在A中插入v2，在B中插入u2。将v2->u2加入反向重合边集合
											tempA.m_pointArray.push_back(v2);
											tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray.insert(tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray.begin()+kA+1,tempA.m_pointArray.size()-1);
											tempB.m_pointArray.push_back(u2);
											tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray.insert(tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray.begin()+kB+1,tempB.m_pointArray.size()-1);
											temp.u1 = v2;
											temp.u2 = u2;
											//SegCoinReve.push_back(temp);
											kB += 1;//向后跳一个点，跳过插入的点
											//MessageBox( NULL , TEXT("这是对话框3") , TEXT("你好") , MB_ICONINFORMATION|MB_YESNO);
										}
										else//排布是v2->u1->v1->u2
										{//在A中插入v1，在B中插入u1。将u1->v1加入反向重合边集合
											tempA.m_pointArray.push_back(v1);
											tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray.insert(tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray.begin()+kA+1,tempA.m_pointArray.size()-1);
											tempB.m_pointArray.push_back(u1);
											tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray.insert(tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray.begin()+kB+1,tempB.m_pointArray.size()-1);
											temp.u1 = u1;
											temp.u2 = v1;
											//SegCoinReve.push_back(temp);
											kB += 1;//向后跳一个点，跳过插入的点
											//MessageBox( NULL , TEXT("这是对话框4") , TEXT("你好") , MB_ICONINFORMATION|MB_YESNO);
										}
									}
									//不向后跳点,而是重新给线段的第二个点赋值
									u2 = tempA.m_pointArray[tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray[(kA+1)%APs]];
								}
							}
							else//普通交点
							{
								node= LineSegInter(u1, u2, v1, v2);//得到交点
								if(zero(node.m_x-u1.m_x)&&zero(node.m_y-u1.m_y) || zero(node.m_x-u2.m_x)&&zero(node.m_y-u2.m_y) 
									|| zero(node.m_x-v1.m_x)&&zero(node.m_y-v1.m_y) || zero(node.m_x-v2.m_x)&&zero(node.m_y-v2.m_y))//交点是某条线段的端点
								{//直接跳过此交点
									continue;
								}
								/*将交点添加到A中*/
								tempA.m_pointArray.push_back(node);
								tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray.insert(tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray.begin()+kA+1,tempA.m_pointArray.size()-1);
								/*将交点添加到B中*/
								tempB.m_pointArray.push_back(node);
								tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray.insert(tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray.begin()+kB+1,tempB.m_pointArray.size()-1);
								kB += 1;//向后跳一个点，跳过插入的点
							}
							APs = tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray.size();
							BPs = tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray.size();
							u1 = tempA.m_pointArray[tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray[kA]];
							u2 = tempA.m_pointArray[tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray[(kA+1)%APs]];
						}//for结束
					}//for结束
				}//for结束
				APs = tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray.size();
			}//for结束
		}//for结束
	}//for结束，计算所有交点结束
}

/*判断点p是否在多边形poly内*/
bool PointInPoligon(CP_Point p, CP_Polygon poly)
{
	int count = 0;
	CP_Point infPoint;//无穷远点
	infPoint.m_x = INFINITY;
	infPoint.m_y = p.m_y;//产生“伪射线L”的另一个端点
	
	/*遍历多边形的每一条边*/
	int polyR, polyL, polyP;//分别表示多边形中区域、环、点数组的大小
	polyR = poly.m_regionArray.size();
	for(int i=0; i<polyR; i++)//每一个区域
	{
		polyL = poly.m_regionArray[i].m_loopArray.size();
		for(int j=0; j<polyL; j++)//每一个环
		{
			polyP = poly.m_regionArray[i].m_loopArray[j].m_pointIDArray.size();
			for(int k=0; k<polyP; k++)//每一个顶点为起点的线段
			{
				int u1, u2;//两个端点在多边形点坐标集合中的序号
				u1 = poly.m_regionArray[i].m_loopArray[j].m_pointIDArray[k];
				u2 = poly.m_regionArray[i].m_loopArray[j].m_pointIDArray[(k+1)%polyP];
				if(dot_online_in(p,poly.m_pointArray[u1],poly.m_pointArray[u2]))//点P在边上
				{
					return true;
				}
				if(!zero(poly.m_pointArray[u1].m_y-poly.m_pointArray[u2].m_y))//边不是水平的
				{
					if(dot_online_in(poly.m_pointArray[u1].m_y>poly.m_pointArray[u2].m_y?poly.m_pointArray[u1]:poly.m_pointArray[u2],infPoint,p))//边上纵坐标较大的端点在L上
					{
						count++;
					}
					else if(dot_online_in(poly.m_pointArray[u1].m_y>poly.m_pointArray[u2].m_y?poly.m_pointArray[u2]:poly.m_pointArray[u1],infPoint,p))//边上纵坐标较小的端点在L上
					{
						//忽略
					}
					else if(intersect_in(poly.m_pointArray[u1],poly.m_pointArray[u2],infPoint,p))//边与L相交
					{
						count++;
					}
	
				}
			}
		}
	}
	/*获取最终结果*/
	if(count%2==0)//偶数：不在多边形内
	{
		return false;
	}
	else//奇数：在多边形内
	{
		return true;
	}
}

/*判断线段u1u2是否在多边形内*/
bool SegInPolygon(CP_Point u1, CP_Point u2, CP_Polygon poly)
{
	CP_Point midpoint;//线段中点

	midpoint.m_x = (u1.m_x+u2.m_x)/2.0;
	midpoint.m_y = (u1.m_y+u2.m_y)/2.0;
	/*如果中点在多边形内，则线段在多边形内*/
	if(PointInPoligon(midpoint,poly))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*升序排序函数*/
bool ascending(const Segment& e1, const Segment& e2)
{
	return e1.u1.m_x<e2.u1.m_x;
	//return e1.u1.m_x<e2.u1.m_x || (zero(e1.u1.m_x-e2.u1.m_x)&&(e1.u1.m_y<e2.u1.m_y));
}

/*检查多边形中有没有面积过小的环，若有，为了避免其看起来像是一条边而不是环，将其删除*/
void PolygonExam()
{
	int polyR, polyL, polyP;
	double xmin=99999, xmax=-99999, ymin=99999, ymax=-99999;

	polyR = AboolB.m_regionArray.size();
	for(int i=0; i<polyR; i++)//循环遍历每个区域
	{
		polyL = AboolB.m_regionArray[i].m_loopArray.size();
		for(int j=0; j<polyL; j++)//循环遍历每个环
		{
			polyP = AboolB.m_regionArray[i].m_loopArray[j].m_pointIDArray.size();
			xmin = ymin = 99999;
			xmax = ymax = -99999;
			for(int k=0; k<polyP; k++)//循环遍历每条边
			{
				int u;//边端点的序号
				u = AboolB.m_regionArray[i].m_loopArray[j].m_pointIDArray[k];
				xmin = AboolB.m_pointArray[u].m_x<xmin?AboolB.m_pointArray[u].m_x:xmin;
				xmax = AboolB.m_pointArray[u].m_x>xmax?AboolB.m_pointArray[u].m_x:xmax;
				ymin = AboolB.m_pointArray[u].m_y<ymin?AboolB.m_pointArray[u].m_y:ymin;
				ymax = AboolB.m_pointArray[u].m_y>ymax?AboolB.m_pointArray[u].m_y:ymax;
			}
			/*环面积过小，删除此环*/
			if(xmax-xmin<2 || ymax-ymin<2)
			{
				for(int k=0; k<polyP; k++)//“删除”环上顶点（将它们转化成第一个顶点的坐标）
				{
					int u;//边端点序号
					u = AboolB.m_regionArray[i].m_loopArray[j].m_pointIDArray[k];
					AboolB.m_pointArray[u] = AboolB.m_pointArray[0];
				}
				vector<CP_Loop>::iterator iter = AboolB.m_regionArray[i].m_loopArray.begin()+j;
				AboolB.m_regionArray[i].m_loopArray.erase(iter);
				j--;
				polyL--;
				break;
			}
		}
	}
}

/*将边集合构建成多边形*/
void ConstructPolygon(CCP_PolygonPlatformDoc* pDoc)
{
	CP_Point Pmin;//当前环中最左侧端点
	Pmin.m_x = 1.79e-308;
	double Xmax=-1000000,Ymax=-1000000,Ymin=1000000;//当前环中x,y的最大值最小值，初始为double型的最小值/最大值
	CP_Point PNow;//当前的顶点（寻找下一条相接的线段的时候用到）

	AboolB.mb_clear();//清空并集
	int RID=-1, LID=-1;//多边形中区域的个数，当前区域中环的个数
	CP_Region tempR;
	CP_Loop tempL;

	while(pDoc->ABEdge.size()!=0)
	{
		/*判当前线段是内环还是新的区域的外环*/
		//if(pDoc->ABEdge[0].u1.m_x-Xmax>eps || pDoc->ABEdge[0].u1.m_y-Ymax>eps || Ymin-pDoc->ABEdge[0].u1.m_y>eps)//当前第一个元素的的坐标大于Xmax或Ymax，说明是一个【新的区域】中的【外环】中的【第一条线段】
		if(RID==-1 || !PointInPoligon(pDoc->ABEdge[1].u1,AboolB))
		{
			RID++;
			LID = 0;
			/*添加区域*/
			AboolB.m_regionArray.push_back(tempR);//向布尔运算结果中添加区域
			AboolB.m_regionArray[RID].m_regionIDinPolygon = RID;//区域的编号
			AboolB.m_regionArray[RID].m_polygon = &AboolB;//区域属于AboolB这个多边形
			/*添加环*/
			AboolB.m_regionArray[RID].m_loopArray.push_back(tempL);//向布尔运算结果中添加环
			AboolB.m_regionArray[RID].m_loopArray[LID].m_loopIDinRegion = LID;//环的编号
			AboolB.m_regionArray[RID].m_loopArray[LID].m_polygon = &AboolB;//环属于A∪B这个多边形
			AboolB.m_regionArray[RID].m_loopArray[LID].m_regionIDinPolygon = RID;//环属于第几个区域
			/*在多边形中添加点*/
			AboolB.m_pointArray.push_back(pDoc->ABEdge[0].u1);
			AboolB.m_pointArray.push_back(pDoc->ABEdge[0].u2);
			AboolB.m_regionArray[RID].m_loopArray[LID].m_pointIDArray.push_back(AboolB.m_pointArray.size()-2);
			AboolB.m_regionArray[RID].m_loopArray[LID].m_pointIDArray.push_back(AboolB.m_pointArray.size()-1);
			/*更新Xmin和Xmax*/
			Pmin.m_x = pDoc->ABEdge[0].u1.m_x;//环的起点，判断环闭合时要用到
			Pmin.m_y = pDoc->ABEdge[0].u1.m_y;
			Xmax = pDoc->ABEdge[0].u2.m_x;
			Ymax = pDoc->ABEdge[0].u1.m_y>pDoc->ABEdge[0].u2.m_y?pDoc->ABEdge[0].u1.m_y:pDoc->ABEdge[0].u2.m_y;
			Ymin = pDoc->ABEdge[0].u1.m_y>pDoc->ABEdge[0].u2.m_y?pDoc->ABEdge[0].u2.m_y:pDoc->ABEdge[0].u1.m_y;//环的最外面，判断内环/外环的时候要用到（起初是第一条边的尾端点）
			PNow = pDoc->ABEdge[0].u2;//当前的顶点是这条线段的尾端点，利用此顶点寻找下一条边
			/*将该边从ABEdge中删除*/
			vector<Segment>::iterator it = pDoc->ABEdge.begin();
			pDoc->ABEdge.erase(it);
		}
		else//第一个元素的坐标小于Xmax，说明是【当前区域】的一个【内环】
		{
			LID++;
			/*添加环*/
			AboolB.m_regionArray[RID].m_loopArray.push_back(tempL);//向布尔运算结果中添加环
			AboolB.m_regionArray[RID].m_loopArray[LID].m_loopIDinRegion = LID;//环的编号
			AboolB.m_regionArray[RID].m_loopArray[LID].m_polygon = &AboolB;//环属于A∪B这个多边形
			AboolB.m_regionArray[RID].m_loopArray[LID].m_regionIDinPolygon = RID;//环属于第几个区域
			/*在多边形中添加点*/
			AboolB.m_pointArray.push_back(pDoc->ABEdge[0].u1);
			AboolB.m_pointArray.push_back(pDoc->ABEdge[0].u2);
			AboolB.m_regionArray[RID].m_loopArray[LID].m_pointIDArray.push_back(AboolB.m_pointArray.size()-2);
			AboolB.m_regionArray[RID].m_loopArray[LID].m_pointIDArray.push_back(AboolB.m_pointArray.size()-1);
			/*只需要更新Xmin*/
			Pmin.m_x = pDoc->ABEdge[0].u1.m_x;//环的起点，判断环闭合时要用到
			Pmin.m_y = pDoc->ABEdge[0].u1.m_y;
			PNow = pDoc->ABEdge[0].u2;//当前的顶点是这条线段的尾端点，利用此顶点寻找下一条边
			/*将该边从ABEdge中删除*/
			vector<Segment>::iterator it = pDoc->ABEdge.begin();
			pDoc->ABEdge.erase(it);
		}

		/*寻找当前环中的所有线段*/
		while(1)
		{
			/*遍历查找下一条线段（线段的起点和上一条线段的尾点重合）*/
			vector<Segment>::iterator it;
			for(it=pDoc->ABEdge.begin(); it!=pDoc->ABEdge.end(); it++)
			{
				if(zero(it->u1.m_x-PNow.m_x)&&zero(it->u1.m_y-PNow.m_y))
				{
					break;//找到便退出
				}
			}

			/*如果这条线段的终点是该环的起点，说明这个环已经完成，退出while循环*/
			if(zero(it->u2.m_x-Pmin.m_x)&&zero(it->u2.m_y-Pmin.m_y))
			{
				/*将该边从ABEdge中删除*/
				pDoc->ABEdge.erase(it);
				break;
			}
			/*如果这条线段的终点不是该环的起点，说明这个环还没有完成在，继续循环*/
			/*多边形中添加点*/
			AboolB.m_pointArray.push_back(it->u2);//只需要添加线段尾端点，因为头端点也就是上一条线段中的尾端点，已经添加
			AboolB.m_regionArray[RID].m_loopArray[LID].m_pointIDArray.push_back(AboolB.m_pointArray.size()-1);
			PNow = it->u2;
			Xmax = Xmax>it->u2.m_x?Xmax:it->u2.m_x;//更新外环最外侧坐标
			Ymax = Ymax>it->u2.m_y?Ymax:it->u2.m_y;
			Ymin = Ymin<it->u2.m_y?Ymin:it->u2.m_y;
			/*将该边从ABEdge中删除*/
			pDoc->ABEdge.erase(it);
		}
	}
	/*检查多边形中有没有面积较小的环，如果有，为了避免其看起来像一条线，删去此环*/
	PolygonExam();
}

// CCP_PolygonPlatformView 打印
void CCP_PolygonPlatformView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CCP_PolygonPlatformView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CCP_PolygonPlatformView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CCP_PolygonPlatformView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

//~
void CCP_PolygonPlatformView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{

	ClientToScreen(&point);
//	OnContextMenu(this, point);
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    if (pDoc->m_flagAdd == 3)
    {
        pDoc->m_flagAdd = 0;
        mb_statusSetText("点添加操作结束。", "请继续其他操作");
        return;
    } // if结束
	    if ((pDoc->m_flagAdd == 1) || (pDoc->m_flagAdd == 2)) // 环
    {
        int n = pDoc->m_flagAddPointArray.size( );
        if (n<=0)
        {
            pDoc->m_flagAdd = 0;
            mb_statusSetText("环添加操作结束。", "请继续其他操作");
            return;
        } // if结束
		       if (n<3)
        {
            if (MessageBox("构成环的点不能少于3个，是否继续输入? 选是则继续，否则放弃前面输入的点。"
                      , "操作确认"
                      , MB_ICONQUESTION|MB_YESNO)==IDYES)
                return;
            if (pDoc->m_flagAdd == 1)
                 mb_statusSetText("外环点添加操作结束。", "请继续其他操作");
            else mb_statusSetText("内环点添加操作结束。", "请继续其他操作");
            pDoc->m_flagAdd = 0;
            pDoc->m_flagAddPointArray.clear( );
            return;
        } // if结束
       int i, k;
        CP_Polygon* pn;
        if (pDoc->m_flagAddIDPolygon==0)
             pn= &(pDoc->m_a);
        else pn= &(pDoc->m_b);
        int nv = pn->m_pointArray.size( );
        pn->m_pointArray.resize(nv+n);
        for (i=0, k=nv; i<n; i++, k++)
        {
            pn->m_pointArray[k].m_x = pDoc->m_flagAddPointArray[i].m_x;
            pn->m_pointArray[k].m_y = pDoc->m_flagAddPointArray[i].m_y;
        } // for结束
		       if (pDoc->m_flagAdd == 1)
        {
            pDoc->m_flagAddIDLoop=0;
            pDoc->m_flagAddIDRegion=pn->m_regionArray.size( );
            pn->m_regionArray.resize(pDoc->m_flagAddIDRegion+1);
            pn->m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray.resize(1);
            pn->m_regionArray[pDoc->m_flagAddIDRegion].m_polygon=pn;
            pn->m_regionArray[pDoc->m_flagAddIDRegion].m_regionIDinPolygon = pDoc->m_flagAddIDRegion;
        }
        else
        {
            pDoc->m_flagAddIDLoop= pn->m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray.size( );
            pn->m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray.resize(pDoc->m_flagAddIDLoop+1);
        } // if/else结束
			           pn->m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray[pDoc->m_flagAddIDLoop].m_polygon = pn;

        pn->m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray[pDoc->m_flagAddIDLoop].m_regionIDinPolygon = pDoc->m_flagAddIDRegion;

        pn->m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray[pDoc->m_flagAddIDLoop].m_loopIDinRegion = pDoc->m_flagAddIDLoop;

        pn->m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray[pDoc->m_flagAddIDLoop].m_pointIDArray.resize(n);

        for (i=0, k= nv; i<n; i++, k++)
        {
            pn->m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray[pDoc->m_flagAddIDLoop].m_pointIDArray[i] = k;
        } // for结束

		       if (pDoc->m_flagAdd == 1)
             mb_statusSetText("外环点添加操作结束。", "请继续其他操作");
        else mb_statusSetText("内环点添加操作结束。", "请继续其他操作");
        pDoc->m_flagAdd = 0;
        pDoc->m_flagAddPointArray.clear( );
        Invalidate( );
        return;
    } // 外if结束
}

void CCP_PolygonPlatformView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CCP_PolygonPlatformView 诊断

#ifdef _DEBUG
void CCP_PolygonPlatformView::AssertValid() const
{
	CView::AssertValid();
}

void CCP_PolygonPlatformView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCP_PolygonPlatformDoc* CCP_PolygonPlatformView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCP_PolygonPlatformDoc)));
	return (CCP_PolygonPlatformDoc*)m_pDocument;
}
#endif //_DEBUG


// CCP_PolygonPlatformView 消息处理程序


void CCP_PolygonPlatformView::OnUpdateComboAorb(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
    CMFCRibbonBar* robbon_bar = ((CFrameWndEx*)AfxGetMainWnd())->GetRibbonBar();
    if (robbon_bar==NULL)
        return;
    CMFCRibbonComboBox* pbox = (CMFCRibbonComboBox*)robbon_bar->FindByID(ID_COMBO_AorB); // 获取编辑框句柄
    if (pbox==NULL)
        return;
    pbox->AddItem("多边形A");
    pbox->AddItem("多边形B");

}


void CCP_PolygonPlatformView::OnComboAorb()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    CMFCRibbonBar* robbon_bar = ((CFrameWndEx*)AfxGetMainWnd())->GetRibbonBar();
    if (robbon_bar==NULL)
        return;
    CMFCRibbonComboBox* pbox = (CMFCRibbonComboBox*)robbon_bar->FindByID(ID_COMBO_AorB); // 获取编辑框句柄
	    if (pbox==NULL)
        return;
    pbox->AddItem("多边形A");
    pbox->AddItem("多边形B");
    int i = pbox->GetCurSel( );
    pbox->SelectItem(i);
    if (i==0)
        pDoc->m_flagBuildA = true;
    else pDoc->m_flagBuildA = false;
    Invalidate( );

}


void CCP_PolygonPlatformView::OnEdgeNumber()
{
	// TODO: 在此添加命令处理程序代码
	
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    CString string;
    CMFCRibbonBar* robbon_bar = ((CFrameWndEx*)AfxGetMainWnd())->GetRibbonBar(); //获取Ribbon bar 句柄
    if (robbon_bar==NULL)
        return;
	    CMFCRibbonEdit* slider = (CMFCRibbonEdit*)robbon_bar->FindByID(ID_EDGE_NUMBER); // 获取编辑框句柄
    if (slider==NULL)
        return;
    string= slider->GetEditText(); // 获取数字
    int i = atoi(string);
    if (i<3)
        i = 3;
    if (i>10000)
        i=10000;
    pDoc->m_edgeNumber = i;
    string.Format("%d", i);
    slider->SetEditText(string);
    Invalidate( );
}


void CCP_PolygonPlatformView::OnTolerance()
{
	// TODO: 在此添加命令处理程序代码
	 CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    CString string;
    CMFCRibbonBar* robbon_bar = ((CFrameWndEx*)AfxGetMainWnd())->GetRibbonBar(); //获取Ribbon bar 句柄
    if (robbon_bar==NULL)
        return;

     CMFCRibbonEdit* slider = (CMFCRibbonEdit*)robbon_bar->FindByID(ID_TOLERANCE); // 获取编辑框句柄
    if (slider==NULL)
        return;
    string= slider->GetEditText(); // 获取数字
    double d = atof(string);
    if (d<=0.0)
        d = 1e-6;
    pDoc->m_tolerance = d;
    string.Format("%g", pDoc->m_tolerance);
    slider->SetEditText(string);
    Invalidate( );

}


void CCP_PolygonPlatformView::OnNewRightOutloop()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    CRect r;
    GetClientRect(& r);
    double dr = (r.right<r.bottom ? r.right : r.bottom);
    dr /=3;
    if (pDoc->m_flagBuildA)
         gb_polygonNewOutLoopRegular(pDoc->m_a, pDoc->m_edgeNumber, dr, 0.0, 0.0);
    else gb_polygonNewOutLoopRegular(pDoc->m_b, pDoc->m_edgeNumber, dr, 0.0, 0.0);
    Invalidate( );
    char s[100];
    sprintf_s(s, 100, "新外环是正%1d边形", pDoc->m_edgeNumber);
    if (pDoc->m_flagBuildA)
         mb_statusSetText("在多边形A中创建了新外环。", s);
    else mb_statusSetText("在多边形B中创建了新外环。", s);

}


void CCP_PolygonPlatformView::OnViewStandard()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    pDoc->m_scale = 1.0; // 缩放比例
    pDoc->m_translation.m_x = 0.0; // 坐标平移量
    pDoc->m_translation.m_y = 0.0; // 坐标平移量
    Invalidate( );
    mb_statusSetText("标准化坐标系。", "不平移，也不缩放。");
}


void CCP_PolygonPlatformView::OnViewFit()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    double dxMin, dyMin, dxMax, dyMax, ra, s1, s2;
    int na = pDoc->m_a.m_pointArray.size( );
    int nb = pDoc->m_b.m_pointArray.size( );
    if ((na==0) && (nb==0))
        return;
	    if (na!=0)
    {
        dxMin = pDoc->m_a.m_pointArray[0].m_x;
        dxMax = dxMin;
        dyMin = pDoc->m_a.m_pointArray[0].m_y;
        dyMax = dyMin;
    }
    else if (nb!=0)
    {
        dxMin = pDoc->m_b.m_pointArray[0].m_x;
        dxMax = dxMin;
        dyMin = pDoc->m_b.m_pointArray[0].m_y;
        dyMax = dyMin;
    }
    int i;
    for (i=0; i<na; i++)
    {
        if (dxMin>pDoc->m_a.m_pointArray[i].m_x)
            dxMin=pDoc->m_a.m_pointArray[i].m_x;
        if (dxMax<pDoc->m_a.m_pointArray[i].m_x)
            dxMax=pDoc->m_a.m_pointArray[i].m_x;
        if (dyMin>pDoc->m_a.m_pointArray[i].m_y)
            dyMin=pDoc->m_a.m_pointArray[i].m_y;
        if (dyMax<pDoc->m_a.m_pointArray[i].m_y)
            dyMax=pDoc->m_a.m_pointArray[i].m_y;
    } // for结束
        for (i=0; i<nb; i++)
    {
        if (dxMin>pDoc->m_b.m_pointArray[i].m_x)
            dxMin=pDoc->m_b.m_pointArray[i].m_x;
        if (dxMax<pDoc->m_b.m_pointArray[i].m_x)
            dxMax=pDoc->m_b.m_pointArray[i].m_x;
        if (dyMin>pDoc->m_b.m_pointArray[i].m_y)
            dyMin=pDoc->m_b.m_pointArray[i].m_y;
        if (dyMax<pDoc->m_b.m_pointArray[i].m_y)
            dyMax=pDoc->m_b.m_pointArray[i].m_y;
    } // for结束
    CRect r;
    GetClientRect(& r);
    r.bottom -=40;
    r.right -=40;
    pDoc->m_translation.m_x=(dxMin+dxMax)/2;
    pDoc->m_translation.m_y=(dyMin+dyMax)/2;
    ra=(double)(dxMax-dxMin);
    if (ra<10e-8)
        ra=1;
    s1=(double)(r.right-r.left)/ra;
    ra=(double)(dyMax-dyMin);
    if (ra<10e-8)
        ra=1;
    s2=(double)(r.bottom-r.top)/ra;
    pDoc->m_scale=(s1<s2 ? s1 : s2);
    Invalidate( );
    mb_statusSetText("自适应显示!", "尽量充满屏幕!");
}


void CCP_PolygonPlatformView::OnSelectPoint()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    if (pDoc->m_flagSelectType==1)
        pDoc->m_flagSelectType = 0;
    else pDoc->m_flagSelectType = 1;
    pDoc->m_flagSelect = false;
    Invalidate(); // 刷新
}



void CCP_PolygonPlatformView::OnUpdateSelectPoint(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    if (pDoc->m_flagSelectType==1)
        pCmdUI->SetCheck(true);
    else pCmdUI->SetCheck(false);
}


void CCP_PolygonPlatformView::OnUpdateSelectLoop(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    if (pDoc->m_flagSelectType==2)
        pCmdUI->SetCheck(true);
    else pCmdUI->SetCheck(false);
}



void CCP_PolygonPlatformView::OnSelectLoop()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    if (pDoc->m_flagSelectType==2)
        pDoc->m_flagSelectType = 0;
    else pDoc->m_flagSelectType = 2;
    pDoc->m_flagSelect = false;
    Invalidate(); // 刷新
}




void CCP_PolygonPlatformView::OnUpdateSelectRegion(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    if (pDoc->m_flagSelectType==3)
        pCmdUI->SetCheck(true);
    else pCmdUI->SetCheck(false);
}


void CCP_PolygonPlatformView::OnSelectRegion()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    if (pDoc->m_flagSelectType==3)
        pDoc->m_flagSelectType = 0;
    else pDoc->m_flagSelectType = 3;
    pDoc->m_flagSelect = false;
    Invalidate(); // 刷新
}



void CCP_PolygonPlatformView::OnUpdateSelectPolygon(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    if (pDoc->m_flagSelectType==4)
        pCmdUI->SetCheck(true);
    else pCmdUI->SetCheck(false);
}



void CCP_PolygonPlatformView::OnSelectPolygon()
{
	// TODO: 在此添加命令处理程序代码
    CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    if (pDoc->m_flagSelectType==4)
        pDoc->m_flagSelectType = 0;
    else pDoc->m_flagSelectType = 4;
    pDoc->m_flagSelect = false;
    Invalidate(); // 刷新
}



void CCP_PolygonPlatformView::OnUpdateSelectTriangle(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    if (pDoc->m_flagSelectType==5)
        pCmdUI->SetCheck(true);
    else pCmdUI->SetCheck(false);
}



void CCP_PolygonPlatformView::OnSelectTriangle()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    if (pDoc->m_flagSelectType==5)
        pDoc->m_flagSelectType = 0;
    else pDoc->m_flagSelectType = 5;
    pDoc->m_flagSelect = false;
    Invalidate(); // 刷新
}



void CCP_PolygonPlatformView::OnSelectOnly()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    pDoc->m_flagShowSelect ^= true;//异或 -----取反器
    Invalidate(); // 刷新
}



void CCP_PolygonPlatformView::OnUpdateSelectOnly(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    pCmdUI->SetCheck(pDoc->m_flagShowSelect);
}



void CCP_PolygonPlatformView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CView::OnLButtonDown(nFlags, point);
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    CP_Point ps, pg;
    ps.m_x = point.x;
    ps.m_y = point.y;
    CRect r;
    GetClientRect(& r);
    gb_pointConvertFromScreenToGlobal(pg, ps, 
        pDoc->m_scale, pDoc->m_translation, r.right, r.bottom);
    pDoc->m_basePoint = pg;
    bool flagSuceess = false;
    CP_Polygon* pn0;
	if (pDoc->m_flagAddIDPolygon==0)
         pn0 = &(pDoc->m_a);
    else pn0 = &(pDoc->m_b);
    if (pDoc->m_flagAdd == 3)
    {
        gb_insertPointInPolygon(*pn0, pDoc->m_flagAddIDRegion, pDoc->m_flagAddIDLoop, pDoc->m_flagAddIDPointInLoop, pg);
        mb_statusSetText("点添加操作成功。", "用鼠标右键结束点添加操作");
        Invalidate( );
        CView::OnLButtonDown(nFlags, point);
        return;
    } // 外if结束
    if (pDoc->m_flagAddIDPolygon==0)
         pn0 = &(pDoc->m_a);
    else pn0 = &(pDoc->m_b);
    if (pDoc->m_flagAdd == 3)
    {
        gb_insertPointInPolygon(*pn0, pDoc->m_flagAddIDRegion, pDoc->m_flagAddIDLoop, pDoc->m_flagAddIDPointInLoop, pg);
        mb_statusSetText("点添加操作成功。", "用鼠标右键结束点添加操作");
        Invalidate( );
        CView::OnLButtonDown(nFlags, point);
        return;
    } // 外if结束
    if ((pDoc->m_flagAdd == 1) || (pDoc->m_flagAdd == 2)) // 环
    {
        pDoc->m_flagAddPointArray.push_back(pg);
        if (pDoc->m_flagAdd == 1)
             mb_statusSetText("外环点添加操作成功。", "用鼠标右键结束外环添加操作");
        else mb_statusSetText("内环点添加操作成功。", "用鼠标右键结束内环添加操作");
        Invalidate( );
        CView::OnLButtonDown(nFlags, point);
        return;
    } // 外if结束

    if ((!(pDoc->m_flagShowA))&&(!(pDoc->m_flagShowB)))
    {
        CView::OnLButtonDown(nFlags, point);
        return;
    } // if结束
	    double da, db;
    int ida, idb, ira, irb;
    CP_Polygon* pn1;
    VT_IntArray* pSet0;
    VT_IntArray* pSet1;
    CP_Point p0, p1;
    switch(pDoc->m_flagSelectType)
    {
    case 1: // 点。
        if (pDoc->m_flagShowA)
            gb_distanceMinPointPolygon(da, ida, pg, pDoc->m_a);
        else ida = -1;
        if (pDoc->m_flagShowB)
            gb_distanceMinPointPolygon(db, idb, pg, pDoc->m_b);
        else idb = -1;
        if (ida>=0)
        {
            if (idb>=0)
            {
                if (da<=db)
                {
                    pDoc->m_flagSelect = true;
                    pDoc->m_flagSelectPolygon = 0;
                    pDoc->m_flagSelectID = ida;
                }
                else
                {
                    pDoc->m_flagSelect = true;
                    pDoc->m_flagSelectPolygon = 1;
                    pDoc->m_flagSelectID = idb;
                }
            }
            else
            {
                pDoc->m_flagSelect = true;
                pDoc->m_flagSelectPolygon = 0;
                pDoc->m_flagSelectID = ida;
            }
        }
	    else
        {
            if (idb>=0)
            {
                pDoc->m_flagSelect = true;
                pDoc->m_flagSelectPolygon = 1;
                pDoc->m_flagSelectID = idb;
            }
            else pDoc->m_flagSelect = false;
        }
	         if (pDoc->m_flagSelect)
        {
            if (pDoc->m_flagMoveSame)
            {
                if (pDoc->m_flagSelectPolygon==0)
                {
                    pn0 = &(pDoc->m_a);
                    pn1 = &(pDoc->m_b);
                    pSet0 = &(pDoc->m_flagSelectIDSetInA);
                    pSet1 = &(pDoc->m_flagSelectIDSetInB);
                }
                else
                {
                    pn0 = &(pDoc->m_b);
                    pn1 = &(pDoc->m_a);
                    pSet0 = &(pDoc->m_flagSelectIDSetInB);
                    pSet1 = &(pDoc->m_flagSelectIDSetInA);
                } // if/else结束
                gb_intArrayInitPoint(*pSet0, *pn0, pDoc->m_flagSelectID, pDoc->m_tolerance);
                gb_intArrayInitPolygonSamePoint(*pSet1, *pn1, *pSet0, *pn0, pDoc->m_tolerance);
            } // if结束
        } // if结束
        break;
		    case 2: // 环。
    case 3: // 区域。
    case 4: // 多边形。
        if (pDoc->m_flagShowA)
            gb_distanceMinPointLoop(da, ira, ida, pg, pDoc->m_a);
        else ida = -1;
        if (pDoc->m_flagShowB)
            gb_distanceMinPointLoop(db, irb, idb, pg, pDoc->m_b);
        else idb = -1;
		if (ida>=0)
        {
            if (idb>=0)
            {
                if (da<=db)
                {
                    pDoc->m_flagSelect = true;
                    pDoc->m_flagSelectPolygon = 0;
                    pDoc->m_flagSelectRegion = ira;
                    pDoc->m_flagSelectID = ida;
                }
                else
                {
                    pDoc->m_flagSelect = true;
                    pDoc->m_flagSelectPolygon = 1;
                    pDoc->m_flagSelectRegion = irb;
                    pDoc->m_flagSelectID = idb;
                }
            }
			else
            {
                pDoc->m_flagSelect = true;
                pDoc->m_flagSelectPolygon = 0;
                pDoc->m_flagSelectRegion = ira;
                pDoc->m_flagSelectID = ida;
            }
        }
        else
        {
            if (idb>=0)
            {
                pDoc->m_flagSelect = true;
                pDoc->m_flagSelectPolygon = 1;
                pDoc->m_flagSelectRegion = irb;
                pDoc->m_flagSelectID = idb;
            }
            else pDoc->m_flagSelect = false;
        }
        if (pDoc->m_flagSelect)
        {
            if (pDoc->m_flagMoveSame)
            {
                if (pDoc->m_flagSelectPolygon==0)
                {
                    pn0 = &(pDoc->m_a);
                    pn1 = &(pDoc->m_b);
                    pSet0 = &(pDoc->m_flagSelectIDSetInA);
                    pSet1 = &(pDoc->m_flagSelectIDSetInB);
                }
                else
                {
                    pn0 = &(pDoc->m_b);
                    pn1 = &(pDoc->m_a);
                    pSet0 = &(pDoc->m_flagSelectIDSetInB);
                    pSet1 = &(pDoc->m_flagSelectIDSetInA);
                } // if/else结束
				              if (pDoc->m_flagSelectType==4)
                    gb_intArrayInitPolygon(*pSet0, *pn0);
                else if (pDoc->m_flagSelectType==3)
                    gb_intArrayInitRegion(*pSet0, *pn0, pDoc->m_flagSelectRegion, pDoc->m_tolerance);
                else if (pDoc->m_flagSelectType==2)
                    gb_intArrayInitLoop(*pSet0, *pn0, pDoc->m_flagSelectRegion, pDoc->m_flagSelectID, pDoc->m_tolerance);
                gb_intArrayInitPolygonSamePoint(*pSet1, *pn1, *pSet0, *pn0, pDoc->m_tolerance);
            } // if结束
        } // if结束
        break;
    case 5: // 剖分三角形。
        break;
    } // switch结束

   CView::OnLButtonDown(nFlags, point);
    Invalidate( );
    if (pDoc->m_flagSelect)
        pDoc->m_flagMouseDown = true;
}




void CCP_PolygonPlatformView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CView::OnLButtonUp(nFlags, point);
	CView::OnLButtonUp(nFlags, point);
    // TODO: 在此添加消息处理程序代码和/或调用默认值
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    if (!pDoc->m_flagSelect)
        return;
    if (!pDoc->m_flagMouseDown)
        return;
    CP_Point ps, pg;
    ps.m_x = point.x;
    ps.m_y = point.y;
    CRect r;
    GetClientRect(& r);
    gb_pointConvertFromScreenToGlobal(pg, ps, 
        pDoc->m_scale, pDoc->m_translation, r.right, r.bottom);
	    double vx = pg.m_x - pDoc->m_basePoint.m_x;
    double vy = pg.m_y - pDoc->m_basePoint.m_y;
    if (pDoc->m_flagMoveSame)
    {
        gb_movePointIntArray(pDoc->m_a, pDoc->m_flagSelectIDSetInA, vx, vy);
        gb_movePointIntArray(pDoc->m_b, pDoc->m_flagSelectIDSetInB, vx, vy);
        pDoc->m_basePoint = pg;
        Invalidate( );
        pDoc->m_flagMouseDown = false;
        return;
    } // if结束
	   switch(pDoc->m_flagSelectType)
    {
    case 1: // 点。
        if (pDoc->m_flagSelectPolygon==0) // A
             gb_movePoint(pDoc->m_a, pDoc->m_flagSelectID, vx, vy);
        else gb_movePoint(pDoc->m_b, pDoc->m_flagSelectID, vx, vy);
        break;
    case 2: // 环。
        if (pDoc->m_flagSelectPolygon==0) // A
             gb_moveLoop(pDoc->m_a, pDoc->m_flagSelectRegion, pDoc->m_flagSelectID, vx, vy);
        else gb_moveLoop(pDoc->m_b, pDoc->m_flagSelectRegion, pDoc->m_flagSelectID, vx, vy);
        break;
    case 3: // 区域。
        if (pDoc->m_flagSelectPolygon==0) // A
             gb_moveRegion(pDoc->m_a, pDoc->m_flagSelectRegion, vx, vy);
        else gb_moveRegion(pDoc->m_b, pDoc->m_flagSelectRegion, vx, vy);
        break;
		    case 4: // 多边形。
        if (pDoc->m_flagSelectPolygon==0) // A
             gb_movePolygon(pDoc->m_a, vx, vy);
        else gb_movePolygon(pDoc->m_b, vx, vy);
        break;
    case 5: // 剖分三角形。
        break;
    } // switch结束

    pDoc->m_basePoint = pg;
    Invalidate( );
    pDoc->m_flagMouseDown = false;
}



void CCP_PolygonPlatformView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
    CView::OnMouseMove(nFlags, point);
    // TODO: 在此添加消息处理程序代码和/或调用默认值
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    if (!pDoc->m_flagSelect)
        return;
    if (!pDoc->m_flagMouseDown)
        return;
    CP_Point ps, pg;
    ps.m_x = point.x;
    ps.m_y = point.y;
    CRect r;
    GetClientRect(& r);
    gb_pointConvertFromScreenToGlobal(pg, ps, 
        pDoc->m_scale, pDoc->m_translation, r.right, r.bottom);
	   double vx = pg.m_x - pDoc->m_basePoint.m_x;
    double vy = pg.m_y - pDoc->m_basePoint.m_y;

    if (pDoc->m_flagMoveSame)
    {
        gb_movePointIntArray(pDoc->m_a, pDoc->m_flagSelectIDSetInA, vx, vy);
        gb_movePointIntArray(pDoc->m_b, pDoc->m_flagSelectIDSetInB, vx, vy);
        pDoc->m_basePoint = pg;
        Invalidate( );
        return;
    } // if结束

    switch(pDoc->m_flagSelectType)
    {
    case 1: // 点。
        if (pDoc->m_flagSelectPolygon==0) // A
             gb_movePoint(pDoc->m_a, pDoc->m_flagSelectID, vx, vy);
        else gb_movePoint(pDoc->m_b, pDoc->m_flagSelectID, vx, vy);
        break;
		    case 2: // 环。
        if (pDoc->m_flagSelectPolygon==0) // A
             gb_moveLoop(pDoc->m_a, pDoc->m_flagSelectRegion, pDoc->m_flagSelectID, vx, vy);
        else gb_moveLoop(pDoc->m_b, pDoc->m_flagSelectRegion, pDoc->m_flagSelectID, vx, vy);
        break;
    case 3: // 区域。
        if (pDoc->m_flagSelectPolygon==0) // A
             gb_moveRegion(pDoc->m_a, pDoc->m_flagSelectRegion, vx, vy);
        else gb_moveRegion(pDoc->m_b, pDoc->m_flagSelectRegion, vx, vy);
        break;
    case 4: // 多边形。
        if (pDoc->m_flagSelectPolygon==0) // A
             gb_movePolygon(pDoc->m_a, vx, vy);
        else gb_movePolygon(pDoc->m_b, vx, vy);
        break;
    case 5: // 剖分三角形。
        break;
    } // switch结束

    pDoc->m_basePoint = pg;
    Invalidate( );
}


void CCP_PolygonPlatformView::OnNewRightInloop()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    bool flagSuccess = false;
    bool flagA = true;
    CRect r;
    GetClientRect(& r);
    double dr = (r.right<r.bottom ? r.right : r.bottom);
    dr /=4;
    int ir = pDoc->m_flagSelectRegion;
    if (!pDoc->m_flagSelect)
    {
        if (pDoc->m_flagBuildA)
             flagSuccess = gb_polygonNewInLoopRegular(pDoc->m_a, pDoc->m_a.m_regionArray.size( )-1,
                pDoc->m_edgeNumber, dr, 0.0, 0.0);
        else
        {
            flagSuccess = gb_polygonNewInLoopRegular(pDoc->m_b, pDoc->m_b.m_regionArray.size( )-1,
                pDoc->m_edgeNumber, dr, 0.0, 0.0);
            flagA = false;
        } // if/else结束
    }
	else
    {
        switch(pDoc->m_flagSelectType)
        {
        case 1: // 点。
        case 4: // 多边形。
            if (pDoc->m_flagSelectPolygon==0)
                 flagSuccess = gb_polygonNewInLoopRegular(pDoc->m_a, pDoc->m_a.m_regionArray.size( )-1,
                    pDoc->m_edgeNumber, dr, 0.0, 0.0);
            else
            {
                flagSuccess = gb_polygonNewInLoopRegular(pDoc->m_b, pDoc->m_b.m_regionArray.size( )-1,
                    pDoc->m_edgeNumber, dr, 0.0, 0.0);
                flagA = false;
            } // if/else结束
            break;
            case 2: // 环。
        case 3: // 区域。
            if (pDoc->m_flagSelectPolygon==0)
                 flagSuccess = gb_polygonNewInLoopRegular(pDoc->m_a, pDoc->m_flagSelectRegion,
                    pDoc->m_edgeNumber, dr, 0.0, 0.0);
            else
            {
                flagSuccess = gb_polygonNewInLoopRegular(pDoc->m_b, pDoc->m_flagSelectRegion,
                    pDoc->m_edgeNumber, dr, 0.0, 0.0);
                flagA = false;
            } // if/else结束
            break;
        } // switch结束
    } // if/else结束
    Invalidate( );

    char s[100];
    sprintf_s(s, 100, "新内环位于多边形%c中。", (flagA ? 'A' : 'B'));
    if (flagSuccess)
         mb_statusSetText("新内环创建成功。", s);
    else mb_statusSetText("没有创建新内环。", s);
}




void CCP_PolygonPlatformView::OnAddOutloop()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    if (pDoc->m_flagAdd!=0)
    {
        MessageBox("前一个添加操作还没有完成，请继续前一个操作或按鼠标右键结束前一个添加操作。", "操作错误");
    } // if结束
	if (pDoc->m_flagSelect)
    {
        if (pDoc->m_flagSelectPolygon==0)
             pDoc->m_flagAddIDPolygon = 0;
        else pDoc->m_flagAddIDPolygon = 1;
    }
    else
    {
        if (pDoc->m_flagBuildA)
             pDoc->m_flagAddIDPolygon = 0;
        else pDoc->m_flagAddIDPolygon = 1;
    } // if/else结束
    pDoc->m_flagAddPointArray.clear( );
    pDoc->m_flagAdd = 1;
    mb_statusSetText("请按鼠标左键在工作区中添加新外环的点", "请用鼠标右键结束新外环添加操作");
}


void CCP_PolygonPlatformView::OnAddInloop()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    if (pDoc->m_flagAdd!=0)
    {
        MessageBox("前一个添加操作还没有完成，请继续前一个操作或按鼠标右键结束前一个添加操作。", "操作错误");
    } // if结束
    bool flagSuceess = false;
    CP_Polygon* pn = NULL;
	    if (pDoc->m_flagSelect)
    {
        if (pDoc->m_flagSelectPolygon==0)
        {
            pDoc->m_flagAddIDPolygon = 0;
            pn = &(pDoc->m_a);
        }
        else
        {
            pDoc->m_flagAddIDPolygon = 1;
            pn = &(pDoc->m_b);
        } // if/else结束
		       switch(pDoc->m_flagSelectType)
        {
        case 1: // 点。
            flagSuceess = gb_findPointInLoop(*pn, pDoc->m_flagAddIDRegion,
                pDoc->m_flagAddIDLoop, pDoc->m_flagAddIDPointInLoop,
                pDoc->m_flagSelectID);
            break;
        case 2: // 环。
            pDoc->m_flagAddIDRegion = pDoc->m_flagSelectRegion;
            pDoc->m_flagAddIDLoop = pDoc->m_flagSelectID;
            flagSuceess = true;
            break;
        case 3: // 区域。
            pDoc->m_flagAddIDRegion = pDoc->m_flagSelectRegion;
            pDoc->m_flagAddIDLoop = (*pn).m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray.size( )-1;
            flagSuceess = true;
            break;
			        case 4: // 多边形。
            pDoc->m_flagAddIDRegion = (*pn).m_regionArray.size( )-1;
            if (pDoc->m_flagAddIDRegion<0)
                break;
            pDoc->m_flagAddIDLoop = (*pn).m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray.size( )-1;
            if (pDoc->m_flagAddIDLoop<0)
                break;
            flagSuceess = true;
            break;
        } // switch结束
    }
    else
    {
        if (pDoc->m_flagBuildA)
        {
            pDoc->m_flagAddIDPolygon = 0;
            pn = &(pDoc->m_a);
        }
        else
        {
            pDoc->m_flagAddIDPolygon = 1;
            pn = &(pDoc->m_b);
        } // if/else结束

        pDoc->m_flagAddIDRegion = (*pn).m_regionArray.size( )-1;
        if (pDoc->m_flagAddIDRegion>=0)
        {
            pDoc->m_flagAddIDLoop = (*pn).m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray.size( )-1;
            if (pDoc->m_flagAddIDLoop>=0)
            {
                flagSuceess = true;
            } // if(idL)结束
        } // if(idr)结束
    } // if/else结束
    if (flagSuceess)
    {
        pDoc->m_flagAddPointArray.clear( );
        pDoc->m_flagAdd = 2;
        mb_statusSetText("请按鼠标左键在工作区中添加新内环的点", "请用鼠标右键结束新内环添加操作");
    }
    else
    {
        pDoc->m_flagAdd = 0;
        if (pDoc->m_flagAddIDPolygon==0)
            mb_statusSetText("内环添加操作失败。", "多边形A还没有任何外环。");
        else
            mb_statusSetText("内环添加操作失败。", "请给多边形B增加外环，再执行本操作。");
    } // if/else结束
}




void CCP_PolygonPlatformView::OnAddPoint()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    if (pDoc->m_flagAdd!=0)
    {
        MessageBox("前一个添加操作还没有完成，请继续前一个操作或按鼠标右键结束前一个添加操作。", "操作错误");
        return;
    } // if结束

    bool flagSuceess = false;
    CP_Polygon* pn = NULL;
	   if (pDoc->m_flagSelect)
    {
        if (pDoc->m_flagSelectPolygon==0)
        {
            pDoc->m_flagAddIDPolygon = 0;
            pn = &(pDoc->m_a);
        }
        else
        {
            pDoc->m_flagAddIDPolygon = 1;
            pn = &(pDoc->m_b);
        } // if/else结束

        switch(pDoc->m_flagSelectType)
        {
        case 1: // 点。
            flagSuceess = gb_findPointInLoop(*pn, pDoc->m_flagAddIDRegion,
                pDoc->m_flagAddIDLoop, pDoc->m_flagAddIDPointInLoop,
                pDoc->m_flagSelectID);
            break;
      case 2: // 环。
            pDoc->m_flagAddIDRegion = pDoc->m_flagSelectRegion;
            pDoc->m_flagAddIDLoop = pDoc->m_flagSelectID;
            pDoc->m_flagAddIDPointInLoop = (*pn).m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray[pDoc->m_flagAddIDLoop].m_pointIDArray.size( )-1; 
            if (pDoc->m_flagAddIDPointInLoop<0)
                break;
            flagSuceess = true;
            break;
        case 3: // 区域。
            pDoc->m_flagAddIDRegion = pDoc->m_flagSelectRegion;
            pDoc->m_flagAddIDLoop = (*pn).m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray.size( )-1;
            pDoc->m_flagAddIDPointInLoop = (*pn).m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray[pDoc->m_flagAddIDLoop].m_pointIDArray.size( )-1; 
            if (pDoc->m_flagAddIDPointInLoop<0)
                break;
            flagSuceess = true;
            break;
			        case 4: // 多边形。
            pDoc->m_flagAddIDRegion = (*pn).m_regionArray.size( )-1;
            if (pDoc->m_flagAddIDRegion<0)
                break;
            pDoc->m_flagAddIDLoop = (*pn).m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray.size( )-1;
            if (pDoc->m_flagAddIDLoop<0)
                break;
            pDoc->m_flagAddIDPointInLoop = (*pn).m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray[pDoc->m_flagAddIDLoop].m_pointIDArray.size( )-1; 
            if (pDoc->m_flagAddIDPointInLoop<0)
                break;
            flagSuceess = true;
            break;
        } // switch结束
    }
        else
    {
        if (pDoc->m_flagBuildA)
        {
            pDoc->m_flagAddIDPolygon = 0;
            pn = &(pDoc->m_a);
        }
        else
        {
            pDoc->m_flagAddIDPolygon = 1;
            pn = &(pDoc->m_b);
        } // if/else结束

        pDoc->m_flagAddIDRegion = (*pn).m_regionArray.size( )-1;
		        if (pDoc->m_flagAddIDRegion>=0)
        {
            pDoc->m_flagAddIDLoop = (*pn).m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray.size( )-1;
            if (pDoc->m_flagAddIDLoop>=0)
            {
                pDoc->m_flagAddIDPointInLoop = (*pn).m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray[pDoc->m_flagAddIDLoop].m_pointIDArray.size( )-1; 
                if (pDoc->m_flagAddIDPointInLoop>=0)
                {
                    flagSuceess = true;
                } // if(idLv)结束
            } // if(idL)结束
        } // if(idr)结束
    } // if/else结束
	       if (flagSuceess)
    {
        pDoc->m_flagAdd = 3;
        mb_statusSetText("用鼠标左键在工作区中添加点", "用鼠标右键结束点添加操作");
    }
    else
    {
        pDoc->m_flagAdd = 0;
        if (pDoc->m_flagAddIDPolygon==0)
            mb_statusSetText("点添加操作失败。", "多边形A还没有任何外环。");
        else
            mb_statusSetText("点添加操作失败。", "请给多边形B增加外环，再执行本操作。");
        } // if/else结束
}




void CCP_PolygonPlatformView::OnDelete()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    if (!pDoc->m_flagSelect)
    {
        MessageBox("还没有拾取图形，无法删除。", "无效操作");
		return;
    } // if结束
    if (pDoc->m_flagAdd!=0)
    {
        MessageBox("添加图形的操作还没有结束，无法删除。", "无效操作");
		return;
    } // if结束
	   CP_Polygon* pn;
    if (pDoc->m_flagSelectPolygon==0)
         pn = &(pDoc->m_a);
    else pn = &(pDoc->m_b);
    switch(pDoc->m_flagSelectType)
    {
    case 1: // 点。
        gb_removePoint(*pn, pDoc->m_flagSelectID);
        break;
    case 2: // 环。
        gb_removeLoop(*pn, pDoc->m_flagSelectRegion, pDoc->m_flagSelectID);
        break;
    case 3: // 区域。
        gb_removeRegion(*pn, pDoc->m_flagSelectRegion);
        break;
    case 4: // 多边形。
        pn->mb_clear( );
        break;
    } // switch结束
    pDoc->m_flagSelect = false;
    Invalidate(); // 刷新
}


void CCP_PolygonPlatformView::OnUpdateMoveSame(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    pCmdUI->SetCheck(pDoc->m_flagMoveSame);

}


void CCP_PolygonPlatformView::OnMoveSame()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    pDoc->m_flagMoveSame ^= true;
    if (!(pDoc->m_flagMoveSame))
    {
        pDoc->m_flagSelectIDSetInA.clear( );
        pDoc->m_flagSelectIDSetInB.clear( );
    } // if结束

}


void CCP_PolygonPlatformView::OnUpdateViewA(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    pCmdUI->SetCheck(pDoc->m_flagShowA);

}




void CCP_PolygonPlatformView::OnViewA()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    pDoc->m_flagShowA ^= true;

    if (!(pDoc->m_flagShowA))//如果不显示A
        if (pDoc->m_flagSelectPolygon==0)//拾取到A
            pDoc->m_flagSelect = false;//则表示什么都没有拾取到

    Invalidate(); // 刷新

}


void CCP_PolygonPlatformView::OnUpdateViewB(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    pCmdUI->SetCheck(pDoc->m_flagShowB);

}



void CCP_PolygonPlatformView::OnViewB()
{
	// TODO: 在此添加命令处理程序代码
		CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    pDoc->m_flagShowB ^= true;
    if (!(pDoc->m_flagShowB))
        if (pDoc->m_flagSelectPolygon!=0)
            pDoc->m_flagSelect = false;
    Invalidate(); // 刷新

}


void CCP_PolygonPlatformView::OnUpdateViewPointId(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    pCmdUI->SetCheck(pDoc->m_flagShowPointID);

}



void CCP_PolygonPlatformView::OnViewPointId()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    pDoc->m_flagShowPointID ^= true;
    Invalidate(); // 刷新

}


void CCP_PolygonPlatformView::OnViewTFace()
{
	// TODO: 在此添加命令处理程序代码
		CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    pDoc->m_flagShowTriangleFace ^= true;
    Invalidate(); // 刷新

}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CCP_PolygonPlatformView::OnViewTEdge()
{
	// TODO: 在此添加命令处理程序代码

}


void CCP_PolygonPlatformView::OnUpdateViewTFace(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    pCmdUI->SetCheck(pDoc->m_flagShowTriangleFace);

}

/*显示布尔运算结果*/
void CCP_PolygonPlatformView::OnViewResult()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    pDoc->m_flagShowResult ^= true;

    Invalidate(); // 刷新
}

void CCP_PolygonPlatformView::OnUpdateViewResult(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    pCmdUI->SetCheck(pDoc->m_flagShowResult);
}

/*并运算*/
void CCP_PolygonPlatformView::OnPolygonUnion()
{
	OnCheck();//先进行合法性检查
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();//获取Doc类指针
	int polyR, polyL, polyP;//分别表示多边形中区域、环、点数组的大小 
	AddNodes(pDoc);//求多边形交点，并将添加交点后的多边形赋给tempA和tempB
	
	/*先得到A∪B中所有的边*/
	pDoc->ABEdge.clear();//先清空
	/*属于A且不被B包含的边+A、B重合且同向的边*/
	polyR = tempA.m_regionArray.size();
	for(int i=0; i<polyR; i++)//循环遍历每个区域
	{
		polyL = tempA.m_regionArray[i].m_loopArray.size();
		for(int j=0; j<polyL; j++)//循环遍历每个环
		{
			polyP = tempA.m_regionArray[i].m_loopArray[j].m_pointIDArray.size();
			for(int k=0; k<polyP; k++)//循环遍历每条边
			{
				int u1, u2;//边端点的序号
				u1 = tempA.m_regionArray[i].m_loopArray[j].m_pointIDArray[k];
				u2 = tempA.m_regionArray[i].m_loopArray[j].m_pointIDArray[(k+1)%polyP];
				Segment temp;//临时变量存储当前边
				temp.u1 = tempA.m_pointArray[u1];//获取线段端点坐标，赋给临时结构变量
				temp.u2 = tempA.m_pointArray[u2];

				/*判断该线段是否是AB重合且同向的边*/
				vector<Segment>::iterator iter;
				iter = find(SegCoinSame.begin(),SegCoinSame.end(),temp);//在边重合且同向数组中寻找看有没有当前边

				if((SegCoinSame.size()==0||iter==SegCoinSame.end())&&SegInPolygon(temp.u1,temp.u2,pDoc->m_b))//如果该边既不是重合且同向的边，又属于B，继续循环
				{
					continue;
				}
				else//其他情况：【A、B重合且同向】 或 【属于A且不被B包含】，将其加入到ABEdge中
				{
					pDoc->ABEdge.push_back(temp);//将边添加到A∪B的边集合中
				}
			}
		}
	}
	/*属于B且不被A包含的边*/
	polyR = tempB.m_regionArray.size();
	for(int i=0; i<polyR; i++)//循环遍历每个区域
	{
		polyL = tempB.m_regionArray[i].m_loopArray.size();
		for(int j=0; j<polyL; j++)//循环遍历每个环
		{
			polyP = tempB.m_regionArray[i].m_loopArray[j].m_pointIDArray.size();
			for(int k=0; k<polyP; k++)//循环遍历每条边
			{
				int u1, u2;//边端点的序号
				u1 = tempB.m_regionArray[i].m_loopArray[j].m_pointIDArray[k];
				u2 = tempB.m_regionArray[i].m_loopArray[j].m_pointIDArray[(k+1)%polyP];
				Segment temp;//临时变量存储当前边
				temp.u1 = tempB.m_pointArray[u1];//获取线段端点坐标，赋给临时结构变量
				temp.u2 = tempB.m_pointArray[u2];

				if(SegInPolygon(temp.u1,temp.u2,pDoc->m_a))//如果该边属于A，继续循环(包括了线段重合的情况)
				{
					continue;
				}
				else//其他情况：【属于A且不被B包含】，将其加入到ABEdge中
				{
					pDoc->ABEdge.push_back(temp);//将边添加到A∪B的边集合中
				}
			}
		}
	}

	/*遍历边集合，得到最终的多边形结构*/
	sort(pDoc->ABEdge.begin(),pDoc->ABEdge.end(),ascending);//先对vector中元素按照x-y坐标升序排序
	ConstructPolygon(pDoc);//将边集合构建成多边形

	Invalidate(true);
}

/*交运算*/
void CCP_PolygonPlatformView::OnPolygonIntersection()
{
	OnCheck();//先进行合法性检查
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();//获取Doc类指针
	int polyR, polyL, polyP;//分别表示多边形中区域、环、点数组的大小 
	AddNodes(pDoc);//求多边形交点，并将添加交点后的多边形赋给tempA和tempB
	
	/*先得到A∩B中所有的边*/
	pDoc->ABEdge.clear();//先清空
	/*属于A且被B包含的边+A、B重合且同向的边*/
	polyR = tempA.m_regionArray.size();
	for(int i=0; i<polyR; i++)//循环遍历每个区域
	{
		polyL = tempA.m_regionArray[i].m_loopArray.size();
		for(int j=0; j<polyL; j++)//循环遍历每个环
		{
			polyP = tempA.m_regionArray[i].m_loopArray[j].m_pointIDArray.size();
			for(int k=0; k<polyP; k++)//循环遍历每条边
			{
				int u1, u2;//边端点的序号
				u1 = tempA.m_regionArray[i].m_loopArray[j].m_pointIDArray[k];
				u2 = tempA.m_regionArray[i].m_loopArray[j].m_pointIDArray[(k+1)%polyP];
				Segment temp;//临时变量存储当前边
				temp.u1 = tempA.m_pointArray[u1];//获取线段端点坐标，赋给临时结构变量
				temp.u2 = tempA.m_pointArray[u2];

				/*判断该线段是否是AB重合且同向的边*/
				vector<Segment>::iterator iter;
				iter = find(SegCoinSame.begin(),SegCoinSame.end(),temp);//在边重合且同向数组中寻找看有没有当前边

				if((SegCoinSame.size()==0||iter==SegCoinSame.end())&&!SegInPolygon(temp.u1,temp.u2,pDoc->m_b))//如果该边既不是重合且同向的边，又不属于B，继续循环
				{
					continue;
				}
				else//其他情况：【A、B重合且同向】 或 【属于A且被B包含】，将其加入到ABEdge中
				{
					pDoc->ABEdge.push_back(temp);//将边添加到A∩B的边集合中
				}
			}
		}
	}
	/*属于B且被A包含的边*/
	polyR = tempB.m_regionArray.size();
	for(int i=0; i<polyR; i++)//循环遍历每个区域
	{
		polyL = tempB.m_regionArray[i].m_loopArray.size();
		for(int j=0; j<polyL; j++)//循环遍历每个环
		{
			polyP = tempB.m_regionArray[i].m_loopArray[j].m_pointIDArray.size();
			for(int k=0; k<polyP; k++)//循环遍历每条边
			{
				int u1, u2;//边端点的序号
				u1 = tempB.m_regionArray[i].m_loopArray[j].m_pointIDArray[k];
				u2 = tempB.m_regionArray[i].m_loopArray[j].m_pointIDArray[(k+1)%polyP];
				Segment temp;//临时变量存储当前边
				temp.u1 = tempB.m_pointArray[u1];//获取线段端点坐标，赋给临时结构变量
				temp.u2 = tempB.m_pointArray[u2];

				/*判断该线段是否是AB重合且同向的边*/
				vector<Segment>::iterator iter;
				iter = find(SegCoinSame.begin(),SegCoinSame.end(),temp);//在边重合且同向数组中寻找看有没有当前边
				if(iter!=SegCoinSame.end()||!SegInPolygon(temp.u1,temp.u2,pDoc->m_a))//如果该边不属于A，继续循环(包括了线段重合的情况)
				{
					continue;
				}
				else//其他情况：【属于A且被B包含】，将其加入到ABEdge中
				{
					pDoc->ABEdge.push_back(temp);//将边添加到A∩B的边集合中
				}
			}
		}
	}

	/*遍历边集合，得到最终的多边形结构*/
	sort(pDoc->ABEdge.begin(),pDoc->ABEdge.end(),ascending);//先对vector中元素按照x-y坐标升序排序
	ConstructPolygon(pDoc);//将边集合构建成多边形

	Invalidate(true);
}

/*差运算（A-B）*/
void CCP_PolygonPlatformView::OnPolygonAB()
{
	OnCheck();//先进行合法性检查
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();//获取Doc类指针
	int polyR, polyL, polyP;//分别表示多边形中区域、环、点数组的大小 
	AddNodes(pDoc);//求多边形交点，并将添加交点后的多边形赋给tempA和tempB
	
	/*先得到A-B中所有的边*/
	pDoc->ABEdge.clear();//先清空
	/*属于A且不被B包含的边+A、B重合且反向的边*/
	polyR = tempA.m_regionArray.size();
	for(int i=0; i<polyR; i++)//循环遍历每个区域
	{
		polyL = tempA.m_regionArray[i].m_loopArray.size();
		for(int j=0; j<polyL; j++)//循环遍历每个环
		{
			polyP = tempA.m_regionArray[i].m_loopArray[j].m_pointIDArray.size();
			for(int k=0; k<polyP; k++)//循环遍历每条边
			{
				int u1, u2;//边端点的序号
				u1 = tempA.m_regionArray[i].m_loopArray[j].m_pointIDArray[k];
				u2 = tempA.m_regionArray[i].m_loopArray[j].m_pointIDArray[(k+1)%polyP];
				Segment temp;//临时变量存储当前边
				temp.u1 = tempA.m_pointArray[u1];//获取线段端点坐标，赋给临时结构变量
				temp.u2 = tempA.m_pointArray[u2];

				/*判断该线段是否是AB重合且反向的边*/
				vector<Segment>::iterator iterR;
				iterR = find(SegCoinReve.begin(),SegCoinReve.end(),temp);//在边重合且反向数组中寻找看有没有当前边
				/*判断该线段是否是AB重合且同向的边*/
				vector<Segment>::iterator iterS;
				iterS = find(SegCoinSame.begin(),SegCoinSame.end(),temp);//在边重合且反向数组中寻找看有没有当前边

				if(iterS!=SegCoinSame.end() || ((SegCoinReve.size()==0||iterR==SegCoinReve.end())&&SegInPolygon(temp.u1,temp.u2,pDoc->m_b)))//如果该边是重合同向的边或者既不是重合且反向的边，又属于B，继续循环
				{
					continue;
				}
				else//其他情况：【A、B重合且反向】 或 【属于A且不被B包含】，将其加入到ABEdge中
				{
					pDoc->ABEdge.push_back(temp);//将边添加到A∪B的边集合中
				}
			}
		}
	}
	/*属于B且被A包含的边，并将它们反向*/
	polyR = tempB.m_regionArray.size();
	for(int i=0; i<polyR; i++)//循环遍历每个区域
	{
		polyL = tempB.m_regionArray[i].m_loopArray.size();
		for(int j=0; j<polyL; j++)//循环遍历每个环
		{
			polyP = tempB.m_regionArray[i].m_loopArray[j].m_pointIDArray.size();
			for(int k=0; k<polyP; k++)//循环遍历每条边
			{
				int u1, u2;//边端点的序号
				u1 = tempB.m_regionArray[i].m_loopArray[j].m_pointIDArray[k];
				u2 = tempB.m_regionArray[i].m_loopArray[j].m_pointIDArray[(k+1)%polyP];
				Segment temp;//临时变量存储当前边
				temp.u1 = tempB.m_pointArray[u1];//获取线段端点坐标，赋给临时结构变量
				temp.u2 = tempB.m_pointArray[u2];

				/*判断该线段是否是AB重合且反向的边*/
				vector<Segment>::iterator iterR;
				iterR = find(SegCoinReve.begin(),SegCoinReve.end(),temp);//在边重合且反向数组中寻找看有没有当前边
				/*判断该线段是否是AB重合且同向的边*/
				vector<Segment>::iterator iterS;
				iterS = find(SegCoinSame.begin(),SegCoinSame.end(),temp);//在边重合且反向数组中寻找看有没有当前边

				if((SegCoinReve.size()!=0&&iterR!=SegCoinReve.end()) || (SegCoinSame.size()!=0&&iterS!=SegCoinSame.end()))//线段重合,说明包含在A里面，继续循环
				{
					continue;
				}
				else if(!SegInPolygon(temp.u1,temp.u2,pDoc->m_a))//如果该边不属于A，继续循环(包括了线段重合的情况)
				{
					continue;
				}
				else//其他情况：【属于B且被A包含】，将其反向，并加入到ABEdge中
				{
					temp.u1 = tempB.m_pointArray[u2];
					temp.u2 = tempB.m_pointArray[u1];
					pDoc->ABEdge.push_back(temp);//将边添加到A-B的边集合中
				}
			}
		}
	}

	/*遍历边集合，得到最终的多边形结构*/
	sort(pDoc->ABEdge.begin(),pDoc->ABEdge.end(),ascending);//先对vector中元素按照x-y坐标升序排序
	ConstructPolygon(pDoc);//将边集合构建成多边形

	Invalidate(true);
}

/*差运算（B-A）*/
void CCP_PolygonPlatformView::OnPolygonBA()
{
	OnCheck();//先进行合法性检查
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();//获取Doc类指针
	int polyR, polyL, polyP;//分别表示多边形中区域、环、点数组的大小 
	AddNodes(pDoc);//求多边形交点，并将添加交点后的多边形赋给tempA和tempB
	
	/*先得到B-A中所有的边*/
	pDoc->ABEdge.clear();//先清空
	/*属于B且不被A包含的边+A、B重合且反向的边*/
	polyR = tempB.m_regionArray.size();
	for(int i=0; i<polyR; i++)//循环遍历每个区域
	{
		polyL = tempB.m_regionArray[i].m_loopArray.size();
		for(int j=0; j<polyL; j++)//循环遍历每个环
		{
			polyP = tempB.m_regionArray[i].m_loopArray[j].m_pointIDArray.size();
			for(int k=0; k<polyP; k++)//循环遍历每条边
			{
				int u1, u2;//边端点的序号
				u1 = tempB.m_regionArray[i].m_loopArray[j].m_pointIDArray[k];
				u2 = tempB.m_regionArray[i].m_loopArray[j].m_pointIDArray[(k+1)%polyP];
				Segment temp;//临时变量存储当前边
				temp.u1 = tempB.m_pointArray[u1];//获取线段端点坐标，赋给临时结构变量
				temp.u2 = tempB.m_pointArray[u2];

				/*判断该线段是否是AB重合且同向的边*/
				vector<Segment>::iterator iterS;
				iterS = find(SegCoinSame.begin(),SegCoinSame.end(),temp);//在边重合且反向数组中寻找看有没有当前边

				if(iterS!=SegCoinSame.end() || SegInPolygon(temp.u1,temp.u2,pDoc->m_a))//如果该边属于A，继续循环
				{
					continue;
				}
				else//其他情况：【属于B且不被A包含】，将其加入到ABEdge中
				{
					pDoc->ABEdge.push_back(temp);//将边添加到A∪B的边集合中
				}
			}
		}
	}
	/*属于A且被B包含的边，并将它们反向*/
	polyR = tempA.m_regionArray.size();
	for(int i=0; i<polyR; i++)//循环遍历每个区域
	{
		polyL = tempA.m_regionArray[i].m_loopArray.size();
		for(int j=0; j<polyL; j++)//循环遍历每个环
		{
			polyP = tempA.m_regionArray[i].m_loopArray[j].m_pointIDArray.size();
			for(int k=0; k<polyP; k++)//循环遍历每条边
			{
				int u1, u2;//边端点的序号
				u1 = tempA.m_regionArray[i].m_loopArray[j].m_pointIDArray[k];
				u2 = tempA.m_regionArray[i].m_loopArray[j].m_pointIDArray[(k+1)%polyP];
				Segment temp;//临时变量存储当前边
				temp.u1 = tempA.m_pointArray[u1];//获取线段端点坐标，赋给临时结构变量
				temp.u2 = tempA.m_pointArray[u2];

				/*判断该线段是否是AB重合且反向的边*/
				vector<Segment>::iterator iter;
				iter = find(SegCoinReve.begin(),SegCoinReve.end(),temp);//在边重合且反向数组中寻找看有没有当前边
				/*判断该线段是否是AB重合且同向的边*/
				vector<Segment>::iterator iterS;
				iterS = find(SegCoinSame.begin(),SegCoinSame.end(),temp);//在边重合且反向数组中寻找看有没有当前边

				if(iterS!=SegCoinSame.end() || (SegCoinReve.size()==0||iter==SegCoinReve.end())&&!SegInPolygon(temp.u1,temp.u2,pDoc->m_b))//如果该边不是重合且反向的边,且该边不属于B
				{
					continue;
				}
				else if(iter!=SegCoinReve.end())
				{
					pDoc->ABEdge.push_back(temp);//将边添加到A-B的边集合中
				}
				else//其他情况：【属于A且被B包含】，将其反向，并加入到ABEdge中
				{
					temp.u1 = tempA.m_pointArray[u2];
					temp.u2 = tempA.m_pointArray[u1];
					pDoc->ABEdge.push_back(temp);//将反向后的边添加到A-B的边集合中
				}
			}
		}
	}

	/*遍历边集合，得到最终的多边形结构*/
	sort(pDoc->ABEdge.begin(),pDoc->ABEdge.end(),ascending);//先对vector中元素按照x-y坐标升序排序
	ConstructPolygon(pDoc);//将边集合构建成多边形

	Invalidate(true);
}

/*合法性检查
  如下几种检查：
  1.外环应为逆时针，内环应为顺时针（手动添加内外环时可能会出错）；
    同时，检查每个环中点的个数是否>=3，若不是，不合法。
  2.环 必须是简单多边形，即环中任意两条边不能有除端点外的其它顶点；
  3.多边形的一个区域中，内环必须在该区域的外环的内部；
  4.多边形的一个区域中，内环之间不能相互包含；
  5.多边形的一个区域中，内环间交点只能是端点，不能是边或其它；
  6.同一个多边形的不同区域不能相交，若相交，交点只能是端点，不能是边或其它。
   （注意当多边形只有两个外环，且这两个外环相互嵌套的情况！）
*/
/*检查各个环的方向+环中点的个数>=3*/
bool LoopDirection(CCP_PolygonPlatformDoc* pDoc)
{
	int polyR, polyL, polyP;//分别表示多边形中区域、环、点数组的大小

	//判断多边形A是否合法
	polyR = pDoc->m_a.m_regionArray.size();
	for(int i=0; i<polyR; i++)//依次判断每个区域是否合法
	{
		//判断该区域中的外环是否合法（逆时针,叉乘为正，只需要凸点连接的两条边即可）
		polyP = pDoc->m_a.m_regionArray[i].m_loopArray[0].m_pointIDArray.size();
		if(polyP < 3)//顶点过少
		{
			MessageBoxA(0,"多边形A(外环)顶点过少，请重新构造！","警告",MB_OK);
			pDoc->m_a.mb_clear();//清空
			if(!pDoc->m_a.m_pointArray.size()&&!pDoc->m_b.m_pointArray.size())
			{
				pDoc->mb_initData();
			}
			return false;
		}
		else
		{
			double maxX=-9999999, x1, x2, y1, y2;//最右侧凸点的x坐标，两条边向量的坐标
			int maxID, u1, u2, u3;//最右侧凸点的ID，三个顶点在顶点数组中的ID

			for(int p=0; p<polyP; p++)//寻找最右侧凸点
			{
				int u = pDoc->m_a.m_regionArray[i].m_loopArray[0].m_pointIDArray[p];
				if(maxX < pDoc->m_a.m_pointArray[u].m_x)
				{
					maxX = pDoc->m_a.m_pointArray[u].m_x;
					maxID = p;
				}
			}
			u1 = pDoc->m_a.m_regionArray[i].m_loopArray[0].m_pointIDArray[(maxID-1+polyP)%polyP];
			u2 = pDoc->m_a.m_regionArray[i].m_loopArray[0].m_pointIDArray[maxID];
			u3 = pDoc->m_a.m_regionArray[i].m_loopArray[0].m_pointIDArray[(maxID+1)%polyP];
			x1 = pDoc->m_a.m_pointArray[u2].m_x-pDoc->m_a.m_pointArray[u1].m_x;
			y1 = pDoc->m_a.m_pointArray[u2].m_y-pDoc->m_a.m_pointArray[u1].m_y;
			x2 = pDoc->m_a.m_pointArray[u3].m_x-pDoc->m_a.m_pointArray[u2].m_x;
			y2 = pDoc->m_a.m_pointArray[u3].m_y-pDoc->m_a.m_pointArray[u2].m_y;
			if(x1*y2-x2*y1<=0)//叉乘不是正，表明此外环不合法
			{
				MessageBoxA(0,"多边形A(外环)方向错误，请重新构造！","警告",MB_OK);
				pDoc->m_a.mb_clear();//清空
				if(!pDoc->m_a.m_pointArray.size()&&!pDoc->m_b.m_pointArray.size())
				{
					pDoc->mb_initData();
				}
				return false;
			}
		}

		//顺序判断所有内环是否合法（顺时针，叉乘为负，只需要判断第1、2条边即可）
		polyL = pDoc->m_a.m_regionArray[i].m_loopArray.size();
		for(int j=1; j<polyL; j++)//内环从环数组的第二个位置开始
		{
			polyP = pDoc->m_a.m_regionArray[i].m_loopArray[j].m_pointIDArray.size();
			if(polyP < 3)//顶点过少
			{
				MessageBoxA(0,"多边形A(内环)顶点过少，请重新构造！","警告",MB_OK);
				pDoc->m_a.mb_clear();//清空
				if(!pDoc->m_a.m_pointArray.size()&&!pDoc->m_b.m_pointArray.size())
				{
					pDoc->mb_initData();
				}
				return false;
			}
			else
			{
				double maxX=-9999999, x1, x2, y1, y2;//最右侧凸点的x坐标，两条边向量的坐标
				int maxID, u1, u2, u3;//最右侧凸点在环中的序号，三个顶点在顶点数组中的ID

				for(int p=0; p<polyP; p++)//寻找最右侧凸点
				{
					int u = pDoc->m_a.m_regionArray[i].m_loopArray[j].m_pointIDArray[p];
					if(maxX < pDoc->m_a.m_pointArray[u].m_x)
					{
						maxX = pDoc->m_a.m_pointArray[u].m_x;
						maxID = p;
					}
				}
				u1 = pDoc->m_a.m_regionArray[i].m_loopArray[j].m_pointIDArray[(maxID-1+polyP)%polyP];
				u2 = pDoc->m_a.m_regionArray[i].m_loopArray[j].m_pointIDArray[maxID];
				u3 = pDoc->m_a.m_regionArray[i].m_loopArray[j].m_pointIDArray[(maxID+1)%polyP];
				x1 = pDoc->m_a.m_pointArray[u2].m_x-pDoc->m_a.m_pointArray[u1].m_x;
				y1 = pDoc->m_a.m_pointArray[u2].m_y-pDoc->m_a.m_pointArray[u1].m_y;
				x2 = pDoc->m_a.m_pointArray[u3].m_x-pDoc->m_a.m_pointArray[u2].m_x;
				y2 = pDoc->m_a.m_pointArray[u3].m_y-pDoc->m_a.m_pointArray[u2].m_y;
				if(x1*y2-x2*y1>=0)//叉乘不是负，表明此内环不合法
				{
					MessageBoxA(0,"多边形A(内环)方向错误，请重新构造！","警告",MB_OK);
					pDoc->m_a.mb_clear();//清空
					if(!pDoc->m_a.m_pointArray.size()&&!pDoc->m_b.m_pointArray.size())
					{
						pDoc->mb_initData();
					}
					return false;
				}
			}
		}
	}

	//判断多边形B是否合法（判断各个环的方向）
	polyR = pDoc->m_b.m_regionArray.size();
	for(int i=0; i<polyR; i++)//依次判断每个区域是否合法
	{
		//判断该区域中的外环是否合法（逆时针,叉乘为正，只需要判断第1、2条边即可）
		polyP = pDoc->m_b.m_regionArray[i].m_loopArray[0].m_pointIDArray.size();
		if(polyP < 3)//顶点过少
		{
			MessageBoxA(0,"多边形B(外环)顶点过少，请重新构造！","警告",MB_OK);
			pDoc->m_b.mb_clear();//清空
			if(!pDoc->m_a.m_pointArray.size()&&!pDoc->m_b.m_pointArray.size())
			{
				pDoc->mb_initData();
			}
			return false;
		}
		else
		{
			double maxX=-9999999, x1, x2, y1, y2;//最右侧凸点的x坐标，两条边向量的坐标
			int maxID, u1, u2, u3;//最右侧凸点的ID，三个顶点在顶点数组中的ID

			for(int p=0; p<polyP; p++)//寻找最右侧凸点
			{
				int u = pDoc->m_b.m_regionArray[i].m_loopArray[0].m_pointIDArray[p];
				if(maxX < pDoc->m_b.m_pointArray[u].m_x)
				{
					maxX = pDoc->m_b.m_pointArray[u].m_x;
					maxID = p;
				}
			}
			u1 = pDoc->m_b.m_regionArray[i].m_loopArray[0].m_pointIDArray[(maxID-1+polyP)%polyP];
			u2 = pDoc->m_b.m_regionArray[i].m_loopArray[0].m_pointIDArray[maxID];
			u3 = pDoc->m_b.m_regionArray[i].m_loopArray[0].m_pointIDArray[(maxID+1)%polyP];
			x1 = pDoc->m_b.m_pointArray[u2].m_x-pDoc->m_b.m_pointArray[u1].m_x;
			y1 = pDoc->m_b.m_pointArray[u2].m_y-pDoc->m_b.m_pointArray[u1].m_y;
			x2 = pDoc->m_b.m_pointArray[u3].m_x-pDoc->m_b.m_pointArray[u2].m_x;
			y2 = pDoc->m_b.m_pointArray[u3].m_y-pDoc->m_b.m_pointArray[u2].m_y;
			if(x1*y2-x2*y1<=0)//叉乘不是正，表明此外环不合法
			{
				MessageBoxA(0,"多边形B(外环)方向错误，请重新构造！","警告",MB_OK);
				pDoc->m_b.mb_clear();//清空
				if(!pDoc->m_a.m_pointArray.size()&&!pDoc->m_b.m_pointArray.size())
				{
					pDoc->mb_initData();
				}
				return false;
			}
		}

		//顺序判断所有内环是否合法（顺时针，叉乘为负，只需要判断第1、2条边即可）
		polyL = pDoc->m_b.m_regionArray[i].m_loopArray.size();
		for(int j=1; j<polyL; j++)//内环从环数组的第二个位置开始
		{
			polyP = pDoc->m_b.m_regionArray[i].m_loopArray[j].m_pointIDArray.size();
			if(polyP < 3)//顶点过少
			{
				MessageBoxA(0,"多边形B(内环)顶点过少，请重新构造！","警告",MB_OK);
				pDoc->m_b.mb_clear();//清空
				if(!pDoc->m_a.m_pointArray.size()&&!pDoc->m_b.m_pointArray.size())
				{
					pDoc->mb_initData();
				}
				return false;
			}
			else
			{
				double maxX=-9999999, x1, x2, y1, y2;//最右侧凸点的x坐标，两条边向量的坐标
				int maxID, u1, u2, u3;//最右侧凸点的ID，三个顶点在顶点数组中的ID

				for(int p=0; p<polyP; p++)//寻找最右侧凸点
				{
					int u = pDoc->m_b.m_regionArray[i].m_loopArray[j].m_pointIDArray[p];
					if(maxX < pDoc->m_b.m_pointArray[u].m_x)
					{
						maxX = pDoc->m_b.m_pointArray[u].m_x;
						maxID = p;
					}
				}
				u1 = pDoc->m_b.m_regionArray[i].m_loopArray[j].m_pointIDArray[(maxID-1+polyP)%polyP];
				u2 = pDoc->m_b.m_regionArray[i].m_loopArray[j].m_pointIDArray[maxID];
				u3 = pDoc->m_b.m_regionArray[i].m_loopArray[j].m_pointIDArray[(maxID+1)%polyP];
				x1 = pDoc->m_b.m_pointArray[u2].m_x-pDoc->m_b.m_pointArray[u1].m_x;
				y1 = pDoc->m_b.m_pointArray[u2].m_y-pDoc->m_b.m_pointArray[u1].m_y;
				x2 = pDoc->m_b.m_pointArray[u3].m_x-pDoc->m_b.m_pointArray[u2].m_x;
				y2 = pDoc->m_b.m_pointArray[u3].m_y-pDoc->m_b.m_pointArray[u2].m_y;
				if(x1*y2-x2*y1>=0)//叉乘不是负，表明此内环不合法
				{
					MessageBoxA(0,"多边形B(内环)方向错误，请重新构造！","警告",MB_OK);
					pDoc->m_b.mb_clear();//清空
					if(!pDoc->m_a.m_pointArray.size()&&!pDoc->m_b.m_pointArray.size())
					{
						pDoc->mb_initData();
					}
					return false;
				}
			}
		}
	}
	return true;
}

/*检查一个多边形中的所有边两两之间有没有除了端点外的其它交点，或者是重合边(2,5)*/
bool EdgeIntersection(CCP_PolygonPlatformDoc* pDoc)
{
	vector<Segment> Edge;//将多边形中所有边放到这个vector中，然后再双重循环检查
	int polyR, polyL, polyP;//分别表示多边形中区域、环、点数组的大小

	//检查A多边形
	//将多边形A中的每条边都加入到Edge中
	polyR = pDoc->m_a.m_regionArray.size();
	for(int i=0; i<polyR; i++)
	{
		polyL = pDoc->m_a.m_regionArray[i].m_loopArray.size();
		for(int j=0; j<polyL; j++)
		{
			polyP = pDoc->m_a.m_regionArray[i].m_loopArray[j].m_pointIDArray.size();
			for(int k=0; k<polyP; k++)
			{
				Segment seg;
				int u1,u2;//线段两个端点在端点数组中的序号
				u1 = pDoc->m_a.m_regionArray[i].m_loopArray[j].m_pointIDArray[k];
				u2 = pDoc->m_a.m_regionArray[i].m_loopArray[j].m_pointIDArray[(k+1)%polyP];
				seg.u1 = pDoc->m_a.m_pointArray[u1];
				seg.u2 = pDoc->m_a.m_pointArray[u2];
				Edge.push_back(seg);
			}
		}
	}
	//双重循环检查所有边两两之间有无端点外的其它交点或重合边
	for(int i=0; Edge.size()>0&&i<Edge.size()-1; i++)
	{
		for(int j=i+1; j<Edge.size(); j++)
		{
			Segment temp;//存储第一条边的反向边
			temp.u1 = Edge[i].u2;
			temp.u2 = Edge[i].u1;

			if(Edge[i]==Edge[j] || temp==Edge[j])//两边完全重合
			{
				MessageBoxA(0,"多边形A中有边重合，不合法，请重新构造！","警告",MB_OK);
				pDoc->m_a.mb_clear();//清空
				if(!pDoc->m_a.m_pointArray.size()&&!pDoc->m_b.m_pointArray.size())
				{
					pDoc->mb_initData();
				}
				return false;
			}
			else if(parallel(Edge[i].u1,Edge[i].u2,Edge[j].u1,Edge[j].u2) && (dots_inline(Edge[i].u1,Edge[i].u2,Edge[j].u1)||dots_inline(Edge[j].u1,Edge[j].u2,Edge[i].u1)))//部分重合
			{
				MessageBoxA(0,"多边形A中有边重合，不合法，请重新构造！","警告",MB_OK);
				pDoc->m_a.mb_clear();//清空
				if(!pDoc->m_a.m_pointArray.size()&&!pDoc->m_b.m_pointArray.size())
				{
					pDoc->mb_initData();
				}
				return false;
			}
			else if(!intersect_in(Edge[i].u1,Edge[i].u2,Edge[j].u1,Edge[j].u2))//不相交
			{
				continue;		
			}
			else if(Edge[i].u1==Edge[j].u1 || Edge[i].u1==Edge[j].u2 || Edge[i].u2==Edge[j].u1 || Edge[i].u2==Edge[j].u2)//交点是顶点
			{
				continue;
			}
			else
			{
				MessageBoxA(0,"多边形A中有边相交，不合法，请重新构造！","警告",MB_OK);
				pDoc->m_a.mb_clear();//清空
				if(!pDoc->m_a.m_pointArray.size()&&!pDoc->m_b.m_pointArray.size())
				{
					pDoc->mb_initData();
				}
				return false;
			}
		}
	}
	Edge.clear();//边结构清空
	//检查B多边形
	//将多边形B中的每条边都加入到Edge中
	polyR = pDoc->m_b.m_regionArray.size();
	for(int i=0; i<polyR; i++)
	{
		polyL = pDoc->m_b.m_regionArray[i].m_loopArray.size();
		for(int j=0; j<polyL; j++)
		{
			polyP = pDoc->m_b.m_regionArray[i].m_loopArray[j].m_pointIDArray.size();
			for(int k=0; k<polyP; k++)
			{
				Segment seg;
				int u1,u2;//线段两个端点在端点数组中的序号
				u1 = pDoc->m_b.m_regionArray[i].m_loopArray[j].m_pointIDArray[k];
				u2 = pDoc->m_b.m_regionArray[i].m_loopArray[j].m_pointIDArray[(k+1)%polyP];
				seg.u1 = pDoc->m_b.m_pointArray[u1];
				seg.u2 = pDoc->m_b.m_pointArray[u2];
				Edge.push_back(seg);
			}
		}
	}
	//双重循环检查所有边两两之间有无端点外的其它交点或重合边
	for(int i=0; Edge.size()>0&&i<Edge.size()-1; i++)
	{
		for(int j=i+1; j<Edge.size(); j++)
		{
			Segment temp;//存储第一条边的反向边
			temp.u1 = Edge[i].u2;
			temp.u2 = Edge[i].u1;

			if(Edge[i]==Edge[j] || temp==Edge[j])//两边完全重合
			{
				MessageBoxA(0,"多边形B中有边重合，不合法，请重新构造！","警告",MB_OK);
				pDoc->m_b.mb_clear();//清空
				if(!pDoc->m_a.m_pointArray.size()&&!pDoc->m_b.m_pointArray.size())
				{
					pDoc->mb_initData();
				}
				return false;
			}
			else if(parallel(Edge[i].u1,Edge[i].u2,Edge[j].u1,Edge[j].u2) && (dots_inline(Edge[i].u1,Edge[i].u2,Edge[j].u1)||dots_inline(Edge[j].u1,Edge[j].u2,Edge[i].u1)))//部分重合
			{
				MessageBoxA(0,"多边形B中有边重合，不合法，请重新构造！","警告",MB_OK);
				pDoc->m_b.mb_clear();//清空
				if(!pDoc->m_a.m_pointArray.size()&&!pDoc->m_b.m_pointArray.size())
				{
					pDoc->mb_initData();
				}
				return false;
			}
			else if(!intersect_in(Edge[i].u1,Edge[i].u2,Edge[j].u1,Edge[j].u2))//不相交
			{
				continue;		
			}
			else if(Edge[i].u1==Edge[j].u1 || Edge[i].u1==Edge[j].u2 || Edge[i].u2==Edge[j].u1 || Edge[i].u2==Edge[j].u2)//交点是顶点
			{
				continue;
			}
			else
			{
				MessageBoxA(0,"多边形B中有边相交，不合法，请重新构造！","警告",MB_OK);
				pDoc->m_b.mb_clear();//清空
				if(!pDoc->m_a.m_pointArray.size()&&!pDoc->m_b.m_pointArray.size())
				{
					pDoc->mb_initData();
				}
				return false;
			}
		}
	}
	return true;
}

/*检查多边形同一区域中的内环是否在该区域的外环中
--将该区域的外环作为一个多边形，判断该区域所有内环的所有边的中点
  是否在这个多边形内来判断这个内环是不是在外环内；*/
bool InOutLoop(CCP_PolygonPlatformDoc* pDoc)
{
	CP_Polygon temp;//将每个区域的外环作为一个单独的多边形
	int polyR, polyL, polyP;//分别表示多边形中区域、环、点数组的大小

	//判断多边形A是否合法
	polyR = pDoc->m_a.m_regionArray.size();
	for(int i=0; i<polyR; i++)//对多边形的每个区域单独检查
	{
		temp.mb_clear();
		temp.m_pointArray = pDoc->m_a.m_pointArray;
		temp.m_regionArray.push_back(pDoc->m_a.m_regionArray[i]);
		temp.m_regionArray[0].m_loopArray.clear();
		temp.m_regionArray[0].m_loopArray.push_back(pDoc->m_a.m_regionArray[i].m_loopArray[0]);//将该区域中的外环作为一个单独的多边形temp
		polyL = pDoc->m_a.m_regionArray[i].m_loopArray.size();
		for(int j=1; j<polyL; j++)//依次判断每个内环是不是在外环(多边形temp)内部
		{
			polyP = pDoc->m_a.m_regionArray[i].m_loopArray[j].m_pointIDArray.size();
			for(int k=0; k<polyP; k++)
			{
				int u1,u2;//边端点序号
				CP_Point middle;//边中点
				u1 = pDoc->m_a.m_regionArray[i].m_loopArray[j].m_pointIDArray[k];
				u2 = pDoc->m_a.m_regionArray[i].m_loopArray[j].m_pointIDArray[(k+1)%polyP];
				middle.m_x = (pDoc->m_a.m_pointArray[u1].m_x+pDoc->m_a.m_pointArray[u2].m_x)/2;
				middle.m_y = (pDoc->m_a.m_pointArray[u1].m_y+pDoc->m_a.m_pointArray[u2].m_y)/2;
				if(!PointInPoligon(middle,temp))//内环任意一条边的中点不在多边形temp（外环）内部
				{
					MessageBoxA(0,"多边形A中有内环在外环外部，请重新构造！","警告",MB_OK);
					pDoc->m_a.mb_clear();//清空
					if(!pDoc->m_a.m_pointArray.size()&&!pDoc->m_b.m_pointArray.size())
					{
						pDoc->mb_initData();
					}
					return false;
				}
			}
		}
	}

	//判断多边形B是否合法
	polyR = pDoc->m_b.m_regionArray.size();
	for(int i=0; i<polyR; i++)//对多边形的每个区域单独检查
	{
		temp.mb_clear();
		temp.m_pointArray = pDoc->m_b.m_pointArray;
		temp.m_regionArray.push_back(pDoc->m_b.m_regionArray[i]);
		temp.m_regionArray[0].m_loopArray.clear();
		temp.m_regionArray[0].m_loopArray.push_back(pDoc->m_b.m_regionArray[i].m_loopArray[0]);//将该区域中的外环作为一个单独的多边形temp
		polyL = pDoc->m_b.m_regionArray[i].m_loopArray.size();
		for(int j=1; j<polyL; j++)//依次判断每个内环是不是在外环(多边形temp)内部
		{
			polyP = pDoc->m_b.m_regionArray[i].m_loopArray[j].m_pointIDArray.size();
			for(int k=0; k<polyP; k++)
			{
				int u1,u2;//边端点序号
				CP_Point middle;//边中点
				u1 = pDoc->m_b.m_regionArray[i].m_loopArray[j].m_pointIDArray[k];
				u2 = pDoc->m_b.m_regionArray[i].m_loopArray[j].m_pointIDArray[(k+1)%polyP];
				middle.m_x = (pDoc->m_b.m_pointArray[u1].m_x+pDoc->m_b.m_pointArray[u2].m_x)/2;
				middle.m_y = (pDoc->m_b.m_pointArray[u1].m_y+pDoc->m_b.m_pointArray[u2].m_y)/2;
				if(!PointInPoligon(middle,temp))//内环任意一条边的中点不在多边形temp（外环）内部
				{
					MessageBoxA(0,"多边形B中有内环在外环外部，请重新构造！","警告",MB_OK);
					pDoc->m_b.mb_clear();//清空
					if(!pDoc->m_a.m_pointArray.size()&&!pDoc->m_b.m_pointArray.size())
					{
						pDoc->mb_initData();
					}
					return false;
				}
			}
		}
	}
	return true;
}

/*检查多边形同一个区域中的内环是否存在包含关系
--将内环作为一个单独的多边形（点顺序应该不用考虑，因为只需要判断交点个数），
  判断其它内环所有边的中点是否在这个多边形内来判断他们是否存在包含关系*/
bool InInLoop(CCP_PolygonPlatformDoc* pDoc)
{
	CP_Polygon temp;//将每个区域的每个内环作为一个单独的多边形
	int polyR, polyL, polyP;//分别表示多边形中区域、环、点数组的大小

	//判断多边形A是否合法
	polyR = pDoc->m_a.m_regionArray.size();
	for(int i=0; i<polyR; i++)//对多边形的每个区域单独检查
	{
		polyL = pDoc->m_a.m_regionArray[i].m_loopArray.size();
		for(int j=1; j<polyL; j++)//将该区域内的每个内环作为一个单独的多边形
		{
			temp.mb_clear();
			temp.m_pointArray = pDoc->m_a.m_pointArray;
			temp.m_regionArray.push_back(pDoc->m_a.m_regionArray[i]);
			temp.m_regionArray[0].m_loopArray.clear();
			temp.m_regionArray[0].m_loopArray.push_back(pDoc->m_a.m_regionArray[i].m_loopArray[j]);//将此内环作为一个单独的多边形temp
			for(int k=1; k<polyL; k++)//检测此内环是否包含其它的内环
			{
				if(j==k)//不检测和自己的包含情况
				{
					continue;
				}
				polyP = pDoc->m_a.m_regionArray[i].m_loopArray[k].m_pointIDArray.size();
				for(int p=0; p<polyP; p++)
				{
					int u1,u2;//边端点序号
					CP_Point middle;//边中点
					u1 = pDoc->m_a.m_regionArray[i].m_loopArray[k].m_pointIDArray[p];
					u2 = pDoc->m_a.m_regionArray[i].m_loopArray[k].m_pointIDArray[(p+1)%polyP];
					middle.m_x = (pDoc->m_a.m_pointArray[u1].m_x+pDoc->m_a.m_pointArray[u2].m_x)/2;
					middle.m_y = (pDoc->m_a.m_pointArray[u1].m_y+pDoc->m_a.m_pointArray[u2].m_y)/2;
					if(PointInPoligon(middle,temp))//内环任意一条边的中点在多边形temp（另一个内环）内部
					{
						MessageBoxA(0,"多边形A中有内环包含了内环，请重新构造！","警告",MB_OK);
						pDoc->m_a.mb_clear();//清空
						if(!pDoc->m_a.m_pointArray.size()&&!pDoc->m_b.m_pointArray.size())
						{
							pDoc->mb_initData();
						}
						return false;
					}
				}
			}
		}
	}

	//判断多边形B是否合法
	polyR = pDoc->m_b.m_regionArray.size();
	for(int i=0; i<polyR; i++)//对多边形的每个区域单独检查
	{
		polyL = pDoc->m_b.m_regionArray[i].m_loopArray.size();
		for(int j=1; j<polyL; j++)//将该区域内的每个内环作为一个单独的多边形
		{
			temp.mb_clear();
			temp.m_pointArray = pDoc->m_b.m_pointArray;
			temp.m_regionArray.push_back(pDoc->m_b.m_regionArray[i]);
			temp.m_regionArray[0].m_loopArray.clear();
			temp.m_regionArray[0].m_loopArray.push_back(pDoc->m_b.m_regionArray[i].m_loopArray[j]);//将此内环作为一个单独的多边形temp
			for(int k=1; k<polyL; k++)//检测此内环是否包含其它的内环
			{
				if(j==k)//不检测和自己的包含情况
				{
					continue;
				}
				polyP = pDoc->m_b.m_regionArray[i].m_loopArray[k].m_pointIDArray.size();
				for(int p=0; p<polyP; p++)
				{
					int u1,u2;//边端点序号
					CP_Point middle;//边中点
					u1 = pDoc->m_b.m_regionArray[i].m_loopArray[k].m_pointIDArray[p];
					u2 = pDoc->m_b.m_regionArray[i].m_loopArray[k].m_pointIDArray[(p+1)%polyP];
					middle.m_x = (pDoc->m_b.m_pointArray[u1].m_x+pDoc->m_b.m_pointArray[u2].m_x)/2;
					middle.m_y = (pDoc->m_b.m_pointArray[u1].m_y+pDoc->m_b.m_pointArray[u2].m_y)/2;
					if(PointInPoligon(middle,temp))//内环任意一条边的中点在多边形temp（另一个内环）内部
					{
						MessageBoxA(0,"多边形B中有内环包含了内环，请重新构造！","警告",MB_OK);
						pDoc->m_b.mb_clear();//清空
						if(!pDoc->m_a.m_pointArray.size()&&!pDoc->m_b.m_pointArray.size())
						{
							pDoc->mb_initData();
						}
						return false;
					}
				}
			}
		}
	}

	return true;
}

/*检查同一个多边形中一个区域被另一个区域完全包含的情况
--将一个区域作为一个单独的多边形，
  判断其它区域的外环的所有边的中点是否在这个多边形内来判断他们是否存在包含关系*/
bool InInRegion(CCP_PolygonPlatformDoc* pDoc)
{
	CP_Polygon temp;//将每个多边形的每个区域作为一个单独的多边形
	int polyR, polyL, polyP;//分别表示多边形中区域、环、点数组的大小

	//检查多边形A
	polyR = pDoc->m_a.m_regionArray.size();
	for(int i=0; i<polyR; i++)
	{
		temp.mb_clear();
		temp.m_pointArray = pDoc->m_a.m_pointArray;
		temp.m_regionArray.push_back(pDoc->m_a.m_regionArray[i]);//将此区域作为一个单独的多边形temp
		for(int j=0; j<polyR; j++)
		{
			if(i==j)//不检查自身和自身的包含性
			{
				continue;
			}
			polyP = pDoc->m_a.m_regionArray[j].m_loopArray[0].m_pointIDArray.size();
			for(int k=0; k<polyP; k++)
			{
				int u1,u2;//边端点序号
				CP_Point middle;//边中点
				u1 = pDoc->m_a.m_regionArray[j].m_loopArray[0].m_pointIDArray[k];
				u2 = pDoc->m_a.m_regionArray[j].m_loopArray[0].m_pointIDArray[(k+1)%polyP];
				middle.m_x = (pDoc->m_a.m_pointArray[u1].m_x+pDoc->m_a.m_pointArray[u2].m_x)/2;
				middle.m_y = (pDoc->m_a.m_pointArray[u1].m_y+pDoc->m_a.m_pointArray[u2].m_y)/2;
				if(PointInPoligon(middle,temp))//外环任意一条边的中点在多边形temp（另一个区域）内部
				{
					MessageBoxA(0,"多边形A中有区域包含了另一个区域，请重新构造！","警告",MB_OK);
					pDoc->m_a.mb_clear();//清空
					if(!pDoc->m_a.m_pointArray.size()&&!pDoc->m_b.m_pointArray.size())
					{
						pDoc->mb_initData();
					}
					return false;
				}
			}
		}
	}

	//检查多边形B
	polyR = pDoc->m_b.m_regionArray.size();
	for(int i=0; i<polyR; i++)
	{
		temp.mb_clear();
		temp.m_pointArray = pDoc->m_b.m_pointArray;
		temp.m_regionArray.push_back(pDoc->m_b.m_regionArray[i]);//将此区域作为一个单独的多边形temp
		for(int j=0; j<polyR; j++)
		{
			if(i==j)//不检查自身和自身的包含性
			{
				continue;
			}
			polyP = pDoc->m_b.m_regionArray[j].m_loopArray[0].m_pointIDArray.size();
			for(int k=0; k<polyP; k++)
			{
				int u1,u2;//边端点序号
				CP_Point middle;//边中点
				u1 = pDoc->m_b.m_regionArray[j].m_loopArray[0].m_pointIDArray[k];
				u2 = pDoc->m_b.m_regionArray[j].m_loopArray[0].m_pointIDArray[(k+1)%polyP];
				middle.m_x = (pDoc->m_b.m_pointArray[u1].m_x+pDoc->m_b.m_pointArray[u2].m_x)/2;
				middle.m_y = (pDoc->m_b.m_pointArray[u1].m_y+pDoc->m_b.m_pointArray[u2].m_y)/2;
				if(PointInPoligon(middle,temp))//外环任意一条边的中点在多边形temp（另一个区域）内部
				{
					MessageBoxA(0,"多边形B中有区域包含了另一个区域，请重新构造！","警告",MB_OK);
					pDoc->m_b.mb_clear();//清空
					if(!pDoc->m_a.m_pointArray.size()&&!pDoc->m_b.m_pointArray.size())
					{
						pDoc->mb_initData();
					}
					return false;
				}
			}
		}
	}

	return true;
}

void CCP_PolygonPlatformView::OnCheck()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();//获取Doc类指针
	if(!pDoc->m_a.m_pointArray.size() && !pDoc->m_b.m_pointArray.size())
	{
		return;
	}

	//检查各个环的方向(1)
	if(!LoopDirection(pDoc))
	{
		Invalidate(true);//重新绘制
		return;
	}
	//检查一个多边形中的所有边两两之间有没有除了端点外的其它交点，或者是重合边(2,5)
	else if(!EdgeIntersection(pDoc))
	{
		Invalidate(true);//重新绘制
		return;
	}
	//检查多边形同一区域中的内环是否在该区域的外环中
	else if(!InOutLoop(pDoc))
	{
		Invalidate(true);//重新绘制
		return;
	}
	//检查多边形同一区域中的内环是否在其它某个内环中
	else if(!InInLoop(pDoc))
	{
		Invalidate(true);//重新绘制
		return;
	}
	//检查同一个多边形中是否存在一个区域被另外一个区域完全包含的情况
	else if(!InInRegion(pDoc))
	{
		Invalidate(true);
		return;
	}

	//都合法
	MessageBox("多边形均为合法多边形，请继续其它操作","提示",MB_OK);
}
