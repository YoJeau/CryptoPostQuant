## Création de la VM

J'ai créé la VM sous virtual box à partir d'un iso de kubuntu. Elle tourne sous la version  Ubuntu 20.04.2 LTS x86_64.

Installation des vbox guest utilities :   
```sudo apt-get install virtualbox-guest-utils virtualbox-guest-x11 virtualbox-guest-dkms```

Note : Prévoir une VM avec minimum 35/40 Gio

## Installation CHERI LLVM Compiler Infrastructure

Sur le [site](https://www.cl.cam.ac.uk/research/security/ctsrd/cheri/cheri-llvm.html) de l'université de cambridge on nous dit d'installer le [cheribuild tool](https://github.com/CTSRD-CHERI/cheribuild).

Je vais donc suivre l'installation de cheribuild :

```
Debian/Ubuntu

If you are building CHERI on a Debian/Ubuntu-based machine, please install the following packages:

apt install autoconf automake libtool pkg-config clang bison cmake ninja-build samba flex texinfo time libglib2.0-dev libpixman-1-dev libarchive-dev libarchive-tools libbz2-dev libattr1-dev libcap-ng-dev mercurial

Older versions of Ubuntu may report errors when trying to install libarchive-tools. In this case try using apt install bsdtar instead.
```
Ensuite on git clone le repo : 

```git clone https://github.com/CTSRD-CHERI/cheribuild.git```

```
Installation de qemu, sdk riscv64-purecap et compiler etc

./cheribuild.py run-riscv64-purecap -d
```

Exécuter cette commande m'a permis de créer la toolchain et d'obtenir SDK situé dans ```~/cheri/output/sdk/``` ainsi que qemu avec cheribsd

int x=1;
int secret_key=4096;
int main()
{
    int *p=&x;
    p=p+1;
    printf("%d",*p);
}

## Crosscompilation

Pour compiler pour du riscv64 purecap: 

```
~/cheri/output/sdk/bin/clang -target riscv64-unknown-freebsd13 --sysroot=/home/cheri/cheri/output/rootfs-riscv64-purecap -B/home/cheri/cheri/output/sdk/bin -march=rv64imafdcxcheri -mabi=l64pc128d -mno-relax prog.c -o prog
```



## Partage de fichier

Je voulais faire un dossier partagé, mais je n'y arrive pas. Je recrée donc une image avec mes projets à l'intérieur.
[Source](https://nsrutherford.gitlab.io/note/cheri/configure/)

```
# make some projects in folder ~/projects

# copy project to the root file system of the cheribsd version you are using (I use morello-purecap here)
cp -r ~/projects/ ~/cheri/output/rootfs-riscv64-purecap/projects
 
# build new cheribsd image for you desired mode
cd ~/Desktop/cheribuild/
./cheribuild.py disk-image-riscv64-purecap

# you may get a prompt asking you to confirm if you want to include your projects directory in the image, type 'y'

# run cheribsd
./cheribuild.py run-riscv64-purecap

# in cheribsd
cd /projects
```

## Connection CHERIBSD

On se connecte avec les identifiants :

login : root  
pas de mot de passe