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
int update = 0;

vector<vector<int>> qrcode_ptr;

void preprocess2(vector<vector<int>> &v, const int n, const vector<int> &lb, const vector<int> &cb, const vector<int> &lt, const vector<int> &ct, const vector<int> &qb, const vector<int> &db);

int calcula_quadrados(int n, int quadrante);

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
        {
            v[line][i] = value;
            update++;
        }
    }
}

void fill_collumn(vector<vector<int>> &v, const int n, const int collumn, const int value)
{
    for (int i = 0; i < n; i++)
    {
        if (v[i][collumn] == -1)
        {
            v[i][collumn] = value;
            update++;
        }
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
            if (v[i][i] == -1)
            {
                v[i][i] = value;
                update++;
            }
        }
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            if (v[i][n - i - 1] == -1)
            {
                v[i][n - i - 1] = value;
                update++;
            }
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
                if (v[i][j] == -1)
                {
                    v[i][j] = value;
                }
            }
        }
    }
    else if (quadrant == 1)
    {
        for (int i = 0; i < flor; i++)
        {
            for (int j = 0; j < flor; j++)
            {
                if (v[i][j] == -1)
                {
                    v[i][j] = value;
                }
            }
        }
    }
    else if (quadrant == 2)
    {
        for (int i = flor; i < n; i++)
        {
            for (int j = 0; j < flor; j++)
            {
                if (v[i][j] == -1)
                {
                    v[i][j] = value;
                }
            }
        }
    }
    else
    {
        for (int i = flor; i < n; i++)
        {
            for (int j = flor; j < n; j++)
            {
                if (v[i][j] == -1)
                {
                    v[i][j] = value;
                }
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
                    update++;
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
                    update++;
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
                    if (v[i][j] == -1)
                    {
                        v[i][j] = 0;
                        update++;
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
                    update++;
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
                        update++;
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
                        update++;
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

    preprocess2(v, n, lb, cb, lt, ct, qb, db);

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

void preprocess2(vector<vector<int>> &v, const int n, const vector<int> &lb, const vector<int> &cb, const vector<int> &lt, const vector<int> &ct, const vector<int> &qb, const vector<int> &db)
{
    // Verificar se em cada linha ja temos o numero de brancas
    for (int i = 0; i < n; i++)
    {
        int num_branco_esperado = n - lb[i];

        int count_branco = 0;
        int count_preto = 0;

        for (int j = 0; j < n; j++)
        {
            if (v[i][j] == 0)
            {
                count_branco++;
            }
        }

        if (count_branco == num_branco_esperado)
        {
            for (int j = 0; j < n; j++)
            {
                if (v[i][j] == -1)
                {
                    v[i][j] = 1;
                    update++;
                }
            }
        }

        if (count_preto == lb[i])
        {
            for (int j = 0; j < n; j++)
            {
                if (v[i][j] == -1)
                {
                    v[i][j] = 0;
                    update++;
                }
            }
        }
    }

    // Verificar se em cada coluna ja temos o numero de brancas

    for (int i = 0; i < n; i++)
    {
        int num_branco_esperado = n - cb[i];

        int count_branco = 0;
        int count_preto = 0;

        for (int j = 0; j < n; j++)
        {
            if (v[j][i] == 0)
            {
                count_branco++;
            }
        }

        if (count_branco == num_branco_esperado)
        {
            for (int j = 0; j < n; j++)
            {
                if (v[j][i] == -1)
                {
                    v[j][i] = 1;
                    update++;
                }
            }
        }

        if (count_preto == cb[i])
        {
            for (int j = 0; j < n; j++)
            {
                if (v[j][i] == -1)
                {
                    v[j][i] = 0;
                    update++;
                }
            }
        }
    }

    int max_white_cells_first_quadrant = 0;
    int max_white_cells_second_quadrant = 0;
    int max_white_cells_third_quadrant = 0;
    int max_white_cells_fourth_quadrant = 0;

    if (n % 2 == 1)
    {
        max_white_cells_first_quadrant = calcula_quadrados(n, 1) - qb[0];
        max_white_cells_second_quadrant = calcula_quadrados(n, 2) - qb[1];
        max_white_cells_third_quadrant = calcula_quadrados(n, 3) - qb[2];
        max_white_cells_fourth_quadrant = calcula_quadrados(n, 4) - qb[3];
    }
    else
    {
        max_white_cells_first_quadrant = n * n / 4 - qb[0];
        max_white_cells_second_quadrant = n * n / 4 - qb[1];
        max_white_cells_third_quadrant = n * n / 4 - qb[2];
        max_white_cells_fourth_quadrant = n * n / 4 - qb[3];
    }

    // Verificar se em cada quadrante ja temos o numero de brancas
    // Second quadrant
    int sum_black = 0;
    int sum_white = 0;
    for (int k = 0; k < n / 2; k++)
    {
        for (int l = 0; l < n / 2; l++)
        {
            if (v[k][l] == 1)
            {
                sum_black++;
            }
            if (v[k][l] == 0)
            {
                sum_white++;
            }
        }
    }

    if (sum_black == qb[1])
    {
        fill_quadrant(v, n, 1, 0);
    }

    if (sum_white == max_white_cells_second_quadrant)
    {
        fill_quadrant(v, n, 1, 1);
    }

    // First quadrant
    sum_black = 0;
    sum_white = 0;
    for (int k = 0; k < n / 2; k++)
    {
        for (int l = n / 2; l < n; l++)
        {
            if (v[k][l] == 1)
            {
                sum_black++;
            }

            if (v[k][l] == 0)
            {
                sum_white++;
            }
        }
    }

    if (sum_black == qb[0])
    {
        fill_quadrant(v, n, 0, 0);
    }

    if (sum_white == max_white_cells_first_quadrant)
    {
        fill_quadrant(v, n, 0, 1);
    }

    // Third quadrant
    sum_black = 0;
    sum_white = 0;
    for (int k = n / 2; k < n; k++)
    {
        for (int l = 0; l < n / 2; l++)
        {
            if (v[k][l] == 1)
            {
                sum_black++;
            }
            if (v[k][l] == 0)
            {
                sum_white++;
            }
        }
    }

    if (sum_black == qb[2])
    {
        fill_quadrant(v, n, 2, 0);
    }
    if (sum_white == max_white_cells_third_quadrant)
    {
        fill_quadrant(v, n, 2, 1);
    }

    // Fourth quadrant
    sum_black = 0;
    sum_white = 0;
    for (int k = n / 2; k < n; k++)
    {
        for (int l = n / 2; l < n; l++)
        {
            if (v[k][l] == 1)
            {
                sum_black++;
            }
            if (v[k][l] == 0)
            {
                sum_white++;
            }
        }
    }

    if (sum_black == qb[3])
    {
        fill_quadrant(v, n, 3, 0);
    }
    if (sum_white == max_white_cells_fourth_quadrant)
    {
        fill_quadrant(v, n, 3, 1);
    }
}

bool is_filled_row(vector<vector<int>> &v, int n, int line)
{
    for (int i = 0; i < n; i++)
    {
        if (v[line][i] == -1)
        {
            return false;
        }
    }
    return true;
}

bool is_filled_collumn(vector<vector<int>> &v, int n, int collumn)
{
    for (int i = 0; i < n; i++)
    {
        if (v[i][collumn] == -1)
        {
            return false;
        }
    }
    return true;
}

bool is_fill_diagonal(vector<vector<int>> &v, int n, int diagonal)
{
    if (diagonal == 0)
    {
        for (int i = 0; i < n; i++)
        {
            if (v[i][i] == -1)
            {
                return false;
            }
        }
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            if (v[i][n - 1 - i] == -1)
            {
                return false;
            }
        }
    }
    return true;
}

bool is_filled_quadrant(vector<vector<int>> &v, int n, int quadrant)
{
    int flor = floor(n / 2);
    if (quadrant == 2)
    {
        for (int i = 0; i < flor; i++)
        {
            for (int j = 0; j < flor; j++)
            {
                if (v[i][j] == -1)
                {
                    return false;
                }
            }
        }
    }
    else if (quadrant == 1)
    {
        for (int i = 0; i < flor; i++)
        {
            for (int j = flor; j < n; j++)
            {
                if (v[i][j] == -1)
                {
                    return false;
                }
            }
        }
    }
    else if (quadrant == 3)
    {
        for (int i = flor; i < n; i++)
        {
            for (int j = 0; j < flor; j++)
            {
                if (v[i][j] == -1)
                {
                    return false;
                }
            }
        }
    }
    else
    {
        for (int i = flor; i < n; i++)
        {
            for (int j = flor; j < n; j++)
            {
                if (v[i][j] == -1)
                {
                    return false;
                }
            }
        }
    }
    return true;
}

bool is_impossivel(vector<vector<int>> &qr, const int n, int linha, int col, const vector<int> &lb, const vector<int> &cb, const vector<int> &lt, const vector<int> &ct, const vector<int> &qb, const vector<int> &db)
{
    // Se a linha esta cheia, podemos verificar com != , se nao, com > aa
    int count = 0;
    int count_transitions = 0;

    /*---------------------------------------- LINHAS ----------------------------------------*/
    if (is_filled_row(qr, n, linha))
    {

        for (int i = 0; i < n; i++)
        {
            if (qr[linha][i] == 1)
            {
                count++;
            }

            if ((qr[linha][i] == 1 && qr[linha][i - 1] == 0) ||
                (qr[linha][i] == 0 && qr[linha][i - 1] == 1))
            {
                count_transitions++;
            }
        }

        if (count != lb[linha])
        {
            return true;
        }

        if (count_transitions != lt[linha])
        {
            return true;
        }
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            if (qr[linha][i] == 1)
            {
                count++;
            }
            if ((qr[linha][i] == 1 && qr[linha][i - 1] == 0) ||
                (qr[linha][i] == 0 && qr[linha][i - 1] == 1))
            {
                count_transitions++;
            }
        }

        if (count > lb[linha])
        {
            return true;
        }

        if (count_transitions > lt[linha])
        {
            return true;
        }
    }

    /*---------------------------------------- COLUNAS ----------------------------------------*/

    // Se a coluna esta cheia, podemos verificar com != , se nao, com >
    count = 0;
    count_transitions = 0;

    if (is_filled_collumn(qr, n, linha))
    {

        for (int i = 0; i < n; i++)
        {
            if (qr[i][linha] == 1)
            {
                count++;
            }
            if ((qr[i][col] == 1 && qr[i - 1][col] == 0) ||
                (qr[i][col] == 0 && qr[i - 1][col] == 1))
            {
                count_transitions++;
            }
        }

        if (count != cb[linha])
        {
            return true;
        }

        if (count_transitions != ct[col])
        {
            return true;
        }
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            if (qr[i][linha] == 1)
            {
                count++;
            }
            if ((qr[i][col] == 1 && qr[i - 1][col] == 0) ||
                (qr[i][col] == 0 && qr[i - 1][col] == 1))
            {
                count_transitions++;
            }
        }

        if (count > cb[linha])
        {
            return true;
        }

        if (count_transitions > ct[col])
        {
            return true;
        }
    }

    /*---------------------------------------- DIAGONAL ----------------------------------------*/

    // Se a diagonal esta cheia, podemos verificar com != , se nao, com >

    count = 0;

    if (is_fill_diagonal(qr, n, 0))
    {

        for (int i = 0; i < n; i++)
        {
            if (qr[i][i] == 1)
            {
                count++;
            }
        }

        if (count != db[0])
        {
            return true;
        }
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            if (qr[i][i] == 1)
            {
                count++;
            }
        }

        if (count > db[0])
        {
            return true;
        }
    }

    count = 0;

    if (is_fill_diagonal(qr, n, 1))
    {

        for (int i = 0; i < n; i++)
        {
            if (qr[i][n - 1 - i] == 1)
            {
                count++;
            }
        }

        if (count != db[1])
        {
            return true;
        }
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            if (qr[i][n - 1 - i] == 1)
            {
                count++;
            }
        }

        if (count > db[1])
        {
            return true;
        }
    }

    /*---------------------------------------- QUADRANTES ----------------------------------------*/

    // Se o quadrante esta cheio, podemos verificar com != , se nao, com >

    count = 0;

    for (int i = 0; i < 4; i++)
    {
        if (is_filled_quadrant(qr, n, i + 1))
        {
            for (int j = 0; j < n; j++)
            {
                for (int k = 0; k < n; k++)
                {
                    if (qr[j][k] == 1)
                    {
                        count++;
                    }
                }
            }

            if (count != qb[i])
            {
                return true;
            }
        }
        else
        {
            for (int j = 0; j < n; j++)
            {
                for (int k = 0; k < n; k++)
                {
                    if (qr[j][k] == 1)
                    {
                        count++;
                    }
                }
            }

            if (count > qb[i])
            {
                return true;
            }
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

int count_white_cells_row(const vector<vector<int>> &qrcode, const int n, const int row)
{
    int sum_white = 0;

    for (int j = 0; j < n; j++)
    {
        if (qrcode[row][j] == 0)
        {
            sum_white++;
        }
    }

    return sum_white;
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

int count_white_cells_col(const vector<vector<int>> &qrcode, const int n, const int col)
{
    int sum_white = 0;

    for (int i = 0; i < n; i++)
    {
        if (qrcode[i][col] == 0)
        {
            sum_white++;
        }
    }

    return sum_white;
}

int count_black_cells_diagonal(const vector<vector<int>> &qrcode, const int n)
{
    int sum_black = 0;

    for (int i = 0; i < n; i++)
    {
        if (qrcode[i][i] == 1)
        {
            sum_black++;
        }
    }

    return sum_black;
}

int count_white_cells_diagonal(const vector<vector<int>> &qrcode, const int n)
{
    int sum_white = 0;

    for (int i = 0; i < n; i++)
    {
        if (qrcode[i][i] == 0)
        {
            sum_white++;
        }
    }

    return sum_white;
}

int count_black_cells_antidiagonal(const vector<vector<int>> &qrcode, const int n)
{
    int sum_black = 0;

    for (int i = 0; i < n; i++)
    {
        if (qrcode[i][n - i - 1] == 1)
        {
            sum_black++;
        }
    }

    return sum_black;
}

int count_white_cells_antidiagonal(const vector<vector<int>> &qrcode, const int n)
{
    int sum_white = 0;

    for (int i = 0; i < n; i++)
    {
        if (qrcode[i][n - i - 1] == 0)
        {
            sum_white++;
        }
    }

    return sum_white;
}

int calcula_quadrados(int n, int quadrante)
{
    int comprimento;
    int largura;

    if (quadrante == 1)
    {
        comprimento = n / 2 + 1;
        largura = n / 2;
    }
    else if (quadrante == 2)
    {
        comprimento = n / 2;
        largura = n / 2;
    }
    else if (quadrante == 3)
    {
        comprimento = n / 2;
        largura = n / 2 + 1;
    }
    else if (quadrante == 4)
    {
        comprimento = n / 2 + 1;
        largura = n / 2 + 1;
    }

    return comprimento * largura;
}

bool is_full(const vector<vector<int>> &qrcode, const int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (qrcode[i][j] == -1)
            {
                return false;
            }
        }
    }
    return true;
}

void geraqrcode(vector<vector<int>> &qrcode, const int n, int i, int j, const vector<int> &lb, const vector<int> &cb, const vector<int> &lt, const vector<int> &ct, const vector<int> &qb, const vector<int> &db, int sum_black_total)
{
    // Print the qrcode so far
    // printqrcode(&qrcode, n);

    NUM_CELULAS_PROCESSADAS++;

    // if the board is full, check if it is a solution and print it if it is
    if (is_full(qrcode, n))
    {
        if (check(qrcode, n, lb, cb, db, qb, lt, ct))
        {
            GLOBAL++;
            qrcode_ptr = qrcode;
        }
        return;
    }

    if (j > 0 && is_filled_collumn(qrcode, n, j - 1))
    {
        // Check if the number of black cells in the column is correct
        if (count_black_cells_col(qrcode, n, j - 1) != cb[j - 1])
        {
            return;
        }
        if (count_white_cells_col(qrcode, n, j - 1) != n - cb[j - 1])
        {
            return;
        }
    }

    // If the line i-1 has the right number of black cells
    if (i > 0 && count_black_cells_row(qrcode, n, i - 1) != lb[i - 1])
    {
        return;
    }

    // If the line i-1 has the right number of white cells
    if (i > 0 && count_white_cells_row(qrcode, n, i - 1) != n - lb[i - 1])
    {
        return;
    }

    // If the line i-1 has more transitions than lt[i-1]
    if (i > 0 && countTransitionsRow(qrcode, i - 1) != lt[i - 1])
    {
        return;
    }

    // If the column j-1 has the right number of black cells
    if (j > 0 && count_black_cells_col(qrcode, n, j - 1) > cb[j - 1])
    {
        return;
    }

    // If the column j-1 has the right number of white cells
    if (j > 0 && count_white_cells_col(qrcode, n, j - 1) > n - cb[j - 1])
    {
        return;
    }

    // If the column j-1 has more transitions than ct[j-1]
    if (j > 0 && countTransitionsCol(qrcode, j - 1) > ct[j - 1])
    {
        return;
    }

    // Check if the already exceeded the number of black cells in the diagonal
    if (i > 0 && count_black_cells_diagonal(qrcode, n) > db[0])
    {
        return;
    }

    // Check if the already exceeded the number of white cells in the diagonal
    if (i > 0 && count_white_cells_diagonal(qrcode, n) > n - db[0])
    {
        return;
    }

    // Check if the already exceeded the number of black cells in the antidiagonal
    if (i > 0 && count_black_cells_antidiagonal(qrcode, n) > db[1])
    {
        return;
    }

    // Check if the already exceeded the number of white cells in the antidiagonal

    if (i > 0 && count_white_cells_antidiagonal(qrcode, n) > n - db[1])
    {
        return;
    }

    // Check the number of black cells
    if (sum_black_total > MAX_BLACK_CELLS)
    {
        return;
    }

    // If we already passed half of the board, we can check if the number of black cells and the number of white cells are correct in the first and second quadrants
    int max_white_cells_first_quadrant = 0;
    int max_white_cells_second_quadrant = 0;
    int max_white_cells_third_quadrant = 0;
    int max_white_cells_fourth_quadrant = 0;

    if (n % 2 == 1)
    {
        max_white_cells_first_quadrant = calcula_quadrados(n, 1) - qb[0];
        max_white_cells_second_quadrant = calcula_quadrados(n, 2) - qb[1];
        max_white_cells_third_quadrant = calcula_quadrados(n, 3) - qb[2];
        max_white_cells_fourth_quadrant = calcula_quadrados(n, 4) - qb[3];
    }
    else
    {
        max_white_cells_first_quadrant = n * n / 4 - qb[0];
        max_white_cells_second_quadrant = n * n / 4 - qb[1];
        max_white_cells_third_quadrant = n * n / 4 - qb[2];
        max_white_cells_fourth_quadrant = n * n / 4 - qb[3];
    }

    if (i > n / 2)
    {
        // Second quadrant
        int sum_black = 0;
        int sum_white = 0;
        for (int k = 0; k < n / 2; k++)
        {
            for (int l = 0; l < n / 2; l++)
            {
                if (qrcode[k][l] == 1)
                {
                    sum_black++;
                }
                if (qrcode[k][l] == 0)
                {
                    sum_white++;
                }
            }
        }

        if (sum_black != qb[1])
        {
            return;
        }

        if (sum_white != max_white_cells_second_quadrant)
        {
            return;
        }

        // First quadrant
        sum_black = 0;
        sum_white = 0;
        for (int k = 0; k < n / 2; k++)
        {
            for (int l = n / 2; l < n; l++)
            {
                if (qrcode[k][l] == 1)
                {
                    sum_black++;
                }

                if (qrcode[k][l] == 0)
                {
                    sum_white++;
                }
            }
        }

        if (sum_black != qb[0])
        {
            return;
        }

        if (sum_white != max_white_cells_first_quadrant)
        {
            return;
        }
    }
    else
    {
        // Second quadrant
        int sum_black = 0;
        int sum_white = 0;
        for (int k = 0; k < n / 2; k++)
        {
            for (int l = 0; l < n / 2; l++)
            {
                if (qrcode[k][l] == 1)
                {
                    sum_black++;
                }
                if (qrcode[k][l] == 0)
                {
                    sum_white++;
                }
            }
        }

        if (sum_black > qb[1])
        {
            return;
        }
        if (sum_white > max_white_cells_second_quadrant)
        {
            return;
        }

        // First quadrant
        sum_black = 0;
        sum_white = 0;
        for (int k = 0; k < n / 2; k++)
        {
            for (int l = n / 2; l < n; l++)
            {
                if (qrcode[k][l] == 1)
                {
                    sum_black++;
                }
                if (qrcode[k][l] == 0)
                {
                    sum_white++;
                }
            }
        }

        if (sum_black > qb[0])
        {
            return;
        }
        if (sum_white > max_white_cells_first_quadrant)
        {
            return;
        }
    }

    if (i > n / 2)
    {
        // Third quadrant
        int sum_black = 0;
        int sum_white = 0;
        for (int k = n / 2; k < n; k++)
        {
            for (int l = 0; l < n / 2; l++)
            {
                if (qrcode[k][l] == 1)
                {
                    sum_black++;
                }
                if (qrcode[k][l] == 0)
                {
                    sum_white++;
                }
            }
        }

        if (sum_black > qb[2])
        {
            return;
        }
        if (sum_white > max_white_cells_third_quadrant)
        {
            return;
        }

        // Fourth quadrant
        sum_black = 0;
        sum_white = 0;
        for (int k = n / 2; k < n; k++)
        {
            for (int l = n / 2; l < n; l++)
            {
                if (qrcode[k][l] == 1)
                {
                    sum_black++;
                }
                if (qrcode[k][l] == 0)
                {
                    sum_white++;
                }
            }
        }

        if (sum_black > qb[3])
        {
            return;
        }
        if (sum_white > max_white_cells_fourth_quadrant)
        {
            return;
        }
    }

    // if the row is full, go to the next row
    if (j == n)
    {
        geraqrcode(qrcode, n, i + 1, 0, lb, cb, lt, ct, qb, db, sum_black_total);
        return;
    }

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

int count_black_cells_qrcode(vector<vector<int>> &qrcode, int n)
{
    int sum_black = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (qrcode[i][j] == 1)
            {
                sum_black++;
            }
        }
    }

    return sum_black;
}

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    // redireciona a entrada para um arquivo
    // freopen("testes\\teste_tudo.in", "r", stdin);
    // freopen("testes\\test_help_enunciado1.in", "r", stdin); // 309 - 215 - 207 - 163 - 149 -147
    // freopen("testes\\test_help_enunciado2.in", "r", stdin);
    // freopen("testes\\test_help_1.in", "r", stdin);
    // freopen("testes\\test_help_2.in", "r", stdin); // Sem pre processamento       42310 - 28884 - 28816
    // freopen("testes\\test_help_3.in", "r", stdin); // Sem pre processamento    1337707 - 51381 - 998 - 790 - 664 - 652 - 644
    // freopen("testes\\test_help_4.in", "r", stdin); // Sem pre processamento    2826195 - 36249 - 21151 - 8643 - 7761 - 6529 - 6455 - 6454
    // freopen("testes\\test_help_5.in", "r", stdin); // Sem pre  processamento   3091057 - 1522640 - 2045900 - 2011593 - 1136392 - 997038 - 992954

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
                // preprocess(qrcode, N, lb, cb, lt, ct, qb, db);
                //  printqrcode(&qrcode, N);
                num_ones = count_black_cells_qrcode(qrcode, N);
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