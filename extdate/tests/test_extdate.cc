#include <stdlib.h>
#include <iostream>

#include "../extdatetime.h"


void test1_unit(int a_year)
{
	std::cout << a_year << "  (QDate|ExtDate): " << ((QDate::leapYear(a_year)) ? "yes" : "no")
			<<"|"<< ((ExtDate::leapYear(a_year)) ? "yes" : "no") << std::endl;
}

void test1()
{
	int a_set_of_years[] =
	{
		1996,
		1997,
		1998,
		2000,
		1999,
		2001,
		1900,
		1800,
		1700,
		1600,
		2100,
		2200,
		2300,
		0,
		-1,
		-4,
		-100,
		-200,
		-300,
		-400,
		-500
	};
	uint i;
	std::cout << "Checking Leap Years:\n" << std::endl;
	for (i = 0 ; i < sizeof(a_set_of_years)/sizeof(a_set_of_years[0]) ; i++)
	{
		test1_unit(a_set_of_years[i]);
	}
	std::cout << "--------------------" << std::endl;
}

void test2_unit(int y, int m, int d)
{
	QDate	q(y, m, d);
	ExtDate	e(y, m, d);
	int q_week_number = q.dayOfWeek();
	int e_week_number = e.dayOfWeek();
	int q_day_of_year = q.dayOfYear();
	int e_day_of_year = e.dayOfYear();
	std::cout << "(" << y << ", " << m << ", " << d << ") ::  "
			<< q.toString("dd.MMM.yyyy").toLocal8Bit().data() << "  :  "
			<< q.dayOfWeek() << " : " << q_week_number << " : " << q_day_of_year << " :: "
			<< e.toString("%d.%b.%Y").toLocal8Bit().data() << " : "
			<< e.dayOfWeek() << " : " << e_week_number << " : " << e_day_of_year << std::endl;
}

void test2()
{
	int a_set_of_dates[][3] =
	{
		{0, 1, 1},
		{1, 1, 1},
		{2, 1, 1},
		{3, 1, 1},
		{4, 1, 1},
		{5, 1, 1},
		{99, 1, 1},
		{100, 1, 1},
		{100, 12, 31},
		{101, 1, 1},
		{102, 1, 1},
		{103, 1, 1},
		{104, 1, 1},
		{399, 1, 1},
		{400, 1, 1},
		{401, 1, 1},
		{402, 1, 1},
		{403, 1, 1},
		{404, 1, 1},
		{2003, 1, 1},
		{2003, 1, 2},
		{2003, 1, 3},
		{2003, 1, 4},
		{2003, 1, 5},
		{2003, 1, 6},
		{2003, 1, 7},
		{2003, 1, 8},
		{2003, 1, 9},
		{2003, 1, 10},
		{2003, 1, 11},
		{2003, 1, 12},
		{2003, 1, 13},
		{2003, 1, 14},
		{2003, 1, 15},
		{2003, 1, 16},
		{2003, 1, 17},
		{2003, 1, 18},
		{2003, 12, 19},
		{2003, 12, 20},
		{2003, 12, 21},
		{2003, 12, 22},
		{2003, 12, 23},
		{2003, 12, 24},
		{2003, 12, 25},
		{2003, 12, 26},
		{2003, 12, 27},
		{2003, 12, 28},
		{2003, 12, 29},
		{2003, 12, 30},
		{2003, 12, 31},
		{2004, 1, 1},
		{2003, 11, 2}
	};
	uint i;
	std::cout << "(y, m, d) :: QDate : Q.dayOfWeek() : Q.weekNumber() : Q.dayOfYear() :: ExtDate : E.dayOfWeek() : E.weekNumber() : E.dayOfYear()\n" << std::endl;

	for (i = 0 ; i < sizeof(a_set_of_dates)/sizeof(a_set_of_dates[0]) ; i++)
	{
		test2_unit(a_set_of_dates[i][0], a_set_of_dates[i][1], a_set_of_dates[i][2]);
	}
	std::cout << "--------------------" << std::endl;
}

void test3_unit(int y, int m, int d, int dm)
{
	QDate	q(y, m, d);
	ExtDate	e(y, m, d);
	QDate q2 = q.addMonths(dm);
	ExtDate e2 = e.addMonths(dm);
	std::cout << e.toString("%d.%b.%Y").toLocal8Bit().data() << " + " << dm
		<< " months :: ExtDate : " << e2.toString("%d.%b.%Y").toLocal8Bit().data()
		<< "   QDate : " << q2.toString("dd.MMM.yyyy").toLocal8Bit().data() << std::endl;
}

