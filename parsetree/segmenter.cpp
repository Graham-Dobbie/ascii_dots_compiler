#include "segmenter.hpp"

namespace segmenter {

Cord::Cord() {
    this->x = 0;
    this->y = 0;
};

Cord::Cord(int x, int y) {
    this->x = 0;
    this->y = 0;
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

void Cord::print(){
    std::cout << "(" << this->x << "," << this->y << ")" << std::endl;
}

Segment::Segment(std::string type, std::vector<Cord> inputs, std::vector<Cord> outputs, std::vector<char> text_data) {

    this->type = type;
    this->inputs = inputs;
    this->outputs = outputs;
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

    for(int i = 0; i < text.size(); i++){
        for(int j = 0; j < text[i].size(); j++){
            std::cout << text[i][j];
        }
        std::cout << std::endl;
    }
}

Cord *Segmenter::_findChar() {
    for (int i = 0; i < this->text.size(); i++) {
        for (int j = 0; j < this->text[i].size(); j++) {
            std::cout << i << "," << j << std::endl;
            std::cout << text[i][j] << std::endl;
            if (not(text[i][j] == ' ')) {
                std::cout << i << "," << j << std::endl;
                Cord c = Cord(i, j);
                return &c;
            }
        }
    }
    std::cout << "here" << std::endl;
    return NULL;
}

Cord Segmenter::_step(Cord last, Cord current) {

    char c = this->text[current.x][current.y];

    char direction_pipes[] = {'/', '\\'};
    char merger_pipes[] = {'*', '>', '<', 'v', '^', '(', ')'};

    if (std::find(direction_pipes, direction_pipes, c) != 0) {

        if (c == '/') {
            Cord direction = current - last;
            if ((direction.x == 1) and (direction.y == 0)) {
                return current + Cord(0, -1);
            } else if ((direction.x == -1) and (direction.y == 0)) {
                return current + Cord(0, 1);
            } else if ((direction.x == 0) and (direction.y == 1)) {
                return current + Cord(1, 0);
            } else if ((direction.x == 0) and (direction.y == 1)) {
                return current + Cord(-1, 0);
            } else {
                std::cout << "Bad Cord" << std::endl;
                return Cord(-1, -1);
            }
        }

        else if (c == '\\') {
            Cord direction = current - last;
            if ((direction.x == 1) and (direction.y == 0)) {
                return current + Cord(0, 1);
            } else if ((direction.x == -1) and (direction.y == 0)) {
                return current + Cord(0, -1);
            } else if ((direction.x == 0) and (direction.y == 1)) {
                return current + Cord(-1, 0);
            } else if ((direction.x == 0) and (direction.y == 1)) {
                return current + Cord(1, 0);
            } else {
                std::cout << "Bad Cord" << std::endl;
                return Cord(-1, -1);
            }
        }
    }

    std::vector<Cord> untested_neighbors = current.getNeighbors();
    std::vector<Cord> neighbors;

    for (int i = 0; i < untested_neighbors.size(); i++) {
        Cord neighbor = untested_neighbors[i];
        if ((neighbor.x >= 0) or (neighbor.x < this->text.size())) {
            if ((neighbor.y >= 0) or (neighbor.y < this->text[neighbor.x].size())) {
                neighbors.push_back(neighbor);
            }
        }
    }
}

Segment Segmenter::_followPath(Cord *cord) {
    std::vector<Cord> cord_list;
    cord_list.push_back(*cord);
}

std::vector<Segment> Segmenter::getSegments() {
    std::vector<Segment> token_list;

    Cord *c = this->_findChar();
    if (c != nullptr) {
        Segment s = this->_followPath(c);

    } else {
        return token_list;
    }
}

} // namespace segmenter