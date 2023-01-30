#include <stdio.h>
#include <vector>
#include <iostream>
#include <chrono>
#include <ctime>
#include <string>

using namespace std;

int main() {

// Método 1
  auto num = 380000;
  auto soma = 3*(num/10000000) + (num/1000000 - (num/10000000)*10) + 7*(num/100000 - (num/1000000)*10) +  3*(num/10000 - (num/100000)*10) +  (num/1000 - (num/10000)*10) + (num/100 - (num/1000)*10) + 7*(num/10 - (num/100)*10) + 3*(num - (num/10)*10);
  auto calculaDigito = (10 - (soma % 10)) % 10;
  cout << "Dígito = " << calculaDigito;


// Método 2

  auto valor = "003800005";
  auto numero = valor;
  auto conta =  numero.substring(0, 1) * 3 +
      numero.substring(1, 2) * 1 +
      numero.substring(2, 3) * 7 +
      numero.substring(3, 4) * 3 +
      numero.substring(4, 5) * 1 +
      numero.substring(5, 6) * 1 +
      numero.substring(6, 7) * 7 +
      numero.substring(7, 8) * 3; 
  auto digito = (conta % 10 == 0) ? 0 : 10 - (conta % 10);
  cout << "Dígito é: " << digito
}