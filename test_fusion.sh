#!/bin/bash

#fusion réussi
touch Test/fusion_reussi.txt
# ./fusion Test/file1.o Test/file2.o Test/test.o
arm-none-eabi-ld -r -o Test/fusion/fusion_eabi.o Test/fusion/file1.o Test/fusion/file2.o 2>error.txt
./phase2 Test/fusion/file1.o Test/fusion/file2.o Test/fusion/fusion_projet.o 2>error.txt
echo "******************************header******************************" > Test/fusion_reussi.txt
echo "Version arm-none-eabi-" >> Test/fusion_reussi.txt
./phase1 -h Test/fusion/fusion_eabi.o >> Test/fusion_reussi.txt
echo "Version ./fusion" >> Test/fusion_reussi.txt
./phase1 -h Test/fusion/fusion_projet.o >> Test/fusion_reussi.txt
echo "" >> Test/fusion_reussi.txt
echo "******************************section header******************************" >> Test/fusion_reussi.txt
echo "Version arm-none-eabi-" >> Test/fusion_reussi.txt
./phase1 -S Test/fusion/fusion_eabi.o >> Test/fusion_reussi.txt
echo "Version ./fusion" >> Test/fusion_reussi.txt
./phase1 -S Test/fusion/fusion_projet.o >> Test/fusion_reussi.txt
echo "" >> Test/fusion_reussi.txt
echo "******************************symbole******************************" >> Test/fusion_reussi.txt
echo "Version arm-none-eabi-" >> Test/fusion_reussi.txt
./phase1 -s Test/fusion/fusion_eabi.o >> Test/fusion_reussi.txt
echo "Version ./fusion" >> Test/fusion_reussi.txt
./phase1 -s Test/fusion/fusion_projet.o >> Test/fusion_reussi.txt
echo "" >> Test/fusion_reussi.txt
echo "******************************table des reimplantation******************************" >> Test/fusion_reussi.txt
echo "Version arm-none-eabi-" >> Test/fusion_reussi.txt
./phase1 -r Test/fusion/fusion_eabi.o >> Test/fusion_reussi.txt
echo "Version ./fusion" >> Test/fusion_reussi.txt
./phase1 -r Test/fusion/fusion_projet.o >> Test/fusion_reussi.txt
echo "" >> Test/fusion_reussi.txt
echo "******************************Génération de l'éxécutable et éxecution******************************" >> Test/fusion_reussi.txt
echo "Version arm-none-eabi" >> Test/fusion_reussi.txt
arm-none-eabi-gcc --specs=rdimon.specs -Wl,--start-group -lgcc -lc -lm -lrdimon -Wl,--end-group -o Test/fusion/prog Test/fusion/fusion_eabi.o && arm-none-eabi-run ./Test/fusion/prog >> Test/fusion_reussi.txt
echo "Version ./fusion" >> Test/fusion_reussi.txt
arm-none-eabi-gcc --specs=rdimon.specs -Wl,--start-group -lgcc -lc -lm -lrdimon -Wl,--end-group -o Test/fusion/test Test/fusion/fusion_projet.o && arm-none-eabi-run ./Test/fusion/test >> Test/fusion_reussi.txt



#fusion réussi avec mult1.o
touch Test/fusion_reussi_mult.txt
# ./fusion Test/file1.o Test/file2.o Test/test.o
arm-none-eabi-ld -r -o Test/fusion/fusion_eabi2.o Test/fusion/file2.o Test/fusion/mult1.o 2>error.txt
./phase2 Test/fusion/file2.o Test/fusion/mult1.o Test/fusion/fusion_projet2.o 2>error.txt
echo "******************************header******************************" > Test/fusion_reussi_mult.txt
echo "Version arm-none-eabi-" >> Test/fusion_reussi_mult.txt
./phase1 -h Test/fusion/fusion_eabi2.o >> Test/fusion_reussi_mult.txt
echo "Version ./fusion" >> Test/fusion_reussi_mult.txt
./phase1 -h Test/fusion/fusion_projet2.o >> Test/fusion_reussi_mult.txt
echo "" >> Test/fusion_reussi_mult.txt
echo "******************************section header******************************" >> Test/fusion_reussi_mult.txt
echo "Version arm-none-eabi-" >> Test/fusion_reussi_mult.txt
./phase1 -S Test/fusion/fusion_eabi2.o >> Test/fusion_reussi_mult.txt
echo "Version ./fusion" >> Test/fusion_reussi_mult.txt
./phase1 -S Test/fusion/fusion_projet2.o >> Test/fusion_reussi_mult.txt
echo "" >> Test/fusion_reussi_mult.txt
echo "******************************symbole******************************" >> Test/fusion_reussi_mult.txt
echo "Version arm-none-eabi-" >> Test/fusion_reussi_mult.txt
./phase1 -s Test/fusion/fusion_eabi2.o >> Test/fusion_reussi_mult.txt
echo "Version ./fusion" >> Test/fusion_reussi_mult.txt
./phase1 -s Test/fusion/fusion_projet2.o >> Test/fusion_reussi_mult.txt
echo "" >> Test/fusion_reussi_mult.txt
echo "******************************table des reimplantation******************************" >> Test/fusion_reussi_mult.txt
echo "Version arm-none-eabi-" >> Test/fusion_reussi_mult.txt
./phase1 -r Test/fusion/fusion_eabi2.o >> Test/fusion_reussi_mult.txt
echo "Version ./fusion" >> Test/fusion_reussi_mult.txt
./phase1 -r Test/fusion/fusion_projet2.o >> Test/fusion_reussi_mult.txt
echo "" >> Test/fusion_reussi_mult.txt
echo "******************************Génération de l'éxécutable et éxecution******************************" >> Test/fusion_reussi_mult.txt
echo "Version arm-none-eabi" >> Test/fusion_reussi_mult.txt
arm-none-eabi-gcc --specs=rdimon.specs -Wl,--start-group -lgcc -lc -lm -lrdimon -Wl,--end-group -o Test/fusion/prog Test/fusion/fusion_eabi.o && arm-none-eabi-run ./Test/fusion/prog 2>> Test/fusion_reussi_mult.txt >> Test/fusion_reussi_mult.txt
echo "Version ./fusion" >> Test/fusion_reussi.txt
arm-none-eabi-gcc --specs=rdimon.specs -Wl,--start-group -lgcc -lc -lm -lrdimon -Wl,--end-group -o Test/fusion/test Test/fusion/fusion_projet.o && arm-none-eabi-run ./Test/fusion/test 2>> Test/fusion_reussi._mult.txt >> Test/fusion_reussi_mult.txt






