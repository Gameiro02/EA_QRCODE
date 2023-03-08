#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>

/* numero de células pretas em cada linha, , vetor de tamanho N (lb);
 numero de células pretas em cada coluna, vetor de tamanho N (cb);
 numero de transições em cada linha, vetor de tamanho N (lt);
 numero de transições em cada coluna, vetor de tamanho N (ct);
 numero de células pretas em cada quadrante, vetor de tamanho 4 (qb);
 numero de células pretas em cada diagonal, vetor de tamanho 2 (db);*/

using namespace std;

int NUM_CELULAS_PROCESSADAS = 0;
int GLOBAL = 0; // Global variable to count the number of solutions

int NUMERO_ZEROS = 0;
int NUMERO_UNS = 0;
int MAX_BLACK_CELLS = 0;
bool STOP = false;

vector<vector<int>> qrcode_ptr;

bool is_on_diagonal(int n, int i, int j)
{
    if (i == j || i + j == n - 1)
    {
        return true;
    }
    return false;
}

int count_cells_in_quadrant(const vector<vector<int>> &qrcode, int n, int quadrant_row_start, int quadrant_col_start)
{
    int row_end = quadrant_row_start + n / 2;
    int col_end = quadrant_col_start + n / 2;
    int count = 0;

    for (int i = quadrant_row_start; i < row_end; i++)
    {
        for (int j = quadrant_col_start; j < col_end; j++)
        {
            if (qrcode.at(i).at(j) == 1)
            {
                count++;
            }
        }
    }

    return count;
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
                cout << "?";
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

void fill_line(vector<vector<int>> &v, const int n, const int line, const int value)
{
    for (int i = 0; i < n; i++)
    {
        if (v[line][i] == -1)
            v[line][i] = value;
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

void verifica_linhas(vector<vector<int>> &v, int n, int linha_total, int linha)
{
    int count = 0;
    for (int i = 0; i < n; i++)
    {
        if (v[linha][i] == 1)
        {
            count++;
        }

        if (linha_total == count)
        {
            for (int j = 0; j < n; j++)
            {
                if (v[linha][j] == -1)
                {
                    v[linha][j] = 0;
                }
            }
        }
    }
}

void verifica_colunas(vector<vector<int>> &v, int n, int coluna_total, int coluna)
{
    int count = 0;
    for (int i = 0; i < n; i++)
    {
        if (v[i][coluna] == 1)
        {
            count++;
        }

        if (coluna_total == count)
        {
            for (int j = 0; j < n; j++)
            {
                if (v[j][coluna] == -1)
                {
                    v[j][coluna] = 0;
                }
            }
        }
    }
}

void verifica_quadrantes(vector<vector<int>> &v, int n, const vector<int> &quadrante_total)
{
    int count1 = 0;
    int count2 = 0;
    int count3 = 0;
    int count4 = 0;
    int flor = n / 2;

    // quadrant 1 if l <= floor(N/2) and c > floor(N/2)
    for (int i = 0; i < n; i++)
    {
        for (int j = flor; j < n; j++)
        {
            if (i <= flor && j > flor)
            {
                if (v[i][j] == 1)
                {
                    count1++;
                }
            }
            if (i <= flor && j <= flor)
            {
                if (v[i][j] == 1)
                {
                    count2++;
                }
            }
            if (i > flor && j <= flor)
            {
                if (v[i][j] == 1)
                {
                    count3++;
                }
            }
            if (i > flor && j > flor)
            {
                if (v[i][j] == 1)
                {
                    count4++;
                }
            }
        }
    }

    if (quadrante_total[0] == count1)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (i <= flor && j > flor)
                {
                    if (v[i][j] == -11)
                    {
                        v[i][j] = 0;
                    }
                }
            }
        }
    }

    if (quadrante_total[1] == count2)
    {
        for (int i = 0; i < flor; i++)
        {
            for (int j = 0; j < flor; j++)
            {
                if (v[i][j] == -1)
                {
                    v[i][j] = 0;
                }
            }
        }
    }

    if (quadrante_total[2] == count3)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (i > flor && j <= flor)
                {
                    if (v[i][j] == -1)
                    {
                        v[i][j] = 0;
                    }
                }
            }
        }
    }

    if (quadrante_total[3] == count4)
    {
        for (int i = flor; i < n; i++)
        {
            for (int j = flor; j < n; j++)
            {
                if (i > flor && j > flor)
                {
                    if (v[i][j] == 1)
                    {
                        v[i][j] = 0;
                    }
                }
            }
        }
    }
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
        // if qb[i] == floor(n/2 +1)^2
        else if (qb[i] == (floor(n / 2 + 1) * floor(n / 2 + 1)))
        {
            // Fill all the quadrant with 1
            fill_quadrant(v, n, i, 1);
        }
    }

    for (int i = 0; i < n; i++)
    {
        verifica_linhas(v, n, lb[i], i);
    }

    for (int i = 0; i < n; i++)
    {
        verifica_colunas(v, n, cb[i], i);
    }

    // qb
    verifica_quadrantes(v, n, qb);

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

        if (lb[i] == 0 && lt[i] != 0)
        {
            cout << "DEFECT: No QR Code generated!" << endl;
            return false;
        }
        if (cb[i] == 0 && ct[i] != 0)
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
    int sum = 0;
    for (int i = 0; i < n / 2; i++)
    {
        sum += lb[i];
    }

    if (sum != qb[0] + qb[1])
    {
        cout << "DEFECT: No QR Code generated!" << endl;
        return false;
    }

    sum = 0;
    for (int i = n / 2; i < n; i++)
    {
        sum += lb[i];
    }

    if (sum != qb[2] + qb[3])
    {
        cout << "DEFECT: No QR Code generated!" << endl;
        return false;
    }

    // DO the same for the 1st and 4th quadrant
    sum = 0;

    for (int i = n / 2; i < n; i++)
    {
        sum += cb[i];
    }

    if (sum != qb[0] + qb[3])
    {
        cout << "DEFECT: No QR Code generated!" << endl;
        return false;
    }

    sum = 0;
    for (int i = 0; i < n / 2; i++)
    {
        sum += cb[i];
    }

    if (sum != qb[1] + qb[2])
    {
        cout << "DEFECT: No QR Code generated!" << endl;
        return false;
    }

    return true;
}

