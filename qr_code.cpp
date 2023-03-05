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

bool check(vector<vector<int>> &qrcode, const int n, const vector<int> &linhas, const vector<int> &colunas, const vector<int> &diagonais, const vector<int> &quadrants, const vector<int> &lt, const vector<int> &ct)
{

    int line = 0;
    int column = 0;
    int diagonal = 0;
    int inversediagonal = 0;
    int quadrantsEqual[4] = {0, 0, 0, 0};
    int rowTransitions = 0;
    int colTransitions = 0;

    int flor = floor(n / 2);

    for (int i = 0; i < n; i++)
    {

        int valorLinha = qrcode[i][0];
        rowTransitions = 0;
        int valorColuna = qrcode[0][i];
        colTransitions = 0;

        for (int j = 0; j < n; j++)
        {

            if (qrcode[i][j] == 1)
            {
                line++;
            }
            if (qrcode[j][i] == 1)
            {
                column++;
            }

            if (i == j && qrcode[i][j] == 1)
            {
                diagonal++;
            }

            if (n - 1 == j + i && qrcode[i][j] == 1)
            {
                inversediagonal++;
            }

            if (qrcode[i][j] != valorLinha)
            {
                rowTransitions++;
            }

            if (qrcode[j][i] != valorColuna)
            {
                colTransitions++;
            }

            if (i < flor && j >= flor && qrcode[i][j] == 1)
            {
                quadrantsEqual[0]++;
            }

            else if (i < flor && j < flor && qrcode[i][j] == 1)
            {
                quadrantsEqual[1]++;
            }

            else if (i >= flor && j < flor && qrcode[i][j] == 1)
            {
                quadrantsEqual[2]++;
            }

            else if (i >= flor && j >= flor && qrcode[i][j] == 1)
            {
                quadrantsEqual[3]++;
            }

            valorColuna = qrcode[j][i];
            valorLinha = qrcode[i][j];
        }

        if (line != linhas[i] || column != colunas[i] || rowTransitions != lt[i] || colTransitions != ct[i])
        {
            // std::cout << "Erro nas transicoes" << std::endl;
            return false;
        }

        line = 0;
        column = 0;
    }

    if (diagonal != diagonais[0] || inversediagonal != diagonais[1])
    {
        return false;
    }

    for (int i = 0; i < 4; i++)
    {
        if (quadrantsEqual[i] != quadrants[i])
        {
            return false;
        }
    }

    return true;
}

// i is the row and j is the column
void geraqrcode(vector<vector<int>> &qrcode, const int n, int i, int j, const vector<int> &lb, const vector<int> &cb, const vector<int> &lt, const vector<int> &ct, const vector<int> &qb, const vector<int> &db)
{
    // if the board is full, check if it is a solution and print it if it is
    if (i == n)
    {
        if (check(qrcode, n, lb, cb, db, qb, lt, ct))
        {
            GLOBAL++;
            qrcode_ptr = qrcode;
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

bool preprocessamento(const int n, const vector<int> &lb, const vector<int> &cb, const vector<int> &lt, const vector<int> &ct, const vector<int> &qb, const vector<int> &db, int &test_linha, int &test_coluna, int &test_quadrante)
{
    if (test_linha != test_coluna || test_linha != test_quadrante || test_coluna != test_quadrante)
    {
        cout << "DEFECT: No QR Code generated!" << endl;
        return false;
    }

    for (int i = 0; i < n; i++)
    {
        if (lb[i] > n || cb[i] > n || lt[i] > n || ct[i] > n)
        {
            cout << "DEFECT: No QR Code generated!" << endl;
            return false;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        if (qb[i] > n)
        {
            cout << "DEFECT: No QR Code generated!" << endl;
            return false;
        }
    }

    for (int i = 0; i < 2; i++)
    {
        if (db[i] > n)
        {
            cout << "DEFECT: No QR Code generated!" << endl;
            return false;
        }
    }

    return true;
}

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    // redireciona a entrada para um arquivo
    // freopen("testes\\test_help_enunciado1.in", "r", stdin);

    int T;
    cin >> T;

    int test_linha = 0;
    int test_coluna = 0;
    int test_quadrante = 0;

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
            cin >> lb[i];
            test_linha += lb[i];
        }

        // cb
        for (int i = 0; i < N; i++)
        {
            cin >> cb[i];
            test_coluna += cb[i];
        }

        for (int i = 0; i < N; i++)
        {
            cin >> lt[i];
        }

        // ct
        for (int i = 0; i < N; i++)
        {
            cin >> ct[i];
        }

        // qb
        for (int i = 0; i < 4; i++)
        {
            cin >> qb[i];
            test_quadrante += qb[i];
        }

        // db
        for (int i = 0; i < 2; i++)
        {
            cin >> db[i];
        }

        if (preprocessamento(N, lb, cb, lt, ct, qb, db, test_linha, test_coluna, test_quadrante))
        {

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
        }
        GLOBAL = 0;
        test_coluna = 0;
        test_linha = 0;
        test_quadrante = 0;
    }

    return 0;
}

// Run : g++ qr_code.cpp -o qr_code; ./qr_code
