/**
 * Nom:     Oriol
 * Cognoms: Benítez Bravo
 * NIU:     1566931
 *
 * Nom:     Oliver Einard
 * Cognoms: Tarragó Boardman
 * NIU:     1527541
 */

#include <stdio.h>
#include <stdlib.h>

typedef enum {SUCCESS, NO_FILE, EMPTY_FILE, NO_MEMORY} RETURN;

typedef struct alu {
    int niu;
    float notes[5];
    struct alu *seg;
} Alu;

float mitjana(const float *dades, int n);
void imprimirllista(const Alu *llista_alumnes);
void llegir_alumne(FILE *des_de, Alu *alumne);

int main() {
    int length = 0;

    FILE *fitxer_dades = fopen("Llista.txt", "r");
    if (fitxer_dades == NULL) {
        printf("\nNo s'ha accedit al fitxer de dades\n");
        return NO_FILE;
    }

    int niu;
    if (fscanf(fitxer_dades, "%i;", &niu) == EOF) {
        printf("\nFitxer buit\n");
        return EMPTY_FILE;
    }

    Alu *primer = (Alu *) malloc(sizeof(Alu));
    if (primer == NULL) {
        printf("Problema assignant espai de memoria\n");
        return NO_MEMORY;
    }
    ++length;
    primer->niu = niu;
    llegir_alumne(fitxer_dades, primer);
    primer->notes[4] = mitjana(primer->notes, 4);

    Alu *anterior;
    while (fscanf(fitxer_dades, "%i;", &niu) != EOF) {
        Alu *actual = (Alu *) malloc(sizeof(Alu));
        if (actual == NULL) {
            printf("Problema assignant espai de memoria\n");
            return NO_MEMORY;
        }

        length++;
        actual->niu = niu;
        llegir_alumne(fitxer_dades, actual);
        actual->notes[4] = mitjana(actual->notes, 4);

        if (actual->niu < primer->niu) {
            actual->seg = primer;
            primer = actual;
        } else {
            anterior = primer;
            while (anterior->seg != NULL && (anterior->seg)->niu < actual->niu) {
                anterior = anterior->seg;
            }
            actual->seg = anterior->seg;
            anterior->seg = actual;
        }
    }
    fclose(fitxer_dades);

    imprimirllista(primer);

    printf("\nS'ha llegit informacio de %d linies.\n\n", length);
    return EXIT_SUCCESS;
}

void llegir_alumne(FILE *des_de, Alu *alumne) {
    for (int i = 0; i < 4; i++) {
        fscanf(des_de, "%f", &alumne->notes[i]);
        fgetc(des_de);//llegeix i descarta els ; i el \n
    }
}

void imprimirllista(const Alu *llista_alumnes) {
    while (llista_alumnes != NULL) {
        printf("%d | ", llista_alumnes->niu);
        for (int i = 0; i < 4; i++) {
            printf("%5.1f", llista_alumnes->notes[i]);
        }
        printf(" |%6.2f", llista_alumnes->notes[4]);
        printf("\n");
        llista_alumnes = llista_alumnes->seg;
    }
}

float mitjana(const float *dades, int n) {
    float m = 0.f;
    for (int i = 0; i < n; i++) {
        m += dades[i];
    }

    return  m / (float) n;
}
