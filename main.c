/* Antonescu Albert - 314CD */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "coada.h"

// structura pentru un pixel
typedef struct {
    unsigned char r, g, b;
} RGB;

// structura pentru un nod din arbore
typedef struct tree {
    RGB culoare;
    // sub blocurile pentru divizare
    struct tree *sub_blocuri[4];
} QuadTree;

QuadTree* comprimare(RGB **imagine, int factor, int x, int y, int dim) {
    QuadTree *nod = (QuadTree*)malloc(sizeof(QuadTree));

    // calculam media pentru fiecare culoare
    long long r = 0, g = 0, b = 0;
    for (int i = x; i < x + dim; i++) {
        for (int j = y; j < y + dim; j++) {
            r += imagine[i][j].r;
            g += imagine[i][j].g;
            b += imagine[i][j].b;
        }
    }

    r /= (dim * dim);
    g /= (dim * dim);
    b /= (dim * dim);

    // calculam similaritatea culorii medii
    long long mean = 0;
    for (int i = x; i < x + dim; i++) {
        for (int j = y; j < y + dim; j++) {
            mean += ((r - imagine[i][j].r) * (r - imagine[i][j].r) +
                    (g - imagine[i][j].g) * (g - imagine[i][j].g) +
                    (b - imagine[i][j].b) * (b - imagine[i][j].b));
        }
    }

    mean /= (3 * dim * dim);

    // daca similaritatea este mai mica decat factorul, atunci
    // blocul este comprimat intr-un singur pixel
    if (mean <= factor) {
        nod->culoare.r = r;
        nod->culoare.g = g;
        nod->culoare.b = b;
        nod->sub_blocuri[0] = nod->sub_blocuri[1] = nod->sub_blocuri[2] = nod->sub_blocuri[3] = NULL;
    }
    // altfel, blocul este divizat in 4 sub blocuri
    else {
        nod->sub_blocuri[0] = comprimare(imagine, factor, x, y, dim / 2);
        nod->sub_blocuri[1] = comprimare(imagine, factor, x, y + dim / 2, dim / 2);
        nod->sub_blocuri[2] = comprimare(imagine, factor, x + dim / 2, y + dim / 2, dim / 2);
        nod->sub_blocuri[3] = comprimare(imagine, factor, x + dim / 2, y, dim / 2);
    }
    return nod;
}

int numara_blocuri(QuadTree *root) {
    if (root == NULL) {
        return 0;
    }
    // daca nodul nu are sub blocuri, atunci este un bloc cu culoare
    if (root->sub_blocuri[0] == NULL) {
        return 1;
    } else {
        // altfel, numaram blocurile din sub blocuri
        return numara_blocuri(root->sub_blocuri[0]) +
               numara_blocuri(root->sub_blocuri[1]) +
               numara_blocuri(root->sub_blocuri[2]) +
               numara_blocuri(root->sub_blocuri[3]);
    }
}

int numara_nivele(QuadTree *root) {
    if (root == NULL) {
        return 0;
    }
    // daca nodul nu are sub blocuri, atunci este un bloc cu culoare
    if (root->sub_blocuri[0] == NULL) {
        return 1;
    } else {
        // altfel, numaram nivelele din sub blocuri
        int max = 0;
        for (int i = 0; i < 4; i++) {
            int nivel = numara_nivele(root->sub_blocuri[i]);
            if (nivel > max) {
                max = nivel;
            }
        }
        return max + 1;
    }
}

int numara_drum_minim(QuadTree *root) {
    if (root == NULL) {
        return 0;
    }
    // daca nodul nu are sub blocuri, atunci este un bloc cu culoare
    if (root->sub_blocuri[0] == NULL) {
        return 1;
    } else {
        // altfel, numaram drumul minim catre o frunza
        int min = 1000000000;
        for (int i = 0; i < 4; i++) {
            int drum = numara_drum_minim(root->sub_blocuri[i]);
            if (drum < min) {
                min = drum;
            }
        }
        return min + 1;
    }
}

