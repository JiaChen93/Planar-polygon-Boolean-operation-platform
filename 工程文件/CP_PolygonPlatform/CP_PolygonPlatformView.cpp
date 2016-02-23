// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://go.microsoft.com/fwlink/?LinkId=238214��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// CP_PolygonPlatformView.cpp : CCP_PolygonPlatformView ���ʵ��
//

#include "stdafx.h"
#include<algorithm>
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "CP_PolygonPlatform.h"
#endif

#include "CP_PolygonPlatformDoc.h"
#include "CP_PolygonPlatformView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif





CP_Polygon tempA, tempB;//�ֱ�ΪA��B��ӽ����Ķ����
CP_Polygon AboolB;//����������
vector<Segment> SegCoinSame;//A��B�غ���ͬ���߶����飬���߶ζ˵�˳��Ϊ��A�е�˳��
vector<Segment> SegCoinReve;//A��B�غ��ҷ����߶����飬���߶ζ˵�˳��Ϊ��A�е�˳��

// CCP_PolygonPlatformView

IMPLEMENT_DYNCREATE(CCP_PolygonPlatformView, CView)

BEGIN_MESSAGE_MAP(CCP_PolygonPlatformView, CView)
	// ��׼��ӡ����
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

// CCP_PolygonPlatformView ����/����

CCP_PolygonPlatformView::CCP_PolygonPlatformView()
{
	// TODO: �ڴ˴���ӹ������

}

CCP_PolygonPlatformView::~CCP_PolygonPlatformView()
{
}

//!!
void  CCP_PolygonPlatformView::mb_statusSetText(char* s1, char* s2)
{
	CRect r;
	// View�л�ȡMainFrameָ��
	CMainFrame *pMainFrame=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	//��ȡ״̬��ָ��
	CMFCRibbonStatusBar *statusBar=
		(CMFCRibbonStatusBar *)pMainFrame->GetDescendantWindow(AFX_IDW_STATUS_BAR);
	//��ȡRibbonStatusBar�ϵ�Ԫ��
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
		} // if����
	} // ��if����
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
		} // if����
	} // ��if����
	statusBar->Invalidate( );
} // ��CCP_PolygonPlatformView�ĳ�Ա����mb_statusSetText����

//!!
BOOL CCP_PolygonPlatformView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CCP_PolygonPlatformView ����

void CCP_PolygonPlatformView::OnDraw(CDC* pDC)
{
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
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
	} // if (pDoc->m_flagShowTriangleFace)����
	if (!pDoc->m_flagShowSelect)
	{
		if (pDoc->m_flagShowA)//��ʾA
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
		} // if����
		if (pDoc->m_flagShowB)//��ʾB
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
		} // if����
		if(pDoc->m_a.m_pointArray.size()&&pDoc->m_b.m_pointArray.size()&&pDoc->m_flagShowResult)//��ʾ����������
		{
			/*��ʾ����Ķ����*/
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
	} // if(!pDoc->m_flagShowSelect)����: ��ʾ���еĻ�������
	if (pDoc->m_flagSelect) // ��ʾѡ��
	{
		switch(pDoc->m_flagSelectType)
		{
		case 1: // �㡣
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
		case 2: // ����
			if (pDoc->m_flagSelectPolygon==0&&pDoc->m_a.m_pointArray.size()!=0)
			{
				if (pDoc->m_flagSelectID==0)
					gb_drawLoop(pDC, pDoc->m_a.m_regionArray[pDoc->m_flagSelectRegion].m_loopArray[pDoc->m_flagSelectID],
					pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
					255, 100, 100,
					5); // �⻷
				else
					gb_drawLoop(pDC, pDoc->m_a.m_regionArray[pDoc->m_flagSelectRegion].m_loopArray[pDoc->m_flagSelectID],
					pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
					100, 255, 50,
					5); // �ڻ�
			} // A
			else if(pDoc->m_flagSelectPolygon==1&&pDoc->m_b.m_pointArray.size()!=0)
			{
				if (pDoc->m_flagSelectID==0)
					gb_drawLoop(pDC, pDoc->m_b.m_regionArray[pDoc->m_flagSelectRegion].m_loopArray[pDoc->m_flagSelectID],
					pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
					200, 100, 200,
					5); // �⻷
				else
					gb_drawLoop(pDC, pDoc->m_b.m_regionArray[pDoc->m_flagSelectRegion].m_loopArray[pDoc->m_flagSelectID],
					pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
					100, 50, 255,
					5); // �ڻ�
			} // B
			break;
		case 3: // ����
			int i, n;
			if (pDoc->m_flagSelectPolygon==0&&pDoc->m_a.m_pointArray.size()!=0)
			{
				n = pDoc->m_a.m_regionArray[pDoc->m_flagSelectRegion].m_loopArray.size( );
				for (i=0; i<n; i++)
					if (i==0)
						gb_drawLoop(pDC, pDoc->m_a.m_regionArray[pDoc->m_flagSelectRegion].m_loopArray[i],
						pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
						255, 100, 100,
						5); // �⻷
					else
						gb_drawLoop(pDC, pDoc->m_a.m_regionArray[pDoc->m_flagSelectRegion].m_loopArray[i],
						pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
						100, 255, 50,
						5); // �ڻ�
			} // A
			else if(pDoc->m_flagSelectPolygon==1&&pDoc->m_b.m_pointArray.size()!=0)
			{
				n = pDoc->m_b.m_regionArray[pDoc->m_flagSelectRegion].m_loopArray.size( );
				for (i=0; i<n; i++)
					if (i==0)
						gb_drawLoop(pDC, pDoc->m_b.m_regionArray[pDoc->m_flagSelectRegion].m_loopArray[i],
						pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
						200, 100, 200,
						5); // �⻷
					else
						gb_drawLoop(pDC, pDoc->m_b.m_regionArray[pDoc->m_flagSelectRegion].m_loopArray[i],
						pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
						100, 50, 255,
						5); // �ڻ�
			} // B
			break;
		case 4: // ����Ρ�
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
		case 5: // �ʷ������Ρ�
			break;
		} // switch����
	} // if(pDoc->m_flagSelect)����
	if (pDoc->m_flagAdd == 1) // �⻷
	{
		gb_drawPointArrayLine(pDC, pDoc->m_flagAddPointArray,
			pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
			200, 0, 255,
			5);
		gb_drawPointArrayPoint(pDC, pDoc->m_flagAddPointArray,
			pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
			0, 0, 0,
			16);
	} // if (pDoc->m_flagAdd == 1)����
	if (pDoc->m_flagAdd == 2) // �ڻ�
	{
		gb_drawPointArrayLine(pDC, pDoc->m_flagAddPointArray,
			pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
			0, 200, 255,
			5);
		gb_drawPointArrayPoint(pDC, pDoc->m_flagAddPointArray,
			pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
			0, 0, 0,
			16);
	} // if (pDoc->m_flagAdd == 2)����
}

/*����AboolB�еı�*/
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
	} //for����
	pDC->SelectObject(penOld);
} // ����gb_drawPolygonLoop����

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
	} //for����
	pDC->SelectObject(penOld);
} // ����gb_drawPointArrayLine����

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
	} // for����
} // ����gb_drawPointArrayPoint����

//!!
void gb_drawPointGlobal(CDC* pDC, CP_Point pointGlobal, double scale, CP_Point translation, int screenX, int screenY, int r, int g, int b, int size)
{
	CP_Point ps;
	gb_pointConvertFromGlobalToScreen(ps, pointGlobal, scale, translation,screenX, screenY);
	gb_drawPointScreen(pDC, (int)(ps.m_x+0.5), (int)(ps.m_y+0.5), r, g, b, size);
} // ����gb_drawPointScreen����

//!!
void gb_drawPointScreen(CDC* pDC, int x, int y, int r, int g, int b, int size)
{
	CBrush brush(RGB(r, g, b));
	CBrush* brushOld = (CBrush*)pDC->SelectObject(&brush);;
	// ���Ƹ���
	CRect rect(x - size, y - size, x + size, y + size);
	pDC->Ellipse(&rect);
	pDC->SelectObject(brushOld);
} // ����gb_drawPointScreen����

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
		} // �ڲ�for����
	} // �ⲿfor����
} // ����gb_drawPolygonLoop����

//!!
void gb_drawPolygonPoint(CDC* pDC, CP_Polygon& p, double scale, CP_Point translation, int screenX, int screenY, int r, int g, int b, int size)
{
	int n = p.m_pointArray.size( );
	int i;
	for (i=0; i<n; i++)
	{
		gb_drawPointGlobal(pDC, p.m_pointArray[i], scale, translation, screenX, screenY, r, g, b, size);
	} // for����
} // ����gb_drawPolygonPoint����



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
			} // for(iLv)����
		} // for(iL)����
	} // for(ir)����
	pDC->SetTextColor(cOld);
} // ����gb_drawPolygonPointID����

/*�����߶γ��ȵ�ƽ��*/
double seglength(CP_Point p1, CP_Point p2)
{
	return pow((p1.m_x-p2.m_x),2)+pow((p1.m_y-p2.m_y),2);
}

/*���㽻��˻�(P1-P0)x(P2-P0)*/
double xmult(CP_Point p1, CP_Point p2, CP_Point p0)
{
	return (p1.m_x - p0.m_x)*(p2.m_y - p0.m_y) - (p2.m_x - p0.m_x)*(p1.m_y - p0.m_y);
}

/*�е��Ƿ����߶���,�����˵�*/
int dot_online_in(CP_Point p, CP_Point l1, CP_Point l2)
{
	return zero(xmult(p, l1, l2)) 
		&& (l1.m_x - p.m_x)*(l2.m_x - p.m_x)<eps 
		&& (l1.m_y - p.m_y)*(l2.m_y - p.m_y)<eps;
}

/*���������߶�ͬ��,�����߶��Ϸ���0*/
int same_side(CP_Point p1, CP_Point p2, CP_Point l1, CP_Point l2)
{
	return xmult(l1, p1, l2)*xmult(l1, p2, l2)>eps;
}

/*����ֱ��ƽ��*/
int parallel(CP_Point u1, CP_Point u2, CP_Point v1, CP_Point v2)
{
	double temp = (u1.m_x - u2.m_x)*(v1.m_y - v2.m_y) - (v1.m_x - v2.m_x)*(u1.m_y - u2.m_y);
	return zero(temp);
}

/*�����㹲��*/
int dots_inline(CP_Point p1, CP_Point p2, CP_Point p3)
{
	return zero(xmult(p1, p2, p3));
}

/*�����߶��ཻ,�����˵�Ͳ����غ�*/
int intersect_in(CP_Point u1, CP_Point u2, CP_Point v1, CP_Point v2)
{
	if (!dots_inline(u1, u2, v1) || !dots_inline(u1, u2, v2))
	{
		return !same_side(u1, u2, v1, v2) && !same_side(v1, v2, u1, u2);
	}	
	return dot_online_in(u1, v1, v2) || dot_online_in(u2, v1, v2) || dot_online_in(v1, u1, u2) || dot_online_in(v2, u1, u2);
}

