#include <sstream>
using namespace std;

template <typename T>
string convertirAString(const T& valor){
    ostringstream oss;
    oss<<valor;
    return oss.str();
}
