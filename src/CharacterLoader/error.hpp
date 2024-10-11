//
// Created by PinkySmile on 09/10/24.
//

#ifndef CHARACTERENGINE_ERROR_HPP
#define CHARACTERENGINE_ERROR_HPP


#include <string>

std::string GetLastErrorAsString(DWORD errorMessageID = GetLastError());
std::wstring GetLastErrorAsWString(DWORD errorMessageID = GetLastError());



#endif //CHARACTERENGINE_ERROR_HPP
