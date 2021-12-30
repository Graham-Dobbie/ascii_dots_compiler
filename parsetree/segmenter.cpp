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

Symbol::Symbol() {
    std::map<std::string, std::pair<std::regex, Cord>> _directions;
    char name;
}

Symbol::Symbol(char name, std::regex left, std::regex right, std::regex up, std::regex down) {
    this->name = name;

    std::pair<std::regex, Cord> l(left, Cord(0, -1));
    std::pair<std::regex, Cord> r(right, Cord(0, 1));
    std::pair<std::regex, Cord> u(up, Cord(-1, 0));
    std::pair<std::regex, Cord> d(down, Cord(1, 0));

    this->_directions.insert(std::pair<std::string, std::pair<std::regex, Cord>>("left", l));
    this->_directions.insert(std::pair<std::string, std::pair<std::regex, Cord>>("right", r));
    this->_directions.insert(std::pair<std::string, std::pair<std::regex, Cord>>("up", u));
    this->_directions.insert(std::pair<std::string, std::pair<std::regex, Cord>>("down", d));
}

Symbol::Symbol(char name, std::map<std::string, std::pair<std::regex, Cord>> directions) {
    this->name = name;
    this->_directions = directions;
}

bool Symbol::operator==(const char &c) {
    if (c == this->name) {
        return true;
    } else {
        return false;
    }
}

Cord Symbol::point(char left, char right, char up, char down) {

    std::string left_s(1, left);
    std::string right_s(1, right);
    std::string up_s(1, up);
    std::string down_s(1, down);

    std::smatch matches;
    std::regex_search(left_s, matches, this->_directions["left"].first);
    if (!matches.empty()) {
        return this->_directions["left"].second;
    }

    std::regex_search(right_s, matches, this->_directions["right"].first);
    if (!matches.empty()) {
        return this->_directions["right"].second;
    }

    std::regex_search(up_s, matches, this->_directions["up"].first);
    if (!matches.empty()) {
        return this->_directions["up"].second;
    }

    std::regex_search(down_s, matches, this->_directions["down"].first);
    if (!matches.empty()) {
        return this->_directions["down"].second;
    }

    return Cord(0, 0);
}

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

    std::regex hoz_tmp("^[\\-]+");
    std::regex vert_tmp("^[\\|]+");
    this->_symbols.push_back(Symbol('.', hoz_tmp, hoz_tmp, vert_tmp, vert_tmp));

    std::regex hoz("^[^\\s\\|\\<\\>\\^V\\[\\]\\{\\}\\*\\~\\(\\)]+");
    std::regex nothing("^(?=x)(?!x)");
    this->_symbols.push_back(Symbol('-', hoz, hoz, nothing, nothing));

    std::regex vert("^[^\\s\\-\\<\\>\\^V\\[\\]\\{\\}\\*\\~\\(\\)]+");
    this->_symbols.push_back(Symbol('|', nothing, nothing, vert, vert));

    this->_symbols.push_back(Symbol('/', hoz, hoz, vert, vert));
    this->_symbols.push_back(Symbol('\\', hoz, hoz, vert, vert));

    this->_symbols.push_back(Symbol('>', nothing, nothing, nothing, nothing));
    this->_symbols.push_back(Symbol('<', nothing, nothing, nothing, nothing));
    this->_symbols.push_back(Symbol('V', nothing, nothing, nothing, nothing));
    this->_symbols.push_back(Symbol('^', nothing, nothing, nothing, nothing));
    this->_symbols.push_back(Symbol('(', nothing, nothing, nothing, nothing));
    this->_symbols.push_back(Symbol(')', nothing, nothing, nothing, nothing));
    this->_symbols.push_back(Symbol('*', nothing, nothing, nothing, nothing));
    this->_symbols.push_back(Symbol('~', nothing, nothing, nothing, nothing));

    this->_any_char = Symbol('?', hoz, hoz, vert, vert);
}

