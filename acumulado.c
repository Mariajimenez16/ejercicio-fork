#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid;
    int numero = 10;
    int acumulado = 0;
    int pipes[3][2];

    // Crear pipes
    for (int i = 0; i < 3; i++) {
        pipe(pipes[i]);
    }

    for (int i = 0; i < 3; i++) {
        pid = fork();

        if (pid < 0) {
            printf("Error al crear el proceso hijo\n");
            exit(1);
        }

        else if (pid == 0) { // HIJO
            close(pipes[i][0]); // cerrar lectura

            int parte = numero / 3;
            int inicio = i * parte + 1;
            int fin = (i == 2) ? numero : (i + 1) * parte;
            int suma_parcial = 0;

            for (int j = inicio; j <= fin; j++) {
                suma_parcial += j;
            }

            printf("Hijo %d suma parcial de %d a %d = %d\n",
                   i + 1, inicio, fin, suma_parcial);

            write(pipes[i][1], &suma_parcial, sizeof(suma_parcial));
            close(pipes[i][1]);
            exit(0);
        }
    }

    // PADRE, uso de IA en esta parte del padre para acumular las sumas parciales de los hijos
    int suma;

    for (int i = 0; i < 3; i++) {
        close(pipes[i][1]); // cerrar escritura
        read(pipes[i][0], &suma, sizeof(suma));
        acumulado += suma;
        close(pipes[i][0]);
        wait(NULL);
    }

    printf("Padre: la suma total es %d\n", acumulado);
    return 0;
}
