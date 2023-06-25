#pragma once
const int MAX_WEIGHT = 100; //placeholder
const int DISTANCE_BETWEEN_FLOORS = 150;
enum DIRECTION {UP,DOWN,STOP,LEFT,RIGHT};
class Person {
public:
    int weight;
    int x;
    int y;
    int origin;
    int destination;
    int goal_x;
    DIRECTION direction;
    
    Person(int dest, int org, int x, int y) {
        this->weight = 70;
        this->destination = dest;
        this->origin = org;
        this->x = x;
        this->y = y;
        this->direction = STOP;
    }
};
class Elevator {
public:
    Elevator();
    int SetDestination(int d);
    int GetFloor();
    void Movement(vector<Person>* persons);
    int GetDestination();
    int GetPositionY();
    void addToQueue(Person person);
    void requestHandler(vector<Person> *persons);
    void ChangeDirection();
    vector<Person> peopleInElevator;
private:
    int floor;
    int destination;
    bool passenger_got;
    int max_weight;
    int velocity;
    int y;
   
    DIRECTION direction;
    vector<int> queue;

    
};
Elevator::Elevator() {
    this->floor = 0;
    this->max_weight = MAX_WEIGHT;
    this->velocity = 3;
    this->y = 0;
    this->destination = 0;
    this->passenger_got = false;
    this->direction = UP;
}
int Elevator::GetPositionY() {
    return this->y;
}
int Elevator::SetDestination(int d) {
    this->destination = d;
    return destination;
}
int Elevator::GetDestination() {
    return destination;
}
int Elevator::GetFloor(){
    return floor;
}

void Elevator::addToQueue(Person person) {
    if (queue.empty()) {
        if (person.origin < floor) direction = DOWN;
        else if (person.origin > floor) direction = UP;
        else direction = STOP;
        queue.push_back(person.origin);
        queue.push_back(person.destination);
        return;
    }
    //TODO: lepszy algorytm dla windy
    queue.push_back(person.origin);
    queue.push_back(person.destination);
}
void Elevator::requestHandler(vector<Person> *persons) {
    if (queue.empty()) return;
    if (direction != STOP) {
        destination = queue.front();
        if (floor == destination) {
            queue.erase(queue.begin());
            ChangeDirection();
        }
    }
}
void Elevator::ChangeDirection() {
    if (queue.empty()) direction = STOP;
    else if (queue.front() < floor) direction = DOWN;
    else if (queue.front() > floor) direction = UP;
    for (auto& peep : peopleInElevator) {
        peep.direction = direction;
    }
}
void Elevator::Movement(vector<Person>* persons) {
    //elevator movement
    if (y < destination * DISTANCE_BETWEEN_FLOORS) y += velocity;
    else if (y > destination * DISTANCE_BETWEEN_FLOORS) y -= velocity;
    else {
        floor = destination;
        ChangeDirection();
        //people going in
        for (auto& peep : persons[floor]) {
            if (peep.direction == STOP) {
                if (floor % 2 == 0) {
                    peep.direction = RIGHT;
                    peep.goal_x = peep.x + 350;
                }
                else {
                    peep.direction = LEFT;
                    peep.goal_x = peep.x - 350;
                }
            }
        }
        //people going out
        for (auto& peep : peopleInElevator) {
            if (peep.destination == floor) {
                if (floor % 2 != 0) {
                    peep.direction = RIGHT;
                    peep.goal_x = peep.x + 350;
                }
                else {
                    peep.direction = LEFT;
                    peep.goal_x = peep.x - 350;
                }
                persons[floor].push_back(peep);
            }
        }

    }
    //person movement, only care about the people that are on the same floor as elevator or are inside it
    for (auto& peep : persons[floor]) {
        switch (peep.direction) {
        case LEFT:
            peep.x -= velocity;
            if (peep.x <= peep.goal_x) {
                peopleInElevator.push_back(peep);
            }
            break;
        case RIGHT:
            peep.x += velocity;
            if (peep.x >= peep.goal_x) {
                peopleInElevator.push_back(peep);
            }
            break;
        case STOP:
            break;
        }
    }
    // Erase-Remove Idiom algorithm to avoid vector going out of range when erasing elements in loop
    persons[floor].erase(
        std::remove_if(
            persons[floor].begin(),
            persons[floor].end(),
            [](Person const& p) { return (p.x <= p.goal_x && p.direction == LEFT) || (p.x >= p.goal_x && p.direction == RIGHT); }
        ),
        persons[floor].end()
    );
    for (auto& peep : peopleInElevator) {
        switch (peep.direction) {
        case UP:
            peep.y -= velocity;
            break;
        case DOWN:
            peep.y += velocity;
            break;
        case STOP:
            break;
        case LEFT:
            peep.x -= velocity;
            break;
        case RIGHT:
            peep.x += velocity;
            break;
        }
    }
    peopleInElevator.erase(
        std::remove_if(
            peopleInElevator.begin(),
            peopleInElevator.end(),
            [&](Person const& p) { return p.destination == floor; }
        ),
        peopleInElevator.end()
    );
    if (persons[floor].empty() && !queue.empty()) {
        ChangeDirection();
        requestHandler(persons);
    }

}