Cord *Segmenter::_findChar() { // search text for a non space (' ') char
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
// Cord Segmenter::_step(Cord last, Cord current) {}

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

    Segment seg;

    seg.type = "PARTIAL";

    Cord prv_cord = Cord(-1, -1);
    Cord src_cord = *start;

    while (true) {
        char c = this->text[src_cord.x][src_cord.y];
        char c_l = this->text[src_cord.x][src_cord.y - 1];

        if ((c_l == '[') or (c_l == '{')) { // Check to see if it moved into an operation
            return seg;

        } else {
            std::vector<Cord> n = this->_validNieghbors(src_cord);
            if (c == '.') { // need to check for multiple path ways

                std::vector<Cord> valid_n;

                for (int i = 0; i < n.size(); i++) {
                    Cord n_cord = n[i];
                    char n_c = this->text[n_cord.x][n_cord.y];
                    if ((n_c == '|') and ((n_cord.x == src_cord.x - 1) or (n_cord.x == src_cord.x + 1))) {
                        valid_n.push_back(n_cord);
                    } else if ((n_c == '-') and ((n_cord.y == src_cord.y - 1) or (n_cord.y == src_cord.y + 1))) {
                        valid_n.push_back(n_cord);
                    }
                }

                if (valid_n.size() > 1) {
                    throw std::runtime_error("Multiple pipes to one dot");
                } else if ((valid_n.size() < 2) and (direction)) {
                    seg.cords.push_back(src_cord);
                    seg.outlets.push_back(src_cord);
                    seg.text_data.push_back(c);
                    prv_cord = src_cord;
                    src_cord = valid_n[0];
                } else {
                    return seg;
                }

            } else {
                seg.cords.push_back(src_cord);
                seg.text_data.push_back(c);

                char c_l = ' ';
                char c_r = ' ';
                char c_u = ' ';
                char c_d = ' ';

                for (int i = 0; i < n.size(); i++) {

                    Cord n_cord = n[i];
                    char n_c = this->text[n_cord.x][n_cord.y];
                    Cord dir = src_cord - n_cord;

                    bool valid = true;

                    if (n_cord == prv_cord) {
                        valid = false;
                    }

                    else if (c == '/') {
                        if ((src_cord - prv_cord == Cord(0, 1)) and !(dir == Cord(-1, 0))) {
                            valid = false;
                        } else if ((src_cord - prv_cord == Cord(0, -1)) and !(dir == Cord(1, 0))) {
                            valid = false;
                        }
                    } else if (c == '\\') {
                        if ((src_cord - prv_cord == Cord(0, 1)) and !(dir == Cord(1, 0))) {
                            valid = false;
                        } else if ((src_cord - prv_cord == Cord(0, -1)) and !(dir == Cord(-1, 0))) {
                            valid = false;
                        }
                    } else {
                        if ((src_cord - prv_cord == Cord(0, 1)) and !(dir == Cord(0, -1))) {
                            valid = false;
                        } else if ((src_cord - prv_cord == Cord(0, -1)) and !(dir == Cord(0, 1))) {
                            valid = false;
                        } else if ((src_cord - prv_cord == Cord(1, 0)) and !(dir == Cord(-1, 0))) {
                            valid = false;
                        } else if ((src_cord - prv_cord == Cord(-1, 0)) and !(dir == Cord(1, 0))) {
                            valid = false;
                        }
                    }

                    if (valid) {
                        if (dir == Cord(0, 1)) {
                            c_l = c;
                        } else if (dir == Cord(0, -1)) {
                            c_r = c;
                        } else if (dir == Cord(-1, 0)) {
                            c_u = c;
                        } else if (dir == Cord(1, 0)) {
                            c_d = c;
                        }
                    }
                }

                Symbol s;
                for (int i = 0; i < this->_symbols.size(); i++) {
                    if (this->_symbols[i] == c) {
                        s = this->_symbols[i];
                    }
                }

                Cord point = s.point(c_l, c_r, c_u, c_d);
                if (point == Cord(0,0)){
                    seg.outlets.push_back(src_cord);
                    return seg;
                }else{
                    prv_cord = src_cord;
                    src_cord = src_cord + point;
                }
            }
        }
    }
}

Segment Segmenter::_followPath(Cord *cord) { // follows a path from a start point to its ends

    Segment seg;

    // Checking to see if the path is a operation
    bool in_op = false;
    Cord op_cord;

    char c = this->text[cord->x][cord->y];
    char c_l = this->text[cord->x][cord->y - 1];

    if ((c == '[') or (c == ']') or (c == '{') or (c == '}')) {
        in_op = true;
        if ((c == '[') or (c == '{'))
            op_cord = *cord;
        else {
            op_cord = *cord + Cord(0, -2);
        }
    } else if ((c_l == '[') or (c_l == '{')) {
        in_op = true;
        op_cord = *cord + Cord(0, -1);
    }

    if (in_op) { // If the path is in an operation create an easy segment
        seg.type = "OPERATION";
        for (int i = 0; i < 3; i++) {
            Cord c = *cord + Cord(0, i);
            seg.cords.push_back(c);
            seg.text_data.push_back(this->text[c.x][c.y]);
        }

        seg.outlets.push_back(*cord + Cord(0, -1));
        seg.outlets.push_back(*cord + Cord(0, 3));
        seg.outlets.push_back(*cord + Cord(1, 1));
        seg.outlets.push_back(*cord + Cord(-1, 1));

        return seg;
    }

    else {

        // segment search starts from the same place but haas different directions
        Segment left = this->_followDirection(cord, false);
        Segment right = this->_followDirection(cord, true);

        // combine the left and right segments

        seg.type = "UNORDERED";

        for (int i = 0; i < left.cords.size(); i++) {
            seg.cords.push_back(left.cords[i]);
            seg.text_data.push_back(left.text_data[i]);
        }
        for (int i = 0; i < right.cords.size(); i++) {
            seg.cords.push_back(right.cords[i]);
            seg.text_data.push_back(right.text_data[i]);
        }
        for (int i = 0; i < left.outlets.size(); i++) {
            seg.outlets.push_back(left.outlets[i]);
        }
        for (int i = 0; i < right.outlets.size(); i++) {
            seg.outlets.push_back(right.outlets[i]);
        }

        // TODO: order the segment into a left to right line
        //  Code: 

        return seg;
    }
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