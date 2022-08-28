#include "gtest/gtest.h"
#include "../BMPSize/BitmapProxy.hpp"
#include "../BMPSize/libbitmap-master/libbitmap.h"

/* empty */
std::string empty_path = "";	// does not exist

/* exists */
std::string formatted1 = "test_images/test1_100x100.bmp";	// 100 x 100
std::string not_formatted1 = "test_images/test1.bmp";		// 100 x 100				

/* does not exist */
std::string formatted2 = "test_images/test2_1000x1000.bmp";	// we don't need it to exist
std::string not_formatted2 = "test_images/test2.bmp";
std::string formatted_wrongly1 = "test_images/test3.100x100.bmp";
std::string formatted_wrongly2 = "test_images/test3_100X100.bmp";
std::string formatted_wrongly3 = "test_images/test3_100x5a5.bmp";
std::string formatted_wrongly4 = "test_images/test3_x.bmp";

/* with or without .bmp extension */
std::string with_bmp = "test_images/test4.bmp";	// exists
std::string without_bmp = "test_images/test4";	// exists

using Size = std::pair<size_t, size_t>;

TEST(Constructor, Default)
{
	BitmapProxy proxy;
	Size size = { 0, 0 };
	EXPECT_EQ(proxy.get_size(), size);
	EXPECT_EQ(proxy.is_ready(), true);
}

TEST(Load, EmptyPath)
{
	BitmapProxy proxy;
	proxy.load(empty_path);
	EXPECT_EQ(proxy.is_ready(), false);
}

TEST(Load, FileExists)
{
	BitmapProxy proxy;
	proxy.load(formatted1);
	EXPECT_EQ(proxy.is_ready(), false);
}

TEST(Load, FileNotExist)
{
	BitmapProxy proxy;
	proxy.load(formatted2);
	EXPECT_EQ(proxy.is_ready(), false);
}

TEST(Load, ConstString)
{
	BitmapProxy proxy;
	proxy.load("bmp_2x4.bmp");
	EXPECT_EQ(proxy.is_ready(), false);
}

/* get_size tests */

TEST(Formatted, Exist)
{
	BitmapProxy proxy;
	proxy.load(formatted1);
	Size size = { 100, 100 };
	EXPECT_EQ(proxy.get_size(), size);
	EXPECT_EQ(proxy.is_ready(), true);
}

TEST(Formatted, NotExist)
{
	BitmapProxy proxy;
	proxy.load(formatted2);
	Size size = { 1000, 1000 };
	EXPECT_EQ(proxy.get_size(), size);
	EXPECT_EQ(proxy.is_ready(), true);
}

TEST(NotFormatted, Exist)
{
	BitmapProxy proxy;
	proxy.load(not_formatted1);
	Size size = { 100, 100 };
	EXPECT_EQ(proxy.get_size(), size);
	EXPECT_EQ(proxy.is_ready(), true);
}

TEST(NotFormatted, NotExist)
{
	BitmapProxy proxy;
	proxy.load(not_formatted2);
	Size size = { 0, 0 };
	EXPECT_EQ(proxy.get_size(), size);
	EXPECT_EQ(proxy.is_ready(), true);
}

TEST(WrongFormat, Exist_Test1)
{
	BitmapProxy proxy;
	proxy.load(formatted_wrongly1);
	Size size = { 0, 0 };
	EXPECT_EQ(proxy.get_size(), size);
	EXPECT_EQ(proxy.is_ready(), true);
}

TEST(WrongFormat, Exist_Test2)
{
	BitmapProxy proxy;
	proxy.load(formatted_wrongly2);
	Size size = { 0, 0 };
	EXPECT_EQ(proxy.get_size(), size);
	EXPECT_EQ(proxy.is_ready(), true);
	EXPECT_EQ(proxy.is_ready(), true);
}

TEST(WrongFormat, Exist_Test3)
{
	BitmapProxy proxy;
	proxy.load(formatted_wrongly3);
	Size size = { 0, 0 };
	EXPECT_EQ(proxy.get_size(), size);
	EXPECT_EQ(proxy.is_ready(), true);
}

TEST(Extension, WithBmp)
{
	BitmapProxy proxy;
	proxy.load(with_bmp);
	Size size = { 10, 56 };
	EXPECT_EQ(proxy.get_size(), size);
	EXPECT_EQ(proxy.is_ready(), true);
}

TEST(Extension, WithoutBmp)
{
	BitmapProxy proxy;
	proxy.load(without_bmp);
	Size size = { 10, 56 };
	EXPECT_EQ(proxy.get_size(), size);
	EXPECT_EQ(proxy.is_ready(), true);
}

TEST(Reload, FormattedToNotFormatted)
{
	BitmapProxy proxy;
	proxy.load(formatted1);

	Size size = { 100, 100 };
	EXPECT_EQ(proxy.get_size(), size);
	EXPECT_EQ(proxy.is_ready(), true);

	proxy.load(not_formatted1);
	EXPECT_EQ(proxy.is_ready(), false);

	size = { 100, 100 };
	EXPECT_EQ(proxy.get_size(), size);
	EXPECT_EQ(proxy.is_ready(), true);
}

TEST(Reload, NotFormattedToFormatted)
{
	BitmapProxy proxy;
	proxy.load(not_formatted1);

	Size size = { 100, 100 };
	EXPECT_EQ(proxy.get_size(), size);
	EXPECT_EQ(proxy.is_ready(), true);

	proxy.load(formatted1);
	EXPECT_EQ(proxy.is_ready(), false);

	size = { 100, 100 };
	EXPECT_EQ(proxy.get_size(), size);
	EXPECT_EQ(proxy.is_ready(), true);
}

TEST(Reload, ExistToNotExist)
{
	BitmapProxy proxy;
	proxy.load(not_formatted1);

	Size size = { 100, 100 };
	EXPECT_EQ(proxy.get_size(), size);
	EXPECT_EQ(proxy.is_ready(), true);

	proxy.load(not_formatted2);
	EXPECT_EQ(proxy.is_ready(), false);

	size = { 0, 0 };
	EXPECT_EQ(proxy.get_size(), size);
	EXPECT_EQ(proxy.is_ready(), true);
}

TEST(Reload, NotExistToExist)
{
	BitmapProxy proxy;
	proxy.load(not_formatted2);

	Size size = { 0, 0 };
	EXPECT_EQ(proxy.get_size(), size);
	EXPECT_EQ(proxy.is_ready(), true);

	proxy.load(not_formatted1);
	EXPECT_EQ(proxy.is_ready(), false);

	size = { 100, 100 };
	EXPECT_EQ(proxy.get_size(), size);
	EXPECT_EQ(proxy.is_ready(), true);
}