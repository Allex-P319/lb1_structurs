#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <iomanip>

using namespace std;

void fill_random(vector<int>& arr, int n) {
    arr.resize(n);
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000;
    }
}

void fill_sorted(vector<int>& arr, int n) {
    fill_random(arr, n);
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// ==================== ФУНКЦИИ ====================

// 1. O(1) - Доступ к элементу (не изменяет массив)
int f01_access_middle(const vector<int>& arr) {
    return arr[arr.size() / 2];
}

// 2. O(log n) - Lower bound
int f04_lower_bound(const vector<int>& arr, int key) {
    int lo = 0, hi = arr.size();
    while (lo < hi) {
        int m = lo + (hi - lo) / 2;
        if (arr[m] < key) lo = m + 1;
        else hi = m;
    }
    return lo;
}

// 3. O(n) - Сумма элементов
long long f09_sum(const vector<int>& arr) {
    long long s = 0;
    for (size_t i = 0; i < arr.size(); i++) {
        s += arr[i];
    }
    return s;
}

// 4. O(n log n) - Пирамидальная сортировка
static void hs_sift(vector<int>& a, int n, int i) {
    for (;;) {
        int big = i, l = 2 * i + 1, r = 2 * i + 2;
        if (l < n && a[l] > a[big]) big = l;
        if (r < n && a[r] > a[big]) big = r;
        if (big == i) break;
        swap(a[i], a[big]);
        i = big;
    }
}

void f16_heap_sort(vector<int>& arr) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; i--)
        hs_sift(arr, n, i);
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        hs_sift(arr, i, 0);
    }
}

// 5. O(n²) - Сортировка вставками
void f20_insertion_sort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// ==================== ИЗМЕРЕНИЯ ====================

double get_time() {
    return clock() / (double)CLOCKS_PER_SEC;
}

void save_to_csv(const string& filename, const string& algorithm, int size, double time) {
    ofstream file(filename, ios::app);
    if (file.is_open()) {
        file << algorithm << "," << size << "," << fixed << setprecision(6) << time << "\n";
        file.close();
    }
}

void measure_constant_time() {
    cout << "\nO(1) - ДОСТУП К ЭЛЕМЕНТУ (f01_access_middle)\n";
    cout << "Размер\tВремя (1 млн вызовов)\n";
    cout << "----------------------------\n";

    int sizes[] = {1000, 10000, 100000, 1000000};

    for (int n : sizes) {
        vector<int> arr;
        fill_random(arr, n);

        double start = get_time();
        volatile int result; // чтобы компилятор не выбросил вызов
        for (int i = 0; i < 1000000; i++) {
            result = f01_access_middle(arr);
        }
        double end = get_time();

        cout << n << "\t" << (end - start) << " с\n";
        save_to_csv("results.csv", "O(1)_access_middle", n, (end - start));
    }
}

void measure_log_time() {
    cout << "\nO(log n) - LOWER BOUND (f04_lower_bound)\n";
    cout << "Размер\tВремя (10 тыс. вызовов)\n";
    cout << "----------------------------\n";

    int sizes[] = {1000, 2000, 3000, 4000, 5000};

    for (int n : sizes) {
        vector<int> arr;
        fill_sorted(arr, n);
        int key = arr[n / 2];

        double start = get_time();
        for (int i = 0; i < 10000; i++) {
            f04_lower_bound(arr, key);
        }
        double end = get_time();

        cout << n << "\t" << (end - start) << " с\n";
        save_to_csv("results.csv", "O(log_n)_lower_bound", n, (end - start));
    }
}

void measure_linear_time() {
    cout << "\nO(n) - СУММА ЭЛЕМЕНТОВ (f09_sum)\n";
    cout << "Размер\tВремя\n";
    cout << "----------------------------\n";

    int sizes[] = {1000, 10000, 100000, 500000, 1000000, 2000000};

    for (int n : sizes) {
        vector<int> arr;
        fill_random(arr, n);

        double start = get_time();
        long long result = f09_sum(arr);
        double end = get_time();

        cout << n << "\t" << (end - start) << " с\n";
        save_to_csv("results.csv", "O(n)_sum", n, (end - start));
    }
}

void measure_nlogn_time() {
    cout << "\nO(n log n) - ПИРАМИДАЛЬНАЯ СОРТИРОВКА (f16_heap_sort)\n";
    cout << "Размер\tВремя\n";
    cout << "----------------------------\n";

    int sizes[] = {1000, 5000, 10000, 50000, 100000, 200000, 500000};

    for (int n : sizes) {
        vector<int> original;
        fill_random(original, n);

        double start = get_time();
        vector<int> arr = original;
        f16_heap_sort(arr);
        double end = get_time();

        cout << n << "\t" << (end - start) << " с\n";
        save_to_csv("results.csv", "O(n_log_n)_heapsort", n, (end - start));
    }
}

void measure_quadratic_time() {
    cout << "\nO(n²) - СОРТИРОВКА ВСТАВКАМИ (f20_insertion_sort)\n";
    cout << "Размер\tВремя\n";
    cout << "----------------------------\n";

    int sizes[] = {100, 500, 1000, 2000, 3000, 5000, 7000, 10000};

    for (int n : sizes) {
        vector<int> original;
        fill_random(original, n);

        double start = get_time();
        vector<int> arr = original;
        f20_insertion_sort(arr);
        double end = get_time();

        cout << n << "\t" << (end - start) << " с\n";
        save_to_csv("results.csv", "O(n^2)_insertion_sort", n, (end - start));
    }
}

// ==================== ГЛАВНАЯ ====================

int main() {
    srand(time(0));

    ofstream file("results.csv");
    if (file.is_open()) {
        file << "Algorithm,Size,Time\n";
        file.close();
    }

    cout << "ИЗМЕРЕНИЕ ВРЕМЕНИ ВЫПОЛНЕНИЯ\n";
    cout << "============================\n";

    measure_constant_time();
    measure_log_time();
    measure_linear_time();
    measure_nlogn_time();
    measure_quadratic_time();

    cout << "\nВсе тесты завершены!\n";
    cout << "CSV файл 'results.csv' создан.\n";

    return 0;
}