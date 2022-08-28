#include <vector>
#include <iostream>
#include "ObjectPool.hpp"

class Point
{
public:
	Point()					// конструктор без параметров
		: m_x(0), m_y(0) {}

	Point(int x, int y)		// конструктор с параметрами
		: m_x(x), m_y(y) {}

	void print()
	{
		std::cout << m_x << std::endl;
		std::cout << m_y << std::endl;
	}

	~Point() {}

private:
	int m_x, m_y;
};

int main()
{
	ObjectPool<Point> pp{};

	Point& point1 = pp.alloc();
	std::cout << "Point 1:" << std::endl;
	point1.print();

	Point& point2 = pp.alloc(10, 10);
	std::cout << "Point 2:" << std::endl;
	point2.print();

	return 0;
}