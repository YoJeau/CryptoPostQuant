#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <conio.h>

using namespace std;              // Domaine de nommage standard

const string messageIn = "C:/Users/greta/Documents/Stage/algorithme_cryptris/files/message.txt" ;
const string messageOut = "C:/Users/greta/Documents/Stage/algorithme_cryptris/files/decrypted_message.txt" ;
const string sk = "C:/Users/greta/Documents/Stage/algorithme_cryptris/files/sk.txt" ;
const string pk = "C:/Users/greta/Documents/Stage/algorithme_cryptris/files/pk.txt" ;
const int cryptSize = 2 ;

int randomNumber(int min, int max) {

	return min + (rand() % (max - min + 1)); // Génère un nombre aléatoire entre min et max
}
// renvoie un nombre aléatoire.

vector<char> parse() {
	vector<char> message ;
	message.resize(cryptSize) ;
	ifstream f (messageIn.c_str()) ;
	if(!f.is_open()) {
		cout << "Unable to open the file !" << endl ;
	}
	else {
		for(int i = 0 ; i < (cryptSize * 4) ; i ++) {
			f.get(message[i]) ;
		}
	}
	f.close() ;
	return message ;
}
// retourne sous forme de tableau deux octets du fichier lu.

void parse(vector<char> text) {
	ofstream f (messageOut.c_str()) ;
	if(!f.is_open()) {
		cout << "Unable to open the file !" << endl ;
	}
	else {
		for(int i = 0 ;  i < (cryptSize * 4) ; i ++) {
			f << text[i] ;
		}
	}
	f << endl ;
	f.close() ;
	return ;
}
// écrit dans un fichier texte les deux caractères déchiffrés.

vector<int> rotation(vector<int> alea) {
	vector<int> r ;
	r.resize(cryptSize * 4) ;
	int n = randomNumber(0, 7) ;
	if(n != 0) {
		for(int i = 0 ; i < (cryptSize * 4) ; i ++) {
			r[(i + n) % (cryptSize * 4)] = alea[i] ;
		}
		return r ;
	}
	else {
		return alea ;
	}
}
// fait n rotation dans le vecteur (12345678 -> 81234567), n aléatoire de 0 à 7.

vector<int> rotation(vector<int> code, vector<int> alea) {
	vector<int> r ;
	r.resize(cryptSize * 4) ;
	int distance1 = 0 ;
	int distance2 = 0 ;
	for(int i=1 ; i < (cryptSize * 4) ; i ++) {
		if(abs(code[i])>abs(code[distance1])) {
			distance1 = i;
		}
		if(abs(alea[i])>abs(alea[distance2])) {
			distance2 = i;
		}
	}
	int n = (distance1 + (cryptSize * 4) - distance2) % (cryptSize * 4) ;
	if(distance1 != distance2) {
		for(int i = 0 ; i < (cryptSize * 4) ; i ++) {
			r[(i + n) % (cryptSize * 4)] = alea[i] ;
		}
		return r ;
	}
	else {
		return alea ;
	}
}
// fait n rotation dans le vecteur pour ciblé le plus grand nombre du code.

vector<int> changeSign(vector<int> message) {
	if(randomNumber(0, 1) == 1) {
		for(int i = 0 ; i <(cryptSize * 4) ; i ++) {
			message[i] *= -1 ;
		}
	}
	return message ;
}
// tire un nombre aléatoire entre 0 et 1.
// - Si 1, renvoie le vecteur en inversant les signes des valeurs du vecteur.
// - Si 0, fin

vector<int> changeSign(vector<int> code, vector<int> alea) {
	int distance = 0 ;
	for(int i=1 ; i < (cryptSize * 4) ; i ++) {
		if(abs(alea[i])>abs(alea[distance])) {
			distance = i;
		}
	}
	if(code[distance] < 0.0 != alea[distance] < 0.0) {
		for(int j = 0 ; j < (cryptSize * 4) ; j++) {
			alea[j] *= -1 ;
		}
	}
	return alea ;
}
// si la plus grande valeur du code et la plus grande valeur d’alea sont du même signe,
// alors, inverse les signes des valeurs du vecteur alea.
// puis retourne le vecteur alea.

