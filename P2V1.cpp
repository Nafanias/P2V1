#include <iostream>
#include <vector>
#include <limits> 
#include <string> 
#include <iomanip>
#include <Windows.h>
#include <cfloat> 

// Функция для определения ранга матрицы
int findRank(std::vector<std::vector<double>>& matrix) {
    int rank = 0; // Инициализация переменной rank для хранения ранга матрицы
    int rows = static_cast<int>(matrix.size()); // Получение количества строк в матрице с помощью static_cast
    int cols = static_cast<int>(matrix[0].size()); // Получение количества столбцов в матрице с помощью static_cast

    for (int col = 0; col < cols; col++) { // Проход по каждому столбцу матрицы
        for (int row = rank; row < rows; row++) {
            if (matrix[row][col] != 0) { // Поиск ненулевого элемента в столбце начиная с текущей строки ранга
                std::swap(matrix[row], matrix[rank]); // Перестановка строк, чтобы ненулевой элемент стал ведущим элементом
                for (int i = rank + 1; i < rows; i++) {
                    double factor = matrix[i][col] / matrix[rank][col]; // Вычисление коэффициента для обнуления элемента
                    for (int j = col; j < cols; j++) {
                        matrix[i][j] -= factor * matrix[rank][j]; // Преобразование строки для обнуления элемента
                    }
                }
                rank++; // Увеличение ранга после обработки строки
                break; // Переход к следующему столбцу
            }
        }
    }

    return rank; // Возвращение ранга матрицы
}

// Функция для транспонирования матрицы
std::vector<std::vector<double>> transposeMatrix(const std::vector<std::vector<double>>& matrix) {
    int rows = static_cast<int>(matrix.size()); // Получение количества строк в матрице с помощью static_cast
    int cols = static_cast<int>(matrix[0].size()); // Получение количества столбцов в матрице с помощью static_cast

    // Создание новой матрицы для результата транспонирования
    std::vector<std::vector<double>> transposed(cols, std::vector<double>(rows, 0.0));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            transposed[j][i] = matrix[i][j]; // Запись элементов транспонированной матрицы
        }
    }

    return transposed;
}

// Функция проверяет, является ли матрица квадратной (одинаковое количество строк и столбцов)
bool isSquareMatrix(const std::vector<std::vector<double>>& matrix) {
    return matrix.size() == 0 ? false : matrix.size() == matrix[0].size();
}

// Функция для вычисления следа матрицы (суммы элементов на главной диагонали матрицы)
double trace(const std::vector<std::vector<double>>& matrix) {
    double sum = 0.0;
    for (size_t i = 0; i < matrix.size(); i++) {
        // Проверяем, что индекс не превышает размерность матрицы
        if (i >= matrix[i].size()) {
            throw std::out_of_range("Индекс главной диагонали превышает размер матрицы");
        }
        sum += matrix[i][i];
    }

    if (sum > DBL_MAX) {
        throw std::overflow_error("Сумма элементов на главной диагонали больше максимального значения диапазона double");
    }

    return sum;
}

// Функция для нахождения определителя матрицы
double determinant(const std::vector<std::vector<double>>& matrix) {
    int n = static_cast<int>(matrix.size());

    // Проверка на квадратную матрицу и размерность больше 1
    if (!isSquareMatrix(matrix) || n < 2) {
        return 0.0;
    }

    // Рекурсивная функция для вычисления определителя
    if (n == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }
    else {
        double det = 0;
        int sign = 1;

        for (int i = 0; i < n; ++i) {
            std::vector<std::vector<double>> submatrix;
            for (int j = 1; j < n; ++j) {
                std::vector<double> row;
                for (int k = 0; k < n; ++k) {
                    if (k != i) {
                        row.push_back(matrix[j][k]);
                    }
                }
                submatrix.push_back(row);
            }
            det += sign * matrix[0][i] * determinant(submatrix);
            sign = -sign;
        }

        return det;
    }
}

