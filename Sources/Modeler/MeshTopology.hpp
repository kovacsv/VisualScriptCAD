#ifndef MODELER_MESHTOPOLOGY_HPP
#define MODELER_MESHTOPOLOGY_HPP

#include <vector>
#include <functional>
#include <unordered_map>

namespace Modeler
{

extern const unsigned int NoTriangle;

class MeshTopology
{
public:
	friend class MeshTopologyBuilder;

	enum class Status
	{
		Valid,
		Invalid
	};

	struct Edge
	{
		unsigned int	beg;
		unsigned int	end;
		unsigned int	triangle1;
		unsigned int	triangle2;
	};

	struct TriangleEdge
	{
		unsigned int	edge;
		bool			reversed;
	};

	struct Triangle
	{
		TriangleEdge	edge1;
		TriangleEdge	edge2;
		TriangleEdge	edge3;
	};
	
	MeshTopology ();

	Status							GetStatus () const;
	bool							IsEmpty () const;
	void							Clear ();

	bool							IsClosed () const;

	const std::vector<Triangle>&	GetTriangles () const;
	const std::vector<Edge>&		GetEdges () const;

private:
	Status					status;
	std::vector<Triangle>	triangles;
	std::vector<Edge>		edges;
};

class MeshTopologyBuilder
{
public:
	enum class Result
	{
		NoError,
		NonManifoldEdgeFound,
		InvalidTopology
	};

	MeshTopologyBuilder (MeshTopology& topology);

	Result	AddTriangle (unsigned int v1, unsigned int v2, unsigned int v3);

private:
	struct EdgeKey
	{
		bool			operator== (const EdgeKey& rhs) const;

		unsigned int	beg;
		unsigned int	end;
	};
	
	struct EdgeKeyHash
	{
		template <class EdgeKey>
		std::size_t operator() (const EdgeKey& key) const
		{
			return std::hash<unsigned int> {} (key.beg) + std::hash<unsigned int> {} (key.end);
		}
	};

	Result	AddTriangleEdges (MeshTopology::Triangle& triangle, unsigned int v1, unsigned int v2, unsigned int v3);
	Result	AddEdge (unsigned int v1, unsigned int v2, unsigned int triangle, MeshTopology::TriangleEdge& triEdge);

	MeshTopology&												topology;
	std::unordered_map<EdgeKey, unsigned int, EdgeKeyHash>		edgeMap;
};

}

#endif
