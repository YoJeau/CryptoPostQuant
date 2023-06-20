#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include "encrypt.h"
#include "operations.h"
#include "ascii_trits.h"

using namespace std;

// Emplacements des différents fichiers nécessaires et générés à l'utilisation
const string msg_path = "files/message.txt";
const string pk_path = "files/gen/pk.txt";
const string ec_path = "files/gen/encrypted.txt";

// Nombre de caractère(s) à chiffrer/déchiffrer à la fois
const int cryptSize = 2;

int encrypt(void)
{
	cout << "encrypt()" << endl;

	srand(time(NULL));	// Initialise le générateur de nombres aléatoires avec l'horloge du système

	bool negativ = false;
	bool add = false;
	vector<int> publicKey;
	vector<char> message;
	vector<int> trits;
	vector<int> code;
	vector<int> encrypted;
	vector<int> alea;

// Place la clé publique dans un vecteur d'entiers
	ifstream pk(pk_path.c_str());
	if(pk.is_open())
	{
		int a = 0;
		int z = 1;
		char b;
		while(pk >> b)
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
					if(publicKey[a] < 0)
					{
						publicKey[a] = (publicKey[a] * 10) - (b - '0');
					}
					else
					{
						publicKey[a] = (publicKey[a] * 10) + (b - '0');
					}
				}
			}
			else if(negativ)
			{
				negativ = false;
				add = true;
				publicKey.push_back((b - '0') * -1);
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
					publicKey.push_back(b - '0');
                }
			}
		}
	}
	else
	{
		cout << "Unable to open pk.txt !" << endl;
	}
	pk.close();

// Place le contenu de mesage.txt dans un vecteur de caractères
	ifstream msg(msg_path.c_str());
	if(msg.is_open())
	{
		char c;
		while(msg.get(c))
		{
			if(c != '\n' && c != '\t' && c !='\r')
			{
				message.push_back(c);
			}
		}
	}
	else
	{
		cout << "Unable to open message.txt !" << endl;
	}
	msg.close();

// Appel de la fonction asciiToTrits afin de traduire le message en trits (définie dans operationscpp)
	trits = asciiToTrits(message);

	int m = (cryptSize * 4);
	int j = 0;

// Si la dernière passe n'est pas complète, ajoute des 0 à la fin du vecteur d'entiers trits
	if((trits.size() % (cryptSize * 4)) != 0)
	{		
		for(int i = 0 ; i < ((cryptSize * 4) - (trits.size() % (cryptSize * 4))) ; i++)
		{
			trits.push_back(0);
		}
	}

// Chiffrement de l'ensemble du message sous forme de trits, lot par lot, en fonction du nombre d'éléments définie par lot dans cryptSize
	while(m <= trits.size())
	{
		// Transfert un lot de trits dans le vecteur code
		for(j ; j < m ; j++)
		{
			code.push_back(trits[j]);
		}

		// Utilise les fonctions de chiffrement tant que le vecteur code n'est pas entierement chiffré
		while(find(code.begin(), code.end(), 0) != code.end() || find(code.begin(), code.end(), 1) != code.end() || find(code.begin(), code.end(), -1) != code.end())
		{
			cout << "\tencrypt : 0, 1 or -1 found" << endl;
			alea = publicKey;
			alea = rotation(alea);
			alea = changeSign(alea);
			code = addition(code, alea);
		}
		
		// Tranfert le code chiffré dans le vecteur encrypted
		for(int k = 0 ; k < (cryptSize * 4) ; k++)
		{
			encrypted.push_back(code[k]);
		}

		// Remet à zéro le vecteur code pour qu'il reçoit un autre lot
		code.clear();
		m += (cryptSize * 4);
	}

// Ecrit le message chiffré dans le fichier encrypted.txt
	ofstream ec(ec_path.c_str());
	if(ec.is_open())
	{
		for(int l = 0 ;  l < encrypted.size() ; l++)
		{
			ec << encrypted[l] << ".";
		}
	}
	else
	{
		cout << "Unable to open encrypted.txt !" << endl;
	}
	ec << endl;
	ec.close();

	cout << "encrypt success !\n" << endl;
	
	return 0;
}