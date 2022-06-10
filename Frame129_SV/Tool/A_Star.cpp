// A_Star.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "A_Star.h"
#include "afxdialogex.h"


// CA_Star 대화 상자입니다.

IMPLEMENT_DYNAMIC(CA_Star, CDialog)

CA_Star::CA_Star(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_A_STAR, pParent)
{

}

CA_Star::~CA_Star()
{
}

void CA_Star::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CA_Star, CDialog)
END_MESSAGE_MAP()


// CA_Star 메시지 처리기입니다.

void CA_Star::A_Star_Cord(void)
{
	//int[] dirY = new int[] { -1, 0, 1, 0, -1, 1, 1, -1 };
	//int[] dirX = new int[] { 0, -1, 0, 1, -1, -1, 1, 1 };
	//int[] cost = new int[] { 10, 10, 10, 10, 14, 14, 14, 14};

	//bool[, ] closed = new bool[_board.Size, _board.Size];
	//int[, ] open = new int[_board.Size, _board.Size];
	//PriorityQueue<PQNode> pq = new PriorityQueue<PQNode>();
	//Pos[, ] parent = new Pos[_board.Size, _board.Size];

	//for (int y = 0; y < _board.Size; y++)
	//	for (int x = 0; x < _board.Size; x++)
	//		open[y, x] = Int32.MaxValue;

	////시작점 발견
	//open[PosY, PosX] = 10 * (Math.Abs(_board.DestY - PosY) + Math.Abs(_board.DestX - PosX));
	//pq.Push(new PQNode(){ F = 10 * (Math.Abs(_board.DestY - PosY) + Math.Abs(_board.DestX - PosX)), G = 0, Y = PosY, X = PosX });
	//parent[PosY, PosX] = new Pos(PosY, PosX);

	////제일 좋은 후보 찾기
	//while (pq.Count() > 0)
	//{
	//	PQNode node = pq.Pop();

	//	//이미 방문했으면 스킵
	//	if (closed[node.Y, node.X])
	//		continue;

	//	//f(n)이 가장 작은 노드 방문 완료
	//	closed[node.Y, node.X] = true;

	//	if (node.Y == _board.DestY && node.X == _board.DestX)
	//		break;

	//	for (int i = 0; i < dirY.Length; i++)
	//	{
	//		int nextY = node.Y + dirY[i];
	//		int nextX = node.X + dirX[i];

	//		//유효범위 체크
	//		if (nextX < 0 || nextX >= _board.Size || nextY < 0 || nextY >= _board.Size)
	//			continue;
	//		if (_board.Tile[nextY, nextX] == Board.TileType.Wall)
	//			continue;
	//		if (closed[nextY, nextX])
	//			continue;

	//		int g = node.G + cost[i];
	//		int h = 10 * (Math.Abs(_board.DestY - nextY) + Math.Abs(_board.DestX - nextX));

	//		if (open[nextY, nextX] < g + h)
	//			continue;

	//		open[nextY, nextX] = g + h;
	//		pq.Push(new PQNode(){ F = g + h, G = g, Y = nextY, X = nextX });
	//		parent[nextY, nextX] = new Pos(node.Y, node.X);
	//	}
	//}

	//CalcPathFromParent(parent);
}