// Функция для вычисления обратной матрицы
std::vector<std::vector<double>> inverseMatrix(const std::vector<std::vector<double>>& matrix) {
    int n = static_cast<int>(matrix.size());

    // Создание матрицы для хранения обратной матрицы
    std::vector<std::vector<double>> inverse(n, std::vector<double>(n));

    // Вычисление определителя исходной матрицы
    double det = determinant(matrix);

    // Проверка на вырожденность матрицы
    if (det == 0) {
        std::cout << "Матрица вырожденная, обратной матрицы не существует." << std::endl;
        return inverse;
    }

    if (n == 2) {
        double invDet = 1.0 / det;
        inverse[0][0] = matrix[1][1] * invDet;
        inverse[0][1] = -matrix[0][1] * invDet;
        inverse[1][0] = -matrix[1][0] * invDet;
        inverse[1][1] = matrix[0][0] * invDet;
    }
    else {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                // Получение минора для текущего элемента
                std::vector<std::vector<double>> minorMatrix;
                for (int k = 0; k < n; ++k) {
                    if (k == i) continue;
                    std::vector<double> row;
                    for (int l = 0; l < n; ++l) {
                        if (l == j) continue;
                        row.push_back(matrix[k][l]);
                    }
                    minorMatrix.push_back(row);
                }

                // Вычисление подматрицы и определения знака
                double minorDet = determinant(minorMatrix);

                double sign = ((i + j) % 2 == 0) ? 1 : -1;
                inverse[j][i] = sign * minorDet / det;
            }
        }
    }

    return inverse;
}

// Функция для проверки на равенство двух матриц (2 матрицы равны по размерам и значениям внутри них)
bool areMatricesEqual(const std::vector<std::vector<double>>& matrix1, const std::vector<std::vector<double>>& matrix2) {
    if (matrix1.size() != matrix2.size() || matrix1[0].size() != matrix2[0].size()) {
        return false; // Матрицы разных размеров
    }

    for (int i = 0; i < matrix1.size(); i++) {
        for (int j = 0; j < matrix1[0].size(); j++) {
            if (matrix1[i][j] != matrix2[i][j]) {
                return false; // Найдены различающиеся элементы
            }
        }
    }

    return true; // Матрицы равны
}

// Функция сложения двух матриц
std::vector<std::vector<double>> addMatrices(const std::vector<std::vector<double>>& matrix1, const std::vector<std::vector<double>>& matrix2) {
    int rows = static_cast<int>(matrix1.size()); // Получение количества строк в матрице с помощью static_cast
    int cols = static_cast<int>(matrix1[0].size()); // Получение количества столбцов в матрице с помощью static_cast

    // Проверка, что обе матрицы имеют одинаковое количество строк и столбцов
    if (matrix1.size() != matrix2.size() || matrix1[0].size() != matrix2[0].size()) {
        std::cerr << "Матрицы должны иметь одинаковое количество строк и столбцов." << std::endl;
        return std::vector<std::vector<double>>(); // Возвращаем пустую матрицу
    }

    std::vector<std::vector<double>> result(rows, std::vector<double>(cols, 0.0));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((std::numeric_limits<double>::max)() - matrix1[i][j] < matrix2[i][j]) {
                throw std::overflow_error("Переполнение при сложении элементов матриц");
            }
            result[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }

    return result;
}

