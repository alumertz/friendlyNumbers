#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

// calcula mdc
int gcd(int u, int v) { 
    if (v == 0) 
        return u; 
    return gcd(v, u % v); 
}

void friendly_numbers(long int start, long int end) {
    long int last = end - start + 1; // número de elementos no intervalo
    printf("last = %ld\n", last);

    long int *the_num; // números do intervalo
    the_num = (long int*) malloc(sizeof(long int) * last);
    long int *num; // numeradores simplificados
    num = (long int*) malloc(sizeof(long int) * last);
    long int *den; // denominadores simplificados
    den = (long int*) malloc(sizeof(long int) * last);

    long int i, j, factor, ii, sum, done, n;

    // calcula soma dos divisores para cada número do intervalo [start, end]
    for (i = start; i <= end; i++) {
        ii = i - start; // índice ajustado para começar de 0
        printf("\ni = %ld, ii = %ld\n", i, ii);

        sum = 1 + i; // inicializa soma dos divisores com 1 e o próprio número
        printf("sum (inicial) = %ld\n", sum);

        the_num[ii] = i; // número usado
        done = i; // limite para factor, é atualizado depois
        factor = 2; // fator de divisão

        // testa fatores e soma eles
        while (factor < done) {
            if ((i % factor) == 0) {
                sum += (factor + (i / factor));
                if ((done = i / factor) == factor)
                    sum -= factor;
                printf("factor = %ld, sum = %ld, done = %ld\n", factor, sum, done);
            }
            factor++;
        }
        num[ii] = sum;
        den[ii] = i;

        // calcula fração simplificada
        n = gcd(num[ii], den[ii]);
        printf("gcd(%ld, %ld) = %ld\n", num[ii], den[ii], n);

        num[ii] /= n;
        den[ii] /= n;

        printf("num[%ld] = %ld, den[%ld] = %ld\n", ii, num[ii], ii, den[ii]);
    }

    // verifica números amigáveis
    for (i = 0; i < last; i++) {
        for (j = i + 1; j < last; j++) {
            printf("Comparando num[%ld] = %ld/den[%ld] = %ld com num[%ld] = %ld/den[%ld] = %ld\n",
                   i, num[i], i, den[i], j, num[j], j, den[j]);

            if ((num[i] == num[j]) && (den[i] == den[j])) {
                printf("%ld and %ld are FRIENDLY\n", the_num[i], the_num[j]);
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
    while (1) {
        printf("Digite o intervalo (start end): ");
        scanf("%ld%ld", &start, &end);
        if (start == 0 && end == 0)
            break;
        printf("Intervalo: %ld to %ld\n", start, end);
        friendly_numbers(start, end);
    }
    return 0;
}
