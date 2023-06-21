#pragma once
const int MAX_WEIGHT = 100; //placeholder
class Elevator {
public:
    Elevator();
    int floor;
    int dest;
    int origin;
    int GetPositionY();
    int SetPositionY(int y);
private:
    int max_weight;
    float velocity;
    float rel_pos_y;
};
Elevator::Elevator() {
    this->floor = 0;
    this->max_weight = MAX_WEIGHT;
    this->velocity = 1.0;
    this->rel_pos_y = 0;
    this->dest = 0;
    this->origin = 0;
}
int Elevator::GetPositionY() {
    return rel_pos_y;
}
int Elevator::SetPositionY(int y) {
    this->rel_pos_y = y;
    return rel_pos_y;
}