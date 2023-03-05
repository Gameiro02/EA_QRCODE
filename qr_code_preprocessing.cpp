#include <iostream>
#include <vector>

using namespace std;

// Função que verifica se uma célula pode ser preenchida com base nas restrições de linha e coluna
bool canFill(int rowFilled, int colFilled, int rowTrans, int colTrans, int totalRows, int totalCols, int row, int col)
{
    // Verifica se a célula está em uma linha com restrição de preenchimento
    if (rowFilled > 0 && row == rowFilled - 1)
    {
        // Verifica se ainda há espaço para preencher células nessa linha
        if (totalCols - colFilled >= totalCols - col)
        {
            // Verifica se a célula é a última a ser preenchida na linha
            if (totalCols - colFilled == totalCols - col && rowTrans > 0)
            {
                return false;
            }
            return true;
        }
        else
        {
            return false;
        }
    }

    // Verifica se a célula está em uma coluna com restrição de preenchimento
    if (colFilled > 0 && col == colFilled - 1)
    {
        // Verifica se ainda há espaço para preencher células nessa coluna
        if (totalRows - rowFilled >= totalRows - row)
        {
            // Verifica se a célula é a última a ser preenchida na coluna
            if (totalRows - rowFilled == totalRows - row && colTrans > 0)
            {
                return false;
            }
            return true;
        }
        else
        {
            return false;
        }
    }

    return false;
}

// Função que determina as células que podem ser preenchidas em preprocessamento
void preprocess(vector<vector<int>> &grid, vector<int> &rowFilled, vector<int> &colFilled, vector<int> &rowTrans, vector<int> &colTrans, vector<int> &quadFilled, int diag1Filled, int diag2Filled)
{
    int totalRows = grid.size();
    int totalCols = grid[0].size();

    // Verifica cada célula na grade
    for (int i = 0; i < totalRows; i++)
    {
        for (int j = 0; j < totalCols; j++)
        {
            // Verifica se a célula já foi preenchida
            if (grid[i][j] != -1)
            {
                continue;
            }

            // Verifica se a célula pode ser preenchida com base nas restrições de linha e coluna
            if (canFill(rowFilled[i], colFilled[j], rowTrans[i], colTrans[j], totalRows, totalCols, i, j))
            {
                // Preenche a célula
                grid[i][j] = 1;

                // Atualiza as restrições de linha e coluna
                rowFilled[i]++;
                colFilled[j]++;

                // Verifica se a célula faz parte de uma transição de linha ou coluna
                if (j > 0 && grid[i][j - 1] != -1 && grid[i][j - 1] != grid[i][j])
                {
                    rowTrans[i]++;
                }
                if (i > 0 && grid[i - 1][j] != -1 && grid[i - 1][j] != grid[i][j])
                {
                    colTrans[j]++;
                }
            }

            // Verifica se a célula pode ser // preenchida com base nas restrições do quadrante
            if (quadFilled.size() > 0)
            {
                int quadRow = i / 3;
                int quadCol = j / 3;
                int quadIndex = quadRow * 3 + quadCol;
                if (quadFilled[quadIndex] < 3 && rowFilled[i] < 3 && colFilled[j] < 3)
                {
                    grid[i][j] = 1;
                    rowFilled[i]++;
                    colFilled[j]++;
                    quadFilled[quadIndex]++;
                }
            }

            // Verifica se a célula pode ser preenchida na diagonal principal
            if (i == j && diag1Filled > 0 && rowFilled[i] < diag1Filled && colFilled[j] < diag1Filled)
            {
                grid[i][j] = 1;
                rowFilled[i]++;
                colFilled[j]++;
            }

            // Verifica se a célula pode ser preenchida na diagonal secundária
            if (i + j == totalRows - 1 && diag2Filled > 0 && rowFilled[i] < diag2Filled && colFilled[j] < diag2Filled)
            {
                grid[i][j] = 1;
                rowFilled[i]++;
                colFilled[j]++;
            }
        }
    }
}

int getQuadrant(int row, int col, int n)
{
    int mid = n / 2;
    if (row <= mid && col > mid)
    {
        return 1;
    }
    else if (row <= mid && col <= mid)
    {
        return 2;
    }
    else if (row > mid && col <= mid)
    {
        return 3;
    }
    else
    {
        return 4;
    }
}

bool canBeZero(int row, int col, vector<vector<int>> &grid, vector<int> &rowFilled, vector<int> &colFilled, vector<int> &rowTrans, vector<int> &colTrans, vector<int> &quadFilled, int &diag1Filled, int &diag2Filled)
{
    int n = grid.size();
    int quad = getQuadrant(row, col, n);

    // Verifica se a célula pode ser preenchida com um 0
    if (rowFilled[row] < n / 2 && colFilled[col] < n / 2 && rowTrans[row] < n / 2 && colTrans[col] < n / 2 && quadFilled[quad] < n / 2 && diag1Filled < n / 2 && diag2Filled < n / 2)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool canBeOne(int row, int col, vector<vector<int>> &grid, vector<int> &rowFilled, vector<int> &colFilled, vector<int> &rowTrans, vector<int> &colTrans, vector<int> &quadFilled, int &diag1Filled, int &diag2Filled)
{
    int n = grid.size();
    int quad = getQuadrant(row, col, n);

    // Verifica se a célula pode ser preenchida com um 1
    if (rowFilled[row] < n / 2 && colFilled[col] < n / 2 && rowTrans[row] < n / 2 && colTrans[col] < n / 2 && quadFilled[quad] < n / 2 && diag1Filled < n / 2 && diag2Filled < n / 2 && grid[row][col] == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int main()
{
    // Exemplo de uso da função preprocess
    vector<vector<int>> grid = {{-1, -1, -1, -1, -1, -1, -1, -1, -1},
                                {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                                {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                                {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                                {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                                {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                                {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                                {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                                {-1, -1, -1, -1, -1, -1, -1, -1, -1}};

    vector<int> rowFilled = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    vector<int> colFilled = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    vector<int> rowTrans = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    vector<int> colTrans = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    vector<int> quadFilled = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    int diag1Filled = 0;
    int diag2Filled = 0;
    int n = grid.size();
    int numFilled = 0;

    preprocess(grid, rowFilled, colFilled, rowTrans, colTrans, quadFilled, diag1Filled, diag2Filled);
    // Verifica se a célula pode ser uma 1 ou 0 com base nas informações disponíveis
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (grid[i][j] == -1)
            {
                // Verifica se a célula pode ser preenchida com um 1
                if (canBeOne(i, j, grid, rowFilled, colFilled, rowTrans, colTrans, quadFilled, diag1Filled, diag2Filled))
                {
                    grid[i][j] = 1;
                    numFilled++;
                }
                // Caso contrário, verifica se pode ser preenchida com um 0
                else if (canBeZero(i, j, grid, rowFilled, colFilled, rowTrans, colTrans, quadFilled, diag1Filled, diag2Filled))
                {
                    grid[i][j] = 0;
                    numFilled++;
                }
            }
        }
    }
}
