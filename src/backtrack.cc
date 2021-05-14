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

	bool* mark = new bool[Nq]();
	Vertex* embd = new Vertex[Nq]();

	Vertex* state=new Vertex[Nq];
	size_t lenM = 0;

	size_t* csPos = new size_t[Nq]();

	Vertex u = 0;

	size_t* csSize = new size_t[Nq]();
	for (size_t i = 0; i < Nq; i++) csSize[i] = cs.GetCandidateSize(i);
	
	Heap extVtx(Nq, csSize);

	double argmin = (cs.GetCandidateSize(0) / query.GetDegree(0)), argtemp;

	for (size_t i = 1; i < Nq; i++) {
		argtemp = (cs.GetCandidateSize(i) / query.GetDegree(i));
		if (argtemp < argmin) {
			u = i;
			argmin = argtemp;
		}
	}

	state[lenM++] = u;
	mark[u] = true;
	extVtx.insert(u);

	bool call = true;
	while (lenM > 0) {
		bool matched = false;
		if (lenM == Nq) {
			std::cout << "a" ;
			for (int i = 0; i < Nq; i++)std::cout<<" "<<embd[i];
			std::cout << "\n";
			call = false;
		
		}
		else {
			u = call ? extVtx.remove() : state[lenM - 1];

			size_t j = csPos[u];
			Vertex un, v;
			while (j < cs.GetCandidateSize(u)) {
				bool Extendable = true;
				v = cs.GetCandidate(u, j);
				for (size_t k = query.GetNeighborStartOffset(u); k < query.GetNeighborEndOffset(u); k++) {
					un = query.GetNeighbor(k);
					if (mark[un] && !(data.IsNeighbor(embd[un], v))) {
						Extendable = false;
						break;
					}
				}

				if (Extendable) {
					embd[u] = v;
					csPos[u] = j + 1;
					state[lenM++] = u;
					matched = true;
					mark[u] = true;

					for (size_t k = query.GetNeighborStartOffset(u); k < query.GetNeighborEndOffset(u); k++) {
						un = query.GetNeighbor(k);
						if (!mark[un]) extVtx.insert(un);
					}
					break;
				}
				j++;
			}
		}



		if (!matched) {

			call = false;
			lenM--;
			extVtx.insert(u);
			csPos[u] = 0;
			mark[u] = false;
		}

	}





	delete[] mark;
	delete[] embd;
	delete[] state;
	delete[] csSize;
	delete[] csPos;
}