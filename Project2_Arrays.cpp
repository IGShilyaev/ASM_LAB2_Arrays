
#include <iostream>

using namespace std;

int main()
{
 
	const short length1 = 5;
	const short length2 = 7;
	

	short arr1[length1];
	short arr2[length2];
	short searchElement;
	short searchRes[2];
	
	
	cout << "Array 1:\n";
	for (int i = 0; i < length1; i++)
	{
		cout << "Element " << i + 1 <<": ";
		cin >> arr1[i];
	}

	cout << "Array 2:\n";
	for (int i = 0; i < length2; i++)
	{
		cout << "Element " << i + 1 << ": ";
		cin >> arr2[i];
	}

	cout << "Look for: ";
	cin >> searchElement;
	cout << "\n";

	short lengthUnion = length1;
	for (int i = 0; i < length2; i++)
	{
		if (find(arr1, arr1 + length1, arr2[i]) == arr1 + length1) lengthUnion++;
	}
	short unionRes[100];

	short lengthIntersect = 0;
	for (int i = 0; i < length1; i++) 
	{
		if (find(arr2, arr2 + length2, arr1[i]) != arr2 + length2) lengthIntersect++;
	}
	short interRes[100];
	
	short lengthDiff = length1;
	for (int i = 0; i < length1; i++)
	{
		if (find(arr2, arr2 + length2, arr1[i]) != arr2 + length2) lengthDiff--;
	}
	short diffRes[100];
	

	__asm
	{
		lea ebx, arr1
		CALL firstArrSearch
		
		xor ebx, ebx
		lea ebx, arr2
		CALL secondArrSearch

		xor ebx,ebx
		xor eax,eax
		lea eax, unionRes
		lea ebx, arr1
		CALL Union

		xor ebx,ebx
		xor edx,edx
		lea ebx, arr2
		lea edx, interRes
		CALL FindIntersec

		jmp fullFinish

		
		//______________firstArrSearch - Begin__________________
#pragma region firstSearch
		firstArrSearch:		// Поиск элемента в первом массиве
							// Адрес начала массива в ebx
							// В конце позиция элемента в массиве записывается в первый элемент searchRes (счет с 1)
							// Если элемент не был найден - записывается 0
		push ax
		push cx
		push dx
		mov dx, 1
		mov ax, searchElement
		mov cx, length1
		push cx
		
	search1:
		cmp ax, [ebx]
		jz numberEntry1
		inc dx
		dec cx
		add ebx, 2
		and cx,cx
		jnz search1 
		xor dx, dx
		jmp numberEntry1

	numberEntry1:
		xor ebx, ebx
		lea ebx, searchRes
		mov [ebx], dx
		jmp procFin1

		procFin1:
		pop cx
		mov length1, cx
		pop dx
		pop cx
		pop ax
		RET
#pragma endregion
		//______________firstArrSearch - END_________________
		

		//______________secondArrSearch - Begin_____________________
#pragma region secondSearch
		secondArrSearch:	// Поиск элемента во втором массиве
							// Адрес начала массива в ebx
							// В конце позиция элемента в массиве записывается во второй элемент searchRes (счет с 1)
							// Если элемент не был найден - записывается 0
			push ax
			push cx
			push dx
			mov dx, 1
			mov ax, searchElement
			mov cx, length2
			push cx

		search2 :
			cmp ax, [ebx]
			jz numberEntry2
			inc dx
			dec cx
			add ebx, 2
			and cx, cx
			jnz search2
			xor dx, dx
			jmp numberEntry2

		numberEntry2 :
			xor ebx, ebx
			lea ebx, searchRes
			add ebx, 2
			mov[ebx], dx
			jmp procFin2

		procFin2 :
			pop cx
			mov length2, cx
			pop dx
			pop cx
			pop ax
			RET
#pragma endregion
		//______________secondArrSearch - END_______________________


			//______________Union - Begin_________________
#pragma region Union

		Union:		   //Операция объединения множеств
					   //На старте первый одномерный массив в ebx, unionRes в eax
					   //Элементы, входящие в объединение, записываются в unionRes
		push cx
		push dx
		xor dx, dx
		mov cx, length1
		push cx

		Fill1:
		mov dx, [ebx]
		mov [eax], dx
		add eax, 2
		add ebx, 2
		dec cx
		and cx,cx
		jnz Fill1

		pop cx
		mov length1, cx

		xor ebx, ebx
		lea ebx, arr2
		mov cx, length2
		push cx

		Fill2:
		CALL ContainsCheck
		and si, si
		jnz skipadd
		
		mov dx, [ebx]
		mov [eax], dx
		add eax, 2
		
		skipadd:
		add ebx, 2
		dec cx
		and cx, cx
		jnz Fill2

		pop cx
		mov length2, cx
		pop dx
		pop cx
		RET

#pragma endregion
			//______________Union - END__________________


			//______________ContainsCheck - Begin________________
#pragma region ContainsCheck
			ContainsCheck: //Проверка на наличие элемента массива в ebx в массиве arr1
						   //Если элемент обнаружен, в si заносится 1. Иначе заносится 0
			push eax
			push dx
			push cx
			mov dx, [ebx]
			xor eax, eax
			lea eax, arr1
			
			comparisons:
			cmp dx, [eax]
			jz oneres

			add eax, 2
			dec cx
			and cx, cx
			jnz comparisons
			jmp zerores
			
			zerores:
			xor si, si
			jmp finishcheck

			oneres:
			mov si, 1
			jmp finishcheck

			finishcheck:
			pop cx
			pop dx
			pop eax
			RET
#pragma endregion
			//______________ContainsCheck - END


			//______________ FindInersec - Begin_________________
#pragma region Intersection
				// Операция пересечения множеств
				// На входе введенные массивы в eax и в ebx;
				// Результаты записываются в массив в edx
		FindIntersec:
			push cx
			push dx
			
			mov cx, length2
			push cx
			
			InterFill:
			CALL ContainsCheck
			and si,si
			jz skipIntersec
			mov di, [ebx]
			mov [edx], di
			add ed=x, 2

			skipIntersec:
			add ebx, 2
			dec cx
			and cx, cx
			jnz InterFill

			pop cx
			mov length2, cx
			pop dx
			pop cx

		RET

#pragma endregion
			//______________ FindInersec - END_________________


	fullFinish:
	}

	cout << searchRes[0] << '\n' << searchRes[1] << endl;
	for (int i = 0; i < lengthUnion; i++)
		cout << unionRes[i] << ' ';
	cout << endl;
	for (int i = 0; i < lengthIntersect; i++)
		cout << interRes[i] << ' ';
	cout << endl;
	for (int i = 0; i < lengthDiff; i++)
		cout << interRes[i] << ' ';
	cout << endl;
	return 0;
}

