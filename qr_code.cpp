#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

int NUM_CELULAS_PROCESSADAS = 0;
int GLOBAL = 0;

vector<vector<int>> qrcode_ptr;

int count_line_transicoes(const vector<vector<int>> &qrcode, const int n, const vector<int> &lt, const vector<int> &ct, int linha)
{
    int rowTransitions = 0;
    int valorLinha = qrcode[linha][0];
    for (int j = 0; j < n; j++)
    {
        if (qrcode[linha][j] != valorLinha)
        {
            rowTransitions++;
            valorLinha = qrcode[linha][j];
        }
    }
    return rowTransitions;
}

int count_col_transicoes(const vector<vector<int>> &qrcode, const int n, const vector<int> &lt, const vector<int> &ct, int coluna)
{
    int colTransitions = 0;
    int valorColuna = qrcode[0][coluna];
    for (int j = 0; j < n; j++)
    {
        if (qrcode[j][coluna] != valorColuna)
        {
            colTransitions++;
            valorColuna = qrcode[j][coluna];
        }
    }
    return colTransitions;
}

void print_2darray(const vector<vector<int>> &v, const int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (v[i][j] == -1)
                cout << "x ";
            else
                cout << v[i][j] << " ";
        }
        cout << endl;
    }
}

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
            else if ((*qrcode_ptr)[i][j] == 0)
            {
                cout << " ";
            }
            else
            {
                cout << "x";
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
        // std::cout << "Erro nas diagonais" << std::endl;
        return false;
    }

    for (int i = 0; i < 4; i++)
    {
        if (quadrantsEqual[i] != quadrants[i])
        {
            // std::cout << "Erro nos quadrantes" << std::endl;
            return false;
        }
    }

    return true;
}

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

    // only change the cell if it is equal to -1
    if (qrcode.at(i).at(j) == -1)
    {
        qrcode.at(i).at(j) = 1;
        geraqrcode(qrcode, n, i, j + 1, lb, cb, lt, ct, qb, db);
        qrcode.at(i).at(j) = 0;
        geraqrcode(qrcode, n, i, j + 1, lb, cb, lt, ct, qb, db);
        qrcode.at(i).at(j) = -1;
    }
    else
    {
        geraqrcode(qrcode, n, i, j + 1, lb, cb, lt, ct, qb, db);
    }
    NUM_CELULAS_PROCESSADAS++;
}

void fill_line(vector<vector<int>> &v, const int n, const int line, const int value)
{
    for (int i = 0; i < n; i++)
    {
        if (v[line][i] == -1)
            v[line][i] = value;
    }
}

void fill_line2(vector<vector<int>> &v, const int n, const int line)
{
    for (int i = 0; i < n - 1; i++)
    {
        if (v[line][i] == -1)
        {
            // Fill the oposite value of the previous cell
            if (v[line][i - 1] == 1)
                v[line][i] = 0;
            else if (v[line][i - 1] == 0)
                v[line][i] = 1;
        }
    }
}