vector<int> addition(vector<int> a, vector<int> b) {
	for(int i = 0 ; i < (cryptSize * 4) ; i ++) {
		a[i] += b[i] ;
	}
	return a ;
}
// renvoie l’addition des valeurs du 1er argument aux valeurs du 2eme argument.

vector<int> asciiToTrits(vector<char> ascii) {
	vector<int> message ;
	for(int i = 0 ; i < (cryptSize) ; i ++) {
		switch(ascii[i]) {
			case ' ' :
				message.push_back( 0 ) ;
				message.push_back( 0 ) ;
				message.push_back( 0 ) ;
				message.push_back( 0 ) ;
				break ;
			case '0' :
				message.push_back( 1 ) ;
				message.push_back( 0 ) ;
				message.push_back( 0 ) ;
				message.push_back( 0 ) ;
				break ;
			case '1' :
				message.push_back( -1 ) ;
				message.push_back( 0 ) ;
				message.push_back( 0 ) ;
				message.push_back( 0 ) ;
				break ;
			case '2' :
				message.push_back( 0 ) ;
				message.push_back( 1 ) ;
				message.push_back( 0 ) ;
				message.push_back( 0 ) ;
				break ;
			case '3' :
				message.push_back( 1 ) ;
				message.push_back( 1 ) ;
				message.push_back( 0 ) ;
				message.push_back( 0 ) ;
				break ;
			case '4' :
				message.push_back( -1 ) ;
				message.push_back( 1 ) ;
				message.push_back( 0 ) ;
				message.push_back( 0 ) ;
				break ;
			case '5' :
				message.push_back( 0 ) ;
				message.push_back( -1 ) ;
				message.push_back( 0 ) ;
				message.push_back( 0 ) ;
				break ;
			case '6' :
				message.push_back( 1 ) ;
				message.push_back( -1 ) ;
				message.push_back( 0 ) ;
				message.push_back( 0 ) ;
				break ;
			case '7' :
				message.push_back( -1 ) ;
				message.push_back( -1 ) ;
				message.push_back( 0 ) ;
				message.push_back( 0 ) ;
				break ;
			case '8' :
				message.push_back( 0 ) ;
				message.push_back( 0 ) ;
				message.push_back( 1 ) ;
				message.push_back( 0 ) ;
				break ;
			case '9' :
				message.push_back( 1 ) ;
				message.push_back( 0 ) ;
				message.push_back( 1 ) ;
				message.push_back( 0 ) ;
				break ;
			case 'a' :
				message.push_back( -1 ) ;
				message.push_back( 0 ) ;
				message.push_back( 1 ) ;
				message.push_back( 0 ) ;
				break ;
			case 'b' :
				message.push_back( 0 ) ;
				message.push_back( 1 ) ;
				message.push_back( 1 ) ;
				message.push_back( 0 ) ;
				break ;
			case 'c' :
				message.push_back( 1 ) ;
				message.push_back( 1 ) ;
				message.push_back( 1 ) ;
				message.push_back( 0 ) ;
				break ;
			case 'd' :
				message.push_back( -1 ) ;
				message.push_back( 1 ) ;
				message.push_back( 1 ) ;
				message.push_back( 0 ) ;
				break ;
			case 'e' :
				message.push_back( 0 ) ;
				message.push_back( -1 ) ;
				message.push_back( 1 ) ;
				message.push_back( 0 ) ;
				break ;
			case 'f' :
				message.push_back( 1 ) ;
				message.push_back( -1 ) ;
				message.push_back( 1 ) ;
				message.push_back( 0 ) ;
				break ;
			case 'g' :
				message.push_back( -1 ) ;
				message.push_back( -1 ) ;
				message.push_back( 1 ) ;
				message.push_back( 0 ) ;
				break ;
			case 'h' :
				message.push_back( 0 ) ;
				message.push_back( 0 ) ;
				message.push_back( -1 ) ;
				message.push_back( 0 ) ;
				break ;
			case 'i' :
				message.push_back( 1 ) ;
				message.push_back( 0 ) ;
				message.push_back( -1 ) ;
				message.push_back( 0 ) ;
				break ;
			case 'j' :
				message.push_back( -1 ) ;
				message.push_back( 0 ) ;
				message.push_back( -1 ) ;
				message.push_back( 0 ) ;
				break ;
			case 'k' :
				message.push_back( 0 ) ;
				message.push_back( 1 ) ;
				message.push_back( -1 ) ;
				message.push_back( 0 ) ;
				break ;
			case 'l' :
				message.push_back( 1 ) ;
				message.push_back( 1 ) ;
				message.push_back( -1 ) ;
				message.push_back( 0 ) ;
				break ;
			case 'm' :
				message.push_back( -1 ) ;
				message.push_back( 1 ) ;
				message.push_back( -1 ) ;
				message.push_back( 0 ) ;
				break ;
			case 'n' :
				message.push_back( 0 ) ;
				message.push_back( -1 ) ;
				message.push_back( -1 ) ;
				message.push_back( 0 ) ;
				break ;
			case 'o' :
				message.push_back( 1 ) ;
				message.push_back( -1 ) ;
				message.push_back( -1 ) ;
				message.push_back( 0 ) ;
				break ;
			case 'p' :
				message.push_back( -1 ) ;
				message.push_back( -1 ) ;
				message.push_back( -1 ) ;
				message.push_back( 0 ) ;
				break ;
			case 'q' :
				message.push_back( 0 ) ;
				message.push_back( 0 ) ;
				message.push_back( 0 ) ;
				message.push_back( 1 ) ;
				break ;
			case 'r' :
				message.push_back( 1 ) ;
				message.push_back( 0 ) ;
				message.push_back( 0 ) ;
				message.push_back( 1 ) ;
				break ;
			case 's' :
				message.push_back( -1 ) ;
				message.push_back( 0 ) ;
				message.push_back( 0 ) ;
				message.push_back( 1 ) ;
				break ;
			case 't' :
				message.push_back( 0 ) ;
				message.push_back( 1 ) ;
				message.push_back( 0 ) ;
				message.push_back( 1 ) ;
				break ;
			case 'u' :
				message.push_back( 1 ) ;
				message.push_back( 1 ) ;
				message.push_back( 0 ) ;
				message.push_back( 1 ) ;
				break ;
			case 'v' :
				message.push_back( -1 ) ;
				message.push_back( 1 ) ;
				message.push_back( 0 ) ;
				message.push_back( 1 ) ;
				break ;
			case 'w' :
				message.push_back( 0 ) ;
				message.push_back( -1 ) ;
				message.push_back( 0 ) ;
				message.push_back( 1 ) ;
				break ;
			case 'x' :
				message.push_back( 1 ) ;
				message.push_back( -1 ) ;
				message.push_back( 0 ) ;
				message.push_back( 1 ) ;
				break ;
			case 'y' :
				message.push_back( -1 ) ;
				message.push_back( -1 ) ;
				message.push_back( 0 ) ;
				message.push_back( 1 ) ;
				break ;
			case 'z' :
				message.push_back( 0 ) ;
				message.push_back( 0 ) ;
				message.push_back( 1 ) ;
				message.push_back( 1 ) ;
				break ;
			case 'A' :
				message.push_back( 1 ) ;
				message.push_back( 0 ) ;
				message.push_back( 1 ) ;
				message.push_back( 1 ) ;
				break ;
			case 'B' :
				message.push_back( -1 ) ;
				message.push_back( 0 ) ;
				message.push_back( 1 ) ;
				message.push_back( 1 ) ;
				break ;
			case 'C' :
				message.push_back( 0 ) ;
				message.push_back( 1 ) ;
				message.push_back( 1 ) ;
				message.push_back( 1 ) ;
				break ;
			case 'D' :
				message.push_back( 1 ) ;
				message.push_back( 1 ) ;
				message.push_back( 1 ) ;
				message.push_back( 1 ) ;
				break ;
			case 'E' :
				message.push_back( -1 ) ;
				message.push_back( 1 ) ;
				message.push_back( 1 ) ;
				message.push_back( 1 ) ;
				break ;
			case 'F' :
				message.push_back( 0 ) ;
				message.push_back( -1 ) ;
				message.push_back( 1 ) ;
				message.push_back( 1 ) ;
				break ;
			case 'G' :
				message.push_back( 1 ) ;
				message.push_back( -1 ) ;
				message.push_back( 1 ) ;
				message.push_back( 1 ) ;
				break ;
			case 'H' :
				message.push_back( -1 ) ;
				message.push_back( -1 ) ;
				message.push_back( 1 ) ;
				message.push_back( 1 ) ;
				break ;
			case 'I' :
				message.push_back( 0 ) ;
				message.push_back( 0 ) ;
				message.push_back( -1 ) ;
				message.push_back( 1 ) ;
				break ;
			case 'J' :
				message.push_back( 1 ) ;
				message.push_back( 0 ) ;
				message.push_back( -1 ) ;
				message.push_back( 1 ) ;
				break ;
			case 'K' :
				message.push_back( -1 ) ;
				message.push_back( 0 ) ;
				message.push_back( -1 ) ;
				message.push_back( 1 ) ;
				break ;
			case 'L' :
				message.push_back( 0 ) ;
				message.push_back( 1 ) ;
				message.push_back( -1 ) ;
				message.push_back( 1 ) ;
				break ;
			case 'M' :
				message.push_back( 1 ) ;
				message.push_back( 1 ) ;
				message.push_back( -1 ) ;
				message.push_back( 1 ) ;
				break ;
			case 'N' :
				message.push_back( -1 ) ;
				message.push_back( 1 ) ;
				message.push_back( -1 ) ;
				message.push_back( 1 ) ;
				break ;
			case 'O' :
				message.push_back( 0 ) ;
				message.push_back( -1 ) ;
				message.push_back( -1 ) ;
				message.push_back( 1 ) ;
				break ;
			case 'P' :
				message.push_back( 1 ) ;
				message.push_back( -1 ) ;
				message.push_back( -1 ) ;
				message.push_back( 1 ) ;
				break ;
			case 'Q' :
				message.push_back( -1 ) ;
				message.push_back( -1 ) ;
				message.push_back( -1 ) ;
				message.push_back( 1 ) ;
				break ;
			case 'R' :
				message.push_back( 0 ) ;
				message.push_back( 0 ) ;
				message.push_back( 0 ) ;
				message.push_back( -1 ) ;
				break ;
			case 'S' :
				message.push_back( 1 ) ;
				message.push_back( 0 ) ;
				message.push_back( 0 ) ;
				message.push_back( -1 ) ;
				break ;
			case 'T' :
				message.push_back( -1 ) ;
				message.push_back( 0 ) ;
				message.push_back( 0 ) ;
				message.push_back( -1 ) ;
				break ;
			case 'U' :
				message.push_back( 0 ) ;
				message.push_back( 1 ) ;
				message.push_back( 0 ) ;
				message.push_back( -1 ) ;
				break ;
			case 'V' :
				message.push_back( 1 ) ;
				message.push_back( 1 ) ;
				message.push_back( 0 ) ;
				message.push_back( -1 ) ;
				break ;
			case 'W' :
				message.push_back( -1 ) ;
				message.push_back( 1 ) ;
				message.push_back( 0 ) ;
				message.push_back( -1 ) ;
				break ;
			case 'X' :
				message.push_back( 0 ) ;
				message.push_back( -1 ) ;
				message.push_back( 0 ) ;
				message.push_back( -1 ) ;
				break ;
			case 'Y' :
				message.push_back( 1 ) ;
				message.push_back( -1 ) ;
				message.push_back( 0 ) ;
				message.push_back( -1 ) ;
				break ;
			case 'Z' :
				message.push_back( -1 ) ;
				message.push_back( -1 ) ;
				message.push_back( 0 ) ;
				message.push_back( -1 ) ;
				break ;
			case '.' :
				message.push_back( 0 ) ;
				message.push_back( 0 ) ;
				message.push_back( 1 ) ;
				message.push_back( -1 ) ;
				break ;
			case ',' :
				message.push_back( 1 ) ;
				message.push_back( 0 ) ;
				message.push_back( 1 ) ;
				message.push_back( -1 ) ;
				break ;
			case '!' :
				message.push_back( -1 ) ;
				message.push_back( 0 ) ;
				message.push_back( 1 ) ;
				message.push_back( -1 ) ;
				break ;
			case '?' :
				message.push_back( 0 ) ;
				message.push_back( 1 ) ;
				message.push_back( 1 ) ;
				message.push_back( -1 ) ;
				break ;
			case ':' :
				message.push_back( 1 ) ;
				message.push_back( 1 ) ;
				message.push_back( 1 ) ;
				message.push_back( -1 ) ;
				break ;
			case ';' :
				message.push_back( -1 ) ;
				message.push_back( 1 ) ;
				message.push_back( 1 ) ;
				message.push_back( -1 ) ;
				break ;
			case '*' :
				message.push_back( 0 ) ;
				message.push_back( -1 ) ;
				message.push_back( 1 ) ;
				message.push_back( -1 ) ;
				break ;
			case '+' :
				message.push_back( 1 ) ;
				message.push_back( -1 ) ;
				message.push_back( 1 ) ;
				message.push_back( -1 ) ;
				break ;
			case '-' :
				message.push_back( -1 ) ;
				message.push_back( -1 ) ;
				message.push_back( 1 ) ;
				message.push_back( -1 ) ;
				break ;
			case '/' :
				message.push_back( 0 ) ;
				message.push_back( 0 ) ;
				message.push_back( -1 ) ;
				message.push_back( -1 ) ;
				break ;
			case '=' :
				message.push_back( 1 ) ;
				message.push_back( 0 ) ;
				message.push_back( -1 ) ;
				message.push_back( -1 ) ;
				break ;
			case '<' :
				message.push_back( -1 ) ;
				message.push_back( 0 ) ;
				message.push_back( -1 ) ;
				message.push_back( -1 ) ;
				break ;
			case '>' :
				message.push_back( 0 ) ;
				message.push_back( 1 ) ;
				message.push_back( -1 ) ;
				message.push_back( -1 ) ;
				break ;
			case '@' :
				message.push_back( 1 ) ;
				message.push_back( 1 ) ;
				message.push_back( -1 ) ;
				message.push_back( -1 ) ;
				break ;
			case '#' :
				message.push_back( -1 ) ;
				message.push_back( 1 ) ;
				message.push_back( -1 ) ;
				message.push_back( -1 ) ;
				break ;
			case '$' :
				message.push_back( 0 ) ;
				message.push_back( -1 ) ;
				message.push_back( -1 ) ;
				message.push_back( -1 ) ;
				break ;
			case '%' :
				message.push_back( 1 ) ;
				message.push_back( -1 ) ;
				message.push_back( -1 ) ;
				message.push_back( -1 ) ;
				break ;
			case '&' :
				message.push_back( -1 ) ;
				message.push_back( -1 ) ;
				message.push_back( -1 ) ;
				message.push_back( -1 ) ;
				break ;
		}
	}
	ofstream f ("C:/Users/greta/Documents/Stage/algorithme_cryptris/files/asciiToTrits.txt") ;
	if(!f.is_open()) {
		cout << "Unable to open the file !" << endl ;
	}
	else {
		f << "ascii : " ;
		for(int i = 0 ;  i < cryptSize ; i ++) {
			f << ascii[i] ;
		}
		f << "\ntrits : " ;
		for(int j = 0 ;  j < (cryptSize * 4) ; j ++) {
			f << message[j] << " " ;
		}
	}
	f << endl ;
	f.close() ;
	return message ;
}
// traduit les symboles ASCII en trits (-1, 0, 1).

