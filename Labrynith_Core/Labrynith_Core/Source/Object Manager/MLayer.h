#include "MFlake.h"
#include <vector>
using namespace std;

#include "../BST.h"

class MLayer
{
	BST<int> SearchIndex;
	vector<MFlake> m_vFlakes;
};