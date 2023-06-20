#include <vector>
#include <fstream>
#include <iostream>
#include "pk_gen.h"
#include "operations.h"

using namespace std;

// Emplacements des différents fichiers nécessaires et générés à l'utilisation
const string sk_path = "files/gen/sk.txt";
const string pk_path = "files/gen/pk.txt";

// Nombre de caractère(s) à chiffrer/déchiffrer à la fois
const int cryptSize = 2;

// Crée une clé publique à partir de la clé secrète et l'écrit dans le fichier pk.txt
int pk_gen(void)
{
	cout << "publicKey()" << endl;

	srand(time(NULL));	// Initialise le générateur de nombres aléatoires avec l'horloge du système

	int n = randomNumber(5, 10);
	cout << "number of secret key additions : " << n << endl;

	bool negativ = false;
	bool add = false;
	vector<int> alea;
	vector<int> publicKey;
	vector<int> secretKey;

	ifstream sk(sk_path.c_str());
	if(sk.is_open())
	{
		int a = 0;
		char b;
		while(sk >> b)
		{
			if(add)
			{
				if(b == '.')
				{
					add = false;
					a++;
				}
				else
				{
					if(secretKey[a] < 0)
					{
						secretKey[a] = (secretKey[a] * 10) - (b - '0');
					}
					else
					{
						secretKey[a] = (secretKey[a] * 10) + (b - '0');
					}
				}
			}
			else if(negativ)
			{
				negativ = false;
				add = true;
				secretKey.push_back((b - '0') * -1);
			}
			else
			{
				if(b == '-')
				{
					negativ = true;
				}
				else
				{
					add = true;
					secretKey.push_back(b - '0');
				}
			}
		}
	}
	else
	{
		cout << "Unable to open sk.txt !" << endl;
	}
	sk.close();

	publicKey = secretKey;

	for(int i = 0 ; i < n ; i++)
	{
		cout << "\taddition : " << i + 1 << endl;
		alea = secretKey;
		alea = rotation(alea);
		alea = changeSign(alea);
		publicKey = addition(publicKey, alea);
	}

	ofstream pk(pk_path.c_str());
	if(pk.is_open()) {
        for(int j = 0 ;  j < (cryptSize * 4) ; j++)
		{
			pk << publicKey[j] << ".";
		}
	}
	else
	{
		cout << "Unable to open pk.txt !" << endl;
	}
	pk << endl;
	pk.close();

	cout << "public key success !\n" << endl;

	return 0;
} 