/*�������߶ν���,�������߶��Ƿ��ཻ(ͬʱ����Ҫ�ж��Ƿ�ƽ��!)*/
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
/*���㡢��ӽ��㣬�õ�tempA��tempB*/
void AddNodes(CCP_PolygonPlatformDoc* pDoc)
{
	Segment temp;//��ʱ�߶νṹ����

	/*��A��B����θ�����ʱ����*/
	tempA = pDoc->m_a;
	tempB = pDoc->m_b;
	SegCoinSame.clear();//����غ��߶�����
	SegCoinReve.clear();

	/*ѭ��A��B��ÿ���ߣ����㽻��*/
	CP_Point u1, u2, v1, v2, node;//�����߶εĶ˵�����ǵĽ���
	int ARs, ALs,APs, BRs, BLs, BPs;//�ֱ��ʾA��B�����򡢻��͵�����Ĵ�С

	ARs=tempA.m_regionArray.size();
	for(int iA=0; iA<ARs; iA++)//Aÿ������
	{
		ALs = tempA.m_regionArray[iA].m_loopArray.size();
		for(int jA=0; jA<ALs; jA++)//Aÿ����
		{
			APs = tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray.size();
			for(int kA=0; kA<APs; kA++)//Aÿ���㣨��ţ�
			{
				APs = tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray.size();
				/*���߶ζ˵�������㸳����ֵ*/
				u1 = tempA.m_pointArray[tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray[kA]];
				u2 = tempA.m_pointArray[tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray[(kA+1)%APs]];

				/*ƥ��B�е�ÿ���߶�*/
				BRs = tempB.m_regionArray.size();
				for(int iB=0; iB<BRs; iB++)//Bÿ������
				{
					BLs = tempB.m_regionArray[iB].m_loopArray.size();
					for(int jB=0; jB<BLs; jB++)//Bÿ����
					{
						BPs = tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray.size();
						for(int kB=0; kB<BPs; kB++)//Bÿ����
						{
							BPs = tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray.size();
							/*���߶ζ˵�������㸳����ֵ*/
							v1 = tempB.m_pointArray[tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray[kB]];
							v2 = tempB.m_pointArray[tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray[(kB+1)%BPs]];

							/*�ж��ཻ/�غϣ��󽻵�*/
							if(zero(u1.m_x-v1.m_x)&&zero(u1.m_y-v1.m_y) && zero(u2.m_x-v2.m_x)&&zero(u2.m_y-v2.m_y))//������ȫ�غ���ͬ��
							{
								temp.u1 = u1;
								temp.u2 = u2;
								SegCoinSame.push_back(temp);//����ͬ��������(˳��Ϊ�߶���A�е�˳��)
							}
							else if(zero(u1.m_x-v2.m_x)&&zero(u1.m_y-v2.m_y) && zero(u2.m_x-v1.m_x)&&zero(u2.m_y-v1.m_y))//������ȫ�غ��ҷ���
							{
								temp.u1 = u1;
								temp.u2 = u2;
								SegCoinReve.push_back(temp);//���뷴��������(˳��Ϊ�߶���A�е�˳��)
							}
							else if (parallel(u1, u2, v1, v2)&&!dots_inline(u1,u2,v1) || !intersect_in(u1, u2, v1, v2))//���ཻ
							{//�޽���
							}
							else if (parallel(u1, u2, v1, v2) && dots_inline(u1, u2, v1))//�غ�
							{//��Ϊ�����غϺ���ȫ�غ�
								
								//��������ȫ�غ��е��������
								//else if ((pow(v1.m_x - v2.m_x, 2) + pow(v1.m_y - v2.m_y, 2)) - (pow(u1.m_x - u2.m_x, 2) + pow(u1.m_y - u2.m_y, 2))>eps)
								if(between(u1.m_x,v1.m_x,v2.m_x) && between(u2.m_x,v1.m_x,v2.m_x))	
								{//A���߶ν϶�,��B���߶�����ӵ�
									/*�ڶ���ε����꼯���в����*/
									if(seglength(v1,u1)-seglength(v1,u2)<-eps)//u1��ǰ��ABͬ��
									{   /*�Ȳ���u1,�ٲ���u2*/
										tempB.m_pointArray.push_back(u1);
										tempB.m_pointArray.push_back(u2);
										temp.u1 = u1;
										temp.u2 = u2;
										SegCoinSame.push_back(temp);//����ͬ��������(˳��Ϊ�߶���A�е�˳��)
									}
									else//u2��ǰ,AB����
									{   /*�Ȳ���u2,�ٲ���u1*/
										tempB.m_pointArray.push_back(u2);
										tempB.m_pointArray.push_back(u1);	
										temp.u1 = u1;
										temp.u2 = u2;
										SegCoinReve.push_back(temp);//���뷴��������(˳��Ϊ�߶���A�е�˳��)
									}
									/*�ڶ���εĻ��еĵ����кż����в��������*/
									tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray.insert(tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray.begin()+kB+1,tempB.m_pointArray.size()-2);
									tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray.insert(tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray.begin()+kB+2,tempB.m_pointArray.size()-1);
									kB += 2;//����������㣬��������ĵ�
								}
								else if(between(v1.m_x,u1.m_x,u2.m_x) && between(v2.m_x,u1.m_x,u2.m_x))
								{//B���߶ν϶�,��A���߶�����ӵ�
									/*�ڶ���ε����꼯���в����*/
									if(seglength(u1,v1)-seglength(u1,v2)<-eps)//v1��ǰ��ABͬ��
									{   /*�Ȳ���v1,�ٲ���v2*/
										tempA.m_pointArray.push_back(v1);
										tempA.m_pointArray.push_back(v2);
										temp.u1 = v1;
										temp.u2 = v2;
										SegCoinSame.push_back(temp);//����ͬ��������(˳��Ϊ�߶���A�е�˳��)
									}
									else//v2��ǰ,AB����
									{   /*�Ȳ���v2,�ٲ���v1*/
										tempA.m_pointArray.push_back(v2);
										tempA.m_pointArray.push_back(v1);		
										temp.u1 = v2;
										temp.u2 = v1;
										SegCoinReve.push_back(temp);//���뷴��������(˳��Ϊ�߶���A�е�˳��)
									}
									/*�ڶ���εĻ��еĵ����кż����в��������*/
									tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray.insert(tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray.begin()+kA+1,tempA.m_pointArray.size()-2);
									tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray.insert(tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray.begin()+kA+2,tempA.m_pointArray.size()-1);
									//���������,�������¸��߶εĵڶ����㸳ֵ
									u2 = tempA.m_pointArray[tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray[(kA+1)%APs]];
								}
								else if(!between(v1.m_x,u1.m_x,u2.m_x) && !between(v2.m_x,u1.m_x,u2.m_x)&& !between(v1.m_y,u1.m_y,u2.m_y) && !between(v2.m_y,u1.m_y,u2.m_y)
									&& !between(u1.m_x,v1.m_x,v2.m_x) && !between(u2.m_x,v1.m_x,v2.m_x)&& !between(u1.m_y,v1.m_y,v2.m_y) && !between(u2.m_y,v1.m_y,v2.m_y))
								{
									continue;
								}
								else//�����غ�
								{
									if((u1.m_x-v1.m_x)*(u2.m_x-v2.m_x)+(u1.m_y-v1.m_y)*(u2.m_y-v2.m_y) >= eps)//����ͬ��
									{
;										if((v1.m_x-u1.m_x)*(v1.m_x-u2.m_x)+(v1.m_y-u1.m_y)*(v1.m_y-u2.m_y) < -eps)//�Ų���u1->v1->u2->v2
										{//��A�в���v1����B�в���u2����v1->u2����ͬ���غϱ߼���
											tempA.m_pointArray.push_back(v1);
											tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray.insert(tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray.begin()+kA+1,tempA.m_pointArray.size()-1);
											tempB.m_pointArray.push_back(u2);
											tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray.insert(tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray.begin()+kB+1,tempB.m_pointArray.size()-1);
											temp.u1 = v1;
											temp.u2 = u2;
											//SegCoinSame.push_back(temp);
											kB += 1;//�����һ���㣬��������ĵ�
											//MessageBox( NULL , TEXT("���ǶԻ���1") , TEXT("���") , MB_ICONINFORMATION|MB_YESNO);
										}
										else//�Ų���v1->u1->v2->u2
										{//��A�в���v2����B�в���u1����u1->v2����ͬ����s�ϱ߼���
											tempA.m_pointArray.push_back(v2);
											tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray.insert(tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray.begin()+kA+1,tempA.m_pointArray.size()-1);
											tempB.m_pointArray.push_back(u1);
											tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray.insert(tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray.begin()+kB+1,tempB.m_pointArray.size()-1);
											temp.u1 = u1;
											temp.u2 = v2;
											//SegCoinSame.push_back(temp);
											kB += 1;//�����һ���㣬��������ĵ�
											//MessageBox( NULL , TEXT("���ǶԻ���2") , TEXT("���") , MB_ICONINFORMATION|MB_YESNO);
										}
									}
									else//���߷���
									{
										if((v2.m_x-u1.m_x)*(v2.m_x-u2.m_x)+(v2.m_y-u1.m_y)*(v2.m_y-u2.m_y) < -eps)//�Ų���u1->v2->u2->v1
										{//��A�в���v2����B�в���u2����v2->u2���뷴���غϱ߼���
											tempA.m_pointArray.push_back(v2);
											tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray.insert(tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray.begin()+kA+1,tempA.m_pointArray.size()-1);
											tempB.m_pointArray.push_back(u2);
											tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray.insert(tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray.begin()+kB+1,tempB.m_pointArray.size()-1);
											temp.u1 = v2;
											temp.u2 = u2;
											//SegCoinReve.push_back(temp);
											kB += 1;//�����һ���㣬��������ĵ�
											//MessageBox( NULL , TEXT("���ǶԻ���3") , TEXT("���") , MB_ICONINFORMATION|MB_YESNO);
										}
										else//�Ų���v2->u1->v1->u2
										{//��A�в���v1����B�в���u1����u1->v1���뷴���غϱ߼���
											tempA.m_pointArray.push_back(v1);
											tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray.insert(tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray.begin()+kA+1,tempA.m_pointArray.size()-1);
											tempB.m_pointArray.push_back(u1);
											tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray.insert(tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray.begin()+kB+1,tempB.m_pointArray.size()-1);
											temp.u1 = u1;
											temp.u2 = v1;
											//SegCoinReve.push_back(temp);
											kB += 1;//�����һ���㣬��������ĵ�
											//MessageBox( NULL , TEXT("���ǶԻ���4") , TEXT("���") , MB_ICONINFORMATION|MB_YESNO);
										}
									}
									//���������,�������¸��߶εĵڶ����㸳ֵ
									u2 = tempA.m_pointArray[tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray[(kA+1)%APs]];
								}
							}
							else//��ͨ����
							{
								node= LineSegInter(u1, u2, v1, v2);//�õ�����
								if(zero(node.m_x-u1.m_x)&&zero(node.m_y-u1.m_y) || zero(node.m_x-u2.m_x)&&zero(node.m_y-u2.m_y) 
									|| zero(node.m_x-v1.m_x)&&zero(node.m_y-v1.m_y) || zero(node.m_x-v2.m_x)&&zero(node.m_y-v2.m_y))//������ĳ���߶εĶ˵�
								{//ֱ�������˽���
									continue;
								}
								/*��������ӵ�A��*/
								tempA.m_pointArray.push_back(node);
								tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray.insert(tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray.begin()+kA+1,tempA.m_pointArray.size()-1);
								/*��������ӵ�B��*/
								tempB.m_pointArray.push_back(node);
								tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray.insert(tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray.begin()+kB+1,tempB.m_pointArray.size()-1);
								kB += 1;//�����һ���㣬��������ĵ�
							}
							APs = tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray.size();
							BPs = tempB.m_regionArray[iB].m_loopArray[jB].m_pointIDArray.size();
							u1 = tempA.m_pointArray[tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray[kA]];
							u2 = tempA.m_pointArray[tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray[(kA+1)%APs]];
						}//for����
					}//for����
				}//for����
				APs = tempA.m_regionArray[iA].m_loopArray[jA].m_pointIDArray.size();
			}//for����
		}//for����
	}//for�������������н������
}

/*�жϵ�p�Ƿ��ڶ����poly��*/
bool PointInPoligon(CP_Point p, CP_Polygon poly)
{
	int count = 0;
	CP_Point infPoint;//����Զ��
	infPoint.m_x = INFINITY;
	infPoint.m_y = p.m_y;//������α����L������һ���˵�
	
	/*��������ε�ÿһ����*/
	int polyR, polyL, polyP;//�ֱ��ʾ����������򡢻���������Ĵ�С
	polyR = poly.m_regionArray.size();
	for(int i=0; i<polyR; i++)//ÿһ������
	{
		polyL = poly.m_regionArray[i].m_loopArray.size();
		for(int j=0; j<polyL; j++)//ÿһ����
		{
			polyP = poly.m_regionArray[i].m_loopArray[j].m_pointIDArray.size();
			for(int k=0; k<polyP; k++)//ÿһ������Ϊ�����߶�
			{
				int u1, u2;//�����˵��ڶ���ε����꼯���е����
				u1 = poly.m_regionArray[i].m_loopArray[j].m_pointIDArray[k];
				u2 = poly.m_regionArray[i].m_loopArray[j].m_pointIDArray[(k+1)%polyP];
				if(dot_online_in(p,poly.m_pointArray[u1],poly.m_pointArray[u2]))//��P�ڱ���
				{
					return true;
				}
				if(!zero(poly.m_pointArray[u1].m_y-poly.m_pointArray[u2].m_y))//�߲���ˮƽ��
				{
					if(dot_online_in(poly.m_pointArray[u1].m_y>poly.m_pointArray[u2].m_y?poly.m_pointArray[u1]:poly.m_pointArray[u2],infPoint,p))//����������ϴ�Ķ˵���L��
					{
						count++;
					}
					else if(dot_online_in(poly.m_pointArray[u1].m_y>poly.m_pointArray[u2].m_y?poly.m_pointArray[u2]:poly.m_pointArray[u1],infPoint,p))//�����������С�Ķ˵���L��
					{
						//����
					}
					else if(intersect_in(poly.m_pointArray[u1],poly.m_pointArray[u2],infPoint,p))//����L�ཻ
					{
						count++;
					}
	
				}
			}
		}
	}
	/*��ȡ���ս��*/
	if(count%2==0)//ż�������ڶ������
	{
		return false;
	}
	else//�������ڶ������
	{
		return true;
	}
}

