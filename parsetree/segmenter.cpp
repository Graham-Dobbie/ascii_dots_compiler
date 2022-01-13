#include "segmenter.hpp"

namespace segmenter {

Cord::Cord() { // I hope the cord class is understandable if not yikes
    this->x = 0;
    this->y = 0;
};

Cord::Cord(int x, int y) { // I hope the cord class is understandable if not yikes
    this->x = x;
    this->y = y;
};

bool Cord::operator==(const Cord &v) { // I hope the cord class is understandable if not yikes
    if ((this->x == v.x) and (this->y == v.y)) {
        return true;
    } else {
        return false;
    }
}

// I hope the cord class is understandable if not yikes
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

    this->_prep_text(raw_text);
    this->printText();
    std::cout << std::endl;

    std::cout << "Warps:" << std::endl;
    for (int i = 0; i < this->_warps.size(); i++) {
        std::cout << this->_warps[i] << ", ";
    }

    std::cout << std::endl;

    std::cout << "Libs:" << std::endl;
    for (int i = 0; i < this->_libs_used.size(); i++) {
        std::cout << this->_libs_used[i] << ", ";
    }

    std::cout << std::endl;
    std::cout << "Lib Warp:" << std::endl;
    std::cout << this->_lib_warp;
    std::cout << std::endl;
}

void Segmenter::_prep_text(std::vector<std::string> raw_text) {

    for (int i = 0; i < raw_text.size(); i++) { // convert raw text
        std::vector<char> app_vec;
        std::vector<int> pass_app_vec;
        for (int j = 0; j < raw_text[i].size(); j++) {
            app_vec.push_back(raw_text[i][j]);
        }
        this->text.push_back(app_vec);
    }

    this->_deleteComments();
    this->_fuction_data_scan(); // scan for libaries, warps and libary warps
    this->cropText();

    std::vector<char> non_bicons;
    char symbols[] = {'-', '|', '.', '>', '<', '^', 'v', '(', ')', '*', '~', '{', '}', '[', ']', '"', '\''};

    for (int i = 0; i < sizeof(symbols); i++) { // Adding symbols as symbols
        non_bicons.push_back(symbols[i]);
    }

    for (int i = 0; i < this->_warps.size(); i++) { // Adding warps as symbols
        non_bicons.push_back(this->_warps[i]);
    }

    for (int i = 0; i < raw_text.size(); i++) { // For the eraser function tells how many passes a char needs untill its erased
        std::vector<int> pass_app_vec;
        for (int j = 0; j < raw_text[i].size(); j++) {

            if (std::find(non_bicons.begin(), non_bicons.end(), this->text[i][j]) != non_bicons.end()) {
                pass_app_vec.push_back(1);
            } else if (this->text[i][j] == ' ') {
                pass_app_vec.push_back(0);
            } else {
                pass_app_vec.push_back(2);
            }
        }
        this->_erase_passes.push_back(pass_app_vec);
    }
}

