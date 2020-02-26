#include"figure.h"
#include"Field.h"
#include"sprite.h"
#include<stdlib.h>

Plane field[1];

void FieldInit() {
	field[0].position = D3DXVECTOR3(8, 0, 5);
	field[0].rotation = D3DXVECTOR3(0, 0, 0);
	field[0].scale = D3DXVECTOR3(100, 0, 100);
}
void FieldDraw() {
	field[0].Draw(TEXTURE_INDEX_FIELD, 255);
}
void FieldUnInit() {

}