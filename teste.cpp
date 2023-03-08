bool solve(int row, int col)
{
    if (row == N)
    { // a última linha foi preenchida
        return true;
    }

    int nextRow = row;
    int nextCol = col + 1;

    if (nextCol == N)
    { // passa para a próxima linha
        nextRow++;
        nextCol = 0;
    }
    for (int i = 0; i <= 1; i++)
    { // loop para testar as duas possibilidades (0 ou 1)
        matrix[row][col] = i;
        if (isValid(row, col))
        {
            if (solve(nextRow, nextCol))
            {
                return true;
            }
        }
    }
    matrix[row][col] = 0; // remove o valor atribuído
    return false;
}