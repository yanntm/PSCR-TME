#include "List.h"
namespace pr {

// ******************* Chainon

Chainon::Chainon (const std::string & data, Chainon * next):data(data),next(next) {}

size_t Chainon::length() {
    
    if(next == nullptr){
        return 1;
    }
    
    return (1+ next->length());
    
}

void Chainon::print (std::ostream & os) const {
    os << data ;
    if (next == nullptr) {
        return;
    }
    
    os << ", ";
    next->print(os);
}

// ******************  List
const std::string & List::operator[] (size_t index) const  {
    Chainon * it = tete;
    for (size_t i=0; i < index ; i++) {
        it = it->next;
    }
    return it->data;
}

void List::push_back (const std::string& val) {
    if (tete == nullptr) {
        tete = new Chainon(val);
    } else {
        Chainon * fin = tete;
        while (fin->next) {
            fin = fin->next;
        }
        fin->next = new Chainon(val);
    }
}

void List::push_front (const std::string& val) {
    tete = new Chainon(val,tete);
}

bool List::empty() {
    return tete == nullptr;
}

size_t List::size() const {
    if (tete == nullptr) {
        return 0;
    } else {
        return tete->length();
    }
}

//}

std::ostream & operator<< (std::ostream & os, const pr::List & vec)
{
    os << "[";
    if (vec.tete != nullptr) {
        vec.tete->print (os) ;
    }
    os << "]";
    return os;
}
} // namespace pr
