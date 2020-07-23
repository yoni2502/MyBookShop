
#include <stdlib.h>
#include <iostream>

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <boost/tokenizer.hpp>
#include <string>

using namespace std;

#define DB_NAME "booksshop"

vector<string> OrdersFieldsList;
vector<string> BooksFieldsList;
vector<string> CustomersFieldsList;
vector<string> SalesFieldsList;

void build_db(sql::Connection* con)
{
	sql::Statement* stmt = con->createStatement();
	stmt->execute("USE " DB_NAME);

	// Books
	stmt->execute("DROP TABLE IF EXISTS books");
	// BookID = Global Identifier (Unique)
	// BookGroup = for editions
	stmt->execute("CREATE TABLE books ( \
		BookID int PRIMARY KEY, \
		BookGroup int, \
		Title varchar(255), \
		Translator varchar(255), \
		Publisher varchar(255), \
		Year int, \
		Pages int, \
		Weight float \
	)");
	BooksFieldsList.push_back("BookID");
	BooksFieldsList.push_back("BookGroup");
	BooksFieldsList.push_back("Title");
	BooksFieldsList.push_back("Translator");
	BooksFieldsList.push_back("Publisher");
	BooksFieldsList.push_back("Year");
	BooksFieldsList.push_back("Pages");
	BooksFieldsList.push_back("Weight");
	stmt->execute("INSERT INTO books (BookID, BookGroup, Title, Translator, Publisher, Year, Pages, Weight) VALUES (1, 1,'A', 'T1', 'P1', 1993, 20, 1.2)");
	stmt->execute("INSERT INTO books (BookID, BookGroup, Title, Translator, Publisher, Year, Pages, Weight) VALUES (2, 1,'B', 'T2', 'P2', 1994, 20, 0.3)");
	stmt->execute("INSERT INTO books (BookID, BookGroup, Title, Translator, Publisher, Year, Pages, Weight) VALUES (3, 3,'C', 'T3', 'P3', 1995, 400, 1.8)");
	stmt->execute("INSERT INTO books (BookID, BookGroup, Title, Translator, Publisher, Year, Pages, Weight) VALUES (4, 4,'D', 'T4', 'P4', 1996, 20, 22)");
	stmt->execute("INSERT INTO books (BookID, BookGroup, Title, Translator, Publisher, Year, Pages, Weight) VALUES (5, 5,'F', 'T5', 'P5', 1997, 32, 2.3)");
	stmt->execute("INSERT INTO books (BookID, BookGroup, Title, Translator, Publisher, Year, Pages, Weight) VALUES (6, 5,'F', 'T6', 'P1', 1998, 20, 1.2)");
	stmt->execute("INSERT INTO books (BookID, BookGroup, Title, Translator, Publisher, Year, Pages, Weight) VALUES (7, 7,'G', 'T1', 'P2', 1999, 254, 1)");
	stmt->execute("INSERT INTO books (BookID, BookGroup, Title, Translator, Publisher, Year, Pages, Weight) VALUES (8, 8,'H', 'T2', 'P3', 1993, 20, 1.2)");
	stmt->execute("INSERT INTO books (BookID, BookGroup, Title, Translator, Publisher, Year, Pages, Weight) VALUES (9, 9,'I', 'T3', 'P4', 1996, 56, 6.4)");
	stmt->execute("INSERT INTO books (BookID, BookGroup, Title, Translator, Publisher, Year, Pages, Weight) VALUES (10, 10,'J', 'T4', 'P5', 1998, 20, 1.2)");
	stmt->execute("INSERT INTO books (BookID, BookGroup, Title, Translator, Publisher, Year, Pages, Weight) VALUES (11, 11,'RR', 'T9', 'P9', 2202, 20, 1.2)");

	// Authors
	stmt->execute("DROP TABLE IF EXISTS authors");
	stmt->execute("CREATE TABLE authors ( \
		AuthorID int PRIMARY KEY, \
		Name varchar(255) \
	)");
	stmt->execute("INSERT INTO authors (AuthorID, Name) VALUES (1, 'A1')");
	stmt->execute("INSERT INTO authors (AuthorID, Name) VALUES (2, 'A2')");
	stmt->execute("INSERT INTO authors (AuthorID, Name) VALUES (3, 'A3')");
	stmt->execute("INSERT INTO authors (AuthorID, Name) VALUES (4, 'A4')");
	stmt->execute("INSERT INTO authors (AuthorID, Name) VALUES (5, 'A5')");
	stmt->execute("INSERT INTO authors (AuthorID, Name) VALUES (6, 'A6')");
	stmt->execute("INSERT INTO authors (AuthorID, Name) VALUES (7, 'A7')");
	stmt->execute("INSERT INTO authors (AuthorID, Name) VALUES (8, 'A8')");
	stmt->execute("INSERT INTO authors (AuthorID, Name) VALUES (9, 'A9')");
	stmt->execute("INSERT INTO authors (AuthorID, Name) VALUES (10, 'A10')");

	// Books Authors
	stmt->execute("DROP TABLE IF EXISTS booksAuthors");
	stmt->execute("CREATE TABLE booksAuthors ( \
		BookID int, \
		AuthorID int \
	)");
	stmt->execute("INSERT INTO booksAuthors (BookID, AuthorID) VALUES (1,1)");
	stmt->execute("INSERT INTO booksAuthors (BookID, AuthorID) VALUES (2,2)");
	stmt->execute("INSERT INTO booksAuthors (BookID, AuthorID) VALUES (3,3)");
	stmt->execute("INSERT INTO booksAuthors (BookID, AuthorID) VALUES (4,4)");
	stmt->execute("INSERT INTO booksAuthors (BookID, AuthorID) VALUES (5,5)");
	stmt->execute("INSERT INTO booksAuthors (BookID, AuthorID) VALUES (6,6)");
	stmt->execute("INSERT INTO booksAuthors (BookID, AuthorID) VALUES (7,7)");
	stmt->execute("INSERT INTO booksAuthors (BookID, AuthorID) VALUES (8,8)");
	stmt->execute("INSERT INTO booksAuthors (BookID, AuthorID) VALUES (8,1)");
	stmt->execute("INSERT INTO booksAuthors (BookID, AuthorID) VALUES (9,9)");
	stmt->execute("INSERT INTO booksAuthors (BookID, AuthorID) VALUES (9,1)");
	stmt->execute("INSERT INTO booksAuthors (BookID, AuthorID) VALUES (10,10)");

	// Stock (store and stock)
	stmt->execute("DROP TABLE IF EXISTS stock");
	stmt->execute("CREATE TABLE stock ( \
		StockID int PRIMARY KEY, \
		BookID int, \
		Location varchar(255), \
		State varchar(255), \
		EnteredOn date \
	)");
	stmt->execute("INSERT INTO stock (StockID, BookID, Location, State, EnteredOn) VALUES (1, 1, 'STORE', 'NEW', '1999-02-14')");
	stmt->execute("INSERT INTO stock (StockID, BookID, Location, State, EnteredOn) VALUES (2, 2, 'STOCK', 'REGULAR', '2000-02-14')");
	stmt->execute("INSERT INTO stock (StockID, BookID, Location, State, EnteredOn) VALUES (3, 3, 'STORE', 'NEW', '2015-02-17')");
	stmt->execute("INSERT INTO stock (StockID, BookID, Location, State, EnteredOn) VALUES (4, 4, 'STOCK', 'BAD', '2013-03-5')");
	stmt->execute("INSERT INTO stock (StockID, BookID, Location, State, EnteredOn) VALUES (5, 5, 'STORE', 'NEW', '2019-02-14')");
	stmt->execute("INSERT INTO stock (StockID, BookID, Location, State, EnteredOn) VALUES (6, 6, 'STOCK', 'REGULAR', '1999-08-13')");
	stmt->execute("INSERT INTO stock (StockID, BookID, Location, State, EnteredOn) VALUES (7, 7, 'STORE', 'NEW', '2017-04-14')");
	stmt->execute("INSERT INTO stock (StockID, BookID, Location, State, EnteredOn) VALUES (8, 8, 'STOCK', 'REGULAR', '2020-02-24')");
	stmt->execute("INSERT INTO stock (StockID, BookID, Location, State, EnteredOn) VALUES (9, 9, 'STORE', 'BAD', '1998-02-1')");
	stmt->execute("INSERT INTO stock (StockID, BookID, Location, State, EnteredOn) VALUES (10, 10, 'STOCK','NEW', '2001-02-14')");

	// Orders
	stmt->execute("DROP TABLE IF EXISTS orders");
	stmt->execute("CREATE TABLE orders ( \
		OrderID int, \
		BookID int, \
		CustomerID int, \
		Status varchar(255), \
		ContactAt date, \
		CreatedAt date \
		, PRIMARY KEY(OrderID, BookID, CustomerID) \
	)");
	OrdersFieldsList.push_back("OrderID");
	OrdersFieldsList.push_back("BookID");
	OrdersFieldsList.push_back("CustomerID");
	OrdersFieldsList.push_back("Status");
	OrdersFieldsList.push_back("ContactAt");
	OrdersFieldsList.push_back("CreatedAt");
	stmt->execute("INSERT INTO orders (OrderID, BookID, CustomerID, Status, ContactAt, CreatedAt) VALUES (1, 1, 1, 'WAITING', NULL, '2001-02-14')");
	stmt->execute("INSERT INTO orders (OrderID, BookID, CustomerID, Status, ContactAt, CreatedAt) VALUES (2, 2, 1, 'ARRIVED', NULL, '1993-02-15')");
	stmt->execute("INSERT INTO orders (OrderID, BookID, CustomerID, Status, ContactAt,CreatedAt) VALUES (3, 6, 7, 'CONTACT', '2019-02-14', '2002-02-16')");
	stmt->execute("INSERT INTO orders (OrderID, BookID, CustomerID, Status, ContactAt,CreatedAt) VALUES (4, 7, 8, 'CONTACT', '2020-02-14', '2002-02-17')");
	stmt->execute("INSERT INTO orders (OrderID, BookID, CustomerID, Status, ContactAt,CreatedAt) VALUES (5, 8, 2, 'ARRIVED', '2020-02-14', '2002-02-18')");
	stmt->execute("INSERT INTO orders (OrderID, BookID, CustomerID, Status, ContactAt,CreatedAt) VALUES (6, 3, 3, 'CONTACT', '2020-02-14', '2002-02-19')");
	stmt->execute("INSERT INTO orders (OrderID, BookID, CustomerID, Status, ContactAt,CreatedAt) VALUES (7, 4, 4, 'WAITING', '2020-02-14', '2002-02-22')");
	stmt->execute("INSERT INTO orders (OrderID, BookID, CustomerID, Status, ContactAt,CreatedAt) VALUES (8, 5, 5, 'CONTACT', '2020-02-14', '2002-02-23')");
	stmt->execute("INSERT INTO orders (OrderID, BookID, CustomerID, Status, ContactAt,CreatedAt) VALUES (9, 9, 6, 'CONTACT', '2020-02-14', '2002-02-12')");
	stmt->execute("INSERT INTO orders (OrderID, BookID, CustomerID, Status, ContactAt,CreatedAt) VALUES (10, 7, 8, 'WAITING', '2020-02-14', '2002-02-11')");


	// Sales
	stmt->execute("DROP TABLE IF EXISTS sales");
	stmt->execute("CREATE TABLE sales ( \
		SaleID int, \
		CustomerID int, \
		BookID int, \
		Count int, \
		SaleDeliveryID int, \
		Payment float, \
		PaymentType varchar(255), \
		SalesmenID int, \
		CreatedAt date \
		, PRIMARY KEY(SaleID, CustomerID, BookID) \
	)");
	SalesFieldsList.push_back("SaleID");
	SalesFieldsList.push_back("CustomerID");
	SalesFieldsList.push_back("BookID");
	SalesFieldsList.push_back("Count");
	SalesFieldsList.push_back("SaleDeliveryID");
	SalesFieldsList.push_back("Payment");
	SalesFieldsList.push_back("PaymentType");
	SalesFieldsList.push_back("SalesmenID");
	SalesFieldsList.push_back("CreatedAt");
	stmt->execute("INSERT INTO sales (SaleID, BookID, CustomerID, Count, SaleDeliveryID, Payment, PaymentType,  SalesmenID, CreatedAt) VALUES (1,1,1,1,1, 13.2 , 'BIT' ,1, '2020-01-14')");
	stmt->execute("INSERT INTO sales (SaleID, BookID, CustomerID, Count, SaleDeliveryID, Payment, PaymentType,  SalesmenID, CreatedAt) VALUES (1,2,1,2,10, 1.2 , 'TRANSFER' ,1, '2001-01-14')");
	stmt->execute("INSERT INTO sales (SaleID, BookID, CustomerID, Count, SaleDeliveryID, Payment, PaymentType,  SalesmenID, CreatedAt) VALUES (2,1,1,1,2, 10 , 'CREDIT' , 1,'2020-02-14')");
	stmt->execute("INSERT INTO sales (SaleID, BookID, CustomerID, Count, SaleDeliveryID, Payment, PaymentType,  SalesmenID, CreatedAt) VALUES (3,1,2,1,3, 5 , 'TRANSFER' ,2, '2001-03-14')");
	stmt->execute("INSERT INTO sales (SaleID, BookID, CustomerID, Count, SaleDeliveryID, Payment, PaymentType,  SalesmenID, CreatedAt) VALUES (4,8,4,2,4, 29.4 , 'BIT' ,2, '2020-03-14')");
	stmt->execute("INSERT INTO sales (SaleID, BookID, CustomerID, Count, SaleDeliveryID, Payment, PaymentType,  SalesmenID, CreatedAt) VALUES (5,3,4,3,5, 22.4 , 'BIT' ,3, '2003-03-11')");
	stmt->execute("INSERT INTO sales (SaleID, BookID, CustomerID, Count, SaleDeliveryID, Payment, PaymentType,  SalesmenID, CreatedAt) VALUES (6,5,2,1,6, 22.4 , 'BIT',3, '2008-03-11')");
	stmt->execute("INSERT INTO sales (SaleID, BookID, CustomerID, Count, SaleDeliveryID, Payment, PaymentType,  SalesmenID, CreatedAt) VALUES (7,6,3,1,7, 22.4 , 'BIT',3, '2020-03-11')");
	stmt->execute("INSERT INTO sales (SaleID, BookID, CustomerID, Count, SaleDeliveryID, Payment, PaymentType,  SalesmenID, CreatedAt) VALUES (8,6,3,1,8, 5.1 , 'TRANSFER' ,5, '2020-02-13')");
	stmt->execute("INSERT INTO sales (SaleID, BookID, CustomerID, Count, SaleDeliveryID, Payment, PaymentType,  SalesmenID, CreatedAt) VALUES (9,6,3,1,9, 3.9 , 'CREDIT',6, '2020-01-18')");

	// Sales Deliveries
	stmt->execute("DROP TABLE IF EXISTS salesDeliveries");
	stmt->execute("CREATE TABLE salesDeliveries ( \
		SaleDeliveryID int, \
		DeliveryType varchar(255), \
		Address varchar(255), \
		Status varchar(255) \
		, PRIMARY KEY(SaleDeliveryID) \
	)");
	stmt->execute("INSERT INTO salesDeliveries (SaleDeliveryID, DeliveryType, Address,  Status) VALUES (1,'IPO-REGISTERED', 'ADDRESS1' , 'ON-STORE')");
	stmt->execute("INSERT INTO salesDeliveries (SaleDeliveryID, DeliveryType, Address,   Status) VALUES (2,'XPRESS-POINT', 'ADDRESS2' , 'ON-DELIVERY')");
	stmt->execute("INSERT INTO salesDeliveries (SaleDeliveryID, DeliveryType, Address,   Status) VALUES (3,'IPO-FAST', 'ADDRESS3' , 'ON-DELIVERY')");
	stmt->execute("INSERT INTO salesDeliveries (SaleDeliveryID, DeliveryType, Address,   Status) VALUES (4,'XPRESS-HOME', 'ADDRESS4' , 'FINISHED')");
	stmt->execute("INSERT INTO salesDeliveries (SaleDeliveryID, DeliveryType, Address,   Status) VALUES (5,'IPO-FAST', 'ADDRESS5' , 'ON_DELIVERY')");
	stmt->execute("INSERT INTO salesDeliveries (SaleDeliveryID, DeliveryType, Address,   Status) VALUES (6,'XPRESS-HOME', 'ADDRESS6' , 'ON_DELIVERY')");
	stmt->execute("INSERT INTO salesDeliveries (SaleDeliveryID, DeliveryType, Address,   Status) VALUES (7,'IPO-REGISTERED', 'ADDRESS7' , 'ON_DELIVERY')");
	stmt->execute("INSERT INTO salesDeliveries (SaleDeliveryID, DeliveryType, Address,   Status) VALUES (8,'XPRESS-HOME', 'ADDRESS8' , 'ON_DELIVERY')");
	stmt->execute("INSERT INTO salesDeliveries (SaleDeliveryID, DeliveryType, Address,   Status) VALUES (9,'XPRESS-POINT', 'ADDRESS9' , 'FINISHED')");
	stmt->execute("INSERT INTO salesDeliveries (SaleDeliveryID, DeliveryType, Address,   Status) VALUES (10,'IPO-REGISTERED', 'ADDRESS10' , 'ON-STORE')");


	// Shop Buy
	stmt->execute("DROP TABLE IF EXISTS shopBuy");
	stmt->execute("CREATE TABLE shopBuy ( \
		ID int PRIMARY KEY, \
		BookID int, \
		Price float, \
		BuyAt date \
	)");
	stmt->execute("INSERT INTO shopBuy (ID, BookID, Price, BuyAt) VALUES (1, 1, 12, '2002-02-14')");
	stmt->execute("INSERT INTO shopBuy (ID, BookID, Price, BuyAt) VALUES (2, 2, 14, '2003-02-12')");
	stmt->execute("INSERT INTO shopBuy (ID, BookID, Price, BuyAt) VALUES (3, 3, 15, '2020-01-11')");
	stmt->execute("INSERT INTO shopBuy (ID, BookID, Price, BuyAt) VALUES (4, 4, 16, '2020-01-11')");
	stmt->execute("INSERT INTO shopBuy (ID, BookID, Price, BuyAt) VALUES (5, 5, 17, '2020-01-09')");
	stmt->execute("INSERT INTO shopBuy (ID, BookID, Price, BuyAt) VALUES (6, 6, 18, '2020-01-02')");
	stmt->execute("INSERT INTO shopBuy (ID, BookID, Price, BuyAt) VALUES (7, 7, 19, '2020-01-01')");
	stmt->execute("INSERT INTO shopBuy (ID, BookID, Price, BuyAt) VALUES (8, 8, 20, '2020-01-13')");
	stmt->execute("INSERT INTO shopBuy (ID, BookID, Price, BuyAt) VALUES (9, 9, 21, '2020-01-19')");
	stmt->execute("INSERT INTO shopBuy (ID, BookID, Price, BuyAt) VALUES (10, 10, 22, '2020-01-12')");

	// Customers
	stmt->execute("DROP TABLE IF EXISTS customers");
	stmt->execute("CREATE TABLE customers ( \
		CustomerID int PRIMARY KEY, \
		Name varchar(255), \
		Phone varchar(255), \
		CreatedAt date \
	)");
	CustomersFieldsList.push_back("CustomerID");
	CustomersFieldsList.push_back("Name");
	CustomersFieldsList.push_back("Phone");
	CustomersFieldsList.push_back("CreatedAt");
	stmt->execute("INSERT INTO customers (CustomerID, Name, Phone, CreatedAt) VALUES (1, 'C1', '0521111111', '1999-02-14')");
	stmt->execute("INSERT INTO customers (CustomerID, Name, Phone, CreatedAt) VALUES (2, 'C2', '0521111112', '1994-02-14')");
	stmt->execute("INSERT INTO customers (CustomerID, Name, Phone, CreatedAt) VALUES (3, 'C3', '0521111113', '1995-02-14')");
	stmt->execute("INSERT INTO customers (CustomerID, Name, Phone, CreatedAt) VALUES (4, 'C4', '0521111114', '1992-02-14')");
	stmt->execute("INSERT INTO customers (CustomerID, Name, Phone, CreatedAt) VALUES (5, 'C5', '0521111115', '1991-02-14')");
	stmt->execute("INSERT INTO customers (CustomerID, Name, Phone, CreatedAt) VALUES (6, 'C6', '0521111116', '1996-02-14')");
	stmt->execute("INSERT INTO customers (CustomerID, Name, Phone, CreatedAt) VALUES (7, 'C7', '0521111117', '1996-02-14')");
	stmt->execute("INSERT INTO customers (CustomerID, Name, Phone, CreatedAt) VALUES (8, 'C8', '0521111118', '1997-02-14')");
	stmt->execute("INSERT INTO customers (CustomerID, Name, Phone, CreatedAt) VALUES (9, 'C9', '0521111119', '2000-02-14')");
	stmt->execute("INSERT INTO customers (CustomerID, Name, Phone, CreatedAt) VALUES (10, 'C10', '0521111110', '2002-02-14')");

	// Salesmen
	stmt->execute("DROP TABLE IF EXISTS salesmen");
	stmt->execute("CREATE TABLE salesmen ( \
		SalesmenID int PRIMARY KEY, \
		Name varchar(255), \
		Address varchar(255), \
		Phone varchar(255), \
		Stillwork bool, \
		JoinedAt date, \
		QuitAt date \
	)");
	stmt->execute("INSERT INTO salesmen (SalesmenID, Name, Address, Phone, Stillwork,JoinedAt,QuitAt) VALUES (1, 'S1', 'A1', '0527854478', True, '2002-02-14', NULL)");
	stmt->execute("INSERT INTO salesmen (SalesmenID, Name, Address, Phone, Stillwork,JoinedAt,QuitAt) VALUES (2, 'S2', 'A2', '0527854478', False, '2002-02-14', '2002-02-18')");
	stmt->execute("INSERT INTO salesmen (SalesmenID, Name, Address, Phone, Stillwork,JoinedAt,QuitAt) VALUES (3, 'S3', 'A3', '0527854478', True, '2002-02-14', NULL)");
	stmt->execute("INSERT INTO salesmen (SalesmenID, Name, Address, Phone, Stillwork,JoinedAt,QuitAt) VALUES (4, 'S4', 'A4', '0527854478', True, '2002-02-14', NULL)");
	stmt->execute("INSERT INTO salesmen (SalesmenID, Name, Address, Phone, Stillwork,JoinedAt,QuitAt) VALUES (5, 'S5', 'A5', '0527854478', True, '2002-02-14', NULL)");
	stmt->execute("INSERT INTO salesmen (SalesmenID, Name, Address, Phone, Stillwork,JoinedAt,QuitAt) VALUES (6, 'S6', 'A6', '0527854478', True, '2002-02-14', NULL)");
	stmt->execute("INSERT INTO salesmen (SalesmenID, Name, Address, Phone, Stillwork,JoinedAt,QuitAt) VALUES (7, 'S7', 'A7', '0527854478', True, '2002-02-14', NULL)");
	stmt->execute("INSERT INTO salesmen (SalesmenID, Name, Address, Phone, Stillwork,JoinedAt,QuitAt) VALUES (8, 'S8', 'A8', '0527854478', True, '2002-02-14', NULL)");
	stmt->execute("INSERT INTO salesmen (SalesmenID, Name, Address, Phone, Stillwork,JoinedAt,QuitAt) VALUES (9, 'S9', 'A9', '0527854478', True, '2002-02-14', NULL)");
	stmt->execute("INSERT INTO salesmen (SalesmenID, Name, Address, Phone, Stillwork,JoinedAt,QuitAt) VALUES (10, 'S10', 'A10', '0527854478', True, '2002-02-14', NULL)");

	// Salesmen Hours
	stmt->execute("DROP TABLE IF EXISTS salesmenHours");
	stmt->execute("CREATE TABLE salesmenHours ( \
		SalesmenID int, \
		Year int, \
		Month int, \
		NumHours int, \
		MoneyPerHour int, \
		PRIMARY KEY(SalesmenID, Year, Month) \
	)");
	stmt->execute("INSERT INTO salesmenHours (SalesmenID, Year, Month, NumHours, MoneyPerHour) VALUES (1, 2020, 1, 10, 10)");
	stmt->execute("INSERT INTO salesmenHours (SalesmenID, Year, Month, NumHours, MoneyPerHour) VALUES (2, 2020, 2, 10, 10)");
	stmt->execute("INSERT INTO salesmenHours (SalesmenID, Year, Month, NumHours, MoneyPerHour) VALUES (3, 2020, 3, 10, 10)");
	stmt->execute("INSERT INTO salesmenHours (SalesmenID, Year, Month, NumHours, MoneyPerHour) VALUES (4, 2020, 4, 10, 10)");
	stmt->execute("INSERT INTO salesmenHours (SalesmenID, Year, Month, NumHours, MoneyPerHour) VALUES (5, 2020, 5, 10, 10)");
	stmt->execute("INSERT INTO salesmenHours (SalesmenID, Year, Month, NumHours, MoneyPerHour) VALUES (6, 2020, 6, 10, 10)");
	stmt->execute("INSERT INTO salesmenHours (SalesmenID, Year, Month, NumHours, MoneyPerHour) VALUES (7, 2020, 7, 10, 10)");
	stmt->execute("INSERT INTO salesmenHours (SalesmenID, Year, Month, NumHours, MoneyPerHour) VALUES (8, 2020, 8, 10, 10)");
	stmt->execute("INSERT INTO salesmenHours (SalesmenID, Year, Month, NumHours, MoneyPerHour) VALUES (9, 2020, 9, 10, 10)");
	stmt->execute("INSERT INTO salesmenHours (SalesmenID, Year, Month, NumHours, MoneyPerHour) VALUES (10, 2020, 10, 10, 10)");

	// Shop Payments
	stmt->execute("DROP TABLE IF EXISTS shopPayments");
	stmt->execute("CREATE TABLE shopPayments ( \
		ShoppaymentID int PRIMARY KEY, \
		Type varchar(255), \
		Year int, \
		Month int, \
		AmountPayed float \
	)");
	stmt->execute("INSERT INTO shopPayments (ShoppaymentID, Type, Year, Month, AmountPayed) VALUES (1, 'ARNONA', 2020, 1, 1000)");
	stmt->execute("INSERT INTO shopPayments (ShoppaymentID, Type, Year, Month, AmountPayed) VALUES (2, 'WATER', 2020, 2,  1000)");
	stmt->execute("INSERT INTO shopPayments (ShoppaymentID, Type, Year, Month, AmountPayed) VALUES (3, 'ELECTRIC', 2020, 3,  3000)");
	stmt->execute("INSERT INTO shopPayments (ShoppaymentID, Type, Year, Month, AmountPayed) VALUES (4, 'WATER', 2020, 4,  1000)");
	stmt->execute("INSERT INTO shopPayments (ShoppaymentID, Type, Year, Month, AmountPayed) VALUES (5, 'ARNONA', 2020, 5,  3000)");
	stmt->execute("INSERT INTO shopPayments (ShoppaymentID, Type, Year, Month, AmountPayed) VALUES (6, 'PHONE', 2020, 6,  1000)");
	stmt->execute("INSERT INTO shopPayments (ShoppaymentID, Type, Year, Month, AmountPayed) VALUES (7, 'SALARIES', 2020, 7,  3000)");
	stmt->execute("INSERT INTO shopPayments (ShoppaymentID, Type, Year, Month, AmountPayed) VALUES (8, 'ARNONA', 2020, 8,  1000)");
	stmt->execute("INSERT INTO shopPayments (ShoppaymentID, Type, Year, Month, AmountPayed) VALUES (9, 'ELECTRIC', 2020, 9,  3000)");
	stmt->execute("INSERT INTO shopPayments (ShoppaymentID, Type, Year, Month, AmountPayed) VALUES (10, 'WATER', 2020, 10,  1000)");

	delete stmt;
}

