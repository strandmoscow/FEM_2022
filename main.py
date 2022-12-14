import os
import numpy as np
from jinja2 import Environment, FileSystemLoader
import matplotlib.pyplot as plt

author_name="{Гаспарян Грета Андраниковна}"
author_name_short="{Гаспарян Г.А.}"
group="{РК6-73б}"
variant="{49}"

a = 40.0
b = 0.0
c = -4.0
d = 11.0

min_int = 0.0
min_cond_type = 1
min_value = -2.0

max_int = 17.0
max_cond_type = 0
max_value = 6.0


an_sol_for_c = "(exp(-x/sqrt(10)) * (-8 * sqrt(10) * exp(sqrt(0.4) * x) + 11 * exp(x/sqrt(10)) + 11 * exp((x + 34.)/sqrt(10)) + 13 * exp((2. * x + 17)/sqrt(10)) + 8 * sqrt(10) * exp(17. * sqrt(0.4)) + 13. * exp(17./sqrt(10))))/(4. * (1. + exp(17. * sqrt(0.4))))"
analytical_solution = "\\frac{e^{-x / \\sqrt{10}}\\left(-8 \\sqrt{10} e^{\\sqrt{2 / 5} x}+11 e^{x / \\sqrt{10}}+11 e^{(x+34) / \\sqrt{10}}+13 e^{(2 x+17) / \\sqrt{10}}+ 8 \\sqrt{10} e^{17 \\sqrt{2 / 5}}+13 e^{17 / \\sqrt{10}}\\right)}{4\\left(1+e^{17 \\sqrt{2 / 5}}\\right)}"


# Jinja init

environment = Environment(
    loader=FileSystemLoader("templates/")
)

base_template = environment.get_template("labs/educmm_txb_COMPMATHLAB-Solution_N_M/educmm_lab_Variant_N_M-id.tex")

base_res_file_name = f"res/labs/educmm_txb_COMPMATHLAB-Solution_N_M/educmm_lab_Variant_N_M-id.tex"
base_text = base_template.render(
    author_name=author_name,
    author_name_short=author_name_short,
    group=group,
    variant=variant
)

code_tempate_lin = environment.get_template("code/lin.cc")

code_file_name_lin_20 = f"c code/lin_20/code_lin_20.cc"
code_lin_20 = code_tempate_lin.render(
    elem=20,
    a=a,
    b=b,
    c=c,
    d=d,
    min_int=min_int,
    min_cond_type=min_cond_type,
    min_value=min_value,
    max_int=max_int,
    max_cond_type=max_cond_type,
    max_value=max_value,
    lin = "true",
    an_sol_for_c = an_sol_for_c
)

code_file_name_lin_40 = f"c code/lin_40/code_lin_40.cc"
code_lin_40 = code_tempate_lin.render(
    elem=40,
    a=a,
    b=b,
    c=c,
    d=d,
    min_int=min_int,
    min_cond_type=min_cond_type,
    min_value=min_value,
    max_int=max_int,
    max_cond_type=max_cond_type,
    max_value=max_value,
    lin = "true",
    an_sol_for_c = an_sol_for_c
)

code_file_name_cub_20 = f"c code/cub_20/code_cub_20.cc"
code_cub_20 = code_tempate_lin.render(
    elem=20,
    a=a,
    b=b,
    c=c,
    d=d,
    min_int=min_int,
    min_cond_type=min_cond_type,
    min_value=min_value,
    max_int=max_int,
    max_cond_type=max_cond_type,
    max_value=max_value,
    lin = "false",
    an_sol_for_c = an_sol_for_c
)

code_file_name_cub_40 = f"c code/cub_40/code_cub_40.cc"
code_cub_40 = code_tempate_lin.render(
    elem=40,
    a=a,
    b=b,
    c=c,
    d=d,
    min_int=min_int,
    min_cond_type=min_cond_type,
    min_value=min_value,
    max_int=max_int,
    max_cond_type=max_cond_type,
    max_value=max_value,
    lin="false",
    an_sol_for_c=an_sol_for_c
)

