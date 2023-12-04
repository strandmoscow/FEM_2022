#include <iostream>
#include <vector>
#include <cmath>

double EPS = 1e-16, CUB=0.0001195516;
double X_BEGIN = 0.0;
double X_END = 7.0;
size_t ELEMS_NUM = 20;
double L = (X_END - X_BEGIN) / ELEMS_NUM;

double a = 7.0, B = 6.0, C = 0.0, D = -5.0, usl_left = 10.0, usl_right = -5.0; // au"+Bu'+Cu+D=0

std::vector<double> solve_with_gauss(std::vector<std::vector<double>>& A, std::vector<double>& b){
    size_t row_size = A.size();
    size_t col_size = A.back().size();

    // Прямой ход Гаусса
    double pivot = 0.;
    for (size_t i = 0; i < row_size; i++) {
        for (size_t j = i + 1; j < col_size; j++) {
            if (std::abs(A.at(j).at(i)) < EPS) {
                continue;
            }
            pivot = A.at(j).at(i) / A.at(i).at(i);
            b.at(j) -= pivot * b.at(i);
            for (size_t k = 0; k < row_size; k++) {
                A.at(j).at(k) -= pivot * A.at(i).at(k) ;
            }
        }
    }

    // Обратный ход Гаусса
    std::vector<double> x(row_size);
    for (int i = row_size - 1.; i >= 0; i--) {
        x.at(i) = b.at(i);
        for (size_t j = i + 1; j < row_size; j++) {
            x.at(i) -= x.at(j) * A.at(i).at(j);
        }
        x.at(i) /= A.at(i).at(i);
    }

    return x;
}

double analytical_solution(double x) {
    return 5. / 36. * (6. * x + 49. * exp(6. - (6. * x) / 7.) - 49. * exp(6.) + 72.);
}

std::vector<double> build_analytical_solution(std::vector<double>& x_vec) {
    size_t x_vec_size = x_vec.size();
    std::vector<double> y_vec = std::vector<double>(x_vec_size);
    for (size_t i = 0; i < x_vec_size; i++) {
        y_vec.at(i) = analytical_solution(x_vec.at(i));
    }
    return y_vec;
}

std::vector<double> build_linear_solution(size_t elems_num) {
    double L = (X_END - X_BEGIN) / elems_num;
    size_t size = elems_num + 1;
    std::vector< std::vector<double> > A(size, std::vector<double>(size));
    std::vector<double> b(size);

    // Локальная матрица жесткости для линейного КЭ
    std::vector< std::vector<double> > local_matrix = {
        { a/L - C * L/3.0 + B*1.0/2.0, -a/L - C * L/6.0 - B*1.0/2.0},
        { -a/L - C * L/6.0 + B*1.0/2.0, a/L - C*L/3.0 - B*1.0/2.0},
    };

    // Ансамблирование и получение глобальной матрицы жесткости для линейного КЭ
    for (size_t i = 0; i < elems_num; i++) {
        for (size_t j = 0; j < 2; j++) {
            for (size_t k = 0; k < 2; k++) {
                A.at(i + j).at(i + k) += local_matrix.at(j).at(k);
            }
        }
    }

    for (size_t i = 0; i < size ; i++) {
        b.at(i) = D * L;
    }

    // Учет ГУ
    if ( 0 == 1 ) {
        b.at(0) =  D * L /2. - a*usl_left;
    } else {
        b.at(0) = usl_left;
        A.at(0).at(0) = 1;
        A.at(0).at(1) = 0;
    }

    if ( 1 == 1 ) {
        b.at(size - 1) =  D * L /2. + a*usl_right;
    } else {
        b.at(size - 1) = usl_right;
        A.at(size - 1).at(size - 1) = 1;
        A.at(size - 1).at(size - 2) = 0;
    }

    // Решение полученной СЛАУ методом Гаусса
    std::vector<double> res = solve_with_gauss(A, b);
    return res;
}

