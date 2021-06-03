/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"
#include "heap.h"
#include "stack.h"

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}

void Backtrack::PrintAllMatches(const Graph& data, const Graph& query,
	const CandidateSet& cs) {

	size_t num_query = query.GetNumVertices(), num_data = data.GetNumVertices();
	std::cout << "t " << num_query << "\n";

	std::vector<Vertex> embedding(num_query, -1);
	std::vector<bool> query_mapped(num_query, false), data_mapped(num_data, false), Is_extendableVertex(num_query, false);
	Stack state(num_query); //Store recurrence state

	std::vector<size_t> measure(num_query), pivotCS(num_query), posCS(num_query), num_extendableNeighbor(num_query);
	Heap extendableVertex(num_query, measure);
	std::vector<std::vector<std::pair<Vertex, size_t>>> extendableCS(num_query);
	for (Vertex w = 0; w < num_query; w++) {
		extendableCS[w].resize(cs.GetCandidateSize(w));
		for (size_t i = 0; i < cs.GetCandidateSize(w); i++) {
			extendableCS[w][i] = std::pair<Vertex, size_t>(cs.GetCandidate(w, i), num_query + 1);
		}
	}

	Vertex u = 0, un, v=-1;
	std::pair< Vertex, size_t > temp;

	// Selecting root vertex
	long double argmin = cs.GetCandidateSize(0) / ((long double)query.GetDegree(0)), argtemp;
	for (size_t i = 1; i < num_query; i++) {
		argtemp = cs.GetCandidateSize(i) / ((long double)query.GetDegree(i));
		if (argtemp < argmin) {
			u = i;
			argmin = argtemp;
		}
	}

	measure[u] = cs.GetCandidateSize(u) * query.GetDegree(u);
	extendableVertex.insert(u);
	Is_extendableVertex[u] = true;

	bool call = true; //Store whether the function is examining new vertex or backtracking
	bool matches;
	std::vector<size_t> stoppedNeighborOffset(num_query);
		
	do {
		matches = true;
		if (call) {
			u = extendableVertex.remove();
			posCS[u] = pivotCS[u];
		}
		else {
			u = state.peek();
			data_mapped[extendableCS[u][posCS[u]].first] = false;
			for (size_t k = stoppedNeighborOffset[u]; k >= query.GetNeighborStartOffset(u); k--) {
				un = query.GetNeighbor(k);

				if (!query_mapped[un]) {
					num_extendableNeighbor[un]--;

					for (size_t it = pivotCS[un] - 1; extendableCS[un][it].second == state.size(); it--) {
						if (!data.IsNeighbor(embedding[u], extendableCS[un][it].first)) {
							extendableCS[un][it].second = num_query + 1;
							pivotCS[un]--;
						}
					}

					if (Is_extendableVertex[un]) {
						extendableVertex.remove(un);
					}
					else {
						Is_extendableVertex[un] = true;
					}

					measure[un] = (cs.GetCandidateSize(un) - pivotCS[un]) * num_extendableNeighbor[un];
					extendableVertex.insert(un);
				}
			}
			posCS[u]++;
		}

		while ((posCS[u] < cs.GetCandidateSize(u)) && data_mapped[v = extendableCS[u][posCS[u]].first]) posCS[u]++;
		matches = (posCS[u] < cs.GetCandidateSize(u));

		if (!matches) {
			if (!call) {
				state.pop();
				query_mapped[u] = false;
				data_mapped[embedding[u]] = false;
				embedding[u] = -1;
			}
			extendableVertex.insert(u);
			call = false;
			continue;
		}

		stoppedNeighborOffset[u] = query.GetNeighborEndOffset(u);
		for (size_t k = query.GetNeighborStartOffset(u); k < query.GetNeighborEndOffset(u); k++) {
			un = query.GetNeighbor(k);

			if (!query_mapped[un]) {
				num_extendableNeighbor[un]++;
				for (size_t itr = pivotCS[un]; itr < cs.GetCandidateSize(un); itr++) {
					if (!data.IsNeighbor(v, extendableCS[un][itr].first)) {
						extendableCS[un][itr].second = state.size();

						temp = extendableCS[un][itr];
						extendableCS[un][itr] = extendableCS[un][pivotCS[un]];
						extendableCS[un][pivotCS[un]] = temp;

						pivotCS[un]++;
					}
				}

				if (pivotCS[un] == cs.GetCandidateSize(un)) {
					stoppedNeighborOffset[u] = k;
					matches = false;
					break;
				}
				else {
					measure[un] = (cs.GetCandidateSize(un) - pivotCS[un]) * num_extendableNeighbor[un];

					if (Is_extendableVertex[un]) {
						extendableVertex.remove(un);
					}
					else {
						Is_extendableVertex[un] = true;
					}

					extendableVertex.insert(un);
				}
			}
		}

		if (!matches) {
			call = false;
			continue;
		}

		embedding[u] = v;
		data_mapped[v] = true;

		if (call) {
			query_mapped[u] = true;
			state.push(u);
			Is_extendableVertex[u] = false;
		}

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