#pragma once



#include <string>


#include <nlohmann/json.hpp> //For Json


using json = nlohmann::json;

class DatabaseManager {
public:
    DatabaseManager(const std::string& databasePath);
    ~DatabaseManager();


    bool VerifyCredentials(const std::string& username, const std::string& password);


    std::string GetUserRole(const std::string& username);

private:

    void GenerateDefaultDatabase(); //Private function for initial setup




    std::string databasePath;
    json database;



    bool databaseExists;
};
