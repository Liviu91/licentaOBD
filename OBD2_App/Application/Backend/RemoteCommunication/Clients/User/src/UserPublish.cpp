// UserPublish.cpp
#include "UserPublish.h"
#include <mqtt/client.h>
#include <iostream>

UserPublish::UserPublish()
    : client(std::make_unique<mqtt::client>(serverAddress, clientId)),
    connOpts()
{ 
    connOpts.set_clean_session(true);
    connOpts.set_keep_alive_interval(20);
    connOpts.set_mqtt_version(MQTTVERSION_3_1_1);
}
UserPublish::~UserPublish() {
    try {
        if (client && client->is_connected()) {
            client->disconnect();
        }
    }
    catch (const mqtt::exception& exc) {
        std::cerr << "MQTT disconnection exception: " << exc.what() << std::endl;
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
        std::cerr << "MQTT connection exception (" << exc.get_reason_code() << "): " << exc.what() << std::endl;
    }
    catch (const std::exception& exc) {
        std::cerr << "General exception: " << exc.what() << std::endl;
    }
}

bool UserPublish::Publish(const std::string& topic, const std::string& payload) {
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
