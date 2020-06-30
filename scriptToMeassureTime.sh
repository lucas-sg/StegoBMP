
echo "RUNNING EMBEDING METRICS ON A BIG FILE (~2MB SIZE):\n"

echo "About to run LSB1 embeding of a samplePdf (~2MB size) using LSB1 algorithm"
STARTTIME=$(($(date +%s%N)/1000000))
./main -embed -in pdfSample.pdf -p ../../Escritorio/grupo4/lima.bmp -out sampleLSB1.bmp -steg LSB1
ENDTIME=$(($(date +%s%N)/1000000))
echo "It takes $(($ENDTIME - $STARTTIME)) millis to complete LSB1 Embed of a BMP..."
echo "_________________________________________________________________________________\n"

echo "About to run LSB1 embeding of a samplePdf (~2MB size) using LSB4 algorithm"
STARTTIME=$(($(date +%s%N)/1000000))
./main -embed -in pdfSample.pdf -p ../../Escritorio/grupo4/lima.bmp -out sampleLSB4.bmp -steg LSB4
ENDTIME=$(($(date +%s%N)/1000000))
echo "It takes $(($ENDTIME - $STARTTIME)) millis to complete LSB4 Embed of a BMP..."
echo "_________________________________________________________________________________\n"

echo "About to run LSB1 embeding of a samplePdf (~2MB size) using LSBI algorithm"
STARTTIME=$(($(date +%s%N)/1000000))
./main -embed -in pdfSample.pdf -p ../../Escritorio/grupo4/lima.bmp -out sampleLSBI.bmp -steg LSBI
ENDTIME=$(($(date +%s%N)/1000000))
echo "It takes $(($ENDTIME - $STARTTIME)) millis to complete LSBI Embed of a BMP..."
echo "_________________________________________________________________________________\n"
echo "_________________________________________________________________________________\n"

echo "RUNNING EMBEDING METRICS ON A MEDIUM FILE (~900KB SIZE):\n"

echo "About to run LSB1 embeding of a samplePdf (~900KB size) using LSB1 algorithm"
STARTTIME=$(($(date +%s%N)/1000000))
./main -embed -in tests/resources/sample.bmp -p ../../Escritorio/grupo4/lima.bmp -out sampleLSB1.bmp -steg LSB1
ENDTIME=$(($(date +%s%N)/1000000))
echo "It takes $(($ENDTIME - $STARTTIME)) millis to complete LSB1 Embed of a BMP..."
echo "_________________________________________________________________________________\n"

echo "About to run LSB1 embeding of a samplePdf (~900KB size) using LSB4 algorithm"
STARTTIME=$(($(date +%s%N)/1000000))
./main -embed -in tests/resources/sample.bmp -p ../../Escritorio/grupo4/lima.bmp -out sampleLSB4.bmp -steg LSB4
ENDTIME=$(($(date +%s%N)/1000000))
echo "It takes $(($ENDTIME - $STARTTIME)) millis to complete LSB4 Embed of a BMP..."
echo "_________________________________________________________________________________\n"

echo "About to run LSB1 embeding of a samplePdf (~900KB size) using LSBI algorithm"
STARTTIME=$(($(date +%s%N)/1000000))
./main -embed -in tests/resources/sample.bmp -p ../../Escritorio/grupo4/lima.bmp -out sampleLSBI.bmp -steg LSBI
ENDTIME=$(($(date +%s%N)/1000000))
echo "It takes $(($ENDTIME - $STARTTIME)) millis to complete LSBI Embed of a BMP..."
echo "_________________________________________________________________________________\n"
echo "_________________________________________________________________________________\n"

echo "RUNNING EMBEDING METRICS ON A SMALL FILE (~30KB SIZE):\n"

echo "About to run LSB1 embeding of a sampleTxt (~30KB size) using LSB1 algorithm"
STARTTIME=$(($(date +%s%N)/1000000))
./main -embed -in tests/resources/loremIpsum.txt -p ../../Escritorio/grupo4/lima.bmp -out sampleLSB1.bmp -steg LSB1
ENDTIME=$(($(date +%s%N)/1000000))
echo "It takes $(($ENDTIME - $STARTTIME)) millis to complete LSB1 Embed of a BMP..."
echo "_________________________________________________________________________________\n"

echo "About to run LSB1 embeding of a sampleTxt (~30KB size) using LSB4 algorithm"
STARTTIME=$(($(date +%s%N)/1000000))
./main -embed -in tests/resources/loremIpsum.txt -p ../../Escritorio/grupo4/lima.bmp -out sampleLSB4.bmp -steg LSB4
ENDTIME=$(($(date +%s%N)/1000000))
echo "It takes $(($ENDTIME - $STARTTIME)) millis to complete LSB4 Embed of a BMP..."
echo "_________________________________________________________________________________\n"

echo "About to run LSB1 embeding of a sampleTxt (~30KB size) using LSBI algorithm"
STARTTIME=$(($(date +%s%N)/1000000))
./main -embed -in tests/resources/loremIpsum.txt -p ../../Escritorio/grupo4/lima.bmp -out sample.bmp -steg LSBI
ENDTIME=$(($(date +%s%N)/1000000))
echo "It takes $(($ENDTIME - $STARTTIME)) millis to complete LSBI Embed of a BMP..."
echo "_________________________________________________________________________________\n"

