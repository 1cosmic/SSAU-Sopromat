#include <stdio.h>
#include <math.h>
#include <unistd.h>

#define MAX_PLOT 5


// consts.
long long E;
double alfa;  
short deltaT;
float sigma;

float conv_A(float Ai) {
    // Convert Ai in 0.000...
    return Ai * pow(10, -4);
}

float conv_N(float Ni) {
    return Ni * pow(10, 3);
}

float conv_sigma() {
    return sigma * pow(10, -3);
}

void get_sigma(float Sigma[], float N[], float A[]) {

    // Sigmas block.
    printf("Calculate sigma...\n");

    for(int i = 1; i < MAX_PLOT; ++i) {
        Sigma[i] = N[i] / conv_A(A[i]);

        printf("Sigma #%i: %0.1f Pa. Convert: %.2f MPa.\n", i, Sigma[i], Sigma[i] / 1000);
    }
}

void get_U(double U[], float N[], float A[], float l[]) {
    // Расчитывает  U на каждом из участков (изменение длины).

    double temp_NiLiAi, temp_ATL;
    double sum_U = 0;

    printf("\nCalculate U...\n");

    // Calculate and write results.
    for (int i = 1; i < MAX_PLOT; ++i) {
        
        temp_NiLiAi = (conv_N(N[i]) * l[i]) / (E * conv_A(A[i]));
        temp_ATL = alfa * deltaT * l[i];
        U[i] = (temp_NiLiAi + temp_ATL) * 1000;

        printf("U #%i: %.10f mm. | ", i, U[i]);
        printf("Sum U%i: %f + %f = ", i, sum_U, U[i]);
        sum_U += U[i];
        printf("%f mm.\n", sum_U);

        // printf("I: %i | N %i, l %f, E %lli, A %f, alfa %.10f, delta %i", i, conv_N(N[i]), l[i], E, conv_A(A[i]), alfa, deltaT);
    }
    printf("###################\n");
    printf("Sum U = %0.2f\n", sum_U);
}

double get_Rb(float N[], float A[], float l[]) {
    double Rb;

    long double const_SAdT, sum_LiFi_Ai = 0, sum_Li_Ai = 0;
    float sum_Li = 0;

    printf("\nCalculate to Rb...\n");

    // Calculate and fill in params.
    for (int i = 1; i < 5; ++i) {

        sum_Li += l[i];  // sum all l(1-6).
        sum_Li_Ai += (l[i] / conv_A(A[i]));  // sum all divided (Li / Ai).
        sum_LiFi_Ai += ((l[i] * conv_N(N[i])) / conv_A(A[i]));  // sum all devided (Li * Fi / Ai).
    }
    // Calculate right of part the equelaion.
    const_SAdT = (conv_sigma() - (alfa * deltaT * sum_Li)) * E;

    // Getting result of calculate.
    Rb = ((sum_LiFi_Ai - const_SAdT) / sum_Li_Ai) / 1000;
    
    printf("###################\n");
    printf("Your result: %0.2f kN.\n", Rb );

    return Rb;
}

void crt_N(float Ni[], float N[], double Rb) { 

    // Generate N, from second task. ITS MEGA CYKLE BLYAAAAAT!
    // V CHEM CILA, BRAT?
    // ... In C!!

    printf("\nCalculate N, from second task...\n");

    for (int i = 1; i < 5; ++i) {
        Ni[i] = N[i] - Rb;
        printf("N%i: %0.2f\n", i, Ni[i]);
    }

    printf("###################\n");
    printf("Done!\n");
}

// Ввод пользовательских значений для расчёта.
void user_input() {
    // float N[], float A[], float l[]
    puts("Приветствую, мой друг! Это настоящий МАСТХЭФ для сопромата.");
    sleep(3);
    
    puts("Сейчас тебе будет предложено ввести данные. Заполняй всё внимательно!");
    sleep(3);
    
    puts("\nПрограмма написана мной в течение 2х дней, а акцент ставился на расчёты.");
    puts("Поэтому если что-то выпадает в ошибку, просто перезапусти код.");
    sleep(5);



}

int main() {
    // Const.
    E = 2 * pow(10, 11);  
    alfa = 125 * pow(10, -7);
    sigma = 0.2f;
    deltaT = 20;

    float N[] = {0, 400.0, 200.0, 200.0, 500.0, 500.0, 0};
    float A[] = {0, 78.5f, 78.5f, 28.26f, 28.26f, 12.56f, 12.56f};
    float l[] = {0, 0.75f, 0.75f, 0.5f, 0.5f, 0.25f, 0.25f};

    // // CHECK OUT VALUES.
    // float N2[] = {0, 200.0, 100.0, 100.0, 0};
    // float A2[] = {0, 20.0f, 20.0f, 10.0f, 10.0f};
    // float l2[] = {0, 0.5f, 0.5f, 1.0f, 1.0f};    

    float sigma[MAX_PLOT] = {0};
    double U[MAX_PLOT] = {0};

    float second_N[MAX_PLOT] = {0};
    float sigma2[MAX_PLOT] = {0};    
    double U2[MAX_PLOT] = {0};
    double Rb, Rb2;  // Rb2 для сверки верности кода по заранее реш. задаче. 

    int i;

    user_input();

    // Решение первой задачи.
    get_sigma(sigma, N, A);  // вычисляем сигму участков.
    get_U(U, N, A, l);  // вычисляем изменение длин участков.

    // Решение второй задачи.
    printf("\n\nTASK NUMBER OF TWO.\n\n");
    Rb = get_Rb(N, A, l);  // вычисляем Rb с учётом упорной поверхности.
    crt_N(second_N, N, Rb);  // вычисляем новые силы, действ. на брус.
    get_sigma(sigma2, second_N, A);  // вычисляем сигму участков.
    get_U(U2, second_N, A, l);  // вычисляем изменения длины каждого участка.


    // Чек зе саунд, андеграунд
    // Rb2 = get_Rb(N2, A2, l2);
    // crt_N(second_N, N2, Rb2);
    // get_U(U2, second_N, A2, l2);

    return 0;
}