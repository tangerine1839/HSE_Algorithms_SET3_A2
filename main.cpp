#include <algorithm>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

void hybridMergeSort(std::vector<int>& array, int left, int right, int threshold);
void mergeSort(std::vector<int>& arr, int left, int right);


class ArrayGenerator
{
private:
    std::vector<int> array;
    int maxLength = 100000;
    int minRange = 0;
    int maxRange = 6000;

    void generateArray()
    {
        std::mt19937 gen(static_cast<unsigned>(std::time(nullptr)));
        std::uniform_int_distribution<int> dist(minRange, maxRange);
        for (int i = 0; i < maxLength; ++i)
        {
            array.push_back(dist(gen));
        }
    }
public:
    ArrayGenerator()
    {
        generateArray();
    }

    std::vector<int> getRandomArray(int size)
    {
        return std::vector<int> (array.begin(), array.begin() + size);
    }

    std::vector<int> getReversedArray(int size)
    {
        std::vector<int> arr(array.begin(), array.begin() + size);
        std::sort(arr.rbegin(), arr.rend());
        return arr;
    }

    std::vector<int> getAlmostSortedArray(int size, int swaps)
    {
        std::vector<int> arr(array.begin(), array.begin() + size);
        std::sort(arr.begin(), arr.end());

        std::mt19937 gen(static_cast<unsigned>(std::time(nullptr)));
        std::uniform_int_distribution<int> dist(0, size - 1);

        for (int i = 0; i < swaps; ++i) {
            std::swap(arr[dist(gen)], arr[dist(gen)]);
        }

        return arr;
    }
};

class SortTester
{
public:
    double measureTimeOfMergeSort(std::vector<int>& array, int size)
    {
        int trials = 5;
        long long totalTime = 0;

        for (int i = 0; i < trials; ++i) {
            std::vector<int> copy = array;
            auto start = std::chrono::high_resolution_clock::now();
            mergeSort(copy, 0, copy.size() - 1);
            auto elapsed = std::chrono::high_resolution_clock::now() - start;
            long long msec = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
            totalTime += std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
        }

        return static_cast<double> (totalTime) / trials;
    }

    double measureTimeOfHybridMergeSort(std::vector<int>& array, int size, int threshold)
    {
        const int trials = 5;
        long long totalTime = 0;

        for (int i = 0; i < trials; ++i)
        {
            std::vector<int> copy = array;
            auto start = std::chrono::high_resolution_clock::now();
            hybridMergeSort(copy, 0, copy.size() - 1, threshold);
            auto elapsed = std::chrono::high_resolution_clock::now() - start;
            long long msec = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
            totalTime += std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
        }

        return static_cast<double> (totalTime) / trials;
    }

};
int main()
{
    ArrayGenerator generator;
    SortTester tester;

    std::string fileName1 = "merge_sort_random_array.txt";
    std::string fileName2 = "merge_sort_reversed_array.txt";
    std::string fileName3 = "merge_sort_almost_sorted_array.txt";

    std::string fileName4 = "hybrid_sort_random_array.txt";
    std::string fileName5 = "hybrid_sort_reversed_array.txt";
    std::string fileName6 = "hybrid_sort_almost_sorted_array.txt";

    std::ofstream outFile1(fileName1);
    std::ofstream outFile2(fileName2);
    std::ofstream outFile3(fileName3);

    std::ofstream outFile4(fileName4);
    std::ofstream outFile5(fileName5);
    std::ofstream outFile6(fileName6);

    for (int size = 500; size <= 100000; size += 100)
    {
        std::vector<int> randomArray = generator.getRandomArray(size);
        std::vector<int> reversedArray = generator.getReversedArray(size);
        std::vector<int> almostSortedArray = generator.getAlmostSortedArray(size, size / 100);

        double randomTime = tester.measureTimeOfMergeSort(randomArray, size);
        double reversedTime = tester.measureTimeOfMergeSort(reversedArray, size);
        double nearlySortedTime = tester.measureTimeOfMergeSort(almostSortedArray, size);

        outFile1 << size << ' ' << randomTime << std::endl;
        outFile2 << size << ' ' << reversedTime << std::endl;
        outFile3 << size << ' ' << nearlySortedTime << std::endl;

        std::vector<int> threshold{5, 10, 20, 30, 50};

        outFile4 << size;
        outFile5 << size;
        outFile6 << size;
        for (int i = 0; i < threshold.size(); ++i)
        {
            randomTime = tester.measureTimeOfHybridMergeSort(randomArray, size, threshold[i]);
            reversedTime = tester.measureTimeOfHybridMergeSort(reversedArray, size, threshold[i]);
            nearlySortedTime = tester.measureTimeOfHybridMergeSort(almostSortedArray, size, threshold[i]);

            outFile4 << ' ' << randomTime;
            outFile5 << ' ' << reversedTime;
            outFile6 << ' ' << nearlySortedTime;
        }
        outFile4 << std::endl;
        outFile5 << std::endl;
        outFile6 << std::endl;
    }
    outFile1.flush();
    outFile2.flush();
    outFile3.flush();
    outFile4.flush();
    outFile5.flush();
    outFile6.flush();

    outFile1.close();
    outFile2.close();
    outFile3.close();
    outFile4.close();
    outFile5.close();
    outFile6.close();

    return 0;
}

void insertionSort(std::vector<int>& arr, int left, int right)
{
    for (int i = left + 1; i <= right; ++i)
    {
        int j = i - 1;
        while (arr[i] < arr[j] && j >= left)
        {
            std::swap(arr[i], arr[j]);
            j--;
            i--;
        }
    }
}
void merge(std::vector<int>& arr, int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<int> left_arr(n1);
    std::vector<int> right_arr(n2);

    for (int i = 0; i < n1; i++)
    {
        left_arr[i] = arr[left + i];
    }
    for (int i = 0; i < n2; i++)
    {
        right_arr[i] = arr[mid + 1 + i];

    }

    int i = 0;
    int j = 0;
    int k = left;

    while (i < n1 && j < n2)
    {
        if (left_arr[i] <= right_arr[j])
        {
            arr[k] = left_arr[i];
            i++;
        }
        else
        {
            arr[k] = right_arr[j];
            j++;
        }
        k++;
    }
    while (i < n1)
    {
        arr[k] = left_arr[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = right_arr[j];
        j++;
        k++;
    }
}

void hybridMergeSort(std::vector<int>& arr, int left, int right, int threshold)
{
    if (right - left + 1 <= threshold)
    {
        insertionSort(arr, left, right);
        return;
    }

    if (left < right)
    {
        int mid = left + (right - left) / 2;

        hybridMergeSort(arr, left, mid, threshold);
        hybridMergeSort(arr, mid + 1, right, threshold);
        merge(arr, left, mid, right);
    }
}

void mergeSort(std::vector<int>& arr, int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}