/*�ж��߶�u1u2�Ƿ��ڶ������*/
bool SegInPolygon(CP_Point u1, CP_Point u2, CP_Polygon poly)
{
	CP_Point midpoint;//�߶��е�

	midpoint.m_x = (u1.m_x+u2.m_x)/2.0;
	midpoint.m_y = (u1.m_y+u2.m_y)/2.0;
	/*����е��ڶ�����ڣ����߶��ڶ������*/
	if(PointInPoligon(midpoint,poly))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*����������*/
bool ascending(const Segment& e1, const Segment& e2)
{
	return e1.u1.m_x<e2.u1.m_x;
	//return e1.u1.m_x<e2.u1.m_x || (zero(e1.u1.m_x-e2.u1.m_x)&&(e1.u1.m_y<e2.u1.m_y));
}

/*�����������û�������С�Ļ������У�Ϊ�˱����俴��������һ���߶����ǻ�������ɾ��*/
void PolygonExam()
{
	int polyR, polyL, polyP;
	double xmin=99999, xmax=-99999, ymin=99999, ymax=-99999;

	polyR = AboolB.m_regionArray.size();
	for(int i=0; i<polyR; i++)//ѭ������ÿ������
	{
		polyL = AboolB.m_regionArray[i].m_loopArray.size();
		for(int j=0; j<polyL; j++)//ѭ������ÿ����
		{
			polyP = AboolB.m_regionArray[i].m_loopArray[j].m_pointIDArray.size();
			xmin = ymin = 99999;
			xmax = ymax = -99999;
			for(int k=0; k<polyP; k++)//ѭ������ÿ����
			{
				int u;//�߶˵�����
				u = AboolB.m_regionArray[i].m_loopArray[j].m_pointIDArray[k];
				xmin = AboolB.m_pointArray[u].m_x<xmin?AboolB.m_pointArray[u].m_x:xmin;
				xmax = AboolB.m_pointArray[u].m_x>xmax?AboolB.m_pointArray[u].m_x:xmax;
				ymin = AboolB.m_pointArray[u].m_y<ymin?AboolB.m_pointArray[u].m_y:ymin;
				ymax = AboolB.m_pointArray[u].m_y>ymax?AboolB.m_pointArray[u].m_y:ymax;
			}
			/*�������С��ɾ���˻�*/
			if(xmax-xmin<2 || ymax-ymin<2)
			{
				for(int k=0; k<polyP; k++)//��ɾ�������϶��㣨������ת���ɵ�һ����������꣩
				{
					int u;//�߶˵����
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

/*���߼��Ϲ����ɶ����*/
void ConstructPolygon(CCP_PolygonPlatformDoc* pDoc)
{
	CP_Point Pmin;//��ǰ���������˵�
	Pmin.m_x = 1.79e-308;
	double Xmax=-1000000,Ymax=-1000000,Ymin=1000000;//��ǰ����x,y�����ֵ��Сֵ����ʼΪdouble�͵���Сֵ/���ֵ
	CP_Point PNow;//��ǰ�Ķ��㣨Ѱ����һ����ӵ��߶ε�ʱ���õ���

	AboolB.mb_clear();//��ղ���
	int RID=-1, LID=-1;//�����������ĸ�������ǰ�����л��ĸ���
	CP_Region tempR;
	CP_Loop tempL;

	while(pDoc->ABEdge.size()!=0)
	{
		/*�е�ǰ�߶����ڻ������µ�������⻷*/
		//if(pDoc->ABEdge[0].u1.m_x-Xmax>eps || pDoc->ABEdge[0].u1.m_y-Ymax>eps || Ymin-pDoc->ABEdge[0].u1.m_y>eps)//��ǰ��һ��Ԫ�صĵ��������Xmax��Ymax��˵����һ�����µ������еġ��⻷���еġ���һ���߶Ρ�
		if(RID==-1 || !PointInPoligon(pDoc->ABEdge[1].u1,AboolB))
		{
			RID++;
			LID = 0;
			/*�������*/
			AboolB.m_regionArray.push_back(tempR);//�򲼶����������������
			AboolB.m_regionArray[RID].m_regionIDinPolygon = RID;//����ı��
			AboolB.m_regionArray[RID].m_polygon = &AboolB;//��������AboolB��������
			/*��ӻ�*/
			AboolB.m_regionArray[RID].m_loopArray.push_back(tempL);//�򲼶�����������ӻ�
			AboolB.m_regionArray[RID].m_loopArray[LID].m_loopIDinRegion = LID;//���ı��
			AboolB.m_regionArray[RID].m_loopArray[LID].m_polygon = &AboolB;//������A��B��������
			AboolB.m_regionArray[RID].m_loopArray[LID].m_regionIDinPolygon = RID;//�����ڵڼ�������
			/*�ڶ��������ӵ�*/
			AboolB.m_pointArray.push_back(pDoc->ABEdge[0].u1);
			AboolB.m_pointArray.push_back(pDoc->ABEdge[0].u2);
			AboolB.m_regionArray[RID].m_loopArray[LID].m_pointIDArray.push_back(AboolB.m_pointArray.size()-2);
			AboolB.m_regionArray[RID].m_loopArray[LID].m_pointIDArray.push_back(AboolB.m_pointArray.size()-1);
			/*����Xmin��Xmax*/
			Pmin.m_x = pDoc->ABEdge[0].u1.m_x;//������㣬�жϻ��պ�ʱҪ�õ�
			Pmin.m_y = pDoc->ABEdge[0].u1.m_y;
			Xmax = pDoc->ABEdge[0].u2.m_x;
			Ymax = pDoc->ABEdge[0].u1.m_y>pDoc->ABEdge[0].u2.m_y?pDoc->ABEdge[0].u1.m_y:pDoc->ABEdge[0].u2.m_y;
			Ymin = pDoc->ABEdge[0].u1.m_y>pDoc->ABEdge[0].u2.m_y?pDoc->ABEdge[0].u2.m_y:pDoc->ABEdge[0].u1.m_y;//���������棬�ж��ڻ�/�⻷��ʱ��Ҫ�õ�������ǵ�һ���ߵ�β�˵㣩
			PNow = pDoc->ABEdge[0].u2;//��ǰ�Ķ����������߶ε�β�˵㣬���ô˶���Ѱ����һ����
			/*���ñߴ�ABEdge��ɾ��*/
			vector<Segment>::iterator it = pDoc->ABEdge.begin();
			pDoc->ABEdge.erase(it);
		}
		else//��һ��Ԫ�ص�����С��Xmax��˵���ǡ���ǰ���򡿵�һ�����ڻ���
		{
			LID++;
			/*��ӻ�*/
			AboolB.m_regionArray[RID].m_loopArray.push_back(tempL);//�򲼶�����������ӻ�
			AboolB.m_regionArray[RID].m_loopArray[LID].m_loopIDinRegion = LID;//���ı��
			AboolB.m_regionArray[RID].m_loopArray[LID].m_polygon = &AboolB;//������A��B��������
			AboolB.m_regionArray[RID].m_loopArray[LID].m_regionIDinPolygon = RID;//�����ڵڼ�������
			/*�ڶ��������ӵ�*/
			AboolB.m_pointArray.push_back(pDoc->ABEdge[0].u1);
			AboolB.m_pointArray.push_back(pDoc->ABEdge[0].u2);
			AboolB.m_regionArray[RID].m_loopArray[LID].m_pointIDArray.push_back(AboolB.m_pointArray.size()-2);
			AboolB.m_regionArray[RID].m_loopArray[LID].m_pointIDArray.push_back(AboolB.m_pointArray.size()-1);
			/*ֻ��Ҫ����Xmin*/
			Pmin.m_x = pDoc->ABEdge[0].u1.m_x;//������㣬�жϻ��պ�ʱҪ�õ�
			Pmin.m_y = pDoc->ABEdge[0].u1.m_y;
			PNow = pDoc->ABEdge[0].u2;//��ǰ�Ķ����������߶ε�β�˵㣬���ô˶���Ѱ����һ����
			/*���ñߴ�ABEdge��ɾ��*/
			vector<Segment>::iterator it = pDoc->ABEdge.begin();
			pDoc->ABEdge.erase(it);
		}

		/*Ѱ�ҵ�ǰ���е������߶�*/
		while(1)
		{
			/*����������һ���߶Σ��߶ε�������һ���߶ε�β���غϣ�*/
			vector<Segment>::iterator it;
			for(it=pDoc->ABEdge.begin(); it!=pDoc->ABEdge.end(); it++)
			{
				if(zero(it->u1.m_x-PNow.m_x)&&zero(it->u1.m_y-PNow.m_y))
				{
					break;//�ҵ����˳�
				}
			}

			/*��������߶ε��յ��Ǹû�����㣬˵��������Ѿ���ɣ��˳�whileѭ��*/
			if(zero(it->u2.m_x-Pmin.m_x)&&zero(it->u2.m_y-Pmin.m_y))
			{
				/*���ñߴ�ABEdge��ɾ��*/
				pDoc->ABEdge.erase(it);
				break;
			}
			/*��������߶ε��յ㲻�Ǹû�����㣬˵���������û������ڣ�����ѭ��*/
			/*���������ӵ�*/
			AboolB.m_pointArray.push_back(it->u2);//ֻ��Ҫ����߶�β�˵㣬��Ϊͷ�˵�Ҳ������һ���߶��е�β�˵㣬�Ѿ����
			AboolB.m_regionArray[RID].m_loopArray[LID].m_pointIDArray.push_back(AboolB.m_pointArray.size()-1);
			PNow = it->u2;
			Xmax = Xmax>it->u2.m_x?Xmax:it->u2.m_x;//�����⻷���������
			Ymax = Ymax>it->u2.m_y?Ymax:it->u2.m_y;
			Ymin = Ymin<it->u2.m_y?Ymin:it->u2.m_y;
			/*���ñߴ�ABEdge��ɾ��*/
			pDoc->ABEdge.erase(it);
		}
	}
	/*�����������û�������С�Ļ�������У�Ϊ�˱����俴������һ���ߣ�ɾȥ�˻�*/
	PolygonExam();
}

// CCP_PolygonPlatformView ��ӡ
void CCP_PolygonPlatformView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CCP_PolygonPlatformView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CCP_PolygonPlatformView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CCP_PolygonPlatformView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
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
        mb_statusSetText("����Ӳ���������", "�������������");
        return;
    } // if����
	    if ((pDoc->m_flagAdd == 1) || (pDoc->m_flagAdd == 2)) // ��
    {
        int n = pDoc->m_flagAddPointArray.size( );
        if (n<=0)
        {
            pDoc->m_flagAdd = 0;
            mb_statusSetText("����Ӳ���������", "�������������");
            return;
        } // if����
		       if (n<3)
        {
            if (MessageBox("���ɻ��ĵ㲻������3�����Ƿ��������? ѡ����������������ǰ������ĵ㡣"
                      , "����ȷ��"
                      , MB_ICONQUESTION|MB_YESNO)==IDYES)
                return;
            if (pDoc->m_flagAdd == 1)
                 mb_statusSetText("�⻷����Ӳ���������", "�������������");
            else mb_statusSetText("�ڻ�����Ӳ���������", "�������������");
            pDoc->m_flagAdd = 0;
            pDoc->m_flagAddPointArray.clear( );
            return;
        } // if����
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
        } // for����
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
        } // if/else����
			           pn->m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray[pDoc->m_flagAddIDLoop].m_polygon = pn;

        pn->m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray[pDoc->m_flagAddIDLoop].m_regionIDinPolygon = pDoc->m_flagAddIDRegion;

        pn->m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray[pDoc->m_flagAddIDLoop].m_loopIDinRegion = pDoc->m_flagAddIDLoop;

        pn->m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray[pDoc->m_flagAddIDLoop].m_pointIDArray.resize(n);

        for (i=0, k= nv; i<n; i++, k++)
        {
            pn->m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray[pDoc->m_flagAddIDLoop].m_pointIDArray[i] = k;
        } // for����

		       if (pDoc->m_flagAdd == 1)
             mb_statusSetText("�⻷����Ӳ���������", "�������������");
        else mb_statusSetText("�ڻ�����Ӳ���������", "�������������");
        pDoc->m_flagAdd = 0;
        pDoc->m_flagAddPointArray.clear( );
        Invalidate( );
        return;
    } // ��if����
}

void CCP_PolygonPlatformView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CCP_PolygonPlatformView ���

#ifdef _DEBUG
void CCP_PolygonPlatformView::AssertValid() const
{
	CView::AssertValid();
}

void CCP_PolygonPlatformView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCP_PolygonPlatformDoc* CCP_PolygonPlatformView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCP_PolygonPlatformDoc)));
	return (CCP_PolygonPlatformDoc*)m_pDocument;
}
#endif //_DEBUG


// CCP_PolygonPlatformView ��Ϣ�������


void CCP_PolygonPlatformView::OnUpdateComboAorb(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
    CMFCRibbonBar* robbon_bar = ((CFrameWndEx*)AfxGetMainWnd())->GetRibbonBar();
    if (robbon_bar==NULL)
        return;
    CMFCRibbonComboBox* pbox = (CMFCRibbonComboBox*)robbon_bar->FindByID(ID_COMBO_AorB); // ��ȡ�༭����
    if (pbox==NULL)
        return;
    pbox->AddItem("�����A");
    pbox->AddItem("�����B");

}


void CCP_PolygonPlatformView::OnComboAorb()
{
	// TODO: �ڴ���������������
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    CMFCRibbonBar* robbon_bar = ((CFrameWndEx*)AfxGetMainWnd())->GetRibbonBar();
    if (robbon_bar==NULL)
        return;
    CMFCRibbonComboBox* pbox = (CMFCRibbonComboBox*)robbon_bar->FindByID(ID_COMBO_AorB); // ��ȡ�༭����
	    if (pbox==NULL)
        return;
    pbox->AddItem("�����A");
    pbox->AddItem("�����B");
    int i = pbox->GetCurSel( );
    pbox->SelectItem(i);
    if (i==0)
        pDoc->m_flagBuildA = true;
    else pDoc->m_flagBuildA = false;
    Invalidate( );

}


void CCP_PolygonPlatformView::OnEdgeNumber()
{
	// TODO: �ڴ���������������
	
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    CString string;
    CMFCRibbonBar* robbon_bar = ((CFrameWndEx*)AfxGetMainWnd())->GetRibbonBar(); //��ȡRibbon bar ���
    if (robbon_bar==NULL)
        return;
	    CMFCRibbonEdit* slider = (CMFCRibbonEdit*)robbon_bar->FindByID(ID_EDGE_NUMBER); // ��ȡ�༭����
    if (slider==NULL)
        return;
    string= slider->GetEditText(); // ��ȡ����
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
	// TODO: �ڴ���������������
	 CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    CString string;
    CMFCRibbonBar* robbon_bar = ((CFrameWndEx*)AfxGetMainWnd())->GetRibbonBar(); //��ȡRibbon bar ���
    if (robbon_bar==NULL)
        return;

     CMFCRibbonEdit* slider = (CMFCRibbonEdit*)robbon_bar->FindByID(ID_TOLERANCE); // ��ȡ�༭����
    if (slider==NULL)
        return;
    string= slider->GetEditText(); // ��ȡ����
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
	// TODO: �ڴ���������������
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
    sprintf_s(s, 100, "���⻷����%1d����", pDoc->m_edgeNumber);
    if (pDoc->m_flagBuildA)
         mb_statusSetText("�ڶ����A�д��������⻷��", s);
    else mb_statusSetText("�ڶ����B�д��������⻷��", s);

}


