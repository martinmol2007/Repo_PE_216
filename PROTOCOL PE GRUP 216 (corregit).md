# Comparació empírica del temps de càlcul de l'enèsim nombre de Fibonacci segons l'algorisme

**Protocol · Grup 216**

*Martín Molina Pavón · Aaron Gutiérrez Miró · Martí Cervilla Torres*

## Resum

Aquest estudi compara el temps d'execució de tres algorismes que calculen l'enèsim nombre de Fibonacci. Cada algorisme es mesurarà amb els mateixos valors de n, en un disseny de dades aparellades, per estimar quant de temps guanya un algorisme respecte d'un altre i com varia aquest guany amb n.

## 1. Introducció i objectius

El nombre de Fibonacci F(n) es pot calcular amb algorismes de cost teòric molt diferent: recursió bàsica (exponencial), programació dinàmica (lineal) i duplicació ràpida (logarítmic). L'ordre de velocitat és previsible, però el cost teòric no indica quant temps real es guanya, ja que ignora les constants i els detalls d'implementació. Les dades empíriques permeten quantificar aquest guany.

**Objectiu principal:** estimar la diferència de temps d'execució entre la programació dinàmica i la duplicació ràpida, i com varia amb n.

**Objectius secundaris:** comprovar si el temps observat de cada algorisme s'ajusta al seu cost teòric, i quantificar el temps de la recursió bàsica dins del rang de n en què és executable.

## 2. Variables

| Variable | Rol | Tipus | Definició i unitats |
|---|---|---|---|
| Y: `temps_ns` | Resposta | Numèrica contínua | Temps d'execució d'un càlcul, en nanosegons |
| X: `metode` | Explicativa | Categòrica (3 nivells) | Recursió bàsica, programació dinàmica o duplicació ràpida |
| Z: `n` | Covariable | Numèrica discreta | Posició del nombre de Fibonacci calculat |

**Instrument:** el programa `mesura.cc`, escrit en C++17 i compilat amb `g++ -O2`. Consulta el rellotge `std::chrono::steady_clock` just abans i just després de cada càlcul; la diferència és el temps de l'execució.

**Error de mesura:** el temps pot variar per altres processos de l'ordinador, i el rellotge no mesura bé temps molt curts. En una prova prèvia, una funció gairebé buida ja ha donat uns 20 ns. Per això cada càlcul es repeteix 10 vegades i s'exclouran les n amb temps massa petits per ser mesurats.

**Nombres grans:** F(n) supera la capacitat de `unsigned long long` a partir de n = 94. **[PER DECIDIR: aritmètica amb mòdul 10⁹+7 o enters grans. Cal explicar com afecta el temps.]**

**Emmagatzematge:** fitxers `.txt` de text pla, llegibles per R. Hi haurà un fitxer amb les 10 mesures de cada n i mètode, una taula de mitjanes per mètode i una taula conjunta amb una fila per n i una columna per mètode.

## 3. Pla de recollida

**Mida de la mostra:** **[PER DECIDIR: 25-30 valors de n.]** Cada valor es mesurarà 10 vegades amb cada algorisme. Les repeticions d'una mateixa n no són independents, de manera que s'analitzarà la mitjana de cada combinació de n i algorisme. Per tant, la mida de la mostra és el nombre de valors de n, no el nombre d'execucions.

**Rang de cada algorisme:** la recursió bàsica només és executable amb n petites. En una prova prèvia ha trigat uns 0,3 s amb n = 42, cosa que permet estimar uns 27 minuts per execució amb n = 60. Es mesurarà fins a n = **[PER DECIDIR]**. Els altres dos algorismes arribaran fins a n = **[PER DECIDIR]**.

**Procediment:**

1. Es compila el programa amb les opcions indicades.
2. Es tanquen els altres programes de l'ordinador.
3. S'executa el programa, que llegeix els valors de n i desa els resultats automàticament.
4. Per a cada n, l'ordre dels algorismes es decideix a l'atzar amb una llavor fixa. **[PER CONFIRMAR: cal afegir-ho al programa.]**
5. Es comprova que les taules es llegeixen correctament amb R.

Totes les mesures es faran amb el mateix ordinador i en les mateixes condicions: **[PER OMPLIR: processador, memòria i sistema operatiu]**. El codi i les dades es guardaran en un repositori de GitHub, perquè l'estudi sigui reproduïble.

**Responsabilitats:** **[PER OMPLIR: qui programa cada algorisme, qui executa l'experiment i qui revisa les dades]**.

**Calendari:** la recollida durarà com a màxim una setmana, amb les dades definitives el **[PER OMPLIR: data]**.

## 4. Procés de mostreig

**Població de referència:** els valors enters de n entre **[PER DECIDIR: mínim]** i **[PER DECIDIR: màxim]**, calculats amb la implementació i l'ordinador descrits. Els resultats no es poden generalitzar directament a altres màquines.

**Extracció de la mostra:** el rang es dividirà en trams logarítmics i, dins de cada tram, s'escollirà el mateix nombre de valors a l'atzar amb un generador de nombres aleatoris i una llavor fixa. **[PER CONFIRMAR]** No és una mostra aleatòria simple: una MAS sobre tot el rang donaria gairebé només valors grans i no permetria estudiar la relació entre el temps i n. Aquest procediment cobreix tot el rang i evita triar els valors a mà.

**Disseny:** les dades són aparellades, perquè els mateixos valors de n es mesuren amb tots els algorismes. Les comparacions amb la recursió bàsica només es podran fer dins del rang comú.
