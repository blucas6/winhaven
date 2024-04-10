#include <iostream>
#define CHAR_LIMIT 256
using namespace std;

int main() {
    for (int i=0; i<CHAR_LIMIT; i++) {
        if (i > 0 && (i < 7 || i > 13)) cout << "(" << char(i) << " - " << i << ")\n";
    }
}