void CCP_PolygonPlatformView::OnViewStandard()
{
	// TODO: �ڴ���������������
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    pDoc->m_scale = 1.0; // ���ű���
    pDoc->m_translation.m_x = 0.0; // ����ƽ����
    pDoc->m_translation.m_y = 0.0; // ����ƽ����
    Invalidate( );
    mb_statusSetText("��׼������ϵ��", "��ƽ�ƣ�Ҳ�����š�");
}


void CCP_PolygonPlatformView::OnViewFit()
{
	// TODO: �ڴ���������������
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
    } // for����
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
    } // for����
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
    mb_statusSetText("����Ӧ��ʾ!", "����������Ļ!");
}


void CCP_PolygonPlatformView::OnSelectPoint()
{
	// TODO: �ڴ���������������
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    if (pDoc->m_flagSelectType==1)
        pDoc->m_flagSelectType = 0;
    else pDoc->m_flagSelectType = 1;
    pDoc->m_flagSelect = false;
    Invalidate(); // ˢ��
}



void CCP_PolygonPlatformView::OnUpdateSelectPoint(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
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
	// TODO: �ڴ������������û����洦��������
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
	// TODO: �ڴ���������������
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    if (pDoc->m_flagSelectType==2)
        pDoc->m_flagSelectType = 0;
    else pDoc->m_flagSelectType = 2;
    pDoc->m_flagSelect = false;
    Invalidate(); // ˢ��
}




void CCP_PolygonPlatformView::OnUpdateSelectRegion(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
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
	// TODO: �ڴ���������������
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    if (pDoc->m_flagSelectType==3)
        pDoc->m_flagSelectType = 0;
    else pDoc->m_flagSelectType = 3;
    pDoc->m_flagSelect = false;
    Invalidate(); // ˢ��
}



void CCP_PolygonPlatformView::OnUpdateSelectPolygon(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
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
	// TODO: �ڴ���������������
    CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    if (pDoc->m_flagSelectType==4)
        pDoc->m_flagSelectType = 0;
    else pDoc->m_flagSelectType = 4;
    pDoc->m_flagSelect = false;
    Invalidate(); // ˢ��
}



void CCP_PolygonPlatformView::OnUpdateSelectTriangle(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
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
	// TODO: �ڴ���������������
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    if (pDoc->m_flagSelectType==5)
        pDoc->m_flagSelectType = 0;
    else pDoc->m_flagSelectType = 5;
    pDoc->m_flagSelect = false;
    Invalidate(); // ˢ��
}



void CCP_PolygonPlatformView::OnSelectOnly()
{
	// TODO: �ڴ���������������
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    pDoc->m_flagShowSelect ^= true;//��� -----ȡ����
    Invalidate(); // ˢ��
}



void CCP_PolygonPlatformView::OnUpdateSelectOnly(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    pCmdUI->SetCheck(pDoc->m_flagShowSelect);
}



void CCP_PolygonPlatformView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

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
        mb_statusSetText("����Ӳ����ɹ���", "������Ҽ���������Ӳ���");
        Invalidate( );
        CView::OnLButtonDown(nFlags, point);
        return;
    } // ��if����
    if (pDoc->m_flagAddIDPolygon==0)
         pn0 = &(pDoc->m_a);
    else pn0 = &(pDoc->m_b);
    if (pDoc->m_flagAdd == 3)
    {
        gb_insertPointInPolygon(*pn0, pDoc->m_flagAddIDRegion, pDoc->m_flagAddIDLoop, pDoc->m_flagAddIDPointInLoop, pg);
        mb_statusSetText("����Ӳ����ɹ���", "������Ҽ���������Ӳ���");
        Invalidate( );
        CView::OnLButtonDown(nFlags, point);
        return;
    } // ��if����
    if ((pDoc->m_flagAdd == 1) || (pDoc->m_flagAdd == 2)) // ��
    {
        pDoc->m_flagAddPointArray.push_back(pg);
        if (pDoc->m_flagAdd == 1)
             mb_statusSetText("�⻷����Ӳ����ɹ���", "������Ҽ������⻷��Ӳ���");
        else mb_statusSetText("�ڻ�����Ӳ����ɹ���", "������Ҽ������ڻ���Ӳ���");
        Invalidate( );
        CView::OnLButtonDown(nFlags, point);
        return;
    } // ��if����

    if ((!(pDoc->m_flagShowA))&&(!(pDoc->m_flagShowB)))
    {
        CView::OnLButtonDown(nFlags, point);
        return;
    } // if����
	    double da, db;
    int ida, idb, ira, irb;
    CP_Polygon* pn1;
    VT_IntArray* pSet0;
    VT_IntArray* pSet1;
    CP_Point p0, p1;
    switch(pDoc->m_flagSelectType)
    {
    case 1: // �㡣
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
                } // if/else����
                gb_intArrayInitPoint(*pSet0, *pn0, pDoc->m_flagSelectID, pDoc->m_tolerance);
                gb_intArrayInitPolygonSamePoint(*pSet1, *pn1, *pSet0, *pn0, pDoc->m_tolerance);
            } // if����
        } // if����
        break;
		    case 2: // ����
    case 3: // ����
    case 4: // ����Ρ�
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
                } // if/else����
				              if (pDoc->m_flagSelectType==4)
                    gb_intArrayInitPolygon(*pSet0, *pn0);
                else if (pDoc->m_flagSelectType==3)
                    gb_intArrayInitRegion(*pSet0, *pn0, pDoc->m_flagSelectRegion, pDoc->m_tolerance);
                else if (pDoc->m_flagSelectType==2)
                    gb_intArrayInitLoop(*pSet0, *pn0, pDoc->m_flagSelectRegion, pDoc->m_flagSelectID, pDoc->m_tolerance);
                gb_intArrayInitPolygonSamePoint(*pSet1, *pn1, *pSet0, *pn0, pDoc->m_tolerance);
            } // if����
        } // if����
        break;
    case 5: // �ʷ������Ρ�
        break;
    } // switch����

   CView::OnLButtonDown(nFlags, point);
    Invalidate( );
    if (pDoc->m_flagSelect)
        pDoc->m_flagMouseDown = true;
}




void CCP_PolygonPlatformView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//CView::OnLButtonUp(nFlags, point);
	CView::OnLButtonUp(nFlags, point);
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
    } // if����
	   switch(pDoc->m_flagSelectType)
    {
    case 1: // �㡣
        if (pDoc->m_flagSelectPolygon==0) // A
             gb_movePoint(pDoc->m_a, pDoc->m_flagSelectID, vx, vy);
        else gb_movePoint(pDoc->m_b, pDoc->m_flagSelectID, vx, vy);
        break;
    case 2: // ����
        if (pDoc->m_flagSelectPolygon==0) // A
             gb_moveLoop(pDoc->m_a, pDoc->m_flagSelectRegion, pDoc->m_flagSelectID, vx, vy);
        else gb_moveLoop(pDoc->m_b, pDoc->m_flagSelectRegion, pDoc->m_flagSelectID, vx, vy);
        break;
    case 3: // ����
        if (pDoc->m_flagSelectPolygon==0) // A
             gb_moveRegion(pDoc->m_a, pDoc->m_flagSelectRegion, vx, vy);
        else gb_moveRegion(pDoc->m_b, pDoc->m_flagSelectRegion, vx, vy);
        break;
		    case 4: // ����Ρ�
        if (pDoc->m_flagSelectPolygon==0) // A
             gb_movePolygon(pDoc->m_a, vx, vy);
        else gb_movePolygon(pDoc->m_b, vx, vy);
        break;
    case 5: // �ʷ������Ρ�
        break;
    } // switch����

    pDoc->m_basePoint = pg;
    Invalidate( );
    pDoc->m_flagMouseDown = false;
}



void CCP_PolygonPlatformView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    CView::OnMouseMove(nFlags, point);
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
    } // if����

    switch(pDoc->m_flagSelectType)
    {
    case 1: // �㡣
        if (pDoc->m_flagSelectPolygon==0) // A
             gb_movePoint(pDoc->m_a, pDoc->m_flagSelectID, vx, vy);
        else gb_movePoint(pDoc->m_b, pDoc->m_flagSelectID, vx, vy);
        break;
		    case 2: // ����
        if (pDoc->m_flagSelectPolygon==0) // A
             gb_moveLoop(pDoc->m_a, pDoc->m_flagSelectRegion, pDoc->m_flagSelectID, vx, vy);
        else gb_moveLoop(pDoc->m_b, pDoc->m_flagSelectRegion, pDoc->m_flagSelectID, vx, vy);
        break;
    case 3: // ����
        if (pDoc->m_flagSelectPolygon==0) // A
             gb_moveRegion(pDoc->m_a, pDoc->m_flagSelectRegion, vx, vy);
        else gb_moveRegion(pDoc->m_b, pDoc->m_flagSelectRegion, vx, vy);
        break;
    case 4: // ����Ρ�
        if (pDoc->m_flagSelectPolygon==0) // A
             gb_movePolygon(pDoc->m_a, vx, vy);
        else gb_movePolygon(pDoc->m_b, vx, vy);
        break;
    case 5: // �ʷ������Ρ�
        break;
    } // switch����

    pDoc->m_basePoint = pg;
    Invalidate( );
}


void CCP_PolygonPlatformView::OnNewRightInloop()
{
	// TODO: �ڴ���������������
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
        } // if/else����
    }
	else
    {
        switch(pDoc->m_flagSelectType)
        {
        case 1: // �㡣
        case 4: // ����Ρ�
            if (pDoc->m_flagSelectPolygon==0)
                 flagSuccess = gb_polygonNewInLoopRegular(pDoc->m_a, pDoc->m_a.m_regionArray.size( )-1,
                    pDoc->m_edgeNumber, dr, 0.0, 0.0);
            else
            {
                flagSuccess = gb_polygonNewInLoopRegular(pDoc->m_b, pDoc->m_b.m_regionArray.size( )-1,
                    pDoc->m_edgeNumber, dr, 0.0, 0.0);
                flagA = false;
            } // if/else����
            break;
            case 2: // ����
        case 3: // ����
            if (pDoc->m_flagSelectPolygon==0)
                 flagSuccess = gb_polygonNewInLoopRegular(pDoc->m_a, pDoc->m_flagSelectRegion,
                    pDoc->m_edgeNumber, dr, 0.0, 0.0);
            else
            {
                flagSuccess = gb_polygonNewInLoopRegular(pDoc->m_b, pDoc->m_flagSelectRegion,
                    pDoc->m_edgeNumber, dr, 0.0, 0.0);
                flagA = false;
            } // if/else����
            break;
        } // switch����
    } // if/else����
    Invalidate( );

    char s[100];
    sprintf_s(s, 100, "���ڻ�λ�ڶ����%c�С�", (flagA ? 'A' : 'B'));
    if (flagSuccess)
         mb_statusSetText("���ڻ������ɹ���", s);
    else mb_statusSetText("û�д������ڻ���", s);
}




void CCP_PolygonPlatformView::OnAddOutloop()
{
	// TODO: �ڴ���������������
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    if (pDoc->m_flagAdd!=0)
    {
        MessageBox("ǰһ����Ӳ�����û����ɣ������ǰһ������������Ҽ�����ǰһ����Ӳ�����", "��������");
    } // if����
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
    } // if/else����
    pDoc->m_flagAddPointArray.clear( );
    pDoc->m_flagAdd = 1;
    mb_statusSetText("�밴�������ڹ�������������⻷�ĵ�", "��������Ҽ��������⻷��Ӳ���");
}


void CCP_PolygonPlatformView::OnAddInloop()
{
	// TODO: �ڴ���������������
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    if (pDoc->m_flagAdd!=0)
    {
        MessageBox("ǰһ����Ӳ�����û����ɣ������ǰһ������������Ҽ�����ǰһ����Ӳ�����", "��������");
    } // if����
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
        } // if/else����
		       switch(pDoc->m_flagSelectType)
        {
        case 1: // �㡣
            flagSuceess = gb_findPointInLoop(*pn, pDoc->m_flagAddIDRegion,
                pDoc->m_flagAddIDLoop, pDoc->m_flagAddIDPointInLoop,
                pDoc->m_flagSelectID);
            break;
        case 2: // ����
            pDoc->m_flagAddIDRegion = pDoc->m_flagSelectRegion;
            pDoc->m_flagAddIDLoop = pDoc->m_flagSelectID;
            flagSuceess = true;
            break;
        case 3: // ����
            pDoc->m_flagAddIDRegion = pDoc->m_flagSelectRegion;
            pDoc->m_flagAddIDLoop = (*pn).m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray.size( )-1;
            flagSuceess = true;
            break;
			        case 4: // ����Ρ�
            pDoc->m_flagAddIDRegion = (*pn).m_regionArray.size( )-1;
            if (pDoc->m_flagAddIDRegion<0)
                break;
            pDoc->m_flagAddIDLoop = (*pn).m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray.size( )-1;
            if (pDoc->m_flagAddIDLoop<0)
                break;
            flagSuceess = true;
            break;
        } // switch����
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
        } // if/else����

        pDoc->m_flagAddIDRegion = (*pn).m_regionArray.size( )-1;
        if (pDoc->m_flagAddIDRegion>=0)
        {
            pDoc->m_flagAddIDLoop = (*pn).m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray.size( )-1;
            if (pDoc->m_flagAddIDLoop>=0)
            {
                flagSuceess = true;
            } // if(idL)����
        } // if(idr)����
    } // if/else����
    if (flagSuceess)
    {
        pDoc->m_flagAddPointArray.clear( );
        pDoc->m_flagAdd = 2;
        mb_statusSetText("�밴�������ڹ�������������ڻ��ĵ�", "��������Ҽ��������ڻ���Ӳ���");
    }
    else
    {
        pDoc->m_flagAdd = 0;
        if (pDoc->m_flagAddIDPolygon==0)
            mb_statusSetText("�ڻ���Ӳ���ʧ�ܡ�", "�����A��û���κ��⻷��");
        else
            mb_statusSetText("�ڻ���Ӳ���ʧ�ܡ�", "��������B�����⻷����ִ�б�������");
    } // if/else����
}




