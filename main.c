#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

//calcula mdc
int gcd(int u, int v) { 
    if (v == 0) 
        return u; 
    return gcd(v, u % v); 
}


void friendly_numbers(long int start, long int end) {
    long int last = end - start + 1; // n de elementos no intervalo

    long int *the_num; //nros do intervalo
    the_num = (long int*) malloc(sizeof(long int) * last);
    long int *num; //numeradores simplificados
    num = (long int*) malloc(sizeof(long int) * last);
    long int *den; //denominadores simplificados
    den = (long int*) malloc(sizeof(long int) * last);

    long int i, j, factor, ii, sum, done, n;

        //calcula soma dos divisores para cada numero do intervalo [start,end]
//    #pragma omp parallel shared(num, den, the_num, n) reduction(+:sum)
//    {
//        #pragma omp for
        for (i = start; i <= end; i++) {
            ii = i - start; // indice ajustado pra comecar de 0
            sum = 1 + i; //inicializa soma dos div com 1
            the_num[ii] = i; //numero usado
            done = i; // limite para factor, é atualizado dps
            factor = 2; // fator de divisao

            //testa fatores e soma eles
            while (factor < done){
                if ((i % factor) == 0){ //poderia ser compartilhado
                    sum += (factor + (i / factor));
                    if ((done = i / factor) == factor)
                        sum -= factor;
                }
                factor++;
            }

            num[ii] = sum;
            den[ii] = i;

            //calcula fracao simplificada
                n = gcd(num[ii], den[ii]);
                num[ii] /= n;
                den[ii] /= n;

        }

//    }

    // abaixo funciona
    // outra sugestão: pragma omp for collapse(2) private (i, j).
    // outra sugestão: pragma omp loop
    // 1 threads: 0.001424, 0.001050, 0.001084
    // 2 threads: 0.001070, 0.000908, 0.000983
    // 4 threads: 0.000700, 0.000571, 0.000748
    // 8 threads: 0.000884, 0.000676, 0.000574
       #pragma omp parallel private(i, j) shared(den, num, the_num) num_threads(12)
        {
            #pragma omp for
            for (i = 0; i < last; i++) {
                for (j = i + 1; j < last; j++) {
                    if ((num[i] == num[j]) && (den[i] == den[j]))
                        printf("%ld and %ld are FRIENDLY\n",
                        the_num[i], the_num[j]);
                }
            }
        }

    free(the_num);
    free(num);
    free(den);
}

int main(int argc, char **argv) {
    long int start;
    long int end;
    double timer_start, timer_end;

    while(1){
        scanf("%ld%ld", &start, &end);
        if (start == 0 && end == 0)
            break;
        printf("Number %ld to %ld\n", start, end);

        timer_start = omp_get_wtime();
        friendly_numbers(start, end);
        timer_end = omp_get_wtime();
        printf("Work took %f seconds\n\n", timer_end - timer_start);
    }

    return 0;
}