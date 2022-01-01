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

Segment::Segment(std::string type, std::vector<Cord> inputs, std::vector<std::pair<Cord, Cord>> outputs, std::vector<char> text_data) {

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

    for (int i = 0; i < raw_text.size(); i++) { // convert raw text
        std::vector<char> app_vec;
        for (int j = 0; j < raw_text[i].size(); j++) {
            app_vec.push_back(raw_text[i][j]);
        }
        this->text.push_back(app_vec);
    }

    for (int i = 0; i < text.size(); i++) { // print raw text
        for (int j = 0; j < text[i].size(); j++) {
            std::cout << text[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    this->_fuction_data_scan(); // scan for libaries, warps and libary warps
    std::cout << "Warps:" << std::endl;
    for (int i = 0; i < this->_warps.size(); i++) {
        std::cout << this->_warps[i] << '\n';
    }
    std::cout << std::endl;

    std::cout << "Libs:" << std::endl;
    for (int i = 0; i < this->_libs_used.size(); i++) {
        std::cout << this->_libs_used[i] << '\n';
    }
    std::cout << std::endl;
    std::cout << "Lib Warp:" << std::endl;
    std::cout << this->_lib_warp;
    std::cout << std::endl;

    // Make symbols
    std::regex hoz_tmp("^[\\-]+");
    std::regex vert_tmp("^[\\|]+");
    this->_symbols.push_back(Symbol('.', hoz_tmp, hoz_tmp, vert_tmp, vert_tmp));

    std::string hoz_str = "^[^\\s\\|\\<\\>\\^V\\[\\]\\{\\}\\*\\~\\(\\)";
    std::string vert_str = "^[^\\s\\-\\<\\>\\^V\\[\\]\\{\\}\\*\\~\\(\\)";

    for (int i = 0; i < this->_warps.size(); i++) { // Adds warps to the no fly symbol list
        hoz_str = hoz_str + this->_warps[i];
        vert_str = vert_str + this->_warps[i];
    }
    hoz_str = hoz_str + "]+";
    vert_str = vert_str + "]+";

    // start making symbols
    std::regex hoz(hoz_str);
    std::regex nothing("^(?=x)(?!x)");
    this->_symbols.push_back(Symbol('-', hoz, hoz, nothing, nothing));

    std::regex vert(vert_str);
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

    for (int i = 0; i < this->_warps.size(); i++) { // Adding warps as symbols
        this->_symbols.push_back(Symbol(this->_warps[i], nothing, nothing, nothing, nothing));
    }

    this->_any_char = Symbol('?', hoz, hoz, vert, vert); // if a char in not in the list it has this symbol
}

void Segmenter::_fuction_data_scan() {
    Cord *c = new Cord;
    for (int i = 0; i < this->text.size(); i++) {
        if (text[i].size() != 0) {
            if (text[i][0] == '%') {

                char invalid_chars[] = {' ', '-', '|', '/', '\\', '.', '>', '<', '^', 'V', '(', ')', '*', '~', '+', '{', '}', '[',  ']', '_',
                                        'a', '!', '#', '$', '1',  '2', '3', '4', '5', '6', '7', '8', '9', '0', '@', '%', '"', '\'', '?', '$'};

                if (text[i][1] == '^') {

                    if (std::find(invalid_chars, invalid_chars, text[i][2]) != invalid_chars) {
                        throw std::runtime_error("Libary warp cannot be an invalid charater");
                    } else if (std::find(this->_warps.begin(), this->_warps.end(), text[i][2]) != this->_warps.end()) {
                        throw std::runtime_error("Libary warp cannot be a used warp");
                    } else if ((std::find(invalid_chars, invalid_chars, text[i][2]) == invalid_chars) and (std::find(this->_warps.begin(), this->_warps.end(), text[i][2]) == this->_warps.end())) {
                        this->_lib_warp = text[i][2];
                        this->_warps.push_back(text[i][2]);
                        this->text[i][0] = ' ';
                        this->text[i][1] = ' ';
                        this->text[i][2] = ' ';
                    } else {
                        throw std::runtime_error("Idk why you are here");
                    }
                } else if (text[i][1] == '!') {

                    std::string lib_str = "";

                    int j = 2;
                    char c = text[i][j];
                    this->text[i][0] = ' ';
                    this->text[i][1] = ' ';

                    while (c != ' ') {
                        lib_str = lib_str + c;
                        this->text[i][j] = ' ';
                        j++;
                        c = text[i][j];
                    }

                    this->_libs_used.push_back(lib_str);

                    j++;

                    if (std::find(invalid_chars, invalid_chars, text[i][j]) != invalid_chars) {
                        throw std::runtime_error("Libary warp cannot be an invalid charater");
                    } else if (std::find(this->_warps.begin(), this->_warps.end(), text[i][j]) != this->_warps.end()) {
                        throw std::runtime_error("Libary warp cannot be a used warp");
                    } else if ((std::find(invalid_chars, invalid_chars, text[i][j]) == invalid_chars) and (std::find(this->_warps.begin(), this->_warps.end(), text[i][j]) == this->_warps.end())) {
                        this->_warps.push_back(text[i][j]);
                        this->text[i][j] = ' ';
                    } else {
                        throw std::runtime_error("Idk why you are here");
                    }

                } else if (text[i][1] == '$') {
                    this->text[i][0] = ' ';
                    this->text[i][1] = ' ';

                    int j = 2;
                    char c = text[i][j];
                    bool out = false;

                    while (!out) {
                        if (std::find(invalid_chars, invalid_chars, c) != invalid_chars) {
                            throw std::runtime_error("Warp cannot be an invalid charater");
                        } else if (std::find(this->_warps.begin(), this->_warps.end(), c) != this->_warps.end()) {
                            throw std::runtime_error("Warp cannot be a used warp");
                        } else if ((std::find(invalid_chars, invalid_chars, c) == invalid_chars) and (std::find(this->_warps.begin(), this->_warps.end(), c) == this->_warps.end())) {
                            this->_warps.push_back(c);
                        } else {
                            throw std::runtime_error("Idk why you are here");
                        }
                        this->text[i][j] = ' ';
                        j++;
                        c = text[i][j];
                        if ((j >= this->text[i].size()) or (c == ' ')) {
                            out = true;
                        }
                    }
                }
            }
        }
    }
}

void Segmenter::_deleteComments() {
    bool more_comments = true;
    while (more_comments) {

        Cord start = Cord(-1, -1);

        for (int i = 0; i < this->text.size(); i++) {
            for (int j = 0; j < this->text[i].size(); j++) {
                if ((text[i][j] == '\'') and (text[i][j + 1] == '\'')) {
                    start = Cord(i, j);
                    break;
                }
            }
        }

        if (not(start == Cord(-1, -1))) {
            this->text[start.x][start.y] = ' ';
            this->text[start.x][start.y + 1] = ' ';
            for (int j = start.y + 2; j < this->text[start.x].size(); j++) {
                if ((text[start.x][j] == '\'') and (text[start.x][j + 1] == '\'')) {
                    this->text[start.x][j] = ' ';
                    this->text[start.x][j + 1] = ' ';
                    break;
                } else {
                    this->text[start.x][j] = ' ';
                }
            }
        } else {
            more_comments = false;
        }
    }
}

Cord *Segmenter::_findDot() { // search text for a non space (' ') or non directional char
    Cord *c = new Cord;
    for (int i = 0; i < this->text.size(); i++) {
        for (int j = 0; j < this->text[i].size(); j++) {
            if (text[i][j] == '.') {
                c->x = i;
                c->y = j;
                return c;
            }
        }
    }
    return nullptr;
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

std::pair<Cord, Cord> Segmenter::_step(Cord src, Cord dir) {

    if ((src.x >= 0) and (src.x < this->text.size())) {
        if ((src.y >= 0) and (src.y < this->text[src.x].size())) {
            Cord new_dir;

            char c = this->text[src.x][src.y];

            if (c == '/') {
                new_dir = Cord(-dir.y, -dir.x);
            } else if (c == '\\') {
                new_dir = Cord(dir.y, dir.x);
            } else if (c == ' ') {
                Cord new_src = Cord(-1, -1);
                Cord new_dir = Cord(0, 0);
                return std::pair<Cord, Cord>(new_src, new_dir);
            } else {
                new_dir = dir;
            }

            Cord new_src = src + new_dir;
            return std::pair<Cord, Cord>(new_src, new_dir);
        } else {
            Cord new_src = Cord(-1, -1);
            Cord new_dir = Cord(0, 0);
            return std::pair<Cord, Cord>(new_src, new_dir);
        }

    } else {
        Cord new_src = Cord(-1, -1);
        Cord new_dir = Cord(0, 0);
        return std::pair<Cord, Cord>(new_src, new_dir);
    }
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

Segment Segmenter::_followDirection(bool direction) {

    Segment seg;

    seg.type = "PARTIAL";

    Cord prv_cord = Cord(-1, -1);
    Cord dir_cord = Cord(0, 0);

    while (true) {
        char c = this->text[this->_cursor.x][this->_cursor.y];
        char c_l = this->text[this->_cursor.x][this->_cursor.y - 1];

        char merger_pipes[] = {'*', '>', '<', 'v', '^', '(', ')'};

        if ((c_l == '[') or (c_l == '{')) { // Check to see if it moved into an operation
            return seg;

        } else if ((std::find(merger_pipes, merger_pipes, c) != merger_pipes) or std::find(this->_warps.begin(), this->_warps.end(), c) != this->_warps.end()) {

        } else {
            std::vector<Cord> n = this->_validNieghbors(this->_cursor);
            if (c == '.') { // need to check for multiple path ways

                std::vector<Cord> valid_n;

                for (int i = 0; i < n.size(); i++) {
                    Cord n_cord = n[i];
                    char n_c = this->text[n_cord.x][n_cord.y];
                    if ((n_c == '|') and ((n_cord.x == this->_cursor.x - 1) or (n_cord.x == this->_cursor.x + 1))) {
                        valid_n.push_back(n_cord);
                    } else if ((n_c == '-') and ((n_cord.y == this->_cursor.y - 1) or (n_cord.y == this->_cursor.y + 1))) {
                        valid_n.push_back(n_cord);
                    }
                }

                if (valid_n.size() > 1) {
                    throw std::runtime_error("Multiple pipes to one dot");
                } else if ((valid_n.size() < 2) and (direction)) {
                    seg.cords.push_back(this->_cursor);
                    seg.text_data.push_back(c);
                    prv_cord = this->_cursor;
                    this->_cursor = valid_n[0];
                } else {
                    return seg;
                }

            } else {
                seg.cords.push_back(this->_cursor);
                seg.text_data.push_back(c);

                char c_l = ' ';
                char c_r = ' ';
                char c_u = ' ';
                char c_d = ' ';

                for (int i = 0; i < n.size(); i++) {

                    Cord n_cord = n[i];
                    char n_c = this->text[n_cord.x][n_cord.y];
                    Cord dir = this->_cursor - n_cord;

                    bool valid = true;

                    if (n_cord == prv_cord) {
                        valid = false;
                    }

                    else if (c == '/') {
                        if ((this->_cursor - prv_cord == Cord(0, 1)) and !(dir == Cord(-1, 0))) {
                            valid = false;
                        } else if ((this->_cursor - prv_cord == Cord(0, -1)) and !(dir == Cord(1, 0))) {
                            valid = false;
                        }
                    } else if (c == '\\') {
                        if ((this->_cursor - prv_cord == Cord(0, 1)) and !(dir == Cord(1, 0))) {
                            valid = false;
                        } else if ((this->_cursor - prv_cord == Cord(0, -1)) and !(dir == Cord(-1, 0))) {
                            valid = false;
                        }
                    } else {
                        if ((this->_cursor - prv_cord == Cord(0, 1)) and !(dir == Cord(0, -1))) {
                            valid = false;
                        } else if ((this->_cursor - prv_cord == Cord(0, -1)) and !(dir == Cord(0, 1))) {
                            valid = false;
                        } else if ((this->_cursor - prv_cord == Cord(1, 0)) and !(dir == Cord(-1, 0))) {
                            valid = false;
                        } else if ((this->_cursor - prv_cord == Cord(-1, 0)) and !(dir == Cord(1, 0))) {
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
                if (point == Cord(0, 0)) {
                    // seg.outlets.push_back(this->_cursor);
                    return seg;
                } else {
                    prv_cord = this->_cursor;
                    this->_cursor = this->_cursor + point;
                }
            }
        }
    }
}

Cord Segmenter::_getDot(Cord c) {
    std::vector<Cord> n = this->_validNieghbors(c);
    std::vector<Cord> valid_n;

    for (int i = 0; i < n.size(); i++) {
        Cord n_cord = n[i];
        char n_c = this->text[n_cord.x][n_cord.y];
        if ((n_c == '|') and ((n_cord.x == c.x - 1) or (n_cord.x == c.x + 1))) {
            valid_n.push_back(n_cord);
        } else if ((n_c == '-') and ((n_cord.y == c.y - 1) or (n_cord.y == c.y + 1))) {
            valid_n.push_back(n_cord);
        }
    }
    if (valid_n.size() > 1) {
        throw std::runtime_error("Multiple pipes to one dot");
    } else if (valid_n.size() < 2) {
        return valid_n[0];
    } else {
        return Cord(-1, -1);
    }
}

void Segmenter::_getOp(Cord c) {

    Segment seg;

    seg.type = "OPERATION";
    for (int i = 0; i < 3; i++) {
        Cord c = this->_cursor + Cord(0, i);
        seg.cords.push_back(c);
        seg.text_data.push_back(this->text[c.x][c.y]);
    }

    seg.outlets.push_back(std::pair<Cord, Cord>(this->_cursor, this->_cursor + Cord(0, -1)));
    seg.outlets.push_back(std::pair<Cord, Cord>(this->_cursor + Cord(0, 2), this->_cursor + Cord(0, 3)));
    seg.outlets.push_back(std::pair<Cord, Cord>(this->_cursor + Cord(0, 1), this->_cursor + Cord(1, 1)));
    seg.outlets.push_back(std::pair<Cord, Cord>(this->_cursor + Cord(0, 1), this->_cursor + Cord(-1, 1)));
}

void Segmenter::_followPath(Cord start, Cord next) { // follows a path from a start point to its ends

    Segment seg;

    Cord src = start;
    Cord dir = next - src;

    Cord op_cord;

    bool in_path = true;
    bool in_op = false;
    bool in_merger = false;

    char merger_pipes[] = {'*', '>', '<', 'v', '^', '(', ')'};

    while (in_path) {
        char c = ' ';
        char c_l = ' ';
        if ((src.x >= 0) and (src.x < this->text.size())) {
            if ((src.y >= 0) and (src.y < this->text[src.x].size())) {
                c = this->text[src.x][src.y];
                c_l = this->text[src.x][src.y - 1];
            } else {
                in_path = false;
            }
        } else {
            in_path = false;
        }

        // Ckeck to see if its
        if (c == '.') { // need to check for multiple path ways
            seg.cords.push_back(src);
            seg.text_data.push_back(c);

            Cord next_cord = this->_getDot(src);

            if (!(next_cord == Cord(-1, -1))) {
                dir = next_cord - src;
                src = next_cord;
            } else {
                in_path = false;
            }
        } else if ((std::find(merger_pipes, merger_pipes, c) != merger_pipes) or (std::find(this->_warps.begin(), this->_warps.end(), c) != this->_warps.end())) {
            seg.outlets.push_back(std::pair<Cord, Cord>(src - dir, src));
            in_merger = true;
            in_path = false;
        }

        // Checking to see if the path is a operation

        if ((c == '[') or (c == ']') or (c == '{') or (c == '}')) {
            in_op = true;
            in_path = false;
            if ((c == '[') or (c == '{'))
                op_cord = src;
            else {
                op_cord = src + Cord(0, -2);
            }
        } else if ((c_l == '[') or (c_l == '{')) {
            in_op = true;
            in_path = false;
            op_cord = src + Cord(0, -1);
        }

        if (in_path) {

            std::pair<Cord, Cord> cords = this->_step(src, dir);

            Cord next_cord = cords.first;

            if (!(next_cord == Cord(-1, -1))) {
                seg.cords.push_back(src);
                seg.text_data.push_back(c);
                dir = cords.second;
                src = next_cord;
            } else {
                in_path = false;
            }
        }
    }
    this->segments.push_back(seg);

    if (in_op) {
        this->_getOp(op_cord);
    }
}

std::vector<Segment> Segmenter::getSegments() { // get the ordered segments of the text

    std::vector<Segment> segs_list;

    this->_deleteComments();
    this->printText();
    std::cout << '\n';
    this->cropText();
    this->printText();
    std::cout << '\n';

    while (true) {

        Cord *c = this->_findDot(); // find a potential segment

        if (c != nullptr) {
            this->_followPath(*c, Cord(-1, -1)); // get the segment from that path

        } else {

            return segs_list;
        }
    }
}

void Segmenter::printText() {
    for (int i = 0; i < this->text.size(); i++) {
        for (int j = 0; j < this->text[i].size(); j++) {
            std::cout << this->text[i][j];
        }
        std::cout << "\n";
    }
}

void Segmenter::cropText() {
    std::vector<int> row_crop;
    std::vector<int> colomn_crop;

    for (int i = 0; i < this->text.size(); i++) {
        bool del = true;
        for (int j = 0; j < this->text[i].size(); j++) {
            if (this->text[i][j] != ' ') {
                del = false;
                break;
            }
        }
        if (!del) {
            row_crop.push_back(i);
        }
    }

    bool going = true;
    int col = 0;

    while (going) {

        bool del = true;
        going = false;

        for (int i = 0; i < this->text.size(); i++) {
            if (col < this->text[i].size()) {
                going = true;
                if (this->text[i][col] != ' ') {
                    del = false;
                }
            }
        }
        if (!del) {
            colomn_crop.push_back(col);
        }

        col++;
    }

    std::vector<std::vector<char>> new_text;
    for (int i = 0; i < this->text.size(); i++) {
        if (std::find(row_crop.begin(), row_crop.end(), i) != row_crop.end()) {
            std::vector<char> row;
            for (int j = 0; j < this->text[i].size(); j++) {
                if (std::find(colomn_crop.begin(), colomn_crop.end(), j) != colomn_crop.end()) {
                    row.push_back(this->text[i][j]);
                }
            }
            if (row.size() > 0) {
                new_text.push_back(row);
            }
        }
    }
    this->text = new_text;
}

} // namespace segmenter