vector<char> tritsToAscii(vector<int> trits) {
	vector<char> ascii ;
	ascii.resize(2) ;
	int n = 0 ;
	for(int i = 0 ; i < (cryptSize ) ; i ++) {
		if(trits[3 + n] == 0) {
			if(trits[2 + n] == 0) {
				if(trits[1 + n] == 0) {
					if(trits[0 + n] == 0) {
						ascii[i] = ' ' ;
					}
					else if(trits[0 + n] == 1) {
						ascii[i] = '0' ;
					}
					else {
						ascii[i] = '1' ;
					}
				}
				else if(trits[1 + n] == 1) {
					if(trits[0 + n] == 0) {
						ascii[i] = '2' ;
					}
					else if(trits[0 + n] == 1) {
						ascii[i] = '3' ;
					}
					else {
						ascii[i] = '4' ;
					}
				}
				else {
					if(trits[0 + n] == 0) {
						ascii[i] = '5' ;
					}
					else if(trits[0 + n] == 1) {
						ascii[i] = '6' ;
					}
					else {
						ascii[i] = '7' ;
					}
				}
			}
			else if(trits[2 + n] == 1) {
				if(trits[1 + n] == 0) {
					if(trits[0 + n] == 0) {
						ascii[i] = '8' ;
					}
					else if(trits[0 + n] == 1) {
						ascii[i] = '9' ;
					}
					else {
						ascii[i] = 'a' ;
					}
				}
				else if(trits[1 + n] == 1) {
					if(trits[0 + n] == 0) {
						ascii[i] = 'b' ;
					}
					else if(trits[0 + n] == 1) {
						ascii[i] = 'c' ;
					}
					else {
						ascii[i] = 'd' ;
					}
				}
				else {
					if(trits[0 + n] == 0) {
						ascii[i] = 'e' ;
					}
					else if(trits[0 + n] == 1) {
						ascii[i] = 'f' ;
					}
					else {
						ascii[i] = 'g' ;
					}
				}
			}
			else {
				if(trits[1 + n] == 0) {
					if(trits[0 + n] == 0) {
						ascii[i] = 'h' ;
					}
					else if(trits[0 + n] == 1) {
						ascii[i] = 'i' ;
					}
					else {
						ascii[i] = 'j' ;
					}
				}
				else if(trits[1 + n] == 1) {
					if(trits[0 + n] == 0) {
						ascii[i] = 'k' ;
					}
					else if(trits[0 + n] == 1) {
						ascii[i] = 'l' ;
					}
					else {
						ascii[i] = 'm' ;
					}
				}
				else {
					if(trits[0 + n] == 0) {
						ascii[i] = 'n' ;
					}
					else if(trits[0 + n] == 1) {
						ascii[i] = 'o' ;
					}
					else {
						ascii[i] = 'p' ;
					}
				}
			}
}
		else if(trits[3 + n] == 1) {
			if(trits[2 + n] == 0) {
				if(trits[1 + n] == 0) {
					if(trits[0 + n] == 0) {
						ascii[i] = 'q' ;
					}
					else if(trits[0 + n] == 1) {
						ascii[i] = 'r' ;
					}
					else {
						ascii[i] = 's' ;
					}
				}
				else if(trits[1 + n] == 1) {
					if(trits[0 + n] == 0) {
						ascii[i] = 't' ;
					}
					else if(trits[0 + n] == 1) {
						ascii[i] = 'u' ;
					}
					else {
						ascii[i] = 'v' ;
					}
				}
				else {
					if(trits[0 + n] == 0) {
						ascii[i] = 'w' ;
					}
					else if(trits[0 + n] == 1) {
						ascii[i] = 'x' ;
					}
					else {
						ascii[i] = 'y' ;
					}
				}
			}
			else if(trits[2 + n] == 1) {
				if(trits[1 + n] == 0) {
					if(trits[0 + n] == 0) {
						ascii[i] = 'z' ;
					}
					else if(trits[0 + n] == 1) {
						ascii[i] = 'A' ;
					}
					else {
						ascii[i] = 'B' ;
					}
				}
				else if(trits[1 + n] == 1) {
					if(trits[0 + n] == 0) {
						ascii[i] = 'C' ;
					}
					else if(trits[0 + n] == 1) {
						ascii[i] = 'D' ;
					}
					else {
						ascii[i] = 'E' ;
					}
				}
				else {
					if(trits[0 + n] == 0) {
						ascii[i] = 'F' ;
					}
					else if(trits[0 + n] == 1) {
						ascii[i] = 'G' ;
					}
					else {
						ascii[i] = 'H' ;
					}
				}
			}
			else {
				if(trits[1 + n] == 0) {
					if(trits[0 + n] == 0) {
						ascii[i] = 'I' ;
					}
					else if(trits[0 + n] == 1) {
						ascii[i] = 'J' ;
					}
					else {
						ascii[i] = 'K' ;
					}
				}
				else if(trits[1 + n] == 1) {
					if(trits[0 + n] == 0) {
						ascii[i] = 'L' ;
					}
					else if(trits[0 + n] == 1) {
						ascii[i] = 'M' ;
					}
					else {
						ascii[i] = 'N' ;
					}
				}
				else {
					if(trits[0 + n] == 0) {
						ascii[i] = 'O' ;
					}
					else if(trits[0 + n] == 1) {
						ascii[i] = 'P' ;
					}
					else {
						ascii[i] = 'Q' ;
					}
				}
			}
		}
		else {
			if(trits[2 + n] == 0) {
				if(trits[1 + n] == 0) {
					if(trits[0 + n] == 0) {
						ascii[i] = 'R' ;
					}
					else if(trits[0 + n] == 1) {
						ascii[i] = 'S' ;
					}
					else {
						ascii[i] = 'T' ;
					}
				}
				else if(trits[1 + n] == 1) {
					if(trits[0 + n] == 0) {
						ascii[i] = 'U' ;
					}
					else if(trits[0 + n] == 1) {
						ascii[i] = 'V' ;
					}
					else {
						ascii[i] = 'W' ;
					}
				}
				else {
					if(trits[0 + n] == 0) {
						ascii[i] = 'X' ;
					}
					else if(trits[0 + n] == 1) {
						ascii[i] = 'Y' ;
					}
					else {
						ascii[i] = 'Z' ;
					}
				}
			}
			else if(trits[2 + n] == 1) {
				if(trits[1 + n] == 0) {
					if(trits[0 + n] == 0) {
						ascii[i] = '.' ;
					}
					else if(trits[0 + n] == 1) {
						ascii[i] = ',' ;
					}
					else {
						ascii[i] = '!' ;
					}
				}
				else if(trits[1 + n] == 1) {
					if(trits[0 + n] == 0) {
						ascii[i] = '?' ;
					}
					else if(trits[0 + n] == 1) {
						ascii[i] = ':' ;
					}
					else {
						ascii[i] = ';' ;
					}
				}
				else {
					if(trits[0 + n] == 0) {
						ascii[i] = '*' ;
					}
					else if(trits[0 + n] == 1) {
						ascii[i] = '+' ;
					}
					else {
						ascii[i] = '-' ;
					}
				}
			}
			else {
				if(trits[1 + n] == 0) {
					if(trits[0 + n] == 0) {
						ascii[i] = '/' ;
					}
					else if(trits[0 + n] == 1) {
						ascii[i] = '=' ;
					}
					else {
						ascii[i] = '<' ;
					}
				}
				else if(trits[1 + n] == 1) {
					if(trits[0 + n] == 0) {
						ascii[i] = '>' ;
					}
					else if(trits[0 + n] == 1) {
						ascii[i] = '@' ;
					}
					else {
						ascii[i] = '#' ;
					}
				}
				else {
					if(trits[0 + n] == 0) {
						ascii[i] = '$' ;
					}
					else if(trits[0 + n] == 1) {
						ascii[i] = '%' ;
					}
					else {
						ascii[i] = '&' ;
					}
				}
			}
		}
        n += 4 ;
	}
	ofstream f ("C:/Users/greta/Documents/Stage/algorithme_cryptris/files/tritsToAscii.txt") ;
	if(!f.is_open()) {
		cout << "Unable to open the file !" << endl ;
	}
	else {
		f << "trits : " ;
		for(int i = 0 ;  i < (cryptSize * 4) ; i ++) {
			f << trits[i] << " " ;
		}
		f << "\nascii : " ;
		for(int j = 0 ;  j < cryptSize ; j ++) {
			f << ascii[j] ;
		}
	}
	f << endl ;
	f.close() ;
	return ascii;
}
// traduit les trits (-1, 0, 1) en symboles ASCII.

