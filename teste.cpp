#include <vector>

using namespace std;

void descobrir_celulas(vector<vector<int>> &matriz, int n, vector<int> &lb, vector<int> &cb, vector<int> &lt, vector<int> &ct, vector<int> &qb, vector<int> &db)
{
    // Parte 1: inferir células com base em informações sobre linhas, colunas, diagonais e quadrantes
    for (int i = 0; i < n; i++)
    {
        int numPretoLinha = lb[i];
        int numTransLinha = lt[i];

        // Condição 1: se a linha só tem uma transição de cor e o número de células pretas já é conhecido, a linha pode ser totalmente preta
        if (numPretoLinha != -1 && numTransLinha == 1)
        {
            int posInicial = 0;
            int posFinal = 0;
            for (int j = 0; j < n; j++)
            {
                if (matriz[i][j] == 1)
                {
                    posInicial = j;
                    break;
                }
            }
            for (int j = n - 1; j >= 0; j--)
            {
                if (matriz[i][j] == 1)
                {
                    posFinal = j;
                    break;
                }
            }
            if (numPretoLinha == posFinal - posInicial + 1)
            {
                for (int j = posInicial; j <= posFinal; j++)
                {
                    matriz[i][j] = 1;
                }
            }
        }

        // Condição 2: se a linha só tem uma transição de cor e o número de células pretas ainda não é conhecido, o número de células pretas é a metade do número total de células (arredondado para baixo)
        if (numPretoLinha == -1 && numTransLinha == 1)
        {
            int numCelulas = n;
            int numPreto = numCelulas / 2;
            for (int j = 0; j < n; j++)
            {
                if (matriz[i][j] == 0)
                {
                    matriz[i][j] = 1;
                    numPreto--;
                }
                if (numPreto == 0)
                {
                    break;
                }
            }
        }

        // Condição 3: se o número de células pretas já é conhecido e o número de transições de cor é igual a esse número menos 1, a linha pode ser totalmente preta
        if (numPretoLinha != -1 && numTransLinha == numPretoLinha - 1)
        {
            int numCelulas = n;
            int numPreto = numPretoLinha;
            for (int j = 0; j < n; j++)
            {
                if (matriz[i][j] == 0)
                {
                    matriz[i][j] = 1;
                    numPreto--;
                }
                if (numPreto == 0)
                {
                    break;
                }
            }
        }

        // Condição 4: se o número de células pretas já é conhecido e o número de transições de cor é maior do que esse número menos 1, a linha pode ser totalmente branca
        int numBranco = (n * n) - numPretoLinha;
        for (int j = 0; j < n; j++)
        {
            if (matriz[i][j] == 1)
            {
                matriz[i][j] = 0;
                numBranco--;
            }
            if (numBranco == 0)
            {
                break;
            }
        }

        // Condição 5: se o número de células pretas já é conhecido e o número de transições de cor é menor do que a metade desse número, a linha pode ser totalmente branca
        if (lb[i] != -1 && numTransLinha < numPretoLinha / 2)
        {
            for (int j = 0; j < n; j++)
            {
                matriz[i][j] = 0;
            }
        }

        // Condição 6: se o número de células pretas já é conhecido e o número de transições de cor é maior do que a metade desse número, a linha pode ser totalmente preta
        if (numPretoLinha != -1 && numTransLinha > numPretoLinha / 2)
        {
            for (int j = 0; j < n; j++)
            {
                matriz[i][j] = 1;
            }
        }

        // Condição 7: se o número de células pretas em uma linha é igual ao número de células em uma coluna e o número de transições de cor em ambas é igual, as células pretas na linha também são pretas na coluna correspondente
        if (numPretoLinha != -1)
        {
            int colunaCorrespondente = -1;
            int numPretoColuna = -1;
            int numTransColuna = -1;
            for (int j = 0; j < n; j++)
            {
                if (cb[j] == numPretoLinha)
                {
                    colunaCorrespondente = j;
                    numPretoColuna = cb[j];
                    numTransColuna = ct[j];
                    break;
                }
            }
            if (colunaCorrespondente != -1 && numTransLinha == numTransColuna)
            {
                for (int j = 0; j < n; j++)
                {
                    if (matriz[i][j] == 1)
                    {
                        matriz[j][colunaCorrespondente] = 1;
                    }
                }
            }
        }

        // Condição 8: se o número de células pretas em uma linha é igual ao número de células em uma coluna e o número de células pretas em uma das diagonais principais é igual a esse número, as células pretas na linha também são pretas na coluna correspondente
        int numPretoColuna = cb[j];
        int numTransColuna = ct[j];
        if (numPretoLinha == numPretoColuna && numPretoLinha == db[i])
        {
            for (int k = 0; k < n; k++)
            {
                if ((k == i || k == j) && matriz[i][j] == 1)
                {
                    matriz[k][k] = 1;
                }
                if (k != i && k != j)
                {
                    matriz[k][n - 1 - k] = 0;
                }
            }
        }
    }
}

int main()
{
    int n = 5;
    int lb[5] = {1, 1, 1, 1, 1};
    int cb[5] = {1, 1, 1, 1, 1};
    int lt[5] = {2, 2, 2, 2, 2};
    int ct[5] = {2, 2, 2, 2, 2};
    int qb[5] = {2, 2, 2, 2, 2};
    int db = 1;
    int matriz[5][5] = {0};

    descobrir_celulas(n, lb, cb, lt, ct, qb, db, matriz);

    // Imprimir a matriz resultante
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << matriz[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}
