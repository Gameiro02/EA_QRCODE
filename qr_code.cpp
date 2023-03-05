#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

int MAXN = 31;
int GLOBAL = 0;

vector<vector<int>> qrcode_ptr;

void printqrcode(const vector<vector<int>> *qrcode_ptr, const int n)
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
            if ((*qrcode_ptr)[i][j] == 1)
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

bool checklinhas(const vector<vector<int>> &qrcode, const int n, const vector<int> &lb)
{
    int count = 0;

    //
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (qrcode[i][j] == 1)
            {
                count++;
            }
        }

        if (count != lb[i])
        {
            return false;
        }

        count = 0;
    }
    return true;
}

bool checkcolunas(const vector<vector<int>> &qrcode, const int n, const vector<int> &cb)
{
    int count = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (qrcode[j][i] == 1)
            {
                count++;
            }
        }

        if (count != cb[i])
        {
            return false;
        }

        count = 0;
    }

    return true;
}

bool check_transicoes(const vector<vector<int>> &qrcode, const int n, const vector<int> &lt, const vector<int> &ct)
{
    int rowTransitions = 0;
    int colTransitions = 0;

    for (int i = 0; i < n; ++i)
    {
        int rowPrev = qrcode[i][0];
        int colPrev = qrcode[0][i];
        for (int j = 1; j < n; ++j)
        {
            if (qrcode[i][j] != rowPrev)
            {
                ++rowTransitions;
                rowPrev = qrcode[i][j];
            }
            if (qrcode[j][i] != colPrev)
            {
                ++colTransitions;
                colPrev = qrcode[j][i];
            }
        }
        // std::cout << "rowTransitions: " << rowTransitions << std::endl;
        // std::cout << "colTransitions: " << colTransitions << std::endl;
        if (rowTransitions != lt[i] || colTransitions != ct[i])
        {
            return false;
        }
        rowTransitions = 0;
        colTransitions = 0;
    }
    return true;
}

bool checkquadrante(const vector<vector<int>> &qrcode, const int n, const vector<int> &qb)
{
    int count = 0;
    int count2 = 0;
    int count3 = 0;
    int count4 = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i < floor(n / 2) && j >= floor(n / 2) && qrcode[i][j] == 1)
            {
                count++;
            }
            if (i < floor(n / 2) && j < floor(n / 2) && qrcode[i][j] == 1)
            {
                count2++;
            }
            if (i >= floor(n / 2) && j < floor(n / 2) && qrcode[i][j] == 1)
            {
                count3++;
            }
            if (i >= floor(n / 2) && j >= floor(n / 2) && qrcode[i][j] == 1)
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

bool checkdiagonais(const vector<vector<int>> &qrcode, const int n, const vector<int> &db)
{
    int count = 0;  // diagonal principal
    int count2 = 0; // anti-diagonal

    // Anti-diagonal
    for (int i = 0; i < n; i++)
    {
        count2 += qrcode[i][n - i - 1];
        count += qrcode[i][i];
    }

    if (count != db[0] || count2 != db[1])
    {
        return false;
    }

    return true;
}

bool check(const vector<vector<int>> &qrcode, const int n, const vector<int> &lb, const vector<int> &cb, const vector<int> &lt, const vector<int> &ct, const vector<int> &qb, const vector<int> &db)
{
    if (checklinhas(qrcode, n, lb) && checkcolunas(qrcode, n, cb) && check_transicoes(qrcode, n, lt, ct) && checkquadrante(qrcode, n, qb) && checkdiagonais(qrcode, n, db))
    {
        return true;
    }
    return false;
}

int blackCells(const vector<vector<int>> &qrcode, const int n)
{
    int count = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (qrcode[i][j] == 1)
            {
                count++;
            }
        }
    }
    return count;
}

// i is the row and j is the column
void geraqrcode(vector<vector<int>> &qrcode, const int n, int i, int j, const vector<int> &lb, const vector<int> &cb, const vector<int> &lt, const vector<int> &ct, const vector<int> &qb, const vector<int> &db)
{
    // if the board is full, check if it is a solution and print it if it is
    if (i == n)
    {
        if (check(qrcode, n, lb, cb, lt, ct, qb, db))
        {
            GLOBAL++;
            // Create a pointer to the qrcode variable
            qrcode_ptr = qrcode;
            // printqrcode(qrcode, n);
        }
        return;
    }

    // if the row is full, go to the next row
    if (j == n)
    {
        geraqrcode(qrcode, n, i + 1, 0, lb, cb, lt, ct, qb, db);
        return;
    }

    // try to put a black cell in the current position then try to put a white cell

    qrcode.at(i).at(j) = 1;
    geraqrcode(qrcode, n, i, j + 1, lb, cb, lt, ct, qb, db);
    qrcode.at(i).at(j) = 0;
    geraqrcode(qrcode, n, i, j + 1, lb, cb, lt, ct, qb, db);
}

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    // redireciona a entrada para um arquivo
    // freopen("input.txt", "r", stdin);

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

        vector<vector<int>> qrcode(N, vector<int>(N));

        // lb
        for (int i = 0; i < N; i++)
        {
            // cin >> numero;
            // lb[i] = numero;
            cin >> lb[i];
        }

        // cb
        for (int i = 0; i < N; i++)
        {
            cin >> cb[i];
        }

        // lt
        for (int i = 0; i < N; i++)
        {
            cin >> lt[i];
        }

        // ct
        for (int i = 0; i < N; i++)
        {
            cin >> ct[i];
        }

        // db
        for (int i = 0; i < 4; i++)
        {
            cin >> qb[i];
        }

        // qb
        for (int i = 0; i < 2; i++)
        {
            cin >> db[i];
        }

        geraqrcode(qrcode, N, 0, 0, lb, cb, lt, ct, qb, db);

        if (GLOBAL == 1)
        {
            cout << "VALID: 1 QR Code generated!" << endl;
            printqrcode(&qrcode_ptr, N);
        }
        else if (GLOBAL > 1)
        {
            cout << "INVALID: " << GLOBAL << " QR Codes generated!" << endl;
        }
        else if (GLOBAL == 0)
        {
            cout << "DEFECT: No QR Code generated!" << endl;
        }
        GLOBAL = 0;
    }

    // std::cout << GLOBAL << std::endl;

    return 0;
}

// Run : g++ qr_code.cpp -o qrcode; ./qrcode