void CCP_PolygonPlatformView::OnAddPoint()
{
	// TODO: �ڴ���������������
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    if (pDoc->m_flagAdd!=0)
    {
        MessageBox("ǰһ����Ӳ�����û����ɣ������ǰһ������������Ҽ�����ǰһ����Ӳ�����", "��������");
        return;
    } // if����

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
        } // if/else����

        switch(pDoc->m_flagSelectType)
        {
        case 1: // �㡣
            flagSuceess = gb_findPointInLoop(*pn, pDoc->m_flagAddIDRegion,
                pDoc->m_flagAddIDLoop, pDoc->m_flagAddIDPointInLoop,
                pDoc->m_flagSelectID);
            break;
      case 2: // ����
            pDoc->m_flagAddIDRegion = pDoc->m_flagSelectRegion;
            pDoc->m_flagAddIDLoop = pDoc->m_flagSelectID;
            pDoc->m_flagAddIDPointInLoop = (*pn).m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray[pDoc->m_flagAddIDLoop].m_pointIDArray.size( )-1; 
            if (pDoc->m_flagAddIDPointInLoop<0)
                break;
            flagSuceess = true;
            break;
        case 3: // ����
            pDoc->m_flagAddIDRegion = pDoc->m_flagSelectRegion;
            pDoc->m_flagAddIDLoop = (*pn).m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray.size( )-1;
            pDoc->m_flagAddIDPointInLoop = (*pn).m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray[pDoc->m_flagAddIDLoop].m_pointIDArray.size( )-1; 
            if (pDoc->m_flagAddIDPointInLoop<0)
                break;
            flagSuceess = true;
            break;
			        case 4: // ����Ρ�
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
        } // switch����
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
        } // if/else����

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
                } // if(idLv)����
            } // if(idL)����
        } // if(idr)����
    } // if/else����
	       if (flagSuceess)
    {
        pDoc->m_flagAdd = 3;
        mb_statusSetText("���������ڹ���������ӵ�", "������Ҽ���������Ӳ���");
    }
    else
    {
        pDoc->m_flagAdd = 0;
        if (pDoc->m_flagAddIDPolygon==0)
            mb_statusSetText("����Ӳ���ʧ�ܡ�", "�����A��û���κ��⻷��");
        else
            mb_statusSetText("����Ӳ���ʧ�ܡ�", "��������B�����⻷����ִ�б�������");
        } // if/else����
}




void CCP_PolygonPlatformView::OnDelete()
{
	// TODO: �ڴ���������������
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    if (!pDoc->m_flagSelect)
    {
        MessageBox("��û��ʰȡͼ�Σ��޷�ɾ����", "��Ч����");
		return;
    } // if����
    if (pDoc->m_flagAdd!=0)
    {
        MessageBox("���ͼ�εĲ�����û�н������޷�ɾ����", "��Ч����");
		return;
    } // if����
	   CP_Polygon* pn;
    if (pDoc->m_flagSelectPolygon==0)
         pn = &(pDoc->m_a);
    else pn = &(pDoc->m_b);
    switch(pDoc->m_flagSelectType)
    {
    case 1: // �㡣
        gb_removePoint(*pn, pDoc->m_flagSelectID);
        break;
    case 2: // ����
        gb_removeLoop(*pn, pDoc->m_flagSelectRegion, pDoc->m_flagSelectID);
        break;
    case 3: // ����
        gb_removeRegion(*pn, pDoc->m_flagSelectRegion);
        break;
    case 4: // ����Ρ�
        pn->mb_clear( );
        break;
    } // switch����
    pDoc->m_flagSelect = false;
    Invalidate(); // ˢ��
}


void CCP_PolygonPlatformView::OnUpdateMoveSame(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    pCmdUI->SetCheck(pDoc->m_flagMoveSame);

}


void CCP_PolygonPlatformView::OnMoveSame()
{
	// TODO: �ڴ���������������
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    pDoc->m_flagMoveSame ^= true;
    if (!(pDoc->m_flagMoveSame))
    {
        pDoc->m_flagSelectIDSetInA.clear( );
        pDoc->m_flagSelectIDSetInB.clear( );
    } // if����

}


void CCP_PolygonPlatformView::OnUpdateViewA(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    pCmdUI->SetCheck(pDoc->m_flagShowA);

}




void CCP_PolygonPlatformView::OnViewA()
{
	// TODO: �ڴ���������������
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    pDoc->m_flagShowA ^= true;

    if (!(pDoc->m_flagShowA))//�������ʾA
        if (pDoc->m_flagSelectPolygon==0)//ʰȡ��A
            pDoc->m_flagSelect = false;//���ʾʲô��û��ʰȡ��

    Invalidate(); // ˢ��

}


void CCP_PolygonPlatformView::OnUpdateViewB(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    pCmdUI->SetCheck(pDoc->m_flagShowB);

}



void CCP_PolygonPlatformView::OnViewB()
{
	// TODO: �ڴ���������������
		CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    pDoc->m_flagShowB ^= true;
    if (!(pDoc->m_flagShowB))
        if (pDoc->m_flagSelectPolygon!=0)
            pDoc->m_flagSelect = false;
    Invalidate(); // ˢ��

}


void CCP_PolygonPlatformView::OnUpdateViewPointId(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    pCmdUI->SetCheck(pDoc->m_flagShowPointID);

}



void CCP_PolygonPlatformView::OnViewPointId()
{
	// TODO: �ڴ���������������
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    pDoc->m_flagShowPointID ^= true;
    Invalidate(); // ˢ��

}


void CCP_PolygonPlatformView::OnViewTFace()
{
	// TODO: �ڴ���������������
		CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    pDoc->m_flagShowTriangleFace ^= true;
    Invalidate(); // ˢ��

}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CCP_PolygonPlatformView::OnViewTEdge()
{
	// TODO: �ڴ���������������

}


void CCP_PolygonPlatformView::OnUpdateViewTFace(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    pCmdUI->SetCheck(pDoc->m_flagShowTriangleFace);

}

/*��ʾ����������*/
void CCP_PolygonPlatformView::OnViewResult()
{
	// TODO: �ڴ���������������
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    pDoc->m_flagShowResult ^= true;

    Invalidate(); // ˢ��
}

void CCP_PolygonPlatformView::OnUpdateViewResult(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    pCmdUI->SetCheck(pDoc->m_flagShowResult);
}

/*������*/
void CCP_PolygonPlatformView::OnPolygonUnion()
{
	OnCheck();//�Ƚ��кϷ��Լ��
	// TODO: �ڴ���������������
	CCP_PolygonPlatformDoc* pDoc = GetDocument();//��ȡDoc��ָ��
	int polyR, polyL, polyP;//�ֱ��ʾ����������򡢻���������Ĵ�С 
	AddNodes(pDoc);//�����ν��㣬������ӽ����Ķ���θ���tempA��tempB
	
	/*�ȵõ�A��B�����еı�*/
	pDoc->ABEdge.clear();//�����
	/*����A�Ҳ���B�����ı�+A��B�غ���ͬ��ı�*/
	polyR = tempA.m_regionArray.size();
	for(int i=0; i<polyR; i++)//ѭ������ÿ������
	{
		polyL = tempA.m_regionArray[i].m_loopArray.size();
		for(int j=0; j<polyL; j++)//ѭ������ÿ����
		{
			polyP = tempA.m_regionArray[i].m_loopArray[j].m_pointIDArray.size();
			for(int k=0; k<polyP; k++)//ѭ������ÿ����
			{
				int u1, u2;//�߶˵�����
				u1 = tempA.m_regionArray[i].m_loopArray[j].m_pointIDArray[k];
				u2 = tempA.m_regionArray[i].m_loopArray[j].m_pointIDArray[(k+1)%polyP];
				Segment temp;//��ʱ�����洢��ǰ��
				temp.u1 = tempA.m_pointArray[u1];//��ȡ�߶ζ˵����꣬������ʱ�ṹ����
				temp.u2 = tempA.m_pointArray[u2];

				/*�жϸ��߶��Ƿ���AB�غ���ͬ��ı�*/
				vector<Segment>::iterator iter;
				iter = find(SegCoinSame.begin(),SegCoinSame.end(),temp);//�ڱ��غ���ͬ��������Ѱ�ҿ���û�е�ǰ��

				if((SegCoinSame.size()==0||iter==SegCoinSame.end())&&SegInPolygon(temp.u1,temp.u2,pDoc->m_b))//����ñ߼Ȳ����غ���ͬ��ıߣ�������B������ѭ��
				{
					continue;
				}
				else//�����������A��B�غ���ͬ�� �� ������A�Ҳ���B��������������뵽ABEdge��
				{
					pDoc->ABEdge.push_back(temp);//������ӵ�A��B�ı߼�����
				}
			}
		}
	}
	/*����B�Ҳ���A�����ı�*/
	polyR = tempB.m_regionArray.size();
	for(int i=0; i<polyR; i++)//ѭ������ÿ������
	{
		polyL = tempB.m_regionArray[i].m_loopArray.size();
		for(int j=0; j<polyL; j++)//ѭ������ÿ����
		{
			polyP = tempB.m_regionArray[i].m_loopArray[j].m_pointIDArray.size();
			for(int k=0; k<polyP; k++)//ѭ������ÿ����
			{
				int u1, u2;//�߶˵�����
				u1 = tempB.m_regionArray[i].m_loopArray[j].m_pointIDArray[k];
				u2 = tempB.m_regionArray[i].m_loopArray[j].m_pointIDArray[(k+1)%polyP];
				Segment temp;//��ʱ�����洢��ǰ��
				temp.u1 = tempB.m_pointArray[u1];//��ȡ�߶ζ˵����꣬������ʱ�ṹ����
				temp.u2 = tempB.m_pointArray[u2];

				if(SegInPolygon(temp.u1,temp.u2,pDoc->m_a))//����ñ�����A������ѭ��(�������߶��غϵ����)
				{
					continue;
				}
				else//���������������A�Ҳ���B��������������뵽ABEdge��
				{
					pDoc->ABEdge.push_back(temp);//������ӵ�A��B�ı߼�����
				}
			}
		}
	}

	/*�����߼��ϣ��õ����յĶ���νṹ*/
	sort(pDoc->ABEdge.begin(),pDoc->ABEdge.end(),ascending);//�ȶ�vector��Ԫ�ذ���x-y������������
	ConstructPolygon(pDoc);//���߼��Ϲ����ɶ����

	Invalidate(true);
}