void test3()
{
	int a_set_of_dates_delta[][4] =
	{
		{2003, 11, 5, 0},
		{2003, 11, 5, 1},
		{2003, 11, 5, -1},
		{2003, 11, 5, 2},
		{2003, 11, 5, 3}
	};
	uint i;
	std::cout << "Adding Months:\n" << std::endl;
	for (i = 0 ; i < sizeof(a_set_of_dates_delta)/sizeof(a_set_of_dates_delta[0]) ; i++)
	{
		test3_unit(a_set_of_dates_delta[i][0], a_set_of_dates_delta[i][1], a_set_of_dates_delta[i][2], a_set_of_dates_delta[i][3]);
	}
	std::cout << "--------------------" << std::endl;
}
void test4_unit(int y, int m, int d, int dy)
{
	QDate	q(y, m, d);
	ExtDate	e(y, m, d);
	QDate	q2 = q.addYears(dy);
	ExtDate	e2 = e.addYears(dy);
	std::cout << e.toString("%d.%m.%Y").toLocal8Bit().data() << " + " << dy << " years :: ExtDate : "
		<< e2.toString().toLocal8Bit().data() << "   QDate : "
		<< q2.toString().toLocal8Bit().data() << std::endl;
}

void test4()
{
	int a_set_of_dates_delta[][4] =
	{
		{-1, 11, 5, 0},
		{-1, 11, 5, 1},
		{-1, 11, 5, 2},
		{2003, 11, 5, 1},
		{2003, 11, 5, -1},
		{2003, 11, 5, 2},
		{2003, 11, 5, 3}
	};
	uint i;
	std::cout << "Adding years:\n" << std::endl;
	for (i = 0 ; i < sizeof(a_set_of_dates_delta)/sizeof(a_set_of_dates_delta[0]) ; i++)
	{
		test4_unit(a_set_of_dates_delta[i][0], a_set_of_dates_delta[i][1], a_set_of_dates_delta[i][2], a_set_of_dates_delta[i][3]);
	}
	std::cout << "--------------------" << std::endl;
}

void test5_unit(int y, int m, int d, const char *qformat, const char *eformat)
{
	QDate q(y, m, d);
	ExtDate e(y, m, d);

	if ( QString(qformat) == "<default>" )
		std::cout << eformat << " : " << e.toString().toLocal8Bit().data() << " :: "
				<< qformat << " : " << q.toString().toLocal8Bit().data() << std::endl;
	else
		std::cout << eformat << " : " << e.toString(eformat).toLocal8Bit().data() << " :: "
				<< qformat << " : " << q.toString(qformat).toLocal8Bit().data() << std::endl;
}

void test5()
{
	const char *q_set_of_formats[7] =
	{
		"d.M.yy",
		"dd.MM.yy",
		"ddd.MMM.yy",
		"ddd dd.MMM.yy",
		"dddd.MMMM.yy",
		">dd.M.yyyy<",
		"<default>"
	};
	const char *e_set_of_formats[7] =
	{
		"%e.%n.%y",
		"%d.%m.%y",
		"%d.%b.%y",
		"%a %d.%b.%y",
		"%A.%B.%y",
		">%d.%n.%Y<",
		"<default>"
	};

	uint i;
	std::cout << "Date.toString(\"...\")" << std::endl;
	std::cout << "Ext Format : ExtDate :: Q Format : QDate\n" << std::endl;

	for (i = 0 ; i < sizeof(q_set_of_formats)/sizeof(q_set_of_formats[0]) ; i++)
	{
		test5_unit(2003, 11, 5, q_set_of_formats[i], e_set_of_formats[i]);
	}
	std::cout << "--------------------" << std::endl;
}

void test6_unit(int y, int m, int d)
{
	std::cout << d << "/" << m << "/" << y << " :: "
		<< ( ExtDate::isValid(y, m, d) ? "TRUE" : "FALSE" ) << " : "
		<< ( QDate::isValid(y, m, d) ? "TRUE" : "FALSE" ) << std::endl;
}

