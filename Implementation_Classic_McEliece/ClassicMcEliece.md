# Algorithme POST QUANTUM 2 : Classic McEliece

Avec l'arrivée des ordinateurs quantiques, les algorithmees à clé publiques utilisés ne sont plus sur et deviennent facilement cassables avec l'utilisation d'un ordinateur quantique. Pour résoudre à ce problème le NIST (National Institute of Standards and Technology) à ouvert un concours pour trouver des remplacements aux algorithmes à clé publiques actuels (RSA, Diffie Hellman, DSA, ECC).

McEliece est un chiffrement à clé publique inventé en 1978. C'était un des premiers programmes a utiliser de l'aléatoire dans le chiffrement. Il a été rarement utilisé dans la pratique en raison d'une clé de chiffrement particulièrement grande et un message chiffré bien plus long que l'original. 

Il repose sur un problème très différent des algorithmes asymétriques (factorisation) et n'est donc pas affecté par la percée dans ce domaine des ordinateurs quantiques.

Il a donc été amélioré et utilisé pour le NIST.

"
Classic McEliece brings all of this together. It is a KEM designed for IND-CCA2 security at a very high security level, even against quantum computers. The KEM is built conservatively from a PKE designed for OW-CPA security, namely Niederreiter's dual version of McEliece's PKE using binary Goppa codes. Every level of the construction is designed so that future cryptographic auditors can be confident in the long-term security of post-quantum public-key encryption.
"  
KEM: key encapsulation mechanism.  
IND-CCA2 security : Indistinguishability under chosen ciphertext attack/adaptive chosen ciphertext attack  (Attaque à texte chiffré choisi)  
PKE : Public Key Cryptography  
OW-CPA : One-Wayness Against Chosen-Plaintext Attack


