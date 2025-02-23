// UserPublish.cpp
#include "UserPublish.h"
#include <mqtt/client.h>
#include <iostream>

<<<<<<< HEAD
UserPublish::UserPublish() {
    client = std::make_unique<mqtt::async_client>(serverAddress, clientId);
    Connect();
}

UserPublish::~UserPublish() {
    if (client && client->is_connected()) {
        client->disconnect()->wait();
=======
UserPublish::UserPublish()
    : client(std::make_unique<mqtt::client>(serverAddress, clientId)),
    connOpts()
{
    connOpts.set_clean_session(true);
}

UserPublish::~UserPublish() {
    try {
        if (client && client->is_connected()) {
            client->disconnect();
        }
    }
    catch (const mqtt::exception& exc) {
        std::cerr << "MQTT disconnection exception: " << exc.what() << std::endl;
>>>>>>> c3d9484 (MQTT connection exception: MQTT error [-1]: TCP/TLS connect failure)
    }
}

void UserPublish::Connect() {
    try {
        if (!client) {
            std::cerr << "MQTT client not initialized!" << std::endl;
            return;
        }

        if (!client->is_connected()) {
            client->connect(connOpts);
            std::cerr << "Successfully connected to MQTT broker." << std::endl;
        }
    }
    catch (const mqtt::exception& exc) {
        std::cerr << "MQTT connection exception: " << exc.what() << std::endl;
    }
}

bool UserPublish::Publish(const std::string& topic, const std::string& payload) {
<<<<<<< HEAD
  //  client = std::make_unique<mqtt::async_client>(serverAddress, clientId);
=======
>>>>>>> c3d9484 (MQTT connection exception: MQTT error [-1]: TCP/TLS connect failure)
    //if (!client->is_connected()) {
    //    std::cerr << "MQTT client is not connected. Attempting to reconnect..." << std::endl;
    //    //Connect();
    //    if (!client->is_connected()) {
    //        std::cerr << "Failed to reconnect to MQTT broker." << std::endl;
    //        return false;
    //    }
    //}

    //try {
    //    mqtt::message_ptr pubmsg = mqtt::make_message(topic, payload);
    //    pubmsg->set_qos(1);
    //    auto token = client->publish(pubmsg);
    //    token->wait(); // Ensure the message is sent before proceeding
    //    return true;
    //}
    //catch (const mqtt::exception& exc) {
    //    std::cerr << "Error publishing message: " << exc.what() << std::endl;
    //    return false;
    //}
    return true;
}
