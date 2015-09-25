#include "Flag.h"

Flag::Flag(){

}

void Flag::set(){
	this->value = 1;
}
void Flag::clear(){
	this->value = 0;
}

uint8_t Flag::get(){
	if (this->value >= 1)
		return this->value;
	else
		return 0;
}