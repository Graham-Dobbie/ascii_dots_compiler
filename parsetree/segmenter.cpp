#include "segmenter.hpp"

namespace segmenter {

Cord::Cord() {
    this->x = 0;
    this->y = 0;
};

Cord::Cord(int x, int y) {
    this->x = x;
    this->y = y;
};

bool Cord::operator==(const Cord &v) {
    if ((this->x == v.x) and (this->y == v.y)) {
        return true;
    } else {
        return false;
    }
}

Cord Cord::operator+(const Cord &v) { return Cord(this->x + v.x, this->y + v.y); }

Cord Cord::operator-(const Cord &v) { return Cord(this->x - v.x, this->y - v.y); }

Cord Cord::operator*(const int s) { return Cord(this->x * s, this->y * s); }

std::vector<Cord> Cord::getNeighbors() {
    std::vector<Cord> cords;
    cords.push_back(Cord(this->x + 1, this->y));
    cords.push_back(Cord(this->x - 1, this->y));
    cords.push_back(Cord(this->x, this->y + 1));
    cords.push_back(Cord(this->x, this->y - 1));
    return cords;
};

void Cord::print() { std::cout << this->x << "," << this->y << std::endl; }

Segment::Segment(std::string type, std::vector<Cord> inputs, std::vector<Cord> outputs, std::vector<char> text_data) {

    this->type = type;
    this->cords = inputs;
    this->outlets = outputs;
    this->text_data = text_data;
};

Segment::Segment() {
    this->type = "END";
    this->text_data;
}

Segment::Segment(std::string type, std::vector<char> text_data) {
    this->type = type;
    this->text_data = text_data;
}

bool Segment::isEnd() {
    if (this->type == "END") {
        return true;
    } else {
        return false;
    }
}

void Segment::print() {

    std::cout << this->type << ":";
    for (int i = 0; i < this->text_data.size(); i++) {
        std::cout << this->text_data[i];
    }
    std::cout << std::endl;
}

Segmenter::Segmenter() {}

Segmenter::Segmenter(std::vector<std::string> raw_text) {

    for (int i = 0; i < raw_text.size(); i++) {
        std::vector<char> app_vec;
        for (int j = 0; j < raw_text[i].size(); j++) {
            app_vec.push_back(raw_text[i][j]);
        }
        this->text.push_back(app_vec);
    }

    for (int i = 0; i < text.size(); i++) {
        for (int j = 0; j < text[i].size(); j++) {
            std::cout << text[i][j];
        }
        std::cout << std::endl;
    }
}

Cord *Segmenter::_findChar() { //search text for a non space (' ') char
    Cord *c = new Cord;
    for (int i = 0; i < this->text.size(); i++) {
        for (int j = 0; j < this->text[i].size(); j++) {
            if (not(text[i][j] == ' ')) {

                c->x = i;
                c->y = j;
                return c;
            }
        }
    }
    return c;
}

std::vector<Cord> Segmenter::_validNieghbors(Cord c) {
    std::vector<Cord> untested_neighbors = c.getNeighbors();
    std::vector<Cord> neighbors;
    for (int i = 0; i < untested_neighbors.size(); i++) {
        Cord neighbor = untested_neighbors[i];
        if ((neighbor.x >= 0) and (neighbor.x < this->text.size())) {
            if ((neighbor.y >= 0) and (neighbor.y < this->text[neighbor.x].size())) {
                neighbors.push_back(neighbor);
            }
        }
    }
    return neighbors;
}

// Cord Segmenter::_step(Cord last, Cord current) {

//     char c = this->text[current.x][current.y];

//     char direction_pipes[] = {'/', '\\'};
//     char merger_pipes[] = {'*', '>', '<', 'v', '^', '(', ')'};

//     if (std::find(direction_pipes, direction_pipes, c) != 0) {

//         if (c == '/') {
//             Cord direction = current - last;
//             if ((direction.x == 1) and (direction.y == 0)) {
//                 return current + Cord(0, -1);
//             } else if ((direction.x == -1) and (direction.y == 0)) {
//                 return current + Cord(0, 1);
//             } else if ((direction.x == 0) and (direction.y == 1)) {
//                 return current + Cord(1, 0);
//             } else if ((direction.x == 0) and (direction.y == 1)) {
//                 return current + Cord(-1, 0);
//             } else {
//                 std::cout << "Bad Cord" << std::endl;
//                 return Cord(-1, -1);
//             }
//         }

//         else if (c == '\\') {
//             Cord direction = current - last;
//             if ((direction.x == 1) and (direction.y == 0)) {
//                 return current + Cord(0, 1);
//             } else if ((direction.x == -1) and (direction.y == 0)) {
//                 return current + Cord(0, -1);
//             } else if ((direction.x == 0) and (direction.y == 1)) {
//                 return current + Cord(-1, 0);
//             } else if ((direction.x == 0) and (direction.y == 1)) {
//                 return current + Cord(1, 0);
//             } else {
//                 throw std::
//                 std::cout << "Bad Cord" << std::endl;

//             }
//         }
//     }

//     if

//         std
//         ::vector<Cord> untested_neighbors = current.getNeighbors();
//     std::vector<Cord> neighbors;
// }

Segment Segmenter::_followDirection(Cord *start, bool direction) {
    Segment seg = Segment();
    seg.type = "UNORDERED";

    Cord *current_cord = start;
    Cord *previous_cord = new Cord(-1, -1);
    bool broken = false;

    while (!broken) {
        char c = text[current_cord->x][current_cord->y];
        if (c == '.') { // if the current char is a start

            seg.cords.push_back(*current_cord);
            seg.outlets.push_back(*current_cord);
            std::vector<Cord> neighbors = this->_validNieghbors(*current_cord);

            for (int i = neighbors.size(); i >= 1; i--) {

                Cord n_cord = neighbors[i - 1];
                char n_c = text[n_cord.x][n_cord.y];

                if (n_c == '-') {
                    if (n_cord.x != current_cord->x) {
                        neighbors.pop_back();
                    }
                } else if (n_c == '|') {
                    if (n_cord.y != current_cord->y) {
                        neighbors.pop_back();
                    }
                } else {
                    neighbors.pop_back();
                }
            }

            if ((neighbors.size() == 1)) {
                Cord n_cord = neighbors[0];
                char n_c = text[n_cord.x][n_cord.y];
                if (not(neighbors[-1] == *previous_cord)) {
                    if (n_c == '-') {
                        if ((n_cord.y == current_cord->y + 1) and direction) {
                            previous_cord = current_cord;
                            current_cord = &neighbors[0];
                        } else {
                            broken = true;
                        }
                    } else if (n_c == '|') {
                        if ((n_cord.x == current_cord->x + 1) and direction) {
                            previous_cord = current_cord;
                            current_cord = &neighbors[0];
                        } else {
                            broken = true;
                        }

                    } else {
                        broken = true;
                    }

                } else if (neighbors.size() == 0) {
                    broken = true;
                } else {
                    throw std::runtime_error("Dot starter has two exits");
                }

            } else if (c == '-') {
                seg.cords.push_back(*current_cord);
                std::vector<Cord> neighbors = this->_validNieghbors(*current_cord);

                for (int i = neighbors.size(); i >= 1; i--) {

                    Cord n_cord = neighbors[i - 1];
                    char n_c = text[n_cord.x][n_cord.y];

                    if ((n_cord.x != current_cord->x) or (n_cord == *previous_cord)) {
                        neighbors.pop_back();
                    }
                }
                if(neighbors.size() == 1){
                    
                }
                else if(neighbors.size() == 0){
                    seg.outlets.push_back(*current_cord);
                }
                else{
                    throw std::runtime_error("bro how?");
                }


            } else {
                std::cout << "not implemented yet" << std::endl;
                broken = true;
            }
        }
        return seg;
    }
}

Segment Segmenter::_followPath(Cord *cord) { // follows a path from a start point to its ends

    // segment search starts from the same place but haas different directions
    Segment left = this->_followDirection(cord, false);
    Segment right = this->_followDirection(cord, true);

    // combine the left and right segments 
    Segment combined;
    combined.type = "UNORDERED";

    for (int i = 0; i < left.cords.size(); i++) {
        combined.cords.push_back(left.cords[i]);
    }
    for (int i = 0; i < right.cords.size(); i++) {
        combined.cords.push_back(right.cords[i]);
    }
    for (int i = 0; i < left.outlets.size(); i++) {
        combined.outlets.push_back(left.outlets[i]);
    }
    for (int i = 0; i < right.outlets.size(); i++) {
        combined.outlets.push_back(right.outlets[i]);
    }

    //TODO: order the segment into a left to right line
    // Code:

    return combined;
}

std::vector<Segment> Segmenter::getSegments() { // get the ordered segments of the text

    std::vector<Segment> segs_list;

    while (true) {

        Cord *c = this->_findChar(); // find a potential segment

        if (c != nullptr) {

            Segment s = this->_followPath(c); // get the segment from that path
            segs_list.push_back(s);

        } else {

            return segs_list;
        }
    }
}

} // namespace segmenter