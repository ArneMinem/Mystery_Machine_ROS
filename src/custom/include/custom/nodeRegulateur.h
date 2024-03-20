//
// Created by matti on 20/03/2024.
//

#ifndef PROJET_RC_NODEREGULATEUR_H
#define PROJET_RC_NODEREGULATEUR_H


class nodeRegulateur : public rclcpp::Node {
public:
    string etatTopicName = "";
    string positionTopicName = "";
    manualTopicName = "";
    commandTopicName = "";

    void callbackTimer()
};


#endif //PROJET_RC_NODEREGULATEUR_H
