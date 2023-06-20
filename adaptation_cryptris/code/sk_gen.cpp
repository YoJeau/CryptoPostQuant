#include <vector>
#include <fstream>
#include <iostream>
#include "sk_gen.h"
#include "operations.h"

using namespace std;

// Emplacement du fichier nécessaire et généré à l'utilisation
const string sk_path = "files/gen/sk.txt";

// Nombre de caractère(s) à chiffrer/déchiffrer à la fois
const int cryptSize = 2;

// Crée une clé secrète et l'écrit dans le fichier sk.txt
int sk_gen(void)
{
	cout << "secretKey()" << endl;

	srand(time(NULL));	// Initialise le générateur de nombres aléatoires avec l'horloge du système

	vector<int> secretKey;

	secretKey.push_back(randomNumber(10, 30));
	for(int i = 1 ; i < (cryptSize * 4) ; i++)
	{
		secretKey.push_back(randomNumber(-1, 1));
	}
	secretKey = rotation(secretKey);
	secretKey = changeSign(secretKey);

	ofstream f (sk_path.c_str());
	if(f.is_open())
	{
        for(int j = 0 ;  j < (cryptSize * 4) ; j++)
		{
			f << secretKey[j] << ".";
		}
	}
	else
	{
		cout << "Unable to open sk.txt !" << endl;
	}
	f << endl;
	f.close();

	cout << "secret key success !\n" << endl;
	
	return 0;
}