void padTo(std::string& str, const size_t num, const char paddingChar = ' ')
{
	if (num > str.size())
		str.insert(0, num - str.size(), paddingChar);
}
void printTable(vector<string> fields, sql::ResultSet* res) {
	int pad = 15;

	// print header row
	for (vector<string>::iterator it = fields.begin(); it != fields.end(); ++it) {
		string col = *it;
		padTo(col, pad);
		cout << col;
	}
	cout << endl;

	// print rows
	while (res->next()) {
		for (vector<string>::iterator it = fields.begin(); it != fields.end(); ++it) {
			string col = *it;
			string val;
			try {
				val = res->getString(col.c_str()).c_str();
			}
			catch (const std::exception& e) {
				val = to_string(res->getInt(col));
			}

			padTo(val, pad);
			cout << val;
		}
		cout << endl;
	}
}

void op1(sql::Connection* con) {
	sql::Statement* stmt = con->createStatement();
	sql::ResultSet* res;
	sql::PreparedStatement* pstmt;
	stmt->execute("USE " DB_NAME);

	int bookId;
	cout << "Enter Book ID" << endl;
	cin >> bookId;

	pstmt = con->prepareStatement("SELECT * FROM stock WHERE BookID=?");
	pstmt->setInt(1, bookId);
	pstmt->executeUpdate();
	res = pstmt->executeQuery();

	if (res->next()) {
		cout << "The book exist in stock" << endl;
	}
	else {
		cout << "The book doesn't exist in stock" << endl;
	}

	delete pstmt;
	delete res;
	delete stmt;
}
void op2(sql::Connection* con) {
	sql::Statement* stmt = con->createStatement();
	sql::ResultSet* res;
	stmt->execute("USE " DB_NAME);

	res = stmt->executeQuery("SELECT * FROM customers ORDER BY CreatedAt ASC LIMIT 1");

	if (res->next()) {
		cout << "The earliest customer is " << res->getString("Name").c_str() << endl;
	}
	else {
		cout << "There are no customers" << endl;
	}

	delete res;
	delete stmt;
}
void op3(sql::Connection* con) {
	sql::Statement* stmt = con->createStatement();
	sql::ResultSet* res;
	stmt->execute("USE " DB_NAME);

	res = stmt->executeQuery("SELECT * FROM stock ORDER BY EnteredOn ASC LIMIT 1");

	if (res->next()) {
		cout << "The earliest book in stock is " << res->getInt("BookID") << endl;
	}
	else {
		cout << "There are no books in stock" << endl;
	}

	delete res;
	delete stmt;
}
void op4(sql::Connection* con) {
	sql::Statement* stmt = con->createStatement();
	sql::ResultSet* res;
	stmt->execute("USE " DB_NAME);

	res = stmt->executeQuery("SELECT * FROM orders ORDER BY CreatedAt DESC");
	printTable(OrdersFieldsList, res);

	delete res;
	delete stmt;
}
void op5(sql::Connection* con) {
	sql::Statement* stmt = con->createStatement();
	sql::ResultSet* res;
	sql::PreparedStatement* pstmt;
	stmt->execute("USE " DB_NAME);

	int bookId;
	cout << "Enter Book ID" << endl;
	cin >> bookId;
	pstmt = con->prepareStatement("SELECT COUNT(*) FROM sales WHERE BookID=?");
	pstmt->setInt(1, bookId);
	pstmt->executeUpdate();
	res = pstmt->executeQuery();

	res->next();
	cout << "Num of copies = " << res->getInt(1) << endl;

	delete pstmt;
	delete res;
	delete stmt;
}
void op6(sql::Connection* con) {
	sql::Statement* stmt = con->createStatement();
	sql::ResultSet* res;
	stmt->execute("USE " DB_NAME);

	string from;
	string to;
	cout << "Enter from date (yyyy-mm-dd):" << endl;
	cin >> from;
	cout << "Enter to date (yyyy-mm-dd):" << endl;
	cin >> to;

	string query = " \
		SELECT * \
		FROM ( \
				SELECT a.AuthorID, Name, COUNT(*) as counter  \
				FROM sales s JOIN booksAuthors ba ON s.BookID = ba.BookID JOIN authors a ON ba.AuthorID = a.AuthorID \
				WHERE CreatedAt BETWEEN '" + from + "' AND '" + to + "' GROUP BY a.AuthorID, Name \
			) t \
		ORDER BY t.counter DESC \
		LIMIT 1";

	res = stmt->executeQuery(query.c_str());

	res->next();
	cout << "The common author in this dates is " << res->getString("Name").c_str() << endl;

	delete res;
	delete stmt;
}
void op7(sql::Connection* con) {
	sql::Statement* stmt = con->createStatement();
	sql::ResultSet* res;
	stmt->execute("USE " DB_NAME);

	string query = " \
		SELECT * \
		FROM customers c JOIN ( \
			SELECT s.CustomerID  \
			FROM sales s JOIN customers c ON s.CustomerID = c.CustomerID GROUP BY s.CustomerID ORDER BY COUNT(*) DESC LIMIT 3 \
		) t ON c.CustomerID = t.CustomerID \
	";

	res = stmt->executeQuery(query.c_str());
	printTable(CustomersFieldsList, res);

	delete res;
	delete stmt;
}
void op8(sql::Connection* con) {
	sql::Statement* stmt = con->createStatement();
	sql::ResultSet* res;
	stmt->execute("USE " DB_NAME);

	string query = " \
		SELECT b.Title, COUNT(distinct b.Translator) as num FROM stock s JOIN books b ON s.BookID = b.BookID \
		GROUP BY b.BookGroup, b.Title \
		ORDER BY num DESC \
		LIMIT 1 \
	";

	res = stmt->executeQuery(query.c_str());
	res->next();
	cout << "The book with the largest amount of translations in stock is: " << res->getString("Title").c_str() << endl;

	delete res;
	delete stmt;
}
void op9(sql::Connection* con) {
	sql::Statement* stmt = con->createStatement();
	sql::ResultSet* res;
	stmt->execute("USE " DB_NAME);

	int customerId;
	cout << "Enter Customer ID" << endl;
	cin >> customerId;

	string query = " \
		SELECT * \
		FROM sales \
		WHERE CustomerID = " + to_string(customerId) + " ORDER BY CreatedAt DESC";

	vector<string> fields;
	fields.push_back("BookID");
	fields.push_back("CreatedAt");
	fields.push_back("Payment");

	res = stmt->executeQuery(query.c_str());
	printTable(fields, res);

	delete res;
	delete stmt;
}
void op10(sql::Connection* con) {
	sql::Statement* stmt = con->createStatement();
	sql::ResultSet* res;
	stmt->execute("USE " DB_NAME);

	int customerId;
	cout << "Enter Customer ID" << endl;
	cin >> customerId;

	string query = "SELECT * FROM orders WHERE CustomerID=" + to_string(customerId) + " ORDER BY CreatedAt DESC";
	res = stmt->executeQuery(query.c_str());
	printTable(OrdersFieldsList, res);

	delete res;
	delete stmt;
}
void op11_showprice(sql::Connection* con, string name, double pricePerKg) {
	sql::Statement* stmt = con->createStatement();
	sql::ResultSet* res;
	stmt->execute("USE " DB_NAME);

	string query = "SELECT o.*, b.Weight FROM orderTmp o JOIN books b ON o.BookID = b.BookID";

	double total = 0;
	res = stmt->executeQuery(query.c_str());
	while (res->next()) {
		total += res->getInt("Count") * res->getDouble("Weight") * pricePerKg;
	}
	cout << name << " = " << total << endl;

	delete res;
	delete stmt;
}
void op11(sql::Connection* con) {
	sql::Statement* stmt = con->createStatement();
	stmt->execute("USE " DB_NAME);

	stmt->execute("DROP TABLE IF EXISTS orderTmp");
	stmt->execute("CREATE TABLE orderTmp ( \
		BookID int PRIMARY KEY, \
		Count int \
	)");

	while (true) {

		int bookId;
		cout << "Enter Book ID (or 0 to finish order)" << endl;
		cin >> bookId;

		if (bookId == 0) {
			break;
		}

		int count;
		cout << "Enter Count" << endl;
		cin >> count;

		string query = "INSERT INTO orderTmp (BookID, Count) VALUES (" + to_string(bookId) + ", " + to_string(count) + ")";
		stmt->execute(query.c_str());
	}

	{
		// IPO
		{
			// REGISTERED
			op11_showprice(con, "IPO-REGISTERED", 1);
		}
		{
			// DELIVERY
			op11_showprice(con, "IPO-DELIVERY", 2);
		}
		{
			// FAST
			op11_showprice(con, "IPO-FAST", 3);

		}
	}
	{
		// XPRESS
		{
			// HOME
			op11_showprice(con, "XPRESS-HOME", 4);
		}
		{
			// POINT
			op11_showprice(con, "XPRESS-POINT", 5);

		}
	}

	stmt->execute("DROP TABLE IF EXISTS orderTmp");

	delete stmt;
}
void op12(sql::Connection* con) {
	sql::Statement* stmt = con->createStatement();
	sql::ResultSet* res;
	stmt->execute("USE " DB_NAME);

	int customerId;
	cout << "Enter Customer ID" << endl;
	cin >> customerId;

	string query = " \
		SELECT s.* FROM sales s JOIN \
		(SELECT s.SaleID FROM sales s JOIN salesDeliveries sd ON s.SaleDeliveryID = sd.SaleDeliveryID  WHERE s.CustomerID = " + to_string(customerId) + " GROUP BY s.SaleID HAVING COUNT(distinct sd.SaleDeliveryID) > 1) t \
		ON s.SaleID = t.SaleID \
		ORDER BY s.SaleID \
	";

	res = stmt->executeQuery(query.c_str());
	printTable(SalesFieldsList, res);

	delete res;
	delete stmt;
}
void op13(sql::Connection* con) {
	sql::Statement* stmt = con->createStatement();
	sql::ResultSet* res;
	stmt->execute("USE " DB_NAME);

	string code;
	cout << "Enter Delivery Code (SaleDeliveryID)" << endl;
	cin >> code;

	string query = "SELECT * FROM salesDeliveries WHERE SaleDeliveryID = '" + code + "'";

	res = stmt->executeQuery(query.c_str());
	if (res->next()) {
		cout << "The status of the delivery is " << res->getString("Status").c_str() << endl;
	}
	else {
		cout << "Delivery code doesn't exist in the system" << endl;
	}

	delete res;
	delete stmt;
}
void op14(sql::Connection* con) {
	sql::Statement* stmt = con->createStatement();
	sql::ResultSet* res;
	stmt->execute("USE " DB_NAME);

	string month;
	cout << "Enter month (1-12)" << endl;
	cin >> month;

	string year;
	cout << "Enter year" << endl;
	cin >> year;

	string query = "SELECT CONVERT(ROUND(SUM(s.Payment),2),CHAR) as Total FROM sales s JOIN salesDeliveries sd ON s.SaleDeliveryID = sd.SaleDeliveryID WHERE sd.DeliveryType LIKE 'XPRESS%' AND MONTH(s.CreatedAt) = " + month + " AND YEAR(s.CreatedAt) = " + year;

	res = stmt->executeQuery(query.c_str());
	res->next();
	cout << "The sum of all the deliveries of Xpress is " << res->getString("Total").c_str() << endl;

	delete res;
	delete stmt;
}
void op15(sql::Connection* con) {
	sql::Statement* stmt = con->createStatement();
	sql::ResultSet* res;
	stmt->execute("USE " DB_NAME);

	string month;
	cout << "Enter month (1-12)" << endl;
	cin >> month;

	string year;
	cout << "Enter year" << endl;
	cin >> year;

	string query = "SELECT CONVERT(ROUND(SUM(Payment),2),CHAR) as Total FROM sales WHERE PaymentType = 'BIT' AND MONTH(CreatedAt) = " + month + " AND YEAR(CreatedAt) = " + year;
	res = stmt->executeQuery(query.c_str());
	res->next();
	cout << "The sum of all the BIT payments to the store is " << res->getString("Total").c_str() << endl;

	delete res;
	delete stmt;
}
void op16(sql::Connection* con) {
	sql::Statement* stmt = con->createStatement();
	sql::ResultSet* res;
	stmt->execute("USE " DB_NAME);

	string query = "SELECT ROUND(SUM(Payment)/COUNT(*),2) as res FROM sales WHERE CreatedAt >= DATE_SUB(NOW(), INTERVAL 12 MONTH)";
	res = stmt->executeQuery(query.c_str());
	res->next();
	double avg = res->getDouble("res");

	query = "SELECT * FROM sales WHERE CreatedAt >= DATE_SUB(NOW(), INTERVAL 12 MONTH) AND Payment > " + to_string(avg);

	res = stmt->executeQuery(query.c_str());
	printTable(SalesFieldsList, res);

	delete res;
	delete stmt;
}
void op17(sql::Connection* con) {
	sql::Statement* stmt = con->createStatement();
	sql::ResultSet* res;
	stmt->execute("USE " DB_NAME);

	string query = "SELECT COUNT(distinct s.SaleDeliveryID) as Total FROM sales s JOIN salesDeliveries sd ON s.SaleDeliveryID = sd.SaleDeliveryID WHERE sd.DeliveryType LIKE 'XPRESS%' AND s.CreatedAt >= DATE_SUB(NOW(), INTERVAL 12 MONTH)";
	res = stmt->executeQuery(query.c_str());
	res->next();
	cout << "The num of Xpress deliveries is " << res->getString("Total").c_str() << endl;

	query = "SELECT COUNT(distinct s.SaleDeliveryID) as Total FROM sales s JOIN salesDeliveries sd ON s.SaleDeliveryID = sd.SaleDeliveryID WHERE sd.DeliveryType LIKE 'IPO%' AND s.CreatedAt >= DATE_SUB(NOW(), INTERVAL 12 MONTH)";;
	res = stmt->executeQuery(query.c_str());
	res->next();
	cout << "The num of Israel Post Office deliveries is " << res->getString("Total").c_str() << endl;

	delete res;
	delete stmt;
}
void op18(sql::Connection* con) {
	sql::Statement* stmt = con->createStatement();
	sql::ResultSet* res;
	stmt->execute("USE " DB_NAME);

	string query = " \
		SELECt s.* FROM sales s JOIN \
		(SELECT s.SaleID, b.BookGroup, COUNT(*) as num FROM sales s JOIN books b ON s.BookID = b.BookID GROUP BY s.SaleID, b.BookGroup HAVING COUNT(*) > 1) t \
		ON s.SaleID = t.SaleID \
	";

	res = stmt->executeQuery(query.c_str());
	printTable(SalesFieldsList, res);

	delete res;
	delete stmt;
}
void op19(sql::Connection* con) {
	sql::Statement* stmt = con->createStatement();
	sql::ResultSet* res;
	stmt->execute("USE " DB_NAME);

	string query = " \
		SELECT * FROM customers c JOIN \
		(SELECT distinct s.CustomerID FROM sales s WHERE s.CustomerID NOT IN(SELECT distinct CustomerID FROM sales ss WHERE ss.CreatedAt >= DATE_SUB(NOW(), INTERVAL 24 MONTH))) t \
		ON c.CustomerID = t.CustomerID \
	";

	res = stmt->executeQuery(query.c_str());
	printTable(CustomersFieldsList, res);

	delete res;
	delete stmt;
}
void op20(sql::Connection* con) {
	sql::Statement* stmt = con->createStatement();
	sql::ResultSet* res;
	stmt->execute("USE " DB_NAME);

	string query = " \
		SELECT c.* FROM customers c JOIN \
		(SELECT DISTINCT CustomerID FROM orders WHERE Status = 'CONTACT' AND ContactAt < DATE_SUB(NOW(), INTERVAL 14 DAY)) t ON c.CustomerID = t.CustomerID \
	";

	res = stmt->executeQuery(query.c_str());
	printTable(CustomersFieldsList, res);

	delete res;
	delete stmt;
}
void op21(sql::Connection* con) {
	sql::Statement* stmt = con->createStatement();
	sql::ResultSet* res;
	stmt->execute("USE " DB_NAME);

	for (int i = 1; i < 12; i++) {
		string query = "SELECT COUNT(*) as num FROM stock WHERE MONTH(EnteredOn) = " + to_string(i) + " AND Location = 'STOCK'";
		res = stmt->executeQuery(query.c_str());
		res->next();
		cout << "The number of books in stock in month " << i << " is " << res->getString("num").c_str() << endl;
	}

	delete res;
	delete stmt;
}
void op22(sql::Connection* con) {
	sql::Statement* stmt = con->createStatement();
	sql::ResultSet* res;
	stmt->execute("USE " DB_NAME);

	string from;
	string to;
	cout << "Enter from date (yyyy-mm-dd):" << endl;
	cin >> from;
	cout << "Enter to date (yyyy-mm-dd):" << endl;
	cin >> to;

	string query = "SELECT COUNT(*) as counter, SUM(Price) as total FROM shopBuy WHERE BuyAt BETWEEN '" + from + "' AND '" + to + "'";
	cout << query << endl;
	res = stmt->executeQuery(query.c_str());

	res->next();
	cout << "The number of store purchases is " << res->getString("counter").c_str() << endl;
	cout << "The number of total money is " << res->getString("total").c_str() << endl;

	delete res;
	delete stmt;
}
void op23(sql::Connection* con) {
	sql::Statement* stmt = con->createStatement();
	sql::ResultSet* res;
	stmt->execute("USE " DB_NAME);

	string month;
	cout << "Enter month (1-12)" << endl;
	cin >> month;

	string year;
	cout << "Enter year" << endl;
	cin >> year;

	string query = "SELECT ROUND(SUM(Payment),2) as TotalSales FROM sales WHERE MONTH(CreatedAt) = " + month + " AND YEAR(CreatedAt) = " + year;
	res = stmt->executeQuery(query.c_str());
	res->next();
	double totalSales = res->getDouble("TotalSales");


	query = "SELECT ROUND(SUM(Price),2) as TotalBuy FROM shopBuy WHERE MONTH(BuyAt) = " + month + " AND YEAR(BuyAt) = " + year;
	res = stmt->executeQuery(query.c_str());
	res->next();
	double totalBuy = res->getDouble("TotalBuy");

	cout << "The total profit is " << (totalSales - totalBuy) << endl;

	delete res;
	delete stmt;
}
void op24(sql::Connection* con) {
	sql::Statement* stmt = con->createStatement();
	sql::ResultSet* res;
	stmt->execute("USE " DB_NAME);

	for (int i = 1; i < 12; i++) {
		string query = "SELECT ROUND((SUM(Payment)/COUNT(*)),2) as res FROM sales WHERE MONTH(CreatedAt) = " + to_string(i);
		res = stmt->executeQuery(query.c_str());
		res->next();
		string num = res->getString("res").c_str();
		if (num == "") {
			num = "0";
		}
		cout << "The average of deals in month " << i << " is " << num << endl;
	}

	delete res;
	delete stmt;
}
void op25(sql::Connection* con) {
	sql::Statement* stmt = con->createStatement();
	sql::ResultSet* res;
	stmt->execute("USE " DB_NAME);

	string salesmanId;
	cout << "Enter salesman id" << endl;
	cin >> salesmanId;

	string month;
	cout << "Enter month (1-12)" << endl;
	cin >> month;

	string year;
	cout << "Enter year" << endl;
	cin >> year;

	string query = "SELECT NumHours, MoneyPerHour FROM salesmenHours WHERE SalesmenID = " + salesmanId + " AND Year =" + year + " AND Month = " + month;

	res = stmt->executeQuery(query.c_str());
	res->next();
	cout << "The salary is " << res->getInt("NumHours") * res->getInt("MoneyPerHour") << endl;

	delete res;
	delete stmt;
}
void op26(sql::Connection* con) {
	sql::Statement* stmt = con->createStatement();
	sql::ResultSet* res;
	stmt->execute("USE " DB_NAME);

	string month;
	cout << "Enter month (1-12)" << endl;
	cin >> month;

	string year;
	cout << "Enter year" << endl;
	cin >> year;

	string query = " \
		SELECT SalesmenID, COUNT(*) as num \
		FROM sales WHERE MONTH(CreatedAt) = 1 AND YEAR(CreatedAt) = 2020 \
		GROUP BY SalesmenID \
		ORDER BY COUNT(*) DESC LIMIT 1 \
	";
	res = stmt->executeQuery(query.c_str());
	res->next();
	cout << "The top seller is " << res->getInt("SalesmenID") << " with " << res->getInt("num") << " sales" << endl;

	delete res;
	delete stmt;
}

