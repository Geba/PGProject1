#include <iostream>

using namespace std;

double fatorial(double k){
		if(k == 0){
			return 1;
		} else {
			return k *fatorial(k-1);
		}
	}

	double combinacao(double n, double p){
		if(p > n){
			return 0;
		} else {
		double c = fatorial(n)/(fatorial(p)*fatorial(n-p));
		return c;
		}
	}



int main(){

		cout << "Digite o grau da curva: ";
		int grau;
		cin >> grau;
		cout << "Grau = " << grau;
		double coef[grau+1][grau+1];
		int aux = grau;


		//for de todos os b's
		for (int i = 0; i < grau+1; i++) {
			if(i > 0){
				aux = aux- 1;
			}
			cout << endl;
			//preenchendo as linhas ou seja b0, b1...
			for (int j = 0; j < grau + 1; j++) {
				coef[i][j] = combinacao(aux, j);
				cout << coef[i][j] << " ";
			}
		}

        double trans[grau+1][grau+1];
		aux = grau;
		for (int i = 0; i < grau + 1; i++) {
			for (int j = 0; j < grau + 1; j++) {
				trans[aux-j][i] = coef[i][j];
			}
		}

		return 0;
	}
