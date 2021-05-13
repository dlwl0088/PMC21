/**
 * @file backtrack.h
 *
 */

#ifndef BACKTRACK_H_
#define BACKTRACK_H_

#include "candidate_set.h"
#include "common.h"
#include "graph.h"

class Backtrack {
public:
	Backtrack();
	~Backtrack();

	void PrintAllMatches(const Graph& data, const Graph& query,
		const CandidateSet& cs);

	void recurBacktrack(const Graph& data, const Graph& query,
		const CandidateSet& cs);//added

private:
	std::vector<size_t> M;
	size_t Size_M;

};

#endif  // BACKTRACK_H_