void search_customer(sql::Connection* con)
{
	sql::Statement* stmt = con->createStatement();
	sql::ResultSet* res;
	stmt->execute("USE " DB_NAME);

	cout << "Search by (choose option):" << endl;
	cout << "1) name" << endl;
	cout << "2) phone" << endl;

	int option;
	cin >> option;

	string query;
	if (option == 1)
	{
		cout << "Enter name:" << endl;
		string name;
		cin >> name;

		query = "SELECT * FROM customers WHERE Name like '%" + name + "%'";
	}
	else if (option == 2)
	{
		cout << "Enter phone:" << endl;
		string phone;
		cin >> phone;

		query = "SELECT * FROM customers WHERE Phone like '%" + phone + "%'";
	}

	if (option == 1 || option == 2)
	{
		res = stmt->executeQuery(query.c_str());
		printTable(CustomersFieldsList, res);

		delete res;
	}

	delete stmt;
}

void search_book(sql::Connection* con)
{
	sql::Statement* stmt = con->createStatement();
	sql::ResultSet* res;
	stmt->execute("USE " DB_NAME);

	cout << "Search by (choose option):" << endl;
	cout << "1) title" << endl;
	cout << "2) author" << endl;

	int option;
	cin >> option;

	string query;
	if (option == 1)
	{
		cout << "Enter title:" << endl;
		string title;
		cin >> title;

		query = "SELECT * FROM books WHERE Title like '%" + title + "%'";
	}
	else if (option == 2)
	{
		cout << "Enter author:" << endl;
		string author;
		cin >> author;

		query = "SELECT * FROM books WHERE BookID IN (SELECT DISTINCT b.BookID \
				 FROM books b JOIN booksAuthors ba ON (b.BookID = ba.BookID) \
					  JOIN authors a ON (a.AuthorID = ba.AuthorID) \
			     WHERE a.Name like '%" + author + "%')";
	}

	if (option == 1 || option == 2)
	{
		res = stmt->executeQuery(query.c_str());
		printTable(BooksFieldsList, res);

		delete res;
	}

	delete stmt;
}

