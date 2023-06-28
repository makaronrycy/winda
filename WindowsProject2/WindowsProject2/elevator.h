#pragma once
const int MAX_WEIGHT = 600; 
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
        this->goal_x = 0;
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
    void addToQueue(int request);
    void ChangeDirection();
    void sortQueue();
    void DeleteFrontRequest();
    int GetWeight();
    void Clear();
    vector<Person> peopleInElevator;
    vector<int> queue;
private:
    int floor;
    int destination;
    bool passenger_got;
    int max_weight;
    int velocity;
    int y;
    int weight;
    DIRECTION direction;
};
Elevator::Elevator() {
    this->floor = 0;
    this->max_weight = MAX_WEIGHT;
    this->velocity = 1;
    this->y = 0;
    this->destination = 0;
    this->passenger_got = false;
    this->direction = STOP;
    this->weight = 0;
}
void Elevator::Clear() {
    this->floor = 0;
    this->max_weight = MAX_WEIGHT;
    this->velocity = 1;
    this->y = 0;
    this->destination = 0;
    this->passenger_got = false;
    this->direction = STOP;
    this->weight = 0;
    peopleInElevator.clear();
    queue.clear();
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

void Elevator::addToQueue(int request) {
    if (queue.empty()) {
        if (request < floor) direction = DOWN;
        else if (request > floor) direction = UP;
        queue.push_back(request);
        return;
    }
    queue.push_back(request);
    sortQueue();
}
void Elevator::sortQueue() {
    int cur_track;
    vector<int> up, down;
    int head = floor;

    for (int i = 0; i < queue.size(); i++) {
        if (queue[i] < head)
            down.push_back(queue[i]);
        if (queue[i] > head)
            up.push_back(queue[i]);
        if (queue[i] == head && direction == UP)
            down.push_back(queue[i]);
    }
    DIRECTION Sdirection = direction;
    // sorting left and right vectors
    std::sort(up.begin(), up.end());
    std::sort(down.begin(), down.end());

    if (Sdirection == STOP) {
        if (queue.front() > floor) Sdirection = UP;
        else Sdirection = DOWN;
    }
    // run the while loop two times.
    // one by one scanning right
    // and left of the head
    queue.clear();
    int run = 2;
    while (run != 0) {
        if (Sdirection == DOWN) {
            for (int i = down.size() - 1; i >= 0; i--) {
                cur_track = down[i];
                // appending current track to seek sequence
                queue.push_back(cur_track);
            }
            Sdirection = UP;
        }
        else if (Sdirection == UP) {
            for (int i = 0; i < up.size(); i++) {
                cur_track = up[i];
                // appending current track to seek sequence
                queue.push_back(cur_track);
            }
            Sdirection = DOWN;
        }
        run--;
    }
}
void Elevator::ChangeDirection() {
    if (queue.empty()) direction = STOP;
    else if (destination < floor) {
        direction = DOWN;
        //sortQueue();
    }
    else if (destination > floor) {
        direction = UP;
        //sortQueue();
    }
    else if (destination == floor) direction = STOP;
    for (auto& peep : peopleInElevator) {
        peep.direction = direction;
    }
}
void Elevator::DeleteFrontRequest() {
    if (!queue.empty()) queue.erase(queue.begin());
}
int Elevator::GetWeight() {
    return this->weight;
}
void Elevator::Movement(vector<Person>* persons) {
    //elevator movement
    if (y < destination * DISTANCE_BETWEEN_FLOORS-velocity) y += velocity;
    else if (y > destination * DISTANCE_BETWEEN_FLOORS+velocity) y -= velocity;
    else {
        floor = destination;
        ChangeDirection();
        //people going in
        for (auto& peep : persons[floor]) {
            if (peep.direction == STOP) {
                if (floor % 2 == 0) {
                    peep.direction = RIGHT;
                    int x = peopleInElevator.size();
                    peep.goal_x = peep.x + 400+ (x*10);
                }
                else {
                    peep.direction = LEFT;
                    int x = peopleInElevator.size();
                    peep.goal_x = peep.x - 350 - (x*10);
                }
            }
        }
        //people going out
        for (auto& peep : peopleInElevator) {
            if (peep.destination == floor) {
                weight -= peep.weight;
                if (floor % 2 != 0) {
                    peep.direction = RIGHT;
                    peep.goal_x = peep.x + 350;
                }
                else {
                    peep.direction = LEFT;
                    peep.goal_x = peep.x - 350;
                }
                DeleteFrontRequest();
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
                //only execute when going into elevator
                if (floor != peep.destination) {
                    peopleInElevator.push_back(peep);
                    addToQueue(peep.destination);
                    weight += peep.weight;
                }
            }
            break;
        case RIGHT:
            peep.x += velocity;
            if (peep.x >= peep.goal_x) {
                //only execute when going into elevator
                if (floor != peep.destination) {
                    peopleInElevator.push_back(peep);
                    addToQueue(peep.destination);
                    weight += peep.weight;
                }
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
        peep.direction = direction;
        switch (peep.direction) {
        case UP:
            peep.y -= velocity;
            break;
        case DOWN:
            peep.y += velocity;
            break;
        case STOP:
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
        destination = queue.front();
        ChangeDirection();
    }
}
