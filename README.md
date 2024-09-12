# Proiect de Compresie a Imaginilor folosind Arbori Cuaternari

## Descriere
Acest proiect implementează un algoritm de compresie a imaginilor utilizând arbori cuaternari. Arborele cuaternar este o structură de date în care fiecare nod are 4 copii, reprezentând împărțirea unei imagini în 4 pătrate. Algoritmul decide dacă o zonă a imaginii este suficient de omogenă pentru a fi comprimată sau dacă trebuie împărțită recursiv în pătrate mai mici.

Ceința completă poate fi văzută în fișierul `Image-Compression.pdf`

## Implementare
Algoritmul folosește o **parcurgere în adâncime** pentru a construi arborele și pentru a determina numărul de frunze și niveluri ale acestuia. Parcurgerea pe nivele este utilizată pentru afișarea structurii comprimate.

## Funcționalități principale

### Compresie
- **Arborele cuaternar** este folosit pentru compresia imaginilor. Fiecare nod corespunde unui pătrat din imagine. Nodurile terminale (frunze) reprezintă regiuni omogene care nu necesită divizare suplimentară.
- **Criteriul de omogenitate** este determinat prin calcularea culorii medii a fiecărui pătrat și compararea cu un prag dat (factorul de compresie). Dacă diferențele de culoare sunt suficient de mici, regiunea este considerată omogenă.
- **Recursivitate**: Dacă o zonă nu este omogenă, se împarte în 4 subzone, iar procesul continuă pentru fiecare subzonă.

### Decompresie
- **Reconstrucția imaginii** se face prin parcurgerea arborelui cuaternar. Nodurile frunză conțin informații despre culoarea medie a zonelor pe care le reprezintă. Imaginea este reconstruită pe baza acestei structuri.
- **Parcurgere pe nivele**: Pentru afișarea comprimată a imaginii, arborele este parcurs pe nivele, afișând fiecare nivel al structurii generate.

## Structura fișierelor

### Fișier PPM
- Fișierul de intrare folosește formatul PPM, care stochează imaginile ca o matrice de pixeli RGB.

### Fișier comprimat
- Fișierul de ieșire este un fișier binar care conține informații despre structura arborelui cuaternar: noduri interne și frunze cu valori de culoare pentru pixelii pe care îi reprezintă.

## Modul de utilizare

Programul poate fi rulat cu următoarele opțiuni în linia de comandă:
- `./quadtree -c1 factor [input_image.ppm] [output_file]`: Realizează compresia imaginii conform cerinței 1.
- `./quadtree -c2 factor [input_image.ppm] [output_file]`: Realizează compresia conform cerinței 2.
- `./quadtree -d [compressed_file] [output_image.ppm]`: Decomprimă o imagine din fișierul comprimat și recreează imaginea originală.
