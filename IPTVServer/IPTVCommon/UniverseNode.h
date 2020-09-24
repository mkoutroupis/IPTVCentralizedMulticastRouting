#ifndef UNIVERSENODE_H_
#define UNIVERSENODE_H_

namespace IPTVCommon {
	class UniverseNode {
		unsigned int _id;
	public:
		UniverseNode(unsigned int id){
			_id = id;
		}
		~UniverseNode(){}

		unsigned int GetId() const{
			return _id;
		}

		bool operator < (const UniverseNode &b) const {
			return GetId() < b.GetId();
		}

		bool operator <= (const UniverseNode &b) const {
			return GetId() <= b.GetId();
		}

		bool operator > (const UniverseNode &b) const {
			return GetId() > b.GetId();
		}

		bool operator >= (const UniverseNode &b) const {
			return GetId() >= b.GetId();
		}

		bool operator == (const UniverseNode &b) const {
			return GetId() == b.GetId();
		}

		bool operator != (const UniverseNode &b) const {
			return GetId() != b.GetId();
		}
	};
}

#endif