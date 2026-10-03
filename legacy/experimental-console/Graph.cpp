#include "system.h"
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;


unsigned main()
{
	System sys("a.txt");
	sys.combinate();
	cout << "Ready";
}
