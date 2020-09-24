#ifndef UNIVERSEHOLDER_H_
#define UNIVERSEHOLDER_H_

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include "UniverseNode.h"
#include "IUniverseParser.h"
#include "Logger.h"

using namespace std;
using namespace IPTVCommon;

namespace boost_generic = boost;

class UniverseHolder: public enable_shared_from_this<UniverseHolder>
{
	typedef boost_generic::property<boost_generic::vertex_name_t, unsigned int> VertexProperty;
	typedef boost_generic::property<boost_generic::edge_weight_t, float> EdgeProperty;
	typedef boost_generic::adjacency_list<boost_generic::vecS, boost_generic::vecS, boost_generic::directedS, VertexProperty, EdgeProperty> UniverseGraph;
	typedef boost_generic::graph_traits<UniverseGraph>::vertex_descriptor Vertex;
	typedef boost_generic::graph_traits<UniverseGraph>::edge_descriptor Edge;
	typedef boost_generic::graph_traits<UniverseGraph>::vertex_iterator VertexIterator;
	typedef boost_generic::graph_traits<UniverseGraph>::edge_iterator EdgeIterator;

	typedef boost_generic::property_map<UniverseGraph, boost::vertex_index_t >::type IndexMap;
	typedef boost_generic::property_map<UniverseGraph, boost::vertex_name_t >::type NameMap;
	typedef boost_generic::property_map<UniverseGraph, boost::edge_weight_t >::type WeightMap;
	typedef boost_generic::iterator_property_map<Vertex*, IndexMap, Vertex, Vertex&> PredecessorMap;
	typedef boost_generic::iterator_property_map<float*, IndexMap, float, float&> DistanceMap;

	IUniverseParser* _parser = nullptr;
	UniverseGraph _g_universe;

	void _InitializeGraph(map<unsigned int, vector<unsigned int>>& _map);
	Vertex _AddNode(unsigned int id);
	Vertex _AddNodeIfNotExists(unsigned int id);
	void _RemoveNode(unsigned int id);
	Vertex _GetNode(unsigned int id);
	void _AddEdge(const Vertex& node1, const Vertex& node2);
	void _RemoveEdge(const Vertex& node1, const Vertex& node2);
	bool _IsNullVertex(const Vertex& v);
public:
	shared_ptr<UniverseHolder> getptr() {
		return shared_from_this();
	}

	UniverseHolder(){}
	UniverseHolder(IUniverseParser& parser);
	~UniverseHolder(){
		delete _parser;
		_parser = nullptr;

		IPTVCommon::Logger::Current().LogDebug("UniverseHolder deleted successfully.");
	}

	void AddNode(const UniverseNode& n);
	void RemoveNode(const UniverseNode& n);
	UniverseNode GetNode(unsigned int id);
	void AddEdge(const UniverseNode& n1, const UniverseNode& n2);
	void RemoveEdge(const UniverseNode& n, const UniverseNode& n2);
	vector<unsigned int> GetShortestPath(const UniverseNode& x, const UniverseNode& y);
	void RemoveAllNodes() {
		_g_universe.clear();
	}
	void Print(){
		cout << "************* UniverseHolder *************" << '\n';
		cout << "*Vertices " << num_vertices(_g_universe) << '\n';

		NameMap keymap = boost_generic::get(boost_generic::vertex_name, _g_universe);

		IndexMap indexmap = boost_generic::get(boost_generic::vertex_index, _g_universe);

		for (VertexIterator v = vertices(_g_universe).first; v != vertices(_g_universe).second; ++v) {
			cout << indexmap[*v] + 1 << ": \"" << keymap[*v] << "\"\n";
		}

		cout << "*Edges\n";
		for (EdgeIterator e = edges(_g_universe).first; e != boost_generic::edges(_g_universe).second; ++e) {
			cout << keymap[boost_generic::source(*e, _g_universe)] << ' '
				<< keymap[boost_generic::target(*e, _g_universe)] << " 1.0\n"; // HACK!
		}
		cout << "/************* UniverseHolder *************/" << '\n';
	}
};

#endif