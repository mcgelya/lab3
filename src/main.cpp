#include <climits>
#include <iostream>
#include <sstream>

#include "deque.h"

int parseInteger(std::string& s, bool& err, int l = INT_MIN, int r = INT_MAX) {
    long long x = 0;
    bool minus = false;
    err = 0;
    for (int i = 0; i < s.size(); ++i) {
        if (i == 0 && s[i] == '-') {
            minus = true;
            continue;
        }
        if (!std::isdigit(s[i])) {
            std::cout << "\nNot a number!\n";
            err = 1;
            return -1;
        }
        x = x * 10 + (s[i] - '0');
        if (x < l || x > r) {
            std::cout << "\nNumber is out of range!\n";
            err = 1;
            return -1;
        }
    }
    if (x < l || x > r) {
        std::cout << "\nNumber is out of range!\n";
        err = 1;
        return -1;
    }
    if (minus) {
        x = -x;
    }
    return x;
}

int getInteger(bool& err, int l = INT_MIN, int r = INT_MAX) {
    std::string s;
    std::getline(std::cin, s);
    return parseInteger(s, err, l, r);
}

int main() {
    int b, c;
    {
        std::cout << "Enter bufferSize of deque: ";
        bool err = 0;
        b = getInteger(err, 1, 100);
        if (err) {
            std::cout << "Setting bufferSize to 5.\n";
        }
    }
    {
        std::cout << "Enter chunkSize of deque: ";
        bool err = 0;
        c = getInteger(err, 1, 100);
        if (err) {
            std::cout << "Setting chunkSize to 5.\n";
        }
    }

    Deque<int> d(0, b, c);
    while (true) {
        std::cout << "\n";
        d.Visualize();
        std::cout << "\nChoose operation:\n"
                     "1. Push numbers to the back\n"
                     "2. Pop number from the back\n"
                     "3. Push numbers to the front\n"
                     "4. Pop number from the front\n"
                     "5. Recreate deque with different bufferSize and chunkSize (it will become empty)\n"
                     "6. Get element by index\n"
                     "7. Quit"
                  << std::endl;
        std::string c;
        std::getline(std::cin, c);
        if (c == "1" || c == "3") {
            std::cout << "Input numbers: ";
            std::string s;
            std::getline(std::cin, s);
            std::istringstream ss(s);
            std::string cur;
            while (ss >> cur) {
                bool err = 0;
                int x = parseInteger(cur, err);
                if (err) {
                    break;
                }
                if (c == "1") {
                    d.PushBack(x);
                } else {
                    d.PushFront(x);
                }
            }
        } else if (c == "2" || c == "4") {
            if (d.GetSize() == 0) {
                std::cout << "\nDeque is empty! Try again\n" << std::endl;
                continue;
            }
            if (c == "2") {
                d.PopBack();
            } else {
                d.PopFront();
            }
        } else if (c == "5") {
            std::cout << "Enter bufferSize of deque: ";
            bool err = 0;
            int b = getInteger(err, 1, 100);
            if (err) {
                continue;
            }
            std::cout << "Enter chunkSize of deque: ";
            int c = getInteger(err, 1, 100);
            if (err) {
                continue;
            }
            d = Deque<int>(0, b, c);
        } else if (c == "6") {
            std::cout << "Enter index: ";
            bool err = 0;
            int ind = getInteger(err, 1, d.GetSize());
            --ind;
            if (err) {
                continue;
            }
            std::cout << "Here it is: " << d[ind] << "\n";
        } else if (c == "7") {
            break;
        } else {
            std::cout << "Incorrect option!" << std::endl;
        }
    }
}