[Site de l'algorithme](https://classic.mceliece.org/)  


# Implementation

|                   | Public Key | Private Key | Ciphertext | Session key |
|-------------------|------------|-------------|------------|-------------|
| mceliece348864    | 261120     | 6492        | 96         | 32          |
| mceliece348864f   | 261120     | 6492        | 96         | 32          |
| mceliece460896    | 524160     | 13608       | 156        | 32          |
| mceliece460896f   | 524160     | 13608       | 156        | 32          |
| mceliece6688128   | 1044992    | 13932       | 208        | 32          |
| mceliece6688128f  | 1044992    | 13932       | 208        | 32          |
| mceliece6960119   | 1047319    | 13948       | 194        | 32          |
| mceliece6960119f  | 1047319    | 13948       | 194        | 32          |
| mceliece8192128   | 1357824    | 14120       | 208        | 32          |
| mceliece8192128f  | 1357824    | 14120       | 208        | 32          |

Known Answer Tests et Code : https://csrc.nist.gov/Projects/post-quantum-cryptography/round-4-submissions

On va utiliser le mceliece348864 et le mceliece8192128 pour nos tests.

## Implémentation mcelice348864

1. Installer <openssl/conf.h> si non accessible (```sudo apt install libssl-dev```)
1. Récupérer le .zip Keccak de ce repository.
2. Placer le dossier Keccak dans le dossier contenant l'implémentation à crosscompiler
3. Modifier crypto_hash.h pour include Keccak/SimpleFIPS202.h
4. Remplacer le -lkeccak dans le script build par :  
```Keccak/SimpleFIPS202.c Keccak/KeccakSponge.c Keccak/KeccakP-1600-compact64.c```
5. Si vous voulez crosscompiler, remplacer gcc et target par votre cible de crosscompilation (voir exemple ci dessous)


### Erreurs rencontrées


```
nist/rng.c:9:10: fatal error: openssl/conf.h: No such file or directory
    9 | #include <openssl/conf.h>
      |          ^~~~~~~~~~~~~~~~
compilation terminated.
In file included from operations.c:5:
crypto_hash.h:3:10: fatal error: libkeccak.a.headers/SimpleFIPS202.h: No such file or directory
    3 | #include <libkeccak.a.headers/SimpleFIPS202.h>
      |          ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
```

Installation de openssl/conf.h
```sudo apt install libssl-dev```

Installation de xsltproc
Utilisation du script [ici](https://groups.google.com/a/list.nist.gov/g/pqc-forum/c/qu_Y0bs3dnw?pli=1) pour l'installation 
```sh
cd
git clone https://github.com/XKCP/XKCP.git
cd XKCP
time make AVX2/libXKCP.a
time make AVX2/libXKCP.so

mkdir -p $HOME/include
mkdir -p $HOME/lib
ln -s $HOME/XKCP/bin/AVX2/libXKCP.a.headers $HOME/include/libkeccak.a.headers
ln -s $HOME/XKCP/bin/AVX2/libXKCP.a $HOME/lib/libkeccak.a
ln -s $HOME/XKCP/bin/AVX2/libXKCP.so $HOME/lib/libkeccak.so

export CPATH="$CPATH:$HOME/include"
export LIBRARY_PATH="$LIBRARY_PATH:$HOME/lib"
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$HOME/lib"

cd
wget https://classic.mceliece.org/nist/mceliece-20221023.tar.gz
tar -xf mceliece-20221023.tar.gz
wget https://classic.mceliece.org/nist/mceliece-kat-20221023.tar.gz
tar -xf mceliece-kat-20221023.tar.gz

cd mceliece-20221023
for x in *Impl*/kem/mceliece*
do
find "$x" -type d \
| while read dir
do
[ -e "$dir"/Makefile ] || continue
( cd "$dir"
echo "$dir"
make
for kat in kat_kem.req kat_kem.rsp kat_kem.int
do
p=`basename "$x"`
cmp $kat "$HOME/mceliece-kat-20221023/KAT/kem/$p/$kat"
done
)
done 
done
```

Avec ça j'ai pu faire tourner le Classic McEliece sous ubuntu.

Pour cross compiler j'ai mis les librairies (du moins la librairie libkeccak) dans rootfs-riscv64-purecap/usr/lib (libkeccak.a et libkecckak.so) et /usr/include (libkeccak.a.headers)
```
ld: error: /home/cheri/cheri/output/rootfs-riscv64-purecap/usr/lib64c/libkeccak.so is incompatible with elf64lriscv
```  
Donc il faudrait pouvoir compiler cette librairie pour qu'elle soit compatible avec de l'elf64lriscv.  

Piste:  
Librairie keccak-f1600 pour le risc ? https://github.com/Ko-/riscvcrypto


J'ai décidé de récupérer les fichiers dont le programme a besoin pour faire tourner Keccak.

crypto_hash.h
```c
/* This file uses SHAKE256 implemented in the Keccak Code Package */

#include <libkeccak.a.headers/SimpleFIPS202.h> 

#define crypto_hash_32b(out,in,inlen) \
  SHAKE256(out,32,in,inlen)

#define shake(out,outlen,in,inlen) \
  SHAKE256(out,outlen,in,inlen)
```

operations.c include crypto hash ! 

observons XKCP/lib/high/Keccak/FIPS202

SimpleFIPS202.h
> Include config.h

SimpleFIPS202.c
> Include KeccakSponge.h et SimpleFIPS202.h

KeccakHash.h
> Include config.h et KeccakSponge.h

KeccakHash.c
> Include KeccakHash.h

Dans XKCP/lib/high/Keccak/

KeccakSponge.h
> Incude aligh.h et config.h

Dans XKCP/common
align.h


J'ai ensuite regardé le makefile.build et j'obtiens un lien vers :
XKCP/doc/HOWTO-customize.build
> Je n'ai pas l'impression que ça permet de choisir quelle compatiblité c'est pour la librarie a build, mais juste ça permet de faire une librairie plus petite pour qu'on ai pas un gros pack


Solution pour court circuiter ce problème de librarie :

Je récupère les fichiers suivants :
align.h brg_endian.h config.h (modifié) KeccakHash.c KeccakHash.h KeccapP-1600-compact64.c KeccakP-1600-SnP.h KeccakSponge.c KeccakSponge.h KeccakSponge.inc SimpleFIPS202.c SimpleFIPS202.h
J'ai créé un makefile et constaté que je réussi a compiler le SimpleFIPS202.c (dont les fonctions sont utilisés dans crypto_hash ) pour du cheri riscv64.  

Ensuite j'ai récupéré tout ces fichiers et je les ai mis dans le dossier mceliece348864/Keccak
Ensuite j'ai modifié le script build :
```sh
#!/bin/sh
~/cheri/output/sdk/bin/clang -target riscv64-unknown-freebsd13 --sysroot=/home/cheri/cheri/output/rootfs-riscv64-purecap -B/home/cheri/cheri/output/sdk/bin -march=rv64imafdcxcheri -mabi=l64pc128d -mno-relax -Wall -Wextra -Wno-unused-function -Wno-unused-parameter -Wno-sign-compare -I. -Isubroutines -DKAT -DKATNUM=`cat KATNUM` "-DCRYPTO_NAMESPACE(x)=x" "-D_CRYPTO_NAMESPACE(x)=_##x" -o kat nist/kat_kem.c nist/rng.c benes.c bm.c controlbits.c decrypt.c encrypt.c gf.c operations.c pk_gen.c root.c sk_gen.c synd.c transpose.c SimpleFIPS202.c KeccakSponge.c KeccakP-1600-compact64.c util.c -lcrypto -ldl 
```
Ainsi que le fichier crypto_hash.h :
```c
/* This file uses SHAKE256 implemented in the Keccak Code Package */

#include "SimpleFIPS202.h" 

#define crypto_hash_32b(out,in,inlen) \
  SHAKE256(out,32,in,inlen)

#define shake(out,outlen,in,inlen) \
  SHAKE256(out,outlen,in,inlen)
```

On peut voir que j'ai remplacé le -lkeccak par les fichiers SimpleFIPS202.c KeccakSponge.c KeccakP-1600-compact64.c et que a la place d'include un header une bibliothèque j'include un des fichiers rajouté.

Suite à cela j'arrive à avoir un exécutable kat, que j'ai mis sur la VM qemu.

## Temps d'exécution

Problème : C'est long à éxécuter, très long notament parce que ça génère un fichier avec 5 356 212 caractères.

```
time ./kat kem/mceliece348864 > kat_kem.int 8> kat_kem.req 9> kat_kem.rsp
     4332.37 real      4308.64 user         0.27 sys

root@cheribsd-riscv64-purecap:/projects # time ./kat kem/mceliece348864 > kat_kem.int 8> kat_kem.req 9> kat_kem.rsp
     4012.89 real      3991.77 user         0.18 sys
time ./kat kem/mceliece348864 > kat_kem.int 8> kat_kem.req 9> kat_kem.rsp 
     4478.00 real      4459.90 user         0.24 sys
```
Et après avoir comparé les kat_kem int req et rsp avec les attendus, on a les mêmes.
Sous ubuntu :
```
real    1,206s        user    0,979s     sys     0,012s
```

Idée : Voir si les ressources pour la VM QEMU sont bien allouées