with open(base_res_file_name, mode="w+", encoding="utf-8") as base:
    base.write(base_text)
    print(f"... wrote {base_res_file_name}")

with open(code_file_name_lin_20, mode="w+", encoding="utf-8") as code_lin_20_file:
    code_lin_20_file.write(code_lin_20)
    print(f"... wrote {code_file_name_lin_20}")

with open(code_file_name_lin_40, mode="w+", encoding="utf-8") as code_lin_40_file:
    code_lin_40_file.write(code_lin_40)
    print(f"... wrote {code_file_name_lin_40}")

with open(code_file_name_cub_20, mode="w+", encoding="utf-8") as code_cub_20_file:
    code_cub_20_file.write(code_cub_20)
    print(f"... wrote {code_file_name_cub_20}")

with open(code_file_name_cub_40, mode="w+", encoding="utf-8") as code_cub_40_file:
    code_cub_40_file.write(code_cub_40)
    print(f"... wrote {code_file_name_cub_40}")

os.system("g++ -o c20.out '/Users/andrejstrelcenko/Library/Mobile Documents/com~apple~CloudDocs/Бомонка/7 семестр/Модели и методы анализа проектных решений/ДЗ2/doc auto/c code/cub_20/code_cub_20.cc' -std=c++11")
os.system("mv c20.out '/Users/andrejstrelcenko/Library/Mobile Documents/com~apple~CloudDocs/Бомонка/7 семестр/Модели и методы анализа проектных решений/ДЗ2/doc auto/c code/cub_20'")

os.system("g++ -o c40.out '/Users/andrejstrelcenko/Library/Mobile Documents/com~apple~CloudDocs/Бомонка/7 семестр/Модели и методы анализа проектных решений/ДЗ2/doc auto/c code/cub_40/code_cub_40.cc' -std=c++11")
os.system("mv c40.out '/Users/andrejstrelcenko/Library/Mobile Documents/com~apple~CloudDocs/Бомонка/7 семестр/Модели и методы анализа проектных решений/ДЗ2/doc auto/c code/cub_40'")

os.system("g++ -o l20.out '/Users/andrejstrelcenko/Library/Mobile Documents/com~apple~CloudDocs/Бомонка/7 семестр/Модели и методы анализа проектных решений/ДЗ2/doc auto/c code/lin_20/code_lin_20.cc' -std=c++11")
os.system("mv l20.out '/Users/andrejstrelcenko/Library/Mobile Documents/com~apple~CloudDocs/Бомонка/7 семестр/Модели и методы анализа проектных решений/ДЗ2/doc auto/c code/lin_20'")

os.system("g++ -o l40.out '/Users/andrejstrelcenko/Library/Mobile Documents/com~apple~CloudDocs/Бомонка/7 семестр/Модели и методы анализа проектных решений/ДЗ2/doc auto/c code/lin_40/code_lin_40.cc' -std=c++11")
os.system("mv l40.out '/Users/andrejstrelcenko/Library/Mobile Documents/com~apple~CloudDocs/Бомонка/7 семестр/Модели и методы анализа проектных решений/ДЗ2/doc auto/c code/lin_40'")

os.system("'/Users/andrejstrelcenko/Library/Mobile Documents/com~apple~CloudDocs/Бомонка/7 семестр/Модели и методы анализа проектных решений/ДЗ2/doc auto/c code/cub_20/c20.out'")
os.system("'/Users/andrejstrelcenko/Library/Mobile Documents/com~apple~CloudDocs/Бомонка/7 семестр/Модели и методы анализа проектных решений/ДЗ2/doc auto/c code/cub_40/c40.out'")
os.system("'/Users/andrejstrelcenko/Library/Mobile Documents/com~apple~CloudDocs/Бомонка/7 семестр/Модели и методы анализа проектных решений/ДЗ2/doc auto/c code/lin_20/l20.out'")
os.system("'/Users/andrejstrelcenko/Library/Mobile Documents/com~apple~CloudDocs/Бомонка/7 семестр/Модели и методы анализа проектных решений/ДЗ2/doc auto/c code/lin_40/l40.out'")