std::vector<double> build_cube_solution(size_t elems_num) {
    double L = (X_END - X_BEGIN) / elems_num;
    size_t size = elems_num + 1;
    std::vector< std::vector<double> > A(size,std::vector<double>(size));
    std::vector<double> b(size);
    
    // Локальная матрица жесткости для кубического КЭ
    std::vector< std::vector<double> > local_matrix = {
        {  a*37.0/(10.0*L) - C*8*L/105.0 +B*1.0/2.0,    -a*189.0/(40.0*L) - C*33*L/560.0 - B*57/80.0, a*27.0/(20.0*L) + C*3*L/140.0 + B*3.0/10.0, -a*13.0/(40.0*L) -  C*19.0*L/1680.0 - B*7/80.0},
        { -a*189.0/(40.0*L) - C*33*L/560.0 + B*57/80.0,  a*54.0/(5.0*L)-C*27*L/70.0,                  -a*297.0/(40*L) + C*27*L/560.0 - B*81.0/80.0,    a*27.0/(20.0*L) +  C*3*L/140.0 + B*3.0/10.0},
        {  a*27.0/(20.0*L) + C*3*L/140.0 - B*3.0/10.0,  -a*297.0/(40.0*L) + C*27*L/560.0 + B*81.0/80.0,  a*54.0/(5.0*L) - C*27*L/70.0,                -a*189.0/(40.0*L) - C*33*L/560.0 - B*57/80.0},
        { -a*13.0/(40.0*L) - C*19.0*L/1680.0 + B*7/80.0,   a*27.0/(20.0*L) + C*3*L/140.0 - B*3.0/10.0 , -a*189.0/(40.0*L) - C*33*L/560.0 + B*57/80.0,      a*37.0/(10.0*L) - C*8*L/105.0 - B*1.0/2.0}
    };
    
    // Локальный вектор нагрузок (дополнительные слагаемые для первого и последнего элементов учитываются далее)
    std::vector<double> local_b = { D * L / 8.0,
                                    D * 3.0 * L / 8.0,
                                    D * 3.0 * L / 8.0,
                                    D * L / 8.0 };

    
    // Производим матричные преобразования для обнуления элементов локальной матрицы жесткости, относящихся к внутренним узлам
    for (size_t i = 1; i < 3; i++) {
        for (size_t j = 0; j < 4; j++) {
            if (std::fabs(local_matrix.at(j).at(i)) > EPS && i != j) {
                double  val = local_matrix.at(j).at(i) /local_matrix.at(i).at(i);
                local_b.at(j) -= val * local_b.at(i);
                for (size_t k = 0; k < 4; k++) {
                    local_matrix.at(j).at(k) -= val *local_matrix.at(i).at(k);
                }
            }
            continue;
        }
    }
    
    
    // Исключаем внутренние узлы из рассмотрения
    std::vector< std::vector<double> >  local_matrix_mod  = { { local_matrix.at(0).at(0), local_matrix.at(0).at(3) },
                                                              { local_matrix.at(3).at(0), local_matrix.at(3).at(3) } };
    std::vector<double> local_b_mod = { local_b.at(0), 
                                        local_b.at(3)
                                        };
    
    // Ансамблирование и получение глобальной матрицы жесткости для кубического КЭ
    for (size_t i = 0; i < elems_num; i++) {
        for (size_t j = 0; j < 2; j++) {
            for (size_t k = 0; k < 2; k++) {
                A.at(i + j).at(i + k) += local_matrix_mod.at(j).at(k);
            }
        }
    }

    for (size_t i = 0; i < elems_num; i++) {
        b.at(i) += local_b_mod.at(0);
        b.at(i+1) += local_b_mod.at(1);
    }
       
    // Учет ГУ
    if (0 == 1 ) {
        b.at(0) =  local_b_mod.at(0) - a * usl_left;
    } else {
        b.at(0) = usl_left;
        A.at(0).at(0) = 1.;
        A.at(0).at(1) = 0.;
    }

    if (1 == 1 ) {
        b.at(size - 1) =  local_b_mod.at(1) + a * usl_right;
    } else {
        b.at(size - 1) = usl_right;
        A.at(size - 1).at(size - 1) = 1.;
        A.at(size - 1).at(size - 2) = 0.;
    }
    
    // Решение полученной СЛАУ методом Гаусса
    std::vector<double> res = solve_with_gauss(A, b);
    return res;
}

double calc_abs_error(const std::vector<double>& y_real, const std::vector<double>& y) {
    double max_err = 0.0;
    for (size_t i = 0; i < y_real.size(); i++) {
        double err = std::fabs(y_real.at(i) - y.at(i));
        if (err > max_err) {
            max_err = err;
        }
    }
    return max_err;
}

int main() {
    
    // нахождение количества линейных КЭ
    int N=19000,n=0;
    double err=10;
    std::vector<double> y_r(N + 1);
    std::vector<double> y(N + 1);
    FILE* pogr;
    pogr = fopen("res/labs/text/pogr.txt", "w");
//    while (err>CUB && n<=5000){
//
//        double L = (X_END - X_BEGIN) / N;
//        std::vector<double> x(N + 1);
//        for (size_t i = 0; i < x.size(); i++) {
//            x.at(i) = X_BEGIN + i * L;
//        }
//
//        y = build_linear_solution(N);
//        y_r = build_analytical_solution(x);
//
//        err=calc_abs_error(y_r, y);
//
//        printf("%e - %e = %e: %d\n", calc_abs_error(y_r, y), CUB, calc_abs_error(y_r, y)-CUB, N);
//        N+=1;
//        n+=1;
//    }

    fprintf(pogr, "%d", 555);


    fclose(pogr);
  
    return 0;
}