int count_black_cells_first_q(const vector<vector<int>> &qrcode, const int n)
{
    // Count the black cells in the first and in the fourth quadrant
    // The first quadrant is the one in the top right corner : lines 0 to n/2 and columns n/2 to n
    int sum_black = 0;

    for (int i = 0; i < n / 2; i++)
    {
        for (int j = n / 2; j < n; j++)
        {
            if (qrcode[i][j] == 1)
            {
                sum_black++;
            }
        }
    }

    return sum_black;
}

int count_black_cells_second_q(const vector<vector<int>> &qrcode, const int n)
{
    // Count the black cells in the second and in the third quadrant
    // The second quadrant is the one in the top left corner : lines 0 to n/2 and columns 0 to n/2

    int sum_black = 0;

    for (int i = 0; i < n / 2; i++)
    {
        for (int j = 0; j < n / 2; j++)
        {
            if (qrcode[i][j] == 1)
            {
                sum_black++;
            }
        }
    }

    return sum_black;
}

int count_black_cells_third_q(const vector<vector<int>> &qrcode, const int n)
{
    // Count the black cells in the second and in the third quadrant
    // The third quadrant is the one in the bottom left corner : lines n/2 to n and columns 0 to n/2

    int sum_black = 0;

    for (int i = n / 2; i < n; i++)
    {
        for (int j = 0; j < n / 2; j++)
        {
            if (qrcode[i][j] == 1)
            {
                sum_black++;
            }
        }
    }

    return sum_black;
}

int count_black_cells_fourth_q(const vector<vector<int>> &qrcode, const int n)
{
    // Count the black cells in the first and in the fourth quadrant
    // The fourth quadrant is the one in the bottom right corner : lines n/2 to n and columns n/2 to n

    int sum_black = 0;

    for (int i = n / 2; i < n; i++)
    {
        for (int j = n / 2; j < n; j++)
        {
            if (qrcode[i][j] == 1)
            {
                sum_black++;
            }
        }
    }

    return sum_black;
}

int countTransitionsRow(vector<vector<int>> matrix, int row)
{
    int count = 0;
    int n = matrix[0].size();

    for (int i = 1; i < n; i++)
    {
        if ((matrix[row][i] == 1 && matrix[row][i - 1] == 0) ||
            (matrix[row][i] == 0 && matrix[row][i - 1] == 1))
        {
            count++;
        }
    }
    return count;
}

int countTransitionsCol(vector<vector<int>> matrix, int col)
{
    int count = 0;
    int n = matrix.size();

    for (int i = 1; i < n; i++)
    {
        if ((matrix[i][col] == 1 && matrix[i - 1][col] == 0) ||
            (matrix[i][col] == 0 && matrix[i - 1][col] == 1))
        {
            count++;
        }
    }
    return count;
}

// Function to count the number of black cells in a given row
int count_black_cells_row(const vector<vector<int>> &qrcode, const int n, const int row)
{
    int sum_black = 0;

    for (int j = 0; j < n; j++)
    {
        if (qrcode[row][j] == 1)
        {
            sum_black++;
        }
    }

    return sum_black;
}

int count_black_cells_col(const vector<vector<int>> &qrcode, const int n, const int col)
{
    int sum_black = 0;

    for (int i = 0; i < n; i++)
    {
        if (qrcode[i][col] == 1)
        {
            sum_black++;
        }
    }

    return sum_black;
}