/*������*/
void CCP_PolygonPlatformView::OnPolygonIntersection()
{
	OnCheck();//�Ƚ��кϷ��Լ��
	// TODO: �ڴ���������������
	CCP_PolygonPlatformDoc* pDoc = GetDocument();//��ȡDoc��ָ��
	int polyR, polyL, polyP;//�ֱ��ʾ����������򡢻���������Ĵ�С 
	AddNodes(pDoc);//�����ν��㣬������ӽ����Ķ���θ���tempA��tempB
	
	/*�ȵõ�A��B�����еı�*/
	pDoc->ABEdge.clear();//�����
	/*����A�ұ�B�����ı�+A��B�غ���ͬ��ı�*/
	polyR = tempA.m_regionArray.size();
	for(int i=0; i<polyR; i++)//ѭ������ÿ������
	{
		polyL = tempA.m_regionArray[i].m_loopArray.size();
		for(int j=0; j<polyL; j++)//ѭ������ÿ����
		{
			polyP = tempA.m_regionArray[i].m_loopArray[j].m_pointIDArray.size();
			for(int k=0; k<polyP; k++)//ѭ������ÿ����
			{
				int u1, u2;//�߶˵�����
				u1 = tempA.m_regionArray[i].m_loopArray[j].m_pointIDArray[k];
				u2 = tempA.m_regionArray[i].m_loopArray[j].m_pointIDArray[(k+1)%polyP];
				Segment temp;//��ʱ�����洢��ǰ��
				temp.u1 = tempA.m_pointArray[u1];//��ȡ�߶ζ˵����꣬������ʱ�ṹ����
				temp.u2 = tempA.m_pointArray[u2];

				/*�жϸ��߶��Ƿ���AB�غ���ͬ��ı�*/
				vector<Segment>::iterator iter;
				iter = find(SegCoinSame.begin(),SegCoinSame.end(),temp);//�ڱ��غ���ͬ��������Ѱ�ҿ���û�е�ǰ��

				if((SegCoinSame.size()==0||iter==SegCoinSame.end())&&!SegInPolygon(temp.u1,temp.u2,pDoc->m_b))//����ñ߼Ȳ����غ���ͬ��ıߣ��ֲ�����B������ѭ��
				{
					continue;
				}
				else//�����������A��B�غ���ͬ�� �� ������A�ұ�B��������������뵽ABEdge��
				{
					pDoc->ABEdge.push_back(temp);//������ӵ�A��B�ı߼�����
				}
			}
		}
	}
	/*����B�ұ�A�����ı�*/
	polyR = tempB.m_regionArray.size();
	for(int i=0; i<polyR; i++)//ѭ������ÿ������
	{
		polyL = tempB.m_regionArray[i].m_loopArray.size();
		for(int j=0; j<polyL; j++)//ѭ������ÿ����
		{
			polyP = tempB.m_regionArray[i].m_loopArray[j].m_pointIDArray.size();
			for(int k=0; k<polyP; k++)//ѭ������ÿ����
			{
				int u1, u2;//�߶˵�����
				u1 = tempB.m_regionArray[i].m_loopArray[j].m_pointIDArray[k];
				u2 = tempB.m_regionArray[i].m_loopArray[j].m_pointIDArray[(k+1)%polyP];
				Segment temp;//��ʱ�����洢��ǰ��
				temp.u1 = tempB.m_pointArray[u1];//��ȡ�߶ζ˵����꣬������ʱ�ṹ����
				temp.u2 = tempB.m_pointArray[u2];

				/*�жϸ��߶��Ƿ���AB�غ���ͬ��ı�*/
				vector<Segment>::iterator iter;
				iter = find(SegCoinSame.begin(),SegCoinSame.end(),temp);//�ڱ��غ���ͬ��������Ѱ�ҿ���û�е�ǰ��
				if(iter!=SegCoinSame.end()||!SegInPolygon(temp.u1,temp.u2,pDoc->m_a))//����ñ߲�����A������ѭ��(�������߶��غϵ����)
				{
					continue;
				}
				else//���������������A�ұ�B��������������뵽ABEdge��
				{
					pDoc->ABEdge.push_back(temp);//������ӵ�A��B�ı߼�����
				}
			}
		}
	}

	/*�����߼��ϣ��õ����յĶ���νṹ*/
	sort(pDoc->ABEdge.begin(),pDoc->ABEdge.end(),ascending);//�ȶ�vector��Ԫ�ذ���x-y������������
	ConstructPolygon(pDoc);//���߼��Ϲ����ɶ����

	Invalidate(true);
}

/*�����㣨A-B��*/
void CCP_PolygonPlatformView::OnPolygonAB()
{
	OnCheck();//�Ƚ��кϷ��Լ��
	// TODO: �ڴ���������������
	CCP_PolygonPlatformDoc* pDoc = GetDocument();//��ȡDoc��ָ��
	int polyR, polyL, polyP;//�ֱ��ʾ����������򡢻���������Ĵ�С 
	AddNodes(pDoc);//�����ν��㣬������ӽ����Ķ���θ���tempA��tempB
	
	/*�ȵõ�A-B�����еı�*/
	pDoc->ABEdge.clear();//�����
	/*����A�Ҳ���B�����ı�+A��B�غ��ҷ���ı�*/
	polyR = tempA.m_regionArray.size();
	for(int i=0; i<polyR; i++)//ѭ������ÿ������
	{
		polyL = tempA.m_regionArray[i].m_loopArray.size();
		for(int j=0; j<polyL; j++)//ѭ������ÿ����
		{
			polyP = tempA.m_regionArray[i].m_loopArray[j].m_pointIDArray.size();
			for(int k=0; k<polyP; k++)//ѭ������ÿ����
			{
				int u1, u2;//�߶˵�����
				u1 = tempA.m_regionArray[i].m_loopArray[j].m_pointIDArray[k];
				u2 = tempA.m_regionArray[i].m_loopArray[j].m_pointIDArray[(k+1)%polyP];
				Segment temp;//��ʱ�����洢��ǰ��
				temp.u1 = tempA.m_pointArray[u1];//��ȡ�߶ζ˵����꣬������ʱ�ṹ����
				temp.u2 = tempA.m_pointArray[u2];

				/*�жϸ��߶��Ƿ���AB�غ��ҷ���ı�*/
				vector<Segment>::iterator iterR;
				iterR = find(SegCoinReve.begin(),SegCoinReve.end(),temp);//�ڱ��غ��ҷ���������Ѱ�ҿ���û�е�ǰ��
				/*�жϸ��߶��Ƿ���AB�غ���ͬ��ı�*/
				vector<Segment>::iterator iterS;
				iterS = find(SegCoinSame.begin(),SegCoinSame.end(),temp);//�ڱ��غ��ҷ���������Ѱ�ҿ���û�е�ǰ��

				if(iterS!=SegCoinSame.end() || ((SegCoinReve.size()==0||iterR==SegCoinReve.end())&&SegInPolygon(temp.u1,temp.u2,pDoc->m_b)))//����ñ����غ�ͬ��ı߻��߼Ȳ����غ��ҷ���ıߣ�������B������ѭ��
				{
					continue;
				}
				else//�����������A��B�غ��ҷ��� �� ������A�Ҳ���B��������������뵽ABEdge��
				{
					pDoc->ABEdge.push_back(temp);//������ӵ�A��B�ı߼�����
				}
			}
		}
	}
	/*����B�ұ�A�����ıߣ��������Ƿ���*/
	polyR = tempB.m_regionArray.size();
	for(int i=0; i<polyR; i++)//ѭ������ÿ������
	{
		polyL = tempB.m_regionArray[i].m_loopArray.size();
		for(int j=0; j<polyL; j++)//ѭ������ÿ����
		{
			polyP = tempB.m_regionArray[i].m_loopArray[j].m_pointIDArray.size();
			for(int k=0; k<polyP; k++)//ѭ������ÿ����
			{
				int u1, u2;//�߶˵�����
				u1 = tempB.m_regionArray[i].m_loopArray[j].m_pointIDArray[k];
				u2 = tempB.m_regionArray[i].m_loopArray[j].m_pointIDArray[(k+1)%polyP];
				Segment temp;//��ʱ�����洢��ǰ��
				temp.u1 = tempB.m_pointArray[u1];//��ȡ�߶ζ˵����꣬������ʱ�ṹ����
				temp.u2 = tempB.m_pointArray[u2];

				/*�жϸ��߶��Ƿ���AB�غ��ҷ���ı�*/
				vector<Segment>::iterator iterR;
				iterR = find(SegCoinReve.begin(),SegCoinReve.end(),temp);//�ڱ��غ��ҷ���������Ѱ�ҿ���û�е�ǰ��
				/*�жϸ��߶��Ƿ���AB�غ���ͬ��ı�*/
				vector<Segment>::iterator iterS;
				iterS = find(SegCoinSame.begin(),SegCoinSame.end(),temp);//�ڱ��غ��ҷ���������Ѱ�ҿ���û�е�ǰ��

				if((SegCoinReve.size()!=0&&iterR!=SegCoinReve.end()) || (SegCoinSame.size()!=0&&iterS!=SegCoinSame.end()))//�߶��غ�,˵��������A���棬����ѭ��
				{
					continue;
				}
				else if(!SegInPolygon(temp.u1,temp.u2,pDoc->m_a))//����ñ߲�����A������ѭ��(�������߶��غϵ����)
				{
					continue;
				}
				else//���������������B�ұ�A�����������䷴�򣬲����뵽ABEdge��
				{
					temp.u1 = tempB.m_pointArray[u2];
					temp.u2 = tempB.m_pointArray[u1];
					pDoc->ABEdge.push_back(temp);//������ӵ�A-B�ı߼�����
				}
			}
		}
	}

	/*�����߼��ϣ��õ����յĶ���νṹ*/
	sort(pDoc->ABEdge.begin(),pDoc->ABEdge.end(),ascending);//�ȶ�vector��Ԫ�ذ���x-y������������
	ConstructPolygon(pDoc);//���߼��Ϲ����ɶ����

	Invalidate(true);
}

/*�����㣨B-A��*/
void CCP_PolygonPlatformView::OnPolygonBA()
{
	OnCheck();//�Ƚ��кϷ��Լ��
	// TODO: �ڴ���������������
	CCP_PolygonPlatformDoc* pDoc = GetDocument();//��ȡDoc��ָ��
	int polyR, polyL, polyP;//�ֱ��ʾ����������򡢻���������Ĵ�С 
	AddNodes(pDoc);//�����ν��㣬������ӽ����Ķ���θ���tempA��tempB
	
	/*�ȵõ�B-A�����еı�*/
	pDoc->ABEdge.clear();//�����
	/*����B�Ҳ���A�����ı�+A��B�غ��ҷ���ı�*/
	polyR = tempB.m_regionArray.size();
	for(int i=0; i<polyR; i++)//ѭ������ÿ������
	{
		polyL = tempB.m_regionArray[i].m_loopArray.size();
		for(int j=0; j<polyL; j++)//ѭ������ÿ����
		{
			polyP = tempB.m_regionArray[i].m_loopArray[j].m_pointIDArray.size();
			for(int k=0; k<polyP; k++)//ѭ������ÿ����
			{
				int u1, u2;//�߶˵�����
				u1 = tempB.m_regionArray[i].m_loopArray[j].m_pointIDArray[k];
				u2 = tempB.m_regionArray[i].m_loopArray[j].m_pointIDArray[(k+1)%polyP];
				Segment temp;//��ʱ�����洢��ǰ��
				temp.u1 = tempB.m_pointArray[u1];//��ȡ�߶ζ˵����꣬������ʱ�ṹ����
				temp.u2 = tempB.m_pointArray[u2];

				/*�жϸ��߶��Ƿ���AB�غ���ͬ��ı�*/
				vector<Segment>::iterator iterS;
				iterS = find(SegCoinSame.begin(),SegCoinSame.end(),temp);//�ڱ��غ��ҷ���������Ѱ�ҿ���û�е�ǰ��

				if(iterS!=SegCoinSame.end() || SegInPolygon(temp.u1,temp.u2,pDoc->m_a))//����ñ�����A������ѭ��
				{
					continue;
				}
				else//���������������B�Ҳ���A��������������뵽ABEdge��
				{
					pDoc->ABEdge.push_back(temp);//������ӵ�A��B�ı߼�����
				}
			}
		}
	}
	/*����A�ұ�B�����ıߣ��������Ƿ���*/
	polyR = tempA.m_regionArray.size();
	for(int i=0; i<polyR; i++)//ѭ������ÿ������
	{
		polyL = tempA.m_regionArray[i].m_loopArray.size();
		for(int j=0; j<polyL; j++)//ѭ������ÿ����
		{
			polyP = tempA.m_regionArray[i].m_loopArray[j].m_pointIDArray.size();
			for(int k=0; k<polyP; k++)//ѭ������ÿ����
			{
				int u1, u2;//�߶˵�����
				u1 = tempA.m_regionArray[i].m_loopArray[j].m_pointIDArray[k];
				u2 = tempA.m_regionArray[i].m_loopArray[j].m_pointIDArray[(k+1)%polyP];
				Segment temp;//��ʱ�����洢��ǰ��
				temp.u1 = tempA.m_pointArray[u1];//��ȡ�߶ζ˵����꣬������ʱ�ṹ����
				temp.u2 = tempA.m_pointArray[u2];

				/*�жϸ��߶��Ƿ���AB�غ��ҷ���ı�*/
				vector<Segment>::iterator iter;
				iter = find(SegCoinReve.begin(),SegCoinReve.end(),temp);//�ڱ��غ��ҷ���������Ѱ�ҿ���û�е�ǰ��
				/*�жϸ��߶��Ƿ���AB�غ���ͬ��ı�*/
				vector<Segment>::iterator iterS;
				iterS = find(SegCoinSame.begin(),SegCoinSame.end(),temp);//�ڱ��غ��ҷ���������Ѱ�ҿ���û�е�ǰ��

				if(iterS!=SegCoinSame.end() || (SegCoinReve.size()==0||iter==SegCoinReve.end())&&!SegInPolygon(temp.u1,temp.u2,pDoc->m_b))//����ñ߲����غ��ҷ���ı�,�Ҹñ߲�����B
				{
					continue;
				}
				else if(iter!=SegCoinReve.end())
				{
					pDoc->ABEdge.push_back(temp);//������ӵ�A-B�ı߼�����
				}
				else//���������������A�ұ�B�����������䷴�򣬲����뵽ABEdge��
				{
					temp.u1 = tempA.m_pointArray[u2];
					temp.u2 = tempA.m_pointArray[u1];
					pDoc->ABEdge.push_back(temp);//�������ı���ӵ�A-B�ı߼�����
				}
			}
		}
	}

	/*�����߼��ϣ��õ����յĶ���νṹ*/
	sort(pDoc->ABEdge.begin(),pDoc->ABEdge.end(),ascending);//�ȶ�vector��Ԫ�ذ���x-y������������
	ConstructPolygon(pDoc);//���߼��Ϲ����ɶ����

	Invalidate(true);
}