vector<int> secretKey() {
	vector<int> secret ;
	int n ;
	secret.resize(cryptSize * 4) ;
	secret[0] = randomNumber(5, 10) ;
	for(int i = 1 ; i < (cryptSize * 4) ; i ++) {

		secret[i] = randomNumber(-1, 1) ;
	}
	secret = rotation(secret) ;
	secret = changeSign(secret) ;
	ofstream f (sk.c_str()) ;
	if(!f.is_open()) {
		cout << "Unable to open the file !" << endl ;
	}
	else {
		for(int j = 0 ;  j < (cryptSize * 4) ; j ++) {
			f << secret[j] << " " ;
		}
	}
	f << endl ;
	f.close() ;
	return secret ;
}
// retourne une clé secrète sous forme de vecteur de 8 entiers.

vector<int> publicKey(vector<int> secret) {
	vector<int> alea ;
	vector<int> publicKey ;
	alea.resize(cryptSize * 4) ;
	publicKey.resize(cryptSize * 4) ;
	int n = randomNumber(2, 10) ;
	for(int i = 0 ; i < n ; i ++) {
		alea = secret ;
		alea = rotation(alea) ;
		alea = changeSign(alea) ;
		publicKey = addition(publicKey, alea) ;
	}
	ofstream f (pk.c_str()) ;
	if(!f.is_open()) {
		cout << "Unable to open the file !" << endl ;
	}
	else {
		for(int j = 0 ;  j < (cryptSize * 4) ; j ++) {
			f << publicKey[j] << " " ;
		}
	}
	f << endl ;
	f.close() ;
	return publicKey ;
}
// crée une clé publique sous forme de vecteur de 8 entiers.

