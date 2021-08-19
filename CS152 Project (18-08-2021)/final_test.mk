Medical_Inventory.exe : final_test.o medical_h.o
	gcc final_test.o medical_h.o -o Medical_Inventory

final_test.o : final_test.c medical_h.h
	gcc -c final_test.c 

medical_h.o : medical_h.c medical_h.h
	gcc -c medical_h.c 