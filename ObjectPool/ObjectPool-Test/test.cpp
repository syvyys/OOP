#include "gtest/gtest.h"
#include "memleak_global/gtest-memleak.h"
#include "../ObjectPool/ObjectPool.hpp"

class Point
{
public:
	Point() : x(0), y(0) {}
	Point(int _x, int _y) : x(_x), y(_y) {}
	int get_x()
	{
		return x;
	}
	int get_y()
	{
		return y;
	}
	~Point() {}
private:
	int x, y;
};

class A
{
public:
	A() = delete;
	A(int _a) : a(_a) {};
private:
	int a;
};

struct P
{
	int i;
	double d;
};

TEST(PoolConstructor, Default)
{
	ObjectPool<Point> pp;
	for (int i = 0; i < default_size; ++i)
	{
		EXPECT_NO_THROW(pp.alloc());
	}
	EXPECT_THROW(pp.alloc(), std::out_of_range);
}

TEST(PoolConstructor, NotDefault)
{
	static const size_t size = 2;
	ObjectPool<Point> pp(size);
	for (int i = 0; i < size; ++i)
	{
		EXPECT_NO_THROW(pp.alloc());
	}
	EXPECT_THROW(pp.alloc(), std::out_of_range);
}

TEST(Alloc, PODType)
{
	ObjectPool<P> pp;
	P& p = pp.alloc(1, 1.0);
	EXPECT_EQ(p.i, 1);
	EXPECT_EQ(p.d, 1.0);
}

TEST(Alloc, DefaultConstructor) // by constuctor I mean object's constructor, not pool's
{
	ObjectPool<Point> pp;
	Point& point = pp.alloc();
	EXPECT_EQ(point.get_x(), 0);
	EXPECT_EQ(point.get_y(), 0);
}

TEST(Alloc, ConstructorWithParameters)
{
	ObjectPool<Point> pp;
	Point& point = pp.alloc(1, -1);
	EXPECT_EQ(point.get_x(), 1);
	EXPECT_EQ(point.get_y(), -1);
}

TEST(Alloc, CopyConstructor)
{
	ObjectPool<Point> pp;
	Point point1(10, 11);
	Point& point2 = pp.alloc(point1);
	EXPECT_EQ(point2.get_x(), 10);
	EXPECT_EQ(point2.get_y(), 11);
}

TEST(Alloc, NoDefaultConstructor)
{
	ObjectPool<A> pp;
	//A& a1 = pp.alloc();	// not working without default constructor
	A& a2 = pp.alloc(10);
}

TEST(Alloc, DifferentAllocation)
{
	ObjectPool<P> pool(30);
	std::vector<P*> objects;

	for (int i = 0; i < 20; ++i)
	{
		EXPECT_NO_THROW(objects.push_back(&pool.alloc()));
	}

	for (int i = 0; i < 10; ++i)
	{
		EXPECT_NO_THROW(pool.free(*objects[2 * i]));
	}

	for (int i = 0; i < 10; ++i)
	{
		EXPECT_NO_THROW(objects[2 * i] = &pool.alloc());
	}

	for (int i = 0; i < 10; ++i)
	{
		EXPECT_NO_THROW(objects.push_back(&pool.alloc()));
	}
}

TEST(Alloc, NoThrow)
{
	ObjectPool<Point> pp(10);
	std::vector<Point*> points;

	for (int i = 0; i < 10; ++i)
	{
		EXPECT_NO_THROW(points.push_back(&pp.alloc()));
	}

	for (int i = 0; i < 5; ++i)
	{
		EXPECT_NO_THROW(pp.free(*points[i]));
	}

	for (int i = 0; i < 5; ++i)
	{
		EXPECT_NO_THROW(pp.alloc());
	}
}

TEST(Alloc, OutOfRangeThrow)
{
	static const size_t size = 1;
	ObjectPool<Point> pp(size);
	Point& point = pp.alloc();
	EXPECT_THROW(pp.alloc(), std::out_of_range);
}

TEST(Alloc, HowManyAreStillFree)
{
	ObjectPool<Point> pp(10);
	for (int i = 0; i < 10; ++i)
	{
		pp.alloc();
		EXPECT_EQ(pp.num_free(), 10 - i - 1);
	}
}

TEST(Free, OneObject)
{
	ObjectPool<Point> pp(10);
	Point& point = pp.alloc();
	EXPECT_NO_THROW(pp.free(point));
	EXPECT_EQ(pp.num_free(), 10);
}

TEST(Free, AllObjects)
{
	ObjectPool<Point> pp(10);
	std::vector<std::reference_wrapper<Point>> points;
	for (int i = 0; i < 10; ++i)
	{
		points.push_back(pp.alloc());
	}

	for (int i = 0; i < 10; ++i)
	{
		pp.free(points[i]);
		EXPECT_EQ(pp.num_free(), i + 1);
	}
}

TEST(Free, NoThrow)
{
	ObjectPool<Point> pp(10);
	std::vector<Point*> points;

	for (int i = 0; i < 10; ++i)
	{
		points.push_back(&pp.alloc());
	}

	for (int i = 0; i < 10; ++i)
	{
		EXPECT_NO_THROW(pp.free(*points[i]));
	}
}

TEST(Free, FreedObject)
{
	ObjectPool<Point> pp(3);
	Point& point1 = pp.alloc();
	Point& point2 = pp.alloc();
	Point& point3 = pp.alloc();
	pp.free(point2);
	EXPECT_THROW(pp.free(point2), std::out_of_range);
}

TEST(Free, NoSuchObject)
{
	ObjectPool<Point> pp(1);
	Point& point1 = pp.alloc();
	Point point2(10, 10);
	EXPECT_THROW(pp.free(point2), std::out_of_range);
}

TEST(Free, DirectOrder)
{
	ObjectPool<P> pool(30);
	std::vector<std::reference_wrapper<P>> objects;

	for (int i = 0; i < 30; ++i)
	{
		EXPECT_NO_THROW(objects.push_back(pool.alloc()));
	}

	for (int i = 0; i < 30; ++i)
	{
		EXPECT_NO_THROW(pool.free(objects[i]));
	}
}

TEST(Free, ReverseOrder)
{
	ObjectPool<P> pool(30);
	std::vector<std::reference_wrapper<P>> objects;

	for (int i = 0; i < 30; ++i)
	{
		EXPECT_NO_THROW(objects.push_back(pool.alloc()));
	}

	for (int i = 29; i >= 0; --i)
	{
		EXPECT_NO_THROW(pool.free(objects[i]));
	}
}

TEST(Free, PseudorandomOrder)
{
	ObjectPool<P> pool(40);
	std::vector<std::reference_wrapper<P>> objects;

	for (int i = 0; i < 40; ++i)
	{
		EXPECT_NO_THROW(objects.push_back(pool.alloc()));
	}

	for (int i = 0; i < 10; ++i) // 0, 2, 4, 8, ..., 18
	{
		EXPECT_NO_THROW(pool.free(objects[2 * i]));
	}

	for (int i = 0; i < 10; ++i) // 38, 36, 34, ..., 20
	{
		EXPECT_NO_THROW(pool.free(objects[40 - 2 * i - 2]));
	}

	for (int i = 0; i < 10; ++i) // 21, 23, 25, ..., 39
	{
		EXPECT_NO_THROW(pool.free(objects[20 + 2 * i + 1]));
	}

	for (int i = 0; i < 10; ++i) // 19, 17, 15, ..., 1
	{
		EXPECT_NO_THROW(pool.free(objects[20 - 2 * i - 1]));
	}
}