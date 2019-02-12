#ifndef GRID_H
#define GRID_H

#include <vector>
#include "Vector3.h"

struct GridPt
{
	int x, z;

	GridPt(int _x = 0, int _z = 0) : x(_x), z(_z) {}
	void Set(int _x = 0, int _z = 0) { x = _x; z = _z;}

	bool operator==(const GridPt& rhs) const; //Equal check
	bool operator!=(const GridPt& rhs) const; //InEqual check

};

class Grid
{
public:
	enum TILE_CONTENT
	{
		TILE_EMPTY = 0,
		TILE_USED,

		TILE_TOTAL //Should not have tiles below this
	};
	enum DIRECTION
	{
		DIR_UP = 0,
		DIR_DOWN,
		DIR_LEFT,
		DIR_RIGHT,
		DIR_LEFTUP,
		DIR_LEFTDOWN,
		DIR_RIGHTUP,
		DIR_RIGHTDOWN,
	};
	Grid();
	~Grid();
	void Generate(unsigned key, unsigned size, GridPt start, float wallLoad);
	bool Move(DIRECTION direction);
	unsigned GetKey();
	unsigned GetSize();
	GridPt GetCurr();
	int GetNumMove();
	void SetCurr(GridPt newCurr);
	void SetNumMove(int num);

	TILE_CONTENT See(GridPt tile);
	std::vector<TILE_CONTENT> m_grid;
private:
	//Do not make these public
	unsigned m_key;
	unsigned m_size;

	GridPt m_curr;
	int m_numMove;
};

int GetGridIndex(int gridX, int gridZ);

int GetGridIndex(GridPt pt);

bool isPointInGrid(GridPt pt);

std::pair<int, int> GetPoint(int index);

Vector3 GetGridPos(GridPt pt);

GridPt GetPoint(Vector3 pos);
#endif