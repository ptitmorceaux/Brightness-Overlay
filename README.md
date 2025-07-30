## 🔧 Fonctionnement

Ce programme affiche une **fenêtre noire transparente** par-dessus tout le bureau, sans interagir avec les clics de souris (grâce au mode *click-through*), afin de **simuler une baisse de luminosité logicielle**. Il ne modifie pas la luminosité réelle de l'écran, mais agit comme un **"voile" graphique**.

Il lit en continu la valeur contenue dans :

* `FILE_BRIGHTNESS` : `/sys/class/backlight/intel_backlight/brightness`
* Et la valeur maximale depuis : `/sys/class/backlight/intel_backlight/max_brightness`

Ces fichiers sont présents **par défaut** sur les systèmes Linux compatibles avec les pilotes Intel via le kernel (`i915`). Ce sont les interfaces standard du kernel Linux pour gérer la luminosité des écrans (via `sysfs`).

Chaque fois que le fichier de brightness est modifié (via une commande comme `echo` ou via une touche Fn), l'opacité de la surcouche est recalculée dynamiquement.

---

## 📁 Dépendances

Ce projet **ne fonctionne que sous X11** pour le moment. Il nécessite les bibliothèques suivantes :

* `libgtk-3-dev` (GTK+ 3)
* `libglib2.0-dev` (GLib)
* `libx11-dev` (X11)
* `libxext-dev` (pour l'extension Shape de X11)

Installation (Debian/Ubuntu) :

```bash
sudo apt install libgtk-3-dev libglib2.0-dev libx11-dev libxext-dev
```

---

## 🛠️ Compilation

Le projet utilise un `Makefile` minimaliste pour compiler les fichiers C.

Voici une explication de son contenu typique :

```makefile
CC = gcc
CFLAGS = -Wall -Wextra -g `pkg-config --cflags gtk+-3.0`
LDFLAGS = `pkg-config --libs gtk+-3.0` -lX11 -lXext

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

TARGET = $(BIN_DIR)/overlay_x11

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -rf $(BUILD_DIR)/*.o

fclean: clean
	rm -f $(TARGET)

re: fclean all
```

### Détails :

* `CC`, `CFLAGS`, `LDFLAGS` : Configuration du compilateur et des dépendances.
* `SRCS` : Tous les fichiers `.c` dans `src/`
* `OBJS` : Correspondants `.o` dans `build/`
* `all` : Cible par défaut qui compile le binaire dans `bin/`
* `clean`, `fclean`, `re` : Nettoyage des fichiers objets et binaires.

### Chemin du binaire :

```bash
./bin/overlay_x11
```