#include <iostream>
#include <string.h>
#include <fstream>

using namespace std;

int INF = 0; // Global variable to hold largest value + 1

void display_shortest_path_by_next(int **matrix, int **next, int n, int i, int j, int K, bool is_root = true)
{
    static int depth = 0; // aka number of elements in path
    static int *path = new int[n + 1];

    int k = next[i][j];

    depth++;

    if (k == -1)
    {
        if (is_root)
        {
            path[depth] = i;
        }

        if (depth > 1)
        {
            if (path[depth - 1] != j)
                path[depth] = j;
            else
                depth--;
        }
        else if (is_root)
            path[++depth] = j;
        else
            path[depth] = j;
    }
    else
    {
        if (depth > 1)
        {
            if (path[depth - 1] != i)
                path[depth] = i;
            else
                depth--;
        }
        else
            path[depth] = i;

        display_shortest_path_by_next(matrix, next, n, i, k, K, false);
        display_shortest_path_by_next(matrix, next, n, k, j, K, false);
    }

    if (is_root)
    {
        int a;

        cout << endl
             << "According to Floyds's Algorithm:" << endl
             << "Shorted distanceance between V1 and V2: " << matrix[i][j] << endl
             << "Following are the villages where the bottle refilling will take place: " << endl;
        for (a = 1; a < depth; a++)
            cout << path[a] << " > ";
        cout << path[a] << endl;
        cout << "Number of villages between V1 and V2: " << depth - 2 << endl;

        delete[] path;
    }
}

void floyds_algo(int **matrix, int **next, int n, int v1, int v2, int K)
{
    int i, j, k;

    for (k = 0; k < n; k++)
    {
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                if (matrix[i][k] + matrix[k][j] < matrix[i][j])
                {
                    matrix[i][j] = matrix[i][k] + matrix[k][j];
                    next[i][j] = k;
                }
            }
        }
    }

    display_shortest_path_by_next(matrix, next, n, v1, v2, K);
}

int min_d(int *total_distance, int *ignored, int n)
{
    int min = -1, min_i, i;

    for (i = 0; i < n; i++)
    {
        if (!ignored[i] && (total_distance[i] < min || min == -1) && total_distance[i] != -1)
        {
            min = total_distance[i];
            min_i = i;
        }
    }

    return min == -1 ? -1 : min_i;
}

int print_djistra_path(int **matrix, int *path, int v2, bool is_root = true)
{
    static int depth = 0;

    depth++;
    if (path[v2] == -1)
    {
        cout << v2;
    }
    else
    {
        print_djistra_path(matrix, path, path[v2], false);
        
        cout << " > " << v2;
    }

    if (is_root) cout << endl;

    return depth;
}

void dijkstra_algo(int **matrix, int n, int v1, int v2, int K)
{
    int i, u, v, c;
    int *total_distance = new int[n],
        *ignored = new int[n],
        *length = new int[n],
        *path = new int[n];

    path[v1] = -1;
    ignored[0] = 0;
    length[0] = 0;

    for (i = 0; i < n; i++)
        total_distance[i] = INF;

    total_distance[v1] = 0;
    for (c = 0; c < n - 1; c++)
    {
        u = min_d(total_distance, ignored, n);

        if (u == -1)
            break;

        ignored[u] = 1;
        for (v = 0; v < n; v++)
        {
            if (!ignored[v] && matrix[u][v] &&
                total_distance[u] + matrix[u][v] < total_distance[v] &&
                total_distance[u] != -1)
            {
                path[v] = u;
                length[v] = length[path[v]] + 1;
                total_distance[v] = total_distance[u] + matrix[u][v];
            }
            else if (!ignored[v] && matrix[u][v] &&
                     total_distance[u] + matrix[u][v] == total_distance[v] &&
                     length[u] + 1 < length[v])
            {
                path[v] = u;
                length[v] = length[u] + 1;
            }
        }
    }

    if (total_distance[v2] != -1)
    {
        cout << endl
             << "According to Dijkstra's Algorithm:" << endl
             << "Shorted distance between V1 and V2: " << total_distance[v2] << endl
             << "Following are the villages where the bottle refilling will take place: " << endl;

        i = print_djistra_path(matrix, path, v2);
        cout << "Number of villages between V1 and V2: " << i - 2 << endl;
    }
    else
    {
        cout << endl << "There is no shortest path from V1 to V2" << endl;
    }

    delete[] total_distance;
    delete[] ignored;
    delete[] path;
    delete[] length;
}

int main(int argc, char *argv[])
{
    int N = 0;            // Number of vertices
    int V1 = -1, V2 = -1; // Start and end vertices
    int i = 0, j = 0;
    int K = 0;                                                                     // Bottle capacity
    char map1[] = "Shortest-to-post-100.txt", map2[] = "Shortest-to-post-300.txt"; // Maps
    int map_to_use = 0;

    ifstream file;
    int a, b, c;

    int **NEXT, **MATRIX1, **MATRIX2; // MATRIX1 for Floyd algo, MATRIX2 for Dijkstra Algo

    cout << "There are two maps: " << endl;
    cout << "1. " << map1 << endl;
    cout << "2. " << map2 << endl;
    do
    {
        cout << "Which map to use? (Choose 1 or 2): ";
        cin >> map_to_use;
    } while (map_to_use < 1 || map_to_use > 2);

    N = map_to_use == 1 ? 100 : 300;

    cout << "Enter Starting vertex (V1): ";
    cin >> V1;
    cout << "Enter Target vertex (V2): ";
    cin >> V2;
    cout << "Enter Capacity of Bottle (K): ";
    cin >> K;

    // Read from file to MATRIX
    if (map_to_use == 1)
        file.open(map1);
    else
        file.open(map2);
    if (file.is_open())
    {
        // Initializing matrices
        NEXT = new int *[N];
        MATRIX1 = new int *[N];
        MATRIX2 = new int *[N];
        for (i = 0; i < N; i++)
        {
            NEXT[i] = new int[N];
            MATRIX1[i] = new int[N];
            MATRIX2[i] = new int[N];

            for (j = 0; j < N; j++)
            {
                NEXT[i][j] = -1;
                MATRIX1[i][j] = -1;
                MATRIX2[i][j] = -1;
            }
        }
        while (file >> a >> b >> c)
        {
            if (a >= 0 && a < N && b >= 0 && b < N)
            {
                if (c < 0)
                    MATRIX1[a][b] = 2; // -1 is to be replaced with 2, as mentioned in document
                else
                    MATRIX1[a][b] = c;
                MATRIX2[a][b] = MATRIX1[a][b];

                if (INF <= MATRIX1[a][b])
                    INF = MATRIX1[a][b] + 1;
                // MATRIX2 is deep copy of MATRIX1 so it can be used in both algorithms
            }
        }

        file.close();

        // Apply Floyd Algorithm
        floyds_algo(MATRIX1, NEXT, N, V1, V2, K);

        // Apply Dijkstra Algorithm
        dijkstra_algo(MATRIX2, N, V1, V2, K);

        // deallocate dynamically allocated memory
        for (i = 0; i < N; i++)
        {
            delete[] NEXT[i];
            delete[] MATRIX1[i];
            delete[] MATRIX2[i];
        }
        delete[] NEXT;
        delete[] MATRIX1;
        delete[] MATRIX2;
    }
    else
    {
        cout << "Failed to open input file." << endl;
    }

    return 0;
}
