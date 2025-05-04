#include "algebra.h"
#include <stdio.h>
#include <math.h>

Matrix create_matrix(int row, int col)
{
    Matrix m;
    m.rows = row;
    m.cols = col;
    return m;
}

Matrix add_matrix(Matrix a, Matrix b)
{
    // ToDo
    if(a.rows != b.rows || a.cols != b.cols){
        printf("Error: Matrix a and b must have the same rows and cols.\n");
        return create_matrix(0, 0);
    }
    Matrix result = create_matrix(a.rows, a.cols);
    for (int i = 0; i < a.rows; i++){
        for (int j = 0;j < a.cols; j++){
            result.data[i][j] = a.data[i][j] + b.data[i][j];
        }
    }
    return result;
}

Matrix sub_matrix(Matrix a, Matrix b)
{
    // ToDo
    if(a.rows != b.rows || a.cols != b.cols){
        printf("Error: Matrix a and b must have the same rows and cols.\n");
        return create_matrix(0, 0);
    }
    Matrix result = create_matrix(a.rows, a.cols);
    for (int i = 0; i < a.rows; i++){
        for (int j = 0;j < a.cols; j++){
            result.data[i][j] = a.data[i][j] - b.data[i][j];
        }
    }
    return result;
}

Matrix mul_matrix(Matrix a, Matrix b)
{
    // ToDo
    if(a.cols != b.rows){
        printf("Error: The number of cols of matrix a must be equal to the number of rows of matrix b.\n");
        return create_matrix(0, 0);
    }
    Matrix result = create_matrix(a.rows, b.cols);
    for (int i = 0; i < a.rows; i++){
        for (int j = 0;j < a.cols; j++){
            double sum = 0.0;
            for(int k = 0;k < a.cols;k++){
                sum += a.data[i][k] * b.data[k][j];
            }
            result.data[i][j] = sum;
        }
    }
    return result;
}

Matrix scale_matrix(Matrix a, double k)
{
    // ToDo
    Matrix result = create_matrix(a.rows, a.cols);
    for (int i = 0; i < a.rows; i++){
        for (int j = 0;j < a.cols; j++){
            result.data[i][j] = a.data[i][j] * k;
        }
    }
    return result;
}

Matrix transpose_matrix(Matrix a)
{
    // ToDo
    Matrix result = create_matrix(a.cols, a.rows);
    for (int i = 0; i < a.rows; i++){
        for (int j = 0;j < a.cols; j++){
            result.data[j][i] = a.data[i][j];
        }
    }
    return result;
}

double det_matrix(Matrix a)
{
    // ToDo
    if(a.rows != a.cols){
        printf("Error: The matrix must be a square matrix.\n");
        return 0;
    }
    //复制矩阵
    int n = a.rows;
    Matrix tmp = create_matrix(n, n);
    for(int i = 0;i < n;i++){
        for(int j =0;j < n;j++){
            tmp.data[i][j] = a.data[i][j];
        }
    }
    int sign = 1;
    //行列式化成上三角形式
    for(int i = 0;i < n;i++){
        //寻找最大主元
        int max_row = i;
        for(int k = i + 1;k < n;k++){
            if(fabs(tmp.data[k][i]) > fabs(tmp.data[max_row][i])){
                max_row = k;
            }
        }
        //主元为0
        if(fabs(tmp.data[max_row][i] < 1e-12)){
            return 0;
        }
        //交换当前行和最大主元所在行
        if(max_row != i){
            for(int j = 0;j < n;j++){
                double temp = tmp.data[i][j];
                tmp.data[i][j] = tmp.data[max_row][j];
                tmp.data[max_row][j] = temp;
            }
            //行交换，行列式乘-1
            sign *= -1;
        }
        //高斯消元
        for(int k = i + 1;k < n;k++){
            double factor = tmp.data[k][i] / tmp.data[i][i];
            for(int j = i;j < n;j++){
                tmp.data[k][j] -= factor * tmp.data[i][j];
            }
        }
    }
    //计算行列式
    double det = sign;
    for(int i = 0;i < n;i++){
        det *= tmp.data[i][i];
    }
    return det;
}

Matrix inv_matrix(Matrix a)
{
    if(a.rows != a.cols){
        printf("Error: The matrix must be a square matrix.\n");
        return create_matrix(0, 0);
    }
    int n = a.rows;
    double det = det_matrix(a);
    if(fabs(det) < 1e-12){
        printf("Error: The matrix is singular.\n");
        return create_matrix(0, 0);
    }
    //计算伴随矩阵
    Matrix adjugate = create_matrix(n, n);
    for(int i = 0;i < n;i++){
        for(int j = 0;j < n;j++){
            Matrix sub = create_matrix(n-1, n-1);
            for(int si = 0, ai = 0;ai < n;ai++){
                if(ai == i)continue;
                for(int sj = 0, aj = 0; aj < n;aj++){
                    if(aj == j)continue;
                    sub.data[si][sj] = a.data[ai][aj];
                    sj ++;
                }
                si ++;
            }
            //计算余因子
            double sign = ((i + j) % 2 == 0) ? 1.0 : -1.0;
            //转置写入
            adjugate.data[j][i] = sign * det_matrix(sub);
        }
    }
    return scale_matrix(adjugate, 1.0 / det);
}

int rank_matrix(Matrix a)
{
    // 与求行列式类似，找主元、高斯消元，找非0行
    Matrix tmp = create_matrix(a.rows, a.cols);
    for(int i = 0;i < a.rows;i++){
        for(int j = 0;j < a.cols;j++){
            tmp.data[i][j] = a.data[i][j];
        }
    }
    int rank = 0;
    for(int row = 0, col = 0;col < tmp.cols && row < tmp.rows;col++){
        int max_row = row;
        for(int i = row ; i < tmp.rows;i++){
            if(fabs(tmp.data[i][col]) > fabs(tmp.data[max_row][col])){
                max_row = i;
            }
        }
        if(fabs(tmp.data[max_row][col]) < 1e-12){
            continue;
        }
        if(max_row != row){
            for(int j = col;j < tmp.cols;j++){
                double temp = tmp.data[row][j];
                tmp.data[row][j] = tmp.data[max_row][j];
                tmp.data[max_row][j] = temp;
            }
        }
        for(int i = row + 1;i < tmp.rows;i++){
            double factor = tmp.data[i][col] / tmp.data[row][col];
            for(int j = col;j < tmp.cols;j++){
                tmp.data[i][j] -= factor * tmp.data[row][j];
            }
        }
        row++;
        rank++;
    } 
    return rank;
}

double trace_matrix(Matrix a)
{
    // ToDo
    if (a.rows != a.cols){
        printf("Error: The matrix must be a square matrix.\n");
        return 0;
    }
    double trace = 0.0;
    for (int i = 0;i < a.rows;i++){
        trace += a.data[i][i];
    }
    return trace;
}

void print_matrix(Matrix a)
{
    for (int i = 0; i < a.rows; i++)
    {
        for (int j = 0; j < a.cols; j++)
        {
            // 按行打印，每个元素占8个字符的宽度，小数点后保留2位，左对齐
            printf("%-8.2f", a.data[i][j]);
        }
        printf("\n");
    }
}