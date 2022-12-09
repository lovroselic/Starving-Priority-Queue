// https://www.codeabbey.com/index/task_view/starving-priority-queue
//

#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <chrono>
#include <cmath>
#include <cassert>
#include <limits>
#include <numeric>
#include <tuple>
#include <regex> 

#include "Console.h"
#include "CodeAbbey.h"
#include "LS_library.h"
#define VERSION "1.0"

using namespace std;

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

long long int LinearCongruentialGenerator(int A, int C, int M, long long int seed);

class Node {
public:
	long long starving;
	long long initialTime;

	Node(long long starving, long long initialTime) {
		this->starving = starving;
		this->initialTime = initialTime;
	}
};

class MaxBinaryHeap {
public:
	vector<Node*> HEAP;

	MaxBinaryHeap() {
		HEAP = {};
	};

	int size() {
		return HEAP.size();
	}

	int parent(int i) {
		return (i - 1) / 2;
	}

	int leftChild(int i) {
		return 2 * i + 1;
	}

	int rightChild(int i) {
		return 2 * i + 2;
	}

	void siftUp(int i) {
		while (i > 0 && this->HEAP[this->parent(i)]->starving < this->HEAP[i]->starving) {
			swap(this->HEAP[this->parent(i)], this->HEAP[i]);
			i = this->parent(i);
		}
	}

	void siftDown(int i) {
		int maxIndex = i;
		int L = this->leftChild(i);
		if (L < this->size() && this->HEAP[L]->starving > this->HEAP[maxIndex]->starving) {
			maxIndex = L;
		}
		int R = this->rightChild(i);
		if (R < this->size() && this->HEAP[R]->starving > this->HEAP[maxIndex]->starving) {
			maxIndex = R;
		}
		if (i != maxIndex) {
			swap(this->HEAP[i], this->HEAP[maxIndex]);
			this->siftDown(maxIndex);
		}
	}

	void insert(Node* node) {
		this->HEAP.push_back(node);
		this->siftUp(this->size() - 1);
	}
	Node* extractMax() {
		if (this->size() == 0) return nullptr;
		Node* result = this->HEAP.at(0);
		this->HEAP.at(0) = this->HEAP[this->size() - 1];
		this->HEAP.pop_back();
		this->siftDown(0);
		return result;
	}

	void display() {
		for (auto node : HEAP) {
			cout << " " << node->starving;
		}
		cout << endl;
	}
};

int main() {
	auto t1 = high_resolution_clock::now();
	cout << "Starving Priority v" << VERSION << "!\n\n";
	//string path = "Test.txt";
	string path = "Try.txt";
	vector<string> raw_data = loadData(path);
	//printVector(raw_data);
	/***********************/

	const int A = 445;
	const int C = 700001;
	const int M = 2097152;
	//const int maxStarvation = 9;
	const int maxStarvation = 999;

	vector<int> data = splitToInt(raw_data[0], " ");
	//printVector(data);
	int N = data.at(0);
	long long int X = data.at(1);

	MaxBinaryHeap BH = MaxBinaryHeap();
	long long int sumStarvation = 0;

	for (long long int time = 0; time <= 2 * N; time++) {
		X = LinearCongruentialGenerator(A, C, M, X);
		int starvationDegree = (X % maxStarvation) + 1;
		if (time > 0 && time % 2 == 0) {
			Node* feed = BH.extractMax();
			long long int discomfort = feed->starving * (time - feed->initialTime);
			sumStarvation += discomfort;
		}
		if (time < N) {
			Node* person = new Node(starvationDegree, time);
			BH.insert(person);
		}
	}

	cout << "\nSolution: " << sumStarvation << endl;

	/***********************/
	auto t2 = high_resolution_clock::now();
	auto ms_int = duration_cast<milliseconds>(t2 - t1);
	cout << "Execution time: " << ms_int.count() << " ms" << endl;
}

long long int LinearCongruentialGenerator(int A, int C, int M, long long int seed) {
	return ((static_cast<long long>(A) * seed) + C) % M;
}
