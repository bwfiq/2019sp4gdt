#include "Grid.h"
#include <iostream>
#include "MyMath.h"
#include "Application.h"
#define SLEEPTIME 10

bool GridPt::operator==(const GridPt& rhs) const
{
	return (this->x == rhs.x && this->z == rhs.z);
}
bool GridPt::operator!=(const GridPt& rhs) const
{
	return (this->x != rhs.x || this->z != rhs.z);
}
Grid::Grid()
{
}

Grid::~Grid()
{
}

Grid::TILE_CONTENT Grid::See(GridPt tile)
{
	return m_grid[tile.z * m_size + tile.x];
}
void Grid::Generate(unsigned key, unsigned size, GridPt start, float wallLoad)
{
	m_grid.clear();
	if (size == 0)
		return;
	start.x = Math::Clamp(start.x, 0, (int)size - 1);
	start.z = Math::Clamp(start.z, 0, (int)size - 1);
	wallLoad = Math::Clamp(wallLoad, 0.f, 0.8f);
	unsigned total = size * size;
	m_grid.resize(total);
	std::fill(m_grid.begin(), m_grid.end(), TILE_EMPTY);
	unsigned startId = start.z * size + start.x;
	srand(key);
	for (int i = 0; i < (int)total * wallLoad;)
	{
		unsigned chosen = rand() % total;
		if (chosen == startId)
			continue;
		if (m_grid[chosen] == TILE_EMPTY)
		{
			m_grid[chosen] = TILE_USED;
			++i;
		}
	}
	std::cout << "Grid " << key << std::endl;
	for (int row = (int)size - 1; row >= 0; --row)
	{
		for (int col = 0; col < (int)size; ++col)
		{
			if (m_grid[row * size + col] == TILE_USED)
				std::cout << "1 ";
			else
				std::cout << "0 ";
		}
		std::cout << std::endl;
	}
	m_key = key;
	m_size = size;
	m_numMove = 0;
}

unsigned Grid::GetKey()
{
	return m_key;
}

unsigned Grid::GetSize()
{
	return m_size;
}

GridPt Grid::GetCurr()
{
	return m_curr;
}

int Grid::GetNumMove()
{
	return m_numMove;
}

void Grid::SetCurr(GridPt newCurr)
{
	//Application::GetInstance().Iterate();
	//Sleep(SLEEPTIME);
	++m_numMove;
	m_curr = newCurr;
}

void Grid::SetNumMove(int num)
{
	m_numMove = num;
}

bool Grid::Move(DIRECTION direction)
{
	Application::GetInstance().Iterate();
	Sleep(SLEEPTIME);
	++m_numMove;
	GridPt temp = m_curr;
	switch (direction)
	{
	case DIR_LEFT:
		if (temp.x == 0)
			return false;
		temp.x -= 1;
		break;
	case DIR_RIGHT:
		if (temp.x == (int)m_size - 1)
			return false;
		temp.x += 1;
		break;
	case DIR_UP:
		if (temp.z == (int)m_size - 1)
			return false;
		temp.z += 1;
		break;
	case DIR_DOWN:
		if (temp.z == 0)
			return false;
		temp.z -= 1;
		break;
	case DIR_LEFTUP:
		if (temp.z == (int)m_size - 1 || temp.x == 0)
			return false;
		temp.z += 1;
		temp.x -= 1;
		break;
	case DIR_LEFTDOWN:
		if (temp.z == 0 || temp.x == 0)
			return false;
		temp.z -= 1;
		temp.x -= 1;
		break;
	case DIR_RIGHTUP:
		if (temp.z == (int)m_size - 1 || temp.x == (int)m_size - 1)
			return false;
		temp.z += 1;
		temp.x += 1;
		break;
	case DIR_RIGHTDOWN:
		if (temp.z == 0 || temp.x == (int)m_size - 1)
			return false;
		temp.z -= 1;
		temp.x += 1;
		break;
	}
	
	int tempId = temp.z * m_size + temp.x;
	if (m_grid[tempId] == TILE_USED)
		return false;
	m_curr = temp;
	return true;
}