void test6()
{
	int a_set_of_dates[][3] =
	{
		{-1, 11, 5},
		{-1, 11, 5},
		{-1, 11, 5},
		{2003, 11, 5},
		{2003, -1, 5},
		{2003, 0, 5},
		{2003, 12, 5},
		{2003, 13, 5},
		{2003, 11, -2},
		{2003, 11, 0},
		{2003, 11, 40},
		{2004, 2, 28},
		{2004, 2, 29},
		{2004, 2, 30},
		{2003, 2, 28},
		{2003, 2, 29},
		{2003, 2, 30}
	};
	uint i;
	std::cout << "Date.isValid()" << std::endl;
	std::cout << "d/m/y :: ExtDate.isValid() : QDate.isValid()\n" << std::endl;

	for (i = 0 ; i < sizeof(a_set_of_dates)/sizeof(a_set_of_dates[0]) ; i++)
	{
		test6_unit(a_set_of_dates[i][0], a_set_of_dates[i][1], a_set_of_dates[i][2]);
	}
	std::cout << "--------------------" << std::endl;
}

void test7()
{
	std::cout << "Express the current date:\n" << std::endl;
	QDate q = QDate::currentDate(Qt::LocalTime);
	ExtDate e = ExtDate::currentDate(Qt::TimeSpec(Qt::LocalTime));
	std::cout << "Qt::LocalTime :: ExtDate : " << e.toString().toLocal8Bit().data() << "   QDate : "
		<< q.toString().toLocal8Bit().data() << std::endl;
	q = QDate::currentDate(Qt::UTC);
	e = ExtDate::currentDate(Qt::UTC);
	std::cout << "Qt::UTC       :: ExtDate : " << e.toString().toLocal8Bit().data() << "   QDate : "
		<< q.toString().toLocal8Bit().data() << std::endl;
	q = QDate::currentDate();
	e = ExtDate::currentDate();
	std::cout << "<default>     :: ExtDate : " << e.toString().toLocal8Bit().data() << "   QDate : "
		<< q.toString().toLocal8Bit().data() << std::endl;
	std::cout << "--------------------" << std::endl;
}

void test8() {
	std::cout << "Set dates using days 1-32 for Jan and Feb (some will be invalid): \n" << std::endl;
	std::cout << "    QDate      :     ExtDate" << std::endl;

	for ( uint m=1; m<=2; ++m ) {
		for ( uint d=1; d<=32; ++d ) {

			QDate   test1( 2004, m, d );
			ExtDate test2( 2004, m, d );
			std::cout << test1.toString( "ddd dd.MMM.yy" ).toLocal8Bit().data() << "  :  "
		<< test2.toString( "%a %d.%b.%y" ).toLocal8Bit().data() << std::endl;
		}
	}

	std::cout << "--------------------" << std::endl;
}

void test9() {
	std::cout << "QDateTime : ExtDateTime: \n" << std::endl;
	QDateTime q = QDateTime::currentDateTime();
	ExtDateTime e = ExtDateTime::currentDateTime();

	std::cout << q.toString().toLocal8Bit().data() << " : " << e.toString().toLocal8Bit().data() << std::endl;
	std::cout << "--------------------" << std::endl;
}

void test10() {
	ExtDate d1 = ExtDate::fromString("20050829", Qt::ISODate);
	ExtDate d2 = ExtDate::fromString("30 Aug 2005", Qt::TextDate);
	ExtDate d3 = ExtDate::fromString("Aug 31, 2005", "%b %d, %Y");
	ExtDate d4 = ExtDate::fromString("2005foo09bar01", "%Yfoo%mbar%d");
	std::cout << "ExtDate::fromString(\"20050829\", Qt::ISODate): " 
		<< d1.toString().toLocal8Bit().data() << std::endl;
	std::cout << "ExtDate::fromString(\"30 Aug 2005\", Qt::TextDate): " 
		<< d2.toString().toLocal8Bit().data() << std::endl;
	std::cout << "ExtDate::fromString(\"Aug 31, 2005\", \"%b %d, %Y\"): " 
		<< d3.toString().toLocal8Bit().data() << std::endl;
	std::cout << "ExtDate::fromString(\"2005foo09bar01\", \"%Yfoo%mbar%d\"): " 
		<< d4.toString().toLocal8Bit().data() << std::endl;

	std::cout << "--------------------" << std::endl;
}

int main(int argc, char *argv[])
{
	// shut up gcc
	(void)argc;
	(void)argv;

	test1();
	test2();
	test3();
	test4();
	test5();
	test6();
	test7();
	test8();
	test9();
	test10();
	exit(0);
}
