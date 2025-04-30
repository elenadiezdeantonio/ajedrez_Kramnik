
#include <iostream>
#include "Tablero.h"
#include "Peon.h"

using namespace std;
int main()
{
    Tablero chess;

    chess.meterPieza(new Peon(3,0,'b'));

    chess.printTablero();
    std::cout << "Hello World!\n";

    return 0;
}