void reconstruieste_imagine(QuadTree *root, RGB **imagine, int x, int y, int dim) {
    if (root == NULL) {
        return;
    }

    // daca nodul nu are sub blocuri, atunci este un bloc cu culoare
    // si coloram pixelii din imagine cu culoarea blocului
    if (root->sub_blocuri[0] == NULL) {
        for (int i = x; i < x + dim; i++) {
            for (int j = y; j < y + dim; j++) {
                imagine[i][j].r = root->culoare.r;
                imagine[i][j].g = root->culoare.g;
                imagine[i][j].b = root->culoare.b;
            }
        }
    } else {
        // altfel, reconstruim imaginea din sub blocuri
        reconstruieste_imagine(root->sub_blocuri[0], imagine, x, y, dim / 2);
        reconstruieste_imagine(root->sub_blocuri[1], imagine, x, y + dim / 2, dim / 2);
        reconstruieste_imagine(root->sub_blocuri[2], imagine, x + dim / 2, y + dim / 2, dim / 2);
        reconstruieste_imagine(root->sub_blocuri[3], imagine, x + dim / 2, y, dim / 2);
    }
}

void afisare(QuadTree *root, int nivel, FILE* fisier) {
    if (root == NULL) {
        return;
    }

    // daca nivelul este 0, atunci afisam culoarea blocului
    if (nivel == 0) {
        if (root->sub_blocuri[0] == NULL) {
            // daca nodul nu are sub blocuri, atunci este un bloc cu culoare
            // se afiseaza 1 si culoarea blocului
            unsigned char c = 1;
            fwrite(&c, sizeof(unsigned char), 1, fisier);
            fwrite(&root->culoare, sizeof(RGB), 1, fisier);
        } else {
            // altfel, se afiseaza 0
            unsigned char c = 0;
            fwrite(&c, sizeof(unsigned char), 1, fisier);
        }
        
    } else {
        // altfel, afisam sub blocurile
        for (int i = 0; i < 4; i++) {
            afisare(root->sub_blocuri[i], nivel - 1, fisier);
        }
    }
}

void free_tree(QuadTree *nod) {
    if (nod == NULL) {
        return;
    }

    free_tree(nod->sub_blocuri[0]);
    free_tree(nod->sub_blocuri[1]);
    free_tree(nod->sub_blocuri[2]);
    free_tree(nod->sub_blocuri[3]);
    free(nod);
}

