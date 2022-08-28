#include "gtest/gtest.h"
#include "../SharedPtr/SharedPtr.hpp"

#include <crtdbg.h> // windowsmen only

class MemoryLeaksTests : public::testing::Test
{
protected:
	void SetUp(void) override
	{
		_CrtMemCheckpoint(&startup);
	}

	void TearDown(void) override
	{
		_CrtMemState teardown, diff;
		_CrtMemCheckpoint(&teardown);
		ASSERT_EQ(0, _CrtMemDifference(&diff, &startup, &teardown)) << "Memory leaks detected";
	}

	_CrtMemState startup{};
};

struct A
{
	int i{};
	double d{};
};

class Constructor : public MemoryLeaksTests {};

TEST_F(Constructor, Default)
{
	SharedPTR<int> sp;
	EXPECT_EQ(sp.operator->(), nullptr);
	EXPECT_EQ(sp.reference_count(), 0);
	EXPECT_EQ(sp.operator bool(), false);
}

TEST_F(Constructor, WithParameters)
{
	char* p = new char;
	SharedPTR<char> sp(p);
	EXPECT_EQ(sp.operator->(), p);
	EXPECT_EQ(sp.reference_count(), 1);
	EXPECT_EQ(sp.operator bool(), true);
}

TEST_F(Constructor, Move)
{
	A* p = new A{ 0, 0.0 };
	SharedPTR<A> sp1(p);
	SharedPTR<A> sp2(std::move(sp1));

	EXPECT_EQ(sp1.operator->(), nullptr);
	EXPECT_EQ(sp1.reference_count(), 0);
	EXPECT_EQ(sp1.operator bool(), false);

	EXPECT_EQ(sp2.operator->(), p);
	EXPECT_EQ(sp2->i, 0);
	EXPECT_EQ(sp2->d, 0.0);
	EXPECT_EQ(sp2.reference_count(), 1);
	EXPECT_EQ(sp2.operator bool(), true);
}
//
TEST_F(Constructor, Copy)
{
	int* p = new int;
	SharedPTR<int> sp1(p);
	SharedPTR<int> sp2(sp1);

	EXPECT_EQ(sp1.operator->(), p);
	EXPECT_EQ(sp1.reference_count(), 2);
	EXPECT_EQ(sp1.operator bool(), true);

	EXPECT_EQ(sp2.operator->(), p);
	EXPECT_EQ(sp2.reference_count(), 2);
	EXPECT_EQ(sp2.operator bool(), true);
}

TEST(DereferenceOperator, ThrowNullptr)
{
	SharedPTR<int> sp;
	EXPECT_THROW(*sp, std::out_of_range);
}

TEST(DereferenceOperator, NoThrow)
{
	double* p = new double;
	SharedPTR<double> sp(p);
	EXPECT_NO_THROW(*sp);
	EXPECT_EQ(*sp, *p);
}

TEST(OperatorArrow, Nullptr)
{
	SharedPTR<int> sp;
	EXPECT_EQ(sp.operator->(), nullptr);
}

TEST(OperatorArrow, NotNullptr)
{
	int* p = new int;
	SharedPTR<int> sp(p);
	EXPECT_EQ(sp.operator->(), p);
}

TEST(OperatorBool, Nullptr)
{
	SharedPTR<int> sp;
	EXPECT_EQ(sp.operator bool(), false);
}

TEST(OperatorBool, NotNullptr)
{
	int* p = new int;
	SharedPTR<int> sp(p);
	EXPECT_EQ(sp.operator bool(), true);
}

TEST(Get, Nullptr)
{
	SharedPTR<int> sp;
	EXPECT_EQ(sp.get(), nullptr);
}

TEST(Get, Ptr)
{
	A* p = new A;
	SharedPTR<A> sp(p);
	EXPECT_EQ(sp.get(), p);
}

TEST(ReferenceCount, Nullptr)
{
	SharedPTR<int> sp;
	EXPECT_EQ(sp.reference_count(), 0);
}

TEST(ReferenceCount, OneShared)
{
	int* p = new int;
	SharedPTR<int> sp(p);
	EXPECT_EQ(sp.reference_count(), 1);
}

