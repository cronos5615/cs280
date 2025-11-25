#include "val.h"


int main(){

}

Value Value::operator*(const Value& op) const {
    double first,second;
    switch(this->T){
        case VNUM:
            first = this->Ntemp;
            break;
        case VSTRING:
            try{
                first = stod(this->Stemp);
            }
            catch(invalid_argument){
                return Value();
            }
        case VBOOL:

        default:
            return Value();
    }

}
Value Value::operator<(const Value& op) const{

}
Value Value::Catenate(const Value& oper) const{

}
Value Value::Repeat(const Value& oper) const{

}
Value Value::SEQ(const Value& oper) const{

}
