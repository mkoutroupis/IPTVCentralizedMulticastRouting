//#ifndef UNIVERSENODE_H_
//#define UNIVERSENODE_H_

using namespace std;

class UniverseNode {
	int _id;
public:
	UniverseNode(int id){
		_id = id;
	}
	~UniverseNode(){}

	int GetId(){
		return _id;
	}
};

//#endif