TEST(ReferenceCount, ManyShared)
{
	int* p = new int;
	SharedPTR<int> sp1(p);
	SharedPTR<int> sp2(sp1);
	SharedPTR<int> sp3(sp2);
	SharedPTR<int> sp4(sp3);

	EXPECT_EQ(sp1.get(), p);
	EXPECT_EQ(sp2.get(), p);
	EXPECT_EQ(sp3.get(), p);
	EXPECT_EQ(sp4.get(), p);

	EXPECT_EQ(sp1.reference_count(), 4);
	EXPECT_EQ(sp2.reference_count(), 4);
	EXPECT_EQ(sp3.reference_count(), 4);
	EXPECT_EQ(sp4.reference_count(), 4);
}

class AssignmentOperator : public MemoryLeaksTests {};

TEST_F(AssignmentOperator, Pointer)
{
	int* p = new int;
	SharedPTR<int> sp;
	sp = p;

	EXPECT_EQ(sp.get(), p);
	EXPECT_EQ(sp.reference_count(), 1);
}

TEST_F(AssignmentOperator, Lvalue_SameShared)
{
	int* p = new int;
	SharedPTR<int> sp(p);
	sp = sp;

	EXPECT_EQ(sp.get(), p);
	EXPECT_EQ(sp.reference_count(), 1);
}

TEST_F(AssignmentOperator, Rvalue_SameShared)
{
	int* p = new int;
	SharedPTR<int> sp(p);
	sp = std::move(sp);

	EXPECT_EQ(sp.get(), p);
	EXPECT_EQ(sp.reference_count(), 1);
}

TEST_F(AssignmentOperator, Lvalue_OtherShared)
{
	A* p = new A{ 1, 1.0 };
	SharedPTR<A> sp1(p);
	SharedPTR<A> sp2;
	sp2 = sp1;

	EXPECT_EQ(sp1.get(), p);
	EXPECT_EQ(sp1->i, 1);
	EXPECT_EQ(sp1->d, 1.0);
	EXPECT_EQ(sp1.reference_count(), 2);

	EXPECT_EQ(sp2.get(), p);
	EXPECT_EQ(sp2->i, 1);
	EXPECT_EQ(sp2->d, 1.0);
	EXPECT_EQ(sp2.reference_count(), 2);
}

TEST_F(AssignmentOperator, Rvalue_OtherShared)
{
	A* p = new A{ 1, 1.0 };
	SharedPTR<A> sp1(p);
	SharedPTR<A> sp2;
	sp2 = std::move(sp1);

	EXPECT_EQ(sp1.get(), nullptr);
	EXPECT_EQ(sp1.reference_count(), 0);

	EXPECT_EQ(sp2.get(), p);
	EXPECT_EQ(sp2->i, 1);
	EXPECT_EQ(sp2->d, 1.0);
	EXPECT_EQ(sp2.reference_count(), 1);
}

class Release : public MemoryLeaksTests {};

TEST_F(Release, Nullptr)
{
	SharedPTR<int> sp;

	EXPECT_EQ(sp.get(), nullptr);
	EXPECT_EQ(sp.reference_count(), 0);

	sp.release();

	EXPECT_EQ(sp.get(), nullptr);
	EXPECT_EQ(sp.reference_count(), 0);
}

TEST_F(Release, OneShared)
{
	int* p = new int;
	SharedPTR<int> sp(p);

	EXPECT_EQ(sp.get(), p);
	EXPECT_EQ(sp.reference_count(), 1);

	sp.release();

	EXPECT_EQ(sp.get(), nullptr);
	EXPECT_EQ(sp.reference_count(), 0);
}

TEST_F(Release, ManyShared)
{
	char* p = new char;
	SharedPTR<char> sp1(p);
	SharedPTR<char> sp2 = sp1;
	SharedPTR<char> sp3 = sp2;

	sp1.release();

	EXPECT_EQ(sp1.get(), nullptr);
	EXPECT_EQ(sp2.get(), p);
	EXPECT_EQ(sp3.get(), p);

	EXPECT_EQ(sp1.reference_count(), 0);
	EXPECT_EQ(sp2.reference_count(), 2);
	EXPECT_EQ(sp3.reference_count(), 2);

	sp3.release();

	EXPECT_EQ(sp1.get(), nullptr);
	EXPECT_EQ(sp2.get(), p);
	EXPECT_EQ(sp3.get(), nullptr);

	EXPECT_EQ(sp1.reference_count(), 0);
	EXPECT_EQ(sp2.reference_count(), 1);
	EXPECT_EQ(sp3.reference_count(), 0);
}

class Reset : public MemoryLeaksTests {};

