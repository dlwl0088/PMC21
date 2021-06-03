/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"
#include "stack.h"
#include <map>

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}

void Backtrack::PrintAllMatches(const Graph& data, const Graph& query,
	const CandidateSet& cs) {

	size_t num_query = query.GetNumVertices(), num_data = data.GetNumVertices();
	std::cout << "t " << num_query << "\n";

	std::vector<Vertex> embedding(num_query, -1);
	std::vector<bool> query_mapped(num_query, false), data_mapped(num_data, false), Is_extendableVertex(num_query, false);
	Stack state(num_query); //Store recurrence state

	std::vector<size_t> measure(num_query), pivotCS(num_query), posCS(num_query), num_matchedNeighbor(num_query);
	std::multimap<size_t, Vertex> extendableVertex;

	std::vector<std::vector<std::pair<Vertex, Vertex>>> extendableCS(num_query);
	for (Vertex w = 0; w < num_query; w++) {
		extendableCS[w].resize(cs.GetCandidateSize(w));
		for (size_t i = 0; i < cs.GetCandidateSize(w); i++) {
			extendableCS[w][i] = std::pair<Vertex, Vertex>(cs.GetCandidate(w, i), -1);
		}
	}

	Vertex u = 0, un, v = -1;
	std::pair< Vertex, Vertex > temp;
	size_t itr;
	std::multimap<size_t, Vertex>::iterator it;
	std::pair<std::multimap<size_t, Vertex>::iterator, std::multimap<size_t, Vertex>::iterator> range;

	// Selecting root vertex
	size_t argmax = cs.GetCandidateSize(0) * query.GetDegree(0), argtemp;
	for (size_t i = 1; i < num_query; i++) {
		argtemp = cs.GetCandidateSize(i) * query.GetDegree(i);
		if (argtemp > argmax) {
			u = i;
			argmax = argtemp;
		}
	}

	measure[u] = 0;
	extendableVertex.insert(std::pair<size_t, Vertex>(measure[u], u));
	Is_extendableVertex[u] = true;

	bool call = true; //Store whether the function is examining new vertex or backtracking
	bool matched;
	std::vector<size_t> stoppedNeighborOffset(num_query);

	do {
		if (call) {
			u = (it = --extendableVertex.end())->second;
			extendableVertex.erase(it);
			Is_extendableVertex[u] = false;

			posCS[u] = pivotCS[u];
			state.push(u);

		}
		else {
			u = state.peek();
			data_mapped[extendableCS[u][posCS[u]].first] = false;
			query_mapped[u] = false;
			embedding[u] = -1;


			for (size_t k = query.GetNeighborStartOffset(u); k < stoppedNeighborOffset[u]; k++) {
				un = query.GetNeighbor(k);

				if (!query_mapped[un]) {
					num_matchedNeighbor[un]--;

					itr = pivotCS[un];
					while (itr > 0) {
						--itr;
						if (extendableCS[un][itr].second != u) {
							itr++;
							break;
						}

						extendableCS[un][itr].second = -1;
					}
					pivotCS[un] = itr;

					if (Is_extendableVertex[un]) {
						range = extendableVertex.equal_range(measure[un]);
						for (it = range.first; it != range.second; it++) if (it->second == un) break;;
						extendableVertex.erase(it);
					}
					else {
						Is_extendableVertex[un] = true;
					}

					measure[un] = (pivotCS[un]+1) * num_matchedNeighbor[un];
					extendableVertex.insert(std::pair<size_t, Vertex>(measure[un], un));
				}
			}
			posCS[u]++;
		}

		while ((posCS[u] < cs.GetCandidateSize(u)) && data_mapped[v = extendableCS[u][posCS[u]].first]) posCS[u]++;

		if (posCS[u] == cs.GetCandidateSize(u)) {
			measure[u] = (pivotCS[u]+1) * num_matchedNeighbor[u];
			extendableVertex.insert(std::pair<size_t, Vertex>(measure[u], u));
			Is_extendableVertex[u] = true;

			state.pop();

			call = false;
			continue;
		}

		matched = true;
		stoppedNeighborOffset[u] = query.GetNeighborEndOffset(u);
		for (size_t k = query.GetNeighborStartOffset(u); k < query.GetNeighborEndOffset(u); k++) {
			un = query.GetNeighbor(k);

			if (!query_mapped[un]) {
				num_matchedNeighbor[un]++;
				for (itr = pivotCS[un]; itr < cs.GetCandidateSize(un); itr++) {
					if (!data.IsNeighbor(v, extendableCS[un][itr].first)) {
						extendableCS[un][itr].second = u;

						temp = extendableCS[un][itr];
						extendableCS[un][itr] = extendableCS[un][pivotCS[un]];
						extendableCS[un][pivotCS[un]] = temp;

						pivotCS[un]++;
					}
				}

				if (pivotCS[un] == cs.GetCandidateSize(un)) {
					stoppedNeighborOffset[u] = k + 1;
					matched = false;
					break;
				}
				else {
					if (Is_extendableVertex[un]) {
						range = extendableVertex.equal_range(measure[un]);
						for (it = range.first; it != range.second; it++) if (it->second == un) break;;
						extendableVertex.erase(it);
					}
					else {
						Is_extendableVertex[un] = true;
					}

					measure[un] = (pivotCS[un]+1 )* num_matchedNeighbor[un];
					extendableVertex.insert(std::pair<size_t, Vertex>(measure[un], un));
				}
			}
		}

		if (!matched) {
			call = false;
			continue;
		}

		data_mapped[v] = true;
		query_mapped[u] = true;
		embedding[u] = v;

		if (state.size() == num_query) {
			std::cout << "a";
			for (size_t i = 0; i < num_query; i++) {
				std::cout << " " << embedding[i];
			}
			std::cout << "\n";
			call = false;
		}
		else {
			call = true;
		}

	} while (state.size() > 0);
}