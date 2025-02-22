// UserPublish.h
#ifndef USERPUBLISH_H
#define USERPUBLISH_H

#include <mqtt/async_client.h>
#include <string>

class UserPublish {
public:
    UserPublish();
    //~UserPublish();
    bool Publish(const std::string& topic, const std::string& payload);

private:
    mqtt::async_client* client;
    const std::string serverAddress = "tcp://localhost:1884"; // Update if needed
    const std::string clientId = "UserPublisher";
    void Connect();
};

#endif // USERPUBLISH_H
