// UserPublish.cpp
#include "UserPublish.h"
#include <mqtt/client.h>
#include <iostream>

UserPublish::UserPublish() {
    client = std::make_unique<mqtt::async_client>(serverAddress, clientId);
    Connect();
}

UserPublish::~UserPublish() {
    if (client && client->is_connected()) {
        client->disconnect()->wait();
    }
}

void UserPublish::Connect() {
    try {
        mqtt::connect_options connOpts;
        connOpts.set_clean_session(true);
        auto token = client->connect(connOpts);
        token->wait(); // Wait for connection
        if (!client->is_connected()) {
            std::cerr << "MQTT connection failed!" << std::endl;
        }
    }
    catch (const mqtt::exception& exc) {
        std::cerr << "Error connecting to MQTT broker: " << exc.what() << std::endl;
    }
}

bool UserPublish::Publish(const std::string& topic, const std::string& payload) {
  //  client = std::make_unique<mqtt::async_client>(serverAddress, clientId);
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