/*�Ϸ��Լ��
  ���¼��ּ�飺
  1.�⻷ӦΪ��ʱ�룬�ڻ�ӦΪ˳ʱ�루�ֶ�������⻷ʱ���ܻ������
    ͬʱ�����ÿ�����е�ĸ����Ƿ�>=3�������ǣ����Ϸ���
  2.�� �����Ǽ򵥶���Σ����������������߲����г��˵�����������㣻
  3.����ε�һ�������У��ڻ������ڸ�������⻷���ڲ���
  4.����ε�һ�������У��ڻ�֮�䲻���໥������
  5.����ε�һ�������У��ڻ��佻��ֻ���Ƕ˵㣬�����Ǳ߻�������
  6.ͬһ������εĲ�ͬ�������ཻ�����ཻ������ֻ���Ƕ˵㣬�����Ǳ߻�������
   ��ע�⵱�����ֻ�������⻷�����������⻷�໥Ƕ�׵��������
*/
/*���������ķ���+���е�ĸ���>=3*/
bool LoopDirection(CCP_PolygonPlatformDoc* pDoc)
{
	int polyR, polyL, polyP;//�ֱ��ʾ����������򡢻���������Ĵ�С

	//�ж϶����A�Ƿ�Ϸ�
	polyR = pDoc->m_a.m_regionArray.size();
	for(int i=0; i<polyR; i++)//�����ж�ÿ�������Ƿ�Ϸ�
	{
		//�жϸ������е��⻷�Ƿ�Ϸ�����ʱ��,���Ϊ����ֻ��Ҫ͹�����ӵ������߼��ɣ�
		polyP = pDoc->m_a.m_regionArray[i].m_loopArray[0].m_pointIDArray.size();
		if(polyP < 3)//�������
		{
			MessageBoxA(0,"�����A(�⻷)������٣������¹��죡","����",MB_OK);
			pDoc->m_a.mb_clear();//���
			if(!pDoc->m_a.m_pointArray.size()&&!pDoc->m_b.m_pointArray.size())
			{
				pDoc->mb_initData();
			}
			return false;
		}
		else
		{
			double maxX=-9999999, x1, x2, y1, y2;//���Ҳ�͹���x���꣬����������������
			int maxID, u1, u2, u3;//���Ҳ�͹���ID�����������ڶ��������е�ID

			for(int p=0; p<polyP; p++)//Ѱ�����Ҳ�͹��
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
			if(x1*y2-x2*y1<=0)//��˲��������������⻷���Ϸ�
			{
				MessageBoxA(0,"�����A(�⻷)������������¹��죡","����",MB_OK);
				pDoc->m_a.mb_clear();//���
				if(!pDoc->m_a.m_pointArray.size()&&!pDoc->m_b.m_pointArray.size())
				{
					pDoc->mb_initData();
				}
				return false;
			}
		}

		//˳���ж������ڻ��Ƿ�Ϸ���˳ʱ�룬���Ϊ����ֻ��Ҫ�жϵ�1��2���߼��ɣ�
		polyL = pDoc->m_a.m_regionArray[i].m_loopArray.size();
		for(int j=1; j<polyL; j++)//�ڻ��ӻ�����ĵڶ���λ�ÿ�ʼ
		{
			polyP = pDoc->m_a.m_regionArray[i].m_loopArray[j].m_pointIDArray.size();
			if(polyP < 3)//�������
			{
				MessageBoxA(0,"�����A(�ڻ�)������٣������¹��죡","����",MB_OK);
				pDoc->m_a.mb_clear();//���
				if(!pDoc->m_a.m_pointArray.size()&&!pDoc->m_b.m_pointArray.size())
				{
					pDoc->mb_initData();
				}
				return false;
			}
			else
			{
				double maxX=-9999999, x1, x2, y1, y2;//���Ҳ�͹���x���꣬����������������
				int maxID, u1, u2, u3;//���Ҳ�͹���ڻ��е���ţ����������ڶ��������е�ID

				for(int p=0; p<polyP; p++)//Ѱ�����Ҳ�͹��
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
				if(x1*y2-x2*y1>=0)//��˲��Ǹ����������ڻ����Ϸ�
				{
					MessageBoxA(0,"�����A(�ڻ�)������������¹��죡","����",MB_OK);
					pDoc->m_a.mb_clear();//���
					if(!pDoc->m_a.m_pointArray.size()&&!pDoc->m_b.m_pointArray.size())
					{
						pDoc->mb_initData();
					}
					return false;
				}
			}
		}
	}

	//�ж϶����B�Ƿ�Ϸ����жϸ������ķ���
	polyR = pDoc->m_b.m_regionArray.size();
	for(int i=0; i<polyR; i++)//�����ж�ÿ�������Ƿ�Ϸ�
	{
		//�жϸ������е��⻷�Ƿ�Ϸ�����ʱ��,���Ϊ����ֻ��Ҫ�жϵ�1��2���߼��ɣ�
		polyP = pDoc->m_b.m_regionArray[i].m_loopArray[0].m_pointIDArray.size();
		if(polyP < 3)//�������
		{
			MessageBoxA(0,"�����B(�⻷)������٣������¹��죡","����",MB_OK);
			pDoc->m_b.mb_clear();//���
			if(!pDoc->m_a.m_pointArray.size()&&!pDoc->m_b.m_pointArray.size())
			{
				pDoc->mb_initData();
			}
			return false;
		}
		else
		{
			double maxX=-9999999, x1, x2, y1, y2;//���Ҳ�͹���x���꣬����������������
			int maxID, u1, u2, u3;//���Ҳ�͹���ID�����������ڶ��������е�ID

			for(int p=0; p<polyP; p++)//Ѱ�����Ҳ�͹��
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
			if(x1*y2-x2*y1<=0)//��˲��������������⻷���Ϸ�
			{
				MessageBoxA(0,"�����B(�⻷)������������¹��죡","����",MB_OK);
				pDoc->m_b.mb_clear();//���
				if(!pDoc->m_a.m_pointArray.size()&&!pDoc->m_b.m_pointArray.size())
				{
					pDoc->mb_initData();
				}
				return false;
			}
		}

		//˳���ж������ڻ��Ƿ�Ϸ���˳ʱ�룬���Ϊ����ֻ��Ҫ�жϵ�1��2���߼��ɣ�
		polyL = pDoc->m_b.m_regionArray[i].m_loopArray.size();
		for(int j=1; j<polyL; j++)//�ڻ��ӻ�����ĵڶ���λ�ÿ�ʼ
		{
			polyP = pDoc->m_b.m_regionArray[i].m_loopArray[j].m_pointIDArray.size();
			if(polyP < 3)//�������
			{
				MessageBoxA(0,"�����B(�ڻ�)������٣������¹��죡","����",MB_OK);
				pDoc->m_b.mb_clear();//���
				if(!pDoc->m_a.m_pointArray.size()&&!pDoc->m_b.m_pointArray.size())
				{
					pDoc->mb_initData();
				}
				return false;
			}
			else
			{
				double maxX=-9999999, x1, x2, y1, y2;//���Ҳ�͹���x���꣬����������������
				int maxID, u1, u2, u3;//���Ҳ�͹���ID�����������ڶ��������е�ID

				for(int p=0; p<polyP; p++)//Ѱ�����Ҳ�͹��
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
				if(x1*y2-x2*y1>=0)//��˲��Ǹ����������ڻ����Ϸ�
				{
					MessageBoxA(0,"�����B(�ڻ�)������������¹��죡","����",MB_OK);
					pDoc->m_b.mb_clear();//���
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

/*���һ��������е����б�����֮����û�г��˶˵�����������㣬�������غϱ�(2,5)*/
bool EdgeIntersection(CCP_PolygonPlatformDoc* pDoc)
{
	vector<Segment> Edge;//������������б߷ŵ����vector�У�Ȼ����˫��ѭ�����
	int polyR, polyL, polyP;//�ֱ��ʾ����������򡢻���������Ĵ�С

	//���A�����
	//�������A�е�ÿ���߶����뵽Edge��
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
				int u1,u2;//�߶������˵��ڶ˵������е����
				u1 = pDoc->m_a.m_regionArray[i].m_loopArray[j].m_pointIDArray[k];
				u2 = pDoc->m_a.m_regionArray[i].m_loopArray[j].m_pointIDArray[(k+1)%polyP];
				seg.u1 = pDoc->m_a.m_pointArray[u1];
				seg.u2 = pDoc->m_a.m_pointArray[u2];
				Edge.push_back(seg);
			}
		}
	}
	//˫��ѭ��������б�����֮�����޶˵��������������غϱ�
	for(int i=0; Edge.size()>0&&i<Edge.size()-1; i++)
	{
		for(int j=i+1; j<Edge.size(); j++)
		{
			Segment temp;//�洢��һ���ߵķ����
			temp.u1 = Edge[i].u2;
			temp.u2 = Edge[i].u1;

			if(Edge[i]==Edge[j] || temp==Edge[j])//������ȫ�غ�
			{
				MessageBoxA(0,"�����A���б��غϣ����Ϸ��������¹��죡","����",MB_OK);
				pDoc->m_a.mb_clear();//���
				if(!pDoc->m_a.m_pointArray.size()&&!pDoc->m_b.m_pointArray.size())
				{
					pDoc->mb_initData();
				}
				return false;
			}
			else if(parallel(Edge[i].u1,Edge[i].u2,Edge[j].u1,Edge[j].u2) && (dots_inline(Edge[i].u1,Edge[i].u2,Edge[j].u1)||dots_inline(Edge[j].u1,Edge[j].u2,Edge[i].u1)))//�����غ�
			{
				MessageBoxA(0,"�����A���б��غϣ����Ϸ��������¹��죡","����",MB_OK);
				pDoc->m_a.mb_clear();//���
				if(!pDoc->m_a.m_pointArray.size()&&!pDoc->m_b.m_pointArray.size())
				{
					pDoc->mb_initData();
				}
				return false;
			}
			else if(!intersect_in(Edge[i].u1,Edge[i].u2,Edge[j].u1,Edge[j].u2))//���ཻ
			{
				continue;		
			}
			else if(Edge[i].u1==Edge[j].u1 || Edge[i].u1==Edge[j].u2 || Edge[i].u2==Edge[j].u1 || Edge[i].u2==Edge[j].u2)//�����Ƕ���
			{
				continue;
			}
			else
			{
				MessageBoxA(0,"�����A���б��ཻ�����Ϸ��������¹��죡","����",MB_OK);
				pDoc->m_a.mb_clear();//���
				if(!pDoc->m_a.m_pointArray.size()&&!pDoc->m_b.m_pointArray.size())
				{
					pDoc->mb_initData();
				}
				return false;
			}
		}
	}
	Edge.clear();//�߽ṹ���
	//���B�����
	//�������B�е�ÿ���߶����뵽Edge��
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
				int u1,u2;//�߶������˵��ڶ˵������е����
				u1 = pDoc->m_b.m_regionArray[i].m_loopArray[j].m_pointIDArray[k];
				u2 = pDoc->m_b.m_regionArray[i].m_loopArray[j].m_pointIDArray[(k+1)%polyP];
				seg.u1 = pDoc->m_b.m_pointArray[u1];
				seg.u2 = pDoc->m_b.m_pointArray[u2];
				Edge.push_back(seg);
			}
		}
	}
	//˫��ѭ��������б�����֮�����޶˵��������������غϱ�
	for(int i=0; Edge.size()>0&&i<Edge.size()-1; i++)
	{
		for(int j=i+1; j<Edge.size(); j++)
		{
			Segment temp;//�洢��һ���ߵķ����
			temp.u1 = Edge[i].u2;
			temp.u2 = Edge[i].u1;

			if(Edge[i]==Edge[j] || temp==Edge[j])//������ȫ�غ�
			{
				MessageBoxA(0,"�����B���б��غϣ����Ϸ��������¹��죡","����",MB_OK);
				pDoc->m_b.mb_clear();//���
				if(!pDoc->m_a.m_pointArray.size()&&!pDoc->m_b.m_pointArray.size())
				{
					pDoc->mb_initData();
				}
				return false;
			}
			else if(parallel(Edge[i].u1,Edge[i].u2,Edge[j].u1,Edge[j].u2) && (dots_inline(Edge[i].u1,Edge[i].u2,Edge[j].u1)||dots_inline(Edge[j].u1,Edge[j].u2,Edge[i].u1)))//�����غ�
			{
				MessageBoxA(0,"�����B���б��غϣ����Ϸ��������¹��죡","����",MB_OK);
				pDoc->m_b.mb_clear();//���
				if(!pDoc->m_a.m_pointArray.size()&&!pDoc->m_b.m_pointArray.size())
				{
					pDoc->mb_initData();
				}
				return false;
			}
			else if(!intersect_in(Edge[i].u1,Edge[i].u2,Edge[j].u1,Edge[j].u2))//���ཻ
			{
				continue;		
			}
			else if(Edge[i].u1==Edge[j].u1 || Edge[i].u1==Edge[j].u2 || Edge[i].u2==Edge[j].u1 || Edge[i].u2==Edge[j].u2)//�����Ƕ���
			{
				continue;
			}
			else
			{
				MessageBoxA(0,"�����B���б��ཻ�����Ϸ��������¹��죡","����",MB_OK);
				pDoc->m_b.mb_clear();//���
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

/*�������ͬһ�����е��ڻ��Ƿ��ڸ�������⻷��
--����������⻷��Ϊһ������Σ��жϸ����������ڻ������бߵ��е�
  �Ƿ����������������ж�����ڻ��ǲ������⻷�ڣ�*/
bool InOutLoop(CCP_PolygonPlatformDoc* pDoc)
{
	CP_Polygon temp;//��ÿ��������⻷��Ϊһ�������Ķ����
	int polyR, polyL, polyP;//�ֱ��ʾ����������򡢻���������Ĵ�С

	//�ж϶����A�Ƿ�Ϸ�
	polyR = pDoc->m_a.m_regionArray.size();
	for(int i=0; i<polyR; i++)//�Զ���ε�ÿ�����򵥶����
	{
		temp.mb_clear();
		temp.m_pointArray = pDoc->m_a.m_pointArray;
		temp.m_regionArray.push_back(pDoc->m_a.m_regionArray[i]);
		temp.m_regionArray[0].m_loopArray.clear();
		temp.m_regionArray[0].m_loopArray.push_back(pDoc->m_a.m_regionArray[i].m_loopArray[0]);//���������е��⻷��Ϊһ�������Ķ����temp
		polyL = pDoc->m_a.m_regionArray[i].m_loopArray.size();
		for(int j=1; j<polyL; j++)//�����ж�ÿ���ڻ��ǲ������⻷(�����temp)�ڲ�
		{
			polyP = pDoc->m_a.m_regionArray[i].m_loopArray[j].m_pointIDArray.size();
			for(int k=0; k<polyP; k++)
			{
				int u1,u2;//�߶˵����
				CP_Point middle;//���е�
				u1 = pDoc->m_a.m_regionArray[i].m_loopArray[j].m_pointIDArray[k];
				u2 = pDoc->m_a.m_regionArray[i].m_loopArray[j].m_pointIDArray[(k+1)%polyP];
				middle.m_x = (pDoc->m_a.m_pointArray[u1].m_x+pDoc->m_a.m_pointArray[u2].m_x)/2;
				middle.m_y = (pDoc->m_a.m_pointArray[u1].m_y+pDoc->m_a.m_pointArray[u2].m_y)/2;
				if(!PointInPoligon(middle,temp))//�ڻ�����һ���ߵ��е㲻�ڶ����temp���⻷���ڲ�
				{
					MessageBoxA(0,"�����A�����ڻ����⻷�ⲿ�������¹��죡","����",MB_OK);
					pDoc->m_a.mb_clear();//���
					if(!pDoc->m_a.m_pointArray.size()&&!pDoc->m_b.m_pointArray.size())
					{
						pDoc->mb_initData();
					}
					return false;
				}
			}
		}
	}

	//�ж϶����B�Ƿ�Ϸ�
	polyR = pDoc->m_b.m_regionArray.size();
	for(int i=0; i<polyR; i++)//�Զ���ε�ÿ�����򵥶����
	{
		temp.mb_clear();
		temp.m_pointArray = pDoc->m_b.m_pointArray;
		temp.m_regionArray.push_back(pDoc->m_b.m_regionArray[i]);
		temp.m_regionArray[0].m_loopArray.clear();
		temp.m_regionArray[0].m_loopArray.push_back(pDoc->m_b.m_regionArray[i].m_loopArray[0]);//���������е��⻷��Ϊһ�������Ķ����temp
		polyL = pDoc->m_b.m_regionArray[i].m_loopArray.size();
		for(int j=1; j<polyL; j++)//�����ж�ÿ���ڻ��ǲ������⻷(�����temp)�ڲ�
		{
			polyP = pDoc->m_b.m_regionArray[i].m_loopArray[j].m_pointIDArray.size();
			for(int k=0; k<polyP; k++)
			{
				int u1,u2;//�߶˵����
				CP_Point middle;//���е�
				u1 = pDoc->m_b.m_regionArray[i].m_loopArray[j].m_pointIDArray[k];
				u2 = pDoc->m_b.m_regionArray[i].m_loopArray[j].m_pointIDArray[(k+1)%polyP];
				middle.m_x = (pDoc->m_b.m_pointArray[u1].m_x+pDoc->m_b.m_pointArray[u2].m_x)/2;
				middle.m_y = (pDoc->m_b.m_pointArray[u1].m_y+pDoc->m_b.m_pointArray[u2].m_y)/2;
				if(!PointInPoligon(middle,temp))//�ڻ�����һ���ߵ��е㲻�ڶ����temp���⻷���ڲ�
				{
					MessageBoxA(0,"�����B�����ڻ����⻷�ⲿ�������¹��죡","����",MB_OK);
					pDoc->m_b.mb_clear();//���
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

/*�������ͬһ�������е��ڻ��Ƿ���ڰ�����ϵ
--���ڻ���Ϊһ�������Ķ���Σ���˳��Ӧ�ò��ÿ��ǣ���Ϊֻ��Ҫ�жϽ����������
  �ж������ڻ����бߵ��е��Ƿ����������������ж������Ƿ���ڰ�����ϵ*/
bool InInLoop(CCP_PolygonPlatformDoc* pDoc)
{
	CP_Polygon temp;//��ÿ�������ÿ���ڻ���Ϊһ�������Ķ����
	int polyR, polyL, polyP;//�ֱ��ʾ����������򡢻���������Ĵ�С

	//�ж϶����A�Ƿ�Ϸ�
	polyR = pDoc->m_a.m_regionArray.size();
	for(int i=0; i<polyR; i++)//�Զ���ε�ÿ�����򵥶����
	{
		polyL = pDoc->m_a.m_regionArray[i].m_loopArray.size();
		for(int j=1; j<polyL; j++)//���������ڵ�ÿ���ڻ���Ϊһ�������Ķ����
		{
			temp.mb_clear();
			temp.m_pointArray = pDoc->m_a.m_pointArray;
			temp.m_regionArray.push_back(pDoc->m_a.m_regionArray[i]);
			temp.m_regionArray[0].m_loopArray.clear();
			temp.m_regionArray[0].m_loopArray.push_back(pDoc->m_a.m_regionArray[i].m_loopArray[j]);//�����ڻ���Ϊһ�������Ķ����temp
			for(int k=1; k<polyL; k++)//�����ڻ��Ƿ�����������ڻ�
			{
				if(j==k)//�������Լ��İ������
				{
					continue;
				}
				polyP = pDoc->m_a.m_regionArray[i].m_loopArray[k].m_pointIDArray.size();
				for(int p=0; p<polyP; p++)
				{
					int u1,u2;//�߶˵����
					CP_Point middle;//���е�
					u1 = pDoc->m_a.m_regionArray[i].m_loopArray[k].m_pointIDArray[p];
					u2 = pDoc->m_a.m_regionArray[i].m_loopArray[k].m_pointIDArray[(p+1)%polyP];
					middle.m_x = (pDoc->m_a.m_pointArray[u1].m_x+pDoc->m_a.m_pointArray[u2].m_x)/2;
					middle.m_y = (pDoc->m_a.m_pointArray[u1].m_y+pDoc->m_a.m_pointArray[u2].m_y)/2;
					if(PointInPoligon(middle,temp))//�ڻ�����һ���ߵ��е��ڶ����temp����һ���ڻ����ڲ�
					{
						MessageBoxA(0,"�����A�����ڻ��������ڻ��������¹��죡","����",MB_OK);
						pDoc->m_a.mb_clear();//���
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

	//�ж϶����B�Ƿ�Ϸ�
	polyR = pDoc->m_b.m_regionArray.size();
	for(int i=0; i<polyR; i++)//�Զ���ε�ÿ�����򵥶����
	{
		polyL = pDoc->m_b.m_regionArray[i].m_loopArray.size();
		for(int j=1; j<polyL; j++)//���������ڵ�ÿ���ڻ���Ϊһ�������Ķ����
		{
			temp.mb_clear();
			temp.m_pointArray = pDoc->m_b.m_pointArray;
			temp.m_regionArray.push_back(pDoc->m_b.m_regionArray[i]);
			temp.m_regionArray[0].m_loopArray.clear();
			temp.m_regionArray[0].m_loopArray.push_back(pDoc->m_b.m_regionArray[i].m_loopArray[j]);//�����ڻ���Ϊһ�������Ķ����temp
			for(int k=1; k<polyL; k++)//�����ڻ��Ƿ�����������ڻ�
			{
				if(j==k)//�������Լ��İ������
				{
					continue;
				}
				polyP = pDoc->m_b.m_regionArray[i].m_loopArray[k].m_pointIDArray.size();
				for(int p=0; p<polyP; p++)
				{
					int u1,u2;//�߶˵����
					CP_Point middle;//���е�
					u1 = pDoc->m_b.m_regionArray[i].m_loopArray[k].m_pointIDArray[p];
					u2 = pDoc->m_b.m_regionArray[i].m_loopArray[k].m_pointIDArray[(p+1)%polyP];
					middle.m_x = (pDoc->m_b.m_pointArray[u1].m_x+pDoc->m_b.m_pointArray[u2].m_x)/2;
					middle.m_y = (pDoc->m_b.m_pointArray[u1].m_y+pDoc->m_b.m_pointArray[u2].m_y)/2;
					if(PointInPoligon(middle,temp))//�ڻ�����һ���ߵ��е��ڶ����temp����һ���ڻ����ڲ�
					{
						MessageBoxA(0,"�����B�����ڻ��������ڻ��������¹��죡","����",MB_OK);
						pDoc->m_b.mb_clear();//���
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

/*���ͬһ���������һ��������һ��������ȫ���������
--��һ��������Ϊһ�������Ķ���Σ�
  �ж�����������⻷�����бߵ��е��Ƿ����������������ж������Ƿ���ڰ�����ϵ*/
bool InInRegion(CCP_PolygonPlatformDoc* pDoc)
{
	CP_Polygon temp;//��ÿ������ε�ÿ��������Ϊһ�������Ķ����
	int polyR, polyL, polyP;//�ֱ��ʾ����������򡢻���������Ĵ�С

	//�������A
	polyR = pDoc->m_a.m_regionArray.size();
	for(int i=0; i<polyR; i++)
	{
		temp.mb_clear();
		temp.m_pointArray = pDoc->m_a.m_pointArray;
		temp.m_regionArray.push_back(pDoc->m_a.m_regionArray[i]);//����������Ϊһ�������Ķ����temp
		for(int j=0; j<polyR; j++)
		{
			if(i==j)//��������������İ�����
			{
				continue;
			}
			polyP = pDoc->m_a.m_regionArray[j].m_loopArray[0].m_pointIDArray.size();
			for(int k=0; k<polyP; k++)
			{
				int u1,u2;//�߶˵����
				CP_Point middle;//���е�
				u1 = pDoc->m_a.m_regionArray[j].m_loopArray[0].m_pointIDArray[k];
				u2 = pDoc->m_a.m_regionArray[j].m_loopArray[0].m_pointIDArray[(k+1)%polyP];
				middle.m_x = (pDoc->m_a.m_pointArray[u1].m_x+pDoc->m_a.m_pointArray[u2].m_x)/2;
				middle.m_y = (pDoc->m_a.m_pointArray[u1].m_y+pDoc->m_a.m_pointArray[u2].m_y)/2;
				if(PointInPoligon(middle,temp))//�⻷����һ���ߵ��е��ڶ����temp����һ�������ڲ�
				{
					MessageBoxA(0,"�����A���������������һ�����������¹��죡","����",MB_OK);
					pDoc->m_a.mb_clear();//���
					if(!pDoc->m_a.m_pointArray.size()&&!pDoc->m_b.m_pointArray.size())
					{
						pDoc->mb_initData();
					}
					return false;
				}
			}
		}
	}

	//�������B
	polyR = pDoc->m_b.m_regionArray.size();
	for(int i=0; i<polyR; i++)
	{
		temp.mb_clear();
		temp.m_pointArray = pDoc->m_b.m_pointArray;
		temp.m_regionArray.push_back(pDoc->m_b.m_regionArray[i]);//����������Ϊһ�������Ķ����temp
		for(int j=0; j<polyR; j++)
		{
			if(i==j)//��������������İ�����
			{
				continue;
			}
			polyP = pDoc->m_b.m_regionArray[j].m_loopArray[0].m_pointIDArray.size();
			for(int k=0; k<polyP; k++)
			{
				int u1,u2;//�߶˵����
				CP_Point middle;//���е�
				u1 = pDoc->m_b.m_regionArray[j].m_loopArray[0].m_pointIDArray[k];
				u2 = pDoc->m_b.m_regionArray[j].m_loopArray[0].m_pointIDArray[(k+1)%polyP];
				middle.m_x = (pDoc->m_b.m_pointArray[u1].m_x+pDoc->m_b.m_pointArray[u2].m_x)/2;
				middle.m_y = (pDoc->m_b.m_pointArray[u1].m_y+pDoc->m_b.m_pointArray[u2].m_y)/2;
				if(PointInPoligon(middle,temp))//�⻷����һ���ߵ��е��ڶ����temp����һ�������ڲ�
				{
					MessageBoxA(0,"�����B���������������һ�����������¹��죡","����",MB_OK);
					pDoc->m_b.mb_clear();//���
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
	// TODO: �ڴ���������������
	CCP_PolygonPlatformDoc* pDoc = GetDocument();//��ȡDoc��ָ��
	if(!pDoc->m_a.m_pointArray.size() && !pDoc->m_b.m_pointArray.size())
	{
		return;
	}

	//���������ķ���(1)
	if(!LoopDirection(pDoc))
	{
		Invalidate(true);//���»���
		return;
	}
	//���һ��������е����б�����֮����û�г��˶˵�����������㣬�������غϱ�(2,5)
	else if(!EdgeIntersection(pDoc))
	{
		Invalidate(true);//���»���
		return;
	}
	//�������ͬһ�����е��ڻ��Ƿ��ڸ�������⻷��
	else if(!InOutLoop(pDoc))
	{
		Invalidate(true);//���»���
		return;
	}
	//�������ͬһ�����е��ڻ��Ƿ�������ĳ���ڻ���
	else if(!InInLoop(pDoc))
	{
		Invalidate(true);//���»���
		return;
	}
	//���ͬһ����������Ƿ����һ����������һ��������ȫ���������
	else if(!InInRegion(pDoc))
	{
		Invalidate(true);
		return;
	}

	//���Ϸ�
	MessageBox("����ξ�Ϊ�Ϸ�����Σ��������������","��ʾ",MB_OK);
}
