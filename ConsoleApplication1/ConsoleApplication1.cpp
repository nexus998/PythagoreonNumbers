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

	//1 task. Find all pythagoreon numbers until 1000 (in Assembly)
	cout << "Getting pythagoreon numbers..." << "\n";

	ofstream o;
	o.open("output.txt");

	for (int i = 1; i <= 1000 - 5; i+=4) {
		for (int j = i + 1; j <= 1000 - 4; j++) {

			__declspec(align(16))float firstNumbers[4];
			__declspec(align(16))float otherNumbers[4];
			__declspec(align(16))float res[4];

			float* firstNumbersPointer = firstNumbers;
			float* otherNumbersPointer = otherNumbers;
			float* resPointer = res;

			firstNumbers[0] = i;
			firstNumbers[1] = i + 1;
			firstNumbers[2] = i + 2;
			firstNumbers[3] = i + 3;

			otherNumbers[0] = j;
			otherNumbers[1] = j + 1;
			otherNumbers[2] = j + 2;
			otherNumbers[3] = j + 3;

			__asm {
				// xmm0 take the first 4 numbers
				mov eax, firstNumbersPointer
				movaps xmm0, [eax]

				// xmm1 take the other 4 nums
				mov eax, otherNumbersPointer
				movaps xmm1, [eax]

				//// xmm2 keliam kvadratu visus skaicius
				movaps xmm2, xmm0
				mulps xmm2, xmm0
				movaps xmm3, xmm1
				mulps xmm3, xmm1

				// xmm4 sudedam pakeltus sk
				movaps xmm4, xmm2
				addps xmm4, xmm3

				// xmm5 traukiam sakni is sumos
				sqrtps xmm5, xmm4
				// xmm5 apvalinam iki int 
				roundps xmm5, xmm5, 1

				// xmm5 keliam kvadratu
				mulps xmm5, xmm5

				// xmm5 randam skirtuma
				subps xmm5, xmm4

				// result
				mov eax, resPointer
				movaps[eax], xmm5
			}

			for (int k = 0; k < 4; k++) {
				if (res[k] == 0) {
					o << int(firstNumbers[k]) << " " << int(otherNumbers[k]) << " " << sqrt(int(firstNumbers[k]) * int(firstNumbers[k]) + int(otherNumbers[k]) * int(otherNumbers[k])) << endl;
				}
			}
		}
	}

	o.close();
	cout << "Finished!" << endl;
	return 0;
}
