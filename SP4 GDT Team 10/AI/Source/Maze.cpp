#include "Maze.h"
#include <iostream>
#include "MyMath.h"
#include "Application.h"
#define SLEEPTIME 10

bool MazePt::operator==(const MazePt& rhs) const
{
	return (this->x == rhs.x && this->y == rhs.y);
}
bool MazePt::operator!=(const MazePt& rhs) const
{
	return (this->x != rhs.x || this->y != rhs.y);
}
Maze::Maze()
{
}

Maze::~Maze()
{
}

Maze::TILE_CONTENT Maze::See(MazePt tile)
{
	return m_grid[tile.y * m_size + tile.x];
}
void Maze::Generate(unsigned key, unsigned size, MazePt start, float wallLoad)
{
	m_grid.clear();
	if (size == 0)
		return;
	start.x = Math::Clamp(start.x, 0, (int)size - 1);
	start.y = Math::Clamp(start.y, 0, (int)size - 1);
	wallLoad = Math::Clamp(wallLoad, 0.f, 0.8f);
	unsigned total = size * size;
	m_grid.resize(total);
	std::fill(m_grid.begin(), m_grid.end(), TILE_EMPTY);
	unsigned startId = start.y * size + start.x;
	srand(key);
	for (int i = 0; i < (int)total * wallLoad;)
	{
		unsigned chosen = rand() % total;
		if (chosen == startId)
			continue;
		if (m_grid[chosen] == TILE_EMPTY)
		{
			m_grid[chosen] = TILE_WALL;
			++i;
		}
	}
	std::cout << "Maze " << key << std::endl;
	for (int row = (int)size - 1; row >= 0; --row)
	{
		for (int col = 0; col < (int)size; ++col)
		{
			if (m_grid[row * size + col] == TILE_WALL)
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

unsigned Maze::GetKey()
{
	return m_key;
}

unsigned Maze::GetSize()
{
	return m_size;
}

MazePt Maze::GetCurr()
{
	return m_curr;
}

int Maze::GetNumMove()
{
	return m_numMove;
}

void Maze::SetCurr(MazePt newCurr)
{
	//Application::GetInstance().Iterate();
	//Sleep(SLEEPTIME);
	++m_numMove;
	m_curr = newCurr;
}

void Maze::SetNumMove(int num)
{
	m_numMove = num;
}

bool Maze::Move(DIRECTION direction)
{
	Application::GetInstance().Iterate();
	Sleep(SLEEPTIME);
	++m_numMove;
	MazePt temp = m_curr;
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
		if (temp.y == (int)m_size - 1)
			return false;
		temp.y += 1;
		break;
	case DIR_DOWN:
		if (temp.y == 0)
			return false;
		temp.y -= 1;
		break;
	case DIR_LEFTUP:
		if (temp.y == (int)m_size - 1 || temp.x == 0)
			return false;
		temp.y += 1;
		temp.x -= 1;
		break;
	case DIR_LEFTDOWN:
		if (temp.y == 0 || temp.x == 0)
			return false;
		temp.y -= 1;
		temp.x -= 1;
		break;
	case DIR_RIGHTUP:
		if (temp.y == (int)m_size - 1 || temp.x == (int)m_size - 1)
			return false;
		temp.y += 1;
		temp.x += 1;
		break;
	case DIR_RIGHTDOWN:
		if (temp.y == 0 || temp.x == (int)m_size - 1)
			return false;
		temp.y -= 1;
		temp.x += 1;
		break;
	}
	
	int tempId = temp.y * m_size + temp.x;
	if (m_grid[tempId] == TILE_WALL)
		return false;
	m_curr = temp;
	return true;
}
