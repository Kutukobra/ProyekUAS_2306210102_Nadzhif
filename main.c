#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// Fungsi menghitung jumlah line (data) dalam sebuah file dataset
int fileLineCount(FILE* file) {
    char buf[1024];
    int counter = 0;
    for(;;)
    {
        size_t res = fread(buf, 1, 1024, file);
        if (ferror(file))
            return -1;

        int i;
        for(i = 0; i < res; i++)
            if (buf[i] == '\n')
                counter++;

        if (feof(file))
            break;
    }

    rewind(file);

    return counter - 1;
}

double regressionSlope(double x[], double y[], int n) {
    double sum_x = 0.0, sum_y = 0.0, sum_xy = 0.0, sum_xx = 0.0;

    // Sum setiap variabel untuk regresi
    for (int i = 0; i < n; i++) {
        sum_x += x[i];
        sum_y += y[i];
        sum_xy += x[i] * y[i];
        sum_xx += x[i] * x[i];
    }

    // Slope regresi least square
    return (n * sum_xy - sum_x * sum_y) / (n * sum_xx - (sum_x * sum_x));
}

double calculateConductivity(double b) {
    // Konstanta eksperimen
    static const double m = 4.3e-3, x = 3.8e-3, A = 7.76e-3, c = 900, T1 = 26.2, T2 = 49.6;

    return (m * c * x) / (A * (T2 - T1)) * b;
}

// Plot to CSV for python matplotlib script
double plotCSV(FILE *input, double x[], double y[], double b, int count) {
    FILE *pout = fopen("regression_plot.csv", "w");

    char header[100];
    rewind(input);
    fgets(header, sizeof(header), input);

    fprintf(pout, "dt,dT,y_fit\n");
    for (int i = 0; i < count; i++) {
        double dt = x[i], dT = y[i];
        double y_fit = b * dt; // y fit buat aproksimasi berdasarkan regresi
        fprintf(pout, "%lf,%lf,%lf\n", dt, dT, y_fit);
    }
    fclose(pout);
}

int main() {
    FILE *input = fopen("dataset.csv", "r");
    if (input == NULL) {
        printf("Error: Cannot open dataset.csv\n");
        return 1;
    }
    int count = fileLineCount(input);

    // Alokasi memori untuk x y
    double *x = malloc(sizeof(double) * count);
    double *y = malloc(sizeof(double) * count); 
    
    char header[100];
    fgets(header, sizeof(header), input);

    // Input setiap dt dan dT
    for (int i = 0; i < count; i++)     {
        if (fscanf(input, "%lf,%lf", &y[i], &x[i]) != 2) {
            printf("Error reading dataset %d\n", i);
            continue;
        }
    }

    double b = regressionSlope(x, y, count);

    double k = calculateConductivity(b);

    printf("Kemiringan regresi (b = dT/dt)\t:\t %.6f K/s\n", b);
    printf("Konduktivitas termal k\t\t:\t %.6e W/mK\n", k);

    plotCSV(input, x, y, b, count);

    free(x);
    free(y);

    fclose(input);
    return 0;
}