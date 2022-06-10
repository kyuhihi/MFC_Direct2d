// A_Star.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "A_Star.h"
#include "afxdialogex.h"


// CA_Star ��ȭ �����Դϴ�.

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


// CA_Star �޽��� ó�����Դϴ�.

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

	////������ �߰�
	//open[PosY, PosX] = 10 * (Math.Abs(_board.DestY - PosY) + Math.Abs(_board.DestX - PosX));
	//pq.Push(new PQNode(){ F = 10 * (Math.Abs(_board.DestY - PosY) + Math.Abs(_board.DestX - PosX)), G = 0, Y = PosY, X = PosX });
	//parent[PosY, PosX] = new Pos(PosY, PosX);

	////���� ���� �ĺ� ã��
	//while (pq.Count() > 0)
	//{
	//	PQNode node = pq.Pop();

	//	//�̹� �湮������ ��ŵ
	//	if (closed[node.Y, node.X])
	//		continue;

	//	//f(n)�� ���� ���� ��� �湮 �Ϸ�
	//	closed[node.Y, node.X] = true;

	//	if (node.Y == _board.DestY && node.X == _board.DestX)
	//		break;

	//	for (int i = 0; i < dirY.Length; i++)
	//	{
	//		int nextY = node.Y + dirY[i];
	//		int nextX = node.X + dirX[i];

	//		//��ȿ���� üũ
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