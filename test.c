#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <math.h>

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
    #pragma omp parallel for private(ii, sum, done, factor, n) shared(the_num, num, den, start, end)
    for (i = start; i <= end; i++) {
        ii = i - start; // indice ajustado pra comecar de 0
        sum = 1; //inicializa soma dos div com 1
        the_num[ii] = i; //numero usado
        done = (int)sqrt(i);

        //testa fatores e soma eles
       #pragma omp parallel for reduction(+:sum)
        for (int factor = 2; factor <= done; factor++) {
            if (i % factor == 0) {
                sum += factor;
                if (factor != i / factor) {
                    sum += i / factor;
                }
            }
        }
        num[ii] = sum;
        den[ii] = i;

        //calcula fracao simplificada
        n = gcd(num[ii], den[ii]);
        num[ii] /= n;
        den[ii] /= n;
    }

    #pragma omp parallel private(i, j) shared(den, num, the_num)
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
    while(1){
        scanf("%ld%ld", &start, &end);
        if (start == 0 && end == 0)
            break;
        printf("Number %ld to %ld\n", start, end);
        friendly_numbers(start, end);

    }
    return 0;
}