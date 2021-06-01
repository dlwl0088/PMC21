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
	size_t Nd = data.GetNumVertices();

	std::cout << "t " << Nq << "\n";

	std::vector<Vertex> embd(Nq, -1); //  Store partial embedding
	std::vector<bool> isembd(Nd, false); // Avoid inserting overlapping vertices into embd	
	std::vector<bool> mark(Nq, false); // Mark whether vertices of query is visited or not
	std::vector<size_t> csPos(Nq); // the current position in the candidate set of each vertex

	std::vector<Vertex> state(Nq); // Store recurrence state 
	size_t len = 0; // length of current partial embedding

	std::vector<size_t> measure(Nq); // the measure for selecting extendable vertices
	Heap extVtx(Nq, measure); // Store extendable vertices
	std::vector<bool> inserted(Nq, false); // Avoid inserting overlapping vertices into heap

	Vertex u = 0; // current vertex in the query
	Vertex un, v;

	// Selecting root vertex

	long double argmin = cs.GetCandidateSize(0) / ((long double)query.GetDegree(0)), argtemp;

	for (size_t i = 1; i < Nq; i++) {
		argtemp = cs.GetCandidateSize(i) / ((long double)query.GetDegree(i));
		if (argtemp < argmin) {
			u = i;
			argmin = argtemp;
		}
	}

	bool embedded_u, embedded_v;

	bool call = true; //Store whether the function is examining new vertex or backtracking

	measure[u] = cs.GetCandidateSize(u);
	extVtx.insert(u);
	inserted[u] = true;

	do {
		if (len == Nq) {
			std::cout << "a";
			for (size_t i = 0; i < Nq; i++)
			{
				std::cout << " " << embd[i];
			}
			std::cout << "\n";
			call = false;
		}

		if (call) {
			u = extVtx.remove();
			inserted[u] = false;
		}
		else {
			u = state[--len];
			mark[u] = false;
			isembd[embd[u]] = false;
			embd[u] = -1;
			//std::cout << u << " " << csPos[u] << " " << cs.GetCandidateSize(u) << std::endl;
		}

		//std::cout << "\n";

		embedded_u = false;

		for (size_t j = csPos[u]; j < cs.GetCandidateSize(u); j++) {
			v = cs.GetCandidate(u, j);
			//std::cout << u << " " << csPos[u]<< " " << v << "\n";
			//std::cout << v << std::endl;

			if (isembd[v]) continue;

			embedded_v = true;


			for (size_t k = query.GetNeighborStartOffset(u); k < query.GetNeighborEndOffset(u); k++) {
				un = query.GetNeighbor(k);
				if (mark[un] && !(data.IsNeighbor(embd[un], v))) {
					embedded_v = false;
					break;
				}
			}


			if (embedded_v) {
				embd[u] = v;
				isembd[v] = true;
				csPos[u] = j + 1;
				mark[u] = true;

				for (size_t k = query.GetNeighborStartOffset(u); k < query.GetNeighborEndOffset(u); k++) {
					un = query.GetNeighbor(k);
					if (!mark[un]&&!inserted[un]) {
							measure[un] = cs.GetCandidateSize(un) - csPos[un];
							extVtx.insert(un);
							inserted[un] = true;

					}
				}

				state[len++] = u;

				embedded_u = true;
				call = true;

				break;
			}
		}
		if (embedded_u) continue;

		//backtrack
		call = false;
		csPos[u] = 0;

		measure[u] = cs.GetCandidateSize(u);
		extVtx.insert(u);
		inserted[u] = true;

	} while (len);
}