// UserPublish.cpp
#include "UserPublish.h"
#include <mqtt/client.h>
#include <iostream>

UserPublish::UserPublish() {
    client = new mqtt::async_client(serverAddress, clientId);
    Connect();
}

UserPublish::~UserPublish() {
    if (client->is_connected()) {
        client->disconnect()->wait();
    }
    delete client;
}

void UserPublish::Connect() {
    try {
        mqtt::connect_options connOpts;
        connOpts.set_clean_session(true);
        client->connect(connOpts)->wait();
    }
    catch (const mqtt::exception& exc) {
        std::cerr << "Error connecting to MQTT broker: " << exc.what() << std::endl;
    }
}

bool UserPublish::Publish(const std::string& topic, const std::string& payload) {
    if (!client->is_connected()) {
        std::cerr << "MQTT client is not connected. Attempting to reconnect..." << std::endl;
        Connect();
        if (!client->is_connected()) {
            std::cerr << "Failed to reconnect to MQTT broker." << std::endl;
            return false;
        }
    }

    try {
        mqtt::message_ptr pubmsg = mqtt::make_message(topic, payload);
        pubmsg->set_qos(1);
        client->publish(pubmsg)->wait();
        return true;
    }
    catch (const mqtt::exception& exc) {
        std::cerr << "Error publishing message: " << exc.what() << std::endl;
        return false;
    }
}
