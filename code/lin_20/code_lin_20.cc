#include <iostream>
#include <vector>
#include <cmath>

double EPS = 1e-16;
double X_BEGIN = 1.0;
double X_END = 32.0;
size_t ELEMS_NUM = 20;
double L = (X_END - X_BEGIN) / ELEMS_NUM;

double a = 4.0, B = 0.0, C = -11.0, D = 7.0, usl_left = -10.0, usl_right = 5.0; // au"+Bu'+Cu+D=0

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
    return (exp(-1. / 2. * sqrt(11.) * (x + 1.)) * (-117. * exp(sqrt(11.) * x) + 7. * exp(1. / 2. * sqrt(11.) * (x + 1.)) + 7. * exp(1. / 2. * sqrt(11.) * (x + 63.)) + 10. * sqrt(11.) * exp(1. / 2. * sqrt(11.) * (2. * x + 31.)) - 10. * sqrt(11.) * exp((33. * sqrt(11.))/2.) - 117. * exp(32. * sqrt(11.))))/(11. * (1. + exp(31. * sqrt(11.))));
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

     std::vector<double> x(ELEMS_NUM + 1);
     for (size_t i = 0; i < x.size(); i++) {
         x.at(i) = X_BEGIN + i * L;
     }
     size_t x_size = x.size();

    std::vector<double> y;
    if (true) {
        y = build_linear_solution(ELEMS_NUM);
    } else {
        y = build_cube_solution(ELEMS_NUM);
    }
     std::vector<double> y_real = build_analytical_solution(x);
    

     FILE* gp;
     FILE* ab;
     FILE* pgr;
     FILE* tab;
     if (true) {
        if(ELEMS_NUM == 20) {
            gp = fopen("res/labs/text/graph/lin_20.txt", "w");
            ab = fopen("res/labs/text/graph/abs.txt", "w");
            for (size_t i = 0; i < x_size; i++) {
                fprintf(ab, "%lf %lf\n", x.at(i), y_real.at(i));
            }
            pgr = fopen("res/labs/text/pgr/lin_20.txt", "w");
            tab = fopen("res/labs/text/tab/lin_20.txt", "w");
        }
        if(ELEMS_NUM == 40) {
            gp = fopen("res/labs/text/graph/lin_40.txt", "w");
            pgr = fopen("res/labs/text/pgr/lin_40.txt", "w");
            tab = fopen("res/labs/text/tab/lin_40.txt", "w");
        }
     } else {
        if(ELEMS_NUM == 20) {
            gp = fopen("res/labs/text/graph/cub_20.txt", "w");
            pgr = fopen("res/labs/text/pgr/cub_20.txt", "w");
            tab = fopen("res/labs/text/tab/cub_20.txt", "w");
        }
        if(ELEMS_NUM == 40) {
            gp = fopen("res/labs/text/graph/cub_40.txt", "w");
            pgr = fopen("res/labs/text/pgr/cub_40.txt", "w");
            tab = fopen("res/labs/text/tab/cub_40.txt", "w");
        }
     }

     for (size_t i = 0; i < x.size()-1; i++) {
        fprintf(tab, "%lf & %lf & %lf & %e \\\\\n", x.at(i), y_real.at(i), y.at(i), std::fabs(y_real.at(i) - y.at(i)));
     }
     fprintf(tab, "%lf & %lf & %lf & %e", x.at(x.size()-1), y_real.at(x.size()-1), y.at(x.size()-1), std::fabs(y_real.at(x.size()-1) - y.at(x.size()-1)));

     for (size_t i = 0; i < x_size; i++) {
         fprintf(gp, "%lf %lf\n", x.at(i), y.at(i));
     }

     fprintf(pgr, "%e", calc_abs_error(y_real, y));
     fclose(gp);
     fclose(ab);
     fclose(pgr);
     fclose(tab);

     return 0;
}