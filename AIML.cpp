#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <omp.h>

using namespace std;

struct Point {
    double x, y;
    int cluster;
};

double distance(Point a, Point b) {
    return sqrt((a.x - b.x) * (a.x - b.x) +
                (a.y - b.y) * (a.y - b.y));
}

void initializePoints(vector<Point>& points, int n) {
    for (int i = 0; i < n; i++) {
        Point p;
        p.x = rand() % 1000;
        p.y = rand() % 1000;
        p.cluster = -1;
        points.push_back(p);
    }
}

void initializeCentroids(vector<Point>& centroids, int k) {
    for (int i = 0; i < k; i++) {
        Point c;
        c.x = rand() % 1000;
        c.y = rand() % 1000;
        c.cluster = i;
        centroids.push_back(c);
    }
}

void sequentialKMeans(vector<Point>& points,
                      vector<Point>& centroids,
                      int iterations) {

    int k = centroids.size();

    for (int iter = 0; iter < iterations; iter++) {

        // Assign clusters
        for (int i = 0; i < points.size(); i++) {

            double minDist = 1e9;
            int bestCluster = 0;

            for (int j = 0; j < k; j++) {
                double dist = distance(points[i], centroids[j]);

                if (dist < minDist) {
                    minDist = dist;
                    bestCluster = j;
                }
            }

            points[i].cluster = bestCluster;
        }

        // Update centroids
        vector<double> sumX(k, 0);
        vector<double> sumY(k, 0);
        vector<int> count(k, 0);

        for (int i = 0; i < points.size(); i++) {
            int c = points[i].cluster;

            sumX[c] += points[i].x;
            sumY[c] += points[i].y;
            count[c]++;
        }

        for (int j = 0; j < k; j++) {
            if (count[j] > 0) {
                centroids[j].x = sumX[j] / count[j];
                centroids[j].y = sumY[j] / count[j];
            }
        }
    }
}

void parallelKMeans(vector<Point>& points,
                    vector<Point>& centroids,
                    int iterations) {

    int k = centroids.size();

    for (int iter = 0; iter < iterations; iter++) {

        // Parallel cluster assignment
        #pragma omp parallel for
        for (int i = 0; i < points.size(); i++) {

            double minDist = 1e9;
            int bestCluster = 0;

            for (int j = 0; j < k; j++) {

                double dist = distance(points[i], centroids[j]);

                if (dist < minDist) {
                    minDist = dist;
                    bestCluster = j;
                }
            }

            points[i].cluster = bestCluster;
        }

        vector<double> sumX(k, 0);
        vector<double> sumY(k, 0);
        vector<int> count(k, 0);

        // Parallel reduction
        #pragma omp parallel
        {
            vector<double> localSumX(k, 0);
            vector<double> localSumY(k, 0);
            vector<int> localCount(k, 0);

            #pragma omp for
            for (int i = 0; i < points.size(); i++) {

                int c = points[i].cluster;

                localSumX[c] += points[i].x;
                localSumY[c] += points[i].y;
                localCount[c]++;
            }

            #pragma omp critical
            {
                for (int j = 0; j < k; j++) {
                    sumX[j] += localSumX[j];
                    sumY[j] += localSumY[j];
                    count[j] += localCount[j];
                }
            }
        }

        // Update centroids
        for (int j = 0; j < k; j++) {
            if (count[j] > 0) {
                centroids[j].x = sumX[j] / count[j];
                centroids[j].y = sumY[j] / count[j];
            }
        }
    }
}

int main() {

    srand(time(0));

    int numPoints = 100000;
    int k = 5;
    int iterations = 20;

    vector<Point> points1;
    vector<Point> points2;

    initializePoints(points1, numPoints);
    points2 = points1;

    vector<Point> centroids1;
    vector<Point> centroids2;

    initializeCentroids(centroids1, k);
    centroids2 = centroids1;

    // Sequential timing
    double start1 = omp_get_wtime();

    sequentialKMeans(points1, centroids1, iterations);

    double end1 = omp_get_wtime();

    // Parallel timing
    double start2 = omp_get_wtime();

    parallelKMeans(points2, centroids2, iterations);

    double end2 = omp_get_wtime();

    cout << "Sequential Time: "
         << end1 - start1 << " seconds\n";

    cout << "Parallel Time: "
         << end2 - start2 << " seconds\n";

    cout << "Speedup: "
         << (end1 - start1) / (end2 - start2)
         << endl;

    return 0;
}