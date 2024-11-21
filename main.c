#include <stdio.h>
#include <stdlib.h>
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
    #pragma omp parallel for
        //calcula soma dos divisores para cada numero do intervalo [start,end]
        for (i = start; i <= end; i++) {
            ii = i - start; // indice ajustado pra comecar de 0
            sum = 1 + i; //inicializa soma dos div com 1
            the_num[ii] = i; //numero usado
            done = i; // limite para factor, é atualizado dps
            factor = 2; // fator de divisao

            //testa fatores e soma eles
            while (factor < done){
                if ((i % factor) == 0){
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
    #pragma omp for collapse(2)
        for (i = 0; i < last; i++) {
            for (j = i + 1; j < last; j++) {
                if ((num[i] == num[j]) && (den[i] == den[j]))
                    printf("%ld and %ld are FRIENDLY\n",
                    the_num[i], the_num[j]);
            }
        }

    free(the_num);
    free(num);
    free(den);
}
//}

int main(int argc, char **argv) {
    double itime, ftime, exec_time;
    itime = omp_get_wtime();

    long int start;
    long int end;
    while(1){
        scanf("%ld%ld", &start, &end);
        if (start == 0 && end == 0)
            break;
        printf("Number %ld to %ld\n", start, end);
        friendly_numbers(start, end);

    }

    ftime = omp_get_wtime();
    exec_time = ftime - itime;
    printf("\n\nTime taken is %f", exec_time);
    return 0;
}