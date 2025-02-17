#include "DatabaseManager.h"
#include <iostream>
#include <fstream> //for file operations if using file as a database
#include <string>

#include <nlohmann/json.hpp> //For Json if you use it as a database (add dependency)
#include <filesystem> // If using files for the database. For checking if database exists


using json = nlohmann::json; //Make json easier to use


DatabaseManager::DatabaseManager(const std::string& databasePath) : databasePath(databasePath), databaseExists(false)
{

    if (std::filesystem::exists(databasePath))
    {
        try
        {
            std::ifstream inputFile(databasePath);
            inputFile >> database;


            databaseExists = !database.empty();



        }
        catch (const json::parse_error& ex)
        {
            // Handle JSON parsing error. Consider error message or logging
            std::cerr << "Error parsing database JSON: " << ex.what() << std::endl;


            databaseExists = false;

        }




    }






}

DatabaseManager::~DatabaseManager()
{

    if (databaseExists)
    {


        try {
            std::ofstream outputFile(databasePath); // Overwrite existing file


            outputFile << std::setw(4) << database << std::endl;

        }
        catch (const json::parse_error& ex)
        {
            std::cerr << "Error updating database: " << ex.what() << std::endl;




        }



    }

}



bool DatabaseManager::VerifyCredentials(const std::string& username, const std::string& password)
{

    if (!databaseExists)
    {

        GenerateDefaultDatabase(); // Call a function to set up initial values or from an external file

    }




    for (const auto& user : database["users"])
    {


        if (user["username"] == username && user["password"] == password) // Consider using more secure password hashing!
        {


            return true;
        }



    }

    return false;

}

std::string DatabaseManager::GetUserRole(const std::string& username)
{
    if (databaseExists)
    {

        for (const auto& user : database["users"])
        {

            if (user["username"] == username)
            {

                return user["role"];

            }


        }


    }

    return ""; // Or throw an exception, return "unknown", etc.
}

void DatabaseManager::GenerateDefaultDatabase()
{


    // Example of adding default users to your database, very important if the file does not exist to generate the structure of the json

    database["users"] = json::array(); // Create an empty array of users



    json user1;

    user1["username"] = "user1";

    user1["password"] = "user1"; //Consider hashing

    user1["role"] = "user";



    database["users"].push_back(user1);


    json technician1;

    technician1["username"] = "technician1";

    technician1["password"] = "technician1"; //Consider hashing

    technician1["role"] = "technician";


    database["users"].push_back(technician1);




    //Similar for technician2 and user2
    databaseExists = true; //Database is created
}