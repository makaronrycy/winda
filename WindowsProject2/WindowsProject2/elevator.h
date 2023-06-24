#pragma once
const int MAX_WEIGHT = 100; //placeholder
const int DISTANCE_BETWEEN_FLOORS = 150;
class Elevator {
public:
    Elevator();
    int GetPositionY();
    int SetPositionY(int y);
    int SetDestination(int d);
    int SetOrigin(int o);
    int GetFloor();
    bool Movement();
    bool SetisAscending();
private:
    int floor;
    int destination;
    int origin;
    bool passenger_got;
    int max_weight;
    int velocity;
    int rel_pos_y;
    bool isAscending;
};
Elevator::Elevator() {
    this->floor = 0;
    this->max_weight = MAX_WEIGHT;
    this->velocity = 3;
    this->rel_pos_y = 0;
    this->destination = 0;
    this->origin = 0;
    this->passenger_got = false;
    this->isAscending = true;
}
int Elevator::GetPositionY() {
    return rel_pos_y;
}
int Elevator::SetPositionY(int y) {
    this->rel_pos_y = y;
    return rel_pos_y;
}
int Elevator::SetDestination(int d) {
    this->destination = d;
    return destination;
}
int Elevator::SetOrigin(int o) {
    this->origin = o;
    return origin;
}
int Elevator::GetFloor() {
    return floor;
}
bool Elevator::SetisAscending() {
    return !isAscending;
}
bool Elevator::Movement() {
    if (origin == destination) return false;
    if (!passenger_got) {
        if (rel_pos_y < origin * DISTANCE_BETWEEN_FLOORS) SetPositionY(rel_pos_y + velocity);
        else if (rel_pos_y > origin * DISTANCE_BETWEEN_FLOORS) SetPositionY(rel_pos_y - velocity);
        else {
            floor = origin;
            passenger_got = true;
        }
    }
    else {
        if (rel_pos_y < destination * DISTANCE_BETWEEN_FLOORS) SetPositionY(rel_pos_y + velocity);
        else if (rel_pos_y > destination * DISTANCE_BETWEEN_FLOORS) SetPositionY(rel_pos_y - velocity);
        else {
            floor = destination;
            origin = destination;
            passenger_got = false;
        }
    }
    return true;
}