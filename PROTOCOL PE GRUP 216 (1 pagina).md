# Temps de càlcul de l'enèsim nombre de Fibonacci segons l'algorisme

**Protocol · Grup 216:** *Martín Molina Pavón · Aaron Gutiérrez Miró · Martí Cervilla Torres*

## 1. Introducció i objectius

El nombre de Fibonacci F(n) es pot calcular amb algorismes de cost teòric molt diferent: recursió bàsica (exponencial), programació dinàmica (lineal) i duplicació ràpida (logarítmic). El cost teòric ignora les constants i els detalls d'implementació, i no indica quant temps real es guanya. Les dades empíriques permeten quantificar aquest guany.

**Objectiu principal:** estimar la diferència de temps d'execució entre la programació dinàmica i la duplicació ràpida, i com varia amb n.

**Objectius secundaris:** comprovar si el temps observat s'ajusta al cost teòric i quantificar el temps de la recursió bàsica dins del rang en què és executable.

## 2. Variables

| Variable | Rol | Tipus | Definició i unitats |
|---|---|---|---|
| Y: `temps_ns` | Resposta | Numèrica contínua | Temps d'execució d'un càlcul, en nanosegons |
| X: `metode` | Explicativa | Categòrica (3 nivells) | Recursió bàsica, programació dinàmica o duplicació ràpida |
| Z: `n` | Covariable | Numèrica discreta | Posició del nombre de Fibonacci calculat |

**Instrument:** el programa `mesura.cc` (C++17, `g++ -O2`) pren el temps com la diferència del rellotge `std::chrono::steady_clock` just abans i just després de cada càlcul.

**Error de mesura:** el temps varia per altres processos de l'ordinador, i el rellotge no mesura bé temps molt curts. Per això cada càlcul es repeteix 10 vegades i s'exclouen les n amb temps massa petits.

**Nombres grans:** F(n) supera la capacitat de `unsigned long long` a partir de n = 94. **[PER DECIDIR: mòdul 10⁹+7 o enters grans, i com afecta el temps.]**

**Emmagatzematge:** fitxers `.txt` llegibles per R, amb les 10 mesures de cada n i mètode i una taula conjunta de mitjanes.

## 3. Pla de recollida

**Mida de la mostra:** **[PER DECIDIR: 25-30]** valors de n, mesurats 10 vegades amb cada algorisme. Les repeticions d'una mateixa n no són independents: s'analitza la mitjana per n i algorisme, i la mida de la mostra és el nombre de valors de n.

**Rang:** n fins a **[PER DECIDIR]**. Si una execució supera 60 s, aquell algorisme descarta aquesta n i les més grans. Això limita sobretot la recursió bàsica.

**Procediment:** es compila el programa, es tanquen els altres programes i s'executa `mesura.cc`, que llegeix els valors de n i desa els resultats. Per a cada n, l'ordre dels algorismes es decideix a l'atzar amb una llavor fixa **[PER CONFIRMAR: cal afegir-ho al programa]**. Tot es mesura amb el mateix ordinador: **[PER OMPLIR: processador, memòria i sistema operatiu]**. El codi i les dades es guarden a GitHub.

**Responsabilitats:** **[PER OMPLIR: qui programa cada algorisme, qui executa i qui revisa]**. **Calendari:** dades definitives el **[PER OMPLIR: data]**, en una setmana com a màxim.

## 4. Procés de mostreig

**Població de referència:** els enters n entre **[PER DECIDIR: mínim]** i **[PER DECIDIR: màxim]**, amb aquesta implementació i aquest ordinador. Els resultats no es generalitzen a altres màquines.

**Extracció:** el rang es divideix en trams logarítmics i, dins de cada tram, s'escull el mateix nombre de valors a l'atzar amb una llavor fixa **[PER CONFIRMAR]**. No és una MAS, perquè aquesta donaria gairebé només valors grans i no permetria estudiar la relació entre el temps i n.

**Disseny:** dades aparellades, perquè els mateixos valors de n es mesuren amb tots els algorismes. Les comparacions amb la recursió bàsica només es fan dins del rang comú.