vector<int> encrypt(vector<int> message, vector<int> publicKey) {
	vector<int> alea ;
	alea.resize(cryptSize * 4) ;
	alea = publicKey ;
	alea = rotation(alea) ;
	alea = changeSign(alea) ;
	message = addition(message, alea) ;
	if(find(message.begin(), message.end(), 0 ) != message.end()) {
		cout << "encrypt avec 0" << endl ;
		encrypt(message, publicKey) ;
	}
	else if(find(message.begin(), message.end(), 1 ) != message.end()) {
	cout << "encrypt avec 1" << endl ;
		encrypt(message, publicKey) ;
	}
	else if(find(message.begin(), message.end(), -1 ) != message.end()) {
	cout << "encrypt avec -1" << endl ;
		encrypt(message, publicKey) ;
	}
	cout << "encrypt bon" << endl ;
	ofstream f ("C:/Users/greta/Documents/Stage/algorithme_cryptris/files/encrypted.txt") ;
	if(!f.is_open()) {
		cout << "Unable to open the file !" << endl ;
	}
	else {
		for(int i = 0 ;  i < (cryptSize * 4) ; i ++) {
			f << message[i] << " " ;
		}
	}
	f << endl ;
	f.close() ;
	cout << "fin encrypt" << endl ;
	return message ;
}
// retourne le message chiffré sous forme de vecteur de 8 entiers.

