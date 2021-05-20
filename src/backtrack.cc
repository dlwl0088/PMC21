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
	//std::string path = "res.igraph";
	//std::ofstream fp;
	//fp.open(path);

	size_t Nq = query.GetNumVertices();
	size_t Nd = data.GetNumVertices();

	std::cout << "t " << Nq << "\n";
	//fp << "t " << Nq;

	bool* mark = new bool[Nq](); // Mark whether vertices of query is visited or not
	bool* inserted = new bool[Nq](); // Avoid inserting overlapping vertices into heap
	bool* isembd = new bool[Nd](); // Avoid inserting overlapping vertices into embd

	Vertex* embd = new Vertex[Nq]();
	size_t len = 0; // length of current partial embedding

	Vertex* state = new Vertex[Nq](); // Store recurrence state 


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

	for (size_t i = 0; i < Nq; i++) embd[i] = -1;

	state[len] = u;
	bool fi = true;
	bool embedded_u, embedded_v;
	extVtx.insert(u);
	inserted[u] = true;

	bool call = true;//Store whether the function is examining new vertex or backtracking
	while (len > 0 || fi) {
		fi = false;
		if (len == Nq) {
			std::cout << "a";
			//fp << "\n";
			//fp << "a";
			for (size_t i = 0; i < Nq; i++)
			{
				std::cout << " " << embd[i];
				//fp << " " << embd[i];
			}
			std::cout << "\n";

			call = false;
		}

		u = call ? extVtx.remove() : state[len--];
		if (call) inserted[u] = false;
		//if (!call) std::cout << (int)state[len + 1] << "\n";

		//std::cout << call << " " << (int)len << " " << (int)u << "\n";

		Vertex un, v;

		embedded_u = false;
		for (size_t j = csPos[u]; j < cs.GetCandidateSize(u); j++) {
			v = cs.GetCandidate(u, j);
			//std::cout << (int)j << "/" << cs.GetCandidateSize(u) << ": ";
			//std::cout << (int)v << "\n";

			if (isembd[v]) continue;

			embedded_v = true;
			//std::cout << query.GetNeighborStartOffset(u) << "\n";
			//std::cout << query.GetNeighborEndOffset(u) << "\n";
			for (size_t k = query.GetNeighborStartOffset(u); k < query.GetNeighborEndOffset(u); k++) {
				un = query.GetNeighbor(k);
				//std::cout << k << " un: " << (int)un << "\n";
				//std::cout << mark[un] << "\n";
				//std::cout << embd[un] << "\n";
				//if (mark[un]) std::cout << data.IsNeighbor(embd[un], v) << "\n";
				if (mark[un] && !(data.IsNeighbor(embd[un], v))) {
					embedded_v = false;
					break;
				}
			}

			if (embedded_v) {
				embd[u] = v;
				isembd[v] = true;
				csPos[u] = j + 1;
				state[len++] = u;
				embedded_u = true;
				mark[u] = true;

				for (size_t k = query.GetNeighborStartOffset(u); k < query.GetNeighborEndOffset(u); k++) {
					un = query.GetNeighbor(k);
					if (!mark[un] && !inserted[un]) {
						extVtx.insert(un);
						inserted[un] = true;
					}
				}
				break;
			}
		}
		if (!embedded_u) { // backtracking
			call = false;
			if (!inserted[u]) {
				extVtx.insert(u);
				inserted[u] = true;
			}
			csPos[u] = 0;
			mark[u] = false;
			isembd[embd[u]] = false;
		}
	}

	delete[] inserted;
	delete[] isembd;

	delete[] mark;
	delete[] embd;
	delete[] state;
	delete[] measure;
	delete[] csPos;

	//fp.close();
}
