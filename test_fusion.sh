#!/bin/bash

#fusion réussi
touch Test/fusion_reussi.txt
# ./fusion Test/file1.o Test/file2.o Test/test.o
arm-none-eabi-ld -r -o Test/test.o Test/file1.o Test/prog.o
echo "header" > fusion_reussi.txt
echo "Version arm-none-eabi-" >> fusion_reussi.txt
readelf -h Test/prog.o >> fusion_reussi.txt
echo "Version ./fusion" >> fusion_reussi.txt
readelf -h Test/test.o >> fusion_reussi.txt
echo "" >> fusion_reussi.txt
echo "section header" >> fusion_reussi.txt
echo "Version arm-none-eabi-" >> fusion_reussi.txt
readelf -S Test/prog.o >> fusion_reussi.txt
echo "Version ./fusion" >> fusion_reussi.txt
readelf -S Test/test.o >> fusion_reussi.txt
echo "" >> fusion_reussi.txt
echo "symbole" >> fusion_reussi.txt
echo "Version arm-none-eabi-" >> fusion_reussi.txt
readelf -s Test/prog.o >> fusion_reussi.txt
echo "Version ./fusion" >> fusion_reussi.txt
readelf -s Test/test.o >> fusion_reussi.txt
echo "" >> fusion_reussi.txt
echo "table des reimplantation" >> fusion_reussi.txt
echo "Version arm-none-eabi-" >> fusion_reussi.txt
readelf -r Test/prog.o >> fusion_reussi.txt
echo "Version ./fusion" >> fusion_reussi.txt
readelf -r Test/test.o >> fusion_reussi.txt
echo "" >> fusion_reussi.txt
echo "Génération de l'éxécutable et éxecution" >> fusion_reussi.txt
echo "Version arm-none-eabi"
amm-none-eabi-gcc --specs=rdimon.specs -Wl,--start-group -lgcc -lc -lm -lrdimon -Wl,--end-group -o Test/prog Test/prog.o && arm-none-eabi-run ./Test/prog 2>> fusion_reussi.txt
echo "Version ./fusion" >> fusion.txt
# amm-none-eabi-gcc --specs=rdimon.specs -Wl,--start-group -lgcc -lc -lm -lrdimon -Wl,--end-group -o Test/test Test/test.o && arm-none-eabi-run ./Test/test 2>> fusion.txt



#Fichier non translatable
touch Test/fusion_non_translatable.txt
echo "Version arm-none-eabi :" > Test/fusion_non_translatable.txt
arm-none-eabi-ld -r -o Test/fusion.o Test/file2.o Test/fileEXEC 2>> Test/fusion_non_translatable.txt
echo "Version ./fursion :"
./fusion Test/file2.o Test/fileEXEC Test/test.o 2>> Test/fusion_non_translatable.txt



#Fusion problèmes de symboles
touch Test/fusion_echoue_symbole.txt
echo "Version arm-none-eabi :" > Test/fusion_echoue_symbole.txt
arm-none-eabi-ld -r -o Test/fusion.o Test/file1.o Test/file1.o 2>> Test/fusion_echoue_symbole.txt
echo "Version ./fursion :"
./fusion Test/file1.o Test/file1.o Test/test.o 2>> Test/fusion_echoue_symbole.txt