void Segmenter::_fuction_data_scan() {
    Cord *c = new Cord;
    for (int i = 0; i < this->text.size(); i++) { // The lib/warp declarations have to be on the far left
        if (text[i].size() != 0) {
            if (text[i][0] == '%') {

                char invalid_chars[] = {' ', '-', '|', '/', '\\', '.', '>', '<', '^', 'V', '(', ')', '*', '~', '+', '{', '}', '[',  ']', '_',
                                        'a', '!', '#', '$', '1',  '2', '3', '4', '5', '6', '7', '8', '9', '0', '@', '%', '"', '\'', '?', '$'};

                // parses the Lib warp
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
                    // parses the libs useds
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
                    // parses the warps used
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
                if ((text[i][j] == '`') and (text[i][j + 1] == '`')) {
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

Cord *Segmenter::_findDot() { // search text for a period
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

Cord *Segmenter::_findFunc() { // search text for a function
    Cord *c = new Cord;
    for (int i = 0; i < this->text.size(); i++) {
        for (int j = 0; j < this->text[i].size(); j++) {
            if (std::find(this->_warps.begin(), this->_warps.end(), this->text[i][j]) != this->_warps.end()) {
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
        if ((src.y >= 0) and (src.y < this->text[src.x].size())) { // Checks to see if the src is inbounds
            Cord new_dir;

            char c = this->text[src.x][src.y];

            if (c == '/') {
                new_dir = Cord(-dir.y, -dir.x); // pls dont ask me explain this part the proof is trivial
            } else if (c == '\\') {
                new_dir = Cord(dir.y, dir.x); // same here
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
        return std::pair<Cord, Cord>(new_src, new_dir); // ik this is repetivie code fight me
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
    for (int i = 0; i < 3; i++) { // go horizontaly by three and make the seg/erase it

        Cord next_c = c + Cord(0, i); // Make seg

        seg.cords.push_back(next_c);
        seg.text_data.push_back(this->text[next_c.x][next_c.y]);

        this->text[next_c.x][next_c.y] = ' ';
    }

    // The four corners/outlets of the op seg
    seg.outlets.push_back(std::pair<Cord, Cord>(c, c + Cord(0, -1)));
    seg.outlets.push_back(std::pair<Cord, Cord>(c + Cord(0, 2), c + Cord(0, 3)));
    seg.outlets.push_back(std::pair<Cord, Cord>(c + Cord(0, 1), c + Cord(1, 1)));
    seg.outlets.push_back(std::pair<Cord, Cord>(c + Cord(0, 1), c + Cord(-1, 1)));

    this->_segs.push_back(seg);
    this->printText();

    // On each outlet folow the next segs
    for (int i = 0; i < seg.outlets.size(); i++) {

        Cord src = seg.outlets[i].first;
        Cord start = seg.outlets[i].second;
        Cord next = start + (start - src);

        this->_followPath(start, next, false);
    }
}

void Segmenter::_getMerger(Cord c) {

    Segment seg;

    char cord_c = this->text[c.x][c.y];
    seg.cords.push_back(c);
    seg.text_data.push_back(cord_c);

    seg.type = "MERGER";

    // Its four outlets if not the parenthesis fuck the parenthesis
    seg.outlets.push_back(std::pair<Cord, Cord>(c, c + Cord(0, -1)));
    seg.outlets.push_back(std::pair<Cord, Cord>(c, c + Cord(0, 1)));

    if ((cord_c != ')') and (cord_c != '(')) { // hot people have four outlets
        seg.outlets.push_back(std::pair<Cord, Cord>(c, c + Cord(1, 0)));
        seg.outlets.push_back(std::pair<Cord, Cord>(c, c + Cord(-1, 0)));
    }

    this->_segs.push_back(seg);
    this->_eraseSeg(seg.cords);
    this->printText();

    // Follow the outlets
    for (int i = 0; i < seg.outlets.size(); i++) {

        Cord src = seg.outlets[i].first;
        Cord start = seg.outlets[i].second;
        Cord next = start + (start - src);

        this->_followPath(start, next, false);
    }
}

void Segmenter::_eraseSeg(std::vector<Cord> cords) { // This is  solid 2/10 function still needs work

    // non_bicons symbols are just symbols that aren't bicons that you want to delete after one pass

    for (int i = 0; i < cords.size(); i++) {

        Cord cord = cords[i];
        char c = this->text[cord.x][cord.y];
        int pass = this->_erase_passes[cord.x][cord.y];

        // If its forbin then do the kickin
        if (pass == 2) {
            this->_erase_passes[cord.x][cord.y]--;
        }else if(pass == 1){
            this->text[cord.x][cord.y] = ' ';
            this->_erase_passes[cord.x][cord.y]--;
        }
    }
}

Segment Segmenter::_getQuote(Segment seg, Cord src, Cord dir) {

    char c = this->text[src.x][src.y];

    bool in_quote = true;
    seg.cords.push_back(src);
    seg.text_data.push_back(c);
    src = src + dir;

    // Kust loop through the single line w one direction untill you hit the end of the quote
    while (in_quote) {

        c = this->text[src.x][src.y];
        if (c != '"') {
            seg.cords.push_back(src);
            seg.text_data.push_back(c);
            src = src + dir;
        } else {
            seg.cords.push_back(src);
            seg.text_data.push_back(c);
            src = src + dir;
            in_quote = false;
        }
    }
    return seg;
}

void Segmenter::_followPath(Cord start, Cord next, bool begining) { // follows a path from a start point to its ends

    Segment seg;
    seg.type = "PIPE";

    Cord src = start;      // The src cord is the current cursor of this follow path
    Cord dir = next - src; // The direction of the path

    Cord op_cord; // Used if the following segment is a merger/op

    bool in_path = true; // maintains the while loop
    bool in_op = false;
    bool in_merger = false;

    char merger_pipes[] = {'*', '>', '<', 'v', '^', '(', ')', '~'};

    // starts a loop that steps through a set of pipes and stops at a merger/op type char set
    while (in_path) {

        // AAYYYOOO shity code so it doesn't throw errors
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
        if (c == ' ') {
            in_path = false;
        }

        if (c == '.') { // Ckeck to see if its a period
            seg.cords.push_back(src);
            seg.text_data.push_back(c);

            Cord next_cord = this->_getDot(src);

            if ((!(next_cord == Cord(-1, -1)) and begining)) { // if its starting set the direction
                dir = next_cord - src;
                src = next_cord;
            } else { // end the path
                in_path = false;
            }
        }
        // Check if the char is a merger type aka direction control or a warp
        if ((std::find(std::begin(merger_pipes), std::end(merger_pipes), c) != std::end(merger_pipes)) or (std::find(this->_warps.begin(), this->_warps.end(), c) != this->_warps.end())) {
            seg.outlets.push_back(std::pair<Cord, Cord>(src - dir, src));
            in_merger = true;
            in_path = false;

            op_cord = src;
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

        if (c == '"') { // Check to see if the string has entered a quote
            seg = this->_getQuote(seg, src, dir);
            src = seg.cords[seg.cords.size() - 1];
        }

        if (in_path) { // The prgram must still in the path at this point

            std::pair<Cord, Cord> cords = this->_step(src, dir); // will step once through to pipes

            Cord next_cord = cords.first;

            if (!(next_cord == Cord(-1, -1))) { // if there is a posible next step run the loop again
                c = this->text[src.x][src.y];
                seg.cords.push_back(src);
                seg.text_data.push_back(c);
                dir = cords.second;
                src = next_cord;
            } else {
                in_path = false;
            }
        }
    }

    // Once it has made the seg it deletes it from the text so it isn't retraced
    this->_eraseSeg(seg.cords);
    this->printText();

    // Out of the while loop the path can lead to a op, merger, or nothing
    if (in_op) {
        seg.outlets.push_back(std::pair<Cord, Cord>(src, src + dir));
        this->_getOp(op_cord);
    } else if (in_merger) {
        seg.outlets.push_back(std::pair<Cord, Cord>(src, src + dir));
        this->_getMerger(op_cord);
    }
    if (seg.cords.size() > 0) {
        this->_segs.push_back(seg);
    }
}

std::vector<Segment> Segmenter::getSegments() { // get the ordered segments of the text

    this->_deleteComments();
    this->printText();
    std::cout << '\n';

    while (true) {

        Cord *c = this->_findDot(); // find a potential segment

        if (c != nullptr) {
            this->_followPath(*c, Cord(-1, -1), true); // get the segment from that path

        } else {
            Cord *func_cord = this->_findFunc();
            if (func_cord != nullptr) {
                this->_getMerger(*func_cord);
            } else {
                return this->_segs;
            }
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

    bool going = true;
    int row = 0;

    while (going) {
        bool del = false;

        for (int i = 0; i < this->text[row].size(); i++) {
            char c = this->text[row][i];
            if (c != ' ') {
                going = false;
                del = false;
                break;
            } else {
                del = true;
            }
        }

        if(del){
            row_crop.push_back(row);
        }
        row++;
    }

    going = true;
    int col = 0;

    while (going) {
        bool del = false;

        for (int i = 0; i < this->text.size(); i++) {
            if (col < this->text[i].size()) {
                going = true;
                char c = this->text[i][col];
                if (c != ' ') {
                    going = false;
                    del = false;
                    break;
                } else {
                    del =true;
                }
            }
        }
        if(del){
            colomn_crop.push_back(col);
        }
        col++;
    }

    std::vector<std::vector<char>> new_text;
    for (int i = 0; i < this->text.size(); i++) {
        if (std::find(row_crop.begin(), row_crop.end(), i) == row_crop.end()) {
            std::vector<char> row;
            for (int j = 0; j < this->text[i].size(); j++) {
                if (std::find(colomn_crop.begin(), colomn_crop.end(), j) == colomn_crop.end()) {
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