# Matplot
fig, ax = plt.subplots()


absolute = np.loadtxt('res/labs/text/graph/abs.txt')
x_abs = absolute[:, 0]
y_abs = absolute[:, 1]

# "lin_20.txt", "lin_40.txt", "cub_20.txt", "cub_40.txt"
result_files = [{'name': "cub_20.txt", 'save_to': "res/labs/img/cub/20.png"},
                {'name': "cub_40.txt", 'save_to': "res/labs/img/cub/40.png"},
                {'name': "lin_20.txt", 'save_to': "res/labs/img/lin/20.png"},
                {'name': "lin_40.txt", 'save_to': "res/labs/img/lin/40.png"}]

for file in result_files:
    absolute = np.loadtxt('res/labs/text/graph/' + file['name'])

    x = absolute[:, 0]
    y = absolute[:, 1]
    ax.set(xlabel='X', ylabel='U')

    ax.plot(x, y, 'r-')
    ax.plot(x_abs, y_abs, 'b-')
    ax.grid()
    fig.savefig(file['save_to'])
    plt.cla()

with open('res/labs/text/pgr/cub_20.txt', 'r') as f:
    pogr_num = float(f.read())

code_tempate_pogr = environment.get_template("code/pogr.cc")

code_file_name_pogr = f"c code/pogr/pogr.cc"
code_pogr = code_tempate_pogr.render(
    elem=20,
    a=a,
    b=b,
    c=c,
    d=d,
    min_int=min_int,
    min_cond_type=min_cond_type,
    min_value=min_value,
    max_int=max_int,
    max_cond_type=max_cond_type,
    max_value=max_value,
    lin = "true",
    an_sol_for_c = an_sol_for_c,
    cube=pogr_num
)

with open(code_file_name_pogr, mode="w+", encoding="utf-8") as code_pogr_file:
    code_pogr_file.write(code_pogr)
    print(f"... wrote {code_file_name_pogr}")

os.system("g++ -o pogr.out '/Users/andrejstrelcenko/Library/Mobile Documents/com~apple~CloudDocs/Бомонка/7 семестр/Модели и методы анализа проектных решений/ДЗ2/doc auto/c code/pogr/pogr.cc' -std=c++11")
os.system("mv pogr.out '/Users/andrejstrelcenko/Library/Mobile Documents/com~apple~CloudDocs/Бомонка/7 семестр/Модели и методы анализа проектных решений/ДЗ2/doc auto/c code/pogr'")
os.system("'/Users/andrejstrelcenko/Library/Mobile Documents/com~apple~CloudDocs/Бомонка/7 семестр/Модели и методы анализа проектных решений/ДЗ2/doc auto/c code/pogr/pogr.out'")


base_template = environment.get_template("labs/educmm_txb_COMPMATHLAB-Solution_N_M.tex")

latex_text_file_name = f"res/labs/educmm_txb_COMPMATHLAB-Solution_N_M.tex"
latex_text = base_template.render(
    a=a,
    a_int=int(a),
    a_abs=abs(int(a)),
    b=b,
    b_int=int(b),
    b_abs=abs(int(b)),
    c=c,
    c_int=int(c),
    c_abs=abs(int(c)),
    d=d,
    d_int=int(d),
    d_abs=abs(int(d)),
    min_int = min_int,
    min_int_int = int(min_int),
    min_cond_type = min_cond_type,
    min_value = min_value,
    min_value_int = int(min_value),
    max_int = max_int,
    max_int_int = int(max_int),
    max_cond_type = max_cond_type,
    max_value = max_value,
    max_value_int = int(max_value),
    analytical_solution = analytical_solution,
    an_sol_for_c = an_sol_for_c,
    code=code_lin_20
)


with open(latex_text_file_name, mode="w+", encoding="utf-8") as tex:
    tex.write(latex_text)
    print(f"... wrote {latex_text_file_name}")