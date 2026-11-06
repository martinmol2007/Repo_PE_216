**PROTOCOL PE GRUP 216**

*Martin Molina Pavon*   
*Aarón Gutiérrez Miró*   
*Martí Cervilla Torres*  

L’objectiu d’aquest treball és comparar el temps que triguen diversos algorismes per calcular el n-èssim nombre de Fibonacci. Es duran a terme diverses proves portant al límit el codi per veure quin algorisme és més eficient, o simplement, funciona amb casos molt grans.

1. **Objectius:**

Calcular el temps que triguen els algorismes a calcular el n-èssim nombre de Fibonacci amb 3 algorismes diferents per comparar-los respecte a les n’s triades i de manera general per veure quin és més eficient.

2. **Variables:**  
* X: Algorismes (Recursió Bàsica, Programació Dinàmica i Duplicació Ràpida)  
* Y: Temps per calcular l’n-èssim terme de Fibonacci.  
* Z: Valor n

Calcularem el terme n-èssim de Fibonacci 10 cops per cada n i farem la mitjana, així, serà un resultat molt més aleatori que sí l’executem només un cop. Després, agruparem totes les mitjanes d’un mètode en una taula per tenir tota l’info d’un mètode, també ho farem amb els tres mètodes (tots els resultats en una mateixa taula). La informació es guardarà en un repo de GitHub, encara que farem servir documents .txt.

3. **Pla de recollida:**

Executarem un total de 46 valors diferents de n. Les n’s van des de valor molt petit, com 1 o 2, fins a valor extrems com 10000000\. S’han agafat aquests nombres, ja que contenen tots els extrems, els que triguen poc i els que molt, i al mig, valors raonables que marcaran la diferència entre algorismes. S’ha creat un programa per anar llegint fitxers i escrivint les dades automàticament. Cadascun farà un algorisme i posarem les dades en comú.

4. **Procés de mostreig:**

La població de referència són les 46 n’s que farem servir per mesurar el temps que triguen els algorismes. La manera en la qual obtindrem una mostra aleatòria és la comentada prèviament, executant el codi 10 vegades per a cada n i fer la mitjana. Objectivament, mai serà totalment aleatòria, ja que l’execució d’un codi depèn de moltes coses, com per exemple, els components d’unt PC com la CPU. Les dades són aparellades, pel fet que mesurem el temps de les mateixes n’s amb diferents algorismes.  
