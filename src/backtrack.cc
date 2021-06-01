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

	std::vector<bool> mark(Nq, false); // Mark whether vertices of query is visited or not
	std::vector<bool> inserted(Nq, false); // Avoid inserting overlapping vertices into heap
	std::vector<bool> isembd(Nd, false); // Avoid inserting overlapping vertices into embd

	std::vector<Vertex> embd(Nq, -1);
	size_t len = 0; // length of current partial embedding

	std::vector<Vertex> state(Nq); // Store recurrence state 


	std::vector<size_t> csPos(Nq); // the current position in the candidate set of each vertex

	std::vector<size_t> measure(Nq); // the measure for selecting extendable vertices


	Heap extVtx(Nq, measure); // Store extendable vertices

	Vertex u = 0; // current vertex in the query
	Vertex un, v;

	// Selecting root vertex
	long double argmin = (cs.GetCandidateSize(0) / ((long double)query.GetDegree(0))), argtemp;

	for (size_t i = 1; i < Nq; i++) {
		argtemp = (cs.GetCandidateSize(i) / ((long double)query.GetDegree(i)));
		if (argtemp < argmin) {
			u = i;
			argmin = argtemp;
		}
	}


	state[len++] = u;

	bool embedded_u, embedded_v;

	bool call = true; //Store whether the function is examining new vertex or backtracking

	v = cs.GetCandidate(u, 0);
	embd[u] = v;
	isembd[v] = true;
	csPos[u] = 1;
	mark[u] = true;

	for (size_t k = query.GetNeighborStartOffset(u); k < query.GetNeighborEndOffset(u); k++) {
		un = query.GetNeighbor(k);
		measure[un] = (cs.GetCandidateSize(un) - csPos[un]);
		extVtx.insert(un);
		inserted[un] = true;
	}



	while (len > 0) {
		if (len == Nq) {
			std::cout << "a";
			for (size_t i = 0; i < Nq; i++)
			{
				std::cout << " " << embd[i];
			}
			std::cout << "\n";

			isembd[embd[u]] = false;
			embd[u] = -1;
			mark[u] = false;
			len--;
		}

		else {
			if (call) {
				u = extVtx.remove();
				inserted[u] = false;
			}
			else {
				if (embd[u] != -1) isembd[embd[u]] = false;
				embd[u] = -1;
				measure[u] = (cs.GetCandidateSize(u) - csPos[u]);
				extVtx.insert(u);
				inserted[u] = true;

				len--;

				u = state[len];
				mark[u] = false;
				isembd[embd[u]] = false;
				embd[u] = -1;
				//std::cout << u << " " << csPos[u] << " " << cs.GetCandidateSize(u) << std::endl;
			}
		}
		//for (size_t j = 0; j < len; j++) std::cout << state[j] << " ";
		//std::cout << "\n";

		embedded_u = false;



		for (size_t j = csPos[u]; j < cs.GetCandidateSize(u); j++) {
			v = cs.GetCandidate(u, j);
			//std::cout << u << " " << csPos[u]<< " " << v << "\n";
			//std::cout << v << std::endl;

			if (isembd[v]) continue;

			embedded_v = true;

			if (len < query.GetDegree(u)) {
				for (size_t pos = 0; pos < len; pos++) {
					if (!(data.IsNeighbor(embd[state[pos]], v))) {
						embedded_v = false;
						break;
					}
				}
			}
			else {
				for (size_t k = query.GetNeighborStartOffset(u); k < query.GetNeighborEndOffset(u); k++) {
					un = query.GetNeighbor(k);
					if (mark[un] && !(data.IsNeighbor(embd[un], v))) {
						embedded_v = false;
						break;
					}
				}
			}


			if (embedded_v) {
				embd[u] = v;
				isembd[v] = true;
				csPos[u] = j + 1;
				state[len++] = u;
				embedded_u = true;
				mark[u] = true;
				call = true;

				for (size_t k = query.GetNeighborStartOffset(u); k < query.GetNeighborEndOffset(u); k++) {
					un = query.GetNeighbor(k);
					if (!mark[un] && !inserted[un]) {
						measure[un] = (cs.GetCandidateSize(un) - csPos[un]);
						extVtx.insert(un);
						inserted[un] = true;
					}
				}
				break;
			}
		}
		if (!embedded_u) { // backtracking
			call = false;
			csPos[u] = 0;
			mark[u] = false;
		}
	}

}