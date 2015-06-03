#include "globals.h"
#include <sstream>

#define LOG_LEVEL 1

using namespace std;

bool isWithin(int A, int B, int C)
{
    return abs(A-B) < C;
}

string indent(int level)
{
    ostringstream ss;
    for(int i = 0; i < level - 1; i++)
    {
        ss << "->";
    }
    return ss.str();
}

void log(int level, string message)
{
    if(LOG_LEVEL >= level) cout << indent(level) << message << endl;
}
