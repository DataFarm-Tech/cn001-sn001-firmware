#include "Node.hpp"
#include "esp_system.h"  // for esp_random()
#include <string>

Node::Node() {
    // Generate nodeID automatically in constructor
    char c;
    uint32_t rand_val;
    size_t i;
    const size_t id_len = 6;

    for (i = 0; i < id_len; ++i) {
        rand_val = esp_random() % 62; // 10 digits + 26 uppercase + 26 lowercase

        if (rand_val < 10) {
            c = '0' + rand_val;           // digits 0-9
        } else if (rand_val < 36) {
            c = 'A' + (rand_val - 10);    // uppercase A-Z
        } else {
            c = 'a' + (rand_val - 36);    // lowercase a-z
        }

        nodeID += c;
    }
}

std::string Node::getNodeID() const {
    return nodeID;
}