bool do_op(sql::Connection* con, int role) {
	/*
	 * Input:
	 * 1  = 1
	 * 2  =
	 * 3  =
	 * 4  =
	 * 5  = 1
	 * 6  = 1990-01-01 2030-01-01
	 * 7  =
	 * 8  =
	 * 9  = 1
	 * 10 = 1
	 * 11 = 1 2 0
	 * 12 = 1
	 * 13 = 1
	 * 14 = 3 2020
	 * 15 = 3 2020
	 * 16 =
	 * 17 =
	 * 18 =
	 * 19 =
	 * 20 =
	 * 21 =
	 * 22 = 1900-09-09 2030-09-09
	 * 23 = 3 2020
	 * 24 =
	 * 25 = 3 3 2020
	 * 26 = 3 2020
	 */

	if (role == 1) {
		cout << "Choose operation (0-28):" << endl;
		cout << "0 = Exit" << endl;
		cout << "1 = Book X exist in stock?" << endl;
		cout << "2 = Who is earliest customer?" << endl;
		cout << "3 = What is the oldest book?" << endl;
		cout << "4 = Print order list" << endl;
		cout << "5 = How many copies of book Y have been sold?" << endl;
		cout << "6 = What is the common author between dates X to Y?" << endl;
		cout << "7 = Who are the 3 customers that bought the largest amount of book ever?" << endl;
		cout << "8 = Who is the book with the largest amount of translations in stock?" << endl;
		cout << "9 = Buying history of customer X" << endl;
		cout << "10 = Show order list of customer X" << endl;
		cout << "11 = Calculate price of delivery" << endl;
		cout << "12 = Does customer X split its purchase to different deliveries?" << endl;
		cout << "13 = Show current status of delivery" << endl;
		cout << "14 = What is the sum of all the deliveries of Xpress in month X?" << endl;
		cout << "15 = What is the sum of all the Bit transfers to the shop in month Y?" << endl;
		cout << "16 = List all deals in the last 12 months, that their profit is bigger than the average" << endl;
		cout << "17 = How many deliveries performed by Israel Post and Xpress in the last 12 months?" << endl;
		cout << "18 = List all the deliveries that include at least 2 editions of the same book" << endl;
		cout << "19 = List all customers that bought a book once, but didn't purchase in the last 24 months" << endl;
		cout << "20 = List all customers that the shop told them that the book is available 14 days ago but they still didn't purchase the book" << endl;
		cout << "21 = Number of books in the stock in each month" << endl;
		cout << "22 = How many books the store purchased between D1 to D2 and what is the total amount of payment?" << endl;
		cout << "23 = What is the profit in year Y and month X?" << endl;
		cout << "24 = Show average of deals in every month" << endl;
		cout << "25 = Show salary of worker Z in month X and year Y" << endl;
		cout << "26 = Who is the top seller in month X and year Y?" << endl;
		cout << "27 = Search customer" << endl;
		cout << "28 = Search book" << endl;

		int option;
		cin >> option;
		switch (option) {
		case 0: return false;
		case 1: op1(con); break;
		case 2: op2(con); break;
		case 3: op3(con); break;
		case 4: op4(con); break;
		case 5: op5(con); break;
		case 6: op6(con); break;
		case 7: op7(con); break;
		case 8: op8(con); break;
		case 9: op9(con); break;
		case 10: op10(con); break;
		case 11: op11(con); break;
		case 12: op12(con); break;
		case 13: op13(con); break;
		case 14: op14(con); break;
		case 15: op15(con); break;
		case 16: op16(con); break;
		case 17: op17(con); break;
		case 18: op18(con); break;
		case 19: op19(con); break;
		case 20: op20(con); break;
		case 21: op21(con); break;
		case 22: op22(con); break;
		case 23: op23(con); break;
		case 24: op24(con); break;
		case 25: op25(con); break;
		case 26: op26(con); break;
		case 27: search_customer(con); break;
		case 28: search_book(con); break;
		}
		return true;
	}

	cout << "Choose operation (0-15):" << endl;
	cout << "0 = Exit" << endl;
	cout << "1 = Book X exist in stock?" << endl;
	cout << "2 = Who is earliest customer?" << endl;
	cout << "3 = What is the oldest book?" << endl;
	cout << "4 = Print order list" << endl;
	cout << "5 = How many copies of book Y have been sold?" << endl;
	cout << "6 = What is the common author between dates X to Y?" << endl;
	cout << "7 = Who are the 3 customers that bought the largest amount of book ever?" << endl;
	cout << "8 = Who is the book with the largest amount of translations in stock?" << endl;
	cout << "9 = Buying history of customer X" << endl;
	cout << "10 = Show order list of customer X" << endl;
	cout << "11 = Calculate price of delivery" << endl;
	cout << "12 = Does customer X split its purchase to different deliveries?" << endl;
	cout << "13 = Show current status of delivery" << endl;
	cout << "14 = Search customer" << endl;
	cout << "15 = Search book" << endl;

	int option;
	cin >> option;
	switch (option) {
	case 0: return false;
	case 1: op1(con); break;
	case 2: op2(con); break;
	case 3: op3(con); break;
	case 4: op4(con); break;
	case 5: op5(con); break;
	case 6: op6(con); break;
	case 7: op7(con); break;
	case 8: op8(con); break;
	case 9: op9(con); break;
	case 10: op10(con); break;
	case 11: op11(con); break;
	case 12: op12(con); break;
	case 13: op13(con); break;
	case 14: search_customer(con); break;
	case 15: search_book(con); break;
	}
	return true;
}

void start(sql::Connection* con)
{
	cout << "Choose role (1=owner, 2=seller):" << endl;
	int role = 0;
	cin >> role;
	while (role != 1 && role != 2) {
		cout << "Choose role (1=owner, 2=seller):" << endl;
		cin >> role;
	}
	while (do_op(con, role)) {}
}

int main(int argc, char* argv[])
{
	sql::Driver* driver;
	sql::Connection* con;

	driver = get_driver_instance();
	con = driver->connect("tcp://127.0.0.1:3307", "root", "");

	build_db(con);
	start(con);

	delete con;
}