int main(int argc, char *argv[]) {
    if (strcmp(argv[1], "-c1") == 0) {
        // citim datele din fisier
        int factor = atoi(argv[2]);
        FILE* intrare = fopen(argv[3], "rb");
        FILE* iesire = fopen(argv[4], "w");

        char str[100];
        fgets(str, 100, intrare);
        int marime_imagine;
        fscanf(intrare, "%d", &marime_imagine);
        fscanf(intrare, "%d\n", &marime_imagine);

        int culoare_maxima;
        fgets(str, sizeof(str), intrare);
        culoare_maxima =atoi(str);

        RGB **imagine = (RGB**)malloc(marime_imagine * sizeof(RGB*));
        for (int i = 0; i < marime_imagine; i++) {
            imagine[i] = (RGB*)malloc(marime_imagine * sizeof(RGB));
            fread(imagine[i], sizeof(RGB), marime_imagine, intrare);
        }

        // construim arborele de compresie
        QuadTree *radacina = comprimare(imagine, factor, 0, 0, marime_imagine);

        // determinam statistici despre arbore
        fprintf(iesire, "%d\n", numara_nivele(radacina));
        fprintf(iesire, "%d\n", numara_blocuri(radacina));
        fprintf(iesire, "%d\n", marime_imagine >> (numara_drum_minim(radacina) - 1));

        // eliberam memoria
        for (int i = 0; i < marime_imagine; i++) {
            free(imagine[i]);
        }
        free_tree(radacina);
        free(imagine);
        fclose(intrare);
        fclose(iesire);
    }
    else if (strcmp(argv[1], "-c2") == 0) {
        // citim datele din fisier
        int factor = atoi(argv[2]);
        FILE* intrare = fopen(argv[3], "rb");
        FILE* iesire = fopen(argv[4], "wb");

        char str[100];
        fgets(str, 100, intrare);
        int marime_imagine;
        fscanf(intrare, "%d", &marime_imagine);
        fscanf(intrare, "%d\n", &marime_imagine);

        int culoare_maxima;
        fgets(str, sizeof(str), intrare);
        culoare_maxima =atoi(str);

        RGB **imagine = (RGB**)malloc(marime_imagine * sizeof(RGB*));
        for (int i = 0; i < marime_imagine; i++) {
            imagine[i] = (RGB*)malloc(marime_imagine * sizeof(RGB));
            fread(imagine[i], sizeof(RGB), marime_imagine, intrare);
        }

        // construim arborele de compresie
        QuadTree *radacina = comprimare(imagine, factor, 0, 0, marime_imagine);

        // afisam arborele in fisier
        int nivele = numara_nivele(radacina);
        fwrite(&marime_imagine, sizeof(unsigned int), 1, iesire);
        for (int i = 0; i <= nivele; i++) {
            afisare(radacina, i, iesire);
        }

        // eliberam memoria
        for (int i = 0; i < marime_imagine; i++) {
            free(imagine[i]);
        }
        free(imagine);
        free_tree(radacina);
        fclose(intrare);
        fclose(iesire);
    }
    else if (strcmp(argv[1], "-d") == 0) {
        // citim datele din fisier
        QuadTree *radacina = malloc(sizeof(QuadTree));

        FILE* intrare = fopen(argv[2], "rb");
        FILE* iesire = fopen(argv[3], "wb");

        int marime_imagine;
        fread(&marime_imagine, sizeof(int), 1, intrare);

        // folosim o coada pentru a reconstrui arborele
        TQueue coada = initQueue(radacina);

        while (isEmpty(coada) == 0) {
            // extragem nodul curent din coada
            QuadTree *nod = (QuadTree*)front(coada);
            coada = dequeue(coada);
            
            if (nod == NULL)
                continue;

            unsigned char c;
            fread(&c, sizeof(unsigned char), 1, intrare);
            if (c == 1) {
                // nodul este frunza
                fread(&nod->culoare, sizeof(RGB), 1, intrare);
                nod->sub_blocuri[0] = NULL;
                nod->sub_blocuri[1] = NULL;
                nod->sub_blocuri[2] = NULL;
                nod->sub_blocuri[3] = NULL;
            } else {
                // nodul nu este frunza
                // il adaugam in coada si alocam memorie pentru subarborii sai
                for (int i = 0; i < 4; i++) {
                    nod->sub_blocuri[i] = malloc(sizeof(QuadTree));
                    coada = enqueue(coada, nod->sub_blocuri[i]);
                }
            }
        }
        freeQueue(coada);

        // reconstruim imaginea
        RGB **imagine = (RGB**)malloc(marime_imagine * sizeof(RGB*));
        for (int i = 0; i < marime_imagine; i++) {
            imagine[i] = (RGB*)malloc(marime_imagine * sizeof(RGB));
        }

        reconstruieste_imagine(radacina, imagine, 0, 0, marime_imagine);

        // afisam imaginea in fisier
        fprintf(iesire, "P6\n");
        fprintf(iesire, "%d %d\n", marime_imagine, marime_imagine);
        fprintf(iesire, "255\n");
        for (int i = 0; i < marime_imagine; i++) {
            fwrite(imagine[i], sizeof(RGB), marime_imagine, iesire);
        }

        // eliberam memoria
        free_tree(radacina);
        for (int i = 0; i < marime_imagine; i++) {
            free(imagine[i]);
        }
        free(imagine);
    }
    return 0;
}
