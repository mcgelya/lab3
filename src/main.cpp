#include <iostream>

#include "deque.h"

int main() {
    Deque<int, 5, 4> d;
    while (true) {
        std::cout << "\nChoose operation:\n"
                     "1. Push number to the back\n"
                     "2. Pop number from the back\n"
                     "3. Push number to the front\n"
                     "4. Pop number from the front\n"
                     "5. Quit"
                  << std::endl;
        std::string c;
        std::getline(std::cin, c);
        if (c == "1" || c == "3") {
            std::string s;
            std::getline(std::cin, s);
            long long x = 0;
            bool minus = false;
            bool ok = true;
            for (int i = 0; i < s.size(); ++i) {
                if (i == 0 && s[i] == '-') {
                    minus = true;
                    continue;
                }
                if (!std::isdigit(s[i])) {
                    std::cout << "\nNot a number!" << std::endl;
                    break;
                }
                x = x * 10 + (s[i] - '0');
                if (x < INT_MIN || x > INT_MAX) {
                    std::cout << "\nNumber is too large!" << std::endl;
                    break;
                }
            }
            if (!ok) {
                continue;
            }
            if (c == "1") {
                d.PushBack(x);
            } else {
                d.PushFront(x);
            }
        } else if (c == "2" || c == "4") {
            if (d.GetSize() == 0) {
                std::cout << "\nDeque is empty!" << std::endl;
                continue;
            }
            if (c == "2") {
                d.PopBack();
            } else {
                d.PopFront();
            }
        } else if (c == "5") {
            break;
        } else {
            std::cout << "Incorrect option!" << std::endl;
        }
        d.Visualize();
    }
}
