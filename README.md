# Bloc Transversal – Probabilitat i Estadística (FIB-UPC)

Treball del **Bloc Transversal (BT)** de l'assignatura de Probabilitat i Estadística del Grau en Enginyeria Informàtica.

| | |
|---|---|
| **Grup de PE** | Grup XX |
| **Professor/a** | _Per omplir_ |
| **Integrants** | Membre 1 · Membre 2 · Membre 3 |
| **Tema** | Comparació empírica d'algorismes per calcular la successió de Fibonacci |
| **Estat del tema** | ⏳ **Pendent d'aprovació pel professor** |

---

## Índex

1. [Objectiu del Bloc Transversal](#1-objectiu-del-bloc-transversal)
2. [Avaluació](#2-avaluació)
3. [Proposta d'estudi](#3-proposta-destudi)
4. [Estat del codi actual](#4-estat-del-codi-actual)
5. [Notes sobre les dades](#5-notes-sobre-les-dades)
6. [Estructura del repositori](#6-estructura-del-repositori)
7. [Pla de treball (TODO)](#7-pla-de-treball-todo)
8. [Calendari orientatiu](#8-calendari-orientatiu)
9. [Organització del grup](#9-organització-del-grup)
10. [Recursos](#10-recursos)

---

## 1. Objectiu del Bloc Transversal

Segons la presentació del BT ([`BT_Presentacio.pdf`](Informacio/Introduccio/BT_Presentacio.pdf)):

> En acabar el Bloc T, el participant analitzarà crítica i numèricament afirmacions informàtiques com *"l'algorisme A és més eficient que el B"* o *"el cost de l'algorisme C és n log n"*, aportant evidències empíriques.

A més, haurem de:
- Fer **informes escrits** concisos i **reproduïbles**.
- Fer **presentacions orals** eficients i informatives.
- **Treballar en equip** (grups de 3 persones del mateix grup de PE).

El tema ha de ser **"informàtic" i assolible**. Cal consultar la proposta amb el professor.

---

## 2. Avaluació

| Element | Pes | Què és |
|---|---|---|
| **Protocol** | 20% | Justificació, objectius i disseny de la recollida de dades (2-4 pàgines) |
| **Lliurament** (informe + dades) | 40% | Informe escrit amb resultats, dades i codi R |
| **Presentació oral** | 40% | Defensa davant del professor i la classe (**< 7 minuts**) |

> ⚠️ **Requisit per ser avaluat:** cada document ha de contenir els elements imprescindibles de la seva *checklist*.

### Criteris de les rúbriques (escala [0,4) · [4,7) · [7,9) · [9,10])

| Criteri | Protocol | Informe | Presentació |
|---|:---:|:---:|:---:|
| **Clar** – redacció clara i ordenada | ✅ | ✅ | ✅ |
| **Correcte** – ortografia, sintaxi, ús de tecnicismes (amb aclariments) | ✅ | ✅ | ✅ |
| **Complet** – conté Objectius, Variables, Pla de recollida i Procés de mostreig | ✅ | | |
| **Concís** – sense divagacions | | ✅ | |
| **Rigorós** – ús correcte de les eines estadístiques (descriptiva, gràfics, IC, premisses...) | ✅ | ✅ | ✅ (també al torn de preguntes) |
| **Coherent** – coherència entre les parts (i amb l'informe, a la presentació) | ✅ | ✅ | ✅ |
| **Equilibrada** – intervencions equilibrades entre els membres | | | ✅ |
| **Ajustada al temps** | | | ✅ |

---

## 3. Proposta d'estudi

**Pregunta d'investigació:** com varia el temps d'execució del càlcul de l'n-èssim nombre de Fibonacci segons l'algorisme utilitzat i la mida de l'entrada *n*?

Algorismes a comparar:
1. **Naive** (recursiu, versió lenta) – cost teòric O(φⁿ).
2. **Memoïtzació / programació dinàmica** (vector passat per referència) – cost teòric O(n).
3. **Duplicació ràpida** (*fast doubling*, expressió transformada) – cost teòric O(log n).

### Variables

| Variable | Tipus | Descripció |
|---|---|---|
| **Y** | Resposta numèrica | Temps d'execució (unitat a fixar: µs o ms) |
| **X** | Factor (acció) | Algorisme: `naive` / `memo` / `doubling` |
| **Z** | Covariable | *n*, la posició del nombre de Fibonacci a calcular |

> ⚠️ **A consultar amb el professor:** la guia recomana que X sigui **dicotòmica** (2 nivells). Volem comparar els **3 algorismes**; si no s'accepta, l'alternativa és **memoïtzació vs fast doubling** com a anàlisi principal (i el naive com a anàlisi secundària o descartat).

### Disseny proposat

- **Dades aparellades:** cada valor de *n* s'executa amb tots els algorismes (és més eficient estadísticament).
- **Mostra aleatòria simple (MAS):** els valors de *n* s'escullen **a l'atzar** dins d'un rang definit (amb un generador de nombres aleatoris i una llavor fixada perquè sigui reproduïble), no escollits a mà.
- **Mida mostral:** mínim **30** observacions, recomanable **~50**.
- **Una sola resposta principal (Y)**: el temps d'execució.

### Problemes a resoldre abans del protocol

- [ ] **Rang de *n* del naive:** és exponencial, a partir de *n* ≈ 40-45 triga massa. No pot compartir rang amb els altres dos → decidir si s'usa un rang comú petit, rangs separats o si s'analitza a part.
- [ ] **Temps massa petits:** el fast doubling triga microsegons; la màquina no és prou precisa. Solució: repetir cada càlcul *k* vegades i guardar la mitjana (o el total).
- [ ] **Desbordament (overflow):** amb `unsigned long long` els resultats són incorrectes per a *n* > 93. Fer servir el **mòdul 10⁹+7 a tots tres algorismes** perquè facin la mateixa feina.
- [ ] **Condicions de mesura constants:** mateixa màquina, mateix compilador i flags (p.ex. `g++ -O2`), tancar programes, ordre d'execució dels algorismes aleatoritzat.
- [ ] **Evitar una pregunta òbvia:** naive vs la resta és evident; cal que l'interès estigui en quantificar les diferències i ajustar models (p.ex. comprovar si el cost observat s'ajusta a O(n) o O(log n)).

---

## 4. Estat del codi actual

| Fitxer | Què fa | Observacions |
|---|---|---|
| `test.cc` | Fast doubling recursiu amb mòdul 10⁹+7 | ✅ Correcte, O(log n). Falta mesurar el temps. |
| `test2.cc` | Programació dinàmica **iterativa** amb vector | ⚠️ No és memoïtzació "per referència" com diu la proposta. ⚠️ Errada al missatge: `"long longrodueix"` → `"Introdueix"`. ⚠️ Sense mòdul → overflow per *n* > 93. ⚠️ Vector de mida *n*+1: amb *n* = 10⁸ ocupa ~800 MB de memòria. |

**Què falta:**
- [ ] Implementar la versió **naive**.
- [ ] Implementar (o adaptar `test2.cc` a) la versió **memoïtzació** amb vector per referència.
- [ ] Unificar els 3 algorismes (mateix tipus de retorn i mateix mòdul).
- [ ] Programa de **mesura de temps** amb `<chrono>` (`std::chrono::steady_clock`).
- [ ] Generació de valors de *n* **aleatoris** amb llavor fixa.
- [ ] Escriure els resultats directament a un fitxer `.csv` llegible per R.

---

## 5. Notes sobre les dades

Extretes de les nostres notes i de [`01_Dades.pdf`](Informacio%20addicional/ReadMe%20First/01_Dades.pdf):

- Les dades s'han de guardar en fitxers que **R pugui llegir**: `.csv`, `.txt` o `.xlsx` (els més recomanables).
- Les dades **no** han de contenir caràcters problemàtics per a R: `?`, `!`, `*`, accents...
- **Noms de variables sense espais** ni símbols estranys.
- **No deixar files ni columnes en blanc.**
- Format de **taula plana**: files = observacions, columnes = variables.
- Mínim **30 observacions**, recomanable **unes 50**.
- La recollida de dades no hauria de durar **més de 3 setmanes**.
- Importar les dades a R **amb comandes** (`read.csv(...)`), no amb el menú.

### Estructura del fitxer de dades

Com que el disseny és **aparellat**, cada fila és un valor de *n* i hi ha una columna per algorisme:

```
n,naive,memo,doubling
37,1523.4,0.82,0.05
12,0.91,0.11,0.03
...
```

Si al final el disseny fos **independent**, el format seria `X,Y,Z` (una fila per execució):

```
algorisme,temps,n
memo,0.82,37
doubling,0.05,912
...
```

---

## 6. Estructura del repositori

Estructura actual i carpetes proposades (🆕 = encara no existeix):

```
Repo_PE_216/
├── README.md
├── Makefile                  # Compilacio i flux de treball amb git (make help)
├── .gitignore                # Fitxers que no es pugen (bin/, .claude/, temporals...)
├── Guia_flux_de_treball_git.pdf  # Guia de git per al grup
├── Guia_installacio_git.pdf      # Com instal·lar git, make i g++ (primer cop)
├── test.cc, test2.cc         # Codis C++ (es compilen a l'arrel amb make)
├── bin/                      # Executables generats per make (ignorat per git)
├── dades/                    # 🆕 Fitxers .csv amb les observacions
├── R/                        # 🆕 Scripts d'anàlisi en R
├── docs/                     # 🆕 Protocol, informe i presentació
├── Informacio/               # Rúbriques i checklists oficials
│   ├── Introduccio/
│   ├── Protocol/
│   ├── Informe escrit/
│   └── Presentació oral/
└── Informacio addicional/    # Guies, lectures, exemples i idees
```

---

## 7. Pla de treball (TODO)

### Fase 0 – Tema
- [ ] Consensuar la proposta entre els tres membres.
- [ ] Enviar la proposta al professor i esperar l'acceptació.
- [ ] Decidir si X té 3 nivells o 2 (segons el que digui el professor).

### Fase 1 – Protocol (2-4 pàgines) · *checklist:* [`checklists_JC 1.pdf`](Informacio/Protocol/checklists_JC%201.pdf)
- [ ] **Presentació:** títol, autors, seccions diferenciades, tipografia i marges adequats.
- [ ] **Llenguatge:** sense faltes, frases completes i simples, sense redundàncies ni opinions personals.
- [ ] **Resum** i **introducció** justificant que l'objectiu no és trivial.
- [ ] **Objectius:** factibles, quantificables, sense ambigüitat; distingir principal i secundaris.
- [ ] **Variables:** relacionades amb l'objectiu, mesurables, reproduïbles; descriure l'instrument (programa de mesura), unitats, com es guarda la informació i possible error de mesura.
- [ ] **Pla de recollida:** previsió de *n* (i justificació), pla d'accions, repartiment de responsabilitats.
- [ ] **Procés de mostreig:** població de referència, mecanisme d'extracció aleatòria (MAS), aleatorització de l'ordre dels algorismes, dades aparellades o independents.
- [ ] Revisar-lo amb la rúbrica [`Rubrica PROTOCOLO.pdf`](Informacio/Protocol/Rubrica%20PROTOCOLO.pdf) i lliurar.

### Fase 2 – Codi
- [ ] Implementar els 3 algorismes amb la mateixa interfície i el mateix mòdul.
- [ ] Programa de mesura de temps amb repeticions.
- [ ] Generador de *n* aleatoris amb llavor fixa.
- [ ] Sortida a `.csv` dins de `dades/`.
- [ ] Documentar com compilar i executar (perquè sigui reproduïble).

### Fase 3 – Recollida de dades
- [ ] Executar l'experiment en les condicions fixades al protocol.
- [ ] Comprovar que el fitxer es llegeix bé amb R (sense valors buits ni caràcters estranys).
- [ ] Anotar qualsevol incidència o canvi respecte al protocol.

### Fase 4 – Anàlisi en R
- [ ] Script de lectura de les dades.
- [ ] **Descriptiva** numèrica (mitjana, mediana, desviació...) i gràfica (boxplots, diagrames de dispersió temps vs *n*).
- [ ] **Comparació** entre algorismes: intervals de confiança per a la diferència (dades aparellades).
- [ ] **Model lineal** de Y en funció de Z (valorar transformacions, p.ex. `log(temps)`).
- [ ] **Validació gràfica de les premisses** (normalitat, residus...).

### Fase 5 – Informe · *checklist:* [`checklists_JC 2.pdf`](Informacio/Informe%20escrit/checklists_JC%202.pdf)
- [ ] **Resum** ampliat amb resultats numèrics i conclusions.
- [ ] **Motivació** (similar al protocol).
- [ ] **Metodologia** reproduïble, explicant els canvis respecte al protocol i les anàlisis estadístiques.
- [ ] **Resultats:** descriptiva, IC, models; taules amb xifres significatives raonables; transformacions indicades; premisses; **sense interpretacions**.
- [ ] **Discussió:** resum dels resultats, **limitacions**, lliçons i recomanacions per a estudis futurs.
- [ ] Adjuntar **dades** i **codi R**.
- [ ] Emplenar l'**annex de contribucions** ([`annex informe.docx`](Informacio/Informe%20escrit/annex%20informe.docx), [`checklists_JC 4.pdf`](Informacio/Informe%20escrit/checklists_JC%204.pdf)).
- [ ] Revisar-lo amb la rúbrica [`Rubrica INFORME escrito.pdf`](Informacio/Informe%20escrit/Rubrica%20INFORME%20escrito.pdf) i lliurar.

### Fase 6 – Presentació oral (< 7 min) · *checklist:* [`checklists_JC 3.pdf`](Informacio/Presentaci%C3%B3%20oral/checklists_JC%203.pdf)
- [ ] Diapositives amb lletra gran, fons clar i text fosc, sense imatges de fons ni massa text.
- [ ] Estructura: objectiu i hipòtesi → procés seguit → resultats seleccionats → limitacions i recomanacions (sense *abstract*).
- [ ] Repartir el temps i les respostes **equitativament** entre els tres membres.
- [ ] **Assajar** amb cronòmetre; no llegir.
- [ ] Preparar possibles preguntes sobre els conceptes estadístics.
- [ ] Revisar-la amb la rúbrica [`Rubrica PRESENTACION oral.pdf`](Informacio/Presentaci%C3%B3%20oral/Rubrica%20PRESENTACION%20oral.pdf).

---

## 8. Calendari orientatiu

Com que **no fem el treball a classe**, aquest calendari és orientatiu (basat en el de `BT_Presentacio.pdf`). Cal omplir les dates límit quan les sapiguem.

| Fase | Setmana orientativa | Data límit | Estat |
|---|---|---|---|
| Lectura de documentació i formació del grup | Set. 1-3 | — | ✅ |
| Proposta del tema | Set. 4 | _per confirmar_ | 🔄 En curs |
| Acceptació del professor | Set. 5 | _per confirmar_ | ⏳ |
| Preparació i lliurament del protocol | Set. 6-7 | _per confirmar_ | ⏳ |
| Codi i recollida de dades | Set. 8-9 | _per confirmar_ | ⏳ |
| Exploració de dades | Set. 10 | _per confirmar_ | ⏳ |
| Anàlisi estadística | Set. 11 | _per confirmar_ | ⏳ |
| Preparació i lliurament de l'informe | Set. 12-13 | _per confirmar_ | ⏳ |
| Presentació oral | Set. 14 | _per confirmar_ | ⏳ |

---

## 9. Organització del grup

Treballem **a casa**, quan els membres del grup coincidim.

### Flux de treball amb git

Tots treballem a la branca `main`. Explicació completa per a principiants: [`Guia_flux_de_treball_git.pdf`](Guia_flux_de_treball_git.pdf).

**Primer cop?** Instal·la git, make i g++ i inicia sessió a GitHub seguint [`Guia_installacio_git.pdf`](Guia_installacio_git.pdf).

```bash
make config NOM="Nom Cognom" EMAIL="correu@exemple.com"   # només la primera vegada
make pull                                                 # 1. abans de començar a treballar
make                                                      # 2. compilar
make save MSG="Que he fet"                                # 3. compila + commit + pull + push
make save                                                 #    igual, amb el missatge per defecte (MSG_DEFECTE)
make save MSG="Que hem fet" AMB="2 3"                     #    igual, amb els membres 2 i 3 com a coautors
```

Per fer servir els coautors, cal posar abans el nom i el correu de GitHub de cada membre a les variables `MEMBRE_1`, `MEMBRE_2` i `MEMBRE_3` del `Makefile`. Feu servir el **correu privat de GitHub** (`...@users.noreply.github.com`, a *Settings → Emails*), no el personal: els correus dels commits queden públics a l'historial.

`make help` mostra totes les ordres disponibles.

### Repartiment de responsabilitats

| Tasca | Responsable |
|---|---|
| Codi dels algorismes i mesura de temps | _Per decidir_ |
| Recollida de dades | _Per decidir_ |
| Anàlisi en R | _Per decidir_ |
| Redacció del protocol / informe | _Per decidir_ |
| Revisió final (coherència, rigor, completesa) | _Per decidir_ |
| Diapositives de la presentació | _Per decidir_ |

### Registre de reunions

| Data | Assistents | Acords / tasques |
|---|---|---|
| _dd/mm/aaaa_ | | |

---

## 10. Recursos

### Documentació oficial (carpeta `Informacio/`)
- [Presentació del BT](Informacio/Introduccio/BT_Presentacio.pdf)
- Protocol: [checklist](Informacio/Protocol/checklists_JC%201.pdf) · [rúbrica](Informacio/Protocol/Rubrica%20PROTOCOLO.pdf)
- Informe: [checklist](Informacio/Informe%20escrit/checklists_JC%202.pdf) · [rúbrica](Informacio/Informe%20escrit/Rubrica%20INFORME%20escrito.pdf) · [annex (checklist)](Informacio/Informe%20escrit/checklists_JC%204.pdf) · [annex (plantilla)](Informacio/Informe%20escrit/annex%20informe.docx)
- Presentació: [checklist](Informacio/Presentaci%C3%B3%20oral/checklists_JC%203.pdf) · [rúbrica](Informacio/Presentaci%C3%B3%20oral/Rubrica%20PRESENTACION%20oral.pdf)

### Informació addicional (carpeta `Informacio addicional/`)
- **Llegir primer:** [00_Elecció del tema](Informacio%20addicional/ReadMe%20First/00_Elecci%C3%B3%20del%20tema.pdf) · [01_Dades](Informacio%20addicional/ReadMe%20First/01_Dades.pdf)
- Lectures sobre treball en equip: [Assertivitat](Informacio%20addicional/Lectures/asertividad.pdf) · [Jetas y mantas](Informacio%20addicional/Lectures/jetas_y_mantas.pdf) · [Treball en equip](Informacio%20addicional/Lectures/trabajo_en_equipo.pdf)
- Més exemples de treballs: <https://www-eio.upc.es/teaching/pe/BloTrans/mas_ejemplos/>
- Idees per millorar: <https://www-eio.upc.es/teaching/pe/BloTrans/Mejorar/>
