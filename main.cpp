#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <random>

using namespace std;

void generateRandomIntArray(int arr[], int N) {

	for (int i = 0; i < N; i++) {
		arr[i] = i;
	}
	random_shuffle(&arr[0], &arr[N]);
}

void generateRandomStringArray(string arr[], istream& stream, int N) {

	int counter = 0;
	string line;
	while (getline(stream, line) && counter < N) {
		arr[counter] = line;
		counter++;
	}
	random_shuffle(&arr[0], &arr[N]);
}

template<typename T>
void generateInputOutputFile(T arr[], ostream& stream, int N) {

	for (int i = 0; i < N; i++) {
		stream << arr[i] << endl;
	}
}

template<typename T>
bool check_if_sorted(T arr[], int N) {

	int i = 1;
	bool is_sorted = true;
	while ((i < N) && is_sorted) {
		if (arr[i - 1] > arr[i])
		{
			is_sorted = false;
			break;
		}
		i++;
	}
	return is_sorted;
}

template<typename T>
void printArray(T arr[], int N) {

	for (int i = 0; i < N; i++) {
		cout << " " << arr[i];
	}
	cout << endl;
}

template <typename T>
void bubbleSort(T arr[], int N) {
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N - i - 1; ++j) {
			if (arr[j] > arr[j + 1]) {
				//swap elements
				T temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
}

template<typename T>
void shellSort(T arr[], int n) {

	for (int interval = n / 2; interval > 0; interval /= 2) {
		for (int i = interval; i < n; i ++) {
			T temp = arr[i];
			int j;
			for (j = i; j >= interval && arr[j - interval] > temp; j -= interval) {
				arr[j] = arr[j - interval];
			}
			arr[j] = temp;
		}
	}
}

template<typename T>
void shellSortTwo(T arr[], int n) {

	int expDerived[8] = { 1, 4, 10, 23, 57, 132, 301, 701 };

	for (int k = 7; k >= 0 ; k--) {
		for (int i = expDerived[k]; i < n; i++) {
			T temp = arr[i];
			int j;
			for (j = i; j >= expDerived[k] && arr[j - expDerived[k]] > temp; j -= expDerived[k]) {
				arr[j] = arr[j - expDerived[k]];
			}
			arr[j] = temp;
		}
	}
}

template<typename T>
void quickSort(T arr[], int low, int high, int pivotMode) {

	int i = low, j = high;
	T pivot = arr[low];
	// First element
	if (pivotMode == 0) {
		pivot = arr[low];
	}
	// Random element
	else if (pivotMode == 1) {
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<>distr(i, j);
		int randomIndex = distr(gen);
		pivot = arr[randomIndex];
	}
	// Median of three
	else {
		int mid = (i + j) / 2;
		T smallArr[3] = { arr[i], arr[mid], arr[j] };
		bubbleSort<T>(smallArr, 3);
		pivot = smallArr[1];
	}
	do {
		while (arr[i] < pivot) {
			i++;
		}
		while (arr[j] > pivot) {
			j--;
		}
		if (i <= j) {
			T temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
			i++;
			j--;
		}
	} while (i <= j);

	if (low < j)
		quickSort(arr, low, j, pivotMode);
	if (high > i)
		quickSort(arr, i, high, pivotMode);
}

template<typename T>
void insertionSort(T arr[], int low, int N) {

	for (int i = low + 1; i <= N; i++) {
		T key = arr[i];
		int j = i;
		while (key < arr[j - 1] && j > low) {
			arr[j] = arr[j - 1];
			--j;
		}
		arr[j] = key;
	}
}

template <typename T>
int partition(T arr[], int low, int high) {

	T pivot = arr[high];
	int pivotIndex = low;
	for (int i = low; i < high; i++) {
		if (arr[i] < pivot) {
			T temp = arr[i];
			arr[i] = arr[pivotIndex];
			arr[pivotIndex] = temp;
			pivotIndex++;
		}
	}
	T temp = arr[pivotIndex];
	arr[pivotIndex] = arr[high];
	arr[high] = temp;

	return pivotIndex;
}

template <typename T>
void hybridQuickSort(T arr[], int low, int high) {

	while (low < high) {
		// Perform Insertion Sort on small subarrays, max size = 10
		if (high - low + 1 < 10000) {
			insertionSort<T>(arr, low, high);
			break;
		}
		else {
			int pivot = partition<T>(arr, low, high);
			if (pivot - low < high - pivot) {
				hybridQuickSort<T>(arr, low, pivot - 1);
				low = pivot + 1;
			}
			else { 
				hybridQuickSort<T>(arr, pivot + 1, high);
				high = pivot - 1;
			}
		}
	}
}

int main() {

	// generate sample data
	const int dataSize = 512000;
	int* data1 = new int[dataSize];
	string* data2 = new string[dataSize];
	generateRandomIntArray(data1, dataSize);

	ifstream file("english.txt"); // English Dictionary
	if (file) {
		generateRandomStringArray(data2, file, dataSize);
	}
	file.close();

	ofstream inputFile("input.txt");
	if (inputFile) {
		generateInputOutputFile<string>(data2, inputFile, dataSize);
	}
	inputFile.close();

	// measure execution time
	chrono::time_point<chrono::system_clock> start, end;
	start = chrono::system_clock::now();

	if(!check_if_sorted<string>(data2, dataSize))
		hybridQuickSort<string>(data2, 0, dataSize - 1);

	end = chrono::system_clock::now();
	chrono::duration<double> executionTime = end - start;
	cout << "Execution time: " << executionTime.count() << "s" << endl;
	if (check_if_sorted<string>(data2, dataSize))
		cout << "Sorted successfully!" << endl;

	ofstream outputFile("output.txt");
	if (outputFile) {
		generateInputOutputFile<string>(data2, outputFile, dataSize);
	}
	outputFile.close();

	cout << "\nSuccessfully saved data in the .txt file" << endl;

	delete[] data1;
	delete[] data2;

	return 0;
}