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

typedef enum { SUCCESS, NO_FILE, EMPTY_FILE, NO_MEMORY } RETURN;

typedef struct alu {
    int niu;
    float notes[5];
    struct alu *seg;
} Alu;

/**
 * Assigna memòria per a un Alu i inicialitza el punter seg a NULL
 * @return Punter a memòria de mida sizeof(Alu) si s'ha pogut assignar, NULL sino.
 */
Alu *new_Alu();

float mitjana(const float *dades, int n);

void imprimirllista(const Alu *llista_alumnes);

void llegir_alumne(FILE *des_de, Alu *alumne);

/**
 * Insereix un alumne a una llista ordenada d'alumnes mantenint un ordre creixent
 * @param alumne Alumne a inserir
 * @param llista_ordenada Llista d'alumnes on inserir
 * @return Un punter al primer element de la llista
 */
Alu *inserir(Alu *alumne, Alu *llista_ordenada);

/**
 * Versió d'inserir fent servir recursivitat. És més ràpid però pot haver un stack overflow si la llista és massa
 * gran. (> 100k elements)
 *
 * @see Alu *inserir(Alu *alumne, Alu *llista_ordenada)
 *
 * @param alumne Alumne a inserir
 * @param llista_ordenada Llista d'alumnes on inserir
 * @return Un punter al primer element de la llista
 */
Alu *inserir_rec(Alu *alumne, Alu *llista_ordenada);

/**
 * Esborra un alumne de una llista d'alumnes
 * @param niu NIU de l'alumne a esborrar
 * @param llista_alumnes Llista d'alumnes
 * @return Punter al primer element de la llista. Si l'alumne esborrat era el primer, serà un punter al segón element
 * de la llista passada per argument, o NULL si l'element esborrat era l'únic element de la llista.
 */
Alu *esborrar(int niu, Alu *llista_alumnes);

/**
 * Versió d'esborrar fent servir recursivitat. En aquest cas, es més efectiva la versió sense recursivitat.
 *
 * @see Alu *esborrar(int niu, Alu *llista_alumnes)
 *
 * @param niu
 * @param llista_alumnes
 * @return
 */
Alu *esborrar_rec(int niu, Alu *llista_alumnes);

void demanar_alumne(Alu *alumne);

int menu();

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

    Alu *primer = new_Alu();
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
        Alu *actual = new_Alu();
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
    printf("\nDades del fitxer llegides.\n");

    int opcio;
    while ((opcio = menu()) != 3) {
        if (opcio == 1) {
            Alu *alumne = new_Alu();
            if (alumne == NULL) {
                printf("Problema assignant espai de memoria\n");
                return NO_MEMORY;
            }
            demanar_alumne(alumne);
            inserir_rec(alumne, primer);
            ++length;
        } else if (opcio == 2) {
            int niu;
            printf("Introdueix el niu de l'alumne a esborrar: ");
            scanf("%d", &niu);
            primer = esborrar(niu, primer);
            --length;
        }
    }
    imprimirllista(primer);

    printf("\nS'ha llegit informacio de %d linies.\n\n", length);
    return EXIT_SUCCESS;
}

Alu *new_Alu() {
    Alu *alu = (Alu*) malloc(sizeof(Alu));
    if (alu != NULL) {
        alu->seg = NULL;
    }
    return alu;
}

int menu() {
    printf("\n1. Insertar alumne.\n"
           "2. Esborrar alumne.\n"
           "3. Llistar i acabar.\n"
           "Introdueix una opció: ");
    int opcio = -1;
    while (opcio < 1 || opcio > 3) {
        scanf("%d", &opcio);
    }
    printf("\n");
    return opcio;
}

void llegir_alumne(FILE *des_de, Alu *alumne) {
    for (int i = 0; i < 4; i++) {
        fscanf(des_de, "%f", &alumne->notes[i]);
        fgetc(des_de);//llegeix i descarta els ; i el \n
    }
}

Alu *inserir(Alu *alumne, Alu *llista_ordenada) {
    Alu *actual = llista_ordenada, *anterior = NULL;
    while (actual->seg != NULL && actual->niu < alumne->niu) {
        anterior = actual;
        actual = actual->seg;
    }
    if (actual->niu < alumne->niu) {
        if (actual->seg != NULL) {
            alumne->seg = actual->seg;
        }
        actual->seg = alumne;
    } else {
        if (actual == llista_ordenada) {
            alumne->seg = llista_ordenada;
            llista_ordenada = alumne; // alumne te el niu més petit de tots, i será el nou principi de la llista
        } else {
            anterior->seg = alumne; // si actual != llista_ordenada llavors anterior != NULL
            alumne->seg = actual;
        }
    }
    return llista_ordenada;
}

Alu *inserir_rec(Alu *alumne, Alu *llista_ordenada) {
    /** I a part de més ràpid, és més simple */
    if (alumne->niu < llista_ordenada->niu) {
        alumne->seg = llista_ordenada;
        return alumne;
    } else {
        llista_ordenada->seg = llista_ordenada->seg != NULL ? inserir_rec(alumne, llista_ordenada->seg) : alumne;
        return llista_ordenada;
    }
}

Alu *esborrar(int niu, Alu *llista_alumnes) {
    Alu *actual = llista_alumnes, *anterior = NULL;
    while (actual != NULL && actual->niu != niu) {
        anterior = actual;
        actual = actual->seg;
    }
    if (actual != NULL) { // And then actual->niu == niu
        if (anterior == NULL) { // And then actual == llista_alumnes, that is, actual is the first element
            llista_alumnes = llista_alumnes->seg;
        } else {
            anterior->seg = actual->seg;
        }
        free(actual);
    }
    return llista_alumnes;
}

Alu *esborrar_rec(int niu, Alu *llista_alumnes) {
    if (llista_alumnes != NULL) {
        if (llista_alumnes->niu == niu) {
            Alu *seg = llista_alumnes->seg;
            free(llista_alumnes);
            llista_alumnes = seg;
        } else if (llista_alumnes->seg != NULL) {
            llista_alumnes->seg = esborrar_rec(niu, llista_alumnes->seg);
        }
    }
    return llista_alumnes;
}

void demanar_alumne(Alu *alumne) {
    printf("\nIntrodueix separat per comes: niu, nota1, nota2, nota3, nota4: ");
    scanf("%d, %f, %f, %f, %f", &alumne->niu, &alumne->notes[0], &alumne->notes[1], &alumne->notes[2],
          &alumne->notes[3]);
    alumne->notes[4] = mitjana(alumne->notes, 4);
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

    return m / (float) n;
}
