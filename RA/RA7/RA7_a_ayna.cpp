#include "val.h"
bool getNumericVal(double& ref,const Value& val){
    switch(val.GetType()){
        case VNUM:
            ref = val.GetNum();
            break;
        case VSTRING:
            try{
                ref = stod(val.GetString());
            }
            catch(const invalid_argument&){
                return false;
            }
            break;
        default:
            return false;
    }
    return true;
}
bool getStringVal(string& ref, const Value& val){
    ostringstream oss;
    switch(val.GetType()){
        case VNUM:
            oss << val.GetNum();
            ref = oss.str();
            break;
        case VSTRING:
            ref = val.GetString();
            break;
        default:
            return false;
    }
    return true;
}

Value Value::operator*(const Value& op) const {
    double first,second;
    if(!getNumericVal(first,*this) || !getNumericVal(second,op)){
        return Value();
    }
    return Value(first*second);
}

Value Value::operator<(const Value& op) const{
    double first,second;
    if(!getNumericVal(first,*this) || !getNumericVal(second,op)){
        return Value();
    }
    return Value(first < second);
}
Value Value::Catenate(const Value& oper) const{
    string first,second;
    if(!getStringVal(first,*this) || !getStringVal(second,oper)){
        return Value();
    }
    return Value(first+second);
}
Value Value::Repeat(const Value& oper) const{
    string first,result = "";
    int second;
    double temp;
    if(!getStringVal(first,*this) || !getNumericVal(temp,oper))
        return Value();
    second = temp;
    for (int i = 0; i < second; i++)
        result += first;
    return Value(result);
}
Value Value::SEQ(const Value& oper) const{
    string first,second;
    if(!getStringVal(first,*this) || !getStringVal(second,oper)){
        return Value();
    }
    return Value(first == second);
}

