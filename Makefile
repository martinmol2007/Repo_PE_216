# =============================================================================
#  Makefile - Bloc Transversal PE (FIB-UPC)
#  Ordres disponibles: make help
# =============================================================================

# ---- Missatge del commit (canvieu-lo abans de fer "make save") -------------
#  També es pot passar per terminal:  make save MSG="Afegit l'algorisme naive"
MSG = Can

# ---- Membres del grup (per posar-los com a coautors) -------------------------
#  Poseu el nom i el correu que cadascu te a GitHub (perque GitHub el reconegui)
MEMBRE_1 = Martin Molina <martinmolinapavon1970@gmail.com>
MEMBRE_2 = Membre 2 <membre2@exemple.com>
MEMBRE_3 = Membre 3 <membre3@exemple.com>

#  Coautors del commit (buit = cap):
#    make save MSG="..." AMB=2            -> afegeix el membre 2
#    make save MSG="..." AMB="2 3"        -> afegeix els membres 2 i 3
#    make save MSG="..." COAUTOR="Nom <correu>"  -> qualsevol altra persona
AMB     =
COAUTOR =
TRAILERS = $(foreach i,$(AMB),--trailer "Co-authored-by: $(MEMBRE_$(i))") \
           $(if $(COAUTOR),--trailer "Co-authored-by: $(COAUTOR)")

# ---- Compilacio -------------------------------------------------------------
CXX      = g++
CXXFLAGS = -O2 -std=c++17 -Wall
BINDIR   = bin

SRCS = $(wildcard *.cc)
BINS = $(SRCS:%.cc=$(BINDIR)/%)

# ---- Git ----------------------------------------------------------------------
BRANCA = main

.PHONY: all help run clean status pull commit push save log config

# Per defecte: compila tots els .cc de l'arrel
all: $(BINS)
	@echo ">> Compilacio feta. Executables a $(BINDIR)/"

$(BINDIR)/%: %.cc | $(BINDIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(BINDIR):
	@mkdir -p $(BINDIR)

# Executa un programa:  make run P=test
run: all
	@if [ -z "$(P)" ]; then echo "Indica el programa: make run P=nom (sense .cc)"; exit 1; fi
	./$(BINDIR)/$(P)

clean:
	rm -rf $(BINDIR)
	@echo ">> Executables esborrats"

# ---- Flux de treball en grup ----------------------------------------------------

# Configura el teu nom i correu per als commits (nomes cal la primera vegada)
#  make config NOM="Nom Cognom" EMAIL="correu@exemple.com"
config:
	@if [ -z "$(NOM)" ] || [ -z "$(EMAIL)" ]; then \
		echo 'Us: make config NOM="Nom Cognom" EMAIL="correu@exemple.com"'; exit 1; fi
	git config user.name "$(NOM)"
	git config user.email "$(EMAIL)"
	git config pull.rebase true
	@echo ">> Configuracio feta"

# Mostra l'estat del repositori
status:
	@git status -sb

# Baixa els canvis dels companys (FES-HO SEMPRE ABANS DE COMENCAR A TREBALLAR)
pull:
	@echo ">> Baixant canvis de GitHub..."
	@git pull --rebase --autostash origin $(BRANCA) || { \
		echo ""; \
		echo "!! CONFLICTE: tu i un company heu canviat les mateixes linies."; \
		echo "   1. Obre els fitxers marcats amb 'both modified' (git status)."; \
		echo "   2. Tria quina versio es queda i esborra les marques <<<<<<< ======= >>>>>>>"; \
		echo "   3. git add <fitxer>  i  git rebase --continue"; \
		echo "   Si et perds:  git rebase --abort  (tot torna com estava)"; \
		exit 1; }

# Guarda els teus canvis en un commit local (amb el missatge de MSG)
commit:
	@git add -A
	@if git diff --cached --quiet; then \
		echo ">> No hi ha canvis per guardar"; \
	else \
		git commit -m "$(MSG)" $(TRAILERS) && echo ">> Commit fet: $(MSG)"; \
	fi

# Puja els teus commits a GitHub
push:
	@git push origin $(BRANCA) || { \
		echo ""; \
		echo "!! GitHub ha rebutjat el push: un company ha pujat canvis abans."; \
		echo "   Fes  make pull  i despres  make push."; \
		exit 1; }
	@echo ">> Canvis pujats a GitHub"

# TOT EN UN: compila, fa commit, baixa els canvis dels companys i puja
save: all commit pull push
	@echo ">> Repositori guardat i sincronitzat"

# Historial dels ultims commits
log:
	@git log --oneline --graph --decorate -15

help:
	@echo "Ordres disponibles:"
	@echo "  make               Compila tots els .cc a $(BINDIR)/"
	@echo "  make run P=test    Compila i executa bin/test"
	@echo "  make clean         Esborra els executables"
	@echo ""
	@echo "  make config NOM=\"...\" EMAIL=\"...\"   Configura git (primera vegada)"
	@echo "  make status        Estat del repositori"
	@echo "  make pull          Baixa els canvis dels companys"
	@echo "  make commit        Commit local amb el missatge MSG"
	@echo "  make push          Puja els commits a GitHub"
	@echo "  make save          Compila + commit + pull + push (tot en un)"
	@echo "  make save MSG=\"...\"  Igual, amb missatge personalitzat"
	@echo "  make save MSG=\"...\" AMB=2       Amb el membre 2 com a coautor"
	@echo "  make save MSG=\"...\" AMB=\"2 3\"   Amb els membres 2 i 3 com a coautors"
	@echo "  make log           Historial de commits"
