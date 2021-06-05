/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"
#include <map>
#include <stack>
#include <cmath>

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}

void Backtrack::PrintAllMatches(const Graph& data, const Graph& query,
	const CandidateSet& cs) {

	size_t num_Vq = query.GetNumVertices(), num_VG = data.GetNumVertices();
	std::cout << "t " << num_Vq << "\n";

	std::vector<Vertex> embedding(num_Vq, -1);
	std::vector<bool> Vq_mapped(num_Vq, false), VG_mapped(num_VG, false);
	std::stack<Vertex> state; //Store recurrence state

	float exp = 0.75;

	std::vector<size_t> pivotCS(num_Vq), posCS(num_Vq), num_parent(num_Vq, 1);
	std::vector<float> key(num_Vq);
	std::vector<size_t> num_CS(num_Vq);
	std::multimap<float, Vertex> ExVertex; //ExtendableVertex
	std::multimap<size_t, Vertex> ExVertexList;

	std::vector<std::vector<std::pair<Vertex, Vertex>>> ExCan(num_Vq);//Extendable Candidates
	for (Vertex w = 0; w < num_Vq; w++) {
		ExCan[w].resize(cs.GetCandidateSize(w));
		for (size_t i = 0; i < cs.GetCandidateSize(w); i++) ExCan[w][i] = std::pair<Vertex, Vertex>(cs.GetCandidate(w, i), -1);
	}

	Vertex u = 0, un, v = -1;
	std::pair< Vertex, Vertex > tempCS;
	std::pair< float, Vertex > tempEV;
	std::pair< size_t, Vertex > tempEVL;
	std::multimap<float, Vertex>::iterator it;
	std::multimap<size_t, Vertex>::iterator itL;
	std::pair<std::multimap<float, Vertex>::iterator, std::multimap<float, Vertex>::iterator> range;
	std::pair<std::multimap<size_t, Vertex>::iterator, std::multimap<size_t, Vertex>::iterator> rangeL;

	for (Vertex w = 0; w < num_Vq; w++) {
		ExVertex.insert(std::pair<float, Vertex>(key[w] = (float)cs.GetCandidateSize(w) / pow(query.GetDegree(w), exp), w));
		ExVertexList.insert(std::pair<size_t, Vertex>(num_CS[w] = cs.GetCandidateSize(w), w));
	}

	bool call = true; //Store whether the function is examining new vertex or backtracking
	bool matched, ExCan_Exists;
	std::vector<size_t> stoppedNeighborOffset(num_Vq);

	do {
		if (call) {
			u = (it = ExVertex.begin())->second;
			ExVertex.erase(it);

			rangeL = ExVertexList.equal_range(num_CS[u]);
			for (itL = rangeL.first; itL != rangeL.second; itL++) if (itL->second == u) break;;
			ExVertexList.erase(itL);

			state.push(u);
			Vq_mapped[u] = true;

			posCS[u] = pivotCS[u];
		}
		else {
			u = state.top();
			VG_mapped[ExCan[u][posCS[u]].first] = false;
			embedding[u] = -1;

			for (size_t k = query.GetNeighborStartOffset(u); k < stoppedNeighborOffset[u]; k++) {
				un = query.GetNeighbor(k);

				if (!Vq_mapped[un]) {
					num_parent[un]--;

					size_t i = pivotCS[un];
					while (i > 0) {
						--i;
						if (ExCan[un][i].second != u) {
							i++;
							break;
						}

						ExCan[un][i].second = -1;
					}
					pivotCS[un] = i;

					range = ExVertex.equal_range(key[un]);
					for (it = range.first; it != range.second; it++) if (it->second == un) break;;
					tempEV = *it;
					ExVertex.erase(it);
					tempEV.first = key[un] = ((float)(cs.GetCandidateSize(un) - pivotCS[un]) / (num_parent[un] * pow(query.GetDegree(un), exp)));

					ExVertex.insert(tempEV);

					rangeL = ExVertexList.equal_range(num_CS[un]);
					for (itL = rangeL.first; itL != rangeL.second; itL++) if (itL->second == un) break;;
					tempEVL = *itL;
					ExVertexList.erase(itL);
					tempEVL.first = num_CS[un] = cs.GetCandidateSize(un) - pivotCS[un];

					ExVertexList.insert(tempEVL);
				}
			}
			posCS[u]++;
		}

		while ((posCS[u] < cs.GetCandidateSize(u)) && VG_mapped[v = ExCan[u][posCS[u]].first]) posCS[u]++;

		ExCan_Exists = (posCS[u] != cs.GetCandidateSize(u));
		if (ExCan_Exists) {
			rangeL = ExVertexList.equal_range(1);
			for (itL = rangeL.first; itL != rangeL.second; itL++) {
				if (ExCan[itL->second][pivotCS[itL->second]].first == v) {
					ExCan_Exists = false;
					break;
				}
			}
		}

		if (!ExCan_Exists) {
			ExVertex.insert(std::pair<float, Vertex>(key[u] = (float)(cs.GetCandidateSize(u) - pivotCS[u]) / (num_parent[u] * pow(query.GetDegree(u), exp)), u));
			ExVertexList.insert(std::pair<size_t, Vertex>(num_CS[u] = cs.GetCandidateSize(u) - pivotCS[u], u));

			state.pop();
			Vq_mapped[u] = false;

			call = false;
			continue;
		}

		matched = true;
		stoppedNeighborOffset[u] = query.GetNeighborEndOffset(u);
		for (size_t k = query.GetNeighborStartOffset(u); k < query.GetNeighborEndOffset(u); k++) {
			un = query.GetNeighbor(k);

			if (!Vq_mapped[un]) {
				num_parent[un]++;
				for (size_t i = pivotCS[un]; i < cs.GetCandidateSize(un); i++) {
					if (!data.IsNeighbor(v, ExCan[un][i].first)) {
						ExCan[un][i].second = u;

						tempCS = ExCan[un][i];
						ExCan[un][i] = ExCan[un][pivotCS[un]];
						ExCan[un][pivotCS[un]] = tempCS;

						pivotCS[un]++;
					}
				}

				if (pivotCS[un] == cs.GetCandidateSize(un)) {
					stoppedNeighborOffset[u] = k + 1;
					matched = false;
					break;
				}
				else {
					range = ExVertex.equal_range(key[un]);
					for (it = range.first; it != range.second; it++) if (it->second == un) break;;
					tempEV = *it;
					ExVertex.erase(it);
					tempEV.first = key[un] = (float)(cs.GetCandidateSize(un) - pivotCS[un]) / (num_parent[un] * pow(query.GetDegree(un), exp));

					ExVertex.insert(tempEV);

					rangeL = ExVertexList.equal_range(num_CS[un]);
					for (itL = rangeL.first; itL != rangeL.second; itL++) if (itL->second == un) break;;
					tempEVL = *itL;
					ExVertexList.erase(itL);
					tempEVL.first = num_CS[un] = cs.GetCandidateSize(un) - pivotCS[un];

					ExVertexList.insert(tempEVL);
				}
			}
		}

		if (!matched) {
			call = false;
			continue;
		}

		embedding[u] = v;
		VG_mapped[v] = true;

		if (state.size() == num_Vq) {
			std::cout << "a";
			for (size_t i = 0; i < num_Vq; i++) {
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