# TP Criptografía y seguridad: Esteganografía
StegoBMP es una aplicación que permite tanto ocultar archivos en imágenes BMP como extraerlos. Para esto hay tres métodos de esteganografía implementados: LSB1, LSB4 y LSBI. Cada uno embebe información en los bits menos significativos de cada byte del archivo portador, de ahí el acrónimo Least Significant Bit o LSB.

* __LSB1:__ Remplaza el LSB de cada byte del portador por uno del archivo a ocultar. Para esto se requiere que el tamaño del archivo portador sea mayor a 8 veces el tamaño del archivo a ocultar.
* __LSB4:__ Reemplaza los 4 LSB de cada byte del portador por 4 del archivo a ocultar. Para esto se requiere que el tamaño del archivo portador sea mayor a 2 veces el tamaño del archivo a ocultar.
* __LSBI:__ Similar a LSB1, sólo que en este caso los bytes a reemplazar en el portador no son contiguos sino que están separados por un "salto". Además en LSBI se cifra el mensaje con el cifrado de flujo RC4 antes de ser embebido. 

## Build StegoBMP:
Para buildear el proyecto ejecutar `make stegobmp`

## Build tests:
Para buildear la suite de tests ejecutar `make testSuite`