void geraqrcode(vector<vector<int>> &qrcode, const int n, int i, int j, const vector<int> &lb, const vector<int> &cb, const vector<int> &lt, const vector<int> &ct, const vector<int> &qb, const vector<int> &db, int sum_black_total)
{
    // Print the qrcode so far
    // printqrcode(&qrcode, n);

    NUM_CELULAS_PROCESSADAS++;

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
        geraqrcode(qrcode, n, i + 1, 0, lb, cb, lt, ct, qb, db, sum_black_total);
        return;
    }

    // If if the line i-1 has the right number of black cells

    if (i > 0 && count_black_cells_row(qrcode, n, i - 1) != lb[i - 1])
    {
        return;
    }

    // Check if the number of 1's in the row is equal to the number of 1's in the line
    // Check if the number of 1's in the column is equal to the number of 1's in the column
    // Check if the number of 1's in the diagonal is equal to the number of 1's in the diagonal

    int sum_d = 0;
    int sum_d2 = 0;
    int sum = 0;
    int sum_c = 0;
    for (int k = 0; k < n; k++)
    {
        if (qrcode.at(i).at(k) == 1)
        {
            sum++;
        }
        if (qrcode.at(k).at(j) == 1)
        {
            sum_c++;
        }
        if (qrcode.at(k).at(k) == 1)
        {
            sum_d++;
        }
        if (qrcode.at(k).at(n - k - 1) == 1)
        {
            sum_d2++;
        }
    }

    if (sum > lb.at(i) || sum_c > cb.at(j) || sum_d > db.at(0) || sum_d2 > db.at(1))
    {
        return;
    }

    // Check if the number of black cells in the matrix has already been exceeded
    if (sum_black_total > MAX_BLACK_CELLS)
    {
        return;
    }

    // If we already passed the second quadrant, check if the number of black cells in the second quadrant is equal to the number of black cells in the qb[1]
    if (i > n / 2)
    {
        int sum_black_second_q = count_black_cells_second_q(qrcode, n);
        if (sum_black_second_q != qb[1])
        {
            return;
        }
    }

    // If we already passed the first quadrant, check if the number of black cells in the first quadrant is equal to the number of black cells in the qb[0]
    if (i > n / 2)
    {
        int sum_black_first_q = count_black_cells_first_q(qrcode, n);
        if (sum_black_first_q != qb[0])
        {
            return;
        }
    }

    // // If we already passed the third quadrant, check if the number of black cells in the third quadrant is equal to the number of black cells in the qb[2]
    // if (i > n / 2 && j > n / 2)
    // {
    //     int sum_black_third_q = count_black_cells_third_q(qrcode, n);
    //     if (sum_black_third_q != qb[2])
    //     {
    //         return;
    //     }
    // }

    // // Check if the number of transitions in the row is equal to the number of transitions in the lt
    // if (countTransitionsRow(qrcode, i) > lt.at(i))
    // {
    //     return;
    // }

    // // Check if the number of transitions in the column is equal to the number of transitions in the ct
    // if (countTransitionsCol(qrcode, j) > ct.at(j))
    // {
    //     return;
    // }

    // If we already passed the half of the matrix, check if the number of black cells in the first half is equal to the number of black cells of the first quadrant + the number of black cells of the fourth quadrant
    // if (i >= n / 2)
    // {
    //     sum = 0;
    //     for (int i = 0; i < n / 2; i++)
    //     {
    //         sum += lb[i];
    //     }

    //     if (sum != qb[0] + qb[1])
    //     {
    //         return;
    //     }
    // }

    // only change the cell if it is equal to -1
    if (qrcode.at(i).at(j) == -1)
    {
        qrcode.at(i).at(j) = 1;
        geraqrcode(qrcode, n, i, j + 1, lb, cb, lt, ct, qb, db, sum_black_total + 1);
        qrcode.at(i).at(j) = 0;
        geraqrcode(qrcode, n, i, j + 1, lb, cb, lt, ct, qb, db, sum_black_total);
        qrcode.at(i).at(j) = -1;
    }
    else
    {
        geraqrcode(qrcode, n, i, j + 1, lb, cb, lt, ct, qb, db, sum_black_total);
    }
}

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    // redireciona a entrada para um arquivo
    // freopen("teste.in", "r", stdin);
    // freopen("testes\\test_help_enunciado1.in", "r", stdin);
    // freopen("testes\\test_help_enunciado2.in", "r", stdin);
    //  freopen("testes\\test_help_1.in", "r", stdin);
    // freopen("testes\\test_help_2.in", "r", stdin); // Sem pre processamento
    // freopen("testes\\test_help_3.in", "r", stdin); // Sem pre processamento
    // freopen("testes\\test_help_4.in", "r", stdin); // Sem pre processamento       // tem de ter 1.2/1.3
    //   freopen("testes\\test_help_5.in", "r", stdin);

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
        int num_ones = 0;

        vector<vector<int>> qrcode(N, vector<int>(N, -1));

        // lb
        for (int i = 0; i < N; i++)
        {
            cin >> lb[i];
            test_linha += lb[i];
            MAX_BLACK_CELLS += lb[i];
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
                // printqrcode(&qrcode, N);
                //  geraqrcode(qrcode, N, 0, 0, lb, cb, lt, ct, qb, db);
                geraqrcode(qrcode, N, 0, 0, lb, cb, lt, ct, qb, db, num_ones);
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
        MAX_BLACK_CELLS = 0;
    }

    return 0;
}

// Run : g++ -O2 qr_code.cpp -o qr_code; ./qr_code
