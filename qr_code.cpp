#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

int MAXN = 31;
int GLOBAL = 0;

bool checklinhas(vector<vector<int>> &array, int n, vector<int> &lb)
{
    int count = 0;
    for (int i = 0; i < n; i++)
    {
        int linha = array[i][0];

        for (int j = 0; j < n; j++)
        {
            if (array[i][j] == 1)
            {
                count++;
            }
            linha = array[i][j];
        }

        if (count != lb[i])
        {
            return false;
        }

        count = 0;
    }

    return true;
}

bool checkcolunas(vector<vector<int>> &array, int n, vector<int> &cb)
{
    int count = 0;

    for (int i = 0; i < n; i++)
    {
        int coluna = array[0][i];

        for (int j = 0; j < n; j++)
        {
            if (array[j][i] == 1)
            {
                count++;
            }
            coluna = array[j][i];
        }

        if (count != cb[i])
        {
            return false;
        }

        count = 0;
    }

    return true;
}

bool checktrasicoes(vector<vector<int>> &array, int n, vector<int> &lt, vector<int> &ct)
{
    int count = 0;
    int count2 = 0;

    for (int i = 0; i < n; i++)
    {
        int linha = array[i][0];
        int coluna = array[0][i];

        for (int j = 0; j < n; j++)
        {
            if (array[i][j] == 1)
            {
                count++;
            }
            if (array[j][i] == 1)
            {
                count2++;
            }
            linha = array[i][j];
            coluna = array[j][i];
        }

        if (count != lt[i] || count2 != ct[i])
        {
            return false;
        }

        count = 0;
        count2 = 0;
    }

    return true;
}

bool checkquadrante(vector<vector<int>> &array, int n, vector<int> &qb)
{
    int count = 0;
    int count2 = 0;
    int count3 = 0;
    int count4 = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i < floor(n / 2) && j >= floor(n / 2) && array[i][j] == 1)
            {
                count++;
            }
            if (i < floor(n / 2) && j < floor(n / 2) && array[i][j] == 1)
            {
                count2++;
            }
            if (i >= floor(n / 2) && j < floor(n / 2) && array[i][j] == 1)
            {
                count3++;
            }
            if (i >= floor(n / 2) && j >= floor(n / 2) && array[i][j] == 1)
            {
                count4++;
            }
        }
    }

    if (count != qb[0] || count2 != qb[1] || count3 != qb[2] || count4 != qb[3])
    {
        return false;
    }

    return true;
}

bool checkdiagonais(vector<vector<int>> &array, int n, vector<int> &db)
{
    int count = 0;
    int count2 = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i == j && array[i][j] == 1)
            {
                count++;
            }
            if (i + j == n - 1 && array[i][j] == 1)
            {
                count2++;
            }
        }
    }

    if (count != db[0] || count2 != db[1])
    {
        return false;
    }

    return true;
}

bool check(vector<vector<int>> &array, int n, vector<int> &lb, vector<int> &cb, vector<int> &lt, vector<int> &ct, vector<int> &qb, vector<int> &db)
{
    if (checklinhas(array, n, lb) && checkcolunas(array, n, cb) && checktrasicoes(array, n, lt, ct) && checkquadrante(array, n, qb) && checkdiagonais(array, n, db))
    {
        return true;
    }

    return false;
}

int blackCells(vector<vector<int>> &array, int n)
{
    int count = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (array[i][j] == 1)
            {
                count++;
            }
        }
    }
    return count;
}

void printArray(vector<vector<int>> &array, int n)
{
    // make the boarder of the board with + in the corners and - in the sides
    cout << "+";
    for (int i = 0; i < n; i++)
    {
        cout << "-";
    }
    cout << "+" << endl;

    // print the board
    for (int i = 0; i < n; i++)
    {
        cout << "|";
        for (int j = 0; j < n; j++)
        {
            if (array[i][j] == 1)
            {
                cout << "#";
            }
            else
            {
                cout << " ";
            }
        }
        cout << "|" << endl;
    }

    // make the boarder of the board with + in the corners and - in the sides
    cout << "+";
    for (int i = 0; i < n; i++)
    {
        cout << "-";
    }
    cout << "+" << endl;
}

void geraArray(vector<vector<int>> &array, int n, int i, int j, vector<int> &lb, vector<int> &cb, vector<int> &lt, vector<int> &ct, vector<int> &qb, vector<int> &db)
{
    if (i == n)
    {
        if (check(array, n, lb, cb, lt, ct, qb, db))
        {
            printArray(array, n);
            GLOBAL++;
        }
        return;
    }

    if (j == n)
    {
        geraArray(array, n, i + 1, 0, lb, cb, lt, ct, qb, db);
        return;
    }

    array[i][j] = 1;
    geraArray(array, n, i, j + 1, lb, cb, lt, ct, qb, db);
    array[i][j] = 0;
    geraArray(array, n, i, j + 1, lb, cb, lt, ct, qb, db);
}

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    // redireciona a entrada para um arquivo
    freopen("input.txt", "r", stdin);

    int T;
    cin >> T;

    while (T--)
    {
        int N;
        cin >> N;

        vector<int> lb(N);
        vector<int> cb(N);
        vector<int> lt(N);
        vector<int> ct(N);
        vector<int> qb(4);
        vector<int> db(2);

        vector<vector<int>> array(N, vector<int>(N, 0));
        int numero;

        // lb
        for (int i = 0; i < N; i++)
        {

            cin >> numero;
            lb[i] = numero;
        }

        // cb
        for (int i = 0; i < N; i++)
        {
            cin >> numero;
            cb[i] = numero;
        }

        // lt
        for (int i = 0; i < N; i++)
        {
            cin >> numero;
            lt[i] = numero;
        }

        // ct
        for (int i = 0; i < N; i++)
        {
            cin >> numero;
            ct[i] = numero;
        }

        // db
        for (int i = 0; i < 4; i++)
        {
            cin >> numero;
            qb[i] = numero;
        }

        // qb
        for (int i = 0; i < 2; i++)
        {
            cin >> numero;
            db[i] = numero;
        }

        int pretos = 0;

        for (int i = 0; i < 4; i++)
        {
            pretos += qb[i];
        }

        geraArray(array, N, 0, 0, lb, cb, lt, ct, qb, db);
    }

    return 0;
}

// Run : g++ qr_code.cpp -o qrcode; ./qrcode