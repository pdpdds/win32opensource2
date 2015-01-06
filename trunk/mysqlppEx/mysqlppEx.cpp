// mysqlppEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <iostream>
#include "cmdline.h"
#include "stock.h"

using namespace std;

#pragma comment(lib, "mysqlpp_d.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	try {
		// Establish the connection to the database server.
		mysqlpp::Connection con("test",
			"127.0.0.1:3306", "root", "root");

		// Show initial state
		mysqlpp::Query query = con.query();
		cout << "Initial state of stock table:" << endl;

		// Insert a few rows in a single transaction set
		{
			// Use a higher level of transaction isolation than MySQL
			// offers by default.  This trades some speed for more
			// predictable behavior.  We've set it to affect all
			// transactions started through this DB server connection,
			// so it affects the next block, too, even if we don't
			// commit this one.
			mysqlpp::Transaction trans(con,
				mysqlpp::Transaction::serializable,
				mysqlpp::Transaction::session);

			stock row("Sauerkraut", 42, 1.2, 0.75,
				mysqlpp::sql_date("2006-03-06"), mysqlpp::null);
			query.insert(row);
			query.execute();

			cout << "\nRow inserted, but not committed." << endl;
			cout << "Verify this with another program (e.g. simple1), "
				"then hit Enter." << endl;
			getchar();

			cout << "\nCommitting transaction gives us:" << endl;
			trans.commit();			
		}

		// Now let's test auto-rollback
		{
			// Start a new transaction, keeping the same isolation level
			// we set above, since it was set to affect the session.
			mysqlpp::Transaction trans(con);
			cout << "\nNow adding catsup to the database..." << endl;

			stock row("Catsup", 3, 3.9, 2.99,
				mysqlpp::sql_date("2006-03-06"), mysqlpp::null);
			query.insert(row);
			query.execute();
		}
		cout << "\nNo, yuck! We don't like catsup. Rolling it back:" <<
			endl;		

	}
	catch (const mysqlpp::BadQuery& er) {
		// Handle any query errors
		cerr << "Query error: " << er.what() << endl;
		return -1;
	}
	catch (const mysqlpp::BadConversion& er) {
		// Handle bad conversions
		cerr << "Conversion error: " << er.what() << endl <<
			"\tretrieved data size: " << er.retrieved <<
			", actual size: " << er.actual_size << endl;
		return -1;
	}
	catch (const mysqlpp::Exception& er) {
		// Catch-all for any other MySQL++ exceptions
		cerr << "Error: " << er.what() << endl;
		return -1;
	}

	return 0;
}

