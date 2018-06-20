#include "Input.h"



Input::Input(std::string hashID_, long unsigned index_, std::vector<byte> signature_) : hashID(hashID_), index(index_), signature(signature_)
{
}


Input::~Input()
{
}