TEST_F(Reset, NullptrToNullptr)
{
	SharedPTR<int> sp;
	EXPECT_NO_THROW(sp.reset());
	EXPECT_EQ(sp.get(), nullptr);
	EXPECT_EQ(sp.reference_count(), 0);
}

TEST_F(Reset, NullptrToPtr)
{
	SharedPTR<int> sp;
	int* p = new int;
	EXPECT_NO_THROW(sp.reset(p));
	EXPECT_EQ(sp.get(), p);
	EXPECT_EQ(sp.reference_count(), 1);
}

TEST_F(Reset, PtrToOtherPtr)
{
	int* p1 = new int;
	int* p2 = new int;
	SharedPTR<int> sp(p1);
	EXPECT_NO_THROW(sp.reset(p2));
	EXPECT_EQ(sp.get(), p2);
	EXPECT_EQ(sp.reference_count(), 1);
}

TEST_F(Reset, PtrToSamePtr)
{
	int* p = new int;
	SharedPTR<int> sp(p);
	EXPECT_THROW(sp.reset(p), std::out_of_range);
}

class Swap : public MemoryLeaksTests {};
 
TEST_F(Swap, PtrAndNullptr)
{
	A* p = new A{ 0, 0.0 };
	SharedPTR<A> sp1(p);
	SharedPTR<A> sp2;

	sp1.swap(sp2);

	EXPECT_EQ(sp1.get(), nullptr);
	EXPECT_EQ(sp1.reference_count(), 0);
	EXPECT_EQ(sp2.get(), p);
	EXPECT_EQ(sp2->i, 0);
	EXPECT_EQ(sp2->d, 0.0);
	EXPECT_EQ(sp2.reference_count(), 1);
}

TEST_F(Swap, PtrAndOtherPtr)
{
	int* p1 = new int(1);
	int* p2 = new int(-1);
	SharedPTR<int> sp1(p1);
	SharedPTR<int> sp2(p2);

	sp2.swap(sp1);

	EXPECT_EQ(sp1.get(), p2);
	EXPECT_EQ(*sp1, -1);
	EXPECT_EQ(sp1.reference_count(), 1);

	EXPECT_EQ(sp2.get(), p1);
	EXPECT_EQ(*sp2, 1);
	EXPECT_EQ(sp2.reference_count(), 1);
}

TEST_F(Swap, PtrAndSamePtr)
{
	int* p = new int;
	SharedPTR<int> sp(p);

	sp.swap(sp);

	EXPECT_EQ(sp.get(), p);
	EXPECT_EQ(sp.reference_count(), 1);
}

class Custom
{
public:
	Custom(int* _p1, double* _p2) : p1(_p1), p2(_p2) {};
	int* p1{};
	double* p2{};
};

class CustomDeleter
{
public:
	void operator()(Custom* p)
	{
		if (p != nullptr)
		{
			delete p->p1;
			delete p->p2;
		}
		delete p;
	}
};

class Deleter : public MemoryLeaksTests {};
 
TEST_F(Deleter, DefaultDeleter)
{
	int* p = new int;
	SharedPTR<int> sp(p);
	const std::default_delete<int>* deleter = sp.get_deleter();
	(*deleter)(new int);
}

TEST_F(Deleter, Custom_OneShared)
{
	Custom* custom = new Custom(new int{ 0 }, new double{ 0.0 });
	SharedPTR<Custom, CustomDeleter> sp(custom);
	EXPECT_EQ(*(sp->p1), 0);
	EXPECT_EQ(*(sp->p2), 0.0);
	*(sp->p2) = 1.0;
	EXPECT_EQ(*(sp->p2), 1.0);
}

TEST_F(Deleter, Custom_TwoShared)
{
	Custom* custom = new Custom(new int{ 0 }, new double{ 0.0 });
	SharedPTR<Custom, CustomDeleter> sp1(custom);
	SharedPTR<Custom, CustomDeleter> sp2(sp1);
}

TEST_F(Deleter, Custom_ManyShared)
{
	Custom* custom = new Custom(new int{ 0 }, new double{ 0.0 });
	SharedPTR<Custom, CustomDeleter> sp1(custom);
	SharedPTR<Custom, CustomDeleter> sp2(sp1);
	SharedPTR<Custom, CustomDeleter> sp3 = sp2;
	SharedPTR<Custom, CustomDeleter> sp4(std::move(sp3));
	SharedPTR<Custom, CustomDeleter> sp5 = std::move(sp4);
}