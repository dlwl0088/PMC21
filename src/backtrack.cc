/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}

void Backtrack::PrintAllMatches(const Graph& data, const Graph& query,
	const CandidateSet& cs) {
	std::cout << "t " << query.GetNumVertices() << "\n";

	// implement your code here.

	//Store embedding
	M.assign(query.GetNumVertices(), -1);
	Size_M = 0;

}


void Backtrack::recurBacktrack(const Graph& data, const Graph& query,
	const CandidateSet& cs) {

	if (Size_M == query.GetNumVertices()) {
		std::cout << "t " << query.GetNumVertices() << "\n";
	}
	else if (Size_M == 0) {
		size_t r = 0;//root ID for query DAG

		float argmin = (cs.GetCandidateSize(0) / query.GetDegree(0)), argtemp;

		for (size_t i = 1; i < query.GetNumVertices(); i++) {
			argtemp = (cs.GetCandidateSize(i) / query.GetDegree(i));
			if (argtemp < argmin) {
				r = i;
				argmin = argtemp;
			}
		}

		query.MarkVisited(r, true);

		for (size_t i = 0, v; i < cs.GetCandidateSize(r); i++) {

			v = cs.GetCandidate(r, i);
			M[r] = v;
			Size_M=1;
			data.MarkVisited(v, true);

			recurBacktrack(data, query, cs);
			data.MarkVisited(v, false);

		}


	}
	else {


	}
	return;
}