vector<int> decrypt(vector<int> code, vector<int> secret) {
	vector<int> alea ;
	alea.resize(cryptSize * 4) ;
	bool contain = false ;
	alea = secret ;
	alea = rotation(code, alea) ;
	alea = changeSign(code, alea) ;
	code = addition(code, alea) ;
	for(int i = 0 ; i < (cryptSize * 4) ; i ++) {
		if(code[i] != 0 && code[i] != -1 && code[i] != 1) {
			contain = true ;
		}
	}
	if(contain) {
	cout << "rappel decrypt" << endl ;
		decrypt(code, secret) ;
	}
	cout << "fin   decrypt" << endl ;
	ofstream f ("C:/Users/greta/Documents/Stage/algorithme_cryptris/files/decrypted.txt") ;
	if(!f.is_open()) {
		cout << "Unable to open the file !" << endl ;
	}
	else {
		for(int i = 0 ;  i < (cryptSize * 4) ; i ++) {
			f << code[i] << " " ;
		}
	}
	f << endl ;
	f.close() ;
	return code ;
}
// retourne les deux caractères déchiffré sous forme de tableau de 8 entiers, 2 trits.

int main(void)                    // Portée principale
{
	srand(time(0)); // Initialise le générateur de nombres aléatoires avec l'horloge du système

	vector<int> s ;
	vector<int> p ;
	vector<int> m ;
	vector<int> c ;
	vector<int> d ;
	vector<char> t ;

	s = secretKey() ;
	cout << "secret key success !" << endl ;
	p = publicKey(s) ;
	cout << "public key success !" << endl ;
	m = asciiToTrits(parse()) ;
	cout << "asciiToTrits success !" << endl ;
	c = encrypt(m, p) ;
	cout << "encrypt success !" << endl ;
	d = decrypt(c, s) ;
	cout << "decrypt success !" << endl ;
	t = tritsToAscii(d) ;
	cout << "tritsToAscii success !" << endl ;
	parse(t) ;
	cout << "parse success !" << endl ;

	getch();                      // Attente d'une touche
	return 0;                     // Fin du programme
}