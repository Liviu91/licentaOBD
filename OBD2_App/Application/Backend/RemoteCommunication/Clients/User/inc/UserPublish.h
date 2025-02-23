#ifndef USERPUBLISH_H
#define USERPUBLISH_H

#include <mqtt/client.h>
#include <string>
#include <memory>

class UserPublish {
public:
    UserPublish();
    ~UserPublish();
    std::unique_ptr<mqtt::client> client;
    bool Publish(const std::string& topic, const std::string& payload);
    void Connect();

private:

    
    const std::string serverAddress = "tcp://127.0.0.1:1884";// "tcp://localhost:1884";
    const std::string clientId = "UserPublisher";
    mqtt::connect_options connOpts;

};

#endif // USERPUBLISH_H
