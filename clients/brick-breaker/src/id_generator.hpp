#pragma once

class IdGenerator {
public:
    unsigned int generate() {
        return current_id++;
    }
private:
    unsigned int current_id {0};
};
