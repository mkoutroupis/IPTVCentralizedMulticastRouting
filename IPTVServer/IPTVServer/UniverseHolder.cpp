#include "UniverseHolder.h"

UniverseHolder::UniverseHolder(IUniverseParser& parser){
	_parser = &parser;

	try
	{
		_parser->Validate();
		IPTVCommon::Logger::Current().LogDebug("UniverseParser validated successfully.");

		_parser->Parse();
		IPTVCommon::Logger::Current().LogDebug("UniverseParser parsed successfully.");

		_InitializeGraph(_parser->m_universe);
		IPTVCommon::Logger::Current().LogDebug("UniverseHolder instantiated successfully.");
	}
	catch (exception& e)
	{
		string exceptionMessage(e.what());
		string errorMessage = "UniverseHolder error during instantiation. Exception: " + exceptionMessage;
		cerr << errorMessage << endl;
		throw errorMessage;
	}
}

void UniverseHolder::_InitializeGraph(map<unsigned int, vector<unsigned int>>& _map)
{
	for (map<unsigned int, vector<unsigned int>>::iterator i = _map.begin(); i != _map.end(); ++i)
	{
		unsigned int nodeId = i->first;

		Vertex node = _AddNodeIfNotExists(nodeId);

		vector<unsigned int> neighbors = i->second;
		for (vector<unsigned int>::iterator it = neighbors.begin(); it != neighbors.end(); ++it)
		{
			Vertex neightborNode = _AddNodeIfNotExists(*it);

			_AddEdge(node, neightborNode);
		}
	}

}

void UniverseHolder::AddNode(const UniverseNode& n)
{
	_AddNode(n.GetId());
}

void UniverseHolder::RemoveNode(const UniverseNode& n)
{
	_RemoveNode(n.GetId());
}

UniverseNode UniverseHolder::GetNode(unsigned int id)
{
	UniverseNode n(id);
	return n;
}

UniverseHolder::Vertex UniverseHolder::_GetNode(unsigned int id)
{
	VertexIterator vi, vi_end;

	NameMap keymap = boost_generic::get(boost_generic::vertex_name, _g_universe);

	for (tie(vi, vi_end) = boost_generic::vertices(_g_universe); vi != vi_end; ++vi)
	{
		if (keymap[*vi] == id)
		{
			return *vi;
		}
	}

	return boost_generic::graph_traits<UniverseGraph>::null_vertex();
}

bool UniverseHolder::_IsNullVertex(const Vertex& v)
{
	return v == boost_generic::graph_traits<UniverseGraph>::null_vertex();
}

UniverseHolder::Vertex UniverseHolder::_AddNode(unsigned int id)
{
	NameMap keymap = boost_generic::get(boost_generic::vertex_name, _g_universe);

	Vertex v = boost_generic::add_vertex(_g_universe);
	keymap[v] = id;

	return v;
}

UniverseHolder::Vertex UniverseHolder::_AddNodeIfNotExists(unsigned int id)
{
	Vertex node = _GetNode(id);
	if (!_IsNullVertex(node)) return node;

	return _AddNode(id);
}

void UniverseHolder::_RemoveNode(unsigned int id)
{
	Vertex node = _GetNode(id);
	if (!_IsNullVertex(node))
	{
		boost_generic::remove_vertex(node, _g_universe);
	}
}

void UniverseHolder::AddEdge(const UniverseNode& n1, const UniverseNode& n2)
{
	Vertex v1 = _GetNode(n1.GetId());
	Vertex v2 = _GetNode(n2.GetId());
	if (_IsNullVertex(v1) || _IsNullVertex(v2)) return;

	_AddEdge(v1, v2);
}

void UniverseHolder::RemoveEdge(const UniverseNode& n1, const UniverseNode& n2)
{
	Vertex v1 = _GetNode(n1.GetId());
	Vertex v2 = _GetNode(n2.GetId());
	if (_IsNullVertex(v1) || _IsNullVertex(v2)) return;

	_RemoveEdge(v1, v2);
}

void UniverseHolder::_AddEdge(const UniverseHolder::Vertex& node, const UniverseHolder::Vertex& neightborNode)
{
	WeightMap edge_weight = boost_generic::get(boost_generic::edge_weight, _g_universe);

	Edge e = boost_generic::add_edge(node, neightborNode, _g_universe).first;
	edge_weight[e] = 1;
}

void UniverseHolder::_RemoveEdge(const UniverseHolder::Vertex& node, const UniverseHolder::Vertex& neightborNode)
{
	boost_generic::remove_edge(node, neightborNode, _g_universe);
}

vector<unsigned int> UniverseHolder::GetShortestPath(const UniverseNode& x, const UniverseNode& y){
	
	vector<unsigned int> vertexPath;
	
	Vertex startVertex = _GetNode(x.GetId());
	if (_IsNullVertex(startVertex)) return vertexPath;

	vector<Vertex> predecessors(boost_generic::num_vertices(_g_universe));
	vector<float> distances(boost_generic::num_vertices(_g_universe));

	IndexMap indexMap = boost_generic::get(boost_generic::vertex_index, _g_universe);
	PredecessorMap predecessorMap(&predecessors[0], indexMap);
	DistanceMap distanceMap(&distances[0], indexMap);

	boost::dijkstra_shortest_paths(_g_universe, startVertex, boost::distance_map(distanceMap).predecessor_map(predecessorMap));

	typedef vector<UniverseGraph::edge_descriptor> PathType;

	PathType path;

	Vertex endVertex = _GetNode(y.GetId());
	if (_IsNullVertex(endVertex)) return vertexPath;

	for (Vertex u = predecessorMap[endVertex]; // Start by setting 'u' to the destination node's predecessor
		u != endVertex; // Keep tracking the path until we get to the source
		endVertex = u, u = predecessorMap[endVertex]) // Set the current vertex to the current predecessor, and the predecessor to one level up
	{
		pair<UniverseGraph::edge_descriptor, bool> edgePair = boost_generic::edge(u, endVertex, _g_universe);
		UniverseGraph::edge_descriptor edge = edgePair.first;

		path.push_back(edge);
	}

	NameMap nameMap = boost_generic::get(boost::vertex_name, _g_universe);

	/*// Write shortest path	
	cout << "Shortest path from x to y:" << endl;
	for (PathType::reverse_iterator pathIterator = path.rbegin(); pathIterator != path.rend(); ++pathIterator)
	{
		cout << nameMap[boost_generic::source(*pathIterator, _g_universe)] << " -> " << nameMap[boost_generic::target(*pathIterator, _g_universe)]
			<< " = " << boost_generic::get(boost_generic::edge_weight, _g_universe, *pathIterator) << endl;

	}

	cout << endl;

	cout << "Distance: " << distanceMap[endVertex] << endl;
	*/
	

	for (PathType::reverse_iterator pathIterator = path.rbegin(); pathIterator != path.rend(); ++pathIterator)
	{
		vertexPath.push_back(nameMap[boost_generic::target(*pathIterator, _g_universe)]);
	}

	return vertexPath;
}