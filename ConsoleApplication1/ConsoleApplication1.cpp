#include "pch.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <math.h>
using namespace std;

int main(int argc, char* argv[])
{
	//1 task. Convert string number value to an integer (in Assembly)
	char *inp = argv[1];
	int dydis = static_cast<int>(strlen(argv[1]));
	
	cout << dydis << "\n";

	int answer = 0;

	//ecx - input length
	//edx - answer
	_asm
	{
		push ecx
		push eax
		push edx
		push esi

		mov ecx, dydis
		mov esi, inp

		xor edx, edx
		xor eax, eax

		counter :
		imul edx, 10 // Multiply prev digits by 10
			mov al, [esi] // Load next char to al
			sub al, 48 // Convert to number
			add edx, eax // Add new number
			inc esi
			loop counter //Move to next digit. loops until reaches ecx value

			//edx now contains the result
			mov[answer], edx

			pop ecx
			pop eax
			pop edx
			pop esi
	}
	cout << answer << "\n";
	cout << "Getting pythagoreon numbers..." << "\n";

	ofstream o;
	o.open("output.txt");

	for (int i = 1; i <= 1000 - 4 - 1; i++) {
		for (int j = i + 1; j <= 1000 - 4; j++) {

			__declspec(align(16))float a[4];
			__declspec(align(16))float b[4];
			__declspec(align(16))float res[4];

			float* aptr = a;
			float* bptr = b;
			float* resptr = res;

			a[0] = i + 1;
			a[1] = i + 2;
			a[2] = i + 3;
			a[3] = i + 4;

			b[0] = j + 1;
			b[1] = j + 2;
			b[2] = j + 3;
			b[3] = j + 4;

			__asm {
				// xmm0 pirmi keturi skaiciai
				mov eax, aptr
				movaps xmm0, [eax]

				// xmm1 kiti keturi skaiciai
				mov eax, bptr
				movaps xmm1, [eax]

				//// xmm2 pirmu skaiciu kelimas kvadratu
				movaps xmm2, xmm0
				mulps xmm2, xmm0

				// xmm3 kitu keturiu skaiciu kelimas kvadratu
				movaps xmm3, xmm1
				mulps xmm3, xmm1

				// xmm4 kvadratu suma
				movaps xmm4, xmm2
				addps xmm4, xmm3

				// xmm5 saknis istraukta is kvadratu sumos
				sqrtps xmm5, xmm4

				// xmm5 sveikoji dalis is saknies traukimo
				roundps xmm5, xmm5, 1

				// xmm5 kvadratu suma sveikosios dalies
				mulps xmm5, xmm5

				// xmm5 skirtumas tarp xmm5 ir xmm4
				subps xmm5, xmm4

				// result
				mov eax, resptr
				movaps[eax], xmm5
			}

			for (int k = 0; k < 4; k++) {
				if (res[k] == 0) {
					o << int(a[k]) << " " << int(b[k]) << " " << int(a[k]) * int(a[k]) + int(b[k]) * int(b[k]) << endl;
				}
			}
		}
	}

	o.close();
	cout << "Finished!" << endl;
	return 0;
}
