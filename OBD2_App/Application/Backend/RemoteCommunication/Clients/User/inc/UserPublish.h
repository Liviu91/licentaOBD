#ifndef USERPUBLISH_H
#define USERPUBLISH_H

#include <mqtt/client.h>
#include <string>
#include <memory>

class UserPublish {
public:
    UserPublish();
    ~UserPublish();
<<<<<<< HEAD
=======
    std::unique_ptr<mqtt::client> client;
>>>>>>> c3d9484 (MQTT connection exception: MQTT error [-1]: TCP/TLS connect failure)
    bool Publish(const std::string& topic, const std::string& payload);
    void Connect();

private:
<<<<<<< HEAD
    std::unique_ptr<mqtt::async_client> client;
    const std::string serverAddress = "tcp://localhost:1884"; // Update if needed
=======

    
    const std::string serverAddress = "tcp://localhost:1884";
>>>>>>> c3d9484 (MQTT connection exception: MQTT error [-1]: TCP/TLS connect failure)
    const std::string clientId = "UserPublisher";
    mqtt::connect_options connOpts;

};

#endif // USERPUBLISH_H
