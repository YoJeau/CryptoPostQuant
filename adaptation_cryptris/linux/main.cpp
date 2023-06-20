#include <iostream>
#include "sk_gen.h"
#include "pk_gen.h"
#include "encrypt.h"
#include "decrypt.h"

using namespace std;

int main(void)
{
	bool flag = false;
	char c;

	do
	{
		cout << "Quelle action r" << (char)130 << "aliser ?\n1 : g" << (char)130 << "n" << (char)130 << "rer des cl" << (char)130 << "s de chiffrement\n2 : chiffrer un message\n3 : d" << (char)130 << "chiffrer un message\n4 : quitter"<< endl;
		cin >> c;
		switch(c)
		{
			case '1':
				sk_gen();
				pk_gen();
				cout << "Cl" << (char)130 << "s de chiffrement g" << (char)130 << "n" << (char)130 << "r" << (char)130 << "es.\n" << endl;
				break;
			case '2':
				encrypt();
				cout << "M" << (char)130 << "ssage chiffr" << (char)130 << ".\n" << endl;
				break;
			case '3':
				decrypt();
				cout << "M" << (char)130 << "ssage d" << (char)130 << "chiffr" << (char)130 << ".\n" << endl;
				break;
			case '4':
				flag = true;
				break;
		}
	}
	while(!flag);

	return 0;
} 