// Функция умножения двух матриц
std::vector<std::vector<double>> multiplyMatrices(const std::vector<std::vector<double>>& matrix1, const std::vector<std::vector<double>>& matrix2) {
    // Получаем количество строк и столбцов в матрицах
    int rows1 = static_cast<int>(matrix1.size());
    int cols1 = static_cast<int>(matrix1[0].size());
    int cols2 = static_cast<int>(matrix2[0].size());

    // Проверка, что количество столбцов в первой матрице равно количеству строк во второй матрице
    if (cols1 != matrix2.size()) {
        std::cerr << "Количество столбцов в первой матрице должно быть равно количеству строк во второй матрице." << std::endl;
        return std::vector<std::vector<double>>();
    }

    // Проверка, что количество элементов в каждой строке матрицы одинаковое
    for (const auto& row : matrix1) {
        if (row.size() != cols1) {
            std::cerr << "Неравное количество элементов в строках первой матрицы." << std::endl;
            return std::vector<std::vector<double>>();
        }
    }

    for (const auto& row : matrix2) {
        if (row.size() != cols2) {
            std::cerr << "Неравное количество элементов в строках второй матрицы." << std::endl;
            return std::vector<std::vector<double>>();
        }
    }

    // Создаем результирующую матрицу с нулевыми значениями
    std::vector<std::vector<double>> result(rows1, std::vector<double>(cols2, 0.0));

    // Умножение матриц
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            for (int k = 0; k < cols1; k++) {
                if (result[i][j] + matrix1[i][k] * matrix2[k][j] < result[i][j]) {
                    throw std::overflow_error("Переполнение при умножении матриц.");
                }
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    return result;
}

// Главная функция
int main() {
    SetConsoleCP(1251); // Установка кодовой страницы win-cp 1251 в поток ввода
    SetConsoleOutputCP(1251); // Установка кодовой страницы win-cp 1251 в поток вывода

    int m, n, m2, n2;
    std::string input;

    bool isValidInputM = false;
    // Ввод размерностей матриц 1 (M)
    while (!isValidInputM) {
        std::string input;

        std::cout << "Введите количество строк M матрицы 1: ";
        std::cin >> input;

        try {
            size_t pos = 0;
            m = std::stoi(input, &pos); // Преобразование строки в int
            if (pos < input.size()) {
                throw std::invalid_argument("Неверный формат числа");
            }
            if (m <= 0) {
                throw std::out_of_range("Число должно быть положительным");
            }
            isValidInputM = true;
        }
        catch (const std::invalid_argument& e) {
            std::cout << "Пожалуйста, введите целое число.\n";
        }
        catch (const std::out_of_range& e) {
            std::cout << "Число должно быть неотрицательным и в допустимом диапазоне.\n";
        }

        if (!isValidInputM) {
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
        }
    }

    bool isValidInputN = false;
    // Ввод размерностей матриц 1 (N)
    while (!isValidInputN) {
        std::string input;

        std::cout << "Введите количество столбцов M матрицы 1: ";
        std::cin >> input;

        try {
            size_t pos = 0;
            n = std::stoi(input, &pos); // Преобразование строки в int
            if (pos < input.size()) {
                throw std::invalid_argument("Неверный формат числа");
            }
            if (n <= 0) {
                throw std::out_of_range("Число должно быть положительным");
            }
            isValidInputN = true;
        }
        catch (const std::invalid_argument& e) {
            std::cout << "Пожалуйста, введите целое число.\n";
        }
        catch (const std::out_of_range& e) {
            std::cout << "Число должно быть неотрицательным и в допустимом диапазоне.\n";
        }

        if (!isValidInputN) {
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
        }
    }

    // Ввод элементов для первой матрицы
    std::vector<std::vector<double>> matrix1(m, std::vector<double>(n));
    std::vector<std::vector<double>> originalMatrix1(m, std::vector<double>(n)); // Новая переменная для хранения исходной матрицы 1
    std::vector<std::vector<double>> taskThree(m, std::vector<double>(n)); // Перемменая для выполнения 3 Задания
    std::vector<std::vector<double>> taskFour(m, std::vector<double>(n)); // Перемменая для выполнения 3 Задания
    std::cout << "Введите элементы для первой матрицы 1:\n";
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << "Введите значение элемента для первой матрицы [" << i << "][" << j << "]: ";
            double num;

            try {
                std::cin >> num;
                if (std::cin.fail()) {
                    throw std::invalid_argument("Неверное значение введено");
                } else if (num <= 0) {
                    throw std::invalid_argument("Число должно быть больше 0");
                }
                matrix1[i][j] = num;
                originalMatrix1[i][j] = matrix1[i][j];
                taskThree[i][j] = matrix1[i][j];
                taskFour[i][j] = matrix1[i][j];
            }
            catch (const std::invalid_argument& e) {
                std::cin.clear();
                std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
                std::cerr << "Ошибка: " << e.what() << ", повторите ввод.\n";
                j--; // Повторный ввод для текущего элемента
            }
        }
    }

    bool isValidInputM2 = false;
    // Ввод размерностей матриц 2 (M)
    while (!isValidInputM2) {
        std::string input;

        std::cout << "Введите количество столбцов M матрицы 2: ";
        std::cin >> input;

        try {
            size_t pos = 0;
            m2 = std::stoi(input, &pos); // Преобразование строки в int
            if (pos < input.size()) {
                throw std::invalid_argument("Неверный формат числа");
            }
            if (m2 <= 0) {
                throw std::out_of_range("Число должно быть положительным");
            }
            isValidInputM2 = true;
        }
        catch (const std::invalid_argument& e) {
            std::cout << "Пожалуйста, введите целое число.\n";
        }
        catch (const std::out_of_range& e) {
            std::cout << "Число должно быть неотрицательным и в допустимом диапазоне.\n";
        }

        if (!isValidInputM2) {
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
        }
    }

    bool isValidInputN2 = false;
    // Ввод размерностей матриц 2 (N)
    while (!isValidInputN2) {
        std::string input;

        std::cout << "Введите количество столбцов M матрицы 2: ";
        std::cin >> input;

        try {
            size_t pos = 0;
            n2 = std::stoi(input, &pos); // Преобразование строки в int
            if (pos < input.size()) {
                throw std::invalid_argument("Неверный формат числа");
            }
            if (n2 <= 0) {
                throw std::out_of_range("Число должно быть положительным");
            }
            isValidInputN2 = true;
        }
        catch (const std::invalid_argument& e) {
            std::cout << "Пожалуйста, введите целое число.\n";
        }
        catch (const std::out_of_range& e) {
            std::cout << "Число должно быть неотрицательным и в допустимом диапазоне.\n";
        }

        if (!isValidInputN2) {
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
        }
    }

    // Ввод элементов для второй матрицы
    std::vector<std::vector<double>> matrix2(m2, std::vector<double>(n2));
    std::vector<std::vector<double>> originalMatrix2(m2, std::vector<double>(n2)); // Новая переменная для хранения исходной матрицы 2 (выполнение 2 задания)
    std::vector<std::vector<double>> taskThree2(m2, std::vector<double>(n2)); // Перемменая для выполнения 3 Задания
    std::vector<std::vector<double>> taskFour2(m2, std::vector<double>(n2)); // Перемменая для выполнения 4 Задания
    std::cout << "Введите элементы для первой матрицы 2:\n";
    for (int i = 0; i < m2; i++) {
        for (int j = 0; j < n2; j++) {
            std::cout << "Введите значение элемента для второй матрицы [" << i << "][" << j << "]: ";
            double num;

            try {
                std::cin >> num;
                if (std::cin.fail()) {
                    throw std::invalid_argument("Неверное значение введено");
                } else if (num <= 0) {
                    throw std::invalid_argument("Число должно быть больше 0");
                }
                matrix2[i][j] = num;
                originalMatrix2[i][j] = matrix2[i][j];
                taskThree2[i][j] = matrix2[i][j];
                taskFour2[i][j] = matrix2[i][j];
            }
            catch (const std::invalid_argument& e) {
                std::cin.clear();
                std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
                std::cerr << "Ошибка: " << e.what() << ", повторите ввод.\n";
                j--; // Повторный ввод для текущего элемента
            }
        }
    }

    // Нахождение рангов матриц
    int rank1 = findRank(matrix1);
    int rank2 = findRank(matrix2);

    // Вывод рангов
    std::cout << std::endl;
    std::cout << "Ранг первой матрицы: " << rank1 << std::endl;
    std::cout << "Ранг второй матрицы: " << rank2 << std::endl;

    // Вызов функции для транспонирования матрицы 1
    std::vector<std::vector<double>> transposedMatrix = transposeMatrix(originalMatrix1);

    // Вывод транспонированной матрицы 1
    std::cout << "\nТранспонированная первая матрица:" << std::endl;
    for (const auto& row : transposedMatrix) {
        for (double elem : row) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // Вызов функции для транспонирования матрицы 2
    std::vector<std::vector<double>> transposedMatrix2 = transposeMatrix(originalMatrix2);

    // Вывод транспонированной матрицы 2
    std::cout << "\nТранспонированная вторая матрица:" << std::endl;
    for (const auto& row : transposedMatrix2) {
        for (double elem : row) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // Проверка являются ли данные матрицы квадратными. В случае квадратных матриц найти их определители
    //  (determinant) и следы (trace), а также если возможно вычислить обратные матрицы.
    if (isSquareMatrix(taskThree) && taskThree.size() > 1) {
        double det = determinant(taskThree);
        double tr = 0.0;
        try {
            tr = trace(taskThree);
        }
        catch (const std::out_of_range& e) {
            std::cout << "Ошибка: " << e.what() << std::endl;
        }
        catch (const std::overflow_error& e) {
            std::cout << "Ошибка: " << e.what() << std::endl;
        }
        std::vector<std::vector<double>> invMatrix = inverseMatrix(taskThree);

        std::cout << "Первая матрица квадратная" << std::endl;
        std::cout << "Определитель первой матрицы: " << det << std::endl;
        std::cout << "Следы первой матрицы: " << tr << std::endl;


        if (det != 0) {
            std::cout << "Обратная первая матрица:\n";
            for (const auto& row : invMatrix) {
                for (double elem : row) {
                    std::cout << elem << " ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
    }
    else {
        if (!isSquareMatrix(taskThree)) {
            std::cout << "Первая матрица не квадратная. ";
        }
        else {
            std::cout << "Размерность первой матрицы должна быть больше 1. ";
        }
        std::cout << "Невозможно найти определитель, следы и обратную матрицу МАТРИЦЫ 1." << std::endl;
        std::cout << std::endl;
    }

    // Проверка являются ли данные матрицы квадратными. В случае квадратных матриц найти их определители
    //  (determinant) и следы (trace), а также если возможно вычислить обратные матрицы.
    if (isSquareMatrix(taskThree2) && taskThree2.size() > 1) {
        double det = determinant(taskThree2);
        double tr = 0.0;
        try {
            tr = trace(taskThree2);
        }
        catch (const std::out_of_range& e) {
            std::cout << "Ошибка: " << e.what() << std::endl;
        }
        catch (const std::overflow_error& e) {
            std::cout << "Ошибка: " << e.what() << std::endl;
        }
        std::vector<std::vector<double>> invMatrix2 = inverseMatrix(taskThree2);

        std::cout << "Вторая матрица квадратная" << std::endl;
        // Добавьте вывод определителя, следов и обратной матрицы
        std::cout << "Определитель второй матрицы: " << det << std::endl;
        std::cout << "Следы второй матрицы: " << tr << std::endl;

        if (det != 0)
        {
            std::cout << "Обратная вторая матрица:\n";
            for (const auto& row : invMatrix2) {
                for (double elem : row) {
                    std::cout << elem << " ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }  
    }
    else {
        if (!isSquareMatrix(taskThree2)) {
            std::cout << "Вторая матрица не квадратная. ";
        }
        else {
            std::cout << "Размерность второй матрицы должна быть больше 1. ";
        }
        std::cout << "Невозможно найти определитель, следы и обратную матрицу МАТРИЦЫ 2." << std::endl;
        std::cout << std::endl;
    }

    // Вызов функции areMatricesEqual (нахождение равенства матриц)
    bool resultAreMatricesEqual = areMatricesEqual(taskFour, taskFour2);
    if (resultAreMatricesEqual) {
        std::cout << "\nМатрицы равны." << std::endl;
    }
    else {
        std::cout << "\nМатрицы не равны." << std::endl;
    }

    try {
        // Вызов функции addMatrices
        std::vector<std::vector<double>> result = addMatrices(taskFour, taskFour2);

        // Проверка на пустоту результирующей матрицы и вывод или пропуск вывода
        if (!result.empty()) {
            std::cout << "\nРезультат сложения матриц:" << std::endl;
            for (const auto& row : result) {
                for (const auto& elem : row) {
                    std::cout << elem << " ";
                }
                std::cout << std::endl;
            }
        }
        else {
            std::cout << "\nРезультирующая матрица пустая. Пустые матрицы не могут быть сложены." << std::endl;
        }

        // Вызов функции addMatrices
        std::vector<std::vector<double>> resultmultiplyMatrices = multiplyMatrices(taskFour, taskFour2);

        // Проверка на пустоту результирующей матрицы и вывод или пропуск вывода
        if (!resultmultiplyMatrices.empty()) {
            std::cout << "\nРезультат умножения матриц:" << std::endl;
            // Вывод результирующей матрицы
            for (const auto& row : resultmultiplyMatrices) {
                for (const auto& elem : row) {
                    std::cout << elem << " ";
                }
                std::cout << std::endl;
            }
        }
    }
    catch (std::overflow_error& e) {
        std::cerr << "Произошло переполнение: " << e.what() << std::endl;
    }

    return 0;
}
