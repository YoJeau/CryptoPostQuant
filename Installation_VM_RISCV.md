# Installation d'une Machine virtuelle avec une architecture RISC-V sans le support des capabilities

# Prérequis

Sur le [site](https://www.cl.cam.ac.uk/research/security/ctsrd/cheri/cheri-llvm.html) de l'université de cambridge on nous recommande d'utiliser le [cheribuild tool](https://github.com/CTSRD-CHERI/cheribuild).

On utilise alors cet outil pour l'installation de ce projet.

## Création de la VM avec VirtualBox (7.0).

Kubuntu 20.04.5 LTS amd64 [[DDL](https://cdimage.ubuntu.com/kubuntu/releases/20.04.5/release/kubuntu-20.04.5-desktop-amd64.iso)].

Disque virtuelle de 80 GiB.

Mémoire vive allouée de 4 096 MB.

2 coeurs du processeur alloués.

Ajout des additions invités de VirtualBox :   
```
sudo apt-get install virtualbox-guest-utils virtualbox-guest-x11 virtualbox-guest-dkms
```

## Installation des outils nécessaires

Git, nous permet de copier les fichiers de l'outil cheribuild en local :

```
sudo apt install git
```

Mercurial, nécessaire pour l'utilisation de l'outil cheribuild :

```
sudo apt-get install mercurial
```

Divers outils, nécessaire pour la construction de CHERI sous Debian/Ubuntu (commande venant du [GitHub CTSRD-CHERI](https://github.com/CTSRD-CHERI/cheribuild))

```
sudo apt install autoconf automake libtool pkg-config clang bison cmake ninja-build samba flex texinfo time libglib2.0-dev libpixman-1-dev libarchive-dev libarchive-tools libbz2-dev libattr1-dev libcap-ng-dev
```

# Installation de CHERIBSD sous RISC-V sans le support des capabilities

## Construction des logiciels liés à CHERI (nécessite Python 3.6+)

On commence par copier les fichiers de l'outil CHERIBuild provenant du [GitHub CTSRD-CHERI](https://github.com/CTSRD-CHERI/cheribuild) (les fichiers seront copiés à l'endroit où l'on se situe avec le terminal):

```
git clone https://github.com/CTSRD-CHERI/cheribuild.git
```

Démarrer une VM QEMU exécutant CheriBSD sur une architecture RISC-V (-d permet de compiler toutes les dépendances) :

```
./cheribuild.py run-riscv64 -d
```

Par défaut, `cheribuild.py` va cloner tous les projets dans `~/cheri`, utiliser `~/cheri/build` pour les répertoires de construction et installer dans `~/cheri/output`.

## Connection CHERIBSD

On se connecte avec les identifiants :

login : root  
pas de mot de passe

# Cross-compilation et test des Capabilities

## Programme de test des Capabilities

On commence par créer un répertoire `projects` dans lequel sera placés les programmes de test.

Ensuite on crée un fichier texte, que l'on nomme `test_capability.c`, dans lequel on écrit :

```c
#include <stdio.h>
int x=1;
int secret_key=4096;
int main()
{
  	int *p=&x;
    p=p+1;
    printf("%d",*p);
}
```

On compile le programme `test_capability.c` avec clang :

```
clang -o test_capability test_capability.c
```

On lance le programme dans un terminal :

```
./test_capability.c
```

Ceci nous permet de vérifier que le programme fonctionne correctement.

## Cross-compilation pour RISC-V

On compile le programme  `test_capability.c` pour l'architecture RISC-V :

```
~/cheri/output/sdk/bin/clang -target riscv64-unknown-freebsd13 --sysroot=/home/cheri/cheri/output/rootfs-riscv64 -B/home/cheri/cheri/output/sdk/bin -march=rv64imafdc -mabi=lp64d -mno-relax -o test_capability_RISC-V test_capability.c
```

On copie le programme `test_capability_RISC_V` dans le répertoire `~/cheri/output/rootfs-riscv64/projects`.

On construie une nouvelle image CheriBSD pour l'architecture RISC-V :

```
./cheribuild.py disk-image-riscv64
```