/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"
#include "heap.h"

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}

void Backtrack::PrintAllMatches(const Graph& data, const Graph& query,
	const CandidateSet& cs) {

	size_t Nq = query.GetNumVertices();
	std::cout << "t " << Nq << "\n";

	bool* mark = new bool[Nq](); // Mark whether vertices of query is visited or not

	Vertex* embd = new Vertex[Nq]();
	size_t len = 0; // length of current partial embedding

	Vertex* state = new Vertex[Nq]; // Store recurrence state 


	size_t* csPos = new size_t[Nq](); // the current position in the candidate set of each vertex

	size_t* measure = new size_t[Nq](); // the measure for selecting extendable vertices
	for (size_t i = 0; i < Nq; i++) measure[i] = cs.GetCandidateSize(i);

	Heap extVtx(Nq, measure); // Store extendable vertices

	Vertex u = 0; // current vertex in the query

	// Selecting root vertex
	double argmin = (cs.GetCandidateSize(0) / query.GetDegree(0)), argtemp;

	for (size_t i = 1; i < Nq; i++) {
		argtemp = (cs.GetCandidateSize(i) / query.GetDegree(i));
		if (argtemp < argmin) {
			u = i;
			argmin = argtemp;
		}
	}

	state[len++] = u;
	extVtx.insert(u);

	bool call = true;//Store whether the function is examining new vertex or backtracking
	while (len > 0) {
		if (len == Nq) {
			std::cout << "a";
			for (int i = 0; i < Nq; i++)std::cout << " " << embd[i];
			std::cout << "\n";

			call = false;
		}
		else {
			u = call ? extVtx.remove() : state[len - 1];

			Vertex un, v;

			bool embedded_u = false;
			for (size_t j = csPos[u]; j < cs.GetCandidateSize(u); j++) {
				v = cs.GetCandidate(u, j);

				bool embedded_v = true;
				for (size_t k = query.GetNeighborStartOffset(u); k < query.GetNeighborEndOffset(u); k++) {
					un = query.GetNeighbor(k);
					if (mark[un] && !(data.IsNeighbor(embd[un], v))) {
						embedded_v = false;
						break;
					}
				}

				if (embedded_v) {
					embd[u] = v;
					csPos[u] = j + 1;
					state[len++] = u;
					embedded_u = true;
					mark[u] = true;

					for (size_t k = query.GetNeighborStartOffset(u); k < query.GetNeighborEndOffset(u); k++) {
						un = query.GetNeighbor(k);
						if (!mark[un]) extVtx.insert(un);
					}
					break;
				}
			}
			if (!embedded_u) { // backtracking
				call = false;
				len--;
				extVtx.insert(u);
				csPos[u] = 0;
				mark[u] = false;
			}
		}
	}

	delete[] mark;
	delete[] embd;
	delete[] state;
	delete[] measure;
	delete[] csPos;
}