#Fichier non translatable
touch Test/fusion_non_translatable.txt
echo "Version arm-none-eabi :" > Test/fusion_non_translatable.txt
arm-none-eabi-ld -r -o Test/fusion/fusion_nontranslatable_eabi.o Test/fusion/file2.o Test/fusion/exec 2>> Test/fusion_non_translatable.txt >> Test/fusion_non_translatable.txt
echo "Version ./fusion :" >> Test/fusion_non_translatable.txt
./phase2 Test/fusion/file2.o Test/fusion/exec Test/fusion/nontranslatable_projet.o 2>> Test/fusion_non_translatable.txt >> Test/fusion_non_translatable.txt



#Fusion problèmes de symboles
touch Test/fusion_echoue_symbole.txt
echo "Version arm-none-eabi :" > Test/fusion_echoue_symbole.txt
arm-none-eabi-ld -r -o Test/fusion/echoue_eabi.o Test/fusion/file1.o Test/fusion/file1.o 2>> Test/fusion_echoue_symbole.txt
echo "Version ./fusion :" >> Test/fusion_echoue_symbole.txt
./phase2 Test/fusion/file1.o Test/fusion/file1.o Test/echoue_projet.o 2>> Test/fusion_echoue_symbole.txt






#fusion réussi avec mult1.o et mult2.o
touch Test/fusion_reussi_mult2.txt
# ./fusion Test/file1.o Test/file2.o Test/test.o
arm-none-eabi-ld -r -o Test/fusion/fusion_eabi2.o Test/fusion/mult2.o Test/fusion/mult1.o 2>error.txt
./phase2 Test/fusion/mult2.o Test/fusion/mult1.o Test/fusion/fusion_projet2.o 2>error.txt
echo "******************************header******************************" > Test/fusion_reussi_mult2.txt
echo "Version arm-none-eabi-" >> Test/fusion_reussi_mult2.txt
./phase1 -h Test/fusion/fusion_eabi2.o >> Test/fusion_reussi_mult2.txt
echo "Version ./fusion" >> Test/fusion_reussi_mult2.txt
./phase1 -h Test/fusion/fusion_projet2.o >> Test/fusion_reussi_mult2.txt
echo "" >> Test/fusion_reussi_mult2.txt
echo "******************************section header******************************" >> Test/fusion_reussi_mult2.txt
echo "Version arm-none-eabi-" >> Test/fusion_reussi_mult2.txt
./phase1 -S Test/fusion/fusion_eabi2.o >> Test/fusion_reussi_mult2.txt
echo "Version ./fusion" >> Test/fusion_reussi_mult2.txt
./phase1 -S Test/fusion/fusion_projet2.o >> Test/fusion_reussi_mult2.txt
echo "" >> Test/fusion_reussi_mult2.txt
echo "******************************symbole******************************" >> Test/fusion_reussi_mult2.txt
echo "Version arm-none-eabi-" >> Test/fusion_reussi_mult2.txt
./phase1 -s Test/fusion/fusion_eabi2.o >> Test/fusion_reussi_mult2.txt
echo "Version ./fusion" >> Test/fusion_reussi_mult2.txt
./phase1 -s Test/fusion/fusion_projet2.o >> Test/fusion_reussi_mult2.txt
echo "" >> Test/fusion_reussi_mult2.txt
echo "******************************table des reimplantation******************************" >> Test/fusion_reussi_mult2.txt
echo "Version arm-none-eabi-" >> Test/fusion_reussi_mult2.txt
./phase1 -r Test/fusion/fusion_eabi2.o >> Test/fusion_reussi_mult2.txt
echo "Version ./fusion" >> Test/fusion_reussi_mult2.txt
./phase1 -r Test/fusion/fusion_projet2.o >> Test/fusion_reussi_mult2.txt
echo "" >> Test/fusion_reussi_mult2.txt
echo "******************************Génération de l'éxécutable et éxecution******************************" >> Test/fusion_reussi_mult2.txt
echo "Version arm-none-eabi" >> Test/fusion_reussi_mult2.txt
arm-none-eabi-gcc --specs=rdimon.specs -Wl,--start-group -lgcc -lc -lm -lrdimon -Wl,--end-group -o Test/fusion/prog Test/fusion/fusion_eabi.o && arm-none-eabi-run ./Test/fusion/prog 2>> Test/fusion_reussi_mult2.txt >> Test/fusion_reussi_mul2t.txt
echo "Version ./fusion" >> Test/fusion_reussi.txt
arm-none-eabi-gcc --specs=rdimon.specs -Wl,--start-group -lgcc -lc -lm -lrdimon -Wl,--end-group -o Test/fusion/test Test/fusion/fusion_projet.o && arm-none-eabi-run ./Test/fusion/test 2>> Test/fusion_reussi._mult2.txt >> Test/fusion_reussi_mult2.txt
