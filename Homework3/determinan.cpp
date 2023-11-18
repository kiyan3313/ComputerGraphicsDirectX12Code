#include <iostream>
using namespace std;

int determinan(int matrix[4][4]) {
    int det = 1;
 


    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++){
            int factor = matrix[j][i] / matrix[i][i];
            for (int k = 0; k < 4; k++) {
                matrix[j][k] -= factor * matrix[i][k];
            }
        }
    }

    for (int i = 0; i < 4; i++) {
        det *= matrix[i][i];
    }

    return det;
}
int main() {
    int matrix[4][4];
    cout << "Enter a number:" << endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            cin >> matrix[i][j];
        }
    }
    cout << determinan(matrix) << endl;

    return 0;
}