#include <iostream>
#include <vector>

using namespace std;

void fill_line(vector<vector<int>> &v, const int n, const int line, const int value)
{
    for (int i = 0; i < n; i++)
    {
        v[line][i] = value;
    }
}

void fill_collumn(vector<vector<int>> &v, const int n, const int collumn, const int value)
{
    for (int i = 0; i < n; i++)
    {
        v[i][collumn] = value;
    }
}

void fill_diagonal(vector<vector<int>> &v, const int n, const int diagonal, const int value)
{
    // If diagonal is 0, fill the main diagonal
    // if diagonal is 1, fill the anti-diagonal
    if (diagonal == 0)
    {
        for (int i = 0; i < n; i++)
        {
            v[i][i] = value;
        }
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            v[i][n - i - 1] = value;
        }
    }
}

void preprocess(vector<vector<int>> &v, const int n, const vector<int> &lb, const vector<int> &cb, const vector<int> &lt, const vector<int> &ct, const vector<int> &qb, const vector<int> &db)
{
    // Check if there are lb,cb,db wiht value = 0
    for (int i = 0; i < n; i++)
    {
        // lb
        if (lb[i] == 0)
        {
            // Fill all the line with 0
            fill_line(v, n, i, 0);
        }
        else if (lb[i] == n)
        {
            // Fill all the line with 1
            fill_line(v, n, i, 1);
        }

        // cb
        if (cb[i] == 0)
        {
            // Fill all the collumn with 0
            fill_collumn(v, n, i, 0);
        }
        else if (cb[i] == n)
        {
            // Fill all the collumn with 1
            fill_collumn(v, n, i, 1);
        }

        // db
        if (db[i] == 0)
        {
            // Fill all the diagonal with 0
            fill_diagonal(v, n, i, 0);
        }
        else if (db[i] == n)
        {
            // Fill all the diagonal with 1
            fill_diagonal(v, n, i, 1);
        }
    }
}

void print_vector(vector<vector<int>> &v, const int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << v[i][j] << " ";
        }
        cout << endl;
    }
}

int main()
{
    freopen("teste.in", "r", stdin);

    // Create a 2d vector 9x9 and initialize it with -1
    std::vector<std::vector<int>> v(3, std::vector<int>(3, -1));

    // Create lb,cb,db,lt,ct,qt
    vector<int> lb(3, 0);
    vector<int> cb(3, 0);
    vector<int> lt(3, 0);
    vector<int> ct(3, 0);
    vector<int> qb(4, 0);
    vector<int> db(2, 0);

    lb[0] = 1;
    lb[1] = 0;
    lb[2] = 1;

    cb[0] = 1;
    cb[1] = 0;
    cb[2] = 1;

    lt[0] = 1;
    lt[1] = 0;
    lt[2] = 1;

    ct[0] = 1;
    ct[1] = 0;
    ct[2] = 1;

    qb[0] = 0;
    qb[1] = 1;
    qb[2] = 0;
    qb[3] = 1;

    db[0] = 2;
    db[1] = 0;

    preprocess(v, 3, lb, cb, lt, ct, qb, db);
    print_vector(v, 3);

    return 0;
}

// run: g++ teste.cpp -o teste; ./teste