void fill_collumn(vector<vector<int>> &v, const int n, const int collumn, const int value)
{
    for (int i = 0; i < n; i++)
    {
        if (v[i][collumn] == -1)
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

void fill_quadrant(vector<vector<int>> &v, const int n, const int quadrant, const int value)
{
    // If quadrant is 0, fill the first quadrant
    // if quadrant is 1, fill the second quadrant
    // if quadrant is 2, fill the third quadrant
    // if quadrant is 3, fill the fourth quadrant
    int flor = n / 2;
    if (quadrant == 0)
    {
        for (int i = 0; i < flor; i++)
        {
            for (int j = flor; j < n; j++)
            {
                v[i][j] = value;
            }
        }
    }
    else if (quadrant == 1)
    {
        for (int i = 0; i < flor; i++)
        {
            for (int j = 0; j < flor; j++)
            {
                v[i][j] = value;
            }
        }
    }
    else if (quadrant == 2)
    {
        for (int i = flor; i < n; i++)
        {
            for (int j = 0; j < flor; j++)
            {
                v[i][j] = value;
            }
        }
    }
    else
    {
        for (int i = flor; i < n; i++)
        {
            for (int j = flor; j < n; j++)
            {
                v[i][j] = value;
            }
        }
    }
}

// Pair retuning function to get the first cell != -1 in a specific line
pair<int, int> get_first_cell_line(const vector<vector<int>> &v, const int n, const int line)
{
    for (int i = 0; i < n; i++)
    {
        if (v[line][i] != -1)
        {
            return make_pair(line, i);
        }
    }
    return make_pair(-1, -1);
}

pair<int, int> get_first_cell_collumn(const vector<vector<int>> &v, const int n, const int collumn)
{
    for (int i = 0; i < n; i++)
    {
        if (v[i][collumn] != -1)
        {
            return make_pair(i, collumn);
        }
    }
    return make_pair(-1, -1);
}

bool preprocess(vector<vector<int>> &v, const int n, const vector<int> &lb, const vector<int> &cb, const vector<int> &lt, const vector<int> &ct, const vector<int> &qb, const vector<int> &db)
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
    }

    // db
    if (db[0] == 0)
    {
        // Fill all the diagonal with 0
        fill_diagonal(v, n, 0, 0);
    }
    else if (db[0] == n)
    {
        // Fill all the diagonal with 1
        fill_diagonal(v, n, 0, 1);
    }
    if (db[1] == 0)
    {
        // Fill all the anti-diagonal with 0
        fill_diagonal(v, n, 1, 0);
    }
    else if (db[1] == n)
    {
        // Fill all the anti-diagonal with 1
        fill_diagonal(v, n, 1, 1);
    }

    // qb
    for (int i = 0; i < 4; i++)
    {
        if (qb[i] == 0)
        {
            // Fill all the quadrant with 0
            fill_quadrant(v, n, i, 0);
        }
        else if (qb[i] == n)
        {
            // Fill all the quadrant with 1
            fill_quadrant(v, n, i, 1);
        }
    }

    for (int i = 0; i < n; i++)
    {
        // Check the line transictions
        int rowTransitions = count_line_transicoes(v, n, lt, ct, i);
        if (rowTransitions == lt[i])
        {
            // Fill the rest of the line the opposite value the previous cell
            fill_line2(v, n, i);
        }
    }

    // Check if the qr_code is full and if it is valid
    bool flag = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (v[i][j] == -1)
            {
                flag = 1;
                break;
            }
        }
    }

    if (flag == 0)
    {
        if (check(v, n, lb, cb, db, qb, lt, ct))
        {
            GLOBAL++;
            qrcode_ptr = v;
            return true;
        }
    }
    return false;
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
        if (lb[i] > n || cb[i] > n || lt[i] > n || ct[i] > n || lb[i] < 0 || cb[i] < 0 || lt[i] < 0 || ct[i] < 0)
        {
            cout << "DEFECT: No QR Code generated!" << endl;
            return false;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        if (qb[i] > floor((n / 2 + 1) * (n / 2 + 1)) || qb[i] < 0)
        {
            cout << "DEFECT: No QR Code generated!" << endl;
            return false;
        }
    }

    for (int i = 0; i < 2; i++)
    {
        if (db[i] > n || db[i] < 0)
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
    freopen("teste.in", "r", stdin);

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

        vector<vector<int>> qrcode(N, vector<int>(N, -1));

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
            if (!preprocess(qrcode, N, lb, cb, lt, ct, qb, db))
            {
                printqrcode(&qrcode, N);
                // geraqrcode(qrcode, N, 0, 0, lb, cb, lt, ct, qb, db);
            }
            // preprocess(qrcode, N, lb, cb, lt, ct, qb, db);
            // printqrcode(&qrcode, N);
            // geraqrcode(qrcode, N, 0, 0, lb, cb, lt, ct, qb, db);
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
                // printqrcode(&qrcode, N);
            }
        }
        // std::cout << NUM_CELULAS_PROCESSADAS << std::endl;
        GLOBAL = 0;
        test_coluna = 0;
        test_linha = 0;
        test_quadrante = 0;
        NUM_CELULAS_PROCESSADAS = 0;
    }

    return 0;
}

// Run : g++ qr_code.cpp -o qr_code; ./qr_code
