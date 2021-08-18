import subprocess, sys

nthreads = sys.argv[1]
tamanhos = [1e6, 1e7, 1e8]

resultado = []

print(f'{nthreads} threads:')
for n in tamanhos:
    for i in range(5):
        s = subprocess.check_output(f'./a {n} {nthreads}', shell = True)    
        resultado.append(float(s.decode('ascii')))
    print(f'Tempo médio com {n} números: {round(sum(resultado)/len(resultado), 10)} segundos')
