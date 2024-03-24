#include <iostream>
#include <pqxx/pqxx>
#include <Windows.h>
#pragma execution_character_set( "utf-8" )

class Tablework {

public:
	Tablework(){

	}

	void createtable(pqxx::connection& conn) {

		pqxx::transaction tx(conn);

		static const char* query = "CREATE TABLE if not exists client(     \
		Client_id serial PRIMARY key,                                      \
		Name text NOT NULL,                                                \
		Surname text NOT NULL,                                             \
		Email text NOT NULL                                                \
	    );";

		tx.exec(query);

		tx.commit();
	}

	void createtablephone(pqxx::connection& conn) {
		pqxx::transaction tx(conn);

		static const char* query = "CREATE TABLE if not exists phoneclient(     \
        Client_id INTEGER NOT NULL REFERENCES client(Client_id),                \
        Telephone BIGINT                                                        \
	    );";

		tx.exec(query);

		tx.commit();

	}

	void newclient(pqxx::connection& conn, std::string name, std::string surname, std::string email) {
		pqxx::work tx{ conn };
		tx.exec("INSERT INTO client(Name, Surname, Email) "			
		"VALUES('" + tx.esc(name) + "', '" + tx.esc(surname) + "' , '" + tx.esc(email) + "'  )");
		tx.commit();
	}

	void telephone(pqxx::connection& conn, int id, long long telephone) {
		pqxx::work tx{ conn };
		tx.exec(" INSERT INTO phoneclient (client_id, telephone)" 
			"VALUES( " + tx.esc(std::to_string(id)) + ", " + tx.esc(std::to_string(telephone)) + "  )");
			
		tx.commit();
	}

	void Updateclient(pqxx::connection& conn, std::string email, std::string newname, std::string newsurname, std::string newemail) {
		pqxx::work tx{ conn };
		tx.exec("Update client SET Name = '" + tx.esc(newname) + "' ,Surname = '" + tx.esc(newsurname) + "', Email = '" + tx.esc(newemail) + "' "
			"WHERE Email = '" + tx.esc(email) + "'");
		tx.commit();
	}

	void deletetelephone(pqxx::connection& conn, int id) {
		pqxx::work tx{ conn };
		tx.exec(" Update phoneclient SET Telephone  = " + tx.esc(std::to_string(0)) + " "
			"WHERE client_id = '" + tx.esc(std::to_string(id)) + "'");
		tx.commit();
	}

	void deleteclient(pqxx::connection& conn, int id) {
		pqxx::work tx{ conn };
		tx.exec(" DELETE From phoneclient "
			"WHERE client_id = " + tx.esc(std::to_string(id)) + "");
		tx.exec("DELETE From client "
			"WHERE client_id = " + tx.esc(std::to_string(id)) + "");
		tx.commit();
	}
	
	void selectclient(pqxx::connection& conn, std::string name) {
		pqxx::work tx{ conn };

		auto collection = tx.query<std::string, std::string, std::string, long long>("Select Name, Surname, Email, Telephone From client c join phoneclient pc on c.client_id = pc.client_id "
			"WHERE Name = '" + tx.esc(name) + "' OR Surname = '" + tx.esc(name) + "' OR Email = '" + tx.esc(name) + "' ");

		
		for (auto elem : collection) 
		{

			std::cout << "Name: " << std::get<0>(elem) << "  ";
			std::cout << "Surname: " << std::get<1>(elem) << "  ";
			std::cout << "Email: " << std::get<2>(elem) << "  ";
			std::cout << "Telephone: " << std::get<3>(elem) << std::endl;
		}

		tx.commit();
	}

	void selectclient(pqxx::connection& conn, long long tel) {
		pqxx::work tx{ conn };

		auto collection = tx.query<std::string, std::string, std::string, long long>("Select Name, Surname, Email, Telephone From client c join phoneclient pc on c.client_id = pc.client_id "
			"WHERE Telephone = " + tx.esc(std::to_string(tel)) + " ");


		for (auto elem : collection)
		{

			std::cout << "Name: " << std::get<0>(elem) << "  ";
			std::cout << "Surname: " << std::get<1>(elem) << "  ";
			std::cout << "Email: " << std::get<2>(elem) << "  ";
			std::cout << "Telephone: " << std::get<3>(elem) << std::endl;
		}

		tx.commit();
	}

};



int main(){

	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	setvbuf(stdout, nullptr, _IOFBF, 1000);

	
	try {
		pqxx::connection conn(
			"host=localhost "
			"port=5432 "
			"dbname=client "
			"user=postgres "
			"password=0406Dbnfkz2005 ");

		std::cout << "âààâ";
		Tablework table;
		table.createtable(conn);

		std::cout << "Table created" << std::endl;

		table.createtablephone(conn);

		std::cout << "TablePhone created" << std::endl;

		table.newclient(conn, "Vitaliy", "ËÎðíîâ", "vitaliy@gmail.com");
		
		std::cout << "New client created" << std::endl;

		table.telephone(conn, 1, 79387286482);

		std::cout << "Telephone created" << std::endl;

		table.Updateclient(conn, "vitaliy@gmail.com", "Michael", "Peren", "Michael@gmail.com");
		
		std::cout << "Update client" << std::endl;

		table.selectclient(conn, 79387286482);

		std::cout << "Client select" << std::endl;

		table.deletetelephone(conn, 1);

		std::cout << "Telephone deleted" << std::endl;

		table.deleteclient(conn, 1);

		std::cout << "Client deleted" << std::endl;
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	return 0;

} 