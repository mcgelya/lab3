#include <climits>
#include <iostream>

#include "deque.h"

int getInteger(int& ok, int l = INT_MIN, int r = INT_MAX) {
    std::string s;
    std::getline(std::cin, s);
    long long x = 0;
    bool minus = false;
    ok = 0;
    for (int i = 0; i < s.size(); ++i) {
        if (i == 0 && s[i] == '-') {
            minus = true;
            continue;
        }
        if (!std::isdigit(s[i])) {
            ok = 1;
            return -1;
        }
        x = x * 10 + (s[i] - '0');
        if (x < l || x > r) {
            ok = 2;
            return -1;
        }
    }
    return x;
}

int main() {
    int b, c;
    while (true) {
        std::cout << "Enter bufferSize of deque: ";
        int ok = 0;
        b = getInteger(ok, 1, 100);
        if (ok == 1) {
            std::cout << "\nNot a number!" << std::endl;
            continue;
        }
        if (ok == 2) {
            std::cout << "\nNumber is too large!" << std::endl;
            continue;
        }
        break;
    }
    while (true) {
        std::cout << "Enter chunkSize of deque: ";
        int ok = 0;
        c = getInteger(ok, 1, 100);
        if (ok == 1) {
            std::cout << "\nNot a number!" << std::endl;
            continue;
        }
        if (ok == 2) {
            std::cout << "\nNumber is too large!" << std::endl;
            continue;
        }
        break;
    }
    Deque<int> d(0, b, c);
    while (true) {
        std::cout << "\nChoose operation:\n"
                     "1. Push number to the back\n"
                     "2. Pop number from the back\n"
                     "3. Push number to the front\n"
                     "4. Pop number from the front\n"
                     "5. Recreate deque with different bufferSize and chunkSize (it will become empty)\n"
                     "5. Quit"
                  << std::endl;
        std::string c;
        std::getline(std::cin, c);
        if (c == "1" || c == "3") {
            std::cout << "Input a number: ";
            int ok = 0;
            int x = getInteger(ok);
            if (ok == 1) {
                std::cout << "\nNot a number! Try again\n" << std::endl;
                continue;
            }
            if (ok == 2) {
                std::cout << "\nNumber is too large! Try again\n" << std::endl;
                continue;
            }
            if (c == "1") {
                d.PushBack(x);
            } else {
                d.PushFront(x);
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
            int ok = 0;
            int b = getInteger(ok, 1, 100);
            if (ok == 1) {
                std::cout << "\nNot a number! Try again\n" << std::endl;
                continue;
            }
            if (ok == 2) {
                std::cout << "\nNumber is too large Try again\n" << std::endl;
                continue;
            }
            std::cout << "Enter chunkSize of deque: ";
            int c = getInteger(ok, 1, 100);
            if (ok == 1) {
                std::cout << "\nNot a number! Try again\n" << std::endl;
                continue;
            }
            if (ok == 2) {
                std::cout << "\nNumber is too large Try again\n" << std::endl;
                continue;
            }
            d = Deque<int>(0, b, c);
        } else if (c == "6") {
            break;
        } else {
            std::cout << "Incorrect option!" << std::endl;
        }
        d.Visualize();
    }
}
