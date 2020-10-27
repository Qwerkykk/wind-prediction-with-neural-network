#ifndef CURVES_DTW_H
#define CURVES_DTW_H

#include <vector>
#include <cmath>
#include <iostream>

#include "Euclidean.h"
#include "Position.h"


template<typename T>
T min(T x, T y) {
    if (x > y)
        return x;
    else
        return y;
}


struct {
    template<typename T>
    double operator()(const std::vector<T> &vector1, const std::vector<T> &vector2) {
        int n = vector1.size();
        int m = vector2.size();
        double array[n][m];
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (i == 0 && j == 0) {
                    array[0][0] = Euclidean(vector1[0], vector2[0]);
                } else if (i == 0 && j > 0) {
                    array[i][j] = array[i][j - 1] + Euclidean(vector1[i], vector2[j]);
                } else if (j == 0) {
                    array[i][j] = array[i - 1][j] + Euclidean(vector1[i], vector2[j]);
                } else {
                    array[i][j] = min(min(array[i - 1][j], array[i - 1][j - 1]), array[i][j - 1]) + Euclidean(
                            vector1[i], vector2[j]);
                }
            }
        }
        return array[n - 1][m - 1];
    }

    template<typename T>
    std::vector<Position> *findPath(const std::vector<T> &vector1, const std::vector<T> &vector2) {
        int n = vector1.size();
        int m = vector2.size();
        double array[n][m];
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (i == 0 && j == 0) {
                    array[0][0] = Euclidean(vector1[0], vector2[0]);
                } else if (i == 0 && j > 0) {
                    array[i][j] = array[i][j - 1] + Euclidean(vector1[i], vector2[j]);
                } else if (j == 0) {
                    array[i][j] = array[i - 1][j] + Euclidean(vector1[i], vector2[j]);
                } else {
                    array[i][j] = min(min(array[i - 1][j], array[i - 1][j - 1]), array[i][j - 1]) + Euclidean(
                            vector1[i], vector2[j]);
                }
            }
        }


        int x = n - 1;
        int y = m - 1;

        auto *path = new std::vector<Position>;

        while (x != 0 || y != 0) {
            if (x != 0 && y != 0) {
                if ((array[x - 1][y] < array[x - 1][y - 1] || array[x - 1][y] == array[x - 1][y - 1]) && (array[x - 1][y] < array[x][y - 1] || array[x - 1][y] == array[x][y - 1])) {
                    path->insert(path->begin(), Position(x, y));
                    x = x - 1;
                    continue;
                } else if ((array[x - 1][y - 1] < array[x - 1][y] || array[x - 1][y - 1] == array[x - 1][y]) && (array[x - 1][y - 1] < array[x][y - 1] || array[x - 1][y - 1] == array[x][y - 1])) {
                    path->insert(path->begin(), Position(x, y));
                    x = x - 1;
                    y = y - 1;
                    continue;
                } else if (array[x][y - 1] < array[x - 1][y - 1] && array[x][y - 1] < array[x - 1][y]) {
                    path->insert(path->begin(), Position(x, y));
                    y = y - 1;
                    continue;
                }
            } else if (x != 0 && y == 0) {
                path->insert(path->begin(), Position(x, y));
                x = x - 1;
                continue;
            } else if (x == 0 && y != 0) {
                path->insert(path->begin(), Position(x, y));
                y = y - 1;
                continue;
            }
        }

        path->insert(path->begin(), Position(x, y));

        return path;
    }

} DTW;


#endif

