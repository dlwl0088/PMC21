/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"
#include <map>
#include <stack>

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}

void Backtrack::PrintAllMatches(const Graph& data, const Graph& query,
	const CandidateSet& cs) {

	size_t num_query = query.GetNumVertices(), num_data = data.GetNumVertices();
	std::cout << "t " << num_query << "\n";

	std::vector<Vertex> embedding(num_query, -1);
	std::vector<bool> query_mapped(num_query, false), data_mapped(num_data, false);
	std::stack<Vertex> state; //Store recurrence state


	std::vector<size_t> pivotCS(num_query), posCS(num_query), num_matchedNeighbor(num_query, 1);
	std::vector<float> measure(num_query);
	std::multimap<float, Vertex> extendableVertex;

	std::vector<std::vector<std::pair<Vertex, Vertex>>> extendableCS(num_query);
	for (Vertex w = 0; w < num_query; w++) {
		extendableCS[w].resize(cs.GetCandidateSize(w));
		for (size_t i = 0; i < cs.GetCandidateSize(w); i++) extendableCS[w][i] = std::pair<Vertex, Vertex>(cs.GetCandidate(w, i), -1);
	}

	Vertex u = 0, un, v = -1;
	std::pair< Vertex, Vertex > tempCS;
	std::pair< float, Vertex > tempEV;
	std::multimap<float, Vertex>::iterator it;
	std::pair<std::multimap<float, Vertex>::iterator, std::multimap<float, Vertex>::iterator> range;

	for (Vertex w = 0; w < num_query; w++) extendableVertex.insert(std::pair<float, Vertex>(measure[w] = ((float)cs.GetCandidateSize(w)), w));;

	bool call = true; //Store whether the function is examining new vertex or backtracking
	bool matched;
	std::vector<size_t> stoppedNeighborOffset(num_query);

	do {
		if (call) {
			u = (it = extendableVertex.begin())->second;
			extendableVertex.erase(it);

			state.push(u);
			query_mapped[u] = true;

			posCS[u] = pivotCS[u];
		}
		else {
			u = state.top();
			data_mapped[extendableCS[u][posCS[u]].first] = false;
			embedding[u] = -1;

			for (size_t k = query.GetNeighborStartOffset(u); k < stoppedNeighborOffset[u]; k++) {
				un = query.GetNeighbor(k);

				if (!query_mapped[un]) {
					num_matchedNeighbor[un]--;

					size_t i = pivotCS[un];
					while (i > 0) {
						--i;
						if (extendableCS[un][i].second != u) {
							i++;
							break;
						}

						extendableCS[un][i].second = -1;
					}
					pivotCS[un] = i;

					range = extendableVertex.equal_range(measure[un]);
					for (it = range.first; it != range.second; it++) if (it->second == un) break;;
					tempEV = *it;
					extendableVertex.erase(it);
					tempEV.first = measure[un] = ((float)(cs.GetCandidateSize(un) - pivotCS[un])) / (num_matchedNeighbor[un]);

					extendableVertex.insert(tempEV);
				}
			}
			posCS[u]++;
		}

		while ((posCS[u] < cs.GetCandidateSize(u)) && data_mapped[v = extendableCS[u][posCS[u]].first]) posCS[u]++;

		if (posCS[u] == cs.GetCandidateSize(u)) {
			extendableVertex.insert(std::pair<float, Vertex>(measure[u] = ((float)(cs.GetCandidateSize(u) - pivotCS[u])) / (num_matchedNeighbor[u]), u));

			state.pop();
			query_mapped[u] = false;

			call = false;
			continue;
		}

		matched = true;
		stoppedNeighborOffset[u] = query.GetNeighborEndOffset(u);
		for (size_t k = query.GetNeighborStartOffset(u); k < query.GetNeighborEndOffset(u); k++) {
			un = query.GetNeighbor(k);

			if (!query_mapped[un]) {
				num_matchedNeighbor[un]++;
				for (size_t i = pivotCS[un]; i < cs.GetCandidateSize(un); i++) {
					if (!data.IsNeighbor(v, extendableCS[un][i].first)) {
						extendableCS[un][i].second = u;

						tempCS = extendableCS[un][i];
						extendableCS[un][i] = extendableCS[un][pivotCS[un]];
						extendableCS[un][pivotCS[un]] = tempCS;

						pivotCS[un]++;
					}
				}

				if (pivotCS[un] == cs.GetCandidateSize(un)) {
					stoppedNeighborOffset[u] = k + 1;
					matched = false;
					break;
				}
				else {
					range = extendableVertex.equal_range(measure[un]);
					for (it = range.first; it != range.second; it++) if (it->second == un) break;;
					tempEV = *it;
					extendableVertex.erase(it);
					tempEV.first = measure[un] = ((float)(cs.GetCandidateSize(un) - pivotCS[un])) / (num_matchedNeighbor[un]);

					extendableVertex.insert(tempEV);
				}
			}
		}

		if (!matched) {
			call = false;
			continue;
		}


		embedding[u] = v;
		data_mapped[v] = true;

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